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
 * \file   ltiMedianEvenCase.cpp
 *         Contains the enum eMedianCase which is used in
 *         lti::quickMedian and related classes. Read/write functions
 *         are available.
 * \author Peter Doerfler
 * \date   23.03.2005
 *
 * $Id: ltiMedianEvenCase.cpp,v 1.1 2005-03-29 13:26:46 doerfler Exp $
 */

#include "ltiMedianEvenCase.h"

namespace lti {

  // read function for eMedianEvenCase.
  bool read(ioHandler& handler,eMedianEvenCase& data) {

    std::string str;
    if (handler.read(str)) {
   
      if ((str == "TakeLower") ||
          (str == "takelower") ||
          (str == "takeLower")) {
        data = TakeLower;
      } else if ((str == "TakeHigher") ||
                 (str == "takehigher") ||
                 (str == "takeHigher")) {
        data = TakeHigher;
      } else {
        data = TakeLower;
        handler.setStatusString("undefined eMedianEvenCase");
        return false;
      }

      return true;
    }

    handler.setStatusString("could not read std::string from stream");
    return false;
  }

  // write function for eMedianEvenCase.
  bool write(ioHandler& handler,const eMedianEvenCase& data) {
    bool b=false;
    switch(data) {
    case TakeLower:
      b=handler.write("TakeLower");
      break;
    case TakeHigher:
      b=handler.write("TakeHigher");
      break;
    default:
      b=false;
      handler.setStatusString("undefined eMedianEvenCase");
      handler.write("Unknown");
    }

    return b;
  }

}
