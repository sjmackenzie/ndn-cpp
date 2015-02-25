/**
 * Copyright (C) 2013-2015 Regents of the University of California.
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

#ifndef NDN_PUBLISHERPUBLICKEYDIGEST_H
#define NDN_PUBLISHERPUBLICKEYDIGEST_H

#include <ndn-cpp/c/common.h>
#include "util/blob.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * A PublisherPublicKeyDigest holds a pointer to the publisher public key digest value, if any.
 * We make a separate struct since this is used by multiple other structs.
 */
struct ndn_PublisherPublicKeyDigest {
  struct ndn_Blob publisherPublicKeyDigest; /**< A Blob whose value is a pointer to pre-allocated buffer.  0 for none */
};

/**
 * Initialize an ndn_PublisherPublicKeyDigest struct with 0 for none.
 */
static __inline void ndn_PublisherPublicKeyDigest_initialize(struct ndn_PublisherPublicKeyDigest *self)
{
  ndn_Blob_initialize(&self->publisherPublicKeyDigest, 0, 0);
}

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
namespace ndn {

class PublisherPublicKeyDigestLite : private ndn_PublisherPublicKeyDigest {
public:
  PublisherPublicKeyDigestLite()
  {
    ndn_PublisherPublicKeyDigest_initialize(this);
  }

  const BlobLite&
  getPublisherPublicKeyDigest() const { return BlobLite::upCast(publisherPublicKeyDigest); }

  void
  setPublisherPublicKeyDigest(const BlobLite& publisherPublicKeyDigest)
  {
    BlobLite::upCast(this->publisherPublicKeyDigest) = publisherPublicKeyDigest;
  }

  /**
   * Upcast the reference to the ndn_PublisherPublicKeyDigest struct to a PublisherPublicKeyDigestLite.
   * @param publisherPublicKeyDigest A reference to the ndn_PublisherPublicKeyDigest struct.
   * @return The same reference as PublisherPublicKeyDigestLite.
   */
  static PublisherPublicKeyDigestLite&
  upCast(ndn_PublisherPublicKeyDigest& publisherPublicKeyDigest)
  {
    return *(PublisherPublicKeyDigestLite*)&publisherPublicKeyDigest;
  }

  static const PublisherPublicKeyDigestLite&
  upCast(const ndn_PublisherPublicKeyDigest& publisherPublicKeyDigest)
  {
    return *(PublisherPublicKeyDigestLite*)&publisherPublicKeyDigest;
  }
};

}
#endif

#endif
