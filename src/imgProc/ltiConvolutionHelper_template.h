/*
 * Copyright (C) 1998-2004
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

/* 
 * \file   ltiConvolutionHelper_template.h
 *         Contains a helper class for convolution.
 * \author Pablo Alvarado
 * \date   17.11.1999
 *
 * revisions ..: $Id: ltiConvolutionHelper_template.h,v 1.20 2010-06-23 01:29:12 alvarado Exp $
 */

#include "ltiDebug.h"

namespace lti {

  // ---------------------------------------------
  //   CONVOLUTION HELPER CLASSES
  // ---------------------------------------------


  // ---------------------------------------------
  // 1D CONVOLUTION
  // ---------------------------------------------

  template<class T,class A>
  lti::convHelper1D<T,A>::convHelper1D()
    : kernel_(0),tmpKernel_(0) {
  }

  template<class T,class A>
  lti::convHelper1D<T,A>::~convHelper1D() {
    delete tmpKernel_;
  }

  template<class T,class A>
  template<typename U>
  bool lti::convHelper1D<T,A>::cast(const container* kern,
                                    bool mirrored) {

    const kernel1D<U>* tmpCast = dynamic_cast< const kernel1D<U>* >(kern);
    if (notNull(tmpCast)) {
      tmpKernel_ = new kernel1D<T>;

      // U = from this type is started the cast process
      // T = to this type is done the cast

      if (typeInfo<U>::isFloatingPointType()) { // from floaing point ->
        
        if (typeInfo<T>::isFloatingPointType()) { // -> to floating point
          tmpKernel_->castFrom(*tmpCast);
        } else { // -> to fixed point

          // create a temporal copy of the input kernel
          kernel1D<U>* tmpFloat = new kernel1D<U>(*tmpCast);
          tmpFloat->multiply(static_cast<U>(typeInfo<T>::suggestedNorm()));
          tmpKernel_->castFrom(*tmpFloat);
          delete tmpFloat;
        }
      } else { // from fixed point ->
        tmpKernel_->castFrom(*tmpCast);
 
        if (typeInfo<T>::isFloatingPointType()) { // -> to floating point
          tmpKernel_->denormalize();
        }
      }
     
      if (mirrored) { // want to "fold" the kernel?
        tmpKernel_->mirror();
      }

      // indicate to use the casted kernel
      kernel_ = tmpKernel_;
      return true;
    }
    
    return false;
  }
  

  template<class T,class A>
  bool lti::convHelper1D<T,A>::setKernel(const container* kern,
                                         bool mirrored) {
    kernel_ = dynamic_cast<const kernel1D<T>*>(kern);

    // it is not the expected type, try to cast from know possibilities
    if (isNull(kernel_)) {

      delete tmpKernel_;
      tmpKernel_ = 0;
      
      // try to cast from the first possible kernel type
      bool casted = (cast<int>(kern,mirrored)   || // C Standard says the
                     cast<ubyte>(kern,mirrored) || // conditions are tested
                     cast<float>(kern,mirrored) || // in sequence until the 
                     cast<double>(kern,mirrored)); // first true 
      if (!casted) {
        // no possible cast found!  abort!
        return false;
      }
      assert(notNull(kernel_)); // the condition above should check this
                                // but better be sure!
    } else {
      // expected kernel type.  Just check if we have to mirror it
      if (mirrored) {
        delete tmpKernel_;
        tmpKernel_ = kernel_->clone();
        tmpKernel_->mirror();
        kernel_ = tmpKernel_;
      }
    }  
    
    // check for kernel symmetry
    
    if (kernel_->isSymmetric()) {
      kernelSymmetry_ = lti::Symmetric;
    } else if (kernel_->isAsymmetric()) {
      kernelSymmetry_ = lti::Asymmetric;
    } else {
      kernelSymmetry_ = lti::Default;
    }

    return (notNull(kernel_));
  }

  // ----------------------------------------
  //        Apply method for kernel1D
  // ----------------------------------------

  template<class T,class A>
  void lti::convHelper1D<T,A>::apply(const vector<T>& src,
                                     vector<T>& dest,
                                     const eBoundaryType& boundaryType) {
    if (kernelSymmetry_ == lti::Default) {
      applyAny(src,dest,boundaryType);
    } else {
      applySym(src,dest,boundaryType);
    }
  }

  template<class T,class A>
  void lti::convHelper1D<T,A>::applyAny(const vector<T>& src,
                                        vector<T>& dest,
                                        const eBoundaryType& boundaryType) {

    // Note P.Alvarado (April 24, 2010) 
    
    // The following code has been resurected from a very old LTI-Lib 1 version
    // (1.3 of the CVS), since the changes made by P. Doerfler and B. Hoffman
    // just broke a working code on kernels with an uncentered zero position.
    //
    // The following code is anyway faster.

    // alias for kernel term
    const kernel1D<T>& kern = *kernel_;

    // need kernel as vector
    const vector<T>& filter = kern;

    if (src.size() < filter.size()) {
      // ---------------------------------------
      // if the kernel is bigger than the vector
      // ---------------------------------------

      applyHuge(src,dest,boundaryType);
      return;
    }

    // initialize member "norm"
    lresult_.setNorm( kern.getNorm() );

    int x,i,f,xx,srcSize2;

    const T *xp,*fp,*xxp,*endp;
    T *ip;

    // last valid index of the filter vector
    const int filterLast = filter.lastIdx();

    // initialize size of dest (do not initialize)!)
    if (dest.size() != src.size()) {
      dest.allocate(src.size());
    }

    // middle block filter (the whole kernel can be applied here!)
    const int begin = (kern.lastIdx()<0) ? -kern.lastIdx() : 0;
    const int end   = ((kern.lastIdx()>=filter.size()) ?
                       src.size()-kern.lastIdx() :
                       src.size()-filter.size()+1);
    
    // This is the standard way to implement the convolution:
    //   for (x=begin,i=kern.lastIdx()+x;x<end;++x,++i) {
    //     // x: begin in src vector
    //     // i: place to write local convolution
    //     // lresult : local result
    //     // xx: sample on image to be checked
    //     for (xx=x,lresult_.reset(),f=filterLast;f>=0;--f,++xx ) {
    //       lresult_.accumulate(filter.at(f),src.at(xx));
    //     }
    //     dest.at(i)=lresult_.getResult();
    //   }
    // but is much faster to use pointers and jump over the at() calls:
    // - Valgrind shows us if the memory management is ok
    // - This way is faster for debug mode also

    endp = src.data();
    endp+=end;

    const T *const filterFirstp = (filter.data())-1;
    const T *const filterLastp  = &filter.at(filterLast);

    // ----------------------------------------
    // if the kernel is smaller than the vector (the normal case)

    // The convolution with pointer arithmetic (faster this way!)
    for (xp=&src.at(begin),ip=&dest.at(kern.lastIdx()+begin);
         xp!=endp;++xp,++ip) {
      // x: begin in src vector
      // i: place to write local convolution
      // lresult : local result
      // xx: sample on image to be checked
      for (xxp=xp,lresult_.reset(),fp=filterLastp;
           fp!=filterFirstp;--fp,++xxp ) {
        lresult_.accumulate(*fp,*xxp);
      }
      (*ip)=lresult_.getResult();
    }

    // -----------------------
    //    border conditions
    // -----------------------

    switch (boundaryType) {
    case lti::NoBoundary:
      break;
    case lti::Zero:
      // left border!
      for (x=begin-1,i=kern.lastIdx()+x;
           x>=-kern.lastIdx();
           --x,--i) {
        for (xx=x+filterLast,lresult_.reset(),f=0;
             ((xx>=0) && (f<filter.size()));
             ++f,--xx) {
          lresult_.accumulate(filter.at(f),src.at(xx));
        }
        dest.at(i)=lresult_.getResult();
      }

      // right border!
      for (x=end,i=kern.lastIdx()+x;
           x<src.size()-kern.lastIdx();
           ++x,++i) {
        for (xx=x,lresult_.reset(),f=filterLast;
             ((xx<src.size()) && (f>=0));
             --f,++xx) {
          lresult_.accumulate(filter.at(f),src.at(xx));
        }
        dest.at(i)=lresult_.getResult();
      }

      break;

    case lti::Mirror:
      // left border!
      for (x=begin-1,i=kern.lastIdx()+x;
           x>=-kern.lastIdx();
           --x,--i) {
        for (xx=x+filterLast,lresult_.reset(),f=0;
             (f<filter.size());
             ++f,--xx) {
          if (xx>=0)
            lresult_.accumulate(filter.at(f),src.at(xx));
          else
            lresult_.accumulate(filter.at(f),src.at(-xx));
        }
        dest.at(i)=lresult_.getResult();
      }

      // right border!
      srcSize2=2*src.size()-1;
      for (x=end,i=kern.lastIdx()+x;
           x<src.size()-kern.lastIdx();
           ++x,++i) {
        for (xx=x,lresult_.reset(),f=filterLast;
             (f>=0);
             --f,++xx) {
          if (xx<src.size())
            lresult_.accumulate(filter.at(f),src.at(xx));
          else
            lresult_.accumulate(filter.at(f),src.at(srcSize2-xx));
        }
        dest.at(i)=lresult_.getResult();
      }
      break;

    case lti::Periodic:
      // left border!
      for (x=begin-1,i=kern.lastIdx()+x;
           x>=-kern.lastIdx();
           --x,--i) {
        for (xx=x+filterLast,lresult_.reset(),f=0;
             (f<filter.size());
             ++f,--xx) {
          if (xx>=0)
            lresult_.accumulate(filter.at(f),src.at(xx));
          else
            lresult_.accumulate(filter.at(f),src.at(src.size()+xx));
        }
        dest.at(i)=lresult_.getResult();
      }

      // right border!
      for (x=end,i=kern.lastIdx()+x;
           x<src.size()-kern.lastIdx();
           ++x,++i) {
        for (xx=x,lresult_.reset(),f=filterLast;
             (f>=0);
             --f,++xx) {
          if (xx<src.size())
            lresult_.accumulate(filter.at(f),src.at(xx));
          else
            lresult_.accumulate(filter.at(f),src.at(xx-src.size()));
        }
        dest.at(i)=lresult_.getResult();
      }

      break;
    case lti::Constant:
      T y;

      y = src.at(0);
      // left border!
      for (x=begin-1,i=kern.lastIdx()+x;
           x>=-kern.lastIdx();
           --x,--i) {
        for (xx=x+filterLast,lresult_.reset(),f=0;
             (f<filter.size());
             ++f,--xx) {
          if (xx>=0)
            lresult_.accumulate(filter.at(f),src.at(xx));
          else
            lresult_.accumulate(filter.at(f),y);
        }
        dest.at(i)=lresult_.getResult();
      }

      y = src.at(src.lastIdx());
      // right border!
      for (x=end,i=kern.lastIdx()+x;
           x<src.size()-kern.lastIdx();
           ++x,++i) {
        for (xx=x,lresult_.reset(),f=filterLast;
             (f>=0);
             --f,++xx) {
          if (xx<src.size())
            lresult_.accumulate(filter.at(f),src.at(xx));
          else
            lresult_.accumulate(filter.at(f),y);
        }
        dest.at(i)=lresult_.getResult();
      }

      break;
    default:
      throw exception("unknown border type!");
    }
  }

  template<class T,class A>
  void lti::convHelper1D<T,A>::applyHuge(const vector<T>& src,
                                         vector<T>& dest,
                                         const eBoundaryType& boundaryType) {

    // alias for kernel term
    const kernel1D<T>& kern = *kernel_;

    // initialize member "norm"
    lresult_.setNorm( kern.getNorm() );

    // initialize size of dest (not need to initialization!)
    if (dest.size() != src.size()) {
      dest.allocate(src.size());
    }

    // ensure that this method is called only if kernel is larger than input
    assert(src.size() < kern.size());
    
    int x,f,xx;

    const int size = src.size();
      
    // ---------------------------------------
    // if the kernel is bigger than the vector
    // ---------------------------------------
    
    switch (boundaryType) {
    case lti::NoBoundary:
      break;
      
    case lti::Zero:

      // TODO: this can be considerably optimized
      for (x=0;x<size;++x) {
        for (f = kern.lastIdx(), xx = x+f,lresult_.reset();
             f >= kern.firstIdx(); --f,--xx) {
          if ((xx>=0) && (xx<size)) {
            lresult_.accumulate(kern.at(f),src.at(xx));
          }
        }
        dest.at(x) = lresult_.getResult();
      }
      
      break;
      
    case lti::Mirror:
      
      // TODO: this can be considerably optimized
      for (x=0;x<size;++x) {
        for (f = kern.lastIdx(), xx = x+f,lresult_.reset();
             f >= kern.firstIdx(); 
             --f,--xx) {
          if ((xx>=0) && (xx<size)) {
            lresult_.accumulate(kern.at(f),src.at(xx));
          } else {
            int p,xp;
            p = xx/size;
            xp = xx%size;
            
            if (xx < 0) {
              p = -p + 1;
              if (xp < 0) {
                xp += size;
              }
            }
            
            if ((p & 01) != 0) {
              xp = size-1-xp;
            }
            
            lresult_.accumulate(kern.at(f),src.at(xp));
            
          }
        }
        dest.at(x) = lresult_.getResult();
      }
      
      break;
    case lti::Periodic:

      // TODO: this can be considerably optimized
      for (x=0;x<size;++x) {
        for (f = kern.lastIdx(), xx = x+f ,lresult_.reset();
             f >= kern.firstIdx();
             --f,--xx) {
          int xp;
          
          xp = xx%size;
          
          if (xp < 0) {
            xp += size;
          }
          
          lresult_.accumulate(kern.at(f),src.at(xp));
        }
        dest.at(x) = lresult_.getResult();
      }
      
      break;
      
    case lti::Constant:

      // TODO: this can be considerably optimized

      for (x=0;x<size;++x) {
        for (f = kern.lastIdx(), xx = x+f,lresult_.reset();
             f >= kern.firstIdx(); --f,--xx) {
          if (xx < 0) {
            lresult_.accumulate(kern.at(f),src.at(0));
          } else if (xx > src.lastIdx()) {
            lresult_.accumulate(kern.at(f),src.at(src.lastIdx()));
          } else {
            lresult_.accumulate(kern.at(f),src.at(xx));
          }
        }
        dest.at(x) = lresult_.getResult();
      }
      
      break;
    default:
      throw exception("unknown border type");
    }
  }

  // Apply to kernels with odd or even symmetry
  template<class T,class A>
  void lti::convHelper1D<T,A>::applySym(const vector<T>& src,
                                        vector<T>& dest,
                                        const eBoundaryType& boundaryType) {

    // Note P.Alvarado (April 24, 2010) 
    
    // The following code has been resurected from a very old LTI-Lib 1 version
    // (1.3 of the CVS), since the changes made by P. Doerfler and B. Hoffman
    // just broke a working code on kernels with an uncentered zero position.
    //
    // The following code is anyway faster.

    // alias for kernel term
    const kernel1D<T>& kern = *kernel_;

    // need kernel as vector
    const vector<T>& filter = kern;

    if (src.size() < filter.size()) {
      applyHuge(src,dest,boundaryType);
      return;
    }

    int x,i,f,xx,srcSize2;
    lresult_.reset();

    const T *xp,*fp,*xxp,*xrp,*endp;
    T *ip;

    // initialize member "norm"
    lresult_.setNorm( kern.getNorm() );

    // last valid index of the filter vector
    const int filterLast = filter.lastIdx();

    // initialize size of dest (not need to initialization!)
    if (dest.size() != src.size()) {
      dest.allocate(src.size());
    }

    // middle block filter (the whole kernel can be applied here!)
    const int begin = (kern.lastIdx()<0) ? -kern.lastIdx() : 0;
    const int end   = ((kern.lastIdx()>=filter.size()) ?
                       src.size()-kern.lastIdx() :
                       src.size()-filter.size()+1);
    
    // This is the standard way to implement the convolution:
    //   for (x=begin,i=kern.lastIdx()+x;x<end;++x,++i) {
    //     // x: begin in src vector
    //     // i: place to write local convolution
    //     // lresult : local result
    //     // xx: sample on image to be checked
    //     for (xx=x,lresult_.reset(),f=filterLast;f>=0;--f,++xx ) {
    //       lresult_.accumulate(filter.at(f),src.at(xx));
    //     }
    //     dest.at(i)=lresult_.getResult();
    //   }
    // but is much faster to use pointers and jump over the at() calls:
    // - valgrind shows us if the memory management is ok
    // - This way is faster for debug mode also

    endp = src.data();
    endp+=end;

    // since kernel is symmetric, we just need to visit half of it
    const T *const filterFirstp = &filter.at(filterLast/2); 
    const T *const filterLastp = &filter.at(filterLast); 

    // ----------------------------------------
    // if the kernel is smaller than the vector (the normal case)

    switch(kernelSymmetry_) {
    case lti::Asymmetric:
      // The convolution with pointer arithmetic (faster this way!)
      for (xp=&src.at(begin),ip=&dest.at(kern.lastIdx()+begin);
           xp!=endp;++xp,++ip) {
        // x: begin in src vector
        // i: place to write local convolution
        // lresult : local result
        // xx: sample on image to be checked
        // xr: sample from the right side of the kernel
        for (xxp=xp,xrp=xxp+filterLast,lresult_.reset(),fp=filterLastp;
             fp!=filterFirstp;--fp,++xxp,--xrp ) {
          lresult_.accumulateASym(*fp,*xxp,*xrp);
        }
        lresult_.accumulateZero(*xxp); // center of the filter
        (*ip)=lresult_.getResult();
      }
      break;
    case lti::Symmetric:
      // The convolution with pointer arithmetic (faster this way!)
      for (xp=&src.at(begin),ip=&dest.at(kern.lastIdx()+begin);
           xp!=endp;++xp,++ip) {
        // x: begin in src vector
        // i: place to write local convolution
        // lresult : local result
        // xx: sample on image to be checked
        // xr: sample from the right side of the kernel
        for (xxp=xp,xrp=xxp+filterLast,lresult_.reset(),fp=filterLastp;
             fp!=filterFirstp;--fp,++xxp,--xrp ) {
          lresult_.accumulateSym(*fp,*xxp,*xrp);
        }
        lresult_.accumulate(*fp,*xxp); // center of the filter
        (*ip)=lresult_.getResult();
      }
      break;
    default:
      return; // we should not be here at all!!
    }
      
    // -----------------------
    //    border conditions
    // -----------------------

    switch (boundaryType) {
    case lti::NoBoundary:
      break;
    case lti::Zero:
      // left border!
      for (x=begin-1,i=kern.lastIdx()+x;
           x>=-kern.lastIdx();
           --x,--i) {
        for (xx=x+filterLast,lresult_.reset(),f=0;
             ((xx>=0) && (f<filter.size()));
             ++f,--xx) {
          lresult_.accumulate(filter.at(f),src.at(xx));
        }
        dest.at(i)=lresult_.getResult();
      }

      // right border!
      for (x=end,i=kern.lastIdx()+x;
           x<src.size()-kern.lastIdx();
           ++x,++i) {
        for (xx=x,lresult_.reset(),f=filterLast;
             ((xx<src.size()) && (f>=0));
             --f,++xx) {
          lresult_.accumulate(filter.at(f),src.at(xx));
        }
        dest.at(i)=lresult_.getResult();
      }

      break;

    case lti::Mirror:
      // left border!
      for (x=begin-1,i=kern.lastIdx()+x;
           x>=-kern.lastIdx();
           --x,--i) {
        for (xx=x+filterLast,lresult_.reset(),f=0;
             (f<filter.size());
             ++f,--xx) {
          if (xx>=0)
            lresult_.accumulate(filter.at(f),src.at(xx));
          else
            lresult_.accumulate(filter.at(f),src.at(-xx));
        }
        dest.at(i)=lresult_.getResult();
      }

      // right border!
      srcSize2=2*src.size()-1;
      for (x=end,i=kern.lastIdx()+x;
           x<src.size()-kern.lastIdx();
           ++x,++i) {
        for (xx=x,lresult_.reset(),f=filterLast;
             (f>=0);
             --f,++xx) {
          if (xx<src.size())
            lresult_.accumulate(filter.at(f),src.at(xx));
          else
            lresult_.accumulate(filter.at(f),src.at(srcSize2-xx));
        }
        dest.at(i)=lresult_.getResult();
      }
      break;

    case lti::Periodic:
      // left border!
      for (x=begin-1,i=kern.lastIdx()+x;
           x>=-kern.lastIdx();
           --x,--i) {
        for (xx=x+filterLast,lresult_.reset(),f=0;
             (f<filter.size());
             ++f,--xx) {
          if (xx>=0)
            lresult_.accumulate(filter.at(f),src.at(xx));
          else
            lresult_.accumulate(filter.at(f),src.at(src.size()+xx));
        }
        dest.at(i)=lresult_.getResult();
      }

      // right border!
      for (x=end,i=kern.lastIdx()+x;
           x<src.size()-kern.lastIdx();
           ++x,++i) {
        for (xx=x,lresult_.reset(),f=filterLast;
             (f>=0);
             --f,++xx) {
          if (xx<src.size())
            lresult_.accumulate(filter.at(f),src.at(xx));
          else
            lresult_.accumulate(filter.at(f),src.at(xx-src.size()));
        }
        dest.at(i)=lresult_.getResult();
      }

      break;
    case lti::Constant:
      T y;

      y = src.at(0);
      // left border!
      for (x=begin-1,i=kern.lastIdx()+x;
           x>=-kern.lastIdx();
           --x,--i) {
        for (xx=x+filterLast,lresult_.reset(),f=0;
             (f<filter.size());
             ++f,--xx) {
          if (xx>=0)
            lresult_.accumulate(filter.at(f),src.at(xx));
          else
            lresult_.accumulate(filter.at(f),y);
        }
        dest.at(i)=lresult_.getResult();
      }

      y = src.at(src.lastIdx());
      // right border!
      for (x=end,i=kern.lastIdx()+x;
           x<src.size()-kern.lastIdx();
           ++x,++i) {
        for (xx=x,lresult_.reset(),f=filterLast;
             (f>=0);
             --f,++xx) {
          if (xx<src.size())
            lresult_.accumulate(filter.at(f),src.at(xx));
          else
            lresult_.accumulate(filter.at(f),y);
        }
        dest.at(i)=lresult_.getResult();
      }

      break;
    default:
      throw exception("unknown border type!");
    }
  }

  template<class T,class A>
  void lti::convHelper1D<T,A>::applyCol(const matrix<T>& src,
                                        matrix<T>& dest,
                                        const eBoundaryType& boundaryType) {
    if (kernelSymmetry_ == lti::Default) {
      applyAnyCol(src,dest,boundaryType);
    } else {
      applySymCol(src,dest,boundaryType);
    }

  }

  template<class T,class A>
  void lti::convHelper1D<T,A>::applyAnyCol(const matrix<T>& src,
                                                 matrix<T>& dest,
                                           const eBoundaryType& boundaryType) {


    // alias for kernel term
    const kernel1D<T>& kern = *kernel_;

    // need kernel as vector
    const vector<T>& filter = kern;

    if (src.rows() < filter.size()) {
      applyHugeCol(src,dest,boundaryType);
      return;
    }

    // initialize member "norm"
    lresult_.setNorm( kern.getNorm() );

    // initialize size of dest (not need to initialization!)
    dest.allocate(src.size());

    // middle block filter (the whole kernel can be applied here!)
    const int begin = (kern.lastIdx()<0) ? -kern.lastIdx() : 0;
    const int end   = ((kern.lastIdx()>=filter.size()) ?
                       src.rows()-kern.lastIdx() :
                       src.rows()-filter.size()+1);

    // last valid index of the filter vector
    const int filterLast = filter.lastIdx();
    const T *const filterFirstp= (filter.data())-1;
    const T *const filterLastp = &filter.at(filterLast);

    // ----------------------------------------
    // the kernel is smaller than the vector
    const int srcColumns = src.columns();

    int x,i,f,xx,srcSize2;
    int col;
    const T *fp, *sp, *ssp;
    T* dp;

    for (x=begin,i=kern.lastIdx()+x;x<end;++x,++i) { // x is the row of src

      // for each column of the matrix do...
      dp=&dest.at(i,0);
      sp=&src.at(x,0);
      const T* spe = sp+srcColumns;
      for (;sp!=spe; ++sp,++dp) {
        // x: begin in src vector
        // i: place to write local convolution
        // lresult : local result
        // xx: sample on image to be checked
        for (ssp=sp,lresult_.reset(),fp=filterLastp;
             fp!=filterFirstp;--fp,ssp+=srcColumns) {
          lresult_.accumulate(*fp,*ssp);
        }
        *dp=lresult_.getResult();
      }
    }

    // -----------------------
    //    border conditions
    // -----------------------
    
    switch (boundaryType) {
    case lti::NoBoundary:
      break;
    case lti::Zero:
      // left border!
      for (x=begin-1,i=kern.lastIdx()+x;
           x>=-kern.lastIdx();
           --x,--i) {
        for (col=0;col<src.columns();++col) {
          for (xx=x+filterLast,lresult_.reset(),f=0;
               ((xx>=0) && (f<filter.size()));
               ++f,--xx) {
            lresult_.accumulate(filter.at(f),src.at(xx,col));
          }
          dest.at(i,col)=lresult_.getResult();
        }
      }
      
      // right border!
      for (x=end,i=kern.lastIdx()+x;
           x<src.rows()-kern.lastIdx();
           ++x,++i) {
        for (col=0;col<src.columns();++col) {
          for (xx=x,lresult_.reset(),f=filterLast;
               ((xx<src.rows()) && (f>=0));
               --f,++xx) {
            lresult_.accumulate(filter.at(f),src.at(xx,col));
          }
          dest.at(i,col)=lresult_.getResult();
        }
      }
      break;
      
    case lti::Mirror:
      // left border!
      for (x=begin-1,i=kern.lastIdx()+x;
           x>=-kern.lastIdx();
           --x,--i) {
        for (col=0;col<src.columns();++col) {
          for (xx=x+filterLast,lresult_.reset(),f=0;
               (f<filter.size());
               ++f,--xx) {
            if (xx>=0)
              lresult_.accumulate(filter.at(f),src.at(xx,col));
            else
              lresult_.accumulate(filter.at(f),src.at(-xx,col));
          }
          dest.at(i,col)=lresult_.getResult();
        }
      }
      // right border!
      srcSize2=2*src.lastRow();
      for (x=end,i=kern.lastIdx()+x;
           x<src.rows()-kern.lastIdx();
           ++x,++i) {
        for (col=0;col<src.columns();++col) {
          for (xx=x,lresult_.reset(),f=filterLast;
               (f>=0);
               --f,++xx) {
            if (xx<src.rows())
              lresult_.accumulate(filter.at(f),src.at(xx,col));
            else
              lresult_.accumulate(filter.at(f),src.at(srcSize2-xx,col));
          }
          dest.at(i,col)=lresult_.getResult();
        }
      }
      break;
      
    case lti::Periodic:
      // left border!
      for (x=begin-1,i=kern.lastIdx()+x;
           x>=-kern.lastIdx();
           --x,--i) {
        for (col=0;col<src.columns();++col) {
          for (xx=x+filterLast,lresult_.reset(),f=0;
               (f<filter.size());
               ++f,--xx) {
            if (xx>=0)
              lresult_.accumulate(filter.at(f),src.at(xx,col));
            else
              lresult_.accumulate(filter.at(f),src.at(src.rows()+xx,col));
          }
          dest.at(i,col)=lresult_.getResult();
        }
      }
      
      // right border!
      for (x=end,i=kern.lastIdx()+x;
           x<src.rows()-kern.lastIdx();
           ++x,++i) {
        for (col=0;col<src.columns();++col) {
          for (xx=x,lresult_.reset(),f=filterLast;
               (f>=0);
               --f,++xx) {
            if (xx<src.rows())
              lresult_.accumulate(filter.at(f),src.at(xx,col));
            else
              lresult_.accumulate(filter.at(f),src.at(xx-src.rows(),col));
          }
          dest.at(i,col)=lresult_.getResult();
        }
      }
      break;
      
    case lti::Constant:
      // left border!
      for (x=begin-1,i=kern.lastIdx()+x;
           x>=-kern.lastIdx();
           --x,--i) {
        for (col=0;col<src.columns();++col) {
          for (xx=x+filterLast,lresult_.reset(),f=0;
               (f<filter.size());
               ++f,--xx) {
            if (xx>=0)
              lresult_.accumulate(filter.at(f),src.at(xx,col));
            else
              lresult_.accumulate(filter.at(f),src.at(0,col));
          }
          dest.at(i,col)=lresult_.getResult();
        }
      }
      
      // right border!
      for (x=end,i=kern.lastIdx()+x;
           x<src.rows()-kern.lastIdx();
           ++x,++i) {
        for (col=0;col<src.columns();++col) {
          for (xx=x,lresult_.reset(),f=filterLast;
               (f>=0);
               --f,++xx) {
            if (xx<src.rows())
              lresult_.accumulate(filter.at(f),src.at(xx,col));
            else
              lresult_.accumulate(filter.at(f),src.at(src.lastRow(),col));
          }
          dest.at(i,col)=lresult_.getResult();
        }
      }
      break;
    default:
      throw exception("unknown border type");
    }
    
  }

  template<class T,class A>
  void lti::convHelper1D<T,A>::applyHugeCol(const matrix<T>& src,
                                            matrix<T>& dest,
                                            const eBoundaryType& boundaryType) {


    // alias for kernel term
    const kernel1D<T>& kern = *kernel_;

    assert(src.rows() < kern.size());  // this method is ONLY for huge kernels

    dest.allocate(src.size());

    int x,f,xx;
    int col;
    lresult_.reset();

    // initialize member "norm"
    lresult_.setNorm( kern.getNorm() );

    // initialize size of dest (not need to initialization!)
    dest.allocate(src.size());

    // ---------------------------------------
    // if the kernel is bigger than the vector

    switch (boundaryType) {
    case lti::NoBoundary:
      break;
    case lti::Zero:
      for (x=0;x<src.rows();++x) {
        for (col=0;col<src.columns();++col) {
          for (f = kern.lastIdx(), xx = x+f,lresult_.reset();
               f >= kern.firstIdx(); --f,--xx) {
            if ((xx>=0) && (xx<src.rows())) {
              lresult_.accumulate(kern.at(f),src.at(xx,col));
            }
          }
          dest.at(x,col) = lresult_.getResult();
        }
      }
      break;
      
    case lti::Mirror:
      for (x=0;x<src.rows();++x) {
        for (col=0;col<src.columns();++col) {
          for (f = kern.lastIdx(), xx = x+f,lresult_.reset();
               f >= kern.firstIdx(); --f,--xx) {
            if ((xx>=0) && (xx<src.rows())) {
              lresult_.accumulate(kern.at(f),src.at(xx,col));
            } else {
              int p,xp;
              p = xx/src.rows();
              xp = xx%src.rows();

              if (xx < 0) {
                p = -p + 1;
                if (xp < 0) {
                  xp += src.rows();
                }
              }

              if ((p & 01) != 0) { // even number?
                xp = src.rows() -1 -xp;
              }

              lresult_.accumulate(kern.at(f),src.at(xp,col));

            }
          }
          dest.at(x,col) = lresult_.getResult();
        }
      }
      break;

    case lti::Periodic:

      for (x=0;x<src.rows();++x) {
        for (col=0;col<src.columns();++col) {
          for (f = kern.lastIdx(), xx = x+f,lresult_.reset();
               f >= kern.firstIdx(); --f,--xx) {
            int xp;

            xp = xx%src.rows();
            if (xp < 0) {
              xp+=src.rows();
            }
            lresult_.accumulate(kern.at(f),src.at(xp,col));
          }
          dest.at(x,col) = lresult_.getResult();
        }
      }
      break;

    case lti::Constant:
      for (x=0;x<src.rows();++x) {
        for (col=0;col<src.columns();++col) {
          for (f = kern.lastIdx(), xx = x+f,lresult_.reset();
               f >= kern.firstIdx(); --f,--xx) {
            if (xx < 0) {
              lresult_.accumulate(kern.at(f),src.at(0,col));
            } else if (xx >= src.rows()) {
              lresult_.accumulate(kern.at(f),src.at(src.lastRow(),col));
            } else {
              lresult_.accumulate(kern.at(f),src.at(xx,col));
            }
          }
          dest.at(x,col) = lresult_.getResult();
        }
      }

      break;
    default:
      throw exception("unknown border type!");
    }
  }

  template<class T,class A>
  void lti::convHelper1D<T,A>::applySymCol(const matrix<T>& src,
                                                 matrix<T>& dest,
                                           const eBoundaryType& boundaryType) {


    // alias for kernel term
    const kernel1D<T>& kern = *kernel_;

    // need kernel as vector
    const vector<T>& filter = kern;

    if (src.rows() < filter.size()) {
      applyHugeCol(src,dest,boundaryType);
      return;
    }

    // initialize member "norm"
    lresult_.setNorm( kern.getNorm() );

    // initialize size of dest (not need to initialization!)
    dest.allocate(src.size());

    // middle block filter (the whole kernel can be applied here!)
    const int begin = (kern.lastIdx()<0) ? -kern.lastIdx() : 0;
    const int end   = ((kern.lastIdx()>=filter.size()) ?
                       src.rows()-kern.lastIdx() :
                       src.rows()-filter.size()+1);

    // last valid index of the filter vector
    const int filterLast = filter.lastIdx();
    const T *const filterFirstp= &filter.at(filterLast/2);
    const T *const filterLastp = &filter.at(filterLast);

    // ----------------------------------------
    // the kernel is smaller than the vector
    const int srcColumns = src.columns();

    int x,i,f,xx,srcSize2;
    int col;
    const T *fp,*sp,*spd,*ssp,*sbp;
    T* dp;

    switch(kernelSymmetry_) {
    case lti::Asymmetric:

      // for each row at the destination do
      for (x=begin,i=kern.lastIdx()+x;x<end;++x,++i) { // x is the row of src
        
        // for each column of the destination row
        dp=&dest.at(i,0);
        sp=&src.at(x,0);
        spd=&src.at(x+filterLast,0);

        const T* spe = sp+srcColumns;
        for (;sp!=spe; ++sp,++dp,++spd) { // for each row in the mask
          // x: begin in src vector
          // i: place to write local convolution
          // lresult : local result
          // xx: sample on image to be checked
          for (ssp=sp,sbp=spd,lresult_.reset(),fp=filterLastp;
               fp!=filterFirstp;--fp,ssp+=srcColumns,sbp-=srcColumns) {
            lresult_.accumulateASym(*fp,*ssp,*sbp);
          }
          lresult_.accumulateZero(*ssp); // center of the filter
          *dp=lresult_.getResult();
        }
      }
      break;

    case lti::Symmetric:

      // for each row at the destination do
      for (x=begin,i=kern.lastIdx()+x;x<end;++x,++i) { // x is the row of src
        
        // for each column of the destination row
        dp=&dest.at(i,0);
        sp=&src.at(x,0);
        spd=&src.at(x+filterLast,0);

        const T* spe = sp+srcColumns;
        for (;sp!=spe; ++sp,++dp,++spd) { // for each row in the mask
          // x: begin in src vector
          // i: place to write local convolution
          // lresult : local result
          // xx: sample on image to be checked
          for (ssp=sp,sbp=spd,lresult_.reset(),fp=filterLastp;
               fp!=filterFirstp;--fp,ssp+=srcColumns,sbp-=srcColumns) {
            lresult_.accumulateSym(*fp,*ssp,*sbp);
          }
          lresult_.accumulate(*fp,*ssp); // center of the filter
          *dp=lresult_.getResult();
        }
      }
      break;
      
    default:
      return; // we shouldn't get here at all!
    }
    
    // -----------------------
    //    border conditions
    // -----------------------
    
    switch (boundaryType) {
    case lti::NoBoundary:
      break;
    case lti::Zero:
      // left border!
      for (x=begin-1,i=kern.lastIdx()+x;
           x>=-kern.lastIdx();
           --x,--i) {
        for (col=0;col<src.columns();++col) {
          for (xx=x+filterLast,lresult_.reset(),f=0;
               ((xx>=0) && (f<filter.size()));
               ++f,--xx) {
            lresult_.accumulate(filter.at(f),src.at(xx,col));
          }
          dest.at(i,col)=lresult_.getResult();
        }
      }
      
      // right border!
      for (x=end,i=kern.lastIdx()+x;
           x<src.rows()-kern.lastIdx();
           ++x,++i) {
        for (col=0;col<src.columns();++col) {
          for (xx=x,lresult_.reset(),f=filterLast;
               ((xx<src.rows()) && (f>=0));
               --f,++xx) {
            lresult_.accumulate(filter.at(f),src.at(xx,col));
          }
          dest.at(i,col)=lresult_.getResult();
        }
      }
      break;
      
    case lti::Mirror:
      // left border!
      for (x=begin-1,i=kern.lastIdx()+x;
           x>=-kern.lastIdx();
           --x,--i) {
        for (col=0;col<src.columns();++col) {
          for (xx=x+filterLast,lresult_.reset(),f=0;
               (f<filter.size());
               ++f,--xx) {
            if (xx>=0)
              lresult_.accumulate(filter.at(f),src.at(xx,col));
            else
              lresult_.accumulate(filter.at(f),src.at(-xx,col));
          }
          dest.at(i,col)=lresult_.getResult();
        }
      }
      // right border!
      srcSize2=2*src.lastRow();
      for (x=end,i=kern.lastIdx()+x;
           x<src.rows()-kern.lastIdx();
           ++x,++i) {
        for (col=0;col<src.columns();++col) {
          for (xx=x,lresult_.reset(),f=filterLast;
               (f>=0);
               --f,++xx) {
            if (xx<src.rows())
              lresult_.accumulate(filter.at(f),src.at(xx,col));
            else
              lresult_.accumulate(filter.at(f),src.at(srcSize2-xx,col));
          }
          dest.at(i,col)=lresult_.getResult();
        }
      }
      break;
      
    case lti::Periodic:
      // left border!
      for (x=begin-1,i=kern.lastIdx()+x;
           x>=-kern.lastIdx();
           --x,--i) {
        for (col=0;col<src.columns();++col) {
          for (xx=x+filterLast,lresult_.reset(),f=0;
               (f<filter.size());
               ++f,--xx) {
            if (xx>=0)
              lresult_.accumulate(filter.at(f),src.at(xx,col));
            else
              lresult_.accumulate(filter.at(f),src.at(src.rows()+xx,col));
          }
          dest.at(i,col)=lresult_.getResult();
        }
      }
      
      // right border!
      for (x=end,i=kern.lastIdx()+x;
           x<src.rows()-kern.lastIdx();
           ++x,++i) {
        for (col=0;col<src.columns();++col) {
          for (xx=x,lresult_.reset(),f=filterLast;
               (f>=0);
               --f,++xx) {
            if (xx<src.rows())
              lresult_.accumulate(filter.at(f),src.at(xx,col));
            else
              lresult_.accumulate(filter.at(f),src.at(xx-src.rows(),col));
          }
          dest.at(i,col)=lresult_.getResult();
        }
      }
      break;
      
    case lti::Constant:
      // left border!
      for (x=begin-1,i=kern.lastIdx()+x;
           x>=-kern.lastIdx();
           --x,--i) {
        for (col=0;col<src.columns();++col) {
          for (xx=x+filterLast,lresult_.reset(),f=0;
               (f<filter.size());
               ++f,--xx) {
            if (xx>=0)
              lresult_.accumulate(filter.at(f),src.at(xx,col));
            else
              lresult_.accumulate(filter.at(f),src.at(0,col));
          }
          dest.at(i,col)=lresult_.getResult();
        }
      }
      
      // right border!
      for (x=end,i=kern.lastIdx()+x;
           x<src.rows()-kern.lastIdx();
           ++x,++i) {
        for (col=0;col<src.columns();++col) {
          for (xx=x,lresult_.reset(),f=filterLast;
               (f>=0);
               --f,++xx) {
            if (xx<src.rows())
              lresult_.accumulate(filter.at(f),src.at(xx,col));
            else
              lresult_.accumulate(filter.at(f),src.at(src.lastRow(),col));
          }
          dest.at(i,col)=lresult_.getResult();
        }
      }
      break;
    default:
      throw exception("unknown border type");
    }
    
  }


  template<class T,class A>
  void lti::convHelper1D<T,A>::applyRow(const matrix<T>& src,
                                              matrix<T>& dest,
                                        const eBoundaryType& boundaryType){

    int i;

    dest.allocate(src.rows(),src.columns());

    if (kernelSymmetry_ == lti::Default) {
      for (i=0;i<src.rows();++i) {
        applyAny(src.getRow(i),dest.getRow(i),boundaryType);
      }
    } else {
      for (i=0;i<src.rows();++i) {
        applySym(src.getRow(i),dest.getRow(i),boundaryType);
      }
    }
  }

  template<class T,class A>
  A& lti::convHelper1D<T,A>::getAccumulator() {
    return lresult_;
  }

  template<class T,class A>
  const A& lti::convHelper1D<T,A>::getAccumulator() const {
    return lresult_;
  }

  // ---------------------------------------------
  // 2D FILTERING
  // ---------------------------------------------
  template<class T,class A>
  lti::convHelper2D<T,A>::convHelper2D()
    : kernel_(0),sKernel_(0),mask_(0),tmpKernel_(0),tmpSKernel_(0) {
  }

  template<class T,class A>
  lti::convHelper2D<T,A>::~convHelper2D() {
    delete tmpKernel_;
    tmpKernel_=0;
    delete tmpSKernel_;
    tmpSKernel_=0;
  }

  template<class T,class A>
  template<typename U>
  bool lti::convHelper2D<T,A>::cast2(const container* kern,
                                     bool mirrored) {

    const kernel2D<U>* tmpCast = dynamic_cast< const kernel2D<U>* >(kern);
    if (notNull(tmpCast)) {
      tmpKernel_ = new kernel2D<T>;

      // U = from this type is started the cast process
      // T = to this type is done the cast

      if (typeInfo<U>::isFloatingPointType()) { // from floaing point ->
        
        if (typeInfo<T>::isFloatingPointType()) { // -> to floating point
          tmpKernel_->castFrom(*tmpCast);
        } else { // -> to fixed point

          // create a temporal copy of the input kernel
          kernel2D<U>* tmpFloat = new kernel2D<U>(*tmpCast);
          tmpFloat->multiply(static_cast<U>(typeInfo<T>::suggestedNorm()));
          tmpKernel_->castFrom(*tmpFloat);
          delete tmpFloat;
        }
      } else { // from fixed point ->
        tmpKernel_->castFrom(*tmpCast);
 
        if (typeInfo<T>::isFloatingPointType()) { // -> to floating point
          tmpKernel_->denormalize();
        }
      }
     
      if (mirrored) { // want to "fold" the kernel?
        tmpKernel_->mirror();
      }

      // indicate to use the casted kernel
      kernel_ = tmpKernel_;
      return true;
    }
    
    return false;
  }

  template<class T,class A>
  template<typename U>
  bool lti::convHelper2D<T,A>::castS(const container* kern,
                                     bool mirrored) {

    const separableKernel<U>* tmpCast = 
      dynamic_cast< const separableKernel<U>* >(kern);

    if (notNull(tmpCast)) {
      tmpSKernel_ = new separableKernel<T>;

      // U = from this type is started the cast process
      // T = to this type is done the cast

      if (typeInfo<U>::isFloatingPointType()) { // from floaing point ->
        
        if (typeInfo<T>::isFloatingPointType()) { // -> to floating point
          tmpSKernel_->castFrom(*tmpCast);
        } else { // -> to fixed point

          // create a temporal copy of the input kernel
          separableKernel<U>* tmpFloat = new separableKernel<U>(*tmpCast);
          tmpFloat->multiply(static_cast<U>(typeInfo<T>::suggestedNorm()));
          tmpSKernel_->castFrom(*tmpFloat);
          delete tmpFloat;
        }
      } else { // from fixed point ->
        tmpSKernel_->castFrom(*tmpCast);
 
        if (typeInfo<T>::isFloatingPointType()) { // -> to floating point
          tmpSKernel_->denormalize();
        }
      }
     
      if (mirrored) { // want to "fold" the kernel?
        tmpSKernel_->mirror();
      }

      // indicate to use the casted kernel
      sKernel_ = tmpSKernel_;
      return true;
    }
    
    return false;
  }

  template<class T,class A>
  bool lti::convHelper2D<T,A>::setKernel(const container* kern,
                                         bool mirrored) {


    // try separable
    sKernel_ = dynamic_cast<const separableKernel<T>*>(kern);
    kernel_ = 0;

    // it is not the expected type, try to cast from known possibilities
    if (isNull(sKernel_)) {

      // it may be a kernel2D... try it!
      kernel_ = dynamic_cast<const kernel2D<T>*>(kern);

      if (isNull(kernel_)) {
    
        delete tmpKernel_;
        tmpKernel_ = 0;
        
        delete tmpSKernel_;
        tmpSKernel_ = 0;
        
        // try to cast from the first possible kernel type
        bool casted = (castS<float>(kern,mirrored)  || 
                       cast2<float>(kern,mirrored)  ||
                       castS<ubyte>(kern,mirrored)  || 
                       cast2<ubyte>(kern,mirrored)  ||
                       castS<int>(kern,mirrored)    || 
                       cast2<int>(kern,mirrored)    ||
                       castS<double>(kern,mirrored) ||
                       cast2<double>(kern,mirrored));

        if (!casted) {
          // no possible cast found!  abort!
          return false;
        }

        // the condition above should check this but better be sure!
        assert(notNull(kernel_) || notNull(sKernel_));

      } else { // we have an expected 2D kernel
        if (mirrored) {
          delete tmpKernel_;
          tmpKernel_ = kernel_->clone();
          tmpKernel_->mirror();
          kernel_ = tmpKernel_;
        }
      }
    } else { // we have an expected separable kernel
      if (mirrored) {
        delete tmpSKernel_;
        tmpSKernel_ = sKernel_->clone();
        tmpSKernel_->mirror();
        sKernel_ = tmpSKernel_;
      }
    }
    return (notNull(kernel_) || notNull(sKernel_));
  }


  template<class T,class A>
  bool lti::convHelper2D<T,A>::isKernelSeparable() const {
    return notNull(sKernel_);
  }

  template<class T,class A>
  void lti::convHelper2D<T,A>::apply(const matrix<T>& src,
                                           matrix<T>& dest,
                                     const eBoundaryType& boundaryType) {

    int x,y,i,j,f,g,xx,yy,filterLastRow,filterLastCol;
    int beginRow,endRow,beginCol,endCol;
    lresult_.reset();

    // alias for kernel term
    const kernel2D<T>& kern = *kernel_;

    if ((kern.columns() < 1) ||
        (kern.rows() < 1)) {
      throw exception("unknown border type!");
    }

    // initialize member "norm"
    lresult_.setNorm(kern.getNorm());

    // need kernel as matrix
    const matrix<T>& filter = kern;
    filterLastRow = filter.lastRow();
    filterLastCol = filter.lastColumn();

    // initialize size of dest
    dest.allocate(src.rows(),src.columns());

    // middle block filter (the whole kernel can be applied here!)
    beginRow = (kern.lastRow()<0) ? -kern.lastRow() : 0;
    beginCol = (kern.lastColumn()<0) ? -kern.lastColumn() : 0;
    endRow   = (kern.lastRow()>=filter.rows()) ?
      src.rows()-kern.lastRow() :
      src.rows()-filter.rows()+1;
    endCol   = (kern.lastColumn()>=filter.columns()) ?
      src.columns()-kern.lastColumn() :
      src.columns()-filter.columns()+1;

    if ((src.rows() >= filter.rows()) &&
        (src.columns() >= filter.columns())) {
      // ----------------------------------------
      // if the kernel is smaller than the matrix


      for (y=beginRow,j=kern.lastRow()+y;y<endRow;++y,++j) {
        for (x=beginCol,i=kern.lastColumn()+x;x<endCol;++x,++i) {
          // x,y: begin in src vector
          // i,j: place to write local convolution
          // lresult_ : local result
          // xx,yy: sample on image to be checked
          for (yy=y,lresult_.reset(),g=filterLastRow;g>=0;--g,++yy) {
            for (xx=x,f=filterLastCol;f>=0;--f,++xx ) {
              lresult_.accumulate(filter.at(g,f),src.at(yy,xx));
            }
          }
          dest.at(j,i)=lresult_.getResult();
        }
      }

      // -----------------------
      //    border conditions
      // -----------------------

      switch (boundaryType) {
      case lti::NoBoundary:
        break;
      case lti::Zero: {
        // left border!
        for (y=beginRow,j=kern.lastRow()+y;y<endRow;++y,++j) {
          for (x=beginCol-1,i=kern.lastColumn()+x;
               x>=-kern.lastColumn();
               --x,--i) {
            for (yy=y,lresult_.reset(),g=filterLastRow;g>=0;--g,++yy) {
              for (xx=x+filterLastCol,f=0;
                   ((xx>=0) && (f<filter.columns()));
                   ++f,--xx) {
                lresult_.accumulate(filter.at(g,f),src.at(yy,xx));
              }
            }
            dest.at(j,i)=lresult_.getResult();
          }
        }

        // right border!
        for (y=beginRow,j=kern.lastRow()+y;y<endRow;++y,++j) {
          for (x=endCol,i=kern.lastColumn()+x;
               x<src.columns()-kern.lastColumn();
               ++x,++i) {
            for (yy=y,lresult_.reset(),g=filterLastRow;g>=0;--g,++yy) {
              for (xx=x,f=filterLastCol;
                   ((xx<src.columns()) && (f>=0));
                   --f,++xx) {
                lresult_.accumulate(filter.at(g,f),src.at(yy,xx));
              }
            }
            dest.at(j,i)=lresult_.getResult();
          }
        }

        // upper border!
        for (y=beginRow-1,j=kern.lastRow()+y;
             y>=-kern.lastRow();
             --y,--j) {
          for (x=beginCol,i=kern.lastColumn()+x;x<endCol;++x,++i) {
            for (yy=y+filterLastRow,lresult_.reset(),g=0;
                 ((yy>=0) && (g<filter.rows()));
                 ++g,--yy) {
              for (xx=x,f=filterLastCol;f>=0;--f,++xx ) {

                lresult_.accumulate(filter.at(g,f),src.at(yy,xx));
              }
            }
            dest.at(j,i)=lresult_.getResult();
          }
        }

        // bottom border!
        for (y=endRow,j=kern.lastRow()+y;
             y<src.rows()-kern.lastRow();
             ++y,++j) {
          for (x=beginCol,i=kern.lastColumn()+x;x<endCol;++x,++i) {
            for (yy=y,lresult_.reset(),g=filterLastRow;
                 ((yy<src.rows()) && (g>=0));
                 --g,++yy) {
              for (xx=x,f=filterLastCol;f>=0;--f,++xx ) {
                lresult_.accumulate(filter.at(g,f),src.at(yy,xx));
              }
            }
            dest.at(j,i)=lresult_.getResult();
          }
        }

        // upper left
        for (y=beginRow-1,j=kern.lastRow()+y;
             y>=-kern.lastRow();
             --y,--j) {
          for (x=beginCol-1,i=kern.lastColumn()+x;
               x>=-kern.lastColumn();
               --x,--i) {
            for (yy=y+filterLastRow,lresult_.reset(),g=0;
                 ((yy>=0) && (g<filter.rows()));
                 ++g,--yy) {
              for (xx=x+filterLastCol,f=0;
                   ((xx>=0) && (f<filter.columns()));
                   ++f,--xx) {
                lresult_.accumulate(filter.at(g,f),src.at(yy,xx));
              }
            }
            dest.at(j,i)=lresult_.getResult();
          }
        }

        // upper right
        for (y=beginRow-1,j=kern.lastRow()+y;
             y>=-kern.lastRow();
             --y,--j) {
          for (x=endCol,i=kern.lastColumn()+x;
               x<src.columns()-kern.lastColumn();
               ++x,++i) {

            for (yy=y+filterLastRow,lresult_.reset(),g=0;
                 ((yy>=0) && (g<filter.rows()));
                 ++g,--yy) {
              for (xx=x,f=filterLastCol;
                   ((xx<src.columns()) && (f>=0));
                   --f,++xx) {
                lresult_.accumulate(filter.at(g,f),src.at(yy,xx));
              }
            }
            dest.at(j,i)=lresult_.getResult();
          }
        }

        // bottom left
        for (y=endRow,j=kern.lastRow()+y;
             y<src.rows()-kern.lastRow();
             ++y,++j) {
          for (x=beginCol-1,i=kern.lastColumn()+x;
               x>=-kern.lastColumn();
               --x,--i) {
            for (yy=y,lresult_.reset(),g=filterLastRow;
                 ((yy<src.rows()) && (g>=0));
                 --g,++yy) {
              for (xx=x+filterLastCol,f=0;
                   ((xx>=0) && (f<filter.columns()));
                   ++f,--xx) {
                lresult_.accumulate(filter.at(g,f),src.at(yy,xx));
              }
            }
            dest.at(j,i)=lresult_.getResult();
          }
        }

        // bottom right
        for (y=endRow,j=kern.lastRow()+y;
             y<src.rows()-kern.lastRow();
             ++y,++j) {
          for (x=endCol,i=kern.lastColumn()+x;
               x<src.columns()-kern.lastColumn();
               ++x,++i) {
            for (yy=y,lresult_.reset(),g=filterLastRow;
                 ((yy<src.rows()) && (g>=0));
                 --g,++yy) {
              for (xx=x,f=filterLastCol;
                   ((xx<src.columns()) && (f>=0));
                   --f,++xx) {
                lresult_.accumulate(filter.at(g,f),src.at(yy,xx));
              }
            }
            dest.at(j,i)=lresult_.getResult();
          }
        }
      }
        break;

      case lti::Mirror: {
        // left border!
        for (y=beginRow,j=kern.lastRow()+y;y<endRow;++y,++j) {
          for (x=beginCol-1,i=kern.lastColumn()+x;
               x>=-kern.lastColumn();
               --x,--i) {
            for (yy=y,lresult_.reset(),g=filterLastRow;g>=0;--g,++yy) {
              for (xx=x+filterLastCol,f=0;
                   (f<filter.columns());
                   ++f,--xx) {
                if (xx>=0)
                  lresult_.accumulate(filter.at(g,f),src.at(yy,xx));
                else
                  lresult_.accumulate(filter.at(g,f),src.at(yy,-xx));
              }
            }
            dest.at(j,i)=lresult_.getResult();
          }
        }

        // right border!
        int srcCol2=2*src.lastColumn();
        for (y=beginRow,j=kern.lastRow()+y;y<endRow;++y,++j) {
          for (x=endCol,i=kern.lastColumn()+x;
               x<src.columns()-kern.lastColumn();
               ++x,++i) {
            for (yy=y,lresult_.reset(),g=filterLastRow;g>=0;--g,++yy) {
              for (xx=x,f=filterLastCol;
                   (f>=0);
                   --f,++xx) {
                if (xx<src.columns())
                  lresult_.accumulate(filter.at(g,f),src.at(yy,xx));
                else
                  lresult_.accumulate(filter.at(g,f),src.at(yy,srcCol2-xx));
              }
            }
            dest.at(j,i)=lresult_.getResult();
          }
        }

        // upper border!
        for (y=beginRow-1,j=kern.lastRow()+y;
             y>=-kern.lastRow();
             --y,--j) {
          for (x=beginCol,i=kern.lastColumn()+x;x<endCol;++x,++i) {
            for (yy=y+filterLastRow,lresult_.reset(),g=0;
                 (g<filter.rows());
                 ++g,--yy) {
              for (xx=x,f=filterLastCol;f>=0;--f,++xx ) {
                if (yy>=0)
                  lresult_.accumulate(filter.at(g,f),src.at(yy,xx));
                else
                  lresult_.accumulate(filter.at(g,f),src.at(-yy,xx));
              }
            }
            dest.at(j,i)=lresult_.getResult();
          }
        }

        // bottom border!
        int srcRow2=2*src.lastRow();
        for (y=endRow,j=kern.lastRow()+y;
             y<src.rows()-kern.lastRow();
             ++y,++j) {
          for (x=beginCol,i=kern.lastColumn()+x;x<endCol;++x,++i) {
            for (yy=y,lresult_.reset(),g=filterLastRow;
                 (g>=0);
                 --g,++yy) {
              for (xx=x,f=filterLastCol;f>=0;--f,++xx ) {
                if (yy<src.rows())
                  lresult_.accumulate(filter.at(g,f),src.at(yy,xx));
                else
                  lresult_.accumulate(filter.at(g,f),src.at(srcRow2-yy,xx));
              }
            }
            dest.at(j,i)=lresult_.getResult();
          }
        }

        // upper left
        for (y=beginRow-1,j=kern.lastRow()+y;
             y>=-kern.lastRow();
             --y,--j) {
          for (x=beginCol-1,i=kern.lastColumn()+x;
               x>=-kern.lastColumn();
               --x,--i) {
            for (yy=y+filterLastRow,lresult_.reset(),g=0;
                 (g<filter.rows());
                 ++g,--yy) {
              for (xx=x+filterLastCol,f=0;
                   (f<filter.columns());
                   ++f,--xx) {
                lresult_.accumulate(filter.at(g,f),
                                    src.at((yy>=0)?yy:-yy,
                                           (xx>=0)?xx:-xx));
              }
            }
            dest.at(j,i)=lresult_.getResult();
          }
        }

        // upper right
        for (y=beginRow-1,j=kern.lastRow()+y;
             y>=-kern.lastRow();
             --y,--j) {
          for (x=endCol,i=kern.lastColumn()+x;
               x<src.columns()-kern.lastColumn();
               ++x,++i) {

            for (yy=y+filterLastRow,lresult_.reset(),g=0;
                 (g<filter.rows());
                 ++g,--yy) {
              for (xx=x,f=filterLastCol;
                   (f>=0);
                   --f,++xx) {
                lresult_.accumulate(filter.at(g,f),
                                    src.at((yy>=0) ? yy : -yy,
                                           (xx<src.columns()) ? xx : srcCol2-xx));
              }
            }
            dest.at(j,i)=lresult_.getResult();
          }
        }

        // bottom left
        for (y=endRow,j=kern.lastRow()+y;
             y<src.rows()-kern.lastRow();
             ++y,++j) {
          for (x=beginCol-1,i=kern.lastColumn()+x;
               x>=-kern.lastColumn();
               --x,--i) {
            for (yy=y,lresult_.reset(),g=filterLastRow;
                 (g>=0);
                 --g,++yy) {
              for (xx=x+filterLastCol,f=0;
                   (f<filter.columns());
                   ++f,--xx) {
                lresult_.accumulate(filter.at(g,f),
                                    src.at((yy<src.rows()) ? yy : srcRow2-yy,
                                           (xx>=0) ? xx : -xx));
              }
            }
            dest.at(j,i)=lresult_.getResult();
          }
        }

        // bottom right
        for (y=endRow,j=kern.lastRow()+y;
             y<src.rows()-kern.lastRow();
             ++y,++j) {
          for (x=endCol,i=kern.lastColumn()+x;
               x<src.columns()-kern.lastColumn();
               ++x,++i) {
            for (yy=y,lresult_.reset(),g=filterLastRow;
                 (g>=0);
                 --g,++yy) {
              for (xx=x,f=filterLastCol;
                   (f>=0);
                   --f,++xx) {
                lresult_.accumulate(filter.at(g,f),
                                    src.at((yy<src.rows()) ? yy : srcRow2-yy,
                                           (xx<src.columns()) ? xx : srcCol2-xx));
              }
            }
            dest.at(j,i)=lresult_.getResult();
          }
        }
      }
        break;

      case lti::Periodic: {
        // left border!
        for (y=beginRow,j=kern.lastRow()+y;y<endRow;++y,++j) {
          for (x=beginCol-1,i=kern.lastColumn()+x;
               x>=-kern.lastColumn();
               --x,--i) {
            for (yy=y,lresult_.reset(),g=filterLastRow;g>=0;--g,++yy) {
              for (xx=x+filterLastCol,f=0;
                   (f<filter.columns());
                   ++f,--xx) {
                if (xx>=0)
                  lresult_.accumulate(filter.at(g,f),src.at(yy,xx));
                else
                  lresult_.accumulate(filter.at(g,f),src.at(yy,src.columns()+xx));
              }
            }
            dest.at(j,i)=lresult_.getResult();
          }
        }

        // right border!
        for (y=beginRow,j=kern.lastRow()+y;y<endRow;++y,++j) {
          for (x=endCol,i=kern.lastColumn()+x;
               x<src.columns()-kern.lastColumn();
               ++x,++i) {
            for (yy=y,lresult_.reset(),g=filterLastRow;g>=0;--g,++yy) {
              for (xx=x,f=filterLastCol;
                   (f>=0);
                   --f,++xx) {
                if (xx<src.columns())
                  lresult_.accumulate(filter.at(g,f),src.at(yy,xx));
                else
                  lresult_.accumulate(filter.at(g,f),src.at(yy,xx-src.columns()));
              }
            }
            dest.at(j,i)=lresult_.getResult();
          }
        }

        // upper border!
        for (y=beginRow-1,j=kern.lastRow()+y;
             y>=-kern.lastRow();
             --y,--j) {
          for (x=beginCol,i=kern.lastColumn()+x;x<endCol;++x,++i) {
            for (yy=y+filterLastRow,lresult_.reset(),g=0;
                 (g<filter.rows());
                 ++g,--yy) {
              for (xx=x,f=filterLastCol;f>=0;--f,++xx ) {
                if (yy>=0)
                  lresult_.accumulate(filter.at(g,f),src.at(yy,xx));
                else
                  lresult_.accumulate(filter.at(g,f),src.at(yy+src.rows(),xx));
              }
            }
            dest.at(j,i)=lresult_.getResult();
          }
        }

        // bottom border!
        for (y=endRow,j=kern.lastRow()+y;
             y<src.rows()-kern.lastRow();
             ++y,++j) {
          for (x=beginCol,i=kern.lastColumn()+x;x<endCol;++x,++i) {
            for (yy=y,lresult_.reset(),g=filterLastRow;
                 (g>=0);
                 --g,++yy) {
              for (xx=x,f=filterLastCol;f>=0;--f,++xx ) {
                if (yy<src.rows())
                  lresult_.accumulate(filter.at(g,f),src.at(yy,xx));
                else
                  lresult_.accumulate(filter.at(g,f),src.at(yy-src.rows(),xx));
              }
            }
            dest.at(j,i)=lresult_.getResult();
          }
        }

        // upper left
        for (y=beginRow-1,j=kern.lastRow()+y;
             y>=-kern.lastRow();
             --y,--j) {
          for (x=beginCol-1,i=kern.lastColumn()+x;
               x>=-kern.lastColumn();
               --x,--i) {
            for (yy=y+filterLastRow,lresult_.reset(),g=0;
                 (g<filter.rows());
                 ++g,--yy) {
              for (xx=x+filterLastCol,f=0;
                   (f<filter.columns());
                   ++f,--xx) {
                lresult_.accumulate(filter.at(g,f),
                                    src.at((yy>=0) ? yy : yy+src.rows(),
                                           (xx>=0) ? xx : xx+src.columns()));
              }
            }
            dest.at(j,i)=lresult_.getResult();
          }
        }

        // upper right
        for (y=beginRow-1,j=kern.lastRow()+y;
             y>=-kern.lastRow();
             --y,--j) {
          for (x=endCol,i=kern.lastColumn()+x;
               x<src.columns()-kern.lastColumn();
               ++x,++i) {

            for (yy=y+filterLastRow,lresult_.reset(),g=0;
                 (g<filter.rows());
                 ++g,--yy) {
              for (xx=x,f=filterLastCol;
                   (f>=0);
                   --f,++xx) {
                lresult_.accumulate(filter.at(g,f),
                                    src.at((yy>=0)?yy:yy+src.rows(),
                                           (xx<src.columns())?xx:xx-src.columns()));
              }
            }
            dest.at(j,i)=lresult_.getResult();
          }
        }

        // bottom left
        for (y=endRow,j=kern.lastRow()+y;
             y<src.rows()-kern.lastRow();
             ++y,++j) {
          for (x=beginCol-1,i=kern.lastColumn()+x;
               x>=-kern.lastColumn();
               --x,--i) {
            for (yy=y,lresult_.reset(),g=filterLastRow;
                 (g>=0);
                 --g,++yy) {
              for (xx=x+filterLastCol,f=0;
                   (f<filter.columns());
                   ++f,--xx) {
                lresult_.accumulate(filter.at(g,f),
                                    src.at((yy<src.rows())?yy:yy-src.rows(),
                                           (xx>=0)?xx:xx+src.columns()));
              }
            }
            dest.at(j,i)=lresult_.getResult();
          }
        }

        // bottom right
        for (y=endRow,j=kern.lastRow()+y;
             y<src.rows()-kern.lastRow();
             ++y,++j) {
          for (x=endCol,i=kern.lastColumn()+x;
               x<src.columns()-kern.lastColumn();
               ++x,++i) {
            for (yy=y,lresult_.reset(),g=filterLastRow;
                 (g>=0);
                 --g,++yy) {
              for (xx=x,f=filterLastCol;
                   (f>=0);
                   --f,++xx) {
                lresult_.accumulate(filter.at(g,f),
                                    src.at((yy<src.rows())?yy:yy-src.rows(),
                                           (xx<src.columns())?xx:xx-src.columns()));
              }
            }
            dest.at(j,i)=lresult_.getResult();
          }
        }
      }
        break;

      case lti::Constant: {
        // left border!
        for (y=beginRow,j=kern.lastRow()+y;y<endRow;++y,++j) {
          for (x=beginCol-1,i=kern.lastColumn()+x;
               x>=-kern.lastColumn();
               --x,--i) {
            for (yy=y,lresult_.reset(),g=filterLastRow;g>=0;--g,++yy) {
              for (xx=x+filterLastCol,f=0;
                   (f<filter.columns());
                   ++f,--xx) {
                if (xx>=0)
                  lresult_.accumulate(filter.at(g,f),src.at(yy,xx));
                else
                  lresult_.accumulate(filter.at(g,f),src.at(yy,0));
              }
            }
            dest.at(j,i)=lresult_.getResult();
          }
        }

        // right border!
        for (y=beginRow,j=kern.lastRow()+y;y<endRow;++y,++j) {
          for (x=endCol,i=kern.lastColumn()+x;
               x<src.columns()-kern.lastColumn();
               ++x,++i) {
            for (yy=y,lresult_.reset(),g=filterLastRow;g>=0;--g,++yy) {
              for (xx=x,f=filterLastCol;
                   (f>=0);
                   --f,++xx) {
                if (xx<src.columns())
                  lresult_.accumulate(filter.at(g,f),src.at(yy,xx));
                else
                  lresult_.accumulate(filter.at(g,f),src.at(yy,src.lastColumn()));
              }
            }
            dest.at(j,i)=lresult_.getResult();
          }
        }

        // upper border!
        for (y=beginRow-1,j=kern.lastRow()+y;
             y>=-kern.lastRow();
             --y,--j) {
          for (x=beginCol,i=kern.lastColumn()+x;x<endCol;++x,++i) {
            for (yy=y+filterLastRow,lresult_.reset(),g=0;
                 (g<filter.rows());
                 ++g,--yy) {
              for (xx=x,f=filterLastCol;f>=0;--f,++xx ) {
                if (yy>=0)
                  lresult_.accumulate(filter.at(g,f),src.at(yy,xx));
                else
                  lresult_.accumulate(filter.at(g,f),src.at(0,xx));
              }
            }
            dest.at(j,i)=lresult_.getResult();
          }
        }

        // bottom border!
        for (y=endRow,j=kern.lastRow()+y;
             y<src.rows()-kern.lastRow();
             ++y,++j) {
          for (x=beginCol,i=kern.lastColumn()+x;x<endCol;++x,++i) {
            for (yy=y,lresult_.reset(),g=filterLastRow;
                 (g>=0);
                 --g,++yy) {
              for (xx=x,f=filterLastCol;f>=0;--f,++xx ) {
                if (yy<src.rows())
                  lresult_.accumulate(filter.at(g,f),src.at(yy,xx));
                else
                  lresult_.accumulate(filter.at(g,f),src.at(src.lastRow(),xx));
              }
            }
            dest.at(j,i)=lresult_.getResult();
          }
        }

        // upper left
        for (y=beginRow-1,j=kern.lastRow()+y;
             y>=-kern.lastRow();
             --y,--j) {
          for (x=beginCol-1,i=kern.lastColumn()+x;
               x>=-kern.lastColumn();
               --x,--i) {
            for (yy=y+filterLastRow,lresult_.reset(),g=0;
                 (g<filter.rows());
                 ++g,--yy) {
              for (xx=x+filterLastCol,f=0;
                   (f<filter.columns());
                   ++f,--xx) {
                lresult_.accumulate(filter.at(g,f),
                                    src.at((yy>=0) ? yy : 0,
                                           (xx>=0) ? xx : 0));
              }
            }
            dest.at(j,i)=lresult_.getResult();
          }
        }

        // upper right
        for (y=beginRow-1,j=kern.lastRow()+y;
             y>=-kern.lastRow();
             --y,--j) {
          for (x=endCol,i=kern.lastColumn()+x;
               x<src.columns()-kern.lastColumn();
               ++x,++i) {

            for (yy=y+filterLastRow,lresult_.reset(),g=0;
                 (g<filter.rows());
                 ++g,--yy) {
              for (xx=x,f=filterLastCol;
                   (f>=0);
                   --f,++xx) {
                lresult_.accumulate(filter.at(g,f),
                                    src.at((yy>=0)?yy:0,
                                           (xx<src.columns())?xx:src.lastColumn()));
              }
            }
            dest.at(j,i)=lresult_.getResult();
          }
        }

        // bottom left
        for (y=endRow,j=kern.lastRow()+y;
             y<src.rows()-kern.lastRow();
             ++y,++j) {
          for (x=beginCol-1,i=kern.lastColumn()+x;
               x>=-kern.lastColumn();
               --x,--i) {
            for (yy=y,lresult_.reset(),g=filterLastRow;
                 (g>=0);
                 --g,++yy) {
              for (xx=x+filterLastCol,f=0;
                   (f<filter.columns());
                   ++f,--xx) {
                lresult_.accumulate(filter.at(g,f),
                                    src.at((yy<src.rows()) ? yy : src.lastRow(),
                                           (xx>=0) ? xx : 0));
              }
            }
            dest.at(j,i)=lresult_.getResult();
          }
        }

        // bottom right
        for (y=endRow,j=kern.lastRow()+y;
             y<src.rows()-kern.lastRow();
             ++y,++j) {
          for (x=endCol,i=kern.lastColumn()+x;
               x<src.columns()-kern.lastColumn();
               ++x,++i) {
            for (yy=y,lresult_.reset(),g=filterLastRow;
                 (g>=0);
                 --g,++yy) {
              for (xx=x,f=filterLastCol;
                   (f>=0);
                   --f,++xx) {
                lresult_.accumulate(filter.at(g,f),
                                    src.at((yy<src.rows()) ? yy : src.lastRow(),
                                           (xx<src.columns()) ? xx : src.lastColumn()));
              }
            }
            dest.at(j,i)=lresult_.getResult();
          }
        }
      }
        break;

      default:
        throw exception("unknown border type!");
      }
    } else {
      // ---------------------------------------
      // if the kernel is bigger than the vector

      switch (boundaryType) {
      case lti::NoBoundary:
        break;
      case lti::Zero:
        for (y=0;y<src.rows();++y) {
          for (x=0;x<src.columns();++x) {
            for (g = kern.lastRow(), yy = y, lresult_.reset();
                 g >=kern.firstRow();--g,++yy) {
              for (f = kern.lastColumn(), xx = x;
                   f >= kern.firstColumn(); --f,++xx) {
                if ((xx>=0) && (xx<src.columns()) &&
                    (yy>=0) && (yy<src.rows())) {
                  lresult_.accumulate(kern.at(g,f),src.at(yy,xx));
                }
              }
            }
            dest.at(y,x) = lresult_.getResult();
          }
        }
        break;

      case lti::Mirror:
        for (y=0;y<src.rows();++y) {
          for (x=0;x<src.columns();++x) {

            for (g = kern.lastRow(), yy = y-kern.lastRow(), lresult_.reset();
                 g >=kern.firstRow();--g,++yy) {
              for (f = kern.lastColumn(), xx = x-kern.lastColumn();
                   f >= kern.firstColumn(); --f,++xx) {

                if ((xx>=0) && (xx<src.columns()) &&
                    (yy>=0) && (yy<src.rows())) {
                  lresult_.accumulate(kern.at(g,f),src.at(yy,xx));
                } else {
                  int32 px,py,xp,yp;

                  px = xx/src.columns();
                  py = yy/src.rows();

                  xp = xx % src.columns();
                  yp = yy % src.rows();

                  if (xp>=0) {
                    xp = ((px & 01) == 0) ? xp : src.lastColumn()-xp;
                  } else {
                    xp = ((px & 01) == 0) ? -xp-1 : src.columns()+xp;
                  }

                  if (yp>=0) {
                    yp = ((py & 01) == 0) ? yp : src.lastColumn()-yp;
                  } else {
                    yp = ((py & 01) == 0) ? -yp-1 : src.columns()+yp;
                  }

                  lresult_.accumulate(kern.at(g,f),src.at(yp,xp));
                }
              }
            }
            dest.at(y,x) = lresult_.getResult();
          }
        }
        break;

      case lti::Periodic:

        for (y=0;y<src.rows();++y) {
          for (x=0;x<src.columns();++x) {
            for (g = kern.lastRow(), yy = y, lresult_.reset();
                 g >=kern.firstRow();--g,++yy) {
              for (f = kern.lastColumn(), xx = x;
                   f >= kern.firstColumn(); --f,++xx) {
                lresult_.accumulate(kern.at(g,f),
                                    src.at(yy%src.rows(),xx%src.columns()));
              }
            }
            dest.at(y,x) = lresult_.getResult();
          }
        }

        break;

      case lti::Constant:
        for (y=0;y<src.rows();++y) {
          for (x=0;x<src.columns();++x) {

            for (g = kern.lastRow(), yy = y, lresult_.reset();
                 g >=kern.firstRow();--g,++yy) {
              for (f = kern.lastColumn(), xx = x;
                   f >= kern.firstColumn(); --f,++xx) {

                int xp,yp;
                yp = (yy<0) ? 0 : (yy>=src.rows()) ? src.lastRow() : yy;
                xp = (xx<0) ? 0 : (xx>=src.rows()) ? src.lastRow() : xx;

                lresult_.accumulate(kern.at(g,f),src.at(yp,xp));
              }
            }
            dest.at(y,x) = lresult_.getResult();
          }
        }

        break;
      default:
        throw exception("unknown border type!");
      }
    }
  }

  template<class T,class A>
  void lti::convHelper2D<T,A>::applySep(const matrix<T>& src,
                                        matrix<T>& dest,
                                        const eBoundaryType& boundaryType)
  {
    // alias for kernel term
    const separableKernel<T>& kern = *sKernel_;

    // initialize member "norm"
    lti::convHelper1D<T,A> conv1D;

    // create main buffer
    matrix<T> buffer;

    // check sizes
    if (src.size() != dest.size()) {
      dest.allocate(src.size());
    }

    const int NoP = kern.getNumberOfPairs();
    int i;
    matrix<T> tmpB;
    
    for (i=0;i<NoP;++i) {
      // rows filter
      if ((kern.getRowFilter(i).size() == 1) &&
          (kern.getRowFilter(i).getOffset() == 0)) {
        buffer.multiply(src,kern.getRowFilter(i).at(0));
      } else {
        conv1D.setKernel(&kern.getRowFilter(i));
        conv1D.applyRow(src,buffer,boundaryType);
      }

      // columns filter
      if ((kern.getColFilter(i).size() == 1) &&
          (kern.getColFilter(i).getOffset() == 0)) {
        tmpB.multiply(buffer,kern.getColFilter(i).at(0));
      } else {
        conv1D.setKernel(&kern.getColFilter(i));
        conv1D.applyCol(buffer,tmpB,boundaryType);
      }
      
      if (i==0) {
        // the first time just transfer the result to dest
        tmpB.detach(dest);
      } else {
        // the rest separable parts should be accumulated in the result!
        dest.add(tmpB);
      }
    }  
  }

  template<class T,class A>
  A& lti::convHelper2D<T,A>::getAccumulator() {
    return lresult_;
  }

  template<class T,class A>
  const A& lti::convHelper2D<T,A>::getAccumulator() const {
    return lresult_;
  }

  template<class T,class A>
  void lti::convHelper2D<T,A>::applyMask(const matrix<T>& src,
                                         matrix<T>& dest,
                                         const eBoundaryType& boundaryType) {
    // no correlation possible without a matching mask
    if (isNull(mask_) || (mask_->size() != kernel_->size())) {
      throw invalidParametersException();
    }

    int x,y,i,j,xx,yy,f,g,filterLastRow,filterLastCol;
    int beginRow,endRow,beginCol,endCol;
    lresult_.reset();

    // alias for kernel term
    const kernel2D<T>& kern = *kernel_;

    if ((kern.columns() < 1) ||
        (kern.rows() < 1)) {
      throw exception("unknown border type!");
    }

    // initialize member "norm"
    lresult_.setNorm(kern.getNorm());

    // need kernel as matrix
    const matrix<T>& filter = kern;
    filterLastRow = filter.lastRow();
    filterLastCol = filter.lastColumn();

    // initialize size of dest
    dest.assign(src.rows(),src.columns(),T(0));

    // middle block filter (the whole kernel can be applied here!)
    beginRow = (kern.lastRow()<0) ? -kern.lastRow() : 0;
    beginCol = (kern.lastColumn()<0) ? -kern.lastColumn() : 0;
    endRow   = (kern.lastRow()>=filter.rows()) ?
      src.rows()-kern.lastRow() :
      src.rows()-filter.rows()+1;
    endCol   = (kern.lastColumn()>=filter.columns()) ?
      src.columns()-kern.lastColumn() :
      src.columns()-filter.columns()+1;

    pointList<int>::const_iterator p;

    if ((src.rows() >= filter.rows()) &&
        (src.columns() >= filter.columns())) {
      // ----------------------------------------
      // if the kernel is smaller than the matrix

      lti::areaPoints shape;
      shape.extractFromMask(*mask_);

      for (y=beginRow,j=kern.lastRow()+y;y<endRow;++y,++j) {
        for (x=beginCol,i=kern.lastColumn()+x;x<endCol;++x,++i) {
          // x,y: begin in src vector
          // i,j: place to write local convolution
          // lresult_ : local result
          // xx,yy: sample on image to be checked

          lresult_.reset();
          for (p = shape.begin(); p != shape.end(); p++) {
            lresult_.accumulate(filter.at(*p),
                                src.at(y+filterLastRow - p->y,
                                       x+filterLastCol - p->x));
          }
          dest.at(j,i)=lresult_.getResult();
        }
      }

      // -----------------------
      //    border conditions
      // -----------------------

      switch (boundaryType) {
      case lti::NoBoundary:
        break;
      case lti::Zero: {
        // left border!
        for (y=beginRow,j=kern.lastRow()+y;y<endRow;++y,++j) {
          for (x=beginCol-1,i=kern.lastColumn()+x;
               x>=-kern.lastColumn();
               --x,--i) {
            for (yy=y,lresult_.reset(),g=filterLastRow;g>=0;--g,++yy) {
              for (xx=x+filterLastCol,f=0;
                   ((xx>=0) && (f<filter.columns()));
                   ++f,--xx) {
                if (mask_->at(g,f) != 0) {
                  lresult_.accumulate(filter.at(g,f),src.at(yy,xx));
                }
              }
            }
            dest.at(j,i)=lresult_.getResult();
          }
        }

        // right border!
        for (y=beginRow,j=kern.lastRow()+y;y<endRow;++y,++j) {
          for (x=endCol,i=kern.lastColumn()+x;
               x<src.columns()-kern.lastColumn();
               ++x,++i) {
            for (yy=y,lresult_.reset(),g=filterLastRow;g>=0;--g,++yy) {
              for (xx=x,f=filterLastCol;
                   ((xx<src.columns()) && (f>=0));
                   --f,++xx) {
                if (mask_->at(g,f) != 0) {
                  lresult_.accumulate(filter.at(g,f),src.at(yy,xx));
                }
              }
            }
            dest.at(j,i)=lresult_.getResult();
          }
        }

        // upper border!
        for (y=beginRow-1,j=kern.lastRow()+y;
             y>=-kern.lastRow();
             --y,--j) {
          for (x=beginCol,i=kern.lastColumn()+x;x<endCol;++x,++i) {
            for (yy=y+filterLastRow,lresult_.reset(),g=0;
                 ((yy>=0) && (g<filter.rows()));
                 ++g,--yy) {
              for (xx=x,f=filterLastCol;f>=0;--f,++xx ) {
                if (mask_->at(g,f) != 0) {
                  lresult_.accumulate(filter.at(g,f),src.at(yy,xx));
                }
              }
            }
            dest.at(j,i)=lresult_.getResult();
          }
        }

        // bottom border!
        for (y=endRow,j=kern.lastRow()+y;
             y<src.rows()-kern.lastRow();
             ++y,++j) {
          for (x=beginCol,i=kern.lastColumn()+x;x<endCol;++x,++i) {
            for (yy=y,lresult_.reset(),g=filterLastRow;
                 ((yy<src.rows()) && (g>=0));
                 --g,++yy) {
              for (xx=x,f=filterLastCol;f>=0;--f,++xx ) {
                if (mask_->at(g,f) != 0) {
                  lresult_.accumulate(filter.at(g,f),src.at(yy,xx));
                }
              }
            }
            dest.at(j,i)=lresult_.getResult();
          }
        }

        // upper left
        for (y=beginRow-1,j=kern.lastRow()+y;
             y>=-kern.lastRow();
             --y,--j) {
          for (x=beginCol-1,i=kern.lastColumn()+x;
               x>=-kern.lastColumn();
               --x,--i) {
            for (yy=y+filterLastRow,lresult_.reset(),g=0;
                 ((yy>=0) && (g<filter.rows()));
                 ++g,--yy) {
              for (xx=x+filterLastCol,f=0;
                   ((xx>=0) && (f<filter.columns()));
                   ++f,--xx) {
                if (mask_->at(g,f) != 0) {
                  lresult_.accumulate(filter.at(g,f),src.at(yy,xx));
                }
              }
            }
            dest.at(j,i)=lresult_.getResult();
          }
        }

        // upper right
        for (y=beginRow-1,j=kern.lastRow()+y;
             y>=-kern.lastRow();
             --y,--j) {
          for (x=endCol,i=kern.lastColumn()+x;
               x<src.columns()-kern.lastColumn();
               ++x,++i) {

            for (yy=y+filterLastRow,lresult_.reset(),g=0;
                 ((yy>=0) && (g<filter.rows()));
                 ++g,--yy) {
              for (xx=x,f=filterLastCol;
                   ((xx<src.columns()) && (f>=0));
                   --f,++xx) {
                if (mask_->at(g,f) != 0) {
                  lresult_.accumulate(filter.at(g,f),src.at(yy,xx));
                }
              }
            }
            dest.at(j,i)=lresult_.getResult();
          }
        }

        // bottom left
        for (y=endRow,j=kern.lastRow()+y;
             y<src.rows()-kern.lastRow();
             ++y,++j) {
          for (x=beginCol-1,i=kern.lastColumn()+x;
               x>=-kern.lastColumn();
               --x,--i) {
            for (yy=y,lresult_.reset(),g=filterLastRow;
                 ((yy<src.rows()) && (g>=0));
                 --g,++yy) {
              for (xx=x+filterLastCol,f=0;
                   ((xx>=0) && (f<filter.columns()));
                   ++f,--xx) {
                if (mask_->at(g,f) != 0) {
                  lresult_.accumulate(filter.at(g,f),src.at(yy,xx));
                }
              }
            }
            dest.at(j,i)=lresult_.getResult();
          }
        }

        // bottom right
        for (y=endRow,j=kern.lastRow()+y;
             y<src.rows()-kern.lastRow();
             ++y,++j) {
          for (x=endCol,i=kern.lastColumn()+x;
               x<src.columns()-kern.lastColumn();
               ++x,++i) {
            for (yy=y,lresult_.reset(),g=filterLastRow;
                 ((yy<src.rows()) && (g>=0));
                 --g,++yy) {
              for (xx=x,f=filterLastCol;
                   ((xx<src.columns()) && (f>=0));
                   --f,++xx) {
                if (mask_->at(g,f) != 0) {
                  lresult_.accumulate(filter.at(g,f),src.at(yy,xx));
                }
              }
            }
            dest.at(j,i)=lresult_.getResult();
          }
        }
      }
        break;

      case lti::Mirror: {
        // left border!
        for (y=beginRow,j=kern.lastRow()+y;y<endRow;++y,++j) {
          for (x=beginCol-1,i=kern.lastColumn()+x;
               x>=-kern.lastColumn();
               --x,--i) {
            for (yy=y,lresult_.reset(),g=filterLastRow;g>=0;--g,++yy) {
              for (xx=x+filterLastCol,f=0;
                   (f<filter.columns());
                   ++f,--xx) {
                if (mask_->at(g,f) != 0) {
                  if (xx>=0)
                    lresult_.accumulate(filter.at(g,f),src.at(yy,xx));
                  else
                    lresult_.accumulate(filter.at(g,f),src.at(yy,-xx));
                }
              }
            }
            dest.at(j,i)=lresult_.getResult();
          }
        }

        // right border!
        int srcCol2=2*src.lastColumn();
        for (y=beginRow,j=kern.lastRow()+y;y<endRow;++y,++j) {
          for (x=endCol,i=kern.lastColumn()+x;
               x<src.columns()-kern.lastColumn();
               ++x,++i) {
            for (yy=y,lresult_.reset(),g=filterLastRow;g>=0;--g,++yy) {
              for (xx=x,f=filterLastCol;
                   (f>=0);
                   --f,++xx) {
                if (mask_->at(g,f) != 0) {
                  if (xx<src.columns())
                    lresult_.accumulate(filter.at(g,f),src.at(yy,xx));
                  else
                    lresult_.accumulate(filter.at(g,f),src.at(yy,srcCol2-xx));
                }
              }
            }
            dest.at(j,i)=lresult_.getResult();
          }
        }

        // upper border!
        for (y=beginRow-1,j=kern.lastRow()+y;
             y>=-kern.lastRow();
             --y,--j) {
          for (x=beginCol,i=kern.lastColumn()+x;x<endCol;++x,++i) {
            for (yy=y+filterLastRow,lresult_.reset(),g=0;
                 (g<filter.rows());
                 ++g,--yy) {
              for (xx=x,f=filterLastCol;f>=0;--f,++xx ) {
                if (mask_->at(g,f) != 0) {
                  if (yy>=0)
                    lresult_.accumulate(filter.at(g,f),src.at(yy,xx));
                  else
                    lresult_.accumulate(filter.at(g,f),src.at(-yy,xx));
                }
              }
            }
            dest.at(j,i)=lresult_.getResult();
          }
        }

        // bottom border!
        int srcRow2=2*src.lastRow();
        for (y=endRow,j=kern.lastRow()+y;
             y<src.rows()-kern.lastRow();
             ++y,++j) {
          for (x=beginCol,i=kern.lastColumn()+x;x<endCol;++x,++i) {
            for (yy=y,lresult_.reset(),g=filterLastRow;
                 (g>=0);
                 --g,++yy) {
              for (xx=x,f=filterLastCol;f>=0;--f,++xx ) {
                if (mask_->at(g,f) != 0) {
                  if (yy<src.rows())
                    lresult_.accumulate(filter.at(g,f),src.at(yy,xx));
                  else
                    lresult_.accumulate(filter.at(g,f),src.at(srcRow2-yy,xx));
                }
              }
            }
            dest.at(j,i)=lresult_.getResult();
          }
        }

        // upper left
        for (y=beginRow-1,j=kern.lastRow()+y;
             y>=-kern.lastRow();
             --y,--j) {
          for (x=beginCol-1,i=kern.lastColumn()+x;
               x>=-kern.lastColumn();
               --x,--i) {
            for (yy=y+filterLastRow,lresult_.reset(),g=0;
                 (g<filter.rows());
                 ++g,--yy) {
              for (xx=x+filterLastCol,f=0;
                   (f<filter.columns());
                   ++f,--xx) {
                if (mask_->at(g,f) != 0) {
                  lresult_.accumulate(filter.at(g,f),
                                      src.at((yy>=0)?yy:-yy,
                                             (xx>=0)?xx:-xx));
                }
              }
            }
            dest.at(j,i)=lresult_.getResult();
          }
        }

        // upper right
        for (y=beginRow-1,j=kern.lastRow()+y;
             y>=-kern.lastRow();
             --y,--j) {
          for (x=endCol,i=kern.lastColumn()+x;
               x<src.columns()-kern.lastColumn();
               ++x,++i) {

            for (yy=y+filterLastRow,lresult_.reset(),g=0;
                 (g<filter.rows());
                 ++g,--yy) {
              for (xx=x,f=filterLastCol;
                   (f>=0);
                   --f,++xx) {
                if (mask_->at(g,f) != 0) {
                  lresult_.accumulate(filter.at(g,f),
                                      src.at((yy>=0) ? yy : -yy,
                                             (xx<src.columns()) ? xx : srcCol2-xx));
                }
              }
            }
            dest.at(j,i)=lresult_.getResult();
          }
        }

        // bottom left
        for (y=endRow,j=kern.lastRow()+y;
             y<src.rows()-kern.lastRow();
             ++y,++j) {
          for (x=beginCol-1,i=kern.lastColumn()+x;
               x>=-kern.lastColumn();
               --x,--i) {
            for (yy=y,lresult_.reset(),g=filterLastRow;
                 (g>=0);
                 --g,++yy) {
              for (xx=x+filterLastCol,f=0;
                   (f<filter.columns());
                   ++f,--xx) {
                if (mask_->at(g,f) != 0) {
                  lresult_.accumulate(filter.at(g,f),
                                      src.at((yy<src.rows()) ? yy : srcRow2-yy,
                                             (xx>=0) ? xx : -xx));
                }
              }
            }
            dest.at(j,i)=lresult_.getResult();
          }
        }

        // bottom right
        for (y=endRow,j=kern.lastRow()+y;
             y<src.rows()-kern.lastRow();
             ++y,++j) {
          for (x=endCol,i=kern.lastColumn()+x;
               x<src.columns()-kern.lastColumn();
               ++x,++i) {
            for (yy=y,lresult_.reset(),g=filterLastRow;
                 (g>=0);
                 --g,++yy) {
              for (xx=x,f=filterLastCol;
                   (f>=0);
                   --f,++xx) {
                if (mask_->at(g,f) != 0) {
                  lresult_.accumulate(filter.at(g,f),
                                      src.at((yy<src.rows()) ? yy : srcRow2-yy,
                                             (xx<src.columns()) ? xx : srcCol2-xx));
                }
              }
            }
            dest.at(j,i)=lresult_.getResult();
          }
        }
      }
        break;

      case lti::Periodic: {
        // left border!
        for (y=beginRow,j=kern.lastRow()+y;y<endRow;++y,++j) {
          for (x=beginCol-1,i=kern.lastColumn()+x;
               x>=-kern.lastColumn();
               --x,--i) {
            for (yy=y,lresult_.reset(),g=filterLastRow;g>=0;--g,++yy) {
              for (xx=x+filterLastCol,f=0;
                   (f<filter.columns());
                   ++f,--xx) {
                if (mask_->at(g,f) != 0) {
                  if (xx>=0)
                    lresult_.accumulate(filter.at(g,f),src.at(yy,xx));
                  else
                    lresult_.accumulate(filter.at(g,f),
                                        src.at(yy,src.columns()+xx));
                }
              }
            }
            dest.at(j,i)=lresult_.getResult();
          }
        }

        // right border!
        for (y=beginRow,j=kern.lastRow()+y;y<endRow;++y,++j) {
          for (x=endCol,i=kern.lastColumn()+x;
               x<src.columns()-kern.lastColumn();
               ++x,++i) {
            for (yy=y,lresult_.reset(),g=filterLastRow;g>=0;--g,++yy) {
              for (xx=x,f=filterLastCol;
                   (f>=0);
                   --f,++xx) {
                if (mask_->at(g,f) != 0) {
                  if (xx<src.columns())
                    lresult_.accumulate(filter.at(g,f),src.at(yy,xx));
                  else
                    lresult_.accumulate(filter.at(g,f),
                                        src.at(yy,xx-src.columns()));
                }
              }
            }
            dest.at(j,i)=lresult_.getResult();
          }
        }

        // upper border!
        for (y=beginRow-1,j=kern.lastRow()+y;
             y>=-kern.lastRow();
             --y,--j) {
          for (x=beginCol,i=kern.lastColumn()+x;x<endCol;++x,++i) {
            for (yy=y+filterLastRow,lresult_.reset(),g=0;
                 (g<filter.rows());
                 ++g,--yy) {
              for (xx=x,f=filterLastCol;f>=0;--f,++xx ) {
                if (mask_->at(g,f) != 0) {
                  if (yy>=0)
                    lresult_.accumulate(filter.at(g,f),src.at(yy,xx));
                  else
                    lresult_.accumulate(filter.at(g,f),
                                        src.at(yy+src.rows(),xx));
                }
              }
            }
            dest.at(j,i)=lresult_.getResult();
          }
        }

        // bottom border!
        for (y=endRow,j=kern.lastRow()+y;
             y<src.rows()-kern.lastRow();
             ++y,++j) {
          for (x=beginCol,i=kern.lastColumn()+x;x<endCol;++x,++i) {
            for (yy=y,lresult_.reset(),g=filterLastRow;
                 (g>=0);
                 --g,++yy) {
              for (xx=x,f=filterLastCol;f>=0;--f,++xx ) {
                if (mask_->at(g,f) != 0) {
                  if (yy<src.rows())
                    lresult_.accumulate(filter.at(g,f),src.at(yy,xx));
                  else
                    lresult_.accumulate(filter.at(g,f),
                                        src.at(yy-src.rows(),xx));
                }
              }
            }
            dest.at(j,i)=lresult_.getResult();
          }
        }

        // upper left
        for (y=beginRow-1,j=kern.lastRow()+y;
             y>=-kern.lastRow();
             --y,--j) {
          for (x=beginCol-1,i=kern.lastColumn()+x;
               x>=-kern.lastColumn();
               --x,--i) {
            for (yy=y+filterLastRow,lresult_.reset(),g=0;
                 (g<filter.rows());
                 ++g,--yy) {
              for (xx=x+filterLastCol,f=0;
                   (f<filter.columns());
                   ++f,--xx) {
                if (mask_->at(g,f) != 0) {
                  lresult_.accumulate(filter.at(g,f),
                                      src.at((yy>=0) ? yy : yy+src.rows(),
                                             (xx>=0) ? xx : xx+src.columns()));
                }
              }
            }
            dest.at(j,i)=lresult_.getResult();
          }
        }

        // upper right
        for (y=beginRow-1,j=kern.lastRow()+y;
             y>=-kern.lastRow();
             --y,--j) {
          for (x=endCol,i=kern.lastColumn()+x;
               x<src.columns()-kern.lastColumn();
               ++x,++i) {

            for (yy=y+filterLastRow,lresult_.reset(),g=0;
                 (g<filter.rows());
                 ++g,--yy) {
              for (xx=x,f=filterLastCol;
                   (f>=0);
                   --f,++xx) {
                if (mask_->at(g,f) != 0) {
                  lresult_.accumulate(filter.at(g,f),
                                      src.at((yy>=0)?yy:yy+src.rows(),
                                             (xx<src.columns())? 
                                             xx : xx-src.columns()));
                }
              }
            }
            dest.at(j,i)=lresult_.getResult();
          }
        }

        // bottom left
        for (y=endRow,j=kern.lastRow()+y;
             y<src.rows()-kern.lastRow();
             ++y,++j) {
          for (x=beginCol-1,i=kern.lastColumn()+x;
               x>=-kern.lastColumn();
               --x,--i) {
            for (yy=y,lresult_.reset(),g=filterLastRow;
                 (g>=0);
                 --g,++yy) {
              for (xx=x+filterLastCol,f=0;
                   (f<filter.columns());
                   ++f,--xx) {
                if (mask_->at(g,f) != 0) {
                  lresult_.accumulate(filter.at(g,f),
                                      src.at((yy<src.rows())?yy:yy-src.rows(),
                                             (xx>=0)?xx:xx+src.columns()));
                }
              }
            }
            dest.at(j,i)=lresult_.getResult();
          }
        }

        // bottom right
        for (y=endRow,j=kern.lastRow()+y;
             y<src.rows()-kern.lastRow();
             ++y,++j) {
          for (x=endCol,i=kern.lastColumn()+x;
               x<src.columns()-kern.lastColumn();
               ++x,++i) {
            for (yy=y,lresult_.reset(),g=filterLastRow;
                 (g>=0);
                 --g,++yy) {
              for (xx=x,f=filterLastCol;
                   (f>=0);
                   --f,++xx) {
                if (mask_->at(g,f) != 0) {
                  lresult_.accumulate(filter.at(g,f),
                                      src.at((yy<src.rows())?yy:yy-src.rows(),
                                             (xx<src.columns())?
                                             xx:xx-src.columns()));
                }
              }
            }
            dest.at(j,i)=lresult_.getResult();
          }
        }
      }
        break;

      case lti::Constant: {
        // left border!
        for (y=beginRow,j=kern.lastRow()+y;y<endRow;++y,++j) {
          for (x=beginCol-1,i=kern.lastColumn()+x;
               x>=-kern.lastColumn();
               --x,--i) {
            for (yy=y,lresult_.reset(),g=filterLastRow;g>=0;--g,++yy) {
              for (xx=x+filterLastCol,f=0;
                   (f<filter.columns());
                   ++f,--xx) {
                if (mask_->at(g,f) != 0) {
                  if (xx>=0)
                    lresult_.accumulate(filter.at(g,f),src.at(yy,xx));
                  else
                    lresult_.accumulate(filter.at(g,f),src.at(yy,0));
                }
              }
            }
            dest.at(j,i)=lresult_.getResult();
          }
        }

        // right border!
        for (y=beginRow,j=kern.lastRow()+y;y<endRow;++y,++j) {
          for (x=endCol,i=kern.lastColumn()+x;
               x<src.columns()-kern.lastColumn();
               ++x,++i) {
            for (yy=y,lresult_.reset(),g=filterLastRow;g>=0;--g,++yy) {
              for (xx=x,f=filterLastCol;
                   (f>=0);
                   --f,++xx) {
                if (mask_->at(g,f) != 0) {
                  if (xx<src.columns())
                    lresult_.accumulate(filter.at(g,f),src.at(yy,xx));
                  else
                    lresult_.accumulate(filter.at(g,f),
                                        src.at(yy,src.lastColumn()));
                }
              }
            }
            dest.at(j,i)=lresult_.getResult();
          }
        }

        // upper border!
        for (y=beginRow-1,j=kern.lastRow()+y;
             y>=-kern.lastRow();
             --y,--j) {
          for (x=beginCol,i=kern.lastColumn()+x;x<endCol;++x,++i) {
            for (yy=y+filterLastRow,lresult_.reset(),g=0;
                 (g<filter.rows());
                 ++g,--yy) {
              for (xx=x,f=filterLastCol;f>=0;--f,++xx ) {
                if (mask_->at(g,f) != 0) {
                  if (yy>=0)
                    lresult_.accumulate(filter.at(g,f),src.at(yy,xx));
                  else
                    lresult_.accumulate(filter.at(g,f),src.at(0,xx));
                }
              }
            }
            dest.at(j,i)=lresult_.getResult();
          }
        }

        // bottom border!
        for (y=endRow,j=kern.lastRow()+y;
             y<src.rows()-kern.lastRow();
             ++y,++j) {
          for (x=beginCol,i=kern.lastColumn()+x;x<endCol;++x,++i) {
            for (yy=y,lresult_.reset(),g=filterLastRow;
                 (g>=0);
                 --g,++yy) {
              for (xx=x,f=filterLastCol;f>=0;--f,++xx ) {
                if (mask_->at(g,f) != 0) {
                  if (yy<src.rows())
                    lresult_.accumulate(filter.at(g,f),src.at(yy,xx));
                  else
                    lresult_.accumulate(filter.at(g,f),
                                        src.at(src.lastRow(),xx));
                }
              }
            }
            dest.at(j,i)=lresult_.getResult();
          }
        }

        // upper left
        for (y=beginRow-1,j=kern.lastRow()+y;
             y>=-kern.lastRow();
             --y,--j) {
          for (x=beginCol-1,i=kern.lastColumn()+x;
               x>=-kern.lastColumn();
               --x,--i) {
            for (yy=y+filterLastRow,lresult_.reset(),g=0;
                 (g<filter.rows());
                 ++g,--yy) {
              for (xx=x+filterLastCol,f=0;
                   (f<filter.columns());
                   ++f,--xx) {
                if (mask_->at(g,f) != 0) {
                  lresult_.accumulate(filter.at(g,f),
                                      src.at((yy>=0) ? yy : 0,
                                             (xx>=0) ? xx : 0));
                }
              }
            }
            dest.at(j,i)=lresult_.getResult();
          }
        }

        // upper right
        for (y=beginRow-1,j=kern.lastRow()+y;
             y>=-kern.lastRow();
             --y,--j) {
          for (x=endCol,i=kern.lastColumn()+x;
               x<src.columns()-kern.lastColumn();
               ++x,++i) {

            for (yy=y+filterLastRow,lresult_.reset(),g=0;
                 (g<filter.rows());
                 ++g,--yy) {
              for (xx=x,f=filterLastCol;
                   (f>=0);
                   --f,++xx) {
                if (mask_->at(g,f) != 0) {
                  lresult_.accumulate(filter.at(g,f),
                                      src.at((yy>=0)?yy:0,
                                             (xx<src.columns())?
                                             xx:src.lastColumn()));
                }
              }
            }
            dest.at(j,i)=lresult_.getResult();
          }
        }

        // bottom left
        for (y=endRow,j=kern.lastRow()+y;
             y<src.rows()-kern.lastRow();
             ++y,++j) {
          for (x=beginCol-1,i=kern.lastColumn()+x;
               x>=-kern.lastColumn();
               --x,--i) {
            for (yy=y,lresult_.reset(),g=filterLastRow;
                 (g>=0);
                 --g,++yy) {
              for (xx=x+filterLastCol,f=0;
                   (f<filter.columns());
                   ++f,--xx) {
                if (mask_->at(g,f) != 0) {
                  lresult_.accumulate(filter.at(g,f),
                                      src.at((yy<src.rows()) ? 
                                             yy : src.lastRow(),
                                             (xx>=0) ? xx : 0));
                }
              }
            }
            dest.at(j,i)=lresult_.getResult();
          }
        }

        // bottom right
        for (y=endRow,j=kern.lastRow()+y;
             y<src.rows()-kern.lastRow();
             ++y,++j) {
          for (x=endCol,i=kern.lastColumn()+x;
               x<src.columns()-kern.lastColumn();
               ++x,++i) {
            for (yy=y,lresult_.reset(),g=filterLastRow;
                 (g>=0);
                 --g,++yy) {
              for (xx=x,f=filterLastCol;
                   (f>=0);
                   --f,++xx) {
                if (mask_->at(g,f) != 0) {
                  lresult_.accumulate(filter.at(g,f),
                                      src.at((yy<src.rows()) ? 
                                             yy : src.lastRow(),
                                             (xx<src.columns()) ?
                                             xx : src.lastColumn()));
                }
              }
            }
            dest.at(j,i)=lresult_.getResult();
          }
        }
      }
        break;

      default:
        throw exception("unknown border type!");
      }
    } else {
      // ---------------------------------------
      // if the kernel is bigger than the matrix

      int kernFirstRow = kern.firstRow();
      int kernFirstCol = kern.firstColumn();

      switch (boundaryType) {
      case lti::NoBoundary:
        break;
      case lti::Zero:
        for (y=0;y<src.rows();++y) {
          for (x=0;x<src.columns();++x) {
            for (g = kern.lastRow(), yy = y+g, lresult_.reset();
                 g >=kern.firstRow();--g,--yy) {
              for (f = kern.lastColumn(), xx = x+f;
                   f >= kern.firstColumn(); --f,--xx) {
                if ((xx>=0) && (xx<src.columns()) &&
                    (yy>=0) && (yy<src.rows()) &&
                    (mask_->at(g - kernFirstRow, f - kernFirstCol) != 0)) {
                  lresult_.accumulate(kern.at(g,f),src.at(yy,xx));
                }
              }
            }
            dest.at(y,x) = lresult_.getResult();
          }
        }
        break;

      case lti::Mirror:
        for (y=0;y<src.rows();++y) {
          for (x=0;x<src.columns();++x) {

            for (g = kern.lastRow(), yy = y+g, lresult_.reset();
                 g >=kern.firstRow();--g,--yy) {
              for (f = kern.lastColumn(), xx = x+f;
                   f >= kern.firstColumn(); --f,--xx) {
                if (mask_->at(g - kernFirstRow, f - kernFirstCol) != 0) {
                  continue;
                }
                if ((xx>=0) && (xx<src.columns()) &&
                    (yy>=0) && (yy<src.rows())) {
                  lresult_.accumulate(kern.at(g,f),src.at(yy,xx));
                } else {
                  int px,py,xp,yp;

                  px = xx/src.columns();
                  py = yy/src.rows();

                  xp = xx % src.columns();
                  yp = yy % src.rows();

                  if (xx < 0) {
                    px = -px + 1;
                    if (xp < 0) {
                      xp += src.columns();
                    }
                  }

                  if (yy < 0) {
                    py = -py + 1;
                    if (yp < 0) {
                      yp += src.rows();
                    }
                  }

                  xp = ((px & 01) == 0) ? xp % src.columns() :
                    src.lastColumn()-(xp%src.columns());
                  yp = ((py & 01) == 0) ? yp % src.rows() :
                    src.lastRow()-(yp%src.rows());
                  lresult_.accumulate(kern.at(g,f),src.at(yp,xp));
                }
              }
            }
            dest.at(y,x) = lresult_.getResult();
          }
        }
        break;

      case lti::Periodic:

        for (y=0;y<src.rows();++y) {
          for (x=0;x<src.columns();++x) {
            for (g = kern.lastRow(), yy = y+g, lresult_.reset();
                 g >=kern.firstRow();--g,--yy) {
              for (f = kern.lastColumn(), xx = x+f;
                   f >= kern.firstColumn(); --f,--xx) {
                if (mask_->at(g - kernFirstRow, f - kernFirstCol) != 0) {
                  lresult_.accumulate(kern.at(g,f),
                                      src.at(yy%src.rows(),xx%src.columns()));
                }
              }
            }
            dest.at(y,x) = lresult_.getResult();
          }
        }

        break;

      case lti::Constant:
        for (y=0;y<src.rows();++y) {
          for (x=0;x<src.columns();++x) {

            for (g = kern.lastRow(), yy = y+g, lresult_.reset();
                 g >=kern.firstRow();--g,--yy) {
              for (f = kern.lastColumn(), xx = x+f;
                   f >= kern.firstColumn(); --f,--xx) {
                if (mask_->at(g - kernFirstRow, f - kernFirstCol) != 0) {
                  int xp,yp;
                  yp = (yy<0) ? 0 : (yy>=src.rows()) ? src.lastRow() : yy;
                  xp = (xx<0) ? 0 : (xx>=src.rows()) ? src.lastRow() : xx;

                  lresult_.accumulate(kern.at(g,f),src.at(yp,xp));
                }
              }
            }
            dest.at(y,x) = lresult_.getResult();
          }
        }

        break;
      default:
        throw exception("unknown border type!");
      }
    }
  }

  template<class T,class A>
  void lti::convHelper2D<T,A>::setMask(const matrix<ubyte>& mask) {
    this->mask_ = &mask;
  }

  template<class T,class A>
  const matrix<ubyte>* lti::convHelper2D<T,A>::getMask() const {
    return mask_;
  }

}
