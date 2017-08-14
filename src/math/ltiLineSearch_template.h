/*
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

/** 
 * \file   ltiLineSearch_template.h
 *         Contains the class lti::lineSearch, 
 *         which is used to find the minimum of an objective function along
 *         a given direction.
 * 
 * \author Pablo Alvarado
 * \date   19.05.2009
 *
 * revisions ..: $Id: ltiLineSearch_template.h,v 1.5 2012-01-03 02:22:25 alvarado Exp $
 */

#include <limits>
#include "ltiMath.h"

namespace lti {

  template<typename T,class C>
  lineSearch::adapter<T,C>::adapter(const C& functorInterface,
                                    const vector<T>& from,
                                    const vector<T>& direction) 
    : lastPos(from),
      functor_(functorInterface),
      from_(from),
      direction_(direction) {
  }

  template<typename T,class C>
  inline T lineSearch::adapter<T,C>::operator()(const T val) {
    const T res = functor_.apply(lastPos.addScaled(from_,val,direction_));

    _lti_debug4("      f(" << val << ") = f" << lastPos << " = " << res << 
                "\n");

    return res;
  }

  template<typename T>
  lineSearch::helper<T>::helper(const parameters& params) 
    : maxIterations_(params.maxIterations),
      tolerance_(T(params.epsilon)),
      glimit_(T(params.maxBracketStepMagnification)),
      ax_(),bx_(),cx_(),
      fa_(),fb_(),fc_(),
      fmin_(),xmin_() {
  }

  template<typename T>
  inline void lineSearch::helper<T>::shift(T& a,T& b,T& c,const T d) {
    a=b;
    b=c;
    c=d;
  }

  template<typename T>
  inline T lineSearch::helper<T>::sign(const T mag,
                                       const T s) {
    if (s>=static_cast<T>(0)) {
      if (mag>=static_cast<T>(0)) {
        return mag;
      } else {
        return -mag;
      }
    } else {
      if (mag<static_cast<T>(0)) {
        return mag;
      } else {
        return -mag;
      }
    }
  }

  template<typename T>
  template<class C1>
  void lineSearch::helper<T>::bracket(const T a, 
                                      const T b,
                                      C1& objFunction) {

    const T gold = static_cast<T>(1.61803398874989484820458683436563811);
    const T tiny = std::numeric_limits<T>::epsilon();

    // initialize attributes with starting bracket
    ax_ = a;
    bx_ = b;

    //
    T fu;
    fa_ = objFunction(ax_);
    fb_ = objFunction(bx_);

    // ensure that always fa_ > fb_
    if (fb_ > fa_) {
      swap(ax_,bx_);
      swap(fa_,fb_);
    }

    // first guess for c
    cx_ = bx_ + gold*(bx_-ax_);
    fc_ = objFunction(cx_);

    // until a bracket is found do...
    while (fb_ > fc_) {
      const T bmax=bx_-ax_;
      const T r=bmax*(fb_-fc_);
      const T bmcx = bx_-cx_;
      const T q=bmcx*(fb_-fa_);
      const T qmr = (q-r);
      const T den = (abs(qmr)<tiny) ? signum(qmr)*tiny : 2*qmr;
      T u=bx_-(bmcx*q-bmax*r)/den;
      const T ulim = bx_-glimit_*bmcx;
      const T umcx = u-cx_;

      if ( (bx_-u)*umcx > static_cast<T>(0) ) {
        // Parabolic u lies between b and c
        fu = objFunction(u);
        if (fu<fc_) { // minimum between b and c
          ax_=bx_;
          bx_=u;
          fa_=fb_;
          fb_=fu;
          return;
        } else if (fu > fb_) { // minimum between a and u
          cx_ = u;
          fc_ = fu;
          return;
        }
        // note that bx_ and cx_ haven't change, so it is valid to use bmcx
        u=cx_-gold*bmcx; // parabolic fit failed! use default magnification
        fu=objFunction(u);
      } else if ((umcx*(ulim-u)) > static_cast<T>(0)) {
        // Parabolic fit lies between c and the limit
        fu = objFunction(u);
        if (fu<fc_) {
          shift(bx_,cx_,u,u+gold*umcx);
          shift(fb_,fc_,fu,objFunction(u));
        }
      } else if ((u-ulim)*(ulim-cx_) >= static_cast<T>(0)) {
        // restrict u to maximum allowed value
        u=ulim;
        fu=objFunction(u);
      } else {
        u=cx_-gold*bmcx;
        fu=objFunction(u);
      }

      shift(ax_,bx_,cx_,u);
      shift(fa_,fb_,fc_,fu);
    }
      
  }


  template<typename T>
  template<class C1>
  T lineSearch::helper<T>::brent(C1& objFunction) {

    T a,b,d=T(0);
    T fu,fv,fw,fx;
    T p,q,r,tol1,tol2,u,v,w,x,xm;
    T e=static_cast<T>(0);
    p=q=r=static_cast<T>(0);

    const T cgold = static_cast<T>(0.381966011250105151795413165634);
    const T zeps = std::numeric_limits<T>::epsilon()*static_cast<T>(1.0e-3);

    minmax(ax_,cx_,a,b);
    x=w=v    = bx_;
    fw=fv=fx = fb_;

    for (int i=0;i<maxIterations_;++i) { // main loop
      xm=static_cast<T>(0.5)*(a+b);
      tol2=2*(tol1=tolerance_*abs(x)+zeps);

      _lti_debug4("  " << i << ". Analysing [" << a << ";" << b << "]:\n"); 
      _lti_debug4("    Current minimum at f(" << x << ") = " << fx << "\n");
      
      if (abs(x-xm) <= (tol2-static_cast<T>(0.5)*(b-a))) { // are we ready?
        _lti_debug4("    Ready: f(" << x << ") = " << fx << "\n");
        xmin_=x;
        return (fmin_=fx);
      }

      if (abs(e) > tol1) { // Construct a provisional parabolic fit
        r=(x-w)*(fx-fv);
        q=(x-v)*(fx-fw);
        p=(x-v)*q-(x-w)*r;
        q=2*(q-r);

        if (q>static_cast<T>(0)) {
          p = -p;
        } else {
          q = -q;
        }
        r=e;
        e=d;
      }

      if ((abs(p) < abs(static_cast<T>(0.5)*q*r)) &&
          (p < q*(x-a)) &&
          (p < q*(b-x))) {
        
        // The previous conditions determine how feasible is the parabolic
        // fit.  
        
        // Take the golden section step into the larger of the two segments

        d=p/q; // Take the parabolic step
        u=x+d;
        _lti_debug4("    Parabolic fit ok. Next step d=" << d << "\n");
        
        if (((u-a) < tol2) || ((b-u) < tol2)) {
          d=(x<xm)?tol1:-tol1;
        }
      } else {
        _lti_debug4("    Parabolic fit failed. Next step d=" << d << "\n");
        e = (x<xm) ? b-x : a-x;
        d = cgold*e;
      }

      u=(abs(d) >= tol1) ? x+d : x + ((d>0) ? tol1 : -tol1);

      fu=objFunction(u); // just one function evaluation per iteration

      if (fu <= fx) {
        if (u >= x) {
          a=x;
        } else {
          b=x;
        }

        // this shift assigns u to x, and fu to fx
        shift(v,w,x,u);
        shift(fv,fw,fx,fu);
        
      } else {
        if (u < x) {
          a=u;
        } else {
          b=u;
        }
        if ( (fu <= fw) || (w == x) ) {
          v=w;
          fv=fw;
          w=u;
          fw=fu;
        } else if ( (fu <= fv) || (v == x) || (v == w)) {
          v=u;
          fv=fu;
        }
      }

    } // end of for maxiterations

    // report error
    // maximum number can never be a minimum:
    return std::numeric_limits<T>::max(); 
  }

  template<typename T>
  template<class C>
  T lineSearch::helper<T>::minimize(const vector<T>& origin,
                                    const vector<T>& direction,
                                          vector<T>& newPos,
                                    const C& mvFunctor) {
    // one dimensional functor
    adapter<T,C> odFunctor(mvFunctor,origin,direction);

    // find the initial bracket
    bracket(static_cast<T>(0),static_cast<T>(1),odFunctor);  
    
    // apply brent's algorithm to find the minimum
    const T val = brent(odFunctor);

    // get position of the minimum
    newPos.addScaled(origin,xmin_,direction);

    _lti_debug4("           f(" << xmin_ << ") = f" << newPos << " = " << 
                val << std::endl);

    return val;
  }

  template<typename T,class C>
  bool lineSearch::apply(const C& func,
                         const vector<T>& origin,
                         const vector<T>& direction,
                         vector<T>& newPosition,
                         T& val) const {

    const parameters& par = getParameters();

    helper<T> h(par);
    newPosition.copy(origin);
    val=h.minimize(origin,direction,newPosition,func);
    return (val!=std::numeric_limits<T>::max());
  }
  
}


