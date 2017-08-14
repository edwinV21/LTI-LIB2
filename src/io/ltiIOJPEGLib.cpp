/*
 * Copyright (C) 1998-2006
 * Lehrstuhl fuer Technische Informatik, RWTH-Aachen, Germany
 *
 * This file is part of the LTI-Computer Vision Library (LTI-Lib)
 *
 * The LTI-Lib is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License (LGPL)
 * as published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * The LTI-Lib is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with the LTI-Lib; see the file LICENSE.  If
 * not, write to the Free Software Foundation, Inc., 59 Temple Place -
 * Suite 330, Boston, MA 02111-1307, USA.
 */

/** 
 * \file   ltiIOJPEGLib.cpp
 *         Contains the implementation of lti::ioJPEG class to deal with .jpeg
 *         files.
 * \author Benjamin Winkler
 * \author Pablo Alvarado
 * \date   27.10.1999 (LTI-Lib 1)
 * \date   16.01.2006 (LTI-Lib 2)
 *
 * revisions ..: $Id: ltiIOJPEGLib.cpp,v 1.8 2012-01-03 02:22:25 alvarado Exp $
 */

#include "ltiConfig.h"

// the libjpeg functor has a higher priority than the local one. 
// If HAVE_LIBJPEG is defined, then this will be compiled

#if defined(HAVE_LIBJPEG) 


#include "ltiIOJPEG.h"
#include "ltiTypes.h"
#include "ltiFactory.h"
#include "ltiUsePalette.h"
#include "ltiChannel8.h"

#include <cstdio>
#include <setjmp.h>

extern "C" {
#include <jpeglib.h>
#include <jerror.h>
}

namespace lti {

  // Error handler used in loading jpeg image
  struct my_error_mgr {
    struct jpeg_error_mgr pub;  // "public" fields
    jmp_buf setjmp_buffer;      // for return to caller
    char error_msg[256];
  };

  typedef my_error_mgr *my_error_ptr;

  // Here is the routine that will replace the standard error_exit method
  static void my_error_exit(j_common_ptr cinfo) {
    // cinfo-err really points to a myErrorMgr struct, so coerce pointer
    my_error_ptr myerr = reinterpret_cast<my_error_ptr>(cinfo->err);

    // Always display the message
    // We could postpone this until after returning, if we chose
    (*cinfo->err->format_message) (cinfo,myerr->error_msg);
    // Return control to the setjmp point
    longjmp(myerr->setjmp_buffer, 1);
  }

  // In ioImageInterface register this as reader/writer of JPEG files
  _LTI_REGISTER_IN_FACTORY_AS(JPG,ioImageInterface,ioJPEG)
  _LTI_REGISTER_IN_FACTORY_AS(JPEG,ioImageInterface,ioJPEG)

  // -----------------------------------------------------------------------
  // ioJPEG
  // -----------------------------------------------------------------------


  ioJPEG::parameters::parameters()
    : ioImageInterface::parameters() {
    quality        = 75;
    progressive    = false;
    comment        = "";
    rowsPerRestart = 0;
  }
  
  ioJPEG::parameters::parameters(const parameters& other) 
    : ioImageInterface::parameters() {
    copy(other);
  }

  ioJPEG::parameters&
  ioJPEG::parameters::copy(const ioJPEG::parameters& other) {

    ioImageInterface::parameters::copy(other);
    
    // copy additional information
    quality = other.quality;
    progressive = other.progressive;
    comment = other.comment;
    rowsPerRestart = other.rowsPerRestart;
    
    return (*this);
  }

  ioJPEG::parameters&
  ioJPEG::parameters::operator=(const ioJPEG::parameters& other) {
    return copy(other);
  }


  const std::string& ioJPEG::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  ioJPEG::parameters* ioJPEG::parameters::clone() const  {
    return (new parameters(*this));
  }

  ioJPEG::parameters* ioJPEG::parameters::newInstance() const  {
    return (new parameters(*this));
  }


  bool ioJPEG::parameters::write(ioHandler& handler,
                                 const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {
      lti::write(handler,"quality",quality);
      lti::write(handler,"progressive",progressive);
      lti::write(handler,"comment",comment);
      lti::write(handler,"rowsPerRestart",rowsPerRestart);
    }

    b = b && ioImageInterface::parameters::write(handler,false);

    if (complete) {
      b = b && handler.writeEnd();
    }

    return b;
  }

  bool ioJPEG::parameters::read(ioHandler& handler,
                                const bool complete) {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {
      lti::read(handler,"quality",quality);
      lti::read(handler,"progressive",progressive);
      lti::read(handler,"comment",comment);
      lti::read(handler,"rowsPerRestart",rowsPerRestart);
    }

    b = b && ioImageInterface::parameters::read(handler,false);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

  // ----------------------------------------------------------------------
  // ioJPEG
  // ----------------------------------------------------------------------

  // constructor
  ioJPEG::ioJPEG() : ioImageInterface() {
    parameters defPar;
    setParameters(defPar);
  }

  ioJPEG::ioJPEG(const ioJPEG::parameters& param) : ioImageInterface() {
    setParameters(param);
  }

  ioJPEG::ioJPEG(const ioJPEG& other) : ioImageInterface() {
    copy(other);
  }

  // destructor
  ioJPEG::~ioJPEG() {
  }

  // returns the current parameters
  const ioJPEG::parameters& ioJPEG::getParameters() const {
    const parameters* params =
      dynamic_cast<const parameters*>(&ioImageInterface::getParameters());
    if (isNull(params)) {
      throw invalidParametersException(name());
    }
    return *params;
  }
  
  const std::string& ioJPEG::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  ioJPEG* ioJPEG::clone() const {
    return new ioJPEG(*this);
  }

  ioJPEG* ioJPEG::newInstance() const {
    return new ioJPEG();
  }

  // shortcut for apply (load file named "filename")
  bool ioJPEG::load(const std::string& filename,
                    image& theImage,
                    bool& isGray) {

    FILE* fp;

    // open the file
    if ((fp = fopen(filename.c_str(), "rb")) == NULL) {
      setStatusString("File could not be found: ");
      appendStatusString(filename.c_str());
      return false;
    }

    bool res = load(fp,theImage,isGray);

    fclose(fp);

    return res;
  }

  // shortcut for apply (load file named "filename")
  bool ioJPEG::load(const std::string& filename,image& theImage) {
    bool isGray;
    return load(filename,theImage,isGray);
  }

  bool ioJPEG::load(const std::string& filename,
                    matrix<ubyte>& theImage,
                    palette& colors) {
    bool isGray;
    image img;
    if (load(filename,img,isGray)) {
      channel8 chnl;
      chnl.castFrom(img);
      theImage.swap(chnl);
      colors.copy(getGrayPalette());
      return true;
    }
    return false;
  }

  bool ioJPEG::load(const std::string& filename,
                    matrix<ubyte>& chnl) {
    return ioImageInterface::load(filename,chnl);
  }
                 

  bool ioJPEG::load(const std::string& filename,
                    matrix<float>& chnl) {
    return ioImageInterface::load(filename,chnl);
  }

  bool ioJPEG::load(const std::string& filename,
                    matrix<int32>& chnl,
                    palette& pal) {
    return ioImageInterface::load(filename,chnl,pal);
  }
  
  bool ioJPEG::load(const std::string& filename,
                    matrix<int32>& chnl) {
    return ioImageInterface::load(filename,chnl);
  }
  
  // load the picture defined in parameters, determine whether a gray
  // scale image was loaded, while using libJPEG
  bool ioJPEG::load(FILE* fp, image& theImage, bool& isGray) {
    // clear image:
    theImage.clear();

    // contains JPEG decompression parameters and pointers to working place
    struct jpeg_decompress_struct compressInfo;
    // error handler
    struct my_error_mgr jpegErrorHandler;
    //output row buffer
    JSAMPARRAY outputBuffer;
    // physical row width in output buffer
    int rowStride, height, width; //, rowCounter;

    // set up the normal JPEG error routines
    compressInfo.err = jpeg_std_error(&jpegErrorHandler.pub);
    jpegErrorHandler.pub.error_exit = my_error_exit;

    if (setjmp(jpegErrorHandler.setjmp_buffer)) {
      jpeg_destroy_decompress(&compressInfo);
      setStatusString(jpegErrorHandler.error_msg);
      return false;
    }

    // Initialize the JPEG decompression object
    jpeg_create_decompress(&compressInfo);

    // specify data source
    jpeg_stdio_src(&compressInfo, fp);

    // read file parameters
    jpeg_read_header(&compressInfo, TRUE);

    // Start decompressor
    jpeg_start_decompress(&compressInfo);

    // JSAMPLEs per row in output buffer
    rowStride = compressInfo.output_width * compressInfo.output_components;
    // rowCounter = 0;
    height = compressInfo.output_height;
    width = compressInfo.output_width;
    theImage.allocate(height, width);

    // Make a one-row-high sample array that will go away when done with image
    outputBuffer = (*compressInfo.mem->alloc_sarray)
      ((j_common_ptr) &compressInfo,JPOOL_IMAGE, rowStride, 1);

    // Use the library's state variable compressInfo.output_scanline
    // as the loop counter
    image::iterator it = theImage.begin();
    JSAMPLE *ptr,*eptr;

    if (compressInfo.output_components == 1) {
      isGray = true;
      while (compressInfo.output_scanline < compressInfo.output_height) {
        jpeg_read_scanlines(&compressInfo, outputBuffer, 1);
        ptr = outputBuffer[0];
        eptr = ptr+compressInfo.output_width;
        ubyte val;
        // in case of grayscale image
        for (;ptr != eptr;++ptr,++it) {
          val = *ptr;
          (*it).set(val,val,val,0);
        }
      }
    } else if (compressInfo.output_components == 3) {
      isGray = false;
      while (compressInfo.output_scanline < compressInfo.output_height) {
        jpeg_read_scanlines(&compressInfo, outputBuffer, 1);
        ptr = outputBuffer[0];
        eptr = ptr+(3*compressInfo.output_width);
        // in case of color image
        for (;ptr != eptr;++it,ptr+=3) {
          (*it).set(ptr[0], ptr[1], ptr[2], 0);
        }
      }
    } else {
      setStatusString("Unknown image format!");
      return false;
    }

    // Finish decompression
    jpeg_finish_decompress(&compressInfo);

    // Release JPEG decompression object, close the file
    jpeg_destroy_decompress(&compressInfo);

    return true;
  }

  //scans all chunks before the first SoS-chunk(!) for image information
  bool ioJPEG::checkHeader(const std::string& filename, 
                           headerInformation& info) {

    info.type = "JPG";

    // contains JPEG decompression parameters and pointers to working place
    jpeg_decompress_struct compressInfo;

    // error handler
    my_error_mgr jpegErrorHandler;

    // file to open
    FILE *fp;

    // open the file
    if ((fp = fopen(filename.c_str(), "rb")) == NULL) {
      setStatusString("File could not be found: ");
      appendStatusString(filename.c_str());
      return 0;
    }

    // set up the normal JPEG error routines
    compressInfo.err = jpeg_std_error(&jpegErrorHandler.pub);
    jpegErrorHandler.pub.error_exit = my_error_exit;

    if (setjmp(jpegErrorHandler.setjmp_buffer)) {
      jpeg_destroy_decompress(&compressInfo);
      fclose(fp);
      setStatusString(jpegErrorHandler.error_msg);
      return false;
    }

    // Initialize the JPEG decompression object
    jpeg_create_decompress(&compressInfo);

    // specify data source
    jpeg_stdio_src(&compressInfo, fp);

    // read in the different comments marker
    jpeg_save_markers(&compressInfo, JPEG_COM, 0xFFFF);

    // read file parameters
    jpeg_read_header(&compressInfo, TRUE);

    // return the value to the parameters
    info.size.x = compressInfo.image_width;
    info.size.y = compressInfo.image_height;

    // gray valued image?
    if (compressInfo.out_color_space == JCS_GRAYSCALE) {
      info.colorEntries = 256;
      info.bpp = 8;
    } else {
      info.colorEntries = 0;
      info.bpp = 24;
    }
    
    // get the comment marker
    //if (notNull(compressInfo.marker_list)) {
    //  comment = (char*)compressInfo.marker_list->data;
    //} else {
    //  comment = "";
    //}

    // Release JPEG decompression object, close the file
    jpeg_destroy_decompress(&compressInfo);
    fclose(fp);

    return true;
  }

  // save JPEG images using libjpeg
  bool ioJPEG::save(const std::string& filename,
                    const image& theImage) {
    FILE *fp;

    // check the file
    if ((fp = fopen(filename.c_str(), "wb")) == NULL) {
      setStatusString("File cannot be opened. Is its path correct? ");
      return false;
    }
    bool result=save(fp,theImage);
    fclose(fp);
    
    return result;
  }
  
  // save JPEG images using libjpeg
  bool ioJPEG::save(FILE* fp, const image& theImage) {

    const parameters& param = getParameters();
    struct jpeg_compress_struct compressInfo;
    jpeg_error_mgr jerr;
    JSAMPARRAY inputBuffer;
    int rowStride, rowCounter;

    if (isNull(fp)) {
      setStatusString("Internal Error: file was null");
      return false;
    }

    // allocate and initialize JPEG compression object
    // set up the error handler at first
    compressInfo.err = jpeg_std_error(&jerr);
    jpeg_create_compress(&compressInfo);

    // specify data destination
    jpeg_stdio_dest(&compressInfo, fp);

    // set parameters for compression
    compressInfo.image_width = theImage.columns();
    compressInfo.image_height = theImage.rows();
    // # of color components per pixel
    compressInfo.input_components = 3;
    // colorspace of input image
    compressInfo.in_color_space = JCS_RGB;

    // set other parameters as defaults
    jpeg_set_defaults(&compressInfo);
    jpeg_set_quality(&compressInfo, param.quality, TRUE);

    // Start compressor
    jpeg_start_compress(&compressInfo, TRUE);

    //  write the image, allocate the memory needed
    rowStride = 3 * theImage.columns();
    rowCounter = 0;   // set the row index in the theI2mage matrix
    inputBuffer =
      (*compressInfo.mem->alloc_sarray)((j_common_ptr) &compressInfo,
                                        JPOOL_IMAGE, rowStride, 1);

    // copy theImage matrix to the inputBuffer one line every time,
    // and write into the file
    palette::const_iterator it,eit;
    JSAMPLE* ptr;
    while (compressInfo.next_scanline < compressInfo.image_height) {
      ptr = inputBuffer[0];
      it = theImage.getRow(rowCounter).begin();
      eit = theImage.getRow(rowCounter).end();
      for (;it != eit;++it) {
        (*ptr) = (*it).getRed();
        ++ptr;
        (*ptr) = (*it).getGreen();
        ++ptr;
        (*ptr) = (*it).getBlue();
        ++ptr;
      }
      rowCounter++;  // increment of the row index in theImage matrix
      jpeg_write_scanlines(&compressInfo, inputBuffer, 1);
    }

    // Finish compression, close the file
    jpeg_finish_compress(&compressInfo);
    // release the allocated memory
    jpeg_destroy_compress(&compressInfo);

    return true;
  }

  // save JPEG images using libjpeg
  bool ioJPEG::save(const std::string& filename,
                    const matrix<ubyte>& theImage) {

    const parameters& param = getParameters();
    struct jpeg_compress_struct compressInfo;
    jpeg_error_mgr jerr;
    JSAMPARRAY inputBuffer;
    int rowStride, rowCounter;
    FILE *fp;

    // check the file
    if ((fp = fopen(filename.c_str(), "wb")) == NULL) {
      setStatusString("File does not exist: ");
      appendStatusString(filename.c_str());
      return false;
    }

    // allocate and initialize JPEG compression object
    // set up the error handler at first
    compressInfo.err = jpeg_std_error(&jerr);
    jpeg_create_compress(&compressInfo);

    // specify data destination
    jpeg_stdio_dest(&compressInfo, fp);

    // set parameters for compression
    compressInfo.image_width = theImage.columns();
    compressInfo.image_height = theImage.rows();
    // # of color components per pixel
    compressInfo.input_components = 1;
    // colorspace of input image
    compressInfo.in_color_space = JCS_GRAYSCALE;

    // set other parameters as defaults
    jpeg_set_defaults(&compressInfo);
    jpeg_set_quality(&compressInfo, param.quality, TRUE);

    // Start compressor
    jpeg_start_compress(&compressInfo, TRUE);

    //  write the image, allocate the memory needed
    rowStride = theImage.columns();
    rowCounter = 0;   // set the row index in the theI2mage matrix
    inputBuffer =
      (*compressInfo.mem->alloc_sarray)((j_common_ptr) &compressInfo,
                                        JPOOL_IMAGE, rowStride, 1);

    // copy theImage matrix to the inputBuffer one line every time,
    // and write into the file
    vector<ubyte>::const_iterator it,eit;
    JSAMPLE* ptr;
    while (compressInfo.next_scanline < compressInfo.image_height) {
      ptr = inputBuffer[0];
      it = theImage.getRow(rowCounter).begin();
      eit = theImage.getRow(rowCounter).end();
      for (;it != eit;++it,++ptr) {
        (*ptr) = (JSAMPLE)(*it);
      }
      rowCounter++;  // increment of the row index in theImage matrix
      jpeg_write_scanlines(&compressInfo, inputBuffer, 1);
    }

    // Finish compression, close the file
    jpeg_finish_compress(&compressInfo);
    // release the allocated memory
    jpeg_destroy_compress(&compressInfo);

    fclose(fp);

    return true;
  }

  bool ioJPEG::save(const std::string& filename,
                    const matrix<ubyte>& theImage,
                    const palette& thePalette) {
    // check if the palette is a gray valued one
    if (thePalette == getGrayPalette()) { 
      return save(filename,theImage);
    } 

    usePalette palApply;
    image img;

    // check if thePalette is sufficient for theImage
    const ubyte max = theImage.findMaximum();
    if (max >= thePalette.size()) {
      setStatusString("Palette does not have enough entries");
      return false;
    }

    palApply.apply(theImage,thePalette,img);
    return save(filename,img);
  }

  bool ioJPEG::save(const std::string& filename,
                    const matrix<float>& theImage) {
    return ioImageInterface::save(filename,theImage);
  }

  bool ioJPEG::save(const std::string& filename,
                    const matrix<int32>& theImage) {
    return ioImageInterface::save(filename,theImage);
  }

  bool ioJPEG::save(const std::string& filename,
                    const matrix<int32>& theImage,
                    const palette& pal) {
    return ioImageInterface::save(filename,theImage,pal);
  }
}

#else

#ifndef HAVE_LOCALJPEG
#  ifdef _LTI_MSC_VER
#    pragma message("WARNING: No JPEG support found.")
#  else
#    warning "WARNING: No JPEG support found."
#  endif
#endif

#endif

