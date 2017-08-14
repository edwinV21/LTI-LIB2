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
 * \file   ltiSegmentationGenetics.h
 *         Defines the abstract parent class of the genetic evaluation of
 *         segmentation algorithms.
 * \author Pablo Alvarado
 * \date   21.05.08
 *
 * $Id: ltiSegmentationGenetics.h,v 1.2 2008-06-03 20:35:27 alvarado Exp $
 */

#ifndef _LTI_SEGMENTATION_GENETICS_H_
#define _LTI_SEGMENTATION_GENETICS_H_

#include "ltiSecondOrderStatistics.h"
#include "ltiGenetics.h"
#include "ltiSegmentation.h"
#include "ltiImage.h"
#include "ltiChannel.h"
#include "ltiChannel8.h"

#include <vector>

namespace lti {
  /**
   * Segmentation Genetics.
   *
   * Base class for genetic classes that evaluate segmentation algorithms.
   *
   */
  class segmentationGenetics : public genetics {
  public:
    /**
     * Type used to represent chromosomes
     */
    typedef genetics::chromosome chromosome;

    /**
     * The parameters for the class segmentationGenetics.
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
       * - numberOfRegions
       * - numRegionsReciprocal (1/number of regions)
       * - pixelWisePotentialAccuracy
       * - objectWisePotentialAccuracy
       * - regionWiseInformationContent
       * - throughput (1/time)
       * - region integrity
       * - pixelWiseCertainty
       *
       * This sequence is important, since it determines the positions of
       * the single measures in the fitness output vector.
       *
       * If more fitness measures are selected than the inherited parameter
       * fitnessSpaceDimensionality, then the first ones in this sequence
       * will be selected.
       *
       * If less fitness measure are selected than the inherited parameter
       * fitnessSpaceDimensionality, then the rest of the vector will be
       * filled with zeros.
       */
      //@{
      /** 
       * Total number of regiong detected per image.
       *
       * This is usually a cost, and therefore you will never want to optimize
       * the number of regions as if it were a fitness, but, in some
       * applications, it may be necessary to search for the optimal
       * "over-segmentations", which will therefore use this as fitness.
       * 
       * Default value: false
       */
      bool numberOfRegions;

      /**
       * Reciprocal for the number of regions
       *
       * This is the inverse of the number of regions (1/number of regions).
       * Usually you want the get the less possible number of regions that
       * still don't merge different objects while partitioning an image.
       * 
       * This can be interpreted as the mean size of the regions normalized
       * to the size of the image.
       *
       * Default value: true
       */
      bool numRegionsReciprocal;

      /**
       * Pixel-wise potential accuracy.
       *
       * Describes the percentange of the image pixels than can be optimally
       * assigned to one of the ground-truth regions.
       *
       * This can be interpreted as a measure inversely proportional to the
       * degree of undersegmentation.
       *
       * Default value: true
       */
      bool pixelWisePotentialAccuracy;
      
      /**
       * Object-wise potential accuracy.
       *
       * Like pixel-wise potential accuracy but the contribution of each
       * reference region is normalized to its size so that all regions in
       * the reference segmentation are equally important.
       *
       * Default value: false
       */
      bool objectWisePotentialAccuracy;

      /**
       * Region-wise information content.
       *
       * Proportion of the reference region covered by each single region.
       *
       * Default value: false
       */
      bool regionWiseInformationContent;

      /**
       * Throughput
       *
       * Number of images segmented per second.
       *
       * Default value: false
       */
      bool throughput; 

      /**
       * Region integrity
       *
       * Measures the number of regions mapped to a reference region.  The
       * function used punishes over- and under-segmentation.
       * 
       * Default value: false
       */
      bool regionIntegrity;

      /**
       * Pixel-wise certainty
       *
       * This makes only sense for some segmentation functors, that can deliver
       * the certainty with which a pixel was assigned to a label.
       *
       * Default value: false
       */
      bool pixelWiseCertainty;
      //@}

      /**
       * Name of a file containing all images to be analyzed.
       *
       * Default value: "images.txt"
       */
      std::string images;

      /**
       * Mask postfix
       *
       * For each image file in \a images, a manual segmented mask is required
       * as "ground truth".
       *
       * The filename of the masks will be assumed to be the same
       * image name with the given postfix.  For example, if an image is
       * called "test.png" and the postfix is "-preseg", then the
       * mask will be assumed to be "test-preseg.png".
       *
       * Default value: "_premask"
       */
      std::string goldenPostfix;

      /**
       * Previous Mask Postfix.
       *
       * To save evaluation time, the test of higher-level algorithm can load
       * the lower-level segmentations of the images in a file with this
       * postfix and type lti (see lti::ltiFunctor), which is the only format
       * that can save imatrices.  For example, if an image is called
       * "test.png" and the prevStagePostfix is "-ibs", then the mask will
       * be assumed to be in "test-ibs.lti".
       *
       * If no file is found with these names, then a low-level segmentation 
       * will be assumed, which does not require any previous masks.
       *
       * Default value: "_ibs" 
       */
      std::string prevStagePostfix;

      /**
       * @name Fitness parameterizations
       */
      //@{
      /**
       * Minimal size for valid region
       *
       * Percentage of the image size that defines the minimal size of a
       * region to be considered in the statistics for region-wise information
       * content and object-wise potential accuracy.
       *
       * This value must be between 0 and 1.
       *
       * Default: 0.0005 (i.e. 0.05%)
       */
      float minValidRegionSize;
      //@}

    };

    /**
     * Default constructor.
     */
    segmentationGenetics();

    /**
     * Default constructor.
     */
    segmentationGenetics(const segmentationGenetics& other);

    /**
     * Destructor
     */
    virtual ~segmentationGenetics();

    /**
     * Return the fully qualified name of this class.
     */
    virtual const std::string& name() const = 0;

    /**
     * Clone method.
     */
    virtual segmentationGenetics* clone() const = 0;

    /**
     * New instance method.
     */
    virtual segmentationGenetics* newInstance() const = 0;

    /**
     * Copy method
     */
    segmentationGenetics& copy(const segmentationGenetics& other);

    /**
     * Alias of copy
     */
    segmentationGenetics& operator=(const segmentationGenetics& other);

    /**
     * Write the parameters in the given ioHandler.
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
     * Read the parameters from the given ioHandler.
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
    virtual segmentation::parameters* 
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

  public:
    /**
     * Evaluation of the location search with the given channel
     * using the given parameterization.
     */
    bool evaluate(const segmentation::parameters& param,
                  dvector& fitness) const;


    /**
     * @name Methods to compute scalar fitness
     */
    //@{
    /**
     * Evaluation of the location search with the given channel
     * using the given parameterization.
     */
    virtual bool evaluate(segmentation& segmenter,
                          const image& chnl,
                          const channel8& mask,
                          const segmentation::parameters& params,
                          dvector& fitness,
                          dvector& norm) const;

    /**
     * Evaluation of the segmentation with the given channel using
     * the given parameterization.
     *
     * The real measures for the single image are obtained with the
     * element-wise division between fitness and norm.  But if several images
     * are used in the computations, the norms of each image have to be added
     * first.
     */
    virtual bool evaluate(segmentation& segmenter,
                          const image& img,
                          const imatrix& prevStage,
                          const channel8& mask,
                          const segmentation::parameters& param,
                                dvector& fitness,
                                dvector& norm) const;

    /**
     * Evaluation of the segmentation with the given channel using
     * a previously set parameterization.
     *
     * The real measures for the single image are obtained with the
     * element-wise division between fitness and norm.  But if several images
     * are used in the computations, the norms of each image have to be added
     * first.
     */
    virtual bool evaluate(segmentation& segmenter,
                          const image& img,
                          const imatrix& prevStage,
                          const channel8& mask,
                          dvector& fitness,
                          dvector& norm) const;

    /**
     * Evaluation of segmentation
     *
     * Since you give already the results in this method, it is not possible to
     * compute the throughput and certainty measures.
     */
    virtual bool evaluate(const imatrix& result,
                          const channel8& refMask,
                          dvector& fitness,
                          dvector& norm,
                          const float minValidRegionSize=0.0005) const;

    /**
     * Attach the instance of segmentation to be used.
     *
     * This class will take care of the memory management
     */
    bool setSegmenter(const segmentation& instance);

    /**
     * Return a read-only reference to the internal segmentation instance
     *
     * Before calling this method you should verify with canSegment() if
     * a valid instance has been set yet.
     */
    const segmentation& getSegmenter() const;

    /**
     * Check if a valid segmentation instance has already been set.
     */
    bool canSegment() const;

  protected:
    /**
     * Instance of the proper segmentation class used.
     * 
     * This instance is used as a "seed" only, since multi-threading
     * optimization may produce some race conditions if the same instance is
     * used in all threads.  The real used instances are clones of this one.
     */
    segmentation* segmenter_;

    /**
     * Segmentate an image using the given segmentation functor.
     *
     * The default implementation works for most image-based segmentation
     * algorithms.
     *
     * @param segmenter instance to be used, with its parameters ready
     * @param img image to be segmented
     * @param prevStage segmentation mask from a previous stage.  Image-based
     *                  segmentation algorithms ignore this argument (in a
     *                  strict sense they consider a mask in which each pixel
     *                  has its own label).
     * @param mask new segmentation labeled mask.
     * @param certainty the certainty, with which the algorithm assigns each
     *                  pixel to its label.  If an algorithm does not provide
     *                  such measure (like almost all image-based algorithms)
     *                  this value will be empty, and for the computations
     *                  it will be assumed that all pixels have a certainty of
     *                  1.0 to be what they are.
     *
     * @return true if successful, false otherwise.
     */
    virtual bool segment(segmentation& segmenter,
                         const image& img,
                         const imatrix& prevStage,
                         imatrix& mask,
                         channel& certainty) const;

    /**
     * @name Evaluation Functions
     *
     * The real evaluation is provided here in order to compute measures
     * independently of the Pareto Front.
     */
    //@{
    /**
     * Index names for the complete multidimensional fitness measure
     */
    enum {
      IdxNumRegions,        /**< Mean number of regions.                   */
      IdxNumRegionsRecip,   /**< Reciprocal of the mean number of regions. */
      IdxPixelAccuracy,     /**< Pixel wise potential accuracy.            */
      IdxObjectAccuracy,    /**< Object wise potential accuracy.           */
      IdxRegionWiseInfo,    /**< Region wise information content.          */
      IdxThroughput,        /**< 1.0/time in s^-1, meaning the number of
                             *   complete segmentations per second.
                             */
      IdxRegionIntegrity,   /**< Region integrity                          */
      IdxPixelCertainty,    /**< Pixel-wise certainty                      */
      IdxTotalFitnessDim
    };

    //@}

    /**
     * Cache for the images
     */
    std::vector<image> imageData_;

    /**
     * Cache for the presegmentation masks
     */
    std::vector<channel8> maskData_;

    /**
     * Cache for the previous stage masks
     */
    std::vector<imatrix> prevMaskData_;

    /**
     * File names of the reference data
     */
    std::vector<std::string> goldenNames_;

    /**
     * Get the image name
     */
    std::string getMaskName(const std::string& imgName,
                            const std::string& postfix) const;

    /**
     * Get the image name of the previous stage file
     */
    std::string getPrevName(const std::string& imgName,
                            const std::string& postfix) const;
    /**
     * Dimension of the fitness space
     */
    int dimFitness_;
  };
    
}

#endif
