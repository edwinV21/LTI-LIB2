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
 * \file   ltiDrawBase_template.h
 * \author Jens Paustenbach
 * \date   19.11.2002
 * 
 * $Id: ltiDrawBase_template.h,v 1.11 2009-06-08 01:02:48 alvarado Exp $
 */

#include "ltiRound.h"

#include "ltiDebug.h"

namespace lti {

  // --------------------------------------------------
  // drawBase
  // --------------------------------------------------

  // default constructor
  template<class T>
  drawBase<T>::drawBase()
    : object() {
  }
  
  // copy member
  template<class T>
  drawBase<T>& drawBase<T>::copy(const drawBase&) {
    return (*this);
  }
  
  // copy constructor
  template<class T>
  drawBase<T>::drawBase(const drawBase& other)
    : object(), status() {
    copy(other);
  }
  
  // destructor
  template<class T>
  drawBase<T>::~drawBase() {
  }
  
  // returns the name of this type
  template<class T>
  const std::string& drawBase<T>::name() const {
    _LTI_RETURN_CLASS_NAME
  }
  
  /* TODO
     template <class T>
     void drawBase<T>::setLinePattern(const linePattern& pat) {
     linepat=pat;
     useLinePattern=true;
     }
  */
  
  /* TODO
     template <class T>
     void drawBase<T>::setFillPattern(const fillPattern& pat) {
     fillpat=pat;
     useFillPattern=true;
     }
  */
  
  template<class T>
  void drawBase<T>::setMarkerType(eMarkerType style) {
    actSymbol_=style;
  }
  
  template<class T>
  void drawBase<T>::setMarkerSize(int s) {
    actSize_=s;
  }
  
  template<class T>
  void drawBase<T>::setStyle(const char* style) {
    
    const int len=int(strlen(style));
    
    filled_=false;
    
    if (len==0 || len>4) {
      //error
      return;
    }

    switch(style[0]) {

    case '.':
      setMarkerType(Pixel);
      break;
    case 'o':
      setMarkerType(Circle);
      break;
    case 'x':
      setMarkerType(Xmark);
      break;
    case '+':
      setMarkerType(Plus);
      break;
    case '*':
      setMarkerType(Star);
      break;
    case 's':
      setMarkerType(Square);
      break;
    case 'd':
      setMarkerType(Diamond);
      break;
    case '^':
      setMarkerType(TriangleUp);
      break;
    case 'v':
      setMarkerType(TriangleDown);
      break;
    case '<':
      setMarkerType(TriangleLeft);
      break;
    case '>':
      setMarkerType(TriangleRight);
      break;
    case '#':
      setMarkerType(Dot);
      break;
    case 'l':
      setMarkerType(LtiLogo);
      break;
    default:
      if (len==1) {
        setColor(style);
      } else if (len==2) {
        if (isdigit(style[1])) {
          setColor(style);
        } else {
          char color[2]="";
          strncpy(color, style,1);
          setColor(color);
          setStyle(&style[1]);
        }
      } else if (len==3) {
        if (isdigit(style[1])) {
          char color[3]="";
          strncpy(color, style,2);
          setColor(color);
          setStyle(&style[2]);
        } else {
          char color[2]="";
          strncpy(color, style,1);
          setColor(color);
          setStyle(&style[1]);
        }
      } else {
        char color[3]="";
        strncpy(color, style,2);
        setColor(color);
        setStyle(&style[2]);
      }
      return;
    }

    if (len==1) return; //done

    if (style[1]=='f') {
      filled_=true;
      if (len>2) {
        setColor(&style[2]);
      }
    } else {
      filled_=false;
      setColor(&style[1]);
    }

  }


  template<class T>
  void drawBase<T>::marker(const ipointList& c, int width, eMarkerType t,
                           const ipoint& offset) {

    ipointList::const_iterator it;
    ipointList::const_iterator itEnd;

    it = c.begin();
    itEnd = c.end();

    if (offset==ipoint(0,0)) {
      while (it!=itEnd) {
        marker((*it), width, t);
        ++it;
      }
    } else {
      while (it!=itEnd) {
        marker((*it).x+offset.x,(*it).y+offset.y, width, t);
        ++it;
      }
    }
  }

  template<class T>
  void drawBase<T>::marker(const ipointList& c, int width, const char* style,
                           const ipoint& offset) {

    ipointList::const_iterator it;
    ipointList::const_iterator itEnd;

    it = c.begin();
    itEnd = c.end();

    if (offset==ipoint(0,0)) {
      while (it!=itEnd) {
        marker((*it), width, style);
        ++it;
      }
    } else {
      while (it!=itEnd) {
        marker((*it).x+offset.x,(*it).y+offset.y, width, style);
        ++it;
      }
    }
  }


  template<class T>
  void drawBase<T>::set(const ipointList& c, 
                        const ipoint& offset,
                        const bool lines) {

    ipointList::const_iterator it;
    ipointList::const_iterator itEnd;

    it = c.begin();
    itEnd = c.end();

    if (lines) {    
      if (it!=itEnd) {
        set((*it).x+offset.x,(*it).y+offset.y);
        ++it;
      }
      while (it!=itEnd) {
        lineTo((*it).x+offset.x,(*it).y+offset.y);
        ++it;
      }
    } else {
      if (offset==lti::ipoint(0,0)) {
        while (it!=itEnd) {
          set((*it));
          ++it;
        }
      }
      else {
        while (it!=itEnd) {
          set((*it).x+offset.x,(*it).y+offset.y);
          ++it;
        }
      }
    }
  }


  template<class T>
  void drawBase<T>::set(const location& loc, bool showAngleLine) {
    // assuming image coordinate system (positive angle clock-wise)
    const float cosa = cos(loc.angle);
    const float sina = -sin(loc.angle);

    // compute the four points
    ipoint corn[4];
    const float r = loc.radius;

    corn[0].x  = static_cast<int>(0.5f + r*(cosa + sina) + loc.position.x);
    corn[0].y  = static_cast<int>(0.5f + r*(-sina + cosa) + loc.position.y);

    corn[1].x  = static_cast<int>(0.5f + r*(-cosa + sina) + loc.position.x);
    corn[1].y  = static_cast<int>(0.5f + r*(sina + cosa) + loc.position.y);

    corn[2].x  = static_cast<int>(0.5f + r*(-cosa - sina) + loc.position.x);
    corn[2].y  = static_cast<int>(0.5f + r*(sina - cosa) + loc.position.y);

    corn[3].x  = static_cast<int>(0.5f + r*(cosa - sina) + loc.position.x);
    corn[3].y  = static_cast<int>(0.5f + r*(-sina - cosa) + loc.position.y);


    // draw the box
    line(corn[0], corn[1]);
    lineTo(corn[2]);
    lineTo(corn[3]);
    lineTo(corn[0]);

    // draw the radius
    if (showAngleLine) {
      line(static_cast<int>(0.5f + loc.position.x),
           static_cast<int>(0.5f + loc.position.y),
           static_cast<int>(0.5f + r*cosa+loc.position.x),
           static_cast<int>(0.5f - r*sina+loc.position.y));
    }
  }


  template<class T>
  void drawBase<T>::set(const rectLocation& loc, bool showAngleLine) {
    // assuming image coordinate system (positive angle clock-wise)
    const float cosa = cos(loc.angle);
    const float sina = -sin(loc.angle);

    // compute the four points
    ipoint corn[4];
    const float l = loc.minLength/2;
    const float m = loc.maxLength/2;

    corn[0].x  = static_cast<int>(0.5f + m*cosa + l*sina + loc.position.x);
    corn[0].y  = static_cast<int>(0.5f - m*sina + l*cosa + loc.position.y);

    corn[1].x  = static_cast<int>(0.5f - m*cosa + l*sina + loc.position.x);
    corn[1].y  = static_cast<int>(0.5f + m*sina + l*cosa + loc.position.y);

    corn[2].x  = static_cast<int>(0.5f - m*cosa - l*sina + loc.position.x);
    corn[2].y  = static_cast<int>(0.5f + m*sina - l*cosa + loc.position.y);

    corn[3].x  = static_cast<int>(0.5f + m*cosa - l*sina + loc.position.x);
    corn[3].y  = static_cast<int>(0.5f - m*sina - l*cosa + loc.position.y);

    // draw the box
    line(corn[0], corn[1]);
    lineTo(corn[2]);
    lineTo(corn[3]);
    lineTo(corn[0]);

    // draw the radius
    if (showAngleLine) {
      line(static_cast<int>(0.5f + loc.position.x),
           static_cast<int>(0.5f + loc.position.y),
           static_cast<int>(0.5f + m*cosa + loc.position.x),
           static_cast<int>(0.5f - m*sina + loc.position.y));
    }
  }


  template<class T>
  void drawBase<T>::set(const affineLocation& loc,
                        bool approximateEllipse,
                        bool showAngleLine) {
    // assuming image coordinate system (positive angle clock-wise)
    
    const fmatrix& tm = loc.transMatrix;
    const float r = (closeToZero(loc.radius) ? 1.f : loc.radius);
    const float& t00 = tm.at(0,0);
    const float& t01 = tm.at(0,1);
    const float& t10 = tm.at(1,0);
    const float& t11 = tm.at(1,1);

    
    if (approximateEllipse) {

      // the following calculation is adopted from the code found here
      // http://www.devmaster.net/forums/archive/index.php/t-6163.html
      
      float m[4];
      float angle;
      int ra, rb;
      
      // build ellipse representation matrix (unit circle transformation).
      // the 2x2 matrix multiplication with the upper 2x2 of a_mat is inlined.
      m[0] = r * ( t00 + t01);
      m[1] = r * ( t10 + t11);
      m[2] = r * (-t00 + t01);
      m[3] = r * (-t10 + t11);
      
      // to implict equation (centered)
      float A = (m[0]*m[0]) + (m[2]*m[2]);
      float C = (m[1]*m[1]) + (m[3]*m[3]);
      float B = (m[0]*m[1] + m[2]*m[3]) * 2.0f;
      
      _lti_debug2("m[0] = " << m[0]
                  << "  m[1] = " << m[1]
                  << "  m[2] = " << m[2]
                  << "  m[3] = " << m[3] << "\n"
                  << "A = " << A
                  << "  C = " << C
                  << "  B = " << B << "\n");
      
      // precalculate distance A to C
      float ac = A-C;
      
      float A2, C2;
      // convert implicit equation to angle and halfaxis:
      if (closeToZero(B)) {
        angle = 0;
        A2 = A;
        C2 = C;
      } else if (closeToZero(ac)) {
        angle = static_cast<float>(Pi / 4.0);
        A2 = A + B*0.5f;
        C2 = A - B*0.5f;
      } else {
        // Precalculate radical:
        float K = sqrt(1 + B*B /(ac*ac));
        A2 = 0.5f * (A+C + K*ac);
        C2 = 0.5f * (A+C - K*ac);
        angle = 0.5f * atan2(B,ac);
      }
      
      _lti_debug2("A2 = " << A2
                  << "  C2 = " << C2
                  << "  angle = " << angle << "\n\n");


      // This can get slightly below zero due to rounding issues.
      // it's save to clip to zero in this case 
      // (this yields a zero length halfaxis)
      if (A2 < 0) A2 = 0; else A2 = sqrt(A2);
      if (C2 < 0) C2 = 0; else C2 = sqrt(C2);

      // now A2 and C2 are half-axis:
      const float s2 = 1.f/sqrt(2.f);
      if (ac <= 0) {
        rb = iround(s2*A2);
        ra = iround(s2*C2);
      } else {
        rb = iround(s2*C2);
        ra = iround(s2*A2);
      }
    
      ellipse(loc.position, ra, rb, angle, false);

      // draw the orientation
      if (showAngleLine) {
        
        //TODO don't know what the correct radius is here, so just use
        //the bigger one (ra)
        
        const float orient = angle-loc.angle;
        const float cosa = cos(orient);
        const float sina = sin(orient);
        line(static_cast<int>(0.5f + loc.position.x),
             static_cast<int>(0.5f + loc.position.y),
             static_cast<int>(0.5f + ra*cosa+loc.position.x),
             static_cast<int>(0.5f - ra*sina+loc.position.y));
      }
      
    } else { // approximateEllipse 

      const float dx1 = r * ( t00 + t01);
      const float dx2 = r * ( t00 - t01);
      const float dy1 = r * ( t10 + t11);
      const float dy2 = r * (-t10 + t11);
    
      const float& locx = loc.position.x;
      const float& locy = loc.position.y;
      
      const int x1 = iround(locx - dx1);
      const int y1 = iround(locy - dy1);
      const int x2 = iround(locx + dx2);
      const int y2 = iround(locy - dy2);
      const int x3 = iround(locx + dx1);
      const int y3 = iround(locy + dy1);
      const int x4 = iround(locx - dx2);
      const int y4 = iround(locy + dy2);
      
      line(x1,y1,x2,y2);
      lineTo(x3,y3);
      lineTo(x4,y4);
      lineTo(x1,y1);
    }
  }

  template<class T>
  void drawBase<T>::set(const list<location>& locs,
                        bool showAngleLine) {
    list<location>::const_iterator it;
    for (it=locs.begin();it!=locs.end();++it) {
      set(*it, showAngleLine);
    }
  }

  template<class T>
  void drawBase<T>::set(const list<affineLocation>& locs,
                        bool approximateEllipse,
                        bool showAngleLine) {
    list<affineLocation>::const_iterator lit = locs.begin();
    const list<affineLocation>::const_iterator lite = locs.end();
    while (lit != lite) {
      set(*lit, approximateEllipse, showAngleLine);
    }
  }
  
  template<class T>
  void drawBase<T>::polarLine(const ipoint& p,
                              const float& length,
                              const float& angle) {

    int xdiff = iround( length * cos(angle) );
    int ydiff = iround( length * sin(angle) );

    line(p.x, p.y, p.x+xdiff, p.y+ydiff);

  }


}  // namespace lti
