/*
 * Copyright (C) 1998 - 2005
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
 * \file   ltiAffineTransform.h Contains the class lti::ltiAffineTransform,
 * which transforms a region form a source image to a square region._
 * \author Roland Neul
 * \date   18.05.2004
 *
 * revisions ..: $Id: ltiAffineTransform.h,v 1.4 2006-03-28 13:32:09 doerfler Exp $
 */


#ifndef _LTI_AFFINE_TRANSFORM_H_
#define _LTI_AFFINE_TRANSFORM_H_

#include "ltiImage.h"
#include "ltiBilinearInterpolation.h"
#include "ltiModifier.h"
#include "ltiRGBAPixel.h"
#include "ltiPoint.h"

#include <cmath>
using std::isfinite;

namespace lti {
  /**
   * This functor transforms a region from a source image (or channel) to a
   * square region. It differs from geometricTransform in several aspects.
   * First it uses a fixed sized destination. The idea behind is that you have
   * got an image that has a small region with a known affine distortion, and
   * this functor removes this distortion from the local region. This functor
   * is not meant to transform a whole image! Another difference is it uses
   * only a 2x3 matrix which cannot cover projective transformations.
   * There are two ways in which the functor can be used. If you have to
   * transform many regions which have the same matrix associated with them
   * you should first call the use() method with a 2x2 transformation matrix
   * and then call the apply() method multiple times with the source data and
   * the position. If you only use a matrix once, simply call the apply method
   * with a 2x3 matrix. The source positions use homogenous coordinates and
   * the destination positions euclidean coordinates.
   *
   *
   */
  template <typename T, template<class> class Inter = bilinearInterpolation>
  class affineTransform : public modifier {
  public:

    /**
     * There are two options which kind of invariance is
     * achieved. With Skew there is now normalization with respect to
     * rotation. Full also tries to remove rotation, leaving a 180
     * degree rotation.
     *
     * Note that Full is the more intuitive version. However, when the
     * orientation of an affineLocation cannot be determined robustly
     * this is quite useless.
     *
     * Skew results in an image that can be rotated by k*90 degrees
     * (K=0..3) from the result of Full.
     */
    enum eTransformType {
      Skew, /**< only 90 degree invariance */
      Full  /**< 180 degree invariance */
    };
      
    /**
     * The parameters for the class affineTransform
     */
    class parameters : public modifier::parameters {
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
       * Returns the name of this type.
       */
      virtual const std::string& name() const;

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
       * Returns a pointer to a clone of the parameters
       */
      virtual parameters* clone() const;

      /**
       * Returns a pointer to a new instance of the parameters
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
       * The radius of the destination region. The destination will be
       * resized to a square region with edge length 2 * r + 1.
       */
      int patchRadius;

      /**
       * Type of transform used to achieve invariance. See
       * eTransformType for details.
       *
       * Default: Full
       */
      eTransformType transformType;

      /**
       * Parameters for the used interpolation functor.
       * Note: interpolationParameters.boundaryType will be ignored
       * The internal interpolation functor will always use the
       * the boundaryType parameter of this class.
       */
      typename Inter<T>::parameters interpolationParameters;
    };

    /**
     * Default constructor
     */
    affineTransform();

    /**
     * Construct a functor using the given parameters
     */
    affineTransform(const parameters& par);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    affineTransform(const affineTransform& other);

    /**
     * Destructor
     */
    virtual ~affineTransform();

    /**
     * Returns the name of this type.
     */
    virtual const std::string& name() const;

    /**
     * Transform a region from the source data to a square patch with edge
     * length 2 * patchRadius + 1. The 2x2-transformation matrix must be set
     * with the use() method before calling this. The point is the center of
     * the region to transform
     * @param src source data.
     * @param p center of the region to transform.
     * @param dest here the result will be left.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const matrix<T>& src,
               const point<float>& p,
               matrix<T>& dest) const;

    /**
     * Transform a region from the source data to a square patch with edge
     * length 2 * patchRadius + 1. Using the supplied 2x3 matrix. The matrix
     * includes a translation, so no extra information is needed. Use this
     * method if you only apply a transformation once. For multiple
     * applications of the same matrix on different channels and at different
     * positions you should use the other apply method with a prior call to
     * the use() method to set the 2x2 part of the translation matrix.
     * @param src channel with the source data.
     * @param transMatrix transformation matrix (including translation)
     * @param dest channel where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const matrix<T>& src,
               const matrix<float>& transMatrix,
               matrix<T>& dest) const;

    /**
     * Transform a region from the source data to a square patch with edge
     * length 2 * patchRadius + 1. Using the supplied 2x2 matrix and the
     * center of the patch. Use this method if you only apply a transformation
     * once. For multiple applications of the same matrix on different
     * channels and at different positions you should use the other apply
     * method with a prior call to the use() method to set the 2x2 part of the
     * translation matrix.
     *
     * @param src channel with the source data.
     * @param p center of the region to transform.
     * @param transMatrix transformation matrix (including translation)
     * @param dest channel where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const matrix<T>& src,
               const fpoint& p,
               const matrix<float>& transMatrix,
               matrix<T>& dest) const;


    /**
     * sets a transformation matrix for later apply() calls accepts
     * 2x2 and 2x3 matrices but does not use the third column if present.
     * @param transMatrix transformation matrix without translation.
     * @return true if successful
     */
    bool use(const matrix<float>& transMatrix);

    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    affineTransform& copy(const affineTransform& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    affineTransform& operator=(const affineTransform& other);

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual affineTransform* clone() const;

     /**
      * Returns a pointer to a new instance of this functor
      */
    virtual affineTransform* newInstance() const;

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * update parameters
     */
    virtual bool updateParameters();
    
  protected:

    /** apply for Skew */
    bool applySkew(const matrix<T>& src,
                   const double& cx, const double& cy,
                   const matrix<float>& transMatrix,
                   matrix<T>& dest) const;

    /** apply for Full */
    bool applyFull(const matrix<T>& src,
                   const double& cx, const double& cy,
                   const matrix<float>& transMatrix,
                   matrix<T>& dest) const;
    
    /** use for Skew */
    bool useSkew(const matrix<float>& transMatrix);

    /** use for Full */
    bool useFull(const matrix<float>& transMatrix);

    /** pre-compiled mappings */
    matrix<float> xMap_, yMap_;

    /** min/max values of mappings */
    float xamaxMap_, yamaxMap_;

    /** status flag */
    bool mapsAreValid_;

    /** interpolation functor */
    Inter<T> interpol_;
  };
}

#include "ltiAffineTransform_template.h"

#endif

