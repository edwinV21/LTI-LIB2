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
 * \file   ltiMatrixTransform.cpp
 *         Contains the implementation of class matrixTransform,
 * 
 * \author Pablo Alvarado
 * \date   08.09.2007
 *
 * revisions ..: $Id: ltiMatrixTransform_template.h,v 1.16 2013-12-03 14:48:23 alvarado Exp $
 */

#include <limits>
#include "ltiRound.h"

namespace lti {


  /**
   * Global scope function to create a rotation homogeneous matrix for
   * 3D spaces, which is always a 4x4 matrix.
   *
   * @param center Point in the space that is kept constant
   * @param axis Rotation axis.
   * @param angle Magnitude of the angle of the rotation
   * @return The 4x4 homogeneous matrix representing a 3D rotation.
   */
  template<typename T>
  matrix<T> rotationMatrix(const point3D<T>& center,
                           const point3D<T>& axis,
                           const T& angle) {
    matrix<T> m;
    m.allocate(4,4);
   
    T tx,ty,tz;

    tx = center.x;
    ty = center.y;
    tz = center.z;

    // the wished rotation angle
    
    T ctmp, stmp;
    lti::sincos(angle, stmp, ctmp);

    const T sina = static_cast<T>(stmp);
    const T cosa = static_cast<T>(ctmp);

    const T sqrXZ = axis.x*axis.x + axis.z*axis.z; 
    const T sqrXYZ = sqrXZ + axis.y*axis.y;
    const T absXZ = sqrt(sqrXZ);

    // psi (or here just p) is the rotation angle on y
    T sinp,cosp;
    if (absXZ == 0) {
      sinp = T(0);
      cosp = T(1);
    }
    else {
      sinp = axis.x/absXZ;
      cosp = axis.z/absXZ;
    }

    // theta is the rotation angle on x
    const T sint = axis.y/sqrt(sqrXYZ);
    const T cost = sqrt(sqrXZ/sqrXYZ);

    // the following code was partially automatic generated, and can
    // still be optimized:

    // temporary data
    const T cosp2 = cosp*cosp;
    const T cost2 = cost*cost;
    const T sint2 = static_cast<T>(1) - cost2;
    const T t7 = cosp2*cosa*cost2;
    const T t8 = cosa*cost2;
    const T t9 = cosp2*cost2;
    const T t11 = cost*cosp;
    const T t13 = t11*sina;
    const T t15 = cost*sinp;
    const T t17 = sint*cosa;
    const T t18 = t15*t17;
    const T t19 = t15*sint;
    const T t21 = cosp*sinp;
    const T t22 = t21*t8;
    const T t23 = sint*sina;
    const T t24 = t21*cost2;
    const T t26 = tx*cosp2;
    const T t29 = cosp*cost2;
    const T t31 = cost*ty;
    const T t32 = sinp*sint;
    const T t34 = t32*cosa;
    const T t38 = sinp*cosa*cost2;
    const T t40 = tx*cosa;
    const T t41 = cosp*sina;
    const T t48 = t26*(cost2-t8) + t31*(t32 - t34 + t41) + 
      tz*(sinp*t29 - cosp*t38 - sint*sina) +
      (tx - t40)*sint2;
    const T t51 = t15*sina;
    const T t52 = t11*t17;
    const T t53 = t11*sint;
    const T t55 = cost*tx;
    const T t61 = cost*tz;
    const T t62 = sinp*sina;
    const T t64 = sint*cosp;
    const T t65 = t64*cosa;
    const T t80 = cosp2*tz;
    
    // Rotation matrix:
    m.at(0,0) = t7+cosa-t8+cost2-t9;
    m.at(0,1) = -t13+t18-t19;
    m.at(0,2) = t22+t23-t24;
    m.at(0,3) = t48;

    m.at(1,0) = t13+t18-t19;
    m.at(1,1) = t8+sint2;
    m.at(1,2) = t51-t52+t53;
    m.at(1,3) = (t55*(t32-t41-t34)
                 +ty*cost2*(static_cast<T>(1)-cosa)
                 +t61*(t65-t62-t64));

    m.at(2,0) = t22-t23-t24;
    m.at(2,1) = -t51-t52+t53;
    m.at(2,2) = -t7+cosa+t9;                     
    m.at(2,3) = (tx*(sinp*t29 + sint*sina - cosp*t38)
                 +t31*(t65+t62-t64)
                 +t80*(t8-cost2)
                 +tz-tz*cosa);
    
    m.at(3,0) = 0;
    m.at(3,1) = 0;
    m.at(3,2) = 0;
    m.at(3,3) = T(1);
    
    return m;
  }

  /**
   * Global scope function to create a rotation homogeneous matrix for
   * 2D spaces, which is always a 3x3 matrix.
   *
   * @param center Point in the space that is kept constant
   * @param angle Magnitude of the angle of the rotation
   * @return The 3x3 homogeneous matrix representing a 2D rotation.
   */
  template<typename T>
  matrix<T> rotationMatrix(const point<T>& center,
                           const T& angle) {

    matrix<T> m;
    m.allocate(3,3);
   
    // the wished rotation angle
    
    T cosa, sina;
    sincos(angle, sina, cosa);

    m.at(0,0) =  cosa;
    m.at(0,1) = -sina;
    m.at(0,2) = -cosa*center.x + sina*center.y + center.x;
    m.at(1,0) =  sina;
    m.at(1,1) =  cosa;
    m.at(1,2) =  -sina*center.x - cosa*center.y + center.y;
    m.at(2,0) =  0;
    m.at(2,1) =  0;
    m.at(2,2) =  static_cast<T>(1);

    return m;
  }

  /**
   * Global scope function to create a shift homogeneous matrix in a
   * 3D space, which is always 4x4.
   *
   * @param s Shift amount.
   * @return The 4x4 homogeneous matrix representing a 3D shift.
   */
  template<typename T>
  matrix<T> translationMatrix(const point3D<T>& s) {
    matrix<T> m;
    m.allocate(4,4);
    m.setIdentity();
    m.at(0,3)=s.x;
    m.at(1,3)=s.y;
    m.at(2,3)=s.z;

    return m;
  }

  /**
   * Global scope function to create a shift homogeneous matrix for
   * 2D spaces, which is always a 3x3 matrix.
   *
   * @param s Shift amount.
   * @return The 3x3 homogeneous matrix representing a 2D shift.
   */
  template<typename T>
  matrix<T> translationMatrix(const point<T>& s) {
    matrix<T> m;
    m.allocate(3,3);
    m.setIdentity();
    m.at(0,2)=s.x;
    m.at(1,2)=s.y;

    return m;    
  }

  /**
   * Global scope function to create a homogeneous matrix for scaling in
   * 3D spaces.
   *
   * @param s Scale amount in each axis.
   * @return The 4x4 homogeneous matrix representing a 3D scale
   */
  template<typename T>
  matrix<T> scalingMatrix(const point3D<T>& s) {
    matrix<T> m(4,4,T(0));
    m.at(0,0)=s.x;
    m.at(1,1)=s.y;
    m.at(2,2)=s.z;
    m.at(3,3)=static_cast<T>(1);

    return m;
  }

  /**
   * Global scope function to create a homogeneous matrix for scaling in
   * 2D spaces.
   *
   * @param s Scale amount in each axis.
   * @return The 3x3 homogeneous matrix representing a 2D scale
   */
  template<typename T>
  matrix<T> scalingMatrix(const point<T>& s) {
    matrix<T> m(3,3,T(0));
    m.at(0,0)=s.x;
    m.at(1,1)=s.y;
    m.at(2,2)=static_cast<T>(1);

    return m;
  }

  /**
   * Global scope function to create a homogeneous matrix for scaling in
   * 2D spaces.
   *
   * @param s Scale amount in each axis.
   * @return The 3x3 homogeneous matrix representing a 2D scale
   */
  template<typename T>
  matrix<T> scalingMatrix(const T& s) {
    matrix<T> m(3,3,T(0));
    m.at(0,0)=s;
    m.at(1,1)=s;
    m.at(2,2)=static_cast<T>(1);

    return m;
  }


  template<typename T>
  matrix<T> projection(const T& f) {
    matrix<T> m(4,4);
    m.setIdentity();
    m.at(3,2)=static_cast<T>(1)/f;
    return m;
  }

  // --------------------------------------------------
  // matrixTransform<T,I>::parameters
  // --------------------------------------------------

  // default constructor
  template <typename T, template<class> class I>
  matrixTransform<T,I>::parameters::parameters()
    : geometricTransform<T,I>::parameters() {
    transformation.allocate(2,2);
    transformation.setIdentity();
  }

  // copy constructor
  template <typename T, template<class> class I>
  matrixTransform<T,I>::parameters::parameters(const parameters& other)
    : geometricTransform<T,I>::parameters() {
    copy(other);
  }

  // destructor
  template <typename T, template<class> class I>
  matrixTransform<T,I>::parameters::~parameters() {
  }

  // copy member

  template <typename T, template<class> class I>
  typename matrixTransform<T,I>::parameters&
  matrixTransform<T,I>::parameters::copy(const parameters& other) {
    geometricTransform<T,I>::parameters::copy(other);
    
    transformation = other.transformation;

    return *this;
  }

  // alias for copy method
  template <typename T, template<class> class I>
  typename matrixTransform<T,I>::parameters&
  matrixTransform<T,I>::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  // class name
  template <typename T, template<class> class I>
  const std::string& matrixTransform<T,I>::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
      }

  // clone method
  template <typename T, template<class> class I>
  typename matrixTransform<T,I>::parameters*
  matrixTransform<T,I>::parameters::clone() const {
    return new parameters(*this);
  }

  // new instance
  template <typename T, template<class> class I>
  typename matrixTransform<T,I>::parameters*
  matrixTransform<T,I>::parameters::newInstance() const {
    return new parameters();
  }

  /*
   * Write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  template <typename T, template<class> class I>
  bool matrixTransform<T,I>::parameters::write(ioHandler& handler,
                                               const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {
      lti::write(handler,"transformation",transformation);
    }

    b = b && geometricTransform<T,I>::parameters::write(handler,false);

    if (complete) {
      b = b && handler.writeEnd();
    }

    return b;
  }

  /*
   * Read the parameters from the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also read, otherwise only the data block will be read.
   * @return true if read was successful
   */
  template <typename T, template<class> class I>
  bool matrixTransform<T,I>::parameters::read(ioHandler& handler,
                                              const bool complete)  {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {
      lti::read(handler,"transformation",transformation);
    }

    b = b && geometricTransform<T,I>::parameters::read(handler,false);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }


  // --------------------------------------------------
  // matrixTransform<T,I>
  // --------------------------------------------------

  // default constructor
  template <typename T, template<class> class I>
  matrixTransform<T,I>::matrixTransform() :
    geometricTransform<T,I>(),helper_(0) {

    // create an instance of the parameters with the default values
    parameters defaultParameters;
    // set the default parameters
    this->setParameters(defaultParameters);
  }

  // default constructor
  template <typename T, template<class> class I>
  matrixTransform<T,I>::matrixTransform(const parameters& par)
    : geometricTransform<T,I>(),helper_(0) {


    // set the given parameters
    this->setParameters(par);
  }


  // copy constructor
  template <typename T, template<class> class I>
  matrixTransform<T,I>::matrixTransform(const matrixTransform<T,I>& other)
    : geometricTransform<T,I>(),helper_(0) {
    copy(other);
  }

  // destructor
  template <typename T, template<class> class I>
  matrixTransform<T,I>::~matrixTransform() {
    delete helper_;
    helper_=0;
  }

  // copy member
  template <typename T, template<class> class I>
  matrixTransform<T,I>& 
  matrixTransform<T,I>::copy(const matrixTransform<T,I>& other) {
    geometricTransform<T,I>::copy(other);

    // updateParameters is in charge of helper_

    // copy this local parameters
    offset_ = other.offset_;
    usedSize_ = other.usedSize_;

    return (*this);
  }

  // alias for copy member
  template <typename T, template<class> class I>
  matrixTransform<T,I>&
  matrixTransform<T,I>::operator=(const matrixTransform<T,I>& other) {
    return (copy(other));
  }

  // class name
  template <typename T, template<class> class I>
  const std::string& matrixTransform<T,I>::name() const {
    _LTI_RETURN_CLASS_NAME
      }

  // clone member
  template <typename T, template<class> class I>
  matrixTransform<T,I>* matrixTransform<T,I>::clone() const {
    return new matrixTransform<T,I>(*this);
  }

  // create a new instance
  template <typename T, template<class> class I>
  matrixTransform<T,I>* matrixTransform<T,I>::newInstance() const {
    return new matrixTransform<T,I>();
  }

  // return parameters
  template <typename T, template<class> class I>
  const typename matrixTransform<T,I>::parameters&
  matrixTransform<T,I>::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&geometricTransform<T,I>::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }

  // return parameters
  template <typename T, template<class> class I>
  typename matrixTransform<T,I>::parameters&
  matrixTransform<T,I>::getRWParameters(){
    parameters* par =
      dynamic_cast<parameters*>(&geometricTransform<T,I>::getRWParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }


  template <typename T, template<class> class I>
  bool matrixTransform<T,I>::updateParameters() {
    if (geometricTransform<T,I>::updateParameters()) {
      delete helper_;
      helper_ = 0;
      
      const parameters& par = getParameters();
      const fmatrix& mat = par.transformation;
      
      if (mat.size() == ipoint(2,2)) {
        helper_ = new helper2x2(this->interpolator_,par);
      } else if (mat.size() == ipoint(3,2)) {
        helper_ = new helper2x3(this->interpolator_,par);
      } else if ( (mat.size() == ipoint(3,3)) ||
                  (mat.size() == ipoint(4,3)) ) {
        helper_ = new helper3x3(this->interpolator_,par);
      } else if (mat.size() == ipoint(4,4)) {
        helper_ = new helper4x4(this->interpolator_,par);
      } else if (mat.size() == ipoint(3,4)) {
        helper_ = new helper4x3(this->interpolator_,par);
      } else {
        this->setStatusString("Wrong dimensions for transformation matrix");
      }
      return (notNull(helper_) && helper_->analyzeMatrix());
    }
    return false;
  }

  template <typename T, template<class> class I>
  bool matrixTransform<T,I>::setMatrix(const fmatrix& mat) {
    parameters& par = getRWParameters();
    par.transformation.copy(mat);
    
    if (updateParameters()) {
      if (notNull(helper_)) {
        ipoint dims;
        helper_->evalDims(usedSize_,dims,offset_);
      }
      return true;
    }

    return false;
  }

  // -------------------------------------------------------------------
  // The apply() member functions
  // -------------------------------------------------------------------
  
  // On place apply for type matrix<T>!
  template <typename T, template<class> class I>
  bool matrixTransform<T,I>::apply(matrix<T>& srcdest) const {
    return geometricTransform<T,I>::apply(srcdest);
  }

  template <typename T, template<class> class I>
  bool matrixTransform<T,I>::apply(matrix<T>& srcdest,fpoint& offset) const {
    return geometricTransform<T,I>::apply(srcdest,offset);
  }

  template <typename T, template<class> class I>
  bool matrixTransform<T,I>::apply(const matrix<T>& src,
                                   matrix<T>& dest) const {
    fpoint offset(0.0f,0.0f);
    return apply(src,dest,offset);
  }

  template <typename T, template<class> class I>
  bool matrixTransform<T,I>::apply(const matrix<T>& src,
                                   matrix<T>& dest,
                                   fpoint& offset) const {

    if (notNull(helper_)) {
      return helper_->apply(src,dest,offset);
    }

    return false;
  }

  template <typename T, template<class> class I>
  bool matrixTransform<T,I>::apply(const matrix<T>& src,
                                   matrix<T>& dest,
                                   matrix<float>& z,
                                   fpoint& offset) const {

    if (notNull(helper_)) {
      return helper_->apply(src,dest,z,offset);
    }

    return false;
  }

  // -------------------------------------------------------------------
  // The helperBase class
  // -------------------------------------------------------------------
  template <typename T, template<class> class I>
  matrixTransform<T,I>::helperBase::helperBase(interpolator_type& interp,
                                               const parameters& par) 
    : interpolator_(interp), params_(par) {
  }
  
  template <typename T, template<class> class I>
  matrixTransform<T,I>::helperBase::~helperBase() {
  }

  template <typename T, template<class> class I>
  bool matrixTransform<T,I>::helperBase::apply(const matrix<value_type>& src,
                                               matrix<value_type>& dest,
                                               matrix<float>& z,
                                               fpoint& offset) const {
    bool b = apply(src,dest,offset);
    z.assign(src.size(),0.0f);
    return b;
  }

  template <typename T, template<class> class I> bool 
  matrixTransform<T,I>::helperBase::getDimsFromCorners(const fpoint& tl,
                                                       const fpoint& tr,
                                                       const fpoint& bl,
                                                       const fpoint& br,
                                                       ipoint& dim,
                                                       fpoint& offset) const {

    fpoint theMin,theMax;

    theMin.x = min(min(tr.x,tl.x),min(br.x,bl.x));
    theMax.x = max(max(tr.x,tl.x),max(br.x,bl.x));
    theMin.y = min(min(tr.y,tl.y),min(br.y,bl.y));
    theMax.y = max(max(tr.y,tl.y),max(br.y,bl.y));

    if (params_.resizeMode == geometricTransformBase::KeepOrigin) {
      dim.set(iround(theMax.x),iround(theMax.y));
      offset.set(0.0f,0.0f);
      return ((dim.x>=0) && (dim.y>=0));
    } else { // AdjustDimensions
      dim.set(iround(theMax.x-theMin.x+1.0f),
              iround(theMax.y-theMin.y+1.0f));
      offset.copy(theMin);
      return true;
    }

    return false;
  
  }

  // -------------------------------------------------------------------
  // The helper2x2 class
  // -------------------------------------------------------------------
  template <typename T, template<class> class I>
  matrixTransform<T,I>::helper2x2::helper2x2(interpolator_type& interp,
                                             const parameters& par) 
    : helperBase(interp,par) {
  }
  
  template <typename T, template<class> class I>
  matrixTransform<T,I>::helper2x2::~helper2x2() {
  }

  template <typename T, template<class> class I>
  bool matrixTransform<T,I>::helper2x2::analyzeMatrix() {
    // double check correct dimensions
    const fmatrix& mat = this->params_.transformation;
    if (mat.size() != ipoint(2,2)) {
      return false;
    }
    
    float det = mat.at(0,0)*mat.at(1,1) - mat.at(1,0)*mat.at(0,1);
    if (abs(det) < std::numeric_limits<float>::epsilon()) {
      // singular matrix
      return false;
    }

    // inverse of the matrix
    m00_ =  mat.at(1,1)/det;
    m01_ = -mat.at(0,1)/det;
    m10_ = -mat.at(1,0)/det;
    m11_ =  mat.at(0,0)/det;

    return true;
  } 

  template <typename T, template<class> class I> bool 
  matrixTransform<T,I>::helper2x2::apply(const matrix<value_type>& src,
                                               matrix<value_type>& dest,
                                               fpoint& offset) const {
    ipoint destSize;
    if (evalDims(src.size(),destSize,offset)) {
      // dimensions valid: let's transform...

      dest.allocate(destSize);
      
      const int ex=destSize.x;
      const int ey=destSize.y;
      int x,y;
      
      // in principle, we have to compute m10_*x+m11_*y,m00_*x+m01_*y
      // but due to the constant change of x and y in steps of 1, this
      // can be much more efficiently computed using sums
      float xm10,xm00,ym11(offset.y*m11_),ym01(offset.y*m01_);

      const float m10 = offset.x*m10_;
      const float m00 = offset.x*m00_;

      for (y=0;y<ey;++y) {
        xm10=m10;
        xm00=m00;
        for (x=0;x<ex;++x) {
          dest.at(y,x)=this->interpolator_.interpolate(src,
                                                       xm10+ym11,
                                                       xm00+ym01);
          xm10+=m10_;
          xm00+=m00_;
        }
        ym11+=m11_;
        ym01+=m01_;
      }

    } else {
      // wrong dimensions: just clean the image, as there is nothing to do
      dest.clear();
      return false;
    }
    return true;
  }

  /*
   * After the matrix has been analyzed, this method can be used to
   * compute the dimensions of the resulting image and the offset.
   */
  template <typename T, template<class> class I>
  bool matrixTransform<T,I>::helper2x2::evalDims(const ipoint& orig,
                                                       ipoint& res,
                                                       fpoint& offset) const {

    if (this->params_.resizeMode == geometricTransformBase::KeepDimensions) {
      res.copy(orig);
      offset.set(0,0);
      return true;
    }

    const fmatrix& mat = this->params_.transformation;
    
    fpoint tl(0,0);
    fpoint tr(mat.at(0,0)*(orig.x-1),mat.at(1,0)*(orig.x-1));
    fpoint bl(mat.at(0,1)*(orig.y-1),mat.at(1,1)*(orig.y-1));
    fpoint br(mat.at(0,0)*(orig.x-1)+mat.at(0,1)*(orig.y-1),
              mat.at(1,0)*(orig.x-1)+mat.at(1,1)*(orig.y-1));

    return this->getDimsFromCorners(tl,tr,bl,br,res,offset);
  }

  template <typename T, template<class> class I>
  void matrixTransform<T,I>::helper2x2::forwards(const fpoint& orig,
                                                 fpoint& dest,
                                                 const fpoint& offset) const {
 
    const fmatrix& mat = this->params_.transformation;
    dest.x = mat.at(0,0)*orig.x + mat.at(0,1)*orig.y - offset.x;
    dest.y = mat.at(1,0)*orig.x + mat.at(1,1)*orig.y - offset.y;
  }

  template <typename T, template<class> class I>
  void matrixTransform<T,I>::helper2x2::backwards(const fpoint& dest,
                                                  fpoint& orig,
                                                  const fpoint& offset) const {
    fpoint tmp(dest.x+offset.x,dest.y+offset.y);
    orig.x = tmp.x*m00_ + tmp.y*m01_;
    orig.y = tmp.x*m10_ + tmp.y*m11_;
  }

  // -------------------------------------------------------------------
  // The helper2x3 class
  // -------------------------------------------------------------------
  template <typename T, template<class> class I>
  matrixTransform<T,I>::helper2x3::helper2x3(interpolator_type& interp,
                                             const parameters& par) 
    : helperBase(interp,par) {
  }
  
  template <typename T, template<class> class I>
  matrixTransform<T,I>::helper2x3::~helper2x3() {
  }

  template <typename T, template<class> class I>
  bool matrixTransform<T,I>::helper2x3::analyzeMatrix() {
    // double check correct dimensions
    const fmatrix& mat = this->params_.transformation;
    if (mat.size() != ipoint(3,2)) {
      return false;
    }
    
    float det = mat.at(0,0)*mat.at(1,1) - mat.at(1,0)*mat.at(0,1);
    if (abs(det) < std::numeric_limits<float>::epsilon()) {
      // singular matrix
      return false;
    }

    // inverse of the matrix
    m00_ =  mat.at(1,1)/det;
    m01_ = -mat.at(0,1)/det;
    m02_ = (mat.at(0,1)*mat.at(1,2) - mat.at(0,2)*mat.at(1,1))/det;
    m10_ = -mat.at(1,0)/det;
    m11_ =  mat.at(0,0)/det;
    m12_ = (mat.at(0,2)*mat.at(1,0) - mat.at(0,0)*mat.at(1,2))/det;

    return true;

  } 

  template <typename T, template<class> class I> bool 
  matrixTransform<T,I>::helper2x3::apply(const matrix<value_type>& src,
                                         matrix<value_type>& dest,
                                         fpoint& offset) const {
    ipoint destSize;
    if (evalDims(src.size(),destSize,offset)) {

      // dimensions valid: let's transform...

      dest.allocate(destSize);
      
      // We can avoid an addition renaming the effect of the offset
      const float m12 = m12_ + m10_*offset.x + m11_*offset.y;
      const float m02 = m02_ + m00_*offset.x + m01_*offset.y;

      const int ex = destSize.x;
      const int ey = destSize.y;

      int x,y;
      float xm10,xm00,ym11(0.0f),ym01(0.0f);
      for (y=0;y<ey;++y) {
        xm10=m12;
        xm00=m02;
        for (x=0;x<ex;++x) {
          dest.at(y,x)=this->interpolator_.interpolate(src,
                                                       xm10+ym11,
                                                       xm00+ym01);
          xm10+=m10_;
          xm00+=m00_;
        }
        ym11+=m11_;
        ym01+=m01_;
      }

    } else {
      // wrong dimensions: just clean the image, as there is nothing to do
      dest.clear();
      return false;
    }
    return true;
  }

  template <typename T, template<class> class I>
  bool matrixTransform<T,I>::helper2x3::evalDims(const ipoint& orig,
                                                 ipoint& res,
                                                 fpoint& offset) const {
    if (this->params_.resizeMode == geometricTransformBase::KeepDimensions) {
      res.copy(orig);
      offset.set(0,0);
      return true;
    }

    const fmatrix& m = this->params_.transformation;
    
    fpoint tl(m.at(0,2),m.at(1,2));
    fpoint tr(m.at(0,0)*(orig.x-1)+m.at(0,2),m.at(1,0)*(orig.x-1)+m.at(1,2));
    fpoint bl(m.at(0,1)*(orig.y-1)+m.at(0,2),m.at(1,1)*(orig.y-1)+m.at(1,2));
    fpoint br(m.at(0,0)*(orig.x-1)+m.at(0,1)*(orig.y-1)+m.at(0,2),
              m.at(1,0)*(orig.x-1)+m.at(1,1)*(orig.y-1)+m.at(1,2));

    return this->getDimsFromCorners(tl,tr,bl,br,res,offset);
  }

  template <typename T, template<class> class I>
  void matrixTransform<T,I>::helper2x3::forwards(const fpoint& orig,
                                                 fpoint& dest,
                                                 const fpoint& offset) const {
 
    const fmatrix& mat = this->params_.transformation;
    dest.x = mat.at(0,0)*orig.x + mat.at(0,1)*orig.y + mat.at(0,2) - offset.x;
    dest.y = mat.at(1,0)*orig.x + mat.at(1,1)*orig.y + mat.at(1,2) - offset.y;
  }

  template <typename T, template<class> class I>
  void matrixTransform<T,I>::helper2x3::backwards(const fpoint& dest,
                                                  fpoint& orig,
                                                  const fpoint& offset) const {
    fpoint tmp(dest.x+offset.x,dest.y+offset.y);
    orig.x = tmp.x*m00_ + tmp.y*m01_ + m02_;
    orig.y = tmp.x*m10_ + tmp.y*m11_ + m12_;
  }

  // -------------------------------------------------------------------
  // The helper3x3 class
  // -------------------------------------------------------------------
  template <typename T, template<class> class I>
  matrixTransform<T,I>::helper3x3::helper3x3(interpolator_type& interp,
                                             const parameters& par) 
    : helperBase(interp,par) {
  }
  
  template <typename T, template<class> class I>
  matrixTransform<T,I>::helper3x3::~helper3x3() {
  }

  template <typename T, template<class> class I>
  bool matrixTransform<T,I>::helper3x3::analyzeMatrix() {
    // double check correct dimensions
    const fmatrix& m = this->params_.transformation;

    // compute an index to pick a correct 3x3 matrix (even for a 3x4 one)
    // i is used for rows, j is used for columns
    int i(-1),j(-1);
    if (m.size() == ipoint(3,3)) {
      i=2;
      j=2;
    } else if (m.size() == ipoint(4,3)) {
      i=2;
      j=3;
    } else if (m.size() == ipoint(3,4)) {
      i=3;
      j=2;
    } else if (m.size() == ipoint(4,4)) {
      i=3;
      j=3;
    } else {
      return false;
    }

    // just to avoid confusion
    const float m00 = m.at(0,0);
    const float m01 = m.at(0,1);
    const float m02 = m.at(0,j);

    const float m10 = m.at(1,0);
    const float m11 = m.at(1,1);
    const float m12 = m.at(1,j);

    const float m20 = m.at(i,0);
    const float m21 = m.at(i,1);
    const float m22 = m.at(i,j);

    // take the inverse multiplicative of the determinant to save some ms
    const float det = (m00 * ( m11 * m22 - m12 * m21 ) + 
                       m10 * ( m02 * m21 - m01 * m22 ) + 
                       m20 * ( m01 * m12 - m02 * m11 ) );

    if (abs(det) < std::numeric_limits<float>::epsilon()) {
      // singular matrix
      return false;
    }

    const float idet = 1.0f/det;

    // inverse of the matrix
    m00_ =  (m11*m22 - m12*m21)*idet;
    m01_ = -(m01*m22 - m02*m21)*idet;
    m02_ =  (m01*m12 - m02*m11)*idet;
    m10_ = -(m10*m22 - m12*m20)*idet;
    m11_ =  (m00*m22 - m02*m20)*idet;
    m12_ = -(m00*m12 - m02*m10)*idet;
    m20_ =  (m10*m21 - m11*m20)*idet;
    m21_ = -(m00*m21 - m01*m20)*idet;
    m22_ =  (m00*m11 - m01*m10)*idet;

    return true;
  } 

  template <typename T, template<class> class I> bool 
  matrixTransform<T,I>::helper3x3::apply(const matrix<value_type>& src,
                                         matrix<value_type>& dest,
                                         fpoint& offset) const {

    ipoint destSize;
    if (evalDims(src.size(),destSize,offset)) {

      // dimensions valid: let's transform...

      dest.allocate(destSize);

      // We can avoid an addition renaming the effect of the offset
      const float m22 = m22_ + m20_*offset.x + m21_*offset.y;
      const float m12 = m12_ + m10_*offset.x + m11_*offset.y;
      const float m02 = m02_ + m00_*offset.x + m01_*offset.y;

      const int ex = destSize.x;
      const int ey = destSize.y;

      // in principle, we have to compute m10_*x+m11_*y+m12,m00_*x+m01_*y+m02
      // but due to the constant change of x and y in steps of 1, this
      // can be much more efficiently computed using sums
      float xm10,xm00,ym11(0.0f),ym01(0.0f);
      float xm20,ym21(0.0f);
      int x,y;
      for (y=0;y<ey;++y) {
        xm00=m02;
        xm10=m12;
        xm20=m22;
        for (x=0;x<ex;++x) {
          const float alpha =  xm20 + ym21;
          if (alpha == 0.0f) {
            dest.at(y,x)=static_cast<value_type>(0);
          } else {
            const float ai=1.0f/alpha;
            dest.at(y,x) =
              this->interpolator_.interpolate(src,
                                              (xm10+ym11)*ai,
                                              (xm00+ym01)*ai);
          }
          xm20+=m20_;
          xm10+=m10_;
          xm00+=m00_;
        }
        ym21+=m21_;
        ym11+=m11_;
        ym01+=m01_;
      }
    } else {
      // wrong dimensions: just clean the image, as there is nothing to do
      dest.clear();
      return false;
    }
    return true;

  }

  template <typename T, template<class> class I>
  bool matrixTransform<T,I>::helper3x3::evalDims(const ipoint& orig,
                                                 ipoint& res,
                                                 fpoint& offset) const {

    if (this->params_.resizeMode == geometricTransformBase::KeepDimensions) {
      res.copy(orig);
      offset.set(0,0);
      return true;
    }

    const fmatrix& m = this->params_.transformation;
    // compute an index to pick a correct 3x3 matrix (even for a 3x4 one)
    // i is used for rows, j is used for columns
    int i(-1),j(-1);
    if (m.size() == ipoint(3,3)) {
      i=2;
      j=2;
    } else if (m.size() == ipoint(4,3)) {
      i=2;
      j=3;
    } else if (m.size() == ipoint(3,4)) {
      i=3;
      j=2;
    } else if (m.size() == ipoint(4,4)) {
      i=3;
      j=3;
    } else {
      return false;
    }

    // just to avoid confusion
    const float m00 = m.at(0,0);
    const float m01 = m.at(0,1);
    const float m02 = m.at(0,j);

    const float m10 = m.at(1,0);
    const float m11 = m.at(1,1);
    const float m12 = m.at(1,j);

    const float m20 = m.at(i,0);
    const float m21 = m.at(i,1);
    const float m22 = m.at(i,j);

    float a;
    a=m22;
    if (a==0.0f) {
      return false; // this would imply an infinite image
    }
    fpoint tl(m02/a,m12/a);

    a=m20*(orig.x-1)+m22;
    if (a==0.0f) {
      return false; // this would imply an infinite image
    }
    fpoint tr((m00*(orig.x-1)+m02)/a,
              (m10*(orig.x-1)+m12)/a);

    a=m21*(orig.y-1)+m22;
    if (a==0.0f) {
      return false; // this would imply an infinite image
    }
    fpoint bl((m01*(orig.y-1)+m02)/a,
              (m11*(orig.y-1)+m12)/a);

    a=m20*(orig.x-1)+m21*(orig.y-1)+m22;
    if (a==0.0f) {
      return false; // this would imply an infinite image
    }
    fpoint br((m00*(orig.x-1)+m01*(orig.y-1)+m02)/a,
              (m10*(orig.x-1)+m11*(orig.y-1)+m12)/a);

    return this->getDimsFromCorners(tl,tr,bl,br,res,offset);

  }

  template <typename T, template<class> class I>
  void matrixTransform<T,I>::helper3x3::forwards(const fpoint& orig,
                                                 fpoint& dest,
                                                 const fpoint& offset) const {
 
    const fmatrix& mat = this->params_.transformation;
    const float n =
      mat.at(2,0)*orig.x + mat.at(2,1)*orig.y + mat.at(2,2);
      
    dest.x = (mat.at(0,0)*orig.x + mat.at(0,1)*orig.y + mat.at(0,2))/n
      - offset.x;
    dest.y = (mat.at(1,0)*orig.x + mat.at(1,1)*orig.y + mat.at(1,2))/n
      - offset.y;
  }

  template <typename T, template<class> class I>
  void matrixTransform<T,I>::helper3x3::backwards(const fpoint& dest,
                                                  fpoint& orig,
                                                  const fpoint& offset) const {
    fpoint tmp(dest.x+offset.x,dest.y+offset.y);
    const float n = m20_*tmp.x + m21_*tmp.y + m22_;
    orig.x = (tmp.x*m00_ + tmp.y*m01_ + m02_)/n;
    orig.y = (tmp.x*m10_ + tmp.y*m11_ + m12_)/n;
  }

  // -------------------------------------------------------------------
  // The helper4x4 class
  // -------------------------------------------------------------------
  template <typename T, template<class> class I>
  matrixTransform<T,I>::helper4x4::helper4x4(interpolator_type& interp,
                                             const parameters& par) 
    : helperBase(interp,par) {
  }
  
  template <typename T, template<class> class I>
  matrixTransform<T,I>::helper4x4::~helper4x4() {
  }

  template <typename T, template<class> class I> bool
  matrixTransform<T,I>::helper4x4::inverse(const double& m00,
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
                                           const double& m33) {

    // take the inverse multiplicative of the determinant to save some ms
    const double s02 = m02*m13 - m03*m12;
    const double s03 = m02*m23 - m03*m22;
    const double s12 = m12*m23 - m13*m22;
    const double s13 = m12*m33 - m13*m32;
    const double s23 = m22*m33 - m23*m32;
    const double s33 = m02*m33 - m03*m32;
    const double det = (m00*(m11*s23 - m21*s13) +
                        m31*(m00*s12 - m10*s03) +
                        m10*(m21*s33 - m01*s23) +
                        m20*(m01*s13 - m11*s33 + m31*s02) +
                        m30*(m11*s03 - m21*s02 - m01*s12));
    
    det_=static_cast<float>(det);
    
    if (abs(det) < std::numeric_limits<float>::epsilon()) {
      // singular matrix
      return false;
    }

    const float idet = 1.0f/det_;

    // inverse of the matrix
    m00_ = float(idet*(m11*s23 - m21*s13 + m31*s12));
    m01_ = float(idet*(m21*s33 - m01*s23 - m31*s03));
    m02_ = float(idet*(m01*s13 - m11*s33 + m31*s02));
    m03_ = float(idet*(m11*s03 - m01*s12 - m21*s02));
 
    m10_ = float(idet*(m20*s13 - m10*s23 - m30*s12)); 
    m11_ = float(idet*(m00*s23 - m20*s33 + m30*s03)); 
    m12_ = float(idet*(m10*s33 - m00*s13 - m30*s02)); 
    m13_ = float(idet*(m00*s12 - m10*s03 + m20*s02));
 
    m20_ = float(idet*(m10*(m21*m33 - m23*m31) - m20*(m11*m33 - m13*m31) +
                       m30*(m11*m23 - m13*m21))); 
    m21_ = float(idet*(m00*(m23*m31 - m21*m33) + m20*(m01*m33 - m03*m31) +
                       m30*(m03*m21 - m01*m23))); 
    m22_ = float(idet*(m00*(m11*m33 - m13*m31) - m10*(m01*m33 - m03*m31) +
                       m30*(m01*m13 - m03*m11))); 
    m23_ = float(idet*(m00*(m13*m21 - m11*m23) + m10*(m01*m23 - m03*m21) -
                       m20*(m01*m13 - m03*m11))); 

    m30_ = float(idet*(m10*(m22*m31 - m21*m32) + m20*(m11*m32 - m12*m31) -
                       m30*(m11*m22 - m12*m21))); 
    m31_ = float(idet*(m00*(m21*m32 - m22*m31) - m20*(m01*m32 - m02*m31) + 
                       m30*(m01*m22 - m02*m21))); 
    m32_ = float(idet*(m00*(m12*m31 - m11*m32) + m10*(m01*m32 - m02*m31) - 
                       m30*(m01*m12 - m02*m11))); 
    m33_ = float(idet*(m00*(m11*m22 - m12*m21) - m10*(m01*m22 - m02*m21) + 
                       m20*(m01*m12 - m02*m11))); 

    return true;
  }


  template <typename T, template<class> class I>
  bool matrixTransform<T,I>::helper4x4::analyzeMatrix() {
    // double check correct dimensions
    const fmatrix& m = this->params_.transformation;

    // double check matrix size
    if (m.size() != ipoint(4,4)) {
      return false;
    }

    return inverse(m.at(0,0),m.at(0,1),m.at(0,2),m.at(0,3),
                   m.at(1,0),m.at(1,1),m.at(1,2),m.at(1,3),
                   m.at(2,0),m.at(2,1),m.at(2,2),m.at(2,3),
                   m.at(3,0),m.at(3,1),m.at(3,2),m.at(3,3));

  } 

  template <typename T, template<class> class I> bool 
  matrixTransform<T,I>::helper4x4::trans(const fmatrix& m,
                                         const ipoint& p,
                                         fpoint& res) const {

    const float a = m.at(3,0)*p.x + m.at(3,1)*p.y + m.at(3,3);
    
    if (a!=0.0f) {
      res.set( (m.at(0,0)*p.x + m.at(0,1)*p.y + m.at(0,3)) / a,
               (m.at(1,0)*p.x + m.at(1,1)*p.y + m.at(1,3)) / a );
      return true;
    } else {
      return false;
    }
    
    return true;
  }

  template <typename T, template<class> class I> bool 
  matrixTransform<T,I>::helper4x4::getMatrix(fmatrix& m) const {
    m.allocate(4,4);

    m.at(0,0) = m00_;
    m.at(0,1) = m01_;
    m.at(0,2) = m02_;
    m.at(0,3) = m03_;

    m.at(1,0) = m10_;
    m.at(1,1) = m11_;
    m.at(1,2) = m12_;
    m.at(1,3) = m13_;

    m.at(2,0) = m20_;
    m.at(2,1) = m21_;
    m.at(2,2) = m22_;
    m.at(2,3) = m23_;

    m.at(3,0) = m30_;
    m.at(3,1) = m31_;
    m.at(3,2) = m32_;
    m.at(3,3) = m33_;
    
    return true;
  }

  template <typename T, template<class> class I> bool 
  matrixTransform<T,I>::helper4x4::apply(const matrix<value_type>& src,
                                         matrix<value_type>& dest,
                                         fpoint& offset) const {
    ipoint destSize;
    if (evalDims(src.size(),destSize,offset)) {

      // dimensions valid: let's transform...

      dest.allocate(destSize);

      // We can avoid an addition renaming the effect of the offset
      const float m03 = m03_ + m00_*offset.x + m01_*offset.y;
      const float m13 = m13_ + m10_*offset.x + m11_*offset.y;
      const float m23 = m23_ + m20_*offset.x + m21_*offset.y;
      const float m33 = m33_ + m30_*offset.x + m31_*offset.y;

      const int ex = destSize.x;
      const int ey = destSize.y;
      int x,y;

      // if det_ is zero, then we cannot invert 
      if (abs(det_) < std::numeric_limits<float>::epsilon()) {
        dest.fill(static_cast<value_type>(0));
        return true;
      }

      // just for clarification, this is the straightforward implementation
      // of the linear transformation:

      // float z; // corresponding z coordinate of the mapped point
      // float a;
      // for (y=0;y<ey;++y) {
      //   for (x=0;x<ex;++x) {
      //     z = -(m20_*x + m21_*y + m23)/m22_;
      //     a = m30_*x + m31_*y + m32_*z + m33;
      //     if (a <= 0.0f) {
      //       // singularity
      //       dest.at(y,x)=static_cast<value_type>(0);
      //     } else {
      //       const float ai = 1.0f/a;
      //       dest.at(y,x) = this->
      //         interpolator_.interpolate(src,
      //                                   (m10_*x + m11_*y + m12_*z + m13)*ai,
      //                                  (m00_*x + m01_*y + m02_*z + m03)*ai);
      //     }
      //   }
      // }

      // now, the previous code can be implemented much more efficiently
      // noting that the change in x and y are always in 1 by 1 steps.

      float xcax,xcyx,xcxx,ycay(0.0f),ycyy(0.0f),ycxy(0.0f);

      const float czx = -m20_/m22_;
      const float czy = -m21_/m22_;
      const float czo = -m23 /m22_;

      const float cax = m30_ + m32_*czx;
      const float cay = m31_ + m32_*czy;
      const float cao = m33  + m32_*czo;

      const float cxx = m00_ + m02_*czx;
      const float cxy = m01_ + m02_*czy;
      const float cxo = m03  + m02_*czo;

      const float cyx = m10_ + m12_*czx;
      const float cyy = m11_ + m12_*czy;
      const float cyo = m13  + m12_*czo;

      for (y=0;y<ey;++y) {

        xcax = cao;
        xcxx = cxo;
        xcyx = cyo;

        for (x=0;x<ex;++x) {
          const float a =  xcax + ycay;

//           if (a <= 0.0f) { // CHECK ME!
//             // If a<0.0f then we are not in an usual proyection, but maybe
//             // this check should be left to the user, i.e. having a parameter
//             // that allows selecting if only =0 is outfiltered, only negative
//             // or only positive...  Some tests were done, and it seems to 
//             // work this way with "normal" projective transformations, but
//             // this may still need to be modified! -- P.Alvarado 090829
//             dest.at(y,x)=static_cast<value_type>(0);
//           } else {
//             const float ai=1.0f/a;
//             dest.at(y,x) =
//               this->interpolator_.interpolate(src,
//                                               (xcyx + ycyy)*ai,
//                                               (xcxx + ycxy)*ai);
//           }

          // The following lines can be changed to allow ploting only on
          // one sign of a (see above)
          // -----vvv-----

          if (a != 0.0f) {
            const float ai=1.0f/a;
            dest.at(y,x) =
              this->interpolator_.interpolate(src,
                                              (xcyx + ycyy)*ai,
                                              (xcxx + ycxy)*ai);
          }

          // -----^^^-----  
        

          xcax+=cax;
          xcxx+=cxx;
          xcyx+=cyx;
        }
        ycay+=cay;
        ycyy+=cyy;
        ycxy+=cxy;
      }
    } else {
      // wrong dimensions: just clean the image, as there is nothing to do
      dest.clear();
      return false;
    }
    return true;


  }




  /*
   * See the other apply for detailed explanations
   */
  template <typename T, template<class> class I> bool 
  matrixTransform<T,I>::helper4x4::apply(const matrix<value_type>& src,
                                         matrix<value_type>& dest,
                                         matrix<float>& z,
                                         fpoint& offset) const {
    ipoint destSize;
    if (evalDims(src.size(),destSize,offset)) {

      // dimensions valid: let's transform...

      dest.allocate(destSize);
      z.allocate(destSize);

      // We can avoid an addition renaming the effect of the offset
      const float m03 = m03_ + m00_*offset.x + m01_*offset.y;
      const float m13 = m13_ + m10_*offset.x + m11_*offset.y;
      const float m23 = m23_ + m20_*offset.x + m21_*offset.y;
      const float m33 = m33_ + m30_*offset.x + m31_*offset.y;

      const int ex = destSize.x;
      const int ey = destSize.y;
      int x,y;

      // if det_ is zero, then we cannot invert 
      if (abs(det_) < std::numeric_limits<float>::epsilon()) {
        dest.fill(static_cast<value_type>(0));
        return true;
      }

      // just for clarification, this is the straightforward implementation
      // of the linear transformation:

      // float z; // corresponding z coordinate of the mapped point
      // float a;
      // for (y=0;y<ey;++y) {
      //   for (x=0;x<ex;++x) {
      //     z = -(m20_*x + m21_*y + m23)/m22_; // note this is z of SRC
      //     a = m30_*x + m31_*y + m32_*z + m33;
      //     if (a <= 0.0f) {
      //       // singularity
      //       dest.at(y,x)=static_cast<value_type>(0);
      //     } else {
      //       const float ai = 1.0f/a;
      //       dest.at(y,x) = this->
      //         interpolator_.interpolate(src,
      //                                   (m10_*x + m11_*y + m12_*z + m13)*ai,
      //                                  (m00_*x + m01_*y + m02_*z + m03)*ai);
      //     }
      //   }
      // }

      // now, the previous code can be implemented much more efficiently
      // noting that the change in x and y are always in 1 by 1 steps.

      float xcax,xczx,xcyx,xcxx;
      float ycay(0.0f),yczy(0.0),ycyy(0.0f),ycxy(0.0f);

      const float czx = -m20_/m22_;
      const float czy = -m21_/m22_;
      const float czo = -m23 /m22_;

      const float cax = m30_ + m32_*czx;
      const float cay = m31_ + m32_*czy;
      const float cao = m33  + m32_*czo;

      const float cxx = m00_ + m02_*czx;
      const float cxy = m01_ + m02_*czy;
      const float cxo = m03  + m02_*czo;

      const float cyx = m10_ + m12_*czx;
      const float cyy = m11_ + m12_*czy;
      const float cyo = m13  + m12_*czo;

      for (y=0;y<ey;++y) {

        xcax = cao;
        xcxx = cxo;
        xcyx = cyo;
        xczx = czo;

        for (x=0;x<ex;++x) {
          const float a =  xcax + ycay;

//           if (a <= 0.0f) { // CHECK ME!
//             // If a<0.0f then we are not in an usual proyection, but maybe
//             // this check should be left to the user, i.e. having a parameter
//             // that allows selecting if only =0 is outfiltered, only negative
//             // or only positive...  Some tests were done, and it seems to 
//             // work this way with "normal" projective transformations, but
//             // this may still need to be modified! -- P.Alvarado 090829
//             dest.at(y,x)=static_cast<value_type>(0);
//           } else {
//             const float ai=1.0f/a;
//             dest.at(y,x) =
//               this->interpolator_.interpolate(src,
//                                               (xcyx + ycyy)*ai,
//                                               (xcxx + ycxy)*ai);
//           }

          // The following lines can be changed to allow ploting only on
          // one sign of a (see above)
          // -----vvv-----

          if (a != 0.0f) {
            const float ai=1.0f/a;
            dest.at(y,x) =
              this->interpolator_.interpolate(src,
                                              (xcyx + ycyy)*ai,
                                              (xcxx + ycxy)*ai);
            z.at(y,x) = xczx + yczy;
          }

          // -----^^^-----  
        

          xcax+=cax;
          xcxx+=cxx;
          xcyx+=cyx;
          xczx+=czx;
        }
        ycay+=cay;
        yczy+=czy;
        ycyy+=cyy;
        ycxy+=cxy;
      }
    } else {
      // wrong dimensions: just clean the image, as there is nothing to do
      dest.clear();
      z.clear();
      return false;
    }
    return true;


  }




  template <typename T, template<class> class I>
  bool matrixTransform<T,I>::helper4x4::evalDims(const ipoint& orig,
                                                 ipoint& res,
                                                 fpoint& offset) const {

    if (this->params_.resizeMode == geometricTransformBase::KeepDimensions) {
      res.copy(orig);
      offset.set(0,0);
      return true;
    }

    const fmatrix& m = this->params_.transformation;
    if (m.size() != ipoint(4,4)) {
      return false;
    }

    fpoint tl,tr,bl,br;

    if (!trans(m,ipoint(0,0),tl)) {
      return false; // this would imply an infinite image
    }
    if (!trans(m,ipoint(orig.x-1,0),tr)) {
      return false; // this would imply an infinite image
    }
    if (!trans(m,ipoint(0,orig.y-1),bl)) {
      return false; // this would imply an infinite image
    }
    if (!trans(m,ipoint(orig.x-1,orig.y-1),br)) {
      return false; // this would imply an infinite image
    }
    
    return this->getDimsFromCorners(tl,tr,bl,br,res,offset);
  }

  template <typename T, template<class> class I>
  void matrixTransform<T,I>::helper4x4::forwards(const fpoint& orig,
                                                 fpoint& dest,
                                                 const fpoint& offset) const {
 
    const fmatrix& mat = this->params_.transformation;
    const float n =
      mat.at(3,0)*orig.x + mat.at(3,1)*orig.y + mat.at(3,3);
      
    dest.x = (mat.at(0,0)*orig.x + mat.at(0,1)*orig.y + mat.at(0,3))/n
      - offset.x;
    dest.y = (mat.at(1,0)*orig.x + mat.at(1,1)*orig.y + mat.at(1,3))/n
      - offset.y;
  }

  template <typename T, template<class> class I>
  void matrixTransform<T,I>::helper4x4::backwards(const fpoint& dest,
                                                  fpoint& orig,
                                                  const fpoint& offset) const {
    fpoint tmp(dest.x+offset.x,dest.y+offset.y);
    const float n = m30_*tmp.x + m31_*tmp.y + m33_;
    orig.x = (tmp.x*m00_ + tmp.y*m01_ + m03_)/n;
    orig.y = (tmp.x*m10_ + tmp.y*m11_ + m13_)/n;
  }


  // -------------------------------------------------------------------
  // The helper4x3 class
  // -------------------------------------------------------------------
  template <typename T, template<class> class I>
  matrixTransform<T,I>::helper4x3::helper4x3(interpolator_type& interp,
                                             const parameters& par) 
    : helper4x4(interp,par) {
  }
  
  template <typename T, template<class> class I>
  matrixTransform<T,I>::helper4x3::~helper4x3() {
  }

  template <typename T, template<class> class I>
  bool matrixTransform<T,I>::helper4x3::analyzeMatrix() {

    // double check correct dimensions
    const fmatrix& m = this->params_.transformation;

    // double check matrix size
    if (m.size() != ipoint(3,4)) {
      return false;
    }

    return this->inverse(m.at(0,0),m.at(0,1),0.0,m.at(0,2),
                         m.at(1,0),m.at(1,1),0.0,m.at(1,2),
                         m.at(2,0),m.at(2,1),1.0,m.at(2,2),
                         m.at(3,0),m.at(3,1),0.0,m.at(3,2));
  } 


  template <typename T, template<class> class I>
  bool matrixTransform<T,I>::helper4x3::evalDims(const ipoint& orig,
                                                 ipoint& res,
                                                 fpoint& offset) const {

    if (this->params_.resizeMode == geometricTransformBase::KeepDimensions) {
      res.copy(orig);
      offset.set(0,0);
      return true;
    }

    const fmatrix& m = this->params_.transformation;
    if (m.size() != ipoint(4,3)) {
      return false;
    }

    fpoint tl,tr,bl,br;

    if (!trans(m,ipoint(0,0),tl)) {
      return false; // this would imply an infinite image
    }
    if (!trans(m,ipoint(orig.x-1,0),tr)) {
      return false; // this would imply an infinite image
    }
    if (!trans(m,ipoint(0,orig.y-1),bl)) {
      return false; // this would imply an infinite image
    }
    if (!trans(m,ipoint(orig.x-1,orig.y-1),br)) {
      return false; // this would imply an infinite image
    }
    
    return this->getDimsFromCorners(tl,tr,bl,br,res,offset);
  }

  template <typename T, template<class> class I> bool 
  matrixTransform<T,I>::helper4x3::trans(const fmatrix& m,
                                         const ipoint& p,
                                         fpoint& res) const {

    const float a = m.at(3,0)*p.x + m.at(3,1)*p.y + m.at(3,2);
    
    if (a!=0.0f) {
      res.set( (m.at(0,0)*p.x + m.at(0,1)*p.y + m.at(0,2)) / a,
               (m.at(1,0)*p.x + m.at(1,1)*p.y + m.at(1,2)) / a );
      return true;
    } else {
      return false;
    }
    
    return true;
  }

  // --------------------------------------------------------------------------
  // Position transformation interface
  // --------------------------------------------------------------------------

  template <typename T, template<class> class I>
  bool matrixTransform<T,I>::use(const ipoint& size) {
    if (notNull(helper_)) {
      ipoint dims;
      usedSize_.copy(size);
      return helper_->evalDims(size,dims,offset_);
    }
    return false;
  } 

  template <typename T, template<class> class I>
  void matrixTransform<T,I>::forwards(const fpoint& orig,
                                      fpoint& dest) const {
    assert(notNull(helper_));
    helper_->forwards(orig,dest,offset_);
  }

  template <typename T, template<class> class I>
  void matrixTransform<T,I>::backwards(const fpoint& dest,
                                       fpoint& orig) const {
    assert(notNull(helper_));
    helper_->backwards(dest,orig,offset_);
  }

  template <typename T, template<class> class I>
  void matrixTransform<T,I>::forwards(const fpointList& orig,
                                      fpointList& dest) const {

    assert(notNull(helper_));
    dest.clear();
    fpointList::const_iterator it;
    fpoint p;
    for (it=orig.begin();it!=orig.end();++it) {
      helper_->forwards((*it),p,offset_);
      dest.push_back(p);
    }
  }

  template <typename T, template<class> class I>
  void matrixTransform<T,I>::forwards(const vector<fpoint>& orig,
                                      vector<fpoint>& dest) const {
    
    assert(notNull(helper_));
    dest.allocate(orig.size());
    vector<fpoint>::iterator dit;
    vector<fpoint>::const_iterator it;
    const vector<fpoint>::const_iterator eit = orig.end();
    
    for (it=orig.begin(),dit=dest.begin();
         it!=eit;
         ++it,++dit) {
      helper_->forwards((*it),(*dit),offset_);
    }
  }
  
  template <typename T, template<class> class I>
  void matrixTransform<T,I>::forwards(const vector<ipoint>& orig,
                                      vector<ipoint>& dest) const {
    
    assert(notNull(helper_));
    dest.allocate(orig.size());
    vector<ipoint>::iterator dit;
    vector<ipoint>::const_iterator it;
    const vector<ipoint>::const_iterator eit = orig.end();
    fpoint p,q;
    
    for (it=orig.begin(),dit=dest.begin();
         it!=eit;
         ++it,++dit) {
      q.set(static_cast<float>((*it).x),
            static_cast<float>((*it).y));
      helper_->forwards(q,p,offset_);
      (*dit).set(iround(p.x),iround(p.y));
    }
  }
  
  template <typename T, template<class> class I>
  void matrixTransform<T,I>::forwards(const ipointList& orig,
                                      ipointList& dest) const {

    assert(notNull(helper_));
    dest.clear();
    ipointList::const_iterator it;
    fpoint p,q;
    for (it=orig.begin();it!=orig.end();++it) {
      q.set(static_cast<float>((*it).x),
            static_cast<float>((*it).y));
      helper_->forwards(q,p,offset_);
      dest.push_back(ipoint(iround(p.x),iround(p.y)));
    }
  }

  template <typename T, template<class> class I>
  void matrixTransform<T,I>::backwards(const fpointList& dest,
                                       fpointList& orig) const {
    assert(notNull(helper_));
    orig.clear();
    fpointList::const_iterator it;
    fpoint p;
    for (it=dest.begin();it!=dest.end();++it) {
      helper_->backwards((*it),p,offset_);
      orig.push_back(p);
    }
  }

  template <typename T, template<class> class I>
  void matrixTransform<T,I>::backwards(const vector<fpoint>& orig,
                                       vector<fpoint>& dest) const {
    
    assert(notNull(helper_));
    dest.allocate(orig.size());
    vector<fpoint>::iterator dit;
    vector<fpoint>::const_iterator it;
    const vector<fpoint>::const_iterator eit = orig.end();
    
    for (it=orig.begin(),dit=dest.begin();
         it!=eit;
         ++it,++dit) {
      helper_->backwards((*it),(*dit),offset_);
    }
  }

  template <typename T, template<class> class I>
  void matrixTransform<T,I>::backwards(const vector<ipoint>& orig,
                                       vector<ipoint>& dest) const {
    
    assert(notNull(helper_));
    dest.allocate(orig.size());
    vector<ipoint>::iterator dit;
    vector<ipoint>::const_iterator it;
    const vector<ipoint>::const_iterator eit = orig.end();
    fpoint p,q;

    for (it=orig.begin(),dit=dest.begin();
         it!=eit;
         ++it,++dit) {
      q.set(static_cast<float>((*it).x),
            static_cast<float>((*it).y));
      helper_->backwards(q,p,offset_);
      (*dit).set(iround(p.x),iround(p.y));
    }
  }

  template <typename T, template<class> class I>
  void matrixTransform<T,I>::backwards(const ipointList& dest,
                                       ipointList& orig) const {
    assert(notNull(helper_));
    orig.clear();
    ipointList::const_iterator it;
    fpoint p,q;
    for (it=dest.begin();it!=dest.end();++it) {
      q.set(static_cast<float>((*it).x),
            static_cast<float>((*it).y));
      helper_->backwards(q,p,offset_);
      orig.push_back(ipoint(iround(p.x),iround(p.y)));
    }
  }

}
