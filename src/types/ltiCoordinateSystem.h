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
 * $Id: ltiCoordinateSystem.h,v 1.2 2009-10-16 19:39:51 alvarado Exp $
 */

#ifndef _LTI_COORDINATE_SYSTEM_H_
#define _LTI_COORDINATE_SYSTEM_H_

#include "ltiIoHandler.h"

namespace lti {

  /**
   * eCoordinateSystem
   *
   *
   * The coordinate system type defines different type of coordinate systems
   * for the operations that need this indication, like the lti::fft or
   * lti::ifft functors.  Not all values are valid in all applications, but
   * the documentation should explain what is done with the invalid ones.
   *
   * For this type the methods read(ioHandler&, eCoordinateSystem&) and
   * write(ioHandler&, const eCoordinateSystem&) are implemented. Note
   * that template functions read(ioHandler&, const std::string&, T&)
   * and write(ioHandler&, const std::string&, const T&) exist. (see
   * Developer's Guide for further information).
   *
   * @ingroup gTypes
   */
  enum eCoordinateSystem {
    Cartesian,  /**< Cartesian coordinates.  */
    Polar,      /**< Polar coordinates, implying 2D magnitude and angle. */
    Cylindrical,/**< Cylindrical coordinates imply radius, angle and height */
    Spherical   /**< Spherical coordinates imply two angles and a radius */
  };
  
  /**
   * Read function for eCoordinateSystem.
   *
   * @ingroup gStorable
   */
  bool read(ioHandler& handler,eCoordinateSystem& data);

  /**
   * Write function for eCoordinateSystem.
   *
   * @ingroup gStorable
   */
  bool write(ioHandler& handler,const eCoordinateSystem& data);

  
}
#endif
