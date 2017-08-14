/*
 * Copyright (C) 2002, 2003, 2004, 2005, 2006
 * Lehrstuhl fuer Technische Informatik, RWTH-Aachen, Germany
 *
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
 * \file   ltiColorModelEstimation.h
 *         Contains the class lti::colorModelEstimation, which computes 3D
 *         color histograms.
 * \author Pablo Alvarado
 * \date   12.03.2002
 *
 * revisions ..: $Id: ltiColorModelEstimation.cpp,v 1.3 2009-09-19 00:17:39 alvarado Exp $
 */

#include "ltiColorModelEstimation.h"
#include "ltiRound.h"
#include "ltiChannel.h"

namespace lti {
  // --------------------------------------------------
  // colorModelEstimation::parameters
  // --------------------------------------------------

  // default constructor
  colorModelEstimation::parameters::parameters()
    : functor::parameters() {

    histogramDimensions.set(32,32,32);
    maskIgnore.set(1,0);
  }

  // copy constructor
  colorModelEstimation::parameters::parameters(const parameters& other)
    : functor::parameters()  {
    copy(other);
  }

  // destructor
  colorModelEstimation::parameters::~parameters() {
  }

  // get type name
  const std::string& colorModelEstimation::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // copy member

  colorModelEstimation::parameters&
  colorModelEstimation::parameters::copy(const parameters& other) {
    functor::parameters::copy(other);

    histogramDimensions = other.histogramDimensions;
    maskIgnore = other.maskIgnore;

    return *this;
  }

  // alias for copy member
  colorModelEstimation::parameters&
  colorModelEstimation::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  // clone member
  colorModelEstimation::parameters*
  colorModelEstimation::parameters::clone() const {
    return new parameters(*this);
  }

  colorModelEstimation::parameters*
  colorModelEstimation::parameters::newInstance() const {
    return new parameters;
  }

  /*
   * Write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool colorModelEstimation::parameters::write(ioHandler& handler,
                                              const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {
      lti::write(handler,"histogramDimensions",histogramDimensions);
      lti::write(handler,"maskIgnore",maskIgnore);
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
   * @return true if write was successful
   */
  bool colorModelEstimation::parameters::read(ioHandler& handler,
                                        const bool complete) {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {
      lti::read(handler,"histogramDimensions",histogramDimensions);
      lti::read(handler,"maskIgnore",maskIgnore);
    }

    b = b && functor::parameters::read(handler,false);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

  // --------------------------------------------------
  // colorModelEstimation
  // --------------------------------------------------

  // default constructor
  colorModelEstimation::colorModelEstimation()
    : functor(){

    // create an instance of the parameters with the default values
    parameters defaultParameters;
    // set the default parameters
    setParameters(defaultParameters);
  }

  // default constructor
  colorModelEstimation::colorModelEstimation(const parameters& param)
    : functor(){

    // set the default parameters
    setParameters(param);
  }

  // copy constructor
  colorModelEstimation::colorModelEstimation(const colorModelEstimation& other)
    : functor()  {
    copy(other);
  }

  // destructor
  colorModelEstimation::~colorModelEstimation() {
  }

  // returns the name of this type
  const std::string& colorModelEstimation::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // copy member
  colorModelEstimation&
  colorModelEstimation::copy(const colorModelEstimation& other) {
    functor::copy(other);
    
    theModel_.copy(other.theModel_);

    index_.copy(other.index_);

    hdimRed_   = other.hdimRed_;
    hdimGreen_ = other.hdimGreen_;
    hdimBlue_  = other.hdimBlue_;

    lastIdxRed_   = hdimRed_-1;
    lastIdxGreen_ = hdimGreen_-1;
    lastIdxBlue_  = hdimBlue_-1;

    return (*this);
  }

  // alias for copy member
  colorModelEstimation&
  colorModelEstimation::operator=(const colorModelEstimation& other) {
    return (copy(other));
  }


  // clone member
  colorModelEstimation* colorModelEstimation::clone() const {
    return new colorModelEstimation(*this);
  }

  colorModelEstimation* colorModelEstimation::newInstance() const {
    return new colorModelEstimation;
  }

  /*
   * Set functor's parameters.
   * This member makes a copy of <em>theParam</em>: the functor
   * will keep its own copy of the parameters!
   *
   * This overloading will initialize the size of the internal color model
   * @return true if successful, false otherwise
   */
  bool colorModelEstimation::updateParameters() {
    return reset();
  }


  // return parameters
  const colorModelEstimation::parameters&
    colorModelEstimation::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }

  // -------------------------------------------------------------------
  // The apply-methods!
  // -------------------------------------------------------------------

  /*
   * reset the internal histogram to consider new elements.
   * The internal model will be empty after this.  The first "consider()"
   * will take the desired dimensions from the parameters and will initialize
   * the internal histogram with the proper size.
   */
  bool colorModelEstimation::reset() {
    const parameters& param = getParameters();

    hdimRed_   = param.histogramDimensions.getRed();
    hdimGreen_ = param.histogramDimensions.getGreen();
    hdimBlue_  = param.histogramDimensions.getBlue();

    ignoreInterval_ = param.maskIgnore;

    lastIdxRed_   = hdimRed_-1;
    lastIdxGreen_ = hdimGreen_-1;
    lastIdxBlue_  = hdimBlue_-1;

    ivector dims(3);
    dims.at(0)=hdimRed_;
    dims.at(1)=hdimGreen_;
    dims.at(2)=hdimBlue_;

    theModel_.clear();
    theModel_.resize(3,dims);

    index_.assign(3,0);

    n_ = 0;
    sumOuter_.assign(3,3,0.0);
    sum_.set(0.0,0.0,0.0);

    return true;
  }

  /*
   * consider all pixels in the image.  The first call to this member (or
   * after a reset()) will initialize the size of the internal model.
   * @param img image with colors to be considered
   * @return true if successful or false otherwise.
   */
  bool colorModelEstimation::consider(const rgbaPixel& pix,
                                     const int times) {

    index_.at(0)=static_cast<int>(pix.getRed()  *hdimRed_  / 256);
    index_.at(1)=static_cast<int>(pix.getGreen()*hdimGreen_/ 256);
    index_.at(2)=static_cast<int>(pix.getBlue() *hdimBlue_ / 256);
    theModel_.put(index_,times);

    ++n_;
    rgbPixel<double> p(pix);
    sum_.add(p);

    // outer product of color is symmetric => consider only the upper
    // triangular matrix, and when asking for the model compute the
    // complete one...
    omit_ = sumOuter_.begin();

    (*omit_) += p.red*p.red;
    ++omit_;
    (*omit_) += p.red*p.green;
    ++omit_;
    (*omit_) += p.red*p.blue;
    omit_+=2;
    (*omit_) += p.green*p.green;
    ++omit_;
    (*omit_) += p.green*p.blue;
    omit_+=3;
    (*omit_) += p.blue*p.blue;

    return true;
  }

  /*
   * consider all pixels in the image.  The first call to this member (or
   * after a reset()) will initialize the size of the internal model.
   * @param img image with colors to be considered
   * @return true if successful or false otherwise.
   */
  bool colorModelEstimation::smoothConsider(const rgbaPixel& pix,
                                           const int times) {

    int fr,tr,fg,tg,fb,tb;
    double qr,qg,qb;
    int t,q;

    t=static_cast<int>(pix.getRed()*hdimRed_);
    fr = t/256;    
    q = (t - fr*256 - 128);
    if (q < 0) {
      tr = fr;
      fr = max(0,tr-1);
      qr = (q+256)/256.0;
    } else {
      tr = fr+1;
      if (tr==hdimRed_) {
        tr--;
      }
      qr = q/256.0;
    }

    t=static_cast<int>(pix.getGreen()*hdimGreen_);
    fg = t/256;
    q = (t-fg*256 - 128);
    if (q < 0) {
      tg = fg;
      fg = max(0,tg-1);
      qg = (q+256)/256.0;
    } else {
      tg = fg+1;
      if (tg==hdimGreen_) {
        tg--;
      }
      qg = q/256.0;
    }
    
    t=static_cast<int>(pix.getBlue()*hdimBlue_);
    fb = t/256;
    q = (t-fb*256 - 128);
    if (q < 0) {
      tb=fb;
      fb=max(0,tb-1);
      qb = (q+256)/256.0;
    } else {
      tb=fb+1;
      if (tb==hdimBlue_) {
        tb--;
      }
      qb = q/256.0;
    }

    // now interpolate the eight values (using a grey code to change only one
    // index element at a time)
    index_.at(0)=fr;
    index_.at(1)=fg;
    index_.at(2)=fb;
    theModel_.put(index_,times*(1.0-qr)*(1.0-qg)*(1.0-qb));
    
    index_.at(0)=tr;
    theModel_.put(index_,times*qr*(1.0-qg)*(1.0-qb));
    
    index_.at(1)=tg;
    theModel_.put(index_,times*qr*qg*(1.0-qb));

    index_.at(0)=fr;
    theModel_.put(index_,times*(1.0-qr)*qg*(1.0-qb));

    index_.at(2)=tb;
    theModel_.put(index_,times*(1.0-qr)*qg*qb);

    index_.at(0)=tr;
    theModel_.put(index_,times*qr*qg*qb);
    
    index_.at(1)=fg;
    theModel_.put(index_,times*qr*(1.0-qg)*qb);

    index_.at(0)=fr;
    theModel_.put(index_,times*(1.0-qr)*(1.0-qg)*qb);
    
    // the gaussian model
    ++n_;
    rgbPixel<double> p(pix);
    sum_.add(p);

    // outer product of color is symmetric => consider only the upper
    // triangular matrix, and when asking for the model compute the
    // complete one...
    omit_ = sumOuter_.begin();

    (*omit_) += p.red*p.red;
    ++omit_;
    (*omit_) += p.red*p.green;
    ++omit_;
    (*omit_) += p.red*p.blue;
    omit_+=2;
    (*omit_) += p.green*p.green;
    ++omit_;
    (*omit_) += p.green*p.blue;
    omit_+=3;
    (*omit_) += p.blue*p.blue;

    return true;
  }


  /*
   * consider all pixels in the image.  The first call to this member (or
   * after a reset()) will initialize the size of the internal model.
   * @param img image with colors to be considered
   * @return true if successful or false otherwise.
   */
  bool colorModelEstimation::consider(const float& c1,
                                      const float& c2,
                                      const float& c3,
                                      const int times) {
    index_.at(0)=iround(c1*lastIdxRed_);
    index_.at(1)=iround(c2*lastIdxGreen_);
    index_.at(2)=iround(c3*lastIdxBlue_);
    theModel_.put(index_,times);

    ++n_;
    rgbPixel<double> p(c1,c2,c3);
    sum_.add(p);

    // outer product of color is symmetric => consider only the upper
    // triangular matrix, and when asking for the model compute the
    // complete one...
    omit_ = sumOuter_.begin();

    (*omit_) += p.red*p.red;
    ++omit_;
    (*omit_) += p.red*p.green;
    ++omit_;
    (*omit_) += p.red*p.blue;
    omit_+=2;
    (*omit_) += p.green*p.green;
    ++omit_;
    (*omit_) += p.green*p.blue;
    omit_+=3;
    (*omit_) += p.blue*p.blue;

    return true;
  }


  /*
   * consider a pixel for the model.
   * @param pal the color palette to be considered
   * @return true if successful or false otherwise.
   */
  bool colorModelEstimation::consider(const palette& pal) {
    palette::const_iterator it,eit;
    for (it=pal.begin(),eit=pal.end();it!=eit;++it) {
      consider(*it);
    }
    return true;
  }


  /*
   * consider all pixels in the image.  The first call to this member (or
   * after a reset()) will initialize the size of the internal model.
   * @param img image with colors to be considered
   * @return true if successful or false otherwise.
   */
  bool colorModelEstimation::consider(const image& img) {
    vector<image::value_type>::const_iterator it,eit;
    int y;

    for (y=0;y<img.rows();++y) {
      const vector<image::value_type>& vct=img.getRow(y);
      for (it=vct.begin(),eit=vct.end();it!=eit;++it) {
        consider(*it);
      }
    }

    return true;
  }

  /*
   * consider all pixels in the image.  The first call to this member (or
   * after a reset()) will initialize the size of the internal model.
   * @param img image with colors to be considered
   * @return true if successful or false otherwise.
   */
  bool colorModelEstimation::consider(const channel& c1,
                                      const channel& c2,
                                      const channel& c3) {

    if ((c1.size() != c2.size()) ||
        (c1.size() != c3.size())) {
      setStatusString("Channels have different sizes");
      return false;
    }

    int y,x;

    for (y=0;y<c1.rows();++y) {
      for (x=0;x<c1.columns();++x) {
        consider(c1.at(y,x),c2.at(y,x),c3.at(y,x));
      }
    }

    return true;
  }


  /*
   * consider all pixels in the image.  The first call to this member (or
   * after a reset()) will initialize the size of the internal model.
   * @param img image with colors to be considered
   * @return true if successful or false otherwise.
   */
  bool colorModelEstimation::consider(const image& img,
                                      const channel8& mask) {

    if (img.size() != mask.size()) {
      setStatusString("Image and mask must have the same size");
      return false;
    }

    vector<image::value_type>::const_iterator it,eit;
    vector<channel8::value_type>::const_iterator mit;
    int y;
    const ubyte to = static_cast<ubyte>(ignoreInterval_.to);

    for (y=0;y<img.rows();++y) {
      const vector<image::value_type>& vct=img.getRow(y);
      for (it=vct.begin(),eit=vct.end(),mit=mask.getRow(y).begin();
           it!=eit;
           ++it,++mit) {
        if ((*mit) > to) {
          consider(*it);
        }
      }
    }

    return true;
  }

  /*
   * consider all pixels in the image.  The first call to this member (or
   * after a reset()) will initialize the size of the internal model.
   * @param img image with colors to be considered
   * @return true if successful or false otherwise.
   */
  bool colorModelEstimation::consider(const image& img,
                                      const imatrix& mask) {

    if (img.size() != mask.size()) {
      setStatusString("Image and mask must have the same size");
      return false;
    }

    vector<image::value_type>::const_iterator it,eit;
    ivector::const_iterator mit;
    int y;
    const int to = static_cast<int>(ignoreInterval_.to);

    for (y=0;y<img.rows();++y) {
      const vector<image::value_type>& vct=img.getRow(y);
      for (it=vct.begin(),eit=vct.end(),mit=mask.getRow(y).begin();
           it!=eit;
           ++it,++mit) {
        if ((*mit) >= to) {
          consider(*it);
        }
      }
    }

    return true;
  }

  bool colorModelEstimation::consider(const channel& c1,
                                      const channel& c2,
                                      const channel& c3,
                                      const channel8& mask) {

    if ((c1.size() != c2.size()) ||
        (c1.size() != c3.size()) ||
        (c1.size() != mask.size())) {
      setStatusString("Channels have different sizes");
      return false;
    }
    const ubyte to = static_cast<ubyte>(ignoreInterval_.to);
    int y,x;

    for (y=0;y<c1.rows();++y) {
      for (x=0;x<c1.columns();++x) {
        if (mask.at(y,x) > to) {
          consider(c1.at(y,x),c2.at(y,x),c3.at(y,x));
        }
      }
    }

    return true;
  }

  bool colorModelEstimation::consider(const channel& c1,
                                     const channel& c2,
                                     const channel& c3,
                                     const imatrix& mask) {

    if ((c1.size() != c2.size()) ||
        (c1.size() != c3.size()) ||
        (c1.size() != mask.size())) {
      setStatusString("Channels have different sizes");
      return false;
    }

    int y,x;
    const int to = static_cast<int>(ignoreInterval_.to);

    for (y=0;y<c1.rows();++y) {
      for (x=0;x<c1.columns();++x) {
        if (mask.at(y,x) > to) {
          consider(c1.at(y,x),c2.at(y,x),c3.at(y,x));
        }
      }
    }

    return true;
  }


  bool colorModelEstimation::consider(const image& img,
                                     const channel8& mask,
                                     const ubyte label) {

    if (img.size() != mask.size()) {
      setStatusString("Image and mask must have the same size");
      return false;
    }

    vector<image::value_type>::const_iterator it,eit;
    vector<channel8::value_type>::const_iterator mit;
    int y;

    for (y=0;y<img.rows();++y) {
      const vector<image::value_type>& vct=img.getRow(y);
      for (it=vct.begin(),eit=vct.end(),mit=mask.getRow(y).begin();
           it!=eit;
           ++it,++mit) {
        if ((*mit) == label) {
          consider(*it);
        }
      }
    }

    return true;
  }

  bool colorModelEstimation::consider(const image& img,
                                     const imatrix& mask,
                                     const int label) {

    if (img.size() != mask.size()) {
      setStatusString("Image and mask must have the same size");
      return false;
    }

    vector<image::value_type>::const_iterator it,eit;
    vector<int>::const_iterator mit;
    int y;

    for (y=0;y<img.rows();++y) {
      const vector<rgbaPixel>& vct=img.getRow(y);
      for (it=vct.begin(),eit=vct.end(),mit=mask.getRow(y).begin();
           it!=eit;
           ++it,++mit) {
        if ((*mit) == label) {
          consider(*it);
        }
      }
    }

    return true;
  }


  /*
   * consider all the images in the given list
   *
   * @param imgs list of images to be considered
   * @return true if successful or false otherwise.
   */
  bool colorModelEstimation::consider(const std::list<image>& imgs) {
    std::list<image>::const_iterator it;
    for (it=imgs.begin();it!=imgs.end();++it) {
      consider(*it);
    }
    return true;
  }

  /*
   * consider all the images in the given list.  The mask list must have
   * the same size, and the corresponding images must also have the same
   * size.
   */
  bool colorModelEstimation::consider(const std::list<image>& imgs,
                                      const std::list<channel8>& masks) {
    std::list<image>::const_iterator it;
    std::list<channel8>::const_iterator mit;
    if (imgs.size() != masks.size()) {
      setStatusString("lists of masks and images have different sizes.");
      return false;
    }

    for (it=imgs.begin(),mit=masks.begin();it!=imgs.end();++it,++mit) {
      consider(*it,*mit);
    }

    return true;
  }

  /*
   * consider all pixels in the image.  The first call to this member (or
   * after a reset()) will initialize the size of the internal model.
   * @param img image with colors to be considered
   * @return true if successful or false otherwise.
   */
  bool colorModelEstimation::consider(const image& img,
                                      const channel8& mask,
                                      colorModelEstimation& rest) {

    if (img.size() != mask.size()) {
      setStatusString("Image and mask must have the same size");
      return false;
    }

    const ubyte from = static_cast<ubyte>(ignoreInterval_.from);
    const ubyte to   = static_cast<ubyte>(ignoreInterval_.to);

    vector<image::value_type>::const_iterator it,eit;
    vector<channel8::value_type>::const_iterator mit;
    int y;

    for (y=0;y<img.rows();++y) {
      const vector<image::value_type>& vct=img.getRow(y);
      for (it=vct.begin(),eit=vct.end(),mit=mask.getRow(y).begin();
           it!=eit;
           ++it,++mit) {
        if ((*mit) > to) {
          consider(*it);
        } else if ((*mit) < from) {
          rest.consider(*it);
        }
      }
    }

    return true;
  }

  /*
   * consider all pixels in the image.  The first call to this member (or
   * after a reset()) will initialize the size of the internal model.
   * @param img image with colors to be considered
   * @return true if successful or false otherwise.
   */
  bool colorModelEstimation::consider(const image& img,
                                      const imatrix& mask,
                                      colorModelEstimation& rest) {

    if (img.size() != mask.size()) {
      setStatusString("Image and mask must have the same size");
      return false;
    }

    const int from = static_cast<int>(ignoreInterval_.from);
    const int to   = static_cast<int>(ignoreInterval_.to);

    vector<image::value_type>::const_iterator it,eit;
    ivector::const_iterator mit;
    int y;

    for (y=0;y<img.rows();++y) {
      const vector<image::value_type>& vct=img.getRow(y);
      for (it=vct.begin(),eit=vct.end(),mit=mask.getRow(y).begin();
           it!=eit;
           ++it,++mit) {
        if ((*mit) > to) {
          consider(*it);
        } else if ((*mit) < from){
          rest.consider(*it);
        }
      }
    }

    return true;
  }

  bool colorModelEstimation::consider(const channel& c1,
                                      const channel& c2,
                                      const channel& c3,
                                      const channel8& mask,
                                      colorModelEstimation& rest) {

    if ((c1.size() != c2.size()) ||
        (c1.size() != c3.size()) ||
        (c1.size() != mask.size())) {
      setStatusString("Channels have different sizes");
      return false;
    }

    const ubyte from = static_cast<ubyte>(ignoreInterval_.from);
    const ubyte to   = static_cast<ubyte>(ignoreInterval_.to);

    int y,x;

    for (y=0;y<c1.rows();++y) {
      for (x=0;x<c1.columns();++x) {
        const ubyte m = mask.at(y,x);
        if (m > to) {
          consider(c1.at(y,x),c2.at(y,x),c3.at(y,x));
        } else if (m < from) {
          rest.consider(c1.at(y,x),c2.at(y,x),c3.at(y,x));
        }
      }
    }

    return true;
  }

  bool colorModelEstimation::consider(const channel& c1,
                                     const channel& c2,
                                     const channel& c3,
                                     const imatrix& mask,
                                      colorModelEstimation& rest) {

    if ((c1.size() != c2.size()) ||
        (c1.size() != c3.size()) ||
        (c1.size() != mask.size())) {
      setStatusString("Channels have different sizes");
      return false;
    }


    const int from = static_cast<int>(ignoreInterval_.from);
    const int to   = static_cast<int>(ignoreInterval_.to);

    int y,x;

    for (y=0;y<c1.rows();++y) {
      for (x=0;x<c1.columns();++x) {
        const int m = mask.at(y,x);
        if (m > to) {
          consider(c1.at(y,x),c2.at(y,x),c3.at(y,x));
        } else if (m < from){
          rest.consider(c1.at(y,x),c2.at(y,x),c3.at(y,x));
        }
      }
    }

    return true;
  }


  bool colorModelEstimation::consider(const image& img,
                                      const channel8& mask,
                                      const ubyte label,
                                      colorModelEstimation& rest) {

    if (img.size() != mask.size()) {
      setStatusString("Image and mask must have the same size");
      return false;
    }

    vector<image::value_type>::const_iterator it,eit;
    vector<channel8::value_type>::const_iterator mit;
    int y;

    for (y=0;y<img.rows();++y) {
      const vector<image::value_type>& vct=img.getRow(y);
      for (it=vct.begin(),eit=vct.end(),mit=mask.getRow(y).begin();
           it!=eit;
           ++it,++mit) {
        if ((*mit) == label) {
          consider(*it);
        } else {
          rest.consider(*it);
        }
      }
    }

    return true;
  }

  bool colorModelEstimation::consider(const image& img,
                                      const imatrix& mask,
                                      const int label,
                                      colorModelEstimation& rest) {

    if (img.size() != mask.size()) {
      setStatusString("Image and mask must have the same size");
      return false;
    }

    vector<image::value_type>::const_iterator it,eit;
    vector<int>::const_iterator mit;
    int y;

    for (y=0;y<img.rows();++y) {
      const vector<rgbaPixel>& vct=img.getRow(y);
      for (it=vct.begin(),eit=vct.end(),mit=mask.getRow(y).begin();
           it!=eit;
           ++it,++mit) {
        if ((*mit) == label) {
          consider(*it);
        } else {
          rest.consider(*it);
        }
      }
    }

    return true;
  }


    bool colorModelEstimation::consider(const std::list<image>& imgs,
                                        const std::list<channel8>& masks,
                                      colorModelEstimation& rest) {
    std::list<image>::const_iterator it;
    std::list<channel8>::const_iterator mit;
    if (imgs.size() != masks.size()) {
      setStatusString("lists of masks and images have different sizes.");
      return false;
    }

    for (it=imgs.begin(),mit=masks.begin();it!=imgs.end();++it,++mit) {
      consider(*it,*mit,rest);
    }

    return true;
  }






  // END OF NEW ONES



  /*
   * get a constant reference to the internal model
   */
  const dhistogram& colorModelEstimation::getColorModel() const {
    return theModel_;
  }

  /*
   * get a copy of the internal model in the given %parameter.
   * @param model the color model (as 3D histogram) computed until now
   * @return true if apply successful or false otherwise.
   */
  bool colorModelEstimation::apply(dhistogram& model) const {
    model.copy(theModel_);
    return true;
  }

  /*
   * get the mean color and covariance matrix of the internal model
   * @param mean the mean color
   * @param covar covariance matrix of the color (3x3 matrix)
   * @return true if apply successful, false otherwise.
   */
  bool colorModelEstimation::apply(rgbPixel<double>& mean,
                                  dmatrix& covar) const {

    mean.copy(sum_);
    if (n_<2) {
      covar.assign(3,3,0.0);
      return true;
    }

    mean.divide(n_);

    dmatrix a;
    a.allocate(3,3);
    outer(mean,mean,a);

    covar.copy(sumOuter_);
    // complete the outer product sum (till now only the upper triangular mat.)
    covar.at(1,0)=covar.at(0,1);
    covar.at(2,0)=covar.at(0,2);
    covar.at(2,1)=covar.at(1,2);

    covar.addScaled(-n_,a);
    covar.divide(n_-1);

    return true;
  }

  /*
   * outer product of two colors
   */
  void colorModelEstimation::outer(const rgbPixel<double>& a,
                                  const rgbPixel<double>& b,
                                  dmatrix& mat) const {
    dmatrix::iterator it = mat.begin();
    (*it)=a.red*b.red;
    ++it;
    (*it)=a.red*b.green;
    ++it;
    (*it)=a.red*b.blue;
    ++it;

    (*it)=a.green*b.red;
    ++it;
    (*it)=a.green*b.green;
    ++it;
    (*it)=a.green*b.blue;
    ++it;

    (*it)=a.blue*b.red;
    ++it;
    (*it)=a.blue*b.green;
    ++it;
    (*it)=a.blue*b.blue;
  }



}
