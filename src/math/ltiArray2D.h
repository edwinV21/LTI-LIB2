/*
 * Copyright (C) 2007 by Pablo Alvarado
 * 
 * This file is part of the LTI-Computer Vision Library 2 (LTI-Lib-2)
 *
 * The LTI-Lib-2 is free software; you can redistribute it and/or
 * modify it under the terms of the BSD License.
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the authors nor the names of its contributors may be
 *    used to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/** 
 * \file   ltiArray2D.h
 *         Contains the template class lti::array2D<T>.
 * \author Pablo Alvarado
 * \date   28.04.00
 *
 * revisions ..: $Id: ltiArray2D.h,v 1.3 2012-09-16 05:22:59 alvarado Exp $
 */

#ifndef _LTI_ARRAY_2_D_H_
#define _LTI_ARRAY_2_D_H_

#include <vector>
#include "ltiArray.h"
#include "ltiTypes.h"
#include "ltiMatrix.h"
#include "ltiTypeInfo.h"

#include "ltiKernel1D.h" 

namespace lti {
  /**
   * Two-dimensional container, that can be address with negative and possitive
   * indices.
   *
   * @todo The access time can be considerably reduced using the same
   *       technique as with the 1D arrays, i.e. using a pointer to
   *       the (0,0) cell, which can then be accessed directly with
   *       the negative * values.  Some additional thoughts about this
   *       have to be done.
   */
  template<typename T>
  class array2D : public matrix<T> {
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
     * Default constructor
     */
    array2D();   

    /**
     * Create a new container of indicated size and initial value. The offset
     * is kept at zero.
     *
     * @param rows number of rows
     * @param columns number of columns
     * @param init initial value of all elements
     */
    array2D(const int rows,
            const int columns,
            const T& init);

    /**
     * Create a new unintialized container of indicated size. The offset is
     * kept at zero.
     *
     * \warning This is an interface change with the previous library.  It has
     * been done to be consistent with the more basic features of the C++
     * language.  If you write for example "int c;", the content of \c c is
     * not defined, and in the same way, if you want a kernel1D with 
     * initialized data, you have to specify explicitely the value with 
     * which the elements have to be initialized.

     * @param nbRows number of rows
     * @param nbColumns number of columns
     */
    array2D(const int nbRows,
            const int nbColumns);

    /**
     * Create a new container with indicated indices.
     *
     * @param fromRow index of the lower row of the kernel
     * @param fromCol index of the lower column of the kernel
     * @param toRow index of the higher row of the kernel
     * @param toCol index of the higher column of the kernel
     * @param iniValue the initial value for the elements of the kernel
     *
     * You can initialize a 5x3 container to filter a lti::channel with
     * following code:
     *
     * \code
     * lti::array2D<channel::value_type> aKernel(-1,-2,1,2,0);
     * aKernel.at(-1,-2)=0.2;
     * aKernel.at(-1,-1)=0.2;
     * aKernel.at(0,0)=0.2;
     * aKernel.at(1,1)=0.2;
     * aKernel.at(1,2)=0.2;
     * \endcode
     */
    array2D(const int fromRow,
            const int fromCol,
            const int toRow,
            const int toCol,
            const T& iniValue);

    /**
     * Create a new uninitialized container with indicated indices.
     *
     * @param fromRow index of the lower row of the kernel
     * @param fromCol index of the lower column of the kernel
     * @param toRow index of the higher row of the kernel
     * @param toCol index of the higher column of the kernel
     *
     *
     * You can initialize a 5x3 container to filter a lti::channel with
     * following code:
     *
     * \code
     * lti::array2D<channel::value_type> aKernel(-1,-2,1,2,0);
     * aKernel.at(-1,-2)=0.2;
     * aKernel.at(-1,-1)=0.2;
     * aKernel.at(0,0)=0.2;
     * aKernel.at(1,1)=0.2;
     * aKernel.at(1,2)=0.2;
     * \endcode
     *
     * \warning This is an interface change with the previous library.  It has
     * been done to be consistent with the more basic features of the C++
     * language.  If you write for example "int c;", the content of \c c is
     * not defined, and in the same way, if you want a kernel1D with 
     * initialized data, you have to specify explicitely the value with 
     * which the elements have to be initialized.
     */
    array2D(const int fromRow,
            const int fromCol,
            const int toRow,
            const int toCol);

    /** 
     * Copy constructor
     * @param other the one dimensional container to be copied
     */
    array2D(const array2D& other);

    /**
     * Destructor
     */
    virtual ~array2D();

    /**
     * Copy member
     * @param other container to be copied
     * @return a reference to this instance
     */
    array2D<T>& copy(const array2D<T>& other);

    /**
     * Alias for copy method
     */
    inline array2D<T>& operator=(const array2D<T>& other);

    /**
     * Copy from container of different type
     * @param other a one dimensional container of another type
     * @return a reference to this instance
     */
    template<typename U>
    array2D<T>& castFrom(const array2D<U>& other) {
      // implementation needs to be here due to bug in VC++
      matrix<T>::castFrom(other);
      offset_ = other.getOffset();
      return (*this);
    }

    /**
     * Copy the content of the other matrix in this container and assign
     * the index (fromRow,fromColumn) to the first element of the matrix.
     * For example if <code>other</code> is a 3x3 matrix, then
     * <code>castFrom(other,-1,-1)</code> is a 3x3 container which indices
     * lay inside [-1,1].
     * @param other the matrix with the data to be copied
     * @param fromRow index for the first row of the matrix
     * @param fromColumn index for the first column of the matrix
     * @return a reference to this instance
     */
    array2D<T>& castFrom(const matrix<T>& other,
                         const int fromRow,
                         const int fromColumn);


    /**
     * Copy the content of the other matrix in this container and assign
     * the index \p firstElement to the first element of the matrix.
     * For example if <code>other</code> is a 3x3 matrix, then
     * <code>castFrom(other,-1,-1)</code> is a 3x3 container which indices
     * lay inside [-1,1].
     * @param other the matrix with the data to be copied
     * @param firstElement index for the first element of the matrix
     * @return a reference to this instance
     */
    array2D<T>& castFrom(const matrix<T>& other,
                         const ipoint& firstElement = ipoint(0,0));


    /**
     * Resize container with indicated indexes
     *
     * @param fromRow index of the lower row of the kernel
     * @param fromCol index of the lower column of the kernel
     * @param toRow index of the higher row of the kernel
     * @param toCol index of the higher column of the kernel
     * @param iniValue the initial value for the elements of the kernel
     * @param resizeType eResizeType controls behaviour or resize
     *
     * You can initialize a 5x3 container to filter a lti::channel with
     * following code:
     *
     * \code
     * lti::array2D<channel::value_type> aKernel;
     * aKernel.resize(-1,-2,1,2,0,Init);
     * aKernel.at(-1,-2)=0.2;
     * aKernel.at(-1,-1)=0.2;
     * aKernel.at(0,0)=0.2;
     * aKernel.at(1,1)=0.2;
     * aKernel.at(1,2)=0.2;
     * \endcode
     */
    void resize(const int fromRow, const int fromCol,
                const int toRow, const int toCol,
		const T& iniValue,
                const eResizeType& resizeType=CopyAndInit);

    /**
     * Change the dimensions of the array2D and leave ALL data
     * uninitialized.
     *
     * @param fromRow index of the lower row of the kernel
     * @param fromCol index of the lower column of the kernel
     * @param toRow index of the higher row of the kernel
     * @param toCol index of the higher column of the kernel
     *
     * The old values will be discarded and the new data will be kept
     * uninitialized.  In principle this is an alias for 
     * \c resize(fromRow, fromCol, toRow, toCol, T(), AllocateOnly).
     *
     * If the resize is possible (see useExternData()), this %object
     * will always own the data!
     */
    inline void allocate(const int fromRow, const int fromCol,
                         const int toRow, const int toCol);

    /**
     * Change the dimensions of the array2D and initialize ALL data
     * with the given value.
     *
     * The old values will be discarded and all data will be initialized with
     * initValue.  In principle this is an alias for
     * \c resize(fromRow, fromCol, toRow, toCol, initValue, Init).
     *
     * If the resize is possible (see useExternData()), this %object
     * will always own the data!
     *
     * @param fromRow index of the lower row of the kernel
     * @param fromCol index of the lower column of the kernel
     * @param toRow index of the higher row of the kernel
     * @param toCol index of the higher column of the kernel
     * @param initValue value to be assigned to all container elements
     */
    inline void assign(const int fromRow, const int fromCol,
                       const int toRow, const int toCol,
                       const T& initValue);

    /**
     * Fills this container between the "from's" and "to's" with the
     * contents of the matrix <code>mat</code> starting at
     * <code>startAtRow</code> and <code>startAtCol</code>
     *
     * @param mat      the matrix with the data to be copied
     * @param fromRow  first row of the submatrix to be filled
     * @param fromCol  first column of the submatrix to be filled
     * @param toRow    last row of the submatrix to be filled
     * @param toCol    last column of the submatrix to be filled
     * @param startAtRow starting row of <code>mat</code> where the data is
     *                   located.
     * @param startAtCol starting column of <code>mat</code> where the data
     *                   is located.
     */
    void fill(const matrix<T>& mat,
              const int fromRow= MinIndex,
              const int fromCol= MinIndex,
              const int toRow=   MaxIndex,
              const int toCol=   MaxIndex,
              const int startAtRow=0,
              const int startAtCol=0);

    /**
     * Fills this container between the "from's" and "to's" with the
     * contents of the matrix <code>mat</code> starting at
     * <code>startAtRow</code> and <code>startAtCol</code>
     *
     * @param data     the data that should fill this kernel
     * @param fromRow  first row of the submatrix to be filled
     * @param fromCol  first column of the submatrix to be filled
     * @param toRow    last row of the submatrix to be filled
     * @param toCol    last column of the submatrix to be filled
     */
    void fill(const T* data,
              const int fromRow= MinIndex,
              const int fromCol= MinIndex,
              const int toRow=   MaxIndex,
              const int toCol=   MaxIndex);

    /**
     * Fills container elements with \a iniValue.
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
              const int fromRow=MinIndex,
              const int fromCol=MinIndex,
              const int toRow=MaxIndex,
              const int toCol=MaxIndex);
    
    
    /**
     * Return the name of this class, including the template type
     */
    virtual const std::string& name() const;

    /**
     * Clone member.
     *
     * @return a pointer to a copy of this object
     */
    virtual array2D<T>* clone() const;

    /**
     * Clone member.
     *
     * @return a pointer to a copy of this object
     */
    virtual array2D<T>* newInstance() const;

    /**
     * Return container offset.
     *
     * Returns offset, i.e. center of the filter-kernel
     * @return the position of the element with index 0 in the memory array.
     *         This value is the same as -firstIdx()
     */
    const ipoint& getOffset() const;

    /**
     * Set container offset.
     *
     * Set container offset, i.e. center of the filter-kernel
     * @param p the position of the element with index (0,0) in the memory
     *          array.
     *          This value is the same as point(-firstRow(),-firstColumns()).
     */
    void setOffset(const ipoint& p);

    /**
     * First row index
     */
    inline int firstRow() const;

    /**
     * Last row index
     */
    inline int lastRow() const;

    /**
     * First column index
     */
    inline int firstColumn() const;

    /**
     * Last column index
     */
    inline int lastColumn() const;

    /*----------------------------------*/
    /*  Access operators                */
    /*----------------------------------*/     

    /**
     * Access operator with offset
     */
    inline T& at(const ipoint& p);

    /**
     * Access operator with offset
     */
    inline const T& at(const ipoint& p) const;

    /**
     * Access operator with offset
     */
    inline T& at(const int y,const int x);
 
    /**
     * Access operator with offset
     */
    inline const T& at(const int y,const int x) const;

    /**
     * Mirror the other container and leave the result here, i.e.
     * at(y,x) = other.at(-y,-x);
     * @param other the container to be copied and then mirrored
     * @return a reference to this instance
     */
    array2D<T>& mirror(const array2D<T>& other);

    /**
     * Mirror this kernel, i.e.
     * at(y,x) = at(-y,-x);
     * @return a reference to this instance
     */
    array2D<T>& mirror();

    /**
     * Write the object in the given ioHandler
     */
    virtual bool write(ioHandler& handler,const bool complete = true) const;

    /**
     * Read the object from the given ioHandler
     */
    virtual bool read(ioHandler& handler,const bool complete = true);

  protected:
    /**
     * Absolute coordinates of the container point (0,0)
     */
    ipoint offset_;

  };

  // ----------------------------------------------------------
  //   Typical used types
  // ----------------------------------------------------------

  
  /**
   * Two dimensional container of integers
   */
  typedef array2D<int>    iarray2D;

  /**
   * Two dimensional container of floats
   */
  typedef array2D<float>  farray2D;

  /**
   * Two dimensional container of doubles
   */
  typedef array2D<double> darray2D;

  /**
   * Two dimensional container of unsigned bytes
   */
  typedef array2D<ubyte>  barray2D;
}

#include "ltiArray2D_inline.h"

#endif
