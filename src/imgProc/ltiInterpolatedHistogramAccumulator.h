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
 * \file   ltiInterpolatedHistogramAccumulator.h
 *         Contains the template class lti::interpolatedHistogramAccumulator<T>
 * \author Arnd Hannemann
 * \date   24.02.2005
 *
 * revisions ..: $Id: ltiInterpolatedHistogramAccumulator.h,v 1.2 2005-04-22 15:34:38 doerfler Exp $
 */

#ifndef _LTI_INTERPOLATED_HISTOGRAM_ACCUMULATOR_H_
#define _LTI_INTERPOLATED_HISTOGRAM_ACCUMULATOR_H_

#include "ltiRGBAPixel.h"

namespace lti {

  template <typename T>

  /**
   * This accumulator calculates a interpolated histogram.
   *
   * Only useable for float,double,ubyte and rgbaPixel.
   *
   * @see localSampling
   */
  class interpolatedHistogramAccumulator {
  public:
    /**
     * Default Constructor.
     *
     * @param bins how large the histogram will be.
     * @param minValue the accumulater expects
     *  values in the interval [minValue,maxValue).
     * @param maxValue the accumulater expects
     *  values in the interval [minValue,maxValue).
     */
    interpolatedHistogramAccumulator(const int bins = 36, 
                                     const T minValue = 0,
                                     const T maxValue = 1);    

    /**
     * Copy Constructor.
     */
    interpolatedHistogramAccumulator(const interpolatedHistogramAccumulator<T>& other);

    /**
     * Type of the accumulator
     */
    typedef T value_type;
    
    /**
     * Default Destructor
     */
    ~interpolatedHistogramAccumulator();

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
    inline int getArraySize() const;

    /**
     * Copy data of "other" accumulator.
     * @param other the accumulator to be copied
     * @return a reference to this accumulator object
     */
    interpolatedHistogramAccumulator<T>& 
    copy(const interpolatedHistogramAccumulator<T>& other);
    

  protected:   
    
    /**
     * Count how much pixels were accumulated. 
     */
    double count_;

    /**
     * How much bins should be used.
     */
    int bins_;
    
    /**
     * The lowest expected value.
     */
    T minValue_;

    /**
     * The highest expected value.
     */
    T maxValue_;

    /**
     * Pointer to histogram array.
     */
    double *histAry_;

    /**
     * The width of one bin.
     */
    double binWidth_;

    /**
     * Internally used for faster accumulate.
     */
    float offset_;
    
  };

  /**
   * This accumulator calculates a interpolated histogram.
   *
   *
   * @see localSampling
   */
  template <>
  class interpolatedHistogramAccumulator<rgbaPixel> {
  public:
    /**
     * Default Constructor
     */
    interpolatedHistogramAccumulator(const int bins,
                          const rgbaPixel::value_type minValue
                          =std::numeric_limits<rgbaPixel::value_type>::min(),
                          const rgbaPixel::value_type maxValue
                          =std::numeric_limits<rgbaPixel::value_type>::max());    
    /**
     * Copy Constructor.
     */
    interpolatedHistogramAccumulator(const interpolatedHistogramAccumulator<rgbaPixel>& other);

    /**
     * Default Destructor
     */
    ~interpolatedHistogramAccumulator();

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
    inline int getArraySize() const;

    /**
     * Copy data of "other" accumulator.
     * As this accumulator has no data to be copied this method is 
     * does nothing.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    const interpolatedHistogramAccumulator<rgbaPixel>& 
    copy(const interpolatedHistogramAccumulator<rgbaPixel>& other);


  protected:   
    
    /**
     * Count how much pixels were accumulated.
     */
    double count_;

    /**
     * Size of histograms.
     */
    int bins_;
    
    /**
     * Lowest expected value in a color channel.
     */
    rgbaPixel::value_type minValue_;

    /**
     * Highest expected value in a color channel.
     *
     */
    rgbaPixel::value_type maxValue_;

    /** 
     * Histogram for red values.
     */
    double *rHistAry_;

    /**
     * Histogram for green values.
     */
    double *gHistAry_;

    /**
     * Histogram for blue values.
     */
    double *bHistAry_;

    /**
     * Width of one bin.
     */
    float binWidth_;
    
    /**
     * Internally used for faster accumulate().
     */
    float offset_;

  };

}
#include "ltiInterpolatedHistogramAccumulator_template.h"
#endif
