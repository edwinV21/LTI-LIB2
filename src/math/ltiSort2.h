/*
 * Copyright (C) 1998-2005
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
 * \file   ltiSort2.h
 *         Sort the elements in a matrix or vector
 * \author Pablo Alvarado
 * \date   17.08.2000
 *
 * $Id: ltiSort2.h,v 1.5 2007-10-09 03:10:50 alvarado Exp $
 */

#ifndef _LTI_SORT2_H_
#define _LTI_SORT2_H_

#include "ltiFunctor.h"
#include "ltiVector.h"
#include "ltiMatrix.h"
#include "ltiPerformanceConfig.h"
#include "ltiSortingOrder.h"

namespace lti {
  
  /**
   * Sort two vectors, using the first one as key.
   *
   * This class is used to sort the elements of two given vectors or
   * matrices.  The first one (of type T) contains always the keys to
   * be used by sorting, and the second (of type U) one will be sorted
   * accordingly to first one. 
   *
   * For example, if you have a second vector of integers, which was
   * initialized with the indices (0 for the first element, 1 for the second
   * and so on), at the end you can use this sorted vector to know which
   * position has an element of the first vector after sorting:
   *
   * \code
   * // the key vector
   * const float fdata[] = {3.2, 1.5, 4.2, 2.0};
   * lti::vector<float> a(4,fdata);
   *
   * // the data vector
   * const int idata[] = {0,1,2,3};
   * lti::vector<int> idx(4,idata);
   *
   * // sorting object:
   * sort2<float,int> sorter;
   *
   * // sort the vector a, and accordingly the vector b
   * sorter.apply(a,b);
   *
   * // after this you will get:
   * // a = 1.5, 2.0, 3.2, 4.2
   * // b = 1  , 3  , 0  , 2
   * \endcode
   *
   * The sort2::parameters inherit from sort::parameters, and therefore you can
   * also here specify the sorting order and the threshold for applying bubble-
   * sort.
   *
   * This functor requires that the type T accept the operator<.
   *
   * @see lti::scramble, lti::sort, lti::quickPartialSort2
   *
   * \ingroup gSorting
   */
  class sort2 : public functor { 
  public:
    /**
     * Type used in the specification of the sorting order
     * (used when sorting the rows or columns of a matrix using as keys
     *  the values of a vector)
     */
    enum eWhichVectors {
      Columns, /*!< sort the columns of the matrix  */
      Rows     /*!< sort the rows of the matrix     */
    };
    
    /**
     * The parameters for the class sort
     */
    class parameters : public functor::parameters {
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
       * Returns the name of this class.
       */
      virtual const std::string& name() const;
      /**
       * Returns a pointer to a clone of the parameters
       */
      virtual parameters* clone() const;

      /**
       * Returns a pointer to a new instance of the parameters
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

      // ---------------------------------------------------------
      //  Parameters
      // ---------------------------------------------------------


      /**
       * Specify if in the apply(vector<T>,matrix<U>) the rows or the columns
       * of the matrix should be sorted.
       * The default value is Rows
       */
      eWhichVectors whichVectors;

      /**
       * Order of the sorting
       * the default value is ascending order
       */
      eSortingOrder sortingOrder;

      /**
       * For vector/matrices with size smaller than this value, a bubble sort
       * will be used (this way is faster)!
       *
       * The default value is usually 10, but if you configured your
       * system for performance this value could change.
       *
       * The best value can be found in the ltiPerformanceConfig.h file,
       * under _LTI_PERFORMANCE_SORT_STOP_QUICKSORT.
       *
       * Default value: 10 or better.
       */
      int thresholdForBubble;

    };


    /**
     * Default constructor.
     * @param sortingOrder sets the sorting order in the parameters. 
     *                     Default is Ascending.
     * @param whichVectors sets the vectors treated in a matrix. 
     *                     Default is Rows
     */
    sort2(const eSortingOrder sortingOrder=Ascending,
          const eWhichVectors whichVectors=Rows);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    sort2(const sort2& other);

    /**
     * Construct with given parameters
     */
    sort2(const parameters& par);

    /**
     * Destructor
     */
    virtual ~sort2();

    /**
     * Sort all the elements of the matrix.  The elements will be
     * ordered row-wise.  For example, the key matrix at the left will
     * be sorted into the matrix at the right side:
     * \code
     *
     *  | 2 8 3 |         | 1 2 3 |
     *  | 1 4 5 |  --->   | 4 5 6 |
     *  | 7 9 6 |         | 7 8 9 |
     *
     * \endcode
     * @param key matrix<T> with the key data.  The result will be
     * left here too.
     * @param srcdest matrix<U> with the data that will be sorted
     * according to the key data
     * @return true if successful, false otherwise
     */
    template <typename T,typename U>
    bool apply(matrix<T>& key,matrix<U>& srcdest) const;

    /**
     * Operates on the given parameter.
     * @param key vector<T> with the key data.  The result
     *                      will be left here too.
     * @param srcdest vector<U> with the data that will be sorted according
     *                          to the key data
     * @return true if successful, false otherwise
     */
    template <typename T,typename U>
    bool apply(vector<T>& key,vector<U>& srcdest) const;

    /**
     * Sort the rows of the matrix, using as key the elements of the given
     * vector.  For example, the matrix at the left side will be sorted in
     * the matrix at the right side with the key-vector (5 2 3):
     *
     * \code
     *
     *  | 2 8 3 |         | 1 4 5 |
     *  | 1 4 5 |  --->   | 7 9 6 |
     *  | 7 9 6 |         | 2 8 3 |
     *
     * \endcode
     *
     * The number of rows of the matrix must be equal to the number of
     * elements in the key vector.
     *
     * @param key vector<T> with the key data.  The result
     *                      will be left here too.
     * @param srcdest matrix<U> with the rows that will be sorted according
     *                          to the key data
     * @return true if successful, false otherwise
     */
    template <typename T,typename U>
    bool apply(vector<T>& key,matrix<U>& srcdest) const;


    /**
     * Sort all the elements of the matrix.  The elements will be
     * ordered row-wise.  For example, the key matrix at the left will
     * be sorted into the matrix at the right side:
     *
     * \code
     *
     *  | 2 8 3 |         | 1 2 3 |
     *  | 1 4 5 |  --->   | 4 5 6 |
     *  | 7 9 6 |         | 7 8 9 |
     *
     * \endcode
     * @param key matrix<T> with the key data.
     * @param src matrix<U> with the source data.
     * @param destkey matrix<T> where the sorted keys will be left.
     * @param dest matrix<U> where the sorted data (using the key) will
     *                       be left.
     * @return true if successful, false otherwise
     */
    template <typename T,typename U>
    bool apply(const matrix<T>& key, const matrix<U>& src,
                     matrix<T>& destkey,matrix<U>& dest) const;

    /**
     * Operates on a copy of the given parameters.
     * @param key vector<T> with the key data.
     * @param src vector<U> with the source data.
     * @param destkey vector<T> where the sorted keys will be left.
     * @param dest vector<U> where the sorted data (using the key) will
     *                       be left.
     * @return true if successful, false otherwise
     */
    template <typename T,typename U>
    bool apply(const vector<T>& key,const vector<U>& src,
                     vector<T>& destkey, vector<U>& dest) const;

    /**
     * Sort the rows of the matrix, using as key the elements of the given
     * vector.  For example, the matrix at the left side will be sorted in
     * the matrix at the right side with the key-vector (5 2 3):
     *
     * \code
     *
     *  | 2 8 3 |         | 1 4 5 |
     *  | 1 4 5 |  --->   | 7 9 6 |
     *  | 7 9 6 |         | 2 8 3 |
     *
     * \endcode
     *
     * The number of rows of the matrix must be equal to the number of
     * elements in the key vector.
     *
     * @param key vector<T> with the key data.
     * @param src matrix<U> with the rows that will be sorted according
     *                          to the key data
     * @param destkey the sorted key-vector
     * @param dest the matrix with sorted rows.
     * @return true if successful, false otherwise
     */
    template <typename T,typename U>
    bool apply(const vector<T>& key,const matrix<U>& src,
                     vector<T>& destkey, matrix<U>& dest) const;

    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    sort2& copy(const sort2& other);

    /**
     * Returns the name of this class.
     */
    const std::string& name() const;

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual sort2* clone() const;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual sort2* newInstance() const;

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;


  protected:
    
    template <typename T,typename U>
    void quicksort(vector<T>& vct,vector<U>& vct2,
                   const int begin,
                   const int end) const;

    template <typename T,typename U>
    int partitionAsc(vector<T>& vct,vector<U>& vct2,
                     const int begin,
                     const int end) const;

    template <typename T,typename U>
    void insertionsortAsc(vector<T>& vct,vector<U>& vct2,
                          const int begin,
                          const int end) const;

    template <typename T,typename U>
    int partitionDesc(vector<T>& vct,vector<U>& vct2,
                      const int begin,
                      const int end) const;

    template <typename T,typename U>
    void insertionsortDesc(      vector<T>& vct,
                                 vector<U>& vct2,
                           const int begin,
                           const int end) const;

    template <typename U>
    void reorder(const ivector& indices,
                 const matrix<U>& src,
                       matrix<U>& dest) const;
  
  };
  
}

#include "ltiSort2_template.h"

#endif
