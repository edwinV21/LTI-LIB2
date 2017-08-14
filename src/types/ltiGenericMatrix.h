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
 * \file   ltiGenericMatrix.h
 *         Contains a template class to describe matrices of data, in a way
 *         that each row can be accessed as a lti::genericVector
 * \author Pablo Alvarado
 * \date   09.04.1999
 *
 * $Id: ltiGenericMatrix.h,v 1.21 2013-11-06 16:28:29 alvarado Exp $
 */

#ifndef _LTI_GENERIC_MATRIX_H_
#define _LTI_GENERIC_MATRIX_H_

#include "ltiContainer.h"
#include "ltiGenericVector.h"
#include "ltiPoint.h"
#include "ltiRectangle.h"
#include "ltiAssert.h"
#include "ltiResizeType.h"
#include "ltiDebugIterator.h"
#include "ltiDeprecated.h"

namespace lti {
  /**
   * Container class for generic matrices.
   *
   * The lti::genericMatrix class allows the representation of \e n x \e m
   * matrices of any type that does not use any form of dynamic memory
   * allocation.  The rows will be indexed between 0 and n-1, and the columns
   * between 0 and m-1.
   *
   * All types defined in ltiTypes.h use static members and can be contained
   * by the lti::genericVector and lti::genericMatrix classes.
   *
   * The genericMatrix class is a container class implemented as template.
   *
   * If you need to create a genericMatrix of floats with 20 rows and
   * 15 columns, all elements initialized with an initial value of
   * 4.27 just create it:
   *
   * \code
   * lti::genericMatrix<float> myMat(20,15,4.27f) // creates genericMatrix 
   *                                              // with 300 elements
   *                                              // all initialized with 4.27f
   * \endcode
   *
   * To access the lti::genericMatrix elements use the access operators.
   * There are many possibilities.  With at(const int row, const int
   * col) it is possible to access an element directly.  With getRow(const
   * int row) you can get the row vector.  You cannot for instance
   * resize nor change the memory referenced in this vector (see
   * lti::vector::resize).  For example:
   *
   * \code
   * float accu = 0; // initialize accumulator
   * lti::genericMatrix<float> myMat(20,15,4.27f)
   * lti::vector<float> myVct;
   *
   * for (int j = 0; j < myMat.rows(); j++) {
   *   for (int i = 0; i < myMat.columns(); i++) {
   *   tmp += myMat.at(j,i); // access each element of the genericMatrix:
   *                         // j is the row and i the column
   *   }
   * }
   *
   * myMat.getRowCopy(5,myVct); // copy the sixth row in myVct!
   * myVct.resize(6);           // Valid, the vector has its own memory!
   * myMat.getRow(5).resize(6)  // ERROR!! the vector is not resizable!
   *
   * \endcode
   *
   * The image representation in the LTI-Lib is based on the
   * lti::genericMatrix class.  It is quite confusing to use first the
   * y-coordinate and then the x-coordinate to access the image elements.  To
   * avoid confusion use the lti::point class to access the elements of the
   * genericMatrix:
   *
   * \code
   *
   * lti::channel8 aChannel(20,15); // creates an 8bit image
   * lti::channel8::value_type tmp; // tmp is of the element type of the
   *                                // channel8!
   * lti::point p;
   *
   *
   * for (p.y = 0; p.y < aChannel.rows(); p.y++) {
   *   for (p.x = 0; p.x < aChannel.columns(); p.x++) {
   *   tmp += aChannel.at(p); // access each element of the genericMatrix:
   *                          // equivalent to: tmp += aChannel.at(p.y,p.x)!
   *   }
   * }
   *
   * \endcode
   *
   * The generic matrices can be created in two modes: Connected or Lined.  In
   * the connected matrices the memory of the first element in a row is the
   * next cell to the one of the last element in the previous row.  In the
   * "Lined" matrices this is not the case.  The are used in the LTI-Lib to
   * represent submatrices that share the memory with a bigger matrix.  Many
   * functor programers have ignored this fact, so it is possible that several
   * classes cannot cope with "Lined" matrices.  If you detect such a problem
   * please report it to one of the mailing lists or make a bug report.
   *
   * @see lti::image, lti::channel, lti::channel8,
   *      lti::matrix
   *
   * The genericMatrix has following methods:
   * - Constructors Constructors
   *   -  You can construct an empty genericMatrix with the default constructor
   *      (genericMatrix()).
   *   - If you know the number of rows and columns use
   *     genericMatrix(const int rows,const int columns,const T&
   *     initialValue)
   *   - You can create a copy of another genericMatrix or just copy a
   *     submatrix with the copy constructor (genericMatrix(const
   *     genericMatrix<T>& otherGenericMatrix))
   * - Access members
   *   - at(), operator[]()
   *   - The rows() member returns the number of rows of the genericMatrix.
   *   - The columns() member returns the number of columns of the
         genericMatrix.
   * - Fill and Copy members
   *   - With the fill() members you can fill the genericMatrix with a given
   *     constant value or with values taken from other matrices.
   *   - With the copy() member you can copy another genericMatrix.
   *   - You can specify, that the genericMatrix should be used just as a
   *     wrapper-object to access external memory regions: useExternData().
   *     To check if a genericMatrix is a wrapper-object you can use 
   *     ownsData().
   * - Iterators
   *   - It is possible to iterate within the genericMatrix by making use of
   *     the genericMatrix iterators.  (see begin() for more information)
   *   - Instead of reverse_iterators as in the STL we use iterators
   *     going backwards, due to faster execution times (see
   *     inverseBegin() for more information)
   *
   * @ingroup gLinearAlgebra
   * @ingroup gAggregate
   */
  template<class T>
  class genericMatrix : public container {
    public:

    /**
     * type of the contained data
     */
    typedef T value_type;

    /**
     * The data can be stored sequentially in the memory or in lines.
     * The "Line" modus will be used when a matrix is created as a
     * submatrix of another "Connected" matrix.
     */
    enum eStoreMode {
      Connected, /**< the data is stored as a single memory block */
      Line       /**< each line has its own memory block.  The iterators
                      do not work on this mode, but you can iterate on each
                      row */
    };

#   ifdef NDEBUG
    /**
     * Iterator type (allows read and write operations)
     *
     * The use of the iterator classes is similar to the iterators of
     * the STL (Standard Template Library). See lti::genericMatrix::begin()
     * for an example .
     *
     * For the debugging version of the iterators, boundary check will be
     * done!  This explains the low speed of the iterators of the debug
     * version.  In the release version, no boundary check will be done,
     * and the iterators are sometimes a factor 10 faster than the
     * debug iterators.
     *
     * The use of the access operator at() is faster than the iterators in the
     * debug version only.  If you need to iterate on a genericMatrix use
     * iterators instead (in the release version iterators are approximately a
     * factor 3 faster than "at()").
     *
     * Iterators do not work on lined matrices.  This means, if you have
     * a submatrix reference of another one (getMode() == Lined), the
     * iterators won't follow the submatrix, but the original data.  This will
     * not change since it would imply reducing the performance for connected
     * matrices, for which the iterators were designed on the first place!
     *
     * \warning Try to use the prefix incremental operator (i.e. ++it) over
     * the postfix operator (i.e. it++) to allow efficient code also in
     * debug-modus!
     */
    typedef T* iterator;

    /**
     * Constant iterator type (allows read-only operations)
     *
     * The use of the iterator classes is similar to the iterators of
     * the STL (Standard Template Library). See lti::genericMatrix::begin()
     * for an example.
     *
     * For the debugging version of the iterators, boundary check will be
     * done!  This explains the low speed of the iterators of the debug
     * version.  In the release version, no boundary check will be done,
     * and the iterators are sometimes a factor 10 faster than the
     * debug iterators.
     *
     * The use of the access operator at() is faster than the iterators in the
     * debug version only.  If you need to iterate on a genericMatrix use
     * iterators instead (in the release version iterators are approximately a
     * factor 3 faster than "at()").
     *
     * Iterators do not work on lined matrices.  This means, if you have
     * a submatrix reference of another one (getMode() == Lined), the
     * iterators won't follow the submatrix but the original data.  This will
     * not change since it would imply reducing the performance for connected
     * matrices, for which the iterators were designed on the first place!
     *
     * \warning Try to use the prefix incremental operator (i.e. ++it) instead
     * of the postfix operator (i.e. it++) to allow efficient code also in
     * debug-modus!
     */
    typedef const T* const_iterator;

#   else

    /**
     * Iterator type (allows read and write operations)
     *
     * The use of the iterator classes is similar to the iterators of
     * the STL (Standard Template Library). See lti::genericMatrix::begin()
     * for an example .
     *
     * For the debugging version of the iterators, boundary check will be
     * done!  This explains the low speed of the iterators of the debug
     * version.  In the release version, no boundary check will be done,
     * and the iterators are sometimes a factor 10 faster than the
     * debug iterators.
     *
     * The use of the access operator at() is faster than the iterators in the
     * debug version only.  If you need to iterate on a genericMatrix use
     * iterators instead (in the release version iterators are approximately a
     * factor 3 faster than "at()").
     *
     * Iterators do not work on lined matrices.  This means, if you have
     * a submatrix reference of another one (getMode() == Lined), the
     * iterators won't follow the submatrix, but the original data.  This will
     * not change since it would imply reducing the performance for connected
     * matrices, for which the iterators were designed on the first place!
     *
     * \warning Try to use the prefix incremental operator (i.e. ++it) over
     * the postfix operator (i.e. it++) to allow efficient code also in
     * debug-modus!
     */
    typedef internal::debugIterator<genericMatrix<T>, false> iterator;

    /**
     * Constant iterator type (allows read-only operations)
     *
     * The use of the iterator classes is similar to the iterators of
     * the STL (Standard Template Library). See lti::genericMatrix::begin()
     * for an example.
     *
     * For the debugging version of the iterators, boundary check will be
     * done!  This explains the low speed of the iterators of the debug
     * version.  In the release version, no boundary check will be done,
     * and the iterators are sometimes a factor 10 faster than the
     * debug iterators.
     *
     * The use of the access operator at() is faster than the iterators in the
     * debug version only.  If you need to iterate on a genericMatrix use
     * iterators instead (in the release version iterators are approximately a
     * factor 3 faster than "at()").
     *
     * Iterators do not work on lined matrices.  This means, if you have
     * a submatrix reference of another one (getMode() == Lined), the
     * iterators won't follow the submatrix but the original data.  This will
     * not change since it would imply reducing the performance for connected
     * matrices, for which the iterators were designed on the first place!
     *
     * \warning Try to use the prefix incremental operator (i.e. ++it) instead
     * of the postfix operator (i.e. it++) to allow efficient code also in
     * debug-modus!
     */
    typedef internal::debugIterator<genericMatrix<T>, 
                                    true> const_iterator;

#   endif

    /**
     * Default constructor creates an empty genericMatrix
     */
    genericMatrix();

    /**
     * Create a \a rows x \a cols genericMatrix with
     * uninitialized elements.
     *
     * \warning This is an interface change with the previous library.
     * It has been done to be consistent with the more basic features
     * of the C++ language.  If you write for example \c int \c c; ,
     * the content of \c c is not defined, and in the same way, if you
     * want a vector with initialized data, you have to specify
     * explicitely the value with which the elements have to be
     * initialized.
     *
     * @param rows number of rows of the genericMatrix
     * @param cols number of columns of the genericMatrix
     */
    genericMatrix(const int rows,const int cols);

    /**
     * Create a \a size.x x \a size.y genericMatrix with
     * uninitialized elements.
     *
     * \warning This is an interface change with the previous library.
     * It has been done to be consistent with the more basic features
     * of the C++ language.  If you write for example \c int \c c; ,
     * the content of \c c is not defined, and in the same way, if you
     * want a vector with initialized data, you have to specify
     * explicitely the value with which the elements have to be
     * initialized.
     *
     * @param size size of the matrix (size.x columns and size.y rows)
     */
    genericMatrix(const ipoint& size);

    /**
     * Create a \a rows x \a cols genericMatrix and
     * initializes all elements with \a iniValue.
     *
     * @param rows number of rows of the genericMatrix
     * @param cols number of columns of the genericMatrix
     * @param iniValue all elements will be initialized with this value
     */
    genericMatrix(const int rows,const int cols,const T& iniValue);

    /**
     * This constructor creates a connected \a size.y x \a size.x
     * GenericMatrix and initializes all elements with \a iniValue
     *
     * @param size lti::ipoint with the size of the genericMatrix
     *             (size.x is the number of columns and 
     *              size.y the number of rows)
     * @param iniValue all elements will be initialized with this value
     */
    genericMatrix(const ipoint& size,const T& iniValue);

    /**
     * Create a \a rows x \a cols genericMatrix and initialize all 
     * elements with the data pointed by \a data.
     *
     * The first \a cols-elements of the data will be copied on the
     * first row, the next ones on the second row and so on.
     *
     * @param rows number of rows of the genericMatrix
     * @param cols number of columns of the genericMatrix
     * @param data pointer to the memory block with the data to be initialized
     *            with.
     */
    genericMatrix(const int rows,const int cols,const T data[]);

    /**
     * Copy constructor.
     *
     * Create this genericMatrix as a connected copy of another
     * genericMatrix.
     *
     * @param other the genericMatrix to be copied.
     */
    genericMatrix(const genericMatrix<T>& other);
    

    /**
     * Copy constructor.
     *
     * create this genericMatrix as a connected copy of another
     * genericMatrix for this const version, the data will be always
     * copied!  It is also possible to create a copy of a
     * submatrix of another genericMatrix.
     *
     * @param other the genericMatrix to be copied.
     * @param fromRow initial row of the other genericMatrix to be copied
     * @param fromCol initial column of the other genericMatrix to be copied
     * @param toRow last row to be copied of the other genericMatrix
     * @param toCol last column to be copied of the other genericMatrix
     *
     * Example:
     * \code
     * lti::genericMatrix<int> m(4,6,0); // integer genericMatrix with 25 
     *                                   // elements
     * // ...
     * // initialize GenericMatrix with:
     * //        0  1  2  3  4  5
     * //        2  1  5  4  0  3
     * //        1  2  1  2  3  2
     * //        3  3  2  1  2  3
     *
     * lti::genericMatrix<int> sm(m,0,1,2,3)  // last line will lead to
     * //                                 following contents in sm:
     * //        1  2  3
     * //        1  5  4
     * //        2  1  2
     * \endcode
     */
    genericMatrix(const genericMatrix<T>& other,
                  const int fromRow,
                  const int fromCol,
                  const int toRow=MaxIndex,
                  const int toCol=MaxIndex);


    /**
     * Copy constructor.
     *
     * Create this genericMatrix as a connected copy of a submatrix of another
     * genericMatrix.
     *
     * @param other the genericMatrix to be copied.
     * @param from initial position in the other genericMatrix to be copied
     * @param to last position to be copied of the other genericMatrix
     *
     * Example:
     * \code
     * lti::genericMatrix<int> m(4,6,0); // integer genericMatrix with 25 
     *                                   // elements
     * // ...
     * // initialize GenericMatrix with:
     * //        0  1  2  3  4  5
     * //        2  1  5  4  0  3
     * //        1  2  1  2  3  2
     * //        3  3  2  1  2  3
     *
     * lti::genericMatrix<int> sm(m,0,2,1,3)  // last line will lead to
     * //                                 following contents in sm:
     * //        1  2  3
     * //        1  5  4
     * //        2  1  2
     * \endcode
     */
    genericMatrix(const genericMatrix<T>& other,
                  const ipoint& from,
                  const ipoint& to);

    
    /**
     * Copy constructor.
     *
     * create this genericMatrix as a connected copy of another genericMatrix
     * taking only the rows indicated by the vector.
     * for this const version, the data will be always copied!
     * Multiple occurence of one row index in \a rows is allowed.
     *
     * @param other the genericMatrix to be copied.
     * @param rows inidices of the rows to be copied
     *
     * Example:
     * \code
     * lti::vector<int> rows(2);
     * // initialize with
     * // 1 3
     * lti::genericMatrix<int> m(4,6,0); // integer genericMatrix with 25
     *                                   // elements
     * // ...
     * // initialize GenericMatrix with:
     * //        0  1  2  3  4  5
     * //        2  1  5  4  0  3
     * //        1  2  1  2  3  2
     * //        3  3  2  1  2  3
     *
     * lti::genericMatrix<int> sm(m,rows)     // last line will lead to
     * //                                 following contents in sm:
     * //        2  1  5  4  0  3
     * //        3  3  2  1  2  3
     * \endcode
     */
    genericMatrix(const genericMatrix<T>& other,
                  const genericVector<int>& rows);

    /**
     * Copy constructor (reference to a submatrix).
     *
     * Creates submatrix of another matrix.
     *
     * if \a copyData == \c true, the new object has its own data
     * (equivalent to previous copy constructor).
     *
     * if \a copyData == \c false, the new object has references to
     * the other matrix, which means that the data is not necessarily
     * consecutive.  (This will not be a connected but a lined matrix)
     *
     * Those algorithms which use direct access to the matrix memory block
     * should check first if the memory lies in a consecutive block!
     * (see getMode())
     *
     * The only way to transfer or copy the lined-referenced property of a
     * submatrix is with the method swap() or detach().
     *
     * @param copyData should the data of the other matrix be copied or not
     * @param other the matrix with the data to be copied or to be shared
     * @param fromRow initial row of the other matrix to be copied
     * @param fromCol initial column of the other matrix to be copied
     * @param toRow last row to be copied of the other matrix
     * @param toCol last column to be copied of the other matrix
     */
    genericMatrix(const bool copyData, genericMatrix<T>& other,
                  const int fromRow=0,
                  const int fromCol=0,
                  const int toRow=MaxIndex,
                  const int toCol=MaxIndex);

    /**
     * Destructor
     */
    virtual ~genericMatrix();

    /**
     * Owns this %object the data?
     * returns <em>false</em> if this genericMatrix contains a reference to
     *         extern data.
     */
    inline bool ownsData() const;

    /**
     * If this object does not own its data, this member will create a
     * new memory buffer with the same data and will make this
     * genericMatrix as its owner.  You can also be sure, that the new
     * memory block will be connected (see also getMode() ).  If this
     * genericMatrix already owns its data nothing happens.
     */
    void restoreOwnership();

    /**
     * Reference to extern data.
     *
     * This member allows the use of this %object as an access-functor for
     * the 'data'. An access to the element at (r,c) is equivalent to
     * data[r*columns() + c].
     * The user must take care for memory allocation and deallocation:
     * this object will never delete the data!.
     * If <em>rows</em> and <em>cols</em> are invalid dimensions, the
     * behaviour will be unpredictible.
     * @param rows number of rows
     * @param cols number of columns
     * @param data a pointer to the memory block to be used
     *
     * For an example see attach()
     */
    void useExternData(const int rows,const int cols,T* data);

    /**
     * Attach extern data to the genericMatrix.
     *
     * This member allows the use of this %object as an access-functor for
     * the 'data'. An access to the element at (r,c) is equivalent to
     * data[r*columns() + c].
     * If <em>rows</em> and <em>cols</em> are invalid dimensions, the
     * behaviour will be unpredictible.
     *
     * The memory will be administrated by this genericMatrix object,
     * and may be deleted if required (genericMatrix deleted or
     * resized!).  The user should not try to manipulate the memory
     * allocation of the data after the attachment!  See also
     * useExternData().
     *
     * @param rows number of rows
     * @param cols number of columns
     * @param data a pointer to the memory block to be used
     *
     * Example:
     * \code
     * lti::genericMatrix<int> myMat;
     * int block1[25];
     * int* block2;
     * block2 = new int[25];
     *
     * myMat.useExternData(5,5,block1); // ok
     * myMat.attach(5,5,block1); // wrong!!! genericMatrix will try 
     *                           // to manipulate stack memory: 
     *                           // DO NOT DO THIS!!!!!
     * myMat.attach(5,5,block2); // ok!  but do not try to delete the memory
     *                           //      block2!!
     * \endcode
     */
    void attach(const int rows,const int cols,T* data);

    /**
     * Free the data of this object and hand it over to the
     * "receiver". The value of ownsData is also transfered to the
     * receiver. (see Note).
     *
     * This function makes a "memory block transfusion" to another
     * genericMatrix.  It is a very efficient way to make a copy of
     * this genericMatrix, if you don't need the source data anymore!
     *
     * If the current matrix does not own its data, neither will the receiver
     * matrix, which will also be just a wrapper of the same data.
     *
     * If the current matrix is lined, then the other matrix will end as an
     * exact copy if this one, and this one will be emptied after the
     * detachment.  In other words, it works similar like a swap() followed by
     * clear().  In extremely time critical situations you may want to use
     * swap() as it does not need to deallocate any memory of the receiver and
     * is therefore a little bit faster.
     * 
     * Please take special care to the memory management issues involved if the
     * current object does not own its data, as its control will still be
     * yours.
     *
     * At the end of the detachment, this genericMatrix will be empty.
     *
     * @param receiver the genericMatrix which will receive the memory
     *        block.  All data of that genericMatrix will be first deleted!
     */
    void detach(genericMatrix<T>& receiver);

    /**
     * Free the data of this object and hand it over to the
     * "receiver". The value of ownsData is also transfered to the
     * receiver. (see Note).
     *
     * This function makes a "memory block transfusion" to a vector by
     * concatenating the rows of the genericMatrix.  It is a very efficient
     * way to move the data of this genericMatrix into a vector, if you don't
     * need the source data anymore!
     *
     * \b Note: This method will fail if this genericMatrix is not
     * connected. Also take care that if the attach() or
     * useExternData() methods of this genericMatrix have been called before
     * detachment, the same rules for memory management apply now for
     * the receiver.
     *
     * At the end of the detachment, this genericMatrix will be empty.
     * @param receiver the genericMatrix which will receive the memory block.
     *                 All data of that genericMatrix will be first deleted!
     */
    void detach(genericVector<T>& receiver);

    /**
     * Exchange (in a fast way) the data between this and the other 
     * genericMatrix.
     *
     * Similar to detach(), this method will exchange the complete memory
     * blocks, avoiding an element-wise copy.
     * @param other the genericMatrix with which the data will be exchanged.
     */
    void swap(genericMatrix<T>& other);

    /**
     * Data storage mode.
     *
     * Returns the data storage mode, which can be:
     * - \a Connected if the memory is a single block, or
     * - \a Line      if the memory of each line is allocated
     *                          in different places.
     *
     * For the lined-matrices the interators will not work.  You can however
     * iterate on each individual row, which are always connected.
     *
     * The only possible way to get a lined-matrix is as a submatrix of
     * another one, using the appropriate constructor:
     * genericMatrix(const bool, genericMatrix<T>&,const int,const int,
     * const int,const int)
     */
    inline eStoreMode getMode() const;

    /**
     * Number of rows of the genericMatrix
     */
    inline int rows() const;

    /**
     * Number of columns of the genericMatrix
     */
    inline int columns() const;

    /**
     * Index of the last row (rows()-1)
     */
    inline int lastRow() const;

    /**
     * Index of the last columns (columns()-1)
     */
    inline int lastColumn() const;

    /**
     * Number of "physical" rows of the matrix.
     *
     * @return If this is a \a Connected Matrix, (see getMode()),
     * this member returns the same value as rows().
     *
     * If this is a \a Line Matrix, this value is bigger or equal
     * than rows().  If this was created with the copy constructor for a
     * submatrix with "no copy data", this value will return the size of the
     * original matrix.
     */
    inline int metaRows() const;

    /**
     * Number of "physical" columns of the matrix.
     *
     * @return If this is a \a Connected Matrix, (see getMode()),
     * this member returns the same value as columns().
     *
     * If this is a \a Line Matrix, this value is bigger or equal
     * than columns().  If this was created with the copy constructor for a
     * submatrix with "no copy data", this value will return the number of
     * columns of the original matrix.
     */
    inline int metaColumns() const;

    /**
     * Return type of the size() member
     */
    typedef ipoint size_type;

    /**
     * Returns the size of the %genericMatrix in a lti::point structure.
     *
     * @return lti::point with the number of columns in its
     *         \a x coordinate and the number of rows in its
     *         \a y coordinate.
     */
    inline const size_type& size() const;

    /**
     * Return iterator to the begin of the genericMatrix.
     *
     * The use of the interators is similar to the iterators of the
     * Standard Template Library (STL).
     * If you need to iterate on all elements of the genericMatrix, you can
     * use following code:
     * \code
     * int tmp,accu;                  // a temporal variable
     * lti::genericMatrix<int> myMat(10,8,1); // a vector with 10 elements
     * lti::genericMatrix<int>::iterator it;  // an iterator
     *
     * for (it=myMat.begin();it!=myMat.end();it++) {
     *   tmp = *it;                   // tmp has value of element pointed
     *                                // by the iterator.
     *   accu += tmp;
     *   (*it) = accu;                // change the value in the genericMatrix.
     * }
     * \endcode
     * Please note that if you define \c it as a const_iterator,
     * you can not do something like <code>*it=accu</code>.
     */
    inline iterator begin();

    /**
     * Return first element of the genericMatrix as a const_iterator.
     *
     * Note that you can not change the values of the genericMatrix
     * elements when you use a const_iterator. See also begin() for an example
     */
    inline const_iterator begin() const;

    /**
     * returns iterator to the end of the genericMatrix
     */
    inline iterator end();

    /**
     * returns iterator to the end of the genericMatrix
     */
    inline const_iterator end() const;


    /**
     * This method returns an iterator that points to the \b last
     * valid element of the genericMatrix. It is used for inverse order
     * iteration through the genericMatrix using normal iterators (as opposed
     * to reverse iterators). This has the advantage that iterators
     * going from front to end and in the inverse direction are the
     * same and can thus be compared, copied etc. Further the
     * implementation of reverse_iterators is not as fast as that of
     * iterators and thus not desired in the LTI-Lib.
     *
     * See lti::genericVector<T>::inverseBegin() for an example.
     */
    inline iterator inverseBegin();

    /**
     * This method returns an iterator that points to the \b last
     * valid element of the genericMatrix. See inverseBegin() for more details.
     */
    inline const_iterator inverseBegin() const;

    /**
     * This method returns an iterator that points to the element \b
     * before the \b first valid element of the genericMatrix. It is used to
     * mark the end for inverse order iteration through the genericMatrix
     * using normal iterators (as opposed to reverse iterators). This
     * has the advantage that iterators going from front to end and in
     * the inverse direction are the same and can thus be compared,
     * copied etc.
     */
    inline iterator inverseEnd();

    /**
     * This method returns an iterator that points to the element \b
     * before the \b first valid element of the genericMatrix.
     */
    inline const_iterator inverseEnd() const;

    /**
     * Change the dimensions of the genericMatrix.
     *
     * @param newRows new number of rows
     * @param newCols new number of columns
     * @param iniValue the initialization value.
     * @param resizeType specifies what should happen with the data of the
     *                   resized matrix.
     *
     * For example:
     * \code
     *   lti::genericMatrix<int> myMat;  // creates empty genericMatrix
     *   myMat.resize(5,5,0);     // genericMatrix with 5x5 elements 
     *                            // initialized with 0
     *   myMat.resize(10,7,2);    // genericMatrix has now 10x7 elements: the
     *                            // submatrix 5x5 at (0,0) has still 0s
     *                            // and the rest have a 2
     *   myMat.resize(20,10,0,lti::AllocateOnly); // now the genericMatrix has 
     *                                            // 20 elements but their
     *                                            // values are unknown.
     *   myMat.resize(5,5,1,Copy); // the genericMatrix has now 5x5
     *                             // elements all initialized with 1
     * \endcode
     *
     * If the resize is possible (see useExternData()), this %object
     * will always owns the data!
     *
     * @see eResizeType
     */
    void resize(const int newRows,const int newCols,
                const T& iniValue,
                const eResizeType resizeType=CopyAndInit);

    /**
     * Change the dimensions of the genericMatrix.
     *
     * @param newDim   new dimensions of the genericMatrix
     * @param iniValue the initialization value.
     * @param resizeType specifies what should happen with the data of the
     *                   resized matrix.
     *
     * If the resize is possible (see useExternData()), this %object
     * will always own the data!
     *
     * This is equivalent to call
     *   resize(newDim.y,newDim.x,iniValue,resizeType)
     *
     * @see eResizeType
     */
    inline void resize(const ipoint& newDim,
                       const T& iniValue,
                       const eResizeType resizeType=CopyAndInit);

    /**
     * Change the dimensions of the genericMatrix.
     *
     * @param newRows new number of rows
     * @param newCols new number of columns
     *
     * The old values will be copied, but the new data will be kept
     * uninitialized.  In principle this is an alias for 
     * \c resize(newRows,newCols,T(),Copy).
     *
     * If the resize is possible (see useExternData()), this %object
     * will always own the data!
     */
    inline void resize(const int newRows,const int newCols);

    /**
     * Change the dimensions of the genericMatrix.
     *
     * @param newSize new size of the matrix.
     *
     * The old values will be copied, but the new data will be kept
     * uninitialized.  In principle this is an alias for 
     * \c resize(newRows,newCols,T(),Copy).
     *
     * If the resize is possible (see useExternData()), this %object
     * will always own the data!
     */
    inline void resize(const ipoint& newSize);

    /**
     * Change the dimensions of the genericMatrix and leave ALL data
     * uninitialized.
     *
     * @param newRows new number of rows
     * @param newCols new number of columns
     *
     * The old values will be discarded and the new data will be kept
     * uninitialized.  In principle this is an alias for 
     * \c resize(newRows,newCols,T(),AllocateOnly).
     *
     * If the resize is possible (see useExternData()), this %object
     * will always own the data!
     */
    inline void allocate(const int newRows,const int newCols);

    /**
     * Change the dimensions of the genericMatrix and leave ALL data
     * uninitialized.
     *
     * @param newSize new size of the matrix.
     *
     * The old values will be discarded and the new data will be kept
     * uninitialized.  In principle this is an alias for 
     * \c resize(newSize.y, newSize.x, T(), AllocateOnly).
     *
     * If the resize is possible (see useExternData()), this %object
     * will always own the data!
     */
    inline void allocate(const ipoint& newSize);

    /**
     * Change the dimensions of the genericMatrix and initialize ALL data
     * with the given value.
     *
     * The old values will be discarded and all data will be initialized with
     * initValue.  In principle this is an alias for \c
     * resize(newRows, newCols, initValue, Init).
     *
     * If the resize is possible (see useExternData()), this %object
     * will always own the data!
     *
     * @param newRows new number of rows
     * @param newCols new number of columns
     * @param initValue value to be assigned to all matrix elements
     */
    inline void assign(const int newRows,
                       const int newCols,
                       const T& initValue);

    /**
     * Change the dimensions of the genericMatrix and initialize ALL data
     * with the given value.
     *
     * The old values will be discarded and all data will be initialized with
     * initValue.  In principle this is an alias for \c
     * resize(newSize.y, newSize.x, initValue, Init).
     *
     * If the resize is possible (see useExternData()), this %object
     * will always own the data!
     * 
     * @param newSize new size of the matrix.
     * @param initValue value to be assigned to all matrix elements
     */
    inline void assign(const ipoint& newSize,
                       const T& initValue);

    /**
     * Clears the genericMatrix (at the end this will be an empty
     * genericMatrix)
     */
    void clear();

    /**
     * Returns true if the genericMatrix is empty
     */
    inline bool empty() const;

    /**
     * Fills genericMatrix elements with \a iniValue.
     * The fill "area" is limited by \a fromCol,\a toCol,
     * \a fromRow and \a toRow.
     * If these values are out of bounds, they will be (internally)
     * adjusted to correct values.
     *
     * @param iniValue the elements will be initialized with this value
     * @param fromRow  first row of the submatrix to be filled
     * @param fromCol  first column of the submatrix to be filled
     * @param toRow    last row of the submatrix to be filled
     * @param toCol    last column of the submatrix to be filled
    */
    void fill(const T& iniValue,
              const int fromRow=0,
              const int fromCol=0,
              const int toRow=MaxIndex,
              const int toCol=MaxIndex);

    /**
     * Fills genericMatrix elements with \a iniValue.
     * The fill "area" is limited by \a from and \a to
     * points
     * If these values are out of bounds, they will be (internally)
     * adjusted to correct values.
     *
     * @param iniValue the elements will be initialized with this value
     * @param from first position of the submatrix to be filled
     * @param to   last row of the submatrix to be filled
    */
    inline void fill(const T& iniValue,
                     const ipoint& from,
                     const ipoint& to=ipoint(MaxIndex,MaxIndex));

    /**
     * Fills genericMatrix elements with \a iniValue.
     * The fill "area" is limited by \a window.
     * If these values are out of bounds, they will be (internally)
     * adjusted to correct values.
     * @param iniValue the elements will be initialized with this value
     * @param window   the window to be filled
     */
    inline void fill(const T& iniValue,const irectangle& window);

    /**
     * Fills genericMatrix elements with the data pointed by \a data.
     * The fill "area" is limited by \a fromCol,\a toCol,
     * \a fromRow and \a toRow.
     * If these values are out of bounds, they will be (internally)
     * adjusted to correct values.
     *
     * WARNING: Syntax changed for the parameters:  the old version was
     *          fromRow,toRow, fromCol,toCol.
     *          Now the order is:
     *          fromRow,FROMCOL,TOROW,toCol.  This allow to give the
     *          first coordinates only!
     *
     * @param data     pointer to the data to be copied.
     * @param fromRow  first row of the submatrix to be filled
     * @param fromCol  first column of the submatrix to be filled
     * @param toRow    last row of the submatrix to be filled
     * @param toCol    last column of the submatrix to be filled
     */
    void fill(const T data[],
              const int fromRow=0,
              const int fromCol=0,
              const int toRow=MaxIndex,
              const int toCol=MaxIndex);

    /**
     * Fills genericMatrix elements with the data pointed by \a data.
     * The fill "area" is limited by \a fromCol,\a toCol,
     * \a fromRow and \a toRow.
     * If these values are out of bounds, they will be (internally)
     * adjusted to correct values.
     *
     * @param data  pointer to the data to be copied.
     * @param from  first position of the submatrix to be filled
     * @param to    last position of the submatrix to be filled
     */
    inline void fill(const T data[],
                     const ipoint& from,
                     const ipoint& to=ipoint(MaxIndex,MaxIndex));

    /**
     * Fills genericMatrix elements with \a iniValue.
     * The fill "area" is limited by \a window.
     * If these values are out of bounds, they will be (internally)
     * adjusted to correct values.
     * @param data   pointer to the data to be copied
     * @param window the window to be filled
     */
    inline void fill(const T data[],const irectangle& window);

    /**
     * Fills this genericMatrix between the "from's" and "to's" with
     * the contents of the genericMatrix \a mat starting at
     * \a startAtRow and \a startAtCol
     *
     * @param mat      the genericMatrix with the data to be copied
     * @param fromRow  first row of the submatrix to be filled
     * @param fromCol  first column of the submatrix to be filled
     * @param toRow    last row of the submatrix to be filled
     * @param toCol    last column of the submatrix to be filled
     * @param startAtRow starting row of \a mat where the data is
     *                   located.
     * @param startAtCol starting column of \a mat where the data
     *                   is located.
     */
    void fill(const genericMatrix<T>& mat,
              const int fromRow=0,
              const int fromCol=0,
              const int toRow=MaxIndex,
              const int toCol=MaxIndex,
              const int startAtRow=0,
              const int startAtCol=0);

    /**
     * Fills this genericMatrix between the "from's" and "to's" with
     * the contents of the genericMatrix \a mat starting at
     * \a startAtRow and \a startAtCol
     *
     * @param mat     the genericMatrix with the data to be copied
     * @param from    first position of the submatrix to be filled
     * @param to      last position of the submatrix to be filled
     * @param startAt starting position of \a mat where the data is
     *                located.
     */
    inline void fill(const genericMatrix<T>& mat,
                     const ipoint& from,
                     const ipoint& to=ipoint(MaxIndex,MaxIndex),
                     const ipoint& startAt=ipoint(0,0));

    /**
     * Fills the region of this genericMatrix specified by
     * \a window with the contents of the genericMatrix
     * \a mat starting at \a start.  If these
     * values are out of bounds, they will be (internally) adjusted to
     * correct values.
     *
     * @param mat    pointer to the data to be copied
     * @param window the window to be filled
     * @param start  the start position of the region to be copied of the
     *               genericMatrix \a mat
     */
    inline void fill(const genericMatrix<T>& mat,
                     const irectangle& window,
                     const ipoint& start=ipoint(0,0));

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
     * Access element at the given row and column
     * @param row the row of the element
     * @param col the column of the element
     * @return a reference to the genericMatrix element
     */
    inline T& at(const int row, const int col);

    /**
     * Read-only access at the given row and column
     * @param row the row of the element
     * @param col the column of the element
     * @return a reference to the genericMatrix element
     */
    inline const T& at(const int row, const int col) const;

    /**
     * Access operator of genericMatrix element as a point in a 2D-Map
     *
     * @param p position of the element (this is equivalent to at(p.y,p.x))
     * @return a reference to the genericMatrix element
     */
    inline T& at(const ipoint& p);

    /**
     * Const access operator of genericMatrix element as a point in a 2D-Map
     *
     * @param p position of the element (this is equivalent to at(p.y,p.x))
     * @return a const reference to the vector element
     */
    inline const T& at(const ipoint& p) const;

    /**
     * Access element at the given position.
     *
     * (Can only be used in connected matrices.  See constructors for more
     * information on this.)
     *
     * With this operator the genericMatrix can be accessed as a
     * vector, where the rows of the genericMatrix are concatenated.
     * The access to the genericMatrix with at(row,col) is equivalent
     * to elem(row*columns()+col).
     *
     * @param pos the index of the element of the genericMatrix
     *
     * @return a reference to the genericMatrix element
     */
    inline T& elem(const int pos);

    /**
     * Access element at the given position.
     *
     * (Can only be used in connected matrices.  See constructors for more
     * information on this.)
     *
     * With this operator the genericMatrix can be accessed as a
     * vector, where the rows of the genericMatrix are concatenated.
     * The access to the genericMatrix with at(row,col) is equivalent
     * to elem(row*columns()+col).
     *
     * @param pos the index of the element of the genericMatrix
     *
     * @return a reference to the genericMatrix element
     */
    inline const T& elem(const int pos) const;

    /**
     * Return genericMatrix-row as a vector.
     *
     * This method works fast, since it returns a reference to the row vector.
     * The data will NOT be copied.
     *
     * @param row the row to be accessed
     * @return a reference to the vector row
     */
    inline genericVector<T>& getRow(const int row);

    /**
     * Return genericMatrix-row as a const vector.
     * This method works fast, since it returns a reference to the row vector.
     * The data will NOT be copied.
     *
     * @param row the row to be accessed
     * @return a const reference to the vector row
     */
    inline const genericVector<T>& getRow(const int row) const;

    /**
     * Alias for getRow()
     */
    inline genericVector<T>& operator[](const int row);

    /**
     * Alias for getRow()
     */
    inline const genericVector<T>& operator[](const int row) const;

    /**
     * Copy a row vector in the given parameter.
     *
     * This method copies the data of a given row of the genericMatrix
     * in the given vector.
     *
     * @param row the number of the row to be copied
     * @param theRow the vector, where the data will be copied
     * @see getRow()
     */
    inline void getRowCopy(const int row,genericVector<T>& theRow) const;

    /**
     * Return genericMatrix-row as a vector.
     * This method copies the data of the genericMatrix, therefore is not as
     * fast as getRow()
     * @param row the number of the row to be copied
     * @return a vector with the contents of the row of the genericMatrix
     */
    inline genericVector<T> getRowCopy(const int row) const;

    /**
     * Return genericMatrix-column as a vector.
     * This method copies the data of the genericMatrix, therefore is not as
     * fast as getRow()
     * @param col the number of the column to be copied
     * @param theCol a vector, where the column vector of the genericMatrix
     *               should be copied.
     */
    void getColumnCopy(const int col,genericVector<T>& theCol) const;

    /**
     * Return genericMatrix-column as a vector.
     *
     * This method copies the data of the genericMatrix, therefore is not as
     * fast as getRow()
     * @param col the number of the column to be copied
     * @return a vector with the contents of the column of the genericMatrix
     */
    inline genericVector<T> getColumnCopy(const int col) const;

    /**
     * Return the diagonal elements of the genericMatrix as vector.
     *
     * This method copies the diagonal elements of the genericMatrix into
     * the vector. If the genericMatrix is non-symmetrical, the vector will
     * be of dimension min(rows(),columns()).
     * @param diag a vector, where the diagonal of the genericMatrix
     *             should be copied.
     */
    void getDiagonal(genericVector<T>& diag) const;

    /**
     * Return the diagonal elements of the genericMatrix as vector.
     * This method copies the diagonal elements of the genericMatrix into
     * the vector. If the genericMatrix is non-symmetrical, the vector will
     * be of dimension min(rows(),columns()).
     * @return a vector with the diagonal elements of the genericMatrix.
     */
    inline genericVector<T> getDiagonal() const;

    /**
     * Sets the diagonal of the genericMatrix to the values given in
     * the genericVector \a diag. Let \c r be the number of rows and
     * \c c be the number of columns of the matrix. Then \c minRC is
     * \c min(r,c). Also let \c d be the size of \a diag. Only \c
     * min(minRC,d) values are copied from \a diag. If \c d is smaller
     * than \c minRC the remaining values on the diagonal are left
     * untouched. The copying always starts at (0,0) of the matrix.
     *
     * @param diag values to be copied into the diagonal of the matrix
     */
    void setDiagonal(const genericVector<T>& diag);

    /**
     * Copy the data of a vector in a given row.
     *
     * @param row the row that receives the data.
     * @param theRow the vector with the data to be copied
     */
    inline void setRow(const int row,const genericVector<T>& theRow);

    /**
     * Copy the data of a vector in a given column
     * @param col the column that receives the data.
     * @param theCol the vector with the data to be copied
     */
    void setColumn(const int col,const genericVector<T>& theCol);

    /**
     * Assigment operator.
     *
     * Copy the contents of \a other in this object.
     *
     * The result of the copy is always a connected genericMatrix.
     * I.e. you cannot copy the sub-genericMatrix property of another
     * genericMatrix.
     *
     * @param other the other genericMatrix to be copied
     */
    genericMatrix<T>& copy(const genericMatrix<T>& other);

    /**
     * Assigment operator.
     *
     * Copy the contents of a submatrix of \a other into this object.  The
     * content of the current object is removed and at the end it will contain
     * only the submatrix.
     *
     * The result of this copy is always a connected genericMatrix.
     * I.e. you cannot copy the sub-genericMatrix property of another
     * genericMatrix.
     *
     * If you need a submatrix as reference of another matrix consider also
     * using the constructor genericMatrix(const bool,genericMatrix<T>&,
     * const int,const int,const int,const int).
     *
     * @param other the other genericMatrix to be copied
     * @param fromRow initial row of the other genericMatrix to be copied
     * @param fromCol initial column of the other genericMatrix to be copied
     * @param toRow last row to be copied of the other genericMatrix
     * @param toCol last column to be copied of the other genericMatrix
     */
    genericMatrix<T>& copy(const genericMatrix<T>& other,
                           const int fromRow,
                           const int fromCol=0,
                           const int toRow=MaxIndex,                           
                           const int toCol=MaxIndex);


    /**
     * Assigment operator.
     *
     * Copy the contents of a submatrix of \a other into this object.  The
     * content of the current object is removed and at the end it will contain
     * only the submatrix.
     *
     * The result of this copy is always a connected genericMatrix.
     * I.e. you cannot copy the sub-genericMatrix property of another
     * genericMatrix.
     *
     * If you need a submatrix as reference of another matrix consider also
     * using the constructor genericMatrix(const bool,genericMatrix<T>&,
     * const int,const int,const int,const int).
     *
     * @param other the other genericMatrix to be copied
     * @param from initial submatrix indices (x,y) to be copied
     * @param to   last submatrix indices (x,y) to be copied (inclusive)
     */
    inline genericMatrix<T>& copy(const genericMatrix<T>& other,
                                  const ipoint& from,
                                  const ipoint& to);

    /**
     * Assigment operator.
     *
     * Copy the contents of a submatrix of \a other into this object.  The
     * content of the current object is removed and at the end it will contain
     * only the copy of the submatrix.
     *
     * The result of the copy is always a connected genericMatrix.
     * I.e. you cannot copy the sub-genericMatrix property of another
     * genericMatrix.
     *
     * If you need a submatrix as reference of another matrix consider also
     * using the constructor genericMatrix(const bool,genericMatrix<T>&,
     * const int,const int,const int,const int).
     *
     * @param other the other genericMatrix to be copied
     * @param window rectangle define the copy area
     */
    inline genericMatrix<T>& copy(const genericMatrix<T>& other,
                                  const irectangle& window);

    /**
     * Assigment operator.
     *
     * Copy the contents of the specified rows/columns of
     * \a other into this object. Multiple occurence of one
     * row/column index in \a idx is allowed. If the
     * argument \b rows is true, \b idx specifies rows, if false \b
     * idx specifies columns.
     *
     * The result of the copy is always a connected genericMatrix.
     * I.e. you cannot copy the sub-genericMatrix property of another
     * genericMatrix.
     *
     * @param other the other genericMatrix to be copied
     * @param idx indices of the rows to be copied.
     * @param rows if true works on rows, else on columns
     */
    genericMatrix<T>& copy(const genericMatrix<T>& other,
                           const genericVector<int>& idx,
                           bool rows=true);

    /**
     * Copy the \a other genericMatrix by casting each of
     * its elements
     *
     * @param other The genericMatrix to be casted
     *
     * Example:
     * \code
     *   lti::genericMatrix<int> matA(10,10,1);// a genericMatrix of integers
     *   lti::genericMatrix<double> matB;      // a genericMatrix of doubles
     *
     *   matB.castFrom(matA);          // this will copy matA in matB!!
     * \endcode
     */
    template<typename U>
    genericMatrix<T>& castFrom(const genericMatrix<U>& other);

    /**
     * This is just an alias for copy(const genericMatrix<T>&) to facilitate
     * generic programming.
     *
     * @param other The genericMatrix to be copied.
     */
    genericMatrix<T>& castFrom(const genericMatrix<T>& other);

    /**
     * Copy a submatrix of the \a other genericMatrix by casting each of
     * its elements.
     *
     * @param other The genericMatrix to be casted
     * @param fromRow initial row of the other genericMatrix to be copied
     * @param fromCol initial column of the other genericMatrix to be copied
     * @param toRow last row to be copied of the other genericMatrix
     * @param toCol last column to be copied of the other genericMatrix
     *
     * @return a reference to the current casted matrix.
     */
    template<typename U>
    genericMatrix<T>& castFrom(const genericMatrix<U>& other,
                               const int fromRow,
                               const int fromCol=0,
                               const int toRow=MaxIndex,
                               const int toCol=MaxIndex);

    /**
     * This is just an alias for copy(const genericMatrix<T>&, const int
     * fromRow, const int fromCol, const int toRow, const int toCol) to
     * facilitate generic programming.
     *
     * @param other The genericMatrix to be copied.
     * @param fromRow initial row of the other genericMatrix to be copied
     * @param fromCol initial column of the other genericMatrix to be copied
     * @param toRow last row to be copied of the other genericMatrix
     * @param toCol last column to be copied of the other genericMatrix
     *
     * @return a reference to the current casted matrix.
     */
    genericMatrix<T>& castFrom(const genericMatrix<T>& other,
                               const int fromRow,
                               const int fromCol=0,
                               const int toRow=MaxIndex,
                               const int toCol=MaxIndex);

    /**
     * Returns the name of this type.
     */
    virtual const std::string& name() const;

    /**
     * Create a clone of this genericMatrix
     * @return a pointer to a copy of this genericMatrix
     */
    virtual genericMatrix<T>* clone() const;

    /**
     * Create a new instance of genericMatrix
     * @return a pointer to a new instance of genericMatrix
     */
    virtual genericMatrix<T>* newInstance() const;

    /**
     * Compare this genericMatrix with other.
     *
     * @param other the other genericMatrix to be compared with
     * @return true if both matrices have the same elements and same size
     */
    bool equals(const genericMatrix<T>& other) const;

    /**
     * Compare the contents of each element of this genericMatrix with the
     * other one.  It assumes the type T can be compared using the
     * operator==.
     *
     * @param other the other genericMatrix to be compared with
     * @return true if both matrices have the same elements and same size
     */
    inline bool operator==(const genericMatrix<T>& other) const;

    /**
     * Assigment operator (alias for copy(other)).
     *
     * @param other the genericMatrix to be copied
     * @return a reference to the actual genericMatrix
     */
    inline genericMatrix<T>& operator=(const genericMatrix<T>& other);

    /**
     * \name Apply Methods
     *
     * Following methods are used to apply simple functions to each element
     * of the vector.
     */
    //@{

    /**
     * Applies a C-function to each element of the genericMatrix.
     * @param function a pointer to a C-function
     * @return a reference to the actual genericMatrix
     */
    genericMatrix<T>& apply(T (*function)(T));

    /**
     * Applies a C-function to each element of the other genericMatrix
     * @param other the genericMatrix which elements will go through the given
     *              function.
     * @param function a pointer to a C-function
     * @return a reference to the actual genericMatrix
     */
    genericMatrix<T>& apply(const genericMatrix<T>& other,T (*function)(T));

    /**
     * Applies a C-function to each element of the genericMatrix.
     * @param function a pointer to a C-function
     * @return a reference to the actual genericMatrix
     */
    genericMatrix<T>& apply(T (*function)(const T&));

    /**
     * Applies a C-function to each element of the other genericMatrix
     * @param other the genericMatrix which elements will go through the given
     *              function.
     * @param function a pointer to a C-function
     * @return a reference to the actual genericMatrix
     */
    genericMatrix<T>& apply(const genericMatrix<T>& other,
                            T (*function)(const T&));

    /**
     * A two-parameter C-function receives the i-th elements of this
     * and the given genericMatrix and the result will be left in this
     * genericMatrix.  Note that both matrices must have the same size!
     * @param other the second genericMatrix to be considered (the first
     *              genericMatrix will be this object!)
     * @param function a pointer to C-function with two parameters
     * @return a reference to the actual genericMatrix
     */
    genericMatrix<T>& apply(const genericMatrix<T>& other,
                            T (*function)(const T&,const T&));

    /**
     * A two-parameter C-function receives the i-th elements of this
     * and the given genericMatrix and the result will be left in this
     * genericMatrix.  Note that both matrices must have the same size!
     * @param other the second genericMatrix to be considered (the first
     *              genericMatrix will be this object!)
     * @param function a pointer to C-function with two parameters
     * @return a reference to the actual genericMatrix
     */
    genericMatrix<T>& apply(const genericMatrix<T>& other,
                            T (*function)(T,T));

    /**
     * A two-parameter C-function receives the i-th elements of both given
     * matrices and leaves the result here.
     * Note that both matrices must have the same size!
     * @param a the first genericMatrix
     * @param b the second genericMatrix
     * @param function a pointer to C-function with two parameters
     * @return a reference to the actual genericMatrix
     */
    genericMatrix<T>& apply(const genericMatrix<T>& a,
                     const genericMatrix<T>& b,
                     T (*function)(const T&,const T&));

    /**
     * A two-parameter C-function receives the i-th elements of both given
     * matrices and leaves the result here.
     * Note that both matrices must have the same size!
     * @param a the first genericMatrix
     * @param b the second genericMatrix
     * @param function a pointer to C-function with two parameters
     * @return a reference to the actual genericMatrix
     */
    genericMatrix<T>& apply(const genericMatrix<T>& a,
                     const genericMatrix<T>& b,
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
     * Allocate \a n number of rows or the appropriate type.
     */
    inline virtual genericVector<T>* allocRows(const int n);

    /**
     * Size of the matrix as point
     */
    ipoint theSize_;

    /**
     * Index of the last row
     */
    int lastRowIdx_;

    /**
     * Index of the last column
     */
    int lastColIdx_;

    /**
     * Number of rows the "physical" matrix
     * (always bigger than the dimensions of the "active" matrix).
     */
    int metaNumRows_;

    /**
     * Number of columns of the "physical" matrix
     * (always bigger than the dimensions of the "active" matrix).
     */
    int metaNumColumns_;

    /**
     * Size of theElements
     */
    int totalSize_;

    /**
     * Indicates if \a theElements points to own data or to
     * external data.
     */
    bool ownData_;

    /**
     * Indicate if \a theElements points to consecutive memory
     * or to "sparse" memory
     */
    eStoreMode mode_;

    /**
     * Pointer to the elements of the genericMatrix
     */
    T* theElements_;

    /**
     * Table of pointers to the rows
     */
    genericVector<T>* rowAddressTable_;
  };
}

namespace std {
  /*
   * Outputs the elements of the vector on a stream
   */
  template <class T>
  ostream& operator<<(ostream& s,const lti::genericMatrix<T>& amat);
}

#include "ltiGenericMatrix_inline.h"
#include "ltiGenericMatrix_template.h"

#else
#include "ltiGenericMatrix_template.h"
#endif

