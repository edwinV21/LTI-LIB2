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
 * \file   ltiNearestNeighborInterpolation_template.h
 *         Contains the template class lti::nearestNeighborInterpolation.
 * \author Pablo Alvarado
 * \date   12.06.2001
 *
 * revisions ..: $Id: ltiNearestNeighborInterpolation_template.h,v 1.5 2012-11-17 14:49:04 alvarado Exp $
 */

#include "ltiRound.h"

namespace lti {

  // --------------------------------------------------
  // nearestNeighborInterpolation
  // --------------------------------------------------

  // default constructor
  template<class T>
  nearestNeighborInterpolation<T>::nearestNeighborInterpolation()
    : fixedGridInterpolation<T>() {
    parameters defaultParameters;
    this->setParameters(defaultParameters);
  }

  // default constructor
  template<class T>
  nearestNeighborInterpolation<T>::nearestNeighborInterpolation(
                                            const eBoundaryType boundaryType)
    : fixedGridInterpolation<T>() {
    parameters defaultParameters(boundaryType);
    this->setParameters(defaultParameters);
  }

  // copy constructor
  template<class T>
  nearestNeighborInterpolation<T>::nearestNeighborInterpolation(
                                       const nearestNeighborInterpolation& other
                                       )
    : fixedGridInterpolation<T>() {
    this->copy(other);
  }

  // destructor
  template<class T>
  nearestNeighborInterpolation<T>::~nearestNeighborInterpolation() {
  }

  // class name
  template<class T>
  const std::string& nearestNeighborInterpolation<T>::name() const {
    _LTI_RETURN_CLASS_NAME
  }


  // copy member
  template<class T>
  nearestNeighborInterpolation<T>&
  nearestNeighborInterpolation<T>::copy(const nearestNeighborInterpolation<T>& 
                                        other) {
    fixedGridInterpolation<T>::copy(other);
    return (*this);
  }

  // clone member
  template<class T>
  nearestNeighborInterpolation<T>* nearestNeighborInterpolation<T>::clone() const {
    return new nearestNeighborInterpolation(*this);
  }

  // newInstance member
  template<class T>
  nearestNeighborInterpolation<T>* nearestNeighborInterpolation<T>::newInstance() const {
    return new nearestNeighborInterpolation();
  }

  // return parameters
  template<class T>
  const typename nearestNeighborInterpolation<T>::parameters&
  nearestNeighborInterpolation<T>::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }

  // return range of influence
  template<typename T>
  int nearestNeighborInterpolation<T>::getRangeOfInfluence() const {
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
  T nearestNeighborInterpolation<T>::apply(const vector<T>& src,
                                           const float& x) const {
    const int truncX = iround(x);

    // the interpolation:
    if (static_cast<unsigned int>(truncX) <
        static_cast<unsigned int>(src.lastIdx())) {
      return src.at(truncX);
    } else {
      switch (getParameters().boundaryType) {
        case lti::NoBoundary:
        case lti::Zero: {
          return static_cast<T>(0);
        } break;
        case lti::Constant: {
          if (truncX<0) {
            return src.at(0);
          } else {
            return src.at(src.lastIdx());
          }
        } break;
        case lti::Mirror: {
          // fix x
          int p = truncX/src.size();
          int xp = truncX%src.size();
          
          if (truncX < 0) {
            p = -p + 1;
            if (xp < 0) {
              xp += src.size();
            }
          }
          
          if ((p & 01) != 0) { // odd number
            xp = src.size()-1-xp;
          }

          return src.at(xp);

          } break;
          case lti::Periodic: {
            int xp = truncX%src.size();

            if (xp < 0) {
              xp += src.size();
            }
            
            return src.at(xp);
          } break;
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
  T nearestNeighborInterpolation<T>::apply(const float& x) const {
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
  T nearestNeighborInterpolation<T>::apply(const matrix<T>& src,
                                           const float& row,
                                           const float& col) const {
    
    return interpolate(src,row,col);
  }

  template<class T>
  T nearestNeighborInterpolation<T>::apply(const matrix<T>& src,
                                           const point<float>& p) const {
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
  T nearestNeighborInterpolation<T>::apply(const float& row,
                                           const float& col) const {
    assert(notNull(this->theMatrix_));
    return interpolate(*this->theMatrix_,row,col);
  }

  template<class T>
  T nearestNeighborInterpolation<T>::apply(const point<float>& p) const {
    assert(notNull(this->theMatrix_));
    return interpolate(*this->theMatrix_,p.y,p.x);
  }


}

