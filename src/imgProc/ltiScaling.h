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
 * \file   ltiScaling.h
 *         This file contains the functor scaling which can be used to
 *         scale different types of matrices.
 * \author Pablo Alvarado
 * \date   21.11.2003
 *
 * $Id: ltiScaling.h,v 1.6 2012-10-31 03:26:42 alvarado Exp $
 */

#ifndef _LTI_SCALING_H_
#define _LTI_SCALING_H_

#include "ltiFunctor.h"
#include "ltiBoundaryType.h"

#include "ltiMatrixProcessingInterface.h"
#include "ltiBilinearInterpolation.h"

#include "ltiBiquadraticInterpolation.h"
#include "ltiBicubicInterpolation.h"
#include "ltiGenericInterpolation.h"
#include "ltiNearestNeighborInterpolation.h"
#include "ltiBicubicInterpolationWorkaround.h"

namespace lti {

  /**
   * Class lti::scaling
   *
   * Scaling implements a functor to rescale an image using a real valued
   * scaling factor.
   *
   * This functor represents a faster alternative to lti::matrixTransform, used
   * if you only need to scale the image without any other kind of geometric
   * mapping.  It is usually employed to upsample an image, using some
   * interpolation policy indicated in the parameters.
   *
   * You can give also factors smaller than one to downsample an image, but
   * this will just take the interpolated corresponding pixel of the original
   * image, without consideration of its neighborhood.  For a more
   * theoretically founded downsampling functor, you can use lti::downsampling,
   * which allows you to make a low-pass filtering, to avoid artifacts
   * resulting from a violation of the Nyquist sampling theorem.  If you need
   * to downsample by a non integer factor, then you should manually apply a
   * low-pass filter (or smoothing filter) to the image before scaling it down.
   *
   * If you just want to downsample for some visualization, this
   * functor might be enough (lti::decimation can be even faster if the
   * downsampling factor is an integer).
   *
   * For more complex operations you can still use the lti::matrixTransform 
   * functor, or one of its sibling classes.
   *
   * Here is shown an example for scaling a color image using the short cuts:
   * \code
   * // Scale up a color image
   * lti::scaling<rgbaPixel> colorScaler;
   * lti::image img,bigger;
   * // ... get or create the image img somehow
   * colorScaler.scale(1.5,img,bigger);
   * \endcode
   *
   * Here another example for the complete LTI-Lib interface, for a case with
   * float channels is provided:
   *
   * \code
   * // Scale up a channel
   * lti::scaling<float> scaler;
   * lti::scaling<float>::parameters scalerParam;
   * scalerParam.scale.set(1.5f,1.5f); // increase in 150%
   * scaler.setParameters(scalerParam);
   *
   * lti::channel chnl,bigger;
   * // ... get or create the channel chnl somehow
   * colorScaler.apply(chnl,bigger);
   * \endcode
   *
   * @see lti::matrixTransform, lti::rotation, lti::flipImage
   *
   * @see lti::downsampling, lti::upsampling, lti::filledUpsampling, 
   *      lti::decimation
   *
   * @see lti::scaling::parameters
   *
   * @ingroup gGeometry
   */
  template<typename T, template<class> class Inter = bilinearInterpolation> 
  class scaling : public functor,
                  public matrixProcessingInterface<T> {
  public:
    
    // shortcut
    typedef T value_type;
    
    /**
     * The parameters for the class scaling
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
       * Returns the name of this type.
       */
      virtual const std::string& name() const;

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
       * Scaling factor.
       *
       * Each axis of the image can be scaled by a different factor.  The
       * \a x component of this point will be the horizontal scaling factor and
       * the \a y component the vertical scaling factor.
       *
       * Only positive values are allowed.  You can flip the image using
       * lti::flipImage if you need to first. 
       *
       * With \a scale between 0 and 1 this functor does a down-sampling and
       * above 1.0 up-sampling. Read the notes in the functor description.
       *
       * Default value: fpoint(sqrt(2),sqrt(2))
       */
      fpoint scale;


      /**
       * Parameters for the used interpolation functor.
       * Note: interpolationParameters.boundaryType will be ignored
       * The internal interpolation functor will always use the
       * the boundaryType parameter of this class.
       */
      typename Inter<value_type>::parameters interpolationParameters;
      
      /**
       * how the boundaries will be used
       *
       * Default: eBoundaryType::Zero
       */
      eBoundaryType boundaryType;
      
    };

    /**
     * Default constructor
     */
    scaling();

    /**
     * Construct a functor using the given parameters
     */
    scaling(const parameters& par);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    scaling(const scaling& other);

    /**
     * Destructor
     */
    virtual ~scaling();

    /**
     * Returns the name of this type.
     */
    virtual const std::string& name() const;
    
    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    scaling& copy(const scaling& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    scaling& operator=(const scaling& other);

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual scaling<T,Inter>* clone() const;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual scaling<T,Inter>* newInstance() const;

    /**
     * Returns used parameters
     */
    const typename scaling<T,Inter>::parameters& getParameters() const;

    /**
     * updateParameters 
     */
    virtual bool updateParameters();

    /**
     * Shortcut for setting the boundary type of a functor derived from
     * modifier.
     *
     * \b Note: The boundaryType is modified via direct access to the
     * internal parameters object.  
     */
    bool setBoundaryType(const eBoundaryType boundaryType);

    /**
     * @name Standard apply methods.
     *
     * The scaling factor and interpolation types are taken from the
     * parameters.
     */
    //@{
    /**
     * Operates on the given %parameter.
     * @param srcdest image with the source data.  The result
     *                 will be left here too.
     * @return true if apply successful or false otherwise.
     */
    bool apply(matrix<T>& srcdest) const;

    /**
     * Operates on a copy of the given %parameters.
     * @param src image with the source data.
     * @param dest image where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const matrix<T>& src, matrix<T>& dest) const;
    //@}

    /**
     * @name Symmetric scaling functions.
     *
     * The scaling factor for both vertical and horizontal axes is given
     * directly.
     */
    //@{
    /**
     * Operates on the given %parameter.
     * @param scale scaling factor used for both, the horizontal and vertical
     *             axes.  The scaling factor in the parameters will be ignored.
     * @param srcdest matrix with the source data.  The result
     *                 will be left here too.
     * @return true if apply successful or false otherwise.
     */
    bool scale(const float scale,matrix<T>& srcdest) const;

    /**
     * Operates on a copy of the given %parameters.
     * @param scale scaling factor used for both, the horizontal and vertical 
     *            axes.  The scaling factor in the parameters will be ignored.
     * @param src image with the source data.
     * @param dest image where the result will be left.
     * @return true if scale successful or false otherwise.
     */
    bool scale(const float scale,const matrix<T>& src, matrix<T>& dest) const;
    //@}

    /**
     * @name Asymmetric scaling functions.
     *
     * The scaling factor for horizontal and vertical axes are given through
     * the x and y components of the point respectively.
     */
    //@{
    /**
     * Operates on the given %parameter.
     * @param scale scaling factor used instead of the value given in the
     *              parameters object.
     * @param srcdest image with the source data.  The result
     *                 will be left here too.
     * @return true if scale successful or false otherwise.
     */
    bool scale(const fpoint& scale, matrix<T>& srcdest) const;

    /**
     * Operates on a copy of the given %parameters.
     * @param scale scaling factor used instead of the value given in the
     *              parameters object.
     * @param src image with the source data.
     * @param dest image where the result will be left.
     * @return true if scale successful or false otherwise.
     */
    bool scale(const fpoint& scale,const matrix<T>& src,matrix<T>& dest) const;

    //@}


  protected:
    template <typename T2,template<class> class I>
    class scaleHelper {
    public:
      /**
       * Scales the source matrix to the size of the destination matrix.
       */
      bool operator() (const matrix<T2>& src, matrix<T2>& dest) const;
      I<T2> interpol_;
    };

    template <typename T2>
    class scaleHelper<T2,bilinearInterpolation> {
      friend class scaling<T,Inter>;
    public:
      bool operator() (const matrix<T2>& src, matrix<T2>& dest) const;
      bilinearInterpolation<T2> interpol_;
    };

    template <typename T2>
    class scaleHelper<T2,nearestNeighborInterpolation> {
    public:
      bool operator() (const matrix<T2>& src, matrix<T2>& dest) const;
      nearestNeighborInterpolation<T2> interpol_;
    };

    template <typename T2>
    class scaleHelper<T2,bicubicInterpolationWorkaround> {
    public:
      bool operator() (const matrix<T2>& src, matrix<T2>& dest) const;
      bicubicInterpolationWorkaround<T2> interpol_;
    };

    template <typename T2>
    class scaleHelper<T2,biquadraticInterpolation> {
    public:
      bool operator() (const matrix<T2>& src, matrix<T2>& dest) const;
      biquadraticInterpolation<T2> interpol_;
    };


    /**
     * Class containing the real algorithms.
     * This is set in updateParameters();
     */
    //scalingWorker* worker;
    

    /**
     * scaling helper class instance
     */
    scaleHelper<value_type, Inter> scale_;

    
  };
}

#include "ltiScaling_template.h"

#endif


