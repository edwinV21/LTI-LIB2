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
 * \file   ltiChainCode.cpp
 *         Implements chainCode and constants for Canzler code
 * \author LTI
 * \date   19.08.1998
 *
 * $Id: ltiChainCode.cpp,v 1.3 2006-03-23 12:19:35 doerfler Exp $
 */

#include "ltiChainCode.h"

namespace lti {

  // static members of chainCode
  const ubyte chainCode::Nothing = 0;
  const ubyte chainCode::Top     = 1;
  const ubyte chainCode::Right   = 2;
  const ubyte chainCode::Bottom  = 4;
  const ubyte chainCode::Left    = 8;

  /*
   * read the chainCode element from the given ioHandler.  The
   * complete flag will be ignored
   */
  bool read(ioHandler& handler,chainCode& cc,const bool) {
    bool b;
    std::string symb;
    b = handler.readSymbol(symb);

    if (b) {
      if (symb == "noMove") {
        cc = chainCode::NOMOVE;
      } else if (symb == "E") {
        cc = chainCode::E;
      } else if (symb == "NE") {
        cc = chainCode::NE;
      } else if (symb == "N") {
        cc = chainCode::N;
      } else if (symb == "NW") {
        cc = chainCode::NW;
      } else if (symb == "W") {
        cc = chainCode::W;
      } else if (symb == "SW") {
        cc = chainCode::SW;
      } else if (symb == "S") {
        cc = chainCode::S;
      } else if (symb == "SE") {
        cc = chainCode::SE;
      } else {
        cc == chainCode::UNKNOWN;
      }
    }

    return b;
  }

  /*
   * write the matrix in the given ioHandler.  The complete flag indicates
   * if the enclosing begin and end should be also be written or not
   */
  bool write(ioHandler& handler,const chainCode& cc,const bool) {
    bool b;
    switch (cc.getDirection()) {
        case chainCode::NOMOVE:
          b = handler.writeSymbol("noMove");
          break;
        case chainCode::E:
          b = handler.writeSymbol("E");
          break;
        case chainCode::NE:
          b = handler.writeSymbol("NE");
          break;
        case chainCode::N:
          b = handler.writeSymbol("N");
          break;
        case chainCode::NW:
          b = handler.writeSymbol("NW");
          break;
        case chainCode::W:
          b = handler.writeSymbol("W");
          break;
        case chainCode::SW:
          b = handler.writeSymbol("SW");
          break;
        case chainCode::S:
          b = handler.writeSymbol("S");
          break;
        case chainCode::SE:
          b = handler.writeSymbol("SE");
          break;
        default:
          b = handler.writeSymbol("unknown");
    }
    return b;
  }


}
