/*
 * Copyright (C) 1998-2005
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
 * \file   ltiModifier.cpp
 *         Contains the base class modifier.
 * \author Pablo Alvarado
 * \date   19.11.99
 *
 * revisions ..: $Id: ltiModifier.cpp,v 1.13 2009-04-19 04:06:10 alvarado Exp $
 */


#include "ltiModifier.h"
//#include "ltiSplitImageToRGB.h"
//#include "ltiMergeRGBToImage.h"

namespace lti {
  // --------------------------------------------------
  // modifier::parameters
  // --------------------------------------------------

  // default constructor
  modifier::parameters::parameters() : functor::parameters() {
    // default 
    boundaryType = Zero;
  }

  // copy constructor
  modifier::parameters::parameters(const parameters& other) : 
    functor::parameters() {
    copy(other);
  }

  // clone member
  modifier::parameters* modifier::parameters::clone() const {
    return new parameters(*this);
  }

  // newInstance member
  modifier::parameters* modifier::parameters::newInstance() const {
    return new parameters();
  }

  // return class name
  const std::string& modifier::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  

  // modifier parameters
  modifier::parameters& modifier::parameters::copy(const parameters& other) {
    functor::parameters::copy(other);
    boundaryType = other.boundaryType;

    return (*this);
  }

  /*
   * write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool modifier::parameters::write(ioHandler& handler,
                                   const bool complete) const
  {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    b = b && lti::write(handler, "boundaryType", boundaryType);

    b = b && functor::parameters::write(handler,false);

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
  bool modifier::parameters::read(ioHandler& handler,
                                  const bool complete)
  {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    b = b && lti::read(handler, "boundaryType", boundaryType);

    b = b && functor::parameters::read(handler,false);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

  // returns the current parameters
  const modifier::parameters& modifier::getParameters() const  {
    const parameters* params =
      dynamic_cast<const parameters*>(&functor::getParameters());

    if(params==0) {
      throw invalidParametersException(name());
    }

    return *params;
  }

  // returns the current parameters (protected non const member!)
  modifier::parameters& modifier::getRWParameters() {
    parameters* par = dynamic_cast<parameters*>(&functor::getRWParameters());

    if(par==0) {
      throw invalidParametersException(name());
    }

    return *par;
  }

  // destructor
  modifier::~modifier() {
  }

  // return class name
  const std::string& modifier::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  

  // set boundary type
  bool modifier::setBoundaryType(const eBoundaryType boundaryType) {
    parameters *param = 
      dynamic_cast<parameters*>(&parametersManager::getRWParameters());
    if(param==0) {
      throw invalidParametersException(name());
    }
    param->boundaryType = boundaryType;
    return updateParameters();
  }

//   // apply for float vector
//   bool modifier::apply(fvector& srcdest) const {
//     throw lti::functor::invalidMethodException();
//     return false;
//   }

//   // apply for integer vector
//   bool modifier::apply(ivector& srcdest) const {
//     throw lti::functor::invalidMethodException();
//     return false;
//   }

//   // apply for ubyte vector
//   bool modifier::apply(vector<ubyte>& srcdest) const {
//     throw lti::functor::invalidMethodException();
//     return false;
//   }

//   // apply for channel
//   bool modifier::apply(channel& srcdest) const {
//     throw lti::functor::invalidMethodException();
//     return false;
//   }

//   // apply for matrix of integers
//   bool modifier::apply(imatrix& srcdest) const {
//     throw lti::functor::invalidMethodException();
//     return false;
//   }

//   // apply for channel of unsigned bytes
//   bool modifier::apply(channel8& srcdest) const {
//     throw lti::functor::invalidMethodException();
//     return false;
//   }

  // apply for an image
  bool modifier::apply(image&) const {
    throw invalidMethodException();
    return false;
//     splitImageToRGB splitter;
//     mergeRGBToImage merger;
//     bool result = true;
//     channel red,green,blue;
//     channel rred,rgreen,rblue;

//     result = result && splitter.apply(srcdest,red,green,blue);

//     result = result && apply(red,rred);
//     result = result && apply(green,rgreen);
//     result = result && apply(blue,rblue);
      
//     result = result && merger.apply(rred,rgreen,rblue,srcdest);

//     return result;
  }

//   // apply (on copy) for a vector of floats
//   bool modifier::apply(const fvector& src, fvector& dest) const {
//     throw lti::functor::invalidMethodException();
//     return false;
//   }

//   // apply (on copy) for a vector of floats
//   bool modifier::apply(const ivector& src, ivector& dest) const {
//     throw lti::functor::invalidMethodException();
//     return false;
//   }

//   // apply (on copy) for a vector of floats
//   bool modifier::apply(const vector<ubyte>& src,
//                                        vector<ubyte>& dest) const {
//     throw lti::functor::invalidMethodException();
//     return false;
//   }

//   // apply (on copy) for a vector of floats
//   bool modifier::apply(const channel& src, channel& dest) const {
//     throw lti::functor::invalidMethodException();
//     return false;
//   }

//   // apply (on copy) for a vector of floats
//   bool modifier::apply(const imatrix& src, imatrix& dest) const {
//     throw lti::functor::invalidMethodException();
//     return false;
//   }

//   // apply (on copy) for a vector of floats
//   bool modifier::apply(const channel8& src, channel8& dest) const {
//     throw lti::functor::invalidMethodException();
//     return false;
//   }

  // apply (on copy) for a vector of floats
  bool modifier::apply(const image&, image&) const {
    throw invalidMethodException();
    return false;
    /*
    splitImageToRGB splitter;
    mergeRGBToImage merger;

    channel red,green,blue;
    channel rred,rgreen,rblue;

    splitter.apply(src,red,green,blue);

    apply(red,rred);
    apply(green,rgreen);
    apply(blue,rblue);
    
    merger.apply(rred,rgreen,rblue,dest);    

    return true; */
  }

}

