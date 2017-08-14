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
 * \file   ltiBresenhamCircle_template.h
 * \author Peter Doerfler
 * \date   30.07.2005
 * 
 * $Id: ltiBresenhamCircle_template.h,v 1.2 2005-08-09 14:34:34 arndh Exp $
 */

#ifndef _LTI_BRESENHAM_LINE_TEMPLATE_H_
#define _LTI_BRESENHAM_LINE_TEMPLATE_H_

namespace lti {


  // Implementation adapted from 
  // http://www.cs.fit.edu/~wds/classes/graphics/Rasterize/brescircle.html
  template<typename POINT_STORE, typename T>
  bool bresenhamCircle::renderCircle(const int cx, const int cy,
                                     const int rad, 
                                     POINT_STORE& store, T color) const{
    

    int x = 0;
    int y = rad;
    int g = 3 - 2*rad;
    int diagonalInc = 10 - 4*rad;
    int rightInc = 6;
    while (x <= y) {
      addPoints(cx,cy,x,y,store,color);
      if (g >=  0) {
        g += diagonalInc; 
        diagonalInc += 8;
        y--;
      } else {
        g += rightInc;
        diagonalInc += 4;
      }
      rightInc += 4;
      x++;
    }
    return true;
  }

  
}

#endif
