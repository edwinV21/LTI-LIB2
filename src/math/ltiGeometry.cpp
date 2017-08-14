/*
 * Copyright (C) 1998 - 2004
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
 * \file   ltiGeometry.cpp
 *         Explicit instantiations of functions defined in ltiGeometry.h
 * \author Pablo Alvarado
 * \date   19.03.2002
 *
 * $Id: ltiGeometry.cpp,v 1.1 2004-08-02 14:42:37 doerfler Exp $
 */


#include "ltiGeometry.h"
#include "ltiGeometry_template.h"

namespace lti {

  // explicit instantiations of functions for int, float and double

  template bool intersection<int>(const point<int>& p1,const point<int>& p2,
                                  const point<int>& p3,const point<int>& p4,
                                  point<int>& p);

  template bool intersection<float>(const point<float>& p1,
                                    const point<float>& p2,
                                    const point<float>& p3,
                                    const point<float>& p4,
                                    point<float>& p);

  template bool intersection<double>(const point<double>& p1,
                                     const point<double>& p2,
                                     const point<double>& p3,
                                     const point<double>& p4,
                                     point<double>& p);

  template int minDistanceSqr<int>(const point<int>& p1,
                                  const point<int>& p2,
                                  const point<int>& p3,
                                  point<int>& p);

  template float minDistanceSqr<float>(const point<float>& p1,
                                    const point<float>& p2,
                                    const point<float>& p3,
                                    point<float>& p);

  template double minDistanceSqr<double>(const point<double>& p1,
                                     const point<double>& p2,
                                     const point<double>& p3,
                                     point<double>& p);

  template int minDistanceSqr<int>(const point<int>& p1,
                                 const point<int>& p2,
                                 const point<int>& p3,
                                 const point<int>& p4,
                                 point<int>& pa,
                                 point<int>& pb);

  template float minDistanceSqr<float>(const point<float>& p1,
                                     const point<float>& p2,
                                     const point<float>& p3,
                                     const point<float>& p4,
                                     point<float>& pa,
                                     point<float>& pb);

  template double minDistanceSqr<double>(const point<double>& p1,
                                       const point<double>& p2,
                                       const point<double>& p3,
                                       const point<double>& p4,
                                       point<double>& pa,
                                       point<double>& pb);

  template int clockwiseTurn<int>(const point<int>& p0,
                                  const point<int>& p1,
                                  const point<int>& p2);

  template int clockwiseTurn<float>(const point<float>& p0,
                                    const point<float>& p1,
                                    const point<float>& p2);

  template int clockwiseTurn<double>(const point<double>& p0,
                                     const point<double>& p1,
                                     const point<double>& p2);

}

