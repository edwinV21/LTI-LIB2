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
 *         Contains the class lti::downhillSimplex, 
 *         which is used to find the minimum of an objective function along
 *         a given direction.
 * 
 * \author Pablo Alvarado
 * \date   19.05.2009
 *
 * revisions ..: $Id: ltiDownhillSimplex_template.h,v 1.5 2012-07-01 00:36:15 alvarado Exp $
 */

#include <limits>
#include "ltiMath.h"

//#define _LTI_DEBUG 3
#include <ltiDebug.h>

#ifdef _LTI_DEBUG 
#include <cstdio>
#include <iomanip>
#endif

namespace lti {

  template<typename T>
  void downhillSimplex::printSimplex(const matrix<T>& simplex,
                                     const vector<T>& vals) const {
    for (int i=0;i<simplex.rows();++i) {
      _lti_debug4("    | ");
      for (int j=0;j<simplex.columns();++j) {
        _lti_debug4(std::setw(14) << simplex.at(i,j));
      }
      _lti_debug4(" | => " << std::setw(14) << vals.at(i) << std::endl);
    }
  }


  template<typename T,class C>
  bool downhillSimplex::apply(const C& func,
                              const vector<T>& origin,
                              vector<T>& newPosition,
                              T& val) const {
    return apply(func,origin,T(1),newPosition,val);
  }

  template<typename T,class C>
  bool downhillSimplex::apply(const C& func,
                              const vector<T>& origin,
                              const T lambda,
                              vector<T>& newPosition,
                              T& val) const {

    vector<T> lambdas(origin.size(),lambda);
    return apply(func,origin,lambdas,newPosition,val);
  }

  template<typename T,class C>
  bool downhillSimplex::apply(const C& func,
                              const vector<T>& origin,
                              const vector<T>& lambdas,
                              vector<T>& newPosition,
                              T& val) const {

    const int n = origin.size();
    matrix<T> oSimplex(n+1,n);
    oSimplex.getRow(0).copy(origin);

    if (lambdas.size()<n) {
      setStatusString("Too few lambda values given.");
      return false;
    }

    for (int j=1;j<oSimplex.rows();++j) {
      oSimplex.getRow(j).copy(origin);
      oSimplex.at(j,j-1)+=lambdas.at(j-1);
    }

    return apply(func,oSimplex,newPosition,val);
  }

  template<typename T,class C>
  T downhillSimplex::helper(matrix<T>& p,
                            vector<T>& y,
                            vector<T>& psum,
                            const int ihi,
                            const T factor,
                            const C& func) const {
    const int ndim = p.columns();
    vector<T> ptry(ndim);
    vector<T>& worst=p.getRow(ihi);
    
    const T fac1 = (T(1)-factor)/static_cast<T>(ndim);
    const T fac2 = factor-fac1; //-(original fac2) because we will add  

    _lti_debug4("    Extrapolating @" << ihi << " with factor " << factor);

    // trial point is chosen such that 
    //
    //   ptry = centroid + factor*(centroid-highest)
    //
    // where the centroid is done for the best ndim points
    // an efficient way of doing it is through the sum of the points.
    _lti_debug4("  Psum: "<<psum<<std::endl);
    _lti_debug4("  Worst: "<<worst<<std::endl);
    ptry.addScaled(fac1,psum,fac2,worst);              
    _lti_debug4("  Extrapolated point: "<<ptry<<std::endl);
    // function at the new point
    const T ytry = func.apply(ptry);
    _lti_debug4("   with function value: "<<ytry<<std::endl);
    
    if (ytry < y.at(ihi)) {
      _lti_debug4("     extrapolation worked!" << std::endl);
      y.at(ihi)=ytry;
      for (int j=0;j<ndim;++j) {
        psum.at(j)+=(ptry.at(j)-worst.at(j));
        worst.at(j)=ptry.at(j);
      }
    } else {
      _lti_debug4("     extrapolation DID NOT worked!" << std::endl);
    }

    _lti_debug4("     got " << ptry << " => " << ytry << std::endl);

    return ytry;
  }

  template<typename T,class C>
  bool downhillSimplex::apply(const C& func,
                              const matrix<T>& oSimplex,
                              vector<T>& posMin,
                              T& val) const {

    const parameters& par = getParameters();
    
    const T ftol = static_cast<T>(par.tolerance);
    const T eps  = std::numeric_limits<T>::epsilon();

    const int n = oSimplex.columns();

    if (oSimplex.rows()!=n+1) {
      setStatusString("Inconsistent simplex dimensions");
      return false;
    }

    // index of highest, lowest and second highest
    int ihi,ilo,inhi;
    const int mpts = oSimplex.rows();
    const int ndim = oSimplex.columns();
    
    vector<T> psum(ndim),pdim(ndim),x(ndim);//PDIM MUST BE PMIN

    matrix<T> p(oSimplex); // current simplex
    vector<T> y(mpts);     // function values at simplex corners
    
    // evaluate the function at the corners
    for (int i=0;i<mpts;++i) {
      y.at(i) = func.apply(p.getRow(i));
    }

    int nfunc(0); // number of function evaluations    
    accumulateRows(p,psum);
    
    while(true) {
      ilo=0;
      // find out the highest, worst and second worst cases.
      
      // this syntax may need some explaining:
      // if y[0] > y[1] then assign 1 to inhi and 0 to ihi, if
      // not, is the other way around (more info on "comma operator")
      ihi = (y.at(0) > y.at(1)) ? (inhi=1 , 0) : (inhi=0 , 1);

      for (int i=0;i<mpts;++i) {
        if (y.at(i) <= y.at(ilo)) {
          ilo = i;
        }
        if (y.at(i) > y.at(ihi)) {
          inhi=ihi;
          ihi=i;
        } else if ((y.at(i) > y.at(inhi)) && (i != ihi)) {
          inhi=i;
        }
      }
     
      T rtol=static_cast<T>(2.0*abs(y.at(ihi)-y.at(ilo))/
                            (abs(y.at(ihi))+abs(y.at(ilo))+eps));
      
     
      _lti_debug3("\n  " << nfunc <<". (highest) Function at " <<p.getRow(ihi) 
                  << " = " <<y.at(ihi) <<std::endl);      
      _lti_debug3("  " << nfunc <<". (second highest) Function at " 
                  << p.getRow(inhi) << " = " <<y.at(inhi) <<std::endl);
      _lti_debug3("  " << nfunc << ". Function at " << p.getRow(ilo) << 
                  " = " << y.at(ilo) << std::endl);

      _lti_if_debug4(printSimplex(p,y));

      // Compute the fractional range from highest to lowest and return if ok
      if (rtol < ftol) {
        
        _lti_debug3("  rtol < ftol, since " << 
                    rtol << " < " << ftol << std::endl);
        
        posMin.copy(p.getRow(ilo));
        val=y.at(ilo);
        return true;
      }
      
      if (nfunc >= par.maxIterations) {
        _lti_debug3("  Maximum iterations reached" << std::endl);
        
        posMin.copy(p.getRow(ilo));
        val=y.at(ilo);
        
        setStatusString("Maximum allowed number of iterations reached");
        return false;
      }
      
      nfunc += 2;
      
      // New interation
      
      // Reflect the simplex from the high point
      
      T ytry=this->helper(p,y,psum,ihi,T(-1),func);
      if (ytry <= y.at(ilo)) {
        _lti_debug4("    Good reflexion, try more." << std::endl);
        // New point better than the best -> try an additional extrapolation
        ytry=this->helper(p,y,psum,ihi,T(2),func);
      } else if (ytry >= y.at(inhi)) {
        _lti_debug4("    New point worse, contract." << std::endl);

        // New point is worse than 2nd worst, so do a contraction
        T ysave  =  y.at(ihi);
        ytry  =  this->helper(p,y,psum,ihi,T(0.5f),func);
        if (ytry >= ysave) { // Didn't work
          _lti_debug4("  1D contraction didn't work, "
                      << "contracting in many dimensions" << std::endl);
          const vector<T>& iloRow = p.getRow(ilo);
          const typename vector<T>::const_iterator ebit = iloRow.end();
          for (int i=0;i<mpts;++i) {
            if (i != ilo) {
              typename vector<T>::iterator it;
              typename vector<T>::const_iterator bit;
              vector<T>& row = p.getRow(i);

              for (it=row.begin(),bit=iloRow.begin();
                   bit!=ebit;
                   ++it,++bit) {
                (*it) = static_cast<T>(0.5*((*it)+(*bit)));
              }
              
              y.at(i) = func.apply(row);

            }
          }

          // update number of evaluations
          nfunc += ndim;
          accumulateRows(p,psum);          
        }
      } else {
        --nfunc;
      }

    }

    return false;
  }

  template<typename T>
  void downhillSimplex::accumulateRows(const matrix<T> rows,
                                       vector<T>& acc) const {
    acc.copy(rows.getRow(0));
    for (int i=1;i<rows.rows();++i) {
      acc.add(rows.getRow(i));
    }
  }


  
}


