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


/*
 * \file   ltiChainCode_inline.h
 *         Implements inline functions of chainCode
 * \author LTI
 * \date   19.08.1998
 *
 * $Id: ltiChainCode_inline.h,v 1.3 2005-03-16 16:20:48 doerfler Exp $
 */

namespace lti {

  inline void chainCode::fromDeltas(const int x,const int y) {
    if (x == 0) {
      if (y == 0) {
	value = NOMOVE;
      } else if (y >  0) {
	value = S;
      } else if (y <  0) {
	value = N;
      }
    } else if (x >  0) {
      if (y == 0) {
	value = E;
      } else if (y >  0) {
	value = SE;
      } else if (y <  0) {
	value = NE;
      }
    } else if (x <  0) {
      if (y >  0) {
	value = SW;
      } else if (y == 0) {
	value = W;
      } else if (y <  0) {
	value = NW;
      }
    }
  }

  chainCode::direction chainCode::getDirection() const {
    return value;
  }

  bool chainCode::isDirection() const {
    return (int(value)>=0);
  }

  ipoint chainCode::getNext(const ipoint& here) const {
    return (delta()+here);
  }


  ipoint chainCode::getPrevious(const ipoint& here) const {
    return (delta()-here);
  }

  int chainCode::deltaX() const {
    if ((value>=NW) && (value<=SW))
      return -1;
    if ((value==E) || (value==NE) || (value==SE))
      return 1;
    return 0;
  }

  int chainCode::deltaY() const {
    if (value>=SW) { // && (value<=SE) (always true since value is always <=SE
      return 1;
    }
    if ((value>=NE) && (value<=NW)) {
      return -1;
    }
    return 0;
  }

  ipoint chainCode::delta() const {
    return ipoint(deltaX(),deltaY());
  }

  chainCode& chainCode::copy(const chainCode& other) {
    value = other.getDirection();
    return (*this);
  }

  chainCode& chainCode::copy(const direction& d) {
    value = d;
    return (*this);
  }

  bool chainCode::compare(const chainCode& other) {
    return (value == other.getDirection());
  }

  bool chainCode::compare(const direction& other) {
    return (value == other);
  }

  chainCode& chainCode::subtract(const chainCode& first,
				 const chainCode& second) {
    value = direction((8+int(first.getDirection())
                        -int(second.getDirection()))%8);
    return *this;
  }

  chainCode& chainCode::subtract(const chainCode& other) {
    value = direction((8+int(value)
 		        -int(other.getDirection()))%8);
    return (*this);
  }

  chainCode& chainCode::add(const chainCode& first,
			    const chainCode& second) {
    value = direction((int(first.getDirection())+
                       int(second.getDirection()))%8);
    return *this;
  }

  chainCode& chainCode::add(const chainCode& other) {
    value = direction((int(value)
 		      +int(other.getDirection()))%8);
    return (*this);
  }

  /*
   * return the Canzler Code for the pixel pointed by this chainCode
   * if the second chainCode is the one given as parameter
   * @param nextChainCode the next chain code of this one in the chain
   *                      code sequence.
   * @return the canzler bit-coding for the edges of the pixel pointed
   *         by this chainCode which are part of the boundary.  The values
   *         returned are bit or of the constants Left, Top, Right and
   *         Bottom.  If the combination is invalid, "Nothing" will be returned
   */
  ubyte chainCode::getCanzlerCode(const chainCode& nextChainCode) const {
    // Look-Up Table to find efficiently the Canzler-Code
    static const ubyte markarray[8][8]={{ 1, 1, 0, 0, 7, 7, 3, 3},  // E
                                        { 9, 9, 8, 8, 0,15,11,11},  // NE
                                        { 9, 9, 8, 8, 0, 0,11,11},  // N
                                        {13,13,12,12, 4, 4, 0,15},  // NW
                                        {13,13,12,12, 4, 4, 0, 0},  // W
                                        { 0,15,14,14, 6, 6, 2, 2},  // SW
                                        { 0, 0,14,14, 6, 6, 2, 2},  // S
                                        { 1, 1, 0,15, 7, 7, 3, 3}}; // SE
    return
    markarray[static_cast<int>(value)][static_cast<int>(nextChainCode.value)];
  }

}
