/*
 * Copyright (C) 2003 Vlad Popovici, EPFL STI-ITS, Switzerland
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
 * \file   ltiRotation.h
 *         This file contains the functor rotation with rotates matrices
 * \author Vlad Popovici
 * \author Pablo Alvarado
 * \date   18.06.2003
 *
 * $Id: ltiRotation_template.h,v 1.3 2007-12-26 04:49:23 alvarado Exp $
 */

#include "ltiMath.h"
#include "ltiRound.h"
#include <limits>

#include "ltiDebug.h"

namespace lti {

  // Skews horizontally the matrix<T> src and save the result in dst.
  template <typename T>
  bool rotation::horizontalSkew(const matrix<T>& src, 
                                   matrix<T>& dst, 
                                   const int rowIdx,
                                   const int colOffset, 
                                   const double weight) const {
    
    // ASSUMTION: dst has the correct size, which is always >= src.size()

    T p0,p1;

    if (colOffset>=0) {
      // col offset is positive: black + data + black

      T oldpx = T(0);

      // some pointer arithmetic to accelerate the shift
      T* dptr = &dst.at(rowIdx,0);
      T* eptr = dptr+colOffset;

      // clear the beginning
      while (dptr<eptr) {
        *dptr++=T(0);
      }    
      
      // copy the shifted elements
      const T* sptr = &src.at(rowIdx,0);

      if (colOffset+src.columns() < dst.columns()) {
        eptr = (&dst.at(rowIdx,0)) + colOffset + src.columns();
      } else {
        eptr = (&dst.at(rowIdx,0)) + dst.columns();
      }
    
      while (dptr<eptr) {        
        p0      = (*sptr);
        p1      = static_cast<T>(p0 * weight);
        p0     -= (p1 - oldpx);
        (*dptr) = p0;
        oldpx   = p1;

        ++dptr;
        ++sptr;
      }

      eptr = (&dst.at(rowIdx,0)) + dst.columns();

      if (dptr<eptr) {
        *dptr++=oldpx;
      }

      while (dptr < eptr) {
        *dptr++=T(0);
      }

    } else {
      // colOffset<0
      T oldpx = static_cast<T>(src.at(rowIdx,-1-colOffset) * weight);

      // some pointer arithmetic to accelerate the shift
      T* dptr = &dst.at(rowIdx,0);
      const T* sptr = (&src.at(rowIdx,0))-colOffset; // colOffset is negative!
      const T *const eptr = (&src.at(rowIdx,0))+src.columns();
      
      while (sptr<eptr) {
        p0      = (*sptr);
        p1      = static_cast<T>(p0 * weight);
        p0     -= (p1 - oldpx);
        (*dptr) = p0;
        oldpx   = p1;

        ++dptr;
        ++sptr;
      }

      // try to get the last interpolation with zero
      T *const edptr = (&dst.at(rowIdx,0)) + dst.columns();
      if (dptr<edptr) {
        *dptr++=oldpx;
      }

      // fill the rest with zero
      while (dptr < edptr) {
        *dptr++=T(0);
      }

    }

    return true;
  }


  // Skews vertically the matrix<T> src and save the result in dst.
  template <typename T>
  bool rotation::verticalSkew(const matrix<T>& src,
                                 matrix<T>& dst,
                                 const int colIdx, 
                                 const int rowOffset,
                                 const double weight) const {
    

    // ASSUMTION: dst has the correct size
    T p0,p1;
    int y,dy;

    if (rowOffset>=0) {
      T oldpx = T(0);

      // clear the beginning
      for (y=0;y<rowOffset;++y) {
        dst.at(y,colIdx)=T(0);
      }
     
      // copy the shifted elements
      int end;
      if (rowOffset+src.rows() < dst.rows()) {
        end = rowOffset+src.rows();
      }
      else {
        end = dst.rows();
      }
      
      for (dy=0;y<end;++y,++dy) {
        p0               = src.at(dy,colIdx);
        p1               = static_cast<T>(p0 * weight);
        p0              -= (p1 - oldpx);
        dst.at(y,colIdx) = p0;
        oldpx            = p1;
      }

      if (y < dst.rows()) {
        dst.at(y,colIdx) = oldpx;
        ++y;
      }
     
      for (;y<dst.rows();++y) {
        dst.at(y,colIdx)=T(0);
      }

    } else {
      // rowOffset<0
      T oldpx = static_cast<T>(src.at(-1-rowOffset,colIdx) * weight);

      for (y=-rowOffset,dy=0;y<src.rows();++y,++dy) {
        p0                = src.at(y,colIdx);
        p1                = static_cast<T>(p0 * weight);
        p0               -= (p1 - oldpx);
        dst.at(dy,colIdx) = p0;
        oldpx             = p1;
      }

      if (dy < dst.rows()) {
        dst.at(dy,colIdx) = oldpx;
        ++dy;
      }

      // fill the rest with zero
      for (;dy<dst.rows();++dy) {
        dst.at(dy,colIdx) = T(0);
      }

    }

    return true;
  }

  // Rotates by an angle between -pi/2 and pi/2 using the 3-shear technique.
  template <typename T>
  bool rotation::rotateShear(const matrix<T>& src,
                             matrix<T>& dst,
                             const double& angle) const {
    if (angle == 0.0) {
      dst.copy(src);
      return true;
    }

    const double sinA = sin(angle);
    const double cosA = cos(angle);
    const double tanA = tan(angle/2.0);
    double factor = 0.0;

    const int nRows1 = src.rows();
    const int nCols1 = src.columns() + iround(src.rows() * abs(tanA));
 
    matrix<T> tmpImg1(nRows1, nCols1);

    // Shear 1: horizontal
    int x,y;
    if (tanA < 0.0) { // clockwise rotation
      for (y = 0; y < nRows1; y++) {
	factor = tanA * (y - nRows1 + 0.5);
        horizontalSkew(src, tmpImg1, y, static_cast<int>(floor(factor)),
                       factor - floor(factor));
      }
    }      
    else { // counter-clockwise rotation
      for (y = 0; y < nRows1; y++) {
        factor = tanA * (y + 0.5);
        horizontalSkew(src, tmpImg1, y, static_cast<int>(floor(factor)),
                       factor - floor(factor));
      }
    }

    // Shear 2: vertical
    const int nCols2 = nCols1;
    const int nRows2 = static_cast<int>(abs(sinA) * src.columns() + 
                                        cosA* src.rows()) + 1;

    matrix<T> tmpImg2(nRows2,nCols2);

    if (sinA < 0.0) { // clockwise
      factor = -sinA * (src.columns() - nCols2);
    }
    else {  // counter-clockwise
      factor = sinA * (src.columns() - 1.0);
    }
    for (x = 0; x < nCols2; ++x) {
      verticalSkew(tmpImg1, tmpImg2, x, static_cast<int>(floor(factor)),
		   factor - floor(factor));
      factor -= sinA;
    }

    // free some memory:
    tmpImg1.clear();

    // prepare destination
    const int nRows = nRows2;
    const int nCols = static_cast<int>(abs(sinA) * src.rows() + 
                                       cosA * src.columns()) + 1;

    dst.allocate(nRows, nCols);

    // Shear 3: horizontal
    if (sinA < 0.0) { // clockwise
      factor = tanA * (-sinA * (src.columns() - 1) + 1.0 - nRows);
    }
    else {   // counter-clockwise
      factor = -tanA * sinA * (src.columns() - 1);
    }
    for (y = 0; y < nRows; ++y) {
      horizontalSkew(tmpImg2, dst, y, static_cast<int>(floor(factor)),
		     factor - floor(factor));
      factor += tanA;
    }

    return true;
  }

  // Rotates by 90 degrees.
  template <typename T>
  bool rotation::rotate90(const matrix<T>& src, 
                             matrix<T>& dst) const {
    const int cols=src.columns();
    const int rows=src.rows();
    dst.allocate(cols,rows);
    const int lrow=dst.lastRow();

    for (int y = 0; y < rows; y++) {
      for (int x = 0; x < cols; x++) {
    	dst.at(lrow-x,y) = src.at(y,x);
      }
    }
    
    return true;
  }


  // Rotates by 180 degrees.
  template <typename T>
  bool rotation::rotate180(const matrix<T>& src,
                              matrix<T>& dst) const {
    const int cols=src.columns();
    const int rows=src.rows();
    dst.allocate(rows,cols);
    const int lrow=rows-1;
    const int lcol=cols-1;

    for (int y = 0; y < rows; y++) {
      for (int x = 0; x < cols; x++) {
	dst.at(lrow-y,lcol-x) = src.at(y,x);
      }
    }

    return true;
  }


  // Rotates by 270 degrees.
  template <typename T>
  bool rotation::rotate270(const matrix<T>& src,
                              matrix<T>& dst) const {
    const int cols=src.columns();
    const int rows=src.rows();
    dst.allocate(cols,rows);
    const int lcol=dst.lastColumn();

    for (int y = 0; y < rows; y++) {
      for (int x = 0; x < cols; x++) {
	dst.at(x,lcol-y) = src.at(y,x);
      }
    }

    return true;
  }

  template <typename T>
  bool rotation::rotate(const matrix<T>& src,matrix<T>& dest,
                        const double& theAngle) const {
    matrix<T> tmpImg;

    //angle is counter-clockwise for shear method
    const double angle=-theAngle;

    // make sure angle is in [0.0 , 2*Pi)
    double a = abs(angle);
    const double s = signum(angle);
    const double k = a / (2.0*Pi);

    if (abs(a) <= std::numeric_limits<double>::epsilon()) { // nothing to do!
      dest.copy(src);
      return true;
    }

    a = s * (a - 2.0*Pi*floor(k));
    if (a < 0.0) {
      a += 2.0*Pi;
    }

    if (a > (Pi*0.25) && a <= (Pi*0.75)) {
      _lti_debug("Call rotate90 a="<<a<<"\n");
      rotate90(src, tmpImg);            // start with a 90 degree rotation
      a -= 0.5*Pi;   // it's between -45 and 45
    }
    else if (a > (Pi*0.75) && a <= (Pi*1.25)) {
      rotate180(src, tmpImg);        // start with a 180 degree rotation
      a -= Pi;                       // it's between -45 and 45
    }
    else if (a > (Pi*1.25) && a <= (Pi*1.75)) {
      rotate270(src, tmpImg);        // start with a 270 degree rotation
      a -= 1.5*Pi;                   // it's between -45 and 45
    }
    else {
      tmpImg.copy(src);
    }
    
    if (abs(a) > std::numeric_limits<double>::epsilon()) { // final rotation:
      rotateShear(tmpImg, dest, a);
    }
    else {
      tmpImg.detach(dest);
    }

    return true;
  }

  template <typename T>
  bool rotation::rotate(matrix<T>& srcdest,
                        const double& theAngle) const {
    matrix<T> tmp;
    if (rotate(srcdest,tmp,theAngle)) {
      tmp.detach(srcdest);
      return true;
    }
    return false;
  }

  template <typename T>
  bool rotation::apply(const matrix<T>& src, matrix<T>& dest) const {
    return rotate(src,dest,getParameters().angle);
  }

  template <typename T>
  bool rotation::apply(matrix<T>& srcdest) const {
    return rotate(srcdest,getParameters().angle);
  }


}
