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
 * \file   ltiSort2_template.h
 *         Sort the elements in a matrix or vector
 * \author Pablo Alvarado
 * \date   17.08.2000
 *
 * $Id: ltiSort2_template.h,v 1.7 2008-08-06 03:40:58 alvarado Exp $
 */

namespace lti {
  

  // -------------------------------------------------------------------
  // The apply-methods!
  // -------------------------------------------------------------------


  template <typename U>
  void sort2::reorder(const vector<int>& indices,
                      const matrix<U>& src,
                            matrix<U>& dest) const {
    const parameters& param = getParameters();
    int i,j;

    dest.allocate(src.size());

    if (param.whichVectors == sort2::Rows) {
      for (i=0;i<indices.size();++i) {
        dest.getRow(i).copy(src.getRow(indices.at(i)));
      }
    } else {
      // reorder the columns
      for (i=0;i<indices.size();++i) {
        const int idx = indices.at(i);
        for (j=0;j<dest.rows();++j) {
          dest.at(j,i) = src.at(j,idx);
        }
      }
    }
  }

  // On place apply for type matrix<T>!
  template <typename T,typename U>
  bool sort2::apply(matrix<T>& key,matrix<U>& srcdest) const {
    if (key.size() != srcdest.size()) {
      this->setStatusString("Key and Value data must have the same size");
      return false;
    }

    vector<T> tmpvct;
    tmpvct.useExternData(key.rows()*key.columns(),
                         &key.at(0,0),
                         true);

    vector<U> tmpvct2;
    tmpvct2.useExternData(srcdest.rows()*srcdest.columns(),
                          &srcdest.at(0,0),
                          true);

    return apply(tmpvct,tmpvct2);
  }

  // On place apply for type vector<T>!
  template <typename T,typename U>
  bool sort2::apply(vector<T>& key,vector<U>& srcdest) const {
    if (key.size() != srcdest.size()) {
      this->setStatusString("Key and Value data must have the same size");
      return false;
    }

    quicksort(key,srcdest,0,srcdest.lastIdx());

    return true;
  }
  

  template <typename T,typename U>
  bool sort2::apply(vector<T>& key, matrix<U>& srcdest) const {
    if (key.size() != srcdest.rows()) {
      this->setStatusString("Vector size must be equal to the number of rows");
      return false;
    }

    int i;
    ivector indices(key.size());

    for (i=0;i<indices.size();++i) {
      indices.at(i) = i;
    }

    if (apply(key,indices)) {
      matrix<U> tmp;
      reorder(indices,srcdest,tmp);
      tmp.detach(srcdest);
    } else {
      return false;
    }

    return true;
  }

  template <typename T,typename U>
  bool sort2::apply(const vector<T>& key, const matrix<U>& src,
                          vector<T>& keydest, matrix<U>& dest) const {
    if (key.size() != src.rows()) {
      this->setStatusString("Vector size must be equal to the number of rows");
      return false;
    }

    keydest.copy(key);

    int i;
    ivector indices;
    indices.resize(key.size(),0,false,false);

    for (i=0;i<indices.size();++i) {
      indices.at(i) = i;
    }

    if (apply(keydest,indices)) {
      reorder(indices,src,dest);
    } else {
      return false;
    }

    return true;
  }

  // On copy apply for type matrix<T>!
  template <typename T,typename U>
  bool sort2::apply(const matrix<T>& key,const matrix<U>& src,
                          matrix<T>& keydest,matrix<U>& dest) const {
    if (key.size() != src.size()) {
      this->setStatusString("Key and Value data must have the same size");
      return false;
    }

    keydest.copy(key);
    dest.copy(src);

    return apply(keydest,dest);
  }

  // On copy apply for type vector<T>!
  template <typename T,typename U>
  bool sort2::apply(const vector<T>& key,const vector<U>& src,
                          vector<T>& keydest,vector<U>& dest) const {
    if (key.size() != src.size()) {
      this->setStatusString("Key and Value data must have the same size");
      return false;
    }

    keydest.copy(key);
    dest.copy(src);

    return apply(keydest,dest);
  }

  // use one vector to sort the other one

  template <typename T,typename U>
  void sort2::insertionsortAsc(vector<T>& vct,
                               vector<U>& vct2,
                               const int begin,
                               const int end) const {
    T a;
    U a2;

    int i,j;
    for (j=begin+1;j<=end;j++) {
      a = vct.at(j);
      a2 = vct2.at(j);
      for (i=j-1;i>=begin;i--) {
        if (!(a < vct.at(i))) {
          break;
        }
        vct.at(i+1)=vct.at(i);
        vct2.at(i+1)=vct2.at(i);
      }
      vct.at(i+1)=a;
      vct2.at(i+1)=a2;
    }
  }

  template <typename T,typename U>
  void sort2::insertionsortDesc(vector<T>& vct,
                                vector<U>& vct2,
                                const int begin,
                                const int end) const {
    T a;
    U a2;
    int i,j;
    for (j=begin+1;j<=end;j++) {
      a = vct.at(j);
      a2 = vct2.at(j);
      for (i=j-1;i>=begin;i--) {
        if (!(vct.at(i) < a)) {
          break;
        }
        vct.at(i+1)=vct.at(i);
        vct2.at(i+1)=vct2.at(i);
      }
      vct.at(i+1)=a;
      vct2.at(i+1)=a2;
    }
  }

  template <typename T,typename U>
  void sort2::quicksort(vector<T>& vct,
                        vector<U>& vct2,
                        const int begin,
                        const int end) const {

    int i;
    const parameters& par = getParameters();

    if (par.sortingOrder == Ascending) {

      if ((end-begin) > par.thresholdForBubble) {
        i = partitionAsc(vct,vct2,begin,end);
        quicksort(vct,vct2,begin,i-1);
        quicksort(vct,vct2,i+1,end);
      } else {
        insertionsortAsc(vct,vct2,begin,end);
      }

    } else {
      if (end-begin > par.thresholdForBubble) {
        i = partitionDesc(vct,vct2,begin,end);
        quicksort(vct,vct2,begin,i-1);
        quicksort(vct,vct2,i+1,end);
      } else {
        insertionsortDesc(vct,vct2,begin,end);
      }
    }
  }

  template <typename T,typename U>
  int sort2::partitionDesc(vector<T>& vct,
                           vector<U>& vct2,
                           const int begin,
                           const int end) const {

    int i,j;
    T v,t;
    U t2;

    v = vct.at(end);

    i = begin-1 ;
    j = end ;
    for(;;) {
      while (v < vct.at(++i)) {
      }

      while ( (--j >= 0) && (vct.at(j) < v) ) {
      }

      if ( i >= j ) {
        break ;
      }
      t=vct.at(i);
      vct.at(i)=vct.at(j);
      vct.at(j)=t;

      t2=vct2.at(i);
      vct2.at(i)=vct2.at(j);
      vct2.at(j)=t2;

    }

    t=vct.at(i);
    vct.at(i)=vct.at(end);
    vct.at(end)=t;

    t2=vct2.at(i);
    vct2.at(i)=vct2.at(end);
    vct2.at(end)=t2;

    return i;
  }

  template <typename T,typename U>
  int sort2::partitionAsc(vector<T>& vct,
                          vector<U>& vct2,
                          const int begin,
                          const int end) const {

    int i,j;
    T v,t;
    U t2;

    v = vct.at(end);
    i = begin-1 ;
    j = end ;
    for(;;) {
      while (vct.at(++i) < v) {
      }

      while ( (--j >= 0) && (v < vct.at(j)) ) {
      }

      if ( i >= j ) {
        break ;
      }
      t=vct.at(i);
      vct.at(i)=vct.at(j);
      vct.at(j)=t;

      t2=vct2.at(i);
      vct2.at(i)=vct2.at(j);
      vct2.at(j)=t2;
    }
    t=vct.at(i);
    vct.at(i)=vct.at(end);
    vct.at(end)=t;

    t2=vct2.at(i);
    vct2.at(i)=vct2.at(end);
    vct2.at(end)=t2;

    return i;
  }

}
