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
 * \file   ltiSingularValueDecomposition_inline.h
 *         Compute the SVD of a matrix
 * \author Xin Gu
 * \date   11.01.2001
 *
 * $Id: ltiSingularValueDecomposition.cpp,v 1.5 2006-09-07 13:38:37 doerfler Exp $
 */

#include "ltiMacroSymbols.h"
#include "ltiSingularValueDecomposition.h"
#include "ltiSingularValueDecomposition_template.h"

#ifdef HAVE_LAPACK
#include "clapack.h"
#endif

namespace lti {

#ifdef HAVE_LAPACK

  template<>
  int singularValueDecomposition<float>::gesvd(char* jobu, char* jobvt,
					       integer* m, integer* n, 
                                               float* a, integer* lda,
					       float* s,
                                               float* u, integer* ldu,
                                               float* vt, integer* ldvt,
					       float* work, integer* lwork,
					       integer* info) const {
    lockInterface();
    int tmp=LA_SGESVD(jobu,jobvt,m,n,a,lda,
                      s,u,ldu,vt,ldvt,
                      work,lwork,info);
    unlockInterface();
    return tmp;
  }
  
  template<>
  int singularValueDecomposition<double>::gesvd(char* jobu, char* jobvt,
						integer* m, integer* n, 
                                                double* a, integer* lda,
						double* s,
                                                double* u, integer* ldu,
						double* vt, integer* ldvt,
						double* work, integer* lwork,
						integer* info) const {
    lockInterface();
    int tmp=LA_DGESVD(jobu,jobvt,m,n,a,lda,
                      s,u,ldu,vt,ldvt,
                      work,lwork,info);
    unlockInterface();
    return tmp;
  }
  
  template<>
  int singularValueDecomposition<float>::gesdd(char* jobz, 
                                               integer* m, integer* n,
                                               float* a, integer* lda,
					       float* s, 
                                               float* u, integer* ldu,
                                               float* vt, integer* ldvt,
					       float* work, integer* lwork, 
                                               integer* iwork, integer* info) const {
    lockInterface();
    int tmp=LA_SGESDD(jobz,m,n,a,lda,
                      s,u,ldu,vt,ldvt,
                      work,lwork,iwork,info);
    unlockInterface();
    return tmp;
  }
  
  template<>
  int singularValueDecomposition<double>::gesdd(char* jobz, 
                                                integer* m, integer* n,
                                                double* a, integer* lda,
						double* s, 
                                                double* u, integer* ldu,
						double* vt, integer* ldvt,
						double* work, integer* lwork, 
                                                integer* iwork,integer* info) const {
    lockInterface();
    int tmp=LA_DGESDD(jobz,m,n,a,lda,
                      s,u,ldu,vt,ldvt,
                      work,lwork,iwork,info);
    unlockInterface();
    return tmp;
  }
  
#endif

  // explicit instantiations

  template class singularValueDecomposition<float>;
  template class singularValueDecomposition<double>;

}
