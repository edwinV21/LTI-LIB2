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
 * \file   ltiV4l2.h
 *         Contains the class lti::v4l2, which provide access to webcams and
 *         other devices that support the Video4Linux2 API.
 * \author Pablo Alvarado
 * \date   05.10.2008
 *
 * revisions ..: $Id: ltiV4l2.h,v 1.5 2013-05-04 22:56:58 alvarado Exp $
 */

#ifndef _LTI_V4L2_H_
#define _LTI_V4L2_H_

#include "ltiHardwareConfig.h"

#ifdef _USE_V4L2
#if __linux__

#include "ltiFrameGrabber.h"
#include "ltiCamera.h"
#include "ltiPanTiltUnit.h"
#include "ltiLensUnit.h"
#include "ltiInterval.h"

#include <sys/types.h>
#include <linux/types.h>
#include <asm/types.h>

// C-stuff required for the device control

// these types are required by <linux/videodev.h> but some
// macro jungle may avoid their definition, so just use some
// configure trick to check if they can be obtained from <linux/types.h>
// or otherwise define them here
#if !defined (HAVE_U64_V4L)
typedef unsigned long ulong;
typedef lti::uint64 __u64;
typedef lti::int64 __s64;
#endif

#include <linux/videodev2.h>
#include "ltiV4l2_patch.h"   // define some missing symbols for old videodev2


#include <vector>
#include <map>

#undef None

namespace lti {

  /**
   * Capture Images from a Video-for-Linux-2 device.
   *
   * This class allows capturing images from webcams and other devices which
   * kernel modules follow the Video4Linux2 API.  Hence, this class, is only
   * available for Linux.
   *
   * You can access information on formats and constants used here in the
   * documentation of the V4L2 at 
   * 
   * http://v4l2spec.bytesex.org/spec/book1.htm
   *
   * Additionally, the header file from which all constants are taken can be
   * found in /usr/include/linux/videodev2.h
   *
   * Some web-cams may be supported by inherited classes to provide access to
   * features of the particular devices.
   *
   * @ingroup gAcquisition
   */
  class v4l2 : public frameGrabber,
               public camera,
               public panTiltUnit,
               public lensUnit {

  public:
    /**
     * Some additional features in the V4L2 API
     */
    enum {
      FeatureVerticalFlip = 500,      /**< Vertical Flip             */
      FeatureHorizontalFlip,          /**< Horizontal Flip           */
      FeaturePowerLineFilter,         /**< Power Line Filter         */
      FeatureWhiteBalanceTemperature, /**< White Balance Temperature */
      FeatureBacklightCompensation    /**< Backlight compensation    */
    };


    /**
     * Video4Linux2 pixel format
     *
     * Some devices support different pixel formats.  Since in the LTI-Lib
     * the interface with the user will be through the lti::image directly, 
     * then the user doesn't really care about the internal used format.
     * Hence, this class will try to guess the fastest way to get the 
     * images without much format conversions to the lti::rgbaPixel which is
     * just the RGBA format.
     *
     * This enumeration is provided for completeness.
     */
    enum ePixelFormat {
      /**
       * Raw format.
       *
       * This is not a format, but an access door to the internal
       * raw data stream provided by the drivers.  It is is mainly used for
       * debugging conversion routines.
       */
      Raw = -3,

      /**
       * Invalid format value
       */
      Invalid = -2,

      /**
       * Automatic selection.
       *
       * This value indicates to find automatically a valid pixel format for
       * the camera.
       */
      Auto = -1,

      /**
       * RGB-3-3-2 (8 bits per pixel)
       */
      RGB332  = V4L2_PIX_FMT_RGB332,
      /**
       * xxxxrrrr ggggbbbb  (16 bits per pixel)
       */
      RGB444  = V4L2_PIX_FMT_RGB444,
      /**
       * RGB-5-5-5      (16 bits per pixel)
       */
      RGB555  = V4L2_PIX_FMT_RGB555,
      /**
       * RGB-5-6-5      (16 bits per pixel)
       */
      RGB565  = V4L2_PIX_FMT_RGB565,
      /**
       * RGB-5-5-5 BE   (16 bits per pixel)
       */
      RGB555X = V4L2_PIX_FMT_RGB555X,
      /**
       * RGB-5-6-5 BE   (16 bits per pixel)
       */
      RGB565X = V4L2_PIX_FMT_RGB565X,
      /**
       * BGR-8-8-8      (24 bits per pixel)
       */
      BGR24   = V4L2_PIX_FMT_BGR24,
      /**
       * RGB-8-8-8      (24 bits per pixel)
       */
      RGB24   = V4L2_PIX_FMT_RGB24,
      /**
       * BGR-8-8-8-8    (32 bits per pixel)
       */
      BGR32   = V4L2_PIX_FMT_BGR32,
      /**
       * RGB-8-8-8-8    (32 bits per pixel)
       */
      RGB32   = V4L2_PIX_FMT_RGB32,
      /**
       * Greyscale      (8 bits per pixel)
       */
      GREY    = V4L2_PIX_FMT_GREY,
      /**
       * Greyscale      (16 bits per pixel)
       */
      Y16     = V4L2_PIX_FMT_Y16,
      /**
       * 8-bit palette  (8 bits per pixel)
       */
      PAL8    = V4L2_PIX_FMT_PAL8,
      /**
       * YVU 4:1:0      (9 bits per pixel)
       */
      YVU410  = V4L2_PIX_FMT_YVU410,
      /**
       * YVU 4:2:0      (12 bits per pixel)
       */
      YVU420  = V4L2_PIX_FMT_YVU420,
      /**
       * YUV 4:2:2      (16 bits per pixel)
       */
      YUYV    = V4L2_PIX_FMT_YUYV,
      /**
       * YUV 4:2:2      (16 bits per pixel)
       */
      UYVY    = V4L2_PIX_FMT_UYVY,
      /**
       * YVU422 planar  (16 bits per pixel)
       */
      YUV422P = V4L2_PIX_FMT_YUV422P,
      /**
       * YVU411 planar  (16 bits per pixel)
       */
      YUV411P = V4L2_PIX_FMT_YUV411P,
      /**
       * YUV 4:1:1      (12 bits per pixel)
       */
      Y41P    = V4L2_PIX_FMT_Y41P,
      /**
       * xxxxyyyy uuuuvvvv  (16 bits per pixel)
       */
      YUV444  = V4L2_PIX_FMT_YUV444,
      /**
       * YUV-5-5-5      (16 bits per pixel)
       */
      YUV555  = V4L2_PIX_FMT_YUV555,
      /**
       * YUV-5-6-5      (16 bits per pixel)
       */
      YUV565  = V4L2_PIX_FMT_YUV565,
      /**
       * YUV-8-8-8-8    (32 bits per pixel)
       */
      YUV32   = V4L2_PIX_FMT_YUV32,

      /**
       * Y/CbCr 4:2:0   (12 bits per pixel)
       *
       * two planes -- one Y, one Cr + Cb interleaved
       */
      NV12    = V4L2_PIX_FMT_NV12,
      /**
       * Y/CrCb 4:2:0   (12 bits per pixel)
       */
      NV21    = V4L2_PIX_FMT_NV21,

      /**
       * YUV 4:1:0      (9 bits per pixel)
       *
       * (not defined in the V4L2 specification)
       */
      YUV410  = V4L2_PIX_FMT_YUV410,
      /**
       * YUV 4:2:0      (12 bits per pixel)
       *
       * (not defined in the V4L2 specification)
       */
      YUV420  = V4L2_PIX_FMT_YUV420,
      /**
       * YUV 4:2:2      (16 bits per pixel)
       *
       * (not defined in the V4L2 specification)
       */
      YYUV    = V4L2_PIX_FMT_YYUV,
      /**
       * 8-bit color    (8 bits per pixel)
       *
       * (not defined in the V4L2 specification)
       */
      HI240   = V4L2_PIX_FMT_HI240,
      /**
       * YUV 4:2:0 16x16 macroblocks  (8 bits per pixel)
       *
       * (not defined in the V4L2 specification)
       */
      HM12    = V4L2_PIX_FMT_HM12,

      /**
       * BGBG.. GRGR..  (8 bits per pixel)
       * see http://www.siliconimaging.com/RGB%20Bayer.htm
       */
      SBGGR8  = V4L2_PIX_FMT_SBGGR8,
      /**
       * BGBG.. GRGR..  (16 bits per pixel)
       * see http://www.siliconimaging.com/RGB%20Bayer.htm
       */
      SBGGR16 = V4L2_PIX_FMT_SBGGR16,

      /**
       * Motion-JPEG    (compressed format)
       */
      MJPEG    = V4L2_PIX_FMT_MJPEG,
      /**
       * JFIF JPEG      (compressed format)
       */
      JPEG     = V4L2_PIX_FMT_JPEG,
      /**
       * Unknown (compressed format)
       */
      DV       = V4L2_PIX_FMT_DV,
      /**
       * MPEG-1/2/4     (compressed format)
       */
      MPEG     = V4L2_PIX_FMT_MPEG,

      /**
       * Winnov hw compress  (Vendor-specific formatl)
       */
      WNVA     = V4L2_PIX_FMT_WNVA,
      /**
       * SN9C10x compression  (Vendor-specific format)
       */
      SN9C10X  = V4L2_PIX_FMT_SN9C10X,
      /**
       * pwc older webcam  (Vendor-specific format)
       */
      PWC1     = V4L2_PIX_FMT_PWC1,
      /**
       * pwc newer webcam  (Vendor-specific format)
       */
      PWC2     = V4L2_PIX_FMT_PWC2,
      /**
       * ET61X251 compression  (Vendor-specific format)
       */
      ET61X251 = V4L2_PIX_FMT_ET61X251
      
    };

    /**
     * Enumeration of possible configurations for the power line filter.
     */
    enum ePowerLineFilter {
      Disabled = V4L2_CID_POWER_LINE_FREQUENCY_DISABLED, /**< Power Line Filter
                                                              Disabled */
      Filter50Hz=V4L2_CID_POWER_LINE_FREQUENCY_50HZ, /**< Power Line Filter
                                                          in 50Hz */
      Filter60Hz=V4L2_CID_POWER_LINE_FREQUENCY_60HZ  /**< Power Line Filter
                                                          in 60Hz */
    };
    
    /**
     * Enumeration of possible I/O access methods.
     */
    enum eIOMethod {
      Read,      /**< Use read() calls                  */
      MemoryMap, /**< Use memory mapped buffers         */
      UserSpace  /**< Use application allocated buffers */
    };
    
    /**
     * v4l2::parameters class.
     *
     * Most attributes are inherited from the lti::camera::parameters
     * interface.
     *
     * \section resolution Resolution and frame rate
     *
     * The image size and pixel format and frame-rates are strongly
     * related within the video4linux2 API.  However, not all cameras
     * support reading or setting the framerates easily.
     *
     * \section onePushMode One-push mode
     *
     * Not all v4l2 cameras support one-push configurations,
     * i.e., a mode in which the camera works in auto-adjustment mode
     * but only in a short time interval, after which the settings are
     * maintained.  Maybe in future releases this functionality will
     * be added through a software-based implementation.
     *
     */
    class parameters : public frameGrabber::parameters,
                       public camera::parameters,
                       public panTiltUnit::parameters,
                       public lensUnit::parameters {
    public:
      /**
       * Constructor to generate an invalid set of parameters (all values out
       * of range).
       *
       * If \a valid is \c false, the parameter set will be invalid, otherwise
       * a default set will be generated.
       */
      parameters(const bool valid=true);

      /**
       * Copy constructor
       */
      parameters(const parameters& other);

      /**
       * Copy member
       */
      parameters& copy(const parameters& other);

      /**
       * Returns a pointer to a clone of the parameters.
       */
      virtual parameters* clone() const;

      /**
       * Returns a pointer to a new instance of the parameters
       */
      virtual parameters* newInstance() const;

      /**
       * Returns the name of this class
       */
      const std::string& name() const;

      /**
       * Write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool write(ioHandler& handler,const bool complete=true) const;

      /**
       * Write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool read(ioHandler& handler,const bool complete=true);


      // ------------------------------------------------
      // the parameters
      // ------------------------------------------------

      // from lti::camera::parameters

      // resolution
      // framerate

      // equivalences between v4l2 and lti::camera (IIDC)
      //
      // V4L2_CID_BRIGHTNESS          FeatureBrightness   / Mode: FeatureManual
      // V4L2_CID_CONTRAST            FeatureContrast     / Mode: FeatureManual
      // V4L2_CID_SATURATION          FeatureSaturation   / Mode: FeatureManual
      // V4L2_CID_HUE                 FeatureHue          / Mode: FeatureManual
      // V4L2_CID_HUE_AUTO            FeatureHue          / Mode: FeatureAuto
      // V4L2_CID_AUTO_WHITE_BALANCE  FeatureWhiteBalance / Mode: FeatureAuto
      // V4L2_CID_DO_WHITE_BALANCE    FeatureWhiteBalance / Mode: OnePush
      // V4L2_CID_RED_BALANCE         FeatureWhiteBalance / Mode: FeatureManual
      // V4L2_CID_BLUE_BALANCE        FeatureWhiteBalance / Mode: FeatureManual
      // V4L2_CID_GAMMA               FeatureGamma        / Mode: FeatureManual
      // V4L2_CID_EXPOSURE            FeatureShutter      / Mode: FeatureManual
      // V4L2_CID_AUTOGAIN            FeatureGain         / Mode: FeatureAuto
      // V4L2_CID_GAIN                FeatureGain         / Mode: FeatureManual
      // V4L2_CID_HFLIP               -
      // V4L2_CID_VFLIP               -
      // V4L2_CID_POWER_LINE_FREQUENCY -
      // V4L2_CID_WHITE_BALANCE_TEMPERATURE -
      // V4L2_CID_SHARPNESS           FeatureSharpness    / Mode: FeatureManual
      // V4L2_CID_BACKLIGHT_COMPENSATION -

      // The extended camera class

      // V4L2_CID_EXPOSURE_AUTO       FeatureShutter      / Mode: FeatureAuto
      // V4L2_CID_EXPOSURE_ABSOLUTE   FeatureShutter      / Mode: FeatureAbs
      // V4L2_CID_EXPOSURE_AUTO_PRIORITY
      // V4L2_CID_PAN_RELATIVE
      // V4L2_CID_TILT_RELATIVE
      // V4L2_CID_PAN_RESET
      // V4L2_CID_TILT_RESET
      // V4L2_CID_PAN_ABSOLUTE
      // V4L2_CID_TILT_ABSOLUTE
      // V4L2_CID_FOCUS_ABSOLUTE
      // V4L2_CID_FOCUS_RELATIVE
      // V4L2_CID_FOCUS_AUTO
      
      /**
       * I/O Method
       *
       * This class supports three ways for the communication with the devices,
       * but depending on the kernel driver used, maybe only some of them might
       * be supported.
       *
       * Default value: MemoryMap
       */
      eIOMethod ioMethod;
      
      /**
       * Pixel format
       *
       * Suggested pixel format for the camera.
       *
       * If you call dumpParameters(), then this value will be set to the
       * format in use by your camera.
       *
       * Default value: Auto
       */
      ePixelFormat pixelFormat;

      /**
       * Bayer demosaicing functor name
       *
       * If the pixelFormat is a bayer one, then the given class will be used
       * for demosaicing.  It must be a valid name of a class that inherits
       * from lti::bayerDemosaicing.
       *
       * Default value: lti::bilinearDemosaicing
       */
      std::string bayerDemosaicing;
      
      /**
       * VerticalFlip feature state.
       *
       * The vertical flip feature mode in V4L2 is whether not
       * available or manually set.
       *
       * - FeatureNA: VerticalFlip not available
       * - FeatureManual: User sets the verticalFlip value manually with the
       *                  attribute \c parameters::verticalFlip
       *
       * Default value: FeatureManual
       */
      eCamFeatureMode verticalFlipMode;

      /**
       * Vertical flip
       *
       * Vertical flip the image
       *
       * Default value: false
       */
      bool verticalFlip;

      /**
       * HorizontalFlip feature state.
       *
       * The horizontal flip feature mode in V4L2 is whether not
       * available or manually set.
       *
       * - FeatureNA: HorizontalFlip not available
       * - FeatureManual: User sets the horizontalFlip value manually with the
       *                  attribute \c parameters::horizontalFlip
       *
       * Default value: FeatureManual
       */
      eCamFeatureMode horizontalFlipMode;

      /**
       * Horizontal flip
       *
       * Horizontal flip the image
       *
       * Default value: false
       */
      bool horizontalFlip;

      /**
       * PowerLineFilter feature state.
       *
       * The powerLineFilter feature mode in V4L2 is whether not
       * available or manually set.
       *
       * - FeatureNA: PowerLineFilter not available
       * - FeatureManual: User sets the powerLineFilter value manually with the
       *                  attribute \c parameters::powerLineFilter
       *
       * Default value: FeatureManual
       */
      eCamFeatureMode powerLineFilterMode;

      /**
       * Power line filter configuration.
       *
       * If available, you can activate the filter for power line effect to
       * 50Hz (Filter50Hz) or to 60Hz (Filter60Hz).
       *
       * Default value: Disabled
       */
      ePowerLineFilter powerLineFilter;

      /**
       * White balance temperature feature state.
       *
       * The white balance temperature feature mode in V4L2 is whether
       * not available or manually set.
       *
       * - FeatureNA: HorizontalFlip not available
       * - FeatureManual: User sets the whiteBalanceTemperature value manually 
       *                  with the attribute 
       *                  \c parameters::whiteBalanceTemperature
       *
       * Default value: FeatureManual
       */
      eCamFeatureMode whiteBalanceTemperatureMode;

      /**
       * White Balance Temperature
       *
       * This value must be between 2800K (incandescent light) to 6500K
       * (daylight)
       *
       * Default value: 6500 (K)
       */
      int whiteBalanceTemperature;

      /**
       * Back-light compensation feature state.
       *
       * The backlightCompensation feature mode in V4L2 is whether not
       * available or manually set.
       *
       * - FeatureNA: PowerLineFilter not available
       * - FeatureManual: User sets the backlightCompensation value manually
       *                  with the attribute
       *                  \c parameters::backlightCompensation
       *
       * Default value: FeatureManual
       */
      eCamFeatureMode backlightCompensationMode;

      /**
       * Backlight compensation.
       *
       * This value depends on the specific camera.  Usually the minimum value
       * allowed deactivates the compensation.
       *
       * Default value: 0
       */
      int backlightCompensation;

      /**
       * Device name.
       *
       * This string denotes the name of the device used.  For Linux it
       * could be for example /dev/video or /dev/video0.  There is a special
       * constructor that allows you to give this value directly.
       *
       * Default value: /dev/video0
       */
      std::string deviceFile;

      /**
       * Select time out.
       *
       * The images are captured using the system select() function to monitor
       * the device.  You can specify which is the maximal time to wait for an
       * image.  The units of the given value are seconds.
       *
       * Default value: 2.0 seconds
       *
       */
      float selectTimeout;

      /**
       * Select retries
       *
       * Some cameras have a problem while selecting the device, which is
       * a step in the standard protocol for accessing it using the method
       * select().
       *
       * For some cameras it is sufficient to increase the time given
       * to wait for a new image (see parameters::selectTimeout), while
       * for other it is necessary to try several times.
       *
       * This parameter specifies how many times the selection has to
       * be retried when an error occurs.
       *
       * Default value: 5
       */
      int selectRetries;

    };

    /**
     * Default constructor
     */
    v4l2();

    /**
     * Constructor with initialization for the given device
     */
    v4l2(const std::string& device);

    /**
     * Copy constructor
     */
    v4l2(const v4l2& other);

    /**
     * Constructor, sets the parameters
     */
    v4l2(const parameters& theParam);

    /**
     * Destructor
     */
    virtual ~v4l2( void );

    /**
     * Returns current parameters.
     */
    const parameters& getParameters() const;

    /**
     * Update parameters.
     *
     * Overload to allow changing the camera attributes
     */
    virtual bool updateParameters();

    /**
     * Dump parameters from the camera.
     *
     * Get the current parameters from the camera
     * (the camera device is the one who is dumping data into the
     * given object).
     *
     * This method assumes that a specific camera has already been initialized,
     * and for that, one parameter set with the proper device name has to have
     * been set already.
     */
    bool dumpParameters(parameters& par);

    /**
     * Returns the name of this class
     */
    const std::string& name() const;

    /**
     * Load an (color) image from the camera
     */
    bool apply(image& theImage);

    /**
     * Load a grey value channel from the camera
     */
    bool apply(channel8& theChannel);

    /**
     * Check if the camera associated with this instance is active (working)
     */
    bool isActive() const;

    /**
     * Copy member
     */
    v4l2& copy(const v4l2& other);

    /**
     * Clone member
     */
    v4l2* clone() const;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual v4l2* newInstance() const;

    /**
     * Check if the frame grabber has been initialized
     */
    bool isInitialized() const;

    /**
     * Initialize "frame grabber".
     */
    bool init();

    /**
     * Initialize format.
     *
     * This should be the last initialization step, and takes some
     * information gained in the rest of the initializations and in
     * the user parameters.
     */
    bool initFormat(const eIOMethod ioMethod,
                    const ePixelFormat pxFmt,
                    const v4l2_field fieldOrder,
                    ipoint& res);

    /**
     * @name Direct access to camera features.
     *
     * These methods access the camera registers directly, and they
     * try to update the internal parameters.  However, since the
     * cameras may adjust themselves, you may find the method
     * dumpParameters() or the inherited classes useful, which asks
     * the camera for all parameters in use at once.
     *
     * If a feature supports the "one-push mode", then the automatic
     * configuration will begin as soon as you call the corresponding method
     * with the value \c FeatureOnePush as mode (for example
     * setWhiteBalance(FeatureOnePush) ).
     *
     * If you set the mode of one or more features to one-push, then you may
     * want to wait for them to be ready with the method onePushWait().  Of
     * course, this will work if and only if the camera supports this feature
     * mode.
     *
     * The methods to query information from the camera do indeed ask the
     * camera for the corresponding value (if and only if this is possible).
     * There are cameras which do not support the readout capability for the
     * registers holding the feature values.  In that case, the get* methods
     * will return \c false.
     */
    //@{

    /**
     * Set brightness control mode and register in camera directly.
     *
     * @param mode Camera desired mode (@see eCamFeatureMode)
     * @param brightness If mode is manual, the value to be used.
     *
     * @return true if operation was successfull, false otherwise
     */
    virtual bool setBrightness(const eCamFeatureMode mode,
                               const float brightness=0);

    /**
     * Get brightness mode and value directly out of camera.
     *
     * @param mode Variable where the current brightness mode is to be 
     *              written.
     * @param brightness Variable where the brightness value is to be written.
     *
     * @return true if operation was successfull, false otherwise
     */ 
    virtual bool getBrightness(eCamFeatureMode& mode,
                               float& brightness) const;

    /**
     * Set exposure control mode and register in camera directly.
     *
     * @param mode Camera desired mode (@see eCamFeatureMode)
     * @param exposure If mode is manual, the value to be used.
     *
     * @return true if operation was successfull, false otherwise
     */
    virtual bool setExposure(const eCamFeatureMode mode,
                             const float exposure=0);

    /**
     * Get exposure mode and value directly out of camera.
     *
     * @param mode Variable where the current exposure mode is to be 
     *              written.
     * @param exposure Variable where the exposure value is to be written.
     *
     * @return true if operation was successfull, false otherwise
     */ 
    virtual bool getExposure(eCamFeatureMode& mode,
                             float& exposure) const;

    /**
     * Set sharpness control mode and register in camera directly.
     *
     * @param mode Camera desired mode (@see eCamFeatureMode)
     * @param sharpness If mode is manual, the value to be used.
     *
     * @return true if operation was successfull, false otherwise
     */
    virtual bool setSharpness(const eCamFeatureMode mode,
                              const float sharpness=0);

    /**
     * Get sharpness mode and value directly out of camera.
     *
     * @param mode Variable where the current sharpness mode is to be 
     *              written.
     * @param sharpness Variable where the sharpness value is to be written.
     *
     * @return true if operation was successfull, false otherwise
     */ 
    virtual bool getSharpness(eCamFeatureMode& mode,
                              float& sharpness) const;

    /**
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
    virtual bool setWhiteBalance(const eCamFeatureMode mode,
                                 const float ubGain=0,
                                 const float vrGain=0);
    
    /**
     * Get whiteBalance mode and value directly out of camera.
     *
     * @param mode Variable where the current whiteBalance mode is to be 
     *              written.
     * @param ubGain gain for the blue channel (or U channel)
     * @param vrGain gain for the red channel (or V channel)
     *
     * @return true if operation was successfull, false otherwise
     */ 
    virtual bool getWhiteBalance(eCamFeatureMode& mode,
                                 float& ubGain,
                                 float& vrGain) const;

    /**
     * Set hue control mode and register in camera directly.
     *
     * @param mode Camera desired mode (@see eCamFeatureMode)
     * @param hue If mode is manual, the value to be used.
     *
     * @return true if operation was successfull, false otherwise
     */
    virtual bool setHue(const eCamFeatureMode mode,
                        const float hue=0);

    /**
     * Get hue mode and value directly out of camera.
     *
     * @param mode Variable where the current hue mode is to be 
     *              written.
     * @param hue Variable where the hue value is to be written.
     *
     * @return true if operation was successfull, false otherwise
     */ 
    virtual bool getHue(eCamFeatureMode& mode,
                        float& hue) const;

    /**
     * Set saturation control mode and register in camera directly.
     *
     * @param mode Camera desired mode (@see eCamFeatureMode)
     * @param saturation If mode is manual, the value to be used.
     *
     * @return true if operation was successfull, false otherwise
     */
    virtual bool setSaturation(const eCamFeatureMode mode,
                               const float saturation=0);

    /**
     * Get saturation mode and value directly out of camera.
     *
     * @param mode Variable where the current saturation mode is to be 
     *              written.
     * @param saturation Variable where the saturation value is to be written.
     *
     * @return true if operation was successfull, false otherwise
     */ 
    virtual bool getSaturation(eCamFeatureMode& mode,
                               float& saturation) const;
    
    /**
     * Set gamma control mode and register in camera directly.
     *
     * @param mode Camera desired mode (@see eCamFeatureMode)
     * @param gamma If mode is manual, the value to be used.
     *
     * @return true if operation was successfull, false otherwise
     */
    virtual bool setGamma(const eCamFeatureMode mode,
                          const float gamma=0);

    /**
     * Get gamma mode and value directly out of camera.
     *
     * @param mode Variable where the current gamma mode is to be 
     *              written.
     * @param gamma Variable where the gamma value is to be written.
     *
     * @return true if operation was successfull, false otherwise
     */ 
    virtual bool getGamma(eCamFeatureMode& mode,
                          float& gamma) const;

    /**
     * Set shutter control mode and register in camera directly.
     *
     * @param mode Camera desired mode (@see eCamFeatureMode)
     * @param shutter If mode is manual, the value to be used.
     *
     * @return true if operation was successfull, false otherwise
     */
    virtual bool setShutter(const eCamFeatureMode mode,
                            const float shutter=0);

    /**
     * Get shutter mode and value directly out of camera.
     *
     * @param mode Variable where the current shutter mode is to be 
     *              written.
     * @param shutter Variable where the shutter value is to be written.
     *
     * @return true if operation was successfull, false otherwise
     */ 
    virtual bool getShutter(eCamFeatureMode& mode,
                            float& shutter) const;

    /**
     * Set gain control mode and register in camera directly.
     *
     * @param mode Camera desired mode (@see eCamFeatureMode)
     * @param gain If mode is manual, the value to be used.
     *
     * @return true if operation was successfull, false otherwise
     */
    virtual bool setGain(const eCamFeatureMode mode,
                         const float gain=0);

    /**
     * Get gain mode and value directly out of camera.
     *
     * @param mode Variable where the current gain mode is to be 
     *              written.
     * @param gain Variable where the gain value is to be written.
     *
     * @return true if operation was successfull, false otherwise
     */ 
    virtual bool getGain(eCamFeatureMode& mode,
                         float& gain) const;

    /**
     * Get current frame rate directly out of camera.
     * @return true if operation was successfull, false otherwise
     */
    virtual bool getFramerate(float& framerate) const;

    /**
     * Get current resolution, posibly directly out of the camera
     */
    virtual bool getResolution(ipoint& resolution) const;

    /**
     * Set framerate directly to the camera.
     *
     * Since all buses have a finite bandwidth, the larger the image,
     * the less images per second can be send.  Most camera types
     * support only particular combinations of framerate and
     * resolution.  Therefore, it usually necessary to set both
     * parameters at once, to avoid invalid combinations.
     *
     * @return true if operation was successfull, false otherwise
     */
    virtual bool setFramerateAndResolution(const float framerate,
                                           const ipoint& resolution);

    //@}

    /**
     * Features of Video4Linux2
     */
    //@{
    /**
     * Set vertical flip.
     *
     * @param flip If set to false, the flip-feature will be deactivated.
     */
    bool setVerticalFlip(const bool flip=true);

    /**
     * Get vertical flip.
     *
     * @param flip If set to false, the flip-feature will be deactivated.
     */
    bool getVerticalFlip(bool& flip) const;

    /**
     * Set horizontal flip.
     *
     * @param flip If set to false, the flip-feature will be deactivated.
     */
    bool setHorizontalFlip(const bool flip=true);

    /**
     * Get horizontal flip.
     *
     * @param flip If set to false, the flip-feature will be deactivated.
     */
    bool getHorizontalFlip(bool& flip) const;

    /**
     * Set power line filter frequency.
     */
    bool setPowerLineFilter(const ePowerLineFilter filter);

    /**
     * Get power line filter frequency.
     */
    bool getPowerLineFilter(ePowerLineFilter& filter) const;

    /**
     * Set white balance temperature
     */
    bool setWhiteBalanceTemperature(const int temp);

    /**
     * Get white balance temperature
     */
    bool getWhiteBalanceTemperature(int& temp) const;

    /**
     * Set backlight compensation.
     */
    bool setBacklightCompensation(const int blCompensation);

    /**
     * Get backlight compensation.
     */
    bool getBacklightCompensation(int& blCompensation) const;
    //@}


    /**
     * @name Pan and Tilt interface
     *
     * The following methods are implemented to comply with the
     * lti::panTiltUnit interface.
     */
    //@{
    /**
     * Set pan control mode and register in device directly.
     *
     * @param mode Device desired mode (@see eCamFeatureMode)
     * @param pan If mode is manual, the value to be used.
     *
     * @return true if operation was successfull, false otherwise
     */
    virtual bool setPan(const eCamFeatureMode mode,
                        const float pan=0);

    /**
     * Get pan mode and value directly out of device.
     *
     * @param mode Variable where the current pan mode is to be 
     *              written.
     * @param pan Variable where the pan value is to be written.
     *
     * @return true if operation was successfull, false otherwise
     */ 
    virtual bool getPan(eCamFeatureMode& mode,
                        float& pan) const;

    /**
     * Set tilt control mode and register in device directly.
     *
     * @param mode Device desired mode (@see eCamFeatureMode)
     * @param tilt If mode is manual, the value to be used.
     *
     * @return true if operation was successfull, false otherwise
     */
    virtual bool setTilt(const eCamFeatureMode mode,
                         const float tilt=0);

    /**
     * Get tilt mode and value directly out of device.
     *
     * @param mode Variable where the current tilt mode is to be 
     *              written.
     * @param tilt Variable where the tilt value is to be written.
     *
     * @return true if operation was successfull, false otherwise
     */ 
    virtual bool getTilt(eCamFeatureMode& mode,
                         float& tilt) const;

    /**
     * Set pan control mode and register in device directly.
     *
     * @param mode Device desired mode (@see eCamFeatureMode)
     * @param pan If mode is manual, the value for pan to be used.
     * @param tilt If mode is manual, the value for tilt to be used.
     *
     * @return true if operation was successfull, false otherwise
     */
    virtual bool setPanTilt(const eCamFeatureMode mode,
                            const float pan,
                            const float tilt);

    /**
     * Get pan mode and value directly out of device.
     *
     * @param panMode Variable where the current pan mode is to be 
     *               written.
     * @param pan  Variable where the pan value is to be written.
     * @param tiltMode Variable where the current tilt mode is to be 
     *               written.
     * @param tilt  Variable where the tilt value is to be written.
     *
     * @return true if operation was successfull, false otherwise
     */ 
    virtual bool getPanTilt(eCamFeatureMode& panMode,
                            float& pan,
                            eCamFeatureMode& tiltMode,
                            float& tilt) const;

    /**
     * Reset whole pan-tilt units
     *
     * Pan-tilt units, as mechanical systems, tend to accumulate positioning
     * errors, and once in a while require a reset to calibrate its position.
     *
     * This method should do that task, and alters the internal parameters to
     * indicate the current set angle after the reset.
     */
    virtual bool panTiltReset();

    //@}

    /**
     * @name Lenses interface
     *
     * The following methods are implemented to comply with the
     * lti::lensUnit interface.
     */
    //@{
    /**
     * Set iris control mode and register in device directly.
     *
     * @param mode Device desired mode (@see eCamFeatureMode)
     * @param iris If mode is manual, the value to be used.
     *
     * @return true if operation was successfull, false otherwise
     */
    virtual bool setIris(const eCamFeatureMode mode,
                               const float iris=0);

    /**
     * Get iris mode and value directly out of device.
     *
     * @param mode Variable where the current iris mode is to be 
     *              written.
     * @param iris Variable where the iris value is to be written.
     *
     * @return true if operation was successfull, false otherwise
     */ 
    virtual bool getIris(eCamFeatureMode& mode,
                               float& iris) const;

    /**
     * Set focus control mode and register in device directly.
     *
     * @param mode Device desired mode (@see eCamFeatureMode)
     * @param focus If mode is manual, the value to be used.
     *
     * @return true if operation was successfull, false otherwise
     */
    virtual bool setFocus(const eCamFeatureMode mode,
                             const float focus=0);

    /**
     * Get focus mode and value directly out of device.
     *
     * @param mode Variable where the current focus mode is to be 
     *              written.
     * @param focus Variable where the focus value is to be written.
     *
     * @return true if operation was successfull, false otherwise
     */ 
    virtual bool getFocus(eCamFeatureMode& mode,
                             float& focus) const;

    /**
     * Set zoom control mode and register in device directly.
     *
     * @param mode Device desired mode (@see eCamFeatureMode)
     * @param zoom If mode is manual, the value to be used.
     *
     * @return true if operation was successfull, false otherwise
     */
    virtual bool setZoom(const eCamFeatureMode mode,
                              const float zoom=0);

    /**
     * Get zoom mode and value directly out of device.
     *
     * @param mode Variable where the current zoom mode is to be 
     *              written.
     * @param zoom Variable where the zoom value is to be written.
     *
     * @return true if operation was successfull, false otherwise
     */ 
    virtual bool getZoom(eCamFeatureMode& mode,
                         float& zoom) const;

    /**
     * Set opticalFilter control mode and register in device directly.
     *
     * Depending on the color space used by the device (RGB or YUV), the
     * color gains are applied to UV or to BR.
     *
     * @param mode Device desired mode (@see eCamFeatureMode)
     * @param opticalFilter value to set
     *
     * @return true if operation was successfull, false otherwise
     */
    virtual bool setOpticalFilter(const eCamFeatureMode mode,
                                  const float opticalFilter=0);
    
    /**
     * Get opticalFilter mode and value directly out of device.
     *
     * @param mode Variable where the current opticalFilter mode is to be 
     *              written.
     * @param opticalFilter variable to store the value obtained from the
     *                      device
     *
     * @return true if operation was successfull, false otherwise
     */ 
    virtual bool getOpticalFilter(eCamFeatureMode& mode,
                                  float& opticalFilter) const;

    
    //@}

    /**
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
    virtual bool isFeatureAvailable(const unsigned int featureId) const;

    /**
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
    virtual bool isFeatureReadable(const unsigned int featureId) const;

    /**
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
    virtual unsigned int checkFeatureModes(const unsigned int featId) const;

    /**
     * Get feature mode of the device for one specific feature.
     *
     * @return \c true if feature mode could be successfully read, or \c false
     *         if this was not possible (maybe the device does not support
     *         the given feature at all).
     */
    virtual bool getFeatureMode(const unsigned int featureId,
                                eCamFeatureMode& mode) const;

    /**
     * Set operation mode of a particular feature of the device.
     *
     * @return \c true if feature mode could be successfully set, or \c false
     *         if this was not possible (maybe the device does not support
     *         the given mode for the feature.
     */
    virtual bool setFeatureMode(const unsigned int featureId,
                                const eCamFeatureMode mode);

    /**
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
    virtual bool setFeatureValue(const unsigned int featureId,
                                 const eCamFeatureMode mode,
                                 const float value=0);
    
    /**
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
    virtual bool getFeatureValue(const unsigned int featureId,
                                 eCamFeatureMode& mode,
                                 float& value) const;

    /**
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
     * For pan and tilt, the units are degrees/100. 
     *
     * @return \c true if successful or \c false otherwise.
     */
    virtual bool checkFeatureValueRange(const unsigned int featureId,
                                        float& minValue,
                                        float& maxValue) const;

    /**
     * Get the set of supported framerates and resolutions.
     *
     * Note that not all combination of framerates and resolutions are
     * supported by a specific camera.  You get a vector of 
     * framerates, a vector of resolutions and a matrix indicating
     * if a given combination is supported or not.
     *
     * Since in Video4Linux2 this depends on the selected pixel format, then
     * this function will operate on the current pixel format set in the
     * parameters.
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
    bool supportedFrameratesAndResolutions(fvector& framerates,
                                           vector<ipoint>& resols,
                                           matrix<ubyte>& val) const;

    /**
     * Supported pixel formats
     *
     * Create a vector with the supported pixel formats.
     */
    bool supportedPixelFormats(std::vector<ePixelFormat>& formats) const;

    /**
     * Return the active camera name.
     *
     *
     * This method is intended for user interfaces, rather than some
     * kind of string-based checking of the camera being used.
     * Therefore, do not rely on the output of this string for
     * detecting if a camera is or not the one you are looking for.
     */
    virtual const std::string cameraName() const;


  protected:
   
    /**
     * A very simple buffer type, used by the different I/O methods
     */
    class bufferType {
    public:
      /**
       * Constructor
       * 
       * Creates a clean buffer
       */
      bufferType();

      /**
       * Destructor
       */
      ~bufferType();

      /**
       * Reserve memory, for the method indicated.
       *
       * @param size Desired size of the buffer.
       * @param method I/O method using this buffer
       * @param handleOrPageSize used as handle only if method is MemoryMap
       *                         or as page size if method is UserSpace.
       * @param offset used only if method is MemoryMap
       *
       * @return pointer to the allocated memory, which is also
       *         stored in the corresponding attribute ptr_;
       */
      void* reserve(const unsigned int size,
                    const eIOMethod method,
                    const int handleOrPageSize=-1,
                    const int offset=0);
      
      /**
       * Free the reserved memory
       *
       * @return true if everything went ok, or false otherwise.
       */
      bool release();

      /**
       * Return the used I/O Method in the current buffer.
       */
      inline eIOMethod getMethod() const;

      /**
       * Return the pointer to the start of the memory block
       */
      inline void* ptr() const;
    
      /**
       * Return the size of the memory block
       */
      inline unsigned int size() const;

    private:
      /**
       * Pointer to the memory block
       */
      void* ptr_;

      /**
       * Size of the reserved memory block
       */
      unsigned int size_;

      /**
       * Method of the memory block
       */
      eIOMethod method_;
    };

    /**
     * Type used for the controls_
     */
    typedef std::map<uint32,v4l2_queryctrl> controlsType;

    /**
     * On intialization, all supported features are 
     * reported on this map for faster access later.
     */
    controlsType controls_;

    /**
     * On initialization, all supported pixel formats are
     * inserted here
     */
    std::vector<v4l2_fmtdesc> pixelFormats_;

    /**
     * Return the best match for the given pixel format.
     *
     * The guess tries to keep the color space.
     *
     * For this method to work, the pixelFormat_ attribute should have
     * been already initialized by the corresponding initialization method.
     *
     * @param pxFrm suggested pixel format
     * @return 'nearest' supported pixel format to the given suggestion
     */
    ePixelFormat getBestFormat(const ePixelFormat pxFrm) const;

    /**
     * Return the best frame period for the given frameRate
     */
    v4l2_fract getBestFramePeriod(const float frameRate,
                                  const ePixelFormat pxFrm,
                                  const ipoint& resolution) const;

    /**
     * Each resolution has associated a number of possible framerates
     */
    struct resolutionData {
      ipoint resolution;
      std::vector<v4l2_fract> frameIntervals;
    };

    /**
     * On initialization, this complex database is filled with the
     * available resolutions for each format, and the available framerates
     * for each resolution.
     *
     * The index of this vector correponds to the index in the pixelFormats_
     * vector, for a given pixel format.
     */
    std::vector< std::vector<resolutionData> > sizesFR_;

    /**
     * Array of buffers used
     */
    std::vector<bufferType> buffers_;

    /**
     * The actual capture size for the images
     */
    ipoint resolution_;

    /**
     * Current pixel format in use
     */
    ePixelFormat pixelFormat_;

    /**
     * When the camera is initialized, this value is set to the
     * minimum available image size
     */
    ipoint minSize_;

    /**
     * When the camera is initialized, this value is set to the
     * maximum available image size
     */
    ipoint maxSize_;

    /**
     * Flag indicates if the camera interface has been initialized
     */
    bool initialized_;

    /**
     * Contains after initialization true if the device supports cropping
     * or false otherwise
     */
    bool cropSupported_;

    /**
     * Flag to indicate if we can change the frame rate
     */
    bool frameRateChangeSupported_;

    /**
     * Name of the camera being used
     */
    std::string camName_;

    /**
     * Name of the kernel module being used
     */
    std::string driverName_;

    /**
     * String with bus information
     */
    std::string busInfo_;

    /**
     * Flag indicating that capturing is in progress
     */
    bool capturing_;

    /**
     * Timeout for select() 
     */
    float timeout_;

    /**
     * Retries for select()
     */
    int retries_;

    /**
     * Flag indicating if a pan/tilt unit has been detected
     */
    bool panTilt_;

    /**
     * Flag indicating if a lens unit has been detected
     */
    bool lens_;

    /**
     * The camera file handle.
     * this is valid only if <code>initialized</code> is true
     */
    int cameraHndl_;

    /**
     * Set the camera parameters.
     *
     * This method transfers the parameter values in the v4l2::parameters
     * instance to the camera, but only those that have changed, to save some
     * USB bandwidth.
     *
     * The recursion parameter is used to take note on the number of trials
     * taken to recover from major errors
     */
    bool setCameraParameters(const int recursion=0);

    /**
     * Counter of recursions used in the initialization
     */
    int recursions_;

    /**
     * Returns current parameters.
     */
    parameters& getRWParameters();

    /**
     * Last used parameter values (used to check if a value needs to be
     * changed).  These are needed to spare some time sending the new
     * parameters to the camera
     */
    parameters lastUsedParams_;

    /**
     * Appends a kernel string into a std::string.
     *
     * It is assumed that src is 0 terminated, and no more than 32
     * characters should be copied
     */
    void transfer(std::string& dest,
                  const __u8* src) const;

   
    /**
     * Wrapper for the ioctl system function, which retries in recoverable
     * cases.
     */
    int xioctl(int cameraHndl_,
               uint32 request,
               void *arg) const;

    /**
     * Produce a standard status string containing the given string,
     * the error number and the string associated with that error.
     */
    void report(const char *const what) const;

    /**
     * Open camera device
     */
    bool openDevice(const parameters& par);

    /**
     * Close camera device
     */
    bool closeDevice();

    /**
     * Initialize device
     */
    bool initDevice(const parameters& par);

    /**
     * Init the controls_ database
     */
    bool initControls();

    /**
     * Init the controls_ database
     */
    bool initPixelFormats();

    /**
     * Init the database containing resolutions and frame-rates supported
     *
     * The method assumes that initPixelFormats() has already being called.
     */
    bool initSizeFR();

    /**
     * Init read() based I/O method
     */
    bool initRead(const int imageSize);

    /**
     * Init memory map based I/O method
     */
    bool initMemoryMap();

    /**
     * Init user space based I/O method
     */
    bool initUserSpace(const int imageSize);

    /**
     * Uninitialize device
     */
    bool uninitDevice();

    /**
     * Start capturing
     */
    //bool startCapture(const parameters& par);

    /**
     * Stop capturing
     */
    //bool stopCapture(const parameters& par);


    /**
     * Convert a value using the given format, to degrees
     */
    inline float toDegrees(const float val,
                           const eAngularUnitsType atype) const;

    /**
     * Convert a degree angle to the internal used representation
     */
    inline float toAngleVal(const float deg,
                            const eAngularUnitsType atype) const;

    /**
     * Read buffer
     *
     * The template type I means wether lti::image or lti::channel8
     */
    template<class I>
    bool readFrame(I& img);

    /**
     * Process the data and assemble the img
     *
     * The template type I means wether lti::image or lti::channel8
     */
    template<class I>
    bool processImage(void* data,
                      const unsigned int bufferSize,
                      const bool turnAround,
                      I& img);
    
    /**
     * Start capturing
     */
    bool startCapturing();

    /**
     * Stop capturing
     */
    bool stopCapturing();

  private:
    /**
     * Convenience method to clear all attributes of an instance of type T
     */
    template<class T>
    inline void clear(T& data) const;

    /**
     * Normalize a control value to a parameter from 0 to 1
     */
    float normalize(const int val,const v4l2_queryctrl& ctrl) const;

    /**
     * Denormalize a parameter value into the value control value
     */
    int32 denormalize(const float par,const v4l2_queryctrl& ctrl) const;

  public:
    /**
     * Abstract class used to convert data from a given pixel format
     * type to RGB or gray value.
     *
     * Each new linux kernel version seems to add new supported pixel formats
     * and it is not desirable to add new methods for each new format nor to
     * have a huge switch in order to do the proper color space transformation.
     *
     * Therefore, here we use a typical factory pattern, in which the
     * key for the factory is a pixel format, and a reference to a
     * conversion class is returned.
     *
     * Hence, for a new format just a new class needs to be inherited
     * and registered into the corresponding factory.  The init()
     * method should be static in nature, and is used to initialize
     * LUT and other static stuff.  That is the reason why the
     * references in the factory for the convertBase are not const.  Otherwise
     * the initialization could not be done.
     */
    class convertBase {
    public:
      /**
       * Virtual destructor
       */
      virtual ~convertBase();

      /**
       * Called when setting the parameters
       */
      virtual bool init(const parameters& par) = 0;

      /**
       * Convert data buffer to an image
       * 
       * @param data pointer to memory block 
       * @param bsize buffer size
       * @param turnAround if true, the image will be turned around
       * @param img resulting image
       *
       * @return true if successful, false otherwise
       */
      virtual bool convert(void* data,
                           const unsigned int bsize,
                           const bool turnAround,
                           image& img) const = 0;

      /**
       * Convert data buffer to an channel8 (grey valued channel)
       *
       * @param data pointer to memory block 
       * @param bsize buffer size
       * @param turnAround if true, the image will be turned around
       * @param img resulting channel
       *
       * @return true if successful, false otherwise
       */
      virtual bool convert(void* data,
                           const unsigned int bsize,
                           const bool turnAround,
                           channel8& img) const;

    protected:
      /**
       * Turn an image around
       */
      template<class I>
      bool turn(I& img) const;
    };

    /**
     * Factory of converters
     *
     * Singleton class that provides a const reference to a proper
     * converter instance.
     *
     * @see lti::v4l2::convertBase
     */
    class factory {
      friend class registration;
    public:
      /**
       * Return the one and only existing factory instance
       */
      static factory& getFactory();

      /**
       * Return the registered instance or NULL otherwise
       */
      convertBase* instance(const ePixelFormat pxFmt);

      /**
       * Registers an object under the given format.
       *
       * Note that exactly the given instance will be kept.  This
       * means you should never ever delete the given object.  This
       * factory will take care of that at the proper time.
       */
      void registerObject(convertBase* instance,
                          const ePixelFormat index);

      /**
       * Represents the registration of an object of type U,
       * constructed with the default parameterless constructor, and
       * under the given ePixelFormat value.
       */
      template <typename U>
      class registration {      
      public:      
        /**
         * Creates a new registration, thus registering an object of class U.
         */
        registration(const ePixelFormat pxFmt) {
          static U u; // one single static instance for each class U
          convertBase* tmp=getFactory().instance(pxFmt);
          if (isNull(tmp)) {
            getFactory().registerObject(&u,pxFmt);
          }
        }

      };

    private:
      /**
       * Eliminate possibility of constructing several factories
       */
      factory();

      /**
       * Eliminate possibility of constructing a copy of a factory
       */
      factory(const factory& other);
      
      /**
       * Eliminate possibility of destructing a factory
       */
      ~factory();

      /**
       * The type of the object map
       */
      typedef std::map<ePixelFormat,convertBase*> object_map_type;
    
      /**
       * The object map. Maps names to instances.
       */
      object_map_type objectMap_;

    };
    
  protected:
    /**
     * Converter in use.
     */
    convertBase* converter_;
    
  };

  /**
   * Write v4l2::ePixelFormat
   */
  bool write(ioHandler& handler,const v4l2::ePixelFormat val);

  /**
   * Read v4l2::ePixelFormat
   */
  bool read(ioHandler& handler,v4l2::ePixelFormat& val);

  /**
   * Write v4l2::ePowerLineFilter
   */
  bool write(ioHandler& handler,const v4l2::ePowerLineFilter val);

  /**
   * Read v4l2::ePowerLineFilter
   */
  bool read(ioHandler& handler,v4l2::ePowerLineFilter& val);

  /**
   * Write v4l2::eIOMethod
   */
  bool write(ioHandler& handler,const v4l2::eIOMethod val);

  /**
   * Read v4l2::eIOMethod
   */
  bool read(ioHandler& handler,v4l2::eIOMethod& val);


}
#endif // __linux__
#endif // _USE_V4L2
#endif // #ifdef _LTI_V4L2_H_
