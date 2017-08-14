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
 * \file   ltiSVD.h
 *         Singular Value Decomposition
 * \author Xin Gu
 * \author Pablo Alvarado
 * \date   11.01.2001
 *
 * revisions ..: $Id: ltiSVD.h,v 1.3 2009-04-08 03:41:37 alvarado Exp $
 */


#ifndef LTI_S_V_D_H_
#define LTI_S_V_D_H_

#include "ltiMath.h"
#include "ltiMatrix.h"
#include "ltiLinearAlgebraFunctor.h"

#ifdef HAVE_LAPACK
#include "ltiLapackInterface.h"
#endif

namespace lti {

  /**
   * Singular Value Decomposition (SVD).
   *
   * The functor will take a matrix A and compute its singular value
   * decomposition, consisting of three matrices U, W, and V, with
   * \f[
   *  A = U \cdot W \cdot V^*
   * \f]
   * where \f$V'\f$ denotes the transpose of \f$V\f$. \f$U\f$ is a
   * column-orthonormal matrix, \f$W\f$ is a diagonal matrix with the
   * singular values on the diagonal, and V is a orthonormal
   * matrix. Those columns of V whose corresponding entries in W are
   * zero are the basis of A's null space.
   *
   * You can find more theoretical information about a similar
   * algorithm in W. H. Press, S. A. Teukolsky, W. T. Vetterling, and
   * B. P. Flannery: Numerical Recipes in C, 2nd edition, Cambridge
   * University Press, 1992.
   *
   * For a quick review check the Wikipedia at
   * http://en.wikipedia.org/wiki/Singular_value_decomposition.
   *
   * This class uses LAPACK if it is available.
   *
   * You should prefer this class over the
   * lti::singularValueDecomposition<T> which is now deprecated.
   *
   * @see lti::svd::parameters
   * 
   * @ingroup gLinearAlgebra
   */
  class svd: public linearAlgebraFunctor {
  public:

    /**
     * The parameters for the class
     */
    class parameters : public linearAlgebraFunctor::parameters {
    public:
      /**
       * Default constructor
       */
      parameters();

      /**
       * Copy constructor.
       *
       * @param other the parameters object to be copied
       */
      parameters(const parameters& other);

      /**
       * destructor
       */
      ~parameters();

      /**
       * returns name of this type
       */
      const std::string& name() const;

      /**
       * copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& copy(const parameters& other);

      /**
       * Assigns the contents of the other object to this object
       */
      inline parameters& operator=(const parameters& other);

      /**
       * Returns a pointer to a clone of the parameters.
       */
      virtual parameters* clone() const;

      /**
       * Returns a pointer to a clone of the parameters.
       */
      virtual parameters* newInstance() const;

      /**
       * Read the parameters from the given ioHandler.
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

      // ---------------------------------------------------
      // the parameters
      // ---------------------------------------------------

      /**
       * If true, singular values and corresponding singular vectors
       * are rearranged so that singular values have descending
       * order.
       *
       * Default is false.
       */
      bool sort;

      /**
       * Specifies whether U (false) or Ut (true) is returned. It is
       * faster to return Ut.
       *
       * Default is false, ie U.
       */
      bool transposeU;

      /**
       * Specifies whether V (false) or Vt (true) is returned. It is
       * faster to return V.
       *
       * Default is false, ie V.
       */
      bool transposeV;

      /**
       * @name LAPACK options
       *
       * The following attributes are considered if the LAPACK library is
       * detected, and otherwise just ignored.
       */
      //@{
      /**
       * If \c true the divide-and-conquer method for calculating the SVD is
       * used. This is generally faster, especially on large matrices. However,
       * it uses more temporary memory than the simple method. Thus, if the
       * computation fails due to memory problems setting this parameter to \c
       * false might solve the problem.
       *
       * Default value: \c true
       */
      bool useDivideAndConquer;

      /**
       * Let the data matrix have M rows and N columns. Then, usually, U will
       * be an MxM orthogonal matrix and V an NxN orthogonal matrix. However,
       * there are only min(M,N) singular values. For most applications it
       * suffices to calculate only min(M,N) left and right singular vectors,
       * which is done when \c useMinDimensions is true (default). All singular
       * values are calculated when false.
       *
       * Default value: \c true
       */
      bool useMinDimensions;
      //@}
    };

    /**
     * Default constructor
     */
    svd();

    /**
     * Default constructor with parameters
     */
    svd(const parameters& params);

    /**
     * Constructor.
     *
     * Sets parameters::sort to the given value.
     */
    svd(bool sort);

    /**
     * Copy constructor
     */
    svd(const svd& other);

    /**
     * Destructor
     */
    virtual ~svd();

    /**
     * Returns the name of this type ("svd")
     */
    virtual const std::string& name() const;

    /**
     * Copy data of "other" functor.
     *
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    svd& copy(const svd& other);

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual svd* clone() const;

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual svd* newInstance() const;

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * OnPlace version of Singular Value Decomposition.
     *
     * Singular Value Decomposition means that a m*n-matrix A is
     * decomposed into three matrices U,W,V, such that A = U*W*V'. U
     * is m*n, W is a diagonal matrix with n elements (which is
     * implemented as vector), V is a n*n-matrix.
     *
     * Note that the function returns V, not V'. 
     *
     * @param src matrix<T> with the source matrix, will also contain
     *            the U matrix after the function has returned. If src is a m*n
     *            matrix, U will also be of size m*n
     * @param w vector<T> with the singular values, sorted
     *            descendingly The elements of this vector constitute the
     *            diagonal of the W matrix. 
     * @param v the V matrix
     *
     * @return \c true is the decomposition was successfull, 
     *         \c false if an error occured
     */
    bool decomposition(fmatrix& src, fvector& w, fmatrix& v) const;

    /**
     * OnPlace version of Singular Value Decomposition.
     *
     * Singular Value Decomposition means that a m*n-matrix A is
     * decomposed into three matrices U,W,V, such that A = U*W*V'. U
     * is m*n, W is a diagonal matrix with n elements (which is
     * implemented as vector), V is a n*n-matrix.
     *
     * Note that the function returns V, not V'. 
     *
     * @param src matrix<T> with the source matrix, will also contain
     *            the U matrix after the function has returned. If src is a m*n
     *            matrix, U will also be of size m*n
     * @param w vector<T> with the singular values, sorted
     *            descendingly The elements of this vector constitute the
     *            diagonal of the W matrix. 
     * @param v the V matrix
     *
     * @return \c true is the decomposition was successfull, 
     *         \c false if an error occured
     */
    bool decomposition(dmatrix& src, dvector& w, dmatrix& v) const;

    /**
     * OnPlace version of Singular Value Decomposition.
     *
     * @param src matrix<T> with the source matrix, will also contain
     *            the U matrix after the function has returned.
     * @param w vector<T> with the singular values, sorted descendingly if
     *                    parameters::sort is true.
     *                    The elements of this vector constitute the diagonal
     *                    of the W matrix.
     * @param v the V matrix
     * @return true is the decomposition was successfull, false if an
     *         error occured
     */
    virtual bool apply(fmatrix& src, fvector& w, fmatrix& v) const;

    /**
     * OnPlace version of Singular Value Decomposition.
     *
     * @param src matrix<T> with the source matrix, will also contain
     *            the U matrix after the function has returned.
     * @param w vector<T> with the singular values, sorted descendingly if
     *                    parameters::sort is true.
     *                    The elements of this vector constitute the diagonal
     *                    of the W matrix.
     * @param v the V matrix
     * @return true is the decomposition was successfull, false if an
     *         error occured
     */
    virtual bool apply(dmatrix& src, dvector& w, dmatrix& v) const;

    /**
     * OnCopy version of Singular Value Decomposition.
     *
     * @param src matrix<T> with the source matrix
     * @param u the U matrix
     * @param w vector<T> with the singular values, sorted descendingly if
     *                    parameters::sort is true.
     *                    The elements of this vector constitute the diagonal
     *                    of the W matrix.
     * @param v the V matrix
     * @return true is the decomposition was successfull, false if an
     *         error occured
     */
    virtual bool apply(const fmatrix& src, fmatrix& u,
                       fvector& w, fmatrix& v) const;

    /**
     * OnCopy version of Singular Value Decomposition.
     *
     * @param src matrix<T> with the source matrix
     * @param u the U matrix
     * @param w vector<T> with the singular values, sorted descendingly if
     *                    parameters::sort is true.
     *                    The elements of this vector constitute the diagonal
     *                    of the W matrix.
     * @param v the V matrix
     * @return true is the decomposition was successfull, false if an
     *         error occured
     */
    virtual bool apply(const dmatrix& src, dmatrix& u,
                       dvector& w, dmatrix& v) const;

  protected:

    /**
     * Helper class
     */
    template<typename T>
    class helper 
#ifdef HAVE_LAPACK
    : public lapackInterface
#endif
    {
    public:
      /**
       * The only constructor expects a parameter object
       */
      helper(const parameters& par);

      /**
       * On-Copy version of Singular Value Decomposition. Singular Value
       *
       * Decomposition means that a m*n-matrix A is decomposed into three
       * matrices U,W,V, such that  A = U*W*V'. U is m*n, W is a diagonal
       * matrix with n elements (which is implemented as vector), V is a
       * n*n-matrix. Note that the function returns V, not V'.
       *
       * @param src matrix<T> with the source matrix, will also contain
       *            the U matrix after the function has returned. If
       *            src is a m*n matrix, U will also be of size m*n
       * @param u the U matrix
       * @param w vector<T> with the singular values, sorted descendingly
       *                    The elements of this vector constitute the diagonal
       *                    of the W matrix.
       * @param v the V matrix
       * @return true is the decomposition was successfull, false if an
       *         error occured
       */
      bool decomposition(const matrix<T>& src,
                         matrix<T>& u,
                         vector<T>& w,
                         matrix<T>& v) const;

      /**
       * On-Place version of Singular Value Decomposition. Singular Value
       *
       * Decomposition means that a m*n-matrix A is decomposed into three
       * matrices U,W,V, such that  A = U*W*V'. U is m*n, W is a diagonal
       * matrix with n elements (which is implemented as vector), V is a
       * n*n-matrix. Note that the function returns V, not V'.
       *
       * @param src matrix<T> with the source matrix, will also contain
       *            the U matrix after the function has returned. If
       *            src is a m*n matrix, U will also be of size m*n
       * @param w vector<T> with the singular values, sorted descendingly
       *                    The elements of this vector constitute the diagonal
       *                    of the W matrix.
       * @param v the V matrix
       * @return true is the decomposition was successfull, false if an
       *         error occured
       */
      bool decomposition(matrix<T>& src, vector<T>& w, matrix<T>& v) const;

    private:
      /**
       * Reference to the parameters
       */
      const parameters& params_;

      /**
       * help method:(a^2+b^2)^0.5 without destructive underflow or overflow
       */
      inline T pythag(const T a,const T b) const;
      
      /**
       * sign
       */
      inline T sign(const T a,const T b) const;
      
      /**
       * check if the given number is zero
       */
      inline bool isZero(const T x) const;
      
      /**
       * check if the given number is not zero
       */
      inline bool notZero(const T x) const;
      
      /**
       * Compute the dot product of a part of two matrix rows
       */
      inline T dotOfRows(const matrix<T>& data,
                         const int row1, 
                         const int row2,
                               int lowCol=0,
                         const int highCol=container::MaxIndex) const;
      
      /**
       * Compute the dot product of a part of two matrix columns
       */
      inline T dotOfColumns(const matrix<T>& data,
                            const int col1,
                            const int col2,
                                  int lowRow=0,
                            const int highRow=container::MaxIndex) const;
      
      /**
       * Compute the sum of a part of a matrix row
       */
      inline T sumOfRowPart(const matrix<T>& data,
                            const int row,
                                  int lowCol=0, 
                            const int highCol=container::MaxIndex) const;

      /**
       * Compute the sum of a part of a matrix column
       */
      inline T sumOfColumnPart(const matrix<T>& data,
                               const int col,
                               int lowRow=0,
                               const int highRow=container::MaxIndex) const;
      
      
      /**
       * Compute the sum of the absolute value of a part of a matrix row
       */
      inline T sumOfAbsRowPart(const matrix<T>& data,
                               const int row,
                               int lowCol=0,
                               const int highCol=container::MaxIndex) const;
      
      /**
       * Compute the sum of the absolute values of a part of a matrix column
       */
      inline T sumOfAbsColumnPart(const matrix<T>& data,
                                  const int col,
                                  int lowRow=0, 
                                  const int highRow=container::MaxIndex) const;
      
      inline void multiplyColumn(matrix<T>& data, 
                                 const int col,
                                 const T factor,
                                 int lowRow=0,
                                 const int highRow=container::MaxIndex) const;
      
      inline void multiplyRow(matrix<T>& data, 
                              const int row,
                              const T factor,
                              int lowCol=0,
                              const int highCol=container::MaxIndex) const;
      
      inline void fillColumn(matrix<T>& data, 
                             const int col, 
                             const T value,
                             int lowRow=0, 
                             const int highRow=container::MaxIndex) const;
      
      inline void fillRow(matrix<T>& data, 
                          const int row, 
                          const T value,
                          int lowCol=0,
                          const int highCol=container::MaxIndex) const;

      /**
       * OnPlace version of Singular Value Decomposition. Singular Value
       *
       * Decomposition means that a m*n-matrix A is decomposed into three
       * matrices U,W,V, such that  A = U*W*V'. U is m*n, W is a diagonal
       * matrix with n elements (which is implemented as vector), V is a
       * n*n-matrix. Note that the function returns V, not V'.
       *
       * @param src matrix<T> with the source matrix, will also contain
       *            the U matrix after the function has returned. If
       *            src is a m*n matrix, U will also be of size m*n
       * @param w vector<T> with the singular values, sorted descendingly
       *                    The elements of this vector constitute the diagonal
       *                    of the W matrix.
       * @param v the V matrix
       * @return true is the decomposition was successfull, false if an
       *         error occured
       */
      bool decompositionLocal(matrix<T>& src,
                              vector<T>& w,
                              matrix<T>& v) const;


#ifdef HAVE_LAPACK

      // lapack routine in template form
      
      int svd(char* jobu, char* jobvt,
              integer* m, integer* n, T* a, integer* lda,
              T* s, T* u, integer* ldu, T* vt, integer* ldvt,
              T* work, integer* lwork,
              integer* info) const;
      
      int sdd(char* jobz, integer* m, integer* n, T* a, integer* lda,
              T* s, T* u, integer* ldu, T* vt, integer* ldvt,
              T* work, integer* lwork, integer* iwork,
              integer* info) const;

      bool lapackApply(const matrix<T>& theMatrix,
                       matrix<T>& leftSV,
                       vector<T>& singularValues,
                       matrix<T>& rightSVtrans) const;

      bool lapackApply(matrix<T>& theMatrix,
                       vector<T>& singularValues,
                       matrix<T>& rightSVtrans) const;
#endif
      
    };

  };
}

#include "ltiSVD_inline.h"

#endif
