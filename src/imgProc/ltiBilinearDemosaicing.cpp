/*
 * Copyright (C) 1998-2004
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


/* 
 * \file   ltiBilinearDemosaicing.cpp
 *         Contains the class bilinearDemosaicing, implementation of
 *         bilinear interpolation for Bayer demosaicing.
 * \author Peter Doerfler
 * \date   12.12.2004
 *
 * revisions ..: $Id: ltiBilinearDemosaicing.cpp,v 1.9 2008/10/27 01:59:09 alvarado Exp $
 */

#include "ltiBilinearDemosaicing.h"
#include "ltiFactory.h"


namespace lti {

  //register in bayerDemosaicing factory
  _LTI_REGISTER_IN_FACTORY(bayerDemosaicing,bilinearDemosaicing)
 
  // --------------------------------------------------
  // bilinearDemosaicing::parameters
  // --------------------------------------------------

  // default constructor
  bilinearDemosaicing::parameters::parameters()
    : bayerDemosaicing::parameters() {
  }

  // copy constructor
  bilinearDemosaicing::parameters::parameters(const parameters& other)
    : bayerDemosaicing::parameters() {
    copy(other);
  }

  // destructor
  bilinearDemosaicing::parameters::~parameters() {
  }

  // copy member
  bilinearDemosaicing::parameters&
  bilinearDemosaicing::parameters::copy(const parameters& other) {
    bayerDemosaicing::parameters::copy(other);
   
    return *this;
  }

  // alias for copy method
  bilinearDemosaicing::parameters&
  bilinearDemosaicing::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  // class name
  const std::string& bilinearDemosaicing::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone method
  bilinearDemosaicing::parameters* 
  bilinearDemosaicing::parameters::clone() const {
    return new parameters(*this);
  }

  // new instance
  bilinearDemosaicing::parameters* 
  bilinearDemosaicing::parameters::newInstance() const {
    return new parameters();
  }

  /*
   * Write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool bilinearDemosaicing::parameters::write(ioHandler& handler,
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
  bool bilinearDemosaicing::parameters::read(ioHandler& handler,
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
  // bilinearDemosaicing
  // --------------------------------------------------

  // default constructor
  bilinearDemosaicing::bilinearDemosaicing()
    : bayerDemosaicing() {

    // create an instance of the parameters with the default values
    parameters defaultParameters;
    // set the default parameters
    setParameters(defaultParameters);
  }

  // default constructor
  bilinearDemosaicing::bilinearDemosaicing(const parameters& par)
    : bayerDemosaicing() {

    // set the given parameters
    setParameters(par);
  }


  // copy constructor
  bilinearDemosaicing::bilinearDemosaicing(const bilinearDemosaicing& other)
    : bayerDemosaicing() {
    copy(other);
  }

  // destructor
  bilinearDemosaicing::~bilinearDemosaicing() {
  }

  // copy member
  bilinearDemosaicing& 
  bilinearDemosaicing::copy(const bilinearDemosaicing& other) {
    bayerDemosaicing::copy(other);

    return (*this);
  }

  // alias for copy member
  bilinearDemosaicing&
  bilinearDemosaicing::operator=(const bilinearDemosaicing& other) {
    return (copy(other));
  }

  // class name
  const std::string& bilinearDemosaicing::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone member
  bilinearDemosaicing* bilinearDemosaicing::clone() const {
    return new bilinearDemosaicing(*this);
  }

  // clone member
  bilinearDemosaicing* bilinearDemosaicing::newInstance() const {
    return new bilinearDemosaicing();
  }

  // return parameters
  const bilinearDemosaicing::parameters&
  bilinearDemosaicing::getParameters() const {
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
  bool bilinearDemosaicing::apply(const matrix<ubyte>& src,image& dest) const {

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
  bool bilinearDemosaicing::pattern(const matrix<ubyte>& src,
                                    image& dest) const {

    const ubyte* ptr = &src.at(0,0);

    const int cols = src.columns();
    const int rows = src.rows();
    const int rowsm= rows-1;
    const int colsm= cols-1;
    const int colsp= cols+1;

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
        (*dit).set(ptr[cols],
                   ptr[0],
                   ptr[1]);
      } else { // pixel is blue
        // The very first pixel (top-left corner)
        (*dit).set(ptr[colsp],
                   static_cast<ubyte>((ptr[1]+ptr[cols])/2),
                   ptr[0]);
      }
    
      // the top line, except the last point
      for (++dit,++ptr;dit<edit;++dit,++ptr) {
        green = ! green;
        if (green) {
          // odd pixel
          (*dit).set(ptr[cols],
                     ptr[0],
                     static_cast<ubyte>((ptr[-1]+ptr[1])/2));
        } else {
          // even pixel
          (*dit).set(static_cast<ubyte>((ptr[colsm]+ptr[colsp])/2),
                     static_cast<ubyte>((ptr[-1]+ptr[1]+ptr[cols])/3),
                     ptr[0]);
        }
      }

      green = ! green;
      
      if ( green ) {
        // the last pixel is odd
        // odd pixel
        (*dit).set(ptr[cols],
                   ptr[0],
                   ptr[-1]);       
      } else { // blue
        // the last pixel is even
        (*dit).set(ptr[colsm],
                   static_cast<ubyte>((ptr[-1]+ptr[cols])/2),
                   ptr[0]);
      }
    
    } else { // if first line red
      
      if ( green ) { // first pixel green
        // The very first pixel (top-left corner)
        (*dit).set(ptr[1],
                   ptr[0],
                   ptr[cols]);
      } else { // first pixel red
        // The very first pixel (top-left corner)
        (*dit).set(ptr[0],
                   static_cast<ubyte>((ptr[1]+ptr[cols])/2),
                   ptr[colsp]);
      }
    
      // the top line, except the last point
      for (++dit,++ptr;dit<edit;++dit,++ptr) {
        green = ! green;
        if (green) { // green pixel
          (*dit).set(static_cast<ubyte>((ptr[-1]+ptr[1])/2),
                     ptr[0],
                     ptr[cols]);
        } else { // red pixel
          (*dit).set(ptr[0],
                     static_cast<ubyte>((ptr[-1]+ptr[1]+ptr[cols])/3),
                     static_cast<ubyte>((ptr[colsm]+ptr[colsp])/2));
        }
      }

      green = ! green;
      
      if ( green ) {
        // the last pixel is green
        // odd pixel
        (*dit).set(ptr[cols],
                   ptr[0],
                   ptr[-1]);       
      } else { // red 
        // the last pixel is red
        (*dit).set(ptr[0],
                   static_cast<ubyte>((ptr[-1]+ptr[cols])/2),
                   ptr[colsm]);
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
          (*dit).set(static_cast<ubyte>((ptr[-cols]+ptr[cols])/2),
                     ptr[0],
                     ptr[1]);
        } else { // blue pixel
          (*dit).set(static_cast<ubyte>((ptr[-colsm]+ptr[colsp])/2),
                     static_cast<ubyte>((ptr[-cols]+ptr[1]+ptr[cols])/3),
                     ptr[0]);
        }

        for (++dit,++ptr;dit<edit;++dit,++ptr) {
          green = ! green;
          if (green) {
            (*dit).set(static_cast<ubyte>((ptr[-cols]+ptr[cols])/2),
                       ptr[0],
                       static_cast<ubyte>((ptr[-1]+ptr[1])/2));
          } else {
            (*dit).set(static_cast<ubyte>((ptr[-colsp]+ptr[-colsm]+
                                           ptr[colsm]+ptr[colsp])/4),
                       static_cast<ubyte>((ptr[-cols]+ptr[-1]+
                                           ptr[1]+ptr[cols])/4),
                       ptr[0]);
          }

        } // for dit

        green = ! green;

        // last pixel
        if ( green ) { // last is green
          (*dit).set(static_cast<ubyte>((ptr[-cols]+ptr[cols])/2),
                     ptr[0],
                     ptr[-1]);
          
        } else { // last is blue
          (*dit).set(static_cast<ubyte>((ptr[-colsp]+ptr[colsm])/2),
                     static_cast<ubyte>((ptr[-cols]+ptr[-1]+ptr[cols])/3),
                     ptr[0]);
          
        }

      } else { // red row
        // the first line pixel
        if (green) {
          (*dit).set(ptr[1],
                     ptr[0],
                     static_cast<ubyte>((ptr[-cols]+ptr[cols])/2));
        } else { // red
          (*dit).set(ptr[0],
                     static_cast<ubyte>((ptr[-cols]+ptr[1]+ptr[cols])/3),
                     static_cast<ubyte>((ptr[-colsm]+ptr[colsp])/2));
        }
        
        for (++dit,++ptr;dit<edit;++dit,++ptr) {
          green = ! green;

          if (green) {
            (*dit).set(static_cast<ubyte>((ptr[-1]+ptr[1])/2),
                       ptr[0],
                       static_cast<ubyte>((ptr[-cols]+ptr[cols])/2));
          } else { // we're on red
            (*dit).set(ptr[0],
                       static_cast<ubyte>((ptr[-cols]+ptr[-1]+
                                           ptr[1]+ptr[cols])/4),
                       static_cast<ubyte>((ptr[-colsp]+ptr[-colsm]+
                                           ptr[colsm]+ptr[colsp])/4));
          }

        } // for dit

        green = ! green;

        // last pixel of current red line
        if ( green ) { // last is green
          (*dit).set(ptr[-1],
                     ptr[0],
                     static_cast<ubyte>((ptr[-cols]+ptr[cols])/2));
        } else { // last is red
          (*dit).set(ptr[0],
                     static_cast<ubyte>((ptr[-cols]+ptr[-1]+ptr[cols])/3),
                     static_cast<ubyte>((ptr[-colsp]+ptr[colsm])/2));
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
        (*dit).set(ptr[-cols],
                   ptr[0],
                   ptr[1]);
      } else { // blue
        (*dit).set(ptr[-colsm],
                   static_cast<ubyte>((ptr[-cols]+ptr[1])/2),
                   ptr[0]);
      }

      for (++dit,++ptr;dit<edit;++dit,++ptr) {
        green = ! green;
        // odd pixel
        if ( green) {
          (*dit).set(ptr[-cols],
                     ptr[0],
                     static_cast<ubyte>((ptr[-1]+ptr[1])/2));
        } else {
          (*dit).set(static_cast<ubyte>((ptr[-colsp]+ptr[-colsm])/2),
                     static_cast<ubyte>((ptr[-cols]+ptr[-1]+ptr[1])/3),
                     ptr[0]);
        }
      } // for dit

      green = ! green;
      
      // last pixel
      if ( green ) { // last is green
        (*dit).set(ptr[-cols],
                   ptr[0],
                   ptr[-1]);
        
      } else { // last is blue
        (*dit).set(ptr[-colsp],
                   static_cast<ubyte>((ptr[-cols]+ptr[-1])/2),
                   ptr[0]);
      }

    } else { // last line is red

      // the first line pixel
      if ( green ) {
        (*dit).set(ptr[1],
                   ptr[0],
                   ptr[-cols]);
      } else { // red
        (*dit).set(ptr[0],
                   static_cast<ubyte>((ptr[-cols]+ptr[1])/2),
                   ptr[-colsm]);
      }

      for (++dit,++ptr;dit<edit;++dit,++ptr) {
        green = ! green;
        if ( green ) {
          (*dit).set(static_cast<ubyte>((ptr[-1]+ptr[1])/2),
                     ptr[0],
                     ptr[-cols]);
        } else { // on red
          (*dit).set(ptr[0],
                     static_cast<ubyte>((ptr[-cols]+ptr[-1]+ptr[1])/3),
                     static_cast<ubyte>((ptr[-colsp]+ptr[-colsm])/2));
        }
      } // for dit

      green = ! green;

      // last pixel
      if ( green ) { // last is green
        (*dit).set(ptr[-1],
                   ptr[0],
                   ptr[-cols]);
      } else { // last is red
        (*dit).set(ptr[0],
                   static_cast<ubyte>((ptr[-cols]+ptr[-1])/2),
                   ptr[-colsp]);
      }
    } // else last line red
    
    return true;

  }

}

