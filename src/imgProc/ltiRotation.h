/*
 * Copyright (C) 2003 Vlad Popovici, EPFL STI-ITS, Switzerland
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
 * \file   ltiRotation.h
 *         This file contains the functor rotation with rotates matrices
 * \author Vlad Popovici
 * \author Pablo Alvarado
 * \date   18.06.2003
 *
 * $Id: ltiRotation.h,v 1.3 2012-08-12 02:01:08 alvarado Exp $
 */

#ifndef _LTI_ROTATION_H_
#define _LTI_ROTATION_H_

// For a description and a more pedagogical implementation of the
// rotation algorithm, see http://www.leptonica.com/index.html

#include "ltiModifier.h"

namespace lti {

  /**
   * Rotation implements a rotation functor.
   *
   * The algorithm used is call Rotation by Shear, nicely
   * reviewed at http://www.leptonica.com/rotation.html#ROTATION-BY-SHEAR
   *
   * The computed image will always be enlarged to contain
   * the whole (rotated) image.  In other words, you will get the same
   * result as using lti::matrixTransform using the parameter
   * \c keepDimensions set to false. 
   *
   * This functor always uses a bilinear interpolation, but supports
   * only the boundary type "Zero", to avoid some comparisons and
   * increase efficiency.
   *
   * \warning The speed gains of this functor compared with
   * lti::matrixTransform are marginal or non-existent, i.e. this functor
   * algorithm can be even slower.  The functor exists to provide an
   * interface, that can wrap a more efficient algorithm in the future.
   *
   * For more complex operations you can still use the lti::matrixTransform 
   * functor.
   *
   * The member functions are explicitly instantiated for image,
   * matrix<ubyte> (which is similar to channel8), and matrix<float>
   * (which is similar to channel). Other uses might be possible
   * depending on the type T. You have to include
   * ltiRotation_template.h for such purposes.
   *
   * @see lti::matrixTransform
   *
   * @ingroup gGeometry
   */
  class rotation : public modifier {
  public:
    /**
     * the parameters for the class rotation
     */
    class parameters : public modifier::parameters {
    public:
      /**
       * default constructor
       */
      parameters();

      /**
       * copy constructor
       * @param other the parameters object to be copied
       */
      parameters(const parameters& other);

      /**
       * destructor
       */
      ~parameters();

      /**
       * copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& copy(const parameters& other);

      /**
       * copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& operator=(const parameters& other);

      /**
       * Returns the name of this type.
       */
      virtual const std::string& name() const;

      /**
       * returns a pointer to a clone of the parameters
       */
      virtual parameters* clone() const;

      /**
       * returns a pointer to a newInstance of the parameters
       */
      virtual parameters* newInstance() const;

      /**
       * write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool write(ioHandler& handler,const bool complete=true) const;

      /**
       * read the parameters from the given ioHandler
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
       * Rotation angle.  
       *
       * In radians.  You can use the global function lti::degToRad() to
       * convert a degrees value to radians.
       *
       * As usual in the LTI-Lib, the coordinate system of image is
       * left-handed, (y=0 means the top of the image, y>0 goes down).  An
       * increasing positive angle denotes in such a coordinate system a
       * clockwise rotation.
       *
       * Default value: 0
       */
      double angle;

    };

    /**
     * default constructor
     */
    rotation();

    /**
     * Construct a functor using the given parameters
     */
    rotation(const parameters& par);

    /**
     * copy constructor
     * @param other the object to be copied
     */
    rotation(const rotation& other);

    /**
     * destructor
     */
    virtual ~rotation();
    
    /**
     * Rotate \a srcdest by parameters::angle and leave the result in
     * \a srcdest as well
     * @param srcdest matrix with the source data.  The result
     *                will be left here too.
     * @return true if apply successful or false otherwise.
     */
    template <typename T>
    bool apply(matrix<T>& srcdest) const;

    /**
     * Rotate \a src by parameters::angle and leave the result in
     * \a dest
     * @param src matrix with the source data.
     * @param dest matrix where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    template <typename T>
    bool apply(const matrix<T>& src, matrix<T>& dest) const;


    /**
     * Operates on the given %parameter.
     * @param srcdest image with the source data.  The result
     *                 will be left here too.
     * @param angle rotation angle used instead of the value in the parameters
     *              object, which is ignored.
     * @return true if apply successful or false otherwise.
     */
    template <typename T>
    bool rotate(matrix<T>& srcdest, const double& angle) const;

    /**
     * Operates on a copy of the given %parameters.
     * @param src image with the source data.
     * @param dest image where the result will be left.
     * @param angle rotation angle used instead of the value in the parameters
     *              object, which is ignored.
     * @return true if apply successful or false otherwise.
     */
    template <typename T>
    bool rotate(const matrix<T>& src, matrix<T>& dest,
                const double& angle) const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    rotation& copy(const rotation& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    rotation& operator=(const rotation& other);

    /**
     * Returns the name of this type.
     */
    virtual const std::string& name() const;
    
    /**
     * returns a pointer to a clone of this functor.
     */
    virtual rotation* clone() const;

    /**
     * returns a pointer to a newInstance of this functor.
     */
    virtual rotation* newInstance() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

  protected:

    /**
     * Skews horizontally the matrix<T> src and save the result in dst.
     * The argument weight is an interpolation factor between 0 and 1
     */ 
    template <typename T>
    bool horizontalSkew(const matrix<T>& src, 
		        matrix<T>& dst, 
			const int rowIdx,
			const int colOffset,
			const double weight) const;

    /**
     * Skews vertically the matrix<T> src and save the result in dst.
     * The argument weight is an interpolation factor between 0 and 1
     */
    template <typename T>
    bool verticalSkew(const matrix<T>& src,
		      matrix<T>& dst,
		      const int colIdx,
		      const int rowOffset,
		      const double weight) const;

    /**
     * rotate the matrix 90 degrees
     */
    template <typename T>
    bool rotate90(const matrix<T>& src, matrix<T>& dst) const;

    /**
     * rotate the matrix 180 degrees
     */
    template <typename T>
    bool rotate180(const matrix<T>& src, matrix<T>& dst) const;

    /**
     * rotate the matrix 270 degrees
     */
    template <typename T>
    bool rotate270(const matrix<T>& src, matrix<T>& dst) const;

    /**
     * rotate the matrix using 3 shear technique. 
     */
    template <typename T>
    bool rotateShear(const matrix<T>& src, matrix<T>& dest,
                     const double& angle) const;
  };
}

#endif


