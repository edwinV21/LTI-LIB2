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
 * \file   ltiDraw_inline.h
 * \author Pablo Alvarado
 * \date   09.02.2005
 * 
 * $Id: ltiDraw_inline.h,v 1.9 2009-06-08 01:02:48 alvarado Exp $
 */

#ifndef _LTI_DRAW_INLINE_H_
#define _LTI_DRAW_INLINE_H_

namespace lti {
  
  /**
   * fill the canvas with the current color, starting at the given
   * seed until a color different to the original one at the seed is
   * found.  This is similar to the fill tools usually found in
   * paint programs.
   */
  template<class T>
  inline void draw<T>::fill(const int x,const int y) {
    fill(ipoint(x,y));
  }
 
  /**
   * fillUntil fill the canvas with the current color, 
   * starting at the given seed until the stop color is reached.
   */
  template<class T>
  inline void draw<T>::fillUntil(const int x,const int y,const T& stopColor) {
    fillUntil(ipoint(x,y),stopColor);
  }
  
  /**
   * Set pixel at p in the color set by setColor().
   * @param p coordinates of the pixel to be set
   */
  template<class T>
  inline void draw<T>::set(const ipoint& p) {
    set(p.x,p.y);
  }
  
  /**
   * Set pixel at p in the color set by setColor().
   * @param p coordinates of the pixel to be set
   */
  /* TODO
     template<class T>
     template<class U>
     inline void draw<T>::set(const hPoint2D<U>& p) {
     set(p.x/p.h,p.y/p.h);
     };
  */
  
  /**
   * Draw a line from the point p to point p2.
   * @see line(int,int,int,int)
   */
  template<class T>
  inline void draw<T>::line(const ipoint& p1,const ipoint& p2) {
    line(p1.x,p1.y,p2.x,p2.y);
  }
  
  /**
   * Draw a line 
   * @see line(int,int,int,int)
   */
  template<class T>
  inline void draw<T>::line(const lti::line<int>& l) {
    line(l.getStart(),l.getEnd());
  }
  
  /**
   * the homogeneus point p represents also a line, which equation is
   * given by p.x*x + p.y*y + p.h*1 = 0.  This line is drawn with this
   * member
   */
  /* TODO
     template<class T>
     template <class U>
     inline void draw<T>::line(const hPoint2D<U>& p) {
     if (isNull(img_)) {
     return;
     }
     if (p.x != U(0)) {
     line(-p.h/p.x,0,(-p.y*(img_->lastRow())-p.h)/p.x,img_->lastRow());
     } else if (p.y != U(0)) {
     line(0,-p.h/p.y,img_->lastColumn(),(-p.x*(img_->lastColumn())-p.h)/p.y);
     }
     }
  */
  
  /**
   * Draw a line from the last point to the point p.
   * The "last point" will be defined with the last "set", "point", "line" or
   * "lineTo" method.
   */
  template<class T>
  inline void draw<T>::lineTo(const ipoint& p) {
    lineTo(p.x,p.y);
  }

  /**
   * draw an arrow from point p1 to point p2
   */
  template<class T>
  inline void draw<T>::arrow(const ipoint& p1,const ipoint& p2,const float& size){
    arrow(p1.x,p1.y,p2.x,p2.y,size);
  }
  
  /**
   * draw a rectangle
   * @param r rectangle (with orientation 0)
   * @param filled true if the rectangle must be filled, false if only the
   *               border needs to be drawn.
   */
  template<class T>
  inline void draw<T>::rectangle(const lti::rectangle<int>& r, const bool filled) {
    rectangle(r.ul.x,r.ul.y,r.br.x,r.br.y,filled);
  }
  
  /**
   * draw a rotated rectangle
   * @param r rectangle (with orientation 0)
   * @param angle rotation angle at the middle point of the rectangle
   * @param filled true if the rectangle must be filled, false if only the
   *               border needs to be drawn.
   */
  template<class T>
  inline void draw<T>::rectangle(const lti::rectangle<int>& r, const float angle,
                                 const bool filled) {
    rectangle(r.ul.x,r.ul.y,r.br.x,r.br.y,angle,filled);
  }
  
  /**
   * draw a rectangle
   * @param upperLeft upper left corner of the rectangle with orientation 0
   * @param bottomRight bottom right corner of the rectangle with orientation
   *                    0
   * @param filled true if the rectangle must be filled, false if only the
   *               border needs to be drawn.
   * @see rectangle(int,int,int,int,const bool)
   */
  template<class T>
  inline void draw<T>::rectangle(const ipoint& upperLeft, const ipoint& bottomRight,
                                 const bool filled) {
    rectangle(upperLeft.x,upperLeft.y,bottomRight.x,bottomRight.y,filled);
  }
  
  /**
   * draw a rectangle
   * @param upperLeft upper left corner of the rectangle with orientation 0
   * @param bottomRight bottom right corner of the rectangle with orientation
   *                    0
   * @param angle rotation angle at the middle point of the rectangle
   * @param filled true if the rectangle must be filled, false if only the
   *               border needs to be drawn.
   * @see rectangle(int,int,int,int,const bool)
   */
  template<class T>
  inline void draw<T>::rectangle(const ipoint& upperLeft, const ipoint& bottomRight,
                                 const float angle,
                                 const bool filled) {
    rectangle(upperLeft.x,upperLeft.y,bottomRight.x,bottomRight.y,angle,filled);
  }
  
  /**
   * draw a circle with circle center (x,y) and radius 'r'
   */
  template<class T>
  inline void draw<T>::circle(const int x,const int y,
                              const int r, 
                              const bool filled) {
    circle(ipoint(x,y),r,filled);
  }
  
  /**
   * Sets pixels at all points in c, moved by he given offset.
   */
  template<class T>
  inline void draw<T>::set(const ipointList& c, 
                           const ipoint& offset,
                           const bool lines) {
    drawBase<T>::set(c,offset,lines);
  }

  /**
   * draw a location.
   *
   * @param loc location with position, angle and radius
   * @param showAngleLine if true, a line from the middle point of the
   *        location (given by loc.position) with the angle given by
   *        loc.angle will be drawn.
   */
  template<class T>
  inline void draw<T>::set(const location& loc,const bool showAngleLine) {
    drawBase<T>::set(loc,showAngleLine);
  }

  /**
   * draw an affineLocation.
   *
   * @param loc affineLocation
   * @param showAngleLine if true, a line from the middle point of the
   *        location (given by loc.position) with the angle given by
   *        loc.angle will be drawn.
   */
  template<class T>
  inline void draw<T>::set(const affineLocation& loc,
                           bool approximateEllipse,
                           bool showAngleLine) {
    drawBase<T>::set(loc,approximateEllipse,showAngleLine);
  }

  /**
   * draw a rectLocation
   *
   * @param loc location with position, angle and radius
   * @param showAngleLine if true, a line from the middle point of the
   *        location (given by loc.position) with the angle given by
   *        loc.angle will be drawn.
   */
  template<class T>
  inline void draw<T>::set(const rectLocation& loc,const bool showAngleLine) {
    drawBase<T>::set(loc,showAngleLine);
  }

  /**
   * draw a list of locations
   */
  template<class T>
  inline void draw<T>::set(const list<location>& locs,
                           const bool showAngleLine) {
    drawBase<T>::set(locs,showAngleLine);
  }

  /**
   * draw a list of locations
   */
  template<class T>
  inline void draw<T>::set(const list<affineLocation>& locs,
                           bool approximateEllipse,
                           bool showAngleLine) {
    drawBase<T>::set(locs,approximateEllipse,showAngleLine);
  }
  
  /**
   * draw an integer <code>num</code> at position 'x'/'y' with size 's'.
   * 's' must be given as quota of image height (0.0 to 1.0).
   * @see text()
   * @param num the number to be written
   * @param x the column where the number will be drawn
   * @param y the row where the number will be drawn
   */
  template<class T>
  inline void draw<T>::number(const int num,
                              const int x,
                              const int y) {
    number(num,x,y,0.03125f);
  }

  /**
   * Draw the text string txt at position x,y.
   * @see text(const std::string,const int,const int,const point&,
   *           const float,const bool)
   */
  template<class T>
  inline void draw<T>::text(const std::string txt,const int x, const int y) {
    text(txt,x,y,ipoint(1,1),1.0f,true);
  }
  
  /**
   * This is need for drawing filled rotated ellipses
   */
  template<class T>
  inline ipoint draw<T>::turnPoint(const float x, const float y, float angle) {
    float sa=sin(angle);
    float ca=cos(angle);
    return lti::ipoint(iround(x*ca-y*sa),
                       iround(x*sa+y*ca));
  }
  
  /**
   * This is need for drawing filled rotated ellipses
   * this enters a point into the point list, the first one into x1
   * the second one into x2
   * if any more come, they are merged into the closest one.
   *
   * The vectors x1 and x2 can be seen as input- or output point lists, while
   * miny and maxy contain the range of y values used until now.
   *
   * You must ensure that the x1 and x2 arrays are bigger than the
   * value contained in p.y.
   */
  template<class T>
  inline void draw<T>::enterPX(const ipoint& p,
                               int* x1, int* x2, int& miny, int& maxy) {
    if ((p.y >= 0) && (p.y < img_->rows())) {
      x1[p.y]=lti::min(x1[p.y],p.x);
      x2[p.y]=lti::max(x2[p.y],p.x);
      if (p.y < miny) {
        miny=p.y;
      }
      if (p.y > maxy) {
        maxy=p.y;
      }
    }
  }
  
  template<class T>
  inline bool draw<T>::inCanvas(const int x,const int y) const {
    assert(img_ != 0);
    return ((static_cast<unsigned int>(x) <
             static_cast<unsigned int>(img_->columns())) &&
            (static_cast<unsigned int>(y) <
             static_cast<unsigned int>(img_->rows())));
  }
  
  template<class T>
  inline bool draw<T>::inCanvas(const ipoint& p) const {
    assert(img_ != 0);
    return ((static_cast<unsigned int>(p.x) <
             static_cast<unsigned int>(img_->columns())) &&
            (static_cast<unsigned int>(p.y) <
             static_cast<unsigned int>(img_->rows())));
  } 
  
}

#endif
