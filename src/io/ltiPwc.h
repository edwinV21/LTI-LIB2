/*
 * Copyright (C) 2001, 2002, 2003, 2004
 * Pablo Alvarado
 * Lehrstuhl fuer Technische Informatik, RWTH-Aachen, Germany
 *
 * Copyright (C) 2005, 2006, 2007
 * Pablo Alvarado
 * Electronics Engineering Department, ITCR, Costa Rica
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
 * \file   ltiPwc.h
 *         Class for grabbing images from a Philips webcam using the pwc kernel
 *         module on linux.
 * \author Pablo Alvarado
 * \date   24.04.2001
 *
 * $Id: ltiPwc.h,v 1.8 2007-05-10 22:38:51 alvarado Exp $
 */


#ifndef _LTI_PWC_H_
#define _LTI_PWC_H_

#include "ltiHardwareConfig.h"

#ifdef _USE_PWC

#include "ltiFrameGrabber.h"
#include "ltiCamera.h"
#include "ltiPanTiltUnit.h"

#if __linux__

#include <string>

#undef None

namespace lti {

  /**
   * "Frame-grabber" functor for Philips USB Webcams.
   *
   * This class allows capturing images from Philips USB cameras using the pwc
   * kernel module, common for a large family of webcams.  For a compatibility
   * list see <a href="http://www.saillard.org/linux/pwc/">here</a>).  Even if
   * there is no physical frame-grabber, this class provides the standard
   * frameGrabber interface to simplify its use.
   *
   * At this time, only an implementation for Linux is provided.
   *
   * Due to the API of the pwc kernel module, setting the parameters
   * is much more efficient than calling the feature-wise methods,
   * since several features are set at once in each kernel call.
   * Hence, try to use setParameters() instead of the from lti::camera
   * inherited methods.
   *
   * The configure script will check if the \c pwc-ioctl.h header file
   * can be located on the standard include paths.  That will be the
   * selection criteria used to compile the functor and to define in
   * the ltiHardwareConfig.h file the preprocessor symbol "_USE_PWC"
   *
   * The file \c pwc-ioctl.h is usually included in your Linux kernel sources
   * (/usr/src/linux/drivers/usb/media/pwc), or in any case, in the internet
   * pages for that module.
   *
   * This functor has been adapted to be used with the newest kernel modules
   * provided at http://www.saillard.org/linux/pwc/.  Follow the instructions
   * there to install it.
   *
   * \warning When you install the module using \c module-assistant in any
   * Debian-based distribution for Linux, ensure that only the newest \c pwc.ko
   * module survives.  Otherwise you may get weird error messages due to the
   * use of the obsolete module in the standard kernel.
   *
   * You can also check the wiki page of the LTI-Lib for more information
   * (http://ltilib.pdoerfler.com/wiki/ToUCam).
   *
   * This class was ported from the lti::toUCam class of the LTI-Lib 1,
   * and changed its name to \a pwc because \a ToUCam that was just
   * one particular model of camera from the huge list of supported
   * ones, and \a pwc is a general name used by kernel and other users
   * to denote the "Philips Web Cams".
   *
   * @ingroup gAcquisition
   */
  class pwc : public frameGrabber,
              public camera,
              public panTiltUnit {

  public:
    /**
     * The image compression rate
     */
    enum eCompressionType {
      None   = 0,  /**< no compression will be used   */
      Low    = 1,  /**< low compression               */
      Medium = 2,  /**< middle compression rate       */
      High   = 3,  /**< high compression              */
      Invalid = -1 /**< flag to denote invalid value  */
      
    };

    /**
     * White balance automatic types
     */
    enum eWhiteBalanceType {
      InvalidWB   = -1, /**< Invalid value           */
      Indoor      =  0, /**< Indoor lighting         */
      Outdoor     =  1, /**< Outdoor lighting        */
      Fluorescent =  2, /**< Fluorescent lighting    */
      Auto        =  4  /**< Automatic White Balance */
    };

    /**
     * Noise reduction mode
     */
    enum eDenoiseType {
      InvalidDenoise = -1,/**< Invalid value          */
      NoDenoise      = 0, /**< No noise reduction     */
      LowDenoise     = 1, /**< Low noise reduction    */
      MedDenoise     = 2, /**< Medium noise reduction */
      HighDenoise    = 3  /**< High noise reduction   */
    };
    
    /**
     * pwc::parameters class.
     *
     * Most attributes are inherited from the lti::camera::parameters interface
     * but several notes must be stated:
     *
     * \section resolution Resolution and frame rate
     *
     * The image size and frame-rate are strongly related with pwc
     * based cameras.
     *
     * The following values are accepted by the camera: 128x96, 160x120,
     * 176x144, 320x240, 352x288 and 640x480.
     *
     * Default value: 320x240
     *
     * Following is an excerpt of the original driver pages:
     *
     * Resolution chart
     *
     * Below is a table that shows the possible resolutions and
     * framerates for the webcams. On the left, all possible
     * physical resolutions are listed; at the top, the various
     * framerates for the cameras.
     *
     * If a cell is blank, it means the combination of resolution
     * and framerate is not directly supported by the webcam. If an
     * unsupported image size is requested by a program, the first
     * smaller image size is selected and padded with a black or
     * gray border.
     *
     * A green cell means the combination is supported uncompressed;
     * a red cell indicates that combination is only available with
     * compressed modes.
     * \htmlonly
         <table border="1">
         <tr>
           <th rowspan="2" valign="bottom">Size</th>
           <th colspan="8">PCA 645/646 &amp; VC010</th>
           <th rowspan="8" width="15">&nbsp;</td>
           <th colspan="6">PCVC 675/680/690</th>
           <th rowspan="8" width="15">&nbsp;</th>
           <th colspan="6">PCVC 730/740</th>
         </tr>

         <tr>
           <th width="30">3.75</th>
           <th width="30">5</th>
           <th width="30">7.5</th>
           <th width="30">10</th>
           <th width="30">12</th>
           <th width="30">15</th>
           <th width="30">20</th>
           <th width="30">24</th>
           <th width="30">5</th>
           <th width="30">10</th>
           <th width="30">15</th>
           <th width="30">20</th>
           <th width="30">25</th>
           <th width="30">30</th>
           <th width="30">5</th>
           <th width="30">10</th>
           <th width="30">15</th>
           <th width="30">20</th>
           <th width="30">25</th>
           <th width="30">30</th>
         </tr>
         <tr>
           <th>sQCIF<br>128x96</th>
           <td>&nbsp;</td>
           <td bgcolor="#00cc00">&nbsp;</td>
           <td bgcolor="#00cc00">&nbsp;</td>
           <td bgcolor="#00cc00">&nbsp;</td>
           <td bgcolor="#00cc00">&nbsp;</td>
           <td bgcolor="#00cc00">&nbsp;</td>
           <td bgcolor="#00cc00">&nbsp;</td>
           <td bgcolor="#00cc00">&nbsp;</td>
           <td bgcolor="#00cc00">&nbsp;</td>
           <td bgcolor="#00cc00">&nbsp;</td>
           <td bgcolor="#00cc00">&nbsp;</td>
           <td bgcolor="#00cc00">&nbsp;</td>
           <td bgcolor="#00cc00">&nbsp;</td>
           <td bgcolor="#00cc00">&nbsp;</td>
           <td>&nbsp;</td>
           <td>&nbsp;</td>
           <td>&nbsp;</td>
           <td>&nbsp;</td>
           <td>&nbsp;</td>
           <td>&nbsp;</td>
         </tr>
         <tr>
           <th>QSIF<br>160x120</th>
           <td>&nbsp;</td>
           <td>&nbsp;</td>
           <td>&nbsp;</td>
           <td>&nbsp;</td>
           <td>&nbsp;</td>
           <td>&nbsp;</td>
           <td>&nbsp;</td>
           <td>&nbsp;</td>
           <td bgcolor="#00cc00">&nbsp;</td>
           <td bgcolor="#00cc00">&nbsp;</td>
           <td bgcolor="#00cc00">&nbsp;</td>
           <td bgcolor="#00cc00">&nbsp;</td>
           <td bgcolor="#00cc00">&nbsp;</td>
           <td bgcolor="#00cc00">&nbsp;</td>
           <td bgcolor="#00cc00">&nbsp;</td>
           <td bgcolor="#00cc00">&nbsp;</td>
           <td bgcolor="#00cc00">&nbsp;</td>
           <td bgcolor="#00cc00">&nbsp;</td>
           <td bgcolor="#00cc00">&nbsp;</td>
           <td bgcolor="#00cc00">&nbsp;</td>
         </tr>
         <tr>
           <th>QCIF<br>176x144</th>
           <td>&nbsp;</td>
           <td bgcolor="#00cc00">&nbsp;</td>
           <td bgcolor="#00cc00">&nbsp;</td>
           <td bgcolor="#00cc00">&nbsp;</td>
           <td bgcolor="#00cc00">&nbsp;</td>
           <td bgcolor="#00cc00">&nbsp;</td>
           <td bgcolor="#00cc00">&nbsp;</td>
           <td bgcolor="#00cc00">&nbsp;</td>
           <td bgcolor="#00cc00">&nbsp;</td>
           <td bgcolor="#00cc00">&nbsp;</td>
           <td bgcolor="#00cc00">&nbsp;</td>
           <td bgcolor="#00cc00">&nbsp;</td>
           <td bgcolor="#00cc00">&nbsp;</td>
           <td bgcolor="#cc0000">&nbsp;</td>
           <td>&nbsp;</td>
           <td>&nbsp;</td>
           <td>&nbsp;</td>
           <td>&nbsp;</td>
           <td>&nbsp;</td>
           <td>&nbsp;</td>
         </tr>
         <tr>
           <th>SIF<br>320x240</th>
           <td>&nbsp;</td>
           <td>&nbsp;</td>
           <td>&nbsp;</td>
           <td>&nbsp;</td>
           <td>&nbsp;</td>
           <td>&nbsp;</td>
           <td>&nbsp;</td>
           <td>&nbsp;</td>
           <td bgcolor="#00cc00">&nbsp;</td>
           <td bgcolor="#cc0000">&nbsp;</td>
           <td bgcolor="#cc0000">&nbsp;</td>
           <td bgcolor="#cc0000">&nbsp;</td>
           <td bgcolor="#cc0000">&nbsp;</td>
           <td bgcolor="#cc0000">&nbsp;</td>
           <td bgcolor="#00cc00">&nbsp;</td>
           <td bgcolor="#cc0000">&nbsp;</td>
           <td bgcolor="#cc0000">&nbsp;</td>
           <td bgcolor="#cc0000">&nbsp;</td>
           <td bgcolor="#cc0000">&nbsp;</td>
           <td bgcolor="#cc0000">&nbsp;</td>
         </tr>
         <tr>
           <th>CIF<br>352x288</td>
           <td bgcolor="#00cc00">&nbsp;</td>
           <td bgcolor="#00cc00">&nbsp;</td>
           <td bgcolor="#00cc00">&nbsp;</td>
           <td bgcolor="#00cc00">&nbsp;</td>
           <td bgcolor="#00cc00">&nbsp;</td>
           <td bgcolor="#00cc00">&nbsp;</td>
           <td>&nbsp;</td>
           <td>&nbsp;</td>
           <td bgcolor="#00cc00">&nbsp;</td>
           <td bgcolor="#cc0000">&nbsp;</td>
           <td bgcolor="#cc0000">&nbsp;</td>
           <td bgcolor="#cc0000">&nbsp;</td>
           <td bgcolor="#cc0000">&nbsp;</td>
           <td bgcolor="#cc0000">&nbsp;</td>
           <td>&nbsp;</td>
           <td>&nbsp;</td>
           <td>&nbsp;</td>
           <td>&nbsp;</td>
           <td>&nbsp;</td>
           <td>&nbsp;</td>
         </tr>
         <tr>
           <th>VGA<br>640x480</td>
           <td>&nbsp;</td>
           <td>&nbsp;</td>
           <td>&nbsp;</td>
           <td>&nbsp;</td>
           <td>&nbsp;</td>
           <td>&nbsp;</td>
           <td>&nbsp;</td>
           <td>&nbsp;</td>
           <td bgcolor="#cc0000">&nbsp;</td>
           <td bgcolor="#cc0000">&nbsp;</td>
           <td bgcolor="#cc0000">&nbsp;</td>
           <td>&nbsp;</td>
           <td>&nbsp;</td>
           <td>&nbsp;</td>
           <td bgcolor="#cc0000">&nbsp;</td>
           <td bgcolor="#cc0000">&nbsp;</td>
           <td bgcolor="#cc0000">&nbsp;</td>
           <td>&nbsp;</td>
           <td>&nbsp;</td>
           <td>&nbsp;</td>
         </tr>
         </table>
         \endhtmlonly
     *
     * (VGA resolutions are limited to 15 fps: there isn't
     * enough bandwidth available on the USB bus to squeeze through
     * more, even with compression).
     *
     * The Philips Web Cam module supports any image size between the
     * minimum and maximum sizes. For image sizes that are not an
     * 'integral' image size directly supported by the camera, the
     * image is now padded with a gray or black border, depending on
     * the palette.
     *
     * Note: the driver only pads the image; it selects a format
     * that is smaller or equal to the requested size; it does not
     * crop an image that is only a bit larger than the requested
     * current image.
     *
     * \section imageFeats Image features
     * 
     * - brightness: values from 0.0 to 1.0, only FeatureManual mode.
     * - contrast: values from 0.0 to 1.0, only Feature Manual mode.
     * - sharpness: values from 0.0 to 1.0, FeatureManual and FeatureAuto mode.
     * - whiteBalance: FeatureManual and FeatureAuto modes.  In manual mode
     *   redGain and blueGain require values from 0.0 to 1.0.  In case of
     *   automatic mode, the whiteBalance attribute indicates what kind of
     *   automatic adaption is required (outdoors,indoors or fluorescent
     *   lightning can be chosen).
     * - hue: values from 0.0 to 1.0, only FeatureManual mode.
     * - saturation: values from 0.0 to 1.0, only FeatureManual mode.
     * - gamma: values from 0.0 to 1.0, only FeatureManual mode.
     * - shutter: values from 0.0 to 1.0, FeatureManual and FeatureAuto modes.
     *   In manual mode, a value of 1.0 means the longest possible
     *   integration time, which is theoretically almost one second
     *   (65535/65536).  If you need to set this time to exactly t
     *   seconds (with t<1), then you can use as value t*65535/65536.  You have
     *   to consider, however, that the pwc-based cameras give priority to 
     *   the frameRate setting.  Since the slowest frameRate is 5 frames per
     *   second, then the maximal effective integration time you can set here
     *   is 1/5 seconds (a value of 0.199996948242f)
     * - gain: values from 0.0 to 1.0, FeatureManual and Feature Auto modes.
     *
     * \section onePushMode One-push mode
     *
     * The pwc cameras do not support any one-push configuration,
     * i.e., a mode in which the camera works in auto-adjustment mode
     * but only in a short time interval, after which the settings are
     * maintained.  Maybe in future releases this functionality will
     * be added through a software-based implementation.
     *
     * \section panTilt Pan and Tilt
     *
     * Cameras supporting the pan and tilt units (like Logitech's
     * Orbit or Sphere cameras) use as internal angular values 1/100 of a
     * degree, i.e. a value of 100 in pan or tilt parameters, using as 
     * angularUnits DeviceDependent, means 1 degree.
     *
     */
    class parameters : public frameGrabber::parameters,
                       public camera::parameters,
                       public panTiltUnit::parameters {
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

      /**
       * Set compression preference.
       *
       * The Philips webcams use losy compression techniques to get the
       * images across the (narrow) USB bus. With this parameter
       * you can specify if you like no compression, low, medium or
       * high compression. Higher compression means less bandwidth is
       * used, but it could introduce artefacts in the image.
       *
       * Default value: Medium
       */
      eCompressionType compression;

      /**
       * White balance mode specialization.
       *
       * If the lti::camera::parameters::whiteBalanceMode is set to
       * automatic mode, then with this mode you can select what kind
       * of setting is prefered.  See lti::pwc::eWhiteBalanceType for the
       * set of options.
       *
       * Default value: Outdoor
       */
      eWhiteBalanceType whiteBalance;

      /**
       * White balance control speed.
       *
       * If the whiteBalanceMode is set to FeatureAuto, then this
       * value indicates how fast the camera reacts to changes in
       * lighting.
       *
       * Value range from 0.0 to 1.0.  A value of 1.0 means the
       * fastest possible reaction time and 0.0 the slowest possible
       * reaction time.
       *
       * Default value: 0.5
       */
      float whiteBalanceControlSpeed;


      /**
       * White balance control delay.
       *
       * If the whiteBalanceMode is set to FeatureAuto, then this
       * value indicates how long the camera waits before it starts
       * adjusting the red/blue gains.
       *
       * Default value: 0.5
       */
      float whiteBalanceControlDelay;

      /**
       * LED
       *
       * Specify a value of zero or negative for an "off LED" and a value of
       * exactly 1 for a LED always on.
       *
       * Otherwise, the led "on" time will be computed as "led%65536"
       * and the led "off" time as "led/65536".  Both times will have
       * a resolution of 100ms and accept a maximum value of 25000.
       * Not all camera models support the LED control.
       *
       * Default value: 1 (led on).
       */
      int led;

      /**
       * Backlight compensation.
       *
       * If \c true, the backlight compensation is activated.
       *
       * Default value: false
       */
      bool backlightCompensation;

      /**
       * Anti-flicker mode.
       *
       * Feature to compensate for monitor refresh rate and the
       * electrical power supply frequencies, which can produce some
       * 'pulsations' in the acquired images.
       *
       * Default value: false
       */ 
      bool antiflickerMode;

      /**
       * Amount of noise reduction
       *
       * Default: NoDenoise; (i.e. noise reduction deactivated)
       */
      eDenoiseType denoise;

      /**
       * Device name.
       *
       * This string denotes the name of the device used.  For Linux it
       * could be for example /dev/video or /dev/video0.  There is a special
       * constructor that allows you to give this value directly.
       *
       * Default value: for Linux: /dev/video
       *                for Windows: not implemented yet.
       */
      std::string deviceFile;
    };

    /**
     * Default constructor
     */
    pwc();

    /**
     * Constructor with initialization for the given device
     */
    pwc(const std::string& device);

    /**
     * Copy constructor
     */
    pwc(const pwc& other);

    /**
     * Constructor, sets the parameters
     */
    pwc(const parameters& theParam);

    /**
     * Destructor
     */
    virtual ~pwc( void );

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
     * Get the current parameters from the camera
     * (the camera device is the one who is dumping data into the
     * given object.
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
    pwc& copy(const pwc& other);

    /**
     * Clone member
     */
    pwc* clone() const;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual pwc* newInstance() const;

    /**
     * Check if the frame grabber has been initialized
     */
    bool isInitialized() const;

    /**
     * Initialize "frame grabber".
     *
     */
    bool init();

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
     * Set whiteBalance control mode and register in camera directly.
     *
     * Depending on the color space used by the camera (RGB or YUV), the
     * color gains are applied to UV or to BR.
     *
     * @param mode Camera desired mode (@see eCamFeatureMode)

     * @param whiteBalance type of automatic white balance (unique for PWC)
     *                     to be used (Outdoor, indoor, fluorescent
     *                     illumination, etc.)
     * @param ubGain gain for the blue channel (or U channel)
     * @param vrGain gain for the red channel (or V channel)
     *
     * @return true if operation was successfull, false otherwise
     */
    bool setWhiteBalance(const eCamFeatureMode mode,
                         const eWhiteBalanceType whiteBalance,
                         const float ubGain=0,
                         const float vrGain=0);
    
    /**
     * Get whiteBalance mode and value directly out of camera.
     *
     * @param mode Variable where the current whiteBalance mode is to be 
     *              written.
     * @param whiteBalance type of automatic white balance mode being used.
     * @param ubGain gain for the blue channel (or U channel)
     * @param vrGain gain for the red channel (or V channel)
     *
     * @return true if operation was successfull, false otherwise
     */ 
    bool getWhiteBalance(eCamFeatureMode& mode,
                         eWhiteBalanceType& whiteBalance,
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
     *             written.
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
     * @param pan  If mode is manual, the value of pan to be used.
     * @param tilt If mode is manual, the value of tilt to be used.
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
     *                written.
     * @param pan Variable where the pan value is to be written.
     * @param tiltMode Variable where the current tilt mode is to be 
     *                 written.
     * @param tilt Variable where the tilt value is to be written.
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
     * The actual capture size for the images
     */
    ipoint resolution_;

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
     * Minimum pan value
     */
    int minPan_;

    /**
     * Maximum pan value
     */
    int maxPan_;
    
    /**
     * Minimum tilt value
     */
    int minTilt_;

    /**
     * Maximum tilt value
     */
    int maxTilt_;

    /**
     * Pan-Tilt unit supported
     */
    bool panTilt_;

    /**
     * Flag indicates if the camera interface has been initialized
     */
    bool initialized_;

    /**
     * Name of the camera being used
     */
    std::string camName_;

    /**
     * Type of camera detected (used to check available resolutions)
     */
    int camType_;

    /**
     * The camera file handle.
     * this is valid only if <code>initialized</code> is true
     */
    int cameraHndl_;

    /**
     * Set the camera parameters.
     *
     * This method transfers the parameter values in the pwc::parameters
     * instance to the camera, but only those that have changed, to save some
     * USB bandwidth.
     */
    bool setCameraParameters();

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
     * Look-up table to accelerate computation of saturation
     */
    static const ubyte* lutSaturation_;

    /**
     * Look up tables to accelerate conversion Luv -> RGB
     */
    //@{
    static const int* lutUg_;
    static const int* lutUb_;
    static const int* lutVg_;
    static const int* lutVr_;
    //@}

    /**
     * Initialize the Look-Up-Tables
     */
    void initializeLUTs();
   
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
  };

  /**
   * Write pwc::eCompressionType
   */
  bool write(ioHandler& handler,const pwc::eCompressionType val);

  /**
   * Read pwc::eCompressionType
   */
  bool read(ioHandler& handler,pwc::eCompressionType& val);
  
  /**
   * Write pwc::eWhiteBalanceType
   */
  bool write(ioHandler& handler,const pwc::eWhiteBalanceType val);

  /**
   * Read pwc::eWhiteBalanceType
   */
  bool read(ioHandler& handler,pwc::eWhiteBalanceType& val);
  
  /**
   * Write pwc::eDenoiseType
   */
  bool write(ioHandler& handler,const pwc::eDenoiseType val);

  /**
   * Read pwc::eDenoiseType
   */
  bool read(ioHandler& handler,pwc::eDenoiseType& val);


}
#endif // __linux__
#endif // _USE_PWC
#endif // #ifdef _LTI_PWC_H_
