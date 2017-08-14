/*
 * Copyright (C) 2013
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
 * \file   ltiChessCornerness.cpp
 *         Contains the class lti::chessCornerness, which implements
 *         the ChESS corner detector proposed by Stuart Bennett and
 *         Joan Lasenby
 * 
 * \author Pablo Alvarado
 * \date   10.03.2013
 *
 * revisions ..: $Id: ltiChessCornerness.cpp,v 1.2 2013-03-10 14:10:26 alvarado Exp $
 */

#include "ltiChessCornerness.h"

#undef _LTI_DEBUG
#define _LTI_DEBUG 4

#include "ltiDebug.h"

#ifdef _LTI_DEBUG
#  include "ltiViewer2D.h"
#endif

namespace lti {
  // --------------------------------------------------
  // chessCornerness::parameters
  // --------------------------------------------------

  // default constructor
  chessCornerness::parameters::parameters()
    : cornernessFunctor::parameters() {
    boundaryType = NoBoundary;
    suppressNegatives = true;
  }

  // copy constructor
  chessCornerness::parameters::parameters(const parameters& other)
    : cornernessFunctor::parameters() {
    copy(other);
  }

  // destructor
  chessCornerness::parameters::~parameters() {
  }

  // copy member

  chessCornerness::parameters&
  chessCornerness::parameters::copy(const parameters& other) {
    cornernessFunctor::parameters::copy(other);

    boundaryType = other.boundaryType;
    suppressNegatives = other.suppressNegatives;
    return *this;
  }

  // alias for copy method
  chessCornerness::parameters&
  chessCornerness::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  // class name
  const std::string& chessCornerness::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone method
  chessCornerness::parameters*
  chessCornerness::parameters::clone() const {
    return new parameters(*this);
  }

  // new instance
  chessCornerness::parameters*
  chessCornerness::parameters::newInstance() const {
    return new parameters();
  }

  /*
   * Write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool chessCornerness::parameters::write(ioHandler& handler,
                                         const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {
      lti::write(handler,"boundaryType",boundaryType);
      lti::write(handler,"suppressNegatives",suppressNegatives);
    }

    b = b && cornernessFunctor::parameters::write(handler,false);

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
  bool chessCornerness::parameters::read(ioHandler& handler,
                                        const bool complete)  {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {
      lti::read(handler,"boundaryType",boundaryType);
      lti::read(handler,"suppressNegatives",suppressNegatives);
    }

    b = b && cornernessFunctor::parameters::read(handler,false);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

  // --------------------------------------------------
  // chessCornerness
  // --------------------------------------------------

  // default constructor
  chessCornerness::chessCornerness()
    : cornernessFunctor() {
    
    // create an instance of the parameters with the default values
    parameters defaultParameters;
    // set the default parameters
    setParameters(defaultParameters);

    this->cornernessType_=Max;
  }

  // default constructor
  chessCornerness::chessCornerness(const parameters& par)
    : cornernessFunctor() {

    // set the given parameters
    setParameters(par);

    this->cornernessType_=Max;
  }


  // copy constructor
  chessCornerness::chessCornerness(const chessCornerness& other)
    : cornernessFunctor() {
    copy(other);
    this->cornernessType_=Max;
  }

  // destructor
  chessCornerness::~chessCornerness() {
  }

  // copy member
  chessCornerness& 
  chessCornerness::copy(const chessCornerness& other) {
    cornernessFunctor::copy(other);

    return (*this);
  }

  // alias for copy member
  chessCornerness&
  chessCornerness::operator=(const chessCornerness& other) {
    return (copy(other));
  }

  // class name
  const std::string& chessCornerness::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone member
  chessCornerness* chessCornerness::clone() const {
    return new chessCornerness(*this);
  }

  // create a new instance
  chessCornerness* chessCornerness::newInstance() const {
    return new chessCornerness();
  }

  // return parameters
  const chessCornerness::parameters&
  chessCornerness::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }

  /*
   * The interpolator used is configured appropriatelly
   *
   * @return true if successful, false otherwise
   */
  bool chessCornerness::updateParameters() {
    // get the set of parameters just indicated by the user
     const parameters& par = getParameters();
     
     // do whatever you need to initialize with par
     bool success=true;

     suppressNegatives_ = par.suppressNegatives;
     nearestNeighborInterpolation<float>::parameters nniPar;
     nniPar.boundaryType = par.boundaryType;
     boundaryType_ = par.boundaryType;
     success = nni_.setParameters(nniPar);

     // return true only if everything was ok.
     return success;
  }


  /*
   * Does the actual calculation of the cornerness of \a src and leaves it
   * in \a dest.
   *
   * @param src channel with the source data.
   * @param dest channel where the result will be left.
   * @param minCornerness minimum cornerness value
   * @param maxCornerness maximum cornerness value
   * @return true if apply successful or false otherwise.
   */
  bool chessCornerness::worker(const channel& src,
			       channel& dest,
			       float& minCornerness,
			       float& maxCornerness) const {

    if (src.empty()) {
      dest.clear();
      minCornerness=maxCornerness=0.0f;
      return true; // very easy to compute an empty cornerness
    }
    
    if (src.getMode() != channel::Connected) {
      channel newSrc;
      newSrc.copy(src);  // ensure connectness;
      return worker(newSrc,dest,minCornerness,maxCornerness);
    }

    minCornerness = 1000000.f; // just and impossible huge value
    maxCornerness = 0.0f;


    if (boundaryType_ == NoBoundary) {
      dest.assign(src.size(),0.0f);
      return chess5(src,dest,minCornerness,maxCornerness);
    } else {

      static const int radius = 5;
      const int lastX = src.columns()-radius;
      const int lastY = src.rows()-radius;

      dest.allocate(src.size());
      return (chess5(src,dest,minCornerness,maxCornerness) &&
	      border5(src,irectangle(0,0,src.columns(),radius),
		      dest,minCornerness,maxCornerness) &&
	      border5(src,irectangle(0,radius,radius,lastY),
		      dest,minCornerness,maxCornerness) &&
	      border5(src,irectangle(lastX,radius,src.columns(),lastY),
		      dest,minCornerness,maxCornerness) &&
	      border5(src,irectangle(0,lastY,src.columns(),src.rows()),
		      dest,minCornerness,maxCornerness));
    }

    return false;
  }
  
  bool chessCornerness::chess5(const channel& src,
			       channel& dest,
			       float& minCornerness,
			       float& maxCornerness) const {
    static const int radius = 5;
    float ring[16];
    int x,y;
    const int lastX = src.columns()-radius;
    const int lastY = src.rows()-radius;
    for (y=radius;y<lastY;++y) {

      const int ym5=y-5;
      const int ym4=y-4;
      const int ym2=y-2;
      const int yp2=y+2;
      const int yp4=y+4;
      const int yp5=y+5;
      
      for (x=radius;x<lastX;++x) {
	// we are centered at (x,y)

	const int xm5=x-5;
	const int xm4=x-4;
	const int xm2=x-2;
	const int xp2=x+2;
	const int xp4=x+4;
	const int xp5=x+5;

	float localMean;

	// loop unrolled

	// fill the ring, trying to minimize cache misses
	ring[ 2]=src.at(ym5,xm2);
	ring[ 1]=src.at(ym5,x  );
	ring[ 0]=src.at(ym5,xp2);
	ring[ 3]=src.at(ym4,xm4);
	ring[15]=src.at(ym4,xp4);
	ring[ 4]=src.at(ym2,xm5);
	ring[14]=src.at(ym2,xp5);
	
	localMean = src.at(y-1,x);
	
	ring[ 5]=src.at(y  ,xm5);

	localMean+= (src.at(y,x-1)+src.at(y,x)+src.at(y,x+1));

	ring[13]=src.at(y  ,xp5);

	localMean+= src.at(y+1,x);

	ring[ 6]=src.at(yp2,xm5);
	ring[12]=src.at(yp2,xp5);
	ring[ 7]=src.at(yp4,xm4);
	ring[11]=src.at(yp4,xp4);
	ring[ 8]=src.at(yp5,xm2);
	ring[ 9]=src.at(yp5,x  );
	ring[10]=src.at(yp5,xp2);

	localMean*=16.0f/5.0f;

	// use the ring to compute the remaining responses
	float sumResponse = 0.0f;
	float difResponse = 0.0f;
	float mean = 0.0f;

	for (int j=0;j<4;++j) {
	  const float a000 = ring[j];
	  const float a090 = ring[j+4];
	  const float a180 = ring[j+8];
	  const float a270 = ring[j+12];

	  sumResponse+=lti::abs(a000-a090+a180-a270);
	  difResponse+=lti::abs(a000-a180)+lti::abs(a090-a270);
	  mean+=a000+a090+a180+a270;
	}

	const float res = (suppressNegatives_) ?
	  rectify(sumResponse-difResponse-lti::abs(mean-localMean)) :
	  (sumResponse-difResponse-lti::abs(mean-localMean));
	
	dest.at(y,x)=res;
	if (res>maxCornerness) {
	  maxCornerness=res;
	} else if (res<minCornerness) {
	  minCornerness=res;
	}
	
      }
    }
    return true;
  }

  float chessCornerness::chess(const channel& src,const ipoint& pos) const {
    static const int radius = 5;
    const int lastX = src.columns()-radius;
    const int lastY = src.rows()-radius;

    float ring[16];
    const int x(pos.x);
    const int y(pos.y);
    
    const int ym5=y-5;
    const int ym4=y-4;
    const int ym2=y-2;
    const int yp2=y+2;
    const int yp4=y+4;
    const int yp5=y+5;
    
    const int xm5=x-5;
    const int xm4=x-4;
    const int xm2=x-2;
    const int xp2=x+2;
    const int xp4=x+4;
    const int xp5=x+5;
      
    float localMean;
      
    if ((x<radius) || (x>=lastX) || (y<radius) || (y>=lastY)) {

      // Oh no! we got into the boundary... we have to check around what to do

      if (boundaryType_ == NoBoundary) {
	return 0.0f;
      }

      // fill the ring, trying to minimize cache misses
      ring[ 2]=nni_.interpolate(src,ym5,xm2);
      ring[ 1]=nni_.interpolate(src,ym5,x  );
      ring[ 0]=nni_.interpolate(src,ym5,xp2);
      ring[ 3]=nni_.interpolate(src,ym4,xm4);
      ring[15]=nni_.interpolate(src,ym4,xp4);
      ring[ 4]=nni_.interpolate(src,ym2,xm5);
      ring[14]=nni_.interpolate(src,ym2,xp5);
      
      localMean = nni_.interpolate(src,y-1,x);
      
      ring[ 5]=nni_.interpolate(src,y  ,xm5);
      
      localMean+= (nni_.interpolate(src,y,x-1) +
		   nni_.interpolate(src,y,x)   +
		   nni_.interpolate(src,y,x+1));
      
      ring[13]=nni_.interpolate(src,y  ,xp5);
      
      localMean+= nni_.interpolate(src,y+1,x);
      
      ring[ 6]=nni_.interpolate(src,yp2,xm5);
      ring[12]=nni_.interpolate(src,yp2,xp5);
      ring[ 7]=nni_.interpolate(src,yp4,xm4);
      ring[11]=nni_.interpolate(src,yp4,xp4);
      ring[ 8]=nni_.interpolate(src,yp5,xm2);
      ring[ 9]=nni_.interpolate(src,yp5,x  );
      ring[10]=nni_.interpolate(src,yp5,xp2);
      
    } else {
      // a faster implementation for the inside part of the image
      
      // fill the ring, trying to minimize cache misses
      ring[ 2]=src.at(ym5,xm2);
      ring[ 1]=src.at(ym5,x  );
      ring[ 0]=src.at(ym5,xp2);
      ring[ 3]=src.at(ym4,xm4);
      ring[15]=src.at(ym4,xp4);
      ring[ 4]=src.at(ym2,xm5);
      ring[14]=src.at(ym2,xp5);
      
      localMean = src.at(y-1,x);
      
      ring[ 5]=src.at(y  ,xm5);
      
      localMean+= (src.at(y,x-1)+src.at(y,x)+src.at(y,x+1));
      
      ring[13]=src.at(y  ,xp5);
      
      localMean+= src.at(y+1,x);
      
      ring[ 6]=src.at(yp2,xm5);
      ring[12]=src.at(yp2,xp5);
      ring[ 7]=src.at(yp4,xm4);
      ring[11]=src.at(yp4,xp4);
      ring[ 8]=src.at(yp5,xm2);
      ring[ 9]=src.at(yp5,x  );
      ring[10]=src.at(yp5,xp2);
      
    }

    localMean*=16.0f/5.0f;

    // use the ring to compute the remaining responses
    float sumResponse = 0.0f;
    float difResponse = 0.0f;
    float mean = 0.0f;
    
    for (int j=0;j<4;++j) {
      const float a000 = ring[j];
      const float a090 = ring[j+4];
      const float a180 = ring[j+8];
      const float a270 = ring[j+12];
      
      sumResponse+=lti::abs(a000-a090+a180-a270);
      difResponse+=lti::abs(a000-a180)+lti::abs(a090-a270);
      mean+=a000+a090+a180+a270;
    }

    const float res = (suppressNegatives_) ?
      rectify(sumResponse-difResponse-lti::abs(mean-localMean)) :
      (sumResponse-difResponse-lti::abs(mean-localMean));

    return res;
  }


  bool chessCornerness::border5(const channel& src,
				const irectangle& region,
				channel& dest,
				float& minCornerness,
				float& maxCornerness) const {
    float ring[16];
    int x,y;

    // top
    for (y=region.ul.y;y<region.br.y;++y) {

      const int ym5=y-5;
      const int ym4=y-4;
      const int ym2=y-2;
      const int yp2=y+2;
      const int yp4=y+4;
      const int yp5=y+5;
      
      for (x=region.ul.x;x<region.br.x;++x) {
	// we are centered at (x,y)

	const int xm5=x-5;
	const int xm4=x-4;
	const int xm2=x-2;
	const int xp2=x+2;
	const int xp4=x+4;
	const int xp5=x+5;

	float localMean;

	// loop unrolled

	// fill the ring, trying to minimize cache misses
	ring[ 2]=nni_.interpolate(src,ym5,xm2);
	ring[ 1]=nni_.interpolate(src,ym5,x  );
	ring[ 0]=nni_.interpolate(src,ym5,xp2);
	ring[ 3]=nni_.interpolate(src,ym4,xm4);
	ring[15]=nni_.interpolate(src,ym4,xp4);
	ring[ 4]=nni_.interpolate(src,ym2,xm5);
	ring[14]=nni_.interpolate(src,ym2,xp5);
	
	localMean = nni_.interpolate(src,y-1,x);
	
	ring[ 5]=nni_.interpolate(src,y  ,xm5);

	localMean+= (nni_.interpolate(src,y,x-1) +
		     nni_.interpolate(src,y,x)   +
		     nni_.interpolate(src,y,x+1));

	ring[13]=nni_.interpolate(src,y  ,xp5);

	localMean+= nni_.interpolate(src,y+1,x);

	ring[ 6]=nni_.interpolate(src,yp2,xm5);
	ring[12]=nni_.interpolate(src,yp2,xp5);
	ring[ 7]=nni_.interpolate(src,yp4,xm4);
	ring[11]=nni_.interpolate(src,yp4,xp4);
	ring[ 8]=nni_.interpolate(src,yp5,xm2);
	ring[ 9]=nni_.interpolate(src,yp5,x  );
	ring[10]=nni_.interpolate(src,yp5,xp2);

	localMean*=16.0f/5.0f;

	// use the ring to compute the remaining responses
	float sumResponse = 0.0f;
	float difResponse = 0.0f;
	float mean = 0.0f;

	for (int j=0;j<4;++j) {
	  const float a000 = ring[j];
	  const float a090 = ring[j+4];
	  const float a180 = ring[j+8];
	  const float a270 = ring[j+12];

	  sumResponse+=lti::abs(a000-a090+a180-a270);
	  difResponse+=lti::abs(a000-a180)+lti::abs(a090-a270);
	  mean+=a000+a090+a180+a270;
	}

	const float res = (suppressNegatives_) ?
	  rectify(sumResponse-difResponse-lti::abs(mean-localMean)) :
	  (sumResponse-difResponse-lti::abs(mean-localMean));

	dest.at(y,x)=res;
	if (res>maxCornerness) {
	  maxCornerness=res;
	} else if (res<minCornerness) {
	  minCornerness=res;
	}
	
      }
    }
    return true;
  }


  /*
   * Does the actual calculation of the cornerness of \a src and leaves it
   * in \a dest.
   *
   * Due to the different nature of grey and color images a
   * different but similar method will be used here. This can be
   * done by e.g. using a colorGradient instead of a normal
   * gradient.
   *
   * @param src channel with the source data.
   * @param dest channel where the result will be left.
   * @param minCornerness minimum cornerness value
   * @param maxCornerness maximum cornerness value
   * @return true if apply successful or false otherwise.
   */
  bool chessCornerness::worker(const image& src, 
			       channel& dest,
			       float& minCornerness,
			       float& maxCornerness) const {
    
    if (src.empty()) {
      dest.clear();
      minCornerness=maxCornerness=0.0f;
      return true; // very easy to compute an empty cornerness
    }

    // to avoid H calculation, compute here the saturation and
    // intensity channels
    vector<rgbaPixel>::const_iterator slit;
    
    channel intensity(src.size()),asaturation(src.size());
    channel::iterator iit=intensity.begin();
    channel::iterator sit=asaturation.begin();

    // efficiently split the channel
    int r,g,b;
    for (int y=0;y<src.rows();++y) {
      const vector<rgbaPixel>& srcline = src.getRow(y);
      const vector<rgbaPixel>::const_iterator sleit = srcline.end();
      for (slit=srcline.begin();slit!=sleit;++slit,++iit,++sit) {
        (*slit).get(r,g,b);
        const float i    = static_cast<float>(r+g+b)/3.0f;
        (*iit) = i/255.0f; // intensity
        // 1-saturation:
        (*sit) = (i > 0.0f) ? (static_cast<float>(min(r,g,b))/i) : 1.0f;
      }
    }

#ifdef _LTI_DEBUG
    static viewer2D iview("Intensity");
    static viewer2D sview("Saturation");
    iview.show(intensity);
    sview.show(asaturation);
#endif

    // now compute the cornerness of the intensity channel
    if (worker(intensity,dest,minCornerness,maxCornerness)) {

      // now recompute with saturation
      const channel::const_iterator dit =dest.end();
      float s = asaturation.at(0,0);
      minCornerness = maxCornerness = dest.at(0,0)*s;
      
      for (iit=dest.begin(),sit=asaturation.begin();iit!=dit;++iit,++sit) {
	const float v = ((*iit)*=(*sit));
	if (v>maxCornerness) {
	  maxCornerness = v;
	} else if (v<minCornerness) {
	  minCornerness = v;
	}
      }
    } else {
      return false;
    }
    
    return true;
    
  }



}

