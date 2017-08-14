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
 * \file   ltiInterpolationType.cpp
 *         Defines the enumeration type eInterpolationType used in many
 *         places to specify which kind of interpolation should be
 *         employed.
 * \author Pablo Alvarado
 * \date 21.11.2003
 *
 * $Id: ltiInterpolationType.cpp,v 1.1 2005-01-12 13:27:26 doerfler Exp $
 */

#include "ltiInterpolationType.h"

namespace lti {

  /*
   * read function for eInterpolationType.
   *
   * @ingroup gStorable
   */
  bool read(ioHandler& handler,eInterpolationType& data) {

    std::string str;
    if (handler.read(str)) {
   
      if (str.find("earest") != std::string::npos) {
        data = NearestNeighborInterpolation;
      } else if (str.find("ilinear") != std::string::npos) {
        data = BilinearInterpolation;
      } else if (str.find("iquadratic") != std::string::npos) {
        data = BiquadraticInterpolation;
      } else if (str.find("icubic") != std::string::npos) {
        data = BicubicInterpolation;
      } else {
        data = NearestNeighborInterpolation;
        return false;
      }

      return true;
    }

    return false;
  }

  /*
   * write function for eInterpolationType.
   *
   * @ingroup gStorable
   */
  bool write(ioHandler& handler,const eInterpolationType& data) {
    bool b=false;
    switch(data) {
      case NearestNeighborInterpolation:
        b=handler.write("NearestNeighborInterpolation");
        break;
      case BilinearInterpolation:
        b=handler.write("BilinearInterpolation");
        break;
      case BiquadraticInterpolation:
        b=handler.write("BiquadraticInterpolation");
        break;
      case BicubicInterpolation:
        b=handler.write("BicubicInterpolation");
        break;
      default:
        b=handler.write("Unknown");
        b=false;
    }

    return b;
  }

}
