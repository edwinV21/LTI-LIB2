/*
 * Copyright (C)  2000, 2001, 2002, 2003, 2004, 2005, 2006
 * Lehrstuhl fuer Technische Informatik, RWTH-Aachen, Germany
 * Copyright (C) 2010
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
 * \file   ltiLDA.h
 *         Linear Discriminant Analysis
 * \author Pablo Alvarado
 * \author Jochen Wickel
 * \date   09.10.2010
 *
 * revisions ..: $Id: ltiLDA.h,v 1.1 2010-10-13 03:09:36 alvarado Exp $
 */

#ifndef _LTI_L_D_A_H_
#define _LTI_L_D_A_H_

#include "ltiLinearAlgebraFunctor.h"
#include "ltiLDAInterface.h"
#include "ltiMatrix.h"
#include "ltiMatrixInversion.h"

namespace lti {
  /**
   * Linear Discriminant Analysis (LDA).
   *
   * Functor to perform the linear discriminant analysis of a labeled  data set.
   *
   * It receives a set of input vectors in form of a matrix (each row of the
   * matrix corresponds to an input vector) and a set of labels for each
   * row-vector, which will be used to transform the data using LDA.
   *
   * The first time you use the apply()-method, the transformation
   * matrix will be computed.  You can use this transformation matrix
   * with other data sets using the transform() methods.
   *
   * Please note that the eigenvector matrices will contain the
   * eigenvector in the COLUMNS and not in the rows, as could be
   * expected.  This avoids the requirement of transposing matrices in
   * the vector transformations.
   *
   * For large data matrices is is advisable to use singular value
   * decomposition instead of an eigensystem for the PCA. The
   * operation will usually be faster and using less memory. To do so
   * set parameters::useSVD to true.
   *
   * @see lti::lda::parameters
   *
   * @ingroup gLinearAlgebra
   */
  template <typename T>
  class lda : public ldaInterface<T>,
              public linearAlgebraFunctor {
  public:
    /**
     * The parameters for the class lda
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
       * Return  name of this class
       */
      const std::string& name() const;

      /**
       * Copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& copy(const parameters& other);

      /**
       * Assigns the contents of the other object to this object
       */
      parameters& operator=(const parameters& other);

      /**
       * Return a pointer to a clone of the parameters
       */
      virtual parameters* clone() const;

      /**
       * Return a pointer to a new instance of the parameters
       */
      virtual parameters* newInstance() const;

      /**
       * Read the parameters from the given ioHandler
       *
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also read, otherwise only the data block will be read.
       * @return true if write was successful
       */
      virtual bool read(ioHandler &handler, const bool complete=true);

      /**
       * Write the parameters in the given ioHandler.
       *
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be
       *        written.
       * @return true if write was successful
       */
      virtual bool write(ioHandler& handler,const bool complete=true) const;

      // --------------
      // The parameters
      // --------------

      /**
       * Final dimension of the reduced vectors.
       *
       * This attribute is used only if autoDimension is set to false.
       *
       * If set to zero, then all dimensions will be used.
       *
       * Default value: 3
       */
      int resultDimension;

      /**
       * This flag determines, if the functor should determine a
       * maximum allowable dimension itself. "Maximum dimension" means
       * that the dimension is equal to the number of eigenvalues of
       * the covariance matrix which are larger than zero.
       *
       * Default value: \c false
       */
      bool autoDimension;

      /**
       * The factor which determines relevant eigenvectors. An eigenvector
       * is considered relevant if its eigenvalue is at least as large
       * as the largest eigenvalue divided by this number. Usually,
       * it takes values between 1e4 and 1e6.
       *
       * Default value: T(100000)
       */
      T relevance;

      /**
       * This flag denotes if the transformed data should be
       * centered around zero. This is the usual behaviour of
       * the LDA, but for some special operations it may be
       * necessary to disable this. If the flag is false,
       * the mean of the transformed data is moved to the transformed
       * mean of the source data.
       *
       * Default value: true
       */
      bool centerData;

      /**
       * When \c true, singular value decomposition instead of eigensystem
       * solution is used to calculate the eigenvectors and
       * eigenvalues. This can be much faster and less memory consuming.
       *
       * Default value: \c false.
       */
      bool useSVD;
    };

  protected:
    
    /**
     * Default constructor.
     *
     * @param createDefaultParams if true (default) a default parameters
     *                            object will be created.
     */
    lda(const bool createDefaultParams);

  public:
    /**
     * Default constructor.
     */
    lda();

    /**
     * Default constructor with parameters
     * 
     */
    lda(const parameters& par);

    /**
     * Copy constructor.
     *
     * @param other the object to be copied
     */
    lda(const lda& other);

    /**
     * Destructor
     */
    virtual ~lda();

    /**
     * Returns the name of this type ("lda")
     */
    virtual const std::string& name() const;

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
                       matrix<T>& result);

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
                       const ivector& labels);

    /**
     * Transforms a single vector according to a previously computed
     * transformation matrix. (this is an alias for the transform() method)
     */
    virtual inline bool apply(const vector<T> &src, vector<T>& result) {
      return transform(src,result);
    };

    /**
     * Transforms a single vector according to a previously computed
     * transformation matrix.
     *
     * @param src the data vector
     * @param result the vector which will receive the transformed data
     * @return a reference to <code>result</code>
     */
    virtual bool transform(const vector<T> &src, vector<T>& result) const;

    /**
     * Transform an entire matrix according to a previously computed
     * transformation matrix. Unfortunately, we must choose a name
     * different from apply.
     *
     * @param src the data matrix
     * @param result the matrix which will receive the transformed data
     * @return true if successful, false otherwise.
     */
    virtual bool transform(const matrix<T> &src, matrix<T>& result) const;

    /**
     * Transform an entire matrix according to a previously computed
     * transformation matrix.
     * @param srcdest the data matrix.  The result will be left here too.
     * @return true if successful, false otherwise.
     */
    virtual bool transform(matrix<T> &srcdest) const;

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
                                        const ivector& labels);

    /**
     * Alias for computeTransformMatrix()
     */
    virtual bool train(const matrix<T>& src,
                       const ivector& labels);

    /**
     * Reconstructs a data vector \c dest from the given coefficients
     * \c coeff, using the transformMatrix found by
     * computeTransformMatrix() or apply() and the appropriate offset.
     *
     * @param coeff LDA coefficients for reconstruction.
     * @param dest reconstructed data vector.
     * @return true if reconstruction was successful
     */
    virtual bool reconstruct(const vector<T>& coeff, vector<T>& dest) const;

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
    virtual bool reconstruct(const matrix<T>& coeff, matrix<T>& dest) const;

    /**
     * Returns the previously computed transform matrix.
     *
     * @param result the matrix which will receive the transformation
     *        matrix.
     * @return true if the matrix could be computed, false otherwise.
     */
    virtual bool getTransformMatrix(matrix<T>& result) const;

    /**
     * Returns the previously computed transform matrix.
     *
     * @return a const reference to the last computed or used transformation
     *           matrix.
     */
    virtual const matrix<T>& getTransformMatrix() const;

    /**
     * Returns the previously computed eigenvalues of the covariance
     * matrix.
     *
     * @param result the vector which will receive the eigenvalues.
     * @return true if the values could be obtained, false otherwise.
     */
    virtual bool getEigenValues(vector<T>& result) const;

    /**
     * Returns the previously computed eigenvalues of the covariance
     * matrix.
     *
     * @return a const reference to the last computed eigenvalues
     */
    virtual const vector<T>& getEigenValues() const;

    /**
     * Returns the previously computed eigenvectors of the covariance
     * matrix.
     *
     * @param result the matrix which will receive the eigenvectors.
     *        Each column of the matrix contains one eigenvector.
     * @return true if the vectors could be obtained, false otherwise
     */
    virtual bool getEigenVectors(matrix<T>& result) const;

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
    virtual bool getEigenVectorsInRows(matrix<T>& result) const;

    /**
     * Returns the previously computed eigenvectors of the covariance
     * matrix.
     *
     * @return a const reference to the last computed eigenvectors
     */
    virtual const matrix<T>& getEigenVectors() const;

    /**
     * Return the mean value of all used vectors.
     */
    virtual bool getOffsetVector(vector<T>& result) const;

    /**
     * Return the mean value of all used vectors.
     */
    virtual const vector<T>& getOffsetVector() const;

    /**
     * Set the dimension to which the vectors should be reduced.  This
     * is just considered when computing the transformation
     * matrix. After this matrix is determined the (destination)
     * dimension is fixed and just can be changed by recalculating the
     * transformation matrix.
     */
    virtual void setDimension(int k);

    /**
     * Copy data of "other" functor.
     *
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    lda& copy(const lda& other);

    /**
     * Alias for copy.
     */
    lda& operator=(const lda& other);

    /**
     * Return a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * Return a pointer to a new instance of this functor.
     */
    virtual functor* newInstance() const;

    /**
     * Return used parameters
     */
    const parameters& getParameters() const;

    /**
     * Update functor's parameters.
     *
     * This member initializes some internal data according to the values in
     * the parameters.
     *
     * @return true if successful, false otherwise
     */
    virtual bool updateParameters();

    /**
     * Read this functor from the given handler.
     */
    virtual bool read(ioHandler &handler, const bool complete=true);

    /**
     * Write this functor to the given handler.
     */
    virtual bool write(ioHandler &handler, const bool complete=true) const;

    /**
     * Number of dimensions considered in the transformation
     * @return the number of dimensions used for the transformation.  It
     *         is always less or equal the number of dimensions of the input
     *         vectors.
     */
    int getUsedDimension() const;

  protected:

    /**
     * Compute the transformation matrix.  Similar to the apply() method,
     * but it does not transform the given data (this saves some time).
     *
     * This method computes the transformation matrix through the inverse
     * of \f$S_w^{-1}S_b\f$.
     * 
     * @param src the matrix with the input data to be analysed.
     * @param labels for each row of the data matrix, this vector contains the 
     *        class label
     * @return true if transformation matrix could be computed, false otherwise
     */
    virtual bool computeTransformMatrix1(const matrix<T>& src,
                                         const ivector& labels);

    /**
     * Compute the transformation matrix.  Similar to the apply() method,
     * but it does not transform the given data (this saves some time).
     *
     * This method computes the transformation matrix through the eigenvalues
     * of the symmetric matrix \f$S_b^{1/2}S_w^{-1}S_b^{1/2}\f$.
     * 
     * @param src the matrix with the input data to be analysed.
     * @param labels for each row of the data matrix, this vector contains the 
     *        class label
     * @return true if transformation matrix could be computed, false otherwise
     */
    virtual bool computeTransformMatrix2(const matrix<T>& src,
                                         const ivector& labels);

    /**
     * Compute the between-class scatter matrix Sb and the within-class scatter
     * matrix Sw
     */
    bool computeSbSw(const matrix<T>& src, 
                     const ivector& labels,
                     matrix<T>& Sb,
                     matrix<T>& Sw,
                     vector<T>& offset) const;

    /**
     * Determines the intrinsic dimensionality of the data set if the
     * user specify autoDim, otherwise return parameters::resultDim.
     * The attribute usedDimensionality_ will be set with the returned value
     */
    int checkDim();

    /**
     * Resets all private members to size 0. Used when an error occurs
     * in the calculation of the transform matrix.
     */
    void reset();

    /**
     * From the intra and inter covariances build the transformation matrix
     */
    bool buildTransform(const matrix<T>& Sw, const matrix<T>& Sb);


  protected:
    /**
     * Matrix inversion functor
     */
    matrixInversion<T> inv_;

    /**
     * Ordered eigen vectors
     */
    matrix<T> orderedEigVec_;

    /**
     * Transformation matrix
     */
    matrix<T> transformMatrix_;

    /**
     * Eigenvalues
     */
    vector<T> eigValues_;

    /**
     * Offset
     */
    vector<T> offset_;

    /**
     * Transformed offset
     */
    vector<T> transformedOffset_;

    /**
     * Dimensionality being used.  This value is set by the checkDim() method.
     */
    int usedDimensionality_;
    
  };


}

#endif
