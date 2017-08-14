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
 * \file   ltiInterpolatedHistogramAccumulator_template.h
 *         Contains the template class interpolatedHistogramAccumulator<T>
 * \author Arnd Hannemann
 * \date   24.02.2005
 *
 * revisions ..: $Id: ltiInterpolatedHistogramAccumulator_template.h,v 1.8 2007-12-26 04:49:23 alvarado Exp $
 */

#include "ltiDebug.h"

namespace lti {

  // --------------------------------------------------
  // interpolatedHistogramAccumulator
  // --------------------------------------------------


  // default constructor
  template <typename T>
  interpolatedHistogramAccumulator<T>::interpolatedHistogramAccumulator(const int bins,
                                                const T minValue,
                                                const T maxValue) {

    bins_ = bins;
    minValue_ = minValue;
    maxValue_ = maxValue;
    histAry_  = new double[bins];
    binWidth_ = static_cast<float>(maxValue-minValue) / bins;
    offset_   = static_cast<float>(-minValue_)-static_cast<float>(binWidth_)/2.f;
    _lti_debug("binWidth_:"<<binWidth_<<"\n");
    reset();
  }

  
  // specialization for rgbaPixel
  interpolatedHistogramAccumulator<rgbaPixel>::
  interpolatedHistogramAccumulator(const int bins,
                                   const rgbaPixel::value_type minValue,
                                   const rgbaPixel::value_type maxValue) {
    bins_ = bins;
    minValue_ = minValue;
    maxValue_ = maxValue;
    rHistAry_ = new double[bins];
    gHistAry_ = new double[bins];
    bHistAry_ = new double[bins];
    binWidth_ = static_cast<float>(maxValue-minValue) / bins;
    offset_   = -static_cast<float>(minValue_)-binWidth_/2.f;
    reset();
  }

  // copy constructor
  template <typename T>
  interpolatedHistogramAccumulator<T>::
  interpolatedHistogramAccumulator(const interpolatedHistogramAccumulator<T>& other) {
    bins_ = 0;
    histAry_ = 0;
    copy(other);
  }

  // copy constructor - specialization for rgbaPixel
  interpolatedHistogramAccumulator<rgbaPixel>::
  interpolatedHistogramAccumulator(const interpolatedHistogramAccumulator<rgbaPixel>& other) {
    rHistAry_ = 0;
    gHistAry_ = 0;
    bHistAry_ = 0;
    bins_ = 0;
    copy(other);
  }

  // Destructor
  template <typename T>
  inline interpolatedHistogramAccumulator<T>::~interpolatedHistogramAccumulator() {
    delete[] histAry_;
    histAry_ = 0;
  }

  // specialization for rgbaPixel
  interpolatedHistogramAccumulator<rgbaPixel>::~interpolatedHistogramAccumulator() {
    delete[] rHistAry_;
    delete[] gHistAry_;
    delete[] bHistAry_;
    rHistAry_ = gHistAry_ = bHistAry_ = 0;
  }
  
  // copy
  template <typename T>
  interpolatedHistogramAccumulator<T>&
  interpolatedHistogramAccumulator<T>::copy(const interpolatedHistogramAccumulator<T>& other){

    // if size has changed allocate new memory
    if (bins_ != other.bins_) {
      bins_ = other.bins_;
      delete[] histAry_;
      histAry_ = new double[bins_];
    }
    
    // copy memory blockwise
    memcpy(histAry_,other.histAry_,bins_*sizeof(double));

    // copy other variables
    minValue_ = other.minValue_;
    maxValue_ = other.maxValue_;
    binWidth_ = other.binWidth_;
    count_    = other.count_;
    offset_   = other.offset_;
    
    return *this;
  }

  // copy - specialization for rgbaPixel
  const interpolatedHistogramAccumulator<rgbaPixel>&
  interpolatedHistogramAccumulator<rgbaPixel>::copy(const interpolatedHistogramAccumulator<rgbaPixel>& other){

    // if size has changed allocate new memory
    if (bins_ != other.bins_) {
      bins_ = other.bins_;
      delete[] rHistAry_;
      delete[] gHistAry_;
      delete[] bHistAry_;
      rHistAry_ = new double[bins_];
      gHistAry_ = new double[bins_];
      bHistAry_ = new double[bins_];
    }

    // copy memory blockwise
    memcpy(rHistAry_,other.rHistAry_,bins_*sizeof(double));
    memcpy(gHistAry_,other.gHistAry_,bins_*sizeof(double));
    memcpy(bHistAry_,other.bHistAry_,bins_*sizeof(double));

    // other variables
    minValue_ = other.minValue_;
    maxValue_ = other.maxValue_;
    binWidth_ = other.binWidth_;
    count_    = other.count_;
    offset_   = other.offset_;

    return *this;
  }


  // reset
  template <typename T>
  inline bool interpolatedHistogramAccumulator<T>::reset() {
    count_ =  0;
    int i;
    for (i=0; i<bins_;i++) {
      histAry_[i] = 0;
    }
    return true;
  }
  
  // specialization for rgbaPixel
  inline bool interpolatedHistogramAccumulator<rgbaPixel>::reset() {
    count_ =  0;
    int i;
    for (i=0; i<bins_;i++) {
      rHistAry_[i] = 0;
      gHistAry_[i] = 0;
      bHistAry_[i] = 0;
    }    
    return true;
  }

  template <typename T>
  inline int interpolatedHistogramAccumulator<T>::getArraySize() const{
    return bins_;
  }

  inline int interpolatedHistogramAccumulator<rgbaPixel>::getArraySize() const{
    return bins_*3;
  } 


  template <typename T>
  inline bool interpolatedHistogramAccumulator<T>::getResult(double result[]) const{
    // copy elements
    memcpy(result,histAry_,bins_*sizeof(double));
  

    // normalization
    if (count_!=0) {
      int i;
      for (i = 0;i<bins_;i++) {
        result[i] /= count_;
      }
    }

    return true;
  }

  // specialization for rgbaPixel
  inline bool interpolatedHistogramAccumulator<rgbaPixel>::getResult(double result[]) const {

    
    memcpy(result,rHistAry_,bins_*sizeof(double));
    //offset
    memcpy(result+bins_,gHistAry_,bins_*sizeof(double));
    memcpy(result+2*bins_,bHistAry_,bins_*sizeof(double));

    // normalization
    const double denom =  count_;

    if (denom!=0) {
      int i;
      for (i = 0;i<getArraySize();i++) {
        result[i] /= denom;
      }
    }
    return true;
  }

  // accumulate
  template <typename T>
  inline bool interpolatedHistogramAccumulator<T>::accumulate(const T& value,
                                                              const float&,
                                                              const float&) {

    // value out of bounds
    if ((value < minValue_) || (value >= maxValue_)) return false;
    
    // if a value is in the center of a bin it is only accumulated to one bin 
    // offset_ = -minValue_-binWidth_/2;
    const float fIndex   = (static_cast<float>(value)+offset_) / static_cast<float>(binWidth_);
    const int indexLeft  = static_cast<int>(fIndex);
    const int indexRight = indexLeft +1;

    // if a value is at the bounds of the histogram the 
    // only one bin is increased by one
    if ((fIndex < 0) || (indexRight >= bins_)) {
      histAry_[indexLeft]++;
    } else {
      const float weight   = fIndex-static_cast<float>(indexLeft);
      histAry_[indexLeft] += 1.0-weight;
      histAry_[indexRight] += weight;
    }
    _lti_debug("fIndex:"<<fIndex<<"\n");
    _lti_debug("indexLeft:"<<indexLeft<<"\n");
    _lti_debug("indexRight:"<<indexRight<<"\n");
    count_++;
    return true;
  }

  // accumulate
  template <typename T>
  inline bool interpolatedHistogramAccumulator<T>
  ::accumulate(const T& value,
               const float& weight,
               const float&,
               const float&) {
    
    // value out of bounds
    if ((value < minValue_) || (value >= maxValue_)) return false;
    
    // if a value is in the center of a bin it is only accumulated to one bin 
    // offset_ = -minValue_-binWidth_/2;
    const float fIndex   = (static_cast<float>(value)+offset_) / static_cast<float>(binWidth_);
    const int indexLeft  = static_cast<int>(fIndex);
    const int indexRight = indexLeft +1;

    // if a value is at the bounds of the histogram the 
    // only one bin is increased by one
    if ((fIndex < 0) || (indexRight >= bins_)) {
      histAry_[indexLeft] += weight;
    } else {
      const float w   = (fIndex-static_cast<float>(indexLeft))*weight;
      histAry_[indexLeft] += weight-w;
      histAry_[indexRight] += w;
    }

    count_ += weight;

    return true;

  }

  // specialization for rgbaPixel
  inline bool 
  interpolatedHistogramAccumulator<rgbaPixel>
  ::accumulate(const rgbaPixel& value,
               const float&,
               const float&) {    
    ubyte ru,gu,bu;
    value.get(ru,gu,bu);
    const float r = static_cast<float>(ru);
    const float g = static_cast<float>(gu);
    const float b = static_cast<float>(bu);

    // value out of bounds
    if ((r < minValue_) || (r >= maxValue_)) return false;
    if ((g < minValue_) || (g >= maxValue_)) return false;
    if ((b < minValue_) || (b >= maxValue_)) return false;    
    
    // if a value is in the center of a bin it is only accumulated to one bin 
    // offset_ = -minValue_-binWidth_/2;
    const float fIndexR   = (static_cast<float>(r)+offset_) / binWidth_;
    const int indexLeftR  = static_cast<int>(fIndexR);
    const int indexRightR = indexLeftR +1;

    const float fIndexG   = (static_cast<float>(g)+offset_) / binWidth_;
    const int indexLeftG  = static_cast<int>(fIndexG);
    const int indexRightG = indexLeftG +1;

    const float fIndexB   = (static_cast<float>(b)+offset_) / binWidth_;
    const int indexLeftB  = static_cast<int>(fIndexB);
    const int indexRightB = indexLeftB +1;

    // if a value is at the bounds of the histogram the 
    // only one bin is increased by one
    // RED
    if ((fIndexR < 0) || (indexRightR >= bins_)) {
      rHistAry_[indexLeftR]++;
    } else {
      const float weightR   = fIndexR-static_cast<float>(indexLeftR);
      rHistAry_[indexLeftR] += 1.0-weightR;
      rHistAry_[indexRightR] += weightR;
    }

    // GREEN
    if ((fIndexG < 0) || (indexRightG >= bins_)) {
      gHistAry_[indexLeftG]++;
    } else {
      const float weightG   = fIndexG-static_cast<float>(indexLeftG);
      gHistAry_[indexLeftG] += 1.0-weightG;
      gHistAry_[indexRightG] += weightG;
    }

    // BLUE
    if ((fIndexB < 0) || (indexRightB >= bins_)) {
      bHistAry_[indexLeftB]++;
    } else {
      const float weightB   = fIndexB-static_cast<float>(indexLeftB);
      bHistAry_[indexLeftB] += 1.0-weightB;
      bHistAry_[indexRightB] += weightB;
    }

    count_++;
    return true;


  }

  // specialization for rgbaPixel
  inline bool 
  interpolatedHistogramAccumulator<rgbaPixel>
  ::accumulate(const rgbaPixel& value,
               const float& weight,
               const float&,
               const float&) {    

    ubyte ru,gu,bu;
    value.get(ru,gu,bu);
    const float r = static_cast<float>(ru);
    const float g = static_cast<float>(gu);
    const float b = static_cast<float>(bu);

    // value out of bounds
    if ((r < minValue_) || (r >= maxValue_)) return false;
    if ((g < minValue_) || (g >= maxValue_)) return false;
    if ((b < minValue_) || (b >= maxValue_)) return false;    
    
    // if a value is in the center of a bin it is only accumulated to one bin 
    // offset_ = -minValue_-binWidth_/2;
    const float fIndexR   = (static_cast<float>(r)+offset_) / binWidth_;
    const int indexLeftR  = static_cast<int>(fIndexR);
    const int indexRightR = indexLeftR +1;

    const float fIndexG   = (static_cast<float>(g)+offset_) / binWidth_;
    const int indexLeftG  = static_cast<int>(fIndexG);
    const int indexRightG = indexLeftG +1;

    const float fIndexB   = (static_cast<float>(b)+offset_) / binWidth_;
    const int indexLeftB  = static_cast<int>(fIndexB);
    const int indexRightB = indexLeftB +1;

    // if a value is at the bounds of the histogram the 
    // only one bin is increased by one
    // RED
    if ((fIndexR < 0) || (indexRightR >= bins_)) {
      rHistAry_[indexLeftR] += weight;
    } else {
      const float weightR   = (fIndexR-static_cast<float>(indexLeftR))*weight;
      rHistAry_[indexLeftR] += weight-weightR;
      rHistAry_[indexRightR] += weightR;
    }
    // GREEN
    if ((fIndexG < 0) || (indexRightG >= bins_)) {
      gHistAry_[indexLeftG] += weight;
    } else {
      const float weightG   = (fIndexG-static_cast<float>(indexLeftG))*weight;
      gHistAry_[indexLeftG] += weight-weightG;
      gHistAry_[indexRightG] += weightG;
    }
    // BLUE
    if ((fIndexB < 0) || (indexRightB >= bins_)) {
      bHistAry_[indexLeftB] += weight;
    } else {
      const float weightB   = (fIndexB-static_cast<float>(indexLeftB))*weight;
      bHistAry_[indexLeftB] += weight-weightB;
      bHistAry_[indexRightB] += weightB;
    }

    count_ += weight;
    return true;

  }
}

