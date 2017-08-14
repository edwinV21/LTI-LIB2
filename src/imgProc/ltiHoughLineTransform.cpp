/*
 * Copyright (C) 2008
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
 * \file   ltiHoughLineTransform.cpp
 *         Contains the class houghLineTransform,
 *         which has still to be documented by Pablo Alvarado.
 * 
 * \author Henning Luepschen
 * \author Pablo Alvarado
 * \date   18.12.2008
 *
 * revisions ..: $Id: ltiHoughLineTransform.cpp,v 1.6 2013-03-03 03:03:12 alvarado Exp $
 */

#include "ltiHoughLineTransform.h"
#include "ltiRound.h"

#include "ltiMath.h"

#include "ltiPointList.h"
#include "ltiSort2.h"
#include "ltiMaximumFilter.h"

#undef _LTI_DEBUG
//#define _LTI_DEBUG 4

#include "ltiDebug.h"

#ifdef _LTI_DEBUG
#include "ltiViewer2D.h"
#include <iostream>
using std::cout;
using std::endl;
#endif


namespace lti {
  // --------------------------------------------------
  // houghLineTransform::parameters
  // --------------------------------------------------

  // default constructor
  houghLineTransform::parameters::parameters()
    : functor::parameters() {
    
    transformationArea = irectangle(0,0,
                                    std::numeric_limits<int>::max(),
                                    std::numeric_limits<int>::max());
    baseValue = int(0);
    accuracy = 180;
    range = 10;
    accumulationMode = Classic;

    stdDevFactor = 1.0f;
    hystheresis =  0.5f;
    localMaximaRadius = 1;
    searchRadius = 0.25;
  }

  // copy constructor
  houghLineTransform::parameters::parameters(const parameters& other)
    : functor::parameters() {
    copy(other);
  }

  // destructor
  houghLineTransform::parameters::~parameters() {
  }

  // copy member

  houghLineTransform::parameters&
  houghLineTransform::parameters::copy(const parameters& other) {
    functor::parameters::copy(other);

    transformationArea.copy(other.transformationArea);
    baseValue = other.baseValue;
    accuracy = other.accuracy;
    range = other.range;
    accumulationMode = other.accumulationMode;

    stdDevFactor = other.stdDevFactor;
    hystheresis = other.hystheresis;
    localMaximaRadius = other.localMaximaRadius;
    searchRadius = other.searchRadius;

    return *this;
  }

  // alias for copy method
  houghLineTransform::parameters&
  houghLineTransform::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  // class name
  const std::string& houghLineTransform::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone method
  houghLineTransform::parameters*
  houghLineTransform::parameters::clone() const {
    return new parameters(*this);
  }

  // new instance
  houghLineTransform::parameters*
  houghLineTransform::parameters::newInstance() const {
    return new parameters();
  }

  /*
   * Write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool houghLineTransform::parameters::write(ioHandler& handler,
                                             const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {

      lti::write(handler,"transformationArea",transformationArea);
      lti::write(handler,"baseValue",baseValue);
      lti::write(handler,"accuracy",accuracy);
      lti::write(handler,"range",range);
      lti::write(handler,"accumulationMode",accumulationMode);

      lti::write(handler,"stdDevFactor",stdDevFactor);
      lti::write(handler,"hystheresis",hystheresis);
      lti::write(handler,"localMaximaRadius",localMaximaRadius);
      lti::write(handler,"searchRadius",searchRadius);

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
  bool houghLineTransform::parameters::read(ioHandler& handler,
                                            const bool complete)  {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {
      lti::read(handler,"transformationArea",transformationArea);
      lti::read(handler,"baseValue",baseValue);
      lti::read(handler,"accuracy",accuracy);
      lti::read(handler,"range",range);
      lti::read(handler,"accumulationMode",accumulationMode);

      lti::read(handler,"stdDevFactor",stdDevFactor);
      lti::read(handler,"hystheresis",hystheresis);
      lti::read(handler,"localMaximaRadius",localMaximaRadius);
      lti::read(handler,"searchRadius",searchRadius);
    }

    b = b && functor::parameters::read(handler,false);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

  // --------------------------------------------------
  // houghLineTransform
  // --------------------------------------------------

  // default constructor
  houghLineTransform::houghLineTransform()
    : functor(),accuracy_(0) {

    // create an instance of the parameters with the default values
    parameters defaultParameters;
    // set the default parameters
    setParameters(defaultParameters);
  }

  // default constructor
  houghLineTransform::houghLineTransform(const parameters& par)
    : functor(),accuracy_(0) {

    // set the given parameters
    setParameters(par);
  }


  // copy constructor
  houghLineTransform::houghLineTransform(const houghLineTransform& other)
    : functor(),accuracy_(0) {
    copy(other);
  }

  // destructor
  houghLineTransform::~houghLineTransform() {
  }

  // copy member
  houghLineTransform& 
  houghLineTransform::copy(const houghLineTransform& other) {
    functor::copy(other);

    return (*this);
  }

  // alias for copy member
  houghLineTransform&
  houghLineTransform::operator=(const houghLineTransform& other) {
    return (copy(other));
  }

  // class name
  const std::string& houghLineTransform::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone member
  houghLineTransform* houghLineTransform::clone() const {
    return new houghLineTransform(*this);
  }

  // create a new instance
  houghLineTransform* houghLineTransform::newInstance() const {
    return new houghLineTransform();
  }

  // return parameters
  const houghLineTransform::parameters&
  houghLineTransform::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }

  // return parameters
  houghLineTransform::parameters& houghLineTransform::getRWParameters() {
    parameters* par = dynamic_cast<parameters*>(&functor::getRWParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }

  bool houghLineTransform::updateParameters() {
    // we need to check the accuracy parameter :   
    const parameters& localPar = getParameters();

    if ( (localPar.accuracy != 45) && 
         (localPar.accuracy != 90) &&
         ( (localPar.accuracy%180)!=0) ) {
      setStatusString("Invalid accuracy (not 45 or 90)");
      return false;
    }

    if (localPar.accuracy != accuracy_) {
      accuracy_ = localPar.accuracy;
      // now create sinus/cosinus integer tables.
      createSinCosTable(accuracy_);
    }

    return true;
    
  }

  // Create sinus/cosinus integer table to speed things up.
  void houghLineTransform::createSinCosTable(const int size) {

    cosinus_.allocate(size);
    sinus_.allocate(size);

    // create integer tables with values between (-8192) and 8192.
    double sina, cosa;
    for(int i=0; i<size; ++i) {
      sincos(i*Pi/size, sina, cosa);
      cosinus_[i] = iround(cosa*8192);
      sinus_[i]   = iround(sina*8192);
    }
  }

  // -------------------------------------------------------------------
  // The apply() member functions
  // -------------------------------------------------------------------


  /**
   * This method is used to get the list of points in the parameter
   * space, sorted by their hough transformation value, with the
   * maxima at the beginning.
   *
   * The given threshold value determines if a pixel in the transformation
   * should be considered or not.
   *
   */
  bool houghLineTransform::getHoughSpacePoints(const channel32& houghSpace,
                                               vector<fpoint>& pos) const {
    const parameters& param = getParameters();
    return getHoughSpacePoints(houghSpace,pos,
                               param.stdDevFactor,
                               param.hystheresis,
                               param.localMaximaRadius,
                               param.searchRadius);
  }

  
  /**
   * This method is used to get the list of points in the parameter
   * space, sorted by their hough transformation value, with the
   * maxima at the beginning.
   *
   * The given threshold value determines if a pixel in the transformation
   * should be considered or not.
   *
   */
  bool houghLineTransform::getHoughSpacePoints(const channel32& houghSpace,
                                               vector<fpoint>& pos,
                                               const float stdDevFactor,
                                               const float hystheresis,
                                               const int wndRadius,
                                               const float searchRadius) const{

    ipointList stack;
    float maxPts,accMax,acc2Max;
    int y,x,i,j;
    float v;

    if ( (hystheresis < 0.0f) || (hystheresis > 1.0f) ||
         (wndRadius < 0) || 
         (wndRadius > min(houghSpace.rows(),houghSpace.columns()))) {
      setStatusString("Invalid argument values");
      return false;
    }

    // use i as a temporary variable here
    if (searchRadius < 0) { // i.e. no limit!
      i = houghSpace.rows(); 
    } else if (searchRadius < 1.0) { // i.e. a ratio of the max radius, which
                                     // is half of the acc channel size
      i = iround(searchRadius * houghSpace.rows() * 0.5f);
    } else {
      i = iround(searchRadius); // absolute value indicated
    }

    const int maxSearchRadius = i;

    channel houghf,premax;
    houghf.castFrom(houghSpace);

    // maximum filter
    maximumFilter<float>::parameters mxfPar;
    mxfPar.maskWindow = irectangle(-wndRadius,-wndRadius,wndRadius,wndRadius);
    mxfPar.boundaryType = lti::Periodic;
    maximumFilter<float> mxf(mxfPar);

    mxf.apply(houghf,premax);

    maxPts=accMax=acc2Max=0.0f;
    for (y=0;y<premax.rows();++y) {
      const vector<float>& hv = houghf.getRow(y);
      const vector<float>& pm = premax.getRow(y);
      for (x=0;x<premax.columns();++x) {
        v = hv.at(x);
        if ((v>0) && (pm.at(x) <= v)) {
          maxPts++;
          accMax  += v;
          acc2Max += v*v;
          stack.push_back(ipoint(x,y));
        }
      }
    }

    // now, erase all points under the average + stdDevFactor x std. deviation
    const float avrg = accMax/maxPts;
    const float stdd = sqrt((acc2Max/maxPts) - (avrg*avrg));
    const float threshold = max(avrg + stdDevFactor*stdd,0.0f);

    ipointList::iterator it;
    it = stack.begin();
    while (it != stack.end()) {
      if (houghf.at(*it) > threshold) {
        ++it;
      } else {
        it = stack.erase(it);
        maxPts--;
      }
    };

    // the rest of points need to be sorted before we use a region growing
    // algorithm.
    vector<ipoint> pts;
    ivector vals;

    pts.allocate(static_cast<int>(maxPts));
    vals.allocate(static_cast<int>(maxPts));

    it = stack.begin();
    for (it=stack.begin(),i=0;it!=stack.end();++it,++i) {
      pts.at(i)=*it;
      vals.at(i)=houghSpace.at(*it);
    }

    stack.clear(); // we don't need the stack data anymore (release memory)

    // sort the vectors using sort2 functor (Descending order -> true)
    sort2 sorter(Descending);
    sorter.apply(vals,pts);

    // now, do region growing from the smallest maxima to the biggest ones
    float actualMax,actualThresh;
    static const int deltax[] = {1,1,0,-1,-1,-1,0,1};
    static const int deltay[] = {0,-1,-1,-1,0,1,1,1};
    ipoint p,q,actualMaxPos,seed;
    ipointList localMax;

    i=pts.lastIdx();
    while (i>=0) {

      actualMaxPos = seed = pts.at(i);
      actualMax = houghf.at(seed);

      if (actualMax > 0) {
                   
        actualThresh = actualMax*hystheresis;

        _lti_debug4("Starting point " << i << ": " << actualMax << " @ " << 
                    actualMaxPos << "  limited by " << actualThresh <<
                    std::endl);

        // Insert on stack the current max position as seed
        stack.push_back(seed);

        while (!stack.empty()) {
          // get next point to be evaluated
          p = stack.back();
          stack.pop_back();
          v = houghf.at(p);
          houghf.at(p) = -1; // flag: already evaluated

          _lti_debug4("  eval " << v << " @ " << p);

          if (v > actualMax) {
            actualMax = v;
            actualMaxPos = p;

            _lti_debug4(" * new max * ");
          }
          
          _lti_debug4(std::endl);

          for (j=0;j<8;j++) {
            q.x = p.x+deltax[j];
            q.y = p.y+deltay[j];

            if (q.y < 0) {
              continue;
            } else if (q.y > houghf.rows()) {
              continue;
            }

            if (q.x < 0) {
              q.x += houghf.columns();
              q.y = houghf.rows() - q.y; // since the change of angle means
                                         // automatically a change of the
                                         // radius sign, which is represented
                                         // here as complement.
            } else if (q.x >= houghf.columns()) {
              q.x -= houghf.columns();
              q.y = houghf.rows() - q.y; // since the change of angle means
                                         // automatically a change of the
                                         // radius sign.
            }

            if ( (houghf.at(q) > actualThresh) &&
                 (abs(q.y-seed.y) < maxSearchRadius) ) {
              stack.push_back(q);
            } else {
              houghf.at(q) = -1;
            }
          }
        } // while something still on stack

        // the surviving maxima is a local maxima!
        localMax.push_front(actualMaxPos);
        
        _lti_debug4(" FINAL: " << actualMaxPos << std::endl);
      }

      i--;
    } // while i>0 (i.e. while still maxima on the vector)

    pos.allocate(localMax.size());

    const float angFactor = static_cast<float>(Pi/houghf.columns());
    const float r0 = round(static_cast<float>(houghf.rows())/2.0f);

    for (i=0,it=localMax.begin();i<pos.size();++i,++it) {
      pos.at(i).set((*it).y-r0 , (*it).x*angFactor);
      _lti_debug4("Max @ "<< (*it) << " -> " << pos.at(i) << std::endl);
    }

#ifdef _LTI_DEBUG
#if _LTI_DEBUG > 2
    cout << endl;
    cout << localMax.size() << "/" << premax.rows()*premax.columns()
         << " maxima found" << endl;
    cout << avrg << " average value" << endl;
    cout << stdd << " std. deviation" << endl;

    static viewer2D view1("Max Filtered");
    static viewer2D view3("Grown Hough");

    view1.show(premax);
    view3.show(houghf);
#endif
#endif
    return true;
  }

  void houghLineTransform::houghSpaceToAccImage(const ipoint& accSize,
                                                const ipoint& houghSpace,
                                                ipoint& accImage) const {
    
    const float angFactor = static_cast<float>(Pi/accSize.x);
    const int r0 = static_cast<const int>(round(static_cast<float>(accSize.y)/2.0f));

    accImage.y = houghSpace.x + r0;
    accImage.x = iround(static_cast<float>(houghSpace.y)/angFactor);
  }


  bool houghLineTransform::accToSegment(const ipoint& accSize,
                                        const ipoint& pos,
                                        const ipoint& imageSize,
                                        iline& segment,
                                        const ipoint& origin) const {
    const float angFactor = static_cast<float>(Pi/accSize.x);
    const float r0 = round(static_cast<float>(accSize.y)/2.0f);

    return getSegment(imageSize,pos.y-r0,pos.x*angFactor,segment,origin);
  }

  /**
   * Given a radius and an angle, compute the two points at the border of
   * the image
   */
  bool houghLineTransform::getSegment(const ipoint& imageSize,
                                      const float radius,
                                      const float angle,
                                      iline& segment,
                                      const ipoint& origin) const {

    ipoint c;
    if (origin.x < 0) {
      c.x = imageSize.x/2;
    } else {
      c.x = origin.x;
    }

    if (origin.y < 0) {
      c.y = imageSize.y/2;
    } else {
      c.y = origin.y;
    }

    float stmp, ctmp;
    sincos(angle, stmp, ctmp);
    const float cosa = ctmp;
    const float sina = stmp;

    ipoint a,b;

    if ((angle > Pi/4.0) && (angle < (3.0*Pi/4.0))) {

      a.x=0;
      a.y=iround((radius-(cosa*(a.x-c.x)))/sina)+c.y;

      if (a.y<0) {
        a.y=0;
        a.x=iround((radius-(sina*(a.y-c.y)))/cosa)+c.x;
      } else if (a.y>=imageSize.y) {
        a.y=imageSize.y-1;
        a.x=iround((radius-(sina*(a.y-c.y)))/cosa)+c.x;
      }

      b.x=imageSize.x-1;
      b.y=iround((radius - (cosa*(b.x-c.x)))/sina) + c.y;

      if (b.y<0) {
        b.y=0;
        b.x=iround((radius-(sina*(b.y-c.y)))/cosa)+c.x;
      } else if (b.y>=imageSize.y) {
        b.y=imageSize.y-1;
        b.x=iround((radius-(sina*(b.y-c.y)))/cosa)+c.x;
      }

    } else {

      a.y=0;
      a.x=iround((radius-(sina*(a.y-c.y)))/cosa)+c.x;

      if (a.x<0) {
        a.x=0;
        a.y=iround((radius-(cosa*(a.x-c.x)))/sina)+c.y;
      } else if (a.x>=imageSize.x) {
        a.x=imageSize.x-1;
        a.y=iround((radius-(cosa*(a.x-c.x)))/sina)+c.y;
      }

      b.y = imageSize.y-1;
      b.x = iround((radius - (sina*(b.y-c.y)))/cosa) + c.x;

      if (b.x<0) {
        b.x=0;
        b.y=iround((radius - (cosa*(b.x-c.x)))/sina) + c.y;
      } else if (b.x>=imageSize.x) {
        b.x=imageSize.x-1;
        b.y=iround((radius - (cosa*(b.x-c.x)))/sina) + c.y;
      }
    }

    segment.set(a,b);

    return true;
  }

  template<typename T>
  inline T houghLineTransform::norm(const int val) const {
    return static_cast<T>(val);
  }
  
  template<>
  inline rgbaPixel houghLineTransform::norm<rgbaPixel>(const int val) const {
    return rgbaPixel(val,val,val);
  }

  template<>
  inline float houghLineTransform::norm<float>(const int val) const {
    return static_cast<float>(val)/255.0f;
  }

  template<>
  inline double houghLineTransform::norm<double>(const int val) const {
    return static_cast<double>(val)/255.0;
  }

  // cast method between given value in src and the value_type of the
  // accumulator
  template<typename T>
  inline channel32::value_type 
  houghLineTransform::getAccVal(const T val) const {
    return static_cast<channel32::value_type>(val);
  }

  template<>
  inline channel32::value_type
  houghLineTransform::getAccVal<float>(const float val) const {
    return static_cast<channel32::value_type>(val*255);
  }

  template<>
  inline channel32::value_type
  houghLineTransform::getAccVal<rgbaPixel>(const rgbaPixel val) const {
    return (static_cast<channel32::value_type>(val.getRed()) +
            static_cast<channel32::value_type>(val.getGreen()) +
            static_cast<channel32::value_type>(val.getBlue()))/3;
  }



  template<typename T>
  bool houghLineTransform::hough(const matrix<T>& src,
                                 const channel& angleSrc,
                                 channel32& dest) const {
    const parameters& par = getParameters();
    switch (par.accumulationMode) {
      case Classic:
        return houghClassic(par.transformationArea,src,angleSrc,dest);
        break;
      case Gradient:
        return houghGradient(par.transformationArea,src,angleSrc,dest);
        break;
      default:
        return houghClassic(par.transformationArea,src,angleSrc,dest);
    }

    return false;
  }

  template<typename T>
  bool houghLineTransform::hough(const irectangle& transformationArea,
                                 const matrix<T>& src,
                                 const channel& angleSrc,
                                 channel32& dest) const {
    const parameters& par = getParameters();
    switch (par.accumulationMode) {
      case Classic:
        return houghClassic(transformationArea,src,angleSrc,dest);
        break;
      case Gradient:
        return houghGradient(transformationArea,src,angleSrc,dest);
        break;
      default:
        return houghClassic(transformationArea,src,angleSrc,dest);
    }

    return false;
  }

  /**
   * This method implements the hough transform, depending on the
   * image type being analysed.
   *
   * The implementation code must be here due to a MS-VC++ bug, that
   * produces an error C2893 if the code is where it should be.
   */
  template<class T>
  bool houghLineTransform::houghGradient(const irectangle& transformationArea,
                                         const matrix<T>& src,
                                         const channel& angleSrc,
                                         channel32& dest) const {

    int iY, iX;   // X and Y in source.
    int y,x;      // iY and iX shifted
    int angle;   // angle in transformation accumulator
    int axisIntercept;  // axis intercept in transformation accu
    int distance, oldY, counter; // used for Bresenham's algorithm
    int maxAI;    // 1/2 of maximum axis intercept

    // Variables used for orientation
    int gradientAngle;    // Angle determined by the gradient edge filter
    int angleRange[2][2];
    int rangeCnt;

    // variables used for speeding up
    int iTmp, iTmp2;


    const parameters& params = getParameters();
    const T baseValue = norm<T>(params.baseValue);

    // Check size.
    const int minx = max(0,transformationArea.ul.x);
    const int maxx = min(src.lastColumn(),transformationArea.br.x);

    const int miny = max(0,transformationArea.ul.y);
    const int maxy = min(src.lastRow(),transformationArea.br.y);

    const int dimX = maxx-minx+1;
    const int dimY = maxy-miny+1;

    const int range = params.range;

    // maximum radii from the origin to the borders of the region analysed
    const int midX = (dimX+1)/2;
    const int midY = (dimY+1)/2;

    // position of the origin
    const int x0 = midX + minx;
    const int y0 = midY + miny;

    // constant required to convert from grad to deg
    static const float gtd = static_cast<float>(180.0/Pi);

    // the transformation accumulator will have the size:
    // [accuracy_,2*maxAI]
    maxAI = static_cast<int>(sqrt(midX*midX + midY*midY)/10+1)*10;
    dest.assign(2*maxAI, accuracy_, 0);

    // In order to be used correctly with sinus/cosinus values from the table
    maxAI *= 8192;

    // The angle range values must be multiplied with this factor
    const float fFactor = accuracy_ / 180.0f;
    channel32::value_type val;
    oldY = 0;

    // scan whole source
    for( iY=miny; iY<=maxy; iY++ ) {
      // with a relatively high probability, there will be points at
      // the most lines, so we can compute the shifted y just once per line.
      y = iY-y0;

      for( iX=minx; iX<=maxx; iX++ ) {
        // Only consider points having a value that is
        // greater than the base value (usually white
        // lines on black background)
        if (src.at(iY,iX) > baseValue) {
          val = getAccVal(src.at(iY,iX));
          // compute the shifted x just when necessary:
          x = iX-x0;

          // Radians -> Degrees
          gradientAngle = static_cast<int>(angleSrc.at(iY,iX) * gtd);

          // We have lines. We only need values between 0 and 180
          if(gradientAngle<0) {
            gradientAngle += 180;
          } else if (gradientAngle >= 180) {
            gradientAngle -= 180;
          }

          // Determine the angle range in which the sinusoid will be drawn.
          if( gradientAngle < range ) {
            angleRange[0][0] = 0;
            angleRange[1][0] = static_cast<int>((180 - range +
                                                   gradientAngle)*fFactor);
            angleRange[1][1] = static_cast<int>(180*fFactor);
          }
          else {
            angleRange[0][0] = static_cast<int>((gradientAngle-
                                                   range)*fFactor);
            angleRange[1][0] = 0;
            angleRange[1][1] = 0;
          }

          if( gradientAngle > 180-range ) {
            angleRange[0][1] = static_cast<int>(180*fFactor);
            angleRange[1][1] = static_cast<int>((gradientAngle-
                                                   (180-range))*fFactor);
          }
          else {
            angleRange[0][1] = static_cast<int>((gradientAngle+
                                                   range)*fFactor);
          }

          rangeCnt = 0;

          while( rangeCnt<2 && angleRange[rangeCnt][1] != 0 ) {

            angle=angleRange[rangeCnt][0];
            axisIntercept = (x*cosinus_[angle] +
                              y*sinus_[angle] + maxAI) / 8192;
            oldY = axisIntercept;

            for( angle++;
                 angle<angleRange[rangeCnt][1];
                 angle++) {

              // determine axis intercept for given angle angle using
              // the fast sinus/cosinus integer tables. Add maxAI in
              // order to have only positive values (thus integer
              // rounding will be correct).
              axisIntercept = (x*cosinus_[angle] +
                               y*sinus_[angle] + maxAI) / 8192;

              // Now accumulate point in the transformation
              // accumulator using a fast form of Bresenham's line
              // algorithm.  See also:
              // http://graphics.cs.ucdavis.edu/GraphicsNotes/
              // Bresenhams-Algorithm/Bresenhams-Algorithm.html
              distance=axisIntercept-oldY;
              iTmp2=angle-1;
              if (distance<=1 && distance>=-1) {
                dest.at(oldY,iTmp2)+=val;
              }
              else {
                if (distance>0) {
                  iTmp=oldY+distance;

                  for (counter=oldY; counter<iTmp; counter++) {
                    dest.at(counter,iTmp2)+=val;
                  }
                }
                else {
                  iTmp=axisIntercept+1-distance;

                  for (counter=axisIntercept+1;
                       counter<iTmp;
                       counter++) {
                    dest.at(counter,iTmp2)+=val;
                  }
                }
              }

              oldY = axisIntercept;
            }

            // the last column must not remain empty!
            if (angle >= accuracy_) {
              axisIntercept = (-x*cosinus_[0]
                                -y*sinus_[0] + maxAI) / 8192;
            } else {
              axisIntercept = (x*cosinus_[angle] +
                                y*sinus_[angle] + maxAI) / 8192;
            }

            distance=axisIntercept-oldY;
            iTmp2=angle-1;

            if (distance<=1 && distance>=-1) {
              dest.at(oldY,iTmp2)+=val;
            }
            else {
              if (distance>0) {
                iTmp=oldY+distance;

                for (counter=oldY; counter<iTmp; counter++) {
                  dest.at(counter,iTmp2)+=val;
                }
              }
              else {
                iTmp=axisIntercept+1-distance;

                for (counter=axisIntercept+1;
                     counter<iTmp;
                     counter++) {
                  dest.at(counter,iTmp2)+=val;
                }
              }
            }
            // end of last column count

            rangeCnt++;
          }
        }
      }
    }

    return true;
  }

  /**
   * This method implements the hough transform, depending on the
   * image type being analysed.
   *
   * The implementation code must be here due to a MS-VC++ bug, that
   * produces an error C2893 if the code is where it should be.
   */
  template<class T>
  bool houghLineTransform::houghClassic(const irectangle& transformationArea,
                                        const matrix<T>& src,
                                        const channel& angleSrc,
                                        channel32& dest) const {

    int iY, iX;   // X and Y in source.
    int y,x;      // iY and iX shifted
    int angle;   // angle in transformation accumulator
    int axisIntercept;  // axis intercept in transformation accu
    int distance, oldY, counter; // used for Bresenham's algorithm
    int maxAI;    // 1/2 of maximum axis intercept

    // Variables used for orientation
    int gradientAngle;    // Angle determined by the gradient edge filter
    int angleRange[2][2];
    int rangeCnt;

    // variables used for speeding up
    int iTmp, iTmp2;

    const parameters& params = getParameters();
    const T baseValue = norm<T>(params.baseValue);

    // Check size.
    const int minx = max(0,transformationArea.ul.x);
    const int maxx = min(src.lastColumn(),transformationArea.br.x);

    const int miny = max(0,transformationArea.ul.y);
    const int maxy = min(src.lastRow(),transformationArea.br.y);

    const int dimX = maxx-minx+1;
    const int dimY = maxy-miny+1;

    const int range = params.range;

    // maximum radii from the origin to the borders of the region analysed
    const int midX = (dimX+1)/2;
    const int midY = (dimY+1)/2;

    // position of the origin
    const int x0 = midX + minx;
    const int y0 = midY + miny;

    // constant required to convert from grad to deg
    const float gtd = static_cast<float>(180.0/Pi);

    // the transformation accumulator will have the size:
    // [accuracy_,2*maxAI]
    maxAI = static_cast<int>(sqrt(midX*midX + midY*midY)/10+1)*10;
    dest.assign(2*maxAI, accuracy_, 0);

    // In order to be used correctly with sinus/cosinus values from the table
    maxAI *= 8192;

    // The angle range values must be multiplied with this factor
    const float fFactor = accuracy_ / 180.0f;
    oldY = 0;

    // scan whole source
    for( iY=miny; iY<=maxy; iY++ ) {
      // with a relatively high probability, there will be points at
      // the most lines, so we can compute the shifted y just once per line.
      y = iY-y0;

      for( iX=minx; iX<=maxx; iX++ ) {
        // Only consider points having a value that is
        // greater than the base value (usually white
        // lines on black background)
        if (src.at(iY,iX) > baseValue) {
          // compute the shifted x just when necessary:
          x = iX-x0;

          // Radians -> Degrees
          gradientAngle = static_cast<int>(angleSrc.at(iY,iX) * gtd);

          // We have lines. We only need values between 0 and 180
          if(gradientAngle<0) {
            gradientAngle += 180;
          } else if (gradientAngle>=180) {
            gradientAngle -= 180;
          }

          // Determine the angle range in which the sinusoid will be drawn.
          if( gradientAngle < range ) {
            angleRange[0][0] = 0;
            angleRange[1][0] = static_cast<int>((180 - range +
                                                 gradientAngle)*fFactor);
            angleRange[1][1] = static_cast<int>(180*fFactor);
          }
          else {
            angleRange[0][0] = static_cast<int>((gradientAngle-
                                                 range)*fFactor);
            angleRange[1][0] = 0;
            angleRange[1][1] = 0;
          }

          if( gradientAngle > 180-range ) {
            angleRange[0][1] = static_cast<int>(180*fFactor);
            angleRange[1][1] = static_cast<int>((gradientAngle-
                                                 (180-range))*fFactor);
          }
          else {
            angleRange[0][1] = static_cast<int>((gradientAngle+
                                                 range)*fFactor);
          }

          rangeCnt = 0;

          while( rangeCnt<2 && angleRange[rangeCnt][1] != 0 ) {

            angle=angleRange[rangeCnt][0];
            axisIntercept = (x*cosinus_[angle] +
                             y*sinus_[angle] + maxAI) / 8192;
            oldY = axisIntercept;

            for( angle++;
                 angle<angleRange[rangeCnt][1];
                 angle++) {

              // determine axis intercept for given angle angle using
              // the fast sinus/cosinus integer tables. Add maxAI in
              // order to have only positive values (thus integer
              // rounding will be correct).
              axisIntercept = (x*cosinus_[angle] +
                               y*sinus_[angle] + maxAI) / 8192;

              // Now accumulate point in the transformation
              // accumulator using a fast form of Bresenham's line
              // algorithm.  See also:
              // http://graphics.cs.ucdavis.edu/GraphicsNotes/
              // Bresenhams-Algorithm/Bresenhams-Algorithm.html
              distance=axisIntercept-oldY;
              iTmp2=angle-1;

              if (distance<=1 && distance>=-1) {
                dest.at(oldY,iTmp2)++;
              }
              else {
                if (distance>0) {
                  iTmp=oldY+distance;

                  for (counter=oldY; counter<iTmp; counter++) {
                    dest.at(counter,iTmp2)++;
                  }
                }
                else {
                  iTmp=axisIntercept+1-distance;

                  for (counter=axisIntercept+1;
                       counter<iTmp;
                       counter++) {
                    dest.at(counter,iTmp2)++;
                  }
                }
              }

              oldY = axisIntercept;
            }

            // the last column must not remain empty!
            if (angle >= accuracy_) {
              axisIntercept = (-x*cosinus_[0]
                               -y*sinus_[0] + maxAI) / 8192;
            } else {
              axisIntercept = (x*cosinus_[angle] +
                               y*sinus_[angle] + maxAI) / 8192;
            }

            distance=axisIntercept-oldY;
            iTmp2=angle-1;

            if (distance<=1 && distance>=-1) {
              dest.at(oldY,iTmp2)++;
            }
            else {
              if (distance>0) {
                iTmp=oldY+distance;

                for (counter=oldY; counter<iTmp; counter++) {
                  dest.at(counter,iTmp2)++;
                }
              }
              else {
                iTmp=axisIntercept+1-distance;

                for (counter=axisIntercept+1;
                     counter<iTmp;
                     counter++) {
                  dest.at(counter,iTmp2)++;
                }
              }
            }
            // end of last column count

            rangeCnt++;
          }
        }
      }
    }

    return true;
  }

  // On copy apply for type channel8!
  bool houghLineTransform::apply(const channel& src,
                                 const channel& angleSrc,
                                 channel32& dest) const {
    return hough(src,angleSrc,dest);
  }

  // On copy apply for type channel8!
  bool houghLineTransform::apply(const channel8& src,
                                 const channel& angleSrc,
                                 channel32& dest) const {
    return hough(src,angleSrc,dest);
  }

  // On copy apply for type image!
  bool houghLineTransform::apply(const image& src,
                                 const channel& angleSrc,
                                 channel32& dest) const {

    return hough(src,angleSrc,dest);
  }

  // On copy apply for type channel32!
  bool houghLineTransform::apply(const channel32& src,
                                 const channel& angleSrc,
                                 channel32& dest) const {
    return hough(src,angleSrc,dest);
  }


  // On copy apply for type channel8!
  bool houghLineTransform::apply(const channel& src,
                                 const channel& angleSrc,
                                 genericVector<iline>& segs) const {
    channel32 dest;
    return hough(src,angleSrc,dest) && getSegments(dest,src.size(),segs);
  }

  // On copy apply for type channel8!
  bool houghLineTransform::apply(const channel8& src,
                                 const channel& angleSrc,
                                 genericVector<iline>& segs) const {
    channel32 dest;
    return hough(src,angleSrc,dest) && getSegments(dest,src.size(),segs);
  }

  // On copy apply for type image!
  bool houghLineTransform::apply(const image& src,
                                 const channel& angleSrc,
                                 genericVector<iline>& segs) const {

    channel32 dest;
    return hough(src,angleSrc,dest) && getSegments(dest,src.size(),segs);
  }

  // On copy apply for type channel32!
  bool houghLineTransform::apply(const channel32& src,
                                 const channel& angleSrc,
                                 genericVector<iline>& segs) const {
    channel32 dest;
    return hough(src,angleSrc,dest) && getSegments(dest,src.size(),segs);
  }



  // On copy apply for type channel8!
  bool houghLineTransform::apply(const irectangle& area,
                                 const channel& src,
                                 const channel& angleSrc,
                                 channel32& dest) const {
    return hough(area,src,angleSrc,dest);
  }

  // On copy apply for type channel8!
  bool houghLineTransform::apply(const irectangle& area,
                                 const channel8& src,
                                 const channel& angleSrc,
                                 channel32& dest) const {
    return hough(area,src,angleSrc,dest);
  }

  // On copy apply for type image!
  bool houghLineTransform::apply(const irectangle& area,
                                 const image& src,
                                 const channel& angleSrc,
                                 channel32& dest) const {

    return hough(area,src,angleSrc,dest);
  }

  // On copy apply for type channel32!
  bool houghLineTransform::apply(const irectangle& area,
                                 const channel32& src,
                                 const channel& angleSrc,
                                 channel32& dest) const {
    return hough(area,src,angleSrc,dest);
  }

  // short cut for accuracy (new sinus/cosinus tables may be created)
  int houghLineTransform::getAccuracy() const {
    return accuracy_;
  }

  bool houghLineTransform::setAccuracy(const int accuracy) {

    getRWParameters().accuracy = accuracy;
    return updateParameters();
  }

  /**
   * Get the segments in an accumulation channel
   */
  bool houghLineTransform::getSegments(const channel32& accSpace,
                                       const ipoint& chnlSize,
                                       genericVector<iline>& segments) const {

    const parameters& par = getParameters();
    // Now get the local maxima of the transformAccu
    lti::vector< fpoint > maxpos;
    if (!getHoughSpacePoints(accSpace,
                             maxpos,
                             par.stdDevFactor,
                             par.hystheresis,
                             par.localMaximaRadius,
                             par.searchRadius)) {
      return false;
    }
 
    ipoint p;
    segments.allocate(maxpos.size());
    for (int k=0;k<maxpos.size();++k) {
      getSegment(chnlSize,maxpos.at(k).x,maxpos.at(k).y,segments.at(k));
    }

    return true;
  }


  /**
   * Read a houghLineTransform::eAccumulationMode
   *
   * @ingroup gStorable
   */
  bool read(ioHandler& handler,
            houghLineTransform::eAccumulationMode& data) {
    std::string str;
    
    if (handler.read(str)) {
      
      if (str.find("lassic") != std::string::npos) {
        data = houghLineTransform::Classic;
      } else if (str.find("radient") != std::string::npos) {
        data = houghLineTransform::Gradient;
      } else {
        data = houghLineTransform::Classic;
        return false;
      }

      return true;
    } 

    return false;  
  }

  /**
   * Write a houghLineTransform::eAccumulationMode
   *
   * @ingroup gStorable
   */
  bool write(ioHandler& handler,
             const houghLineTransform::eAccumulationMode& data) {
    bool b = false;
    switch(data) {
      case houghLineTransform::Classic:
        b = handler.write("Classic");
        break;
      case houghLineTransform::Gradient:
        b = handler.write("Gradient");
        break;
      default:
        b=false;
        handler.write("Unknown");
        handler.setStatusString("undefined houghLineTransform");
    }
    
    return b;
  }




}

