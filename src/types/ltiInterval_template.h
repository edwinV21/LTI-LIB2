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


/**
 * \file   ltiInterval_template.h
 *         Defines the template type lti::interval<T> and all shortcuts like
 *         lti::iinterval.
 * \author Pablo Alvarado
 * \date   01.11.2002
 *
 * $Id: ltiInterval_template.h,v 1.5 2010-04-04 13:58:56 alvarado Exp $
 */


namespace lti {

  // helper structure to create at compile time efficiently the
  // length function
  // The trailing underscore denotes "private".
  // we use a private method defined in ltiRound_template!
  // The booleans indicate if the types are floating point types or not.
  template<typename T,bool B2> 
  struct intervalLength_ {
    static inline T length(const T from,const T to) {
      return (to-from+1);
    }
  };

  template<typename T>
  struct intervalLength_<T,true> {
    static inline T length(const T from,const T to) {
      return (to-from);
    }
  };


  /*
   * Default constructor
   */
  template<typename T>
  interval<T>::interval(const T newfrom,const T newto) 
    : from(newfrom),to(newto) {
  }

  /*
   * Copy constructor
   */    
  template<typename T>
  template <typename U>
  interval<T>::interval(const interval<U>& p) 
    : from(static_cast<T>(p.from)),to(static_cast<T>(p.to)) {
  }
  
  template<typename T>
  template <typename U>
  interval<T>& interval<T>::castFrom(const interval<U>& p) {
    from = static_cast<T>(p.from);
    to = static_cast<T>(p.to);
    return *this;
  }
  
  /*
   * Set the coordinate values and return a reference to this interval
   */
  template<typename T>
  inline interval<T>& interval<T>::set(const T tx,const T ty) {
    from=tx;
    to=ty;
    return *this;
  }
  
  /*
   * Get the limit values
   */
  template<typename T>
  inline void interval<T>::get(T& tx,T& ty) const {
    tx=from;
    ty=to;
  }
  
  /*
   * Return \c true if \a from is greater or equal to \a to
   */
  template<typename T>
  inline bool interval<T>::valid() const {
    return (from<=to);
  }
  
  /*
   * Return \c true if \a from less than \a to
   */
  template<typename T>
  inline bool interval<T>::empty() const {
    return (from>to);
  }

  /*
   * Scale an interval, multiplying each limit by the given scalar.
   *
   * The type U has to be compatible with the type T in the sense that
   * they can be multiplied and casted to T.  This applies to most build 
   * in type like int, float, etc.
   */
  template<typename T>
  template<typename U>
  inline interval<T>& interval<T>::multiply(const U c) {
    from=static_cast<T>(from*c);
    to  =static_cast<T>(to*c);
  }

  /*
   * Scale an interval, multiplying each limit by the given scalar.
   *
   * The type U has to be compatible with the type T in the sense that
   * they can be multiplied and casted to T.  This applies to most build 
   * in type like int, float, etc.
   */
  template<typename T>
  template<typename U>
  inline interval<T> interval<T>::operator*(const U c) const {
    return interval<T>(static_cast<T>(from*c),
                       static_cast<T>(to*c));
  }
  
  /*
   * Multiply the other interval interval<T> with a given scale factor
   *
   * The type U has to be compatible with the type T in the sense that
   * they can be multiplied and casted to T.  This applies to most build 
   * in type like int, float, etc.
   */
  template<typename T>
  template<typename U>
  inline interval<T>& 
  interval<T>::multiply(const interval<T>& other,const U c) {
    from=static_cast<T>(other.from*c);
    to  =static_cast<T>(other.to*c);
    return *this;
  }
  
  /*
   * Multiply interval<T> with a given factor
   *
   * The type U has to be compatible with the type T in the sense that
   * they can be multiplied and casted to T.  This applies to most build 
   * in type like int, float, etc.
   */
  template<typename T>
  template<typename U>
  inline interval<T>& interval<T>::operator*=(const U c) {
    return multiply(c);
  }
  
  /*
   * Multiplies elementwise the components of this and the interval c, and
   * leave the result here.
   */
  template<typename T>
  inline interval<T>& interval<T>::emultiply(const interval<T>& c) {
    from*=c.from;
    to*=c.to;
    return *this;
  }

  /*
   * Multiplies elementwise the components of \a a and \a b and leave the
   * result here.
   */
  template<typename T>
  inline interval<T>& interval<T>::emultiply(const interval<T>& a,
                                             const interval<T>& b) {
    from = a.from*b.from;
    to   = a.to  *b.to;
    return *this;
  }
  
  /*
   * Divide each component of interval<T> with a given factor
   */
  template<typename T>
  template<typename U>
  inline interval<T>& interval<T>::divide(const U c) {
    from/=c;
    to/=c;
    return *this;
  }

  /*
   * Divide each component of other other interval<T> with a given factor
   */
  template<typename T>
  template<typename U>
  inline interval<T>& interval<T>::divide(const interval<T>& other,const U c) {
    from = static_cast<T>(other.from/c);
    to   = static_cast<T>(other.to/c);
    return *this;
  }
  
  /*
   * Divide each component of interval<T> by a given factor
   */
  template<typename T>
  template <typename U>
  inline interval<T> interval<T>::operator/(const U c) const {
    return interval<T>(static_cast<T>(from/c),static_cast<T>(to/c));
  }

  /*
   * Divide each component of interval<T> by a given factor
   */
  template<typename T>
  template <typename U>
  inline interval<T>& interval<T>::operator/=(const U c) {
    return this->divide(c);
  }

  /*
   * Elementwise division of each component of the intervals
   */
  template<typename T>
  inline interval<T>& interval<T>::edivide(const interval<T>& c) {
    from/=c.from;
    to/=c.to;
    return *this;
  }

  /*
   * Elementwise division of each component of the intervals
   */
  template<typename T>
  inline interval<T>& interval<T>::edivide(const interval<T>& a,
                                           const interval<T>& b) {
    from = a.from/b.from;
    to   = a.to  /b.to;
    return *this;
  }

  /*
   * Modulo c of the integer part of each component of the interval
   */
  template<typename T>
  inline interval<T> interval<T>::operator%(const int c) const {
    return interval<T>(static_cast<int>(from) % c,
                       static_cast<int>(to) % c);
  }

  /*
   * Check if element is contained in the interval
   *
   * Return true if the given value is contained within the
   * container.  This method assumes that the intervall represents a closed
   * interval, so that the boundaries are considered as part of it.
   */
  template<typename T>
  inline bool interval<T>::contains(const T val) const {
    return ( ! ((val<from) || (to<val)) );
  }

  template<typename T>
  inline bool interval<T>::contains(const interval<T>& val) const {
    return ( (from<=val.from) && (val.to<=to) );
  }

  /*
   * Find the smallest interval which contains all points of both,
   * this interval and the given one.
   *
   * Since the intervals may not overlap, the result 
   *
   * @param p the other interval to be added to this one
   * @return a reference to this interval
   */
  template<typename T>
  inline interval<T>& interval<T>::join(const interval<T>& p) {
    from = min(from,p.from);
    to   = max(to,p.to);
    return *this;
  }
    
  /*
   * Find the smallest interval which contains all points of the intervals
   * a and b.
   */
  template<typename T>
  inline interval<T>& interval<T>::join(const interval<T>& a,
                                        const interval<T>& b) {
    from = min(a.from,b.from);
    to   = max(a.to,b.to);
    return *this;
  }
  
  /*
   * Find intersection
   */
  template<typename T>
  inline interval<T>& interval<T>::intersect(const interval<T>& p) {
    from = max(from,p.from);
    to   = min(to,p.to);
    return *this;
  }
    
  /*
   * Find intersection
   */
  template<typename T>
  inline interval<T>& interval<T>::intersect(const interval<T>& a,
                                             const interval<T>& b) {
    from = max(a.from,b.from);
    to   = min(a.to,b.to);
    return *this;
  }

  /*
   * Set subtraction of this and the given interval.
   *
   * Here, the subtraction means the smallest interval which
   * contains all the points of the set-subtraction of this interval
   * minus all the elements of the given one.
   */
  template<typename T>
  inline interval<T>& interval<T>::subtract(const interval<T>& p) {
    return subtract(*this,p);
  }


  /*
   * Subtract the two other intervals and leave the result here
   *
   * Here, the subtraction means the smallest interval which
   * contains all the points of the set-subtraction of this interval
   * minus all the elements of the given one.
   *
   * @param a first interval
   * @param b interval to be subtracted from the first one
   * @return a reference to this interval, which will contain a-b
   */
  template<typename T>
  inline interval<T>& interval<T>::subtract(const interval<T>& a,
                                            const interval<T>& b) {
    
    if (a.contains(b)) {
      return copy(a);
    } 
    
    if (a.from<b.from) {
      from=a.from;
      to=b.from;
      return *this;
    }
    
    from=b.to;
    to = a.to;
    
    return *this;
  }

    /*
     * Operator -
     */
  template<typename T>
  inline interval<T> interval<T>::operator-(const interval<T>& p) const {
    interval<T> me(*this);
    me.subtract(p);
    return me;
  }

  /*
   * Operator -=
   */
  template<typename T>
  inline interval<T>& interval<T>::operator-=(const interval<T>& p) {
    return subtract(p);
  }
  
  /*
   * Copy operator
   */
  template<typename T>
  inline interval<T>& interval<T>::copy(const interval<T>& p) {
    from=p.from;
    to=p.to;
    return *this;
  }

    /*
     * Operator =
     */
  template<typename T>
  inline interval<T>& interval<T>::operator=(const interval<T>& p) {
    return copy(p);
  }
  

  /*
   * Operator ==
   */
  template<typename T>
  inline bool interval<T>::operator==(const interval<T>& p) const {
    return ((from==p.from)&&(to==p.to));
  }
  
    /*
     * Operator !=
     */
  template<typename T>
  inline bool interval<T>::operator!=(const interval<T>& p) const {
    return ((from!=p.from)||(to!=p.to));
  }

  /*
   * Length of the interval
   *
   * For this method, the intervals are considered closed.  This is
   * specially important for integer types, which will return
   * to-from+1, while for floating-point types it just return to-from.
   * In other words, the "length" of an integer interval is the number of
   * elements that fit within, while the length of a floating point interval is
   * the real euclidean length.
   *
   * \warning The meaning of this method may change in the future, depending on
   * what is more useful...
   *
   */
  template<typename T>
  inline T interval<T>::length() const {
    return intervalLength_<T,fp_info_<T>::isFP>::length(from,to);
  }
  
  /*
   * Return the closest value of type T which lies on the interval.
   *
   * This method returns the same value of the argument iff that
   * value lies within the interval, \a from if the given value is
   * less than it, or \a to if it is greater.
   */
  template<typename T>
  inline T interval<T>::closest(const T val) const {
    return within(val,from,to);
  }
  
  /*
   * @name Access as vector
   */
  //@{
  /*
   * Used to simulate vector access.  It allows the use of interval
   * in templates expecting a vector-like structure.
   *
   * The correspondence between the elements of the vector and
   * the components will be [0] for x and [1] for y
   */
  template<typename T>
  inline T& interval<T>::operator[](const int i) {
    assert(static_cast<unsigned int>(i)<2);
    return data[i];
  }

  /*
   * Used to simulate read-only vector access.  It allows the use of interval
   * in templates expecting a vector-like structure.
   *
   * The correspondence between the elements of the vector and
   * the components will be [0] for x and [1] for y
   */
  template<typename T>
  inline const T& interval<T>::operator[](const int i) const{
    assert(static_cast<unsigned int>(i)<2);
    return data[i];
  }
  
  /*
   * Used to simulate the vector size.
   */
  template<typename T>
  inline typename interval<T>::size_type interval<T>::size() const {
    return 2;
  }

  /*
   * Read the vector from the given ioHandler.  The complete flag indicates
   * if the enclosing begin and end should be also be readed
   *
   * @ingroup gStorable
   */
  template <typename T>
  bool read(ioHandler& handler,interval<T>& p,const bool complete) {
    bool b(true);

    if (complete) {
      b = b && handler.readBegin();
    }
    
    b = b && lti::read(handler,p.from);
    b = b && handler.readDataSeparator();
    b = b && lti::read(handler,p.to);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

  /*
   * Write the vector in the given ioHandler.  The complete flag indicates
   * if the enclosing begin and end should be also be written or not
   *
   * @ingroup gStorable
   */
  template<typename T>
  bool write(ioHandler& handler,
             const interval<T>& p,
             const bool complete) {

    bool b(true);

    if (complete) {
      b = b && handler.writeBegin();
    }
    
    b = b && lti::write(handler,p.from);
    b = b && handler.writeDataSeparator();
    b = b && lti::write(handler,p.to);

    if (complete) {
      b = b && handler.writeEnd();
    }

    return b;
  }
}

namespace std {
  
  //inline ostream& operator<<(ostream& s,const lti::interval& p);
  template <typename T>
  inline ostream& operator<<(ostream& s,const lti::interval<T>& p) {
    s << "[" 
      << p.from << ","
      << p.to << "]";
    return s;
  }
  
  //inline ostream& operator>>(istream& s,const lti::interval& p);
  template <typename T>
  inline istream& operator>>(istream& s,lti::interval<T>& p) {
    char c;
    s >> c
      >> p.from >> c
      >> p.to   >> c;

    return s;
  }

}
