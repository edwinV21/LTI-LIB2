/*
 * Copyright (C) 2009
 * Pablo Alvarado
 *
 * This file is part of the LTI-Computer Vision Library 2 (LTI-Lib-2)
 *
 * The LTI-Lib-2 is free software; you can redistribute it and/or
 * modify it under the terms of the BSD License.
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the authors nor the names of its contributors may be
 *    used to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */


/**
 * \file   ltiCoordinateSystem.h
 *         Contains the enum eCoordinateSystem which provides global constants
 *         to identify typical coordinate systems like Polar or Cartesic. The
 *         corresponding read and write functions are also defined.
 * \author Pablo Alvarado
 * \date   06.09.2009
 * $Id: ltiCoordinateSystem.cpp,v 1.2 2009-09-07 02:43:16 alvarado Exp $
 */

#include "ltiIoHandler.h"
#include "ltiCoordinateSystem.h"

namespace lti {
  // read function for eCoordinateSystem.
  bool read(ioHandler& handler,eCoordinateSystem& data) {

    std::string str;
    if (handler.read(str)) {
   
      if (str.find("artes") != std::string::npos) {
        data = Cartesian;
      } else if (str.find("olar") != std::string::npos)  {
        data = Polar;
      } else if (str.find("pheric") != std::string::npos) {
        data = Spherical;
      } else if (str.find("ylindric") != std::string::npos) {
        data = Cylindrical;
      } else {
        data = Cartesian;
        handler.setStatusString("undefined eCoordinateSystem");
        return false;
      }

      return true;
    }

    handler.setStatusString("could not read std::string from stream");
    return false;
  }

  // write function for eCoordinateSystem.
  bool write(ioHandler& handler,const eCoordinateSystem& data) {
    bool b=false;
    switch(data) {
    case Cartesian:
      b=handler.write("Cartesian");
      break;
    case Polar:
      b=handler.write("Polar");
      break;
    case Spherical:
      b=handler.write("Spherical");
      break;
    case Cylindrical:
      b=handler.write("Cylindrical");
      break;
    default:
      b=false;
      handler.setStatusString("undefined eCoordinateSystem");
      handler.write("Unknown");
    }

    return b;
  }
  
}
