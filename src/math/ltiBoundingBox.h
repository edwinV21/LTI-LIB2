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
 * \file   ltiBoundingBox.h
 * \author Jochen Wickel
 * \date   25.9.2000
 *
 * $Id: ltiBoundingBox.h,v 1.1 2005-07-22 17:52:25 gquiros Exp $
 */

#ifndef _LTI_BOUNDING_BOX_H_
#define _LTI_BOUNDING_BOX_H_

#include "ltiFunctor.h"
#include "ltiVector.h"
#include "ltiMatrix.h"
#include "ltiDataRepresentationType.h"

// because of a stupid implementation of the C library in Visual C++, we
// have to undefine min and max
#ifdef _LTI_MSC_6
#undef min
#undef max
#endif

namespace lti {
  /**
   * Boudaries of a hyperbox enclosing all points of a n-dimensional space.

   * This functor computes for each "dimension" of points in a n-dimensional
   * space the extrema (i.e. minimum and maximum values).  This way,
   * it computes the boundaries of a hyperbox where all points are contained.
   *
   * The points are given in form of vectors.  If a matrix is given, the
   * parameter parameters::rowWise indicates if the vectors are taken from
   * the rows (true) or from the columns (false) of the matrix.
   *
   * Several methods besides the standard apply() are provided in order to
   * \e clip points into a given hyperbox.
   */
  template <typename T> 
  class boundingBox : public functor {
  public:
    /**
     * the parameters for the class boundsFunctor
     */
    class parameters : public functor::parameters {
    public:
      /**
       * default constructor
       */
      parameters();

      /**
       * copy constructor
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
       * returns a pointer to a clone of the parameters
       */
      virtual parameters* clone() const;

      /**
       * returns a pointer to a new instance of the parameters
       */
      virtual parameters* newInstance() const;

      /**
       * The LTI-Lib usually represents a data sample as a row of a
       * matrix. However, by choosing Columns here, you can achieve
       * the opposite behavior. This is not recommended since it is
       * confusing and much slower!
       *
       * Default value: Rows
       */
      eDataRepresentationType dataRepresentationType;

    };

    /**
     * default constructor
     */
    boundingBox();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    boundingBox(const boundingBox& other);

    /**
     * destructor
     */
    virtual ~boundingBox();

    /**
     * returns the name of this type
     */
    virtual const std::string& name() const;

    /**
     * The result of this function depends on the value of
     * parameters.dataRepresentationType. If this parameter is Rows, the functor
     * will compute a vector, whose elements contain each the mean
     * of one column of the matrix (the computes the mean of the rows,
     * where each row is a data point in n-dimensional space.
     * If the value is Columns, the result vector contains the mean
     * of the columns of the matrix (each column a data point).
     * @param src matrix<T> with the source data.
     * @param min vector<T> where the minimum vector of the matrix will be
     *                      left
     * @param max vector<T> where the maximum vector of the matrix will be
     *                      left
     */
    void apply(const matrix<T>& src, vector<T>& min, vector<T>& max) const;

    /**
     * This function will compute a vector, whose elements contain
     * each the minimum of one column of the matrix (this computes the
     * n-dimensional equivalent of a lower-left corner of the bounding
     * box of a data set, where each row is a data point in
     * n-dimensional space).
     * @param src matrix<T> with the source data.
     * @param dest matrix<T> where the result will be left.  */
    void min(const matrix<T>& src, vector<T>& dest) const;

    /**
     * Computes a vector which contains the minimum elements of a and b
     * @param a vector<T> first operand
     * @param b vector<T> second operand
     * @param dest vector<T> which will receive the element-wise minimum
     *          of both operands
     */
    void min(const vector<T> &a, const vector<T> &b,
             vector<T>& dest) const;
 
    /**
     * This function
     * will compute a vector, whose elements contain each the maximum
     * of one column of the matrix (this computes the n-dimensional
     * equivalent of an upper-right corner of the bounding box of
     * a data set, where each row is a data point in n-dimensional space).
     * @param src matrix<T> with the source data.
     * @param dest matrix<T> where the result will be left.
     */
    void max(const matrix<T>& src, vector<T>& dest) const;   
   
    /**
     * Computes a vector which contains the maximum elements of a and b
     * @param a vector<T> first operand
     * @param b vector<T> second operand
     * @param dest vector<T> which will receive the element-wise maximum
     *          of both operands
     */
    void max(const vector<T> &a, const vector<T> &b,
             vector<T>& dest) const;
    
    /**
     * Clips the values of the vector a to lowerBounds and upperBounds.
     * For each vector element a[i],
     * if lowerBounds[i] <= a[i] <= higherBounds[i],
     * the value of a[i] remains unchanged. If a[i] < lowerBounds[i],
     * a[i] will be set to lowerBounds[i]. Analagous with upperBounds.
     * @param a vector<T> to be clipped.
     * @param lowerBounds vector<T> which contains the smallest permitted
     *                    elements.
     * @param upperBounds vector<T> which contains the largest permitted
     *                    elements.
     */
    void clip(vector<T> &a,
              const vector<T>& lowerBounds,
              const vector<T>& upperBounds) const;

    /**
     * Clips the values of the matrix a to lowerBounds and upperBounds.
     * For each matrix element a[i][j],
     * if lowerBounds[i][j] <= a[i][j] <= higherBounds[i][j],
     * the value of a[i][j] remains unchanged. If a[i][j] < lowerBounds[i][j],
     * a[i][j] will be set to lowerBounds[i][j]. Analagous with upperBounds.
     * @param a matrix<T> to be clipped.
     * @param lowerBounds matrix<T> which contains the smallest permitted
     *                    elements.
     * @param upperBounds matrix<T> which contains the largest permitted
     *                    elements.
     */
    void clip(matrix<T> &a,
              const matrix<T>& lowerBounds,
              const matrix<T>& upperBounds) const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    boundingBox& copy(const boundingBox& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual boundingBox* clone() const;

    /**
     * returns a pointer to a new instance of this functor.
     */
    virtual boundingBox* newInstance() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

  private:

    /**
     * This function will compute a vector, whose elements contain
     * each the minimum of one column of the matrix (this computes the
     * n-dimensional equivalent of a lower-left corner of the bounding
     * box of a data set, where each row is a data point in
     * n-dimensional space).
     * @param src matrix<T> with the source data.
     * @param dest matrix<T> where the result will be left.  */
    void minOfRows(const matrix<T>& src, vector<T>& dest) const;

    /**
     * This function
     * will compute a vector, whose elements contain each the maximum
     * of one column of the matrix (this computes the n-dimensional
     * equivalent of an upper-right corner of the bounding box of
     * a data set, where each row is a data point in n-dimensional space).
     * @param src matrix<T> with the source data.
     * @param dest matrix<T> where the result will be left.
     */
    void maxOfRows(const matrix<T>& src, vector<T>& dest) const;

    /**
     * This function
     * will compute a vector, whose elements contain each the minimum
     * of one row of the matrix (this computes the n-dimensional
     * equivalent of a lower-left corner of the bounding box of
     * a data set, where each column is a data point in n-dimensional space).
     * @param src matrix<T> with the source data.
     * @param dest matrix<T> where the result will be left.
     */
    void minOfColumns(const matrix<T>& src, vector<T>& dest) const;

    /**
     * This function
     * will compute a vector, whose elements contain each the maximum
     * of one row of the matrix (this computes the n-dimensional
     * equivalent of an upper-right corner of the bounding box of
     * a data set, where each column is a data point in n-dimensional space).
     * @param src matrix<T> with the source data.
     * @param dest matrix<T> where the result will be left.
     */
    void maxOfColumns(const matrix<T>& src, vector<T>& dest) const;

    /**
     * This function is a combination of minOfRows and maxOfRows
     * @param src matrix<T> with the source data.
     * @param min vector<T> where the minimum will be left.
     * @param max vector<T> where the maximum will be left.
     */
    void boundsOfRows(const matrix<T>& src, vector<T>& min,
                      vector<T>& max) const;

    /**
     * This function is a combination of minOfColumns and maxOfColumns
     * @param src matrix<T> with the source data.
     * @param min vector<T> where the minimum will be left.
     * @param max vector<T> where the maximum will be left.
     */
    void boundsOfColumns(const matrix<T>& src,
                         vector<T>& min, vector<T>& max) const;

  };
}

#include "ltiBoundingBox_template.h"

#endif
