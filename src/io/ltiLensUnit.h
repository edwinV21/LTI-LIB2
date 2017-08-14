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
 * \file   ltiLensUnit.h
 *         Contains an abstract parent class for all hardware that
 *         can control mechanical lens options as zoom, focus and iris
 * \author Pablo Alvarado
 * \date   16.04.2007
 * $Id: ltiLensUnit.h,v 1.1 2007-04-18 21:18:18 alvarado Exp $
 */

#ifndef _LTI_LENS_UNIT_H
#define _LTI_LENS_UNIT_H

#include "ltiCamFeatureMode.h"
#include "ltiIoHandler.h"

namespace lti {
  /**
   * Abstract class to unify a minimal set of features for optical
   * devices that allow software control of zoom, focus and iris.
   *
   * Usually, this class will be used in conjuntion with the
   * lti::frameGrabber class and the lti::camera classes.
   *
   * It makes use of the type lti::eCamFeatureMode to provide several
   * state for the features.  The concept is originary from the
   * firewire cameras, but it can be extended to other devices as well.
   *
   * Since the derived classes may be strongly dependent on the
   * hardware used, the methods are mostly pure virtual, as the
   * efficient implementations in most cases depend on each particular
   * hardware interface.
   *
   * @ingroup gAcquisition
   */
  class lensUnit {
  public:
    /**
     * Feature IDs.
     *
     * These IDs have been selected based on the IIDC standard
     */
    enum {
      FeatureIris=425,           /**< Iris           */
      FeatureFocus,              /**< Focus          */
      FeatureZoom=432,           /**< Zoom           */
      FeatureOpticalFilter=435   /**< Optical Filter */    
    };

    /**
     * The parameters for the class lti::lensUnit
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
       * Mode used for mechanical iris control.
       *
       * - FeatureOff: iris control will have fixed values.
       * - FeatureAuto: device controls iris automatically.
       * - FeatureManual: iris will be manually controlled through
       *                  the value parameters::iris.
       * - FeatureOnePush: iris is set once automatically and then
       *                   the state returns to manual.
       *
       * Default value: FeatureAuto
       */
      eCamFeatureMode irisMode;

      /**
       * Manually set value for the mechanical iris control.
       *
       * Default: 0
       */
      float iris;

      /**
       * Mode used for lens focus control.
       *
       * - FeatureOff: focus control will have fixed values.
       * - FeatureAuto: device controls focus automatically.
       * - FeatureManual: focus will be manually controlled through
       *                  the value parameters::focus.
       * - FeatureOnePush: focus is set once automatically and then
       *                   the state returns to manual.
       *
       * Default value: FeatureAuto
       */
      eCamFeatureMode focusMode;

      /**
       * Value used for manual lens focus control.
       *
       * Default: 0
       */
      float focus;

      /**
       * Mode used for lens zoom control.
       *
       * - FeatureOff: zoom control will have fixed values.
       * - FeatureAuto: device controls zoom automatically.
       * - FeatureManual: zoom will be manually controlled through
       *                  the value parameters::zoom.
       * - FeatureOnePush: zoom is set once automatically and then
       *                   the state returns to manual.
       *
       * Default value: FeatureAuto
       */
      eCamFeatureMode zoomMode;

      /**
       * Value used for manual lens zoom control.
       *
       * Default: 0
       */
      float zoom;

      /**
       * Optical filter control mode
       *
       * - FeatureOff: optical filter control will have fixed values.
       * - FeatureAuto: device controls optical filter automatically.
       * - FeatureManual: optical filter will be manually controlled through
       *                  the value parameters::opticalFilter.
       * - FeatureOnePush: optical filter is set once automatically and then
       *                   the state returns to manual.
       *
       * Default value: FeatureAuto
       *
       */
      eCamFeatureMode opticalFilterMode;

      /**
       * Value used for the optical filter.
       *
       * Default value: 0
       */
      float opticalFilter;
    };

    /**
     * Default constructor
     */
    lensUnit();

    /**
     * Default destructor
     *
     */
    virtual ~lensUnit();

    /**
     * @name Direct access to lens features.
     *
     * These methods access the device registers directly, and
     * they try to update the internal parameters.
     *
     * If a feature supports the "one-push mode", then the automatic
     * configuration will begin as soon as you call the corresponding method
     * with the value \c FeatureOnePush as mode (for example
     * setIris(FeatureOnePush) ).
     *
     * If you set the mode of one or more features to one-push, then you may
     * want to wait for them to be ready with the method onePushWait().  Of
     * course, this will work if and only if the device supports this feature
     * mode.
     *
     * The methods to query information from the device do indeed ask the
     * device for the corresponding value (if and only if this is possible).
     * There are devices which do not support the readout capability for the
     * registers holding the feature values.  In that case, the get* methods
     * will return \c false.
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
                               const float iris=0)=0;

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
                               float& iris) const=0;

    /**
     * Set focus control mode and register in device directly.
     *
     * @param mode Device desired mode (@see eCamFeatureMode)
     * @param focus If mode is manual, the value to be used.
     *
     * @return true if operation was successfull, false otherwise
     */
    virtual bool setFocus(const eCamFeatureMode mode,
                             const float focus=0)=0;

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
                             float& focus) const=0;

    /**
     * Set zoom control mode and register in device directly.
     *
     * @param mode Device desired mode (@see eCamFeatureMode)
     * @param zoom If mode is manual, the value to be used.
     *
     * @return true if operation was successfull, false otherwise
     */
    virtual bool setZoom(const eCamFeatureMode mode,
                              const float zoom=0)=0;

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
                         float& zoom) const=0;

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
                                  const float opticalFilter=0)=0;
    
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
                                  float& opticalFilter) const=0;

    /**
     * Check if the device system is active
     */
    virtual bool isActive() const=0;

    /**
     * Check if a particular feature is available in the current active
     * device.
     *
     * You have to ensure that a device is already active (see
     * isActive()).
     *
     * \param featureId The identification of the feature expects one of the
     *                  - FeatureIris
     *                  - FeatureFocus
     *                  - FeatureZoom
     *                  - FeatureOpticalFilter
     *
     * This pure virtual method may have other possible values as
     */
    virtual bool isFeatureAvailable(const unsigned int featureId) const=0;

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
    virtual bool isFeatureReadable(const unsigned int featureId) const=0;

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
    virtual unsigned int checkFeatureModes(const unsigned int featId) const=0;

    /**
     * Get feature mode of the device for one specific feature.
     *
     * @return \c true if feature mode could be successfully read, or \c false
     *         if this was not possible (maybe the device does not support
     *         the given feature at all).
     */
    virtual bool getFeatureMode(const unsigned int featureId,
                                eCamFeatureMode& mode) const=0;

    /**
     * Set operation mode of a particular feature of the device.
     *
     * @return \c true if feature mode could be successfully set, or \c false
     *         if this was not possible (maybe the device does not support
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
     *                  (see device::isFeatureAvailable() for a list of
     *                   valid values)
     * @param mode Device desired mode (@see eCamFeatureMode)
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
     *         example, if the feature is not supported by the device).
     */
    virtual bool getFeatureValue(const unsigned int featureId,
                                 eCamFeatureMode& mode,
                                 float& value) const=0;

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
     * @return \c true if successful or \c false otherwise.
     */
    virtual bool checkFeatureValueRange(const unsigned int featureId,
                                        float& minValue,
                                        float& maxValue) const=0;
    //@}
  }; // class lensUnit
} // namespace lti

#endif
