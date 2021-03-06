/*
 * Copyright (C) 1998-2006
 * Lehrstuhl fuer Technische Informatik, RWTH-Aachen, Germany
 * Instituto Tecnológico de Costa Rica, Costa Rica
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
 * \file   ltiEndianness.cpp
 *         Contains the class endianness, which allows to read/write data from
 *         a file written with a known endianness.
 * \author Pablo Alvarado
 * \date   19.04.1999
 *
 */

#include "ltiEndianness.h"
#include "ltiEndianness_template.h"

namespace lti {
  template class endianness<LittleEndian>;
  template class endianness<BigEndian>;
}
