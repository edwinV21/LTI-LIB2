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
 * \file   ltiCamera.cpp
 *         Contains an abstract parent class for all camera classes.
 * \author Pablo Alvarado
 * \date   16.04.2007
 * $Id: ltiCamera.cpp,v 1.2 2007-04-30 03:22:13 alvarado Exp $
 */

#include "ltiCamera.h"

namespace lti {

  // ------------------------------------------------------------------
  // camera::parameters
  // ------------------------------------------------------------------

  /*
   * Default constructor
   */
  camera::parameters::parameters() {
    resolution.set(640,400);
    framerate=15.0f;
    brightnessMode=FeatureAuto;
    brightness=0;
    exposureMode=FeatureAuto;
    exposure=0;
    sharpnessMode=FeatureAuto;
    sharpness=0;
    whiteBalanceMode=FeatureAuto;
    redGain=0;
    blueGain=0;
    hueMode=FeatureAuto;
    hue=0;
    saturationMode=FeatureAuto;
    saturation=0;
    gammaMode=FeatureAuto;
    gamma=0;
    shutterMode=FeatureAuto;
    shutter=0;
    gainMode=FeatureAuto;
    gain=0;
    waitOnePush=false;
    onePushUpdateRate=2;
    onePushTimeout=10;
  }

  /*
   * Copy constructor
   * @param other the parameters object to be copied
   */
  camera::parameters::parameters(const parameters& other) {
    copy(other);
  }
  
  /*
   * Destructor
   */
  camera::parameters::~parameters() {
  }

  /*
   * Copy the contents of a parameters object
   * @param other the parameters object to be copied
   * @return a reference to this parameters object
   */
  camera::parameters& camera::parameters::copy(const parameters& other) {
    resolution.copy(other.resolution);
    framerate=other.framerate;

    brightnessMode=other.brightnessMode;
    brightness=other.brightness;

    exposureMode=other.exposureMode;
    exposure=other.exposure;

    sharpnessMode=other.sharpnessMode;
    sharpness=other.sharpness;

    whiteBalanceMode=other.whiteBalanceMode;
    redGain=other.redGain;
    blueGain=other.blueGain;

    hueMode=other.hueMode;
    hue=other.hue;

    saturationMode=other.saturationMode;
    saturation=other.saturation;

    gammaMode=other.gammaMode;
    gamma=other.gamma;

    shutterMode=other.shutterMode;
    shutter=other.shutter;

    gainMode=other.gainMode;
    gain=other.gain;

    waitOnePush=other.waitOnePush;
    onePushUpdateRate=other.onePushUpdateRate;
    onePushTimeout=other.onePushTimeout;

    return *this;
  }

  /*
   * Copy the contents of a parameters object
   * @param other the parameters object to be copied
   * @return a reference to this parameters object
   */
  camera::parameters& camera::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  /*
   * Write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool camera::parameters::write(ioHandler& handler,const bool complete) const{
    bool b=true;

    if (complete) {
      b=handler.writeBegin();
    }

    b = b && lti::write(handler,"resolution",resolution);
    b = b && lti::write(handler,"framerate",framerate);
    b = b && lti::write(handler,"brightnessMode",brightnessMode);
    b = b && lti::write(handler,"brightness",brightness);
    b = b && lti::write(handler,"exposureMode",exposureMode);
    b = b && lti::write(handler,"exposure",exposure);
    b = b && lti::write(handler,"sharpnessMode",sharpnessMode);
    b = b && lti::write(handler,"sharpness",sharpness);
    b = b && lti::write(handler,"whiteBalanceMode",whiteBalanceMode);
    b = b && lti::write(handler,"redGain",redGain);
    b = b && lti::write(handler,"blueGain",blueGain);
    b = b && lti::write(handler,"hueMode",hueMode);
    b = b && lti::write(handler,"hue",hue);
    b = b && lti::write(handler,"saturationMode",saturationMode);
    b = b && lti::write(handler,"saturation",saturation);
    b = b && lti::write(handler,"gammaMode",gammaMode);
    b = b && lti::write(handler,"gamma",gamma);
    b = b && lti::write(handler,"shutterMode",shutterMode);
    b = b && lti::write(handler,"shutter",shutter);
    b = b && lti::write(handler,"gainMode",gainMode);
    b = b && lti::write(handler,"gain",gain);
    b = b && lti::write(handler,"waitOnePush",waitOnePush);
    b = b && lti::write(handler,"onePushUpdateRate",onePushUpdateRate);
    b = b && lti::write(handler,"onePushTimeout",onePushTimeout);
    
    if (complete) {
      b=b&&handler.writeEnd();
    }
    
    return b;
  }

  /*
   * Read the parameters from the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool camera::parameters::read(ioHandler& handler,const bool complete) {
    bool b=true;

    if (complete) {
      b=handler.readBegin();
    }

    b = b && lti::read(handler,"resolution",resolution);
    b = b && lti::read(handler,"framerate",framerate);
    b = b && lti::read(handler,"brightnessMode",brightnessMode);
    b = b && lti::read(handler,"brightness",brightness);
    b = b && lti::read(handler,"exposureMode",exposureMode);
    b = b && lti::read(handler,"exposure",exposure);
    b = b && lti::read(handler,"sharpnessMode",sharpnessMode);
    b = b && lti::read(handler,"sharpness",sharpness);
    b = b && lti::read(handler,"whiteBalanceMode",whiteBalanceMode);
    b = b && lti::read(handler,"redGain",redGain);
    b = b && lti::read(handler,"blueGain",blueGain);
    b = b && lti::read(handler,"hueMode",hueMode);
    b = b && lti::read(handler,"hue",hue);
    b = b && lti::read(handler,"saturationMode",saturationMode);
    b = b && lti::read(handler,"saturation",saturation);
    b = b && lti::read(handler,"gammaMode",gammaMode);
    b = b && lti::read(handler,"gamma",gamma);
    b = b && lti::read(handler,"shutterMode",shutterMode);
    b = b && lti::read(handler,"shutter",shutter);
    b = b && lti::read(handler,"gainMode",gainMode);
    b = b && lti::read(handler,"gain",gain);
    b = b && lti::read(handler,"waitOnePush",waitOnePush);
    b = b && lti::read(handler,"onePushUpdateRate",onePushUpdateRate);
    b = b && lti::read(handler,"onePushTimeout",onePushTimeout);
    
    if (complete) {
      b=b&&handler.readEnd();
    }
    
    return b;
  }

  // ------------------------------------------------------------------
  // camera
  // ------------------------------------------------------------------

  
  /*
   * Default constructor
   *
   * You usually avoid using this constructor.  Using camera(const
   * parameters&) you can provide directly for which camera the constructed
   * instance should work.
   */
  camera::camera() {
  }

  camera::~camera() {
  }

  // some aliases for exposure
  bool camera::setContrast(const eCamFeatureMode mode,const float contrast) {
    return setExposure(mode,contrast);
  }

  bool camera::getContrast(eCamFeatureMode& mode,float& contrast) const {
    return getExposure(mode,contrast);
  }

  bool camera::checkFeatureValueRange(const unsigned int featureId,
                                            finterval& range) const {
    return checkFeatureValueRange(featureId,range.from,range.to);
  }
}

