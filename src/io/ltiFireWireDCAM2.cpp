/*
 * Copyright (C) 2004, 2005, 2006
 * Arnd Hannemann, RWTH-Aachen 
 * Copyright (C) 2007, 2008, 2009, 2010, 2011, 2012
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
 * \file   ltiFireWireDCAM.cpp
 *         Contains a class for grabbing images from FireWire cameras
 *         using libdc1394 version 2
 * \author Arnd Hannemann
 * \author Pablo Alvarado
 * \date   21.10.2004
 * $Id: ltiFireWireDCAM2.cpp,v 1.2 2012-03-26 03:07:41 alvarado Exp $
 */

#include "ltiHardwareConfig.h"

#if defined(_USE_FIRE_WIRE_DCAM) && defined(_LTI_DC1394_VER_2)

#include "ltiFireWireDCAM2.h"
#include "ltiMutex.h"
#include "ltiLispStreamHandler.h"
#include "ltiTimer.h"
#include "ltiFactory.h"
#include "ltiPassiveWait.h"

#include <cstring>
#include <sstream>
#include <fstream>

#include<limits>

#undef _LTI_DEBUG
//#define _LTI_DEBUG 2
#include "ltiDebug.h"

namespace lti {

  //
  // Local and private constants
  //
  enum {
    Delay=50000 // 50 ms delay
  };

  // --------------------------------------------------
  // fireWireDCAM::parameters
  // --------------------------------------------------

  std::string fireWireDCAM::parameters::cameraNamesFile = "cameras.txt";

  // default constructor
  fireWireDCAM::parameters::parameters()
    : frameGrabber::parameters(),
      camera::parameters(),
      panTiltUnit::parameters(),
      lensUnit::parameters() {
    
    fixMode = Upload;
    resolution.set(640,480);
    encoding = YUV422;
    framerate = 15.0f;
    
    brightnessMode = FeatureAuto;
    brightness = 0;

    exposureMode = FeatureAuto;
    exposure=0;

    sharpnessMode = FeatureAuto;
    sharpness = 0;

    whiteBalanceMode = FeatureAuto;
    redGain = 0;
    blueGain = 0;
    
    hueMode = FeatureAuto;
    hue = 0;

    saturationMode = FeatureAuto;
    saturation = 0;

    gammaMode = FeatureAuto;
    gamma = 0;

    shutterMode = FeatureAuto;
    shutter = 0;

    gainMode = FeatureAuto;
    gain = 0;

    irisMode = FeatureAuto;
    iris = 0;

    focusMode = FeatureAuto;
    focus = 0;

    zoomMode = FeatureAuto;
    zoom = 0;

    panMode = FeatureAuto;
    pan = 0;

    tiltMode = FeatureAuto;
    tilt = 0;

    opticalFilterMode = FeatureAuto;
    opticalFilter=0;

    waitOnePush = false;
    onePushUpdateRate = 2.0f;
    onePushTimeout = 10.0f;

    snapShotMode = true;
    dmaBufferSize = 2;
    dropFrames = true;
    deviceFile = "/dev/video1394/0";

    bayerMethod = "bilinearDemosaicing";
    bayerPattern = bayerDemosaicing::RGGB;

    cameraName = "Default";
  }

  // copy constructor
  fireWireDCAM::parameters::parameters(const parameters& other)
    : frameGrabber::parameters(),
      camera::parameters(),
      panTiltUnit::parameters(),
      lensUnit::parameters() {
    copy(other);
  }

  // destructor
  fireWireDCAM::parameters::~parameters() {
  }

  // class name
  const std::string& fireWireDCAM::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone member
  fireWireDCAM::parameters* fireWireDCAM::parameters::clone() const {
    return new parameters(*this);
  }

  // newInstance member
  fireWireDCAM::parameters* fireWireDCAM::parameters::newInstance() const {
    return new parameters();
  }

  // copy member

  fireWireDCAM::parameters&
  fireWireDCAM::parameters::copy(const parameters& other) {
    frameGrabber::parameters::copy(other);
    camera::parameters::copy(other);
    panTiltUnit::parameters::copy(other);
    lensUnit::parameters::copy(other);
    
    fixMode=other.fixMode;
    encoding=other.encoding;
    
    dmaBufferSize = other.dmaBufferSize;
    dropFrames = other.dropFrames;
    deviceFile = other.deviceFile;

    bayerMethod  = other.bayerMethod;
    bayerPattern = other.bayerPattern;

    // cameraNamesFile = other.cameraNamesFile; // not necessary (is static)
    cameraName = other.cameraName;

    return *this;
  }
    
  // alias for copy member
  fireWireDCAM::parameters&
  fireWireDCAM::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  /*
   * write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was succeful
   */
  bool fireWireDCAM::parameters::write(ioHandler& handler,
                                       const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }
    
    b = b && lti::write(handler,"fixMode",fixModeName(fixMode));
    b = b && lti::write(handler,"encoding",encodingName(encoding));
    b = b && lti::write(handler,"dmaBufferSize",dmaBufferSize);
    b = b && lti::write(handler,"dropFrames",dropFrames);
    b = b && lti::write(handler,"deviceFile",deviceFile);
    
    b = b && lti::write(handler,"bayerMethod",bayerMethod);
    b = b && lti::write(handler,"bayerPattern",bayerPattern);
    
    b = b && lti::write(handler,"cameraNamesFile",cameraNamesFile);
    b = b && lti::write(handler,"cameraName",cameraName);      
    
    b = b && camera::parameters::write(handler,false);
    b = b && panTiltUnit::parameters::write(handler,false);
    b = b && lensUnit::parameters::write(handler,false);
    b = b && frameGrabber::parameters::write(handler,false);

    if (complete) {
      b = b && handler.writeEnd();
    }

    return b;
  }

  // convert the enum to a string with its name
  const char* 
  fireWireDCAM::parameters::encodingName(const eColorMode& code) const {
    switch (code) {
    case Mono8:
      return "Mono8";
      break;
    case Mono16:
      return "Mono16";
      break;
    case Mono16s:
      return "Mono16s";
      break;
    case YUV411:
      return "YUV411";
      break;
    case YUV422:
      return "YUV422";
      break;
    case YUV444:
      return "YUV444";
      break;
    case RGB8:
      return "RGB8";
      break;
    case RGB16:
      return "RGB16";
      break;
    case RGB16s:
      return "RGB16s";
      break;
    case Raw8:
      return "Raw8";
      break;
    case Raw16:
      return "Raw16";
      break;
    default:
      return "UnknownEncoding";
      break;
    }
    
    return "Unknown";
  }

  // read format 7 code mode
  fireWireDCAM::eColorMode 
  fireWireDCAM::parameters::getEncoding(const std::string& str) const {
    
    if (str.find("ono8") != std::string::npos) {
      return Mono8;
    } else if (str.find("ono16s") != std::string::npos) {
      return Mono16s;
    } else if (str.find("ono16") != std::string::npos) {
      return Mono16;
    } else if (str.find("YUV411") != std::string::npos) {
      return YUV411;
    } else if (str.find("YUV422") != std::string::npos) {
      return YUV422;
    } else if (str.find("YUV444") != std::string::npos) {
      return YUV444;
    } else if (str.find("RGB8") != std::string::npos) {
      return RGB8;
    } else if (str.find("RGB16s") != std::string::npos) {
      return RGB16s;
    } else if (str.find("RGB16") != std::string::npos) {
      return RGB16;
    } else if (str.find("aw8") != std::string::npos) {
      return Raw8;
    } else if (str.find("aw16") != std::string::npos) {
      return Raw16;
    } if (str.find("ONO8") != std::string::npos) {
      // Try also the names used by libdc1394 (uppercase)
      return Mono8;
    } else if (str.find("ONO16S") != std::string::npos) {
      return Mono16s;
    } else if (str.find("ONO16") != std::string::npos) {
      return Mono16;
    } else if (str.find("RGB16S") != std::string::npos) {
      return RGB16s;
    } else if (str.find("AW8") != std::string::npos) {
      return Raw8;
    } else if (str.find("AW16") != std::string::npos) {
      return Raw16;
    }
    
    return UnknownEncoding;
  } 

  /*
   * Get a string for the fix mode
   */
  const char* 
  fireWireDCAM::parameters::fixModeName(const eFixMode& mode) const {
    switch(mode) {
      case AutoFix:
        return "AutoFix";
        break;
      case Upload:
        return "Upload";
        break;
      default:
        return "NoFix";
    }
    return 0;
  }

  /*
   * For the given string, return a valid fix mode 
   */
  fireWireDCAM::eFixMode 
  fireWireDCAM::parameters::getFixMode(const std::string& nme) const {
    if (nme.find("uto") != std::string::npos) {
      return AutoFix;
    } else if (nme.find("oad")) {
      return Upload;
    } 
    return NoFix;
  }

  /*
   * read the parameters from the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also read, otherwise only the data block will be read.
   * @return true if read was succeful
   */
  bool fireWireDCAM::parameters::read(ioHandler& handler,
                                        const bool complete) {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    std::string str;
    b = b && lti::read(handler,"fixMode",str);
    fixMode = getFixMode(str);
    b = b && lti::read(handler,"encoding",str);
    encoding = getEncoding(str);

    b = b && lti::read(handler,"dmaBufferSize",dmaBufferSize);
    b = b && lti::read(handler,"dropFrames",dropFrames);
    b = b && lti::read(handler,"deviceFile",deviceFile);

    b = b && lti::read(handler,"bayerMethod",bayerMethod);
    b = b && lti::read(handler,"bayerPattern",bayerPattern);

    b = b && lti::read(handler,"cameraNamesFile",cameraNamesFile);
    b = b && lti::read(handler,"cameraName",cameraName);

    b = b && camera::parameters::read(handler,false);
    b = b && panTiltUnit::parameters::read(handler,false);
    b = b && lensUnit::parameters::read(handler,false);
    b = b && frameGrabber::parameters::read(handler,false);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

  // --------------------------------------------------
  // fireWireDCAM::cameraSet
  // --------------------------------------------------
  fireWireDCAM::cameraSet::cameraSet()
    : dc1394_(0) {

    // initialize library context
    dc1394_ = dc1394_new();
  }

  fireWireDCAM::cameraSet::~cameraSet() {
    deleteCameras();

    // finish library context
    dc1394_free(dc1394_);
    
  }
  
  const fireWireDCAM::cameraInfo& 
  fireWireDCAM::cameraSet::operator[](const int idx) const {
    assert(static_cast<unsigned int>(idx)<cams_.size());
    return cams_[idx];
  }

  fireWireDCAM::cameraInfo& 
  fireWireDCAM::cameraSet::operator[](const int idx) {
    assert(static_cast<unsigned int>(idx)<cams_.size());
    return cams_[idx];
  }
  
  unsigned int fireWireDCAM::cameraSet::size() const {
    return cams_.size();
  }

  bool fireWireDCAM::cameraSet::findCamera(const uint64_t& guid,
                                           int& camIdx) const {
    unsigned int i;
    camIdx=-1;
    lock_.lock();
    for (i=0;i<cams_.size();++i) {
      if (cams_[i].info->guid == guid) {
        camIdx=i;
        i=cams_.size(); // break;
      }
    }
    lock_.unlock();
    return (camIdx>=0);
  }

  bool fireWireDCAM::cameraSet::findFreeCamera(int& camIdx) const {
    unsigned int i;
    camIdx=-1;
    lock_.lock();
    for (i=0;i<cams_.size();++i) {
      if (!cams_[i].inUse) {
        camIdx=i;
        i=cams_.size(); // break;
      }
    }
    lock_.unlock();
    return (camIdx>=0);
  }

  bool fireWireDCAM::cameraSet::reserveIndex(const int idx) {
    lock_.lock();
    bool inUse = cams_[idx].inUse;  // if this is true, cam is already in use
    if (!inUse) {
      cams_[idx].inUse = true;
    }
    lock_.unlock();
    return (!inUse);
  }

  bool fireWireDCAM::cameraSet::releaseIndex(const int idx) {
    if (!cams_[idx].inUse) {
      return false; // cannot be released, because is not in use!
    }
    lock_.lock();
    cams_[idx].inUse = false;
    lock_.unlock();
    return true;
  }

  bool fireWireDCAM::cameraSet::getCameras() {

    // The following code has been adapted from the method GetCameraNodes in
    // the file camera.c of coriander (by Damien Douxchamps)
    lock_.lock();

    deleteCameras();

    dc1394camera_list_t* cameraList;   // List of cameras 

    // Get the camera list
    dc1394_camera_enumerate(dc1394_,&cameraList);
    
    // Create a list of cameras with coriander's camera type camera_t
    for (unsigned int i=0;i<cameraList->num;++i) {
      // add an empty structure to the vector
      cams_.push_back(cameraInfo());
      // get a reference to the new element
      cameraInfo& cam = cams_.back();
      // initialize camera get all properties of that camera
      cam.init(dc1394_,cameraList->ids[i].guid);
    }
    
    // free camera list:
    dc1394_camera_free_list(cameraList);
    lock_.unlock();

    return !cams_.empty();
  }

  bool fireWireDCAM::cameraSet::deleteCameras() {
    // Create a list of cameras with coriander's camera type camera_t
    for (unsigned int i=0;i<cams_.size();++i) {
      cams_[i].free();
    }

    return cams_.empty();
  }

  // --------------------------------------------------
  // fireWireDCAM::cameraInfo
  // --------------------------------------------------
  fireWireDCAM::cameraInfo::cameraInfo() 
    : info(0),videoMode(),inUse(false) {
    // ensure that all data is initialized to some value
  }

  fireWireDCAM::cameraInfo::~cameraInfo() {
    inUse=false;
    free();
  }

  bool fireWireDCAM::cameraInfo::init(dc1394_t *dc1394, uint64_t guid) {

    if (notNull(info)) {
      dc1394_camera_free(info);
      info=0;
    }

    info=dc1394_camera_new(dc1394,guid);
    if (isNull(info)) {
      errorStr="Could not create new camera object";
      return false;
    }

    bool error=false;
    // initialize the structure to avoid non-sense data
    memset(&featureSet,0,sizeof(dc1394featureset_t));
    if (dc1394_feature_get_all(info,&featureSet)!=DC1394_SUCCESS) {
      errorStr="Could not get camera feature information.";
      error=true;
    }

    error = error || !getFormat7Capabilities();
    error = error || !getSupportedModesAndFramerates();
    
    return !error;
  }

  void fireWireDCAM::cameraInfo::free() {
    if (notNull(info)) {
      dc1394_camera_free(info);
      info=0;
    }
  }

  bool fireWireDCAM::cameraInfo::getFormat7Capabilities() {
    int i;
    format7.supported=false;
    
    for (i=0;i<DC1394_VIDEO_MODE_FORMAT7_NUM;++i) {
      format7.modeSet.mode[i].present=DC1394_FALSE;
    }

    if (dc1394_format7_get_modeset(info, &format7.modeSet)!=DC1394_SUCCESS) {
      errorStr ="Could not query format 7 information";
      return false;
    }
    
    for (i=0;i<DC1394_VIDEO_MODE_FORMAT7_NUM;++i) {
      if (format7.modeSet.mode[i].present!=0) {
        format7.supported=true;
        break;
      }
    }

    return true;
  }


  bool fireWireDCAM::cameraInfo::getSupportedModesAndFramerates() {

    // clean all previous information, just in case
    supportedModes.clear();
    supportedFramerates.clear();
    
    if (isNull(info)) {
      errorStr = "Camera not set up";
      return false;
    }

    dc1394video_modes_t modes;
    if (dc1394_video_get_supported_modes(info,&modes)<0) {
      errorStr = "Could not get supported formats";
      return false;
    };

    dc1394framerates_t rates;
    
    for (uint32_t i=0;i<modes.num;++i) {
      dc1394video_mode_t mode = modes.modes[i];
      supportedModes.insert(mode);
      
      if (dc1394_is_video_mode_scalable(mode)) {
        // format 7 has no fixed framerates: add all, just in case

        // pablo alvarado: it is not clear to me how to correctly manage this
        //                 issue, since I could not find any documentation on
        //                 this particular topic.  The chosen way of adding all
        //                 framerates is not reliable since the camera may not
        //                 support any of the framerates at all.  In coriander
        //                 just no framerate is set, but this is not compatible
        //                 with the LTI-Lib way.  I hope the get_framerate
        //                 methods at least provide the proper information.
        std::set<float>& supported = supportedFramerates[mode];
        for (float fr=240.f;fr>1.0;fr/=2.f) {
          supported.insert(fr);
        }
      } else {
        if (dc1394_video_get_supported_framerates(info,mode,&rates)
            !=DC1394_SUCCESS) { 
          errorStr = "Could not get supported framerates";
          return false;
        } 

        std::set<float>& supported = supportedFramerates[mode];
        for (uint32_t j=0;j<rates.num;++j) {
          dc1394framerate_t rate=rates.framerates[j];
          float frate=0.f;
          dc1394_framerate_as_float(rate,&frate);
          supported.insert(frate);
        }
      }
    }

     
    return true;
  }

  // --------------------------------------------------
  // fireWireDCAM::nameDB
  // --------------------------------------------------
  fireWireDCAM::nameDB::nameDB() 
  : init_(false),modified_(false) {
  }

  fireWireDCAM::nameDB::~nameDB() {
    dump();
  }
  
  
  bool fireWireDCAM::nameDB::use(const std::string& filename) {
    if ((filename != filename_) || !init_) {
      lock_.lock();
      if ((filename != filename_) && init_) {
        lock_.unlock(); // avoid a dead lock()
        dump();
        lock_.lock(); // continue...
      }
      if ( (init_ = read(filename)) ) {
        filename_=filename;      
      } else {
        filename_.clear(); // to avoid future problems... use it as a flag
      }
      lock_.unlock();
    }
    return init_;
  }

  bool fireWireDCAM::nameDB::query(const std::string& name,
                                   uint64_t& id) const {    
    bool error = false;
    if (!init_) {
      return false;
    }
    lock_.lock();
    std::map<std::string,uint64_t>::const_iterator it;
    it = data_.find(name);
    if (it != data_.end()) {
      id=(*it).second;
    } else {
      id = 0; // invalid value
      error = true;
    }
    lock_.unlock();
    return !error;
  }

  bool fireWireDCAM::nameDB::query(const uint64_t& id,
                                   std::string& name) const {    
    if (!init_) {
      return false;
    }
    lock_.lock();

    std::map<std::string,uint64_t>::const_iterator it,eit;

    // the only to search for id is with a linear search
    for (it=data_.begin(),eit=data_.end();it!=eit;++it) {
      if ((*it).second == id) {
        name = (*it).first;
        lock_.unlock();
        return true;
      }
    }

    lock_.unlock();
    name.clear();
    return false;
  }


  bool fireWireDCAM::nameDB::add(const std::string& name,uint64_t& id) {
    bool error = false;
    if (!init_) {
      return false;
    }

    lock_.lock();
    // try to find if the data already exists
    std::map<std::string,uint64_t>::const_iterator it;
    it = data_.find(name);
    if (it != data_.end()) {
      error = true; // cannot add it!  It already exists!
    } else {
      data_[name]=id; // add the data
      modified_=true;
    }
    lock_.unlock();
    return !error;
  }
  
  bool fireWireDCAM::nameDB::del(const std::string& name) {
    bool error = false;
    if (!init_) {
      return false;
    }
    lock_.lock();
    std::map<std::string,uint64_t>::iterator it;
    it = data_.find(name);
    if (it != data_.end()) {
      data_.erase(it);
      modified_=true;
    } else {
      error = true;
    }
    lock_.unlock();
    return !error;
  }
  
  bool fireWireDCAM::nameDB::dump() {
    bool error = true;
    lock_.lock();

    if (!init_) {
      lock_.unlock();
      return false;
    }
    if (!modified_) {
      lock_.unlock();
      return true;
    }

    std::ofstream out(filename_.c_str());
    if (out) {
      error=false;
      lispStreamHandler lsh(out);
      lsh.writeComment("Camera name and corresponding hex ID");
      lsh.writeComment("This file will be automatically modified");
      out << std::endl;

      std::string str;
      std::map<std::string,uint64_t>::const_iterator it;
      for (it=data_.begin();it!=data_.end();++it) {
        hexToString((*it).second,str);
        lsh.writeBegin();
        lsh.write((*it).first);
        lsh.writeKeyValueSeparator();
        lsh.write(str);
        lsh.writeEnd();
        lsh.writeEOL();
      }
      out << std::endl;
      out.close();
    }
    lock_.unlock();
    return !error;
  }

  bool fireWireDCAM::nameDB::read(const std::string& filename) {
    bool error = false;
    modified_=false;

    std::ifstream in(filename.c_str());
    if (in) {
      lispStreamHandler lsh(in);
      data_.clear(); // erase all data known before, since we will get it all
                     // from a file
      // we read lines until we get an error
      bool cont=true;
      std::string name,id;
      uint64_t d;
      while (cont && lsh.tryBegin()) {
        bool b = lsh.read(name);
        b = b && lsh.readKeyValueSeparator();
        b = b && lsh.read(id);
        b = b && lsh.readEnd();
        if (b) {
          if (!stringToHex(id,d)) {
            error=true;
            break;
          }
          data_[name]=d;
        } else {
          cont = false;
        }
      }
      in.close();
    } else {
      // well, maybe the file does not exist.  Create it empty
      std::ofstream out(filename.c_str());
      if (out) {
        out << std::endl;
        out.close();
      } else {
        error = true;
      }
    }
    return !error;  
  }

  void fireWireDCAM::nameDB::hexToString(const uint64_t& data,
                                         std::string& str) const {

    static const char d2a[] = {'0','1','2','3','4','5','6','7','8','9',
                               'A','B','C','D','E','F'};
    str.clear();
    uint64_t d = data;
    uint32 n;

    do {
      n= static_cast<uint32>(d & 0x0F);
      str = d2a[n]+str;// append to the left the new digit
      d=d>>4;// take the lowest 4 bits away shifting at the same time the rest.
    } while(d!=0);
    str="0x"+str;
  }
  
  bool fireWireDCAM::nameDB::stringToHex(const std::string& str,
                                         uint64_t& data) const {

    // to avoid segfaults, just fill 128 chars of ascii to nibble LUT.
    static const char a2d[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                               0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                               0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                               0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 0, 0, 0, 0, 0,
                               0,10,11,12,13,14,15, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                               0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                               0,10,11,12,13,14,15, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                               0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    data = 0;
    if (str.find("0x") == 0) {
      unsigned int i = 2;
      while (i<str.length()) {
        data = data<<4;// shift 4 bits to the left
        data+=a2d[static_cast<int>(str[i] & 0x7F)];
        ++i;
      }
      return true;
    } 
    return false;
  }

  // --------------------------------------------------
  // fireWireDCAM::static attributes
  // --------------------------------------------------
  fireWireDCAM::cameraSet fireWireDCAM::cams_;
  fireWireDCAM::nameDB fireWireDCAM::cameraNames_;

  // --------------------------------------------------
  // fireWireDCAM
  // --------------------------------------------------

  // default constructor
  fireWireDCAM::fireWireDCAM()
    : frameGrabber(),camera(),panTiltUnit(),lensUnit() {

    activeCamera_=-1; // set an invalid value
    camera_=0;

    snapShotCapable_ = false;

    features_=0;

    colorMode_ = YUV422;
    captureSetUp_ = false;

    bayer_ = 0;
   
    // create an instance of the parameters with the default values
    parameters defaultParameters;
    // set the default parameters
    setParameters(defaultParameters);
  }

  // this constructor must not be implemented!!!
  fireWireDCAM::fireWireDCAM(const parameters& par)
    : frameGrabber(),camera(),panTiltUnit(),lensUnit() {
    
    activeCamera_=-1; // set an invalid value
    camera_=0;

    snapShotCapable_ = false;

    features_=0;

    colorMode_ = YUV422;
    
    captureSetUp_ = false;
   
    bayer_ = 0;

    // set the default parameters
    setParameters(par);
  }


  // copy constructor
  fireWireDCAM::fireWireDCAM(const fireWireDCAM& other)
    : frameGrabber(),camera(),panTiltUnit(),lensUnit() {

    activeCamera_=-1; // set an invalid value
    camera_=0;

    snapShotCapable_ = false;

    features_=0;

    colorMode_ = YUV422;
    
    captureSetUp_ = false;

    bayer_ = 0;

    copy(other);
  }

  // destructor
  fireWireDCAM::~fireWireDCAM() {
    releaseCamera();

    delete bayer_;
    bayer_=0;
  }

  // class name
  const std::string& fireWireDCAM::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone member
  fireWireDCAM* fireWireDCAM::clone() const {
    return new fireWireDCAM(*this);
  }

  // newInstance member
  fireWireDCAM* fireWireDCAM::newInstance() const {
    return new fireWireDCAM();
  }

  // copy member
  fireWireDCAM&
  fireWireDCAM::copy(const fireWireDCAM& other) {
    frameGrabber::copy(other);
    
    // It makes no sense to "copy" a grabbing functor here, since
    // only one instance can talk to one camera at the same time.
    // The only possible meaning is to copy the parameters, an let
    // the updateParameters do its job.  It may fail

    // Free all camera stuff here! 
    releaseCamera();     
    
    return (*this);
  }

  // alias for copy member
  fireWireDCAM&
  fireWireDCAM::operator=(const fireWireDCAM& other) {
    return (copy(other));
  }

  // return parameters
  const fireWireDCAM::parameters& fireWireDCAM::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if (isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }

  // return parameters
  fireWireDCAM::parameters& fireWireDCAM::getRWParameters() {
    parameters* par = dynamic_cast<parameters*>(&functor::getRWParameters());
    if (isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }

  bool fireWireDCAM::isActive() const {
    return (activeCamera_>=0);
  }

  bool fireWireDCAM::hasCameraFormat7() const {
    if (isActive()) { // only if the camera is active, the array
                            // contains meaningful data
      return cams_[activeCamera_].format7.supported;
    }
    return false;
  }

  bool fireWireDCAM::releaseCamera() {
    if (!isActive()) {
      // nothing to release
      return true;
    }

    // stop transmission
    isoFlowStopTotal();

    if (captureSetUp_) {
      _lti_debug("release old camera structure...\n");

      // remember: even if this is called release "camera", it just
      // releases the "capture" relevant stuff
      dc1394_capture_stop(camera_);
      captureSetUp_ = false;
    }

    if (camera_->can_switch_on_off) {
      // we try to turn camera off, but it may not work
      dc1394_camera_set_power(camera_,DC1394_OFF);
    }

    cams_.releaseIndex(activeCamera_);
    activeCamera_=-1;
    camera_=0;
    features_=0;
    activeCameraName_.clear();
   
    return true;
  }

  eCamFeatureMode 
  fireWireDCAM::fixMode(const unsigned int states,
                        const eCamFeatureMode desired) const {

    if ((states & desired)==0) {
      // oh oh, desired state not supported, so let us just priorize a bit
      if ((states & FeatureAuto) != 0) {
        return FeatureAuto;          
      } else if ((states & FeatureManual) != 0) {
        return FeatureManual;
      } else if ((states & FeatureAbsolute) != 0) {
        return FeatureAbsolute;
      } else if ((states & FeatureOnePush) != 0) {
        return FeatureOnePush;
      } else if ((states & FeatureOff) != 0) {
        return FeatureOff;
      }
      return FeatureNA; // invalid value
    }
    return desired; // feature supported
  }

  void fireWireDCAM::fixFeature(const unsigned int featureId,
                                float& value,
                                eCamFeatureMode& state) const {

    if (!isActive()) {
      // no active camera => nothing to do
      return;
    }

    if (isFeatureAvailable(featureId)) {
      const dc1394feature_info_t& fts = 
        features_->feature[featureId-DC1394_FEATURE_MIN];
      if (value < static_cast<float>(fts.min)) {
        value = static_cast<float>(fts.min);
      } else if (value > static_cast<float>(fts.max)) {
        value = static_cast<float>(fts.max);
      }

      // fix mode
      unsigned int states = checkFeatureModes(featureId);
      state = fixMode(states,state);
    } else {
      value=0;
      state=FeatureNA;
    }

  }


  bool fireWireDCAM::fixParameters(parameters& par) const {
    if (!isActive()) {
      setStatusString("Cannot fix parameters of a non-active camera");
      return false;
    }

    const cameraInfo& cam = cams_[activeCamera_];

    // we have to keep most attributes of par untouched, but the following
    // have to be copied from the current set:
    par.cameraName = activeCameraName_;
    par.deviceFile = getParameters().deviceFile;

    // first, get the resolutions and encodings available in the IIDC standard
    dc1394video_mode_t mode;
    mode = convertMode(par.resolution,par.encoding);

    // now, we have to check if the mode is supported.
    // If it is a mode of the format7, then due to the implementation of 
    // the previous method, it will be supported.  If the mode is of formats
    // 0, 1 or 2, then we may require many adaptions
    if (mode >= DC1394_VIDEO_MODE_FORMAT7_MIN) {
      mode = computeClosestMode(mode);      
    } 

    convertMode(mode,par.resolution,par.encoding); // get it back to LTI-Lib
    par.framerate = computeClosestFrameRate(mode,par.framerate);

    // now use the camera information to adjust some values if necessary
    fixFeature(DC1394_FEATURE_BRIGHTNESS,par.brightness,par.brightnessMode);
    fixFeature(DC1394_FEATURE_EXPOSURE,par.exposure,par.exposureMode);
    fixFeature(DC1394_FEATURE_SHARPNESS,par.sharpness,par.sharpnessMode);

    if (isFeatureAvailable(DC1394_FEATURE_WHITE_BALANCE)) {
      const dc1394feature_info_t& fi = 
        cam.featureSet.feature[DC1394_FEATURE_WHITE_BALANCE-DC1394_FEATURE_MIN];
      if (par.redGain < fi.min) {
        par.redGain = fi.min;
      } else if (par.redGain > fi.max) {
        par.redGain = fi.max;
      }
      if (par.blueGain < fi.min) {
        par.blueGain = fi.min;
      } else if (par.blueGain > fi.max) {
        par.blueGain = fi.max;
      }
 
      // fix mode
      unsigned int states = checkFeatureModes(DC1394_FEATURE_WHITE_BALANCE);
      par.whiteBalanceMode = fixMode(states,par.whiteBalanceMode);
    } else {
      par.blueGain = par.redGain = 0;
      par.whiteBalanceMode = FeatureNA;
    }

    fixFeature(DC1394_FEATURE_HUE,par.hue,par.hueMode);
    fixFeature(DC1394_FEATURE_SATURATION,par.saturation,par.saturationMode);
    fixFeature(DC1394_FEATURE_GAMMA,par.gamma,par.gammaMode);
    fixFeature(DC1394_FEATURE_SHUTTER,par.shutter,par.shutterMode);
    fixFeature(DC1394_FEATURE_GAIN,par.gain,par.gainMode);
    fixFeature(DC1394_FEATURE_IRIS,par.iris,par.irisMode);
    fixFeature(DC1394_FEATURE_FOCUS,par.focus,par.focusMode);
    fixFeature(DC1394_FEATURE_ZOOM,par.zoom,par.zoomMode);
    fixFeature(DC1394_FEATURE_PAN,par.pan,par.panMode);
    fixFeature(DC1394_FEATURE_TILT,par.tilt,par.tiltMode);
    fixFeature(DC1394_FEATURE_OPTICAL_FILTER,par.opticalFilter,par.opticalFilterMode);

    return true;
  }
  
  bool fireWireDCAM::isModeColor(const dc1394video_mode_t mode) const {

    dc1394color_coding_t colorCoding=static_cast<dc1394color_coding_t>(0);
    if (mode >= DC1394_VIDEO_MODE_EXIF) {
      // it is video mode 7: we need the current camera specs to
      // find out if the mode is color or not
      if (notNull(camera_)) {
        dc1394_get_color_coding_from_video_mode(camera_,mode,&colorCoding);
      } else {
        // ERROR CASE: just return 'not a color mode' 
        return false;
      }
    } else {
      // for all standard modes is irrelevant if the camera is active or not
      dc1394_get_color_coding_from_video_mode(camera_,mode,&colorCoding);
    }

    dc1394bool_t isColor;
    dc1394_is_color(colorCoding,&isColor);

    return (isColor != DC1394_FALSE);
  }

  float fireWireDCAM::computeClosestFrameRate(const dc1394video_mode_t mode,
                                              const float fr) const {

    if (!isActive()) {
      return 0.0f;
    }

    const cameraInfo& cam = cams_[activeCamera_];

    // get closest supported framerate for the selected mode
    std::map<dc1394video_mode_t,std::set<float> >::const_iterator frsi;
    frsi = cam.supportedFramerates.find(mode);
    if (frsi == cam.supportedFramerates.end()) {
      // this should never happen because:
      // - mode is always supported (we have just ensure that)
      // - the camera method should fill the valid FR for all supported modes
      return 0.0f;
    }

    const std::set<float>& frates = (*frsi).second;
    std::set<float>::const_iterator fri = frates.begin();
    float bestFr = *fri;
    float dfr = abs(bestFr-fr);
    float tmpdfr;

    ++fri;
    while(fri!=frates.end()) {
      tmpdfr = abs((*fri)-fr);
      if (tmpdfr<dfr) {
        dfr=tmpdfr;
        bestFr = (*fri);
      }
      ++fri;
    }

    return bestFr;
  }

  dc1394video_mode_t  
  fireWireDCAM::computeClosestMode(const dc1394video_mode_t mode) const {
  
    const std::set<dc1394video_mode_t>& sm=cams_[activeCamera_].supportedModes;

    if (sm.empty()) {
      return dc1394video_mode_t(); // as error signal!
    }

    if (mode>=DC1394_VIDEO_MODE_EXIF) {
      std::set<dc1394video_mode_t>::const_iterator it;
      it = sm.find(mode);
      if (it != sm.end()) {
        return mode; // it is supported
      }
      return dc1394video_mode_t();  // not one of the normal modes
    }

    // the approximation is done with the following priorities:
    // 1. keep the mono or color attribute (unless your camera is
    //    monochrome, of course)
    // 2. select the closest resolution.
    
    // separate sets in color and monochrome modes
    std::set<dc1394video_mode_t> inColor,monochrome;
    std::set<dc1394video_mode_t>::const_iterator it,best;

    it = sm.find(mode);
    if (it != sm.end()) {
      return mode; // it is supported
    }
    

    for (it = sm.begin(); it != sm.end(); ++it) {
      if (isModeColor(*it)) {
        inColor.insert(*it);
      } else {
        monochrome.insert(*it);
      }
    }

    eColorMode encoding;
    ipoint resolution;
    convertMode(mode,resolution,encoding);
    const int numPxl = resolution.x*resolution.y;
    const int enc = encoding;
    int d,tmpd,ed,tmped; // distance
    // first try to find for each particular case
    if (!isModeColor(mode) && !monochrome.empty()) {
      // the user wants a monochrome mode, and there are some available
      it = best = monochrome.begin();
      convertMode(*it,resolution,encoding);
      d = abs(numPxl-(resolution.x*resolution.y));
      ed = abs(enc - static_cast<int>(encoding));
      ++it;
      while (it != monochrome.end()) {
        convertMode(*it,resolution,encoding);
        tmpd = abs(numPxl-(resolution.x*resolution.y));
        tmped = abs(enc - static_cast<int>(encoding));
        if ((tmpd<d) || ((tmpd<=d) && (tmped<ed)))  {
          best=it;
          d=tmpd;
          ed=tmped;
        }
        ++it;
      }
      return (*best);
    } // else: we have only color modes!, so get the best it gets!

    it = best = inColor.begin();
    convertMode(*it,resolution,encoding);
    d = abs(numPxl-(resolution.x*resolution.y));
    ed = abs(enc - static_cast<int>(encoding));
    ++it;
    while (it != inColor.end()) {
      convertMode(*it,resolution,encoding);
      tmpd = abs(numPxl-(resolution.x*resolution.y));
      tmped = abs(enc - static_cast<int>(encoding));
      if ((tmpd<d) || ((tmpd<=d) && (tmped<ed)))  {
        best=it;
        d=tmpd;
        ed=tmped;
      }
      ++it;
    }
    return (*best);
  }

  // only updateParameters when camera is initialized
  bool fireWireDCAM::updateParameters() {
    bool b = true;

    // TODO: a way to detect changes in the cameras connected need to be
    //       devised yet
    if (cams_.size() <= 0) {
      cams_.getCameras();
    }

    parameters& par = getRWParameters();

    // delete old bayer functor
    delete bayer_;

    // create new one
    const factory<bayerDemosaicing>& f=factory<bayerDemosaicing>::getFactory();

    bayer_ = f.newInstance(par.bayerMethod); 
    
    if (bayer_==0) {
      // TODO: use DC1394_BAYER_METHOD_XXX if no valid LTI-Lib-2 class is
      // given
      setStatusString(std::string("No class ") + par.bayerMethod + " found.");
      return false;
    }

    // we need to set the right bayerPattern into the new functor
    bayerDemosaicing::parameters* bdpar = bayer_->getParameters().clone();
    bdpar->bayerPattern = par.bayerPattern;
    bayer_->attachParameters(*bdpar); // bayer_ takes care of memory managment.

    if (!isActive()) {
      if (!initCamera(par)) {
        return false;
      }
      activeCameraName_ = par.cameraName;
    } else if (activeCameraName_ != par.cameraName) {
      // it seems that a different camera has to be used
      releaseCamera();
      if (!initCamera(par)) {
        return false;
      }
      activeCameraName_ = par.cameraName;
    }
      
    if (par.fixMode == Upload) {
      dumpParameters(par);
    } else if (par.fixMode == AutoFix) {
      fixParameters(par);
    }

    _lti_debug("setting parameters in camera...\n");

    // brightness
    if (isFeatureAvailable(DC1394_FEATURE_BRIGHTNESS)) {
      b = b && setBrightness(par.brightnessMode,par.brightness);
    }

    // exposure
    if (isFeatureAvailable(DC1394_FEATURE_EXPOSURE)) {
      b = b && setExposure(par.exposureMode,par.exposure);
    }

    // sharpness
    if (isFeatureAvailable(DC1394_FEATURE_SHARPNESS)) {
      b = b && setSharpness(par.sharpnessMode,par.sharpness);
    }

    // white balance
    if (isFeatureAvailable(DC1394_FEATURE_WHITE_BALANCE)) {
      b = b && setWhiteBalance(par.whiteBalanceMode,par.blueGain,par.redGain);
    }

    // hue
    if (isFeatureAvailable(DC1394_FEATURE_HUE)) {
      b = b && setHue(par.hueMode,par.hue);
    }

    // saturation
    if (isFeatureAvailable(DC1394_FEATURE_SATURATION)) {
      b = b && setSaturation(par.saturationMode,par.saturation);
    }

    // gamma
    if (isFeatureAvailable(DC1394_FEATURE_GAMMA)) {
      b = b && setGamma(par.gammaMode,par.gamma);
    }

    // shutter
    if (isFeatureAvailable(DC1394_FEATURE_SHUTTER)) {
      b = b && setShutter(par.shutterMode,par.shutter);
    }

    // gain
    if (isFeatureAvailable(DC1394_FEATURE_GAIN)) {
      b = b && setGain(par.gainMode,par.gain);
    }

    // iris
    if (isFeatureAvailable(DC1394_FEATURE_IRIS)) {
      b = b && setIris(par.irisMode,par.iris);
    }

    // focus
    if (isFeatureAvailable(DC1394_FEATURE_FOCUS)) {
      b = b && setFocus(par.focusMode,par.focus);
    }

    // zoom
    if (isFeatureAvailable(DC1394_FEATURE_ZOOM)) {
      b = b && setZoom(par.zoomMode,par.zoom);
    }

    // pan
    if (isFeatureAvailable(DC1394_FEATURE_PAN)) {
      b = b && setPan(par.panMode,par.pan);
    }

    // tilt
    if (isFeatureAvailable(DC1394_FEATURE_TILT)) {
      b = b && setTilt(par.tiltMode,par.tilt);
    }

    // optical filter
    if (isFeatureAvailable(DC1394_FEATURE_OPTICAL_FILTER)) {
      b = b && setOpticalFilter(par.opticalFilterMode,par.opticalFilter);
    }

    // if user want this so, we will wait
    if (par.waitOnePush) {
      b = b && onePushWait();
    } else {
      onePushLock_.lock();
      waitingFeatures_.clear();
      onePushLock_.unlock();
    }

    dc1394video_mode_t videoMode=dc1394video_mode_t();
    dc1394framerate_t framerate = convertFramerate(par.framerate);

    const cameraInfo& cam = cams_[activeCamera_];

    if (b) {
      // check if the mode in the parameters is supported.
      std::set<dc1394video_mode_t>::const_iterator it;
      const std::set<dc1394video_mode_t>& supportedModes = cam.supportedModes;

      videoMode = convertMode(par.resolution,par.encoding);
      it = supportedModes.find(videoMode);
      if (it == supportedModes.end()) {
        b=false;
        setStatusString("Selected mode not supported by this camera");
        return b;
      } else {
        // determine color mode and format
        ipoint res;
        convertMode(videoMode,res,colorMode_);
      }
    } else {
      return false;
    }
      
    // stop transmission
    isoFlowStopTotal();

    if (captureSetUp_) {
      _lti_debug("release old camera structure...\n");

      // remember: even if this is called release "camera", it just
      // releases the "capture" buffers and related stuff
      // dc1394_capture_stop(cam.info); // this should be called only
      //                                // when finishing with the camera
      captureSetUp_ = false;
    }
     
    passiveWait(Delay); // give some time for the camera to get ready
      
    _lti_debug("setup capture...\n");

    // we MUST set first the format/fps/image/size/position/...
    // adapted from coriander's thread_iso.c

    dc1394switch_t isoState;

    // If ISO is active, don't set anything but capture
    if (dc1394_video_get_transmission(cam.info, &isoState)!=DC1394_SUCCESS) {
      setStatusString("Failed to get ISO state.");
      return false;
    }

    if (isoState!=DC1394_ON) {
      // set format and other stuff
      if (dc1394_video_set_mode(cam.info, videoMode)!=DC1394_SUCCESS){
        setStatusString("Failed to set current video mode.");
        return false;
      }
    
      // set framerate or ROI:
      if (dc1394_is_video_mode_scalable(videoMode)==DC1394_TRUE) {

        uint32_t maxX,maxY;
        if (dc1394_format7_get_max_image_size(camera_,videoMode,&maxX,&maxY)
            !=DC1394_SUCCESS) {
          setStatusString("Failed to get format7 max image size.");
          return false;
        }

        // let us center the roi in order to get the user's desired
        // resolution (well, image size)
        uint32_t offX,offY;
        offX=(maxX-par.resolution.x)/2;
        offY=(maxY-par.resolution.y)/2;
        maxX=par.resolution.x;
        maxY=par.resolution.y;

        if (dc1394_format7_set_roi(cam.info,
                                   videoMode,
                   static_cast<dc1394color_coding_t>(DC1394_QUERY_FROM_CAMERA),
                                   DC1394_QUERY_FROM_CAMERA, 
                                   offX,offY,maxX,maxY)!=DC1394_SUCCESS) {
          setStatusString("Failed to set format7 mode.");
          return false;
        }
      } else {
        dc1394framerate_t tmpFr;
        dc1394_video_get_framerate(cam.info, &tmpFr);
        if (dc1394_video_set_framerate(cam.info, framerate)!=DC1394_SUCCESS){
          setStatusString("Failed to set framerate.");
          return false;
        }
      }
    } // end if iso_state is off.

    // In coriander, the iso transmission is started by a button, here it is
    // started when the user set the parameters
    b = b && isoFlowResume();

    // the libdc1394 will take charge of the deviceFile string deallocation
    // so we need to create a copy of it using malloc (as the lib will use
    // free)
    dc1394error_t err;
    if ((err=dc1394_capture_setup(cam.info,
                               par.dmaBufferSize,
                               DC1394_CAPTURE_FLAGS_DEFAULT))==DC1394_SUCCESS) {
      captureSetUp_ = true;
    } else {
      _lti_debug("Unable to setup capture: " << errorString(err) << "\n");
      setStatusString("Error setting up capture: ");
      appendStatusString(errorString(err));
      b = false;
    } 
      
    bool snapShotMode = (par.snapShotMode && snapShotCapable_);

    // if snapShotMode mode is not set turn on transmission
    if (!snapShotMode) {
      _lti_debug("start iso-transmission...\n");
      if (dc1394_video_set_transmission(cam.info,DC1394_ON)!=DC1394_SUCCESS) {
        setStatusString("unable to start iso transmission\n");
        b = false;
      }
      
      // TODO: what to do with this?
      dc1394switch_t isoPwr;
      if (dc1394_video_get_transmission(cam.info,&isoPwr) != DC1394_SUCCESS) {
        setStatusString("Could not get ISO status");
        return false;
      }

      
      
    }

    return b;
  }

  // -------------------------------------------------------------------
  // The apply-methods!
  // -------------------------------------------------------------------

  // helper function to capture a frame 
  bool fireWireDCAM::captureFrame(dc1394video_frame_t*& frame) {
    frame=0;
    const parameters& par = getParameters();

    // is there a camera handle?
    if (!captureSetUp_) {
      // there should be a status-string from updateParameters()
      return false;
    }

    if (par.snapShotMode && snapShotCapable_) {      
      _lti_debug4("set into one shot mode...\n");
      if (dc1394_video_set_one_shot(camera_,DC1394_ON)!=DC1394_SUCCESS) {
        _lti_debug4("unable to enter one shot mode...\n");
        setStatusString("Unable to enter one shot mode.\n");
        return false;
      }
    }

    _lti_debug4("frames behind: " << camera_->frames_behind << "\n");

    _lti_debug4("dc1394_dma_single_capture(&camera_)...");

    // capture and wait
    if (dc1394_capture_dequeue(camera_,DC1394_CAPTURE_POLICY_WAIT,&frame)
        !=DC1394_SUCCESS) {
      _lti_debug4("failed.\n");
      setStatusString("Unable to capture single frame. Check camera setup.\n");
      return false;
    }

    _lti_debug4(" ok.\n");

    return true;
  }

  
  // On place apply for type image!
  bool fireWireDCAM::apply(image& srcdest) {
    dc1394video_frame_t *frame;

    // capture frame
    if (!(captureFrame(frame))) {
      // statusString is set in captureFrame();
      return false;
    }

    // determine how the conversion should be done
    switch (colorMode_) {
    case (YUV411) : {
      _lti_debug4("convert frame YUV411 -> RGB...\n");
      yuv411toRGB(frame,srcdest);
    } break;
    case (YUV422) : {
      _lti_debug4("convert frame YUV422 -> RGB...\n");
      yuv422toRGB(frame,srcdest);
    } break;
    case (Mono8) : {
      _lti_debug4("convert frame mono8 -> RGB...\n");
      const int rows = frame->size[1];
      const int cols = frame->size[0];
      ubyte* sPtr = reinterpret_cast<ubyte*>(frame->image);
      ch8_.useExternData(rows,cols,sPtr);
      srcdest.castFrom(ch8_);
    } break;
    case (Raw8) :  {
      _lti_debug4("bayer conversion of frame...\n");
      const int rows = frame->size[1];
      const int cols = frame->size[0];
      ubyte* sPtr = reinterpret_cast<ubyte*>(frame->image);
      ch8_.useExternData(rows,cols,sPtr);
      bayer_->apply(ch8_,srcdest);
    } break;
    default: 
      setStatusString("Unsupported encoding type");
      return false;
      break;
    }
    
    _lti_debug4("release buffer...\n");
    dc1394_capture_enqueue(camera_,frame);
    
    return true;
  }

  bool fireWireDCAM::apply(channel8& dest) {
    dc1394video_frame_t *frame;
    // capture frame
    if (!(captureFrame(frame))) {
      // statusString is set in captureFrame();
      return false;
    }

    // determine how the conversion should be done
    switch (colorMode_) {
    case (YUV411) : {
      _lti_debug4("convert frame YUV411 -> channel8...\n");
      yuv411toChannel8(frame,dest);
    } break;
    case (YUV422) : {
      _lti_debug4("convert frame YUV422 -> channel8...\n");
      yuv422toChannel8(frame,dest);
    } break;
    case (Mono8) : {
      _lti_debug4("convert mono8 frame -> channel8...\n");
      const int rows = frame->size[1];
      const int cols = frame->size[0];
      ubyte* sPtr = reinterpret_cast<ubyte*>(frame->image);
      ch8_.useExternData(rows,cols,sPtr);
      dest.copy(ch8_);
    } break;
    case (Raw8) :  {
      _lti_debug4("bayer conversion of frame...\n");
      const int rows = frame->size[1];
      const int cols = frame->size[0];
      ubyte* sPtr = reinterpret_cast<ubyte*>(frame->image);
      ch8_.useExternData(rows,cols,sPtr);
      image tmp;
      bayer_->apply(ch8_,tmp);
      dest.castFrom(tmp);
    } break;
    default: 
      setStatusString("Unsupported encoding type");
      return false;
      break;
    }
    
    _lti_debug4("release buffer...\n");
    dc1394_capture_enqueue(camera_,frame);
    return true;
  }

  bool fireWireDCAM::grabRaw(matrix<ubyte>& dest) {
    // nothing done when not in mono mode
    if ((colorMode_!=Mono8) && (colorMode_!=Raw8)) {
      setStatusString("Error: to grab raw images the camera colormode " \
                      "must be Mono8 or Raw8\n");
      return false;
    }

    // capture frame
    dc1394video_frame_t *frame;
    if (!(captureFrame(frame))) {
      // statusString is set in captureFrame();
      return false;
    } 

    // just resize and copy data
    _lti_debug4("copy frame...\n");
    const int rows = frame->size[1];
    const int cols = frame->size[0];
    dest.allocate(rows,cols);
    ubyte* ptr = reinterpret_cast<ubyte*>(dest.data());
    memcpy(ptr,frame->image,sizeof(ubyte)*rows*cols);


    // never forget to release the buffer ;-)
    _lti_debug4("release buffer...\n");
    dc1394_capture_enqueue(camera_,frame);

    return true;
  }


  // simple conversion from yuv422 to rgb, NO interpolation is done
  void fireWireDCAM::yuv422toRGB(dc1394video_frame_t *frame,image& dest) const {
    const int rows = frame->size[1];
    const int cols = frame->size[0];
    dest.allocate(rows,cols);

    // 4 pixels are stored in 8 byte
    enum {
      elemSize = 8
    };

    // iterator for destination pixels
    image::iterator itd = dest.begin();

    ubyte* sPtr = reinterpret_cast<ubyte*>(frame->image);
    // iterator for capture buffer
    ubyte* iPtr;
    // end of capture buffer
    ubyte* ePtr = sPtr + ((rows*cols)/4)*elemSize;
    
    for (iPtr = sPtr; iPtr != ePtr;) {
      // byte structure is uyvyuyvyuyvy
      const ubyte u1 = *(iPtr++);
      const ubyte y1 = *(iPtr++);
      const ubyte v1 = *(iPtr++);
      const ubyte y2 = *(iPtr++);
      // 1st pixel
      merger_.apply(y1,u1,v1,*(itd++));
      // 2nd pixel
      merger_.apply(y2,u1,v1,*(itd++));      
    }
  }

  // simple conversion from yuv422 to rgb, NO interpolation is done
  void fireWireDCAM::yuv422toChannel8(dc1394video_frame_t *frame,channel8& dest) const {
    const int rows = frame->size[1];
    const int cols = frame->size[0];
    dest.allocate(rows,cols);

    // 4 pixels are stored in 8 byte
    const int elemSize = 8;

    // iterator for destination pixels
    channel8::iterator itd = dest.begin();

    ubyte* sPtr = reinterpret_cast<ubyte*>(frame->image);
    // iterator for capture buffer
    ubyte* iPtr;
    // end of capture buffer
    ubyte* ePtr = sPtr + ((rows*cols)/4)*elemSize;
    
    for (iPtr = sPtr; iPtr != ePtr;) {
      // byte structure is uyvyuyvyuyvy
      iPtr++;
      *(itd++) = *(iPtr++);
      iPtr++;
      *(itd++) = *(iPtr++);
    }
  }

  // simple conversion from yuv411 to rgb, NO interpolation is done
  void fireWireDCAM::yuv411toRGB(dc1394video_frame_t *frame,image& dest) const {
    const int rows = frame->size[1];
    const int cols = frame->size[0];
    dest.allocate(rows,cols);

    // 4 pixels are stored in 6 byte
    const int elemSize = 6;

    // iterator for destination pixels
    image::iterator itd = dest.begin();

    ubyte* sPtr = reinterpret_cast<ubyte*>(frame->image);
    // iterator for capture buffer
    ubyte* iPtr;
    // end of capture buffer
    ubyte* ePtr = sPtr + ((rows*cols)/4)*elemSize;
    
    for (iPtr = sPtr; iPtr != ePtr;) {
      // byte structure is uyyvyyuyyvyy
      const ubyte u1 = *(iPtr++);
      const ubyte y1 = *(iPtr++);
      const ubyte y2 = *(iPtr++);
      const ubyte v1 = *(iPtr++);
      const ubyte y3 = *(iPtr++);
      const ubyte y4 = *(iPtr++);
      // 1st pixel
      merger_.apply(y1,u1,v1,*(itd++));
      // 2nd pixel
      merger_.apply(y2,u1,v1,*(itd++));      
      // 3rd pixel
      merger_.apply(y3,u1,v1,*(itd++));
      // 4th pixel
      merger_.apply(y4,u1,v1,*(itd++));      
    }
  }

  // simple conversion from yuv411 to rgb, NO interpolation is done
  void fireWireDCAM::yuv411toChannel8(dc1394video_frame_t *frame,channel8& dest) const {
    const int rows = frame->size[1];
    const int cols = frame->size[0];
    dest.allocate(rows,cols);

    // 4 pixels are stored in 6 byte
    const int elemSize = 6;

    // iterator for destination pixels
    channel8::iterator itd = dest.begin();

    ubyte* sPtr = reinterpret_cast<ubyte*>(frame->image);
    // iterator for capture buffer
    ubyte* iPtr;
    // end of capture buffer
    ubyte* ePtr = sPtr + ((rows*cols)/4)*elemSize;
    
    for (iPtr = sPtr; iPtr != ePtr;) {
      // byte structure is uyyvyyuyyvyy
      iPtr++;
      *(itd++) = *(iPtr++);
      *(itd++) = *(iPtr++);
      iPtr++;
      *(itd++) = *(iPtr++);
      *(itd++) = *(iPtr++);
    }
  }

  bool fireWireDCAM::dumpParameters(parameters& par) {
    if (!isActive()) {
      return false;
    }

    bool b = true;;

    // the initial set of parameters is surely  almost complete
    if (&par != &getParameters()) {
      par.copy(getParameters());
    }

    par.fixMode = Upload;

    // snapShotCapable_ was initialized by cameraInit()
    par.snapShotMode = (par.snapShotMode && snapShotCapable_);
    
    _lti_debug("reading parameters from camera...\n");

    // videomode
    b = b && getVideoMode(par.resolution,par.encoding);
    _lti_debug(" videomode is: " << par.resolution <<
               " encoding: " << par.encoding << "\n");

    // framerate
    b = b && getFramerate(par.framerate);

    // sometimes the camera does not support the reported framerate, so fixit
    dc1394video_mode_t mode = convertMode(par.resolution,par.encoding);
    par.framerate = computeClosestFrameRate(mode,par.framerate);

    _lti_debug(" framerate is: "<<par.framerate<<"\n");

    // brightness
    if (isFeatureReadable(DC1394_FEATURE_BRIGHTNESS)) {
      b = b && getBrightness(par.brightnessMode,par.brightness);     
    } else {
      par.brightness = 0;
      par.brightnessMode = FeatureNA;
    }
    
    // exposure
    if (isFeatureReadable(DC1394_FEATURE_EXPOSURE)) {
      b = b && getExposure(par.exposureMode,par.exposure);     
    } else {
      par.exposure = 0;
      par.exposureMode = FeatureNA;
    }

    // sharpness
    if (isFeatureReadable(DC1394_FEATURE_SHARPNESS)) {
      b = b && getSharpness(par.sharpnessMode,par.sharpness);     
    } else {
      par.sharpness = 0;
      par.sharpnessMode = FeatureNA;
    }

    // white balance (redGain/blueGain)
    if (isFeatureReadable(DC1394_FEATURE_WHITE_BALANCE)) {
      b = b && getWhiteBalance(par.whiteBalanceMode,par.blueGain,par.redGain);
    } else {
      par.redGain = 0;
      par.blueGain = 0;
      par.whiteBalanceMode = FeatureNA;
    }
    
    // hue
    if (isFeatureReadable(DC1394_FEATURE_HUE)) {
      b = b && getHue(par.hueMode,par.hue);     
    } else {
      par.hue = 0;
      par.hueMode = FeatureNA;
    }

    // saturation
    if (isFeatureReadable(DC1394_FEATURE_SATURATION)) {
      b = b && getSaturation(par.saturationMode,par.saturation);     
    } else {
      par.saturation = 0;
      par.saturationMode = FeatureNA;
    }

    // gamma
    if (isFeatureReadable(DC1394_FEATURE_GAMMA)) {
      b = b && getGamma(par.gammaMode,par.gamma);     
    } else {
      par.gamma = 0;
      par.gammaMode = FeatureNA;
    }

    // shutter
    if (isFeatureReadable(DC1394_FEATURE_SHUTTER)) {
      b = b && getShutter(par.shutterMode,par.shutter);     
    } else {
      par.shutter = 0;
      par.shutterMode = FeatureNA;
    }

    // gain
    if (isFeatureReadable(DC1394_FEATURE_GAIN)) {
      b = b && getGain(par.gainMode,par.gain);     
    } else {
      par.gain = 0;
      par.gainMode = FeatureNA;
    }

    // iris
    if (isFeatureReadable(DC1394_FEATURE_IRIS)) {
      b = b && getIris(par.irisMode,par.iris);     
    } else {
      par.iris = 0;
      par.irisMode = FeatureNA;
    }

    // focus
    if (isFeatureReadable(DC1394_FEATURE_FOCUS)) {
      b = b && getFocus(par.focusMode,par.focus);     
    } else {
      par.focus = 0;
      par.focusMode = FeatureNA;
    }

    // zoom
    if (isFeatureReadable(DC1394_FEATURE_ZOOM)) {
      b = b && getZoom(par.zoomMode,par.zoom);     
    } else {
      par.zoom = 0;
      par.zoomMode = FeatureNA;
    }

    // pan
    if (isFeatureReadable(DC1394_FEATURE_PAN)) {
      b = b && getPan(par.panMode,par.pan);     
    } else {
      par.pan = 0;
      par.panMode = FeatureNA;
    }

    // tilt
    if (isFeatureReadable(DC1394_FEATURE_TILT)) {
      b = b && getTilt(par.tiltMode,par.tilt);     
    } else {
      par.tilt = 0;
      par.tiltMode = FeatureNA;
    }

    // optical filter
    if (isFeatureReadable(DC1394_FEATURE_OPTICAL_FILTER)) {
      b = b && getOpticalFilter(par.opticalFilterMode,par.opticalFilter);     
    } else {
      par.opticalFilter = 0;
      par.opticalFilterMode = FeatureNA;
    }

    return b;
  }
  
  // protected internal helper
  bool fireWireDCAM::isFeatureAvailable(const unsigned int featureId) const {
    assert (notNull(features_));
    assert((featureId>=DC1394_FEATURE_MIN) && (featureId<=DC1394_FEATURE_MAX));

    if (!isActive()) {
      setStatusString("No active camera set yet");
      return false;
    }

    const dc1394feature_info_t& fi =
      features_->feature[featureId-DC1394_FEATURE_MIN];

    // we are assuming this is true.  If some time in the future it isn't
    // something has to shout outloud!!
    assert(fi.id == static_cast<dc1394feature_t>(featureId));

    return (fi.available==DC1394_TRUE);
  }
 

  // protected internal helper
  bool fireWireDCAM::isFeatureReadable(const unsigned int featureId) const {
    assert (notNull(features_));
    assert((featureId>=DC1394_FEATURE_MIN) && (featureId<=DC1394_FEATURE_MAX));

    const dc1394feature_info_t& fi =
      features_->feature[featureId-DC1394_FEATURE_MIN];

    // we are assuming this is true.  If some time in the future it isn't
    // something has to shout outloud!!
    assert(fi.id == static_cast<dc1394feature_t>(featureId));

    return ( (fi.available==DC1394_TRUE) && 
             (fi.readout_capable==DC1394_TRUE) );
  }
 
  unsigned int 
  fireWireDCAM::checkFeatureModes(const unsigned int featureId) const {
    assert (notNull(features_));
    assert((featureId>=DC1394_FEATURE_MIN) && (featureId<=DC1394_FEATURE_MAX));

    const dc1394feature_info_t& fts = 
      features_->feature[featureId-DC1394_FEATURE_MIN];

    unsigned int result = 0;
    if (fts.available==DC1394_TRUE) {
      if (fts.on_off_capable == DC1394_TRUE) {
        result |= FeatureOff;
      }
      if (fts.absolute_capable == DC1394_TRUE) {
        result |= FeatureAbsolute;
      }

      for (unsigned int i=0;i<fts.modes.num;++i) {
        switch(fts.modes.modes[i]) {
        case DC1394_FEATURE_MODE_MANUAL:
          result |= FeatureManual;
          break;
        case DC1394_FEATURE_MODE_AUTO:
          result |= FeatureAuto;
          break;
        case DC1394_FEATURE_MODE_ONE_PUSH_AUTO:
          result |= FeatureOnePush;
          break;
        default:
          // no change at all... unknown mode
          break;
        }
      }
    }
    return result;
  }
  
  bool fireWireDCAM::checkFeatureValueRange(const unsigned int featureId,
                                            float& minValue,
                                            float& maxValue) const {
    assert (notNull(features_));
    assert((featureId>=DC1394_FEATURE_MIN) && (featureId<=DC1394_FEATURE_MAX));
    
    const dc1394feature_info_t& fts = 
      features_->feature[featureId-DC1394_FEATURE_MIN];

    minValue = static_cast<float>(fts.min);
    maxValue = static_cast<float>(fts.max);
    
    return (fts.available==DC1394_TRUE);
  }

  // Get the camera feature state
  bool fireWireDCAM::getFeatureMode(const unsigned int featureId,
                                    eCamFeatureMode& state) const {
    assert (notNull(features_));
    assert((featureId>=DC1394_FEATURE_MIN) && (featureId<=DC1394_FEATURE_MAX));

    if (isNull(camera_)) {
      setStatusString("No valid camera set yet");
      return false;   
    }
    
    const dc1394feature_info_t& fts =
      features_->feature[featureId-DC1394_FEATURE_MIN];

    if (fts.available == DC1394_FALSE) {
      state = FeatureNA;
      return true;
    }

    if ( (fts.on_off_capable == DC1394_TRUE) &&
         (fts.is_on == DC1394_OFF) ) {
      state = FeatureOff;
      return true;  // feature available and turned off! everything is ok
    }

    // if we got here, wether the feature is always on (not on_off_capable),
    // or it can be turned off, but it is anyway on!
    switch (fts.current_mode) {
    case DC1394_FEATURE_MODE_MANUAL:
      state=FeatureManual;
      break;
    case DC1394_FEATURE_MODE_AUTO:
      state=FeatureAuto;
      break;
    case DC1394_FEATURE_MODE_ONE_PUSH_AUTO:
      state=FeatureOnePush;
      break;
    default:
      break;
    }

    dc1394switch_t pwr;
    if (dc1394_feature_get_absolute_control(camera_,
                                            fts.id,
                                            &pwr) == DC1394_SUCCESS) {
      if (pwr==DC1394_ON) {
        state=FeatureAbsolute;
      }
      return true;
    }

    state = FeatureNA;
    // if we get until here, then the camera is maybe not fully IIDC compliant
    return false;        // report an error anyway
  }


  // protected internal helper
  bool fireWireDCAM::isFeatureAuto(const unsigned int featureId) const {
    assert (notNull(features_));
    assert((featureId>=DC1394_FEATURE_MIN) && (featureId<=DC1394_FEATURE_MAX));

    dc1394feature_mode_t mode;
    if (dc1394_feature_get_mode(camera_,
                                static_cast<dc1394feature_t>(featureId),
                                &mode) != DC1394_SUCCESS) {
      // return false if an error occurs
      _lti_debug("unable to ask if feature " <<
                 dc1394_feature_desc[featureId-DC1394_FEATURE_MIN]<<" is auto\n");
      
      return false;
    }
    return (mode==DC1394_FEATURE_MODE_AUTO);
  }

  // protected internal helper
  bool fireWireDCAM::setFeatureAuto(const unsigned int featureId, 
                                    const bool on) {
    assert (notNull(features_));
    assert((featureId>=DC1394_FEATURE_MIN) && (featureId<=DC1394_FEATURE_MAX));

    dc1394feature_modes_t modes;
    if (dc1394_feature_get_modes(camera_,
                                 dc1394feature_t(featureId),
                                 &modes) != DC1394_SUCCESS) {
      setStatusString("Could not get available modes for feature.");
      return false;
    }

    uint32_t i;
    for (i=0;i<modes.num;++i) {
      if (modes.modes[i] == DC1394_FEATURE_MODE_AUTO) {
        break;
      }
    }

    bool autoCapable = (i<modes.num);

    if (autoCapable) {
      if (dc1394_feature_set_mode(camera_,
                                  dc1394feature_t(featureId),
                                  DC1394_FEATURE_MODE_AUTO) != DC1394_SUCCESS) {
        std::string str = "Auto setting for feature ";
        str += dc1394_feature_get_string(dc1394feature_t(featureId));
        str += " could not be turned ";
        str += (on?"on":"off");
        setStatusString(str.c_str());
        
        return false;
      } else {

        // Update the features info
        features_->feature[featureId-DC1394_FEATURE_MIN].current_mode
          = DC1394_FEATURE_MODE_AUTO;
      }
    } else if (on) {
      // there is an error only if the user wants to turn the auto mode on
      // the system may accept manual, but no auto
      std::string str = "Feature ";
      str += dc1394_feature_get_string(dc1394feature_t(featureId));
      str += " does not support the auto state.";
      setStatusString(str.c_str());      
      return false;          
    }
    
    return true;
  }


  bool fireWireDCAM::setFeatureOn(const unsigned int featureId,const bool on) {
    assert (notNull(features_));
    assert((featureId>=DC1394_FEATURE_MIN) && (featureId<=DC1394_FEATURE_MAX));

    // we have to update some attributes here (following corianders code...)
    // so, we also need to get a read-writable reference to the feature's
    // data structure.
    dc1394feature_info_t& fts =
      features_->feature[featureId-DC1394_FEATURE_MIN];

    const dc1394switch_t val = on ? DC1394_ON : DC1394_OFF;

    if ( fts.on_off_capable == DC1394_TRUE) {
      if (dc1394_feature_set_power(camera_,
                                   dc1394feature_t(featureId),
                                   val) != DC1394_SUCCESS) {
        std::string str = "Feature ";
        str += dc1394_feature_get_string(dc1394feature_t(featureId));
        str += " could not be turned ";
        str += (on?"on":"off");
        setStatusString(str.c_str());
        
        return false;
      } else {
        features_->feature[featureId-DC1394_FEATURE_MIN].is_on =
          on ? DC1394_ON : DC1394_OFF; // update the state
      }
    } else {
      std::string str = "Feature ";
      str += dc1394_feature_get_string(dc1394feature_t(featureId));
      str += " does not support the on/off capability";
      setStatusString(str.c_str());
      
      return false;          
    }
    
    return true;
  }

  bool fireWireDCAM::setFeatureAbsoluteMode(const unsigned int featureId,
                                             const bool on) {
    assert (notNull(features_));
    assert((featureId>=DC1394_FEATURE_MIN) && (featureId<=DC1394_FEATURE_MAX));
    
    // we have to update some attributes here (following corianders code...)
    // so, we also need to get a read-writable reference to the feature's
    // data structure.
    dc1394feature_info_t& fts =
      features_->feature[featureId-DC1394_FEATURE_MIN];

    const dc1394switch_t val = on ? DC1394_ON : DC1394_OFF;

    if ( fts.absolute_capable == DC1394_TRUE) {
      if (dc1394_feature_set_absolute_control(camera_,
                                              dc1394feature_t(featureId),
                                              val) != DC1394_SUCCESS) {
        std::string str = "Absolute setting for feature ";
        str += dc1394_feature_get_string(dc1394feature_t(featureId));
        str += " could not be ";
        str += (on?"activated":"deactivated");
        setStatusString(str.c_str());
        
        return false;
      } else {
        features_->feature[featureId-DC1394_FEATURE_MIN].abs_control =
          on ? DC1394_ON : DC1394_OFF; // update the state
      }
    } else {
      std::string str = "Feature ";
      str += dc1394_feature_get_string(dc1394feature_t(featureId));
      str += " does not support the absolute setting.";
      setStatusString(str.c_str());
      
      return false;          
    }
    
    return true;
  }

  // Set feature state
  bool fireWireDCAM::setFeatureMode(const unsigned int featureId,
                                    const eCamFeatureMode state) {
    assert (notNull(features_));
    assert((featureId>=DC1394_FEATURE_MIN) && (featureId<=DC1394_FEATURE_MAX));
    
    // read only reference to 
    const dc1394feature_info_t& fts =
      features_->feature[featureId-DC1394_FEATURE_MIN];

    switch(state) {
    case FeatureNA:
      return true;
      break;
    case FeatureOff:
      return setFeatureOn(featureId,false);
      break;
    case FeatureAuto:
      // for auto we have to turn the feature on, if necessary:
      if (fts.on_off_capable == DC1394_TRUE) {
        if (!setFeatureOn(featureId)) { // turn it on
          return false; // if error occurred
        }
      }
      
      // for absolute setting
      if (fts.absolute_capable == DC1394_TRUE) {
        // for manual setting we assume the absolute mode must be off
        // otherwise the user should select FeatureAbsolute
        if (!setFeatureAbsoluteMode(featureId,false)) {
          return false;
        }
      }
        
      // now to the auto stuff
      if (!setFeatureAuto(featureId)) {
        return false;
      }
      
      return true;  // feature available and turned off! everything is ok
      break;
      
    case FeatureManual: {
      
      uint32_t f;
      for (f=0;f<fts.modes.num;++f) {
        if (fts.modes.modes[f] == DC1394_FEATURE_MODE_MANUAL) {
          break;
        }
      }

      if ( f < fts.modes.num ) { // mode manual is supported
        
        // for manual we have to turn the feature on, if necessary:
        if (fts.on_off_capable == DC1394_TRUE) {
          if (!setFeatureOn(featureId)) { // turn it on
            return false; // if error occurred
          }
        }
        
        // for absolute setting
        if (fts.absolute_capable == DC1394_TRUE) {
          if (!setFeatureAbsoluteMode(featureId,false)) {
            return false;
          }
        }
        
        // turn off the auto state
        if (!setFeatureAuto(featureId,false)) {
          return false;
        }
        
      } else {
        std::string str = "Feature ";
        str += dc1394_feature_get_string(dc1394feature_t(featureId));
        str += " does not support manual settings.";
        setStatusString(str.c_str());
        
        return false;          
      }
      return true;  // feature available and turned off! everything is ok
    } break;
    case FeatureOnePush: {
      // for one-push we have to turn the feature on, if necessary:
      if (fts.on_off_capable == DC1394_TRUE) {
        if (!setFeatureOn(featureId)) { // turn it on
          return false; // if error occurred
        }
      }
      
      // for absolute setting
      if (fts.absolute_capable == DC1394_TRUE) {
        if (!setFeatureAbsoluteMode(featureId,false)) {
          return false;
        }
      }
        
      return onePushQueueInsert(featureId);
    } break;
    case FeatureAbsolute:
      if ( fts.absolute_capable == DC1394_TRUE) {
          
        // for absolute state we have to turn the feature on, if necessary:
        if (fts.on_off_capable == DC1394_TRUE) {
          if (!setFeatureOn(featureId)) { // turn it on
            return false; // if error occurred
          }
        }
          
        // turn off the auto state
        if (!setFeatureAuto(featureId,false)) {
          return false;
        }
          
        // for absolute setting
        if (fts.absolute_capable == DC1394_TRUE) {
          if (!setFeatureAbsoluteMode(featureId,true)) {
            return false;
          }
        }
          
        return true;
      }
      break;
    default:
      return false;
    }
    
    return false;
  }

  const std::string fireWireDCAM::cameraName() const {
    std::string cname = activeCameraName_;

    if (!isActive()) {
      return std::string("");
    }
    
    cname += std::string(": ") + camera_->vendor + " " + camera_->model;

    return cname;
  }

  // print actual feature set for debug purpose
  bool fireWireDCAM::printFeatures() const {
    if (!isActive()) {
      return false;
    }
    dc1394_feature_print_all(features_,0); // 0 means stdout

    return true;
  }

  bool fireWireDCAM::initCamera(parameters& par) {

    // well, theoretically 'par' contains which camera we are looking for:
    if (!cameraNames_.use(par.cameraNamesFile)) {
      std::string msg = "Could not open or understand file " +
        par.cameraNamesFile;
      setStatusString(msg.c_str());
      return false;
    }
    uint64_t guid=0;

    // Flag to indicate if the cameraName is not being used and has to be
    // added to the database as soon as the first active camera has being
    // detected.
    bool needToAdd=false;
    needToAdd = !cameraNames_.query(par.cameraName,guid);

    if (needToAdd) {
      if (!cams_.findFreeCamera(activeCamera_)) {
        camera_=0;
        activeCamera_=-1;
        features_=0;
        setStatusString("Camera already in use!");
        return false;
      } 

      camera_=cams_[activeCamera_].info;
      features_=&cams_[activeCamera_].featureSet;

      if (!cameraNames_.add(par.cameraName,camera_->guid)) {
        // this should not happen ever
        setStatusString("Could not register camera name");
        return false;
      }

      cameraNames_.dump();
    } else {
      if (!cams_.findCamera(guid,activeCamera_)) {
        // The camera given in the parameters is not working, but maybe the
        // user just wants to use any available camera.  We assume that an
        // AutoFix or an Upload mode implies this assumption.

        if ( (par.fixMode == AutoFix) || (par.fixMode == Upload) ) {

          if (!cams_.findFreeCamera(activeCamera_)) {
            camera_=0;
            features_=0;
            std::string msg = "Camera " + par.cameraName + 
              " not connected or already in use.";
            setStatusString(msg);
            return false;
          }

          // Two things can happen:
          // 1.  An active camera is for the very first time connected, so that
          //     is not even within the camera names file, or
          // 2.  The camera is already in that file, but the wrong name of the 
          //     parameters have been chosen.

          // First than all, we need to check which case we have!
          if (!cameraNames_.query(camera_->guid,par.cameraName)) {
            
            // this is the first case
            
            // We need a new name for this camera. Build it from the
            // vendor, model and just to make sure that no two cameras get
            // the same name, with the index in the array
            
            int postfix = 0;
            std::string newName;
            uint64_t dummyId;
            camera_=cams_[activeCamera_].info;
            features_=&cams_[activeCamera_].featureSet;
            do {
              std::ostringstream oss;
              oss << camera_->vendor << " " 
                  << camera_->model;
              if (postfix > 0) {
                oss << " (" << postfix << ")";
              }
              
              newName = oss.str();
              postfix++;
            } while (cameraNames_.query(newName,dummyId)); // while name exists

            if (!cameraNames_.add(newName,camera_->guid)) {
              // this should not ever happen
              setStatusString("Could not register camera name");
              return false;
            }
        
            cameraNames_.dump();
            par.cameraName = newName;
          }

          // the first case has already been considered in the query
          
        } else {
          std::string msg = "Camera " + par.cameraName + " is not present.";
          setStatusString(msg.c_str());
          return false;
        }
      }      
    }
    
    // we found the camera, but let's check if we can use it
    if (!cams_.reserveIndex(activeCamera_)) {
      // ups, somebody was using it already!
      std::string msg = "Camera " + par.cameraName + " already in use.";
      setStatusString(msg.c_str());

      activeCamera_=-1;
      camera_=0;
      features_=0;

      return false;
    }    

    cameraInfo& cam = cams_[activeCamera_];
    camera_ = cam.info;
        
    // initialize shadows/shortcuts
    features_ = &cam.featureSet;

#ifdef _LTI_DEBUG
    // get camerainfo for debugging
    _lti_debug4(" Using " << camera_->model << 
                " (" << camera_->vendor << ").\n");
#endif

    snapShotCapable_ = (camera_->one_shot_capable != DC1394_FALSE);

    // first turn camera on...
    if (camera_->can_switch_on_off != DC1394_FALSE) {
        // check if power on/off is available
      if (dc1394_camera_set_power(camera_,DC1394_ON) != DC1394_SUCCESS) {
        setStatusString("Could not turn camera on");
        return false;      
      }
    }

    if (dc1394_video_get_iso_speed(camera_,&isoSpeed_) != DC1394_SUCCESS) {
      // something went really wrong here!
      _lti_debug("Error: could not get IEEE1394 bus channel and speed");
      setStatusString("Could not get IEEE1394 bus channel and speed");
      return false;
    }

    return isActive();
  }

  // -----------------------------------------------------------------------
  // Methods for direct access to camera
  // -----------------------------------------------------------------------


  inline bool fireWireDCAM::setFeatureValue(const unsigned int featureId,
                                            const eCamFeatureMode state,
                                            const float value) {
    if (isNull(camera_)) {
      setStatusString("No valid camera set yet");
      return false;
    }

    if (state == FeatureNA) {
      return true;
    }

    if (!isFeatureAvailable(featureId)) {
      std::string str = "Feature ";
      str += dc1394_feature_get_string(dc1394feature_t(featureId));
      str += " not supported.";
      setStatusString(str.c_str());
      return false;
    }

    if (setFeatureMode(featureId,state)) {
      if (state == FeatureManual) {
        if (dc1394_feature_set_value(camera_,
                                     dc1394feature_t(featureId),
                                     static_cast<uint32_t>(value+0.5f))
            != DC1394_SUCCESS) {
          std::string str = "Could not set ";
          str += dc1394_feature_get_string(dc1394feature_t(featureId));
          str += " value";
          setStatusString(str.c_str());
          return false;
        }
      } else if (state == FeatureAbsolute) {
        if (dc1394_feature_set_absolute_value(camera_,
                                              dc1394feature_t(featureId),
                                              value) != DC1394_SUCCESS) {
          std::string str = "Could not set ";
          str += dc1394_feature_get_string(dc1394feature_t(featureId));
          str += " absolute value";
          setStatusString(str.c_str());
          return false;
        }
      }

      return true;
    }
        
    return false;
  }

  inline bool fireWireDCAM::getFeatureValue(const unsigned int featureId,
                                            eCamFeatureMode& state,
                                            float& value) const {

    if (!getFeatureMode(featureId,state)) {
      return false;
    }
    
    const dc1394feature_info_t& fts =
      features_->feature[featureId-DC1394_FEATURE_MIN];
    
    if (fts.readout_capable == DC1394_FALSE) {
      std::string str = "Feature";
      str += dc1394_feature_get_string(dc1394feature_t(featureId));
      str += " is not read-out capable.";
      setStatusString(str.c_str());
      return false;
    }

    if (state != FeatureAbsolute) {
      uint32_t tmp;
      if (dc1394_feature_get_value(camera_,
                                   static_cast<dc1394feature_t>(featureId),
                                   &tmp) != DC1394_SUCCESS) {
        std::string str = "Value for feature ";
        str += dc1394_feature_get_string(dc1394feature_t(featureId));
        str += " could not be read.";
        setStatusString(str.c_str());
        return false;
      }
      value=static_cast<float>(tmp);
    } else {
      if (dc1394_feature_get_absolute_value(camera_,
                                       static_cast<dc1394feature_t>(featureId),
                                       &value) != DC1394_SUCCESS) {
        std::string str = "Absolute value for feature ";
        str += dc1394_feature_get_string(dc1394feature_t(featureId));
        str += " could not be read.";
        setStatusString(str.c_str());
        return false;
      }
    }
    
    return true;
  }

  bool fireWireDCAM::setBrightness(const eCamFeatureMode state,
                                   const float brightness) {

    if (setFeatureValue(DC1394_FEATURE_BRIGHTNESS,state,brightness)) {
      parameters& par = getRWParameters();
      par.brightnessMode = state;
      par.brightness      = brightness;
      return true;
    }
        
    return false;
  }
  
  bool fireWireDCAM::getBrightness(eCamFeatureMode& state,
                                   float& brightness) const {
    return getFeatureValue(DC1394_FEATURE_BRIGHTNESS,state,brightness);
  }
  
  bool fireWireDCAM::setExposure(const eCamFeatureMode state,
                                 const float exposure) {
    if (setFeatureValue(DC1394_FEATURE_EXPOSURE,state,exposure)) {
      parameters& par = getRWParameters();
      par.exposureMode = state;
      par.exposure      = exposure;
      return true;
    }
        
    return false;
  }
  
  bool fireWireDCAM::getExposure(eCamFeatureMode& state,
                                 float& exposure) const {
    return getFeatureValue(DC1394_FEATURE_EXPOSURE,state,exposure);
  }
  
  bool fireWireDCAM::setSharpness(const eCamFeatureMode state,
                                  const float sharpness) {
    if (setFeatureValue(DC1394_FEATURE_SHARPNESS,state,sharpness)) {
      parameters& par = getRWParameters();
      par.sharpnessMode = state;
      par.sharpness      = sharpness;
      return true;
    }
        
    return false;
  }
  
  bool fireWireDCAM::getSharpness(eCamFeatureMode& state,
                                  float& sharpness) const {
    return getFeatureValue(DC1394_FEATURE_SHARPNESS,state,sharpness);
  }

  bool fireWireDCAM::setWhiteBalance(const eCamFeatureMode state,
                                     const float ubGain,
                                     const float rvGain) {
    
    if (!isFeatureAvailable(DC1394_FEATURE_WHITE_BALANCE)) {
      setStatusString("Feature white balance not supported");
      return false;
    }

    if (setFeatureMode(DC1394_FEATURE_WHITE_BALANCE,state)) {
      if ((state == FeatureManual) || (state == FeatureAbsolute)) {
        if (dc1394_feature_whitebalance_set_value(camera_,
                                                  static_cast<uint32_t>(ubGain),
                                                  static_cast<uint32_t>(rvGain))
            != DC1394_SUCCESS) {
          setStatusString("Could not set white balance B/U and R/V gains");
          return false;
        }
      } 

      parameters& par = getRWParameters();
      par.whiteBalanceMode = state;
      par.blueGain         = ubGain;
      par.redGain          = rvGain;
      
      return true;
    }
        
    // BUG? How to set the values in absolute mode?

    return false;

  }
  
  bool fireWireDCAM::getWhiteBalance(eCamFeatureMode& state,
                                     float& ubGain,
                                     float& vrGain) const {
    if (!getFeatureMode(DC1394_FEATURE_WHITE_BALANCE,state)) {
      return false;
    }
    
    const dc1394feature_info_t& fts =
      features_->feature[DC1394_FEATURE_WHITE_BALANCE-DC1394_FEATURE_MIN];
    
    if (fts.readout_capable == DC1394_FALSE) {
      setStatusString("Feature white balance is not read-out capable.");
      return false;
    }

    uint32_t bg,rg;
    if (dc1394_feature_whitebalance_get_value(camera_,&bg,&rg)
        != DC1394_SUCCESS) {
      setStatusString("Value for feature white balance could not be read.");
      return false;
    }
    ubGain=static_cast<float>(bg);
    vrGain=static_cast<float>(rg);
    
    return true;
  }
  
  bool fireWireDCAM::setHue(const eCamFeatureMode state,
                            const float hue) {
    if (setFeatureValue(DC1394_FEATURE_HUE,state,hue)) {
      parameters& par = getRWParameters();
      par.hueMode = state;
      par.hue      = hue;
      return true;
    }
        
    return false;
  }
  
  bool fireWireDCAM::getHue(eCamFeatureMode& state,
                            float& hue) const {
    return getFeatureValue(DC1394_FEATURE_HUE,state,hue);
  }
  
  bool fireWireDCAM::setSaturation(const eCamFeatureMode state,
                                   const float saturation) {
    if (setFeatureValue(DC1394_FEATURE_SATURATION,state,saturation)) {
      parameters& par = getRWParameters();
      par.saturationMode = state;
      par.saturation      = saturation;
      return true;
    }
        
    return false;
  }
  
  bool fireWireDCAM::getSaturation(eCamFeatureMode& state,
                                   float& saturation) const {
    
    return getFeatureValue(DC1394_FEATURE_SATURATION,state,saturation);
  }
  
  bool fireWireDCAM::setGamma(const eCamFeatureMode state,
                              const float gamma) {
    if (setFeatureValue(DC1394_FEATURE_GAMMA,state,gamma)) {
      parameters& par = getRWParameters();
      par.gammaMode = state;
      par.gamma      = gamma;
      return true;
    }
        
    return false;
  }

  bool fireWireDCAM::getGamma(eCamFeatureMode& state,
                              float& gamma) const {
    return getFeatureValue(DC1394_FEATURE_GAMMA,state,gamma);
  }

  bool fireWireDCAM::setShutter(const eCamFeatureMode state,
                                const float shutter) {
    if (setFeatureValue(DC1394_FEATURE_SHUTTER,state,shutter)) {
      parameters& par = getRWParameters();
      par.shutterMode = state;
      par.shutter      = shutter;
      return true;
    }
        
    return false;
  }

  bool fireWireDCAM::getShutter(eCamFeatureMode& state,
                                float& shutter) const {
    return getFeatureValue(DC1394_FEATURE_SHUTTER,state,shutter);
  }

  bool fireWireDCAM::setGain(const eCamFeatureMode state,
                             const float gain) {
    if (setFeatureValue(DC1394_FEATURE_GAIN,state,gain)) {
      parameters& par = getRWParameters();
      par.gainMode = state;
      par.gain      = gain;
      return true;
    }
        
    return false;
  }

  bool fireWireDCAM::getGain(eCamFeatureMode& state,
                             float& gain) const {
    return getFeatureValue(DC1394_FEATURE_GAIN,state,gain);
  }

  bool fireWireDCAM::setIris(const eCamFeatureMode state,
                             const float iris) {
     if (setFeatureValue(DC1394_FEATURE_IRIS,state,iris)) {
      parameters& par = getRWParameters();
      par.irisMode = state;
      par.iris      = iris;
      return true;
    }
        
    return false;
 }

  bool fireWireDCAM::getIris(eCamFeatureMode& state,
                             float& iris) const {
    return getFeatureValue(DC1394_FEATURE_IRIS,state,iris);
  }

  bool fireWireDCAM::setFocus(const eCamFeatureMode state,
                              const float focus) {
    if (setFeatureValue(DC1394_FEATURE_FOCUS,state,focus)) {
      parameters& par = getRWParameters();
      par.focusMode = state;
      par.focus      = focus;
      return true;
    }
        
    return false;
  }

  bool fireWireDCAM::getFocus(eCamFeatureMode& state,
                              float& focus) const {
    return getFeatureValue(DC1394_FEATURE_FOCUS,state,focus);
  }

  bool fireWireDCAM::setZoom(const eCamFeatureMode state,
                             const float zoom) {
    if (setFeatureValue(DC1394_FEATURE_ZOOM,state,zoom)) {
      parameters& par = getRWParameters();
      par.zoomMode = state;
      par.zoom      = zoom;
      return true;
    }
        
    return false;
  }

  bool fireWireDCAM::getZoom(eCamFeatureMode& state,
                             float& zoom) const {
    return getFeatureValue(DC1394_FEATURE_ZOOM,state,zoom);
  }

  bool fireWireDCAM::setPan(const eCamFeatureMode state,
                            const float pan) {
    if (setFeatureValue(DC1394_FEATURE_PAN,state,pan)) {
      parameters& par = getRWParameters();
      par.panMode = state;
      par.pan      = pan;
      return true;
    }
        
    return false;
  }

  bool fireWireDCAM::getPan(eCamFeatureMode& state,
                            float& pan) const {
    return getFeatureValue(DC1394_FEATURE_PAN,state,pan);
  }

  bool fireWireDCAM::setTilt(const eCamFeatureMode state,
                             const float tilt) {
     if (setFeatureValue(DC1394_FEATURE_TILT,state,tilt)) {
      parameters& par = getRWParameters();
      par.tiltMode = state;
      par.tilt     = tilt;
      return true;
    }
        
    return false;
  }

  bool fireWireDCAM::getTilt(eCamFeatureMode& state,
                             float& tilt) const {
    return getFeatureValue(DC1394_FEATURE_TILT,state,tilt);
  }
  
  bool fireWireDCAM::setPanTilt(const eCamFeatureMode mode,
                                const float pan,
                                const float tilt) {
    if (setFeatureValue(DC1394_FEATURE_PAN,mode,pan) &&
        setFeatureValue(DC1394_FEATURE_TILT,mode,tilt)) {
      parameters& par = getRWParameters();
      par.panMode = mode;
      par.pan      = pan;
      par.tiltMode = mode;
      par.tilt     = tilt;
      return true;
    } 

    return false;
  }

  bool fireWireDCAM::getPanTilt(eCamFeatureMode& panMode,
                                float& pan,
                                eCamFeatureMode& tiltMode,
                                float& tilt) const {
    return (getFeatureValue(DC1394_FEATURE_PAN,panMode,pan) && 
            getFeatureValue(DC1394_FEATURE_TILT,tiltMode,tilt));
            
  }
  
  bool fireWireDCAM::panTiltReset() {
    return true; // until now, we don't know how to reset the pan-tilt
                 // unit of a FireWire cam!
  }

  bool fireWireDCAM::setOpticalFilter(const eCamFeatureMode state,
                                      const float opticalFilter) {
    if (setFeatureValue(DC1394_FEATURE_OPTICAL_FILTER,state,opticalFilter)) {
      parameters& par = getRWParameters();
      par.opticalFilterMode = state;
      par.opticalFilter     = opticalFilter;
      return true;
    }
        
    return false;
  }

  bool fireWireDCAM::getOpticalFilter(eCamFeatureMode& state,
                                      float& opticalFilter) const {
    return getFeatureValue(DC1394_FEATURE_OPTICAL_FILTER,state,opticalFilter);
  }
  
  // -----------------------------------------------------------------------
  // End of methods for direct access to camera
  // -----------------------------------------------------------------------

  bool fireWireDCAM::getFramerate(float& rate) const {
    dc1394framerate_t irate;
    bool b = true;

    if (isNull(camera_)) {
      setStatusString("No valid camera set yet");
      return false;
    }

    b = b && (dc1394_video_get_framerate(camera_,&irate) == DC1394_SUCCESS);
    if (b) {
      // same enumeration values
      rate = convertFramerate(irate);
    } else {
      rate=0.0f;
    }

    return b;
  }


  /*
   * Set frame rate directly to of camera.
   * @return true if operation was successfull, false otherwise
   */
  bool fireWireDCAM::setFramerateAndResolution(const float framerate,
                                               const ipoint& resolution) {

    if (isNull(camera_)) {
      setStatusString("No valid camera set yet");
      return false;
    }

    parameters& par = getRWParameters();
    cameraInfo& cam = cams_[activeCamera_];
    dc1394video_mode_t mode = dc1394video_mode_t();

    // Check on the permitted resolution/framework since the camera may allow a
    // given pair but only for black/white images before setting anything, a
    // verification has to be done (see coriander)

    // check if the mode in the parameters is supported.
    std::set<dc1394video_mode_t>::const_iterator it;
    const std::set<dc1394video_mode_t>& supportedModes = cam.supportedModes;
    
    mode = convertMode(resolution,par.encoding);
    it = supportedModes.find(mode);
    if (it == supportedModes.end()) {
      setStatusString("Selected resolution and encoding not supported "\
                      "by this camera");
      return false;
    } 
      
    // stop transmission
    isoFlowStop();

    // we have to get the closest supported framerate
    const std::set<float>& supportedFramerates = cam.supportedFramerates[mode];

    float diff=std::numeric_limits<float>::max();
    float best=0.0f;

    std::set<float>::const_iterator fit;
    const std::set<float>::const_iterator eit=supportedFramerates.end();
    for (fit=supportedFramerates.begin();fit!=eit;++fit) {
      float d = std::abs(framerate-*fit);
      if (d < diff) {
        diff=d;
        best=*fit;
      }
    }

    dc1394framerate_t bestFramerate = convertFramerate(best);

    if (dc1394_video_set_mode(camera_,mode) != DC1394_SUCCESS) {
      setStatusString("Could not set video mode");
      return false;
    } else {
      cam.videoMode = mode;
    }
    
    if (dc1394_video_set_framerate(camera_,bestFramerate)!=DC1394_SUCCESS) {
      setStatusString("Cannot set video framerate");
      return false;
    } else {
      cam.framerate=bestFramerate;
      par.framerate=convertFramerate(bestFramerate);
    }
            
    if (dc1394_feature_get_all(camera_,&cam.featureSet) != DC1394_SUCCESS) {
      setStatusString("Could not get camera feature information!");
      return false;
    }

    // the libdc1394 will take charge of the deviceFile string deallocation
    // so we need to create a copy of it using malloc (as the lib will use
    // free)
    if (captureSetUp_) {
      _lti_debug4("release old camera structure...\n");

      // remember: even if this is called release "camera", it just
      // releases the "capture" buffers and related stuff
      dc1394_capture_stop(camera_);
      captureSetUp_ = false;
    }

    // Possible flags
    // DC1394_CAPTURE_FLAGS_CHANNEL_ALLOC
    // DC1394_CAPTURE_FLAGS_BANDWIDTH_ALLOC
    // DC1394_CAPTURE_FLAGS_DEFAULT  /* do bandwidth and channel allocation */
    // DC1394_CAPTURE_FLAGS_AUTO_ISO /* automatically start iso before capture
    //                                  and stop it after */

    if (dc1394_capture_setup(camera_,
                             par.dmaBufferSize,
                             DC1394_CAPTURE_FLAGS_DEFAULT)==DC1394_SUCCESS) {
      captureSetUp_ = true;
    } else {
      _lti_debug("unable to setup capture\n");
      return false;
    } 

    // we have to resume now, before we change the miscInfo data structure
    isoFlowResume();
    
    par.resolution=resolution;

    return true;
  }
  
  /*
   * Get current resolution, posibly directly out of the camera
   */
  bool fireWireDCAM::getResolution(ipoint& resolution) const {
    eColorMode encoding;
    return getVideoMode(resolution,encoding);
  }

  /*
   * Get the set of supported framerates and resolutions.
   *
   * Note that not all combination of framerates and resolutions are
   * supported by a specific camera.  You get a vector of 
   * framerates, a vector of resolutions and a matrix indicating
   * if a given combination is supported or not.
   *
   * @param framerates vector containing a set of supported framerates.
   *                   There are cameras for which the framerate can be
   *                   "fine"-adjusted.  This method returns a subset
   *                   of it.
   * @param resols     vector containing the resolutions supported.
   * @param val        matrix, for which each entry at(i,j) 
   *                   for the framerate at position i of the \a framerates 
   *                   vector and for the resolution at position j of the
   *                   \a resols vector, contains a 0 if the combination
   *                   is not supported and not zero otherwise.
   */
  bool 
  fireWireDCAM::supportedFrameratesAndResolutions(fvector&        framerates,
                                                  vector<ipoint>& resols,
                                                  matrix<ubyte>&  val) const {
    
    if (!isActive()) {
      setStatusString("No valid camera set yet");
      return false;
    }

    framerates.clear();
    resols.clear();
    val.clear();

    std::set<float> theFramerates;
    std::set<ipoint> theResols;

    const cameraInfo& info = cams_[activeCamera_];
    // get through the supportedFramerates map, which also has the supported
    // modes
    std::map<dc1394video_mode_t,std::set<float> >::const_iterator mit;
    std::set<float>::const_iterator sit;
    ipoint resolution;
    eColorMode encoding;

    for (mit=info.supportedFramerates.begin();
         mit!=info.supportedFramerates.end();
         ++mit) {
      // for all available modes
      // get for the mode the equivalent resolution
      
      convertMode((*mit).first,resolution,encoding);
      theResols.insert(resolution);

      // for each resolution (i.e. mode) register the available framerates
      for (sit=(*mit).second.begin();
           sit!=(*mit).second.end();
           ++sit) {
        theFramerates.insert(*sit);
      }
    }

    std::map<float,int> fr2Row;   // framerate to row hash
    std::map<ipoint,int> res2Col; // resolution to column hash

    framerates.allocate(theFramerates.size());
    resols.allocate(theResols.size());
    val.assign(framerates.size(),resols.size(),0);
    int i;
    for (i=0,sit=theFramerates.begin();sit!=theFramerates.end();++sit,++i) {
      framerates[i] = (*sit);
      fr2Row[*sit]  = i;
    }

    std::set<ipoint>::const_iterator rit;
    for (i=0,rit=theResols.begin();rit!=theResols.end();++rit,++i) {
      resols[i]     = (*rit);
      res2Col[*rit] = i;
    }

    // now the difficult part, we have to build val!
    for (mit=info.supportedFramerates.begin();
         mit!=info.supportedFramerates.end();
         ++mit) {
      // for all available modes
      // get for the mode the equivalent resolution
      
      convertMode((*mit).first,resolution,encoding);
      int col = res2Col[resolution];
      // for each resolution (i.e. mode) register the available framerates
      for (sit=(*mit).second.begin();
           sit!=(*mit).second.end();
           ++sit) {
        val.at(fr2Row[*sit],col)=1;
      }
    }

    return !val.empty();
  }


  dc1394framerate_t fireWireDCAM::convertFramerate(const float fps) const {
    int twoExp = lti::iround(std::log(fps/1.875f)/std::log(2));

    if (twoExp<0) {
      twoExp=0;
    } else if (twoExp>7) {
      twoExp=7;
    }
    
    return static_cast<dc1394framerate_t>(DC1394_FRAMERATE_MIN+twoExp);
  }

  float fireWireDCAM::convertFramerate(const dc1394framerate_t eFR) const {
    float rate = float();
    dc1394_framerate_as_float(eFR,&rate);
    return rate;
  }

  fireWireDCAM::eColorMode 
  fireWireDCAM::getColorCode7(const dc1394color_coding_t code) const {
    switch (code) {
      case DC1394_COLOR_CODING_MONO8:
        return Mono8;
        break;
      case DC1394_COLOR_CODING_YUV411:
        return YUV411;
        break;
      case DC1394_COLOR_CODING_YUV422:
        return YUV422;
        break;
      case DC1394_COLOR_CODING_YUV444:
        return YUV444;
        break;
      case DC1394_COLOR_CODING_RGB8:
        return RGB8;
        break;
      case DC1394_COLOR_CODING_MONO16:
        return Mono16;
        break;
      case DC1394_COLOR_CODING_RGB16:
        return RGB16;
        break;
      case DC1394_COLOR_CODING_MONO16S:
        return Mono16s;
        break;
      case DC1394_COLOR_CODING_RGB16S:
        return RGB16s;
        break;
      case DC1394_COLOR_CODING_RAW8:
        return Raw8;
        break;
      case DC1394_COLOR_CODING_RAW16:
        return Raw16;
        break;
      default:
        return UnknownEncoding;
    }
    return UnknownEncoding;
  }

  bool fireWireDCAM::getVideoMode(ipoint& resolution,
                                  eColorMode& encoding) const {
    
    if (isNull(camera_)) {
      setStatusString("No valid camera set yet");
      return false;
    }

    dc1394video_mode_t imode;
    bool b = true;
    b = b && (dc1394_video_get_mode(camera_,&imode)==DC1394_SUCCESS);
    
    // some times the camera reports a mode it does not support!
    imode = computeClosestMode(imode);

    if (b) {
      if (imode < DC1394_VIDEO_MODE_FORMAT7_MIN) {
        // same enumeration values
        convertMode(imode,resolution,encoding);
      } else if (imode <= DC1394_VIDEO_MODE_FORMAT7_MAX){
        uint32_t x(0),y(0);
        b = b && (dc1394_format7_get_image_size(camera_,imode, &x, &y)
                  ==DC1394_SUCCESS);
        resolution.set(static_cast<int>(x),static_cast<int>(y));

        dc1394color_coding_t colorCoding;
        b = b && (dc1394_format7_get_color_coding(camera_,imode,&colorCoding)
                  ==DC1394_SUCCESS);

        encoding = getColorCode7(colorCoding);
      } else {
        // unsupported mode (or TODO yet) 
        resolution.set(0,0);
        encoding = Mono8;
      }
    }
    return b;
  }

  dc1394video_mode_t 
  fireWireDCAM::convertMode(const ipoint& resolution,
                            const eColorMode& encoding) const {
    static const int resols[] = {(160*120),
                                 (320*240),
                                 (640*480),
                                 (800*600),
                                 (1024*768),
                                 (1280*960),
                                 (1600*1200)};
    
    int i,idx,tmp;
    int d;
    idx=-1;
    // comparisons will be done on the total number of pixels
    const int np = resolution.x*resolution.y;
    d=abs(np - resols[0]);
    for (i=1;i<7;++i) {
      tmp=abs(np - resols[i]);
      if (tmp<d) {
        d=tmp;
        idx=i;
      } else {
        // since the array is sorted, we can abort here as no smaller
        // distances will be found
        break;
      }
    }

    // we have to check if the active camera supports format7.  If so,
    // we switch to format7 only if there is some error above
    if (hasCameraFormat7() && (d>0)) {
      // since d>0 then the resolution is not one of the usual ones
      // and we need to activate format7

      // Let us look for the format7 mode with the nearest resolution that
      // is larger than the desired one
      
      // largest number possible
      int bestSize=std::numeric_limits<int>::max();
      int iidx=-1;
      const dc1394format7modeset_t& f7i = cams_[activeCamera_].format7.modeSet;
      for (i=0;i<DC1394_VIDEO_MODE_FORMAT7_NUM;++i) {

        const dc1394format7mode_t& mode = f7i.mode[i];
        
        if (!mode.present) {
          continue; // mode not available, try the next one
        }

        // ok, mode i present but we don't know if it supports the desired
        // color encoding.  We have to check all available ones
        const dc1394color_codings_t& codings = mode.color_codings;
        uint32_t j=0;
        while ((j<codings.num) &&
               (encoding != getColorCode7(codings.codings[j]))) {
          ++j;
        }
        if (j<codings.num) {
          // yes, we found the right encoding

          // we got here, because there is a format7-mode present with the
          // correct color coding
          
          // compute distance (in pixels) to the desired size
          // : positive means the format7 is larger than required
          d=(mode.max_size_x*mode.max_size_y)-np;
          if ((d>=0) && (d<bestSize)) {
            bestSize=d;
            iidx=i;
          }
        }
      }
        
      if (iidx>=0) {
        // this is the best format7 mode, we assume the format can be resized
        return dc1394video_mode_t(DC1394_VIDEO_MODE_FORMAT7_MIN+iidx);
      } else {
        // no suitable format 7 mode found.
        // just let us go on with the best not-7 mode computed previously 
        if (idx<0) {
          // ups, no "best normal mode" found and no format 7 neither
          return dc1394video_mode_t(); // => return error
        }
      }
    }

    switch(idx) {
      case 0: // 160x120
        return DC1394_VIDEO_MODE_160x120_YUV444;  // this is the only availabe
                                                  // mode for this resolution
        break;
      case 1: // 320x240
        return DC1394_VIDEO_MODE_320x240_YUV422;  // this is the only availabe
                                                  // mode for this resolution
        break;
      case 2: // 640x480
        switch(encoding) {
          case Mono8:
            return DC1394_VIDEO_MODE_640x480_MONO8;
            break;
          case Mono16:
          case Mono16s:
            return DC1394_VIDEO_MODE_640x480_MONO16;
            break;
          case YUV411:
            return DC1394_VIDEO_MODE_640x480_YUV411;
            break;
          case YUV422:
            return DC1394_VIDEO_MODE_640x480_YUV422;
            break;
          case YUV444:
            return DC1394_VIDEO_MODE_640x480_YUV422; // nearest?
            break;
          case RGB8:
            return DC1394_VIDEO_MODE_640x480_RGB8;
            break;
          case RGB16:
          case RGB16s:
            return DC1394_VIDEO_MODE_640x480_RGB8; // nearest?
          default:
            return dc1394video_mode_t();
        }
        break;
      case 3: // 800x600
        switch(encoding) {
          case Mono8:
            return DC1394_VIDEO_MODE_800x600_MONO8;
            break;
          case Mono16:
          case Mono16s:
            return DC1394_VIDEO_MODE_800x600_MONO16;
            break;
          case YUV411:
            return DC1394_VIDEO_MODE_800x600_YUV422; // nearest?
            break;
          case YUV422:
            return DC1394_VIDEO_MODE_800x600_YUV422;
            break;
          case YUV444:
            return DC1394_VIDEO_MODE_800x600_YUV422; // nearest?
            break;
          case RGB8:
            return DC1394_VIDEO_MODE_800x600_RGB8;
            break;
          case RGB16:
          case RGB16s:
            return DC1394_VIDEO_MODE_800x600_RGB8; // nearest?
          default:
            return dc1394video_mode_t();
        }
        break;
      case 4: // 1024x768
        switch(encoding) {
          case Mono8:
            return DC1394_VIDEO_MODE_1024x768_MONO8;
            break;
          case Mono16:
          case Mono16s:
            return DC1394_VIDEO_MODE_1024x768_MONO16;
            break;
          case YUV411:
            return DC1394_VIDEO_MODE_1024x768_YUV422; // nearest?
            break;
          case YUV422:
            return DC1394_VIDEO_MODE_1024x768_YUV422;
            break;
          case YUV444:
            return DC1394_VIDEO_MODE_1024x768_YUV422; // nearest?
            break;
          case RGB8:
            return DC1394_VIDEO_MODE_1024x768_RGB8;
            break;
          case RGB16:
          case RGB16s:
            return DC1394_VIDEO_MODE_1024x768_RGB8; // nearest?
          default:
            return dc1394video_mode_t();
        }
        break;
      case 5: // 1280x960
         switch(encoding) {
          case Mono8:
            return DC1394_VIDEO_MODE_1280x960_MONO8;
            break;
          case Mono16:
          case Mono16s:
            return DC1394_VIDEO_MODE_1280x960_MONO16;
            break;
          case YUV411:
            return DC1394_VIDEO_MODE_1280x960_YUV422; // nearest?
            break;
          case YUV422:
            return DC1394_VIDEO_MODE_1280x960_YUV422;
            break;
          case YUV444:
            return DC1394_VIDEO_MODE_1280x960_YUV422; // nearest?
            break;
          case RGB8:
            return DC1394_VIDEO_MODE_1280x960_RGB8;
            break;
          case RGB16:
          case RGB16s:
            return DC1394_VIDEO_MODE_1280x960_RGB8; // nearest?
          default:
            return dc1394video_mode_t();
        }
       break;
      case 6: // 1600x1200
         switch(encoding) {
          case Mono8:
            return DC1394_VIDEO_MODE_1600x1200_MONO8;
            break;
          case Mono16:
          case Mono16s:
            return DC1394_VIDEO_MODE_1600x1200_MONO16;
            break;
          case YUV411:
            return DC1394_VIDEO_MODE_1600x1200_YUV422; // nearest?
            break;
          case YUV422:
            return DC1394_VIDEO_MODE_1600x1200_YUV422;
            break;
          case YUV444:
            return DC1394_VIDEO_MODE_1600x1200_YUV422; // nearest?
            break;
          case RGB8:
            return DC1394_VIDEO_MODE_1600x1200_RGB8;
            break;
          case RGB16:
          case RGB16s:
            return DC1394_VIDEO_MODE_1600x1200_RGB8; // nearest?
            break;
          default:
            return dc1394video_mode_t();
        }
       break;
      default:
        return dc1394video_mode_t(); // error, we cannot get here!
    }
    
  }
  
  void fireWireDCAM::convertMode(const dc1394video_mode_t value,
                                 ipoint& resolution,
                                 eColorMode& encoding) const {
    switch(value) {
      case DC1394_VIDEO_MODE_160x120_YUV444:
        resolution.set(160,120);
        encoding = YUV444;
        break;
      case DC1394_VIDEO_MODE_320x240_YUV422:
        resolution.set(320,240);
        encoding = YUV422;
        break;
      case DC1394_VIDEO_MODE_640x480_YUV411:
        resolution.set(640,480);
        encoding = YUV411;
        break;
      case DC1394_VIDEO_MODE_640x480_YUV422:
        resolution.set(640,480);
        encoding = YUV422;
        break;
      case DC1394_VIDEO_MODE_640x480_RGB8:
        resolution.set(640,480);
        encoding = RGB8;
        break;
      case DC1394_VIDEO_MODE_640x480_MONO8:
        resolution.set(640,480);
        encoding = Mono8;
        break;
      case DC1394_VIDEO_MODE_640x480_MONO16:
        resolution.set(640,480);
        encoding = Mono16;
        break;
      case DC1394_VIDEO_MODE_800x600_YUV422:
        resolution.set(800,600);
        encoding = YUV422;
        break;
      case DC1394_VIDEO_MODE_800x600_RGB8:
        resolution.set(800,600);
        encoding = RGB8;
        break;
      case DC1394_VIDEO_MODE_800x600_MONO8:
        resolution.set(800,600);
        encoding = Mono8;
        break;
      case DC1394_VIDEO_MODE_1024x768_YUV422:
        resolution.set(1024,768);
        encoding = YUV422;
        break;
      case DC1394_VIDEO_MODE_1024x768_RGB8:
        resolution.set(1024,768);
        encoding = RGB8;
        break;
      case DC1394_VIDEO_MODE_1024x768_MONO8:
        resolution.set(1024,768);
        encoding = Mono8;
        break;
      case DC1394_VIDEO_MODE_800x600_MONO16:
        resolution.set(800,600);
        encoding = Mono16;
        break;
      case DC1394_VIDEO_MODE_1024x768_MONO16:
        resolution.set(1024,768);
        encoding = Mono16;
        break;
      case DC1394_VIDEO_MODE_1280x960_YUV422:
        resolution.set(1280,960);
        encoding = YUV422;
        break;
      case DC1394_VIDEO_MODE_1280x960_RGB8:
        resolution.set(1280,960);
        encoding = RGB8;
        break;
      case DC1394_VIDEO_MODE_1280x960_MONO8:
        resolution.set(1280,960);
        encoding = Mono8;
        break;
      case DC1394_VIDEO_MODE_1600x1200_YUV422:
        resolution.set(1600,1200);
        encoding = YUV422;
        break;
      case DC1394_VIDEO_MODE_1600x1200_RGB8:
        resolution.set(1600,1200);
        encoding = RGB8;
        break;
      case DC1394_VIDEO_MODE_1600x1200_MONO8:
        resolution.set(1600,1200);
        encoding = Mono8;
        break;
      case DC1394_VIDEO_MODE_1280x960_MONO16:
        resolution.set(1280,960);
        encoding = Mono16;
        break;
      case DC1394_VIDEO_MODE_1600x1200_MONO16:
        resolution.set(1600,1200);
        encoding = Mono16;
        break;
      case DC1394_VIDEO_MODE_EXIF:
        // dummy values, as this mode maybe does not even exist in real cams.
        resolution.set(0,0);
        encoding = Mono8;
        break;
      case DC1394_VIDEO_MODE_FORMAT7_0:
      case DC1394_VIDEO_MODE_FORMAT7_1:
      case DC1394_VIDEO_MODE_FORMAT7_2:
      case DC1394_VIDEO_MODE_FORMAT7_3:
      case DC1394_VIDEO_MODE_FORMAT7_4:
      case DC1394_VIDEO_MODE_FORMAT7_5:
      case DC1394_VIDEO_MODE_FORMAT7_6:
      case DC1394_VIDEO_MODE_FORMAT7_7:
        convertMode7(value,resolution,encoding);
        break;
      default:
        resolution.set(0,0);
        encoding = Mono8;
        break;
    }
  }

  void fireWireDCAM::convertMode7(const dc1394video_mode_t value,
                                  ipoint& resolution,
                                  eColorMode& encoding) const {

    if (hasCameraFormat7()) {
      if ((value>=DC1394_VIDEO_MODE_FORMAT7_MIN) && 
          (value<=DC1394_VIDEO_MODE_FORMAT7_MAX)) {
        const dc1394format7modeset_t& f7ms =
          cams_[activeCamera_].format7.modeSet;
          
        const dc1394format7mode_t& f7m = 
          f7ms.mode[value-DC1394_VIDEO_MODE_FORMAT7_MIN];
        
        if (f7m.present) {
          resolution.set(f7m.max_size_x,f7m.max_size_y);
          encoding = getColorCode7(f7m.color_coding);
          return;
        }
      }
      // else -> not format7
    }

    // unsupported format
    resolution.set(0,0);
    encoding = Mono8;
  }

  // unsigned int fireWireDCAM::getFormat(const ipoint& resolution) const {
  //   const int d = resolution.x*resolution.y;
  //   if ((d==160*120) ||
  //       (d==320*240) ||
  //       (d==640*480)) {
  //     return FORMAT_VGA_NONCOMPRESSED;
  //   } else if ((d==800*600) ||
  //              (d==1024*768)) {
  //     return FORMAT_SVGA_NONCOMPRESSED_1;
  //   } else if ((d==1280*960) ||
  //              (d==1600*1200)) {
  //     return FORMAT_SVGA_NONCOMPRESSED_2;
  //   } 
  //   return FORMAT_SCALABLE_IMAGE_SIZE;
  // }


  // ------------------------------------------------------------------------
  // One-Push scheduler
  // ------------------------------------------------------------------------
  bool fireWireDCAM::onePushQueueInsert(const unsigned int featureId) {
    
    if (notNull(camera_)) {
      setStatusString("No camera active yet");
      return false;
    }

    // read only reference to 
    const dc1394feature_info_t& fts
      = features_->feature[featureId-DC1394_FEATURE_MIN];

    uint32_t f;
    for (f=0;f<fts.modes.num;++f) {
      if (fts.modes.modes[f] == DC1394_FEATURE_MODE_ONE_PUSH_AUTO) {
        break;
      }
    }

    if (f<fts.modes.num) {
      // one push supported for that feature
      if (dc1394_feature_set_mode(camera_,
                                  dc1394feature_t(featureId),
                                  DC1394_FEATURE_MODE_ONE_PUSH_AUTO)
          != DC1394_SUCCESS) {
        std::string str = "One-push for feature ";
        str += dc1394_feature_get_string(dc1394feature_t(featureId));
        str += " could not be activated";
        setStatusString(str.c_str());
        return false;
      }
      onePushLock_.lock();
      waitingFeatures_.insert(featureId);
      onePushLock_.unlock();
      return true;
    }

    return false; // not supported at all
  }

  bool fireWireDCAM::onePushWait() {
    const parameters& par = getParameters();

    double theTime = 0;
    const double timeStep = 1000000.0/par.onePushUpdateRate;
    const double timeout = par.onePushTimeout;
    std::set<unsigned int>::iterator it,tmpIt;
    bool ready = false;

    while (!ready && (theTime < timeout)) {
      onePushLock_.lock();
      it = waitingFeatures_.begin();
      while (it != waitingFeatures_.end()) {
        const dc1394feature_t featureId = dc1394feature_t(*it);
        dc1394feature_mode_t mode;

        if (dc1394_feature_get_mode(camera_,featureId,&mode)==DC1394_SUCCESS) {
          if (mode == DC1394_FEATURE_MODE_ONE_PUSH_AUTO) {
            // this feature was ready: remove it from the queue
            tmpIt=it;
            ++it;
            waitingFeatures_.erase(tmpIt);
          } else {
            // still active, just continue
            ++it;
          }        
        } else {
          // we cannot get information on that feature, better remove it
          tmpIt=it;
          ++it;
          waitingFeatures_.erase(tmpIt);
        }
      }
      ready = waitingFeatures_.empty();
      onePushLock_.unlock();
      // now wait and then try again, if we still have time
      passiveWait(static_cast<int>(timeStep));
      theTime+=timeStep;
    }
    
    return ready;
  }

  bool fireWireDCAM::isoFlowStop() {
    if (!isActive()) {
      setStatusString("No active camera yet");
      return false;
    }

    dc1394switch_t pwr;

    if (dc1394_video_get_transmission(camera_,&pwr)!=DC1394_SUCCESS) {
      setStatusString("Could not get iso status");
      return false;
    }
    else if (pwr != DC1394_OFF) {
      if (dc1394_video_set_transmission(camera_,DC1394_OFF)!=DC1394_SUCCESS) {
        // ... (if not done, restarting is no more possible)
        setStatusString("Could not stop ISO transmission");
        return false;
      }
    }
    return true;
  }

  bool fireWireDCAM::isoFlowStopTotal() {
    return isoFlowStop();
  }

  bool fireWireDCAM::isoFlowResume() {

    if (!isActive()) {
      // Unset camera cannot resume
      return false;
    }

    dc1394switch_t isoOn;
    dc1394_video_get_transmission(camera_,&isoOn);

    if (isoOn != DC1394_ON) {
      // restart iff it was 'off' before 

      if (dc1394_video_set_transmission(camera_,DC1394_ON) != DC1394_SUCCESS) {
        setStatusString("Could not start ISO transmission");
        return false;
      }

      if (dc1394_video_get_transmission(camera_,&isoOn) != DC1394_SUCCESS) {
        setStatusString("Could not get ISO status");
        return false;
      } else {

        int timeout=0;

        // ISO could not be restarted. Trying again for 5 seconds
        while ( (isoOn == DC1394_OFF) && (timeout<5000) ) {
          passiveWait(Delay);
          timeout+=Delay/1000;
          if (dc1394_video_set_transmission(camera_,DC1394_ON)
              != DC1394_SUCCESS) {
            // ... (if not done, restarting is no more possible)
            setStatusString("Could not start ISO transmission");
            return false;
          } else if (dc1394_video_get_transmission(camera_,&isoOn) !=
                     DC1394_SUCCESS) {
            setStatusString("Could not get ISO status");
            return false;
          }
        }

        if (isoOn == DC1394_OFF) {
          setStatusString("Can't start ISO, giving up...");
          return false;
        }
      }
    }

    return true;
  }


  /**
   * Initialize frame grabber.
   *
   * @return true if successful, false otherwise
   */
  bool fireWireDCAM::init() {
    // camera set does this the first time a fireWireDCAM is created
    return isInitialized();
  }

  /**
   * Initialize frame grabber.
   *
   * @return true if successful, false otherwise
   */
  bool fireWireDCAM::isInitialized() const {
    // camera set does this the first time a fireWireDCAM is created
    return cams_.size()>0;
  }

  const char* fireWireDCAM::errorString(dc1394error_t err) {
    static const char *const errStr[] = {
      "DC1394_SUCCESS                    ",
      "DC1394_FAILURE                    ",
      "DC1394_NOT_A_CAMERA               ",
      "DC1394_FUNCTION_NOT_SUPPORTED     ",
      "DC1394_CAMERA_NOT_INITIALIZED     ",
      "DC1394_MEMORY_ALLOCATION_FAILURE  ",
      "DC1394_TAGGED_REGISTER_NOT_FOUND  ",
      "DC1394_NO_ISO_CHANNEL             ",
      "DC1394_NO_BANDWIDTH               ",
      "DC1394_IOCTL_FAILURE              ",
      "DC1394_CAPTURE_IS_NOT_SET         ",
      "DC1394_CAPTURE_IS_RUNNING         ",
      "DC1394_RAW1394_FAILURE            ",
      "DC1394_FORMAT7_ERROR_FLAG_1       ",
      "DC1394_FORMAT7_ERROR_FLAG_2       ",
      "DC1394_INVALID_ARGUMENT_VALUE     ",
      "DC1394_REQ_VALUE_OUTSIDE_RANGE    ",
      "DC1394_INVALID_FEATURE            ",
      "DC1394_INVALID_VIDEO_FORMAT       ",
      "DC1394_INVALID_VIDEO_MODE         ",
      "DC1394_INVALID_FRAMERATE          ",
      "DC1394_INVALID_TRIGGER_MODE       ",
      "DC1394_INVALID_TRIGGER_SOURCE     ",
      "DC1394_INVALID_ISO_SPEED          ",
      "DC1394_INVALID_IIDC_VERSION       ",
      "DC1394_INVALID_COLOR_CODING       ",
      "DC1394_INVALID_COLOR_FILTER       ",
      "DC1394_INVALID_CAPTURE_POLICY     ",
      "DC1394_INVALID_ERROR_CODE         ",
      "DC1394_INVALID_BAYER_METHOD       ",
      "DC1394_INVALID_VIDEO1394_DEVICE   ",
      "DC1394_INVALID_OPERATION_MODE     ",
      "DC1394_INVALID_TRIGGER_POLARITY   ",
      "DC1394_INVALID_FEATURE_MODE       ",
      "DC1394_INVALID_LOG_TYPE           ",
      "DC1394_INVALID_BYTE_ORDER         ",
      "DC1394_INVALID_STEREO_METHOD      ",
      "DC1394_BASLER_NO_MORE_SFF_CHUNKS  ",
      "DC1394_BASLER_CORRUPTED_SFF_CHUNK ",
      "DC1394_BASLER_UNKNOWN_SFF_CHUNK   "};
    if ((err<=0) && (err>=-39)) {
      return errStr[-int(err)];
    } 
    return "Unknown dc1394 error";
  }

  
}
    

#include "ltiUndebug.h"

#endif //USE_FIRE_WIRE_DCAM
