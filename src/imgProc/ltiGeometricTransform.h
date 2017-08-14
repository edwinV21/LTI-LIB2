/*
 * Copyright (C) 2007
 * Pablo Alvarado
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
 * \file   ltiGeometricTransform.h
 *         Contains the classes lti::geometricTransform<T> and 
 *         lti::geometricTransformBase,  
 *         which are the parent classes of all geometric transformation
 *         classes, like rotation, scale, translation, etc.
 * \author Pablo Alvarado
 * \date   08.09.2007
 *
 * revisions ..: $Id: ltiGeometricTransform.h,v 1.5 2010/02/09 15:55:32 alvarado Exp $
 */

#ifndef _LTI_GEOMETRIC_TRANSFORM_H_
#define _LTI_GEOMETRIC_TRANSFORM_H_

#include "ltiMatrix.h"
#include "ltiFunctor.h"
#include "ltiMatrixProcessingInterface.h"

#include "ltiBilinearInterpolation.h"

namespace lti {

  /**
   * Class geometricTransformBase.
   *
   * This is an abstract class parent of all functors that achieve geometric
   * transformation of images, like affine or homogeneous transformations.  It
   * contains the basic parameters that are not template dependent.
   *
   * @see geometricTransformBase::parameters.
   *
   * @ingroup gGeometricTrans
   */
  class geometricTransformBase : public functor {
  public:

    /**
     * Enumeration to specified how the dimensions of the resulting image 
     * has to be computed
     */
    enum eResizeMode {
      KeepDimensions, /**< Keep the dimensions of the original image, including
                       *   the relative position of the origin.
                       */
      KeepOrigin,     /**< Keep the origin relative position, but adjust the
                       *   rest of the dimensions to hold the complete
                       *   transformed image.
                       */
      AdjustDimensions /**< Adjust the dimension of the resulting image to 
                        *   contain the whole transformed image.  This implies
                        *   loosing the relative position of the origin.
                        */
      
    };

    /**
     * The parameters for the class geometricTransform
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
       * Resize mode.
       *
       * Choose how to deal with the final result dimensions.
       *
       * Default value: KeepDimensions
       */
      eResizeMode resizeMode;
    };

    /**
     * Default constructor
     */
    geometricTransformBase();

    /**
     * Construct a functor using the given parameters
     */
    geometricTransformBase(const parameters& par);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    geometricTransformBase(const geometricTransformBase& other);

    /**
     * Destructor
     */
    virtual ~geometricTransformBase();

    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    geometricTransformBase& copy(const geometricTransformBase& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    geometricTransformBase& operator=(const geometricTransformBase& other);

    /**
     * Returns the complete name of the functor class
     */
    virtual const std::string& name() const;

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual geometricTransformBase* clone() const = 0;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual geometricTransformBase* newInstance() const = 0;

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;

  protected:
    /**
     * Returns used parameters
     */
    parameters& getRWParameters();

  };

  /**
   * Class geometricTransform.
   *
   * This is an abstract template class, parent of all functors that achieve
   * geometric transformation of images, like affine or homogeneous
   * transformations.
   *
   * The template parameter I indicates the interpolator type to be used. 
   * The class provided must be inherited from lti::fixedGridInterpolation.
   * Note that the interpolator works for one type of containers only, and
   * only that type will be supported by this class too.
   *
   * @see geometricTransform<I>::parameters.
   *
   * @ingroup gGeometricTrans
   */
  template<typename T, template<class> class I = bilinearInterpolation> 
  class geometricTransform : public geometricTransformBase,
                             public matrixProcessingInterface<T> {
  public:
    typedef I<T> interpolator_type;
    typedef T value_type;
    
    /**
     * The parameters for the class geometricTransform
     */
    class parameters : public geometricTransformBase::parameters {
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
       * Instance of the interpolator parameters to be used.
       *
       * Default value: default parameters instance
       */
      typename interpolator_type::parameters interpolatorParams;
    };

    /**
     * Default constructor
     */
    geometricTransform();

    /**
     * Construct a functor using the given parameters
     */
    geometricTransform(const parameters& par);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    geometricTransform(const geometricTransform<T,I>& other);

    /**
     * Destructor
     */
    virtual ~geometricTransform();

    /**
     * Transform geometrically the given image and leave the result on the
     * same container.
     *
     * @param srcdest matrix<T> with the source data. The result
     *                will be left here too.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(matrix<value_type>& srcdest) const;

    /**
     * Transform geometrically the source image and leave the result on the
     * destination container.
     *
     * Operates on a copy of the given %parameters.
     *
     * @param src matrix<value_type> with the source data.
     * @param dest matrix<value_type> where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const matrix<value_type>& src, 
                             matrix<value_type>& dest) const = 0;


    /**
     * Transform geometrically the given image and leave the result on the
     * same container.
     *
     * If the parameters specify to AdjustDimensions, then the offset
     * value will contain the relative position of the \a srcdest
     * origin with respect to the original image coordinate system.
     * To all other resize policies, the value of offset is set to
     * (0,0).
     *
     * @param srcdest matrix<T> with the source data. The result
     *                will be left here too.
     * @param offset position of the origin of the result with respect to the
     *               coordinate system of the original image.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(matrix<value_type>& srcdest,
                       fpoint& offset) const;

    /**
     * Transform geometrically the source image and leave the result on the
     * destination container.
     *
     *
     * If the parameters specify to AdjustDimensions, then the offset
     * value will contain the relative position of the \a srcdest
     * origin with respect to the original image coordinate system.
     * To all other resize policies, the value of offset is set to
     * (0,0).
     *
     * @param src matrix<value_type> with the source data.
     * @param dest matrix<value_type> where the result will be left.
     * @param offset position of the origin of the result with respect to the
     *               coordinate system of the original image.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const matrix<value_type>& src, 
                             matrix<value_type>& dest,
                             fpoint& offset) const = 0;

    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    geometricTransform& copy(const geometricTransform<T,I>& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    geometricTransform& operator=(const geometricTransform<T,I>& other);

    /**
     * Returns the complete name of the functor class
     */
    virtual const std::string& name() const;

    /**
     * Returns a pointer to a clone of this functor.
     */
  virtual geometricTransform<T,I>* clone() const = 0;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual geometricTransform<T,I>* newInstance() const = 0;

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * Update parameters instance
     */
    virtual bool updateParameters();

    /**
     * Return a read-writable reference to the internal interpolator object.
     */
    interpolator_type& getInterpolator();

    /**
     * Return a read-only reference to the internal interpolator object.
     */
    const interpolator_type& getInterpolator() const;

  protected:
    /**
     * Returns used parameters
     */
    parameters& getRWParameters();

    /**
     * Interpolator instance with the appropriate parameters.
     *
     * The updateParameters method will ensure that the interpolator
     * parameters are set appropriatelly
     */
    interpolator_type interpolator_;

  }; // class geometricTransform<T,I>

  /**
   * Read the resize mode
   *
   * @ingroup gStorable
   */
  bool read(ioHandler& handler,geometricTransformBase::eResizeMode& data);

  /**
   * Write the resize mode
   *
   * @ingroup gStorable
   */
  bool write(ioHandler& handler,
             const geometricTransformBase::eResizeMode& data);
} // namespace lti

#include "ltiGeometricTransform_template.h"

#endif

