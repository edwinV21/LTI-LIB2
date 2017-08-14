/*
 * Copyright (C) 2008
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
 * \file   ltiPseudoInverseMP.h
 *         Contains the class lti::pseudoInverseMP, 
 *         which has still to be documented by Pablo Alvarado
 * 
 * \author Pablo Alvarado
 * \date   24.03.2008
 *
 * revisions ..: $Id: ltiPseudoInverseMP.h,v 1.2 2008-04-04 20:39:09 alvarado Exp $
 */

#ifndef _LTI_PSEUDO_INVERSE_M_P_H_
#define _LTI_PSEUDO_INVERSE_M_P_H_

#include "ltiMatrix.h"
#include "ltiLinearAlgebraFunctor.h"

namespace lti {

  /**
   * Class pseudoInverseMP
   *
   * This functor calculates the Moore-Penrose Pseudo-Inverse of a matrix.
   * It is used to solve an over-determined linear equation system according to
   * LMS. The pseudo-inverse of \f$H\f$ is defined as
   *
   * \f[ H^+= (H^t\dot H)^{-1}\dot H^t \f]
   *
   * For some applications smoothing of this solution is desired. This is
   * achieved through a smoothing matrix \f$\tilde{H}\f$ and a (small)
   * factor \f$\lambda\f$:
   *
   * \f[ H^+= (H^t\dot H + \lambda\tilde{H})^{-1}\dot H^t \f]
   *
   * where \f$\tilde{H}\f$ is called the smoothing matrix.
   *
   * @ingroup gLinearAlgebra
   */
  class pseudoInverseMP : public linearAlgebraFunctor {
  public:
    /**
     * The parameters for the class pseudoInverseMP
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
       * Smoothing factor.
       *
       * The smoothing factor is a (small) value \f$\lambda\f$ used to
       * construct the pseudo-inverse in the following manner:
       *
       * \f[ H^+= (H^t\dot H + \lambda\tilde{H})^{-1}\dot H^t \f]
       *
       * Default value: 0.01
       */
      double lambda;
    };

    /**
     * Default constructor
     */
    pseudoInverseMP();

    /**
     * Construct a functor using the given parameters
     */
    pseudoInverseMP(const parameters& par);

    /**
     * Construct a functor using the given smoothing factor lambda
     */
    pseudoInverseMP(const double& lambda);


    /**
     * Copy constructor
     * @param other the object to be copied
     */
    pseudoInverseMP(const pseudoInverseMP& other);

    /**
     * Destructor
     */
    virtual ~pseudoInverseMP();


    /**
     * Calculates the Moore-Penrose pseudo-inverse <b>without</b> smoothing.
     * @param src matrix<T> to be inverted
     * @param dest Moore-Penrose pseudo-inverse of src
     * @result a reference to <code>dest</code>.
     */
    template<typename T>
    matrix<T>& apply(const matrix<T>& src,matrix<T>& dest) const;

    /**
     * Calculates the Moore-Penrose pseudo-inverse <b>with</b> smoothing.
     * Do not forget to set parameters::lambda to the desired value.
     * @param src matrix<T> to be inverted
     * @param smooth matrix<T> used for smoothing
     * @param dest Moore-Penrose pseudo-inverse of src
     * @result a reference to <code>dest</code>.
     */
    template<typename T>
    matrix<T>& apply(const matrix<T>& src,
                     const matrix<T>& smooth,
                     matrix<T>& dest) const;


    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    pseudoInverseMP& copy(const pseudoInverseMP& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    pseudoInverseMP& operator=(const pseudoInverseMP& other);

    /**
     * Returns the complete name of the functor class
     */
    virtual const std::string& name() const;

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual pseudoInverseMP* clone() const;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual pseudoInverseMP* newInstance() const;

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;
  };
}

#include "ltiPseudoInverseMP_template.h"

#endif

