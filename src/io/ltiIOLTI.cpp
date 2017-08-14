/*
 * Copyright (C) 2008
 * Pablo Alvarado
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
 * \file   ltiIOLTI.cpp
 *         Contains the class lti::ioLTI,
 *         which provides IO access to LTI image files.
 * 
 * \author Pablo Alvarado
 * \author Jochen Wickel
 * \date   03.06.2003
 *
 * revisions ..: $Id: ltiIOLTI.cpp,v 1.3 2014-08-21 09:03:49 alvarado Exp $
 */


#include "ltiIOLTI.h"
#include "ltiRGBAPixel.h"
#include "ltiImage.h"
#include "ltiFactory.h"
#include "ltiEndianness.h"

#include <fstream>
#include <vector>
#include <map>

namespace lti {

  // In ioImageInterface register this as reader/writer of PNG files
  _LTI_REGISTER_IN_FACTORY_AS(LTI,ioImageInterface,ioLTI)

  // -----------------------------------------------------------------------
  // ioLTI::parameters
  // -----------------------------------------------------------------------

  ioLTI::parameters::parameters() : ioImageInterface::parameters() {
    codec = "lti::identityCodec";
  }
  
  ioLTI::parameters::parameters(const parameters& other) 
    : ioImageInterface::parameters() {
    copy(other);
  }
  
  ioLTI::parameters& ioLTI::parameters::copy(const parameters& other) {
    ioImageInterface::parameters::copy(other);
    codec = other.codec;

    return (*this);
  }

  const std::string& ioLTI::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  ioLTI::parameters* ioLTI::parameters::clone() const  {
    return (new parameters(*this));
  }

  ioLTI::parameters* ioLTI::parameters::newInstance() const  {
    return (new parameters());
  }

  /*
   * write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool ioLTI::parameters::write(ioHandler& handler,
                                const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }
    
    if (b) {
      lti::write(handler,"codec",codec);
    }
    
    b = b && ioImageInterface::parameters::write(handler,false);

    if (complete) {
      b = b && handler.writeEnd();
    }

    return b;
  }

  /*
   * read the parameters from the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also read, otherwise only the data block will be read.
   * @return true if write was successful
   */
  bool ioLTI::parameters::read(ioHandler& handler,
                               const bool complete) {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {
      lti::read(handler,"codec",codec);
    }

    b = b && ioImageInterface::parameters::read(handler,false);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

  // -------------------------------------------------------
  //  ioLTI::header
  // -------------------------------------------------------

  ioLTI::header::header() : type(0x544c),contents(0),compression(0),size(0),
                            reserved1(0),reserved2(0),rows(0),columns(0),
                            codec("lti::identityCodec") {
  }

  int ioLTI::header::length() const {
    return (compression<3)?24:(24+static_cast<int>(codec.length())+1);
  }

  bool ioLTI::header::read(std::ifstream &in) {
    typedef endianness<LittleEndian> io;
    
    uint16 tmpType;
    uint32 tmpReserved;
    io::read(in,tmpType);  // io reads with the correct endianness

    if (tmpType != type) { // This means LT, and is mandatory for a LTI file!
      return false;
    }

    io::read(in,contents);
    io::read(in,compression);
    io::read(in,size);
    io::read(in,tmpReserved); // just ignore the readed data!
    io::read(in,tmpReserved); // just ignore the readed data!
    io::read(in,rows);
    io::read(in,columns);
    
    if (compression>=3) {
      codec.clear();
      ubyte character;
      io::read(in,character);
      while (character != 0) {
        codec += character;
        io::read(in,character);
      } 
    } else {
      switch(compression) {
        case 0:
          codec = "lti::identityCodec";
          break;
        case 1:
          codec = "lti::runLengthCodec";
          break;
        case 2:
          codec = "lti::flateCodec";
          break;
        default:
          // we should never get here 
          return false;
      }
    }

    return true;
  }

  bool ioLTI::header::write(std::ofstream &out) const {
    typedef endianness<LittleEndian> io;
 
    io::write(out,type);  // io writes with the correct endianness
    io::write(out,contents);
    io::write(out,compression);
    io::write(out,size);
    io::write(out,reserved1);
    io::write(out,reserved2);
    io::write(out,rows);
    io::write(out,columns);
    
    if (compression >= 3) {
      for (unsigned int i=0;i<codec.length();++i) {
        io::write(out,static_cast<ubyte>(codec[i]));
      }
      io::write(out,static_cast<ubyte>(0));
    }

    return true;
  }

  /**
   * Loads a matrix from the given stream. The data are written
   * into theChannel, the meta-data are written into theHeader
   * These methods ignore all the parameters in this objects.
   * The only reason they are not static is that they modify
   * the status string.
   */
  template<typename T>
  bool ioLTI::loadMatrix(const std::string& filename,
                         matrix<T>& theChannel,
                         header& theHeader) const {
    //open stream
    std::ifstream in;
    in.open(filename.c_str(),std::ios::in|std::ios::binary);

    if (!(in.good() && in.is_open())) {
      std::string str = "Could not open file " + filename;
      setStatusString(str.c_str());
      return false;
    }

    return loadMatrix(in,theChannel,theHeader);
  }

  template<typename T>
  bool ioLTI::loadMatrix(std::ifstream& in,
                         matrix<T>& theChannel,
                         header& theHeader) const {
    theChannel.clear();
    
    //open failed?
    if (!(in.good() && in.is_open())) {
      std::string str = "Could not open stream";
      setStatusString(str.c_str());
      return false;
    }
    
    // read header
    if (!theHeader.read(in)) {
      setStatusString("Wrong header. Is this an LTI file?");
      return false;
    }
    
    
    //create the right codec for this file
    dataCodec* codec =
      factory<dataCodec>::getFactory().newInstance(theHeader.codec);
    
    if (isNull(codec)) {
      std::string msg = "Unknown codec: " + theHeader.codec;
      setStatusString(msg);
      return false;
    }
    
//     _lti_debug2("type: "     << theHeader.type     << std::endl);
//     _lti_debug2("contents: " << theHeader.contents << std::endl);
//     _lti_debug2("size: "     << theHeader.size     << std::endl);
//     _lti_debug2("rows: "     << theHeader.rows     << std::endl);
//     _lti_debug2("columns: "  << theHeader.columns  << std::endl);
//     _lti_debug2("codec: "    << theHeader.codec    << std::endl);
        
    if (theHeader.contents != getTypeCode(T())) {
      std::string str="Inconsistent file type. ";
      str+=theChannel.name();
      str+" expected but ";
      // should we really try to recover from type errors???
      // the following compiles, but might not get you what you expect
      switch(theHeader.contents) {
      case 'b':
        str+=" lti::matrix<ubyte> found.";
        break;
      case 'i':
        str+=" lti::imatrix found.";
        break;
      case 'f':
        str+=" lti::matrix<float> found.";
        break;
      case 'd':
        str+=" lti::matrix<double> found.";
        break;
      case 'c':
        str+=" lti::matrix<rgbaPixel> found.";
        break;
      case 'w':
        str+=" lti::matrix<fcomplex> found.";
        break;
      case 'z':
        str+=" lti::matrix<dcomplex> found.";
        break;
      default:
        str+=std::string(" unknown type ")
          + char(theHeader.contents) + std::string(" found.");
      }
      setStatusString(str.c_str());
      in.close();
      delete codec;
      return false;
    } else {
      bool flag=loadBody(in,theChannel,theHeader,codec);
      in.close();
      delete codec;
      return flag;
    }
      
  }

  /**
   * Saves the given matrix to the given stream. The header is modified
   * with appropriate data derived from the matrix structure.
   */
  template<typename T>
  bool ioLTI::saveMatrix(const std::string& filename, 
                         const matrix<T>& theChannel,
                         header& theHeader,
                         const std::string& codecName) const {
    std::ofstream out;
    out.open(filename.c_str(),std::ios::out|std::ios::binary);
    
    //stream ok?
    if (!(out.good() && out.is_open())) {
      std::string str = "Could not open file " + filename + " for writing.";
      setStatusString(str.c_str());
      out.close();
      return false;
    }

    return saveMatrix(out,theChannel,theHeader,codecName);
  }

  template<typename T>
  bool ioLTI::saveMatrix(std::ofstream& out,
                         const matrix<T>& theChannel,
                         header& theHeader,
                         const std::string& codecName) const {
    
    //stream ok?
    if (!(out.good() && out.is_open())) {
      std::string str = "Could not open stream for writing.";
      setStatusString(str.c_str());
      out.close();
      return false;
    }
    
    // build the codec
    dataCodec* codec = factory<dataCodec>::getFactory().newInstance(codecName);
    
    theHeader.contents=getTypeCode(T());

    if (codecName == "lti::identityCodec") {
      theHeader.compression = 0;
    } else if (codecName == "lti::runLengthCodec") {
      theHeader.compression = 1;
    } else if (codecName == "lti::flateCodec") {
      theHeader.compression = 2;
    } else {
      theHeader.compression = 3;
      theHeader.codec = codecName;
    }
    
    int tmpsize=sizeof(T)*theChannel.rows()*theChannel.columns();
    int encsize = codec->estimateEncodedSize(tmpsize);
    
    // create the temporary buffer
    dataCodec::buffer tmp(encsize);
    
    // encode the data
    codec->encode((const ubyte*)theChannel.data(),tmpsize,
                  tmp.data(),encsize);
    
    theHeader.size = encsize;
    theHeader.rows = theChannel.rows();
    theHeader.columns = theChannel.columns();
    
    // write the header
    if (!theHeader.write(out)) {
      setStatusString("Could not write header.");
      out.close();
      delete codec;
      return false;
    }
    
    // write the data
    out.write((const char*)(tmp.data()),theHeader.size);
    out.close();
    
    delete codec;
    return true;
  }
  
  /**
   * Loads the matrix data from the given stream. The meta-data
   * must be passed in the given header.
   */
  template<typename T>
  bool ioLTI::loadBody(std::ifstream& in, matrix<T>& theChannel,
                const header& theHeader,
                dataCodec* codec) const {
    
    
    dataCodec::buffer tmp(theHeader.size);
    in.read((char*)(tmp.data()),theHeader.size);
    
    int dummy=theHeader.rows*theHeader.columns*sizeof(T);
    
    theChannel.allocate(theHeader.rows,theHeader.columns);
    
    codec->decode(tmp.data(),theHeader.size,
                  (ubyte*)(theChannel.data()),dummy);
    
    return true;
    
  }
  
  // ----------------------------------------------------------------------
  // ioLTI
  // ----------------------------------------------------------------------

  // constructor
  ioLTI::ioLTI() : ioImageInterface() {
    parameters par;
    setParameters(par);
  }

  ioLTI::ioLTI(const parameters& par) : ioImageInterface() {
    setParameters(par);
  }

  ioLTI::~ioLTI() {
  }

  // returns the current parameters
  const ioLTI::parameters& ioLTI::getParameters() const {
    const parameters* params =
      dynamic_cast<const parameters*>(&ioImageInterface::getParameters());
    if (isNull(params)) {
      throw invalidParametersException(name());
    }
    return *params;
  }

  const std::string& ioLTI::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  ioLTI* ioLTI::clone() const {
    return new ioLTI(*this);
  }

  ioLTI* ioLTI::newInstance() const {
    return new ioLTI();
  }

  // read file
  bool ioLTI::load(const std::string& filename,image& theImage) {
    
    // open file to read
    return loadMatrix(filename,theImage,theHeader_);
  }

  // read file
  bool ioLTI::load(const std::string& filename,
                     matrix<ubyte>& theChannel,
                     lti::palette& colors) {
    
    colors.clear();
    // open file to read
    return loadMatrix(filename,theChannel,theHeader_);
    
  }

  bool ioLTI::load(const std::string& filename,
                   matrix<ubyte>& theChannel) {
    // open file to read
    return loadMatrix(filename,theChannel,theHeader_);
  }

  bool ioLTI::load(const std::string& filename,
                     matrix<float>& theChannel) {
    // open file to read
    return loadMatrix(filename,theChannel,theHeader_);
  }

  bool ioLTI::load(const std::string& filename,
                   matrix<int32>& theChannel) {
    // open file to read
    return loadMatrix(filename,theChannel,theHeader_);
  }
  
  bool ioLTI::load(const std::string& filename,
                   matrix<int32>& theChannel,
                   lti::palette& pal) {
    pal.clear();
    // open file to read
    return loadMatrix(filename,theChannel,theHeader_);
  }
  

  bool ioLTI::load(std::ifstream& in, image& theImage) {
    // open file to read
    return loadMatrix(in,theImage,theHeader_);
  }

  bool ioLTI::load(std::ifstream& in, matrix<float>& theChannel) {
    // open file to read
    return loadMatrix(in,theChannel,theHeader_);
  }

  // checkfile
  bool ioLTI::checkHeader(const std::string& filename,
                          headerInformation& info) {
    info.type = "";
    info.bpp = 0;
    info.size.set(0,0);
    info.colorEntries = 0;

    bool result = true;

    // open file to read
    std::ifstream in;
    in.open(filename.c_str(),std::ios::in | std::ios::binary);
    if (in.good() && in.is_open()) {
      // read header and infoheader
      result = theHeader_.read(in);
      info.type = "LTI";
      info.info = char(theHeader_.contents);
      switch(theHeader_.contents) {
      case 'b':
        info.bpp = 8*sizeof(ubyte);
        break;
      case 'i':
        info.bpp = 8*sizeof(uint32);
        break;
      case 'f':
        info.bpp = 8*sizeof(float);
        break;
      case 'd':
        info.bpp = 8*sizeof(double);
        break;
      case 'c':
        info.bpp = 8*sizeof(rgbaPixel);
        break;
      case 'w':
        info.bpp = 8*sizeof(fcomplex);
        break;
      case 'z':
        info.bpp = 8*sizeof(dcomplex);
        break;
      default:
        result = false;
      }

      info.size.set(theHeader_.columns,theHeader_.rows);
    } // if (good) ...
    else {
      in.close();
      setStatusString("Could not open LTI file: ");
      appendStatusString(filename.c_str());
      return false;
    }

    in.close();
    return result;
  }

  // apply
  bool ioLTI::save(const std::string& filename,const image& theImage) {
    return saveMatrix(filename,theImage,theHeader_,getParameters().codec);
  }

  /** saves 8-bit channel
  */
  bool ioLTI::save(const std::string& filename,
                     const matrix<ubyte>& theChannel,
                     const lti::palette& ) {
    return saveMatrix(filename,theChannel,theHeader_,getParameters().codec);
  }

  bool ioLTI::save(const std::string& filename,
                   const matrix<ubyte>& chnl) {
    return saveMatrix(filename,chnl,theHeader_,getParameters().codec);
  }
                 
  bool ioLTI::save(const std::string& filename,
                   const matrix<float>& chnl) {
    return saveMatrix(filename,chnl,theHeader_,getParameters().codec);
  }
  
  bool ioLTI::save(const std::string& filename,
                   const matrix<int32>& chnl,
                   const lti::palette&) {
    return saveMatrix(filename,chnl,theHeader_,getParameters().codec);
  }
  
  bool ioLTI::save(const std::string& filename,
                   const matrix<int32>& chnl) {
    return saveMatrix(filename,chnl,theHeader_,getParameters().codec);
  }

  /*
   * Read a eExtremaType
   *
   * @ingroup gStorable
   */
  bool read(ioHandler& handler,ioLTI::eCompressionType& data) {
    std::string str;
    if (handler.read(str)) {
      if (str.find("one") != std::string::npos) {
        data = ioLTI::None;
      } else if (str.find("ength") != std::string::npos) {
        data = ioLTI::RunLength;
      } else if (str.find("late") != std::string::npos) {
        data = ioLTI::Flate;
      } else {
        data = ioLTI::Other;
      }

      return true;
    }

    return false;
  }

  /*
   * Write a eExtremaType
   *
   * @ingroup gStorable
   */
  bool write(ioHandler& handler,const ioLTI::eCompressionType& data) {
    bool b=false;
    switch(data) {
      case ioLTI::None:
        b = handler.write("None");
        break;
      case ioLTI::RunLength:
        b = handler.write("RunLength");
        break;
      case ioLTI::Flate:
        b = handler.write("Flate");
        break;
      default:
        b = handler.write("Other");
        break;
    }
    return b;
  }


} // namespace lti

