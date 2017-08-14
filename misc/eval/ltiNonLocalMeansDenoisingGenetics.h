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
 * \file   ltiNonLocalMeansDenoisingGenetics.h
 *         Evaluate Non-Local Means Denoising
 * \author Miguel Angel Aguilar Ulloa
 * \date   21.10.07
 *
 * $Id: ltiNonLocalMeansDenoisingGenetics.h,v 1.1 2008-06-02 02:45:50 alvarado Exp $
 */

#ifndef _LTI_NON_LOCAL_MEANS_DENOISING_GENETICS_H_
#define _LTI_NON_LOCAL_MEANS_DENOISING_GENETICS_H_

#include "ltiDenoisingGenetics.h"
#include "ltiNonLocalMeansDenoising.h"
#include "ltiInterval.h"

namespace lti {

  /**
   * Evaluate Non-Local Means Denoising
   */
  class nonLocalMeansDenoisingGenetics : public denoisingGenetics {
  public:
    /**
     * Type used to represent chromosomes
     */
    typedef denoisingGenetics::chromosome chromosome;

    /**
     * The parameters for the class denoisingGenetics.
     *
     * These are the specific parameters of the evaluation concepts,
     * where filenames, which fitness scalars compose the aggregate
     * fitness, and so on.
     */
    class parameters : public denoisingGenetics::parameters {
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
       * Interval for values of variance of gaussian kernel.
       *
       * The value range of variance will lie in this closed
       * interval.
       *
       * Default value: [0,100]
       */
      dinterval varianceInterval;

      /**
       * Interval for values of filter degree.
       *
       * The value range of filter degree will lie in this closed
       * interval.
       *
       * Default value: [0,1]
       */
      dinterval filterDegreeInterval;
    };


    /**
     * Default constructor.
     */
    nonLocalMeansDenoisingGenetics();

    /**
     * Default constructor.
     */
    nonLocalMeansDenoisingGenetics(const nonLocalMeansDenoisingGenetics::parameters& p);

    /**
     * Copy constructor.
     */
    nonLocalMeansDenoisingGenetics(const nonLocalMeansDenoisingGenetics& other);


    /**
     * Destructor
     */
    virtual ~nonLocalMeansDenoisingGenetics();

    /**
     * Return the fully qualified name of this class.
     */
    virtual const std::string& name() const;

    /**
     * Clone method.
     */
    virtual nonLocalMeansDenoisingGenetics* clone() const;

    /**
     * New instance method.
     */
    virtual nonLocalMeansDenoisingGenetics* newInstance() const;

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
    virtual nonLocalMeansDenoising::parameters* 
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
      BitsForBoundaryType = 2,
      BitsForWindowSize = 2,
      BitsForSubregionSize = 3,
      BitsForVariance = 16,
      BitsForFilterDegree = 16
    };
  };

}

#endif
