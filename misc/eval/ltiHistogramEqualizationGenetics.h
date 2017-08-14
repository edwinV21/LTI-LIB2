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

/**
 * \file   ltiHistogramEqualizationGenetics.h
 *         Defines the abstract parent class of the genetics for contrast \
 *         enhancement algorithm.
 * \author Pablo Alvarado
 * \date   19.08.06
 *
 * $Id: ltiHistogramEqualizationGenetics.h,v 1.1 2008-06-02 02:45:49 alvarado Exp $
 */

#ifndef _LTI_HISTOGRAM_EQUALIZATION_GENETICS_H_
#define _LTI_HISTOGRAM_EQUALIZATION_GENETICS_H_

#include "ltiContrastEnhancementGenetics.h"
#include "ltiHistogramEqualization.h"
#include "ltiInterval.h"

namespace lti {

  /**
   * Evaluate histogram equalization.
   */
  class histogramEqualizationGenetics : public contrastEnhancementGenetics {
  public:
    /**
     * Type used to represent chromosomes
     */
    typedef contrastEnhancementGenetics::chromosome chromosome;

    /**
     * The parameters for the class contrastEnhancementGenetics.
     *
     * These are the specific parameters of the evaluation concepts,
     * where filenames, which fitness scalars compose the aggregate
     * fitness, and so on.
     */
    class parameters : public contrastEnhancementGenetics::parameters {
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
       * Interval for values of lower input limit.
       *
       * The value range of lower input limit will lie in this closed
       * interval.
       *
       * Default value: [0,1]
       */
      dinterval lowerInputLimitInterval;

      /**
       * Interval for values of upper input limit.
       *
       * The value range of upper input limit will lie in this closed
       * interval.
       *
       * Default value: [0,1]
       */
      dinterval upperInputLimitInterval;

      /**
       * Interval for values of lower output limit.
       *
       * The value range of lower output limit will lie in this closed
       * interval.
       *
       * Default value: [0,1]
       */
      dinterval lowerOutputLimitInterval;

      /**
       * Interval for values of upper output limit.
       *
       * The value range of lower output limit will lie in this closed
       * interval.
       *
       * Default value: [0,1]
       */
      dinterval upperOutputLimitInterval;

    };


    /**
     * Default constructor.
     */
    histogramEqualizationGenetics();

    /**
     * Default constructor.
     */
    histogramEqualizationGenetics(const histogramEqualizationGenetics::parameters& p);

    /**
     * Copy constructor.
     */
    histogramEqualizationGenetics(const histogramEqualizationGenetics& other);


    /**
     * Destructor
     */
    virtual ~histogramEqualizationGenetics();

    /**
     * Return the fully qualified name of this class.
     */
    virtual const std::string& name() const;

    /**
     * Clone method.
     */
    virtual histogramEqualizationGenetics* clone() const;

    /**
     * New instance method.
     */
    virtual histogramEqualizationGenetics* newInstance() const;

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
    virtual histogramEqualization::parameters* 
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
      BitsForLowerInputLimit = 16,
      BitsForLowerOutputLimit = 16,
      BitsForUpperInputLimit = 16,
      BitsForUpperOutputLimit = 16,
      BitsForCells = 9
    };
  };

}

#endif
