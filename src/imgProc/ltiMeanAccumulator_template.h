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
 * \file   ltiMeanAccumulator_template.h
 *         Contains the template class <meanAccumulator>
 * \author Arnd Hannemann
 * \date   26.8.2004
 *
 * revisions ..: $Id: ltiMeanAccumulator_template.h,v 1.6 2006-01-18 15:32:41 doerfler Exp $
 */

namespace lti {

  // --------------------------------------------------
  // meanAccumulator
  // --------------------------------------------------


  // default constructor
  template <typename T>
  meanAccumulator<T>::meanAccumulator() {
    reset();
  }

  // specialization for rgbaPixel
  meanAccumulator<rgbaPixel>::meanAccumulator() {
    reset();
  }

  // copy constructor
  template <typename T>
  meanAccumulator<T>::meanAccumulator(const meanAccumulator<T>& other) {
    copy(other);
  }

  // copy constructor
  meanAccumulator<rgbaPixel>::
  meanAccumulator(const meanAccumulator<rgbaPixel>& other) {
    copy(other);
  }


  // Destructor
  template <typename T>
  meanAccumulator<T>::~meanAccumulator() {
  }

  // specialization for rgbaPixel
  meanAccumulator<rgbaPixel>::~meanAccumulator() {
  }

  // copy
  template <typename T>
  inline meanAccumulator<T>& 
  meanAccumulator<T>::copy(const meanAccumulator<T>& other) {
    count_ = other.count_;
    sum_ = other.sum_;
    return *this;
  }

  // copy specialization for rgbaPixel
  inline meanAccumulator<rgbaPixel>& 
  meanAccumulator<rgbaPixel>::copy(const meanAccumulator<rgbaPixel>& other) {
    count_ = other.count_;
    rsum_ = other.rsum_;
    gsum_ = other.gsum_;
    bsum_ = other.bsum_;
    return *this;
  }

  // reset
  template <typename T>
  inline bool meanAccumulator<T>::reset() {
    count_ =  0;
    sum_ = 0;
    return true;
  }
  
  // specialization for rgbaPixel
  inline bool meanAccumulator<rgbaPixel>::reset() {
    count_ =  0;
    rsum_ = 0;
    gsum_ = 0;
    bsum_ = 0;
    return true;
  }

  template <typename T>
  inline int meanAccumulator<T>::getArraySize() {
    return ArraySize_;
  }

  inline int meanAccumulator<rgbaPixel>::getArraySize(){
    return ArraySize_;
  } 


  template <typename T>
  inline bool meanAccumulator<T>::getResult(double result[]) const {
    // copy elements
    result[Mean] = sum_ / count_;
    return true;
  }

  // specialization for rgbaPixel
  inline bool meanAccumulator<rgbaPixel>::getResult(double result[]) const {
    // normalization to one
    const double denom =  count_ * 255;
    result[RedMean]   = rsum_ / denom;
    result[GreenMean] = gsum_ / denom;
    result[BlueMean]  = bsum_ / denom;
    return true;
  }

  // accumulate
  template <typename T>
  inline bool meanAccumulator<T>::accumulate(const T& value,
					     const float&,
					     const float&) {
    sum_ += value;
    count_++;
    return true;
  }

  // accumulate
  template <typename T>
  inline bool meanAccumulator<T>::accumulate(const T& value,
                                             const float& weight,
					     const float&,
					     const float&) {
    sum_ += value * weight;
    count_ += weight;
    return true;
  }

  // specialization for rgbaPixel
  inline bool meanAccumulator<rgbaPixel>::accumulate(const rgbaPixel& value,
						     const float&,
						     const float&) {    
    rsum_ += value.getRed();
    gsum_ += value.getGreen();
    bsum_ += value.getBlue();
    count_++;
    return true;
  }

  // specialization for rgbaPixel
  inline bool meanAccumulator<rgbaPixel>::accumulate(const rgbaPixel& value,
                                                     const float& weight,
						     const float&,
						     const float&) {    
    rsum_ += value.getRed() * weight;
    gsum_ += value.getGreen() * weight;
    bsum_ += value.getBlue() * weight;
    count_+= weight;
    return true;
  }
}


