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
 * \file   ltiIoObject.cpp
 * \author Benjamin Winkler
 * \date   14.12.2001
 *
 * $Id: ltiIoObject.cpp,v 1.4 2006-03-23 12:19:35 doerfler Exp $
 */

#include "ltiIoObject.h"

namespace lti {

  // default constructor
  ioObject::ioObject() : object() {
  }

  // destructor
  ioObject::~ioObject() {
  }

  bool ioObject::write(ioHandler& handler, const bool) const {
    std::string txt;
    txt = std::string("Write not implemented for ") + name();
    handler.setStatusString(txt);
    return false;
  }

  bool ioObject::read(ioHandler& handler,const bool) {
    std::string txt;
    txt = std::string("Read not implemented for ") + name();
    handler.setStatusString(txt);
    return false;
  }

  // read ioObject
  bool read(ioHandler& handler, ioObject& p, const bool complete) {
    return p.read(handler,complete);
  }

  // write ioObject
  bool write(ioHandler& handler, const ioObject& p, const bool complete) {
    return p.write(handler,complete);
  }
}

