/*
 * Copyright (C) 2003, 2004, 2005, 2006
 * Lehrstuhl für Technische Informatik
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
 * \file   ltiCWAGMSegmentation.cpp
 *         Segmentation method based on a graph merge approach.
 * \author Pablo Alvarado
 * \date   30.06.2003
 *
 * revisions ..: $Id: ltiCWAGMSegmentation.cpp,v 1.4 2010-08-28 02:17:25 alvarado Exp $
 */

#include "ltiMacroSymbols.h"

#include "ltiMath.h"
#include "ltiCWAGMSegmentation.h"
#include "ltiConstants.h"
#include "ltiFactory.h"
#include "ltiSplitImageToRGB.h"
#include "ltiMergeRGBToImage.h"
#include "ltiConvolution.h"
#include "ltiGaussKernels.h"
#include "ltiComputePalette.h"

#undef _LTI_DEBUG
// #define _LTI_DEBUG 4
#include "ltiDebug.h"

#ifdef _LTI_DEBUG
#include <fstream>
#include "ltiViewer2D.h"
#include "ltiTimer.h"
#endif

namespace lti {

  // --------------------------------------------------
  // cwagmSegmentation::parameters
  // --------------------------------------------------

  // default constructor
  cwagmSegmentation::parameters::parameters()
    : segmentation::parameters() {

    medianParam.boundaryType = Constant; // very important!
    enhanceContrast = false;

    colorSplitter = "XYZ";

    watershedParam.neighborhood8 = true;
    watershedParam.rainfall      = true;
    watershedParam.threshold     = 255; // means here should be automatically 
                                        // determined.

    harisRegionMergeParam.mergeMode=regionGraphColorHaris::Optimal;
    harisRegionMergeParam.mergeThreshold = 1;
    harisRegionMergeParam.minRegionNumber = 10;

    minProbForWatershedThreshold = 0.45f;
  }

  // copy constructor
  cwagmSegmentation::parameters::parameters(const parameters& other)
    : segmentation::parameters() {
    copy(other);
  }

  // destructor
  cwagmSegmentation::parameters::~parameters() {
  }

  // get type name
  const std::string& cwagmSegmentation::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // copy member

  cwagmSegmentation::parameters&
  cwagmSegmentation::parameters::copy(const parameters& other) {
    segmentation::parameters::copy(other);

    // preprocessing
    medianParam.copy(other.medianParam);
    enhanceContrast = other.enhanceContrast;

    // split general
    colorSplitter = other.colorSplitter;
    colorContrastParam.copy(other.colorContrastParam);

    // split watershed
    watershedParam.copy(other.watershedParam);
    minProbForWatershedThreshold=other.minProbForWatershedThreshold;
    harisRegionMergeParam = other.harisRegionMergeParam;
    
    return *this;
  }

  // alias for copy member
  cwagmSegmentation::parameters&
  cwagmSegmentation::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  // clone member
  cwagmSegmentation::parameters*
  cwagmSegmentation::parameters::clone() const {
    return new parameters(*this);
  }

  cwagmSegmentation::parameters*
  cwagmSegmentation::parameters::newInstance() const {
    return new parameters;
  }

  /*
   * write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool cwagmSegmentation::parameters::write(ioHandler& handler,
                                            const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {
      // preprocessing
      handler.writeEOL();
      handler.writeComment("Preprocessing");
      lti::write(handler,"medianParam",medianParam);
      lti::write(handler,"enhanceContrast",enhanceContrast);

      // general split configuration
      handler.writeEOL();
      handler.writeComment("Watershed split and merge configuration");
      
      lti::write(handler,"colorSplitter",colorSplitter);
      lti::write(handler,"colorContrastParam",colorContrastParam);

      // split with watersheds
      lti::write(handler,"watershedParam",watershedParam);
      lti::write(handler,"minProbForWatershedThreshold",
                 minProbForWatershedThreshold);
      lti::write(handler,"harisRegionMergeParam",harisRegionMergeParam);

    }

    b = b && segmentation::parameters::write(handler,false);

    if (complete) {
      b = b && handler.writeEnd();
    }

    return b;
  }

  /*
   * read the parameters from the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also read, otherwise only the data block will be read.
   * @return true if write was successful
   */
  bool cwagmSegmentation::parameters::read(ioHandler& handler,
                                           const bool complete) {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {
      // preprocessing
      b = lti::read(handler,"medianParam",medianParam) && b;
      b = lti::read(handler,"enhanceContrast",enhanceContrast) && b;
      
      b = lti::read(handler,"colorSplitter",colorSplitter) && b;
      b = lti::read(handler,"colorContrastParam",colorContrastParam) && b;
      
      // split with watersheds
      b = lti::read(handler,"watershedParam",watershedParam) && b;
      b = lti::read(handler,"minProbForWatershedThreshold",
                minProbForWatershedThreshold) && b;
      b = lti::read(handler,"harisRegionMergeParam",
                    harisRegionMergeParam) && b;
    }

    b = b && segmentation::parameters::read(handler,false);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

  // --------------------------------------------------
  // cwagmSegmentation
  // --------------------------------------------------

  // default constructor
  cwagmSegmentation::cwagmSegmentation()
    : segmentation(),splitter_(0) {

    // create an instance of the parameters with the default values
    parameters defaultParameters;
    // set the default parameters
    setParameters(defaultParameters);
  }

  // default constructor
  cwagmSegmentation::cwagmSegmentation(const parameters& par)
    : segmentation(),splitter_(0) {

    // set the given parameters
    setParameters(par);
  }


  // copy constructor
  cwagmSegmentation::cwagmSegmentation(const cwagmSegmentation& other)
    : segmentation(),splitter_(0) {
    copy(other);
  }

  // destructor
  cwagmSegmentation::~cwagmSegmentation() {
    delete splitter_;
    splitter_=0;
  }

  // returns the name of this type
  const std::string& cwagmSegmentation::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // copy member
  cwagmSegmentation&
  cwagmSegmentation::copy(const cwagmSegmentation& other) {
    segmentation::copy(other);

    return (*this);
  }

  // alias for copy member
  cwagmSegmentation&
  cwagmSegmentation::operator=(const cwagmSegmentation& other) {
    return (copy(other));
  }
  
  // clone member
  cwagmSegmentation* cwagmSegmentation::clone() const {
    return new cwagmSegmentation(*this);
  }

  cwagmSegmentation* cwagmSegmentation::newInstance() const {
    return new cwagmSegmentation;
  }

  // return parameters
  const cwagmSegmentation::parameters&
  cwagmSegmentation::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }

  bool cwagmSegmentation::updateParameters() {
    bool res(false);
    if (segmentation::updateParameters()) {

      res = true;
      const parameters& par = getParameters();
      
      res = res && median_.setParameters(par.medianParam);
      res = res && colorContrast_.setParameters(par.colorContrastParam);
      
      //res = res && watershed_.setParameters(par.watershedParam);
      //res = res && haris_.setParameters(par.harisRegionMergeParam);
      
      delete splitter_;
      splitter_=0;
      
      splitter_ = 
        factory<splitImage>::getFactory().newInstance(par.colorSplitter);
      if (isNull(splitter_)) {
        std::string str = "Wrong color splitter functor name ";
        str += par.colorSplitter;
        setStatusString(str.c_str());
        res = false;
      }
    }
    return res;
  }

  // -------------------------------------------------------------------
  // The apply-methods!
  // -------------------------------------------------------------------

  // On copy apply for type image!
  bool cwagmSegmentation::apply(const image& src,
                                      imatrix& regions) const {
    ivector regionSizes;
    return apply(src,regions,regionSizes);
  }

  // On copy apply for type image!
  bool cwagmSegmentation::apply(const image& src,
                                      imatrix& regions,
                                      ivector& regionSizes) const {
    channel gradM,gradA;
    image preproc;
    return apply(src,preproc,regions,regionSizes,gradM,gradA);
  }


  // On copy apply for type image!
  bool cwagmSegmentation::apply(const image& src,
                                      image& preproc,
                                      imatrix& regions,
                                      ivector& regionSizes) const {
    channel gradM,gradA;
    return apply(src,preproc,regions,regionSizes,gradM,gradA);
  }

  // On copy apply for type image!
  bool cwagmSegmentation::apply(const image& src,
                                      image& preproc,
                                      imatrix& regions,
                                      ivector& regionSizes,
                                      channel& gradM,
                                      channel& gradA) const {

    // split and remove small regions and edges
    vector<float> gradHisto;
    float maxGrad;

    return 
      preprocess(src,preproc) &&
      worker(src,preproc,regions,regionSizes,gradM,gradA,gradHisto,maxGrad);
  }

  /*
   * Preprocess
   * Denoise and Enhance Contrast
   */
  bool cwagmSegmentation::preprocess(const image& src,image& dest) const {

    const parameters& param = getParameters();

    if (param.medianParam.kernelSize < 2) {
      dest.copy(src);
    } else {
      // denoise with median filter (each color channel)
      channel8 R,G,B;
      splitImageToRGB splitterRGB;
      mergeRGBToImage merger;
      
      splitterRGB.apply(src,R,G,B);
      median_.apply(R);
      median_.apply(G);
      median_.apply(B);
      merger.apply(R,G,B,dest);
    }

#   if defined(_LTI_DEBUG) && (_LTI_DEBUG > 2)
    static viewer2D viewM("Median");
    viewM.show(dest);
#   endif

    // enhance contrast
    if (param.enhanceContrast) {
      enhanceContrast(dest);
    }

#   if defined(_LTI_DEBUG) && (_LTI_DEBUG > 2)
    static viewer2D viewC("Contrast");
    viewC.show(dest);
#   endif

    return true;
  }

  // ----------------------------------
  // split image in homogeneous regions
  // ----------------------------------

  bool cwagmSegmentation::worker(const image& ,
                                 const image& src,
                                 imatrix& regions,
                                 ivector& regionSizes,
                                 channel& gradM,
                                 channel& gradA,
                                 vector<float>& gradHisto,
                                 float& maxGrad) const {

    const parameters& param = getParameters();

    if (isNull(splitter_)) {
      return false;
    }

    // First, get the color gradient
    channel c1,c2,c3;
    splitter_->apply(src,c1,c2,c3); // in the right color space...

    // color contrast gradient
    colorContrast_.apply(c1,c2,c3,gradM,gradA,maxGrad);

    // cast gradient and compute histogram
    gradHisto.assign(256,0.0f);

    // watershed works only with channel8
    channel8 gradM8;
    gradM8.allocate(gradM.size());
    
    const float factor = gradHisto.lastIdx()/maxGrad;
    const int   size   = gradM.rows()*gradM.columns();

    int i;
    float val;
    for (i=0;i<size;++i) {
      val = gradM.elem(i); 
      gradHisto.at(gradM8.elem(i)=iround(val*factor))++;
    }
    gradHisto.divide(static_cast<float>(size));

    watershedSegmentation::parameters wsPar(param.watershedParam);

    if (wsPar.threshold == 255) {
      // automatic detection of threshold
      
      vector<float> gaHisto(gradHisto.size(),0.0f);
      
      // accumulative histogram
      gaHisto.at(0)=gradHisto.at(0);
      for (i=gaHisto.size()-2;i>=0;--i) {
        gaHisto.at(i)=gaHisto.at(i+1)+gradHisto.at(i);
      }

      // threshold from probabilities directly
      const float pfactor = 1.0f-param.minProbForWatershedThreshold;
      for (i=0;gaHisto.at(i)>pfactor;++i) {
      }
      wsPar.threshold = i;
 
    }

    watershedSegmentation watershed(wsPar);

    // now apply the watershed segmentation
    if (!watershed.apply(gradM8,regions)) {
      setStatusString(std::string("Watershed failed: ") +
                      watershed.getStatusString());
      return false;
    }

#   if defined(_LTI_DEBUG) && (_LTI_DEBUG > 2)
    static viewer2D view("Grad8"),vieww("Watersheds"),viewl("lines");
    _lti_debug("Threshold used: " << int(wsPar.threshold) << std::endl);
    view.show(gradM8);
    vieww.show(regions);
    channel8 wslines;
    watershed.apply(gradM8,wslines);
    viewl.show(wslines);
#   endif

    // reduce the oversegmentation using Haris et.al. distance merging strategy
    // extended to color spaces.
    regionGraphColorHaris::graph_type graph;
    ivector equivLabels;

    regionGraphColorHaris haris(param.harisRegionMergeParam);
    haris.apply(regions,c1,c2,c3,0,graph); // generate the graph
    haris.apply(graph,equivLabels);        // merge
    haris.reassignLabels(equivLabels,regions,regionSizes,true);
 
    return true;
  }

  /*
   * The simplest contrast enhancement consists in mapping the input value
   * range of each channel into 0 to 255, each channel independently of
   * the others.
   */
  bool cwagmSegmentation::enhanceContrast(image& src) const {
    if (src.empty()) {
      return true;
    }

    // first find min and max values for each component
    image::iterator it,eit;
    rgbaPixel minPx(src.at(0,0)),maxPx(src.at(0,0));

    for (it=src.begin(),eit=src.end();it!=eit;++it) {
      if ((*it).getRed() < minPx.getRed()) {
        minPx.setRed((*it).getRed());
      } else if ((*it).getRed() > maxPx.getRed()) {
        maxPx.setRed((*it).getRed());
      }

      if ((*it).getGreen() < minPx.getGreen()) {
        minPx.setGreen((*it).getGreen());
      } else if ((*it).getGreen() > maxPx.getGreen()) {
        maxPx.setGreen((*it).getGreen());
      }

      if ((*it).getBlue() < minPx.getBlue()) {
        minPx.setBlue((*it).getBlue());
      } else if ((*it).getBlue() > maxPx.getBlue()) {
        maxPx.setBlue((*it).getBlue());
      }
    }

    static bool eachChannelIndependently = false;

    if (eachChannelIndependently) {
      // maxima and minima for each channel found, now scale each channel with
      // the proper linear equations
      const int nr = static_cast<int>(maxPx.getRed())   - minPx.getRed();
      const int ng = static_cast<int>(maxPx.getGreen()) - minPx.getGreen();
      const int nb = static_cast<int>(maxPx.getBlue())  - minPx.getBlue();
      
      const float fr = (nr == 0) ? 0.0f : 255.0f/nr;
      const float fg = (ng == 0) ? 0.0f : 255.0f/ng;
      const float fb = (nb == 0) ? 0.0f : 255.0f/nb;
      
      for (it=src.begin(),eit=src.end();it!=eit;++it) {
        (*it).set(static_cast<ubyte>(fr*((*it).getRed()-minPx.getRed())),
                  static_cast<ubyte>(fg*((*it).getGreen()-minPx.getGreen())),
                  static_cast<ubyte>(fb*((*it).getBlue()-minPx.getBlue())),
                  0);
      }
    } else {
      const int maxVal = max(maxPx.getRed(),maxPx.getGreen(),maxPx.getBlue());
      const int minVal = min(minPx.getRed(),minPx.getGreen(),minPx.getBlue());
      const int nd = maxVal-minVal;
      const float f = (nd == 0) ? 0.0f : 255.0f/nd;

      for (it=src.begin(),eit=src.end();it!=eit;++it) {
        (*it).set(static_cast<ubyte>(f*((*it).getRed()-minVal)),
                  static_cast<ubyte>(f*((*it).getGreen()-minVal)),
                  static_cast<ubyte>(f*((*it).getBlue()-minVal)),
                  0);
      }
    }

    return true;
  }

}
