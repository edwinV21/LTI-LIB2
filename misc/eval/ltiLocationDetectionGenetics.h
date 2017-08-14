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
 * \file   ltiLocationDetectionGenetics.h
 *         Defines the abstract parent class of the genetic evaluation of
 *         locationDetection derived classes.
 * \author Pablo Alvarado
 * \date   02.12.07
 *
 * $Id: ltiLocationDetectionGenetics.h,v 1.5 2010-03-28 13:32:46 alvarado Exp $
 */

#ifndef _LTI_LOCATION_DETECTION_GENETICS_H_
#define _LTI_LOCATION_DETECTION_GENETICS_H_

#include "ltiSecondOrderStatistics.h"
#include "ltiGenetics.h"
#include "ltiLocationDetection.h"
#include "ltiChannel.h"
#include "ltiChannel8.h"
#include "ltiKdTree.h"
#include "ltiBilinearInterpolation.h"
#include "ltiMatrixTransform.h"

#include <vector>

namespace lti {
  /**
   * Base class for genetic classes that evaluate location detection functors.
   *
   * All algorithms will be tested with lti::channel (float valued pixels).
   *
   * \warning The locationDetection algorithms will be verified using a set of
   * images.  Since it would be extremely expensive to load each time
   * the set of images, then they will be internally cached.  Please
   * consider this when deciding how big your image set should be (in regards
   * to the computer memory).
   *
   * This class permits to evaluate the parameterization of the
   * lti::locationDetection inherited classes.
   *
   * The possible fitness criteria used here are:
   * - number of locations
   * - reciprocal of location  number (1/number of locations)
   * - number of occupied levels in the pyramid
   * - reciprocal of the time taken to extract all locations
   * - scale repeatability, i.e. considering scale changes only
   * - rotation repeatability, i.e. considering rotations only
   * - rotation scale repeatability, i.e. considering scale and rotation 
   * - scale stable locations, i.e. considering scale changes only
   * - rotation stable locations, i.e. considering rotations only
   * - rotation scale stable locations, i.e. considering scale and rotation 
   * 
   * For the evaluation of a parameterization the algorithm takes all
   * images stored in the file specified in parameters::images.  For
   * each one of them a set of rotations and scalings are applied to
   * the image and the locations estracted from it.  Since the geometric
   * transformation is known, we have a "should-be" set of locations, obtained
   * transforming the original image locations.  This set is compared with
   * the locations extracted from the transformed image.  The correspondences
   * are counted and they constitute in percentage the fitness measure.
   *
   * The "repeatability" measures are equal to the "stable locations"
   * normalized by the total number of locations.

   */
  class locationDetectionGenetics : public genetics {
  public:
    /**
     * Type used to represent chromosomes
     */
    typedef genetics::chromosome chromosome;

    /**
     * The parameters for the class locationDetectionGenetics.
     *
     * These are the specific parameters of the evaluation concepts,
     * where filenames, which fitness scalars compose the aggregate
     * fitness, and so on.
     */
    class parameters : public genetics::parameters {
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
       * @name Fitness measures
       *
       * Value containing the 1D fitness measures to be computed.
       * The constants defined here are:
       * - numberOfLocations
       * - locationsReciprocal (1/number of locations)
       * - occupiedLevels
       * - time
       * - scaleRepeatability
       * - rotationRepeatability
       * - rotationScaleRepeatability
       * - scaleStable
       * - rotationStable
       * - rotationScaleStable
       *
       * This sequence is relevant, since it determines the positions of
       * the single measures in the fitness output vector.
       *
       * If more fitness measures are selected than the inherited parameter
       * fitnessSpaceDimensionality, then the first ones in this sequence
       * will be selected.
       *       
       * The difference between the "*Repeatibility" and the "*Stable" measures
       * is that the first ones are the number of stable locations normalized
       * to the total number of locations, i.e. their maximal value will be
       * always 1.0.  The "*Stable" unnormalized fitness measures may be
       * important to optimize in those applications that require many 
       * stable locations to work, and not only the major part of them.
       */
      //@{
      /** 
       * Total number of locations detected per image.
       *
       * Default value: false
       */
      bool numberOfLocations;

      /**
       * Locations' reciprocal
       *
       * This is the inverse of the number of location (1/number of locations).
       * It is sometimes useful to search of parameterizations that produce a
       * low number of locations (while also maximizing the percentage of
       * stable locations in rotation, position and scale).
       *
       * The used value will be 1/(1+\#locs) to avoid * divisions by zero.
       *
       * Default value: false
       */
      bool locationsReciprocal;

      /**
       * Different location detection approaches may use the concept of "level"
       * differently.  However, most of the assign a scale (radius) to each
       * location.  A possibility to check how much of the scale axis of the
       * scale-space is covered by an approach is given by the range of octaves
       * being used.
       *
       * Default value: false
       */
      bool occupiedOctaves;
      
      /**
       * Mean time required to extract the locations
       *
       * Default value: true
       */
      bool time;

      /**
       * Repeatability considering only image scaling
       *
       * Default value: false
       */
      bool scaleRepeatability;

      /**
       * Repeatability considering only image rotation 
       *
       * Default value: false
       */
      bool rotationRepeatability; 
      
      /**
       * Repeatability considering both rotation and scaling
       *
       * Default value: true
       */
      bool rotationScaleRepeatability;

      /**
       * Absolute number of stable locations considering only image scaling
       *
       * Default value: false
       */
      bool scaleStable;

      /**
       * Absolute number of stable locations considering only image rotation 
       *
       * Default value: false
       */
      bool rotationStable; 
      
      /**
       * Absolute number of stable locations considering both rotation and
       * scaling
       *
       * Default value: true
       */
      bool rotationScaleStable;

      //@}

      /**
       * Name of a file containing all images to be analyzed.
       *
       * Default value: "images.txt"
       */
      std::string images;

      /**
       * Mask posfix
       *
       * For each image file in \a images, a manual segmented mask can
       * be used to ignore those locations outside the objects of interest.
       *
       * The filename of the masks will be assumed to be the same
       * image name with the given postfix.  For example, if a image is
       * called "test.png" and the postfix is "-preseg", then the
       * mask will be assumed to be "test-preseg.png".
       *
       * If left empty, the whole image will be used.
       *
       * Default value: ""
       */
      std::string postfix;

      /**
       * Additive angle change.
       *
       * The interval from firstAngle to lastAngle will be divided in
       * subintervals with a width stepAngle.
       *
       * If the value lies in the interval from 0 to 2*Pi, then it will be
       * assumed to be given in radians, but if the value is greater than 2*Pi,
       * it will be assumed to be in degrees.
       *
       * Default value: lti::degToRad(10), i.e. 10 degrees in radians.
       */
      float stepAngle;

      /**
       * Multiplicative scaling change.
       *
       * The scaling interval will begin with firstScaling, which will be
       * multiplied each type by stepScaling until last scaling is reached.
       *
       * This value must be greater than 1.
       *
       * Default value: sqrt(2)
       */
      float stepScaling;

      /**
       * First scaling of the image will downsample it by a factor of 4
       *
       * Default value: 0.25
       */
      float firstScaling;

      /**
       * Last scaling of the image will be an upsampling by a factor of 2
       *
       * Default value: 2
       */
      float lastScaling;

      /**
       * First angle
       *
       * If the value lies in the interval from 0 to 2*Pi, then it will be
       * assumed to be given in radians, but if the value is greater than 2*Pi,
       * it will be assumed to be in degrees.
       *
       * Default value 0
       */
      float firstAngle;

      /**
       * Last rotation angle
       *
       * If the value lies in the interval from 0 to 2*Pi, then it will be
       * assumed to be given in radians, but if the value is greater than 2*Pi,
       * it will be assumed to be in degrees.
       *
       * Default value: 2*Pi
       */
      float lastAngle;

      /**
       * If true, a location will be considered to match only if position AND
       * rotation lie within the tolerance levels.  If false, only the position
       * will be considered while analyzing the repeatability
       *
       * Default value: true
       */
      bool considerAngle;

      /**
       * Tolerance for scale deviation.
       *
       * Between two candidate matching locations, the match will be accepted
       * only if the ratio between scale of both locations does not goes
       * beneath the given tolerance value, which must be always smaller than
       * one.
       * 
       * Default value: 1/sqrt(2)
       */
      float scaleTolerance;

      /**
       * Tolerance for position deviation.
       *
       * Between two candidate matching locations, the match will be accepted
       * only if the distance between both locations does not exceed the given
       * tolerance value (in pixels), which must be always positive.
       * 
       * Default value: 1.5f
       */
      float positionTolerance;

      /**
       * Angular tolerance.
       *
       * Two locations, which coincide in position and scale, match together
       * if the angular difference is smaller than the given angle.
       * 
       * This must be given in radians, but if you give a value greater than
       * 2*Pi, it will be assumed to be in degrees.
       *
       * Default value: lti::degToRad(10), i.e. 10 degrees in radians.
       */
      float angleTolerance;
    };

    /**
     * Default constructor.
     */
    locationDetectionGenetics();

    /**
     * Default constructor.
     */
    locationDetectionGenetics(const locationDetectionGenetics& other);

    /**
     * Destructor
     */
    virtual ~locationDetectionGenetics();

    /**
     * Return the fully qualified name of this class.
     */
    virtual const std::string& name() const = 0;

    /**
     * Clone method.
     */
    virtual locationDetectionGenetics* clone() const = 0;

    /**
     * New instance method.
     */
    virtual locationDetectionGenetics* newInstance() const = 0;

    /**
     * Copy method
     */
    locationDetectionGenetics& copy(const locationDetectionGenetics& other);

    /**
     * Alias of copy
     */
    locationDetectionGenetics&
    operator=(const locationDetectionGenetics& other);

    /**
     * Write the parameters in the given ioHandler
     *
     * @param handler the ioHandler to be used
     * @param complete if true (the default) the enclosing begin/end will
     *        be also written, otherwise only the data block will be written.
     * @return true if write was successful
     *
     * This method has to be overloaded.  If not it always returns false.
     */
    virtual bool write(ioHandler& handler, const bool complete=true) const;

    /**
     * Read the parameters from the given ioHandler
     *
     * @param handler the ioHandler to be used
     * @param complete if true (the default) the enclosing begin/end will
     *        be also written, otherwise only the data block will be written.
     * @return true if write was successful
     *
     * This method has to be overloaded.  If not it always returns false.
     */
    virtual bool read(ioHandler& handler,const bool complete=true);

    /**
     * Returns parameters in use.
     */
    const parameters& getParameters() const;

    /**
     * Update parameters
     *
     * Among other things, here the internal channel cache is initialized.
     */
    virtual bool updateParameters();

    /**
     * @name Public methods to be reimplemented
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
                                functor::parameters& phenotype) const=0;

    /**
     * Return a fresh allocated parameters for the evaluated functor, which is
     * equivalent to the given genotype.
     *
     * There are some tools to convert binary chains into standard types:
     * - intToBin()
     * - uintToBin()
     * - doubleToBin()
     */
    virtual locationDetection::parameters* 
    chromosomeToPhenotype(const chromosome& genotype) const = 0;

    /**
     * Convert a valid parameters object (phenotype) into binary-chain
     * representation of a chromosome.
     */
    virtual bool phenotypeToChromosome(const functor::parameters& phenotype,
                                             chromosome& genotype) const=0;

    /**
     * Return the length in bits for a chromosome.
     *
     * This method needs to be reimplemented, in order to get some 
     * default implementations to work.
     */
    virtual int getChromosomeSize() const = 0;

    /**
     * Evaluate Chromosome
     *
     * This method is one of the most important ones for any genetic algorithm.
     * Its task is to produce a multidimensional fitness measure for a given
     * chromosome.
     *
     * It returns true if the evaluation was successful, of false if the
     * phenotype represents some invalid parameterization.  It is highly 
     * recomended that the mutation and crossover methods are reimplemented to 
     * avoid invalid parameterizations.
     *
     * There are mainly two types of fitness measures that can be analyzed:
     * empirical goodness and empirical discrepancy (Zhang).  The empirical
     * goodness computes some measure using exclusively the test data, without
     * requiring any ground truth.  The empirical discrepancy assumes the
     * existency of ground truth and provides as measure some distance between
     * the result of an algorithm and the ground truth.  Each class derived
     * from genetics should specify clearly which kind of fitness measures it
     * provides.
     *
     * Since the implementations of genetic algorithms are highly parallel,
     * ensure that this method can be called by several threads without causing
     * any conflicts (avoid the use of static methods and protect the internal
     * state of your inherited classes).
     *
     * @param individual is the encoded parameter object to be evaluated.
     * @param fitness is the result of the evaluation.
     * @return true if the evaluation was successful, or false if it failed.
     */
    virtual bool evaluateChromosome(const chromosome& individual,
                                          dvector& fitness) const; 

    /**
     * Evaluate Chromosome
     *
     * This method is one of the most important ones for any genetic algorithm.
     * Its task is to produce a multidimensional fitness measure for a given
     * chromosome.
     *
     * It returns true if the evaluation was successful, of false if the
     * phenotype represents some invalid parameterization.  It is highly
     * recomended that the mutation and crossover methods are reimplemented to
     * avoid invalid parameterizations.
     *
     * There are mainly two types of fitness measures that can be analyzed:
     * empirical goodness and empirical discrepancy (Zhang).  The empirical
     * goodness computes some measure using exclusively the test data, without
     * requiring any ground truth.  The empirical discrepancy assumes the
     * existency of ground truth and provides as measure some distance between
     * the result of an algorithm and the ground truth.  Each class derived
     * from genetics should specify clearly which kind of fitness measures it
     * provides.
     *
     * Since the implementations of genetic algorithms are highly parallel,
     * ensure that this method can be called by several threads without causing
     * any conflicts (avoid the use of static methods and protect the internal
     * state of your inherited classes).
     *
     * @param id identification of the iteration number.  This parameter is 
     *           used in parallel environments to identify the number of the
     *           "thread" or "process" being run.
     *           The id will be always between 0 and the size of the internal
     *           population-1.
     * @param individual is the encoded parameter object to be evaluated.
     * @param fitness is the result of the evaluation.
     * @return true if the evaluation was successful, or false if it failed.
     */
    virtual bool evaluateChromosome(const int id,
                                    const chromosome& individual,
                                    dvector& fitness) const;
    //@}

    /**
     * Attach the instance of the location dectector to be used.
     *
     * This class will take care of the memory management
     */
    bool setDetector(const locationDetection& instance);

    /**
     * Return a read-only reference to the internal denoiser
     *
     * Before calling this method you should verify with canDenoise() if
     * a valid instance has been set yet.
     */
    const locationDetection& getDetector() const;

    /**
     * Check if a valid locationDetection instance has already been set.
     */
    bool canDetect() const;

  protected:
    /**
     * Instance of the proper denoiser class used
     */
    locationDetection* detector_;

  public:
    /**
     * @name Methods to compute scalar fitness
     */
    //@{
    /**
     * Evaluation of the location search with the given channel
     * using the given parameterization.
     */
    bool evaluate(const channel& chnl,
                  const channel8& mask,
                  dvector& fitness,
                  locationDetection& detector) const;

    /**
     * Evaluate the set of images (in the parameters) using the
     * given parameterization.
     *
     * @param param parameters of the location selector to be used in
     *              the evaluation.
     * @param fitness multidimensional fitness measures for each image in
     *                one row.
     * @param onlyMeanAndVariance if true, the mean and variance of all
     *        results will be computed.  Otherwise all fitness measures
     *        for all images will be provided.
     */
    bool evaluate(const locationDetection::parameters& param,
                  dmatrix& fitness,
                  const bool onlyMeanAndVariance = false) const;

  protected:

    /**
     * Compare both location sets.
     *
     * @param olocsTree kd-Tree with the locations of the original image.
     *                  The data at each element correspond to the location
     *                  index in olocs.
     * @param olocs original image location set.
     * @param blocs back-transformed location set from the location extracted
     *              from the transformed image.
     * @param minScale minimum scale present in the locations.
     * @param posStable number of locations stable respect their position
     * @param rotStable number of locations also stable in detected
     *                  rotation angle (rotStable <= posStable).
     * @return true if successful, false otherwise.
     */
    bool compare(const kdTree<fpoint,int>& olocsTree,
                 const std::vector<location>& olocs,
                 const std::vector<location>& blocs,
                 const float minScale,
                 int& posStable,
                 int& rotStable) const;

    //@}

    /**
     * Original images
     */
    std::vector<channel> origs_;
    
    /**
     * Masks for images
     */
    std::vector<channel8> masks_;

    /**
     * Get the name of the mask that corresponds to the given imgName
     */
    std::string getMaskName(const std::string& imgName,
                            const std::string& postfix) const;

    /**
     * Dimension of the fitness space
     */
    int dimFitness_;

    /**
     * Index names for the complete multidimensional fitness measure
     */
    enum {
      IdxTime,              /**< Well, this is not really time, because that
                             *   would be "cost" instead of "fitness", in
                             *   reality, this dimension contains 1.0/time in
                             *   s^-1, meaning the number of complete
                             *   location extractions per second.
                             */
      IdxNumberOfLocations, /**< Mean number of extracted locations 
                             */
      IdxLocationsRecip,    /**< Location reciprocal.
                             *
                             *   Inverse of the number of locations detected.
                             */
      IdxOccupiedOctaves,   /**< Mean number of occupied levels.
                             */
      IdxScalePosRep,       /**< Scale repeatability defined as total
                             *   number of stable locations divided by
                             *   the total number of locations for
                             *   changes of scaling only.
                             */
      IdxRotationPosRep,    /**< Rotation repeatability defined as
                             *   total number of stable locations
                             *   divided by the total number of
                             *   locations for changes in the rotation
                             *   only.
                             */
      IdxRSPosRep,          /**< Rotation repeatability defined as
                             *   total number of stable locations
                             *   divided by the total number of
                             *   locations for changes in the rotation
                             *   and scale. This will only be computed
                             *   if the RSRepeatability bit in the parameters
                             *   is enabled (it costs too much time!).
                             */
      IdxScaleARep,         /**< Scale angular repeatability defined as
                             *   total number of angular stable
                             *   locations divided by the total number
                             *   of locations for changes of scaling
                             *   only.
                             */
      IdxRotationARep,      /**< Rotation angular repeatability defined
                             *   as total number of angular stable
                             *   locations divided by the total number
                             *   of locations for changes in the
                             *   rotation only.
                             */
      IdxRSARep,            /**< Angular repeatability defined as total
                             *   number of angular stable locations
                             *   divided by the total number of
                             *   locations for changes in the rotation
                             *   and scale.  This will only be computed
                             *   if the RSRepeatability bit in the parameters
                             *   is enabled (it costs too much time!).
                             */
      IdxScalePosStable,    /**< Scale stability defined as total
                             *   number of stable locations divided by
                             *   the total number of locations for
                             *   changes of scaling only.
                             */
      IdxRotationPosStable, /**< Rotation stability defined as
                             *   total number of stable locations
                             *   divided by the total number of
                             *   locations for changes in the rotation
                             *   only.
                             */
      IdxRSPosStable,       /**< Rotation stability defined as
                             *   total number of stable locations
                             *   divided by the total number of
                             *   locations for changes in the rotation
                             *   and scale. This will only be computed
                             *   if the RSStability bit in the parameters
                             *   is enabled (it costs too much time!).
                             */
      IdxScaleAStable,      /**< Scale angular stability defined as
                             *   total number of angular stable
                             *   locations divided by the total number
                             *   of locations for changes of scaling
                             *   only.
                             */
      IdxRotationAStable,   /**< Rotation angular stability defined
                             *   as total number of angular stable
                             *   locations divided by the total number
                             *   of locations for changes in the
                             *   rotation only.
                             */
      IdxRSAStable          /**< Angular stability defined as total
                             *   number of angular stable locations
                             *   divided by the total number of
                             *   locations for changes in the rotation
                             *   and scale.  This will only be computed
                             *   if the RSStability bit in the parameters
                             *   is enabled (it costs too much time!).
                             */
    };

    /**
     * Total number of scalar fitness measures that will be computed
     * 
     */
    static const int totalFitnessDimensionality_ = 16;

    /**
     * Computation of statistics
     */
    secondOrderStatistics<double> stats_;

    /**
     * Type of class used to rotate and scale the channels
     */
    typedef lti::matrixTransform< float > trans_type;

    /**
     * Transformation of the images
     */
    trans_type trans_;
  };
    
}

#endif
