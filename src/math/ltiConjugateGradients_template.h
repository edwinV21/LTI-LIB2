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
 *         Contains the class lti::conjugateGradients, 
 *         which is used to find the minimum of an objective function along
 *         a given direction.
 * 
 * \author Pablo Alvarado
 * \date   19.05.2009
 *
 * revisions ..: $Id: ltiConjugateGradients_template.h,v 1.5 2010-03-12 23:17:26 alvarado Exp $
 */

#include <limits>
#include "ltiMath.h"

namespace lti {

  template<typename T,class C>
  bool conjugateGradients::apply(const C& func,
                                 const vector<T>& origin,
                                 vector<T>& p,
                                 T& val) const {

    const parameters& par = getParameters();
    lineSearch ls(par.lineSearchParameters);
    
    const T ftol = static_cast<T>(par.tolerance);
    const T eps  = std::numeric_limits<T>::epsilon();

    const int n = origin.size();

#if 1
    const T gtol = ftol/3;
    T gg,dgg;

    vector<T> g(n),h(n),last(origin);
    vector<T> grad(n);

    // evaluate the function and its gradient at the origin position
    T fp=func.apply(origin);
    func.gradient(origin,grad);

    _lti_debug4("Origin at " << origin << " = " << fp << std::endl);
    _lti_debug4("  with gradient at " << origin << " = " << grad << std::endl);

    T sumg=T(0);
    for (int i=0;i<n;++i) {
      g[i] = -grad[i];
      sumg+=abs(grad[i] = h[i] = g[i]);
    }

    if (sumg<eps) {
      // hmm, it seems we are already in a minimum, but let the algorithm
      // find that out
      g[0]=h[0]=grad[0]=T(1);
    }

    for (int i=0;i<par.maxIterations;++i) {

      _lti_debug3(i << ". Iteration\n"); 

      // Perform lineSearch of the minimum of the function func, 
      // starting at the origin, along the direction given by grad.
      // That minimum will be at p, and the function value there is val.
      if (!ls.apply(func,last,grad,p,val)) {
	setStatusString("Line search failed");
	val=func.apply(p); // keep on with the last value found.
      }
      // are we ready?
      if (2*abs(val-fp) <= ftol*(abs(val)+abs(fp)+eps)) {
        return true;  // yes, we are!
      }

      // new point reached... find gradient there
      fp = val;
      func.gradient(p,grad);

      _lti_debug3("  At " << p << "\n"); 

      _lti_debug4("  Function at " << p << " = " << fp   << std::endl);
      _lti_debug4("  Gradient at " << p << " = " << grad << std::endl);

      // checking if gradient is too small to continue:
      T test=0;
      T den=max(fp,static_cast<T>(1));
      for (int j=0;j<n;++j) {
        T tmp=abs(grad[j])*max(abs(p[j]),static_cast<T>(1))/den;
        if (tmp>test) {
          test=tmp;
        }
      }
      if (test < gtol) {
        return true;
      }
      
      dgg=gg=static_cast<T>(0);
      for (int j=0;j<n;++j) {
        const T d = grad[j];
        const T e = g[j];
        gg += e*e;
        // dgg += d*d;       // Fletcher-Reeves
        dgg += (d + e)*d; // Polak-Ribiere
      }

      if (gg==static_cast<T>(0)) {
        // perfect zero! (Lucky me!) -> Ready
        return true;
      }
      
      T gam = dgg/gg;
      
      for (int j=0;j<n;++j) {
        g[j] = -grad[j];
        grad[j]=h[j]=g[j]+gam*h[j];
      }

      last.swap(p);
    } // for all iterations

#else

    vector<T> g(n),r(n),gp1(n),last(origin);
    vector<T> grad(n);
    
    // evaluate the function and its gradient at the origin position
    T fp=func.apply(origin);
    func.gradient(origin,grad);

    _lti_debug4("Origin at " << origin << " = " << fp << std::endl);
    _lti_debug4("  with gradient " << origin << " = " << grad << std::endl);

    T sumg=T(0);
    for (int i=0;i<n;++i) {
      g[i] = -grad[i];
      sumg+=sqr(r[i] = g[i]);
    }

    if (sumg<eps) {
      // hmm, it seems we are already in a minimum, but let the algorithm
      // find that out for us: just set the direction to something different 
      // than zero
      g[0]=r[0]=grad[0]=T(1);
    }

    for (int i=0;i<par.maxIterations;++i) {

      _lti_debug3(i << ". Iteration\n"); 

      // Perform lineSearch of the minimum of the function func, 
      // starting at the origin, along the direction given by grad.
      // That minimum will be at p, and the function value there is val.
      ls.apply(func,last,r,p,val);
      // are we ready?
      if (2*abs(val-fp) <= ftol*(abs(val)+abs(fp)+eps)) {
        return true;  // yes, we are!
      }

      // new point reached... find gradient there
      fp = val;
      func.gradient(p,grad);
      
      _lti_debug3("  At " << p << "\n"); 

      _lti_debug4("  Function at " << p << " = " << fp   << std::endl);
      _lti_debug4("  Gradient at " << p << " = " << grad << std::endl);
     
      T beta=T(0);
      T gg(T(0));
      for (int j=0;j<n;++j) {
        const T gp1j = gp1[j] = -grad[j];
        const T gj = g[j];
        gg+=gj*gj;
        beta+=gp1j*(gp1j-gj);
      }

      beta/=gg;
      if (beta < T(0)) {
        beta=T(0);
      }

      
      // Next direction 
      r.addScaled(gp1,beta,r);

      _lti_debug4("  Beta = " << beta << std::endl); 
      _lti_debug4("  Next direction = " << r << std::endl); 
      
      // the next time gp1 is g
      g.swap(gp1);
      p.swap(last);
    } // for all iterations

#endif

    setStatusString("Maximum allowed number of iterations reached");
    return false;
  }
  
}


