/*
 * Copyright (C) 1998, 1999, 2000, 2001
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
 * \file   ltiMergeImage.h
 *         Contains mergeImage, which is a base class 
 *         all merge image functor classes.
 * \author Pablo Alvarado
 * \author Stefan Syberichs
 * \author Thomas Rusert
 * \date   19.04.1999
 * 
 * $Id: ltiMergeImage.h,v 1.4 2012-09-16 04:58:33 alvarado Exp $
 */

#ifndef _LTI_MERGE_IMAGE_H_
#define _LTI_MERGE_IMAGE_H_

#include "ltiFunctor.h"
#include "ltiImage.h"
#include "ltiMath.h"

namespace lti {
  /**
   * Abstract base class for all merge image functor-classes.
   *
   * These functors merge color-channels to a color image.
   *
   * This is a virtual class, so you cannot instantiate it.
   *
   * @ingroup gColor
   */
  class mergeImage : public functor {
  public:
    /**
     * default constructor
     */
    mergeImage();

    /**
     * destructor
     */
    virtual ~mergeImage();

    /**
     * returns a pointer to a clone of the functor.
     */
    virtual mergeImage* clone() const = 0;

    /**
     * returns a pointer to a new instance of the functor.
     */
    virtual mergeImage* newInstance() const = 0;

    /**
     * on-copy operator for 32-bit "floating-point" channels
     */
    virtual bool apply(const matrix<float>& c1,
                       const matrix<float>& c2,
                       const matrix<float>& c3,
                       image& img) const = 0;
    /**
     *  on-copy operator for 8-bit channels
     */
    virtual bool apply(const matrix<ubyte>& c1,
                       const matrix<ubyte>& c2,
                       const matrix<ubyte>& c3,
                       image& img) const = 0;

    /**
     * on-copy operator for 32-bit floating point values
     */
    virtual bool apply(const float& c1,
		       const float& c2,
		       const float& c3,
		       rgbaPixel& pixel) const = 0;

    /**
     * on-copy operator for 8-bit values
     */
    virtual bool apply(const ubyte& c1,
		       const ubyte& c2,
		       const ubyte& c3,
		       rgbaPixel& pixel) const = 0;

  };
}

#endif

