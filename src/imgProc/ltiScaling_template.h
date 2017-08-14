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
 * \file   ltiScaling_template.h
 *         This file contains the functor scaling which can be used to
 *         scale different types of matrices.
 * \author Pablo Alvarado
 * \date   21.11.2003
 *
 * $Id: ltiScaling_template.h,v 1.10 2012-10-31 03:26:42 alvarado Exp $
 */

#include "ltiRound.h"
#include <limits>

namespace lti {

  // ------------------------------------------------------------------------
  // scaling<T,Inter>::scale<T2,I> helper class
  // ------------------------------------------------------------------------

  // non specialized scaling
  template <typename T, template<class> class Inter>
  template <typename T2,template<class> class I>
  bool scaling<T,Inter>::scaleHelper<T2,I>
  ::operator()(const matrix<T2>& src,
               matrix<T2>& dest) const {
    dest.fill(1.0);

    // trivial implementation
    int x,y;
    float fx,fy;

    const float dx = static_cast<float>(src.lastColumn())/dest.lastColumn();
    const float dy = static_cast<float>(src.lastRow())/dest.lastRow();

    int range = interpol_.getRangeOfInfluence();


    if ((src.rows() < range+1) || (src.columns() < range+1)) {
      // the source is too small so we need to use the expensive methods!
      for (fy=0.0,y=0;y<dest.lastRow();++y,fy+=dy) {
        for (fx=0.0,x=0;x<dest.lastColumn();++x,fx+=dx) {
          dest.at(y,x) = interpol_.interpolate(src,fy,fx);
        }
      }
      return true;
    }

    // calculate where checked interpolation must be done
    const int sx = static_cast<int>(range / dx)+1;
    const int ex = dest.lastColumn()-sx;
    const float sfx = sx*dx;
    const int sy = static_cast<int>(range / dy)+1;
    const int ey = dest.lastRow()-sy;
    const float sfy = sy*dy;

    // upper border
    for (fy=0.0,y=0; fy<sfy; ++y, fy+= dy) {
      for (fx=0.0,x=0; x<dest.lastColumn(); ++x, fx += dx) {
        dest.at(y,x) = interpol_.interpolate(src,fy,fx);
      }
    }
    // left border
    for (fy=sfy, y=sy; y<ey; ++y, fy+= dy) {
      for (fx=0.0,x=0; fx<sfx; ++x, fx += dx) {
        dest.at(y,x) = interpol_.interpolate(src,fy,fx);
      }
    }
    // right border
    for (fy=sfy, y=sy; y<ey; ++y, fy+= dy) {
      for (fx=ex*dx,x=ex; x<dest.lastColumn(); ++x, fx += dx) {
        dest.at(y,x) = interpol_.interpolate(src,fy,fx);
      }
    }
    // lower border
    for (fy=ey*dy,y=ey; y<dest.lastRow(); ++y, fy+= dy) {
      for (fx=0.0,x=0; x<dest.lastColumn(); ++x, fx += dx) {
        dest.at(y,x) = interpol_.interpolate(src,fy,fx);
      }
    }
    // center area can be done unchecked
    for (fy=sfy+dy, y=sy+1; y < ey; ++y, fy+=dy) {
      for (fx=sfx+dx, x=sx+1; x < ex; ++x, fx+=dx) {
        dest.at(y,x) = interpol_.interpolateUnchk(src,fy,fx);
      }
    }
    

    return true;
  }
                                                
  // specialized for bilinearInterpolation
  template <typename T, template<class> class Inter>
  template <typename T2>
  bool scaling<T,Inter>::scaleHelper<T2,bilinearInterpolation>::
  operator()(const matrix<T2>& src,
             matrix<T2>& dest) const {
    int x,y,ix,iy,ix1;
    float fx,fy,rx,ry;
    const float dx = static_cast<float>(src.lastColumn())/dest.lastColumn();
    const float dy = static_cast<float>(src.lastRow())/dest.lastRow();

    if ((src.rows() < 2) || (src.columns() < 2)) {
      // the source is too small for a bilinear kernel to pass
      // completely therein, so we need to use the expensive methods!
      for (fy=0.0,y=0;y<dest.lastRow();++y,fy+=dy) {
        for (fx=0.0,x=0;x<dest.lastColumn();++x,fx+=dx) {
          dest.at(y,x) = interpol_.interpolate(src,fy,fx);
        }
      }

      return true;
    }

    for (fy=0.0f,y=0;y<dest.lastRow();++y,fy+=dy) {
      iy = static_cast<int>(fy);
      ry = fy-static_cast<float>(iy);

      const T *const row1 = &src.at(iy,0);
      const T *const row2 = &src.at(iy+1,0);

      for (fx=0.0f,x=0;x<dest.lastColumn();++x,fx+=dx) {
        ix = static_cast<int>(fx);
        ix1 = ix+1;
        rx = fx-static_cast<float>(ix);
        dest.at(y,x) = interpol_.compute(ry,rx,row1[ix],row1[ix1],
                                         row2[ix],row2[ix1]);
      }
      ix = src.lastColumn();
      dest.at(y,x) = interpol_.compute(ry,row1[ix],row2[ix]);
    }

    // last row
    iy = src.lastRow();
    const T *const row1 = &src.at(iy,0);
    for (fx=0.0f,x=0;x<dest.lastColumn();++x,fx+=dx) {
      ix = static_cast<int>(fx);
      rx = fx-static_cast<float>(ix);
      dest.at(y,x) = interpol_.compute(rx,row1[ix],row1[ix+1]);
    }
    
    dest.at(y,x) = row1[src.lastColumn()];

    return true;
  }
  

  // specialized for biquadraticInterpolation
  template <typename T, template<class> class Inter>
  template <typename T2>
  bool scaling<T,Inter>::scaleHelper<T2,biquadraticInterpolation>::
  operator()(const matrix<T2>& src,
             matrix<T2>& dest) const {
    int x,y,ix,iy,ix1,ix2;
    float fx,fy,rx,ry;
    const float dx = static_cast<float>(src.lastColumn())/dest.lastColumn();
    const float dy = static_cast<float>(src.lastRow())/dest.lastRow();

    if ((src.columns() < 3) || (dest.columns() < 3)) {
      // the source is too small for a biquadratic kernel to pass
      // completely therein, so we need to use the expensive methods!
      for (fy=0.0,y=0;y<dest.lastRow();++y,fy+=dy) {
        for (fx=0.0,x=0;x<dest.lastColumn();++x,fx+=dx) {
          dest.at(y,x) = interpol_.interpolate(src,fy,fx);
        }
      }

      return true;
    }

    const int firstCol = iround(ceil(1.0f/dx));
    const int firstRow = iround(ceil(1.0f/dy));

    const int lastCol = static_cast<int>((src.columns() - 1.5f)/dx);
    const int lastRow = static_cast<int>((src.rows() - 1.5f)/dy);
    
    // top
    for (fy=0.0f,y=0;y<firstRow;++y,fy+=dy) {
      for (fx=0.0f,x=0;x<dest.columns();++x,fx+=dx) {
        dest.at(y,x) = interpol_.interpolate(src,fy,fx);
      }
    }

    // the main block

    for (;y<lastRow;++y,fy+=dy) {
      // left
      for (fx=0.0f,x=0;x<firstCol;++x,fx+=dx) {
        dest.at(y,x) = interpol_.interpolate(src,fy,fx);
      }
      
      // center

      // initialize the y coordinates for the whole line
      iy = static_cast<int>(fy);
      ry = fy-static_cast<float>(iy);
      if (ry <= 0.5f) {
        --iy;
      } else {
        ry-=1.0f;
      }

      const T *const row0 = &src.at(iy,0);
      const T *const row1 = &src.at(iy+1,0);
      const T *const row2 = &src.at(iy+2,0);

      // the line itself (without borders)
      for (;x<lastCol;++x,fx+=dx) {
        ix = static_cast<int>(fx);
        rx = fx-static_cast<float>(ix);
        if (rx <= 0.5f) {
          --ix;
        } else {
          rx-=1.0f;
        }

        ix1=ix+1;
        ix2=ix+2;

        dest.at(y,x) = interpol_.compute(ry,rx,
                                         row0[ix],row0[ix1],row0[ix2],
                                         row1[ix],row1[ix1],row1[ix2],
                                         row2[ix],row2[ix1],row2[ix2]);
      }

      // right
      for (;x<dest.columns();++x,fx+=dx) {
        dest.at(y,x) = interpol_.interpolate(src,fy,fx);
      }
    }

    // bottom
    for (;y<dest.rows();++y,fy+=dy) {
      for (fx=0.0f,x=0;x<dest.columns();++x,fx+=dx) {
        dest.at(y,x) = interpol_.interpolate(src,fy,fx);
      }
    }

    return true;
  }

  // specialized for nearestNeighborInterpolation
  template <typename T, template<class> class Inter>
  template <typename T2>
  bool scaling<T,Inter>::scaleHelper<T2,nearestNeighborInterpolation>::
    operator()(const matrix<T2>& src,
               matrix<T2>& dest) const {
      
    // TODO: use nearestNeighborInterpolation instead
    int x,y;
    float fx,fy;

    const float dx = static_cast<float>(src.lastColumn())/dest.lastColumn();
    const float dy = static_cast<float>(src.lastRow())/dest.lastRow();

    for (fy=0.0f,y=0;y<dest.rows();++y,fy+=dy) {
      const T *const row1 = &src.at(static_cast<int>(fy+0.5f),0);
      for (fx=0.0f,x=0;x<dest.columns();++x,fx+=dx) {
        dest.at(y,x) = row1[static_cast<int>(fx+0.5f)];
      }
    }
    return true;
  }

  // specialized for bicubicInterpolationWorkaround
  template <typename T, template<class> class Inter>
  template <typename T2>
  bool scaling<T,Inter>::scaleHelper<T2,bicubicInterpolationWorkaround>::
    operator()(const matrix<T2>& src,
               matrix<T2>& dest) const {
    
    int x,y,ix,iy;
    float fx,fy,rx,ry;
    const float dx = static_cast<float>(src.lastColumn())/dest.lastColumn();
    const float dy = static_cast<float>(src.lastRow())/dest.lastRow();
    
    // the interpolation used is in fact genericInterpolation!
    
    if ((src.columns() < 4) || (src.columns() < 4)) {
      // the source is too small for a bicubic kernel to pass
      // completely therein, so we need to use the expensive methods!
       for (fy=0.0,y=0;y<dest.lastRow();++y,fy+=dy) {
         for (fx=0.0,x=0;x<dest.lastColumn();++x,fx+=dx) {
           dest.at(y,x) = interpol_.interpolate(src,fy,fx);
         }
       }

       return true;
     }

     // The interpolation can be computed efficiently only in the middle.
     // The borders require special consideration to compute the boundary
     // conditions.

     // first compute the limits of the valid middle region, where the
     // kernel fits completely

     const int firstCol = iround(ceil(1.0f/dx));
     const int firstRow = iround(ceil(1.0f/dy));

     const int lastCol = static_cast<int>((src.columns() - 2.0f)/dx);
     const int lastRow = static_cast<int>((src.rows() - 2.0f)/dy);

     const T* rows[4];

     // top
     for (fy=0.0f,y=0;y<firstRow;++y,fy+=dy) {
       for (fx=0.0f,x=0;x<dest.columns();++x,fx+=dx) {
         dest.at(y,x) = interpol_.interpolate(src,fy,fx);
       }
     }

     // vertical central block has three parts: left, center and right 
     for (;y<lastRow;++y,fy+=dy) {
       // left
       for (fx=0.0f,x=0;x<firstCol;++x,fx+=dx) {
         dest.at(y,x) = interpol_.interpolate(src,fy,fx);
       }

       // center
      
       // initialize the y coordinates for the whole line
       iy = static_cast<int>(fy);
       ry = fy-static_cast<float>(iy);

       // the line itself (without borders)
       for (;x<lastCol;++x,fx+=dx) {
         ix = static_cast<int>(fx);
         rx = fx-static_cast<float>(ix);
         --ix;

         rows[0] = &src.at(iy-1,ix);
         rows[1] = &src.at(iy  ,ix);
         rows[2] = &src.at(iy+1,ix);
         rows[3] = &src.at(iy+2,ix);

         dest.at(y,x) = interpol_.compute(ry,rx,rows);
       }

       // right
       for (;x<dest.columns();++x,fx+=dx) {
         dest.at(y,x) = interpol_.interpolate(src,fy,fx);
       }
     }

     // bottom
     for (;y<dest.rows();++y,fy+=dy) {
       for (fx=0.0f,x=0;x<dest.columns();++x,fx+=dx) {
         dest.at(y,x) = interpol_.interpolate(src,fy,fx);
       }
     }

     return true;
    
  }

  // --------------------------------------------------
  // scaling<T,Inter>::parameters
  // --------------------------------------------------

  // default constructor
  template <typename T, template<class> class Inter>
  scaling<T,Inter>::parameters::parameters()
    : functor::parameters() {
    scale = fpoint(sqrt(2.0f),sqrt(2.0f));
    boundaryType = Zero;
  }

  // copy constructor
  template <typename T, template<class> class Inter>
  scaling<T,Inter>::parameters::parameters(const parameters& other)
    : functor::parameters() {
    copy(other);
  }

  // destructor
  template <typename T, template<class> class Inter>
  scaling<T,Inter>::parameters::~parameters() {
  }

  // return class name
  template <typename T, template<class> class Inter>
  const std::string& scaling<T,Inter>::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // copy member
  template <typename T, template<class> class Inter>
  typename scaling<T,Inter>::parameters&
  scaling<T,Inter>::parameters::copy(const parameters& other) {
    functor::parameters::copy(other);
    
    scale = other.scale;
    boundaryType = other.boundaryType;

    interpolationParameters.copy(other.interpolationParameters);

    return *this;
  }

  // alias for copy member
  template <typename T, template<class> class Inter>
  typename scaling<T,Inter>::parameters&
    scaling<T,Inter>::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  // clone member
  template <typename T, template<class> class Inter>
  typename scaling<T,Inter>::parameters* 
  scaling<T,Inter>::parameters::clone() const {
    return new parameters(*this);
  }

  // newInstance member
  template <typename T, template<class> class Inter>
  typename scaling<T,Inter>::parameters* 
  scaling<T,Inter>::parameters::newInstance() const {
    return new parameters();
  }

  /*
   * write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  template <typename T, template<class> class Inter>
  bool scaling<T,Inter>::parameters::write(ioHandler& handler,
                                           const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {
      
      lti::write(handler,"scale",scale);
      lti::write(handler,"boundaryType",boundaryType);
    }

    b = b && functor::parameters::write(handler,false);

    if (complete) {
      b = b && handler.writeEnd();
    }

    return b;
  }


  /*
   * read the parameters from the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also read, otherwise only the data block will be read.
   * @return true if write was successful
   */
  template <typename T, template<class> class Inter>
  bool scaling<T,Inter>::parameters::read(ioHandler& handler,
                                 const bool complete) {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {
      lti::read(handler,"scale",scale);
      lti::read(handler,"boundaryType",boundaryType);
    }

    b = b && functor::parameters::read(handler,false);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }


  // --------------------------------------------------
  // scaling
  // --------------------------------------------------

  // default constructor
  template <typename T, template<class> class Inter>
  scaling<T,Inter>::scaling()
    : functor() {

    // create an instance of the parameters with the default values
    parameters defaultParameters;
    // set the default parameters
    setParameters(defaultParameters);

  }

  // copy constructor
  template <typename T, template<class> class Inter>
  scaling<T,Inter>::scaling(const parameters& par)
    : functor() {

    //init worker
    //    worker=new scalingWorker(par.interpolationType);
    // set the given parameters
    setParameters(par);
  }


  // copy constructor
  template <typename T, template<class> class Inter>
  scaling<T,Inter>::scaling(const scaling& other)
    : functor()  {
    copy(other);
  }

  // destructor
  template <typename T, template<class> class Inter>
  scaling<T,Inter>::~scaling() {
    // delete worker;
//     worker = 0;
  }

  // return class name
  template <typename T, template<class> class Inter>
  const std::string& scaling<T,Inter>::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // copy member
  template <typename T, template<class> class Inter>
  scaling<T,Inter>& 
  scaling<T,Inter>::copy(const scaling<T,Inter>& other) {
    functor::copy(other);

    // quick and dirty
    scale_.interpol_.copy(other.scale_.interpol_);
    //interpol_.copy(other.interpol_);

    return (*this);
  }

  // alias for copy member
  template <typename T, template<class> class Inter>
  scaling<T,Inter>& 
  scaling<T,Inter>::operator=(const scaling<T,Inter>& other) {
    return (copy(other));
  }

  // clone member
  template <typename T, template<class> class Inter>
  scaling<T,Inter>* scaling<T,Inter>::clone() const {
    return new scaling(*this);
  }

  // newInstance member
  template <typename T, template<class> class Inter>
  scaling<T,Inter>* scaling<T,Inter>::newInstance() const {
    return new scaling();
  }

  // return parameters
  template <typename T, template<class> class Inter>
  const typename scaling<T,Inter>::parameters& 
  scaling<T,Inter>::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }

  template <typename T, template<class> class Inter>
  bool scaling<T,Inter>::updateParameters() {
    const parameters& par = getParameters();
    typename Inter<value_type>::parameters 
      tmp(par.interpolationParameters);
    // force using boundaryType of this class
    tmp.boundaryType = par.boundaryType;
    return scale_.interpol_.setParameters(tmp);
  }


  // set boundary type
  template <typename T, template<class> class Inter>
  bool scaling<T,Inter>::setBoundaryType(const eBoundaryType boundaryType) {
    parameters *param = 
      dynamic_cast<parameters*>(&parametersManager::getRWParameters());
    if(param==0) {
      throw invalidParametersException(name());
    }
    param->boundaryType = boundaryType;
    return updateParameters();
  }


  // -------------------------------------------------------------------
  // The apply-methods!
  // -------------------------------------------------------------------

  
  // On place apply for type image!
  template <typename T, template<class> class Inter>
  bool scaling<T,Inter>::apply(matrix<T>& srcdest) const {
    matrix<T> tmpImg;
    if (apply(srcdest,tmpImg)) {
      tmpImg.detach(srcdest);
      return true;
    }
    return false;
  }

  // On copy apply for type image!
  template <typename T, template<class> class Inter>
  bool scaling<T,Inter>::apply(const matrix<T>& src,matrix<T>& dest) const {
    const parameters& par = getParameters();
    return scale(par.scale,src,dest);
  }

  // --------------------
  // shortcuts with scale
  // --------------------

  // On place scale for type image!
  template <typename T, template<class> class Inter>
  bool scaling<T,Inter>::scale(const fpoint& s, matrix<T>& srcdest) const {
    matrix<T> tmpImg;
    if (scale(s,srcdest,tmpImg)) {
      tmpImg.detach(srcdest);
      return true;
    }
    return false;
  }

  // On copy scale for type image!
  template <typename T, template<class> class Inter>
  bool scaling<T,Inter>::scale(const fpoint& theScale,
                               const matrix<T>& src, matrix<T>& dst) const {    
    // no scaling at all
    if (theScale == fpoint(1.0f,1.0f)) {
      dst.copy(src);
      return true;
    }

    // no output at all
    if (src.empty() || (theScale.x == 0.0f) || (theScale.y == 0.0f)) {
      dst.clear();
      return true;
    }

    // resize destination
    int x,y;

    // avoid empty images
    x = max(1,iround(src.columns()*theScale.x));
    y = max(1,iround(src.rows()*theScale.y));

    dst.allocate(y,x);

    return scale_(src,dst);
  }

  // --------------------
  // shortcuts with scale
  // --------------------

  // On place scale for type image!
  template <typename T, template<class> class Inter>
  bool scaling<T,Inter>::scale(const float s, matrix<T>& srcdest) const {
    matrix<T> tmpImg;
    if (scale(s,srcdest,tmpImg)) {
      tmpImg.detach(srcdest);
      return true;
    }
    return false;
  }


  // On copy scale for type image!
  template <typename T, template<class> class Inter>
  bool scaling<T,Inter>::scale(const float s,
                               const matrix<T>& src,matrix<T>& dest) const {
    return scale(fpoint(s,s),src,dest);
  }

}
