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
 * \file   ltiIoHandler_template.h
 *         Contains the definitions of global read/write functions for class T
 * \author Pablo Alvarado
 * \date   07.12.2000
 *
 * $Id: ltiIoHandler_template.h,v 1.1 2004-08-12 11:48:43 doerfler Exp $
 */

namespace lti {

  /**
   * Handler functions with standard storable interface
   */
  template <class T>
  bool write(ioHandler& handler,const std::string& name,
             const T& data,const bool complete) {

    if (complete) {
      handler.writeBegin();
    }
    handler.writeSymbol(name);
    handler.writeKeyValueSeparator();
    write(handler,data);
    if (complete) {
      handler.writeEnd();
    }
    return handler.writeEOL();
  }

  /**
   * Handler functions with standard storable interface
   */
  template <class T>
  bool read(ioHandler& handler,const std::string& name,
            T& data,const bool complete) {
    int level = handler.getLevel();
    bool result = true;

    if (complete) {
      result = handler.readBegin();
    }

    if (result && handler.trySymbol(name)) {
      result = result && handler.readKeyValueSeparator();

      result = result && read(handler,data);

      if (complete) {
        result = result && handler.readEnd();
      }

      while (result && (handler.getLevel() > level)) {
        result = result && handler.readEnd();
      }

    } else {
      result=false;
      handler.appendStatusString("\nSymbol " + name + " not found: ");
      handler.appendContextStatus();
      handler.restoreLevel();
    }

    return result;
  }

}
