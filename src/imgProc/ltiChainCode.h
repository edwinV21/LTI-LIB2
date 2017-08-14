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
 * \file   ltiChainCode.h
 *         Declares chainCode and constants for Canzler code
 * \author LTI
 * \date   19.08.1998
 *
 * $Id: ltiChainCode.h,v 1.5 2005-06-06 12:59:56 doerfler Exp $
 */

#ifndef LTI_CHAIN_CODE_H
#define LTI_CHAIN_CODE_H

#include "ltiIoHandler.h"
#include "ltiPoint.h"

namespace lti {

  /**
   * Elements of a Chaincode.
   * The used directions follows the next diagram:
   *
   * \code
   * NW   N   NE
   *    \ | /
   * W -  x - E
   *    / | \
   * SW   S   SE
   * \endcode
   *
   * The "int" value resulting from casting these chain codes is
   * proportional to the angle with a proportionality constant of 45°,
   * i.e. if 'c' is a chainCode object, then the equivalent direction
   * in degrees is 45*int(c.getDirection()).
   *
   * @ingroup gShape
   */
  class chainCode {
  public:
    /**
     * Directions used in chain codes
     * The used directions follows the next diagram:
     *
     * \code
     * NW   N   NE
     *    \ | /
     * W -  x - E
     *    / | \
     * SW   S   SE
     * \endcode
     *
     * The "int" value resulting from casting these chain codes is
     * proportional to the angle with a proportionality constant of 45°,
     * i.e. if 'c' is a chainCode object, then the equivalent direction
     * in degrees is 45*int(c.getDirection()).
     */
    enum direction {UNKNOWN=-2, /**< Unknown */
                    NOMOVE=-1,  /**< No Move */
                    E=0,        /**< East (or Right) */
                    NE,         /**< North-East (or Right-Up) */
                    N,          /**< North (or Up) */
                    NW,         /**< North-West (or North-West) */
                    W,          /**< West (or West) */
                    SW,         /**< South-West (or Left-Down) */
                    S,          /**< South (or Down) */
                    SE          /**< South-East (or Right-Down) */
    };

    /**
     * @name Canzler Codes
     *
     * Given two chain codes for a sequence of three adjacent pixels,
     * the Canzler-Codes give you information about which borders of the middle
     * pixel belong to the boundary.  With the getCanzlerCode() you can obtain
     * the corresponding coding for the pixel pointed by this chain code, if
     * the next chain code is the one given.  They are somehow similar to the
     * so called "chain crack codes", but instead of specifying a direction
     * of the boundary, they code which "crack codes" belong to the middle
     * pixel between two chain codes.
     *
     * There are 16 Canzler-Codes, that can be obtained by the combination
     * of the four values Top, Right, Bottom and Left, which specify that
     * the border runs on the top, right, bottom and/or left edge of the
     * pixel respectively.
     */
    //@{
    static const ubyte Nothing; /// Nothing has the value 0
    static const ubyte Top;     /// Top has value 1 (the first bit)
    static const ubyte Right;   /// Right has value 2 (the second bit)
    static const ubyte Bottom;  /// Bottom has value 4 (the third bit)
    static const ubyte Left;    /// Left has value 8 (the fourth bit)
    //@}

    /**
     * default constructor
     */
    chainCode(const direction& v = NOMOVE)
      : value(v) {
    };

    /**
     * constructor to cast an integer
     */
    chainCode(const int v) : value(direction(v%8)) {};

    /**
     * copy constructor
     */
    chainCode(const chainCode& other)
      : value(other.value) {
    };

    /** Constrcutor. This constructor creates a chainCode object using the
	sign of the parameters to calculate the direction.
	Image-coordinates are used, i.e. a positive "y" implies a
	change towards "south", and a positive x implies a
	change towards "east". */
    chainCode(const int x,const int y) : value(UNKNOWN) {
      fromDeltas(x,y);
    };

    /**
     * Constructor.This constructor creates a chainCode object using
     * two points
     */
    chainCode(const ipoint& here,const ipoint& next) : value(UNKNOWN) {
      ipoint tmp(next);
      tmp.subtract(here);
      fromDeltas(tmp.x,tmp.y);
    };

    /**
     * returns direction of this chain element
     */
    inline direction getDirection() const;

    /**
     * Possible Direction.returns true if this object contains one of
     * the eight posible directions, and false otherwise
     */
    inline bool isDirection() const;

    /**
     * returns next point with start point "here" and using this chainCode.
     */
    inline ipoint getNext(const ipoint& here) const;

    /**
     * returns previous point with start point "here" and using this chainCode.
     */
    inline ipoint getPrevious(const ipoint& here) const;

    /**
     * returns change in x.
     * For example, if the value is NW, deltaX() returns -1.
     */
    inline int deltaX() const;

    /**
     * returns change in y.
     * For example, if the value is NW, deltaY() returns -1.
     */
    inline int deltaY() const;

    /**
     * returns point with changes in both direction, x and y.
     */
    inline ipoint delta() const;

    /**
     * become other object
     */
    inline chainCode& operator=(const direction& other) {
      return copy(other);
    };

    /**
     * become other object
     */
    inline chainCode& copy(const direction& other);

    /**
     * become other object
     */
    inline chainCode& copy(const chainCode& other);

    /**
     * become other object
     */
    inline chainCode& operator=(const chainCode& other) {
      return(copy(other));
    };

    /**
     * compare with another object
     */
    inline bool compare(const chainCode& other);

    /**
     * compare with another object
     */
    inline bool operator==(const chainCode& other) {
      return compare(other);
    };

    /**
     * compare with another object
     */
    inline bool compare(const direction& other);

    /**
     * compare with another object
     */
    inline bool operator==(const direction& other) {
      return compare(other);
    };

    /**
     * subtract the angles of the first and second chain code elements and
     * leave the result here!
     */
    inline chainCode& subtract(const chainCode& first,
                               const chainCode& second);
    /**
     * subtract the angle equivalent of the <em>other</em> chainCode
     * from *this
     */
    inline chainCode operator-(const chainCode& other) const {
      chainCode tmp;
      tmp.subtract(*this,other);
      return tmp;
    };

    /**
     * subtract the angle equivalent of the other chain code from this one.
     * The result will be left in this instance, and a reference to it will
     * be returned.
     */
    inline chainCode& subtract(const chainCode& other);

    /**
     * subtract another chain code
     */
    inline chainCode& operator-=(const chainCode& other) {
      return subtract(other);
    };

    /**
     * add the angles of the first and second chain codes
     */
    inline chainCode& add(const chainCode& first,
                          const chainCode& second);

    /**
     * add the angles of this and the other chain code
     */
    inline chainCode operator+(const chainCode& other) const {
      chainCode tmp;
      tmp.add(*this,other);
      return tmp;
    };

    /**
     * add the angles of the other chain code to this chainCode and
     * leave the result in this instance.  Returns a reference to the instance
     */
    inline chainCode& add(const chainCode& other);

    /**
     * add another chain code
     */
    inline chainCode& operator+=(const chainCode& other) {
      return add(other);
    };

    /**
     * divide chain code equivalent angle with an integer.  The result
     * will be left in this instance and a reference to it will be returned
     */
    inline chainCode& divide(const int other) {
      value=static_cast<direction>(static_cast<int>(value)/other);
      return (*this);
    };

    /**
     * divide chain code angle with integer.
     */
    inline chainCode operator/(const int other) {
      chainCode tmp(*this);
      tmp.divide(other);
      return (tmp);
    }

    /**
     * return the Canzler Code for the pixel pointed by this chainCode
     * if the second chainCode is the one given as parameter.
     *
     * The result a the bitwise OR of some of the codes Top, Bottom, Left and
     * Right.
     *
     * @param nextChainCode the next chain code of this one in the chain
     *                      code sequence.
     * @return the canzler bit-coding for the edges of the pixel pointed
     *         by this chainCode which are part of the boundary.  The values
     *         returned are bit or of the constants Left, Top, Right and
     *         Bottom.
     */
    inline ubyte getCanzlerCode(const chainCode& nextChainCode) const;

  protected:
    /**
     * Attribute containing the chain code
     */
    direction value;

    /**
     * compute chain code from x and y deltas
     */
    inline void fromDeltas(const int x,const int y);
  };

  //***********************************************************
  // Global IO functions
  //***********************************************************

  /**
   * read the matrix from the given ioHandler.  The complete flag indicates
   * if the enclosing begin and end should be also be readed
   *
   * @ingroup gStorable
   */
  bool read(ioHandler& handler,chainCode& cc,const bool complete=true);

  /**
   * write the matrix in the given ioHandler.  The complete flag indicates
   * if the enclosing begin and end should be also be written or not
   *
   * @ingroup gStorable
   */
  bool write(ioHandler& handler,const chainCode& cc,const bool complete=true);
}

#include "ltiChainCode_inline.h"

#endif



