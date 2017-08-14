/*
 * Copyright (C) 2009
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
 * \file   ltiFastAreaDescription.cpp
 *         Contains the class lti::fastAreaDescription, 
 *         which efficiently computes area descriptors of labeled masks.
 *
 * \author Dorothee Finck
 * \date   11.6.2003
 * \author Pablo Alvarado
 * \date   18.10.2009
 *
 * revisions ..: $Id: ltiFastAreaDescription.cpp,v 1.4 2013-01-17 16:30:31 alvarado Exp $
 */

#include "ltiFastAreaDescription.h"
#include "ltiSort2.h"
#include "ltiSort.h"

#include <limits>

#undef _LTI_DEBUG
// #define _LTI_DEBUG 1
#include "ltiDebug.h"


namespace lti {
  // --------------------------------------------------
  // fastAreaDescription::parameters
  // --------------------------------------------------

  // default constructor
  fastAreaDescription::parameters::parameters()
    : fastRelabeling::parameters() {
    
    minimumDistance.set(0,0);
    mergeClose = false;
    nLargest = 0;
    minimumMergedObjectSize = 1;
  }

  // copy constructor
  fastAreaDescription::parameters::parameters(const parameters& other)
    : fastRelabeling::parameters() {
    copy(other);
  }

  // destructor
  fastAreaDescription::parameters::~parameters() {
  }

  // copy member

  fastAreaDescription::parameters&
  fastAreaDescription::parameters::copy(const parameters& other) {
    fastRelabeling::parameters::copy(other);

    
    minimumDistance = other.minimumDistance;
    mergeClose = other.mergeClose;
    nLargest = other.nLargest;
    minimumMergedObjectSize = other.minimumMergedObjectSize;

    return *this;
  }

  // alias for copy method
  fastAreaDescription::parameters&
  fastAreaDescription::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  // class name
  const std::string& fastAreaDescription::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone method
  fastAreaDescription::parameters*
  fastAreaDescription::parameters::clone() const {
    return new parameters(*this);
  }

  // new instance
  fastAreaDescription::parameters*
  fastAreaDescription::parameters::newInstance() const {
    return new parameters();
  }

  /*
   * Write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool fastAreaDescription::parameters::write(ioHandler& handler,
                                         const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {
      
      lti::write(handler,"minimumDistance",minimumDistance);
      lti::write(handler,"mergeClose",mergeClose);
      lti::write(handler,"nLargest",nLargest);
      lti::write(handler,"minimumMergedObjectSize",minimumMergedObjectSize);
    }

    b = b && fastRelabeling::parameters::write(handler,false);

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
  bool fastAreaDescription::parameters::read(ioHandler& handler,
                                        const bool complete)  {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {
      
      lti::read(handler,"minimumDistance",minimumDistance);
      lti::read(handler,"mergeClose",mergeClose);
      lti::read(handler,"nLargest",nLargest);
      lti::read(handler,"minimumMergedObjectSize",minimumMergedObjectSize);
    }

    b = b && fastRelabeling::parameters::read(handler,false);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

  // --------------------------------------------------
  // fastAreaDescription
  // --------------------------------------------------

  // default constructor
  fastAreaDescription::fastAreaDescription()
    : fastRelabeling() {

    // create an instance of the parameters with the default values
    parameters defaultParameters;
    // set the default parameters
    setParameters(defaultParameters);
  }

  // default constructor
  fastAreaDescription::fastAreaDescription(const parameters& par)
    : fastRelabeling() {

    // set the given parameters
    setParameters(par);
  }


  // copy constructor
  fastAreaDescription::fastAreaDescription(const fastAreaDescription& other)
    : fastRelabeling() {
    copy(other);
  }

  // destructor
  fastAreaDescription::~fastAreaDescription() {
  }

  // copy member
  fastAreaDescription& 
  fastAreaDescription::copy(const fastAreaDescription& other) {
    fastRelabeling::copy(other);

    return (*this);
  }

  // alias for copy member
  fastAreaDescription&
  fastAreaDescription::operator=(const fastAreaDescription& other) {
    return (copy(other));
  }

  // class name
  const std::string& fastAreaDescription::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone member
  fastAreaDescription* fastAreaDescription::clone() const {
    return new fastAreaDescription(*this);
  }

  // create a new instance
  fastAreaDescription* fastAreaDescription::newInstance() const {
    return new fastAreaDescription();
  }

  // return parameters
  const fastAreaDescription::parameters&
  fastAreaDescription::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }

  // -------------------------------------------------------------------
  // some helpers
  // -------------------------------------------------------------------
  bool fastAreaDescription::
  initRectangleVector(const int numLab,
                      const ipoint& size,
                      std::vector<irectangle>& objects) const {

    // initialize the min as max, and the max as min for correct 
    // bounding box detection.  
    const irectangle obj(size,ipoint(0,0));
    objects.clear();
    objects.resize(numLab,obj);
  
    return (numLab > 0);
  }

  bool fastAreaDescription::
  initAreaDescriptors(const int numLab,
                      std::vector<areaDescriptor>& objects) const {

    areaDescriptor obj;
    obj.reset();
    
    objects.clear();
    objects.resize(numLab,obj);
    
    return (numLab > 0);
  }

  bool fastAreaDescription::computeEquivLabels(const matrix<ubyte>& src, 
                                               imatrix& dest,
                                               ivector& objSize, 
                                               ivector& equivLabels,
                                               int& numObjects) const {

    ivector tmpEquivLabels;
    //this apply returns a partially labeled dest -> more efficient
    if ( !(fastRelabeling::partial(src, dest, tmpEquivLabels, numObjects) && 
           fastRelabeling::relabelWithArea(tmpEquivLabels,numObjects,dest,
                                           objSize) ) ) {
      return false;
    }
     
    // the background is counted as an object, we do not want this!!!
    // objSize[0] = 0;
      
    const parameters& par = getParameters(); 

    // sort labels, but do not relabel the image, process nBest
    // and suppress small objects
    if ( par.sortSize ) {
      if ( !sortLabels(par.minimumObjectSize,par.nLargest,
                       objSize,numObjects,equivLabels)) {
        return false;
      }
    } else {
      // no sorting desired
      int minSize = par.minimumObjectSize;

      // does the user wants the n-largest elements only?
      if (par.nLargest > 0) {
        if (par.nLargest > objSize.lastIdx()) {
          minSize = 1;
        } else {
          // find out which equivalent minSize would produce only the
          // n largest objects
          sort sorter(Descending);
          const int tmpBg = objSize.at(0);
          objSize.at(0)=std::numeric_limits<int>::max();
          ivector tmpSizes;
          sorter.apply(objSize,tmpSizes);
          objSize.at(0)=tmpBg;
          minSize = tmpSizes.at(min(tmpSizes.lastIdx(),
                                    par.nLargest));
        }
      }

      if (minSize > 1) {
        if (!suppress(minSize,objSize,numObjects,equivLabels)) {
          return false;
        }
      } else {
        // we need a dummy equivLabels
        equivLabels.allocate(numObjects);
        for (int i = 0;i<equivLabels.size();++i) {
          equivLabels.at(i)=i;
        }
      }

    }

    return true;
  }

  //----------------
  // merge functions
  //----------------
  template<class T>
  bool fastAreaDescription::merge(std::vector<T>& src,
                                  ivector& objSize,
                                  imatrix& dest) const {
    
    // Only first order merges are done (i.e. the fulfillment of 
    // the closeness for the found rectangles only
    
    const parameters& par = getParameters();

    // LUT for new labels
    ivector reindex;
    
    reindex.allocate(static_cast<int>(src.size()));
    for (int i=0;i<reindex.size();++i) {
      reindex.at(i)=i;
    }
    
    // always ignore the background at the first entry
    const unsigned int lastSrc = static_cast<unsigned int>(src.size())-1;
    for (unsigned int i=1;i<lastSrc;++i) { // compare all combinations
      for (unsigned int j=i+1;j<src.size();++j) {
        if (src[i].isClose(src[j],par.minimumDistance)) {
          _lti_debug("Labels " << i << " and " << j << " close enough!\n");
          // we have to merge label i with label j
          reindex.at(j) = static_cast<int>(i);
        } 
      }
    }

    // now we have to fix the reindex
    for (int i=1;i<reindex.size();++i) {
      if (reindex.at(i) != i) { // there is a lower equivalence
        // ok, seek for the lowest equivalent label
        int label = reindex.at(i);
        while (reindex.at(label) != label) {
          label = reindex.at(label);
        }
        _lti_debug("Merging label " << i << " and " << label << std::endl);
        reindex.at(i) = label;
        // label is the lowest equivalent one: merge the rectangles
        src[label].join(src[i]);
        objSize.at(label)+=objSize.at(i); // and increase the number of pixels
        objSize.at(i)=0;
      }
    }

    // the previous computed tables have holes!
    // we need to remove the holes, and maintain the rectangles consistent
    int lastLabel=1;
    int fixOffset=0;
    for (int i=1;i<reindex.size();++i) {
      if (reindex.at(i) == i) {
        reindex.at(i) = lastLabel;
        ++lastLabel;
      } else {
        reindex.at(i) = reindex.at(reindex.at(i));
        src.erase(src.begin()+i-fixOffset);

        // we have to simulate the erase for the ivector
        const int to = min(static_cast<int>(src.size()),objSize.lastIdx());
        int k;
        for (k=i-fixOffset;k<to;++k) {
          objSize.at(k)=objSize.at(k+1);
        }
        objSize.at(k)=0;
        fixOffset++;
      }
    }
    
    // eliminate all unused entries
    objSize.resize(static_cast<int>(src.size()));

    _lti_debug("Relabel : " << reindex << std::endl);
    _lti_debug("Obj Size: " << objSize << std::endl);

    // |-------------------------------------------------------------------|
    // |before we sort the objects, the user may want to remove some merged|
    // |objects.                                                           |
    // |-------------------------------------------------------------------|
    if (par.minimumMergedObjectSize > 1) {
      
      fixOffset=0;
      {
        int i=0;
        while (i<objSize.size()) {
          
          if ((objSize.at(i) < par.minimumMergedObjectSize) &&
              (objSize.at(i) > 0)) {
            
            const int label = i+fixOffset;
            // object needs removal
            
            _lti_debug("--------- " << std::endl);
            _lti_debug("Label " << label << " will be removed with " <<
                       objSize.at(i) << " pixels" << std::endl);
            
            // search all entries in the reindex that point to this
            // index i, and redirect them to the background
            for (int j=1;j<reindex.size();++j) {
              if (reindex.at(j) == label) {
                reindex.at(j) = 0;
              }
            }
            
            // pass the pixels to the background
            src[0].join(src[i]);
            src.erase(src.begin()+i);
            
            objSize.at(0)+=objSize.at(i);
            
            // we have to simulate the erase for the ivector
            const int to = objSize.lastIdx();
            int k;
            for (k=i;k<to;++k) {
              objSize.at(k)=objSize.at(k+1);
            }
            objSize.at(k)=0;
            fixOffset++;
            
            _lti_debug("  Relabel : " << reindex << std::endl);
            _lti_debug("  Obj Size: " << objSize << std::endl);
          } else {
            ++i;
          }
        }
        
      }
      
      // now we have to fix the label gaps produced by setting some of 
      // them to zero
      
      ivector flags(reindex.size(),0);
      for (int i=0;i<flags.size();++i) {
        flags.at(reindex.at(i))=1;
      }

      ivector acc(reindex.size());
      acc.at(0)=0;
      for (int i=0;i<flags.lastIdx();++i) {
        acc.at(i+1) = flags.at(i) + acc.at(i);
      }

      for (int i=0;i<reindex.size();++i) {
        reindex.at(i)=acc.at(reindex.at(i));
      }
    }

    // Is sorting expected?
    if (par.sortSize) {

      ivector labels;
      labels.allocate(objSize.size());
      int i;
      for (i=0;i<labels.size();++i) {
        labels.at(i)=i;
      }
      
      // avoid removing the background label from 0
      int bkgPix = objSize.at(0);
      objSize.at(0)=std::numeric_limits<int>::max();
      
      // sort the labels
      sort2 sorter(Descending); // descending order
      sorter.apply(objSize,labels);

      objSize.at(0)=bkgPix;

      // create an inverse LUT on the labels
      ivector ilabels(labels.size());
      for (i=0;i<ilabels.size();++i) {
        ilabels.at(labels.at(i))= i;
      }

      // now reassign the reindex to the new sorted labels
      for (i=1;i<reindex.size();++i) {
        reindex.at(i)=ilabels.at(reindex.at(i));
      }
      
      // we have to re-sort the objects
      std::vector<T> tmpSrc(src.size());
      for (unsigned int ui=0;ui<src.size();++ui) {
        tmpSrc[ui]=src[labels[ui]];
      }
      src.swap(tmpSrc);
    }

    // relabel
    imatrix::iterator it;
    const imatrix::iterator eit=dest.end();
    for (it=dest.begin();it!=eit;++it) {
      *it = reindex.at(*it);
    }

    return true;
  }

  // -------------------------------------------------------------------
  // The apply-methods!
  // -------------------------------------------------------------------

  bool fastAreaDescription::apply(const matrix<ubyte> & src,
                                  imatrix& dest,
                                  std::vector<irectangle>& objects) const {
    
    int numObjects;
    ivector objSize;
    ivector equivLabels;
    
    if ( !computeEquivLabels( src, dest, objSize, equivLabels, numObjects ) ||
         !initRectangleVector( numObjects, dest.size(), objects ) ) {
      return false;
    }

    const int numRows (dest.rows());
    const int numCols (dest.columns());
    
    for(int y=0; y<numRows; ++y) {
      for (int x=0 ; x<numCols; ++x) {
        int& px = dest.at(y,x);
        const int index ( equivLabels.at(px) );
        px = index; //relabel the image
        if(y < objects[index].ul.y) {
          objects[index].ul.y = y;
        }
        if(y > objects[index].br.y) {
          objects[index].br.y = y;
        }
        
        if(x < objects[index].ul.x) {
          objects[index].ul.x = x;
        }
        if(x > objects[index].br.x) {
          objects[index].br.x = x;
        }
      }
    }
  
    const parameters& par = getParameters();
    if ( par.mergeClose ) {
      merge(objects,objSize,dest);
    }
    
    return true;
  }

  bool fastAreaDescription::apply(const matrix<ubyte> & src, 
                                  std::vector<irectangle>& objects) const {
    imatrix dest;
    return apply(src,dest,objects);
  }

  bool fastAreaDescription::apply(const matrix<ubyte> & src, 
                                  imatrix& dest,
                                  std::vector<areaDescriptor>& objects) const {
    int numObjects;
    ivector objSize;
    ivector equivLabels;

    if ( !computeEquivLabels( src, dest, objSize, equivLabels, numObjects ) ||
         !initAreaDescriptors( numObjects, objects ) ) {
      return false;
    }

    const int numRows (dest.rows());
    const int numCols (dest.columns());
    
    for(int y=0; y<numRows; ++y) {
      for (int x=0 ; x<numCols; ++x) {
        int& px = dest.at(y,x);
        const int index ( equivLabels.at(px) );
        px = index; //relabel the image

        objects[index].consider(x,y);
      }
    }
  
    const parameters& par = getParameters();
    if ( par.mergeClose ) {
      merge(objects,objSize,dest);
    }

    // normalize the results
    std::vector<areaDescriptor>::iterator it;
    const std::vector<areaDescriptor>::iterator eit = objects.end();
    for (it=objects.begin();it!=eit;++it) {
      (*it).normalizeCOG();
    }
    
    return true;
  }

  bool fastAreaDescription::apply(const matrix<ubyte> & src, 
                                  std::vector<areaDescriptor>& objects) const {

    imatrix dest;
    return apply(src,dest,objects);
  }


}

