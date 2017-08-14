/*
 * Copyright (C) 2007
 * ITCR, Pablo Alvarado
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
 * \file   ltiUnivariateContinuousDistribution.h
 *         Contains the class lti::univariateContinuousDistribution, 
 *         parent of all univariate continuous random distributions.
 * \author Pablo Alvarado
 * \date   23.09.2007
 *
 * revisions ..: $Id: ltiUnivariateContinuousDistribution.h,v 1.1 2007-09-26 21:21:52 alvarado Exp $
 */

#ifndef _LTI_UNIVARIATE_CONTINUOUS_DISTRIBUTION_H_
#define _LTI_UNIVARIATE_CONTINUOUS_DISTRIBUTION_H_

#include "ltiRandomDistribution.h"

namespace lti {

  /**
   * Class univariateContinuousDistribution
   *
   * All univariate continuous distributions return random numbers in a
   * floating point format.  This class establishes the basic interface to
   * allow virtual inheritance of the apply methods.
   *
   * @see univariateContinuousDistribution::parameters.
   *
   * @ingroup gRandomContinuous
   */
  class univariateContinuousDistribution : public randomDistribution {
  public:
    /**
     * Default constructor.
     *
     * Inherited classes can turn off the parameter initialization, since
     * they do the job.
     */
    univariateContinuousDistribution(const bool initParams=true);

    /**
     * Construct a functor using the given parameters
     */
    univariateContinuousDistribution(const parameters& par);

    /**
     * Copy constructor
     * @param o the object to be copied
     */
    univariateContinuousDistribution(const univariateContinuousDistribution& o);
    /**
     * Destructor
     */
    virtual ~univariateContinuousDistribution();

    /**
     * Get a random number.
     *
     * Returns a random number distributed accordingly to the type of the
     * current instance.  
     *
     * The univariateContinuousDistribution can be used to obtain numbers
     * in the interval [0,1).
     *
     * @param rnd float reference where the random number has to be left.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(float& rnd);

    /**
     * Get a random number.
     *
     * Returns a random number distributed accordingly to the type of the
     * current instance.  
     *
     * The univariateContinuousDistribution can be used to obtain numbers
     * in the interval [0,1).
     *
     * @param rnd double reference where the random number has to be left.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(double& rnd);

    /**
     * Get a single precision random number.
     *
     * Returns a random number distributed accordingly to the type of the
     * current instance.  
     *
     * The univariateContinuousDistribution can be used to obtain numbers
     * in the interval [0,1).
     *
     * @return a random float number.
     */
    virtual float fdraw();

    /**
     * Get a double precision random number.
     *
     * Returns a random number distributed accordingly to the type of the
     * current instance.  
     *
     * The univariateContinuousDistribution can be used to obtain numbers
     * in the interval [0,1).
     *
     * @return a random double number.
     */
    virtual double draw();

    /**
     * Non-virtual method to get a single precision random number.
     *
     * Returns a random number distributed accordingly to the type of the
     * current instance.  
     *
     * The univariateContinuousDistribution can be used to obtain numbers
     * in the interval [0,1).
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
     * The univariateContinuousDistribution can be used to obtain numbers
     * in the interval [0,1).
     *
     * @return a random double number.
     */
    double rand();


    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    univariateContinuousDistribution& 
    copy(const univariateContinuousDistribution& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    univariateContinuousDistribution& 
    operator=(const univariateContinuousDistribution& other);

    /**
     * Returns the complete name of the functor class
     */
    virtual const std::string& name() const;

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual univariateContinuousDistribution* clone() const;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual univariateContinuousDistribution* newInstance() const;

    /**
     * Returns used parameters
     */
    bool updateParameters();
    
  protected:
    /**
     * Float normalizer.
     */
    float fnorm_;

    /**
     * Double normalizer.
     */
    double dnorm_;

  };
}

#endif

