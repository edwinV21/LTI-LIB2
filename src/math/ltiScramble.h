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
 * \file   ltiScramble.h
 *         Contains the class lti::scramble wich is used to randomly
 *         redistribute the elements in a matrix or vector.
 * \author Pablo Alvarado
 * \date   09.10.2007
 *
 * revisions ..: $Id: ltiScramble.h,v 1.3 2010-04-22 04:58:26 alvarado Exp $
 */

#ifndef _LTI_SCRAMBLE_H_
#define _LTI_SCRAMBLE_H_

#include "ltiFunctor.h"
#include "ltiMatrix.h"
#include "ltiUniformDiscreteDistribution.h"

namespace lti {

  /**
   * Scramble vectors and matrices.
   *
   * This class is used to scramble the elements of a given vector or
   * matrix. The actual implementation chooses two random indices of
   * the vector or matrix and swaps their contents.  This operation is
   * performed N/2 times, with N the number of elements in the container.
   *
   * A random generator is created as attribute and modified each time the
   * apply is called with a new vector or matrix size.  Therefore the apply
   * methods are not constant, to avoid misunderstandings in multithreaded
   * environments.
   *
   * Example
   * \code
   * // We want the system wide random generator, and not the pseudo-random
   * // used as default.
   * lti::scramble::parameters sPar;
   * sPar.randomParams.generator = lti::randomDistribution::SystemWide;
   * 
   * // Create the scrambler
   * lti::scramble scrambler(sPar);
   *
   * // Data block
   * const int data[] = {0,1,2,3,4,5,6,7,8,9}
   * lti::ivector v(10,data); // initialize the vector
   * 
   * while(true) {
   *   scrambler.apply(v); // produce a permutation of the data
   *   std::cout << v << std::endl; // and print it out
   * }
   *
   * \endcode
   *
   * @see scramble::parameters.
   *
   * @ingroup gSorting
   */
  class scramble : public functor {
  public:
    /**
     * The parameters for the class scramble
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
       * Parameters of the random number generator used to select the
       * elements.  The min and max values are ignored, since they are
       * adapted in each apply to the correct range of indices for the
       * container being scrambled.
       */
      uniformDiscreteDistribution::parameters randomParams;

    };

    /**
     * Default constructor
     */
    scramble();

    /**
     * Construct a functor using the given parameters
     */
    scramble(const parameters& par);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    scramble(const scramble& other);

    /**
     * Destructor
     */
    virtual ~scramble();

    /**
     * Operates on the given argument.
     *
     * @param srcdest matrix<T> with the source data. The result
     *                will be left here too.
     * @return true if apply successful or false otherwise.
     */
    template<typename T>
    bool apply(matrix<T>& srcdest);

    /**
     * Operates on the given argument.
     *
     * @param srcdest vector<T> with the source data. The result
     *                will be left here too.
     * @return true if apply successful or false otherwise.
     */
    template<typename T>
    bool apply(vector<T>& srcdest);

    /**
     * Operates on a copy of the given arguments.
     *
     * @param src matrix<T> with the source data.
     * @param dest matrix<T> where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    template<typename T>
    bool apply(const matrix<T>& src, matrix<T>& dest);

    /**
     * Operates on a copy of the given arguments.
     *
     * @param src vector<T> with the source data.
     * @param dest vector<T> where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    template<typename T>
    bool apply(const vector<T>& src, vector<T>& dest);


    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    scramble& copy(const scramble& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    scramble& operator=(const scramble& other);

    /**
     * Returns the complete name of the functor class
     */
    virtual const std::string& name() const;

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual scramble* clone() const;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual scramble* newInstance() const;

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * Update parameters
     */
    bool updateParameters();

  protected:
    
    /**
     * The random generator is a mutable attribute that provides the
     * random number.  It has to be an attribute to allow generating
     * different random numbers each time tha apply is called.
     */
    uniformDiscreteDistribution rnd_;

    /**
     * Shadow of the last used size
     */
    int max_;
  };  
}

#include "ltiScramble_template.h"

#endif

