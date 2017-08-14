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
 * \file   ltiPointList.cpp
 *         Defines a class for lists of points
 * \author Suat Akyol
 * \date   28.06.2000
 *
 * $Id: ltiPointList.cpp,v 1.3 2009-06-04 21:44:50 alvarado Exp $
 */

#include "ltiPointList.h"
#include "ltiPointList_template.h"

namespace lti {
  // explicit instantiations
  template class pointList<int>;
  template class pointList<float>;
  template class pointList<double>;

  template bool read(ioHandler& handler,pointList<int>&,const bool);
  template bool read(ioHandler& handler,pointList<float>&,const bool);
  template bool read(ioHandler& handler,pointList<double>&,const bool);

  template bool write(ioHandler& handler,const pointList<int>&,const bool);
  template bool write(ioHandler& handler,const pointList<float>&,const bool);
  template bool write(ioHandler& handler,const pointList<double>&,const bool);

} // namespace lti

namespace std {
  template ostream& operator<<(ostream& s,const lti::pointList<int>&);
  template ostream& operator<<(ostream& s,const lti::pointList<float>&);
  template ostream& operator<<(ostream& s,const lti::pointList<double>&);
}

