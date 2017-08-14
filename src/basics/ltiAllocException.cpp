/*
 * Copyright (C) 2006
 * Pablo Alvarado, Instituto Tecnológico de Costa Rica
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
 * \file   ltiAllocException.cpp
 *         Defines the memory allocation exception
 * \author Pablo Alvarado
 * \date   15.09.2006
 *
 * $Id: ltiAllocException.cpp,v 1.1 2007-04-04 19:18:14 alvarado Exp $
 */

#include "ltiAllocException.h"

namespace lti {

  allocException::allocException() : exception("memory allocation error") {
  }    

  const std::string& allocException::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  allocException* allocException::clone() const {
    return (new allocException());
  }

  allocException* allocException::newInstance() const {
    return (new allocException());
  }
}

