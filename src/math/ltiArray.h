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
 * \file   ltiArray.h
 *         Contains the template class lti::array<T>.
 * \author Pablo Alvarado
 * \date   09.04.99
 *
 * $Id: ltiArray.h,v 1.17 2010-04-13 03:51:22 alvarado Exp $
 */

#ifndef _LTI_ARRAY_H_
#define _LTI_ARRAY_H_

#include "ltiAssert.h"
#include "ltiVector.h"
#include "ltiResizeType.h"

namespace lti {

  /**
   * Array class.
   *
   * A n-dimensional array indexed between \a from and \a to, where
   * <em>to-from+1 = n</em>.  The indices \a from and \a to can be negative,
   * but \a from must be lower or equal \a to.
   *
   * All inherited arithmetical operations still need to be reviewed, since
   * the vector methods will only check if the size \a n of the array is
   * correct.  Use these operations at your own risk.  It is safer if you use
   * the on-place version of the arithmetical methods, since they won't change
   * the index range of the array. Otherwise, the range will be change to
   * [0,n-1].
   * 
   * @ingroup gAggregate
   */
  template<typename T>
  class array : public vector<T> {
  public:

    enum {

      /**
       * Maximum allowed index (24 bit)
       */
      MaxIndex = +0x007fffff,

      /**
       * Minimum allowed index (24 bit)
       */
      MinIndex = -0x00800000

    };

    /**
     * Default constructor creates an empty array;
     */
    array();

    /**
     * Constructor with uninitialized data.
     *
     * Creates an array indexed between \a from and \a to.
     * The content of the array is undefined.
     * and initializes it with \a iniValue
     *     
     * \warning This is an interface change with the previous library.
     * It has been done to be consistent with the more basic features
     * of the C++ language.  If you write for example \c int \c c; ,
     * the content of \c c is not defined, and in the same way, if you
     * want a vector with initialized data, you have to specify
     * explicitely the value with which the elements have to be
     * initialized.
     *
     * @param from index of first element
     * @param to index of last element
     */
    array(const int from,const int to);

    /**
     * constructor.
     * creates an array indexed between \a from and \a to
     * and initializes it with the given value.
     */
    array(const int from,const int to, const T& iniValue);


    /**
     * Copy constructor.
     * copy contents of \a other array in this array. 
     * @param other the array to be copied
     */
    array(const array<T>& other);

    /**
     * copy constructor.
     * copy contents of \a other vector in this array.
     * @param other the other vector to be copied
     * @param theOffset the optional offset of the indices
     */
    array(const vector<T>& other,const int theOffset=0);

    /**
     * Destructor
     */
    virtual ~array();

    /**
     * Reference to extern data.
     *
     * This member allows the use of this %object as an wrapper-%object to
     * access some memory block as a genericVector.
     * The user must take care for memory allocation and deallocation of
     * the block.  This %object will never delete the external data!.
     * @param from    first index
     * @param to      last index (inclusive)
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
     * lti::array<double> myArray; // an empty array
     *
     * myArray.resize(-2,2,0);     // resize the genericVector: now 5 elements
     *                             // initialized with 0
     *
     * myVct.useExternData(-2,7,a,true); // use the genericVector as wrapper
     *                                   // for the memory block
     *
     * tmp = myVct.at(5);                // tmp is now 10
     *
     * myVct.at(9) = 3;                  // the last element of myVct
     *                                   // has now the value 3
     *
     * myVct.resize(-2,5);         // INVALID!! this will throw an exception
     *                             // constReferenceException()
     * \endcode
     *
     *
     * If the total size equal to \c to - \c from + 1 is greater than the
     * allocated memory, the behaviour could be unpredictible.
     */
    void useExternData(const int from,
                       const int to,
                       T* data,
                       const eConstantReference constRef=VariableReference);

    /**
     * Attach extern data to the array.
     *
     * This member allows the use of this %object as an access-functor for
     * the 'data', so that the first valid index of the array 
     * maps to the first position of the given memory block.
     *
     * If (from-to+1) is greater than the size of the given memory block, the
     * behaviour will be unpredictible.
     *
     * The memory will be administrated by this array instance, and may be
     * deleted if required (e.g. array deleted or resized!).  The user should
     * not try to manipulate the memory allocation of the data after the
     * attachment!  See also useExternData().
     *
     * @param from initial index
     * @param to   final index
     * @param data a pointer to the memory block to be used
     *
     * Example:
     * \code
     * lti::array<int> myArray;
     * int block1[25];
     * int* block2;
     * block2 = new int[25];
     *
     * myArray.useExternData(0,25,block1); // ok
     * myArray.attach(0,25,block1); // wrong!!! array will try to manipulate
     *                              // stack memory: DO NOT DO THIS!!!!!
     * myArray.attach(0,25,block2); // ok!  but do not try to delete the memory
     *                              //      block2!!
     * \endcode
     */
    void attach(const int from,
                const int to,
                T* data);

    /**
     * Free the data of this object and hand it over to the
     * "receiver". The value of ownsData is also transfered to the
     * receiver. (see Note).
     *
     * This function makes a "memory block transfusion" to another array.  It
     * is a very efficient way to make a "copy" of this array, if you don't
     * need the this object anymore!
     *
     * \b Note: Take care that if the attach() or useExternData()
     * methods of this array have been called before detachment, the
     * same rules for memory management apply now for the receiver.
     *
     * At the end of the detachment, this array will be empty.
     *
     * @param receiver the genericVector which will receive the memory
     *        block.  All data of that genericVector will be first deleted!
     */
    void detach(array<T>& receiver);

    /**
     * Exchange (in a fast way) the data between this and the other array.
     * Similar to detach(), this method will exchange the complete memory
     * blocks, avoiding an element-wise copy.  
     *
     * @param other the genericVector with which the data will be exchanged.
     */
    void swap(array<T>& other);


    /**
     * change the dimension of the array.
     * After a resize, this object owns the data!
     */
    void resize(const int from,
                const int to,
                const T& iniValue,
                const eResizeType resizeType = CopyAndInit);

    /**
     * Resize the array keeping all the old elements, but without initializing
     * the new ones.
     *
     * This is an alias for resize(from, to, T(),Copy);
     */
    inline void resize(const int from, const int to);

    /**
     * Change size, in opposite to resize, does \b not copy the previous data
     * but changes the size of the array, discarding all contained data.
     *
     * This is in principle an alias to
     * resize(from,to,T(),AllocateOnly).
     *
     * @param from new start of array
     * @param to new end of array
     */
    inline void allocate(const int from, const int to);

    /**
     * Change size, in opposite to resize, does \b not copy the previous data
     * but changes the size of the array, discarding all contained data.
     * All elements will be initialized with the given value.
     *
     * This is in principle an alias to
     * resize(from,to,T(),Init).
     *
     * @param from new start of array
     * @param to new end of array
     * @param initValue all elements of the vector will be initialized with
     *        this value.
     */
    inline void assign(const int from, const int to, const T& initValue);

    /**
     * fills the array with value \a data between \a from
     * and \a to.
     */
    void fill(const T& data,
              const int from = MinIndex,
	      const int to = MaxIndex );

    /**
     * Fills the array with data pointed by \a data
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
     *   lti::array<double> myAry(10,0);  // array with 10 
     *                                    // elements with 0
     *   myAry.fill(data,1,3);            // myAry=[0,2,4,8,0,0,0,0,0,0]
     * \endcode
     */
    void fill(const T data[],
              const int from = MinIndex,
              const int to = MaxIndex);

    /**
     * returns first index.
     * the returned value is equal to \a from -parameter by construction
     * or resizing.
     */
    inline int firstIdx() const {return firstArrayElement_;};

    /**
     * change the first index of the array, without changing the content
     */
    void setFirstIdx(const int newIdx);

    /**
     * returns last index.
     * the returned value is equal to \a to -parameter by contruction of
     * resizing
     */
    inline int lastIdx() const {return lastArrayElement_;};

    /**
     * access operator.
     * access element x with firstIdx() <= x <= lastIdx().
     */
    inline T& at(const int x) {
      assert ( (firstIdx() <= x) && (x <= lastIdx()) );
      assert ( this->theElements_ != 0 );
      return ( theElement0_[x] );
    };

    /**
     * const access operator.
     * access element x with firstIdx() <= x <= lastIdx().
     */
    inline const T& at(const int x) const {
      assert ( (firstIdx() <= x) && (x <= lastIdx()) );
      assert ( this->theElements_ != 0 );
      return ( theElement0_[x] );
    };

    /**
     * access operator (alias for at(x)).
     */
    inline T& operator[](const int x) {return at(x);};

    /**
     * const access operator (alias for at(x))
     */
    inline const T& operator[](const int x) const {return at(x);};


    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    array<T>& copy(const array<T>& other);


    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    array<T>& copy(const vector<T>& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    array<T>& operator=(const array<T>& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    array<T>& operator=(const vector<T>& other);

    /**
     * Returns the name of this class.
     */
    virtual const std::string& name() const;

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual array<T>* clone() const;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual array<T>* newInstance() const;

    /**
     * cast and copy the elements of the "other" array
     */
    template<class U>
    array<T>& castFrom(const array<U>& other) {

      vector<T>::castFrom(other);

      offset_ = -other.firstIdx();
      firstArrayElement_ = other.firstIdx();
      lastArrayElement_  = other.lastIdx();
      theElement0_ = this->theElements_ + offset_;

      return (*this);
    }


    /**
     * @name Apply Methods
     */
    //@{

    /**
     * applies a C-function to each element of the array.
     * 
     * In the following example, %array \a vct is initialized with
     * 4.0. After applying \a sqrt(), all elements of \a vct are 2.0.
     * \code
     * array<float> vct(4,4.0);
     * vct.apply(sqrt);
     * \endcode
     * @param function a pointer to a C-function
     * @return a reference to the actual array
     */
    array<T>& apply(T (*function)(T));

    /**
     * applies a C-function to each element of the other array and leaves
     * the result here.
     * @param other the source array
     * @param function a pointer to a C-function
     * @return a reference to the actual array
     */
    array<T>& apply(const array<T>& other,T (*function)(T));

    /**
     * applies a C-function to each element of the array.
     * @param function a pointer to a C-function
     * @return a reference to the actual array
     */
    array<T>& apply(T (*function)(const T&));

    /**
     * applies a C-function to each element the other array and
     * leaves the result here.
     * @param other the array with the source data
     * @param function a pointer to a C-function
     * @return a reference to the actual array
     */
    array<T>& apply(const array<T>& other,T (*function)(const T&));

    /**
     * a two-parameter C-function receives the i-th elements of this
     * and the given array and the result will be left in this
     * array.  Note that both arrays MUST have the same size!
     * If both arrays have different size, the function will throw an
     * assertion without changing anything!
     * @param other the second array to be considered (the first
     *              array will be this object!)
     * @param function a pointer to a two parameters C-function
     * @return a reference to the actual array
     */
    array<T>& apply(const array<T>& other,T (*function)(const T&,const T&));

    /**
     * a two-parameter C-function receives the i-th elements of this
     * and the given array and the result will be left in this
     * array.  Note that both arrays MUST have the same size!
     * If both arrays have different size, the function will throw an
     * assertion without changing anything!
     * @param other the second array to be considered (the first
     *              array will be this object!)
     * @param function a pointer to a two parameters C-function
     * @return a reference to the actual array
     */
    array<T>& apply(const array<T>& other,T (*function)(T,T));

    /**
     * a two-parameter C-function receives the i-th elements of the
     * given arrays and leaves the result here.
     * Note that both arrays MUST have the same size!
     * If both arrays have different size, the function will throw an
     * assertion without changing anything!
     *
     * The following example uses lti::min as function. The arrays \a
     * a and \a b contain the values [1,2,3,4] and [4,3,2,1],
     * respectively. After applying the function, %array \a c
     * contains the values [1,2,2,1].
     * \code 
     * iarray a,b,c;
     * int i=0;
     * for (i=0; i<4; ++i) {
     *   a.at(i)=i+1;
     *   b.at(i)=4-i;
     * }
     * c.apply(a,b,lti::min);
     * \endcode
     * @param a the first array
     * @param b the second array
     * @param function a pointer to a two parameters C-function
     * @return a reference to the actual array
     */
    array<T>& apply(const array<T>& a,
                     const array<T>& b,
                     T (*function)(const T&,const T&));

    /**
     * a two-parameter C-function receives the i-th elements of the
     * given arrays and leaves the result here.
     * Note that both arrays MUST have the same size!
     * If both arrays have different size, the function will throw an
     * assertion without changing anything!
     * @param a the first array
     * @param b the second array
     * @param function a pointer to a two parameters C-function
     * @return a reference to the actual array
     */
    array<T>& apply(const array<T>& a,
                     const array<T>& b,
                     T (*function)(T,T));

    //@}


    /**
     * write the object in the given ioHandler
     */
    virtual bool write(ioHandler& handler,const bool complete = true) const;

    /**
     * read the object from the given ioHandler
     */
    virtual bool read(ioHandler& handler,const bool complete = true);


  protected:
    /**
     * offset for the 0-th element
     */
    int offset_;

    /**
     * index of the first element of the array
     */
    int firstArrayElement_;

    /**
     * index of the last element of the array.
     *
     * if the array is casted to a vector, the "lastIdx()" should return
     * size()-1 instead of the array lastIdx!.  That's the reason why
     * we use a new member for the last index in the array!
     */
    int lastArrayElement_;

    /**
     * A pointer to the element at index 0
     * (This allows a faster access than adding the offset each time)
     */
    T* theElement0_;
  };

}

#include "ltiArray_inline.h"

#endif

