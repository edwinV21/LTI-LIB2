/*
 * Copyright (C) 2009 by Pablo Alvarado
 * 
 * This file is part of the lecture CE-5201 Digital Image Processing and
 * Analysis, at the Costa Rica Institute of Technology.
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

/* 
 * \file   ltiFastRelabeling.cpp
 *         Contains the class lti::fastRelabeling used to detect connected
 *         regions in a labeled mask.
 * \author Pablo Alvarado
 * \date   16.5.2003
 *
 * revisions ..: $Id: ltiFastRelabeling.cpp,v 1.3 2009-11-12 21:59:06 alvarado Exp $
 */

#include "ltiFastRelabeling.h"
#include "ltiSort2.h"

#include <limits>

#undef _LTI_DEBUG
// #define _LTI_DEBUG 3
#include "ltiDebug.h"

#ifdef _LTI_DEBUG
#include "ltiViewer2D.h"
#endif

namespace lti {

  namespace internal {
    /**
     * The helper class partially relabels a matrix<T> in a matrix<U>.
     * The returned equivLabels vector contains all necessary information to
     * complete the relabeling process.  It does not the job here in order to
     * allow other classes to compute other information while finishing the
     * computations, saving some time in time critical operations.
     */
    template<class T,class U>
    class relabelingHelper {
    public:
      /**
       * The only constructor expects the threshold and
       * if the input mask is labeled or not
       */
      relabelingHelper(const T minThresh,
                       const T maxThresh,
                       const bool labeled,
                       const bool neigh4);
      
      /**
       * Partially relabel the src matrix and leave the result in
       * dest matrix.
       */
      bool apply(const matrix<T>& src,
                 matrix<U>& dest,
                 vector<U>& equivLabels,
                 int& numObjects) const;
      
      /**
       * Just relabel the destination, without computing anything else.
       *
       * On-place method, the partial labels are expected in dest, where the
       * result is written.
       *
       * The relabelingHelper::apply plus this method finish the (unsorted)
       * work.
       */
      bool simpleRelabel(const vector<U>& equivLabels,
                         matrix<U>& dest) const;
      
      /**
       * Just relabel the destination, without computing anything else
       *
       * On-copy method, the partial labels are expected in \a idest, and
       * the result will be left in dest, that will be properly resized.
       *
       * The relabelingHelper::apply plus this method finish the (unsorted)
       * work.
       */
      bool simpleRelabel(const ivector& equivLabels,
                         const imatrix& idest,
                         matrix<U>& dest) const;
      
      /**
       * Just relabel the destination, but compute also the size of
       * each object.
       *
       * On-place method, the partial labels are expected in dest, where the
       * result is written.
       *
       * The relabelingHelper::apply plus this method finish the (unsorted)
       * work.
       */
      bool relabelWithArea(const vector<U>& equivLabels,
                           const int numObjects,
                           matrix<U>& dest,
                           ivector& objSize) const;
      
      /**
       * Just relabel the destination, but compute also the size of
       * each object.
       *
       * On-copy method, the partial labels are expected in \a idest, and
       * the result will be left in dest, that will be properly resized.
       *
       * The relabelingHelper::apply plus this method finish the (unsorted)
       * work.
       */
      bool relabelWithArea(const ivector& equivLabels,
                           const int numObjects,
                           const imatrix& idest,
                           matrix<U>& dest,
                           ivector& objSize) const;
      
      /**
       * Sort labels and eliminate those objects with sizes smaller than the
       * given threshold.
       *
       * @param minSize minimum number of pixels an object must have.
       * @param nLargest if greater than zero, only the given number of objects
       *                 will be returned (besides the background).
       * @param numPixel number of pixels per object.  The index of the vector
       *                 corresponds to the label in the given mask.
       *                 After calling this vector is also sorted.
       * @param newNumPixelsSize the new valid number of elements of the num
       *                         pixels, which is not resized to avoid
       *                         memory handling
       * @param mask the new label index
       */
      bool sortLabels(const int minSize,
                      const int nLargest,
                      ivector& numPixels,
                      int& newNumPixelsSize,
                      vector<U>& reindex) const;
      
      /**
       * Sort labels and eliminate those objects with sizes smaller than the
       * given threshold.
       *
       * @param minSize minimum number of pixels an object must have.
       * @param nLargest if greater than zero, only the given number of objects
       *                 will be returned (besides the background).
       * @param numPixel number of pixels per object.  The index of the vector
       *                 corresponds to the label in the given mask.
       *                 After calling this vector is also sorted.
       * @param newNumPixelsSize the new valid number of elements of the num
       *                         pixels, which is not resized to avoid
       *                         memory handling
       * @param mask new relabeled mask
       */
      bool sortLabels(const int minSize,
                      const int nLargest,
                      ivector& numPixels,
                      int& newNumPixelsSize,
                      matrix<U>& mask) const;
      
      
      /**
       * Sort labels and eliminate those objects with sizes smaller than the
       * given threshold.
       *
       * @param minSize minimum number of pixels an object must have.
       * @param nLargest if greater than zero, only the given number of objects
       *                 will be returned (besides the background).
       * @param numPixel number of pixels per object.  The index of the vector
       *                 corresponds to the label in the given mask.
       *                 After calling this vector is also sorted.
       * @param newNumPixelsSize the new valid number of elements of the num
       *                         pixels, which is not resized to avoid
       *                         memory handling
       * @param mask new relabeled mask
       * @param objects vector of objects
       */
      bool sortLabels(const int minSize,
                      const int nLargest,
                            ivector& numPixels,
                            int& newNumPixelsSize,
                            matrix<U>& mask,
                            std::vector<areaPoints>& objects) const;
      

      /**
       * Compute new label equivalences considering the suppression of
       * small regions.
       *
       * @return New number of labels used.
       */
      int suppress(const int minSize,
                         ivector& numPixels,
                         vector<U>& labels) const;

      /**
       * Suppress small objects.
       *
       * @param minSize minimum number of pixels an object must have.
       * @param numPixel number of pixels per object.  The index of the vector
       *                 corresponds to the label in the given mask.
       *                 After calling this vector is also sorted.
       * @param newNumPixelsSize the new valid number of elements of the num
       *                         pixels, which is not resized to avoid
       *                         memory handling
       * @param mask new relabeled mask
       */
      bool suppress(const int minSize,
                    ivector& numPixels,                                  
                    int& newNumPixelsSize,
                    matrix<U>& mask) const;
      
      /**
       * Suppress small objects.
       *
       * @param minSize minimum number of pixels an object must have.
       * @param numPixel number of pixels per object.  The index of the vector
       *                 corresponds to the label in the given mask.
       *                 After calling this vector is also sorted.
       * @param newNumPixelsSize the new valid number of elements of the num
       *                         pixels, which is not resized to avoid
       *                         memory handling
       * @param objects vector of objects
       * @param mask new relabeled mask
       */
      bool suppress(const int minSize,
                    ivector& numPixels,                                  
                    int& newNumPixelsSize,
                    matrix<U>& mask,
                    std::vector<areaPoints>& objects) const;
      
    protected:

      /**
       * Set in constructor to specify if the input mask is labeled or not
       */
      const bool labeled_;
      
      /**
       * Type of neighborhood used
       */
      const bool fourNeighborhood_;
      
      
      /**
       * Set in constructor to the threshold to be used:
       * Only values >= minThreshold will be considered for relabeling
       */
      const T minThreshold_;
      
      /**
       * Set in constructor to the threshold to be used:
       * Only values <= maxThreshold will be considered for relabeling
       */
      const T maxThreshold_;
      
      /**
       * Relabel unlabeled mask, 4 neighborhood
       */
      bool relabelUnlabeled4(const matrix<T>& src,
                             matrix<U>& dest,
                             vector<U>& equivLabels,
                             int& numObjects) const;
      
      /**
       * Relabel labeled mask, 4 neighborhood
       */
      bool relabelLabeled4(const matrix<T>& src,
                           matrix<U>& dest,
                           vector<U>& equivLabels,
                           int& numObjects) const;
      
      /**
       * Relabel unlabeled mask, 8 neighborhood
       */
      bool relabelUnlabeled8(const matrix<T>& src,
                             matrix<U>& dest,
                             vector<U>& equivLabels,
                             int& numObjects) const;
      
      /**
       * Relabel labeled mask, 8 neighborhood
       */
      bool relabelLabeled8(const matrix<T>& src,
                           matrix<U>& dest,
                           vector<U>& equivLabels,
                           int& numObjects) const;
      
      /**
       * Update equivalence vector.
       *
       * This method ensures that the smallest number of labels are
       * used, and rewrites the equivLabels so that it can be employed
       * as a LUT.
       *
       * Remember that the background is always the last element of the
       * equivLabels vector.
       *
       * @return number of objects found
       */
      int updateEquivalencesVector(vector<U>& equivLabels,
                                   const int numLabels,
                                   const int background) const;
      
    };
    
    // --------------------------------------------------------------------
    // Implementation relabelingHelper<T,U>
    // --------------------------------------------------------------------
    
    template<class T,class U>
    relabelingHelper<T,U>::relabelingHelper(const T minThresh,
                                            const T maxThresh,
                                            const bool lab,
                                            const bool neigh4)
      : labeled_(lab),fourNeighborhood_(neigh4),
        minThreshold_(minThresh),maxThreshold_(maxThresh) {
    }
    
    template<class T,class U>
    bool 
    relabelingHelper<T,U>::simpleRelabel(const vector<U>& equivLabels,
                                         matrix<U>& dest) const {
      
      int y;
      typename vector<U>::iterator vit,eit;
      for (y=0;y<dest.rows();++y) {
        vector<U>& vct = dest.getRow(y);
        for (vit=vct.begin(),eit=vct.end();vit!=eit;++vit) {
          (*vit)=equivLabels.at((*vit));
        }
      }
      
      return true;
    }
    
    template<class T,class U>
    bool 
    relabelingHelper<T,U>::simpleRelabel(const ivector& equivLabels,
                                         const imatrix& idest,
                                         matrix<U>& dest) const {
      
      dest.allocate(idest.size());
      int y;
      ivector::const_iterator ivit;
      typename vector<U>::iterator vit,eit;
      for (y=0;y<dest.rows();++y) {
        const ivector& ivct = idest.getRow(y);
        vector<U>& vct = dest.getRow(y);
        for (ivit=ivct.begin(),vit=vct.begin(),eit=vct.end();
             vit!=eit;
             ++vit,++ivit) {
          (*vit)=static_cast<U>(equivLabels.at(*ivit));
        }
      }
      
      return true;
    }
    
    template<class T,class U>
    bool 
    relabelingHelper<T,U>::relabelWithArea(const vector<U>& equivLabels,
                                           const int numObjects,
                                           matrix<U>& dest,
                                           ivector& numPixels) const {
      
      // initialize counters
      numPixels.assign(numObjects,0);
      
      int y;
      typename vector<U>::iterator vit,eit;
      for (y=0;y<dest.rows();++y) {
        vector<U>& vct = dest.getRow(y);
        for (vit=vct.begin(),eit=vct.end();vit!=eit;++vit) {
          (*vit)=equivLabels.at((*vit));
          numPixels.at(*vit)++;
        }
      }
      
      return true;
    }
    
    template<class T,class U>
    bool 
    relabelingHelper<T,U>::relabelWithArea(const ivector& equivLabels,
                                           const int numObjects,
                                           const imatrix& idest,
                                           matrix<U>& dest,
                                           ivector& numPixels) const {
      // destination
      dest.allocate(idest.size());
      
      // initialize counters
      numPixels.assign(numObjects,0);
      
      int y;
      ivector::const_iterator ivit;
      typename vector<U>::iterator vit,eit;
      for (y=0;y<dest.rows();++y) {
        vector<U>& vct = dest.getRow(y);
        const ivector ivct = idest.getRow(y);
        for (ivit=ivct.begin(),vit=vct.begin(),eit=vct.end();
             vit!=eit;
             ++vit,++ivit) {
          (*vit)=static_cast<U>(equivLabels.at(*ivit));
          numPixels.at(*vit)++;
        }
      }
      
      return true;
    }
    
    template<class T,class U>
    bool relabelingHelper<T,U>::sortLabels(const int minSize,
                                           const int nLargest,
                                           ivector& numPixels,
                                           int& newNumPixelsSize,
                                           vector<U>& reindex) const {
      
      vector<U> labels;
      labels.allocate(numPixels.size());
      int i;
      for (i=0;i<labels.size();++i) {
        labels.at(i)=static_cast<U>(i);
      }
      
      // avoid removing the background label from 0
      int bkgPix = numPixels.at(0);
      numPixels.at(0)=std::numeric_limits<int>::max();
      
      // sort the labels
      sort2 sorter(Descending); // descending order
      sorter.apply(numPixels,labels);
      
      reindex.allocate(labels.size());
      
      const int last = min(reindex.size(),
                           (nLargest > 0) ? nLargest+1 : reindex.size());

      // labels for biggest objects
      for (i=0;(i<last) && (numPixels.at(i) >= minSize);++i) {
        reindex.at(labels.at(i))=i;
      }
      
      newNumPixelsSize = i;

      // reset labels for smallest objects
      for (;i<reindex.size();++i) {
        bkgPix += numPixels.at(i); // new pixels to the background
        numPixels.at(i)=0;
        reindex.at(labels.at(i))=0;            // background label
      }
      
      numPixels.at(labels.at(0)) = bkgPix; // restore pixel count
      
      return true;
    }
    
    template<class T,class U>
    bool relabelingHelper<T,U>::sortLabels(const int minSize,
                                           const int nLargest,
                                           ivector& numPixels,
                                           int& newNumPixelsSize,
                                           matrix<U>& mask) const {

      if (mask.empty()) {
        return true;
      }

      vector<U> reindex;
      sortLabels(minSize,nLargest,numPixels,newNumPixelsSize,reindex);

      return simpleRelabel(reindex,mask);
    }

    template<class T,class U>
    bool relabelingHelper<T,U>::sortLabels(const int minSize,
                                           const int nLargest,
                                           ivector& numPixels,
                                           int& newNumPixelsSize,
                                           matrix<U>& mask,
                                           std::vector<areaPoints>& objects
                                           ) const {
      if (mask.empty()) {
        return true;
      }

      vector<U> reindex;
      sortLabels(minSize,nLargest,numPixels,newNumPixelsSize,reindex);

      objects.clear();
      objects.resize(newNumPixelsSize);
      // relabel
      int x,y;
      for (y=0;y<mask.rows();++y) {
        for (x=0;x<mask.columns();++x) {
          U& vit = mask.at(y,x);
          vit=reindex.at(vit);
          (objects[vit]).push_back(ipoint(x,y));
        }
      }
      return true;
    }

    template<class T,class U>
    int relabelingHelper<T,U>::suppress(const int minSize,
                                              ivector& numPixels,
                                              vector<U>& labels) const {

      labels.allocate(numPixels.size());
      labels.at(0)=0; // background is always background

      int i,j;
      // beginning with 1, to ignore background
      for (i=1,j=1;i<labels.size();++i) {
        if (numPixels.at(i) >= minSize) {
          labels.at(i) = j;
          numPixels.at(j)=numPixels.at(i);
          j++;
        } else {
          labels.at(i)    = 0;
          numPixels.at(0)+= numPixels.at(i);
          numPixels.at(i) = 0;
        }
      }

      return j;
    }


    template<class T,class U>
    bool relabelingHelper<T,U>::suppress(const int minSize,
                                         ivector& numPixels,
                                         int& newNumPixelsSize,
                                         matrix<U>& mask) const {

      if (mask.empty()) {
        return true;
      }
      
      vector<U> labels; // a new equivalence labels LUT
      newNumPixelsSize = suppress(minSize,numPixels,labels);
      return simpleRelabel(labels,mask);
    }


    template<class T,class U>
    bool relabelingHelper<T,U>::suppress(const int minSize,
                                         ivector& numPixels,
                                         int& newNumPixelsSize,
                                         matrix<U>& mask,
                                         std::vector<areaPoints>& objects
                                         ) const {

      if (mask.empty()) {
        return true;
      }

      vector<U> labels; // a new equivalence labels LUT
      const int numLabels = suppress(minSize,numPixels,labels);
      newNumPixelsSize = numLabels;
      objects.clear();
      objects.resize(numLabels);

      // relabel
      int x,y;
      for (y=0;y<mask.rows();++y) {
        for (x=0;x<mask.columns();++x) {
          U& vit = mask.at(y,x);
          _lti_debug3("mask.at(" << y << "," << x << ")= " << vit << " -> " << 
                      labels.at(vit) << std::endl);
          vit=labels.at(vit);
          objects[vit].push_back(ipoint(x,y));
        }
      }
    
      return true;
    }


    template<class T,class U>
    bool relabelingHelper<T,U>::apply(const matrix<T>& src,
                                      matrix<U>& dest,
                                      vector<U>& equivLabels,
                                      int& numObjects) const {
      if (src.empty()) {
        dest.clear();
        equivLabels.clear();
        numObjects=0;
        return true;
      }

      if (fourNeighborhood_) {
        if (labeled_) {
          return relabelLabeled4(src,dest,equivLabels,numObjects);
        } else {
          return relabelUnlabeled4(src,dest,equivLabels,numObjects);
        }
      } else {
        if (labeled_) {
          return relabelLabeled8(src,dest,equivLabels,numObjects);
        } else {
          return relabelUnlabeled8(src,dest,equivLabels,numObjects);
        }
      }
    }

    template<class T,class U>
    bool relabelingHelper<T,U>::relabelLabeled4(const matrix<T>& src,
                                                matrix<U>& dest,
                                                vector<U>& equivLabels,
                                                int& numObjects) const {
#if defined(_LTI_DEBUG) && (_LTI_DEBUG >= 3)
      static viewer2D::parameters vpar;
      vpar.contrast=2.0f;
      vpar.zoomFactor=2;
      vpar.title="Relabeling";
      static viewer2D view(vpar);

      vpar.title="Labels";
      vpar.pixelsPerElement = 1;
      vpar.useBoxes=true;
      vpar.useLines=true;
      vpar.drawMeanValues = false;

      static viewer2D viewl(vpar);
#endif

      dest.allocate(src.size());
    
      // first line    
      int x,y;

      // the labels vector worst case is one where each pixel is
      // isolated.
      const int size = src.rows()*src.columns();
      const U background = 
        static_cast<U>(min(static_cast<int>(std::numeric_limits<U>::max()),
                           size-1));

      U currentLabel = 0;
      U lastLabel;
      U tmpLabel;
      T lastSrc;

      equivLabels.assign(size,background);
    
      // ---------------------------------
      // first iteration in scan direction
      // ---------------------------------

      // TODO: this first iteration can use iterators or pointer arithmetic
      //       to considerably speed up the computations.

      // first line
      lastSrc = src.at(0,0);
      lastLabel = dest.at(0,0) = 
        ((lastSrc < minThreshold_) || (lastSrc > maxThreshold_)) ? background :
        currentLabel++;
    
      for (x=1;x<src.columns();++x) {
        const T thisSrc = src.at(0,x);
      
        if ((thisSrc < minThreshold_) || (thisSrc > maxThreshold_)) {
          dest.at(0,x) = lastLabel = background;
        } else {
          if (lastSrc != thisSrc) {
            dest.at(0,x) = lastLabel = currentLabel;
            ++currentLabel;
          } else {
            dest.at(0,x) = lastLabel;
          }
        }

        lastSrc=thisSrc;
      }

      // the rest of the image
      for (y=1;y<src.rows();++y) {
        const T thisSrc0 = src.at(y,0);

        // first line pixel
        if ((thisSrc0 < minThreshold_) || (thisSrc0 > maxThreshold_)) {
          dest.at(y,0) = lastLabel = background;
        }
        else {
          if (src.at(y-1,0) != thisSrc0) {
            dest.at(y,0) = lastLabel = currentLabel;
            ++currentLabel;
          } else {
            dest.at(y,0) = lastLabel = dest.at(y-1,0);
          }
        }
      
        lastSrc=src.at(y,0);

        // rest of the line
        for (x=1;x<src.columns();++x) {
          const T& thisSrc = src.at(y,x);

          if ((thisSrc < minThreshold_) || (thisSrc > maxThreshold_)) {
            lastLabel = dest.at(y,x) = background;
          }
          else {
            tmpLabel=dest.at(y-1,x);
            if (lastLabel != background) {
              if (tmpLabel==background) {
                // on the top background,
                if (lastSrc == thisSrc) {
                  // same label on the left, keep it!
                  dest.at(y,x) = lastLabel;
                } else {
                  // on the left a different label, so get new one.
                  dest.at(y,x) = lastLabel = currentLabel;
                  currentLabel++;
                }
              } else {
                // here comes the difficult one, on the top and on the left
                // there are labels, and we need to "synchronize" them
              
                if ((thisSrc == lastSrc) && (thisSrc == src.at(y-1,x))) {
                  // all three labels belong to the same object
                
                  // both pixels have different labels, synchronize them:
                
                  // find the lowest valid label for the left element
                  while (equivLabels.at(lastLabel) != background) {
                    lastLabel = equivLabels.at(lastLabel);
                  }
                
                  // find the lowest valid label for the top element
                  while (equivLabels.at(tmpLabel) != background) {
                    tmpLabel = equivLabels.at(tmpLabel);
                  }
                
                  if (tmpLabel != lastLabel) {
                    if (lastLabel < tmpLabel) {
                      equivLabels.at(tmpLabel) = lastLabel;
                    } else {
                      equivLabels.at(lastLabel) = tmpLabel;
                      lastLabel = tmpLabel;
                    }                  
                  }
                  dest.at(y,x) = lastLabel;

                } else if (thisSrc == lastSrc) {
                  // just the left pixel share the same label
                  dest.at(y,x) = lastLabel;
                } else if (thisSrc == src.at(y-1,x)) {
                  dest.at(y,x) = lastLabel = tmpLabel;               
                } else {
                  // new label necessary
                  // on the left background and different label on the top
                  dest.at(y,x) = lastLabel = currentLabel;
                  currentLabel++;
                }
              }
                       
            } else if (thisSrc == src.at(y-1,x)) {
              // on the left is background, but same label on the top,
              // so let's take the top label!
              dest.at(y,x) = lastLabel = tmpLabel;
            } else {
              // on the left background and different label on the top
              dest.at(y,x) = lastLabel = currentLabel;
              currentLabel++;
            }
          }

          lastSrc=thisSrc;
        }
      }

#if defined(_LTI_DEBUG) && (_LTI_DEBUG >= 3)
      view.show(dest);
      vector<U> tmpLabels(currentLabel,equivLabels.data());
      viewl.show(tmpLabels);
#endif

      // relabel 
      numObjects = updateEquivalencesVector(equivLabels,
                                            currentLabel,
                                            background);

      // in case the type U does not allow too many objects, the relabeled
      // mask can be very inconsistent!
      return (currentLabel < background);
    }

    template<class T,class U>
    bool relabelingHelper<T,U>::relabelUnlabeled4(const matrix<T>& src,
                                                  matrix<U>& dest,
                                                  vector<U>& equivLabels,
                                                  int& numObjects) const {

#if defined(_LTI_DEBUG) && (_LTI_DEBUG >= 3)
      static viewer2D::parameters vpar;
      vpar.contrast=2.0f;
      vpar.zoomFactor=4;
      vpar.title="Relabeling";
      static viewer2D view(vpar);

      vpar.title="Labels";
      vpar.pixelsPerElement = 3;
      vpar.useBoxes=true;
      vpar.useLines=true;
      vpar.drawMeanValues = false;

      static viewer2D viewl(vpar);
#endif

      dest.allocate(src.size());
    
      // first line
      int x,y;

      // the labels vector worst case is one where each pixel is
      // isolated.
      const int size = src.rows()*src.columns();
      const U background = 
        static_cast<U>(min(static_cast<int>(std::numeric_limits<U>::max()),
                           size-1));
      U currentLabel = 0;
      U lastLabel;
      U tmpLabel;

      equivLabels.assign(size,background);
    
      // ---------------------------------
      // first iteration in scan direction
      // ---------------------------------

      // TODO: this first iteration can use iterators or pointer arithmetic
      //       to considerably speed up the computations.

      // first line
      lastLabel = dest.at(0,0) = ((src.at(0,0) < minThreshold_) ||
                                  (src.at(0,0) > maxThreshold_)) ? background :
        currentLabel++;
      for (x=1;x<src.columns();++x) {
        if ((src.at(0,x)  < minThreshold_) || (src.at(0,x)  > maxThreshold_)) {
          lastLabel = dest.at(0,x) = background;
        } else {
          if (lastLabel == background) {
            lastLabel = dest.at(0,x) = currentLabel;
            ++currentLabel;
          } else {
            dest.at(0,x) = lastLabel;
          }
        }
      }

      // the rest of the image
      for (y=1;y<src.rows();++y) {      

        // first line pixel
        if ((src.at(y,0)  < minThreshold_) || (src.at(y,0)  > maxThreshold_)) {
          lastLabel = dest.at(y,0) = background;
        }
        else {
          if ((lastLabel = dest.at(y-1,0)) == background)  {
            lastLabel = dest.at(y,0) = currentLabel;
            currentLabel++;
          } else {
            dest.at(y,0) = lastLabel = dest.at(y-1,0);
          }
        }
        
        // rest of the line
        for (x=1;x<src.columns();++x) {
          if ((src.at(y,x) < minThreshold_) || (src.at(y,x) > maxThreshold_)) {
            dest.at(y,x) = lastLabel = background;
          }
          else {
            tmpLabel=dest.at(y-1,x);
            if (lastLabel != background) {
              if ((lastLabel != tmpLabel) && (tmpLabel != background)) {
                // find the lowest valid label for the left element
                while (equivLabels.at(lastLabel) != background) {
                  lastLabel = equivLabels.at(lastLabel);
                }

                // find the lowest valid label for the top element
                while (equivLabels.at(tmpLabel) != background) {
                  tmpLabel = equivLabels.at(tmpLabel);
                }

                if (lastLabel < tmpLabel) {
                  equivLabels.at(tmpLabel) = lastLabel;
                } else if (lastLabel > tmpLabel) {
                  equivLabels.at(lastLabel) = tmpLabel;
                  lastLabel = tmpLabel;
                }              
              }
              dest.at(y,x) = lastLabel;

            } else if (tmpLabel != background) {
              dest.at(y,x) = lastLabel = tmpLabel;
            } else {
              dest.at(y,x) = lastLabel = currentLabel;
              currentLabel++;
            }
          }
        }
      }

#if defined(_LTI_DEBUG) && (_LTI_DEBUG >= 3)
      view.show(dest);
      vector<U> tmpLabels(currentLabel,equivLabels.data());
      viewl.show(tmpLabels);
#endif

      // relabel 
      numObjects = updateEquivalencesVector(equivLabels,
                                            currentLabel,
                                            background);

      // in case the type U does not allow too many objects, the relabeled
      // mask can be very inconsistent!
      return (currentLabel < background);
    }


    template<class T,class U>
    bool relabelingHelper<T,U>::relabelLabeled8(const matrix<T>& src,
                                                matrix<U>& dest,
                                                vector<U>& equivLabels,
                                                int& numObjects) const {
#if defined(_LTI_DEBUG) && (_LTI_DEBUG >= 3)
      static viewer2D::parameters vpar;
      vpar.contrast=2.0f;
      vpar.zoomFactor=2;
      vpar.title="Relabeling";
      static viewer2D view(vpar);

      vpar.title="Labels";
      vpar.pixelsPerElement = 1;
      vpar.useBoxes=true;
      vpar.useLines=true;
      vpar.drawMeanValues = false;

      static viewer2D viewl(vpar);
#endif

      dest.allocate(src.size());
    
      // first line    
      int x,y,ym1;

      // the labels vector worst case is one where each pixel is
      // isolated.
      const int size = src.rows()*src.columns();
      const U background = 
        static_cast<U>(min(static_cast<int>(std::numeric_limits<U>::max()),
                           size-1));
      U currentLabel = 0;
      U lastLabel;
      U tmpLabel;
      T lastSrc;

      equivLabels.assign(size,background);
    
      U neighbors[4];
      int n;

      // ---------------------------------
      // first iteration in scan direction
      // ---------------------------------

      // TODO: this first iteration can use iterators or pointer arithmetic
      //       to considerably speed up the computations.

      // first line
      lastSrc = src.at(0,0);
      lastLabel = dest.at(0,0) = 
        ((lastSrc < minThreshold_) || (lastSrc > maxThreshold_)) ? background :
        currentLabel++;
    
      for (x=1;x<src.columns();++x) {
        const T thisSrc = src.at(0,x);
      
        if ((thisSrc < minThreshold_) || (thisSrc > maxThreshold_)) {
          dest.at(0,x) = lastLabel = background;
        } else {
          if (lastSrc != thisSrc) {
            dest.at(0,x) = lastLabel = currentLabel;
            ++currentLabel;
          } else {
            dest.at(0,x) = lastLabel;
          }
        }

        lastSrc=thisSrc;
      }

      // the rest of the image
      for (y=1,ym1=0;y<src.rows();++y,++ym1) {      
        const T thisSrc0 = src.at(y,0);

        // first line pixel
        if ((thisSrc0 < minThreshold_) || (thisSrc0 > maxThreshold_)) {
          dest.at(y,0) = background;
        }
        else {
          // which neighbor pixels share label
          x=n=0;
        
          if (thisSrc0 == src.at(ym1,x)) {
            tmpLabel = dest.at(ym1,x);
            neighbors[n++]=tmpLabel;
          }
          if (thisSrc0 == src.at(ym1,x+1)) {
            tmpLabel = dest.at(ym1,x+1);
            if ((n==0) || (tmpLabel != neighbors[n-1])) {              
              neighbors[n++]=tmpLabel;
            }
          }

          if (n==0) {
            // no neighbors -> new label
            dest.at(y,x) = currentLabel;
            currentLabel++;
          } else if (n==1) {
            // only one neighbor, just get its label
            dest.at(y,x) = neighbors[0];
          } else {
            lastLabel=neighbors[n-1];              
            tmpLabel=neighbors[n-2];

            while (equivLabels.at(lastLabel) != background) {
              lastLabel = equivLabels.at(lastLabel);
            }
          
            // find the lowest valid label for the top element
            while (equivLabels.at(tmpLabel) != background) {
              tmpLabel = equivLabels.at(tmpLabel);
            }
          
            if (lastLabel < tmpLabel) {
              equivLabels.at(tmpLabel) = lastLabel;
            } else if (lastLabel > tmpLabel) {
              equivLabels.at(lastLabel) = tmpLabel;
              lastLabel = tmpLabel;
            }
            dest.at(y,x) = lastLabel;
          }
        }
      
        // rest of the line
        for (x=1;x<src.lastColumn();++x) {
          const T& thisSrc = src.at(y,x);

          if ((thisSrc < minThreshold_) || (thisSrc > maxThreshold_)) {
            dest.at(y,x) = background;
          }
          else {
            // which neighbor pixels share label
            n=0;
          
            if (thisSrc == src.at(y,x-1)) {
              neighbors[n++]=dest.at(y,x-1);
            }
            if (thisSrc == src.at(ym1,x-1)) {
              tmpLabel = dest.at(ym1,x-1);
              if ((n==0) || (tmpLabel != neighbors[n-1])) {
                neighbors[n++]=tmpLabel;
              }
            }
            if (thisSrc == src.at(ym1,x)) {
              tmpLabel = dest.at(ym1,x);
              if ((n==0) || (tmpLabel != neighbors[n-1])) {
                neighbors[n++]=tmpLabel;
              }
            }
            if (thisSrc == src.at(ym1,x+1)) {
              tmpLabel = dest.at(ym1,x+1);
              if ((n==0) || (tmpLabel != neighbors[n-1])) {              
                neighbors[n++]=tmpLabel;
              }
            }

            if (n==0) {
              // no neighbors -> new label
              dest.at(y,x) = currentLabel;
              currentLabel++;
            } else if (n==1) {
              // only one neighbor, just get its label
              dest.at(y,x) = neighbors[0];
            } else {
              lastLabel=neighbors[n-1];              
              while (n>=2) {
                tmpLabel=neighbors[n-2];

                while (equivLabels.at(lastLabel) != background) {
                  lastLabel = equivLabels.at(lastLabel);
                }

                // find the lowest valid label for the top element
                while (equivLabels.at(tmpLabel) != background) {
                  tmpLabel = equivLabels.at(tmpLabel);
                }

                if (lastLabel < tmpLabel) {
                  equivLabels.at(tmpLabel) = lastLabel;
                } else if (lastLabel > tmpLabel) {
                  equivLabels.at(lastLabel) = tmpLabel;
                  lastLabel = tmpLabel;
                }
                neighbors[n-2]=lastLabel;
                --n;
              } // end while n
              dest.at(y,x) = lastLabel;
            }
          } 
        }

        // last pixel
        const T& thisSrcL = src.at(y,x);
      
        if ((thisSrcL < minThreshold_) || (thisSrcL > maxThreshold_)) {
          dest.at(y,x) = background;
        }
        else {
          // which neighbor pixels share label
          n=0;
        
          if (thisSrcL == src.at(y,x-1)) {
            neighbors[n++]=dest.at(y,x-1);
          }
          if (thisSrcL == src.at(ym1,x-1)) {
            tmpLabel = dest.at(ym1,x-1);
            if ((n==0) || (tmpLabel != neighbors[n-1])) {
              neighbors[n++]=tmpLabel;
            }
          }
          if (thisSrcL == src.at(ym1,x)) {
            tmpLabel = dest.at(ym1,x);
            if ((n==0) || (tmpLabel != neighbors[n-1])) {
              neighbors[n++]=tmpLabel;
            }
          }

          if (n==0) {
            // no neighbors -> new label
            dest.at(y,x) = currentLabel;
            currentLabel++;
          } else if (n==1) {
            // only one neighbor, just get its label
            dest.at(y,x) = neighbors[0];
          } else {
            lastLabel=neighbors[n-1];              
            while (n>=2) {
              tmpLabel=neighbors[n-2];
            
              while (equivLabels.at(lastLabel) != background) {
                lastLabel = equivLabels.at(lastLabel);
              }
            
              // find the lowest valid label for the top element
              while (equivLabels.at(tmpLabel) != background) {
                tmpLabel = equivLabels.at(tmpLabel);
              }
            
              if (lastLabel < tmpLabel) {
                equivLabels.at(tmpLabel) = lastLabel;
              } else if (lastLabel > tmpLabel) {
                equivLabels.at(lastLabel) = tmpLabel;
                lastLabel = tmpLabel;
              }
              neighbors[n-2]=lastLabel;
              --n;
            } // end while n
            dest.at(y,x) = lastLabel;
          }
        } 
      }

#if defined(_LTI_DEBUG) && (_LTI_DEBUG >= 3)
      view.show(dest);
      vector<U> tmpLabels(currentLabel,equivLabels.data());
      viewl.show(tmpLabels);
#endif

      // relabel 
      numObjects = updateEquivalencesVector(equivLabels,
                                            currentLabel,
                                            background);

      // in case the type U does not allow too many objects, the relabeled
      // mask can be very inconsistent!
      return (currentLabel < background);
    }

    template<class T,class U>
    bool relabelingHelper<T,U>::relabelUnlabeled8(const matrix<T>& src,
                                                  matrix<U>& dest,
                                                  vector<U>& equivLabels,
                                                  int& numObjects) const {

#if defined(_LTI_DEBUG) && (_LTI_DEBUG >= 3)
      static viewer2D::parameters vpar;
      vpar.contrast=2.0f;
      vpar.zoomFactor=4;
      vpar.title="Relabeling";
      static viewer2D view(vpar);

      vpar.title="Labels";
      vpar.pixelsPerElement = 3;
      vpar.useBoxes=true;
      vpar.useLines=true;
      vpar.drawMeanValues = false;

      static viewer2D viewl(vpar);
#endif

      dest.allocate(src.size());
    
      // first line
    
      int x,y,ym1;

      // the labels vector worst case is one where each pixel is
      // isolated.
      const int size = src.rows()*src.columns();
      const U background = 
        static_cast<U>(min(static_cast<int>(std::numeric_limits<U>::max()),
                           size-1));
      U currentLabel = 0;
      U lastLabel;
      U tmpLabel;

      equivLabels.assign(size,background);

      U neighbors[4];
      int n;
    
      // ---------------------------------
      // first iteration in scan direction
      // ---------------------------------

      // TODO: this first iteration can use iterators or pointer arithmetic
      //       to considerably speed up the computations.

      // first line
      lastLabel = dest.at(0,0) = ((src.at(0,0) < minThreshold_) ||
                                  (src.at(0,0) > maxThreshold_)) ? background :
        currentLabel++;
      for (x=1;x<src.columns();++x) {
        if ((src.at(0,x)  < minThreshold_) || (src.at(0,x)  > maxThreshold_)) {
          lastLabel = dest.at(0,x) = background;
        } else {
          if (lastLabel == background) {
            lastLabel = dest.at(0,x) = currentLabel;
            ++currentLabel;
          } else {
            dest.at(0,x) = lastLabel;
          }
        }
      }

      // the rest of the image
      for (y=1,ym1=0;y<src.rows();++y,++ym1) {      
        // first line pixel
        if ((src.at(y,0)  < minThreshold_) || (src.at(y,0)  > maxThreshold_)) {
          lastLabel = dest.at(y,0) = background;
        }
        else {
          // which neighbor pixels share label
          x=n=0;
          if ((tmpLabel=dest.at(ym1,x))   != background) {
            neighbors[n++]=tmpLabel;          
          }
          if ((tmpLabel=dest.at(ym1,x+1)) != background) {
            if ((n==0) || (tmpLabel != neighbors[n-1])) {              
              neighbors[n++]=tmpLabel;
            }
          }

          if (n==0) {
            // no neighbors -> new label
            dest.at(y,x) = currentLabel;
            currentLabel++;
          } else if (n==1) {
            // only one neighbor, just get its label
            dest.at(y,x) = neighbors[0];
          } else {
            lastLabel=neighbors[n-1];              
            tmpLabel=neighbors[n-2];

            while (equivLabels.at(lastLabel) != background) {
              lastLabel = equivLabels.at(lastLabel);
            }
          
            // find the lowest valid label for the top element
            while (equivLabels.at(tmpLabel) != background) {
              tmpLabel = equivLabels.at(tmpLabel);
            }

            if (lastLabel < tmpLabel) {
              equivLabels.at(tmpLabel) = lastLabel;
            } else if (lastLabel > tmpLabel) {
              equivLabels.at(lastLabel) = tmpLabel;
              lastLabel = tmpLabel;
            }
            neighbors[n-2]=lastLabel;
            dest.at(y,x) = lastLabel;
          }
        }
        
        // rest of the line
        for (x=1;x<src.lastColumn();++x) {
          if ((src.at(y,x) < minThreshold_) || (src.at(y,x) > maxThreshold_)) {
            // pixel is "background"
            dest.at(y,x) = background;
          }
          else {
            // which neighbor pixels share label
            n=0;

            if ((tmpLabel=dest.at(y,x-1))   != background) {
              neighbors[n++]=tmpLabel;
            }
            if ((tmpLabel=dest.at(ym1,x-1)) != background) {
              if ((n==0) || (tmpLabel != neighbors[n-1])) {
                neighbors[n++]=tmpLabel;
              }
            }
            if ((tmpLabel=dest.at(ym1,x))   != background) {
              if ((n==0) || (tmpLabel != neighbors[n-1])) {
                neighbors[n++]=tmpLabel;
              }
            }
            if ((tmpLabel=dest.at(ym1,x+1)) != background) {
              if ((n==0) || (tmpLabel != neighbors[n-1])) {              
                neighbors[n++]=tmpLabel;
              }
            }

            if (n==0) {
              // no neighbors -> new label
              dest.at(y,x) = currentLabel;
              currentLabel++;
            } else if (n==1) {
              // only one neighbor, just get its label
              dest.at(y,x) = neighbors[0];
            } else {
              lastLabel=neighbors[n-1];              
              while (n>=2) {
                tmpLabel=neighbors[n-2];

                while (equivLabels.at(lastLabel) != background) {
                  lastLabel = equivLabels.at(lastLabel);
                }

                // find the lowest valid label for the top element
                while (equivLabels.at(tmpLabel) != background) {
                  tmpLabel = equivLabels.at(tmpLabel);
                }

                if (lastLabel < tmpLabel) {
                  equivLabels.at(tmpLabel) = lastLabel;
                } else if (lastLabel > tmpLabel) {
                  equivLabels.at(lastLabel) = tmpLabel;
                  lastLabel = tmpLabel;
                }
                neighbors[n-2]=lastLabel;
                --n;
              }
              dest.at(y,x) = lastLabel;
            }
          } // end while n
        } // end for all x in this y

        // last pixel
        if ((src.at(y,x) < minThreshold_) || (src.at(y,x) > maxThreshold_)) {
          // pixel is "background"
          dest.at(y,x) = background;
        }
        else {
          // which neighbor pixels share label
          n=0;
        
          if ((tmpLabel=dest.at(y,x-1))   != background) {
            neighbors[n++]=tmpLabel;
          }
          if ((tmpLabel=dest.at(ym1,x-1)) != background) {
            if ((n==0) || (tmpLabel != neighbors[n-1])) {
              neighbors[n++]=tmpLabel;
            }
          }
          if ((tmpLabel=dest.at(ym1,x))   != background) {
            if ((n==0) || (tmpLabel != neighbors[n-1])) {
              neighbors[n++]=tmpLabel;
            }
          }

          if (n==0) {
            // no neighbors -> new label
            dest.at(y,x) = currentLabel;
            currentLabel++;
          } else if (n==1) {
            // only one neighbor, just get its label
            dest.at(y,x) = neighbors[0];
          } else {
            lastLabel=neighbors[n-1];              
            while (n>=2) {
              tmpLabel=neighbors[n-2];
            
              while (equivLabels.at(lastLabel) != background) {
                lastLabel = equivLabels.at(lastLabel);
              }
            
              // find the lowest valid label for the top element
              while (equivLabels.at(tmpLabel) != background) {
                tmpLabel = equivLabels.at(tmpLabel);
              }
            
              if (lastLabel < tmpLabel) {
                equivLabels.at(tmpLabel) = lastLabel;
              } else if (lastLabel > tmpLabel) {
                equivLabels.at(lastLabel) = tmpLabel;
                lastLabel = tmpLabel;
              }
              neighbors[n-2]=lastLabel;
              --n;
            }
            dest.at(y,x) = lastLabel;
          }
        }
      }

#if defined(_LTI_DEBUG) && (_LTI_DEBUG >= 3)
      view.show(dest);
      vector<U> tmpLabels(currentLabel,equivLabels.data());
      viewl.show(tmpLabels);
#endif

      // relabel 
      numObjects = updateEquivalencesVector(equivLabels,
                                            currentLabel,
                                            background);

      // in case the type U does not allow too many objects, the relabeled
      // mask can be very inconsistent!
      return (currentLabel < background);
    }


    template<class T,class U>
    int 
    relabelingHelper<T,U>::updateEquivalencesVector(vector<U>& equivLabels,
                                                    const int numLabels,
                                                    const int background
                                                    ) const {

      // first pass is ready.  At this point currentLabel contains
      // one plus the last label used.
    
      // fix the equivalence vector
      int x;

      for (x=0;x<numLabels;++x) {
        if (equivLabels.at(x) != background) {
          while (equivLabels.at(equivLabels.at(x)) != background) {
            equivLabels.at(x)=equivLabels.at(equivLabels.at(x));
          }
        } 
      }

      int lastLabel = 1;
      for (x=0;x<numLabels;++x) {
        if (equivLabels.at(x) == background) {
          equivLabels.at(x) = lastLabel;
          ++lastLabel;
        } else {
          equivLabels.at(x) = equivLabels.at(equivLabels.at(x));
        }
      }
      equivLabels.at(background)=0;

      return lastLabel;
    }

  } // namespace internal

  // --------------------------------------------------
  // fastRelabeling::parameters
  // --------------------------------------------------

  // default constructor
  fastRelabeling::parameters::parameters()
    : functor::parameters() {
    
    minThreshold = int(1);
    maxThreshold = std::numeric_limits<int>::max();
    assumeLabeledMask = bool(true);
    fourNeighborhood  = bool(true);
    sortSize = false;
    minimumObjectSize = 1;
  }

  // copy constructor
  fastRelabeling::parameters::parameters(const parameters& other)
    : functor::parameters() {
    copy(other);
  }

  // destructor
  fastRelabeling::parameters::~parameters() {
  }

  // get type name
  const std::string& fastRelabeling::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // copy member

  fastRelabeling::parameters&
  fastRelabeling::parameters::copy(const parameters& other) {
    functor::parameters::copy(other);
  
    minThreshold      = other.minThreshold;
    maxThreshold      = other.maxThreshold;
    assumeLabeledMask = other.assumeLabeledMask;
    fourNeighborhood  = other.fourNeighborhood;
    sortSize          = other.sortSize;
    minimumObjectSize = other.minimumObjectSize;

    return *this;
  }

  // alias for copy member
  fastRelabeling::parameters&
  fastRelabeling::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  // clone member
  fastRelabeling::parameters* fastRelabeling::parameters::clone() const {
    return new parameters(*this);
  }

  // clone member
  fastRelabeling::parameters* fastRelabeling::parameters::newInstance() const {
    return new parameters();
  }

  /*
   * write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool fastRelabeling::parameters::write(ioHandler& handler,
                                         const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {
      lti::write(handler,"minThreshold",minThreshold);    
      lti::write(handler,"maxThreshold",maxThreshold);
      lti::write(handler,"assumeLabeledMask",assumeLabeledMask);
      lti::write(handler,"fourNeighborhood",fourNeighborhood);
      lti::write(handler,"sortSize",sortSize);
      lti::write(handler,"minimumObjectSize",minimumObjectSize);
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
  bool fastRelabeling::parameters::read(ioHandler& handler,
                                        const bool complete) {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {
      lti::read(handler,"minThreshold",minThreshold);     
      lti::read(handler,"maxThreshold",maxThreshold);
      lti::read(handler,"assumeLabeledMask",assumeLabeledMask);
      lti::read(handler,"fourNeighborhood",fourNeighborhood);
      lti::read(handler,"sortSize",sortSize);
      lti::read(handler,"minimumObjectSize",minimumObjectSize);
    }

    b = b && functor::parameters::read(handler,false);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

  // --------------------------------------------------
  // fastRelabeling
  // --------------------------------------------------

  // default constructor
  fastRelabeling::fastRelabeling()
    : functor() {

    // create an instance of the parameters with the default values
    parameters defaultParameters;
    // set the default parameters
    setParameters(defaultParameters);
  }

  // default constructor
  fastRelabeling::fastRelabeling(const parameters& par)
    : functor() {

    // set the given parameters
    setParameters(par);
  }


  // copy constructor
  fastRelabeling::fastRelabeling(const fastRelabeling& other)
    : functor() {
    copy(other);
  }

  // destructor
  fastRelabeling::~fastRelabeling() {
  }

  // returns the name of this type
  const std::string& fastRelabeling::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // copy member
  fastRelabeling&
  fastRelabeling::copy(const fastRelabeling& other) {
    functor::copy(other);
    return (*this);
  }

  // alias for copy member
  fastRelabeling&
  fastRelabeling::operator=(const fastRelabeling& other) {
    return (copy(other));
  }

  // clone member
  fastRelabeling* fastRelabeling::clone() const {
    return new fastRelabeling(*this);
  }

  // clone member
  fastRelabeling* fastRelabeling::newInstance() const {
    return new fastRelabeling();
  }

  // return parameters
  const fastRelabeling::parameters&
  fastRelabeling::getParameters() const {
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

  // On place apply for type matrix<ubyte>!
  bool fastRelabeling::apply(matrix<ubyte>& srcdest) const {
    int numObjects;
    return apply(srcdest,numObjects);
  }

  // On place apply for type imatrix!
  bool fastRelabeling::apply(imatrix& srcdest) const {
    int numObjects;
    return apply(srcdest,numObjects);
  }

  // On place apply for type matrix<ubyte>!
  bool fastRelabeling::apply(matrix<ubyte>& srcdest,int& numObjects) const {
    matrix<ubyte> dest;
    if (apply(srcdest,dest,numObjects)) {
      if (srcdest.getMode() == matrix<ubyte>::Connected) {
        dest.detach(srcdest);
      } else {
        srcdest.fill(dest);
      }
      return true;
    }
    return false;
  }

  // On place apply for type imatrix!
  bool fastRelabeling::apply(imatrix& srcdest,int& numObjects) const {
    imatrix dest;
    if (apply(srcdest,dest,numObjects)) {
      if (srcdest.getMode() == imatrix::Connected) {
        dest.detach(srcdest);
      } else {
        srcdest.fill(dest);
      }
      return true;
    }
    dest.detach(srcdest);
    return false;
  }

  // On copy apply for type matrix<ubyte>!
  bool fastRelabeling::apply(const matrix<ubyte>& src,
                             matrix<ubyte>& dest,
                             int& numObjects) const {
    
    ivector equivLabels;
    const parameters& par = getParameters();

    internal::relabelingHelper<ubyte,ubyte> 
      helper(static_cast<ubyte>(max(0,par.minThreshold)),
             static_cast<ubyte>(min(255,par.maxThreshold)),
             par.assumeLabeledMask,
             par.fourNeighborhood);

    internal::relabelingHelper<ubyte,int> 
      ihelper(static_cast<ubyte>(max(0,par.minThreshold)),
              static_cast<ubyte>(min(255,par.maxThreshold)),
              par.assumeLabeledMask,
              par.fourNeighborhood);

    imatrix idest;

    if (ihelper.apply(src,idest,equivLabels,numObjects)) {
      if (par.sortSize) {
        ivector objSize;
        return (helper.relabelWithArea(equivLabels,numObjects,
                                       idest,dest,objSize) &&
                helper.sortLabels(par.minimumObjectSize,0,
                                  objSize,numObjects,dest));
      } else {
        if (par.minimumObjectSize <= 1) {
          return helper.simpleRelabel(equivLabels,idest,dest);
        } else {
          ivector objSize;
          return (helper.relabelWithArea(equivLabels,numObjects,
                                         idest,dest,objSize)&&
                  helper.suppress(par.minimumObjectSize,
                                  objSize,numObjects,dest));
        }
      }
    }
    return false;
  }

  // On copy apply for type matrix<ubyte>!
  bool fastRelabeling::apply(const matrix<ubyte>& src,
                             imatrix& dest,
                             int& numObjects) const {

    vector<int> equivLabels;
    const parameters& par = getParameters();

    internal::relabelingHelper<ubyte,int> 
      helper(static_cast<ubyte>(max(0,par.minThreshold)),
             static_cast<ubyte>(min(255,par.maxThreshold)),
             par.assumeLabeledMask,
             par.fourNeighborhood);
    
    if (helper.apply(src,dest,equivLabels,numObjects)) {
      if (par.sortSize) {
        ivector objSize;
        return (helper.relabelWithArea(equivLabels,numObjects,dest,objSize) &&
                helper.sortLabels(par.minimumObjectSize,0,
                                  objSize,numObjects,dest));
      } else {
        if (par.minimumObjectSize <= 1) {
          return helper.simpleRelabel(equivLabels,dest);
        } else {
          ivector objSize;
          return (helper.relabelWithArea(equivLabels,numObjects,dest,objSize)&&
                  helper.suppress(par.minimumObjectSize,
                                  objSize,numObjects,dest));
        }
      }
    }

    return false;
  }

  // On copy apply for type imatrix!
  bool fastRelabeling::apply(const imatrix& src,
                             imatrix& dest,
                             int& numObjects) const {
    vector<int> equivLabels;
    const parameters& par = getParameters();

    internal::relabelingHelper<int,int> helper(par.minThreshold,
                                               par.maxThreshold,
                                               par.assumeLabeledMask,
                                               par.fourNeighborhood);
    
    if (helper.apply(src,dest,equivLabels,numObjects)) {
      if (par.sortSize) {
        ivector objSize;
        return (helper.relabelWithArea(equivLabels,numObjects,dest,objSize) &&
                helper.sortLabels(par.minimumObjectSize,0,
                                  objSize,numObjects,dest));
      } else {
        if (par.minimumObjectSize <= 1) {
          return helper.simpleRelabel(equivLabels,dest);
        } else {
          ivector objSize;
          return (helper.relabelWithArea(equivLabels,numObjects,dest,objSize)&&
                  helper.suppress(par.minimumObjectSize,
                                  objSize,numObjects,dest));
        }
      }
    }

    return false;
  }

  bool fastRelabeling::apply(const matrix<ubyte>& src,
                             matrix<ubyte>& dest) const {
    int numObjects;
    return apply(src,dest,numObjects);
  }

  bool fastRelabeling::apply(const imatrix& src,
                             imatrix& dest) const {
    int numObjects;
    return apply(src,dest,numObjects);
  }

  bool fastRelabeling::apply(const matrix<ubyte>& src,
                             imatrix& dest) const {
    int numObjects;
    return apply(src,dest,numObjects);
  }

  ///////////////////////


  // On copy apply for type matrix<ubyte>!
  bool fastRelabeling::apply(const matrix<ubyte>& src,
                             matrix<ubyte>& dest,
                             ivector& objSize) const {
    
    int numObjects;
    ivector equivLabels;
    const parameters& par = getParameters();

    internal::relabelingHelper<ubyte,ubyte> 
      helper(static_cast<ubyte>(max(0,par.minThreshold)),
             static_cast<ubyte>(min(255,par.maxThreshold)),
             par.assumeLabeledMask,
             par.fourNeighborhood);

    internal::relabelingHelper<ubyte,int> 
      ihelper(static_cast<ubyte>(max(0,par.minThreshold)),
              static_cast<ubyte>(min(255,par.maxThreshold)),
              par.assumeLabeledMask,
              par.fourNeighborhood);

    imatrix idest;
    int numObjs;

    if (ihelper.apply(src,idest,equivLabels,numObjects) &&
        helper.relabelWithArea(equivLabels,numObjects,idest,dest,objSize)) {
      if (par.sortSize) {
        if (helper.sortLabels(par.minimumObjectSize,0,
                              objSize,numObjs,dest)) {
          objSize.resize(numObjs);
          return true;
        }
      } else {
        if (helper.suppress(par.minimumObjectSize,objSize,numObjs,dest)) {
          objSize.resize(numObjs);
          return true;
        }
      }
    }
    return false;
  }

  // On copy apply for type matrix<ubyte>!
  bool fastRelabeling::apply(const matrix<ubyte>& src,
                             imatrix& dest,
                             ivector& objSize) const {
    int numObjects;
    vector<int> equivLabels;
    const parameters& par = getParameters();

    internal::relabelingHelper<ubyte,int> 
      helper(static_cast<ubyte>(max(0,par.minThreshold)),
             static_cast<ubyte>(min(255,par.maxThreshold)),
             par.assumeLabeledMask,
             par.fourNeighborhood);

    int numObjs;

    if (helper.apply(src,dest,equivLabels,numObjects) &&
        helper.relabelWithArea(equivLabels,numObjects,dest,objSize)) {
      if (par.sortSize) {
        if (helper.sortLabels(par.minimumObjectSize,0,
                              objSize,numObjs,dest)) {
          objSize.resize(numObjs);
          return true;
        }
      } else {
        if (helper.suppress(par.minimumObjectSize,objSize,numObjs,dest)) {
          objSize.resize(numObjs);
          return true;
        }
      }
    }

    return false;
  }

  // On copy apply for type imatrix!
  bool fastRelabeling::apply(const imatrix& src,
                             imatrix& dest,
                             ivector& objSize) const {
    int numObjects;
    vector<int> equivLabels;
    const parameters& par = getParameters();

    internal::relabelingHelper<int,int> helper(par.minThreshold,
                                               par.maxThreshold,
                                               par.assumeLabeledMask,
                                               par.fourNeighborhood);
    
    int numObjs;
    
    if (helper.apply(src,dest,equivLabels,numObjects) &&
        helper.relabelWithArea(equivLabels,numObjects,dest,objSize)) {
      if (par.sortSize) {
        if (helper.sortLabels(par.minimumObjectSize,0,objSize,numObjs,dest)) {
          objSize.resize(numObjs);
          return true;
        }
      } else {
        if (helper.suppress(par.minimumObjectSize,objSize,numObjs,dest)) {
          objSize.resize(numObjs);
          return true;
        }
      }
    }

    return false;
  }

  ///////////////////////


  // On copy apply for type matrix<ubyte>!
  bool fastRelabeling::apply(const matrix<ubyte>& src,
                             matrix<ubyte>& dest,
                             ivector& objSize,
                             std::vector<areaPoints>& objects) const {
    
    int numObjects;
    ivector equivLabels;
    const parameters& par = getParameters();

    internal::relabelingHelper<ubyte,ubyte> 
      helper(static_cast<ubyte>(max(0,par.minThreshold)),
             static_cast<ubyte>(min(255,par.maxThreshold)),
             par.assumeLabeledMask,
             par.fourNeighborhood);

    internal::relabelingHelper<ubyte,int> 
      ihelper(static_cast<ubyte>(max(0,par.minThreshold)),
             static_cast<ubyte>(min(255,par.maxThreshold)),
             par.assumeLabeledMask,
             par.fourNeighborhood);

    imatrix idest;
    int numObjs;

    if (ihelper.apply(src,idest,equivLabels,numObjects) &&
        helper.relabelWithArea(equivLabels,numObjects,idest,dest,objSize)) {
      if (par.sortSize) {
        if (helper.sortLabels(par.minimumObjectSize,0,objSize,numObjs,
                              dest,objects)) {
          objSize.resize(numObjs);
          return true;
        }
      } else {
        if (helper.suppress(par.minimumObjectSize,objSize,numObjs,
                            dest,objects)) {
          objSize.resize(numObjs);
          return true;
        }
      }
    }
    return false;
  }

  // On copy apply for type matrix<ubyte>!
  bool fastRelabeling::apply(const matrix<ubyte>& src,
                             imatrix& dest,
                             ivector& objSize,
                             std::vector<areaPoints>& objects) const {
    int numObjects;
    vector<int> equivLabels;
    const parameters& par = getParameters();

    internal::relabelingHelper<ubyte,int> 
      helper(static_cast<ubyte>(max(0,par.minThreshold)),
             static_cast<ubyte>(min(255,par.maxThreshold)),
             par.assumeLabeledMask,
             par.fourNeighborhood);

    int numObjs;

    if (helper.apply(src,dest,equivLabels,numObjects) &&
        helper.relabelWithArea(equivLabels,numObjects,dest,objSize)) {
      if (par.sortSize) {
        if (helper.sortLabels(par.minimumObjectSize,0,objSize,numObjs,
                              dest,objects)) {
          objSize.resize(numObjs);
          return true;
        }
      } else {
        if (helper.suppress(par.minimumObjectSize,objSize,numObjs,
                            dest,objects)) {
          objSize.resize(numObjs);
          return true;
        }
      }
    }

    return false;
  }

  // On copy apply for type imatrix!
  bool fastRelabeling::apply(const imatrix& src,
                             imatrix& dest,
                             ivector& objSize,
                             std::vector<areaPoints>& objects) const {
    int numObjects;
    vector<int> equivLabels;
    const parameters& par = getParameters();

    internal::relabelingHelper<int,int> helper(par.minThreshold,
                                               par.maxThreshold,
                                               par.assumeLabeledMask,
                                               par.fourNeighborhood);
    
    int numObjs;

    if (helper.apply(src,dest,equivLabels,numObjects) &&
        helper.relabelWithArea(equivLabels,numObjects,dest,objSize)) {

#if defined(_LTI_DEBUG) && (_LTI_DEBUG >= 3)
      static viewer2D view("Mask before suppress");;
      view.show(dest);
      view.waitKey();
#endif
      if (par.sortSize) {
        if (helper.sortLabels(par.minimumObjectSize,0,objSize,numObjs,
                              dest,objects)) {
          objSize.resize(numObjs);
          return true;
        }
      } else {
        if (helper.suppress(par.minimumObjectSize,objSize,numObjs,
                            dest,objects)) {
          objSize.resize(numObjs);
          return true;
        }
      }
    }

    return false;
  }

  //////////////////////

  // On copy apply for type matrix<ubyte>!
  bool fastRelabeling::partial(const matrix<ubyte>& src,
                                     matrix<ubyte>& dest,
                                     ivector& equivLabels,
                                     int& numObjects) const {
    
    const parameters& par = getParameters();

    internal::relabelingHelper<ubyte,ubyte> helper(par.minThreshold,
                                                   par.maxThreshold,
                                                   par.assumeLabeledMask,
                                                   par.fourNeighborhood);

    vector<ubyte> tmp;
    if (helper.apply(src,dest,tmp,numObjects)) {
      equivLabels.castFrom(tmp);
      return true;
    }
    return false;
  }

  // On copy apply for type matrix<ubyte>!
  bool fastRelabeling::partial(const matrix<ubyte>& src,
                                     imatrix& dest,
                                     ivector& equivLabels,
                                     int& numObjects) const {
    const parameters& par = getParameters();

    internal::relabelingHelper<ubyte,int> helper(par.minThreshold,
                                                 par.maxThreshold,
                                                 par.assumeLabeledMask,
                                                 par.fourNeighborhood);
    
    return helper.apply(src,dest,equivLabels,numObjects);
  }

  // On copy apply for type imatrix!
  bool fastRelabeling::partial(const imatrix& src, 
                                     imatrix& dest,
                                     ivector& equivLabels,
                                     int& numObjects) const {
    const parameters& par = getParameters();

    internal::relabelingHelper<int,int> helper(par.minThreshold,
                                               par.maxThreshold,
                                               par.assumeLabeledMask,
                                               par.fourNeighborhood);
    
    return helper.apply(src,dest,equivLabels,numObjects);
  }

  //sort integer labels
  bool fastRelabeling::sortLabels(const int minSize, 
                                  const int nLargest,
				  ivector& numPixels,
                                  int& newNumPixelsSize,
				  ivector& reindex) const {
    const parameters& par = getParameters();

    internal::relabelingHelper<int,int> helper(par.minThreshold,
                                               par.maxThreshold,
                                               par.assumeLabeledMask,
                                               par.fourNeighborhood);
    
    return helper.sortLabels(minSize, nLargest,
                             numPixels, newNumPixelsSize,reindex);
  }

  //sort integer labels
  bool fastRelabeling::suppress(const int minSize, 
                                      ivector& numPixels,
                                      int& newNumPixelsSize,
                                      ivector& reindex) const {
    const parameters& par = getParameters();

    internal::relabelingHelper<int,int> helper(par.minThreshold,
                                               par.maxThreshold,
                                               par.assumeLabeledMask,
                                               par.fourNeighborhood);
    
    newNumPixelsSize = helper.suppress(minSize, numPixels, reindex);

    return true;
  }

  
  // Just relabel the destination, but compute also the size of each object.
  bool fastRelabeling::relabelWithArea(const ivector& equivLabels,
				       const int numObjects,
				       imatrix& dest,
				       ivector& numPixels) const {
    const parameters& par = getParameters();
    
    internal::relabelingHelper<int,int> helper(par.minThreshold,
                                               par.maxThreshold,
                                               par.assumeLabeledMask,
                                               par.fourNeighborhood);
    
    return helper.relabelWithArea(equivLabels, numObjects, dest, numPixels);
  }
  
  /**
   * Just relabel the destination, but compute also the size of each object.
   */
  bool fastRelabeling::relabelWithArea(const vector<ubyte>& equivLabels,
				       const int numObjects,
				       matrix<ubyte>& dest,
				       ivector& numPixels) const {
    const parameters& par = getParameters();

    internal::relabelingHelper<ubyte,ubyte> 
      helper(static_cast<ubyte>(max(0,par.minThreshold)),
             static_cast<ubyte>(min(0,par.maxThreshold)),
             par.assumeLabeledMask,
             par.fourNeighborhood);
    
    return helper.relabelWithArea(equivLabels, numObjects, dest, numPixels);
  }

}
