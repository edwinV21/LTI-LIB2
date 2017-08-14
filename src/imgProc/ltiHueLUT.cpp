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
 * \file   ltiHueLUT.cpp
 *         Contains the hue lookup table.
 * \author Pablo Alvarado
 * \author Peter Doerfler
 * \date   28.04.2005
 *
 * $Id: ltiHueLUT.cpp,v 1.3 2006-01-06 05:04:37 alvarado Exp $
 */
#include "ltiHueLUT.h"
#include "ltiConstants.h"

#undef  _LTI_DEBUG
//#define _LTI_DEBUG 3
#include "ltiDebug.h"

namespace lti {

  // static members
  const hueLUT::cfloatptr_* hueLUT::hueLUT_ = 0;

  // constructor
  hueLUT::hueLUT() {
    constructHueLUT();
  }


  // construct a hue LUT.
  void hueLUT::constructHueLUT() {
    if (isNull(hueLUT_)) {
      static const int nrows = 512;
      static const int size = nrows*(nrows+1)/2;
      static const int maxVal = nrows-1;

      // use the singleton approach to avoid memory leak at the end of 
      // the application
      static float lutData[size]; // the data
      float* lutLines[nrows];     // pointers to 0-th element of each line
      static cfloatptr_ clutLines[nrows];

      int i,j,k,r,g;

      for (i=0,j=0,k=nrows;i<nrows;i++,j+=k,k--) {
        lutLines[i]=&lutData[j];
        clutLines[i]=&lutData[j];
      }

      float** tmpLUT = lutLines;

      double alpha,beta;
      for (r=0, j=nrows;r<nrows;r++, j--) {
        for (g=0;g<j;g++) {
          alpha = 0.5*(3.*r-maxVal);          
          alpha /= sqrt(3.*(r*r+r*g+g*g) - maxVal*(3.*(r+g)-maxVal));
          beta = acos(alpha)/constants<float>::twoPi();

          if (maxVal>(2*g+r)) { //i.e. blue > green
            beta = 1.-beta;
          }

          tmpLUT[r][g] = static_cast<float>(beta);
        }
      }

      // now make the singleton array "public".  It will be deleted at
      // the end.
      hueLUT_ = clutLines;
    }
  }

  /*
   * Returns the name of this class
   */
  const std::string& hueLUT::name() const {
    _LTI_RETURN_CLASS_NAME
  }
    
  /*
   * Returns a pointer to a clone of the parameters.
   */
  hueLUT* hueLUT::clone() const {
    return new hueLUT(*this);
  }
    
  /*
   * Returns a pointer to a clone of the parameters.
   */
  hueLUT* hueLUT::newInstance() const {
    return new hueLUT();
  }
}

#include "ltiUndebug.h"
