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
 * \file   ltiBoundaryExpansion.h
 *         Contains the class lti::boundaryExpansion, 
 *         which is used to expand an image with a border defined through the
 *         boundaryType parameter
 * 
 * \author Pablo Alvarado
 * \date   03.10.2007
 *
 * revisions ..: $Id: ltiBoundaryExpansion.h,v 1.1 2007-10-05 00:46:15 alvarado Exp $
 */

#ifndef _LTI_BOUNDARY_EXPANSION_H_
#define _LTI_BOUNDARY_EXPANSION_H_

#include "ltiMatrix.h"
#include "ltiVector.h"
#include "ltiFunctor.h"
#include "ltiTypeInfo.h"
#include "ltiBoundaryType.h"

namespace lti {

  /**
   * Class boundaryExpansion
   *
   * This class is used to expand the border of an image and fill it according
   * to the boundary type specified in the parameters.  It is employed by
   * several functors to work using "internal" pixels only, without worring
   * about the specific border type to be used.
   *
   * If negative values of border are given in the parameters, the image will
   * be clipped.
   *
   * @see boundaryExpansion::parameters.
   *
   * @ingroup gGeometricTrans
   */
  class boundaryExpansion : public functor {
  public:
    /**
     * The parameters for the class boundaryExpansion
     */
    class parameters : public functor::parameters {
    public:
      /**
       * Default constructor
       */
      parameters();

      /**
       * Copy constructor
       * @param other the parameters object to be copied
       */
      parameters(const parameters& other);

      /**
       * Destructor
       */
      ~parameters();

      /**
       * Copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& copy(const parameters& other);

      /**
       * Copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& operator=(const parameters& other);

      /**
       * Returns the complete name of the parameters class.
       */
      virtual const std::string& name() const;

      /**
       * Returns a pointer to a clone of the parameters.
       */
      virtual parameters* clone() const;

      /**
       * Returns a pointer to a new instance of the parameters.
       */
      virtual parameters* newInstance() const;

      /**
       * Write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool write(ioHandler& handler,const bool complete=true) const;

      /**
       * Read the parameters from the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool read(ioHandler& handler,const bool complete=true);

      // ------------------------------------------------
      // the parameters
      // ------------------------------------------------

      /**
       * Number of pixels in the bottom border.
       *
       * Default value: 5
       */
      int bottomBorder;

      /**
       * Number of pixels in the left border.
       *
       * Default value: 5
       */
      int leftBorder;

      /**
       * Number of pixels in the right border.
       *
       * Default value: 5
       */
      int rightBorder;

      /**
       * Number of pixels in the top border.
       *
       * Default value: 5
       */
      int topBorder;

      /**
       * Type of boundary expansion
       */
      eBoundaryType boundaryType;

      /**
       * Normalization.
       *
       * If set to \c true, then the values will be normalized and denormalized
       * when transfering the data from one matrix of one type to a matrix of
       * another type, so that the values preserve their nature.  For instance,
       * when converting ubyte to float, the values will be divided by 255 to
       * produce values between 0.0f and 1.0f, or the other way around. The
       * normalization constants are given by
       * lti::typeInfo<T>::suggestedNorm().
       *
       * If set to \c false then the values are simply casted.
       *
       * Default value: false
       */
      bool normalize;

    };

    /**
     * Default constructor
     */
    boundaryExpansion();

    /**
     * Construct a functor with a border of homogeneous width all around, and
     * the given boundary type.
     *
     * @param width width of the border
     * @param type boundary type
     * @param normalize indicate if normalization by the type conversion has
     *                  to be made.
     */
    boundaryExpansion(const int width,
                      const eBoundaryType type = Zero,
                      const bool normalize = false);

    /**
     * Construct a functor using the given parameters
     */
    boundaryExpansion(const parameters& par);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    boundaryExpansion(const boundaryExpansion& other);

    /**
     * Destructor
     */
    virtual ~boundaryExpansion();

    /**
     * Expand the boundary of the given matrix and leave the result on the
     * same matrix.
     *
     * @param srcdest matrix<T> with the source data. The result
     *                will be left here too.
     * @return true if apply successful or false otherwise.
     */
    template<typename T>
    bool apply(matrix<T>& srcdest) const;

    /**
     * Operates on a copy of the given arguments.
     *
     * @param src matrix<T> with the source data.
     * @param dest matrix<T> where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    template<typename T,typename U>   
    bool apply(const matrix<T>& src, matrix<U>& dest) const;

    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    boundaryExpansion& copy(const boundaryExpansion& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    boundaryExpansion& operator=(const boundaryExpansion& other);

    /**
     * Returns the complete name of the functor class
     */
    virtual const std::string& name() const;

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual boundaryExpansion* clone() const;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual boundaryExpansion* newInstance() const;

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;

  private:
    /**
     * The class doing the real job
     */ 
    template<typename T,typename U,bool N>
    class helperBase;

    /**
     * The class doing the real job
     */ 
    template<typename T,typename U,bool N>
    class helper;
  };
}

#include "ltiBoundaryExpansion_template.h"

#endif

