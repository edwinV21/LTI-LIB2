/*
 * Copyright (C) 1998-2004
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
 * \file   ltiSeperableKernel_template.h
 *         Contains the template class seperableKernel<T>
 * \author Pablo Alvarado
 * \date   28.04.2000
 *
 * revisions ..: $Id: ltiSeparableKernel_template.h,v 1.8 2007-11-03 23:49:11 alvarado Exp $
 */
#include "ltiMinimizeBasis.h"
#include <list>

namespace lti {
  // --------------------------------------------------
  // Separable Kernel
  // --------------------------------------------------

  // constructor
  template<class T>
  separableKernel<T>::separableKernel() : container() {
    rowKernels_.clear();
    colKernels_.clear();
  }

  // copy constructor
  template<class T>
  separableKernel<T>::separableKernel(const separableKernel<T>& other) : container() {
    copy(other);
  }

  // constructor
  template<class T>
  separableKernel<T>::separableKernel(const int from,const int to,const T& iniValue)
    : container() {

    rowKernels_.clear();
    rowKernels_.resize(1);
    colKernels_.clear();
    colKernels_.resize(1);

    kernel1D<T> tmp(from,to,iniValue);
    rowKernels_[0] = tmp;
    colKernels_[0] = tmp;
  }

  // constructor
  template<class T>
  separableKernel<T>::separableKernel(const kernel1D<T>& subkernel)
    : container() {

    rowKernels_.clear();
    rowKernels_.resize(1);
    colKernels_.clear();
    colKernels_.resize(1);

    rowKernels_[0] = subkernel;
    colKernels_[0] = subkernel;
  }

  // destructor
  template<class T>
  separableKernel<T>::~separableKernel() {
    rowKernels_.clear();
    colKernels_.clear();
  }

  // copy
  template<class T>
  separableKernel<T>& separableKernel<T>::copy(const separableKernel<T>& other) {
    rowKernels_ = other.rowKernels_;
    colKernels_ = other.colKernels_;
    return *this;
  }

  // clone
  template<class T> 
  const std::string& separableKernel<T>::name() const{
    _LTI_RETURN_CLASS_NAME
  }

  // clone
  template<class T> 
  separableKernel<T>* separableKernel<T>::clone() const{
      return (new separableKernel<T>(*this));
  }
  
  // newInstance
  template<class T> 
  separableKernel<T>* separableKernel<T>::newInstance() const{
      return (new separableKernel<T>());
  }

  // separate two dimensional kernel
  template<class T>
    bool separableKernel<T>::separate(const kernel2D<T>& k,
				      const double& maxDev) {

    const matrix<T>& km=static_cast<const matrix<T> >(k);
    ipoint offset = k.getOffset();

    // operator to separate a matrix into basis vectors
    minimizeBasis<double> separator;
    // stateFunctor to calculate basic statistics of the matrix
//    serialStatsFunctor<T> stat;

    // get the statistics of the matrix
//    stat.reset();          // clear statistics
//    stat.considerElements(km);  // iterate to get data

    typename matrix<T>::const_iterator b = km.begin();
    typename matrix<T>::const_iterator e = km.end();
    T sumOfSquares = static_cast<T>(0);
    for (;b != e;++b) {
      sumOfSquares += (*b)*(*b);
    }
    
    // set parameters for separation
    minimizeBasis<double>::parameters param;
    param.deviationType = minimizeBasis<double>::Matrix;
    param.maxDeviation  = sqrt(static_cast<double>(sumOfSquares))*maxDev;
    param.method        = minimizeBasis<double>::Exact;

    separator.setParameters(param);

    // --- Zero analysis ---

    std::list<int> zeroRows;
    std::list<int> zeroCols;
    std::list<int>::iterator itCols,itRows;
    int x,y,i,j;

    // look for 0-cols
    for (x=0;x<km.columns();++x) {
      y=0;
      while ((y<km.rows()) && (km.at(y,x) == static_cast<T>(0))) {
        ++y;
      }

      if (y==km.rows()) {
        zeroCols.push_back(x);
      }
    }

    // look for 0-rows
    for (y=0;y<km.rows();++y) {
      x=0;
      while ((x<km.columns()) && (km.at(y,x) == static_cast<T>(0))) {
        ++x;
      }

      if (x==km.columns()) {
        zeroRows.push_back(y);
      }
    }

    // create a matrix without the 0-rows or 0-columns

    matrix<double> mtxNot0(km.rows()-static_cast<int>(zeroRows.size()),
                           km.columns()-static_cast<int>(zeroCols.size()));

    for (y=0,j=0,itRows=zeroRows.begin();y<km.rows();++y) {
      if ((itRows==zeroRows.end()) || ((*itRows) != y)) {
        for (x=0,i=0,itCols=zeroCols.begin();x<km.columns();++x) {
          if ((itCols==zeroCols.end()) || ((*itCols) != x)) {
            mtxNot0.at(j,i) = km.at(y,x);
            ++i;
          } else {
            ++itCols;
          }
        }
        ++j;
      } else {
        ++itRows;
      }
    }

    // separate!
    matrix<double> basis,factors;
    matrix<T> rows,cols;
    separator.apply(mtxNot0,basis,factors);

    // fill with 0-cols and 0-rows (transpose of basis + 0-cols)
    rows.allocate(basis.columns(),basis.rows()+static_cast<int>(zeroCols.size()));
    cols.allocate(factors.columns(),factors.rows()+static_cast<int>(zeroRows.size()));

    // fill the rows
    for (y=0;y<rows.rows();++y) {
      for (x=0,i=0,itCols=zeroCols.begin();x<rows.columns();++x) {
	if ((itCols==zeroCols.end()) || ((*itCols) != x)) {
	  rows.at(y,x) = static_cast<T>(basis.at(i,y));
	  ++i;
	} else {
	  rows.at(y,x) = T(0);
	  ++itCols;
	}
      }
    }

    // fill the cols
    for (y=0;y<cols.rows();++y) {
      for (x=0,i=0,itRows=zeroRows.begin();x<cols.columns();++x) {
	if ((itRows==zeroRows.end()) || ((*itRows) != x)) {
	  cols.at(y,x) = static_cast<T>(factors.at(i,y));
	  ++i;
	} else {
	  cols.at(y,x) = T(0);
	  ++itCols;
	}
      }
    }

    // fill the filter list with the kernels

    // this must always be OK!
    assert(rows.rows() == cols.rows());


    rowKernels_.clear();
    rowKernels_.resize(rows.rows());
    colKernels_.clear();
    colKernels_.resize(cols.rows());

    for (y=0;y<rows.rows();++y) {
      rowKernels_[y] = kernel1D<T>(rows.getRow(y),offset.x);
      colKernels_[y] = kernel1D<T>(cols.getRow(y),offset.y);
    }

    return true;
  }

  template<class T>
  int separableKernel<T>::getNumberOfPairs() const {
    return static_cast<int>(rowKernels_.size());
  }

  template<class T>
  void separableKernel<T>::setNumberOfPairs(const int numPairs) {
    rowKernels_.resize(numPairs);
    colKernels_.resize(numPairs);
  }

  template<class T>
  void separableKernel<T>::denormalize() {
    typename std::vector< kernel1D<T> >::iterator it;

    for (it = rowKernels_.begin();it != rowKernels_.end(); ++it) {
      (*it).denormalize();
    }

    for (it = colKernels_.begin();it != colKernels_.end(); ++it) {
      (*it).denormalize();
    }
  }

  template<class T>
  separableKernel<T>& separableKernel<T>::multiply(const T& value) {
    typename std::vector< kernel1D<T> >::iterator it;

    for (it = rowKernels_.begin();it != rowKernels_.end(); ++it) {
      (*it).multiply(value);
    }

    for (it = colKernels_.begin();it != colKernels_.end(); ++it) {
      (*it).multiply(value);
    }

    return (*this);
  }

  template<class T>
  T separableKernel<T>::computeSumOfElements() const {

    T result = T(0);
    int i;
    for (i=0;i<getNumberOfPairs();++i) {
      result+=getRowFilter(i).computeSumOfElements()*
        getColFilter(i).computeSumOfElements();
    }

    return result;
  }


  template<class T>
  void separableKernel<T>::setNorm(const T& newNorm) {

    typename std::vector< kernel1D<T> >::iterator it;
    for (it = rowKernels_.begin();it != rowKernels_.end(); ++it) {
      (*it).setNorm(newNorm);
    }

    for (it = colKernels_.begin();it != colKernels_.end(); ++it) {
      (*it).setNorm(newNorm);
    }
  }

  // mirror other kernel
  template <class T>
  separableKernel<T>& separableKernel<T>::mirror(const separableKernel<T>& other) {
    int i;

    setNumberOfPairs(other.getNumberOfPairs());

    for (i=0;i<getNumberOfPairs();++i) {
      getRowFilter(i).mirror(other.getRowFilter(i));
      getColFilter(i).mirror(other.getColFilter(i));
    }

    return (*this);
  }

  // mirror
  template <class T>
  separableKernel<T>& separableKernel<T>::mirror() {

    int i;
    for (i=0;i<getNumberOfPairs();++i) {
      getRowFilter(i).mirror();
      getColFilter(i).mirror();
    }

    return (*this);
  }

  // applies a C-Function to each element
  template<class T>
  separableKernel<T>& separableKernel<T>::apply(T (*function)(T)) {

    typename std::vector< kernel1D<T> >::iterator rit,cit;

    for (rit=rowKernels_.begin(),cit=colKernels_.begin();
         rit!=rowKernels_.end();
         ++rit,++cit) {
      (*rit).apply(*function);
      (*cit).apply(*function);
    }

    return (*this);
  }

  // applies a C-Function to each element
  template<class T>
  separableKernel<T>& separableKernel<T>::apply(T (*function)(const T&)) {

    typename std::vector< kernel1D<T> >::iterator rit,cit;

    for (rit=rowKernels_.begin(),cit=colKernels_.begin();
         rit!=rowKernels_.end();
         ++rit,++cit) {
      (*rit).apply(*function);
      (*cit).apply(*function);
    }

    return (*this);
  }

  // applies a C-Function to each element
  template<class T>
  separableKernel<T>& 
  separableKernel<T>::apply(const separableKernel<T>& other,
                            T (*function)(T)) {

    copy(other);
    return apply(*function);
  }

  // applies a C-Function to each element
  template<class T>
  separableKernel<T>& 
  separableKernel<T>::apply(const separableKernel<T>& other,
                                    T (*function)(const T&)) {

    copy(other);
    return apply(*function);
  }


  // -----------------------------------------------------------------
  // Storable interface
  // -----------------------------------------------------------------

  /*
   * write the object in the given ioHandler
   */
  template<class T>
  bool separableKernel<T>::write(ioHandler& handler,
                           const bool complete) const {
    bool b = true;

    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {
      lti::write(handler,"pairs",getNumberOfPairs());
      handler.writeBegin();
      handler.writeSymbol("data");
      handler.writeKeyValueSeparator();
      handler.writeBegin();
      int i;
      for (i=0;i<getNumberOfPairs();++i) {
        lti::write(handler,getRowFilter(i));
        handler.writeEOL();
        lti::write(handler,getColFilter(i));
        handler.writeEOL();
      }
      handler.writeEnd();
      handler.writeEnd();
    }

    if (complete) {
      b = b && handler.writeEnd();
    }

    return b;

  }

  /*
   * read the object from the given ioHandler
   */
  template<class T>
  bool separableKernel<T>::read(ioHandler& handler,const bool complete) {
    int pairs;

    bool b = true;

    if (complete) {
      b = handler.readBegin();
    }

    if (b) {
      std::string str;
      int i;

      lti::read(handler,"pairs",pairs);
      setNumberOfPairs(pairs);
      handler.readBegin();
      handler.readSymbol(str);
      if (str == "data") {
        handler.readBegin();
        for (i=0;i<pairs;++i) {
          lti::read(handler,getRowFilter(i));
          lti::read(handler,getColFilter(i));
        }
        handler.readEnd();
      }
      handler.readEnd();
    }

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

  template <class T>
  std::ostream& operator<<(std::ostream& s, const separableKernel<T>& kern) {
    s << "(";
    const int pairs = kern.getNumberOfPairs()-1;
    for (int i = 0; i < pairs; ++i) {
      s << "(" << kern.getRowFilter(i) << "\n  " 
        << kern.getColFilter(i) << ")\n ";
    }
    s << "(" << kern.getRowFilter(pairs) << "\n  "
      << kern.getColFilter(pairs) << "))";
    return s;
  }

}



