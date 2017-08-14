/*
 * Copyright (C) 2008
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
 * \file   ltiV4l2_patch.h
 *         Contains a patch for older videodev2.h files (the interface API
 *         for Video4Linux2), that allows this class to compile.
 *         Users should not include this file EVER.
 *         It has to be included AFTER videodev2.h has been loaded
 * \author Pablo Alvarado
 * \date   05.10.2008
 *
 * revisions ..: $Id: ltiV4l2_patch.h,v 1.1 2010-09-09 02:13:17 alvarado Exp $
 */

#ifndef _LTI_V4L2_PATCH_H_
#define _LTI_V4L2_PATCH_H_

#include "ltiHardwareConfig.h"

#ifdef _USE_V4L2
#if __linux__

#include <asm/types.h>          /* for videodev2.h */
#include <linux/videodev2.h>

// Older versions lack some symbols we use, so let us define them here
#undef _LTI_V4L2_OLD

#ifndef V4L2_CTRL_CLASS_CAMERA // our arbitrarilly chosen flag


#define _LTI_V4L2_OLD 1 // flag to eliminate some unavailable code in
                        // old kernels

// these became part of the  v4l2_field  enum, so create them as macros 
#define V4L2_FIELD_INTERLACED_TB = 8
#define V4L2_FIELD_INTERLACED_BT = 9

#define V4L2_PIX_FMT_RGB444  v4l2_fourcc('R','4','4','4')
#define V4L2_PIX_FMT_Y16     v4l2_fourcc('Y','1','6',' ')
#define V4L2_PIX_FMT_PAL8    v4l2_fourcc('P','A','L','8')
#define V4L2_PIX_FMT_YUV444  v4l2_fourcc('Y','4','4','4')
#define V4L2_PIX_FMT_YUV555  v4l2_fourcc('Y','U','V','O')
#define V4L2_PIX_FMT_YUV565  v4l2_fourcc('Y','U','V','P')
#define V4L2_PIX_FMT_YUV32   v4l2_fourcc('Y','U','V','4')
#define V4L2_PIX_FMT_HM12    v4l2_fourcc('H','M','1','2')
#define V4L2_PIX_FMT_SBGGR16 v4l2_fourcc('B','Y','R','2')

/* Experimental Frame Size and frame rate enumeration */
/*
 *	F R A M E   S I Z E   E N U M E R A T I O N
 */
enum v4l2_frmsizetypes {
  V4L2_FRMSIZE_TYPE_DISCRETE	= 1,
  V4L2_FRMSIZE_TYPE_CONTINUOUS	= 2,
  V4L2_FRMSIZE_TYPE_STEPWISE	= 3
};

struct v4l2_frmsize_discrete {
  __u32			width;	      /* Frame width [pixel] */
  __u32			height;	      /* Frame height [pixel] */
};

struct v4l2_frmsize_stepwise {
  __u32			min_width;  /* Minimum frame width [pixel] */
  __u32			max_width;  /* Maximum frame width [pixel] */
  __u32			step_width; /* Frame width step size [pixel] */
  __u32			min_height; /* Minimum frame height [pixel] */
  __u32			max_height; /* Maximum frame height [pixel] */
  __u32			step_height;/* Frame height step size [pixel]*/
};

struct v4l2_frmsizeenum {
  __u32			index;		/* Frame size number */
  __u32			pixel_format;	/* Pixel format */
  __u32			type; /* Frame size type the device supports.*/
  
  union {					/* Frame size */
    struct v4l2_frmsize_discrete	discrete;
    struct v4l2_frmsize_stepwise	stepwise;
  };
  
  __u32   reserved[2];			/* Reserved space for future use */
};

/*
 *	F R A M E   R A T E   E N U M E R A T I O N
 */
enum v4l2_frmivaltypes {
  V4L2_FRMIVAL_TYPE_DISCRETE	= 1,
  V4L2_FRMIVAL_TYPE_CONTINUOUS	= 2,
  V4L2_FRMIVAL_TYPE_STEPWISE	= 3
};

struct v4l2_frmival_stepwise {
  struct v4l2_fract	min;		/* Minimum frame interval [s] */
  struct v4l2_fract	max;		/* Maximum frame interval [s] */
  struct v4l2_fract	step;		/* Frame interval step size [s] */
};

struct v4l2_frmivalenum {
  __u32			index;		/* Frame format index */
  __u32			pixel_format;	/* Pixel format */
  __u32			width;		/* Frame width */
  __u32			height;		/* Frame height */
  __u32			type;	/* Frame interval type the device supports. */
  
  union {					/* Frame interval */
    struct v4l2_fract		discrete;
    struct v4l2_frmival_stepwise	stepwise;
  };
  
  __u32	reserved[2];			/* Reserved space for future use */
};

#define V4L2_CTRL_CLASS_CAMERA 0x009a0000	/* Camera class controls */

#define V4L2_CID_POWER_LINE_FREQUENCY	(V4L2_CID_BASE+24)
enum v4l2_power_line_frequency {
  V4L2_CID_POWER_LINE_FREQUENCY_DISABLED	= 0,
  V4L2_CID_POWER_LINE_FREQUENCY_50HZ	= 1,
  V4L2_CID_POWER_LINE_FREQUENCY_60HZ	= 2
};
#define V4L2_CID_HUE_AUTO			(V4L2_CID_BASE+25)
#define V4L2_CID_WHITE_BALANCE_TEMPERATURE	(V4L2_CID_BASE+26)
#define V4L2_CID_SHARPNESS			(V4L2_CID_BASE+27)
#define V4L2_CID_BACKLIGHT_COMPENSATION 	(V4L2_CID_BASE+28)
#define V4L2_CID_CHROMA_AGC                     (V4L2_CID_BASE+29)
#define V4L2_CID_COLOR_KILLER                   (V4L2_CID_BASE+30)
/* last CID + 1 */
#undef V4L2_CID_LASTP1 // the last LAST was not LAST at all!
#define V4L2_CID_LASTP1                         (V4L2_CID_BASE+31)

/*  Camera class control IDs */
#define V4L2_CID_CAMERA_CLASS_BASE 	(V4L2_CTRL_CLASS_CAMERA | 0x900)
#define V4L2_CID_CAMERA_CLASS 		(V4L2_CTRL_CLASS_CAMERA | 1)

#define V4L2_CID_EXPOSURE_AUTO			(V4L2_CID_CAMERA_CLASS_BASE+1)
enum  v4l2_exposure_auto_type {
	V4L2_EXPOSURE_AUTO = 0,
	V4L2_EXPOSURE_MANUAL = 1,
	V4L2_EXPOSURE_SHUTTER_PRIORITY = 2,
	V4L2_EXPOSURE_APERTURE_PRIORITY = 3
};
#define V4L2_CID_EXPOSURE_ABSOLUTE		(V4L2_CID_CAMERA_CLASS_BASE+2)
#define V4L2_CID_EXPOSURE_AUTO_PRIORITY		(V4L2_CID_CAMERA_CLASS_BASE+3)

#define V4L2_CID_PAN_RELATIVE			(V4L2_CID_CAMERA_CLASS_BASE+4)
#define V4L2_CID_TILT_RELATIVE			(V4L2_CID_CAMERA_CLASS_BASE+5)
#define V4L2_CID_PAN_RESET			(V4L2_CID_CAMERA_CLASS_BASE+6)
#define V4L2_CID_TILT_RESET			(V4L2_CID_CAMERA_CLASS_BASE+7)

#define V4L2_CID_PAN_ABSOLUTE			(V4L2_CID_CAMERA_CLASS_BASE+8)
#define V4L2_CID_TILT_ABSOLUTE			(V4L2_CID_CAMERA_CLASS_BASE+9)

#define V4L2_CID_FOCUS_ABSOLUTE			(V4L2_CID_CAMERA_CLASS_BASE+10)
#define V4L2_CID_FOCUS_RELATIVE			(V4L2_CID_CAMERA_CLASS_BASE+11)
#define V4L2_CID_FOCUS_AUTO			(V4L2_CID_CAMERA_CLASS_BASE+12)

#define VIDIOC_ENUM_FRAMESIZES	_IOWR ('V', 74, struct v4l2_frmsizeenum)
#define VIDIOC_ENUM_FRAMEINTERVALS _IOWR ('V', 75, struct v4l2_frmivalenum)
#define VIDIOC_G_ENC_INDEX      _IOR  ('V', 76, struct v4l2_enc_idx)
#define VIDIOC_ENCODER_CMD      _IOWR ('V', 77, struct v4l2_encoder_cmd)
#define VIDIOC_TRY_ENCODER_CMD  _IOWR ('V', 78, struct v4l2_encoder_cmd)



#endif // V4L2_PIX_FMT_RGB444
#endif // __linux__
#endif // _USE_V4L2
#endif // #ifdef _LTI_V4L2_PATCH_H_
