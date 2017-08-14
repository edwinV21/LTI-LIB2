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
 * \file   ltiUnivariateDiscreteDistribution.h
 *         Contains the class lti::univariateDiscreteDistribution, 
 *         parent of all univariate discrete random distributions.
 * \author Pablo Alvarado
 * \date   23.09.2007
 *
 * revisions ..: $Id: ltiUnivariateDiscreteDistribution.h,v 1.3 2010-04-22 02:25:41 alvarado Exp $
 */

#ifndef _LTI_UNIVARIATE_DISCRETE_DISTRIBUTION_H_
#define _LTI_UNIVARIATE_DISCRETE_DISTRIBUTION_H_

#include "ltiRandomDistribution.h"

namespace lti {

  /**
   * Class univariateDiscreteDistribution
   *
   * All univariate discrete distributions return random numbers in an
   * integer type.  This class establishes the basic interface to
   * allow virtual inheritance of the apply methods.
   *
   * @see univariateDiscreteDistribution::parameters.
   *
   * @ingroup gRandomDiscrete
   */
  class univariateDiscreteDistribution : public randomDistribution {
  public:
    /**
     * Default constructor
     */
    univariateDiscreteDistribution();

    /**
     * Copy constructor
     * @param othe the object to be copied
     */
    univariateDiscreteDistribution(const univariateDiscreteDistribution& othe);

    /**
     * Destructor
     */
    virtual ~univariateDiscreteDistribution();

    /**
     * Get a random number.
     *
     * Returns a random number distributed accordingly to the type of the
     * current instance.  
     *
     * The univariateDiscreteDistribution can be used to obtain numbers
     * in the interval [0,max()].
     *
     * @param rnd double reference where the random number has to be left.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(int& rnd) = 0;

    /**
     * Virtual method to get a integer random number.
     *
     * Returns a random number distributed accordingly to the type of the
     * current instance.  
     *
     * The univariateDiscreteDistribution can be used to obtain numbers
     * in the interval [0,max()], where max() is the method of this class.
     *
     * @return a random float number.
     */
    virtual int draw() = 0;

    /**
     * Non-virtual method to get a integer random number.
     *
     * Returns a random number distributed accordingly to the type of the
     * current instance.  
     *
     * The univariateDiscreteDistribution can be used to obtain numbers
     * in the interval [0,max()], where max() is the method of this class.
     *
     * @return a random float number.
     */
    int rand();

    /**
     * Virtual method to obtain the maximum possible number
     * (inclusive) to be returned by this distribution
     */
    virtual int max() const = 0;

    /**
     * Virtual method to obtain the minimum possible number
     * (inclusive) to be returned by this distribution.
     */
    virtual int min() const = 0;

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual univariateDiscreteDistribution* clone() const = 0;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual univariateDiscreteDistribution* newInstance() const = 0;
    
  };
}

#endif

