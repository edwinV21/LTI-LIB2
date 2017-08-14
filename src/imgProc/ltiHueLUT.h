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
 * \file   ltiHueLUT.h
 *         Contains the hue lookup table.
 * \author Pablo Alvarado
 * \author Peter Doerfler
 * \date   28.04.2005
 *
 * $Id: ltiHueLUT.h,v 1.3 2007-12-26 04:49:23 alvarado Exp $
 */

#ifndef _LTI_HUE_LUT_H_
#define _LTI_HUE_LUT_H_

#include "ltiObject.h"
#include "ltiMath.h"
#include "ltiAssert.h"

namespace lti {
  /**
   * Following class provides a mono-state object that contains a 0.5MB Look-up
   * Table to accelerate the computation of hues.
   * 
   * The hue is defined in terms of the RGB components and rgb chromaticities
   * as
   * \f[
   *   \begin{aligned}
   *      H &= \arccos\left(\frac{\frac{1}{2}\left[(R-G) + (R-B)\right]}
   *                       {\sqrt{(R-G)^2+(R-B)(G-B))}}\right) \\ 
   *        &= \arccos\left(\frac{\frac{1}{2}\left[(r-g) + (r-b)\right]}
   *                       {\sqrt{(r-g)^2+(r-b)(g-b))}}\right) \\
   *        &= \arccos\left[\frac{3r-1}
                                 {2\sqrt{3r^2+3g^2+3rg-3r-3g+1}}\right]
   *   \end{aligned}
   * \f]
   *
   * where for the last equation it was used the fact that \f$ r=1-g-b \f$.
   *
   * \b Note: This class normalizes the hue to be in the range [0,1]
   * which is more practical for the LTI-Lib than the typical 0..360
   * degrees definition.
   *
   * The LUT uses the fact that r+g+b=255 and thus only takes r and g as
   * inputs, where r,g, and b are the chromaticity values obtained by
   * normalization as r=R/(R+G+B), g=G/(R+G+B). These values need to be
   * multiplied by 511, which is the last index the LUT takes. Note that
   * r+g<512, and r,g >= 0
   *
   * As a mono-state class, only one instance of the LUT array will be created.
   *
   * @ingroup gColor
   */
  class hueLUT : public object {
  public:
    /**
     * Construct the class
     * Here, the LUT will be build just once (if not already done). 
     * The first time you construct this class, it will take about 0.15 seconds
     * to build the LUT.
     */
    hueLUT();

    /**
     * return the hue for (r,g). r,g >=0 and r+g<512
     *
     * @param r red component  
     * @param g green component
     * @return hue between 0 and 1
     */
    inline float operator() (const int r,const int g) const;

    /**
     * Returns the name of this class
     */
    virtual const std::string& name() const;
    
    /**
     * Returns a pointer to a clone of the parameters.
     */
    virtual hueLUT* clone() const;
    
    /**
     * Returns a pointer to a clone of the parameters.
     */
    virtual hueLUT* newInstance() const;

  protected:
    /**
     * Construct a hue LUT. It assumes that r,g>=0 and r+g<512.  The
     * 0.5MB memory required should not be a problem in modern PCs
     * anymore!.
     */
    static void constructHueLUT();

    /**
     * Type required to create the LUT as a const structure
     */
    typedef const float* cfloatptr_;
    
    /**
     * The hueLUT with floats.
     */
    static const cfloatptr_* hueLUT_;
  };

  inline float hueLUT::operator() (const int r,const int g) const {
    
#ifdef _LTI_DEBUG
    _lti_debug4("r=" << r << " g=" << g << "\n");
#endif

    // check in debug modus that range is ok
    assert((r>=0) && (g>=0) && ((r+g)<512));
    
    // just return the value
    return hueLUT_[r][g];
  }


}

#endif
