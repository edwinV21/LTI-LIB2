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
 * \file   ltiSurfLocalDescriptor.cpp
 *         Contains the class lti::surfLocalDescriptor, 
 *         which computes local descriptors with the SURF approach.
 * \author Pablo Alvarado
 * \date   12.11.2007
 *
 * revisions ..: $Id: ltiSurfLocalDescriptor.cpp,v 1.4 2010-02-09 15:55:32 alvarado Exp $
 */

#include "ltiSurfLocalDescriptor.h"
#include "ltiIntegralImage.h"
#include "ltiTypeInfo.h"
#include "ltiRound.h"
#include "ltiConstants.h"
#include "ltiArctanLUT.h"

#include "ltiMatrixTransform.h"
#include "ltiBilinearInterpolation.h"

#include <vector>
#include <limits>

#undef _LTI_DEBUG
//#define _LTI_DEBUG 1

#ifdef _LTI_DEBUG
#include "ltiViewer2D.h"
#define _LTI_DEBUG 1
#endif
#include "ltiDebug.h"


namespace lti {

  // --------------------------------------------------
  // surfLocalDescriptor::parameters
  // --------------------------------------------------

  // default constructor
  surfLocalDescriptor::parameters::parameters()
    : functor::parameters() {

    orientationMode = Approximate;
    orientationClusters = 12;
    waveletSize = int(2);
    signSplit = bool(false);
    gaussianWeight = float(3.5);
    numberOfSubregions = int(4);
    subregionSamples = int(5);
    normalize = true;
    boundaryType = Constant;
  }

  // copy constructor
  surfLocalDescriptor::parameters::parameters(const parameters& other)
    : functor::parameters() {
    copy(other);
  }

  // destructor
  surfLocalDescriptor::parameters::~parameters() {
  }

  // copy member

  surfLocalDescriptor::parameters&
  surfLocalDescriptor::parameters::copy(const parameters& other) {
    functor::parameters::copy(other);

    orientationMode = other.orientationMode;
    orientationClusters = other.orientationClusters;
    waveletSize = other.waveletSize;
    signSplit = other.signSplit;
    gaussianWeight = other.gaussianWeight;
    numberOfSubregions = other.numberOfSubregions;
    subregionSamples = other.subregionSamples;
    normalize = other.normalize;
    boundaryType = other.boundaryType;
    return *this;
  }

  // alias for copy method
  surfLocalDescriptor::parameters&
  surfLocalDescriptor::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  // class name
  const std::string& surfLocalDescriptor::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone method
  surfLocalDescriptor::parameters*
  surfLocalDescriptor::parameters::clone() const {
    return new parameters(*this);
  }

  // new instance
  surfLocalDescriptor::parameters*
  surfLocalDescriptor::parameters::newInstance() const {
    return new parameters();
  }

  /*
   * Write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool surfLocalDescriptor::parameters::write(ioHandler& handler,
                                         const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {
      lti::write(handler,"orientationMode",orientationMode);
      lti::write(handler,"orientationClusters",orientationClusters);
      lti::write(handler,"waveletSize",waveletSize);
      lti::write(handler,"signSplit",signSplit);
      lti::write(handler,"gaussianWeight",gaussianWeight);
      lti::write(handler,"numberOfSubregions",numberOfSubregions);
      lti::write(handler,"subregionSamples",subregionSamples);
      lti::write(handler,"normalize",normalize);
      lti::write(handler,"boundaryType",boundaryType);
    }

    b = b && functor::parameters::write(handler,false);

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
  bool surfLocalDescriptor::parameters::read(ioHandler& handler,
                                        const bool complete)  {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {
      lti::read(handler,"orientationMode",orientationMode);     
      lti::read(handler,"orientationClusters",orientationClusters);
      lti::read(handler,"waveletSize",waveletSize);
      lti::read(handler,"signSplit",signSplit);
      lti::read(handler,"gaussianWeight",gaussianWeight);
      lti::read(handler,"numberOfSubregions",numberOfSubregions);
      lti::read(handler,"subregionSamples",subregionSamples);
      lti::read(handler,"normalize",normalize);
      lti::read(handler,"boundaryType",boundaryType);
    }

    b = b && functor::parameters::read(handler,false);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

  // --------------------------------------------------
  // surfLocalDescriptor
  // --------------------------------------------------

  // default constructor
  surfLocalDescriptor::surfLocalDescriptor()
    : functor() {

    // create an instance of the parameters with the default values
    parameters defaultParameters;
    // set the default parameters
    setParameters(defaultParameters);
  }

  // default constructor
  surfLocalDescriptor::surfLocalDescriptor(const parameters& par)
    : functor() {

    // set the given parameters
    setParameters(par);
  }


  // copy constructor
  surfLocalDescriptor::surfLocalDescriptor(const surfLocalDescriptor& other)
    : functor() {
    copy(other);
  }

  // destructor
  surfLocalDescriptor::~surfLocalDescriptor() {
  }

  // copy member
  surfLocalDescriptor& 
  surfLocalDescriptor::copy(const surfLocalDescriptor& other) {
    functor::copy(other);

    return (*this);
  }

  // alias for copy member
  surfLocalDescriptor&
  surfLocalDescriptor::operator=(const surfLocalDescriptor& other) {
    return (copy(other));
  }

  // class name
  const std::string& surfLocalDescriptor::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone member
  surfLocalDescriptor* surfLocalDescriptor::clone() const {
    return new surfLocalDescriptor(*this);
  }

  // create a new instance
  surfLocalDescriptor* surfLocalDescriptor::newInstance() const {
    return new surfLocalDescriptor();
  }

  // return parameters
  const surfLocalDescriptor::parameters&
  surfLocalDescriptor::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }

  bool surfLocalDescriptor::updateParameters() {
    // ensure the initialization of the arctan LUT
    arctanLUT atanLUT;
    
    if (functor::updateParameters()) {
      const parameters& param = getParameters();
      const int winSize = param.numberOfSubregions * param.subregionSamples;
      float w = float(winSize)/2.0f;

      gaussian_.allocate(winSize,winSize);
      for (int y=0;y<winSize;++y) {
        for (int x=0;x<winSize;++x) {
          gaussian_.at(y,x)=exp(-0.5f*(sqr(x-w)+sqr(y-w))/
                                sqr(param.gaussianWeight));
        }
      }
    }
    return true;
  }

  // -------------------------------------------------------------------
  // The apply() member functions
  // -------------------------------------------------------------------
  inline float surfLocalDescriptor::adist(const float a,const float b) const {
    const float tmp = abs(a-b);
    if (tmp > constants<float>::pi()) {
      return constants<float>::twoPi()-tmp;
    }
    return tmp;
  }

  inline float surfLocalDescriptor::sadist(const float a,const float b) const {
    const float tmp = b-a;
    if (tmp < -constants<float>::pi()) {
      return constants<float>::twoPi()+tmp;
    } 
    if (tmp > constants<float>::pi()) {
      return tmp-constants<float>::twoPi();
    }
    return tmp;
  }

  void surfLocalDescriptor::stats(const fvector& theSin,
                                  const fvector& theCos,
                                  const ivector& theLocs,
                                  const int n,
                                  float& meanSin,
                                  float& meanCos,
                                  float& varSin,
                                  float& varCos) const {

    meanSin=meanCos=varSin=varCos = 0.0f;
    for (int k=0;k<n;++k) {
      const int l = theLocs.at(k);
      meanCos+=theCos.at(l);
      meanSin+=theSin.at(l);
      
      varCos+=sqr(theCos.at(l));
      varSin+=sqr(theSin.at(l));
    }
    
    // the variances
    varCos = (varCos - sqr(meanCos)/n)/n;
    varSin = (varSin - sqr(meanSin)/n)/n;
    
    // these are the mean sine and cosine
    meanCos /= n;
    meanSin /= n;
  }

  bool surfLocalDescriptor::cluster(const list<location>& locs,
                                    std::vector<location>& refs,
                                    imatrix& clusters,
                                    fvector& angles,
                                    fvector& sines,
                                    fvector& cosines,
                                    ivector& numLocs) const {
    const parameters& param = getParameters();
    if (locs.empty()) {
      return true;
    }

    // fast computation of angles
    arctanLUT atanLUT;

    const int totalLocs = locs.size();

    // we cannot have more clusters than locations
    const int numClusters = min(totalLocs,param.orientationClusters);

    refs.resize(totalLocs);
    angles.allocate(numClusters);
    numLocs.assign(numClusters,0);

    fvector theCos(totalLocs);
    fvector theSin(totalLocs);

    list<location>::const_iterator it,eit;
    int i;
    for (i=0,it=locs.begin(),eit=locs.end();it!=eit;++it,++i) {
      refs[i].copy(*it);
      sincos((*it).angle,theSin.at(i),theCos.at(i));
    }

    // instead of lists, we will work with indices to the refs
    // each row will represent a cluster, the columns will contain
    // the references or -1 if no element belongs there.
    clusters.assign(numClusters,totalLocs,-1);

    // values used to compute mean and variance of each cluster
    // they will be computed in cartesian coordinates.
    cosines.assign(numClusters,0.0f);
    sines.assign(numClusters,0.0f);
    fvector sumC2(numClusters,0.0f);
    fvector sumS2(numClusters,0.0f);

    // initialization with regularly distributed angles
    for (i=0;i<numClusters;++i) {
      angles.at(i)=constants<float>::twoPi()*i/numClusters;
    }

    // now sort the vectors to the nearest list
    const float slice = constants<float>::pi()/numClusters;
    for (i=0;i<totalLocs;++i) {
      // since initialization with regular distribution, we can directly
      // check which angle this loc belongs to

      // assume all angles are between -2pi..2pi, but normalize it to be
      // always positive between 0 and 2pi
      float& nangle = refs[i].angle;
      while (nangle>constants<float>::twoPi()) {
        nangle-=constants<float>::twoPi();
      }
      while (nangle<0) {
        nangle+=constants<float>::twoPi();
      }

      const int idx = static_cast<int>((nangle+slice) *
                                       numClusters/constants<float>::twoPi()) 
                      % numClusters;

      cosines.at(idx)+=theCos.at(i);
      sines.at(idx)+=theSin.at(i);
      
      sumC2.at(idx)+=sqr(theCos.at(i));
      sumS2.at(idx)+=sqr(theSin.at(i));

      clusters.at(idx,numLocs.at(idx)++)=i;
    }

    
    // convert the sumX and sumX2 in mean and variance respectively
    for (i=0;i<numClusters;++i) {
      const int n = numLocs.at(i);
      if (n != 0) {
        // the variances
        sumC2.at(i) = (sumC2.at(i) - (sqr(cosines.at(i))/n))/n;
        sumS2.at(i) = (sumS2.at(i) - (sqr(sines.at(i))/n))/n;

        // these are the mean sine and cosine
        cosines.at(i) /= n;
        sines.at(i) /= n;
      }
    }

    // to continue with the initialization, we need to check if there is a
    // cluster that can be split
    for (i=0;i<numClusters;++i) {
      if (numLocs.at(i) == 0) {
        // we have an empty cluster.

        // try to find the best cluster to be split
        int numLocsCand = -1;
        int cand = -1;
        for (int j=0;j<numClusters;++j) {
          if ((numLocs.at(j) > 0) &&
              (sumC2.at(j) > std::numeric_limits<float>::epsilon()) &&
              (sumS2.at(j) > std::numeric_limits<float>::epsilon())) {
            // we have it! The variances of cluster j are not zero, so we can
            // split it
            if (numLocs.at(j) > numLocsCand) {
              numLocsCand = numLocs.at(j);
              cand = j;
            }
          }
        }
        // cand has the cluster with most elements that can be split
        if (cand >=0) {
          const float jangle = atanLUT(sines.at(cand),cosines.at(cand));
          // Take all values less than the mean and put them
          // into cluster i
          for (int k=0;k<numLocs.at(cand);++k) {
            if (sadist(refs[clusters.at(cand,k)].angle,jangle) < 0.0f) {
              clusters.at(i,numLocs.at(i)++) = clusters.at(cand,k);
              // move the last element to the current position and
              // indicate that the cand cluster has one element less.
              clusters.at(cand,k)=clusters.at(cand,--numLocs.at(cand));
            }
          }

          // recompute the mean and variances of the cand and i clusters
          stats(theSin,theCos,clusters.getRow(cand),numLocs.at(cand),
                sines.at(cand),cosines.at(cand),sumS2.at(cand),sumC2.at(cand));

          stats(theSin,theCos,clusters.getRow(i),numLocs.at(i),
                sines.at(i),cosines.at(i),sumS2.at(i),sumC2.at(i));

        } else {
          // no split of any further cluster possible, abort the mission
          break;
        }        
      }
    }


    // at last, we can compute the mean angles of each cluster
    for (i=0;i<numClusters;++i) {
      angles.at(i)=atanLUT(sines.at(i),cosines.at(i));
    }

    // that was a long initialization!  
    // now we can go on with the kmeans algorithm
    int maxIter = 10; // avoid too long convergence loops!
    int numChanges;
    do {
      numChanges = 0;

      // for each cluster i
      for (i=0;i<numClusters;++i) {
        // for each location assigned to cluster i
        // note that the numLocs may change!
        for (int j=0;j<numLocs.at(i);++j) {
          // The angle of the location
          const float aloc = refs[clusters.at(i,j)].angle;

          float minDist = adist(aloc,angles.at(i));
          int bestCluster = i;
          // check if the location better fits into another cluster k
          for (int k=0;k<numClusters;++k) {
            float tmp = adist(refs[clusters.at(i,j)].angle,angles.at(i));
            if (tmp<minDist) {
              bestCluster = k;
              minDist = tmp;
            }
          }

          if (bestCluster != i) {
            // oh no! we have a change
            numChanges++;
            // move the location to its new place
            clusters.at(bestCluster,numLocs.at(bestCluster)++)=
              clusters.at(i,j);
            clusters.at(i,j) = clusters.at(i,--numLocs.at(i));
          }
        }
      }

      if (numChanges > 0) {
        // now we have to update the stats of each cluster
        for (i=0;i<numClusters;++i) {
          stats(theSin,theCos,clusters.getRow(i),numLocs.at(i),
                sines.at(i),cosines.at(i),sumS2.at(i),sumC2.at(i));
          angles.at(i)=atanLUT(sines.at(i),cosines.at(i));
        }
      }

      --maxIter;
    } while((numChanges>0)&&(maxIter>0));
    
    return true;
  }
  
  template<typename T>
  bool surfLocalDescriptor::helper(const matrix<T>& src,
                                   const list<location>& locs,
                                   std::list<dvector>& desc) const {
    const parameters& param = getParameters();
    switch(param.orientationMode) {
      case Ignore:
        return helperIgnore(src,locs,desc);
        break;
      case Approximate:
        return helperApprox(src,locs,desc);
        break;
      case Cluster:
        return helperCluster(src,locs,desc);
        break;
      default:
        return false;
    }

    return true;
  }

    /*
     * Base class for blocks
     */
  class surfLocalDescriptor::block {
  public:
    /*
     * Constructor
     */
    block() {};
    
    /*
     * Destructor
     */
    virtual ~block() {};
    
    
    /*
     * Accumulate
     */
    virtual void acc(const float dx,
                     const float dy) = 0;
    
    /*
     * Reset
     */
    virtual void reset() = 0;
    
    /*
     * Upload the data into the vector
     */
    virtual void load(int& idx,
                      dvector& vct) = 0;
  };
  
  /*
   * Normal block
   */
  class surfLocalDescriptor::block4 : public surfLocalDescriptor::block {
  protected:
    float accdx;
    float accdy;
    float accadx;
    float accady;
    
  public:
    /*
     * Constructor
     */
    block4() {};
    
    /*
     * Destructor
     */
    virtual ~block4() {};
    
    
    /*
     * Accumulate
     */
    virtual void acc(const float dx,
                     const float dy) {
      accdx  += dx;
      accdy  += dy;
      accadx += abs(dx);
      accady += abs(dy);
    }
    
    /*
     * Reset
     */
    virtual void reset() {
      accdx  = 0.0f;
      accdy  = 0.0f;
      accadx = 0.0f;
      accady = 0.0f;
    }
    
    /*
     * Upload the data into the vector
     */
    virtual void load(int& idx,
                      dvector& vct) {
      vct.at(idx++)=accdx;
      vct.at(idx++)=accadx;
      vct.at(idx++)=accdy;
      vct.at(idx++)=accady;
    }
  };
  
  
  /*
   * Extended block
   */
  class surfLocalDescriptor::block8 : public surfLocalDescriptor::block {
  protected:
    float paccdx;
    float paccdy;
    float paccadx;
    float paccady;
    
    float naccdx;
    float naccdy;
    float naccadx;
    float naccady;
    
  public:
    /*
     * Constructor
     */
    block8() {};
    
    /*
     * Destructor
     */
    virtual ~block8() {};
    
    
    /*
     * Accumulate
     */
    virtual void acc(const float dx,
                     const float dy) {
      
      if (dy<0.0) {
        naccdx += dx;
        naccadx+= abs(dx);
      } else {
        paccdx += dx;
        paccadx+= abs(dx);
      }
      
      if (dx<0.0) {
        naccdy += dy;
        naccady+= abs(dy);
      } else {
        paccdy += dy;
        paccady+= abs(dy);
      }
    }
    
    /*
     * Reset
     */
    virtual void reset() {
      paccdx  = 0.0f;
      paccadx = 0.0f;
      naccdx  = 0.0f;
      naccadx = 0.0f;
      
      paccdy  = 0.0f;
      paccady = 0.0f;
      naccdy  = 0.0f;
      naccady = 0.0f;
      
    }
    
    /*
     * Upload the data into the vector
     */
    virtual void load(int& idx,
                      dvector& vct) {
      vct.at(idx++)=paccdx;
      vct.at(idx++)=paccdy;
      vct.at(idx++)=paccadx;
      vct.at(idx++)=paccady;
      
      vct.at(idx++)=naccdx;
      vct.at(idx++)=naccdy;
      vct.at(idx++)=naccadx;
      vct.at(idx++)=naccady;
    }
  };

  template<typename T>
  bool surfLocalDescriptor::helperIgnore(const matrix<T>& src,
                                         const list<location>& locs,
                                         std::list<dvector>& desc) const {
    const parameters& param = getParameters();

    // prepare the integral image functor
    integralImage integrator(param.boundaryType);
    matrix<typename typeInfo<T>::accumulation_type> intImg;
    
    // some general geometric information of the locations
    const int wndSize = param.numberOfSubregions * param.subregionSamples;
    const int hSide = wndSize/2;

    // size of the descriptor
    const int blockSize = (param.signSplit) ? 8 : 4 ;
    const int dSize = sqr(param.numberOfSubregions) * blockSize;
    block4 acc4;
    block8 acc8;
    block* acc;
    if (param.signSplit) {
      acc = &acc8;
    } else {
      acc = &acc4;
    }
    dvector d(dSize); // container for one descriptor

#if defined(_LTI_DEBUG)
    static matrix<T> canvas;
    canvas.copy(src);
    static viewer2D view("helperIgnore::debug");
#endif
    
    desc.clear(); // prepare to insert each descriptor

    list<location>::const_iterator it,eit;
   
    // compute integral image of the src channel
    integrator.apply(src,intImg);

    const unsigned int rows = intImg.rows();
    const unsigned int cols = intImg.columns();

    for (it=locs.begin(),eit=locs.end();it!=eit;++it) {
      const location& loc = *it;
      const float lhSide = hSide * loc.radius;
      
      const float lx = loc.position.x;
      const float ly = loc.position.y;

      const int wlsh = iround(loc.radius*param.waveletSize/2.0f);

      int idx = 0;
      for (int y=0;y<param.numberOfSubregions;++y) {
        const int yoff = y*param.subregionSamples;
        for (int x=0;x<param.numberOfSubregions;++x) {
          const int xoff = x*param.subregionSamples;

          // for each sub-region of the window
          acc->reset();
          float ry = ly - lhSide + yoff*loc.radius; 
          for (int yy=0;yy<param.subregionSamples;++yy) {
            const int iry = iround(ry);
            ry += loc.radius;

            // if we leave the image, go on with the next line
            if (static_cast<uint32>(iry) >= rows) {
              continue;
            }

            float rx = lx - lhSide + xoff*loc.radius;

            for (int xx=0;xx<param.subregionSamples;++xx) {
              // here rx and ry have the position of the pixel
              const int irx = iround(rx);
                    
              rx += loc.radius;
              
              // if we leave the image, go on with the next one
              if (static_cast<uint32>(irx) >= cols) {
                continue;
              }

              _lti_if_debug(canvas.at(iry,irx)=255);

              const float weight = gaussian_.at(yy+yoff,xx+xoff);
                    
              // we are within the image, so let's compute the haar
              // values
              const float dx = 
                weight*(integrator.sum(intImg,
                                       irx,iry-wlsh,irx+wlsh,iry+wlsh) -
                        integrator.sum(intImg,
                                       irx-wlsh,iry-wlsh,irx,iry+wlsh));
              const float dy =
                weight*(integrator.sum(intImg,
                                       irx-wlsh,iry,irx+wlsh,iry+wlsh) -
                        integrator.sum(intImg,
                                       irx-wlsh,iry-wlsh,irx+wlsh,iry));
              
              acc->acc(dx,dy);
            }
          }
          acc->load(idx,d);
        }
      } // end of window loop
            
      // normalize the vector
      if (param.normalize) {
        const double dnorm = sqrt(d.dot(d));
        d.divide(dnorm);
      }

      desc.push_back(d);
    } // end of locations loop
    
    _lti_if_debug(view.show(canvas));

    return true;
  }

  template<typename T>
  bool surfLocalDescriptor::helperCluster(const matrix<T>& src,
                                          const list<location>& locs,
                                          std::list<dvector>& desc) const {

    const parameters& param = getParameters();

    // new formats for the locations, clustered according to their values
    std::vector< list<location> > sortedLocs;
    std::vector<location> refs;
    imatrix clusters;
    fvector angles;
    ivector numLocs;
    fvector sines;
    fvector cosines;

    // prepare the transformation functor for rotations
    typedef matrixTransform<T> trans_type;
    typename trans_type::parameters transPar;
    transPar.resizeMode = geometricTransformBase::AdjustDimensions;
    transPar.interpolatorParams.boundaryType = param.boundaryType;
    trans_type transformer(transPar);
    matrix<T> chnl;
    fpoint offset;

    // prepare the integral image functor
    integralImage integrator(param.boundaryType);
    matrix<typename typeInfo<T>::accumulation_type> intImg;

    // get the orientation clusters, and if everything is ok go on...
    if (cluster(locs,refs,clusters,angles,sines,cosines,numLocs)) {
      
      // some general geometric information of the locations
      const int wndSize = param.numberOfSubregions * param.subregionSamples;
      const int hSide = wndSize/2;

      // size of the descriptor
      const int blockSize = (param.signSplit) ? 8 : 4 ;
      const int dSize = sqr(param.numberOfSubregions) * blockSize;
      block4 acc4;
      block8 acc8;
      block* acc;
      if (param.signSplit) {
        acc = &acc8;
      } else {
        acc = &acc4;
      }


#if defined(_LTI_DEBUG)
      static matrix<T> canvas;
      static viewer2D view("helperCluster::debug");
#endif


      
      // matrix holding all descriptors
      dmatrix vcts(static_cast<int>(refs.size()),dSize);

      // for each available cluster
      for (int c=0;c<clusters.rows();++c) {
        // number of locations in current cluster
        const int lic = numLocs.at(c);
        if (lic > 0) {
          // cluster c has some locations
          // rotate the image in the oposite direction, so that we can 
          // computate the descriptors as if the locationes were zero oriented.
          
          // rotate
          transformer.setMatrix(rotationMatrix(fpoint(0.0f,0.0f),
                                               -angles.at(c)));
          transformer.apply(src,chnl,offset);
          
          _lti_if_debug(canvas.copy(chnl));

          const float sina = sines.at(c);
          const float cosa = cosines.at(c);

          // compute integral image of the rotated channel
          integrator.apply(chnl,intImg);

          const unsigned int rows = intImg.rows();
          const unsigned int cols = intImg.columns();

          for (int i=0;i<lic;++i) {
            const int locPos = clusters.at(c,i);
            const location& loc = refs[locPos];
            dvector& d = vcts.getRow(locPos);
            
            const float lhSide = hSide * loc.radius;

            // the rotation has moved the position of the location
            // we need that new position in the intImg coordinates

            _lti_debug("Offset: " << offset << std::endl);

            const int lx = iround(cosa*loc.position.x+sina*loc.position.y-
                                  offset.x);
            const int ly = iround(-sina*loc.position.x+cosa*loc.position.y-
                                  offset.y);

            const int wlsh = iround(loc.radius*param.waveletSize/2.0f);

            int idx = 0;
            for (int y=0;y<param.numberOfSubregions;++y) {
              const int yoff = y*param.subregionSamples;
              for (int x=0;x<param.numberOfSubregions;++x) {
                const int xoff = x*param.subregionSamples;

                // for each sub-region of the window
                acc->reset();
                float ry = ly - lhSide + yoff*loc.radius; 
                for (int yy=0;yy<param.subregionSamples;++yy) {
                  const int iry = iround(ry);
                  ry += loc.radius;

                   // if we leave the image, go on with the next line
                  if (static_cast<uint32>(iry) >= rows) {
                    continue;
                  }

                  float rx = lx - lhSide + xoff*loc.radius;

                  for (int xx=0;xx<param.subregionSamples;++xx) {
                    // we save the following direct implementation with sums
                    // const int rx = iround(xx*s*cosa - yy*s*sina + s*tx);
                    // const int ry = iround(xx*s*sina + yy*s*cosa + s*ty);
                    
                    // here rx and ry have the position of the pixel
                    const int irx = iround(rx);
                    
                    rx += loc.radius;

                    // if we leave the image, go on with the next one
                    if (static_cast<uint32>(irx) >= cols) {
                      continue;
                    }

                    _lti_if_debug(canvas.at(iry,irx)=255);

                    const float weight = gaussian_.at(yy+yoff,xx+xoff);
                    
                    // we are within the image, so let's compute the haar
                    // values
                    const float dx = 
                      weight*(integrator.sum(intImg,
                                             irx,iry-wlsh,irx+wlsh,iry+wlsh) -
                              integrator.sum(intImg,
                                             irx-wlsh,iry-wlsh,irx,iry+wlsh));
                    const float dy =
                      weight*(integrator.sum(intImg,
                                             irx-wlsh,iry,irx+wlsh,iry+wlsh) -
                              integrator.sum(intImg,
                                             irx-wlsh,iry-wlsh,irx+wlsh,iry));
                    
                    acc->acc(dx,dy);
                  }
                }
                acc->load(idx,d);
              }
            }
            
            // normalize the vector
            if (param.normalize) {
              const double dnorm = sqrt(d.dot(d));
              d.divide(dnorm);
            }
          }          
        }
      }

      _lti_if_debug(view.show(canvas));

      // pass the information to the desc list
      desc.clear();
      for (int i=0;i<vcts.rows();++i) {
        desc.push_back(vcts.getRow(i));
      }

      return true;
    }

    return false;
  }

  template<typename T>
  bool surfLocalDescriptor::helperApprox(const matrix<T>& src,
                                         const list<location>& locs,
                                         std::list<dvector>& desc) const {


    const parameters& param = getParameters();

    // first compute the integral image
    matrix<typename typeInfo<T>::accumulation_type> intImg;
    integralImage integrator(param.boundaryType);
    integrator.apply(src,intImg);

    desc.clear();

    const uint32 rows = static_cast<uint32>(intImg.rows());
    const uint32 cols = static_cast<uint32>(intImg.columns());
    
    const int wndSize = param.numberOfSubregions * param.subregionSamples;
    const int hSide = wndSize/2;

    // size of the descriptor
    const int blockSize = (param.signSplit) ? 8 : 4 ;
    const int dSize = sqr(param.numberOfSubregions) * blockSize;
    block4 acc4;
    block8 acc8;
    block* acc;
    if (param.signSplit) {
      acc = &acc8;
    } else {
      acc = &acc4;
    }

    dvector d(dSize);

#if defined(_LTI_DEBUG)
    static matrix<T> canvas;
    canvas.copy(src);
    static viewer2D view("helperApprox::debug");
#endif


    list<location>::const_iterator it,eit;
    for (it=locs.begin(),eit=locs.end();it!=eit;++it) {
      const location& loc = (*it);
      
      // the angles needed for the mapping
      const float cosa = cos(loc.angle);
      const float sina = sin(loc.angle);

      const float rcosa = loc.radius*cosa;
      const float rsina = loc.radius*sina;

      const float tx = loc.position.x - hSide*(rcosa - rsina);
      const float ty = loc.position.y - hSide*(rcosa + rsina);

      const int wlsh = iround(loc.radius*param.waveletSize/2.0f);
      
      d.assign(dSize,0); // clear vector
      int idx = 0;      
      for (int y=0;y<param.numberOfSubregions;++y) {
        const int yoff = y*param.subregionSamples;
        for (int x=0;x<param.numberOfSubregions;++x) {
          const int xoff = x*param.subregionSamples;
          // for each sub-region of the window
          acc->reset();
          for (int yy=0;yy<param.subregionSamples;++yy) {
            const int yyy = yy+yoff;
            float rx = xoff*rcosa - yyy*rsina + tx;
            float ry = xoff*rsina + yyy*rcosa + ty;
            
            for (int xx=0;xx<param.subregionSamples;++xx) {
              // we save the following direct implementation with sums
              // const int rx = iround(xx*s*cosa - yy*s*sina + s*tx);
              // const int ry = iround(xx*s*sina + yy*s*cosa + s*ty);

              // here rx and ry have the position of the pixel
              const int irx = iround(rx);
              const int iry = iround(ry);
       
              // update for next time
              rx += rcosa;
              ry += rsina;

              // if we leave the image, go on with the next one
              if ( (static_cast<uint32>(irx) >= cols) ||
                   (static_cast<uint32>(iry) >= rows) ) {

                continue;
              }

              _lti_if_debug(canvas.at(iry,irx)=255);

              const float weight=gaussian_.at(yyy,xx+xoff);

              // we are within the image, so let's compute the haar values
              const float dx = 
                weight*(integrator.sum(intImg,
                                       irx,iry-wlsh,irx+wlsh,iry+wlsh) -
                        integrator.sum(intImg,
                                       irx-wlsh,iry-wlsh,irx,iry+wlsh));
              const float dy =
                weight*(integrator.sum(intImg,
                                       irx-wlsh,iry,irx+wlsh,iry+wlsh) -
                        integrator.sum(intImg,
                                       irx-wlsh,iry-wlsh,irx+wlsh,iry));
              
              // here is a trick to avoid the real rotation of the image.  This
              // is NOT documented in the paper, but since everything is
              // approximated, a further simplification won't hurt.

              // We can think of this Haar responses as a scale-dependent
              // approximation of the first gaussian derivatives in a given
              // direction, which at the same time are steerable filters, with
              // the cosine and sine of the orientation angle as interpolation
              // coefficient.
              
              // this might react somehow chaotical for complex random textures
              // of fine granularity, but in most cases should work well. (the
              // real image rotation and computation of the Haar responses
              // would also react chaotically anyway)

              const float rdx =  dx*cosa + dy*sina;
              const float rdy =  -dx*sina + dy*cosa;

              acc->acc(rdx,rdy);
            }
          }
          acc->load(idx,d);
        }
      }

      // normalize the vector
      if (param.normalize) {
        const double dnorm = sqrt(d.dot(d));
        d.divide(dnorm);
      }
      desc.push_back(d);
    }

    _lti_if_debug(view.show(canvas));
    
    return true;
  }

  /*
   * Operates on the given argument.
   *
   * @param src channel8 with the image to be described.
   * @param locs list of locations to be described
   * @param desc resulting descritors computed for each location.
   * @return true if apply successful or false otherwise.
   */
  bool surfLocalDescriptor::apply(const channel8& src,
                                  const list<location>& locs,
                                  std::list<dvector>& desc) const {
    return helper(src,locs,desc);
  }
  
  /*
   * Operates on the given argument.
   *
   * @param src channel8 with the image to be described.
   * @param locs list of locations to be described
   * @param desc resulting descritors computed for each location.
   * @return true if apply successful or false otherwise.
   */
  bool surfLocalDescriptor::apply(const channel& src,
                                  const list<location>& locs,
                                        std::list<dvector>& desc) const {
    return helper(src,locs,desc);
  }

  /*
   * Operates on the given argument.
   *
   * @param src channel8 with the image to be described.
   * @param loc the locations to be described
   * @param desc resulting descritor computed for the given location.
   * @return true if apply successful or false otherwise.
   */
  bool surfLocalDescriptor::apply(const channel& src,
                                  const location& loc,
                                  dvector& desc) const {
    list<location> locs;
    locs.push_back(loc);

    std::list<dvector> descl;
    if (helper(src,locs,descl)) {
      descl.front().detach(desc);
      return true;
    } 
    return false;
  }
  
  /**
   * Read a fastHessianDetection::eLevelSelectionMethod
   *
   * @ingroup gStorable
   */
  bool read(ioHandler& handler,
            surfLocalDescriptor::eOrientationMode& data) {
    std::string str;
    if (handler.read(str)) {
      if (str.find("gnor") != std::string::npos) {
        data = surfLocalDescriptor::Ignore;
      } else if (str.find("pprox") != std::string::npos) {
        data = surfLocalDescriptor::Approximate;
      } else if (str.find("luster") != std::string::npos) {
        data = surfLocalDescriptor::Cluster;
      } else {
        str = "Unknown symbol " + str;
        handler.setStatusString(str);

        data = surfLocalDescriptor::Approximate;
        return false;
      }
      return true;
    }
    return false;
  }

  /**
   * Write a fastHessianDetection::eLevelSelectionMethod
   *
   * @ingroup gStorable
   */
  bool write(ioHandler& handler,
             const surfLocalDescriptor::eOrientationMode& data) {
    bool b = false;
    switch(data) {
      case surfLocalDescriptor::Ignore:
        b = handler.write("Ignore");
        break;
      case surfLocalDescriptor::Approximate:
        b = handler.write("Approximate");
        break;
      case surfLocalDescriptor::Cluster:
        b = handler.write("Cluster");
        break;
      default:
        handler.write("Unknown");
        handler.setStatusString("Undefined eOrientationMode");
        b = false;
    }
    return b;
  }


}

