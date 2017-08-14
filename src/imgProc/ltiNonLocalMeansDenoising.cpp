/*
 * Copyright (C) 2007
 * Pablo Alvarado, Instituto Tecnol�gico de Costa Rica
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
 * \file   ltiNonLocalMeansDenoising.cpp
 *         Contains a class for image denoising
 * 
 * \author Miguel Angel Aguilar Ulloa
 * \date   29/09/2007
 *
 * revisions ..: $Id: ltiNonLocalMeansDenoising.cpp,v 1.1 2008-02-18 00:17:23 alvarado Exp $
 */

#include "ltiNonLocalMeansDenoising.h"
#include "ltiTypeInfo.h"
#include "ltiTypes.h"
#include "ltiGaussKernels.h"
#include "ltiKernel2D.h"
#include "ltiBoundaryExpansion.h"
#include "ltiMath.h"

namespace lti {
  // --------------------------------------------------
  // nonLocalMeansDenoising::parameters
  // --------------------------------------------------

  // default constructor
  nonLocalMeansDenoising::parameters::parameters()
    : denoising::parameters() {
    // default values
    windowSize = int(5);        
    subregionSize = int(11);
    variance = 5.0;
    filterDegree = 0.006f;
    boundaryType = Mirror;
  }

  // copy constructor
  nonLocalMeansDenoising::parameters::parameters(const parameters& other)
    : denoising::parameters() {
    copy(other);
  }

  // destructor
  nonLocalMeansDenoising::parameters::~parameters() {
  }

  // copy member

  nonLocalMeansDenoising::parameters&
  nonLocalMeansDenoising::parameters::copy(const parameters& other) {
    denoising::parameters::copy(other);

    windowSize = other.windowSize;
    subregionSize = other.subregionSize;
    variance = other.variance;
    filterDegree = other.filterDegree;

    return *this;
  }

  // alias for copy method
  nonLocalMeansDenoising::parameters&
  nonLocalMeansDenoising::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  // class name
  const std::string& nonLocalMeansDenoising::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone method
  nonLocalMeansDenoising::parameters*
  nonLocalMeansDenoising::parameters::clone() const {
    return new parameters(*this);
  }

  // new instance
  nonLocalMeansDenoising::parameters*
  nonLocalMeansDenoising::parameters::newInstance() const {
    return new parameters();
  }

  /*
   * Write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool nonLocalMeansDenoising::parameters::write(ioHandler& handler,
                                         const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {
      lti::write(handler,"windowSize",windowSize);
      lti::write(handler,"subregionSize",subregionSize);
      lti::write(handler,"variance",variance);
      lti::write(handler,"filterDegree",filterDegree);
    }

    b = b && denoising::parameters::write(handler,false);

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
  bool nonLocalMeansDenoising::parameters::read(ioHandler& handler,
                                        const bool complete)  {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {
      
      lti::read(handler,"windowSize",windowSize);
      lti::read(handler,"subregionSize",subregionSize);
      lti::read(handler,"variance",variance);
      lti::read(handler,"filterDegree",filterDegree);
    }

    b = b && denoising::parameters::read(handler,false);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

  // --------------------------------------------------
  // nonLocalMeansDenoising
  // --------------------------------------------------

  // default constructor
  nonLocalMeansDenoising::nonLocalMeansDenoising()
    : denoising() {
    // create an instance of the parameters with the default values
    parameters defaultParameters;
    // set the default parameters
    setParameters(defaultParameters);
  }

  // default constructor
  nonLocalMeansDenoising::nonLocalMeansDenoising(const parameters& par)
    : denoising() {
    // set the given parameters
    setParameters(par);
  }

  // copy constructor
  nonLocalMeansDenoising::nonLocalMeansDenoising(const nonLocalMeansDenoising& other)
    : denoising() {
    copy(other);
  }

  // destructor
  nonLocalMeansDenoising::~nonLocalMeansDenoising() {
  }

  // copy member
  nonLocalMeansDenoising& 
  nonLocalMeansDenoising::copy(const nonLocalMeansDenoising& other) {
    denoising::copy(other);

    return (*this);
  }

  // alias for copy member
  nonLocalMeansDenoising&
  nonLocalMeansDenoising::operator=(const nonLocalMeansDenoising& other) {
    return (copy(other));
  }

  // class name
  const std::string& nonLocalMeansDenoising::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone member
  nonLocalMeansDenoising* nonLocalMeansDenoising::clone() const {
    return new nonLocalMeansDenoising(*this);
  }

  // create a new instance
  nonLocalMeansDenoising* nonLocalMeansDenoising::newInstance() const {
    return new nonLocalMeansDenoising();
  }

  // return parameters
  const nonLocalMeansDenoising::parameters&
  nonLocalMeansDenoising::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&denoising::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }

  // -------------------------------------------------------------------
  // The apply() member functions
  // -------------------------------------------------------------------

  //Apply the NL-means algorithm to a channel for denoise it.
  bool nonLocalMeansDenoising::denoise(const channel& src, channel& dest) const {

    if (src.empty()) {
      dest.clear();
      return true;
    }

    //resizes the destination matrix
    dest.allocate(src.size());

    // NL-means
    float average;    // it acumulates the value of the pixels weighted
    float weight;     // weight for each pair of windows
    float weightMax;  // max weight for each pixel
    float totalWeight;// the total weight for each pixel
    float d;          // it accumulates the value of the euclidean distance for
                      // each pair of windows

    // get parameters
    const parameters& param = getParameters();

    // filter degree
    const float filterDegree = param.filterDegree;

    // subregion size
    const int subregionSize = param.subregionSize +
      ((param.subregionSize%2 == 0) ? 1:0);
    const int halfSubregion = subregionSize/2;

    // the method can only be applied with a even value of kernelSize
    const int sizeOfWindow = param.windowSize +
      ((param.windowSize%2 == 0) ? 1:0);
    const int halfWindow = sizeOfWindow/2;

    // boundary type
    const eBoundaryType boundaryType = param.boundaryType; 
   
    // Variance for gaussian kernel
    const double variance = param.variance;

    // kernel
    gaussKernel2D<float> gauss(sizeOfWindow,variance);
    kernel2D<float>      kern;
    kern.castFrom(gauss);

    // need kernel as matrix
    //const matrix<float>& filter = kern;
    
    // Iterators
   
    // src iterator
    channel::const_iterator sItRow;  // row iterator
    channel::const_iterator eSItRow; // end row
    channel::const_iterator sItCol;  // column iterator
    channel::const_iterator eSItCol; // end column

    // dest iterator
    channel::iterator dItB=dest.begin(); // dest begin
    channel::iterator dItRow;        // row iterator
    channel::iterator dItCol;        // column iterator

    // subregion
    channel::const_iterator sRItRow;  // row iterator
    channel::const_iterator eSRItRow; // end row
    channel::const_iterator sRItCol;  // column iterator
    channel::const_iterator eSRItCol; // end column

    // window of pixel of interest
    channel::const_iterator wItRow;
    channel::const_iterator eWItRow;
    channel::const_iterator wItCol;
    channel::const_iterator eWItCol;

    // search window
    channel::const_iterator sWItRow;
    channel::const_iterator sWItCol;

    // kernel window
    matrix<float>::const_iterator kIt;
    matrix<float>::const_iterator kItB=kern.begin();

    // temp iterators
    channel::const_iterator tmp1, tmp2, tmp3, 
      tmp4, tmp5, tmp6, tmp7, tmp8;

    // -----------------------
    //    border conditions
    // -----------------------

    channel srcExp;  // src with boundary expanded

    boundaryExpansion expander(halfWindow,boundaryType,false);
    if (!expander.apply(src,srcExp)) {
      setStatusString(expander.getStatusString());
      return false;
    }

    // rows
    // begin of src row
    for(sItRow = srcExp.begin()+halfWindow+halfWindow*srcExp.columns(),
          dItRow = dItB, // begin of dest row 
          eSItRow = srcExp.begin() + halfWindow +
          (srcExp.rows()-halfWindow)*srcExp.columns(); // end point in rows
        eSItRow != sItRow;     // end condition in rows
        sItRow +=srcExp.columns(),dItRow+=dest.columns()) {// increment in rows
      // columns
      for(sItCol = sItRow,      // begin of src column 
            dItCol= dItRow,     // begin of dest column
            eSItCol = sItRow+srcExp.columns()-sizeOfWindow+1; // end point cols
          eSItCol != sItCol;    // end condition in colums
          ++sItCol, ++dItCol) { // increment in rows

        average = 0.0;
        totalWeight = 0.0;
        weight = 0.0;
        weightMax = 0.0;
        d = 0.0f;

        // cicle for search windows throw subregions in image

        // limits in subregion, top row 
        tmp1= sItCol-halfSubregion*srcExp.columns();
        tmp2= srcExp.begin()+halfWindow*srcExp.columns() +
          (sItCol-srcExp.begin())%srcExp.columns(); // min row

        // limits in subregion, bottom row
        tmp3= sItCol+halfSubregion*srcExp.columns();
        // max row
        tmp4= srcExp.begin() +
          (srcExp.rows() - halfWindow)*srcExp.columns()+
          (sItCol-srcExp.begin())%srcExp.columns(); 

        for(sRItRow = lti::max(tmp1,tmp2),eSRItRow = lti::min(tmp3,tmp4);
            eSRItRow != sRItRow;             
            sRItRow +=srcExp.columns()) {

          // limits in subregion, left column

          // min column
          tmp5 = sRItRow-(sRItRow-srcExp.begin())%srcExp.columns()+halfWindow;
          tmp6 = sRItRow-halfSubregion;

          // limits in subregion, right column
          tmp7 = sRItRow-(sRItRow-srcExp.begin())%srcExp.columns()+
            srcExp.columns()-halfWindow; // max column

          tmp8 = sRItRow+halfSubregion;

          for(sRItCol = lti::max(tmp5,tmp6),eSRItCol = lti::min(tmp7,tmp8); 
              eSRItCol != sRItCol; 
              ++sRItCol ) {

            if(sItCol== sRItCol) continue;

            d = 0.0f; // initialize euclidean distance

            //it calculates the euclidean distance between two windows

            // start row in window 
            for(wItRow = sItCol -halfWindow*srcExp.columns()-halfWindow ,  
                  // start row in search window
                  sWItRow = sRItCol-halfWindow*srcExp.columns()-halfWindow,
                  kIt = kItB,               // start kernel
                  eWItRow = wItRow+sizeOfWindow*srcExp.columns();  // end row
                eWItRow != wItRow;  // end condition
                wItRow += srcExp.columns(),
                  sWItRow += srcExp.columns()) { // increment rows

              // start column window, search window, kernel
              for (wItCol = wItRow, sWItCol = sWItRow,
                     eWItCol= wItRow+sizeOfWindow;    // end column in window
                   eWItCol != wItCol;                 // end condition
                   ++wItCol, ++sWItCol, ++kIt) {   // increment column

                d += (*kIt)*sqr(*wItCol-*sWItCol); 
              }
            }
            weight = exp(-d/filterDegree);
            if (weight>weightMax) {
              weightMax = weight; // find weight max
            }

            totalWeight += weight;                   // compute total weight
            average  += weight*(*sRItCol);           // compute the average
          }
        } 

        totalWeight += weightMax;         // add max weight to total weigth
        average  += weightMax*(*sItCol);  // add max weight to average

        if (totalWeight>0.0f) { // to prevent the zero division
          *dItCol = (average/totalWeight);
        }
        else {
          *dItCol = *sItCol; 
        }
      }
    }
    return true;
  }

  // On place apply for type channel!
  bool nonLocalMeansDenoising::apply(channel& srcdest) const {

    channel tmp;
    if (apply(srcdest,tmp)) {
      tmp.detach(srcdest);
      return true;
    }
    return false;
  }

  // On place apply for type channel8!
  bool nonLocalMeansDenoising::apply(channel8& srcdest) const {

    channel8 tmp;
    if (apply(srcdest,tmp)) {
      tmp.detach(srcdest); // faster than srcdest.copy(tmp);
      return true;
    }
    return false;
  }

  // Denoise src with the NL-means and gives the result to dest for type
  // channel!
  bool nonLocalMeansDenoising::apply(const channel& src, channel& dest) const {
    return denoise(src,dest);
  }

  // Denoise src with the NL-means and gives the result to dest for type
  // channel8!
  bool nonLocalMeansDenoising::apply(const channel8& src, channel8& dest) const {
    
    channel srcTmp, destTmp;
    srcTmp.castFrom(src);
    if (denoise(srcTmp,destTmp)) {
      dest.castFrom(destTmp);
      return true;
    }
    return false;
  }
}

