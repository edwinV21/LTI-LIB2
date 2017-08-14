/*
 * Copyright (C) 1998-2005
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
 * \file   ltiGaussianPyramid.h
 *         Contains the class template class lti::gaussianPyramid<T>.
 * \author Pablo Alvarado
 * \date   30.10.2000
 *
 * revisions ..: $Id: ltiGaussianPyramid.h,v 1.1 2005-02-15 14:31:22 arndh Exp $
 */

#ifndef _LTI_GAUSSIAN_PYRAMID_H_
#define _LTI_GAUSSIAN_PYRAMID_H_

#include "ltiPyramid.h"
#include "ltiTypes.h"
#include "ltiContainer.h"

namespace lti {
  /**
   * GaussianPyramid class.
   *
   * This class implements the gaussian pyramids as described in
   * Greenspan et.at. "Overcomplete Steerable Pyramid Filters and
   * Rotation Invariance", Proc. of the IEEE Conf. on Computer Visiona
   * and Pattern Recognition, Seattle, 1994
   *
   * It allows an easy manipulation of the different resolutions.
   *
   * See method generate() to an detailed explanation.
   *
   * The template type T is the type of the elements in the pyramid.
   *
   * Example:
   *
   * \code
   *
   * channel chnl;
   * // initialize channel or load it from an image...
   *
   * lti::gaussianPyramid<lti::channel> thePyramid(4); // four levels
   * thePyramid.generate(chnl); // generate a pyramid for the given channel
   *
   * \endcode
   *
   * @ingroup gLocalSampling
   */
  template <class T>
  class gaussianPyramid : public pyramid<T> {
  public:

    /**
     * create a gaussian pyramid with the given number of resolutions
     * @param resolutions the number of resolutions that the pyramid can hold
     *                    (default 0: an empty pyramid will be created)
     * @param upsampleWithGaussian a gaussian kernel will be used to
     *                             upsample the images.  If false a squared
     *                             kernel will be used.
     *
     * @param gaussianSize the size of the gaussian kernel (default 3)
     * @param variance the variance of the gaussian kernel (default -1, meaning
     *                 that the variance should be calculated as described in
     *                 the lti::gaussKernel1D<T>)
     */
    gaussianPyramid(const int resolutions = 0,
                    const int gaussianSize = 3,
                    const double& variance = -1,
                    const bool upsampleWithGaussian = true);

    /**
     * create this gaussianPyramid as a copy of another gaussianPyramid
     * @param other the gaussianPyramid to be copied.
     */
    gaussianPyramid(const gaussianPyramid& other);

    /**
     * destructor
     */
    virtual ~gaussianPyramid();

    /**
     * return the size and variance of the used gaussian kernel
     */
    void getKernelParameters(int& size,
                             double& variance,
                             bool& gaussian) const;

    /**
     * set the kernel parameters
     *
     * @param size the size of the kernel
     * @param variance the variance for the gaussian kernel.  A negative
     *                 value will force the default variance of a gaussian
     *                 kernel with size <code>size</code>.
     * @param gaussian specify if for the channel upsampling a gaussian or
     *                 a rectangular kernel should be used.
     */
    void setKernelParameters(const int size,
                             const double& variance=-1,
                             const bool gaussian = true);


    /**
     * Returns the complete name of the functor class
     */
    virtual const std::string& name() const;

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual gaussianPyramid* clone() const;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual gaussianPyramid* newInstance() const;

    /**
     * assigment operator.
     * copy the contents of <code>other</code> in this %object.
     * @param other the source gaussianPyramid to be copied.
     * @return a reference to this object
     */
    gaussianPyramid<T>& copy(const gaussianPyramid<T>& other);

 
    /**
     * generate the gaussian pyramid of the given object.
     *
     * The pyramid will contain the number of resolutions specified in
     * the construction or in the resize() method.  The resolution "0"
     * will correspond the the original channel, and the resolution
     * i+1 is always a factor 2 smaller than the resolution i.
     */
    void generate(const T& src);

    /**
     * generate the gaussian pyramid of the given object.
     *
     * The pyramid will contain the number of resolutions specified
     * by theResolutions.
     * The resolution "0" will correspond the the original channel,
     * and the resolution i+1 is always a factor 2 smaller than
     * the resolution i.
     */
    void generate(const T& src,const int theResolutions);

    /**
     * generate the gaussian pyramid of the given object. Proceed, until
     * given limit.x or limit.y is reached. smallest resolution will be > limit.
     *
     * The resolution "0" corresponds to the original channel, and the resolution
     * i+1 is always a factor 2 smaller than the resolution i.
     */
    void generate(const T& src, const lti::ipoint& limit);

    /**
     * reconstructs the resolution with index i from a second resolution j.
     * If i>j, the object at(i) will be returned, otherwise the corresponding
     * upsampling will be done with the kernel type specified in the
     * construction
     *
     * @param i the resolution to be reconstructed
     * @param fromJ the resolution from which the data is to be taken.
     * @param result the resulting object
     * @return a reference to the result object.
     */
    T& reconstruct(const int i, const int fromJ, T& result) const;

    /**
     * reconstructs the resolution with index i from a second
     * resolution j.
     * If i>j, the object the same pyramid will be
     * returned, otherwise the corresponding upsampling will be done
     * with the kernel type specified in the construction
     * A pyramid with the same original size will be returned, where only
     * the elements between i and fromJ will be initialized.
     *
     * @param i the resolution to be reconstructed
     * @param fromJ the resolution from which the data is to be taken.
     * @param result the resulting pyramid.
     * @return a reference to the result pyramid
     */
    pyramid<T>& reconstruct(const int i, const int fromJ,
                            pyramid<T>& result) const;

  protected:

    /**
     * kernel size
     */
    int kernelSize_;

    /**
     * kernel variance
     */
    double kernelVariance_;

    /**
     * specify if the upsampling kernel is gaussian (true) or
     * rectangular(false)
     */
    bool gaussian_;
  };

} // namespace lti

#include "ltiGaussianPyramid_template.h"

#endif

