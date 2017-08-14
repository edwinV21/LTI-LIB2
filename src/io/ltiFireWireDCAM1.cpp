
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
 * \file   ltiFireWireDCAM.cpp
 *         Contains a class for grabbing images from FireWire cameras.
 * \author Arnd Hannemann
 * \author Pablo Alvarado
 * \date   21.10.2004
 * $Id: ltiFireWireDCAM1.cpp,v 1.1 2012-01-05 18:32:32 alvarado Exp $
 */

#include "ltiHardwareConfig.h"

#if defined(_USE_FIRE_WIRE_DCAM) && defined(_LTI_DC1394_VER_1)

#include "ltiFireWireDCAM1.h"
#include "ltiMutex.h"
#include "ltiLispStreamHandler.h"
#include "ltiTimer.h"
#include "ltiFactory.h"
#include "ltiPassiveWait.h"

#include <cstring>
#include <sstream>
#include <fstream>

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
  // fireWireDCAM::busInfo
  // --------------------------------------------------
  fireWireDCAM::busInfo::busInfo() :
    handles(0),cameraNodes(0),cameraNum(-1),portCameraNum(0),portNum(0),
    cardFound(false) {
  }

  fireWireDCAM::busInfo::~busInfo() {
    deleteCameraNodes();
  }

  int fireWireDCAM::busInfo::busResetHandler(raw1394handle_t,
                                             unsigned int) {
    _lti_debug("Bus reset handler was called!\n");

    // TODO:  we need to decide what is to be done if we get here
    
    return 1;
  }
  
  bool fireWireDCAM::busInfo::get() {
    return getNodes();
  }

  bool fireWireDCAM::busInfo::deleteCameraNodes() {
    if (notNull(cameraNodes)   &&
        notNull(portCameraNum) &&
        notNull(handles)) {
        
      for (int port=0;port<portNum;port++) {
        dc1394_free_camera_nodes(cameraNodes[port]);
        cameraNodes[port]=0;
        dc1394_destroy_handle(handles[port]);
        handles[port]=0;
      }
      delete[] cameraNodes;
      cameraNodes=0;

      delete[] portCameraNum;
      portCameraNum=0;

      delete[] handles;
      handles=0;

      return true;
    } else {
      cameraNodes=0;
      portCameraNum=0;
      handles=0;
    }

    return false;
  }

  bool fireWireDCAM::busInfo::getNodes() {
    // The following code has been adapted from the method GetCameraNodes in
    // the file camera.c of coriander (by Damien Douxchamps)
    
    raw1394handle_t tmpHandle;
    int port;

    tmpHandle=raw1394_new_handle(); // calling the library handler method
    cardFound=false;
    cameraNum=0;

    if (notNull(tmpHandle)) {
      // Just get how many ports are required
      portNum=raw1394_get_port_info(tmpHandle, NULL, 0);      
      // and destroy the handler
      raw1394_destroy_handle(tmpHandle);

      // ensure that no nodes are being used
      deleteCameraNodes();

      typedef nodeid_t* nodeIdPtr;

      // allocate the required memory
      cameraNodes=new nodeIdPtr[portNum];
      portCameraNum=new int[portNum];
      handles=new raw1394handle_t[portNum];

      // initialize the memory
      for (port=0;port<portNum;port++) {
        // get a handle to the current interface card
        handles[port]=dc1394_create_handle(port);
        if (notNull(handles[port])) { // if the card is present
          cardFound=true;

          // set bus reset handler
          raw1394_set_bus_reset_handler(handles[port], busResetHandler);

          // probe the IEEE1394 bus for DC camera:
          // this method write the number of cameras in the portCameraNum[.]
          cameraNodes[port]=dc1394_get_camera_nodes(handles[port],
                                                    &portCameraNum[port],
                                                    0); // 0 don't show cams.
          // accumulate the total number of cameras
          cameraNum+=portCameraNum[port];
        } else {
          cameraNodes[port]=0;
          portCameraNum[port]=0;
        }
      }
    }
    
    return cardFound;
  }

  // --------------------------------------------------
  // fireWireDCAM::cameraInfo
  // --------------------------------------------------
  fireWireDCAM::cameraInfo::cameraInfo() {
    // ensure that all data is initialized to some value
    memset(&info,0,sizeof(dc1394_camerainfo));
    memset(&featureSet,0,sizeof(dc1394_feature_set));
    memset(&miscInfo,0,sizeof(dc1394_miscinfo));
    info.handle=0;
    inUse=false;
  }

  fireWireDCAM::cameraInfo::~cameraInfo() {
    inUse=false;
    if (notNull(info.handle)) {
      dc1394_destroy_handle(info.handle);
    }
  }

  bool fireWireDCAM::cameraInfo::getFormat7Capabilities() {
     int i, f;
     quadlet_t value;
     bool error=false;
     format7.available = false;
  
     if (dc1394_query_supported_formats(info.handle, info.id, &value) !=
         DC1394_SUCCESS) {
       errorStr+="Could not query supported formats. ";
       error=true;
     } else {
       if (value & (0x1 << 24)) { // is format7 supported?
         if (dc1394_query_supported_modes(info.handle,
                                          info.id,
                                          FORMAT_SCALABLE_IMAGE_SIZE,
                                          &value)!=DC1394_SUCCESS) {
           errorStr+="Could not query Format7 supported modes. ";
           error=true;
         } else {
           format7.available = true;
           for (i=0,f=MODE_FORMAT7_MIN; f<=MODE_FORMAT7_MAX; f++,i++) {
             format7.mode[i].present = ((value & (0x1 << (31-i))) != 0);
             getFormat7ModeInfo(f);
           }
         }
       }
       else { // format7 is not supported!!
         format7.available = false;
         for (i=0,f=MODE_FORMAT7_MIN;f<=MODE_FORMAT7_MAX;f++,i++) {
           format7.mode[i].present=false;
         }
       }
     }
     return !error;
  }

  bool 
  fireWireDCAM::cameraInfo::getFormat7ModeInfo(const unsigned int modeId) {

    // modeId : 

    format7ModeInfo& mode = format7.mode[modeId-MODE_FORMAT7_MIN];
    bool error=false;

    if (mode.present) { // check for mode presence before query
      if (dc1394_query_format7_max_image_size(info.handle,
                                              info.id,
                                              modeId,
                                              &mode.maxSize.x,
                                              &mode.maxSize.y) != 
          DC1394_SUCCESS) {
        errorStr += "Error querying format7 max image size. ";
        error=true;
      }
      
      if (dc1394_query_format7_unit_size(info.handle,
                                         info.id,
                                         modeId,
                                         &mode.unitSize.x,
                                         &mode.unitSize.y) !=
          DC1394_SUCCESS) {
        errorStr += "Error querying format7 unit size. ";
        error=true;
      }

      // quick hack to keep size/position even. If pos/size is ODD,
      // strange color/distorsions occur on some cams (e.g. Basler
      // cams). This will have to be really fixed later. 
      // REM: this is fixed by using the unit_position: 

      // fprintf(stderr,"Using pos units = %d %d\n",info->mode[i].step_pos_x,
      // info->mode[i].step_pos_y);

      if (dc1394_query_format7_unit_position(info.handle,
                                             info.id,
                                             modeId,
                                             &mode.unitPos.x,
                                             &mode.unitPos.y) !=
          DC1394_SUCCESS) {
        errorStr += "Error querying format7 unit position. ";
        error=true;
      }

      mode.unitPos.set(0,0);
    

      if (dc1394_query_format7_image_position(info.handle,
                                              info.id,
                                              modeId,
                                              &mode.pos.x,
                                              &mode.pos.y)!=DC1394_SUCCESS) {
        errorStr += "Error querying format7 image position. ";
        error=true;
      }
      
      if (dc1394_query_format7_image_size(info.handle,
                                          info.id,
                                          modeId,
                                          &mode.size.x,
                                          &mode.size.y) != DC1394_SUCCESS) {
        errorStr += "Error querying format7 image size. ";
        error=true;
      }
      
      if (dc1394_query_format7_byte_per_packet(info.handle,
                                               info.id,
                                               modeId,
                                               &mode.bpp) != DC1394_SUCCESS) {
        errorStr += "Error querying format7 bytes per packet. ";
        error=true;
      }
      
      if (mode.bpp==0) {
        // sometimes a camera will not set the bpp register until a
        // valid image size has been set after boot. If BPP is zero, we
        // therefore try again after setting the image size to the
        // maximum size.
        errorStr += "Camera reported a BPP of ZERO.";
        errorStr += "Trying to set maximum size to correct this. ";
        
        if (dc1394_set_format7_image_position(info.handle,
                                              info.id,
                                              modeId,0,0) != DC1394_SUCCESS) {
          errorStr += "Error setting format7 image position. ";
          error=true;
        }
        
        if (dc1394_set_format7_image_size(info.handle,
                                          info.id,
                                          modeId,
                                          mode.maxSize.x,
                                          mode.maxSize.y) != DC1394_SUCCESS) {
          errorStr += "Error setting format7 image size. ";
          error=true;
        }
        
        // maybe we should also force a color coding here.
        if (dc1394_query_format7_byte_per_packet(info.handle,
                                                 info.id,
                                                 modeId,
                                                 &mode.bpp)!=DC1394_SUCCESS) {
          errorStr += "Error querying format7 bytes per packet. ";
          error=true;
        }
        
        if (mode.bpp==0) {
          errorStr += "    BPP still zero. Giving up. ";
          error=true;
        }
      }
      
      if (dc1394_query_format7_packet_para(info.handle,
                                           info.id,
                                           modeId,
                                           &mode.minBpp,
                                           &mode.maxBpp) !=DC1394_SUCCESS) {
        errorStr += "Error querying format7 packet parameters. ";
        error=true;
      }
      
      if (dc1394_query_format7_pixel_number(info.handle,
                                            info.id,
                                            modeId,
                                            &mode.pixnum) !=DC1394_SUCCESS) {
        errorStr += "Error querying format7 pixel number. ";
        error=true;
      }
      
      // we need a temporal variable of the exact type.
      unsigned long long int tmp64;

      if (dc1394_query_format7_total_bytes(info.handle,
                                           info.id,
                                           modeId,
                                           &tmp64)!=DC1394_SUCCESS) {
        errorStr += "Error querying format7 total bytes per frame. ";
        error=true;
      } else {
        mode.totalBytes = static_cast<uint64>(tmp64); 
      }
      


      if (dc1394_query_format7_color_coding_id(info.handle,
                                               info.id,
                                               modeId,
                                               &mode.colorCodingId) !=
          DC1394_SUCCESS) {
        errorStr += "Error querying format7 color coding ID. ";
        error=true;
      }
      
      if (dc1394_query_format7_color_coding(info.handle,
                                            info.id,
                                            modeId,
                                            &mode.colorCoding) != 
          DC1394_SUCCESS) {
        errorStr += "Error querying format7 color coding. ";
        error=true;
      }

      if (dc1394_query_format7_packet_per_frame(info.handle,
                                                info.id,
                                                modeId,
                                                &mode.packetsPerFrame) != 
          DC1394_SUCCESS) {
        errorStr += "Error querying format7 packets per frame. ";
        error=true;
      }

    } else {
      // mode is not present, so don't even try to get info from the camera
      // just reset all values
      mode.size.set(0,0);
      mode.maxSize.set(0,0);
      mode.pos.set(0,0);
      mode.unitSize.set(0,0);
      mode.unitPos.set(0,0);
      mode.colorCodingId = 0;
      mode.colorCoding = 0;  // this is an invalid value, since it should>=320
      mode.pixnum      = 0;
      mode.bpp         = 0;
      mode.minBpp      = 0;
      mode.maxBpp      = 0;
      mode.totalBytes  = 0;

      error = false;
    }

    
    return (!error);
  }

  bool fireWireDCAM::cameraInfo::
  getSupportedModesAndFramerates(const nodeid_t& node) {

    static const uint32 fbits[] = { uint32(0x1)<<31, // Format 0
                                    uint32(0x1)<<30, // Format 1
                                    uint32(0x1)<<29, // Format 2
                                    uint32(0x1)<<28, // Format 3
                                    uint32(0x1)<<27, // Format 4
                                    uint32(0x1)<<26, // Format 5
                                    uint32(0x1)<<25, // Format 6
                                    uint32(0x1)<<24  // Format 7
    };
    
    static const uint32 fmin[] = { MODE_FORMAT0_MIN,
                                   MODE_FORMAT1_MIN,
                                   MODE_FORMAT2_MIN,
                                   1,
                                   1,
                                   1,
                                   MODE_FORMAT6_MIN,
                                   MODE_FORMAT7_MIN };

    static const uint32 fmax[] = { MODE_FORMAT0_MAX,
                                   MODE_FORMAT1_MAX,
                                   MODE_FORMAT2_MAX,
                                   0,
                                   0,
                                   0,
                                   MODE_FORMAT6_MAX,
                                   MODE_FORMAT7_MAX };

    static const float framerates[] = {1.875f,
                                       3.75f,
                                       7.5f,
                                       15.f,
                                       30.f,
                                       60.f,
                                       120.f,
                                       240.f};

    // clear all available information
    int i,total(0);
    uint32 j,f;
    supportedModes.clear();
    supportedFramerates.clear();
    quadlet_t formats=0,modes=0,fr=0;

    if (dc1394_query_supported_formats(info.handle,
                                       node,
                                       &formats) != DC1394_SUCCESS) {
      errorStr += "Could not get supported formats";
      return false;
    }

    // for each format (0 to 2) but not 6 or 7, because they are different!
    for (i=0;i<3;++i) {
      // if the format is supported
      if ((formats & fbits[i]) != 0) {
        // format i is supported! -> get modes for such format
        if ((fmin[i]<=fmax[i]) &&
            (dc1394_query_supported_modes(info.handle,
                                          node,
                                          i+FORMAT_MIN,
                                          &modes) == DC1394_SUCCESS)) {

          for (j=fmin[i];j<=fmax[i];++j) {
            if (modes & (uint32(0x1)<<(31-(j-fmin[i])))) {
              supportedModes.insert(j);
              total++;

              // now that we have a supported mode, we can ask for
              // supported framerates
              if (dc1394_query_supported_framerates(info.handle, node,
                                                    i+FORMAT_MIN,j,
                                                    &fr)==DC1394_SUCCESS) {
                for (f=0;f<NUM_FRAMERATES;++f) {
                  if (fr & (uint32(0x1) << (31-f))) {
                    supportedFramerates[j].insert(framerates[f]);
                  } // if framerate supported
                } // for all possible framerates
              } // if query of supported framerates for mode is ok 
            } // if mode supported
          } // for all modes in the format
        } // if second check if format is supported
      } // end of if format supported
    } // end of for formats 0..2
    
    // format 6 is for still images, so there are not "frame rate" for a still
    // image
    
    // for format 7 we need to compute supported framerate and
    // supported resolutions
    
    // we can assume, when we get here, that the cameraInfo::format7 structure
    // has already being filled, so that we can get the information there.
    if (format7.available) {
      for (i=0;i<NUM_MODE_FORMAT7;++i) {
        if (format7.mode[i].present) {
          j = i+fmin[7];
          supportedModes.insert(j); // insert supported mode
          
          // since the camera has already being initialized, we cannot
          // change the packets per frame, and therefore, the only available 
          // framerate has indirectly already being set.
          //
          // Compute framerate from packet size.  See
          // http://damien.douxchamps.net/ieee1394/libdc1394/v1.x/faq.php
          
          // the iso_speed is from an enum value, busPeriod in us
          float busPeriod = 500.0f / (uint32(0x1) << miscInfo.iso_speed);

          supportedFramerates[j].insert(1000000.0f/ 
                                 (busPeriod*format7.mode[i].packetsPerFrame));

          // framerate = 1.0/(busPeriod * numPackets)
        }
      }
    }
     
    return true;
  }

  bool fireWireDCAM::cameraInfo::get(const int port,const nodeid_t& node) {
    
    info.handle=dc1394_create_handle(port);
    bool error=false;
    errorStr.clear();

    if (dc1394_get_camera_info(info.handle, node, &info) !=
        DC1394_SUCCESS) {
      errorStr="Could not get camera basic information! ";
      error=true;
    }

    // after the previous call the info struct contains now the node also:
    if (dc1394_get_camera_misc_info(info.handle, info.id, &miscInfo) !=
        DC1394_SUCCESS) {
      errorStr+= "Could not get camera misc information! ";
      error=true;
    }

    if (dc1394_get_camera_feature_set(info.handle,info.id, &featureSet) !=
        DC1394_SUCCESS) {
      errorStr+= "Could not get camera feature information! ";
      error=true;
    }

    if (dc1394_query_basic_functionality(info.handle,info.id, &basics) !=
        DC1394_SUCCESS) {
      errorStr+= "Could not get camera basics information! ";
      error=true;
    }
    

    error = error || !getFormat7Capabilities();
    error = error || !getSupportedModesAndFramerates(node);
    
    return !error;
  }

  // --------------------------------------------------
  // fireWireDCAM::cameraSet
  // --------------------------------------------------
  fireWireDCAM::cameraSet::cameraSet() {
  }

  fireWireDCAM::cameraSet::~cameraSet() {
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

  bool fireWireDCAM::cameraSet::get(busInfo& bus) {
    lock_.lock();
    cams_.clear(); // begin with an empty camera set
    int port,i,end;
    cameraInfo dummyCam;
    bool error = false;

    // for each available port
    for (port=0;port<bus.portNum;port++) {
      // if this port has some handles
      if (notNull(bus.handles[port])) {
        // for each camera in the port
        for (i=0;i<bus.portCameraNum[port];i++) {
          // get camera data
          cams_.push_back(dummyCam);
          if (!cams_.back().get(port,*(bus.cameraNodes[i]))) {
            error=true;
            errorStr+=cams_.back().errorStr;
            cams_.pop_back();
          } else {
            // check that the camera is not yet found through another
            // interface card (for strange bus topologies):
            end=cams_.size()-1;  // check all but the last one
            for (i=0;i<end;++i) {
              if (cams_[i].info.euid_64 == cams_.back().info.euid_64) {
                // camera already there!  remove and continue
                cams_.pop_back();
              }
            }
          }
        }
      }
    }
    lock_.unlock();
    return !error;
  }

  bool fireWireDCAM::cameraSet::findCamera(const u_int64_t& euID64,
                                           int& camIdx) const {
    unsigned int i;
    camIdx=-1;
    lock_.lock();
    for (i=0;i<cams_.size();++i) {
      if (cams_[i].info.euid_64 == euID64) {
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
      return false; // cannot be release, because is not in use!
    }
    lock_.lock();
    cams_[idx].inUse = false;
    lock_.unlock();
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
                                   u_int64_t& id) const {    
    bool error = false;
    if (!init_) {
      return false;
    }
    lock_.lock();
    std::map<std::string,u_int64_t>::const_iterator it;
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

  bool fireWireDCAM::nameDB::query(const u_int64_t& id,
                                   std::string& name) const {    
    if (!init_) {
      return false;
    }
    lock_.lock();

    std::map<std::string,u_int64_t>::const_iterator it,eit;

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


  bool fireWireDCAM::nameDB::add(const std::string& name,u_int64_t& id) {
    bool error = false;
    if (!init_) {
      return false;
    }

    lock_.lock();
    // try to find if the data already exists
    std::map<std::string,u_int64_t>::const_iterator it;
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
    std::map<std::string,u_int64_t>::iterator it;
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
      std::map<std::string,u_int64_t>::const_iterator it;
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
      u_int64_t d;
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

  void fireWireDCAM::nameDB::hexToString(const u_int64_t& data,
                                         std::string& str) const {

    static const char d2a[] = {'0','1','2','3','4','5','6','7','8','9',
                               'A','B','C','D','E','F'};
    str.clear();
    u_int64_t d = data;
    uint n;

    do {
      n= static_cast<uint>(d & 0x0F);
      str = d2a[n]+str;// append to the left the new digit
      d=d>>4;// take the lowest 4 bits away shifting at the same time the rest.
    } while(d!=0);
    str="0x"+str;
  }
  
  bool fireWireDCAM::nameDB::stringToHex(const std::string& str,
                                         u_int64_t& data) const {

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
  bool fireWireDCAM::hwCreated_ = false;
  fireWireDCAM::busInfo fireWireDCAM::bus_;
  fireWireDCAM::cameraSet fireWireDCAM::cams_;
  fireWireDCAM::nameDB fireWireDCAM::cameraNames_;

  // --------------------------------------------------
  // fireWireDCAM
  // --------------------------------------------------

  // default constructor
  fireWireDCAM::fireWireDCAM()
    : frameGrabber(),camera(),panTiltUnit(),lensUnit() {

    activeCamera_=-1; // set an invalid value

    snapShotCapable_ = false;

    handle_=0;
    features_=0;
    node_=0;
    
    busChannel_ = 0;
    busSpeed_ = 0;

    colorMode_ = YUV422;
    
    memset(&camera_,0,sizeof(dc1394_cameracapture));
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

    snapShotCapable_ = false;

    handle_=0;
    features_=0;
    node_=0;

    busChannel_ = 0;
    busSpeed_ = 0;

    colorMode_ = YUV422;
    
    memset(&camera_,0,sizeof(dc1394_cameracapture));
    captureSetUp_ = false;
   
    bayer_ = 0;

    // set the default parameters
    setParameters(par);
  }


  // copy constructor
  fireWireDCAM::fireWireDCAM(const fireWireDCAM& other)
    : frameGrabber(),camera(),panTiltUnit(),lensUnit() {

    activeCamera_=-1; // set an invalid value

    snapShotCapable_ = false;

    handle_=0;
    features_=0;
    node_=0;

    busChannel_ = 0;
    busSpeed_ = 0;

    colorMode_ = YUV422;
    
    memset(&camera_,0,sizeof(dc1394_cameracapture));
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
    if (activeCamera_>=0) { // only if the camera is active, the array
                            // contains meaningful data
      return cams_[activeCamera_].format7.available;
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

    if ((cams_[activeCamera_].basics & (0x1<<16)) !=0) {
      dc1394_camera_off(*handle_,*node_);
    }

    if (captureSetUp_) {
      _lti_debug("release old camera structure...\n");

      // remember: even if this is called release "camera", it just
      // releases the "capture" relevant stuff
      dc1394_dma_release_camera(*handle_,&camera_);
      captureSetUp_ = false;
    }

    cams_.releaseIndex(activeCamera_);
    activeCamera_=-1;
    activeCameraName_.clear();
    handle_=0;
    features_=0;
    node_=0;
    
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

    if (isFeatureAvailable(featureId)) {
      const dc1394_feature_info& fi = 
        cams_[activeCamera_].featureSet.feature[featureId-FEATURE_MIN];
      if (value < fi.min) {
        value = fi.min;
      } else if (value > fi.max) {
        value = fi.max;
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
    unsigned int mode;
    mode = convertMode(par.resolution,par.encoding);

    // now, we have to check if the mode is supported.
    // If it is a mode of the format7, then due to the implementation of 
    // the previous method, it will be supported.  If the mode is of formats
    // 0, 1 or 2, then we may require many adaptions
    if (mode <= MODE_FORMAT2_MAX) {
      mode = computeClosestMode(mode);      
    } 

    convertMode(mode,par.resolution,par.encoding); // get it back to LTI-Lib
    par.framerate = computeClosestFrameRate(mode,par.framerate);

    // now use the camera information to adjust some values if necessary
    fixFeature(FEATURE_BRIGHTNESS,par.brightness,par.brightnessMode);
    fixFeature(FEATURE_EXPOSURE,par.exposure,par.exposureMode);
    fixFeature(FEATURE_SHARPNESS,par.sharpness,par.sharpnessMode);

    if (isFeatureAvailable(FEATURE_WHITE_BALANCE)) {
      const dc1394_feature_info& fi = 
        cam.featureSet.feature[FEATURE_WHITE_BALANCE-FEATURE_MIN];
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
      unsigned int states = checkFeatureModes(FEATURE_WHITE_BALANCE);
      par.whiteBalanceMode = fixMode(states,par.whiteBalanceMode);
    } else {
      par.blueGain = par.redGain = 0;
      par.whiteBalanceMode = FeatureNA;
    }

    fixFeature(FEATURE_HUE,par.hue,par.hueMode);
    fixFeature(FEATURE_SATURATION,par.saturation,par.saturationMode);
    fixFeature(FEATURE_GAMMA,par.gamma,par.gammaMode);
    fixFeature(FEATURE_SHUTTER,par.shutter,par.shutterMode);
    fixFeature(FEATURE_GAIN,par.gain,par.gainMode);
    fixFeature(FEATURE_IRIS,par.iris,par.irisMode);
    fixFeature(FEATURE_FOCUS,par.focus,par.focusMode);
    fixFeature(FEATURE_ZOOM,par.zoom,par.zoomMode);
    fixFeature(FEATURE_PAN,par.pan,par.panMode);
    fixFeature(FEATURE_TILT,par.tilt,par.tiltMode);
    fixFeature(FEATURE_OPTICAL_FILTER,par.opticalFilter,par.opticalFilterMode);

    return true;
  }
  
  bool fireWireDCAM::isModeColor(const unsigned int mode) const {
    // to make this method efficient, we use some low-level information about
    // the bit settings for the formats.
    return !(((mode & 0x07) >= 5) ||
             (((mode>=MODE_FORMAT1_MIN) && (mode & 0x07)==2)));
  }

  float fireWireDCAM::computeClosestFrameRate(const unsigned int mode,
                                              const float fr) const {

    const cameraInfo& cam = cams_[activeCamera_];

    // get closest supported framerate for the selected mode
    std::map<uint32,std::set<float> >::const_iterator frsi;
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

  unsigned int 
  fireWireDCAM::computeClosestMode(const unsigned int mode) const {
  
    const std::set<uint32>& sm = cams_[activeCamera_].supportedModes;
    if (sm.empty()) {
      return 0; // as error signal!
    }

    if (mode>MODE_FORMAT2_MAX) {
      std::set<uint32>::const_iterator it;
      it = sm.find(mode);
      if (it != sm.end()) {
        return mode; // it is supported
      }
      return 0;  // not one of the normal modes
    }

    // the approximation is done with the following priorities:
    // 1. keep the mono or color attribute (unless your camera is
    //    monochrome, of course)
    // 2. select the closest resolution.
    
    // separate sets in color and monochrome modes
    std::set<uint32> inColor,monochrome;
    std::set<uint32>::const_iterator it,best;

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

    parameters& par = getRWParameters();

    // delete old bayer functor
    delete bayer_;

    // create new one
    const factory<bayerDemosaicing>& f=factory<bayerDemosaicing>::getFactory();

    bayer_ = f.newInstance(par.bayerMethod); 
    if (bayer_==0) {
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
    if (isFeatureAvailable(FEATURE_BRIGHTNESS)) {
      b = b && setBrightness(par.brightnessMode,par.brightness);
    }

    // exposure
    if (isFeatureAvailable(FEATURE_EXPOSURE)) {
      b = b && setExposure(par.exposureMode,par.exposure);
    }

    // sharpness
    if (isFeatureAvailable(FEATURE_SHARPNESS)) {
      b = b && setSharpness(par.sharpnessMode,par.sharpness);
    }

    // white balance
    if (isFeatureAvailable(FEATURE_WHITE_BALANCE)) {
      b = b && setWhiteBalance(par.whiteBalanceMode,par.blueGain,par.redGain);
    }

    // hue
    if (isFeatureAvailable(FEATURE_HUE)) {
      b = b && setHue(par.hueMode,par.hue);
    }

    // saturation
    if (isFeatureAvailable(FEATURE_SATURATION)) {
      b = b && setSaturation(par.saturationMode,par.saturation);
    }

    // gamma
    if (isFeatureAvailable(FEATURE_GAMMA)) {
      b = b && setGamma(par.gammaMode,par.gamma);
    }

    // shutter
    if (isFeatureAvailable(FEATURE_SHUTTER)) {
      b = b && setShutter(par.shutterMode,par.shutter);
    }

    // gain
    if (isFeatureAvailable(FEATURE_GAIN)) {
      b = b && setGain(par.gainMode,par.gain);
    }

    // iris
    if (isFeatureAvailable(FEATURE_IRIS)) {
      b = b && setIris(par.irisMode,par.iris);
    }

    // focus
    if (isFeatureAvailable(FEATURE_FOCUS)) {
      b = b && setFocus(par.focusMode,par.focus);
    }

    // zoom
    if (isFeatureAvailable(FEATURE_ZOOM)) {
      b = b && setZoom(par.zoomMode,par.zoom);
    }

    // pan
    if (isFeatureAvailable(FEATURE_PAN)) {
      b = b && setPan(par.panMode,par.pan);
    }

    // tilt
    if (isFeatureAvailable(FEATURE_TILT)) {
      b = b && setTilt(par.tiltMode,par.tilt);
    }

    // optical filter
    if (isFeatureAvailable(FEATURE_OPTICAL_FILTER)) {
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

    unsigned int format = 0;
    unsigned int mode = 0;
    unsigned int framerate = convertFramerate(par.framerate);

    const cameraInfo& cam = cams_[activeCamera_];

    if (b) {
      // check if the mode in the parameters is supported.
      std::set<uint32>::const_iterator it;
      const std::set<uint32>& supportedModes = cam.supportedModes;

      mode = convertMode(par.resolution,par.encoding);
      it = supportedModes.find(mode);
      if (it == supportedModes.end()) {
        b=false;
        setStatusString("Selected mode not supported by this camera");
        return b;
      } else {
        // determine color mode and format
        ipoint res;
        convertMode(mode,res,colorMode_);
        format=getFormat(res);
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
      dc1394_dma_release_camera(*handle_,&camera_);
      captureSetUp_ = false;
    }
     
    passiveWait(Delay); // give some time for the camera to get ready
      
    _lti_debug("setup capture...\n");
    int drop = (par.dropFrames) ? 1 : 0;

    // the libdc1394 will take charge of the deviceFile string deallocation
    // so we need to create a copy of it using malloc (as the lib will use
    // free)
    char* devFile = 
      reinterpret_cast<char*>(malloc(par.deviceFile.length()+1));
    strcpy(devFile,par.deviceFile.c_str());

    if (dc1394_dma_setup_capture(*handle_,
                                 *node_,
                                 busChannel_,
                                 format,
                                 mode,
                                 busSpeed_,
                                 framerate,
                                 par.dmaBufferSize,
                                 drop,
                                 devFile,
                                 &camera_)==DC1394_SUCCESS) {
      captureSetUp_ = true;
    } else {
      _lti_debug("unable to setup capture\n");
      b = false;
    } 
      
    bool snapShotMode = (par.snapShotMode && snapShotCapable_);

    // if snapShotMode mode is not set turn on transmission
    if (!snapShotMode) {
      _lti_debug("start iso-transmission...\n");
      if (dc1394_start_iso_transmission(*handle_,*node_)
          !=DC1394_SUCCESS) {
        setStatusString("unable to start iso transmission\n");
        b = false;
      }

      if (dc1394_get_iso_status(*handle_,
                                *node_,
                                &cams_[activeCamera_].miscInfo.is_iso_on)
          != DC1394_SUCCESS) {
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
  bool fireWireDCAM::captureFrame() {
    const parameters& par = getParameters();

    // is there a camera handle?
    if (!captureSetUp_) {
      // there should be a status-string from updateParameters()
      return false;
    }

    if (par.snapShotMode && snapShotCapable_) {      
      _lti_debug4("set into one shot mode...\n");
      if (dc1394_set_one_shot(*handle_,*node_)!=DC1394_SUCCESS) {
        _lti_debug4("unable to enter one shot mode...\n");
        setStatusString("unable to enter one shot mode.\n");
        return false;
      }
    }

    _lti_debug4("last dma buffer: " << camera_.dma_last_buffer << " of " <<
               camera_.num_dma_buffers << "\n");

    _lti_debug4("dc1394_dma_single_capture(&camera_)...");

    // capture and wait
    if (dc1394_dma_single_capture(&camera_)!=DC1394_SUCCESS) {
      _lti_debug4("failed.\n");
      setStatusString("Unable to capture single frame. Check camera setup.\n");
      return false;
    }

    _lti_debug4(" ok.\n");

    return true;
  }

  
  // On place apply for type image!
  bool fireWireDCAM::apply(image& srcdest) {
    // capture frame
    if (!(captureFrame())) {
      // statusString is set in captureFrame();
      return false;
    }

    // determine how the conversion should be done
    switch (colorMode_) {
    case (YUV411) : {
      _lti_debug4("convert frame YUV411 -> RGB...\n");
      yuv411toRGB(srcdest);
    } break;
    case (YUV422) : {
      _lti_debug4("convert frame YUV422 -> RGB...\n");
      yuv422toRGB(srcdest);
    } break;
    case (Mono8) : {
      _lti_debug4("convert frame mono8 -> RGB...\n");
      const int rows = camera_.frame_height;
      const int cols = camera_.frame_width;
      ubyte* sPtr = reinterpret_cast<ubyte*>(camera_.capture_buffer);
      ch8_.useExternData(rows,cols,sPtr);
      srcdest.castFrom(ch8_);
    } break;
    case (Raw8) :  {
      _lti_debug4("bayer conversion of frame...\n");
      const int rows = camera_.frame_height;
      const int cols = camera_.frame_width;
      ubyte* sPtr = reinterpret_cast<ubyte*>(camera_.capture_buffer);
      ch8_.useExternData(rows,cols,sPtr);
      bayer_->apply(ch8_,srcdest);
    } break;
    default: 
      setStatusString("Unsupported encoding type");
      return false;
      break;
    }
    
    _lti_debug4("release buffer...\n");
    dc1394_dma_done_with_buffer(&camera_);
    
    return true;
  }

  bool fireWireDCAM::apply(channel8& dest) {
    // capture frame
    if (!(captureFrame())) {
      // statusString is set in captureFrame();
      return false;
    }

    // determine how the conversion should be done
    switch (colorMode_) {
    case (YUV411) : {
      _lti_debug4("convert frame YUV411 -> channel8...\n");
      yuv411toChannel8(dest);
    } break;
    case (YUV422) : {
      _lti_debug4("convert frame YUV422 -> channel8...\n");
      yuv422toChannel8(dest);
    } break;
    case (Mono8) : {
      _lti_debug4("convert mono8 frame -> channel8...\n");
      const int rows = camera_.frame_height;
      const int cols = camera_.frame_width;
      ubyte* sPtr = reinterpret_cast<ubyte*>(camera_.capture_buffer);
      ch8_.useExternData(rows,cols,sPtr);
      dest.copy(ch8_);
    } break;
    case (Raw8) :  {
      _lti_debug4("bayer conversion of frame...\n");
      const int rows = camera_.frame_height;
      const int cols = camera_.frame_width;
      ubyte* sPtr = reinterpret_cast<ubyte*>(camera_.capture_buffer);
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
    dc1394_dma_done_with_buffer(&camera_);
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
    if (!(captureFrame())) {
      // statusString is set in captureFrame();
      return false;
    } 

    // just resize and copy data
    _lti_debug4("copy frame...\n");
    int rows = camera_.frame_height;
    int cols = camera_.frame_width;
    dest.allocate(rows,cols);
    ubyte* ptr = reinterpret_cast<ubyte*>(dest.data());
    memcpy(ptr,camera_.capture_buffer,sizeof(ubyte)*rows*cols);


    // never forget to release the buffer ;-)
    _lti_debug4("release buffer...\n");
    dc1394_dma_done_with_buffer(&camera_);

    return true;
  }


  // simple conversion from yuv422 to rgb, NO interpolation is done
  void fireWireDCAM::yuv422toRGB(image& dest) const {
    int rows = camera_.frame_height;
    int cols = camera_.frame_width;
    dest.allocate(rows,cols);

    // 4 pixels are stored in 8 byte
    enum {
      elemSize = 8
    };

    // iterator for destination pixels
    image::iterator itd = dest.begin();

    ubyte* sPtr = reinterpret_cast<ubyte*>(camera_.capture_buffer);
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
  void fireWireDCAM::yuv422toChannel8(channel8& dest) const {
    int rows = camera_.frame_height;
    int cols = camera_.frame_width;
    dest.allocate(rows,cols);

    // 4 pixels are stored in 8 byte
    const int elemSize = 8;

    // iterator for destination pixels
    channel8::iterator itd = dest.begin();

    ubyte* sPtr = reinterpret_cast<ubyte*>(camera_.capture_buffer);
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
  void fireWireDCAM::yuv411toRGB(image& dest) const {
    int rows = camera_.frame_height;
    int cols = camera_.frame_width;
    dest.allocate(rows,cols);

    // 4 pixels are stored in 6 byte
    const int elemSize = 6;

    // iterator for destination pixels
    image::iterator itd = dest.begin();

    ubyte* sPtr = reinterpret_cast<ubyte*>(camera_.capture_buffer);
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
  void fireWireDCAM::yuv411toChannel8(channel8& dest) const {
    int rows = camera_.frame_height;
    int cols = camera_.frame_width;
    dest.allocate(rows,cols);

    // 4 pixels are stored in 6 byte
    const int elemSize = 6;

    // iterator for destination pixels
    channel8::iterator itd = dest.begin();

    ubyte* sPtr = reinterpret_cast<ubyte*>(camera_.capture_buffer);
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
    unsigned int mode = convertMode(par.resolution,par.encoding);
    par.framerate = computeClosestFrameRate(mode,par.framerate);

    _lti_debug(" framerate is: "<<par.framerate<<"\n");

    // brightness
    if (isFeatureReadable(FEATURE_BRIGHTNESS)) {
      b = b && getBrightness(par.brightnessMode,par.brightness);     
    } else {
      par.brightness = 0;
      par.brightnessMode = FeatureNA;
    }
    
    // exposure
    if (isFeatureReadable(FEATURE_EXPOSURE)) {
      b = b && getExposure(par.exposureMode,par.exposure);     
    } else {
      par.exposure = 0;
      par.exposureMode = FeatureNA;
    }

    // sharpness
    if (isFeatureReadable(FEATURE_SHARPNESS)) {
      b = b && getSharpness(par.sharpnessMode,par.sharpness);     
    } else {
      par.sharpness = 0;
      par.sharpnessMode = FeatureNA;
    }

    // white balance (redGain/blueGain)
    if (isFeatureReadable(FEATURE_WHITE_BALANCE)) {
      b = b && getWhiteBalance(par.whiteBalanceMode,par.blueGain,par.redGain);
    } else {
      par.redGain = 0;
      par.blueGain = 0;
      par.whiteBalanceMode = FeatureNA;
    }
    
    // hue
    if (isFeatureReadable(FEATURE_HUE)) {
      b = b && getHue(par.hueMode,par.hue);     
    } else {
      par.hue = 0;
      par.hueMode = FeatureNA;
    }

    // saturation
    if (isFeatureReadable(FEATURE_SATURATION)) {
      b = b && getSaturation(par.saturationMode,par.saturation);     
    } else {
      par.saturation = 0;
      par.saturationMode = FeatureNA;
    }

    // gamma
    if (isFeatureReadable(FEATURE_GAMMA)) {
      b = b && getGamma(par.gammaMode,par.gamma);     
    } else {
      par.gamma = 0;
      par.gammaMode = FeatureNA;
    }

    // shutter
    if (isFeatureReadable(FEATURE_SHUTTER)) {
      b = b && getShutter(par.shutterMode,par.shutter);     
    } else {
      par.shutter = 0;
      par.shutterMode = FeatureNA;
    }

    // gain
    if (isFeatureReadable(FEATURE_GAIN)) {
      b = b && getGain(par.gainMode,par.gain);     
    } else {
      par.gain = 0;
      par.gainMode = FeatureNA;
    }

    // iris
    if (isFeatureReadable(FEATURE_IRIS)) {
      b = b && getIris(par.irisMode,par.iris);     
    } else {
      par.iris = 0;
      par.irisMode = FeatureNA;
    }

    // focus
    if (isFeatureReadable(FEATURE_FOCUS)) {
      b = b && getFocus(par.focusMode,par.focus);     
    } else {
      par.focus = 0;
      par.focusMode = FeatureNA;
    }

    // zoom
    if (isFeatureReadable(FEATURE_ZOOM)) {
      b = b && getZoom(par.zoomMode,par.zoom);     
    } else {
      par.zoom = 0;
      par.zoomMode = FeatureNA;
    }

    // pan
    if (isFeatureReadable(FEATURE_PAN)) {
      b = b && getPan(par.panMode,par.pan);     
    } else {
      par.pan = 0;
      par.panMode = FeatureNA;
    }

    // tilt
    if (isFeatureReadable(FEATURE_TILT)) {
      b = b && getTilt(par.tiltMode,par.tilt);     
    } else {
      par.tilt = 0;
      par.tiltMode = FeatureNA;
    }

    // optical filter
    if (isFeatureReadable(FEATURE_OPTICAL_FILTER)) {
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
    assert((featureId>=FEATURE_MIN) && (featureId<=FEATURE_MAX));

    const dc1394_feature_info& fts=features_->feature[featureId-FEATURE_MIN];

    // we are assuming this is true.  If some time in the future it isn't
    // something has to shout outloud!!
    assert(fts.feature_id == featureId);

    return (fts.available==DC1394_TRUE);
  }
 

  // protected internal helper
  bool fireWireDCAM::isFeatureReadable(const unsigned int featureId) const {
    assert (notNull(features_));
    assert((featureId>=FEATURE_MIN) && (featureId<=FEATURE_MAX));

    const dc1394_feature_info& fts=features_->feature[featureId-FEATURE_MIN];

    // we are assuming this is true.  If some time in the future it isn't
    // something has to shout outloud!!
    assert(fts.feature_id == featureId);

    return ( (fts.available==DC1394_TRUE) && 
             (fts.readout_capable==DC1394_TRUE) );
  }
 
  unsigned int 
  fireWireDCAM::checkFeatureModes(const unsigned int featureId) const {
    assert (notNull(features_));
    assert((featureId>=FEATURE_MIN) && (featureId<=FEATURE_MAX));

    const dc1394_feature_info& fts=features_->feature[featureId-FEATURE_MIN];

    unsigned int result = 0;
    if (fts.available==DC1394_TRUE) {
      if (fts.on_off_capable == DC1394_TRUE) {
        result |= FeatureOff;
      }
      if (fts.auto_capable == DC1394_TRUE) {
        result |= FeatureAuto;
      }
      if (fts.manual_capable == DC1394_TRUE) {
        result |= FeatureManual;
      }
      if (fts.one_push == DC1394_TRUE) {
        result |= FeatureOnePush;
      }
      if (fts.absolute_capable == DC1394_TRUE) {
        result |= FeatureAbsolute;
      }
    }
    return result;
  }
  
  bool fireWireDCAM::checkFeatureValueRange(const unsigned int featureId,
                                            float& minValue,
                                            float& maxValue) const {
    assert (notNull(features_));
    assert((featureId>=FEATURE_MIN) && (featureId<=FEATURE_MAX));
    
    const dc1394_feature_info& fts=features_->feature[featureId-FEATURE_MIN];

    minValue = static_cast<float>(fts.min);
    maxValue = static_cast<float>(fts.max);
    
    return (fts.available==DC1394_TRUE);
  }

  // Get the camera feature state
  bool fireWireDCAM::getFeatureMode(const unsigned int featureId,
                                     eCamFeatureMode& state) const {
    assert (notNull(features_));
    assert((featureId>=FEATURE_MIN) && (featureId<=FEATURE_MAX));
    
    const dc1394_feature_info& fts=features_->feature[featureId-FEATURE_MIN];

    if (fts.available == DC1394_FALSE) {
      state = FeatureNA;
      return true;
    }

    if ( (fts.on_off_capable == DC1394_TRUE) &&
         (fts.is_on == DC1394_FALSE) ) {
      state = FeatureOff;
      return true;  // feature available and turned off! everything is ok
    }

    // if we got here, wether the feature is always on (not on_off_capable),
    // or it can be turned off, but it is anyway on!

    if ( (fts.auto_capable == DC1394_TRUE) &&
         (fts.auto_active == DC1394_TRUE) ) {
      state=FeatureAuto;
      return true;
    }

    if ( (fts.one_push == DC1394_TRUE) &&
         (fts.one_push_active == DC1394_TRUE) ) {
      state=FeatureOnePush;
      return true;
    }

    if ( (fts.absolute_capable == DC1394_TRUE) &&
         (fts.abs_control == DC1394_TRUE) ) {
      state=FeatureAbsolute;
      return true;
    }

    if (fts.manual_capable == DC1394_TRUE) {
      state=FeatureManual;
      return true;
    }

    state = FeatureNA;
    // if we get until here, then the camera is maybe not fully IIDC compliant
    return false;        // report an error anyway
  }


  // protected internal helper
  bool fireWireDCAM::isFeatureAuto(const unsigned int featureId) const {
    assert (notNull(features_));
    assert((featureId>=FEATURE_MIN) && (featureId<=FEATURE_MAX));

    dc1394bool_t isAuto;
    if (dc1394_is_feature_auto(*handle_,*node_,featureId,&isAuto) 
        != DC1394_SUCCESS) {
      // return false if an error occurs
      _lti_debug("unable to ask if feature " <<
                 dc1394_feature_desc[featureId-FEATURE_MIN]<<" is auto\n");
      
      return false;
    }
    return (isAuto==DC1394_TRUE);
  }

  // protected internal helper
  bool fireWireDCAM::setFeatureAuto(const unsigned int featureId, 
                                    const bool on) {
    assert (notNull(features_));
    assert((featureId>=FEATURE_MIN) && (featureId<=FEATURE_MAX));

    // we have to update some attributes here (following corianders code...)
    // so, we also need to get a read-writable reference to the feature's
    // data structure.
    dc1394_feature_info& fts =
      cams_[activeCamera_].featureSet.feature[featureId-FEATURE_MIN];

    const unsigned int val = on ? 1 : 0;

    if ( fts.auto_capable == DC1394_TRUE) {
      if (dc1394_auto_on_off(*handle_,*node_,featureId,val) != 
          DC1394_SUCCESS) {
        std::string str = "Auto setting for feature ";
        str += dc1394_feature_desc[featureId-FEATURE_MIN];
        str += " could not be turned ";
        str += (on?"on":"off");
        setStatusString(str.c_str());
        
        return false;
      } else {
        fts.auto_active = on ? DC1394_TRUE : DC1394_FALSE; // update the state
      }
    } else if (on) {
      // there is an error only if the user wants to turn the auto mode on
      // the system may accept manual, but no auto
      std::string str = "Feature ";
      str += dc1394_feature_desc[featureId-FEATURE_MIN];
      str += " does not support the auto state.";
      setStatusString(str.c_str());      
      return false;          
    }
    
    return true;
  }


  bool fireWireDCAM::setFeatureOn(const unsigned int featureId,
                                  const bool on) {
    assert (notNull(features_));
    assert((featureId>=FEATURE_MIN) && (featureId<=FEATURE_MAX));

    // we have to update some attributes here (following corianders code...)
    // so, we also need to get a read-writable reference to the feature's
    // data structure.
    dc1394_feature_info& fts =
      cams_[activeCamera_].featureSet.feature[featureId-FEATURE_MIN];

    const unsigned int val = on ? 1 : 0;

    if ( fts.on_off_capable == DC1394_TRUE) {
      if (dc1394_feature_on_off(*handle_,*node_,featureId,val) != 
          DC1394_SUCCESS) {
        std::string str = "Feature ";
        str += dc1394_feature_desc[featureId-FEATURE_MIN];
        str += " could not be turned ";
        str += (on?"on":"off");
        setStatusString(str.c_str());
        
        return false;
      } else {
        fts.is_on = on ? DC1394_TRUE : DC1394_FALSE; // update the state
      }
    } else {
      std::string str = "Feature ";
      str += dc1394_feature_desc[featureId-FEATURE_MIN];
      str += " does not support the on/off capability";
      setStatusString(str.c_str());
      
      return false;          
    }
    
    return true;
  }

  bool fireWireDCAM::setFeatureAbsoluteMode(const unsigned int featureId,
                                             const bool on) {
    assert (notNull(features_));
    assert((featureId>=FEATURE_MIN) && (featureId<=FEATURE_MAX));
    
    // we have to update some attributes here (following corianders code...)
    // so, we also need to get a read-writable reference to the feature's
    // data structure.
    dc1394_feature_info& fts =
      cams_[activeCamera_].featureSet.feature[featureId-FEATURE_MIN];

    const unsigned int val = on ? 1 : 0;

    if ( fts.absolute_capable == DC1394_TRUE) {
      if (dc1394_absolute_setting_on_off(*handle_,*node_,featureId,val) != 
          DC1394_SUCCESS) {
        std::string str = "Absolute setting for feature ";
        str += dc1394_feature_desc[featureId-FEATURE_MIN];
        str += " could not be ";
        str += (on?"activated":"deactivated");
        setStatusString(str.c_str());
        
        return false;
      } else {
        fts.abs_control = on ? DC1394_TRUE : DC1394_FALSE; // update the state
      }
    } else {
      std::string str = "Feature ";
      str += dc1394_feature_desc[featureId-FEATURE_MIN];
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
    assert((featureId>=FEATURE_MIN) && (featureId<=FEATURE_MAX));
    
    // read only reference to 
    const dc1394_feature_info& fts=features_->feature[featureId-FEATURE_MIN];

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

      case FeatureManual:
        if ( fts.manual_capable == DC1394_TRUE) {

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
          str += dc1394_feature_desc[featureId-FEATURE_MIN];
          str += " does not support manual settings.";
          setStatusString(str.c_str());
          
          return false;          
        }
        return true;  // feature available and turned off! everything is ok
        break;
      case FeatureOnePush:
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
        break;
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

    const cameraInfo& cam = cams_[activeCamera_];
    const dc1394_camerainfo& info = cam.info;
    
    cname += std::string(": ") + info.vendor + " " + info.model;

    return cname;
  }

  // print actual feature set for debug purpose
  bool fireWireDCAM::printFeatures() const {
    dc1394_print_feature_set(const_cast<dc1394_feature_set*>(features_));
    return true;
  }

  bool fireWireDCAM::init() {
    return initHardware();
  }

  bool fireWireDCAM::isInitialized() const {
    return hwCreated_;
  }

  // init camera
  bool fireWireDCAM::initHardware() {
    if (!hwCreated_) { // static attribute
      if (bus_.get()) {
        if (cams_.get(bus_)) {            
          hwCreated_=true;
        } else {
          setStatusString(cams_.errorStr.c_str());
        }
      } else {
        setStatusString("Could not access camera. " \
                        "Check if the camera is conected, " \
                        "if you have permissions on devices /dev/raw1394 and "\
                        "/dev/video1394, or check if the firewire kernel "\
                        "modules are loaded.");
      }
    } 

    return hwCreated_;
  }

  bool fireWireDCAM::initCamera(parameters& par) {

    if (!hwCreated_) {
      if (!initHardware()) {
        return false;
      }
    }

    bool error = false;

    // well, theoretically 'par' contains which camera we are looking for:
    if (!cameraNames_.use(par.cameraNamesFile)) {
      std::string msg = "Could not open or understand file " +
        par.cameraNamesFile;
      setStatusString(msg.c_str());
      return false;
    }
    u_int64_t euid64=0;

    // Flag to indicate if the cameraName is not being used and has to be
    // added to the database as soon as the first active camera has being
    // detected.
    bool needToAdd=false;
    needToAdd = !cameraNames_.query(par.cameraName,euid64);

    if (needToAdd) {
      if (!cams_.findFreeCamera(activeCamera_)) {
        setStatusString("Camera already in use!");
        return false;
      }

      if (!cameraNames_.add(par.cameraName,cams_[activeCamera_].info.euid_64)){
        // this should not happen ever
        setStatusString("Could not register camera name");
        return false;
      }

      cameraNames_.dump();
    } else {
      if (!cams_.findCamera(euid64,activeCamera_)) {
        // The camera given in the parameters is not working, but maybe the
        // user just wants to use any available camera.  We assume that an
        // AutoFix or an Upload mode implies this assumption.

        if ( (par.fixMode == AutoFix) || (par.fixMode == Upload) ) {

          if (!cams_.findFreeCamera(activeCamera_)) {
            std::string msg = "Camera " + par.cameraName + 
              " not connected or already in use.";
            setStatusString(msg);
            return false;
          }

          // two things can happen:
          // 1.  An active camera is for the very first time connected, so that
          //     is not even within the camera names file, or
          // 2.  The camera is already in that file, but the wrong name of the 
          //     parameters have been chosen.

          // First than all, we need to check which case we have!
          if (!cameraNames_.query(cams_[activeCamera_].info.euid_64,
                                  par.cameraName)) {
            
            // this is the first case
            
            // We need a new name for this camera. Build it from the
            // vendor, model and just to make sure that no two cameras get
            // the same name, with the index in the array
            
            int postfix = 0;
            std::string newName;
            u_int64_t dummyId;
            do {
              std::ostringstream oss;
              oss << cams_[activeCamera_].info.vendor << " " 
                  << cams_[activeCamera_].info.model;
              if (postfix > 0) {
                oss << " (" << postfix << ")";
              }
              
              newName = oss.str();
              postfix++;
            } while (cameraNames_.query(newName,dummyId)); // while name exists

            if (!cameraNames_.add(newName,
                                  cams_[activeCamera_].info.euid_64)) {
              // this should not happen ever
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
      return false;
    }    

    const cameraInfo& cam = cams_[activeCamera_];
    const dc1394_camerainfo& info = cam.info;
    const dc1394_miscinfo& miscInfo = cam.miscInfo;
        
    // initialize shadows/shortcuts
    handle_   = &info.handle;
    node_     = &info.id;
    features_ = &cam.featureSet;

    error = isNull(*handle_);
    
#ifdef _LTI_DEBUG
    // get camerainfo for debugging
    _lti_debug4(" Using " << info.model << 
                " (" << info.vendor << ") on Node " << node_ << ".\n");
#endif

    snapShotCapable_ = (miscInfo.one_shot_capable != DC1394_FALSE);

    // first turn camera on...
    if ((cam.basics & (0x1<<16)) != 0) { // check if power on/off is available
      if (dc1394_camera_on(*handle_,*node_) != DC1394_SUCCESS) {
        setStatusString("Could not turn camera on");
        return false;      
      }
    }

    if (dc1394_get_iso_channel_and_speed(*handle_, 
                                         *node_,
                                         &busChannel_,
                                         &busSpeed_) != DC1394_SUCCESS) {
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

    if (state == FeatureNA) {
      return true;
    }

    if (!isFeatureAvailable(featureId)) {
      std::string str = "Feature ";
      str += dc1394_feature_desc[featureId-FEATURE_MIN];
      str += " not supported.";
      setStatusString(str.c_str());
      return false;
    }

    if (setFeatureMode(featureId,state)) {
      if (state == FeatureManual) {
        if (dc1394_set_feature_value(*handle_,*node_,featureId,
                                  static_cast<unsigned int>(value))
            != DC1394_SUCCESS) {
          std::string str = "Could not set ";
          str += dc1394_feature_desc[featureId-FEATURE_MIN];
          str += " value";
          setStatusString(str.c_str());
          return false;
        }
      } else if (state == FeatureAbsolute) {
        if (dc1394_set_absolute_feature_value(*handle_,*node_,
                                              featureId,value)
            != DC1394_SUCCESS) {
          std::string str = "Could not set ";
          str += dc1394_feature_desc[featureId-FEATURE_MIN];
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
    
    const dc1394_feature_info& fts=features_->feature[featureId-FEATURE_MIN];
    
    if (fts.readout_capable == DC1394_FALSE) {
      std::string str = "Feature";
      str += dc1394_feature_desc[featureId-FEATURE_MIN];
      str += " is not read-out capable.";
      setStatusString(str.c_str());
      return false;
    }

    if (state != FeatureAbsolute) {
      unsigned int tmp;
      if (dc1394_get_feature_value(*handle_,*node_,featureId,&tmp)
          != DC1394_SUCCESS) {
        std::string str = "Value for feature ";
        str += dc1394_feature_desc[featureId-FEATURE_MIN];
        str += " could not be read.";
        setStatusString(str.c_str());
        return false;
      }
      value=static_cast<float>(tmp);
    } else {
      if (dc1394_query_absolute_feature_value(*handle_,*node_,featureId,&value)
          != DC1394_SUCCESS) {
        std::string str = "Absolute value for feature ";
        str += dc1394_feature_desc[featureId-FEATURE_MIN];
        str += " could not be read.";
        setStatusString(str.c_str());
        return false;
      }
    }
    
    return true;
  }

  bool fireWireDCAM::setBrightness(const eCamFeatureMode state,
                                   const float brightness) {

    if (setFeatureValue(FEATURE_BRIGHTNESS,state,brightness)) {
      parameters& par = getRWParameters();
      par.brightnessMode = state;
      par.brightness      = brightness;
      return true;
    }
        
    return false;
  }
  
  bool fireWireDCAM::getBrightness(eCamFeatureMode& state,
                                   float& brightness) const {
    return getFeatureValue(FEATURE_BRIGHTNESS,state,brightness);
  }
  
  bool fireWireDCAM::setExposure(const eCamFeatureMode state,
                                 const float exposure) {
    if (setFeatureValue(FEATURE_EXPOSURE,state,exposure)) {
      parameters& par = getRWParameters();
      par.exposureMode = state;
      par.exposure      = exposure;
      return true;
    }
        
    return false;
  }
  
  bool fireWireDCAM::getExposure(eCamFeatureMode& state,
                                 float& exposure) const {
    return getFeatureValue(FEATURE_EXPOSURE,state,exposure);
  }
  
  bool fireWireDCAM::setSharpness(const eCamFeatureMode state,
                                  const float sharpness) {
    if (setFeatureValue(FEATURE_SHARPNESS,state,sharpness)) {
      parameters& par = getRWParameters();
      par.sharpnessMode = state;
      par.sharpness      = sharpness;
      return true;
    }
        
    return false;
  }
  
  bool fireWireDCAM::getSharpness(eCamFeatureMode& state,
                                  float& sharpness) const {
    return getFeatureValue(FEATURE_SHARPNESS,state,sharpness);
  }

  bool fireWireDCAM::setWhiteBalance(const eCamFeatureMode state,
                                     const float ubGain,
                                     const float rvGain) {
    
    if (!isFeatureAvailable(FEATURE_WHITE_BALANCE)) {
      setStatusString("Feature white balance not supported");
      return false;
    }

    if (setFeatureMode(FEATURE_WHITE_BALANCE,state)) {
      if ((state == FeatureManual) || (state == FeatureAbsolute)) {
        if (dc1394_set_white_balance(*handle_,*node_,
                                     static_cast<unsigned int>(ubGain),
                                     static_cast<unsigned int>(rvGain))
            != DC1394_SUCCESS) {
          setStatusString("Could not set white balance B/U and R/V gains");
          return false;
        }
      } 

      parameters& par = getRWParameters();
      par.whiteBalanceMode = state;
      par.blueGain          = ubGain;
      par.redGain           = rvGain;
      
      return true;
    }
        
    // BUG? How to set the values in absolute mode?

    return false;

  }
  
  bool fireWireDCAM::getWhiteBalance(eCamFeatureMode& state,
                                     float& ubGain,
                                     float& vrGain) const {
    if (!getFeatureMode(FEATURE_WHITE_BALANCE,state)) {
      return false;
    }
    
    const dc1394_feature_info& fts =
      features_->feature[FEATURE_WHITE_BALANCE-FEATURE_MIN];
    
    if (fts.readout_capable == DC1394_FALSE) {
      setStatusString("Feature white balance is not read-out capable.");
      return false;
    }

    unsigned int bg,rg;
    if (dc1394_get_white_balance(*handle_,*node_,&bg,&rg)
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
    if (setFeatureValue(FEATURE_HUE,state,hue)) {
      parameters& par = getRWParameters();
      par.hueMode = state;
      par.hue      = hue;
      return true;
    }
        
    return false;
  }
  
  bool fireWireDCAM::getHue(eCamFeatureMode& state,
                            float& hue) const {
    return getFeatureValue(FEATURE_HUE,state,hue);
  }
  
  bool fireWireDCAM::setSaturation(const eCamFeatureMode state,
                                   const float saturation) {
    if (setFeatureValue(FEATURE_SATURATION,state,saturation)) {
      parameters& par = getRWParameters();
      par.saturationMode = state;
      par.saturation      = saturation;
      return true;
    }
        
    return false;
  }
  
  bool fireWireDCAM::getSaturation(eCamFeatureMode& state,
                                   float& saturation) const {
    
    return getFeatureValue(FEATURE_SATURATION,state,saturation);
  }
  
  bool fireWireDCAM::setGamma(const eCamFeatureMode state,
                              const float gamma) {
    if (setFeatureValue(FEATURE_GAMMA,state,gamma)) {
      parameters& par = getRWParameters();
      par.gammaMode = state;
      par.gamma      = gamma;
      return true;
    }
        
    return false;
  }

  bool fireWireDCAM::getGamma(eCamFeatureMode& state,
                              float& gamma) const {
    return getFeatureValue(FEATURE_GAMMA,state,gamma);
  }

  bool fireWireDCAM::setShutter(const eCamFeatureMode state,
                                const float shutter) {
    if (setFeatureValue(FEATURE_SHUTTER,state,shutter)) {
      parameters& par = getRWParameters();
      par.shutterMode = state;
      par.shutter      = shutter;
      return true;
    }
        
    return false;
  }

  bool fireWireDCAM::getShutter(eCamFeatureMode& state,
                                float& shutter) const {
    return getFeatureValue(FEATURE_SHUTTER,state,shutter);
  }

  bool fireWireDCAM::setGain(const eCamFeatureMode state,
                             const float gain) {
    if (setFeatureValue(FEATURE_GAIN,state,gain)) {
      parameters& par = getRWParameters();
      par.gainMode = state;
      par.gain      = gain;
      return true;
    }
        
    return false;
  }

  bool fireWireDCAM::getGain(eCamFeatureMode& state,
                             float& gain) const {
    return getFeatureValue(FEATURE_GAIN,state,gain);
  }

  bool fireWireDCAM::setIris(const eCamFeatureMode state,
                             const float iris) {
     if (setFeatureValue(FEATURE_IRIS,state,iris)) {
      parameters& par = getRWParameters();
      par.irisMode = state;
      par.iris      = iris;
      return true;
    }
        
    return false;
 }

  bool fireWireDCAM::getIris(eCamFeatureMode& state,
                             float& iris) const {
    return getFeatureValue(FEATURE_IRIS,state,iris);
  }

  bool fireWireDCAM::setFocus(const eCamFeatureMode state,
                              const float focus) {
    if (setFeatureValue(FEATURE_FOCUS,state,focus)) {
      parameters& par = getRWParameters();
      par.focusMode = state;
      par.focus      = focus;
      return true;
    }
        
    return false;
  }

  bool fireWireDCAM::getFocus(eCamFeatureMode& state,
                              float& focus) const {
    return getFeatureValue(FEATURE_FOCUS,state,focus);
  }

  bool fireWireDCAM::setZoom(const eCamFeatureMode state,
                             const float zoom) {
    if (setFeatureValue(FEATURE_ZOOM,state,zoom)) {
      parameters& par = getRWParameters();
      par.zoomMode = state;
      par.zoom      = zoom;
      return true;
    }
        
    return false;
  }

  bool fireWireDCAM::getZoom(eCamFeatureMode& state,
                             float& zoom) const {
    return getFeatureValue(FEATURE_ZOOM,state,zoom);
  }

  bool fireWireDCAM::setPan(const eCamFeatureMode state,
                            const float pan) {
    if (setFeatureValue(FEATURE_PAN,state,pan)) {
      parameters& par = getRWParameters();
      par.panMode = state;
      par.pan      = pan;
      return true;
    }
        
    return false;
  }

  bool fireWireDCAM::getPan(eCamFeatureMode& state,
                            float& pan) const {
    return getFeatureValue(FEATURE_PAN,state,pan);
  }

  bool fireWireDCAM::setTilt(const eCamFeatureMode state,
                             const float tilt) {
     if (setFeatureValue(FEATURE_TILT,state,tilt)) {
      parameters& par = getRWParameters();
      par.tiltMode = state;
      par.tilt     = tilt;
      return true;
    }
        
    return false;
  }

  bool fireWireDCAM::getTilt(eCamFeatureMode& state,
                             float& tilt) const {
    return getFeatureValue(FEATURE_TILT,state,tilt);
  }
  
  bool fireWireDCAM::setPanTilt(const eCamFeatureMode mode,
                                const float pan,
                                const float tilt) {
    if (setFeatureValue(FEATURE_PAN,mode,pan) &&
        setFeatureValue(FEATURE_TILT,mode,tilt)) {
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
    return (getFeatureValue(FEATURE_PAN,panMode,pan) && 
            getFeatureValue(FEATURE_TILT,tiltMode,tilt));
            
  }
  
  bool fireWireDCAM::panTiltReset() {
    return true; // until now, we don't know how to reset the pan-tilt
                 // unit of a FireWire cam!
  }

  bool fireWireDCAM::setOpticalFilter(const eCamFeatureMode state,
                                      const float opticalFilter) {
    if (setFeatureValue(FEATURE_OPTICAL_FILTER,state,opticalFilter)) {
      parameters& par = getRWParameters();
      par.opticalFilterMode = state;
      par.opticalFilter     = opticalFilter;
      return true;
    }
        
    return false;
  }

  bool fireWireDCAM::getOpticalFilter(eCamFeatureMode& state,
                                      float& opticalFilter) const {
    return getFeatureValue(FEATURE_OPTICAL_FILTER,state,opticalFilter);
  }
  
  // -----------------------------------------------------------------------
  // End of methods for direct access to camera
  // -----------------------------------------------------------------------

  bool fireWireDCAM::getFramerate(float& rate) const {
    unsigned int irate;
    bool b = true;
    b = b && (dc1394_get_video_framerate(*handle_,*node_,&irate)
              ==DC1394_SUCCESS);
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
    parameters& par = getRWParameters();

    unsigned int format = 0;
    unsigned int mode = 0;
    unsigned int fr = convertFramerate(framerate);

    cameraInfo& cam = cams_[activeCamera_];

    // TODO: Do a more exhaustive check on the permitted resolution/framework
    // since the camera may allow a given pair but only for black/white images
    // before setting anything, a verification has to be done (see coriander)

    // check if the mode in the parameters is supported.
    std::set<uint32>::const_iterator it;
    const std::set<uint32>& supportedModes =
      cams_[activeCamera_].supportedModes;
    
    mode = convertMode(resolution,par.encoding);
    it = supportedModes.find(mode);
    if (it == supportedModes.end()) {
      setStatusString("Selected mode not supported by this camera");
      return false;
    } 
      
    format = getFormat(resolution);

    // stop transmission
    isoFlowStop();

    if (dc1394_set_video_format(cam.info.handle,
                                cam.info.id,
                                format) != DC1394_SUCCESS) {
      setStatusString("Could not set video format");
      return false;
    } else {
      cam.miscInfo.format = format;
    }
    
    if (dc1394_set_video_mode(cam.info.handle,
                              cam.info.id,
                              mode) != DC1394_SUCCESS) {
      setStatusString("Could not set video mode");
      return false;
    } else {
      cam.miscInfo.mode = mode;
    }

    // check consistency of framerate
    if (format!=FORMAT_SCALABLE_IMAGE_SIZE) {
      quadlet_t value=0;
      _lti_debug2("Querying supported frames...");

      if (dc1394_query_supported_framerates(cam.info.handle,
                                            cam.info.id,
                                            format,
                                            mode,
                                            &value)!=DC1394_SUCCESS) {
        _lti_debug2("failed\n");

        setStatusString("Could not read supported framerates");
        return false;
      } else {
        int newFramerate=-1;
        int current=fr-FRAMERATE_MIN;

        if ((value & (uint32(0x1)<<(31-current)))==0) {
          _lti_debug2("need change.\n");
          // the current framerate is not OK for the new
          // mode/format. Switch to nearest framerate
          
          bool cont = true;

          for (int i=0;i<=((NUM_FRAMERATES>>1)+1);i++) {
            // search radius is num_framerates/2 +1 for safe rounding
            if ( (value & (uint32(0x1)<<(31-(current+i)))) && cont) {
              newFramerate=current+i+FRAMERATE_MIN;
              cont=false;
            }
            if ( (value & (uint32(0x1)<<(31-(current-i)))) && cont) {
              newFramerate=current-i+FRAMERATE_MIN;
              cont=false;
            }
          }
        } else {
          _lti_debug2("ok.\n");

          // current, i.e. fr is supported as is
          newFramerate=fr;
        }

        _lti_debug2("Changing framerate to " << 
                    convertFramerate(uint32(newFramerate)));
        
        if (dc1394_set_video_framerate(cam.info.handle,
                                       cam.info.id,
                                       newFramerate)!=DC1394_SUCCESS) {
          setStatusString("Cannot set video framerate");
          return false;
        }
        else {
          cam.miscInfo.framerate=newFramerate;
          fr = newFramerate;
          par.framerate=convertFramerate(fr);
        }
      }
    
    } else {
      par.framerate = framerate;
    }
        
    // Get updated information from the camera
    if (dc1394_get_camera_info(cam.info.handle,
                               cam.info.id, 
                               &cam.info) != DC1394_SUCCESS) {
      setStatusString("Could not get basic information from the camera!");
      return false;
    }

    if (dc1394_get_camera_feature_set(cam.info.handle,
                                      cam.info.id,
                                      &cam.featureSet) 
        != DC1394_SUCCESS) {
      setStatusString("Could not get camera feature information!");
      return false;
    }

    // the libdc1394 will take charge of the deviceFile string deallocation
    // so we need to create a copy of it using malloc (as the lib will use
    // free)
    char* devFile = 
      reinterpret_cast<char*>(malloc(par.deviceFile.length()+1));
    strcpy(devFile,par.deviceFile.c_str());
    int drop = (par.dropFrames) ? 1 : 0;

    if (captureSetUp_) {
      _lti_debug4("release old camera structure...\n");

      // remember: even if this is called release "camera", it just
      // releases the "capture" buffers and related stuff
      dc1394_dma_release_camera(*handle_,&camera_);
      captureSetUp_ = false;
    }

    if (dc1394_dma_setup_capture(cam.info.handle,
                                 cam.info.id,
                                 busChannel_,
                                 format,
                                 mode,
                                 busSpeed_,
                                 fr,
                                 par.dmaBufferSize,
                                 drop,
                                 devFile,
                                 &camera_)==DC1394_SUCCESS) {
      captureSetUp_ = true;
    } else {
      _lti_debug("unable to setup capture\n");
      return false;
    } 

    // we have to resume now, before we change the miscInfo data structure
    isoFlowResume();
    
    if (dc1394_get_camera_misc_info(cam.info.handle,
                                    cam.info.id,
                                    &cam.miscInfo) != DC1394_SUCCESS) {
      setStatusString("Could not get misc information from the camera!");
      return false;
    }

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
    
    framerates.clear();
    resols.clear();
    val.clear();

    std::set<float> theFramerates;
    std::set<ipoint> theResols;

    const cameraInfo& info = cams_[activeCamera_];
    // get through the supportedFramerates map, which also has the supported
    // modes
    std::map<uint32,std::set<float> >::const_iterator mit;
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


  unsigned int fireWireDCAM::convertFramerate(const float fps) const {
    static const float framerates[] = {1.875f,
                                       3.75f,
                                       7.5f,
                                       15.f,
                                       30.f,
                                       60.f,
                                       120.f,
                                       240.f};

    static const unsigned int frSize = 8;
    float min,tmp; 
    unsigned int minIdx,i;

    min=abs(fps-framerates[0]);
    minIdx=0;

    for (i=1;i<frSize;++i) {
      tmp = abs(fps-framerates[i]);
      if (tmp<min) {
        min=tmp;
        minIdx=i;
      } else {
        break; // since framerates[] is sorted is unnecessary to continue
      }
    }
    
    return (FRAMERATE_MIN+minIdx);
  }

  float fireWireDCAM::convertFramerate(const unsigned int eFR) const {
    switch(eFR) {
      case FRAMERATE_1_875:
        return 1.875f;
        break;
      case FRAMERATE_3_75:
        return 3.75f;
        break;
      case FRAMERATE_7_5:
        return 7.5f;
        break;
      case FRAMERATE_15:
        return 15.f;
        break;
      case FRAMERATE_30:
        return 30.f;
        break;
      case FRAMERATE_60:
        return 60.f;
        break;
      case FRAMERATE_120:
        return 120.f;
        break;
      case FRAMERATE_240:
        return 240.f;
        break;
      default:
        return 0.0f;
    }
    return 0.0f;
  }

  fireWireDCAM::eColorMode 
  fireWireDCAM::getColorCode7(const unsigned int code) const {
    switch (code) {
      case COLOR_FORMAT7_MONO8:
        return Mono8;
        break;
      case COLOR_FORMAT7_YUV411:
        return YUV411;
        break;
      case COLOR_FORMAT7_YUV422:
        return YUV422;
        break;
      case COLOR_FORMAT7_YUV444:
        return YUV444;
        break;
      case COLOR_FORMAT7_RGB8:
        return RGB8;
        break;
      case COLOR_FORMAT7_MONO16:
        return Mono16;
        break;
      case COLOR_FORMAT7_RGB16:
        return RGB16;
        break;
      case COLOR_FORMAT7_MONO16S:
        return Mono16s;
        break;
      case COLOR_FORMAT7_RGB16S:
        return RGB16s;
        break;
      case COLOR_FORMAT7_RAW8:
        return Raw8;
        break;
      case COLOR_FORMAT7_RAW16:
        return Raw16;
        break;
      default:
        return UnknownEncoding;
    }
    return UnknownEncoding;
  }

  bool fireWireDCAM::getVideoMode(ipoint& resolution,
                                  eColorMode& encoding) const {
    unsigned int imode;
    bool b = true;
    b = b && (dc1394_get_video_mode(*handle_,*node_,&imode)
              ==DC1394_SUCCESS);
    
    // some times the camera reports a mode it does not support!
    imode = computeClosestMode(imode);

    if (b) {
      if (imode <= MODE_FORMAT2_MAX) {
        // same enumeration values
        convertMode(imode,resolution,encoding);
      } else if (imode >= MODE_FORMAT7_MIN) {
        unsigned int x(0),y(0);
        b = b && (dc1394_query_format7_image_size(*handle_,*node_,
                                                  imode, &x, &y)
                  ==DC1394_SUCCESS);
        resolution.set(static_cast<int>(x),static_cast<int>(y));
        b = b && (dc1394_query_format7_color_coding_id(*handle_,*node_,
                                                       imode,&x)
                  ==DC1394_SUCCESS);
        encoding = getColorCode7(x);
      } else {
        // unsupported mode (or TODO yet) 
        resolution.set(0,0);
        encoding = Mono8;
      }
    }
    return b;
  }

  unsigned int fireWireDCAM::convertMode(const ipoint& resolution,
                                         const eColorMode& encoding) const {
    static const int resols[] = {(160*120),
                                 (320*240),
                                 (640*480),
                                 (800*600),
                                 (1024*768),
                                 (1280*960),
                                 (1600*1200)};
    
    int i,idx,d,tmp;
    idx=0;
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
      
      const format7Info& f7i = cams_[activeCamera_].format7;
      i=0;
      // find first format7 mode available
      while((i<NUM_MODE_FORMAT7) && 
            !f7i.mode[i].present &&
            (encoding != getColorCode7(f7i.mode[i].colorCodingId))) {
        ++i;
      }
      if (i<NUM_MODE_FORMAT7) {
        // we get here, if there is at least one format7 mode present!
        // otherwise, we will get below the most similar non-format7
        // mode.

        d=(f7i.mode[i].maxSize.x*f7i.mode[i].maxSize.y)-np;
        idx=i;
        ++i;
        while(i<NUM_MODE_FORMAT7) {
          tmp = (f7i.mode[i].maxSize.x*f7i.mode[i].maxSize.y) - np;
          if (f7i.mode[i].present && (tmp>0) && ((d<0) || (tmp<d))) {
            d = tmp;
            idx = i;
          }
          ++i;
        }

        // idx contains the index of the only present format7 mode, or
        // the index of the mode which is able to contain the desired
        // resolution the closest it gets.
        return MODE_FORMAT7_MIN+idx;
        return true;
      }
    }


    switch(idx) {
      case 0: // 160x120
        return MODE_160x120_YUV444;  // this is the only availabe mode for this
                                     // resolution
        break;
      case 1: // 320x240
        return MODE_320x240_YUV422;  // this is the only availabe mode for this
                                     // resolution
        break;
      case 2: // 640x480
        switch(encoding) {
          case Mono8:
            return MODE_640x480_MONO;
            break;
          case Mono16:
          case Mono16s:
            return MODE_640x480_MONO16;
            break;
          case YUV411:
            return MODE_640x480_YUV411;
            break;
          case YUV422:
            return MODE_640x480_YUV422;
            break;
          case YUV444:
            return MODE_640x480_YUV422; // nearest?
            break;
          case RGB8:
            return MODE_640x480_RGB;
            break;
          case RGB16:
          case RGB16s:
            return MODE_640x480_RGB; // nearest?
          default:
            return 0;
        }
        break;
      case 3: // 800x600
        switch(encoding) {
          case Mono8:
            return MODE_800x600_MONO;
            break;
          case Mono16:
          case Mono16s:
            return MODE_800x600_MONO16;
            break;
          case YUV411:
            return MODE_800x600_YUV422; // nearest?
            break;
          case YUV422:
            return MODE_800x600_YUV422;
            break;
          case YUV444:
            return MODE_800x600_YUV422; // nearest?
            break;
          case RGB8:
            return MODE_800x600_RGB;
            break;
          case RGB16:
          case RGB16s:
            return MODE_800x600_RGB; // nearest?
          default:
            return 0;
        }
        break;
      case 4: // 1024x768
        switch(encoding) {
          case Mono8:
            return MODE_1024x768_MONO;
            break;
          case Mono16:
          case Mono16s:
            return MODE_1024x768_MONO16;
            break;
          case YUV411:
            return MODE_1024x768_YUV422; // nearest?
            break;
          case YUV422:
            return MODE_1024x768_YUV422;
            break;
          case YUV444:
            return MODE_1024x768_YUV422; // nearest?
            break;
          case RGB8:
            return MODE_1024x768_RGB;
            break;
          case RGB16:
          case RGB16s:
            return MODE_1024x768_RGB; // nearest?
          default:
            return 0;
        }
        break;
      case 5: // 1280x960
         switch(encoding) {
          case Mono8:
            return MODE_1280x960_MONO;
            break;
          case Mono16:
          case Mono16s:
            return MODE_1280x960_MONO16;
            break;
          case YUV411:
            return MODE_1280x960_YUV422; // nearest?
            break;
          case YUV422:
            return MODE_1280x960_YUV422;
            break;
          case YUV444:
            return MODE_1280x960_YUV422; // nearest?
            break;
          case RGB8:
            return MODE_1280x960_RGB;
            break;
          case RGB16:
          case RGB16s:
            return MODE_1280x960_RGB; // nearest?
          default:
            return 0;
        }
       break;
      case 6: // 1600x1200
         switch(encoding) {
          case Mono8:
            return MODE_1600x1200_MONO;
            break;
          case Mono16:
          case Mono16s:
            return MODE_1600x1200_MONO16;
            break;
          case YUV411:
            return MODE_1600x1200_YUV422; // nearest?
            break;
          case YUV422:
            return MODE_1600x1200_YUV422;
            break;
          case YUV444:
            return MODE_1600x1200_YUV422; // nearest?
            break;
          case RGB8:
            return MODE_1600x1200_RGB;
            break;
          case RGB16:
          case RGB16s:
            return MODE_1600x1200_RGB; // nearest?
            break;
          default:
            return 0;
        }
       break;
      default:
        return 0; // error, we cannot get here!
    }
    
  }
  
  void fireWireDCAM::convertMode(const unsigned int value,
                                 ipoint& resolution,
                                 eColorMode& encoding) const {
    switch(value) {
      case MODE_160x120_YUV444:
        resolution.set(160,120);
        encoding = YUV444;
        break;
      case MODE_320x240_YUV422:
        resolution.set(320,240);
        encoding = YUV422;
        break;
      case MODE_640x480_YUV411:
        resolution.set(640,480);
        encoding = YUV411;
        break;
      case MODE_640x480_YUV422:
        resolution.set(640,480);
        encoding = YUV422;
        break;
      case MODE_640x480_RGB:
        resolution.set(640,480);
        encoding = RGB8;
        break;
      case MODE_640x480_MONO:
        resolution.set(640,480);
        encoding = Mono8;
        break;
      case MODE_640x480_MONO16:
        resolution.set(640,480);
        encoding = Mono16;
        break;
      case MODE_800x600_YUV422:
        resolution.set(800,600);
        encoding = YUV422;
        break;
      case MODE_800x600_RGB:
        resolution.set(800,600);
        encoding = RGB8;
        break;
      case MODE_800x600_MONO:
        resolution.set(800,600);
        encoding = Mono8;
        break;
      case MODE_1024x768_YUV422:
        resolution.set(1024,768);
        encoding = YUV422;
        break;
      case MODE_1024x768_RGB:
        resolution.set(1024,768);
        encoding = RGB8;
        break;
      case MODE_1024x768_MONO:
        resolution.set(1024,768);
        encoding = Mono8;
        break;
      case MODE_800x600_MONO16:
        resolution.set(800,600);
        encoding = Mono16;
        break;
      case MODE_1024x768_MONO16:
        resolution.set(1024,768);
        encoding = Mono16;
        break;
      case MODE_1280x960_YUV422:
        resolution.set(1280,960);
        encoding = YUV422;
        break;
      case MODE_1280x960_RGB:
        resolution.set(1280,960);
        encoding = RGB8;
        break;
      case MODE_1280x960_MONO:
        resolution.set(1280,960);
        encoding = Mono8;
        break;
      case MODE_1600x1200_YUV422:
        resolution.set(1600,1200);
        encoding = YUV422;
        break;
      case MODE_1600x1200_RGB:
        resolution.set(1600,1200);
        encoding = RGB8;
        break;
      case MODE_1600x1200_MONO:
        resolution.set(1600,1200);
        encoding = Mono8;
        break;
      case MODE_1280x960_MONO16:
        resolution.set(1280,960);
        encoding = Mono16;
        break;
      case MODE_1600x1200_MONO16:
        resolution.set(1600,1200);
        encoding = Mono16;
        break;
      case MODE_EXIF:
        // dummy values, as this mode maybe does not even exist in real cams.
        resolution.set(0,0);
        encoding = Mono8;
        break;
      case MODE_FORMAT7_0:
      case MODE_FORMAT7_1:
      case MODE_FORMAT7_2:
      case MODE_FORMAT7_3:
      case MODE_FORMAT7_4:
      case MODE_FORMAT7_5:
      case MODE_FORMAT7_6:
      case MODE_FORMAT7_7:
        convertMode7(value,resolution,encoding);
        break;
      default:
        resolution.set(0,0);
        encoding = Mono8;
        break;
    }
  }

  void fireWireDCAM::convertMode7(const unsigned int value,
                                  ipoint& resolution,
                                  eColorMode& encoding) const {

    if (hasCameraFormat7()) {
      if ((value>=MODE_FORMAT7_MIN) && (value<=MODE_FORMAT7_MAX)) {
        const format7ModeInfo& f7m = 
          cams_[activeCamera_].format7.mode[value-MODE_FORMAT7_MIN];
        
        if (f7m.present) {
          resolution.castFrom(f7m.maxSize);
          encoding = getColorCode7(f7m.colorCodingId);
          return;
        }
      }
      // else -> not format7
    }

    // unsupported format
    resolution.set(0,0);
    encoding = Mono8;
  }

  unsigned int fireWireDCAM::getFormat(const ipoint& resolution) const {
    const int d = resolution.x*resolution.y;
    if ((d==160*120) ||
        (d==320*240) ||
        (d==640*480)) {
      return FORMAT_VGA_NONCOMPRESSED;
    } else if ((d==800*600) ||
               (d==1024*768)) {
      return FORMAT_SVGA_NONCOMPRESSED_1;
    } else if ((d==1280*960) ||
               (d==1600*1200)) {
      return FORMAT_SVGA_NONCOMPRESSED_2;
    } 
    return FORMAT_SCALABLE_IMAGE_SIZE;
  }


  // ------------------------------------------------------------------------
  // One-Push scheduler
  // ------------------------------------------------------------------------
  bool fireWireDCAM::onePushQueueInsert(const unsigned int featureId) {
    // read only reference to 
    const dc1394_feature_info& fts=features_->feature[featureId-FEATURE_MIN];

    if (fts.one_push == DC1394_TRUE) {
      // one push supported for that feature
      if (dc1394_start_one_push_operation(*handle_,*node_,featureId) 
          != DC1394_SUCCESS) {
        std::string str = "One-push for feature ";
        str += dc1394_feature_desc[featureId-FEATURE_MIN];
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
        const unsigned int featureId = *it;
        dc1394bool_t value;
        
        if (dc1394_is_one_push_in_operation(*handle_,*node_,featureId,&value)
            ==DC1394_SUCCESS) {
          if (value == DC1394_FALSE) {
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
    cameraInfo& cam = cams_[activeCamera_];
    if (dc1394_get_iso_status(cam.info.handle, 
                              cam.info.id,
                              &cam.miscInfo.is_iso_on)!=DC1394_SUCCESS) {
      setStatusString("Could not get iso status");
      return false;
    }
    else {
      if (cam.miscInfo.is_iso_on != DC1394_FALSE) {
        if (dc1394_stop_iso_transmission(cam.info.handle, 
                                         cam.info.id) != DC1394_SUCCESS) {
          // ... (if not done, restarting is no more possible)
          setStatusString("Could not stop ISO transmission");
          return false;
        }
      }
    }
    return true;
  }

  bool fireWireDCAM::isoFlowStopTotal() {
    if (isoFlowStop()) {
      // set the proper flag off
      cams_[activeCamera_].miscInfo.is_iso_on = DC1394_FALSE;
    } else {
      return false;
    }
    return true;
  }

  bool fireWireDCAM::isoFlowResume() {

    cameraInfo& cam = cams_[activeCamera_];

    if (cam.miscInfo.is_iso_on != DC1394_FALSE) {
      // restart iff it was 'on' before 

      passiveWait(Delay); // necessary to avoid desynchronized ISO flow.
      if (dc1394_start_iso_transmission(cam.info.handle, 
                                        cam.info.id) != DC1394_SUCCESS) {
        setStatusString("Could not start ISO transmission");
        return false;
      }

      if (dc1394_get_iso_status(cam.info.handle,
                                cam.info.id,
                                &cam.miscInfo.is_iso_on) != DC1394_SUCCESS) {
        setStatusString("Could not get ISO status");
        return false;
      } else {

        int timeout=0;

        // ISO could not be restarted. Trying again for 5 seconds
        while ( (cam.miscInfo.is_iso_on == DC1394_FALSE) &&
                (timeout<5000) ) {
          passiveWait(Delay);
          timeout+=Delay/1000;
          if (dc1394_start_iso_transmission(cam.info.handle,
                                            cam.info.id) != DC1394_SUCCESS) {
            // ... (if not done, restarting is no more possible)
            setStatusString("Could not start ISO transmission");
            return false;
          }
          else {
            if (dc1394_get_iso_status(cam.info.handle,
                                      cam.info.id,
                                      &cam.miscInfo.is_iso_on) !=
                DC1394_SUCCESS) {
              setStatusString("Could not get ISO status");
              return false;
            }
          }
        }

	if (cam.miscInfo.is_iso_on == DC1394_FALSE) {
	  setStatusString("Can't start ISO, giving up...");
          return false;
        }
      }
    }

    return true;
  }
  
}
    

#include "ltiUndebug.h"

#endif //USE_FIRE_WIRE_DCAM
