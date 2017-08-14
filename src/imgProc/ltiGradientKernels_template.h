/*
 * Copyright (C) 1998 - 2004
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
 * \file  ltiGradientKernels.h
 *        This file contains the definitions of linear kernels that can be
 *        used for calculating the gradient of a channel or other matrix type
 * \author Pablo Alvarado
 * \date 11.07.2000
 *
 * $Id: ltiGradientKernels_template.h,v 1.5 2012-11-17 14:49:04 alvarado Exp $
 */

#include "ltiMath.h"

namespace lti {
  // ANDO

  // constructor
  template<typename T>
    andoKernelX<T>::andoKernelX(const int size)
    : separableKernel<T>() {
    generate(size);
  }

  // copy constructor
  template<class T>
  andoKernelX<T>::andoKernelX(const andoKernelX<T>& other) 
    : separableKernel<T>() {
    this->copy(other);
  }

  // clone
  template<class T> 
  andoKernelX<T>* andoKernelX<T>::clone() const {
      return (new andoKernelX<T>(*this));
  }
  
  // newInstance
  template<class T> 
  andoKernelX<T>* andoKernelX<T>::newInstance() const {
      return (new andoKernelX<T>());
  }

  // typename
  template<class T> 
  const std::string& andoKernelX<T>::name() const {
      _LTI_RETURN_CLASS_NAME
  }

  // generate filter
  template<typename T>
  void  andoKernelX<T>::generate(const int size) {

    this->setNumberOfPairs(1);

    if ((size>=3) && (size<=5)) {
      int i,l,h;
      l = -size/2;
      h = l+size-1;

      this->getRowFilter(0).allocate(l,h);
      this->getColFilter(0).allocate(l,h);

      const T rowNorm = this->getRowFilter(0).getNorm();
      const T colNorm = this->getColFilter(0).getNorm();

      static const double k3row[3] = {1.000000,0.000000,-1.000000};
      static const double k3col[3] = { 0.112737,0.274526,0.112737};

      static const double k4row[4] =
        {+0.870541 , +1.000000,  -1.000000, -0.870541};
      static const double k4col[4] =
        { 0.0254738,  0.112996,  0.112996, 0.0254738};

      static const double k5row[5] =
        {+0.379022,+1.000000, 0.000000, -1.000000, -0.379022};
      static const double k5col[5] =
        { 0.0101693, 0.0708223, 0.122602, 0.0708223, 0.0101693};

      switch(size) {
        case 3:
          for (i=0;i<size;i++) {
            this->getRowFilter(0).at(i-1) = static_cast<T>(k3row[i])*rowNorm;
            this->getColFilter(0).at(i-1) = static_cast<T>(k3col[i])*colNorm;
          }
          break;
        case 4:
          for (i=0;i<size;i++) {
            this->getRowFilter(0).at(i-2) = static_cast<T>(k4row[i])*rowNorm;
            this->getColFilter(0).at(i-2) = static_cast<T>(k4col[i])*colNorm;
          }
          break;
        case 5:
          for (i=0;i<size;i++) {
            this->getRowFilter(0).at(i-2) = static_cast<T>(k5row[i])*rowNorm;
            this->getColFilter(0).at(i-2) = static_cast<T>(k5col[i])*colNorm;
          }
          break;
      }
    }
  }

  // constructor
  template<typename T>
    andoKernelY<T>::andoKernelY(const int size)
    : separableKernel<T>() {
    generate(size);
  }

  // copy constructor
  template<class T>
  andoKernelY<T>::andoKernelY(const andoKernelY<T>& other) 
    : separableKernel<T>() {
    this->copy(other);
  }

  // clone
  template<class T> 
  andoKernelY<T>* andoKernelY<T>::clone() const {
    return (new andoKernelY<T>(*this));
  }
  
  // newInstance
  template<class T> 
  andoKernelY<T>* andoKernelY<T>::newInstance() const {
    return (new andoKernelY<T>());
  }

  // typename
  template<class T> 
  const std::string& andoKernelY<T>::name() const {
      _LTI_RETURN_CLASS_NAME
  }

  // generate filter
  template<typename T>
    void  andoKernelY<T>::generate(const int size) {

    this->setNumberOfPairs(1);

    if ((size>=3) && (size<=5)) {
      int i,l,h;
      l = -size/2;
      h = l+size-1;

      this->getRowFilter(0).allocate(l,h);
      this->getColFilter(0).allocate(l,h);

      const T rowNorm = this->getRowFilter(0).getNorm();
      const T colNorm = this->getColFilter(0).getNorm();

      static const double k3col[3] = {+1.000000,0.000000,-1.000000};
      static const double k3row[3] = { 0.112737,0.274526,0.112737};

      static const double k4col[4] =
        {+0.870541 , +1.000000,  -1.000000, -0.870541};
      static const double k4row[4] =
        { 0.0254738,  0.112996,  0.112996, 0.0254738};

      static const double k5col[5] =
        {+0.379022,+1.000000, 0.000000, -1.000000, -0.379022};
      static const double k5row[5] =
        { 0.0101693, 0.0708223, 0.122602, 0.0708223, 0.0101693};

      switch(size) {
        case 3:
          for (i=0;i<size;i++) {
            this->getRowFilter(0).at(i-1) = static_cast<T>(k3row[i])*rowNorm;
            this->getColFilter(0).at(i-1) = static_cast<T>(k3col[i])*colNorm;
          }
          break;
        case 4:
          for (i=0;i<size;i++) {
            this->getRowFilter(0).at(i-2) = static_cast<T>(k4row[i])*rowNorm;
            this->getColFilter(0).at(i-2) = static_cast<T>(k4col[i])*colNorm;
          }
          break;
        case 5:
          for (i=0;i<size;i++) {
            this->getRowFilter(0).at(i-2) = static_cast<T>(k5row[i])*rowNorm;
            this->getColFilter(0).at(i-2) = static_cast<T>(k5col[i])*colNorm;
          }
          break;
      }
    }
  }

  // -----------------------------
  // SOBEL
  
  template<typename T>
  sobelKernelX<T>::sobelKernelX(const bool normalized) : separableKernel<T>() {
    this->setNumberOfPairs(1);

    // row filter -1 0 1
    this->getRowFilter(0).assign(-1,1,static_cast<T>(0));
    this->getRowFilter(0).at(-1) = -1;
    this->getRowFilter(0).at( 1) =  1;

    // col filter 1 2 1
    this->getColFilter(0).assign(-1,1,static_cast<T>(1));
    this->getColFilter(0).at(0)=2;

    if (normalized) {
      if (std::numeric_limits<T>::is_integer) {
        this->getRowFilter(0).setNorm(2);
        this->getColFilter(0).setNorm(2);
      } else {
        this->getRowFilter(0).divide(static_cast<T>(2));
        this->getColFilter(0).divide(static_cast<T>(2));
      }
    }

  }

  // copy constructor
  template<class T>
  sobelKernelX<T>::sobelKernelX(const sobelKernelX<T>& other) 
    : separableKernel<T>() {
    this->copy(other);
  }

  // clone
  template<class T> 
  sobelKernelX<T>* sobelKernelX<T>::clone() const {
      return (new sobelKernelX<T>(*this));
  }
  
  // newInstance
  template<class T> 
  sobelKernelX<T>* sobelKernelX<T>::newInstance() const {
      return (new sobelKernelX<T>());
  }

  // typename
  template<class T> 
  const std::string& sobelKernelX<T>::name() const {
      _LTI_RETURN_CLASS_NAME
  }
  
  template<typename T>
  sobelKernelY<T>::sobelKernelY(const bool normalized) : separableKernel<T>() {
    this->setNumberOfPairs(1);

    // col filter -1 0 1
    this->getColFilter(0).assign(-1,1,static_cast<T>(0));
    this->getColFilter(0).at(-1) = -1;
    this->getColFilter(0).at( 1) =  1;

    // row filter 1 2 1
    this->getRowFilter(0).assign(-1,1,static_cast<T>(1));
    this->getRowFilter(0).at(0)=2;

    if (normalized) {
      if (std::numeric_limits<T>::is_integer) {
        this->getRowFilter(0).setNorm(2);
        this->getColFilter(0).setNorm(2);
      } else {
        this->getRowFilter(0).divide(static_cast<T>(2));
        this->getColFilter(0).divide(static_cast<T>(2));
      }
    }

  }

  // copy constructor
  template<class T>
  sobelKernelY<T>::sobelKernelY(const sobelKernelY<T>& other) 
    : separableKernel<T>() {
    this->copy(other);
  }

  // clone
  template<class T> 
  sobelKernelY<T>* sobelKernelY<T>::clone() const {
      return (new sobelKernelY<T>(*this));
  }
  
  // newInstance
  template<class T> 
  sobelKernelY<T>* sobelKernelY<T>::newInstance() const {
      return (new sobelKernelY<T>());
  }

  // typename
  template<class T> 
  const std::string& sobelKernelY<T>::name() const {
      _LTI_RETURN_CLASS_NAME
  }

  // -----------------------------
  // PREWITT
  
  template<typename T>
  prewittKernelX<T>::prewittKernelX(const bool normalized) : separableKernel<T>() {
    this->setNumberOfPairs(1);

    // row filter -1 0 1
    this->getRowFilter(0).assign(-1,1,static_cast<T>(0));
    this->getRowFilter(0).at(-1) = -1;
    this->getRowFilter(0).at( 1) =  1;

    // col filter 1 1 1
    this->getColFilter(0).assign(-1,1,static_cast<T>(1));

    if (normalized) {
      if (std::numeric_limits<T>::is_integer) {
        this->getColFilter(0).setNorm(3);
      } else {
        this->getColFilter(0).divide(static_cast<T>(3));
      }
    }
  }

  // copy constructor
  template<class T>
  prewittKernelX<T>::prewittKernelX(const prewittKernelX<T>& other) 
    : separableKernel<T>() {
    this->copy(other);
  }

  // clone
  template<class T> 
  prewittKernelX<T>* prewittKernelX<T>::clone() const {
      return (new prewittKernelX<T>(*this));
  }
  
  // newInstance
  template<class T> 
  prewittKernelX<T>* prewittKernelX<T>::newInstance() const {
      return (new prewittKernelX<T>());
  }

  // typename
  template<class T> 
  const std::string& prewittKernelX<T>::name() const {
      _LTI_RETURN_CLASS_NAME
  }
  
  template<typename T>
  prewittKernelY<T>::prewittKernelY(const bool normalized) : separableKernel<T>() {
    this->setNumberOfPairs(1);

    // col filter -1 0 1
    this->getColFilter(0).assign(-1,1,static_cast<T>(0));
    this->getColFilter(0).at(-1) = -1;
    this->getColFilter(0).at( 1) =  1;

    // row filter 1 1 1
    this->getRowFilter(0).assign(-1,1,static_cast<T>(1));

    if (normalized) {
      if (std::numeric_limits<T>::is_integer) {
        this->getRowFilter(0).setNorm(3);
      } else {
        this->getRowFilter(0).divide(static_cast<T>(3));
      }
    }
  }

  // copy constructor
  template<class T>
  prewittKernelY<T>::prewittKernelY(const prewittKernelY<T>& other) 
    : separableKernel<T>() {
    this->copy(other);
  }

  // clone
  template<class T> 
  prewittKernelY<T>* prewittKernelY<T>::clone() const {
      return (new prewittKernelY<T>(*this));
  }
  
  // newInstance
  template<class T> 
  prewittKernelY<T>* prewittKernelY<T>::newInstance() const {
      return (new prewittKernelY<T>());
  }

  // typename
  template<class T> 
  const std::string& prewittKernelY<T>::name() const {
      _LTI_RETURN_CLASS_NAME
  }

  // -----------------------------
  // HARRIS
  
  template<typename T>
  harrisKernelX<T>::harrisKernelX() : separableKernel<T>() {
    this->setNumberOfPairs(1);

    // row filter -2 -1 0 1 2
    this->getRowFilter(0).assign(-2,2,static_cast<T>(0));
    this->getRowFilter(0).at(-2) = static_cast<T>(-2);
    this->getRowFilter(0).at(-1) = static_cast<T>(-1);
    this->getRowFilter(0).at( 1) = static_cast<T>(1);
    this->getRowFilter(0).at( 2) = static_cast<T>(2);

    // col filter empty
    this->getColFilter(0).allocate(0,0);
  }

  // copy constructor
  template<class T>
  harrisKernelX<T>::harrisKernelX(const harrisKernelX<T>& other) 
    : separableKernel<T>() {
    this->copy(other);
  }

  // clone
  template<class T> 
  harrisKernelX<T>* harrisKernelX<T>::clone() const {
      return (new harrisKernelX<T>(*this));
  }
  
  // newInstance
  template<class T> 
  harrisKernelX<T>* harrisKernelX<T>::newInstance() const {
      return (new harrisKernelX<T>());
  }

  // typename
  template<class T> 
  const std::string& harrisKernelX<T>::name() const {
      _LTI_RETURN_CLASS_NAME
  }
  
  template<typename T>
  harrisKernelY<T>::harrisKernelY() : separableKernel<T>() {
    this->setNumberOfPairs(1);

    // col filter -2 -1 0 1 2
    this->getColFilter(0).assign(-2,2,static_cast<T>(0));
    this->getColFilter(0).at(-2) = static_cast<T>(-2);
    this->getColFilter(0).at(-1) = static_cast<T>(-1);
    this->getColFilter(0).at( 1) = static_cast<T>(1);
    this->getColFilter(0).at( 2) = static_cast<T>(2);

    // row filter empty
    this->getRowFilter(0).allocate(0,0);
  }

  // copy constructor
  template<class T>
  harrisKernelY<T>::harrisKernelY(const harrisKernelY<T>& other) 
    : separableKernel<T>() {
    this->copy(other);
  }

  // clone
  template<class T> 
  harrisKernelY<T>* harrisKernelY<T>::clone() const {
      return (new harrisKernelY<T>(*this));
  }
  
  // newInstance
  template<class T> 
  harrisKernelY<T>* harrisKernelY<T>::newInstance() const {
      return (new harrisKernelY<T>());
  }

  // typename
  template<class T> 
  const std::string& harrisKernelY<T>::name() const {
      _LTI_RETURN_CLASS_NAME
  }

  // ---------------------------------------
  // ROBINSON
  
  template<typename T>
  robinsonKernelX<T>::robinsonKernelX() : kernel2D<T>(-1,-1,1,1) {
    this->at(-1,-1) = -1;
    this->at(-1, 0) =  1;
    this->at(-1, 1) =  1;
    this->at( 0,-1) = -1;
    this->at( 0, 0) = -2;
    this->at( 0, 1) =  1;
    this->at( 1,-1) = -1;
    this->at( 1, 0) =  1;
    this->at( 1, 1) =  1;
  }

  // copy constructor
  template<class T>
  robinsonKernelX<T>::robinsonKernelX(const robinsonKernelX<T>& other) 
    : kernel2D<T>(other) {
  }

  // clone
  template<class T> 
  robinsonKernelX<T>* robinsonKernelX<T>::clone() const {
      return (new robinsonKernelX<T>(*this));
  }
  
  // newInstance
  template<class T> 
  robinsonKernelX<T>* robinsonKernelX<T>::newInstance() const {
      return (new robinsonKernelX<T>());
  }

  // typename
  template<class T> 
  const std::string& robinsonKernelX<T>::name() const {
      _LTI_RETURN_CLASS_NAME
  }
  
  template<typename T>
  robinsonKernelY<T>::robinsonKernelY() : kernel2D<T>(-1,-1,1,1) {
    this->at(-1,-1) = -1;
    this->at(-1, 0) = -1;
    this->at(-1, 1) = -1;
    this->at( 0,-1) =  1;
    this->at( 0, 0) = -2;
    this->at( 0, 1) =  1;
    this->at( 1,-1) =  1;
    this->at( 1, 0) =  1;
    this->at( 1, 1) =  1;
  }

  // copy constructor
  template<class T>
  robinsonKernelY<T>::robinsonKernelY(const robinsonKernelY<T>& other) 
    : kernel2D<T>(other) {
  }

  // clone
  template<class T> 
  robinsonKernelY<T>* robinsonKernelY<T>::clone() const {
      return (new robinsonKernelY<T>(*this));
  }
  
  // newInstance
  template<class T> 
  robinsonKernelY<T>* robinsonKernelY<T>::newInstance() const {
      return (new robinsonKernelY<T>());
  }

  // typename
  template<class T> 
  const std::string& robinsonKernelY<T>::name() const {
      _LTI_RETURN_CLASS_NAME
  }

  // ---------------------------------------
  // KIRSCH
  
  template<typename T>
  kirschKernelX<T>::kirschKernelX() : kernel2D<T>(-1,-1,1,1) {
    this->at(-1,-1) = -5;
    this->at(-1, 0) =  3;
    this->at(-1, 1) =  3;
    this->at( 0,-1) = -5;
    this->at( 0, 0) =  0;
    this->at( 0, 1) =  3;
    this->at( 1,-1) = -5;
    this->at( 1, 0) =  3;
    this->at( 1, 1) =  3;
  }

  // copy constructor
  template<class T>
  kirschKernelX<T>::kirschKernelX(const kirschKernelX<T>& other) 
    : kernel2D<T>(other) {
  }

  // clone
  template<class T> 
  kirschKernelX<T>* kirschKernelX<T>::clone() const {
      return (new kirschKernelX<T>(*this));
  }
  
  // newInstance
  template<class T> 
  kirschKernelX<T>* kirschKernelX<T>::newInstance() const {
      return (new kirschKernelX<T>());
  }

  // typename
  template<class T> 
  const std::string& kirschKernelX<T>::name() const {
      _LTI_RETURN_CLASS_NAME
  }
  
  template<typename T>
  kirschKernelY<T>::kirschKernelY() : kernel2D<T>(-1,-1,1,1) {
    this->at(-1,-1) = -5;
    this->at(-1, 0) = -5;
    this->at(-1, 1) = -5;
    this->at( 0,-1) =  3;
    this->at( 0, 0) =  0;
    this->at( 0, 1) =  3;
    this->at( 1,-1) =  3;
    this->at( 1, 0) =  3;
    this->at( 1, 1) =  3;
  }

  // copy constructor
  template<class T>
  kirschKernelY<T>::kirschKernelY(const kirschKernelY<T>& other) 
    : kernel2D<T>(other) {
  }

  // clone
  template<class T> 
  kirschKernelY<T>* kirschKernelY<T>::clone() const {
      return (new kirschKernelY<T>(*this));
  }
  
  // newInstance
  template<class T> 
  kirschKernelY<T>* kirschKernelY<T>::newInstance() const {
      return (new kirschKernelY<T>());
  }

  // typename
  template<class T> 
  const std::string& kirschKernelY<T>::name() const {
      _LTI_RETURN_CLASS_NAME
  }

}

