/*
 * Copyright (C) 1998 - 2005
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
 * \file   ltiSecondOrderStatistics.h
 *         Contains the functor secondOrderStatistics, for means,
 *         variances and covariance matrix
 * \author Jochen Wickel
 * \author Peter Doerfler
 * \date   25.09.2000
 *
 * $Id: ltiSecondOrderStatistics.h,v 1.8 2007-10-07 03:17:47 alvarado Exp $
 */

#ifndef _LTI_SECOND_ORDER_STATISTICS_H_
#define _LTI_SECOND_ORDER_STATISTICS_H_

#include "ltiFunctor.h"
#include "ltiVector.h"
#include "ltiMatrix.h"
#include "ltiDataRepresentationType.h"

namespace lti {
  /**
   * Computes mean, variance, and/or covariance matrix of data
   * matrices and vectors. Correlations can be obtained as well.
   *
   * The first template parameter (T) represents the type of the input data,
   * and the second (U) the type of the output data, which defaults to T
   * if not specified. The user should ensure that the result values fit into
   * U, that is, T*T should be assignable to a U without loss of precision.
   * 
   * \section rcrep Row or column representations
   *
   * If the data samples are given in form of a matrix data samples
   * (points) can be in the rows or columns of the matrix. The LTI-Lib
   * usually assumes rows, but here you can choose via
   * parameters::dataRepresentationType.
   *
   * The following example clarifies the usage of
   * eDataRepresentationType. Given the data matrix
   *
   * \code
   * |  5  6  10  2 |
   * |  1 -2   4  4 |
   * |  3  2   1 -3 |
   * \endcode
   *
   * With the default parameters (Rows, Empirical, no correlation)
   * calling variance() yields (4 8 21 13). Switching to Maxlikely yields 
   * (2.667 5.333 14 8.667). 
   *
   * When the data samples are set to be in Columns the empirical
   * variance is (8.667 4.667 2)^T. The Maxlikely variance is 
   * (6.5 3.5 1.5)^T.
   *
   * \section vtype Variance type
   *
   * Variances and covariances can either be calculated Empirical
   * (unbiased) or as maximum likelihood (Maxlikely, second
   * moments). With N the number of samples, the former normalizes by
   * dividing by N-1 the latter by N.
   *
   * To obtain a correlation matrix instead of a covariance matrix set
   * parameters::useCorrelation to true.
   *
   * Besides the mandatory apply-methods, this functor also offers a number
   * of shortcuts to calculate only first or second order
   * statistics. However, if both are needed it is always faster to
   * call an apply() method that serves your purpose.
   *
   * \section cplx Complex numbers
   *
   * This class works with matrices and vectors of complex numbers too.  The
   * definition of mean and variance is kept as with real numbers.
   *
   * \ingroup gStatistics
   */
  template <typename T,  typename U = T>
  class secondOrderStatistics : public functor {
  public:

    /**
     * Type of the variance. It can either be empirical or maximum
     * likelihood.
     */
    enum eVarianceType {
      Empirical=0, /**< Empirical Variance:
                    *   \f[ \frac{1}{n-1} \sum_i^n (x_i - \mu)^2 \f]
                    */
      Maxlikely=1  /**< Maximum Likelihood Variance:
                    *   \f[ \frac{1}{n} \sum_i^n (x_i - \mu)^2 \f]
                    */
    };

    /**
     * the parameters for the class secondOrderStatistics
     */
    class parameters : public functor::parameters {
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
       * Copy the contents of a parameters object.
       *
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& copy(const parameters& other);

      /**
       * Copy the contents of a parameters object
       * \param other the parameters object to be copied
       * \return a reference to this parameters object
       */
      parameters& operator=(const parameters& other);
      
      /**
       * Returns the name of this class.
       */
      const std::string& name() const;

      /**
       * Write the parameters in the given ioHandler
       *
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be
       *        written.
       * @return true if write was successful
       */
      virtual bool write(ioHandler& handler, const bool complete=true) const;

      /**
       * Read the parameters from the given ioHandler
       *
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also read, otherwise only the data block will be read.
       * @return true if write was successful
       */
      virtual bool read(ioHandler& handler, const bool complete = true);

      /**
       * Returns a pointer to a clone of the parameters
       */
      virtual parameters* clone() const;

      /**
       * Returns a pointer to a new instance of the parameters
       */
      virtual parameters* newInstance() const;

      //***************************************
      // The parameters
      //***************************************

      /**
       * The LTI-Lib usually represents a data sample as a row of a
       * matrix. However, by choosing Columns here, you can achieve
       * the opposite behavior. This is not recommended since it is
       * confusing and much slower!
       *
       * Default value: Rows
       */
      eDataRepresentationType dataRepresentationType;

      /**
       * The type of the variance computation. If Empirical is used,
       * the empirical variance or covariance matrix is computed
       * (division by number of samples minus 1), otherwise, the
       * maximum likelihood estimator is computed (division by number
       * of samples).
       *
       * Default value: Empirical
       */
      eVarianceType varianceType;

      /**
       * If this flag is true, the covariance matrix is normalized
       * to contain the correlation coefficients instead of the
       * covariances.
       *
       * Default value: false
       */
      bool useCorrelation;
    };

    /**
     * Default constructor
     */
    secondOrderStatistics(eDataRepresentationType dataRep=Rows);

    /**
     * Copy constructor
     *
     * @param other the object to be copied
     */
    secondOrderStatistics(const secondOrderStatistics& other);

    /**
     * Constructor that set the parameters to \a param.
     *
     * @param param uses these parameters
     */
    secondOrderStatistics(const parameters& param);
    
    /**
     * Destructor
     */
    virtual ~secondOrderStatistics();

    /**
     * Copy data of "other" functor.
     *
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    secondOrderStatistics& copy(const secondOrderStatistics& other);

    /**
     * Alias for copy member
     *
     * \param other the functor to be copied
     * \return a reference to this functor object
     */
    secondOrderStatistics& operator=(const secondOrderStatistics& other);

    /**
     * Returns the name of this class.
     */
    const std::string& name() const;

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual secondOrderStatistics* clone() const;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual secondOrderStatistics* newInstance() const;

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;

    //--------------------------------------------------
    // Apply methods
    //--------------------------------------------------

    /**
     * Computes the \a mean and \a variance of the given vector.
     *
     * @param src vector<T> with the source data.
     * @param mean mean value of \a src elements
     * @param variance variance of the elements of \a src
     * @return true if successful, false otherwise.
     */
    bool apply(const vector<T>& src, U& mean, U& variance) const;
   
    /**
     * Computes the \a mean and \a variance of the elements in the given
     * matrix.
     *
     * @param src matrix<T> with the source data.
     * @param mean mean value of \a src elements
     * @param variance variance of the elements of \a src
     * @return true if successful, false otherwise.
     */
    bool apply(const matrix<T>& src, U& mean, U& variance) const;
    
    /**
     * Computes the mean %vector and each dimension's variance for the
     * data samples given in \a src.
     *
     * The representation of data sample (point) as Rows or Columns of
     * \a src can be chosen via parameters::dataRepresentationType but
     * is usually the default: Rows.
     *
     * In this case each dimension of \a mean and \a variance
     * corresponds to the mean value and variance of the
     * corresponding column of \a src.
     *
     * Depending on the value of parameters::varianceType the
     * Empirical of Maxlikely variances are calculated.
     *
     * @param src matrix<T> with the source data.
     * @param mean mean vector of \a src rows or columns
     * @param variance variances of each sample dimension
     * @return true if successful, false otherwise.
     */
    bool apply(const matrix<T>& src, 
               vector<U>& mean, vector<U>& variance ) const;

    /**
     * Computes the mean %vector and the covariance %matrix for the
     * data samples given in \a src.
     *
     * The representation of data sample (point) as Rows or Columns of
     * \a src can be chosen via parameters::dataRepresentationType but
     * is usually the default: Rows.
     *
     * In this case each dimension of \a mean corresponds to the mean
     * value of the corresponding column of \a src. The \a covMatrix
     * has dimension src.columns() x src.columns().
     *
     * Depending on the value of parameters::varianceType the
     * Empirical of Maxlikely variances are calculated.
     *
     * If parameters::useCorrelation is true the correlation
     * coefficients are calculated instead of the covariances.
     *
     * @param src matrix<T> with the source data.
     * @param mean mean vector of \a src rows or columns
     * @param cov covariance matrix
     * @return true if successful, false otherwise.
     */
    bool apply(const matrix<T>& src, 
               vector<U>& mean, matrix<U>& cov) const;

    //--------------------------------------------------
    // shortcut functions
    //--------------------------------------------------

    /**
     * Computes only the \a mean value of the given \a src %vector.
     *
     * Do not use this function if you also need the variances, as the
     * combined calculation via apply() is much faster.
     *
     * @param src vector<T> with the source data.
     * @param mean mean value of \a src elements
     * @return true if successful, false otherwise.
     */
    bool mean(const vector<T>& src, U& mean) const;

    /**
     * Computes only the \a mean %vector of the given \a src data.
     *
     * Do not use this function if you also need the variances, as the
     * combined calculation via apply() is much faster.
     *
     * Data samples in \a src can either be in Rows (default) or
     * Columns. This can be set in \a dataRep.
     *
     * @param src the source data.
     * @param mean mean vector of \a src
     * @param dataRep data representation, Rows or Columns
     * @return true if successful, false otherwise.
     */
    bool mean(const matrix<T>& src, 
              vector<U>& mean,
              eDataRepresentationType dataRep=Rows) const;

    /**
     * Computes only the \a variance of the given \a src %vector.
     *
     * Do not use this function if you also need the mean value, as
     * the combined calculation via apply() is much faster.
     *
     * @param src vector<T> with the source data.
     * @param variance variance of the elements of \a src
     * @return true if successful, false otherwise.
     */
    bool variance(const vector<T>& src, U& variance) const;

    /**
     * Computes only the \a variance %vector of the given \a src data.
     *
     * Do not use this function if you also need the mean %vector, as
     * the combined calculation via apply() is much faster.
     *
     * Data samples in \a src can either be in Rows (default) or
     * Columns. This can be set in \a dataRep. Other parameters are
     * used as set in this functor's parameters (see apply()).
     *
     * @param src the source data.
     * @param variance variance of each dimension of \a src
     * @param dataRep data representation, Rows or Columns
     * @return true if successful, false otherwise.
     */
    bool variance(const matrix<T>& src, 
                  vector<U>& variance,
                  eDataRepresentationType dataRep=Rows) const;

    /**
     * Computes only the covariance %matrix \a covMat of the given \a
     * src data.
     *
     * Do not use this function if you also need the mean %vector, as
     * the combined calculation via apply() is much faster.
     *
     * Data samples in \a src can either be in Rows (default) or
     * Columns. This can be set in \a dataRep. Other parameters are
     * used as set in this functor's parameters (see apply()).
     *
     * @param src the source data.
     * @param covMat covariance %matrix of \a src
     * @param dataRep data representation, Rows or Columns
     * @return true if successful, false otherwise.
     */
    bool covarianceMatrix(const matrix<T>& src, 
                          matrix<U>& covMat,
                          eDataRepresentationType dataRep=Rows) const;

  protected:

    //--------------------------------------------------
    // helper functions
    //--------------------------------------------------

    /**
     * This function computes the mean and variance of each column of
     * the matrix. I.e. each row is a data point in n-dimensional
     * space).
     *
     * @param src matrix<T> with the source data.
     * @param mu mean row vector;
     * @param var variance of row vectors 
     */
    bool applyRows(const matrix<T>& src, vector<U>& mu, vector<U>& var) const;

    /**
     * This function computes the mean and variance of each row of
     * the matrix. I.e. each column is a data point in n-dimensional
     * space).
     *
     * @param src matrix<T> with the source data.
     * @param mu mean column vector;
     * @param var variance of column vectors 
     */
    bool applyColumns(const matrix<T>& src, 
                      vector<U>& mu, vector<U>& var) const;

    /**
     * This function computes the mean and covariance matrix of the
     * row vectors of the matrix. I.e. each row is a data point in
     * n-dimensional space).
     *
     * @param src matrix<T> with the source data.
     * @param mu mean row vector;
     * @param cov covariance matrix of row vectors 
     */
    bool applyRows(const matrix<T>& src, vector<U>& mu, matrix<U>& cov) const;

    /**
     * This function computes the mean and covariance matrix of the
     * row vectors of the matrix. I.e. each row is a data point in
     * n-dimensional space).
     *
     * @param src matrix<T> with the source data.
     * @param mu mean row vector;
     * @param cov covariance matrix of row vectors 
     */
    bool applyColumns(const matrix<T>& src, 
                      vector<U>& mu, matrix<U>& cov) const;
    
    /**
     * Converts a covariance matrix into a correlation coefficient matrix.
     */
    void covar2corrcoef(matrix<U>& cv) const;

  };
}

#include "ltiSecondOrderStatistics_template.h"

#endif

