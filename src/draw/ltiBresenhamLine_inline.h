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


/**
 * \file   ltiBresenhamLine_inline.h
 * \author Pablo Alvarado
 * \author Gustavo Quiros
 * \date   15.3.2005
 * 
 * $Id: ltiBresenhamLine_inline.h,v 1.7 2006-03-23 12:19:34 doerfler Exp $
 */

#ifndef _LTI_BRESENHAM_LINE_INLINE_H_
#define _LTI_BRESENHAM_LINE_INLINE_H_

namespace lti {
  
  template<typename T>
  inline bool bresenhamLine::apply(const int fx, const int fy,
                                   const int tx, const int ty,
                                   genericMatrix<T>& m, T color) const {
    return renderLine(fx,fy,tx,ty,m,color);
  }

  inline bool bresenhamLine::apply(const int fx, const int fy,
                                   const int tx, const int ty,
                                   ipointList& l) const {
    return renderLine(fx,fy,tx,ty,l,0);
  }

  template<typename T>
  inline bool bresenhamLine::apply(const ipoint& f, const ipoint& t, 
                                   genericMatrix<T>& m, T color) const {
    return renderLine(f.x,f.y,t.x,t.y,m,color);
  }

  inline bool bresenhamLine::apply(const ipoint& f, const ipoint& t, 
                                   ipointList& l) const {
    return renderLine(f.x,f.y,t.x,t.y,l,0);
  }
  
  template<typename T>
  inline void bresenhamLine::addPoint(const int x, const int y,
                                      ipointList& l, T) const {
    l.push_back(ipoint(x,y));
  }

  template<typename T>
  inline void bresenhamLine::addPoint(const int x, const int y,
                                      genericMatrix<T>& m, T color) const {
    
    // lti::draw needs this check. Should find a way to select this
    // efficiently

    if ((static_cast<uint32>(x) < static_cast<uint32>(m.columns())) &&
        (static_cast<uint32>(y) < static_cast<uint32>(m.rows()))) {

      m.at(y,x) = color;
    }
  }

}

#endif
