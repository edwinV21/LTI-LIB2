/*
 * Copyright (C) 1998 - 2005
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
 * \file   ltiGenericInterpolation_template.h
 *         Contains the functor genericInterpolation which uses LUT
 *         for interpolation kernels
 * \author Pablo Alvarado
 * \date   12.06.2001
 *
 * $Id: ltiGenericInterpolation_template.h,v 1.8 2012-11-17 14:49:04 alvarado Exp $
 */

#include "ltiRound.h"

namespace lti {

  // --------------------------------------------------
  // genericInterpolation::parameters
  // --------------------------------------------------

  // default constructor
  template <typename T>
  genericInterpolation<T>::parameters::parameters() 
    : fixedGridInterpolation<T>::parameters() {
    kernelType = BicubicKernel;
    numSamplesPerInterval = 256;
    this->boundaryType = Constant;
    kernelLUT.clear();
  }

  // constructor
  template <typename T>
  genericInterpolation<T>::parameters::parameters(const eBoundaryType btype) 
    : fixedGridInterpolation<T>::parameters(btype) {
    kernelType = BicubicKernel;
    numSamplesPerInterval = 256;
    kernelLUT.clear();
  }

  // copy constructor
  template <typename T>
  genericInterpolation<T>::parameters::parameters(const parameters& other) 
    : fixedGridInterpolation<T>::parameters() {
    copy(other);
  }

  // destructor
  template <typename T>
  genericInterpolation<T>::parameters::~parameters() {
    kernelLUT.clear();
  }

  // copy member function
  template <typename T>
  typename genericInterpolation<T>::parameters& 
  genericInterpolation<T>::parameters::copy(const parameters& other) {
    fixedGridInterpolation<T>::parameters::copy(other);
    
    kernelType = other.kernelType;
    numSamplesPerInterval = other.numSamplesPerInterval;
    kernelLUT.copy(other.kernelLUT);
    return *this;
  }

  // alias for copy member function
  template <typename T>
  typename genericInterpolation<T>::parameters& 
  genericInterpolation<T>::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  // return class name
  template<typename T>
  const std::string& genericInterpolation<T>::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
      }

  // clone member function
  template <typename T>
  typename genericInterpolation<T>::parameters*
  genericInterpolation<T>::parameters::clone() const {
    return new parameters(*this);
  }

  // newInstance member function
  template <typename T>
  typename genericInterpolation<T>::parameters*
  genericInterpolation<T>::parameters::newInstance() const {
    return new parameters();
  }

  // write parameters
  template <typename T>
  bool 
  genericInterpolation<T>::parameters::write(ioHandler& handler,
                                             const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }
    
    if (b) {
      switch (kernelType) {
        case BilinearKernel:
          lti::write(handler,"kernelType","BilinearKernel");
          break;
        case BicubicKernel:
          lti::write(handler,"kernelType","BicubicKernel");
          break;
        case GenericKernel:
          lti::write(handler,"kernelType","GenericKernel");
          break;
        default:
          lti::write(handler,"kernelType","Unknown");
          b=false;
      }
      
      lti::write(handler,"numSamplesPerInterval",numSamplesPerInterval);
      lti::write(handler,"kernelLUT",kernelLUT);
    }
    
    b = b && fixedGridInterpolation<T>::parameters::write(handler, false);
    
    if (complete) {
      b = b && handler.writeEnd();
    }
    
    return b;
  }

  template <typename T>
  bool 
  genericInterpolation<T>::parameters::read(ioHandler& handler,
                                            const bool complete) {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }
    
    if (b) {
      std::string str;
      
      lti::read(handler,"kernelType",str);
      if (str.find("ilinear") != std::string::npos) {
        kernelType = BilinearKernel;
      } else if (str.find("icubic") != std::string::npos) {
        kernelType = BicubicKernel;            
      } else if (str.find("eneric") != std::string::npos) {
        kernelType = GenericKernel;
      } else {
        handler.setStatusString("Unknown interpolation kernel type");
        b=false;
      }
      
      lti::read(handler,"numSamplesPerInterval",numSamplesPerInterval);
      lti::read(handler,"kernelLUT",kernelLUT);
    }
    
    b = b && fixedGridInterpolation<T>::parameters::read(handler,false);
    
    if (complete) {
      b = b && handler.readEnd();
    }
    
    return b;
  }

  // --------------------------------------------------
  // genericInterpolation
  // --------------------------------------------------

  // default constructor
  template<typename T>
  genericInterpolation<T>::genericInterpolation()
    : fixedGridInterpolation<T>(),rows_(0),bufferRows_(0)  {
    parameters defaultParameters;
    this->setParameters(defaultParameters);
  }

  // default constructor
  template<typename T>
  genericInterpolation<T>::genericInterpolation(const eBoundaryType btyp)
    : fixedGridInterpolation<T>(),rows_(0),bufferRows_(0) {
    parameters defaultParameters(btyp);
    this->setParameters(defaultParameters);
  }

  // default constructor
  template<typename T>
  genericInterpolation<T>::genericInterpolation(const parameters& params)
    : fixedGridInterpolation<T>(),rows_(0),bufferRows_(0) {
    this->setParameters(params);
  }

  // copy constructor
  template<typename T>
  genericInterpolation<T>::genericInterpolation(const 
                                                genericInterpolation& other)
    : fixedGridInterpolation<T>(),rows_(0),bufferRows_(0) {
    copy(other);
  }

  // destructor
  template<typename T>
  genericInterpolation<T>::~genericInterpolation() {
    delete[] rows_;
    rows_=0;
    delete[] bufferRows_;
    bufferRows_=0;
  }

  // copy member
  template<typename T>
  genericInterpolation<T>&
  genericInterpolation<T>::copy(const genericInterpolation<T>& other) {
    fixedGridInterpolation<T>::copy(other);
    return (*this);
  }

  // alias for copy member
  template<typename T>
  genericInterpolation<T>&
  genericInterpolation<T>::operator=(const genericInterpolation<T>& other) {
    return (copy(other));
  }

  // class name
  template<typename T>
  const std::string& genericInterpolation<T>::name() const {
    _LTI_RETURN_CLASS_NAME
      }

  // clone member
  template<typename T>
  genericInterpolation<T>* genericInterpolation<T>::clone() const {
    return new genericInterpolation(*this);
  }

  // newInstance member
  template<typename T>
  genericInterpolation<T>* genericInterpolation<T>::newInstance() const {
    return new genericInterpolation();
  }

  // return parameters
  template<typename T>
  const typename genericInterpolation<T>::parameters&
  genericInterpolation<T>::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }

  // return range of influence
  template<typename T>
  int genericInterpolation<T>::getRangeOfInfluence() const {
    return rangeOfInfluence_;
  }


  template<typename T>
  bool 
  genericInterpolation<T>::updateParameters() {
    // update rangeOfInfluence
    const parameters& param = getParameters();
    switch (param.kernelType) {
      case BilinearKernel:
        rangeOfInfluence_=1;
        break;
      case BicubicKernel:
        rangeOfInfluence_=2;
        break;
      case GenericKernel:
        rangeOfInfluence_=99;
        break;
      default:
        rangeOfInfluence_=99;
    }
      

    return initLUT();
  }

  template<typename T>
  bool genericInterpolation<T>::initLUT() {

    const parameters& param = getParameters();
    intervalSize_ = param.numSamplesPerInterval;
    bool result = false;
    // initialize the LUT
    // each method must set the "numberOfIntervals" attribute..
    switch (param.kernelType) {
      case BilinearKernel:
        result = initBilinearLUT();
        break;
      case BicubicKernel:
        result = initBicubicLUT();
        break;
      case GenericKernel:
        result = initGenericLUT();
      default:
        initBilinearLUT();
    }

    // initialize the buffers for 2D interpolation
    column_.assign(numberOfIntervals_,T(0));
    buffer_.assign(numberOfIntervals_,numberOfIntervals_,T(0));
    firstElemColumn_ = column_.data();
       
    delete[] rows_;
    rows_=0;
    delete[] bufferRows_;
    bufferRows_=0;

    rows_ = new const T*[numberOfIntervals_];
    bufferRows_ = new const T*[numberOfIntervals_];
    int i;
    for (i=0;i<numberOfIntervals_;++i) {
      rows_[i]=0;
      bufferRows_[i]=&buffer_.at(i,0);
    }

    // initialize some constants
    firstSampleOffset_ = numberOfIntervals_/2 - 1;

    return result;
  }


  template<typename T>
  bool genericInterpolation<T>::initBilinearLUT() {
    numberOfIntervals_ = 2;
    lastIntervalFirstSample_ = intervalSize_;
    interpolationKernel_.allocate(intervalSize_*2);

    const float m = -1.0f/interpolationKernel_.size();
    const int center = intervalSize_;
    int i;
    interpolationKernel_.at(0)=0.0f;
    interpolationKernel_.at(center)=1.0f;
    for (i=1;i<intervalSize_;++i) {
      interpolationKernel_.at(center-i) =
        interpolationKernel_.at(center+i) = 1.0f-m*static_cast<float>(i);
    }

    return true;
  }

  template<typename T>
  bool genericInterpolation<T>::initBicubicLUT() {
    numberOfIntervals_ = 4;
    interpolationKernel_.allocate(intervalSize_*4);
    lastIntervalFirstSample_ = 3*intervalSize_;

    const int center = 2*intervalSize_;

    int i;
    float x;
    interpolationKernel_.at(0)=0.0f;
    interpolationKernel_.at(center)=1.0f;

    for (i=1;i<intervalSize_;++i) {
      x = static_cast<float>(i)/static_cast<float>(intervalSize_);
      interpolationKernel_.at(center-i) = 
        interpolationKernel_.at(center+i) = 1.0f+x*x*(x-2.0f);
    }
    
    for (;i<2*intervalSize_;++i) {
      x = static_cast<float>(i)/static_cast<float>(intervalSize_);
      interpolationKernel_.at(center-i) = 
        interpolationKernel_.at(center+i)=4.0f+(x*(x*(5-x)-8.0f));
    }

    return true;
  }

  template<typename T>
  bool genericInterpolation<T>::initGenericLUT() {
    const parameters& par = getParameters();
    numberOfIntervals_ = iround(par.kernelLUT.size()/intervalSize_);
    if ((numberOfIntervals_*intervalSize_ != par.kernelLUT.size()) ||
        odd(numberOfIntervals_)) {
      // this is an invalid kernel
      initBilinearLUT();
      return false;
    } else {
      interpolationKernel_.copy(par.kernelLUT);
      lastIntervalFirstSample_ = (numberOfIntervals_-1)*intervalSize_;
    }
    return true;
  }

  // -------------------------------------------------------------------
  // The apply-methods!
  // -------------------------------------------------------------------


  /*
   * Returns the interpolated value of the vector at the real valued
   * position x.
   * @param src vector<T> with the source data.
   * @param x the real valued position to be interpolated.
   * @return the interpolated value of the vector.
   */
  template<typename T>
  T genericInterpolation<T>::apply(const vector<T>& src,
                                   const float& x) const {

    int truncX = static_cast<int>(x);
    float fractX = x - static_cast<float>(truncX);
    truncX-=firstSampleOffset_;

    // the interpolation:    
    if ((truncX >= 0) && (truncX <= (src.size()-numberOfIntervals_))) {
      return compute(fractX,&src[truncX]);
    } else {
      int i;
      switch (getParameters().boundaryType) {
        case lti::NoBoundary: {
          return T();
        } 
        
        case lti::Zero: {
          for (i=0;i<numberOfIntervals_;++i) {
            firstElemColumn_[i] = this->zeroAt(src,truncX+i);
          }
        } break;
        case lti::Constant: {
          for (i=0;i<numberOfIntervals_;++i) {
            firstElemColumn_[i] = this->cstAt(src,truncX+i);
          }
        } break;
        case lti::Mirror: {
          for (i=0;i<numberOfIntervals_;++i) {
            firstElemColumn_[i] = this->mirrAt(src,truncX+i);
          }
        } break;
        case lti::Periodic: {
          for (i=0;i<numberOfIntervals_;++i) {
            firstElemColumn_[i] = this->periAt(src,truncX+i);
          }
        } break;
        default:
          return T();
      }
      return compute(fractX,firstElemColumn_);
    }


  }

  /*
   * Returns the interpolated value of the vector specified with
   * use() at the real valued position x.
   * @param x the real valued position to be interpolated.
   * @return the interpolated value of the vector.  */
  template<typename T>
  T genericInterpolation<T>::apply(const float& x) const {
    assert(notNull(this->theVector_));
    return apply(*this->theVector_,x);
  }

  /*
   * Returns the interpolated value of the matrix at the real valued
   * position (row,col).
   *
   * @param src matrix<T> with the source data.
   * @param row which row
   * @param col which column
   * @return the interpolated value of the matrix.
   */
  template<typename T>
  T genericInterpolation<T>::apply(const matrix<T>& src,
                                   const float& row,
                                   const float& col) const {

    return interpolate(src,row,col);
  }

  template<typename T>
  T genericInterpolation<T>::apply(const matrix<T>& src,
                                   const fpoint& p) const {
    return interpolate(src,p.y,p.x);
  }

  /*
   * Returns the interpolated value of the matrix specified with
   * use() at the real valued position (row,col).
   *
   * @param row which row
   * @param col which column
   * @return the interpolated value of the matrix.
   */
  template<typename T>
  T genericInterpolation<T>::apply(const float& row,
                                   const float& col) const {
    assert(notNull(this->theMatrix_));
    return interpolate(*this->theMatrix_,row,col);
  }

  template<typename T>
  T genericInterpolation<T>::apply(const fpoint& p) const {
    assert(notNull(this->theMatrix_));
    return interpolate(*this->theMatrix_,p.y,p.x);
  }

  /*
   * Returns the interpolated value of the matrix at the real valued
   * position (row,col).
   *
   * @param src matrix<T> with the source data.
   * @param row which row
   * @param col which column
   * @return the interpolated value of the matrix.
   */
  template<typename T>
  T genericInterpolation<T>::interpolate(const matrix<T>& src,
                                         const float row,
                                         const float col) const {
    // precomputations for x
    int truncX = static_cast<int>(col);
    float fractX = col - static_cast<float>(truncX);
    truncX-=firstSampleOffset_;

    // precomputation for y
    int truncY = static_cast<int>(row);
    float fractY = row - static_cast<float>(truncY);
    truncY-=firstSampleOffset_;
    int i,j;

    if ((truncX >= 0) && (truncX <= (src.columns()-numberOfIntervals_)) &&
        (truncY >= 0) && (truncY <= (src.rows()-numberOfIntervals_))) {
      for (i=0;i<numberOfIntervals_;++i) {
        rows_[i]=&src.at(truncY+i,truncX);
      }
      return compute(fractY,fractX,rows_);
    } else {
      switch(getParameters().boundaryType) {
        case lti::NoBoundary: {
          return T();
        } 
        case lti::Zero: {         
          for (j=0;j<numberOfIntervals_;++j) {
            for (i=0;i<numberOfIntervals_;++i) {
              buffer_.at(j,i)=this->zeroAt(src,truncY+j,truncX+i);
            }
          }
        } break;
        case lti::Constant: {
          for (j=0;j<numberOfIntervals_;++j) {
            for (i=0;i<numberOfIntervals_;++i) {
              buffer_.at(j,i)=this->cstAt(src,truncY+j,truncX+i);
            }
          }
        } break;
        case lti::Mirror: {
          for (j=0;j<numberOfIntervals_;++j) {
            for (i=0;i<numberOfIntervals_;++i) {
              buffer_.at(j,i)=this->mirrAt(src,truncY+j,truncX+i);
            }
          }
        } break;
        case lti::Periodic: {
          for (j=0;j<numberOfIntervals_;++j) {
            for (i=0;i<numberOfIntervals_;++i) {
              buffer_.at(j,i)=this->periAt(src,truncY+j,truncX+i);
            }
          }
        } break;
        default:
          return T();
      }
      return compute(fractY,fractX,bufferRows_);

    }


  }

}

