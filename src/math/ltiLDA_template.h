/*
 * Copyright (C)  2000, 2001, 2002, 2003, 2004, 2005, 2006
 * Lehrstuhl fuer Technische Informatik, RWTH-Aachen, Germany
 * Copyright (C) 2010
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

/*
 * \file   ltiLDA_template.h
 *         Linear Discriminant Analysis
 * \author Pablo Alvarado
 * \author Jochen Wickel
 * \date   09.10.2010
 *
 * revisions ..: $Id: ltiLDA_template.h,v 1.3 2010-11-04 02:24:39 alvarado Exp $
 */

#include "ltiEigenSystem.h"
#include "ltiSVD.h"
#include "ltiTypeInfo.h"

#undef _LTI_DEBUG
// #define _LTI_DEBUG 4
#include "ltiDebug.h"

namespace lti {
  // --------------------------------------------------
  // lda::parameters
  // --------------------------------------------------
  
  /*
   * Default constructor
   */
  template <typename T>
  lda<T>::parameters::parameters() : linearAlgebraFunctor::parameters() {

    resultDimension=3;
    autoDimension=false;
    relevance=T(100000);
    centerData=true;
    useSVD=false;
  }

  /*
   * Copy constructor
   * @param other the parameters object to be copied
   */
  template <typename T>
  lda<T>::parameters::parameters(const parameters& other) 
    : linearAlgebraFunctor::parameters() {
    copy(other);
  }

  /**
   * Destructor
   */
  template <typename T>
  lda<T>::parameters::~parameters() {
  }

  /*
   * Returns name of this type
   */
  template <typename T>
  const std::string& lda<T>::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  /*
   * Copy the contents of a parameters object.
   *
   * @param other the parameters object to be copied
   * @return a reference to this parameters object
   */
  template <typename T>
  typename lda<T>::parameters&
  lda<T>::parameters::copy(const parameters& other) {

    linearAlgebraFunctor::parameters::copy(other);
    
    resultDimension=other.resultDimension;
    autoDimension=other.autoDimension;
    relevance=other.relevance;
    centerData=other.centerData;
    useSVD=other.useSVD;

    return *this;
  }

  
  /**
   * Assigns the contents of the other object to this object
   */
  template <typename T>
  typename lda<T>::parameters& 
  lda<T>::parameters::operator=(const parameters& other) {
    copy(other);
    return *this;
  }

  /*
   * Returns a pointer to a clone of the parameters
   */
  template <typename T>
  typename lda<T>::parameters* lda<T>::parameters::clone() const {
    return new parameters(*this);
  }

  template <typename T>
  typename lda<T>::parameters* lda<T>::parameters::newInstance() const {
    return new parameters;
  }

  /*
   * Read the parameters from the given ioHandler.
   *
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also read, otherwise only the data block will be read.
   * @return true if write was successful
   */
  template<typename T>
  bool lda<T>::parameters::read(ioHandler &handler, const bool complete) {
    bool b=true;
    
    if (complete) {
      b=handler.readBegin();
    }
    
    if (b) {
      lti::read(handler,"resultDimension",resultDimension);
      lti::read(handler,"autoDimension",autoDimension);
      lti::read(handler,"relevance", relevance);
      lti::read(handler,"centerData", centerData);
      lti::read(handler,"useSVD", useSVD);
    }
    
    b = b && linearAlgebraFunctor::parameters::read(handler,false);
    
    if (complete) {
      b=b && handler.readEnd();
    }
    
    return b;
  }

  /**
   * Write the parameters in the given ioHandler.
   *
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be
   *        written.
   * @return true if write was successful
   */
  template <typename T>
  bool lda<T>::parameters::write(ioHandler& handler,
                                 const bool complete) const {
    bool b=true;
    
    if (complete) {
      b=handler.writeBegin();
    }
    
    if (b) {
      lti::write(handler,"resultDimension",resultDimension);
      lti::write(handler,"autoDimension",autoDimension);
      lti::write(handler,"relevance", relevance);
      lti::write(handler,"centerData", centerData);
      lti::write(handler,"useSVD", useSVD);
    }
    
    b = b && linearAlgebraFunctor::parameters::write(handler,false);
    
    if (complete) {
      b=b && handler.writeEnd();
    }
    
    return b;
  }


  // --------------------------------------------------
  // lda
  // --------------------------------------------------

  template <typename T>
  lda<T>::lda()
    : linearAlgebraFunctor(),usedDimensionality_(0) {
    parameters tmp;
    setParameters(tmp);
    usedDimensionality_=0; // indicate that no training has been done
  }

  // default constructor
  template <typename T>
  lda<T>::lda(const bool createDefParam)
    : linearAlgebraFunctor(),usedDimensionality_(0) {
    if (createDefParam) {
      parameters tmp;
      setParameters(tmp);
    }
    usedDimensionality_=0; // indicate that no training has been done
  }

  template <typename T>
  lda<T>::lda(const parameters& par)
    : linearAlgebraFunctor(),usedDimensionality_(0) {
    setParameters(par);    
    usedDimensionality_=0; // indicate that no training has been done
  }

  // copy constructor
  template <typename T>
  lda<T>::lda(const lda<T>& oth)
    : ldaInterface<T>(),linearAlgebraFunctor(),usedDimensionality_(0) {
    copy(oth);
  }

  // destructor
  template <typename T>
  lda<T>::~lda() {
  }

  // returns the name of this type
  template <typename T>
  const std::string& lda<T>::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // copy member
  template <typename T>
  lda<T>& lda<T>::copy(const lda<T>& other) {
    linearAlgebraFunctor::copy(other);

    orderedEigVec_.copy(other.orderedEigVec_);
    transformMatrix_.copy(other.transformMatrix_);
    eigValues_.copy(other.eigValues_);

    offset_.copy(other.offset_);
    transformedOffset_.copy(other.transformedOffset_);

    usedDimensionality_ = other.usedDimensionality_;

    return (*this);
  }

  template <typename T>
  lda<T>&
  lda<T>::operator=(const lda<T>& other) {
    return copy(other);
  }

  // clone member
  template <typename T>
  functor* lda<T>::clone() const {
    return new lda<T>(*this);
  }

  // clone member
  template <typename T>
  functor* lda<T>::newInstance() const {
    return new lda<T>();
  }

  // return parameters
  template <typename T>
  const typename lda<T>::parameters&
  lda<T>::getParameters() const {
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

  // On copy apply for type matrix<T>!
  template <typename T>
  bool lda<T>::computeTransformMatrix(const matrix<T>& src,
                                      const ivector& labels) {
    if (getParameters().useSVD) {
      return computeTransformMatrix2(src,labels);
    } 
    return computeTransformMatrix1(src,labels);
  }

  // On copy apply for type matrix<T>!
  template <typename T>
  bool lda<T>::computeTransformMatrix1(const matrix<T>& src,
                                       const ivector& labels) {

    if (src.empty()) {
      setStatusString("Data matrix is empty");
      reset();
      return false;
    }

    if (src.rows() != labels.size()) {
      setStatusString("Label number incompatible with number of vectors");
      reset();
      return false;
    }

    matrix<T> Sw,Sb;
 
   return 
      computeSbSw(src,labels,Sb,Sw,offset_) &&
      buildTransform(Sw,Sb);

  }

  /**
   * From the intra and inter covariances build the transformation matrix
   */
  template <typename T>
  bool lda<T>::buildTransform(const matrix<T>& Sw, const matrix<T>& Sb) {
    matrix<T> iSw;
    if (!inv_.apply(Sw,iSw)) {
      setStatusString("Matrix Sw could not be inverted:");
      appendStatusString(inv_.getStatusString());
      return false;
    }
    
    iSw.multiply(Sb);

    // The checkDim() method gets the user specified percentage of dimensions
    // and alters the usedDimensionality_ attribute.
    typename eigenSystem<T>::parameters esPar;
    esPar.sort=true;
    esPar.dimensions = 0;
    esPar.leftVector = false;

    eigenSystem<T> es(esPar);

    if (!es.apply(iSw,eigValues_,orderedEigVec_)) {
      setStatusString(es.getStatusString());
      usedDimensionality_=0;
      eigValues_.clear();
      orderedEigVec_.clear();
      return false;
    }

    int dim = checkDim();

    if (dim <= 0) {
      transformMatrix_.clear();
      setStatusString("Covariance matrix has rank 0");
      return false;
    }

    transformMatrix_.copy(orderedEigVec_,0,0,orderedEigVec_.rows(),dim-1);

    if (getParameters().centerData) {
      transformedOffset_.resize(dim);
      transformedOffset_.fill(T(0));
    } else {
      transformMatrix_.leftMultiply(offset_,transformedOffset_);
    }
    return true;    
  }


  // On copy apply for type matrix<T>!
  template <typename T>
  bool lda<T>::computeTransformMatrix2(const matrix<T>& src,
                                       const ivector& labels) {

    if (src.empty()) {
      setStatusString("Data matrix is empty");
      reset();
      return false;
    }

    if (src.rows() != labels.size()) {
      setStatusString("Label number incompatible with number of vectors");
      reset();
      return false;
    }

    matrix<T> Sw,Sb;
    computeSbSw(src,labels,Sb,Sw,offset_);

    matrix<T> iSw;
    if (!inv_.apply(Sw,iSw)) {
      setStatusString("Matrix Sw could not be inverted:");
      appendStatusString(inv_.getStatusString());
      return false;
    }
    
    // This method will force using just symmetric matrices for the
    // eigensystem solutions (but twice), which should be faster and more 
    // stable for larger problems
    

    // we need to compute the square root matrix of Sb, and its inverse,
    // and we do so through its eigenvector/eigenvalue decomposition, which
    // we do through svd, since it is quite robust
    svd::parameters svdPar;
    svdPar.sort = true;
    svdPar.useDivideAndConquer = true;
    svdPar.transposeV = true;

    svd svder(svdPar);
    matrix<T> U,Vt;
    vector<T> S;
    svder.apply(Sb,U,S,Vt);
    
    matrix<T> Sbrt(Sb.size()),Sbirt(Sb.size());
    
    for (int i=0;i<S.size();++i) {
      const T sv = S.at(i);
      const T srtsv = sqrt(abs(sv));
      const T isrtsv = (srtsv>0) ? T(1)/srtsv : T(0);
      Sbrt.getRow(i).multiply(Vt.getRow(i),srtsv);
      Sbirt.getRow(i).multiply(Vt.getRow(i),isrtsv);
    }

    Sbrt.leftMultiply(U);
    Sbirt.leftMultiply(U);

    iSw.multiply(Sbrt);
    iSw.leftMultiply(Sbrt);

    // free some memory
    S.clear();
    Sbrt.clear();

    if (!svder.apply(iSw,U,eigValues_,Vt))  {
      setStatusString(svder.getStatusString());
      usedDimensionality_=0;
      eigValues_.clear();
      orderedEigVec_.clear();
      return false;
    }

    orderedEigVec_.multiply(Sbirt,U);

    int dim = checkDim();

    if (dim <= 0) {
      transformMatrix_.clear();
      setStatusString("Covariance matrix has rank 0");
      return false;
    }

    transformMatrix_.copy(orderedEigVec_,0,0,orderedEigVec_.rows(),dim-1);

    if (getParameters().centerData) {
      transformedOffset_.resize(dim);
      transformedOffset_.fill(T(0));
    } else {
      transformMatrix_.leftMultiply(offset_,transformedOffset_);
    }
    return true;
  }



  // On copy apply for type matrix<T>!
  template <typename T>
  bool lda<T>::apply(const matrix<T>& src, 
                     const ivector& labels,
                     matrix<T>& dest) {
    return computeTransformMatrix(src,labels) && transform(src,dest);
  }

  template <typename T>
  bool lda<T>::train(const matrix<T>& src,
                     const ivector& labels) {
    return computeTransformMatrix(src,labels);
  }

  /*
   * Transforms a single vector according to a previously computed
   * transformation matrix.
   */
  template <typename T>
  bool lda<T>::transform(const vector<T>& src,vector<T>& result) const {

    const parameters& p = getParameters();

    vector<T> tmp;

    if (offset_.size() != src.size()) {
      setStatusString("Transformation matrix and input data incompatible.");
      return false;
    }

    tmp.subtract(src,offset_);
    transformMatrix_.leftMultiply(tmp,result);
    if (!p.centerData) {
      result.add(transformedOffset_);
    }
    _lti_debug("f\n");

    return true;
  }

  /*
   * Transform an entire matrix according to a previously computed
   * transformation matrix. Unfortunately, we must choose a name
   * different from apply.
   * @param src the data matrix
   * @param result the matrix which will receive the transformed data
   * @return a reference to <code>result</code>
   */
  template <typename T>
  bool lda<T>::transform(const matrix<T> &src,matrix<T>& result) const {
    const parameters& p = getParameters();
    if (offset_.size() != src.columns()) {
      setStatusString("Transformation matrix and input data incompatible.");
      return false;
    }

    int i;
    result.allocate(src.size());
    _lti_debug("Transform Matrix: " << transformMatrix_ << std::endl);

    for (i=0; i<result.rows(); i++) {
      result.getRow(i).subtract(src.getRow(i),offset_);
    }

    result.multiply(transformMatrix_);
    if (!p.centerData) {
      for (i=0; i<result.rows(); i++) {
        result.getRow(i).add(transformedOffset_);
      }
    }

    return true;
  }

  template <typename T>
  bool lda<T>::transform(matrix<T>& srcdest) const {
    matrix<T> tmp;
    if (transform(srcdest,tmp)) {
      tmp.detach(srcdest);
      return true;
    }

    return false;
  }

  template <typename T>
  bool lda<T>::apply(matrix<T>& srcdest, 
                     const ivector& labels) {
    matrix<T> tmp;
    if (apply(srcdest,labels,tmp)) {
      tmp.detach(srcdest);
      return true;
    }
    return false;
  }

  template <typename T>
  bool lda<T>::reconstruct(const vector<T>& coeff,vector<T>& dest) const {
    assert(coeff.size()==transformMatrix_.columns());

    transformMatrix_.multiply(coeff,dest);
    dest.add(offset_);
    return true;
  }

  template <typename T>
  bool lda<T>::reconstruct(const matrix<T>& coeff,matrix<T>& dest) const {
   
    bool rc=true;
    dest.allocate(coeff.rows(), transformMatrix_.rows());
    for (int i=0; i<coeff.rows(); i++) {
      rc = rc && reconstruct(coeff.getRow(i), dest.getRow(i));
    }
    return rc;
  }

  template <typename T>
  int lda<T>::getUsedDimension() const {
    return usedDimensionality_;
  }

  /*
   * Compute the between-class scatter matrix Sb and the 
   */
  template <class T>
  bool lda<T>::computeSbSw(const matrix<T>& src,
                           const ivector &labels,
                           matrix<T>& Sb,
                           matrix<T>& Sw,
                           vector<T>& offset) const {

    // TODO: This method should be replicated for the case that nclasses is
    //       too large, in which case the current method would require too
    //       much memory.  It is easily done by computing it like in the
    //       LTI-Lib 1 class.

    _lti_debug("\nBegin computeSbSw\n");

    if (src.rows() != labels.size()) {
      setStatusString("Wrong size of labels vector");
      return false;
    }

    /* For reference, the between-class scatter matrix is defined as 
     * 
     * Sb = sum_(for all classes c) (mu_c - mu_t)(mu_c - mu_t)'
     *
     * where mu_c is the mean vector of the class c and mu_t is the total
     * mean of all classes, and the ' means transpose (the whole thing is an
     * outer product)
     *
     * Sw = sum_(for all classes c) sum_(for all elements i in class c)
     *         (x_i-mu_c)(x_i-mu_c)'
     */
    const int cols    = src.columns();
    const int nclasses= labels.findMaximum()+1;
    const int samples = src.rows();

    // class mean
    matrix<T> cmean(nclasses,cols,T(0));
    // number of elements per class
    vector<T> mc(nclasses,T(0));
    // mean of all vector of all classes
    vector<T> tmean(cols,T(0));
    // temporary storage matrix
    matrix<T> outProd;

    // accumulation of the outer products of each element
    std::vector< matrix<T> > accXi(nclasses);

    // initialize all accumulation matrices
    for (int i=0;i<nclasses;++i) {
      accXi[i].assign(cols,cols,T(0));
    }

    // first, compute the class-mean of each class, and accumulate the
    // outer products of each element for each class

    // 1. accumulation stage
    for (int i=0; i<samples; i++) {
      const int idx = labels.at(i);
      const vector<T>& row = src.getRow(i);
      cmean.getRow(idx).add(row);
      tmean.add(row);
      mc.at(idx)++;

      outProd.outerProduct(row,row);
      accXi[idx].add(outProd);
    }
    
    // 2. normalization stage 
    // (also compute the class mean minus the total mean)
    Sb.assign(cols,cols,T(0));
    Sw.assign(cols,cols,T(0));
    tmean.divide(static_cast<float>(samples));

    offset.copy(tmean);
    _lti_debug("Total mean: " << tmean << std::endl);

    vector<T> xmmc;
    for (int i=0;i<nclasses;++i) {
      if (mc.at(i) <= 0) {
        continue; // not a valid class... just ignore
      }
      vector<T>& row = cmean.getRow(i);
      row.divide(mc.at(i));   // here we got the real class mean

      _lti_debug("Mean of class " << i << ": " << row << std::endl);

      xmmc.subtract(row,tmean);// map to a coordinate system centered at tmean

      outProd.outerProduct(xmmc,xmmc);
      Sb.addScaled(T(mc.at(i)),outProd);

      outProd.outerProduct(row,row);
      Sw.add(accXi[i].addScaled(-mc.at(i),outProd));
      
      _lti_debug("Covariance of class " << i << ":\n" << accXi[i] << std::endl);
    }
    Sw.divide(static_cast<float>(labels.size()));

    _lti_debug("Sw\n" << Sw << "\nSb:\n" << Sb << std::endl);

    _lti_debug("\nEnd computeSbSw\n");
    return true;
  }

  template <typename T>
  int lda<T>::checkDim() {
    const parameters& p=getParameters();
    if (p.autoDimension) {
      int n=0;
      if (eigValues_.size() > 0) {
        typename vector<T>::const_iterator i=eigValues_.begin();
        // set the lowest accepted eigenvalue to the largest
        // eigenvalue divided by the "relevance" parameter. This works
        // only, if eigValues_.at(0) is really the maximum, i.e. if
        // the eigenvalues are sorted descendingly.
        T evLimit=abs(*i)/p.relevance;

        while (i != eigValues_.end()) {
          if (abs(*i) > evLimit) {
            n++;
          }
          i++;
        }

        usedDimensionality_=n;
      } else {
        usedDimensionality_=0;
      }
      _lti_debug("Result dimensionality: " << n << std::endl);
    } else {
      usedDimensionality_ = p.resultDimension<=0 ?
                              eigValues_.size() :
                              p.resultDimension;
    }
    return usedDimensionality_;
  }

  template <typename T>
  void lda<T>::reset() {
    usedDimensionality_=0;
    eigValues_.clear();
    orderedEigVec_.clear();
    transformMatrix_.clear();
    offset_.clear();
    transformedOffset_.clear();
  }

  template <typename T>
  void lda<T>::setDimension(int n) {
    parameters p(getParameters());
    p.resultDimension=n;
    setParameters(p);
  }

  template <typename T>
  bool lda<T>::updateParameters() {

    bool boo=true;
    const parameters& p=getParameters();

    // usedDimensionality is set by the checkDim() which can be called only
    // after a training session.  If no training has been performed yet,
    // then the returned value will be set to zero, which is catched
    // on the next step.
    int dim = (p.autoDimension) ? usedDimensionality_ : p.resultDimension;

    if (dim <= 0) { // if no training has been done, use everything
      dim=orderedEigVec_.columns();
    }

    if (dim != transformMatrix_.columns()) {
      transformMatrix_.copy(orderedEigVec_,0,0,container::MaxIndex,dim-1);
    }

    if (p.useSVD) {
      inv_.useSVD();
    } else {
      inv_.useLUD();
    }

    return boo;
  }

  template <typename T>
  bool lda<T>::getTransformMatrix(matrix<T>& result) const {
    result.copy(transformMatrix_);
    return true;
  }

  template <typename T>
  const matrix<T>&
  lda<T>::getTransformMatrix() const {
    return transformMatrix_;
  }

  template <typename T>
  bool lda<T>::getOffsetVector(vector<T>& result) const {
    result.copy(offset_);
    return true;
  }

  template <typename T>
  const vector<T>&
  lda<T>::getOffsetVector() const {
    return offset_;
  }

  template <typename T>
  bool lda<T>::getEigenVectors(matrix<T>& result) const {
    result.copy(orderedEigVec_);
    return true;
  }

  template <typename T>
  bool lda<T>::getEigenVectorsInRows(matrix<T>& result) const {
    result.transpose(orderedEigVec_);
    return true;
  }

  template <typename T>
  bool lda<T>::getEigenValues(vector<T>& result) const {
    result.copy(eigValues_);
    return true;
  }

  template <typename T>
  const matrix<T>&
  lda<T>::getEigenVectors() const {
    return orderedEigVec_;
  }

  template <typename T>
  const vector<T>&
  lda<T>::getEigenValues() const {
    return eigValues_;
  }

  template <typename T>
  bool lda<T>::read(ioHandler& handler,const bool complete) {
    bool b=true;

    if (complete) {
      b=handler.readBegin();
    }

    if (b) {
      parameters param;
      lti::read(handler,"parameters",param);
      setParameters(param);
      
      lti::read(handler,"eigenVectors",orderedEigVec_);
      lti::read(handler,"eigenValues",eigValues_);
      lti::read(handler,"transformedOffset",transformedOffset_);
      lti::read(handler,"offset",offset_);

      lti::read(handler,"usedDimensionality",usedDimensionality_);

      int dim=min(checkDim(),orderedEigVec_.columns());
      transformMatrix_.clear();
      transformMatrix_.copy(orderedEigVec_,0,0,container::MaxIndex,dim-1);

      if (complete) {
        b=b && handler.readEnd();
      }

    }
    return b;
  }

  template <typename T>
  bool lda<T>::write(ioHandler& handler,
                     const bool complete) const {
    bool b=true;

    if (complete) {
      b=handler.writeBegin();
    }

    if (b) {
      lti::write(handler,"parameters",getParameters());

      lti::write(handler,"eigenVectors",orderedEigVec_);
      lti::write(handler,"eigenValues",eigValues_);
      lti::write(handler,"transformedOffset",transformedOffset_);

      lti::write(handler,"offset",offset_);

      lti::write(handler,"usedDimensionality",usedDimensionality_);
     
      if (complete) {
        b=b && handler.writeEnd();
      }
    }
    return b;
  }
}

#include "ltiUndebug.h"
