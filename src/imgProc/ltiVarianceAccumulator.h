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
 * \file   ltiVarianceAccumulator.h
 *         Contains the template class lti::varianceAccumulator<T>
 * \author Arnd Hannemann
 * \date   27.8.2004
 *
 * revisions ..: $Id: ltiVarianceAccumulator.h,v 1.11 2005-03-17 12:57:22 arndh Exp $
 */

#ifndef _LTI_VARIANCE_ACCUMULATOR_H_
#define _LTI_VARIANCE_ACCUMULATOR_H_


#include "ltiRGBAPixel.h"

namespace lti {

  /**
   * This accumulator calculates the mean and variance.
   *
   * Only useable for float,double,ubyte and rgbaPixel.
   *
   * @see localSampling
   */
  template <typename T>
  class varianceAccumulator {
  public:
    /**
     * Default constructor.
     */
    varianceAccumulator();   
  
    /**
     * Copy constructor.
     * @param other the accumulator object to be copied
     */
    varianceAccumulator(const varianceAccumulator<T>& other);
    
    /**
     * Destructor.
     */
    ~varianceAccumulator();

    /**
     * Type of the accumulator
     */
    typedef T value_type;
    
    /**
     * Accumulates a given value
     * @param value value to accumulate
     * @param posx x-position of the value (not used here)
     * @param posy y-position of the value (not used here)
     */
    inline bool accumulate(const T& value, 
                           const float& posx, const float& posy); 

    /**
     * Accumulates a given value
     * @param value value to accumulate
     * @param weight the value will be weighted with that.
     * @param posx x-position of the value (not used here)
     * @param posy y-position of the value (not used here)
     */
    inline bool accumulate(const T& value, 
                           const float& weight,
                           const float& posx, const float& posy); 
          
    /**
     * Copy data of "other" accumulator.
     * As this accumulator has no data to be copied this method
     * does nothing.
     * @param other the accumulator to be copied
     * @return a reference to this accumulator object
     */
    inline varianceAccumulator<T>& copy(const varianceAccumulator<T>& other);


    /** 
     * Resets all stored information
     */
    inline bool reset();
    
    /**      
     * Returns the stored information.
     */
    inline bool getResult(double result[]) const;

    /**
     * Return size of the resulting array
     */
    inline static int getArraySize();

    /**
     * Names for the indices of the result array
     */
    enum {
      Mean=0,    /**< Index for mean in the result array     */
      Variance=1 /**< Index for variance in the result array */
    };

  protected:   
    
    /**
     * Count how much pixels were accumulated 
     */
    double count_;

    /** 
     * Size of the result array is 1
     */
    enum {
      ArraySize_ = 2
    };

    /** 
     * Sum of all accumulated values 
     */
    double sum_;

    /**
     * Squared sum of all accumulated values 
     */
    double squareSum_;    
  };

  /**
   * 
   */
  template <>
  class varianceAccumulator<rgbaPixel> {
  public:
    /**
     * Default constructor.
     */
    varianceAccumulator();    
    
    /**
     * Copy constructor.
     * @param other the accumulator object to be copied
     */
    varianceAccumulator(const varianceAccumulator<rgbaPixel>& other);

    /**
     * Destructor.
     */
    ~varianceAccumulator();

    /**
     * Type of the accumulator
     */
    typedef rgbaPixel value_type;
    

    /**
     * Accumulates a given value
     * @param value value to accumulate
     * @param posx x-position of the value (not used here)
     * @param posy y-position of the value (not used here)
     */
    inline bool accumulate(const rgbaPixel& value, 
                           const float& posx, const float& posy); 

    /**
     * Accumulates a given value
     * @param value value to accumulate
     * @param weight the value will weighted with that
     * @param posx x-position of the value (not used here)
     * @param posy y-position of the value (not used here)
     */
    inline bool accumulate(const rgbaPixel& value, 
                           const float& weight,
                           const float& posx, const float& posy); 

    /**
     * Copy data of "other" accumulator.
     * As this accumulator has no data to be copied this method
     * does nothing.
     * @param other the accumulator to be copied
     * @return a reference to this accumulator object
     */
    inline varianceAccumulator<rgbaPixel>& 
    copy(const varianceAccumulator<rgbaPixel>& other);
              
    /** 
     * Resets all stored information
     */
    inline bool reset();
    
    /**      
     * Returns the stored information.
     */
    inline bool getResult(double result[]) const;

    /**
     * Return size of the resulting array
     */
    inline static int getArraySize();

    /**
     * Indices names in the result array
     */
    enum {
      RedMean=0,      /**< Index for red mean in the result array       */
      GreenMean=1,    /**< Index for green mean in the result array     */
      BlueMean=2,     /**< Index for blue mean in the result array      */
      RedVariance=3,  /**< Index for red variance in the result array   */
      GreenVariance=4,/**< Index for green variance in the result array */
      BlueVariance=5  /**< Index for blue variance in the result array  */
    };

  protected:   
    
    /** 
     * count how much pixels were accumulated 
     **/
    double count_;

    /**
     * Size of the result array
     */
    enum {
      ArraySize_ = 6
    };

    /** red sum of all accumulated values */
    double rsum_;

    /** green sum of all accumulated value */
    double gsum_;

    /** blue sum of all accumulated value */
    double bsum_;

    /** red sum of squared accumulated values */
    double rSquareSum_;

    /** green sum of squared accumulated value */
    double gSquareSum_;

    /** blue sum of squared accumulated value */
    double bSquareSum_;
  };
}
#include "ltiVarianceAccumulator_template.h"
#endif
