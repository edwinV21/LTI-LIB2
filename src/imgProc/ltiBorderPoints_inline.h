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
 * \file   ltiBorderPoints_inline.h
 *         Implements inline functions of borderPoints
 * \author LTI
 * \date   19.08.1998
 *
 * $Id: ltiBorderPoints_inline.h,v 1.5 2007-09-24 04:01:11 alvarado Exp $
 */

namespace lti {

  inline borderPoints& borderPoints::operator=(const borderPoints& other) {
    return copy(other);
  }

  inline bool borderPoints::inside(const ipoint& p,const matrix<ubyte>& mask) {
    return ((p.x>=0) && (p.x<mask.columns()) &&
            (p.y>=0) && (p.y<mask.rows()));
  }

}



