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
 * \file   ltiSymmetricEigenSystem.cpp
 *         Contains the template class symmetricEigenSystem<T>
 * \author Thomas Rusert
 * \date   16.06.99
 *
 * revisions ..: $Id: ltiSymmetricEigenSystem.cpp,v 1.3 2009-07-26 16:34:22 alvarado Exp $
 */

#include "ltiSymmetricEigenSystem.h"
#include "ltiSymmetricEigenSystem_template.h"

#ifdef HAVE_LAPACK
#  include "clapack.h"
#endif

namespace lti {

#ifdef HAVE_LAPACK
  template<>
  int symmetricEigenSystem<float>::evr(char* jobz, char* range, char* uplo,
                                       integer* n, float* a, integer* lda,
                                       float* vl, float* vu,
                                       integer* il, integer* iu,
                                       float* abstol,
                                       integer* m, float* w,
                                       float* z, integer* ldz, integer* isuppz,
                                       float* work, integer* lwork,
                                       integer* iwork, integer* liwork,
                                       integer* info) const {
    lockInterface();

    assert(notNull(jobz));
    assert(notNull(range));
    assert(notNull(uplo));
    assert(notNull(n));
    assert(notNull(a));
    assert(notNull(lda));
    assert(notNull(vl));
    assert(notNull(vu));
    assert(notNull(il));
    assert(notNull(iu));
    assert(notNull(abstol));
    assert(notNull(m));
    assert(notNull(w));
    assert(notNull(z));
    assert(notNull(ldz));
    assert(notNull(isuppz));
    assert(notNull(work));
    assert(notNull(lwork));
    assert(notNull(iwork));
    assert(notNull(liwork));
    assert(notNull(info));

    int tmp=LA_SSYEVR(jobz,range,uplo,n,a,lda,vl,vu,il,iu,abstol,
                      m,w,z,ldz,isuppz,work,lwork,iwork,liwork,info);
    unlockInterface();
    return tmp;
  }

  template<>
  int symmetricEigenSystem<double>::evr(char* jobz, char* range,char* uplo,
                                        integer* n, double* a, integer* lda,
                                        double* vl, double* vu,
                                        integer* il, integer* iu,
                                        double* abstol,
                                        integer* m, double* w,
                                        double* z, integer* ldz, 
                                        integer* isuppz,
                                        double* work, integer* lwork,
                                        integer* iwork, integer* liwork,
                                        integer* info) const {
    lockInterface();
    
    assert(notNull(jobz));
    assert(notNull(range));
    assert(notNull(uplo));
    assert(notNull(n));
    assert(notNull(a));
    assert(notNull(lda));
    assert(notNull(vl));
    assert(notNull(vu));
    assert(notNull(il));
    assert(notNull(iu));
    assert(notNull(abstol));
    assert(notNull(m));
    assert(notNull(w));
    assert(notNull(z));
    assert(notNull(ldz));
    assert(notNull(isuppz));
    assert(notNull(work));
    assert(notNull(lwork));
    assert(notNull(iwork));
    assert(notNull(liwork));
    assert(notNull(info));

    int tmp=LA_DSYEVR(jobz,range,uplo,n,a,lda,vl,vu,il,iu,abstol,
                      m,w,z,ldz,isuppz,work,lwork,iwork,liwork,info);

    
    unlockInterface();
    return tmp;
  }

  template<>
  void symmetricEigenSystem<float>::lamch(char* cmach,
                                          float& val) const {
    lockInterface();

    assert(notNull(cmach));

    val=static_cast<float>(LA_SLAMCH(cmach));

    unlockInterface();
  }

  template<>
  void symmetricEigenSystem<double>::lamch(char* cmach,
                                           double& val) const {
    lockInterface();

    assert(notNull(cmach));

    val=static_cast<double>(LA_DLAMCH(cmach));

    unlockInterface();
  }

#endif // HAVE_LAPACK

  template class symmetricEigenSystem<float>;
  template class symmetricEigenSystem<double>;
} // namespace lti

