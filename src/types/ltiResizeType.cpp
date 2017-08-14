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
 * \file   ltiResizeType.cpp
 * \author Peter Doerfler
 * \date   02.04.2003
 * $Id: ltiResizeType.cpp,v 1.2 2004-12-01 09:42:07 doerfler Exp $
 */

#include "ltiResizeType.h"

namespace lti {

  // read function for eResizeType.
  bool read(ioHandler& handler,eResizeType& data) {

    std::string str;
    if (handler.read(str)) {
   
      if ((str == "AllocateOnly") ||
          (str == "allocateonly")) {
        data = AllocateOnly;
      } else if ((str == "Copy") ||
                 (str == "copy")) {
        data = Copy;
      } else if ((str == "Init") ||
                 (str == "init")) {
        data = Init;
      } else if ((str == "CopyAndInit") ||
                 (str == "copyandinit")) {
        data = CopyAndInit;
      } else {
        data = Copy;
        handler.setStatusString("undefined eResizeType");
        return false;
      }

      return true;
    }

    handler.setStatusString("could not read std::string from stream");
    return false;
  }

  // write function for eResizeType.
  bool write(ioHandler& handler,const eResizeType& data) {
    bool b=false;
    switch(data) {
    case AllocateOnly:
      b=handler.write("AllocateOnly");
      break;
    case Copy:
      b=handler.write("Copy");
      break;
    case Init:
      b=handler.write("Init");
      break;
    case CopyAndInit:
      b=handler.write("CopyAndInit");
      break;
    default:
      b=false;
      handler.setStatusString("undefined eResizeType");
      handler.write("Unknown");
    }

    return b;
  }

}
