/*
 * Copyright (C) 1998, 1999, 2000, 2001
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
 * \file   ltiTypes.h
 *         This file contains the definitions of the basic types used in the
 *         LTI-Lib.
 *
 *         Some types are redefined to compensate the lack of specification in
 *         the C++ Standard about the byte length of the standard types int,
 *         char, float, etc.  Here an lti::uint32 uses always four bytes,
 *         lti::uint16 uses two bytes and so on.
 *
 *         The defined basic types are:
 *         - lti::ubyte  (1 byte unsigned)
 *         - lti::byte   (1 byte signed)
 *         - lti::uint16 (2 byte unsigned)
 *         - lti::int16  (2 byte signed)
 *         - lti::uint32 (4 byte unsigned)
 *         - lti::int32  (4 byte signed)
 *         - lti::uint64 (8 byte unsigned)
 *         - lti::int64  (8 byte signed)
 *         - lti::sreal  (4 byte floating point)
 *         - lti::dreal  (8 byte floating point)
 *
 * \author Pablo Alvarado
 * \date   31.03.1999
 *
 * $Id: ltiTypes.h,v 1.8 2012-09-16 05:22:59 alvarado Exp $
 */

#ifndef _LTI_TYPES_H
#define _LTI_TYPES_H

#include "ltiConfig.h"

namespace lti {
  /**
   * 8-bit unsigned type (0..255)
   *
   * @ingroup gTypes
   */
  typedef unsigned _LTI_BYTE ubyte;

  /**
   * 8-bit signed type (-128..127)
   *
   * @ingroup gTypes
   */
  typedef signed _LTI_BYTE byte;

  /**
   * 16-bit unsigned type (0..65535)
   *
   * @ingroup gTypes
   */
  typedef unsigned _LTI_WORD16 uint16;

  /**
   * 16-bit signed type (-32768..32767)
   *
   * @ingroup gTypes
   */
  typedef signed _LTI_WORD16 int16;

  /**
   * 32-bit unsigned type (0..(2^32)-1)
   *
   * @ingroup gTypes
   */
  typedef unsigned _LTI_WORD32 uint32;

  /**
   * 32-bit signed type (-(2^31)..(2^31-1))
   *
   * @ingroup gTypes
   */
  typedef signed _LTI_WORD32 int32;

  /**
   * 64-bit unsigned type (0..(2^64)-1)
   *
   * @ingroup gTypes
   */
  __extension__ typedef unsigned _LTI_WORD64 uint64;

  /**
   * 64-bit signed type (-(2^63)..(2^63-1))
   *
   * @ingroup gTypes
   */
  __extension__ typedef signed _LTI_WORD64 int64;

  /**
   * 32-bit floating point type
   *
   * @ingroup gTypes
   */
  typedef float sreal;

  /**
   * 64-bit floating point type
   *
   * @ingroup gTypes
   */
  typedef double dreal;

  /**
   * Minimal int32 value achievable using only 24bits
   */
  const int32 MinInt24 = -0x00800000;

  /**
   * Maximal int32 value achievable using only 24bits
   */
  const int32 MaxInt24 = +0x007fffff;

}

#endif

