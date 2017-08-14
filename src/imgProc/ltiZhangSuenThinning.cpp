/*
 * Copyright (C) 2008
 * Ronald Eduardo Garcia Fernandez
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
 * \file   ltiZhangSuenThinning.cpp
 *         Contains the class lti::zhangSuenThinning with the well known
 *         thinning algorithm.
 * 
 * \author Ronald Eduardo Garcia Fernandez
 * \date   05/14/2008
 *
 * revisions ..: $Id: ltiZhangSuenThinning.cpp,v 1.3 2013-08-28 14:49:40 alvarado Exp $
 */


#include "ltiZhangSuenThinning.h"

#include <limits>

#undef _LTI_DEBUG
//#define _LTI_DEBUG 4
#include "ltiDebug.h"

#ifdef _LTI_DEBUG
#include "ltiViewer2D.h"
#endif

namespace lti {
  // --------------------------------------------------
  // zhangSuenThinning::parameters
  // --------------------------------------------------

  // default constructor
  zhangSuenThinning::parameters::parameters()
    : functor::parameters() {
    // -1 no limit iterate until there are no more pixels to delete 
    maxIterations = int(-1);
    backgroundValue = 0;
    lineValue = 255;
  }

  // copy constructor
  zhangSuenThinning::parameters::parameters(const parameters& other)
    : functor::parameters() {
    copy(other);
  }

  // destructor
  zhangSuenThinning::parameters::~parameters() {
  }

  // copy member

  zhangSuenThinning::parameters&
  zhangSuenThinning::parameters::copy(const parameters& other) {
    functor::parameters::copy(other);

    maxIterations   = other.maxIterations;
    backgroundValue = other.backgroundValue;
    lineValue       = other.lineValue;

    return *this;
  }

  // alias for copy method
  zhangSuenThinning::parameters&
  zhangSuenThinning::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  // class name
  const std::string& zhangSuenThinning::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone method
  zhangSuenThinning::parameters*
  zhangSuenThinning::parameters::clone() const {
    return new parameters(*this);
  }

  // new instance
  zhangSuenThinning::parameters*
  zhangSuenThinning::parameters::newInstance() const {
    return new parameters();
  }

  /*
   * Write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool zhangSuenThinning::parameters::write(ioHandler& handler,
                                         const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {
      
      lti::write(handler,"maxIterations",maxIterations);
      lti::write(handler,"backgroundValue",backgroundValue);
      lti::write(handler,"lineValue",lineValue);
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
  bool zhangSuenThinning::parameters::read(ioHandler& handler,
                                        const bool complete)  {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {
      
      lti::read(handler,"maxIterations",maxIterations);
      lti::read(handler,"backgroundValue",backgroundValue);
      lti::read(handler,"lineValue",lineValue);
    }

    b = b && functor::parameters::read(handler,false);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

  // --------------------------------------------------
  // zhangSuenThinning
  // --------------------------------------------------

  // default constructor
  zhangSuenThinning::zhangSuenThinning()
    : functor() {

    // create an instance of the parameters with the default values
    parameters defaultParameters;
    // set the default parameters
    setParameters(defaultParameters);
  }

  // default constructor
  zhangSuenThinning::zhangSuenThinning(const parameters& par)
    : functor() {

    // set the given parameters
    setParameters(par);
  }

  // Construct a constrast enhancement functor with the given delta
  zhangSuenThinning::zhangSuenThinning(const int maxIterations)
      : functor() {
    parameters tmpParam;
    tmpParam.maxIterations = maxIterations;
    setParameters(tmpParam);
  }

  // copy constructor
  zhangSuenThinning::zhangSuenThinning(const zhangSuenThinning& other)
    : functor() {
    copy(other);
  }

  // destructor
  zhangSuenThinning::~zhangSuenThinning() {
  }

  // copy member
  zhangSuenThinning& 
  zhangSuenThinning::copy(const zhangSuenThinning& other) {
    functor::copy(other);

    return (*this);
  }

  // alias for copy member
  zhangSuenThinning&
  zhangSuenThinning::operator=(const zhangSuenThinning& other) {
    return (copy(other));
  }

  // class name
  const std::string& zhangSuenThinning::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone member
  zhangSuenThinning* zhangSuenThinning::clone() const {
    return new zhangSuenThinning(*this);
  }

  // create a new instance
  zhangSuenThinning* zhangSuenThinning::newInstance() const {
    return new zhangSuenThinning();
  }

  // return parameters
  const zhangSuenThinning::parameters&
  zhangSuenThinning::getParameters() const {
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

  template<typename T>
  bool zhangSuenThinning::skeletonImage(const lti::matrix<T> &src,
                                        lti::matrix<T> &dest) const {
	 
    if (src.empty()) {
      dest.clear();
      return false;
    }

    // get parameters
    const parameters& param = getParameters();
    const int maxIterations = param.maxIterations <= 0 ? 
      std::numeric_limits<int>::max() : param.maxIterations;	

    // if T is floating type then norm by 255, otherwise by 1
    T norm;
    if (std::numeric_limits<T>::is_integer) {
      norm = T(1);
    } else {
      norm = T(255);
    }
    
    const T background = T(param.backgroundValue)/norm;
    const T lineVal    = T(param.lineValue)/norm;

    // Table for first iteration
    // with 1 meaning a delete case 
    //
    // This table encodes the four conditions:
    // a) 2 <= number of pixels in the 8-neighborhood <= 6
    // b) Number of transitions 0 to 1 is exactly 1
    // c) p_1 & p_3 & p_5 = 0
    // d) p_3 & p_5 & p_7 = 0
    static const ubyte s1[] = {0,0,0,1,0,0,1,1,0,0, //   0 -   9
                               0,0,1,0,1,1,0,0,0,0, //  10 -  19
                               0,0,0,0,1,0,0,0,1,0, //  20 -  29
                               1,0,0,0,0,0,0,0,0,0, //  30 -  39
                               0,0,0,0,0,0,0,0,1,0, //  40 -  49
                               0,0,0,0,0,0,1,0,0,0, //  50 -  59
                               1,0,1,0,0,0,0,0,0,0, //  60 -  69
                               0,0,0,0,0,0,0,0,0,0, //  70 -  79
                               0,0,0,0,0,0,0,0,0,0, //  80 -  89
                               0,0,0,0,0,0,1,0,0,0, //  90 -  99
                               0,0,0,0,0,0,0,0,0,0, // 100 - 109
                               0,0,1,0,0,0,0,0,0,0, // 110 - 119
                               1,0,0,0,0,0,0,0,0,1, // 120 - 129
                               0,1,0,0,0,1,0,0,0,0, // 130 - 139
                               0,0,0,1,0,0,0,0,0,0, // 140 - 149
                               0,0,0,0,0,0,0,0,0,0, // 150 - 159
                               0,0,0,0,0,0,0,0,0,0, // 160 - 169
                               0,0,0,0,0,0,0,0,0,0, // 170 - 179
                               0,0,0,0,0,0,0,0,0,0, // 180 - 189
                               0,0,1,1,0,1,0,0,0,1, // 190 - 199
                               0,0,0,0,0,0,0,1,0,0, // 200 - 209
                               0,0,0,0,0,0,0,0,0,0, // 210 - 219
                               0,0,0,0,1,1,0,1,0,0, // 220 - 229
                               0,1,0,0,0,0,0,0,0,0, // 230 - 239
                               1,1,0,1,0,0,0,0,1,1, // 240 - 249
                               0,0,0,0,0,0};        // 250 - 255

    // Table for second iteration
    // with 1 meaning a delete case
    //
    // This table encodes the four conditions:
    // a) 2 <= number of pixels in the 8-neighborhood <= 6
    // b) Number of transitions 0 to 1 is exactly 1
    // c') p_1 & p_3 & p_7 = 0
    // d') p_1 & p_5 & p_7 = 0
    static const ubyte s2[] = {0,0,0,1,0,0,1,1,0,0, //   0 -   9
                               0,0,1,0,1,1,0,0,0,0, //  10 -  19
                               0,0,0,0,1,0,0,0,1,0, //  20 -  29
                               1,1,0,0,0,0,0,0,0,0, //  30 -  39
                               0,0,0,0,0,0,0,0,1,0, //  40 -  49
                               0,0,0,0,0,0,1,0,0,0, //  50 -  59
                               1,0,1,1,0,0,0,0,0,0, //  60 -  69
                               0,0,0,0,0,0,0,0,0,0, //  70 -  79
                               0,0,0,0,0,0,0,0,0,0, //  80 -  89
                               0,0,0,0,0,0,1,0,0,0, //  90 -  99
                               0,0,0,0,0,0,0,0,0,0, // 100 - 109
                               0,0,1,0,0,0,0,0,0,0, // 110 - 119
                               1,0,0,0,1,0,1,0,0,1, // 120 - 129
                               0,1,0,0,0,1,0,0,0,0, // 130 - 139
                               0,0,0,1,0,0,0,0,0,0, // 140 - 149
                               0,0,0,0,0,0,0,0,0,1, // 150 - 159
                               0,0,0,0,0,0,0,0,0,0, // 160 - 169
                               0,0,0,0,0,0,0,0,0,0, // 170 - 179
                               0,0,0,0,0,0,0,0,0,0, // 180 - 189
                               0,0,1,1,0,1,0,0,0,0, // 190 - 199
                               0,0,0,0,0,0,0,0,0,0, // 200 - 209
                               0,0,0,0,0,0,0,0,0,0, // 210 - 219
                               0,0,0,0,1,1,0,1,0,0, // 220 - 229
                               0,0,0,0,0,0,0,0,0,0, // 230 - 239
                               1,0,0,0,0,0,0,0,1,0, // 240 - 249
                               0,0,1,0,0,0};        // 250 - 259
  
    // flag to stop the algorithm
    bool keepGoing=true;
    
    int iterations=0;
    
    //conditions for pixel deletion
    
    // reserve the memory for the result
    dest.allocate(src.size());

    dest.getRow(0).fill(T(0));
    dest.getRow(dest.lastRow()).fill(T(0));

    // temporal matrix
    matrix<T> tmp(src.size());
    tmp.getRow(0).fill(T(0));
    tmp.getRow(dest.lastRow()).fill(T(0));

    // this points to the data being taken for analysis, and will
    // jump from src, to tmp and dest
    const matrix<T>* s = &src;

    const int rows = src.lastRow();
    const int cols = src.lastColumn();

#ifdef _LTI_DEBUG
    viewer2D::parameters viewerPar;
    viewerPar.title="Iterations";
    viewerPar.mappingType = viewer2DPainter::Optimal;
    viewer2D view(viewerPar);
#endif


    // Iterate until there are no more pixels to delete
    while (keepGoing && (iterations < maxIterations)) {
      keepGoing = false;
      
      // first iteration
      for(int j=1; j < rows; ++j) { 
        vector<T>& brow = tmp.getRow(j);
        int i;
        
        brow.at(0)=0;
        for(i=1; i < cols; ++i) {
          const T center = s->at(j,i);
          if (center == 0) {
            brow.at(i) = 0;
            continue;
          }

          const ubyte code = 
            static_cast<ubyte>( ((s->at(j-1,i  ) != 0) ?   1 : 0) +   // N
                                ((s->at(j-1,i+1) != 0) ?   2 : 0) +   // NE
                                ((s->at(j  ,i+1) != 0) ?   4 : 0) +   // E
                                ((s->at(j+1,i+1) != 0) ?   8 : 0) +   // SE
                                ((s->at(j+1,i  ) != 0) ?  16 : 0) +   // S
                                ((s->at(j+1,i-1) != 0) ?  32 : 0) +   // SW
                                ((s->at(j  ,i-1) != 0) ?  64 : 0) +   // W
                                ((s->at(j-1,i-1) != 0) ? 128 : 0) );  // NW
          // if we delete something, then there is a change and we keep on
          if (s1[code]!=0) {
            brow.at(i) = 0;
            keepGoing = true;
          } else {
            brow.at(i) = 1;
          }         
        }
        brow.at(i)=0;
      }

      _lti_if_debug(view.show(tmp));
      _lti_debug("  Iteration " << iterations << "(a) \n");

      // second iteration operates on the last result
      s=&tmp;

      for(int j=1; j < rows; ++j) { 
        vector<T>& brow = dest.getRow(j);
        int i;
        
        brow.at(0)=0;
        for(i=1; i < cols; ++i) {
          const T center = s->at(j,i);
          if (center == 0) {
            brow.at(i) = 0;
            continue;
          }
          const ubyte code = 
            static_cast<ubyte>( ((s->at(j-1,i  ) != 0) ?   1 : 0) +   // N
                                ((s->at(j-1,i+1) != 0) ?   2 : 0) +   // NE
                                ((s->at(j  ,i+1) != 0) ?   4 : 0) +   // E
                                ((s->at(j+1,i+1) != 0) ?   8 : 0) +   // SE
                                ((s->at(j+1,i  ) != 0) ?  16 : 0) +   // S
                                ((s->at(j+1,i-1) != 0) ?  32 : 0) +   // SW
                                ((s->at(j  ,i-1) != 0) ?  64 : 0) +   // W
                                ((s->at(j-1,i-1) != 0) ? 128 : 0) );  // NW
          // if we delete something, then there is a change and we keep on
          if (s2[code]!=0) {
            brow.at(i) = 0;
            keepGoing = true;
          } else {
            brow.at(i) = 1;
          }         
        }
        brow.at(i)=0;
      }
      
      s=&dest;

      _lti_if_debug(view.show(dest));
      _lti_debug("  Iteration " << iterations << "(b) \n");
      
      iterations++;
    }
    
    // finaly, change the values
    for(int j=0; j < dest.rows(); ++j){  //Restore image to its original colors
      for(int i = 0; i < dest.columns(); ++i){
        dest.at(j,i) = (dest.at(j,i)!=0) ? lineVal : background;
      }
    }
    
    _lti_debug("Thinning complete! " <<
               "Number of iterations: " << iterations <<'\n');
  
    return true;
    
  }
  

  // On place apply for type fmatrix!
  bool zhangSuenThinning::apply(fmatrix& srcDest) const {
    
    fmatrix tmp;
    
    if(skeletonImage(srcDest,tmp)){
      tmp.detach(srcDest);
      return true;
    }
    return false;
  }

  // On place apply for type matrix<ubyte>!
  bool zhangSuenThinning::apply(matrix<ubyte>& srcDest) const {
    matrix<ubyte> tmp;
    if (skeletonImage(srcDest,tmp)) {
      tmp.detach(srcDest);
      return true;
    }

    return false;
  }

  // On copy apply for type fmatrix!
  bool zhangSuenThinning::apply(const fmatrix& src, fmatrix& dest) const {
 
    return skeletonImage(src,dest);
  }

  // On copy apply for type matrix<ubyte>!
  bool zhangSuenThinning::apply(const matrix<ubyte>& src,
                                matrix<ubyte>& dest) const {
    return skeletonImage(src,dest);
  }
}
