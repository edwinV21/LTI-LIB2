/*
 * Copyright (C) 1998-2006
 * Lehrstuhl fuer Technische Informatik, RWTH-Aachen, Germany
 * Copyright (C) 2009, 2010
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

/*
 * \file   ltiViewer1DPainter_template.h
 *         Contains the class viewer1DPainter.
 * \author Pablo Alvarado
 * \date   05.07.2005
 *
 * revisions ..: $Id: ltiViewer1DPainter_template.h,v 1.2 2010-01-15 05:01:05 alvarado Exp $
 */

#include "ltiMath.h"
#include "ltiRound.h"
#include "ltiDraw.h"

namespace lti {

  // -------------------------------------------------------------------
  // Statistics cache management
  // -------------------------------------------------------------------

  // -----------------
  // Cache Manager
  // -----------------

  template<class T>
  bool viewer1DPainter::cacheManager::checkCacheMiss(const T* data,
                                                     const int& size) const {
    
    if ((dataPtr_  != static_cast<const void*>(data)) ||
        (dataSize_ != size)) {
      return true; // total cache miss! (data changed!)
    }

    return false;
  }


  template<class V>
  bool viewer1DPainter::cacheManager::updateSignature(const V& data) {
    bool changes = false;
    
    if ((static_cast<const void*>(&data[0]) != dataPtr_) ||
        (dataSize_ != data.size())) {
      // Cache miss
      // invalidate all blocks
      invalidate();
      
      dataPtr_  = &data[0];
      dataSize_ = data.size();
      changes = true;
    }
    
    return changes;
  }

  template <class V>
  bool viewer1DPainter::cacheManager::update(const V& vct) {

    updateSignature(vct);

    bool changes = false;
    if (!blocks.minMaxOk_) {
      typename V::value_type minVal,maxVal;
      findExtremes(vct,minVal,maxVal,blocks.mean_);
      blocks.min_ = static_cast<double>(minVal);
      blocks.max_ = static_cast<double>(maxVal);
      blocks.minMaxOk_ = true;
      changes = true;
    }
    
    return changes;
  }

  template<class V>
  void viewer1DPainter::cacheManager::
  findExtremes(const V& vct,
               typename V::value_type& theMin,
               typename V::value_type& theMax,
               double& theMean) const {

    // use a generic valid way, since std::vectors have not findExtrema!
    typename V::const_iterator it=vct.begin();
    const typename V::const_iterator eit=vct.end();

    if (it == eit) {
      theMin=theMax=0;
      theMean = 0.0;
      return;
    }

    theMin=*it;
    theMax=*it;
    theMean=0.0;
    ++it;
    while (it!=eit) {
      minmax(*it,theMin,theMax);
      theMean+=*it;
      ++it;
    }
    theMean/=vct.size();
  }


  // -------------------------------------------------------------------
  // The apply-methods!
  // -------------------------------------------------------------------

  // On copy apply for type image!
  template <class V>
  bool viewer1DPainter::apply(const V& src,image& dest) {
    const parameters& par = getParameters();

    const int width = static_cast<int>(src.size()*par.pixelsPerElement*par.zoom.x);

    return apply(src,
                 irectangle(ipoint(0,0),ipoint(width,vectorHeight_)),
                 dest);
  }

  template <class V>
  bool viewer1DPainter::apply(const V& src,
                              drawBase<rgbaPixel>& painter) {
    
    if (src.empty()) {
      return true;
    }
  }

  template <class V>
  bool viewer1DPainter::apply(const V& src,
                              const irectangle& viewport,
                              image& dest) {

    const parameters& par = getParameters();

    if (src.empty()) {
      dest.clear();
      return true;
    }

    dest.assign(viewport.getDimensions(),par.backgroundColor);
    draw<rgbaPixel> painter;
    painter.use(dest);
    return render(src,viewport,painter);
  }

  template <class V>
  bool viewer1DPainter::render(const V& src,
                               const irectangle& vp,
                               drawBase<rgbaPixel>& painter) {

    if (src.empty()) {
      return true;
    }
    
    irectangle viewPort(vp);
    viewPort.ensureConsistency();

    const parameters& param = getParameters();

    typename V::value_type vmin, vmax;

    if (cache_.checkCacheMiss(src.data(),src.size())) {
      cache_.update(src);
    }
     
    vmin=static_cast<typename V::value_type>(cache_.blocks.min_);
    vmax=static_cast<typename V::value_type>(cache_.blocks.max_);
    
    if (param.forceZero) {
      vmin = min(vmin, typename V::value_type(0));
      vmax = max(vmax, typename V::value_type(0));
    }

    // linear mapping between vector values and image coordinates
    double tmp = double(vmax - vmin);
    if (abs(tmp)>std::numeric_limits<double>::epsilon()) {
      tmp = -vectorHeight_/tmp;
    } else {
      tmp = 0.0;
    }
    
    // set mapping attributes
    const double my =  tmp;
    const double by = -my * vmax - viewPort.ul.y;
    const int totalWidth 
      = iround(ceil(src.size()*param.pixelsPerElement*param.zoom.x));
    const int portWidth  = viewPort.br.x - viewPort.ul.x;
    const int portHeight = viewPort.br.y - viewPort.ul.y;

    const double mx = double(totalWidth)/src.size();
    const double bx = double(-viewPort.ul.x);

    // draw grid
    if (param.drawGrid) {
      painter.setColor(param.gridColor);
      //horizontal grid
      if (param.ticks.y > 0) {
        const double incr = 
          static_cast<double>(vectorHeight_)/(param.ticks.y+1);
        const float fg = static_cast<float>(by-iround(floor(by/incr))*incr);
        float y = fg;
        const float tg = static_cast<float>(portHeight);
        while (y<=tg) {
          painter.horizontalLine(0,portWidth,static_cast<int>(y));
          y+=static_cast<float>(incr);
        }

      }
      // vertical grid
      if (param.ticks.x > 0) {
        const double incr = static_cast<double>(totalWidth+1)/(param.ticks.x+1);
        const float fg = static_cast<float>(bx-iround(floor(bx/incr))*incr);
        float x = fg;
        const float tg = static_cast<float>(portWidth);
        while (x<=tg) {
          painter.verticalLine(static_cast<int>(x),0,portHeight);
          x+=static_cast<float>(incr);
        }
      }
    }

    // draw zero level
    if (param.drawHorizontalAxis) {
      painter.setColor(param.axisColor);
      
      const int i = iround(by);
      if ((i >= 0) && (i <= portHeight)) {
        painter.horizontalLine(0,portWidth,i);
      }

      const int j = iround(bx);
      if ((j >= 0) && (j <= portWidth)) {
        painter.verticalLine(j,0,portHeight);
      }
    }

    // draw the mean value if necessary
    if (param.drawMeanValue) {
      painter.setColor(param.meanColor);
      painter.horizontalLine(0,portWidth,iround(my*cache_.blocks.mean_+by));
    }
    
    // Compute the initial and final indices
    const int sx = within(static_cast<int>(-bx/mx),0,src.lastIdx());
    const int ex = within(static_cast<int>((viewPort.br.x-bx)/mx),
                          0,src.lastIdx());
    painter.setColor(param.lineColor);
    
    switch(param.style) {
    case Dots: {
      if (param.marker == ".") {
        for (int vi = sx;vi <= ex;++vi) {
          const int ix = static_cast<int>(mx*vi+bx);
          const int iy = iround(my*src.at(vi)+by);
          
          painter.set(ix,iy);
        }
      } else {
        for (int vi = sx;vi <= ex;++vi) {
          const int ix = static_cast<int>(mx*vi+bx);
          const int iy = iround(my*src.at(vi)+by);
          
          painter.setColor(param.markerColor);
          painter.marker(ix,iy,param.marker.c_str());
        }
      }
    } break;
    case Boxes: {
      int vi = sx;
      int lastix = static_cast<int>(mx*vi+bx);
      int lastiy = iround(my*src.at(vi)+by);
      irectangle tmpR;
      for (vi++;vi <= ex;++vi) {
        const int ix = static_cast<int>(mx*vi+bx);
        const int iy = iround(my*src.at(vi)+by);
        tmpR.setConsistent(lastix,lastiy,ix,iround(by));
        painter.rectangle(tmpR);
        lastix=ix;
        lastiy=iy;
      }
      
    } break;
    case Steps: {
      int vi = sx;
      int lastix = static_cast<int>(mx*vi+bx);
      int lastiy = iround(my*src.at(vi)+by);

      for (vi++;vi <= ex;++vi) {
        const int ix = static_cast<int>(mx*vi+bx);
        const int iy = iround(my*src.at(vi)+by);

        painter.horizontalLine(lastix,ix,lastiy);
        painter.verticalLine(ix,lastiy,iy);

        lastix=ix;
        lastiy=iy;
      }
      
    } break;
    case Impulses: {
      if (param.marker == ".") {
        for (int vi = sx;vi <= ex;++vi) {
          const int ix = static_cast<int>(mx*vi+bx);
          const int iy = iround(my*src.at(vi)+by);
          
          painter.line(ix,iy,ix,iround(by));
        }
      } else {
        for (int vi = sx;vi <= ex;++vi) {
          const int ix = static_cast<int>(mx*vi+bx);
          const int iy = iround(my*src.at(vi)+by);
          
          painter.setColor(param.lineColor);
          painter.line(ix,iy,ix,iround(by));
          painter.setColor(param.markerColor);
          painter.marker(ix,iy,param.marker.c_str());
        }
      }
    } break;
    case Lines: {

      int vi = sx;
      int lastix = static_cast<int>(mx*vi+bx);
      int lastiy = iround(my*src.at(vi)+by);

      if (param.marker == ".") {
        for (vi++;vi <= ex;++vi) {
          const int ix = static_cast<int>(mx*vi+bx);
          const int iy = iround(my*src.at(vi)+by);
          
          painter.line(lastix,lastiy,ix,iy);

          lastix=ix;
          lastiy=iy;
        }
      } else {
        
        for (vi++;vi <= ex;++vi) {
          const int ix = static_cast<int>(mx*vi+bx);
          const int iy = iround(my*src.at(vi)+by);
          
          painter.setColor(param.lineColor);
          painter.line(lastix,lastiy,ix,iy);

          painter.setColor(param.markerColor);
          painter.marker(lastix,lastiy,param.marker.c_str());

          lastix=ix;
          lastiy=iy;
        }
        painter.setColor(param.markerColor);
        painter.marker(lastix,lastiy,param.marker.c_str());
      }
      
    } break;
    default:
      setStatusString("Unknown style parameter\n");
      return false;
    }
    
    return true;
  }
  
}

