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
 * \file   ltiSliceSampling_template.h
 *         Contains the template class sliceSampling.
 * \author Arnd Hannemann
 * \date   27.08.2004
 *
 * revisions ..: $Id: ltiSliceSampling_template.h,v 1.19 2009-08-30 04:30:29 alvarado Exp $
 */

#include "ltiBilinearInterpolation.h"
#include "ltiNearestNeighborInterpolation.h"

#include <cstring>

namespace lti {
  // -------------------------------------------------------------------
  // The apply-methods!
  // -------------------------------------------------------------------

  template <class Acc>
  bool sliceSampling::apply(Acc& acc,
                            const matrix<typename Acc::value_type>& img,
                            const location& loc,
                            dvector& destv,
                            const eResultOrder& order) const {
    bool rc = apply(acc,img,loc,destv);
    if (order==Sequential) {
      switchOrder(destv);
    }
    return rc;
  }


  template <class Acc>
  bool sliceSampling::apply(Acc& acc,
                            const matrix<typename Acc::value_type>& img,
                            const matrix<float>& src2,
                            const location& loc,
                            dvector& destv,
                            const eResultOrder& order) const {
    bool rc = apply(acc,img,src2,loc,destv);
    if (order==Sequential) {
      switchOrder(destv);
    }
    return rc;
  }


  // apply on single source
  template <class Acc>
  bool sliceSampling::apply(Acc& acc,
                            const matrix<typename Acc::value_type>& img,
                            const location& loc,
                            dvector& destv) const {
  
    const parameters& par = getParameters();

    // interpolation
    static const bilinearInterpolation<typename Acc::value_type> bilin;

    //create accumulators
    static std::vector<Acc> accuVec;
    int size = par.slices;
//    normalMask_.generate(par.maskRadius,0.0,par.slices)
    if (par.overlap) {
      size *= 2;
    }
    // resize accuvector
    if (static_cast<int>(accuVec.size())!=size) {
      accuVec.resize(size);
    }

    // reset given accumulator and copy parameters
    acc.reset();
    typename std::vector<Acc>::iterator it;
    for (it = accuVec.begin();it != accuVec.end();it++) {
      it->copy(acc);
    }     

    
 
    // shortcut
    const int radius = par.maskRadius;

    // we need a transformation from the location to the image space

    const float ssina = loc.radius*sin(loc.angle)/radius;
    const float scosa = loc.radius*cos(loc.angle)/radius;

    const float maxx = static_cast<float>(img.lastColumn()) - 1.0f;
    const float maxy = static_cast<float>(img.lastRow()) - 1.0f;

    // each accumulator result has the same size
    int arraySize = accuVec.at(0).getArraySize();

    // matrix which is used to apply filter kernel
    matrix<double> result;
    if (par.overlap) {
      result.resize(par.slices*2,arraySize);
    } else {
      result.resize(par.slices,arraySize);
    }
   
    // pointer to memory
    double* dest = &(result[0][0]);

    float fy,fx,zx,zy;

    if (par.overlap) {
      // interpolation of the boundary necessary
      // this is a robust but slow implementation
      // offset for second mask
      int offset = par.slices;
      for (int y=-radius;y<=radius;++y) {
        const int o = ioPts_.at(y);
        const float sy = y*ssina;
        const float cy = y*scosa;
        for (int x=-o;x<=o;++x) {
          zx = x*scosa - sy;
          zy = x*ssina + cy;
          fx = zx + loc.position.x;
          fy = zy + loc.position.y;
          if ((fy >= 0) && (fy < maxy) &&
              (fx >= 0) && (fx < maxx)) {
            const int i = normalMask_.at(y,x);
            const int j = overlapMask_.at(y,x);
            const typename Acc::value_type& val = 
              bilin.interpolateUnchk(img,fy,fx);
            accuVec.at(i).accumulate(val,zx,zy);
            accuVec.at(j+offset).accumulate(val,zx,zy);
          } // if      
        } // for x
      } // for y

      // construct matrix
      double *p = dest; 
      // each row in the matrix is filled with the accumulator
      // result of one slice
      for (int i=0, j=0;i<par.slices;i++) {
        // the accu can write its data to the appropiate places
        // in the matrix
        accuVec.at(i).getResult(p+(j*arraySize));
        j++;
        accuVec.at(par.slices+i).getResult(p+(j*arraySize));
        j++;
      }

    } // if par.overlap
    else {
      // interpolation of the boundary necessary
      // this is a robust but slow implementation
      for (int y=-radius;y<=radius;++y) {
        const int o = ioPts_.at(y);
        const float sy = y*ssina;
        const float cy = y*scosa;
        for (int x=-o;x<=o;++x) {
          zx = x*scosa - sy;
          zy = x*ssina + cy;
          fx = zx + loc.position.x;
          fy = zy + loc.position.y;
          if ((fy >= 0) && (fy < maxy) &&
              (fx >= 0) && (fx < maxx)) {
            const int i = normalMask_.at(y,x);
            // the accu can write its data to the appropiate places
            // in the matrix            
            accuVec.at(i).accumulate(bilin.interpolateUnchk(img,fy,fx),
                                     zx,zy);
          } // if      
        } // for x
      } // for y
      double *p = dest; 
      for (int i=0;i<par.slices;i++) {
        accuVec.at(i).getResult(p+(i*arraySize));
      }
    } // else

    //transpose to sort values of accumulators
    //now the transpose function allocates new memory 
    //and after that the dest pointer gets invalid
    result.transpose();
    dest = 0;

    //eventually filter result
    if (par.filter) {
      // filter each row
      const int rows = result.rows();
      const int cols = result.columns();
      const int dsize = sizeof(double);

      // TODO: perhaps there is a more efficient way todo this
      dvector tmp;
      for (int i=0; i<rows; i++) {
        dvector& vec = result.getRow(i);
        filter_.apply(vec,tmp);
        // copy result back
        memcpy(&(vec[0]),&(tmp[0]),cols*dsize);
      }
    }

    // transfer data of the matrix to the result vector
    // the originally results of the accus is now
    // in interleaved order in the result vector
    result.detach(destv);
    
    
#ifdef _LTI_DEBUG
    double *m = new double[arraySize];
    accuVec.at(0).getResult(m);
    _lti_debug("slice0 = ("<<m[0]);
    for (int i = 1; i< arraySize;i++) {
      _lti_debug(","<<m[i]);
    }
    _lti_debug(")\n");
    delete m;
    m=0;
#endif
    
    return true;
  } // apply on single source


  // apply without location
  template <class Acc>
  bool sliceSampling::apply(Acc& acc,
                            const matrix<typename Acc::value_type>& img,
                            dvector& destv,
                            const eResultOrder& order) const {
    // image size
    ipoint isize = img.size();

    // typedef shortcut
    typedef typename Acc::value_type T;
  
    const parameters& par = getParameters();

    //create accumulators
    static std::vector<Acc> accuVec;
    int size = par.slices;

    if (par.overlap) {
      size *= 2;
    }
    // resize accuvector
    if (static_cast<int>(accuVec.size())!=size) {
      accuVec.resize(size);
    }

    // reset given accumulator and copy parameters
    acc.reset();
    typename std::vector<Acc>::iterator it;
    for (it = accuVec.begin();it != accuVec.end();it++) {
      it->copy(acc);
    }     
 
    // shortcut
    const int radius = par.maskRadius;

    // each accumulator result has the same size
    int arraySize = accuVec.at(0).getArraySize();

    // matrix which is used to apply filter kernel
    matrix<double> result;
    if (par.overlap) {
      result.resize(par.slices*2,arraySize);
    } else {
      result.resize(par.slices,arraySize);
    }

    // compute center
    const int cx = iround(isize.x/2);
    const int cy = iround(isize.y/2);
    
    // pointer to memory
    double* dest = &(result[0][0]);

    if (par.overlap) {

      // offset for second mask
      int offset = par.slices;
      for (int y=-radius;y<=radius;++y) {
        const float fy = static_cast<float>(y);
        const int o = ioPts_.at(y);
        for (int x=-o;x<=o;++x) {
          const float fx = static_cast<float>(x);
          const int tx = x + cx;
          const int ty = y + cy;
          
          const int i = normalMask_.at(y,x);
          const int j = overlapMask_.at(y,x);
          const T val = img.at(ty,tx);
          
          // accumulate
          accuVec.at(i).accumulate(val,fx,fy);
          accuVec.at(j+offset).accumulate(val,fx,fy);
       
        } // for x
      } // for y

      // construct matrix
      double *p = dest; 
      // each row in the matrix is filled with the accumulator
      // result of one slice
      for (int i=0, j=0; i<par.slices; i++) {
        // the accu can write its data to the appropiate places
        // in the matrix
        accuVec.at(i).getResult(p+(j*arraySize));
        j++;
        accuVec.at(par.slices+i).getResult(p+(j*arraySize));
        j++;
      }

    } // if par.overlap
    else {

      for (int y=-radius;y<=radius;++y) {
        const float fy = static_cast<float>(y);
        const int o = ioPts_.at(y);
        for (int x=-o;x<=o;++x) {
          const float fx = static_cast<float>(x);
          const int tx = x + cx;
          const int ty = y + cy;
          
          const int i = normalMask_.at(y,x);
          const T val = img.at(ty,tx);
          
          // accumulate
          accuVec.at(i).accumulate(val,fx,fy);
       
        } // for x
      } // for y
      
      // construct matrix
      double *p = dest; 
      for (int i=0; i<par.slices; i++) {
        // the accu can write its data to the appropiate places
        // in the matrix            
        accuVec.at(i).getResult(p+(i*arraySize));
      }

    } // else

    //transpose to sort values of accumulators
    //now the transpose function allocates new memory 
    //and after that the dest pointer gets invalid
    if ((order == Interleaved) || (par.filter)) {
      result.transpose();
      dest = 0;
    }

    // filter result if necessary
    if (par.filter) {
      // filter each row
      const int rows = result.rows();
      const int cols = result.columns();
      const int dsize = sizeof(double);
      // TODO: perhaps there is a more efficient way todo this
      dvector tmp;
      for (int i=0; i<rows; i++) {
        dvector& vec = result.getRow(i);
        filter_.apply(vec,tmp);
        // copy result back
        memcpy(&(vec[0]),&(tmp[0]),cols*dsize);
      }
      // transpose back if necessary
      if (order == Sequential) {
        result.transpose();
        dest = 0;
      }
    }

    // transfer data of the matrix to the result vector
    result.detach(destv);
    
    return true;
  }


  // apply on two sources without location
  template <class Acc>
  bool sliceSampling::apply(Acc& acc,
                            const matrix<typename Acc::value_type>& img,
                            const matrix<float>& src2,
                            dvector& destv,
                            const eResultOrder& order) const {
    // image size
    ipoint isize = img.size();

    // typedef shortcut
    typedef typename Acc::value_type T;
  
    const parameters& par = getParameters();

    //create accumulators
    static std::vector<Acc> accuVec;
    int size = par.slices;

    if (par.overlap) {
      size *= 2;
    }
    // resize accuvector
    if (static_cast<int>(accuVec.size())!=size) {
      accuVec.resize(size);
    }

    // reset given accumulator and copy parameters
    acc.reset();
    typename std::vector<Acc>::iterator it;
    for (it = accuVec.begin();it != accuVec.end();it++) {
      it->copy(acc);
    }     
 
    // shortcut
    const int radius = par.maskRadius;

    // each accumulator result has the same size
    int arraySize = accuVec.at(0).getArraySize();

    // matrix which is used to apply filter kernel
    matrix<double> result;
    if (par.overlap) {
      result.resize(par.slices*2,arraySize);
    } else {
      result.resize(par.slices,arraySize);
    }

    // compute center
    const int cx = iround(isize.x/2);
    const int cy = iround(isize.y/2);
    
    // pointer to memory
    double* dest = &(result[0][0]);

    if (par.overlap) {

      // offset for second mask
      const int offset = par.slices;
      for (int y=-radius; y<=radius; ++y) {
        const float fy = static_cast<float>(y);
        const int o = ioPts_.at(y);
        for (int x=-o; x<=o; ++x) {
          const float fx = static_cast<float>(x);
          const int tx = x + cx;
          const int ty = y + cy;
          
          const int i = normalMask_.at(y,x);
          const int j = overlapMask_.at(y,x);
          const T val = img.at(ty,tx);
          const float weight = src2.at(ty,tx);
          
          // accumulate
          accuVec.at(i).accumulate(val,weight,fx,fy);
          accuVec.at(j+offset).accumulate(val,weight,fx,fy);
       
        } // for x
      } // for y

      // construct matrix
      double *p = dest; 
      // each row in the matrix is filled with the accumulator
      // result of one slice
      for (int i=0, j=0; i<par.slices; i++) {
        // the accu can write its data to the appropiate places
        // in the matrix
        accuVec.at(i).getResult(p+(j*arraySize));
        j++;
        accuVec.at(par.slices+i).getResult(p+(j*arraySize));
        j++;
      }

    } // if par.overlap
    else {

      for (int y=-radius; y<=radius; ++y) {
        const float fy = static_cast<float>(y);
        const int o = ioPts_.at(y);
        for (int x=-o; x<=o; ++x) {
          const float fx = static_cast<float>(x);
          const int tx = x + cx;
          const int ty = y + cy;
          
          const int i = normalMask_.at(y,x);
          const T val = img.at(ty,tx);
          const float weight = src2.at(ty,tx);
          
          // accumulate
          accuVec.at(i).accumulate(val,weight,fx,fy);
       
        } // for x
      } // for y
      
      // construct matrix
      double *p = dest; 
      for (int i=0;i<par.slices;i++) {
        // the accu can write its data to the appropiate places
        // in the matrix            
        accuVec.at(i).getResult(p+(i*arraySize));
      }

    } // else

    //transpose to sort values of accumulators
    //now the transpose function allocates new memory 
    //and after that the dest pointer gets invalid
    if ((order == Interleaved) || (par.filter)) {
      result.transpose();
      dest = 0;
    }

    // filter result if necessary
    if (par.filter) {
      // filter each row
      const int rows = result.rows();
      const int cols = result.columns();
      const int dsize = sizeof(double);
      // TODO: perhaps there is a more efficient way todo this
      dvector tmp;
      for (int i=0;i<rows;i++) {
        dvector& vec = result.getRow(i);
        filter_.apply(vec,tmp);
        // copy result back
        memcpy(&(vec[0]),&(tmp[0]),cols*dsize);
      }
      // transpose back if necessary
      if (order == Sequential) {
        result.transpose();
        dest = 0;
      }
    }

    // transfer data of the matrix to the result vector
    result.detach(destv);
    
    return true;
  }

  // apply on two sources
  template <class Acc>
  bool sliceSampling::apply(Acc& acc,
                            const matrix<typename Acc::value_type>& img,
                            const matrix<float>& src2,
                            const location& loc,
                            dvector& destv) const {

    // typedef shortcut
    typedef typename Acc::value_type T;
  
    const parameters& par = getParameters();

    // interpolation
    const static bilinearInterpolation<T> bilin;
    const static nearestNeighborInterpolation<T> nearest;

    //create accumulators
    static std::vector<Acc> accuVec;
    int size = par.slices;
//    normalMask_.generate(par.maskRadius,0.0,par.slices)
    if (par.overlap) {
      size *= 2;
    }
    // resize accuvector
    if (static_cast<int>(accuVec.size())!=size) {
      accuVec.resize(size);
    }

    // reset given accumulator and copy parameters
    acc.reset();
    typename std::vector<Acc>::iterator it;
    for (it = accuVec.begin();it != accuVec.end();it++) {
      it->copy(acc);
    }     
 
    // shortcut
    const int radius = par.maskRadius;

    // we need a transformation from the location to the image space

    const float ssina = loc.radius*sin(loc.angle)/radius;
    const float scosa = loc.radius*cos(loc.angle)/radius;

    const float maxx = static_cast<float>(img.lastColumn()) - 1.0f;
    const float maxy = static_cast<float>(img.lastRow()) - 1.0f;

    // each accumulator result has the same size
    int arraySize = accuVec.at(0).getArraySize();

    // matrix which is used to apply filter kernel
    matrix<double> result;
    if (par.overlap) {
      result.resize(par.slices*2,arraySize);
    } else {
      result.resize(par.slices,arraySize);
    }
   
    // pointer to memory
    double* dest = &(result[0][0]);

    float fy,fx,zx,zy;

    if (par.overlap) {
      // interpolation of the boundary necessary
      // this is a robust but slow implementation
      // offset for second mask
      const int offset = par.slices;
      for (int y=-radius;y<=radius;++y) {
        const int o = ioPts_.at(y);
        const float sy = y*ssina;
        const float cy = y*scosa;
        for (int x=-o; x<=o; ++x) {
          zx = x*scosa - sy;
          zy = x*ssina + cy;
          fx = zx + loc.position.x;
          fy = zy + loc.position.y;
          if ((fy >= 0) && (fy < maxy) &&
              (fx >= 0) && (fx < maxx)) {
            const int i = normalMask_.at(y,x);
            const int j = overlapMask_.at(y,x);
            // TODO: optimize this
            T val;
            if (par.keyIsOrientation) {
              val = bilin.interpolateUnchk(img,fy,fx);
              clipAngle(val,loc.angle);
            } else {
              val = nearest.interpolateUnchk(img,fy,fx);
            }
            const float weight = bilin.interpolateUnchk(src2,fy,fx);

            // accumulate
            accuVec.at(i).accumulate(val,weight,zx,zy);
            accuVec.at(j+offset).accumulate(val,weight,zx,zy);
          } // if      
        } // for x
      } // for y

      // construct matrix
      double *p = dest; 
      // each row in the matrix is filled with the accumulator
      // result of one slice
      for (int i=0, j=0; i<par.slices; i++) {
        // the accu can write its data to the appropiate places
        // in the matrix
        accuVec.at(i).getResult(p+(j*arraySize));
        j++;
        accuVec.at(par.slices+i).getResult(p+(j*arraySize));
        j++;
      }

    } // if par.overlap
    else {
      // interpolation of the boundary necessary
      // this is a robust but slow implementation
      for (int y=-radius; y<=radius; ++y) {
        const int o = ioPts_.at(y);
        const float sy = y*ssina;
        const float cy = y*scosa;
        for (int x=-o; x<=o; ++x) {
          zx = x*scosa - sy;
          zy = x*ssina + cy;
          fx = zx + loc.position.x;
          fy = zy + loc.position.y;
          if ((fy >= 0) && (fy < maxy) &&
              (fx >= 0) && (fx < maxx)) {
            const int i = normalMask_.at(y,x);

            // TODO: optimize this
            T val;
            if (par.keyIsOrientation) {
              val = bilin.interpolateUnchk(img,fy,fx);
              clipAngle(val,loc.angle);
            } else {
              val = nearest.interpolateUnchk(img,fy,fx);
            }
            const float weight = bilin.interpolateUnchk(src2,fy,fx);

            // accumulate
            accuVec.at(i).accumulate(val,
                                     weight,
                                     zx,zy);
          } // if      
        } // for x
      } // for y

      // construct matrix
      double *p = dest; 
      for (int i=0;i<par.slices;i++) {
        // the accu can write its data to the appropiate places
        // in the matrix            
        accuVec.at(i).getResult(p+(i*arraySize));
      }
    } // else

    //transpose to sort values of accumulators
    //now the transpose function allocates new memory 
    //and after that the dest pointer gets invalid
    result.transpose();
    dest = 0;

    //eventually filter result
    if (par.filter) {
      // filter each row
      const int rows = result.rows();
      const int cols = result.columns();
      const int dsize = sizeof(double);
      // TODO: perhaps there is a more efficient way todo this
      dvector tmp;
      for (int i=0;i<rows;i++) {
        dvector& vec = result.getRow(i);
        filter_.apply(vec,tmp);
        // copy result back
        memcpy(&(vec[0]),&(tmp[0]),cols*dsize);
      }
    }

    // transfer data of the matrix to the result vector
    // the originally results of the accus is now
    // in interleaved order in the result vector
    result.detach(destv);
    
    
#ifdef _LTI_DEBUG
    double *m = new double[arraySize];
    accuVec.at(0).getResult(m);
    _lti_debug("slice0 = ("<<m[0]);
    for (int i = 1; i< arraySize;i++) {
      _lti_debug(","<<m[i]);
    }
    _lti_debug(")\n");
    delete m;
    m=0;
#endif
    
    return true;
  } // apply on two sources  


  // apply on lists for one matrix
  template <class Acc>
  bool sliceSampling::apply(Acc& acc,
                            const matrix<typename Acc::value_type>& img,
                            const list<location>& locs,
                            std::list<dvector>& dests) const {

    const parameters& par = getParameters();

    // interpolation
    const static bilinearInterpolation<typename Acc::value_type> bilin;

    // create accumulators
    static std::vector<Acc> accuVec;
    int size = par.slices;
    // normalMask_.generate(par.maskRadius,0.0,par.slices)
    if (par.overlap) {
      size *= 2;
    }
    // resize accuvector
    if (static_cast<int>(accuVec.size()) != size) {
      accuVec.resize(size);
    }

    // copy parameters to each accumulator
    typename std::vector<Acc>::iterator it;
    for (it = accuVec.begin();it != accuVec.end();it++) {
      it->copy(acc);
    }     
    
    // iterator to locations
    list<location>::const_iterator lit= locs.begin();

    // shortcut
    const int radius = par.maskRadius;
    
    // local variables
    float fy,fx,zx,zy;

    // clear destination list
    dests.clear();

    // each accumulator result has the same size
    const int arraySize = accuVec.at(0).getArraySize();


    const float maxx = static_cast<float>(img.lastColumn()) - 1.0f;
    const float maxy = static_cast<float>(img.lastRow()) - 1.0f;

    if (par.overlap) {
      for (;lit!=locs.end();++lit) {
        // reset all accus;
        
        for (it = accuVec.begin();it != accuVec.end();it++) {
          it->reset();
        }     

        // add result vector to list
        // this is done here to avoid the copy of data at insertion
        dests.push_back(dvector());
                
        // we need a transformation from the location to the image space
        
        const float ssina = lit->radius*sin(lit->angle)/radius;
        const float scosa = lit->radius*cos(lit->angle)/radius;
                      
        // matrix which is used to apply filter kernel
        matrix<double> result(par.slices*2,arraySize);

        // pointer to memory
        double* dest = &(result[0][0]);
        
        // offset for second mask
        const int offset = par.slices;

        for (int y=-radius; y<=radius; ++y) {
          const int o = ioPts_.at(y);
          const float sy = y*ssina;
          const float cy = y*scosa;
          for (int x=-o; x<=o; ++x) {
            zx = x*scosa - sy;
            zy = x*ssina + cy;
            fx = zx + lit->position.x;
            fy = zy + lit->position.y;
            if ((fy >= 0) && (fy < maxy) &&
                (fx >= 0) && (fx < maxx)) {
              const int i = normalMask_.at(y,x);
              const int j = overlapMask_.at(y,x);
              const typename Acc::value_type& val = 
                bilin.interpolateUnchk(img,fy,fx);
              accuVec.at(i).accumulate(val,zx,zy);
              accuVec.at(j+offset).accumulate(val,zx,zy);
            } // if      
          } // for x
        } // for y

        // construct matrix
        double *p = dest; 
        // each row in the matrix is filled with the accumulator
        // result of one slice
        for (int i=0, j=0; i<par.slices; i++) {
          // the accu can write its data to the appropiate places
          // in the matrix
          accuVec.at(i).getResult(p+(j*arraySize));
          j++;
          accuVec.at(par.slices+i).getResult(p+(j*arraySize));
          j++;
        }

        //transpose to sort values of accumulators
        //now the transpose function allocates new memory 
        //and after that the dest pointer gets invalid
        result.transpose();
        dest = 0;
        
        //eventually filter result
        if (par.filter) {
          // filter each row
          const int rows = result.rows();
          const int cols = result.columns();
          const int dsize = sizeof(double);
          // TODO: perhaps there is a more efficient way todo this
          dvector tmp;
          for (int i=0;i<rows;i++) {
            dvector& vec = result.getRow(i);
            filter_.apply(vec,tmp);
            // copy result back
            memcpy(&(vec[0]),&(tmp[0]),cols*dsize);
          }
        }

        // transfer data of the matrix to the result vector
        // the originally results of the accus is now
        // in interleaved order in the result vector
        result.detach(dests.back());
    
      } // for locations
    } // if overlap 
    else {
      for (;lit!=locs.end();++lit) {
        // reset all accus;
        for (it = accuVec.begin();it != accuVec.end();it++) {
          it->reset();
        }     

        // add result vector to list
        // this is done here to avoid the copy of data at insertion
        dests.push_back(dvector());
                
        // we need a transformation from the location to the image space
        
        const float ssina = lit->radius*sin(lit->angle)/radius;
        const float scosa = lit->radius*cos(lit->angle)/radius;
                      
        // matrix which is used to apply filter kernel
        matrix<double> result(par.slices,arraySize);

        // pointer to memory
        double* dest = &(result[0][0]);
        
        for (int y=-radius; y<=radius; ++y) {
          const int o = ioPts_.at(y);
          const float sy = y*ssina;
          const float cy = y*scosa;
          for (int x=-o; x<=o; ++x) {
            zx = x*scosa - sy;
            zy = x*ssina + cy;
            fx = zx + lit->position.x;
            fy = zy + lit->position.y;
            if ((fy >= 0) && (fy < maxy) &&
                (fx >= 0) && (fx < maxx)) {
              const int i = normalMask_.at(y,x);
              const typename Acc::value_type& val = 
                bilin.interpolateUnchk(img,fy,fx);
              accuVec.at(i).accumulate(val,zx,zy);
            } // if      
          } // for x
        } // for y

        // construct matrix
        double *p = dest; 
        // each row in the matrix is filled with the accumulator
        // result of one slice
        for (int i=0; i<par.slices; i++) {
          // the accu can write its data to the appropiate places
          // in the matrix
          accuVec.at(i).getResult(p+(i*arraySize));
        }

        //transpose to sort values of accumulators
        //now the transpose function allocates new memory 
        //and after that the dest pointer gets invalid
        result.transpose();
        dest = 0;
        
        //eventually filter result
        if (par.filter) {
          // filter each row
          const int rows = result.rows();
          const int cols = result.columns();
          const int dsize = sizeof(double);
          // TODO: perhaps there is a more efficient way todo this
          dvector tmp;
          for (int i=0; i<rows; i++) {
            dvector& vec = result.getRow(i);
            filter_.apply(vec,tmp);
            // copy result back
            memcpy(&(vec[0]),&(tmp[0]),cols*dsize);
          }
        }

        // transfer data of the matrix to the result vector
        // the originally results of the accus is now
        // in interleaved order in the result vector
        result.detach(dests.back());
    
      } // for locations
    }
    return true;
  }

  // apply on lists for two matrices
  template <class Acc>
  bool sliceSampling::apply(Acc& acc,
                            const matrix<typename Acc::value_type>& img,
                            const matrix<float>& src2,
                            const list<location>& locs,
                            std::list<dvector>& dests) const {
    const parameters& par = getParameters();

    // interpolation
    const static bilinearInterpolation<typename Acc::value_type> bilin;

    //create accumulators
    static std::vector<Acc> accuVec;
    int size = par.slices;

    if (par.overlap) {
      size *= 2;
    }
    // resize accuvector
    if (static_cast<int>(accuVec.size())!=size) {
      accuVec.resize(size);
    }

    // copy parameters to each accumulator
    typename std::vector<Acc>::iterator it;
    for (it = accuVec.begin();it != accuVec.end();it++) {
      it->copy(acc);
    }     
    
    // iterator to locations
    list<location>::const_iterator lit= locs.begin();

    // shortcut
    const int radius = par.maskRadius;
    
    // local variables
    float fy,fx,zx,zy;

    // clear destination list
    dests.clear();

    // each accumulator result has the same size
    const int arraySize = accuVec.at(0).getArraySize();


    const float maxx = static_cast<float>(img.lastColumn()) - 1.0f;
    const float maxy = static_cast<float>(img.lastRow()) - 1.0f;

    if (par.overlap) {
      for (;lit!=locs.end();++lit) {
        // reset all accus;
        for (it = accuVec.begin();it != accuVec.end();it++) {
          it->reset();
        }     

        // add result vector to list
        // this is done here to avoid the copy of data at insertion
        dests.push_back(dvector());
                
        // we need a transformation from the location to the image space
        
        const float ssina = lit->radius*sin(lit->angle)/radius;
        const float scosa = lit->radius*cos(lit->angle)/radius;
                      
        // matrix which is used to apply filter kernel
        matrix<double> result(par.slices*2,arraySize);

        // pointer to memory
        double* dest = &(result[0][0]);
        
        // offset for second mask
        const int offset = par.slices;

        for (int y=-radius; y<=radius; ++y) {
          const int o = ioPts_.at(y);
          const float sy = y*ssina;
          const float cy = y*scosa;
          for (int x=-o; x<=o; ++x) {
            zx = x*scosa - sy;
            zy = x*ssina + cy;
            fx = zx + lit->position.x;
            fy = zy + lit->position.y;
            if ((fy >= 0) && (fy < maxy) &&
                (fx >= 0) && (fx < maxx)) {
              const int i = normalMask_.at(y,x);
              const int j = overlapMask_.at(y,x);
              const typename Acc::value_type& val = 
                bilin.interpolateUnchk(img,fy,fx);
              const float weight = bilin.interpolateUnchk(src2,fy,fx);
              accuVec.at(i).accumulate(val,weight,zx,zy);
              accuVec.at(j+offset).accumulate(val,weight,zx,zy);
            } // if      
          } // for x
        } // for y

        // construct matrix
        double *p = dest; 
        // each row in the matrix is filled with the accumulator
        // result of one slice
        for (int i=0, j=0; i<par.slices; i++) {
          // the accu can write its data to the appropiate places
          // in the matrix
          accuVec.at(i).getResult(p+(j*arraySize));
          j++;
          accuVec.at(par.slices+i).getResult(p+(j*arraySize));
          j++;
        }

        //transpose to sort values of accumulators
        //now the transpose function allocates new memory 
        //and after that the dest pointer gets invalid
        result.transpose();
        dest = 0;
        
        //eventually filter result
        if (par.filter) {
          // filter each row
          const int rows = result.rows();
          const int cols = result.columns();
          const int dsize = sizeof(double);
          // TODO: perhaps there is a more efficient way todo this
          dvector tmp;
          for (int i=0;i<rows;i++) {
            dvector& vec = result.getRow(i);
            filter_.apply(vec,tmp);
            // copy result back
            memcpy(&(vec[0]),&(tmp[0]),cols*dsize);
          }
        }

        // transfer data of the matrix to the result vector
        // the originally results of the accus is now
        // in interleaved order in the result vector
        result.detach(dests.back());
    
      } // for locations
    } // if overlap 
    else {
      for (;lit!=locs.end();++lit) {
        // reset all accus;
        for (it = accuVec.begin();it != accuVec.end();it++) {
          it->reset();
        }     

        // add result vector to list
        // this is done here to avoid the copy of data at insertion
        dests.push_back(dvector());
                
        // we need a transformation from the location to the image space
        
        const float ssina = lit->radius*sin(lit->angle)/radius;
        const float scosa = lit->radius*cos(lit->angle)/radius;
                      
        // matrix which is used to apply filter kernel
        matrix<double> result(par.slices,arraySize);

        // pointer to memory
        double* dest = &(result[0][0]);
        
        for (int y=-radius; y<=radius; ++y) {
          const int o = ioPts_.at(y);
          const float sy = y*ssina;
          const float cy = y*scosa;
          for (int x=-o; x<=o; ++x) {
            zx = x*scosa - sy;
            zy = x*ssina + cy;
            fx = zx + lit->position.x;
            fy = zy + lit->position.y;
            if ((fy >= 0) && (fy < maxy) &&
                (fx >= 0) && (fx < maxx)) {
              const int i = normalMask_.at(y,x);
              const typename Acc::value_type& val = 
                bilin.interpolateUnchk(img,fy,fx);
              const float weight = bilin.interpolateUnchk(src2,fy,fx);
              accuVec.at(i).accumulate(val,weight,zx,zy);
            } // if      
          } // for x
        } // for y

        // construct matrix
        double *p = dest; 
        // each row in the matrix is filled with the accumulator
        // result of one slice
        for (int i=0, j=0; i<par.slices; i++) {
          // the accu can write its data to the appropiate places
          // in the matrix
          accuVec.at(i).getResult(p+(i*arraySize));
        }

        //transpose to sort values of accumulators
        //now the transpose function allocates new memory 
        //and after that the dest pointer gets invalid
        result.transpose();
        dest = 0;
        
        //eventually filter result
        if (par.filter) {
          // filter each row
          const int rows = result.rows();
          const int cols = result.columns();
          const int dsize = sizeof(double);
          // TODO: perhaps there is a more efficient way todo this
          dvector tmp;
          for (int i=0; i<rows; i++) {
            dvector& vec = result.getRow(i);
            filter_.apply(vec,tmp);
            // copy result back
            memcpy(&(vec[0]),&(tmp[0]),cols*dsize);
          }
        }

        // transfer data of the matrix to the result vector
        // the originally results of the accus is now
        // in interleaved order in the result vector
        result.detach(dests.back());
    
      } // for locations
    } // apply on lists for two matrices
    
    return true;
  }

  template<class Acc>
  bool sliceSampling::apply(Acc& acc,
                            const matrix<typename Acc::value_type>& src,
                            const list<location>& locs,
                            std::list<dvector>& dest,
                            const eResultOrder& order) const {
    
    bool rc = apply(acc,src,locs,dest);
    if (order==Sequential) {
      std::list<dvector>::iterator it;
      for (it=dest.begin();it!=dest.end();++it) {
        switchOrder(*it);
      }
    }
    return rc;
  }

}
