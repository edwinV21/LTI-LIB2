/*
 * Copyright (C) 2002, 2003, 2004, 2005, 2006
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
 * \file   ltiClassifier2DVisualizer.h
 * \author Jochen Wickel
 * \date   15.5.2002
 *
 * $Id: ltiClassifier2DVisualizer.cpp,v 1.1 2008-04-04 22:57:47 alvarado Exp $
 */

#include "ltiClassifier2DVisualizer.h"
#include "ltiDraw.h"
#include "ltiMergeRGBToImage.h"
#include "ltiChannel.h"
#include <limits>

#undef _LTI_DEBUG
//#define _LTI_DEBUG 1
#include "ltiDebug.h"

namespace lti {
  // --------------------------------------------------
  // classifier2DVisualizer::parameters
  // --------------------------------------------------

  // class colors for visualization
  const rgbaPixel classifier2DVisualizer::parameters::defaultColors[]={
    // primary and secondary colors
    Red, Green, Blue, Yellow, Magenta, Cyan,
    // other colors
    DarkOrange, Fusia, BrightGreen, LawnGreen, LightBlue,
    DarkViolet,
    // grey levels
    Grey75, Grey50, Grey25
  };

  // default constructor
  classifier2DVisualizer::parameters::parameters()
    : functor::parameters() {

    lowerLeft=dpoint(0,0);
    upperRight=dpoint(1.3333,1);
    imgSize=ipoint(1024,768);
    pixSize=2;
    highlightSize=6;
    showBoundaries = false;
    boundaryColor = Grey75;
    highlightColor = White;
    highlightColor2 = Black;
    colorOffset=0;
    colorMap=palette(sizeof(defaultColors)/sizeof(rgbaPixel),defaultColors);
    scaleRGBTogether = false;
  }

  // copy constructor
  classifier2DVisualizer::parameters::parameters(const parameters& other)
    : functor::parameters()  {
    copy(other);
  }

  // destructor
  classifier2DVisualizer::parameters::~parameters() {
  }

  // get type name
  const std::string& classifier2DVisualizer::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // copy member

  classifier2DVisualizer::parameters&
    classifier2DVisualizer::parameters::copy(const parameters& other) {
    functor::parameters::copy(other);

    lowerLeft=other.lowerLeft;
    upperRight=other.upperRight;
    imgSize=other.imgSize;
    pixSize=other.pixSize;
    highlightSize=other.highlightSize;
    showBoundaries = other.showBoundaries;
    boundaryColor = other.boundaryColor;
    highlightColor = other.highlightColor;
    highlightColor2 = other.highlightColor2;
    colorOffset=other.colorOffset;
    colorMap=other.colorMap;
    scaleRGBTogether=other.scaleRGBTogether;
    return *this;
  }

  // alias for copy member
  classifier2DVisualizer::parameters&
    classifier2DVisualizer::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  // clone member
  classifier2DVisualizer::parameters*
  classifier2DVisualizer::parameters::clone() const {
    return new parameters(*this);
  }

  // clone member
  classifier2DVisualizer::parameters*
  classifier2DVisualizer::parameters::newInstance() const {
    return new parameters;
  }

  /*
   * write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool classifier2DVisualizer::parameters::write(ioHandler& handler,
                                         const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {

      lti::write(handler,"lowerLeft",lowerLeft);
      lti::write(handler,"upperRight",upperRight);
      lti::write(handler,"imgSize",imgSize);
      lti::write(handler,"pixSize",pixSize);
      lti::write(handler,"highlightSize",highlightSize);
      lti::write(handler,"showBoundaries",showBoundaries);
      lti::write(handler,"boundaryColor",boundaryColor);
      lti::write(handler,"highlightColor",highlightColor);
      lti::write(handler,"highlightColor2",highlightColor2);
      lti::write(handler,"colorOffset",colorOffset);
      lti::write(handler,"colorMap",colorMap);
      lti::write(handler,"scaleRGBTogether",scaleRGBTogether);
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
  bool classifier2DVisualizer::parameters::read(ioHandler& handler,
                                        const bool complete) {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {

      lti::read(handler,"lowerLeft",lowerLeft);
      lti::read(handler,"upperRight",upperRight);
      lti::read(handler,"imgSize",imgSize);
      lti::read(handler,"pixSize",pixSize);
      lti::read(handler,"highlightSize",highlightSize);
      lti::read(handler,"showBoundaries",showBoundaries);
      lti::read(handler,"boundaryColor",boundaryColor);
      lti::read(handler,"highlightColor",highlightColor);
      lti::read(handler,"highlightColor2",highlightColor2);
      lti::read(handler,"colorOffset",colorOffset);
      lti::read(handler,"colorMap",colorMap);
      lti::read(handler,"scaleRGBTogether",scaleRGBTogether);
    }

    b = b && functor::parameters::read(handler,false);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

  // --------------------------------------------------
  // classifier2DVisualizer
  // --------------------------------------------------

  // default constructor
  classifier2DVisualizer::classifier2DVisualizer()
    : functor() {

    // create an instance of the parameters with the default values
    parameters defaultParameters;
    // set the default parameters
    setParameters(defaultParameters);
  }

  // default constructor
  classifier2DVisualizer::classifier2DVisualizer(const parameters& defPar)
    : functor() {

    // set the default parameters
    setParameters(defPar);
  }

  // copy constructor
  classifier2DVisualizer::classifier2DVisualizer(const classifier2DVisualizer& other)
    : functor()  {
    copy(other);
  }

  // destructor
  classifier2DVisualizer::~classifier2DVisualizer() {
  }

  // returns the name of this type
  const std::string& classifier2DVisualizer::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // copy member
  classifier2DVisualizer&
  classifier2DVisualizer::copy(const classifier2DVisualizer& other) {
    functor::copy(other);

    // Copy members, if any

    return (*this);
  }

  // alias for copy member
  classifier2DVisualizer&
    classifier2DVisualizer::operator=(const classifier2DVisualizer& other) {
    return (copy(other));
  }


  // clone member
  functor* classifier2DVisualizer::clone() const {
    return new classifier2DVisualizer(*this);
  }

  functor* classifier2DVisualizer::newInstance() const {
    return new classifier2DVisualizer;
  }

  // return parameters
  const classifier2DVisualizer::parameters&
    classifier2DVisualizer::getParameters() const {
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
   * Applies the given classifier to the area defined in the
   * parameters object. The image will contain the visualization
   * result.
   */
  bool classifier2DVisualizer::apply(const supervisedInstanceClassifier& cls,
                                     image& result) const {
    const parameters& p=getParameters();

    channel red(p.imgSize);
    channel green(p.imgSize);
    channel blue(p.imgSize);

    mergeRGBToImage merger;

    const int height=p.imgSize.y;
    const int width=p.imgSize.x;

    const double xlow=p.lowerLeft.x;
    const double ylow=p.lowerLeft.y;
    const double xspan=p.upperRight.x-xlow;
    const double yspan=p.upperRight.y-ylow;

    // now determine the colors
    int* cid=new int[height];

    float maxr=0;
    float maxg=0;
    float maxb=0;

    float br=float(p.boundaryColor.getRed())/255;
    float bg=float(p.boundaryColor.getGreen())/255;
    float bb=float(p.boundaryColor.getBlue())/255;

    for (int y=0; y<height; y++) {
      cid[y]=0;
    }

    int pixOffset=p.pixSize/2;

    // now determine class membership for each image pixel
    for (int x=pixOffset; x<width; x+=p.pixSize) {
      _lti_debug("  " << x << "\r");
      for (int y=pixOffset; y<height; y+=p.pixSize) {
        dvector tmp(2);
        tmp[0]=xlow+xspan*double(x)/double(width);
        tmp[1]=ylow+yspan*double(height-y)/double(height);
        classifier::result o;

        cls.classify(tmp,o);

        double r=0,g=0,b=0;

        // compute color mix
        double maxcp=0;
        int newcid=0;
        // compute color mix
        for (int i=0; i<o.size(); i++) {
          int id;
          o.getId(i,id);
          double prob;
          o.getValueById(id,prob);
          if (prob > 1) {
            std::cerr << "WARNING: x=" << x << ", y=" << y;
            std::cerr << ", v[" << i << "]=" << prob << "\n";
          }
          if (prob > maxcp) {
            maxcp=prob;
            newcid=i;
          }
          r+=prob*double(p.getColor(id).getRed());
          g+=prob*double(p.getColor(id).getGreen());
          b+=prob*double(p.getColor(id).getBlue());
        }
        if (r > maxr) {
          maxr=static_cast<float>(r);
        }
        if (g > maxg) {
          maxg=static_cast<float>(g);
        }
        if (b > maxb) {
          maxb=static_cast<float>(b);
        }
        //cerr << "\n";
        // cid[y-1] is the class id of the pixel above
        // cid[y] is the class id of the pixel to the left
        if (p.showBoundaries) {
          if ((y > pixOffset && newcid != cid[y-p.pixSize]) ||
              (x > pixOffset && newcid != cid[y])) {
            r=-1;//red[y+pixSize/2][x+pixSize/2]=-1;
            g=-1;//green[y+pixSize/2][x+pixSize/2]=-1;
            b=-1;//blue[y+pixSize/2][x+pixSize/2]=-1;
          }
        }

        for (int tx=x-pixOffset; tx<lti::min(x+p.pixSize,width); tx++) {
          for (int ty=y-pixOffset; ty<lti::min(y+p.pixSize,height); ty++) {
            red[ty][tx]=static_cast<float>(r);
            green[ty][tx]=static_cast<float>(g);
            blue[ty][tx]=static_cast<float>(b);
          }
        }
        cid[y]=newcid;
      }
    }

    if (p.scaleRGBTogether) {
      maxr = maxg = maxb = max(maxr,maxg,maxb);
    }

    // scale channels to 0..1
    _lti_debug("Red Max = " << maxr << "\n");
    if (maxr > std::numeric_limits<float>::epsilon()) {
      red.multiply(1.0f/maxr);
    }
    _lti_debug("Green Max = " << maxg << "\n");
    if (maxg > std::numeric_limits<float>::epsilon()) {
      green.multiply(1.0f/maxg);
    }
    _lti_debug("Blue Max = " << maxb << "\n");
    if (maxb > std::numeric_limits<float>::epsilon()) {
      blue.multiply(1.0f/maxb);
    }

    // re-paint boundaries
    if (p.showBoundaries) {
      channel::iterator ci;
      for (ci=red.begin(); ci != red.end(); ci++) {
        if (*ci < 0) {
          *ci=br;
        }
      }
      for (ci=green.begin(); ci != green.end(); ci++) {
        if (*ci < 0) {
          *ci=bg;
        }
      }
      for (ci=blue.begin(); ci != blue.end(); ci++) {
        if (*ci < 0) {
          *ci=bb;
        }
      }
    }

    // create image
    merger.apply(red,green,blue,result);
    //addCopyright(*panel);
    delete[] cid;
    return true;
  }

  /*
   * Applies the given classifier to the area defined in the
   * parameters object. The image will contain the visualization
   * result.
   */
  bool classifier2DVisualizer::apply(const supervisedInstanceClassifier& cls,
                                     const dmatrix& highlights,
                                     image& result) const {
    ivector tmpa;
    genericVector<bool> tmpb;
    return apply(cls,highlights,result,tmpa,tmpb);
  }

  /*
   * Applies the given classifier to the area defined in the
   * parameters object. The image will contain the visualization
   * result.
   */
  bool classifier2DVisualizer::apply(const supervisedInstanceClassifier& cls,
                                     const dmatrix& highlights,
                                     image& result,
                                     const ivector& labels) const {
    genericVector<bool> tmpb;
    return apply(cls,highlights,result,labels,tmpb);
  }

  /*
   * Applies the given classifier to the area defined in the
   * parameters object. The image will contain the visualization
   * result.
   */
  bool classifier2DVisualizer::apply(const supervisedInstanceClassifier& cls,
                                     const dmatrix& highlights,
                                     image& result,
                                     const ivector& labels,
                                     const genericVector<bool>& htype) const {
    const parameters& p=getParameters();

    draw<rgbaPixel> g2;

    const int height=p.imgSize.y;
    const int width=p.imgSize.x;

    const double xlow=p.lowerLeft.x;
    const double ylow=p.lowerLeft.y;
    const double xspan=p.upperRight.x-xlow;
    const double yspan=p.upperRight.y-ylow;
    const double yhigh=p.upperRight.y;

    bool flag=apply(cls,result);
    bool considerLabels=(labels.size() == highlights.rows());
    bool considerType=(htype.size() == highlights.rows());
    irectangle rect2(0,0,p.highlightSize-3,p.highlightSize-3);
    irectangle rect(0,0,p.highlightSize-1,p.highlightSize-1);

    g2.use(result);

    // plot vectors
    for (int i=0; i<highlights.rows(); i++) {

      int xc=int((highlights[i][0]-xlow)/
                 xspan*double(width-1))+(p.pixSize-1)/2;
      int yc=int((yhigh-highlights[i][1])/
                 yspan*double(height-1))+(p.pixSize-1)/2;
      // get this value
      if (xc >= 0 && xc < width && yc >= 0 && yc < height) {
        ipoint supc=ipoint(xc,yc);

        rect.setCenter(supc);
        // determine color of outer rectangle
        if (considerType && htype[i]) {
          g2.setColor(p.highlightColor2);
        } else {
          g2.setColor(p.highlightColor);
        }
        if (considerLabels) {
          g2.rectangle(rect,true);
          // set class color
          g2.setColor(p.getColor(labels.at(i)));
          rect2.setCenter(supc);
          // draw a filled rectangle in the class color
          g2.rectangle(rect2,true);
        } else {
          g2.rectangle(rect,false);
        }
      }
    }
    return flag;
  }
}
