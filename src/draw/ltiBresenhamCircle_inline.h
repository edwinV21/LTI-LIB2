/*
 * Copyright (C) 1998 - 2005
 * Peter Doerfler
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
 * \file   ltiBresenhamCircle_inline.h
 * \author Peter Doerfler
 * \date   30.07.2005
 * 
 * $Id: ltiBresenhamCircle_inline.h,v 1.2 2005-08-09 14:34:34 arndh Exp $
 */

#ifndef _LTI_BRESENHAM_CIRCLE_INLINE_H_
#define _LTI_BRESENHAM_CIRCLE_INLINE_H_

namespace lti {
  
  template<typename T>
  inline bool bresenhamCircle::apply(const int cx, const int cy,
                                     const int radius,
                                     genericMatrix<T>& m, T color) const {
    return renderCircle(cx,cy,radius,m,color);
  }

  inline bool bresenhamCircle::apply(const int cx, const int cy,
                                     const int radius,
                                     ipointList& l) const {
    return renderCircle(cx,cy,radius,l,0);
  }

  template<typename T>
  inline bool bresenhamCircle::apply(const ipoint& c, const int rad,
                                     genericMatrix<T>& m, T color) const {
    return renderCircle(c.x,c.y,rad,m,color);
  }

  inline bool bresenhamCircle::apply(const ipoint& c, const int rad,
                                     ipointList& l) const {
    return renderCircle(c.x,c.y,rad,l,0);
  }

  inline bool bresenhamCircle::apply(const int rad, array<int>& ar) const {
    ar.resize(-rad,rad,0,Init);
    return renderCircle(0,0,rad,ar,0);
  }
  
  template<typename T>
  inline void bresenhamCircle::addPoints(const int cx, const int cy,
                                         const int x, const int y,
                                         ipointList& l, T) const {

    if (x==0) {
      if (y==0) {
        l.push_back(ipoint(cx,cy));
      } else {
        const int x1=cx;
        const int x3=cx+y;
        const int x4=cx-y;
        const int y1=cy+y;
        const int y2=cy-y;
        const int y3=cy;
        ipoint p(x1,y1);
        l.push_back(p);
        p.y=y2;
        l.push_back(p);
        p.set(x3,y3);
        l.push_back(p);
        p.x=x4;
        l.push_back(p);
      }
    } else if (y==0) {
      const int x1=cx+x;
      const int x2=cx-x;
      const int x3=cx;
      const int y1=cy;
      const int y3=cy+x;
      const int y4=cy-x;
      ipoint p(x1,y1);
      l.push_back(p);
      p.x=x2;
      l.push_back(p);
      p.set(x3,y3);
      l.push_back(p);
      p.y=y4;
      l.push_back(p);
    } else {       
      const int x1=cx+x;
      const int x2=cx-x;
      const int x3=cx+y;
      const int x4=cx-y;
      const int y1=cy+y;
      const int y2=cy-y;
      const int y3=cy+x;
      const int y4=cy-x;
      ipoint p(x1,y1);
      l.push_back(p);
      p.y=y2;
      l.push_back(p);
      p.x=x2;
      l.push_back(p);
      p.y=y1;
      l.push_back(p);
      p.set(x3,y3);
      l.push_back(p);
      p.y=y4;
      l.push_back(p);
      p.x=x4;
      l.push_back(p);
      p.y=y3;
      l.push_back(p);
    }
  }

  template <typename T>
  inline void bresenhamCircle::setPoint(const int x, const int y,
                                        const int mx, const int my,
                                        genericMatrix<T>& m, T color) const {
    if ((x>=0) && (x<mx) && (y>=0) && (y<my)) {
      m.at(y,x) = color;
    }
  }

  template<typename T>
  inline void bresenhamCircle::addPoints(const int cx, const int cy,
                                         const int x, const int y,
                                         genericMatrix<T>& m, T color) const {

    const int mx=m.columns();
    const int my=m.rows();

    if (x==0) {
      if (y==0) {
      } else {
        const int x1=cx;
        const int x3=cx+y;
        const int x4=cx-y;
        const int y1=cy+y;
        const int y2=cy-y;
        const int y3=cy;
        setPoint(x1,y1,mx,my,m,color);
        setPoint(x1,y2,mx,my,m,color);
        setPoint(x3,y3,mx,my,m,color);
        setPoint(x4,y3,mx,my,m,color);
      }
    } else if (y==0) {
      const int x1=cx+x;
      const int x2=cx-x;
      const int x3=cx;
      const int y1=cy;
      const int y3=cy+x;
      const int y4=cy-x;
      setPoint(x1,y1,mx,my,m,color);
      setPoint(x2,y1,mx,my,m,color);
      setPoint(x3,y3,mx,my,m,color);
      setPoint(x3,y4,mx,my,m,color);
    } else {
      const int x1=cx+x;
      const int x2=cx-x;
      const int x3=cx+y;
      const int x4=cx-y;
      const int y1=cy+y;
      const int y2=cy-y;
      const int y3=cy+x;
      const int y4=cy-x;
      setPoint(x1,y1,mx,my,m,color);
      setPoint(x1,y2,mx,my,m,color);
      setPoint(x2,y1,mx,my,m,color);
      setPoint(x2,y2,mx,my,m,color);
      setPoint(x3,y3,mx,my,m,color);
      setPoint(x3,y4,mx,my,m,color);
      setPoint(x4,y3,mx,my,m,color);
      setPoint(x4,y4,mx,my,m,color);
    }
  }

  template<typename T>
  inline void bresenhamCircle::addPoints(const int, const int,
                                         const int x, const int y,
                                         array<int>& pts, T) const {

    if (x==0) {
      if (y!=0) {
        if (y>pts.at(0)) {
          pts.at(0)=y;
        }
      }
    } else if (y==0) {
      if (x>pts.at(0)) {
        pts.at(0)=x;
      }
    } else {
      if (y>pts.at(x)) {
        pts.at(x)=y;
        pts.at(-x)=y;
      }
      if (x>pts.at(y)) {
        pts.at(y)=x;
        pts.at(-y)=x;
      }
    }
  }

}

#endif
