/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil -*- */
/**
 * Copyright (C) 2013-2014 Regents of the University of California.
 * @author: Yingdi Yu <yingdi@cs.ucla.edu>
 * @author: Jeff Thompson <jefft0@remap.ucla.edu>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version, with the additional exemption that
 * compiling, linking, and/or using OpenSSL is allowed.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * A copy of the GNU Lesser General Public License is in the file COPYING.
 */

#include "../util/logging.hpp"
#include <ndn-cpp/security/security-exception.hpp>
#include <ndn-cpp/security/policy/policy-manager.hpp>
#include <ndn-cpp/security/key-chain.hpp>
#include <ndn-cpp/security/policy/no-verify-policy-manager.hpp>
#include <ndn-cpp/sha256-with-rsa-signature.hpp>

INIT_LOGGER("ndn.KeyChain");

using namespace std;
using namespace ndn::func_lib;

namespace ndn {

KeyChain::KeyChain
  (const ptr_lib::shared_ptr<IdentityManager>& identityManager,
   const ptr_lib::shared_ptr<PolicyManager>& policyManager)
: identityManager_(identityManager), policyManager_(policyManager),
  face_(0), maxSteps_(100)
{
}

KeyChain::KeyChain(const ptr_lib::shared_ptr<IdentityManager>& identityManager)
: identityManager_(identityManager),
  policyManager_(ptr_lib::make_shared<NoVerifyPolicyManager>()),
  face_(0), maxSteps_(100)
{
}

KeyChain::KeyChain()
: identityManager_(ptr_lib::make_shared<IdentityManager>()),
  policyManager_(ptr_lib::make_shared<NoVerifyPolicyManager>()),
  face_(0), maxSteps_(100)
{
}

void
KeyChain::sign(Data& data, const Name& certificateName, WireFormat& wireFormat)
{
  identityManager_->signByCertificate(data, certificateName, wireFormat);
}

void
KeyChain::sign
  (Interest& interest, const Name& certificateName, WireFormat& wireFormat)
{
  // TODO: Handle signature algorithms other than Sha256WithRsa.
  Sha256WithRsaSignature signature;
  signature.getKeyLocator().setType(ndn_KeyLocatorType_KEYNAME);
  signature.getKeyLocator().setKeyName(certificateName.getPrefix(-1));

  // Append the encoded SignatureInfo.
  interest.getName().append(wireFormat.encodeSignatureInfo(signature));

  // Append an empty signature so that the "signedPortion" is correct.
  interest.getName().append(Name::Component());
  // Encode once to get the signed portion, and sign.
  SignedBlob encoding = interest.wireEncode(wireFormat);
  ptr_lib::shared_ptr<Signature> signedSignature = sign
    (encoding.signedBuf(), encoding.signedSize(), certificateName);

  // Remove the empty signature and append the real one.
  interest.setName(interest.getName().getPrefix(-1).append
    (wireFormat.encodeSignatureValue(*signedSignature)));
}

ptr_lib::shared_ptr<Signature>
KeyChain::sign(const uint8_t* buffer, size_t bufferLength, const Name& certificateName)
{
  return identityManager_->signByCertificate(buffer, bufferLength, certificateName);
}

void
KeyChain::signByIdentity(Data& data, const Name& identityName, WireFormat& wireFormat)
{
  Name signingCertificateName;

  if (identityName.size() == 0) {
    Name inferredIdentity = policyManager_->inferSigningIdentity(data.getName());
    if (inferredIdentity.size() == 0)
      signingCertificateName = identityManager_->getDefaultCertificateName();
    else
      signingCertificateName = identityManager_->getDefaultCertificateNameForIdentity(inferredIdentity);
  }
  else
    signingCertificateName = identityManager_->getDefaultCertificateNameForIdentity(identityName);

  if (signingCertificateName.size() == 0)
    throw SecurityException("No qualified certificate name found!");

  if (!policyManager_->checkSigningPolicy(data.getName(), signingCertificateName))
    throw SecurityException("Signing Cert name does not comply with signing policy");

  identityManager_->signByCertificate(data, signingCertificateName, wireFormat);
}

ptr_lib::shared_ptr<Signature>
KeyChain::signByIdentity(const uint8_t* buffer, size_t bufferLength, const Name& identityName)
{
  Name signingCertificateName = identityManager_->getDefaultCertificateNameForIdentity(identityName);

  if (signingCertificateName.size() == 0)
    throw SecurityException("No qualified certificate name found!");

  return identityManager_->signByCertificate(buffer, bufferLength, signingCertificateName);
}

void
KeyChain::verifyData
  (const ptr_lib::shared_ptr<Data>& data, const OnVerified& onVerified, const OnVerifyFailed& onVerifyFailed, int stepCount)
{
  _LOG_TRACE("Enter Verify");

  if (policyManager_->requireVerify(*data)) {
    ptr_lib::shared_ptr<ValidationRequest> nextStep = policyManager_->checkVerificationPolicy
      (data, stepCount, onVerified, onVerifyFailed);
    if (nextStep)
      face_->expressInterest
        (*nextStep->interest_,
         bind(&KeyChain::onCertificateData, this, _1, _2, nextStep),
         bind(&KeyChain::onCertificateInterestTimeout, this, _1, nextStep->retry_, onVerifyFailed, data, nextStep));
  }
  else if (policyManager_->skipVerifyAndTrust(*data))
    onVerified(data);
  else
    onVerifyFailed(data);
}

void
KeyChain::verifyInterest
  (const ptr_lib::shared_ptr<Interest>& interest,
   const OnVerifiedInterest& onVerified,
   const OnVerifyInterestFailed& onVerifyFailed, int stepCount,
   WireFormat& wireFormat)
{
  _LOG_TRACE("Enter Verify");

  if (policyManager_->requireVerify(*interest)) {
    ptr_lib::shared_ptr<ValidationRequest> nextStep =
      policyManager_->checkVerificationPolicy
        (interest, stepCount, onVerified, onVerifyFailed, wireFormat);
    if (nextStep)
#if 0
      face_->expressInterest
        (*nextStep->interest_,
         bind(&KeyChain::onCertificateData, this, _1, _2, nextStep),
         bind(&KeyChain::onCertificateInterestTimeout, this, _1,
              nextStep->retry_, onVerifyFailed, data, nextStep));
#else
    throw SecurityException("verifyInterest: ValidationRequest not implemented yet");
#endif
  }
  else if (policyManager_->skipVerifyAndTrust(*interest))
    onVerified(interest);
  else
    onVerifyFailed(interest);
}

void
KeyChain::onCertificateData(const ptr_lib::shared_ptr<const Interest> &interest, const ptr_lib::shared_ptr<Data> &data, ptr_lib::shared_ptr<ValidationRequest> nextStep)
{
  // Try to verify the certificate (data) according to the parameters in nextStep.
  verifyData(data, nextStep->onVerified_, nextStep->onVerifyFailed_, nextStep->stepCount_);
}

void
KeyChain::onCertificateInterestTimeout
  (const ptr_lib::shared_ptr<const Interest> &interest, int retry, const OnVerifyFailed& onVerifyFailed, const ptr_lib::shared_ptr<Data> &data,
   ptr_lib::shared_ptr<ValidationRequest> nextStep)
{
  if (retry > 0)
    // Issue the same expressInterest as in verifyData except decrement retry.
    face_->expressInterest
      (*interest,
       bind(&KeyChain::onCertificateData, this, _1, _2, nextStep),
       bind(&KeyChain::onCertificateInterestTimeout, this, _1, retry - 1, onVerifyFailed, data, nextStep));
  else
    onVerifyFailed(data);
}

}
