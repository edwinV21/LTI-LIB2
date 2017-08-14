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
 * \file   ltiVector.h
 *         Contains the template class lti::vector, which is the mathematical
 *         version of lti::genericVector.  Some arithmetical operations have
 *         been added.
 * \author Pablo Alvarado
 * \date   09.04.1999
 *
 * $Id: ltiVector.h,v 1.11 2012-07-23 17:39:26 alvarado Exp $
 */

#ifndef _LTI_VECTOR_H_
#define _LTI_VECTOR_H_

#define _LTI_GENERIC_VECTOR_DONT_INSTANTIATE_REQUEST
#include "ltiGenericVector.h"
#undef _LTI_GENERIC_VECTOR_DONT_INSTANTIATE_REQUEST

namespace lti {
  /**
   * Mathematical vector container class.
   *
   * The lti::vector class allows the representation of n-dimensional vectors.
   * The elements of the vector will be indexed between 0 an n-1.
   *
   * Note that this class is NOT intended to be a subtitute for std::vector.
   * If you need a vector of elements which use some sort of dynamic memory
   * allocation then you should use the std::vector class of the Standard
   * Template Library.
   *
   * Only following types are supported by lti::vector:
   * - ubyte
   * - byte
   * - int32
   * - uint32
   * - long
   * - float
   * - double
   * - rgbaPixel
   * - rgbPixel<float>
   * - ipoint
   * - fpoint
   * - dpoint
   * - dpoint3D
   * - fcomplex
   * - dcomplex
   *
   * This restriction is done to avoid the typical template code explosion
   * due to frequently used types in the library.  If you need the vector as
   * container of any other type, consider using the std::vector or the
   * lti::genericVector.
   *
   * The vector class is a container class implemented as template, where the
   * template type T denotes the type of the elements.  It is by no means a
   * generic container, since the types it supports must fulfill many
   * requirements, specially support for all arithmetical operators, and the
   * types supported are explicitely instantiated in the library.
   *
   * If you need to create a vector of floats with 256 elements, all
   * initialized with a value of 4.27 just create it:
   *
   * \code
   * lti::vector<float> myVct(256,4.27f) // creates vector with 256 elements
   *                                     // all initialized with 4.27f
   * \endcode
   *
   * To access the vector elements use the access operators at() (or the
   * overloaded operator[]()).  For example:
   *
   *
   * \code
   * float accu = 0; // initialize accumulator
   *
   * for (int i = 0; i < myVct.size(); i++) {
   *   tmp += myVct.at(i); // access each element of the vector
   * }
   * \endcode
   *
   * The vector has following methods:
   * - Constructors Constructors
   *   - You can construct an empty vector with the default constructor
   *     (vector()).
   *   - If you know the number of elements use
   *     vector(const int size,const T& initialValue)
   *   - You can create the copy of another vector with the copy constructor
   *     (vector(const vector<T>& otherVector))
   *   - You can create a vector with a copy of the data of a C++ array
   *     the constructor (vector(const int theSize,const T data[]))
   * - Access members
   *   - at(), operator[]()
   *   - The size() member returns the number of elements of the vector.
   *   - empty() returns true if the size of the vector is zero.
   *   - The firstIdx() will return in a vector always 0 and the lastIdx()
   *     will be always size()-1;
   * - Fill and Copy members
   *   - With the fill() members you can fill the vector with a given
   *     constant value or with values taken from other vectors.
   *   - With the copy() member you can copy another vector, an interval of it
   *     or specified elements to this vector.
   *   - You can specify, that the vector should be used just as a
   *     wrapper-object to access external memory regions: useExternData().
   *     To check if a vector is a wrapper-object you can use ownsData().
   *   - clear() sets the size of the vector to zero.
   * - Mathematical operations
   *   - Scalar product: dot()
   *   - Elementwise multiplication/division: emultiply(), edivide()
   *   - Add (subtract) another vector to (from) the actual vector:
   *     add(const vector<T>&), subtract(const vector<T>&)
   *   - Add another vector \em scaled to the actual vector: addScaled()
   *   - A constant is used as second argument of the functions add,
   *     subtract, multiply, divide and all element of the vector as
   *     first argument: add(const T), subtract(const T), 
   *     multiply(const T), divide(const T)
   *   - computeSumOfElements() and computeProductOfElements() return the sum 
   *     and product of all elements of the vector, respectively
   * - Extremes of the vector
   *   - find minimum, maximum values or both: findMinimum(), findMaximum(), 
   *     findExtremes()
   *   - find their indices: findIndexOfMinimum(), findIndexOfMaximum(),
   *     findIndexOfExtremes()
   * - Iterators
   *   - It is possible to iterate within the vector by making use of
   *     the vector iterators.  (see begin() for more information)
   *   - Instead of reverse_iterators as in the STL we use iterators
   *     going backwards, due to faster execution times (see
   *     inverseBegin() for more information)
   *
   * @ingroup gAggregate
   * @ingroup gLinearAlgebra
   */
  template<typename T>
  class vector : public genericVector<T> {
  public:

    // inherit the iterator types from the generic vector
    typedef typename genericVector<T>::iterator iterator;
    typedef typename genericVector<T>::const_iterator const_iterator;

    /**
     * Default constructor creates an empty vector;
     */
    vector();

    /**
     * Create a vector of the given size. The content of the vector
     * is undefined.
     *
     *
     * \warning This is an interface change with the previous library.  It has
     * been done to be consistent with the more basic features of the C++
     * language.  If you write for example "int c;", the content of \c c is
     * not defined, and in the same way, if you want a vector with initialized
     * data, you have to specify explicitely the value with which the elements
     * have to be initialized.
     *
     * @param theSize number of elements of the vector.
     */
    explicit vector(const int theSize);

    /**
     * Create a vector of the given size and initialize it with the
     * given value.
     *
     * @param theSize number of elements of the vector.
     * @param iniValue all elements will be initialized with this value.
     */
    explicit vector(const int theSize,const T& iniValue);

    /**
     * Create a vector of the given size and initialize it with the
     * given data. The \a data will be copied.
     *
     * @see useExternData()
     *
     * @param theSize number of elements of the vector.
     * @param data a pointer to the data that will be copied.
     */
    vector(const int theSize,const T data[]);

    /**
     * Create a vector of the given size and initialize it with the given
     * data, the same way "useExternData" does.  The \a data will not be
     * copied!.
     *
     * @see useExternData()
     *
     * @param theSize number of elements of the vector.
     * @param data a pointer to the data that will be used.
     * @param constRef if this parameter is true, it will not be possible to
     *                change the pointer to the external memory block nor
     *                to resize the vector.  Despite this, the value of each
     *                element can be changed by the access
     *                operators.  For Example:
     */
    vector(const int theSize,T data[],const eConstantReference constRef);

    /**
     * Create this vector as a copy of another genericVector
     *
     * @param other the vector to be copied.
     */
    vector(const genericVector<T>& other);

    /**
     * Create this vector as a copy of specified interval of elements of
     * another vector. Indices below zero are set to zero, indices greater
     * than the size of the %vector to the size-1.
     *
     * @param other the vector to be copied.
     * @param from starting point included
     * @param to end point included.
     */
    vector(const genericVector<T>& other, 
           const int from,
           const int to=genericVector<T>::MaxIndex);

    /**
     * Create this vector as a copy of specified elements of another vector.
     *
     * \a idx can contain the same index more than once.
     *
     * @param other the vector to be copied.
     * @param idx indices of the elements to be copied
     *
     */
    vector(const genericVector<T>& other, const genericVector<int>& idx);

    /**
     * Create this vector as a copy of another std::vector
     *
     * @param other the vector to be copied.
     */
    vector(const std::vector<T>& other);

    /**
     * Destructor
     */
    virtual ~vector();

    /**
     * Returns the name of this class.
     */
    const std::string& name() const;

    /**
     * Create a clone of this vector.
     *
     * @return a pointer to a copy of this vector
     */
    virtual vector<T>* clone() const;

    /**
     * Create a new vector instance.
     *
     * @return a pointer to a new instance of vector
     */
    virtual vector<T>* newInstance() const;

    /**
     * Compare this vector with other, and use the given tolerance to
     * determine if the value of each element of the other vector
     * approximately equals the values of the actual vector elements.
     *
     * An element \a x is approximately equal to another element \a y
     * with a tolerance \a t, if following equation holds:
     * \a x-t < \a y < \a x+t
     *
     * @param other the other vector to be compared with
     * @param tolerance the tolerance to be used
     *
     * @return true if both vectors are approximatly equal
     */
    bool prettyCloseTo(const genericVector<T>& other,
                       const T& tolerance) const;

    /**
     * @name Arithmetical Operations
     */
    //@{
    /**
     * Dot product with another vector of the \e same type.
     *
     * If the dimensions of both vector are not the same, an assertion will
     * be thrown.
     *
     * If T is a complex type, then the elements of this vector are
     * complex conjugated first.
     *
     * @param other the other vector to be multiplied with
     * @return a scalar value with the type of the vector elements
     */
    T dot(const genericVector<T>& other) const;

    /**
     * Elementwise multiplication.
     * Each element of this vector will be multiplied with the elements
     * of the other vector and the result will be left in this %object!
     * If both vectors have different size, an assertion will be thrown
     * The return vector is this %object!
     * @param other the other vector to be multiplied with
     * @return a reference to the actual vector
     */
    vector<T>& emultiply(const genericVector<T>& other);

    /**
     * Elementwise multiplication.
     * This vector will contain the elementwise multiplication of the
     * elements in \a first and \a second.
     * If both vectors have different size, an assertion will be thrown
     * @param first the first vector
     * @param second the second vector will be multiplied with the
     *               first vector
     * @return a reference to the actual vector
     */
    vector<T>& emultiply(const genericVector<T>& first,
                         const genericVector<T>& second);

    /**
     * Elementwise division.
     *
     * Each element of this vector will be divided by the elements
     * of the other vector and the result will be left in this %object!
     * The returned vector is this %object!
     * If both vectors have different size, an assertion will be thrown
     * @param other the other vector to be divided by
     * @return a reference to the actual vector
     */
    vector<T>& edivide(const genericVector<T>& other);

    /**
     * Elementwise division.
     *
     * This vector will contain the elementwise division of the
     * elements in \a first by \a second.
     * If both vectors have different size, an assertion will be thrown
     * @param first the first vector
     * @param second the second vector, is the divisor
     * @return a reference to the actual vector
     */
    vector<T>& edivide(const genericVector<T>& first,
                       const genericVector<T>& second);

    /**
     * Divide this vector with a constant.  This vector will changed!
     * Returns this vector.
     * synonym for divide(const T cst).
     * @param cst the elements of the vector will be divided with this
     * constant
     * @return a reference to the actual vector
     */
    inline vector<T>& edivide(const T cst);

    /**
     * Divide the other vector with a constant and leave the result here.
     * Returns a reference to this vector. <p>
     * synonym for divide(const vector<T>& other,const T cst).
     * @param other the vector to be divide by the constant value
     * @param cst the elements of the vector will be divided with this
     *            constant
     * @return a reference to the actual vector
     */
    inline vector<T>& edivide(const genericVector<T>& other,const T cst);

    /**
     * Add another vector of same dimension and leave the result in this
     * %object.
     * 
     * The type of the other vector is casted to the one of this vector
     * before adding.
     *
     * If both vectors have different size, an assertion will be
     * thrown @param other the other vector to be added with @return a
     * reference to the actual vector
     */
    template<typename U>
    vector<T>& add(const genericVector<U>& other);

    /**
     * Add two vector and leave the result in this %object.
     * If both vectors have different size, an assertion will be thrown
     *
     * The result is casted to the type of this vector
     *
     * @param first the first vector
     * @param second the second vector will be added with the first
     *               vector
     * @return a reference to the actual vector
     */
    template<typename U,typename V>
    vector<T>& add(const genericVector<U>& first,
                   const genericVector<V>& second);

    /**
     * Add constant to this vector.  This vector is changed.
     * Returns this vector.
     * @param cst constant scalar to be added with each element
     * @return a reference to the actual vector
     */
    vector<T>& add(const T cst);

    /**
     * Add constant to the other vector and leave the result here.
     * Returns a reference to this vector.
     * @param other the other vector
     * @param cst constant scalar to be added with each element of the other
     *            vector
     * @return a reference to the actual vector
     */
    vector<T>& add(const genericVector<T>& other,const T cst);

    /**
     * Alias for add(const T cst)
     */
    vector<T>& operator+=(const T cst);

    /**
     *  Alias for add(const vector<T>& other)
     */
    vector<T>& operator+=(const genericVector<T>& other);

    /**
     * Add another vector scaled by \e b to this vector. The vectors
     * must be of the same types and dimensions. Let \e A be this vector
     * and \e B the other vector, then this method performs:<p>
     * \f$A=A+b\cdot B\f$
     * If both vectors have different size, an assertion will be thrown
     * @param b scaling factor for \a other
     * @param other the vector to be added after scaling
     * @return a reference to this vector
     */
    vector<T>& addScaled(const T b, const genericVector<T>& other);

    /**
     * Leave the scaled %sum of two vectors in this vector. The vectors
     * must be of the same types and dimensions. Let \e A be the
     * first vector and \e B the second vector with corresponding
     * scaling factors \e a and \e b, further \e C this
     * vector, then this method performs:<p>
     * \f$C=a\cdot A+b\cdot B\f$
     * If both vectors have different size, an assertion will be thrown
     * @param a scaling factor for \a first
     * @param first the first vector to be added after scaling
     * @param b scaling factor for \a second
     * @param second the second vector to be added after scaling
     * @return a reference to this vector
     */
    vector<T>& addScaled(const T a, const genericVector<T>& first,
                         const T b, const genericVector<T>& second);


    /**
     * Leave the addition of the first vector and the second vector
     * scaled with the given factor in this vector. The vectors must
     * be of the same types and dimensions. Let \e A be the first
     * vector and \e B the second vector with corresponding scaling
     * factor \e b, further \e C this vector, then this method
     * performs:<p> \f$C=A+b\cdot B\f$
     * If both vectors have different size, an assertion will be thrown
     * @param first the first vector to be added after scaling
     * @param b scaling factor for \a second
     * @param second the second vector to be added after scaling
     * @return a reference to this vector
     */
    vector<T>& addScaled(const genericVector<T>& first,
                         const T b, 
                         const genericVector<T>& second);

    /**
     * Subtract constant from this vector.  This vector is changed.
     * Returns this vector.
     * @param cst constant scalar to be subtracted from each element
     * @return a reference to the actual vector
     */
    vector<T>& subtract(const T cst);

    /**
     * Subtract constant from the other vector and leave the result here.
     * Returns a reference to this vector.
     * @param other the other vector
     * @param cst constant scalar to be subtracted from each element of the
     *            other vector
     * @return a reference to the actual vector
     */
    vector<T>& subtract(const genericVector<T>& other, const T cst);

    /**
     * Subtracts another vector of the same type and same dimension
     * and leaves the result in this %object
     * If both vectors have different size, an assertion will be thrown
     * @param other will be substracted from this vector
     * @return a reference to the actual vector
     */
    vector<T>& subtract(const genericVector<T>& other);

    /**
     * Subtracts two vectors and leaves the result in this %object.
     * If both vectors have different size, an assertion will be thrown
     * @param first the first vector
     * @param second the second vector will be substracted from the
     *               first vector
     * @return a reference to the actual vector
     */
    vector<T>& subtract(const genericVector<T>& first,
                        const genericVector<T>& second);

    /**
     * Alias for substract(const vector<T>& other)
     * If both vectors have different size, an assertion will be thrown
     */
    vector<T>& operator-=(const genericVector<T>& other);

    /**
     * Alias for subtract(const T& cst)
     */
    vector<T>& operator-=(const T cst);

    /**
     * Multiply this vector with a constant.  This vector will changed!
     * Returns this vector.
     * If both vectors have different size, an assertion will be thrown
     * @param cst constant scalar to be multiplied with
     * @return a reference to the actual vector
     */
    vector<T>& multiply(const T cst);

    /**
     * Multiply the other %vector with a constant and leave the result here.
     * Returns a reference to this vector.
     * If both vectors have different size, an assertion will be thrown
     * @param other the other vector to be multiplied with the constant value
     * @param cst constant scalar to be multiplied with the other vector.
     * @return a reference to the actual vector
     */
    vector<T>& multiply(const genericVector<T>& other,const T cst);

    /**
     * Multiply with a constant.  This vector is changed.
     * @param cst constant scalar to be multiplied with
     * @return a reference to the actual vector
     */
    vector<T>& operator*=(const T cst);

    /**
     * Divide this vector by a constant.  This vector will changed!
     * Returns this vector.
     * @param cst the elements of the vector will be divided with this
     * constant
     * @return a reference to the actual vector
     */
    vector<T>& divide(const T cst);

    /**
     * Divide the other vector by a constant and leave the result here.
     * Returns a reference to this vector.
     * @param other the vector to be divide by the constant value
     * @param cst the elements of the vector will be divided with this
     *            constant
     * @return a reference to the actual vector
     */
    vector<T>& divide(const genericVector<T>& other,const T cst);

    /**
     * Alias for divide(const T& cst)
     */
    vector<T>& operator/=(const T cst);

    /**
     * Calculate the sum of all elements of the vector.
     * This member can be used with classes which define the operator '+='
     */
    T computeSumOfElements() const;

    /**
     * Calculate the product of all elements of the vector.
     * This member can be used with classes which define the operator '*='
     */
    T computeProductOfElements() const;
    //@}


    /**
     * @name Find extreme values
     */
    //@{

    /**
     * Find the smallest element of the vector
     */
    T findMinimum() const;

    /**
     * Find the index of the smallest element of the vector
     */
    int findIndexOfMinimum() const;

    /**
     * Find the biggest element of the vector
     */
    T findMaximum() const;

    /**
     * Find the index of the biggest element of the vector
     */
    int findIndexOfMaximum() const;

    /**
     * Find the extremes of the vector (smallest and biggest elements)
     */
    void findExtremes(T& theMinimum, T& theMaximum) const;

    /**
     * Find the indices of the extremes of the vector
     * (smallest and biggest elements)
     */
    void findIndexOfExtremes(int& theIdxMinimum, int& theIdxMaximum) const;
    //@}
  };

} // namespace lti

#include "ltiVector_inline.h"

namespace lti {
  /**
   *  Vector of double
   */
  typedef vector<double> dvector;

  /**
   *  Vector of float
   */
  typedef vector<float>  fvector;

  /**
   *  Vector of integer
   */
  typedef vector<int32>  ivector;

}

#endif
