/*
 * Copyright (C) 1998, 1999, 2000, 2001
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
 * \file   ltiColors.h
 *         This file contains some more symbolic color definitions,
 *         like orange, pink, red, green, blue, etc.
 * \author Jochen Wickel
 * \date   12.11.01
 * 
 * $Id: ltiColors.h,v 1.5 2007-04-16 16:36:30 alvarado Exp $
 */

#ifndef LTI_COLORS_H
#define LTI_COLORS_H

#include "ltiRGBAPixel.h"
#include "ltiMath.h"

namespace lti {

  /**
   * @name Color Constants.
   *
   * These color constants are defined in the file ltiColors.h.
   *
   * Note that the color names are all own
   * creations, they are not correlated to any official naming
   * standards at all.
   */
  //@{

  // primary and secondary color constants

  /**
   * Constant for the color black.
   */
  static const rgbaPixel Black(0,0,0);

  /**
   * Constant for the color read.
   */
  static const rgbaPixel Red(255,0,0);

  /**
   * Constant for the color green.
   */
  static const rgbaPixel Green(0,255,0);

  /**
   * Constant for the color blue.
   */
  static const rgbaPixel Blue(0,0,255);

  /**
   * Constant for the color yellow.
   */
  static const rgbaPixel Yellow(255,255,0);

  /**
   * Constant for the color magenta.
   */
  static const rgbaPixel Magenta(255,0,255);

  /**
   * Constant for the color cyan.
   */
  static const rgbaPixel Cyan(0,255,255);

  /**
   * Constant for the color white.
   */
  static const rgbaPixel White(255,255,255);

  // darker primary colors

  /**
   * Constant for dark red
   */
  static const rgbaPixel DarkRed(127,0,0);

  /**
   * Constant for dark green
   */
  static const rgbaPixel DarkGreen(0,127,0);

  /**
   * Constant for dark blue
   */
  static const rgbaPixel DarkBlue(0,0,127);

  // darker secondary colors

  /**
   * constant for the color dark yellow
   */
  static const rgbaPixel DarkYellow(127,127,0);

  /**
   * constant for some kind of dark cyan.
   */
  static const rgbaPixel DarkCyan(0,127,127);

  /**
   * constant for some kind of dark magenta.
   */
  static const rgbaPixel DarkMagenta(127,0,127);

  // brighter primary colors

  /**
   * constant for a kind of bright green.
   */
  static const rgbaPixel BrightGreen(0,255,127);

  /**
   * constant for a kind of bright green.
   */
  static const rgbaPixel BrightBlue(0,127,255);

  /**
   * constant for a kind of bright red.
   */
  static const rgbaPixel BrightRed(255,127,64);

  // brighter secondary colors

  /**
   * constant for some kind of bright magenta.
   */
  static const rgbaPixel BrightMagenta(255,127,255);

  /**
   * constant for some kind of bright yellow.
   */
  static const rgbaPixel BrightYellow(255,255,127);

  /**
   * constant for some kind of bright cyan.
   */
  static const rgbaPixel BrightCyan(127,255,255);


  // other colors

  /**
   * constant for the color orange.
   */
  static const rgbaPixel DarkOrange(192,64,0);

  /**
   * constant for the color fusia.
   */
  static const rgbaPixel Fusia(255,0,127);

  /**
   * constant for the color pink.
   */
  static const rgbaPixel Pink(255,127,127);

  /**
   * constant for another kind of bright green.
   */
  static const rgbaPixel LawnGreen(127,255,0);

  /**
   * constant for some kind of lemon color
   */
  static const rgbaPixel Lemon(127,255,127);

  /**
   * constant for a light blue.
   */
  static const rgbaPixel LightBlue(0,127,255);

  /**
   * constant for a light blue.
   */
  static const rgbaPixel DarkViolet(127,0,255);

  /**
   * constant for the color violet.
   */
  static const rgbaPixel Violet(127,127,255);

  /**
   * Function for scaling a color's intensity.
   * @param src rgbaPixel of the original color
   * @param f factor by which to multiply the R, G and B values. Warning:
   * Saturation effects might lead to strange results.
   * @return rgbaPixel where R, G and B have the given value <code>t</code>.
   */
  inline rgbaPixel scale(const rgbaPixel& src, double f) {
    return rgbaPixel(static_cast<ubyte>(within(src.getRed() *f,0.0,255.0)),
                     static_cast<ubyte>(within(src.getGreen()*f,0.0,255.0)),
                     static_cast<ubyte>(within(src.getBlue() *f,0.0,255.0)));
  }

  /**
   * Function for darkening a color.
   * @param src rgbaPixel of the original color
   * @param f factor which tells how much darker the new color should be.
   * Example: 0.2 means: Make the color 20% darker. 0 means:
   * leave the original, 1.0 means: make it black
   * @return rgbaPixel where R, G and B have the given value <code>t</code>.
   */
  inline rgbaPixel darker(const rgbaPixel& src, double f=0.5) {
    return scale(src,(1.0-f));
  }

  /**
   * Function for brightening a color.
   * @param src rgbaPixel of the original color
   * @param f factor which tells how much darker the new color should be.
   * Example: 0.2 means: Make the color 20% brighter. 0 means:
   * leave the original, 1.0 means: make it twice as bright. Warning:
   * Saturation effects might lead to strange results.
   * @return rgbaPixel where R, G and B have the given value <code>t</code>.
   */
  inline rgbaPixel brighter(const rgbaPixel& src, double f=0.5) {
    return scale(src,(1.0+f));
  }


  /**
   * function for returning a grey value.
   * @param t grey value must be between 0 and 255
   * @return rgbaPixel where R, G and B have the given value <code>t</code>.
   */
  inline rgbaPixel grey(int t) {
    return rgbaPixel(t,t,t);
  }

  /**
   * function for returning a gray value.
   * @param t grey value must be between 0 and 255
   * @return rgbaPixel where R, G and B have the given value <code>t</code>.
   */
  inline rgbaPixel gray(int t) {
    return rgbaPixel(t,t,t);
  }

  /**
   * function for returning a grey value.
   * @param t grey value must be between 0 and 1
   * @return rgbaPixel where R, G and B have the given value <code>t*255</code>.
   */
  inline rgbaPixel grey(double t) {
    return rgbaPixel(int(t*255),int(t*255),int(t*255.0));
  }

  /**
   * function for returning a gray value.
   * @param t grey value must be between 0 and 1
   * @return rgbaPixel where R, G and B have the given value <code>t*255</code>.
   */
  inline rgbaPixel gray(double t) {
    return rgbaPixel(int(t*255),int(t*255),int(t*255.0));
  }

  /**
   * Definition for a 100% bright grey, i.e. White
   */
  static const rgbaPixel Grey100(255,255,255);

  /**
   * Definition for a 75% bright grey, i.e. bright grey
   */
  static const rgbaPixel Grey75(191,191,191);

  /**
   * Definition for a 50% bright grey, i.e. medium grey
   */
  static const rgbaPixel Grey50(127,127,127);

  /**
   * Definition for a 25% bright grey, i.e. dark grey
   */
  static const rgbaPixel Grey25(63,63,63);

  /**
   * Definition for a 0% bright grey, i.e. Black
   */
  static const rgbaPixel Grey0(0,0,0);

}

#endif

