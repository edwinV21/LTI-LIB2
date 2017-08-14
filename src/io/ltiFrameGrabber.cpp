/*
 * Copyright (C) 1998 - 2005
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
 * \file   ltiFrameGrabber.cpp
 *         Base class for all framegrabbers and similar like USB and 
 *         FireWire cameras
 * \author Peter Mathes
 * \date   13.08.1999
 *
 * $Id: ltiFrameGrabber.cpp,v 1.9 2007-04-18 21:31:55 alvarado Exp $
 */

#include "ltiFrameGrabber.h"

namespace lti {

  // --------------------------------------------------------------------------
  //constructor
  frameGrabber::parameters::parameters() 
    : functor::parameters() {
    turnAround=false;
    snapShotMode=true;
  }

  //copy constructor
  frameGrabber::parameters::parameters(const parameters& other) 
    : functor::parameters() {
    copy(other);
  }

  // destructor
  frameGrabber::parameters::~parameters() {
  }

  // class name
  const std::string& frameGrabber::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  frameGrabber::parameters* frameGrabber::parameters::clone() const {
    return ( new parameters(*this) );
  }

  frameGrabber::parameters* frameGrabber::parameters::newInstance() const {
    return ( new parameters() );
  }

  frameGrabber::parameters&
    frameGrabber::parameters::copy(const parameters& other ) {

    functor::parameters::copy(other);

    turnAround=other.turnAround;
    snapShotMode=other.snapShotMode;

    return ( *this );
  }

  // alias for copy method
  frameGrabber::parameters&
  frameGrabber::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  /*
   * write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool frameGrabber::parameters::write(ioHandler& handler,
                                       const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    b = b && functor::parameters::write(handler,false);

    b = b && lti::write(handler,"turnAround",turnAround);
    b = b && lti::write(handler,"snapShotMode",snapShotMode);

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
  bool frameGrabber::parameters::read(ioHandler& handler,
                                      const bool complete) {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    b = b && functor::parameters::read(handler,false);

    b = b && lti::read(handler,"turnAround",turnAround);
    b = b && lti::read(handler,"snapShotMode",snapShotMode);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }


  // default constructor
  frameGrabber::frameGrabber() : functor() {
  }

  frameGrabber::frameGrabber( const parameters& theParam ) : functor() {
    setParameters( theParam );
  }

  frameGrabber::frameGrabber(const frameGrabber& other) : functor() {
    copy(other);
  }

  frameGrabber::~frameGrabber() {
  }

  // class name
  const std::string& frameGrabber::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  frameGrabber& frameGrabber::copy(const frameGrabber& other) {
    functor::copy(other);
    return (*this);
  }

  // alias for copy method
  frameGrabber&
  frameGrabber::operator=(const frameGrabber& other) {
    return copy(other);
  }

  const frameGrabber::parameters& frameGrabber::getParameters() const {
    const parameters* params =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if( params==0 )
      throw invalidParametersException(name());
    return *params;
  }

  // Apply method for channel
  bool frameGrabber::apply(channel& theChannel) {
    channel8 tmp;
    if (apply(tmp)) {
      theChannel.castFrom(tmp);
      return true;
    }
    return false;
  }

  bool frameGrabber::apply(channel32& theChannel) {
    channel8 tmp;
    if (apply(tmp)) {
      theChannel.castFrom(tmp);
      return true;
    }
    return false;
  }


} // namespace lti

