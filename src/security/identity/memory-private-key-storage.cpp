/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil -*- */
/**
 * Copyright (C) 2013-2014 Regents of the University of California.
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

#if 1
#include <stdexcept>
#endif
#include "../../c/util/crypto.h"
#include <openssl/ec.h>
#include <ndn-cpp/security/security-exception.hpp>
#include <ndn-cpp/security/identity/memory-private-key-storage.hpp>

using namespace std;

namespace ndn {

MemoryPrivateKeyStorage::~MemoryPrivateKeyStorage()
{
}

void
MemoryPrivateKeyStorage::setPublicKeyForKeyName
  (const Name& keyName, KeyType keyType, const uint8_t* publicKeyDer,
   size_t publicKeyDerLength)
{
  publicKeyStore_[keyName.toUri()] = PublicKey::fromDer(keyType, Blob(publicKeyDer, publicKeyDerLength));
}

void
MemoryPrivateKeyStorage::setPrivateKeyForKeyName
  (const Name& keyName, KeyType keyType, const uint8_t* privateKeyDer,
   size_t privateKeyDerLength)
{
  privateKeyStore_[keyName.toUri()] = ptr_lib::make_shared<PrivateKey>
    (keyType, privateKeyDer, privateKeyDerLength);
}

void
MemoryPrivateKeyStorage::generateKeyPair(const Name& keyName, KeyType keyType, int keySize)
{
#if 1
  throw runtime_error("MemoryPrivateKeyStorage::generateKeyPair not implemented");
#endif
}

ptr_lib::shared_ptr<PublicKey>
MemoryPrivateKeyStorage::getPublicKey(const Name& keyName)
{
  map<string, ptr_lib::shared_ptr<PublicKey> >::iterator publicKey = publicKeyStore_.find(keyName.toUri());
  if (publicKey == publicKeyStore_.end())
    throw SecurityException(string("MemoryPrivateKeyStorage: Cannot find public key ") + keyName.toUri());
  return publicKey->second;
}

Blob
MemoryPrivateKeyStorage::sign(const uint8_t* data, size_t dataLength, const Name& keyName, DigestAlgorithm digestAlgorithm)
{
  if (digestAlgorithm != DIGEST_ALGORITHM_SHA256)
    return Blob();

  uint8_t digest[SHA256_DIGEST_LENGTH];
  ndn_digestSha256(data, dataLength, digest);
  // TODO: use RSA_size to get the proper size of the signature buffer.
  uint8_t signatureBits[1000];
  unsigned int signatureBitsLength;

  // Find the private key and sign.
  map<string, ptr_lib::shared_ptr<PrivateKey> >::iterator privateKey = privateKeyStore_.find(keyName.toUri());
  if (privateKey == privateKeyStore_.end())
    throw SecurityException(string("MemoryPrivateKeyStorage: Cannot find private key ") + keyName.toUri());
  if (privateKey->second->getKeyType() == KEY_TYPE_RSA) {
    if (!RSA_sign(NID_sha256, digest, sizeof(digest), signatureBits,
                  &signatureBitsLength, privateKey->second->getRsaPrivateKey()))
      throw SecurityException("Error in RSA_sign");
  }
  else if (privateKey->second->getKeyType() == KEY_TYPE_EC) {
    if (!ECDSA_sign(NID_sha256, digest, sizeof(digest), signatureBits,
                  &signatureBitsLength, privateKey->second->getEcPrivateKey()))
      throw SecurityException("Error in RSA_sign");
  }
  else
    // We don't expect this to happen.
    throw SecurityException("Unrecognized private key type");

  return Blob(signatureBits, (size_t)signatureBitsLength);
}

Blob
MemoryPrivateKeyStorage::decrypt(const Name& keyName, const uint8_t* data, size_t dataLength, bool isSymmetric)
{
#if 1
  throw runtime_error("MemoryPrivateKeyStorage::decrypt not implemented");
#endif
}

Blob
MemoryPrivateKeyStorage::encrypt(const Name& keyName, const uint8_t* data, size_t dataLength, bool isSymmetric)
{
#if 1
  throw runtime_error("MemoryPrivateKeyStorage::encrypt not implemented");
#endif
}

void
MemoryPrivateKeyStorage::generateKey(const Name& keyName, KeyType keyType, int keySize)
{
#if 1
  throw runtime_error("MemoryPrivateKeyStorage::generateKey not implemented");
#endif
}

bool
MemoryPrivateKeyStorage::doesKeyExist(const Name& keyName, KeyClass keyClass)
{
  if (keyClass == KEY_CLASS_PUBLIC)
    return publicKeyStore_.find(keyName.toUri()) != publicKeyStore_.end();
  else if (keyClass == KEY_CLASS_PRIVATE)
    return privateKeyStore_.find(keyName.toUri()) != privateKeyStore_.end();
  else
    // KEY_CLASS_SYMMETRIC not implemented yet.
    return false;
}

MemoryPrivateKeyStorage::PrivateKey::PrivateKey
  (KeyType keyType, const uint8_t* keyDer, size_t keyDerLength)
{
  keyType_ = keyType;
  rsaPrivateKey_ = 0;
  ecPrivateKey_ = 0;

  // Use a temporary pointer since d2i updates it.
  const uint8_t *derPointer = keyDer;
  if (keyType == KEY_TYPE_RSA) {
    rsaPrivateKey_ = d2i_RSAPrivateKey(NULL, &derPointer, keyDerLength);
    if (!rsaPrivateKey_)
      throw SecurityException("PrivateKey constructor: Error decoding RSA private key DER");
  }
  else if (keyType == KEY_TYPE_EC) {
    ecPrivateKey_ = d2i_ECPrivateKey(NULL, &derPointer, keyDerLength);
    if (!ecPrivateKey_)
      throw SecurityException("PrivateKey constructor: Error decoding EC private key DER");
  }
  else
    throw SecurityException("PrivateKey constructor: Unrecognized keyType");
}

MemoryPrivateKeyStorage::PrivateKey::~PrivateKey()
{
  if (rsaPrivateKey_)
    RSA_free(rsaPrivateKey_);
  if (ecPrivateKey_)
    EC_KEY_free(ecPrivateKey_);
}

}
