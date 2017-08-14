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
 * \file   ltiViewer1DPainter.cpp
 *         Contains the class viewer1DPainter.
 * \author Pablo Alvarado
 * \date   05.07.2005
 *
 * revisions ..: $Id: ltiViewer1DPainter.cpp,v 1.2 2010-01-15 05:01:05 alvarado Exp $
 */

#include "ltiViewer1DPainter.h"
#include "ltiLabelAdjacencyMap.h"
#include "ltiMath.h"
#include "ltiRound.h"
#include "ltiHistogramEqualization.h"
#include "ltiColors.h"

namespace lti {
  // --------------------------------------------------
  // viewer1DPainter::parameters
  // --------------------------------------------------

  // default constructor
  viewer1DPainter::parameters::parameters()
    : parametersManager::parameters() {

    backgroundColor = Black;
    axisColor = Grey75;
    gridColor = Grey50;
    lineColor = White;
    meanColor = Red;
    markerColor = Green;
    marker = ".";
    zoom.set(1.f,1.f);
    drawHorizontalAxis = true;
    drawMeanValue = false;
    drawGrid = false;
    ticks.set(1,1);
    forceZero = false;
    pixelsPerElement = 3;
    vectorHeight = 256;
    style = Boxes;
    
  }

  // copy constructor
  viewer1DPainter::parameters::parameters(const parameters& other)
    : parametersManager::parameters() {
    copy(other);
  }

  // destructor
  viewer1DPainter::parameters::~parameters() {
  }

  // copy member

  viewer1DPainter::parameters&
  viewer1DPainter::parameters::copy(const parameters& other) {
    parametersManager::parameters::copy(other);

    backgroundColor    = other.backgroundColor ;
    axisColor          = other.axisColor;
    gridColor          = other.gridColor;
    lineColor          = other.lineColor;
    meanColor          = other.meanColor;
    markerColor        = other.markerColor;
    marker             = other.marker;
    zoom.copy(other.zoom);
    drawHorizontalAxis = other.drawHorizontalAxis;
    drawMeanValue      = other.drawMeanValue;
    drawGrid           = other.drawGrid;
    ticks.copy(other.ticks);
    forceZero          = other.forceZero;
    pixelsPerElement   = other.pixelsPerElement;
    vectorHeight       = other.vectorHeight;
    style              = other.style;

    return *this;
  }

  // alias for copy method
  viewer1DPainter::parameters&
  viewer1DPainter::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  // class name
  const std::string& viewer1DPainter::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone method
  viewer1DPainter::parameters* viewer1DPainter::parameters::clone() const {
    return new parameters(*this);
  }

  // new instance
  viewer1DPainter::parameters* 
  viewer1DPainter::parameters::newInstance() const {
    return new parameters();
  }

  /*
   * Write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool viewer1DPainter::parameters::write(ioHandler& handler,
                                          const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {      

      lti::write(handler,"backgroundColor",backgroundColor);
      lti::write(handler,"axisColor",axisColor);
      lti::write(handler,"gridColor",gridColor);
      lti::write(handler,"lineColor",lineColor);
      lti::write(handler,"meanColor",meanColor);
      lti::write(handler,"markerColor",markerColor);
      lti::write(handler,"marker",marker);
      lti::write(handler,"zoom",zoom);
      lti::write(handler,"drawHorizontalAxis",drawHorizontalAxis);
      lti::write(handler,"drawMeanValue",drawMeanValue);
      lti::write(handler,"drawGrid",drawGrid);
      lti::write(handler,"ticks",ticks);
      lti::write(handler,"forceZero",forceZero);
      lti::write(handler,"pixelsPerElement",pixelsPerElement);
      lti::write(handler,"vectorHeight",vectorHeight);
      lti::write(handler,"style",style);

    }

    b = b && parametersManager::parameters::write(handler,false);

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
  bool viewer1DPainter::parameters::read(ioHandler& handler,
                                        const bool complete)  {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {
      lti::read(handler,"backgroundColor",backgroundColor);
      lti::read(handler,"axisColor",axisColor);
      lti::read(handler,"gridColor",gridColor);
      lti::read(handler,"lineColor",lineColor);
      lti::read(handler,"meanColor",meanColor);
      lti::read(handler,"markerColor",markerColor);
      lti::read(handler,"marker",marker);
      lti::read(handler,"zoom",zoom);
      lti::read(handler,"drawHorizontalAxis",drawHorizontalAxis);
      lti::read(handler,"drawMeanValue",drawMeanValue);
      lti::read(handler,"drawGrid",drawGrid);
      lti::read(handler,"ticks",ticks);
      lti::read(handler,"forceZero",forceZero);
      lti::read(handler,"pixelsPerElement",pixelsPerElement);
      lti::read(handler,"vectorHeight",vectorHeight);
      lti::read(handler,"style",style);
    }

    b = b && parametersManager::parameters::read(handler,false);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

  // --------------------------------------------------
  // viewer1DPainter
  // --------------------------------------------------

  // default constructor
  viewer1DPainter::viewer1DPainter()
    : functor() {

    // create an instance of the parameters with the default values
    parameters defaultParameters;
    // set the default parameters
    setParameters(defaultParameters);
  }

  // default constructor
  viewer1DPainter::viewer1DPainter(const parameters& par)
    : functor() {

    // set the given parameters
    setParameters(par);

  }


  // copy constructor
  viewer1DPainter::viewer1DPainter(const viewer1DPainter& other)
    : functor() {

    copy(other);
  }

  // destructor
  viewer1DPainter::~viewer1DPainter() {
  }

  // copy member
  viewer1DPainter& 
  viewer1DPainter::copy(const viewer1DPainter& other) {
    functor::copy(other);

    cache_.invalidate();
    return (*this);
  }

  // alias for copy member
  viewer1DPainter&
  viewer1DPainter::operator=(const viewer1DPainter& other) {
    return (copy(other));
  }

  // class name
  const std::string& viewer1DPainter::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone member
  viewer1DPainter* viewer1DPainter::clone() const {
    return new viewer1DPainter(*this);
  }

  // create a new instance
  viewer1DPainter* viewer1DPainter::newInstance() const {
    return new viewer1DPainter();
  }

  void viewer1DPainter::setZoom(const fpoint& zoomFactor) {
    getRWParameters().zoom=zoomFactor;
  }

  // return parameters
  const viewer1DPainter::parameters& viewer1DPainter::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }

  // return RW parameters
  viewer1DPainter::parameters& viewer1DPainter::getRWParameters() {
    parameters* par = dynamic_cast<parameters*>(&functor::getRWParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }

  bool viewer1DPainter::updateParameters() {
    const parameters& par = getParameters();

    vectorHeight_ = static_cast<int>(par.zoom.y*par.vectorHeight);
    haveNewParameters_ = true;

    return true;
  }


  // -------------------------------------------------------------------
  // Statistics cache management
  // -------------------------------------------------------------------

  // -----------------
  // Cache Block
  // -----------------
  viewer1DPainter::cacheBlock::cacheBlock() 
    : minMaxOk_(false) {
  }

  void viewer1DPainter::cacheBlock::invalidate() {
    minMaxOk_ = false;
  }

  // -----------------
  // Cache Manager
  // -----------------

  viewer1DPainter::cacheManager::cacheManager() 
    : dataPtr_(0),dataSize_(0) {
  }

  void viewer1DPainter::cacheManager::invalidate() {
    blocks.invalidate();
    
    dataPtr_  = 0;
    dataSize_ = 0;
  }

  void viewer1DPainter::invalidateCache() {
    cache_.invalidate();
  }
  
  // -------------------------------------------------------------------
  // The apply-methods!
  // -------------------------------------------------------------------

  /**
   * Read a viewer1DPainter::eStyle
   *
   * @ingroup gStorable
   */
  bool read(ioHandler& handler,
            viewer1DPainter::eStyle& data) {
    std::string str;
    
    if (handler.read(str)) {
      
      if (str.find("ots") != std::string::npos) {
        data = viewer1DPainter::Dots;
      } else if (str.find("ox") != std::string::npos) {
        data = viewer1DPainter::Boxes;
      } else if (str.find("evel") != std::string::npos) {
        data = viewer1DPainter::Steps;
      } else if (str.find("mpuls") != std::string::npos) {
        data = viewer1DPainter::Impulses;
      } else if (str.find("ine") != std::string::npos) {
        data = viewer1DPainter::Lines;
      } else {
        data = viewer1DPainter::Dots;
      }

      return true;
    }

    return false;
  }

  /**
   * Write a viewer1DPainter::eStyle
   *
   * @ingroup gStorable
   */
  bool write(ioHandler& handler,
             const viewer1DPainter::eStyle& data) {
    bool b = false;
    switch(data) {
    case viewer1DPainter::Dots:
        b = handler.write("Dots");
        break;
      case viewer1DPainter::Boxes:
        b = handler.write("Boxes");
        break;
      case viewer1DPainter::Steps:
        b = handler.write("Steps");
        break;
      case viewer1DPainter::Impulses:
        b = handler.write("Impulses");
        break;
      case viewer1DPainter::Lines:
        b = handler.write("Lines");
        break;
      default:
        handler.write("Unknown");
        b = false;
        handler.setStatusString("undefined viewer1DPainter::eStyle");
        break;
    }

    return b;
  }


}

