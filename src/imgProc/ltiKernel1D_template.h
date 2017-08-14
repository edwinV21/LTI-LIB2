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
 * \file   ltiKernel1D_template.h
 *         Contains the template class lti::kernel1D<T>.
 * \author Pablo Alvarado
 * \date   28.04.00
 *
 * revisions ..: $Id: ltiKernel1D_template.h,v 1.10 2010-04-24 23:54:28 alvarado Exp $
 */

namespace lti {
  // --------------------------------------------------
  // One Dimensional Kernel
  // --------------------------------------------------

  // constructor
  template<typename T>
  kernel1D<T>::kernel1D() : array<T>() {
    norm_ = typeInfo<T>::suggestedNorm();
  }

  // constructor
  template<typename T>
  kernel1D<T>::kernel1D(const int from,
                        const int to,
                        const T& init)
    : array<T>(from,to,init) {
    norm_ = typeInfo<T>::suggestedNorm();
  }

  // constructor
  template<typename T>
  kernel1D<T>::kernel1D(const vector<T>& other,
                        const int theOffset)
    : array<T>(other,theOffset) {
    norm_ = typeInfo<T>::suggestedNorm();
  }

  // copy constructor
  template<typename T>
  kernel1D<T>::kernel1D(const kernel1D<T>& other) : array<T>() {
    copy(other);
  }

  template<typename T>
  kernel1D<T>::kernel1D(const int from, const int to)
    : array<T>(from,to) {
    norm_ = typeInfo<T>::suggestedNorm();
  }

  // destructor
  template<typename T>
  kernel1D<T>::~kernel1D() {
  }

  // creates a clone of this object
  template<typename T>
  const std::string& kernel1D<T>::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // creates a clone of this object
  template<typename T>
  kernel1D<T>* kernel1D<T>::clone() const {
    return new kernel1D<T>(*this);
  }

  // creates a clone of this object
  template<typename T>
  kernel1D<T>* kernel1D<T>::newInstance() const {
    return new kernel1D<T>();
  }

  // copy
  template<typename T>
  kernel1D<T>& kernel1D<T>::copy(const kernel1D<T>& other) {
    array<T>::copy(other);
    norm_=other.getNorm();
    return (*this);
  }

  // get Offset
  template<typename T>
  int kernel1D<T>::getOffset() const {
    return (-array<T>::firstIdx());
  }

  template<typename T>
  void kernel1D<T>::setOffset(const int theOffset) {
    this->setFirstIdx(-theOffset);
  }

  // denormalize
  template<typename T>
  void kernel1D<T>::denormalize() {
    if (norm_ == T(1)) {
      return; // nothing to be done (already normalized!)
    }

    this->divide(norm_);
    norm_ = T(1);
  }

  /*
   * copy the content of the other vector in this kernel and assign
   * the index (firstElement) to the first element of the vector.
   * For example if <code>other</code> is a 3D vector, then
   * <code>castFrom(other,-1)</code> is a 3-elements-kernel which indices
   * lay inside [-1,1].
   * @param other the vector with the data to be copied
   * @param firstElement index for the first element of the vector
   * @return a reference to this instance
   */
  template<typename T>
  kernel1D<T>& kernel1D<T>::castFrom(const vector<T>& other,
                                     const int firstElement) {
    vector<T>::copy(other);
    setOffset(-firstElement);
    return (*this);
  }

  // mirror other kernel
  template <typename T>
  kernel1D<T>& kernel1D<T>::mirror(const kernel1D<T>& other) {

    vector<T>& vct = (*this); // This kernel but as vector
    const vector<T>& ovct = other; // The other kernel, but as vector

    vct.allocate(ovct.size());

    const int last = vct.size()-1;
    typename vector<T>::const_iterator cit,eit;
    T* ptr;

    for (cit=ovct.begin(),eit=ovct.end(),ptr=&vct.at(last);
         cit!=eit;
         ++cit,--ptr) {
      (*ptr)=(*cit);
    }

    setOffset(last-other.getOffset());

    norm_ = other.getNorm();

    return (*this);
  }

  // mirror
  template <typename T>
  kernel1D<T>& kernel1D<T>::mirror() {
    kernel1D<T> tmp;
    tmp.mirror(*this);
    setOffset(tmp.getOffset());
    tmp.detach(*this);
    return (*this);
  }

  template <typename T>
  bool kernel1D<T>::isSymmetric() const {
    if (this->firstIdx() != -this->lastIdx()) {
      // the kernel is not even geometrically symmetric
      return false;
    }

    typename array<T>::const_iterator f = this->begin();
    typename array<T>::const_iterator b = this->inverseBegin();

    // since we are in a kernel, we can assume we have random access iterators
    while ( (f <= b) && ((*f) == (*b))) {
      ++f;
      --b;
    }
    
    return (f > b);
  }


  template <typename T>
  bool kernel1D<T>::isAsymmetric() const {
    if (this->firstIdx() != -this->lastIdx()) {
      // the kernel is not even geometrically symmetric
      return false;
    }

    typename array<T>::const_iterator f = this->begin();
    typename array<T>::const_iterator b = this->inverseBegin();

    // since we are in a kernel, we can assume we have random access iterators
    while ( (f <= b) && ((*f) == -(*b))) {
      ++f;
      --b;
    }
    
    return ((f > b) && (this->at(0) == T(0)));
  }

  /*
   * Get kernel symmetric component
   */
  template <typename T>
  void kernel1D<T>::computeSymmetricComponent() {
    kernel1D<T> tmp(*this);
    computeSymmetricComponent(tmp);
  }
  
  /*
   * Get kernel symmetric component of other kernel.
   */
  template <typename T>
  void kernel1D<T>::computeSymmetricComponent(const kernel1D<T>& other) {
    // if empty, nothing to do
    if (other.empty()) {
      this->clear();
      return;
    }

    typedef typename typeInfo<T>::accumulation_type accType;
    const int maxIdx = max(abs(other.firstIdx()),abs(other.lastIdx()));
    this->allocate(-maxIdx,maxIdx);

    if (other.lastIdx() == -other.firstIdx()) {

      // geometrically symmetric, that is now easy
      this->at(0)=other.at(0);
      for (int i=1;i<other.lastIdx();++i) {
        this->at(i) = this->at(-i) =
          static_cast<T>((static_cast<accType>(other.at(i)) +
                          static_cast<accType>(other.at(-i)))/
                         static_cast<accType>(2));
      } 
    } else {
      // kernel is not symmetric
      for (int i=0;i<this->lastIdx();++i) {
        const int mi = -i;
        accType acc(0);
        if (mi >= other.firstIdx()) {
          acc = other.at(mi);
        }
        if (i <= other.lastIdx()) {
          acc+= other.at(i);
        }
        this->at(i)=this->at(mi)=static_cast<T>(acc/accType(2));
      }
    }
  }

  /*
   * Get kernel asymmetric component
   */
  template <typename T>
  void kernel1D<T>::computeAsymmetricComponent() {
  }

  /*
   * Get kernel asymmetric component of other kernel.
   */
  template <typename T>
  void kernel1D<T>::computeAsymmetricComponent(const kernel1D<T>& other) {
    this->copy(other);
  }


  // -----------------------------------------------------------------
  // Storable interface
  // -----------------------------------------------------------------

  /*
   * write the object in the given ioHandler
   */
  template<typename T>
  bool kernel1D<T>::write(ioHandler& handler,const bool complete) const {
    bool b = true;

    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {
      lti::write(handler,"norm",getNorm());
      array<T>::write(handler,false);
    }

    if (complete) {
      b = b && handler.writeEnd();
    }

    return b;
  }

  /*
   * read the object from the given ioHandler
   */
  template<typename T>
  bool kernel1D<T>::read(ioHandler& handler,const bool complete) {
    T norm;
    bool b = true;

    if (complete) {
      b = handler.readBegin();
    }
    
    if (b) {
      lti::read(handler,"norm",norm);
      array<T>::read(handler,false);
      setNorm(norm);
    }

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

} // namespace
