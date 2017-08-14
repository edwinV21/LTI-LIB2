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
 * \file   ltiGaussianPyramid_template.h
 *         Contains the class template class lti::gaussianPyramid<T>.
 * \author Pablo Alvarado
 * \date   29.06.2000
 *
 * revisions ..: $Id: ltiGaussianPyramid_template.h,v 1.1 2005-02-15 14:31:22 arndh Exp $
 */

#include "ltiGaussianPyramid.h"
#include "ltiDownsampling.h"
#include "ltiGaussKernels.h"
#include "ltiUpsampling.h"
#include "ltiFilledUpsampling.h"
#include "ltiMath.h"
#include "ltiKernel1D.h"
#include "ltiKernel2D.h"
#include "ltiSeparableKernel.h"

namespace lti {

  /*
   * Small private helper class required at compile time to determine if a
   * given type is a structure or not.
   */
  template<class T>
  class getSubtype {
  public:
    typedef T subtype;
  };

  // template specializations
  
  template<>
  class getSubtype<rgbaPixel> {
  public:
    typedef rgbaPixel::value_type subtype;
  };

  /*
   * default constructor creates an empty gaussianPyramid;
   */
  template <class T>
  gaussianPyramid<T>::gaussianPyramid(const int resolutions,
                                      const int gaussianSize,
                                      const double& variance,
                                      const bool upsampleWithGaussian)
    : pyramid<T>(resolutions),kernelSize_(gaussianSize),
      kernelVariance_(variance),gaussian_(upsampleWithGaussian) {
  }

  /*
   * create this gaussianPyramid as a copy of another gaussianPyramid
   * @param other the gaussianPyramid to be copied.
   */
  template <class T>
  gaussianPyramid<T>::gaussianPyramid(const gaussianPyramid<T>& other) 
    : pyramid<T>() {
    copy(other);
  }

  

  /*
   * destructor
   */
  template <class T>
  gaussianPyramid<T>::~gaussianPyramid() {
  }

  /**
   * return the size and variance of the used gaussian kernel
   */
  template <class T>
  void gaussianPyramid<T>::getKernelParameters(int& size,
                                               double& variance,
                                               bool& useGaussian) const {
    size = kernelSize_;
    variance = kernelVariance_;
    useGaussian = gaussian_;
  }

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
  template <class T>
  void gaussianPyramid<T>::setKernelParameters(const int size,
                                               const double& variance,
                                               const bool useGaussian) {
    kernelSize_ = size;
    kernelVariance_ = variance;
    gaussian_ = useGaussian;
  }

  /*
   * assigment operator.
   * copy the contents of <code>other</code> in this %object.
   * @param other the source gaussianPyramid to be copied.
   * @return a reference to this object
   */
  template <class T>
  gaussianPyramid<T>&
  gaussianPyramid<T>::copy(const gaussianPyramid<T>& other) {

    pyramid<T>::copy(other);
    gaussian_ = other.gaussian_;
    kernelVariance_ = other.kernelVariance_;
    kernelSize_ = other.kernelSize_;

    return *this;
  }


    // class name
  template <class T>
  const std::string& gaussianPyramid<T>::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone member
  template <class T>
  gaussianPyramid<T>* gaussianPyramid<T>::clone() const {
    return new gaussianPyramid<T>(*this);
  }

  // create a new instance
  template <class T>
  gaussianPyramid<T>* gaussianPyramid<T>::newInstance() const {
    return new gaussianPyramid<T>();
  }


  /**
   * generate pyramid
   */
  template <class T>
  void gaussianPyramid<T>::generate(const T& src) {

    if (this->size()<=0) {
      return;
    }

    this->at(0).copy(src);

    downsampling downsampler;
    downsampling::parameters dParam;

    typedef typename getSubtype<typename T::value_type>::subtype value_type;

    gaussKernel2D<value_type> kern(kernelSize_,kernelVariance_);
    dParam.setKernel(kern);
    dParam.boundaryType = lti::Constant;
    dParam.factor = ipoint(2,2);

    downsampler.setParameters(dParam);

    int i;
    for (i=1;i<this->size();++i) {
      downsampler.apply(this->at(i-1),this->at(i));
    }
  }

  /**
   * generate pyramid
   */
  template <class T>
  void gaussianPyramid<T>::generate(const T& src, const int theResolutions) {

    this->resize(theResolutions,false);
    generate(src);
  }

  /**
   * generate pyramid
   */
  template <class T>
    void gaussianPyramid<T>::generate(const T& src,
                                      const lti::ipoint& limit) {

    if ( (limit.x<1)||(limit.y<1) ) {
      return;
    }

    int steps = 1;
    lti::ipoint thePoint = src.size();

    while (    (thePoint.x > limit.x)
            && (thePoint.y > limit.y) ) {
      thePoint.x /= 2;
      thePoint.y /= 2;
      steps++;
    }
    generate(src,steps);
  }

  /**
   * reconstruct an object
   */
  template <class T>
    T& gaussianPyramid<T>::reconstruct(const int i, const int fromJ,
                                       T& result) const {

    assert(i<this->size());
    assert(fromJ<this->size());

    typedef typename T::value_type value_type;

    if (i>=fromJ) {
      result.copy(this->at(i));
    } else {

      if (gaussian_) {
        upsampling upsampler;
        upsampling::parameters uParam;

        kernel1D<float> kern1(-1,1,0.5f);
        kern1.at(0)=1.0f;

        separableKernel<float> kern(kern1);

        uParam.setKernel(kern);
        uParam.boundaryType = lti::Constant;
        uParam.factor = ipoint(2,2);
        upsampler.setParameters(uParam);

        int x;
        upsampler.apply(this->at(fromJ),result);

        for (x=i+1;x<fromJ;x++) {
          upsampler.apply(result);
        }

        if (this->at(i).size() != result.size()) {
          result.resize(this->at(i).size(),value_type(),true,false);
        }
      } else {
        const int factor = (1<<(fromJ-i)); // int(0.5+pow(2,fromJ-i));

        filledUpsampling upsampler;
        filledUpsampling::parameters uParam;
        uParam.factor = ipoint(factor,factor);

        upsampler.setParameters(uParam);
        upsampler.apply(this->at(fromJ),result);

        if (this->at(i).size() != result.size()) {
          result.resize(this->at(i).size(),value_type(),true,false);
        }
      }
    }

    return result;
  }

  /**
   * reconstruct an object
   */
  template <class T>
    pyramid<T>& gaussianPyramid<T>::reconstruct(const int i, const int fromJ,
                                                pyramid<T>& result) const {

    assert(i<this->size());
    assert(fromJ<this->size());

    typedef typename T::value_type value_type;

    result.resize(this->size(),false); // discard old data!

    if (i>=fromJ) {
      int j;
      for (j=fromJ;j<=i;j++) {
        result.at(j).copy(this->at(j));
      }
    } else {

      if (gaussian) {
        upsampling upsampler;
        upsampling::parameters uParam;

        kernel1D<float> kern1(-1,1,0.5f);
        kern1.at(0)=1.0f;

        separableKernel<float> kern(kern1);

        uParam.setKernel(kern);
        uParam.boundaryType = lti::Constant;
        uParam.factor = ipoint(2,2);
        upsampler.setParameters(uParam);

        result.at(fromJ).copy(this->at(fromJ));

        int x;

        for (x=fromJ;x>i;x--) {
          upsampler.apply(result.at(x),result.at(x-1));

          // fix size after upsampling
          if (this->at(x-1).size() != result.at(x-1).size()) {
            result.at(x-1).resize(this->at(x-1).size(),value_type(),
                                  true,false);
          }

        }

      } else {
        int factor = 2;

        filledUpsampling upsampler;
        filledUpsampling::parameters uParam;
        uParam.factor = ipoint(factor,factor);

        upsampler.setParameters(uParam);

        result.at(fromJ).copy(this->at(fromJ));

        int x;
        for (x=fromJ;x>i;x--) {
          upsampler.apply(result.at(x),result.at(x-1));

          // fix size after upsampling
          if (this->at(x-1).size() != result.at(x-1).size()) {
            result.at(x-1).resize(this->at(x-1).size(),value_type(),
                                  true,false);
          }
        }
      }
    }

    return result;
  }

}

