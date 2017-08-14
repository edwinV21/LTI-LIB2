/*
 * Copyright (C) 1998, 1999, 2000, 2001
 * Lehrstuhl fuer Technische Informatik, RWTH-Aachen, Germany
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
 * \file   ltiIOBMP.cpp
 *         Contains the class lti::ioBMP
 *         which implements IO for images with BMP file format
 * 
 * \author Pablo Alvarado
 * \date   23.04.1999
 *
 * revisions ..: $Id: ltiIOBMP.cpp,v 1.10 2012-09-16 04:58:33 alvarado Exp $
 */

#include "ltiIOBMP.h"
#include "ltiRGBAPixel.h"
#include "ltiImage.h"
#include "ltiUsePalette.h"
#include "ltiKMColorQuantization.h"
#include "ltiMergeRGBToImage.h"
#include "ltiFactory.h"
#include "ltiEndianness.h"

#include <fstream>
#include <vector>
#include <map>

namespace lti {

  // In ioImageInterface register this as reader/writer of PNG files
  _LTI_REGISTER_IN_FACTORY_AS(BMP,ioImageInterface,ioBMP)

  // -----------------------------------------------------------------------
  // ioBMP::parameters
  // -----------------------------------------------------------------------

  ioBMP::parameters::parameters() : ioImageInterface::parameters() {
    compression = true;
    bitsPerPixel = 24;
  }
  
  ioBMP::parameters::parameters(const parameters& other) 
    : ioImageInterface::parameters() {
    copy(other);
  }
  
  ioBMP::parameters& ioBMP::parameters::copy(const parameters& other) {
    ioImageInterface::parameters::copy(other);
    compression = other.compression;
    bitsPerPixel = other.bitsPerPixel;

    return (*this);
  }

  const std::string& ioBMP::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  ioBMP::parameters* ioBMP::parameters::clone() const  {
    return (new parameters(*this));
  }

  ioBMP::parameters* ioBMP::parameters::newInstance() const  {
    return (new parameters());
  }

  /*
   * write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool ioBMP::parameters::write(ioHandler& handler,
                                const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }
    
    if (b) {
      lti::write(handler,"compression",compression);
      lti::write(handler,"bitsPerPixel",bitsPerPixel);
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
  bool ioBMP::parameters::read(ioHandler& handler,
                               const bool complete) {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {
      lti::read(handler,"compression",compression);
      lti::read(handler,"bitsPerPixel",bitsPerPixel);
    }

    b = b && ioImageInterface::parameters::read(handler,false);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

  // -------------------------------------------------------
  //  ioBMP::header
  // -------------------------------------------------------

  ioBMP::header::header() : type(0x4d42),size(0),
                            reserved1(0),reserved2(0),offsetPixels(0) {
  }

  bool ioBMP::header::read(std::ifstream &in) {
    typedef endianness<LittleEndian> io;
    
    uint16 tmpType,tmpReserved;
    io::read(in,tmpType);  // io reads with the correct endianness

    if (tmpType != type) { // This means BM, and is mandatory for a BMP file!
      return false;
    }

    io::read(in,size);
    io::read(in,tmpReserved); // just ignore the readed data!
    io::read(in,tmpReserved); // just ignore the readed data!
    io::read(in,offsetPixels);

    return true;
  }

  bool ioBMP::header::write(std::ofstream &out) const {
    typedef endianness<LittleEndian> io;


    io::write(out,type);  // io writes with the correct endianness
    io::write(out,size);
    io::write(out,reserved1);
    io::write(out,reserved2);
    io::write(out,offsetPixels);

    return true;
  }

  // -------------------------------------------------------
  //  ioBMP::infoHeader
  // -------------------------------------------------------

  ioBMP::infoHeader::infoHeader()
    : size(40), width(0), height(0),
      planes(0), bitCount(0),compression(0),sizeImage(0),xPixPerMeter(0),
      yPixPerMeter(0),colorsUsed(0),colorsImportant(0) {
  }

  bool ioBMP::infoHeader::read(std::ifstream &in) {
    typedef endianness<LittleEndian> io;

    uint32 tmpSize; // counter to check if the size of the header is right

    // read size of this header
    io::read(in,size);
    tmpSize  = sizeof(size);
    if (tmpSize < size) {
      io::read(in,width);
      tmpSize += sizeof(width);
    }
    if (tmpSize < size) {
      io::read(in,height);
      tmpSize += sizeof(height);
    }
    if (tmpSize < size) {
      io::read(in,planes);
      tmpSize += sizeof(planes);
    }
    if (tmpSize < size) {
      io::read(in,bitCount);
      tmpSize += sizeof(bitCount);
    }
    if (tmpSize < size) {
      io::read(in,compression);
      tmpSize += sizeof(compression);
    }
    if (tmpSize < size) {
      io::read(in,sizeImage);
      tmpSize += sizeof(sizeImage);
    }
    if (tmpSize < size) {
      io::read(in,xPixPerMeter);
      tmpSize += sizeof(xPixPerMeter);
    }
    if (tmpSize < size) {
      io::read(in,yPixPerMeter);
      tmpSize += sizeof(yPixPerMeter);
    }
    if (tmpSize < size) {
      io::read(in,colorsUsed);
      tmpSize += sizeof(colorsUsed);
    }
    if (tmpSize < size) {
      io::read(in,colorsImportant);
      tmpSize += sizeof(colorsImportant);
    }

    if (size > tmpSize) {
      // load extra data
      char* buffer;
      buffer = new char[size-tmpSize+2];
      in.read(buffer,size-tmpSize);
      delete[] buffer;
    }

    return true;
  }

  bool ioBMP::infoHeader::write(std::ofstream &out) const {
    typedef endianness<LittleEndian> io;

    size = length();
    io::write(out,size);
    io::write(out,width);
    io::write(out,height);
    io::write(out,planes);
    io::write(out,bitCount);
    io::write(out,compression);
    io::write(out,sizeImage);
    io::write(out,xPixPerMeter);
    io::write(out,yPixPerMeter);
    io::write(out,colorsUsed);
    io::write(out,colorsImportant);

    return true;
  }

  // -------------------------------------------------------
  //  ioBMP::palette
  // -------------------------------------------------------
  ioBMP::palette::palette(int entries) : vector<rgbaPixel>(entries) {
  }

  bool ioBMP::palette::read(std::ifstream &in) {
    typedef endianness<LittleEndian> io;
    int i;
    uint32 v;
    for (i=0;(i<size()) && in.good() && in.is_open() && (!in.eof());i++) {
      io::read(in,v); // read with correct endianness!
      at(i).setValue(v);
    }

    return true;
  }

  bool ioBMP::palette::write(std::ofstream &out) const {
    typedef endianness<LittleEndian> io;
    int i;
    for (i=0;i<size();i++) {
      io::write(out,at(i).getValue()); // write with correct endianness!
    }
    return true;
  }


  // ----------------------------------------------------------------------
  // ioBMP
  // ----------------------------------------------------------------------

  // constructor
  ioBMP::ioBMP() : ioImageInterface() {
    parameters par;
    setParameters(par);
  }

  ioBMP::ioBMP(const parameters& par) : ioImageInterface() {
    setParameters(par);
  }

  ioBMP::~ioBMP() {
  }

  // returns the current parameters
  const ioBMP::parameters& ioBMP::getParameters() const {
    const parameters* params =
      dynamic_cast<const parameters*>(&ioImageInterface::getParameters());
    if (isNull(params)) {
      throw invalidParametersException(name());
    }
    return *params;
  }

  const std::string& ioBMP::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  ioBMP* ioBMP::clone() const {
    return new ioBMP(*this);
  }

  ioBMP* ioBMP::newInstance() const {
    return new ioBMP();
  }

  // read file
  bool ioBMP::load(const std::string& filename,image& theImage) {
    
    // open file to read
    std::ifstream in;
    
    in.open(filename.c_str(),std::ios::in | std::ios::binary);
    bool success=load(in,theImage);
    in.close();
    return success;
  }

  // read file
  bool ioBMP::load(const std::string& filename,
                     matrix<ubyte>& theChannel,
                     lti::palette& colors) {
    
    image img;
    int bpp = load(filename,img,theChannel,colors);
    if ((bpp == 0) || (bpp > 8)) {
      theChannel.clear();
      colors.clear();
      setStatusString("24 bit image cannot be casted to channel");
      return false;
    }
    return true;
  }

  bool ioBMP::load(const std::string& filename,
                   matrix<ubyte>& theChannel) {
    return ioImageInterface::load(filename,theChannel);
  }

  bool ioBMP::load(const std::string& filename,
                     matrix<float>& theChannel) {
    return ioImageInterface::load(filename,theChannel);
  }

  bool ioBMP::load(const std::string& filename,
                     matrix<int32>& theChannel) {
    return ioImageInterface::load(filename,theChannel);
  }
  
  bool ioBMP::load(const std::string& filename,
                     matrix<int32>& theChannel,
                     lti::palette& pal) {
    return ioImageInterface::load(filename,theChannel,pal);
  }
  

  bool ioBMP::load(std::ifstream& in, image& theImage) {

    matrix<ubyte> chnl;
    lti::palette pal;
    
    int bpp = load(in,theImage,chnl,pal);

    if (bpp == 0) {
      return false;
    } else if (bpp == 24) {
      return true;
    }

    usePalette applyPalette;
    return applyPalette.apply(chnl,pal,theImage);
  }

  // read file
  int ioBMP::load(const std::string& filename,
                  image& theImage,
                  matrix<ubyte>& theChannel,
                  lti::palette& colors) {
    
    std::ifstream in;
    in.open(filename.c_str(),std::ios::in | std::ios::binary);
    int ret = load(in,theImage,theChannel,colors);
    in.close();
    return ret;
  }

  // read file
  int ioBMP::load(std::ifstream& file,
                  image& theImage,
                  matrix<ubyte>& theChannel,
                  lti::palette& colors) {
    
    bool success = false;
    // open file to read
    if (file.good() && file.is_open()) {
      // read header and infoheader
      if (!theHeader_.read(file)) {
        setStatusString("Wrong header. Is this a BMP file?");
        return false;
      }
      if (!theInfoHeader_.read(file)) {
        setStatusString("Wrong info header");
        return false;
      }

      // how long is the color table?
      int colorEntries;
      colorEntries = theHeader_.offsetPixels -
        theInfoHeader_.size -
        theHeader_.length();
      
      // this is a valid value only if it's divisible by 4!
      if ((colorEntries % 4) != 0) {
        
        theChannel.clear();
        theImage.clear();
        colors.clear();

        setStatusString("Invalid number of color entries in BMP file");
        return 0;
      }

      colorEntries /= 4;
      
      // read color entries
      thePalette_.allocate(colorEntries);
      thePalette_.read(file);
      colors.copy(thePalette_);

      if (theInfoHeader_.bitCount < 24) {
        theChannel.allocate(theInfoHeader_.height,theInfoHeader_.width);
      } else {
        theImage.allocate(theInfoHeader_.height,theInfoHeader_.width);
        colors.clear();
      }

      // read image
      switch (theInfoHeader_.bitCount) {
        case 1: {
          success = load1bit(file,theChannel);
        }
          break;
        case 4: {
          success = load4bit(file,theChannel);
        }
          break;
        case 8: {
          success = load8bit(file,theChannel);
        }
          break;
        case 24: {
          success = load24bit(file,theImage);
        }
          break;
        default:
          // unknown bit/pixel
          
          theChannel.clear();
          theImage.clear();
          colors.clear();

          setStatusString("Invalid number of pixels in BMP file");
          return 0;
      }
    } // if (good) ...
    else {
      setStatusString("Could not open BMP file (Does it exist?)");
    }

    return (success ? theInfoHeader_.bitCount : 0);
  }

  // checkfile
  bool ioBMP::checkHeader(const std::string& filename,
                          headerInformation& info) {
    info.type = "BMP";
    info.bpp = 0;
    info.size.set(0,0);
    info.colorEntries = 0;

    bool result = true;

    // open file to read
    std::ifstream in;
    in.open(filename.c_str(),std::ios::in | std::ios::binary);
    if (in.good() && in.is_open()) {
      // read header and infoheader
      result = result && theHeader_.read(in);
      result = result && theInfoHeader_.read(in);

      // how long is the color table?
      info.colorEntries = (theHeader_.offsetPixels -
                           theInfoHeader_.size -
                           theHeader_.length());

      // this is a valid value only if it's divisible by 4!
      if ((info.colorEntries % 4) != 0) {
        in.close();
        setStatusString("Invalid number of color entries in BMP file");
        return false;
      }
      
      info.colorEntries /= 4;
      
      info.size.set(theInfoHeader_.width,theInfoHeader_.height);
      info.bpp = theInfoHeader_.bitCount;
    } // if (good) ...
    else {
      in.close();
      setStatusString("Could not open BMP file: ");
      appendStatusString(filename.c_str());
      return false;
    }

    in.close();
    return result;
  }

  bool ioBMP::load1bit(std::ifstream& in,matrix<ubyte>& theChannel) {
    unsigned int x,t;
    int y,mc,b;
    ubyte someBits;

    ubyte* buffer;
    unsigned int bufferSize;

    // every line has 4*i elements (with i integer)
    // the number of rest-bytes 'b' in the file, can be calculated as
    // t = (4 - (x mod 4)) mod 4;
    x = (theInfoHeader_.width+7)/8;  // number of bytes required per line
    t = (4-(x%4))%4;

    bufferSize = (x+t)*theInfoHeader_.height;

    if ((theInfoHeader_.sizeImage>0)&&
        (bufferSize > theInfoHeader_.sizeImage)) {
      setStatusString("Invalid header in BMP file");
      return false;
    }

    if (theInfoHeader_.sizeImage > bufferSize) {
      bufferSize = theInfoHeader_.sizeImage;
    };

    buffer = new ubyte[bufferSize];
    // read the data
    in.read(reinterpret_cast<char*>(buffer),bufferSize);

    mc = 0;
    for (y = theInfoHeader_.height-1;y>=0;y--) {
      x = 0;
      while (x < theInfoHeader_.width) {
        // read 1 byte
        someBits = buffer[mc++];
        // extract bits
        for (b=0;(b<8) && (x<theInfoHeader_.width);b++,x++) {
          theChannel.at(y,x) = ((someBits & 0x80) == 0) ? 0 : 1;
          someBits = someBits << 1; // shift to the right!
        }
      }
      mc+=t;
    }
    delete[] buffer;

    return true;
  }

  bool ioBMP::load4bit(std::ifstream& in,matrix<ubyte>& theChannel) {
    unsigned int x,t;
    int y,mc;
    ubyte someNybbles;

    switch (theInfoHeader_.compression) {
      case 0: { // no compression
        ubyte* buffer;
        unsigned int bufferSize;

        // every line must have 4*i bytes (with i integer)
        // the number of rest-bytes 't' in the file, can be calculated as
        // t = (4 - (x mod 4)) mod 4;
        x = (theInfoHeader_.width+1)/2; //number of bytes used in a line!
        t = (4 - (x % 4)) % 4;

        bufferSize = (x+t)*theInfoHeader_.height;

        if ((theInfoHeader_.sizeImage>0)&&
            (bufferSize > theInfoHeader_.sizeImage)) {
          setStatusString("Invalid header in BMP file");
          return false;
        }

        if (theInfoHeader_.sizeImage > bufferSize) {
          bufferSize = theInfoHeader_.sizeImage;
        }

        buffer = new ubyte[bufferSize];
        // read the data
        in.read(reinterpret_cast<char*>(buffer),bufferSize);


        mc = 0; // memory counter : position in the buffer
        // no compression
        for (y = theInfoHeader_.height-1;y>=0;y--) {
          x = 0;
          while (x < theInfoHeader_.width) {
            // read 1 byte
            someNybbles = buffer[mc++];
            // extract bits
            theChannel.at(y,x++) = someNybbles >> 4; // first four bits!
            if (x<theInfoHeader_.width) {
              theChannel.at(y,x++) = someNybbles & 0x0F; // shift to the right!
            }
          }
          mc+=t;
        }
        delete[] buffer;
      }
        break;

      case 1: {
        // RLE 8 compression
        setStatusString("RLE 8 not available for 4-bit images");
        return false;
        // not avalilable for 4-bit
      }
        break;

      case 2: { // RLE 4 compression
        ubyte num,c;

        // first load the whole image file in a memory buffer (faster!)
        ubyte* buffer;
        const int bufferSize = theInfoHeader_.sizeImage;

        buffer = new ubyte[bufferSize];
        // read the data
        in.read(reinterpret_cast<char*>(buffer),bufferSize);

        x = 0;
        y = theInfoHeader_.height-1;
        theChannel.fill(ubyte(0));
        mc = 0; // memory counter: position in the buffer

        while (y>=0) {
          // read 1 byte
          num = buffer[mc++];

          if (num == 0) {
            // read command
            num = buffer[mc++];
            if (num<3) { // is command
              switch (num) {
                case 0: // fill line with 0! (ready!)
                  x = 0;
                  y--;
                  break;
                case 1: // end of graphic
                  x = 0;
                  y = -1; // flag to indicate EOG
                  break;
                case 2:
                  x+=buffer[mc++]; // offset x
                  y+=buffer[mc++]; // offset y
              }
            } else { // no command -> load normal data stream
              int low,high;
              for (int i=0; i<num; i++) {
                c = buffer[mc++];
                high = (c >> 4) & 0x0F;
                theChannel.at(y,x++) = high;
                i++;
                if (i<num) {
                  low = c & 0x0F;
                  theChannel.at(y,x++) = low;
                }
              }
              if ((mc & 0x01) != 0) { // odd?
                // read dummy value
                mc++;
              }
            }
          } else { // no special command
            c = buffer[mc++]; // c must be copied 'num'-times
            int high,low;
            low = c & 0x0F;
            high = (c >> 4) & 0x0F;

            for (int i=0;i<num;i++) {
              theChannel.at(y,x++) = high;
              i++;
              if (i<num) {
                theChannel.at(y,x++) = low;
              }
            }
          }
        }
        delete[] buffer;
      }
        break;
    }

    return true;
  }

  bool ioBMP::load8bit(std::ifstream& in,matrix<ubyte>& theChannel) {
    switch(theInfoHeader_.compression) {
      case 0: {
        // no compression
        unsigned int x,t,bufferSize,lineSize;
        int y,i;
        ubyte* buffer;

        // first read all data to a buffer!
        // calculate how big must be the buffer?
        // every line have 4*i byte elements (with i integer)
        // the number of rest-bytes 't' in the file, can be calculated as
        // t = (4 - (columns mod 4)) mod 4;
        x = theInfoHeader_.width; // real number of bytes pro line
        t = (4 - (x % 4)) % 4;
        bufferSize = (x+t)*theInfoHeader_.height;
        lineSize = t+theInfoHeader_.width;

        if ((theInfoHeader_.sizeImage>0)&&
            (bufferSize > theInfoHeader_.sizeImage)) {
          setStatusString("Invalid header in BMP file");
          return false;
        }

        buffer = new ubyte[bufferSize];

        // read the data
        in.read(reinterpret_cast<char*>(buffer),bufferSize);

        // all data readed: now transform to a channel
        i = 0;
        for (y = theInfoHeader_.height-1;y>=0;y--) {
          memcpy(&theChannel.at(y,0),&buffer[i], theInfoHeader_.width);
          i+=lineSize;
        }

        delete[] buffer;
      } break;
      case 1: { // RLE8
        ubyte num,c;
        unsigned int x;
        int y,mc;

        // first load the whole image file in a memory buffer (faster!)
        ubyte* buffer;
        const int bufferSize = theInfoHeader_.sizeImage;

        buffer = new ubyte[bufferSize];
        // read the data
        in.read(reinterpret_cast<char*>(buffer),bufferSize);

        x = 0;
        y = theInfoHeader_.height-1;
        mc = 0; // memory counter: position in the buffer

        theChannel.fill(matrix<ubyte>::value_type(0));

        while (y>=0) {
          // 1 byte lesen
          num = buffer[mc++];

          if (num == 0) {
            // read command
            num = buffer[mc++];
            if (num<3) {
              // is command
              switch (num) {
                case 0: // fill line with 0! (ready!)
                  x = 0;
                  y--;
                  break;
                case 1: // end of graphic
                  x = 0;
                  y = -1; // flag to indicate EOG
                  break;
                case 2:
                  x+=buffer[mc++];// offset X
                  y-=buffer[mc++];
              }
            }
            else { // no command -> normal data
              memcpy(&theChannel.at(y,x),&buffer[mc],num);
              x+=num;
              mc+=num;
              if ((num & 0x01) != 0) { // even number of data must be read!
                mc++;
              }
            }
          }
          else { // no special command
            c = buffer[mc++]; // c must be copied 'num'-times
            memset(&theChannel.at(y,x),c,min(num,
                                             ubyte(theChannel.columns()-x)));
            x+=num;
          }
        }
        delete[] buffer;
      } break;
      case 2: {// RLE4
        // not available for 8 bits
        setStatusString("RLE4 not available for 8bit images");
        return false;
      } break;
      default:
        // a new BMP format?
        setStatusString("Compression mode not supported yet");
        return false;
    }

    return true;
  }

  bool ioBMP::load24bit(std::ifstream& in,image& theImage) {

    // first read all data to a buffer!
    ubyte* buffer;

    // calculate how big must be the buffer?
    // every line have 4*i byte elements (with i integer)
    // the number of rest-bytes 't' in the file, can be calculated as
    // t = (4 - (columns mod 4)) mod 4;

    const int x = 3*theInfoHeader_.width; // real number of bytes pro line
    const int t = (4 - (x % 4)) % 4;
    const unsigned int bufferSize = (x+t)*theInfoHeader_.height;

    int y;

    if ((theInfoHeader_.sizeImage>0)&&
        (bufferSize > theInfoHeader_.sizeImage)) {
      setStatusString("Invalid header in BMP file");
      return false;
    }

    buffer = new ubyte[bufferSize];

    // read the data
    in.read(reinterpret_cast<char*>(buffer),bufferSize);

    ubyte* ptr = buffer;
    vector<rgbaPixel>::iterator it,eit;
    
    for (y = theInfoHeader_.height-1;y>=0;--y) {
      vector<rgbaPixel>& vct = theImage.getRow(y);
      for (it=vct.begin(),eit=vct.end();it!=eit;++it,ptr+=3) {
        (*it).set(ptr[2],ptr[1],*ptr,0);
      }
      ptr+=t;
    }

    delete[] buffer;

    return true;
  }



  // apply
  bool ioBMP::save(const std::string& filename,const image& theImage) {
    bool success = false;

    const int bpp = getParameters().bitsPerPixel;
    if (bpp > 8) {
      // open file to write
      std::ofstream out;
      out.open(filename.c_str(),std::ios::out | std::ios::binary);

      if (out.good() && out.is_open()) {
        success = save24bit(out,theImage);
      } else {
        setStatusString("BMP file ");
        appendStatusString(filename);
        appendStatusString(" could not be written. Is the path correct?");
      }
      out.close();
    } else {
      kMColorQuantization::parameters kmcqPar;
      kmcqPar.numberOfColors = (1 << bpp); // equivalent to 2^bpp
      kMColorQuantization quant(kmcqPar);
      matrix<ubyte> mask;
      lti::palette pal;
      
      quant.apply(theImage,mask,pal);
      
      return save(filename,mask,pal);
    }

    return success;
  }

  /** saves 8-bit channel
      see ioBMP::parameters to the possible effects of this member.
  */
  bool ioBMP::save(const std::string& filename,
                     const matrix<ubyte>& theChannel,
                     const lti::palette& colors) {
    
    if (colors.empty()) { // empty palette?
      thePalette_.copy(getGrayPalette());  // use gray scale
    } else {
      thePalette_.copy(colors);
    }

    bool success = false;
    // open file to write
    std::ofstream out;
    out.open(filename.c_str(),std::ios::out | std::ios::binary);

    if (out.good() && out.is_open()) {
      switch(getParameters().bitsPerPixel) {
        case 1: {
          success = save1bit(out,theChannel);
        } break;
        case 4: {
          success = save4bit(out,theChannel);
        } break;
        case 8: {
          success = save8bit(out,theChannel);
        } break;
        case 24: {
          image anImage;
          usePalette applyPalette;
          applyPalette.apply(theChannel,thePalette_,anImage);
          success = save24bit(out,anImage);
        } break;
        default: {
          success = false;
        }
      }
    } else {
      setStatusString("BMP file ");
      appendStatusString(filename);
      appendStatusString(" could not be written. Is the path correct?");
    }
    out.close();

    return success;
  }

  bool ioBMP::save(const std::string& filename,
                   const matrix<ubyte>& chnl) {
    return ioImageInterface::save(filename,chnl);
  }
                 
  bool ioBMP::save(const std::string& filename,
                   const matrix<float>& chnl) {
    return ioImageInterface::save(filename,chnl);
  }
  
  bool ioBMP::save(const std::string& filename,
                   const matrix<int32>& chnl,
                   const lti::palette& pal) {
    return ioImageInterface::save(filename,chnl,pal);
  }
  
  bool ioBMP::save(const std::string& filename,
                   const matrix<int32>& chnl) {
    return ioImageInterface::save(filename,chnl);
  }

  bool ioBMP::save24bit(std::ofstream& out,const image& theImage) {
    int x,y,mc,t;
    int tmpSize = 0;
    rgbaPixel p;

    tmpSize += theHeader_.length();
    tmpSize += theInfoHeader_.length();

    x = 3*theImage.columns(); // real number of bytes pro line
    t = (4 - (x % 4)) % 4;

    tmpSize += (x+t)*theImage.rows();

    theHeader_.size = tmpSize;
    theHeader_.offsetPixels = theHeader_.length() + theInfoHeader_.length();

    theInfoHeader_.size = theInfoHeader_.length();
    theInfoHeader_.width = theImage.columns();
    theInfoHeader_.height = theImage.rows();
    theInfoHeader_.planes = 1;
    theInfoHeader_.bitCount = 24;
    theInfoHeader_.sizeImage = (x+t)*theImage.rows();
    theInfoHeader_.xPixPerMeter = 2835; // 72 dpi
    theInfoHeader_.yPixPerMeter = 2835; // 72 dpi
    theInfoHeader_.compression = 0; // no compression?
    theInfoHeader_.colorsUsed = 0;
    theInfoHeader_.colorsImportant = 0; // All

    ubyte* buffer = new ubyte[theInfoHeader_.sizeImage];
    ubyte* bufferRed  = &buffer[2];
    ubyte* bufferGreen= &buffer[1];
    ubyte* bufferBlue = &buffer[0];

    // construct the buffer
    mc = 0; // memory counter: position in the buffer
    for (y=theImage.rows()-1;y>=0;y--) {
      for (x=0;x<theImage.columns();x++) {
        p=theImage.at(y,x);

        bufferRed[mc]   = p.getRed();
        bufferGreen[mc] = p.getGreen();
        bufferBlue[mc]  = p.getBlue();

        mc+=3;
      }

      for (x=1;x<=t;x++) {
        buffer[mc++] = 0;
      }
    }

    // write header and infoheader
    theHeader_.write(out);
    theInfoHeader_.write(out);
    // write buffer
    out.write(reinterpret_cast<const char*>(buffer), theInfoHeader_.sizeImage);
    delete[] buffer;

    return true;
  }

  bool ioBMP::save8bit(std::ofstream& out,const matrix<ubyte>& theChannel) {
    int x,y,c,t,i;
    uint32 mc;
    int tmpSize;

    ubyte* buffer = 0;

    // calculate header values
    theHeader_.offsetPixels = theHeader_.length() +
      theInfoHeader_.length() +
      thePalette_.size()*4;

    tmpSize = 0;
    tmpSize += theHeader_.length();
    tmpSize += theInfoHeader_.length();
    tmpSize += thePalette_.size()*4;

    x = theChannel.columns(); // real number of bytes pro line
    t = (4 - (x % 4)) % 4;

    theInfoHeader_.sizeImage = (x+t)*theChannel.rows();

    tmpSize += (x+t)*theChannel.rows();

    theHeader_.size = tmpSize;

    theInfoHeader_.size = theInfoHeader_.length();
    theInfoHeader_.width = theChannel.columns();
    theInfoHeader_.height = theChannel.rows();
    theInfoHeader_.planes = 1;
    theInfoHeader_.bitCount = 8;

    theInfoHeader_.xPixPerMeter = 2835; // 72 dpi
    theInfoHeader_.yPixPerMeter = 2835; // 72 dpi
    // RLE8 compression?
    theInfoHeader_.compression = (getParameters().compression) ? 1 : 0;
    theInfoHeader_.colorsUsed = 256;
    theInfoHeader_.colorsImportant = 0; // All

    // ------
    //  compresion required?

    if (theInfoHeader_.compression) {
      // try to compress the data with RLE8
      mc = 0;
      // allocate memory for the buffer, and reserve some space for
      // the case in which the compression won't work!
      buffer = new ubyte[theInfoHeader_.sizeImage+theChannel.columns()];
      for (y=theChannel.rows()-1;y>=0;y--) {
        x = 0;
        while (x<theChannel.columns()) {
          // how many pixel repetitions?
          for (c = 1,i = x+1;
               (i<theChannel.columns()) &&
                 (theChannel.at(y,x)==theChannel.at(y,i));
               i++,c++) {
            // c will contain the number of repetition for the pixel
          }
          if (c>1) { // pixel more than once repeated!
            // belong these pixels to the end of the line?
            if ((c+x>=theChannel.columns())&&(theChannel.at(y,x)==0)) {
              // yes!  flag to exit:
              x=theChannel.columns();
            } else {
              buffer[mc++]=c; // code repeat value c times!
              buffer[mc++]=theChannel.at(y,x);
              x+=c;           // new x
            }
          } else {
            // stand alone value: try to code

            // count how many different pixels come...
            for (c=0,i=x+1;
                 (i<theChannel.columns()) &&
                   (theChannel.at(y,i-1) != (theChannel.at(y,i)));
                 c++,i++) {
              // c will contain the number of different pixels
            }

            if (c<3) {
              // just 1 pixel:
              buffer[mc++]=1;
              buffer[mc++]=theChannel.at(y,x);
              x++;
              if (c==2) { // or 2 pixel
                buffer[mc++]=1;
                buffer[mc++]=theChannel.at(y,x);
                x++;
              }
            } else {
              // write the block as is:
              buffer[mc++]=0;
              buffer[mc++]=c;
              memcpy(&buffer[mc],&theChannel.at(y,x),c);
              mc+=c;
              if ((c & 0x01) != 0) { // odd number of bytes?
                buffer[mc++]=0;  // align the buffer to be even!
              }
              x+=c;
            }
          }
        }
        // mark end of line with 0,0!
        buffer[mc++]=0;
        buffer[mc++]=0;

        if (mc>theInfoHeader_.sizeImage) {
          // compression didn't work... just save the normal image
          delete[] buffer;
          buffer = 0;
          theInfoHeader_.compression = false; // save as no compressed!
          y = -1; // flag to the end of compression!
        }
      }

      // if the compression was ok
      if (theInfoHeader_.compression) {
        // mark the end of the graphic with 0,1
        buffer[mc++]=0;
        buffer[mc++]=1;

        theInfoHeader_.sizeImage=mc;
        theHeader_.size = theHeader_.length()+theInfoHeader_.length()+
          thePalette_.size()*4+mc;
      }
    }

    // if compression is not required or if compressed data is bigger than
    // the original:

    if (!theInfoHeader_.compression) {
      // channel data
      buffer = new ubyte[theInfoHeader_.sizeImage];

      // construct the buffer
      mc = 0; // memory counter: position in the buffer
      for (y=theChannel.rows()-1;y>=0;y--) {
        memcpy(&buffer[mc],&theChannel.at(y,0),theInfoHeader_.width);
        mc += (theInfoHeader_.width+t);
        //for (x=1;x<=t;x++) {
        //  buffer[mc++] = 0;
        //}
      }
    }

    // write header and infoheader
    theHeader_.write(out);
    theInfoHeader_.write(out);
    // write palette
    thePalette_.write(out);
    // write buffer
    out.write(reinterpret_cast<const char*>(buffer),theInfoHeader_.sizeImage);

    delete[] buffer;

    return true;
  }

  bool ioBMP::save4bit(std::ofstream& out,const matrix<ubyte>& theChannel) {
    // maybe one day we will implement this...
    return save8bit(out,theChannel);
  }

  bool ioBMP::save1bit(std::ofstream& out,const matrix<ubyte>& theChannel) {
    int x,y,t,b;
    uint32 mc;
    int tmpSize;
    ubyte theByte;

    ubyte* buffer = 0;

    // calculate header values
    theHeader_.offsetPixels = theHeader_.length() +
      theInfoHeader_.length() +
      thePalette_.size()*4;

    tmpSize = 0;
    tmpSize += theHeader_.length();
    tmpSize += theInfoHeader_.length();
    tmpSize += thePalette_.size()*4;

    x = (theChannel.columns()+7)/8; // real number of bytes pro line
    t = (4 - (x % 4)) % 4;          // fill bytes at the end of a line

    theInfoHeader_.sizeImage = (x+t)*theChannel.rows();
    tmpSize +=  theInfoHeader_.sizeImage;

    theHeader_.size = tmpSize;

    theInfoHeader_.size = theInfoHeader_.length();
    theInfoHeader_.width = theChannel.columns();
    theInfoHeader_.height = theChannel.rows();
    theInfoHeader_.planes = 1;
    theInfoHeader_.bitCount = 1;

    theInfoHeader_.xPixPerMeter = 2835; // 72 dpi
    theInfoHeader_.yPixPerMeter = 2835; // 72 dpi
    theInfoHeader_.compression = 0;
    theInfoHeader_.colorsUsed = 2;
    theInfoHeader_.colorsImportant = 2; // All


    // channel data
    buffer = new ubyte[theInfoHeader_.sizeImage];

    // construct the buffer
    mc = 0; // memory counter: position in the buffer
    for (y=theChannel.rows()-1;y>=0;y--) {
      theByte = 0;
      b = 0x80;
      for (x=0;x<theChannel.columns();x++) {
        if (theChannel.at(y,x) != 0) {
          theByte |= b;
        }
        b = b >> 1; // shift to the right
        if (b==0) {             // 8-bit already accumulated...
          buffer[mc++]=theByte; // ...let them out!
          b=0x80;
          theByte = 0;
        }
      }
      if (b<0x80) {// last byte hasn't been stored jet!
        buffer[mc++]=theByte; // ...let them out!
      }

      mc+=t;
    }

    // write header and infoheader
    theHeader_.write(out);
    theInfoHeader_.write(out);
    // write palette
    thePalette_.write(out);
    // write buffer
    out.write(reinterpret_cast<const char*>(buffer), theInfoHeader_.sizeImage);

    delete[] buffer;

    return true;
  }

} // namespace lti

