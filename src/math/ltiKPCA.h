/*
 * Copyright (C) 2000, 2001, 2002, 2003, 2004, 2005, 2006
 * Lehrstuhl fuer Technische Informatik, RWTH-Aachen, Germany
 *
 * Copyright (C) 2009
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
 * \file   ltiKPCA.h
 *         Contains the functor to perform a kernel principal components
 *         analysis.
 * \author Pablo Alvarado
 * \author Jochen Wickel
 * \date   27.11.2009
 *
 * revisions ..: $Id: ltiKPCA.h,v 1.2 2009-06-07 23:47:43 alvarado Exp $
 */

#ifndef _LTI_K_P_C_A_H_
#define _LTI_K_P_C_A_H_

#include "ltiLinearAlgebraFunctor.h"
#include "ltiKernelFunctorInterface.h"
#include "ltiPCAInterface.h"

namespace lti {
  /**
   * Kernel PCA
   *
   * This functor performs a kernel principal component analysis.
   *
   * It receives a set of input vectors in form of a matrix (each row of
   * the matrix corresponds to an input vector), which will be transformed
   * with KPCA.
   *
   * The first time you use the apply()-method, the transformation
   * matrix will be computed.  You can use this transformation matrix
   * with other data sets using the transform() methods.
   *
   * Please note that the eigenvector matrices will contain the
   * eigenvector in the COLUMNS and not in the rows, as could be
   * expected.  This avoids the requirement of transposing matrices in
   * the vector transformations (see also lti::eigenSystem<T>).
   *
   * The Kernel PCA is described in Schoelkopf, B., Smola, A. and
   * Mueller, K. "Nonlinear Component Analysis as a Kernel Eigenvalue
   * Problem", Neural Computation, vol. 10, no. 5, pp. 1299-1319
   *
   * The reconstruction process uses conjugate gradients in the optimization,
   * as suggested in several technical papers.
   * 
   * Please remember that this algorithms have to store the training data, since
   * it has to be used to span the eigenvectors in a high-dimensional
   * space.  This means that huge amounts of memory will be used with large
   * data sets.  It is therefore recommended to use previous clustering methods
   * and employ the centroids of the clusters as a data compression solution.
   * 
   *
   * @see kpca::parameters.
   *
   * @see lti::linearKernel,lti::polynomialKernel,lti::sigmoidKernel
   * @see lti::radialKernel
   *
   * @ingroup gKernelMath
   */
  template <typename T>
  class kpca: public linearAlgebraFunctor,
              public pcaInterface<T> {
  public:
    /**
     * Parameters for the class lti::kpca
     */
    class parameters : public linearAlgebraFunctor::parameters {
    public:

      /**
       * Default constructor
       */
      parameters();

      /**
       * Copy constructor
       *
       * @param other the parameters object to be copied
       */
      parameters(const parameters& other);

      /**
       * Destructor
       */
      ~parameters();

      /**
       * Returns name of this type
       */
      virtual const std::string& name() const;

      /**
       * Copy the contents of a parameters object
       *
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& copy(const parameters& other);

      /**
       * Assigns the contents of the other object to this object
       */
      inline parameters& operator=(const parameters& other);

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
       *
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool write(ioHandler& handler,const bool complete=true) const;

      /**
       * Read the parameters from the given ioHandler
       *
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool read(ioHandler& handler,const bool complete=true);

      /**
       * Sets a new kernel function.  A copy of the kernel will be
       * done (so it is useless to change the parameters of the given
       * kernel instance, because the internal kernel will never
       * notice the changes done to its "parent").
       */
      void setKernel(const kernelFunctorInterface<T>& k);

      /**
       * Sets a new kernel function. The kernel which is passed here as an
       * argument must have been allocated with new; it must not be
       * a local variable. On destruction of the parameters object,
       * the kernel will be deleted, i.e. this parameters instance will be
       * responsible for the memory managment of the object.
       */
      void attachKernel(kernelFunctorInterface<T>* k);

      /**
       * Sets a new kernel function. The kernel which is passed here as an
       * argument is not deleted by the parameters object, the caller
       * must ensure that there are no memory leaks.
       */
      void useKernel(kernelFunctorInterface<T>* k);

      // ------------------------------------------------
      // the parameters
      // ------------------------------------------------

      /**
       * Kernel functor.
       *
       * Try to use the kernel setting methods in order to ensure a
       * consistent memory managment of the pointed instance.  
       *
       * @see setKernel(),attachKernel(),useKernel()
       *
       * Default value: a radialKernel with default parameters
       */
      kernelFunctorInterface<T>* kernel;

      /**
       * Maximal dimension of the reduced vectors.
       *
       * Default value: 3
       */
      int resultDimension;

      /**
       * Automatically set the dimension.
       *
       * This flag determines, if the functor should determine a
       * maximum allowable dimension itself.
       *
       * "Maximum dimension" means that the dimension is equal to the
       * number of eigenvalues of the covariance matrix which are
       * larger than zero.
       *
       * Default value: false
       */
      bool autoDimension;

      /**
       * This flag determines if the functor should perform a
       * whitening transform of the data. Whitening means that
       * 1. A KPCA is performed
       * 2. A scaling of the transformed data by the inverse of the
       *    square root of the eigenvalues.
       *
       * The default is false.
       */
      bool whitening;

      /**
       * Relevance of eigenvectors.
       *
       * The factor which determines relevant eigenvectors. An eigenvector
       * is considered relevant if its eigenvalue is at least as large
       * as the largest eigenvalue divided by this number. Usually,
       * it takes values between 1e4 and 1e6.
       *
       * Default value: 1e5
       */
      T relevance;

    private:
      /**
       * Flag used to inidicate if the local kernel functor must be deleted
       * by this object or not (just pointing to some external instance...)
       */
      bool destroyKernel_;

    };

    /**
     * Default constructor
     */
    kpca();

    /**
     * Construct with given parameters
     */
    kpca(const parameters& pars);
    

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    kpca(const kpca<T>& other);

    /**
     * Destructor
     */
    virtual ~kpca();

    /**
     * Returns the name of this type ("kpca")
     */
    virtual const std::string& name() const;

    /**
     * Computes the principal components of the data matrix
     * and transforms it according to the new coordinate system.
     * The result is the transformed matrix.
     * \c data and \c result must not be references to the same matrix.
     *
     * If you don't need to transform the input data, and just want to
     * use the input matrix to compute the principal components you
     * can use the method computeTransformMatrix().  If you just need
     * to transform the data, without computing the transformation matrix, you
     * can use the method transform().
     *
     * @param data matrix<T> with the source data.
     * @param result matrix<T> with the result data.
     * @return true if the KPCA could be computed, false otherwise
     */
    virtual bool apply(const matrix<T>& data, matrix<T>& result);

    /**
     * On-Place version of the transformation.
     *
     * If you don't need to transform the input data, and just want to
     * use the input matrix to compute the principal components you
     * can use the method computeTransformMatrix().  If you just need
     * to transform the data, without computing the transformation matrix, you
     * can use the method transform().
     *
     * @param srcdest matrix<T> with the source data, which will also contain
     *        the result.
     * @return a reference to <code>srcdest</code>.
     */
    virtual bool apply(matrix<T>& srcdest);

    /**
     * Transforms a single vector according to a previously computed
     * transformation matrix. (this is an alias for the transform() method)
     */
    virtual inline bool apply(const vector<T> &src, vector<T>& result) {
      return transform(src,result);
    }

    /**
     * Transforms a single vector according to a previously computed
     * transformation matrix.
     * @param src the data vector
     * @param result the vector which will receive the transformed data
     * @return a reference to <code>result</code>
     */
    virtual bool transform(const vector<T> &src, vector<T>& result) const;

    /**
     * Transform an entire matrix according to a previously computed
     * transformation matrix. 
     *
     * @param src the data matrix
     * @param result the matrix which will receive the transformed data
     * @return true if successful, false otherwise.
     */
    virtual bool transform(const matrix<T> &src, matrix<T>& result) const;

    /**
     * Transform an entire matrix according to a previously computed
     * transformation matrix.
     *
     * @param srcdest the data matrix.  The result will be left here too.
     * @return true if successful, false otherwise.
     */
    virtual bool transform(matrix<T> &srcdest) const;

    /**
     * Compute the transformation matrix.  Similar to the apply() method,
     * but it does not transform the given data (this saves some time).
     *
     * @param src the matrix with the input data to be analysed.
     * @return true if transformation matrix could be computed, false otherwise
     */
    virtual bool computeTransformMatrix(const matrix<T>& src);

    /**
     * Alias for computeTransformMatrix()
     */
    virtual bool train(const matrix<T>& src);

    /**
     * Reconstructs a data vector \c dest from the given coefficients
     * \c coeff.
     *
     * In kernel PCA reconstruction is a costly operation, since it
     * involves an optimization process, due to the fact that not all
     * vectors in the high-dimensional eigenspace have corresponding
     * vectors in the original space.  This fact forces to search for
     * the "nearest" equivalent point.  As optimization method here
     * the well-known conjugate-gradients algorithm is used.
     *
     * @param coeff PCA coefficients for reconstruction.
     * @param dest reconstructed data vector.
     * @return true if reconstruction was successful
     */
    virtual bool reconstruct(const vector<T>& coeff, vector<T>& dest) const;

    /**
     * Reconstructs a set of data vectors \c dest from the given
     * coefficients \c coeff. As usual \c coeff as well as \c dest
     * contain one data vector per row.
     *
     * In kernel PCA reconstruction is a costly operation, since it
     * involves an optimization process, due to the fact that not all
     * vectors in the high-dimensional eigenspace have corresponding
     * vectors in the original space.  This fact forces to search for
     * the "nearest" equivalent point.  As optimization method here
     * the well-known conjugate-gradients algorithm is used.
     *
     * @param coeff each row contains PCA coefficients for reconstruction.
     * @param dest each row is one reconstructed data vector.
     * @return true if reconstruction was successful
     */
    virtual bool reconstruct(const matrix<T>& coeff, matrix<T>& dest) const;

    /**
     * Returns the previously computed eigenvalues of the covariance
     * matrix.
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
     * matrix.  Each ROW (as opposite to PCA) will contain the eigenvectors.
     *
     * @param result the matrix which will receive the eigenvectors.
     *        Each row of the matrix contains one eigenvector.
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
     * Returns the previously computed eigenvectors of the covariance
     * matrix.
     * @return a const reference to the last computed eigenvectors.
     *         Each row of the matrix contains one eigenvector.
     */
    virtual const matrix<T>& getEigenVectors() const;

    /**
     * Set the dimension to which the vectors should be reduced.
     */
    virtual void setDimension(int k);

    /**
     * Number of dimensions considered in the transformation
     * @return the number of dimensions used for the transformation.  It
     *         is always less or equal the number of dimensions of the input
     *         vectors.
     */
    int getUsedDimension() const;

    /**
     * Copy data of "other" functor.
     *
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    kpca<T>& copy(const kpca& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual kpca<T>* clone() const;

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual kpca<T>* newInstance() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * set functor's parameters.
     * This member makes a copy of <em>theParam</em>: the functor
     * will keep its own copy of the parameters!
     * @return true if successful, false otherwise
     */
    virtual bool updateParameters();

    /**
     * Reads this functor from the given handler.
     */
    virtual bool read(ioHandler &handler, const bool complete=true);

    /**
     * Writes this functor to the given handler.
     */
    virtual bool write(ioHandler &handler, const bool complete=true) const;

    /**
     * Create a new kernel functor with default parameters.
     *
     * Use the class name of the functor.
     *
     * A pointer to the kernel function is returned, so that the
     * caller may modify the kernel's parameters.
     *
     * Valid name are for instance "lti::linearKernel<T>" or
     * "lti::polynomialKernel<T>" or "lti::radialKernel<T>", with T the
     * proper type name like "double" or "float".
     */
    static kernelFunctorInterface<T>*
    createKernel(const std::string& name);

    /**
     * Debuggin method used to verify the correctness of the used adapters
     */
    void checkAdapter() const;

  protected:
    /**
     * Determines the intrinsic dimensionality of the data set if the
     * user specify autoDim, otherwise return parameters::resultDim.
     * The member usedDimensionality will be set with the returned value
     */
    int checkDim();

    /**
     * Compute kernel matrix.  The data will be centered.
     */
    bool computeKernelMatrix(const matrix<T>& src,matrix<T>& kmat);

    /**
     * Compute test kernel matrix.  The data will be centered.
     * It will be assumed that the source data (srcData) contains the
     * right data
     */
    bool computeTestKernelMatrix(const matrix<T>& src,matrix<T>& kmat) const;

    /**
     * Compute test kernel vector.  The data will be centered.
     * It will be assumed that the source data (srcData_) contains the
     * right data.
     *
     * @param src vector to be mapped to the high-dimensional space
     * @param kvct resulting kernel products of src with all input data
     * @param sum sum of the elements of the kvct, divided by m,
     *        before it is centered in the high dimensional space.
     */
    bool computeTestKernelVector(const vector<T>& src,
                                       vector<T>& kvct,
                                       T& sum) const;

    /**
     * Compute test kernel vector uncentered.
     * It will be assumed that the source data (srcData_) contains the
     * right data
     */
    bool computeTestKernelVectorUncentered(const vector<T>& src,
                                           vector<T>& kvct) const;

    /**
     * Method used to compute part of the gradient of an objetive
     * function.  The part computed is given by
     *
     * \f[
     *   -2\sum_{i=1}^{M} p_i
     *   \sum_{j=1}^{N} \alpha_{j}^{(i)} \dfrac{\partial}{\partial x}k(x,x_j)
     *   +
     *   \dfrac{2}{M} \sum_{j=1}^{N}{\partial x}k(x,x_j)
     *   \left[ 
     *     \sum_{i=1}^M p_i \sum_{j=1}^{M} \alpha_j^{(i)}-1
     *   \right]
     * \f]
     * 
     * which is a vector of dimension equal to the size of the vector x.
     *
     * @param x point where gradient has to be evaluated
     * @param ap product of A (transformation matrix) times p (principal 
     *           components)
     * @param cst constant calculated outside
     * @param result the resulting vector
     * @return a reference to <code>result</code>
     */
    bool dtransform(const vector<T>& x,
                    const vector<T>& ap,
                    const T& cst,
                          vector<T>& result) const;

  private:

    /**
     * Cloning a kernel is a long process, so here we have a simple interface
     * for it.
     */
    static kernelFunctorInterface<T>* 
    cloneKernel(const kernelFunctorInterface<T>& kernel);

    /**
     * Kernel PCA requires the original data set
     */
    matrix<T> srcData_;

    /**
     * Mean of rows of srcData
     */
    vector<T> kUnit_;

    /**
     * Mean of columns of srcData
     */
    vector<T> unitK_;

    /**
     * Mean of all values in the srcData
     */
    T unitKunit_;

    /**
     * Ordered set of eigenvectors (sorted with decreasing order of
     * eigenvalues).
     */
    matrix<T> orderedEigVec_;

    /**
     * Transformation matrix will contain just a subset of the
     * orderedEigVec matrix.
     */
    matrix<T> transformMatrix_;

    /**
     * Vector resulting from the sum of all rows of the transformMatrix_
     */
    vector<T> transMatRowSum_;

    /**
     * Ordered eigenvalues (in decreasing order)
     */
    vector<T> eigValues_;

    /**
     * Scaling factors for the whitening transformation
     */
    vector<T> whiteScale_;

    /**
     * Dimensionality being used.
     */
    int usedDimensionality_;

  private:
    /**
     * This private class is used to provide the interface to the
     * conjugateGradient functor
     */
    class adapter;

    friend class adapter;
  };
}

#endif
