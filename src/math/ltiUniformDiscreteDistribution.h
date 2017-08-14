/*
 * Copyright (C) 2007
 * Pablo Alvarado
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
 * \file   ltiUniformDiscreteDistribution.h
 *         Contains the class lti::uniformDiscreteDistribution to
 *         produce random integers uniformly distributed in an specified 
 *         interval
 * \author Pablo Alvarado
 * \date   25.09.2007
 *
 * revisions ..: $Id: ltiUniformDiscreteDistribution.h,v 1.3 2009-04-19 04:06:10 alvarado Exp $
 */

#ifndef _LTI_UNIFORM_DISCRETE_DISTRIBUTION_H_
#define _LTI_UNIFORM_DISCRETE_DISTRIBUTION_H_

#include "ltiUnivariateDiscreteDistribution.h"

namespace lti {

  /**
   * Class uniformDiscreteDistribution
   *
   * This class generates (pseudo) random numbers uniformly distributed in an
   * interval specified in the parameters.
   *
   * In the sake of speed, the classical modulo operation is used to
   * avoid multiplications and divisions.  The error introduced is
   * however in most applications totally negligible.
   *
   * @see uniformDiscreteDistribution::parameters.
   *
   * @ingroup gRandomDiscrete
   */
  class uniformDiscreteDistribution : public univariateDiscreteDistribution {
  public:
    /**
     * The parameters for the class uniformDiscreteDistribution
     */
    class parameters : public univariateDiscreteDistribution::parameters {
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
       * Lower bound of the value interval.
       *
       * This is the smallest value that can be generated.
       *
       * Default value: 0
       */
      int min;

      /**
       * Higher bound of the value interval.
       *
       * This is the highest value that can be generated.
       *
       * Default value: 100
       */
      int max;

    };

    /**
     * Default constructor
     */
    uniformDiscreteDistribution();

    /**
     * Constructor with a given interval.
     *
     * @param min lower interval bound.
     * @param max higher interval bound.
     */
    uniformDiscreteDistribution(const int min,const int max);

    /**
     * Construct a functor using the given parameters
     */
    uniformDiscreteDistribution(const parameters& par);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    uniformDiscreteDistribution(const uniformDiscreteDistribution& other);

    /**
     * Destructor
     */
    virtual ~uniformDiscreteDistribution();

    /**
     * Get a random number.
     *
     * Returns a random number distributed accordingly to the type of the
     * current instance.  
     *
     * The uniformDiscreteDistribution can be used to obtain numbers
     * in the interval [min(),max()].
     *
     * @param rnd double reference where the random number has to be left.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(int& rnd);

    /**
     * Virtual method to get a integer random number.
     *
     * Returns a random number distributed accordingly to the type of the
     * current instance.  
     *
     * The uniformDiscreteDistribution can be used to obtain numbers
     * in the interval [min(),max()].
     *
     * @return a random float number.
     */
    virtual int draw();

    /**
     * Non-virtual method to get a integer random number.
     *
     * Returns a random number distributed accordingly to the type of the
     * current instance.  
     *
     * The uniformDiscreteDistribution can be used to obtain numbers
     * in the interval [min(),max()].
     *
     * @return a random float number.
     */
    int rand();

    /**
     * Virtual method to obtain the maximum possible number
     * (inclusive) to be returned by this distribution
     */
    virtual int max() const;

    /**
     * Virtual method to obtain the minimum possible number
     * (inclusive) to be returned by this distribution.
     */
    virtual int min() const;

    /**
     * Change the interval, modifying the parameters as well
     */
    bool setInterval(const int lower,const int upper);

    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    uniformDiscreteDistribution& 
    copy(const uniformDiscreteDistribution& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    uniformDiscreteDistribution& 
    operator=(const uniformDiscreteDistribution& other);

    /**
     * Returns the complete name of the functor class
     */
    virtual const std::string& name() const;

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual uniformDiscreteDistribution* clone() const;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual uniformDiscreteDistribution* newInstance() const;

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
     * Returns used parameters
     */
    parameters& getRWParameters();

    /**
     * Shadow of the parameters.min;
     */ 
    int minimum_;

    /**
     * Shadow of the parameters.max;
     */ 
    int maximum_;

    /**
     * delta_ = (maximum_ - minimum_ + 1)
     */
    int delta_;
  };
}

#endif

