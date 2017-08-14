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
 * \file   ltiCamera.h
 *         Contains an abstract parent class for all camera classes.
 * \author Pablo Alvarado
 * \date   16.04.2007
 * $Id: ltiCamera.h,v 1.8 2007-05-10 22:38:51 alvarado Exp $
 */

#ifndef _LTI_CAMERA_H
#define _LTI_CAMERA_H

#include <set>
#include "ltiCamFeatureMode.h"
#include "ltiPoint.h"
#include "ltiInterval.h"
#include "ltiIoHandler.h"
#include "ltiMatrix.h"

namespace lti {
  /**
   * Abstract class to unify a minimal set of features the cameras
   * have to provide.
   *
   * Usually, this class will be used in conjuntion with the
   * lti::frameGrabber class, and if some advanced hardware feature is
   * available, maybe also from lti::lensUnit and lti::panTiltUnit.
   *
   * It makes use of the type lti::eCamFeatureMode to provide several
   * state for the features.  The concept is originary from the
   * firewire cameras, but it can be extended to other devices as well.
   * This allows a more polymorphic use of the camera derived clases.
   *
   * Since the derived classes may be strongly dependent on the
   * hardware used, the methods are mostly pure virtual, as the
   * efficient implementations in most cases depend on each particular
   * hardware interface.
   *
   * @ingroup gAcquisition
   */
  class camera {
  public:
    /**
     * Feature IDs.
     *
     * These IDs have been selected based on the IIDC standard
     */
    enum {
      FeatureBrightness= 416,    /**< Brightness    */
      FeatureContrast=417,       /**< Contrast which is identical to Exposure*/
      FeatureExposure=417,       /**< Exposure      */
      FeatureSharpness,          /**< Sharpness     */
      FeatureWhiteBalance,       /**< White balance */
      FeatureHue,                /**< Hue           */
      FeatureSaturation,         /**< Saturation    */
      FeatureGamma,              /**< Gamma         */
      FeatureShutter,            /**< Shutter       */
      FeatureGain                /**< Gain          */
      //FeatureTemperature=427,    /**< Temperature   */
      //FeatureWhiteShading=430,   /**< White shading */

      //FeatureFrameRate           /**< Frame rate: how to solve conflicts
      //                            *   between frame rate and shutter */

      //FeatureCaptureSize=436,    /**< Quality aspects for format 6(still img)
      //FeatureCaptureQuality=437
    };

    /**
     * The parameters for the class lti::camera
     */
    class parameters {
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
       * Write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      bool write(ioHandler& handler,const bool complete=true) const;

      /**
       * Read the parameters from the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      bool read(ioHandler& handler,const bool complete=true);

      // ------------------------------------------------
      // the parameters
      // ------------------------------------------------

      /**
       * Size of the image to be acquired.
       *
       * Default: 640x480
       */
      ipoint resolution;

      /**
       * Desired framerate in frames per second.
       *
       * Default: 15
       */
      float framerate;
       
      /**
       * @name Camera features
       *
       * The following attributes give access to the features supported
       * by the different cameras.
       *
       * Each particular camera usually support just a subset of these
       * features.  You can use the method
       * lti::camera::isFeatureAvailable() to check if a particular
       * feature is supported by your camera or not.
       *
       * The attributes with the word "mode" control the "mode" or
       * "state" in which the feature is to be used.
       *
       * Not all cameras support all possible states of a particular feature.
       * You can use the method lti::camera::checkFeatureModes() to test
       * the camera support for the on-off, auto, manual, one-push and absolute
       * modes.
       *
       * If a camera supports the one-push state, then the corresponding values
       * for the feature will be automatically adjusted exactly once, just when
       * you set the parameters or update them.  Then the camera automatically
       * returns into the manual state.  You can use the dumpParameters()
       * method of the inherited classes to obtain the adjusted values.
       *
       * Please note that the LTI-Lib uses float values for the features to
       * cover directly the values that a "absolute-setting capable" camera
       * would admit.
       */
      //@{

      /**
       * Brightness feature state.
       *
       * The brightness usually refers to an offset value that is added to the
       * CCD signal before the A/D conversion, and is used to remove residual
       * offsets in the signal chain.  Therefore, is is also known as "black
       * level".  You may check, however, which use is being assigned to this
       * feature in your camera.
       *
       * - FeatureOff: Brightness level will have a fixed value.
       * - FeatureAuto: Camera will control the brightness level automatically.
       * - FeatureManual: User sets the brightness level manually with the
       *                  attribute \c parameters::brightness
       * - FeatureOnePush: Brightness value is automatically set and then
       *                   returns to manual state.
       *
       * Default value: FeatureAuto
       */
      eCamFeatureMode brightnessMode;

      /**
       * Overall brightness.
       *
       * Default value: 0
       */
      float brightness;

      /**
       * Auto exposure mode (alias of contrast mode)
       *
       * - FeatureOff: Exposure will be manually controlled by gain, iris and 
       *               shutter features.
       * - FeatureAuto: reference level is automatically adjusted continuously.
       * - FeatureManual: exposure level automatically set, but the user 
       *                  selects the reference level in parameters::exposure
       * - FeatureOnePush: reference level is set once automatically and then
       *                   the mode returns to manual.
       *
       * Default value: FeatureAuto
       */
      union {
        /**
         * ExposureMode and ContrastMode are synonyms
         */
        eCamFeatureMode exposureMode;

        /**
         * Auto contrast mode (alias of exposureMode)
         *
         * - FeatureOff: Exposure will be manually controlled by gain, iris
         *               and shutter features.
         * - FeatureAuto: reference level is automatically adjusted 
         *                continuously.
         * - FeatureManual: exposure level automatically set, but the user 
         *                  selects the reference level in parameters::exposure
         * - FeatureOnePush: reference level is set once automatically and then
         *                   the mode returns to manual.
         *
         * Default value: FeatureAuto
         */
        eCamFeatureMode contrastMode;
      };
      
      /**
       * Auto exposure control (alias of contrast).
       *
       * Reference level used for the auto exposure control.  This is
       * similar to contrast control.  Usually, if the feature is
       * deactivated, then the exposure is controled by gain and
       * shutter values, or, if you have lenses-controlable hardware,
       * by iris settings too.
       *
       * Other systems rely on a contrast definition, hence, in this functor
       * exposure and contrast are used as synonyms.
       *
       * Default value: 0
       */
      union {
        /**
         * Auto exposure control (alias of contrast).
         *
         * Reference level used for the auto exposure control.  This is
         * similar to contrast control.  Usually, if the feature is
         * deactivated, then the exposure is controled by gain and
         * shutter values, or, if you have lenses-controlable hardware,
         * by iris settings too.
         *
         * Other systems rely on a contrast definition, hence, in this functor
         * exposure and contrast are used as synonyms.
         *
         * Default value: 0
         */
        float exposure;

        /**
         * Auto contrast control (alias of exposure).
         *
         * Reference level used for the auto exposure control.  This is
         * similar to contrast control.  Usually, if the feature is
         * deactivated, then the exposure is controled by gain and
         * shutter values, or, if you have lenses-controlable hardware,
         * by iris settings too.
         *
         * Other systems rely on a contrast definition, hence, in this functor
         * exposure and contrast are used as synonyms.
         *
         * Default value: 0
         */
        float contrast;
      };

      /**
       * Sharpness control mode.
       *
       * - FeatureOff: sharpness level will have a fixed value
       * - FeatureAuto: camera controls sharpness level automatically.
       * - FeatureManual: Sharpness level is set to the value in 
       *                  parameters::sharpness
       * - FeatureOnePush: sharpness level is set once automatically and then
       *                   the mode returns to manual.
       *
       * Default value: FeatureAuto
       */
      eCamFeatureMode sharpnessMode;

      /**
       * Sharpness control value
       *
       * Default value: 0
       */
      float sharpness;

      /**
       * Automatic white balance.
       *
       * If the white balance mode is set to manual, then the values of
       * redGain and blueGain are taken into consideration.
       *
       * - FeatureOff: white balance will have fixed values.
       * - FeatureAuto: camera controls white balance automatically.
       * - FeatureManual: white balance will be manually controlled through
       *                  the values parameters::redGain and 
       *                  parameters::blueGain.
       * - FeatureOnePush: white balance is set once automatically and then
       *                   the mode returns to manual.
       *
       * Default value: FeatureAuto
       */
      eCamFeatureMode whiteBalanceMode;

      /**
       * Gain applied to red (or V) components if the
       * parameters::whiteBalanceMode requires it.
       *
       * Default value: 0
       */
      float redGain;

      /**
       * Gain applied to blue (or U) components if the
       * parameters::whiteBalanceMode requires it.
       *
       * Default value: 0
       */
      float blueGain;

      /**
       * Hue control mode
       *
       * - FeatureOff: hue control will have fixed values.
       * - FeatureAuto: camera controls hue automatically.
       * - FeatureManual: hue will be manually controlled through
       *                  the value parameters::hue.
       * - FeatureOnePush: hue is set once automatically and then
       *                   the mode returns to manual.
       *
       * Default value: FeatureAuto
       */
      eCamFeatureMode hueMode;

      /**
       * Hue control value
       *
       * Controls color phase of the picture
       *
       * Default value: 0
       */
      float hue;

      /**
       * Saturation control mode
       *
       * - FeatureOff: saturation control will have fixed values.
       * - FeatureAuto: camera controls saturation automatically.
       * - FeatureManual: saturation will be manually controlled through
       *                  the value parameters::saturation.
       * - FeatureOnePush: saturation is set once automatically and then
       *                   the mode returns to manual.
       *
       * Default value: FeatureAuto
       */
      eCamFeatureMode saturationMode;

      /**
       * Saturation control value
       *
       * Controls color saturation of the picture
       *
       * Default value: 0
       */
      float saturation;

      /**
       * Gamma control mode
       *
       * - FeatureOff: gamma control will have fixed values.
       * - FeatureAuto: camera controls gamma automatically.
       * - FeatureManual: gamma will be manually controlled through
       *                  the value parameters::gamma.
       * - FeatureOnePush: gamma is set once automatically and then
       *                   the mode returns to manual.
       *
       * Default value: FeatureAuto
       */
      eCamFeatureMode gammaMode;

      /**
       * Gamma control value
       *
       * Controls gamma value used in the adjustment between incomming light
       * level and output picture level.
       *
       * Default value: 0
       */
      float gamma;

      /**
       * Mode for the shutter control.
       *
       * "Shutter" means the integration time of the incomming light.
       * 
       * - FeatureOff: integration time will have fixed values.
       * - FeatureAuto: camera controls shutter automatically.
       * - FeatureManual: shutter will be manually controlled through
       *                  the value parameters::shutter.
       * - FeatureOnePush: shutter is set once automatically and then
       *                   the mode returns to manual.
       *
       * Default value: FeatureAuto
       */
      eCamFeatureMode shutterMode;

      /**
       * Integration time of the incomming light.
       *
       * Influences shutter speed of the camera, higher values
       * usually mean lower shutter speed.
       *
       * Default value: 0
       */
      float shutter;

      /**
       * Mode used for gain control.
       *
       * Gain is usually a factor used to multiply the CCD signal, modifying
       * the entire dynamic range.
       *
       * - FeatureOff: gain control will have fixed values.
       * - FeatureAuto: camera controls gain automatically.
       * - FeatureManual: gain will be manually controlled through
       *                  the value parameters::gain.
       * - FeatureOnePush: gain is set once automatically and then
       *                   the mode returns to manual.
       *
       * Default value: FeatureAuto
       */
       eCamFeatureMode gainMode;

      /**
       * Gain of the camera, usually applied on all colors.
       *
       * Default: 0
       */
      float gain;
      //@}

      /**
       * @name One-Push related parameters.
       *
       * When you set a feature mode in "FeatureOnePush", then the camera is
       * told to switch into a "one-push" mode.  You can indicate with the
       * following attributes, first, if you want to wait for the camera to
       * automatically return to the manual control mode; second, if you do
       * want to wait, then how often you want to check if the camera returned
       * from the "one-push" mode, and for how long.
       */
      //@{
      /**
       * Wait for one-push mode ready
       *
       * This indicates if you want to way for a one-push capable feature
       * to finish the automatic configuration.
       *
       * Default value: false;
       */
      bool waitOnePush;

      /**
       * Frequency (in Herz) of tests if the one-push features are ready.
       * 
       * Default value: 2 (twice a second)
       */
      float onePushUpdateRate;

      /**
       * Time-out for the one-push checks (in seconds)
       *
       * Default value: 10 (seconds)
       */
      float onePushTimeout;
      //@}
    };

    /**
     * Default constructor
     *
     * You usually avoid using this constructor.  Using camera(const
     * parameters&) you can provide directly for which camera the constructed
     * instance should work.
     */
    camera();

    /**
     * Default destructor
     *
     */
    virtual ~camera();

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
                               const float brightness=0)=0;

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
                               float& brightness) const=0;

    /**
     * Set exposure control mode and register in camera directly.
     *
     * @param mode Camera desired mode (@see eCamFeatureMode)
     * @param exposure If mode is manual, the value to be used.
     *
     * @return true if operation was successfull, false otherwise
     */
    virtual bool setExposure(const eCamFeatureMode mode,
                             const float exposure=0)=0;

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
                             float& exposure) const=0;

    /**
     * Set contrast control mode and register in camera directly.
     *
     * This is an alias of setExposure(), so please reimplement
     * that other pure virtual method.
     *
     * @param mode Camera desired mode (@see eCamFeatureMode)
     * @param contrast If mode is manual, the value to be used.
     *
     * @return true if operation was successfull, false otherwise
     */
    virtual bool setContrast(const eCamFeatureMode mode,
                             const float contrast=0);

    /**
     * Get contrast mode and value directly out of camera.
     *
     * This is an alias of getExposure(), so please reimplement
     * that other pure virtual method.
     *
     * @param mode Variable where the current contrast mode is to be 
     *              written.
     * @param contrast Variable where the contrast value is to be written.
     *
     * @return true if operation was successfull, false otherwise
     */ 
    virtual bool getContrast(eCamFeatureMode& mode,
                             float& contrast) const;


    /**
     * Set sharpness control mode and register in camera directly.
     *
     * @param mode Camera desired mode (@see eCamFeatureMode)
     * @param sharpness If mode is manual, the value to be used.
     *
     * @return true if operation was successfull, false otherwise
     */
    virtual bool setSharpness(const eCamFeatureMode mode,
                              const float sharpness=0)=0;

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
                              float& sharpness) const=0;

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
                                 const float vrGain=0)=0;
    
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
                                 float& vrGain) const=0;

    /**
     * Set hue control mode and register in camera directly.
     *
     * @param mode Camera desired mode (@see eCamFeatureMode)
     * @param hue If mode is manual, the value to be used.
     *
     * @return true if operation was successfull, false otherwise
     */
    virtual bool setHue(const eCamFeatureMode mode,
                        const float hue=0)=0;

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
                        float& hue) const=0;

    /**
     * Set saturation control mode and register in camera directly.
     *
     * @param mode Camera desired mode (@see eCamFeatureMode)
     * @param saturation If mode is manual, the value to be used.
     *
     * @return true if operation was successfull, false otherwise
     */
    virtual bool setSaturation(const eCamFeatureMode mode,
                               const float saturation=0)=0;

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
                               float& saturation) const=0;
    
    /**
     * Set gamma control mode and register in camera directly.
     *
     * @param mode Camera desired mode (@see eCamFeatureMode)
     * @param gamma If mode is manual, the value to be used.
     *
     * @return true if operation was successfull, false otherwise
     */
    virtual bool setGamma(const eCamFeatureMode mode,
                          const float gamma=0)=0;

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
                          float& gamma) const=0;

    /**
     * Set shutter control mode and register in camera directly.
     *
     * @param mode Camera desired mode (@see eCamFeatureMode)
     * @param shutter If mode is manual, the value to be used.
     *
     * @return true if operation was successfull, false otherwise
     */
    virtual bool setShutter(const eCamFeatureMode mode,
                            const float shutter=0)=0;

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
                            float& shutter) const=0;

    /**
     * Set gain control mode and register in camera directly.
     *
     * @param mode Camera desired mode (@see eCamFeatureMode)
     * @param gain If mode is manual, the value to be used.
     *
     * @return true if operation was successfull, false otherwise
     */
    virtual bool setGain(const eCamFeatureMode mode,
                         const float gain=0)=0;

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
                         float& gain) const=0;

    //@}
    
    /**
     * @name Methods to change or read the settings on framerate and
     *       resolution.
     */
    //@{
    /**
     * Get current frame rate directly out of camera.
     * @return true if operation was successfull, false otherwise
     */
    virtual bool getFramerate(float& framerate) const=0;


    /**
     * Get current resolution, posibly directly out of the camera
     */
    virtual bool getResolution(ipoint& resolution) const=0;

    /**
     * Set framerate and resolution directly to the camera.
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
                                           const ipoint& resolution)=0;

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
    virtual bool supportedFrameratesAndResolutions(fvector& framerates,
                                                   vector<ipoint>& resols,
                                                   matrix<ubyte>& val) const=0;
    //@}

    /**
     * Check if the frame grabber / camera system is active
     */
    virtual bool isActive() const=0;

    /**
     * Check if a particular feature is available in the current active
     * camera.
     *
     * You have to ensure that a camera is already active (see
     * isActive()).
     *
     * \param featureId The identification of the feature expects one of the
     *                  - FeatureBrightness
     *                  - FeatureExposure
     *                  - FeatureSharpness
     *                  - FeatureWhiteBalance
     *                  - FeatureHue
     *                  - FeatureSaturation
     *                  - FeatureGamma
     *                  - FeatureShutter
     *                  - FeatureGain
     *                  - FeatureTemperature
     *                  - FeatureWhiteShading
     *                  - FeatureFrameRate
     *
     * This pure virtual method may have other possible values as
     */
    virtual bool isFeatureAvailable(const unsigned int featureId) const=0;

    /**
     * Check if a feature is available and is read-out capable.
     *
     * Some cameras have a feature, but you cannot read the state in
     * which that feature is set.  This method returns true if the
     * camera can provide information on the feature state/mode.
     *
     * \param featureId identification for the feature to be queried.
     *                  (see camera::isFeatureAvailable() for a list of
     *                   valid values)
     * \return \c true if feature is read-out capable or \c false if it is not
     *         available at all, or if it is not readable.
     */
    virtual bool isFeatureReadable(const unsigned int featureId) const=0;

    /**
     * Check for availability of feature modes
     *
     * A given feature (see the list of valid identifiers in
     * lti::camera::isFeatureAvailable() ) can have one of five modes:
     * - Not available: the feature does not exist in the current camera
     * - Off mode: the feature is deactivated (some features cannot be
     *             deactivated).
     * - Auto mode: the feature is automatically adjusted 
     * - Manual mode: the user controls the values a feature can get 
     * - One-Push mode: the feature is automatically adjusted but only once.
     *
     * This method asks the active camera which modes are supported and
     * returns a bitwise combination of lti::eCamFeatureMode constants.
     *
     * You have to ensure that a camera is active (see camera::isActive()).
     *
     * \param featId identification for the feature to be modified
     *                  (see camera::isFeatureAvailable() for a list of
     *                   valid values)
     *
     */
    virtual unsigned int checkFeatureModes(const unsigned int featId) const=0;

    /**
     * Get feature mode of the camera for one specific feature.
     *
     * @return \c true if feature mode could be successfully read, or \c false
     *         if this was not possible (maybe the camera does not support
     *         the given feature at all).
     */
    virtual bool getFeatureMode(const unsigned int featureId,
                                eCamFeatureMode& mode) const=0;

    /**
     * Set operation mode of a particular feature of the camera.
     *
     * @return \c true if feature mode could be successfully set, or \c false
     *         if this was not possible (maybe the camera does not support
     *         the given mode for the feature.
     */
    virtual bool setFeatureMode(const unsigned int featureId,
                                const eCamFeatureMode mode)=0;

    /**
     * Set control mode and value of a particular feature.
     *
     * This method does not support those features that require two values,
     * like white balance.
     *
     * @param featureId identification for the feature to be modified
     *                  (see camera::isFeatureAvailable() for a list of
     *                   valid values)
     * @param mode Camera desired mode (@see eCamFeatureMode)
     * @param value If mode is manual, the value to be used.
     *
     * @return true if operation was successfull, false otherwise
     */
    virtual bool setFeatureValue(const unsigned int featureId,
                                 const eCamFeatureMode mode,
                                 const float value=0)=0;
    
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
     *         example, if the feature is not supported by the camera).
     */
    virtual bool getFeatureValue(const unsigned int featureId,
                                 eCamFeatureMode& mode,
                                 float& value) const=0;

    /**
     * Check the valid value range for a specified feature.
     *
     * You have to ensure that there is an active camera before calling this
     * method. (see camera::isActive() )
     *
     * @param featureId identification for the feature to be modified
     *                  (see camera::isFeatureAvailable() for a list of
     *                   valid values)
     * @param minValue minimum value accepted for the specified feature.
     * @param maxValue maximum value accepted for the specified feature.
     *
     * @return \c true if successful or \c false otherwise.
     */
    virtual bool checkFeatureValueRange(const unsigned int featureId,
                                        float& minValue,
                                        float& maxValue) const=0;

    /**
     * Check the valid value range for a specified feature.
     *
     * You have to ensure that there is an active camera before calling this
     * method. (see camera::isActive() )
     *
     * @param featureId identification for the feature to be modified
     *                  (see camera::isFeatureAvailable() for a list of
     *                   valid values)
     * @param range closed interval for permitted values.
     *
     * @return \c true if successful or \c false otherwise.
     */
    virtual bool checkFeatureValueRange(const unsigned int featureId,
                                              finterval& range) const;
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
    virtual const std::string cameraName() const = 0;
    
  }; // class camera
} // namespace lti

#endif
