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
 * \file   ltiSort.h
 *         Sort the elements in a matrix or vector
 * \author Pablo Alvarado
 * \date   17.08.2000
 *
 * $Id: ltiSort.h,v 1.10 2007-10-09 03:10:51 alvarado Exp $
 */

#ifndef _LTI_SORT_H_
#define _LTI_SORT_H_

#include "ltiSortingOrder.h"
#include "ltiFunctor.h"
#include "ltiVector.h"
#include "ltiMatrix.h"
#include "ltiPerformanceConfig.h"

namespace lti {

  /**
   * Sort vectors.
   *
   * This class is used to sort the elements of a given vector or matrix.
   *
   * The sort::parameters::order specify if the elements should be sorted in
   * ascending or descending order.
   *
   * This functor requires that the type T accept the operator<.
   *
   * @see lti::scramble, lti::sort2, lti::quickPartialSort
   *
   * The functor uses the well-known quick-sort algorithm to sort the elements
   * of the vector.  Since the overhead of the recursion makes at some point
   * the quick-sort more innefficient than simpler algorithms, you can specify
   * in the parameters for which size the vectors should be sorted with 
   * the bubble-sort algorithm.
   *
   * The quick-sort is not "stable", this means that elements with the same
   * key value can change their positions in the vector.
   *
   * You should also revise the STL algorithms std::sort() if you are using 
   * containers of the STL.
   *
   * \ingroup gSorting
   */
  class sort : public functor {
  public:

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
      const std::string& name() const;

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

      /**
       * Order of the sorting
       * 
       * Default: Ascending
       */
      eSortingOrder sortingOrder;
    };

    /**
     * Default constructor
     */
    sort(const eSortingOrder sortingOrder=Ascending);

    /**
     * Construct with given parameters
     */
    sort(const parameters& par);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    sort(const sort& other);

    /**
     * Destructor
     */
    virtual ~sort();

    /**
     * Sort all the elements of the matrix.  The elements will be
     * ordered row-wise.  For example, the matrix at the left will
     * be sorted into the matrix at the right side:
     * \code
     *
     *  | 2 8 3 |         | 1 2 3 |
     *  | 1 4 5 |  --->   | 4 5 6 |
     *  | 7 9 6 |         | 7 8 9 |
     *
     * \endcode
     * @param srcdest matrix<T> with the source data.  The result
     *                 will be left here too.
     * @return true if successful, false otherwise
     */
    template <class T>
    bool apply(matrix<T>& srcdest) const;

    /**
     * Operates on the given parameter.
     * @param srcdest vector<T> with the source data.  The result
     *                 will be left here too.
     * @return true if successful, false otherwise
     */
    template <class T>
    bool apply(vector<T>& srcdest) const;

    /**
     * Sort all the elements of the matrix.  The elements will be
     * ordered row-wise.  For example, the matrix at the left will
     * be sorted into the matrix at the right side:
     * \code
     *
     *  | 2 8 3 |         | 1 2 3 |
     *  | 1 4 5 |  --->   | 4 5 6 |
     *  | 7 9 6 |         | 7 8 9 |
     *
     * \endcode
     * @param src matrix<T> with the source data.
     * @param dest matrix<T> where the result will be left.
     * @return true if successful, false otherwise
     */
    template <class T>
    bool apply(const matrix<T>& src,matrix<T>& dest) const;

    /**
     * Operates on a copy of the given parameters.
     * @param src vector<T> with the source data.
     * @param dest vector<T> where the result will be left.
     * @return true if successful, false otherwise
     */
    template <class T>
    bool apply(const vector<T>& src,vector<T>& dest) const;

    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    sort& copy(const sort& other);

    /**
     * Returns the name of this class.
     */
    virtual const std::string& name() const;

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual sort* clone() const;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual sort* newInstance() const;
    
    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * Set parameters
     */
    bool updateParameters();
    
  private:
    /**
     * Quick sort entry point
     */
    template <class T>
    void quicksort(vector<T>& vct,
                   const int begin,
                   const int end) const;
    /**
     * Partition vector for ascending order
     */
    template <class T>
    int partitionAsc(vector<T>& vct,
                     const int begin,
                     const int end) const;
    /**
     * Insertion sort for ascending order
     */
    template <class T>
    void insertionsortAsc(vector<T>& vct,
                          const int begin,
                          const int end) const;
    /**
     * Partition vector for descending order
     */
    template <class T>
    int partitionDesc(vector<T>& vct,
                      const int begin,
                      const int end) const;
    /**
     * Insertion sort for descending order
     */
    template <class T>
    void insertionsortDesc(vector<T>& vct,
                           const int begin,
                           const int end) const;

    /**
     * @name Shadows of the parameters to avoid a function access
     */
    //@{
    /**
     * Threshold for bubble sort
     */
    int thresholdForBubble_;
    
    /**
     * Sorting order
     */
    eSortingOrder order_;
    //@}
    
  };

}

#include "ltiSort_template.h"

#endif
