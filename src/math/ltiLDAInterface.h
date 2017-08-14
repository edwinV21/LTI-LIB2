/*
 * Copyright (C)  2000, 2001, 2002, 2003, 2004, 2005, 2006
 * Lehrstuhl fuer Technische Informatik, RWTH-Aachen, Germany
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
 * \file   ltiLDAInterface.h
 *         Linear Discriminant Analysis Interface
 * \author Pablo Alvarado
 * \date   9.10.2010
 *
 * revisions ..: $Id: ltiLDAInterface.h,v 1.1 2010-10-13 03:09:36 alvarado Exp $
 */

#ifndef _LTI_L_D_A_INTERFACE_H_
#define _LTI_L_D_A_INTERFACE_H_

#include "ltiMatrix.h"

namespace lti {
  /**
   * Abstract interface for Linear Discriminant Analysis (LDA) clases.
   *
   * This is a pure abstract interface for all classes that perform any
   * sort of discriminant components analysis.
   *
   * @see lti::lda, lti::klda
   *
   * @ingroup gLinearAlgebra
   */
  template <typename T>
  class ldaInterface {
  public:

    /**
     * Destructor
     */
    virtual ~ldaInterface() {};

    /**
     * Computes the linear discriminant components of the data matrix
     * and transforms it according to the new coordinate system.
     * The result is the transformed matrix.
     * Data and result must not be references to the same matrix.
     * Data points are expected to be in the rows of the data matrix.
     *
     * If you don't need to transform the input data, and just want to
     * use the input matrix to compute the discriminant components you
     * can use the method computeTransformMatrix().  If you just need
     * to transform the data, without computing the transformation matrix, you
     * can use the method transform().
     *
     * @param data matrix with the source data.
     * @param labels for each row of the data matrix, this vector contains the 
     *        class label
     * @param result matrix<T> with the result data.
     * @return true if the LDA could be computed, false otherwise
     */
    virtual bool apply(const matrix<T>& data, 
                       const ivector& labels,
                       matrix<T>& result) = 0;

    /**
     * On-Place version of the transformation.
     *
     * If you don't need to transform the input data, and just want to
     * use the input matrix to compute the discriminant components you
     * can use the method computeTransformMatrix().  If you just need
     * to transform the data, without computing the transformation matrix, you
     * can use the method transform().
     *
     * @param srcdest matrix with the source data, which will also contain
     *        the result.
     * @param labels for each row of the data matrix, this vector contains the 
     *        class label
     * @return a reference to <code>srcdest</code>.
     */
    virtual bool apply(matrix<T>& srcdest, 
                       const ivector& labels) = 0;

    /**
     * Transforms a single vector according to a previously computed
     * transformation matrix. (this is an alias for the transform() method)
     */
    virtual bool apply(const vector<T> &src, vector<T>& result) = 0;

    /**
     * Transforms a single vector according to a previously computed
     * transformation matrix.
     *
     * @param src the data vector
     * @param result the vector which will receive the transformed data
     * @return a reference to <code>result</code>
     */
    virtual bool transform(const vector<T> &src, vector<T>& result) const = 0;

    /**
     * Transform an entire matrix according to a previously computed
     * transformation matrix. Unfortunately, we must choose a name
     * different from apply.
     *
     * @param src the data matrix
     * @param result the matrix which will receive the transformed data
     * @return true if successful, false otherwise.
     */
    virtual bool transform(const matrix<T> &src, matrix<T>& result) const = 0;

    /**
     * Transform an entire matrix according to a previously computed
     * transformation matrix.
     * @param srcdest the data matrix.  The result will be left here too.
     * @return true if successful, false otherwise.
     */
    virtual bool transform(matrix<T> &srcdest) const = 0;

    /**
     * Compute the transformation matrix.  Similar to the apply() method,
     * but it does not transform the given data (this saves some time).
     *
     * @param src the matrix with the input data to be analysed.
     * @param labels for each row of the data matrix, this vector contains the 
     *        class label
     * @return true if transformation matrix could be computed, false otherwise
     */
    virtual bool computeTransformMatrix(const matrix<T>& src,
                                        const ivector& labels) = 0;

    /**
     * Alias for computeTransformMatrix()
     */
    virtual bool train(const matrix<T>& src,
                       const ivector& labels) = 0;

    /**
     * Reconstructs a data vector \c dest from the given coefficients
     * \c coeff, using the transformMatrix found by
     * computeTransformMatrix() or apply() and the appropriate offset.
     *
     * @param coeff LDA coefficients for reconstruction.
     * @param dest reconstructed data vector.
     * @return true if reconstruction was successful
     */
    virtual bool reconstruct(const vector<T>& coeff, vector<T>& dest) const = 0;

    /**
     * Reconstructs a set of data vectors \c dest from the given
     * coefficients \c coeff, using the transformMatrix found by
     * computeTransformMatrix() or apply() and the appropriate
     * offset. As usual \c coeff as well as \c dest contain one data
     * vector per row.
     *
     * @param coeff each row contains LDA coefficients for reconstruction.
     * @param dest each row is one reconstructed data vector.
     * @return true if reconstruction was successful
     */
    virtual bool reconstruct(const matrix<T>& coeff, matrix<T>& dest) const = 0;

    /**
     * Returns the previously computed transform matrix.
     *
     * @param result the matrix which will receive the transformation
     *        matrix.
     * @return true if the matrix could be computed, false otherwise.
     */
    virtual bool getTransformMatrix(matrix<T>& result) const = 0;

    /**
     * Returns the previously computed transform matrix.
     *
     * @return a const reference to the last computed or used transformation
     *           matrix.
     */
    virtual const matrix<T>& getTransformMatrix() const = 0;

    /**
     * Returns the previously computed eigenvalues of the covariance
     * matrix.
     *
     * @param result the vector which will receive the eigenvalues.
     * @return true if the values could be obtained, false otherwise.
     */
    virtual bool getEigenValues(vector<T>& result) const = 0;

    /**
     * Returns the previously computed eigenvalues of the covariance
     * matrix.
     *
     * @return a const reference to the last computed eigenvalues
     */
    virtual const vector<T>& getEigenValues() const = 0;

    /**
     * Returns the previously computed eigenvectors of the covariance
     * matrix.
     *
     * @param result the matrix which will receive the eigenvectors.
     *        Each column of the matrix contains one eigenvector.
     * @return true if the vectors could be obtained, false otherwise
     */
    virtual bool getEigenVectors(matrix<T>& result) const = 0;

    /**
     * Returns the previously computed eigenvectors of the covariance
     * matrix.
     *
     * This method will call the normal getEigenVectors() methods and
     * after that will transpose the obtained matrix, i.e. it is faster
     * to get the eigenvectors in the columns.
     *
     * @param result the matrix which will receive the eigenvectors.
     *        Each row of the matrix contains one eigenvector.
     * @return true if the vectors could be obtained, false otherwise
     */
    virtual bool getEigenVectorsInRows(matrix<T>& result) const = 0;

    /**
     * Returns the previously computed eigenvectors of the covariance
     * matrix.
     *
     * @return a const reference to the last computed eigenvectors
     */
    virtual const matrix<T>& getEigenVectors() const = 0;

    /**
     * Set the dimension to which the vectors should be reduced.  This
     * is just considered when computing the transformation
     * matrix. After this matrix is determined the (destination)
     * dimension is fixed and just can be changed by recalculating the
     * transformation matrix.
     */
    virtual void setDimension(int k) = 0;

  };


}

#endif
