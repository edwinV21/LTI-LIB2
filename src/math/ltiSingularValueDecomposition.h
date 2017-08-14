/*
 * Copyright (C) 1998 - 2004
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
 * \file   ltiSingularValueDecomposition.h
 *         Compute the SVD of a matrix
 * \author Xin Gu
 * \date   11.01.2001
 *
 * $Id: ltiSingularValueDecomposition.h,v 1.16 2009-04-08 04:25:16 alvarado Exp $
 */

#ifndef _LTI_SINGULAR_VALUE_DECOMPOSITION_H_
#define _LTI_SINGULAR_VALUE_DECOMPOSITION_H_

#include "ltiMath.h"
#include "ltiMatrix.h"
#include "ltiLinearAlgebraFunctor.h"
#include "ltiContainer.h"

#ifdef HAVE_LAPACK
#include "ltiLapackInterface.h"
#endif

namespace lti {

  /**
   * Singular Value Decomposition.
   *
   * \deprecated Use lti::svd instead
   *
   * The functor will take a \f$m\times n\f$ matrix A and compute its singular
   * value decomposition, consisting of three matrices U, W, and V, with
   *
   * \f[
   *  A = U \cdot W \cdot V^*
   * \f]
   *
   * where \f$V^*\f$ denotes the conjugate transpose of V. U is a
   * column-orthonormal \f$m\times m\f$ matrix, W is a diagonal \f$m\times n\f$
   * matrix with the singular values on the diagonal, and V is a orthonormal
   * \f$n\times n\f$ matrix. Those columns of V whose corresponding entries in
   * W are zero are the basis of A's null space.
   *
   * You can find more theoretical information about a similar algorithm in
   * W. H. Press, S. A. Teukolsky, W. T. Vetterling, and B. P. Flannery:
   * Numerical Recipes in C, 2nd edition, Cambridge University Press, 1992.
   * For a quick review check the Wikipedia at
   * http://en.wikipedia.org/wiki/Singular_value_decomposition.
   *
   * This class is usually employed in the computation of linear mean squared
   * error.  For a quick theory review check
   * http://en.wikipedia.org/wiki/Linear_least_squares
   *
   * Only instantiations of floating point types makes sense (i.e. for
   * T double or float). If you want the singular values and
   * corresponding singular vectors to be sorted, you have to set
   * parameters::sort to true.
   *
   * This class uses LAPACK if it is available.
   *
   * @see singularValueDecomposition::parameters
   * 
   * @see lti::svd
   *
   * \section Why a new lti::svd class
   * 
   * The external template interface of this class is not appropriate since
   * it only supports the float and double types.  The newer class is not a
   * template anymore.  It uses an internal template but that will not bother
   * the final users anymore.
   */
  template<typename T>
  class singularValueDecomposition: public linearAlgebraFunctor
#ifdef HAVE_LAPACK
    , public lapackInterface 
#endif
  {
    
  public:
    
    /**
     * The parameters for the class
     */
    class parameters : public linearAlgebraFunctor::parameters {
    public:
      
      /**
       * default constructor
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
       * Assigns the contents of the other object to this object
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
       * Returns a pointer to a clone of the parameters
       */
      virtual parameters* newInstance() const;

      /**
       * Read the parameters from the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also read, otherwise only the data block will be read.
       * @return true if write was successful
       */
      virtual bool read(ioHandler &handler, const bool complete=true);

      /**
       * Write the parameters in the given ioHandler
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
       * Default: false.
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
       * This parameters is only used if useLapack is true.
       *
       * If true the divide-and-conquer method for calculating the SVD
       * is used. This is generally faster, especially on large
       * matrices. However, it uses more temporary memory than the
       * simple method. Thus, if the computation fails due to memory
       * problems setting this parameter to false might solve the
       * problem. 
       *
       * Defaul: true.
       */
      bool useDC;
      
      /**
       * This parameters is only used if useLapack is true.
       *
       * Let the data matrix have M rows and N columns. Then, usually,
       * U will be an MxM orthogonal matrix and V an NxN orthogonal
       * matrix. However, there are only min(M,N) singular values. For
       * most applications it suffices to calculate only min(M,N) left
       * and right singular vectors, which is done when
       * useMinDimensions is true. All singular values are
       * calculated when false.
       *
       * Default: true
       */
      bool useMinDimensions;
    };

    /**
     * Default constructor
     */
    singularValueDecomposition();

    /**
     * Default constructor with parameters
     */
    singularValueDecomposition(const parameters& params);

    /**
     * Constructor. Sets parameters::sort to the given value.
     */
    singularValueDecomposition(bool sort);

    /**
     * Copy constructor
     */
    singularValueDecomposition(const singularValueDecomposition<T>& other);

    /**
     * Destructor
     */
    virtual ~singularValueDecomposition();

    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    singularValueDecomposition& copy(const singularValueDecomposition& other);

    /**
     * Returns the name of this class.
     */
    const std::string& name() const;

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual singularValueDecomposition<T>* clone() const;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual singularValueDecomposition<T>* newInstance() const;

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * OnPlace version of Singular Value Decomposition.
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
    virtual bool apply(matrix<T>& src, vector<T>& w, matrix<T>& v) const;

    /**
     * OnCopy version of Singular Value Decomposition.
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
    virtual bool apply(const matrix<T>& src, matrix<T>& u,
                       vector<T>& w, matrix<T>& v) const;

  protected: 


    /**
     * OnPlace version of Singular Value Decomposition. Singular Value
     * Decomposition means that a m*n-matrix A is decomposed into three
     * matrices U,W,V, such that  A = U*W*V'. U is m*n, W is a diagonal
     * matrix with n elements (which is implemented as vector), V is a
     * n*n-matrix. Note that the function returns V, not V'.
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

#ifdef HAVE_LAPACK

    /**
     * uses Lapack methods to calculate the SVD. This includes setting
     * the enormous amount of parameters.
     */
    bool applyLapack(const matrix<T>& src, matrix<T>& u,
                     vector<T>& w, matrix<T>& v) const;
    
    
    /**
     *  *GESVD computes the singular value decomposition (SVD) of a real
     *  M-by-N matrix A, optionally computing the left and/or right singular
     *  vectors. The SVD is written
     *
     *       A = U * SIGMA * transpose(V)
     *
     *  where SIGMA is an M-by-N matrix which is zero except for its
     *  min(m,n) diagonal elements, U is an M-by-M orthogonal matrix, and
     *  V is an N-by-N orthogonal matrix.  The diagonal elements of SIGMA
     *  are the singular values of A; they are real and non-negative, and
     *  are returned in descending order.  The first min(m,n) columns of
     *  U and V are the left and right singular vectors of A.
     *
     *  Note that the routine returns V**T, not V.
     */
    int gesvd(char* jobu, char* jobvt,
	      integer* m, integer* n, T* a, integer* lda,
	      T* s, T* u, integer* ldu,
	      T* vt, integer* ldvt,
	      T* work, integer* lwork,
	      integer* info) const;
	
    /**
     *  *GESDD computes the singular value decomposition (SVD) of a real
     *  M-by-N matrix A, optionally computing the left and right singular
     *  vectors.  If singular vectors are desired, it uses a
     *  divide-and-conquer algorithm.
     *
     *  The SVD is written
     *
     *       A = U * SIGMA * transpose(V)
     *
     *  where SIGMA is an M-by-N matrix which is zero except for its
     *  min(m,n) diagonal elements, U is an M-by-M orthogonal matrix, and
     *  V is an N-by-N orthogonal matrix.  The diagonal elements of SIGMA
     *  are the singular values of A; they are real and non-negative, and
     *  are returned in descending order.  The first min(m,n) columns of
     *  U and V are the left and right singular vectors of A.
     *
     *  Note that the routine returns VT = V**T, not V.
     *
     *  The divide and conquer algorithm makes very mild assumptions about
     *  floating point arithmetic. It will work on machines with a guard
     *  digit in add/subtract, or on those binary machines without guard
     *  digits which subtract like the Cray X-MP, Cray Y-MP, Cray C-90, or
     *  Cray-2. It could conceivably fail on hexadecimal or decimal machines
     *  without guard digits, but we know of none.
     */
    int gesdd(char* jobz, integer* m, integer* n, T* a, integer* lda,
	      T* s, T* u, integer* ldu, T* vt, integer* ldvt,
	      T* work, integer* lwork, integer* iwork,
	      integer* info) const;
    
#endif

  private:

    /**
     * Help method:(a^2+b^2)^0.5 without destructive underflow or overflow
     */
    T pythag(const T a,const T b) const;

    /**
     * Sign
     */
    inline T sign(const T a,const T b) const;

    /**
     * Check if the given number is zero
     */
    inline bool isZero(const T x) const;

    /**
     * Check if the given number is not zero
     */
    inline bool notZero(const T x) const;

    /**
     * Compute the dot product of a part of two matrix rows
     */
    T dotOfRows(const matrix<T>& data,
                const int row1, const int row2,
                int lowCol=0, const int highCol=container::MaxIndex) const;
    
    /**
     * Compute the dot product of a part of two matrix columns
     */
    T dotOfColumns(const matrix<T>& data,
                   const int col1, const int col2,
                   int lowRow=0, int highRow=container::MaxIndex) const;
    
    /**
     * Compute the sum of a part of a matrix row
     */
    T sumOfRowPart(const matrix<T>& data,
                   const int row,
                   int lowCol=0, const int highCol=container::MaxIndex) const;

    /**
     * Compute the sum of a part of a matrix column
     */
    T sumOfColumnPart(const matrix<T>& data,
                      const int col,
                      int lowRow=0, int highRow=container::MaxIndex) const;
    
    /**
     * Compute the sum of the absolute value of a part of a matrix row
     */
    T sumOfAbsRowPart(const matrix<T>& data,
                      const int row,
                      int lowCol=0, const int highCol=container::MaxIndex) const;

    /**
     * Compute the sum of the absolute values of a part of a matrix column
     */
    T sumOfAbsColumnPart(const matrix<T>& data,
                         const int col,
                         int lowRow=0, int highRow=container::MaxIndex) const;

    void multiplyColumn(matrix<T>& data, const int col, const T factor,
                        int lowRow=0, int highRow=container::MaxIndex) const;

    void multiplyRow(matrix<T>& data, const int row, const T factor,
                     int lowCol=0, const int highCol=container::MaxIndex) const;

    void fillColumn(matrix<T>& data, const int col, const T value,
                    int lowRow=0, int highRow=container::MaxIndex) const;

    void fillRow(matrix<T>& data, const int row, const T value,
                 int lowCol=0, const int highCol=container::MaxIndex) const;
    
  };
}

#endif

