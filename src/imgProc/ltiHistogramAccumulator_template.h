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
 * \file   ltiHistogramAccumulator_template.h
 *         Contains the template class histogramAccumulator<T>
 * \author Arnd Hannemann
 * \date   24.02.2005
 *
 * revisions ..: $Id: ltiHistogramAccumulator_template.h,v 1.5 2006-06-06 11:34:04 doerfler Exp $
 */

namespace lti {

  // --------------------------------------------------
  // histogramAccumulator
  // --------------------------------------------------


  // default constructor
  template <typename T>
  histogramAccumulator<T>::histogramAccumulator(const int bins,
                                                const T minValue,
                                                const T maxValue) {

    bins_ = bins;
    minValue_ = minValue;
    maxValue_ = maxValue;
    histAry_  = new double[bins];
    binWidth_ = static_cast<double>(maxValue-minValue) / bins;
    reset();
  }

  
  // specialization for rgbaPixel
  histogramAccumulator<rgbaPixel>::
  histogramAccumulator(const int bins,
                       const rgbaPixel::value_type minValue,
                       const rgbaPixel::value_type maxValue) {
    bins_ = bins;
    minValue_ = minValue;
    maxValue_ = maxValue;
    rHistAry_ = new double[bins];
    gHistAry_ = new double[bins];
    bHistAry_ = new double[bins];
    binWidth_ = static_cast<double>(maxValue-minValue) / bins;
    reset();
  }

  // copy constructor
  template <typename T>
  histogramAccumulator<T>::
  histogramAccumulator(const histogramAccumulator<T>& other) {
    bins_ = 0;
    histAry_ = 0;
    copy(other);
  }

  // copy constructor - specialization for rgbaPixel
  histogramAccumulator<rgbaPixel>::
  histogramAccumulator(const histogramAccumulator<rgbaPixel>& other) {
    rHistAry_ = 0;
    gHistAry_ = 0;
    bHistAry_ = 0;
    bins_ = 0;
    copy(other);
  }

  // Destructor
  template <typename T>
  inline histogramAccumulator<T>::~histogramAccumulator() {
    delete[] histAry_;
    histAry_ = 0;
  }

  // specialization for rgbaPixel
  histogramAccumulator<rgbaPixel>::~histogramAccumulator() {
    delete[] rHistAry_;
    delete[] gHistAry_;
    delete[] bHistAry_;
    rHistAry_ = gHistAry_ = bHistAry_ = 0;
  }
  
  // copy
  template <typename T>
  histogramAccumulator<T>&
  histogramAccumulator<T>::copy(const histogramAccumulator<T>& other){
    // copy histogram array
    if (bins_ != other.bins_) {
      bins_ = other.bins_;
      delete[] histAry_;
      histAry_ = new double[bins_];
    }
    memcpy(histAry_,other.histAry_,bins_*sizeof(double));

    minValue_ = other.minValue_;
    maxValue_ = other.maxValue_;
    binWidth_ = other.binWidth_;
    count_    = other.count_;
    
    return *this;
  }

  // copy - specialization for rgbaPixel
  const histogramAccumulator<rgbaPixel>&
  histogramAccumulator<rgbaPixel>::copy(const histogramAccumulator<rgbaPixel>& other){
    // copy histogram array
    if (bins_ != other.bins_) {
      bins_ = other.bins_;
      delete[] rHistAry_;
      delete[] gHistAry_;
      delete[] bHistAry_;
      rHistAry_ = new double[bins_];
      gHistAry_ = new double[bins_];
      bHistAry_ = new double[bins_];
    }
    memcpy(rHistAry_,other.rHistAry_,bins_*sizeof(double));
    memcpy(gHistAry_,other.gHistAry_,bins_*sizeof(double));
    memcpy(bHistAry_,other.bHistAry_,bins_*sizeof(double));
    minValue_ = other.minValue_;
    maxValue_ = other.maxValue_;
    binWidth_ = other.binWidth_;
    count_ = other.count_;
    return *this;
  }


  // reset
  template <typename T>
  inline bool histogramAccumulator<T>::reset() {
    count_ =  0;
    int i;
    for (i=0; i<bins_;i++) {
      histAry_[i] = 0;
    }
    return true;
  }
  
  // specialization for rgbaPixel
  inline bool histogramAccumulator<rgbaPixel>::reset() {
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
  inline int histogramAccumulator<T>::getArraySize() const{
    return bins_;
  }

  inline int histogramAccumulator<rgbaPixel>::getArraySize() const{
    return bins_*3;
  } 


  template <typename T>
  inline bool histogramAccumulator<T>::getResult(double result[]) const{
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
  inline bool histogramAccumulator<rgbaPixel>::getResult(double result[]) const {

    
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
  inline bool histogramAccumulator<T>::accumulate(const T& value,
                                                   const float&,
                                                   const float&) {
    const int index = static_cast<int> ((value-minValue_) / binWidth_);
    // value out of bounds
    if ((index < 0) || (index >= bins_)) return false;

    histAry_[index]++;
    count_++;
    return true;
  }

  // accumulate
  template <typename T>
  inline bool histogramAccumulator<T>::accumulate(const T& value,
                                                   const float& weight,
                                                   const float&,
                                                   const float&) {
    const int index = static_cast<int> ((value-minValue_) / binWidth_);
    // value out of bounds
    if ((index < 0) || (index >= bins_)) return false;

    histAry_[index] += weight;
    count_ += weight;

    return true;
  }

  // specialization for rgbaPixel
  inline bool 
  histogramAccumulator<rgbaPixel>::accumulate(const rgbaPixel& value,
                                              const float&,
                                              const float&) {    
    ubyte r,g,b;
    value.get(r,g,b);
    
    const int rIndex = static_cast<int>(((r-minValue_) / binWidth_));
    const int gIndex = static_cast<int>(((g-minValue_) / binWidth_));
    const int bIndex = static_cast<int>(((b-minValue_) / binWidth_));

    // value out of bounds
    if ((rIndex < 0) || (rIndex >= bins_)) return false;
    if ((gIndex < 0) || (gIndex >= bins_)) return false;
    if ((bIndex < 0) || (bIndex >= bins_)) return false;

    rHistAry_[rIndex]++;
    gHistAry_[gIndex]++;
    bHistAry_[bIndex]++;
    count_++;
    return true;
  }

  // specialization for rgbaPixel
  inline bool 
  histogramAccumulator<rgbaPixel>::accumulate(const rgbaPixel& value,
                                              const float& weight,
                                              const float&,
                                              const float&) {    

    ubyte r,g,b;
    value.get(r,g,b);
    
    const int rIndex = static_cast<int>(((r-minValue_) / binWidth_));
    const int gIndex = static_cast<int>(((g-minValue_) / binWidth_));
    const int bIndex = static_cast<int>(((b-minValue_) / binWidth_));

    // value out of bounds
    if ((rIndex < 0) || (rIndex >= bins_)) return false;
    if ((gIndex < 0) || (gIndex >= bins_)) return false;
    if ((bIndex < 0) || (bIndex >= bins_)) return false;

    rHistAry_[rIndex] += weight;
    gHistAry_[gIndex] += weight;
    bHistAry_[bIndex] += weight;

    count_+= weight;
    return true;
  }
}


