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
 * revisions ..: $Id: ltiFastHessianDetection.cpp,v 1.11 2008-01-14 18:01:33 alvarado Exp $
 */

#include "ltiSort2.h"

#undef _LTI_DEBUG
//#define _LTI_DEBUG 2

#ifdef _LTI_DEBUG
#include "ltiViewer2D.h"
#define _LTI_DEBUG 2
#endif
#include "ltiDebug.h"

#include "ltiIntegralImage.h"
#include "ltiFastHessianDetection.h"
#include "ltiFastHessianDetection_template.h"


namespace lti {
  // --------------------------------------------------
  // fastHessianDetection::parameters
  // --------------------------------------------------

  // default constructor
  fastHessianDetection::parameters::parameters()
    : locationDetection::parameters() {

    numberOfLevels = 12;
    initialKernelSize = 9;
    levelSelectionMethod = Blocks;
    initialKernelStep = 6;
    levelGroupSize = 4;
    normPower = 4.0f;

    extrema = Both;
    locationSelectionMode = All;
    threshold = 0.1f;
    subsampleLevels = true;
    initialSamplingStep = 2;
    
    computeOrientation = true;
    orientationNeighborhoodFactor = 6;
    orientationSamplingStepFactor = 1;
    orientationWaveletSizeFactor = 4;
    orientationGaussianFactor = 2.5;
    orientationWindowWidth = 60;
    orientationNumberOfWindows = 6;
    
  }

  // copy constructor
  fastHessianDetection::parameters::parameters(const parameters& other)
    : locationDetection::parameters() {
    copy(other);
  }

  // destructor
  fastHessianDetection::parameters::~parameters() {
  }

  // copy member

  fastHessianDetection::parameters&
  fastHessianDetection::parameters::copy(const parameters& other) {
    locationDetection::parameters::copy(other);

    numberOfLevels = other.numberOfLevels;
    initialKernelSize = other.initialKernelSize;
    levelSelectionMethod = other.levelSelectionMethod;
    initialKernelStep = other.initialKernelStep;
    levelGroupSize = other.levelGroupSize;
    normPower = other.normPower;

    extrema = other.extrema;
    locationSelectionMode = other.locationSelectionMode;
    threshold = other.threshold;
    subsampleLevels = other.subsampleLevels;
    initialSamplingStep = other.initialSamplingStep;

    computeOrientation = other.computeOrientation;
    orientationNeighborhoodFactor = other.orientationNeighborhoodFactor;
    orientationSamplingStepFactor = other.orientationSamplingStepFactor;
    orientationWaveletSizeFactor  = other.orientationWaveletSizeFactor;
    orientationGaussianFactor = other.orientationGaussianFactor;
    orientationWindowWidth    = other.orientationWindowWidth;
    orientationNumberOfWindows  = other.orientationNumberOfWindows;

    return *this;
  }

  // alias for copy method
  fastHessianDetection::parameters&
  fastHessianDetection::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  // class name
  const std::string& fastHessianDetection::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone method
  fastHessianDetection::parameters*
  fastHessianDetection::parameters::clone() const {
    return new parameters(*this);
  }

  // new instance
  fastHessianDetection::parameters*
  fastHessianDetection::parameters::newInstance() const {
    return new parameters();
  }

  /*
   * Write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool fastHessianDetection::parameters::write(ioHandler& handler,
                                         const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {
      lti::write(handler,"numberOfLevels",numberOfLevels);
      lti::write(handler,"initialKernelSize",initialKernelSize);
      lti::write(handler,"levelSelectionMethod",levelSelectionMethod);
      lti::write(handler,"initialKernelStep",initialKernelStep);
      lti::write(handler,"levelGroupSize",levelGroupSize);
      lti::write(handler,"normPower",normPower);

      lti::write(handler,"extrema",extrema);
      lti::write(handler,"locationSelectionMode",locationSelectionMode);
      lti::write(handler,"threshold",threshold);
      lti::write(handler,"subsampleLevels",subsampleLevels);
      lti::write(handler,"initialSamplingStep",initialSamplingStep);

      lti::write(handler,"computeOrientation",computeOrientation);
      lti::write(handler,"orientationNeighborhoodFactor",
                 orientationNeighborhoodFactor);
      lti::write(handler,"orientationSamplingStepFactor",
                 orientationSamplingStepFactor);
      lti::write(handler,"orientationWaveletSizeFactor",
                 orientationWaveletSizeFactor);
      lti::write(handler,"orientationGaussianFactor",
                 orientationGaussianFactor);
      lti::write(handler,"orientationWindowWidth",orientationWindowWidth);
      lti::write(handler,"orientationNumberOfWindows",
                 orientationNumberOfWindows);
    }

    b = b && locationDetection::parameters::write(handler,false);

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
  bool fastHessianDetection::parameters::read(ioHandler& handler,
                                        const bool complete)  {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {
      
      lti::read(handler,"numberOfLevels",numberOfLevels);
      lti::read(handler,"initialKernelSize",initialKernelSize);
      lti::read(handler,"levelSelectionMethod",levelSelectionMethod);
      lti::read(handler,"initialKernelStep",initialKernelStep);
      lti::read(handler,"levelGroupSize",levelGroupSize);
      lti::read(handler,"normPower",normPower);

      lti::read(handler,"extrema",extrema);
      lti::read(handler,"locationSelectionMode",locationSelectionMode);
      lti::read(handler,"threshold",threshold);
      lti::read(handler,"subsampleLevels",subsampleLevels);
      lti::read(handler,"initialSamplingStep",initialSamplingStep);
      
      lti::read(handler,"computeOrientation",computeOrientation);

      lti::read(handler,"orientationNeighborhoodFactor",
                 orientationNeighborhoodFactor);
      lti::read(handler,"orientationSamplingStepFactor",
                 orientationSamplingStepFactor);
      lti::read(handler,"orientationWaveletSizeFactor",
                 orientationWaveletSizeFactor);
      lti::read(handler,"orientationGaussianFactor",
                 orientationGaussianFactor);
      lti::read(handler,"orientationWindowWidth",orientationWindowWidth);
      lti::read(handler,"orientationNumberOfWindows",
                orientationNumberOfWindows);

    }

    b = b && locationDetection::parameters::read(handler,false);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

  // --------------------------------------------------
  // fastHessianDetection
  // --------------------------------------------------

  // default constructor
  fastHessianDetection::fastHessianDetection()
    : locationDetection() {

    // create an instance of the parameters with the default values
    parameters defaultParameters;
    // set the default parameters
    setParameters(defaultParameters);
  }

  // default constructor
  fastHessianDetection::fastHessianDetection(const parameters& par)
    : locationDetection() {

    // set the given parameters
    setParameters(par);
  }


  // copy constructor
  fastHessianDetection::fastHessianDetection(const fastHessianDetection& other)
    : locationDetection() {
    copy(other);
  }

  // destructor
  fastHessianDetection::~fastHessianDetection() {
  }

  // copy member
  fastHessianDetection& 
  fastHessianDetection::copy(const fastHessianDetection& other) {
    locationDetection::copy(other);

    return (*this);
  }

  // alias for copy member
  fastHessianDetection&
  fastHessianDetection::operator=(const fastHessianDetection& other) {
    return (copy(other));
  }

  // class name
  const std::string& fastHessianDetection::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone member
  fastHessianDetection* fastHessianDetection::clone() const {
    return new fastHessianDetection(*this);
  }

  // create a new instance
  fastHessianDetection* fastHessianDetection::newInstance() const {
    return new fastHessianDetection();
  }

  // return parameters
  const fastHessianDetection::parameters&
  fastHessianDetection::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&locationDetection::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }

  // update parameters
  bool fastHessianDetection::updateParameters() {
    const parameters& param = getParameters();
    
    kernelStep_ = max(6,iround(float(param.initialKernelStep)/6.0f)*6);
    kernelSize_ = max(3,iround(float(param.initialKernelSize)/3.0f)*3);
    if (even(kernelSize_)) {
      kernelSize_+=3;
    }

    if (param.computeOrientation) {

      static arctanLUT atanLUT; // force the LUT initialization here

      // the mask radius
      float radius = param.orientationNeighborhoodFactor/
                     param.orientationSamplingStepFactor;
      const int diameter = iround(2.0f*radius+1);
      const int iradius  = diameter/2;

      radius = iradius + 0.49f;
      int norm = 0;

      circLUT_.allocate(-iradius,iradius);
      gaussWeights_.allocate(-iradius,-iradius,iradius,iradius);
      const float variance = sqr(param.orientationGaussianFactor);

      for (int y=-iradius;y<=iradius;++y) {
        const int pos = (circLUT_.at(y) = iround(sqrt(radius*radius-y*y)));
        for (int x=-pos;x<=pos;++x) {
          ++norm;
          gaussWeights_.at(y,x) = exp(-0.5f*(y*y+x*x)/variance);
        }
      }

      // gaussWeights_.divide(norm);
      // wndSamples_ = norm;

      orientationWindowWidth_ = 
        (param.orientationWindowWidth < constants<float>::twoPi()) ?
        param.orientationWindowWidth : degToRad(param.orientationWindowWidth);

      orientationNumberOfWindows_ = max(1,param.orientationNumberOfWindows);
    }

    return true;

  }

  // -------------------------------------------------------------------
  // The apply() member functions
  // -------------------------------------------------------------------

  // On copy apply for type channel8!
  bool fastHessianDetection::apply(const channel8& src, 
                                   list<location>& locs) const {
    list<float> strength;
    return apply(src,locs,strength);
  }
  
  // On copy apply for type channel8!
  bool fastHessianDetection::apply(const channel8& src, 
                                   list<location>& locs,
                                   list<float>& strength,
                                   int& numLocs) const {

    // clear the lists to start from zero...
    locs.clear();
    strength.clear();
    numLocs = 0;

    std::vector<fmatrix> levels;
    ivector kernelSizes,sampleSteps;
    matrix<int32> intImg; // integral image
    if (computeLevels(src,intImg,levels,kernelSizes,sampleSteps)) {

#if defined(_LTI_DEBUG) && (_LTI_DEBUG >= 4)
      _lti_debug("levels: " << kernelSizes << std::endl);
      
      // For detailed debugging, show all computed levels
      _lti_debug("Creating viewers\n");
      static std::vector<viewer2D*> viewers(levels.size(),
                                            static_cast<viewer2D*>(0));

      viewer2D::parameters vpar;
      vpar.mappingType = viewer2DPainter::Optimal;

      for (unsigned int v=0;v<viewers.size();++v) {
        vpar.title = std::string("Level ")+ static_cast<char>(v+'0');
        viewers[v] = new viewer2D(vpar);
        _lti_debug("Showing level " << v << std::endl);
        viewers[v]->show(levels.at(v));
      }      
#endif      
      for (int i=1;i<kernelSizes.lastIdx();++i) {
        _lti_debug4("Search Level " << i << std::endl);

        // Find the maxima
        searchLevelExtremes(kernelSizes[i-1]*1.2f/9.0f,
                            kernelSizes[i]*1.2f/9.0f,
                            kernelSizes[i+1]*1.2f/9.0f,
                            levels[i-1],
                            levels[i],
                            levels[i+1],
                            sampleSteps.at(i),
                            locs,
                            strength,
                            numLocs);
      }

      return (selectLocations(locs,strength,numLocs) && 
              computeOrientations(intImg,locs));
    }
                       
    return true;
  }

  bool fastHessianDetection::apply(const channel8& src, 
                                   list<location>& locs,
                                   list<float>& strength) const {
    int numLocs;
    return apply(src,locs,strength,numLocs);
  }

  bool fastHessianDetection::apply(const channel8& src, 
                                   list<location>& locs,
                                   int& numLocs) const {
    list<float> strength;
    return apply(src,locs,strength,numLocs);
  }

  // On copy apply for type channel!
  bool fastHessianDetection::apply(const channel& src, 
                                   list<location>& locs) const {
    list<float> strength;
    int numLocs;
    return apply(src,locs,strength,numLocs);
  }
  
  // On copy apply for type channel!
  bool fastHessianDetection::apply(const channel& src, 
                                   list<location>& locs,
                                   list<float>& strength,
                                   int& numLocs) const {

    // clear the lists to start from zero...
    locs.clear();
    strength.clear();
    numLocs = 0;

    std::vector<fmatrix> levels;
    ivector kernelSizes,sampleSteps;
    matrix<float> intImg; // integral image
    if (computeLevels(src,intImg,levels,kernelSizes,sampleSteps)) {

#if defined(_LTI_DEBUG) && (_LTI_DEBUG >= 4)
      _lti_debug("levels: " << kernelSizes << std::endl);
      _lti_debug("sampling: " << sampleSteps << std::endl);
      
      // For detailed debugging, show all computed levels
      _lti_debug("Creating viewers\n");
      static std::vector<viewer2D*> viewers(levels.size(),
                                            static_cast<viewer2D*>(0));

      viewer2D::parameters vpar;
      vpar.mappingType = viewer2DPainter::Optimal;

      for (unsigned int v=0;v<viewers.size();++v) {
        vpar.title = std::string("Level ")+ static_cast<char>(v+'0');
        viewers[v] = new viewer2D(vpar);
        _lti_debug("Showing level " << v << std::endl);
        viewers[v]->show(levels.at(v));
      }
      getchar();
#endif      

      for (int i=1;i<kernelSizes.lastIdx();++i) {
        // Find the maxima
        searchLevelExtremes(kernelSizes[i-1]*1.2f/9.0f,
                            kernelSizes[i]*1.2f/9.0f,
                            kernelSizes[i+1]*1.2f/9.0f,
                            levels[i-1],
                            levels[i],
                            levels[i+1],
                            sampleSteps.at(i),
                            locs,
                            strength,
                            numLocs);
      }

      return (selectLocations(locs,strength,numLocs) && 
              computeOrientations(intImg,locs));
    }
                       
    return true;
  }

  bool fastHessianDetection::apply(const channel& src, 
                                   list<location>& locs,
                                   list<float>& strength) const {
    int numLocs;
    return apply(src,locs,strength,numLocs);
  }

  bool fastHessianDetection::apply(const channel& src, 
                                   list<location>& locs,
                                   int& numLocs) const {
    list<float> strength;
    return apply(src,locs,strength,numLocs);
  }


  bool fastHessianDetection::searchLevelExtremes(const float scalePrev,
                                                 const float scale,
                                                 const float scaleNext,
                                                 const fmatrix& prevLevel,
                                                 const fmatrix& level,
                                                 const fmatrix& nextLevel,
                                                 const int sampleStep,
                                                 list<location>& locs,
                                                 list<float>& strength,
                                                 int& numLocs) const {

    _lti_debug("searchLevelExtremes for kernel size " << 
               iround(scale*9.0/1.2) << std::endl);

    const parameters& param = getParameters();
    
    // we run over the "internal" image, leaving one pixel border
    int x,y;
    const int rows = level.rows()-sampleStep;
    const int cols = level.columns()-sampleStep;

    location loc;

    fmatrix hessian(3,3,0.0f);

    const float nlowds  = scalePrev - scale;
    const float nhighds = scaleNext - scale;

    /* For the computation of gradient and second derivatives involving the
     * scale axis, we use the derivates of an interpolated quadratic of the
     * form:
     *
     * f(s) = y = as^2 + bs + c
     *
     * with three pairs of values at (s,f(s)), (s+chi,f(s+chi)),
     * (s-eta,f(s-eta)) taken at three different levels, we can compute a, b
     * and c, assuming that always our current pixel is at (s=0), then we have
     *
     * f'(0) = 2as + b |_{s=0} = b
     *            chi f(s-eta)      (chi - eta)f(s)     eta f(s+chi)
     *       = - --------------  + ----------------- + --------------
     *            eta(eta+chi)          eta chi         chi(eta+chi)
     *
     *       = kp f(s-eta) + k f(s) + kn f(s+chi)
     *
     * f"(0) = 2a
     *            2 f(s-eta)           2 f(s)           2 f(s+chi)
     *       =   --------------  - ----------------- + --------------
     *            eta(eta+chi)        eta chi           chi(eta+chi)
     *
     *       = kkp f(s-eta) + kk f(s) + kkn f(s+chi)
     *
     */
    const float eta = -nlowds;
    const float chi =  nhighds;

    const float etapchi = eta+chi;
    const float etaxchi = eta*chi;

    const float kp = - chi       /(eta*(etapchi));
    const float k  =  (chi - eta)/(etaxchi);
    const float kn =   eta       /(chi*(etapchi));

    const float kkp =   2.0f  /(eta*(etapchi));
    const float kk  =  - 2.0f /(etaxchi);
    const float kkn =   2.0f  /(chi*(etapchi));

    const bool ignoreMax = (param.extrema == Minima);
    const bool ignoreMin = (param.extrema == Maxima);
    
    // We are going to assume here, that the use of the determinant goes as far
    // as to directly apply the threshold to it instead of using the usual
    // trace of the hessian matrix (laplacian).  This can be read between lines
    // in the paper, but is not stated explicitly.  They do not even mention
    // the threshold which is used in their code.
    const float maxThresh =  (param.locationSelectionMode == Absolute) ?
                              param.threshold :
                              -std::numeric_limits<float>::max();
    const float minThresh = -maxThresh;
    
    const float fstep = static_cast<float>(sampleStep);
    const float twostepi = 1.0f/(2.0f*fstep);
    const float sqrstepi = 1.0f/sqr(fstep);
    const float sqrstep4 = sqrstepi/4.0f;

    for (y=sampleStep;y<rows;y+=sampleStep) {
      for (x=sampleStep;x<cols;x+=sampleStep) {

        const float val    = level.at(y,x);
        const float left   = level.at(y,x-sampleStep);
        const float right  = level.at(y,x+sampleStep);
        const float top    = level.at(y-sampleStep,x);
        const float bottom = level.at(y+sampleStep,x);
        const float below  = prevLevel.at(y,x);
        const float above  = nextLevel.at(y,x);

        // first step: just locate an extreme candidate
        if (left > val) {
          // a minimum candidate
          if ( (right <= val) || (ignoreMin)     || (val > minThresh) ||
               (top <= val)   || (bottom <= val) ||
               (above <= val) || (below <= val) ) {
            // no luck!  no local minimum
            continue;
          }

          // it is a minimum candidate
          
        } else if (left < val) {
          // a maximum candidate
          if ( (right >= val) || (ignoreMax)     || (val < maxThresh) ||
               (top >= val)   || (bottom >= val) ||
               (above >= val) || (below >= val) ) {
            // no luck!  no local maximum
            continue;
          }

          // it is a maximum candidate

        } else {
          // wether max nor min
          continue;
        }

        // ok, we have an extreme candidate, so we need to compute precisely
        // where it is.

        // first, the gradient approximation

        const float gx = (right-left)*twostepi; 
        const float gy = (bottom-top)*twostepi;
        const float gs = kp*below + k*val + kn*above;
        
        // second, the 6 entries in the Hessian matrix have to be computed.
        // We will work with the upper triangular matrix to avoid several
        // unnecessary assigments

        // d^2L/dx^2
        const float hxx = (left+right-2.0f*val)*sqrstepi;

        // d^2L/dy^2
        const float hyy = (top + bottom -2.0f*val)*sqrstepi;

        // d^2L/ds^2
        const float hss = kkp*below + kk*val + kkn*above;

        // d^2L/dxdy
        const float hxy = (level.at(y-sampleStep,x-sampleStep) +
                           level.at(y+sampleStep,x+sampleStep) -
                           level.at(y-sampleStep,x+sampleStep) -
                           level.at(y+sampleStep,x-sampleStep))*sqrstep4;

        // d^2L/dxds
        const float hxs = (kp*(prevLevel.at(y,x+sampleStep)-
                               prevLevel.at(y,x-sampleStep)) +
                           k *(right-left) +
                           kn*(nextLevel.at(y,x+sampleStep)-
                               nextLevel.at(y,x-sampleStep)))*
          twostepi;

        // d^2L/dyds
        const float hys = (kp*(prevLevel.at(y+sampleStep,x)-
                               prevLevel.at(y-sampleStep,x)) +
                           k *(bottom-top) +
                           kn*(nextLevel.at(y+sampleStep,x)-
                               nextLevel.at(y-sampleStep,x)))*
          twostepi;

        // determinant of the hessian matrix
        const float det = (hxx*hyy*hss - hxx*hys*hys - hxy*hxy*hss +
                           2.0f*hxy*hxs*hys - hxs*hxs*hyy);

        static const float epsilon = std::numeric_limits<float>::epsilon();
        const float adet = abs(det);

        if (adet < epsilon) {
          continue;
        }

        // The inverse of the hessian matrix is given by:
        // |  hyy*hss-hys^2    -hxy*hss+hxs*hys   hxy*hys-hxs*hyy  |
        // | -hxy*hss+hxs*hys   hxx*hss-hxs^2    -hxx*hys+hxy*hxs  | /det
        // |  hxy*hys-hxs*hyy  -hxx*hys+hxy*hxs   hxx*hyy-hxy^2    |
        // which is also diagonal symmetric
        const float ihxy = -hxy*hss+hxs*hys;
        const float ihxs =  hxy*hys-hxs*hyy;
        const float ihys = -hxx*hys+hxy*hxs;
        const float ihxx =  hyy*hss-hys*hys;
        const float ihyy =  hxx*hss-hxs*hxs;
        const float ihss =  hxx*hyy-hxy*hxy;

        // After Brown&Lowe's paper, the extreme is at -hessian^-1 x grad
        loc.position.set(-(ihxx*gx+ihxy*gy+ihxs*gs)/det,
                         -(ihxy*gx+ihyy*gy+ihys*gs)/det);
        loc.radius = -(ihxs*gx+ihys*gy+ihss*gs)/det;
  
        // one last check:  if the interpolation makes sense, then
        // the values should lie within the cube around (x,y,s)
        const float apx = abs(loc.position.x);
        const float apy = abs(loc.position.y);

        if ( (apx >= fstep) ||
             (apy >= fstep) ||
             (loc.radius < nlowds) ||
             (loc.radius > nhighds) ) {
//           _lti_debug3("Loc @ (" << y << "," << x << "," << scale << ")" );
//           _lti_debug3(" discarded. " << std::endl);
          // ups!  somehow the interpolation produced results outside limits
          continue;
        }
//         _lti_debug2(numLocs << ": ");
//         _lti_debug2("Loc @ (" << x << "," << y << "," << scale << ")" );

        loc.position.x += x;
        loc.position.y += y;
        loc.radius += scale;

//         _lti_debug2(" interp. to (" << loc.position.x << "," << loc.position.y 
//                    << "," << loc.radius << ")");
//         _lti_debug2(" ok. " << std::endl);

        
        locs.push_back(loc);
        strength.push_back(val);
        numLocs++;
      }
    }

    return true;
  }

  bool fastHessianDetection::selectLocations(list<location>& locs,
                                             list<float>& strength,
                                             int& numLocs) const {

    const parameters& param = getParameters();

    if ( (param.locationSelectionMode == All) ||
         (param.locationSelectionMode == Absolute) ) {
      // these methods were already 
      return true;
    }

    list<location> tmpLocs;
    list<float> tmpStrength;
    numLocs = 0;

    const bool ignoreMax = (param.extrema == Minima);
    const bool ignoreMin = (param.extrema == Maxima);

    // compute statistics
    float minVal( std::numeric_limits<float>::max());
    float maxVal(-std::numeric_limits<float>::max());
    double sumVal(0.0),sum2Val(0.0);
    int n(0);

    list<location>::const_iterator lit,elit;
    list<float>::const_iterator sit,esit;
    
    for (sit=strength.begin(),esit=strength.end();sit!=esit;++sit) {
      const float val = (*sit);

      // statistics
      if (val>maxVal) {
        maxVal=val;
      } 
      if (val<minVal) {
        minVal=val;
      }
      ++n;
      sumVal  += val;
      sum2Val += (val*val);
    }
  
    switch(param.locationSelectionMode) {
      case All:
      case Absolute:
        return true;
        break;
      case Relative: {
        const float highThresh = maxVal*param.threshold;
        const float lowThresh  = minVal*param.threshold;
        numLocs = 0;

        for (lit=locs.begin(),elit=locs.end(),sit=strength.begin();
             lit!=elit;
             ++lit,++sit) {
          if (  ( ( (*sit) >= highThresh ) && !ignoreMax ) || 
                ( ( (*sit) <= lowThresh ) && !ignoreMin ) ) {
            tmpLocs.push_back(*lit);
            tmpStrength.push_back(*sit);
            numLocs++;
          }
        }
      } break;
      case Conspicuous: {
        const float mean = static_cast<float>(sumVal/n);
        const float stdDev = 
          static_cast<float>(sqrt((sum2Val - (sqr(sumVal)/n))/n));
        
        const float highThresh = mean+(param.threshold*stdDev);
        const float lowThresh  = mean-(param.threshold*stdDev);
        numLocs = 0;

        for (lit=locs.begin(),elit=locs.end(),sit=strength.begin();
             lit!=elit;
             ++lit,++sit) {
          if (  ( ( (*sit) >= highThresh ) && !ignoreMax ) || 
                ( ( (*sit) <= lowThresh ) && !ignoreMin ) ) {
            tmpLocs.push_back(*lit);
            tmpStrength.push_back(*sit);
            numLocs++;
          }
        }
        
      } break;
      case Number: {

        std::vector<location> sortedLocs;
        fvector sortedStrength;
        // this could be optimized using a partial sort
        sort(locs,strength,sortedLocs,sortedStrength);
        const int nLocs = min(max(1,iround(param.threshold)),
                              sortedStrength.size());
        numLocs = 0;

        for (int i=0;i<nLocs;++i) {
          tmpLocs.push_back(sortedLocs[i]);
          tmpStrength.push_back(sortedStrength.at(i));
          numLocs++;

        }
      } break;
      default:
        return false;
    }

    tmpLocs.swap(locs);
    tmpStrength.swap(strength);
    
    return true;
  }
  
  bool fastHessianDetection::sort(const list<location>& locs,
                                  const list<float>& strength,
                                  std::vector<location>& sortedLocs,
                                  fvector& sortedStrength) const {

    const int lsize = locs.size();
    const int ssize = strength.size();

    if (lsize != ssize) {
      setStatusString("Locations and strength lists have different sizes.");
      return false;
    }

    if (lsize == 0) {
      sortedLocs.clear();
      sortedStrength.clear();
      return true;
    }

    const parameters& param = getParameters();

    std::vector<location> tmpLocs(lsize);
    sortedLocs.resize(lsize);
    ivector indices(lsize);
    sortedStrength.allocate(lsize);

    list<location>::const_iterator lit;
    list<float>::const_iterator sit,eit;
    int i;
    for (lit=locs.begin(),sit=strength.begin(),eit=strength.end(),i=0;
         sit!=eit;
         ++lit,++sit,++i) {
      tmpLocs[i].copy(*lit);
      sortedStrength[i]=(param.extrema == Both) ? abs(*sit) : (*sit);
      indices[i]=i;
    }
      
    sort2 sorter((param.extrema == Minima)?Ascending:Descending);

    sorter.apply(sortedStrength,indices);

    for (i=0;i<indices.size();++i) {
      sortedLocs[i]=tmpLocs[indices.at(i)];
    }
    
    return true;
  }

  // IO Interface of internal types

  bool read(ioHandler& handler,fastHessianDetection::eExtremaType& data) {
    std::string str;
    if (handler.read(str)) {
      
      if (str.find("ax") != std::string::npos) {
        data = fastHessianDetection::Maxima;
      } else if (str.find("in") != std::string::npos) {
        data = fastHessianDetection::Minima;
      } else {
        data = fastHessianDetection::Both;
      }
    }
    
    return true;
  }

  bool write(ioHandler& handler,
             const fastHessianDetection::eExtremaType& data) {
    bool b = false;
    switch(data) {
      case fastHessianDetection::Maxima:
        b = handler.write("Maxima");
        break;
      case fastHessianDetection::Minima:
        b = handler.write("Minima");
        break;
      case fastHessianDetection::Both:
        b = handler.write("Both");
        break;
      default:
        b=false;
        handler.setStatusString("undefined eExtremaType");
        handler.write("Unknown");
    }
    return b;
  }

  bool read(ioHandler& handler,
            fastHessianDetection::eLevelSelectionMethod& data) {
    std::string str;
    if (handler.read(str)) {
      
      if (str.find("lock") != std::string::npos) {
        data = fastHessianDetection::Blocks;
      } else {
        data = fastHessianDetection::Exponential;
      }
    } 

    return true;
  }

  bool write(ioHandler& handler,
             const fastHessianDetection::eLevelSelectionMethod& data) {
    bool b = false;
    switch(data) {
      case fastHessianDetection::Blocks:
        b = handler.write("Blocks");
        break;
      case fastHessianDetection::Exponential:
        b = handler.write("Exponential");
        break;
      default:
        b=false;
        handler.write("Unknown");
        handler.setStatusString("undefined eLevelSelectionMethod");
    }
    return b;
  }

  bool read(ioHandler& handler,
            fastHessianDetection::eLocationSelectionMode& data) {
    std::string str;
    
    if (handler.read(str)) {
      
      if (str.find("ll") != std::string::npos) {
        data = fastHessianDetection::All;
      } else if (str.find("bs") != std::string::npos) {
        data = fastHessianDetection::Absolute;
      } else if (str.find("elat") != std::string::npos) {
        data = fastHessianDetection::Relative;
      } else if (str.find("onsp") != std::string::npos) {
        data = fastHessianDetection::Conspicuous;
      } else if (str.find("umber") != std::string::npos) {
        data = fastHessianDetection::Number;
      } else {
        data = fastHessianDetection::Absolute;
      }

      return true;
    } 

    return false;  
  }

  bool write(ioHandler& handler,
             const fastHessianDetection::eLocationSelectionMode& data) {
    bool b = false;
    switch(data) {
      case fastHessianDetection::All:
        b = handler.write("All");
        break;
      case fastHessianDetection::Absolute:
        b = handler.write("Absolute");
        break;
      case fastHessianDetection::Relative:
        b = handler.write("Relative");
        break;
      case fastHessianDetection::Conspicuous:
        b = handler.write("Conspicuous");
        break;
      case fastHessianDetection::Number:
        b = handler.write("Number");
        break;
      default:
        handler.write("Unknown");
        b = false;
        handler.setStatusString("undefined eLocationSelectionMode");
        break;
    }

    return b;
  }

}

