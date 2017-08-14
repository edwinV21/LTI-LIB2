/*
 * Copyright (C) 2010
 * Pedro Elías Alpízar Salas
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
 * \file   ltiCoherenceEnhancingDiffusion.cpp
 *         Contains the class coherenceEnhancingDiffusion,
 *         which has still to be documented by Pedro Elías Alpízar Salas.
 *
 * \author Pedro Elías Alpízar Salas
 * \date   23.08.2010
 *
 * revisions ..: $Id: ltiCoherenceEnhancingDiffusion.cpp,v 1.5 2012-09-01 01:18:32 alvarado Exp $
 */


#include "ltiCoherenceEnhancingDiffusion.h"
#include "ltiGaussKernels.h"
#include "ltiConvolutionHelper.h"
#include "ltiGradientKernels.h"
#include "ltiOgdKernels.h"
#include "ltiConstants.h"
#include "ltiConvolution.h"

#undef _LTI_DEBUG
// #define _LTI_DEBUG 4
#include "ltiDebug.h"
#include "ltiDebug.h"

#if defined(_LTI_DEBUG)
#  include <ltiViewer2D.h>
#endif


namespace lti {
  // --------------------------------------------------
  // coherenceEnhancingDiffusion::parameters
  // --------------------------------------------------


  // default constructor
  coherenceEnhancingDiffusion::parameters::parameters()
    : functor::parameters() {

    gradientKernelType = gradientFunctor::Difference;
    gradientKernelSize = 3;
    ogdVariance = -1.0f;
    gaussSmoothing = 1.0f;
    gaussSmoothingHessian = 10.0f;
    smootBoundary = Zero;
    phaseFilter = false;
    centerPhaseWindow = 0.0f;
    phaseWindowWidth = constants<float>::halfPi();
    phaseCosWidth = constants<float>::halfPi();
    stepDiffusionTime = 0.15f;
    totalDiffusionTime = 5.0f;
    smallRegularisation = 0.001f;
    diffTensorExponent = 1.0f;
    contrast = 1e-10f;
  }

  // copy constructor
  coherenceEnhancingDiffusion::parameters::parameters(const parameters& other)
    : functor::parameters() {
    copy(other);
  }

  // destructor
  coherenceEnhancingDiffusion::parameters::~parameters() {
  }

  // copy member

  coherenceEnhancingDiffusion::parameters&
  coherenceEnhancingDiffusion::parameters::copy(const parameters& other) {
    functor::parameters::copy(other);

    gradientKernelType = other.gradientKernelType;
    gradientKernelSize = other.gradientKernelSize;
    ogdVariance=other.ogdVariance;
    gaussSmoothing = other.gaussSmoothing;
    gaussSmoothingHessian = other.gaussSmoothingHessian;
    smootBoundary = other.smootBoundary;
    phaseFilter = other.phaseFilter;
    centerPhaseWindow = other.centerPhaseWindow;
    phaseWindowWidth = other.phaseWindowWidth;
    phaseCosWidth = other.phaseCosWidth;
    stepDiffusionTime = other.stepDiffusionTime;
    totalDiffusionTime = other.totalDiffusionTime;
    smallRegularisation = other.smallRegularisation;
    diffTensorExponent = other.diffTensorExponent;
    contrast = other.contrast;

    return *this;
  }

  // alias for copy method
  coherenceEnhancingDiffusion::parameters&
  coherenceEnhancingDiffusion::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  // class name
  const std::string& coherenceEnhancingDiffusion::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone method
  coherenceEnhancingDiffusion::parameters*
  coherenceEnhancingDiffusion::parameters::clone() const {
    return new parameters(*this);
  }

  // new instance
  coherenceEnhancingDiffusion::parameters*
  coherenceEnhancingDiffusion::parameters::newInstance() const {
    return new parameters();
  }

  /*
   * Write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool coherenceEnhancingDiffusion::parameters::write(ioHandler& handler,
                                         const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {

      lti::write(handler,"gradientKernelType",gradientKernelType);
      lti::write(handler,"gradientKernelSize",gradientKernelSize);
      lti::write(handler,"ogdVariance",ogdVariance);
      lti::write(handler,"smootBoundary",smootBoundary);
      lti::write(handler,"gaussSmoothing",gaussSmoothing);
      lti::write(handler,"gaussSmoothingHessian",gaussSmoothingHessian);
      lti::write(handler,"phaseFilter",phaseFilter);
      lti::write(handler,"centerPhaseWindow",centerPhaseWindow);
      lti::write(handler,"phaseWindowWidth",phaseWindowWidth);
      lti::write(handler,"phaseCosWidth",phaseCosWidth);
      lti::write(handler,"stepDiffusionTime",stepDiffusionTime);
      lti::write(handler,"totalDiffusionTime",totalDiffusionTime);
      lti::write(handler,"smallRegularisation",smallRegularisation);
      lti::write(handler,"diffTensorExponent",diffTensorExponent);
      lti::write(handler,"contrast",contrast);
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
  bool coherenceEnhancingDiffusion::parameters::read(ioHandler& handler,
                                        const bool complete)  {
     bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {

      lti::read(handler,"gradientKernelType",gradientKernelType);
      lti::read(handler,"gradientKernelSize",gradientKernelSize);
      lti::read(handler,"ogdVariance",ogdVariance);
      lti::read(handler,"smootBoundary",smootBoundary);
      lti::read(handler,"gaussSmoothing",gaussSmoothing);
      lti::read(handler,"gaussSmoothingHessian",gaussSmoothingHessian);
      lti::read(handler,"phaseFilter",phaseFilter);
      lti::read(handler,"centerPhaseWindow",centerPhaseWindow);
      lti::read(handler,"phaseWindowWidth",phaseWindowWidth);
      lti::read(handler,"phaseCosWidth",phaseCosWidth);
      lti::read(handler,"stepDiffusionTime",stepDiffusionTime);
      lti::read(handler,"totalDiffusionTime",totalDiffusionTime);
      lti::read(handler,"smallRegularisation",smallRegularisation);
      lti::read(handler,"diffTensorExponent",diffTensorExponent);
      lti::read(handler,"contrast",contrast);
    }

    b = b && functor::parameters::read(handler,false);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }


  // --------------------------------------------------
  // coherenceEnhancingDiffusion
  // --------------------------------------------------

  // default constructor
  coherenceEnhancingDiffusion::coherenceEnhancingDiffusion()
    : functor() {

    // create an instance of the parameters with the default values
    parameters defaultParameters;
    // set the default parameters
    setParameters(defaultParameters);
  }

  // default constructor
  coherenceEnhancingDiffusion::
  coherenceEnhancingDiffusion(const parameters& par)
    : functor() {

    // set the given parameters
    setParameters(par);
  }


  // copy constructor
  coherenceEnhancingDiffusion::
  coherenceEnhancingDiffusion(const coherenceEnhancingDiffusion& other)
    : functor() {
    copy(other);
  }

  // destructor
  coherenceEnhancingDiffusion::~coherenceEnhancingDiffusion() {
  }

  // copy member
  coherenceEnhancingDiffusion&
  coherenceEnhancingDiffusion::copy(const coherenceEnhancingDiffusion& other) {
    functor::copy(other);

    return (*this);
  }

  // alias for copy member
  coherenceEnhancingDiffusion& coherenceEnhancingDiffusion::
  operator=(const coherenceEnhancingDiffusion& other) {
    return (copy(other));
  }

  // class name
  const std::string& coherenceEnhancingDiffusion::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone member
  coherenceEnhancingDiffusion* coherenceEnhancingDiffusion::clone() const {
    return new coherenceEnhancingDiffusion(*this);
  }

  // create a new instance
  coherenceEnhancingDiffusion* coherenceEnhancingDiffusion::
                                                      newInstance() const {
    return new coherenceEnhancingDiffusion();
  }

  // return parameters
  const coherenceEnhancingDiffusion::parameters&
  coherenceEnhancingDiffusion::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }

   bool coherenceEnhancingDiffusion::updateParameters() {
    const parameters& par = getParameters();
    centerPhaseWindow_ = (par.centerPhaseWindow > constants<float>::pi()) ?
             degToRad(par.centerPhaseWindow) :
             par.centerPhaseWindow;
    phaseWindowWidth_ = (par.phaseWindowWidth > constants<float>::halfPi()) ?
             degToRad(par.phaseWindowWidth) :
             par.phaseWindowWidth;
    phaseCosWidth_ = (par.phaseCosWidth > constants<float>::halfPi()) ?
             degToRad(par.phaseCosWidth) :
             par.phaseCosWidth;
    maxWinPh_ = centerPhaseWindow_+phaseWindowWidth_/2;
    minWinPh_ = centerPhaseWindow_-phaseWindowWidth_/2;
    maxFilPh_ = maxWinPh_+phaseCosWidth_;
    minFilPh_ = minWinPh_-phaseCosWidth_;
    for(int i=0; i<1024; i++){
      phaseFilterFunction(1.0f,static_cast<float>((i)*Pi/1024),filterTable_[i]);
    }
    return true;
  }

  // -------------------------------------------------------------------
  // The apply() member functions
  // -------------------------------------------------------------------


  // On place apply for type channel!
  bool coherenceEnhancingDiffusion::apply(channel& srcdest) const {
    channel tmp;
    tmp.copy(srcdest);
    return apply(tmp,srcdest);
  }

  // On place apply for type channel8!
  bool coherenceEnhancingDiffusion::apply(channel8& srcdest) const {
    channel tmp, tmp2;
    tmp.castFrom(srcdest);
    bool bTmp = apply(tmp,tmp2);
    srcdest.castFrom(tmp2);
    return bTmp;
  }

  // On copy apply for type channel!
  bool coherenceEnhancingDiffusion::apply(const channel& src,
                                          channel&       dest) const {

    parameters par = getParameters();
    channel chImg, jXX, jYY, jXY, dXX, dXY, dYY;
    float timeD=0;
    float stepTimeMax = par.stepDiffusionTime;

    float tempGaussKernel=par.gaussSmoothing*5;
    tempGaussKernel<1?tempGaussKernel=1:tempGaussKernel=tempGaussKernel;



    gaussKernel2D
    <lti::channel::value_type>kernelSmoot((int)tempGaussKernel,
                              (double)(par.gaussSmoothing*par.gaussSmoothing));

    convolution applyConv;
    convolution::parameters parApplyConv;
    parApplyConv.setKernel(kernelSmoot);
    parApplyConv.boundaryType = par.smootBoundary;
    applyConv.setParameters(parApplyConv);

    //Inicialization of the channels
    dest.copy(src);
    chImg.assign(src.lastRow()+1,src.lastColumn()+1,0);
    jXX.assign(src.lastRow()+1,src.lastColumn()+1,0);
    jXY.assign(src.lastRow()+1,src.lastColumn()+1,0);
    jYY.assign(src.lastRow()+1,src.lastColumn()+1,0);
    dXX.assign(src.lastRow()+1,src.lastColumn()+1,0);
    dXY.assign(src.lastRow()+1,src.lastColumn()+1,0);
    dYY.assign(src.lastRow()+1,src.lastColumn()+1,0);




    while(timeD < (par.totalDiffusionTime - 0.001) ) {
        par.stepDiffusionTime = min(stepTimeMax,par.totalDiffusionTime-timeD);
        timeD+= par.stepDiffusionTime;

        //Smooting the Source image
        applyConv.apply(dest,chImg);

        _lti_debug("Calling structureTensor\n");

        //Creating the Jacobian
        structureTensor(chImg, jXX, jXY, jYY);

        //Creating the tensor field
        if(par.diffTensorExponent == 1){
          _lti_debug("Calling structDiffusionTensor\n");
          structDiffusionTensor(jXX, jXY, jYY, dXX, dXY, dYY);

        } else {
          _lti_debug("Calling structDiffusionTensor with pow\n");
          structDiffusionTensor(jXX, jXY, jYY, par.diffTensorExponent,
                                dXX, dXY, dYY);
        }

        _lti_debug("diffusionSchemeRotInvariance\n");

        //Aplicating the diffusion
        diffusionSchemeRotInvariance(dest, dXX, dXY, dYY, chImg);
        dest.copy(chImg);

        }


        return true;
    }

  // On copy apply for type channel8!
  bool coherenceEnhancingDiffusion::apply(const channel8& src,
                                          channel8&       dest) const
  {
    channel tmp, tmp2;
    tmp.castFrom(src);
    bool bTmp = apply(tmp,tmp2);
    dest.castFrom(tmp2);
    return bTmp;
  }

  bool coherenceEnhancingDiffusion::structureTensor(const channel& gaussSrc,
                                                    channel&       destJXX,
                                                    channel&       destJXY,
                                                    channel&       destJYY)
                                                    const{

    const parameters& par = getParameters();

    //float nMag = 0;
    int iY, iX;
    float cosTmp, sinTmp;
    const int yLastRow = gaussSrc.lastRow();
    const int xLastColumn = gaussSrc.lastColumn();

    gradientFunctor grad;
    gradientFunctor::parameters gradPar;
    channel gradX, gradY;
    channel gradMag, gradPh;

    //Parameters for the gradient
    gradPar.format = gradientFunctor::Cartesian;
    gradPar.kernelType = par.gradientKernelType;
    gradPar.gradientKernelSize =par.gradientKernelSize;
    grad.setParameters(gradPar);

    //Prepare the channels
    gradX.assign(gaussSrc.lastRow()+1,gaussSrc.lastColumn()+1,0);
    gradY.assign(gaussSrc.lastRow()+1,gaussSrc.lastColumn()+1,0);

    //Create the Gaussian Kernel for the smoothing of the Jacobian
    float tempGaussKernel=par.gaussSmoothingHessian*5;
    if(tempGaussKernel<1){
        tempGaussKernel=1;
    }
    gaussKernel2D
    <lti::channel::value_type>kernelSmoot((int)tempGaussKernel,
                (double)(par.gaussSmoothingHessian*par.gaussSmoothingHessian));

    convolution applyConv;
    convolution::parameters parApplyConv;
    parApplyConv.boundaryType = par.smootBoundary;

    //Prepare the gradient to create the Jacobian
    _lti_debug("Creating the gradient to create the Jacobian\n");

    if(!par.phaseFilter){
      grad.apply(gaussSrc, gradX, gradY);
    }
    else{
      gradPar.format = gradientFunctor::Polar;
      grad.setParameters(gradPar);
      grad.apply(gaussSrc, gradMag, gradPh);
      for (iY=0; iY<= yLastRow; iY++) {
        const fvector& gradMagRow =gradMag.getRow(iY);
        const fvector& gradPhRow =gradPh.getRow(iY);
        for (iX=0; iX<= xLastColumn; iX++) {
          int indexTable = static_cast<int>((Pi < gradPhRow.at(iX)?
                                             gradPhRow.at(iX)-Pi:
                                             gradPhRow.at(iX))*1024/Pi);
          sincos(gradPhRow.at(iX), sinTmp, cosTmp);
          gradX.at(iY,iX) = filterTable_[indexTable]*cosTmp*gradMagRow.at(iX);
          gradY.at(iY,iX) = filterTable_[indexTable]*sinTmp*gradMagRow.at(iX);

        }
      }
    }


    _lti_debug("Creating the Jacobian\n");

    //The Jacobian
    destJXX.emultiply(gradX,gradX);
    destJXY.emultiply(gradX,gradY);
    destJYY.emultiply(gradY,gradY);

    /*for (iY=0; iY<= yLastRow; iY++) {
      const fvector& gradXRow =gradX.getRow(iY);
      const fvector& gradYRow =gradY.getRow(iY);
      for (iX=0; iX<= xLastColumn; iX++) {

          destJXX.at(iY,iX) = gradXRow.at(iX)*gradXRow.at(iX);
          destJXY.at(iY,iX) = gradXRow.at(iX)*gradYRow.at(iX);
          destJYY.at(iY,iX) = gradYRow.at(iX)*gradYRow.at(iX);

      }
    }*/

    // Do the gaussian smoothing
    parApplyConv.setKernel(kernelSmoot);
    parApplyConv.boundaryType = par.smootBoundary;
    applyConv.setParameters(parApplyConv);

    _lti_debug("Filtering the Jacobian\n");

    applyConv.apply(destJXX);
    applyConv.apply(destJXY);
    applyConv.apply(destJYY);

    return true;

  }

  bool coherenceEnhancingDiffusion::
  eigenVector(const channel::value_type& jXX,
              const channel::value_type& jXY,
              const channel::value_type& jYY,
              float&                     mu1,
              float&                     mu2,
              float&                     v1X,
              float&                     v1Y,
              float&                     v2X,
              float&                     v2Y) const {
    float tmp, mag;

    // Compute the eigenvectors of J, v1 and v2
    tmp = sqrt((sqr(jXX - jYY)) + 4*(jXY)*(jXY));
    v2X = 2*jXY;
    v2Y = jYY - jXX + tmp;

    /* Normalize */
    mag = sqrt(((v2X)*(v2X)) + ((v2Y)*(v2Y)));
    if(mag!=0) {
      v2X /= mag;
      v2Y /= mag;
    }

    // The eigenvectors are orthogonal
    v1X = -v2Y; v1Y = v2X;

    // Compute the eigenvalues
    mu1 = 0.5f*(jXX + jYY + tmp);
    mu2 = 0.5f*(jXX + jYY - tmp);

    // Make output structure
    if(abs(mu1)>abs(mu2)) {
      tmp=mu1;
      mu1=mu2;
      mu2=tmp;
    } else {
      tmp=v1X;
      mag=v1Y;
      v1Y=v2Y;
      v1X=v2X;
      v2X=tmp;
      v2Y=mag;
    }
    return true;
  }

  bool
  coherenceEnhancingDiffusion::structDiffusionTensor(const channel& jXX,
                                                     const channel& jXY,
                                                     const channel& jYY,
                                                     channel&       dXX,
                                                     channel&       dXY,
                                                     channel&       dYY)
                                                     const {

    const parameters& par = getParameters();
    float mu1, mu2, v2X, v2Y, v1X, v1Y;
    float di;
    float lambda1, lambda2;
    int iY, iX;

    // Eps for finite values
    float eps=1e-20f;

    _lti_debug("Building the diffusion tensor\n");

    for (iY=0; iY<= jXX.lastRow(); iY++) {
      const fvector& jXXRow =jXX.getRow(iY);
      const fvector& jXYRow =jXY.getRow(iY);
      const fvector& jYYRow =jYY.getRow(iY);
      for (iX=0; iX<= jXX.lastColumn(); iX++) {
        eigenVector(jXXRow.at(iX), jXYRow.at(iX), jYYRow.at(iX),
                                                  mu1, mu2, v1X, v1Y, v2X, v2Y);
        di=(mu1-mu2);
        if((di<eps)&&(di>-eps)) {
            lambda1 = par.smallRegularisation;
        }
        else {
            lambda1 = par.smallRegularisation
            + (1.0f - par.smallRegularisation)
            *static_cast<float>(exp(-(par.contrast)/(di*di)));
        }
        lambda2 = par.smallRegularisation;

        // Construct the diffusion tensor
        dXX.at(iY,iX) = lambda1*v1X*v1X + lambda2*v2X*v2X;
        dXY.at(iY,iX) = lambda1*v1X*v1Y + lambda2*v2X*v2Y;
        dYY.at(iY,iX) = lambda1*v1Y*v1Y + lambda2*v2Y*v2Y;
      }
    }

    return true;
  }


  bool
  coherenceEnhancingDiffusion::structDiffusionTensor(const channel& jXX,
                                                     const channel& jXY,
                                                     const channel& jYY,
                                                     const float    tensorExpo,
                                                     channel&       dXX,
                                                     channel&       dXY,
                                                     channel&       dYY)
                                                     const {

    const parameters& par = getParameters();
    float mu1, mu2, v2X, v2Y, v1X, v1Y;
    float di;
    float lambda1, lambda2;
    int iY, iX;

    // Eps for finite values
    float eps=1e-20f;

    _lti_debug("Building the diffusion tensor\n");

    for (iY=0; iY<= jXX.lastRow(); iY++) {
      const fvector& jXXRow =jXX.getRow(iY);
      const fvector& jXYRow =jXY.getRow(iY);
      const fvector& jYYRow =jYY.getRow(iY);
      for (iX=0; iX<= jXX.lastColumn(); iX++) {
        eigenVector(jXXRow.at(iX), jXYRow.at(iX), jYYRow.at(iX),
                                                  mu1, mu2, v1X, v1Y, v2X, v2Y);
        di=(mu1-mu2);
        if((di<eps)&&(di>-eps)) {
            lambda1 = par.smallRegularisation;
        }
        else {
            lambda1 = par.smallRegularisation
            + (1.0f - par.smallRegularisation)
              *static_cast<float>(exp(-(par.contrast)/pow(di,(2.0f*tensorExpo))));
        }
        lambda2 = par.smallRegularisation;

        // Construct the diffusion tensor
        dXX.at(iY,iX) = lambda1*v1X*v1X + lambda2*v2X*v2X;
        dXY.at(iY,iX) = lambda1*v1X*v1Y + lambda2*v2X*v2Y;
        dYY.at(iY,iX) = lambda1*v1Y*v1Y + lambda2*v2Y*v2Y;
      }
    }

    return true;
  }

  bool
  coherenceEnhancingDiffusion::diffusionSchemeRotInvariance(const channel& src,
                                                            const channel& dXX,
                                                            const channel& dXY,
                                                            const channel& dYY,
                                                            channel&       dest)
                                                            const{

    const parameters& par = getParameters();
    int iX,iY;

    channel j1, j2, gradX, gradY, chTmp, chTmp2;
    gradientFunctor grad;
    gradientFunctor::parameters gradPar;

    //Parameters for the gradient
    gradPar.format = gradientFunctor::Cartesian;
    gradPar.kernelType = par.gradientKernelType;
    gradPar.gradientKernelSize = par.gradientKernelSize;
    grad.setParameters(gradPar);

    grad.apply(src, gradX, gradY);

    //Prepare the channels
    j1.assign(src.lastRow()+1,src.lastColumn()+1,0);
    j2.assign(src.lastRow()+1,src.lastColumn()+1,0);

    // 3 : Calculate the flux components
    // j1 = Dxx .* ux + Dxy .*uy;
    // j2 = Dxy .* ux + Dyy .*uy;
    _lti_debug("Calculating the flux components\n");

    chTmp.emultiply(dXX, gradX);
    chTmp2.emultiply(dYY, gradY);
    j1.emultiply(dXY, gradY);
    j2.emultiply(dXY, gradX);
    j1.add(chTmp);
    j2.add(chTmp2);

    /*for (iY=0; iY<= src.lastRow(); iY++) {
      const fvector& gradXRow =gradX.getRow(iY);
      const fvector& gradYRow =gradY.getRow(iY);
      const fvector& dXXRow =dXX.getRow(iY);
      const fvector& dXYRow =dXY.getRow(iY);
      const fvector& dYYRow =dYY.getRow(iY);
      for (iX=0; iX<= src.lastColumn(); iX++) {
          j1.at(iY,iX) = dXXRow.at(iX)*gradXRow.at(iX)
                         +  dXYRow.at(iX)*gradYRow.at(iX);
          j2.at(iY,iX) = dXYRow.at(iX)*gradXRow.at(iX)
                         + dYYRow.at(iX)*gradYRow.at(iX);
      }
    }*/


    // j1(1,:)=0; j1(end,:)=0; j1(:,1)=0; j1(:,end)=0;
    // j2(1,:)=0; j2(end,:)=0; j2(:,1)=0; j2(:,end)=0;
    _lti_debug("Making zero the frame of the imagen\n");

    for (iX=0; iX<= src.lastColumn(); iX++) {
      j1.at(0,iX)=0;
      j2.at(0,iX)=0;
      j1.at(j1.lastRow(),iX)=0;
      j2.at(j2.lastRow(),iX)=0;
    }

    for (iY=0; iY<= src.lastRow(); iY++) {
      j1.at(iY,0)=0;
      j2.at(iY,0)=0;
      j1.at(iY,j1.lastColumn())=0;
      j2.at(iY,j2.lastColumn())=0;
    }
    
    
    // 4 : Calculate ... by means of the optimized derivative filters
    // du = derivatives(j1,'x')+derivatives(j2,'y');
    _lti_debug("Calculate by means of the optimized derivative filters\n");
    
    grad.apply(j1, gradX, chTmp);
    grad.apply(j2, chTmp, gradY);
    gradX.add(gradY);
    /*for (iY=0; iY<= src.lastRow(); iY++) {
      for (iX=0; iX<= src.lastColumn(); iX++) {
          gradX.at(iY,iX) += gradY.at(iY,iX);
      }
    }*/




    // 5 : Update in an explicit way.
    // u=u+du*dt;
               
    _lti_debug("Creating the final imagen\n");

    dest.addScaled(src, par.stepDiffusionTime, gradX);
    /*for (iY=0; iY<= src.lastRow(); iY++) {
      const fvector& srcRow = src.getRow(iY);
      const fvector& gradXRow =gradX.getRow(iY);
      const fvector& gradYRow =gradY.getRow(iY);
      for (iX=0; iX<= src.lastColumn(); iX++) {
         dest.at(iY,iX) = srcRow.at(iX)
         + (gradXRow.at(iX)+gradYRow.at(iX))*par.stepDiffusionTime;
      }
    }*/


    return true;

    }


  bool coherenceEnhancingDiffusion::phaseFilterFunction(const float& mag,
                                                        float        phase,
                                                        float&       destMag)
                                                        const {
    //Regular case
    if((0.0f<=minFilPh_)&&(maxFilPh_<=Pi)){
      if((maxFilPh_ <= phase)||(phase <= minFilPh_)){
        destMag = 0.0f;
      }
      else if((maxWinPh_ < phase)&&(phase < maxFilPh_)){
        destMag = static_cast<float>(cos((phase-maxWinPh_)*Pi/(2*phaseCosWidth_)));
        destMag *= mag;
      }
      else if((minFilPh_ < phase)&&(phase < minWinPh_)){
        destMag = static_cast<float>(sin((phase-minFilPh_)*Pi/(2*phaseCosWidth_)));
        destMag *= mag;
      }
      else{
        destMag = mag;
      }
    }

    //When the filter pass Pi
    else if(Pi<=maxFilPh_){

      if(Pi <= maxWinPh_){
        if((phase <= minFilPh_)&&((maxFilPh_-Pi) <= phase)){
          destMag = 0.0f;
        }
        else if(((maxWinPh_-Pi)<phase)&&(phase<(maxFilPh_-Pi))){
          destMag = static_cast<float>(cos((phase-maxWinPh_-Pi)*Pi/(2*phaseCosWidth_))) * mag;
        }
        else if((minFilPh_ < phase) && (phase < minWinPh_)){
          destMag = static_cast<float>(sin((phase-minFilPh_)*Pi/(2*phaseCosWidth_)))*mag;
        }
        else{
          destMag = mag;
        }
      }
      else{
        if((phase <= minFilPh_)&&((maxFilPh_-Pi) <= phase)){
          destMag = 0.0f;
        }
        else if((maxWinPh_ < phase) && (phase < maxFilPh_)){
          destMag = static_cast<float>(cos((phase-maxWinPh_)*Pi/(2*phaseCosWidth_)))*mag;
        }
        else if(phase < (maxFilPh_-Pi)){
          destMag = static_cast<float>(cos((phase-maxWinPh_-Pi)*Pi/(2*phaseCosWidth_)))*mag;
        }
        else if((minFilPh_ < phase) && (phase < minWinPh_)){
          destMag = static_cast<float>(sin((phase-minFilPh_)*Pi/(2*phaseCosWidth_)))*mag;
        }
        else{
          destMag = mag;
        }
      }
    }

    //When the filter is littler than cero
    else{
      if(minWinPh_ < 0){
        if((phase <= (minFilPh_+Pi))&&((maxFilPh_) <= phase)){
          destMag = 0.0f;
        }
        else if((maxWinPh_ < phase)&&(phase < maxFilPh_)){
          destMag = static_cast<float>(cos((phase-maxWinPh_)*Pi/(2*phaseCosWidth_))) * mag;
        }
        else if(((minFilPh_+Pi) < phase) && (phase < minWinPh_+Pi)){
          destMag = static_cast<float>(sin((phase-minFilPh_+Pi)*Pi/(2*phaseCosWidth_)))* mag;
        }
        else{
          destMag = mag;
        }
      }
      else{
        if((phase <= (minFilPh_+Pi))&&((maxFilPh_) <= phase)){
          destMag = 0.0f;
        }
        else if((maxWinPh_ < phase) && (phase < maxFilPh_)){
          destMag = static_cast<float>(cos((phase-maxWinPh_)*Pi/(2*phaseCosWidth_))) * mag;
        }
        else if((minFilPh_ < phase) && (phase < minWinPh_)){
          destMag = static_cast<float>(sin((phase-minFilPh_)*Pi/(2*phaseCosWidth_))) * mag;
        }
        else if((minFilPh_+Pi) < phase){
          destMag = static_cast<float>(sin((phase-minFilPh_+Pi)*Pi/(2*phaseCosWidth_))) * mag;
        }
        else{
          destMag = mag;
        }
      }
    }

    return true;
  }

}

