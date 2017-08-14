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
 * \file   ltiLineSearch.h
 *         Contains the class lti::lineSearch, 
 *         which is used to find the minimum of an objective function along
 *         a given direction.
 * 
 * \author Pablo Alvarado
 * \date   19.05.2009
 *
 * revisions ..: $Id: ltiLineSearch.h,v 1.3 2012-01-03 02:22:25 alvarado Exp $
 */

#ifndef _LTI_LINE_SEARCH_H_
#define _LTI_LINE_SEARCH_H_

#include "ltiFunctor.h"
#include "ltiVector.h"

#undef _LTI_DEBUG
//#define _LTI_DEBUG 4

#include "ltiDebug.h"

namespace lti {

  /**
   * Class lineSearch
   *
   * The line search is a method commonly used in optimization methods to
   * find the minimum of an objective function.
   *
   * The algorithm employed uses parabolic interpolation together with 
   * Brent's method, as suggested in Press et. al. "Numerical Recipes: The art 
   * of scientific computing", 3rd edition, Cambridge University Press, 2007
   *
   * The apply method is a template with two parameters:
   * - The type T is used for the elements of the vectors, what determines the
   *   precision of the whole algorithm
   * - The class C is a functor class which must support the following method:
   *   T apply(const vector<T>& site) const
   *   which evaluates the objective function at the given site.
   *
   * \b Example Line minimization
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
   * lineSearch ls; // minimization functor
   *
   * // find the minimum
   * ls.apply(parab,org,dir,npos,val);
   *
   * // print results
   * std::cout << "\n\nMinimum at " << npos << " = " << val << std::endl;
   * 
   * \endcode
   *
   * @see lineSearch::parameters.
   *
   * @ingroup gOptimization
   */
  class lineSearch : public functor {
  public:
    /**
     * The parameters for the class lineSearch
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
       * Precision.
       *
       * When the function value does not decrese more than this value
       * then it will be assumed that the minimum has been found.
       * 
       * This value must be higher or equal than
       * sqrt(std::numeric_limits<T>::epsilon()), where T is the type
       * finally used in the apply method for each coordinate of the vector.
       *
       * Default value: sqrt(std::numeric_limits<float>::epsilon())
       */
      double epsilon;

      /**
       * Maximum number of iterations allowed
       *
       * Default value: 100
       */
      int maxIterations;

      /**
       * Maximum allowed parabolic-fit step magnification.
       *
       * This parameter is used in the bracket method.
       *
       * Default value: 100.0
       */
      double maxBracketStepMagnification;
    };

    /**
     * Default constructor
     */
    lineSearch();

    /**
     * Construct a functor using the given parameters
     */
    lineSearch(const parameters& par);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    lineSearch(const lineSearch& other);

    /**
     * Destructor
     */
    virtual ~lineSearch();

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
     * @param direction along which the minimum will be searched
     * @param newPosition resulting point where the function minimum was found
     * @param val value of the function at minimum
     *
     * @return true if apply successful or false otherwise.
     */
    template<typename T,class C>
    bool apply(const C& func,
               const vector<T>& origin,
               const vector<T>& direction,
               vector<T>& newPosition,
               T& val) const;

    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    lineSearch& copy(const lineSearch& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    lineSearch& operator=(const lineSearch& other);

    /**
     * Returns the complete name of the functor class
     */
    virtual const std::string& name() const;

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual lineSearch* clone() const;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual lineSearch* newInstance() const;

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;

  private:

    /**
     * Adapter class
     *
     * This class provides an interface of functor to the multi-dimensional
     * function given at construction time.
     *
     */
    template<typename T,class C>
    class adapter {
    public:
      /**
       * Constructor with the functor to be adapted
       * 
       * @param functorInstance instance of the functor class to
       *        be adapted.
       */
      adapter(const C& functorInterface,
              const vector<T>& from,
              const vector<T>& direction);

      /**
       * Evaluates the functor instance given at construction time
       * at the point given by from+val*direction;
       */
      inline T operator()(const T val);

      /**
       * Last evaluated position
       *
       * This attribute saves creating a vector each time
       */
      vector<T> lastPos;

    protected:
      /**
       * Reference to the used functor class
       */
      const C& functor_;

      /**
       * Reference to the starting point
       */
      const vector<T>& from_;

      /**
       * Reference to the direction vector
       */
      const vector<T>& direction_;

    };
    
    /**
     * Helper class
     * 
     * This is the worker class that does everything.
     * 
     * It has a state that hold the results of each iteration
     */
    template<typename T>
    class helper {
    public:
      /**
       * Only available constructor
       */
      helper(const parameters& params);

      /**
       * Do a bracket search starting from the given \e a and \e b arguments.
       * The position of the last points evaluated will be left in the
       * protected attributes of this class ax_, bx_ and cx_, and the
       * function values evaluated a those points
       * 
       * The class type C denotes here a one-dimensional function.
       *
       * The method will usually employ an adapter to interpret the
       * multi-dimensional function as a one-dimensional one.
       */
      template<class C1>
      void bracket(const T a, 
                   const T b,
                   C1& objFunction); 

      
      /**
       * Find the minimum of the one-dimensional adapted function
       */
      template<class C1>
      T brent(C1& objFunction); 

      
      /**
       * Minimize
       *
       * Do the minimization along the given direction
       */
      template<class C>
      T minimize(const vector<T>& origin,
                 const vector<T>& direction,
                 vector<T>& newPos,
                 const C& mvFunctor);
      
    protected:
      /**
       * Maximum number of iterations 
       */
      int maxIterations_;

      /**
       * Tolerance
       */
      T tolerance_;

      /**
       * Maximum allowed magnification for parabolic-fit step in bracket stage
       */
      T glimit_;

      /**
       * Bracket positions
       */
      //@{
      T ax_,bx_,cx_;
      //@}

      /**
       * Function values at bracket positions
       */
      //@{
      T fa_,fb_,fc_;
      //@}

      /**
       * Shadow of the last minimum value
       */
      T fmin_;

      /**
       * Shadow of the last minimum position
       */
      T xmin_;

      /**
       * Shift the contents of the variables from the last to the first
       */
      inline void shift(T& a,T& b,T& c,const T d);

      /**
       * Sign
       *
       * Transfer the sign to the magnitude
       */
      inline T sign(const T mag,const T s);
    };
  };
}

/*
 * The implementation of template methods has to be available to 
 * everyone, since the objective functor can be anything.
 */
#include "ltiLineSearch_template.h"

#endif

