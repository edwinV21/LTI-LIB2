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
 * \file   ltiSiftSampling_template.h
 *         Contains the class lti::siftSampling.
 * \author Arnd Hannemann
 * \date   5.1.2005
 *
 * revisions ..: $Id: ltiSiftSampling_template.h,v 1.12 2012-09-16 05:22:59 alvarado Exp $
 */

#include "ltiConstants.h"
#include "ltiDebug.h"

namespace lti {
  // -------------------------------------------------------------------
  // The apply-methods!
  // -------------------------------------------------------------------


  // apply for a different result order
  template <class Acc>
  bool siftSampling::apply(Acc& acc,
                           const matrix<typename Acc::value_type>& img,
                           const location& loc,
                           dvector& destv,
                           const eResultOrder& order) const {
    if ((order==DontCare) || (order==Sequential)) {
      return apply(acc,img,loc,destv);
    } else {
      bool rc = apply(acc,img,loc,destv);
      switchOrder(destv);
      return rc;
    } 
  }


  // apply for a different result order
  template <class Acc>
  bool siftSampling::apply(Acc& acc,
                           const matrix<typename Acc::value_type>& img,
                           const matrix<float>& src2,
                           const location& loc,
                           dvector& destv,
                           const eResultOrder& order) const {
    if ((order==DontCare) || (order==Sequential)) {
      return apply(acc,img,src2,loc,destv);
    } else {
      bool rc = apply(acc,img,src2,loc,destv);
      switchOrder(destv);
      return rc;
    } 
  }

  // applies on lists
  template <class Acc>
  bool siftSampling::apply(Acc& acc,
                           const matrix<typename Acc::value_type>& img,
                           const list<location>& locs,
                           std::list<dvector>& dests) const {
    //TODO: optimize this
    list<location>::const_iterator it;
    dests.clear();
    for (it=locs.begin();it!=locs.end();++it) {
      dvector tmp;
      apply(acc,img,*it,tmp);
      dests.push_back(tmp);
    }
    return true;
  }

  // applies on lists
  template <class Acc>
  bool siftSampling::apply(Acc& acc,
                           const matrix<typename Acc::value_type>& img,
                           const list<location>& locs,
                           std::list<dvector>& dests,
                           const eResultOrder&) const {
    //TODO: optimize this
    list<location>::const_iterator it;
    dests.clear();
    for (it=locs.begin();it!=locs.end();++it) {
      dvector tmp;
      apply(acc,img,*it,tmp);
      dests.push_back(tmp);
    }
    return true;
  }

  // applies on lists
  template <class Acc>
  bool siftSampling::apply(Acc& acc,
                           const matrix<typename Acc::value_type>& img,
                           const matrix<float>& src2,
                           const list<location>& locs,
                           std::list<dvector>& dests) const {
    //TODO: optimize this
    list<location>::const_iterator it;
    dests.clear();
    for (it=locs.begin();it!=locs.end();++it) {
      dvector tmp;
      apply(acc,img,src2,*it,tmp);
      dests.push_back(tmp);
    }
    return true;
  }

  // applies on lists
  template <class Acc>
  bool siftSampling::apply(Acc& acc,
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
      apply(acc,img,src2,*it,tmp,order);
      dests.push_back(tmp);
    }
    return true;
  }

  // another apply without location
  template <class Acc>
  bool siftSampling::apply(Acc& acc,
                           const matrix<typename Acc::value_type>& src,
                           const matrix<float>& src2,
                           dvector& destv,
                           const eResultOrder&) const {
    // of course the size of both sources must be equal
    const ipoint size = src.size();
    assert(size==src2.size());

    // typedef shortcuts
    typedef typename Acc::value_type T;
 
    const parameters& par = getParameters();
    // shortcut for faster access

    //shortcuts
    const int bins   = par.binsPerDimension;
    const int bins2  = bins*bins;
    //    const int size   = par.binSize;
    const int bsize  = par.binSize*par.binsPerDimension;
    //    const int hsize  = size / 2;
    const int bhsize = bsize /2;
    
    // src has to be larger than "sampled" region 
    assert(size.x >= bsize);
    assert(size.y >= bsize);

    // compute center
    const int cx = iround(size.x/2);    
    const int cy = iround(size.y/2);

    //create accumulators
    static std::vector<Acc> accuVec(bins2);

    // resize accuvector
    _lti_debug("resizing...");
    if (static_cast<int>(accuVec.size())!=bins2) {
      accuVec.resize(bins2);
    }    
    _lti_debug("done\n");

    // reset first accu and then copy it
    _lti_debug("reset first accu..");
    acc.reset();
    _lti_debug("done\n");

    typename std::vector<Acc>::iterator it;
    for (it = accuVec.begin(); it != accuVec.end(); ++it) {
      (*it).copy(acc);
    }

    int x,y;
    for (y = -bhsize; y<bhsize; y++) {
      const int ty = y+cy;
      
      // y-coordinate for accumulator
      const float ay = norm_.at(y);
      
      for (x = -bhsize; x<bhsize; x++) {
        const int tx = x+cx;
        
        const T val      = src.at(ty,tx);
        const float val2 = src2.at(ty,tx);         
        
        // accumulate values
        accumulate(accuVec,val,val2,x,y,ay);
        
      } //        for (x = -bhsize; x<bhsize; x++) {
    } //  for (y = -bhsize; y<bhsize; y++) {

    // get results from accumulators
    int arraySize = accuVec.at(0).getArraySize();
    matrix<double> result;
    result.resize(bins2, arraySize);    
    
    // pointer to memory
    double* dest = &(result[0][0]);

    // each row in the matrix is filled with the accumulator
    // result of one bin
    int i;
    for (i=0;i<bins2;i++) {
      // the accu can write its data to the appropiate places
      // in the matrix
      accuVec.at(i).getResult(dest+(i*arraySize));
    }
    
    // copy result to destination vector
    result.detach(destv);

    return true;

  }

  // another apply without location
  template <class Acc>
  bool siftSampling::apply(Acc& acc,
                           const matrix<typename Acc::value_type>& src,
                           dvector& destv,
                           const eResultOrder&) const {

    // shortcut
    const ipoint size = src.size();


    // typedef shortcuts
    typedef typename Acc::value_type T;
 
    const parameters& par = getParameters();
    // shortcut for faster access

    //shortcuts
    const int bins   = par.binsPerDimension;
    const int bins2  = bins*bins;
    //    const int size   = par.binSize;
    const int bsize  = par.binSize*par.binsPerDimension;
    //    const int hsize  = size / 2;
    const int bhsize = bsize /2;
    
    // src has to be larger than "sampled" region 
    assert(size.x >= bsize);
    assert(size.y >= bsize);

    // compute center
    const int cx = iround(size.x/2);    
    const int cy = iround(size.y/2);

    //create accumulators
    static std::vector<Acc> accuVec(bins2);

    // resize accuvector
    _lti_debug("resizing...");
    if (static_cast<int>(accuVec.size())!=bins2) {
      accuVec.resize(bins2);
    }    
    _lti_debug("done\n");

    // reset first accu and then copy it
    _lti_debug("reset first accu..");
    acc.reset();
    _lti_debug("done\n");

    typename std::vector<Acc>::iterator it;
    for (it = accuVec.begin(); it != accuVec.end(); ++it) {
      (*it).copy(acc);
    }

    int x,y;
    for (y = -bhsize; y<bhsize; y++) {
      const int ty = y+cy;
      
      // y-coordinate for accumulator
      const float ay = norm_.at(y);
      
      for (x = -bhsize; x<bhsize; x++) {
        const int tx = x+cx;
        
        const T val      = src.at(ty,tx);
        
        // accumulate values
        accumulate(accuVec,val,1.0f,x,y,ay);
        
      } //        for (x = -bhsize; x<bhsize; x++) {
    } //  for (y = -bhsize; y<bhsize; y++) {

    // get results from accumulators
    int arraySize = accuVec.at(0).getArraySize();
    matrix<double> result;
    result.resize(bins2, arraySize);    
    
    // pointer to memory
    double* dest = &(result[0][0]);

    // each row in the matrix is filled with the accumulator
    // result of one bin
    int i;
    for (i=0;i<bins2;i++) {
      // the accu can write its data to the appropiate places
      // in the matrix
      accuVec.at(i).getResult(dest+(i*arraySize));
    }
    
    // copy result to destination vector
    result.detach(destv);

    return true;

  }


                       
  template <class Acc>
  bool siftSampling::apply(Acc& acc,
                           const matrix<typename Acc::value_type>& src,
                           const matrix<float>& src2,
                           const location& loc,
                           dvector& destv) const {
    // of course the size of both sources must be equal
    assert(src.size()==src2.size());
    
    // typedef shortcuts
    typedef typename Acc::value_type T;
 
    const parameters& par = getParameters();
    // shortcut for faster access

    //shortcuts
    const int bins   = par.binsPerDimension;
    const int bins2  = bins*bins;
    //    const int size   = par.binSize;
    const int bsize  = par.binSize*par.binsPerDimension;
    //    const int hsize  = size / 2;
    const int bhsize = bsize /2;

    // for now the radius is ignored
    const float posx  = loc.position.x;
    const float posy  = loc.position.y;
    const float angle = loc.angle;

    //create accumulators
    static std::vector<Acc> accuVec(bins2);

    // resize accuvector
    _lti_debug("resizing...");
    if (static_cast<int>(accuVec.size())!=bins2) {
      accuVec.resize(bins2);
    }    
    _lti_debug("done\n");

    // reset first accu and then copy it
    _lti_debug("reset first accu..");
    acc.reset();
    _lti_debug("done\n");

    typename std::vector<Acc>::iterator it;
    for (it = accuVec.begin(); it != accuVec.end(); ++it) {
      (*it).copy(acc);
    }

    int x,y;


    // interpolators
    const static bilinearInterpolation<T> bilin;
    const static nearestNeighborInterpolation<T> nearest;
    
    if (angle != 0) {
      const float sina = sin(angle);
      const float cosa = cos(angle);

      if (par.keyIsOrientation) {
        
        // check bounds
        if (((posx-rad_) >= 0) && ((posx+rad_) < src.columns()) &&
            ((posy-rad_) >= 0) && ((posy+rad_) < src.rows())) {
          
          _lti_debug("  using unchecked interpolation\n");
          for (y = -bhsize; y<bhsize; y++) {
            // we sample not the center itself but 0.5 to the left and right
            const float fy = y +0.5f;
            
            // y-coordinate for accumulator
            const float ay = norm_.at(y);
            
            for (x = -bhsize; x<bhsize; x++) {
              const float fx = x+0.5f;
              
              // rotation
              const float tx = cosa*fx-sina*fy+posx;
              const float ty = sina*fx+cosa*fy+posy;
              
              // interpolate values (unchecked)
              // using nearest interpolation for keys 
              T val      = nearest.interpolateUnchk(src,ty,tx);
              const float val2 = bilin.interpolateUnchk(src2,ty,tx); 

              // key relative to orientation of location
              clipAngle(val,angle);
              
              // accumulate values
              accumulate(accuVec,val,val2,x,y,ay);
          
            } //        for (x = -bhsize; x<bhsize; x++) {
          } //  for (y = -bhsize; y<bhsize; y++) {
          
        } else {
          _lti_debug("  using checked interpolation\n");
          for (y = -bhsize; y<bhsize; y++) {
            // we sample not the center itself but 0.5 to the left and right
            const float fy = y+0.5f;
            
            // y-coordinate for accumulator
            const float ay = norm_.at(y);
            for (x = -bhsize; x<bhsize; x++) {
              const float fx = x+0.5f;
              
              // rotation
              const float tx = cosa*fx-sina*fy+posx;
              const float ty = sina*fx+cosa*fy+posy;
             
              
              // interpolate values (checked)
              // using nearest interpolation for keys 
              const T val      = nearest.interpolate(src,ty,tx);
              const float val2 = bilin.interpolate(src2,ty,tx);
              
              // key relative to orientation of location
              clipAngle(val,angle);

              // accumulate values
              accumulate(accuVec,val,val2,x,y,ay);
            } // for x
          } // for y
        } // checked interpolation
      } // key is orientation
      else {
        
        if (((posx-rad_) >= 0) && ((posx+rad_) < src.columns()) &&
            ((posy-rad_) >= 0) && ((posy+rad_) < src.rows())) {
          
          _lti_debug("  using unchecked interpolation\n");
          for (y = -bhsize; y<bhsize; y++) {
            // we sample not the center itself but 0.5 to the left and right
            const float fy = y +0.5f;
            
            // y-coordinate for accumulator
            const float ay = norm_.at(y);
            
            for (x = -bhsize; x<bhsize; x++) {
                const float fx = x+0.5f;
                
                // rotation
                const float tx = cosa*fx-sina*fy+posx;
                const float ty = sina*fx+cosa*fy+posy;
                
                // interpolate values
                const T val      = bilin.interpolateUnchk(src,ty,tx);
                const float val2 = bilin.interpolateUnchk(src2,ty,tx); 
                
                // accumulate values
                accumulate(accuVec,val,val2,x,y,ay);
                
            } //        for (x = -bhsize; x<bhsize; x++) {
          } //  for (y = -bhsize; y<bhsize; y++) {
          
        } else {
          _lti_debug("  using checked interpolation\n");
          for (y = -bhsize; y<bhsize; y++) {
            // we sample not the center itself but 0.5 to the left and right
            const float fy = y+0.5f;
            
            // y-coordinate for accumulator
            const float ay = norm_.at(y);
              for (x = -bhsize; x<bhsize; x++) {
                const float fx = x+0.5f;
                
                // rotation
                const float tx = cosa*fx-sina*fy+posx;
                const float ty = sina*fx+cosa*fy+posy;
                
                // interpolate value
                const T val      = bilin.interpolate(src,ty,tx);
                const float val2 = bilin.interpolate(src2,ty,tx);
                
                // accumulate values
                accumulate(accuVec,val,val2,x,y,ay);
              } // for x
          } // for y
        } // checked interpolation
      } // key is NOT orientation
    } //without rotation 
    else {
      if (par.keyIsOrientation) {
        
        // check bounds
        if (((posx-rad_) >= 0) && ((posx+rad_) < src.columns()) &&
            ((posy-rad_) >= 0) && ((posy+rad_) < src.rows())) {
          
          _lti_debug("  using unchecked interpolation\n");
          for (y = -bhsize; y<bhsize; y++) {
            // we sample not the center itself but 0.5 to the left and right
            const float fy = y +0.5f;
            
            // y-coordinate for accumulator
            const float ay = norm_.at(y);
            for (x = -bhsize; x<bhsize; x++) {
              const float fx = x+0.5f;
              
              // no rotation but offset
              const float tx = fx+posx;
              const float ty = fy+posy;
              
              // interpolate values
              // using nearest interpolation for keys 
              const T val      = nearest.interpolate(src,ty,tx);
              const float val2 = bilin.interpolateUnchk(src2,ty,tx);

              // accumulate values
              accumulate(accuVec,val,val2,x,y,ay);            
              
              
            } //        for (x = -bhsize; x<bhsize; x++) {
          } //  for (y = -bhsize; y<bhsize; y++) {
          
        } else {
          _lti_debug("  using checked interpolation\n");
          for (y = -bhsize; y<bhsize; y++) {
            // we sample not the center itself but 0.5 to the left and right
            const float fy = y+0.5f;
            
            // y-coordinate for accumulator
            const float ay = norm_.at(y);
            for (x = -bhsize; x<bhsize; x++) {
              const float fx = x+0.5f;
              
              // no rotation but offset
              const float tx = fx+posx;
              const float ty = fy+posy;           
              
              // interpolate values
              // using nearest interpolation for keys 
              T val            = nearest.interpolate(src,ty,tx);
              const float val2 = bilin.interpolate(src2,ty,tx);
              
              // accumulate values
              accumulate(accuVec,val,val2,x,y,ay);            
          
            } // for x
          } // for y
        } // checked interpolateion
      } else {
        // Key is NOT orientation
        // check bounds
        if (((posx-rad_) >= 0) && ((posx+rad_) < src.columns()) &&
            ((posy-rad_) >= 0) && ((posy+rad_) < src.rows())) {
          
          _lti_debug("  using unchecked interpolation\n");
          for (y = -bhsize; y<bhsize; y++) {
            // we sample not the center itself but 0.5 to the left and right
            const float fy = y +0.5f;
            
            // y-coordinate for accumulator
            const float ay = norm_.at(y);
            for (x = -bhsize; x<bhsize; x++) {
              const float fx = x+0.5f;
              
              // no rotation but offset
              const float tx = fx+posx;
              const float ty = fy+posy;
              
              // interpolate values
              const T val      = bilin.interpolate(src,ty,tx);
              const float val2 = bilin.interpolateUnchk(src2,ty,tx);

              // accumulate values
              accumulate(accuVec,val,val2,x,y,ay);            
              
              
            } //        for (x = -bhsize; x<bhsize; x++) {
          } //  for (y = -bhsize; y<bhsize; y++) {
          
        } else {
          _lti_debug("  using checked interpolation\n");
          for (y = -bhsize; y<bhsize; y++) {
            // we sample not the center itself but 0.5 to the left and right
            const float fy = y+0.5f;
            
            // y-coordinate for accumulator
            const float ay = norm_.at(y);
            for (x = -bhsize; x<bhsize; x++) {
              const float fx = x+0.5f;
              
              // no rotation but offset
              const float tx = fx+posx;
              const float ty = fy+posy;           
              
              // interpolate values
              T val            = bilin.interpolate(src,ty,tx);
              const float val2 = bilin.interpolate(src2,ty,tx);
              
              // accumulate values
              accumulate(accuVec,val,val2,x,y,ay);            
          
            } // for x
          } // for y
        } // checked interpolateion        
      } // else
    } // else

    // get results from accumulators
    int arraySize = accuVec.at(0).getArraySize();
    matrix<double> result;
    result.resize(bins2, arraySize);
    
    
    // pointer to memory
    double* dest = &(result[0][0]);

    // each row in the matrix is filled with the accumulator
    // result of one bin
    int i;
    for (i=0;i<bins2;i++) {
      // the accu can write its data to the appropiate places
      // in the matrix
      accuVec.at(i).getResult(dest+(i*arraySize));
    }
    
    // copy result to destination vector
    result.detach(destv);

    return true;
  }




  template <class Acc>
  bool siftSampling::apply(Acc& acc,
                           const matrix<typename Acc::value_type>& src,
                           const location& loc,
                           dvector& destv) const {
  
    const parameters& par = getParameters();

    // interpolator
    static const bilinearInterpolation<typename Acc::value_type> bilin;

    //shortcuts
    const int bins   = par.binsPerDimension;
    const int bins2  = bins*bins;
    //    const int size   = par.binSize;
    const int bsize  = par.binSize*par.binsPerDimension;
    //    const int hsize  = size / 2;
    const int bhsize = bsize /2;

    // for now the radius is ignored
    const float posx  = loc.position.x;
    const float posy  = loc.position.y;
    const float angle = loc.angle;

    //create accumulators
    std::vector<Acc> accuVec(bins2);

    // resize accuvector
    _lti_debug("resizing...");
    if (static_cast<int>(accuVec.size())!=bins2) {
      accuVec.resize(bins2);
    }    
    _lti_debug("done\n");

    // reset first accu and then copy it
    acc.reset();
    typename std::vector<Acc>::iterator it;
    for (it = accuVec.begin(); it != accuVec.end(); ++it) {
      (*it).copy(acc);
    }

    // x and y coordinates for stepping through the masks
    int x,y;
   
    // TODO: keyIsOrientation is ignored for now
    if (angle != 0) {
      const float sina = sin(angle);
      const float cosa = cos(angle);

      // check bounds
      if (((posx-rad_) >= 0) && ((posx+rad_) < src.columns()) &&
          ((posy-rad_) >= 0) && ((posy+rad_) < src.rows())) {
        
        _lti_debug("  using unchecked interpolation\n");
        for (y = -bhsize; y<bhsize; y++) {
          // we sample not the center itself but 0.5 to the left and right
          const float fy = y +0.5f;
          
          // y-coordinate for accumulator
          const float ay = norm_.at(y);
          for (x = -bhsize; x<bhsize; x++) {
            const float fx = x+0.5f;
            
            // rotation
            const float tx = cosa*fx-sina*fy+posx;
            const float ty = sina*fx+cosa*fy+posy;
                       
            // interpolated value
            typename Acc::value_type val = bilin.interpolateUnchk(src,ty,tx);
                    
            // accumulate values
            accumulate(accuVec,val,x,y,ay);            
          
          } //        for (x = -bhsize; x<bhsize; x++) {
        } //  for (y = -bhsize; y<bhsize; y++) {
      
      } else {
        _lti_debug("  using checked interpolation\n");
        for (y = -bhsize; y<bhsize; y++) {
          // we sample not the center itself but 0.5 to the left and right
          const float fy = y+0.5f;
          
          // y-coordinate for accumulator
          const float ay = norm_.at(y);
          for (x = -bhsize; x<bhsize; x++) {
            const float fx = x+0.5f;
            
            // rotation
            const float tx = cosa*fx-sina*fy+posx;
            const float ty = sina*fx+cosa*fy+posy;
            
            // interpolated value
            typename Acc::value_type val = bilin.interpolate(src,ty,tx);
           
            // accumulate values
            accumulate(accuVec,val,x,y,ay);            
          }
        }
      } // else
    } //without rotation 
    else {
      // check bounds
      if (((posx-rad_) >= 0) && ((posx+rad_) < src.columns()) &&
          ((posy-rad_) >= 0) && ((posy+rad_) < src.rows())) {
        
        _lti_debug("  using unchecked interpolation\n");
        for (y = -bhsize; y<bhsize; y++) {
          // we sample not the center itself but 0.5 to the left and right
          const float fy = y +0.5f;
          
          // y-coordinate for accumulator
          const float ay = norm_.at(y);
          for (x = -bhsize; x<bhsize; x++) {
            const float fx = x+0.5f;
            
            // rotation
            const float tx = fx+posx;
            const float ty = fy+posy;
            
            // interpolated value
            typename Acc::value_type val = bilin.interpolateUnchk(src,ty,tx);
                    
            // accumulate values
            accumulate(accuVec,val,x,y,ay);           
          
          } //        for (x = -bhsize; x<bhsize; x++) {
        } //  for (y = -bhsize; y<bhsize; y++) {
      
      } else {
        _lti_debug("  using checked interpolation\n");
        for (y = -bhsize; y<bhsize; y++) {
          // we sample not the center itself but 0.5 to the left and right
          const float fy = y+0.5f;
          
          // y-coordinate for accumulator
          const float ay = norm_.at(y);
          for (x = -bhsize; x<bhsize; x++) {
            const float fx = x+0.5f;
            
            // rotation
            const float tx = fx+posx;
            const float ty = fy+posy;
            
            // interpolated value
            typename Acc::value_type val = bilin.interpolate(src,ty,tx);
            
            // accumulate values
            accumulate(accuVec,val,x,y,ay);           
          
          }
        }
      } // else
    } // else

    // get results from accumulators
    int arraySize = accuVec.at(0).getArraySize();
    matrix<double> result;
    result.resize(bins2, arraySize);
    
    // pointer to memory
    double* dest = &(result[0][0]);

    // each row in the matrix is filled with the accumulator
    // result of one bin
    int i;
    for (i=0;i<bins2;i++) {
      // the accu can write its data to the appropiate places
      // in the matrix
      accuVec.at(i).getResult(dest+(i*arraySize));
    }
    
    // copy result to destination vector
    result.detach(destv);

    return true;
  }


}
