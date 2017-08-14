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
 * \file   ltiFireWireDCAM.h
 *         Contains a class for grabbing images from FireWire cameras.
 * \author Arnd Hannemann
 * \author Pablo Alvarado
 * \date   21.10.2004
 * $Id: ltiFireWireDCAM2.h,v 1.1 2012-01-05 18:32:32 alvarado Exp $
 */

#ifndef _LTI_FIRE_WIRE_DCAM_2_H_
#define _LTI_FIRE_WIRE_DCAM_2_H_

#include "ltiHardwareConfig.h"

#if defined(_USE_FIRE_WIRE_DCAM) && defined(_LTI_DC1394_VER_2)

#include <dc1394/dc1394.h>

#include <string>
#include <vector>
#include <set>
#include <map>
#include "ltiMergeYCbCrToImage.h"
#include "ltiBayerDemosaicing.h"
#include "ltiMutex.h"
#include "ltiCamFeatureMode.h"

#include "ltiCamera.h"
#include "ltiFrameGrabber.h"
#include "ltiPanTiltUnit.h"
#include "ltiLensUnit.h"

namespace lti {
  /**
   * Grab images from FireWire Cameras.
   *
   * This functor permits to interact with standard FireWire DCAM cameras (aka
   * IIDC v1.3x).  It is a wrapper of the libdc1394 library for (linux), 
   * version 2.  You may find the information of the IIDC standard useful,
   * which can be found on the net (for instance, in
   * http://damien.douxchamps.net/ieee1394/libdc1394/iidc_specifications.php ).
   *
   *  System requirements:
   *    - linux kernel modules ieee1394, ohci1394, raw1394 and video1394
   *    - libdc1394 (>= 2.x)
   *    - libraw1394 (>= 0.9.0)
   *
   *  Installation:
   *    -# compile or install above kernel modules for your kernel (usually
   *       they are in most distributions ready)
   *    -# create video devicefiles for video1394
   *       (e.g. /dev/video1394).  This is also usually automatically done.
   *    -# compile or install libdc1394 and libraw1394.  Most distributions
   *       provide packages for them.  Do not forget to install the "dev" or
   *       "devel" packages of the library as well.
   *    -# the libraries above are now automatically detected by the configure
   *       script.  Therefore, you do not need to edit extrapaths.mk anymore.
   *       However, if you changed the availability, you will require to run
   *       configure again.
   *    -# compile ltilib
   *    -# all kernel modules must be loaded before running any code that uses
   *       this class.
   *    -# Ensure that your user has rights on /dev/video1394/ and /dev/raw1394
   *       what it is usually achieved making your user member of the video
   *       group for the former. For the latter it is up to you how to provide
   *       that right.
   *
   * Example:
   *  \code
   *    // viewer to display taken shot
   *    viewer view("FireWireDCAM");
   * 
   *    fireWireDCAM::parameters camPar;
   *    fireWireDCAM cam;

   *    // will be preserved by init()
   *    camPar.deviceFile = "/dev/video1394/0";
   *    camPar.snapShotMode = true;
   *    camPar.brightness = 22;
   *
   *    // as the camera is initialized now, the new parameters will be written
   *    // into the camera
   *    cam.setParameters(camPar);
   *    // take picture
   *    image img;
   *    cam.apply(img);
   *
   *    // show image
   *    view.show(img);
   *
   *  \endcode
   *
   *  Not yet implemented:
   *  - format7 (scalable image)
   *  - color modes: Mono16, RGB, YUV444
   *  - some camera features: (see fireWireDCAM::parameters)
   *
   *  Other things to do:
   *  - get feature constraints
   *  - it would be much better to use a wrapper class instead of including
   *    raw1394.h and dc1394_control.h directly (namespace)
   *  - better error handling (e.g. remember setup_capture failures etc..)
   *
   * \warning Even if some code has been developed for support of
   * one-push and format7 features, we haven't got a camera that
   * supports them and therefore that code may not work.  Please
   * report any problems, or even better, submit a patch with fixes if
   * they are necessary.
   *
   * @ingroup gAcquisition
   */
  class fireWireDCAM : public frameGrabber,
                       public camera, 
                       public panTiltUnit,
                       public lensUnit {
  public:
    // ------------------------------------------------
    // enumerations
    // ------------------------------------------------
    
    /**
     * Feature IDs.
     *
     * These IDs have been selected based on the IIDC standard
     */
    enum {
      FeatureTemperature=427,    /**< Temperature   */
      FeatureWhiteShading=430,   /**< White shading */

      FeatureFrameRate=431,      /**< Frame rate: how to solve conflicts
                                  *   between frame rate and shutter 
                                  */

      FeatureCaptureSize=436,    /**< Quality aspects for format 6(still img)*/
      FeatureCaptureQuality=437
    };


    /**
     * Auto correction modes for parameters
     *
     * While setting the parameters for a camera, you can decide how to behave
     * on invalid parameters.
     */
    enum eFixMode {
      NoFix,   /**< Use the parameters as they are given, and if something is
                    invalid, return an error */
      AutoFix, /**< Try to use the parameters as they are given, but if
                    something is invalid or not supported by the active camera,
                    change it to a valid configuration */
      Upload   /**< Ignore the given parameter value, and simply get from
                    the active camera device the corresponding value in use. */
    };

    /**
     * Enumeration of encoding methods allowed by the IIDC standard of the IEEE
     * 1394.  Many of these constants are available for Format7 modes only,
     * i.e., if the camera supports scalable image sizes.
     *
     * Some of the following encoding methods are not supported by the LTI-Lib
     * yet, specially those with 16 bits.  Hence, the equivalent formats with
     * lower precision would be used instead.
     */
    enum eColorMode {
      Mono8 = 0, /**< Monochromatic with 8 bits (unsigned) per pixel */
      Mono16,    /**< Monochromatic with 16 bits (unsigned) per pixel */
      Mono16s,   /**< Monochromatic with 16 bits (signed) per pixel */
      YUV411,    /**< Yuv color space with 4:1:1 ratio */
      YUV422,    /**< Yuv color space with 4:2:2 ratio */
      YUV444,    /**< Yuv color space with 4:4:4 ratio */
      RGB8,      /**< RGB encoding with 8 bits per color channel */
      RGB16,     /**< RGB encoding with 16 bits (unsigned) per color channel */
      RGB16s,    /**< RGB encoding with 16 bits (signed) per color channel */
      Raw8,      /**< Raw data in 8 bits */
      Raw16,     /**< Raw data in 16 bits */
      UnknownEncoding   /**< Dummy value for an unknown encoding */
    };

    /**
     * The parameters for the class fireWireDCAM.
     *
     * Many of the attributes are inherited from the "almost" interfaces
     * lti::panTilt::parameters, lti::lensUnit::parameters and mainly
     * lti::camera::parameters.
     *
     * For this particular functor, you have to consider the following aspects.
     *
     * \section resolution Resolution
     *
     * The image size to be acquired is given, as expecten in the inherited
     * attribute \c lti::camera::parameters::resolution.
     *
     * The standard IIDC establishes a finite set of valid resolutions for
     * IEEE1394 cameras, and groups them in so called \e formats.  These
     * formats have the following names:
     * 
     * - Format 0: VGA non compressed, for resolutions less than or equal
     *   to 640x480
     * - Format 1: SVGA non compressed, for resolutions greater than Format 0
     *   and less than or equal to 1024x768
     * - Format 2: SVGA non compressed, for resolutions greater than Format 1
     *   and less than or equal to 1600x1200
     * - Format 6: Still image
     * - Format 7: Scalable image size
     *
     * The set of accepted image sizes is
     *
     * - 160x120   (format 0)
     * - 320x240   (format 0)
     * - 640x480   (format 0)
     * - 800x600   (format 1)
     * - 1024x768  (format 1)
     * - 1280x960  (format 2)
     * - 1600x1200 (format 2)
     *
     * For each format, only a subset of valid pixel encodings are valid.
     * (see the documentation of attribute \c parameters::encoding for a
     * detailed list).
     *
     * Default: 640x480
     *
     * \section frameRate Frame rate
     *
     * The desired framerate is given through the inherited attribute
     * lti::camera::parameters::framerate.
     *
     * The IIDC standard provides additional features (not implemented
     * yet here) to control what do you want to give priority, this
     * frame-rate setting, or the shutter and other exposure settings.
     *
     * Only a finite set of framerates are allowed in the IIDC:
     * - 1.875
     * - 3.75
     * - 7.5
     * - 15
     * - 30
     * - 60
     * - 120
     * - 240
     *
     * If you give another value then the nearest one will be used.
     * If the nearest value of the above list is not supported by the
     * current camera, then the behaviour will depend on \c autofix.
     * 
     * \section camFeats Camera features
     *
     * Each particular camera usually supports just a subset of all
     * available features.  You can use the method
     * lti::fireWireDCAM::isFeatureAvailable() to check if a
     * particular feature is supported by your camera or not.
     *
     * The attributes with the word "Mode" control the "mode" (or as called in
     * the IIDC standard, the "state") in which the feature is to be used (see
     * the IIDC 1.3x specifications for details, which can be found at
     * http://damien.douxchamps.net/ieee1394/libdc1394/iidc_specifications.php
     * See also the documentation of the type lti::eCamFeatureMode.
     *
     * Not all cameras support all possible modes for a particular feature.
     * You can use the method lti::fireWireDCAM::checkFeatureModes() to test
     * the camera support for the on-off, auto, manual, one-push and absolute
     * mode (i.e. states).
     *
     * If a camera supports the one-push state, then the corresponding
     * values for the feature will be automatically adjusted exaclty
     * once, just when you set the parameters or update them.  Then
     * the camera automatically returns into the manual state.  You
     * can use the dumpParameters() method to obtain the adjusted
     * values.
     *
     * Please note that the LTI-Lib uses float values for the features
     * to cover directly the values that a "absolute-setting capable"
     * camera would admit.
     *
     * The supported features of the IIDC specification are:
     * 
     * - DC1394_FEATURE_BRIGHTNESS
     * - DC1394_FEATURE_EXPOSURE
     * - DC1394_FEATURE_SHARPNESS
     * - DC1394_FEATURE_WHITE_BALANCE
     * - DC1394_FEATURE_HUE
     * - DC1394_FEATURE_SATURATION
     * - DC1394_FEATURE_GAMMA
     * - DC1394_FEATURE_SHUTTER
     * - DC1394_FEATURE_GAIN
     * - DC1394_FEATURE_IRIS
     * - DC1394_FEATURE_FOCUS
     * - DC1394_FEATURE_ZOOM
     * - DC1394_FEATURE_PAN
     * - DC1394_FEATURE_TILT
     *
     * The yet unsupported features are (patches to add support for these
     * missing features are always welcome).
     *
     * - DC1394_FEATURE_TEMPERATURE
     * - DC1394_FEATURE_TRIGGER
     * - DC1394_FEATURE_TRIGGER_DELAY
     * - DC1394_FEATURE_WHITE_SHADING
     * - DC1394_FEATURE_FRAME_RATE
     * - DC1394_FEATURE_OPTICAL_FILTER
     * - DC1394_FEATURE_CAPTURE_SIZE
     * - DC1394_FEATURE_CAPTURE_QUALITY
     */
    class parameters : public frameGrabber::parameters,
                       public camera::parameters,
                       public panTiltUnit::parameters, 
                       public lensUnit::parameters {
    public:
 
      /**
       * Default constructor
       */
      parameters();

      /**
       * Copy constructor
       * @param other the parameters object to be copied
       */
      parameters(const parameters& other);

      /**
       * Destructor
       */
      ~parameters();

      /**
       * Returns the name of this class
       */
      virtual const std::string& name() const;

      /**
       * Copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& copy(const parameters& other);

      /**
       * Copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& operator=(const parameters& other);

      /**
       * Returns a pointer to a clone of the parameters.
       */
      virtual parameters* clone() const;

      /**
       * Returns a pointer to a clone of the parameters.
       */
      virtual parameters* newInstance() const;

      /**
       * Write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool write(ioHandler& handler,const bool complete=true) const;

      /**
       * Read the parameters from the given ioHandler
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
       * Indicate how to behave if invalid parameters are given.
       *
       * You can choose if you want your parameters to be taken exactly as they
       * are given, and if there is an invalid configuration to report an error
       * (NoFix).  You can also indicate that those invalid parameters should
       * be automatically modified to valid ones (AutoFix).  A third
       * possibility is to upload the configuration within the physical camera
       * exactly as they are, overwriting everything is given here.
       *
       * Default value: Upload
       */
      eFixMode fixMode;

      /**
       * Pixel color encoding.
       *
       * Each resolution in IIDC standard supports only a subset of the
       * possible encodings and each camera usually supports just a subset of
       * those.
       * 
       * The following table summarizes the possible combinations of
       * resolutions and encodings (red means "not allowed", F# indicates 
       * the format #).
       *
       * <table align="center" frame="box" border=1>
       *   <tr>
       *     <td></td>
       *     <td>Mono8</td>
       *     <td>Mono16</td>
       *     <td>YUV411</td>
       *     <td>YUV422</td>
       *     <td>YUV444</td>
       *     <td>RGB8</td>
       *     <td>RGB16</td>
       *   </tr>
       *   <tr>
       *     <td>160x120</td>
       *     <td bgcolor="red"></td>
       *     <td bgcolor="red"></td>
       *     <td bgcolor="red"></td>
       *     <td bgcolor="red"></td>
       *     <td bgcolor="gray">F0</td>
       *     <td bgcolor="red"></td>
       *     <td bgcolor="red"></td>
       *   </tr>
       *   <tr>
       *     <td>320x240</td>
       *     <td bgcolor="red"></td>
       *     <td bgcolor="red"></td>
       *     <td bgcolor="red"></td>
       *     <td bgcolor="gray">F0</td>
       *     <td bgcolor="red"></td>
       *     <td bgcolor="red"></td>
       *     <td bgcolor="red"></td>
       *   </tr>
       *   <tr>
       *     <td>640x480</td>
       *     <td bgcolor="gray">F0</td>
       *     <td bgcolor="gray">F0</td>
       *     <td bgcolor="gray">F0</td>
       *     <td bgcolor="gray">F0</td>
       *     <td bgcolor="red"></td>
       *     <td bgcolor="gray">F0</td>
       *     <td bgcolor="red"></td>
       *   </tr>
       *   <tr>
       *     <td>800x600</td>
       *     <td bgcolor="darkGray">F1</td>
       *     <td bgcolor="darkGray">F1</td>
       *     <td bgcolor="red"></td>
       *     <td bgcolor="darkGray">F1</td>
       *     <td bgcolor="red"></td>
       *     <td bgcolor="darkGray">F1</td>
       *     <td bgcolor="red"></td>
       *   </tr>
       *   <tr>
       *     <td>1024x768</td>
       *     <td bgcolor="darkGray">F1</td>
       *     <td bgcolor="darkGray">F1</td>
       *     <td bgcolor="red"></td>
       *     <td bgcolor="darkGray">F1</td>
       *     <td bgcolor="red"></td>
       *     <td bgcolor="darkGray">F1</td>
       *     <td bgcolor="red"></td>
       *   </tr>
       *   <tr>
       *     <td>1280x960</td>
       *     <td bgcolor="lightGray">F2</td>
       *     <td bgcolor="lightGray">F2</td>
       *     <td bgcolor="red"></td>
       *     <td bgcolor="lightGray">F2</td>
       *     <td bgcolor="red"></td>
       *     <td bgcolor="lightGray">F2</td>
       *     <td bgcolor="red"></td>
       *   </tr>
       *   <tr>
       *     <td>1600x1200</td>
       *     <td bgcolor="lightGray">F2</td>
       *     <td bgcolor="lightGray">F2</td>
       *     <td bgcolor="red"></td>
       *     <td bgcolor="lightGray">F2</td>
       *     <td bgcolor="red"></td>
       *     <td bgcolor="lightGray">F2</td>
       *     <td bgcolor="red"></td>
       *   </tr>
       * </table>
       *
       * Default value: YUV422
       */
      eColorMode encoding;
       
      /**
       * DMA buffer size in frames.
       *
       * For DMA access provided by the video1394 library, a buffer ring of the
       * given number of images is used.  This means, the higher this number,
       * the higher the latency of the read images (i.e. the time-shift between
       * the image provided by the apply method and the actual time in which
       * that image was taken is much higher), as it will be the given number
       * of frames in the past.
       * 
       * Default: 2
       */
      int dmaBufferSize;

      /**
       * Whether frames will be dropped to keep down latency.
       *
       * This indicates what to do if the ring buffer is full: should the
       * last image there be kept until read (false), or should the buffer
       * be overwritten with the incomming images.
       *
       * Default: true
       */
      bool dropFrames;

      /**
       * Device file for video capture.
       * 
       * Please ensure that you have the proper permission to access that
       * device.
       *
       * Default: "/dev/video1394/0"
       */
      std::string deviceFile;

      /**
       * Name of desired demosaicing method in raw (mono) modes.
       *
       * This corresponds to the name of a class that inherits from the
       * lti::bayerDemosaicing abstract class and that has incribed itself into
       * the lti::factory<bayerDemosaicing>.
       *
       * Default: bilinearDemosaicing
       */      
      std::string bayerMethod;

      /**
       * Bayer pattern used in demosaicing functor.
       */
      bayerDemosaicing::eBayerPattern bayerPattern;

      /**
       * Camera names file.
       *
       * To provide an easy way to identify your cameras, this functor manages
       * a very simple table of camera names associated with a 64bit number.
       * The table is stored in the file with the name given here.  You can
       * provide an absolute path with the file if you want a system-wide
       * name convention for your cameras.
       *
       * If a new camera has been detected, and the \c cameraName is not
       * in the current table, then a new line will be added with that name.
       * If the file is in use, then a name "Camera-x" will be added with "x" a
       * sequence number.
       *
       * The first camera in the file will always be the default one used.
       *
       * Please note that this is a class attribute (a static attribute), which
       * means that all instances share the same file, since it makes sense
       * that all cameras can be addressed using the same names.
       *
       * Default value: "cameras.txt"
       */
      static std::string cameraNamesFile;

      /**
       * Camera name.
       *
       * The camera with the given name in the cameraNamesFile will be used.
       * If the name has not yet been registered in the cameraNamesFiles, then
       * the first available camera will be taken and asigned this \c
       * cameraName, which can be later modified by hand in the corresponding
       * names file.
       *
       * Default value: "Default"
       */
      std::string cameraName;

    protected:
      /**
       * Write the name of the encoding method into a string
       */
      const char* encodingName(const eColorMode& code) const;
      /**
       * Convert the name of an encoding type to the corresponding
       * enum value.
       */
      eColorMode getEncoding(const std::string& name) const;

      /**
       * Get a string for the fix mode
       */
      const char* fixModeName(const eFixMode& mode) const;

      /**
       * For the given string, return a valid fix mode 
       */
      eFixMode getFixMode(const std::string& name) const;

    };

    /**
     * Default constructor
     *
     * You usually avoid using this constructor.  Using fireWireDCAM(const
     * parameters&) you can provide directly for which camera the constructed
     * instance should work.
     */
    fireWireDCAM();

    /**
     * Construct with the given parameters instance.
     *
     * You can give directly in the parameters object for which camera
     * the instance should be created, therefore this is the most used
     * constructor of this functor.
     */
    fireWireDCAM(const parameters& par);


    /**
     * Copy constructor
     * @param other the object to be copied
     */
    fireWireDCAM(const fireWireDCAM& other);

    /**
     * Destructor
     */
    virtual ~fireWireDCAM();

    /**
     * Returns the name of this class.
     */
    virtual const std::string& name() const;
    
    /**
     * Gets one frame from camera.
     * @param dest image the result will be left here.
     * @return true if apply successful or false otherwise. 
     */
    bool apply(image& dest);

    /**
     * Gets one frame from camera.
     * @param dest channel8 the result be left here.
     * @return true if apply successful or false otherwise.
     */
    bool apply(channel8& dest);


    /**
     * Grabs a raw frame from camera.
     * @param dest channle8 where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    bool grabRaw(matrix<ubyte>& dest);

    /**
     * write parameters in camera
     */
    bool updateParameters();

    /**
     * @name Direct access to camera features.
     *
     * These methods access the camera registers directly, and they try to
     * update the internal parameters.  However, since the cameras may adjust
     * themselves, you may find the method fireWireDCAM::dumpParameters()
     * useful, which asks the camera for all parameters in use at once.
     *
     * If a feature supports the "one-push state", then the automatic
     * configuration will begin as soon as you call the corresponding method
     * with the value \c FeatureOnePush as state (for example
     * setWhiteBalance(FeatureOnePush) ).
     *
     * If you set the state of one or more features to one-push, then you may
     * want to wait for them to be ready with the method onePushWait().  Of
     * course, this will work if and only if the camera supports this feature
     * state.
     *
     * The methods to query information from the camera do indeed ask the
     * camera for the corresponding value (if and only if this is possible).
     * There are cameras which do not support the readout capability for the
     * registers holding the feature values.  In that case, the get* methods
     * will return \c false.
     */
    //@{

    /**
     * Set brightness control state and register in camera directly.
     *
     * @param state Camera desired state (@see eCamFeatureMode)
     * @param brightness If state is manual, the value to be used.
     *
     * @return true if operation was successfull, false otherwise
     */
    bool setBrightness(const eCamFeatureMode state,
                       const float brightness=0);

    /**
     * Get brightness state and value directly out of camera.
     *
     * @param state Variable where the current brightness state is to be 
     *              written.
     * @param brightness Variable where the brightness value is to be written.
     *
     * @return true if operation was successfull, false otherwise
     */ 
    bool getBrightness(eCamFeatureMode& state,
                       float& brightness) const;

    /**
     * Set exposure control state and register in camera directly.
     *
     * @param state Camera desired state (@see eCamFeatureMode)
     * @param exposure If state is manual, the value to be used.
     *
     * @return true if operation was successfull, false otherwise
     */
    bool setExposure(const eCamFeatureMode state,
                     const float exposure=0);

    /**
     * Get exposure state and value directly out of camera.
     *
     * @param state Variable where the current exposure state is to be 
     *              written.
     * @param exposure Variable where the exposure value is to be written.
     *
     * @return true if operation was successfull, false otherwise
     */ 
    bool getExposure(eCamFeatureMode& state,
                     float& exposure) const;

    /**
     * Set sharpness control state and register in camera directly.
     *
     * @param state Camera desired state (@see eCamFeatureMode)
     * @param sharpness If state is manual, the value to be used.
     *
     * @return true if operation was successfull, false otherwise
     */
    bool setSharpness(const eCamFeatureMode state,
                      const float sharpness=0);

    /**
     * Get sharpness state and value directly out of camera.
     *
     * @param state Variable where the current sharpness state is to be 
     *              written.
     * @param sharpness Variable where the sharpness value is to be written.
     *
     * @return true if operation was successfull, false otherwise
     */ 
    bool getSharpness(eCamFeatureMode& state,
                      float& sharpness) const;

    /**
     * Set whiteBalance control state and register in camera directly.
     *
     * Depending on the color space used by the camera (RGB or YUV), the
     * color gains are applied to UV or to BR.
     *
     * @param state Camera desired state (@see eCamFeatureMode)
     * @param ubGain gain for the blue channel (or U channel)
     * @param vrGain gain for the red channel (or V channel)
     *
     * @return true if operation was successfull, false otherwise
     */
    bool setWhiteBalance(const eCamFeatureMode state,
                         const float ubGain=0,
                         const float vrGain=0);
    
    /**
     * Get whiteBalance state and value directly out of camera.
     *
     * @param state Variable where the current whiteBalance state is to be 
     *              written.
     * @param ubGain gain for the blue channel (or U channel)
     * @param vrGain gain for the red channel (or V channel)
     *
     * @return true if operation was successfull, false otherwise
     */ 
    bool getWhiteBalance(eCamFeatureMode& state,
                         float& ubGain,
                         float& vrGain) const;

    /**
     * Set hue control state and register in camera directly.
     *
     * @param state Camera desired state (@see eCamFeatureMode)
     * @param hue If state is manual, the value to be used.
     *
     * @return true if operation was successfull, false otherwise
     */
    bool setHue(const eCamFeatureMode state,
                const float hue=0);

    /**
     * Get hue state and value directly out of camera.
     *
     * @param state Variable where the current hue state is to be 
     *              written.
     * @param hue Variable where the hue value is to be written.
     *
     * @return true if operation was successfull, false otherwise
     */ 
    bool getHue(eCamFeatureMode& state,
                float& hue) const;

    /**
     * Set saturation control state and register in camera directly.
     *
     * @param state Camera desired state (@see eCamFeatureMode)
     * @param saturation If state is manual, the value to be used.
     *
     * @return true if operation was successfull, false otherwise
     */
    bool setSaturation(const eCamFeatureMode state,
                       const float saturation=0);

    /**
     * Get saturation state and value directly out of camera.
     *
     * @param state Variable where the current saturation state is to be 
     *              written.
     * @param saturation Variable where the saturation value is to be written.
     *
     * @return true if operation was successfull, false otherwise
     */ 
    bool getSaturation(eCamFeatureMode& state,
                       float& saturation) const;
    
    /**
     * Set gamma control state and register in camera directly.
     *
     * @param state Camera desired state (@see eCamFeatureMode)
     * @param gamma If state is manual, the value to be used.
     *
     * @return true if operation was successfull, false otherwise
     */
    bool setGamma(const eCamFeatureMode state,
                  const float gamma=0);

    /**
     * Get gamma state and value directly out of camera.
     *
     * @param state Variable where the current gamma state is to be 
     *              written.
     * @param gamma Variable where the gamma value is to be written.
     *
     * @return true if operation was successfull, false otherwise
     */ 
    bool getGamma(eCamFeatureMode& state,
                  float& gamma) const;

    /**
     * Set shutter control state and register in camera directly.
     *
     * @param state Camera desired state (@see eCamFeatureMode)
     * @param shutter If state is manual, the value to be used.
     *
     * @return true if operation was successfull, false otherwise
     */
    bool setShutter(const eCamFeatureMode state,
                    const float shutter=0);

    /**
     * Get shutter state and value directly out of camera.
     *
     * @param state Variable where the current shutter state is to be 
     *              written.
     * @param shutter Variable where the shutter value is to be written.
     *
     * @return true if operation was successfull, false otherwise
     */ 
    bool getShutter(eCamFeatureMode& state,
                    float& shutter) const;

    /**
     * Set gain control state and register in camera directly.
     *
     * @param state Camera desired state (@see eCamFeatureMode)
     * @param gain If state is manual, the value to be used.
     *
     * @return true if operation was successfull, false otherwise
     */
    bool setGain(const eCamFeatureMode state,
                 const float gain=0);

    /**
     * Get gain state and value directly out of camera.
     *
     * @param state Variable where the current gain state is to be 
     *              written.
     * @param gain Variable where the gain value is to be written.
     *
     * @return true if operation was successfull, false otherwise
     */ 
    bool getGain(eCamFeatureMode& state,
                 float& gain) const;

    /**
     * Set iris control state and register in camera directly.
     *
     * @param state Camera desired state (@see eCamFeatureMode)
     * @param iris If state is manual, the value to be used.
     *
     * @return true if operation was successfull, false otherwise
     */
    bool setIris(const eCamFeatureMode state,
                 const float iris=0);

    /**
     * Get iris state and value directly out of camera.
     *
     * @param state Variable where the current iris state is to be 
     *              written.
     * @param iris Variable where the iris value is to be written.
     *
     * @return true if operation was successfull, false otherwise
     */ 
    bool getIris(eCamFeatureMode& state,
                 float& iris) const;

    /**
     * Set focus control state and register in camera directly.
     *
     * @param state Camera desired state (@see eCamFeatureMode)
     * @param focus If state is manual, the value to be used.
     *
     * @return true if operation was successfull, false otherwise
     */
    bool setFocus(const eCamFeatureMode state,
                  const float focus=0);

    /**
     * Get focus state and value directly out of camera.
     *
     * @param state Variable where the current focus state is to be 
     *              written.
     * @param focus Variable where the focus value is to be written.
     *
     * @return true if operation was successfull, false otherwise
     */ 
    bool getFocus(eCamFeatureMode& state,
                  float& focus) const;

    /**
     * Set zoom control state and register in camera directly.
     *
     * @param state Camera desired state (@see eCamFeatureMode)
     * @param zoom If state is manual, the value to be used.
     *
     * @return true if operation was successfull, false otherwise
     */
    bool setZoom(const eCamFeatureMode state,
                 const float zoom=0);

    /**
     * Get zoom state and value directly out of camera.
     *
     * @param state Variable where the current zoom state is to be 
     *              written.
     * @param zoom Variable where the zoom value is to be written.
     *
     * @return true if operation was successfull, false otherwise
     */ 
    bool getZoom(eCamFeatureMode& state,
                 float& zoom) const;

    /**
     * Set pan control state and register in camera directly.
     *
     * @param state Camera desired state (@see eCamFeatureMode)
     * @param pan If state is manual, the value to be used.
     *
     * @return true if operation was successfull, false otherwise
     */
    bool setPan(const eCamFeatureMode state,
                const float pan=0);

    /**
     * Get pan state and value directly out of camera.
     *
     * @param state Variable where the current pan state is to be 
     *              written.
     * @param pan Variable where the pan value is to be written.
     *
     * @return true if operation was successfull, false otherwise
     */ 
    bool getPan(eCamFeatureMode& state,
                float& pan) const;

    /**
     * Set tilt control state and register in camera directly.
     *
     * @param state Camera desired state (@see eCamFeatureMode)
     * @param tilt If state is manual, the value to be used.
     *
     * @return true if operation was successfull, false otherwise
     */
    bool setTilt(const eCamFeatureMode state,
                 const float tilt=0);

    /**
     * Get tilt state and value directly out of camera.
     *
     * @param state Variable where the current tilt state is to be 
     *              written.
     * @param tilt Variable where the tilt value is to be written.
     *
     * @return true if operation was successfull, false otherwise
     */ 
    bool getTilt(eCamFeatureMode& state,
                 float& tilt) const;

    /**
     * Set pan control mode and register in device directly.
     *
     * @param mode Device desired mode (@see eCamFeatureMode)
     * @param pan If mode is manual, the value to be used for pan.
     * @param tilt If mode is manual, the value of tilt.
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

    /**
     * Set optical filter control state and register in camera directly.
     *
     * @param state Camera desired state (@see eCamFeatureMode)
     * @param opticalFilter If state is manual, the value to be used.
     *
     * @return true if operation was successfull, false otherwise
     */
    bool setOpticalFilter(const eCamFeatureMode state,
                          const float opticalFilter=0);

    /**
     * Get optical filter state and value directly out of camera.
     *
     * @param state Variable where the current opticalFilter state is to be 
     *              written.
     * @param opticalFilter Variable where the opticalFilter value is to be 
     *                      written.
     *
     * @return true if operation was successfull, false otherwise
     */ 
    bool getOpticalFilter(eCamFeatureMode& state,
                          float& opticalFilter) const;
    //@}
    
    /**
     * Get current video mode directly out of camera.
     *
     * The IIDC enum value read from the camera is decoded into a
     * resolution and a pixel encoding value.
     *
     * \param resolution ipoint with the image resolution.
     * \param encoding encoding type of each pixel (YUV422, RGB, etc.)
     *
     * @return true if operation was successfull, false otherwise
     */ 
    bool getVideoMode(ipoint& resolution,eColorMode& encoding) const;

    /**
     * Get current frame rate directly out of camera.
     * @return true if operation was successfull, false otherwise
     */
    bool getFramerate(float& framerate) const;

    /**
     * Get current resolution, posibly directly out of the camera
     */
    virtual bool getResolution(ipoint& resolution) const;

    /**
     * Set frame rate directly to the camera.
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
     * Member function that prints informations about camera features on
     * stdout.
     * @return true if operation was successfull, false otherwise
     */
    bool printFeatures() const;

    /**
     * Read the actual hardware settings of the \b active camera and leave them
     * into the given parameter instance.
     *
     * @param par the parameters instance on which the gained information
     * will be stored.
     *
     * @return true if operation was successfull, false otherwise (for
     * instance, if there is no active camera to be read)
     */
    bool dumpParameters(parameters &par);

    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    fireWireDCAM& copy(const fireWireDCAM& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    fireWireDCAM& operator=(const fireWireDCAM& other);

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual fireWireDCAM* clone() const;

    /**
     * Create new instance
     */ 
    virtual fireWireDCAM* newInstance() const;

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * \name Access to internal data structures
     *
     * The following methods can be found useful by GUI programmers who want
     * to ask for availability of several features in order to set-up properly
     * GUI elements.
     */
    //@{
    /**
     * Check if a camera has been activated for this functor.
     *
     * @return true if a camera has been successfully activated, or false
     * otherwise.
     */
    virtual bool isActive() const;

    /**
     * Check if the active camera supports format 7
     */
    bool hasCameraFormat7() const;

    /**
     * Check if a particular feature is available in the current active
     * camera.
     *
     * You have to ensure that a camera is already active (see
     * isActive()).
     *
     * \param featureId The identification of the feature expects one of the
     *                  following values, defined in the libdc1394:
     *                  - FEATURE_BRIGHTNESS
     *                  - FEATURE_EXPOSURE
     *                  - FEATURE_SHARPNESS
     *                  - FEATURE_WHITE_BALANCE
     *                  - FEATURE_HUE
     *                  - FEATURE_SATURATION
     *                  - FEATURE_GAMMA
     *                  - FEATURE_SHUTTER
     *                  - FEATURE_GAIN
     *                  - FEATURE_IRIS
     *                  - FEATURE_FOCUS
     *                  - FEATURE_TEMPERATURE
     *                  - FEATURE_TRIGGER
     *                  - FEATURE_TRIGGER_DELAY
     *                  - FEATURE_WHITE_SHADING
     *                  - FEATURE_FRAME_RATE
     *                  - FEATURE_ZOOM
     *                  - FEATURE_PAN
     *                  - FEATURE_TILT
     *                  - FEATURE_OPTICAL_FILTER
     *                  - FEATURE_CAPTURE_SIZE
     *                  - FEATURE_CAPTURE_QUALITY
     */
    bool isFeatureAvailable(const unsigned int featureId) const;

    /**
     * Check if a feature is available and is read-out capable.
     *
     * \param featureId identification for the feature to be queried.
     *                  (see fireWireDCAM::isFeatureAvailable() for a list of
     *                   valid values)
     * \return \c true if feature is read-out capable or \c false if it is not
     *         available at all, or if it is not readable.
     */
    bool isFeatureReadable(const unsigned int featureId) const;

    /**
     * Check for availability of feature states
     *
     * Following the IIDC 1.3x standard, a given feature (see the list of
     * valid identifiers in lti::fireWireDCAM::isFeatureAvailable() ) can
     * have one of four states:
     * - Off state: the feature is deactivated (some features cannot be
     *              deactivated).
     * - Auto state: the feature is automatically adjusted
     * - Manual state: the user controls the values a feature can get
     * - One-Push state: the feature is automatically adjusted but only once.
     *
     * This method asks the active camera which states are supported and
     * returns a bitwise combination of fireWireDCAM::eCamFeatureMode constants.
     *
     * You have to ensure that a camera is active (see
     * fireWireDCAM::isActive()).
     *
     * \param featureId identification for the feature to be modified
     *                  (see fireWireDCAM::isFeatureAvailable() for a list of
     *                   valid values)
     *
     */
    unsigned int checkFeatureModes(const unsigned int featureId) const;

    /**
     * Get feature state of the camera
     *
     * @return \c true if feature state could be successfully read, or \c false
     *         if this was not possible (maybe the camera does not support
     *         the given feature at all).
     */
    bool getFeatureMode(const unsigned int featureId,
                         eCamFeatureMode& state) const;

    /**
     * Set feature state of the camera
     *
     * @return \c true if feature state could be successfully set, or \c false
     *         if this was not possible (maybe the camera does not support
     *         the given state for the feature.
     */
    bool setFeatureMode(const unsigned int featureId,
                         const eCamFeatureMode state);

    /**
     * Set control state and value of a particular feature.
     *
     * This method does not support those features that require two values,
     * like white balance.
     *
     * This private method does not modify the internal parameters.
     *
     * @param featureId identification for the feature to be modified
     *                  (see fireWireDCAM::isFeatureAvailable() for a list of
     *                   valid values)
     * @param state Camera desired state (@see eCamFeatureMode)
     * @param value If state is manual, the value to be used.
     *
     * @return true if operation was successfull, false otherwise
     */
    inline bool setFeatureValue(const unsigned int featureId,
                                const eCamFeatureMode state,
                                const float value=0);
    
    /**
     * Get control state and value of a particular feature.
     *
     * This method does not support those features that require two values,
     * like white balance.
     *
     * @param featureId identification for the feature to be queried.
     * @param state Mode in which the feature is being used
     * @param value if state is manual, the value to be used.
     *
     * @return \c true if operation was successfull, \c false otherwise (for
     *         example, if the feature is not supported by the camera).
     */
    inline bool getFeatureValue(const unsigned int featureId,
                                eCamFeatureMode& state,
                                float& value) const;

    /**
     * Check the valid value range for a specified feature.
     *
     * You have to ensure that there is an active camera before calling this
     * method. (see fireWireDCAM::isActive() )
     *
     * @param featureId identification for the feature to be modified
     *                  (see fireWireDCAM::isFeatureAvailable() for a list of
     *                   valid values)
     * @param minValue minimum value accepted for the specified feature.
     * @param maxValue maximum value accepted for the specified feature.
     *
     * @return \c true if successful or \c false otherwise.
     */
    bool checkFeatureValueRange(const unsigned int featureId,
                                float& minValue,
                                float& maxValue) const;
    //@}

    /**
     * Return the active camera name.
     *
     * You have to check first that the camera is active, or this method
     * will return non-sense.
     *
     * Note that this method does not return a reference, but a new string,
     * since it may build the camera name on-the-go.
     *
     * This method is intended for user interfaces, rather than some
     * kind of string-based checking of the camera being used.
     * Therefore, do not rely on the output of this string for
     * detecting if a camera is or not the one you are looking for.
     */
    virtual const std::string cameraName() const;

  protected:
    /**
     * Returns modifieable parameters
     */
    parameters& getRWParameters();

    /**
     * For a bitwise combination of available states and a desired state,
     * return a valid supported state.
     */
    eCamFeatureMode fixMode(const unsigned int states,
                           const eCamFeatureMode desired) const;

    void fixFeature(const unsigned int featureId,
                    float& value,
                    eCamFeatureMode& state) const;
      /**
     * Analyse the given parameters instance and change its attributes to
     * contain values supported by the active camera.
     *
     * This method will work only if the camera of the given parameters is
     * connected and found.  Otherwise there will be no way to determine
     * which values are appropriate for the camera.
     */
    bool fixParameters(parameters& par) const;

    /**
     * Get the closest framerate of the libdc1394 set of enums to the
     * given framerate value coded in a float of fps.
     *
     * This method does not consider the camera supported framerates, but
     * the simple convertion.
     */
    dc1394framerate_t convertFramerate(const float fps) const;

    /**
     * Get the float value in fps equivalent to the framerate enum value
     * of the libdc1394
     */ 
    float convertFramerate(const dc1394framerate_t eFR) const;

    /**
     * Get "nearest" IIDC mode value from the given resolution and encoding.
     * 
     * As "nearest" it will be taken the resolution with the
     * most similar number of pixels and within that resolution, the "nearest"
     * encoding will be chosen (RGBs are in a group, Mono in another and YUV
     * in another.
     * 
     */
    dc1394video_mode_t convertMode(const ipoint& resolution,
                                   const eColorMode& encoding) const;

    /**
     * From the given value for format/mode after IIDC encoding, get
     * the equivalent resolution and pixel encoding.
     */
    void convertMode(const dc1394video_mode_t value,
                     ipoint& resolution,
                     eColorMode& encoding) const;

    /**
     * For a mode of the formats 0, 1 or 2, indicate if it is a color
     * mode.
     * 
     * @return \c true if the mode is a color mode (RGB or YUV), or \c false
     *         if the mode is monochrome.
     */
    bool isModeColor(const dc1394video_mode_t mode) const;
 
    /**
     * Compute the closest supported mode to a given IIDC valid mode.
     *
     * the approximation is done with the following priorities:
     *  
     * -# keep the mono or color attribute (unless your camera is
     *    monochrome, of course)
     * -# select the closest resolution.
     */
    dc1394video_mode_t computeClosestMode(const dc1394video_mode_t mode) const;

    /**
     * Compute the closest supported framerate for a given mode
     */
    float computeClosestFrameRate(const dc1394video_mode_t mode,
                                  const float fr) const;

    /**
     * From the given value for format7/mode after IIDC encoding, get
     * the equivalent resolution and pixel encoding.
     *
     * This method ask only for format 7 resolutions and encodings.  For this
     * task it has to ask the camera for that information.
     *
     * The resolution returned will be the largest the camera supports.
     */
    void convertMode7(const dc1394video_mode_t value,
                     ipoint& resolution,
                     eColorMode& encoding) const;

    /**
     * Get a functor valid code for a color space given in the libdc1394
     * enumeration for a Format7 color code.
     */
    eColorMode getColorCode7(const dc1394color_coding_t code) const;

    /**
     * Get the corresponding format for the given resolution
     */
    // unsigned int getFormat(const ipoint& resolution) const;


    /**
     * Converts YUV411 to RGB
     */
    void yuv411toRGB(dc1394video_frame_t *frame,image& srcdest) const;

    /**
     * Converts YUV411 to channel8.
     *
     * This method discards all color information, rescuing the y channel
     * only
     */
    void yuv411toChannel8(dc1394video_frame_t *frame,channel8& srcdest) const;

    /**
     * Converts YUV422 to RGB
     */
    void yuv422toRGB(dc1394video_frame_t *frame,image& srcdest) const;

    /**
     * Converts YUV422 to channel8
     *
     * This method discards all color information, rescuing the y channel
     * only
     */
    void yuv422toChannel8(dc1394video_frame_t *frame,channel8& srcdest) const;

    /**
     * Check if a specific feature is set to "auto"
     *
     * See lti::fireWireDCAM::isFeatureAvailable() for a list of the valid
     * \c featureId and their meanings.
     *
     * \param featureId identification for the feature to be modified
     *                  (see fireWireDCAM::isFeatureAvailable() for a list of
     *                   valid values)
     *
     * @return \c true if the feature is set to "auto" or false otherwise.
     */
    bool isFeatureAuto(const unsigned int featureId) const;

    /**
     * Set a feature to "auto"
     *
     * @param featureId identification for the feature to be modified
     *                  (see fireWireDCAM::isFeatureAvailable() for a list of
     *                   valid values)
     * @param on (default true) if \c true, the auto state is activated.
     *              If \c false, the auto state is turned off.
     */
    bool setFeatureAuto(const unsigned int featureId, 
                        const bool on=true);

    /**
     * Turn a feature on or off.
     *
     * @param featureId identification for the feature to be modified
     *                  (see fireWireDCAM::isFeatureAvailable() for a list of
     *                   valid values)
     * @param on if \c true the feature is turned on, if \c false then it is
     *           turned off
     *
     * @return \c true if feature state could be modified, of false otherwise.
     */
    bool setFeatureOn(const unsigned int featureId,
                      const bool on=true);

    /**
     * Turn a feature absolute state on or off
     *
     * @param featureId identification for the feature to be modified
     *                  (see fireWireDCAM::isFeatureAvailable() for a list of
     *                   valid values)
     * @param on if \c true the absolute mode is turned on, if \c false then 
     *           it is turned off
     *
     * @return \c true if feature state could be modified, of false otherwise.
     */
    bool setFeatureAbsoluteMode(const unsigned int featureId,
                                const bool on=true);

    /**
     * Just capture a frame through libdc1394
     */
    bool captureFrame(dc1394video_frame_t*& frame);

    /**
     * @name Initialization methods and attributes for the library libdc1394
     *
     * Methods for the libdc1394 version 2
     *
     * The following methods are used in the initialization of the IEEE1394 bus
     * and cameras.  They are based on the code of "coriander"
     * (http://damien.douxchamps.net/ieee1394/coriander/index.php)
     */
    //@{

    /**
     * Structure to hold format 7 information
     */
    struct format7Info {

      /**
       * Set of modes
       */
      dc1394format7modeset_t modeSet;

      /**
       * Flag set to true if any format 7 is supported 
       */
      bool supported;
  
      int scalePosXHandle;
      int scalePosYHandle;
      int scaleSizeXHandle;
      int scaleSizeYHandle;
    };


    /**
     * The camera information class
     *
     * For each camera detected in the bus a set of information is extracted
     * which is encapsulated in objects of this type.
     *
     * This class is equivalent to coriander's camera_t
     */
    class cameraInfo {
    public:
      /**
       * Default constructor
       */
      cameraInfo();

      /**
       * Destructor
       */
      ~cameraInfo();

      /**
       * Information about the camera
       */
      dc1394camera_t* info;
      
      /**
       * Set of features of the current camera
       */
      dc1394featureset_t featureSet;

      /**
       * Information about format 7
       */
      format7Info format7;

      /**
       * Mode in use.  
       *
       * This should be set externally to one of the modes supported and
       * indicated in supportedModes
       */
      dc1394video_mode_t videoMode;

      /**
       * Framerate in use
       */
      dc1394framerate_t framerate;

      /**
       * Flag to indicate if this camera is already being used by an instance
       */
      bool inUse;
      
      /**
       * Error string
       */
      std::string errorStr;

      /**
       * Store the modes supported.
       *
       * This information is generated when get() is called.
       */
      std::set<dc1394video_mode_t> supportedModes;

      /**
       * Store the framerates supported for each supported mode.
       *
       * The map key is a given mode, using the IIDC value for them, and the
       * value for that key is a set containing the framerates supported.
       *
       * This information is generated when get() is called.
       */
      std::map<dc1394video_mode_t,std::set<float> > supportedFramerates;

      /**
       * Get camera information.
       * @param dc1394 handler of dc1394 library
       * @param guid   camera id
       * \return true if successful or false if an error occurred,
       *          in which case the errorStr will contain an appropriate
       *          message.
       */
      bool init(dc1394_t *dc1394, uint64_t guid);

      /**
       * Free all allocated resources of this camera
       */
      void free();

    protected:
      /**
       * Get information about the format 7 capabilities
       */
      bool getFormat7Capabilities();

      /**
       * Get information on the supported modes.
       *
       * \return true if successful or false otherwise
       */
      bool getSupportedModesAndFramerates();
    };


    /**
     * Set of cameras
     * 
     * This class represents the set of all available cameras
     */
    class cameraSet {
    public:
      /**
       * Default constructor
       */
      cameraSet();

      /**
       * Default destructor
       */
      ~cameraSet();
      
      /**
       * Get the information block of the camera identified with the given
       * index.
       */
      const cameraInfo& operator[](const int idx) const;

      /**
       * Get the information block of the camera identified with the given
       * index.
       */
      cameraInfo& operator[](const int idx);

      /**
       * Return the total number of cameras found
       */
      unsigned int size() const;

      /**
       * Try to find a camera within the set with the given identification.
       * 
       * @param guid   is the identification number used by the libdc1394 
       * @param camIdx if the id is found, this will contain the index to
       *               access the camera with the given id, otherwise it
       *               will be set to -1.
       *
       * \returns \c true if the camera is found, or false if not.
       */
      bool findCamera(const uint64_t& guid,int& camIdx) const;

      /**
       * This method finds the first available camera.
       *
       * @param camIdx index of the first free camera found.  If no free camera
       *               is found, this value is set to -1 and the function
       *               return false.
       * @return true if a free camera was found, false otherwise
       */
      bool findFreeCamera(int& camIdx) const;

      /**
       * Mark the camera with the given index as being in use.
       *
       * Only one instance of this functor can use a particular camera at
       * a time.
       */
      bool reserveIndex(const int idx);

      /**
       * Mark the camera with the given index as free to be used by other
       * instances.
       *
       */
      bool releaseIndex(const int idx);
                             
      /**
       * Error string
       */
      std::string errorStr;

      /**
       * Get all available cameras
       */
      bool getCameras();

    protected:
      /**
       * Camera array 
       */
      std::vector<cameraInfo> cams_;

      /**
       * Protect the camera array
       */
      mutable mutex lock_;

      /**
       * Library context
       */
      dc1394_t *dc1394_;

      /**
       * Get camera nodes
       */
      bool deleteCameras();
    };

    //@}

    /**
     * Camera database
     *
     * Just one instance of this class is used to manage the names of
     * the available cameras.
     */
    class nameDB {
    public:
      /**
       * Default constructor 
       *
       * Does nothing
       */
      nameDB();

      /**
       * Default destructor
       *
       * Saves all new data acquired in the file indicated by the use() method.
       */
      ~nameDB();

      /**
       * Use the given file to serialize the data
       *
       * \returns \c true if the file can be successfully open and read, or
       * if it does not exist, if it can be successfully created.
       */
      bool use(const std::string& filename);

      /**
       * Query a camera id through a camera name.
       *
       * \returns \c true if the name could be found, in which case id will 
       *          contain the proper data.  \c false if the name could not
       *          be found.
       */
      bool query(const std::string& name,uint64_t& id) const;

      /**
       * Query a camera name through a camera id.
       *
       * \returns \c true if the id could be found, in which case name will 
       *          contain the proper data.  \c false if the id could not
       *          be found.
       */
      bool query(const uint64_t& id, std::string& name) const;

      /**
       * Add a name,id pair.
       *
       * \returns \c true if the pair could be successfully added or \c false
       * if there was a problem (usually, the name already existed!
       */
      bool add(const std::string& name,uint64_t& id);

      /**
       * Delete an entry with the given name as key
       *
       * \returns \c true if the entry was removed or \c false if some problem
       * occured, for instance, the entry didn't exist.
       */
      bool del(const std::string& name);
      
      /**
       * Dump the data_ into the file with the name filename_.
       *
       * This locks!  Do not call it from a locked method.
       *
       * \returns \c true if the file could be dumped or \c false if an error
       * occurred (like no use() called before)
       */
      bool dump();

    protected:
      /**
       * Map names to the identification numbers of cameras
       */
      std::map<std::string,uint64_t> data_;

      /**
       * Filename in use (given by use() method)
       */
      std::string filename_;

      /**
       * Protect data
       */
      mutable mutex lock_;

      /**
       * Read the data_ from the file with the name \c filename.
       *
       * This does not lock!  Do it outside the method.
       *
       * \returns \c true if the file could be read or \c false if an error
       * occurred (like no use() called before)
       */
      bool read(const std::string& filename);

      /**
       * Convert a 64 integer into a hex chain
       *
       * Since not all compilers behave the same with 64 bit chains, we
       * better take charge ourselves of converting them into a chain of 16
       * nibbles preceded by 0x.
       */
      void hexToString(const uint64_t& data,
                       std::string& str) const;

      /**
       * Convert a 16 nibble string into a uint64_t value
       *
       * The string has to begin with "0x" and continue with a number
       * of less or exactly 16 hex digits
       */
      bool stringToHex(const std::string& str,
                       uint64_t& data) const;

      /**
       * The file has to be read just once!
       *
       * This flag stores if it was read or not
       */
      bool init_;

      /**
       * Flag to indicate if the database has been modified since the last
       * read.
       */
      bool modified_;

    };

    /**
     * @name Hardware singletons
     *
     * The following attributes represent the IEEE 1394 bus and all
     * available cameras.
     */
    //@{
    /**
     * The camera set attached to the IEEE1394 bus found.
     */
    static cameraSet cams_;

    /**
     * The camera names database
     */
    static nameDB cameraNames_;

    /**
     * Initialize the current camera
     *
     * The initialization may change the camera being used, if the user
     * selected an upload or an autofix mode.
     */
    bool initCamera(parameters& par);
    //@}

    /**
     * Store the index for cams_ of the camera represented by this instance
     *
     * If activeCamera_ is less than zero then it indicates that this functor
     * has not initialized its camera yet.
     */
    int activeCamera_;

    /**
     * This is a shadow of cams_[activeCamera_].info for convinience, or 0
     * if not set
     */
    dc1394camera_t* camera_;

    /**
     * ISO speed of the current camera
     */
    dc1394speed_t isoSpeed_;

    /**
     * Active camera name
     *
     * The index in activeCamera_ corresponds to the a camera with 
     * the name activeCameraName_, which is used to decide if setParameters
     * is telling this functor to change the camera being used.
     */
    std::string activeCameraName_;

    /**
     * Release the active camera.
     *
     * This allow that another instance can take control of this camera.
     */
    bool releaseCamera();

    /**
     * @name Shadow and pointers for active camera attributes
     *
     * This shadows are set in updateParameters()
     */
    //@{

    /**
     * Shadow value of current in use camera to indicate that it is capable of
     * snapShotMode capture.
     *
     * This is a shadow of cams_[activeCamera_].one_shot_capable > 0;
     */
    bool snapShotCapable_;

    /**
     * Saves what features are available.
     *
     * This is a pointer to cams_[activeCamera_].featureSet
     */
    dc1394featureset_t* features_;
    //@}
    
    /**
     * @name Methods used to change on-the-go the resolution and
     * framerate
     */
    //@{
    /**
     * Stop iso flow iff activated.
     */
    bool isoFlowStop();

    /**
     * Like isoFlowStop, but additionaly sets the flag at miscInfo.is_iso_on to
     * false
     */
    bool isoFlowStopTotal();

    /**
     * Restarts stopped iso flow.
     *
     * This method is used in conjunction with isoFlowStop(), as the
     * latter does not change the flag usually employed to indicate if 
     * the iso flow has been started or not. 
     *
     * If the iso flow was running before calling isoFlowStop(), then
     * isoFlowResume() will resume it.  Otherwise it won't start anything.
     */
    bool isoFlowResume();
    //@}

    /**
     * Colormode used for conversion to rgb image 
     */
    eColorMode colorMode_;

    /**
     * Just a buffer channel8
     */
    channel8 ch8_;

    /**
     * Flag that indicates if a dma_setup_capture has been successfully being
     * called, in which case some memory has been reserved, etc. (in
     * updateParameters() usually)
     */
    bool captureSetUp_;

    /**
     * merger for YUV to RGB conversion
     */
    mergeYCbCrToImage merger_;

    /**
     * Pointer to demosaicing functor for Mono to RGB conversion
     */
    bayerDemosaicing* bayer_;

    /**
     * @name One-Push Wait Scheduler
     *
     * The following methods provide a very basic scheduler for
     * check of the successful automatic adjustment of features in the
     * one push mode
     */
    //@{

    /**
     * Feature list in waiting process
     */
    std::set<unsigned int> waitingFeatures_;

    /**
     * Protect the queue from multiple access.
     */
    mutex onePushLock_;

    /**
     * Insert the given feature into the waiting queue.
     * @param featureId identification of the feature that want to wait for
     *                  leaving the one-push state.
     * @return \c true if feature could be inserted or \c false if it was
     *         something wrong (not supported, or a real error).
     */
    bool onePushQueueInsert(const unsigned int featureId);

  public:
    /**
     * Wait until all features in the waiting queue have left the queue.
     *
     * @return \c true if all features left the one-push state on a time
     *         less than the time-out, or false otherwise.
     */
    bool onePushWait();
    
    //@}


    /**
     * @name Inherited from framegrabber
     */
    //@{

    /**
     * Initialize frame grabber.
     *
     * @return true if successful, false otherwise
     */
    bool init();

    /**
     * Returns true if the frame-grabber hardware has been initialized.
     */
    virtual bool isInitialized() const;
    
    //@}

    /**
     * Return a string of the dc1394 error
     */
    static const char* errorString(dc1394error_t err);

  };

}

#endif

#endif
