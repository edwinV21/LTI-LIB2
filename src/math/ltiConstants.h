/*
 * Copyright (C) 1998 - 2004
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
 * \file   ltiConstants.h
 *         Definition of some often needed mathematical constants as
 *         static member functions of a template class: pi(), twoPi(),
 *         halfPi(), e(), and sqrt2().
 * \author Pablo Alvarado
 * \date   16.05.2001
 *
 * $Id: ltiConstants.h,v 1.3 2005-01-03 16:17:50 alvarado Exp $
 */

#ifndef _LTI_CONSTANTS_H_
#define _LTI_CONSTANTS_H_

namespace lti {

  /**
   * This class is a container of some mathematical and physical
   * constants as static member functions.
   *
   * Example:
   * \code
   * constants<float>::pi();
   * \endcode
   */
  template<class T>
  class constants {
  public:
    /**
     * Pi
     */
    static inline const T pi() throw() {
      return static_cast<T>(3.1415926535897932384626433832795);
    };

    /**
     * Twice Pi (2*pi())
     */
    static inline const T twoPi() throw() {
      return static_cast<T>(3.1415926535897932384626433832795*2.0);
    };

    /**
     * Half Pi (pi()/2)
     */
    static inline const T halfPi() throw() {
      return static_cast<T>(3.1415926535897932384626433832795/2.0);
    };

    /**
     * e (basis of natural logarithm) = 2.7182818284590452353602874713526...
     */
    static inline const T e() throw() {
      return static_cast<T>(2.7182818284590452353602874713526);
      // continuing with ... 62497757247093699959574966967627724076630353547594571382178525166427427466391932003059921817413596629043572900334295260
      // more at http://sources.wikipedia.org/wiki/E_to_10%2C000_places
    };

    /**
     * square root of 2 = 1.4142135623730950488016887242097...
     */
    static inline const T sqrt2() throw() {
      return static_cast<T>(1.4142135623730950488016887242097);
    };

  private:
    /**
     * Private constructor avoids the creation of
     * an instance of this class.
     */
    constants() {};
  };
}


#endif

