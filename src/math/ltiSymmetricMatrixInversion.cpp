/*
 * Copyright (C) 1998, 1999, 2000, 2001, 2002, 2003
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
 * \file   ltiSymmetricMatrixInversion.cpp
 *         Inversion of symmetric matrices.
 * \author Peter Doerfler
 * \date   02.08.2003
 *
 * $Id: ltiSymmetricMatrixInversion.cpp,v 1.6 2007-12-26 04:49:23 alvarado Exp $
 */

#include "ltiSymmetricMatrixInversion.h"
#include "ltiSymmetricMatrixInversion_template.h"

#ifdef HAVE_LAPACK
#include "clapack.h"
#endif

#include "ltiDebug.h"

namespace lti {

#ifdef HAVE_LAPACK
  
  template<>
  int symmetricMatrixInversion<float>::potri(char *uplo, integer* n, 
                                             float* a, integer* lda, 
                                             integer* info) const {
    lockInterface();
    int tmp=LA_SPOTRI(uplo,n,a,lda,info);
    unlockInterface();
    return tmp;
  }

  template<>
  int symmetricMatrixInversion<double>::potri(char *uplo, integer* n, 
                                              double* a, integer* lda,
                                              integer* info) const {
    lockInterface();
    int tmp=LA_DPOTRI(uplo,n,a,lda,info);
    unlockInterface();
    return tmp;
  }

#endif

  // explicit instantiations
  template class symmetricMatrixInversion<float>;
  template class symmetricMatrixInversion<double>;

} // namespace lti
