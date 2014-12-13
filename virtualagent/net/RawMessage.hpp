/**
 * Copyright (c) 2014 Hee Won Lee <knowpd@gmail.com>
 *
 * This file is part of TdfAgent.
 *
 * TdfAgent is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * TdfAgent is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
 */

// Copyright (c) 2003-2012 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef RAW_MESSAGE_HPP
#define RAW_MESSAGE_HPP

#include <cstdio>
#include <cstdlib>
#include <cstring>

class RawMessage
{
public:
  enum { HEADER_LENGTH = 4 };
  enum { MAX_BODY_LENGTH = 512 };

  RawMessage()
    : bodyLength_(0)
  {
  }

  const char* data() const
  {
    return data_;
  }

  char* data()
  {
    return data_;
  }

  size_t length() const
  {
    return HEADER_LENGTH + bodyLength_;
  }

  const char* body() const
  {
    return data_ + HEADER_LENGTH;
  }

  char* body()
  {
    return data_ + HEADER_LENGTH;
  }

  size_t body_length() const
  {
    return bodyLength_;
  }

  void body_length(size_t new_length)
  {
    bodyLength_ = new_length;
    if (bodyLength_ > MAX_BODY_LENGTH)
      bodyLength_ = MAX_BODY_LENGTH;
  }

  bool decode_header()
  {
    using namespace std; // For strncat and atoi.
    char header[HEADER_LENGTH + 1] = "";
    strncat(header, data_, HEADER_LENGTH);
    bodyLength_ = atoi(header);
    if (bodyLength_ > MAX_BODY_LENGTH)
    {
      bodyLength_ = 0;
      return false;
    }
    return true;
  }

  void encode_header()
  {
    using namespace std; // For sprintf and memcpy.
    char header[HEADER_LENGTH + 1] = "";
    sprintf(header, "%4ld", bodyLength_);
    memcpy(data_, header, HEADER_LENGTH);
  }

private:
  char data_[HEADER_LENGTH + MAX_BODY_LENGTH];
  size_t bodyLength_;
};

#endif // RAW_MESSAGE_HPP
