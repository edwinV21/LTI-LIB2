/*
 * Copyright (C)  2000, 2001, 2002, 2003, 2004, 2005, 2006
 * Lehrstuhl fuer Technische Informatik, RWTH-Aachen, Germany
 * Copyright (C) 2008
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
 * \file   ltiPCA_template.h
 *         Principal Components Analysis
 * \author Jochen Wickel
 * \author Pablo Alvarado
 * \date   27.11.2000
 *
 * revisions ..: $Id: ltiPCA_template.h,v 1.4 2010-04-13 03:14:11 alvarado Exp $
 */

#include "ltiSymmetricEigenSystem.h"
#include "ltiSecondOrderStatistics.h"
#include "ltiSVD.h"

#undef _LTI_DEBUG
//#define _LTI_DEBUG 2
#include "ltiDebug.h"



namespace lti {
  // --------------------------------------------------
  // pca::parameters
  // --------------------------------------------------
  
  /*
   * Default constructor
   */
  template <typename T>
  pca<T>::parameters::parameters() : linearAlgebraFunctor::parameters() {

    resultDimension=3;
    autoDimension=false;
    useCorrelation=false;
    whitening=false;
    relevance=T(100000);
    centerData=true;
    useSVD=false;
  }

  /*
   * Copy constructor
   * @param other the parameters object to be copied
   */
  template <typename T>
  pca<T>::parameters::parameters(const parameters& other) 
    : linearAlgebraFunctor::parameters() {
    copy(other);
  }

  /**
   * Destructor
   */
  template <typename T>
  pca<T>::parameters::~parameters() {
  }

  /*
   * Returns name of this type
   */
  template <typename T>
  const std::string& pca<T>::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  /*
   * Copy the contents of a parameters object.
   *
   * @param other the parameters object to be copied
   * @return a reference to this parameters object
   */
  template <typename T>
  typename pca<T>::parameters&
  pca<T>::parameters::copy(const parameters& other) {

    linearAlgebraFunctor::parameters::copy(other);
    
    resultDimension=other.resultDimension;
    autoDimension=other.autoDimension;
    useCorrelation=other.useCorrelation;
    whitening=other.whitening;
    relevance=other.relevance;
    centerData=other.centerData;
    useSVD=other.useSVD;

    return *this;
  }

  
  /**
   * Assigns the contents of the other object to this object
   */
  template <typename T>
  typename pca<T>::parameters& 
  pca<T>::parameters::operator=(const parameters& other) {
    copy(other);
    return *this;
  }

  /*
   * Returns a pointer to a clone of the parameters
   */
  template <typename T>
  typename pca<T>::parameters* pca<T>::parameters::clone() const {
    return new parameters(*this);
  }

  template <typename T>
  typename pca<T>::parameters* pca<T>::parameters::newInstance() const {
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
  bool pca<T>::parameters::read(ioHandler &handler, const bool complete) {
    bool b=true;
    
    if (complete) {
      b=handler.readBegin();
    }
    
    if (b) {
      lti::read(handler,"resultDimension",resultDimension);
      lti::read(handler,"autoDimension",autoDimension);
      lti::read(handler,"useCorrelation",useCorrelation);
      lti::read(handler,"whitening",whitening);
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
  bool pca<T>::parameters::write(ioHandler& handler,
                                 const bool complete) const {
    bool b=true;
    
    if (complete) {
      b=handler.writeBegin();
    }
    
    if (b) {
      lti::write(handler,"resultDimension",resultDimension);
      lti::write(handler,"autoDimension",autoDimension);
      lti::write(handler,"useCorrelation",useCorrelation);
      lti::write(handler,"whitening",whitening);
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
  // pca
  // --------------------------------------------------

  template <typename T>
  pca<T>::pca()
    : linearAlgebraFunctor(),usedDimensionality_(0) {
    parameters tmp;
    setParameters(tmp);
    usedDimensionality_=0; // indicate that no training has been done
  }

  // default constructor
  template <typename T>
  pca<T>::pca(const bool createDefParam)
    : linearAlgebraFunctor(),usedDimensionality_(0) {
    if (createDefParam) {
      parameters tmp;
      setParameters(tmp);
    }
    usedDimensionality_=0; // indicate that no training has been done
  }

  template <typename T>
  pca<T>::pca(const parameters& par)
    : linearAlgebraFunctor(),usedDimensionality_(0) {
    setParameters(par);    
    usedDimensionality_=0; // indicate that no training has been done
  }

  // copy constructor
  template <typename T>
  pca<T>::pca(const pca<T>& oth)
    : pcaInterface<T>(),linearAlgebraFunctor(),usedDimensionality_(0) {
    copy(oth);
  }

  // destructor
  template <typename T>
  pca<T>::~pca() {
  }

  // returns the name of this type
  template <typename T>
  const std::string& pca<T>::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // copy member
  template <typename T>
  pca<T>& pca<T>::copy(const pca<T>& other) {
    linearAlgebraFunctor::copy(other);

    orderedEigVec_.copy(other.orderedEigVec_);
    transformMatrix_.copy(other.transformMatrix_);
    eigValues_.copy(other.eigValues_);

    offset_.copy(other.offset_);
    transformedOffset_.copy(other.transformedOffset_);
    scale_.copy(other.scale_);
    whiteScale_.copy(other.whiteScale_);

    usedDimensionality_ = other.usedDimensionality_;

    return (*this);
  }

  template <typename T>
  pca<T>&
  pca<T>::operator=(const pca<T>& other) {
    return copy(other);
  }

  // clone member
  template <typename T>
  functor* pca<T>::clone() const {
    return new pca<T>(*this);
  }

  // clone member
  template <typename T>
  functor* pca<T>::newInstance() const {
    return new pca<T>();
  }

  // return parameters
  template <typename T>
  const typename pca<T>::parameters&
  pca<T>::getParameters() const {
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
  bool pca<T>::apply(const matrix<T>& src,matrix<T>& dest) {
    return computeTransformMatrix(src) && transform(src,dest);
  }

  // On copy apply for type matrix<T>!
  template <typename T>
  bool pca<T>::computeTransformMatrix(const matrix<T>& src) {

    if (src.empty()) {
      setStatusString("Data matrix is empty");
      reset();
      return false;
    }

    const parameters& param = getParameters();

    typename secondOrderStatistics<T>::parameters statsPar;
    statsPar.dataRepresentationType = Rows;
    statsPar.varianceType = secondOrderStatistics<T>::Empirical;
    statsPar.useCorrelation = param.useCorrelation;
    secondOrderStatistics<T> stats(statsPar);

    // determine the center of the data point distribution

    if (!param.useSVD) {

      typename symmetricEigenSystem<T>::parameters sesPars;
      sesPars.sort = true;
      symmetricEigenSystem<T> eig(sesPars);

      // correlation coefficient matrix
      matrix<T> cc;

      // compute correlation coefficients of data matrix
      stats.apply(src,offset_,cc);

      // now compute eigenvectors of correlation coefficient matrix
      if (!eig.apply(cc,eigValues_,orderedEigVec_)) {
        setStatusString(eig.getStatusString());
        reset();
        return false;
      }

    } else { // useSVD

      svd::parameters svdPar;
      svdPar.sort = true;
      svdPar.useMinDimensions = true;
      svd sv(svdPar);

      const int m=src.rows();

      matrix<T> tmp(src);
      for (int i=0; i<m; i++) {
        tmp.getRow(i).subtract(offset_);
      }
      sv.apply(tmp,eigValues_,orderedEigVec_);

      eigValues_.apply(lti::sqr);
      eigValues_.divide(static_cast<T>(m-1));

      _lti_debug(eigValues << std::endl);

      tmp.clear();
    }

    if (param.useCorrelation) {
      // now normalize the data matrix to zero mean and unit variance
      stats.variance(src,scale_,Rows);
      scale_.apply(sqrt); // compute standard deviation
      // mask columns with zero std deviation
      for (typename vector<T>::iterator si=scale_.begin();
           si != scale_.end(); si++) {
        if (abs(*si) < std::numeric_limits<T>::epsilon()) {
          *si=T(1);
        }
      }
    }

    // The checkDim() method gets the user specified percentage of dimensions
    // and alters the usedDimensionality_ attribute.
    int dim = min(checkDim(),orderedEigVec_.columns());

    if (dim <= 0) {
      transformMatrix_.clear();
      setStatusString("Covariance matrix has rank 0");
      return false;
    }

    // the transformation matrix is made simply by the first columns of
    // the ordered eigenvectors
    transformMatrix_.copy(orderedEigVec_,0,0,container::MaxIndex,dim-1);

    _lti_debug("peig: " << orderedEigVec_.size() << std::endl);
    _lti_debug("transform: " << transformMatrix_.size() << std::endl);

    if (param.whitening) {
      whiteScale_=eigValues_;
      whiteScale_.resize(dim,T(0));
      whiteScale_.apply(sqrt);
    }

    if (param.centerData) {
      transformedOffset_.resize(dim);
      transformedOffset_.fill(T(0));
    } else {
      transformMatrix_.leftMultiply(offset_,transformedOffset_);
    }
    return true;
  }

  template <typename T>
  bool pca<T>::train(const matrix<T>& src) {
    return computeTransformMatrix(src);
  }


  //=================================================================
  // Set the covariance matrix and the means directly
  template <typename T>
  bool pca<T>::setCovarianceAndMean(const matrix<T>& coVar,
                                    const vector<T>& meanVec) {
    const parameters& param = getParameters();

    typename symmetricEigenSystem<T>::parameters sesPars;
    sesPars.sort = true;
    sesPars.dimensions = 0;
    symmetricEigenSystem<T> eig(sesPars);

    const int cols=coVar.columns(); // src.columns();

    if (meanVec.size() != cols) {
      setStatusString("Size mismatch of input in setCovariance and Mean");
      return false;
    }

    // now compute eigenvectors of correlation coefficient matrix
    if (!eig.apply(coVar,eigValues_,orderedEigVec_)) {
      setStatusString(eig.getStatusString());
      usedDimensionality_=0;
      eigValues_.clear();
      orderedEigVec_.clear();
      return false;
    }

    offset_ = meanVec;

    int dim = min(checkDim(),orderedEigVec_.columns());

    if (dim <= 0) {
      transformMatrix_.clear();
      setStatusString("Covariance matrix has rank 0");
      return false;
    }

    // the transformation matrix is simply the first columns of
    // the ordered eigenvectors
    transformMatrix_.copy(orderedEigVec_,0,0,container::MaxIndex,dim-1);
    _lti_debug("peig: " << orderedEigVec_.size() << std::endl);
    _lti_debug("transform: " << transformMatrix_.size() << std::endl);

    if (param.whitening) {
      whiteScale_=eigValues_;
      whiteScale_.resize(dim,T(0));
      whiteScale_.apply(sqrt);
    }

    return true;
  }

  /*
   * Transforms a single vector according to a previously computed
   * transformation matrix.
   */
  template <typename T>
  bool pca<T>::transform(const vector<T>& src,vector<T>& result) const {

    const parameters& p = getParameters();

    vector<T> tmp;

    if (offset_.size() != src.size()) {
      setStatusString("Transformation matrix and input data incompatible.");
      return false;
    }

    tmp.subtract(src,offset_);
    if (p.useCorrelation) {
      tmp.edivide(scale_);
    }
    _lti_debug("e");

    transformMatrix_.leftMultiply(tmp,result);
    if (p.whitening) {
      result.edivide(whiteScale_);
    }
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
  bool pca<T>::transform(const matrix<T> &src,matrix<T>& result) const {
    const parameters& p = getParameters();
    if (offset_.size() != src.columns()) {
      setStatusString("Transformation matrix and input data incompatible.");
      return false;
    }

    int i;
    result.allocate(src.size());
    _lti_debug("Transform Matrix: " << transformMatrix << std::endl);
    _lti_debug("PCA: correlation " << scale <<
               (p.useCorrelation ? " (on)" : " (off)") << std::endl);
    for (i=0; i<result.rows(); i++) {
      result.getRow(i).subtract(src.getRow(i),offset_);
      if (p.useCorrelation) {
        result.getRow(i).edivide(scale_);
      }
    }

    result.multiply(transformMatrix_);
    if (p.whitening) {
      for (i=0; i<result.rows(); i++) {
        result.getRow(i).edivide(whiteScale_);
      }
    }
    if (!p.centerData) {
      for (i=0; i<result.rows(); i++) {
        result.getRow(i).add(transformedOffset_);
      }
    }

    return true;
  }

  template <typename T>
  bool pca<T>::transform(matrix<T>& srcdest) const {
    matrix<T> tmp;
    if (transform(srcdest,tmp)) {
      tmp.detach(srcdest);
      return true;
    }

    return false;
  }

  template <typename T>
  bool pca<T>::apply(matrix<T>& srcdest) {
    matrix<T> tmp;
    if (apply(srcdest,tmp)) {
      tmp.detach(srcdest);
      return true;
    }

    return false;
  }


  template <typename T>
  bool pca<T>::reconstruct(const vector<T>& coeff,vector<T>& dest) const {
    assert(coeff.size()==transformMatrix_.columns());

    transformMatrix_.multiply(coeff,dest);
    dest.add(offset_);
    return true;
  }

  template <typename T>
  bool pca<T>::reconstruct(const matrix<T>& coeff,matrix<T>& dest) const {
   
    bool rc=true;
    dest.allocate(coeff.rows(), transformMatrix_.rows());
    for (int i=0; i<coeff.rows(); i++) {
      rc = rc && reconstruct(coeff.getRow(i), dest.getRow(i));
    }
    return rc;
  }

  template <typename T>
  int pca<T>::getUsedDimension() const {
    return usedDimensionality_;
  }

  template <typename T>
  int pca<T>::checkDim() {
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
      usedDimensionality_ = p.resultDimension;
    }
    return usedDimensionality_;
  }

  template <typename T>
  void pca<T>::reset() {
    usedDimensionality_=0;
    eigValues_.clear();
    orderedEigVec_.clear();
    transformMatrix_.clear();
    offset_.clear();
    transformedOffset_.clear();
    scale_.clear();
    whiteScale_.clear();
  }

  template <typename T>
  void pca<T>::setDimension(int n) {
    parameters p(getParameters());
    p.resultDimension=n;
    setParameters(p);
  }

  template <typename T>
  bool pca<T>::updateParameters() {

    bool boo=true;
    const parameters& p=getParameters();

    // usedDimensionality is set by the checkDim() which can be called only
    // after a training session.  If no training has been performed yet,
    // then the returned value will be set to zero, which is catched
    // on the next step.
    int dim = (p.autoDimension) ? usedDimensionality_ : p.resultDimension;

    if (dim <= 0) { // if no training has been done, used everything
      dim=orderedEigVec_.columns();
    }

    if (dim != transformMatrix_.columns()) {
      transformMatrix_.copy(orderedEigVec_,0,0,container::MaxIndex,dim-1);

      if (p.whitening) {
        whiteScale_.copy(eigValues_);
        whiteScale_.resize(dim,T(0));// avoid applying sqrt to uninitializied
                                     // data. Valgrind detect this as error.
        whiteScale_.apply(sqrt);
      }
    }

    return boo;
  }

  template <typename T>
  bool pca<T>::getTransformMatrix(matrix<T>& result) const {
    result.copy(transformMatrix_);
    return true;
  }

  template <typename T>
  const matrix<T>&
  pca<T>::getTransformMatrix() const {
    return transformMatrix_;
  }

  template <typename T>
  bool pca<T>::getOffsetVector(vector<T>& result) const {
    result.copy(offset_);
    return true;
  }

  template <typename T>
  const vector<T>&
  pca<T>::getOffsetVector() const {
    return offset_;
  }

  template <typename T>
  bool pca<T>::getEigenVectors(matrix<T>& result) const {
    result.copy(orderedEigVec_);
    return true;
  }

  template <typename T>
  bool pca<T>::getEigenVectorsInRows(matrix<T>& result) const {
    result.transpose(orderedEigVec_);
    return true;
  }

  template <typename T>
  bool pca<T>::getEigenValues(vector<T>& result) const {
    result.copy(eigValues_);
    return true;
  }

  template <typename T>
  const matrix<T>&
  pca<T>::getEigenVectors() const {
    return orderedEigVec_;
  }

  template <typename T>
  const vector<T>&
  pca<T>::getEigenValues() const {
    return eigValues_;
  }

  template <typename T>
  bool pca<T>::read(ioHandler& handler,const bool complete) {
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
      lti::read(handler,"scale",scale_);

      lti::read(handler,"usedDimensionality",usedDimensionality_);

      int dim=min(checkDim(),orderedEigVec_.columns());
      transformMatrix_.clear();
      transformMatrix_.copy(orderedEigVec_,0,0,container::MaxIndex,dim-1);

      if (getParameters().whitening) {
        whiteScale_=eigValues_;
        whiteScale_.resize(dim,T(0));
        whiteScale_.apply(sqrt);
      }

      if (complete) {
        b=b && handler.readEnd();
      }

    }
    return b;
  }

  template <typename T>
  bool pca<T>::write(ioHandler& handler,
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
      lti::write(handler,"scale",scale_);

      lti::write(handler,"usedDimensionality",usedDimensionality_);
     
      if (complete) {
        b=b && handler.writeEnd();
      }
    }
    return b;
  }
}

#include "ltiUndebug.h"
