/*
 * Copyright (C) 2007
 * Pablo Alvarado
 *
 * This file is part of the LTI-Computer Vision Library 2 (LTI-Lib-2)
 *
 * The LTI-Lib-2 is free software; you can redistribute it and/or
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
 * \file   ltiFastHessianDetection.cpp
 *         Contains the class fastHessianDetection,
 *         which has still to be documented by Pablo Alvarado.
 * 
 * \author Pablo Alvarado
 * \date   23.10.2007
 *
 * revisions ..: $Id: ltiFastHessianDetection_template.h,v 1.10 2007-12-26 04:49:23 alvarado Exp $
 */

#include "ltiIntegralImage.h"
#include "ltiTypeInfo.h"
#include "ltiRound.h"
#include "ltiArctanLUT.h"
#include "ltiInterval.h"
#include "ltiConstants.h"
#include <vector>

namespace lti {


  template<typename T> bool 
  fastHessianDetection::computeDeterminant(const int kernelSize,
                                           const matrix<T>& intImg,
                                           const integralImage& integrator,
                                           const int sampleStep,
                                           matrix<float>& det) const {

    det.allocate(intImg.size());
//     det.assign(intImg.size(),std::numeric_limits<float>::max()); // debug
    
    // According to the paper the 9x9 kernels are build as follow
    // Dxx and Dyy (just Dyy shown) and Dxx

    //  0  0  1  1  1  1  1  0  0        0  0  0  0  0  0  0  0  0 
    //  0  0  1  1  1  1  1  0  0        0  1  1  1  0 -1 -1 -1  0
    //  0  0  1  1  1  1  1  0  0        0  1  1  1  0 -1 -1 -1  0
    //  0  0 -2 -2 -2 -2 -2  0  0        0  1  1  1  0 -1 -1 -1  0
    //  0  0 -2 -2 -2 -2 -2  0  0        0  0  0  0  0  0  0  0  0 
    //  0  0 -2 -2 -2 -2 -2  0  0        0 -1 -1 -1  0  1  1  1  0
    //  0  0  1  1  1  1  1  0  0        0 -1 -1 -1  0  1  1  1  0
    //  0  0  1  1  1  1  1  0  0        0 -1 -1 -1  0  1  1  1  0
    //  0  0  1  1  1  1  1  0  0        0  0  0  0  0  0  0  0  0

    const parameters& param = getParameters();
    
    // why two sqr? 1. Normalization of the kernel, 2. the det is squared.
    const float norm = 
      (std::numeric_limits<T>::is_integer) ?
      1.0f/static_cast<float>(pow(float(kernelSize),
                                  param.normPower))
      :
      sqr(255.0f)/static_cast<float>(pow(float(kernelSize),
                                         param.normPower));

    const int hSide = (kernelSize-1)/2;

    //const int dxx1Height  = 2*kernelSize/9;   // half width of 1's box
    //const int dxx2Width   = kernelSize/9; // half width of -2's box
    
    const int dxx1Height  = (hSide+1)/2;        // half width of 1's box
    const int dxx2Width   = hSide-kernelSize/3; // half width of -2's box

    const int dxyFr = kernelSize-2*kernelSize/3;
    const int dxyEx = hSide-(dxyFr-dxyFr/3)/2;
    const int dxyIn = dxyEx-kernelSize/3+1;

    int y,x;
    float dxx,dxy,dyy;

    // compute frobenius norm ratios
    // ratio of gaussian kernels is always this number
    static const float frobRatioGauss = sqr(0.577658f);
    const float frobRatio = frobRatioGauss*((dxx1Height*2+1)*4.5f/kernelSize);

    _lti_debug3("Frobenius ratio for kernel " << kernelSize << " = " <<
                frobRatio << std::endl);

    // To efficienty compute the filters, we have to avoid computing the
    // boundary conditions for maximal number of pixels.  So, we have first the
    // main loop, where the internalSum() methods can be used

    // -------------
    // The main loop
    // -------------

    int val = (hSide+1);
    int tmp = val%sampleStep;
    const int startPos 
      = (tmp != 0) ? val + (sampleStep-tmp) : val;

    const int lastMainLoopRow = det.rows()    - hSide;
    const int lastMainLoopCol = det.columns() - hSide;

    for (y=startPos;y<lastMainLoopRow;y+=sampleStep) {
      for (x=startPos;x<lastMainLoopCol;x+=sampleStep) {

  // Define a macro to have this code just once

#  define _LTI_INTERNAL_COMPUTE_DETERMINANT_BLOCK(SUM)               \
        dxx =                                                        \
          static_cast<float>(integrator.SUM(intImg,                  \
                                            x-hSide,y-dxx1Height,    \
                                            x+hSide,y+dxx1Height));  \
        dxx-=                                                        \
          3.0f*static_cast<float>(integrator.SUM(intImg,             \
                                                 x-dxx2Width,        \
                                                 y-dxx1Height,       \
                                                 x+dxx2Width,        \
                                                 y+dxx1Height));     \
                                                                     \
        dyy =                                                        \
          static_cast<float>(integrator.SUM(intImg,                  \
                                            x-dxx1Height,y-hSide,    \
                                            x+dxx1Height,y+hSide));  \
        dyy-=                                                        \
          3.0f*static_cast<float>(integrator.SUM(intImg,             \
                                                 x-dxx1Height,       \
                                                 y-dxx2Width,        \
                                                 x+dxx1Height,       \
                                                 y+dxx2Width));      \
                                                                     \
        dxy =                                                        \
          static_cast<float>(integrator.SUM(intImg,                  \
                                            x-dxyEx,y-dxyEx,         \
                                            x-dxyIn,y-dxyIn)) +      \
          static_cast<float>(integrator.SUM(intImg,                  \
                                            x+dxyIn,y+dxyIn,         \
                                            x+dxyEx,y+dxyEx)) -      \
          static_cast<float>(integrator.SUM(intImg,                  \
                                            x-dxyEx,y+dxyIn,         \
                                            x-dxyIn,y+dxyEx)) -      \
          static_cast<float>(integrator.SUM(intImg,                  \
                                            x+dxyIn,y-dxyEx,         \
                                            x+dxyEx,y-dxyIn));       \
                                                                     \
        det.at(y,x) = (dxx*dyy-frobRatio*dxy*dxy)*norm

  // END OF MACRO DEFINITION

        _LTI_INTERNAL_COMPUTE_DETERMINANT_BLOCK(internalSum);
      }
    }

    // ------------------
    // The Boundary Loops
    // ------------------

    if (param.boundaryType == NoBoundary) {
      return true;
    }

    // upper part
    const int yTop = min(startPos,det.rows());
    for (y=0;y<yTop;y+=sampleStep) {
      for (x=0;x<det.columns();x+=sampleStep) {
        _LTI_INTERNAL_COMPUTE_DETERMINANT_BLOCK(sum);
      }
    }
    
    val = max(yTop,lastMainLoopRow);
    tmp = val%sampleStep;
    const int startBottom = (tmp != 0) ? val + (sampleStep-tmp) : val;

    // bottom part
    for (y=startBottom;y<det.rows();y+=sampleStep) {
      for (x=0;x<det.columns();x+=sampleStep) {
        _LTI_INTERNAL_COMPUTE_DETERMINANT_BLOCK(sum);
      }
    }

    // sides
    const int xLeft = min(startPos,det.columns());

    val = max(xLeft,lastMainLoopCol);
    tmp = val%sampleStep;
    const int xRight = (tmp != 0) ? val + (sampleStep-tmp) : val;

    for (y=startPos;y<lastMainLoopRow;y+=sampleStep) {
      // the left side
      for (x=0;x<xLeft;x+=sampleStep) {
        _LTI_INTERNAL_COMPUTE_DETERMINANT_BLOCK(sum);
      }

      // the right side
      for (x=xRight;x<det.columns();x+=sampleStep) {
        _LTI_INTERNAL_COMPUTE_DETERMINANT_BLOCK(sum);
      }
    }

#undef _LTI_INTERNAL_COMPUTE_DETERMINANT_BLOCK

    return true;
  }

  
  /*
   * Compute the "pseudo-pyramid".
   *
   * The template type T is usually int32 or float as types of integral images.
   *
   * @param chnl, the original input channel
   */
  template<typename T> bool fastHessianDetection::
  computeLevels(const matrix<T>& chnl,
                matrix<typename typeInfo<T>::accumulation_type>& intImg,
                std::vector<fmatrix>& levels,
                ivector& kernelSizes,
                ivector& sampleSteps) const {

    const parameters& param = getParameters();
    
    // find out which kernel sizes have to be used
    kernelSizes.allocate(param.numberOfLevels);

    if (param.levelSelectionMethod == Blocks) {
      int kSize(kernelSize_);
      int kStep(kernelStep_);    
      int i(0);
      while (i<kernelSizes.size()) {
        kernelSizes.at(i) = kSize;
        ++i;
        kSize+=kStep;
        if ((i%param.levelGroupSize) == 0) {
          kStep *= 2;
        }
      }
    } else {
      // levelSelectionMethod == Exponential
      // use double because of the geometric series
      const double alpha = pow(2.0,1.0/double(param.levelGroupSize));
      kernelSizes.at(0)=kernelSize_;
      double kSize(static_cast<double>(kernelSize_)*alpha);
      int theSize;
      for (int i=1;i<kernelSizes.size();++i) {
        
        do {
          theSize = iround(kSize/3.0)*3;  // this is the closest number
                                          // divisible by 3
          // the size has to be odd and divisible by three
          if ( (theSize % 2) == 0) {
            // even 
            theSize += 3; // this is the next odd number divisible by three
          }
          kSize *= alpha; // next number
        } while (theSize == kernelSizes.at(i-1));

        kernelSizes.at(i)=theSize;
      }
    }

    // force the levels array to have the proper number of levels with
    // empty matrices
    levels.assign(kernelSizes.size(),fmatrix());

    if (chnl.empty()) {
      return true;
    }

    static const float base2log = log(2.0f);
    const int baseKSize = kernelSizes.at(0);
    // subsampling step computation
    if (param.subsampleLevels) {
      // compute the sampling step for each level
      sampleSteps.allocate(param.numberOfLevels);
      for (int i=0;i<sampleSteps.size();++i) {
        sampleSteps.at(i) = param.initialSamplingStep * 
          max(1,1 << iround(log(kernelSizes.at(i)/baseKSize)/base2log));
      }
    } else {
      sampleSteps.assign(param.numberOfLevels,param.initialSamplingStep);
    }

    // compute the integral image
    integralImage integrator(param.boundaryType);    
    integrator.apply(chnl,intImg);

    // initialize the levels
    for (int i=0;i<kernelSizes.size();++i) {
      computeDeterminant(kernelSizes.at(i),
                         intImg,
                         integrator,
                         (i<1)?param.initialSamplingStep:sampleSteps.at(i-1),
                         levels.at(i));
    }

    return true;
  }

  // internal class used to accumulate in the angular windows and
  // to compute the result
  class fastHessianDetection::orientationAccumulator {
  private:
    // number of windows
    const int numWnds_;

    // accumulated component in x direction
    fvector oWndX_;
      
    // accumulated component in y direction
    fvector oWndY_;
      
    // window width
    const float wndWidth_;
      
    // factor to transform angle (in radians) to the window index
    const float afactor_;

    // half of angular slice but normalized to index values
    const float wndDelta_;

    // prepare the orientation windows
    arctanLUT atanLUT_;

  public:
    // constructor
    orientationAccumulator(const int numWnds,
                           const float wndWidth) 
      : numWnds_(numWnds),
        oWndX_(numWnds),oWndY_(numWnds),wndWidth_(wndWidth),
        afactor_(static_cast<float>(numWnds_)/constants<float>::twoPi()),
        wndDelta_((afactor_*wndWidth_)/2.0001f) {
    }
      
    // destructor
    ~orientationAccumulator() {}

    // reset
    inline void reset() {
      oWndX_.fill(0.0f);
      oWndY_.fill(0.0f);
    }

    // accumulate
    inline void acc(const float dx,const float dy) {
      const float angle = atanLUT_(dy,dx);
        
      const float ia = angle*afactor_;
      const int from = iround(ia-wndDelta_);
      const int to   = iround(ia+wndDelta_);

      for (int j=from;j<=to;++j) {
          
        const int idx = ( (j+numWnds_)%numWnds_ );
          
        oWndX_.at(idx)+=dx; 
        oWndY_.at(idx)+=dy;
      }

    }

    inline void cartToPolar(const float x,const float y,
                            float& mag,float& angle) const {
      mag = x*x + y*y;
      angle = atanLUT_(y,x);
    }

    // result
    inline float result() const {
      // now select the orientation
      // first assign just the one at the zero rads window
      float maxMag,angle;
      cartToPolar(oWndX_.at(0),oWndY_.at(0),maxMag,angle);

      for (int i=1;i<numWnds_;++i) {
        const float mag = sqr(oWndX_.at(i)) + sqr(oWndY_.at(i));
        if (mag > maxMag) {
          maxMag = mag;
          angle = atanLUT_(oWndY_.at(i),oWndX_.at(i));
        }
      }
      return angle;
    }
  };


  template<typename T>
  bool fastHessianDetection::computeOrientations(const matrix<T>& intImg,
                                                 list<location>& locs) const {

    const parameters& param = getParameters();
    if (!param.computeOrientation) {
      return true;
    }

    integralImage integrator(param.boundaryType);    

    
    orientationAccumulator acc(orientationNumberOfWindows_,
                               orientationWindowWidth_);
     
#if defined(_LTI_DEBUG) && (_LTI_DEBUG > 1)
//     static matrix<T> canvas;
//     // reconstruct the image
//     canvas.allocate(intImg.size());
//     for (int y=1;y<canvas.rows();++y) {
//       for (int x=1;x<canvas.columns();++x) {
//         canvas.at(y,x)=integrator.internalSum(intImg,x,y,x,y);
//       }
//     }
//     static viewer2D view("computeOrientations::debug");
#endif

    const int fromY = circLUT_.firstIdx();
    const int toY = circLUT_.lastIdx();
     
    // process each location in the list
    list<location>::iterator it,eit;
    for (it=locs.begin(),eit=locs.end();it!=eit;++it) {
      location& loc = (*it);
      // get the scale
      const float s = loc.radius;

      // clear accumulators for this location
      acc.reset();

      // wavelet size
      const int tmp = iround(s*param.orientationWaveletSizeFactor);
      const int wls = (even(tmp)) ? (tmp+1) : tmp;
      const int wlsh = wls/2;

      // step size
      const float step = s*param.orientationSamplingStepFactor;

      // fast computation window
      const float fcBorder = toY*step + 2.0f + wlsh; // 2 ensures we are in
      const frectangle fcWnd(fcBorder,fcBorder,
                             intImg.lastColumn()-fcBorder,
                             intImg.lastRow()-fcBorder);

      // check if we can avoid costly border comparisons
      if (fcWnd.contains(loc.position)) {
        // we can compute the locations with the fast integralImage methods

        // for all samples within the circular neighborhood compute the
        // haar wavelet coefficient.
        // Assumption: it makes sense to center the wavelet around each
        // sample instead of the standard form begining at the point, since the
        // wavelet is here used as gradient replacement.
        float yy = fromY*step + loc.position.y;
        for (int y=fromY;y<=toY;++y,yy+=step) {
          const int pos = circLUT_.at(y);
          float xx = -pos*step;
          for (int x=-pos;x<=pos;++x,xx+=step) {
            // real image position of the sample
            const ipoint p(iround(xx + loc.position.x),
                           iround(yy));
            const float weight = gaussWeights_.at(y,x);
            // compute the wavelet response at the sample
            
//             _lti_if_debug2(canvas.at(p)=static_cast<T>(255*weight));

            // the horizontal response
            const float wx =
              weight*(integrator.internalSum(intImg,
                                             p.x,p.y-wlsh,p.x+wlsh,p.y+wlsh) -
                      integrator.internalSum(intImg,
                                             p.x-wlsh,p.y-wlsh,p.x,p.y+wlsh));
            
            // the vertical response
            const float wy =
              weight*(integrator.internalSum(intImg,
                                             p.x-wlsh,p.y,p.x+wlsh,p.y+wlsh) -
                      integrator.internalSum(intImg,
                                             p.x-wlsh,p.y-wlsh,p.x+wlsh,p.y));
            
            // sort the vector into the corresponding orientation window(s)
            acc.acc(wx,wy);
          }
        }

      } else {
        // the locations is to near the border, so go with caution

        
        // for all samples within the circular neighborhood compute the
        // haar wavelet coefficient.
        // Assumption: it makes sense to center the wavelet around each
        // sample instead of the standard form begining at the point, since the
        // wavelet is here used as gradient replacement.
        float yy = fromY*step + loc.position.y;
        for (int y=fromY;y<=toY;++y,yy+=step) {
          const int pos = circLUT_.at(y);
          float xx = -pos*step;
          for (int x=-pos;x<=pos;++x,xx+=step) {
            // real image position of the sample
            const ipoint p(iround(xx + loc.position.x),
                           iround(yy));
            const float weight = gaussWeights_.at(y,x);
            // compute the wavelet response at the sample
            
#if defined(_LTI_DEBUG) && (_LTI_DEBUG > 1)
//             if ((static_cast<uint32>(p.x) <
//                  static_cast<uint32>(canvas.columns())) &&
//                 (static_cast<uint32>(p.y) <
//                  static_cast<uint32>(canvas.rows()))) {            
//               canvas.at(p)=static_cast<T>(255*weight);
//             }
#endif
            
            // the horizontal response
            const float wx =
              weight*(integrator.sum(intImg,
                                     p.x,p.y-wlsh,p.x+wlsh,p.y+wlsh) -
                      integrator.sum(intImg,
                                     p.x-wlsh,p.y-wlsh,p.x,p.y+wlsh));
            
            // the vertical response
            const float wy =
              weight*(integrator.sum(intImg,
                                     p.x-wlsh,p.y,p.x+wlsh,p.y+wlsh) -
                      integrator.sum(intImg,
                                     p.x-wlsh,p.y-wlsh,p.x+wlsh,p.y));
            
            // sort the vector into the corresponding orientation window(s)
            acc.acc(wx,wy);
          }
        }
      }

      // now select the orientation
      loc.angle = acc.result();
    }
     
//     _lti_if_debug2(view.show(canvas));
    
    return true;
  }

}
