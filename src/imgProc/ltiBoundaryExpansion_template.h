/*
 * Copyright (C) 2007
 * ITCR, Pablo Alvarado
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

/*
 * \file   ltiBoundaryExpansion_template.h
 *         Contains the class lti::boundaryExpansion, 
 *         which is used to expand an image with a border defined through the
 *         boundaryType parameter
 * 
 * \author Pablo Alvarado
 * \date   03.10.2007
 *
 * revisions ..: $Id: ltiBoundaryExpansion_template.h,v 1.4 2012-11-17 14:49:04 alvarado Exp $
 */

#include "ltiMetaProgrammingTools.h"

namespace lti {

  /*
   * The helper class is the one doing the job.
   *
   * Template techniques take care of instantiating efficient implementations.
   * Most functionality is concentrated here.
   *
   * Template types:
   * - T: source matrix type
   * - U: destination matrix type
   * - N: normalize (true) or just pass data through (false)
   */
  template<typename T,typename U,bool N>
  class boundaryExpansion::helperBase {
  public:
    /*
     * Constructor with the parameters
     */
    helperBase(const parameters& par);
    
  protected:

    /*
     * Cast type T to type U
     *
     * The template boolean indicates if normalization is required or not.
     */
    inline U cast(const T src) const;

    /*
     * Compute the final dest size and allocate the memory for the transfer
     */
    bool resize(const matrix<T>& src, matrix<U>& dest) const;

    /*
     * Fill the boundary content.  It assumes \c dest has already the main
     * block filled
     */
    bool boundary(const matrix<T>& src, matrix<U>& dest) const;

    /*
     * Fill the border with zeroes
     */
    bool zero(matrix<U>& dest) const;

    /*
     * Fill the border with constant extension
     */
    bool constant(matrix<U>& dest) const;

    /*
     * Function that maps an index src into a value that lies between 0 and 
     * size-1 (inclusive) using a mirrored approach
     */
    inline int mirror(const int src,const int size,const int last) const;

    /*
     * Fill the border with mirrored image, when
     * there is a negative border.
     */
    bool mirror(const matrix<T>& src,matrix<U>& dest) const;

    /*
     * Fill the border with periodic image,
     * when all borders are positive
     */
    bool periodic(matrix<U>& dest) const;

    /*
     * Fill the border with periodic image,
     * when there is a negative border.
     */
    bool periodic(const matrix<T>& src,matrix<U>& dest) const;

    /*
     * Reference to parameters
     */
    const parameters& param;

  };

  /*
   * Derived class for general implementations
   */
  template<typename T,typename U,bool N>
  class boundaryExpansion::helper 
    : public boundaryExpansion::helperBase<T,U,N> {
  public:
    /*
     * Constructor with the parameters
     */
    helper(const parameters& par);

    /*
     * Copy the main block of data.  The \c dest object has to have the proper
     * size already.  The template boolean indicates if normalization is
     * required or not.
     */
    bool transfer(const matrix<T>& src, matrix<U>& dest) const;

    /*
     * Operates on a copy of the given arguments.
     *
     * @param src matrix<T> with the source data.
     * @param dest matrix<T> where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const matrix<T>& src, matrix<U>& dest) const;

  };

  /*
   * Derived class for implementations where the input and output
   * types are the same.
   */
  template<typename T>
  class boundaryExpansion::helper<T,T,false> 
    : public boundaryExpansion::helperBase<T,T,false> {
  public:
    /*
     * Constructor with the parameters
     */
    helper(const parameters& par);

    /*
     * Copy the main block of data.  The \c dest object has to have the proper
     * size already.  The template boolean indicates if normalization is
     * required or not.
     */
    bool transfer(const matrix<T>& src, matrix<T>& dest) const;

    /*
     * Operates on a copy of the given arguments.
     *
     * @param src matrix<T> with the source data.
     * @param dest matrix<T> where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const matrix<T>& src, matrix<T>& dest) const;

  };

  // -----------------------------------------------------------------------
  //  Implementation of helperBase
  // -----------------------------------------------------------------------

  template<typename T,typename U,bool N>
  inline U boundaryExpansion::helperBase<T,U,N>::cast(const T src) const {
    if (N) {
      return static_cast<U>(src*typeInfo<U>::suggestedNorm())/
             static_cast<U>(typeInfo<T>::suggestedNorm());
    } else {
      return static_cast<U>(src);
    }
  }

  template<typename T,typename U,bool N>
  boundaryExpansion::helperBase<T,U,N>::helperBase(const parameters& par) 
    : param(par) {
  }
  
  template<typename T,typename U,bool N>
  bool boundaryExpansion::helperBase<T,U,N>::resize(const matrix<T>& src,
                                                    matrix<U>& dest) const {

    const int x = src.columns() + param.leftBorder + param.rightBorder;
    const int y = src.rows()    + param.topBorder  + param.bottomBorder;

    if ( (x<1) || (y<1) ) {
      dest.clear();
      return true;
    }
    dest.allocate(y,x);
    return true;
  }

  
  /*
   * Fill the border with zeroes
   */
  template<typename T,typename U,bool N>
  bool boundaryExpansion::helperBase<T,U,N>::zero(matrix<U>& dest) const {

    static const U val = 0;

    if (param.topBorder>0) {
      dest.fill(val,
                0,0,param.topBorder-1,dest.lastColumn());
    }

    if (param.bottomBorder>0) {
      dest.fill(val,
                dest.rows()-param.bottomBorder,0,
                dest.lastRow(),dest.lastColumn());
    }

    if (param.leftBorder>0) {
      dest.fill(val,
                max(0,param.topBorder),0,
                min(dest.lastRow(),dest.lastRow()-param.bottomBorder),
                param.leftBorder-1);
    }
    
    if (param.rightBorder>0) {
      dest.fill(val,
                max(0,param.topBorder),dest.columns()-param.rightBorder,
                min(dest.lastRow(),dest.lastRow()-param.bottomBorder),
                dest.lastColumn());
    }

    return true;
  }

  /*
   * Fill the border with constant extension
   */
  template<typename T,typename U,bool N>
  bool boundaryExpansion::helperBase<T,U,N>::constant(matrix<U>& dest) const {
    // first fill the left and right border 
    int y;

    const int iy = max(0,param.topBorder);
    const int ey = min(dest.rows(),dest.rows()-param.bottomBorder);
    const int lx = max(0,param.leftBorder);
    const int elx = lx-1;
    const int rx = min(dest.lastColumn(),dest.lastColumn()-param.rightBorder);
    const int irx = rx+1;
    const int erx = dest.lastColumn();

    for (y=iy;y<ey;++y) {
      vector<U>& vct = dest.getRow(y);
      vct.fill(vct.at(lx),0,elx);
      vct.fill(vct.at(rx),irx,erx);
    }
    
    // now we just need to replicate the rows above and below
    for (y=0;y<param.topBorder;++y) {
      dest.getRow(y).fill(dest.getRow(iy));
    }

    const int ly = ey-1;

    for (y=ey;y<dest.rows();++y) {
      dest.getRow(y).fill(dest.getRow(ly));
    }
    
    return true;
  }

    /*
     * Function that maps an index src into a value that lies between 0 and 
     * size-1 (inclusive) using a mirrored approach
     */
  template<typename T,typename U,bool N>
  inline int 
  boundaryExpansion::helperBase<T,U,N>::mirror(const int src,
                                               const int size,
                                               const int last) const {
    const int32 alpha(src%size);
    if (alpha>=0) {
      return (((src/size) & 0x00000001) != 0) ? last-alpha : alpha;
    }
    return (((src/size) & 0x00000001) == 0) ? -alpha-1 : size+alpha;
  }
  
  /*
   * Fill the border with mirrored image, when
   * there is a negative border.
   */
  template<typename T,typename U,bool N>
  bool boundaryExpansion::helperBase<T,U,N>::mirror(const matrix<T>& src,
                                                    matrix<U>& dest) const {

    // this can be improved to use iterators, but now just a preliminary
    // (slow) version
    int x,y;

    // top border
    const int eytb = max(0,min(param.topBorder,dest.rows()));
    const int rows = src.rows();
    const int cols = src.columns();
    const int lastRow = src.lastRow();
    const int lastCol = src.lastColumn();

    for (y=0;y<eytb;++y) {
      const int sy = mirror(y-param.topBorder,rows,lastRow);
      for (x=0;x<dest.columns();++x) {
        const int sx = mirror(x-param.leftBorder,cols,lastCol);
        dest.at(y,x) = cast(src.at(sy,sx));
      }
    }
    
    // bottom border
    const int iytb = min(max(0,dest.rows()-param.bottomBorder),dest.rows());

    for (y=iytb;y<dest.rows();++y) {
      
      const int sy = mirror(y-param.topBorder,rows,lastRow);
      for (x=0;x<dest.columns();++x) {
        const int sx = mirror(x-param.leftBorder,cols,lastCol);
        dest.at(y,x) = cast(src.at(sy,sx));
      }
    }

   
    // lateral borders
    const int exlb = max(0,min(param.leftBorder,dest.columns()));
    const int ixlb = min(max(0,dest.columns()-param.rightBorder),
                         dest.columns());

    for (y=eytb;y<iytb;++y) {
      const int sy = mirror(y-param.topBorder,rows,lastRow);
      
      // left border
      for (x=0;x<exlb;++x) {
        const int sx = mirror(x-param.leftBorder,cols,lastCol);
        dest.at(y,x) = cast(src.at(sy,sx));
      }
      
      // right border
      for (x=ixlb;x<dest.columns();++x) {
        const int sx = mirror(x-param.leftBorder,cols,lastCol);
        dest.at(y,x) = cast(src.at(sy,sx));
      }
    }

    return true;
  }

  /*
   * Fill the border with periodic image,
   * when all borders are positive
   */
  template<typename T,typename U,bool N>
  bool boundaryExpansion::helperBase<T,U,N>::periodic(matrix<U>& dest) const {

    // first the slowest part: fill the left and right borders
    int y;

    const int iy = max(0,param.topBorder);
    const int ey = min(dest.rows(),dest.rows()-param.bottomBorder);

    typename vector<U>::iterator sit,dit,eit;

    for (y=iy;y<ey;++y) {
      vector<U>& vct = dest.getRow(y);
      // the right border
      sit=vct.begin()+param.leftBorder;
      dit=vct.end()-param.rightBorder;
      eit=vct.end();
      for (;dit!=eit;++dit,++sit) {
        (*dit) = (*sit);
      }
      
      // the left border
      sit=vct.inverseBegin()-param.rightBorder;
      dit=vct.inverseEnd()+param.leftBorder;
      eit=vct.inverseEnd();
      for (;dit!=eit;--dit,--sit) {
        (*dit) = (*sit);
      }
    }
    
    // for the top border we need to get in a bottom-up way, just in
    // case the border is bigger than the image.
    int dy;

    for (dy=param.topBorder-1, y=dest.lastRow()-param.bottomBorder;
         dy>=0;
         --dy,--y) {
      dest.getRow(dy).fill(dest.getRow(y));
    }

    // for the bottom border we need to get top-down
    
    for (dy=dest.rows()-param.bottomBorder,y=param.topBorder;
         dy<dest.rows();
         ++dy,++y) {
      dest.getRow(dy).fill(dest.getRow(y));
    }

    return true;
  }
  
  /*
   * Fill the border with periodic image,
   * when there is a negative border.
   */
  template<typename T,typename U,bool N>
  bool boundaryExpansion::helperBase<T,U,N>::periodic(const matrix<T>& src,
                                                      matrix<U>& dest) const {

    // this can be improved to use iterators, but now just a preliminary
    // (slow) version
    int x,y;

    // top border
    const int eytb = max(0,min(param.topBorder,dest.rows()));

    for (y=0;y<eytb;++y) {
      const int sy = static_cast<uint32>(y-param.topBorder)%src.rows();
      for (x=0;x<dest.columns();++x) {
        const int sx = static_cast<uint32>(x-param.leftBorder)%src.columns();
        dest.at(y,x) = cast(src.at(sy,sx));
      }
    }
    
    // bottom border
    const int iytb = min(max(0,dest.rows()-param.bottomBorder),dest.rows());

    for (y=iytb;y<dest.rows();++y) {
      
      const int sy = static_cast<uint32>(y-param.topBorder)%src.rows();
      for (x=0;x<dest.columns();++x) {
        const int sx = static_cast<uint32>(x-param.leftBorder)%src.columns();
        dest.at(y,x) = cast(src.at(sy,sx));
      }
    }

   
    // lateral borders
    const int exlb = max(0,min(param.leftBorder,dest.columns()));
    const int ixlb = min(max(0,dest.columns()-param.rightBorder),
                         dest.columns());

    for (y=eytb;y<iytb;++y) {
      const int sy = static_cast<uint32>(y-param.topBorder)%src.rows();
      
      // left border
      for (x=0;x<exlb;++x) {
        const int sx = static_cast<uint32>(x-param.leftBorder)%src.columns();
        dest.at(y,x) = cast(src.at(sy,sx));
      }
      
      // right border
      for (x=ixlb;x<dest.columns();++x) {
        const int sx = static_cast<uint32>(x-param.leftBorder)%src.columns();
        dest.at(y,x) = cast(src.at(sy,sx));
      }
    }

    return true;
  }

  template<typename T,typename U,bool N>
  bool boundaryExpansion::helperBase<T,U,N>::boundary(const matrix<T>& src,
                                                      matrix<U>& dest) const {

    if (dest.empty()) {
      // ready!  nothing to do
      return true;
    }

    switch (param.boundaryType) {
    case NoBoundary:
      return true; // ready!
      break;
    case Zero:
      return zero(dest);
      break;
    case Mirror:
        return mirror(src,dest);
      break;
    case Periodic:
      if ( (param.topBorder<0)  || (param.bottomBorder<0) ||
           (param.leftBorder<0) || (param.rightBorder<0)  ) {
        return periodic(src,dest);
      } else {
        return periodic(dest);
      }
      break;
    case Constant:
      return constant(dest);
      break;
    default:
      return false;
    }
    
    return true;
  }

  // -------------------------------------------------------------------
  // The helper<T,U>
  // -------------------------------------------------------------------

  template<typename T,typename U,bool N>
  boundaryExpansion::helper<T,U,N>::helper(const parameters& par) 
    : helperBase<T,U,N>(par) {
    
  }

  template<typename T,typename U,bool N>
  bool boundaryExpansion::helper<T,U,N>::transfer(const matrix<T>& src,
                                                        matrix<U>& dest) const{

    if (dest.empty()) {
      // ready!  nothing to do
      return true;
    }

    int sy,dy;
    typename vector<T>::const_iterator sit;
    typename vector<U>::iterator dit,edit;

    // initialize the source y (sy) and destination y (dy) variables 
    if (this->param.topBorder<0) {
      sy =-this->param.topBorder;
      dy = 0;
    } else {
      sy = 0;
      dy = this->param.topBorder;
    }

    // last y value of the source
    const int esy =
      (this->param.bottomBorder<0) ? src.rows() + this->param.bottomBorder 
                                   : src.rows();
    if (sy>esy) {
      // upps! overlap of negative borders!  nothing to do here
      return true;
    }

    // from destination x
    const int fx = max(0,this->param.leftBorder);

    // to destination x
    const int tx = src.columns() +
                   this->param.leftBorder +
                   min(0,this->param.rightBorder);

    if (fx>tx) {
      // upps! overlap of negative borders!  nothing to do here
      return true;
    }

    const int fs = max(0,-this->param.leftBorder);

    for(;sy<esy;++sy,++dy) {
      const vector<T>& sv = src.getRow(sy);
      vector<U>& dv = dest.getRow(dy);
      for (sit=sv.begin()+fs,dit=dv.begin()+fx,edit=dv.begin()+tx;
           dit!=edit;
           ++sit,++dit) {
        (*dit) = this->cast(*sit);
      }
    }

    return true;

  }


  template<typename T,typename U,bool N>
  bool boundaryExpansion::helper<T,U,N>::apply(const matrix<T>& src,
                                                     matrix<U>& dest) const {
    return (this->resize(src,dest) &&
            transfer(src,dest) &&
            this->boundary(src,dest));
  }

  // -------------------------------------------------------------------
  // The helper<T,T>
  // -------------------------------------------------------------------

  template<typename T>
  boundaryExpansion::helper<T,T,false>::helper(const parameters& par) 
    : helperBase<T,T,false>(par) {
    
  }

  template<typename T>
  bool boundaryExpansion::helper<T,T,false>::transfer(const matrix<T>& src,
                                                       matrix<T>& dest) const {
    // while on the same type, no normalization is required, and fill can be
    // used
    if (dest.empty()) {
      // ready!  nothing to do
      return true;
    }

    int sy,dy;
    int sx,dx;

    // initialize the source y (sy) and destination y (dy) variables 
    if (this->param.topBorder<0) {
      sy=-this->param.topBorder;
      dy=0;
    } else {
      sy=0;
      dy=this->param.topBorder;
    }

    // initialize the source x (sx) and destination x (dx) variables 
    if (this->param.leftBorder<0) {
      sx=-this->param.leftBorder;
      dx=0;
    } else {
      sx=0;
      dx=this->param.leftBorder;
    }

    const int tx=src.columns() +
                 this->param.leftBorder-1 +
                 min(0,this->param.rightBorder);

    const int ty=src.rows() + 
                 this->param.topBorder-1 +
                 min(0,this->param.bottomBorder);

    dest.fill(src,dy,dx,ty,tx,sy,sx);
    
    return true;
  }


  template<typename T>
  bool boundaryExpansion::helper<T,T,false>::apply(const matrix<T>& src,
                                                   matrix<T>& dest) const {
    return (this->resize(src,dest) &&
            transfer(src,dest) &&
            this->boundary(src,dest));
  }

  // -------------------------------------------------------------------
  // The apply() member functions
  // -------------------------------------------------------------------
  
  // On place apply for type matrix<T>!
  template<typename T>
  bool boundaryExpansion::apply(matrix<T>& srcdest) const {
    helper<T,T,false> h(getParameters());
    matrix<T> tmp;
    if (h.apply(srcdest,tmp)) {
      tmp.detach(srcdest);
      return true;
    }

    return false;
  }

  // On copy apply for type matrix<T>!
  template<typename T,typename U>
  bool boundaryExpansion::apply(const matrix<T>& src, matrix<U>& dest) const {
    const parameters& par = getParameters();
    if (par.normalize) {
      helper<T,U,true> h(getParameters());
      return h.apply(src,dest);
    } else {
      helper<T,U,false> h(getParameters());
      return h.apply(src,dest);
    }
  }

}

