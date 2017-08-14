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
 * \file   ltiQuickMedian2.h
 *         This file contains the functor quickMedian, which
 *         calculates the median quickly
 * \author Guy Wafo Moudhe
 * \date   27.03.2001
 *
 * $Id: ltiQuickMedian2.h,v 1.4 2012-09-16 05:22:59 alvarado Exp $
 */

#ifndef _LTI_QUICKMEDIAN2_H_
#define _LTI_QUICKMEDIAN2_H_

#include "ltiFunctor.h"
#include "ltiVector.h"
#include "ltiMatrix.h"
#include "ltiQuickMedian.h"

namespace lti {


  /**
   * Quick median for two vectors.
   *
   * This class is used to extract the median of the elements of a
   * given vector or matrix, partitioning at the same time a second
   * vector.  The median is defined as the element at the middle
   * position of the sorted vector.  The algorithm used is based on
   * the quick sort.
   *
   * The difference with the lti::quickMedian functor is that you can
   * "sort" a second vector, which might contain for example the indices
   * of the elements.  This way, you can easily find out, which elements of the
   * original vector are under the median, and which ones above.
   *
   * @see lti::quickMedian, lti::sort, lti::sort2
   *
   * For vectors (or matrices) with an even number n of elements, the
   * median will be the element at (n/2) or (n/2)-1 depending on the
   * parameter settings.
   *
   * The type of the vector elements (T) must accept the operators <
   * and >.
   *
   * @ingroup gStatistics
   */
  class quickMedian2 : public functor {
  public:
    
    typedef quickMedian::parameters parameters;

    /**
     * Default constructor
     */
    quickMedian2();

    /**
     * Constructor to set parameters
     */
    quickMedian2(const parameters& param);

    /**
     * Constructor with indicator what to do for even-sized vectors
     */
    quickMedian2(const eMedianEvenCase medianEvenCase);

    /**
     * copy constructor
     * @param other the object to be copied
     */
    quickMedian2(const quickMedian2& other);

    /**
     * Destructor
     */
    virtual ~quickMedian2();

    /**
     * Returns the name of this class.
     */
    const std::string& name() const;

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual quickMedian2* clone() const;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual quickMedian2* newInstance() const;

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * Operates on the given arguments.
     *
     * Please note that the arguments will be both modified.
     *
     * The resulting keys vector contains the elements less or equal than the
     * median for the indexes <code>x</code> such that 
     * <code>x < size()/2</code>, and higher or equal otherwise.
     *
     * Both vectors must have the same size.
     *
     * The types V and W, can be an lti::genericVector
     * or its derived classes, or a std::vector.  In principle the container
     * type V,W just needs to support:
     * - the type definitions V::size_type and V::value_type, 
     * - the operator[] returning elements of type V::value_type
     * - the V::value_type has to be comparable with the operator<.
     * - the method empty()
     * - the method size() returning a V::size_type
     *
     * @param keys vector with the key data.  The median of this data
     *             is partially sorted while looking for the median.
     * @param data vector with data to be sorted the same way as the keys.
     *             You can for example use a ivector initialized with the
     *             index values (i.e. data(i)=i), so that after the apply
     *             method you can check which elements are below the median
     *             and which above.
     * @param median the median value of keys.
     * @return bool upon success, false otherwise
     */
    template<class V,class W>
    bool apply(V& keys,W& data, typename V::value_type& median) const;

    /**
     * Operates on the given arguments.
     *
     * Please note that the arguments will be both modified.
     *
     * The resulting keys vector contains the elements less or equal than the
     * median for the indexes <code>x</code> such that 
     * <code>x < size()/2</code>, and higher or equal otherwise.
     *
     * Both vectors must have the same size.
     *
     * @param keys V with the key data.  The median of this data
     *             is partially sorted while looking for the median.
     * @param data W with data to be sorted the same way as the keys.
     *             You can for example use a ivector initialized with the
     *             index values (i.e. data(i)=i), so that after the apply
     *             method you can check which elements are below the median
     *             and which above.
     * @return bool upon success, false otherwise
     */
    template<class V,class W>
    bool apply(V& keys,W& data) const;


  protected:
    /**
     * this method calculates the median in a recursively form
     */
    template<class V,class W>
    typename V::value_type findMedian(V& vct,
                                      W& data,
                                      const int begin,
                                      const int end,
                                      const int medianPos) const;
    
    /**
     * this method chooses a pivot-value and ensures that lower values lie
     * at the left and higher values at the right of its final position, which
     * will be returned.
     */
    template<class V,class W>
    int partition(V& vct,
                  W& data,
                  const int begin,
                  const int end) const;
  };

}

#include "ltiQuickMedian2_template.h"

#endif
