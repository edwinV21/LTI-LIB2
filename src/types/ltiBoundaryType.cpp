/*
 * Copyright (C) 1998 - 2004
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
 * \file   ltiBoundaryType.cpp
 * \author Peter Doerfler
 * \date   02.04.2003
 * $Id: ltiBoundaryType.cpp,v 1.1 2004-08-06 09:23:30 doerfler Exp $
 */

#include "ltiBoundaryType.h"

namespace lti {

  // read function for eBoundaryType.
  bool read(ioHandler& handler,eBoundaryType& data) {

    std::string str;
    if (handler.read(str)) {
   
      if ((str == "mirror") ||
          (str == "Mirror")) {
        data = Mirror;
      } else if ((str == "periodic") ||
                 (str == "Periodic")) {
        data = Periodic;
      } else if ((str == "constant") ||
                 (str == "Constant")) {
        data = Constant;
      } else if ((str == "zero") ||
                 (str == "Zero")) {
        data = Zero;
      } else if ((str == "noBoundary") ||
                 (str == "NoBoundary")) {
        data = NoBoundary;
      } else {
        data = Zero;
        handler.setStatusString("undefined eBoundaryType");
        return false;
      }

      return true;
    }

    handler.setStatusString("could not read std::string from stream");
    return false;
  }

  // write function for eBoundaryType.
  bool write(ioHandler& handler,const eBoundaryType& data) {
    bool b=false;
    switch(data) {
    case Mirror:
      b=handler.write("Mirror");
      break;
    case Periodic:
      b=handler.write("Periodic");
      break;
    case Constant:
      b=handler.write("Constant");
      break;
    case Zero:
      b=handler.write("Zero");
      break;
    case NoBoundary:
      b=handler.write("NoBoundary");
      break;
    default:
      b=false;
      handler.setStatusString("undefined eBoundaryType");
      handler.write("Unknown");
    }

    return b;
  }

}
