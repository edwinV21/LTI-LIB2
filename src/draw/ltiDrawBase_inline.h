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
 * \file   ltiDrawBase_inline.h
 * \author Jens Paustenbach
 * \date   09.02.2005
 * 
 * $Id: ltiDrawBase_inline.h,v 1.5 2006-09-14 15:14:02 doerfler Exp $
 */

#ifndef _LTI_DRAW_BASE_INLINE_H_
#define _LTI_DRAW_BASE_INLINE_H_

namespace lti {

  /**
   * sets a single pixel at p
   */
  template<class T>
  inline void drawBase<T>::set(const ipoint& p) {
    set(p.x, p.y);
  }
  
  /**
   * sets a single pixel at p
   */
  /*
    template<class T>
    template<class U>
    inline void drawBase<T>::set(const hPoint2D<U>& p) {
    set(p.x/p.h, p.y/p.h);
    }
  */
  
  /**
   * Set marker at point (x, y).
   * If there is no style, markerType or width is given, the value
   * set by the setStyle or setWidth is used.
   * @param x the x coordinate
   * @param y the y coordinate
   */
  template<class T>
  inline void drawBase<T>::marker(int x, int y) {
    marker(x, y, actSize_, actSymbol_);    
  }
  
  /**
   * Set marker at point (x, y).
   * If there is no style, markerType or width is given, the value
   * set by the setStyle or setWidth is used.
   * @param x the x coordinate
   * @param y the y coordinate
   * @param width the size of the marker symbol
   */
  template<class T>
  inline void drawBase<T>::marker(int x, int y, int width) {
    marker(x, y, width, actSymbol_);    
  }
  
  /**
   * Set marker at point (x, y).
   * If there is no style, markerType or width is given, the value
   * set by the setStyle or setWidth is used.
   * @param x the x coordinate
   * @param y the y coordinate
   * @param style the style of the marker symbol.
   */
  template<class T>
  inline void drawBase<T>::marker(int x, int y, const char* style) {
    marker(x, y, actSize_, style);    
  }
  
  /**
   * Set marker at point (x, y).
   * If there is no style, markerType or width is given, the value
   * set by the setStyle or setWidth is used.
   * @param x the x coordinate
   * @param y the y coordinate
   * @param t the type of the marker
   */
  template<class T>
  inline void drawBase<T>::marker(int x, int y, eMarkerType t) {
    marker(x, y, actSize_, t);    
  }
  
  /**
   * Set marker at point (x, y).
   * If there is no style, markerType or width is given, the value
   * set by the setStyle or setWidth is used.
   * @param x the x coordinate
   * @param y the y coordinate
   * @param w the size of the marker symbol
   * @param style the style of the marker symbol.
   */
  template<class T>
  inline void drawBase<T>::marker(int x, int y, int w, const char* style) {
    drawSymbol(x, y, w, style);  
  }
  
  /**
   * Set marker at point (x, y).
   * If there is no style, markerType or width is given, the value
   * set by the setStyle or setWidth is used.
   * @param x the x coordinate
   * @param y the y coordinate
   * @param w the size of the marker symbol
   * @param t the type of the marker
   */
  template<class T>
  inline void drawBase<T>::marker(int x, int y, int w, eMarkerType t) {
    drawSymbol(x, y, w, t);
  }
  
  /**
   * Set marker at p.
   * @see marker(int, int)
   */
  template<class T>
  inline void drawBase<T>::marker(const ipoint& p) {
    marker(p.x, p.y, actSize_, actSymbol_);    
  }

  /**
   * Set marker at p with given width.
   * @see marker(int, int)
   */
  template<class T>
  inline void drawBase<T>::marker(const ipoint& p, int width) {
    marker(p.x, p.y, width, actSymbol_);    
  }

  /**
   * Set marker at p with given style
   * @see marker(int, int)
   */
  template<class T>
  inline void drawBase<T>::marker(const ipoint& p, const char* style) {
    marker(p.x, p.y, actSize_, style);    
  }

  /**
   * Set given marker type at p.
   * @see marker(int, int)
   */
  template<class T>
  inline void drawBase<T>::marker(const ipoint& p, eMarkerType t) {
    marker(p.x, p.y, actSize_, t);    
  }

  /**
   * Set given marker type at p with given width.
   * @see marker(int, int)
   */
  template<class T>
  inline void drawBase<T>::marker(const ipoint& p, int width, eMarkerType t) {
    marker(p.x, p.y, width, t);    
  }

  /**
   * Set marker at p with given width and style.
   * @see marker(int, int)
   */
  template<class T>
  inline void drawBase<T>::marker(const ipoint& p, int width, 
                                  const char* style) {
    marker(p.x, p.y, width, style);    
  }
  
  /**
   * Set marker at p.
   * @param p coordinates of the pixel to be set
   * @see marker(int, int)
   */
  /* TODO
     template<class T>
     template <class U>
     inline void drawBase<T>::marker(const hPoint2D<U>& p) {
     marker(p.x/p.h, p.y/p.h, actSize_, actSymbol_);    
     }
  */

  /**
   * Set marker at p width given width.
   * @see marker(int, int)
   */
  /* TODO
     template<class T>
     template <class U>
     inline void drawBase<T>::marker(const hPoint2D<U>& p, int width) {
     marker(p.x/p.h, p.y/p.h, width, actSymbol_);    
     }
  */

  /**
   * Set given marker type at p.
   * @see marker(int, int)
   */
  /* TODO
     template<class T>
     template <class U>
     inline void drawBase<T>::marker(const hPoint2D<U>& p, eMarkerType t) {
     marker(p.x/p.h, p.y/p.h, actSize_, t);    
     }
  */
  
  /**
   * Set marker at p width given style.
   * @see marker(int, int, const char*)
   */
  /* TODO
     template<class T>
     template <class U>
     inline void drawBase<T>::marker(const hPoint2D<U>& p, const char* style) {
     marker(p.x/p.h, p.y/p.h, actSize_, style);    
     }
  */
  
  /**
   * Set given marker type at p with given width.
   * @see marker(int, int)
   */
  /* TODO
     template<class T>
     template <class U>
     inline void drawBase<T>::marker(const hPoint2D<U>& p, int width, eMarkerType t) {
     marker(p.x/p.h, p.y/p.h, width, t);    
     }
  */
  
  /**
   * Set marker at p with given width and style.
   * @see marker(int, int)
   */
  /* TODO
     template<class T>
     template <class U>
     inline void drawBase<T>::marker(const hPoint2D<U>& p, int width, const char* style) {
     marker(p.x/p.h, p.y/p.h, width, style);    
     }
  */
  
  /**
   * Sets markers at all positions in c, moved by offset.
   * @see marker(int, int)
   */
  template<class T>
  inline void drawBase<T>::marker(const ipointList& c, const ipoint& offset) {
    marker(c, actSize_, actSymbol_, offset); 
  }
  
  /**
   * Sets markers at all positions in c, moved by offset, with given width.
   * @see marker(int, int)
   */
  template<class T>
  inline void drawBase<T>::marker(const ipointList& c, int width,
                                  const ipoint& offset) {
    marker(c, width, actSymbol_, offset);    
  }
  
  /**
   * Sets markers at all positions in c, moved by offset, with given style.
   * @see marker(int, int)
   */
  template<class T>
  inline void drawBase<T>::marker(const ipointList& c, const char* style,
                                  const ipoint& offset) {
    marker(c, actSize_, style, offset);    
  }
  
  /**
   * Sets given marker types at all positions in c, moved by offset.
   * @see marker(int, int)
   */
  template<class T>
  inline void drawBase<T>::marker(const ipointList& c, eMarkerType t,
                                  const ipoint& offset) {
    marker(c, actSize_, t, offset);    
  }
  
  /**
   * Draws a line from the point p to point p2.
   * @see line(int, int, int, int)
   */
  template<class T>
  inline void drawBase<T>::line(const ipoint& p1, const ipoint& p2) {
    line(p1.x, p1.y, p2.x, p2.y);
  }

  /**
   * Draw a line from the last point to point p.
   * @see lineTo(int, int)
   */
  template<class T>
  inline void drawBase<T>::lineTo(const ipoint& p) {
    lineTo(p.x, p.y);
  }

  // moveTo()
  template<class T>
  inline void drawBase<T>::moveTo(const ipoint& p) {
    moveTo(p.x, p.y);
  }
  
  /**
   * Draws a vertical line from (p1.x, p1.y) to (p1.x, p2.y).
   */
  template<class T>
  inline void drawBase<T>::verticalLine(const ipoint& p1, const ipoint& p2) {
    verticalLine(p1.x, p1.y, p2.y);
  }
  
  /**
   * Draws a horizontal line from (p1.x, p1.y) to (p2.x, p1.y).
   */
  template<class T>
  inline void drawBase<T>::horizontalLine(const ipoint& p1, const ipoint& p2) {
    horizontalLine(p1.x, p2.x, p1.y);
  }
  
  
  /**
   * draws a rectangle.
   * @param upperLeft upper left corner of the rectangle
   * @param bottomRight bottom right corner of the rectangle
   * @param filled if true rectangle is filled
   */
  template<class T>
  inline void drawBase<T>::rectangle(const ipoint& upperLeft, 
                                     const ipoint& bottomRight,
                                     bool filled) {
    rectangle(upperLeft.x, upperLeft.y, bottomRight.x, bottomRight.y, filled);
  }
  
  /**
   * draw a rectangle.
   * the rectangle must contain the upper left and the bottom right point
   * @param r rectangle to be drawn
   * @param filled if true rectangle is filled
   */
  template<class T>
  inline void drawBase<T>::rectangle(const lti::rectangle<int>& r, 
                                     bool filled) {
    rectangle(r.ul.x, r.ul.y, r.br.x, r.br.y, filled);
  }
  
  /**
   * draw an arrow. arrow tip will be at p2.
   * If size<1.0 then tipsize will be the relative portion of arrow length.
   * If size>1.0 then tipsize will be (int)size, independent of arrow length.
   */
  template<class T>
  inline void drawBase<T>::arrow(const ipoint& p1, const ipoint& p2, 
                                 const float& size) {
    arrow(p1.x, p1.y, p2.x, p2.y, size);
  }
  
  /**
   * Draw the given text at the position p=(x, y).
   * @see test(std::string, int, int)
   */
  template<class T>
  inline void drawBase<T>::text(const std::string txt, const ipoint& p) {
    text(txt, p.x, p.y);
  }
  
  /**
   * draw an integer <code>num</code> at position p='x'/'y'.
   * @see number(int, int, int)
   */
  template<class T>
  inline void drawBase<T>::number(int num, const ipoint& p) {
    number(num, p.x, p.y);
  }
  
}

#endif
