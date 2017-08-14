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
 * \file   ltiComputePalette.cpp
 *         Compute a palette based on a color image and a labeled mask.
 * \author Pablo Alvarado
 * \date   11.3.2002
 *
 * revisions ..: $Id: ltiComputePalette.cpp,v 1.2 2012-01-03 02:22:25 alvarado Exp $
 */

#include "ltiComputePalette.h"

#undef _LTI_DEBUG
// #define _LTI_DEBUG 1
#include "ltiDebug.h"

namespace lti {
  // --------------------------------------------------
  // computePalette
  // --------------------------------------------------

  // default constructor
  computePalette::computePalette()
    : functor() {
  }

  // copy constructor
  computePalette::computePalette(const computePalette& other)
    : functor() {
    copy(other);
  }

  // destructor
  computePalette::~computePalette() {
  }

  // returns the name of this type
  const std::string& computePalette::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // copy member
  computePalette& computePalette::copy(const computePalette& other) {
    functor::copy(other);
    
    return (*this);
  }

  // alias for copy member
  computePalette&
    computePalette::operator=(const computePalette& other) {
    return (copy(other));
  }


  // clone member
  computePalette* computePalette::clone() const {
    return new computePalette(*this);
  }

  computePalette* computePalette::newInstance() const {
    return new computePalette;
  }

  // return parameters
  const computePalette::parameters&
    computePalette::getParameters() const {
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


  /**
   * operates on a copy of the given %parameters.
   * @param src image with the source data.
   * @param mask mask with palette entry indices.
   * @param dest computed palette.
   * @return true if apply successful or false otherwise.
   */
  bool computePalette::apply(const image& src,
                             const channel8& mask,
                             palette& dest) const {
    vector<rgbPixel<float> > stdDev;
    return apply(src,mask,dest,stdDev);
  }

  /**
   * operates on a copy of the given %parameters.
   * @param src image with the source data.
   * @param mask mask with palette entry indices.
   * @param dest computed palette.
   * @return true if apply successful or false otherwise.
   */
  bool computePalette::apply(const image& src,
                             const matrix<int>& mask,
                             palette& dest) const {
    vector<rgbPixel<float> > stdDev;
    return apply(src,mask,dest,stdDev);
  }

  bool computePalette::apply(const image& src,
                             const matrix<int>& mask,
                             palette& dest,
                             vector<rgbPixel<float> >& palStdDev) const {
    vector<int> n;
    return apply(src,mask,dest,palStdDev,n);
  }

  bool computePalette::apply(const image& src,
                             const matrix<int>& mask,
                             vector<rgbPixel<float> >& means,
                             ivector& n,
                             const int numLabels) const {

    if (src.size() != mask.size()) {
      setStatusString("Labeled mask and image must have the same size");
      return false;
    }

    const int labels = (numLabels < 0) ? mask.findMaximum()+1 : numLabels;
    means.assign(labels,rgbPixel<float>(0,0,0));
    n.assign(labels,0);
    
    int y,x,l;
    for (y=0;y<src.rows();++y) {
      for (x=0;x<src.columns();++x) {
        l = mask.at(y,x);
        means.at(l)+=src.at(y,x);
        n.at(l)++;
      }
    }

    // compute the mean values
    for (l=0;l<labels;++l) {
      if (n.at(l)>0) {
        means.at(l)/=static_cast<float>(n.at(l));
      }
    }

    return true;
  }

  bool computePalette::apply(const image& src,
                             const channel8& mask,
                             palette& dest,
                             vector<rgbPixel<float> >& palStdDev) const {
    vector<int> n;
    return apply(src,mask,dest,palStdDev,n);
  }

  /**
   * operates on a copy of the given %parameters.
   * @param src image with the source data.
   * @param mask mask with palette entry indices.
   * @param dest computed palette (mean value of all elements with the
   *                               same index).
   * @param palStdDev standard deviation for each palette entry.
   * @return true if apply successful or false otherwise.
   */
  bool computePalette::apply(const image& src,
                             const channel8& mask,
                             palette& dest,
                             vector<rgbPixel<float> >& palStdDev,
                             vector<int>& n) const {

    if (src.size() != mask.size()) {
      setStatusString("Source image and index mask have different sizes");
      return false;
    }

    vector<rgbPixel<float> > sum2(256,rgbPixel<float>(0,0,0));
    vector<rgbPixel<float> >  sum(256,rgbPixel<float>(0,0,0));
    n.assign(256,0);

    int y,i,m,maxi(0);
    vector<rgbaPixel>::const_iterator sit,eit;
    vector<channel8::value_type>::const_iterator cit;
    rgbPixel<float> px;

    for (y=0;y<src.rows();y++) {
      const vector<rgbaPixel>& vct=src.getRow(y);
      const vector<channel8::value_type>& idx=mask.getRow(y);
      for (sit=vct.begin(),eit=vct.end(),cit=idx.begin();
           sit!=eit;
           ++sit,++cit) {
        i = *cit;
        maxi=max(i,maxi);
        n.at(i)++;
        px.copy(*sit);
        sum.at(i).add(px);
        sum2.at(i).add(px*px);
      }
    }

    maxi++;
    dest.allocate(maxi);
    palStdDev.allocate(maxi);

    for (i=0;i<maxi;++i) {
      m = n.at(i);
      const rgbPixel<float>& s = sum.at(i);
      const rgbPixel<float>& s2 = sum2.at(i);

      dest.at(i).setRed(static_cast<ubyte>(s.red/m));
      dest.at(i).setGreen(static_cast<ubyte>(s.green/m));
      dest.at(i).setBlue(static_cast<ubyte>(s.blue/m));
      dest.at(i).setAlpha(0);

      if (m>1) {
        palStdDev.at(i).setRed(sqrt((s2.red-(s.red*s.red/m))/(m-1)));
        palStdDev.at(i).setGreen(sqrt((s2.green-(s.green*s.green/m))/(m-1)));
        palStdDev.at(i).setBlue(sqrt((s2.blue - (s.blue*s.blue/m))/(m-1)));
      } else {
        palStdDev.at(i) = rgbPixel<float>(0,0,0);
      }
    }

    n.resize(maxi,0);

    return true;
  }

  /**
   * operates on a copy of the given %parameters.
   * @param src image with the source data.
   * @param mask mask with palette entry indices.
   * @param dest computed palette (mean value of all elements with the
   *                               same index).
   * @param palStdDev standard deviation for each palette entry.
   * @return true if apply successful or false otherwise.
   */
  bool computePalette::apply(const image& src,
                             const matrix<int>& mask,
                             palette& dest,
                             vector<rgbPixel<float> >& palStdDev,
                             vector<int>& n) const {
    if (src.size() != mask.size()) {
      setStatusString("Source image and index mask have different sizes");
      return false;
    }

    int y,i,m,maxi;

    maxi = mask.findMaximum()+1;

    vector<rgbPixel<float> > sum2(maxi,rgbPixel<float>(0,0,0));
    vector<rgbPixel<float> >  sum(maxi,rgbPixel<float>(0,0,0));
    n.assign(maxi,0);

    vector<rgbaPixel>::const_iterator sit,eit;
    vector<int>::const_iterator cit;
    rgbPixel<float> px;

    for (y=0;y<src.rows();y++) {
      const vector<rgbaPixel>& vct=src.getRow(y);
      const vector<int>& idx=mask.getRow(y);
      for (sit=vct.begin(),eit=vct.end(),cit=idx.begin();
           sit!=eit;
           ++sit,++cit) {
        i = *cit;
        n.at(i)++;
        px.copy(*sit);
        sum.at(i).add(px);
        sum2.at(i).add(px*px);
      }
    }

    dest.allocate(maxi);
    palStdDev.allocate(maxi);

    for (i=0;i<maxi;++i) {
      m = n.at(i);
      const rgbPixel<float>& s = sum.at(i);
      const rgbPixel<float>& s2 = sum2.at(i);

      dest.at(i).setRed(static_cast<ubyte>(s.red/m));
      dest.at(i).setGreen(static_cast<ubyte>(s.green/m));
      dest.at(i).setBlue(static_cast<ubyte>(s.blue/m));
      dest.at(i).setAlpha(0);

      if (m>1) {
        palStdDev.at(i).setRed(sqrt((s2.red-(s.red*s.red/m))/(m-1)));
        palStdDev.at(i).setGreen(sqrt((s2.green-(s.green*s.green/m))/(m-1)));
        palStdDev.at(i).setBlue(sqrt((s2.blue - (s.blue*s.blue/m))/(m-1)));
      } else {
        palStdDev.at(i) = rgbPixel<float>(0,0,0);
      }
    }

    return true;
  }

  //

  /**
   * this apply will generate multivariate statistics for each
   * entry in the palette. It takes more time than the other
   * ones.
   * @param src image with the source data.
   * @param mask mask with palette entry indices.
   * @param dest computed palette (mean value of all elements with the
   *                               same index).  Note that this apply
   *                               will compute with floats)
   * @param palCovar covariance matrices for each palette entry.
   * @param n number of pixels found for each entry
   * @return true if apply successful or false otherwise.
   */
  bool computePalette::apply(const image& src,
                             const matrix<int>& mask,
                             vector<rgbPixel<float> >& dest,
                             std::vector<matrix<float> >& palCovar,
                             vector<int>& n) const {

    if (src.size() != mask.size()) {
      setStatusString("Source image and index mask have different sizes");
      return false;
    }

    int y,i,m,maxi;

    maxi = mask.findMaximum()+1;

    palCovar.resize(maxi);
    dest.allocate(maxi);
    n.assign(maxi,0);

    matrix<double> tmpCovar(3,3,0.0f);
    std::vector<matrix<double> > sumOuter(maxi,tmpCovar);
    vector<rgbPixel<float> > sum(maxi,rgbPixel<float>(0.0f,0.0f,0.0f));

    vector<rgbaPixel>::const_iterator sit,eit;
    vector<int>::const_iterator cit;
    matrix<double>::iterator omit;

    rgbPixel<float> px;

    for (y=0;y<src.rows();y++) {
      const vector<rgbaPixel>& vct=src.getRow(y);
      const vector<int>& idx=mask.getRow(y);
      for (sit=vct.begin(),eit=vct.end(),cit=idx.begin();
           sit!=eit;
           ++sit,++cit) {
        i = *cit;
        n.at(i)++;
        px.copy(*sit);
        sum.at(i).add(px);

        // outer product of color is symmetric => consider only the upper
        // triangular matrix, and when asking for the model compute the
        // complete one...
        omit = sumOuter[i].begin();

        (*omit) += px.red*px.red;
        ++omit;
        (*omit) += px.red*px.green;
        ++omit;
        (*omit) += px.red*px.blue;
        omit+=2;
        (*omit) += px.green*px.green;
        ++omit;
        (*omit) += px.green*px.blue;
        omit+=3;
        (*omit) += px.blue*px.blue;
      }
    }

    for (i=0;i<maxi;++i) {
      m = n.at(i);
      if (m>0) {
        dest.at(i).divide(sum.at(i),static_cast<float>(m));
        const rgbPixel<float>& s = dest.at(i);
        outer(s,s,tmpCovar);

        sumOuter[i].at(1,0)=sumOuter[i].at(0,1);
        sumOuter[i].at(2,0)=sumOuter[i].at(0,2);
        sumOuter[i].at(2,1)=sumOuter[i].at(1,2);

        sumOuter[i].addScaled(static_cast<double>(-m),tmpCovar);
        if (m>1) {
          sumOuter[i].divide(static_cast<double>(m-1));
          palCovar[i].castFrom(sumOuter[i]);
        } else {
          palCovar[i].assign(3,3,0.0f); // fill with zeros
        }

      } else {
        palCovar[i].castFrom(sumOuter[i]); // copy a 3x3 zero matrix
      }
    }

    return true;
  }

  /**
   * this apply will generate multivariate statistics for each
   * entry in the palette.  It takes more time than the other
   * ones.  The image will be assumed to be composed by the given three
   * channels.
   *
   * @param ch1 first channel of the image
   * @param ch2 second channel of the image
   * @param ch3 third channel of the image
   * @param mask mask with palette entry indices.
   * @param dest computed palette (mean value of all elements with the
   *                               same index).  Note that this apply
   *                               will compute with floats)
   * @param palCovar covariance matrices for each palette entry.
   * @param n number of pixels found for each entry
   * @return true if apply successful or false otherwise.
   */
  bool computePalette::apply(const channel& ch1,
                             const channel& ch2,
                             const channel& ch3,
                             const matrix<int>& mask,
                             vector<rgbPixel<float> >& dest,
                             std::vector<matrix<float> >& palCovar,
                             vector<int>& n) const {

    if (ch1.size() != mask.size()) {
      setStatusString("Source channels and index mask have different sizes");
      return false;
    }

    if ((ch1.size() != ch2.size()) ||
        (ch1.size() != ch3.size())) {
      setStatusString("Source channels must have the same size.");
      return false;
    }

    int y,x,i,m,maxi;

    maxi = mask.findMaximum()+1;

    palCovar.resize(maxi);
    dest.allocate(maxi);
    n.assign(maxi,0);

    matrix<double> tmpCovar(3,3,0.0f);
    std::vector<matrix<double> > sumOuter(maxi,tmpCovar);
    vector<rgbPixel<float> > sum(maxi,rgbPixel<float>(0.0f,0.0f,0.0f));

    matrix<double>::iterator omit;
    rgbPixel<float> px;

    for (y=0;y<ch1.rows();++y) {
      for (x=0;x<ch1.columns();++x) {
        i = mask.at(y,x);
        n.at(i)++;
        px.set(ch1.at(y,x),ch2.at(y,x),ch3.at(y,x));
        sum.at(i).add(px);

        // outer product of color is symmetric => consider only the upper
        // triangular matrix, and when asking for the model compute the
        // complete one...
        omit = sumOuter[i].begin();

        (*omit) += px.red*px.red;
        ++omit;
        (*omit) += px.red*px.green;
        ++omit;
        (*omit) += px.red*px.blue;
        omit+=2;
        (*omit) += px.green*px.green;
        ++omit;
        (*omit) += px.green*px.blue;
        omit+=3;
        (*omit) += px.blue*px.blue;
      }
    }

    for (i=0;i<maxi;++i) {
      m = n.at(i);
      if (m>0) {
        dest.at(i).divide(sum.at(i),static_cast<float>(m));
        const rgbPixel<float>& s = dest.at(i);
        outer(s,s,tmpCovar);

        sumOuter[i].at(1,0)=sumOuter[i].at(0,1);
        sumOuter[i].at(2,0)=sumOuter[i].at(0,2);
        sumOuter[i].at(2,1)=sumOuter[i].at(1,2);

        sumOuter[i].addScaled(static_cast<double>(-m),tmpCovar);
        if (m>1) {
          sumOuter[i].divide(static_cast<double>(m-1));
          palCovar[i].castFrom(sumOuter[i]);
        } else {
          palCovar[i].assign(3,3,0.0f); // fill with zeros
        }

      } else {
        palCovar[i].castFrom(sumOuter[i]); // copy a 3x3 zero matrix
      }
    }

    return true;
  }

  /**
   * this apply will generate multivariate statistics for each
   * entry in the palette.  It takes more time than the other
   * ones.  The image will be assumed to be composed by the given three
   * channels.
   *
   * @param ch1 first channel of the image
   * @param ch2 second channel of the image
   * @param ch3 third channel of the image
   * @param mask mask with palette entry indices.
   * @param dest computed palette (mean value of all elements with the
   *                               same index).  Note that this apply
   *                               will compute with floats)
   * @param palCovar covariance matrices for each palette entry.
   * @param n number of pixels found for each entry
   * @return true if apply successful or false otherwise.
   */
  bool computePalette::apply(const channel& ch1,
                             const channel& ch2,
                             const channel& ch3,
                             const matrix<int>& mask,
                             vector<rgbPixel<float> >& dest,
                             vector<int>& n) const {

    if (ch1.size() != mask.size()) {
      setStatusString("Source channels and index mask have different sizes");
      return false;
    }

    if ((ch1.size() != ch2.size()) ||
        (ch1.size() != ch3.size())) {
      setStatusString("Source channels must have the same size.");
      return false;
    }

    int y,x,i,m,maxi;

    maxi = mask.findMaximum()+1;

    dest.assign(maxi,rgbPixel<float>(0.0f,0.0f,0.0f));
    n.assign(maxi,0);

    rgbPixel<float> px;

    for (y=0;y<ch1.rows();++y) {
      for (x=0;x<ch1.columns();++x) {
        i = mask.at(y,x);
        n.at(i)++;
        px.set(ch1.at(y,x),ch2.at(y,x),ch3.at(y,x));
        dest.at(i).add(px);
      }
    }
    
    for (i=0;i<maxi;++i) {
      m = n.at(i);
      if (m>0) {
        dest.at(i).divide(static_cast<float>(m));
      }
    }

    return true;
  }



  /**
   * This apply method computes the mean values and variances of the
   * regions indicated by the mask for the given channel.
   *
   * @param ch channel for which the region statistics will be computed
   * @param mask labeled mask indicating the regions
   * @param means mean values for each region
   * @param vars variances for each region
   * @param n number of pixels per region
   * @return true if successful, false otherwise.
   */
  bool computePalette::apply(const channel& ch,
                             const matrix<int>& mask,
                             vector<float>& means,
                             vector<float>& vars,
                             vector<int>& n) const {
    
    if (ch.size() != mask.size()) {
      setStatusString("Channel and mask have different sizes");
      return false;
    }
    
    int y,x,i,m;

    const int maxi = mask.findMaximum()+1;

    vars.assign(maxi,float(0));
    means.assign(maxi,float(0));
    n.assign(maxi,0);

    for (y=0;y<ch.rows();++y) {
      for (x=0;x<ch.columns();++x) {
        i = mask.at(y,x);
        const float v = ch.at(y,x);

        n.at(i)++;
        means.at(i)+=v;
        vars.at(i)+=(v*v);
      }
    }

    for (i=0;i<maxi;++i) {
      m = n.at(i);
      if (m>0) {
        const float avr = (means.at(i)/=m);
        vars.at(i)      = (vars.at(i)/m) - (avr*avr);
      } 
    }

    return true;
  }


  /**
   * compute the mean value and covariance matrix for just one
   * label in the mask.
   *
   * @param src image with the source data.
   * @param mask mask with palette entry indices.
   * @param label label to be analyzed.
   * @param mean mean color of the given label.
   * @param covar covariance matrix for the given label.
   * @param n number of pixels found with the given label.
   * @return true if apply successful or false otherwise.
   */
  bool computePalette::apply(const image& src,
                             const matrix<int>& mask,
                             const int label,
                             rgbPixel<float>& mean,
                             matrix<float>& covar,
                             int& n) const {

    if (src.size() != mask.size()) {
      setStatusString("Source image and index mask have different sizes");
      return false;
    }

    int y,m; // ,maxi;

    // maxi = mask.findMaximum()+1;

    matrix<double> tmpCovar(3,3,0.0f);
    covar.castFrom(tmpCovar);
    matrix<double> sumOuter(tmpCovar);

    rgbPixel<float> sum(0.0f,0.0f,0.0f);
    mean.copy(sum);
    n = 0;

    vector<rgbaPixel>::const_iterator sit,eit;
    vector<int>::const_iterator cit;
    matrix<double>::iterator omit;

    rgbPixel<float> px;

    for (y=0;y<src.rows();y++) {
      const vector<rgbaPixel>& vct=src.getRow(y);
      const vector<int>& idx=mask.getRow(y);
      for (sit=vct.begin(),eit=vct.end(),cit=idx.begin();
           sit!=eit;
           ++sit,++cit) {
        if (*cit == label) {
          n++;
          px.copy(*sit);
          sum.add(px);

          // outer product of color is symmetric => consider only the upper
          // triangular matrix, and when asking for the model compute the
          // complete one...
          omit = sumOuter.begin();

          (*omit) += px.red*px.red;
          ++omit;
          (*omit) += px.red*px.green;
          ++omit;
          (*omit) += px.red*px.blue;
          omit+=2;
          (*omit) += px.green*px.green;
          ++omit;
          (*omit) += px.green*px.blue;
          omit+=3;
          (*omit) += px.blue*px.blue;
        }
      }
    }

    m = n;
    if (m>0) {
      mean.divide(sum,static_cast<float>(m));
      const rgbPixel<float>& s = mean;
      outer(s,s,tmpCovar);

      sumOuter.at(1,0)=sumOuter.at(0,1);
      sumOuter.at(2,0)=sumOuter.at(0,2);
      sumOuter.at(2,1)=sumOuter.at(1,2);

      sumOuter.addScaled(static_cast<double>(-m),tmpCovar);
      if (m>1) {
        sumOuter.divide(static_cast<double>(m-1));
        covar.castFrom(sumOuter);
      } else {
        covar.assign(3,3,0.0f); // fill with zeros
      }

    } else {
      covar.castFrom(sumOuter); // copy a 3x3 zero matrix
    }

    return true;
  }

  /*
   * outer product of two colors
   */
  void computePalette::outer(const rgbPixel<float>& a,
                             const rgbPixel<float>& b,
                                   matrix<double>& mat) const {
    matrix<double>::iterator it = mat.begin();
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
