/*
 * Copyright (C) 2009
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
 * \file   ltiThresholding.cpp
 *         Contains the class thresholding,
 *         which has still to be documented by Pablo Alvarado.
 * 
 * \author Pablo Alvarado
 * \date   26.10.2009
 *
 * revisions ..: $Id: ltiThresholding.cpp,v 1.2 2009/11/01 23:03:41 alvarado Exp $
 */

#include "ltiThresholding.h"
#include "ltiChannel8.h"

namespace lti {
  // --------------------------------------------------
  // thresholding::parameters
  // --------------------------------------------------

  // default constructor
  thresholding::parameters::parameters()
    : segmentation::parameters() {
    
    backgroundValue = 0.0f;
    foregroundValue = 1.0f;
    foreground = finterval(0.5f,1.0f);
    method = Direct;
    keepBackground = false;
    keepForeground = false;
    histogramBins  = 1024;
    deltaT         = 1;
  }

  // copy constructor
  thresholding::parameters::parameters(const parameters& other)
    : segmentation::parameters() {
    copy(other);
  }

  // destructor
  thresholding::parameters::~parameters() {
  }

  // copy member

  thresholding::parameters&
  thresholding::parameters::copy(const parameters& other) {
    segmentation::parameters::copy(other);

    
    backgroundValue = other.backgroundValue;
    foregroundValue = other.foregroundValue;
    foreground = other.foreground;
    method = other.method;
    keepBackground = other.keepBackground;
    keepForeground = other.keepForeground;
    histogramBins = other.histogramBins;
    deltaT = other.deltaT;

    return *this;
  }

  // alias for copy method
  thresholding::parameters&
  thresholding::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  // class name
  const std::string& thresholding::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone method
  thresholding::parameters*
  thresholding::parameters::clone() const {
    return new parameters(*this);
  }

  // new instance
  thresholding::parameters*
  thresholding::parameters::newInstance() const {
    return new parameters();
  }

  /*
   * Write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool thresholding::parameters::write(ioHandler& handler,
                                         const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {
      
      lti::write(handler,"backgroundValue",backgroundValue);
      lti::write(handler,"foregroundValue",foregroundValue);
      lti::write(handler,"foreground",foreground);
      lti::write(handler,"method",method);
      lti::write(handler,"keepBackground",keepBackground);
      lti::write(handler,"keepForeground",keepForeground);
      lti::write(handler,"histogramBins",histogramBins);
      lti::write(handler,"deltaT",deltaT);
    }

    b = b && segmentation::parameters::write(handler,false);

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
  bool thresholding::parameters::read(ioHandler& handler,
                                        const bool complete)  {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {
      
      lti::read(handler,"backgroundValue",backgroundValue);
      lti::read(handler,"foregroundValue",foregroundValue);
      lti::read(handler,"foreground",foreground);
      lti::read(handler,"method",method);
      lti::read(handler,"keepBackground",keepBackground);
      lti::read(handler,"keepForeground",keepForeground);
      lti::read(handler,"histogramBins",histogramBins);
      lti::read(handler,"deltaT",deltaT);
    }

    b = b && segmentation::parameters::read(handler,false);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

  // --------------------------------------------------
  // thresholding
  // --------------------------------------------------

  // default constructor
  thresholding::thresholding()
    : segmentation() {

    // create an instance of the parameters with the default values
    parameters defaultParameters;
    // set the default parameters
    setParameters(defaultParameters);
  }

  // default constructor
  thresholding::thresholding(const parameters& par)
    : segmentation() {

    // set the given parameters
    setParameters(par);
  }


  // copy constructor
  thresholding::thresholding(const thresholding& other)
    : segmentation() {
    copy(other);
  }

  // destructor
  thresholding::~thresholding() {
  }

  // copy member
  thresholding& 
  thresholding::copy(const thresholding& other) {
    segmentation::copy(other);

    return (*this);
  }

  // alias for copy member
  thresholding&
  thresholding::operator=(const thresholding& other) {
    return (copy(other));
  }

  // class name
  const std::string& thresholding::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone member
  thresholding* thresholding::clone() const {
    return new thresholding(*this);
  }

  // create a new instance
  thresholding* thresholding::newInstance() const {
    return new thresholding();
  }

  // return parameters
  const thresholding::parameters& thresholding::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }

  bool thresholding::updateParameters() {
    if (functor::updateParameters()) {
      const parameters& par = getParameters();

      foreground_.copy(par.foreground);

      method_   = par.method;
      bins_     = par.histogramBins;
      deltaT_   = par.deltaT;
      
      return true;
    }
    return false;
  }

  // -------------------------------------------------------------------
  // The apply() member functions
  // -------------------------------------------------------------------

  /**
   * Threshold the \a src channel and leave the result on \a dest.
   *
   * @param src matrix<T> with the source data.
   * @param dest matrix<T> where the result will be left.
   * @return true if apply successful or false otherwise.
   */
  bool thresholding::apply(const matrix<ubyte>& src, 
                                 matrix<ubyte>& dest) const {

    switch (method_) {
    case Direct: {
      iinterval fg(static_cast<int>(foreground_.from*255.0f+0.5f),
                   static_cast<int>(foreground_.to  *255.0f+0.5f));
      
      return direct(src,dest,fg);
    } break;
    case OtsuInterval:
    case Otsu: {
      ivector hist;
      iinterval fg;
      computeHistogram(src,hist);
      otsu(hist,fg.from);
      fg.to=255;
      return direct(src,dest,fg);
    } break;
    case SimpleInterval:
    case Simple: {
      ivector hist;
      iinterval fg;
      computeHistogram(src,hist);
      simple(hist,fg.from);
      fg.to=255;
      return direct(src,dest,fg);
    } break;
    case Relative: {
      iinterval fg;
      relative(src,fg);
      return direct(src,dest,fg);
    } break;
    default:
      setStatusString("Unknown thresholding method");
      return false;
      break;
    }

    return false;
  }
 

  /**
   * Threshold the \a src channel and leave the result on \a dest.
   *
   * @param src matrix<T> with the source data.
   * @param dest matrix<T> where the result will be left.
   * @return true if apply successful or false otherwise.
   */
  bool thresholding::apply(const matrix<float>& src, 
                                 matrix<ubyte>& dest) const {

    switch(method_) {
    case Direct:
      return direct(src,dest,foreground_);
      break;
    case OtsuInterval:
    case Otsu: {
      ivector hist;
      finterval fg;
      float minVal,maxVal;
      int kstar;
      computeHistogram(src,hist,minVal,maxVal);
      otsu(hist,kstar);
      fg.from = minVal + (maxVal-minVal)*kstar/hist.lastIdx();
      fg.to=maxVal;
      return direct(src,dest,fg);
    } break;
    case SimpleInterval:
    case Simple: {
      ivector hist;
      finterval fg;
      float minVal,maxVal;
      int kstar;
      computeHistogram(src,hist,minVal,maxVal);
      simple(hist,kstar);
      fg.from = minVal + (maxVal-minVal)*kstar/hist.lastIdx();
      fg.to=maxVal;
      return direct(src,dest,fg);
    } break;
    case Relative: {
      finterval fg;
      relative(src,fg);
      return direct(src,dest,fg);
    } break;
    default:
      setStatusString("Unknown thresholding method");
      return false;
    }
    return false;
  }
  
  /**
   * Threshold the \a src channel and leave the result on \a dest.
   *
   * @param src matrix<T> with the source data.
   * @param dest matrix<T> where the result will be left.
   * @return true if apply successful or false otherwise.
   */
  bool thresholding::apply(const matrix<float>& src,
                           matrix<float>& dest) const {

    switch(method_) {
    case Direct:
      return direct(src,dest,foreground_);
      break;
    case OtsuInterval:
    case Otsu: {
      ivector hist;
      finterval fg;
      float minVal,maxVal;
      int kstar;
      computeHistogram(src,hist,minVal,maxVal);
      otsu(hist,kstar);
      fg.from = minVal + (maxVal-minVal)*kstar/hist.lastIdx();
      fg.to=maxVal;
      return direct(src,dest,fg);
    } break;
    case SimpleInterval:
    case Simple: {
      ivector hist;
      finterval fg;
      float minVal,maxVal;
      int kstar;
      computeHistogram(src,hist,minVal,maxVal);
      simple(hist,kstar);
      fg.from = minVal + (maxVal-minVal)*kstar/hist.lastIdx();
      fg.to=maxVal;
      return direct(src,dest,fg);
    } break;
    case Relative: {
      finterval fg;
      relative(src,fg);
      return direct(src,dest,fg);
    } break;
    default:
      setStatusString("Unknown thresholding method");
      return false;
    }
    return false;
  }

  /**
   * Apply the thresholding segmentation to the intensity channel
   *
   * @param src image with the source data.
   * @param dest matrix<T> where the result will be left.
   * @return true if apply successful or false otherwise.
   */
  bool thresholding::apply(const image& src, imatrix& dest) const {
    channel8 inten,tmp;
    inten.castFrom(src);
    
    if (apply(inten,tmp)) {
      dest.castFrom(tmp);
      return true;
    }
    
    return false;
  }


  
  /*
   * Threshold the \a src channel and leave the result on \a dest.
   *
   * @param src matrix<T> with the source data.
   * @param dest matrix<T> where the result will be left.
   * @return true if apply successful or false otherwise.
   */
  bool thresholding::direct(const matrix<ubyte>& src, 
                            matrix<ubyte>& dest,
                            const iinterval& foreg) const {

    const parameters& par = getParameters();
    const bool keepBg = par.keepBackground;
    const bool keepFg = par.keepForeground;

    const ubyte fg = static_cast<ubyte>(par.foregroundValue*255.0f + 0.5f);
    const ubyte bg = static_cast<ubyte>(par.backgroundValue*255.0f + 0.5f);

    // interval already casted from outside
    const ubyte low  = static_cast<ubyte>(foreg.from);
    const ubyte high = static_cast<ubyte>(foreg.to);

    const bool pure = ! (keepBg || keepFg);

    // create a LUT
    ubyte lut[256];

    if (pure) {
      int i;
      // loop unroll
      for (i=0;i<low;++i) {
        lut[i]=bg;
      }
      for (;i<=high;++i) {
        lut[i] = fg;
      }
      for (;i<256;++i) {       
        lut[i] = bg;
      }
      
    } else {

      if (keepBg && keepFg) {
        dest.copy(src);
        return true; // nothing to do, fg and bg have to be copied.
      }

      if (keepBg) {
        int i;
        // loop unroll
        for (i=0;i<low;++i) {
          lut[i]=i;
        }
        for (;i<=high;++i) {
          lut[i] = fg;
        }
        for (;i<256;++i) {       
          lut[i] = i;
        }
      } else { // keep foreground
        int i;
        // loop unroll
        for (i=0;i<low;++i) {
          lut[i]=bg;
        }
        for (;i<=high;++i) {
          lut[i] = i;
        }
        for (;i<256;++i) {       
          lut[i] = bg;
        }
      }
    }

    // use a const lut in case the compiler can make use of it in the optimiz.
    const ubyte *const clut = lut;

    // resize the destination, but do not initialize
    dest.allocate(src.size());

    // now apply the lut to each pixel
    matrix<ubyte>::const_iterator       sit=src.begin();
    const matrix<ubyte>::const_iterator eit=src.end();
    matrix<ubyte>::iterator             dit=dest.begin();

    while(sit != eit) {
      *dit++ = clut[*sit++]; // wow! that's fast!
    }

    return true;
  }
  
  /*
   * Threshold the \a src channel and leave the result on \a dest.
   *
   * @param src matrix<T> with the source data.
   * @param dest matrix<T> where the result will be left.
   * @return true if apply successful or false otherwise.
   */
  bool thresholding::direct(const matrix<float>& src,
                            matrix<ubyte>& dest,
                            const finterval& foreg) const {

    const parameters& par = getParameters();
    const bool keepBg = par.keepBackground;
    const bool keepFg = par.keepForeground;

    const ubyte fg = static_cast<ubyte>(par.foregroundValue*255.0f + 0.5f);
    const ubyte bg = static_cast<ubyte>(par.backgroundValue*255.0f + 0.5f);

    const float low  = foreg.from;
    const float high = foreg.to;

    matrix<float>::const_iterator sit=src.begin();
    const matrix<float>::const_iterator eit=src.end();

    const bool pure = ! (keepBg || keepFg);

    if (pure) {
      dest.allocate(src.size());
      matrix<ubyte>::iterator dit = dest.begin();

      while(sit != eit) {
        (*dit) = ( ((*sit) < low) || ((*sit) > high) ) ? bg : fg;
        ++dit;
        ++sit;
      }

    } else {
      // just resize without initialization
      dest.castFrom(src);
      matrix<ubyte>::iterator dit = dest.begin();

      if (keepBg && keepFg) {
        return true;
      }

      if (keepBg) {
        while(sit != eit) {
          
          (*dit) = ( ((*sit) < low) || ((*sit) > high) ) ?
                   static_cast<ubyte>((*sit)*255.0f) : fg;
          
          ++dit;
          ++sit;
        }
      } else {
        while(sit != eit) {
          
          (*dit) = ( ((*sit) < low) || ((*sit) > high) ) ?
                   bg : static_cast<ubyte>((*sit)*255.0f);
          
          ++dit;
          ++sit;
        }
      }
    }

    return true;
  }
  
  /*
   * Threshold the \a src channel and leave the result on \a dest.
   *
   * @param src matrix<T> with the source data.
   * @param dest matrix<T> where the result will be left.
   * @return true if apply successful or false otherwise.
   */
  bool thresholding::direct(const matrix<float>& src, 
                            matrix<float>& dest,
                            const finterval& foreg) const {

    const parameters& par = getParameters();
    const bool keepBg = par.keepBackground;
    const bool keepFg = par.keepForeground;

    const float fg = static_cast<float>(par.foregroundValue);
    const float bg = static_cast<float>(par.backgroundValue);

    const float low  = foreg.from;
    const float high = foreg.to;

    matrix<float>::const_iterator sit=src.begin();
    const matrix<float>::const_iterator eit=src.end();

    const bool pure = ! (keepBg || keepFg);

    if (pure) {
      dest.allocate(src.size());
      matrix<float>::iterator dit = dest.begin();

      while(sit != eit) {
        (*dit) = ( ((*sit) < low) || ((*sit) > high) ) ? bg : fg;
        ++dit;
        ++sit;
      }

    } else {
      // just resize without initialization
      dest.copy(src);
      matrix<float>::iterator dit = dest.begin();

      if (keepBg && keepFg) {
        return true;
      }

      if (keepBg) {
        while(sit != eit) {
          
          (*dit) = ( ((*sit) < low) || ((*sit) > high) ) ? (*sit) : fg;
          
          ++dit;
          ++sit;
        }
      } else {
        while(sit != eit) {
          
          (*dit) = ( ((*sit) < low) || ((*sit) > high) ) ? bg : (*sit);
          
          ++dit;
          ++sit;
        }
      }
    }

    return true;
  }
  

  /**
   * Compute the histogram of the given matrix<ubyte>
   */
  bool thresholding::computeHistogram(const matrix<ubyte>& in,
                                      ivector& hist) const {
    hist.assign(256,0);
    matrix<ubyte>::const_iterator it;
    const matrix<ubyte>::const_iterator cit = in.end();

    for (it=in.begin();it!=cit;++it) {
      hist.at(*it)++;
    }

    return true;
  }
  
  /**
   * Compute the histogram of the given matrix<float>
   *
   */
  bool thresholding::computeHistogram(const matrix<float>& in,
                                      ivector& hist,
                                      float& minVal,
                                      float& maxVal) const {
    
    
    in.findExtremes(minVal,maxVal);
    hist.assign(bins_,0);

    if (minVal >= maxVal) {
      // it seems that the image has just one single color
      // Force some arbitrary interval, such that the only avaliable value
      // is centered.
      minVal-=0.1f;
      maxVal+=0.1f;
    }

    const float m = (bins_-1)/(maxVal-minVal);
    const float b = 0.5f-m*minVal;

    matrix<float>::const_iterator it;
    const matrix<float>::const_iterator cit = in.end();

    for (it=in.begin();it!=cit;++it) {
      hist.at(static_cast<int>((*it)*m+b))++;
    }

    return true;
  }
  

  /**
   * Given the histogram, compute the between-class variances if
   * the threshold is set to k
   */
  bool thresholding::otsu(const ivector& hist,
                          int& kstar) const {

    // the method works only if something exists
    if (hist.empty()) {
      kstar=-1;
      return false;
    }

    int tmpFrom,tmpTo;
    if (method_ == Otsu) {
      tmpFrom=0;
      tmpTo  =hist.size();
    } else {
      tmpFrom=within(static_cast<int>(foreground_.from*hist.size()+0.5f),
                     0,hist.size());
      tmpTo  =within(static_cast<int>(foreground_.to  *hist.size()+0.5f),
                     0,hist.size());
    }

    const int from = tmpFrom;
    const int to   = tmpTo;

    // Prepare some accumulative data
    int last=0;
    float lastmk = 0.0f;
    ivector acchist(hist.size(),0);
    fvector nmk(hist.size(),0.0f);

    for (int i=from;i<to;++i) {
      const int h=hist.at(i);
      lastmk=(nmk.at(i)=i*h+lastmk);
      last=(acchist.at(i)=h+last);
    }

    // compute the between-class variances for all i, and in parallel, search
    // for the maxima values
    const int n = last;

    if (n==0) {
      // hmm, seems that from == to
      kstar=from;
      return true;
    }

    const float mg = lastmk/n;
    fvector bcvar(hist.size(),0.0f);

    float maxSigma = -1.0f;
    int c=0; // counter for maxima
    int maxIdx=-1;

    for (int i=from;i<to;++i) {
      const float pk=static_cast<float>(acchist.at(i));
      const float var = (bcvar.at(i) = sqr(mg*pk-nmk.at(i))/(pk*(n-pk)));

      if (var >= maxSigma) {
        if (var == maxSigma) {
          c++;
          maxIdx+=i;
        } else {
          c=1;
          maxIdx=i;
          maxSigma = var;
        }
      }
    }

    kstar = maxIdx/c;
    
    return true;
  }


  /**
   * Given the histogram, compute the between-class variances if
   * the threshold is set to k
   */
  bool thresholding::simple(const ivector& hist,
                            int& kstar) const {

    // the method works only if something exists
    if (hist.empty()) {
      kstar=-1;
      return false;
    }

    int tmpFrom,tmpTo;    if (method_ == Simple) {
      tmpFrom=0;
      tmpTo  =hist.size();
    } else {
      tmpFrom=within(static_cast<int>(foreground_.from*hist.size()+0.5f),
                     0,hist.size());
      tmpTo  =within(static_cast<int>(foreground_.to  *hist.size()+0.5f),
                     0,hist.size());
    }

    const int from = tmpFrom;
    const int to   = tmpTo;

    int th = (from+to-1)/2;
    int meanFg,meanBg;
    int nFg,nBg,i;
    int newTh;

    meanFg=meanBg=nFg=nBg=0;
    // compute the mean values
    for (i=from;i<th;++i) {
      const int h = hist.at(i);
      meanBg+=i*h;
      nBg+=h;
    }
    for (;i<to;++i) {
      const int h = hist.at(i);
      meanFg+=i*h;
      nFg+=h;
    }

    newTh = within(static_cast<int>(0.5*(double(meanFg)/nFg + 
                                         double(meanBg)/nBg) + 0.5),
                   from,to-1);
    
    while (abs(th-newTh) > deltaT_) {
      if (newTh > th) {
        for (i=th;i<newTh;++i) {
          const int h = hist.at(i);
          meanBg+=i*h;
          nBg+=h;
          meanFg-=i*h;
          nFg-=h;
        }
      } else {
        for (i=newTh;i<th;++i) {
          const int h = hist.at(i);
          meanBg-=i*h;
          nBg-=h;
          meanFg+=i*h;
          nFg+=h;
        }
      }
      
      th = newTh;

      newTh = within(static_cast<int>(0.5*(double(meanFg)/nFg +
                                           double(meanBg)/nBg) + 0.5),
                     from,to-1);
    };

    kstar = th;
    
    return true;
  }




  /**
   * Compute relative thresholds
   */
  bool thresholding::relative(const matrix<float>& src,
                              finterval& fg) const {

    const parameters& par = getParameters();

    float minVal,maxVal;
    ivector hist;
    computeHistogram(src,hist,minVal,maxVal);

    // total number of pixels
    const int total = src.rows()*src.columns();

    // first threshold
    const int low  = iround(total*par.foreground.from);
    const int high = iround(total*par.foreground.to);

    // accumulator flag 
    int acc=0;
    
    // search for the lowest threshold
    int i=0;
    while ( (acc<=low) && (i<hist.size()) ) {
      acc += hist.at(i);
      ++i;
    }
    
    // go back one
    fg.from = minVal + (maxVal - minVal) * (i-1) / hist.lastIdx();
    
    while ( (acc<=high) && (i<hist.size()) ) {
      acc += hist.at(i);
      ++i;
    }
    // go back one
    fg.to = minVal + (maxVal-minVal) * min(hist.lastIdx(),i)/hist.lastIdx();

    return true;

  }
  
  /**
   * Compute relative thresholds
   */
  bool thresholding::relative(const matrix<ubyte>& src,
                              iinterval& fg) const {

    const parameters& par = getParameters();

    ivector hist;
    computeHistogram(src,hist);

    // total number of pixels
    const int total = src.rows()*src.columns();

    // first threshold
    const int low  = iround(total*par.foreground.from);
    const int high = iround(total*par.foreground.to);

    // accumulator flag 
    int acc=0;
    
    // search for the lowest threshold
    int i=0;
    while ( (acc<=low) && (i<hist.size()) ) {
      acc += hist.at(i);
      ++i;
    }
    
    // go back one
    fg.from = iround(255.0f * (i-1) / hist.lastIdx());
    
    while ( (acc<=high) && (i<hist.size()) ) {
      acc += hist.at(i);
      ++i;
    }

    fg.to = iround(255.0f * min(hist.lastIdx(),i) / hist.lastIdx());

    return true;
  }


  // -------------------------------------------------------------------
  // io Interface
  // -------------------------------------------------------------------

  // read function for eMethod.
  bool read(ioHandler& handler,thresholding::eMethod& data) {

    std::string str;
    if (handler.read(str)) {
   
      if (str.find("irect") != std::string::npos) {
        data = thresholding::Direct;
      } else if (str.find("Otsu") != std::string::npos) {
        data = thresholding::Otsu;
      } else if (str.find("OtsuInterval") != std::string::npos) {
        data = thresholding::OtsuInterval;
      } else if (str.find("elative") != std::string::npos)  {
        data = thresholding::Relative;
      } else if (str.find("imple") != std::string::npos)  {
        data = thresholding::Simple;
      } else if (str.find("impleInterval") != std::string::npos)  {
        data = thresholding::SimpleInterval;
      } else {
        data = thresholding::Direct;
        handler.setStatusString("Undefined eMethod");
        return false;
      }

      return true;
    }

    handler.setStatusString("Could not read std::string from stream");
    return false;
  }

  // write function for eMethod.
  bool write(ioHandler& handler,const thresholding::eMethod& data) {
    bool b=false;
    switch(data) {
    case thresholding::Direct:
      b=handler.write("Direct");
      break;
    case thresholding::Otsu:
      b=handler.write("Otsu");
      break;
    case thresholding::OtsuInterval:
      b=handler.write("OtsuInterval");
      break;
    case thresholding::Simple:
      b=handler.write("Simple");
      break;
    case thresholding::SimpleInterval:
      b=handler.write("SimpleInterval");
      break;
    case thresholding::Relative:
      b=handler.write("Relative");
      break;
    default:
      b=false;
      handler.setStatusString("Undefined eMethod");
      handler.write("Unknown");
    }

    return b;
  }

}

