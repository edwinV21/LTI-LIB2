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
 * \file   ltiMedianFilter_template.h
 *         Contains the class medianFilter
 * \author Guy Wafo Moudhe
 * \date   16.03.2001
 *
 * $Id: ltiMedianFilter_template.h,v 1.5 2009-04-07 23:27:33 alvarado Exp $
 */

namespace lti {

  template <class T>
  bool medianFilter::realMedian(const matrix<T>& src, 
                                matrix<T>& dest) const {

    if (src.empty()) {
      dest.clear();
      return true;
    }

    const parameters& param = getParameters();

    if (param.kernelSize <= 1) {
      dest.copy(src);
      return true;
    }

    // Median Filter
    int i,j,row,col,z;
    int k=0;

    // the method can only be applied with a even value of kernelSize
    const int sizeOfKernel = param.kernelSize+((param.kernelSize%2 == 0)?1:0);
    const int limit = sizeOfKernel/2;

    quickMedian quickmedia;
    vector<T> vect(sizeOfKernel*sizeOfKernel,T(0));

    //resizes the destination matrix
    dest.allocate(src.rows(),src.columns());

    const int lastCol = src.lastColumn()-limit;
    const int lastRow = src.lastRow()-limit;

    //runs through the src's columns
    for(row=limit;row<=lastRow;++row) {
      const int r=row+limit;
      for(col=limit;col<=lastCol;++col) {  //runs through the src's rows
        const int c=col+limit;
        for(i=row-limit;i<=r;++i) { //runs through the rows within the kernel
          //runs through the columns within the kernel
          for(j=col-limit;j<=c;++j) {
            //gives the value from the kernel to the vector
            vect.at(k)=src[i][j];
            k++;
          }
        }
        //fills the destination matrix
        quickmedia.apply(vect, dest.at(row,col));
        k=0;
      }
    }

    // boundary conditions

    switch (param.boundaryType) {
      case lti::Zero:
        for(row=limit;row<=lastRow;++row) {  //runs the left columns
          z = sizeOfKernel*(sizeOfKernel-limit-1);
          vect.fill(T(0),0,z-1);
          const int r=row+limit;
          for(col=0;col<limit;++col) {
            const int c=col+limit;
            z=sizeOfKernel*(sizeOfKernel-limit-1-col);
            for(i=row-limit;i<=r;++i) {
              for(j=0;j<=c;++j){
                vect.at(z)=src[i][j];
                z++;
              }
            }
            quickmedia.apply(vect, dest.at(row,col));
          }
          //runs the right columns
          for(col=lastCol+1;col<=src.lastColumn();++col) {
            z=sizeOfKernel*(sizeOfKernel-(limit+1)-(src.lastColumn()-col));
            vect.fill(T(0),0,z-1);
            for(i=row-limit;i<=r;++i) {
              for(j=col-limit;j<=src.lastColumn();++j) {
                vect.at(z)=src[i][j];
                z++;
              }
            }
            quickmedia.apply(vect, dest.at(row,col));
          }
        }
        for(row=0;row<limit;++row) { //runs the rows at the top
          z=sizeOfKernel*(sizeOfKernel-limit-1);
          vect.fill(T(0),0,z-1);
          const int r=row+limit;
          // runs the rows at the top in the middle
          for(col=limit;col<=lastCol;++col) {
            const int c=col+limit;
            z=sizeOfKernel*(sizeOfKernel-(limit+1)-row);

            for(i=0;i<=r;++i) {

              for(j=col-limit;j<=c;++j) {

                vect.at(z)=src[i][j];
                z++;
              }
            }
            quickmedia.apply(vect, dest.at(row,col));
          }

          // runs at the top,left in the corner
          for(col=0;col<limit;++col) {
            const int c=col+limit;
            z=sizeOfKernel*sizeOfKernel-(limit+1)*(limit+1+row+col)-col*row;
            vect.fill(T(0),0,z-1);
            for(i=0;i<=r;++i) {

              for(j=0;j<=c;++j) {

                vect.at(z)=src[i][j];
                z++;
              }
            }
            quickmedia.apply(vect, dest.at(row,col));
          }
          //runs at the top,right in the corner
          for(col=lastCol+1;col<=src.lastColumn();++col) {

            z=sizeOfKernel*sizeOfKernel-(limit+1)*
              (limit+1+row+(src.lastColumn()-col))-row*(src.lastColumn()-col);
            vect.fill(T(0),0,z-1);
            for(i=0;i<=r;++i) {
              for(j=col-limit;j<=src.lastColumn();++j){
                vect.at(z)=src[i][j];
                z++;
              }
            }
            quickmedia.apply(vect, dest.at(row,col));
          }
        }
        //runs the rows at the bottom
        for(row=lastRow+1;row<=src.lastRow();++row) {
          z=sizeOfKernel*(sizeOfKernel-limit-1);
          vect.fill(T(0),0,z-1);
          //runs the rows at the bottom in the middle
          for(col=limit;col<=lastCol;++col) {
            const int c=col+limit;
            z=sizeOfKernel*(sizeOfKernel-(limit+1)-(src.lastRow()-row));
            vect.fill(T(0),0,z-1);
            for(i=row-limit;i<=src.lastRow();++i) {
              for(j=col-limit;j<=c;++j) {
                vect.at(z)=src[i][j];
                z++;
              }
            }
            quickmedia.apply(vect, dest.at(row,col));
          }
          //runs at the bottom,left in the corner
          for(col=0;col<limit;++col) {
            const int c=col+limit;
            z=sizeOfKernel*sizeOfKernel-(limit+1)*
              (limit+1+(src.lastRow()-row)+col)-col*(src.lastRow()-row);
            vect.fill(T(0),0,z-1);
            for(i=row-limit;i<=src.lastRow();++i) {

              for(j=0;j<=c;++j) {

                vect.at(z)=src[i][j];
                z++;
              }
            }
            quickmedia.apply(vect, dest.at(row,col));
          }
          //runs at the bottom,right in the corner
          for(col=lastCol+1;col<=src.lastColumn();++col) {

            z=sizeOfKernel*sizeOfKernel-(limit+1)*
              (limit+1+(src.lastRow()-row)+(src.lastColumn()-col))-
              (src.lastRow()-row)*(src.lastColumn()-col);
            vect.fill(T(0),0,z-1);
            for (i=row-limit;i<=src.lastRow();++i) {
              for (j=col-limit;j<=src.lastColumn();++j) {
                vect.at(z)=src[i][j];
                z++;
              }
            }
            quickmedia.apply(vect, dest.at(row,col));
          }
        }
        break;
      case lti::Mirror:
        for(row=limit;row<=lastRow;++row) {   //runs the columns left
          const int r=row+limit;
          for(col=0;col<limit;++col) {
            const int c=col+limit;
            z=0;
            for(i=row-limit;i<=r;++i) {

              for(j=col-limit;j<=c;++j) {

                if(j<0) {
                  vect.at(z)=src[i][-j-1];
                  z++;
                }
                else{// if(j>=0){
                  vect.at(z)=src[i][j];
                  z++;
                }
              }
            }
            quickmedia.apply(vect, dest.at(row,col));
          }

          for(col=lastCol+1;col<=src.lastColumn();++col) {
            //runs the columns right
            const int c=col+limit;
            z=0;
            for(i=row-limit;i<=r;++i) {
              for(j=col-limit;j<=c;++j) {
                if(j<=src.lastColumn()) {
                  vect.at(z)=src[i][j];
                  z++;
                }
                else{// if(j>src.lastColumn()) {
                  vect.at(z)=src[i][src.lastColumn()+(src.columns()-j)];
                  z++;
                }
              }
            }
            quickmedia.apply(vect, dest.at(row,col));
          }
        }
        for(row=0;row<limit;++row) {   //runs the rows at the top
          const int r=row+limit;
          // runs the rows at the top in the middle
          for(col=limit;col<=lastCol;++col) {
            const int c=col+limit;
            z=0;
            for(i=row-limit;i<=r;++i) {

              for(j=col-limit;j<=c;++j) {

                if(i<0) {
                  vect.at(z)=src[-i-1][j];
                  z++;
                }
                else { //if(i>=0) {
                  vect.at(z)=src[i][j];
                  z++;
                }
              }
            }
            quickmedia.apply(vect, dest.at(row,col));
          }

          for(col=0;col<limit;++col) {   // runs at the top,left in the corner
            const int c=col+limit;
            z=0;
            for(i=row-limit;i<=r;++i) {

              for(j=col-limit;j<=c;++j) {
                if(i<0 && j<0){
                  vect.at(z)=src[-i-1][-j-1];
                  z++;
                }
                else if(i>=0 && j<0) {
                  vect.at(z)=src[i][-j-1];
                  z++;
                }
                else if(i<0 && j>=0) {
                  vect.at(z)=src[-i-1][j];
                  z++;
                }
                else {   //if(i>=0 && j>=0) {
                  vect.at(z)=src[i][j];
                  z++;
                }
              }
            }

            quickmedia.apply(vect, dest.at(row,col));
          }
          //runs at the top,right in the corner
          for(col=lastCol+1;col<src.columns();++col) {
            const int c=col+limit;
            z=0;
            for(i=row-limit;i<=r;++i) {
              for(j=col-limit;j<=c;++j) {
                if(i<0 && j<=src.lastColumn()) {
                  vect.at(z)=src[-i-1][j];
                  z++;
                }
                else if(i<0 && j>src.lastColumn()) {
                  vect.at(z)=src[-i-1][src.lastColumn()+(src.columns()-j)];
                  z++;
                }
                else if(i>=0 && j>src.lastColumn()) {
                  vect.at(z)=src[i][src.lastColumn()+(src.columns()-j)];
                  z++;
                }
                else { //if(i>=0 && j<=src.lastColumn()) {
                  vect.at(z)=src[i][j];
                  z++;
                }
              }
            }
            quickmedia.apply(vect, dest.at(row,col));
          }
        }
        //runs the rows at the bottom
        for(row=lastRow+1;row<=src.lastRow();++row) {
          const int r=row+limit;
          //runs the rows at the bottom in the middle
          for(col=limit;col<=lastCol;++col) {
            const int c=col+limit;
            z=0;
            for(i=row-limit;i<=r;++i) {
              for(j=col-limit;j<=c;++j) {
                if(i<=src.lastRow()) {
                  vect.at(z)=src[i][j];
                  z++;
                }
                else {// if(i>src.lastRow()) {
                  vect.at(z)=src[src.lastRow()+(src.rows()-i)][j];
                  z++;
                }
              }
            }
            quickmedia.apply(vect, dest.at(row,col));
          }
          for(col=0;col<limit;++col) { //runs at the bottom,left in the corner
            const int c=col+limit;
            z=0;
            for(i=row-limit;i<=r;++i) {
              for(j=col-limit;j<=c;++j) {
                if(i<=src.lastRow() && j<0) {
                  vect.at(z)=src[i][-j-1];
                  z++;
                }
                else if(i<=src.lastRow() && j>=0) {
                  vect.at(z)=src[i][j];
                  z++;
                }
                else if(i>src.lastRow() && j<0) {
                  vect.at(z)=src[src.lastRow()+(src.rows()-i)][-j-1];
                  z++;
                }
                else if(i>src.lastRow() && j>=0) {
                  vect.at(z)=src[src.lastRow()+(src.rows()-i)][j];
                  z++;
                }
              }
            }
            quickmedia.apply(vect, dest.at(row,col));
          }
          //runs at the bottom,right in the corner
          for(col=lastCol+1;col<src.columns();++col) {
            const int c=col+limit;
            z=0;
            for(i=row-limit;i<=r;++i) {
              for(j=col-limit;j<=c;++j) {
                if(i<=src.lastRow() && j<=src.lastColumn()) {
                  vect.at(z)=src[i][j];
                  z++;
                }
                else if(i<=src.lastRow() && j>src.lastColumn()) {
                  vect.at(z)=src[i][src.lastColumn()+(src.columns()-j)];
                  z++;
                }
                else if(i>src.lastRow() && j<=src.lastColumn()) {
                  vect.at(z)=src[src.lastRow()+(src.rows()-i)][j];
                  z++;
                }
                else if(i>src.lastRow() && j>src.lastColumn()) {
                  vect.at(z)=src[src.lastRow()+(src.rows()-i)]
                    [src.lastColumn()+(src.columns()-j)];
                  z++;
                }
              }
            }
            quickmedia.apply(vect, dest.at(row,col));
          }
        }
        break;
      case lti::Periodic:
        for(row=limit;row<=lastRow;++row) {          //runs the left columns
          const int r=row+limit;
          for(col=0;col<limit;++col) {
            const int c=col+limit;
            z=0;
            for(i=row-limit;i<=r;++i) {

              for(j=col-limit;j<=c;++j) {

                if(j<0) {
                  vect.at(z)=src[i][src.columns()+j];
                  z++;
                }
                else{// if(j>=0) {
                  vect.at(z)=src[i][j];
                  z++;
                }
              }
            }
            quickmedia.apply(vect, dest.at(row,col));
          }

          //runs right the columns
          for(col=lastCol+1;col<=src.lastColumn();++col) {
            const int c=col+limit;
            z=0;
            for(i=row-limit;i<=r;++i) {
              for(j=col-limit;j<=c;++j) {
                if(j<=src.lastColumn()) {
                  vect.at(z)=src[i][j];
                  z++;
                }
                else{// if(j>src.lastColumn()) {
                  vect.at(z)=src[i][j-src.columns()];
                  z++;
                }
              }
            }
            quickmedia.apply(vect, dest.at(row,col));
          }
        }
        for(row=0;row<limit;++row) {                //runs the rows at the top
          const int r=row+limit;
          // runs the rows at the top in the middle
          for(col=limit;col<=lastCol;++col) {
            const int c=col+limit;
            z=0;
            for(i=row-limit;i<=r;++i) {

              for(j=col-limit;j<=c;++j) {

                if(i<0) {
                  vect.at(z)=src[i+src.rows()][j];
                  z++;
                }
                else{//if(i>=0) {
                  vect.at(z)=src[i][j];
                  z++;
                }
              }
            }
            quickmedia.apply(vect, dest.at(row,col));
          }
          // runs at the top,left in the corner
          for(col=0;col<limit;++col) {
            const int c=col+limit;
            z=0;
            for(i=row-limit;i<=r;++i) {

              for(j=col-limit;j<=c;++j) {
                if(i<0 && j<0) {
                  vect.at(z)=src[src.rows()+i][src.columns()+j];
                  z++;
                }
                else if(i>=0 && j<0) {
                  vect.at(z)=src[i][src.columns()+j];
                  z++;
                }
                else if(i<0 && j>=0) {
                  vect.at(z)=src[src.rows()+i][j];
                  z++;
                }
                else if(i>=0 && j>=0) {
                  vect.at(z)=src[i][j];
                  z++;
                }
              }
            }

            quickmedia.apply(vect, dest.at(row,col));
          }
          //runs at the top,right in the corner
          for(col=lastCol+1;col<src.columns();++col) {
            const int c=col+limit;
            z=0;
            for(i=row-limit;i<=r;++i) {
              for(j=col-limit;j<=c;++j) {
                if(i<0 && j<=src.lastColumn()) {
                  vect.at(z)=src[src.rows()+i][j];
                  z++;
                }
                else if(i<0 && j>src.lastColumn()) {
                  vect.at(z)=src[src.rows()+i][j-src.columns()];
                  z++;
                }
                else if(i>=0 && j>src.lastColumn()) {
                  vect.at(z)=src[i][j-src.columns()];
                  z++;
                }
                else {//if(i>=0 && j<=src.lastColumn()) {
                  vect.at(z)=src[i][j];
                  z++;
                }
              }
            }
            quickmedia.apply(vect, dest.at(row,col));
          }

        }
        //runs the rows at the bottom
        for(row=lastRow+1;row<=src.lastRow();++row) {
          const int r=row+limit;
          //runs the rows at the bottom in the middle
          for(col=limit;col<=lastCol;++col) {
            const int c=col+limit;
            z=0;
            for(i=row-limit;i<=r;++i) {
              for(j=col-limit;j<=c;++j) {
                if(i<=src.lastRow()) {
                  vect.at(z)=src[i][j];
                  z++;
                }
                else{// if(i>src.lastRow()) {
                  vect.at(z)=src[i-src.rows()][j];
                  z++;
                }
              }
            }
            quickmedia.apply(vect, dest.at(row,col));
          }
          for(col=0;col<limit;++col) {  //runs at the bottom,left in the corner
            const int c=col+limit;
            z=0;
            for(i=row-limit;i<=r;++i) {
              for(j=col-limit;j<=c;++j) {
                if(i<=src.lastRow() && j<0) {
                  vect.at(z)=src[i][src.columns()+j];
                  z++;
                }
                else if(i<=src.lastRow() && j>=0) {
                  vect.at(z)=src[i][j];
                  z++;
                }
                else if(i>src.lastRow() && j<0) {
                  vect.at(z)=src[i-src.rows()][src.columns()+j];
                  z++;
                }
                else if(i>src.lastRow() && j>=0) {
                  vect.at(z)=src[i-src.rows()][j];
                  z++;
                }
              }
            }
            quickmedia.apply(vect, dest.at(row,col));
          }
          //runs at the bottom,right in the corner
          for(col=lastCol+1;col<src.columns();++col) {
            const int c=col+limit;
            z=0;
            for(i=row-limit;i<=r;++i) {
              for(j=col-limit;j<=c;++j) {
                if(i<=src.lastRow() && j<=src.lastColumn()) {
                  vect.at(z)=src[i][j];
                  z++;
                }
                else if(i<=src.lastRow() && j>src.lastColumn()) {
                  vect.at(z)=src[i][j-src.columns()];
                  z++;
                }
                else if(i>src.lastRow() && j<=src.lastColumn()) {
                  vect.at(z)=src[i-src.rows()][j];
                  z++;
                }
                else if(i>src.lastRow() && j>src.lastColumn()) {
                  vect.at(z)=src[i-src.rows()][j-src.columns()];
                  z++;
                }
              }
            }
            quickmedia.apply(vect, dest.at(row,col));
          }

        }
        break;

      case lti::Constant:
        for(row=limit;row<=lastRow;++row) {          //runs the left columns
          const int r=row+limit;
          for(col=0;col<limit;++col) {
            const int c=col+limit;
            z=0;
            for(i=row-limit;i<=r;++i) {

              for(j=col-limit;j<=c;++j) {

                if(j<0) {
                  vect.at(z)=src[i][0];
                  z++;
                }
                else{// if(j>=0) {
                  vect.at(z)=src[i][j];
                  z++;
                }
              }
            }
            quickmedia.apply(vect, dest.at(row,col));
          }

          //runs the right columns
          for(col=lastCol+1;col<=src.lastColumn();++col) {
            const int c=col+limit;
            z=0;
            for(i=row-limit;i<=r;++i) {
              for(j=col-limit;j<=c;++j) {
                if(j<=src.lastColumn()) {
                  vect.at(z)=src[i][j];
                  z++;
                }
                else{// if(j>src.lastColumn()) {
                  vect.at(z)=src[i][src.lastColumn()];
                  z++;
                }
              }
            }
            quickmedia.apply(vect, dest.at(row,col));
          }
        }
        for(row=0;row<limit;++row) {                //runs the rows at the top
          const int r=row+limit;
          // runs the rows at the top in the middle
          for(col=limit;col<=lastCol;++col) {
            const int c=col+limit;
            z=0;
            for(i=row-limit;i<=r;++i) {

              for(j=col-limit;j<=c;++j) {

                if(i<0) {
                  vect.at(z)=src[0][j];
                  z++;
                }
                else{//if(i>=0) {
                  vect.at(z)=src[i][j];
                  z++;
                }
              }
            }
            quickmedia.apply(vect, dest.at(row,col));
          }
          // runs at the top,left in the corner
          for(col=0;col<limit;++col) {
            const int c=col+limit;
            z=0;
            for(i=row-limit;i<=r;++i) {

              for(j=col-limit;j<=c;++j) {
                if(i<0 && j<0){
                  vect.at(z)=src[0][0];
                  z++;
                }
                else if(i>=0 && j<0) {
                  vect.at(z)=src[i][0];
                  z++;
                }
                else if(i<0 && j>=0) {
                  vect.at(z)=src[0][j];
                  z++;
                }
                else if(i>=0 && j>=0) {
                  vect.at(z)=src[i][j];
                  z++;
                }
              }
            }

            quickmedia.apply(vect, dest.at(row,col));
          }
          //runs at the top,right in the corner
          for(col=lastCol+1;col<src.columns();++col) {
            const int c=col+limit;
            z=0;
            for(i=row-limit;i<=r;++i) {
              for(j=col-limit;j<=c;++j) {
                if(i<0 && j<=src.lastColumn()) {
                  vect.at(z)=src[0][j];
                  z++;
                }
                else if(i<0 && j>src.lastColumn()) {
                  vect.at(z)=src[0][src.lastColumn()];
                  z++;
                }
                else if(i>=0 && j>src.lastColumn()) {
                  vect.at(z)=src[i][src.lastColumn()];
                  z++;
                }
                else {//if(i>=0 && j<=src.lastColumn()) {
                  vect.at(z)=src[i][j];
                  z++;
                }
              }
            }
            quickmedia.apply(vect, dest.at(row,col));
          }
        }
        //runs the rows at the bottom
        for(row=lastRow+1;row<=src.lastRow();++row) {
          const int r=row+limit;
          //runs the rows at the bottom in the middle
          for(col=limit;col<=lastCol;++col) {
            const int c=col+limit;
            z=0;
            for(i=row-limit;i<=r;++i) {
              for(j=col-limit;j<=c;++j) {
                if(i<=src.lastRow()) {
                  vect.at(z)=src[i][j];
                  z++;
                }
                else{// if(i>src.lastRow()) {
                  vect.at(z)=src[src.lastRow()][j];
                  z++;
                }
              }
            }
            quickmedia.apply(vect, dest.at(row,col));
          }
          //runs at the bottom,left in the corner
          for(col=0;col<limit;++col) {
            const int c=col+limit;
            z=0;
            for(i=row-limit;i<=r;++i) {
              for(j=col-limit;j<=c;++j) {
                if(i<=src.lastRow() && j<0) {
                  vect.at(z)=src[i][0];
                  z++;
                }
                else if(i<=src.lastRow() && j>=0) {
                  vect.at(z)=src[i][j];
                  z++;
                }
                else if(i>src.lastRow() && j<0) {
                  vect.at(z)=src[src.lastRow()][0];
                  z++;
                }
                else if(i>src.lastRow() && j>=0) {
                  vect.at(z)=src[src.lastRow()][j];
                  z++;
                }
              }
            }
            quickmedia.apply(vect, dest.at(row,col));
          }
          //runs at the bottom,right in the corner
          for(col=lastCol+1;col<src.columns();++col) {
            const int c=col+limit;
            z=0;
            for(i=row-limit;i<=r;++i) {
              for(j=col-limit;j<=c;++j) {
                if(i<=src.lastRow() && j<=src.lastColumn()) {
                  vect.at(z)=src[i][j];
                  z++;
                }
                else if(i<=src.lastRow() && j>src.lastColumn()) {
                  vect.at(z)=src[i][src.lastColumn()];
                  z++;
                }
                else if(i>src.lastRow() && j<=src.lastColumn()) {
                  vect.at(z)=src[src.lastRow()][j];
                  z++;
                }
                else if(i>src.lastRow() && j>src.lastColumn()) {
                  vect.at(z)=src[src.lastRow()][src.lastColumn()];
                  z++;
                }
              }
            }
            quickmedia.apply(vect, dest.at(row,col));
          }
        }

        break;

      default:
        break;
        
    }
    
    return true;
  }



  template <class T>
  bool medianFilter::realMedian(const vector<T>& src, 
                                vector<T>& dest) const {

    if (src.empty()) {
      dest.clear();
      return true;
    }

    const parameters& param = getParameters();

    if (param.kernelSize <= 1) {
      dest.copy(src);
      return true;
    }

    // Median Filter
    int col,z;

    // the method can only be applied with a odd value of kernelSize
    const int sizeOfKernel = param.kernelSize+((param.kernelSize%2 == 0)?1:0);
    const int limit = sizeOfKernel/2;
    const int sizeOfKernelm1=sizeOfKernel-1;

    quickMedian quickmedia;
    vector<T> vect(sizeOfKernel,T(0));
    //resizes the destination matrix
    dest.allocate(src.size());

    const int lastCol = src.lastIdx()-limit;

    if (limit>lastCol) {
      // ups! kernel is bigger than the whole vector
      setStatusString("Kernels larger than data are not supported yet");
      return false;
    }

    //runs through the src's columns
    for(col=limit;col<=lastCol;++col) {  //runs through the src's rows
      vect.fill(src,0,sizeOfKernelm1,col-limit);
      //fills the destination matrix
      quickmedia.apply(vect, dest.at(col));
    }

    // boundary conditions

    const int offset = limit-src.lastIdx();

    switch (param.boundaryType) {
    case lti::Zero:
      // left side
      for(col=0;col<limit;++col) {
        z=limit-col;
        vect.fill(T(0),0,z-1);
        vect.fill(src,z);
        quickmedia.apply(vect, dest.at(col));
      }
      
      // right side
      for(col=lastCol+1;col<=src.lastIdx();++col) {
        z = col+offset;
        vect.fill(T(0),0,z-1);
        vect.fill(src,z,vect.lastIdx(),col-limit);
        quickmedia.apply(vect, dest.at(col));
      }
      break;
      
    case lti::Constant: {
      // left side
      const T left = src.at(0);
      for(col=0;col<limit;++col) {
        z=limit-col;
        vect.fill(left,0,z-1);
        vect.fill(src,z);
        quickmedia.apply(vect, dest.at(col));
      }

      // right side
      const T right = src.at(src.lastIdx());
      for(col=lastCol+1;col<=src.lastIdx();++col) {
        z = col+offset;
        vect.fill(right,0,z-1);
        vect.fill(src,z,vect.lastIdx(),col-limit);
        quickmedia.apply(vect, dest.at(col));
      }
    } break;

    case lti::Mirror: {
      // left side
      for(col=0;col<limit;++col) {
        z=limit-col;
        vect.fill(src,0,z-1);
        vect.fill(src,z);
        quickmedia.apply(vect, dest.at(col));
      }

      // right side
      for(col=lastCol+1;col<=src.lastIdx();++col) {
        z = col+offset;
        vect.fill(src,0,z-1,src.size()-z);
        vect.fill(src,z,vect.lastIdx(),col-limit);
        quickmedia.apply(vect, dest.at(col));
      }
    } break;

    case lti::Periodic: {
      // left side
      for(col=0;col<limit;++col) {
        z=limit-col;
        vect.fill(src,0,z-1,src.size()-z);
        vect.fill(src,z);
        quickmedia.apply(vect, dest.at(col));
      }

      // right side
      for(col=lastCol+1;col<=src.lastIdx();++col) {
        z = col+offset;
        vect.fill(src,0,z-1);
        vect.fill(src,z,vect.lastIdx(),col-limit);
        quickmedia.apply(vect, dest.at(col));
      }
    } break;
      
    default:
      break;
    }
    
    return true;
  }

}
