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

/** 
 * \file   ltiDataRepresentationType.cpp
 *         Contains the enum eDataRepresentationType.
 * \author Jochen Wickel
 * \author Peter Doerfler
 * \date   25.09.2000
 *
 * $Id: ltiDataRepresentationType.cpp,v 1.1 2005-07-19 18:23:25 gquiros Exp $
 */

#include "ltiDataRepresentationType.h"

namespace lti {

  /**
   * read function for eDataRepresentationType.
   *
   * @ingroup gStorable
   */
  bool read(ioHandler& handler,eDataRepresentationType& data){
    std::string str;
    lti::read(handler,"dataRepresentationType", str);
    if ((str=="Rows") || (str=="rows")) {
      data=Rows;
    } else if ((str=="Columns") || (str=="columns")) {
      data=Columns;
    } else {
      data=Rows; //default
      handler.setStatusString("Unknown dataRepresentationType ");
      handler.appendStatusString(str);
      handler.appendStatusString(" in eDataRepresentationType\n");
      return false;
    }
    return true;
  }

  /**
   * write function for eDataRepresentationType.
   *
   * @ingroup gStorable
   */
  bool write(ioHandler& handler,const eDataRepresentationType& data){
    switch (data) {
      case Rows:
        lti::write(handler,"dataRepresentationType","Rows");
        break;
      case Columns:
        lti::write(handler,"dataRepresentationType","Columns");
        break;
      default:
        lti::write(handler,"dataRepresentationType","Unknown");
        return false;
    }
    return true;
  }
  
}
