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
 * \file   ltiAffineTransform_template.h
 * \author Roland Neul
 * \author Arnd Hannemann
 * \date   22.04.2005
 *
 * revisions ..: $Id: ltiAffineTransform_template.h,v 1.8 2007-12-26 04:49:23 alvarado Exp $
 */

namespace lti {

  // -------------------------------------------------------------------
  // affineTransform<T>::parameters
  // -------------------------------------------------------------------
  
  // default constructor
  template <typename T, template<class> class Inter>
  affineTransform<T,Inter>::parameters::parameters()
    : modifier::parameters(), interpolationParameters() {
    patchRadius = int();
    transformType=Full;
  }

  // copy constructor
  template <typename T, template<class> class Inter>
  affineTransform<T,Inter>::parameters::parameters(const parameters& other)
    : modifier::parameters(), interpolationParameters() {
    copy(other);
  }

  // destructor
  template <typename T, template<class> class Inter>
  affineTransform<T,Inter>::parameters::~parameters() {
  }

  // class name
  template <typename T, template<class> class Inter>
  const std::string& affineTransform<T,Inter>::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone member
  template <typename T, template<class> class Inter>
  typename affineTransform<T,Inter>::parameters* 
  affineTransform<T,Inter>::parameters::clone() const {
    return new parameters(*this);
  }

  // newInstance member
  template <typename T, template<class> class Inter>
  typename affineTransform<T,Inter>::parameters*
  affineTransform<T,Inter>::parameters::newInstance() const {
    return new parameters();
  }

  // copy member
  template <typename T, template<class> class Inter>
  typename affineTransform<T,Inter>::parameters&
    affineTransform<T,Inter>::parameters::copy(const parameters& other) {
    modifier::parameters::copy(other);
    
      patchRadius = other.patchRadius;
      transformType = other.transformType;
      interpolationParameters.copy(other.interpolationParameters);

    return *this;
  }

  // alias for copy member
  template <typename T, template<class> class Inter>
  typename affineTransform<T,Inter>::parameters&
    affineTransform<T,Inter>::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  // write parameters
  template <typename T, template<class> class Inter>
  bool affineTransform<T,Inter>::parameters::write(ioHandler& handler,
                                         const bool complete) const
  {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {
     
      lti::write(handler,"patchRadius",patchRadius);
      
      switch (transformType) {
        case Skew:
          lti::write(handler, "transformType", "Skew");
          break;
        case Full:
          lti::write(handler, "transformType", "Full");
          break;
        default:
          handler.setStatusString("Unknonwn transfromType in affineTransform\n");
          b=false;
          lti::write(handler, "transformType", "Unknown");
          break;
      }

      lti::write(handler,"interpolationParameters",interpolationParameters);
    }

    b = b && modifier::parameters::write(handler,false);

    if (complete) {
      b = b && handler.writeEnd();
    }

    return b;
  }

  // read parameters
  template <typename T, template<class> class Inter>
  bool affineTransform<T,Inter>::parameters::read(ioHandler& handler,
                                                  const bool complete) {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {
      lti::read(handler,"patchRadius",patchRadius);

      std::string tmp;
      lti::read(handler,"transformType", tmp); 
      if ((tmp=="Skew") || (tmp=="skew")) {
        transformType=Skew;
      } else if ((tmp=="Full") || (tmp=="full")) {
        transformType=Full;
      } else {
        transformType=Skew; //default
        handler.setStatusString("Unknown transformType in affineTransform\n");
        b=false;
      }
        
      lti::read(handler,"interpolationParameters",interpolationParameters);
    }
    b = b && modifier::parameters::read(handler,false);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

  // --------------------------------------------------
  // affineTransform<T,Inter>
  // --------------------------------------------------

  // default constructor
  template <typename T, template<class> class Inter>
  affineTransform<T,Inter>::affineTransform()
    : modifier(){

    // create an instance of the parameters with the default values
    parameters defaultParameters;
    // set the default parameters
    setParameters(defaultParameters);
  }

  // default constructor
  template <typename T, template<class> class Inter>
  affineTransform<T,Inter>::affineTransform(const parameters& par)
    : modifier() {

    // set the given parameters
    setParameters(par);
  }


  // copy constructor
  template <typename T, template<class> class Inter>
  affineTransform<T,Inter>::affineTransform(const affineTransform& other)
    : modifier() {
    copy(other);
  }

  // destructor
  template <typename T, template<class> class Inter>
  affineTransform<T,Inter>::~affineTransform() {
  }


  // class name
  template <typename T, template<class> class Inter>
  const std::string& affineTransform<T,Inter>::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // copy member
  template <typename T, template<class> class Inter>
  affineTransform<T,Inter>&
    affineTransform<T,Inter>::copy(const affineTransform<T,Inter>& other) {
      modifier::copy(other);

    mapsAreValid_ = other.mapsAreValid_;
    if (mapsAreValid_) {
      xMap_.copy(other.xMap_);
      yMap_.copy(other.yMap_);
    }

    xamaxMap_ = other.xamaxMap_;
    yamaxMap_ = other.yamaxMap_;
    
    interpol_.copy(other.interpol_);
    return (*this);
  }

  // alias for copy member
  template <typename T, template<class> class Inter>
  affineTransform<T,Inter>&
    affineTransform<T,Inter>::operator=(const affineTransform<T,Inter>& other) {
    return (copy(other));
  }


  // clone member
  template <typename T, template<class> class Inter>
  affineTransform<T,Inter>* affineTransform<T,Inter>::clone() const {
    return new affineTransform<T,Inter>(*this);
  }

  // newInstance  member
  template <typename T, template<class> class Inter>
  affineTransform<T,Inter>* affineTransform<T,Inter>::newInstance() const {
    return new affineTransform<T,Inter>();
  }
  
  // return parameters
  template <typename T, template<class> class Inter>
  const typename affineTransform<T,Inter>::parameters&
  affineTransform<T,Inter>::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }

  template <typename T, template<class> class Inter>
  bool affineTransform<T,Inter>::use(const matrix<float>& transMatrix) {
    
    // make sure we got at least a 2x2 matrix
    assert((transMatrix.columns() == 2 || transMatrix.columns() == 3) &&
           transMatrix.rows() == 2);
    
    switch (getParameters().transformType) {
      case Skew:
        return useSkew(transMatrix);
      case Full:
        return useFull(transMatrix);
      default:
        setStatusString("Unknown transformType in affineTransform\n");
        return false;
    }
  }

  // build transformation mapping to get faster transformations
  template <typename T, template<class> class Inter>
  bool affineTransform<T,Inter>::useSkew(const matrix<float>& transMatrix) {
    // get pixel offsets and start point (top left corner)
    const parameters& param=getParameters();
    const int sz = 2*param.patchRadius+1;
    const double r = static_cast<double>(param.patchRadius);
    const double dxx = transMatrix.at(0, 0)/r;
    const double dyx = transMatrix.at(0, 1)/r;
    const double dxy = transMatrix.at(1, 0)/r;
    const double dyy = transMatrix.at(1, 1)/r;
    const double xbase = - (dxx + dyx) * r;
    const double ybase = - (dyy + dxy) * r;
    // generate mapping for transformation
    matrix<float>::iterator itx = xMap_.begin();
    matrix<float>::iterator ity = yMap_.begin();
    double xofsRow = 0.0, yofsRow = 0.0;
    for (int y = 0; y < sz; y++) {
      double xofs = xofsRow, yofs = yofsRow;
      for (int x = 0; x < sz; ++x, ++itx, ++ity) {
        *ity = static_cast<float>(ybase + yofs);
        *itx = static_cast<float>(xbase + xofs);
        xofs += dxx; yofs += dxy;
      }
      // next row
      xofsRow += dyx; yofsRow += dyy;
    }
    // checking necessary?
    const double adxx = fabs(dxx), adxy = fabs(dxy),
                 adyx = fabs(dyx), adyy = fabs(dyy);
    xamaxMap_ = (adxx + adyx) * r;
    yamaxMap_ = (adxy + adyy) * r;
    mapsAreValid_ = true;
    _lti_debug("bounds (x,y): " << xamaxMap_ << ", " << yamaxMap_ <<"\n");
    _lti_debug3("xmap\n" << xMap_ << "\n");
    _lti_debug3("ymap\n" << yMap_ << "\n");
    return true;    
  }

  // build transformation mapping to get faster transformations
  template <typename T, template<class> class Inter>
  bool affineTransform<T,Inter>::useFull(const matrix<float>& transMatrix) {

    // get pixel offsets and start point (top left corner)
    const parameters& param=getParameters();
    const int sz=2*param.patchRadius+1;
    const double r = static_cast<double>(param.patchRadius);
    const double dxx = transMatrix.at(0, 0)/r;
    const double dyx = transMatrix.at(0, 1)/r;
    const double dxy = transMatrix.at(1, 0)/r;
    const double dyy = transMatrix.at(1, 1)/r;
    const double xbase = - (dxx + dyx) * r;
    const double ybase = - (dyy + dxy) * r;

//     _lti_debug("base:" << xbase << " " << ybase << "\n");
    // generate mapping for transformation
    matrix<float>::iterator itx = xMap_.begin();
    matrix<float>::iterator ity = yMap_.begin();
    double tx;
    double ty;
    double ax=xbase;
    double ay=ybase;
    for (int y = 0; y < sz; y++, ax+=dyx, ay+=dyy) {
      tx=ax;
      ty=ay;
      for (int x = 0; x < sz; ++x, ++itx, ++ity, tx+=dxx, ty+=dxy) {
        *itx = static_cast<float>(tx);
        *ity = static_cast<float>(ty);
      }
    }
    // checking necessary?
    const double adxx = fabs(dxx), adxy = fabs(dxy),
                 adyx = fabs(dyx), adyy = fabs(dyy);
    xamaxMap_ = (adxx + adyx) * r;
    yamaxMap_ = (adxy + adyy) * r;
    mapsAreValid_ = true;
    _lti_debug("bounds (x,y): " << xamaxMap_ << ", " << yamaxMap_ <<"\n");
    _lti_debug3("xmap\n" << xMap_ << "\n");
    _lti_debug3("ymap\n" << yMap_ << "\n");
    return true;    
  }

  // set new parameters and reset attributes
  template <typename T, template<class> class Inter>
  bool affineTransform<T,Inter>::updateParameters() {
    const parameters& param = getParameters();
    // call base class method
    bool rc = modifier::updateParameters();

    // set parameters of internal interpolation functor
    // with correct boundary type
    typename Inter<T>::parameters tmp(param.interpolationParameters);
    tmp.boundaryType = param.boundaryType;
    interpol_.setParameters(tmp);
    
    // initialize mappings
    const int sz = 2*param.patchRadius+1;
    xMap_.assign(sz, sz, std::numeric_limits<float>::max());
    yMap_.assign(sz, sz, std::numeric_limits<float>::max());
    mapsAreValid_ = false;
    return rc;
  }    
  

  // -------------------------------------------------------------------
  // The apply-methods!
  // -------------------------------------------------------------------
  
  template <typename T, template<class> class Inter>
  bool affineTransform<T,Inter>::apply(const matrix<T>& src,
                                       const point<float>& p,
                                       matrix<T>& dest) const {
    
    const parameters param = getParameters();
    
    if (src.empty()) {
      setStatusString("empty src matrix in affineTransform\n");
      dest.clear();
      return false;
    }

    if (!mapsAreValid_) {
      setStatusString("affineTransfrom::use must be called before this apply method\n");
      dest.clear();
      return false;
    }
    
    // apply our mapping
    const int sz = 2*param.patchRadius+1;
    dest.allocate(sz, sz);
    matrix<float>::const_iterator itx = xMap_.begin();
    matrix<float>::const_iterator ity = yMap_.begin();
    // checking necessary?
    if (//(!isfinite(xamaxMap_)) || (!isfinite(yamaxMap_)) ||
        p.x - xamaxMap_ < 0.5 ||
        p.x + xamaxMap_ >= static_cast<float>(src.columns()) - 1.5 ||
        p.y - yamaxMap_ < 0.5 ||
        p.y + yamaxMap_ >= static_cast<float>(src.rows()) - 1.5) {
      // use checked interpolator
      for (typename matrix<T>::iterator dstit = dest.begin();
           dstit != dest.end(); ++itx, ++ity, ++dstit) {
        *dstit = interpol_.interpolate(src, *ity + p.y, *itx + p.x);
      }
    } else {
      // use unchecked interpolator
      for (typename matrix<T>::iterator dstit = dest.begin();
           dstit != dest.end(); ++itx, ++ity, ++dstit) {
        *dstit = interpol_.interpolateUnchk(src, *ity + p.y, *itx + p.x);
      }
    }
    
    return true;
  }

  template <typename T, template<class> class Inter>
  bool affineTransform<T,Inter>::apply(const matrix<T>& src,
                                       const matrix<float>& transMatrix,
                                       matrix<T>& dest) const {
    
    // make sure we got a 2x3 matrix
    assert(transMatrix.columns() == 3 && transMatrix.rows() == 2);
    
    const double cx = transMatrix.at(0,2);
    const double cy = transMatrix.at(1,2);

    switch (getParameters().transformType) {
      case Skew:
        return applySkew(src, 
                         cx, cy, 
                         transMatrix, dest);
      case Full:
        return applyFull(src, 
                         cx, cy, 
                         transMatrix, dest);
      default:
        setStatusString("Unknown transformType in affineTransform\n");
        dest.clear();
        return false;
    }
  }

  template <typename T, template<class> class Inter>
  bool affineTransform<T,Inter>::apply(const matrix<T>& src,
                                       const fpoint& center,
                                       const matrix<float>& transMatrix,
                                       matrix<T>& dest) const {
    
    // make sure we got a 2x2 matrix
    assert(transMatrix.columns() == 2 && transMatrix.rows() == 2);
    
    const double cx = center.x;
    const double cy = center.y;

    switch (getParameters().transformType) {
      case Skew:
        return applySkew(src, 
                         cx, cy, 
                         transMatrix, dest);
      case Full:
        return applyFull(src, 
                         cx, cy, 
                         transMatrix, dest);
      default:
        setStatusString("Unknown transformType in affineTransform\n");
        dest.clear();
        return false;
    }
  }

  template <typename T, template<class> class Inter>
  bool affineTransform<T,Inter>::applySkew(const matrix<T>& src,
                                           const double& cx,
                                           const double& cy,
                                           const matrix<float>& transMatrix,
                                           matrix<T>& dest) const {
    const parameters param = getParameters();
    const double r = static_cast<double>(param.patchRadius);
    const int sz = 2*param.patchRadius+1;
    dest.allocate(sz, sz);
    // get pixel offsets and start point (top left corner)
    const double dxx = transMatrix.at(0, 0)/r;
    const double dyx = transMatrix.at(0, 1)/r;
    const double dxy = transMatrix.at(1, 0)/r;
    const double dyy = transMatrix.at(1, 1)/r;
    const double xbase = - (dxx + dyx) * r + cx;
    const double ybase = - (dyy + dxy) * r + cy;
    // transform patch
    typename matrix<T>::iterator it = dest.begin();
    double xofsRow = 0.0, yofsRow = 0.0;
    double xofs, yofs;
    
    // checking necessary?
    const double adxx = fabs(dxx), adxy = fabs(dxy),
      adyx = fabs(dyx), adyy = fabs(dyy);
    const double xamax = (adxx + adyx) * r;
    const double yamax = (adxy + adyy) * r;
    if ((!isfinite(yamax)) || (!isfinite(xamax)) ||
        cy - yamax < 0.0 ||
        static_cast<int>(cy + yamax) >= src.lastRow() ||
        cx - xamax < 0.0 ||
        static_cast<int>(cx + xamax) >= src.lastColumn()) {
      // use checked interpolator
      for (int y = 0; y < sz; y++) {
        xofs = xofsRow; yofs = yofsRow;
        for (int x = 0; x < sz; ++x, ++it) {
          *it = interpol_.interpolate(src, ybase + yofs,
                                      xbase + xofs);
          xofs += dxx; yofs += dxy;
        }
        // next row
        xofsRow += dyx; yofsRow += dyy;
      }
    } else {
      // use unchecked interpolator
      for (int y = 0; y < sz; y++) {
        xofs = xofsRow; yofs = yofsRow;
        for (int x = 0; x < sz; ++x, ++it) {
          *it = interpol_.interpolateUnchk(src, ybase + yofs,
                                          xbase + xofs);
          xofs += dxx; yofs += dxy;
        }
        // next row
        xofsRow += dyx; yofsRow += dyy;
        }
    }
    return true;
  }

  template <typename T, template<class> class Inter>
  bool affineTransform<T,Inter>::applyFull(const matrix<T>& src,
                                           const double& cx,
                                           const double& cy,
                                           const matrix<float>& transMatrix,
                                           matrix<T>& dest) const {
    const parameters param = getParameters();
    const double r = static_cast<double>(param.patchRadius);
    const int sz = 2*param.patchRadius+1;
    dest.allocate(sz, sz);
    // get pixel offsets and start point (top left corner)
    const double dxx = transMatrix.at(0, 0)/r;
    const double dyx = transMatrix.at(0, 1)/r;
    const double dxy = transMatrix.at(1, 0)/r;
    const double dyy = transMatrix.at(1, 1)/r;
    const double xbase = - (dxx + dyx) * r + cx;
    const double ybase = - (dyy + dxy) * r + cy;
    // transform patch
    typename matrix<T>::iterator it = dest.begin();
    double tx;
    double ty;
    double ax=xbase;
    double ay=ybase;
    
    // checking necessary?
    const double adxx = fabs(dxx), adxy = fabs(dxy),
      adyx = fabs(dyx), adyy = fabs(dyy);
    const double xamax = (adxx + adyx) * r;
    const double yamax = (adxy + adyy) * r;
    if ((!isfinite(yamax)) || (!isfinite(xamax)) ||
        cy - yamax < 0.0 ||
        static_cast<int>(cy + yamax) >= src.lastRow() ||
        cx - xamax < 0.0 ||
        static_cast<int>(cx + xamax) >= src.lastColumn()) {
      // use checked interpolator
      for (int y = 0; y < sz; y++, ax+=dyx, ay+=dyy) {
        tx=ax;
        ty=ay;
        for (int x = 0; x < sz; ++x, ++it, tx+=dxx, ty+=dxy) {
          *it = interpol_.interpolate(src, ty, tx);
        }
      }
    } else {
      // use unchecked interpolator
      for (int y = 0; y < sz; y++, ax+=dyx, ay+=dyy) {
        tx=ax;
        ty=ay;
        for (int x = 0; x < sz; ++x,  ++it, tx+=dxx, ty+=dxy) {
          *it = interpol_.interpolateUnchk(src, ty, tx);
        }
      }
    }
    return true;
  }
  
} // namespace
