/*
 * Copyright (C) 2007
 * Pablo Alvarado
 * Electronics Engineering School, ITCR, Costa Rica
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
 * \file   ltiDeprecated.h
 *         Defines a macro that alerts on deprecated member functions
 * \author Pablo Alvarado
 * \date   04.04.2007
 *
 * $Id: ltiDeprecated.h,v 1.1 2007-04-05 22:49:08 alvarado Exp $
 */

#ifndef _LTI_DEPRECATED_H_
#define _LTI_DEPRECATED_H_

#include "ltiMacroSymbols.h"

#undef _LTI_DEPRECATED
#if defined(_LTI_GNU_C)
#  define _LTI_DEPRECATED __attribute__ ((__deprecated__));
#else
#  define _LTI_DEPRECATED
#endif
#endif
