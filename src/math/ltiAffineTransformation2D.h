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
 * \file   ltiAffineTransformation2D.h
 *         Contains the class lti::affineTransformation2D,
 *         which is used to transform coordinate points in a planar
 *         transformation with only rotation and translation.
 *
 * \author Pablo Alvarado
 * \date   15.01.2008
 *
 * revisions ..: $Id: ltiAffineTransformation2D.h,v 1.1 2009-10-19 01:30:17 alvarado Exp $
 */

#ifndef _LTI_AFFINE_TRANSFORMATION2_D_H_
#define _LTI_AFFINE_TRANSFORMATION2_D_H_


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
   * Class affineTransformation2D
   *
   * The affine transformation for 2D spaces performs a planar
   * modification of points in which only rotation, translation, 
   * (anisotropic) scaling and sheer are applied.
   * 
   * With homogeneous coordinates, the affine transformation can be stated as
   * \f[
   * \begin{bmatrix}
   *   x'\\y'\\1
   * \end{bmatrix} 
   * =
   * \begin{bmatrix}
   *   a_{00} & a_{01} & t_x \\
   *   a_{10} & a_{11} & t_y \\
   *   0      & 0      & 1
   * \end{bmatrix}
   * \begin{bmatrix}
   *   x\\y\\1
   * \end{bmatrix} 
   * \f]
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
   * @see affineTransformation2D::parameters
   * @see euclideanTransformation2D
   * @see affineTransformation2D
   *
   * @ingroup gGeometricTrans
   */
  class affineTransformation2D : public functor {
  public:
    /**
     * The parameters for the class affineTransformation2D
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
       * First element of the 2x2 submatrix
       *
       * Default value: 1
       */
      float a00;

      /**
       * Second element of the 2x2 submatrix
       *
       * Default value: 0
       */
      float a01;

      /**
       * Third element of the 2x2 submatrix
       *
       * Default value: 0
       */
      float a10;

      /**
       * Fourth element of the 2x2 submatrix
       *
       * Default value: 1
       */
      float a11;

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
    affineTransformation2D();

    /**
     * Construct a functor using the given parameters
     */
    affineTransformation2D(const parameters& par);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    affineTransformation2D(const affineTransformation2D& other);

    /**
     * Destructor
     */
    virtual ~affineTransformation2D();

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
     * This method is provided to keep consistency with other matrix
     * transformation functors, since in this case, the conversion between the
     * parameters and the matrix is completely straightforward.
     *
     * @param mat matrix container of the result.
     */
    void generateMatrix(fmatrix& mat) const;

    /**
     * Decompose matrix.
     *
     * This method decomposes the computed matrix as a series of simpler
     * processes.
     *
     * The affine transformation matrix H is interpreted as the 3x3 matrix
     *
     * \f[
     * H = \begin{bmatrix} A & t \\ 0^T & 1 \end{bmatrix} 
     * \f]
     *
     * where the 2x2 submatrix A can be decomposed as a series of pure
     * rotations, and asymmetrical scalations, as follow:
     * 
     * \f[
     * A = R(\theta) R(-\phi) D R(\phi)
     * \f]
     *
     * with
     *
     * \f[
     * D = \begin{bmatrix} \lambda_1 & 0 \\ 0 & \lambda_2 \end{bmatrix} 
     * \f]
     */
    void decompose(float& theta,
                   float& phi,
                   float& lambda1,
                   float& lambda2,
                   float& tx,
                   float& ty) const;

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
    affineTransformation2D& copy(const affineTransformation2D& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    affineTransformation2D&
    operator=(const affineTransformation2D& other);

    /**
     * Returns the complete name of the functor class
     */
    virtual const std::string& name() const;

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual affineTransformation2D* clone() const;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual affineTransformation2D* newInstance() const;

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
     * Shadow of the translation point
     */
    fpoint trans_;

    /**
     * Shadow of matrix element
     */
    float a00_;

    /**
     * Shadow of matrix element
     */
    float a01_;

    /**
     * Shadow of matrix element
     */
    float a10_;

    /**
     * Shadow of matrix element
     */
    float a11_;

    /**
     * Singular value decomposition
     *
     * Convenience method to avoid the SVD to be present in the template
     * implementation
     */
    linearLeastSquares lls_;
  };
}

#include "ltiAffineTransformation2D_template.h"

#endif

