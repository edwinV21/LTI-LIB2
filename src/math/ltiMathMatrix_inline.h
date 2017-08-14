/*
 * Copyright (C) 1998-2005
 * Peter Doerfler
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
 * \file   ltiMathMatrix_inline.h
 *         Contains some useful vector and matrix functions that are
 *         too special for the actual classes.
 * \author Peter Doerfler
 * \date   10.07.2005
 *
 * $Id: ltiMathMatrix_inline.h,v 1.2 2007-04-05 22:13:26 alvarado Exp $
 */

namespace lti {

  // square the given matrix
  template <typename T>
  inline void squareMatrix(const matrix<T>& src, 
                           const eTransposeType trans,
                           matrix<T>& dest) {


    switch (trans) {
      case Transpose: {
        matrix<T> tmp;
        tmp.transpose(src);
        const int rows=tmp.rows();
        dest.allocate(rows,rows);
        
        for (int i=0; i<rows; i++) {
          const vector<T>& vec=tmp.getRow(i);
          dest.at(i,i)=vec.dot(vec);
          for (int j=0; j<rows; j++) {
            const T res=vec.dot(tmp.getRow(j));
            dest.at(i,j)=res;
            dest.at(j,i)=res;
          }
        }
        break;
      }

      case Normal: {
        const int rows=src.rows();
        dest.allocate(rows,rows);
        
        for (int i=0; i<rows; i++) {
          const vector<T>& vec=src.getRow(i);
          dest.at(i,i)=vec.dot(vec);
          for (int j=0; j<rows; j++) {
            const T res=vec.dot(src.getRow(j));
            dest.at(i,j)=res;
            dest.at(j,i)=res;
          }
        }
        break;
      }

      default:
        throw exception("Unknown eTransposeType");
        break;
    }

        
  }
  

}
