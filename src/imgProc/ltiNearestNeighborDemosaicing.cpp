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
 * \file   ltiNearestNeighborDemosaicing.cpp
 *         Contains the class nearestNeighborDemosaicing, which is
 *         the most simple bayerDemosaicing.
 * \author Arnd Hannemann
 * \author Peter Doerfler
 * \date   11.12.2004
 *
 * revisions ..: $Id: ltiNearestNeighborDemosaicing.cpp,v 1.10 2012/09/16 04:58:33 alvarado Exp $
 */

#include "ltiNearestNeighborDemosaicing.h"

#undef _LTI_DEBUG
//#define _LTI_DEBUG 1
#include "ltiDebug.h"
#include "ltiFactory.h"

namespace lti {
  //register in bayerDemosaicing factory
  _LTI_REGISTER_IN_FACTORY(bayerDemosaicing,nearestNeighborDemosaicing)

  // --------------------------------------------------
  // nearestNeighborDemosaicing::parameters
  // --------------------------------------------------

  // default constructor
  nearestNeighborDemosaicing::parameters::parameters()
    : bayerDemosaicing::parameters() {
    
  }

  // copy constructor
  nearestNeighborDemosaicing::parameters::parameters(const parameters& other)
    : bayerDemosaicing::parameters() {
    copy(other);
  }

  // destructor
  nearestNeighborDemosaicing::parameters::~parameters() {
  }

  // copy member
  nearestNeighborDemosaicing::parameters&
  nearestNeighborDemosaicing::parameters::copy(const parameters& other) {
    bayerDemosaicing::parameters::copy(other);

    return *this;
  }

  // alias for copy method
  nearestNeighborDemosaicing::parameters&
  nearestNeighborDemosaicing::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  // class name
  const std::string& nearestNeighborDemosaicing::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone method
  nearestNeighborDemosaicing::parameters*
  nearestNeighborDemosaicing::parameters::clone() const {
    return new parameters(*this);
  }

  // new instance
  nearestNeighborDemosaicing::parameters*
  nearestNeighborDemosaicing::parameters::newInstance() const {
    return new parameters();
  }

  /*
   * Write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool nearestNeighborDemosaicing::parameters::write(ioHandler& handler,
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
  bool nearestNeighborDemosaicing::parameters::read(ioHandler& handler,
						    const bool complete) {
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
  // nearestNeighborDemosaicing
  // --------------------------------------------------

  // default constructor
  nearestNeighborDemosaicing::nearestNeighborDemosaicing()
    : bayerDemosaicing() {

    // create an instance of the parameters with the default values
    parameters defaultParameters;
    // set the default parameters
    setParameters(defaultParameters);
  }

  // default constructor
  nearestNeighborDemosaicing::nearestNeighborDemosaicing(const parameters& par)
    : bayerDemosaicing() {

    // set the given parameters
    setParameters(par);
  }


  // copy constructor
  nearestNeighborDemosaicing::
  nearestNeighborDemosaicing(const nearestNeighborDemosaicing& other)
    : bayerDemosaicing() {
    copy(other);
  }

  // destructor
  nearestNeighborDemosaicing::~nearestNeighborDemosaicing() {
  }

  // copy member
  nearestNeighborDemosaicing& 
  nearestNeighborDemosaicing::copy(const nearestNeighborDemosaicing& other) {
    bayerDemosaicing::copy(other);

    return (*this);
  }

  // alias for copy member
  nearestNeighborDemosaicing&
  nearestNeighborDemosaicing::operator=(const nearestNeighborDemosaicing& other) {
    return (copy(other));
  }

  // class name
  const std::string& nearestNeighborDemosaicing::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone member
  nearestNeighborDemosaicing* nearestNeighborDemosaicing::clone() const {
    return new nearestNeighborDemosaicing(*this);
  }

  // clone member
  nearestNeighborDemosaicing* nearestNeighborDemosaicing::newInstance() const {
    return new nearestNeighborDemosaicing();
  }

  // return parameters
  const nearestNeighborDemosaicing::parameters&
  nearestNeighborDemosaicing::getParameters() const {
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
  
  // apply for type matrix<ubyte>, image!
  bool nearestNeighborDemosaicing::apply(const matrix<ubyte>& src,
                                         image& dest) const {

    if (src.empty()) {
      dest.clear(); // nothing to do
      return true;
    }

    const parameters& par = getParameters();
    const eBayerPattern pat = par.bayerPattern;
    
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

    const ubyte* ptr = &src.at(0,0);

    const int rowsm= rows-1;
    const int colsm= cols-1;
    const int colsp= cols+1;

    vector<rgbaPixel>::iterator dit  = dest.getRow(0).begin();
    vector<rgbaPixel>::iterator edit = dit + colsm;


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
        if (green) { // green pixel
          (*dit).set(ptr[cols],ptr[0],ptr[1]);
        } else { // blue
          // even pixel
          (*dit).set(ptr[colsp],
                     static_cast<ubyte>((ptr[1]+ptr[cols])/2),
                     ptr[0]);
        }
      }

      green = ! green;
      
      if ( green ) {
        // the last pixel is green
        (*dit).set(ptr[cols],ptr[0],ptr[-1]);       
      } else { // blue
        (*dit).set(ptr[colsm],ptr[cols],ptr[0]);
      }
    
    } else { // if first line red
      
      if ( green ) { // first pixel green
        (*dit).set(ptr[1],ptr[0],ptr[cols]);
      } else { // first pixel red
        (*dit).set(ptr[0],
                   static_cast<ubyte>((ptr[1]+ptr[cols])/2),
                   ptr[colsp]);
      }
    
      // the top line, except the last point
      for (++dit,++ptr;dit<edit;++dit,++ptr) {
        green = ! green;
        if (green) { // green pixel
          (*dit).set(ptr[1],ptr[0],ptr[cols]);
        } else { // red pixel
          (*dit).set(ptr[0],
                     static_cast<ubyte>((ptr[1]+ptr[cols])/2),
                     ptr[colsp]);
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
        (*dit).set(ptr[colsm],
                   static_cast<ubyte>((ptr[-1]+ptr[cols])/2),
                   ptr[0]);
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
          (*dit).set(ptr[cols],ptr[0],ptr[1]);
        } else { // blue pixel
          (*dit).set(ptr[colsp],
                     static_cast<ubyte>((ptr[1]+ptr[cols])/2),
                     ptr[0]);
        }

        for (++dit,++ptr;dit<edit;++dit,++ptr) {
          green = ! green;
          if (green) {
            (*dit).set(ptr[cols],ptr[0],ptr[1]);
          } else { // blue
            (*dit).set(ptr[colsp],
                       static_cast<ubyte>((ptr[1]+ptr[cols])/2),
                       ptr[0]);
          }
        } // for dit

        green = ! green;

        // last pixel
        if ( green ) { // last is green
          (*dit).set(ptr[cols],ptr[0],ptr[-1]);
        } else { // last is blue
          (*dit).set(ptr[colsm],ptr[cols],ptr[0]);
        }

      } else { // red row
        // the first line pixel
        if (green) {
          (*dit).set(ptr[1],ptr[0],ptr[cols]);
        } else { // red
          (*dit).set(ptr[0],
                     static_cast<ubyte>((ptr[1]+ptr[cols])/2),
                     ptr[colsp]);
        }
        
        for (++dit,++ptr;dit<edit;++dit,++ptr) {
          green = ! green;

          if (green) {
            (*dit).set(ptr[1],ptr[0],ptr[cols]);
          } else { // we're on red
            (*dit).set(ptr[0],
                       static_cast<ubyte>((ptr[1]+ptr[cols])/2),
                       ptr[colsp]);
          }

        } // for dit

        green = ! green;

        // last pixel of current red line
        if ( green ) { // last is green
          (*dit).set(ptr[-1],ptr[0],ptr[cols]);
        } else { // last is red
          (*dit).set(ptr[0],ptr[cols],ptr[colsm]);
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
        (*dit).set(ptr[-cols],ptr[0],ptr[1]);
      } else { // blue
        (*dit).set(ptr[-colsm],ptr[1],ptr[0]);
      }

      for (++dit,++ptr;dit<edit;++dit,++ptr) {
        green = ! green;
        if ( green) {
          (*dit).set(ptr[-cols],ptr[0],ptr[1]);
        } else { // blue
          (*dit).set(ptr[-colsm],ptr[1],ptr[0]);
        }
      } // for dit

      green = ! green;
      
      // last pixel
      if ( green ) { // last is green
        (*dit).set(ptr[-cols],ptr[0],ptr[-1]);
      } else { // last is blue
        (*dit).set(ptr[-colsp],
                   static_cast<ubyte>((ptr[-cols]+ptr[-1])/2),
                   ptr[0]);
      }

    } else { // last line is red

      // the first line pixel
      if ( green ) {
        (*dit).set(ptr[1],ptr[0],ptr[-cols]);
      } else { // red
        (*dit).set(ptr[0],ptr[1],ptr[-colsm]);
      }

      for (++dit,++ptr;dit<edit;++dit,++ptr) {
        green = ! green;
        if ( green ) {
          (*dit).set(ptr[1],ptr[0],ptr[-cols]);
        } else { // on red
          (*dit).set(ptr[0],ptr[1],ptr[-colsm]);
        }
      } // for dit

      green = ! green;

      // last pixel
      if ( green ) { // last is green
        (*dit).set(ptr[-1],ptr[0],ptr[-cols]);
      } else { // last is red
        (*dit).set(ptr[0],
                   static_cast<ubyte>((ptr[-cols]+ptr[-1])/2),
                   ptr[-colsp]);
      }
    } // else last line red
    
    return true;

  }


}

#include "ltiUndebug.h"
