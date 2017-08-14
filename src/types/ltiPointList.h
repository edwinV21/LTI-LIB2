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
 * \file   ltiPointList.h
 *         Defines a class for lists of points
 * \author Suat Akyol
 * \date   28.06.2000
 *
 * $Id: ltiPointList.h,v 1.11 2009-03-19 15:35:56 alvarado Exp $
 */

#ifndef _LTI_POINTLIST_H_
#define _LTI_POINTLIST_H_

#include "ltiObject.h"
#include "ltiIoObject.h"
#include "ltiTypes.h"
#include "ltiPoint.h"
#include "ltiRectangle.h"
#include "ltiException.h"
#include "ltiVector.h"
#include "ltiList.h"

#include <iostream>

namespace lti {
  /**
   * Template class to store a list of points.
   *
   * The %lti::pointList class allows the storage of a list of points<T>.
   * The elements of the pointList can be accessed through iterators.
   *
   * There are several inherited classes which have an additional
   * semantical meaning.
   *
   * @see borderPoints, areaPoints, ioPoints, polygonPoints
   *
   * Example:
   *
   * \code
   * lti::pointList pts;  // a list of points with integer coodinates
   *
   * // create 10 points
   * for (int i=0;i<10;++i) {
   *   pts.push_back(point(i,i));
   * }
   *
   * // iterate on the list of points to add 1 to x and 2 to y:
   * lti::pointList::iterator it;
   * for (it=pts.begin();it!=pts.end();++pts) {
   *   (*pts).add(point(1,2));
   * }
   * \endcode
   *
   * @ingroup gAggregate
   *
   * This class usually is just a wrapper for the
   * lti::smallObjectList<point<T>>, but it depends on the size you
   * set while compiling the library for the lti::list to be a
   * std::list or a lti::smallObjectList.
   */
  template<class T>
  class pointList : public ioObject, public list< point<T> > {
  public:
    
    /**
     * Iterator type (allows read and write operations).
     *
     * The use of the iterator classes is similar to the iterators of
     * the STL (Standard Template Library). See lti::pointList::begin()
     * for an example
     */
    typedef typename list< point<T> >::iterator iterator;

    /**
     * Const iterator type (allows read-only operations).
     *
     * The use of the iterator classes is similar to the iterators of
     * the STL (Standard Template Library). See lti::pointList::begin()
     * for an example.
     */
    typedef typename list< point<T> >::const_iterator const_iterator;

    /**
     * Reference type (allows read and write operations)
     * The use of the reference classes is similar to the references of
     * the STL (Standard Template Library).
     */
    typedef typename list< point<T> >::reference reference;

    /**
     * Const_reference type (allows read-only operations)
     * The use of the reference classes is similar to the references of
     * the STL (Standard Template Library).
     */
    typedef typename list< point<T> >::const_reference const_reference;

    /**
     * Default constructor creates an empty pointList;
     */
    pointList();

    /**
     * Create this pointList as a copy of another pointList
     * @param other the pointList to be copied.
     */
    pointList(const pointList<T>& other);

    /**
     * Create this pointList as a copy of a list< point<T> > of point<T>
     * @param other the pointList to be copied.
     */
    pointList(const list< point<T> >& other);

    /**
     * Destructor
     */
    virtual ~pointList();

    /**
     * Returns the number of elements of the pointList
     */
//     int size() const;

    /**
     * Compares the size of this list with the size of the other point list
     * and returns true if this list has fewer points than the other one.
     */
    bool operator<(const pointList<T>& other) const;

    /**
     * Compares the size of this list with the size of the other point list
     * and returns true if this list has more points than the other one.
     */
    bool operator>(const pointList<T>& other) const;

    /**
     * Returns first element as a const_iterator.
     * Note that you can not change the values of the pointList
     * elements when you use a const_iterator. See also begin()
     */
//     const_iterator begin() const;

    /**
     * Returns an iterator pointing to the first element of the pointList
     * The use of the interators is similar to the iterators of the
     * Standard Template Library (STL).
     * If you need to iterate on all elements of the pointList, you can
     * use following code:
     * \code
     * lti::pointList<int> myPL;               // an empty pointList
     * lti::pointList<int>::iterator it;       // an iterator
     *
     * // Fill pointList with some arbitrary points
     * for (int i=0; i<10; i++) {
     *   myPL.push_back(lti::point(0,i));
     * }
     *
     * // Swap x and y for all points in list
     * for (it=myPL.begin();it!=myPL.end();it++) {
     *   int temp;
     *   temp = (*it).x;
     *   (*it).x = (*it).y;
     *   (*it).y = temp;
     * }
     * \endcode
     */
//     iterator begin();

    /**
     * Returns last element as a const iterator.
     * For an example see begin()
     */
//     const_iterator end() const;

    /**
     * Returns last element as an iterator
     * For an example see begin()
     */
//     iterator end();

    /**
     * Deletes all points from list and leaves empty pointList.
     */
//     void clear();

    /**
     * Erases point, which is denoted by it. Returns iterator to next element.
     */
//     iterator erase(const iterator& it);

    /**
     * Erases points between first and last. Returns iterator to next element.
     */
//     iterator erase(const iterator& first, const iterator& last);

    /**
     * Inserts point before position denoted by it.
     * returns iterator to inserted element.
     *
     * \b WARNING: This is not the same as default list behaviour
     * which returns an iterator pointing after the last inserted
     * element!
     */
//     iterator insert(const iterator& it, const point<T>& thePoint);

    /**
     * Inserts points before position denoted by it.
     * Returns iterator to first element of inserted elements.
     *
     * \b WARNING: This is not the same as default list behaviour
     * which returns an iterator pointing after the last inserted
     * element!
     */
//     iterator insert(const iterator& it,
//                     const int n,
//                     const point<T>& thePoint);

    /**
     * Inserts the elements from first to last, before position denoted by it.
     * Returns iterator to first element of inserted elements.
     *
     * \b WARNING: This is not the same as default list behaviour
     * which returns an iterator pointing after the last inserted
     * element!
     */
//     iterator insert(const iterator& it,
//                     const_iterator first,
//                     const_iterator last);

    /**
     * Transfer all elements in the second list into this one.
     * At the end of the operation, the second list will be empty
     */
//     void splice(const iterator& pos,
//                 pointList<T>& other);

    /**
     * Inserts element at begin of pointList
     */
//     void push_front(const point<T>& thePoint);

    /**
     * Removes element at begin of pointList
     */
//     void pop_front();

    /**
     * Inserts element at end of pointList
     */
//     void push_back(const point<T>& thePoint);

    /**
     * Removes element at end of pointList
     */
//     void pop_back();

    /**
     * Returns a reference to the first element
     */
//     reference front();

    /**
     * Returns a const reference to the first element
     */
//     const_reference front() const;

    /**
     * Returns a reference to the last element
     */
//     reference back();

    /**
     * Returns a const reference to the last element
     */
//     const_reference back() const;

    /**
     * Copy function.
     * copy the contents of \a other in this %object.
     * @param other the source pointList to be copied.
     */
    pointList<T>& copy(const pointList<T>& other);

    /**
     * Assigment operator (alias for copy(other)).
     * @param other the pointList to be copied
     * @return a reference to the actual pointList
     */
    pointList<T>& operator=(const pointList<T>& other);

    /**
     * Returns the name of this type.
     */
    virtual const std::string& name() const;

    /**
     * Create a clone of this pointList
     * @return a pointer to a copy of this pointList
     */
    virtual ioObject* clone() const;

    /**
     * Create a new instance of pointList
     * @return a pointer to a copy of this pointList
     */
    virtual ioObject* newInstance() const;

    /**
     * Sort the points int the pointList in ascending order first of
     * y, and than of x.
     */
//     inline void sort();

    /**
     * Sort the pointList according to the compare function \a comp,
     * which must return a bool and take two point<T> as
     * arguments. See std::less() for an example.
     */
//     template <class Compare>
//     inline void sort(Compare comp);

    /**
     * @name Conversion Methods
     */
    //@{

    /**
     * Copy the elements of the other standard list of point<T> in this
     * %object
     * @param other the source pointList to be copied.
     */
    pointList<T>& castFrom(const list< point<T> >& other);

    /**
     * Copy the \a other point-list by casting each of its elements
     * @param other The point list to be casted
     */
    template<class U>
    inline pointList<T>& castFrom(const pointList<U>& other);

    /**
     * Cast the given vector of points into a list, where the
     * first element in the vector will be the first element
     * in the list.
     * @param other the vector of points with the points
     * @return a reference to this instance
     */
    template<class U>
    inline pointList<T>& castFrom(const vector< point<U> >& other);

    /**
     * Cast the given vector of points into a list, where the
     * first element in the vector will be the first element
     * in the list.
     */
    template<class U>
    inline pointList<T>& castFrom(const vector< point<U> >& other) const;

    /**
     * Cast this list of points into a lti::vector, which can be
     * accessed in a faster way than the list.
     */
    template<class U>
    inline void castTo(vector< point<U> >& other) const;
    
    //@}

    /**
     * Compare this pointList with other
     * @param other the other pointList to be compared with
     * @return true if both pointLists have the same elements and same size
     */
    bool equals(const pointList<T>& other) const;

    /**
     * Compare this pointList with other
     * @param other the other pointList to be compared with
     * @return true if both pointLists have the same elements and same size
     */
    bool operator==(const pointList<T>& other) const;

    /**
     * Returns true if the list is empty
     */
//     bool empty() const;

    /**
     * Returns the last set or calculated boundary.
     *
     * The boundary is the smallest rectangle that contains all the
     * points in the list.  Note that the boundary must be set by the
     * user, or the user must explicitly specify that it must be
     * updated (see lti::pointList<T>::updateBoundary).  It will NOT
     * be updated automatically
     */
    inline const rectangle<T>& getBoundary() const;

    /**
     * Set the boundary of the points.
     *
     * The boundary is the smallest rectangle that contains all the
     * points in the list.  Note that the boundary must be set by the
     * user using this method, or the user must explicitly specify
     * that it must be updated (see lti::pointList<T>::updateBoundary).
     * It will \b NOT be updated automatically with each point you insert or
     * delete.
     */
    inline void setBoundary(const rectangle<T>& r);

    /**
     * Calculate the boundary box.
     *
     * The boundary is the smallest rectangle that contains all the
     * points in the list.  Note that the boundary must be set by the
     * user, or the user must explicitly specify that it must be
     * calculated.
     *
     * This member computes the boundary, but it does not set the compute
     * one into the internal boundary attribute.  See also updateBoundary().
     *
     * @return a the calculated boundary
     */
    const rectangle<T> computeBoundary() const;

     /**
     * Calculate and update the boundary box.
     *
     * The boundary is the smallest rectangle that contains all the
     * points in the list.  Note that the boundary must be set by the
     * user, or the user must explicitly specify that it must be
     * calculated.
     *
     * This member computes the boundary AND set the internal boundary
     * attribute.  See also computeBoundary().
     *
     * @return a reference to the calculated boundary
     */
    const rectangle<T>& updateBoundary();

    /**
     * Write the point list in the given ioHandler
     */
    virtual bool write(ioHandler& handler,const bool complete = true) const;

    /**
     * Read the point list from the given ioHandler
     */
    virtual bool read(ioHandler& handler,const bool complete = true);

  protected:
    /**
     * This pointList class is implemented with a list< point<T> > instance
     */
//     list< point<T> > thePointList_;

    /**
     * Boundary is the smallest rectangle which includes all the points
     * in the list
     */
    rectangle<T> boundary_;
  };

  /**
   * Read the vector from the given ioHandler.  The complete flag indicates
   * if the enclosing begin and end should be also be readed
   *
   * @ingroup gStorable
   */
  template<class T>
  bool read(ioHandler& handler,pointList<T>& plst,const bool complete=true);

  /**
   * Write the vector in the given ioHandler.  The complete flag indicates
   * if the enclosing begin and end should be also be written or not
   *
   * @ingroup gStorable
   */
  template<class T>
  bool write(ioHandler& handler, const pointList<T>& plst,
             const bool complete=true);

  /**
   * ipointList is a list of points with integer coordinates
   */
  typedef pointList<int> ipointList;

  /**
   * fpointList is a list of points with integer coordinates
   */
  typedef pointList<float> fpointList;

} // namespace lti

namespace std {

  template<class T>
  std::ostream& operator<<(std::ostream& s,const lti::pointList<T>& pts);
}

#include "ltiPointList_inline.h"

#endif

