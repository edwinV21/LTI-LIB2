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
 * \file   ltiCornernessFunctor.cpp
 *         Contains the class cornernessFunctor, base class of functor
 *         finding a cornerness
 *         by Peter Doerfler.
 * \author Peter Doerfler
 * \date   13.1.2005
 *
 * revisions ..: $Id: ltiCornernessFunctor.cpp,v 1.6 2012-07-29 03:42:38 alvarado Exp $
 */

#include "ltiCornernessFunctor.h"

namespace lti {
  // --------------------------------------------------
  // cornernessFunctor::parameters
  // --------------------------------------------------

  // default constructor
  cornernessFunctor::parameters::parameters()
    : functor::parameters() {
  }

  // copy constructor
  cornernessFunctor::parameters::parameters(const parameters& other)
    : functor::parameters() {
    copy(other);
  }

  // destructor
  cornernessFunctor::parameters::~parameters() {
  }

  // copy member

  cornernessFunctor::parameters&
  cornernessFunctor::parameters::copy(const parameters& other) {
    functor::parameters::copy(other);

    

    return *this;
  }

  // alias for copy method
  cornernessFunctor::parameters&
  cornernessFunctor::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  // class name
  const std::string& cornernessFunctor::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone method
  cornernessFunctor::parameters* cornernessFunctor::parameters::clone() const {
    return new parameters(*this);
  }

  // new instance
  cornernessFunctor::parameters* cornernessFunctor::parameters::newInstance() const {
    return new parameters();
  }

  /*
   * Write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool cornernessFunctor::parameters::write(ioHandler& handler,
                                         const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {
      
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
  bool cornernessFunctor::parameters::read(ioHandler& handler,
                                        const bool complete)  {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {
      
    }

    b = b && functor::parameters::read(handler,false);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

  // --------------------------------------------------
  // cornernessFunctor
  // --------------------------------------------------

  // default constructor
  cornernessFunctor::cornernessFunctor()
    : functor() {

    //TODO: comment the attributes of your functor
    // If you add more attributes manually, do not forget to do following:
    // 1. indicate in the default constructor the default values
    // 2. make sure that the copy member also copy your new attributes, or
    //    to ensure there, that these attributes are properly initialized.

    // create an instance of the parameters with the default values
    parameters defaultParameters;
    // set the default parameters
    setParameters(defaultParameters);

    cornernessType_=Max;

  }

  // default constructor
  cornernessFunctor::cornernessFunctor(const parameters& par)
    : functor() {

    // set the given parameters
    setParameters(par);

    cornernessType_=Max;
  }


  // copy constructor
  cornernessFunctor::cornernessFunctor(const cornernessFunctor& other)
    : functor() {
    copy(other);
  }

  // destructor
  cornernessFunctor::~cornernessFunctor() {
  }

  // copy member
  cornernessFunctor& 
  cornernessFunctor::copy(const cornernessFunctor& other) {
    functor::copy(other);

    cornernessType_ = other.cornernessType_;

    return (*this);
  }

  // alias for copy member
  cornernessFunctor&
  cornernessFunctor::operator=(const cornernessFunctor& other) {
    return (copy(other));
  }

  // class name
  const std::string& cornernessFunctor::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // return parameters
  const cornernessFunctor::parameters&
  cornernessFunctor::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }

  //get cornernessType
  const cornernessFunctor::eCornernessType& 
  cornernessFunctor::getType() const {
    return cornernessType_;
  }

  // -------------------------------------------------------------------
  // The apply-methods!
  // -------------------------------------------------------------------


  // On place apply for type channel!
  bool cornernessFunctor::apply(channel& srcdest) const {
    channel tmp;
    if (apply(srcdest,tmp)) {
      // can we just use the result as new image?
      if (srcdest.getMode() == channel::Connected) {
        // yes: just transfer the data
        tmp.detach(srcdest);
      } else {
        // no: srcdest is part of another bigger image, just copy
        // the data.
        srcdest.fill(tmp);
      }
      return true;
    }

    return false;
  }

  // On place apply for type channel!
  bool cornernessFunctor::apply(channel& srcdest, 
                                float& cmin, float& cmax) const {
    channel tmp;
    if (worker(srcdest,tmp,cmin,cmax)) {
      // can we just use the result as new image?
      if (srcdest.getMode() == channel::Connected) {
        // yes: just transfer the data
        tmp.detach(srcdest);
      } else {
        // no: srcdest is part of another bigger image, just copy
        // the data.
        srcdest.fill(tmp);
      }
      return true;
    }

    return false;
  }

  // On copy apply for type channel!
  bool cornernessFunctor::apply(const channel& src, channel& dest) const {
    float tmin, tmax;
    return worker(src,dest,tmin,tmax);
  }

  // On copy apply for type channel!
  bool cornernessFunctor::apply(const channel& src, channel& dest,
                                float& cmin, float& cmax) const {
    return worker(src,dest,cmin,cmax);
  }

  // On copy apply for type channel8!
  bool cornernessFunctor::apply(const channel8& src, channel& dest) const {
    float tmin, tmax;
    return apply(src,dest,tmin,tmax);
  }

  // On copy apply for type channel8!
  bool cornernessFunctor::apply(const channel8& src, channel& dest,
                                float& cmin, float& cmax) const {
    channel tmp;
    tmp.castFrom(src);
    return worker(tmp,dest,cmin,cmax);
  }

  // On copy apply for type image!
  bool cornernessFunctor::apply(const image& src, channel& dest) const {
    float tmin, tmax;
    return worker(src,dest,tmin,tmax);
  }

  // On copy apply for type channel!
  bool cornernessFunctor::apply(const image& src, channel& dest,
                                float& cmin, float& cmax) const {
    return worker(src,dest,cmin,cmax);
  }

}
