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
 * \file   ltiKernel2D.h
 *         Contains the template class lti::kernel2D<T>.
 * \author Pablo Alvarado
 * \date   28.04.00
 *
 * revisions ..: $Id: ltiKernel2D.h,v 1.16 2012-09-16 05:22:59 alvarado Exp $
 */

#ifndef _LTI_KERNEL_2_D_H_
#define _LTI_KERNEL_2_D_H_

#include <vector>
#include "ltiArray.h"
#include "ltiTypes.h"
#include "ltiMatrix.h"
#include "ltiTypeInfo.h"

#include "ltiKernel1D.h" 
#include "ltiSeparableKernel.h"

namespace lti {
  template <class T>
  class separableKernel;


  /**
   * Two-dimensional filter kernel
   *
   * The template type of this class should coincide with the template
   * class of the matrix or channel to be convolved with.  For
   * example, if you want to convolve a kernel2D with a channel8, you
   * will need a kernel2D<channel8::value_type> or
   * kernel2D<ubyte>. The offset of the kernel is at
   * int(columns()/2),int(rows()/2). This means that for kernels of
   * odd width and height, the origin (0,0) is exactly in the center,
   * while for kernels with even width and/or height, it is shifted
   * towards the upper left corner.
   *
   * If you instantiate a kernel2D of a fixed point type, like
   * kernel2D<int> or kernel2D<ubyte>, you also need to consider the
   * "norm" of the kernel (see lti::kernel2D<T>::getNorm() and
   * lti::kernel2D<T>::setNorm(const T&)). This "norm" allows the
   * representation of numbers less than 1.0.  You can see this norm as the
   * value to be consider as 1.0, when operating with the kernel.
   * For example, if you have a kernel2D<ubyte> with the values [64,128,64]
   * and norm=255, the the interpreted values during convolution will be
   * [0.25,0.5,0.25].  With floating-point types, the norm will be always
   * assumed to be 1.0.  For other types the default norms are the following:
   *
   * - For int:              65536
   * - For ubyte:              255
   * - Otherwise:                1.0
   *
   * You can generate 2D kernels from separable ones using the
   * castFrom() method.  For example:
   *
   *  \code
   *  lti::gaussKernel2D<float> gauss(5); // a sepKernel 5x5 
   *  lti::kernel2D<float>      kern;     // a 2D kernel;
   *  
   *  kern.castFrom(gauss);
   *  \endcode   
   *
   * You can also use the outerProduct member to construct a 2D kernel from
   * two 1D ones:
   *
   *  \code
   *  lti::gaussKernel1D<float> colKern(3); // a 1D kernel
   *  lti::gaussKernel1D<float> rowKern(5); // another 1D kernel
   *
   *  lti::kernel2D<float>      kern;     // a 2D kernel;
   *  
   *  kern.outerProd(colKern,rowKern);    // outer product of both kernels
   *  \endcode   
   *
   * @see lti::convolution
   *
   * @todo The access time can be considerably reduced using the same
   *       technique as with the 1D arrays, i.e. using a pointer to
   *       the (0,0) cell, which can then be accessed directly with
   *       the negative * values.  Some additional thoughts about this
   *       have to be done.
   */
  template<typename T>
  class kernel2D : public matrix<T> {
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
    kernel2D();   

    /**
     * Create a new kernel of indicated size and initial value. The offset is
     * kept at zero.
     *
     * @param rows number of rows
     * @param columns number of columns
     * @param init initial value of all elements
     */
    kernel2D(const int rows,
	     const int columns,
	     const T& init);

    /**
     * Create a new unintialized kernel of indicated size. The offset is
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
    kernel2D(const int nbRows,
	     const int nbColumns);

    /**
     * Create a new kernel with indicated indices.
     *
     * @param fromRow index of the lower row of the kernel
     * @param fromCol index of the lower column of the kernel
     * @param toRow index of the higher row of the kernel
     * @param toCol index of the higher column of the kernel
     * @param iniValue the initial value for the elements of the kernel
     *
     * You can initialize a 5x3 kernel to filter a lti::channel with
     * following code:
     *
     * \code
     * lti::kernel2D<channel::value_type> aKernel(-1,-2,1,2,0);
     * aKernel.at(-1,-2)=0.2;
     * aKernel.at(-1,-1)=0.2;
     * aKernel.at(0,0)=0.2;
     * aKernel.at(1,1)=0.2;
     * aKernel.at(1,2)=0.2;
     * \endcode
     */
    kernel2D(const int fromRow,
             const int fromCol,
             const int toRow,
             const int toCol,
             const T& iniValue);

    /**
     * Create a new uninitialized kernel with indicated indices.
     *
     * @param fromRow index of the lower row of the kernel
     * @param fromCol index of the lower column of the kernel
     * @param toRow index of the higher row of the kernel
     * @param toCol index of the higher column of the kernel
     *
     *
     * You can initialize a 5x3 kernel to filter a lti::channel with
     * following code:
     *
     * \code
     * lti::kernel2D<channel::value_type> aKernel(-1,-2,1,2,0);
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
    kernel2D(const int fromRow,
             const int fromCol,
             const int toRow,
             const int toCol);

    /** 
     * Copy constructor
     * @param other the one dimensional kernel to be copied
     */
    kernel2D(const kernel2D& other);

    /**
     * Destructor
     */
    virtual ~kernel2D();

    /**
     * Copy member
     * @param other kernel to be copied
     * @return a reference to this instance
     */
    kernel2D<T>& copy(const kernel2D<T>& other);

    /**
     * Alias for copy method
     */
    inline kernel2D<T>& operator=(const kernel2D<T>& other);

    /**
     * Copy from kernel of different type
     * @param other a one dimensional kernel of another type
     * @return a reference to this instance
     */
    template<typename U>
    kernel2D<T>& castFrom(const kernel2D<U>& other) {
      // implementation needs to be here due to bug in VC++
      matrix<T>::castFrom(other);
      offset_ = other.getOffset();
      norm_ = static_cast<T>(other.getNorm());
      return (*this);
    }

    /**
     * Copy the content of the other matrix in this kernel and assign
     * the index (fromRow,fromColumn) to the first element of the matrix.
     * For example if <code>other</code> is a 3x3 matrix, then
     * <code>castFrom(other,-1,-1)</code> is a 3x3 kernel which indices
     * lay inside [-1,1].
     * @param other the matrix with the data to be copied
     * @param fromRow index for the first row of the matrix
     * @param fromColumn index for the first column of the matrix
     * @return a reference to this instance
     */
    kernel2D<T>& castFrom(const matrix<T>& other,
                          const int fromRow,
                          const int fromColumn);


    /**
     * Copy the content of the other matrix in this kernel and assign
     * the index \p firstElement to the first element of the matrix.
     * For example if <code>other</code> is a 3x3 matrix, then
     * <code>castFrom(other,-1,-1)</code> is a 3x3 kernel which indices
     * lay inside [-1,1].
     * @param other the matrix with the data to be copied
     * @param firstElement index for the first element of the matrix
     * @return a reference to this instance
     */
    kernel2D<T>& castFrom(const matrix<T>& other,
                          const ipoint& firstElement = ipoint(0,0));


    /**
     * Copy the contents of the other separable kernel in this
     * two-dimensional kernel
     */
    kernel2D<T>& castFrom(const separableKernel<T>& other);

    /**
     * Resize kernel with indicated indexes
     *
     * @param fromRow index of the lower row of the kernel
     * @param fromCol index of the lower column of the kernel
     * @param toRow index of the higher row of the kernel
     * @param toCol index of the higher column of the kernel
     * @param iniValue the initial value for the elements of the kernel
     * @param resizeType eResizeType controls behaviour or resize
     *
     * You can initialize a 5x3 kernel to filter a lti::channel with
     * following code:
     *
     * \code
     * lti::kernel2D<channel::value_type> aKernel;
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
     * Change the dimensions of the kernel2D and leave ALL data
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
     * Change the dimensions of the kernel2D and initialize ALL data
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
     * @param initValue value to be assigned to all kernel elements
     */
    inline void assign(const int fromRow, const int fromCol,
                       const int toRow, const int toCol,
                       const T& initValue);

    /**
     * Fills this kernel between the "from's" and "to's" with the
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
     * Fills this kernel between the "from's" and "to's" with the
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
     * Fills kernel elements with \a iniValue.
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
    virtual kernel2D<T>* clone() const;

    /**
     * Clone member.
     *
     * @return a pointer to a copy of this object
     */
    virtual kernel2D<T>* newInstance() const;

    /**
     * Get normalization factor.
     *
     * The normalization factor is used by the fixed point types as a
     * representation of the value 1.  For example, the norm for a
     * kernel1D<ubyte> can be 255, if the filter kernel don't need
     * values greater than 1.0.
     */
    inline const T& getNorm() const;

    /**
     * Set normalization factor.
     *
     * @see getNorm()
     */
    inline void setNorm(const T& n);

    /**
     * Return kernel offset.
     *
     * Returns offset, i.e. center of the filter-kernel
     * @return the position of the element with index 0 in the memory array.
     *         This value is the same as -firstIdx()
     */
    const ipoint& getOffset() const;

    /**
     * Set kernel offset.
     *
     * Set kernel offset, i.e. center of the filter-kernel
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
     * Outer product of two kernel1D.
     * The first kernel is a column kernel and the second kernel is a
     * row kernel.
     *
     * @param colKernel column kernel
     * @param rowKernel row kernel
     * @return a reference to this kernel
     *
     * If you want to convert a separable kernel (lti::sepKernel<T>) into a
     * 2D kernel, see also the methods castFrom().
     *
     * The final kernel will contain colKernel.size() rows and rowKernel.size()
     * columns.  Each row will be equal to the rowKernel multiplied by the
     * respective colKernel element.
     *
     */
    kernel2D<T>& outerProduct(const kernel1D<T>& colKernel,
    			      const kernel1D<T>& rowKernel);

    /**
     * Denormalize divide all elements by norm and set the norm to 1!
     */
    void denormalize();

    /**
     * Mirror the other kernel and leave the result here, i.e.
     * at(y,x) = other.at(-y,-x);
     * @param other the kernel to be copied and then mirrored
     * @return a reference to this instance
     */
    kernel2D<T>& mirror(const kernel2D<T>& other);

    /**
     * Mirror this kernel, i.e.
     * at(y,x) = at(-y,-x);
     * @return a reference to this instance
     */
    kernel2D<T>& mirror();

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
     * Absolute coordinates of the kernel point (0,0)
     */
    ipoint offset_;

    /**
     * Normalisation factor.
     *
     * This value will be ignored for floating point formats.
     * For fixed point formats, this value corresponds to 1.0
     */
    T norm_;
  };

  // ----------------------------------------------------------
  //   Typical used types
  // ----------------------------------------------------------

  
  /**
   * Two dimensional kernel of integers
   */
  typedef kernel2D<int>    ikernel2D;

  /**
   * Two dimensional kernel of floats
   */
  typedef kernel2D<float>  fkernel2D;

  /**
   * Two dimensional kernel of doubles
   */
  typedef kernel2D<double> dkernel2D;

  /**
   * Two dimensional kernel of bkernel
   */
  typedef kernel2D<ubyte>  bkernel2D;
}

#include "ltiKernel2D_inline.h"

#endif
