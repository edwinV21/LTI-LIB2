/*
 * Copyright (C) 2007
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
 * \file   ltiFastHessianDetection.h
 *         Contains the class lti::fastHessianDetection, 
 *         which is the location detector used by the SURF concept.
 * 
 * \author Pablo Alvarado
 * \date   23.10.2007
 *
 * revisions ..: $Id: ltiFastHessianDetection.h,v 1.9 2009-04-08 03:36:45 alvarado Exp $
 */

#ifndef _LTI_FAST_HESSIAN_DETECTION_H_
#define _LTI_FAST_HESSIAN_DETECTION_H_

#include "ltiChannel.h"
#include "ltiLocation.h"
#include "ltiChannel8.h"
#include "ltiLocationDetection.h"
#include "ltiList.h"
#include "ltiBoundaryType.h"
#include "ltiArray2D.h"

namespace lti {
  
  class integralImage;


  /**
   * Class fastHessianDetection
   *
   * The class lti::fastHessianDetection is one of the available location
   * detectors in the LTI-Lib, i.e. is a class used to detect interesing points
   * in an image, considering also the "scale" at which the interesting
   * information resides.
   *
   * This particular method is part of the SURF approach for robust feature
   * detection, as explained in detail in the paper:
   *
   * Herbert Bay, Tinne Tuytelaars, and Luc Van Gool.  SURF: Speeded Up Robust
   * Features.  In Proceedings of the 9th European Conference on Computer
   * Vision, May 2006.  Available at
   * http://www.vision.ee.ethz.ch/~surf/papers.html
   *
   * The implementation here provides some configuration possibilities, but is
   * limited to provide the authors concept as reliable as the paper allowed.
   *
   * @see fastHessianDetection::parameters.
   * @see surfLocalDescriptor
   *
   * @ingroup gFeatureExtr
   */
  class fastHessianDetection : public locationDetection {
  public:
    /**
     * Types for locations to be found
     */
    enum eExtremaType {
      Minima, /**< Only detect minima of the determinant image */
      Maxima, /**< Only detect maxima of the determinant image */
      Both    /**< Detect both maxima and minima */
    };
    
    /**
     * Types of level selection in the multiresolutional representation
     */
    enum eLevelSelectionMethod {
      Blocks, /**< Original method suggested in the SURF paper, which 
               *   uses blocks of levels separated by the same scale step,
               *   After a block has been computed, the next one duplicates
               *   the step.
               */
      Exponential /**< The exponential level selection method uses for the
                   *   scales the equation \f$\alpha^n\sigma_B\f$
                   */
    };

    /**
     * Modes available for the selection of conspicuous locations
     *
     * These modes affect the way in which the threshold value is interpreted.
     */
    enum eLocationSelectionMode {
      All,      /**< Ignore the threshold, and compute all locations
                 */
      Absolute, /**< The determinant of the hessian matrix has to be greater
                 *   than the threshold value (or less than -threshold) to be
                 *   considered.
                 */
      Relative, /**< The determinant of the hessian matrix has to be greater
                 *   than a threshold value computed as a percentage of the
                 *   greatest value found at each level.
                 */
      Conspicuous, /**< The determinant of the hessian matrix has to be greater
                    *   than a threshold value computed as the mean of all
                    *   values plus a number of times the standard deviation
                    */
      Number    /**< At most the given number of (strongest) locations will be
                 *   returned.
                 */
                   
    };
    
    /**
     * The parameters for the class fastHessianDetection
     */
    class parameters : public locationDetection::parameters {
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
       * Returns the complete name of the parameters class.
       */
      virtual const std::string& name() const;

      /**
       * Returns a pointer to a clone of the parameters.
       */
      virtual parameters* clone() const;

      /**
       * Returns a pointer to a new instance of the parameters.
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
       * @name Parameters for the "scale-space" levels
       */
      //@{
      /**
       * Number of levels of the multi-scale representation used to detect
       * interest points.
       *
       * You need at least 3 levels.
       *
       * See parameters::initialKernelStep for an example.
       *
       * Default value: 12
       */
      int numberOfLevels;

      /**
       * Kernel size for the highest spatian resolution
       *
       * This is the kernel side size, which means that, if you provide 9, then
       * a kernel 9x9 will be built.
       *
       * The kernel size is intrinsically tied to the sigma (standard
       * deviation) of the kernel.  A 9x9 kernel will always use a sigma=1.2 in
       * accordance to the SURF paper.  Any other kernel size will ajust the
       * sigma linearly.  A kernel of 27x27 will use a sigma of 3x1.2=3.6,
       * since 27=3x9.
       *
       * The size value has to be divisible by 3 and odd.
       *
       * See parameters::initialKernelStep for an example.
       *
       *
       * Default value: 9
       */
      int initialKernelSize;

      /**
       * Level selection method.
       *
       * You can choose between two methods for the level selection:
       * - The \c Blocks method resembles the one proposed in the SURF paper
       *   selecting blocks of levels separated by a fixed amount of "scale".
       * - The \c Exponential method increases the scale exponentially, as
       *   is usual in scale-space pyramids.  However, since the kernel sizes
       *   must be odd and divisible by three, the lower resolution will
       *   not exactly grow at exponential rates.
       *
       * Default value: Blocks
       */
      eLevelSelectionMethod levelSelectionMethod;

      /**
       * Kernel step size change between levels.
       *
       * This parameters is used only if you select the levelSelectionMethod as
       * \c Blocks.
       * 
       * The multiresolutional representation of a channel contains several
       * levels, each one computed with a kernel size to which this step value
       * is iteratively added.  Once the \c levelGroupSize has been reached,
       * the kernel step is duplicated.
       *
       * For example, with 
       * - \c initialKernelSize=9,
       * - \c initialKernelStep=6,
       * - \c levelGroupSize=4, and
       * - \c numberOfLevels=12,
       *
       * then the used kernel sizes will be 
       * -# 9x9,    15x15,  21x21,  27x27,
       * -# 39x39,  51x51,  63x63,  75x75,
       * -# 99x99, 123x123,147x147,171x171 
       *
       * The step value has to be even and divisible by three (and hence,
       * divisible by 6).  If not, then the closest number divisible by 6 will
       * be used.
       *
       * Default value: 6
       */
      int initialKernelStep;

      /**
       * Number of levels in a group or octave
       *
       * If levelSelectionMethod is set to \c Blocks, a group of levels is the
       * number of levels generated using the same kernel step.  The "next"
       * group will double the kernel size step.
       *
       * See parameters::initialKernelStep for an example.
       *
       * If levelSelectionMethod is set to \c Exponential, then this is the
       * number of levels tried to be present in an octave.  However, since
       * the kernel sizes have to be divisible by three and have to be odd,
       * then this might become true only for the higher levels.
       *
       * Default value: 4
       */
      int levelGroupSize;

      /**
       * Norm power.
       *
       * Theoretically this value has to be 4.0f in order to keep the same
       * information weight of all levels.  You can modify this value to
       * slightly smaller values (e.g. 3.9f) if you want to give more weight to
       * larger locations, or larger values (4.1f) if you need to obtain
       * locations of small details.
       *
       * The outputs of the filters approximating the second order derivatives
       * have to be normalized by the filter mask sizes (and hence, the norm
       * will be proportional to the square of the scale).  Additionally, the
       * determinant of the hessian matrix is computed by products of two
       * filter outputs, and hence, the total norm is proportional to the scale
       * to the power of four.
       *
       * Default value: 4
       */
      float normPower;
      //@}

      /**
       * @name Parameters for the location selection
       */
      //@{
      /**
       * Types of extrema to search for.
       *
       * The locations are placed on local extrema of the scale-space
       * representation of the image.  Here you can indicate if they should be
       * placed on \c Maxima, on \c Minima or on \c Both.
       *
       * Default value: Both
       */
      eExtremaType extrema;

      /**
       * Mode for location selection.
       *
       * You can select among different modes for selecting the locations
       * according to their strength in the value of the hessian determinant.
       *
       * This will affect the way in which the threshold value is interpreted.
       * See parameters::threshold for more information.
       *
       * Default value: Absolute
       */
      eLocationSelectionMode locationSelectionMode;

      /**
       * Threshold value.
       *
       * The meaning of this threshold value depends on the settings of the
       * locationSelectionMode.
       *
       * - If \c locationSelectionMode is set to \b All, then this value is
       *   ignored and all detected locations will be returned. 
       * - If \c locationSelectionMode is set to \b Absolute, then this value
       *   means the weakest strength a location can exhibit in order to be
       *   returned in the results.
       * - If \c locationSelectionMode is set to \b Relative, then this value
       *   means the fraction of the maximal location strengh that will be
       *   allowed.  This value has to be between 0 and 1.
       * - If \c locationSelectionMode is set to \b Conspicuous, then this
       *   value means the number of times the standard deviation will be
       *   multiplied with when added to the strengths mean value to compute
       *   the threshold.

       * - If \c locationSelectionMode is set to \b Number, then this value
       *   means the maximal number of locations that will be returned, where
       *   only the strongest locations will be returned.  This is the slowest
       *   method of all, since it requires to sort the values.
       *
       * Default value: 0.1f
       */
      float threshold;

      /**
       * Subsample levels
       *
       * The search for conspicuous locations can be made using all pixels in
       * all levels (value set to \c false) or just some selected samples
       * depending on the scale of a level (value set to \c true).
       *
       * Default value: true
       */
      bool subsampleLevels;

      /**
       * Initial sampling step
       *
       * If subsampleLevels is set to \c true, this is the initial sampling
       * step.  If subsampleLevels is set to \c false then all levels use this
       * value.
       *
       * This value must be 1 or greater.
       *
       * Default value: 2
       */
      int initialSamplingStep;
      //@}

      /**
       * @name Parameters for the estimation of the location orientation.
       *
       * The algorithm in the SURF paper states that on a circular window
       * around the detected location a set of samples is chosen for which some
       * gradient related values (computed through Haar filters) are computed
       * and used in a "weighted voting" approach.  Each sample votes in an
       * orientation window (or windows) that covers the angle of the
       * "gradient" and with a weight given by its magnitude.  At the end, an
       * orientation value is selected of the window with the largest
       * accumulated magnitude and with the angle of the accumulated values.
       */
      //@{
      /**
       * Selection flag used to deactivate orientation computation.
       *
       * In order to compute the so-called U-SURF descriptors, which lack of
       * orientation, we can save some time by deactivating the orientation
       * computation.
       *
       * To deactivate the computation of the orientation just set this
       * parameter to \c false.
       *
       * Default value: \c true
       */
      bool computeOrientation;

      /**
       * Factor used to compute radius of the circular neighborhood employed to
       * compute the orientation of a location.
       *
       * The real neighborhood radius will be computed multiplying the scale on
       * which a location was found by this factor.
       *
       * Default value: 6
       */
      float orientationNeighborhoodFactor;

      /**
       * Factor used to compute the samples within the orientation neighborhood
       * considered.
       *
       * The real sampling step is computed multiplying this factor with 
       * the scale on which a location has been found.
       *
       * The value \c orientationNeighborhoodFactor has to be divisible by
       * this number.
       *
       * Default value: 1
       */
      float orientationSamplingStepFactor;

      /**
       * Factor to compute the Haar wavelet side.
       *
       * The real wavelet size is computed multiplying this factor with
       * the scale on which the location has been found (always rounded to
       * the next odd size, to be centered on the location)
       *
       * Default value: 4
       */
      float orientationWaveletSizeFactor;

      /**
       * Gaussian weights for the orientation
       *
       * The wavelet responses on the samples in the neighborhood used to
       * compute the orientation are weighted with a gaussian that has an
       * standard deviation of this factor multiplied by the scale on which the
       * location has been found.
       *
       * Default value: 2.5
       */
      float orientationGaussianFactor;

      /**
       * Angular width for the orientation window.
       *
       * This is used as follows.  When a sample produces an angular value to
       * vote for, over that angular value a slice is placed with an angular
       * span of the value given here.  All windows that fall beneath that
       * slice will count the vote.  This means, a value of zero forces just
       * one window to be taken into consideration, as the original SURF paper
       * states.
       *
       * If this angular value is less than 2*Pi, then it is assumed to be
       * given in radians.  Otherwise is interpreted in degrees.
       *
       * The value must be positive or zero.
       *
       * In the original paper a value of zero is used, however, a value of 60
       * has proven (with orientationNumberOfWindows=6) to be much more
       * reliable.
       *
       * Default value: 60
       */
      float orientationWindowWidth;

      /**
       * Number of angular windows.
       *
       * Each window will contain angles in an interval of
       * orientationWindowWidth distributed homogeneously on the 2*Pi radians.
       *
       * Default value: 6
       */
      int orientationNumberOfWindows;
      //@}
    };

    /**
     * Default constructor
     */
    fastHessianDetection();

    /**
     * Construct a functor using the given parameters
     */
    fastHessianDetection(const parameters& par);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    fastHessianDetection(const fastHessianDetection& other);

    /**
     * Destructor
     */
    virtual ~fastHessianDetection();

    /**
     * Compute the locations based on the determinant of the "fast Hessian"
     * matrix.
     *
     * @param src channel8 with the source image.
     * @param locs lists of detected locations.
     *
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const channel8& src, 
                       list<location>& locs) const;

    /**
     * Compute the locations based on the determinant of the "fast Hessian"
     * matrix.
     *
     * @param src channel8 with the source image.
     * @param locs lists of detected locations.
     * @param strength the determinant value of the hessian matrix at
     *                 the center of the location.
     *
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel8& src, 
               list<location>& locs,
               list<float>& strength) const;

    /**
     * Compute the locations based on the determinant of the "fast Hessian"
     * matrix.
     *
     * @param src channel8 with the source image.
     * @param locs lists of detected locations.
     * @param strength the determinant value of the hessian matrix at
     *                 the center of the location.
     * @param numLocs number of location in \a locs and \a strength
     *
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel8& src, 
               list<location>& locs,
               list<float>& strength,
               int& numLocs) const;

    /**
     * Compute the locations based on the determinant of the "fast Hessian"
     * matrix.
     *
     * This method provides numLocs = locs.size() directly.  The reason is
     * simple: speed.  The size() method needs to count the elements, and
     * this is usually done in the process.
     *
     * @param src channel8 with the source image.
     * @param locs lists of detected locations.
     * @param numLocs number of locations detected.
     *
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const channel8& src, 
                       list<location>& locs,
                       int& numLocs) const;

    /**
     * Compute the locations based on the determinant of the "fast Hessian"
     * matrix.
     *
     * @param src channel8 with the source image.
     * @param locs lists of detected locations.
     *
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const channel& src, 
                       list<location>& locs) const;

    /**
     * Compute the locations based on the determinant of the "fast Hessian"
     * matrix.
     *
     * @param src channel8 with the source image.
     * @param locs lists of detected locations.
     * @param strength the determinant value of the hessian matrix at
     *                 the center of the location.
     *
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel& src, 
               list<location>& locs,
               list<float>& strength) const;

    /**
     * Compute the locations based on the determinant of the "fast Hessian"
     * matrix.
     *
     * @param src channel8 with the source image.
     * @param locs lists of detected locations.
     * @param strength the determinant value of the hessian matrix at
     *                 the center of the location.
     * @param numLocs number of location in \a locs and \a strength
     *
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel& src, 
               list<location>& locs,
               list<float>& strength,
               int& numLocs) const;


    /**
     * Compute the locations based on the determinant of the "fast Hessian"
     * matrix.
     *
     * This method provides numLocs = locs.size() directly.  The reason is
     * simple: speed.  The size() method needs to count the elements, and
     * this is usually done in the process.
     *
     * @param src channel8 with the source image.
     * @param locs lists of detected locations.
     * @param numLocs number of locations detected.
     *
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const channel& src,
                       list<location>& locs,
                       int& numLocs) const;

    /**
     * Sort the locations according to their strength, in descending
     * order: the strongest locations are always first.
     *
     * This is useful to take a fixed number of the strongest locations.
     *
     * The behaviour is influenced by the parameter settings of
     * parameters::extrema.  If it is set to Maxima, then the values are sorted
     * as given, in descending order; if set to Minima, then the values are
     * sorted in ascending order; if set to Both, then the absolute value of
     * the strength is sorted in descending order.
     *
     * @param locs the unsorted locations as provided by the apply method.
     * @param strength the corresponding strengths for the locations, as
     *                 provided by the apply method.  It has to have exactly
     *                 the same size as the locs list.
     * @param sortedLocs the sorted locations as vector, to have access to
     *                   the rank of the location
     * @param sortedStrength the sorted strength.
     *
     * @return \c true if successful or \c false otherwise.
     */
    bool sort(const list<location>& locs,
              const list<float>& strength,
              std::vector<location>& sortedLocs,
              fvector& sortedStrength) const;
    
    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    fastHessianDetection& copy(const fastHessianDetection& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    fastHessianDetection& operator=(const fastHessianDetection& other);

    /**
     * Returns the complete name of the functor class
     */
    virtual const std::string& name() const;

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual fastHessianDetection* clone() const;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual fastHessianDetection* newInstance() const;

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * Update parameters
     */
    bool updateParameters();

  private:
    /**
     * Compute the determinant of the approximated Hessian matrix.
     *
     * The template type T is usually int32 or float, which are the
     * result types of the integralImage functor.
     *
     * @param kernelSize size of the kernel used.  The associated variance
     *                   can be computed as 1.2*kernelSize/9
     * @param intImg the integral image of the input image
     */
    template<typename T>
    bool computeDeterminant(const int kernelSize,
                            const matrix<T>& intImg,
                            const integralImage& integrator,
                            const int sampleStep,
                            matrix<float>& det) const;

    /**
     * Compute the "pseudo-pyramid".
     *
     * The template type T is usually ubyte or float, which are the
     * possible channel types.
     *
     * @param chnl the original input channel
     * @param intImg integral image for the input channel
     * @param levels all the determinant channels for different scales.
     * @param kernelSizes sizes of kernels used for each level.  If you
     *                    need the scale \e s, it can be computed as 
     *                    \f$1,2 K_s / 9 = s \f$, Where K_s is the kernel
     *                    size.
     */
    template<typename T>
    bool computeLevels(const matrix<T>& chnl,
                       matrix<typename typeInfo<T>::accumulation_type>& intImg,
                       std::vector<fmatrix>& levels,
                       ivector& kernelSizes,
                       ivector& sampleSteps) const;
                       
    /**
     * This method searches for local extrema (minima and maxima) and tries
     * to improve the locations placement with interpolation.
     *
     * @param scale value of scale associated with the central level
     * @param prevLevel "previous" level, with a higher level of detail (lower
     *                  index in the pseudo-pyramid
     * @param level current level, corresponding to the given scale
     * @param nextLevel "next" level, with a lower degree of detail (higher
     *                  index in the pseudo-pyramid
     * @param locs list of detected locations.
     * @param strength determinant of the hessian matrix for the given location
     *                 Since this should not take much time, the value stored
     *                 is the one at the center of the 3x3x3 neighborhood, and
     *                 \e not the interpolated value for the maximum.
     */
    bool searchLevelExtremes(const float scalePrev,
                             const float scale,
                             const float scaleNext,
                             const fmatrix& prevLevel,
                             const fmatrix& level,
                             const fmatrix& nextLevel,
                             const int sampleStep,
                             list<location>& locs,
                             list<float>& strength,
                             int& numLocs) const;

    /**
     * This method removes weak locations, depending on the parameter settings
     * @param locs list of detected locations.
     * @param strength determinant of the hessian matrix for the given location
     *                 Since this should not take much time, the value stored
     *                 is the one at the center of the 3x3x3 neighborhood, and
     *                 \e not the interpolated value for the maximum.
     */
    bool selectLocations(list<location>& locs,
                         list<float>& strength,
                         int& numLocs) const;

    /**
     * Compute the orientations for each location.
     *
     * If the parameters::computeOrientation is set to false, this
     * method returns \c true without making any modifications to the locs.
     *
     * Otherwise, each location will be analyzed using the SURF suggested
     * method to detect the orientation.
     */
    template<typename T>
    bool computeOrientations(const matrix<T>& intImg,
                             list<location>& locs) const;
    
    /**
     * Internal class used to manage the orientation windows.
     */
    class orientationAccumulator;
    
    /**
     * Circle boundary LUT.
     *
     * This vector contains the index of the circle border of gaussWeights_
     */
    array<int> circLUT_;

    /**
     * Gaussian weights 
     */
    array2D<float> gaussWeights_;

    /**
     * Number of samples in the circular window used to compute the orientation
     */
    int wndSamples_;

    /**
     * Shadow for the initialKernelStep, ensuring that it is even and divisible
     * by three.
     */
    int kernelStep_;
    
    /**
     * Shadow for the initialKernelSize, ensuring that it is odd and divisible
     * by three.
     */
    int kernelSize_;

    /**
     * Shadow value of the parameters always in radians.
     */
    float orientationWindowWidth_;

    /**
     * Shadow value of the parameters 
     */
    int orientationNumberOfWindows_;

  };

  /**
   * Read a eExtremaType
   *
   * @ingroup gStorable
   */
  bool read(ioHandler& handler,fastHessianDetection::eExtremaType& data);

  /**
   * Write a eExtremaType
   *
   * @ingroup gStorable
   */
  bool write(ioHandler& handler,
             const fastHessianDetection::eExtremaType& data);
 

  /**
   * Read a fastHessianDetection::eLevelSelectionMethod
   *
   * @ingroup gStorable
   */
  bool read(ioHandler& handler,
            fastHessianDetection::eLevelSelectionMethod& data);

  /**
   * Write a fastHessianDetection::eLevelSelectionMethod
   *
   * @ingroup gStorable
   */
  bool write(ioHandler& handler,
             const fastHessianDetection::eLevelSelectionMethod& data);
  
  /**
   * Read a fastHessianDetection::eLevelSelectionMethod
   *
   * @ingroup gStorable
   */
  bool read(ioHandler& handler,
            fastHessianDetection::eLocationSelectionMode& data);

  /**
   * Write a fastHessianDetection::eLevelSelectionMethod
   *
   * @ingroup gStorable
   */
  bool write(ioHandler& handler,
             const fastHessianDetection::eLocationSelectionMode& data);
  
}

#endif

