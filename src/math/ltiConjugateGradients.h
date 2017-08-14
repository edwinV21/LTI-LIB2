/*
 * Copyright (C) 2009
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
 * \file   ltiConjugateGradients.h
 *         Contains the class lti::conjugateGradients, 
 *         which is used to find the minimum of an objective function.
 * 
 * \author Pablo Alvarado
 * \date   19.05.2009
 *
 * revisions ..: $Id: ltiConjugateGradients.h,v 1.5 2013-03-23 19:57:31 alvarado Exp $
 */

#ifndef _LTI_CONJUGATE_GRADIENTS_H_
#define _LTI_CONJUGATE_GRADIENTS_H_

#include "ltiFunctor.h"
#include "ltiVector.h"
#include "ltiLineSearch.h"

#undef _LTI_DEBUG
//#define _LTI_DEBUG 4

#include "ltiDebug.h"

namespace lti {

  /**
   * Class conjugateGradients
   *
   * The conjugate gradients is an optimization method based on the idea that
   * the (local) minimum of a function can be achieved following a sequence of
   * line optimizations, such that newer directions never collide with the
   * optimizations made by previous directions.
   *
   * If the function to be optimized has a quadratic form, the minimum will be
   * reached in the worst cast after N steps, with N the number of dimensions
   * of the domain of the function.
   *
   * The algorithm employed is known as the
   * Fletcher-Reeves-Polak-Ribiere method, and can be found at Press
   * et. al. "Numerical Recipes: The art of scientific computing", 3rd
   * edition, Cambridge University Press, 2007 pp. 515
   *
   * The apply method is a template with two parameters:
   * - The type T is used for the elements of the vectors, what determines the
   *   precision of the whole algorithm
   * - The class C is a functor class which must support the following method:
   *   - T apply(const vector<T>& site) const
   *   - void gradient(const vector<T>& site,vector<T>& grad) const;
   *   The first one evaluates the objective function at the given site, the
   *   second one computes the gradient at that site.
   *
   * \b Example Conjugate gradient minimization
   *
   * For the following example assumes you have declared a class named \c error,
   * for instance:
   *
   * \code
   *   template<typename T>
   *   class error {
   *   public:
   *     error() {}
   *     // example apply that returns the sum of the squares of all coordinates
   *     T apply(const vector<T>& vct) const {
   *       vector<T> tmp;
   *       tmp.apply(vct,sqr);
   *       return tmp.computeSumOfElements();
   *
   *     }
   *
   *     // since apply() just sums the parameters squared, the gradient is
   *     // twice the input vector
   *     void gradient(const vector<T>& site,vector<T>& grad) const {
   *       grad.copy(site);
   *       grad.multiply(T(2));
   *     }
   *   };
   * \endcode
   *
   * To find the minimum along a line passing through (0,1) with direction 
   * (1,1)
   * 
   * \code
   * // from (0,1)
   * fvector org(2,0.0f);
   * org.at(1)=1.0f;
   *
   * // direction (1,1)
   * fvector dir(2,1.0f);
   *
   * fvector npos;
   * float val;
   * error<float> parab;
   * conjugateGradients cg; // minimization functor
   *
   * // find the minimum
   * cg.apply(parab,org,npos,val);
   *
   * // print results
   * std::cout << "\n\nMinimum at " << npos << " = " << val << std::endl;
   * 
   * \endcode
   *
   * @see conjugateGradients::parameters.
   *
   * @ingroup gOptimization
   */
  class conjugateGradients : public functor {
  public:
    /**
     * The parameters for the class conjugateGradients
     */
    class parameters : public functor::parameters {
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
       * Line search optimization.
       *
       * Parameters for the line search optimization.
       *
       * Default value: default values
       */
      lineSearch::parameters lineSearchParameters;

      /**
       * Maximum number of iterations allowed
       *
       * Default value: 100
       */
      int maxIterations;

      /**
       * Convergence Tolerance.
       *
       * If the optimization process fails to improve the function
       * value by more than this value, then the optimization process
       * is stopped.
       *
       * Default value: sqrt(std::numeric_limits<float>::epsilon())
       */
      double tolerance;

    };

    /**
     * Default constructor
     */
    conjugateGradients();

    /**
     * Construct a functor using the given parameters
     */
    conjugateGradients(const parameters& par);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    conjugateGradients(const conjugateGradients& other);

    /**
     * Destructor
     */
    virtual ~conjugateGradients();

    /**
     * Operates on a copy of the given arguments.
     *
     * This is a template method with two parameters:
     * - The type T is used for the elements of the vectors, what determines the
     *   precision of the whole algorithm
     * - The class C is a functor class which must support the following method:
     *   T apply(const vector<T>& site) const
     *   which evaluates the objective function at the given site.
     *
     * @param func reference to a functor of type C that computes the objective
     *             function.
     * @param origin starting point for the search of the minimum.
     * @param newPosition resulting point where the function minimum was found
     * @param val value of the function at minimum
     *
     * @return true if apply successful or false otherwise.
     */
    template<typename T,class C>
    bool apply(const C& func,
               const vector<T>& origin,
               vector<T>& newPosition,
               T& val) const;

    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    conjugateGradients& copy(const conjugateGradients& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    conjugateGradients& operator=(const conjugateGradients& other);

    /**
     * Returns the complete name of the functor class
     */
    virtual const std::string& name() const;

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual conjugateGradients* clone() const;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual conjugateGradients* newInstance() const;

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;

  };
}

/*
 * The implementation of template methods has to be available to 
 * everyone, since the objective functor can be anything.
 */
#include "ltiConjugateGradients_template.h"

#endif

