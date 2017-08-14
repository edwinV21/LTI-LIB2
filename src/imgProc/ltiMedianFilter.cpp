/*
 * Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006
 * Lehrstuhl fuer Technische Informatik, RWTH-Aachen, Germany
 * Copyright (C) 2009
 * Pablo Alvarado
 *
 * This file is part of the LTI-Computer Vision Library (LTI-Lib)
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
 * \file   ltiMedianFilter.cpp
 *         Contains the class medianFilter
 * \author Guy Wafo Moudhe
 * \date   16.03.2001
 *
 * $Id: ltiMedianFilter.cpp,v 1.9 2010-04-25 05:27:35 alvarado Exp $
 */


#include "ltiMedianFilter.h"
#include "ltiMedianFilter_template.h"

namespace lti {
  // --------------------------------------------------
  // medianFilter::parameters
  // --------------------------------------------------

  medianFilter::parameters::parameters()
    : denoising::parameters() {
    kernelSize = int(3);
  }

  // copy constructor
  medianFilter::parameters::parameters(const parameters& other)
    : denoising::parameters()  {
    copy(other);
  }

  // destructor
  medianFilter::parameters::~parameters() {
  }

  // returns the name of this class
  const std::string& medianFilter::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // copy member

  medianFilter::parameters&
    medianFilter::parameters::copy(const parameters& other) {
    denoising::parameters::copy(other);

    kernelSize = other.kernelSize;

    return *this;
  }

  // alias for copy member
  medianFilter::parameters&
    medianFilter::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  // clone member
  medianFilter::parameters* medianFilter::parameters::clone() const {
    return new parameters(*this);
  }

  // newInstance member function
  medianFilter::parameters* medianFilter::parameters::newInstance() const {
    return new parameters();
  }

  /*
   * write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool medianFilter::parameters::write(ioHandler& handler,
                                       const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {

      lti::write(handler,"kernelSize",kernelSize);
    }

    // This is the standard C++ code, which MS Visual C++ 6 is not able to
    // compile...
    b = b && denoising::parameters::write(handler,false);

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
  bool medianFilter::parameters::read(ioHandler& handler,
                                      const bool complete) {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {

      lti::read(handler,"kernelSize",kernelSize);
    }

    // This is the standard C++ code, which MS Visual C++ 6 is not able to
    // compile...
    b = b && denoising::parameters::read(handler,false);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }


  // --------------------------------------------------
  // medianFilter
  // --------------------------------------------------

  // default constructor
  medianFilter::medianFilter()
    : denoising(){
    parameters defaultParameters;
    setParameters(defaultParameters);
  }

  // default constructor
  medianFilter::medianFilter(const parameters& par)
    : denoising(){
    setParameters(par);
  }

  medianFilter::medianFilter(const int kernelSize,
                             const eBoundaryType boundaryType)
    : denoising(){
    parameters par;
    par.kernelSize = kernelSize;
    par.boundaryType = boundaryType;
    setParameters(par);
  }

  // copy constructor
  medianFilter::medianFilter(const medianFilter& other)
    : denoising(),
      matrixProcessingInterface<ubyte>(),
      matrixProcessingInterface<float>() {
    copy(other);
  }

  // destructor
  medianFilter::~medianFilter() {
  }

  // returns the name of this class
  const std::string& medianFilter::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // copy member
  medianFilter& medianFilter::copy(const medianFilter& other) {
    denoising::copy(other);
    return (*this);
  }

  // clone member
  medianFilter* medianFilter::clone() const {
    return new medianFilter(*this);
  }

  // newInstance member function
  medianFilter* medianFilter::newInstance() const {
    return new medianFilter();
  }

  // return parameters
  const medianFilter::parameters&
    medianFilter::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }

  // shortcut for setting kernel size
  void medianFilter::setKernelSize(int newKernelSize) {
    parameters param(getParameters());
    param.kernelSize = newKernelSize;
    setParameters(param);
  }

  // -------------------------------------------------------------------
  // The apply-methods for matrices!
  // -------------------------------------------------------------------

  // On place apply for type fmatrix!
  bool medianFilter::apply(fmatrix& srcdest) const {
    fmatrix tmp;
    if (apply(srcdest,tmp)) {
      tmp.detach(srcdest);
      return true;
    }
    return false;
  }

  // On place apply for type matrix<ubyte>!
  bool medianFilter::apply(matrix<ubyte>& srcdest) const {
    matrix<ubyte> tmp;
    if (apply(srcdest,tmp)) {
      tmp.detach(srcdest); // faster than srcdest.copy(tmp);
      return true;
    }
    return false;
  }

  // filters src with the median filter and gives the result to dest
  bool medianFilter::apply(const fmatrix& src,fmatrix& dest) const {
    return realMedian(src,dest);
  }

  // On copy apply for type matrix<ubyte>!
  bool medianFilter::apply(const matrix<ubyte>& src,matrix<ubyte>& dest) const {
    return histogramMethod(src,dest);
  }


  // On place apply for type fmatrix!
  bool medianFilter::apply(channel& srcdest) const {
    return apply(static_cast<fmatrix&>(srcdest));
  }

  // On place apply for type matrix<ubyte>!
  bool medianFilter::apply(channel8& srcdest) const {
    return apply(static_cast<matrix<ubyte>&>(srcdest));
  }

  // filters src with the median filter and gives the result to dest
  bool medianFilter::apply(const channel& src,channel& dest) const {
    return apply(static_cast<const fmatrix&>(src),
                 static_cast<fmatrix&>(dest));
  }

  // On copy apply for type matrix<ubyte>!
  bool medianFilter::apply(const channel8& src,channel8& dest) const {
    return apply(static_cast<const matrix<ubyte>&>(src),
                 static_cast<matrix<ubyte>&>(dest));
  }

  // the kernel runs inside the image
  void medianFilter::histogramMethodMiddle(const matrix<ubyte>& src,
                                           matrix<ubyte>& dest,
					   ivector& histogram,
                                           int& lessThanMedian,
                                           int row,int& col,
                                           ubyte& median,
                                           int sizeOfKernel) const {
    int i,j;//index
    int tmp;
    ubyte value;
    const int limit = sizeOfKernel/2;   //half size of the kernel
    const int halfKernelSize = (sizeOfKernel*sizeOfKernel)/2;
    const int lastCol = src.lastColumn()-limit;
    const int r = row+limit;
    col = limit;
    while(col <= (lastCol-1)) {
      j = col-limit;
      // 3. Step
      for(i=row-limit;i<=r;++i) {
        value=src[i][j];       // for each pixel in the leftmost
        --histogram[value];    // column of intensity,take off in the histogram
        if(value < median)    // the number of the corresponding pixels
          --lessThanMedian;
      }
      // 4. Step:
      ++col;
      j = col+limit;               // for each pixel in the rightmost column
      for(i=row-limit;i<=r;++i) {  // of intensity,add in the histogram
        value=src[i][j];         // the number of the corresponding pixels
        ++histogram[value];
        if(value < median)
          ++lessThanMedian;
      }
      if (lessThanMedian > halfKernelSize) {
        // 6. Step
        do {
          --median;
          lessThanMedian -= histogram[median];
        } while(lessThanMedian > halfKernelSize);
        dest.at(row,col) = median;
      }
      else { //if(lessThanMedian<=halfKernelSize) {
        // 5. Step
        tmp = lessThanMedian + histogram[median];
        while (tmp <= halfKernelSize) {
          lessThanMedian = tmp;
          ++median;
          tmp+=histogram[median];
        }
        dest.at(row,col) = median;
      }
    }

  }

   // applies the histogramMethod for the type boundary Constant
  bool medianFilter::histogramMethodConstant(const matrix<ubyte>& src,
                                             matrix<ubyte>& dest,
                                             int sizeOfKernel) const {
    const int limit = sizeOfKernel/2;   //half size of the kernel
        // Median Filter
    int i,j,row,col,z;//index
    int tmp;
    int lessThanMedian = 0;
    ubyte median;
    quickMedian quickmedia;

    ivector histogram(256,int(0));  //the histogram for a matrix<ubyte>
    vector<ubyte> vect(sizeOfKernel*sizeOfKernel,ubyte(0));
    const int rowSize = src.rows();
    const int columnSize = src.columns();
    const int halfKernelSize = (sizeOfKernel*sizeOfKernel)/2;
    //resizes the matrix destination
    dest.allocate(rowSize, columnSize);

    const int lastCol = src.lastColumn()-limit;
    const int lastRow = src.lastRow()-limit;

    ubyte value;
    //runs through the src's columns,inside the image
    for(row=limit;row<=lastRow;++row) {
      const int r = row+limit;
      histogram.fill(int(0));

      col=0;
      z=0;
      const int c=col+limit;
      for(i=row-limit;i<=r;++i)
        for(j=col-limit;j<=c;++j) {
	  if(j<0)
            value=src[i][0];
	  else// if(j>=0)
            value=src[i][j];
	  vect.at(z)=value;
	  ++histogram[value];
	  ++z;
        }

      quickmedia.apply(vect, median);
      dest.at(row,col)=median;

      lessThanMedian=0;
      for(i=0;i<vect.size();++i)
        if(vect[i] < median)
          ++lessThanMedian;

      while(col < limit) {
        for(i=row-limit;i<=r;++i) {
          value = src[i][0];
          --histogram[value];
          if(value < median)
            --lessThanMedian;
	}
        ++col;
        j=col+limit;

        for(i=row-limit;i<=r;++i) {
          value=src[i][j];
          ++histogram[value];
          if(value < median) {
            ++lessThanMedian;
          }
        }
        if (lessThanMedian>halfKernelSize) {
          // 6. Step
          do {
            --median;
            lessThanMedian-=histogram[median];
          } while(lessThanMedian>halfKernelSize);
          dest.at(row,col)=median;
        }
        else { //if(lessThanMedian<=halfKernelSize) {
              // 5. Step
          tmp = lessThanMedian + histogram[median];
          while (tmp <= halfKernelSize) {
            lessThanMedian = tmp;
            ++median;
            tmp+=histogram[median];
          }
          dest.at(row,col)=median;
        }
      }
      // runs inside the image
      histogramMethodMiddle(src, dest,histogram,lessThanMedian,
                            row,col,median,sizeOfKernel);
      col=lastCol;
      while(col < (columnSize-1)) {
        j=col-limit;
        for(i=row-limit;i<=r;++i) {
          value=src[i][j];
          --histogram[value];
          if(value < median)
            --lessThanMedian;
        }
        ++col;

        for(i=row-limit;i<=r;++i) {
          value=src[i][columnSize-1];
          ++histogram[value];
          if(value < median)
            ++lessThanMedian;
        }
        if (lessThanMedian > halfKernelSize) {
          // 6. Step
          do {
            --median;
            lessThanMedian -= histogram[median];
          } while(lessThanMedian > halfKernelSize);
          dest.at(row,col) = median;
        }
        else { //if(lessThanMedian<=halfKernelSize) {
          // 5. Step
          tmp = lessThanMedian + histogram[median];
          while (tmp <= halfKernelSize) {
            lessThanMedian = tmp;
            ++median;
            tmp+=histogram[median];
          }
          dest.at(row,col) = median;
        }
      }
    }
    for(row=0;row<limit;++row) {     //runs the rows at the top
      const int r=row+limit;
      // runs the rows at the top in the middle
      for(col=limit;col<=lastCol;++col) {
        const int c=col+limit;
        z=0;
        for(i=row-limit;i<=r;++i)
	  for(j=col-limit;j<=c;++j) {
	    if(i<0)
              vect.at(z)=src[0][j];
	    else//if(i>=0) {
              vect.at(z)=src[i][j];
	    ++z;
          }
	quickmedia.apply(vect, dest.at(row,col));
      }
      for(col=0;col<limit;++col) {   // runs at the top,left in the corner
        const int c=col+limit;
        z=0;
        for(i=row-limit;i<=r;++i)
	  for(j=col-limit;j<=c;++j) {
            if(i<0 && j<0)
              vect.at(z)=src[0][0];
            else if(i>=0 && j<0)
              vect.at(z)=src[i][0];
            else if(i<0 && j>=0)
              vect.at(z)=src[0][j];
            else //if(i>=0 && j>=0)
              vect.at(z)=src[i][j];
	    ++z;
          }
	quickmedia.apply(vect, dest.at(row,col));
      }
      //runs at the top,right in the corner
      for(col=lastCol+1;col<columnSize;++col) {
        const int c=col+limit;
        z=0;
        for(i=row-limit;i<=r;++i)
          for(j=col-limit;j<=c;++j) {
            if(i<0 && j<=columnSize-1)
              vect.at(z)=src[0][j];
            else if(i<0 && j>columnSize-1)
              vect.at(z)=src[0][columnSize-1];
            else if(i>=0 && j>columnSize-1)
              vect.at(z)=src[i][columnSize-1];
            else //if(i>=0 && j<=columnSize-1)
              vect.at(z)=src[i][j];
	    ++z;
	  }
        quickmedia.apply(vect, dest.at(row,col));
      }
    }
    for(row=lastRow+1;row<=rowSize-1;++row) {  //runs the rows at the bottom
      const int r=row+limit;
      //runs the rows at the bottom in the middle
      for(col=limit;col<=lastCol;++col) {
        const int c=col+limit;
        z=0;
        for(i=row-limit;i<=r;++i)
          for(j=col-limit;j<=c;++j) {
            if(i<=rowSize-1)
              vect.at(z)=src[i][j];
            else // if(i>rowSize-1)
              vect.at(z)=src[rowSize-1][j];
	    ++z;
          }
        quickmedia.apply(vect, dest.at(row,col));
      }
      //runs at the bottom,left in the corner
      for(col=0;col<limit;++col) {
        const int c=col+limit;
        z=0;
        for(i=row-limit;i<=r;++i)
          for(j=col-limit;j<=c;++j) {
            if(i<=rowSize-1 && j<0)
              vect.at(z)=src[i][0];
            else if(i<=rowSize-1 && j>=0)
              vect.at(z)=src[i][j];
            else if(i>rowSize-1 && j<0)
              vect.at(z)=src[rowSize-1][0];
            else //if(i>rowSize-1 && j>=0)
              vect.at(z)=src[rowSize-1][j];
	    ++z;
          }
	quickmedia.apply(vect, dest.at(row,col));
      }
      //runs at the bottom,right in the corner
      for(col=lastCol+1;col<columnSize;++col){
        const int c=col+limit;
        z=0;
        for(i=row-limit;i<=r;++i)
          for(j=col-limit;j<=c;++j) {
            if(i<=rowSize-1 && j<=columnSize-1)
              vect.at(z)=src[i][j];
            else if(i<=rowSize-1 && j>columnSize-1)
              vect.at(z)=src[i][columnSize-1];
            else if(i>rowSize-1 && j<=columnSize-1)
              vect.at(z)=src[rowSize-1][j];
            else //if(i>rowSize-1 && j>columnSize-1)
              vect.at(z)=src[rowSize-1][columnSize-1];
	    ++z;
          }
	quickmedia.apply(vect, dest.at(row,col));
      }
    }
    return true;
  }

  // applies the histogramMethod for the type boundary Periodic
  bool medianFilter::histogramMethodPeriodic(const matrix<ubyte>& src,
                                             matrix<ubyte>& dest,
                                             int sizeOfKernel) const {
    const int limit = sizeOfKernel/2;   //half size of the kernel
    int i,j,row,col,z;//index
    int tmp;
    int lessThanMedian = 0;
    ubyte median;
    quickMedian quickmedia;

    vector<ubyte> vect(sizeOfKernel*sizeOfKernel,ubyte(0));
    ivector histogram(256,int(0));  //the histogram for a matrix<ubyte>

    const int rowSize = src.rows();
    const int columnSize = src.columns();
    const int halfKernelSize = (sizeOfKernel*sizeOfKernel)/2;
    //resizes the matrix destination
    dest.allocate(src.rows(),src.columns());

    const int lastCol = src.lastColumn()-limit;
    const int lastRow = src.lastRow()-limit;

    ubyte value;
    //runs through the src's columns,inside the image
    for(row=limit;row<=lastRow;++row) {
      const int r = row+limit;
      histogram.fill(int(0));

      z=0;
      col=0;
      const int c=col+limit;
      for(i=row-limit;i<=r;++i)
	for(j=col-limit;j<=c;++j) {
          if(j<0)
            value=src[i][j+columnSize];
          else // if(j>=0)
            value=src[i][j];
	  vect.at(z)=value;
	  ++histogram[value];
	  ++z;
        }
      quickmedia.apply(vect, median);
      dest.at(row,col)=median;

      lessThanMedian = 0;
      for(i=0;i<vect.size();++i)   // count the number of pixels with intensity
        if(vect[i] < median)       // can be optimized for Zero boundary type
          ++lessThanMedian;        // less than or equal to median

      while(col < limit) {
	j = col-limit;
        for(i=row-limit;i<=r;++i){
          value=src[i][j+columnSize];
          --histogram[value];
          if(value < median)
            --lessThanMedian;
        }
        ++col;
        j=col+limit;
        for(i=row-limit;i<=r;++i) {
          value=src[i][j];
          ++histogram[value];
          if(value < median)
            ++lessThanMedian;
        }
        if (lessThanMedian > halfKernelSize) {
          // 6. Step
          do {
            --median;
            lessThanMedian-=histogram[median];
          } while(lessThanMedian>halfKernelSize);
          dest.at(row,col)=median;
        }
        else { //if(lessThanMedian<=halfKernelSize) {
          // 5. Step
          tmp = lessThanMedian + histogram[median];
          while (tmp <= halfKernelSize) {
            lessThanMedian = tmp;
            ++median;
            tmp+=histogram[median];
          }
          dest.at(row,col) = median;
        }
      }
      //runs inside the image
      histogramMethodMiddle(src,dest,histogram,lessThanMedian,
			    row,col,median,sizeOfKernel);
      col=lastCol;
      while(col < (columnSize-1)) {
        j = col-limit;

        for(i=row-limit;i<=r;++i) {
          value=src[i][j];     // for each pixel in the leftmost
          --histogram[value];  // column of intensity,take off in the histogram
          if(value < median) // the number of the corresponding pixels
            --lessThanMedian;
	}
        ++col;
        j=col+limit;
        for(i=row-limit;i<=r;++i) {
          value = src[i][j-columnSize];
          ++histogram[value];
          if(value < median)
            ++lessThanMedian;
	}
        if (lessThanMedian>halfKernelSize) {
          // 6. Step
          do {
            --median;
            lessThanMedian-=histogram[median];
          } while(lessThanMedian>halfKernelSize);
          dest.at(row,col)=median;
        }
        else { //if(lessThanMedian<=halfKernelSize) {
              // 5. Step
          tmp = lessThanMedian + histogram[median];
          while (tmp <= halfKernelSize) {
            lessThanMedian = tmp;
            ++median;
            tmp+=histogram[median];
          }
          dest.at(row,col)=median;
        }
      }
    }
    for(row=0;row<limit;++row) {          //runs the rows at the top
      const int r=row+limit;
      // runs the rows at the top in the middle
      for(col=limit;col<=lastCol;++col) {
        const int c=col+limit;
        z=0;
        for(i=row-limit;i<=r;++i)
	  for(j=col-limit;j<=c;++j) {
	    if(i<0)
              vect.at(z)=src[i+rowSize][j];
            else //if(i>=0)
              vect.at(z)=src[i][j];
	    ++z;
          }
        quickmedia.apply(vect, dest.at(row,col));
      }
      for(col=0;col<limit;++col) {       // runs at the top,left in the corner
        const int c=col+limit;
        z=0;
        for(i=row-limit;i<=r;++i)
	  for(j=col-limit;j<=c;++j) {
            if(i<0 && j<0)
              vect.at(z)=src[rowSize+i][columnSize+j];
            else if(i>=0 && j<0)
              vect.at(z)=src[i][columnSize+j];
            else if(i<0 && j>=0)
              vect.at(z)=src[rowSize+i][j];
            else //if(i>=0 && j>=0)
              vect.at(z)=src[i][j];
	    ++z;
          }
        quickmedia.apply(vect, dest.at(row,col));
      }
      //runs at the top,right in the corner
      for(col=lastCol+1;col<columnSize;++col) {
        const int c=col+limit;
        z=0;
        for(i=row-limit;i<=r;++i)
          for(j=col-limit;j<=c;++j) {
            if(i<0 && j<=columnSize-1)
              vect.at(z)=src[rowSize+i][j];
            else if(i<0 && j>columnSize-1)
              vect.at(z)=src[rowSize+i][j-columnSize];
            else if(i>=0 && j>columnSize-1)
              vect.at(z)=src[i][j-columnSize];
            else //if(i>=0 && j<=columnSize-1)
              vect.at(z)=src[i][j];
	    ++z;
          }
	quickmedia.apply(vect, dest.at(row,col));
      }
    }
    for(row=lastRow+1;row<=rowSize-1;++row) {  //runs the rows at the bottom
      const int r=row+limit;
      //runs the rows at the bottom in the middle
      for(col=limit;col<=lastCol;++col) {
        const int c=col+limit;
        z=0;
        for(i=row-limit;i<=r;++i)
          for(j=col-limit;j<=c;++j) {
            if(i<=rowSize-1)
              vect.at(z)=src[i][j];
            else // if(i>rowSize-1)
              vect.at(z)=src[i-rowSize][j];
	    ++z;
	  }
        quickmedia.apply(vect, dest.at(row,col));
      }
      for(col=0;col<limit;++col) {    //runs at the bottom,left in the corner
        const int c=col+limit;
        z=0;
        for(i=row-limit;i<=r;++i)
          for(j=col-limit;j<=c;++j) {
            if(i<=rowSize-1 && j<0)
              vect.at(z)=src[i][columnSize+j];
            else if(i<=rowSize-1 && j>=0)
              vect.at(z)=src[i][j];
            else if(i>rowSize-1 && j<0)
              vect.at(z)=src[i-rowSize][columnSize+j];
            else //if(i>rowSize-1 && j>=0)
              vect.at(z)=src[i-rowSize][j];
	    ++z;
          }
        quickmedia.apply(vect, dest.at(row,col));
      }
      //runs at the bottom,right in the corner
      for(col=lastCol+1;col<columnSize;++col) {
        const int c=col+limit;
        z=0;
        for(i=row-limit;i<=r;++i)
          for(j=col-limit;j<=c;++j) {
            if(i<=rowSize-1 && j<=columnSize-1)
              vect.at(z)=src[i][j];
            else if(i<=rowSize-1 && j>columnSize-1)
              vect.at(z)=src[i][j-columnSize];
            else if(i>rowSize-1 && j<=columnSize-1)
              vect.at(z)=src[i-rowSize][j];
            else //if(i>rowSize-1 && j>columnSize-1)
              vect.at(z)=src[i-rowSize][j-columnSize];
	    ++z;
          }
        quickmedia.apply(vect, dest.at(row,col));
      }
    }
    return true;
  }

  // applies the histogramMethod for the type boundary Mirror
  bool medianFilter::histogramMethodMirror(const matrix<ubyte>& src,
                                           matrix<ubyte>& dest,
                                           int sizeOfKernel) const {
    const int limit = sizeOfKernel/2;   //half size of the kernel

    // Median Filter
    int i,j,row,col,z;//index
    int tmp;
    int lessThanMedian = 0;
    ubyte median;
    quickMedian quickmedia;

    vector<ubyte> vect(sizeOfKernel*sizeOfKernel,ubyte(0));
    ivector histogram(256,int(0));  //the histogram for a matrix<ubyte>

    const int rowSize = src.rows();
    const int columnSize = src.columns();
    const int halfKernelSize = (sizeOfKernel*sizeOfKernel)/2;
    //resizes the matrix destination
    dest.allocate(src.rows(),src.columns());

    const int lastCol = src.lastColumn()-limit;
    const int lastRow = src.lastRow()-limit;
    ubyte value;

    //runs through the src's columns,inside the image
    for(row=limit;row<=lastRow;++row) {
      const int r = row+limit;
      histogram.fill(int(0));

      z=0;
      col=0;
      const int c=col+limit;
      for(i=row-limit;i<=r;++i)
	for(j=col-limit;j<=c;++j) {
          if(j<0)
            value=src[i][-j-1];
          else // if(j>=0)
            value=src[i][j];
	  vect.at(z)=value;
	  ++histogram[value];
	  ++z;
	}
      quickmedia.apply(vect, median);
      dest.at(row,col)=median;

      lessThanMedian = 0;
      for(i=0;i<vect.size();++i)  // count the number of pixels with intensity
        if(vect[i] < median)     // less than median
          ++lessThanMedian;

      while(col < limit) {
        j = col-limit;
        for(i=row-limit;i<=r;++i) {
          value=src[i][-j-1];
          --histogram[value];
          if(value < median)
            --lessThanMedian;
        }
        ++col;
        j=col+limit;
        for(i=row-limit;i<=r;++i) {
          value=src[i][j];
          ++histogram[value];
          if(value < median)
            ++lessThanMedian;
        }
        if (lessThanMedian > halfKernelSize) {
          // 6. Step
          do {
            --median;
            lessThanMedian-=histogram[median];
          } while(lessThanMedian>halfKernelSize);
          dest.at(row,col)=median;
        }
        else { //if(lessThanMedian<=halfKernelSize) {
          // 5. Step
          tmp = lessThanMedian + histogram[median];
          while (tmp <= halfKernelSize) {
            lessThanMedian = tmp;
            ++median;
            tmp+=histogram[median];
          }
          dest.at(row,col)=median;
        }
      }
      // runs inside the image
      histogramMethodMiddle(src,dest,histogram,lessThanMedian,
                            row,col,median,sizeOfKernel);
      col=lastCol;
      while(col < (columnSize-1)) {
        j = col-limit;

        for(i=row-limit;i<=r;++i) {
          value=src[i][j];     // for each pixel in the leftmost
          --histogram[value];  // column of intensity,take off in the histogram
	  if(value < median)   // the number of the corresponding pixels
            --lessThanMedian;
        }
        ++col;
        j=col+limit;
        for(i=row-limit;i<=r;++i) {
          value = src[i][columnSize-1+(columnSize-j)];
          ++histogram[value];
          if(value < median)
            ++lessThanMedian;
        }
        if (lessThanMedian>halfKernelSize) {
          // 6. Step
          do {
            --median;
            lessThanMedian-=histogram[median];
          } while(lessThanMedian>halfKernelSize);
          dest.at(row,col)=median;
        }
        else { //if(lessThanMedian<=halfKernelSize) {
          // 5. Step
          tmp = lessThanMedian + histogram[median];
          while (tmp <= halfKernelSize) {
            lessThanMedian = tmp;
            ++median;
            tmp+=histogram[median];
          }
          dest.at(row,col)=median;
        }
      }

    }
    for(row=0;row<limit;++row) {      //runs the rows at the top
      const int r=row+limit;          // runs the rows at the top in the middle
      for(col=limit;col<=lastCol;++col) {
        const int c=col+limit;
        z=0;
        for(i=row-limit;i<=r;++i)
	  for(j=col-limit;j<=c;++j) {
	    if(i<0)
              vect.at(z)=src[-i-1][j];
            else //if(i>=0)
              vect.at(z)=src[i][j];
	    ++z;
	  }
	quickmedia.apply(vect, dest.at(row,col));
      }

      for(col=0;col<limit;++col) {      // runs at the top,left in the corner
        const int c=col+limit;
        z=0;
        for(i=row-limit;i<=r;++i)
	  for(j=col-limit;j<=c;++j) {
            if(i<0 && j<0)
              vect.at(z)=src[-i-1][-j-1];
            else if(i>=0 && j<0)
              vect.at(z)=src[i][-j-1];
            else if(i<0 && j>=0)
              vect.at(z)=src[-i-1][j];
            else //if(i>=0 && j>=0)
              vect.at(z)=src[i][j];
	    ++z;
          }
        quickmedia.apply(vect, dest.at(row,col));
      }
      //runs at the top,right in the corner
      for(col=lastCol+1;col<columnSize;++col) {
        const int c=col+limit;
        z=0;
        for(i=row-limit;i<=r;++i)
          for(j=col-limit;j<=c;++j) {
            if(i<0 && j<=columnSize-1)
              vect.at(z)=src[-i-1][j];
            else if(i<0 && j>columnSize-1)
              vect.at(z)=src[-i-1][columnSize-1+(columnSize-j)];
            else if(i>=0 && j>columnSize-1)
              vect.at(z)=src[i][columnSize-1+(columnSize-j)];
            else //if(i>=0 && j<=columnSize-1)
              vect.at(z)=src[i][j];
	    ++z;
          }
        quickmedia.apply(vect, dest.at(row,col));
      }
    }
    for(row=lastRow+1;row<=rowSize-1;++row) {  //runs the rows at the bottom
      const int r=row+limit;
      //runs the rows at the bottom in the middle
      for(col=limit;col<=lastCol;++col) {
        const int c=col+limit;
        z=0;
        for(i=row-limit;i<=r;++i)
          for(j=col-limit;j<=c;++j) {
            if(i<=rowSize-1)
              vect.at(z)=src[i][j];
            else // if(i>rowSize-1)
              vect.at(z)=src[rowSize-1+(rowSize-i)][j];
	    ++z;
	  }
        quickmedia.apply(vect, dest.at(row,col));
      }
      for(col=0;col<limit;++col) {    //runs at the bottom,left in the corner
        const int c=col+limit;
        z=0;
        for(i=row-limit;i<=r;++i)
          for(j=col-limit;j<=c;++j) {
            if(i<=rowSize-1 && j<0)
              vect.at(z)=src[i][-j-1];
            else if(i<=rowSize-1 && j>=0)
              vect.at(z)=src[i][j];
            else if(i>rowSize-1 && j<0)
              vect.at(z)=src[rowSize-1+(rowSize-i)][-j-1];
            else //if(i>rowSize-1 && j>=0)
              vect.at(z)=src[rowSize-1+(rowSize-i)][j];
	    ++z;
	  }
        quickmedia.apply(vect, dest.at(row,col));
      }
      //runs at the bottom,right in the corner
      for(col=lastCol+1;col<columnSize;++col) {
        const int c=col+limit;
        z=0;
        for(i=row-limit;i<=r;++i)
          for(j=col-limit;j<=c;++j) {
            if(i<=rowSize-1 && j<=columnSize-1)
              vect.at(z)=src[i][j];
            else if(i<=rowSize-1 && j>columnSize-1)
              vect.at(z)=src[i][columnSize-1+(columnSize-j)];
            else if(i>rowSize-1 && j<=columnSize-1)
              vect.at(z)=src[rowSize-1+(rowSize-i)][j];
            else //if(i>rowSize-1 && j>columnSize-1)
              vect.at(z)
                =src[rowSize-1+(rowSize-i)][columnSize-1+(columnSize-j)];
	    ++z;
	  }
        quickmedia.apply(vect, dest.at(row,col));
      }
    }
    return true;
  }

  // applies the histogramMethod for the type boundary Zero
  bool medianFilter::histogramMethodZero(const matrix<ubyte>& src,
                                          matrix<ubyte>& dest,
                                          int sizeOfKernel) const {
    const int limit = sizeOfKernel/2;   //half size of the kernel
        // Median Filter
    int i,j,row,col,z;//index
    int tmp;
    int lessThanMedian = 0;
    ubyte median;
    quickMedian quickmedia;

    vector<ubyte> vect(sizeOfKernel*sizeOfKernel,ubyte(0));
    ivector histogram(256,int(0));  //the histogram for a matrix<ubyte>

    const int rowSize = src.rows();
    const int columnSize = src.columns();
    const int halfKernelSize = (sizeOfKernel*sizeOfKernel)/2;
    //resizes the matrix destination
    dest.allocate(src.rows(),src.columns());

    const int lastCol = src.lastColumn()-limit;
    const int lastRow = src.lastRow()-limit;
    ubyte value;

    //runs through the src's columns,inside the image
    for(row=limit;row<=lastRow;++row) {
      const int r = row+limit;
      histogram.fill(int(0));
      col=0;

      z = sizeOfKernel*(sizeOfKernel-limit-1);
      vect.fill(ubyte(0),0,z-1);
      histogram[0] = z;  //the number of 0's are known

      for(i=row-limit;i<=r;++i)  // fills the vector to calculate the median
                                 // and fills the histogram
        for(j=0;j<=limit;++j) {
          value=src[i][j];
          vect.at(z)=value;
          ++z;
          ++histogram[value];
        }
      quickmedia.apply(vect, median);    //here is the median
      dest.at(row,0) = median;

      lessThanMedian = 0;
      for(i=0;i<vect.size();++i)  // count the number of pixels with intensity
        if(vect[i] < median)      // less than or equal to median
          ++lessThanMedian;       // this can be optimized for Zero boundary type

      //  col=0;
      // the kernel at the position between the border and the image
      while(col < limit) {
        histogram[0] -= sizeOfKernel;   // cut all the 0 in the leftmost column
                                // because 0 is the smallest value
        if(median != 0) {
          lessThanMedian -= sizeOfKernel;
        }
        ++col;
        j = col+limit;              // for each pixel in the rightmost column
        for(i=row-limit;i<=r;++i) { // of intensity,add the value
                                    //in the histogram
          value=src[i][j];       // and increment the number of pixels less
          ++histogram[value];    // than the median
          if(value < median)
            ++lessThanMedian;
        }
        if (lessThanMedian > halfKernelSize) {
          // 6. Step
          do {
            --median;
            lessThanMedian-=histogram[median];
          } while(lessThanMedian>halfKernelSize);
          dest.at(row,col)=median;
        }
        else { //if(lessThanMedian<=halfKernelSize) {
          // 5. Step
          tmp = lessThanMedian + histogram[median];
          while (tmp <= halfKernelSize) {
            lessThanMedian = tmp;
            ++median;
            tmp+=histogram[median];
          }
          dest.at(row,col)=median;
        }
      }
      //runs inside the image
      histogramMethodMiddle(src,dest,histogram,lessThanMedian,
                            row,col,median,sizeOfKernel);
      col = lastCol;
      while(col < (columnSize-1)) {
        j = col-limit;
        for(i=row-limit;i<=r;++i) {
          value=src[i][j];     // for each pixel in the leftmost
          --histogram[value];  // column of intensity,take off in the histogram
          if(value < median) { // the number of the corresponding pixels
            --lessThanMedian;
          }
        }
        ++col;
        histogram[0] += sizeOfKernel;

        if(median != 0) {
          lessThanMedian += sizeOfKernel;
        }
        if (lessThanMedian > halfKernelSize) {
          // 6. Step
          do {
            --median;
            lessThanMedian -= histogram[median];
          } while(lessThanMedian > halfKernelSize);
          dest.at(row,col) = median;
        }
        else { //if(lessThanMedian<=halfKernelSize) {
          // 5. Step
          tmp = lessThanMedian + histogram[median];
          while (tmp <= halfKernelSize) {
            lessThanMedian = tmp;
            ++median;
            tmp+=histogram[median];
          }
          dest.at(row,col) = median;
        }
      }
    }
    for(row=0;row<limit;++row) {  //runs the rows at the top
      z=sizeOfKernel*(sizeOfKernel-limit-1);
      vect.fill(ubyte(0),0,z-1);
      const int r=row+limit;
      // runs the rows at the top in the middle
      for(col=limit;col<=lastCol;++col) {
        const int c=col+limit;
        z=sizeOfKernel*(sizeOfKernel-(limit+1)-row);

        for(i=0;i<=r;++i)
          for(j=col-limit;j<=c;++j) {
	    vect.at(z)=src[i][j];
            ++z;
          }
        quickmedia.apply(vect, dest.at(row,col));
      }
      for(col=0;col<limit;++col) {    // runs at the top,left in the corner
        const int c=col+limit;
        z=sizeOfKernel*sizeOfKernel-(limit+1)*(limit+1+row+col)-col*row;
        vect.fill(ubyte(0),0,z-1);
        for(i=0;i<=r;++i)
	  for(j=0;j<=c;++j) {
	    vect.at(z)=src[i][j];
            ++z;
	  }
        quickmedia.apply(vect, dest.at(row,col));
      }
      //runs at the top,right in the corner
      for(col=lastCol+1;col<=columnSize-1;++col) {
        z=sizeOfKernel*sizeOfKernel-(limit+1)*(limit+1+row+(columnSize-1-col))-
          row*(columnSize-1-col);
        vect.fill(ubyte(0),0,z-1);
        for(i=0;i<=r;++i) {
          for(j=col-limit;j<=columnSize-1;++j) {
            vect.at(z)=src[i][j];
            ++z;
          }
        }
        quickmedia.apply(vect, dest.at(row,col));
      }
    }
    for(row=lastRow+1;row<=rowSize-1;++row) {  //runs the rows at the bottom
      z=sizeOfKernel*(sizeOfKernel-limit-1);
      vect.fill(ubyte(0),0,z-1);
      //runs the rows at the bottom in the middle
      for(col=limit;col<=lastCol;++col) {
        const int c=col+limit;
        z=sizeOfKernel*(sizeOfKernel-(limit+1)-(rowSize-1-row));
        vect.fill(ubyte(0),0,z-1);
        for(i=row-limit;i<=rowSize-1;++i)
          for(j=col-limit;j<=c;++j) {
            vect.at(z)=src[i][j];
            ++z;
          }
        quickmedia.apply(vect, dest.at(row,col));
      }
      for(col=0;col<limit;++col) {   //runs at the bottom,left in the corner
        const int c=col+limit;
        z=sizeOfKernel*sizeOfKernel-(limit+1)*
          (limit+1+(rowSize-1-row)+col)-col*(rowSize-1-row);
        vect.fill(ubyte(0),0,z-1);
        for(i=row-limit;i<=rowSize-1;++i)
          for(j=0;j<=c;++j) {
            vect.at(z)=src[i][j];
            ++z;
          }
        quickmedia.apply(vect, dest.at(row,col));
      }
      //runs at the bottom,right in the corner
      for(col=lastCol+1;col<=columnSize-1;++col) {
        z=sizeOfKernel*sizeOfKernel-(limit+1)*(limit+1+(rowSize-1-row)+
           (columnSize-1-col))-
          (rowSize-1-row)*(columnSize-1-col);
        vect.fill(ubyte(0),0,z-1);
        for(i=row-limit;i<=rowSize-1;++i)
          for(j=col-limit;j<=columnSize-1;++j) {
            vect.at(z)=src[i][j];
            ++z;
          }
        quickmedia.apply(vect, dest.at(row,col));
      }
    }
    return true;
  }

  // applies the histogramMethod for the type boundary NoBoundary
  bool medianFilter::histogramMethodNoBoundary(const matrix<ubyte>& src,
					       matrix<ubyte>& dest,
					       int sizeOfKernel) const {
    const int limit = sizeOfKernel/2;   //half size of the kernel
    int i,j,row,col,z;
    int lessThanMedian = 0;
    ubyte median;
    quickMedian quickmedia;

    vector<ubyte> vect(sizeOfKernel*sizeOfKernel,ubyte(0));
    ivector histogram(256,int(0));   // the histogram for a matrix<ubyte>

    const int lastRow = src.lastRow()-limit;

    //resizes the destination matrix
    dest.allocate(src.rows(),src.columns());
    ubyte value;
    //runs through the src's columns,inside the image
    for(row=limit;row<=lastRow;++row) {
      const int r = row+limit;
      histogram.fill(int(0));

      z=0;
      for(i=row-limit;i<=r;++i)
	for(j=0;j<sizeOfKernel;++j) {    // determinate the median
                                         // at the beginning
          value = src[i][j];
          vect.at(z) = value; // of each rows and fill the histogram
          ++z;
          ++histogram[value];
        }
      quickmedia.apply(vect, median);
      dest.at(row,limit) = median;

      lessThanMedian=0;
      for(i=0;i<vect.size();++i) // count the number of pixels with intensity
        if(vect[i] < median)     // less than or equal to median
          ++lessThanMedian;

      // runs inside the image
      histogramMethodMiddle(src,dest,histogram,lessThanMedian,
			    row,col,median,sizeOfKernel);
    }
    return true;
 }

  // apply especially with a histogram,only for type matrix<ubyte>
  bool medianFilter::histogramMethod(const matrix<ubyte>& src,
                                     matrix<ubyte>& dest) const {
      
    if (src.empty()) {
      dest.clear();
      return true;
    }

    const parameters& param = getParameters();

    if (param.kernelSize <= 1) {
      dest.copy(src);
      return true;
    }

    const int sizeOfKernel = param.kernelSize + ((param.kernelSize%2 == 0) ? 1:0);
    bool control = false;   // return variable

    // 2. Step
    switch(param.boundaryType) {
      case lti::Zero:
        control = histogramMethodZero(src,dest,sizeOfKernel);
        break;
      case lti::Mirror:
        control = histogramMethodMirror(src,dest,sizeOfKernel);
        break;
      case lti::Constant:
        control = histogramMethodConstant(src,dest,sizeOfKernel);
        break;
      case lti::Periodic:
        control = histogramMethodPeriodic(src,dest,sizeOfKernel);
        break;
      case lti::NoBoundary:
        control = histogramMethodNoBoundary(src,dest,sizeOfKernel);
        break;
      default:
        setStatusString("Unknown boundary type");
        break;
    }
    return control;
  }



  // -------------------------------------------------------------------
  // The apply-methods for vectors!
  // -------------------------------------------------------------------

  // On place apply for type fvector!
  bool medianFilter::apply(fvector& srcdest) const {
    fvector tmp;
    if (apply(srcdest,tmp)) {
      if (srcdest.ownsData()) {
        tmp.detach(srcdest);
      } else {
        srcdest.fill(tmp);
      }
      return true;
    }
    return false;
  }

  // On place apply for type vector<ubyte>!
  bool medianFilter::apply(vector<ubyte>& srcdest) const {
    vector<ubyte> tmp;
    if (apply(srcdest,tmp)) {
      if (srcdest.ownsData()) {
        tmp.detach(srcdest); // faster than srcdest.copy(tmp);
      } else {
        srcdest.fill(tmp);
      }

      return true;
    }
    return false;
  }

  // filters src with the median filter and gives the result to dest
  bool medianFilter::apply(const fvector& src,fvector& dest) const {
    return realMedian(src,dest);
  }

  // On copy apply for type vector<ubyte>!
  bool medianFilter::apply(const vector<ubyte>& src,vector<ubyte>& dest) const {
    return histogramMethod(src,dest);
  }

  // the kernel runs inside the image
  void medianFilter::histogramMethodMiddle(const vector<ubyte>& src,
                                           vector<ubyte>& dest,
					   ivector& histogram,
                                           int& lessThanMedian,
                                           int& col,
                                           ubyte& median,
                                           const int sizeOfKernel) const {
    int j;//index
    int tmp;
    ubyte value;
    const int limit = sizeOfKernel/2;   //half size of the kernel
    const int halfKernelSize = limit;
    const int lastCol = src.lastIdx()-limit;
    col = limit;
    while (col < lastCol) {
      j = col-limit;
      // 3. Step
      value=src.at(j);       // for each pixel in the leftmost
      --histogram[value];    // column of intensity,take off in the histogram
      if (value < median) {  // the number of the corresponding pixels
        --lessThanMedian;
      }
    
      // 4. Step:
      ++col;
      j = col+limit;               // for each pixel in the rightmost column
      value=src.at(j);         // the number of the corresponding pixels
      ++histogram[value];
      if(value < median) {
        ++lessThanMedian;
      }
      
      if (lessThanMedian > halfKernelSize) {
        // 6. Step
        do {
          --median;
          lessThanMedian -= histogram[median];
        } while(lessThanMedian > halfKernelSize);
        dest.at(col) = median;
      }
      else { // if (lessThanMedian<=halfKernelSize) {
        // 5. Step
        tmp = lessThanMedian + histogram[median];
        while (tmp <= halfKernelSize) {
          lessThanMedian = tmp;
          ++median;
          tmp+=histogram[median];
        }
        dest.at(col) = median;
      }
    }

  }

   // applies the histogramMethod for the type boundary Constant
  bool medianFilter::histogramMethodConstant(const vector<ubyte>& src,
                                             vector<ubyte>& dest,
                                             int sizeOfKernel) const {
    const int limit = sizeOfKernel/2;   //half size of the kernel
    // Median Filter
    int j,col,z;//index
    int tmp;
    int lessThanMedian = 0;
    ubyte median;
    quickMedian quickmedia;

    ivector histogram(256,int(0));  //the histogram for a vector<ubyte>
    vector<ubyte> vect(sizeOfKernel,ubyte(0));
    const int columnSize = src.size();
    const int halfKernelSize = sizeOfKernel/2;
    //resizes the vector destination
    dest.allocate(columnSize);

    const int lastCol = src.lastIdx()-limit;
    if (limit>lastCol) {
      // ups! kernel is bigger than the whole vector
      setStatusString("Kernels larger than data are not supported yet");
      return false;
    }

    ubyte value;
    //runs through the src's columns,inside the image
    histogram.fill(int(0));

    // very first pixel
    col=0;
    z=0;
    const int c=col+limit;
    for(j=col-limit;j<=c;++j,++z) {
      if(j<0) {
        value=src.at(0);
      } else { // if(j>=0)
        value=src.at(j);
      }
      vect.at(z)=value;
      ++histogram[value];
    }

    quickmedia.apply(vect, median);
    dest.at(col)=median;

    lessThanMedian=0;
    for(j=0;j<vect.size();++j) {
      if(vect[j] < median) {
        ++lessThanMedian;
      }
    }

    // rest of the left border
    while(col < limit) {
      value = src.at(0);
      --histogram[value];
      if(value < median) {
        --lessThanMedian;
      }
    
      ++col;
      j=col+limit;

      value=src.at(j);
      ++histogram[value];
      if(value < median) {
        ++lessThanMedian;
      }

      if (lessThanMedian>halfKernelSize) {
        // 6. Step
        do {
          --median;
          lessThanMedian-=histogram[median];
        } while(lessThanMedian>halfKernelSize);
        dest.at(col)=median;
      } else { //if(lessThanMedian<=halfKernelSize) {
        // 5. Step
        tmp = lessThanMedian + histogram[median];
        while (tmp <= halfKernelSize) {
          lessThanMedian = tmp;
          ++median;
          tmp+=histogram[median];
        }
        dest.at(col)=median;
      }
    }
    // runs inside the vector
    histogramMethodMiddle(src,
                          dest,
                          histogram,
                          lessThanMedian,
                          col,
                          median,
                          sizeOfKernel);
    // the right border
    col=lastCol;
    while(col < (columnSize-1)) {
      j=col-limit;
      value=src.at(j);
      --histogram[value];
      if(value < median) {
        --lessThanMedian;
      }
    
      ++col;
      
      value=src.at(columnSize-1);
      ++histogram[value];
      if(value < median) {
        ++lessThanMedian;
      }

      if (lessThanMedian > halfKernelSize) {
        // 6. Step
        do {
          --median;
          lessThanMedian -= histogram[median];
        } while(lessThanMedian > halfKernelSize);
        dest.at(col) = median;
      }
      else { //if(lessThanMedian<=halfKernelSize) {
        // 5. Step
        tmp = lessThanMedian + histogram[median];
        while (tmp <= halfKernelSize) {
          lessThanMedian = tmp;
          ++median;
          tmp+=histogram[median];
        }
        dest.at(col) = median;
      }
    }

    return true;
  }

  // applies the histogramMethod for the type boundary Periodic
  bool medianFilter::histogramMethodPeriodic(const vector<ubyte>& src,
                                             vector<ubyte>& dest,
                                             int sizeOfKernel) const {
    const int limit = sizeOfKernel/2;   //half size of the kernel
    // Median Filter
    int j,col,z;//index
    int tmp;
    int lessThanMedian = 0;
    ubyte median;
    quickMedian quickmedia;

    ivector histogram(256,int(0));  //the histogram for a vector<ubyte>
    vector<ubyte> vect(sizeOfKernel,ubyte(0));
    const int columnSize = src.size();
    const int halfKernelSize = sizeOfKernel/2;
    //resizes the vector destination
    dest.allocate(columnSize);

    const int lastCol = src.lastIdx()-limit;
    if (limit>lastCol) {
      // ups! kernel is bigger than the whole vector
      setStatusString("Kernels larger than data are not supported yet");
      return false;
    }

    ubyte value;
    //runs through the src's columns,inside the image
    histogram.fill(int(0));

    // very first pixel
    col=0;
    z=0;
    const int c=col+limit;
    for(j=col-limit;j<=c;++j,++z) {
      if(j<0) {
        value=src.at(src.size()+j);
      } else { // if(j>=0)
        value=src.at(j);
      }
      vect.at(z)=value;
      ++histogram[value];
    }

    quickmedia.apply(vect, median);
    dest.at(col)=median;

    lessThanMedian=0;
    for(j=0;j<vect.size();++j) {
      if(vect[j] < median) {
        ++lessThanMedian;
      }
    }

    // rest of the left border
    while(col < limit) {
      value = src.at((col-limit)+src.size());
      --histogram[value];
      if(value < median) {
        --lessThanMedian;
      }
    
      ++col;
      j=col+limit;

      value=src.at(j);
      ++histogram[value];
      if(value < median) {
        ++lessThanMedian;
      }

      if (lessThanMedian>halfKernelSize) {
        // 6. Step
        do {
          --median;
          lessThanMedian-=histogram[median];
        } while(lessThanMedian>halfKernelSize);
        dest.at(col)=median;
      } else { //if(lessThanMedian<=halfKernelSize) {
        // 5. Step
        tmp = lessThanMedian + histogram[median];
        while (tmp <= halfKernelSize) {
          lessThanMedian = tmp;
          ++median;
          tmp+=histogram[median];
        }
        dest.at(col)=median;
      }
    }
    // runs inside the vector
    histogramMethodMiddle(src,
                          dest,
                          histogram,
                          lessThanMedian,
                          col,
                          median,
                          sizeOfKernel);
    // the right border
    col=lastCol;
    while(col < (columnSize-1)) {
      j=col-limit;
      value=src.at(j);
      --histogram[value];
      if(value < median) {
        --lessThanMedian;
      }
    
      ++col;
      
      value=src.at((col+limit)-src.size());
      ++histogram[value];
      if(value < median) {
        ++lessThanMedian;
      }

      if (lessThanMedian > halfKernelSize) {
        // 6. Step
        do {
          --median;
          lessThanMedian -= histogram[median];
        } while(lessThanMedian > halfKernelSize);
        dest.at(col) = median;
      }
      else { //if(lessThanMedian<=halfKernelSize) {
        // 5. Step
        tmp = lessThanMedian + histogram[median];
        while (tmp <= halfKernelSize) {
          lessThanMedian = tmp;
          ++median;
          tmp+=histogram[median];
        }
        dest.at(col) = median;
      }
    }

    return true;
  }

  // applies the histogramMethod for the type boundary Mirror
  bool medianFilter::histogramMethodMirror(const vector<ubyte>& src,
                                           vector<ubyte>& dest,
                                           int sizeOfKernel) const {
    const int limit = sizeOfKernel/2;   //half size of the kernel

    // Median Filter
    int j,col,z;//index
    int tmp;
    int lessThanMedian = 0;
    ubyte median;
    quickMedian quickmedia;

    ivector histogram(256,int(0));  //the histogram for a vector<ubyte>
    vector<ubyte> vect(sizeOfKernel,ubyte(0));
    const int columnSize = src.size();
    const int halfKernelSize = sizeOfKernel/2;
    //resizes the vector destination
    dest.allocate(columnSize);

    const int lastCol = src.lastIdx()-limit;
    if (limit>lastCol) {
      // ups! kernel is bigger than the whole vector
      setStatusString("Kernels larger than data are not supported yet");
      return false;
    }

    ubyte value;
    //runs through the src's columns,inside the image
    histogram.fill(int(0));

    // very first pixel
    col=0;
    z=0;
    const int c=col+limit;
    for(j=col-limit;j<=c;++j,++z) {
      if(j<0) {
        value=src.at(-1-j);
      } else { // if(j>=0)
        value=src.at(j);
      }
      vect.at(z)=value;
      ++histogram[value];
    }

    quickmedia.apply(vect, median);
    dest.at(col)=median;

    lessThanMedian=0;
    for(j=0;j<vect.size();++j) {
      if(vect[j] < median) {
        ++lessThanMedian;
      }
    }

    // rest of the left border
    while(col < limit) {
      value = src.at(-1-(col-limit));
      --histogram[value];
      if(value < median) {
        --lessThanMedian;
      }
    
      ++col;
      j=col+limit;

      value=src.at(j);
      ++histogram[value];
      if(value < median) {
        ++lessThanMedian;
      }

      if (lessThanMedian>halfKernelSize) {
        // 6. Step
        do {
          --median;
          lessThanMedian-=histogram[median];
        } while(lessThanMedian>halfKernelSize);
        dest.at(col)=median;
      } else { //if(lessThanMedian<=halfKernelSize) {
        // 5. Step
        tmp = lessThanMedian + histogram[median];
        while (tmp <= halfKernelSize) {
          lessThanMedian = tmp;
          ++median;
          tmp+=histogram[median];
        }
        dest.at(col)=median;
      }
    }
    // runs inside the vector
    histogramMethodMiddle(src,
                          dest,
                          histogram,
                          lessThanMedian,
                          col,
                          median,
                          sizeOfKernel);
    // the right border
    col=lastCol;
    while(col < (columnSize-1)) {
      j=col-limit;
      value=src.at(j);
      --histogram[value];
      if(value < median) {
        --lessThanMedian;
      }
    
      ++col;
      
      value=src.at(2*src.size()-1-col-limit);
      ++histogram[value];
      if(value < median) {
        ++lessThanMedian;
      }

      if (lessThanMedian > halfKernelSize) {
        // 6. Step
        do {
          --median;
          lessThanMedian -= histogram[median];
        } while(lessThanMedian > halfKernelSize);
        dest.at(col) = median;
      }
      else { //if(lessThanMedian<=halfKernelSize) {
        // 5. Step
        tmp = lessThanMedian + histogram[median];
        while (tmp <= halfKernelSize) {
          lessThanMedian = tmp;
          ++median;
          tmp+=histogram[median];
        }
        dest.at(col) = median;
      }
    }

    return true;
  }

  // applies the histogramMethod for the type boundary Zero
  bool medianFilter::histogramMethodZero(const vector<ubyte>& src,
                                          vector<ubyte>& dest,
                                          int sizeOfKernel) const {
    const int limit = sizeOfKernel/2;   //half size of the kernel
    // Median Filter
    int j,col,z;//index
    int tmp;
    int lessThanMedian = 0;
    ubyte median;
    quickMedian quickmedia;

    ivector histogram(256,int(0));  //the histogram for a vector<ubyte>
    vector<ubyte> vect(sizeOfKernel,ubyte(0));
    const int columnSize = src.size();
    const int halfKernelSize = sizeOfKernel/2;
    //resizes the vector destination
    dest.allocate(columnSize);

    const int lastCol = src.lastIdx()-limit;
    if (limit>lastCol) {
      // ups! kernel is bigger than the whole vector
      setStatusString("Kernels larger than data are not supported yet");
      return false;
    }

    ubyte value;
    //runs through the src's columns,inside the image
    histogram.fill(int(0));

    // very first pixel
    col=0;
    z=0;
    const int c=col+limit;
    for(j=col-limit;j<=c;++j,++z) {
      if(j<0) {
        value=0;
      } else { // if(j>=0)
        value=src.at(j);
      }
      vect.at(z)=value;
      ++histogram[value];
    }

    quickmedia.apply(vect, median);
    dest.at(col)=median;

    lessThanMedian=0;
    for(j=0;j<vect.size();++j) {
      if(vect[j] < median) {
        ++lessThanMedian;
      }
    }

    // rest of the left border
    while(col < limit) {
      value = 0;
      --histogram[value];
      if(value < median) {
        --lessThanMedian;
      }
    
      ++col;
      j=col+limit;

      value=src.at(j);
      ++histogram[value];
      if(value < median) {
        ++lessThanMedian;
      }

      if (lessThanMedian>halfKernelSize) {
        // 6. Step
        do {
          --median;
          lessThanMedian-=histogram[median];
        } while(lessThanMedian>halfKernelSize);
        dest.at(col)=median;
      } else { //if(lessThanMedian<=halfKernelSize) {
        // 5. Step
        tmp = lessThanMedian + histogram[median];
        while (tmp <= halfKernelSize) {
          lessThanMedian = tmp;
          ++median;
          tmp+=histogram[median];
        }
        dest.at(col)=median;
      }
    }
    // runs inside the vector
    histogramMethodMiddle(src,
                          dest,
                          histogram,
                          lessThanMedian,
                          col,
                          median,
                          sizeOfKernel);
    // the right border
    col=lastCol;
    while(col < (columnSize-1)) {
      j=col-limit;
      value=src.at(j);
      --histogram[value];
      if(value < median) {
        --lessThanMedian;
      }
    
      ++col;
      
      value=0;
      ++histogram[value];
      if(value < median) {
        ++lessThanMedian;
      }

      if (lessThanMedian > halfKernelSize) {
        // 6. Step
        do {
          --median;
          lessThanMedian -= histogram[median];
        } while(lessThanMedian > halfKernelSize);
        dest.at(col) = median;
      }
      else { //if(lessThanMedian<=halfKernelSize) {
        // 5. Step
        tmp = lessThanMedian + histogram[median];
        while (tmp <= halfKernelSize) {
          lessThanMedian = tmp;
          ++median;
          tmp+=histogram[median];
        }
        dest.at(col) = median;
      }
    }
    return true;
  }

  // applies the histogramMethod for the type boundary NoBoundary
  bool medianFilter::histogramMethodNoBoundary(const vector<ubyte>& src,
					       vector<ubyte>& dest,
					       int sizeOfKernel) const {
    const int limit = sizeOfKernel/2;   //half size of the kernel
    int j,col;
    int lessThanMedian = 0;
    ubyte median;
    quickMedian quickmedia;

    vector<ubyte> vect(sizeOfKernel,ubyte(0));
    ivector histogram(256,int(0));   // the histogram for a vector<ubyte>

    //resizes the destination vector
    dest.allocate(src.size());
    ubyte value;

    for(j=0;j<sizeOfKernel;++j) {    // determinate the median
      // at the beginning
      value = src.at(j);
      vect.at(j) = value; // of each rows and fill the histogram
      ++histogram[value];
    }
    quickmedia.apply(vect, median);
    dest.at(limit) = median;

    lessThanMedian=0;
    for(j=0;j<vect.size();++j) {// count the number of pixels with intensity
      if(vect[j] < median) {     // less than or equal to median
        ++lessThanMedian;
      }
    }

    // runs inside the image
    histogramMethodMiddle(src,dest,histogram,lessThanMedian,
                          col,median,sizeOfKernel);

    return true;
  }


  // apply especially with a histogram,only for type vector<ubyte>
  bool medianFilter::histogramMethod(const vector<ubyte>& src,
                                     vector<ubyte>& dest) const {
      
    if (src.empty()) {
      dest.clear();
      return true;
    }

    const parameters& param = getParameters();

    if (param.kernelSize <= 1) {
      dest.copy(src);
      return true;
    }

    const int sizeOfKernel = param.kernelSize + ((param.kernelSize%2 == 0) ? 1:0);
    bool control = false;   // return variable

    // 2. Step
    switch(param.boundaryType) {
      case lti::Zero:
        control = histogramMethodZero(src,dest,sizeOfKernel);
        break;
      case lti::Mirror:
        control = histogramMethodMirror(src,dest,sizeOfKernel);
        break;
      case lti::Constant:
        control = histogramMethodConstant(src,dest,sizeOfKernel);
        break;
      case lti::Periodic:
        control = histogramMethodPeriodic(src,dest,sizeOfKernel);
        break;
      case lti::NoBoundary:
        control = histogramMethodNoBoundary(src,dest,sizeOfKernel);
        break;
      default:
        setStatusString("Unknown boundary type");
        break;
    }
    return control;
  }




}
