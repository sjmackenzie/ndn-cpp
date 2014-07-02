/**
 * Copyright (C) 2014 Regents of the University of California.
 * @author: Jeff Thompson <jefft0@remap.ucla.edu>
 * From PyNDN unit-tests by Adelola Bannis.
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
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * A copy of the GNU General Public License is in the file COPYING.
 */

#include "gtest/gtest.h"
#include <ndn-cpp/name.hpp>

using namespace std;
using namespace ndn;

TEST(TestNameMethods, UriConstructor)
{
  string expectedUri = "/entr%C3%A9e/..../%00%01%02%03";
  Name name(expectedUri);
  ASSERT_EQ(name.size(), 4);
  ASSERT_EQ(name.toUri(), expectedUri);
}

TEST(TestNameMethods, CopyConstructor)
{
  string expectedUri = "/entr%C3%A9e/..../%00%01%02%03";
  Name name(expectedUri);
  Name name2(name);
  ASSERT_EQ(name.size(), 3);
  ASSERT_TRUE(name.equals(name2));
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

