/*
 * Copyright (C) 2007
 * Pablo Alvarado, Instituto Tecnológico de Costa Rica
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
 * \file   ltiGALContrastEnhancement.cpp
 *         Contains a class for contrast enhancement
 * 
 * \author Miguel Angel Aguilar Ulloa
 * \date   12/10/2007
 *
 * revisions ..: $Id: ltiGALContrastEnhancement.cpp,v 1.3 2009-11-12 22:02:20 alvarado Exp $
 */

#include "ltiGALContrastEnhancement.h"

namespace lti {
  // --------------------------------------------------
  // galContrastEnhancement::parameters
  // --------------------------------------------------

  // default constructor
  galContrastEnhancement::parameters::parameters()
    : contrastEnhancement::parameters() {
    delta = float(0.8);
  }

  // copy constructor
  galContrastEnhancement::parameters::parameters(const parameters& other)
    : contrastEnhancement::parameters() {
    copy(other);
  }

  // destructor
  galContrastEnhancement::parameters::~parameters() {
  }

  // copy member

  galContrastEnhancement::parameters&
  galContrastEnhancement::parameters::copy(const parameters& other) {
    contrastEnhancement::parameters::copy(other);

    
    delta = other.delta;

    return *this;
  }

  // alias for copy method
  galContrastEnhancement::parameters&
  galContrastEnhancement::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  // class name
  const std::string& galContrastEnhancement::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone method
  galContrastEnhancement::parameters*
  galContrastEnhancement::parameters::clone() const {
    return new parameters(*this);
  }

  // new instance
  galContrastEnhancement::parameters*
  galContrastEnhancement::parameters::newInstance() const {
    return new parameters();
  }

  /*
   * Write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool galContrastEnhancement::parameters::write(ioHandler& handler,
                                         const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {
      
      lti::write(handler,"delta",delta);
    }

    b = b && contrastEnhancement::parameters::write(handler,false);

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
  bool galContrastEnhancement::parameters::read(ioHandler& handler,
                                        const bool complete)  {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {
      
      lti::read(handler,"delta",delta);
    }

    b = b && contrastEnhancement::parameters::read(handler,false);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

  // --------------------------------------------------
  // galContrastEnhancement
  // --------------------------------------------------

  // default constructor
  galContrastEnhancement::galContrastEnhancement()
    : contrastEnhancement() {
    // create an instance of the parameters with the default values
    parameters defaultParameters;
    // set the default parameters
    setParameters(defaultParameters);
  }

  // default constructor
  galContrastEnhancement::galContrastEnhancement(const parameters& par)
    : contrastEnhancement() {

    // set the given parameters
    setParameters(par);
  }

  // Construct a constrast enhancement contrastEnhancement with the given delta
  galContrastEnhancement::galContrastEnhancement(const float delta)
      : contrastEnhancement() {
    parameters tmpParam;
    tmpParam.delta = delta;
    setParameters(tmpParam);
  }

  // copy constructor
  galContrastEnhancement::galContrastEnhancement(const galContrastEnhancement& other)
    : contrastEnhancement() {
    copy(other);
  }

  // destructor
  galContrastEnhancement::~galContrastEnhancement() {
  }

  // copy member
  galContrastEnhancement& 
  galContrastEnhancement::copy(const galContrastEnhancement& other) {
    contrastEnhancement::copy(other);

    return (*this);
  }

  // alias for copy member
  galContrastEnhancement&
  galContrastEnhancement::operator=(const galContrastEnhancement& other) {
    return (copy(other));
  }

  // class name
  const std::string& galContrastEnhancement::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone member
  galContrastEnhancement* galContrastEnhancement::clone() const {
    return new galContrastEnhancement(*this);
  }

  // create a new instance
  galContrastEnhancement* galContrastEnhancement::newInstance() const {
    return new galContrastEnhancement();
  }

  // return parameters
  const galContrastEnhancement::parameters&
  galContrastEnhancement::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&contrastEnhancement::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }

  // -------------------------------------------------------------------
  // The apply() member functions
  // -------------------------------------------------------------------

  // negative of an image
  inline bool galContrastEnhancement::complementImage(channel8& srcdest) const {
  
    // define iterators

    // srcdest iterator
    channel8::iterator sdIt;
    // end of source
    const channel8::const_iterator eIt = srcdest.end();

    // cicle throw all image
    for(sdIt = srcdest.begin();eIt!=sdIt; ++sdIt) {
      *sdIt = 255-*sdIt;
    }
    return true;
  }
  
  // sweep in the throw all minima or saddle points in the input
  // image and procces hillocks
  bool galContrastEnhancement::processHillocks(channel8& srcdest, 
                                               const float delta) const{
    
    // bolean matrix
    imatrix hillocksMatrix(srcdest.size());

    // delta's matrix
    fmatrix deltaMatrix(srcdest.size(),0.0f);
     
    // fast relabeling
    fastRelabeling::parameters param;
    param.sortSize = false;
    param.assumeLabeledMask = false;
    fastRelabeling labelHillocks(param);

    // image limits
    int rowMax = srcdest.rows();
    int colMax = srcdest.columns();

    // initial plane
    int b=-1;

    // cicle for each sweep 
    while(b<255) {

      // obtain bolean matrix
      for(int row=0; row<rowMax; ++row) {

        // get references to the vectors to accelerate rows access
        ivector& hillVct = hillocksMatrix.getRow(row);
        const vector<ubyte>& srcVct = srcdest.getRow(row);

        for(int col=0; col<colMax; ++col){
          hillVct.at(col) = (srcVct.at(col)>b) ? 1 : 0;
        }
      }

      // identify the set of hillock 
      int hillocksNum; // number of hillocks for this sweep
      labelHillocks.apply(hillocksMatrix,hillocksNum); 
      
      // define the vectors for max value (I(pMax)) 
      // for each hillock and delta Max
      vector<int> pMax(hillocksNum,0);
      vector<float> deltaMax(hillocksNum,0.0f);
      
      // initialize the value to find the next plane from top to bottom
      int bMin=255;
      
      // cicle for each hillock each sweep to find I(pMax) and deltaMax
      for(int row=0; row<rowMax; ++row) {
        for(int col=0; col<colMax; ++col){
          // find the max value for each hillock on the present plane
          if(srcdest.at(row,col)>pMax.at(hillocksMatrix.at(row,col))) {
            pMax.at(hillocksMatrix.at(row,col)) = srcdest.at(row,col);
          }
          // find the minimum absolute value of a pixel above the present
          // plane, this is the value for next plane
          if(srcdest.at(row,col)<=bMin && srcdest.at(row,col)>b) {
            bMin = srcdest.at(row,col);
          }
        }
      }
      
      // find deltaMax for each hillock on the present plane
      for(int i=0; i<deltaMax.size();++i) {
        deltaMax.at(i) = min(delta,(float(255-b))/
                             (float(pMax.at(i)-b))-1.0f);
      }
      
      // initialize the delta to be apply for pixels 
      float deltaApply=0;
      
      //find delta's for each pixel
      for(int row=0; row<rowMax; ++row) {
        for(int col=0; col<colMax; ++col){
          
          // find the delta to be applied to the pixel
          deltaApply = deltaMax.at(hillocksMatrix.at(row,col));
          
          // history of delta to the pixel
          if(deltaApply+deltaMatrix.at(row,col)>delta){ 
            deltaApply = delta - deltaMatrix.at(row,col);
          }
          
          // update the value of pixel
          float update = (1.0f+deltaApply)*
            (float(srcdest.at(row,col)-b))+float(b);
          
          srcdest.at(row,col) = static_cast<ubyte>(update);
          
          deltaMatrix.at(row,col) += deltaApply;
        }
      }
      
      // update the value for the plane for the next sweep
      b = bMin;
    }

    return true;
  }

  bool galContrastEnhancement::enhance(channel8& srcdest) const {

    if (srcdest.empty()) {
      return true;
    }

    // get parameters
    const parameters& param = getParameters();

    // delta value
    const float delta = param.delta;
    
    processHillocks(srcdest,delta); // process hillocks
    complementImage(srcdest);       // complement image
    processHillocks(srcdest,delta); // process valleys
    complementImage(srcdest);       // complement image and obtain final result

    return true;
  }

  // On place apply for type channel!
  bool galContrastEnhancement::apply(channel& srcdest) const {
    
    channel8 srcdestTmp;
    srcdestTmp.castFrom(srcdest);
    
    if(enhance(srcdestTmp)){
      srcdest.castFrom(srcdestTmp);
      return true;
    }
    return false;
  }

  // On place apply for type channel8!
  bool galContrastEnhancement::apply(channel8& srcdest) const {
    return enhance(srcdest);
  }

  // On copy apply for type channel!
  bool galContrastEnhancement::apply(const channel& src, channel& dest) const {
 
    channel8 tmp;
    tmp.castFrom(src);

    if(enhance(tmp)) {
      dest.castFrom(tmp);
      return true;
    }
    return false;
  }

  // On copy apply for type channel8!
  bool galContrastEnhancement::
   apply(const channel8& src, channel8& dest) const {
 
    channel8 tmp;
    tmp.copy(src);

    if(enhance(tmp)) {
      tmp.detach(dest);
      return true;
    }
    return false;
  }
}

