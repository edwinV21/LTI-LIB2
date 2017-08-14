/*
 * Copyright (C) 2008
 * Pablo Alvarado
 *
 * This file is backported from the Computer Vision and Robotics Library
 * (CVR-Lib ) for the LTI-Lib-2
 *
 * The CVR-Lib is free software; you can redistribute it and/or
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
 * \file   ltiAffineTransformation2D_template.h
 *         Contains the class affineTransformation2D,
 *         which has still to be documented by Pablo Alvarado.
 *
 * \author Pablo Alvarado
 * \date   15.01.2008
 *
 * revisions ..: $Id: ltiAffineTransformation2D_template.h,v 1.1 2009-10-19 01:30:17 alvarado Exp $
 */

namespace lti {

  // On place apply for type point<T>!
  template<class P>
  bool affineTransformation2D::apply(P& srcdest) const {
    P dest;
    if (apply(srcdest,dest)) {
      srcdest=dest;
      return true;
    }
    return false;
  }

  // On copy apply for type point<T>!
  template<class P>
  bool affineTransformation2D::apply(const P& src, P& dest) const {

    typedef typename P::value_type value_type;

    const value_type x = src[0];
    const value_type y = src[1];

    dest[0] = static_cast<value_type>( x*a00_ + y*a01_ + trans_.x);
    dest[1] = static_cast<value_type>(-x*a10_ + y*a11_ + trans_.y);

    return true;
  }

  template<class P>
  bool affineTransformation2D::estimate(const std::vector<P>& setA,
                                        const std::vector<P>& setB) {


    typedef double value_type;

    assert(setA.size()>=2);
    assert(setB.size()>=2);

    const value_type x1 = static_cast<value_type>(setA[0].x);
    const value_type y1 = static_cast<value_type>(setA[0].y);

    const value_type x2 = static_cast<value_type>(setA[1].x);
    const value_type y2 = static_cast<value_type>(setA[1].y);

    const value_type x3 = static_cast<value_type>(setA[2].x);
    const value_type y3 = static_cast<value_type>(setA[2].y);

    const value_type bx1 = static_cast<value_type>(setB[0].x);
    const value_type by1 = static_cast<value_type>(setB[0].y);

    const value_type bx2 = static_cast<value_type>(setB[1].x);
    const value_type by2 = static_cast<value_type>(setB[1].y);

    const value_type bx3 = static_cast<value_type>(setB[2].x);
    const value_type by3 = static_cast<value_type>(setB[2].y);

    const value_type det = x1*(y2-y3) + y1*(x3-x2) + y3*x2 - y2*x3 ;

    if (abs(det) > std::numeric_limits<value_type>::epsilon()) {
      const value_type m00 = (y2-y3)/det;
      const value_type m02 = (y3-y1)/det;
      const value_type m04 = (y1-y2)/det;

      const value_type m10 = (x3-x2)/det;
      const value_type m12 = (x1-x3)/det;
      const value_type m14 = (x2-x1)/det;

      const value_type m21 = m00;
      const value_type m23 = m02;
      const value_type m25 = m04;

      const value_type m31 = m10;
      const value_type m33 = m12;
      const value_type m35 = m14;

      const value_type m40 = (y3*x2-y2*x3)/det;
      const value_type m42 = (x3*y1-x1*y3)/det;
      const value_type m44 = (x1*y2-x2*y1)/det;

      const value_type m51 = m40;
      const value_type m53 = m42;
      const value_type m55 = m44;

      parameters& par = getRWParameters();

      a00_ = par.a00 = static_cast<float>(m00*bx1 + m02*bx2 + m04*bx3);
      a01_ = par.a01 = static_cast<float>(m10*bx1 + m12*bx2 + m14*bx3);

      a10_ = par.a10 = static_cast<float>(m21*by1 + m23*by2 + m25*by3);
      a11_ = par.a11 = static_cast<float>(m31*by1 + m33*by2 + m35*by3);

      trans_.x = par.translation.x = 
        static_cast<float>(m40*bx1 + m42*bx2 + m44*bx3);
      trans_.y = par.translation.y = 
        static_cast<float>(m51*by1 + m53*by2 + m55*by3);

      return true;
    }

    return false;
  }

  template<class P>
  bool affineTransformation2D::estimateLLS(const std::vector<P>& setA,
                                           const std::vector<P>& setB) {
    static const ivector empty;
    return estimateLLS(empty,setA,setB);
  }

  template<class P>
  bool affineTransformation2D::estimateLLS(const ivector& selection,
                                           const std::vector<P>& setA,
                                           const std::vector<P>& setB) {

    assert(setA.size() == setB.size());

    const int size = (selection.empty() ? 
                      static_cast<int>(min(setA.size(),setB.size())) :
                      min(static_cast<int>(min(setA.size(),setB.size())),
                          selection.size()));

    // To solve this, the singular value decomposition method is used
    // We can state this problems as the solution of
    // A x = b
    // with A a matrix created with the coordinates of the points in setA,
    //      b a vector with the coordinates of the points in setB
    //      x a vector with the set of parameters to be found
    // Then, to find the solution A is decomposed with the SVD as
    // A = U S V'
    // The linear least square solution is then obtained
    // x = V S^-1 U' b

    typedef double value_type;

    // Construct matrix A and vector b

    matrix<value_type> A(2*size,6);
    vector<value_type> b(2*size);

    if (selection.empty()) {
      for (int i=0,j=0;i<size;++i) {
        b.at(j) = static_cast<value_type>(setB[i].x);
        
        A.at(j,0) = static_cast<value_type>(setA[i].x);
        A.at(j,1) = static_cast<value_type>(setA[i].y);
        A.at(j,2) = 0.0f;
        A.at(j,3) = 0.0f;
        A.at(j,4) = 1.0f;
        A.at(j,5) = 0.0f;
        
        j++;
        
        b.at(j) = static_cast<value_type>(setB[i].y);
        
        A.at(j,0) = 0.0f;
        A.at(j,1) = 0.0f;
        A.at(j,2) = static_cast<value_type>(setA[i].x);
        A.at(j,3) = static_cast<value_type>(setA[i].y);
        A.at(j,4) = 0.0f;
        A.at(j,5) = 1.0f;
        
        j++;
      }
    } else {
      for (int i=0,j=0;i<size;++i) {
        b.at(j) = static_cast<value_type>(setB[selection.at(i)].x);
        
        A.at(j,0) = static_cast<value_type>(setA[selection.at(i)].x);
        A.at(j,1) = static_cast<value_type>(setA[selection.at(i)].y);
        A.at(j,2) = 0.0f;
        A.at(j,3) = 0.0f;
        A.at(j,4) = 1.0f;
        A.at(j,5) = 0.0f;
        
        j++;
        
        b.at(j) = static_cast<value_type>(setB[selection.at(i)].y);
        
        A.at(j,0) = 0.0f;
        A.at(j,1) = 0.0f;
        A.at(j,2) = static_cast<value_type>(setA[selection.at(i)].x);
        A.at(j,3) = static_cast<value_type>(setA[selection.at(i)].y);
        A.at(j,4) = 0.0f;
        A.at(j,5) = 1.0f;
        
        j++;
      }      
    }

    vector<value_type> x;
    if (lls_.apply(A,b,x)) {
      parameters& par = getRWParameters();

      a00_ = par.a00 = static_cast<float>(x.at(0));
      a01_ = par.a01 = static_cast<float>(x.at(1));
      a10_ = par.a10 = static_cast<float>(x.at(2));
      a11_ = par.a11 = static_cast<float>(x.at(3));

      trans_.x = par.translation.x = static_cast<float>(x.at(4));
      trans_.y = par.translation.y = static_cast<float>(x.at(5));
      
      return true;
    }

    return false;
  }

}


