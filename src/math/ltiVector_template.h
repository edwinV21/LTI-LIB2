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
 * \file   ltiVector_template.h
 * \author Pablo Alvarado
 * \date   09.04.1999
 *
 * $Id: ltiVector_template.h,v 1.13 2012-11-17 14:49:05 alvarado Exp $
 */

namespace lti {
  // implementation of template functions

  // -------------------------------------------------------------------
  // vector
  // -------------------------------------------------------------------
  // default constructor
  template<class T>
  vector<T>::vector() : genericVector<T>() {
  }

  template<class T>
  vector<T>::vector(const int theSize)
    : genericVector<T>(theSize) {
  }

  template<class T>
  vector<T>::vector(const int theSize,const T& iniValue)
    : genericVector<T>(theSize,iniValue) {
  }

  template<class T>
  vector<T>::vector(const int theSize,const T* _data)
    : genericVector<T>(theSize,_data) {
  }

  template<class T>
  vector<T>::vector(const int theSize,
                          T* _data,
                    const eConstantReference constRef) 
    : genericVector<T>(theSize,_data,constRef) {
  }

  // copy constructor
  template<class T>
  vector<T>::vector(const genericVector<T>& other)
    : genericVector<T>(other) {
  }

  // copy constructor
  template<class T>
  vector<T>::vector(const genericVector<T>& other,
                    const int from,
                    const int to)
    : genericVector<T>(other,from,to) {
  }

  // copy constructor
  template<class T>
  vector<T>::vector(const genericVector<T>& other,
                    const genericVector<int>& idx)
    : genericVector<T>(other,idx) {
  }

  // pseudo-copy constructor
  template<class T>
  vector<T>::vector(const std::vector<T>& other)
    : genericVector<T>(other) {
  }

  template<class T>
  vector<T>::~vector() {
  }

  // class name
  template<class T>
  const std::string& vector<T>::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // creates a clone of this vector
  template<class T>
  vector<T>* vector<T>::clone() const {
    return new vector<T>(*this);
  }

  template<class T>
  vector<T>* vector<T>::newInstance() const {
    return new vector<T>();
  }

  /* compare this vector with other, and use the given tolerance to
     determine if the value of each element of the other vector
     approximately equals the values of the actual vector elements.

     An element <i>x</i> is aproximatly equal to another element <i>y</i>
     with a tolerance <i>t</i>, if following equation holds:
     <i>x</i>-t < <i>y</i> < <i>x</i>+t

     @param other the other vector to be compared with
     @param tolerance the tolerance to be used

     @return true if both vectors have approximatly equal
  */
  template<class T>
  bool vector<T>::prettyCloseTo(const genericVector<T>& other,
                                const T& tolerance) const {
    bool result;
    
    result = (this->size()==other.size());

    if (result) {
      const_iterator i,e;
      const_iterator o;

      for (i=this->begin(),e=this->end(),o=other.begin();
           result && (i != e);
           ++i,++o) {
        result = result &&
          ((((*i)-tolerance) < (*o)) && ((*o) < ((*i)+tolerance)));
      }
    }

    return result;
  }

  // Arithmetical operations
  template<class T>
  T vector<T>::dot(const genericVector<T>& other) const {
    assert(this->size() == other.size());
    T temp = T(0);

    for (int i=0; i<this->vectorSize_; i++) {
      temp+=conj(this->theElements_[i])*other.at(i);
    }

    return temp;
  }

  // Elementwise multiplication
  template<class T>
  vector<T>& vector<T>::emultiply(const genericVector<T>& other) {
    assert(this->size() == other.size());

    iterator i,e;
    const_iterator o;

    e = this->end();
    for (i=this->begin(),o=other.begin();i!=e;++i,++o)
      (*i)*=(*o);

    return *this;
  }

  // Elementwise multiplication
  template<class T>
  vector<T>& vector<T>::emultiply(const genericVector<T>& first,
                                  const genericVector<T>& second) {
    assert(first.size() == second.size());

    iterator i,e;
    const_iterator a,b;

    this->allocate(first.size());

    for (i=this->begin(),e=this->end(),a=first.begin(),b=second.begin();
         i!=e;++i,++a,++b)
      (*i)=(*a)*(*b);

    return *this;
  }

  // Elementwise division
  template<class T>
  vector<T>& vector<T>::edivide(const genericVector<T>& other) {
    assert(this->size() == other.size());

    iterator i,e;
    const_iterator o;

    e = this->end();
    for (i=this->begin(),o=other.begin();i!=e;++i,++o)
      (*i)/=(*o);

    return *this;
  }

  // Elementwise division
  template<class T>
  vector<T>& vector<T>::edivide(const genericVector<T>& first,
                                const genericVector<T>& second) {
    assert(first.size() == second.size());

    iterator i,e;
    const_iterator a,b;

    this->allocate(first.size());

    for (i=this->begin(),e=this->end(),a=first.begin(),b=second.begin();
         i!=e;++i,++a,++b)
      (*i)=(*a)/(*b);

    return *this;
  }

  // add another scaled vector
  template<class T>
  vector<T>& vector<T>::addScaled(const T a, const genericVector<T>& other) {
    assert(this->size() == other.size());

    iterator i,e;
    const_iterator o;

    e = this->end();
    for (i=this->begin(),o=other.begin();i!=e;++i,++o)
      (*i)+=a*(*o);

    return *this;
  }

  // add two scaled vectors
  template<class T>
  vector<T>& vector<T>::addScaled(const T sf, const genericVector<T>& first,
                                  const T ss, const genericVector<T>& second) {
    assert(first.size() == second.size());

    iterator i,e;
    const_iterator a,b;

    this->allocate(first.size());

    for (i=this->begin(),e=this->end(),a=first.begin(),b=second.begin();
         i!=e;++i,++a,++b)
      (*i)=sf*(*a)+ss*(*b);

    return *this;
  }


  // add two scaled vectors
  template<class T>
  vector<T>& vector<T>::addScaled(const genericVector<T>& first,
                                  const T ss, const genericVector<T>& second) {

    assert(first.size() == second.size());

    iterator i,e;
    const_iterator a,b;

    this->allocate(first.size());

    for (i=this->begin(),e=this->end(),a=first.begin(),b=second.begin();
         i!=e;++i,++a,++b)
      (*i)=(*a)+ss*(*b);

    return *this;
  }

  // calculate the sum of all elements
  template<class T>
  T vector<T>::computeSumOfElements() const {
    T sum;
    sum = T(0); // assume default constructor for numeric types is 0!

    const_iterator i,e;

    for (i=this->begin(),e=this->end(); i!=e; ++i) {
      sum+=(*i);
    }

    return sum;
  }

  // calculate the sum of all elements
  template<class T>
  T vector<T>::computeProductOfElements() const {
    // assume all types have some sort of 1-element
    T prod = T(1);

    const_iterator i,e;

    for (i=this->begin(),e=this->end(); i!=e; ++i) {
      prod*=(*i);
    }

    return prod;
  }


  // add another vector (alias)
  template<class T>
  vector<T>& vector<T>::operator+=(const genericVector<T>& other) {
    return add(other);
  }

  // subtract a constandt from this vector
  template<class T>
  vector<T>& vector<T>::subtract(const T cst) {
    iterator i,e;

    e = this->end();
    for (i=this->begin();i!=e;++i)
      (*i)-=cst;

    return *this;
  }

  // subtract a constant from a vector
  template<class T>
  vector<T>& vector<T>::subtract(const genericVector<T>& other, const T cst) {
    iterator i,e;
    const_iterator o;
    this->allocate(other.size());

    e = this->end();
    for (i=this->begin(),o=other.begin();
         i!=e;++i,++o)
      (*i)=(*o)-cst;

    return *this;
  }

  // subtract another vector
  template<class T>
  vector<T>& vector<T>::subtract(const genericVector<T>& other) {
    assert(this->size() == other.size());

    iterator i,e;
    const_iterator o;

    e = this->end();
    for (i=this->begin(),o=other.begin();i!=e;++i,++o)
      (*i)-=(*o);

    return *this;
  }

  // subtract two vectors
  template<class T>
  vector<T>& vector<T>::subtract(const genericVector<T>& first,
                                 const genericVector<T>& second) {
    assert(first.size() == second.size());

    iterator i,e;
    const_iterator a,b;

    this->allocate(first.size());

    for (i=this->begin(),e=this->end(),a=first.begin(),b=second.begin();
         i!=e;++i,++a,++b)
      (*i)=(*a)-(*b);

    return *this;
  }

  // subtract another vector
  template<class T>
  vector<T>& vector<T>::operator-=(const genericVector<T>& other) {
    return subtract(other);
  }

  // subtract scalar
  template<class T>
  vector<T>& vector<T>::operator-=(const T cst) {
    return subtract(cst);
  }

  // multiply vector with a constant.
  template<class T>
  vector<T>& vector<T>::multiply(const T cst) {
//     iterator i,e;

//     e = this->end();
//     for (i=this->begin();i!=e;++i)
//       (*i)*=cst;

    for (int i=0; i<this->vectorSize_; i++) {
      this->theElements_[i]*=cst;
    }

    return *this;
  }

  // multiply vector with another vector.
  template<class T>
  vector<T>& vector<T>::multiply(const genericVector<T>& other, const T cst) {
    iterator i,e;
    const_iterator o;
    this->allocate(other.size());

    e = this->end();
    for (i=this->begin(),o=other.begin();
         i!=e;++i,++o)
      (*i)=(*o)*cst;

    return *this;
  }

  // add vector with a constant.
  template<class T>
  vector<T>& vector<T>::add(const T cst) {
//     iterator i,e;

//     e = this->end();
//     for (i=this->begin();i!=e;++i)
//       (*i)+=cst;

    for (int i=0; i<this->vectorSize_; i++) {
      this->theElements_[i]+=cst;
    }

    return *this;
  }

  // add a constant to a vector
  template<class T>
  vector<T>& vector<T>::add(const genericVector<T>& other, const T cst) {
    iterator i,e;
    const_iterator o;
    this->allocate(other.size());

    e = this->end();
    for (i=this->begin(),o=other.begin();
         i!=e;++i,++o)
      (*i)=(*o)+cst;

    return *this;
  }

  // add another vector
  template<class T>
  vector<T>& vector<T>::operator+=(const T cst) {
    return add(cst);
  }

  // divide vector by a constant.
  template<class T>
  vector<T>& vector<T>::divide(const T cst) {
    iterator i,e;

    e = this->end();
    for (i=this->begin();i!=e;++i)
      (*i)/=cst;

    return *this;
  }

  // divide vector by a constant.
  template<class T>
  vector<T>& vector<T>::divide(const genericVector<T>& other, const T cst) {
    iterator i,e;
    const_iterator o;
    this->allocate(other.size());

    e = this->end();
    for (i=this->begin(),o=other.begin();
         i!=e;++i,++o)
      (*i)=(*o)/cst;

    return *this;
  }

  // divide by scalar
  template<class T>
  vector<T>& vector<T>::operator/=(const T cst) {
    return divide(cst);
  }

  // multiply vector with a constant (alias).
  template<class T>
  vector<T>& vector<T>::operator*=(const T cst) {
    return multiply(cst);
  }

  // minimum
  template<class T>
  T vector<T>::findMinimum() const {

    const_iterator i,e;
    i = this->begin();

    T theMinimum;

    if (this->size()>0) {
      theMinimum = *i;
    } else {
      return T(); // return something unknown
    }

    ++i;

    for (e = this->end();i!=e;++i) {
      theMinimum = min(theMinimum,*i);
    }

    return theMinimum;
  }

  // minimum
  template<class T>
  int vector<T>::findIndexOfMinimum() const {

    int i;
    int idxOfMin;
    T theMinimum;

    if (this->size()>1) {
      theMinimum = this->at(0);
      idxOfMin = 0;
    } else {
      return 0; // return something unknown
    }

    for (i=1;i<this->size();++i) {
      if (this->at(i) < theMinimum) {
        idxOfMin = i;
        theMinimum = this->at(i);
      }
    }

    return idxOfMin;
  }

  // maximum
  template<class T>
  T vector<T>::findMaximum() const {
    const_iterator i,e;
    i = this->begin();

    T theMaximum;

    if (this->size()>0) {
      theMaximum = *i;
    } else {
      return T(); // return something unknown
    }

    ++i;

    for (e = this->end();i!=e;++i) {
      theMaximum = max(theMaximum,*i);
    }

    return theMaximum;
  }

  // idx of maximum
  template<class T>
  int vector<T>::findIndexOfMaximum() const {

    int i;
    int idxOfMax;
    T theMaximum;

    if (this->size()>1) {
      theMaximum = this->at(0);
      idxOfMax = 0;
    } else {
      return 0; // return something unknown
    }

    for (i=1;i<this->size();++i) {
      if (theMaximum < this->at(i)) {
        idxOfMax = i;
        theMaximum = this->at(i);
      }
    }

    return idxOfMax;
  }

  // find extremes maximum
  template<class T>
  void vector<T>::findExtremes(T& theMinimum,T& theMaximum) const {
    if (this->empty()) {
      return; // nothing to do, leave everything untouched!
    }

    const_iterator i = this->begin();
    const_iterator e = this->end();

    theMinimum = theMaximum = *i;

    // be sure that only "<" is used!

    for (++i;i!=e;++i) {
      if ((*i) < theMinimum) {
        theMinimum = (*i);
      } else if (theMaximum < (*i)) {
        theMaximum = (*i);
      }
    }
  }

  // idx of maximum
  template<class T>
  void vector<T>::findIndexOfExtremes(int& idxOfMin,int& idxOfMax) const {

    if (this->empty()) {
      return;
    }

    int i;
    T theMaximum;
    T theMinimum;
    T tmp;

    theMinimum = theMaximum = this->at(0);
    idxOfMin = idxOfMax = 0;

    for (i=1;i<this->size();++i) {
      tmp = this->at(i);

      // be sure that only "<" is used!

      if (theMaximum < tmp) {
        idxOfMax = i;
        theMaximum = tmp;
      } else if (tmp < theMinimum) {
        idxOfMin = i;
        theMinimum = tmp;
      }
    }
  }


} // namespace lti
