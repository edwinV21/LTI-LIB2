/*
 * Copyright (C) 1998-2005
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

/**
 * \file   ltiGradientFunctor.cpp
 *         This file contains the gradientFunctor which is a
 *         convenient wrapper for convolution of a channel with a
 *         gradient kernel.
 * \author Pablo Alvarado
 * \date   01.06.2003
 *
 * $Id: ltiGradientFunctor.cpp,v 1.9 2010-11-26 09:13:19 schnurr Exp $
 */

#include "ltiGradientFunctor.h"
#include "ltiSplitImageToRGB.h"
#include "ltiArctanLUT.h"
#include "ltiGradientKernels.h"
#include "ltiOgdKernels.h"
#include "ltiConvolution.h"
#include  <limits>

namespace lti {
  // --------------------------------------------------
  // gradientFunctor::parameters
  // --------------------------------------------------

  // default constructor
  gradientFunctor::parameters::parameters()
    : functor::parameters() {
    format = Polar;
    kernelType = Ando;
    gradientKernelSize = 3;
    ogdVariance = -1.0f;
  }

  // copy constructor
  gradientFunctor::parameters::parameters(const parameters& other)
    : functor::parameters() {
    copy(other);
  }


  // destructor
  gradientFunctor::parameters::~parameters() {
  }

  // get type name
  const std::string& gradientFunctor::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // copy member
  gradientFunctor::parameters&
    gradientFunctor::parameters::copy(const parameters& other) {
    functor::parameters::copy(other);

    format = other.format;
    kernelType = other.kernelType;
    gradientKernelSize = other.gradientKernelSize;
    ogdVariance=other.ogdVariance;
    return *this;
  }

  // alias for copy member
  gradientFunctor::parameters&
    gradientFunctor::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  // clone method
  gradientFunctor::parameters* gradientFunctor::parameters::clone() const {
    return new parameters(*this);
  }

  // new instance
  gradientFunctor::parameters* gradientFunctor::parameters::newInstance() const {
    return new parameters();
  }

  /*
   * write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool gradientFunctor::parameters::write(ioHandler& handler,
                                         const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {

      lti::write(handler,"format",format);
      lti::write(handler,"kernelType",kernelType);
      lti::write(handler,"gradientKernelSize",gradientKernelSize);
      lti::write(handler,"ogdVariance",ogdVariance);
    }

    b = b && functor::parameters::write(handler,false);

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
  bool gradientFunctor::parameters::read(ioHandler& handler,
                                        const bool complete) {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {

      lti::read(handler,"format",format);
      lti::read(handler,"kernelType",kernelType);
      lti::read(handler,"gradientKernelSize",gradientKernelSize);
      lti::read(handler,"ogdVariance",ogdVariance);
    }

    b = b && functor::parameters::read(handler,false);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

  // --------------------------------------------------
  // gradientFunctor
  // --------------------------------------------------

  // default constructor
  gradientFunctor::gradientFunctor(const bool initParam) {
    if (initParam) {
      parameters defaultParameters;
      // set the default parameters
      setParameters(defaultParameters);
    }
  }

  // default constructor
  gradientFunctor::gradientFunctor(const eOutputFormat& f,
                                   const int gradKernelSize)
    : functor() {

    // create an instance of the parameters with the default values
    parameters defaultParameters;
    defaultParameters.format=f;
    defaultParameters.gradientKernelSize = gradKernelSize;

    // set the default parameters
    setParameters(defaultParameters);
  }

  // default constructor
  gradientFunctor::gradientFunctor(const parameters& par)
    : functor() {
    // set the given parameters
    setParameters(par);
  }


  // copy constructor
  gradientFunctor::gradientFunctor(const gradientFunctor& other)
    : functor() {
    copy(other);
  }

  // destructor
  gradientFunctor::~gradientFunctor() {
  }

  // returns the name of this type
  const std::string& gradientFunctor::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // copy member
  gradientFunctor&
  gradientFunctor::copy(const gradientFunctor& other) {
    functor::copy(other);

    return (*this);
  }

  // alias for copy member
  gradientFunctor&
  gradientFunctor::operator=(const gradientFunctor& other) {
    return (copy(other));
  }

  // clone member
  gradientFunctor* gradientFunctor::clone() const {
    return new gradientFunctor(*this);
  }

  // newInstance member
  gradientFunctor* gradientFunctor::newInstance() const {
    return new gradientFunctor();
  }

  // return parameters
  const gradientFunctor::parameters&
  gradientFunctor::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }



  bool gradientFunctor::apply(const channel8& src,
                                    channel& xOrMag,
                                    channel& yOrArg) const {

    const parameters& par = getParameters();

    if ((par.format == Polar) &&
        (par.kernelType == Difference)) {
      // extra fast version required by Canny and maybe other functors.
      return xyDifferentiateImage(src,xOrMag,yOrArg);
    } else {
      // other methods need to be converted to float
      // If not, some other specialized methods need to be created.
      channel tmp;
      tmp.castFrom(src);
      return apply(tmp,xOrMag,yOrArg);
    }


  }

  bool gradientFunctor::apply(const channel& src,
                                    channel& xOrMag,
                                    channel& yOrArg) const {

    if (computeGradientCart(src,xOrMag,yOrArg)) {
      const parameters& par = getParameters();
      if (par.format == Polar) {
        int y,x;
        float cx,cy;
        // For Roberts we still need to fix the angle, which is shifted 45 degrees:
        if (par.kernelType == Roberts) {
          const float phaseOffset = static_cast<float>(Pi/4.0);
          const float maxPhase = static_cast<float>(2* Pi);
          float tmpPhase;
          for (y=0;y<src.rows();++y) {
            for (x=0;x<src.columns();++x) {
              cx=xOrMag.at(y,x);
              cy=yOrArg.at(y,x);
              xOrMag.at(y,x) = sqrt(cx*cx + cy*cy);
              tmpPhase = atan2_(cy,cx) + phaseOffset;
              tmpPhase = (tmpPhase < maxPhase) ? tmpPhase : tmpPhase - maxPhase;
              yOrArg.at(y,x) = tmpPhase;
            }
          }
        }
        else
        {
          for (y=0;y<src.rows();++y) {
            for (x=0;x<src.columns();++x) {
              cx=xOrMag.at(y,x);
              cy=yOrArg.at(y,x);
              xOrMag.at(y,x) = sqrt(cx*cx + cy*cy);
              yOrArg.at(y,x) = atan2_(cy,cx);
            }
          }
        }


      }
    } else {
      return false;
    }

    return true;
  }


  /* on copy apply to compute gradient magnitude */
  bool gradientFunctor::apply(const channel& src,
                             channel& mag) const {
    channel tmp;
    bool rc = computeGradientCart(src,mag,tmp);
    int rows = src.rows();
    int y;
    vector<float>::iterator citx,e;
    vector<float>::const_iterator city;
    if (rc) {
      for (y=0;y<rows;y++) {
        vector<float>& vctx = mag.getRow(y);
        const vector<float>& vcty = tmp.getRow(y);
        for (citx=vctx.begin(),
             city=vcty.begin(),e=vctx.end();
             citx!=e;
             ++citx,++city) {
          const float cx=*citx;
          const float cy=*city;
          (*citx) = sqrt(cx*cx+cy*cy);
        }
      }
    }
    return rc;
  }

  /* on place apply to compute gradient magnitude */
  bool gradientFunctor::apply(channel& srcdest) const {
    channel tmp;

    bool b = apply(srcdest,tmp);
    tmp.detach(srcdest);

    return b;
  }


   bool gradientFunctor::computeGradientCart(const channel& src,
                                                   channel& dx,
                                                   channel& dy) const {

     const parameters& par = getParameters();
     dx.allocate(src.size());
     dy.allocate(src.size());

     switch(par.kernelType) {
       case Ando:
         return xyDifferentiateImageCartOpt(src,dx,dy);

       case Difference:
         return xyDifferentiateImageCart(src,dx,dy);

       case Roberts:
         //inter-pixel gradient
         return xyDifferentiateImageCartInterPixel(src,dx,dy);

       case OGD:
       case Sobel:
       case Prewitt:
       case Harris:
         return xyDifferentiateImageCartClassicSep(src,dx,dy);

       case Robinson:
       case Kirsch:
         return xyDifferentiateImageCartClassic2D(src,dx,dy);

       default:
         setStatusString("Wrong kernel type");
         return false;
     }



   }

  bool
  gradientFunctor::xyDifferentiateImageCartOpt(const channel& src,
                                                     channel& dx,
                                                     channel& dy) const {

#   ifdef _LTI_DEBUG
    static viewer viewGX("GX"),viewGY("GY");
#   endif

    const parameters& par = getParameters();
    const int kernelSize = min(5,max(3,par.gradientKernelSize));


    andoKernelX<channel::value_type> gx(kernelSize);
    andoKernelY<channel::value_type> gy(kernelSize);

    convolution grad;
    convolution::parameters gradpar;

    gradpar.boundaryType = lti::Constant;

    // x direction
    gradpar.setKernel(gx);
    grad.setParameters(gradpar);
    grad.apply(src,dx);

    // y direction
    grad.setKernel(gy);
    grad.apply(src,dy);

#   ifdef _LTI_DEBUG
    viewGX.show(dx);
    viewGY.show(dy);
#   endif

    if (par.gradientKernelSize != kernelSize) {
      setStatusString("Wrong size for Ando kernel (only 3,4,5 accepted)");
      return false;
    }

    return true;
  }

  bool
  gradientFunctor::xyDifferentiateImageCartClassicSep(const channel& src,
                                                      channel& dx,
                                                      channel& dy) const {

#   ifdef _LTI_DEBUG
    static viewer viewGX("GX"),viewGY("GY");
#   endif

    const parameters& par = getParameters();

    separableKernel<channel::value_type> gx,gy;

    switch (par.kernelType) {
      case OGD:
        gx.copy(ogd1Kernel<channel::value_type>
                (ogd1Kernel<channel::value_type>::X,
                 par.gradientKernelSize,
                 par.ogdVariance));
        gy.copy(ogd1Kernel<channel::value_type>
                (ogd1Kernel<channel::value_type>::Y,
                 par.gradientKernelSize,
                 par.ogdVariance));
        break;
      case Sobel:
        gx.copy(sobelKernelX<channel::value_type>(true));
        gy.copy(sobelKernelY<channel::value_type>(true));
        break;
      case Harris:
        gx.copy(harrisKernelX<channel::value_type>());
        gy.copy(harrisKernelY<channel::value_type>());
        break;
      case Prewitt:
        gx.copy(prewittKernelX<channel::value_type>(true));
        gy.copy(prewittKernelY<channel::value_type>(true));
        break;
      default:
        setStatusString("Unknown kernel type in gradientFunctor\n");
        return false;

    }

    convolution grad;
    convolution::parameters gradpar;

    gradpar.boundaryType = lti::Constant;

    // x direction
    gradpar.setKernel(gx);
    grad.setParameters(gradpar);
    grad.apply(src,dx);

    // y direction
    grad.setKernel(gy);
    grad.apply(src,dy);

#   ifdef _LTI_DEBUG
    viewGX.show(dx);
    viewGY.show(dy);
#   endif

    return true;
  }

  bool
  gradientFunctor::xyDifferentiateImageCartClassic2D(const channel& src,
                                                     channel& dx,
                                                     channel& dy) const {

#   ifdef _LTI_DEBUG
    static viewer viewGX("GX"),viewGY("GY");
#   endif

    const parameters& par = getParameters();

    kernel2D<channel::value_type> gx,gy;

    switch (par.kernelType) {
      case Robinson:
        gx.copy(robinsonKernelX<channel::value_type>());
        gy.copy(robinsonKernelY<channel::value_type>());
        break;
      case Kirsch:
        gx.copy(kirschKernelX<channel::value_type>());
        gy.copy(kirschKernelY<channel::value_type>());
        break;
      default:
        setStatusString("Unkown 2D kernel type in gradientFunctor\n");
        return false;

    }

    convolution grad;
    convolution::parameters gradpar;

    gradpar.boundaryType = lti::Constant;

    // x direction
    gradpar.setKernel(gx);
    grad.setParameters(gradpar);
    grad.apply(src,dx);

    // y direction
    grad.setKernel(gy);
    grad.apply(src,dy);

#   ifdef _LTI_DEBUG
    viewGX.show(dx);
    viewGY.show(dy);
#   endif

    return true;
  }

  bool
  gradientFunctor::xyDifferentiateImageCart(const channel& src, //in:
                                                  channel& dx,    //out: x
                                                  channel& dy     //out: y
                                                  ) const {

    if (src.columns() < 3) {
      setStatusString("width less than 3");
      return false;
    }
    if (src.rows() < 3) {
      setStatusString("height less than 3");
      return false;
    }

    if (src.getMode()!=channel::Connected) {
      setStatusString("src must be Connected");
      return false;
    }

    const int width  = src.columns();
    const int height = src.rows();

    float* fpDx = &dx.at(0,0);
    float* fpDy = &dy.at(0,0);

    const float* fpSrc = &src.at(0,0);
    const float* rowy;
    const float* colx;

    float* pidx;
    float* pidy;

    const int w1 = width-1;
    const int w2 = width-2;
    const int last = (height-1)*width; // index of begin of last row
    const int lastRow = -w1;           // offset from actual column pointer to
                                       // last row
    const int nextRow = width+1;       // offset from actual column pointer to
                                       // next row

    // top-left corner
    fpDx[0]=(fpSrc[1]-fpSrc[0]);
    fpDy[0]=(fpSrc[width]-fpSrc[0]);

    // top
    pidx = &fpDx[1];
    pidy = &fpDy[1];

    for (colx=&fpSrc[0],rowy=&fpSrc[w1];
         colx<rowy;
         ++colx,++pidx,++pidy) {
      *pidx=(colx[2] - *colx);
      *pidy=(colx[nextRow] - colx[1]);
    }

    // top-right corner
    fpDx[w1]=(fpSrc[w1]-fpSrc[w2]);
    fpDy[w1]=(fpSrc[width+w1]-fpSrc[w1]);

    // main loop (begin at coordinates (1,0)
    pidx = &fpDx[width];
    pidy = &fpDy[width];

    const float *const rowEnd = &fpSrc[last];

    for (rowy=&fpSrc[width];
         rowy<rowEnd;
         rowy+=width) {

      // left side
      *pidx=(rowy[1] - rowy[0]);
      *pidy=(rowy[width] - rowy[-width]);

      ++pidx;
      ++pidy;

      // middle
      const float *const colEnd = &rowy[w2];
      for (colx=rowy;
           colx<colEnd;
           ++colx,++pidx,++pidy) {
        *pidx=(colx[2] - *colx);
        *pidy=(colx[nextRow] - colx[lastRow]);
      }

      // right side
      *pidx=(colx[1] - *colx);
      *pidy=(colx[nextRow] - colx[lastRow]);

      ++pidx;
      ++pidy;
    }

    // bottom-left corner
    fpDx[last]=(fpSrc[last+1]-fpSrc[last]);
    fpDy[last]=(fpSrc[last]-fpSrc[last-width]);

    // bottom
    pidx = &fpDx[last+1];
    pidy = &fpDy[last+1];

    const float *const colEnd = &rowEnd[w2];
    for (colx=rowEnd;
         colx<colEnd;
         ++colx,++pidy,++pidx) {
      *pidx=(colx[2]-*colx);
      *pidy=(colx[1]-colx[lastRow]);
    }

    // bottom-right corner
    fpDx[last+w1]=(fpSrc[last+w1]-fpSrc[last+w2]);
    fpDy[last+w1]=(fpSrc[last+w1]-fpSrc[last-width+w1]);

    return true;
  }


  // IntePixel gradient calculation
  bool
  gradientFunctor::xyDifferentiateImageCartInterPixel(const channel& src,
                                                      channel& dx,
                                                      channel& dy) const {

    const int iWidth  = src.columns();
    const int iHeight = src.rows();

    int iX,iY;

    // main block
    for (iY=0; iY< src.lastRow(); iY++) {
      for (iX=0; iX< src.lastColumn(); iX++) {
        dx.at(iY,iX) = src.at(iY+1,iX+1) - src.at(iY,iX);
        dy.at(iY,iX) = src.at(iY+1,iX)   - src.at(iY,iX+1);
      }
    }

    // last column
    iX=iWidth-1;
    for (iY=0; iY < src.lastRow(); iY++){
      dy.at(iY,iX)=dx.at(iY,iX) = src.at(iY+1,iX)-src.at(iY,iX);
    }

    // last row
    iY=iHeight-1;
    for (iX=0; iX< src.lastColumn(); iX++){
      dx.at(iY,iX)=src.at(iY,iX+1)-src.at(iY,iX);
      dy.at(iY,iX)=src.at(iY,iX)  -src.at(iY,iX+1);
    }

    // the last pixel is always zero, due to the constant boundary
    dx.at(iY,iX) = 0.0f;
    dy.at(iY,iX) = 0.0f;

    return true;
  }

  /*
   * convert a pair of values in cartesic system into a pair in
   * polar system
   */
  inline void gradientFunctor::cartToPolar(const int dx,const int dy,
                                           float& mag,float& angle) const {
    static const float factor = 1.0f/255.0f;
    mag = factor*sqrt(float(dx*dx+dy*dy));
    angle = atan2_(dy,dx);
  }


  // extra fast direct polar  gradient with simple differences
  bool
  gradientFunctor::xyDifferentiateImage(const channel8& src,
                                        channel& gradAbs,    //out: Magnitude
                                        channel& gradPhi     //out: Orientation
                                        ) const {

    if (src.columns() < 3) {
      setStatusString("width less than 3");
      return false;
    }
    if (src.rows() < 3) {
      setStatusString("height less than 3");
      return false;
    }

    if (src.getMode()!=channel8::Connected) {
      setStatusString("src must be Connected");
      return false;
    }

    const int width  = src.columns();
    const int height = src.rows();

    float* fpGradAbs = &gradAbs.at(0,0);
    float* fpGradPhi = &gradPhi.at(0,0);

    const ubyte* fpSrc = &src.at(0,0);
    const ubyte* rowy;
    const ubyte* colx;

    float* pidx;
    float* pidy;

    const int w1 = width-1;
    const int w2 = width-2;
    const int last = (height-1)*width; // index of begin of last row
    const int lastRow = -w1;           // offset from actual column pointer to
                                       // last row
    const int nextRow = width+1;       // offset from actual column pointer to
                                       // next row

    // top-left corner
    cartToPolar(fpSrc[1]-fpSrc[0],fpSrc[width]-fpSrc[0],
                fpGradAbs[0],fpGradPhi[0]);

    // top
    pidx = &fpGradAbs[1];
    pidy = &fpGradPhi[1];

    for (colx=&fpSrc[0],rowy=&fpSrc[w1];
         colx<rowy;
         ++colx,++pidx,++pidy) {
      cartToPolar(colx[2] - *colx,colx[nextRow] - colx[1],*pidx,*pidy);
    }

    // top-right corner
    cartToPolar(fpSrc[w1]-fpSrc[w2],fpSrc[width+w1]-fpSrc[w1],
                fpGradAbs[w1],fpGradPhi[w1]);

    // main loop (begin at coordinates (1,0)
    pidx = &fpGradAbs[width];
    pidy = &fpGradPhi[width];

    const ubyte *const rowEnd = &fpSrc[last];

    for (rowy=&fpSrc[width];
         rowy<rowEnd;
         rowy+=width) {

      cartToPolar(rowy[1] - rowy[0],rowy[width] - rowy[-width],*pidx,*pidy);

      ++pidx;
      ++pidy;

      const ubyte *const colEnd = &rowy[w2];
      for (colx=rowy;
           colx<colEnd;
           ++colx,++pidx,++pidy) {
        cartToPolar(colx[2] - *colx,colx[nextRow] - colx[lastRow],*pidx,*pidy);
      }

      cartToPolar(colx[1] - *colx,colx[nextRow] - colx[lastRow],*pidx,*pidy);
      ++pidx;
      ++pidy;
    }

    // bottom-left corner
    cartToPolar(fpSrc[last+1]-fpSrc[last],fpSrc[last]-fpSrc[last-width],
                fpGradAbs[last],fpGradPhi[last]);

    // bottom
    pidx = &fpGradAbs[last+1];
    pidy = &fpGradPhi[last+1];

    const ubyte *const colEnd = &rowEnd[w2];
    for (colx=rowEnd;
         colx<colEnd;
         ++colx,++pidy,++pidx) {
      cartToPolar(colx[2]-*colx,colx[1]-colx[lastRow],*pidx,*pidy);
    }

    // bottom-right corner
    cartToPolar(fpSrc[last+w1]-fpSrc[last+w2],
                fpSrc[last+w1]-fpSrc[last-width+w1],
                fpGradAbs[last+w1],fpGradPhi[last+w1]);

    return true;
  }

#if 0
  // TODO: This needs to be optimized and called in the polar case of
  //       robinson kernel only
  // IntePixel gradient calculation
  bool
  gradientFunctor::xyDifferentiateImageInterPixel(const channel8& src,
                                                  channel& gradAbs,
                                                  channel& orientation) const {

    const int iWidth  = src.columns();
    const int iHeight = src.rows();

    int iX,iY;
    float fAbs,fPhi;
    static const float correctAngle = static_cast<float>(0.25*Pi);

    // main block
    for (iY=0; iY< src.lastRow(); iY++) {
      for (iX=0; iX< src.lastColumn(); iX++) {
        cartToPolar(src.at(iY+1,iX+1) - src.at(iY,iX),
                    -src.at(iY,iX+1)  + src.at(iY+1,iX),fAbs,fPhi);
        fPhi+=correctAngle;
        if (fPhi>constants<float>::TwoPi()) {
          fPhi-=constants<float>::TwoPi();
        }
        gradAbs.at(iY,iX)=fAbs;
        orientation.at(iY,iX)=fPhi;
      }
    }

    // last column
    for (iY=0; iY< iHeight-1; iY++) {
      iX=iWidth-1;
      cartToPolar(src.at(iY+1,iX)-src.at(iY,iX),
                  -src.at(iY,iX)+src.at(iY+1,iX),fAbs,fPhi);
      fPhi+=correctAngle;
      if (fPhi>constants<float>::TwoPi()) {
        fPhi+=constants<float>::TwoPi();
      }
      gradAbs.at(iY,iX)=fAbs;
      orientation.at(iY,iX)=fPhi;
    }

    // last row
    for (iX=0; iX< iWidth-1; iX++){
      iY=iHeight-1;
      cartToPolar(src.at(iY,iX+1)-src.at(iY,iX),
                  -src.at(iY,iX+1)+src.at(iY,iX),fAbs,fPhi);
      fPhi+=correctAngle;
      if (fPhi>constants<float>::TwoPi()) {
        fPhi+=constants<float>::TwoPi();
      }
      gradAbs.at(iY,iX)=fAbs;
      orientation.at(iY,iX)=fPhi;
    }
    gradAbs.at(iHeight-1,iWidth-1)=0.0;
    orientation.at(iHeight-1,iWidth-1)=0.0;

    return true;
  }
  #endif

  bool read(ioHandler& handler,gradientFunctor::eKernelType& data) {
    std::string str;
    if (handler.read(str)) {

      if (str.find("arri") != std::string::npos) {
        data = gradientFunctor::Harris;
      } else if (str.find("OGD") != std::string::npos) {
        data = gradientFunctor::OGD;
      } else if (str.find("ifferen") != std::string::npos) {
        data = gradientFunctor::Difference;
      } else if (str.find("obert") != std::string::npos) {
        data = gradientFunctor::Roberts;
      } else if (str.find("obel") != std::string::npos) {
        data = gradientFunctor::Sobel;
      } else if (str.find("rewitt") != std::string::npos) {
        data = gradientFunctor::Prewitt;
      } else if (str.find("obins") != std::string::npos) {
        data = gradientFunctor::Robinson;
      } else if (str.find("irsch") != std::string::npos) {
        data = gradientFunctor::Kirsch;
      } else {
        data = gradientFunctor::Ando;
      }
    }

    return true;
  }

  bool write(ioHandler& handler,
             const gradientFunctor::eKernelType& data) {
    bool b = false;
    switch(data) {
      case gradientFunctor::Ando:
        b = handler.write("Ando");
        break;
      case gradientFunctor::OGD:
        b = handler.write("OGD");
        break;
      case gradientFunctor::Difference:
        b = handler.write("Difference");
        break;
      case gradientFunctor::Roberts:
        b = handler.write("Roberts");
        break;
      case gradientFunctor::Sobel:
        b = handler.write("Sobel");
        break;
      case gradientFunctor::Prewitt:
        b = handler.write("Prewitt");
        break;
      case gradientFunctor::Robinson:
        b = handler.write("Robinson");
        break;
      case gradientFunctor::Kirsch:
        b = handler.write("kirsch");
        break;
      case gradientFunctor::Harris:
        b = handler.write("Harris");
        break;
      default:
        b=false;
        handler.setStatusString("undefined eExtremaType");
        handler.write("Unknown");
    }
    return b;
  }

  bool read(ioHandler& handler,gradientFunctor::eOutputFormat& data) {
    std::string str;
    if (handler.read(str)) {

      if (str.find("Po") != std::string::npos) {
        data = gradientFunctor::Polar;
      } else {
        data = gradientFunctor::Cartesian;
      }
    }

    return true;
  }

  bool write(ioHandler& handler,
             const gradientFunctor::eOutputFormat& data) {
    bool b = false;
    switch(data) {
      case gradientFunctor::Polar:
        b = handler.write("Polar");
        break;
      case gradientFunctor::Cartesian:
        b = handler.write("Cartesian");
        break;
      default:
        b=false;
        handler.setStatusString("undefined eExtremaType");
        handler.write("Unknown");
    }
    return b;
  }

}

