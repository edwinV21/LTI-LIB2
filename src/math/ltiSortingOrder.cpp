/*
 * Copyright (C) 1998-2005
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
 * \file   ltiSortingOrder.h
 *         Contains the enum eSortingOrder for specifying sorting
 *         order. IO functions are provided as well.
 * \author Peter Doerfler
 * \date   06.04.2005
 *
 * $Id: ltiSortingOrder.cpp,v 1.1 2005-04-26 15:44:01 doerfler Exp $
 */

#include "ltiSortingOrder.h"


namespace lti {

  // read function for eSortingOrder.
  bool read(ioHandler& handler,eSortingOrder& data) {

    std::string str;
    if (handler.read(str)) {
   
      if ((str == "Ascending") ||
          (str == "ascending")) {
        data = Ascending;
      } else if ((str == "Descending") ||
                 (str == "descending")) {
        data = Descending;
      } else {
        data = Ascending;
        handler.setStatusString("undefined eSortingOrder");
        return false;
      }

      return true;
    }

    handler.setStatusString("could not read std::string from stream");
    return false;
  }

  // write function for eSortingOrder.
  bool write(ioHandler& handler,const eSortingOrder& data) {
    bool b=false;
    switch(data) {
    case Ascending:
      b=handler.write("Ascending");
      break;
    case Descending:
      b=handler.write("Descending");
      break;
    default:
      b=false;
      handler.setStatusString("undefined eSortingOrder");
      handler.write("Unknown");
    }

    return b;
  }


}
