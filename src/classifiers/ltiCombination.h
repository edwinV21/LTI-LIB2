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
 * \file ltiCombination.h 
 * Contains the class lti::combination, which
 * allows to combine classification results from lti::classifier instances.
 * \author Pablo Alvarado
 * \date 05.10.2008
 *
 * revisions ..: $Id: ltiCombination.h,v 1.1 2008-10-07 00:11:28 alvarado Exp $
 */

#ifndef _LTI_COMBINATION_H_
#define _LTI_COMBINATION_H_

#include <list>

#include "ltiClassifier.h"
#include "ltiFunctor.h"

namespace lti {
  /**
   * This class is used to combine several results of different
   * classifier::result instances.
   *
   * There are many possibilities to do this.  All of them will use
   * internaly the classifier::sortedOutput type to compute the
   * probability for each class.  You can just combine linearly the
   * results or use the probabilistic Borda count to consider also how
   * sure is a classifier of its result.
   *
   * Probabilistic Borda Count is documented in S. Syberichs.
   * "Entscheidungs-Fusion zur 3D-Objekterkennung in einem
   * hierarchischen Klassifikationssystem", Diplomarbeit, Lehrstuhl
   * für Technische Informatik, RWTH-Aachen, Juni 2001
   */
  class combination : public functor {
  public:

    /**
     * Possible types of combination strategies.
     *
     * The ProbabilisticBordaCount is described in the diploma thesis of
     * Stefan Syberichs at the LTI, RWTH Aachen: "Decision Fusion for 3D
     * Object Recognition in a Hierarchical Classification System".
     */
    enum eStrategy {
      Linear,                 /**< Linear combination */
      ProbabilisticBordaCount /**< Probabilistic Borda Count */
    };

    /**
     * The parameters for the class combination
     */
    class parameters : public functor::parameters {
    public:

      /**
       * Default constructor
       */
      parameters();

      /**
       * Copy constructor
       *
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
      const std::string& name() const;

      /**
       * Copy the contents of a parameters object
       *
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& copy(const parameters& other);

      /**
       * Copy the contents of a parameters object
       *
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& operator=(const parameters& other);


      /**
       * Returns a pointer to a clone of the parameters
       */
      virtual parameters* clone() const;

      /**
       * Returns a pointer to a newInstsance of the parameters
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
       * Strategy used for combination. Default Linear.
       * @see eStrategy
       */
      eStrategy strategy;

      /**
       * Normalize the result of the combination
       * Default: true
       */
      bool normalize;

      /**
       * Normalize the classifier::result instsances to be combined
       * before combination.  Default false. Usually, the inputs are
       * normalized anyway.
       */
      bool normalizeInputs;

      /**
       * The inputs can be weighted. If this value is true, the corresponding
       * %vector weights must be set as well. Default false.
       */
      bool useWeights;

      /**
       * The weights in case of weighted combination.
       *
       * For each element int the classifier::result list in the apply method,
       * the corresponding element in the weights vector is taken.  This means,
       * This vector should at least as long as the list provided in the
       * apply methods.
       *
       * Default empty.
       */
      dvector weights;
    };

    /**
     * Default constructor
     */
    combination();

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    combination(const combination& other);

    /**
     * Constructor
     * @param params the parameters object to be used.
     */
    combination(const parameters& params);

    /**
     * Destructor
     */
    virtual ~combination();

    /**
     * Returns the name of this type ("combination")
     */
    virtual const std::string& name() const;

    /**
     * Combine the given list of classifier::result objects into one
     * classifier::result instance according to the parameter settings.
     *
     * @param src list of classifier::outputvector objects with the partial
     *            classification results.
     *
     * @param dest classifier::outputvector where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const std::list<classifier::result>& src,
               classifier::result& dest) const;

    /**
     * Combine the given list of classifier::result objects into one
     * classifier::result object according to the parameter settings. No matter
     * what the parameters are, the given weights will be used for
     * combination.
     *
     * @param src     list of classifier::outputvector objects with the partial
     *                classification results.
     * @param weights used for weighting the inputs in same order.
     * @param dest    classifier::outputvector where the result will be left.
     * @return        true if apply successful or false otherwise.
     */
    bool apply(const std::list<classifier::result>& src,
               const dvector& weights,
               classifier::result& dest) const;

    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    combination& copy(const combination& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    combination& operator=(const combination& other);

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual combination* clone() const;

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual combination* newInstance() const;

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;

  protected:

    /**
     * Calculates the sum of the classifier::results contained in src and
     * leaves the result in dest. Returns false if the %vectors in src
     * are not compatible, ie contain the same labels.
     * @param src list of classifier::results to be summed up
     * @param dest result of the sum
     * @return false if values in list are incompatible
     */
    bool sumOfVectors(const std::list<classifier::result> src,
                      classifier::result& dest) const;

    /**
     * Calculates the weighted sum of the classifier::results
     * contained in src and leaves the result in dest. Returns false
     * if the %vectors in src are not compatible, ie contain the same
     * labels.
     *
     * @param src list of classifier::results to be summed up
     * @param weights used for weighting the classifier::results in src
     * @param dest result of the sum
     * @return false if values in list are incompatible
     */
    bool sumOfVectors(const std::list<classifier::result> src,
                      const dvector& weights,
                      classifier::result& dest) const;

    /**
     * First calculates the ProbablilisticBordaCount for each input
     * %vector then sums these measures and returns the sum in
     * dest. Returns false if the %vectors in src are not compatible,
     * ie contain the same labels.
     * @param src list of classifier::results to be summed up
     * @param dest result of the sum
     * @return false if values in list are incompatible
     */
    bool probBordaCount(const std::list<classifier::result> src,
                        classifier::result& dest) const;

    /**
     * First calculates the ProbablilisticBordaCount for each input
     * %vector then sums these measures weighted by weights and returns the
     * sum in dest. Returns false if the %vectors in src are not compatible,
     * ie contain the same labels.
     * @param src list of classifier::results to be summed up
     * @param weights used for weighting the classifier::results in src
     * @param dest result of the sum
     * @return false if values in list are incompatible
     */
    bool probBordaCount(const std::list<classifier::result> src,
                        const dvector& weights,
                        classifier::result& dest) const;


    /**
     * Checks whether all classifier::results contained in src are
     * compatible to each other and that none of them contains the
     * same label more than once. Returns true if everything is ok.
     */
    bool
    noMultAndCompatible(const std::list<classifier::result> src) const;

  };

  /**
   * Write combination::eStrategy
   */
  bool write(ioHandler& handler,const combination::eStrategy val);

  /**
   * Read combination::eStrategy
   */
  bool read(ioHandler& handler,combination::eStrategy& val);


}

#endif
