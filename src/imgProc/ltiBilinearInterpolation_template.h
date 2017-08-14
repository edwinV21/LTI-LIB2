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


/* 
 * \file   ltiBilinearInterpolation_template.h
 *         Contains the template class bilinearInterpolation_template.h
 * \author Pablo Alvarado
 * \date   12.06.2001
 *
 * revisions ..: $Id: ltiBilinearInterpolation_template.h,v 1.9 2012-11-17 14:49:04 alvarado Exp $
 */

#include "ltiRGBPixel.h"
#include "ltiRGBAPixel.h"


namespace lti {
  // --------------------------------------------------
  // bilinearInterpolation<T>::parameters
  // --------------------------------------------------

  // default constructor
  template<typename T>
  bilinearInterpolation<T>::parameters::parameters()
    : fixedGridInterpolation<T>::parameters() {   
  }

  // default constructor
  template<typename T>
  bilinearInterpolation<T>::parameters::parameters(const eBoundaryType btype)
    : fixedGridInterpolation<T>::parameters(btype) {   
  }



  // default constructor
  template<typename T>
  bilinearInterpolation<T>::bilinearInterpolation(const eBoundaryType btype)
    : fixedGridInterpolation<T>() {
    parameters defaultParameters(btype);
    this->setParameters(defaultParameters);
  }


  // copy constructor
  template<typename T>
  bilinearInterpolation<T>::parameters::parameters(const parameters& other)
    : fixedGridInterpolation<T>::parameters() {
    copy(other);
  }

  // destructor
  template<typename T>
  bilinearInterpolation<T>::parameters::~parameters() {
  }

  // copy member
  template<typename T>
  typename bilinearInterpolation<T>::parameters&
  bilinearInterpolation<T>::parameters::copy(const parameters& other) {
    fixedGridInterpolation<T>::parameters::copy(other);

    return *this;
  }

  // alias for copy method
  template<typename T>
  typename bilinearInterpolation<T>::parameters&
  bilinearInterpolation<T>::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  // return class name
  template<typename T>
  const std::string& bilinearInterpolation<T>::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
      }
  
  // clone method
  template<typename T>
  typename bilinearInterpolation<T>::parameters*
  bilinearInterpolation<T>::parameters::clone() const {
    return new parameters(*this);
  }

  // new instance
  template<typename T>
  typename bilinearInterpolation<T>::parameters* 
  bilinearInterpolation<T>::parameters::newInstance() const {
    return new parameters();
  }

  /*
   * Write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  template<typename T>
  bool bilinearInterpolation<T>::parameters::write(ioHandler& handler,
                                                   const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {
      
    }

    b = b && fixedGridInterpolation<T>::parameters::write(handler,false);

    if (complete) {
      b = b && handler.writeEnd();
    }

    return b;
  }

  /*
   * Read the parameters from the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also read, otherwise only the data block will be read.
   * @return true if read was successful
   */
  template<typename T>
  bool bilinearInterpolation<T>::parameters::read(ioHandler& handler,
                                                  const bool complete)  {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {
      
    }

    b = b && fixedGridInterpolation<T>::parameters::read(handler,false);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

  // --------------------------------------------------
  // bilinearInterpolation
  // --------------------------------------------------

  // default constructor
  template<typename T>
  bilinearInterpolation<T>::bilinearInterpolation()
    : fixedGridInterpolation<T>() {

    // create an instance of the parameters with the default values
    parameters defaultParameters;
    // set the default parameters
    this->setParameters(defaultParameters);
  }

  // default constructor
  template<typename T>
  bilinearInterpolation<T>::bilinearInterpolation(const parameters& par)
    : fixedGridInterpolation<T>() {

    // set the given parameters
    this->setParameters(par);
  }


  // copy constructor
  template<typename T>
  bilinearInterpolation<T>::bilinearInterpolation(const bilinearInterpolation<T>& other)
    : fixedGridInterpolation<T>() {
    copy(other);
  }

  // destructor
  template<typename T>
  bilinearInterpolation<T>::~bilinearInterpolation() {
  }

  // copy member
  template<typename T>
  bilinearInterpolation<T>& 
  bilinearInterpolation<T>::copy(const bilinearInterpolation<T>& other) {
    fixedGridInterpolation<T>::copy(other);

    return (*this);
  }

  // alias for copy member
  template<typename T>
  bilinearInterpolation<T>&
  bilinearInterpolation<T>::operator=(const bilinearInterpolation<T>& other) {
    return (copy(other));
  }

  // class name
  template<typename T>
  const std::string& bilinearInterpolation<T>::name() const {
    _LTI_RETURN_CLASS_NAME
      }

  // clone member
  template<typename T>
  bilinearInterpolation<T>* bilinearInterpolation<T>::clone() const {
    return new bilinearInterpolation<T>(*this);
  }

  // clone member
  template<typename T>
  bilinearInterpolation<T>* bilinearInterpolation<T>::newInstance() const {
    return new bilinearInterpolation<T>();
  }

  // return parameters
  template<typename T>
  const typename bilinearInterpolation<T>::parameters&
  bilinearInterpolation<T>::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }


  // return range of influence
  template<typename T>
  int bilinearInterpolation<T>::getRangeOfInfluence() const {
    return 1;
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
  T bilinearInterpolation<T>::apply(const vector<T>& src,
                                    const float& pos) const {
    int truncX = static_cast<int>(pos);
    if (pos<0) {
      --truncX;
    }
    const float fractX = pos - static_cast<float>(truncX);

    // the interpolation:
    if (static_cast<unsigned int>(truncX) <
        static_cast<unsigned int>(src.lastIdx())) {
      return compute(fractX,src.at(truncX),src.at(truncX + 1));
    } else {
      int x,x1;
      switch (getParameters().boundaryType) {
        case lti::NoBoundary: {
        } break;

        case lti::Zero: {
          T a1(0),a2(0);

          if (truncX < 0) { // at the left side
            if (truncX == -1) { // has only one pixel
              a2 = src.at(0);
            } 
          } else {
            // if (truncX >= 0):
            if (truncX >= src.lastIdx()) { // and at the right side
              // is outside
              if (truncX < src.size()) {
                a1 = src.at(truncX);         // or has also one pixel
              }
            }
          }

          return compute(fractX,a1,a2);


        } 
        case lti::Constant: {
          x = truncX;
          x1 = truncX+1;

          if (x<0) {
            x=0;
          } else if (x>=src.size()) {
            x = src.lastIdx();
          }

          if (x1<0) {
            x1=0;
          } else if (x1>=src.size()) {
            x1 = src.lastIdx();
          }

          return compute(fractX,src.at(x),src.at(x1));

        } 
        case lti::Mirror: {
          x = truncX;
          x1 = truncX+1;

          int p,xp;

          // fix x
          p = x/src.size();
          xp = x%src.size();

          if (x < 0) {
            p = -p + 1;
            if (xp < 0) {
              xp += src.size();
            }
          }

          if ((p & 01) != 0) { // odd number
            xp = src.size()-1-xp;
          }

          x = xp;

          // fix x1
          p = x1/src.size();
          xp = x1%src.size();

          if (x1 < 0) {
            p = -p + 1;
            if (xp < 0) {
              xp += src.size();
            }
          }

          if ((p & 01) != 0) { // odd number
            xp = src.size()-1-xp;
          }

          x1 = xp;

          return compute(fractX,src.at(x),src.at(x1));

        } 
        case lti::Periodic: {
          x = truncX;
          x1 = truncX+1;

          x = x%src.size();

          if (x < 0) {
            x += src.size();
          }

          x1 = x1%src.size();

          if (x1 < 0) {
            x1 += src.size();
          }

          return compute(fractX,src.at(x),src.at(x1));
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
  template<typename T>
  T bilinearInterpolation<T>::apply(const float& x) const {
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
  T bilinearInterpolation<T>::apply(const matrix<T>& src,
                                    const float& row,
                                    const float& col) const {

    return interpolate(src,row,col);
  }

  template<typename T>
  T bilinearInterpolation<T>::apply(const matrix<T>& src,
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
  T bilinearInterpolation<T>::apply(const float& row,
                                    const float& col) const {
    assert(notNull(this->theMatrix_));
    return interpolate(*this->theMatrix_,row,col);
  }

  template<typename T>
  T bilinearInterpolation<T>::apply(const point<float>& p) const {
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
  T bilinearInterpolation<T>::interpolate(const matrix<T>& src,
                                          const float row,
                                          const float col) const {

    const int truncY = (row < 0.0f) ?
      static_cast<int>(row)-1 : static_cast<int>(row);
    const int truncY1 = truncY+1;
    const float fractY = row - static_cast<float>(truncY);

    const int truncX = (col < 0.0f) ?
      static_cast<int>(col)-1 : static_cast<int>(col);
    const int truncX1 = truncX+1;
    const float fractX = col - static_cast<float>(truncX);

    // the interpolation:
    if ((static_cast<unsigned int>(truncX)<
         static_cast<unsigned int>(src.lastColumn())) &&
        (static_cast<unsigned int>(truncY)<
         static_cast<unsigned int>(src.lastRow()))) {
      // normal interpolation within matrix range
      return compute(fractY,fractX,
                     src.at(truncY ,truncX),src.at(truncY ,truncX1),
                     src.at(truncY1,truncX),src.at(truncY1,truncX1));
    } else {
      int x,y,x1,y1;
      switch (getParameters().boundaryType) {
        case lti::NoBoundary: {
          return T();
        } 
        case lti::Zero: {
          T a1(0),a2(0),a3(0),a4(0);

          if (truncY < 0) {
            if (truncY == -1) { // the first line
              if (truncX < 0) { // at the left side
                if (truncX == -1) { // has only one pixel
                  a4 = src.at(0,0);
                } 
              } else {
                // if (truncX >= 0):
                if (truncX1 >= (src.columns())) { // and at the right side
                  // is outside
                  if (truncX < src.columns()) {
                    a3 = src.at(0,truncX);         // or has also one pixel
                  }
                } else {
                  a3 = src.at(0,truncX);           // two pixels are in
                  a4 = src.at(0,truncX1);          // middle of the first
                  // line
                }
              }
            }
          } else {
            // if (truncY >= 0)
            if (truncY1 >= (src.rows())) {
              if (truncY < src.rows()) {
                // last line
                if (truncX < 0) { // at the left side
                  if (truncX == -1) { // has only one pixel
                    a2 = src.at(0,0);
                  }
                } else {
                  // if (truncX >= 0):
                  if (truncX1 >= (src.columns())) { // at the right side
                    // is outside
                    if (truncX < src.columns()) {
                      a1 = src.at(truncY,truncX); // or has also one pixel
                    }
                  } else {
                    a1 = src.at(truncY,truncX);   // two pixel in the mid.
                    a2 = src.at(truncY,truncX1); // of the last line
                  }
                }
              }
            } else {
              // for the y inbetween...
              if (truncX < 0) {
                // left border
                if (truncX == -1) {
                  // still one pixel there
                  a2 = src.at(truncY,0);
                  a4 = src.at(truncY1,0);
                } 
              } else {
                // right border
                if (truncX1 >=(src.columns())) {
                  // outside
                  if (truncX < src.columns()) {
                    // still one pixel there
                    a1 = src.at(truncY,truncX);
                    a3 = src.at(truncY1,truncX);
                  }
                }
              }
            }
          }
          return compute(fractY,fractX,a1,a2,a3,a4);

        } 
        case lti::Constant: {
          x = truncX;
          y = truncY;
          x1 = truncX1;
          y1 = truncY1;

          if (x<0) {
            x=0;
          } else if (x>=src.columns()) {
            x = src.lastColumn();
          }

          if (x1<0) {
            x1=0;
          } else if (x1>=src.columns()) {
            x1 = src.lastColumn();
          }

          if (y<0) {
            y=0;
          } else if (y>=src.rows()) {
            y = src.lastRow();
          }

          if (y1<0) {
            y1=0;
          } else if (y1>=src.rows()) {
            y1 = src.lastRow();
          }

          return compute(fractY,fractX,
                         src.at(y,x),src.at(y,x1),
                         src.at(y1,x),src.at(y1,x1));

        } 
        case lti::Mirror: {
          x = truncX;
          y = truncY;
          x1 = truncX1;
          y1 = truncY1;

          int p,xp;

          // fix x
          p = x/src.columns();
          xp = x%src.columns();

          if (x < 0) {
            p = 1-p;
            if (xp < 0) {
              xp += src.columns();
            }
          }

          if ((p & 01) != 0) { // odd number
            xp = src.lastColumn()-xp;
          }

          x = xp;

          // fix x1
          p = x1/src.columns();
          xp = x1%src.columns();

          if (x1 < 0) {
            p = 1-p;
            if (xp < 0) {
              xp += src.columns();
            }
          }

          if ((p & 01) != 0) { // odd number
            xp = src.lastColumn()-xp;
          }

          x1 = xp;

          // fix y
          p = y/src.rows();
          xp = y%src.rows();

          if (y < 0) {
            p = 1-p;
            if (xp < 0) {
              xp += src.rows();
            }
          }

          if ((p & 01) != 0) { // odd number
            xp = src.lastRow()-xp;
          }

          y = xp;

          // fix y1
          p = y1/src.rows();
          xp = y1%src.rows();

          if (y1 < 0) {
            p = 1-p;
            if (xp < 0) {
              xp += src.rows();
            }
          }

          if ((p & 01) != 0) { // odd number
            xp = src.lastRow()-xp;
          }

          y1 = xp;

          return compute(fractY,fractX,
                         src.at(y,x),src.at(y,x1),
                         src.at(y1,x),src.at(y1,x1));
        } 
        case lti::Periodic: {
          x = truncX;
          y = truncY;
          x1 = truncX;
          y1 = truncY;

          x = x%src.columns();

          if (x < 0) {
            x += src.columns();
          }

          x1 = x1%src.columns();

          if (x1 < 0) {
            x1 += src.columns();
          }

          y = y%src.rows();

          if (y < 0) {
            y += src.rows();
          }

          y1 = y1%src.rows();

          if (y1 < 0) {
            y1 += src.rows();
          }

          return compute(fractY,fractX,
                         src.at(y,x),src.at(y,x1),
                         src.at(y1,x),src.at(y1,x1));
        } 
        default:
          return T();
      }
    }

  }


}
