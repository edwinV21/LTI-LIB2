/*
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
 * \file   ltiDistanceTransform.cpp
 *         Contains the class distanceTransform,
 *         which has still to be documented by Pablo Alvarado.
 * 
 * \author Pablo Alvarado
 * \date   09.03.2008
 *
 * revisions ..: $Id: ltiDistanceTransform.cpp,v 1.1 2008-03-09 15:23:53 alvarado Exp $
 */

//TODO: include files
#include "ltiDistanceTransform.h"

namespace lti {
  // --------------------------------------------------
  // distanceTransform::parameters
  // --------------------------------------------------

  // default constructor
  distanceTransform::parameters::parameters()
    : functor::parameters() {
    distance = Euclidean;
  }

  // copy constructor
  distanceTransform::parameters::parameters(const parameters& other)
    : functor::parameters() {
    copy(other);
  }

  // destructor
  distanceTransform::parameters::~parameters() {
  }

  // copy member

  distanceTransform::parameters&
  distanceTransform::parameters::copy(const parameters& other) {
    functor::parameters::copy(other);
    
    distance = other.distance;

    return *this;
  }

  // alias for copy method
  distanceTransform::parameters&
  distanceTransform::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  // class name
  const std::string& distanceTransform::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone method
  distanceTransform::parameters*
  distanceTransform::parameters::clone() const {
    return new parameters(*this);
  }

  // new instance
  distanceTransform::parameters*
  distanceTransform::parameters::newInstance() const {
    return new parameters();
  }

  /*
   * Write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool distanceTransform::parameters::write(ioHandler& handler,
                                            const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {
      
      lti::write(handler,"distance",distance);
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
  bool distanceTransform::parameters::read(ioHandler& handler,
                                           const bool complete)  {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {
      
      lti::read(handler,"distance",distance);
    }

    b = b && functor::parameters::read(handler,false);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

  // --------------------------------------------------
  // distanceTransform - protected inline methods
  // --------------------------------------------------
   
  inline void distanceTransform::EDT_2D(fmatrix& chnl) const {
    //voronoiEDT_2D must be called for every column
    for(int x = 0; x < chnl.columns(); ++x){
      voronoiEDT_2D(chnl, x);
    }
  }

  inline bool distanceTransform::removeEDT(const int du, 
                                           const int dv, 
                                           const int dw, 
                                           const int u, 
                                           const int v, 
                                           const int w) const {
    //11 integer expressions
    const int a = v - u;
    const int b = w - v;
    const int c = w - u;
    return ( (c * dv - b * du - a * dw) > (a * b * c) );
  }

  inline void distanceTransform::sedMask::queryDistance(ipoint &shortest, 
                                                        ipoint &other)const{
    if((other.x >= 0) &&
       ((shortest.x < 0) || (shortest.absSqr() > other.absSqr()))) {
      shortest = other;
    }
  }


  // --------------------------------------------------
  // distanceTransform
  // --------------------------------------------------

  // default constructor
  distanceTransform::distanceTransform()
    : functor() {

    // create an instance of the parameters with the default values
    parameters defaultParameters;
    // set the default parameters
    setParameters(defaultParameters);
  }

  // default constructor
  distanceTransform::distanceTransform(const parameters& par)
    : functor() {

    // set the given parameters
    setParameters(par);
  }


  // copy constructor
  distanceTransform::distanceTransform(const distanceTransform& other)
    : functor() {
    copy(other);
  }

  // destructor
  distanceTransform::~distanceTransform() {
  }

  // copy member
  distanceTransform& 
  distanceTransform::copy(const distanceTransform& other) {
    functor::copy(other);

    //TODO: comment the attributes of your functor
    // If you add more attributes manually, do not forget to do following:
    // 1. indicate in the default constructor the default values
    // 2. make sure that the copy member also copy your new attributes, or
    //    to ensure there, that these attributes are properly initialized.

    return (*this);
  }

  // alias for copy member
  distanceTransform&
  distanceTransform::operator=(const distanceTransform& other) {
    return (copy(other));
  }

  // class name
  const std::string& distanceTransform::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone member
  distanceTransform* distanceTransform::clone() const {
    return new distanceTransform(*this);
  }

  // create a new instance
  distanceTransform* distanceTransform::newInstance() const {
    return new distanceTransform();
  }

  // return parameters
  const distanceTransform::parameters&
  distanceTransform::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }

  // -------------------------------------------------------------------
  // The apply() member functions
  // -------------------------------------------------------------------

  // On place apply for type matrix<ubyte>!
  bool distanceTransform::apply(matrix<ubyte>& srcdest) const {
    fmatrix tmp;
    tmp.castFrom(static_cast<matrix<ubyte> >(srcdest));
    if (apply(tmp)) {
      srcdest.castFrom(static_cast<matrix<float> >(tmp));
      return true;
    }

    return false;
  }

  // On copy apply for type matrix<ubyte>!
  bool distanceTransform::apply(const matrix<ubyte>& src,
                                matrix<ubyte>& dest) const {
    fmatrix tmp;
    tmp.castFrom(static_cast<matrix<ubyte> >(src));
    if (apply(tmp)) {
      dest.castFrom(static_cast<matrix<float> >(tmp));
      return true;
    }
    return false;
  }


  // On place apply for type matrix<ubyte>!
  bool distanceTransform::apply(fmatrix& srcdest) const {

    if ((srcdest.rows() < 2) || (srcdest.columns() < 2)) {
      setStatusString("At least 2 pixels at each axis expected");
      return false;
    }

    const parameters& param = getParameters();

    if(   param.distance == EightNeighborhood
       || param.distance == FourNeighborhood){
      // ensure that the non-zero values are maximal
      int y;
      vector<fmatrix::value_type>::iterator it,eit;
      const float max = static_cast<float>(srcdest.rows()+srcdest.columns());
      for (y=0;y<srcdest.rows();y++) {
        vector<fmatrix::value_type>& vct = srcdest.getRow(y);
        for (it=vct.begin(),eit=vct.end();it!=eit;++it) {
          if ((*it)>0.0f) {
            (*it)=max;
          }
        }
      }
    }

    switch(param.distance){
    case EightNeighborhood:
      
      iteration8back(srcdest);
      iteration8(srcdest);
      return true;
      break;

    case FourNeighborhood:
      
      iteration4back(srcdest);
      iteration4(srcdest);
      return true;
      break;

    case Euclidean:
      
      EDT_1D(srcdest);
      EDT_2D(srcdest);
      srcdest.apply(sqrt);
      return true;
      break;

    case EuclideanSqr:

      EDT_1D(srcdest);
      EDT_2D(srcdest);
      return true;
      break;

    case EightSED:
    
      sedFiltering(srcdest, true);
      srcdest.apply(sqrt);
      return true;
      break;
    
    case EightSEDSqr:

      sedFiltering(srcdest,true);
      return true;
      break;

    case FourSED:

      sedFiltering(srcdest, false);
      srcdest.apply(sqrt);
      return true;
      break;

    case FourSEDSqr:

      sedFiltering(srcdest, false);
      return true;
      break;

    default:
      return false;
    }
  }

  // On copy apply for type matrix<ubyte>!
  bool distanceTransform::apply(const fmatrix& src,fmatrix& dest) const {

    dest.copy(src);
    return apply(dest);

  }

  void distanceTransform::iteration8(fmatrix& chnl) const {
    int x,y,z;

    const int rowm1 = chnl.rows()-1;
    const int colm1 = chnl.columns()-1;

    static const int deltax[12] = {1,1,0,-1,-1,-1, 0, 1,1,1,0,-1};
    static const int deltay[12] = {0,1,1, 1, 0,-1,-1,-1,0,1,1, 1};

    float minimum;

    // upper-left
    if (chnl.at(0,0) > 0) {
      chnl.at(0,0) = 1.0f+min(chnl.at(0,1),chnl.at(1,1),chnl.at(1,0));
    }

    // top
    y = 0;
    for (x=1;x<colm1;++x) {
      if (chnl.at(y,x) > 0) {
        // valid pixel, let's check for the distance value
        minimum = chnl.at(y+deltay[0],x+deltax[0]);

        for (z=1;z<5;++z) {
          minimum = min(minimum,chnl.at(y+deltay[z],x+deltax[z]));
        }

        chnl.at(y,x) = minimum+1.0f;
      }
    }

    // upper-right
    if (chnl.at(0,colm1) > 0) {
      chnl.at(0,colm1) = 1.0f+min(chnl.at(0,colm1-1),chnl.at(1,colm1-1),
                                  chnl.at(1,colm1));
    }

    // inner of the image only...
    for (y=1;y<rowm1;++y) {
      // left border
      x = 0;
      if (chnl.at(y,x) > 0) {
        minimum = chnl.at(y+deltay[6],x+deltax[6]);

        for (z=7;z<11;++z) {
          minimum = min(minimum,chnl.at(y+deltay[z],x+deltax[z]));
        }

        chnl.at(y,x) = minimum+1.0f;
      }

      // inner of the line
      for (x=1;x<colm1;++x) {
        if (chnl.at(y,x) > 0) {
          // valid pixel, let's check for the distance value
          minimum = chnl.at(y+deltay[0],x+deltax[0]);

          for (z=1;z<8;++z) {
            minimum = min(minimum,chnl.at(y+deltay[z],x+deltax[z]));
          }

          chnl.at(y,x) = minimum+1.0f;
        }
      }

      // right border
      if (chnl.at(y,x) > 0) {
        minimum = chnl.at(y+deltay[2],x+deltax[2]);

        for (z=3;z<7;++z) {
          minimum = min(minimum,chnl.at(y+deltay[z],x+deltax[z]));
        }

        chnl.at(y,x) = minimum+1.0f;
      }
    }

    // bottom-left
    if (chnl.at(rowm1,0) > 0) {
      chnl.at(rowm1,0) = 1.0f+min(chnl.at(rowm1,1),chnl.at(rowm1-1,1),
                                  chnl.at(rowm1-1,0));
    }

    // bottom
    for (x=1;x<colm1;++x) {
      if (chnl.at(y,x) > 0) {
        // valid pixel, let's check for the distance value
        minimum = chnl.at(y+deltay[4],x+deltax[4]);

        for (z=5;z<9;++z) {
          minimum = min(minimum,chnl.at(y+deltay[z],x+deltax[z]));
        }

        chnl.at(y,x) = minimum+1.0f;
      }
    }

    // bottom-right
    if (chnl.at(rowm1,colm1) > 0) {
      chnl.at(rowm1,colm1) = 1.0f+min(chnl.at(rowm1,colm1-1),
                                      chnl.at(rowm1-1,colm1-1),
                                      chnl.at(rowm1-1,colm1));
    }

  }

  // iteration for 4-neighborhood

  void distanceTransform::iteration4(fmatrix& chnl) const {
    int x,y,z;

    const int rowm1 = chnl.rows()-1;
    const int colm1 = chnl.columns()-1;

    static const int deltax[6] = {1,0,-1, 0, 1,0};
    static const int deltay[6] = {0,1, 0,-1, 0,1};

    float minimum;

    // upper-left
    if (chnl.at(0,0) > 0) {
      chnl.at(0,0) = 1.0f+min(chnl.at(0,1),chnl.at(1,0));
    }

    // top
    y = 0;
    for (x=1;x<colm1;++x) {
      if (chnl.at(y,x) > 0) {
        // valid pixel, let's check for the distance value
        minimum = chnl.at(y+deltay[0],x+deltax[0]);

        for (z=1;z<3;++z) {
          minimum = min(minimum,chnl.at(y+deltay[z],x+deltax[z]));
        }

        chnl.at(y,x) = minimum+1.0f;
      }
    }

    // upper-right
    if (chnl.at(0,colm1) > 0) {
      chnl.at(0,colm1) = 1.0f+min(chnl.at(0,colm1-1),chnl.at(1,colm1));
    }

    // inner of the image only...
    for (y=1;y<rowm1;++y) {
      // left border
      x = 0;
      if (chnl.at(y,x) > 0) {
        minimum = chnl.at(y+deltay[3],x+deltax[3]);

        for (z=0;z<2;++z) {
          minimum = min(minimum,chnl.at(y+deltay[z],x+deltax[z]));
        }

        chnl.at(y,x) = minimum+1.0f;
      }

      // inner of the line
      for (x=1;x<colm1;++x) {
        if (chnl.at(y,x) > 0) {
          // valid pixel, let's check for the distance value
          minimum = chnl.at(y+deltay[0],x+deltax[0]);

          for (z=1;z<4;++z) {
            minimum = min(minimum,chnl.at(y+deltay[z],x+deltax[z]));
          }

          chnl.at(y,x) = minimum+1.0f;
        }
      }

      // right border
      if (chnl.at(y,x) > 0) {
        minimum = chnl.at(y+deltay[1],x+deltax[1]);

        for (z=2;z<4;++z) {
          minimum = min(minimum,chnl.at(y+deltay[z],x+deltax[z]));
        }

        chnl.at(y,x) = minimum+1.0f;
      }
    }

    // bottom-left
    if (chnl.at(rowm1,0) > 0) {
      chnl.at(rowm1,0) = 1.0f+min(chnl.at(rowm1,1),chnl.at(rowm1-1,0));
    }

    // bottom
    for (x=1;x<colm1;++x) {
      if (chnl.at(y,x) > 0) {
        // valid pixel, let's check for the distance value
        minimum = chnl.at(y+deltay[2],x+deltax[2]);

        for (z=3;z<5;++z) {
          minimum = min(minimum,chnl.at(y+deltay[z],x+deltax[z]));
        }

        chnl.at(y,x) = minimum+1.0f;
      }
    }

    // bottom-right
    if (chnl.at(rowm1,colm1) > 0) {
      chnl.at(rowm1,colm1) = 1.0f+min(chnl.at(rowm1,colm1-1),
                                      chnl.at(rowm1-1,colm1));
    }
  }

  void distanceTransform::iteration8back(fmatrix& chnl) const {
    int x,y,z;

    const int rowm1 = chnl.lastRow();
    const int colm1 = chnl.lastColumn();

    static const int deltax[12] = {1,1,0,-1,-1,-1, 0, 1,1,1,0,-1};
    static const int deltay[12] = {0,1,1, 1, 0,-1,-1,-1,0,1,1, 1};

    float minimum;

    // bottom-right
    if (chnl.at(rowm1,colm1) > 0) {
      chnl.at(rowm1,colm1) = 1.0f+min(chnl.at(rowm1,colm1-1),
                                      chnl.at(rowm1-1,colm1-1),
                                      chnl.at(rowm1-1,colm1));
    }
    // bottom
    y = rowm1;
    for (x=colm1-1;x>0;--x) {
      if (chnl.at(y,x) > 0) {
        // valid pixel, let's check for the distance value
        minimum = chnl.at(y+deltay[4],x+deltax[4]);

        for (z=5;z<9;++z) {
          minimum = min(minimum,chnl.at(y+deltay[z],x+deltax[z]));
        }

        chnl.at(y,x) = minimum+1.0f;
      }
    }
    // bottom-left
    if (chnl.at(rowm1,0) > 0) {
      chnl.at(rowm1,0) = 1.0f+min(chnl.at(rowm1,1),chnl.at(rowm1-1,1),
                                  chnl.at(rowm1-1,0));
    }

    // inner of the image only...
    for (y=rowm1-1;y>0;--y) {
      x = colm1;
      // right border
      if (chnl.at(y,x) > 0) {
        minimum = chnl.at(y+deltay[2],x+deltax[2]);

        for (z=3;z<7;++z) {
          minimum = min(minimum,chnl.at(y+deltay[z],x+deltax[z]));
        }

        chnl.at(y,x) = minimum+1.0f;
      }

      // inner of the line
      for (x=colm1-1;x>0;--x) {
        if (chnl.at(y,x) > 0) {
          // valid pixel, let's check for the distance value
          minimum = chnl.at(y+deltay[0],x+deltax[0]);

          for (z=1;z<8;++z) {
            minimum = min(minimum,chnl.at(y+deltay[z],x+deltax[z]));
          }

          chnl.at(y,x) = minimum+1.0f;
        }
      }

      // left border
      if (chnl.at(y,x) > 0) {
        minimum = chnl.at(y+deltay[6],x+deltax[6]);

        for (z=7;z<11;++z) {
          minimum = min(minimum,chnl.at(y+deltay[z],x+deltax[z]));
        }

        chnl.at(y,x) = minimum+1.0f;
      }
    }

    // upper-right
    if (chnl.at(0,colm1) > 0) {
      chnl.at(0,colm1) = 1.0f+min(chnl.at(0,colm1-1),chnl.at(1,colm1-1),
                                  chnl.at(1,colm1));
    }

    // top
    for (x=colm1-1;x>0;--x) {
      if (chnl.at(y,x) > 0) {
        // valid pixel, let's check for the distance value
        minimum = chnl.at(y+deltay[0],x+deltax[0]);

        for (z=1;z<5;++z) {
          minimum = min(minimum,chnl.at(y+deltay[z],x+deltax[z]));
        }

        chnl.at(y,x) = minimum+1.0f;
      }
    }

    // upper-left
    if (chnl.at(0,0) > 0) {
      chnl.at(0,0) = 1.0f+min(chnl.at(0,1),chnl.at(1,1),chnl.at(1,0));
    }

  }

  void distanceTransform::iteration4back(fmatrix& chnl) const {
    int x,y,z;

    const int rowm1 = chnl.lastRow();
    const int colm1 = chnl.lastColumn();

    static const int deltax[6] = {1,0,-1, 0, 1,0};
    static const int deltay[6] = {0,1, 0,-1, 0,1};

    float minimum;

    // bottom-right
    if (chnl.at(rowm1,colm1) > 0) {
      chnl.at(rowm1,colm1) = 1.0f+min(chnl.at(rowm1,colm1-1),
                                      chnl.at(rowm1-1,colm1));
    }

    // bottom
    y = rowm1;
    for (x=colm1-1;x>0;--x) {
      if (chnl.at(y,x) > 0) {
        // valid pixel, let's check for the distance value
        minimum = chnl.at(y+deltay[2],x+deltax[2]);

        for (z=3;z<5;++z) {
          minimum = min(minimum,chnl.at(y+deltay[z],x+deltax[z]));
        }

        chnl.at(y,x) = minimum+1.0f;
      }
    }

    // bottom-left
    if (chnl.at(rowm1,0) > 0) {
      chnl.at(rowm1,0) = 1.0f+min(chnl.at(rowm1,1),
                                  chnl.at(rowm1-1,0));
    }

    // inner of the image only...
    for (y=rowm1-1;y>0;--y) {
      x = colm1;
      // right border
      if (chnl.at(y,x) > 0) {
        minimum = chnl.at(y+deltay[1],x+deltax[1]);

        for (z=2;z<4;++z) {
          minimum = min(minimum,chnl.at(y+deltay[z],x+deltax[z]));
        }

        chnl.at(y,x) = minimum+1.0f;
      }

      // inner of the line
      for (x=colm1-1;x>0;--x) {
        if (chnl.at(y,x) > 0) {
          // valid pixel, let's check for the distance value
          minimum = chnl.at(y+deltay[0],x+deltax[0]);

          for (z=1;z<4;++z) {
            minimum = min(minimum,chnl.at(y+deltay[z],x+deltax[z]));
          }

          chnl.at(y,x) = minimum+1.0f;
        }
      }

      // left border
      if (chnl.at(y,x) > 0) {
        minimum = chnl.at(y+deltay[3],x+deltax[3]);

        for (z=0;z<2;++z) {
          minimum = min(minimum,chnl.at(y+deltay[z],x+deltax[z]));
        }

        chnl.at(y,x) = minimum+1.0f;
      }
    }

    // upper-right
    if (chnl.at(0,colm1) > 0) {
      chnl.at(0,colm1) = 1.0f+min(chnl.at(0,colm1-1),
                                  chnl.at(1,colm1));
    }

    // top
    for (x=colm1-1;x>0;--x) {
      if (chnl.at(y,x) > 0) {
        // valid pixel, let's check for the distance value
        minimum = chnl.at(y+deltay[0],x+deltax[0]);

        for (z=1;z<3;++z) {
          minimum = min(minimum,chnl.at(y+deltay[z],x+deltax[z]));
        }

        chnl.at(y,x) = minimum+1.0f;
      }
    }

    // upper-left
    if (chnl.at(0,0) > 0) {
      chnl.at(0,0) = 1.0f+min(chnl.at(0,1),chnl.at(1,0));
    }

  }

  void distanceTransform::voronoiEDT_2D(fmatrix& chnl, const int j) const {
    int l = -1,
        fi;
    vector<int> g(chnl.rows()),
                h(chnl.rows());
    int x0 = j,
        x1;
  
    for(x1 = 0; x1 < chnl.rows(); ++x1){
      fi = static_cast<int>(chnl.at(x1, x0));
      if(fi >= 0.0f){  //any value below zero is undefined
        while(   l >= 1 
              && removeEDT(g.at(l - 1), g.at(l), fi, h.at(l - 1), h.at(l), x1))
          --l;
        ++l; g.at(l) = fi; h.at(l) = x1;
      }
    }
    if(l == -1) return;
    int ns = l;
    l = 0;
    for(x1 = 0; x1 < chnl.rows(); ++x1){
      int tmp0 = h.at(l) - x1,
          tmp1 = g.at(l) + tmp0 * tmp0,
          tmp2;
    
      while(true){
        if(l < ns){
          tmp2 = (h.at(l + 1) - x1);
          if(tmp1 > g.at(l + 1) + tmp2 * tmp2){
            ++l;
            tmp0 = h.at(l) - x1;
            tmp1 = g.at(l) + tmp0 * tmp0;
          }else break;
        }else break;
      }
        
      chnl.at(x1, x0) = static_cast<float>(tmp1);
    }
  }

  void distanceTransform::EDT_1D(fmatrix& chnl) const {

    const float undef = -1.0f;  //means any undefined value (distance or pos)
    
    //remember: all foreground pixel are >  0.0f
    //          all background pixel are == 0.0f
    for(int y = 0; y < chnl.rows(); ++y){
      int x, pos = static_cast<int>(undef);
      //first step: forward propagation
      for(x = 0; x < chnl.columns(); ++x){
        if(chnl.at(y, x) == 0.0f){
          //found background pixel
          //now 0.0 means distance to closest background pixel
          pos = x; 
        }
        else if(pos >= 0){
          int tmp = pos - x;
          chnl.at(y, x) = static_cast<float>(tmp * tmp);
        }
        else
          chnl.at(y, x) = undef;
      }
    
      //no background pixel in row => all pixel are set to undef;
      //continue with next row
      if(pos == undef) continue;
      else{
        pos = static_cast<int>(undef);
        for(x = chnl.columns() - 1; x >= 0; --x){
          if(chnl.at(y, x) == 0){
            pos = x; //found fv
          }
          else if(pos != undef){
            int tmp = pos - x;
            tmp *=tmp;
            int ret = static_cast<int>(chnl.at(y, x));
            if(ret > tmp || ret == undef){
              chnl.at(y, x) = static_cast<float>(tmp);
            }
          }
        }
      }
    }
  }

  void distanceTransform::sedFiltering(fmatrix &chnl, 
                                        bool useEightSED) const {

    const float fv  = 0.0f;
    const int undef = -2;

    matrix<ipoint> dist(chnl.size());

    int row, 
        col;

    //init
    for(row = 0; row < chnl.rows(); ++row){
      for(col = 0; col < chnl.columns(); ++col){
        if(chnl.at(row, col) == fv)
          dist.at(row, col) = ipoint(0, 0);
        else
          dist.at(row, col) = ipoint(undef, undef);
      }
    }

    if(useEightSED) 
      eightSEDFiltering(chnl, dist);
    else            
      fourSEDFiltering(chnl, dist);

    //set the distances 
    for(row = 0; row < chnl.rows(); ++row)
      for(col = 0; col < chnl.columns(); ++col)
        chnl.at(row, col) =
          static_cast<float>(dist.at(row, col).distanceSqr(ipoint(0,0)));
  }

  void distanceTransform::fourSEDFiltering(fmatrix &chnl, 
                                            matrix<ipoint> &dist) const {
    
    //create all masks
    static const ipoint mask0[] = { ipoint(-1, 0) };
    static const sedMask l(mask0, 1);

    static const ipoint mask1[] = { ipoint(0, -1) };
    static const sedMask u(mask1, 1);
    
    static const ipoint mask2[] = { ipoint(0, -1), ipoint(-1, 0) };
    static const sedMask ul(mask2, 2);

    static const ipoint mask3[] = { ipoint(1, 0) };
    static const sedMask r(mask3, 1);

    static const ipoint mask4[] = { ipoint(0, 1) };
    static const sedMask d(mask4, 1);
    
    static const ipoint mask5[] = { ipoint(1, 0), ipoint(0, 1) };
    static const sedMask rd(mask5, 2);

    
    ipoint pos;
    pos.y = 0;
    
    //first line
    for(pos.x = 1; pos.x < chnl.columns(); ++pos.x)
      l.filter(dist, pos);
    for(pos.x = chnl.columns() - 2; pos.x >= 0; --pos.x)
      r.filter(dist, pos);

    for(pos.y = 1; pos.y < chnl.rows(); ++pos.y){
      
      pos.x = 0;
      //step down
      u.filter(dist, pos);

      for(pos.x = 1; pos.x < chnl.columns(); ++pos.x)
        ul.filter(dist, pos);
      for(pos.x = chnl.columns() - 2; pos.x >= 0; --pos.x)
        r.filter(dist, pos);
    }

    //and now filter the picture in the opposite direction
    pos.y = chnl.rows() - 1;

    //last line
    for(pos.x = chnl.columns() - 2; pos.x >= 0; --pos.x)
      r.filter(dist, pos);
    for(pos.x = 1; pos.x < chnl.columns(); ++pos.x)
      l.filter(dist, pos);

    for(pos.y = chnl.rows() - 2; pos.y >= 0; --pos.y){
      
      pos.x = chnl.columns() - 1;     
      //step up
      d.filter(dist, pos);

      for(pos.x = chnl.columns() - 2; pos.x >= 0; --pos.x)
        rd.filter(dist, pos);
      for(pos.x = 1; pos.x < chnl.columns(); ++pos.x)
        l.filter(dist, pos);
    }
  }
  
  void distanceTransform::eightSEDFiltering(fmatrix &chnl,
                                            matrix<ipoint> &dist)const{
  
    //create all masks
    static const ipoint mask0[] = { ipoint(-1, 0) };
    static const sedMask xo(mask0, 1);

    static const ipoint mask1[] = { ipoint(-1,-1), ipoint(0,-1), 
                                    ipoint(1,-1), ipoint(-1, 0) };
    static const sedMask xxxxo(mask1, 4);

    static const ipoint mask2[] = { ipoint(-1, -1), ipoint(0, -1),
                                    ipoint(-1, 0) };
    static const sedMask xxxo(mask2, 3);

    static const ipoint mask3[] = { ipoint(0, -1), ipoint(1, -1) };
    static const sedMask xxo(mask3, 2);

    
    static const ipoint mask4[] = { ipoint(1, 0) };
    static const sedMask ox(mask4, 1);

    static const ipoint mask5[] = { ipoint(1, 0), ipoint(-1, 1), 
                                    ipoint(0, 1), ipoint(1, 1) };
    static const sedMask oxxxx(mask5, 4);

    static const ipoint mask6[] = { ipoint(1, 0), ipoint(0, 1), ipoint(1, 1) };
    static const sedMask oxxx(mask6, 3);

    static const ipoint mask7[] = { ipoint(-1, 1), ipoint(0, 1) };
    static const sedMask oxx(mask7, 2);

    //filter the picture
    ipoint pos;
    pos.y = 0; 
    
    //first row
    for(pos.x = 1; pos.x < chnl.columns(); ++pos.x) {
      xo.filter(dist, pos);
    }
    for(pos.x = chnl.columns() - 2; pos.x >= 0; --pos.x) {
      ox.filter(dist, pos);
    }

    for(pos.y = 1; pos.y < chnl.rows(); ++pos.y) {
      pos.x = 0;  
      //step up
      xxo.filter(dist, pos);

      for(pos.x = 1; pos.x < chnl.columns() - 1; ++pos.x) {
        xxxxo.filter(dist, pos);
      }
      xxxo.filter(dist, pos);
      for(pos.x = chnl.columns() - 2; pos.x >= 0; --pos.x) {
        ox.filter(dist, pos);
      }
    }

    //and now filter the picture in the opposite direction
    pos.y = chnl.rows() - 1;

    //last row
    for(pos.x = chnl.columns() - 2; pos.x >= 0; --pos.x) {
      ox.filter(dist, pos);
    }

    for(pos.x = 1; pos.x < chnl.columns(); ++pos.x) {
      xo.filter(dist, pos);
    }

    for(pos.y = chnl.rows() - 2; pos.y >= 0; --pos.y) {
      pos.x = chnl.columns() - 1;
      //step down
      oxx.filter(dist, pos);

      for(pos.x = chnl.columns() - 2; pos.x > 0; --pos.x) {
        oxxxx.filter(dist, pos);
      }
      oxxx.filter(dist, pos);
      for(pos.x = 1; pos.x < chnl.columns() - 1; ++pos.x) {
        xo.filter(dist, pos);
      }
    }
  }


  distanceTransform::sedMask::sedMask(const ipoint mask[], 
                                      int size) : mask_(mask), size_(size) {
  }

  distanceTransform::sedMask::~sedMask() {
  }

  void distanceTransform::sedMask::filter(matrix<ipoint> &dist, 
                                          const ipoint   &pos) const {
      
    ipoint shortest,tmp;

    shortest = dist.at(pos.y + mask_[0].y, pos.x + mask_[0].x) 
               + ipoint(abs(mask_[0].x), abs(mask_[0].y));
    
    for (int i = 1; i < size_; ++i) {
      
      tmp =   dist.at(pos.y + mask_[i].y, pos.x + mask_[i].x) 
            + ipoint(abs(mask_[i].x), abs(mask_[i].y));
      
      queryDistance(shortest, tmp);
    }

    tmp = dist.at(pos);
    queryDistance(shortest, tmp);

    if (shortest.x >= 0) {
      dist.at(pos) = shortest;
    }

  }

  /**
   * Read a distanceTransform::eDistanceType
   *
   * @ingroup gStorable
   */
  bool read(ioHandler& handler,
            distanceTransform::eDistanceType& data) {

    std::string str;
    
    if (handler.read(str)) {
      
      if (str == "EightNeighborhood") {
        data = distanceTransform::EightNeighborhood;
      } else if (str == "FourNeighborhood") {
        data = distanceTransform::FourNeighborhood;
      } else if (str == "EuclideanSqr") {
        data = distanceTransform::EuclideanSqr;
      } else if (str == "Euclidean") {
        data = distanceTransform::Euclidean;
      } else if (str == "EightSED") {
        data = distanceTransform::EightSED;
      } else if (str == "EightSEDSqr") {
        data = distanceTransform::EightSEDSqr;
      } else if (str == "FourSED") {
        data = distanceTransform::FourSED;
      } else if (str == "FourSEDSqr") {
        data = distanceTransform::FourSEDSqr;
      }  
      return true;
    }
    return false;
  }

  /**
   * Write a distanceTransform::eDistanceType
   *
   * @ingroup gStorable
   */
  bool write(ioHandler& handler,
             const distanceTransform::eDistanceType& data) {
    bool b = false;
    switch(data) {
      case distanceTransform::EightNeighborhood:
        b = handler.write("EightNeighborhood");
        break;
      case distanceTransform::FourNeighborhood:
        b = handler.write("FourNeighborhood");
        break;
      case distanceTransform::EuclideanSqr:
        b = handler.write("EuclideanSqr");
        break;
      case distanceTransform::Euclidean:
        b = handler.write("Euclidean");
        break;
      case distanceTransform::EightSED:
        b = handler.write("EightSED");
        break;
      case distanceTransform::EightSEDSqr:
        b = handler.write("EightSEDSqr");
        break;
      case distanceTransform::FourSED:
        b = handler.write("FourSED");
        break;
      case distanceTransform::FourSEDSqr:
        b = handler.write("FourSEDSqr");
        break;
      default:
        handler.write("Unknown");
        b = false;
        handler.setStatusString("Undefined eDistanceType");
        break;
    }

    return b;
  }
  


}

