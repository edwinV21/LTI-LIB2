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
 * \file   ltiNearestNeighborInterpolation_inline.h
 *         Contains the implementations of the inline member functions from
 *         template class lti::nearestNeighborInterpolation<T>.
 * \author Pablo Alvarado
 * \date   12.06.2001
 *
 * revisions ..: $Id: ltiNearestNeighborInterpolation_inline.h,v 1.4 2013-03-10 05:57:10 alvarado Exp $
 */

namespace lti {

  // ---------------------------------------
  // implementation of public inline methods
  // ---------------------------------------

  // interpolateUnchk
  template<class T>
  inline T nearestNeighborInterpolation<T>
  ::interpolateUnchk(const matrix<T>& src,
                     const float row,
                     const float col) const {

    const int truncY = iround(row);
    const int truncX = iround(col);
    return src.at(truncY,truncX);
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
  inline T nearestNeighborInterpolation<T>::interpolate(const matrix<T>& src,
                                                        const float row,
                                                        const float col) const {


    const int truncY = iround(row);
    const int truncX = iround(col);

    // the interpolation:
    if ((static_cast<unsigned int>(truncX)<
         static_cast<unsigned int>(src.columns())) &&
        (static_cast<unsigned int>(truncY)<
         static_cast<unsigned int>(src.rows()))) {
      // normal interpolation within matrix range
      return src.at(truncY,truncX);
    } else {
      switch (getParameters().boundaryType) {
        case lti::NoBoundary: 
        case lti::Zero:{
          return static_cast<T>(0);
        } break;
        case lti::Constant: {
          int x=truncX;
          int y=truncY;
          if (x<0) {
            x=0;
          } else if (x>=src.columns()) {
            x=src.lastColumn();
          }
          if (y<0) {
            y=0;
          } else if (y>=src.rows()) {
            y=src.lastRow();
          }
          return src.at(y,x);
        } break;
        case lti::Mirror: {
          int p,xp,yp;

          // fix x
          p  = truncX/src.columns();
          xp = truncX%src.columns();

          if (truncX < 0) {
            p = 1-p;
            if (xp < 0) {
              xp += src.columns();
            }
          }

          if ((p & 01) != 0) { // odd number
            xp = src.lastColumn()-xp;
          }

          // fix y
          p = truncY/src.rows();
          yp = truncY%src.rows();

          if (truncY < 0) {
            p = 1-p;
            if (yp < 0) {
              yp += src.rows();
            }
          }

          if ((p & 01) != 0) { // odd number
            yp = src.lastRow()-yp;
          }

          return src.at(yp,xp);
        } break;
        case lti::Periodic: {
          int x = truncX%src.columns();

          if (x < 0) {
            x += src.columns();
          }

          int y = truncY%src.rows();
          
          if (y < 0) {
            y += src.rows();
          }

          return src.at(y,x);
        } break;
        default:
          return T();
      }
    }
    return T();
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
  inline T nearestNeighborInterpolation<T>::interpolate(const matrix<T>& src,
                                                        const int row,
                                                        const int col) const {

    // the interpolation:
    if ((static_cast<unsigned int>(col)<
         static_cast<unsigned int>(src.columns())) &&
        (static_cast<unsigned int>(row)<
         static_cast<unsigned int>(src.rows()))) {
      // just return the pixel value
      return src.at(row,col);
    } else {
      switch (getParameters().boundaryType) {
        case lti::NoBoundary: 
        case lti::Zero:{
          return static_cast<T>(0);
        } break;
        case lti::Constant: {
          int x=col;
          int y=row;
          if (x<0) {
            x=0;
          } else if (x>=src.columns()) {
            x=src.lastColumn();
          }
          if (y<0) {
            y=0;
          } else if (y>=src.rows()) {
            y=src.lastRow();
          }
          return src.at(y,x);
        } break;
        case lti::Mirror: {
          int p,xp,yp;

          // fix x
          p  = col/src.columns();
          xp = col%src.columns();

          if (col < 0) {
            p = 1-p;
            if (xp < 0) {
              xp += src.columns();
            }
          }

          if ((p & 01) != 0) { // odd number
            xp = src.lastColumn()-xp;
          }

          // fix y
          p = row/src.rows();
          yp = row%src.rows();

          if (row < 0) {
            p = 1-p;
            if (yp < 0) {
              yp += src.rows();
            }
          }

          if ((p & 01) != 0) { // odd number
            yp = src.lastRow()-yp;
          }

          return src.at(yp,xp);
        } break;
        case lti::Periodic: {
          int x = col%src.columns();

          if (x < 0) {
            x += src.columns();
          }

          int y = row%src.rows();
          
          if (y < 0) {
            y += src.rows();
          }

          return src.at(y,x);
        } break;
        default:
          return T();
      }
    }
    return T();
  }


}
