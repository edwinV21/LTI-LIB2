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
 * \file   ltiNoDemosaicing.h
 *         Contains the class lti::noDemosaicing, which does
 *         simple no interpolation of real colors for a Bayer
 *         pattern.
 * \author Pablo Alvarado
 * \date   26.10.2008
 *
 * revisions ..: $Id: ltiNoDemosaicing.cpp,v 1.2 2008/10/27 01:59:09 alvarado Exp $
 */

#include "ltiNoDemosaicing.h"
#include "ltiFactory.h"


namespace lti {

  //register in bayerDemosaicing factory
  _LTI_REGISTER_IN_FACTORY(bayerDemosaicing,noDemosaicing)
 
  // --------------------------------------------------
  // noDemosaicing::parameters
  // --------------------------------------------------

  // default constructor
  noDemosaicing::parameters::parameters()
    : bayerDemosaicing::parameters() {
  }

  // copy constructor
  noDemosaicing::parameters::parameters(const parameters& other)
    : bayerDemosaicing::parameters() {
    copy(other);
  }

  // destructor
  noDemosaicing::parameters::~parameters() {
  }

  // copy member
  noDemosaicing::parameters&
  noDemosaicing::parameters::copy(const parameters& other) {
    bayerDemosaicing::parameters::copy(other);
   
    return *this;
  }

  // alias for copy method
  noDemosaicing::parameters&
  noDemosaicing::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  // class name
  const std::string& noDemosaicing::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone method
  noDemosaicing::parameters* 
  noDemosaicing::parameters::clone() const {
    return new parameters(*this);
  }

  // new instance
  noDemosaicing::parameters* 
  noDemosaicing::parameters::newInstance() const {
    return new parameters();
  }

  /*
   * Write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool noDemosaicing::parameters::write(ioHandler& handler,
                                         const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {
      
    }

    b = b && bayerDemosaicing::parameters::write(handler,false);

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
  bool noDemosaicing::parameters::read(ioHandler& handler,
                                        const bool complete)  {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {
      
    }

    b = b && bayerDemosaicing::parameters::read(handler,false);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

  // --------------------------------------------------
  // noDemosaicing
  // --------------------------------------------------

  // default constructor
  noDemosaicing::noDemosaicing()
    : bayerDemosaicing() {

    // create an instance of the parameters with the default values
    parameters defaultParameters;
    // set the default parameters
    setParameters(defaultParameters);
  }

  // default constructor
  noDemosaicing::noDemosaicing(const parameters& par)
    : bayerDemosaicing() {

    // set the given parameters
    setParameters(par);
  }


  // copy constructor
  noDemosaicing::noDemosaicing(const noDemosaicing& other)
    : bayerDemosaicing() {
    copy(other);
  }

  // destructor
  noDemosaicing::~noDemosaicing() {
  }

  // copy member
  noDemosaicing& 
  noDemosaicing::copy(const noDemosaicing& other) {
    bayerDemosaicing::copy(other);

    return (*this);
  }

  // alias for copy member
  noDemosaicing&
  noDemosaicing::operator=(const noDemosaicing& other) {
    return (copy(other));
  }

  // class name
  const std::string& noDemosaicing::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone member
  noDemosaicing* noDemosaicing::clone() const {
    return new noDemosaicing(*this);
  }

  // clone member
  noDemosaicing* noDemosaicing::newInstance() const {
    return new noDemosaicing();
  }

  // return parameters
  const noDemosaicing::parameters&
  noDemosaicing::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }

  // -------------------------------------------------------------------
  // The apply-methods!
  // -------------------------------------------------------------------

  
  // On copy apply
  bool noDemosaicing::apply(const matrix<ubyte>& src,image& dest) const {

    if (src.empty()) {
      dest.clear(); // nothing to do
      return true;
    }

    const eBayerPattern pat = getParameters().bayerPattern;

    if (pat==NoBayer) {
      return generateGreyImage(src, dest);
    }

    const int cols = src.columns();
    const int rows = src.rows();

    if ( (cols < 2) ||
         (rows < 2) ) {
      setStatusString("No bayer pattern is complete for this image size");
      return false;
    }

    // reserve some memory
    dest.allocate(src.size());
    return pattern(src,dest);
  }

  // On copy apply
  bool noDemosaicing::pattern(const matrix<ubyte>& src,
                                    image& dest) const {

    const ubyte* ptr = &src.at(0,0);

    const int cols = src.columns();
    const int rows = src.rows();
    const int rowsm= rows-1;
    const int colsm= cols-1;

    vector<rgbaPixel>::iterator dit  = dest.getRow(0).begin();
    vector<rgbaPixel>::iterator edit = dit + colsm;

    const parameters& par = getParameters();

    bool green = false;
    bool blueRow = false;
    bool blueLeft = false;

    switch (par.bayerPattern) {
      case BGGR:
        green = false;
        blueRow = true;
        blueLeft = true;
        break;
      case RGGB:
        green = false;
        blueRow = false;
        blueLeft = false;
        break;
      case GRBG:
        green = true;
        blueRow = false;
        blueLeft = true;
        break;
      case GBRG:
        green = true;
        blueRow = true;
        blueLeft = false;
        break;
      default:
        return false;
    }

    if (blueRow) { // first line blue
      if ( green ) {
        // The very first pixel (top-left corner)
        (*dit).set(0,ptr[0],0);
      } else { // pixel is blue
        // The very first pixel (top-left corner)
        (*dit).set(0,0,ptr[0]);
      }
    
      // the top line, except the last point
      for (++dit,++ptr;dit<edit;++dit,++ptr) {
        green = ! green;
        if (green) {
          // green pixel
          (*dit).set(0,ptr[0],0);
        } else {
          // blue pixel
          (*dit).set(0,0,ptr[0]);
        }
      }

      green = ! green;
      
      if ( green ) {
        // the last pixel is green
        (*dit).set(0,ptr[0],0);       
      } else { // blue
        (*dit).set(0,0,ptr[0]);
      }
    
    } else { // if first line red
      
      if ( green ) { // first pixel green
        // The very first pixel (top-left corner)
        (*dit).set(0,ptr[0],0);
      } else { // first pixel red
        // The very first pixel (top-left corner)
        (*dit).set(ptr[0],0,0);
      }
    
      // the top line, except the last point
      for (++dit,++ptr;dit<edit;++dit,++ptr) {
        green = ! green;
        if (green) { // green pixel
          (*dit).set(0,ptr[0],0);
        } else { // red pixel
          (*dit).set(ptr[0],0,0);
        }
      }

      green = ! green;
      
      if ( green ) {
        // the last pixel is green
        // odd pixel
        (*dit).set(0,ptr[0],0);       
      } else { // red 
        // the last pixel is red
        (*dit).set(ptr[0],0,0);
      }

    } // end first line

    blueRow = ! blueRow;

    // do the inner lines
    for (int i=1; i<rowsm; ++i) {

      dit  = dest.getRow(i).begin();
      edit = dit+colsm;
      ptr = &src.at(i,0);

      green = blueLeft ? ! blueRow : blueRow;

      if (blueRow) { // blue row
        // the first line pixel
        if ( green ) { // green pixel
          (*dit).set(0,ptr[0],0);
        } else { // blue pixel
          (*dit).set(0,0,ptr[0]);
        }

        for (++dit,++ptr;dit<edit;++dit,++ptr) {
          green = ! green;
          if (green) {
            (*dit).set(0,ptr[0],0);
          } else { // blue
            (*dit).set(0,0,ptr[0]);
          }

        } // for dit

        green = ! green;

        // last pixel
        if ( green ) { // last is green
          (*dit).set(0,ptr[0],0);
          
        } else { // last is blue
          (*dit).set(0,0,ptr[0]);
          
        }

      } else { // red row
        // the first line pixel
        if (green) {
          (*dit).set(0,ptr[0],0);
        } else { // red
          (*dit).set(ptr[0],0,0);
        }
        
        for (++dit,++ptr;dit<edit;++dit,++ptr) {
          green = ! green;

          if (green) {
            (*dit).set(0,ptr[0],0);
          } else { // we're on red
            (*dit).set(ptr[0],0,0);
          }

        } // for dit

        green = ! green;

        // last pixel of current red line
        if ( green ) { // last is green
          (*dit).set(0,ptr[0],0);
        } else { // last is red
          (*dit).set(ptr[0],0,0);
        }
      } // else red row

      blueRow = ! blueRow;
    } // for all inner lines

    green = blueLeft ? ! blueRow : blueRow;
    dit = dest.getRow(dest.lastRow()).begin();
    edit = dit + colsm;
    ptr = &src.at(dest.lastRow(),0);

    // the last line
    if ( blueRow ) { // last line is blue

      // the first pixel of last line
      if ( green ) {
        (*dit).set(0,ptr[0],0);
      } else { // blue
        (*dit).set(0,0,ptr[0]);
      }

      for (++dit,++ptr;dit<edit;++dit,++ptr) {
        green = ! green;
        // odd pixel
        if ( green) {
          (*dit).set(0,ptr[0],0);
        } else {
          (*dit).set(0,0,ptr[0]);
        }
      } // for dit

      green = ! green;
      
      // last pixel
      if ( green ) { // last is green
        (*dit).set(0,ptr[0],0);
        
      } else { // last is blue
        (*dit).set(0,0,ptr[0]);
      }

    } else { // last line is red

      // the first line pixel
      if ( green ) {
        (*dit).set(0,ptr[0],0);
      } else { // red
        (*dit).set(ptr[0],0,0);
      }

      for (++dit,++ptr;dit<edit;++dit,++ptr) {
        green = ! green;
        if ( green ) {
          (*dit).set(0,ptr[0],0);
        } else { // on red
          (*dit).set(ptr[0],0,0);
        }
      } // for dit

      green = ! green;

      // last pixel
      if ( green ) { // last is green
        (*dit).set(0,ptr[0],0);
      } else { // last is red
        (*dit).set(ptr[0],0,0);
      }
    } // else last line red
    
    return true;

  }

}

