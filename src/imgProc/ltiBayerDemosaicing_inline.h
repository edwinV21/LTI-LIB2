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
 * \file   ltiBayerDemosaicing.h
 *         Contains the class lti::bayerDemosaicing which is the base
 *         class for all types of Bayer pattern demosaicing
 * \author Peter Doerfler
 * \date   10.12.2004
 *
 * revisions ..: $Id: ltiBayerDemosaicing_inline.h,v 1.1 2004-12-20 11:50:11 doerfler Exp $
 */

namespace lti {

  ubyte bayerDemosaicing::clip(const double& val) const {
    return static_cast<ubyte>( val<0. ? 0. : val>255. ? 255. : val);
  } 

}
