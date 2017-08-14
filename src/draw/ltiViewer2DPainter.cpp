/*
 * Copyright (C) 1998-2006
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

/*
 * \file   ltiViewer2DPainter.cpp
 *         Contains the class viewer2DPainter.
 * \author Pablo Alvarado
 * \date   05.07.2005
 *
 * revisions ..: $Id: ltiViewer2DPainter.cpp,v 1.18 2010-01-02 04:35:18 alvarado Exp $
 */

#include "ltiViewer2DPainter.h"
#include "ltiLabelAdjacencyMap.h"
#include "ltiMath.h"
#include "ltiRound.h"
#include "ltiHistogramEqualization.h"
#include "ltiColors.h"

namespace lti {
  // --------------------------------------------------
  // viewer2DPainter::parameters
  // --------------------------------------------------

  // default constructor
  viewer2DPainter::parameters::parameters()
    : parametersManager::parameters() {
    zoom = 1;
    mappingType = Original;
    offset = 0;
    slope = 1.0f;
    // identity map as default user defined map
    userMapping.resize(256,0);
    for (int i=0;i<userMapping.size();++i) {
      userMapping.at(i)=i;
    }
    
    hexadecimal = false;
    underflowColor = Blue;
    overflowColor = Red;
    paletteType = GrayPalette;
    userPalette.copy(labelAdjacencyMap::defaultPalette);
    labelAdjacency = false;
    adjacencyNeighborhood = FourNeighborhood;
    minAdjacencyColors = true;
  }

  // copy constructor
  viewer2DPainter::parameters::parameters(const parameters& other)
    : parametersManager::parameters() {
    copy(other);
  }

  // destructor
  viewer2DPainter::parameters::~parameters() {
  }

  // copy member

  viewer2DPainter::parameters&
  viewer2DPainter::parameters::copy(const parameters& other) {
    parametersManager::parameters::copy(other);

    zoom = other.zoom;
    mappingType = other.mappingType;
    offset = other.offset;
    slope = other.slope;
    userMapping.copy(other.userMapping);
    hexadecimal = other.hexadecimal;
    underflowColor = other.underflowColor;
    overflowColor = other.overflowColor;
    paletteType = other.paletteType;
    userPalette.copy(other.userPalette);
    labelAdjacency = other.labelAdjacency;
    adjacencyNeighborhood = other.adjacencyNeighborhood;
    minAdjacencyColors = other.minAdjacencyColors;

    return *this;
  }

  // alias for copy method
  viewer2DPainter::parameters&
  viewer2DPainter::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  // class name
  const std::string& viewer2DPainter::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone method
  viewer2DPainter::parameters* viewer2DPainter::parameters::clone() const {
    return new parameters(*this);
  }

  // new instance
  viewer2DPainter::parameters* 
  viewer2DPainter::parameters::newInstance() const {
    return new parameters();
  }

  /*
   * Write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool viewer2DPainter::parameters::write(ioHandler& handler,
                                          const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {      
      lti::write(handler,"zoom",zoom);
      switch (mappingType) {
        case Original:
          lti::write(handler,"mappingType","Original");
          break;
        case OffsetSlope:
          lti::write(handler,"mappingType","OffsetSlope");
          break;
        case MinToBlack:
          lti::write(handler,"mappingType","MinToBlack");
          break;
        case MaxToWhite:
          lti::write(handler,"mappingType","MaxToWhite");
          break;
        case Optimal:
          lti::write(handler,"mappingType","Optimal");
          break;
        case HistogramEq:
          lti::write(handler,"mappingType","HistogramEq");
          break;
        case Log:
          lti::write(handler,"mappingType","Log");
          break;
        case OptimalLog:
          lti::write(handler,"mappingType","OptimalLog");
          break;
        case UserDefMap:
          lti::write(handler,"mappingType","UserDefMap");
          break;
        default:
          lti::write(handler,"mappingType","Original");
          break;
      }

      lti::write(handler,"offset",offset);
      lti::write(handler,"slope",slope);
      lti::write(handler,"userMapping",userMapping);
      lti::write(handler,"hexadecimal",hexadecimal);
      lti::write(handler,"underflowColor",underflowColor);
      lti::write(handler,"overflowColor",overflowColor);

      switch(paletteType) {
        case GrayPalette:
          lti::write(handler,"paletteType","GrayPalette");
          break;
        case RedPalette:
          lti::write(handler,"paletteType","RedPalette");
          break;
        case GreenPalette:
          lti::write(handler,"paletteType","GreenPalette");
          break;
        case BluePalette:
          lti::write(handler,"paletteType","BluePalette");
          break;
        case HuePalette:
          lti::write(handler,"paletteType","HuePalette");
          break;
        case UserDefPalette:
          lti::write(handler,"paletteType","UserDefPalette");
          break;
        default:
          lti::write(handler,"paletteType","GrayPalette");
          break;
      }


      lti::write(handler,"userPalette",userPalette);
      lti::write(handler,"labelAdjacency",labelAdjacency);

      switch(adjacencyNeighborhood) {
        case FourNeighborhood:
          lti::write(handler,"adjacencyNeighborhood","FourNeighborhood");
          break;
        case EightNeighborhood:
          lti::write(handler,"adjacencyNeighborhood","EightNeighborhood");
          break;
        default:
          lti::write(handler,"adjacencyNeighborhood","FourNeighborhood");
          break;
      }

      lti::write(handler,"minAdjacencyColors",minAdjacencyColors);
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
  bool viewer2DPainter::parameters::read(ioHandler& handler,
                                        const bool complete)  {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {
      
      lti::read(handler,"zoom",zoom);
      std::string tmp;
      lti::read(handler,"mappingType",tmp);
      
      if (tmp == "Original") {
        mappingType = Original;
      } else if (tmp == "OffsetSlope") {
        mappingType = OffsetSlope;
      } else if (tmp == "MinToBlack") {
        mappingType = MinToBlack;
      } else if (tmp == "MaxToWhite") {
        mappingType = MaxToWhite;
      } else if (tmp == "Optimal") {
        mappingType = Optimal;
      } else if (tmp == "HistogramEq") {
        mappingType = HistogramEq;
      } else if (tmp == "Log") {
        mappingType = Log;
      } else if (tmp == "OptimalLog") {
        mappingType = OptimalLog;
      } else if (tmp == "UserDefMap") {
        mappingType = UserDefMap;
      } else {
        mappingType = Original;
      }
      
      lti::read(handler,"offset",offset);
      lti::read(handler,"slope",slope);
      lti::read(handler,"userMapping",userMapping);
      lti::read(handler,"hexadecimal",hexadecimal);
      lti::read(handler,"underflowColor",underflowColor);
      lti::read(handler,"overflowColor",overflowColor);

      lti::read(handler,"paletteType",tmp);
      if (tmp.find("ray") != std::string::npos) {
        paletteType = GrayPalette;
      } else if (tmp.find("ed") != std::string::npos) {
        paletteType = RedPalette;
      } else if (tmp.find("reen") != std::string::npos) {
        paletteType = GreenPalette;
      } else if (tmp.find("lue") != std::string::npos) {
        paletteType = BluePalette;
      } else if (tmp.find("ue") != std::string::npos) {
        paletteType = HuePalette;
      } else if (tmp.find("ser") != std::string::npos) {
        paletteType = UserDefPalette;
      } else {
        paletteType = GrayPalette;
      }

      lti::read(handler,"userPalette",userPalette);
      lti::read(handler,"labelAdjacency",labelAdjacency);

      lti::read(handler,"adjacencyNeighborhood",tmp);

      if ((tmp.find("our") != std::string::npos) ||
          (tmp.find("4") != std::string::npos) ){
        adjacencyNeighborhood = FourNeighborhood;
      } else if ((tmp.find("ight") != std::string::npos) ||
                 (tmp.find("8") != std::string::npos) ) {
        adjacencyNeighborhood = EightNeighborhood;
      } else {
        adjacencyNeighborhood = FourNeighborhood;
      }

      lti::read(handler,"minAdjacencyColors",minAdjacencyColors);
    }

    b = b && parametersManager::parameters::read(handler,false);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

  // --------------------------------------------------
  // viewer2DPainter
  // --------------------------------------------------

  // default constructor
  viewer2DPainter::viewer2DPainter()
    : functor(),histEq_(new histogramEqualization),cache_(histEq_) {
    initPalettes();

    // create an instance of the parameters with the default values
    parameters defaultParameters;
    // set the default parameters
    setParameters(defaultParameters);
  }

  // default constructor
  viewer2DPainter::viewer2DPainter(const parameters& par)
    : functor(),histEq_(new histogramEqualization),cache_(histEq_) {
    initPalettes();

    // set the given parameters
    setParameters(par);

  }


  // copy constructor
  viewer2DPainter::viewer2DPainter(const viewer2DPainter& other)
    : functor(),histEq_(new histogramEqualization),cache_(histEq_) {
    initPalettes();

    copy(other);
  }

  // destructor
  viewer2DPainter::~viewer2DPainter() {
    delete histEq_;
  }

  // copy member
  viewer2DPainter& 
  viewer2DPainter::copy(const viewer2DPainter& other) {
    functor::copy(other);

    cache_.invalidate();
    return (*this);
  }

  // alias for copy member
  viewer2DPainter&
  viewer2DPainter::operator=(const viewer2DPainter& other) {
    return (copy(other));
  }

  // class name
  const std::string& viewer2DPainter::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone member
  viewer2DPainter* viewer2DPainter::clone() const {
    return new viewer2DPainter(*this);
  }

  // create a new instance
  viewer2DPainter* viewer2DPainter::newInstance() const {
    return new viewer2DPainter();
  }

  void viewer2DPainter::setZoom(const float zoomFactor) {
    getRWParameters().zoom=zoomFactor;
  }

  // return parameters
  const viewer2DPainter::parameters&
  viewer2DPainter::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }

  // return RW parameters
  viewer2DPainter::parameters& viewer2DPainter::getRWParameters() {
    parameters* par = dynamic_cast<parameters*>(&functor::getRWParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }

  bool viewer2DPainter::updateParameters() {
    const parameters& par = getParameters();

    haveNewParameters_ = true;

    if (par.userPalette.empty()) {
      user_.copy(gray_);
      return true;
    }
    
    int i;
    static const int size = 256;
    user_.allocate(size);
    for (i=0;i<size;++i) {
      user_.at(i)=par.userPalette.at(i%par.userPalette.size());
    }
    return true;
  }

  // -------------------------------------------------------------------
  // Palettes
  // -------------------------------------------------------------------
  // static palettes at first empty
  palette viewer2DPainter::gray_;
  palette viewer2DPainter::red_;
  palette viewer2DPainter::green_;
  palette viewer2DPainter::blue_;
  palette viewer2DPainter::hue_;
  

  void viewer2DPainter::initPalettes() {
    if (gray_.empty()) {
      gray_.allocate(256);
      red_.allocate(gray_.size());
      green_.allocate(gray_.size());
      blue_.allocate(gray_.size());
      hue_.allocate(gray_.size());
      
      int i;
      for (i=0;i<gray_.size();++i) {
        gray_.at(i).set(i,i,i);
        red_.at(i).set(i,0,0);
        green_.at(i).set(0,i,0);
        blue_.at(i).set(0,0,i);
        mergeHSI_.apply(static_cast<ubyte>(i),128,192,hue_.at(i));
      }
    }
  }

  // -------------------------------------------------------------------
  // Statistics cache management
  // -------------------------------------------------------------------

  // -----------------
  // Cache Block
  // -----------------
  viewer2DPainter::cacheBlock::cacheBlock() 
    : minMaxOk_(false),histEqOk_(false) {
  }

  void viewer2DPainter::cacheBlock::invalidate() {
    minMaxOk_ = false;
    histEqOk_ = false;
  }

  // -----------------
  // Cache Manager
  // -----------------

  viewer2DPainter::cacheManager::cacheManager(histogramEqualization*& histoEq) 
    : dataPtr_(0),dataSize_(0),histoEq_(histoEq) {
  }

  bool viewer2DPainter::cacheManager::
  checkCacheMiss(const ubyte* data,
                 const ipoint& size,
                 const int cacheIdx) const {
    
    if ((dataPtr_  != static_cast<const void*>(data)) ||
        (dataSize_ != size)) {
      return true; // total cache miss! (data changed!)
    }

    return !clut_[cacheIdx].second;
  }

  bool viewer2DPainter::cacheManager::
  checkCacheMiss(const int* data,
                 const ipoint& size,
                 const int cacheIdx) const {
    
    if ((dataPtr_  != static_cast<const void*>(data)) ||
        (dataSize_ != size)) {
      return true; // total cache miss! (data changed!)
    }

    return !clut_[cacheIdx].second;
  }


  bool viewer2DPainter::cacheManager::
  checkCacheMiss(const void* data,
                 const eChannelType chnlType,
                 const ipoint& size,
                 const eMappingType mapping) const {
    
    if ((mapping == Original)    ||
        (mapping == OffsetSlope) ||
        (mapping == Log)         ||
        (mapping == UserDefMap)) {
      return false; // not necessary to update anything!
    }


    if ((dataPtr_  != data) ||
        (dataSize_ != size)) {
      return true; // total cache miss! (data changed!)
    }
    
    if (mapping == HistogramEq) {
      return (!blocks_[chnlType].histEqOk_);
    } else {
      // all other mapping types require min-max statistics
      return (!blocks_[chnlType].minMaxOk_);
    }
    
    return false;
    
  }

  void viewer2DPainter::cacheManager::invalidate() {
    int i;
    for (i=0;i<NumCacheBlocks;++i) {
      blocks_[i].invalidate();
    }
    
    for (i=0;i<ClutSize;++i) {
      clut_[i].first.clear();
      clut_[i].second = false;      
    }

    dataPtr_  = 0;
    dataSize_.set(0,0);
  }


  bool viewer2DPainter::cacheManager::
  updateSignature(const matrix<ubyte>& data) {
    bool changes = false;

    if ((static_cast<const void*>(data.data()) != dataPtr_) ||
        (dataSize_ != data.size())) {
      // Cache miss
      // invalidate all blocks
      invalidate();

      dataPtr_  = data.data();
      dataSize_ = data.size();
      changes = true;
    }

    return changes;
    
  }

  bool viewer2DPainter::cacheManager::
  updateSignature(const matrix<int32>& data) {
    bool changes = false;

    if ((static_cast<const void*>(data.data()) != dataPtr_) ||
        (dataSize_ != data.size())) {
      // Cache miss
      // invalidate all blocks
      invalidate();

      dataPtr_  = data.data();
      dataSize_ = data.size();
      changes = true;
    }

    return changes;
  }


  bool viewer2DPainter::cacheManager::update(const void* data,
                                             const eChannelType chnlType,
                                             const matrix<float>& chnl,
                                             const eMappingType mapping) {
    
    bool changes = false;
    if ((mapping == Original)    ||
        (mapping == OffsetSlope) ||
        (mapping == Log)         ||
        (mapping == UserDefMap)) {
      return false; // not necessary to update anything!
    }

    if ((data != dataPtr_) ||
        (dataSize_ != chnl.size())) {
      // Cache miss
      // invalidate all blocks
      invalidate();

      dataPtr_  = data;
      dataSize_ = chnl.size();
      changes = true;
    }

    if (mapping == HistogramEq) {
      if (!blocks_[chnlType].histEqOk_) {
        histoEq_->computeLUT(chnl,blocks_[chnlType].lut_);
        blocks_[chnlType].histEqOk_ = true;
        changes = true;
      }
    } else {
      // all other mapping types require min-max statistics
      if (!blocks_[chnlType].minMaxOk_) {
      
        float minVal,maxVal;
        chnl.findExtremes(minVal,maxVal);
        
        blocks_[chnlType].min_ = minVal;
        blocks_[chnlType].max_ = maxVal;
        blocks_[chnlType].minMaxOk_ = true;
        changes = true;

      }
    }

    return changes;
  }

  bool viewer2DPainter::cacheManager::update(const ubyte* data,
                                             const eChannelType chnlType,
                                             const matrix<ubyte>& chnl,
                                             const eMappingType mapping) {
    
    bool changes = false;

    if ((static_cast<const void*>(data) != dataPtr_) ||
        (dataSize_ != chnl.size())) {
      // Cache miss
      // invalidate all blocks
      invalidate();

      dataPtr_  = data;
      dataSize_ = chnl.size();
      changes = true;
    }

    if ((mapping == Original)    ||
        (mapping == OffsetSlope) ||
        (mapping == Log)         ||
        (mapping == UserDefMap)) {
      return changes; // not necessary to update anything!
    }

    if (mapping == HistogramEq) {

      vector<ubyte> chnl8Lut;

      if (!blocks_[chnlType].histEqOk_) {
        histoEq_->computeLUT(chnl,chnl8Lut);
        blocks_[chnlType].lut_.castFrom(chnl8Lut);
        blocks_[chnlType].histEqOk_ = true;
        changes = true;
      }
    } else {
      // all other mapping types require min-max statistics
      if (!blocks_[chnlType].minMaxOk_) {
      
        ubyte minVal,maxVal;
        chnl.findExtremes(minVal,maxVal);
        
        blocks_[chnlType].min_ = static_cast<float>(minVal);
        blocks_[chnlType].max_ = static_cast<float>(maxVal);
        blocks_[chnlType].minMaxOk_ = true;
        changes = true;

      }
    }

    return changes;
  }

  bool viewer2DPainter::cacheManager::update(const int32* data,
                                             const eChannelType chnlType,
                                             const matrix<int32>& chnl,
                                             const eMappingType mapping) {
    
    bool changes = false;

    if ((static_cast<const void*>(data) != dataPtr_) ||
        (dataSize_ != chnl.size())) {
      // Cache miss
      // invalidate all blocks
      invalidate();

      dataPtr_  = data;
      dataSize_ = chnl.size();
      changes = true;
    }

    if ((mapping == Original)    ||
        (mapping == OffsetSlope) ||
        (mapping == Log)         ||
        (mapping == UserDefMap)) {
      return changes; // not necessary to update anything!
    }
    
    // all other mapping types require min-max statistics (even histEq)
    if (!blocks_[chnlType].minMaxOk_) {
      
      int minVal,maxVal;
      chnl.findExtremes(minVal,maxVal);
      
      blocks_[chnlType].min_ = static_cast<float>(minVal);
      blocks_[chnlType].max_ = static_cast<float>(maxVal);
      blocks_[chnlType].minMaxOk_ = true;
      changes = true;      
    }

    if (mapping == HistogramEq) {
      if (!blocks_[chnlType].histEqOk_) {
        // since there is no histEq for channel32, then we have to
        // use a channel;
        channel tmpChnl;
        tmpChnl.mapLinear(chnl,
                          iround(blocks_[chnlType].min_),
                          iround(blocks_[chnlType].max_));

        histoEq_->computeLUT(tmpChnl,blocks_[chnlType].lut_);
        blocks_[chnlType].histEqOk_ = true;
        changes = true;
      }
    } 

    return changes;
  }

  // -------------------------------------------------------------------
  // The apply-methods!
  // -------------------------------------------------------------------

  void viewer2DPainter::invalidateCache() {
    cache_.invalidate();
  }
  

  // On copy apply for type image!
  bool viewer2DPainter::apply(const matrix<rgbaPixel>& src,
                              image& dest) {

    return apply(src,irectangle(ipoint(0,0),
                                ipoint(src.lastColumn(),src.lastRow())),
                 dest);
  }

  // On copy apply for type image!
  bool viewer2DPainter::apply(const matrix<float>& src,
                              image& dest) {

    return apply(src,irectangle(ipoint(0,0),
                                ipoint(src.lastColumn(),src.lastRow())),
                 dest);
  }

  // On copy apply for type image!
  bool viewer2DPainter::apply(const matrix<ubyte>& src,
                              image& dest) {

    return apply(src,irectangle(ipoint(0,0),
                                ipoint(src.lastColumn(),src.lastRow())),
                 dest);
  }

  // On copy apply for type image!
  bool viewer2DPainter::apply(const matrix<int32>& src,
                              image& dest) {

    return apply(src,irectangle(ipoint(0,0),
                                ipoint(src.lastColumn(),src.lastRow())),
                 dest);
  }

  void viewer2DPainter::analyzeViewport(const irectangle& viewport,
                                        const ipoint& dataSize,
                                        const float& scale,
                                        ipoint& startPx,
                                        ipoint& endPx) const {

    int izoom;

    // numPx holds the number of pixels in the original image that 
    // correspond to the viewport.

    if (scale >= 0.75f) {
      // zoom in
      izoom = iround(scale);
      startPx = viewport.ul/izoom;
      endPx   = viewport.br/izoom;
    } else {
      // zoom out
      izoom = iround(1.0f/scale);
      startPx = viewport.ul*izoom;
      endPx   = viewport.br*izoom;
    }

    // some last verifications

    if (endPx.x < startPx.x) {
      endPx.x = startPx.x;
    }

    if (endPx.y < startPx.y) {
      endPx.y = startPx.y;
    }

    // the window in original dimensions cannot be bigger than
    // the original image
    if (endPx.x >= dataSize.x) {
      endPx.x = dataSize.x-1;
    }

    if (endPx.y >= dataSize.y) {
      endPx.y = dataSize.y-1;
    }
    
    if (startPx.x < 0) {
      startPx.x = 0;
    }

    if (startPx.y < 0) {
      startPx.y = 0;
    }

  }

  bool viewer2DPainter::apply(const matrix<rgbaPixel>& src,
                              const irectangle& viewport,
                              image& dest) {

    const parameters& par = getParameters();
    // number of pixels in x and y directions
    ipoint startPx,endPx;

    // get the real window size in the coordinates of the original image
    analyzeViewport(viewport,src.size(),par.zoom,startPx,endPx);

    image tmp;
    render(src,startPx,endPx,tmp);
    return zoom(tmp,viewport,par.zoom,dest);
  }

  bool viewer2DPainter::apply(const matrix<float>& src,
                              const irectangle& viewport,
                              image& dest)  {

    const parameters& par = getParameters();
    // number of pixels in x and y directions
    ipoint startPx,endPx;

    // get the real window size in the coordinates of the original image
    analyzeViewport(viewport,src.size(),par.zoom,startPx,endPx);

    image tmp;
    render(src,startPx,endPx,tmp);
    return zoom(tmp,viewport,par.zoom,dest);
  }

  bool viewer2DPainter::apply(const matrix<ubyte>& src,
                              const irectangle& viewport,
                              image& dest)  {

    const parameters& par = getParameters();
    // number of pixels in x and y directions
    ipoint startPx,endPx;

    // get the real window size in the coordinates of the original image
    analyzeViewport(viewport,src.size(),par.zoom,startPx,endPx);

    image tmp;
    if (par.labelAdjacency) {
      renderLabels(src,startPx,endPx,tmp);
    } else {
      render(src,startPx,endPx,tmp);
    }

    return zoom(tmp,viewport,par.zoom,dest);
  }

  bool viewer2DPainter::apply(const matrix<int32>& src,
                              const irectangle& viewport,
                              image& dest)  {

    const parameters& par = getParameters();
    // number of pixels in x and y directions
    ipoint startPx,endPx;

    // get the real window size in the coordinates of the original image
    analyzeViewport(viewport,src.size(),par.zoom,startPx,endPx);

    image tmp;
    if (par.labelAdjacency) {
      renderLabels(src,startPx,endPx,tmp);
    } else {
      render(src,startPx,endPx,tmp);
    }

    return zoom(tmp,viewport,par.zoom,dest);
  }


  bool viewer2DPainter::render(const matrix<rgbaPixel>& src,
                               const ipoint from,
                               const ipoint to,
                               image& dest) {

    if (src.empty()) {
      dest.clear();
      return true;
    }

    const parameters& par = getParameters();

    if (par.mappingType == Original) {
      // since src is a const ref, we have to copy it first
      dest.copy(src,from,to);
      return true;
    }


    // for all other cases we need split the image in another color space
    
    switch (par.paletteType) {
      case GrayPalette: {
        
        channel hue,sat,inten;
        
        if (cache_.checkCacheMiss(src.data(),
                                  IntensityCache,
                                  src.size(),
                                  par.mappingType)) {
          
          splitHSI_.apply(src,hue,sat,inten);
          cache_.update(src.data(),IntensityCache,inten,par.mappingType);
          // extract submatrices
          hue.copy(hue,from,to);
          sat.copy(sat,from,to);
          inten.copy(inten,from,to);
          dest.allocate(to.y-from.y+1,to.x-from.x+1);
        } else {
          // use the dest container temprarily to get the slice of the src we
          // need
          dest.copy(src,from,to);
          splitHSI_.apply(dest,hue,sat,inten);
        }

        int x,y;
        
        // do the intensity channel mapping
        mapping(inten,IntensityCache);

        // merge the channels.  Since under/over-flows are possible, we have
        // to do this pixel-wise
        for (y=0;y<inten.rows();++y) {
          for (x=0;x<inten.columns();++x) {
            const float& I = inten.at(y,x);
            if (I < 0.0f) {
              // underflow
              dest.at(y,x)=par.underflowColor;
            } else if (I > 1.0f) {
              dest.at(y,x)=par.overflowColor;
            } else {
              mergeHSI_.apply(hue.at(y,x),sat.at(y,x),I,dest.at(y,x));
            }
          }
        }
      } break;
      case HuePalette: {
        
        channel hue,sat,inten;
        
        if (cache_.checkCacheMiss(src.data(),
                                  HueCache,
                                  src.size(),
                                  par.mappingType)) {
          
          splitHSI_.apply(src,hue,sat,inten);
          cache_.update(src.data(),HueCache,hue,par.mappingType);
          // extract submatrices
          hue.copy(hue,from,to);
          sat.copy(sat,from,to);
          inten.copy(inten,from,to);
          dest.allocate(to.y-from.y+1,to.x-from.x+1);
        } else {
          // use the dest container temprarily to get the slice of the src we
          // need
          dest.copy(src,from,to);
          splitHSI_.apply(dest,hue,sat,inten);
        }

        int x,y;
        
        // do the intensity channel mapping
        mapping(hue,HueCache);

        // merge the channels.  Since under/over-flows are possible, we have
        // to do this pixel-wise
        for (y=0;y<inten.rows();++y) {
          for (x=0;x<inten.columns();++x) {
            float H = hue.at(y,x);
            if (H < 0.0f) {
              // underflow
              H+=1-static_cast<int32>(H);
            } else if (H > 1.0f) {
              H-=static_cast<int32>(H);
            } 
            mergeHSI_.apply(H,sat.at(y,x),inten.at(y,x),dest.at(y,x));
          }
        }
      } break;
      case RedPalette:
      case GreenPalette:
      case BluePalette: {
        
        channel red,green,blue;
        channel* mapped;
        const eChannelType mode = static_cast<eChannelType>(RedCache +
                                                            par.paletteType -
                                                            RedPalette);
        if (cache_.checkCacheMiss(src.data(),
                                  mode,
                                  src.size(),
                                  par.mappingType)) {
          
          splitRGB_.apply(src,red,green,blue);
          
          switch (par.paletteType) {
            case RedPalette:
              mapped=&red;
              break;
            case GreenPalette:
              mapped=&green;
              break;
            default: // BluePalette
              mapped=&blue;
          }

          
          cache_.update(src.data(),mode,*mapped,par.mappingType);
          // extract submatrices
          red.copy(red,from,to);
          green.copy(green,from,to);
          blue.copy(blue,from,to);

          dest.allocate(to.y-from.y+1,to.x-from.x+1);
        } else {
          // use the dest container temporarily to get the slice of src we
          // need
          dest.copy(src,from,to);
          splitRGB_.apply(dest,red,green,blue);
        }
        
        int x,y;
        switch (par.paletteType) {
          case RedPalette:
            mapping(red,RedCache);
            mapped=&red;
            break;
          case GreenPalette:
            mapping(green,GreenCache);
            mapped=&green;
            break;
          default: // BluePalette
            mapping(blue,BlueCache);
            mapped=&blue;
        }

        // merge the channels.  Since under/over-flows are possible, we have
        // to do this pixel-wise
        for (y=0;y<red.rows();++y) {
          for (x=0;x<red.columns();++x) {
            const float& V = mapped->at(y,x);
            if (V<0.0f) {
              dest.at(y,x)=par.underflowColor;
            } else if (V>1.0f) {
              dest.at(y,x)=par.overflowColor;
            } else {
              mergeRGB_.apply(red.at(y,x),green.at(y,x),blue.at(y,x),
                             dest.at(y,x));
            }
          }
        }
      } break;
      case UserDefPalette: {
        // All RGB channels

        channel red,green,blue;
        const eMappingType mt = par.mappingType;

        if ((cache_.checkCacheMiss(src.data(),RedCache,src.size(),mt)) ||
            (cache_.checkCacheMiss(src.data(),GreenCache,src.size(),mt)) ||
            (cache_.checkCacheMiss(src.data(),BlueCache,src.size(),mt)) ) {
          splitRGB_.apply(src,red,green,blue);
          cache_.update(src.data(),RedCache,red,mt);
          cache_.update(src.data(),GreenCache,green,mt);
          cache_.update(src.data(),BlueCache,blue,mt);

          // extract submatrices
          red.copy(red,from,to);
          green.copy(green,from,to);
          blue.copy(blue,from,to);

          dest.allocate(to.y-from.y+1,to.x-from.x+1);
        } else {
          // use the dest container temporarily to get the slice of src we
          // need
          dest.copy(src,from,to);
          splitRGB_.apply(dest,red,green,blue);
        }
        
        int x,y;
        mapping(red,RedCache);
        mapping(green,GreenCache);
        mapping(blue,BlueCache);

        // merge the channels.  Since under/over-flows are possible, we have
        // to do this pixel-wise
        for (y=0;y<red.rows();++y) {
          for (x=0;x<red.columns();++x) {
            const float& R = red.at(y,x);
            const float& G = green.at(y,x);
            const float& B = blue.at(y,x);
            if (min(R,G,B)<0.0f) {
              dest.at(y,x)=par.underflowColor;
            } else if (max(R,G,B)>1.0f) {
              dest.at(y,x)=par.overflowColor;
            } else {
              mergeRGB_.apply(R,G,B,dest.at(y,x));
            }
          }
        }
      } break;

      default:
        dest.copy(src,from,to);
    }
    return true;
  }

  // -----------------------------------
  // Mapping for floating-point channels
  // Used by RGB and channels
  // -----------------------------------
  bool viewer2DPainter::mapping(matrix<float>& srcDest,
                                const eChannelType type) const {

    const parameters& par = getParameters();

    if (par.mappingType == OffsetSlope) {
      const float m = par.slope;
      const float b = par.offset;

      matrix<float>::iterator it,eit;
      for (it=srcDest.begin(),eit=srcDest.end();it!=eit;++it) {
        (*it)=m*(*it)+b;
      }

      return true;
    }

    if (par.mappingType == Log) {
      static const float m = 1.0f/log(256.0f);
      matrix<float>::iterator it,eit;
      
      for (it=srcDest.begin(),eit=srcDest.end();it!=eit;++it) {
        if ((*it)<0.0f) {
          (*it) = -1.0f; // indicate an underflow!
        } else {
          (*it) = log(255.0f*(*it)+1.0f)*m;
        }
      }

      return true;
    }
    
    if (par.mappingType == UserDefMap) {
      if (par.userMapping.size() < 256) {
        setStatusString("Wrong userMapping definition: it needs at least 256 "\
                        "elements");
        return false;
      }

      int idx;
      const vector<ubyte>& lut = par.userMapping;
      const int maxIdx = lut.size()-1;
      matrix<float>::iterator it,eit;

      for (it=srcDest.begin(),eit=srcDest.end();it!=eit;++it) {
        idx = static_cast<int32>((*it)*maxIdx);
        if (idx < 0) {
          (*it)= -1.0f; // underflow;
        } else if (idx > maxIdx) {
          (*it)= 2.0f; // overflow
        } else {
          (*it) = static_cast<float>(lut.at(idx))/255.0f;
        }
      }

      return true;
    }

    // ----------------------

    // The following mapping types need statistics of the whole image!
    // therefore, we have to play with some caching to save time!
    if (par.mappingType == HistogramEq) {
      matrix<float>::iterator it,eit;
      const vector<float>& lut = cache_.blocks_[type].lut_;
      for (it=srcDest.begin(),eit=srcDest.end();it!=eit;++it) {
        (*it)=histEq_->getEqualized((*it),lut);
      }
      
      return true;
    }

    // optimal logarithm
    
    if (par.mappingType == OptimalLog) {
      static const float k = 1.0f/log(256.0f);
      matrix<float>::iterator it,eit;
      const float theMin = cache_.blocks_[type].min_;
      const float theMax = cache_.blocks_[type].max_;

      const float m = (theMin == theMax) ? 0.0f : 255.0f/(theMax-theMin);
      const float b = 1.0f-m*theMin;
      
      for (it=srcDest.begin(),eit=srcDest.end();it!=eit;++it) {
        (*it) = log(m*(*it)+b)*k;      
      }

      return true;
    }

    if ((par.mappingType == Optimal) ||
        (par.mappingType == MaxToWhite) ||
        (par.mappingType == MinToBlack)) {
      
      // only three linear transformations remaining
      const float theMin = 
        (par.mappingType == MaxToWhite) ? 0.0f : cache_.blocks_[type].min_;
      const float theMax = 
        (par.mappingType == MinToBlack) ? 1.0f : cache_.blocks_[type].max_;
      
      const float m = (theMin == theMax) ? 0.0f : 1.0f/(theMax-theMin);
      const float b = -m*theMin;
      
      matrix<float>::iterator it,eit;
      for (it=srcDest.begin(),eit=srcDest.end();it!=eit;++it) {
        (*it) = m*(*it)+b;      
      }
    }

    return true;
  }


  // -----------------------------------
  // Mapping for int32 channels
  // Used by channel32 and similar
  // -----------------------------------
  bool viewer2DPainter::mapping(matrix<int32>& srcDest,
                                const eChannelType type) const {

    const parameters& par = getParameters();

    static const int norm = 255;

    if (par.mappingType == OffsetSlope) {
      const float m = par.slope;
      const float b = par.offset*norm;

      matrix<int32>::iterator it,eit;
      for (it=srcDest.begin(),eit=srcDest.end();it!=eit;++it) {
        (*it)=iround(m*(*it)+b);
      }

      return true;
    }

    if (par.mappingType == Log) {
      static const float m = norm/log(256.0f);
      matrix<int32>::iterator it,eit;
      
      for (it=srcDest.begin(),eit=srcDest.end();it!=eit;++it) {
        if ((*it)<0) {
          (*it) = -1; // indicate an underflow!
        } else {
          (*it) = iround(log((*it)+1.0f)*m);
        }
      }

      return true;
    }
    
    if (par.mappingType == UserDefMap) {
      if (par.userMapping.size() < 256) {
        setStatusString("Wrong userMapping definition: it needs at least 256 "\
                        "elements");
        return false;
      }

      int idx;
      const vector<ubyte>& lut = par.userMapping;
      const int maxIdx = lut.size()-1;
      matrix<int32>::iterator it,eit;

      for (it=srcDest.begin(),eit=srcDest.end();it!=eit;++it) {
        idx = (*it);
        if (idx < 0) {
          (*it)= -1; // underflow;
        } else if (idx > maxIdx) {
          (*it)= norm+1; // overflow
        } else {
          (*it) = lut.at(idx);
        }
      }

      return true;
    }

    // ----------------------

    // The following mapping types need statistics of the whole image!
    // therefore, we have to play with some caching to save time!
    if (par.mappingType == HistogramEq) {
      const vector<float>& lut = cache_.blocks_[type].lut_;
      const float cst = static_cast<float>(lut.lastIdx());
      const int minLabel = iround(cache_.blocks_[type].min_);
      const int maxLabel = iround(cache_.blocks_[type].max_);

      // This is somehow tricky: Since there is no histogram equalization for
      // channel32, we have stored the LUT of that channel mapped to the float
      // interval between 0 and 1.  Therefore, we have to take the channel
      // value, linearly map it into the interval 0..1, the result has to be
      // scaled by cst, to get the real index of the lut, and the result of the
      // lut has to be scaled by 255

      matrix<int32>::iterator it,eit;

      // linear mapping
      const float m = (maxLabel==minLabel) ? 0.0f : cst/(maxLabel-minLabel);
      const float b = -m*minLabel;

      for (it=srcDest.begin(),eit=srcDest.end();it!=eit;++it) {
        (*it)=iround(norm*lut.at(iround(m*(*it)+b)));
      }
      
      return true;
    }

    // optimal logarithm
    
    if (par.mappingType == OptimalLog) {
      static const float k = norm/log(256.0f);
      matrix<int32>::iterator it,eit;
      const float theMin = cache_.blocks_[type].min_;
      const float theMax = cache_.blocks_[type].max_;

      const float m = (theMin == theMax) ? 0.0f : 255.0f/(theMax-theMin);
      const float b = 1.0f-m*theMin;
      
      for (it=srcDest.begin(),eit=srcDest.end();it!=eit;++it) {
        (*it) = iround(log(m*(*it)+b)*k);
      }

      return true;
    }

    if ((par.mappingType == Optimal) ||
        (par.mappingType == MaxToWhite) ||
        (par.mappingType == MinToBlack)) {
      
      // only three linear transformation remaining
      const float theMin = 
        (par.mappingType == MaxToWhite) ? 0.0f : cache_.blocks_[type].min_;
      const float theMax = 
        (par.mappingType == MinToBlack) ? norm : cache_.blocks_[type].max_;
      
      const float m = (theMin == theMax) ? 0.0f : norm/(theMax-theMin);
      const float b = -m*theMin;
      
      matrix<int32>::iterator it,eit;
      for (it=srcDest.begin(),eit=srcDest.end();it!=eit;++it) {
        (*it) = iround(m*(*it)+b);
      }
    }

    return true;
  }


  // --------------------------------------------------
  // Compute LUT for channel8
  // --------------------------------------------------
  
  bool viewer2DPainter::computeC8LUT(ivector& lut) const {

    const parameters& par   = getParameters();
    const eChannelType type = IntensityCache;
    int i;
      
    lut.allocate(256);

    if (par.mappingType == Original) {
      for (i=0;i<lut.size();++i) {
        lut.at(i)=i;
      }
    }

    if (par.mappingType == OffsetSlope) {
      const float m = par.slope;
      const float b = par.offset*lut.lastIdx();
      for (i=0;i<lut.size();++i) {
        lut.at(i) = iround(m*i+b);
      }
      
      return true;
    }

    if (par.mappingType == Log) {
      static const double m = 255.0/log(256.0);
      
      for (i=0;i<lut.size();++i) {
        lut.at(i) = iround(log(i+1.0)*m);
      }

      return true;
    }
    
    if (par.mappingType == UserDefMap) {
      if (par.userMapping.size() < 256) {
        setStatusString("Wrong userMapping definition: it needs at least 256 "\
                        "elements");
        return false;
      }

      lut.castFrom(par.userMapping);

      return true;
    }

    // ----------------------

    // The following mapping types need statistics of the whole image!
    // therefore, we have to play with some caching to save time!
    if (par.mappingType == HistogramEq) {
      const vector<float>& flut = cache_.blocks_[type].lut_;
      
      for (i=0;i<lut.size();++i) {
        lut.at(i)=iround(flut.at(i));
      }
      
      return true;
    }

    // optimal logarithm
    
    if (par.mappingType == OptimalLog) {
      static const float k = 255.0f/log(256.0f);

      const float theMin = cache_.blocks_[type].min_;
      const float theMax = cache_.blocks_[type].max_;

      const float m = (theMin == theMax) ? 0.0f : 255.0f/(theMax-theMin);
      const float b = 1.0f-m*theMin;
      
      for (i=0;i<lut.size();++i) {
        lut.at(i) = iround(k*log(m*i+b));
      }

      return true;
    }

    if ((par.mappingType == Optimal) ||
        (par.mappingType == MaxToWhite) ||
        (par.mappingType == MinToBlack)) {
      
      // only three linear transformation remaining
      const float theMin = 
        (par.mappingType == MaxToWhite) ? 0.0f : cache_.blocks_[type].min_;
      const float theMax = 
        (par.mappingType == MinToBlack) ? 255.0f : cache_.blocks_[type].max_;
      
      const float m = (theMin == theMax) ? 0.0f : 255.0f/(theMax-theMin);
      const float b = -m*theMin;
      for (i=0;i<lut.size();++i) {
        lut.at(i) = iround(m*i+b);
      }
    }
    
    return true;
  }


  // -------------------------------------------------------------------------
  // Creates an image of equal size than source channel with the desired color
  // or gray scale transformation
  // -------------------------------------------------------------------------
  bool viewer2DPainter::render(const matrix<float>& src,
                               const ipoint from,
                               const ipoint to,
                               image& dest)  {

    if (src.empty()) {
      dest.clear();
      return true;
    }

    const parameters& par = getParameters();

    if (cache_.checkCacheMiss(src.data(),
                              IntensityCache,
                              src.size(),
                              par.mappingType)) {
      
      cache_.update(src.data(),IntensityCache,src,par.mappingType);
    }

    // get the window of interest.
    channel tmp;
    tmp.copy(src,from,to);
    dest.allocate(tmp.size());

    // do the intensity channel mapping
    mapping(tmp,IntensityCache);

    const palette* thePalette = 0;
    // from tmp we have to create now the image, which depends on the palette
    // type.
    switch (par.paletteType) {
      case GrayPalette:
        thePalette = &gray_;
        break;
      case RedPalette:
        thePalette = &red_;
        break;
      case GreenPalette:
        thePalette = &green_;
        break;
      case BluePalette:
        thePalette = &blue_;
        break;
      case HuePalette:
        thePalette = &hue_;
        break;
      case UserDefPalette:
        thePalette = &user_;
        break;
      default:
        thePalette = &gray_;
        break;
    }
    
    matrix<float>::const_iterator it,eit;
    matrix<rgbaPixel>::iterator dit;
    for (it=tmp.begin(),eit=tmp.end(),dit=dest.begin();it!=eit;++it,++dit) {

      // check if value in range.  Remember that an evil NaN or Inf could
      // be around!
      if ((*it)>=0.0f) {
        if ((*it)<=1.0f) {
          (*dit)=thePalette->at(iround((*it)*255));
        } else {
          (*dit)=par.overflowColor;
        }
      } else {
        (*dit)=par.underflowColor;
      }

    }
    
    return true;
  }

  // --------------------------------------------------------------------------
  // Creates an image of equal size than source channel8 with the desired color
  // or gray scale transformation
  // --------------------------------------------------------------------------
  bool viewer2DPainter::render(const matrix<ubyte>& src,
                               const ipoint from,
                               const ipoint to,
                               image& dest)  {

    if (src.empty()) {
      dest.clear();
      return true;
    }

    const parameters& par = getParameters();
    if (src.empty() || (from.x >= src.columns())) {
      // if no source data or the window lies outside the data
      dest.clear();
      return false;
    }

    if (cache_.checkCacheMiss(src.data(),
                              src.size(),
                              static_cast<int32>(par.mappingType))) {
      
      // compute all necessary statistics of the channel
      cache_.update(src.data(),IntensityCache,src,par.mappingType);
      
      // since we have a channel8, it will be always much faster to
      // compute first a LUT for whatever the user wants to do
      computeC8LUT(cache_.clut_[par.mappingType].first);

      // we set now the flag that the LUT for the given mapping type is
      // ready.  We cannot set the flag for OffsetSlope nor UserDefMap, as
      // the user can change th
      cache_.clut_[par.mappingType].second = true;
    } else if (haveNewParameters_) {
      // well, here we may loose some time, but there is no way to know if
      // the user really changed the parameters...
      if ((par.mappingType == OffsetSlope) ||
          (par.mappingType == UserDefMap)) {
        computeC8LUT(cache_.clut_[par.mappingType].first);
        cache_.clut_[par.mappingType].second = true;
      }
      haveNewParameters_ = false;
    }

    const palette* thePalette = 0;
    // from tmp we have to create now the image, which depends on the palette
    // type.
    switch (par.paletteType) {
      case GrayPalette:
        thePalette = &gray_;
        break;
      case RedPalette:
        thePalette = &red_;
        break;
      case GreenPalette:
        thePalette = &green_;
        break;
      case BluePalette:
        thePalette = &blue_;
        break;
      case HuePalette:
        thePalette = &hue_;
        break;
      case UserDefPalette:
        thePalette = &user_;
        break;
      default:
        thePalette = &gray_;
        break;
    }

    dest.allocate(to.y-from.y+1,to.x-from.x+1);
    vector<ubyte>::const_iterator it;
    vector<rgbaPixel>::iterator dit,eit;
    const ivector& lut = cache_.clut_[par.mappingType].first;
    if (lut.empty()) {
      return false;
    }
    int y,idx;
    const int ey = min(to.y,src.lastRow());
    const int lastIdx = thePalette->lastIdx();
    for (y=from.y;y<=ey;++y) {
      const vector<ubyte>& srcRow = src.getRow(y);
      vector<rgbaPixel>& destRow = dest.getRow(y-from.y);
      for (it=srcRow.begin()+from.x,dit=destRow.begin(),eit=destRow.end();
           dit!=eit;
           ++it,++dit) {
        idx = lut.at(*it);
        if (idx<0) {
          (*dit)=par.underflowColor;
        } else if (idx > lastIdx) {
          (*dit)=par.overflowColor;
        } else {
          (*dit)=thePalette->at(idx);
        }
      }
    }
    
    return true;
  }

  // --------------------------------------------------------------------------
  // Creates an image of equal size than source channel8 with the desired color
  // or gray scale transformation
  // --------------------------------------------------------------------------
  bool viewer2DPainter::renderLabels(const matrix<ubyte>& src,
                                     const ipoint from,
                                     const ipoint to,
                                     image& dest)  {

    const parameters& par = getParameters();
    if (src.empty() || (from.x >= src.columns())) {
      // if no source data or the window lies outside the data
      dest.clear();
      return false;
    }

    const int cacheIdx = 
      UserDefMap + 1 + 
      ((par.adjacencyNeighborhood == FourNeighborhood) ? 0 : 1) +
      ((par.minAdjacencyColors) ? 0 : 2);

    if (cache_.checkCacheMiss(src.data(),
                              src.size(),
                              cacheIdx)) {

      cache_.updateSignature(src);
      
      labelAdjacencyMap::parameters lamPar;
      lamPar.minColors = par.minAdjacencyColors;
      lamPar.neighborhood = 
        (par.adjacencyNeighborhood == FourNeighborhood) ? 4 : 8;
      lamPar.thePalette.copy(par.userPalette);
      labelAdjacencyMap lam(lamPar);

      labelAdjacencyMap::graph graph;

      lam.adjacency(src,graph);
      lam.computePalette(graph,cache_.clut_[cacheIdx].first);

      // we set now the flag that the LUT for the given mapping type is
      // ready.  We cannot set the flag for OffsetSlope nor UserDefMap, as
      // the user can change th
      cache_.clut_[cacheIdx].second = true;
    } 

    // copy to the destination only the desired submatrix.
    dest.allocate(to.y-from.y+1,to.x-from.x+1);
    vector<ubyte>::const_iterator it;
    vector<rgbaPixel>::iterator dit,eit;
    const ivector& lut = cache_.clut_[cacheIdx].first;
    if (lut.empty()) {
      return false;
    }
    int y,idx;
    const int ey = min(to.y,src.lastRow());
    const int lastIdx = user_.lastIdx();
    for (y=from.y;y<=ey;++y) {
      const vector<ubyte>& srcRow = src.getRow(y);
      vector<rgbaPixel>& destRow = dest.getRow(y-from.y);
      for (it=srcRow.begin()+from.x,dit=destRow.begin(),eit=destRow.end();
           dit!=eit;
           ++it,++dit) {
        idx = lut.at(*it);
        if (idx<0) {
          (*dit)=par.underflowColor;
        } else if (idx > lastIdx) {
          (*dit)=par.overflowColor;
        } else {
          (*dit)=user_.at(idx);
        }
      }
    }
    
    return true;
  }

  // --------------------------------------------------------------------------
  // Creates an image of equal size than source channel8 with the desired color
  // or gray scale transformation
  // --------------------------------------------------------------------------
  bool viewer2DPainter::renderLabels(const matrix<int32>& src,
                                     const ipoint from,
                                     const ipoint to,
                                     image& dest)  {
    
    const parameters& par = getParameters();
    if (src.empty() || (from.x >= src.columns())) {
      // if no source data or the window lies outside the data
      dest.clear();
      return false;
    }
    
    const int cacheIdx = 
      UserDefMap + 1 + 
      ((par.adjacencyNeighborhood == FourNeighborhood) ? 0 : 1) +
      ((par.minAdjacencyColors) ? 0 : 2);
    
    if (cache_.checkCacheMiss(src.data(),
                              src.size(),
                              cacheIdx)) {

      cache_.updateSignature(src);
      
      labelAdjacencyMap::parameters lamPar;
      lamPar.minColors = par.minAdjacencyColors;
      lamPar.neighborhood = 
        (par.adjacencyNeighborhood == FourNeighborhood) ? 4 : 8;
      lamPar.thePalette.copy(par.userPalette);
      labelAdjacencyMap lam(lamPar);

      labelAdjacencyMap::graph graph;

      lam.adjacency(src,graph);

      // from the graph we can get the lowest and highest labels
      // used
      int minLabel,maxLabel;
      graph.findMinMaxIds(minLabel,maxLabel);
      
      if ((minLabel < 0) || (maxLabel >= BiggestLabel)) {
        cache_.tooManyLabels_ = true;
        // ensure we have the statistics we need!
        cache_.blocks_[IntensityCache].min_ = static_cast<float>(minLabel);
        cache_.blocks_[IntensityCache].max_ = static_cast<float>(maxLabel);
      } else {
        lam.computePalette(graph,cache_.clut_[cacheIdx].first);
        cache_.tooManyLabels_ = false;
      }
      
      // we set now the flag that the LUT for the given mapping type is
      // ready.  We cannot set the flag for OffsetSlope nor UserDefMap, as
      // the user can change th
      cache_.clut_[cacheIdx].second = true;
    } 

    if (cache_.tooManyLabels_) {
      // cast just as a channel with optimal linear map
      dest.allocate(to.y-from.y+1,to.x-from.x+1);
      vector<int32>::const_iterator it;
      vector<rgbaPixel>::iterator dit,eit;
      int y;
      const int ey = min(to.y,src.lastRow());

      const float minLabel = cache_.blocks_[IntensityCache].min_;
      const float maxLabel = cache_.blocks_[IntensityCache].max_;

      const float m = 255.0f/(maxLabel-minLabel);
      const float b = -m*minLabel;

      int val;
      
      for (y=from.y;y<=ey;++y) {
        const vector<int32>& srcRow = src.getRow(y);
        vector<rgbaPixel>& destRow = dest.getRow(y-from.y);
        for (it=srcRow.begin()+from.x,dit=destRow.begin(),eit=destRow.end();
             dit!=eit;
             ++it,++dit) {
          val = iround(m*(*it)+b);
          (*dit).set(val,val,val,0);
        }
      }
      
    } else {
      // copy to the destination only the desired submatrix.
      dest.allocate(to.y-from.y+1,to.x-from.x+1);
      vector<int32>::const_iterator it;
      vector<rgbaPixel>::iterator dit,eit;
      const ivector& lut = cache_.clut_[cacheIdx].first;
      if (lut.empty()) {
        return false;
      }
      int y,idx;
      const int ey = min(to.y,src.lastRow());
      const int lastIdx = user_.lastIdx();
      for (y=from.y;y<=ey;++y) {
        const vector<int32>& srcRow = src.getRow(y);
        vector<rgbaPixel>& destRow = dest.getRow(y-from.y);
        for (it=srcRow.begin()+from.x,dit=destRow.begin(),eit=destRow.end();
             dit!=eit;
             ++it,++dit) {
          idx = lut.at(*it);
          if (idx<0) {
            (*dit)=par.underflowColor;
          } else if (idx > lastIdx) {
            (*dit)=par.overflowColor;
          } else {
            (*dit)=user_.at(idx);
          }
        }
      }
    }
    
    return true;
  }

  bool viewer2DPainter::render(const matrix<int32>& src,
                               const ipoint from,
                               const ipoint to,
                               image& dest)  {

    if (src.empty()) {
      dest.clear();
      return true;
    }

    const parameters& par = getParameters();

    if (cache_.checkCacheMiss(src.data(),
                              src.size(),
                              par.mappingType)) {
      
      cache_.update(src.data(),IntensityCache,src,par.mappingType);
    }

    // get the window of interest.
    matrix<int32> tmp;
    tmp.copy(src,from,to);
    dest.allocate(tmp.size());

    // do the intensity channel mapping
    mapping(tmp,IntensityCache);

    const palette* thePalette = 0;
    // from tmp we have to create now the image, which depends on the palette
    // type.
    switch (par.paletteType) {
      case GrayPalette:
        thePalette = &gray_;
        break;
      case RedPalette:
        thePalette = &red_;
        break;
      case GreenPalette:
        thePalette = &green_;
        break;
      case BluePalette:
        thePalette = &blue_;
        break;
      case HuePalette:
        thePalette = &hue_;
        break;
      case UserDefPalette:
        thePalette = &user_;
        break;
      default:
        thePalette = &gray_;
        break;
    }
    
    matrix<int32>::const_iterator it,eit;
    matrix<rgbaPixel>::iterator dit;
    for (it=tmp.begin(),eit=tmp.end(),dit=dest.begin();it!=eit;++it,++dit) {
      if ((*it)<0) {
        (*dit)=par.underflowColor;
      } else if ((*it)>255) {
        (*dit)=par.overflowColor;
      } else {
        (*dit)=thePalette->at(*it);
      }
    }
    
    return true;
  }

  // src is not const since 
  bool viewer2DPainter::zoom(image& src,
                             const irectangle& viewport,
                             const float scale,
                             image& dest) const {

    if (scale == 1.0f) {
      // just transfer the data, without changing anything.
      src.detach(dest);
      return true;
    }

    if (src.empty()) {
      dest.clear();
      return true;
    }
    
    dest.allocate(viewport.br.y-viewport.ul.y+1,
                    viewport.br.x-viewport.ul.x+1);        
    
    // zoom can be to upsample or downsample
    if (scale>0.75) {
      // Up-sampling case   
      //   we cannot use the filled-upsampling functor, because depending on
      //   the "from" argument a pixel should be only partially rendered.

      const int lim = iround(scale);
      const int sx = viewport.ul.x%lim;
            int sy = viewport.ul.y%lim;

      // it can happen that the request of viewport is bigger that the
      // resulting image.  We check for that anomaly here, and resize the
      // image if it was too big

      ipoint c(src.columns()*lim-sx,
               src.rows()   *lim-sy);
      if ((dest.columns() > c.x) ||
          (dest.rows()    > c.y)   ) {
        const int mx = min(dest.columns(),c.x);
        const int my = min(dest.rows(),c.y);
        dest.allocate(my,mx);
      }

      vector<rgbaPixel>::const_iterator sit;
      vector<rgbaPixel>::iterator it,eit;
      
      int y=0;
      c.y = sy;      
      sy = 0;
      while (y<dest.rows()) {
        sit=src.getRow(sy).begin();
        vector<rgbaPixel>& dv = dest.getRow(y);
        // first line is always something special
        for (it=dv.begin(),eit=dv.end(),c.x=sx;it!=eit;++it) {
          (*it)=(*sit);
          c.x++;
          if (c.x >= lim) {
            c.x = 0;
            ++sit;
          }
        }
        sy++;
        
        // copy the previous line
        c.y++;
        y++;
        while ((c.y < lim) && (y < dest.rows())) {
          dest.getRow(y).fill(dest.getRow(y-1));
          c.y++;
          y++;
        }
        c.y = 0;
      }
    } else {
      // Down-sampling case
      const int step = iround(1.0f/scale);
      int y;

      // it can happen that the request of viewport is bigger that the
      // resulting image.  We check for that anomaly here, and resize the
      // image if it was too big

      ipoint c(iround(ceilf(src.columns()*scale)),
               iround(ceilf(src.rows()*scale)));
      if ((dest.columns() > c.x) ||
          (dest.rows() > c.y)) {
        dest.allocate(c);
      } 
      
      vector<rgbaPixel>::iterator it,eit;
      vector<rgbaPixel>::const_iterator sit;
      for (y=0;y<dest.rows();y++) {
        vector<rgbaPixel>& dv = dest.getRow(y);        
        sit = src.getRow(y*step).begin();
        for (it=dv.begin(),eit=dv.end();it!=eit;++it) {
          (*it)=(*sit);
          sit+=step;
        }
      }
    }

    return true;
  }
}

