/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil -*- */
/**
 * Copyright (C) 2013 Regents of the University of California.
 * @author: Jeff Thompson <jefft0@remap.ucla.edu>
 * See COPYING for copyright and distribution information.
 */

#ifndef NDN_SHA256_WITH_RSA_SIGNATURE_HPP
#define NDN_SHA256_WITH_RSA_SIGNATURE_HPP

#include "data.hpp"
#include "key-locator.hpp"
#include "publisher-public-key-digest.hpp"

namespace ndn {

/**
 * A Sha256WithRsaSignature extends Signature and holds the signature bits and other info representing a
 * SHA256-with-RSA signature in a data packet.
 */
class Sha256WithRsaSignature : public Signature {
public:
  /**
   * Return a pointer to a new Sha256WithRsaSignature which is a copy of this signature.
   */
  virtual ptr_lib::shared_ptr<Signature> 
  clone() const;

  /**
   * Set the signatureStruct to point to the values in this signature object, without copying any memory.
   * WARNING: The resulting pointers in signatureStruct are invalid after a further use of this object which could reallocate memory.
   * @param signatureStruct a C ndn_Signature struct where the name components array is already allocated.
   */
  virtual void 
  get(struct ndn_Signature& signatureStruct) const;

  /**
   * Clear this signature, and set the values by copying from the ndn_Signature struct.
   * @param signatureStruct a C ndn_Signature struct
   */
  virtual void 
  set(const struct ndn_Signature& signatureStruct);

  const Blob& 
  getDigestAlgorithm() const { return digestAlgorithm_; }

  /**
   * @deprecated Witness is deprecated. 
   */
  const Blob& 
  getWitness() const { return witness_; }

  const Blob& 
  getSignature() const { return signature_; }
  
  /**
   * @deprecated.  The Signature publisherPublicKeyDigest is deprecated.  If you need a publisher public key digest, 
   * set the keyLocator keyLocatorType to KEY_LOCATOR_DIGEST and set its key data to the digest.
   */
  const PublisherPublicKeyDigest& 
  getPublisherPublicKeyDigest() const { return publisherPublicKeyDigest_; }
  
  /**
   * @deprecated.  The Signature publisherPublicKeyDigest is deprecated.  If you need a publisher public key digest, 
   * set the keyLocator keyLocatorType to KEY_LOCATOR_DIGEST and set its key data to the digest.
   */
  PublisherPublicKeyDigest& 
  getPublisherPublicKeyDigest() { return publisherPublicKeyDigest_; }
  
  const KeyLocator& 
  getKeyLocator() const { return keyLocator_; }
  
  KeyLocator& 
  getKeyLocator() { return keyLocator_; }

  void 
  setDigestAlgorithm(const Blob& digestAlgorithm) { digestAlgorithm_ = digestAlgorithm; }

  /**
   * @deprecated Witness is deprecated. 
   */
  void 
  setWitness(const Blob& witness) { witness_ = witness; }

  void 
  setSignature(const Blob& signature) { signature_ = signature; }

  /**
   * @deprecated.  The Signature publisherPublicKeyDigest is deprecated.  If you need a publisher public key digest, 
   * set the keyLocator keyLocatorType to KEY_LOCATOR_DIGEST and set its key data to the digest.
   */
  void 
  setPublisherPublicKeyDigest(const PublisherPublicKeyDigest& publisherPublicKeyDigest) { publisherPublicKeyDigest_ = publisherPublicKeyDigest; }
  
  void 
  setKeyLocator(const KeyLocator& keyLocator) { keyLocator_ = keyLocator; }
  
  /**
   * Clear all the fields.
   */
  void 
  clear()
  {
    digestAlgorithm_.reset();
    witness_.reset();
    signature_.reset();
    publisherPublicKeyDigest_.clear();
    keyLocator_.clear();
  }

private:
  Blob digestAlgorithm_; /**< if empty, the default is 2.16.840.1.101.3.4.2.1 (sha-256) */
  /** @deprecated Witness is deprecated. */
  Blob witness_;
  Blob signature_;
  /** @deprecated.  The Signature publisherPublicKeyDigest is deprecated.  If you need a publisher public key digest, 
   * set the keyLocator keyLocatorType to KEY_LOCATOR_DIGEST and set its key data to the digest. */
  PublisherPublicKeyDigest publisherPublicKeyDigest_;
  KeyLocator keyLocator_;
};

}

#endif
