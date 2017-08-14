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
 * \file   ltiDownhillSimplex.h
 *         Contains the class downhillSimplex,
 *         which is an optimization mathod that requires only function
 *         evaluations not relying wether on gradients nor in line search.
 * 
 * \author Pablo Alvarado
 * \date   19.05.2009
 *
 * revisions ..: $Id: ltiDownhillSimplex.h,v 1.4 2012-07-01 00:36:15 alvarado Exp $
 */

#ifndef _LTI_DOWNHILL_SIMPLEX_H_
#define _LTI_DOWNHILL_SIMPLEX_H_

#include "ltiFunctor.h"
#include "ltiVector.h"
#include "ltiMatrix.h"

#undef _LTI_DEBUG
//#define _LTI_DEBUG 4

#include "ltiDebug.h"

namespace lti {

  /**
   * Class downhillSimplex
   *
   * The downhill simplex method is an optimization strategy originally 
   * proposed by Nelder and Mead on 1965.  It relies only on objective
   * function evaluations, making it unnecessary to compute gradients or
   * performing line search.
   *
   * It is not very efficient, but it may be sufficient to compute minima
   * as part of hierarchical optimization strategies like Simulated Annealing
   * or Evolutionary Algorithms.
   *
   * The algorithm employed is known is partially inspired on Press
   * et. al. "Numerical Recipes: The art of scientific computing", 3rd edition,
   * Cambridge University Press, 2007 pp. 502
   *
   * The apply method is a template with two parameters:
   * - The type T is used for the elements of the vectors, what determines the
   *   precision of the whole algorithm
   * - The class C is a functor class which must support the following method:
   *   - T apply(const vector<T>& site) const
   *   which evaluates the objective function at the given site.
   *
   * \b Example Downhill simplex minimization
   *
   * The following example assumes you have declared a class named \c error,
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
   * fvector npos;
   * float val;
   * error<float> parab;
   * downhillSimplex cg; // minimization functor
   *
   * // find the minimum
   * cg.apply(parab,org,npos,val);
   *
   * // print results
   * std::cout << "\n\nMinimum at " << npos << " = " << val << std::endl;
   * 
   * \endcode
   *
   * @see downhillSimplex::parameters.
   *
   * @ingroup gOptimization
   */
  class downhillSimplex : public functor {
  public:
    /**
     * The parameters for the class downhillSimplex
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
       * Maximum number of iterations allowed
       *
       * Default value: 5000
       */
      int maxIterations;

      /**
       * Convergence Tolerance.
       *
       * If the optimization process fails to improve the function
       * value by more than this value, then the optimization process
       * is stopped.
       *
       * Default value: std::numeric_limits<double>::epsilon()
       */
      double tolerance;

    };

    /**
     * Default constructor
     */
    downhillSimplex();

    /**
     * Construct a functor using the given parameters
     */
    downhillSimplex(const parameters& par);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    downhillSimplex(const downhillSimplex& other);

    /**
     * Destructor
     */
    virtual ~downhillSimplex();

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
     * This functor just calls the next apply method using \c lambda = 1.
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
     * Operates on a copy of the given arguments.
     *
     * This is a template method with two parameters:
     * - The type T is used for the elements of the vectors, what determines the
     *   precision of the whole algorithm
     * - The class C is a functor class which must support the following method:
     *   T apply(const vector<T>& site) const
     *   which evaluates the objective function at the given site.
     *
     * The initial simplex has \f$N+1\f$ corners, the first one being 
     * \f$\mathbf{p}_0\f$ and the rest \f$N\f$ computed as
     * \f[
     *   \mathbf{p}_i = \mathbf{p}_0 + \lambda \mathbf{e}_i
     * \f]
     * where \f$\mathbf{e}_i\f$ are unit vectors and \f$\lambda\f$ is a factor
     * to multiply all unit vectors.
     *
     * @param func reference to a functor of type C that computes the objective
     *             function.
     * @param origin starting point for the search of the minimum.
     * @param lambda scaling factor for all unit vectors.
     * @param newPosition resulting point where the function minimum was found
     * @param val value of the function at minimum
     *
     * @return true if apply successful or false otherwise.
     */
    template<typename T,class C>
    bool apply(const C& func,
               const vector<T>& origin,
               const T lambda,
               vector<T>& newPosition,
               T& val) const;

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
     * The initial simplex has \f$N+1\f$ corners, the first one being 
     * \f$\mathbf{p}_0\f$ and the rest \f$N\f$ computed as
     * \f[
     *   \mathbf{p}_i = \mathbf{p}_0 + \lambda_i \mathbf{e}_i
     * \f]
     * where \f$\mathbf{e}_i\f$ are unit vectors and \f$\lambda\f$ is a factor
     * to multiply all unit vectors.
     *
     * @param func reference to a functor of type C that computes the objective
     *             function.
     * @param origin starting point for the search of the minimum.
     * @param lambda scaling factors for each of the unit vectors.  It has
     *               to be of dimension \f$N\f$.
     * @param newPosition resulting point where the function minimum was found
     * @param val value of the function at minimum
     *
     * @return true if apply successful or false otherwise.
     */
    template<typename T,class C>
    bool apply(const C& func,
               const vector<T>& origin,
               const vector<T>& lambda,
               vector<T>& newPosition,
               T& val) const;

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
     * The initial simplex has \f$N+1\f$ corners, where \f$N\f$ is
     * the dimension of the vector space.  It is given to the method through
     * the \a oSimplex matrix, with \f$N+1\f$ rows and \f$N\f$ columns, where
     * each row corresponds to one corner of the simplex.
     *
     * @param func reference to a functor of type C that computes the objective
     *             function.
     * @param oSimplex first simplex used in the optimization.
     * @param newPosition resulting point where the function minimum was found
     * @param val value of the function at minimum
     *
     * @return true if apply successful or false otherwise.
     */
    template<typename T,class C>
    bool apply(const C& func,
               const matrix<T>& oSimplex,
               vector<T>& newPosition,
               T& val) const;


    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    downhillSimplex& copy(const downhillSimplex& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    downhillSimplex& operator=(const downhillSimplex& other);

    /**
     * Returns the complete name of the functor class
     */
    virtual const std::string& name() const;

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual downhillSimplex* clone() const;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual downhillSimplex* newInstance() const;

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;

  protected:

    /**
     * Accumulate the rows of a matrix
     */
    template<typename T>
    void accumulateRows(const matrix<T> rows,
                        vector<T>& acc) const;

    /**
     * Helper function performs the "extrapolation" by a given factor through
     * the face of the simplex across the highest point
     *
     * @param p Matrix describing the simplex
     * @param y Vector containing the evaluated simplex corners
     * @param psum Sum of all corners
     * @param ihi Index of the worst candidate in the p simplex
     * @param factor extrapolation factor
     * @param func functor that evaluates the error
     */
    template<typename T,class C>
    T helper(matrix<T>& p,
             vector<T>& y,
             vector<T>& psum,
             const int ihi,
             const T factor,
             const C& func) const;

  private:
    /**
     * Function used just for debuggin purposes
     */
    template<typename T>
    void printSimplex(const matrix<T>& simplex,
                      const vector<T>& vals) const;
             

  };
}

/*
 * The implementation of template methods has to be available to 
 * everyone, since the objective functor can be anything.
 */
#include "ltiDownhillSimplex_template.h"

#endif

