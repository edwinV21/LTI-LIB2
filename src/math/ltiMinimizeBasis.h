/*
 * Copyright (C) 1998-2004
 * Lehrstuhl fuer Technische Informatik, RWTH-Aachen, Germany
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
 * \file   ltiMinimizeBasis.h
 *         Contains the template class lti::minimizeBasis<T>, which generates
 *         a minimum number of basis vectors to approximate a given
 *         set of vectors within a given deviation.
 * \author Thomas Rusert
 * \date   09.06.1999
 *
 * $Id: ltiMinimizeBasis.h,v 1.4 2005-01-07 15:22:18 doerfler Exp $
 */

#ifndef _LTI_MINIMIZE_BASIS_H_
#define _LTI_MINIMIZE_BASIS_H_

#include "ltiMatrix.h"
#include "ltiVector.h"
#include "ltiLinearAlgebraFunctor.h"

namespace lti {

  /**
   * Functor for the minimization of a basis.
   *
   * This functor generates a minimum number of basis vectors to approximate
   * a given set of vectors within a given deviation.
   *
   * There is a Fast and an Exact method of computing the minimized
   * basis.  Normally the Exact method should be used, because it does
   * not only compute the exact solution and the smallest basis, but
   * may even be faster than the "Fast" method. The "Fast" method is
   * not exact and may return a basis which is larger than
   * necessary.
   *
   * @ingroup gLinearAlgebra
   */
  template<typename T>
  class minimizeBasis : public linearAlgebraFunctor  {
  public:
    /**
     * Enumeration of computation methods 
     */
    enum eMethod {Fast, /**< the "Fast" method may not return a basis 
			 *   which is larger than necessary. 
			 */
		  Exact /**< the "Exact" method not only compute the
			 *   exact solution and the smallest basis, but
			 *   may even be faster than the "Fast" method.
			 */
    };
    
    /** 
     * Enumeration of deviation types 
     */
    enum eDeviationType {Element, /**< the elements of the approximating 
				   *   vectors must not deviate more than
				   *   maxDeviation */
			 Vector,  /**< the l2VectorNorm of deviation vector
				   *   between the approximated vector must
				   *   not be larger than maxDeviation
				   */
			 Matrix   /**< the l2MatrixNorm of the difference
				   *   matrix between the approximating
				   *   and the approximated set of vectors 
				   *   must not be larger than maxDeviation
				   */
    };

    /**
     * The parameters for the class minimizeBasis
     */
    class parameters : public linearAlgebraFunctor::parameters {
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
       * Returns the name of this class.
       */
      const std::string& name() const;

      /**
       * Returns a pointer to a clone of the parameters
       */
      virtual parameters* clone() const;

      /**
       * Returns a pointer to a new instance of the parameters
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
       * Method used for computation 
       *
       * Default: Exact
       */
      eMethod method;

      /**
       * Deviation type
       *
       * Default: Element
       */
      eDeviationType deviationType;
      
      /**
       * Maximal deviation
       *
       * Default: 0.1
       */
      double maxDeviation;
      

    };

    /**
     * Default constructor
     */
    minimizeBasis();

    /**
     * Construct a functor using the given parameters
     */
    minimizeBasis(const parameters& par);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    minimizeBasis(const minimizeBasis<T>& other);

    /**
     * Destructor
     */
    virtual ~minimizeBasis();

    /**
     * Operates on the given %parameter.
     * @param testVectors the rows of this matrix
     *        should contain the vectors to be approximated
     * @param newBasis the columns of this matrix 
     *        will contain the new basis vectors
     * @param factors the rows of this matrix will contain
     *        the factors to multiply the new basis vectors
     *        by to approximate the testVectors
     * @return true if apply successful or false otherwise.
     */
    bool apply(const matrix<T>& testVectors,
	       matrix<T>& newBasis,
	       matrix<T>& factors) const;


    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    minimizeBasis<T>& copy(const minimizeBasis<T>& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    minimizeBasis<T>& operator=(const minimizeBasis<T>& other);

    /**
     * Returns the name of this class.
     */
    const std::string& name() const;

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual minimizeBasis<T>* clone() const;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual minimizeBasis<T>* newInstance() const;

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;

  };
}

#include "ltiMinimizeBasis_template.h"

#endif

