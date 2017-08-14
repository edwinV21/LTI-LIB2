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
 * \file   ltiKernel1D.h
 *         Contains the template class lti::kernel1D<T>.
 * \author Pablo Alvarado
 * \date   28.04.00
 *
 * revisions ..: $Id: ltiKernel1D.h,v 1.8 2012-09-16 05:22:59 alvarado Exp $
 */

#ifndef _LTI_KERNEL_1_D_H_
#define _LTI_KERNEL_1_D_H_

#include <vector>
#include "ltiArray.h"
#include "ltiTypes.h"
#include "ltiMatrix.h"
#include "ltiTypeInfo.h"

namespace lti {

  /**
   * One-dimensional filter kernel
   *
   * The template type of this class should coincide with the template
   * class of the vector be convolved with.  For example, if you want
   * to convolve a kernel1D with a vector<double>, you will need a
   * kernel1D<double> (@see lti::convolution).
   *
   * If you instantiate a kernel1D of a fixed point type, like
   * kernel1D<int> or kernel1D<ubyte>, you also need to consider the
   * "norm" of the kernel (see lti::kernel1D<T>::getNorm() and
   * lti::kernel1D<T>::setNorm(const T&)). This "norm" allows the
   * representation of numbers less than 1.0.  You can see this norm as the
   * value to be considered as 1.0, when operating with the kernel.
   * For example, if you have a kernel1D<ubyte> with the values [64,128,64]
   * and norm=255, the the interpreted values during convolution will be
   * [0.25,0.5,0.25].  With floating-point types, the norm will be always
   * assumed to be 1.0.  For other types the default norms are the following:
   *
   * For int:              65536
   * For ubyte:              255
   * Otherwise:                1.0
   *
   * @see lti::convolution
   */
  template<class T>
  class kernel1D : public array<T> {
  public:
    /**
     * Default constructor
     */
    kernel1D();

    /** 
     * Construct a filter kernel indexed from <em>from</em> to
     * <em>to</em> and initialized with the value <em>init</em> 
     */
    kernel1D(const int from,
	     const int to,
	     const T& init);

    /** 
     * Construct a uninitialization filter kernel indexed from
     * <em>from</em> to <em>to</em>
     *
     * \warning This is an interface change with the previous library.  It has
     * been done to be consistent with the more basic features of the C++
     * language.  If you write for example "int c;", the content of \c c is
     * not defined, and in the same way, if you want a kernel1D with 
     * initialized data, you have to specify explicitely the value with 
     * which the elements have to be initialized.
     */
    kernel1D(const int from,
	     const int to);

    /**
     * Construct a kernel from a one dimensional vector
     * @param other the source vector
     * @param theOffset this is the index in the vector that corresponds to
     *                  the index '0' in the filter kernel
     */
    kernel1D(const vector<T>& other,const int theOffset);

    /**
     * Copy constructor
     * @param other the one dimensional kernel to be copied
     */
    kernel1D(const kernel1D& other);

    /**
     * Destructor
     */
    virtual ~kernel1D();

    /**
     * Copy member
     * @param other the one dimensional kernel to be copied
     * @return a reference to this instance
     */
    kernel1D& copy(const kernel1D& other);

    /**
     * Returns the name of this type.
     */
    virtual const std::string& name() const;

    /**
     * Clone member
     * @return a pointer to a copy of this object
     */
    virtual kernel1D<T>* clone() const;

    /**
     * Create a new instance.
     * @return a pointer to a new empty kernel instance
     */
    virtual kernel1D<T>* newInstance() const;

    /**
     * Copy from kernel of different type
     * @param other a one dimensional kernel of another type
     * @return a reference to this instance
     */
    template<class U>
    kernel1D& castFrom(const kernel1D<U>& other) {
      array<T>::castFrom(other);
      norm_ = T(other.getNorm());
      return (*this);
    }

    /**
     * Copy the content of the other vector in this kernel and assign
     * the index (firstElement) to the first element of the vector.
     * For example if <code>other</code> is a 3 dimensional vector, then
     * <code>castFrom(other,-1)</code> is a 3-elements-kernel which indices
     * lay inside [-1,1].
     * @param other the vector with the data to be copied
     * @param firstElement index for the first element of the vector
     * @return a reference to this instance
     */
    kernel1D<T>& castFrom(const vector<T>& other,
                          const int firstElement = 0);

    /**
     * Get offset position of element 0
     * @return the position of the element with index 0 in the memory array.
     *         This value is the same as -firstIdx()
     */
    int getOffset() const;

    /**
     * Set offset position of element 0
     * @return the position of the element with index 0 in the memory array.
     *         This value is the same as -firstIdx()
     */
    void setOffset(const int theOffset);


    /**
     * Get normalization factor
     *
     * The normalization factor is used by the fixed point types as a
     * representation of the value 1.  For example, the norm for a
     * kernel1D<ubyte> can be 255, if the filter kernel don't need values
     * greater than 1.0.
     */
    inline const T& getNorm() const {
      return norm_;
    }

    /**
     * Set normalization factor
     * @see getNorm()
     */
    inline void setNorm(const T& n) {
      norm_=n;
    };

    /**
     * Denormalize divide all elements by norm and set the norm to 1!
     */
    void denormalize();

    /**
     * @name Symmetries
     */
    //@{
    /**
     * Mirror the other kernel and leave the result here, i.e.
     * at(x) = other.at(-x);
     * @param other the kernel to be copied and then mirrored
     * @return a reference to this instance
     */
    kernel1D<T>& mirror(const kernel1D<T>& other);

    /**
     * Mirror this kernel, i.e.
     * at(x) = at(-x);
     * @return a reference to this instance
     */
    kernel1D<T>& mirror();

    /**
     * Check if this kernel is symmetric, i.e. if it is valid
     * at(x)==at(-x) and firstIdx() == -lastIdx()
     */
    bool isSymmetric() const;
    
    /**
     * Check if this kernel is asymmetric, i.e. if it is valid
     * at(x)==-at(-x) and firstIdx() == -lastIdx()
     */
    bool isAsymmetric() const;

    /**
     * Get kernel symmetric component
     *
     * Each discrete function (including kernels like this one) can be
     * decomposed into a symmetric component \f$x_e(n)\f$$ and an asymmetric
     * component \f$x_e(n)\f$ following the equation
     *
     * \f[
     *   \begin{aligned} 
     *     x_e &= \frac{x(t)+x(-t)}{2} \\
     *     x_o &= \frac{x(t)-x(-t)}{2} \\
     *   \end{aligned} 
     * \f]
     *
     * This method extracts the symmetric component only
     */
    void computeSymmetricComponent();

    /**
     * Get kernel symmetric component of other kernel.
     *
     * Each discrete function (including kernels like this one) can be
     * decomposed into a symmetric component \f$x_e(n)\f$$ and an asymmetric
     * component \f$x_e(n)\f$ following the equation
     *
     * \f[
     *   \begin{aligned} 
     *     x_e &= \frac{x(t)+x(-t)}{2} \\
     *     x_o &= \frac{x(t)-x(-t)}{2} \\
     *   \end{aligned} 
     * \f]
     *
     * This method extracts the symmetric component of the other kernel.
     *
     * @param other the kernel which will be decomposed.
     */
    void computeSymmetricComponent(const kernel1D<T>& other);

    /**
     * Get kernel asymmetric component
     *
     * Each discrete function (including kernels like this one) can be
     * decomposed into a symmetric component \f$x_e(n)\f$$ and an asymmetric
     * component \f$x_e(n)\f$ following the equation
     *
     * \f[
     *   \begin{aligned} 
     *     x_e &= \frac{x(t)+x(-t)}{2} \\
     *     x_o &= \frac{x(t)-x(-t)}{2} \\
     *   \end{aligned} 
     * \f]
     *
     * This method extracts the asymmetric component only
     */
    void computeAsymmetricComponent();

    /**
     * Get kernel asymmetric component of other kernel.
     *
     * Each discrete function (including kernels like this one) can be
     * decomposed into a symmetric component \f$x_e(n)\f$$ and an asymmetric
     * component \f$x_e(n)\f$ following the equation
     *
     * \f[
     *   \begin{aligned} 
     *     x_e &= \frac{x(t)+x(-t)}{2} \\
     *     x_o &= \frac{x(t)-x(-t)}{2} \\
     *   \end{aligned} 
     * \f]
     *
     * This method extracts the asymmetric component of the other kernel.
     *
     * @param other the kernel which will be decomposed.
     */
    void computeAsymmetricComponent(const kernel1D<T>& other);
    //@}

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
     * Normalization factor.
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
   * One dimensional kernel of integers
   */
  typedef kernel1D<int32>  ikernel1D;

  /**
   * One dimensional kernel of floats
   */
  typedef kernel1D<float>  fkernel1D;

  /**
   * One dimensional kernel of doubles
   */
  typedef kernel1D<double> dkernel1D;

  /**
   * One dimensional kernel of unsigned bytes
   */
  typedef kernel1D<ubyte>  bkernel1D;


}
#endif 
