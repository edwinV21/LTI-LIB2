/*
 * Copyright (C) 2008
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
 * \file   ltiPseudoInverseMP_template.h
 *         Contains the class lti::pseudoInverseMP, 
 *         which has still to be documented by Pablo Alvarado
 * 
 * \author Pablo Alvarado
 * \date   24.03.2008
 *
 * revisions ..: $Id: ltiPseudoInverseMP_template.h,v 1.1 2008-03-25 17:02:18 alvarado Exp $
 */

#include "ltiLuSolution.h"

namespace lti {

  // calculate MP pseudo inverse NO smoothing
  template<typename T>
  matrix<T>& pseudoInverseMP::apply(const matrix<T>& src,
                                          matrix<T>& dest) const {

    matrix<T> dummy(src.columns(),src.columns(),0.);

    typename vector<T>::const_iterator iti , itj;
    typename vector<T>::const_iterator itie, itje;
    typename matrix<T>::iterator itd;

    int i,j,k;

    // src transposed times src
    for (k=0; k<src.rows(); ++k) {
      itd=dummy.begin();
      for (i=0, iti=src.getRow(k).begin(), itie=src.getRow(k).end();
           iti!=itie;
           iti++, i++) {

        itd+=i;
        itj=src.getRow(k).begin();
        for (itj+=i, itje=src.getRow(k).end();
             itj!=itje; 
             itj++) {

          *itd+=(*itj)*(*iti);
          itd++;

        }
      }
    }

    for (i=1; i<dummy.rows(); ++i) {
      for (j=0; j<i; ++j) {
        dummy.at(i,j)=dummy.at(j,i);
      }
    }

    luSolution<T> luSol;
    typename luSolution<T>::parameters luSolPar;

    // do not copy (dummy could be huge!)
    luSolPar.systemMatrix.useExternData(dummy.rows(),
                                        dummy.columns(),
                                        &dummy.at(0,0));

    luSol.setParameters(luSolPar);

    dest=src;
    for (i=0; i<src.rows(); ++i) {
      luSol.apply(dest.getRow(i));
    }

    dest.transpose();

    return dest;
  }

  template<class T>
  matrix<T>& pseudoInverseMP::apply(const matrix<T>& src,
                                    const matrix<T>& smooth,
                                          matrix<T>& dest) const {


    const parameters& param=getParameters();

    matrix<T> dummy(src.columns(),src.columns(),0.);

    typename vector<T>::const_iterator iti ,itj;
    typename vector<T>::const_iterator itie, itje;
    typename matrix<T>::iterator itd;

    int i,j,k;

    // src transposed times src
    for (k=0; k<src.rows(); k++) {
      itd=dummy.begin();
      for (i=0, iti=src.getRow(k).begin(), itie=src.getRow(k).end();
           iti!=itie;
           iti++, i++) {
        itd+=i;
        itj=src.getRow(k).begin();
        for (itj+=i, itje=src.getRow(k).end();
             itj!=itje;
             itj++) {
          *itd+=(*itj)*(*iti);
          itd++;
        }
      }
    }

    for (i=1; i<dummy.rows(); i++) {
      for (j=0; j<i; j++) {
        dummy.at(i,j)=dummy.at(j,i);
      }
    }

    dummy.addScaled(param.lambda, smooth);

    luSolution<T> luSol(dummy);

    dest=src;

    for (i=0; i<src.rows(); ++i) {
      luSol.apply(dest.getRow(i));
    }

    dest.transpose();

    return dest;
  }
}


