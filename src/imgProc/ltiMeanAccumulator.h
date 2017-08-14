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
 * \file   ltiMeanAccumulator.h
 *         Contains the template class lti::meanAccumulator<T>
 * \author Arnd Hannemann
 * \date   27.8.2004
 *
 * revisions ..: $Id: ltiMeanAccumulator.h,v 1.14 2005-03-16 16:20:49 doerfler Exp $
 */

#ifndef _LTI_MEAN_ACCUMULATOR_H_
#define _LTI_MEAN_ACCUMULATOR_H_

#include "ltiRGBAPixel.h"

namespace lti {

  template <typename T>

  /**
   * This accumulator calculates the mean.
   *
   * Only useable for float,double,ubyte and rgbaPixel.
   *
   * @see localSampling
   */
  class meanAccumulator {
  public:
    /**
     * Default constructor.
     */
    meanAccumulator();    

    /**
     * Copy constructor.
     * @param other the accumulator object to be copied
     */
    meanAccumulator(const meanAccumulator<T>& other);

    /**
     * Type of the accumulator
     */
    typedef T value_type;
    
    /**
     * Destructor
     */
    ~meanAccumulator();

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
     * @param weight the value will weighted with that
     * @param posx x-position of the value (not used here)
     * @param posy y-position of the value (not used here)
     */
    inline bool accumulate(const T& value, 
                           const float& weight,
                           const float& posx, const float& posy); 

              
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
     * Index for mean in the result array
     */
    enum {
      Mean = 0 /**< Index for mean in the result array */
    };

    /**
     * Copy data of "other" accumulator.
     *
     * @param other the accumulator to be copied
     * @return a reference to this accumulator object
     */
    inline meanAccumulator<T>& copy(const meanAccumulator<T>& other);
    

  protected:   
    
    /**
     * Count how much pixels were accumulated 
     */
    double count_;

    /**
     * Size of the result array is 1
     */
    enum {
      ArraySize_ = 1 /**< Size of the result array is 1 */
    };

    /**
     * Sum of all accumulated values 
     */
    double sum_;
    
  };

  /**
   * This accumulator calculates the mean for rgbaPixel.
   *
   * The mean is normalized to [0,1].
   *
   * @see localSampling
   */
  template <>
  class meanAccumulator<rgbaPixel> {
  public:
    /**
     * Default constructor.
     */
    meanAccumulator();    

    /**
     * Copy constructor.
     * @param other the accumulator object to be copied
     */
    meanAccumulator(const meanAccumulator<rgbaPixel>& other);
    
    /**
     * Destructor
     */
    ~meanAccumulator();

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
     * Copy data of "other" accumulator.
     * As this accumulator has no data to be copied this method is 
     * does nothing.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    inline meanAccumulator<rgbaPixel>& 
    copy(const meanAccumulator<rgbaPixel>& other);

    /**
     * Indices names in the result array
     */
    enum {
      RedMean=0,      /**< Index for red mean in the result array       */
      GreenMean=1,    /**< Index for green mean in the result array     */
      BlueMean=2     /**< Index for blue mean in the result array      */
    };

  protected:   
    
    /**
     * Count how much pixels were accumulated 
     */
    double count_;

    /**
     * Size of the result array
     */
    enum {
      ArraySize_ = 3 /**< Size of the result array */
    };

    /** red sum of all accumulated values */
    double rsum_;

    /** green sum of all accumulated value */
    double gsum_;

    /** blue sum of all accumulated value */
    double bsum_;
    
  };

}
#include "ltiMeanAccumulator_template.h"
#endif
