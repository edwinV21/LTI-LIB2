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
 * \file   ltiEigenSystem.cpp
 *         Contains the template class EigenSystem<T>
 * \author Thomas Rusert
 * \date   16.06.99
 *
 * revisions ..: $Id: ltiEigenSystem.cpp,v 1.3 2010-10-11 15:35:26 alvarado Exp $
 */

#include "ltiEigenSystem.h"
#include "ltiEigenSystem_template.h"

#ifdef HAVE_LAPACK
#  include "clapack.h"
#endif

namespace lti {

#ifdef HAVE_LAPACK
  template<>
  int eigenSystem<float>::geev(char* jobvl,char* jobvr, 
                               integer* n,
                               float* a,
                               integer* lda, float* wr, float* wi,
                               float* vl, integer* ldvl, 
                               float* vr, integer* ldvr,
                               float* work, integer* lwork,
                               integer* info) const {

    assert(notNull(jobvl));
    assert(notNull(jobvr));
    assert(notNull(n));
    assert(notNull(a));
    assert(notNull(lda));
    assert(notNull(wr));
    assert(notNull(wi));
    assert(notNull(vl));
    assert(notNull(ldvl));
    assert(notNull(vr));
    assert(notNull(ldvr));
    assert(notNull(work));
    assert(notNull(lwork));
    assert(notNull(info));

    lockInterface();
    int tmp=LA_SGEEV(jobvl, jobvr, n,a,lda,wr,wi,vl,ldvl,vr,ldvr,
                     work,lwork,info);
    unlockInterface();
    return tmp;
  }

  template<>
  int eigenSystem<double>::geev(char* jobvl, char* jobvr,
                                integer* n,
                                double* a, integer* lda, double* wr,
                                double* wi, double* vl, integer* ldvl,
                                double* vr, integer* ldvr, 
                                double* work,
                                integer* lwork, integer* info) const {

    assert(notNull(jobvl));
    assert(notNull(jobvr));
    assert(notNull(n));
    assert(notNull(a));
    assert(notNull(lda));
    assert(notNull(wr));
    assert(notNull(wi));
    assert(notNull(vl));
    assert(notNull(ldvl));
    assert(notNull(vr));
    assert(notNull(ldvr));
    assert(notNull(work));
    assert(notNull(lwork));
    assert(notNull(info));

    lockInterface();
    int tmp=LA_DGEEV(jobvl,jobvr,n,a,lda,wr,wi,vl,ldvl,vr,ldvr,work,lwork,info);
    unlockInterface();
    return tmp;
  }

#endif // HAVE_LAPACK

  template class eigenSystem<float>;
  template class eigenSystem<double>;
} // namespace lti

