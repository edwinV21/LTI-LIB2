/*
 * Copyright (C) 1998-2004
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
 * \file   ltiGenericVector.h
 *         Contains a replacement for std::vector, which has been optimized
 *         for the storage of static types, (i.e. types that do not manage
 *         pointers to objects of any kind) and speeds up the access to the
 *         vector size and other internal properties.
 * \author Pablo Alvarado
 * \date   09.04.1999
 *
 * $Id: ltiGenericVector.h,v 1.17 2013-11-06 16:28:29 alvarado Exp $
 */

#ifndef _LTI_GENERIC_VECTOR_H_
#define _LTI_GENERIC_VECTOR_H_

#include "ltiContainer.h"
#include "ltiConstantReferenceType.h"
#include "ltiConstReferenceException.h"
#include "ltiAllocException.h"
#include "ltiTypes.h"
#include "ltiAssert.h"
#include "ltiResizeType.h"
#include "ltiDebugIterator.h"
#include "ltiDeprecated.h" // this should be removed when the methods are too

#include <vector>

namespace lti {
  /**
   * Vector container class.
   *
   * The lti::genericVector class allows the representation of
   * n-dimensional vectors.  The elements of the vector will be
   * indexed between 0 an n-1.
   *
   * Note that this class is NOT intended to be a substitute for std::vector.
   * If you need a vector of elements which use some sort of dynamic memory
   * allocation then you have to use the std::vector class of the Standard
   * Template Library (STL).  This means, you should not try to make a
   * lti::genericVector of other vectors or matrices, which administrate some
   * memory.  If you do so, the behaviour of copy or fill operations will be
   * unpredictable.
   *
   * Most of times you will want to use lti::vector instead.
   *
   * The difference between lti::genericVector and its most used inherited
   * class lti::vector is the support for arithmetical operations.  The
   * generic vector is more a "pure" container than a multi-dimensional point
   * representation.  It inherits its memory managment versatility to
   * lti::vector.  Again, it is usually employed as a container of any \e
   * static type, which do not do by itself any memory managment.
   *
   * The genericVector class is a generic container class implemented as
   * template, where the template type T is the type of the elements in the
   * vector.
   *
   * If you need to create a vector of floats with 256 elements, all of them
   * initialized with a value of 4.27 just create it with
   *
   * \code
   * lti::genericVector<float> myVct(256,4.27f) // creates vector with 256
   *                                            // elements all initialized 
   *                                            // with 4.27f
   * \endcode
   *
   * To access the vector elements use the access operators at() (or the
   * overloaded operator[]()).  For example:
   *
   * \code
   * float accu = 0; // initialize accumulator
   *
   * for (int i = 0; i < myVct.size(); i++) {
   *   tmp += myVct.at(i); // access each element of the vector
   * }
   * \endcode
   *
   * Here is also a difference to the STL std::vector.  In the STL there is
   * always a boundary check for at() and the boundary is never checked with
   * operator[]().  In the LTI-Lib, the debug-mode library always makes a
   * boundary check for both at() and operator[](), and in the release-mode
   * there is no check for any of both methods.
   *
   * @ingroup gAggregate
   */
  template<typename T>
  class genericVector : public container {
  public:

    /**
     * Type of the genericVector elements.
     */
    typedef T value_type;

    /**
     * Return type of the size() member
     */
    typedef int size_type;

    /**
     * \name Internal classes
     */
    //@{

    typedef lti::constReferenceException constReferenceException;

#   ifdef NDEBUG
    /**
     * \c iterator type (allows read and write operations).
     *
     * The use of the iterator classes is similar to the iterators of the STL
     * (Standard Template Library). See lti::genericVector::begin() and
     * lti::genericVector::inverseBegin() for examples.
     *
     * For the debugging version of the iterators, boundary check will be
     * done!  This explains the low speed of the iterators of the debug
     * version.  In the release version, no boundary check will be done,
     * and the iterators are sometimes a factor 10 faster than the
     * debug iterators.
     *
     * The use of the access operator at() is faster than the iterators in
     * the debug version only.  If you need to iterate on a genericVector use
     * iterators instead (in the release version iterators are approximately a
     * factor 3 faster than at()).
     *
     * \warning Try to use the prefix incremental operator (i.e. ++it) instead
     * of the postfix operator (i.e. it++) to allow efficient code also in
     * debug-modus!
     *
     * @see genericVector<T>::const_iterator
     */
    typedef T* iterator;

    /**
     * \c const_iterator type (allows read-only operations).
     *
     * The use of the iterator classes is similar to the iterators of the STL
     * (Standard Template Library). See lti::genericVector::begin() for an
     * example.
     *
     * For the debugging version of the iterators, boundary check will be
     * done!  This explains the low speed of the iterators of the debug
     * version.  In the release version, no boundary check will be done, and
     * the iterators are sometimes a factor 10 faster than the debug
     * iterators.
     *
     * The use of the access operator at() is faster than the iterators in the
     * debug version only.  If you need to iterate on a genericVector use
     * iterators instead (in the release version iterators are approximately a
     * factor 3 faster than at()).
     *
     * \warning Try to use the prefix incremental operator (i.e. ++it) instead
     * of the postfix operator (i.e. it++) to allow efficient code also in
     * debug-modus!
     *
     * @see genericVector<T>::iterator
     */
    typedef const T* const_iterator;


#   else
    
    /**
     * \c iterator type (allows read and write operations).
     *
     * The use of the iterator classes is similar to the iterators of the STL
     * (Standard Template Library). See lti::genericVector::begin() and
     * lti::genericVector::inverseBegin() for examples.
     *
     * For the debugging version of the iterators, boundary check will be
     * done!  This explains the low speed of the iterators of the debug
     * version.  In the release version, no boundary check will be done,
     * and the iterators are sometimes a factor 10 faster than the
     * debug iterators.
     *
     * The use of the access operator at() is faster than the iterators in
     * the debug version only.  If you need to iterate on a genericVector use
     * iterators instead (in the release version iterators are approximately a
     * factor 3 faster than at()).
     *
     * \warning Try to use the prefix incremental operator (i.e. ++it) instead
     * of the postfix operator (i.e. it++) to allow efficient code also in
     * debug-modus!
     *
     * @see genericVector::const_iterator
     */
    typedef internal::debugIterator<genericVector<T>, false> iterator;

    /**
     * \c const_iterator type (allows read-only operations).
     *
     * The use of the iterator classes is similar to the iterators of the STL
     * (Standard Template Library). See lti::genericVector::begin() for an
     * example.
     *
     * For the debugging version of the iterators, boundary check will be
     * done!  This explains the low speed of the iterators of the debug
     * version.  In the release version, no boundary check will be done, and
     * the iterators are sometimes a factor 10 faster than the debug
     * iterators.
     *
     * The use of the access operator at() is faster than the iterators in the
     * debug version only.  If you need to iterate on a genericVector use
     * iterators instead (in the release version iterators are approximately a
     * factor 3 faster than at()).
     *
     * \warning Try to use the prefix incremental operator (i.e. ++it) instead
     * of the postfix operator (i.e. it++) to allow efficient code also in
     * debug-modus!
     *
     * @see genericVector<T>::iterator
     */
    typedef internal::debugIterator<genericVector<T>,true> const_iterator;
    //@}

#   endif

    /**
     * Default constructor creates an empty genericVector
     */
    genericVector();

    /**
     * Create a genericVector of the given size but do \b not initialize its
     * elements.
     *
     * \warning This is an interface change with the previous library.  It has
     * been done to be consistent with the more basic features of the C++
     * language.  If you write for example "int c;", the content of \c c is
     * not defined, and in the same way, if you want a vector with initialized
     * data, you have to specify explicitely the value with which the elements
     * have to be initialized.
     * 
     * @param theSize number of elements of the genericVector.
     */
    explicit genericVector(const int theSize);

    /**
     * Create a genericVector of the given size and initialize it with the
     * given value.
     *
     * @param theSize number of elements of the genericVector.
     * @param iniValue all elements will be initialized with this value.
     */
    explicit genericVector(const int theSize,const T& iniValue);

    /**
     * Create a genericVector of the given size and initialize it with the
     * given data. The \a data will be copied.
     * @see useExternData()
     * @param theSize number of elements of the genericVector.
     * @param data a pointer to the data that will be copied.
     */
    genericVector(const int theSize,const T data[]);

    /**
     * Create a genericVector of the given size and initialize it with the
     * given data, the same way "useExternData" does.
     * The \a data will \b not be copied!.
     * @see useExternData()
     * @param theSize number of elements of the genericVector.
     * @param data a pointer to the data that will be used.
     * @param constRef if this parameter is \c ConstantReference it will not be
     *                 possible to change the pointer to the external memory
     *                 block nor to resize the genericVector.  Despite this,
     *                 the value of each element can be changed by the access
     *                 operators.
     */
    genericVector(const int theSize,T data[],
                  const eConstantReference constRef);

    /**
     * Create this genericVector as a copy of another genericVector
     * @param other the genericVector to be copied.
     */
    genericVector(const genericVector<T>& other);

    /**
     * Create this genericVector as a copy of specified interval of
     * elements of another genericVector. Indices below zero are set
     * to zero, indices greater than the size of the %genericVector to
     * the size-1.
     * @param other the genericVector to be copied.
     * @param from starting point included
     * @param to end point included.
     */
    genericVector(const genericVector<T>& other,
                  const int from, const int to=MaxIndex);

    /**
     * Create this genericVector as a copy of specified elements of
     * another genericVector.  \a idx can contain the same index more
     * than once.
     * @param other the genericVector to be copied.
     * @param idx indices of the elements to be copied
     */
    genericVector(const genericVector<T>& other, 
                  const genericVector<int>& idx);

    /**
     * Create this genericVector as a copy of another std::genericVector
     * @param other the genericVector to be copied.
     */
    genericVector(const std::vector<T>& other);

    /**
     * Destructor
     */
    virtual ~genericVector();

    /**
     * Check whether this %object owns the data.
     * returns \a false if this genericVector contains a reference to extern
     * data.
     */
    inline bool ownsData() const;

    /**
     * Restore ownership.
     *
     * If this object does not own its data, this member will create a
     * new memory buffer with the same data and will make this matrix
     * as its owner.  You can also be sure, that the new memory block
     * will be connected (see also getMode() ).  If this genericVector
     * already owns its data nothing happens.
     */
    void restoreOwnership();

    /**
     * Reference to extern data.
     *
     * This member allows the use of this %object as an wrapper-%object to
     * access some memory block as a genericVector.
     * The user must take care for memory allocation and deallocation of
     * the block.  This %object will never delete the external data!.
     * @param theSize number of \e elements in the external block.  Note that
     *                this is NOT the number of bytes of the external block.
     * @param data    pointer to the external memory block.
     * @param constRef if this parameter is true, it will not be possible to
     *                change the pointer to the external memory block nor
     *                to resize the genericVector.  Despite this, the value of
     *                each element can be changed by the access operators.
     * For Example:
     * \code
     * int i;
     * double tmp;
     * double a[10];               // memory block!
     *
     * for (i=0;i<10;i++) {
     *   a[i]=2*i;                 // initialize the memory block
     * }
     *
     * lti::genericVector<double> myVct;  // an empty genericVector
     *
     * myVct.resize(5,0);          // resize the genericVector: now 5 elements
     *                             // initialized with 0
     *
     * myVct.useExternData(10,a,true);   // use the genericVector as wrapper
     *                                   // for the memory block
     *
     * tmp = myVct.at(5);                // tmp is now 10
     *
     * myVct.at(9) = 3;                  // the last element of myVct
     *                                   // has now the value 3
     *
     * myVct.resize(5);            // INVALID!! this will throw an exception
     *                             // constReferenceException()
     * \endcode
     *
     *
     * If \a theSize is greater than the allocated memory, the behaviour could
     * be unpredictible.
     */
    void useExternData(const int theSize,
                       T* data,
                       const eConstantReference constRef=VariableReference);

    /**
     * Attach extern data to the vector.
     *
     * This member allows the use of this %object as an access-functor for
     * the 'data'. An access to the element at(x) is equivalent to
     * data[x].
     * If \a theSize is an invalid dimension, the behaviour will be 
     * unpredictible.
     *
     * The memory will be administrated by this genericVector
     * instance, and may be deleted if required (e.g. genericVector deleted
     * or resized!).  The user should not try to manipulate the memory
     * allocation of the data after the attachment!  See also
     * useExternData().
     *
     * @param theSize number of elements of the genericVector
     * @param data a pointer to the memory block to be used
     *
     * Example:
     * \code
     * lti::genericVector<int> myVct;
     * int block1[25];
     * int* block2;
     * block2 = new int[25];
     *
     * myVct.useExternData(25,block1); // ok
     * myVct.attach(25,block1); // wrong!!! matrix will try to manipulate
     *                          // stack memory: DO NOT DO THIS!!!!!
     * myVct.attach(25,block2); // ok!  but do not try to delete the memory
     *                          //      block2!!
     * \endcode
     */
    void attach(const int theSize,T* data);

    /**
     * Free the data of this object and hand it over to the
     * "receiver". The value of ownsData is also transfered to the
     * receiver. (see Note).
     *
     * This function makes a "memory block transfusion" to another
     * genericVector.  It is a very efficient way to make a copy of
     * this genericVector, if you don't need the source data anymore!
     *
     * \b Note: Take care that if the attach() or useExternData()
     * methods of this genericVector have been called before detachment, the
     * same rules for memory management apply now for the receiver.
     *
     * At the end of the detachment, this genericVector will be empty.
     * @param receiver the genericVector which will receive the memory
     *        block.  All data of that genericVector will be first deleted!
     */
    void detach(genericVector<T>& receiver);

    /**
     * Exchange (in a fast way) the data between this and the other
     * genericVector.  Similar to detach(), this method will exchange
     * the complete memory blocks, avoiding an element-wise copy.
     * @param other the genericVector with which the data will be
     *              exchanged.
     */
    void swap(genericVector<T>& other);

    /**
     * Returns the number of elements of the genericVector
     */
    inline size_type size() const;

    /**
     * Returns first index (normally 0)
     */
    inline int firstIdx() const;

    /**
     * Returns first element as a const_iterator.
     *
     * Note that you can not change the values of the genericVector
     * elements when you use a const_iterator. See also begin()
     */
    inline const_iterator begin() const;

    /**
     * Returns iterator pointing to the first element.
     *
     * The use of the iterators is similar to the iterators of the
     * Standard Template Library (STL).
     * If you need to iterate on all elements of the genericVector, you can
     * use following code:
     *
     * \code
     *   int tmp,accu;                        // a temporal variable
     *   lti::genericVector<int> myVct(10,1); // genericVector with 10 elements
     *   // an iterator set to the beginning of myVct
     *   lti::genericVector<int>::iterator it=myVct.begin(); 
     *   // an iterator set to the end of myVct  
     *   lti::genericVector<int>::iterator eit=myVct.begin();
     *
     *   for (; it!=eit ; ++it) {
     *     tmp = *it;                // tmp has value of element pointed
     *                               // by the iterator.
     *     accu += tmp;
     *     (*it) = accu;             // change the value in the genericVector.
     *    }
     * \endcode
     *
     * \note It is significantly faster in debug builds to use a pre-increment
     * with iterators (++it) than a post-increment (it++).
     *
     * Please note that if you define \a it as a const_iterator,
     * you can not do something like \a *it=accu.
     */
    inline iterator begin();

    /**
     * Returns last index (in a genericVector this is always size()-1)
     */
    inline int lastIdx() const;

    /**
     * Returns last index as a const_iterator.
     *
     * For an example see begin()
     */
    inline const_iterator end() const;

    /**
     * Returns last index as an iterator
     *
     * For an example see begin()
     */
    inline iterator end();

    /**
     * This method returns an iterator that points to the \b last
     * valid element of the genericVector. It is used for inverse order
     * iteration through the genericVector using normal iterators (as opposed
     * to reverse_iterators used in the STL). This has the advantage
     * that iterators going from front to end and in the inverse
     * direction are the same and can thus be compared, copied
     * etc. Further the implementation of reverse_iterators is not as
     * fast as that of iterators and thus not desired in the LTI-Lib.
     *
     * \code
     * igenericVector v(false,10);
     * int i,tmp;
     * for (i=0; i<10; i++) {
     *   v.at(i)=i;
     * }
     * igenericVector::iterator forwardIt=v.begin();
     * igenericVector::iterator backIt=v.inverseBegin();
     *
     * while (forwardIt<=backIt) {
     *   tmp = (*forwardIt); (*forwardIt)=(*backIt); (*backIt)=tmp;
     *   ++forwardIt; ++backIt;
     * }
     * \endcode
     */
    inline iterator inverseBegin();

    /**
     * Return an iterator that points to the \b last valid element of the
     * genericVector. See inverseBegin() for more details.
     */
    inline const_iterator inverseBegin() const;

    /**
     * Return an iterator that points to the element \b
     * before the \b first valid element of the genericVector. It is used to
     * mark the end for inverse order iteration through the genericVector
     * using normal iterators (as opposed to reverse_iterators as used
     * in the STL). This has the advantage that iterators going from
     * front to end and in the inverse direction are the same and can
     * thus be compared, copied etc.Further the implementation of
     * reverse_iterators is not as fast as that of iterators and thus
     * not desired in the LTI-Lib.
     */
    inline iterator inverseEnd();

    /**
     * Return an iterator that points to the element \b before the \b first
     * valid element of the genericVector.
     */
    inline const_iterator inverseEnd() const;
    
    /**
     * Change dimension and if desired the contents of the genericVector.
     *
     * @param newSize  the new size of the genericVector
     * @param iniValue the initialization value.
     * @param resizeType specifies what should happen with the data of the
     *                   resized vector.
     *
     * For example:
     * \code
     *   lti::genericVector<int> myVct;  // creates empty genericVector
     *   myVct.resize(5,0);       // genericVector with 5 elements initialized
     *                            // with 0
     *   myVct.resize(10,2);      // genericVector has now 10 elements: the
     *                            // first five are still 0 and the
     *                            // rest have a 2
     *   myVct.resize(20,3,lti::AllocateOnly); // now the genericVector has 20
     *                                         // elements but their values
     *                                         // are unknown.
     *   myVct.resize(5,1,lti::Init); // the genericVector has now 5
     *                                // elements initialized with 1
     *
     * \endcode
     *
     * If the resize is possible (see useExternData()), after the resize, this
     * %object will always own the data!
     *
     * @see eResizeType, resize(const int), allocate(), assign()
     */
    void resize(const int newSize,
                const T& iniValue,
                const eResizeType resizeType=CopyAndInit);

    /**
     * Resize the vector keeping all the old elements, but without initializing
     * the new ones.
     * 
     * This is an alias for resize(newSize,T(),Copy);
     *
     * @see resize(const int,const T&, const eResizeType),allocate(),assign()
     */
    inline void resize(const int newSize);

    /**
     * Change the vector to contain exactltly the given number of elements.  In
     * opposition to resize, allocate() does \b not copy the previous data but
     * only modifies the size of the vector, discarding all contained data and
     * leaving the new data uninitialized.
     *
     * This is in principle an alias to
     * resize(newSize,T(),AllocateOnly).
     *
     * @param newSize new vector size.  All current data will be discarded.
     *
     * @see eResizeType, resize(const int,const T&, const eResizeType)
     * @see resize(const int),allocate(), assign()
     */
    inline void allocate(const int newSize);

    /**
     * Assign \a newSize copies of \a initValue to the genericVector.
     *
     * Change the size and contents of this vector to be exactly \a newSize
     * elements long, with all the elements initialized to the value indicated
     * in \a initValue.  In opposition to resize, this method does \b not copy
     * the previous data but changes the size of the vector, discarding all
     * contained data, replacing it with \a newSize copies of \a initValue.
     *
     * This is in principle an alias to resize(newSize,T(),Init).
     *
     * @param newSize new vector size.
     * @param initValue all elements of the vector will be initialized with
     *        this value.
     */
    inline void assign(const int newSize,const T& initValue);


    /**
     * Removes all elements from the genericVector (Set dimensions to 0)
     */
    void clear();

    /**
     * Returns true if the genericVector is empty
     */
    inline bool empty() const;

    /**
     * Fills the genericVector elements with \a iniValue between
     * \a from and \a to.
     * @param iniValue the elements will be initialized with this
     *                 value.
     * @param from     first element index
     * @param to       last element index
     *
     * If \a from or \a to are out of bounds,
     * they will be (internaly) adjusted to to correct value.
     *
     * Example:
     * \code
     *   lti::genericVector<double> myVct(10,0); // genericVector with 10 
     *                                           // elements with 0
     *   myVct.fill(9,1,3);                      // myVct=[0,9,9,9,0,0,0,0,0,0]
     * \endcode
     */
    void fill(const T& iniValue,const int from = 0,
              const int to = MaxIndex);

    /**
     * Fills the genericVector elements with data pointed by \a data
     * between \a from and \a to.
     * @param data the data to by copied into this genericVector
     * @param from     first element index
     * @param to       last element index
     *
     * If \a from or \a to are out of bounds,
     * they will be (internaly) adjusted to to correct value.
     *
     * Example:
     * \code
     *   double* data = {2,4,8,16};
     *   lti::genericVector<double> myVct(10,0);  // genericVector with 10 
     *                                            // elements with 0
     *   myVct.fill(data,1,3);             // myVct=[0,2,4,8,0,0,0,0,0,0]
     * \endcode
     */
    void fill(const T data[],const int from = 0,
                             const int to = MaxIndex);

    /**
     * Fills the genericVector elements from \a from to \a to with the
     * elements of \a vct starting at \a startAt.
     *
     * @param vct genericVector with the elements to be copied
     * @param from first element index of the actual genericVector
     * @param to   last element index of the actual genericVector
     * @param startAt start index of the source genericVector \a vct.
     *
     * Example:  if a = [0 0 0 0 0] and b = [1 2 3], after a.fill(b,3,4,1)
     * results a = [0 0 0 2 3]
     */
    void fill(const genericVector<T>& vct,const int from = 0,
              const int to = MaxIndex,
              const int startAt = 0);

    /** 
     * Return the pointer to the data block, i.e.
     * the pointer to the first element of the vector
     */
    inline T* data();

    /**
     * Return the const pointer to the data block, i.e.
     * the pointer to the first element of the vector
     */
    inline const T* data() const;


    /**
     * Access element x of the genericVector
     *
     * @param x index of the genericVector element to be accessed. It should
     * be between firstIdx() and lastIdx()
     */
    inline T& at(const int x);

    /**
     * Access element x of the genericVector in a read-only modus.
     *
     * @param x index of the genericVector element to be accessed. It should
     * be between firstIdx() and lastIdx()
     */
    inline const T& at(const int x) const;

    /**
     * Access operator (alias for at(const int x)).
     */
    inline T& operator[](const int x);

    /**
     * Constant access operator (alias for at(const int x) const).
     */
    inline const T& operator[](const int x) const;

    /**
     * Access element \p n of the genericVector. (alias for at(const int x))
     *
     * This member function is needed for generic programming with different
     * container types.
     */
    inline T& elem(int n);

    /**
     * Constant access to element \p n of the genericVector. (alias for
     * at(const int x) const)
     *
     * This member function is needed for generic programming with different
     * container types.
     */
    inline const T& elem(int n) const;

    /**
     * Assigment operator.
     *
     * Copy the contents of \a other in this %object.
     *
     * If this instance has a constReference, then only the contents are
     * copied.
     *
     * @param other the source genericVector to be copied.
     *
     * @return Reference to the current object.
     */
    genericVector<T>& copy(const genericVector<T>& other);

    /**
     * Assignment operator.
     *
     * Copy a specified interval of elements of another genericVector.  At the
     * end this vector will contain t-f+1 elements, where f=max(0,from) and
     * t=min(other.lastIdx()-1).
     *
     * @param other the genericVector to be copied.
     * @param from starting point included
     * @param to end point included.
     *
     * @return Reference to the current object.
     */
    genericVector<T>& copy(const genericVector<T>& other,
                           const int from, const int to=MaxIndex);

    /**
     * Assignment operator.
     *
     * Copy of specified elements of \a other in this %object.  \a idx can
     * contain the same index more than once.
     *
     * @param other the genericVector to be copied.
     * @param idx indices of the elements to be copied
     *
     * @return Reference to the current object.
     */
    genericVector<T>& copy(const genericVector<T>& other, 
                           const genericVector<int>& idx);

    /**
     * Assigment operator (alias for copy(other)).
     *
     * @param other the genericVector to be copied
     *
     * @return a reference to the actual genericVector
     */
    genericVector<T>& operator=(const genericVector<T>& other);

    /**
     * Returns the name of this type 
     */
    virtual const std::string& name() const;

    /**
     * Create a clone of this genericVector.
     *
     * @return A pointer to a copy of this genericVector.
     */
    virtual genericVector<T>* clone() const;

    /**
     * Create a clone of this genericVector.
     *
     * @return a pointer to a copy of this genericVector
     */
    virtual genericVector<T>* newInstance() const;

    /**
     * Compare this genericVector with other.
     *
     * @param other the other genericVector to be compared with
     *
     * @return true if both genericVectors have the same elements and same size
     */
    bool equals(const genericVector<T>& other) const;

    /**
     * Compare this genericVector with other.
     *
     * @param other the other genericVector to be compared with
     *
     * @return true if both genericVectors have the same elements and same size
     */
    inline bool operator==(const genericVector<T>& other) const;

    /**
     * Compare this genericVector with other.
     *
     * @param other the other genericVector to be compared with.
     *
     * @return true if both genericVectors different dimensions or elements
     */
    inline bool operator!=(const genericVector<T>& other) const;

    /**
     * Copy the \a other genericVector by casting each of its elements.
     *
     * @param other The genericVector to be copied.
     *
     * For Example:
     * \code
     *   lti::genericVector<int> vctA(10,1);  // a genericVector of integers
     *   lti::genericVector<double> vctB;     // a genericVector of doubles
     *
     *   vctB.castFrom(vctA);          // this will copy vctA in vctB!!
     * \endcode
     */
    template<class U>
    genericVector<T>& castFrom(const genericVector<U>& other);

    /**
     * This is just an alias for copy(const genericVector<T>&) to facilitate
     * generic programming.
     *
     * @param other The genericVector to be copied.
     */
    genericVector<T>& castFrom(const genericVector<T>& other);

    /**
     * Copy a subvector of the \a other genericVector by casting each of its
     * elements.
     *
     * @param other The genericVector to be copied.
     * @param from starting point included
     * @param to end point included.
     *
     * @return A reference to the current casted subvector
     */
    template<class U>
    genericVector<T>& castFrom(const genericVector<U>& other,
                               const int from, const int to=MaxIndex);


    /**
     * This is just an alias for copy(const genericVector<T>&, const int from,
     * const int to) to facilitate generic programming.
     *
     * @param other The genericVector to be copied.
     * @param from starting point included
     * @param to end point included
     */
    genericVector<T>& castFrom(const genericVector<T>& other,
                               const int from, const int to=MaxIndex);

    /**
     * Cast from a std::genericVector of the same type
     */
    template<class U>
    genericVector<T>& castFrom(const std::vector<U>& other);

    /**
     * @name Apply Methods
     */
    //@{

    /**
     * Applies a C-function to each element of the genericVector.
     * 
     * In the following example, %genericVector \a vct is initialized with
     * 4.0. After applying \a sqrt(), all elements of \a vct are 2.0.
     * \code
     * genericVector<float> vct(4,4.0);
     * vct.apply(sqrt);
     * \endcode
     *
     * @param function a pointer to a C-function
     *
     * @return A reference to the actual genericVector
     */
    genericVector<T>& apply(T (*function)(T));

    /**
     * Applies a C-function to each element of the other genericVector and
     * leaves the result here.
     *
     * @param other the source genericVector
     * @param function a pointer to a C-function
     *
     * @return A reference to the actual genericVector
     */
    genericVector<T>& apply(const genericVector<T>& other,T (*function)(T));

    /**
     * Applies a C-function to each element of the genericVector.
     *
     * @param function a pointer to a C-function
     *
     * @return A reference to the actual genericVector
     */
    genericVector<T>& apply(T (*function)(const T&));

    /**
     * Applies a C-function to each element the other genericVector and
     * leaves the result here.
     *
     * @param other the genericVector with the source data
     * @param function a pointer to a C-function
     *
     * @return A reference to the actual genericVector
     */
    genericVector<T>& apply(const genericVector<T>& other,
                            T (*function)(const T&));

    /**
     * A two-parameter C-function receives the i-th elements of this
     * and the given genericVector and the result will be left in this
     * genericVector.  Note that both genericVectors MUST have the same size!
     * If both genericVectors have different size, the function will throw an
     * assertion without changing anything!
     *
     * @param other the second genericVector to be considered (the first
     *              genericVector will be this object!)
     * @param function a pointer to a two parameters C-function
     *
     * @return A reference to the actual genericVector.
     */
    genericVector<T>& apply(const genericVector<T>& other,
                            T (*function)(const T&,const T&));

    /**
     * A two-parameter C-function receives the i-th elements of this
     * and the given genericVector and the result will be left in this
     * genericVector.  Note that both genericVectors MUST have the same size!
     * If both genericVectors have different size, the function will throw an
     * assertion without changing anything!
     *
     * @param other the second genericVector to be considered (the first
     *              genericVector will be this object!)
     * @param function a pointer to a two parameters C-function
     *
     * @return A reference to the actual genericVector.
     */
    genericVector<T>& apply(const genericVector<T>& other,T (*function)(T,T));

    /**
     * A two-parameter C-function receives the i-th elements of the
     * given genericVectors and leaves the result here.
     * Note that both genericVectors MUST have the same size!
     * If both genericVectors have different size, the function will throw an
     * assertion without changing anything!
     *
     * The following example uses lti::min as function. The genericVectors \a
     * a and \a b contain the values [1,2,3,4] and [4,3,2,1],
     * respectively. After applying the function, %genericVector \a c
     * contains the values [1,2,2,1].
     * \code 
     * igenericVector a,b,c;
     * int i=0;
     * for (i=0; i<4; ++i) {
     *   a.at(i)=i+1;
     *   b.at(i)=4-i;
     * }
     * c.apply(a,b,lti::min);
     * \endcode
     *
     * @param a the first genericVector
     * @param b the second genericVector
     * @param function a pointer to a two parameters C-function
     *
     * @return A reference to the actual genericVector
     */
    genericVector<T>& apply(const genericVector<T>& a,
                            const genericVector<T>& b,
                            T (*function)(const T&,const T&));

    /**
     * A two-parameter C-function receives the i-th elements of the
     * given genericVectors and leaves the result here.
     *
     * Note that both genericVectors MUST have the same size!
     * If both genericVectors have different size, the function will throw an
     * assertion without changing anything!
     *
     * @param a the first genericVector
     * @param b the second genericVector
     * @param function a pointer to a two parameters C-function
     *
     * @return a reference to the actual genericVector
     */
    genericVector<T>& apply(const genericVector<T>& a,
                     const genericVector<T>& b,
                     T (*function)(T,T));

    //@}

    /**
     * @name Input and Output
     */
    //@{
    /**
     * Write the object in the given ioHandler
     */
    virtual bool write(ioHandler& handler,const bool complete = true) const;

    /**
     * Read the object from the given ioHandler
     */
    virtual bool read(ioHandler& handler,const bool complete = true);
    //@}

  protected:
    /**
     * Dimension of the genericVector.
     */
    int vectorSize_;

    /**
     * Index of the last element of the genericVector (always
     * vectorSize-1)
     * 
     * We sacrifice this sizeof(int) to improve speed in many operations that
     * require this last index.
     */
    int idxLastElement_;

    /**
     * Pointer to the first element of the genericVector.
     */
    T* theElements_;

    /**
     * Reference to extern data.
     *
     * If this value ist \a false, then the data pointed by
     * \a theElements will never be deleted in this
     * %object! (see useExternData())
     */
    bool ownData_;

    /**
     * If constReference=true, is not possible to resize or change
     * the reference of this genericVector. 
     *
     * Very important for an efficient matrix class!!
     * (see useExternData())
     */
    eConstantReference constReference_;
  };

} // namespace lti

namespace std {
  /*
   * outputs the elements of the genericVector on a std::stream
   */
  template <typename T>
  ostream& operator<<(ostream& s,const lti::genericVector<T>& v);
}

#include "ltiGenericVector_inline.h"
#include "ltiGenericVector_template.h"

#else

#include "ltiGenericVector_template.h"

#endif

