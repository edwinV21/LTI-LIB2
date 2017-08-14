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
 * \file   ltiQuickMedian.h
 *         This file contains the functor quickMedian, which
 *         calculates the median quickly
 * \author Guy Wafo Moudhe
 * \date   27.03.2001
 *
 * $Id: ltiQuickMedian.h,v 1.7 2012-09-16 05:22:59 alvarado Exp $
 */

#ifndef _LTI_QUICKMEDIAN_H_
#define _LTI_QUICKMEDIAN_H_

#include "ltiFunctor.h"
#include "ltiGenericVector.h"
#include "ltiGenericMatrix.h"
#include "ltiMedianEvenCase.h"

#include <vector>

namespace lti {

  /**
   * Quick median search.
   *
   * This class is used to extract the median of the elements of a
   * given vector or matrix.  The median is defined as the element at
   * the middle position of the sorted vector.  The algorithm used is
   * based on the quick sort.
   * For vectors (or matrices) with an even number n of elements, the
   * median will be the element at (n/2) or (n/2)-1 depending on the
   * parameter settings.
   *
   * On-place applies are in this implementation faster than on copy ones.
   *
   * The type of the vector elements (T) must accept the operators \<
   * and \>.
   *
   * @ingroup gStatistics
   */
  class quickMedian : public functor {
  public:
    
    /**
     * The parameters for the class quickMedian
     */
    class parameters : public functor::parameters {
    public:
      /**
       * Default constructor
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
       * Returns the name of this class.
       */
      const std::string& name() const;

      /**
       * copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& copy(const parameters& other);

      /**
       * Alias for copy member
       * @param other the functor to be copied
       * @return a reference to this functor object
       */
      parameters& operator=(const parameters& other);

      /**
       * returns a pointer to a clone of the parameters
       */
      virtual parameters* clone() const;

      /**
       * returns a pointer to a new instance of the parameters
       */
      virtual parameters* newInstance() const;

      /**
       * Write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool write(ioHandler& handler,const bool complete=true) const;

      /**
       * Read the parameters from the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool read(ioHandler& handler,const bool complete=true);

      // ------------------------------------------------
      // the parameters
      // ------------------------------------------------

      /**
       * Specifies how to consider the case when the number of elements of
       * the vector is even (see lti::eMedianEvenCase)
       *
       * This parameter has no effect for odd-sized vectors.
       *
       * Default value: TakeLower
       */
      eMedianEvenCase medianEvenCase;
    };

    /**
     * Default constructor
     */
    quickMedian();

    /**
     * Constructor to set parameters
     */
    quickMedian(const parameters& param);

    /**
     * Constructor with indicator what to do for even-sized vectors
     */
    quickMedian(const eMedianEvenCase medianEvenCase);

    /**
     * copy constructor
     * @param other the object to be copied
     */
    quickMedian(const quickMedian& other);

    /**
     * destructor
     */
    virtual ~quickMedian();

    /**
     * Returns the name of this class.
     */
    const std::string& name() const;

    /**
     * Calculates the median of the given matrix, which WILL BE modified.
     * The elements of the matrix will be considered as part of a vector
     * with "columns()" times "rows()" elements.
     *
     * @param src matrix<T> with the source data.
     * @param median the median value of the given matrix.
     * @return \c true on success \c false otherwise
     */
    template <typename T>
    bool apply(genericMatrix<T>& src, T& median) const;

    /**
     * Calculates the median of the given matrix, which is NOT modified.
     * The elements of the matrix will be considered as part of a vector
     * with "columns()" times "rows()" elements.
     *
     * @param src matrix<T> with the source data.
     * @param median the median value of the given matrix.
     * @return \c true on success \c false otherwise
     */
    template <typename T>
    bool apply(const genericMatrix<T>& src, T& median) const;

    /**
     * Calculates the median of \a src, the result is left in \a dest.
     * The elements of the matrix will be considered as part of a vector
     * with "columns()" times "rows()" elements.
     *
     * @param src matrix<T> with the source data.
     * @param dest partially sorted matrix.
     * @param median the median value of the given matrix.
     * @return true on success false otherwise
     */
    template <typename T>
    bool apply(const genericMatrix<T>& src,
               genericMatrix<T>& dest, T& median) const;

    /**
     * Find the median of a vector of T, which can be an lti::genericVector
     * or its derived classes.
     *
     * The resulting vector contains the elements less or equal than the median
     * for the indexes <code>x</code> such that <code>x < size()/2</code>,
     * and higher or equal otherwise.
     *
     * @param srcdest vector<T> with the source data.  The result
     *                 will be left here too.
     * @param median  the median value
     *
     * @return true on success false otherwise
     */
    template <class T>
    bool apply(genericVector<T>& srcdest, T& median) const;

    /**
     * Operates on the given parameter.
     * @param src vector with the source data.
     * @param median the median value
     * @return true on success false otherwise
     */
    template <class T>
    bool apply(const genericVector<T>& src, T& median) const;

    /**
     * Operates on the given parameter.
     * @param src vector with the source data.
     * @param dest the partially sorted vector.  The elements at the
     *             first half of the vector are less or equal than the median
     *             and on the other half greater or equal.
     * @param median  the median value
     * @return true on success false otherwise
     */
    template <class T>
    bool apply(const genericVector<T>& src, genericVector<T>& dest,
               T& median) const;

    /**
     * A shortcut function for apply(const vector<T>&, T&) const. Note
     * that internally another vector and T are used.
     *
     * @param src vector whose median is sought
     * @returns the median of \a src
     */
    template <class T>
    T median(const genericVector<T>& src) const;

    /**
     * Find the median of a STL vector of T.
     *
     * The resulting vector contains the elements less or equal than the median
     * for the indexes <code>x</code> such that <code>x < size()/2</code>,
     * and higher or equal otherwise.
     *
     * @param srcdest vector<T> with the source data.  The result
     *                 will be left here too.
     * @param median  the median value
     *
     * @return true on success false otherwise
     */
    template <class T>
    bool apply(std::vector<T>& srcdest, T& median) const;

    /**
     * Operates on the given parameter.
     * @param src vector with the source data.
     * @param median the median value
     * @return true on success false otherwise
     */
    template <class T>
    bool apply(const std::vector<T>& src, T& median) const;

    /**
     * Operates on the given parameter.
     * @param src vector with the source data.
     * @param dest the partially sorted vector.  The elements at the
     *             first half of the vector are less or equal than the median
     *             and on the other half greater or equal.
     * @param median  the median value
     * @return true on success false otherwise
     */
    template <class T>
    bool apply(const std::vector<T>& src, std::vector<T>& dest,T& median) const;

    /**
     * A shortcut function for apply(const vector<T>&, T&) const. Note
     * that internally another vector and T are used.
     *
     * @param src vector whose median is sought
     * @returns the median of \a src
     */
    template <class T>
    T median(const std::vector<T>& src) const;

    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    quickMedian& copy(const quickMedian& other);

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual quickMedian* clone() const;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual quickMedian* newInstance() const;

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;

  protected:
    /**
     * Find the median of a vector type V, which can be an lti::genericVector
     * or its derived classes, or a std::vector.  In principle the container
     * type V just needs to support:
     * - the type definitions V::size_type and V::value_type, 
     * - the operator[] returning elements of type V::value_type
     * - the V::value_type has to be comparable with the operator<.
     * - the method empty()
     * - the method size() returning a V::size_type
     *
     * The resulting vector contains the elements less or equal than the median
     * for the indexes <code>x</code> such that <code>x < size()/2</code>,
     * and higher or equal otherwise.
     *
     * @param srcdest vector<T> with the source data.  The result
     *                 will be left here too.
     * @param median  the median value
     *
     * @return true on success false otherwise
     */
    template <class V>
    bool applyVector(V& srcdest, typename V::value_type& median) const;

    /**
     * Operates on the given parameter.
     * @param src vector with the source data.
     * @param median the median value
     * @return true on success false otherwise
     */
    template <class V>
    bool applyVector(const V& src, typename V::value_type& median) const;

    /**
     * Operates on the given parameter.
     * @param src vector with the source data.
     * @param dest the partially sorted vector.  The elements at the
     *             first half of the vector are less or equal than the median
     *             and on the other half greater or equal.
     * @param median  the median value
     * @return true on success false otherwise
     */
    template <class V>
    bool applyVector(const V& src, V& dest,
                     typename V::value_type& median) const;

    /**
     * A shortcut function for apply(const vector<T>&, T&) const. Note
     * that internally another vector and T are used.
     *
     * @param src vector whose median is sought
     * @returns the median of \a src
     */
    template <class V>
    typename V::value_type medianVector(const V& src) const;

    /**
     * This method calculates the median in a recursively form.
     * The template type V has to be a vector following an interface like
     * the lti::vector or the std::vector, implementing the operator[]
     */
    template <class V>
    typename V::value_type findMedian(V& vct,
                                      const int begin,
                                      const int end,
                                      const int medianPos) const;
    
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


#include "ltiQuickMedian_template.h"

#endif


