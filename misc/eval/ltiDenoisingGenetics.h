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
 * \file   ltiDenoisingGenetics.h
 *         Defines the abstract parent class of the genetics for denoising 
 *         algorithm.
 * \author Pablo Alvarado
 * \date   19.08.06
 *
 * $Id: ltiDenoisingGenetics.h,v 1.7 2008-06-03 17:36:29 alvarado Exp $
 */

#ifndef _LTI_DENOISING_GENETICS_H_
#define _LTI_DENOISING_GENETICS_H_

#include "ltiSecondOrderStatistics.h"
#include "ltiGenetics.h"
#include "ltiDenoising.h"
#include "ltiNoise.h"
#include "ltiChannel.h"
#include <vector>

namespace lti {
  /**
   * Base class for genetic classes that evaluate denoising algorithms.
   *
   * All algorithms will be tested with lti::channel (float valued pixels).
   *
   * \warning The denoising algorithms will be verified using a set of
   * images.  Since it would be extremely expensive to load each time
   * the set of images, then they will be internally cached.  Please
   * consider this when deciding how big your image set should be.
   */
  class denoisingGenetics : public genetics {
  public:
    /**
     * Type used to represent chromosomes
     */
    typedef genetics::chromosome chromosome;

    /**
     * The parameters for the class denoisingGenetics.
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
       * Flags indicating which scalar fitness measures are going to
       * be computed.
       *
       * The possible scalars are:
       * - SNR: improvement in signal to noise ratio.
       * - MSE: 1/mean square error
       * - Speed: how fast is the algorithm (images per second)
       * - method noise scalar: 1/(Average Local Variance)
       */
      //@{
      /** 
       * Signal to noise ratio.
       *
       * The signal to noise ratio is computed as the ratio of the
       * standard deviation of signal vs. the std. deviation of the noise.
       *
       * The fitness value computed if this attribute is set to \c true is
       * the improvement of the SNR.
       *
       * Default value: false
       */
      bool snr;

      /** 
       * Mean square error.
       *
       * This is computed as the mean value of the square of the difference in
       * pixel value between the output of the algorithm and the image without
       * noise.  Since fitness is required (and not cost) the used value is
       * 1/mean square error, i.e. the greater the error the less fit is the
       * algorithm.
       *
       * Default value: true
       */
      bool mse;

      /** 
       * Speed.
       *
       * Good noise reduction is usually computationally expensive.  This
       * measures how many images per second (of the test set) can be computed.
       *
       * Default value: false
       */
      bool speed;

      /**
       * Method noise.
       *
       *
       * The method noise is based on the difference between the noise image
       * and the denoise image, where the goal is that the resultand image
       * look like white noise and should contains as little structure as
       * posible
       *
       * Default value: true
       */
      bool methodNoise;
      //@}

      /**
       * Name of a file containing all images to be analyzed.
       *
       * If you give just one name, the file will be located at the
       * current working directory.  You can also specify a path.
       *
       * Default value: "images.txt"
       */
      std::string images;

      /**
       * Noise postfix.
       *
       * For each image file in \a images, a corresponding image with
       * added noise can be just loaded or can be generated.  If you
       * want to load them then images have to exist with the same name as
       * the original image but with the postfix indicated in this attribute.
       *
       * For example, if an image is called "test.png" and the postfix
       * is "-noised", then the contaminated image will be assumed to be named
       * "test-noised.png".
       *
       * If you provide an empty string, then the noise will be added
       * locally, using the lti::noise functor with the
       * parameterization specified with the parameters::noiseParams.
       *
       * Default value: empty
       */
      std::string postfix;

      /**
       * Noise parameters
       *
       * If the \a noisePostfix attribute is set to an empty matrix,
       * then the contaminated images will be automatically generated.
       */
      noise::parameters noiseParams;
    };

    /**
     * Default constructor.
     */
    denoisingGenetics();

    /**
     * Default constructor.
     */
    denoisingGenetics(const denoisingGenetics& other);

    /**
     * Destructor
     */
    virtual ~denoisingGenetics();

    /**
     * Return the fully qualified name of this class.
     */
    virtual const std::string& name() const;

    /**
     * Clone method.
     */
    virtual denoisingGenetics* clone() const = 0;

    /**
     * New instance method.
     */
    virtual denoisingGenetics* newInstance() const = 0;

    /**
     * Copy method
     */
    denoisingGenetics& copy(const denoisingGenetics& other);

    /**
     * Alias of copy
     */
    denoisingGenetics& operator=(const denoisingGenetics& other);

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
    virtual denoising::parameters* 
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
     * Attach the instance of the denoiser to be used.
     *
     * This class will take care of the memory management
     */
    bool setDenoiser(const denoising& instance);

    /**
     * Return a read-only reference to the internal denoiser
     *
     * Before calling this method you should verify with canDenoise() if
     * a valid instance has been set yet.
     */
    const denoising& getDenoiser() const;

    /**
     * Check if a valid denoising instance has already been set.
     */
    bool canDenoise() const;

  protected:
    /**
     * Instance of the proper denoiser class used
     */
    denoising* denoiserInstance_;

    /**
     * @name Methods to compute scala fitness
     */
    //@{

    /**
     * This function computes the SNR  (Signal to Noise Ratio)
     *
     * @param index of the images in the cache
     * @param diff difference between the original clean image and the output 
     *             of the denoising algorithm.
     */
    double snr(const int index,
               const fmatrix& diff) const;
    
    /**
     * This function computes the MSE (Mean Square Error) 
     *
     * @param diff difference between the original clean image and the output 
     *             of the denoising algorithm.
     */
    double mse(const fmatrix& diff) const;

    /**
     * This function computes the method noise scalar.
     *
     * @param diff difference matrix between two channels/matrices.
     * @return the method noise corresponding to the differences channel.
     */
    double methodNoise(const fmatrix& diff) const;
    //@}

    /**
     * Scale method noise image to the range 0-1 
     */
    static float scale (const float value);

    /**
     * Original images
     */
    std::vector<channel> origs_;
    
    /**
     * Noisy images
     */
    std::vector<channel> noisy_;

    /**
     * Standard Deviation of Noise
     */
    fvector stdevNoise_;

    /**
     * Standard Deviation of Original Images
     */
    fvector stdevImage_;

    /**
     * Construct the noisy image name from the original image name
     */
    std::string getNoiseName(const std::string& imgName,
                             const std::string& postfix) const;

    /**
     * Dimension of the fitness space
     */
    int dimFitness_;

    /**
     * Computation of statistics
     */
    secondOrderStatistics<float> stats_;
  };

}

#endif
