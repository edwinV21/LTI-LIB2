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
 * \file   ltiDecimation_template.h
 * \author Pablo Alvarado
 * \date   6.12.2000
 *
 * $Id: ltiDecimation_template.h,v 1.1 2005-06-30 16:34:08 gquiros Exp $
 */

namespace lti {

  // -------------------------------------------------------------------
  // The apply-methods!
  // -------------------------------------------------------------------
  // On place apply for type matrix<T>!
  template <class T>
  bool decimation::apply(matrix<T>& srcdest) const {
    matrix<T> tmp;
    if (apply(srcdest,tmp)) {
      tmp.detach(srcdest);
      return true;
    }
    return false;
  }

  // On place apply for type vector<T>!
  template <class T>
  bool decimation::apply(vector<T>& srcdest) const {
    vector<T> tmp;
    if (apply(srcdest,tmp)) {
      tmp.detach(srcdest);
      return true;
    }
    return false;
  }
  
  // On copy apply for type matrix<T>!
  template <class T>
  bool decimation::apply(const matrix<T>& src,matrix<T>& dest) const {
    ipoint newSize;
    const parameters& param = getParameters();
    
    newSize.x = (src.columns()+(param.factor.x-1))/param.factor.x;
    newSize.y = (src.rows()+(param.factor.y-1))/param.factor.y;
    
    dest.resize(newSize,T(),AllocateOnly);
    int y,x,yy;
    typename vector<T>::iterator it,eit;
    
    for (y=0,yy=0;yy<dest.rows();y+=param.factor.y,++yy) {
      vector<T>& vct = dest.getRow(yy);
      for (x=0,it=vct.begin(),eit=vct.end();it!=eit;x+=param.factor.x,++it) {
        (*it)=src.at(y,x);
      }
    }
    
    return true;
  }
  
  // On copy apply for type vector<T>!
  template <class T>
  bool decimation::apply(const vector<T>& src,vector<T>& dest) const {
    int newSize;
    const parameters& param = getParameters();
    
    newSize = (src.columns()+(param.factor.x-1))/param.factor.x;
    
    dest.resize(newSize,T(),false,false);
    int x;
    typename vector<T>::iterator it,eit;
    
    for (x=0,it=dest.begin(),eit=dest.end();it!=eit;x+=param.factor.x,++it) {
      (*it)=src.at(x);
    }
    
    return true;
  }
  
}
