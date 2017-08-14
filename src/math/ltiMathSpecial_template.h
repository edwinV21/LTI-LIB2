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
 * \file   ltiMath_template.h
 * \author Thomas Rusert
 * \date   28.04.1999
 *
 * $Id: ltiMathSpecial_template.h,v 1.1 2004-07-26 19:20:34 alvarado Exp $
 */

namespace lti {
  // returns the value ln( gamma(x) ) for x>0
  // see for example: Press, Vetterling, Teukolsky, Flannery
  //                  Numerical Recipes in C, 2nd edition
  //                  Cambridge University Press, 1992
  //                  p. 214
  template<class T>
  T lnGamma(const T& xx) {
    double x,y,tmp,ser;
    static const double cof[6]={ 76.18009172947146,
                                -86.50532032941677,
                                 24.01409824083091,
                                 -1.231739572450155,
                                  0.1208650973866179e-2,
                                 -0.5395239384953e-5};
    int j;

    if(!(xx>0)) {
      // return std::numeric_limits<T>::quiet_NaN() 
      return static_cast<T>(NaN); // return an invalid number!
    }

    y = x = xx;
    tmp = x+5.5;
    tmp -= (x+0.5)*log(tmp);
    ser = 1.000000000190015;
    for (j=0;j<=5;j++) {
      ser += cof[j]/++y;
    }
    
    return static_cast<T>(-tmp+log(2.5066282746310005*ser/x));
  }

}

