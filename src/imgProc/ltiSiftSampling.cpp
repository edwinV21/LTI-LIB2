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
 * \file   ltiSiftSampling.cpp
 *         Contains the class siftSampling.
 * \author Arnd Hannemann
 * \date   5.1.2005
 *
 * revisions ..: $Id: ltiSiftSampling.cpp,v 1.11 2012-09-16 05:22:59 alvarado Exp $
 */


#include "ltiSiftSampling.h"

#undef  _LTI_DEBUG
//#define _LTI_DEBUG 3
#include "ltiDebug.h"

#include "ltiConstants.h"
#include "ltiFactory.h"

#if defined(_LTI_DEBUG) && (_LTI_DEBUG > 2)
#include "ltiViewer.h"
#endif

namespace lti {
  //register in localSampling factory
  _LTI_REGISTER_IN_FACTORY(localSampling,siftSampling)

  // --------------------------------------------------
  // siftSampling::parameters
  // --------------------------------------------------

  // default constructor
  siftSampling::parameters::parameters()
    : localSampling::parameters() {
    binsPerDimension = 3;
    binSize = 2;
    variance = -1;
    filter = true;
  }

  // copy constructor
  siftSampling::parameters::parameters(const parameters& other)
    : localSampling::parameters() {
    copy(other);
  }

  // destructor
  siftSampling::parameters::~parameters() {
  }

  // copy member

  siftSampling::parameters&
  siftSampling::parameters::copy(const parameters& other) {
    localSampling::parameters::copy(other);

    
      binsPerDimension = other.binsPerDimension;
      binSize = other.binSize;
      filter = other.filter;
      variance = other.variance;

    return *this;
  }

  // alias for copy method
  siftSampling::parameters&
  siftSampling::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  // class name
  const std::string& siftSampling::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone method
  siftSampling::parameters* siftSampling::parameters::clone() const {
    return new parameters(*this);
  }

  // new instance
  siftSampling::parameters* siftSampling::parameters::newInstance() const {
    return new parameters();
  }

  /*
   * Write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool siftSampling::parameters::write(ioHandler& handler,
                                         const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {
      
      lti::write(handler,"binsPerDimension",binsPerDimension);
      lti::write(handler,"binSize",binSize);
      lti::write(handler,"filter",filter);
      lti::write(handler,"variance",variance);
    }

    b = b && localSampling::parameters::write(handler,false);

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
  bool siftSampling::parameters::read(ioHandler& handler,
                                        const bool complete)  {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {
      
      lti::read(handler,"binsPerDimension",binsPerDimension);
      lti::read(handler,"binSize",binSize);
      lti::read(handler,"filter",filter);
      lti::read(handler,"variance",variance);
    }

    b = b && localSampling::parameters::read(handler,false);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

  // --------------------------------------------------
  // siftSampling
  // --------------------------------------------------
  
  // Constant for Not Available
  const int siftSampling::NA_ = -1;

  // default constructor
  siftSampling::siftSampling()
    : localSampling() {
 
    //TODO: comment the attributes of your functor
    // If you add more attributes manually, do not forget to do following:
    // 1. indicate in the default constructor the default values
    // 2. make sure that the copy member also copy your new attributes, or
    //    to ensure there, that these attributes are properly initialized.

    // create an instance of the parameters with the default values
    parameters defaultParameters;
    // set the default parameters
    setParameters(defaultParameters);
  }

  // default constructor
  siftSampling::siftSampling(const parameters& par)
    : localSampling() {

    //TODO: comment the attributes of your functor
    // If you add more attributes manually, do not forget to do following:
    // 1. indicate in the default constructor the default values
    // 2. make sure that the copy member also copy your new attributes, or
    //    to ensure there, that these attributes are properly initialized.

    // set the given parameters
    setParameters(par);
  }


  // copy constructor
  siftSampling::siftSampling(const siftSampling& other)
    : localSampling() {
    copy(other);
  }

  // destructor
  siftSampling::~siftSampling() {
  }

  // copy member
  siftSampling& 
  siftSampling::copy(const siftSampling& other) {
    localSampling::copy(other);

    //TODO: comment the attributes of your functor
    // If you add more attributes manually, do not forget to do following:
    // 1. indicate in the default constructor the default values
    // 2. make sure that the copy member also copy your new attributes, or
    //    to ensure there, that these attributes are properly initialized.

    return (*this);
  }

  // alias for copy member
  siftSampling&
  siftSampling::operator=(const siftSampling& other) {
    return (copy(other));
  }

  // class name
  const std::string& siftSampling::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone member
  siftSampling* siftSampling::clone() const {
    return new siftSampling(*this);
  }

  // create a new instance
  siftSampling* siftSampling::newInstance() const {
    return new siftSampling();
  }

  // return parameters
  const siftSampling::parameters&
  siftSampling::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }


  // return number of regions
  int siftSampling::getNumberOfRegions() const {
    const parameters& par = getParameters();
    return par.binsPerDimension*par.binsPerDimension;
  }

  // return result order
  localSampling::eResultOrder siftSampling::getResultOrder() const {
    return Sequential;
  }

  // update parameters
  bool siftSampling::updateParameters() {
    const parameters& par = getParameters();
    _lti_debug("updating parameters...\n");


    
    // shortcuts
    const int size = par.binSize; //*par.binsPerDimension;
    const int size2 = size*size;
    // half of size
    const int hsize = size/2;

    // size of complete region
    const int bsize = par.binsPerDimension * size;
    // half of that
    const int bhsize = bsize /2;

    // acus per row
    const int bins = par.binsPerDimension;

    accu1_.resize(0,0,bsize-1,bsize-1,NA_,Init);
    accu2_ = accu1_; accu3_ = accu1_; accu4_ = accu1_;

    _lti_debug("  creating masks for accumulators...");
    // creating mask for accumulators
    for (int i=0; i<bsize; i++) {
      const int colaccu = static_cast<int>((i-hsize)/size) * bins;
      const int colaccu2 = static_cast<int>((i+hsize)/size) * bins;
      // upper
      if (i >= hsize) {
        for (int j=hsize; j<bsize+hsize; j++) {
          const int rowaccu = static_cast<int>((j-hsize)/size);
          const int val = rowaccu + colaccu;
          // upper left
          if (j < bsize) {
            accu1_.at(i,j) = val;
          }
          // upper right
          if (j >= size) {
            accu2_.at(i,j-size) = val;
          }
        }
      }  
      // lower
      if (i < bsize-hsize) {
        for (int j=hsize; j<bsize+hsize; j++) {
          const int rowaccu = static_cast<int>((j-hsize)/size);
          const int val = rowaccu + colaccu2;
          // lower left
          if (j < bsize) {
            accu3_.at(i,j) = val;
          }
          // lower right
          if (j >= size) {
            accu4_.at(i,j-size) = val;
          }
        }
      }
    }
    _lti_debug("done\n");
    ipoint offset;
    offset.x = bhsize;
    offset.y = bhsize;
    _lti_debug("  bhsize is: "<<bhsize<<"\n");
    _lti_debug("  offset is: "<<offset<<"\n");
    accu1_.setOffset(offset);
    accu2_.setOffset(offset);
    accu3_.setOffset(offset);
    accu4_.setOffset(offset);
    _lti_debug("  firstrow: "<<accu1_.firstRow()<<"\n");
    _lti_debug("  firstcol: "<<accu1_.firstColumn()<<"\n");

#if defined(_LTI_DEBUG) && (_LTI_DEBUG>2)
    static viewer v;
#endif

    _lti_debug("accu1:\n"<<accu1_<<"\n");
#if defined(_LTI_DEBUG) && (_LTI_DEBUG>2)
    v.show(accu1_);
    v.waitButtonPressed();
#endif
    _lti_debug("accu2:\n"<<accu2_<<"\n");
    _lti_debug("accu3:\n"<<accu3_<<"\n");
    _lti_debug("accu4:\n"<<accu4_<<"\n"); 

    
    fmatrix mask1,mask2,mask3,mask4,mask5;
    mask1.resize(size,size);
    mask2.resize(size,size);
    mask3.resize(size,size);
    mask4.resize(size,size);

    mask1_.resize(0,0,bsize-1,bsize-1,0,Init);
    mask2_ = mask1_;
    mask3_ = mask1_;
    mask4_ = mask1_;
    
    _lti_debug("  computing small masks for weight function...");
    // compute mask for one segment first
    for (int i=0; i<size; i++) {
      for (int j=0; j<size; j++) {
        const float buf = 
          static_cast<float>((size-0.5-i) * (size-0.5-j) / size2);
        mask1.at(i,j) = buf;
        mask2.at(i,size-1-j) = buf;
        mask3.at(size-1-i,j) = buf;
        mask4.at(size-1-i,size-1-j) = buf;
      }
    }
    _lti_debug("done\n");

    _lti_debug("mask1:\n"<<mask1<<"\n");
    _lti_debug("mask2:\n"<<mask2<<"\n");
    _lti_debug("mask3:\n"<<mask3<<"\n"); 
    _lti_debug("mask4:\n"<<mask4<<"\n");  

    _lti_debug("  creating big mask for weight function...");
    //copy to big masks
    for (int i=0; i<bsize; i++) {
      int bufi = (i + hsize) % size;
      for (int j=0; j<bsize; j++) {
        int bufj = (j + hsize) % size;
        mask1_.at(i,j) = mask1.at(bufi,bufj);
        mask2_.at(i,j) = mask2.at(bufi,bufj);
        mask3_.at(i,j) = mask3.at(bufi,bufj);
        mask4_.at(i,j) = mask4.at(bufi,bufj);
      }
    }
    mask1_.setOffset(offset);
    mask2_.setOffset(offset);
    mask3_.setOffset(offset);
    mask4_.setOffset(offset);
    _lti_debug("done\n");

    _lti_debug("mask1_:\n"<<mask1_<<"\n");
    _lti_debug("mask2_:\n"<<mask2_<<"\n");
    _lti_debug("mask3_:\n"<<mask3_<<"\n"); 
    _lti_debug("mask4_:\n"<<mask4_<<"\n");  
    
    if (par.filter) {
      _lti_debug("  creating gaussian filter...");
      vector<float> filter(bsize);
      matrix<float> filter2D(bsize,bsize);
      //      kernel2D<float> kern2;
      double var = par.variance;
      // calculate the variance such that the element at (size/2) is
      // the element at 0 divided by (1+int(size/2))
      if (var<0) {
        int n = bhsize;
        if (n<1) {
          n = 1;
        }
        // log == ln (natural logarithm)
        var = (n*n)/(2*log(static_cast<double>(n+1)));
      }

      const double factor = 1.0/sqrt(2*(constants<double>::pi())*var);
      double accu = 0;

      double snorm = static_cast<double>(typeInfo<double>::suggestedNorm());

      double gpos;
      for (int i=0; i<bsize; i++) {
        gpos = static_cast<double>(i)-bhsize+0.5;
        gpos *= -gpos;
        //        _lti_debug("gpos:"<<gpos<<"\n");
        const float val = static_cast<float>(snorm*factor*exp(gpos/(2*var)));
        filter.at(i) = val;
        accu += val;
      }
      accu /= snorm;
      // make sure the sum of all elements is one!      
      for (int i=0; i<bsize; i++) {
        filter.at(i) = static_cast<float>(filter.at(i)/accu);
      }

      // create 2D filter
      for (int i=0; i<bsize; i++) {
        filter2D.getRow(i).multiply(filter,filter.at(i));
      }
      _lti_debug("\n filter1d:\n"<<filter<<"\n");
      _lti_debug("\n filter2d:\n"<<filter2D<<"\n");
      _lti_debug("done\n");

      _lti_debug("  computing weighting function with gaussian...");
      mask1_.emultiply(filter2D);
      mask2_.emultiply(filter2D);
      mask3_.emultiply(filter2D);
      mask4_.emultiply(filter2D);
      _lti_debug("done\n");
    }
                 
    
    _lti_debug("  creating mask for normalization...");
    norm_.resize(0,bsize-1,0,Init);    
    for (int i=0;i<bsize;i++) {
      norm_.at(i) = (i-bhsize+0.5f)/bhsize;
    }
    norm_.setOffset(bhsize);
    _lti_debug("done\n");
    _lti_debug("norm:\n"<<norm_<<"\n");


/*    
    mask5 = mask1_;
    mask5.add(mask2_);
    mask5.add(mask3_);
    mask5.add(mask4_);
*/
    _lti_debug("mask1_:\n"<<mask1_<<"\n");
    _lti_debug("mask2_:\n"<<mask2_<<"\n");
    _lti_debug("mask3_:\n"<<mask3_<<"\n"); 
    _lti_debug("mask4_:\n"<<mask4_<<"\n");  

    //calculate radius for bounding check 
    //perhaps this could be optimized to include location angle
    rad_ = sqrt(static_cast<float>(2*bhsize*bhsize));
    _lti_debug("radius is: "<<rad_<<"\n");

    return true;
  }

  

  // -------------------------------------------------------------------
  // The apply-methods: see ltiSiftSampling_template.h
  // -------------------------------------------------------------------


}

#include "ltiUndebug.h"

