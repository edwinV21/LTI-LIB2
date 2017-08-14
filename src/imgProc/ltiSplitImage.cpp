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
 * \file   ltiSplitImage.cpp
 *         Default implementation of lti::splitImage::extractFirst(),
 *         extractSecond(), extractThird() as calls to the applies with other
 *         channels as dummies
 * \author Pablo Alvarado
 * \author Stefan Syberichs
 * \author Thomas Rusert
 * \date   19.04.1999
 *
 * $Id: ltiSplitImage.cpp,v 1.5 2006-01-13 04:57:56 alvarado Exp $
 */


#include "ltiSplitImage.h"
#include "ltiException.h"
#include "ltiConstants.h"

namespace lti {

  // default constructor
  splitImage::splitImage() : functor() {
  }

  // copy constructor
  splitImage::splitImage(const splitImage& other) : functor() {
    copy(other);
  }

  //destructor
  splitImage::~splitImage() {
  }

  // copy member
  splitImage& 
  splitImage::copy(const splitImage& other) {
    functor::copy(other);

    return (*this);
  }

  // alias for copy member
  splitImage&
  splitImage::operator=(const splitImage& other) {
    return copy(other);
  }

  // return parameters
  const splitImage::parameters&
  splitImage::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }

  /*
   * Returns the first of the three channels into which the image is split.
   */
  bool splitImage::extractFirst(const matrix<image::value_type>& img, 
                                matrix<channel::value_type>& c1) const {
    matrix<channel::value_type> c2,c3;
    return apply(img,c1,c2,c3);
  }

  /*
   * Returns the first of the three channels into which the image is split.
   */
  bool splitImage::extractFirst(const matrix<image::value_type>& img, 
                                matrix<channel8::value_type>& c1) const {
    matrix<channel8::value_type> c2,c3;
    return apply(img,c1,c2,c3);
  }

  /*
   * Returns the second of the three channels into which the image is split.
   */
  bool splitImage::extractSecond(const matrix<image::value_type>& img,
                                 matrix<channel::value_type>& c2) const {
    matrix<channel::value_type> c1,c3;
    return apply(img,c1,c2,c3);
  }

  /*
   * Returns the second of the three channels into which the image is split.
   */
  bool splitImage::extractSecond(const matrix<image::value_type>& img, 
                                 matrix<channel8::value_type>& c2) const {
    matrix<channel8::value_type> c1,c3;
    return apply(img,c1,c2,c3);
  }

  /*
   * Returns the third of the three channels into which the image is split.
   */
  bool splitImage::extractThird(const matrix<image::value_type>& img, 
                                matrix<channel::value_type>& c3) const {
    matrix<channel::value_type> c1,c2;
    return apply(img,c1,c2,c3);
  }

  /*
   * Returns the third of the three channels into which the image is split.
   */
  bool splitImage::extractThird(const matrix<image::value_type>& img,
                                matrix<channel8::value_type>& c3) const {
    matrix<channel8::value_type> c1,c2;
    return apply(img,c1,c2,c3);
  }


} // end of namespace lti

