/*
 * Copyright (C) 2004, 2005, 2006, 2007
 * Arnd Hannemann, Pablo Alvarado
 * RWTH-Aachen, ITCR
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


/*--------------------------------------------------------------------
 * \file   ltiFireWireDCAM.h
 *         Contains a class for grabbing images from FireWire cameras.
 * \author Arnd Hannemann
 * \author Pablo Alvarado
 * \date   21.10.2004
 * $Id: ltiFireWireDCAM.h,v 1.22 2012-01-05 18:32:32 alvarado Exp $
 */

#ifndef _LTI_FIRE_WIRE_DCAM_H_
#define _LTI_FIRE_WIRE_DCAM_H_

#include "ltiHardwareConfig.h"

#if defined _USE_FIRE_WIRE_DCAM

#if defined _LTI_DC1394_VER_1
#  include "ltiFireWireDCAM1.h"
#elif defined _LTI_DC1394_VER_2
#  include "ltiFireWireDCAM2.h"
#endif

#endif

#endif
