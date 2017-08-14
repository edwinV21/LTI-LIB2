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
 * \file   ltiObject.cpp
 * \author Pablo Alvarado
 * \date   01.04.1999
 *
 * $Id: ltiObject.cpp,v 1.4 2005-01-03 16:09:42 alvarado Exp $
 */

#include "ltiObject.h"
#include "ltiClassName.h"

namespace lti {
  object::object() {
  }

  object::~object() {
  }

//   const std::string& object::name() const {
//     _LTI_RETURN_CLASS_NAME
//   }

  std::string object::buildName() const {
    return className::get(*this);
  }

//   object* object::clone() const {
//     return new object();
//   }

//   object* object::newInstance() const {
//     return new object();
//   }
}

