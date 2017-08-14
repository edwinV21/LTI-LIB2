/*
 * Copyright (C) 2006
 * Pablo Alvarado, Instituto Tecnológico de Costa Rica
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
 * \file   ltiConstantReferenceType.cpp
 * \author Pablo Alvarado
 * \date   09.09.2006
 * $Id: ltiConstantReferenceType.cpp,v 1.1 2007-04-05 22:16:42 alvarado Exp $
 */

#include "ltiConstantReferenceType.h"

namespace lti {

  // read function for eConstantReference.
  bool read(ioHandler& handler,eConstantReference& data) {

    std::string str;
    if (handler.read(str)) {
   
      if (str.find("Constant") != std::string::npos) {
        data = ConstantReference;
      } else {
        data = VariableReference;
      }

      return true;
    }

    handler.setStatusString("could not read std::string from stream");
    return false;
  }

  // write function for eConstantReference.
  bool write(ioHandler& handler,const eConstantReference& data) {
    bool b=false;

    if (data == ConstantReference) {
      b=handler.write("ConstantReference");
    } else {
      b=handler.write("VariableReference");
    }

    return b;
  }

}
