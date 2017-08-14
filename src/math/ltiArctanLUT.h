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


/** 
 * \file   ltiArctanLUT.h
 *         Contains the arctan lookup table.
 * \author Pablo Alvarado
 * \date   01.06.2003
 *
 * revisions ..: $Id: ltiArctanLUT.h,v 1.5 2012-09-16 03:28:50 alvarado Exp $
 */

#ifndef _LTI_ARCTAN_LUT_H_
#define _LTI_ARCTAN_LUT_H_

#include "ltiObject.h"
#include "ltiRound.h"
#include "ltiMath.h"
#include "ltiAssert.h"


namespace lti {
  /**
   * Look-up table based arcus tangents
   *
   * The following class provides a mono-state object that contains a 1MB long
   * Look-up Table to accelerate the computation of the arc-tangens.
   *
   * The provided functionality is similar to std::atan2(y,x) which computes
   * arctan(y/x) returning also the right angle quadrant.
   *
   * This method is used in time critical operations.  It is not appropriate
   * if you need exact computations of the arctan.  For that you can use the
   * standard method std::atan2, which is of course much more time expensive.
   *
   * As a mono-state class, only one instance of the LUT will be created.
   *
   * To use it, just create an instance and use the operator() as if you were
   * using std::atan2:
   *
   * \code
   * lti::arctanLUT myatan2;
   *
   * float angle = myatan2(20,10);
   * \endcode
   *
   * The returned angle will be always possitive 0 <= angle < 2*Pi (in radians)
   *
   * \warning This class must receive numbers as arguments.  If it gets special
   *          numbers as NaN or Inf, unpredicted results are to be expected, as
   *          wrong indices will be computed.
   * 
   * @ingroup gMath
   */
  class arctanLUT : public object {
  public:
    /**
     * Construct the class
     * Here, the LUT will be build just once (if not already done). 
     * The first time you construct this class, it will take about 0.15 seconds
     * to build the LUT.
     */
    arctanLUT();

    /**
     * Compute the arctan(dx/dy).
     *
     * It is a little bit slower than the other atan2() method, as it needs
     * to scale the input values into the range -255 and 255.  If you need
     * a extremely efficient computation, try the other atan2() method.
     *
     * @param dy y component.
     * @param dx x component.
     * @return arctan(dy/dx) in radians (always possitive between 0 and 2*Pi)
     */
    inline float operator() (const float dy,const float dx) const;

    /**
     * Compute the arctan(dx/dy).
     *
     * It is a little bit slower than the other atan2() method, as it needs
     * to scale the input values into the range -255 and 255.  If you need
     * a extremely efficient computation, try the other atan2() method.
     *
     * @param dy y component.
     * @param dx x component.
     * @return arctan(dy/dx) in radians (always possitive between 0 and 2*Pi)
     */
    inline float operator() (const double& dy,const double& dx) const;


    /**
     * Compute the arctan(dx/dy).
     *
     * This is the fastest method, but the value range of the input variables
     * is restricted.
     *
     * @param dy y component must be between -255 and 255
     * @param dx x component must be between -255 and 255
     * @return arctan(dy/dx) in radians (always possitive between 0 and 2*Pi)
     */
    inline float operator() (const int dy,const int dx) const;

    /**
     * Returns the name of this class
     */
    virtual const std::string& name() const;
    
    /**
     * Returns a pointer to a clone of the parameters.
     */
    virtual arctanLUT* clone() const;
    
    /**
     * Returns a pointer to a clone of the parameters.
     */
    virtual arctanLUT* newInstance() const;

  protected:
    /**
     * Construct a atan2 LUT.  It will assume that the difference values
     * of the gradients will be always between -255 and 255.  The 1MB Memory
     * required should not be a problem in modern PCs anymore!.
     */
    static void constructArcTanLUT();

    /**
     * Type required to create the LUT as a const structure
     */
    typedef const float* cfloatptr_;
    
    /**
     * The arctanLUT with floats.  After calling constructArcTanLUT() the
     * valid index range for both dimensions will be between -255 and 255.
     * The  arctanLUT[dy][dx] is equivalent to atan2(dy,dx) (but much faster).
     * The entries will always be between 0 and 2*Pi.
     */
    static const cfloatptr_* atanLUT_;
  };

  // implementation of inline methods
  inline float arctanLUT::operator() (const double& dy,
				      const double& dx) const {
    return this->operator()(static_cast<float>(dy),static_cast<float>(dx));
  }

  inline float arctanLUT::operator() (const float dy,const float dx) const {
    if (dy < 0.0f) {
      if (dx < 0.0f) {
        if (dx<dy) {
          return atanLUT_[iround(-255.0f*dy/dx)][-255];
        } else {
          return atanLUT_[-255][iround(-255.0f*dx/dy)];
        }
      } else {
        // dy < 0 and dx >= 0
        if (dx > -dy) {
          return atanLUT_[iround(dy*255.0f/dx)][255];
        } else {
          return atanLUT_[-255][iround(-255.0f*dx/dy)];
        }
      }
    } else {
      if (dx < 0.0f) {
        // dy >= 0 and dx < 0
        if (-dx > dy) {
          return atanLUT_[iround(-dy*255.0f/dx)][-255];
        } else {
          return atanLUT_[255][iround(dx*255.0f/dy)];
        }
      } else {
        // dy >= 0 and dx >= 0
        if (dx>dy) {
          return atanLUT_[iround(dy*255.0f/dx)][255];
        } else if (dx<dy) {
          return atanLUT_[255][iround(dx*255.0f/dy)];          
        } else if (!closeToZero(dx)) {
          return atanLUT_[255][255];
        }
      } 
    }
    return atanLUT_[0][0];    
  }

  inline float arctanLUT::operator() (const int dy,const int dx) const {
    // check in debug modus that range is ok
    assert((dy>-256) && (dy<256) && (dx>-256) && (dx<256));
    
    // just return the value
    return atanLUT_[dy][dx];
  }


}

#endif
