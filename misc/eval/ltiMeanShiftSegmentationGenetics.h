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
 * \file   ltiMeanShiftSegmentationGenetics.h
 *         Defines the genetics class for the mean shift segmentation.
 * \author Pablo Alvarado
 * \date   21.05.08
 *
 * $Id: ltiMeanShiftSegmentationGenetics.h,v 1.1 2008-09-23 22:33:48 alvarado Exp $
 */

#ifndef _LTI_MEAN_SHIFT_SEGMENTATION_GENETICS_H_
#define _LTI_MEAN_SHIFT_SEGMENTATION_GENETICS_H_

#include "ltiSegmentationGenetics.h"
#include "ltiMeanShiftSegmentation.h"

#include <vector>

namespace lti {
  /**
   * Mean Shift Segmentation Genetics.
   *
   * Class for genetic evaluation of the mean shift segmentation algorithm.
   *
   * @see lti::meanShiftSegmentation
   *
   */
  class meanShiftSegmentationGenetics : public segmentationGenetics {
  public:
    /**
     * Type used to represent chromosomes
     */
    typedef segmentationGenetics::chromosome chromosome;

    /**
     * The parameters for the class meanShiftSegmentationGenetics.
     *
     * These are the specific parameters of the evaluation concepts,
     * where filenames, which fitness scalars compose the aggregate
     * fitness, and so on.
     */
    class parameters : public segmentationGenetics::parameters {
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
       * Minimum values for the parameterization of the algorithm
       */
      meanShiftSegmentation::parameters minValues;

      /**
       * Maximum values for the parameterization of the algorithm
       */
      meanShiftSegmentation::parameters maxValues;
    };

    /**
     * Default constructor.
     */
    meanShiftSegmentationGenetics();

    /**
     * Default constructor.
     */
    meanShiftSegmentationGenetics(const meanShiftSegmentationGenetics& other);

    /**
     * Default constructor with parameters.
     */
    meanShiftSegmentationGenetics(const parameters& p);

    /**
     * Destructor
     */
    virtual ~meanShiftSegmentationGenetics();

    /**
     * Return the fully qualified name of this class.
     */
    virtual const std::string& name() const;

    /**
     * Clone method.
     */
    virtual meanShiftSegmentationGenetics* clone() const;

    /**
     * New instance method.
     */
    virtual meanShiftSegmentationGenetics* newInstance() const;

    /**
     * Copy method
     */
    meanShiftSegmentationGenetics& copy(const meanShiftSegmentationGenetics& other);

    /**
     * Alias of copy
     */
    meanShiftSegmentationGenetics& 
    operator=(const meanShiftSegmentationGenetics& other);

    /**
     * Returns parameters in use.
     */
    const parameters& getParameters() const;

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
    virtual segmentation::parameters* 
    chromosomeToPhenotype(const chromosome& genotype) const;

    /**
     * Convert a valid parameters object (phenotype) into binary-chain
     * representation of a chromosome.
     */
    virtual bool phenotypeToChromosome(const functor::parameters& phenotype,
                                             chromosome& genotype) const;

    /**
     * Return the length in bits for a chromosome.
     *
     * This method needs to be reimplemented, in order to get some 
     * default implementations to work.
     */
    virtual int getChromosomeSize() const;

  private:
    /**
     * Private structure to hold the number of bits used for each parameter
     */
    struct bits;

  };
    
}

#endif
