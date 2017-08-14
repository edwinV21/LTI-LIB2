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
 * \file   ltiNormalDistribution.h
 *         Contains the class lti::normalDistribution to
 *         produce random integers normally distributed in an specified 
 *         interval
 * \author Pablo Alvarado
 * \date   25.09.2007
 *
 * revisions ..: $Id: ltiNormalDistribution.h,v 1.2 2007-09-29 00:37:11 alvarado Exp $
 */

#ifndef _LTI_NORMAL_DISTRIBUTION_H_
#define _LTI_NORMAL_DISTRIBUTION_H_

#include "ltiUnivariateContinuousDistribution.h"

namespace lti {

  /**
   * Class normalDistribution
   *
   * This class generates (pseudo) random numbers normally distributed
   * with mean and standard deviation specified in the parameters.
   *
   * The equation for the normal probability density distribution is:
   * \f[
   *   p(x) = \frac{1}{\sigma\sqrt{2\pi}} 
   *                       e^{\frac{1}{2}\left(\frac{x-\mu}{\sigma}\right)^2}
   * \f]
   *
   * where \f$\mu\f$ stands for the mean value and \f$\sigma\f$ for
   * the standard deviation.
   *
   * @see normalDistribution::parameters.
   *
   * @ingroup gRandomContinuous
   */
  class normalDistribution : public univariateContinuousDistribution {

  public:
    /**
     * The parameters for the class normalDistribution
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
       * Mean value of the distribution.
       *
       * Default value: 0.0
       */
      double mean;

      /**
       * Standard deviation of the distribution.
       *
       * The variance is the square of the standard deviation, and
       * hence, the value you indicate here is the square root of the
       * variance.
       *
       * Default value: 1.0
       */
      double sigma;
    };

    /**
     * Default constructor
     */
    normalDistribution();

    /**
     * Constructor with a given interval.
     *
     * @param mean mean value (\f$\mu\f$) of the normal distribution
     * @param sigma standard deviation of the distribution
     */
    normalDistribution(const double mean,const double sigma);

    /**
     * Construct a functor using the given parameters
     */
    normalDistribution(const parameters& par);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    normalDistribution(const normalDistribution& other);

    /**
     * Destructor
     */
    virtual ~normalDistribution();

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
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    normalDistribution& copy(const normalDistribution& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    normalDistribution& operator=(const normalDistribution& other);

    /**
     * Returns the complete name of the functor class
     */
    virtual const std::string& name() const;

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual normalDistribution* clone() const;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual normalDistribution* newInstance() const;

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
     * Simple structure to shadow the parameters and precomputations.
     *
     * The template type has to be float or double
     */
    template<typename T>
    struct shadows {
      /**
       * Default constructor
       */
      shadows();

      /**
       * Shadow of the parameters.sigma;
       */ 
      T sigma;

      /**
       * Shadow of the parameters.mean;
       */ 
      T mu;

      /**
       * Precomputed value
       */
      T precomputed;

      /**
       * Flag to indicate if a precomputation is available
       */
      bool precomputationAvailable;

      /**
       * Normalization constant
       */
      T norm;
    };

    /**
     * Parameters and status for float precision values
     */
    shadows<float> fshadow_;

    /**
     * Parameters and status for double precision values
     */
    shadows<double> dshadow_;

  };
}

#endif

