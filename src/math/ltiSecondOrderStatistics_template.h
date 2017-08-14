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

/**
 * \file   ltiSecondOrderStatistics_template.h
 *         Contains the functor secondOrderStatistics, for means,
 *         variances and covariance matrix
 * \author Jochen Wickel
 * \author Peter Doerfler
 * \date   25.09.2000
 *
 * $Id: ltiSecondOrderStatistics_template.h,v 1.15 2012-07-23 17:39:25 alvarado Exp $
 */

namespace lti {
  // --------------------------------------------------
  // secondOrderStatistics::parameters
  // --------------------------------------------------

  // default constructor
  template <typename T, typename U>
  secondOrderStatistics<T,U>::parameters::parameters()
        : functor::parameters() {
        dataRepresentationType = Rows;
        varianceType=Empirical;
        useCorrelation=false;
  }

  // copy constructor
  template <typename T, typename U>
  secondOrderStatistics<T,U>::parameters::parameters(const parameters& other) 
    : functor::parameters() {
    copy(other);
  }

  // destructor
  template <typename T, typename U>
  secondOrderStatistics<T,U>::parameters::~parameters() {
  }

  // copy
  template <typename T, typename U>
  typename secondOrderStatistics<T,U>::parameters&
  secondOrderStatistics<T,U>::parameters::copy(const parameters& other) {
    functor::parameters::copy(other);

    dataRepresentationType=other.dataRepresentationType;
    varianceType=other.varianceType;
    useCorrelation=other.useCorrelation;
    
    return *this;
  }

  /**
   * Copy the contents of a parameters object
   */
  template<typename T, typename U>
  typename secondOrderStatistics<T,U>::parameters& 
  secondOrderStatistics<T,U>::parameters::operator=(const parameters& other) {
    return copy(other);
  }
  
  /**
   * Returns the name of this class.
   */
  template<typename T, typename U>
  const std::string& secondOrderStatistics<T,U>::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  //write
  template <typename T, typename U>
  bool 
  secondOrderStatistics<T,U>::parameters::write(ioHandler& handler,
                                              const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }
    
    if (b) {
      
      b = b && lti::write(handler, dataRepresentationType);
      /*
      switch (dataRepresentationType) {
        case Rows:
          lti::write(handler,"dataRepresentationType","Rows");
          break;
        case Columns:
          lti::write(handler,"dataRepresentationType","Columns");
          break;
        default:
          lti::write(handler,"dataRepresentationType","Unknown");
          b=false;
          break;
      }
      */

      switch (varianceType) {
        case Empirical:
          lti::write(handler,"varianceType","Empirical");
          break;
        case Maxlikely:
          lti::write(handler,"varianceType","Maxlikely");
          break;
        default:
          lti::write(handler,"varianceType","Unknown");
          b=false;
          break;
      }

      lti::write(handler,"useCorrelation",useCorrelation);
      
    }
        
    b = b && functor::parameters::write(handler,false);
    
    if (complete) {
      b = b && handler.writeEnd();
    }
    
    return b;
  }
  
  //read
  template <typename T, typename U>
  bool 
  secondOrderStatistics<T,U>::parameters::read(ioHandler& handler,
                                             const bool complete) {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }
    
    if (b) {
      std::string str;
      
      b = b && lti::read(handler, dataRepresentationType);
      
      /*
      lti::read(handler,"dataRepresentationType", str);
      if ((str=="Rows") || (str=="rows")) {
        dataRepresentationType=Rows;
      } else if ((str=="Columns") || (str=="columns")) {
        dataRepresentationType=Columns;
      } else {
        dataRepresentationType=Rows; //default
        handler.setStatusString("Unknown dataRepresentationType ");
        handler.appendStatusString(str);
        handler.appendStatusString(" in secondOrderStatistics\n");
        b=false;
      }
      */        
  
      lti::read(handler,"varianceType",str);
      if ((str=="Empirical") || (str=="empirical")) {
        varianceType = Empirical;
      } else if ((str=="Maxlikely") 
                 || (str=="maxlikely") 
                 || (str=="MaxLikely")) {
        varianceType = Maxlikely;
      } else {
        varianceType = Empirical; //default
        handler.setStatusString("Unknown varianceType ");
        handler.appendStatusString(str);
        handler.appendStatusString(" in secondOrderStatistics\n");
        b=false;
      }
          
      lti::read(handler,"useCorrelation",useCorrelation);
    }

    b = b && functor::parameters::read(handler,false);
        
    if (complete) {
      b = b && handler.readEnd();
    }
    
    return b;
  }

  // clone
  template <typename T, typename U>
  typename secondOrderStatistics<T,U>::parameters*
  secondOrderStatistics<T,U>::parameters::clone() const {
    return new parameters(*this);
  }

  // newInstance
  template <typename T, typename U>
  typename secondOrderStatistics<T,U>::parameters*
  secondOrderStatistics<T,U>::parameters::newInstance() const {
    return new parameters();
  }

  // --------------------------------------------------
  // secondOrderStatistics
  // --------------------------------------------------

  // default constructor
  template <typename T, typename U>
  secondOrderStatistics<T,U>::secondOrderStatistics(eDataRepresentationType t)
    : functor() {
    parameters tmp;
    tmp.dataRepresentationType=t;
    setParameters(tmp);
  }

  // copy constructor
  template <typename T, typename U>
  secondOrderStatistics<T,U>::secondOrderStatistics(const secondOrderStatistics& other)
    : functor() {
    copy(other);
  }

  // constructor
  template <typename T, typename U>
  secondOrderStatistics<T,U>::secondOrderStatistics(const parameters& param)
    : functor() {
    setParameters(param);
  }

  // destructor
  template <typename T, typename U>
  secondOrderStatistics<T,U>::~secondOrderStatistics() {
  }

  // copy
  template <typename T, typename U>
  secondOrderStatistics<T,U>&
  secondOrderStatistics<T,U>::copy(const secondOrderStatistics<T,U>& other) {
    functor::copy(other);
    return (*this);
  }

  /*
   * Alias for copy
   */
  template <typename T, typename U>
  secondOrderStatistics<T,U>&
  secondOrderStatistics<T,U>::operator=(const secondOrderStatistics<T,U>& other) {
    return (copy(other));
  }
  
  /* 
   * Class name
   */
  template <typename T, typename U>
  const std::string& secondOrderStatistics<T,U>::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone 
  template <typename T, typename U>
  secondOrderStatistics<T,U>* secondOrderStatistics<T,U>::clone() const {
    return new secondOrderStatistics<T,U>(*this);
  }

  // newInstance 
  template <typename T, typename U>
  secondOrderStatistics<T,U>* 
  secondOrderStatistics<T,U>::newInstance() const {
    return new secondOrderStatistics<T,U>();
  }

  // return parameters
  template <typename T, typename U>
  const typename secondOrderStatistics<T,U>::parameters&
  secondOrderStatistics<T,U>::getParameters() const {
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

  // On copy apply for type vector<T,U>!
  template <typename T, typename U>
  bool secondOrderStatistics<T,U>::apply(const vector<T>& src, 
                                         U& mu, U& var) const {
    const int n=src.size();

    // empty src?
    if (n==0) {
      mu=U(0);
      var=U(0);
      setStatusString("empty src vector\n");
      return false;
    }

    mu=static_cast<U>(src.at(0));
    var=mu*mu;

    // one element? -> shortcut
    if (n==1) {
      var=U(0);
      return true;
    }

    typename vector<T>::const_iterator it=src.begin();
    it++;
    const typename vector<T>::const_iterator ite=src.end();
    
    for (; it!=ite; ++it) {
      const U val=static_cast<U>(*it);
      mu+=val;
      var+=val*val;
    }
    var-=(mu*mu/static_cast<U>(n));
    mu/=static_cast<U>(n);

    if (getParameters().varianceType == Empirical) {
      var/=n-1;
    } else {
      var/=n;
    }

    return true;
  }

  // On copy apply for type matrix<T,U>!
  template <typename T, typename U>
  bool secondOrderStatistics<T,U>::apply(const matrix<T>& src, 
                                         U& mu, U& var) const {

    const ipoint& size = src.size(); 
    const int n = size.x * size.y;
    mu = U(0);
    var = U(0);
    
    // empty src?
    if (n == 0) {
      setStatusString("empty src matrix\n");
      return false;
    }
    
    for(int i = 0; i < size.y; ++i){
      for(int j = 0; j < size.x; ++j){
        const U val = static_cast<U>(src.at(i,j));
        mu  += val;
        var += val * val;
      }
    }
    var -= (mu * mu / n);
    mu /= n;
    
    if (getParameters().varianceType == Empirical) {
      var /= n - 1;
    } else {
      var /= n;
    }
    
    return true;
  }
  
  // On copy apply for type matrix<T,U>!
  template <typename T, typename U>
  bool secondOrderStatistics<T,U>::apply(const matrix<T>& src,
                                         vector<U>& mu,
                                         vector<U>& var) const {

    if (getParameters().dataRepresentationType==Rows) {
      return applyRows(src,mu,var);
    } else {
      return applyColumns(src,mu,var);
    }

  }

  // On copy apply for type matrix<T,U>!
  template <typename T, typename U>
  bool secondOrderStatistics<T,U>::apply(const matrix<T>& src,
                                         vector<U>& mu,
                                         matrix<U>& covar) const {
    
    if (getParameters().dataRepresentationType==Rows) {
      return applyRows(src,mu,covar);
    } else {
      return applyColumns(src,mu,covar);
    }

  }

  // -------------------------------------------------------------------
  // shortcuts
  // -------------------------------------------------------------------

  template <typename T, typename U>
  bool secondOrderStatistics<T,U>::mean(const vector<T>& src, 
                                        U& m) const {
    const int sz=src.size();
    m=U(0);
    if (sz==0) {
      setStatusString("empty src vector\n");
      return false;
    }

    //don't use computeSumOfElement e.g. <ubyte,float> results in wrong result
    for (int i=0; i<sz; i++) {
      m += static_cast<U>(src.at(i));
    }
    m /= sz;

    return true;
  }

  template <typename T, typename U>
  bool secondOrderStatistics<T,U>::mean(const matrix<T>& src, 
                                        vector<U>& m,
                                        eDataRepresentationType rep) const {
    
    if (rep==Rows) {
      const int rows=src.rows();
      if (rows==0) {
        m.assign(src.columns(),U(0));
        setStatusString("empty src matrix\n");
        return false;
      }
      
      //init with first row
      m.castFrom(src.getRow(0));

      const int cols = src.columns();
      
      for (int i=1; i<rows; i++) {
        const vector<T>& vec=src.getRow(i);
        for (int j=0; j<cols; j++) {
          m.at(j)+=static_cast<U>(vec.at(j));
        }
      }
      m.divide(static_cast<U>(rows));
      
      return true;
    } 

    else if (rep==Columns) {

      const int cols=src.columns();

      if (cols==0) {
        m.assign(src.rows(),U(0));
        setStatusString("empty src matrix\n");
        return false;
      }

      const int rows=src.rows();
      m.allocate(rows);
      const double fac = 1./cols;
      for (int i=0; i<rows; i++) {
        m.at(i)=static_cast<U>(src.getRow(i).computeSumOfElements()*fac);
      }
      return true;
      
    }

    else {  //error
      setStatusString("Unknown eDataRepresentationType value in secondOrderStatistics");
      return false;
    }
  }

  ////////
  // almost no overhead in using applies for variance() functions

  template <typename T, typename U>
  bool secondOrderStatistics<T,U>::variance(const vector<T>& src, 
                                            U& var) const {
    U mu;
    return apply(src,mu,var);
  }

  template <typename T, typename U>
  bool secondOrderStatistics<T,U>::variance(const matrix<T>& src, 
                                            vector<U>& var,
                                            eDataRepresentationType rep) const {

    vector<U> mu;
    if (rep==Rows) {
      return applyRows(src,mu,var);
    } else if (rep==Columns) {
      return applyColumns(src,mu,var);
    } else {
      setStatusString("Unknown eDataRepresentationType value in secondOrderStatistics");
      return false;
    }
      
  }

  template <typename T, typename U>
  bool secondOrderStatistics<T,U>::covarianceMatrix(const matrix<T>& src, 
                                                    matrix<U>& cov,
                                                    eDataRepresentationType rep) const {

    vector<U> mu;
    if (rep==Rows) {
      return applyRows(src,mu,cov);
    } else if (rep==Columns) {
      return applyColumns(src,mu,cov);
    } else {
      setStatusString("Unknown eDataRepresentationType value in secondOrderStatistics");
      return false;
    }
      
  }
  

  // -------------------------------------------------------------------
  // helper functions
  // -------------------------------------------------------------------
  
  template <typename T, typename U>
  bool secondOrderStatistics<T,U>::applyRows(const matrix<T>& src,
                                             vector<U>& mu,
                                             vector<U>& var) const {

    const int rows=src.rows();

    // empty matrix?
    if (rows==0) {
      mu.clear();
      var.clear();
      setStatusString("empty src matrix");
      return false;
    }

    const int cols=src.columns();

    // init vectors
    mu.castFrom(src.getRow(0));

    // only one sample?
    if (rows==1) {
      var.assign(cols,U(0));
      return true;
    }

    var.apply(mu,sqr); // compute the square of each element of mu

    // compute the variance of the rows of src.
    // var=1/k \sum_i (x_i-\mu)^2
    // this is equal to 1/k[ (\sum_i x_i^2) - s^2/n ]
    // where s = \sum_i x_i and n = number of rows
    // k is either n or n-1, depending on the type of variance
    for (int i=1; i<rows; i++) {
      for (int j=0; j<cols; j++) {
        const U val=static_cast<U>(src.at(i,j));
        mu.at(j)+=val;
        var.at(j)+=(val*val);
      }
    }

    // subtract s^2/n
    const U div = static_cast<U>(rows);
    for (int j=0; j<cols; ++j) {
      U& s=mu.at(j);
      var.at(j)-=(s*s/div);
      s/=div; // convert s to mu
    }

    // normalize
    if (getParameters().varianceType == Empirical) {
      var.divide(static_cast<U>(rows-1));
    } else {
      var.divide(static_cast<U>(rows));
    }

    return true;
  }

  template <typename T, typename U>
  bool secondOrderStatistics<T,U>::applyColumns(const matrix<T>& src,
                                                vector<U>& mu,
                                                vector<U>& var) const {

    const int cols=src.columns();

    // empty matrix?
    if (cols==0) {
      mu.clear();
      var.clear();
      setStatusString("empty src matrix");
      return false;
    }

    const int rows=src.rows();

    // init vectors
    mu.allocate(rows);
    for(int i = 0; i < rows; ++i){
      mu.at(i) = static_cast<U>(src.at(i,0));
    }

    // only one sample?
    if (cols==1) {
      var.assign(rows,U(0));
      return true;
    }

    // assign the square of each element of mu to var
    var.apply(mu,sqr);

    // compute the variance of the rows of src.
    // var=1/k \sum_i (x_i-\mu)^2
    // this is equal to 1/k \sum_i x_i^2 - s^2/n
    // where s = \sum_i x_i and n = number of rows
    // k is either n or n-1, depending on the type of variance
    for (int i=0; i<rows; i++) {
      U& m=mu.at(i);
      U& v=var.at(i);
      const vector<T>& vec=src.getRow(i);
      for (int j=1; j<cols; j++) {
        const U val=static_cast<U>(vec.at(j));
        m+=val;
        v+=val*val;
      }
    }

    // subtract s^2/n and normalize mu
    const U div = static_cast<U>(cols);
    for (int i=0; i<rows; i++) {
      U& s=mu.at(i);
      var.at(i)-=s*s/div;
      s/=div; // convert s to mu
    }

    // normalize
    if (getParameters().varianceType == Empirical) {
      var.divide(static_cast<U>(cols-1));
    } else {
      var.divide(static_cast<U>(cols));
    }

    return true;
  }

  template<typename T, typename U>
  bool secondOrderStatistics<T,U>::applyRows(const matrix<T>& src,
                                             vector<U>& mu,
                                             matrix<U>& cov) const {

    const int rows=src.rows();

    // empty matrix?
    if (rows==0) {
      mu.clear();
      cov.clear();
      setStatusString("empty src matrix");
      return false;
    }

    const int cols=src.columns();

    // only one sample?
    if (rows==1) {
      mu.castFrom(src.getRow(0));
      cov.assign(rows,rows,U(0));
      return true;
    }

    mean(src,mu,Rows);
    matrix<U> tmp;
    tmp.transpose(src);

    // to normalize cov matrix
    U div = (getParameters().varianceType == Empirical) ?
      static_cast<U>(rows-1) : static_cast<U>(rows);

    // first loop computes mu (well, the accumulation of the data) and
    // the sum of the outer products in an alternate faster way
    cov.allocate(cols,cols);
    for (int i=0;i<cols;++i) {
      tmp.getRow(i).subtract(mu.at(i)); // the i-th row, all elements - mu[i]
      const vector<U>& drow=tmp.getRow(i); // row of the transposed data
      vector<U>& crowi=cov.getRow(i);
      crowi.at(i)=drow.dot(drow)/div;
      for (int j=0;j<i;++j) {
        cov.at(j,i)=crowi.at(j)=drow.dot(tmp.getRow(j))/div;
      }
    }
    
    // convert to correlation if set
    if (getParameters().useCorrelation) {
      covar2corrcoef(cov);
    }
    
    return true;
  }

  template <typename T, typename U>
  bool secondOrderStatistics<T,U>::applyColumns(const matrix<T>& src,
                                                vector<U>& mu, 
                                                matrix<U>& cov) const {

    const int cols=src.columns();

    // empty matrix?
    if (cols==0) {
      mu.clear();
      cov.clear();
      setStatusString("empty src matrix");
      return false;
    }

    const int rows=src.rows();

    // only one sample?
    if (cols==1) {
      mu.allocate(rows);
      for(int i = 0; i<rows; ++i){
        mu.at(i) = static_cast<U>(src.at(i,0));
      }
      
      cov.assign(rows,rows,U(0));
      return true;
    }

    // to normalize cov matrix
    U div  = (getParameters().varianceType == Empirical) ?
      static_cast<U>(rows-1) : static_cast<U>(rows);

    mean(src,mu,Columns);
    matrix<U> tmp;
    tmp.castFrom(src); // calls copy if U==T
    
    cov.allocate(rows,rows);
    for (int i=0; i<rows; i++) {
      tmp.getRow(i).subtract(mu.at(i)); // the i-th row, all elements - mu[i]
      const vector<U>& drow=tmp.getRow(i);
      vector<U>& crowi=cov.getRow(i);
      crowi.at(i)=drow.dot(drow)/div;
      for (int j=0; j<i; j++) {
        cov.at(j,i)=crowi.at(j)=drow.dot(tmp.getRow(j))/div;
      }
    }


    // convert to correlation if set
    if (getParameters().useCorrelation) {
      covar2corrcoef(cov);
    }
    
    return true;

  }

  template <typename T, typename U>
  void secondOrderStatistics<T,U>::covar2corrcoef(matrix<U> &cv) const {
    int row,column;
    vector<U> diag;
    cv.getDiagonal(diag);
    diag.apply(sqrt);

    const U eps(std::numeric_limits<U>::epsilon());
    const U zero(0);

    typename matrix<U>::iterator k=cv.begin();
    row=0;
    column=0;
    const int rows=cv.rows();
    const int cols=cv.columns();
    for (row=0; row<rows; row++) {
      const U drow=diag.at(row);
      if (drow<eps) {
        for (column=0; column<cols; column++, ++k) {
          (*k)=zero;
        }
      } else {
        for (column=0; column<cols; column++, ++k) {
          const U dcol=diag.at(column);
          if (dcol<eps) {
            (*k)=zero;
          } else {
            (*k)/=drow*dcol;
          }
        }
      }
    }
  }



//   template <typename T, typename U>
//   T secondOrderStatistics<T,U>::covariance(const vector<T,U>& a,
//                                    const vector<T,U>& b) const {
//     typename vector<T,U>::const_iterator ia=a.begin();
//     typename vector<T,U>::const_iterator ib=b.begin();

//     assert(a.size() == b.size());
//     T n=T(a.size());
//     T sx=T(0);
//     T sy=T(0);

//     T sxy=T(0);
//     for ( ; ia != a.end(); ++ia, ++ib) {
//       sxy+=(*ia)*(*ib);
//       sx+=*ia;
//       sy+=*ib;
//     }
//     sxy-=sx*sy/n;
//     if (getParameters().type == parameters::Empirical) {
//       return sxy/(n-1);
//     } else {
//       return sxy/n;
//     }

//   }
}


