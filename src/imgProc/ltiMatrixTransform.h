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
 * \file   ltiMatrixTransform.h
 *         Contains the geometric transformation class lti::matrixTransform<T>
 *         which is the more generic linear transformation matrix.
 * \author Pablo Alvarado
 * \date   08.09.2007
 *
 * revisions ..: $Id: ltiMatrixTransform.h,v 1.15 2013-12-03 14:48:23 alvarado Exp $
 */

#ifndef _LTI_MATRIX_TRANSFORM_H_
#define _LTI_MATRIX_TRANSFORM_H_

#include "ltiMatrix.h"
#include "ltiPoint3D.h"
#include "ltiGeometricTransform.h"
#include "ltiPointList.h"

namespace lti {

  /**
   * Global scope function to create a rotation homogeneous matrix for
   * 3D spaces, which is always a 4x4 matrix.
   *
   * The template parameter T represents the type of the matrix entries, which
   * has to be a floating point type like double or float.
   *
   * @ingroup gGeometricTrans
   *
   * @param center Point in the space that is kept constant
   * @param axis Rotation axis.
   * @param angle Magnitude of the angle of the rotation
   * @return The 4x4 homogeneous matrix representing a 3D rotation.
   */
  template<typename T>
  matrix<T> rotationMatrix(const point3D<T>& center,
                     const point3D<T>& axis,
                     const T& angle);

  /**
   * Global scope function to create a rotation homogeneous matrix for
   * 2D spaces, which is always a 3x3 matrix.
   *
   * The template parameter T represents the type of the matrix entries, which
   * has to be a floating point type like double or float.
   *
   * @ingroup gGeometricTrans
   *
   * @param center Point in the space that is kept constant
   * @param angle Magnitude of the angle of the rotation
   * @return The 3x3 homogeneous matrix representing a 2D rotation.
   */
  template<typename T>
  matrix<T> rotationMatrix(const point<T>& center,
                     const T& angle);

  /**
   * Global scope function to create a shift homogeneous matrix in a
   * 3D space, which is always 4x4.
   *
   * The matrix created has the form
   *
   * \f[
   * \begin{bmatrix}
   *   1 & 0 & 0   & s_x \\
   *   0 & 1 & 0   & s_y \\
   *   0 & 0 & 1   & s_z \\
   *   0 & 0 & 0   & 1
   * \end{bmatrix}
   * \f]
   *
   * The template parameter T represents the type of the matrix entries, which
   * has to be a floating point type like double or float.
   *
   * @ingroup gGeometricTrans
   *
   * @param s Shift amount.
   * @return The 4x4 homogeneous matrix representing a 3D shift.
   */
  template<typename T>
  matrix<T> translationMatrix(const point3D<T>& s);

  /**
   * Global scope function to create a shift homogeneous matrix for
   * 2D spaces, which is always a 3x3 matrix.
   *
   * The matrix created has the form
   *
   * \f[
   * \begin{bmatrix}
   *   1 & 0 & s_x \\
   *   0 & 1 & s_y \\
   *   0 & 0 & 1 \\
   * \end{bmatrix}
   * \f]
   *
   * The template parameter T represents the type of the matrix entries, which
   * has to be a floating point type like double or float.
   *
   * @ingroup gGeometricTrans
   *
   * @param s Shift amount.
   * @return The 3x3 homogeneous matrix representing a 2D shift.
   */
  template<typename T>
  matrix<T> translationMatrix(const point<T>& s);

  /**
   * Global scope function to create a homogeneous matrix for scaling in
   * 3D spaces.
   *
   * The matrix created has the form
   *
   * \f[
   * \begin{bmatrix}
   *   s_x & 0 & 0   & 0 \\
   *   0 & s_y & 0   & 0 \\
   *   0 & 0   & s_z & 0 \\
   *   0 & 0   & 0   & 1
   * \end{bmatrix}
   * \f]
   *
   * The template parameter T represents the type of the matrix entries, which
   * has to be a floating point type like double or float.
   *
   * @ingroup gGeometricTrans
   *
   * @param s Scale amount in each axis.
   * @return The 4x4 homogeneous matrix representing a 3D scale
   */
  template<typename T>
  matrix<T> scalingMatrix(const point3D<T>& s);

  /**
   * Global scope function to create a homogeneous matrix for scaling in
   * 2D spaces.
   *
   * The matrix created has the form
   *
   * \f[
   * \begin{bmatrix}
   *   s_x & 0   & 0 \\
   *   0   & s_y & 0 \\
   *   0   & 0   & 1
   * \end{bmatrix}
   * \f]
   *
   * The template parameter T represents the type of the matrix entries, which
   * has to be a floating point type like double or float.
   *
   * @ingroup gGeometricTrans
   *
   * @param s Scale amount in each axis.
   * @return The 3x3 homogeneous matrix representing a 2D scale
   */
  template<typename T>
  matrix<T> scalingMatrix(const point<T>& s);

  /**
   * Global scope function to create a homogeneous matrix for scaling in
   * 2D spaces.
   *
   * The matrix created has the form
   *
   * \f[
   * \begin{bmatrix}
   *   s  & 0  & 0 \\
   *   0  & s  & 0 \\
   *   0  & 0  & 1
   * \end{bmatrix}
   * \f]
   *
   * The template parameter T represents the type of the matrix entries, which
   * has to be a floating point type like double or float.
   *
   * @ingroup gGeometricTrans
   *
   * @param s Scale amount in each axis.
   * @return The 3x3 homogeneous matrix representing a 2D scale
   */
  template<typename T>
  matrix<T> scalingMatrix(const T& s);

  /**
   * Global scope function to create a homogeneous projection matrix, with
   * a focal distance f.
   *
   * The matrix created has the form
   *
   * \f[
   * \begin{bmatrix}
   *   1 & 0 & 0   & 0 \\
   *   0 & 1 & 0   & 0 \\
   *   0 & 0 & 1   & 0 \\
   *   0 & 0 & 1/f & 1
   * \end{bmatrix}
   * \f]
   *
   * The template parameter T represents the type of the matrix entries, which
   * has to be a floating point type like double or float.
   *
   * @ingroup gGeometricTrans
   *
   * @param f focal distance.
   * @return The 4x4 homogeneous matrix representing a 3D scale
   */
  template<typename T>
  matrix<T> projection(const T& f);

  /**
   * Class matrixTransform.
   *
   * This is a template class, used to geometrically transform an
   * image, channel or just a matrix using a linear transformation
   * expressed through a transformation matrix.
   *
   * Additionally, backwards() and forwards() methods allow you to
   * transform points or lists of points.
   *
   * \section suptrans Supported transformations.
   *
   * The transformation matrix is provided in the parameters.  It has to be
   * invertible, and has to have a size of 2x2, 2x3, 3x3, 4x4 or 4x3.
   *
   * \subsection m2x2 Simple 2x2 transformation matrix
   *
   * The 2x2 transformation matrix transforms a pixel at \f$(x,y)\f$
   * into a pixel \f$(x',y')\f$ with the following convention:
   * \f[
   *   \begin{bmatrix}
   *     x' \\ y'
   *   \end{bmatrix}
   *   =
   *   \begin{bmatrix}
   *     m_{0,0} & m_{0,1} \\
   *     m_{1,0} & m_{1,1}
   *   \end{bmatrix}
   *   \begin{bmatrix}
   *     x \\ y
   *   \end{bmatrix}
   *   =
   *   \begin{bmatrix}
   *     m_{0,0}x + m_{0,1}y \\
   *     m_{1,0}x + m_{1,1}y
   *   \end{bmatrix}
   * \f]
   *
   * It is usefull for rotation and scaling operations.
   *
   * \subsection m2x3 2x3 transformation matrix
   *
   * The 2x3 transformation matrix uses homogeneous coordinates to
   * transform a pixel at \f$(x,y)\f$ into a pixel \f$(x',y')\f$ with
   * the following convention:
   * \f[
   *   \begin{bmatrix}
   *     x' \\ y'
   *   \end{bmatrix}
   *   =
   *   \begin{bmatrix}
   *     m_{0,0} & m_{0,1} & m_{0,2}\\
   *     m_{1,0} & m_{1,1} & m_{1,2}
   *   \end{bmatrix}
   *   \begin{bmatrix}
   *     x \\ y \\ 1
   *   \end{bmatrix}
   *   =
   *   \begin{bmatrix}
   *     m_{0,0}x + m_{0,1}y + m_{0,2} \\
   *     m_{1,0}x + m_{1,1}y + m_{1,2}
   *   \end{bmatrix}
   * \f]
   *
   * It is usefull for rotation, scaling and translation operations.
   *
   * \subsection m3x3 3x3 transformation matrix
   *
   * The 3x3 transformation matrix uses homogeneous coordinates to
   * transform a pixel at \f$(x,y)\f$ into a pixel \f$(x',y')\f$ with
   * the following convention:
   * \f[
   *   \begin{bmatrix}
   *     x'' \\ y'' \\ \alpha
   *   \end{bmatrix}
   *   =
   *   \begin{bmatrix}
   *     m_{0,0} & m_{0,1} & m_{0,2}\\
   *     m_{1,0} & m_{1,1} & m_{1,2}\\
   *     m_{2,0} & m_{2,1} & m_{2,2}
   *   \end{bmatrix}
   *   \begin{bmatrix}
   *     x \\ y \\ 1
   *   \end{bmatrix}
   *   =
   *   \begin{bmatrix}
   *     m_{0,0}x + m_{0,1}y + m_{0,2} \\
   *     m_{1,0}x + m_{1,1}y + m_{1,2} \\
   *     m_{2,0}x + m_{2,1}y + m_{2,2}
   *   \end{bmatrix}
   * \f]
   *
   * The point \f$(x',y')\f$ is obtained with
   * \f$(x',y') = (x''/\alpha,y''/\alpha)\f$
   * 
   * It can be used for rotation, scaling and translation operations, but using
   * the third line, more interesting mappings can be achieved.
   *
   * \subsection m3x4 3x4 transformation matrix
   *
   * The 3x4 transformation matrix uses homogeneous coordinates to
   * transform a pixel at \f$(x,y)\f$ into a pixel \f$(x',y')\f$ with
   * the following convention:
   * \f[
   *   \begin{bmatrix}
   *     x'' \\ y'' \\ \alpha
   *   \end{bmatrix}
   *   =
   *   \begin{bmatrix}
   *     m_{0,0} & m_{0,1} & m_{0,2} & m_{0,3}\\
   *     m_{1,0} & m_{1,1} & m_{1,2} & m_{1,3}\\
   *     m_{2,0} & m_{2,1} & m_{2,2} & m_{2,3}
   *   \end{bmatrix}
   *   \begin{bmatrix}
   *     x \\ y \\ 0 \\ 1
   *   \end{bmatrix}
   *   =
   *   \begin{bmatrix}
   *     m_{0,0}x + m_{0,1}y + m_{0,3} \\
   *     m_{1,0}x + m_{1,1}y + m_{1,3} \\
   *     m_{2,0}x + m_{2,1}y + m_{2,3}
   *   \end{bmatrix}
   * \f]
   *
   * The point \f$(x',y')\f$ is obtained with
   * \f$(x',y') = (x''/\alpha,y''/\alpha)\f$.
   * 
   * It can be used for rotation, scaling and translation operations, but using
   * the third line, more interesting mappings can be achieved.  
   *
   * Please note that this configuration is in principle equivalent to
   * a 3x3 matrix, as the third column is "absorved" by the z=0
   * assumption for the pixel positions on the image plane.
   *
   * \subsection m4x4 4x4 transformation matrix
   *
   * The 4x4 transformation matrix uses homogeneous coordinates to
   * transform a pixel at \f$(x,y)\f$ into a pixel \f$(x',y')\f$ with
   * the following convention:
   * \f[
   *   \begin{bmatrix}
   *     x'' \\ y'' \\ z'' \\ \alpha
   *   \end{bmatrix}
   *   =
   *   \begin{bmatrix}
   *     m_{0,0} & m_{0,1} & m_{0,2} & m_{0,3}\\
   *     m_{1,0} & m_{1,1} & m_{1,2} & m_{1,3}\\
   *     m_{2,0} & m_{2,1} & m_{2,2} & m_{2,3}\\
   *     m_{3,0} & m_{3,1} & m_{3,2} & m_{3,3}
   *   \end{bmatrix}
   *   \begin{bmatrix}
   *     x \\ y \\ 0 \\ 1
   *   \end{bmatrix}
   *   =
   *   \begin{bmatrix}
   *     m_{0,0}x + m_{0,1}y + m_{0,3} \\
   *     m_{1,0}x + m_{1,1}y + m_{1,3} \\
   *     m_{2,0}x + m_{2,1}y + m_{2,3} \\
   *     m_{3,0}x + m_{3,1}y + m_{3,3} \\
   *   \end{bmatrix}
   * \f]
   *
   * The point \f$(x',y')\f$ is obtained with
   * \f$(x',y') = (x''/\alpha,y''/\alpha)\f$.
   * 
   * It can be used for rotation, scaling and translation operations,
   * but using the z coordinates, more interesting mappings can be
   * achieved, like perspective projections.
   *
   * \subsection m4x3 4x3 transformation matrix
   *
   * The 4x3 transformation matrix uses homogeneous coordinates to
   * transform a pixel at \f$(x,y)\f$ into a pixel \f$(x',y')\f$ with
   * the following convention:
   * \f[
   *   \begin{bmatrix}
   *     x'' \\ y'' \\ z'' \\ \alpha
   *   \end{bmatrix}
   *   =
   *   \begin{bmatrix}
   *     m_{0,0} & m_{0,1} & m_{0,2}\\
   *     m_{1,0} & m_{1,1} & m_{1,2}\\
   *     m_{2,0} & m_{2,1} & m_{2,2}\\
   *     m_{3,0} & m_{3,1} & m_{3,2}
   *   \end{bmatrix}
   *   \begin{bmatrix}
   *     x \\ y \\ 1
   *   \end{bmatrix}
   *   =
   *   \begin{bmatrix}
   *     m_{0,0}x + m_{0,1}y + m_{0,2} \\
   *     m_{1,0}x + m_{1,1}y + m_{1,2} \\
   *     m_{2,0}x + m_{2,1}y + m_{2,2} \\
   *     m_{3,0}x + m_{3,1}y + m_{3,2} \\
   *   \end{bmatrix}
   * \f]
   *
   * The point \f$(x',y')\f$ is obtained with
   * \f$(x',y') = (x''/\alpha,y''/\alpha)\f$.
   * 
   * It can be used for rotation, scaling and translation operations,
   * but using the z coordinates, more interesting mappings can be
   * achieved, like perspective projections.
   *
   * \section predefmat Frequently used matrices
   *
   * Some frequently used matrices can be created with global functions
   *
   * @see rotationMatrix, translationMatrix, scalingMatrix, projection
   *
   * \section mtexam Example
   *
   * The following example rotates an image on the axis parallel to
   * the y axis but passing through the middle of the image.
   *
   * \code
   *   // type definitions just for shorter names.
   *   typedef lti::matrixTransform< lti::rgbaPixel, 
   *                            lti::nearestNeighborInterpolation > trans_type;
   *   trans_type transformer;
   *
   *   // get an image to work with
   *   lti::ioImage loader;
   *   lti::viewer2D view("Image");
   *   lti::image img,img2; 
   *   if (!loader.load("../img/testImg.bmp",img)) {
   *     std::cout << loader.getStatusString() << std::endl;
   *     exit(1);
   *   }
   *  
   *   lti::fmatrix mat,pro;
   *
   *   // generate a projection matrix to give a perspective effect
   *   pro = lti::projection(1000.0f);
   *
   *   lti::ipoint o = img.size()/2;
   *
   *   // animation of 360 images rotated.
   *   for (int deg=0;deg<=360;++deg) {
   *     float rad=degToRad(static_cast<float>(deg)); // angle in radians
   *
   *     // generate a transformation matrix using several elemental operations
   *     mat = 
   *       lti::translationMatrix(fpoint3D(o.x,o.y,0)) *
   *       pro * 
   *       lti::rotationMatrix(fpoint3D(0.0f,0.0f,0.0f),fpoint3D(1,0,0),rad) *
   *       lti::translationMatrix(fpoint3D(-o.x,-o.y,0));
   *
   *       transformer.setMatrix(mat);
   *       transformer.apply(img,img2);
   *
   *       view.show(img2);
   *   }
   * \endcode
   *
   * \section tparams Template parameter
   *
   * The template parameter I indicates the interpolator type to be used. 
   * The class provided must be inherited from lti::fixedGridInterpolation.
   * Note that the interpolator works for one type of containers only, and
   * only that type will be supported by this class too.
   *
   * @see matrixTransform<I>::parameters.
   *
   * @ingroup gGeometricTrans
   */
  template<typename T, template<class> class I = bilinearInterpolation> 
  class matrixTransform : public geometricTransform<T,I> {
  public:
    typedef T value_type;
    typedef I<T> interpolator_type;

    /**
     * The parameters for the class matrixTransform<T,I>
     */
    class parameters : public geometricTransform<T,I>::parameters {
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
       * Transformation matrix
       *
       * The transformation matrix have to be invertible, and need to have 
       * size 2x2, 2x3, 3x3, 4x4 or 4x3.
       *
       * Default value: 2x2 identity matrix
       */
      fmatrix transformation;
    };

    /**
     * Default constructor
     */
    matrixTransform();

    /**
     * Construct a functor using the given parameters
     */
    matrixTransform(const parameters& par);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    matrixTransform(const matrixTransform<T,I>& other);

    /**
     * Destructor
     */
    virtual ~matrixTransform();

    /**
     * @name Methods to transform matrices or images linearly.
     */
    //@{
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
                             matrix<value_type>& dest) const;

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
     * If you need to find out where in the rotated image is located the origin
     * of the initial image, then that would be in -offset.
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
     * value will contain the relative position of the \a dest
     * origin with respect to the original image coordinate system.
     * To all other resize policies, the value of offset is set to
     * (0,0).
     *
     * If you need to find out where in the rotated image is located the origin
     * of the initial image, then that would be in -offset.
     *
     * For the three dimensional transformations (i.e. transformation matrices
     * of sizes 4x4 and 4x3) it will be assumed that the _source_ image
     * has a z coordinate equal zero.  Hence, the z coordinate of each pixel in
     * the destination image is implicitely computed to fulfill that criterion.
     *
     * @param src matrix<value_type> with the source data.
     * @param dest matrix<value_type> where the result will be left.
     * @param offset position of the origin of the result with respect to the
     *               coordinate system of the original image.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const matrix<value_type>& src, 
                             matrix<value_type>& dest,
                             fpoint& offset) const;


    /**
     * Transform an image according to the matrix provided in the parameters.
     *
     * The offset provides information of the coordinate values of
     * the resulting image with respect to the coordinate system of
     * the original image.  Of course, this will be (0,0) if
     * KeepDimensions or KeepOrigin are chosen in the parameter.
     *
     * For the three dimensional transformations (i.e. transformation matrices
     * of sizes 4x4 and 4x3) it will be assumed that the _source_ image
     * has a z coordinate equal zero.  Hence, the z coordinate of each pixel in
     * the image is computed to fulfill that criterion.
     *
     * This method provides the computation of the z component, which
     * is usually discarded by the other methods.  This method makes
     * sense only for 4x4 transformation matrices.  For all other
     * matrices types, the resulting method will assign zeros to the z
     * component.
     *
     * If you need to find out where in the rotated image is located the
     * origin of the initial image, then that would be in -offset.
     *
     * @param src    original image
     * @param dest   transformed image
     * @param z      z component of the original image pixels, such that
     *               the destination z component is zero.
     * @param offset coordinate in the original reference system of the 
     *               new image pixel (0,0).
     */
    virtual bool apply(const matrix<value_type>& src,
                       matrix<value_type>& dest,
                       matrix<float>& z,
                       fpoint& offset) const;


    //@}

    /**
     * @name Point transformation tools
     *
     * The following tools permit to transform individual points, which is
     * useful when looking for the direct and inverse transformations of
     * particular pixels.
     *
     * Before using any of this methods you MUST call the use() method first,
     * in order for this class to know in what context it has to work, i.e.,
     * since the position of a pixel in the transformed matrix depends entirely
     * on the dimension of the original image and the parameter settings, the
     * use() method precomputes everything necessary to later transform each
     * pixel more efficiently.
     */
    //@{

    /**
     * Provide the size of the matrix from which it will be assumed the
     * positions will be taken.
     *
     * @param size Size of the matrix from which the positions will be analyzed
     * @return \c true if everything is ok, or  \c false if the matrix set in
     *         the parameters is not invertible or some other weird problem.
     */
    bool use(const ipoint& size);

    /**
     * Transform the given point coordinates.
     *
     * \warning Do not forget to provide first the size of the matrix
     *          from which the points are going to be taken, using 
     *          use(const ipoint&).
     *          
     * This method takes the orig position, assumed to be in a reference system
     * of a matrix/image of the size given by the use(const ipoint&) method, 
     * and transforms this position according to the parameters set.
     */
    void forwards(const fpoint& orig,
                        fpoint& dest) const;


    /**
     * Inverse transform the given point coordinates.
     *
     * \warning Do not forget to provide first the size of the matrix
     *          from which the points are going to be taken, using 
     *          use(const ipoint&).
     *          
     * This method is the inverse of forwards(const fpoint&,fpoint&).  Given a
     * position \c dest in the transformed system, it computes the coordinates
     * of that point in the original coordinate system.
     *
     * @param dest Position of a point in the transformed space.
     * @param orig Position of the point in the original space.
     */
    void backwards(const fpoint& dest,
                         fpoint& orig) const;


    /**
     * Transform the given list of points.
     *
     * \warning Do not forget to provide first the size of the matrix
     *          from which the points are going to be taken, using 
     *          use(const ipoint&).
     *          
     * This method takes each point at the orig position, assumed to
     * be in a reference system of a matrix/image of the size given by
     * the use(const ipoint&) method, and transforms it 
     * according to the parameters set, leaving it in the dest list.
     *
     * @param orig List of points in the original space.
     * @param dest List of points in the transformed space.
     */
    void forwards(const fpointList& orig,
                        fpointList& dest) const;


    /**
     * Inverse transform the given point coordinates.
     *
     * \warning Do not forget to provide first the size of the matrix
     *          from which the points are going to be taken, using 
     *          use(const ipoint&).
     *          
     * This method is the inverse of forwards(const fpointList&,fpointList&).
     * Given a list of points \c dest in the transformed system, it computes
     * the coordinates of those points in the original coordinate
     * system.
     *
     * @param dest List of points in the transformed space.
     * @param orig List of points in the original space.
     */
    void backwards(const fpointList& dest,
                         fpointList& orig) const;


    /**
     * Transform each element point of the vector
     *
     * \warning Do not forget to provide first the size of the matrix
     *          from which the points are going to be taken, using 
     *          use(const ipoint&).
     *          
     * This method takes each point at the orig position, assumed to
     * be in a reference system of a matrix/image of the size given by
     * the use(const ipoint&) method, and transforms it 
     * according to the parameters set, leaving it in the dest vector.
     *
     * @param orig Vector of points in the original space.
     * @param dest Vector of points in the transformed space.
     */
    void forwards(const vector<fpoint>& orig,
                        vector<fpoint>& dest) const;


    /**
     * Inverse transform each element point of the vector
     *
     * \warning Do not forget to provide first the size of the matrix
     *          from which the points are going to be taken, using 
     *          use(const ipoint&).
     *          
     * This method is the inverse of 
     * forwards(const vector<fpoint>&,vector<fpoint>&).
     * Given a vector of points \c dest in the transformed system, it computes
     * the coordinates of those points in the original coordinate system.
     *
     * @param dest Vector of points in the transformed space.
     * @param orig Vector of points in the original space.
     */
    void backwards(const vector<fpoint>& dest,
                         vector<fpoint>& orig) const;


    /**
     * Transform the given list of points.
     *
     * \warning Do not forget to provide first the size of the matrix
     *          from which the points are going to be taken, using 
     *          use(const ipoint&).
     *          
     * This method takes each point at the orig position, assumed to
     * be in a reference system of a matrix/image of the size given by
     * the use(const ipoint&) method, and transforms it 
     * according to the parameters set, leaving it in the dest list.
     *
     * This method uses rounding of the coordinates.
     *
     * @param orig List of points in the original space.
     * @param dest List of points in the transformed space.
     */
    void forwards(const ipointList& orig,
                        ipointList& dest) const;


    /**
     * Inverse transform the given point coordinates.
     *
     * \warning Do not forget to provide first the size of the matrix
     *          from which the points are going to be taken, using 
     *          use(const ipoint&).
     *          
     * This method is the inverse of forwards(const fpointList&,fpointList&).
     * Given a list of points \c dest in the transformed system, it computes
     * the coordinates of those points in the original coordinate
     * system.
     *
     * This method uses rounding of the coordinates.
     *
     * @param dest Position of a point in the transformed space.
     * @param orig Position of the point in the original space.
     */
    void backwards(const ipointList& dest,
                         ipointList& orig) const;    


    /**
     * Transform each element point of the vector
     *
     * \warning Do not forget to provide first the size of the matrix
     *          from which the points are going to be taken, using 
     *          use(const ipoint&).
     *          
     * This method takes each point at the orig position, assumed to
     * be in a reference system of a matrix/image of the size given by
     * the use(const ipoint&) method, and transforms it 
     * according to the parameters set, leaving it in the dest vector.
     *
     * @param orig Vector of points in the original space.
     * @param dest Vector of points in the transformed space.
     */
    void forwards(const vector<ipoint>& orig,
                        vector<ipoint>& dest) const;


    /**
     * Inverse transform each element point of the vector
     *
     * \warning Do not forget to provide first the size of the matrix
     *          from which the points are going to be taken, using 
     *          use(const ipoint&).
     *          
     * This method is the inverse of 
     * forwards(const vector<fpoint>&,vector<fpoint>&).
     * Given a vector of points \c dest in the transformed system, it computes
     * the coordinates of those points in the original coordinate system.
     *
     * @param dest Vector of points in the transformed space.
     * @param orig Vector of points in the original space.
     */
    void backwards(const vector<ipoint>& dest,
                         vector<ipoint>& orig) const;

    //@}

    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    matrixTransform<T,I>& copy(const matrixTransform<T,I>& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    matrixTransform<T,I>& operator=(const matrixTransform<T,I>& other);

    /**
     * Returns the complete name of the functor class
     */
    virtual const std::string& name() const;

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual matrixTransform<T,I>* clone() const;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual matrixTransform<T,I>* newInstance() const;

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * Update functor state on a parameters change
     */
    virtual bool updateParameters();

    /**
     * Shortcut for setting the transformation matrix without changing
     * the rest of the parameters.
     *
     * The matrix has to have valid dimensions: 2x2, 2x3, 3x3, 4x4 or 4x3.
     */
    bool setMatrix(const fmatrix& transMat);

  protected:

    /**
     * Returns used parameters
     */
    parameters& getRWParameters();

    /**
     * Base class of helpers
     *
     * This is the parent class of all helper classes, each one
     * designed to optimize the computation of the transformation for
     * a particular matrix size.
     */
    class helperBase {
    public:
      /**
       * The only available constructor forces to provide the
       * interpolator of the enclosing class, which is inherited from the
       * geometricTransform class.
       */
      helperBase(interpolator_type& interp,const parameters& par);

      /**
       * Virtual destructor
       */
      virtual ~helperBase();

      /**
       * Analyze the matrix in the parameters and compute its inverse.
       * If the matrix is not invertible, then return false.
       */
      virtual bool analyzeMatrix() = 0;

      /**
       * Transform an image according to the matrix provided in the parameters.
       *
       * The offset provides information of the coordinate values of
       * the resulting image with respect to the coordinate system of
       * the original image.  Of course, this will be (0,0) if
       * KeepDimensions or KeepOrigin are chosen in the parameter.
       *
       * If you need to find out where in the rotated image is located the
       * origin of the initial image, then that would be in -offset.
       *
       * @param src    original image
       * @param dest   transformed image
       * @param offset coordinate in the original reference system of the 
       *               new image pixel (0,0).
       */
      virtual bool apply(const matrix<value_type>& src,
                               matrix<value_type>& dest,
                               fpoint& offset) const = 0;

      /**
       * Transform an image according to the matrix provided in the parameters.
       *
       * The offset provides information of the coordinate values of
       * the resulting image with respect to the coordinate system of
       * the original image.  Of course, this will be (0,0) if
       * KeepDimensions or KeepOrigin are chosen in the parameter.
       *
       * This method provides the computation of the z component, which is
       * usually discarded.  This method makes sense only for 4x4 matrices.
       * For all other matrices types, the default implementation calls
       * the usual apply method and sets the z component to zero.
       *
       * If you need to find out where in the rotated image is located the
       * origin of the initial image, then that would be in -offset.
       *
       * @param src    original image
       * @param dest   transformed image
       * @param z      z component
       * @param offset coordinate in the original reference system of the 
       *               new image pixel (0,0).
       */
      virtual bool apply(const matrix<value_type>& src,
                               matrix<value_type>& dest,
                               matrix<float>& z,
                               fpoint& offset) const;

      /**
       * Transform the given point coordinates.
       *
       * \warning Do not forget to provide first the size of the matrix
       *          from which the points are going to be taken, using 
       *          use(const ipoint&).
       *          
       * This method takes the orig position, assumed to be in a reference
       * system of a matrix/image of the size given by the use(const ipoint&)
       * method, and transforms this position according to the set parameters.
       */
      virtual void forwards(const fpoint& orig,
                            fpoint& dest,
                            const fpoint& offset) const=0;


      /**
       * Inverse transform the given point coordinates.
       *
       * \warning Do not forget to provide first the size of the matrix
       *          from which the points are going to be taken, using 
       *          use(const ipoint&).
       *          
       * This method is the inverse of forwards(const fpoint&,fpoint&).  Given a
       * position \c dest in the transformed system, it computes the
       * coordinates of that point in the original coordinate system.
       *
       * @param dest Position of a point in the transformed space.
       * @param orig Position of the point in the original space.
       * @param offset Position of the origin of the result with respect to the
       *               coordinate system of the original image.
       */
      virtual void backwards(const fpoint& dest,
                             fpoint& orig,
                             const fpoint& offset) const=0;


      /**
       * After the matrix has been analyzed, this method can be used to
       * compute the dimensions of the resulting image and the offset.
       *
       * If you need to find out where in the rotated image is located the
       * origin of the initial image, then that would be in -offset.
       *
       * Usually, this method just need to compute the coordinates of
       * the corners internally, optimizing the computation according to the
       * matrix employed, followed by a call the the getDimsFromCorners().
       */
      virtual bool evalDims(const ipoint& orig,
                                  ipoint& res,
                                  fpoint& offset) const = 0;

    protected:
      /**
       * Reference to the interpolator object
       */
      const interpolator_type& interpolator_;

      /**
       * Reference to the parameter objects
       */
      const parameters& params_;

      /**
       * From the transformed coordinates of the corners, compute the
       * result image dimension and the offset of the origin of that
       * image.
       *
       * If you need to find out where in the rotated image is located the
       * origin of the initial image, then that would be in -offset.
       *
       * Returns true if the image size is possible or false if the required
       * size is negative (can happen for KeepOrigin).
       */
      bool getDimsFromCorners(const fpoint& tl,
                              const fpoint& tr,
                              const fpoint& bl,
                              const fpoint& br,
                              ipoint& dim,
                              fpoint& offset) const;

    };

    /**
     * Helper for 2x2 matrix transformations
     */
    class helper2x2 : public helperBase {
    public:
     /**
       * The only available constructor.
       */
      helper2x2(interpolator_type& interp,const parameters& par);

      /**
       * Virtual destructor
       */
      virtual ~helper2x2();

      /**
       * Analyze the matrix in the parameters and compute its inverse.
       * If the matrix is not invertible, then return false.
       */
      virtual bool analyzeMatrix();

      /**
       * Transform an image according to the matrix provided in the parameters
       */
      virtual bool apply(const matrix<value_type>& src,
                               matrix<value_type>& dest,
                               fpoint& offset) const;

      /**
       * After the matrix has been analyzed, this method can be used to
       * compute the dimensions of the resulting image and the offset.
       */
      virtual bool evalDims(const ipoint& orig,
                                  ipoint& res,
                                  fpoint& offset) const;

      /**
       * Transform the given point coordinates.
       *
       * \warning Do not forget to provide first the size of the matrix
       *          from which the points are going to be taken, using 
       *          use(const ipoint&).
       *          
       * This method takes the \a orig position, assumed to be in a reference
       * system of a matrix/image of the size given by the use(const ipoint&)
       * method, and transforms this position according to the set parameters.
       *
       * @param dest Position of a point in the transformed space.
       * @param orig Position of the point in the original space.
       * @param offset Position of the origin of the transformed space with 
       *               respect to the coordinate system of the original image.
       */
      virtual void forwards(const fpoint& orig,
                            fpoint& dest,
                            const fpoint& offset) const;


      /**
       * Inverse transform the given point coordinates.
       *
       * \warning Do not forget to provide first the size of the matrix
       *          from which the points are going to be taken, using 
       *          use(const ipoint&).
       *          
       * This method is the inverse of forwards(const fpoint&,fpoint&).  Given
       * a position \c dest in the transformed system, it computes the
       * coordinates of that point in the original coordinate system.
       *
       * @param dest Position of a point in the transformed space.
       * @param orig Position of the point in the original space.
       * @param offset Position of the origin of the transformed space with 
       *               respect to the coordinate system of the original image.
       */
      virtual void backwards(const fpoint& dest,
                             fpoint& orig,
                             const fpoint& offset) const;
    private:
      /**
       * @name Matrix entries
       */
      //@{
      float m00_,m01_;
      float m10_,m11_;
      //@}
    };

    /**
     * Helper for 2x3 matrix transformations
     */
    class helper2x3 : public helperBase {
    public:
      /**
       * The only available constructor.
       */
      helper2x3(interpolator_type& interp,const parameters& par);

      /**
       * Virtual destructor
       */
      virtual ~helper2x3();

      /**
       * Analyze the matrix in the parameters and compute its inverse.
       * If the matrix is not invertible, then return false.
       */
      virtual bool analyzeMatrix();

      /**
       * Transform an image according to the matrix provided in the parameters
       */
      virtual bool apply(const matrix<value_type>& src,
                               matrix<value_type>& dest,
                               fpoint& offset) const;

      /**
       * After the matrix has been analyzed, this method can be used to
       * compute the dimensions of the resulting image and the offset.
       */
      virtual bool evalDims(const ipoint& orig,
                                  ipoint& res,
                                  fpoint& offset) const;
      /**
       * Transform the given point coordinates.
       *
       * \warning Do not forget to provide first the size of the matrix
       *          from which the points are going to be taken, using 
       *          use(const ipoint&).
       *          
       * This method takes the orig position, assumed to be in a reference
       * system of a matrix/image of the size given by the use(const ipoint&)
       * method, and transforms this position according to the set parameters.
       */
      virtual void forwards(const fpoint& orig,
                            fpoint& dest,
                            const fpoint& offset) const;


      /**
       * Inverse transform the given point coordinates.
       *
       * \warning Do not forget to provide first the size of the matrix
       *          from which the points are going to be taken, using 
       *          use(const ipoint&).
       *          
       * This method is the inverse of forwards(const fpoint&,fpoint&).  Given
       * a position \c dest in the transformed system, it computes the
       * coordinates of that point in the original coordinate system.
       *
       * @param dest Position of a point in the transformed space.
       * @param orig Position of the point in the original space.
       * @param offset Position of the origin of the result with respect to the
       *               coordinate system of the original image.
       */
      virtual void backwards(const fpoint& dest,
                             fpoint& orig,
                             const fpoint& offset) const;
    private:
      /**
       * @name Matrix entries
       */
      //@{
      float m00_,m01_,m02_;
      float m10_,m11_,m12_;
      //@}
    };

    /**
     * Helper for 3x3 matrix transformations
     */
    class helper3x3 : public helperBase {
    public:
      /**
       * The only available constructor.
       */
      helper3x3(interpolator_type& interp,const parameters& par);

      /**
       * Virtual destructor
       */
      virtual ~helper3x3();

      /**
       * Analyze the matrix in the parameters and compute its inverse.
       * If the matrix is not invertible, then return false.
       */
      virtual bool analyzeMatrix();

      /**
       * Transform an image according to the matrix provided in the parameters
       */
      virtual bool apply(const matrix<value_type>& src,
                               matrix<value_type>& dest,
                               fpoint& offset) const;

      /**
       * After the matrix has been analyzed, this method can be used to
       * compute the dimensions of the resulting image and the offset.
       */
      virtual bool evalDims(const ipoint& orig,
                                  ipoint& res,
                                  fpoint& offset) const;

      /**
       * Transform the given point coordinates.
       *
       * \warning Do not forget to provide first the size of the matrix
       *          from which the points are going to be taken, using 
       *          use(const ipoint&).
       *          
       * This method takes the orig position, assumed to be in a reference
       * system of a matrix/image of the size given by the use(const ipoint&)
       * method, and transforms this position according to the set parameters.
       */
      virtual void forwards(const fpoint& orig,
                            fpoint& dest,
                            const fpoint& offset) const;


      /**
       * Inverse transform the given point coordinates.
       *
       * \warning Do not forget to provide first the size of the matrix
       *          from which the points are going to be taken, using 
       *          use(const ipoint&).
       *          
       * This method is the inverse of forwards(const fpoint&,fpoint&).  Given
       * a position \c dest in the transformed system, it computes the
       * coordinates of that point in the original coordinate system.
       *
       * @param dest Position of a point in the transformed space.
       * @param orig Position of the point in the original space.
       * @param offset Position of the origin of the result with respect to the
       *               coordinate system of the original image.
       */
      virtual void backwards(const fpoint& dest,
                             fpoint& orig,
                             const fpoint& offset) const;
    private:
      /**
       * @name Matrix entries
       */
      //@{
      float m00_,m01_,m02_;
      float m10_,m11_,m12_;
      float m20_,m21_,m22_;
      //@}
    };


    /**
     * Helper for 4x4 matrix transformations
     */
    class helper4x4 : public helperBase {
    public:
      /**
       * The only available constructor.
       */
      helper4x4(interpolator_type& interp,const parameters& par);

      /**
       * Virtual destructor
       */
      virtual ~helper4x4();

      /**
       * Analyze the matrix in the parameters and compute its inverse.
       * If the matrix is not invertible, then return false.
       */
      virtual bool analyzeMatrix();

      /**
       * Transform an image according to the matrix provided in the parameters
       */
      virtual bool apply(const matrix<value_type>& src,
                               matrix<value_type>& dest,
                               fpoint& offset) const;

      /**
       * Transform an image according to the matrix provided in the parameters.
       *
       * The offset provides information of the coordinate values of
       * the resulting image with respect to the coordinate system of
       * the original image.  Of course, this will be (0,0) if
       * KeepDimensions or KeepOrigin are chosen in the parameter.
       *
       * This method provides the computation of the z component, which is
       * usually discarded.  This method makes sense only for 4x4 matrices.
       * For all other matrices types, the default implementation calls
       * the usual apply method and sets the z component to zero.
       *
       * If you need to find out where in the rotated image is located the
       * origin of the initial image, then that would be in -offset.
       *
       * @param src    original image
       * @param dest   transformed image
       * @param z      z component
       * @param offset coordinate in the original reference system of the 
       *               new image pixel (0,0).
       */
      virtual bool apply(const matrix<value_type>& src,
                               matrix<value_type>& dest,
                               matrix<float>& z,
                               fpoint& offset) const;

      /**
       * After the matrix has been analyzed, this method can be used to
       * compute the dimensions of the resulting image and the offset.
       */
      virtual bool evalDims(const ipoint& orig,
                                  ipoint& res,
                                  fpoint& offset) const;

      /**
       * Get inverse matrix as matrix
       */
      bool getMatrix(fmatrix& m) const;

      /**
       * Get the transformation of the given point (x,y) extending it to
       * homogeneous coordinates (x,y,0,1), and normalizing the result
       */
      bool trans(const fmatrix& m,const ipoint& p,fpoint& res) const;
      
      /**
       * Transform the given point coordinates.
       *
       * \warning Do not forget to provide first the size of the matrix
       *          from which the points are going to be taken, using 
       *          use(const ipoint&).
       *          
       * This method takes the orig position, assumed to be in a reference
       * system of a matrix/image of the size given by the use(const ipoint&)
       * method, and transforms this position according to the set parameters.
       */
      virtual void forwards(const fpoint& orig,
                            fpoint& dest,
                            const fpoint& offset) const;


      /**
       * Inverse transform the given point coordinates.
       *
       * \warning Do not forget to provide first the size of the matrix
       *          from which the points are going to be taken, using 
       *          use(const ipoint&).
       *          
       * This method is the inverse of forwards(const fpoint&,fpoint&).  Given
       * a position \c dest in the transformed system, it computes the
       * coordinates of that point in the original coordinate system.
       *
       * @param dest Position of a point in the transformed space.
       * @param orig Position of the point in the original space.
       * @param offset Position of the origin of the result with respect to the
       *               coordinate system of the original image.
       */
      virtual void backwards(const fpoint& dest,
                             fpoint& orig,
                             const fpoint& offset) const;
    protected:
      /**
       * @name Matrix entries of the inverse
       */
      //@{
      float m00_,m01_,m02_,m03_;
      float m10_,m11_,m12_,m13_;
      float m20_,m21_,m22_,m23_;
      float m30_,m31_,m32_,m33_;
      //@}

      /**
       * @name Determinant of the matrix, used to check singularity
       */
      float det_;

      /**
       * Compute the inverse.
       *
       * Return false if matrix is singular.
       */
      bool inverse(const double& m00,
                   const double& m01,
                   const double& m02,
                   const double& m03,
                   const double& m10,
                   const double& m11,
                   const double& m12,
                   const double& m13,
                   const double& m20,
                   const double& m21,
                   const double& m22,
                   const double& m23,
                   const double& m30,
                   const double& m31,
                   const double& m32,
                   const double& m33);

    };

    /**
     * Helper for 4x3 matrix transformations
     */
    class helper4x3 : public helper4x4 {
    public:
      /**
       * The only available constructor.
       */
      helper4x3(interpolator_type& interp,const parameters& par);

      /**
       * Virtual destructor
       */
      virtual ~helper4x3();

      /**
       * Analyze the matrix in the parameters and compute its inverse.
       * If the matrix is not invertible, then return false.
       */
      virtual bool analyzeMatrix();

      /**
       * After the matrix has been analyzed, this method can be used to
       * compute the dimensions of the resulting image and the offset.
       */
      virtual bool evalDims(const ipoint& orig,
                                  ipoint& res,
                                  fpoint& offset) const;

      /**
       * Get the transformation of the given point (x,y) extending it to
       * homogeneous coordinates (x,y,0,1), and normalizing the result
       */
      bool trans(const fmatrix& m,const ipoint& p,fpoint& res) const;
    };

    /**
     * Pointer to the actual helper being used.
     *
     * This pointer is managed by the updateParameters() method.
     */
    helperBase* helper_;

    /**
     * Last offset computed in use();
     */
    fpoint offset_;

    /**
     * Last size indicated with use()
     */
    ipoint usedSize_;

  }; // class matrixTransform<I<T> >

} // namespace lti

#include "ltiMatrixTransform_template.h"

#endif
