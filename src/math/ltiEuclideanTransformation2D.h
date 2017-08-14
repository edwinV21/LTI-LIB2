/*
 * Copyright (C) 2008
 * Pablo Alvarado
 *
 * This file is backported from the Computer Vision and Robotics Library
 * (CVR-Lib ) for the LTI-Lib-2
 *
 * The CVR-Lib is free software; you can redistribute it and/or
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
 * \file   ltiEuclideanTransformation2D.h
 *         Contains the class lti::euclideanTransformation2D,
 *         which is used to transform coordinate points in a planar
 *         transformation with only rotation and translation.
 *
 * \author Pablo Alvarado
 * \date   15.01.2008
 *
 * revisions ..: $Id: ltiEuclideanTransformation2D.h,v 1.1 2009-10-19 01:30:17 alvarado Exp $
 */

#ifndef _LTI_EUCLIDEAN_TRANSFORMATION2_D_H_
#define _LTI_EUCLIDEAN_TRANSFORMATION2_D_H_


#include "ltiLocation.h"
#include "ltiVector.h"
#include "ltiMatrix.h"
#include "ltiFunctor.h"
#include "ltiSinCos.h"
#include "ltiLinearLeastSquares.h"
#include <vector>
#include <limits>

namespace lti {

  /**
   * Class euclideanTransformation2D
   *
   * The euclidean transformation (also called isometric transformation) for 2D
   * spaces performs a planar modification of points in which only rotation and
   * translation are applied.
   *
   * This class transforms points only, and therefore is not intended for
   * images, in which case you should use lti::matrixTransform.  You can
   * however obtain from this class the matrix used by
   * lti::matrixTransform::parameters to generate the same results.
   *
   * Most apply methods are templates of a point type P, i.e. the type P is
   * usually a container that can be accessed with the operator[], like
   * the types lti::point<T>, lti::vector<T>.
   *
   * @see euclideanTransformation2D::parameters.
   * @see euclideanTransformation3D
   * @see similarityTransformation2D
   * @see affineTransformation2D
   * @see projectiveTransformation2D
   *
   * @ingroup gGeometricTrans
   */
  class euclideanTransformation2D : public functor {
  public:
    /**
     * The parameters for the class euclideanTransformation2D
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
       * Magnitude of the translation in x and y directions
       *
       * Default value: 0
       */
      fpoint translation;

      /**
       * Magnitude of the rotation, provided in radians.
       *
       * If the given value is greater than 2Pi then it is assumed to be given
       * in degrees.
       *
       * Default value: 0
       */
      float angle;

      /**
       * Parameters used in the linearLeastSquares functor for
       * the estimation tasks.
       *
       * Default value: default
       */
      linearLeastSquares::parameters llsParameters;
    };

    /**
     * Default constructor
     */
    euclideanTransformation2D();

    /**
     * Construct a functor using the given parameters
     */
    euclideanTransformation2D(const parameters& par);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    euclideanTransformation2D(const euclideanTransformation2D& other);

    /**
     * Destructor
     */
    virtual ~euclideanTransformation2D();

    /**
     * Transform the given point.
     *
     * @param srcdest point<T> with the source data. The result
     *                will be left here too.
     * @return true if apply successful or false otherwise.
     */
    template<class P>
    bool apply(P& srcdest) const;

    /**
     * Transform the point \a src and write the transformation on the \a dest
     * point.
     *
     * @param src point<T> with the source data.
     * @param dest point<T> where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    template<class P>
    bool apply(const P& src, P& dest) const;

    /**
     * Estimate a transformation that maps the first set of points into
     * the second set of points.
     *
     * The resulting estimation is left in the parameters.
     *
     * This method assumes that the corresponding point to setA[idx] is
     * stored in setB[idx].
     *
     * Only the first \c dof()/2 correspondences will be used for the
     * estimation, and you need at least \c dof()/2 elements for the
     * transformation to be computed.
     */
    template<class P>
    bool estimate(const std::vector<P>& setA,
                  const std::vector<P>& setB);


    /**
     * Do a linear least squares error minimization for a transformation that
     * maps the first set of points into the second set of points.
     *
     * The resulting estimation is left in the parameters.
     *
     * This method assumes that the corresponding point to setA[idx] is
     * stored in setB[idx].
     *
     * For the LLS to be computed, more than \c dof()/2 correspondences are
     * necessary for the estimation.
     */
    template<class P>
    bool estimateLLS(const std::vector<P>& setA,
                     const std::vector<P>& setB);


    /**
     * Do a linear least squares error minimization for a transformation that
     * maps the first set of points into the second set of points.
     *
     * The resulting estimation is left in the parameters.
     *
     * This method assumes that the corresponding point to setA[idx] is
     * stored in setB[idx].
     *
     * For the LLS to be computed, more than \c dof()/2 correspondences are
     * necessary for the estimation.
     *
     * For the estimation, just the points with the indices specified in 
     * \c selection are given.
     *
     * @return true if successful, false otherwise
     */
    template<class P>
    bool estimateLLS(const ivector& selection,
                     const std::vector<P>& setA,
                     const std::vector<P>& setB);

    /**
     * Degrees of freedom of the transformation.
     *
     * Usually this means the minimal number of parameters with which the
     * transforamtion matrix can be generated.
     */
    int dof() const;

    /**
     * Generate the transformation matrix that would be required by
     * lti::matrixTransform to generate the same coordinate transformation.
     * The result is, so to speak, the matrix representation of the parameters.
     *
     * @param mat matrix container of the result.
     */
    void generateMatrix(fmatrix& mat) const;

    /**
     * Generate the transformation matrix that would be required by
     * lti::matrixTransform to generate the same coordinate transformation.
     * The result is, so to speak, the matrix representation of the parameters.
     */
    fmatrix generateMatrix() const;

    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    euclideanTransformation2D& copy(const euclideanTransformation2D& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    euclideanTransformation2D&
    operator=(const euclideanTransformation2D& other);

    /**
     * Returns the complete name of the functor class
     */
    virtual const std::string& name() const;

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual euclideanTransformation2D* clone() const;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual euclideanTransformation2D* newInstance() const;

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * Update parameters
     */
    bool updateParameters();

  protected:

    /**
     * Returns used parameters
     */
    parameters& getRWParameters();

    /**
     * Cosine of the rotation angle
     */
    float cosa_;

    /**
     * Sine of the rotation angle
     */
    float sina_;

    /**
     * Shadow of the translation point
     */
    fpoint trans_;

    /**
     * Singular value decomposition
     *
     * Convenience method to avoid the SVD to be present in the template
     * implementation
     */
    linearLeastSquares lls_;
  };
}

#include "ltiEuclideanTransformation2D_template.h"

#endif

