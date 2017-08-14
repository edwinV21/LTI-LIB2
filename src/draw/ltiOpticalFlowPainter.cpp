/*
 * Copyright (C) 2012
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
 * \file   ltiOpticalFlowPainter.cpp
 *         Contains the class opticalFlowPainter,
 *         which has still to be documented by Pablo Alvarado.
 * 
 * \author Pablo Alvarado
 * \date   29.08.2012
 *
 * revisions ..: $Id: ltiOpticalFlowPainter.cpp,v 1.6 2012-10-08 02:54:12 alvarado Exp $
 */

#include "ltiOpticalFlowPainter.h"
#include "ltiFilledUpsampling.h"
#include "ltiGaussKernels.h"
#include "ltiDownsampling.h"
#include "ltiDraw.h"
#include "ltiConstants.h"
#include "ltiMath.h"
#include "ltiRGBPixel.h"

namespace lti {
  // --------------------------------------------------
  // opticalFlowPainter::parameters
  // --------------------------------------------------

  // default constructor
  opticalFlowPainter::parameters::parameters()
    : functor::parameters() {
    tipColor = rgbaPixel(255,255,255,128);
    backgroundColor = rgbaPixel(0,0,0,0);
    overlayMode = NoOverlay;
    threshold = 0.1f;
    zoom = 1;
    lineColor = rgbaPixel(255,255,255,128);
    samplingRate = int(5);
    scalingMode = GridWidth;
    vectorMode = Arrows;
  }

  // copy constructor
  opticalFlowPainter::parameters::parameters(const parameters& other)
    : functor::parameters() {
    copy(other);
  }

  // destructor
  opticalFlowPainter::parameters::~parameters() {
  }

  // copy member

  opticalFlowPainter::parameters&
  opticalFlowPainter::parameters::copy(const parameters& other) {
    functor::parameters::copy(other);

    tipColor = other.tipColor;
    backgroundColor = other.backgroundColor;
    overlayMode = other.overlayMode;
    threshold = other.threshold;
    zoom = other.zoom;
    lineColor = other.lineColor;
    samplingRate = other.samplingRate;
    scalingMode = other.scalingMode;
    vectorMode = other.vectorMode;

    return *this;
  }

  // alias for copy method
  opticalFlowPainter::parameters&
  opticalFlowPainter::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  // class name
  const std::string& opticalFlowPainter::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone method
  opticalFlowPainter::parameters*
  opticalFlowPainter::parameters::clone() const {
    return new parameters(*this);
  }

  // new instance
  opticalFlowPainter::parameters*
  opticalFlowPainter::parameters::newInstance() const {
    return new parameters();
  }

  /*
   * Write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool opticalFlowPainter::parameters::write(ioHandler& handler,
                                         const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {
      lti::write(handler,"tipColor",tipColor);
      lti::write(handler,"backgroundColor",backgroundColor);
      lti::write(handler,"overlayMode",overlayMode);
      lti::write(handler,"threshold",threshold);
      lti::write(handler,"zoom",zoom);
      lti::write(handler,"lineColor",lineColor);
      lti::write(handler,"samplingRate",samplingRate);
      lti::write(handler,"scalingMode",scalingMode);
      lti::write(handler,"vectorMode",vectorMode);
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
  bool opticalFlowPainter::parameters::read(ioHandler& handler,
                                        const bool complete)  {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {
      lti::read(handler,"tipColor",tipColor);
      lti::read(handler,"backgroundColor",backgroundColor);
      lti::read(handler,"overlayMode",overlayMode);
      lti::read(handler,"threshold",threshold);
      lti::read(handler,"zoom",zoom);
      lti::read(handler,"lineColor",lineColor);
      lti::read(handler,"samplingRate",samplingRate);
      lti::read(handler,"scalingMode",scalingMode);
      lti::read(handler,"vectorMode",vectorMode);
    }

    b = b && functor::parameters::read(handler,false);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

  //
  // Static members and attributes
  //
  palette opticalFlowPainter::colorWheel_;

  // --------------------------------------------------
  // opticalFlowPainter
  // --------------------------------------------------

  // default constructor
  opticalFlowPainter::opticalFlowPainter()
    : functor(),maxMag_(0.0f) {

    initColorWheel();

    // create an instance of the parameters with the default values
    parameters defaultParameters;
    // set the default parameters
    setParameters(defaultParameters);
  }

  // default constructor
  opticalFlowPainter::opticalFlowPainter(const parameters& par)
    : functor(),maxMag_(0.0f) {

    initColorWheel();

    // set the given parameters
    setParameters(par);
  }


  // copy constructor
  opticalFlowPainter::opticalFlowPainter(const opticalFlowPainter& other)
    : functor() {

    initColorWheel();

    copy(other);
  }

  // destructor
  opticalFlowPainter::~opticalFlowPainter() {
  }

  // copy member
  opticalFlowPainter& 
  opticalFlowPainter::copy(const opticalFlowPainter& other) {
    functor::copy(other);

    chart_.copy(other.chart_);
    maxMag_=other.maxMag_;

    return (*this);
  }

  // alias for copy member
  opticalFlowPainter&
  opticalFlowPainter::operator=(const opticalFlowPainter& other) {
    return (copy(other));
  }

  // class name
  const std::string& opticalFlowPainter::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone member
  opticalFlowPainter* opticalFlowPainter::clone() const {
    return new opticalFlowPainter(*this);
  }

  // create a new instance
  opticalFlowPainter* opticalFlowPainter::newInstance() const {
    return new opticalFlowPainter();
  }

  // return parameters
  const opticalFlowPainter::parameters&
  opticalFlowPainter::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }

  bool opticalFlowPainter::updateParameters() {
    const parameters& par = getParameters();
    if (par.vectorMode == Color) {
      renderColorChart(chart_);
    }
    return true;
  }

  // -------------------------------------------------------------------
  // The apply() member functions
  // -------------------------------------------------------------------

  // On place apply for type fmatrix!
  bool opticalFlowPainter::apply(const fmatrix& x,const fmatrix& y,
                                 image& canvas) const {
    float maxMag;

    return apply(x,y,canvas,maxMag);
  }
  
  // On place apply for type fmatrix!
  bool opticalFlowPainter::apply(const fmatrix& x,const fmatrix& y,
                                 image& canvas,float& maxMag) const {

    // x and y components must have the same size
    if (x.size()!=y.size()) {
      setStatusString("Input components have different size");
      return false;
    }

    if (x.empty()) {
      setStatusString("Input components empty");
      canvas.clear();
      return false;      
    }

    int inRows = x.rows();
    int inCols = x.columns();

    // Get Parameters
    const parameters& params = getParameters();

    if (params.vectorMode == Color) {
      return colorFlow(x,y,canvas,maxMag);
    } 

    // Kernel for channel downsampling
    gaussKernel2D<channel::value_type> kernel(params.samplingRate);

    // Parameters for channel downsampling
    downsampling::parameters dsParams;
    dsParams.factor = ipoint(params.samplingRate,params.samplingRate);
    dsParams.boundaryType = Constant;
    dsParams.setKernel(kernel);

    // the downsampler
    downsampling downsampler(dsParams);

    // Downsample input channels
    channel ang, mag, xDown, yDown;
    downsampler.apply(x,xDown);
    downsampler.apply(y,yDown);

    // Calculation of mag and ang and determination of maxmag
    mag.allocate(xDown.size());
    ang.allocate(xDown.size());
   
    // initial value just to ensure valid range of values
    maxMag = sqrt(xDown.at(0,0)*xDown.at(0,0)+
                  yDown.at(0,0)*yDown.at(0,0));

    if (params.scalingMode == RecallMax) {
      maxMag=max(maxMag,maxMag_);
    }

    fmatrix::const_iterator xIt,yIt;
    const fmatrix::const_iterator e = xDown.end();
    fmatrix::iterator magIt,angIt;

    magIt = mag.begin();
    angIt = ang.begin();

    // downsampled computation of magnitude and angle of the vector field
    for ( xIt = xDown.begin(),yIt = yDown.begin();xIt!=e;++xIt,++yIt)	{
      
      const float cmag = sqrt( (*xIt)*(*xIt) + (*yIt)*(*yIt) );
      
      (*magIt) = cmag;
      if (cmag > maxMag ) {
          maxMag = cmag;
      }

      (*angIt)	= atan2( (*yIt) , (*xIt) );
      magIt++;
      angIt++;
    }

    const ipoint canvasSize(params.zoom*inCols,params.zoom*inRows);

    // Prepare canvas for overlayMode
    switch (params.overlayMode)	{
    case NoOverlay: { // for flow field on black background
      // Create clear channel
      canvas.assign(canvasSize,params.backgroundColor);
    }	break;
      
    case OnOutput: { // for overlaying flow field on image,
      // which is given as destination for the canvas
      ipoint dimensions = canvas.size();
      if ( (dimensions != ipoint(0,0)) && (dimensions != canvasSize) )	{
        // prepare upsampler
        lti::filledUpsampling upsampler(ipoint(params.zoom,params.zoom));
        // Upsample overlay image given in canvas
        upsampler.apply(canvas);
      }
      // Adjust size (in case, the given image had incorrect size)
      canvas.resize(canvasSize,params.backgroundColor);
    }	break;

    case OnMagnitude: { // for overlaying flow field on value

      canvas.allocate(x.size());
      image::iterator cIt = canvas.begin();
      const fmatrix::const_iterator xe=x.end();
      for ( xIt = x.begin(),yIt = y.begin();xIt!=xe;++xIt,++yIt,++cIt)	{
        const float cmag = within(sqrt((*xIt)*(*xIt)+(*yIt)*(*yIt))/maxMag,
                                  0.0f,1.0f);
        const ubyte bmag = static_cast<ubyte>(cmag*255.0f);
        cIt->set(bmag,bmag,bmag,0);
      }
      
      // prepare upsampler
      lti::filledUpsampling upsampler(ipoint(params.zoom,params.zoom));
      // Upsample overlay image given in canvas
      upsampler.apply(canvas);

    }	break;

      
    case OnPhase: { // for overlaying flow field on ang
    
      canvas.allocate(x.size());
      image::iterator cIt = canvas.begin();
      const fmatrix::const_iterator xe=x.end();
      for ( xIt = x.begin(),yIt = y.begin();xIt!=xe;++xIt,++yIt,++cIt)	{
        const float cmag = within(atan2(*yIt,*xIt)/constants<float>::twoPi(),
                                  0.0f,1.0f);
        const ubyte bmag = static_cast<ubyte>(cmag*255.0f);
        cIt->set(bmag,bmag,bmag,0);
      }
      
      // prepare upsampler
      lti::filledUpsampling upsampler(ipoint(params.zoom,params.zoom));
      // Upsample overlay image given in canvas
      upsampler.apply(canvas);
    }	break;
    default:
      break;
    }
    
    // Prepare canvas and painters for drawing
    lti::draw<rgbaPixel> linePainter, tipPainter;
    linePainter.use(canvas); // for drawing flow lines
    linePainter.setColor(params.lineColor);
    tipPainter.use(canvas);	// for drawing the tips,
                            // which indicate the direction
    tipPainter.setColor(params.tipColor);

    // factor for scaling flow lines (maxMag corresponds to gridwidth
    // of canvas in pixels)
    float vectorLength=0.0f;
    switch (params.scalingMode) {
    case GridWidth:
      vectorLength = static_cast<float>(params.samplingRate*params.zoom)/maxMag;
      break;
    case NoScaling:
      vectorLength = static_cast<float>(params.zoom);
      break;
    case RecallMax:
      vectorLength = static_cast<float>(params.samplingRate*params.zoom)/maxMag;
      maxMag_=maxMag;
      break;
    default:
      break;
    }

    float tipLength = static_cast<float>(params.samplingRate*params.zoom)/5;

    // Variables for temporary storage of flowvector tip-positions
    float xTip, yTip;

    // Draw canvas
    if (maxMag == 0.0) {
      // ready! (image empty!)
      return true;
    }

    float threshold = params.threshold*maxMag;
    const int a = params.samplingRate*params.zoom;

    for(int i = 0;i < mag.rows();i++) {
      for(int j=0;j < mag.columns();j++) {
        if (mag.at(i,j)>=threshold)	{
          
          xTip = a*j+vectorLength*xDown.at(i,j);
          yTip = a*i+vectorLength*yDown.at(i,j);
          switch (params.vectorMode) {
          case Arrows: {
            linePainter.arrow(a*j,a*i,
                              static_cast<int>(xTip),
                              static_cast<int>(yTip),tipLength);
            break;
          }
          case Lines: {
            linePainter.line(a*j,a*i,
                             static_cast<int>(xTip),
                             static_cast<int>(yTip));
              break;
          }
          default: 
            break;
          }
          tipPainter.set(static_cast<int>(xTip),static_cast<int>(yTip)); 
        }
      }
    }

    return true;
  }

  /*
   * Initialize the color wheel
   */
  void opticalFlowPainter::initColorWheel() {
    if (!colorWheel_.empty()) {
      return;
    }
    
    // relative lengths of color transitions:
    // these are chosen based on perceptual similarity
    // (e.g. one can distinguish more shades between red and yellow 
    //  than between yellow and green)
    // [David Scharstein]

    // here at ltilib we keep the values for consistency with papers
    
    static const int RY = 15;
    static const int YG = 6;
    static const int GC = 4;
    static const int CB = 11;
    static const int BM = 13;
    static const int MR = 6;

    const int ncols = RY + YG + GC + CB + BM + MR;

    colorWheel_.allocate(ncols);

    int k=0;
    for (int i = 0; i < RY; i++) {
      colorWheel_.at(k++).set(255,255*i/RY,0);
    }
    
    for (int i = 0; i < YG; i++) {
      colorWheel_.at(k++).set(255-255*i/YG,255,0);
    }

    for (int i = 0; i < GC; i++) {
      colorWheel_.at(k++).set(0,255,255*i/GC);
    }

    for (int i = 0; i < CB; i++) {
      colorWheel_.at(k++).set(0,255-255*i/CB, 255);
    }

    for (int i = 0; i < BM; i++) {
      colorWheel_.at(k++).set(255*i/BM,0,255);
    }

    for (int i = 0; i < MR; i++) {
      colorWheel_.at(k++).set(255,0,255-255*i/MR);
    }
    
  }

   /*
    * Paint color chart
    */
  void opticalFlowPainter::renderColorChart(image& chart) const {
    
    // initialize chart
    chart.allocate(ChartSide,ChartSide);

    // position of zero at the center of the chart
    const int center = chart.lastColumn()/2;

    const frgbPixel ones(1.0f,1.0f,1.0f);

    // fill colors in
    for (int y=0;y<ChartSide;++y) {
      const float yc=static_cast<float>(y-center);

      for (int x=0;x<ChartSide;++x) {
        // centered coordinates
        float xc=static_cast<float>(x-center);

        // convert them to polar coordinates
        const float m = sqrt(xc*xc+yc*yc)/center; // magnitude
        const float a = atan2(-yc,-xc)/constants<float>::pi(); // angle
        const float fk = (a+1.0f)/2.0f*colorWheel_.lastIdx(); // index in wheel
        
        const int k0 = static_cast<int>(fk); // integer part
        const int k1 = (k0+1)%colorWheel_.size();
        const float f = fk-float(k0); // fractional part
        
        const rgbPixel<float> col0(rgbPixel<float>(colorWheel_.at(k0))/255.0f);
        const rgbPixel<float> col1(rgbPixel<float>(colorWheel_.at(k1))/255.0f);

        // linear interpolation of the saturated color
        rgbPixel<float> col = col0*(1.0f-f) + col1*f;

        if (m <= 1.0f) {
          // increase saturation with radius
          col = ones - (ones-col)*m;
        } else {
          col*=0.75f; // out of range
        }
        col*=255.0f;
        
        chart.at(y,x)=col.getClippedRGBAPixel();
      }
    }

  }

  void opticalFlowPainter::colorChart(image& chart) const {
    const parameters& par = getParameters();
    if (par.vectorMode == Color) {
      chart.copy(chart_);
    } else {
      renderColorChart(chart);
    }

  }  

  /**
   * Paint color chart
   *
   * Additionally to the other method, this one paints in black
   * the axes.
   */
  void opticalFlowPainter::colorChart(image& chart, 
                                      const float maxMagnitude) const {
    colorChart(chart);
    int center = chart.lastColumn()/2;

    // maxMagnitude represents the 
    
    lti::draw<rgbaPixel> painter;
    painter.use(chart);
    painter.setColor(lti::rgbaPixel(0,0,0,0));

    painter.line(center,0,center,chart.lastRow());
    painter.line(0,center,chart.lastColumn(),center);

    int maxCoor = static_cast<int>(maxMagnitude);
    const float m = (chart.lastColumn()-center)/(maxMagnitude);
    const float b = static_cast<float>(center);
    for (int i=-maxCoor;i<=maxCoor;++i) {
      const int coor=iround(m*i+b);
      const int s=(abs(i)%5==0)?5:2;
      
      painter.line(center-s,coor,center+s,coor);
      painter.line(coor,center-s,coor,center+s);
    }

  }

    /**
     * Color Flow encoding
     */
  bool opticalFlowPainter::colorFlow(const fmatrix& x,const fmatrix& y,
                                     image& canvas,float& maxMag) const {
    // Get Parameters
    const parameters& params = getParameters();

    fmatrix::const_iterator xIt,yIt;
    const fmatrix::const_iterator e = x.end();

    maxMag=0.0f;

    // search for maximum magnitude
    for (xIt=x.begin(),yIt=y.begin();xIt!=e;++xIt,++yIt) {
      const float cmag = (*xIt)*(*xIt) + (*yIt)*(*yIt);

      if (cmag > maxMag ) {
          maxMag = cmag;
      }
    }

    maxMag = sqrt(maxMag);

    // now create the canvas with the proper size, and using nearest neighbor
    // interpolation
    canvas.allocate(x.size()*params.zoom);
    
    const int center = chart_.lastColumn()/2;

    for (int yy=0;yy<canvas.rows();++yy) {
      const int oy = (yy/params.zoom);
      for (int xx=0;xx<canvas.columns();++xx) {
        const int ox = (xx/params.zoom);
      
        const int cx = within(iround(center*(1.f+x.at(oy,ox)/maxMag)),
                              0,ChartSide-1);
        const int cy = within(iround(center*(1.f+y.at(oy,ox)/maxMag)),
                              0,ChartSide-1);

        canvas.at(yy,xx).copy(chart_.at(cy,cx));
      }
    }
    return true;
  }


  /**
   * Read a opticalFlowPainter::eOverlayMode
   *
   * @ingroup gStorable
   */
  bool read(ioHandler& handler,
            opticalFlowPainter::eOverlayMode& data) {

    std::string str;
    
    if (handler.read(str)) {
      
      if (str.find("No") != std::string::npos) {
        data = opticalFlowPainter::NoOverlay;
      } else if (str.find("Output") != std::string::npos) {
        data = opticalFlowPainter::OnOutput;
      } else if (str.find("Mag") != std::string::npos) {
        data = opticalFlowPainter::OnMagnitude;
      } else if (str.find("Phase") != std::string::npos) {
        data = opticalFlowPainter::OnPhase;
      } else {
        data = opticalFlowPainter::NoOverlay;
      }

      return true;
    }

    return false;
  }

  /**
   * Write a opticalFlowPainter::eOverlayMode
   *
   * @ingroup gStorable
   */
  bool write(ioHandler& handler,
             const opticalFlowPainter::eOverlayMode& data) {
    bool b = false;
    switch(data) {
    case opticalFlowPainter::NoOverlay:
        b = handler.write("NoOverlay");
        break;
      case opticalFlowPainter::OnOutput:
        b = handler.write("OnOutput");
        break;
      case opticalFlowPainter::OnMagnitude:
        b = handler.write("OnMagnitude");
        break;
      case opticalFlowPainter::OnPhase:
        b = handler.write("OnPhase");
        break;
      default:
        handler.write("Unknown");
        b = false;
        handler.setStatusString("undefined opticalFlowPainter::eOverlayMode");
        break;
    }

    return b;

  }

  /**
   * Read a opticalFlowPainter::eVectorMode
   *
   * @ingroup gStorable
   */
  bool read(ioHandler& handler,
            opticalFlowPainter::eVectorMode& data) {

    std::string str;
    
    if (handler.read(str)) {
      
      if (str.find("Arrow") != std::string::npos) {
        data = opticalFlowPainter::Arrows;
      } else if (str.find("Line") != std::string::npos) {
        data = opticalFlowPainter::Lines;
      } else if (str.find("Color") != std::string::npos) {
        data = opticalFlowPainter::Color;
      } else {
        data = opticalFlowPainter::Arrows;
      }

      return true;
    }

    return false;
  }

  /**
   * Write a opticalFlowPainter::eVectorMode
   *
   * @ingroup gStorable
   */
  bool write(ioHandler& handler,
             const opticalFlowPainter::eVectorMode& data)  {
    bool b = false;
    switch(data) {
    case opticalFlowPainter::Arrows:
        b = handler.write("Arrows");
        break;
      case opticalFlowPainter::Lines:
        b = handler.write("Lines");
        break;
      case opticalFlowPainter::Color:
        b = handler.write("Color");
        break;
      default:
        handler.write("Unknown");
        b = false;
        handler.setStatusString("undefined opticalFlowPainter::eVectorMode");
        break;
    }

    return b;

  }

  /**
   * Read a opticalFlowPainter::eScalingMode
   *
   * @ingroup gStorable
   */
  bool read(ioHandler& handler,
            opticalFlowPainter::eScalingMode& data) {

    std::string str;
    
    if (handler.read(str)) {
      
      if (str.find("No") != std::string::npos) {
        data = opticalFlowPainter::NoScaling;
      } else if (str.find("Grid") != std::string::npos) {
        data = opticalFlowPainter::GridWidth;
      } else if (str.find("Recall") != std::string::npos) {
        data = opticalFlowPainter::RecallMax;
      } else {
        data = opticalFlowPainter::NoScaling;
      }

      return true;
    }

    return false;
  }

  /**
   * Write a opticalFlowPainter::eScalingMode
   *
   * @ingroup gStorable
   */
  bool write(ioHandler& handler,
             const opticalFlowPainter::eScalingMode& data)  {
    bool b = false;
    switch(data) {
    case opticalFlowPainter::GridWidth:
      b = handler.write("GridWidth");
      break;
    case opticalFlowPainter::NoScaling:
      b = handler.write("NoScaling");
      break;
    case opticalFlowPainter::RecallMax:
      b = handler.write("RecallMax");
      break;
    default:
      handler.write("Unknown");
      b = false;
      handler.setStatusString("undefined opticalFlowPainter::eScalingMode");
      break;
    }

    return b;

  }




}

