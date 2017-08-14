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
 * \file   ltiGridSampling_template.h
 *         Contains the template methods of lti::gridSampling.
 * \author Arnd Hannemann
 * \date   12.8.2004
 *
 * revisions ..: $Id: ltiGridSampling_template.h,v 1.12 2009-08-30 04:30:29 alvarado Exp $
 */

#include "ltiNearestNeighborInterpolation.h"

namespace lti {


  // simple but slow implementation
  template <class Acc>
  bool gridSampling::apply(Acc& accu,
                           const matrix<typename Acc::value_type>& img,
                           const list<location>& locs,
                           std::list<dvector>& dests,
                           const eResultOrder& order) const {
    //TODO: optimize this
    list<location>::const_iterator it;
    dests.clear();
    for (it=locs.begin();it!=locs.end();++it) {
      dvector tmp;
      apply(accu,img,*it,tmp,order);
      dests.push_back(tmp);
    }
    return true;
  }

  // apply on single source
  template <class Acc>
  bool gridSampling::apply(Acc& accu,
                           const matrix<typename Acc::value_type>& chan,
                           const location& loc,
                           dvector& dest,
                           const eResultOrder&) const {

    // shortcut
    typedef typename Acc::value_type T;

    bool rc = true;  
    const parameters& par = getParameters();
//    const int rad = loc.radius;

    // interpolation
    static const bilinearInterpolation<typename Acc::value_type> myInterpol;

    const float frad = loc.radius;
    const float angle = loc.angle;
    const float x = loc.position.x;
    const float y = loc.position.y;
    const int rad = par.gridRadius;
    const int r = chan.rows();
    const int c = chan.columns();
    const float fr = static_cast<float>(r)-2;    
    const float fc = static_cast<float>(c)-2;

    // some loop variables
    int i;
    float j,k,tx,ty,fi;
    k = 0.0f;
    const float scale = frad/static_cast<float>(rad);

    // resetting accumulator
    accu.reset();
    if ((par.useOrientation) && (angle != 0.0f)) {
      const float sina=sin(angle);
      const float cosa=cos(angle);
      // check if all pixels are in the image, because
      // then we can use faster unchecked interpolation...
      if ((x-frad>1) && (x+frad<fc) &&
          (y-frad>1) && (y+frad<fr)) {
        for (i=-rad; i<=rad; ++i) {
          fi = static_cast<float>(i);
          k = static_cast<float>(circPoints_.at(i));
          const float sinfi = sina*fi;
          const float cosfi = cosa*fi;
          for (j=-k; j<=k; j+=1.) {
            tx=cosa*j-sinfi;
            tx *= scale;
            ty=sina*j+cosfi;
            ty *= scale;
            const T val = myInterpol.interpolateUnchk(chan,ty+y,tx+x);
            accu.accumulate(val,j*scale,i*scale);                            
          }
        }
      } // if we come to the borders we have to use checked interpolation
      else {
        for (i=-rad; i<=rad; ++i) {
          fi = static_cast<float>(i);
          const float sinfi = sina*fi;
          const float cosfi = cosa*fi;
          k = static_cast<float>(circPoints_.at(i));
          for (j=-k; j<=k; j+=1.) {
            tx=cosa*j-sinfi;
            tx *= scale;
            ty=sina*j+cosfi;
            ty *= scale;
            const T val = myInterpol.interpolate(chan,ty+y,tx+x);
            accu.accumulate(val,j*scale,i*scale);                            
          }
        }
      } // else      
    } // if useOrientation is false it is not neccesary to rotate
    else {
      // check if all pixels are in the image, because
      // then we can use faster unchecked interpolation...
      if ((x-frad>1) && (x+frad<fc) &&
          (y-frad>1) && (y+frad<fr)) {
        for (i=-rad; i<=rad; ++i) {
          fi = static_cast<float>(i);
          k = static_cast<float>(circPoints_.at(i));
          for (j=-k; j<=k; j+=1.) {
            tx = scale*j;
            ty = scale*i;
            const T val = myInterpol.interpolateUnchk(chan,ty+y,tx+x);
            accu.accumulate(val,j*scale,i*scale);                            
          }
        }
        
      } else {
        for (i=-rad; i<=rad; ++i) {
          k = static_cast<float>(circPoints_.at(i));
          for (j=-k; j<=k; j+=1.) {
            tx = scale*j;
            ty = scale*static_cast<float>(i);
            const T val = myInterpol.interpolate(chan,ty+y,tx+x);
            accu.accumulate(val,j*scale,i*scale);                            
          }
        }
      }
    }

    // get result directly into the destination vector
#ifdef _LTI_DEBUG
    // makes bug tracking a bit easier
    dest.resize(accu.getArraySize(),-1.,Init);
#else
    dest.resize(accu.getArraySize(),0.0,AllocateOnly);
#endif
    rc = rc && accu.getResult(&dest[0]);
    return rc;

  }


  // simple but slow implementation
  template <class Acc>
  bool gridSampling::apply(Acc& accu,
                           const matrix<typename Acc::value_type>& img,
                           const matrix<float>& src2,
                           const list<location>& locs,
                           std::list<dvector>& dests,
                           const eResultOrder& order) const {
    //TODO: optimize this
    list<location>::const_iterator it;
    dests.clear();
    for (it=locs.begin();it!=locs.end();++it) {
      dvector tmp;
      apply(accu,img,src2,*it,tmp,order);
      dests.push_back(tmp);
    }
    return true;
  }

  // apply  without location
  template <class Acc>
  bool gridSampling::apply(Acc& accu,
                           const matrix<typename Acc::value_type>& chan,
                           dvector& dest,
                           const eResultOrder&) const {
    ipoint size = chan.size();

    // shortcut
    typedef typename Acc::value_type T;

    const parameters& par = getParameters();

    // shortcuts
    const int rad = par.gridRadius;

    // at least the size must be greater than the diameter
    assert(size.x >= 2*rad);
    assert(size.y >= 2*rad);

    bool rc = true;  

    // determine center
    const int cx = iround(size.x/2);
    const int cy = iround(size.y/2);

    // resetting accumulator
    accu.reset();

    // some loop variables
    int i,k,j,tx,ty;
    for (i=-rad; i<=rad; ++i) {
      const float fi = static_cast<float>(i);                         
      k = circPoints_.at(i);
      for (j=-k; j<=k; ++j) {
        const float fj = static_cast<float>(j);   
        tx = j+cx;
        ty = i+cy;
        const T val = chan.at(ty,tx);
        accu.accumulate(val,fj,fi);                 
      }
    }

    // get result directly into the destination vector
#ifdef _LTI_DEBUG
    // makes bug tracking a bit easier
    dest.resize(accu.getArraySize(),-1.,Init);
#else
    dest.resize(accu.getArraySize(),0.0,AllocateOnly);
#endif

    rc = rc && accu.getResult(&dest[0]);
    return rc;
  }


  // apply on two sources without location
  template <class Acc>
  bool gridSampling::apply(Acc& accu,
                           const matrix<typename Acc::value_type>& chan,
                           const matrix<float>& src2,
                           dvector& dest,
                           const eResultOrder&) const {
    ipoint size = chan.size();
    // of course the sources must have the same size 
    assert(size == src2.size());
    // shortcut
    typedef typename Acc::value_type T;

    const parameters& par = getParameters();

    // shortcuts
    const int rad = par.gridRadius;

    // at least the size must be greater than the diameter
    assert(size.x >= 2*rad);
    assert(size.y >= 2*rad);

    bool rc = true;  

    // determine center
    const int cx = iround(size.x/2);
    const int cy = iround(size.y/2);

    // resetting accumulator
    accu.reset();

    // some loop variables
    int i,k,j,tx,ty;
    for (i=-rad; i<=rad; ++i) {
      const float fi = static_cast<float>(i);                         
      k = circPoints_.at(i);
      for (j=-k; j<=k; ++j) {
        const float fj = static_cast<float>(j);   
        tx = j+cx;
        ty = i+cy;
        T val = chan.at(ty,tx);
        const float val2 = src2.at(ty,tx);
        accu.accumulate(val,val2,fj,fi);                 
      }
    }

    // get result directly into the destination vector
#ifdef _LTI_DEBUG
    // makes bug tracking a bit easier
    dest.resize(accu.getArraySize(),-1.,Init);
#else
    dest.resize(accu.getArraySize(),0.0,AllocateOnly);
#endif

    rc = rc && accu.getResult(&dest[0]);
    return rc;
  }

  // apply on two sources
  template <class Acc>
  bool gridSampling::apply(Acc& accu,
                           const matrix<typename Acc::value_type>& chan,
                           const matrix<float>& src2,
                           const location& loc,
                           dvector& dest,
                           const eResultOrder&) const {
    // of course the sources must have the same size 
    assert(chan.size() == src2.size());
    
    // shortcut
    typedef typename Acc::value_type T;

    bool rc = true;  
    const parameters& par = getParameters();
//    const int rad = loc.radius;

    // interpolation
    static const bilinearInterpolation<typename Acc::value_type> myInterpol;
    static const nearestNeighborInterpolation<typename Acc::value_type>
      nearest;

    const float frad = loc.radius;
    const float angle = loc.angle;
    const float x = loc.position.x;
    const float y = loc.position.y;
    const int rad = par.gridRadius;
    const int r = chan.rows();
    const int c = chan.columns();
    const float fr = static_cast<float>(r)-2;    
    const float fc = static_cast<float>(c)-2;

    // some loop variables
    float tx,ty,fi;
    const float scale = frad/static_cast<float>(rad);

    // resetting accumulator
    accu.reset();
    
    if (par.keyIsOrientation) {
      if ((par.useOrientation) && (angle != 0.0f)){
        const float sina=sin(angle);
        const float cosa=cos(angle);
        // check if all pixels are in the image, because
        // then we can use faster unchecked interpolation...
        if ((x-frad>1) && (x+frad<fc) &&
            (y-frad>1) && (y+frad<fr)) {
          for (int i=-rad; i<=rad; ++i) {
            fi = static_cast<float>(i);
            const float sinfi = sina*fi;
            const float cosfi = cosa*fi;
            const float k = static_cast<float>(circPoints_.at(i));
            for (float j=-k; j<=k; j+=1.f) {
              tx=cosa*j-sinfi;
              tx = scale*tx +x;
              ty=sina*j+cosfi;
              ty = scale*ty+y;
              const T val = nearest.interpolateUnchk(chan,ty,tx);
              const float val2 = myInterpol.interpolateUnchk(src2,ty,tx); 
              
              // key relative to location angle
              clipAngle(val,angle);
              
              accu.accumulate(val,val2,j*scale,i*scale);          
            }
          }
        } // if we come to the borders we have to use checked interpolation
        else {
          for (int i=-rad; i<=rad; ++i) {
            fi = static_cast<float>(i);
            const float sinafi = sina*fi;
            const float cosafi = cosa*fi;
            const float k = static_cast<float>(circPoints_.at(i));
            for (float j=-k; j<=k; j+=1.) {
              tx=cosa*j-sinafi;
              tx = scale*tx+x;
              ty=sina*j+cosafi;
              ty = scale*ty+y;
              const T val = nearest.interpolate(chan,ty,tx);
              const float val2 = myInterpol.interpolate(src2,ty,tx); 
              
              // key relative to location angle
              clipAngle(val,angle);
  
              accu.accumulate(val,val2,j*scale,i*scale);          
            }
          }
        } // else      
      } // if useOrientation is false it is not neccesary to rotate
      else {
      // check if all pixels are in the image, because
      // then we can use faster unchecked interpolation...
        if ((x-frad>1) && (x+frad<fc) &&
            (y-frad>1) && (y+frad<fr)) {
          for (int i=-rad; i<=rad; ++i) {
            fi = static_cast<float>(i);
            static float k = static_cast<float>(circPoints_.at(i));
            for (float j=-k; j<=k; j+=1.) {
              tx = scale*j+x;
              ty = scale*i+y;
              const T val = nearest.interpolateUnchk(chan,ty,tx);
              const float val2 = myInterpol.interpolateUnchk(src2,ty,tx); 
              accu.accumulate(val,val2,j*scale,i*scale);                 
            }
          }
          
        } else {
          for (int i=-rad; i<=rad; ++i) {
            fi = static_cast<float>(i);
            const float k = static_cast<float>(circPoints_.at(i));
            for (float j=-k; j<=k; j+=1.) {
              tx = scale*j+x;
              ty = scale*i+y;
              const T val = nearest.interpolate(chan,ty,tx);
              const float val2 = myInterpol.interpolate(src2,ty,tx); 
              accu.accumulate(val,val2,j*scale,i*scale);                 
            } // for j
          }
        } // if checked
      } // if useOrientation
    } // if keyIsOrientation
    else {     
      if ((par.useOrientation) && (angle != 0.0f)){
        const float sina=sin(angle);
        const float cosa=cos(angle);
        // check if all pixels are in the image, because
        // then we can use faster unchecked interpolation...
        if ((x-frad>1) && (x+frad<fc) &&
            (y-frad>1) && (y+frad<fr)) {
          for (int i=-rad; i<=rad; ++i) {
            fi = static_cast<float>(i);
            const float sinfi = sina*fi;
            const float cosfi = cosa*fi;
            const float k = static_cast<float>(circPoints_.at(i));
            for (float j=-k; j<=k; j+=1.f) {
              tx=cosa*j-sinfi;
              tx = scale*tx+x;
              ty=sina*j+cosfi;
              ty = scale*ty+y;
              const T val = myInterpol.interpolateUnchk(chan,ty,tx);
              const float val2 = myInterpol.interpolateUnchk(src2,ty,tx); 
              accu.accumulate(val,val2,j*scale,i*scale);          
            }
          }
        } // if we come to the borders we have to use checked interpolation
        else {
          for (int i=-rad; i<=rad; ++i) {
            fi = static_cast<float>(i);
            const float sinafi = sina*fi;
            const float cosafi = cosa*fi;
            const float k = static_cast<float>(circPoints_.at(i));
            for (float j=-k; j<=k; j+=1.) {
              tx=cosa*j-sinafi;
              tx *= scale;
              tx += x;
              ty=sina*j+cosafi;
              ty *= scale;
              ty += y;
              const T val = myInterpol.interpolate(chan,ty,tx);
              const float val2 = myInterpol.interpolate(src2,ty,tx); 
              accu.accumulate(val,val2,j*scale,i*scale);          
            }
          }
        } // else      
      } // if useOrientation is false it is not neccesary to rotate
      else {
      // check if all pixels are in the image, because
      // then we can use faster unchecked interpolation...
        if ((x-frad>1) && (x+frad<fc) &&
            (y-frad>1) && (y+frad<fr)) {
          for (int i=-rad; i<=rad; ++i) {
            fi = static_cast<float>(i);
            static float k = static_cast<float>(circPoints_.at(i));
            for (float j=-k; j<=k; j+=1.) {
              tx = scale*j+x;
              ty = scale*i+y;
              const T val = myInterpol.interpolateUnchk(chan,ty,tx);
              const float val2 = myInterpol.interpolateUnchk(src2,ty,tx); 
              accu.accumulate(val,val2,j*scale,i*scale);                 
            }
          }
          
        } else {
          for (int i=-rad; i<=rad; ++i) {
            fi = static_cast<float>(i);
            const float k = static_cast<float>(circPoints_.at(i));
            for (float j=-k; j<=k; j+=1.) {
              tx = scale*j+x;
              ty = scale*fi+y;
              const T val = myInterpol.interpolate(chan,ty,tx);
              const float val2 = myInterpol.interpolate(src2,ty,tx); 
              accu.accumulate(val,val2,j*scale,i*scale);                 
            } // for j
          } // for i
        } // if checked
      } // if useOrientation
    } // else keyIsOrientation

    // get result directly into the destination vector
#ifdef _LTI_DEBUG
    // makes bug tracking a bit easier
    dest.resize(accu.getArraySize(),-1.,Init);
#else
    dest.resize(accu.getArraySize(),0.0,AllocateOnly);
#endif
    rc = rc && accu.getResult(&dest[0]);
    return rc;
  } 
  
  
}
