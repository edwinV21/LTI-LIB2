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


/**
 * \file   ltiAssert.h 
 *         Defines macro assert() as a dummy for release versions but as the
 *         standard assert if a debug version is being compiled.
 * \author Pablo Alvarado
 * \date   09.04.99
 *
 * $Id: ltiAssert.h,v 1.1.1.1 2004-07-23 10:49:36 ltilib Exp $
 */

#ifndef _LTI_ASSERT_H_
#define _LTI_ASSERT_H_

// NDEBUG is a symbol defined in non-debug modes only, which means that
// the symbol assert will be removed except it is a debug mode.
//
// The debug modes define the symbol _NDEBUG

#ifdef NDEBUG
// a release mode or similar
// define the assert macro as nothing
#undef assert
#define assert(x)
#else
// a debug mode or similar
// use the standard assert
#include <cassert>
#endif

#endif

