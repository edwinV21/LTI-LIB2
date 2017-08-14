/*
 * Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006
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
 * \file   ltiPwc.cpp
 *         Grab images from a Philips Webcam using the pwc module.
 * \author Pablo Alvarado
 * \date   24.04.2001
 *
 * $Id: ltiPwc.cpp,v 1.16 2009-04-20 14:59:14 alvarado Exp $
 */

#include "ltiHardwareConfig.h"
#ifdef _USE_PWC
#ifdef __linux__

// C-stuff required for the device control

#include <sys/time.h>
#include <sys/stat.h>

#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include "ltiTypes.h"

// these types are required by <linux/videodev.h> but some
// macro jungle may avoid their definition, so just use some
// configure trick to check if they can be obtained from <linux/types.h>
// or otherwise define them here
#if !defined (HAVE_U64_V4L)
typedef unsigned long ulong;
typedef lti::uint64 __u64;
typedef lti::int64 __s64;
#endif

#include <linux/videodev.h>
#include <pwc-ioctl.h>

#include <cstdio>
#include <unistd.h>
#include <errno.h>
#include <cstring>

#include "ltiPwc.h"
#include "ltiMath.h"
#include "ltiRound.h"
#include "ltiPassiveWait.h"

#undef _LTI_DEBUG
//#define _LTI_DEBUG 1
#include "ltiDebug.h"

namespace lti {

  // -----------------------------------------------------------------
  // io-methods for enumerations
  // -----------------------------------------------------------------

  /*
   * Write pwc::eCompressionType
   */
  bool write(ioHandler& handler,const pwc::eCompressionType val) {
    switch(val) {
    case pwc::None:
      return handler.write("None");
      break;
    case pwc::Low:
      return handler.write("Low");
      break;
    case pwc::Medium:
      return handler.write("Medium");
      break;
    case pwc::High:
      return handler.write("High");
      break;
    default:
      return handler.write("Medium");
    }
    return false;
  }

  /*
   * Read pwc::eCompressionType
   */
  bool read(ioHandler& handler,pwc::eCompressionType& compression) {
    std::string tmp;
    if (handler.read(tmp)) {
      if (tmp == "None") {
        compression = pwc::None;
      } else if (tmp == "Low") {
        compression = pwc::Low;
      } else if (tmp == "Medium") {
        compression = pwc::Medium;
      } else if (tmp == "High") {
        compression = pwc::High;
      } else {
        compression = pwc::Medium; // default value
        
        std::string str("Unknown symbol ");
        str+=tmp;
        handler.setStatusString(str);
        
        return false;
      }
      return true;
    }
    return false;
  }
  
  /*
   * Write pwc::eWhiteBalanceType
   */
  bool write(ioHandler& handler,const pwc::eWhiteBalanceType val) {
    switch(val) {
    case pwc::Indoor:
      return handler.write("Indoor");
      break;
    case pwc::Outdoor:
      return handler.write("Outdoor");
      break;
    case pwc::Fluorescent:
      return handler.write("Fluorescent");
      break;
    case pwc::Auto:
      return handler.write("Auto");
      break;
    case pwc::InvalidWB:
      return handler.write("InvalidWB");
      break;
    default:
      return handler.write("InvalidWB");
    }
    return false;
  }

  /*
   * Read pwc::eWhiteBalanceType
   */
  bool read(ioHandler& handler,pwc::eWhiteBalanceType& whiteBalance) {
    std::string tmp;
    if (handler.read(tmp)) {
      if (tmp == "Indoor") {
        whiteBalance = pwc::Indoor;
      } else if (tmp == "Outdoor") {
        whiteBalance = pwc::Outdoor;
      } else if (tmp == "Fluorescent") {
        whiteBalance = pwc::Fluorescent;
      } else if (tmp == "Auto") {
        whiteBalance = pwc::Auto;
      } else {
        whiteBalance = pwc::InvalidWB;
        std::string str("Unknown symbol ");
        str+=tmp;
        handler.setStatusString(str);
        return false;
      }
      return true;
    }
    return false;
  }
  
  /*
   * Write pwc::eDenoiseType
   */
  bool write(ioHandler& handler,const pwc::eDenoiseType val) {
    switch(val) {
    case pwc::NoDenoise:
      return handler.write("NoDenoise");
      break;
    case pwc::LowDenoise:
      return handler.write("LowDenoise");
      break;
    case pwc::MedDenoise:
      return handler.write("MedDenoise");
      break;
    case pwc::HighDenoise:
      return handler.write("HighDenoise");
      break;
    default:
      return handler.write("InvalidDenoise");
    }
    return false;
  }

  /*
   * Read pwc::eDenoiseType
   */
  bool read(ioHandler& handler,pwc::eDenoiseType& val) {
    std::string str;
    if (handler.read(str)) {
      if (str == "NoDenoise") {
        val = pwc::NoDenoise;
      } else if (str.find("ow") != std::string::npos) {
        val = pwc::LowDenoise;
      } else if (str.find("ed") != std::string::npos) {
        val = pwc::MedDenoise;
      } else if (str.find("igh") != std::string::npos) {
        val = pwc::HighDenoise;
      } else {
        val = pwc::InvalidDenoise;
        std::string tmp("Unknown symbol ");
        tmp+=str;
        handler.setStatusString(str);
        return false;
      }
      return true;
    } 

    return false;
  }

  // -----------------------------------------------------------------
  // pwc static members
  // -----------------------------------------------------------------
  /*
   * look up table to accelerate calculate saturation
   */
  const ubyte* pwc::lutSaturation_ = 0;

  /*
   * look up tables to accelerate conversion Luv -> RGB
   */
  const int* pwc::lutUg_ = 0;
  const int* pwc::lutUb_ = 0;
  const int* pwc::lutVg_ = 0;
  const int* pwc::lutVr_ = 0;

  // -----------------------------------------------------------------
  //  pwc::parameters
  // -----------------------------------------------------------------

  pwc::parameters::parameters(const bool valid)
    : frameGrabber::parameters(), 
      camera::parameters(), 
      panTiltUnit::parameters() {
    

    if (valid) {
      brightnessMode = FeatureManual;
      contrastMode =  FeatureManual;
      sharpnessMode = FeatureManual;
      hueMode = FeatureManual;
      saturationMode = FeatureManual;
      gammaMode = FeatureManual;
      shutterMode = FeatureAuto;
      gainMode = FeatureAuto;
      whiteBalanceMode = FeatureAuto;

      resolution = ipoint(320,240);
      framerate = 30;

      brightness = 0.5;
      contrast = 0.5;
      sharpness = 0.0;


      redGain = 0.5;
      blueGain= 0.5;

      hue = 0.5;
      saturation = 0.5;
      gamma = 0.5;
      shutter = 0.75;
      gain = 0.75;

      snapShotMode = false;

      compression = High;

      whiteBalance = Outdoor;
      whiteBalanceControlSpeed = 0.5;
      whiteBalanceControlDelay = 0.5;


      led = 0; // always off
      backlightCompensation = false;
      antiflickerMode = false;
      denoise = NoDenoise;
      
      deviceFile = "/dev/video0";

    } else {
      // initialize all last used parameters with invalid values to force
      // setting the correct parameters the first time setParameters is called

      brightnessMode = FeatureNA;
      contrastMode =  FeatureNA;
      sharpnessMode = FeatureNA;
      hueMode = FeatureNA;
      saturationMode = FeatureNA;
      gammaMode = FeatureNA;
      shutterMode = FeatureNA;
      gainMode = FeatureNA;
      whiteBalanceMode = FeatureNA;

      resolution = ipoint(-1,-1);
      framerate = -1;

      brightness = -1;
      contrast = -1;
      sharpness = -1;
      

      redGain = -1;
      blueGain = -1;

      hue = -1;
      saturation = -1;
      gamma = -1;
      shutter = -1;
      gain = -1;

      snapShotMode = false;
      compression = Invalid;

      whiteBalance = InvalidWB;
      whiteBalanceControlSpeed = -1;
      whiteBalanceControlDelay = -1;

      led = -1;
      backlightCompensation = false;
      antiflickerMode = false;
      denoise = InvalidDenoise;
      
      deviceFile = "/dev/video0";
    }
  }

  pwc::parameters::parameters(const parameters& other) 
    : frameGrabber::parameters(), 
      camera::parameters(), 
      panTiltUnit::parameters() {
    copy(other);
  }

  pwc::parameters* pwc::parameters::clone() const {
    return ( new parameters(*this) );
  }

  pwc::parameters* pwc::parameters::newInstance() const {
    return ( new parameters );
  }

  // returns the name of this class
  const std::string& pwc::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  pwc::parameters&
  pwc::parameters::copy(const parameters& other ) {
    frameGrabber::parameters::copy(other);
    camera::parameters::copy(other);
    panTiltUnit::parameters::copy(other);

    compression = other.compression;
    whiteBalance = other.whiteBalance;
    whiteBalanceControlSpeed = other.whiteBalanceControlSpeed;
    whiteBalanceControlDelay = other.whiteBalanceControlDelay;
    
    led = other.led;
    backlightCompensation = other.backlightCompensation;
    antiflickerMode = other.antiflickerMode;
    denoise = other.denoise;

    deviceFile = other.deviceFile;

    return ( *this );
  }

  /*
   * write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool pwc::parameters::write(ioHandler& handler,
                              const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    b = b && lti::write(handler,"compression",compression);
    b = b && lti::write(handler,"whiteBalance",whiteBalance);
    b = b && lti::write(handler,"whiteBalanceControlSpeed",
                        whiteBalanceControlSpeed);
    b = b && lti::write(handler,"whiteBalanceControlDelay",
                        whiteBalanceControlDelay);
    b = b &&  lti::write(handler,"led",led);
    b = b &&  lti::write(handler,"backlightCompensation",
                         backlightCompensation);
    b = b &&  lti::write(handler,"antiflickerMode",antiflickerMode);
    b = b &&  lti::write(handler,"denoise",denoise);
    b = b &&  lti::write(handler,"deviceFile",deviceFile);

    b = b && frameGrabber::parameters::write(handler,false);
    b = b && camera::parameters::write(handler,false);
    b = b && panTiltUnit::parameters::write(handler,false);
    
    if (complete) {
      b = b && handler.writeEnd();
    }
    
    return b;
  }

  /*
   * read the parameters from the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also read, otherwise only the data block will be read.
   * @return true if write was successful
   */
  bool pwc::parameters::read(ioHandler& handler,
                             const bool complete) {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    b = b && lti::read(handler,"compression",compression);
    b = b && lti::read(handler,"whiteBalance",whiteBalance);
    b = b && lti::read(handler,"whiteBalanceControlSpeed",
                        whiteBalanceControlSpeed);
    b = b && lti::read(handler,"whiteBalanceControlDelay",
                        whiteBalanceControlDelay);
    b = b &&  lti::read(handler,"led",led);
    b = b &&  lti::read(handler,"backlightCompensation",
                         backlightCompensation);
    b = b &&  lti::read(handler,"antiflickerMode",antiflickerMode);
    b = b &&  lti::read(handler,"denoise",denoise);
    b = b &&  lti::read(handler,"deviceFile",deviceFile);

    b = b && frameGrabber::parameters::read(handler,false);
    b = b && camera::parameters::read(handler,false);
    b = b && panTiltUnit::parameters::read(handler,false);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }


  // --------------------------------
  // The main PWC class
  // --------------------------------

  /*
   * Convert a value using the given format, to degrees
   */
  inline float pwc::toDegrees(const float val,
                              const eAngularUnitsType atype) const {
    switch(atype) {
    case DeviceDependent:
      return val/100.0f;
      break;
    case Degrees:
      return val;
      break;
    case Radians:
      return radToDeg(val);
    }
    return 0;
  }

  /*
   * Convert a value using the given format, to degrees
   */
  inline float pwc::toAngleVal(const float deg,
                               const eAngularUnitsType atype) const {
    switch(atype) {
    case DeviceDependent:
      return deg*100.0f;
      break;
    case Degrees:
      return deg;
      break;
    case Radians:
      return degToRad(deg);
    }
    return 0;
  }



  pwc::pwc()
    : frameGrabber(),camera(),panTiltUnit(),
      initialized_(false),lastUsedParams_(false) {

    initializeLUTs();

    // default parameters
    parameters param;
    setParameters(param);
  }

  pwc::pwc(const std::string& device)
    : frameGrabber(),camera(),panTiltUnit(),
      initialized_(false),lastUsedParams_(false) {

    initializeLUTs();

    // default parameters
    parameters param;
    param.deviceFile = device;
    frameGrabber::setParameters(param);

 }

  pwc::pwc( const parameters& theParam )
    : frameGrabber(),camera(),panTiltUnit(),
      initialized_(false),lastUsedParams_(false) {

    initializeLUTs();

    setParameters(theParam);
  }

  pwc::pwc(const pwc& other)
    : frameGrabber(),camera(),panTiltUnit(),
      initialized_(false),lastUsedParams_(false) {
    copy(other);
  }

  pwc::~pwc() {
    if (initialized_) {
      close(cameraHndl_);
      cameraHndl_ = -1;
      initialized_ = false;
    }
    _lti_debug4("pwc::destructor called and camera freed" << std::endl);
  }

  // returns the name of this class
  const std::string& pwc::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  pwc& pwc::copy(const pwc& other) {
    frameGrabber::copy(other);

    return (*this);
  }

  pwc* pwc::clone() const {
    return new pwc(*this);
  }
  
  // newInstance member function
  pwc* pwc::newInstance() const {
    return new pwc;
  }

  const pwc::parameters& pwc::getParameters() const {
    const parameters* params =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if( isNull(params) ) {
      throw invalidParametersException(name());
    }
    return *params;
  }

  pwc::parameters& pwc::getRWParameters() {
    parameters* params = dynamic_cast<parameters*>(&functor::getRWParameters());
    if( isNull(params) ) {
      throw invalidParametersException(name());
    }
    return *params;
  }

  /*
   * setParameters overload to allow changing the camera attributes
   */
  bool pwc::updateParameters() {
    return setCameraParameters();
  }

  bool pwc::dumpParameters(parameters& par) {
    // get all parameters from the camera

    video_window videoWin;
    if (ioctl(cameraHndl_,VIDIOCGWIN,&videoWin) < 0) {
      // some error getting camera id
      setStatusString(strerror(errno));
      return false;
    }

    par.resolution.set(videoWin.width,videoWin.height);
    par.framerate = (videoWin.flags & PWC_FPS_FRMASK) >> PWC_FPS_SHIFT;
    // fix some known "errors"
    if (par.framerate==4.0f) {
      par.framerate=3.75f;
    } else if (par.framerate==8.0f) {
      par.framerate=7.5f;
    }

    par.snapShotMode = ((videoWin.flags & PWC_FPS_SNAPSHOT)!=0);

    video_picture videoPic;
    if (ioctl(cameraHndl_,VIDIOCGPICT,&videoPic) < 0) {
      // some error getting camera id
      setStatusString(strerror(errno));
      return false;
    }

    par.brightness = videoPic.brightness/65535.0f;
    par.hue        = videoPic.hue/65535.0f;
    par.saturation = videoPic.colour/65535.0f;
    par.contrast   = videoPic.contrast/65535.0f;
    par.gamma      = videoPic.whiteness/65535.0f;

    int ival;
    if (ioctl(cameraHndl_,VIDIOCPWCSCQUAL,&ival) < 0) {
      // some error getting camera id
      setStatusString(strerror(errno));
      return false;
    }
    par.compression = static_cast<eCompressionType>(ival);

    if (!getGain(par.gainMode,par.gain) ||
        !getSharpness(par.sharpnessMode,par.sharpness) ||
        !getShutter(par.shutterMode,par.shutter) ||
        !getWhiteBalance(par.whiteBalanceMode,
                         par.whiteBalance,
                         par.blueGain,
                         par.redGain)) {
      return false;
    }
        
    if (panTilt_) {
      if (!getPanTilt(par.panMode,par.pan,par.tiltMode,par.tilt)) {
        return false;
      }
      par.tiltMode=par.panMode;
    } 
    
    pwc_leds pwcLED;

    if (ioctl(cameraHndl_,VIDIOCPWCGLED,&pwcLED) < 0) {
      // some error getting camera id
      setStatusString(strerror(errno));
      return false;
    }
    
    if ( (pwcLED.led_on>=100) && (pwcLED.led_off==0) ) {
      par.led = 0;
    } else if ( (pwcLED.led_off>=100) && (pwcLED.led_on==0) ) {
      par.led = 1;
    } else {
      par.led = pwcLED.led_on + (pwcLED.led_off << 16);
    }

    pwc_wb_speed pwcSpeed;

    if (ioctl(cameraHndl_,VIDIOCPWCGAWBSPEED,&pwcSpeed) < 0) {
      // some error getting camera id
      setStatusString(strerror(errno));
      return false;
    }

    par.whiteBalanceControlSpeed = 1.0f-(pwcSpeed.control_speed/65535.0f);
    par.whiteBalanceControlDelay = pwcSpeed.control_delay/65535.0f;

    if (ioctl(cameraHndl_,VIDIOCPWCGBACKLIGHT,&ival) < 0) {
      // some error getting camera id
      setStatusString(strerror(errno));
      return false;
    }

    par.backlightCompensation = (ival!=0);

    if (ioctl(cameraHndl_,VIDIOCPWCGFLICKER,&ival) < 0) {
      // some error getting camera id
      setStatusString(strerror(errno));
      return false;     
    }

    par.antiflickerMode = (ival!=0);

    
    if (ioctl(cameraHndl_,VIDIOCPWCGDYNNOISE,&ival) < 0) {
      // some error getting camera id
      setStatusString(strerror(errno));
      return false;     
    }

    par.denoise = static_cast<eDenoiseType>(ival);

    return true;
  }

  // Initialize the camera with the functor parameters
  bool pwc::init() {
    if (initialized_) {
      // already initialized
      return true;
    }

    const parameters& param = getParameters();

    // initialize pwc frame grabber:
    video_capability vcap;
    pwc_probe probe;
    int type,err;

    // check if the given name belongs to a valid device
    struct stat devstat;
    if ((stat(param.deviceFile.c_str(),&devstat)!=0) ||
        (S_ISLNK(devstat.st_mode))) {
      std::string errstr = std::string("Problems with device ") + 
        param.deviceFile + ": " + strerror(errno);
      setStatusString(errstr);
      return false;
    }

    // first try to open the device
    cameraHndl_ = open(param.deviceFile.c_str(),O_RDWR);
    
    if (cameraHndl_ == -1) {
      // some error by opening device
      _lti_debug("Error number: " << errno << std::endl);
      setStatusString(strerror(errno));
      return false;
    }

    if (ioctl(cameraHndl_,VIDIOCGCAP,&vcap) < 0) {      
      // some error getting camera id (it seems we cannot query the
      // camera capabilities)
      setStatusString(strerror(errno));
      close(cameraHndl_);
      cameraHndl_=-1;
      return false;
    }
    
    // get some usefull information about allowed image sizes
    minSize_.set(vcap.minwidth,vcap.minheight);
    maxSize_.set(vcap.maxwidth,vcap.maxheight);

    // check if we have a valid pwc supported camera
    vcap.name[31]='\0'; // ensure that the next comparisons will end!
    memset(&probe,0,sizeof(probe)); // clear all data for sure
    std::string str = vcap.name;
    if ((ioctl(cameraHndl_,VIDIOCPWCPROBE, &probe) == 0) &&
        (str == probe.name)) {
      // everything is ok
      type = probe.type;
    } else {
      setStatusString("No Philips camera detected.");
      close(cameraHndl_);
      cameraHndl_=-1;
      return false;
    }

    // remember the camera name and type
    camName_ = vcap.name;
    camType_ = type;

    _lti_debug4(camName << " detected." << endl);

    // restore factory defaults
    if (ioctl(cameraHndl_,VIDIOCPWCFACTORY) < 0) {
      // some error getting camera id
      setStatusString(strerror(errno));
      close(cameraHndl_);
      cameraHndl_=-1;
      return false;
    }

    // For pan and tilt capable devices, get the valid ranges
    pwc_mpt_range mptRange;
    if ((err=ioctl(cameraHndl_,VIDIOCPWCMPTGRANGE,&mptRange)) < 0) {
      panTilt_=false;

      minPan_=0;
      maxPan_=0;
      
      minTilt_=0;
      maxTilt_=0;
      
    } else {
      panTilt_=true;

      minPan_=mptRange.pan_min;
      maxPan_=mptRange.pan_max;
      
      minTilt_=mptRange.tilt_min;
      maxTilt_=mptRange.tilt_max;
      
      panTiltReset(); // go home!
    }

    initialized_ = true;

    if (setCameraParameters()) {
      return true;
    } else {
      initialized_ = false;
      close(cameraHndl_);
      cameraHndl_=-1;
      return false;
    }

    return false;
  }

  /*
   * Set brightness control mode and register in camera directly.
   *
   * @param mode Camera desired mode (@see eCamFeatureMode)
   * @param brightness If mode is manual, the value to be used.
   *
   * @return true if operation was successfull, false otherwise
   */
  bool pwc::setBrightness(const eCamFeatureMode mode,
                          const float brightness) {
    if (mode != FeatureManual) {
      setStatusString("Brightness only available in manual mode");
      return false;
    }
    if (!initialized_ ) {
      if (!init()) {
        return false;
      }
    }

    if (lastUsedParams_.brightness != brightness) {
      video_picture videoPic;

      // get the values
      if (ioctl(cameraHndl_,VIDIOCGPICT,&videoPic) < 0) {
        // some error getting camera id
        setStatusString(strerror(errno));
        return false;
      }
      
      videoPic.brightness = iround(brightness*65535.0f);

      // set the new values
      if (ioctl(cameraHndl_,VIDIOCSPICT,&videoPic) < 0) {
        // some error getting camera id
        setStatusString(strerror(errno));
        return false;
      } else {
        lastUsedParams_.brightness = brightness;
        lastUsedParams_.brightnessMode = mode;
        getRWParameters().brightness = brightness;
        getRWParameters().brightnessMode = mode; 
      }
    }
    return true;
  }
  
  /*
   * Get brightness mode and value directly out of camera.
   *
   * @param mode Variable where the current brightness mode is to be 
   *              written.
   * @param brightness Variable where the brightness value is to be written.
   *
   * @return true if operation was successfull, false otherwise
   */ 
  bool pwc::getBrightness(eCamFeatureMode& mode,
                          float& brightness) const {
    if (initialized_) {
      video_picture videoPic;
      mode = FeatureManual; // the only available mode
      // get the values
      if (ioctl(cameraHndl_,VIDIOCGPICT,&videoPic) < 0) {
        // some error getting camera id
        setStatusString(strerror(errno));
        return false;
      }
      brightness = static_cast<float>(videoPic.brightness)/65535.0f;
      return true;
    }
    return false;
  }
  
  /*
   * Set contrast through the exposure method control mode and
   * register in camera directly.
   *
   * @param mode Camera desired mode (@see eCamFeatureMode)
   * @param exposure If mode is manual, the value to be used.
   *
   * @return true if operation was successfull, false otherwise
   */
  bool pwc::setExposure(const eCamFeatureMode mode,
                        const float contrast) {
    if (mode != FeatureManual) {
      setStatusString("Contrast only available in manual mode");
      return false;
    }

    if (!initialized_ ) {
      if (!init()) {
        return false;
      }
    }

    if (lastUsedParams_.contrast != contrast) {
      video_picture videoPic;

      // get the values
      if (ioctl(cameraHndl_,VIDIOCGPICT,&videoPic) < 0) {
        // some error getting camera id
        setStatusString(strerror(errno));
        return false;
      }
      
      videoPic.contrast = iround(contrast*65535.0f);

      // set the new values
      if (ioctl(cameraHndl_,VIDIOCSPICT,&videoPic) < 0) {
        // some error getting camera id
        setStatusString(strerror(errno));
        return false;
      } else {
        lastUsedParams_.contrast = contrast;
        lastUsedParams_.contrastMode = mode;
        getRWParameters().contrast = contrast;
        getRWParameters().contrastMode = mode; 
      }
    }
    return true;
  }

    /*
     * Get contrast through the exposure method.
     *
     * @param mode Variable where the current exposure mode is to be 
     *              written.
     * @param exposure Variable where the exposure value is to be written.
     *
     * @return true if operation was successfull, false otherwise
     */ 
  bool pwc::getExposure(eCamFeatureMode& mode,
                             float& contrast) const {
    if (initialized_) {
      video_picture videoPic;
      mode = FeatureManual; // the only available mode
      // get the values
      if (ioctl(cameraHndl_,VIDIOCGPICT,&videoPic) < 0) {
        // some error getting camera id
        setStatusString(strerror(errno));
        return false;
      }
      contrast = static_cast<float>(videoPic.contrast)/65535.0f;
      return true;
    }
    return false;

  }

    /*
     * Set sharpness control mode and register in camera directly.
     *
     * @param mode Camera desired mode (@see eCamFeatureMode)
     * @param sharpness If mode is manual, the value to be used.
     *
     * @return true if operation was successfull, false otherwise
     */
  bool pwc::setSharpness(const eCamFeatureMode mode,
                         const float sharpness) {
    
    if ((mode != FeatureManual) && (mode != FeatureAuto)) {
      setStatusString("Sharpness only available in manual or auto modes");
      return false;
    }

    if (!initialized_ ) {
      if (!init()) {
        return false;
      }
    }

    if ( (lastUsedParams_.sharpnessMode != mode) ||
         ((mode==FeatureManual) && (lastUsedParams_.sharpness != sharpness))) {

      int val=static_cast<int>(sharpness*65535.0f)*((mode==FeatureAuto)?-1:1);
      // get the values
      if (ioctl(cameraHndl_,VIDIOCPWCSCONTOUR,&val) < 0) {
        // some error getting camera id
        setStatusString(strerror(errno));
        return false;
      } else {
        lastUsedParams_.sharpness = sharpness;
        lastUsedParams_.sharpnessMode = mode;
        getRWParameters().sharpness = sharpness;
        getRWParameters().sharpnessMode = mode;
      }
    }
    return true;
  }

    /*
     * Get sharpness mode and value directly out of camera.
     *
     * @param mode Variable where the current sharpness mode is to be 
     *              written.
     * @param sharpness Variable where the sharpness value is to be written.
     *
     * @return true if operation was successfull, false otherwise
     */ 
  bool pwc::getSharpness(eCamFeatureMode& mode,
                         float& sharpness) const {
    if (initialized_) {
      int val;
      if (ioctl(cameraHndl_,VIDIOCPWCGCONTOUR,&val) < 0) {
        // some error getting camera id
        setStatusString(strerror(errno));
        return false;
      }
      // TODO: check if this works (API says you cannot query auto value:)
      mode = (val<0)?FeatureAuto:FeatureManual; // the only available modes
      sharpness = abs(static_cast<float>(val))/65535.0f;
      return true;
    }
    return false;
  }

    /*
     * Set whiteBalance control mode and register in camera directly.
     *
     * Depending on the color space used by the camera (RGB or YUV), the
     * color gains are applied to UV or to BR.
     *
     * @param mode Camera desired mode (@see eCamFeatureMode)
     * @param ubGain gain for the blue channel (or U channel)
     * @param vrGain gain for the red channel (or V channel)
     *
     * @return true if operation was successfull, false otherwise
     */
  bool pwc::setWhiteBalance(const eCamFeatureMode mode,
                            const eWhiteBalanceType whiteBalance,
                            const float ubGain,
                            const float vrGain) {
    if ((mode != FeatureManual) && (mode != FeatureAuto)) {
      setStatusString("White balance only available in manual or auto modes");
      return false;
    }

    if (!initialized_ ) {
      if (!init()) {
        return false;
      }
    }

    if ( (lastUsedParams_.whiteBalanceMode != mode) ||
         ((mode==FeatureManual) && ((lastUsedParams_.redGain  != vrGain) ||
                                    (lastUsedParams_.blueGain != ubGain)))) {

      pwc_whitebalance pwcwb;
      
      if (mode == FeatureAuto) {
        switch(whiteBalance) {
        case Indoor:
          pwcwb.mode = PWC_WB_INDOOR;
          break;
        case Outdoor:
          pwcwb.mode = PWC_WB_OUTDOOR;
          break;
        case Fluorescent:
          pwcwb.mode = PWC_WB_FL;
          break;
        default:
          pwcwb.mode = PWC_WB_AUTO;
          break;
        }
      } else {
        pwcwb.mode = PWC_WB_MANUAL;
      }
      
      pwcwb.manual_red = iround(65535.0f*vrGain);
      pwcwb.manual_blue = iround(65535.0f*ubGain);

      if (ioctl(cameraHndl_,VIDIOCPWCSAWB,&pwcwb) < 0) {
        // some error getting camera id
        setStatusString(strerror(errno));
        return false;
      } else {
        lastUsedParams_.whiteBalanceMode = mode;
        lastUsedParams_.redGain  = vrGain;
        lastUsedParams_.blueGain = ubGain;
        lastUsedParams_.whiteBalance = whiteBalance;
        
        parameters& par = getRWParameters();

        par.whiteBalanceMode = mode;
        par.redGain = vrGain;
        par.blueGain = ubGain;
        par.whiteBalance = whiteBalance;        
      }    
    }
    return true;
  }

  bool pwc::setWhiteBalance(const eCamFeatureMode mode,
                            const float ubGain,
                            const float vrGain) {
    return setWhiteBalance(mode,Auto,ubGain,vrGain);
  }
    
    /*
     * Get whiteBalance mode and value directly out of camera.
     *
     * @param mode Variable where the current whiteBalance mode is to be 
     *              written.
     * @param ubGain gain for the blue channel (or U channel)
     * @param vrGain gain for the red channel (or V channel)
     *
     * @return true if operation was successfull, false otherwise
     */ 
  bool pwc::getWhiteBalance(eCamFeatureMode& mode,
                            eWhiteBalanceType& whiteBalance,
                            float& ubGain,
                            float& vrGain) const {
    
    if (initialized_) {
      pwc_whitebalance pwcwb;
      
      if (ioctl(cameraHndl_,VIDIOCPWCGAWB,&pwcwb) < 0) {
        // some error getting camera id
        setStatusString(strerror(errno));
        return false;
      }

      ubGain = vrGain = -1.0f; // set invalid values: cannot be read, unless...

      if (pwcwb.mode == PWC_WB_MANUAL) {
        mode = FeatureManual;
        ubGain = static_cast<float>(pwcwb.manual_blue/65535.0f);
        vrGain = static_cast<float>(pwcwb.manual_red/65535.0f);
      } else {
        mode = FeatureAuto;
        switch(pwcwb.mode) {
        case PWC_WB_AUTO:
          whiteBalance = Auto;
          ubGain = static_cast<float>(pwcwb.read_blue/65535.0f);
          vrGain = static_cast<float>(pwcwb.read_red/65535.0f);
          break;
        case PWC_WB_INDOOR:
          whiteBalance = Indoor;
          break;
        case PWC_WB_OUTDOOR:
          whiteBalance = Outdoor;
          break;
        case PWC_WB_FL:
          whiteBalance = Fluorescent;
          break;
        default:
          whiteBalance = InvalidWB;
        }
      }
      return true;
    }
    return false;

  }

  bool pwc::getWhiteBalance(eCamFeatureMode& mode,
                            float& ubGain,
                            float& vrGain) const {

    eWhiteBalanceType whiteBalance;
    return getWhiteBalance(mode,whiteBalance,ubGain,vrGain);
  }


    /*
     * Set hue control mode and register in camera directly.
     *
     * @param mode Camera desired mode (@see eCamFeatureMode)
     * @param hue If mode is manual, the value to be used.
     *
     * @return true if operation was successfull, false otherwise
     */
  bool pwc::setHue(const eCamFeatureMode mode,
                        const float hue) {
    if (mode != FeatureManual) {
      setStatusString("Hue only available in manual mode");
      return false;
    }
    if (!initialized_ ) {
      if (!init()) {
        return false;
      }
    }

    if (lastUsedParams_.hue != hue) {
      video_picture videoPic;

      // get the values
      if (ioctl(cameraHndl_,VIDIOCGPICT,&videoPic) < 0) {
        // some error getting camera id
        setStatusString(strerror(errno));
        return false;
      }
      
      videoPic.hue = iround(hue*65535.0f);

      // set the new values
      if (ioctl(cameraHndl_,VIDIOCSPICT,&videoPic) < 0) {
        // some error getting camera id
        setStatusString(strerror(errno));
        return false;
      } else {
        lastUsedParams_.hue = hue;
        lastUsedParams_.hueMode = mode;
        getRWParameters().hue = hue;
        getRWParameters().hueMode = mode;
      }
    }
    return true;
  }

    /*
     * Get hue mode and value directly out of camera.
     *
     * @param mode Variable where the current hue mode is to be 
     *              written.
     * @param hue Variable where the hue value is to be written.
     *
     * @return true if operation was successfull, false otherwise
     */ 
  bool pwc::getHue(eCamFeatureMode& mode,
                        float& hue) const {
    if (initialized_) {
      video_picture videoPic;
      mode = FeatureManual; // the only available mode
      // get the values
      if (ioctl(cameraHndl_,VIDIOCGPICT,&videoPic) < 0) {
        // some error getting camera id
        setStatusString(strerror(errno));
        return false;
      }
      hue = static_cast<float>(videoPic.hue)/65535.0f;
      return true;
   }
    return false;
  }

    /*
     * Set saturation control mode and register in camera directly.
     *
     * @param mode Camera desired mode (@see eCamFeatureMode)
     * @param saturation If mode is manual, the value to be used.
     *
     * @return true if operation was successfull, false otherwise
     */
  bool pwc::setSaturation(const eCamFeatureMode mode,
                          const float saturation) {
    if (mode != FeatureManual) {
      setStatusString("Saturation only available in manual mode");
      return false;
    }
    if (!initialized_ ) {
      if (!init()) {
        return false;
      }
    }

    if (lastUsedParams_.saturation != saturation) {
      video_picture videoPic;

      // get the values
      if (ioctl(cameraHndl_,VIDIOCGPICT,&videoPic) < 0) {
        // some error getting camera id
        setStatusString(strerror(errno));
        return false;
      }
      
      videoPic.colour = iround(saturation*65535.0f);

      // set the new values
      if (ioctl(cameraHndl_,VIDIOCSPICT,&videoPic) < 0) {
        // some error getting camera id
        setStatusString(strerror(errno));
        return false;
      } else {
        lastUsedParams_.saturation = saturation;
        lastUsedParams_.saturationMode = mode;
        getRWParameters().saturation = saturation;
        getRWParameters().saturationMode = mode;
      }
    }
    return true;

  }

    /*
     * Get saturation mode and value directly out of camera.
     *
     * @param mode Variable where the current saturation mode is to be 
     *              written.
     * @param saturation Variable where the saturation value is to be written.
     *
     * @return true if operation was successfull, false otherwise
     */ 
  bool pwc::getSaturation(eCamFeatureMode& mode,
                               float& saturation) const {
    if (initialized_) {
      video_picture videoPic;
      mode = FeatureManual; // the only available mode
      // get the values
      if (ioctl(cameraHndl_,VIDIOCGPICT,&videoPic) < 0) {
        // some error getting camera id
        setStatusString(strerror(errno));
        return false;
      }
      saturation = static_cast<float>(videoPic.colour)/65535.0f;
      return true;
    }
    return false;
  }
    
    /*
     * Set gamma control mode and register in camera directly.
     *
     * @param mode Camera desired mode (@see eCamFeatureMode)
     * @param gamma If mode is manual, the value to be used.
     *
     * @return true if operation was successfull, false otherwise
     */
  bool pwc::setGamma(const eCamFeatureMode mode,
                          const float gamma) {
    if (mode != FeatureManual) {
      setStatusString("Gamma only available in manual mode");
      return false;
    }
    if (!initialized_ ) {
      if (!init()) {
        return false;
      }
    }

    if (lastUsedParams_.gamma != gamma) {
      video_picture videoPic;

      // get the values
      if (ioctl(cameraHndl_,VIDIOCGPICT,&videoPic) < 0) {
        // some error getting camera id
        setStatusString(strerror(errno));
        return false;
      }
      
      videoPic.whiteness = iround(gamma*65535.0f);

      // set the new values
      if (ioctl(cameraHndl_,VIDIOCSPICT,&videoPic) < 0) {
        // some error getting camera id
        setStatusString(strerror(errno));
        return false;
      } else {
        lastUsedParams_.gamma = gamma;
        lastUsedParams_.gammaMode = mode;
        getRWParameters().gamma = gamma;
        getRWParameters().gammaMode = mode;
      }
    }
    return true;
  }

    /*
     * Get gamma mode and value directly out of camera.
     *
     * @param mode Variable where the current gamma mode is to be 
     *              written.
     * @param gamma Variable where the gamma value is to be written.
     *
     * @return true if operation was successfull, false otherwise
     */ 
  bool pwc::getGamma(eCamFeatureMode& mode,
                     float& gamma) const {
    if (initialized_) {
      video_picture videoPic;
      mode = FeatureManual; // the only available mode
      // get the values
      if (ioctl(cameraHndl_,VIDIOCGPICT,&videoPic) < 0) {
        // some error getting camera id
        setStatusString(strerror(errno));
        return false;
      }
      gamma = static_cast<float>(videoPic.whiteness)/65535.0f;
      return true;
    }
    return false;
  }

    /*
     * Set shutter control mode and register in camera directly.
     *
     * @param mode Camera desired mode (@see eCamFeatureMode)
     * @param shutter If mode is manual, the value to be used.
     *
     * @return true if operation was successfull, false otherwise
     */
  bool pwc::setShutter(const eCamFeatureMode mode,
                       const float shutter) {

    if ((mode != FeatureManual) && (mode != FeatureAuto)) {
      setStatusString("Shutter only available in manual or auto modes");
      return false;
    }

    if (!initialized_ ) {
      if (!init()) {
        return false;
      }
    }

    if ( (lastUsedParams_.shutterMode != mode) ||
         ((mode==FeatureManual) && (lastUsedParams_.shutter != shutter))) {

      int val=static_cast<int>(shutter*65535.0f)*((mode==FeatureAuto)?-1:1);
      // set the values
      if (ioctl(cameraHndl_,VIDIOCPWCSSHUTTER,&val) < 0) {
        // some error getting camera id
        setStatusString(strerror(errno));
        return false;
      } else {
        lastUsedParams_.shutter = shutter;
        lastUsedParams_.shutterMode = mode;
        getRWParameters().shutter = shutter;
        getRWParameters().shutterMode = mode;
        
      }
    }
    return true;
  }

    /*
     * Get shutter mode and value directly out of camera.
     *
     * @param mode Variable where the current shutter mode is to be 
     *              written.
     * @param shutter Variable where the shutter value is to be written.
     *
     * @return true if operation was successfull, false otherwise
     */ 
  bool pwc::getShutter(eCamFeatureMode& ,
                       float& ) const {
    setStatusString("Shutter setting cannot be read from the camera");
    return false;
  }
  
  /*
   * Set gain control mode and register in camera directly.
   *
   * @param mode Camera desired mode (@see eCamFeatureMode)
   * @param gain If mode is manual, the value to be used.
   *
   * @return true if operation was successfull, false otherwise
   */
  bool pwc::setGain(const eCamFeatureMode mode,
                    const float gain) {
    
    if ((mode != FeatureManual) && (mode != FeatureAuto)) {
      setStatusString("Gain only available in manual or auto modes");
      return false;
    }

    if (!initialized_ ) {
      if (!init()) {
        return false;
      }
    }

    if ( (lastUsedParams_.gainMode != mode) ||
         ((mode==FeatureManual) && (lastUsedParams_.gain != gain))) {

      int val=static_cast<int>(gain*65535.0f)*((mode==FeatureAuto)?-1:1);
      // set the values
      if (ioctl(cameraHndl_,VIDIOCPWCSAGC,&val) < 0) {
        // some error getting camera id
        setStatusString(strerror(errno));
        return false;
      } else {
        lastUsedParams_.gainMode = mode;
        lastUsedParams_.gain = gain;
        getRWParameters().gainMode = mode;
        getRWParameters().gain = gain;
      }
    }
    return true;
  }

    /*
     * Get gain mode and value directly out of camera.
     *
     * @param mode Variable where the current gain mode is to be 
     *              written.
     * @param gain Variable where the gain value is to be written.
     *
     * @return true if operation was successfull, false otherwise
     */ 
  bool pwc::getGain(eCamFeatureMode& mode,
                    float& gain) const {
    if (initialized_) {
      int val;
      if (ioctl(cameraHndl_,VIDIOCPWCGAGC,&val) < 0) {
        // some error getting camera id
        setStatusString(strerror(errno));
        return false;
      }
      mode = (val<0)?FeatureAuto:FeatureManual; // the only available modes
      gain = abs(static_cast<float>(val))/65535.0f;
      return true;
    }
    return false;
  }

  /*
   * Get current frame rate directly out of camera.
   * @return true if operation was successfull, false otherwise
   */
  bool pwc::getFramerate(float& framerate) const {
    video_window videoWin;
    if (ioctl(cameraHndl_,VIDIOCGWIN,&videoWin) < 0) {
      // some error getting camera id
      setStatusString(strerror(errno));
      return false;
    }
    framerate=(videoWin.flags & PWC_FPS_FRMASK) >> PWC_FPS_SHIFT;
    // fix some known "errors"
    if (framerate==4.0f) {
      framerate=3.75f;
    } else if (framerate==8.0f) {
      framerate=7.5f;
    }

    return true;
  }

  /*
   * Set frame rate directly to of camera.
   * @return true if operation was successfull, false otherwise
   */
  bool pwc::setFramerateAndResolution(const float framerate,
                                      const ipoint& resolution) {
    // get default data
    video_window videoWin;
    if (ioctl(cameraHndl_,VIDIOCGWIN,&videoWin) < 0) {
      // some error getting camera id
      setStatusString(strerror(errno));
      return false;
    }

    videoWin.width  = within(resolution.x,minSize_.x,maxSize_.x);
    videoWin.height = within(resolution.y,minSize_.y,maxSize_.y);

    // set new frame rate
    videoWin.flags &= ~PWC_FPS_FRMASK;
    videoWin.flags |= ((iround(framerate) & 0x3F) << PWC_FPS_SHIFT);

    // set the data
    if (ioctl(cameraHndl_,VIDIOCSWIN,&videoWin) < 0) {
      setStatusString("Invalid frame-rate/resolution configuration. " \
                      "Check your settings.");
      return false;
    }

    lastUsedParams_.framerate = framerate;
    getRWParameters().framerate = framerate;

    resolution_ = ipoint(videoWin.width,videoWin.height);
    lastUsedParams_.resolution = resolution_;
    getRWParameters().resolution = resolution_;

    return true;
  }

  /*
   * Get current resolution, posibly directly out of the camera
   */
  bool pwc::getResolution(ipoint& resolution) const {
    video_window videoWin;
    if (ioctl(cameraHndl_,VIDIOCGWIN,&videoWin) < 0) {
      // some error getting camera id
      setStatusString(strerror(errno));
      return false;
    }

    resolution.set(videoWin.width,videoWin.height);
    return true;
  }


  /*
   * Set pan control mode and register in device directly.
   *
   * @param mode Device desired mode (@see eCamFeatureMode)
   * @param pan If mode is manual, the value to be used.
   *
   * @return true if operation was successfull, false otherwise
   */
  bool pwc::setPan(const eCamFeatureMode mode,
                   const float pan) {
    if (!panTilt_) {
      setStatusString("No pan-tilt unit available");
      return false;
    }

    if (mode != FeatureManual) {
      setStatusString("Pan only available in manual mode");
      return false;
    }

    if (!initialized_ ) {
      if (!init()) {
        return false;
      }
    }

    pwc_mpt_angles ptsetting; // pan-tilt settings
    pwc_mpt_status ptstatus;  // pan-tilt status

    if ( (lastUsedParams_.panMode != mode) ||
         (lastUsedParams_.pan     != pan) ) {
      int st=1; // with 1 it is indicated not to wait the first time...

      // if a previous pan-tilt command is still running, we have to wait
      // for it to finish before we give the new settings in!
      do {
        if (st==0) {
          passiveWait(250000); // wait 1/4 of a second
        }
        if (ioctl(cameraHndl_,VIDIOCPWCMPTSTATUS,&ptstatus) < 0) {
          // some error getting camera id
          setStatusString(strerror(errno));
          return false;
        }
        st = ptstatus.status & 0x07; // get the lowest 3 bits
      } while (st == 0); // while command running, wait!

      // get the pan and tilt values, as maybe something hit a hard stop!
      eCamFeatureMode pm,tm;
      getPanTilt(pm,lastUsedParams_.pan,tm,lastUsedParams_.tilt);

      ptsetting.absolute=0;
      ptsetting.pan=iround(toAngleVal(toDegrees(lastUsedParams_.pan,
                                                lastUsedParams_.angularUnits),
                                      DeviceDependent));

      // set the values
      if (ioctl(cameraHndl_,VIDIOCPWCMPTSANGLE,&ptsetting) < 0) {
        // some error getting camera id
        setStatusString(strerror(errno));
        return false;
      } else {
        getRWParameters().pan  = pan;
        getRWParameters().panMode  = mode;
      }
    }
    return true;
  }

    /*
     * Get pan mode and value directly out of device.
     *
     * @param mode Variable where the current pan mode is to be 
     *              written.
     * @param pan Variable where the pan value is to be written.
     *
     * @return true if operation was successfull, false otherwise
     */ 
  bool pwc::getPan(eCamFeatureMode& mode,
                   float& pan) const {
    float tmp;
    eCamFeatureMode m;
    return getPanTilt(mode,pan,m,tmp);
  }

    /*
     * Set tilt control mode and register in device directly.
     *
     * @param mode Device desired mode (@see eCamFeatureMode)
     * @param tilt If mode is manual, the value to be used.
     *
     * @return true if operation was successfull, false otherwise
     */
  bool pwc::setTilt(const eCamFeatureMode mode,
                    const float tilt) {
    if (!panTilt_) {
      setStatusString("No pan-tilt unit available");
      return false;
    }

    if (mode != FeatureManual) {
      setStatusString("Tilt only available in manual mode");
      return false;
    }

    if (!initialized_ ) {
      if (!init()) {
        return false;
      }
    }

    pwc_mpt_angles ptsetting; // pan-tilt settings
    pwc_mpt_status ptstatus;  // pan-tilt status

    if ( (lastUsedParams_.tiltMode != mode) ||
         (lastUsedParams_.tilt     != tilt) ) {
      int st=1; // with 1 it is indicated not to wait the first time...

      // if a previous pan-tilt command is still running, we have to wait
      // for it to finish before we give the new settings in!
      do {
        if (st==0) {
          passiveWait(250000); // wait 1/4 of a second
        }
        if (ioctl(cameraHndl_,VIDIOCPWCMPTSTATUS,&ptstatus) < 0) {
          // some error getting camera id
          setStatusString(strerror(errno));
          return false;
        }
        st = ptstatus.status & 0x07; // get the lowest 3 bits
      } while (st == 0); // while command running, wait!

      // get the pan and tilt values, as maybe something hit a hard stop!
      eCamFeatureMode pm,tm;
      getPanTilt(pm,lastUsedParams_.pan,tm,lastUsedParams_.tilt);

      ptsetting.absolute=0;
      ptsetting.tilt=iround(toAngleVal(toDegrees(lastUsedParams_.tilt,
                                                 lastUsedParams_.angularUnits),
                                       DeviceDependent));

      // set the values
      if (ioctl(cameraHndl_,VIDIOCPWCMPTSANGLE,&ptsetting) < 0) {
        // some error getting camera id
        setStatusString(strerror(errno));
        return false;
      } else {
        getRWParameters().tilt  = tilt;
        getRWParameters().tiltMode  = mode;
      }
    }
    return true;
  }

    /*
     * Get tilt mode and value directly out of device.
     *
     * @param mode Variable where the current tilt mode is to be 
     *              written.
     * @param tilt Variable where the tilt value is to be written.
     *
     * @return true if operation was successfull, false otherwise
     */ 
  bool pwc::getTilt(eCamFeatureMode& mode,
                    float& tilt) const {
    float tmp;
    eCamFeatureMode m;
    return getPanTilt(mode,tmp,m,tilt);
  }


    /*
     * Set pan control mode and register in device directly.
     *
     * @param mode Device desired mode (@see eCamFeatureMode)
     * @param pan If mode is manual, the value to be used.
     *
     * @return true if operation was successfull, false otherwise
     */
  bool pwc::setPanTilt(const eCamFeatureMode mode,
                       const float pan,
                       const float tilt) {
    if (!panTilt_) {
      setStatusString("No pan-tilt unit available");
      return false;
    }

    if (mode != FeatureManual) {
      setStatusString("Pan only available in manual mode");
      return false;
    }

    if (!initialized_ ) {
      if (!init()) {
        return false;
      }
    }

    pwc_mpt_angles ptsetting; // pan-tilt settings
    pwc_mpt_status ptstatus;  // pan-tilt status

    if ( (lastUsedParams_.panMode != mode) ||
         (lastUsedParams_.tiltMode != mode) ||
         (lastUsedParams_.pan != pan) ||
         (lastUsedParams_.tilt != tilt ) ) {
      int st=1; // with 1 it is indicated not to wait the first time...

      // if a previous pan-tilt command is still running, we have to wait
      // for it to finish before we give the new settings in!
      do {
        if (st==0) {
          passiveWait(250000); // wait 1/4 of a second
        }
        if (ioctl(cameraHndl_,VIDIOCPWCMPTSTATUS,&ptstatus) < 0) {
          // some error getting camera id
          setStatusString(strerror(errno));
          return false;
        }
        st = ptstatus.status & 0x07; // get the lowest 3 bits
      } while (st == 0); // while command running, wait!

      // get the pan and tilt values, as maybe something hit a hard stop!
      eCamFeatureMode pm,tm;
      getPanTilt(pm,lastUsedParams_.pan,tm,lastUsedParams_.tilt);

      ptsetting.absolute=0;
      ptsetting.pan=iround(toAngleVal(toDegrees(pan,
                                                lastUsedParams_.angularUnits),
                                      DeviceDependent));
      ptsetting.tilt=iround(toAngleVal(toDegrees(tilt,
                                                 lastUsedParams_.angularUnits),
                                       DeviceDependent));

      // get the values
      if (ioctl(cameraHndl_,VIDIOCPWCMPTSANGLE,&ptsetting) < 0) {
        // some error getting camera id
        setStatusString(strerror(errno));
        return false;
      } else {
        getRWParameters().pan  = pan;
        getRWParameters().tilt = tilt;
        getRWParameters().panMode  = mode;
        getRWParameters().tiltMode = mode;
      }
    }
    return true;
   
    
  }

    /*
     * Get pan mode and value directly out of device.
     *
     * @param mode Variable where the current pan mode is to be 
     *              written.
     * @param pan Variable where the pan value is to be written.
     *
     * @return true if operation was successfull, false otherwise
     */ 
  bool pwc::getPanTilt(eCamFeatureMode& panMode,
                       float& pan,
                       eCamFeatureMode& tiltMode,
                       float& tilt) const {
    if (!panTilt_) {
      panMode=tiltMode=FeatureNA;
      pan=tilt=0.0f;
      return true;
    }

    pwc_mpt_angles ptsetting; // pan-tilt settings
    if (ioctl(cameraHndl_,VIDIOCPWCMPTGANGLE,&ptsetting) < 0) {
      // some error getting camera id
      setStatusString(strerror(errno));
      return false;
    }

    panMode = tiltMode = FeatureManual;
    pan  = toAngleVal(toDegrees(ptsetting.pan ,DeviceDependent),
                      lastUsedParams_.angularUnits);
    tilt = toAngleVal(toDegrees(ptsetting.tilt,DeviceDependent),
                      lastUsedParams_.angularUnits);

    return true;
  }

  bool pwc::panTiltReset() {
    if (!panTilt_) {
      return true; // everything is ok, we don't even have a unit!
    }
    int val = 3; // reset both: Pan AND Tilt
    if (ioctl(cameraHndl_,VIDIOCPWCMPTRESET,&val) < 0) {
      // some error getting camera id
      setStatusString(strerror(errno));
      return false;
    } 

    lastUsedParams_.pan  = 0.0f;
    lastUsedParams_.tilt = 0.0f;
    
    return true;
  }


    /*
     * Check if a particular feature is available in the current active
     * device.
     *
     * You have to ensure that a device is already active (see
     * isActive()).
     *
     * \param featureId The identification of the feature expects one of the
     *                  - FeaturePan
     *                  - FeatureTilt
     *                  - FeatureZoom
     *                  - FeatureOpticalFilter
     *
     * This pure virtual method may have other possible values as
     */
  bool pwc::isFeatureAvailable(const unsigned int featureId) const {
    switch(featureId) {
    case FeatureBrightness:
    case FeatureExposure:
    case FeatureSharpness:
    case FeatureWhiteBalance:
    case FeatureHue:
    case FeatureSaturation:
    case FeatureGamma:
    case FeatureShutter:
    case FeatureGain:
      return true;
      break;
    case FeaturePan:
    case FeatureTilt:
      return panTilt_;
      break;
    default:
      return false;
    }
    return false;
  }

    /*
     * Check if a feature is available and is read-out capable.
     *
     * Some devices have a feature, but you cannot read the state in
     * which that feature is set.  This method returns true if the
     * device can provide information on the feature state/mode.
     *
     * \param featureId identification for the feature to be queried.
     *                  (see device::isFeatureAvailable() for a list of
     *                   valid values)
     * \return \c true if feature is read-out capable or \c false if it is not
     *         available at all, or if it is not readable.
     */
  bool pwc::isFeatureReadable(const unsigned int featureId) const {
    switch(featureId) {
      //case FeatureBrightness:
      //case FeatureExposure:
      //case FeatureSharpness:
    case FeatureWhiteBalance:
      //case FeatureHue:
      //case FeatureSaturation:
      //case FeatureGamma:
    case FeatureShutter:
    case FeatureGain:
    case FeaturePan:
    case FeatureTilt:
      return true;
      break;
    default:
      return false;
    }
    return false;
  }

    /*
     * Check for availability of feature modes
     *
     * A given feature (see the list of valid identifiers in
     * lti::device::isFeatureAvailable() ) can have one of five modes:
     * - Not available: the feature does not exist in the current device
     * - Off mode: the feature is deactivated (some features cannot be
     *             deactivated).
     * - Auto mode: the feature is automatically adjusted 
     * - Manual mode: the user controls the values a feature can get 
     * - One-Push mode: the feature is automatically adjusted but only once.
     *
     * This method asks the active device which modes are supported and
     * returns a bitwise combination of lti::eCamFeatureMode constants.
     *
     * You have to ensure that a device is active (see device::isActive()).
     *
     * \param featId identification for the feature to be modified
     *                  (see device::isFeatureAvailable() for a list of
     *                   valid values)
     *
     */
  unsigned int pwc::checkFeatureModes(const unsigned int featureId) const {
    switch(featureId) {
    case FeatureBrightness:
    case FeatureExposure:
    case FeatureHue:
    case FeatureSaturation:
    case FeatureGamma:
      return FeatureManual;
      break;
    case FeaturePan:
    case FeatureTilt:
      return (panTilt_)?FeatureManual:FeatureNA;
    case FeatureSharpness:
    case FeatureWhiteBalance:
    case FeatureShutter:
    case FeatureGain:
      return static_cast<unsigned int>(FeatureManual | FeatureAuto);
      break;
    default:
      return FeatureNA;
    }
    return FeatureNA;
  }

    /*
     * Get feature mode of the device for one specific feature.
     *
     * @return \c true if feature mode could be successfully read, or \c false
     *         if this was not possible (maybe the device does not support
     *         the given feature at all).
     */
  bool pwc::getFeatureMode(const unsigned int featureId,
                           eCamFeatureMode& mode) const {
    float u,v; // temp variables;

    switch(featureId) {
    case FeatureBrightness:
    case FeatureExposure:
    case FeatureHue:
    case FeatureSaturation:
    case FeatureGamma:
      mode = FeatureManual;
      return true;
      break;
    case FeaturePan:
    case FeatureTilt:
      mode = (panTilt_) ? FeatureManual : FeatureNA;
      break;
    case FeatureSharpness: 
      return getSharpness(mode,v);
      break;      
    case FeatureWhiteBalance:
      return getWhiteBalance(mode,u,v);
      break;
    case FeatureShutter:
      return getShutter(mode,v);
      break;      
    case FeatureGain:
      return getGain(mode,v);
      break;
    default:
      return 0;
    }
    return 0;
  }

    /*
     * Set operation mode of a particular feature of the device.
     *
     * @return \c true if feature mode could be successfully set, or \c false
     *         if this was not possible (maybe the device does not support
     *         the given mode for the feature.
     */
  bool pwc::setFeatureMode(const unsigned int featureId,
                           const eCamFeatureMode mode) {
    parameters& par = getRWParameters();
    eCamFeatureMode m;
    float u,v;

    switch(featureId) {
    case FeatureBrightness:
      if (mode == FeatureManual) {
        par.brightnessMode = FeatureManual;
        return true;
      }
      break;
    case FeatureExposure:
      if (mode == FeatureManual) {
        par.exposureMode = FeatureManual;
        return true;
      }
      break;
    case FeatureHue:
      if (mode == FeatureManual) {
        par.hueMode = FeatureManual;
        return true;
      }
      break;
    case FeatureSaturation:
      if (mode == FeatureManual) {
        par.saturationMode = FeatureManual;
        return true;
      }
      break;
    case FeatureGamma:
      if (mode == FeatureManual) {
        par.gammaMode = FeatureManual;
        return true;
      }
      break;
    case FeaturePan:
      if (panTilt_ && (mode == FeatureManual)) {
        par.panMode = FeatureManual;
        return true;
      }
      break;
    case FeatureTilt:
      if (panTilt_ && (mode == FeatureManual)) {
        par.tiltMode = FeatureManual;
        return true;
      }
      break;
    case FeatureSharpness: 
      if (getSharpness(m,v)) {
        return setSharpness(mode,v);
      }
      break;      
    case FeatureWhiteBalance:
      if (getWhiteBalance(m,u,v)) {
        return setWhiteBalance(mode,u,v);
      }
      break;
    case FeatureShutter:
      // shutter is not readable
      return setShutter(mode,lastUsedParams_.shutter);
      break;      
    case FeatureGain:
      if (getGain(m,v)) {
        return setGain(mode,v);
      }
      break;
    default:
      return false;
    }
    return false;
    
  }

    /*
     * Set control mode and value of a particular feature.
     *
     * This method does not support those features that require two values,
     * like white balance.
     *
     * @param featureId identification for the feature to be modified
     *                  (see device::isFeatureAvailable() for a list of
     *                   valid values)
     * @param mode Device desired mode (@see eCamFeatureMode)
     * @param value If mode is manual, the value to be used.
     *
     * @return true if operation was successfull, false otherwise
     */
  bool pwc::setFeatureValue(const unsigned int featureId,
                            const eCamFeatureMode mode,
                            const float value) {

    switch(featureId) {
    case FeatureBrightness:
      return setBrightness(mode,value);
      break;
    case FeatureExposure:
      return setContrast(mode,value);
      break;
    case FeatureHue:
      return setHue(mode,value);
      break;
    case FeatureSaturation:
      return setSaturation(mode,value);
      break;
    case FeatureGamma:
      return setGamma(mode,value);
      break;
    case FeaturePan:
      return setPan(mode,value);
      break;
    case FeatureTilt:
      return setTilt(mode,value);
      break;
    case FeatureSharpness:
      return setSharpness(mode,value);
      break;      
    case FeatureWhiteBalance:
      // not setable with this method!
      return false;
      break;
    case FeatureShutter:
      return setShutter(mode,value);
      break;      
    case FeatureGain:
      return setGain(mode,value);
      break;
    default:
      return false;
    }
    return false;
  }
    
    /*
     * Get control mode and value of a particular feature.
     *
     * This method does not support those features that require two values,
     * like white balance.
     *
     * @param featureId identification for the feature to be queried.
     * @param mode Mode in which the feature is being used
     * @param value if mode is manual, the value to be used.
     *
     * @return \c true if operation was successfull, \c false otherwise (for
     *         example, if the feature is not supported by the device).
     */
  bool pwc::getFeatureValue(const unsigned int featureId,
                            eCamFeatureMode& mode,
                            float& value) const {
    switch(featureId) {
    case FeatureBrightness:
      return getBrightness(mode,value);
      break;
    case FeatureExposure:
      return getContrast(mode,value);
      break;
    case FeatureHue:
      return getHue(mode,value);
      break;
    case FeatureSaturation:
      return getSaturation(mode,value);
      break;
    case FeatureGamma:
      return getGamma(mode,value);
      break;
    case FeaturePan:
      return getPan(mode,value);
      break;
    case FeatureTilt:
      return getTilt(mode,value);
      break;
    case FeatureSharpness:
      return getSharpness(mode,value);
      break;      
    case FeatureWhiteBalance:
      // not getable with this method!
      return false;
      break;
    case FeatureShutter:
      return getShutter(mode,value);
      break;      
    case FeatureGain:
      return getGain(mode,value);
      break;
    default:
      return false;
    }
    return false;
  }

    /*
     * Check the valid value range for a specified feature.
     *
     * You have to ensure that there is an active device before calling this
     * method. (see device::isActive() )
     *
     * @param featureId identification for the feature to be modified
     *                  (see device::isFeatureAvailable() for a list of
     *                   valid values)
     * @param minValue minimum value accepted for the specified feature.
     * @param maxValue maximum value accepted for the specified feature.
     *
     * @return \c true if successful or \c false otherwise.
     */
  bool pwc::checkFeatureValueRange(const unsigned int featureId,
                                   float& minValue,
                                   float& maxValue) const {
    minValue=0.0f;
    maxValue=1.0f; // these are almost for everyone true!

    if (panTilt_) {
      if (featureId == FeaturePan) {
        minValue=minPan_;
        maxValue=maxPan_;
      } else if (featureId == FeatureTilt) {
        minValue=minTilt_;
        maxValue=maxTilt_;
      }
    }

    return true;
  }

  // Initialize the camera with the functor parameters
  bool pwc::setCameraParameters() {
    if (!isInitialized()) {
      // already initialized
      return init();
    }

    const parameters& param = getParameters();

    // --------------------------------------------------
    // capture windows
    // --------------------------------------------------

    // flag used to force some cam initializations of boolean vars, which 
    // cannot be set to an invalid initial value
    const bool firstTime = (lastUsedParams_.brightnessMode == FeatureNA);

    // get default data
    video_window videoWin;
    if (ioctl(cameraHndl_,VIDIOCGWIN,&videoWin) < 0) {
      // some error getting camera id
      setStatusString(strerror(errno));
      return false;
    }

    // set new size
    if ((lastUsedParams_.resolution != param.resolution) ||
        (lastUsedParams_.framerate != param.framerate) ||
        (lastUsedParams_.snapShotMode != param.snapShotMode)) {

      videoWin.width  = within(param.resolution.x,minSize_.x,maxSize_.x);
      videoWin.height = within(param.resolution.y,minSize_.y,maxSize_.y);

      lastUsedParams_.resolution.set(videoWin.width,videoWin.height);

      // set new frame rate
      videoWin.flags &= ~PWC_FPS_FRMASK;
      videoWin.flags |= ((static_cast<int>(param.framerate) & 0x3F) <<
                         PWC_FPS_SHIFT);

      lastUsedParams_.framerate = param.framerate;

      // snapshot mode
      if (param.snapShotMode) {
        videoWin.flags |= PWC_FPS_SNAPSHOT;
      } else {
        videoWin.flags &= ~PWC_FPS_SNAPSHOT;
      }

      lastUsedParams_.snapShotMode = param.snapShotMode;

      // set the data
      if (ioctl(cameraHndl_,VIDIOCSWIN,&videoWin) < 0) {
	  setStatusString("Invalid frame-rate/resolution configuration. " \
			  "Check your settings.");
        return false;
      }

      // get the data again to check the size really been used
      if (ioctl(cameraHndl_,VIDIOCGWIN,&videoWin) < 0) {
        // some error getting camera id
        setStatusString(strerror(errno));
        return false;
      }

      // member with the real size:
      resolution_ = ipoint(videoWin.width,videoWin.height);

#     if defined(_LTI_DEBUG) && _LTI_DEBUG >= 4
      if (ioctl(cameraHndl_,VIDIOCGWIN,&videoWin) < 0) {
        // some error getting camera id
        setStatusString(strerror(errno));
        return false;
      }
      _lti_debug(<< "Window properties after set:" << endl
           << "  x = " << videoWin.x << endl
           << "  y = " << videoWin.y << endl
           << "  width = " << videoWin.width << endl
           << "  height = " << videoWin.height << endl
           << "  chromakey = " << videoWin.chromakey << endl
           << "  framerate = "
           << ((videoWin.flags & PWC_FPS_FRMASK) >> PWC_FPS_SHIFT) << endl
           << "  snapshot = " << (videoWin.flags & PWC_FPS_SNAPSHOT) << endl);
#     endif
    }

    // --------------------------------------------------
    // image properties
    // --------------------------------------------------

    if ((lastUsedParams_.brightness != param.brightness) ||
        (lastUsedParams_.hue != param.hue) ||
        (lastUsedParams_.saturation != param.saturation) ||
        (lastUsedParams_.contrast != param.contrast) ||
        (lastUsedParams_.gamma != param.gamma)) {

      video_picture videoPic;

      // get the values
      if (ioctl(cameraHndl_,VIDIOCGPICT,&videoPic) < 0) {
        // some error getting camera id
        setStatusString(strerror(errno));
        return false;
      }

      videoPic.brightness = iround(param.brightness*65535.0f);
      videoPic.hue = iround(param.hue*65535.0f);
      videoPic.colour = iround(param.saturation*65535.0f);
      videoPic.contrast = iround(param.contrast*65535.0f);
      videoPic.whiteness = iround(param.gamma*65535.0f);
      videoPic.depth = 32;

      // the new kernel module accepts only this palette
      // videoPic.palette = VIDEO_PALETTE_RGB32;
      videoPic.palette = VIDEO_PALETTE_YUV420P;

      lastUsedParams_.brightness = param.brightness;
      lastUsedParams_.hue = param.hue;
      lastUsedParams_.saturation = param.saturation;
      lastUsedParams_.contrast = param.contrast;
      lastUsedParams_.gamma = param.gamma;

      // set the new values
      if (ioctl(cameraHndl_,VIDIOCSPICT,&videoPic) < 0) {
        // some error getting camera id
        setStatusString(strerror(errno));
        return false;
      }

#     if defined(_LTI_DEBUG) && _LTI_DEBUG >= 4
      if (ioctl(cameraHndl_,VIDIOCGPICT,&videoPic) < 0) {
        // some error getting camera id
        setStatusString(strerror(errno));
        return false;
      }

      _lti_debug("Image properties after set:" << endl
           << "  brightness = " << videoPic.brightness << endl
           << "  hue = " << videoPic.hue << endl
           << "  colour = " << videoPic.colour << endl
           << "  contrast = " << videoPic.contrast << endl
           << "  whiteness = " << videoPic.whiteness << endl
           << "  depth = " << videoPic.depth << endl
           << "  palette = " << videoPic.palette << endl);
#     endif
    } // end of image properties

    // --------------------------------------------------
    // philips stuff (private calls)
    // --------------------------------------------------

    int tmpint;

    if (lastUsedParams_.compression != param.compression) {
      // compression rate
      tmpint = int(param.compression);
      if (ioctl(cameraHndl_,VIDIOCPWCSCQUAL,&tmpint) < 0) {
        // some error getting camera id
        setStatusString(strerror(errno));
        return false;
      }

      lastUsedParams_.compression = param.compression;

#     if defined(_LTI_DEBUG) && _LTI_DEBUG >= 4
      tmpint = 0;
      if (ioctl(cameraHndl_,VIDIOCPWCGCQUAL,&tmpint) < 0) {
        // some error getting camera id
        setStatusString(strerror(errno));
        return false;
      }
      _lti_debug("  compression:" << tmpint << endl);
#     endif
    }


    if (!setGain(param.gainMode,param.gain) ||
        !setSharpness(param.sharpnessMode,param.sharpness) ||
        !setShutter(param.shutterMode,param.shutter) ||
        !setWhiteBalance(param.whiteBalanceMode,
                         param.whiteBalance,
                         param.blueGain,
                         param.redGain)) {
      return false;
    }

    if (panTilt_) {
      if (param.panMode == param.tiltMode) {
        if (!setPanTilt(param.panMode,
                        within(param.pan,
                               toAngleVal(toDegrees(minPan_,DeviceDependent),
                                          param.angularUnits),
                               toAngleVal(toDegrees(maxPan_,DeviceDependent),
                                          param.angularUnits)),
                        within(param.tilt,
                               toAngleVal(toDegrees(minTilt_,DeviceDependent),
                                          param.angularUnits),
                               toAngleVal(toDegrees(maxTilt_,DeviceDependent),
                                          param.angularUnits)))) {
          return false;
        }
      } else {
        if (!setPan(param.panMode,
                    within(param.pan,
                           toAngleVal(toDegrees(minPan_,DeviceDependent),
                                      param.angularUnits),
                           toAngleVal(toDegrees(maxPan_,DeviceDependent),
                                      param.angularUnits))) ||
            !setTilt(param.tiltMode,
                     within(param.tilt,
                            toAngleVal(toDegrees(minTilt_,DeviceDependent),
                                       param.angularUnits),
                            toAngleVal(toDegrees(maxTilt_,DeviceDependent),
                                       param.angularUnits)))) {
          return false;
        }
      }
    }

    // led status
    if (lastUsedParams_.led != param.led) {
      pwc_leds pwcLED;
      if (param.led <= 0) {
        // turn off the led
        pwcLED.led_on  = 0;
        pwcLED.led_off = 101;
      } else if (param.led == 1) {
        // turn on the led
        pwcLED.led_on  = 101;
        pwcLED.led_off = 0;
      } else {
        pwcLED.led_on  = param.led % 65536;
        pwcLED.led_off = param.led / 65536;
      }
      
      if (ioctl(cameraHndl_,VIDIOCPWCSLED,&pwcLED) < 0) {
        // some error getting camera id
        setStatusString(strerror(errno));
        return false;
      }

      lastUsedParams_.led = param.led;
      
#     if defined(_LTI_DEBUG) && _LTI_DEBUG >=4
      if (ioctl(cameraHndl_,VIDIOCPWCGLED,&pwcLED) < 0) {
        // some error getting camera id
        setStatusString(strerror(errno));
        return false;
      }
      
      _lti_debug4("  led on:  " << pwcLED.led_on << endl);
      _lti_debug4("  led off: " << pwcLED.led_off << endl);

#     endif
    }
    
    if ( (lastUsedParams_.whiteBalanceControlSpeed != 
          param.whiteBalanceControlSpeed) &&
         (lastUsedParams_.whiteBalanceControlDelay != 
          param.whiteBalanceControlDelay) ) {

      pwc_wb_speed pwcSpeed;
      
      pwcSpeed.control_speed = 
        iround((1.0f-param.whiteBalanceControlSpeed)*65535.0f);

      pwcSpeed.control_delay =
        iround(param.whiteBalanceControlDelay*65535.0f);
      
      if (ioctl(cameraHndl_,VIDIOCPWCSAWBSPEED,&pwcSpeed) < 0) {
        // some error getting camera id
        setStatusString(strerror(errno));
        return false;
      }

      lastUsedParams_.whiteBalanceControlSpeed=param.whiteBalanceControlSpeed;
      lastUsedParams_.whiteBalanceControlDelay=param.whiteBalanceControlDelay;
    }


    if (firstTime ||
        (lastUsedParams_.backlightCompensation!=param.backlightCompensation)) {
      tmpint = param.backlightCompensation ? 1 : 0;
      if (ioctl(cameraHndl_,VIDIOCPWCSBACKLIGHT,&tmpint) < 0) {
        // some error getting camera id
        setStatusString(strerror(errno));
        return false;
      }
      lastUsedParams_.backlightCompensation = param.backlightCompensation;
    }
    
    
    if (firstTime ||
        (lastUsedParams_.antiflickerMode != param.antiflickerMode)) {
      tmpint =  param.antiflickerMode ? 1 : 0;
      if (ioctl(cameraHndl_,VIDIOCPWCSFLICKER,&tmpint) < 0) {
        // some error getting camera id
        setStatusString(strerror(errno));
        return false;     
      }
      lastUsedParams_.antiflickerMode = param.antiflickerMode;
    }
 
    if (lastUsedParams_.denoise != param.denoise) {
      tmpint = static_cast<int>(param.denoise);
      if (ioctl(cameraHndl_,VIDIOCPWCSDYNNOISE,&tmpint) < 0) {
        // some error getting camera id
        setStatusString(strerror(errno));
        return false;     
      }
      lastUsedParams_.denoise = param.denoise;
    }
 
    return true;
  }

  // ----------------------------------------------------------------
  bool pwc::isInitialized() const {
    return initialized_;
  }

  bool pwc::isActive() const {
    return isInitialized() && (cameraHndl_>=0);
  }

  const std::string pwc::cameraName() const {
    return camName_;
  }

  bool pwc::supportedFrameratesAndResolutions(fvector& framerates,
                                              vector<ipoint>& resols,
                                              matrix<ubyte>& val) const {

    framerates.clear();
    resols.clear();
    val.clear();

    if (camType_<650) {
      resols.allocate(3);
      resols.at(0).set(128,96);
      resols.at(1).set(176,144);
      resols.at(2).set(352,288);

      framerates.allocate(8);
      static const float frdata[] = {3.75f,  5.f, 7.5f, 10.f,
                                     12.0f, 15.f,20.0f, 24.f};
      framerates.fill(frdata);
      val.assign(8,3,1);
      val.at(0,0)=0;
      val.at(0,1)=0;
      val.at(6,2)=0;
      val.at(7,2)=0;

      return true;
    } else if (camType_<700) {

      resols.allocate(6);

      resols.at(0).set(128,96);
      resols.at(1).set(160,120);
      resols.at(2).set(176,144);
      resols.at(3).set(320,240);
      resols.at(4).set(352,288);
      resols.at(5).set(640,480);

      framerates.allocate(6);
      for (int i=0;i<framerates.size();++i) {
        framerates.at(i) = (i+1)*5.0f;
      }

      val.assign(framerates.size(),resols.size(),1);
      val.at(3,5)=0;
      val.at(4,5)=0;
      val.at(5,5)=0;
      
      return true;
    } else {
      resols.allocate(3);

      resols.at(0).set(160,120);
      resols.at(1).set(320,240);
      resols.at(2).set(640,480);

      framerates.allocate(6);
      for (int i=0;i<framerates.size();++i) {
        framerates.at(i) = (i+1)*5.0f;
      }

      val.assign(framerates.size(),resols.size(),1);
      val.at(3,2)=0;
      val.at(4,2)=0;
      val.at(5,2)=0;

      return true;
    }
   
    return false;
  }


  // load a color image from the usb port
  bool pwc::apply(image& theImage) {

    if (!isInitialized()) {
      theImage.clear();
      // the error message should still be there...
      return false;      
    }

    // file descriptor set
    fd_set fdSet;
    timeval tv;
    int retval;

    // Watch camera to see when it has input.
    FD_ZERO(&fdSet);
    FD_SET(cameraHndl_, &fdSet);

    /* Wait up to two seconds. */
    tv.tv_sec = 2;
    tv.tv_usec = 0;

    retval = select(cameraHndl_+1, &fdSet, NULL, NULL, &tv);

    // Don't rely on the value of tv now!

    if (retval) {
      int res;

      int bufferSize = resolution_.x*resolution_.y*3/2;
      theImage.allocate(resolution_);

      assert(bufferSize>0);

      ubyte* buffer = new ubyte[bufferSize];

      // read image in Yuv 4:2:0 format (the only format available)
      res = ::read(cameraHndl_,(void*)buffer,bufferSize);

      if (res<0) {
        setStatusString(strerror(errno));
        return false;
      }

      ubyte* pxptr = reinterpret_cast<ubyte*>(theImage.data());
      pxptr += (resolution_.x*resolution_.y*4);

      ubyte *Y = buffer;
      ubyte *u = &buffer[resolution_.x*resolution_.y];
      ubyte *v = &buffer[5*resolution_.x*resolution_.y/4];
      ubyte *Yend;

      if (getParameters().turnAround) {
        pxptr = reinterpret_cast<ubyte*>(theImage.data());
        pxptr += (resolution_.x*resolution_.y*4);

        int yy,sy;

        const int usize = resolution_.x/2;

        for (sy=0;sy<resolution_.y;++sy) {
          Yend = Y+resolution_.x;
          while (Y != Yend) {
            yy = (*Y)<<8;

            // dummy
            --pxptr;
            // red
            --pxptr;
            (*pxptr) = lutSaturation_[((yy+lutVr_[*v]) >> 8)&0x3FF];
            // green
            --pxptr;
            (*pxptr) = lutSaturation_[((yy-lutUg_[*u]-lutVg_[*v]) >> 8)&0x3FF];
            // blue
            --pxptr;
            (*pxptr) = lutSaturation_[((yy+lutUb_[*u]) >> 8) & 0x3FF];


            // next pixel doesn't change uv values
            ++Y;
            yy = (*Y)<<8;

            // dummy
            --pxptr;
            // red
            --pxptr;
            (*pxptr) = lutSaturation_[((yy+lutVr_[*v]) >> 8)&0x3FF];
            // green
            --pxptr;
            (*pxptr) = lutSaturation_[((yy-lutUg_[*u]-lutVg_[*v]) >> 8)&0x3FF];
            // blue
            --pxptr;
            (*pxptr) = lutSaturation_[((yy+lutUb_[*u]) >> 8)&0x3FF];

            ++Y;
            ++u;
            ++v;
          }
          if ((sy & 1) == 0) {
            u-=usize;
            v-=usize;
          }
        }

      } else {
        pxptr = reinterpret_cast<ubyte*>(theImage.data());

        int yy,sy;

        const int usize = resolution_.x/2;

        for (sy=0;sy<resolution_.y;++sy) {
          Yend = Y+resolution_.x;
          while (Y != Yend) {
            yy = (*Y)<<8;

            // blue
            (*pxptr) = lutSaturation_[((yy+lutUb_[*u])>>8)&0x3FF];
            pxptr++;
            // green
            (*pxptr) = lutSaturation_[((yy-lutUg_[*u]-lutVg_[*v])>>8)&0x3FF];
            pxptr++;
            // red
            (*pxptr) = lutSaturation_[((yy+lutVr_[*v])>>8)&0x3FF];
            pxptr++;
            // dummy
            pxptr++;

            // next pixel doesn't change uv values
            ++Y;
            yy = (*Y)<<8;

            // blue
            (*pxptr) = lutSaturation_[((yy+lutUb_[*u])>>8)&0x3FF];
            pxptr++;
            // green
            (*pxptr) = lutSaturation_[((yy-lutUg_[*u]-lutVg_[*v])>>8)&0x3FF];
            pxptr++;
            // red
            (*pxptr) = lutSaturation_[((yy+lutVr_[*v])>>8)&0x3FF];
            pxptr++;
            // dummy
            pxptr++;

            ++Y;
            ++u;
            ++v;
          }
          if ((sy & 1) == 0) {
            u-=usize;
            v-=usize;
          }
        }
      }

      delete[] buffer;

    } else {
      theImage.clear();
      setStatusString("Timeout.  No image received");
      return false;
    }

    FD_ZERO(&fdSet);

    return true;
  }

  // load a grey value channel from the
  bool pwc::apply(channel8& theChannel) {
    if (!isInitialized()) {
      // if not initialized yet, do it now!
      theChannel.clear();
      // the error message should still be there...
      return false;      
    }

    // file descriptor set
    fd_set fdSet;
    timeval tv;
    int retval;

    // Watch camera to see when it has input.
    FD_ZERO(&fdSet);
    FD_SET(cameraHndl_, &fdSet);

    /* Wait up to two seconds. */
    tv.tv_sec = 2;
    tv.tv_usec = 0;

    retval = select(cameraHndl_+1, &fdSet, NULL, NULL, &tv);

    // Don't rely on the value of tv now!

    if (retval) {
      int res;

      // read the Y channel
      theChannel.allocate(resolution_);
      res = ::read(cameraHndl_,(void*)theChannel.data(),
                   resolution_.x*resolution_.y);
      if (res<0) {
        setStatusString(strerror(errno));
        return false;
      }
      // read the rest of color information
      int bufferSize = resolution_.x*resolution_.y/2;
      ubyte* buffer = new ubyte[bufferSize];
      res = ::read(cameraHndl_,(void*)buffer,bufferSize);
      delete[] buffer;

      if (res<0) {
        setStatusString(strerror(errno));
        return false;
      }
    } else {
      theChannel.clear();
      setStatusString("Timeout.  No image received");
      return false;
    }

    FD_ZERO(&fdSet);

    return true;
  }

  void pwc::initializeLUTs() {
    if (lutSaturation_ == 0) {
      int i;
      /*
       * look up table to accelerate computation of saturation
       */
      static ubyte tmpSat[1024];
      for (i=-256;i<512;++i) {
        tmpSat[i & 0x3FF] = lti::within(i,0,255);
      }
      lutSaturation_ = tmpSat;

      /*
       * look up tables to accelerate conversion Luv -> RGB
       */
      //       int* lutUgTmp = new int[256];
      //       int* lutUbTmp = new int[256];
      //       int* lutVgTmp = new int[256];
      //       int* lutVrTmp = new int[256];

      static int lutUgTmp[256];
      static int lutUbTmp[256];
      static int lutVgTmp[256];
      static int lutVrTmp[256];      

      for (i=0;i<256;++i) {
        lutUgTmp[i] = (i-128)*88;
        lutUbTmp[i] = (i-128)*454;
        lutVgTmp[i] = (i-128)*183;
        lutVrTmp[i] = (i-128)*359;
      }

      lutUg_ = lutUgTmp;
      lutUb_ = lutUbTmp;
      lutVg_ = lutVgTmp;
      lutVr_ = lutVrTmp;

    }


  }

} // namespace lti

#endif // __linux__
#endif // _USE_PHILIPS_TOUCAM
