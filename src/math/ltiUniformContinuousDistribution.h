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
 * \file   ltiUniformContinuousDistribution.h
 *         Contains the class lti::uniformContinuousDistribution to
 *         produce random integers uniformly distributed in an specified 
 *         interval
 * \author Pablo Alvarado
 * \date   25.09.2007
 *
 * revisions ..: $Id: ltiUniformContinuousDistribution.h,v 1.1 2007-09-26 21:21:52 alvarado Exp $
 */

#ifndef _LTI_UNIFORM_CONTINUOUS_DISTRIBUTION_H_
#define _LTI_UNIFORM_CONTINUOUS_DISTRIBUTION_H_

#include "ltiUnivariateContinuousDistribution.h"

namespace lti {

  /**
   * Class uniformContinuousDistribution
   *
   * This class generates (pseudo) random numbers uniformly distributed in an
   * interval specified in the parameters.
   *
   * @see uniformContinuousDistribution::parameters.
   *
   * @ingroup gRandomContinuous
   */
  class uniformContinuousDistribution :
    public univariateContinuousDistribution {

  public:
    /**
     * The parameters for the class uniformContinuousDistribution
     */
    class parameters : public univariateContinuousDistribution::parameters {
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
       * Lower bound of the value interval (inclusive).
       *
       * This is the smallest value that can be generated.
       *
       * Default value: 0.0
       */
      double min;

      /**
       * Higher bound of the value interval (exclusive).
       *
       * This is the highest value that can be generated.
       *
       * Default value: 1.0
       */
      double max;

    };

    /**
     * Default constructor
     */
    uniformContinuousDistribution();

    /**
     * Constructor with a given interval.
     *
     * @param min lower interval bound.
     * @param max higher interval bound.
     */
    uniformContinuousDistribution(const double min,const double max);

    /**
     * Construct a functor using the given parameters
     */
    uniformContinuousDistribution(const parameters& par);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    uniformContinuousDistribution(const uniformContinuousDistribution& other);

    /**
     * Destructor
     */
    virtual ~uniformContinuousDistribution();

    /**
     * Get a random number.
     *
     * Returns a random number distributed accordingly to the type of the
     * current instance.  
     *
     * @param rnd double reference where the random number has to be left.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(float& rnd);

    /**
     * Get a random number.
     *
     * Returns a random number distributed accordingly to the type of the
     * current instance.  
     *
     * @param rnd double reference where the random number has to be left.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(double& rnd);

    /**
     * Virtual method to get a single precision random number.
     *
     * Returns a random number distributed accordingly to the type of the
     * current instance.  
     *
     * The univariateContinuousDistribution can be used to obtain numbers
     * in the interval [0,max()], where max() is the method of this class.
     *
     * @return a random float number.
     */
    virtual float fdraw();

    /**
     * Virtual method to get a double precision random number.
     *
     * Returns a random number distributed accordingly to the type of the
     * current instance.  
     *
     * The univariateContinuousDistribution can be used to obtain numbers
     * in the interval [0,max()], where max() is the method of this class.
     *
     * @return a random float number.
     */
    virtual double draw();

    /**
     * Non-virtual method to get a single precision random number.
     *
     * Returns a random number distributed accordingly to the type of the
     * current instance.  
     *
     * This method can be used to obtain numbers in the interval [min(),max()].
     *
     * @return a random float number.
     */
    float frand();

    /**
     * Non-virtual method to get a double precision random number.
     *
     * Returns a random number distributed accordingly to the type of the
     * current instance.  
     *
     * This method can be used to obtain numbers in the interval [min(),max()].
     *
     * @return a random double number.
     */
    double rand();

    /**
     * Virtual method to obtain the minimum possible number
     * (inclusive) to be returned by this distribution.
     */
    double min();

    /**
     * Virtual method to obtain the maximum possible number
     * (exclusive) to be returned by this distribution.
     */
    double max();

    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    uniformContinuousDistribution& 
    copy(const uniformContinuousDistribution& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    uniformContinuousDistribution& 
    operator=(const uniformContinuousDistribution& other);

    /**
     * Returns the complete name of the functor class
     */
    virtual const std::string& name() const;

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual uniformContinuousDistribution* clone() const;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual uniformContinuousDistribution* newInstance() const;

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
     * Shadow of the parameters.min;
     */ 
    double minimum_;

    /**
     * Shadow of the parameters.max;
     */ 
    double maximum_;

    /**
     * Shadow of the parameters.min;
     */ 
    float fminimum_;

    /**
     * Shadow of the parameters.max;
     */ 
    float fmaximum_;

    /**
     * delta_ = (maximum_ - minimum_)*dnorm_
     */
    double delta_;

    /**
     * delta_ = (maximum_ - minimum_)*fnorm_
     */
    float fdelta_;
  };
}

#endif

