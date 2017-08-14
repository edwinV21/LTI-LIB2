/*
 * Copyright (C) 1998-2004
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
 * \file   ltiFixedGridInterpolation_inline.h
 *         Contains the class fixedGridInterpolation.
 * \author Peter Doerfler
 * \author Pablo Alvarado
 * \date   03.05.2003
 *
 * $Id: ltiFixedGridInterpolation_inline.h,v 1.5 2012-09-16 05:22:59 alvarado Exp $
 */


namespace lti {

  template<typename T>
  inline T fixedGridInterpolation<T>::interpolate(const matrix<T>& src,
                                                  const float& row,
                                                  const float& col) const {
    return apply(src,row,col);
  }

  // -------------------------------------------------------------------
  // The inline boundary access methods
  // -------------------------------------------------------------------

  /*
   * Access with zero boundary
   */
  template<typename T>
  inline T fixedGridInterpolation<T>::zeroAt(const vector<T>& vct,
                                             const int x) const {
    if (static_cast<uint32>(x)<=static_cast<uint32>(vct.size())) {
      return vct.at(x);
    } 
    return T(0);
  }

  /*
   * Access with constant boundary
   */
  template<typename T>
  inline T fixedGridInterpolation<T>::cstAt(const vector<T>& vct,
                                            const int x) const {
    if (x<0) {
      return vct.at(0);
    } else if (x>vct.lastIdx()) {
      return vct.at(vct.lastIdx());
    }
    return vct.at(x);
  }

  /*
   * Access with mirrored boundary
   */
  template<typename T>
  inline T fixedGridInterpolation<T>::mirrAt(const vector<T>& vct,
                                             const int x) const {

    int p,xp;
    
    // fix x
    const int vsize=vct.size();
    p = x/vsize;
    xp = x%vsize;

    if (x < 0) {
      p = 1-p;
      if (xp < 0) {
        xp += vsize;
      }
    }
            
    if ((p & 01) != 0) { // odd number
      xp = vct.lastIdx()-xp;
    }

    return vct.at(xp);
  }

  /*
   * Access with periodic boundary
   */
  template<typename T>
  inline T fixedGridInterpolation<T>::periAt(const vector<T>& vct,
                                             const int x) const {
    int xp;
    const int vsize = vct.size();
    xp = x % vsize;
    if (xp < 0) {
      xp += vsize;
    }
    return vct.at(xp);
  }

  /*
   * Access with zero boundary
   */
  template<typename T>
  inline T fixedGridInterpolation<T>::zeroAt(const matrix<T>& img,
                                             const int y,
                                             const int x) const {
    if (static_cast<uint32>(y)<=static_cast<uint32>(img.lastRow()) && 
        static_cast<uint32>(x)<=static_cast<uint32>(img.lastColumn())) {
      return img.at(y,x);
    } 
    return T(0);
  }

  /*
   * Access with constant boundary
   */
  template<typename T>
  inline T fixedGridInterpolation<T>::cstAt(const matrix<T>& img,
                                            const int y,
                                            const int x) const {
    
    const int lastR = img.lastRow();
    const int lastC = img.lastColumn();
    
    const int posx = ( x<0 ? 0 : ( x>lastC ? lastC : x) );
    const int posy = ( y<0 ? 0 : ( y>lastR ? lastR : y) );

    return img.at(posy, posx);
  }
  
  /*
   * Access with mirrored boundary
   */
  template<typename T>
  inline T fixedGridInterpolation<T>::mirrAt(const matrix<T>& img,
                                             const int y,
                                             const int x) const {


    int p,xp,yp;
    const int icols=img.columns();
    const int irows=img.rows();
    
    // fix x
    p = x/icols;
    xp = x%icols;

    if (x < 0) {
      p = 1-p;
      if (xp < 0) {
        xp += icols;
      }
    }
            
    if ((p & 01) != 0) { // odd number
      xp = img.lastColumn()-xp;
    }

    // fix y
    p  = y/irows;
    yp = y%irows;

    if (y < 0) {
      p = 1-p;
      if (yp < 0) {
        yp += irows;
      }
    }
            
    if ((p & 01) != 0) { // odd number
      yp = img.lastRow()-yp;
    }

    return img.at(yp,xp);
  }

  /*
   * Access with period boundary
   */
  template<typename T>
  inline T fixedGridInterpolation<T>::periAt(const matrix<T>& img,
                                              const int y,
                                              const int x) const {
    const int icols=img.columns();
    const int irows=img.rows();

    int xp,yp;
    xp = x % icols;
    if (xp < 0) {
      xp += icols;
    }

    yp = y % irows;
    if (yp < 0) {
      yp += irows;
    }

    return img.at(yp,xp);
  }



}
