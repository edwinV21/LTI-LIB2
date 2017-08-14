/*
 * Copyright (C) 2009
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
 * \file   ltiAreaDescriptor.cpp
 *         Contains the class lti::areaDescriptor, 
 *         which is a data structure to hold a simple descriptor of 
 *         a group of pixels, mostly centered on the area covered by those
 *         pixels.
 * \author Pablo Alvarado
 * \date   18.10.2009
 *
 * revisions ..: $Id: ltiAreaDescriptor.cpp,v 1.4 2011-03-13 14:46:09 alvarado Exp $
 */

#include "ltiAreaDescriptor.h"
#include <limits>

namespace lti {
  // --------------------------------------------------
  // areaDescriptor::areaDescriptor
  // --------------------------------------------------

  // default constructor
  areaDescriptor::areaDescriptor() : ioObject() {
    reset();
  }

  // copy constructor
  areaDescriptor::areaDescriptor(const areaDescriptor& other)
    : ioObject() {
    copy(other);
  }

  // destructor
  areaDescriptor::~areaDescriptor() {
  }

  // copy member

  areaDescriptor&
  areaDescriptor::copy(const areaDescriptor& other) {

    area = other.area;
    maxX = other.maxX;
    minX = other.minX;
    minY = other.minY;
    maxY = other.maxY;
    cog = other.cog;

    return *this;
  }

  // alias for copy method
  areaDescriptor&
  areaDescriptor::operator=(const areaDescriptor& other) {
    return copy(other);
  }

  // class name
  const std::string& areaDescriptor::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone method
  areaDescriptor* areaDescriptor::clone() const {
    return new areaDescriptor(*this);
  }

  // new instance
  areaDescriptor* areaDescriptor::newInstance() const {
    return new areaDescriptor;
  }

  /*
   * Write the areaDescriptor in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool areaDescriptor::write(ioHandler& handler,
                                         const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {
      
      lti::write(handler,"area",area);
      lti::write(handler,"maxX",maxX);
      lti::write(handler,"minX",minX);
      lti::write(handler,"minY",minY);
      lti::write(handler,"maxY",maxY);
      lti::write(handler,"cog",cog);
    }

    if (complete) {
      b = b && handler.writeEnd();
    }

    return b;
  }

  /*
   * Read the areaDescriptor from the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also read, otherwise only the data block will be read.
   * @return true if read was successful
   */
  bool areaDescriptor::read(ioHandler& handler,
                                        const bool complete)  {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {
      
      lti::read(handler,"area",area);
      lti::read(handler,"maxX",maxX);
      lti::read(handler,"minX",minX);
      lti::read(handler,"minY",minY);
      lti::read(handler,"maxY",maxY);
      lti::read(handler,"cog",cog);
    }

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }


  /*
   * Set all values to zero, so that you can start considering new points
   */
  void areaDescriptor::reset() {
    area=0;

    minX.set(std::numeric_limits<int>::max(),0);
    minY.set(0,std::numeric_limits<int>::max());

    maxX.set(std::numeric_limits<int>::min(),0);
    maxY.set(0,std::numeric_limits<int>::min());

    cog.set(0.0f,0.0f);
  }

  /*
   * Consider another point into the descriptor
   */
  void areaDescriptor::consider(const ipoint& p) {
    consider(p.x,p.y);
  }

  /*
   * Consider a point into the descriptor
   */
  void areaDescriptor::consider(const int x,const int y) {
    if ( x < minX.x ) {
      minX.x = x;
      minX.y = y;
    }
    if ( x > maxX.x ) {
      maxX.x = x; 
      maxX.y = y;
    } 
    if ( y < minY.y ) {
      minY.x = x; 
      minY.y = y;
    }
    if ( y > maxY.y ) {
      maxY.x = x; 
      maxY.y = y;
    }  
 
    area++;
    cog.x += float(x);
    cog.y += float(y);
  }

  /*
   * Consider the other descriptor points, assuming the other
   * descriptor has not been normalized yet.
   */
  void areaDescriptor::considerUnnormalized(const areaDescriptor& other) {
    area += other.area;
    if ( area == 0 )  {
      return;
    }
    cog += other.cog;

    if ( minX.x > other.minX.x ) {
      minX = other.minX;
    }
    if ( maxX.x < other.maxX.x ) {
      maxX = other.maxX;
    }
    if ( minY.y > other.minY.y ) {
      minY = other.minY;
    }
    if ( maxY.y < other.maxY.y ) {
      maxY = other.maxY;
    }
   
  }

  areaDescriptor& areaDescriptor::join(const areaDescriptor& other) {
    considerUnnormalized(other);
    return *this;
  }

  /*
   * Consider the other descriptor points, assuming the other descriptor
   * has already been normalized
   */
  void areaDescriptor::considerNormalized(const areaDescriptor& other) {
    fpoint cog1 ( cog.x * area,cog.y * area );
    fpoint cog2 ( other.cog.x * other.area, other.cog.y * other.area );
    area += other.area;
    if ( area == 0 )  {
      return;
    }
    cog = ( cog1 + cog2 ) / area;
    if ( minX.x > other.minX.x ) {
      minX = other.minX;
    }
    if ( maxX.x < other.maxX.x ) {
      maxX = other.maxX;
    }
    if ( minY.y > other.minY.y ) {
      minY = other.minY;
    }
    if ( maxY.y < other.maxY.y ) {
      maxY = other.maxY;
    }

  }

  /*
   * Compute the value of cog.
   *
   * The attribute cog accumulates all coordinates given by the consider() 
   * methods.  It is until you call this method that this attribute will
   * honor its name.
   *
   * If you desire to keep considering more points, then you have to
   * call denormalizeCOG() first
   */
  void areaDescriptor::normalizeCOG() {
    if (area>0) {
      cog/=area;
    }
  }

  /*
   * Denormalize cog attribute to continue with point considerations.
   */
  void areaDescriptor::denormalizeCOG() {
    cog*=area;
  }
  
  /*
   * Check for equality
   */
  bool areaDescriptor::operator==(const areaDescriptor& other) const {
    return ( minX == other.minX && maxX == other.maxX &&
	     minY == other.minY && maxY == other.maxY &&
	     area == other.area && cog == other.cog );

  }

  /*
   * Check for inequality
   */
  bool areaDescriptor::operator!=(const areaDescriptor& other) const {
    return ( minX != other.minX && maxX != other.maxX &&
	     minY != other.minY && maxY != other.maxY &&
	     area != other.area && cog != other.cog );
  }

  /*
   * Check for closeness
   *
   * Two area descriptors are said to be close to each other if the
   * rectangulare bouding box of this descriptor expanded by the given border
   * (in x and in y directions by the respective coordinates) overlaps with
   * the not expanded rectangular bounding box of the other descriptor.
   */
  bool areaDescriptor::isClose(const areaDescriptor& other,
                               const ipoint& dist) const {
    const irectangle thisBox(computeBoundingBox());
    const irectangle otherBox(other.computeBoundingBox());
    return thisBox.isClose(otherBox,dist);

  }

  irectangle areaDescriptor::computeBoundingBox(const ipoint& border) const {
    return irectangle(minX.x-border.x,minY.y-border.y,
                      maxX.x+border.x,maxY.y+border.y);
  }
}
