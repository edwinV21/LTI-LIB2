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
 * \file   ltiEuclideanTransformation2D_template.h
 *         Contains the class euclideanTransformation2D,
 *         which has still to be documented by Pablo Alvarado.
 *
 * \author Pablo Alvarado
 * \date   15.01.2008
 *
 * revisions ..: $Id: ltiEuclideanTransformation2D_template.h,v 1.1 2009-10-19 01:30:17 alvarado Exp $
 */

namespace lti {

  // On place apply for type point<T>!
  template<class P>
  bool euclideanTransformation2D::apply(P& srcdest) const {
    P dest;
    if (apply(srcdest,dest)) {
      srcdest=dest;
      return true;
    }
    return false;
  }

  // On copy apply for type point<T>!
  template<class P>
  bool euclideanTransformation2D::apply(const P& src, P& dest) const {

    typedef typename P::value_type value_type;

    const value_type x = src[0];
    const value_type y = src[1];

    dest[0] = static_cast<value_type>( x*cosa_ + y*sina_ + trans_.x);
    dest[1] = static_cast<value_type>(-x*sina_ + y*cosa_ + trans_.y);

    return true;
  }

  template<class P>
  bool euclideanTransformation2D::estimate(const std::vector<P>& setA,
                                           const std::vector<P>& setB) {


    typedef double value_type;

    assert(setA.size()>=2);
    assert(setB.size()>=2);

    const value_type x1 = static_cast<value_type>(setA[0][0]);
    const value_type y1 = static_cast<value_type>(setA[0][1]);

    const value_type x2 = static_cast<value_type>(setA[1][0]);
    const value_type y2 = static_cast<value_type>(setA[1][1]);

    const value_type bx1 = static_cast<value_type>(setB[0][0]);
    const value_type by1 = static_cast<value_type>(setB[0][1]);

    const value_type bx2 = static_cast<value_type>(setB[1][0]);
    const value_type by2 = static_cast<value_type>(setB[1][1]);

    const value_type det = 2*(x1*x2 + y1*y2) - x1*x1 - x2*x2 - y1*y1 - y2*y2;

    if (abs(det) >  std::numeric_limits<value_type>::epsilon()) {
      const value_type m00 = (x2-x1)/det;
      const value_type m01 = (y2-y1)/det;
      const value_type m02 = -m00;
      const value_type m03 = -m01;

      const value_type m10 = (x1*x2 + y1*y2 - x2*x2 - y2*y2)/det;
      const value_type m11 = (y1*x2 - y2*x1)/det;
      const value_type m12 = (x1*x2 + y1*y2 - x1*x1 - y1*y1)/det;
      const value_type m13 = -m11;

      const value_type m20 = m13;
      const value_type m21 = m10;
      const value_type m22 = m11;
      const value_type m23 = m12;

      const value_type m30 = m01;
      const value_type m31 = m02;
      const value_type m32 = m03;
      const value_type m33 = m00;

      const value_type A = m00*bx1 + m01*by1 + m02*bx2 + m03*by2;
      const value_type B = m10*bx1 + m11*by1 + m12*bx2 + m13*by2;
      const value_type C = m20*bx1 + m21*by1 + m22*bx2 + m23*by2;
      const value_type D = m30*bx1 + m31*by1 + m32*bx2 + m33*by2;

      parameters& par = getRWParameters();
      par.angle = atan2(D,A);
      sincos(par.angle,sina_,cosa_);
      trans_ = par.translation.set(B,C);

      return true;
    }

    return false;
  }


  template<class P>
  bool euclideanTransformation2D::estimateLLS(const std::vector<P>& setA,
                                              const std::vector<P>& setB) {
    static const ivector empty;
    return estimateLLS(empty,setA,setB);    
  }

  template<class P>
  bool euclideanTransformation2D::estimateLLS(const ivector& selection,
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

    matrix<value_type> A(2*size,4);
    vector<value_type> b(2*size);

    if (selection.empty()) {
      for (int i=0,j=0;i<size;++i) {
        b.at(j) = static_cast<value_type>(setB[i][0]);
        
        A.at(j,0) = static_cast<value_type>(setA[i][0]);
        A.at(j,1) = 1.0f;
        A.at(j,2) = 0.0f;
        A.at(j,3) = static_cast<value_type>(setA[i][1]);
        
        j++;
        
        b.at(j) = static_cast<value_type>(setB[i][1]);
        
        A.at(j,0) = static_cast<value_type>(setA[i][1]);
        A.at(j,1) = 0.0f;
        A.at(j,2) = 1.0f;
        A.at(j,3) = -static_cast<value_type>(setA[i][0]);
        
        j++;
      }
    } else {
      for (int i=0,j=0;i<size;++i) {
        b.at(j) = static_cast<value_type>(setB[selection.at(i)][0]);
        
        A.at(j,0) = static_cast<value_type>(setA[selection.at(i)][0]);
        A.at(j,1) = 1.0f;
        A.at(j,2) = 0.0f;
        A.at(j,3) = static_cast<value_type>(setA[selection.at(i)][1]);
        
        j++;
        
        b.at(j) = static_cast<value_type>(setB[selection.at(i)][1]);
        
        A.at(j,0) = static_cast<value_type>(setA[selection.at(i)][1]);
        A.at(j,1) = 0.0f;
        A.at(j,2) = 1.0f;
        A.at(j,3) = -static_cast<value_type>(setA[selection.at(i)][0]);
        
        j++;
      }
    }

    vector<value_type> x;
    if (lls_.apply(A,b,x)) {
      parameters& par = getRWParameters();
      par.angle = static_cast<float>(atan2(x.at(3),x.at(0)));
      sincos(par.angle,sina_,cosa_);
      trans_ = par.translation.set(static_cast<float>(x.at(1)),
                                   static_cast<float>(x.at(2)));

      return true;
    }

    return false;
  }

}


