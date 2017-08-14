/*
 * Copyright (C) 2000, 2001, 2002, 2003, 2004, 2005, 2006
 * Lehrstuhl fuer Technische Informatik, RWTH-Aachen, Germany
 *
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

/* 
 * \file   ltiKPCA_template.h
 *         Contains the functor to perform a kernel principal components
 *         analysis.
 * \author Pablo Alvarado
 * \author Jochen Wickel
 * \date   27.11.2009
 *
 * revisions ..: $Id: ltiKPCA_template.h,v 1.5 2009-09-08 21:35:08 alvarado Exp $
 */

#include "ltiObject.h"
#include "ltiMath.h"
#include <limits>

#include "ltiSymmetricEigenSystem.h"
#include "ltiSVD.h"
#include "ltiSecondOrderStatistics.h"
#include "ltiConjugateGradients.h"
#include "ltiFactory.h"

#undef _LTI_DEBUG
// #define _LTI_DEBUG 4
#include "ltiDebug.h"

namespace lti {

  // --------------------------------------------------
  // kpca::parameters
  // --------------------------------------------------
  // default constructor
  template <typename T>
  kpca<T>::parameters::parameters()
    : linearAlgebraFunctor::parameters() {

    kernel= kpca::createKernel("lti::radialKernel");
    destroyKernel_=true;
    
    resultDimension = 3;
    autoDimension = false;
    
    whitening = false;
    relevance=100000.0f;
  }
  
  // copy constructor
  template <typename T>
  kpca<T>::parameters::parameters(const parameters& other)
    : linearAlgebraFunctor::parameters(), kernel(0)  {
    copy(other);
  }

  // destructor
  template <typename T>
  kpca<T>::parameters::~parameters() {
    
    if (notNull(kernel) && destroyKernel_) {
      delete kernel;
      kernel=0;
    }
  }

  template <typename T>
  void 
  kpca<T>::parameters::setKernel(const kernelFunctorInterface<T>& k) {
    if (notNull(kernel) && destroyKernel_) {
      delete kernel;
    }
    kernel = kpca::cloneKernel(k);
    destroyKernel_=true;
  }

  template <typename T>
  void kpca<T>::parameters::attachKernel(kernelFunctorInterface<T>* k) {
    if (notNull(kernel) && destroyKernel_) {
      delete kernel;
    }
    kernel=k;
    destroyKernel_=true;
  }

  template <typename T>
  void kpca<T>::parameters::useKernel(kernelFunctorInterface<T>* k) {
    if (notNull(kernel) && destroyKernel_) {
      delete kernel;
    }
    kernel=k;
    destroyKernel_=false;
  }

  // get type name
  template <typename T>
  const std::string& kpca<T>::parameters::name() const {
    _LTI_RETURN_CLASS_NAME;
  }

  // copy member

  template <typename T>
  typename kpca<T>::parameters&
  kpca<T>::parameters::copy(const parameters& other) {
    linearAlgebraFunctor::parameters::copy(other);
    
    if (kernel != 0 && destroyKernel_) {
      delete kernel;
      kernel=0;
    }
    
    if (notNull(other.kernel)) {
      kernel = kpca::cloneKernel(*other.kernel);
    }
    destroyKernel_=true;

    resultDimension=other.resultDimension;
    autoDimension=other.autoDimension;
    whitening = other.whitening;
    relevance=other.relevance;

    return *this;
  }

  // alias for copy member
  template <typename T>
  typename kpca<T>::parameters&
  kpca<T>::parameters::operator=(const parameters& other) {
    copy(other);

    return *this;
  }

  // clone member
  template <typename T>
  typename kpca<T>::parameters* kpca<T>::parameters::clone() const {
    return new parameters(*this);
  }

  // clone member
  template <typename T>
  typename kpca<T>::parameters* kpca<T>::parameters::newInstance() const {
    return new parameters;
  }

  /*
   * write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  template <typename T>
  bool kpca<T>::parameters::write(ioHandler& handler,
                                  const bool complete) const {
    bool b=true;

    if (complete) {
      b=handler.writeBegin();
    }

    if (b) {
      lti::write(handler,"resultDimension",resultDimension);
      lti::write(handler,"autoDimension",autoDimension);
      lti::write(handler,"whitening",whitening);
      lti::write(handler,"relevance", relevance);

      bool haveKernel = false;
      if (notNull(kernel)) {
        const functor* kernelFunctor = dynamic_cast<const functor*>(kernel);
        if (notNull(kernelFunctor) && 
            lti::write(handler,"kernelType",kernelFunctor->name()) &&
            lti::write(handler,"kernelParam",kernelFunctor->getParameters())) {
          haveKernel = true;
        }
      }
      
      if (!haveKernel) {
        lti::write(handler,"kernelType","none");
      }
    }

    b = b && linearAlgebraFunctor::parameters::write(handler,false);

    if (complete) {
      b=b && handler.writeEnd();
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
  template <typename T>
  bool kpca<T>::parameters::read(ioHandler& handler,
                                 const bool complete) {
    bool b=true;

    if (complete) {
      b=handler.readBegin();
    }

    if (b) {
      lti::read(handler,"resultDimension",resultDimension);
      lti::read(handler,"autoDimension",autoDimension);
      lti::read(handler,"whitening",whitening);
      lti::read(handler,"relevance",relevance);

      if (notNull(kernel) && destroyKernel_) {
        delete kernel;
      }
      kernel=0;

      std::string kernelType;
      lti::read(handler,"kernelType",kernelType);
      if (kernelType != "none") {
        // try to construct the kernel
        kernel = kpca<T>::createKernel(kernelType);
        destroyKernel_ = true;

        if (notNull(kernel)) {
          // A kernel was successfully constructed, now load its paramters

          functor* kernelFunctor = dynamic_cast<functor*>(kernel);
          if (notNull(kernelFunctor)) {
            parametersManager::parameters* kpar =
              kernelFunctor->getParameters().clone();
            lti::read(handler,"kernelParam",*kpar);
            kernelFunctor->attachParameters(*kpar);
          }
        }
      }
    }

    b = b && linearAlgebraFunctor::parameters::read(handler,false);

    if (complete) {
      b=b && handler.readEnd();
    }

    return b;
  }

  // --------------------------------------------------
  // kpca
  // --------------------------------------------------

  // default constructor
  template <typename T>
  kpca<T>::kpca()
    : linearAlgebraFunctor() {
    parameters tmp;
    setParameters(tmp);
  }

  // default constructor
  template <typename T>
  kpca<T>::kpca(const parameters& pars)
    : linearAlgebraFunctor() {
    setParameters(pars);
  }

  // copy constructor
  template <typename T>
    kpca<T>::kpca(const kpca& oth) : linearAlgebraFunctor(),pcaInterface<T>()  {
    copy(oth);
  }

  // destructor
  template <typename T>
  kpca<T>::~kpca() {
  }

  // returns the name of this type
  template <typename T>
  const std::string& kpca<T>::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // copy member
  template <typename T>
  kpca<T>& kpca<T>::copy(const kpca& other) {
    linearAlgebraFunctor::copy(other);

    srcData_.copy(other.srcData_);
    kUnit_.copy(other.kUnit_);
    unitK_.copy(other.unitK_);
    unitKunit_ = other.unitKunit_;
    orderedEigVec_.copy(other.orderedEigVec_);
    transformMatrix_.copy(other.transformMatrix_);
    eigValues_.copy(other.eigValues_);
    whiteScale_.copy(other.whiteScale_);
    usedDimensionality_=other.usedDimensionality_;

    return (*this);
  }

  // clone member
  template <typename T>
  kpca<T>* kpca<T>::clone() const {
    return new kpca<T>(*this);
  }

  // clone member
  template <typename T>
  kpca<T>* kpca<T>::newInstance() const {
    return new kpca<T>;
  }

  // return parameters
  template <typename T>
  const typename kpca<T>::parameters& kpca<T>::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if (isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }

  // -------------------------------------------------------------------
  // The apply-methods!
  // -------------------------------------------------------------------

  // On copy apply for type matrix<T>!
  template <typename T>
  bool kpca<T>::apply(const matrix<T>& src,
                            matrix<T>& dest) {
    return computeTransformMatrix(src) && transform(src,dest);
  }


  /*
   * compute kernel matrix
   */
  template <typename T>
  bool kpca<T>::computeKernelMatrix(const matrix<T>& src,
                                          matrix<T>& kmat) {
    int i,j(0);
    T val;
    const parameters& param = getParameters();

    if (isNull(param.kernel)) {
      setStatusString("No valid kernel found in the parameters");
      return false;
    }

    const kernelFunctorInterface<T>& kernel = *param.kernel;
    const int n = src.rows();

    // this can be a huge matrix! so catch all exceptions to detect if
    // it is possible to create it
    try {
      kmat.allocate(n,n);
    } catch (lti::exception& exp) {
      // some error occured... let's report it
      setStatusString(exp.what());
      return false;
    }

    // and now the non diagonal elements
    for (i=0;i<n;++i) {
      // compute the diagonal of the matrix
      kmat.at(i,i) = (kernel.apply(src.getRow(i),src.getRow(i)));
      // the rest
      for (j=i+1;j<n;++j) {
        val = (kernel.apply(src.getRow(i),src.getRow(j)));
        kmat.at(i,j) = val;
        kmat.at(j,i) = val;
      }
    }

    // centering in feature space
    // This implements (I-1/n) K (I-1/n), with 1 the one-matrix
    // This is the same as K - 1*K/n - K*1/n + 1*K*1/n^2

    // 1*K contains for each element the sum of the elements of the column
    //     of the element
    // K*1 contains for each element the sum of the elements of the row of
    //     the element

    unitK_.assign(n,0);  // initialize with 0
    kUnit_.allocate(n); // initialization not required

    T sumRow,e;
    for (i=0;i<n;++i) {
      sumRow = T(0);
      for (j=0;j<n;++j) {
        e = kmat.at(i,j);
        sumRow+=e;
        unitK_.at(j)+=e;
      }
      kUnit_.at(i) = sumRow;
    }

    unitK_.divide(static_cast<float>(n));
    kUnit_.divide(static_cast<float>(n));

    // the mean of all elements of the matrix
    unitKunit_ = unitK_.computeSumOfElements()/n;

    // now center the data with the equation K - 1*K/n - K*1/n + 1*K*1/n^2
    for (i=0;i<n;++i) {
      for (j=0;j<n;++j) {
        kmat.at(i,j) += (unitKunit_ - (kUnit_.at(i)+unitK_.at(j)));
      }
    }

    return true;
  }

  /*
   * compute kernel matrix
   */
  template <typename T>
  bool kpca<T>::computeTestKernelMatrix(const matrix<T>& src,
                                              matrix<T>& kmat) const {
    int i,j(0);
    const parameters& param = getParameters();

    if (isNull(param.kernel)) {
      setStatusString("No valid kernel found in the parameters");
      return false;
    }
    const kernelFunctorInterface<T>& kernel = *param.kernel;

    const int m = srcData_.rows();
    const int n = src.rows();

    // this can be a huge matrix! so catch all exceptions to detect if
    // it is possible to create it
    try {
      kmat.allocate(n,m);
    } catch (lti::exception& exp) {
      // some error occured... let's report it
      setStatusString(exp.what());
      return false;
    }

    // compute products between src data and the stored srcData_
    for (i=0;i<n;++i) {
      for (j=0;j<m;++j) {
        kmat.at(i,j) = kernel.apply(src.getRow(i),srcData_.getRow(j));
      }
    }

    // centering in feature space
    // This implements (I-1/n) K (I-1/n), with 1 the one-matrix
    // This is the same as K - 1*K/n - K*1/n + 1*K*1/n^2

    // 1*K contains for each element the sum of the elements of the column
    //     of the element
    // K*1 contains for each element the sum of the elements of the row of
    //     the element

    // now center the data with the equation K - 1*K/n - K*1/n + 1*K*1/n^2
    T sumOfRows;
    for (i=0;i<n;++i) {
      sumOfRows = kmat.getRow(i).computeSumOfElements()/m;
      for (j=0;j<m;++j) {
        kmat.at(i,j) += (unitKunit_ - (sumOfRows + unitK_.at(j)));
      }
    }

    return true;
  }

  /*
   * compute kernel matrix for a vector, 
   */
  template <typename T>
  bool kpca<T>::computeTestKernelVectorUncentered(const vector<T>& src,
                                                  vector<T>& kvct) const {
    int j(0);
    const parameters& param = getParameters();
    const int m = srcData_.rows();

    if (isNull(param.kernel)) {
      setStatusString("No valid kernel found in the parameters");
      return false;
    }
    const kernelFunctorInterface<T>& kernel = *param.kernel;

    // this can be a huge matrix! so catch all exceptions to detect if
    // it is possible to create it
    try {
      kvct.allocate(m);
    } catch (lti::exception& exp) {
      // some error occured... let's report it
      setStatusString(exp.what());
      return false;
    }

    // and now the non diagonal elements
    for (j=0;j<m;++j) {
      kvct.at(j) = kernel.apply(src,srcData_.getRow(j));
    }

    return true;
  }


  /*
   * compute kernel matrix
   */
  template <typename T>
  bool kpca<T>::computeTestKernelVector(const vector<T>& src,
                                              vector<T>& kvct,
                                              T& sum) const {
    if (computeTestKernelVectorUncentered(src,kvct)) {
      const int m = srcData_.rows();

      // centering in feature space
      // This implements (I-1/n) K (I-1/n), with 1 the one-matrix
      // This is the same as K - 1*K/n - K*1/n + 1*K*1/n^2
      
      // 1*K contains for each element the sum of the elements of the column
      //     of the element
      // K*1 contains for each element the sum of the elements of the row of
      //     the element
      
      // now center the data with the equation K - 1*K/n - K*1/n + 1*K*1/n^2
      sum = kvct.computeSumOfElements()/m;
      for (int j=0;j<m;++j) {
        kvct.at(j) += (unitKunit_ - (sum + unitK_.at(j)));
      }
      return true;

    }
    return false;
  }

  template <typename T>
  kernelFunctorInterface<T>* 
  kpca<T>::cloneKernel(const kernelFunctorInterface<T>& kernel) {
    const functor* clonable = dynamic_cast<const functor*>(&kernel);
    functor* newOne = clonable->clone();
    if (notNull(newOne)) {
      kernelFunctorInterface<T>* res =
        dynamic_cast< kernelFunctorInterface<T>* >(newOne);
      if (notNull(res)) {
        return res;
      }
      delete res;
    }
    return 0;
  }

  // On copy apply for type matrix!
  template <typename T>
  bool kpca<T>::computeTransformMatrix(const matrix<T>& src) {
    const parameters& param = getParameters();

    // we need the source data for the data transformation later.
    srcData_.copy(src);

    // compute the kernel matrix
    matrix<T> kmat;
    computeKernelMatrix(srcData_,kmat);

    T eval;
    int i,j;
    const int n = src.rows();

    typename symmetricEigenSystem<T>::parameters sesPars;
    sesPars.sort = true;
    symmetricEigenSystem<T> eig(sesPars);
    
    // now compute eigenvectors of the kernel matrix
    if (!eig.apply(kmat,eigValues_,orderedEigVec_)) {
      _lti_debug("pX");
      setStatusString(eig.getStatusString());
      usedDimensionality_=0;
      eigValues_.clear();
      orderedEigVec_.clear();
      return false;
    }
   
    int dim;
    // now norm the eigenvectors with the eigenvalues
    for (i=0;i<n;++i) {
      if ((eigValues_.at(i)/eigValues_.at(0)) <
          std::numeric_limits<T>::epsilon()) {
        eigValues_.at(i) = T(0);
        for (j=0;j<orderedEigVec_.rows();++j) {
          orderedEigVec_.at(j,i) = 0.0;
        }
      } else {
        eval=sqrt(eigValues_.at(i));
        for (j=0;j<orderedEigVec_.rows();++j) {
          orderedEigVec_.at(j,i)/=eval;
        }
      }
    }

    // the number of dimensions of the srcData is not relevant
    dim = min(checkDim(),orderedEigVec_.columns());
    if (dim <= 0) {
      setStatusString("Covariance matrix has rank 0");
      return false;
    }

    transformMatrix_.copy(orderedEigVec_,0,0,container::MaxIndex, dim-1);

    transMatRowSum_.assign(transformMatrix_.columns(),T(0));
    for (j=0;j<transformMatrix_.columns();++j) {
      transMatRowSum_.add(transformMatrix_.getRow(j));
    }
    
    if (param.whitening) {
      whiteScale_.allocate(dim);
      whiteScale_.fill(eigValues_);
      whiteScale_.apply(sqrt);
    }

    return true;
  }

  template <typename T>
  bool kpca<T>::train(const matrix<T>& src) {
    return computeTransformMatrix(src);
  }

  /*
   * Transforms a single vector according to a previously computed
   * transformation matrix.
   */
  template <typename T>
  bool kpca<T>::transform(const vector<T>& src,
                                vector<T>& result) const {

    vector<T> tmp;
    T sum;
    computeTestKernelVector(src,tmp,sum);
    transformMatrix_.leftMultiply(tmp,result);

    if (getParameters().whitening) {
      result.edivide(whiteScale_);
    }

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
  bool kpca<T>::transform(const matrix<T> &src,
                                matrix<T>& result) const {

    computeTestKernelMatrix(src,result);
    result.multiply(transformMatrix_);

    if (getParameters().whitening) {
      for (int i=0; i<result.rows(); ++i) {
        result.getRow(i).edivide(whiteScale_);
      }
    }

    return true;
  }

  template <typename T>
  bool kpca<T>::transform(matrix<T>& srcdest) const {
    matrix<T> tmp;
    if (transform(srcdest,tmp)) {
      tmp.detach(srcdest);
      return true;
    }

    return false;
  }

  template <typename T>
  bool kpca<T>::dtransform(const vector<T>& x,
                           const vector<T>& ap2,
                           const T& cst,
                                 vector<T>& result) const {

    const int m=ap2.size();
    const int dim = x.size();

    const parameters& param = getParameters();

    if (isNull(param.kernel)) {
      setStatusString("No valid kernel found in the parameters");
      return false;
    }

    const kernelFunctorInterface<T>& krnl = *param.kernel;
    vector<T> sumGrads(dim,T(0));
    vector<T> acK(dim,T(0));
    vector<T> grad(dim);

    for (int j=0;j<m;++j) {
      krnl.gradient(x,srcData_.getRow(j),grad);
      sumGrads.add(grad);
      acK.addScaled(ap2.at(j),grad);
    }

    krnl.gradient(x,grad);
    result.addScaled(grad,cst,sumGrads);
    result.subtract(acK);

    return true;
  }

  template <typename T>
  bool kpca<T>::apply(matrix<T>& srcdest) {
    matrix<T> tmp;
    if (apply(srcdest,tmp)) {
      tmp.detach(srcdest);
      return true;
    }

    return false;
  }

  /*
   * Adapter class.
   *
   * Private class used to compute the error of an approximation
   */
  template <typename T>
  class kpca<T>::adapter {
  public:
    /**
     * Constructor
     */
    adapter(const kpca<T>& k,
            const vector<T>& p);

    /**
     * Evaluate the error function at the given site
     */
    T apply(const vector<T>& site) const;

    /**
     * Gradient 
     */
    void gradient(const vector<T>& site,
                        vector<T>& grad) const;
  private:
    /**
     * Kernel functor interface 
     */
    const kpca<T>& kpca_;

    /**
     * Reference to the source data
     */
    const vector<T>& p_;

    /**
     * Sum of squares of p, computed just once! plus unitKunit_
     */
    const T cst_;

    /**
     * Constant used in the computation of the gradient.
     *
     * This constant is equal  to 2 [ 1'  A p - 1 ] / M
     *                                  M
     * where 1'_M is a M-dimensional vector of 1s, A is the transformation
     * matrix and p is the principal components.
     */
    const T gradCst_;

    /**
     * Precomputed vector is equal to 2*A*p.
     */
    vector<T> ap2_;

    /**
     * Reference to the kernel used
     */
    kernelFunctorInterface<T>* k_;
  };

  template <typename T>
  kpca<T>::adapter::adapter(const kpca<T>& k,
                            const vector<T>& p)
    : kpca_(k),p_(p),cst_(p.dot(p) + kpca_.unitKunit_),
      gradCst_(T(2)*(p.dot(kpca_.transMatRowSum_)-T(1))/
               T(kpca_.srcData_.rows())),k_(0) {

    k_ = kpca_.getParameters().kernel;

    kpca_.transformMatrix_.multiply(p,ap2_);
    ap2_.multiply(T(2));
  }

  template <typename T>
  T kpca<T>::adapter::apply(const vector<T>& site) const {
    vector<T> tmp;
    T sum;
    kpca_.computeTestKernelVector(site,tmp,sum);
    kpca_.transformMatrix_.leftMultiply(tmp);
    return (k_->apply(site,site) + cst_ - 2*(p_.dot(tmp) + sum)); 
  }
  
  template <typename T>
  void kpca<T>::adapter::gradient(const vector<T>& site,
                                        vector<T>& grad) const {
    kpca_.dtransform(site,ap2_,gradCst_,grad);
  }
  
  template <typename T>
  bool kpca<T>::reconstruct(const vector<T>& coeff,vector<T>& dest) const {
    
    if (coeff.size() != usedDimensionality_) {
      setStatusString("Inconsistent dimensionality in parameters and " \
                      "coefficients size");
      return false;
    }

    _lti_debug3("Reconstruct " << coeff << std::endl);

    adapter f(*this,coeff);
    conjugateGradients cg;
    vector<T> org(coeff.size(),T(0)); // just some arbitrary starting point
    T val;
    return cg.apply(f,org,dest,val);
  }

  template <typename T>
  bool kpca<T>::reconstruct(const matrix<T>& coeff,matrix<T>& dest) const {

    bool rc=true;
    dest.allocate(coeff.rows(), transformMatrix_.rows());
    for (int i=0; i<coeff.rows(); i++) {
      rc = rc && reconstruct(coeff.getRow(i), dest.getRow(i));
    }
    return rc;
  }

  template <typename T>
  void kpca<T>::checkAdapter() const {
    // first try easiest with center
    vector<T> coeff(usedDimensionality_,T(0));
    vector<T> x(srcData_.columns(),T(0));
    vector<T> g;

    adapter f(*this,coeff);

    const int r=1;

    for (x.at(1)=-r;x.at(1)<=r;++x.at(1)) {
      for (x.at(0)=-r;x.at(0)<=r;++x.at(0)) {
        _lti_debug("Distance from phi" << x << " to coef " << coeff <<
                   " = " << f.apply(x) << std::endl);
        f.gradient(x,g);
        _lti_debug("  Gradient " << g << std::endl);
      }
    }
  }

  template <typename T>
  int kpca<T>::getUsedDimension() const {
    return usedDimensionality_;
  }

  template <typename T>
  bool kpca<T>::getTransformMatrix(matrix<T>& result) const {
    result.copy(transformMatrix_);
    return true;
  }

  template <typename T>
  const matrix<T>&
  kpca<T>::getTransformMatrix() const {
    return transformMatrix_;
  }

  template <typename T>
  int kpca<T>::checkDim() {
    const parameters& p=getParameters();
    if (p.autoDimension) {
      int n=0;
      if (eigValues_.size() > 0) {
        typename lti::vector<T>::const_iterator i = eigValues_.begin();
        // set the lowest accepted eigenvalue to the largest eigenvalue
        // divided by 1e6. This works only, if eigValues.at(0) is really
        // the maximum, i.e. if the eigenvalues are sorted descendingly
        // maybe 1e6 could be enlarged to something around 1e10
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
    } else {
      usedDimensionality_ = p.resultDimension;
    }
    return usedDimensionality_;
  }

  template <typename T>
  void kpca<T>::setDimension(int n) {
    parameters p(getParameters());
    p.resultDimension=n;
    setParameters(p);
  }

  template <typename T>
  bool kpca<T>::updateParameters() {
    bool boo=true;

    const parameters& p=getParameters();
    int dim = (p.autoDimension) ? usedDimensionality_ : p.resultDimension;

    if (dim <= 0) {
      dim=orderedEigVec_.columns();
    }

    if (dim != transformMatrix_.columns()) {
      transformMatrix_.copy(orderedEigVec_,0,0,container::MaxIndex,dim-1);

      
      transMatRowSum_.assign(transformMatrix_.columns(),T(0));
      for (int j=0;j<transformMatrix_.columns();++j) {
        transMatRowSum_.add(transformMatrix_.getRow(j));
      }

      if (p.whitening) {
        whiteScale_.allocate(dim);
        whiteScale_.fill(eigValues_);
        whiteScale_.apply(sqrt);
      }
    }

    return boo;
  }

  template <typename T>
  bool kpca<T>::getEigenVectors(matrix<T>& result) const {
    result.copy(orderedEigVec_);
    return true;
  }

  template <typename T>
  bool kpca<T>::getEigenVectorsInRows(matrix<T>& result) const {
    result.transpose(orderedEigVec_);
    return true;
  }

  template <typename T>
  bool kpca<T>::getEigenValues(vector<T>& result) const {
    result.copy(eigValues_);
    return true;
  }

  template <typename T>
  const matrix<T>& kpca<T>::getEigenVectors() const {
    return orderedEigVec_;
  }

  template <typename T>
  const vector<T>& kpca<T>::getEigenValues() const {
    return eigValues_;
  }



  template <typename T>
  bool kpca<T>::read(ioHandler& handler,
                     const bool complete) {
    bool b=true;

    if (complete) {
      b=handler.readBegin();
    }

    if (b) {
      parameters param;
      lti::read(handler,"parameters",param);
      setParameters(param);

      lti::read(handler,"srcData",srcData_);
      lti::read(handler,"kUnit",kUnit_);
      lti::read(handler,"unitK",unitK_);
      lti::read(handler,"unitKunit",unitKunit_);
      lti::read(handler,"orderedEigVec",orderedEigVec_);
      lti::read(handler,"eigValues",eigValues_);
      lti::read(handler,"usedDimensionality",usedDimensionality_);

      int dim=min(checkDim(),orderedEigVec_.columns());

      transformMatrix_.copy(orderedEigVec_,0,0,container::MaxIndex,dim-1);

      transMatRowSum_.assign(transformMatrix_.columns(),T(0));
      for (int j=0;j<transformMatrix_.columns();++j) {
        transMatRowSum_.add(transformMatrix_.getRow(j));
      }

      if (getParameters().whitening) {
        whiteScale_.allocate(dim);
        whiteScale_.fill(eigValues_);
        whiteScale_.apply(sqrt);
      }

      if (complete) {
        b=b && handler.readEnd();
      }
    }

    return b;
  }

  template <typename T>
  bool kpca<T>::write(ioHandler& handler,
                      const bool complete) const {
    bool b=true;

    if (complete) {
      b=handler.writeBegin();
    }

    if (b) {
      lti::write(handler,"parameters",getParameters());

      lti::write(handler,"srcData",srcData_);
      lti::write(handler,"kUnit",kUnit_);
      lti::write(handler,"unitK",unitK_);
      lti::write(handler,"unitKunit",unitKunit_);
      lti::write(handler,"orderedEigVec",orderedEigVec_);
      lti::write(handler,"eigValues",eigValues_);
      lti::write(handler,"usedDimensionality",usedDimensionality_);

      if (complete) {
        b=b && handler.writeEnd();
      }
    }
    return b;
  }


  template <typename T>
  kernelFunctorInterface<T>*
  kpca<T>::createKernel(const std::string& name) {
    functor* tmp = factory<functor>::getFactory().newInstance(name);
    if (notNull(tmp)) {
      kernelFunctorInterface<T>* ok = 
        dynamic_cast<kernelFunctorInterface<T>*>(tmp);
      if (notNull(ok)) {
        return ok;
      }
      delete tmp;
      return 0;
    }
    return 0;
  }

}

#include "ltiUndebug.h"
