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
 * \file   ltiBiquadraticInterpolation_template.h
 *         The file contains the functor biquadraticInterpolation.
 * \author Pablo Alvarado
 * \date   12.06.2001
 *
 * $Id: ltiBiquadraticInterpolation_template.h,v 1.6 2012-11-17 14:49:04 alvarado Exp $
 */

namespace lti {

  // --------------------------------------------------
  // biquadraticInterpolation
  // --------------------------------------------------

  // default constructor
  template<class T>
  biquadraticInterpolation<T>::biquadraticInterpolation()
    : fixedGridInterpolation<T>() {
    parameters defaultParameters;
    this->setParameters(defaultParameters);
  }

  // default constructor
  template<class T>
  biquadraticInterpolation<T>::biquadraticInterpolation(const eBoundaryType btyp)
    : fixedGridInterpolation<T>() {
    parameters defaultParameters(btyp);
    this->setParameters(defaultParameters);
  }

  // copy constructor
  template<class T>
  biquadraticInterpolation<T>::biquadraticInterpolation(const
                                                        biquadraticInterpolation& other)
    : fixedGridInterpolation<T>() {
    copy(other);
  }

  // destructor
  template<class T>
  biquadraticInterpolation<T>::~biquadraticInterpolation() {
  }

  // copy member
  template<class T>
  biquadraticInterpolation<T>&
  biquadraticInterpolation<T>::copy(const biquadraticInterpolation<T>& other) {
    fixedGridInterpolation<T>::copy(other);
    return (*this);
  }

  // class name
  template<typename T>
  const std::string& biquadraticInterpolation<T>::name() const {
    _LTI_RETURN_CLASS_NAME
      }

  // clone member
  template<class T>
  biquadraticInterpolation<T>*
  biquadraticInterpolation<T>::clone() const {
    return new biquadraticInterpolation(*this);
  }

  // newInstance member
  template<class T>
  biquadraticInterpolation<T>* 
  biquadraticInterpolation<T>::newInstance() const {
    return new biquadraticInterpolation();
  }

  // return parameters
  template<class T>
  const typename biquadraticInterpolation<T>::parameters&
  biquadraticInterpolation<T>::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }


  // return range of influence
  template<typename T>
  int biquadraticInterpolation<T>::getRangeOfInfluence() const {
    return 2;
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
  template<class T>
  T biquadraticInterpolation<T>::apply(const vector<T>& src,
                                       const float& x) const {
    
    int truncX = static_cast<int>(x);
    if (x<0) {
      --truncX;
    }
    float fractX = x - static_cast<float>(truncX);
    if (fractX <= 0.5f) {
      --truncX;
    } else {
      fractX-=1.0f;
    }

    // the interpolation:    
    if ((truncX >= 0) && (truncX < (src.size()-2))) {
      return compute(fractX,src[truncX],src[truncX+1],src[truncX+2]);
    } else {
      switch (getParameters().boundaryType) {
        case lti::NoBoundary: {
        } break;
        
        case lti::Zero: {
          return compute(fractX,
                         this->zeroAt(src,truncX),
                         this->zeroAt(src,truncX+1),
                         this->zeroAt(src,truncX+2));
        } 
        case lti::Constant: {
          return compute(fractX,
                         this->cstAt(src,truncX),
                         this->cstAt(src,truncX+1),
                         this->cstAt(src,truncX+2));
          
        } 
        case lti::Mirror: {
          return compute(fractX,
                         this->mirrAt(src,truncX),
                         this->mirrAt(src,truncX+1),
                         this->mirrAt(src,truncX+2));
        } 
        case lti::Periodic: {
          return compute(fractX,
                         this->periAt(src,truncX),
                         this->periAt(src,truncX+1),
                         this->periAt(src,truncX+2));
        } 
        default:
          return T();
      }
    }
    return T();
  }

  /*
   * Returns the interpolated value of the vector specified with
   * use() at the real valued position x.
   * @param x the real valued position to be interpolated.
   * @return the interpolated value of the vector.  */
  template<class T>
  T biquadraticInterpolation<T>::apply(const float& x) const {
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
  template<class T>
  T biquadraticInterpolation<T>::apply(const matrix<T>& src,
                                       const float& row,
                                       const float& col) const {
    
    return interpolate(src,row,col);
  }

  template<class T>
  T biquadraticInterpolation<T>::apply(const matrix<T>& src,
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
  template<class T>
  T biquadraticInterpolation<T>::apply(const float& row,
                                       const float& col) const {
    assert(notNull(this->theMatrix_));
    return interpolate(*this->theMatrix_,row,col);
  }

  template<class T>
  T biquadraticInterpolation<T>::apply(const fpoint& p) const {
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
  template<class T>
  T biquadraticInterpolation<T>::interpolate(const matrix<T>& src,
                                             const float& row,
                                             const float& col) const {
    // precomputations for x
    int truncX = static_cast<int>(col);
    if (col<0) {
      --truncX;
    }
    float fractX = col - static_cast<float>(truncX);
    if (fractX <= 0.5f) {
      --truncX;
    } else {
      fractX-=1.0f;
    }

    // precomputation for y
    int truncY = static_cast<int>(row);
    if (row<0) {
      --truncY;
    }
    float fractY = row - static_cast<float>(truncY);
    if (fractY <= 0.5f) {
      --truncY;
    } else {
      fractY-=1.0f;
    }

    const int x1=truncX+1;
    const int x2=truncX+2;
    const int y1=truncY+1;
    const int y2=truncY+2;

    if ((truncX >= 0) && (x2 < (src.columns())) &&
        (truncY >= 0) && (y2 < (src.rows()))) {
      return compute(fractY,fractX,
                     src.at(truncY,truncX),src.at(truncY,x1),src.at(truncY,x2),
                     src.at(y1,truncX),    src.at(y1,x1),    src.at(y1,x2),
                     src.at(y2,truncX),    src.at(y2,x1),    src.at(y2,x2));
    } else {     
      switch(getParameters().boundaryType) {
        case lti::NoBoundary: {
        } break;
        case lti::Zero: {         
          return compute(fractY,fractX,
                         this->zeroAt(src,truncY,truncX),
                         this->zeroAt(src,truncY,x1),
			 this->zeroAt(src,truncY,x2),
                         this->zeroAt(src,y1,truncX),
                         this->zeroAt(src,y1,x1),
                         this->zeroAt(src,y1,x2),
                         this->zeroAt(src,y2,truncX),
                         this->zeroAt(src,y2,x1),
                         this->zeroAt(src,y2,x2));
        } 
        case lti::Constant: {
          return compute(fractY,fractX,
                         this->cstAt(src,truncY,truncX),
                         this->cstAt(src,truncY,x1),
                         this->cstAt(src,truncY,x2),
                         this->cstAt(src,y1,truncX),
                         this->cstAt(src,y1,x1),
                         this->cstAt(src,y1,x2),
                         this->cstAt(src,y2,truncX),
                         this->cstAt(src,y2,x1),
                         this->cstAt(src,y2,x2));
        } 
        case lti::Mirror: {
          return compute(fractY,fractX,
                         this->mirrAt(src,truncY,truncX),
                         this->mirrAt(src,truncY,x1),
                         this->mirrAt(src,truncY,x2),
                         this->mirrAt(src,y1,truncX),
                         this->mirrAt(src,y1,x1),
                         this->mirrAt(src,y1,x2),
                         this->mirrAt(src,y2,truncX),
                         this->mirrAt(src,y2,x1),
                         this->mirrAt(src,y2,x2));
        } 
        case lti::Periodic: {
          return compute(fractY,fractX,
                         this->periAt(src,truncY,truncX),
                         this->periAt(src,truncY,x1),
                         this->periAt(src,truncY,x2),
                         this->periAt(src,y1,truncX),
                         this->periAt(src,y1,x1),
                         this->periAt(src,y1,x2),
                         this->periAt(src,y2,truncX),
                         this->periAt(src,y2,x1),
                         this->periAt(src,y2,x2));
	  
        } 
        default:
          return T();
      }
    }

    return T();
  }

}

