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
 * \file   ltiHistogramAccumulator.h
 *         Contains the template class lti::histogramAccumulator<T>
 * \author Arnd Hannemann
 * \date   24.02.2005
 *
 * revisions ..: $Id: ltiHistogramAccumulator.h,v 1.2 2005-03-10 15:29:36 arndh Exp $
 */

#ifndef _LTI_HISTOGRAM_ACCUMULATOR_H_
#define _LTI_HISTOGRAM_ACCUMULATOR_H_

#include "ltiRGBAPixel.h"

namespace lti {

  template <typename T>

  /**
   * This accumulator calculates a histogram.
   *
   * Only useable for float,double,ubyte and rgbaPixel.
   *
   * @see localSampling
   */
  class histogramAccumulator {
  public:
    /**
     * Default Constructor
     */
    histogramAccumulator(const int bins = 36, 
                         const T minValue = 0,
                         const T maxValue = 1);    

    /**
     * Copy Constructor.
     */
    histogramAccumulator(const histogramAccumulator<T>& other);

    /**
     * Type of the accumulator
     */
    typedef T value_type;
    
    /**
     * Default Destructor
     */
    ~histogramAccumulator();

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
    histogramAccumulator<T>& 
    copy(const histogramAccumulator<T>& other);
    

  protected:   
    
    /**
     * Count how much pixels were accumulated 
     */
    double count_;

    /**
     * how much bins should be used
     */
    int bins_;
    
    /**
     * the lowest expected value
     */
    T minValue_;

    /**
     * the highest expected value
     */
    T maxValue_;

    /**
     * pointer to histogram array
     */
    double *histAry_;

    /**
     * the width of one bin
     */
    double binWidth_;
    
  };

  /**
   * This accumulator calculates a histogram.
   *
   *
   * @see localSampling
   */
  template <>
  class histogramAccumulator<rgbaPixel> {
  public:
    /**
     * Default Constructor
     */
    histogramAccumulator(const int bins,
                          const rgbaPixel::value_type minValue
                          =std::numeric_limits<rgbaPixel::value_type>::min(),
                          const rgbaPixel::value_type maxValue
                          =std::numeric_limits<rgbaPixel::value_type>::max());    
    /**
     * Copy Constructor.
     */
    histogramAccumulator(const histogramAccumulator<rgbaPixel>& other);

    /**
     * Default Destructor
     */
    ~histogramAccumulator();

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
    const histogramAccumulator<rgbaPixel>& 
    copy(const histogramAccumulator<rgbaPixel>& other);


  protected:   
    
    /**
     * Count how much pixels were accumulated 
     */
    double count_;

    /**
     * how much bins should be used
     */
    int bins_;
    
    /**
     * the lowest expected value in a color channel
     */
    rgbaPixel::value_type minValue_;

    /**
     * the highest expected value in a color channel
     */
    rgbaPixel::value_type maxValue_;

    /** 
     * histogram for red values 
     */
    double *rHistAry_;

    /**
     * histogram for green values 
     */
    double *gHistAry_;

    /**
     * histogram for blue values 
     */
    double *bHistAry_;

    /**
     * the width of one bin
     */
    double binWidth_;
    
  };

}
#include "ltiHistogramAccumulator_template.h"
#endif
