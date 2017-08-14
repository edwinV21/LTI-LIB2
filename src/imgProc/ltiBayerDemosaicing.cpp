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
 * \file   ltiBayerDemosaicing.cpp
 *         Contains the class bayerDemosaicing, base class for all
 *         Bayer demosaicing methods
 * \author Arnd Hannemann
 * \author Peter Doerfler
 * \date   10.12.2004
 *
 * revisions ..: $Id: ltiBayerDemosaicing.cpp,v 1.9 2012-09-16 04:58:33 alvarado Exp $
 */

#include "ltiBayerDemosaicing.h"

namespace lti {
  // --------------------------------------------------
  // bayerDemosaicing::parameters
  // --------------------------------------------------

  // default constructor
  bayerDemosaicing::parameters::parameters()
    : functor::parameters() {
    
    bayerPattern = RGGB;
  }

  // copy constructor
  bayerDemosaicing::parameters::parameters(const parameters& other)
    : functor::parameters() {
    copy(other);
  }

  // destructor
  bayerDemosaicing::parameters::~parameters() {
  }

  // copy member
  bayerDemosaicing::parameters&
  bayerDemosaicing::parameters::copy(const parameters& other) {
    functor::parameters::copy(other);

    
      bayerPattern = other.bayerPattern;

    return *this;
  }

  // alias for copy method
  bayerDemosaicing::parameters&
  bayerDemosaicing::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  const std::string& 
  bayerDemosaicing::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone method
  bayerDemosaicing::parameters* bayerDemosaicing::parameters::clone() const {
    return new parameters(*this);
  }

  // new instance
  bayerDemosaicing::parameters*
  bayerDemosaicing::parameters::newInstance() const {
    return new parameters();
  }

  /*
   * Write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool bayerDemosaicing::parameters::write(ioHandler& handler,
                                         const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {
      
      b = lti::write(handler,"bayerPattern",bayerPattern);
    }

    b = b && functor::parameters::write(handler,false);

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
  bool bayerDemosaicing::parameters::read(ioHandler& handler,
                                        const bool complete)  {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {
      
      b = lti::read(handler,"bayerPattern",bayerPattern);
    }

    b = b && functor::parameters::read(handler,false);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

  // --------------------------------------------------
  // bayerDemosaicing
  // --------------------------------------------------

  // default constructor
  bayerDemosaicing::bayerDemosaicing()
    : functor() {

    // create an instance of the parameters with the default values
    parameters defaultParameters;
    // set the default parameters
    setParameters(defaultParameters);
  }

  // default constructor
  bayerDemosaicing::bayerDemosaicing(const parameters& par)
    : functor() {

    // set the given parameters
    setParameters(par);
  }


  // copy constructor
  bayerDemosaicing::bayerDemosaicing(const bayerDemosaicing& other)
    : functor() {
    copy(other);
  }

  // destructor
  bayerDemosaicing::~bayerDemosaicing() {
  }

  // copy member
  bayerDemosaicing& 
  bayerDemosaicing::copy(const bayerDemosaicing& other) {
    functor::copy(other);

    return (*this);
  }

  // alias for copy member
  bayerDemosaicing&
  bayerDemosaicing::operator=(const bayerDemosaicing& other) {
    return (copy(other));
  }

  // return parameters
  const bayerDemosaicing::parameters&
  bayerDemosaicing::getParameters() const {
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

  bool bayerDemosaicing::apply(const matrix<ubyte>& src,
                               channel& dest) const {
    
    if (getParameters().bayerPattern==NoBayer) {
      // do castFrom manually
      dest.allocate(src.size());
      matrix<ubyte>::const_iterator sit=src.begin();
      channel::iterator dit=dest.begin();
      const channel::iterator dite=dest.end();
      for (; dit!=dite; ++dit, ++sit) {
        (*dit)=(*sit)/255.f;
      }
      return true;
    } else {
      image dummy;
      bool b=apply(src, dummy);
      dest.castFrom(dummy);
      return b;
    }
  }
  
  bool bayerDemosaicing::apply(const matrix<ubyte>& src,
                               channel8& dest) const {
    if (getParameters().bayerPattern==NoBayer) {
      dest.copy(src);
      return true;
    } else {
      image dummy;
      bool b=apply(src, dummy);
      dest.castFrom(dummy);
      return b;
    }
  }

  void bayerDemosaicing::generateBayerImage(const image& src,
                                            const eBayerPattern& bayerPattern,
                                            image& dest) {
    const int rows=src.rows();
    const int cols=src.columns();
    const rgbaPixel black(0,0,0,0);
    dest.assign(rows,cols,black);

    bool blueRow;
    bool blueLeft;

    switch (bayerPattern) {
      case RGGB:
        blueRow=false;
        blueLeft=false;
        break;
      case BGGR:
        blueRow=true;
        blueLeft=true;
        break;
      case GRBG:
        blueRow=false;
        blueLeft=true;
        break;
      case GBRG:
        blueRow=true;
        blueLeft=false;
        break;
      case NoBayer:
        dest.copy(src);
        return;        
        // default to avoid unintialized warnings
     default: 
        blueRow=false;
        blueLeft=false;
     
    }

    int i,j=0;
    for (i=0; i<rows; i++, j=0) {
        
      if (blueRow) {
        
        if (blueLeft) {
          if (j<cols) {
            dest.at(i,j).blue=src.at(i,j).blue;
          }
          j++;
        }

        for (; j<cols; j++) {
          dest.at(i,j).green=src.at(i,j).green;
          j++;
          if (j<cols) {
            dest.at(i,j).blue=src.at(i,j).blue;
          } else {
            break;
          }
        }
        
      } else {
        
        if (blueLeft) {
          if (j<cols) {
            dest.at(i,j).green=src.at(i,j).green;
          }
          j++;
        }

        for (; j<cols; j++) {
          dest.at(i,j).red=src.at(i,j).red;
          j++;
          if (j<cols) {
            dest.at(i,j).green=src.at(i,j).green;
          } else {
            break;
          }
        }
        
      }

      blueRow = blueRow ? false : true;
    }
   
  }

  void bayerDemosaicing::generateBayerImage(const matrix<ubyte>& src,
                                            const eBayerPattern& bayerPattern,
                                            image& dest) {
    const int rows=src.rows();
    const int cols=src.columns();
    const rgbaPixel black(0,0,0,0);
    dest.assign(rows,cols,black);

    bool blueRow;
    bool blueLeft;

    switch (bayerPattern) {
      case RGGB:
        blueRow=false;
        blueLeft=false;
        break;
      case BGGR:
        blueRow=true;
        blueLeft=true;
        break;
      case GRBG:
        blueRow=false;
        blueLeft=true;
        break;
      case GBRG:
        blueRow=true;
        blueLeft=false;
        break;
      case NoBayer: {
        channel8 dummy;
        dummy.copy(src);
        dest.castFrom(dummy);
        return;
      }
      default:
        // should never get here
        dest.clear();
        return;
    }

    int i,j=0;
    for (i=0; i<rows; i++, j=0) {
        
      if (blueRow) {
        
        if (blueLeft) {
          if (j<cols) {
            dest.at(i,j).blue=src.at(i,j);
          }
          j++;
        }

        for (; j<cols; j++) {
          dest.at(i,j).green=src.at(i,j);
          j++;
          if (j<cols) {
            dest.at(i,j).blue=src.at(i,j);
          } else {
            break;
          }
        }
        
      } else {
        
        if (blueLeft) {
          if (j<cols) {
            dest.at(i,j).green=src.at(i,j);
          }
          j++;
        }

        for (; j<cols; j++) {
          dest.at(i,j).red=src.at(i,j);
          j++;
          if (j<cols) {
            dest.at(i,j).green=src.at(i,j);
          } else {
            break;
          }
        }
        
      }

      blueRow = blueRow ? false : true;
    }
   
  }

  void bayerDemosaicing::filter(const image& src,
                                const eBayerPattern& bayerPattern,
                                channel8& dest) {
    
    
    const int rows=src.rows();
    const int cols=src.columns();
    dest.allocate(rows,cols);
    channel8::iterator dit;

    int i1,i2,j;
    bool startGreen1;
    bool startGreen2;
    // index 1 for red, index 2 for blue rows
    

    switch (bayerPattern) {
      case RGGB:
        i1=0; i2=1;
        startGreen1=false;
        startGreen2=true;
        break;
      case BGGR:
        i1=1; i2=0;
        startGreen1=true;
        startGreen2=false;
        break;
      case GRBG:
        i1=0; i2=1;
        startGreen1=true;
        startGreen2=false;
        break;
      case GBRG:
        i1=1; i2=0;
        startGreen1=false;
        startGreen2=true;
        break;
      default:
        throw invalidParametersException
          ("Unkown eBayerPattern in bayerDemosaicing::filter()");
        break;
    }

    //do red rows
    dit=dest.begin();
    j=0;
    if (i1==1) {
      dit+=cols;
    }
    for (; i1<rows; i1+=2, j=0, dit+=cols) {
      if (startGreen1) {
        (*dit)=src.at(i1,j).green;
        ++dit;
        j++;
      }
      for (; j<cols; j++, ++dit) {
        (*dit)=src.at(i1,j).red;
        ++dit;
        j++;
        if (j<cols) {
          (*dit)=src.at(i1,j).green;
        } else {
          break;
        }
      }
    }

    //do blue rows
    dit=dest.begin();
    j=0;
    if (i2==1) {
      dit+=cols;
    }
    for (; i2<rows; i2+=2, j=0, dit+=cols) {
      if (startGreen2) {
        (*dit)=src.at(i2,j).green;
        ++dit;
        j++;
      }
      for (; j<cols; j++, ++dit) {
        (*dit)=src.at(i2,j).blue;
        ++dit;
        j++;
        if (j<cols) {
          (*dit)=src.at(i2,j).green;
        } else {
          break;
        }
      }
    }
  }

  bool bayerDemosaicing::generateGreyImage(const matrix<ubyte>& src,
                                           image& dest) const {
    channel8 dummy;
    bool b=apply(src,dummy);
    dest.castFrom(dummy);
    return b;
  }


  // --------------------------------------------------------------------
  // global read/write functions for eBayerPattern
  // --------------------------------------------------------------------

  bool read(ioHandler& handler,bayerDemosaicing::eBayerPattern& data) {

    std::string str;
    if (handler.read(str)) {
      if (str == "NoBayer") {
        data = bayerDemosaicing::NoBayer;
      } else if (str == "RGGB") {
        data = bayerDemosaicing::RGGB;
      } else if (str == "BGGR") {
        data = bayerDemosaicing::BGGR;
      } else if (str == "GBRG") {
        data = bayerDemosaicing::GBRG;
      } else if (str == "GRBG") {
        data = bayerDemosaicing::GRBG;
      } else {
        // default
        data = bayerDemosaicing::RGGB;
      }
      return true;
    } else {
      return false;
    }    
  }

  bool write(ioHandler& handler,const bayerDemosaicing::eBayerPattern& data) {
    bool b=false;
    std::string str;
    switch(data) {
      case bayerDemosaicing::NoBayer:
        b=handler.write("NoBayer");
        break;
      case bayerDemosaicing::RGGB:
        b=handler.write("RGGB");
        break;
      case bayerDemosaicing::BGGR:
        b=handler.write("BGGR");
        break;
      case bayerDemosaicing::GRBG:
        b=handler.write("GRBG");
        break;
      case bayerDemosaicing::GBRG:
        b=handler.write("GBRG");
        break;
      default:
        handler.write("NoBayer");
        b=false;
        handler.setStatusString("Unknown eBayerPattern in lti::write\n");
        break;
    }
    return b;
  }


}


