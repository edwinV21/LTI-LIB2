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
 * \file   ltiPyramid.h
 *         Contains the container pyramid, which is a pyramid of matrices
 * \author Pablo Alvarado
 * \date   30.10.2000
 *
 * $Id: ltiPyramid.h,v 1.3 2005-03-16 16:20:49 doerfler Exp $
 */

#ifndef _LTI_PYRAMID_H_
#define _LTI_PYRAMID_H_

#include "ltiContainer.h"
#include <vector>

namespace lti {
  /**
   * Pyramid class.
   *
   * The lti::pyramid class is the parent class for the multiresolutional
   * pyramids.  For example lti::gaussianPyramid
   *
   * It contains some objects of type T (e.g. channel, channel8 or image),
   * which must implement the type T::size_type and the member size().  Note
   * that pyramids of pyramids are allowed.
   *
   * @ingroup gAggregate, gImageProcessing
   */
  template <class T>
  class pyramid : public container {
  public:

    /**
     * This iterator iterates over the levels of the pyramid. It is
     * equivalent to a std::vector<T>::iterator
     */
    typedef typename std::vector<T>::iterator iterator;

    /**
     * This iterator iterates over the levels of the pyramid. It is
     * equivalent to a std::vector<T>::const_iterator
     */
    typedef typename std::vector<T>::const_iterator const_iterator;

    /**
     * Default constructor creates an empty pyramid;
     */
    pyramid();

    /**
     * Create a pyramid with the given number of levels which
     * correspond to resolutions or scales
     *
     * @param levels the number of resolutions or scales in the pyramid
     */
    pyramid(const int levels);

    /**
     * Create this pyramid as a copy of another pyramid
     * @param other the pyramid to be copied.
     */
    pyramid(const pyramid& other);

    /**
     * Destructor
     */
    virtual ~pyramid();

    /**
     * Returns the name of this type.
     */
    virtual const std::string& name() const;

    /**
     * The return type of the size() member
     */
    typedef int size_type;

    /**
     * Returns the number of resolutions of this pyramid
     */
    inline size_type size() const;

    /**
     * Returns true if the size of this pyramid is zero.
     */
    inline bool empty() const;

    /**
     * Returns an iterator pointing to the first element, i.e. the
     * lowest level of the pyramid.
     *
     * Note that you can not change the values of the pyramid
     * elements when you use a const_iterator. See also begin()
     */
    inline const_iterator begin() const;

    /**
     * Returns an iterator pointing to the first element, i.e. the
     * lowest level of the pyramid.
     *
     * The use of the interators is similar to the iterators of the
     * Standard Template Library (STL).
     */
    inline iterator begin();

    /**
     * Returns last index as a const iterator.
     * For an example see begin()
     */
    inline const_iterator end() const;

    /**
     * Returns last index as an iterator
     * For an example see begin()
     */
    inline iterator end();

    /**
     * Change the number of levels/resolutions of the pyramid
     *
     * @param levels the new number of resolutions of the pyramid
     * @param copyData if true (default), the old data will be keeped.
     *                 If false, all data will be lost.
     */
    virtual void resize(const int levels,
			const bool copyData=true);

    /**
     * Append one element to the pyramid
     *
     * @param theElement Element that will be appended to the pyramid
     */
    void append(const T& theElement);

    /**
     * Delete all data from the pyramid, equivalent to resize(0);
     */
    void clear();

    /**
     * Read-only access to the level x of the pyramid
     * @param x index of the pyramid element to be accessed.  It should
     *          be between 0 and size()-1
     * @return the number of entries in the given cell
     */
    inline const T& at(const int x) const;

    /**
     * Access level x of the pyramid
     * @param x index of the pyramid element to be accessed.  It should
     *          be between 0 and size()-1
     * @return the number of entries in the given cell
     */
    inline T& at(const int x);

    /**
     * Read-only access to the level x of the pyramid
     * @param x index of the pyramid element to be accessed.  It should
     *          be between 0 and size()-1
     * @return the number of entries in the given cell
     */
    inline const T& operator[](const int x) const;

    /**
     * Access level x of the pyramid
     * @param x index of the pyramid element to be accessed.  It should
     *          be between 0 and size()-1
     * @return the number of entries in the given cell
     */
    inline T& operator[](const int x);


    /**
     * Assigment operator.
     * copy the contents of <code>other</code> in this %object.
     * @param other the source pyramid to be copied.
     * @return a reference to this object
     */
    pyramid<T>& copy(const pyramid<T>& other);

    /**
     * Create a clone of this pyramid
     * @return a pointer to a copy of this pyramid
     */
    virtual pyramid<T>* clone() const;

    /**
     * Create a new instance of this pyramid
     * @return a pointer to a new instance of this pyramid
     */
    virtual pyramid<T>* newInstance() const;

    /**
     * Compare this pyramid with another one.
     *
     * @param other the other pyramid to be compared with
     * @return true if both pyramids have the same elements and same size
     */
    bool equals(const pyramid& other) const;

    /**
     * Compare if the number of levels and the size of each level of
     * this and the other pyramid are the same.
     *
     * @param other the pyramid to be compared with.
     * @return true if both pyramids and their elements have the same size
     */
    bool sameResolutions(const pyramid& other) const;

    /**
     * Compare this pyramid with another one, alias for equals()
     *
     * @param other the other pyramid to be compared with
     * @return true if both pyramids have the same elements and same size
     */
    inline bool operator==(const pyramid& other) const;

    /**
     * Assigment operator (alias for copy(other)).
     * @param other the pyramid to be copied
     * @return a reference to the actual pyramid
     */
    inline pyramid& operator=(const pyramid& other);

  protected:
    /**
     * The data of the pyramid
     */
    std::vector<T> thePyramid_;

    int levels_;
  };

} // namespace lti

#include "ltiPyramid_inline.h"
#include "ltiPyramid_template.h"

#endif

