/*
 * Copyright (C) 2003, 2004, 2005, 2006
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
 * \file   ltiLaplacianKernel.cpp
 *         Contains the class laplacianKernel, a second derivative kernel2D
 * \author Pablo Alvarado
 * \date   17.11.2003
 *
 * $Id: ltiLaplacianKernel.cpp,v 1.2 2007-04-05 22:55:43 alvarado Exp $
 */


#include "ltiLaplacianKernel.h"

namespace lti {
  // constructor
  laplacianKernel::laplacianKernel(const int sz)
    : kernel2D<float>() {
    generate(sz);
  }

  // generate filter
  void laplacianKernel::generate(const int sz) {
    if (sz == 2) {
      // quadratic surface interpolation Laplacian kernel
      resize(-1,-1,1,1,0.2f,Init);
      at(0,0)=-1.2f;
      at(1,0)=at(0,1)=at(-1,0)=at(0,-1)=0.1f;
      return;
    }

    if (sz == 3) {
      // standard Laplacian kernel
      resize(-1,-1,1,1,0.0f,Init);
      at(0,0)=-4.0f;
      at(1,0)=at(0,1)=at(-1,0)=at(0,-1)=1.0f;
      return;
    }

    if ((sz == 5) || (sz == 9)) {
      
      int i,j,k,l;
      l = sz/2;
      allocate(-l,-l,l,l);

      // the kernels need to be computed only once
      static const value_type* lap5 = 0; // 25 taps
      static const value_type* lap9 = 0; // 81 taps

      if (((sz == 5) && (lap5 == 0)) ||
          ((sz == 9) && (lap9 == 0))) {
        // the kernel still needs to be computed

        static const double k3row[3] = {1.000000,0.000000,-1.000000};
        static const double k3col[3] = {0.112737,0.274526, 0.112737};
        
        static const double k5row[5] =
          {+0.379022,+1.000000, 0.000000, -1.000000, -0.379022};
        static const double k5col[5] =
          { 0.0101693, 0.0708223, 0.122602, 0.0708223, 0.0101693};
       
        const int firstSize = (sz/2)+1;
        const int offset = sz/4;

        dmatrix fst(firstSize,firstSize,0.0f);
        dmatrix scd(sz,sz,0.0f);

        const double* krow;
        const double* kcol;
        
        if (sz == 5) {
          krow = k3row;
          kcol = k3col;
        } else {
          krow = k5row;
          kcol = k5col;
        }

        // 1st derivative
        for (i=0;i<firstSize;++i) {
          for (j=0;j<firstSize;++j) {
            fst.at(i,j) = krow[i]*kcol[j];
          }
        }

        // 2nd derivative
        const int last = sz-offset;
        for (i=offset;i<last;++i) {
          for (j=offset;j<last;++j) {
            for (k=0;k<firstSize;++k) {
              for (l=0;l<firstSize;++l) {
                scd.at(i+k-offset,j+l-offset)+=fst.at(i-offset,j-offset)*
                                               fst.at(k,l);
              }
            }
          }
        }

        // rotate to get the y derivative
        fst.transpose(scd);

        // compute the laplacian kernel (add second derivs in x and y).
        fst.add(scd);

        // the kernel is now in fst, we need to ensure it remains as
        // static element here
        if (sz == 9) {
          static value_type data9[81];
          for (i=0;i<81;++i) {
            data9[i]=static_cast<value_type>(fst.elem(i));
          }
          lap9 = data9; // made it "public" (at least for this class)
        } else {
          static value_type data5[25];
          for (i=0;i<25;++i) {
            data5[i]=static_cast<value_type>(fst.elem(i));
          }
          lap5 = data5; // made it "public" (at least for this class)
        }
      }

      if (sz == 5) {
        for (i=0;i<25;++i) {
        }
        fill(lap5);
      } else {
        fill(lap9);
      }

      return;
    }   
    
    // invalid kernel size
    assert((sz==2) || (sz==3) || (sz==5) || (sz==9));
    clear();

  }

}
