/*
 * Copyright (C) 2008
 * Jochen Wickel
 *
 * This file is part of the LTI-Computer Vision Library 2 (LTI-Lib-2)
 *
 * The LTI-Lib-2 is free software; you can redistribute it and/or
 * modify it under the terms of the BSD License.
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the authors nor the names of its contributors may be
 *    used to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * \file   ltiDataTransformer.cpp
 *         Contains the class dataTransformer,
 *         which has still to be documented by Jochen Wickel.
 * 
 * \author Jochen Wickel
 * \date   5.11.2002
 *
 * revisions ..: $Id: ltiDataTransformer.cpp,v 1.4 2008-05-16 21:27:13 alvarado Exp $
 */

#include <cstdio>
#include "ltiDataTransformer.h"

namespace lti {

  // --------------------------------------------------
  // dataTransformer
  // --------------------------------------------------

  const char *const  dataTransformer::notEnoughSpaceMsg = "Not enough space";
  // default constructor
  dataTransformer::dataTransformer()
    : functor() {
  }

  // copy constructor
  dataTransformer::dataTransformer(const dataTransformer& other)
    : functor() {
    copy(other);
  }

  // destructor
  dataTransformer::~dataTransformer() {
  }

  // copy member
  dataTransformer& 
  dataTransformer::copy(const dataTransformer& other) {
    functor::copy(other);

    return (*this);
  }

  // alias for copy member
  dataTransformer&
  dataTransformer::operator=(const dataTransformer& other) {
    return (copy(other));
  }

  // class name
  const std::string& dataTransformer::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // --------------------------------------------------
  // dataTransformer::buffer
  // --------------------------------------------------

  dataTransformer::buffer::buffer(const int& n) 
    : vector<ubyte>(n) {
  }

  dataTransformer::buffer::buffer(const int& n,
                                  ubyte _data[],
                                  const eConstantReference constRef)
    : vector<ubyte>(n,_data,constRef) {
  }

  bool dataTransformer::buffer::saveRaw(const std::string& filename) const {
    FILE* f=fopen(filename.c_str(),"w");
    if (f != 0) {
      int k=static_cast<int>(fwrite(&at(0),sizeof(value_type),size(),f));
      if (k != size()) {
        fclose(f);
        return false;
      }
      fclose(f);
      return true;
    } else {
      return false;
    }
  }

  bool dataTransformer::buffer::loadRaw(const std::string& filename) {
    FILE* f=fopen(filename.c_str(),"r");
    if (f != 0) {
      int k=fseek(f,0,SEEK_END);
      int len=ftell(f);
      resize(len);
      rewind(f);
      k=static_cast<int>(fread(&at(0),1,len,f));
      fclose(f);
      return (k == len);
    } else {
      return false;
    }
  }
}

