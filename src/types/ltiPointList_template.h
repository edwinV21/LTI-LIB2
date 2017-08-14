/*
 * Copyright (C) 1998, 1999, 2000, 2001
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
 * \file   ltiPointList_template.h
 *         Defines a class for lists of points
 * \author Suat Akyol
 * \date   28.06.2000
 *
 * $Id: ltiPointList_template.h,v 1.11 2012-11-17 14:49:33 alvarado Exp $
 */

#include "ltiMath.h"

namespace lti {
  // implementation of functions

  // -------------------------------------------------------------------
  // pointList
  // -------------------------------------------------------------------

  // default constructor
  template<class T>
  pointList<T>::pointList() : ioObject(), list<point<T> >(), boundary_() {
  }

  // copy constructor
  template<class T>
  pointList<T>::pointList(const pointList<T>& other) 
    : ioObject(), list<point<T> >(), boundary_() {
    copy(other);
  }

  /*
   * create this pointList as a copy of a list< point<T> > of point<T>
   * @param other the pointList to be copied.
   */
  template<class T>
  pointList<T>::pointList(const list< point<T> >& other)
    : ioObject(), list<point<T> >(), boundary_() {
    castFrom(other);
  }

  // destructor
  template<class T>
  pointList<T>::~pointList(){
    this->clear();
  }

  // compares the size of this list with the size of the other point list
  // and returns true if this list has fewer points than the other one.
  template<class T>
  bool pointList<T>::operator<(const pointList<T>& other) const {
    return this->size()<other.size();
  }

  // compares the size of this list with the size of the other point list
  // and returns true if this list has more points than the other one.
  template<class T>
  bool pointList<T>::operator>(const pointList<T>& other) const {
    return this->size()>other.size();
  }

  // returns first element as a const_iterator.
  // Note that you can not change the values of the pointList
  // elements when you use a const_iterator. See also begin()
//   template<class T>
//   typename pointList<T>::const_iterator pointList<T>::begin() const {
//     return thePointList_.begin();
//   }


  // returns first element as an iterator
//   template<class T>
//   typename pointList<T>::iterator pointList<T>::begin() {
//     return thePointList_.begin();
//   }

  // returns last element as a const iterator.
  // For an example see begin()
//   template<class T>
//   typename pointList<T>::const_iterator pointList<T>::end() const {
//     return thePointList_.end();
//   }

  // returns last element as an iterator
  // For an example see begin()
//   template<class T>
//   typename pointList<T>::iterator pointList<T>::end() {
//     return thePointList_.end();
//   }

  // deletes all points from list and leaves empty pointList.
//   template<class T>
//   void pointList<T>::clear(){
//     thePointList_.clear();
//   }

  // erases point, which is denoted by it. Returns iterator to next element.
//   template<class T>
//   typename pointList<T>::iterator
//   pointList<T>::erase(const typename pointList<T>::iterator& it){
//     return thePointList_.erase(it);
//   }

  // erases points between first and last. Returns iterator to next element.
//   template<class T>
//   typename pointList<T>::iterator
//   pointList<T>::erase(const typename pointList<T>::iterator& first,
//                        const typename pointList<T>::iterator& last){
//     return thePointList_.erase(first,last);
//   }

  // inserts points at position denoted by it.
  // Returns iterator to inserted element.
//   template<class T>
//   typename pointList<T>::iterator
//   pointList<T>::insert(const typename pointList<T>::iterator& it,
//                        const point<T>& thePoint){
//     return list<point<T> >::insert(it,thePoint);
//   }

  // inserts points at position denoted by it.
  // Returns iterator to inserted element.
//   template<class T>
//   typename pointList<T>::iterator
//   pointList<T>::insert(const typename pointList<T>::iterator& it,
//                        const int n,
//                        const point<T>& thePoint) {
//     typename pointList<T>::iterator itTemp=it;
//     itTemp--;
//     list<point<T> >::insert(it,n,thePoint);
//     itTemp++;
//     return itTemp;
//   }

  // inserts the elements between first and last at position denoted by it.
//   template<class T>
//   typename pointList<T>::iterator
//   pointList<T>::insert(const typename pointList<T>::iterator& it,
//                        typename pointList<T>::const_iterator first,
//                        typename pointList<T>::const_iterator last) {
//     typename pointList<T>::iterator itTemp=it;
//     itTemp--;
//     list<point<T> >::insert(it,first,last);
//     itTemp++;
//     return itTemp;
//   }

  // inserts element at begin of pointList
//   template<class T>
//   void pointList<T>::push_front(const point<T>& thePoint){
//     thePointList_.push_front(thePoint);
//   }

  // removes element at begin of pointList
//   template<class T>
//   void pointList<T>::pop_front(){
//     thePointList_.pop_front();
//   }

  // inserts element at end of pointList
//   template<class T>
//   void pointList<T>::push_back(const point<T>& thePoint){
//     thePointList_.push_back(thePoint);
//   }

  // removes element at end of pointList
//   template<class T>
//   void pointList<T>::pop_back(){
//     thePointList_.pop_back();
//   }

  // returns a reference to the first element
//   template<class T>
//   typename pointList<T>::reference pointList<T>::front() {
//     return thePointList_.front();
//   }

  // returns a const reference to the first element
//   template<class T>
//   typename pointList<T>::const_reference pointList<T>::front() const {
//     return thePointList_.front();
//   }

  // returns a reference to the last element
//   template<class T>
//   typename pointList<T>::reference pointList<T>::back() {
//     return thePointList_.back();
//   }

  // returns a const reference to the last element
//   template<class T>
//   typename pointList<T>::const_reference pointList<T>::back() const {
//     return thePointList_.back();
//   }

  // assigment operator.
  // copy the contents of <code>other</code> in this %object.
  // @param other the source pointList to be copied.
  template<class T>
  pointList<T>& pointList<T>::copy(const pointList<T>& other){
    list<point<T> >::operator=(other);
    boundary_.copy(other.getBoundary());
    return *this;
  }


  // assigment operator.
  // copy the contents of <code>other</code> in this %object.
  // @param other the source pointList to be copied.
  template<class T>
  pointList<T>& pointList<T>::operator=(const pointList<T>& other){
    return copy(other);
  }

  // assigment operator.
  // copy the contents of <code>other</code> in this %object.
  // @param other the source pointList to be copied.
  template<class T>
  pointList<T>& pointList<T>::castFrom(const list< point<T> >& other){
    list<point<T> >::operator=(other);
    return *this;
  }

  // compare this pointList with other
  // @param other the other pointList to be compared with
  // @return true if both pointLists have the same elements and same size
  template<class T>
  bool pointList<T>::operator==(const pointList<T>& other) const {
    return equals(other);
  }

  // return true if the list is empty
//   template<class T>
//   bool pointList<T>::empty() const {
//     return thePointList_.empty();
//   }


  // return name of the class
  template<class T>
  const std::string& pointList<T>::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // creates a clone of this pointList
  template<class T>
  ioObject* pointList<T>::clone() const {
    pointList<T>* tmp = new pointList<T>(*this);
    return tmp;
  }

  // creates a new instance of pointList
  template<class T>
  ioObject* pointList<T>::newInstance() const {
    pointList<T>* tmp = new pointList<T>;
    return tmp;
  }

  // compare this pointList with other
  // @param other the other pointList to be compared with
  // @return true if both pointList have the same elements and same size
  template<class T>
  bool pointList<T>::equals(const pointList<T>& other) const {
    if (boundary_ == other.boundary_) {
      const_iterator it = this->begin();
      const_iterator itOther = other.begin();
      const const_iterator itEnd = this->end();
      const const_iterator itOtherEnd = other.end();
      while ( (it!=itEnd) && (itOther!=itOtherEnd) && ((*it)==(*itOther)) ) {
        it++;
        itOther++;
      }
      return ((it==itEnd) && (itOther==itOtherEnd));
    } else {
      return false;
    }

  }

  template<class T>
  const rectangle<T> pointList<T>::computeBoundary() const {

    rectangle<T> rect;

    if (this->empty()) {
      rect.set(0,0,0,0);
      return rect;
    }

    typename pointList<T>::const_iterator it;

    it = this->begin();
    rect.ul = *it;
    rect.br = *it;

    for (++it;it!=this->end();++it) {
      rect.ul.x = min(rect.ul.x,(*it).x);
      rect.br.x = max(rect.br.x,(*it).x);
      rect.ul.y = min(rect.ul.y,(*it).y);
      rect.br.y = max(rect.br.y,(*it).y);
    }

    return rect;
  }

  template<class T>
  const rectangle<T>& pointList<T>::updateBoundary() {
    boundary_ = computeBoundary();
    return boundary_;
  }

//   template<class T>
//   void pointList<T>::splice(const iterator& pos,
//                              pointList<T>& other) {
//     // This has to be done
//     thePointList_.splice(pos,other.thePointList_);
    
//     // FIXME: (when smallPointList becomes its splice
//     // this is not what splice should do (this invalids all iterators), but
//     // 
//     //insert(pos,other.begin(),other.end());
//     //other.clear();
//   }

  // read the vector from the given ioHandler.  The complete flag indicates
  // whether the enclosing begin and end should also be read
  template<class T>
  bool pointList<T>::read(ioHandler& handler,const bool complete) {

    int i,sz;
    bool b = true;

    if (complete) {
      b = handler.readBegin();
    }

    if (b) {
      handler.read("size",sz);
      std::string str;

      int level = handler.getLevel();
      point<T> aPoint;

      handler.readBegin();
      handler.readSymbol(str);

      if (str == "data") {
        handler.readKeyValueSeparator();
        handler.readBegin();
        this->clear();

        if (sz > 0) {
          sz--; // read all but the last one
          for (i=0;i<sz;++i) {
            lti::read(handler,aPoint);
            this->push_back(aPoint);
            handler.readKeyValueSeparator();
          }
          lti::read(handler,aPoint);
          this->push_back(aPoint);
        }
      }

      while (handler.readEnd() && (handler.getLevel() > level)) {
      }

    }

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

  // write the vector in the given ioHandler.  The complete flag indicates
  // if the enclosing begin and end should be also be written or not
  template<class T>
  bool pointList<T>::write(ioHandler& handler,const bool complete) const {

    int i;
    bool b = true;

    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {
      handler.write("size",this->size());

      handler.writeBegin();
      handler.writeSymbol(std::string("data"));
      handler.writeKeyValueSeparator();
      handler.writeBegin();
      if (this->size() > 0) {
        const_iterator it;
        const int last = this->size()-1;
        it = this->begin();
        for (i=0;i<last;++i,++it) {
          lti::write(handler,*it);
          handler.writeDataSeparator();
        }
        lti::write(handler,*it);
      }
      handler.writeEnd();
      handler.writeEnd();
    }

    if (complete) {
      b = b && handler.writeEnd();
    }

    return b;
  }

} // namespace lti

namespace lti {

  template<class T>
  bool read(ioHandler& handler,pointList<T>& plst,const bool complete) {
      return plst.read(handler,complete);
  }

  template<class T>
  bool write(ioHandler& handler, const pointList<T>& plst,
             const bool complete) {
      return plst.write(handler,complete);
  }

}


namespace std {
  template<class T>
  std::ostream& operator<<(std::ostream& s,const lti::pointList<T>& pts) {

    typename lti::pointList<T>::const_iterator it,eit;
    if (pts.empty()) {
      return s;
    }

    it  = pts.begin();
    eit = pts.end();

    s << *it;
    for (++it;it!=eit;++it) {
      s << " " << *it;
    }

    return s;
  }

}
