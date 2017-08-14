/*
 * Copyright (C) 2007
 * ITCR, Pablo Alvarado
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
 * \file   ltiMatrixProcessingInterface.h
 *         Simple interface from which all functors that process a matrix<T>
 *         to produce another matrix can be inherited.
 * 
 * \author Pablo Alvarado
 * \date   23.09.2007
 *
 * revisions ..: $Id: ltiMatrixProcessingInterface.h,v 1.2 2007-10-07 03:17:16 alvarado Exp $
 */

#ifndef _LTI_MATRIX_PROCESSING_INTERFACE_H_
#define _LTI_MATRIX_PROCESSING_INTERFACE_H_

#include "ltiMatrix.h"
#include "ltiRGBAPixel.h"

namespace lti {

  /**
   * Class matrixProcessingInterface
   *
   * Very simple interface to allow virtualization of classes that
   * transform a matrix into another one, like convolution, correlation,
   * matrixTransform, etc.
   *
   * Note that it is required that the apply methods do not alter the internal
   * state of the class.
   *
   * \ingroup gInterfaces
   */
  template<typename T>
  class matrixProcessingInterface {
  public:
    
    /**
     * Virtual destructor
     */
    virtual ~matrixProcessingInterface() {};

    /**
     * On-place processing apply.
     * 
     * The inherited methods should take the \a srcdest matrix, process it in
     * some way, and on the same matrix leave the result.  No restrictions are
     * imposed on whether the memory block of the resulting matrix will be the
     * same that the one in the original matrix.  As a matter of fact, it
     * usually won't be.
     *
     * If you need to ensure the memory constancy, and assuming the resulting
     * matrix will always have the same size than the original one, then you
     * can use the following code: 
     * \code
     * matrixProcessingInterfaceInheritedClass<T> theFunctor; 
     * matrix<T> tmp;
     * theFunctor.apply(srcdest,tmp); 
     * srcdest.fill(tmp)
     * \endcode
     *
     * which of course will be slower as it requires to copy all the data of
     * the result in the original matrix.
     *
     * @param srcdest matrix<T> with the source data. The result
     *                will be left here too.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(matrix<T>& srcdest) const = 0;

    /**
     * On-copy processing apply.
     * 
     * The inherited methods take the \a src matrix and process it leaving the
     * result in the \a dest matrix.
     *
     * @param src matrix<T> with the source data.
     * @param dest matrix<T> where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const matrix<T>& src, matrix<T>& dest) const = 0;
  };
}

#endif

