/*
 * Copyright (C) 2006
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

/**
 * \file   ltiFastHessianDetectionGenetics.h
 *         Defines the evaluation class for lti::fastHessianDetection 
 *         algorithm.
 * \author Pablo Alvarado
 * \date   04.12.07
 *
 * $Id: ltiFastHessianDetectionGenetics.h,v 1.1 2007-12-19 03:03:37 alvarado Exp $
 */

#ifndef _LTI_FAST_HESSIAN_DETECTION_GENETICS_H_
#define _LTI_FAST_HESSIAN_DETECTION_GENETICS_H_

#include "ltiFastHessianDetection.h"

#include "ltiInterval.h"
#include "ltiLocationDetectionGenetics.h"


namespace lti {

  /**
   * Evaluate the fastHessianDetection class for location detection after
   * the SURF paper.
   * 
   * @see lti::fastHessianDetection
   */
  class fastHessianDetectionGenetics : public locationDetectionGenetics {
  public:
    /**
     * Type used to represent chromosomes
     */
    typedef locationDetectionGenetics::chromosome chromosome;

    /**
     * The parameters for the class locationDetectionGenetics.
     *
     * These are the specific parameters of the evaluation concepts,
     * where filenames, which fitness scalars compose the aggregate
     * fitness, and so on.
     */
    class parameters : public locationDetectionGenetics::parameters {
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
       * Returns name of this type
       */
      virtual const std::string& name() const;

      /**
       * Returns a pointer to a clone of the parameters
       */
      virtual parameters* clone() const;

      /**
       * Returns a pointer to a clone of the parameters
       */
      virtual parameters* newInstance() const;

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
       * The number of levels used.
       *
       * Maximum allowed range from 3 to 34
       */
      interval<int> numberOfLevels;
      
      /**
       * Initial kernel size
       *
       * Maximum allowed range from 3 to 45
       */
      interval<int> initialKernelSize;

      /**
       * Level selection method
       *
       * Maximum allowed range from Blocks to Exponential
       */
      interval<fastHessianDetection::eLevelSelectionMethod>
      levelSelectionMethod;

      /**
       * Initial kernel step
       *
       * Maximum allowed range from 6 to 48
       */
      interval<int> initialKernelStep;

      /**
       * Level group size 
       *
       * Maximum allowed range from 2 to 17
       */
      interval<int> levelGroupSize;

      /**
       * Norm power
       *
       * Maximum allowed range from 3.5 to 4.5
       */
      interval<float> normPower;

      /**
       * Extrema
       *
       * Maximum allowed range from Minima to Both
       */
      interval<fastHessianDetection::eExtremaType> extrema;

      /**
       * Location selection mode
       *
       * Maximum allowed range from All to Number
       */
      interval<fastHessianDetection::eLocationSelectionMode>
      locationSelectionMode;

      /**
       * Threshold
       *
       * The threshold changes its meaning depending on the 
       * location selection mode set.  The value is always given normalized.
       *
       * - For Absolute, this value is scaled by 1000
       * - For Relative it is used as given
       * - For Conspicuous this value is scaled by 4
       * - For Number this value is multiplied by 1000
       *
       * Maximum allowed range from 0.0 to 1.0f
       */
      interval<float> threshold;

      /**
       * Subsample levels
       *
       * Maximum allowed range from \c false to \c true
       */
      interval<bool> subsampleLevels;

      /**
       * Initial Sampling Step
       *
       * Maximum allowed range from 1 to 32
       */
      interval<int> initialSamplingStep;

      /**
       * Compute Orientation
       *
       * Maximum allowed range from false to true
       */
      interval<bool> computeOrientation;

      /**
       * Orientation: Neighborhood Factor
       *
       * Maximum allowed range from 1 to 32
       */
      interval<int> orientationNeighborhoodFactor;

      /**
       * Orientation: Sampling Step Factor
       *
       * Maximum allowed range from to 1 to 16
       */
      interval<int> orientationSamplingStepFactor;

      /**
       * Orientation: Wavelet Size Factor
       *
       * Maximum allowed range from 1 to 16
       */
      interval<int> orientationWaveletSizeFactor;

      /**
       * Orientation: Gaussian Factor
       *
       * Maximum allowed range from 0.1 to 10.0
       */
      interval<float> orientationGaussianFactor;

      /**
       * Orientation: Window Width
       *
       * The interval is always given in degrees
       *
       * Maximum allowed range from 360/256 to 360/1
       */
      interval<float> orientationWindowWidth;

      /**
       * Orientation: Number Of Windows
       *
       * Maximum allowed range from 1 to 64
       */
      interval<int> orientationNumberOfWindows;
    };


    /**
     * Default constructor.
     */
    fastHessianDetectionGenetics();

    /**
     * Default constructor.
     */
    fastHessianDetectionGenetics(const parameters& par);

    /**
     * Copy constructor.
     */
    fastHessianDetectionGenetics(const fastHessianDetectionGenetics& other);


    /**
     * Destructor
     */
    virtual ~fastHessianDetectionGenetics();

    /**
     * Return the fully qualified name of this class.
     */
    virtual const std::string& name() const;

    /**
     * Clone method.
     */
    virtual fastHessianDetectionGenetics* clone() const;

    /**
     * New instance method.
     */
    virtual fastHessianDetectionGenetics* newInstance() const;

    /**
     * Returns parameters in use.
     */
    const parameters& getParameters() const;

    /**
     * @name Public methods reimplemented
     *
     * Following methods need to be reimplemented to evaluate specific
     * algorithms.
     */
    //@{

    /**
     * Convert a binary-chain representation of a chromosome to a valid
     * parameter object.
     *
     * There are some tools to convert standard types into some binary chains,
     * which can be used by all derived classes:
     * - binToInt()
     * - binToUInt()
     * - binToDouble()
     */
    virtual bool 
    chromosomeToPhenotype(const chromosome& genotype,
                                functor::parameters& phenotype) const;

    /**
     * Return a fresh allocated parameters for the evaluated functor, which is
     * equivalent to the given genotype.
     *
     * There are some tools to convert binary chains into standard types:
     * - intToBin()
     * - uintToBin()
     * - doubleToBin()
     */
    virtual fastHessianDetection::parameters* 
    chromosomeToPhenotype(const chromosome& genotype) const;

    /**
     * Convert a valid parameters object (phenotype) into binary-chain
     * representation of a chromosome.
     */
    virtual bool 
    phenotypeToChromosome(const functor::parameters& phenotype,
                                chromosome& genotype) const;

    /**
     * Return the length in bits for a chromosome.
     *
     * This method needs to be reimplemented, in order to get some 
     * default implementations to work.
     */
    virtual int getChromosomeSize() const;
    //@}

  protected:

    /**
     * Number of bits for each attribute of the parameters
     */
    enum {
      BitsForNumberOfLevels       = 5,
      BitsForInitialKernelSize    = 3,
      BitsForLevelSelectionMethod = 1,
      BitsForInitialKernelStep    = 3,
      BitsForLevelGroupSize       = 4,
      BitsForNormPower            = 5,
      BitsForExtrema              = 2,
      BitsForLocationSelectionMode= 3,
      BitsForThreshold            = 16,
      BitsForSubsampleLevels      = 1,
      BitsForInitialSamplingStep  = 5,
      BitsForComputeOrientation   = 1,
      BitsForOrientationNeighborhoodFactor = 5,
      BitsForOrientationSamplingStepFactor = 4,
      BitsForOrientationWaveletSizeFactor  = 4,
      BitsForOrientationGaussianFactor     = 8,
      BitsForOrientationWindowWidth        = 8,
      BitsForOrientationNumberOfWindows    = 6
    };
  };

}

#endif
