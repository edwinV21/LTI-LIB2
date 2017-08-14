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
 * \file   ltiVarianceAccumulator_template.h
 *         Contains the template class varianceAccumulator<T>
 * \author Arnd Hannemann
 * \date   26.8.2004
 *
 * revisions ..: $Id: ltiVarianceAccumulator_template.h,v 1.7 2006-01-18 15:32:41 doerfler Exp $
 */

namespace lti {

  // --------------------------------------------------
  // varianceAccumulator
  // --------------------------------------------------


  // default constructor
  template <typename T>
  varianceAccumulator<T>::varianceAccumulator() {
    reset();
  }

  // specialization for rgbaPixel
  varianceAccumulator<rgbaPixel>::varianceAccumulator() {
    reset();
  }

  // copy constructor
  template <typename T>
  varianceAccumulator<T>::
  varianceAccumulator(const varianceAccumulator<T>& other) {
    copy(other);
  }

 // copy constructor
  varianceAccumulator<rgbaPixel>::
  varianceAccumulator(const varianceAccumulator<rgbaPixel>& other) {
    copy(other);
  }

  // copy
  template<typename T>
  inline varianceAccumulator<T>& 
  varianceAccumulator<T>::copy(const varianceAccumulator<T>& other) {
    count_ = other.count_;
    sum_ = other.sum_;
    squareSum_ = other.squareSum_;
    return *this;
  }

  // copy / specialization for rgbaPixel
  inline varianceAccumulator<rgbaPixel>& 
  varianceAccumulator<rgbaPixel>
  ::copy(const varianceAccumulator<rgbaPixel>& other) {
    count_ = other.count_;
    rsum_ = other.rsum_;
    gsum_ = other.gsum_;
    bsum_ = other.bsum_;
    rSquareSum_ = other.rSquareSum_;
    gSquareSum_ = other.gSquareSum_;
    bSquareSum_ = other.bSquareSum_;
    return *this;
  }

  // Default Destructor
  template <typename T>
  varianceAccumulator<T>::~varianceAccumulator() {
  }

  // specialization for rgbaPixel
  varianceAccumulator<rgbaPixel>::~varianceAccumulator() {
  }

  // reset
  template <typename T>
  inline bool varianceAccumulator<T>::reset() {
    count_ =  0;
    sum_ = 0;
    squareSum_ = 0;
    return true;
  }
  
  // specialization for rgbaPixel
  inline bool varianceAccumulator<rgbaPixel>::reset() {
    count_ =  0;
    rsum_ = 0;
    gsum_ = 0;
    bsum_ = 0;
    rSquareSum_ = 0;
    gSquareSum_ = 0;
    bSquareSum_ = 0;    
    return true;
  }

  template <typename T>
  inline int varianceAccumulator<T>::getArraySize() {
    return static_cast<int>(ArraySize_);
  }

  inline int varianceAccumulator<rgbaPixel>::getArraySize() {
    return static_cast<int>(ArraySize_);
  }

  template <typename T>
  inline bool varianceAccumulator<T>::getResult(double result[]) const {
    // copy elements
    const double mean = sum_ / count_;
    result[Mean] = mean;
    // unbiased variance
    result[Variance] 
      = static_cast<double>(squareSum_)/(count_-1) - mean*mean;
    return true;
  }

  // specialization for rgbaPixel
  inline bool varianceAccumulator<rgbaPixel>::getResult(double result[]) const{
    const double denom =  count_ * 255;
    const double rmean = rsum_ / denom;
    const double gmean = gsum_ / denom;
    const double bmean = bsum_ / denom;
    const double denom2 = 255*255*(count_-1);
    result[RedMean] = rmean;
    result[GreenMean] = gmean;
    result[BlueMean] = bmean;
    result[RedVariance] = rSquareSum_/denom2-rmean*rmean;
    result[GreenVariance] = gSquareSum_/denom2-gmean*gmean;
    result[BlueVariance] = bSquareSum_/denom2-bmean*bmean;
    return true;
  }

  // accumulate
  template <typename T>
  inline bool varianceAccumulator<T>::accumulate(const T& value,
						 const float&,
						 const float&) {
    sum_ += value;
    squareSum_ += value*value;
    count_++;
    return true;
  }

  // specialization for rgbaPixel
  inline bool 
  varianceAccumulator<rgbaPixel>::accumulate(const rgbaPixel& value,
					     const float&,
					     const float&) {    
    const int r = value.getRed();
    const int g = value.getGreen();
    const int b = value.getBlue();
    rsum_ += r;
    gsum_ += g;
    bsum_ += b;
    rSquareSum_ += r*r;
    gSquareSum_ += g*g;
    bSquareSum_ += b*b;
    count_++;
    return true;
  }

  // accumulate
  template <typename T>
  inline bool varianceAccumulator<T>::accumulate(const T& value,
                                                 const float& weight,
						 const float&,
						 const float&) {
    const double val = value*weight;
    sum_ += val;
    squareSum_ += value*val;
    count_+= weight;
    return true;
  }


  // specialization for rgbaPixel
  inline bool 
  varianceAccumulator<rgbaPixel>::accumulate(const rgbaPixel& value,
                                             const float& weight,
					     const float&,
					     const float&) {    
    const double r = value.getRed();
    const double g = value.getGreen();
    const double b = value.getBlue();
    const double rw = r*weight;
    const double gw = g*weight;
    const double bw = b*weight;
    rsum_ += rw;
    gsum_ += gw;
    bsum_ += bw;
    rSquareSum_ += r*rw;
    gSquareSum_ += g*gw;
    bSquareSum_ += b*bw;
    count_+= weight;
    return true;
  }
  
}


