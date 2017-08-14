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
 * \file   ltiDataCodec.cpp
 *         Contains the class dataCodec,
 *         which has still to be documented by Jochen Wickel.
 * 
 * \author Jochen Wickel
 * \date   5.11.2002
 *
 * revisions ..: $Id: ltiDataCodec.cpp,v 1.8 2008-08-06 03:40:58 alvarado Exp $
 */

#include "ltiDataCodec.h"
#include <cstring> // for memcpy

namespace lti {
  // --------------------------------------------------
  // dataCodec::parameters
  // --------------------------------------------------

  // default constructor
  dataCodec::parameters::parameters()
    : dataTransformer::parameters() {
    
    codingDirection = Decode;
  }

  // copy constructor
  dataCodec::parameters::parameters(const parameters& other)
    : dataTransformer::parameters() {
    copy(other);
  }

  // destructor
  dataCodec::parameters::~parameters() {
  }

  // copy member

  dataCodec::parameters&
  dataCodec::parameters::copy(const parameters& other) {
    dataTransformer::parameters::copy(other);

    
    codingDirection = other.codingDirection;

    return *this;
  }

  // alias for copy method
  dataCodec::parameters&
  dataCodec::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  // class name
  const std::string& dataCodec::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone method
  dataCodec::parameters*
  dataCodec::parameters::clone() const {
    return new parameters(*this);
  }

  // new instance
  dataCodec::parameters*
  dataCodec::parameters::newInstance() const {
    return new parameters();
  }

  /*
   * Write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool dataCodec::parameters::write(ioHandler& handler,
                                         const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {
      
      lti::write(handler,"codingDirection",codingDirection);
    }

    b = b && dataTransformer::parameters::write(handler,false);

    if (complete) {
      b = b && handler.writeEnd();
    }

    return b;
  }

  /*
   * Read the parameters from the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also read, otherwise only the data block will be read.
   * @return true if read was successful
   */
  bool dataCodec::parameters::read(ioHandler& handler,
                                        const bool complete)  {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {
      
      lti::read(handler,"codingDirection",codingDirection);
    }

    b = b && dataTransformer::parameters::read(handler,false);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

  // --------------------------------------------------
  // dataCodec
  // --------------------------------------------------

  // default constructor
  dataCodec::dataCodec()
    : dataTransformer() {
  }

  // default constructor
  dataCodec::dataCodec(const parameters& par)
    : dataTransformer() {

    // set the given parameters
    setParameters(par);
  }


  // copy constructor
  dataCodec::dataCodec(const dataCodec& other)
    : dataTransformer() {
    copy(other);
  }

  // destructor
  dataCodec::~dataCodec() {
  }

  // copy member
  dataCodec& 
  dataCodec::copy(const dataCodec& other) {
    dataTransformer::copy(other);

    return (*this);
  }

  // alias for copy member
  dataCodec&
  dataCodec::operator=(const dataCodec& other) {
    return (copy(other));
  }

  // class name
  const std::string& dataCodec::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // return parameters
  const dataCodec::parameters&
  dataCodec::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }

  // -------------------------------------------------------------------
  // The apply() member functions
  // -------------------------------------------------------------------

  
  /**
   * Encodes data in place.
   */
  bool dataCodec::encode(buffer& srcdest) const {
    bool result=false;
    int n=srcdest.size();
    int dsize=estimateEncodedSize(n);
    buffer dest(dsize);
    int tmp;
    do {
      tmp=dsize;
      result=encodeImplementation(srcdest,dest,n,tmp);
      if (tmp == NotEnoughSpace) {
        dsize=getNewEncodingBufferSize(dsize);
        dest.allocate(dsize);
      }
    } while (tmp == NotEnoughSpace);
    srcdest.fill(dest,0,tmp);
    srcdest.resize(tmp);
    return result;
  }


  /**
   * Encodes data on copy.
   */
  bool dataCodec::encode(const buffer& src, buffer& dest) const {
    int dsize=dest.size();
    bool result=false;
    int tmp;
    do {
      tmp=dsize;
      result=encodeImplementation(src,dest,src.size(),tmp);
      if (tmp == NotEnoughSpace) {
        dsize=getNewEncodingBufferSize(dest.size());
        dest.allocate(dsize);
      }
    } while (tmp == NotEnoughSpace);
    dest.resize(tmp);
    return result;
  }


  /*
   * Decodes data in place.
   */
  bool dataCodec::decode(buffer& srcdest) const {
    bool result=false;
    int n=srcdest.size();
    int dsize=estimateDecodedSize(n);
    buffer dest(dsize);
    int tmp;
    do {
      tmp=dsize;
      result=decodeImplementation(srcdest,dest,n,tmp);
      if (tmp == NotEnoughSpace) {
        dsize=getNewDecodingBufferSize(dsize);
        dest.allocate(dsize);
      }
    } while (tmp == NotEnoughSpace);
    srcdest.fill(dest,0,tmp);
    srcdest.resize(tmp);
    return result;
  }


  /*
   * Decodes data on copy.
   */
  bool dataCodec::decode(const buffer& src, buffer& dest) const {
    int dsize=dest.size();
    bool result=false;
    int tmp;
    do {
      tmp=dsize;
      result=decodeImplementation(src,dest,src.size(),tmp);
      if (tmp == NotEnoughSpace) {
        dsize=getNewDecodingBufferSize(dest.size());
        dest.allocate(dsize);
      }
    } while (tmp == NotEnoughSpace);
    dest.resize(tmp);
    return result;
  }


  /*
   * Encodes data in place.
   */
  bool dataCodec::encode(ubyte* srcdest, int nsrc, int& ndest) const {
    buffer tmp(ndest);
    buffer src(nsrc,srcdest,ConstantReference);
    bool result=false;
    result=encodeImplementation(src,tmp,nsrc,ndest);
    if (ndest == NotEnoughSpace) {
      result=false;
    } else {
      memcpy(srcdest,tmp.data(),ndest);
    }
    return result;
  }


  /**
   * Encodes data on copy.
   */
  bool dataCodec::encode(const ubyte* src, int nsrc,
                         ubyte* dest, int& ndest) const {
    // the next line is ugly, but it is the only way to create a
    // wrapper for the const buffer
    const buffer tsrc(nsrc,const_cast<ubyte*>(src),ConstantReference);
    buffer tdest(ndest,dest,ConstantReference);
    return encodeImplementation(tsrc,tdest,nsrc,ndest);
  }


  /**
   * Decodes data in place.
   */
  bool dataCodec::decode(ubyte* srcdest, int nsrc, int& ndest) const {
    buffer tmp(ndest);
    buffer src(nsrc,srcdest,ConstantReference);
    bool result=false;
    result=decodeImplementation(src,tmp,nsrc,ndest);
    if (ndest == NotEnoughSpace) {
      result=false;
    } else {
      memcpy(srcdest,tmp.data(),ndest);
    }
    return result;
  }



  /**
   * Decodes data on copy.
   */
  bool dataCodec::decode(const ubyte* src, int nsrc,
                         ubyte* dest, int& ndest) const {
    // the next line is ugly, but it is the only way to create a
    // wrapper for the const buffer
    const buffer tsrc(nsrc,const_cast<ubyte*>(src),ConstantReference);
    buffer tdest(ndest,dest,ConstantReference);
    return decodeImplementation(tsrc,tdest,nsrc,ndest);
  }


  int dataCodec::estimateEncodedSize(int originalSize) const {
    return originalSize;
  }

  /*
   * Computes the default buffer size for decoded data. The
   * source of the encoded data buffer is given as argument.
   * The default is to return the given value.
   */
  int dataCodec::estimateDecodedSize(int encodedSize) const {
    return encodedSize;
  }

    /*
     * Computes a new reasonable buffer size for encoded data
     * if the given buffer size is too small. The default is
     * 2*orginialSize;
     */
  int dataCodec::getNewEncodingBufferSize(int originalSize) const {
    return 2*originalSize;
  }

    /*
     * Computes a new reasonable buffer size for encoded data
     * if the given buffer size is too small. The default is
     * 2*orginialSize;
     */
  int dataCodec::getNewDecodingBufferSize(int encodedSize) const {
    return 2*encodedSize;
  }

  /*
   * Operates on the given arguments. If the data is encoded or
   * decoded depends on the coding direction parameter.
   */
  bool dataCodec::apply(ubyte* srcdest, int nsrc, int& ndest) const {
    return
      getParameters().codingDirection == Encode ?
      encode(srcdest, nsrc, ndest) :
      decode(srcdest, nsrc, ndest);
  }

  /*
   * Operates on the given argument. If the data is encoded or
   * decoded depends on the coding direction parameter.
   * @param srcdest vector with the input data which will
   *        also receive the transformed data.
   * @return true if apply successful or false otherwise.
   */
  bool dataCodec::apply(buffer& srcdest) const {
    return
      getParameters().codingDirection == Encode ?
      encode(srcdest) :
      decode(srcdest);
  }

  /*
   * Operates on a copy of the given arguments.
   */
  bool dataCodec::apply(const ubyte* src,
                        int nsrc,
                        ubyte* dest,
                        int& ndest) const {
    return
      getParameters().codingDirection == Encode ?
      encode(src,nsrc,dest,ndest) :
      decode(src,nsrc,dest,ndest);
  }

  /**
   * Operates on the given argument.
   * @param srcdest vector with the input data which will
   *        also receive the transformed data.
   * @return true if apply successful or false otherwise.
   */
  bool dataCodec::apply(const buffer& src, buffer& dest) const {
    return
      getParameters().codingDirection == Encode ?
      encode(src,dest) :
      decode(src,dest);
  }

  /**
   * Read a dataCodec::eCodingDirection
   *
   * @ingroup gStorable
   */
  bool read(ioHandler& handler,
            dataCodec::eCodingDirection& data) {
    std::string str;
    if (handler.read(str)) {
      if (str.find("ncode") != std::string::npos) {
        data = dataCodec::Encode;
      } else if (str.find("ecode") != std::string::npos) {
        data = dataCodec::Decode;
      } else {
        data = dataCodec::Encode;
        return false;
      }
      return true;
    }

    return false;
  }


  /**
   * Write a dataCodec::eCodingDirection
   *
   * @ingroup gStorable
   */
  bool write(ioHandler& handler,
             const dataCodec::eCodingDirection& data) {
    bool b = false;
    switch(data) {
    case dataCodec::Encode:
      b = handler.write("Encode");
      break;
    case dataCodec::Decode:
      b = handler.write("Decode");
      break;
    default:
      b=false;
      handler.setStatusString("Undefined eCodingDirection");
      handler.write("Unknown");
    }
    return b;
  }

}

