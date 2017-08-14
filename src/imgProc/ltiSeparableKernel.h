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
 * \file   ltiSeparableKernel.h
 *         Contains the template class seperableKernel<T>
 * \author Pablo Alvarado
 * \date   28.04.2000
 *
 * $Id: ltiSeparableKernel.h,v 1.9 2010-03-23 20:09:32 alvarado Exp $
 */

#ifndef _LTI_SEPARABLE_KERNEL_H_
#define _LTI_SEPARABLE_KERNEL_H_

#include "ltiKernel1D.h"
#include "ltiKernel2D.h"
#include "ltiTypes.h"


namespace lti {
  template <class T>
    class kernel2D;

  /**
   * Separable Kernel.
   *
   * A separable kernel is a %vector of one dimensional kernels.
   * If a two dimensional kernel can be separated, the convolution can be
   * applied in a very efficient way.
   *
   * A filter kernel K is called separable "in one pair", if the matrix
   * representation of K can be produced as an outer product of two
   * one-dimensional kernels Kx and Ky.
   *
   * The template type of this class should coincide with the template class of
   * the matrix or channel to be convolved with.  For example, if you want to
   * convolve a separableKernel with a lti::channel, you will need a
   * separableKernel<channel::value_type> or separableKernel<float>.
   *
   * If you instantiate a separableKernel of a fixed point type, like
   * separableKernel<int> or separableKernel<ubyte>, you also need to 
   * consider the
   * "norm" of the kernel (see lti::separableKernel<T>::getNorm() and
   * lti::separableKernel<T>::setNorm(const T&)). This "norm" allows the
   * representation of numbers less than 1.0.  You can see this norm as the
   * value to be consider as 1.0, when operating with the kernel.
   * For example, if you have a separableKernel<ubyte> with the values 
   * [64,128,64]
   * and norm=255, the the interpreted values during convolution will be
   * [0.25,0.5,0.25].  With floating-point types, the norm will be always
   * assumed to be 1.0.  For other types the default norms are the following:
   * (see lti::typeInfo)
   *
   * - For int:              65536
   * - For ubyte:              255
   * - Otherwise:              1.0
   *
   * Example:
   *
   * Let us suppose you want to create a kernel equivalent to the mask
   *
   * \code
   * -1 -1 -1 -2 -1 -1 -1
   *  0  0  0  0  0  0  0
   *  1  1  1  2  1  1  1
   * \endcode
   *
   * which is separable in two kernels: one row kernel 
   * \code
   *  1  1  1  2  1  1  1
   * \endcode
   * and another column kernel
   * \code
   * -1
   *  0
   *  1 
   * \endcode
   *
   * The code required for generating such a filter follows:
   *
   * \code
   * lti::separableKernel<float> mask;
   * mask.setNumberOfPairs(1); // Just one filter pair
   * // initialize the row filter
   * kernel1D<float>& row = mask.getRowFilter(0); // alias for the row filter
   * row.assign(-3,3,1.0f);
   * row.at(0)=2;
   * // initialize the column filter
   * kernel1D<float>& col = mask.getColFilter(0); // alias for the col filter
   * col.allocate(-1,1);
   * col.at(-1)=-1;
   * col.at( 0)= 0;
   * col.at( 1)= 1;
   *
   * // Use now the kernel in the convolution
   * lti::channel chnl; 
   * // ... load the channel somehow ...
   *
   * lti::convolution filter(mask);  // create the convolution functor
   * filter.apply(chnl);             // and filter the channel
   * \endcode
   * 
   * @see convolution
   *
   */
  template<class T>
  class separableKernel : public container {
  public:
    /**
     * Default constructor
     */
    separableKernel();

    /**
     * Copy constructor
     * @param other the kernel to be copied.
     */
    separableKernel(const separableKernel& other);

    /**
     * Construct a separable kernel with one filter pair, all elements
     * of the subfilters initialized with the given value.
     * @param from first index of the one dimensional filter kernel
     * @param to   last index of the one dimensional filter kernel
     * @param iniValue initial value for the kernel elements
     */
    separableKernel(const int from,const int to,const T& iniValue=T());

    /**
     * Construct a symmetrical separable kernel
     *
     * The resulting separable kernel will have just one filter pair, where
     * the row and column filters are identical.
     *
     * @param subkernel the one-dimensional kernel to be used as row and
     *                  column filter.
     */
    separableKernel(const kernel1D<T>& subkernel);

    /**
     * Destructor
     */
    virtual ~separableKernel();

    /**
     * Copy member
     * @param other the kernel to be copied.
     * @return a reference to this instance
     */
    separableKernel& copy(const separableKernel& other);

    /**
     * Returns the name of this type 
     */
    virtual const std::string& name() const;

    /**
     * Clone member
     * @return a pointer to a copy of this instance
     */
    virtual separableKernel* clone() const;

    
    /**
     * Return a new instance of a separable kernel
     * @return a pointer to a new instance
     */
    virtual separableKernel* newInstance() const;

    /**
     * Copy from kernel of another type
     * @param other a separable kernel of another type
     * @return a reference to this instance
     */
    template<class U>
    separableKernel& castFrom(const separableKernel<U>& other) {
      // implementation needs to be here due to VC++ bug
      rowKernels_.resize(other.getNumberOfPairs());
      colKernels_.resize(other.getNumberOfPairs());
      for (int i=0;i<other.getNumberOfPairs();i++) {
	rowKernels_[i].castFrom(other.getRowFilter(i));
	colKernels_[i].castFrom(other.getColFilter(i));
      }
      return (*this);
    }

    /**
     * Separate a 2D kernel into 1D kernels
     *
     * Try to separate the two dimensional kernel \a k.  Stop the
     * separation if the error between original and separated kernel is less
     * than \a maxDev.
     *
     * @param k the two dimensional filter to be separated
     * @param maxDev the maximal deviation per element to be achieved
     * @return true if the separation succeeded or false otherwise.
     */
    bool separate(const kernel2D<T>& k,const double &maxDev=0.01);

    /**
     * Number of filter pairs
     */
    int getNumberOfPairs() const;

    /**
     * Set the number of column/row 1D-filters
     */
    void setNumberOfPairs(const int numPairs);

    /**
     * Return a row-kernel
     * @param i the index of the row filter.  This value must be between
     *        0 and getNumberOfPairs()
     */
    inline kernel1D<T>& getRowFilter(const int i) {
      // check for limits
      assert(i<static_cast<int>(rowKernels_.size()));
      return rowKernels_[i];
    };

    /**
     * Return a column-kernel
     * @param i the index of the column filter.  This value must be between
     *        0 and getNumberOfPairs()
     */
    inline kernel1D<T>& getColFilter(const int i) {
      // check for limits
      assert(i<static_cast<int>(colKernels_.size()));
      return colKernels_[i];      
    };

    /**
     * Return an unmodifiable row kernel
     * @param i the index of the row filter.  This value must be between
     *        0 and getNumberOfPairs()
     */
    inline const kernel1D<T>& getRowFilter(const int i) const {
      // check for limits
      assert(i<static_cast<int>(rowKernels_.size()));
      return rowKernels_[i];
    }

    /**
     * Return an unmodifiable column kernel
     * @param i the index of the column filter.  This value must be between
     *        0 and getNumberOfPairs()
     */
    inline const kernel1D<T>& getColFilter(const int i) const {
      // check for limits
      assert(i<static_cast<int>(colKernels_.size()));
      return colKernels_[i];
    };

    /**
     * Denormalize divide all elements by norm and set the norm to 1!
     */
    void denormalize();

    /**
     * Multiply each 1D kernel with a constant value
     * @param value the value to be multiplied with
     * @return a reference to this object
     */
    separableKernel<T>& multiply(const T& value);

    /**
     * Set the norm of each individual 1D kernel to the given value
     * @param newNorm the value to be used as norm
     * @return a reference to this object
     */
    void setNorm(const T& newNorm);

    /**
     * Returns the sum of the elements of the resulting 2D kernel
     */
    T computeSumOfElements() const;

    /**
     * Mirror the other kernel and leave the result here, i.e.
     * at(y,x) = other.at(-y,-x);
     * @param other the kernel to be copied and then mirrored
     * @return a reference to this instance
     */
    separableKernel<T>& mirror(const separableKernel<T>& other);

    /**
     * Mirror this kernel, i.e.
     * at(y,x) = at(-y,-x);
     * @return a reference to this instance
     */
    separableKernel<T>& mirror();

    /**
     * Write the object in the given ioHandler
     */
    virtual bool write(ioHandler& handler,const bool complete = true) const;

    /**
     * Read the object from the given ioHandler
     */
    virtual bool read(ioHandler& handler,const bool complete = true);

    /**
     * @name Apply Methods
     */
    //@{

    /**
     * Applies a C-function to each element of the kernel.
     * 
     * In the following example, kernel \a kernel is initialized with
     * 4.0. After applying \a sqrt(), all elements of \a kernel are 2.0.
     * \code
     * separableKernel<float> kern(-2,2,4.0);
     * kern.apply(sqrt);
     * \endcode
     * @param function a pointer to a C-function
     * @return a reference to the actual kernel
     */
    separableKernel<T>& apply(T (*function)(T));

    /**
     * Applies a C-function to each element of the kernel.
     * @param function a pointer to a C-function
     * @return a reference to the actual kernel
     */
    separableKernel<T>& apply(T (*function)(const T&));

    /**
     * Applies a C-function to each element of the other kernel and leaves
     * the result here.
     * @param other the source kernel
     * @param function a pointer to a C-function
     * @return a reference to the actual kernel
     */
    separableKernel<T>& apply(const separableKernel<T>& other,
			      T (*function)(T));


    /**
     * Applies a C-function to each element the other kernel and
     * leaves the result here.
     *
     * @param other the kernel with the source data
     * @param function a pointer to a C-function
     * @return a reference to the actual kernel
     */
    separableKernel<T>& apply(const separableKernel<T>& other,
			      T (*function)(const T&));

    //@}

  protected:
    /**
     * List of one-dimensional row kernels
     */
    std::vector< kernel1D<T> > rowKernels_;

    /**
     * List of one-dimensional column kernels
     */
    std::vector< kernel1D<T> > colKernels_;

  };

  // ----------------------------------------------------------
  //   Typical used types
  // ----------------------------------------------------------

  /**
   * Separable kernel of integers
   */
  typedef separableKernel<int>    iseparableKernel;

  /**
   * Separable kernel of floats
   */
  typedef separableKernel<float>  fseparableKernel;

  /**
   * Separable kernel of doubles
   */
  typedef separableKernel<double> dseparableKernel;

  /**
   * Separable kernel of unsigned bytes
   */
  typedef separableKernel<ubyte>  bseparableKernel;


  // ----------------------------------------------------------
  //   stream output
  // ----------------------------------------------------------

  /**
   * outputs the separable kernel \p kern on a stream \p s. The kernel pairs
   * are printed in the order they are stored in the separable kernel:
   *
   * \code
   * (((first row kernel)
   *   (first column kernel))
   *  ((second row kernel)
   *   (second column kernel))
   *   ...
   *  ((last row kernel)
   *   (last column kerne)))
   * \endcode
   */
  template <class T>
  std::ostream& operator<<(std::ostream& s, const separableKernel<T>& kern);

}

#endif

