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
 * \file   ltiContrastEnhancementGenetics.h
 *         Defines the abstract parent class of the genetics for contrastEnhancement 
 *         algorithm.
 * \author Miguel Angel Aguilar
 * \date   16.11.07
 *
 * $Id: ltiContrastEnhancementGenetics.h,v 1.2 2012-07-01 00:36:33 alvarado Exp $
 */

#ifndef _LTI_CONTRAST_ENHANCEMENT_GENETICS_H_
#define _LTI_CONTRAST_ENHANCEMENT_GENETICS_H_

#include "ltiGenetics.h"
#include "ltiContrastEnhancement.h"
#include "ltiChannel.h"
#include "ltiSecondOrderStatistics.h"
#include <vector>

namespace lti {
  /**
   * Base class for genetic classes that evaluate contrast enhancement algorithms.
   *
   * All algorithms will be tested with lti::channel (float valued pixels).
   *
   * \warning The contrast enhancement algorithms will be verified using a set of
   * images.  Since it would be extremely expensive to load each time
   * the set of images, then they will be internally cached.  Please
   * consider this when deciding how big your image set should be.
   */
  class contrastEnhancementGenetics : public genetics {
  public:
    /**
     * Type used to represent chromosomes
     */
    typedef genetics::chromosome chromosome;

    /**
     * The parameters for the class contrastEnhancementGenetics.
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
       * - ACE: Average Contrast Enhancement. This is a measure of of contrast 
       *        improvement.
       * - Entropy: This is a measure of the amount of information in the image.
       *            More contrast, more entropy
       * - Speed: how fast is the algorithm (images per second)
       */
      //@{
      /** 
       * Average Contrast Enhancement.
       *
       * This is a measure of the amount of improvement of contrast 
       * between the low contrast image and its enhanced image.
       *
       * The fitness value computed if this attribute is set to \c true is
       * the improvement of the ACE.
       *
       * Default value: true
       */
      bool ace;

      /** 
       * Entropy.
       *
       * Images with high contrast have a large entropy than the images 
       * with low contrast. This is goodness measure.
       *
       * Default value: true
       */
      bool entropy;

      /** 
       * Speed.
       *
       * This measures how many images per second (of the test set) can be computed.
       *
       * Default value: false
       */
      bool speed;

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
    };

    /**
     * Default constructor.
     */
    contrastEnhancementGenetics();

    /**
     * Default constructor.
     */#include "ltiSecondOrderStatistics.h"
    contrastEnhancementGenetics(const contrastEnhancementGenetics& other);

    /**
     * Destructor
     */
    virtual ~contrastEnhancementGenetics();

    /**
     * Return the fully qualified name of this class.
     */
    virtual const std::string& name() const;

    /**
     * Clone method.
     */
    virtual contrastEnhancementGenetics* clone() const = 0;

    /**
     * New instance method.
     */
    virtual contrastEnhancementGenetics* newInstance() const = 0;

    /**
     * Copy method
     */
    contrastEnhancementGenetics& copy(const contrastEnhancementGenetics& other);

    /**
     * Alias of copy
     */
    contrastEnhancementGenetics& operator=(const contrastEnhancementGenetics& other);

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
    virtual contrastEnhancement::parameters* 
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
     * Attach the instance of the contrastEnhancer to be used.
     *
     * This class will take care of the memory management
     */
    bool setContrastEnhancer(const contrastEnhancement& instance);

    /**getDenoiser
     * Return a read-only reference to the internal denoiser
     *
     * Before calling this method you should verify with canDenoise() if
     * a valid instance has been set yet.
     */
    const contrastEnhancement& getContrastEnhancer() const;

    /**
     * Check if a valid contrastEnhancement instance has already been set.
     */
    bool canEnhance() const;

  protected:
    /**
     * Instance of the proper contrastEnhancer class used
     */
    contrastEnhancement* contrastEnhancerInstance_;

    /**
     * @name Methods to compute scala fitness
     */
    //@{

    /**
     * This function computes the ACE  (Average Contrast Enhancement)
     *
     * @param lowContrast original image that supose have a low contrast.
     * @param highContrast enhanced version of original image that 
                           supose have a high contrast.
     */
    double ace(const channel& lowContrast,
               const channel& highContrast) const;
    
    /**
     * This function computes the Entropy 
     *
     * @param src image that will compute its entropy
     */
    double entropy(const channel& src) const;

    //@}

    /**
     * Original images
     */
    std::vector<channel> origs_;
    
    /**
     * Dimension of the fitness space
     */
    int dimFitness_;

  };

}

#endif
