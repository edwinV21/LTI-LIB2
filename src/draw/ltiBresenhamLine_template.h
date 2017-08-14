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
 * \file   ltiBresenhamLine_template.h
 * \author Pablo Alvarado
 * \author Gustavo Quiros
 * \date   15.3.2005
 * 
 * $Id: ltiBresenhamLine_template.h,v 1.5 2005-08-09 14:34:34 arndh Exp $
 */

#ifndef _LTI_BRESENHAM_LINE_TEMPLATE_H_
#define _LTI_BRESENHAM_LINE_TEMPLATE_H_

namespace lti {

  template<typename POINT_STORE, typename T>
  bool bresenhamLine::renderLine(const int x1, const int y1,
                                 const int x2, const int y2, 
                                 POINT_STORE& store, T color) const{
    
    // The Bresenham method for the generation of the optimal line
    
    int dx, dy, d, incrE, incrNE, x, y, stop, dirX, dirY;
    
    // origin
    x = x1;
    y = y1;
    
    // the derivative of the line
    dx = x2 - x1;
    dy = y2 - y1;
    
    // the direction in each coordinate
    dirX = (dx > 0) ? 1 : -1;
    dirY = (dy > 0) ? 1 : -1;

    if(dx == 0){
      // Vertical line
      stop = y2 + dirY;
      while(y != stop){
        addPoint(x,y,store,color);
        y += dirY;
      }
    }
    else if(dy == 0){
      // Horizontal line
      stop = x2 + dirX;
      while(x != stop){
        addPoint(x,y,store,color);
        x += dirX;
      }
    }
    else if(abs(dx) > abs(dy)){
      // Walk on X 
      stop = x2 + dirX;
      incrE = 2 * dy * dirY;
      incrNE = 2 * (dy * dirY - dx * dirX);
      d = 2 * dy * dirY - dx * dirX;
      while(x != stop){
        addPoint(x,y,store,color);
        x += dirX;
        if(d <= 0) { 
          d += incrE; 
        }
        else { 
          d += incrNE; 
          y += dirY; 
        }
      }
    }
    else{
      // Walk on Y
      stop = y2 + dirY;
      incrE = 2 * dx * dirX;
      incrNE = 2 * (dx * dirX - dy * dirY);
      d = 2 * dx * dirX - dy * dirY;
      while(y != stop){
        addPoint(x,y,store,color);
        y += dirY;
        if(d <= 0) { 
          d += incrE; 
        }
        else { 
          d += incrNE; 
          x += dirX; 
        }
      }
    }

    return true;
  }
  
}

#endif
