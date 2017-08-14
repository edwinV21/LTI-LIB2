/*
 * Copyright (C) 1998, 1999, 2000, 2001
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
 * \file   ltiQrSolution.h
 * \author Thomas Rusert
 * \date   02.06.1999
 *
 * $Id: ltiQrSolution.h,v 1.2 2008-03-18 21:04:58 alvarado Exp $
 */

#ifndef _LTI_QR_SOLUTION_H_
#define _LTI_QR_SOLUTION_H_

#include "ltiDecompositionSolution.h"

namespace lti {

  /**
   * QR solution functor.
   *
   * Solves the linear equation Ax=b as a least-squares problem using QR
   * decomposition A=QR  (Householder transformation) of the given
   * (m,n)-matrix A.
   *
   * @see decompositionSolution::parameters
   * @ingroup gLTILib2FormatRequired
   */
  template<class T>
  class qrSolution : public decompositionSolution<T> {
  public:
    /**
     * qrSolution parameter class
     */
    class parameters : public decompositionSolution<T>::parameters {
    public:
      /**
       * default constructor
       */
      parameters();

      /**
       * copy constructor
       */
      parameters(const parameters& other);

      /**
       * copy member.
       */
      parameters& copy(const parameters& other);

      /**
       * Returns the name of this class.
       */
      const std::string& name() const;

      /**
       * returns a pointer to a clone of the parameters.
       */
      virtual parameters* clone() const;

      /**
       * Returns a pointer to a new instance of the parameters.
       */
      virtual parameters* newInstance() const;

      // the parameters

      /**
       * compute residuum?
       */
      bool computeResiduum;
      

    };

    /**
     * default constructor
     */
    qrSolution();

    /**
     * copy constructor
     */
    qrSolution(const qrSolution& other);

    /**
     * constructor, sets the parameters
     */
    qrSolution(const parameters& theParams);

    /**
     * constructor, sets the matrix A
     */
    qrSolution(const matrix<T>& theMatrix);

    /**
     * returns the current parameters.
     */
    const parameters& getParameters() const;

    /** onPlace version of apply.
        Solves the least-squares problem Ax=b and returns the residuum if
        computeResiduum==true.

        WARNING: For use with multiple right sides b of a set of
        equation systems Ax=b, the matrix decomposition is computed
        only on calling <em>apply()</em> the first time. After that
        the existing decomposition will be used until calling
        <em>setParameters()</em>.
    */
    double apply(vector<T>& b);

    /** onCopy version of apply.
        Solves the least-squares problem Ax=b and returns the residuum if
        computeResiduum==true.

        WARNING: For use with multiple right sides b of a set of
        equation systems Ax=b, the matrix decomposition is computed
        only on calling <em>apply()</em> the first time. After that
        the existing decomposition will be used until calling
        <em>setParameters()</em>.
    */
    double apply(const vector<T>& b,vector<T>& x);

    /**
     * copy data of "other" functor.
     */
    qrSolution& copy(const qrSolution& other);

    /**
     * Returns the name of this class.
     */
    const std::string& name() const;

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual qrSolution* clone() const;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual qrSolution* newInstance() const;

  protected:
    vector<double> dcmpVec_;
    vector<double> helpVec_;
  };

}

#endif
