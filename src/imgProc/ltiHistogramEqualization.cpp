/*
 * Copyright (C) 1998-2006
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


/*
 * \file   ltiHistogramEqualization.cpp
 *         Contains a class for histogram equalization of different types of
 *         matrices
 * \author Thomas Erger
 * \author Pablo Alvarado
 * \date   14.04.1999 (LTI-Lib 1)
 * \date   06.01.2006 (LTI-Lib 2)
 *
 * $Id: ltiHistogramEqualization.cpp,v 1.10 2010-04-25 05:27:35 alvarado Exp $
 */

#include "ltiHistogram.h"
#include "ltiMath.h"
#include "ltiVector.h"
#include "ltiHistogramEqualization.h"
#include "ltiTypeInfo.h"
#include "ltiRound.h"

namespace lti {
  // --------------------------------------------------
  // histogramEqualization::parameters
  // --------------------------------------------------

  // default constructor
  histogramEqualization::parameters::parameters()
    : contrastEnhancement::parameters() {

    lowerInputLimit  = 0.0f;
    lowerOutputLimit = 0.0f;
    cells            = 256;
    upperInputLimit  = 1.0f;
    upperOutputLimit = 1.0f;
  }

  // copy constructor
  histogramEqualization::parameters::parameters(const parameters& other)
    : contrastEnhancement::parameters()  {
    copy(other);
  }

  // destructor
  histogramEqualization::parameters::~parameters() {
  }

  const std::string& histogramEqualization::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // copy member
  histogramEqualization::parameters&
  histogramEqualization::parameters::copy(const parameters& other) {
    contrastEnhancement::parameters::copy(other);

    lowerInputLimit  = other.lowerInputLimit;
    lowerOutputLimit = other.lowerOutputLimit;
    cells            = other.cells;
    upperInputLimit  = other.upperInputLimit;
    upperOutputLimit = other.upperOutputLimit;

    return *this;
  }

  // copy member
  histogramEqualization::parameters&
  histogramEqualization::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  // clone member
  histogramEqualization::parameters* 
  histogramEqualization::parameters::clone() const {
    return new parameters(*this);
  }

  // new instance
  histogramEqualization::parameters*
  histogramEqualization::parameters::newInstance() const {
    return new parameters();
  }

  /*
   * write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool histogramEqualization::parameters::write(ioHandler& handler,
                                                const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }
    
    if (b) {
      lti::write(handler,"lowerInputLimit",lowerInputLimit);
      lti::write(handler,"lowerOutputLimit",lowerOutputLimit);
      lti::write(handler,"cells",cells);
      lti::write(handler,"upperInputLimit",upperInputLimit);
      lti::write(handler,"upperOutputLimit",upperOutputLimit);
    }

    b = b && contrastEnhancement::parameters::write(handler,false);
    
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
  bool histogramEqualization::parameters::read(ioHandler& handler,
                                        const bool complete) {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {
      lti::read(handler,"lowerInputLimit",lowerInputLimit);
      lti::read(handler,"lowerOutputLimit",lowerOutputLimit);
      lti::read(handler,"cells",cells);
      lti::read(handler,"upperInputLimit",upperInputLimit);
      lti::read(handler,"upperOutputLimit",upperOutputLimit);
    }

    b = b && contrastEnhancement::parameters::read(handler,false);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

  // --------------------------------------------------
  // histogramEqualization
  // --------------------------------------------------

  // Default constructor
  histogramEqualization::histogramEqualization()
    : contrastEnhancement() {
    parameters defaultParams;
    setParameters(defaultParams);
  }

  // Constructor with parameters
  histogramEqualization::histogramEqualization(const parameters& params)
    : contrastEnhancement(),
      matrixProcessingInterface<ubyte>(),
      matrixProcessingInterface<float>() {
    setParameters(params);
  }

  // Copy constructor
  histogramEqualization::histogramEqualization(const histogramEqualization& o)
    : contrastEnhancement(),
      matrixProcessingInterface<ubyte>(),
      matrixProcessingInterface<float>() {
    copy(o);
  }

  // Destructor
  histogramEqualization::~histogramEqualization() {
  }

  // Name of the class
  const std::string& histogramEqualization::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // Copy member
  histogramEqualization&
  histogramEqualization::copy(const histogramEqualization& other) {
    contrastEnhancement::copy(other);
    return (*this);
  }

  // Clone member
  histogramEqualization* histogramEqualization::clone() const {
    return new histogramEqualization(*this);
  }

  // New instance
  histogramEqualization* histogramEqualization::newInstance() const {
    return new histogramEqualization();
  }

  // Return casted parameters
  const histogramEqualization::parameters&
  histogramEqualization::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&contrastEnhancement::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }

  bool histogramEqualization::updateParameters() {
    const parameters& par = getParameters();
    lower_ = par.lowerInputLimit;
    upper_ = par.upperInputLimit;
    cst_   = (par.cells-1)/(upper_-lower_);

    static const float norm =
      static_cast<float>(typeInfo<ubyte>::suggestedNorm());

    loweru_ = min(255,max(iround(lower_*norm),0));
    upperu_ = min(255,max(iround(upper_*norm),0));

    if (lower_<upper_) {
      return true;
    }

    setStatusString("Wrong parameters: lower >= upper input limits");
    return false;
  }

  // -------------------------------------------------------------------
  // The apply-methods!
  // -------------------------------------------------------------------


  // On place apply for type matrix<float>!
  bool histogramEqualization::apply(matrix<float>& srcdest) const {

    matrix<float> tmp;
    if (apply(srcdest,tmp)) {
      tmp.detach(srcdest);
      return true;
    }
    return false;
  }

  // On place apply for type matrix<float>8!
  bool histogramEqualization::apply(matrix<ubyte>& srcdest) const {
    matrix<ubyte> tmp;
    if (apply(srcdest,tmp)) {
      tmp.detach(srcdest); // srcdest.copy(tmp);
      return true;
    }

    return false;
  }

  // On place apply for type vector<float>!
  bool histogramEqualization::apply(vector<float>& srcdest) const {
    vector<float> tmp;
    if (apply(srcdest,tmp)) {
      tmp.detach(srcdest);
      return true;
    }
    
    return false;
  }

  // On place apply for type vector<ubyte>!
  bool histogramEqualization::apply(vector<ubyte>& srcdest) const {
    vector<ubyte> tmp;
    if (apply(srcdest,tmp)) {
      tmp.detach(srcdest);
      return true;
    }
    
    return false;
  }

  // On copy apply for type matrix<float>!
  bool histogramEqualization::computeLUT(const matrix<float>& src,
                                         vector<float>& lut) const {
    int y,i;

    const parameters& param = getParameters();
    
    if (upper_ <= lower_) {
      return false;
    }

    // create histogram of the src matrix<float>
    ivector histo(param.cells,0);
    vector<float>::const_iterator it,e; // iterators for the lines of src

    for (y=0;y<src.rows();y++) {
      e=src.getRow(y).end();
      for(it=src.getRow(y).begin();it!=e;++it) {
        if ((*it>=lower_) && (*it<=upper_)) { // check if value in input range
          // *cst because the cells of the histogram have even values, -1
          // because otherwise the term in brackets could become equal to
          // param.cells which would create an error
          histo.at(iround(((*it)-lower_)*cst_))++;
        }
      }
    }

    // generate cumulative histogram
    for(i=1;i<param.cells;i++) {
      histo.at(i) += histo.at(i-1);
    }

    const int numEntries = histo.at(histo.lastIdx());

    // generate look-up-table
    lut.allocate(param.cells);

    float ncst;
    if (numEntries > 0) {
      ncst=(param.upperOutputLimit-param.lowerOutputLimit)/numEntries;
    } else {
      ncst = 0.0f;
    }

    for(i=0;i<param.cells;i++) {
      // number of gray levels: cells1
      // the gray level saved at histoc.at(i)
      // divide by cst to get back to values in range 0.0-1.0
      lut.at(i)=static_cast<float>(param.lowerOutputLimit+ncst*histo.at(i));
    } // map the values to outputrange

    return true;
  }

  // On copy apply for type matrix<float>!
  bool histogramEqualization::apply(const matrix<float>& src,
                                          matrix<float>& dest) const {
    
    vector<float> lut;

    computeLUT(src,lut);

    dest.allocate(src.size());
    lti::matrix<float>::iterator dit;
    vector<float>::const_iterator it,e; // iterators for the lines of src
    int y;

    // we have two possibilities, where the first one is a little bit faster
    if (lower_== 0.0f) {
      // write into the destination matrix<float>
      for(dit=dest.begin(),y=0; y<src.rows(); ++y) {
        e=src.getRow(y).end();
        for(it=src.getRow(y).begin();it!=e;++dit,++it) {
          // compare whether value is to be altered
          if ((*it>=0.0f) && (*it<=upper_)) {
            // use look-up-table
            *dit=lut.at(iround((*it)*cst_));
          } else {
            // if value is not to be altered, it keeps its value
            *dit = *it;
          }
        }
      }
    } else {
      // write into the destination matrix<float>
      for(dit=dest.begin(),y=0; y<src.rows(); ++y) {
        e=src.getRow(y).end();
        for(it=src.getRow(y).begin();it!=e;++dit,++it) {
          // compare whether value is to be altered
          if ((*it>=lower_) && (*it<=upper_)) {
            // use look-up-table
            *dit=lut.at(iround((*it-lower_)*cst_));
          } else {
            // if value is not to be altered, it keeps its value
            *dit = *it;
          }
        }
      }
    }

    return true;
  }


  // On copy apply for type matrix<ubyte>!
  bool histogramEqualization::computeLUT(const matrix<ubyte>& src,
                                         vector<ubyte>& lut) const {

    int i,y;
    static const float norm =
      static_cast<float>(typeInfo<ubyte>::suggestedNorm());
    const float csts = cst_/norm;
    
    const parameters& param = getParameters();
   
    if (upperu_ <= loweru_) {
      setStatusString("Wrong input interval in parameters: lower>=upper!");
      return false;
    }

    ivector histo(param.cells,0);
    lti::vector<ubyte>::const_iterator it,e;

    // create histogram of the src matrix<float>

    
    ubyte histLut[256];
    for ( i=loweru_; i<=upperu_; i++ ) {
      histLut[i] = static_cast<ubyte>(0.5f + (i-loweru_)*csts);
    }

    // compute channel histogram
    for (y=0;y<src.rows();y++) {
      e=src.getRow(y).end();
      for(it=src.getRow(y).begin();it!=e;it++) {
        if ((*it>=loweru_) && (*it<=upperu_)) {
          histo.at(histLut[*it])++;
        }
      }
    }

    // generate cumulative histogram
    for(i=1;i<param.cells;i++) {
      histo.at(i) += histo.at(i-1);
    }

    const int numEntries = histo.at(histo.lastIdx());

    // no quantization, so lut can be used for all values
    lut.allocate(256);
    float ncst;
    if (numEntries > 0) {
      ncst = (param.upperOutputLimit-param.lowerOutputLimit)*norm/numEntries;
    } else {
      ncst = 0.0f;
    }

    // first part of the LUT (< lower)
    for(i=0;i<loweru_;i++) {
      lut[i] = i;
    }
    
    const int lol = iround(param.lowerOutputLimit*norm);
    int tmp;

    // apply the algorithm and afterwards map to output range    
    for(;i<=upperu_;++i) {
      tmp = iround(lol+ncst*histo.at(histLut[i]));
      if (tmp < 0) {
        lut.at(i)=0;
      } else if (tmp > 255) {
        lut.at(i)=255;
      } else {
        lut.at(i) = tmp;
      }
    }

    // no quantization, so lut can be used for all values
    for(;i<256;i++) {
      lut.at(i) = i;
    }

    return true;
  }

  bool histogramEqualization::apply(const matrix<ubyte>& src,
                                          matrix<ubyte>& dest) const {


    vector<ubyte> lut;
    computeLUT(src,lut);

    // write information into the destination matrix<ubyte>
    lti::matrix<ubyte>::iterator dit;
    lti::vector<ubyte>::const_iterator it,e;
    int y;
    dest.allocate(src.size());

    for(dit=dest.begin(),y=0; y<src.rows(); ++y) {
      e=src.getRow(y).end();
      for(it=src.getRow(y).begin();it!=e;++dit,++it) {
        *dit = lut.at(*it);
      }
    }

    return true;
  }


  // On copy apply for type vector<float>!
  bool histogramEqualization::apply(const vector<float>& src,
                                          vector<float>& dest) const {
    matrix<float> tmpsrc;
    matrix<float> tmpdest;

    //quite bad code here, because of const input, to be changed
    //the src vector will not be changed,
    tmpsrc.useExternData(1,src.size(),const_cast<float*>(src.data()));

    apply(tmpsrc,tmpdest);

    dest.copy(tmpdest.getRow(0));

    return true;
  }

  // On copy apply for type vector<ubyte>!
  bool histogramEqualization::apply(const vector<ubyte>& src,
                                          vector<ubyte>& dest) const {

      matrix<ubyte> tmpsrc;
      matrix<ubyte> tmpdest;

      // here as well, for the same reason, to be changed
      tmpsrc.useExternData(1,src.size(),const_cast<ubyte*>(src.data()));

      apply(tmpsrc,tmpdest);

      dest.copy(tmpdest.getRow(0));

      return true;
  }

  // The following methods are necessary to fulfill the interface for
  // contrast enhancement algorithms

  // On place apply for type matrix<float>!
  bool histogramEqualization::apply(channel& srcdest) const {
    return apply(static_cast<fmatrix&>(srcdest));
  }

  bool histogramEqualization::apply(channel8& srcdest) const {
    return apply(static_cast<matrix<ubyte>&>(srcdest));
  }

  bool histogramEqualization::apply(const channel8& src,channel8& dest) const {
    return apply(static_cast<const matrix<ubyte>&>(src),
                 static_cast<matrix<ubyte>&>(dest));
  }

  bool histogramEqualization::apply(const channel& src,channel& dest) const {
    return apply(static_cast<const fmatrix&>(src),
                 static_cast<fmatrix&>(dest));
  }
}
