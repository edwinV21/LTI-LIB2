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
 * \file   ltiHardwareConfig.h
 *         Contains a few macros that can be used for including
 *         cameras and framegrabbers. On Linux systems using
 *         extrapaths.mk is preferred.
 *
 *         This file contains definition macros used as flags to
 *         compile hardware related functors like frame grabbers.
 *         Usually these functors check if the corresponding symbol
 *         is defined, and only in that cases the class will be
 *         compiled.
 *
 *         If you work with Linux, GCC and GNU-Make, you can
 *         optionally leave this file untouched and provide in the
 *         corresponding symbol using a -D directive in the file
 *         extrapaths.mk.
 *
 *         For example, to force the use of the phillips webcams, your
 *         extrapaths.mk file would look (note that this is just an
 *         example, since this paticular case is now detected
 *         automatically:
 *
 *         \code
 *           EXTRACXXFLAGS=-D_USE_PWC
 *           EXTRAINCLUDEPATH=
 *           EXTRALIBPATH=
 *           EXTRALIBS=
 *        \endcode
 *
 *        @ingroup gAcquisition
 *
 * \author Pablo Alvarado
 * \date   24.11.1999
 *
 * $Id: ltiHardwareConfig.h
 */

#ifndef _LTI_HARDWARE_CONFIG_H_
#define _LTI_HARDWARE_CONFIG_H_

#include "ltiConfig.h"

// use QuickCam
// #define _USE_QUICKCAM_GRABBER 1

// use Imaging Frame Grabber
// #define _USE_ITI_FRAME_GRABBER 1

// use Matrix Vision Frame Grabber
// #define _USE_MATRIX_VISION_GRABBER 1

// use SiliconSoftware Micro Enable with Channel-Link Frame Grabber
// #define _USE_MICRO_ENABLE_FRAME_GRABBER 1

// use leutron frame grabber
// #define _USE_LEUTRON_GRABBER

#if defined HAVE_LIBDC1394_CONTROL
#  define _USE_FIRE_WIRE_DCAM
#  define _LTI_DC1394_VER_1
#elif defined HAVE_LIBDC1394
#  define _USE_FIRE_WIRE_DCAM
#  define _LTI_DC1394_VER_2
#endif

#if defined HAVE_PWC_IOCTL_H
// use Philips Web Cam (PWC)
#  define _USE_PWC 1
#endif

#if (defined HAVE_LINUX_VIDEODEV2_H) && (defined HAVE_LIBV4L2)
// use Video4Linux2
#  define _USE_V4L2 1
#endif

#endif  // _LTI_HARDWARE_CONFIG_H_
