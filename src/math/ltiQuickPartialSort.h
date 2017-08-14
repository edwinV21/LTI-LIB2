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
 * \file   ltiQuickPartialSort.h
 *         This file contains the functor quickPartialSort, which
 *         calculates the median quickly
 * \author Guy Wafo Moudhe
 * \date   27.03.2001
 *
 * $Id: ltiQuickPartialSort.h,v 1.3 2012-09-16 05:22:59 alvarado Exp $
 */

#ifndef _LTI_QUICK_PARTIAL_SORT_H_
#define _LTI_QUICK_PARTIAL_SORT_H_

#include "ltiFunctor.h"
#include "ltiVector.h"
#include "ltiMatrix.h"

namespace lti {

  /**
   * Quick partial sort.
   *
   * This class is used to find the n-th element of an ascending sorted vector
   * or matrix, while avoiding to sort the complete container (that is the
   * reason for the "partial" part in the class name). The algorithm used is
   * based on the quick sort.
   *
   * On-place applies are in this implementation faster than on copy ones.
   *
   * Note that this class does not has a nested parameters class, since they
   * are not necessary.
   *
   * \ingroup gSorting
   */
  class quickPartialSort : public functor {
  public:
    /**
     * Default constructor
     */
    quickPartialSort();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    quickPartialSort(const quickPartialSort& other);

    /**
     * destructor
     */
    virtual ~quickPartialSort();

    /**
     * Returns the name of this class.
     */
    const std::string& name() const;

    /**
     * Calculates which element of the sorted matrix would be at(row,col) if
     * the whole matrix would be sorted.  The given matrix will be modified so
     * that all elements "before" the given coordinates are less than or equal
     * the element at(row,col), and the elements "after" will be greater. 
     * 
     * The terms "before" and "after" are stated in the context of
     * concatenation of all rows of the matrix into a large vector.
     *
     * @param row row index for the element that has to be found.
     * @param col column index for the element that has to be found.
     * @param src matrix<T> with the source data.
     * @param data the data value of the given matrix.
     * @return \c true on success \c false otherwise
     */
    template <typename T>
    bool apply(const int row, const int col,
               matrix<T>& src, T& data) const;

    /**
     * Calculates the data of the given matrix, which is NOT modified.
     * The elements of the matrix will be considered as part of a vector
     * with "columns()" times "rows()" elements.
     *
     * This method needs to save the contents of the src data, and therefore is
     * not as fast as the one with interface 
     * apply(const int,const int,matrix<T>&,T&).
     * 
     * @param row row index for the element that has to be found.
     * @param col column index for the element that has to be found.
     * @param src matrix<T> with the source data.
     * @param data the data value of the given matrix.
     * @return \c true on success \c false otherwise
     */
    template <typename T>
    bool apply(const int row, const int col,
               const matrix<T>& src, T& data) const;

    /**
     * Calculates the data of \a src, the result is left in \a dest.
     * The elements of the matrix will be considered as part of a vector
     * with "columns()" times "rows()" elements.
     *
     * This method needs to transfer the contents of the src data into the dest
     * matrix, and therefore is not as fast as the one with interface
     * apply(const int,const int,matrix<T>&,T&).
     *
     * @param row row index for the element that has to be found.
     * @param col column index for the element that has to be found.
     * @param src matrix<T> with the source data.
     * @param dest partially sorted matrix.
     * @param data the data value of the given matrix.
     * @return true on success false otherwise
     */
    template <typename T>
    bool apply(const int row, const int col,
               const matrix<T>& src, matrix<T>& dest, T& data) const;

    /**
     * Find the n-th element of a sorted vector of type V, which can be an
     * lti::genericVector or any of its derived classes, or a std::vector.  In
     * principle the container type V just needs to support:
     * - the type definitions V::size_type and V::value_type, 
     * - the operator[] returning elements of type V::value_type 
     * - the V::value_type has to be comparable with the operator<.
     * - the method empty() 
     * - the method size() returning a V::size_type
     *
     * The resulting vector contains the elements less or equal than the data
     * for the indexes <code>x</code> such that <code>x < size()/2</code>,
     * and higher or equal otherwise.
     *
     * @param pos the position of the element that wants to be found.
     * @param srcdest vector with the source data.  The resultwill be left 
     *                here too.
     * @param data    the data value
     *
     * @return true on success false otherwise
     */
    template <class V>
    bool apply(const int pos,V& srcdest, typename V::value_type& data) const;

    /**
     * Operates on the given parameter.
     *
     * @param pos the position of the element that wants to be found.
     * @param src vector with the source data.
     * @param data the data value
     * @return true on success false otherwise
     */
    template <class V>
    bool apply(const int pos,const V& src, typename V::value_type& data) const;

    /**
     * Operates on the given parameter.
     *
     * @param pos the position of the element that wants to be found.
     * @param src vector with the source data.
     * @param dest the partially sorted vector.  The elements at the
     *             first half of the vector are less or equal than the data
     *             and on the other half greater or equal.
     * @param data  the data value
     * @return true on success false otherwise
     */
    template <class V>
    bool apply(const int pos,const V& src, V& dest,
               typename V::value_type& data) const;

    /**
     * A shortcut function for apply(const vector<T>&, T&) const. Note
     * that internally another vector and T are used.
     *
     * @param pos the position of the element that wants to be found.
     * @param src vector whose data is sought
     * @returns the data of \a src
     */
    template <class V>
    typename V::value_type nth(const int pos,const V& src) const;

    /**
     * Copy data of "other" functor.
     *
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    quickPartialSort& copy(const quickPartialSort& other);

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual quickPartialSort* clone() const;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual quickPartialSort* newInstance() const;

  protected:
    /**
     * This method calculates the data in a recursively form.
     * The template type V has to be a vector following an interface like
     * the lti::vector or the std::vector, implementing the operator[]
     */
    template <class V>
    typename V::value_type findNth(V& vct,
                                   const int begin,
                                   const int end,
                                   const int dataPos) const;
    
    /**
     * This method chooses a pivot-value and ensures that lower values lie
     * at the left and higher values at the right of its final position, which
     * will be returned.
     * The template type V has to be a vector following an interface like
     * the lti::vector or the std::vector, implementing the operator[]
     */
    template <class V>
    int partition(V& vct,
                  const int begin,
                  const int end) const;

  };

}


#include "ltiQuickPartialSort_template.h"

#endif


