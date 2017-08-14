/*
 * Copyright (C) 2007
 * Pablo Alvarado, ITCR
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
 * \file   ltiCamFeatureMode.h
 *         Type used to indicate the operation mode of camera
 *         features, like manual, automatic, off, not-available, etc.
 * \author Pablo Alvarado
 * \date   16.04.2007
 * $Id: ltiCamFeatureMode.h,v 1.2 2007-09-14 17:18:11 alvarado Exp $
 */


#ifndef _LTI_CAM_FEATURE_MODE_H
#define _LTI_CAM_FEATURE_MODE_H

#include "ltiIoHandler.h"

namespace lti {

  /**
   * Feature mode.
   *
   * The following values are used in two ways by the camera features
   * derived from lti::camera: first, they can be used to set the
   * operation mode of a feature to one of the given values; second,
   * they can be used to check the availability of different
   * operational modes for a feature.  For this last task you can use
   * bitwise operators to check the availability of features.  The
   * origin of this type is the IIDC standard of FireWire cameras, but
   * the idea can be applied to many other hardware types, providing a
   * unified way of interfacing with the hardware.
   *
   * The available "modes" or "states" (as called in the IIDC standard) are:
   *
   * - The FeatureOff can under circumstances be used to check if a
   *   feature can be turned off.  The use of any other state implies that
   *   the feature is on.
   * - The FeatureOnePush activates the "one-push" adjustment when the
   *   functor parameters are updated (setParameters() activates the
   *   adjustment too, of course), or when one specific method for
   *   direct camera control (like setBrightness()) is used.
   * - The FeatureAbsolute is identical to FeatureManual, except that,
   *   if a camera supports it, the values are given in certain units
   *   specified by the standard, like \b degrees for hue, pan and tilt,
   *   \b percentage for brightness and saturation, \b meters for focus,
   *   and so on.
   * - FeatureNA is used to indicate that a feature is not available or
   *   that the user wants this functor not to touch the configuration of
   *   a given feature.
   *
   * @ingroup gAcquisition
   */
  enum eCamFeatureMode {
    FeatureNA=0,       /**< Feature not available */
    FeatureOff=1,      /**< Feature is deactivated */
    FeatureAuto=2,     /**< Feature adjustments are taken automatically */
    FeatureManual=4,   /**< Feature adjustments are done by the user */
    FeatureOnePush=8,  /**< Feature is automatically adjusted but only once */
    FeatureAbsolute=16 /**< Feature is manually controled but using absolute
                            values */
  };

  /**
   * Read function for eResizeType.
   *
   * @ingroup gStorable
   */
  bool read(ioHandler& handler,eCamFeatureMode& data);

  /**
   * Write function for eResizeType.
   *
   * @ingroup gStorable
   */
  bool write(ioHandler& handler,const eCamFeatureMode& data);  

} // namespace lti
#endif
