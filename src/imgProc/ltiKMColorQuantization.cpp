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

/**
 * \file   ltiKMColorQuantization.cpp
 *         Contains a class for color quantization using the k-Means algorithm.
 * \author Pablo Alvarado
 * \date   23.04.1999 (LTI-Lib 1)
 * \date   12.01.2006 (LTI-Lib 2)
 *
 * $Id: ltiKMColorQuantization.cpp,v 1.8 2007-10-08 20:19:47 alvarado Exp $
 */


#include "ltiMacroSymbols.h"
#include "ltiKMColorQuantization.h"
#include "ltiSort2.h"

#include <set>
#include <cstdlib>

// ---------------------------------------------------------------------------
// Some Debuggin Macros

#undef _LTI_DEBUG
// #define _LTI_DEBUG 4
#include "ltiDebug.h"

// end of debugging macro definitions
// ---------------------------------------------------------------------------


namespace lti {

  // --------------------------------------------------
  // kMColorQuantization::parameters
  // --------------------------------------------------

  // default constructor
  kMColorQuantization::parameters::parameters()
    : colorQuantization::parameters() {

    maximalNumberOfIterations = int(50);
    thresholdDeltaPalette = 0.2f;
  }

  // copy constructor
  kMColorQuantization::parameters::parameters(const parameters& other)
    : colorQuantization::parameters()  {
    copy(other);
  }

  // destructor
  kMColorQuantization::parameters::~parameters() {
  }

  const std::string&  kMColorQuantization::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone member
  kMColorQuantization::parameters*
  kMColorQuantization::parameters::clone() const {
    return new parameters(*this);
  }
  
  // new instance member
  kMColorQuantization::parameters* 
  kMColorQuantization::parameters::newInstance() const {
    return new parameters();
  }

  // copy member
  kMColorQuantization::parameters&
  kMColorQuantization::parameters::copy(const parameters& other) {
    colorQuantization::parameters::copy(other);
    
    maximalNumberOfIterations = other.maximalNumberOfIterations;
    thresholdDeltaPalette     = other.thresholdDeltaPalette;
    
    return *this;
  }
  
  // alias for copy member
  kMColorQuantization::parameters&
  kMColorQuantization::parameters::operator=(const parameters& other) {
    return copy(other);
  }


  /*
   * write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool kMColorQuantization::parameters::write(ioHandler& handler,
                                         const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }
    
    b = b && colorQuantization::parameters::write(handler,false);

    if (b) {

      lti::write(handler,"maximalNumberOfIterations",
                 maximalNumberOfIterations);
      lti::write(handler,"thresholdDeltaPalette",
		 thresholdDeltaPalette);
    }

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
  bool kMColorQuantization::parameters::read(ioHandler& handler,
                                             const bool complete) {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    b = b && colorQuantization::parameters::read(handler,false);
    
    if (b) {
      
      lti::read(handler,"maximalNumberOfIterations",
                maximalNumberOfIterations);
      lti::read(handler,"thresholdDeltaPalette",
		thresholdDeltaPalette);
    }


    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

  // -----------------------------------------------------------------
  // kMColorQuantization
  // -----------------------------------------------------------------

  kMColorQuantization::kMColorQuantization()
    : colorQuantization() {
    parameters defaultParameters;
    setParameters(defaultParameters);
    
    srand(1);// fixed seed for random number generation allows a deterministic
             // behaviour of the algorithm, which allows optimization.
  }

  kMColorQuantization::kMColorQuantization(const parameters& par)
    : colorQuantization() {
    setParameters(par);

    srand(1);// fixed seed for random number generation allows a deterministic
             // behaviour of the algorithm, which allows optimization.
  }

  kMColorQuantization::kMColorQuantization(const kMColorQuantization& other)
    : colorQuantization()  {
    copy(other);

    srand(1);// fixed seed for random number generation allows a deterministic
             // behaviour of the algorithm, which allows optimization.
  }

  kMColorQuantization::~kMColorQuantization() {
  }

  // Name of the class
  const std::string& kMColorQuantization::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // copy data of "other" functor.
  kMColorQuantization&
  kMColorQuantization::copy(const kMColorQuantization& other) {
    colorQuantization::copy(other);
    return (*this);
  }
  
  kMColorQuantization* kMColorQuantization::clone() const {
    return new kMColorQuantization(*this);
  }

  kMColorQuantization* kMColorQuantization::newInstance() const {
    return new kMColorQuantization();
  }

  // returns the current parameters
  const kMColorQuantization::parameters&
  kMColorQuantization::getParameters() const {
    const parameters* params =
      dynamic_cast<const parameters*>(&colorQuantization::getParameters());
    if(isNull(params)) {
      throw invalidParametersException(name());
    }
    return *params;
  }


  bool kMColorQuantization::apply(const image& src,
                                        matrix<ubyte>& dest,
                                        palette& thePalette) const {

    const parameters& param = getParameters();

    if (param.numberOfColors <= 256) {
      matrix<int> tmp;
      bool result = apply(src,tmp,thePalette);
      dest.castFrom(tmp);
      return result;
    } else {
      setStatusString("8-bit mask can represent a maximum of 256 colors");
      return false;
    }
  }

  bool kMColorQuantization::apply(const image& src,
                                  matrix<int>& dest,
                                  palette& thePalette) const {

    const parameters& param = getParameters();
    kMeanColor kMeans(param.numberOfColors,
		      param.maximalNumberOfIterations,
		      param.thresholdDeltaPalette);
    return kMeans(src,dest,thePalette);
  }

  bool kMColorQuantization::apply(const image& src,
                                  image& dest) const {
    return colorQuantization::apply(src,dest);
  }

  bool kMColorQuantization::apply(image& srcdest) const {
    return colorQuantization::apply(srcdest);
  }
  
  // --------------------------------------------------------------------------
  // kMColorQuantization::kMeanColor
  // --------------------------------------------------------------------------

  const int kMColorQuantization::kMeanColor::firstKeySize_ = 4096;
  
  kMColorQuantization::kMeanColor::kMeanColor(const int& maxNumOfClasses,
                                              const int& maxIterations,
					      const float& thresDeltaPal)
    : theHash_(0), maxNumberOfClasses_(maxNumOfClasses),
      maxNumberOfIterations_(maxIterations),
      thresholdDeltaPalette_(thresDeltaPal) {
  }

  kMColorQuantization::kMeanColor::~kMeanColor() {
  }


  bool kMColorQuantization::kMeanColor::operator()(const image& img,
                                                   matrix<int>& colorMap,
                                                   palette& thePalette) {
    
    if (img.empty()) {
      colorMap.clear();
      thePalette.clear();
      return true;
    }

    // find the clusters
    initialize(img);
    getInitialPalette(thePalette);
    iterate();

    // fill the colorMap
    int y,x;
    colorMap.allocate(img.size());
    for (y=0;y<img.rows();++y) {
      for (x=0;x<img.columns();++x) {
        colorMap.at(y,x) = at(img.at(y,x)).index;
      }
    }
    // fill the palette
    thePalette.allocate(centroids_.size());
    for (x=0;x<centroids_.size();++x) {
      centroids_.at(x).castTo(thePalette.at(x));
    }

    delete[] theHash_;
    theHash_ = 0;

    return true;
  }

  /**
   * put the given pixel in the hash table
   */
  inline kMColorQuantization::kMeanColor::hashEntry&
  kMColorQuantization::kMeanColor::at(const rgbaPixel& px) {
    const int key = px.getValue() & 0x00000FFF; // lower 12 bits
    const int secondkey = px.getValue() & 0x00FFF000; // upper 12 bits;
    return theHash_[key][secondkey];
  }

  /**
   * put the given pixel in the hash table
   */
  inline bool kMColorQuantization::kMeanColor::put(const rgbaPixel& px) {
    const int key = px.getValue() & 0x00000FFF; // lower 12 bits
    const int secondkey = px.getValue() & 0x00FFF000; // upper 12 bits;
    hashMapType::iterator it;
    it = theHash_[key].find(secondkey);
    if (it == theHash_[key].end()) {
      theHash_[key][secondkey] = hashEntry(-1,1); // idx=-1, cnt=1
      return true;
    }
    else {
      (*it).second.counter++;
      return false;
    }
  }

  void kMColorQuantization::kMeanColor::initialize(const image& src) {
    int y;

    _lti_debug("Creating hash table.\n");

    // create the the hash table
    delete[] theHash_;
    theHash_ = new hashMapType[firstKeySize_];
    realNumberOfClasses_ = 0;
    
    // insert the pixels in the hash table
    vector<rgbaPixel>::const_iterator it,eit;
    // for each row in the image
    for (y=0;y<src.rows();++y) {
      const vector<rgbaPixel>& vct = src.getRow(y);
      // for each col in the row
      for (it=vct.begin(),eit=vct.end();it!=eit;++it) {
        if (put(*it)) {
          // if newly added it was a new color never used before...
          realNumberOfClasses_++;
        }
      }
    }
  }

  rgbaPixel kMColorQuantization::kMeanColor::getAnImageColor() {
    int i,j,k(0);
    const int steps = static_cast<int>(1.5+(random()*7.0));
    j = lastHashPosition_;
    rgbaPixel px;

    hashMapType::iterator it;
    i=(j+1)%firstKeySize_;

    // for each entry in the array of maps of hashEntries...
    it=theHash_[i].begin();
    while ((i!=j) && (k < steps)) {
      if (it==theHash_[i].end()) {
        i++;
        it=theHash_[i].begin();
      } else {
        ++it;
        if (it != theHash_[i].end()) {
          k++;
        }
      }
    }

    if (k >= steps) {
      // find which centroid corresponds to this entry
      const uint32 val = static_cast<uint32>( (*it).first | i);
      px = rgbaPixel(val);
    }

    lastHashPosition_=(i+1)%firstKeySize_;
    return px;
  }

  void
  kMColorQuantization::kMeanColor::getInitialPalette(const 
                                                     palette& thePalette) {
    // initialize with black
    const rgbaPixel black(0,0,0,0);
    centroids_.resize(min(maxNumberOfClasses_,realNumberOfClasses_),
                      black,Init);
    centerElems_.allocate(centroids_.size());

    _lti_debug("Initial Palette with "<< centroids_.size() <<
               " colors from " << realNumberOfClasses_ << std::endl);

    int i,j,k,kk(0);
    int idx;
    frgbPixel px;
    bool allEntriesUsed = true;
    ivector centrIndex(centerElems_.size());
    ivector tmpCenterElems;
    sort2 sorter(Descending); // sort in descending order

    if (maxNumberOfClasses_ < realNumberOfClasses_) {
      // initialize the centroids with palette and gray values
      int palSize = thePalette.size();
      int centSize = centroids_.size();
      int greyValues = centSize-palSize;
      
      // if there is a palette(size>0), init the centroids
      for (k=0;k<min(centSize,palSize);++k) {
        centroids_.at(k) = thePalette.at(k);
      }

      // init the "rest"-centroids with grey-values
      if (greyValues!=1) {
        for (;k<centroids_.size();++k) {
          const float val = (k-palSize)*255.0f/(greyValues-1);
          centroids_.at(k) = frgbPixel(val,val,val);
        }
      }
      else { // only one greyvalue entry
        const float val = 255.0f/2.0;
        centroids_.at(k) = frgbPixel(val,val,val);
      }
      j = centroids_.size(); // do not take the values from the image...
    }
    else {
      // quantization not really required (just a few colors on image)
      j = 0; // search the colors in the quantization
    }

    do {
      centerElems_.fill(0);

      // assign a cluster label to each pixel

      hashMapType::iterator it;
      // for each entry in the array of maps of hashEntries...
      for (i=firstKeySize_-1;i>=0;--i) {
        for (it=theHash_[i].begin();
             it!=theHash_[i].end();
             ++it) {
          hashEntry& he = (*it).second;

          // j is used as flag to indicate the if the image has less
          // colors than the desired ones (<centroids_.size()) or not.
          if (j<centroids_.size()) {
            centerElems_.at(j) += he.counter;
            he.index = j;
            j++;
          } else {
            // find which centroid corresponds to this entry

            // reconstruct the color value for the entry at (*it)
            const uint32 val = static_cast<uint32>((*it).first | i);
            px = rgbaPixel(val);
            idx = 0;
            float dist = static_cast<float>(centroids_.at(0).distanceSqr(px));
            float tmp;
            for (k=1;k<centroids_.size();++k) {
              if ((tmp = static_cast<float>(centroids_.at(k).distanceSqr(px))) < dist) {
                idx = k;
                dist = tmp;
              }
            }
            he.index = idx;

            // count number of pixels that belong to centroid(idx)
            centerElems_.at(idx) +=  he.counter;
          }
        }
      }

      // recompute centroid-colors
      genericVector<bool> adapted(centroids_.size(),false);
      for (i=firstKeySize_-1;i>=0;--i) {
        for (it=theHash_[i].begin();
             it!=theHash_[i].end();
             ++it) {
          hashEntry& he = (*it).second;
          idx = he.index;
          const uint32 val = static_cast<uint32>((*it).first | i);
          px = rgbaPixel(val);
          // centerElemes[idx] is always >= he.counter
          // the centroids will contain at the end the average of all
          // colors assigned to it.
          if (!adapted.at(idx)) {
            centroids_.at(idx)=frgbPixel(0,0,0);
            adapted.at(idx)=true;
          }

          px.multiply(static_cast<float>(he.counter)/
                      static_cast<float>(centerElems_.at(idx)));
          centroids_.at(idx).add(px);
        }
      }

      // if there are colors unused, they need to be assigned again...
      allEntriesUsed = true;
      kk = (kk % centrIndex.size());
      
      for (i=0;i<adapted.size();++i) {
        if (!adapted.at(i)) {
          allEntriesUsed = false;
          if (tmpCenterElems.empty()) {
            tmpCenterElems.copy(centerElems_);
            for (k=0;k<centrIndex.size();++k)
              centrIndex.at(k)=k;
            sorter.apply(tmpCenterElems,centrIndex);
            k=0;
          }
          // split the biggest clusters

          _lti_debug("Cluster " << i << " unassigned." << std::endl <<
                     "  Splitting " << centrIndex.at(kk) << std::endl);


          centroids_.at(i)=centroids_.at(centrIndex.at(kk)) +
            frgbPixel(static_cast<float>(4*random()-2),
                      static_cast<float>(4*random()-2),
                      static_cast<float>(4*random()-2));
          kk++;
          kk = (kk % centrIndex.size());
        }
      }
      
    } while (j!=0 && !allEntriesUsed);
  }

  void kMColorQuantization::kMeanColor::iterate() {
    bool changed = true;
    vector<frgbPixel> centroidsOld;
    float changePal = thresholdDeltaPalette_+1;
    int iter = 0;
    int i,k,total,counter;
    int idx,idx2;
    float dist,tmp;
    frgbPixel px,px2;
    hashMapType::iterator it;

    while (changed &&
	   iter<maxNumberOfIterations_ &&
	   changePal>thresholdDeltaPalette_) {

      changed = false;
      centroidsOld.copy(centroids_);

      for (i=0;i<firstKeySize_;++i) {
        for (it=theHash_[i].begin();
             it!=theHash_[i].end();
             ++it) {
          hashEntry& he = (*it).second;

          // find which centroid corresponds to this entry
          const uint32 val = static_cast<uint32>( (*it).first | i);
          px = rgbaPixel(val);
          idx = 0;
          dist =  static_cast<float>(centroids_.at(0).distanceSqr(px));
          for (k=1;k<centroids_.size();++k) {
            if ((tmp = static_cast<float>(centroids_.at(k).distanceSqr(px))) < dist) {
              idx = k;
              dist = tmp;
            }
          }

          if (idx != he.index) { // centroid changed!
	    changed = true;

	    counter = he.counter;
            idx2 = he.index; //old
            he.index = idx;  //new
            px2 = px;

            // update the old centroid
            total = centerElems_.at(idx2) - counter;
            if (total!=0) {
              px2.multiply(static_cast<float>(counter)/
                           static_cast<float>(total));
              centroids_.at(idx2).
                multiply(static_cast<float>(centerElems_.at(idx2))/
                         static_cast<float>(total));
              centroids_.at(idx2).subtract(px2);
            }
            centerElems_.at(idx2) = total;

            // recompute centroid
            total = centerElems_.at(idx) + counter;
            px.multiply(static_cast<float>(counter)/
                        static_cast<float>(total));
            centroids_.at(idx).multiply(float(centerElems_.at(idx))/
                                        float(total));
            centroids_.at(idx).add(px);
            centerElems_.at(idx) = total;
          }
        }
      }
      changePal = 0.0f;
      for(k=0;k<centroids_.size();++k) {
        changePal += static_cast<float>(centroids_.at(k).distanceSqr(centroidsOld.at(k)));
      }

      _lti_debug("Iteration: " << iter << " change:"<<changePal<<std::endl);

      iter++;
    }
  }

  double kMColorQuantization::kMeanColor::random() const {
    static const double m = 1.0/RAND_MAX;
    return m*rand();
  }

} // namespace






