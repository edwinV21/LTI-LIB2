/*
 * Copyright (C) 1998 - 2005
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


#include <limits>
#include <queue>

#include "ltiMath.h" // for odd()
#include "ltiRound.h" // for calculating circle points

#undef TIMING
//#define TIMING 1
#ifdef TIMING
# include "ltiTimer.h"
#endif

namespace lti {
  // ------------------------------
  // the helpers
  // ------------------------------

  // used in filter() for MinMax
  template <typename T>
  struct searchMinMax {
    bool isMin;
    bool isMax;
    searchMinMax() : isMin(true), isMax(true) {
    };

    // returns false if a greater or equal AND a less or equal point have been
    // given as b. The next call resets the state.
    inline bool operator()(const T a, const T b) {
      isMin = isMin && (a<b);
      isMax = isMax && (a>b);
      return isMin || isMax;
    }

    inline void reset() {
      isMin=true;
      isMax=true;
    }      
  };

  // simple wrapper used in all helper functions
  template <typename T>
  struct greaterStruct {
    inline bool operator()(const T a, const T b) {
      return a>b;
    }
    inline void reset(){}
  };

  // simple wrapper used in all helper functions
  template <typename T>
  struct lessStruct {
    inline bool operator()(const T& a, const T& b) {
      return a<b;
    }
    inline void reset(){}
  };

  // used as comparator in std::sort() calls for Max
  template <typename T>
  struct sortComparatorMax {
    const matrix<T>* data;
    sortComparatorMax(const matrix<T>& d) {data=&d;};
    inline bool operator()(const ipoint& a, const ipoint& b) const {
      return data->at(a) > data->at(b);
    };
  };    

  // used as comparator in std::sort() calls for Min
  template <typename T>
  struct sortComparatorMin {
    const matrix<T>* data;
    sortComparatorMin(const matrix<T>& d) {data=&d;};
    inline bool operator()(const ipoint& a, const ipoint& b) const {
      return data->at(a) < data->at(b);
    };
  };    

  // inserts extremes in the appropriate pointList
  // For Min and Max it is always the first list
  struct defaultInserter {
    template <class Function>
    inline void operator()(ipointList& pts, ipointList&, 
                           const int col, const int row,
                           Function) {
      pts.push_back(ipoint(col,row));
    }
  };

  // inserts extremes in the appropriate pointList
  // here the Function HAS TO BE searchMinMax
  // Its internal state is used to determine the correct list.
  struct minMaxInserter {
    template <class Function>
    inline void operator()(ipointList& pts, ipointList& pts2, 
                           const int col, const int row,
                           Function func) {
      if (func.isMin) {
        pts.push_back(ipoint(col,row));
      } else {
        pts2.push_back(ipoint(col,row));
      }
    }
  };



  // used as comparator in std::sort() calls for Max
  template <typename T>
  struct sortComparatorMaxV {
    const vector<T>* data;
    sortComparatorMaxV(const vector<T>& d) {data=&d;};
    inline bool operator()(const int& a, const int& b) const {
      return data->at(a) > data->at(b);
    };
  };    

  // used as comparator in std::sort() calls for Min
  template <typename T>
  struct sortComparatorMinV {
    const vector<T>* data;
    sortComparatorMinV(const vector<T>& d) {data=&d;};
    inline bool operator()(const int& a, const int& b) const {
      return data->at(a) < data->at(b);
    };
  };

  // inserts extremes in the appropriate pointList
  // For Min and Max it is always the first list
  struct defaultInserterI {
    template <class Function>
    inline void operator()(lti::list<int>& pts,
                           lti::list<int>&, 
                           const int idx,
                           Function) {
      pts.push_back(idx);
    }
  };

  // inserts extremes in the appropriate pointList
  // here the Function HAS TO BE searchMinMax
  // Its internal state is used to determine the correct list.
  struct minMaxInserterI {
    template <class Function>
    inline void operator()(lti::list<int>& pts,
                           lti::list<int>& pts2, 
                           const int idx,
                           Function func) {
      if (func.isMin) {
        pts.push_back(idx);
      } else {
        pts2.push_back(idx);
      }
    }
  };



  // used for NOT collecting min and max value of the matrix
  template <typename T>
  struct dummyCollector {
    inline void operator()(const T) {}
  };

  // used for collecting min and max value of the matrix
  template <typename T>
  struct collector {
    T minn;
    T maxx;
    collector() : minn(std::numeric_limits<T>::max()),
                  maxx(std::numeric_limits<T>::min()) {
    };
    inline void operator()(const T val) {
      if (val < minn) {
        minn=val;
      } else if (val > maxx) {
        maxx=val;
      }
    };
  };

  //defines regular square shape
  struct squareShape {
    int lastcol, limit;
    squareShape(const int cs, const int lim) 
      : lastcol(cs-1), limit(lim) {
    }
    inline int first(const int col, const int) const {
      return col-limit;
    } 
    inline int last(const int col, const int) const {
      return col+limit;
    }
    inline int firstBoundary(const int, const int) const {
      return 0;
    }
    inline int lastBoundary(const int, const int) const {
      return lastcol;
    } 
  };

  //defines circle shape
  struct circleShape {
    int lastcol, limit;
    int *circ, *circPoints;
    circleShape(const int cs, const int lim) 
      : lastcol(cs-1), limit(lim) {
      const int sz=2*limit+1;
      circ=new int[sz];

      int i,p;
      float rad2=sqr(static_cast<float>(limit));
      float tmp;

      for (i=0; i<sz; i++) {
        circ[i]=-1;
      }
      
      for (i=0; i<=limit; i++) {
        tmp=static_cast<float>(i);
        p=iround(sqrt(rad2-tmp*tmp));
        if (p>circ[i+limit]) {
          circ[i+limit]=p;
          circ[limit-i]=p;
        }
        if (i>circ[p+limit]) {
          circ[p+limit]=i;
          circ[limit-p]=i;
        }
      }
      circPoints=&circ[limit];
      
    }
    ~circleShape() {delete[] circ;}
    inline int first(const int col, const int i) const {
      return col-circPoints[i];
    } 
    inline int last(const int col, const int i) const {
      return col+circPoints[i];
    }
    inline int firstBoundary(const int pos, const int i) const {
      return max(0,first(pos,i));
    }
    inline int lastBoundary(const int pos, const int i) const {
      return min(lastcol,last(pos,i));
    } 
  };



  // -------------------------------------------------------------------
  // The apply-methods for matrices
  // -------------------------------------------------------------------

  template <typename T, class Compare, class Collector, class Shape>
  bool localExtremes::filter(const matrix<T>& src, 
                             ipointList& pts, 
                             Compare comp,
                             Collector& coll,
                             Shape shape) const {

#ifdef TIMING    
    static timer timo(timer::Wall);
    timo.start();
#endif

    if (src.empty()) {
      pts.clear();
      return true;
    }

    const parameters& param = getParameters();


    int i,j,row,col;

    const int slastCol = src.lastColumn();
    const int slastRow = src.lastRow();
    const int kernelSize = param.kernelSize;
    ipoint p;

    // put all points in the list(s) if there is 'no' kernel
    if (kernelSize < 3) {
      for (i=0; i<slastRow; i++) {
        p.y=i;
        for (j=0; j<slastCol; j++) {
          p.x=j;
          pts.push_back(p);
        }
      }
      return true;
    }


    const int sizeOfKernel = odd(kernelSize) ? kernelSize : kernelSize-1;
    const int limit = sizeOfKernel/2;

    const int lastCol  = slastCol-limit;
    const int lastRow  = slastRow-limit;
    bool insert;

    //The basic idea is to go through the kernel above the current row first
    //then do the part rows before and after the current point
    //and finally do the lower part of the kernel.
    //Like this there is no checking for i,j = row,col which would be bad.

    int clast;
    //runs through the src's columns
    for(row=limit;row<=lastRow;++row) { 
      const int r=row+limit;
      for(col=limit;col<=lastCol;++col) {  //runs through the src's rows

        // (row,col) is the current evaluated point.  Around it
        // we have to position the kernel, and check there if it
        // fullfills the requirements

        // check condition on the upper part of the kernel
        const int c=col+limit;
        const T val=src.at(row,col);
        i=row-limit;
        while (i<row) { //runs through the rows within the kernel
                        //runs through the columns within the kernel
          j=shape.first(col,i-row);
          clast=shape.last(col,i-row);
          while (j<=clast && comp(val,src.at(i,j))) {
            j++;
          }
          if (j<=clast) break;
          i++;
        }
        if (i<row) continue;

        // check condition on the middle part of the kernel
        j=col-limit;
        while (j<col && comp(val,src.at(i,j))) {
          j++;
        }
        if (j<col) continue;
        j++; // skip over actual position
        while (j<=c && comp(val,src.at(i,j))) {
          j++;
        }
        if (j<=c) continue;

        // bottom part of the kernel
        i=row+1;
        while (i<=r) { //runs through the rows within the kernel
          //runs through the columns within the kernel
          j=shape.first(col,i-row);
          clast=shape.last(col,i-row);
          while (j<=clast && comp(val,src.at(i,j))) {
            j++;
          }
          if (j<=clast) break;
          i++;
        }
        if (i<=r) continue;

        pts.push_back(p.set(col,row));
        coll(val);
      }
    }

    ///////////////////////
    // boundary conditions
    ///////////////////////

    eBoundaryType boundary = param.boundaryType;
    
    // 
    const int sureRow = min(limit,slastRow);
    const int sureCol = min(limit,slastCol);

    if (boundary == NoBoundary) {
      return true;
    } else if (boundary==Periodic) {
      // actually it makes no sense to use periodic boundaries here
      setStatusString("Periodic boundaryType not implemented in localExtremes");
      return false;
    }
    
    // for Constant and Mirror nothing special needs to be done.
    // for Zero we need to call comp() with T(0) once for each pixel
    // better a bit of overhead then duplicating this much code
    // Using Zero also makes no sense, but you never know.
    // Actually that means that only negativ Mins can be found in the
    // border regions.

    for(row=limit;row<=lastRow;++row) { 

      const int r=row+limit;

      //runs the left columns
      for(col=0;col<sureCol;++col) {
        const int c=min(col+limit,slastCol);
        const T val=src.at(row,col);
        insert = true;
        i=row-limit;
        if (boundary==Zero) {
          insert=comp(val,0);
        }

        // top part of the kernel
        while (insert && i<row) { //runs through the rows within the kernel
          //runs through the columns within the kernel
          j=max(0,shape.firstBoundary(col,i-row));
          clast=min(shape.last(col,i-row),slastCol);
          while (insert && j<=clast) {
            insert=comp(val,src.at(i,j));
            j++;
          }
          i++;
        }
        // the current row, left part
        j=0;
        while (insert && j<col) {
          insert=comp(val,src.at(i,j));
          j++;
        }
        j++; // jump over the current position

        // the current row, right part
        while (insert && j<=c) {
          insert=comp(val,src.at(i,j));
          j++;
        }

        // next row
        i=row+1;

        // bottom part of the kernel
        while (insert && i<=r) { 
          
          j=max(0,shape.firstBoundary(col,i-row));
          clast=min(shape.last(col,i-row),slastCol);
          while (insert && j<=clast) {
            insert=comp(val,src.at(i,j));
            j++;
          }
          i++;
        }
        if (insert) {
          pts.push_back(p.set(col,row));
        }
        coll(val);
      }

      //runs the right columns
      for(col=max(0,lastCol+1);col<=slastCol;++col) {

        const T val=src.at(row,col);
        insert = true;
        i=row-limit;
        if (boundary==Zero) {
          insert=comp(val,0);
        }

        // top part of the kernel
        while (insert && i<row) {
          //runs through the columns within the kernel
          j=max(0,shape.first(col,i-row));
          clast=min(shape.lastBoundary(col,i-row),slastCol);
          while (insert && j<=clast) {
            insert=comp(val,src.at(i,j));
            j++;
          }
          i++;
        }
        // current column, left part
        j=max(0,col-limit);
        while (insert && j<col) {
          insert=comp(val,src.at(i,j));
          j++;
        }
        j++; // jump over current position
        while (insert && j<=slastCol) {
          insert=comp(val,src.at(i,j));
          j++;
        }
        i=row+1; // next row of the kernel at bottom

        // bottom part of the kernel
        while (insert && i<=r) { //runs through the rows within the kernel
          //runs through the columns within the kernel
          j=max(0,shape.first(col,i-row));
          clast=min(shape.lastBoundary(col,i-row),slastCol);
          while (insert && j<=clast) {
            insert=comp(val,src.at(i,j));
            j++;
          }
          i++;
        }
        if (insert) {
          pts.push_back(p.set(col,row));
        }
        coll(val);
      }
    }

    // The very top of the image

    for(row=0;row<sureRow;++row) { //runs the rows at the top
      const int r=min(row+limit,slastRow);
      // runs at the top,left in the corner
      for(col=0;col<sureCol;++col) {
        const int c=min(col+limit,slastCol);
        const T val=src.at(row,col);
        insert = true;
        i=0;
        if (boundary==Zero) {
          insert=comp(val,0);
        }

        // top of the kernel
        while (insert && i<row) { //runs through the rows within the kernel
          //runs through the columns within the kernel
          j=max(0,shape.firstBoundary(col,i-row));
          clast=min(shape.last(col,i-row),slastCol);
          while (insert && j<=clast) {
            insert=comp(val,src.at(i,j));
            j++;
          }
          i++;
        }

        // the current row, on the left
        j=0;
        while (insert && j<col) {
          insert=comp(val,src.at(i,j));
          j++;
        }
        j++; // jump over current position

        // the current row, o the right
        while (insert && j<=c) {
          insert=comp(val,src.at(i,j));
          j++;
        }
        i=row+1; // initialize bottom of the kernel

        // bottom part of the kernel
        while (insert && i<=r) { //runs through the rows of the kernel
          //runs through the columns of the kernel
          j=max(0,shape.firstBoundary(col,i-row));
          clast=min(shape.last(col,i-row),slastCol);
          while (insert && j<=clast) {
            insert=comp(val,src.at(i,j));
            j++;
          }
          i++;
        }
        if (insert) {
          pts.push_back(p.set(col,row));
        }
        coll(val);
      }

      // runs the rows at the top in the middle
      for(col=limit;col<=lastCol;++col) {
        const int c=col+limit;
        const T val=src.at(row,col);
        insert = true;
        i=0;
        if (boundary==Zero) {
          insert=comp(val,0);
        }

        // top part of the kernel
        while (insert && i<row) { //runs through the rows within the kernel
          //runs through the columns within the kernel
          j=shape.first(col,i-row);
          clast=shape.last(col,i-row);
          while (insert && j<=clast) {
            insert=comp(val,src.at(i,j));
            j++;
          }
          i++;
        }

        // on the row line, left part
        j=col-limit;
        while (insert && j<col) {
          insert=comp(val,src.at(i,j));
          j++;
        }
        j++; // jump over the current position

        // on the current row, right part
        while (insert && j<=c) {
          insert=comp(val,src.at(i,j));
          j++;
        }
        i=row+1;
        // bottom part of the kernel
        while (insert && i<=r) { //runs through the rows within the kernel
          //runs through the columns within the kernel
          j=shape.first(col,i-row);
          clast=shape.last(col,i-row);
          while (insert && j<=clast) {
            insert=comp(val,src.at(i,j));
            j++;
          }
          i++;
        }
        if (insert) {
          pts.push_back(p.set(col,row));
        }
        coll(val);
      }
        
      //runs at the top,right in the corner
      for(col=lastCol+1;col<=src.lastColumn();++col) {
        const T val=src.at(row,col);
        insert = true;
        i=0;
        if (boundary==Zero) {
          insert=comp(val,0);
        }
        
        // top part of the kernel
        while (insert && i<row) { //runs through the rows within the kernel
          //runs through the columns within the kernel
          j=max(0,shape.first(col,i-row));
          clast=min(shape.lastBoundary(col,i-row),slastCol);
          while (insert && j<=clast) {
            insert=comp(val,src.at(i,j));
            j++;
          }
          i++;
        }
        
        // on the current row, left part
        j=max(0,col-limit);
        while (insert && j<col) {
          insert=comp(val,src.at(i,j));
          j++;
        }
        j++; // jump over current position

        // on the current row, right part
        while (insert && j<=slastCol) {
          insert=comp(val,src.at(i,j));
          j++;
        }

        // bottom part of the kernel
        i=row+1;
        while (insert && i<=r) { //runs through the rows within the kernel
          //runs through the columns within the kernel
          j=max(0,shape.first(col,i-row));
          clast=min(shape.lastBoundary(col,i-row),slastCol);
          while (insert && j<=clast) {
            insert=comp(val,src.at(i,j));
            j++;
          }
          i++;
        }
        if (insert) {
          pts.push_back(p.set(col,row));
        }
        coll(val);
      }
    }


    //runs the rows at the bottom
    for(row=max(0,lastRow+1);row<=slastRow;++row) {

      //runs at the bottom,left in the corner
      for(col=0;col<sureCol;++col) {
        const int c=min(col+limit,slastCol);
        const T val=src.at(row,col);
        insert = true;
        i=max(0,row-limit);
        if (boundary==Zero) {
          insert=comp(val,0);
        }

        // top part of the kernel
        while (insert && i<row) { //runs through the rows within the kernel
          //runs through the columns within the kernel
          j=max(0,shape.firstBoundary(col,i-row));
          clast=min(shape.last(col,i-row),slastCol);
          while (insert && j<=clast) {
            insert=comp(val,src.at(i,j));
            j++;
          }
          i++;
        }
        
        // current row, left side
        j=0;
        while (insert && j<col) {
          insert=comp(val,src.at(i,j));
          j++;
        }
        j++; // jump over current position

        // current row, right side
        while (insert && j<=c) {
          insert=comp(val,src.at(i,j));
          j++;
        }
        
        // bottom part of the kernel
        i=row+1;
        while (insert && i<=slastRow) { //runs through the rows of the kernel
          //runs through the columns within the kernel
          j=min(0,shape.firstBoundary(col,i-row));
          clast=max(shape.last(col,i-row),slastCol);
          while (insert && j<=clast) {
            insert=comp(val,src.at(i,j));
            j++;
          }
          i++;
        }
        if (insert) {
          pts.push_back(p.set(col,row));
        }
        coll(val);
      }
        
      //runs the rows at the bottom in the middle
      for(col=limit;col<=lastCol;++col) {
        const int c=col+limit;
        const T val=src.at(row,col);
        insert = true;
        i=max(0,row-limit);
        if (boundary==Zero) {
          insert=comp(val,0);
        }

        // top part of the kernel
        while (insert && i<row) { //runs through the rows within the kernel
          //runs through the columns within the kernel
          j=shape.first(col,i-row);
          clast=shape.last(col,i-row);
          while (insert && j<=clast) {
            insert=comp(val,src.at(i,j));
            j++;
          }
          i++;
        }

        // current row, left side
        j=col-limit;
        while (insert && j<col) {
          insert=comp(val,src.at(i,j));
          j++;
        }
        j++; // jump over current position
        while (insert && j<=c) {
          insert=comp(val,src.at(i,j));
          j++;
        }

        // bottom part of the kernel
        i=row+1;
        while (insert && i<=slastRow) {
          j=shape.first(col,i-row);
          clast=shape.last(col,i-row);
          while (insert && j<=clast) {
            insert=comp(val,src.at(i,j));
            j++;
          }
          i++;
        }
        if (insert) {
          pts.push_back(p.set(col,row));
        }
        coll(val);
      }
        
      //runs at the bottom,right in the corner
      for(col=max(0,lastCol+1);col<=slastCol;++col) {
        const T val=src.at(row,col);
        insert = true;
        i=max(0,row-limit);
        if (boundary==Zero) {
          insert=comp(val,0);
        }

        // top part of the kernel
        while (insert && i<row) { //runs through the rows within the kernel
          //runs through the columns within the kernel
          j=max(0,shape.first(col,i-row));
          clast=min(shape.lastBoundary(col,i-row),slastCol);
          while (insert && j<=clast) {
            insert=comp(val,src.at(i,j));
            j++;
          }
          i++;
        }

        // current row, left part
        j=max(0,col-limit);
        while (insert && j<col) {
          insert=comp(val,src.at(i,j));
          j++;
        }
        j++; // jump over current position

        // current row, right part
        while (insert && j<=slastCol) {
          insert=comp(val,src.at(i,j));
          j++;
        }

        // bottom part of the kernel
        i=row+1;
        while (insert && i<=slastRow) { //runs through the rows of the kernel

          //runs through the columns within the kernel
          j=max(0,shape.first(col,i-row));
          clast=min(shape.lastBoundary(col,i-row),slastCol);
          while (insert && j<=clast) {
            insert=comp(val,src.at(i,j));
            j++;
          }
          i++;
        }
        if (insert) {
          pts.push_back(p.set(col,row));
        }
        coll(val);
      }
    }
#ifdef TIMING    
    timo.stop();
    std::cerr << "filter(): " << timo.getTime() << "\n";
#endif
    return true;
  }

  template <typename T, class ComparatorThresh, class Data, class Idxs>
  bool localExtremes::threshold(const Data& src,
                                Idxs& pts, 
                                T& minn, T& maxx,
                                ComparatorThresh compT) const {
#ifdef TIMING    
    static timer timo(timer::Wall);
    timo.start();
#endif

    // nothing todo on empty list
    if (pts.empty()) {
      return true;
    }
    // nothing todo for one element
    typename Idxs::const_iterator oneit=pts.begin();
    oneit++;
    if (oneit==pts.end()) {
      return true;
    }

    const parameters& param=getParameters();
    const float threshFac=param.relativeThreshold;

    // erase thresholded values
    T thresh;
    if (compT(1,0)) {
      thresh=static_cast<T>(maxx-threshFac*(maxx-minn));
    } else {
      thresh=static_cast<T>(minn+threshFac*(maxx-minn));
    } 
    typename Idxs::iterator tit=pts.begin();
    const typename Idxs::iterator tite=pts.end();
    while (tit!=tite) {
      if (compT(thresh,src.at(*tit))) {
        tit=pts.erase(tit);
      } else {
        tit++;
      }
    }
    
#ifdef TIMING    
    timo.stop();
    std::cerr << "threshold(): " << timo.getTime() << "\n";
#endif
    return true;
  }


  // Do hystheresis but keep points if an equal point is found.
  //
  // First the points are sorted so that the most extremal is first.
  // Starting from the second point in the list a neighborhood search is
  // performed bounded by the threshold.
  // The searched area of each point is marked with a unique, ascending 
  // marker and the highest value found is recorded. This can be either
  // the starting point's value or the value leading to the deletion of the
  // starting point. This is needed to deal with regions with equal starting
  // points.
  // The neighborhood search can be stopped as well when a region with a lower
  // marker value is encountered. The current starting value can only be less
  // or equally extreme than the one of the encountered region. Unless the 
  // recorded value is equal to the current value the current point is deleted.
  // Otherwise, the search is stopped as well since no more extremal point can
  // be found.
  template <typename T, class Compare, class CompareSort> 
  bool localExtremes::hystheresis(const matrix<T>& src,
                                  ipointList& pts,
                                  const T minn, const T maxx,
                                  Compare comp,
                                  CompareSort compS) const {

#ifdef TIMING    
    static timer timo(timer::Wall);
    timo.start();
#endif

    const parameters& par=getParameters();
    const float hthresh = par.hystheresisThreshold;
    const int rows=src.rows();
    const int cols=src.columns();

    // need to know whether we're dealing with maxima or minima
    const bool maxSelected=comp(1,0);

    // sort before hystheresis
    pts.sort(compS);

    T actualExtr,actualThresh,v;
    int j;

    // records already visited places.
    // each starting point has its own marker
    imatrix mask(src.size(),std::numeric_limits<int>::max());
    int marker=0;

    // neighborhood
    static const int deltay[] = {-1,-1,-1,0,0,1,1,1};
    static const int deltax[] = {-1,0,1,-1,1,-1,0,1};

    ipoint p,dummy;
    // queue is faster than smallObjectList!
    std::queue<ipoint> todo;
    ipointList::iterator it=pts.begin();
    const ipointList::iterator ite=pts.end();

    // if true erase current point
    bool eraseIt;

    // this vector tracks the extrmal value to each region (marker) searched
    // for deleted points its the value stopping the process for not deleted
    // ones the starting value. This is needed for equal starting values.
    std::vector<T> regionExtr;
    
    // the first one is kept by definition
    actualExtr=src.at(*it);
    regionExtr.push_back(actualExtr);
    it++;
    marker++;

    while (it!=ite) {

      eraseIt=false;
      p = (*it);
      actualExtr = src.at(p);
      regionExtr.push_back(actualExtr);

      if (maxSelected) {
        actualThresh = static_cast<T>(actualExtr-hthresh*(actualExtr-minn));
      } else {
        actualThresh = static_cast<T>(actualExtr+hthresh*(maxx-actualExtr));
      } 
      todo.push(p);
      mask.at(p) = marker; // flag: already evaluated

      while (!todo.empty()) { // do region search
        // get next point to be evaluated
        p = todo.front();
        todo.pop();
        v = src.at(p);

        // check for more extremal value than starting point
        if (comp(v,actualExtr)) {
          eraseIt=true;
          // clear todo
          while (!todo.empty()) {
            todo.pop();
          }
          regionExtr[marker]=v;
          break;
        } 

        for (j=0;j<8;j++) {
          const int tx = p.x+deltax[j];
          const int ty = p.y+deltay[j];
          
          // out of bounds?
          if ((tx < 0) || (tx >= cols) ||
              (ty < 0) || (ty >= rows)) continue;
          
          int& cmask=mask.at(ty,tx);
          // check for encounter with already searched region
          if (cmask<marker) {
            // keep equally valued points but erase others
            if (regionExtr[cmask]!=actualExtr) {
              eraseIt=true;
              regionExtr[marker]=regionExtr[cmask];
            }
            // clear todo
            while (!todo.empty()) {
              todo.pop();
            }
            break;
          } else if (cmask==marker) {   // already queued?
            continue;
          }
          
          // only look at points more extreme than actualThresh
          if (comp(src.at(ty,tx),actualThresh)) {
            todo.push(dummy.set(tx,ty));
            cmask = marker;
          }
        }
      }

      if (eraseIt) {
        it=pts.erase(it);
      } else {
        it++;
      }
      marker++;

    }

#ifdef TIMING    
    timo.stop();
    std::cerr << "hystheresis(): " << timo.getTime() << "\n";
#endif
    return true;
  }

  // Do hystheresis strictly, i.e. no equal values are tolerated.
  //
  // First the points are sorted so that the most extremal is first.
  // Starting from the second point in the list a neighborhood search is
  // performed bounded by the threshold.
  // The searched area of each point is marked with a unique, ascending 
  // marker.
  // The neighborhood search terminates when the queue is empty with no change.
  // The current point is deleted when a more extreme or equal value is found
  // or a region with a lower marker value is encountered.
  template <typename T, class Compare, class CompareSort> 
  bool localExtremes::hystheresisStrict(const matrix<T>& src,
                                        ipointList& pts,
                                        const T minn, const T maxx,
                                        Compare comp,
                                        CompareSort compS) const {

#ifdef TIMING    
    static timer timo(timer::Wall);
    timo.start();
#endif

    const parameters& par=getParameters();
    const float hthresh = par.hystheresisThreshold;
    const int rows=src.rows();
    const int cols=src.columns();

    // need to know whether we're dealing with maxima or minima
    const bool maxSelected=comp(1,0);

    // sort before hystheresis
    pts.sort(compS);

    T actualExtr,actualThresh,v;
    int j;

    // records already visited places.
    // each starting point has its own marker
    imatrix mask(src.size(),std::numeric_limits<int>::max());
    int marker=0;

    // neighborhood
    static const int deltay[] = {-1,-1,-1,0,0,1,1,1};
    static const int deltax[] = {-1,0,1,-1,1,-1,0,1};

    ipoint p, actualPos, dummy;
    std::queue<ipoint> todo;
    ipointList::iterator it=pts.begin();
    const ipointList::iterator ite=pts.end();

    // if true erase current point
    bool eraseIt;
    
    while (it!=ite) {

      eraseIt=false;
      p = (*it);
      actualExtr = src.at(p);
      actualPos = p;
      //       std::cout << marker << ": " << p << "  " << actualExtr << "\n";

      if (maxSelected) {
        actualThresh = static_cast<T>(actualExtr-hthresh*(actualExtr-minn));
      } else {
        actualThresh = static_cast<T>(actualExtr+hthresh*(maxx-actualExtr));
      } 
      todo.push(p);
      mask.at(p) = marker; // flag: already evaluated

      while (!todo.empty()) { // do region search
        // get next point to be evaluated
        p = todo.front();
        todo.pop();
        v = src.at(p);

        // check for more extremal value than starting point
        // check for equal value but different point
        if ( (comp(v,actualExtr)) ||
             ((v==actualExtr) && (p!=actualPos))) {
          eraseIt=true;
          // clear todo
          while (!todo.empty()) {
            todo.pop();
          }
          break;
        } 
        
        for (j=0;j<8;j++) {
          const int tx = p.x+deltax[j];
          const int ty = p.y+deltay[j];
          
          // out of bounds?
          if ((tx < 0) || (tx >= cols) ||
              (ty < 0) || (ty >= rows)) continue;
          
          int& cmask=mask.at(ty,tx);
          // check for encounter with already searched region
          // then search no more and go to next point
          if (cmask<marker) {
            eraseIt=true;
            // clear todo
            while (!todo.empty()) {
              todo.pop();
            }
            //             std::cout << tx << "," << ty << "  marker: " << cmask << "\n";
            break;
          } else if (cmask==marker) { // already queued?
            continue;
          }
          
          // only look at points more extreme than actualThresh
          if (comp(src.at(ty,tx),actualThresh)) {
            todo.push(dummy.set(tx,ty));
            cmask = marker;
            //             std::cout << tx << "," << ty << "  val: " << src.at(ty,tx) << "\n";
          }
        }
      }

      if (eraseIt) {
        it=pts.erase(it);
        //         std::cout << "pos: " << p
        //                   << "  mask: " << mask.at(p)
        //                   << "  value: " << v << "\n\n";
      } else {
        it++;
        //         std::cout << "kept\n\n";
      }
      marker++;

    }

#ifdef TIMING    
    timo.stop();
    std::cerr << "hystheresisStrict(): " << timo.getTime() << "\n";
#endif
    return true;
  }

  // On copy apply for type matrix<T>!
  template <typename T>
  bool localExtremes::worker(const eExtremesType extremesType,
                             const matrix<T>& src,
                             ipointList& pts,
                             ipointList& pts2) const {

#ifdef TIMING    
    static timer timo(timer::Wall);
#endif

    pts.clear();
    pts2.clear();
    ipointList::const_iterator it,ite;

    const parameters& param=getParameters();
    const bool doThreshold=param.relativeThreshold>0 
      && param.relativeThreshold<1;
    const bool doHystheresis=param.hystheresisThreshold>0 
      && param.hystheresisThreshold<1;
    bool needMinMax=doThreshold || doHystheresis;
    collector<T> coll;
    dummyCollector<T> dcoll;
    const int limit=param.kernelSize/2;
    
    switch (extremesType) {
    case Max: {
      sortComparatorMax<T> compS(src);
      greaterStruct<T> greater;
      
      if (needMinMax) {
        if (param.kernelShape==Square) {
          filter(src, pts, greater, coll, squareShape(src.columns(), limit));
        } else {
          filter(src, pts, greater, coll, circleShape(src.columns(), limit));
        }
      } else {
        if (param.kernelShape==Square) {
          filter(src,pts, greater, dcoll, squareShape(src.columns(), limit));
        } else {
          filter(src,pts, greater, dcoll, circleShape(src.columns(), limit));
        }
      }
      
      if (doThreshold) {
        threshold(src, pts, coll.minn, coll.maxx, greater); 
      }
      
      if (doHystheresis) {
        if (param.strictHystheresis) {
          hystheresisStrict(src,pts,coll.minn,coll.maxx,greater,compS);
        } else {
          hystheresis(src, pts, coll.minn, coll.maxx, greater, compS);
        }
      }
      
#ifdef TIMING    
      timo.start();
#endif
      pts.sort(compS);
#ifdef TIMING    
      timo.stop();
      std::cerr << "sorting: " << timo.getTime() << "\n";
#endif
    } break;
    case Min: {
      sortComparatorMin<T> compS(src);
      lessStruct<T> less;
      
      if (needMinMax) {
        if (param.kernelShape==Square) {
          filter(src, pts, less, coll, squareShape(src.columns(), limit));
        } else {
          filter(src, pts, less, coll, circleShape(src.columns(), limit));
        }
      } else {
        if (param.kernelShape==Square) {
          filter(src, pts, less, dcoll, squareShape(src.columns(), limit));
        } else {
          filter(src, pts, less, dcoll, circleShape(src.columns(), limit));
        }
      }
      
      if (doThreshold) {
        threshold(src, pts, coll.minn, coll.maxx, less); 
      }
      
      if (doHystheresis) {
        if (param.strictHystheresis) {
          hystheresisStrict(src, pts, coll.minn, coll.maxx, less, compS);
        } else {
          hystheresis(src, pts, coll.minn, coll.maxx, less, compS);
        }
      }
      
#ifdef TIMING    
      timo.start();
#endif
      pts.sort(compS);
#ifdef TIMING    
      timo.stop();
      std::cerr << "sorting: " << timo.getTime() << "\n";
#endif
    } break;
    case MinMax: {
      sortComparatorMax<T> compSMax(src);
      sortComparatorMin<T> compSMin(src);
      lessStruct<T> less;
      greaterStruct<T> greater;
      
      if (needMinMax) {
        if (param.kernelShape==Square) {
          filter(src, pts, less, coll, squareShape(src.columns(), limit));
          filter(src, pts2,greater,dcoll, squareShape(src.columns(), limit));
        } else {
          filter(src, pts, less, coll, circleShape(src.columns(), limit));
          filter(src, pts2,greater,dcoll, circleShape(src.columns(), limit));
        }
      } else {
        if (param.kernelShape==Square) {
          filter(src, pts, less, dcoll, squareShape(src.columns(), limit));
          filter(src, pts2,greater,dcoll, squareShape(src.columns(), limit));
        } else {
          filter(src, pts, less, dcoll, circleShape(src.columns(), limit));
          filter(src, pts2,greater,dcoll, circleShape(src.columns(), limit));
        }
      }

      std::cerr << "mins: " << pts.size() 
                << "  maxs: " << pts2.size() << "\n";

      if (doThreshold) {
        threshold(src, pts,  coll.minn, coll.maxx, less); 
        threshold(src, pts2, coll.minn, coll.maxx, greater);
      }

      std::cerr << "mins: " << pts.size() 
                << "  maxs: " << pts2.size() << "\n";

      if (doHystheresis) {
        if (param.strictHystheresis) {
          hystheresisStrict(src,pts, coll.minn,coll.maxx,less,compSMin);
          hystheresisStrict(src,pts2,coll.minn,coll.maxx,greater,compSMax);
        } else {
          hystheresis(src, pts,  coll.minn, coll.maxx, less, compSMin);
          hystheresis(src, pts2, coll.minn, coll.maxx, greater, compSMax);
        }
      }

      std::cerr << "mins: " << pts.size() 
                << "  maxs: " << pts2.size() << "\n";

      //sort points
#ifdef TIMING    
      timo.start();
#endif
      pts.sort(compSMin);
      pts2.sort(compSMax);
#ifdef TIMING    
      timo.stop();
      std::cerr << "sorting: " << timo.getTime() << "\n";
#endif
    } break;
    } // end of switch

    clipPoints(pts, param.maxNumber);
    clipPoints(pts2, param.maxNumber);

    return true;
  }

  template <typename T>
  bool localExtremes::apply(const matrix<T>& src,
                            ipointList& pts,
                            ipointList& pts2) const {

    return worker(MinMax, src, pts, pts2);
  }

  // On copy apply for type matrix<T>!
  template <typename T>
  bool localExtremes::apply(const matrix<T>& src,
                            ipointList& pts) const {
    ipointList pts2;
    bool rc = worker(getParameters().extremesType,src, pts, pts2);
    // insert all points from pts2 before the pts.end().
    pts.splice(pts.end(),pts2);
    return rc;
  }



  // -------------------------------------------------------------------
  // The apply-methods for vectors
  // -------------------------------------------------------------------

  
  template <typename T, class Compare, class Collector>
  bool localExtremes::filter(const vector<T>& src, 
                             lti::list<int>& pts, 
                             Compare comp,
                             Collector& coll) const {

#ifdef TIMING    
    static timer timo(timer::Wall);
    timo.start();
#endif

    if (src.empty()) {
      pts.clear();
      return true;
    }

    const parameters& param = getParameters();

    int j,idx;

    const int slastIdx = src.lastIdx();
    const int kernelSize = param.kernelSize;

    // put all points in the list(s) if there is 'no' kernel
    if (kernelSize < 3) {
      for (j=0; j<slastIdx; ++j) {
        pts.push_back(j);
      }
      return true;
    }

    const int sizeOfKernel = odd(kernelSize) ? kernelSize : kernelSize-1;
    const int limit = sizeOfKernel/2;
    const int lastIdx  = max(0,slastIdx-limit);
    bool insert;
    // int clast;

    for(idx=limit;idx<=lastIdx;++idx) {  //runs through the src's rows

      // idx hold the evaluated position.  We have to check if it fulfills
      //     the condition on the kernel around it.
      

      const int c=idx+limit;
      const T val=src.at(idx);

      //runs through the values on the left side of idx
      j=idx-limit;
      // clast=idx+limit;

      while (j<idx && comp(val,src.at(j))) {
        j++;
      }
      
      if (j<idx) continue; // condition not met on the way, try next!

      j++; // skip over actual position

      while (j<=c && comp(val,src.at(j))) {
        j++;
      }
       
      if (j<=c) continue; // condition not met on the way, try next!

      pts.push_back(idx); // if we survived, then we got an extreme
      coll(val); // keeps track of value interval
    }

    ///////////////////////
    // boundary conditions
    ///////////////////////

    eBoundaryType boundary = param.boundaryType;
    
    // 
    const int sureIdx = min(limit,slastIdx);

    if (boundary == NoBoundary) {
      return true;
    } else if (boundary==Periodic) {
      // actually it makes no sense to use periodic boundaries here
      setStatusString("Periodic boundaryType not implemented in localExtremes");
      return false;
    }
    
    // for Constant and Mirror nothing special needs to be done.
    // for Zero we need to call comp() with T(0) once for each pixel
    // better a bit of overhead then duplicating this much code
    // Using Zero also makes no sense, but you never know.
    // Actually that means that only negative Mins can be found in the
    // border regions.

    // left side
    for(idx=0;idx<sureIdx;++idx) {
      
      // idx holds evaluated position

      const int c=min(idx+limit,slastIdx);
      const T val=src.at(idx);
      insert = true;
      if (boundary==Zero) {
        insert=comp(val,0);
      }

      // evaluate the left side
      j=0;
      while (insert && j<idx) {
        insert=comp(val,src.at(j));
        j++;
      }
      // jump over current position
      j++;

      // evaluate the right side
      while (insert && j<=c) {
        insert=comp(val,src.at(j));
        j++;
      }

      if (insert) {
        pts.push_back(idx);
      }
      coll(val);
    }

    // right side
    for(idx=lastIdx+1;idx<=slastIdx;++idx) {

      // idx holds evaluated position

      const T val=src.at(idx);
      insert = true;
      if (boundary==Zero) {
        insert=comp(val,0);
      }

      // left side
      j=max(0,idx-limit);
      while (insert && j<idx) {
        insert=comp(val,src.at(j));
        j++;
      }
      j++; // jump over current position
      // right side
      while (insert && j<=slastIdx) {
        insert=comp(val,src.at(j));
        j++;
      }
      if (insert) {
        pts.push_back(idx);
      }
      coll(val);
    }

#ifdef TIMING    
    timo.stop();
    std::cerr << "filter(): " << timo.getTime() << "\n";
#endif
    return true;
  }

  // Do hystheresis but keep points if an equal point is found.
  //
  // First the points are sorted so that the most extremal is first.
  // Starting from the second point in the list a neighborhood search is
  // performed bounded by the threshold.
  // The searched area of each point is marked with a unique, ascending 
  // marker and the highest value found is recorded. This can be either
  // the starting point's value or the value leading to the deletion of the
  // starting point. This is needed to deal with regions with equal starting
  // points.
  // The neighborhood search can be stopped as well when a region with a lower
  // marker value is encountered. The current starting value can only be less
  // or equally extreme than the one of the encountered region. Unless the 
  // recorded value is equal to the current value the current point is deleted.
  // Otherwise, the search is stopped as well since no more extremal point can
  // be found.
  template <typename T, class Compare, class CompareSort> 
  bool localExtremes::hystheresis(const vector<T>& src,
                                  lti::list<int>& pts,
                                  const T minn, const T maxx,
                                  Compare comp,
                                  CompareSort compS) const {

#ifdef TIMING    
    static timer timo(timer::Wall);
    timo.start();
#endif

    const parameters& par=getParameters();
    const float hthresh = par.hystheresisThreshold;
    const int size=src.size();

    // need to know whether we're dealing with maxima or minima
    const bool maxSelected=comp(1,0);

    // sort before hystheresis
    pts.sort(compS);

    T actualExtr,actualThresh,v;
    int j;

    // records already visited places.
    // each starting point has its own marker
    ivector mask(src.size(),std::numeric_limits<int>::max());
    int marker=0;

    // neighborhood
    static const int deltax[] = {-1,1};

    int p;

    // queue is faster than smallObjectList!
    std::queue<int> todo;
    lti::list<int>::iterator it=pts.begin();
    const lti::list<int>::iterator ite=pts.end();

    // if true erase current point
    bool eraseIt;

    // this vector tracks the extrmal value to each region (marker) searched
    // for deleted points its the value stopping the process for not deleted
    // ones the starting value. This is needed for equal starting values.
    std::vector<T> regionExtr;
    
    // the first one is kept by definition
    actualExtr=src.at(*it);
    regionExtr.push_back(actualExtr);
    it++;
    marker++;

    while (it!=ite) {

      eraseIt=false;
      p = (*it);
      actualExtr = src.at(p);
      regionExtr.push_back(actualExtr);

      if (maxSelected) {
        actualThresh = static_cast<T>(actualExtr-hthresh*(actualExtr-minn));
      } else {
        actualThresh = static_cast<T>(actualExtr+hthresh*(maxx-actualExtr));
      } 
      todo.push(p);
      mask.at(p) = marker; // flag: already evaluated

      while (!todo.empty()) { // do region search
        // get next point to be evaluated
        p = todo.front();
        todo.pop();
        v = src.at(p);

        // check for more extremal value than starting point
        if (comp(v,actualExtr)) {
          eraseIt=true;
          // clear todo
          while (!todo.empty()) {
            todo.pop();
          }
          regionExtr[marker]=v;
          break;
        } 

        for (j=0;j<2;j++) {
          const int tx = p+deltax[j];
          
          // out of bounds?
          if ((tx < 0) || (tx >= size)) continue;
          
          int& cmask=mask.at(tx);
          // check for encounter with already searched region
          if (cmask<marker) {
            // keep equally valued points but erase others
            if (regionExtr[cmask]!=actualExtr) {
              eraseIt=true;
              regionExtr[marker]=regionExtr[cmask];
            }
            // clear todo
            while (!todo.empty()) {
              todo.pop();
            }
            break;
          } else if (cmask==marker) {   // already queued?
            continue;
          }
          
          // only look at points more extreme than actualThresh
          if (comp(src.at(tx),actualThresh)) {
            todo.push(tx);
            cmask = marker;
          }
        }
      }

      if (eraseIt) {
        it=pts.erase(it);
      } else {
        it++;
      }
      marker++;

    }

#ifdef TIMING    
    timo.stop();
    std::cerr << "hystheresis(): " << timo.getTime() << "\n";
#endif
    return true;
  }

  // Do hystheresis strictly, i.e. no equal values are tolerated.
  //
  // First the points are sorted so that the most extremal is first.
  // Starting from the second point in the list a neighborhood search is
  // performed bounded by the threshold.
  // The searched area of each point is marked with a unique, ascending 
  // marker.
  // The neighborhood search terminates when the queue is empty with no change.
  // The current point is deleted when a more extreme or equal value is found
  // or a region with a lower marker value is encountered.
  template <typename T, class Compare, class CompareSort> 
  bool localExtremes::hystheresisStrict(const vector<T>& src,
                                        lti::list<int>& pts,
                                        const T minn, const T maxx,
                                        Compare comp,
                                        CompareSort compS) const {

#ifdef TIMING    
    static timer timo(timer::Wall);
    timo.start();
#endif

    const parameters& par=getParameters();
    const float hthresh = par.hystheresisThreshold;
    const int size=src.size();

    // need to know whether we're dealing with maxima or minima
    const bool maxSelected=comp(1,0);

    // sort before hystheresis
    pts.sort(compS);

    T actualExtr,actualThresh,v;
    int j;

    // records already visited places.
    // each starting point has its own marker
    ivector mask(src.size(),std::numeric_limits<int>::max());
    int marker=0;

    // neighborhood
    static const int deltax[] = {-1,1};

    int p, actualPos;

    // queue is faster than smallObjectList!
    std::queue<int> todo;
    lti::list<int>::iterator it=pts.begin();
    const lti::list<int>::iterator ite=pts.end();

    // if true erase current point
    bool eraseIt;
    
    while (it!=ite) {

      eraseIt=false;
      p = (*it);
      actualExtr = src.at(p);
      actualPos = p;

      if (maxSelected) {
        actualThresh = static_cast<T>(actualExtr-hthresh*(actualExtr-minn));
      } else {
        actualThresh = static_cast<T>(actualExtr+hthresh*(maxx-actualExtr));
      } 
      todo.push(p);
      mask.at(p) = marker; // flag: already evaluated

      while (!todo.empty()) { // do region search
        // get next point to be evaluated
        p = todo.front();
        todo.pop();
        v = src.at(p);

        // check for more extremal value than starting point
        // check for equal value but different point
        if ( (comp(v,actualExtr)) ||
             ((v==actualExtr) && (p!=actualPos))) {
          eraseIt=true;
          // clear todo
          while (!todo.empty()) {
            todo.pop();
          }
          break;
        } 
        
        for (j=0;j<2;j++) {
          const int tx = p+deltax[j];
          // out of bounds?
          if ((tx < 0) || (tx >= size)) continue;
          
          int& cmask=mask.at(tx);
          // check for encounter with already searched region
          // then search no more and go to next point
          if (cmask<marker) {
            eraseIt=true;
            // clear todo
            while (!todo.empty()) {
              todo.pop();
            }
            break;
          } else if (cmask==marker) { // already queued?
            continue;
          }
          
          // only look at points more extreme than actualThresh
          if (comp(src.at(tx),actualThresh)) {
            todo.push(tx);
            cmask = marker;
          }
        }
      }

      if (eraseIt) {
        it=pts.erase(it);
        //         std::cout << "pos: " << p
        //                   << "  mask: " << mask.at(p)
        //                   << "  value: " << v << "\n\n";
      } else {
        it++;
        //         std::cout << "kept\n\n";
      }
      marker++;

    }

#ifdef TIMING    
    timo.stop();
    std::cerr << "hystheresisStrict(): " << timo.getTime() << "\n";
#endif
    return true;

  }


  // On copy apply for type vector<T>!
  template <typename T>
  bool localExtremes::worker(const eExtremesType extremesType,
                             const vector<T>& src,
                             lti::list<int>& pts,
                             lti::list<int>& pts2) const {
    
#ifdef TIMING    
    static timer timo(timer::Wall);
#endif

    pts.clear();
    pts2.clear();
    lti::list<int>::const_iterator it,ite;
    
    const parameters& param=getParameters();
    const bool doThreshold=param.relativeThreshold>0 
      && param.relativeThreshold<1;
    const bool doHystheresis=param.hystheresisThreshold>0 
      && param.hystheresisThreshold<1;
    bool needMinMax=doThreshold || doHystheresis;
    collector<T> coll;
    dummyCollector<T> dcoll;
    
    switch (extremesType) {
    case Max: {
      sortComparatorMaxV<T> compS(src);
      greaterStruct<T> greater;
      
      if (needMinMax) {
        filter(src, pts, greater, coll);
      } else {
        filter(src, pts, greater, dcoll);
      }
      
      if (doThreshold) {
        threshold(src, pts, coll.minn, coll.maxx, greater); 
      }
      
      if (doHystheresis) {
        if (param.strictHystheresis) {
          hystheresisStrict(src,pts,coll.minn,coll.maxx,greater,compS);
        } else {
          hystheresis(src, pts, coll.minn, coll.maxx, greater, compS);
        }
      }
      
#ifdef TIMING    
      timo.start();
#endif
      pts.sort(compS);
#ifdef TIMING    
      timo.stop();
      std::cerr << "sorting: " << timo.getTime() << "\n";
#endif
    } break;
    case Min: {
      sortComparatorMinV<T> compS(src);
      lessStruct<T> less;
      
      if (needMinMax) {
        filter(src, pts, less, coll);
      } else {
        filter(src, pts, less, dcoll);
      }
      
      if (doThreshold) {
        threshold(src, pts, coll.minn, coll.maxx, less); 
      }
      
      if (doHystheresis) {
        if (param.strictHystheresis) {
          hystheresisStrict(src, pts, coll.minn, coll.maxx, less, compS);
        } else {
          hystheresis(src, pts, coll.minn, coll.maxx, less, compS);
        }
      }
      
#ifdef TIMING    
      timo.start();
#endif
      pts.sort(compS);
#ifdef TIMING    
      timo.stop();
      std::cerr << "sorting: " << timo.getTime() << "\n";
#endif
     
    } break;
    case MinMax: {
      sortComparatorMaxV<T> compSMax(src);
      sortComparatorMinV<T> compSMin(src);
      lessStruct<T> less;
      greaterStruct<T> greater;
      
      if (needMinMax) {
        filter(src, pts, less, coll);
        filter(src, pts2,greater,dcoll);
      } else {
        filter(src, pts, less, dcoll);
        filter(src, pts2,greater,dcoll);
      }
      
      std::cerr << "mins: " << pts.size() 
                << "  maxs: " << pts2.size() << "\n";
      
      if (doThreshold) {
        threshold(src, pts,  coll.minn, coll.maxx, less); 
        threshold(src, pts2, coll.minn, coll.maxx, greater);
      }
      
      std::cerr << "mins: " << pts.size() 
                << "  maxs: " << pts2.size() << "\n";
      
      if (doHystheresis) {
        if (param.strictHystheresis) {
          hystheresisStrict(src,pts, coll.minn,coll.maxx,less,compSMin);
          hystheresisStrict(src,pts2,coll.minn,coll.maxx,greater,compSMax);
        } else {
          hystheresis(src, pts,  coll.minn, coll.maxx, less, compSMin);
          hystheresis(src, pts2, coll.minn, coll.maxx, greater, compSMax);
        }
      }
      
      std::cerr << "mins: " << pts.size() 
                << "  maxs: " << pts2.size() << "\n";
      
      //sort points
#ifdef TIMING    
      timo.start();
#endif
      pts.sort(compSMin);
      pts2.sort(compSMax);
#ifdef TIMING    
      timo.stop();
      std::cerr << "sorting: " << timo.getTime() << "\n";
#endif
    } break;
    } // end of switch
    
    clipPoints(pts, param.maxNumber);
    clipPoints(pts2, param.maxNumber);
    
    return true;
  }

  template <typename T>
  bool localExtremes::apply(const vector<T>& src,
                            lti::list<int>& pts,
                            lti::list<int>& pts2) const {

    return worker(MinMax, src, pts, pts2);
  }

  // On copy apply for type vector<T>!
  template <typename T>
  bool localExtremes::apply(const vector<T>& src,
                            lti::list<int>& pts) const {
    lti::list<int> pts2;
    bool rc = worker(getParameters().extremesType,src, pts, pts2);
    // insert all points from pts2 before the pts.end().
    pts.splice(pts.end(),pts2);
    return rc;
  }

}
