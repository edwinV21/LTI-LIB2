/*
 * Copyright (C) 1998-2004
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
 * \file   ltiUsePalette.cpp
 * \author Pablo Alvarado
 * \date   8.5.2001
 *
 * $Id: ltiUsePalette.cpp,v 1.7 2007-04-05 22:56:59 alvarado Exp $
 */

#include "ltiUsePalette.h"

namespace lti {
  // --------------------------------------------------
  // usePalette::parameters
  // --------------------------------------------------

  // default constructor
  usePalette::parameters::parameters()
    : functor::parameters() {

    colors.allocate(256);
    for (int i=0;i<colors.size();++i) {
      colors.at(i)=rgbaPixel(i,i,i);
    }

    linearSearch=false;
    kdTreeOnDemand=false;
    bucketSize=16;

  }

  // copy constructor
  usePalette::parameters::parameters(const parameters& other)
    : functor::parameters()  {
    copy(other);
  }

  // destructor
  usePalette::parameters::~parameters() {
  }

  // get type name
  const std::string& usePalette::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // copy member

  usePalette::parameters&
    usePalette::parameters::copy(const parameters& other) {
    functor::parameters::copy(other);
    colors.copy(other.colors);
    linearSearch=other.linearSearch;
    kdTreeOnDemand=other.kdTreeOnDemand;
    bucketSize=other.bucketSize;

    return *this;
  }

  // alias for copy member
  usePalette::parameters&
    usePalette::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  // clone member
  usePalette::parameters* usePalette::parameters::clone() const {
    return new parameters(*this);
  }

  usePalette::parameters* usePalette::parameters::newInstance() const {
    return new parameters;
  }

  /*
   * write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool usePalette::parameters::write(ioHandler& handler,
                                     const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {

      lti::write(handler,"colors",colors);
      lti::write(handler,"linearSearch",linearSearch);
      lti::write(handler,"kdTreeOnDemand",kdTreeOnDemand);
      lti::write(handler,"bucketSize",bucketSize);
    }

    b = b && functor::parameters::write(handler,false);

    if (complete) {
      b = b && handler.writeEnd();
    }

    return b;
  }

  /*
   * read the parameters from the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also read, otherwise only the data block will be read.
   * @return true if write was successful
   */
  bool usePalette::parameters::read(ioHandler& handler,
                                    const bool complete) {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {

      lti::read(handler,"colors",colors);
      lti::read(handler,"linearSearch",linearSearch);
      lti::read(handler,"kdTreeOnDemand",kdTreeOnDemand);
      lti::read(handler,"bucketSize",bucketSize);

    }

    b = b && functor::parameters::read(handler,false);
    
    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

  // --------------------------------------------------
  // usePalette
  // --------------------------------------------------

  // default constructor
  usePalette::usePalette()
    : functor(){
    // create an instance of the parameters with the default values
    parameters defaultParameters;
    // set the default parameters
    setParameters(defaultParameters);

    treeBuilded_=false;
  }

  // copy constructor
  usePalette::usePalette(const usePalette& other)
    : functor()  {
    copy(other);
  }

  // destructor
  usePalette::~usePalette() {
  }

  // returns the name of this type
  const std::string& usePalette::name() const {
    _LTI_RETURN_CLASS_NAME;
  }

  // copy member
  usePalette& usePalette::copy(const usePalette& other) {
    functor::copy(other);
    treeBuilded_=other.treeBuilded_;
    tree_.copy(other.tree_);
    return (*this);
  }

  // clone member
  usePalette* usePalette::clone() const {
    return new usePalette(*this);
  }

  // clone member
  usePalette* usePalette::newInstance() const {
    return new usePalette;
  }

  // return parameters
  const usePalette::parameters&
  usePalette::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }


    /**
     * set functor's parameters.
     * This member makes a copy of <em>theParam</em>: the functor
     * will keep its own copy of the parameters!
     * @return true if successful, false otherwise
     */
  bool usePalette::updateParameters() {
    const parameters& param = getParameters();
    
    if (!param.linearSearch && !param.kdTreeOnDemand) {
      buildKdTree(param.colors);
    } else {
      treeBuilded_=false;
    }
    
    return true;
  } 

  // -------------------------------------------------------------------
  // The apply-methods!
  // -------------------------------------------------------------------

  /*
   * on-copy
   * takes the channel8 and uses its elements as index for
   * the palette in the parameters.  The result will be left in a
   * new image.
   *
   * @param chnl the matrix<ubyte> with the indices for the palette
   * @param other the resulting image
   * @return true if successful, false otherwise
   */
  bool usePalette::apply(const matrix<ubyte>& chnl,image& other) const {
    const parameters& param = getParameters();
    return apply(chnl,param.colors,other);
  }

  /*
   * on-copy
   * takes the matrix<int> and uses its elements as index for the palette in
   * the parameters.
   * The result will be left in a new image.
   *
   * @param chnl the matrix<int> with the indices for the palette
   * @param other the resulting image
   * @return true if successful, false otherwise
   */
  bool usePalette::apply(const matrix<int>& chnl,image& other) const {
    const parameters& param = getParameters();
    return apply(chnl,param.colors,other);
  }

  /*
   * shortcut for apply with a palette
   * with this shortcut you don't need to preset the parameters!
   */
  bool usePalette::apply(const matrix<ubyte>& chnl,
                         const palette& thePalette,
                         image& other) const {
    int y;
    vector<ubyte>::const_iterator it,eit;
    image::iterator iit;

    if (thePalette.empty()) {
      setStatusString("Palette empty.");
      return false;
    }

    // this ensure a connected image
    other.resize(chnl.size(),rgbaPixel(),AllocateOnly);

    for (y=0,iit=other.begin();y<chnl.rows();++y) {
      const vector<ubyte>& vct = chnl.getRow(y);
      for (it=vct.begin(),eit=vct.end();it!=eit;++it,++iit) {
        (*iit)=thePalette.at(*it);
      }
    }

    return true;
  }

  /*
   * shortcut for apply with a palette
   * with this shortcut you don't need to preset the parameters!
   */
  bool usePalette::apply(const matrix<int>& chnl,
                         const palette& thePalette,
                         image& other) const {
    int y;
    vector<int>::const_iterator it,eit;
    image::iterator iit;

    if (thePalette.empty()) {
      setStatusString("Palette empty.");
      return false;
    }

    // this ensure a connected image
    other.resize(chnl.size(),rgbaPixel(),AllocateOnly);

    for (y=0,iit=other.begin();y<chnl.rows();++y) {
      const vector<int>& vct = chnl.getRow(y);
      for (it=vct.begin(),eit=vct.end();it!=eit;++it,++iit) {
        (*iit)=thePalette.at(*it);
      }
    }
    return true;
  }

  // GRAY VALUED

  /*
   * shortcut for apply with a palette
   * with this shortcut you don't need to preset the parameters!
   */
  bool usePalette::apply(const matrix<ubyte>& chnl,
                         const fvector& thePalette,
                         fmatrix& other) const {
    int y;
    vector<ubyte>::const_iterator it,eit;
    fmatrix::iterator iit;

    if (thePalette.empty()) {
      setStatusString("Palette empty.");
      return false;
    }

    // this ensures a connected image
    other.resize(chnl.size(),float(),AllocateOnly);

    for (y=0,iit=other.begin();y<chnl.rows();++y) {
      const vector<ubyte>& vct = chnl.getRow(y);
      for (it=vct.begin(),eit=vct.end();it!=eit;++it,++iit) {
        (*iit)=thePalette.at(*it);
      }
    }

    return true;
  }

  /*
   * shortcut for apply with a palette
   * with this shortcut you don't need to preset the parameters!
   */
  bool usePalette::apply(const matrix<int>& chnl,
                         const fvector& thePalette,
                         fmatrix& other) const {
    int y;
    vector<int>::const_iterator it,eit;
    fmatrix::iterator iit;

    if (thePalette.empty()) {
      setStatusString("Palette empty.");
      return false;
    }

    // this ensure a connected image
    other.resize(chnl.size(),float(),AllocateOnly);

    for (y=0,iit=other.begin();y<chnl.rows();++y) {
      const vector<int>& vct = chnl.getRow(y);
      for (it=vct.begin(),eit=vct.end();it!=eit;++it,++iit) {
        (*iit)=thePalette.at(*it);
      }
    }
    return true;
  }

  bool usePalette::apply(const image& img, matrix<ubyte>& chnl) {
    const parameters& param = getParameters();

    if (param.linearSearch) {
      return apply(img,param.colors,chnl);
    } else {
      if (!treeBuilded_) {
        buildKdTree(param.colors);
      }

      return apply(img,tree_,chnl);
    }

    return false;
  }

  bool usePalette::apply(const image& img, matrix<int>& chnl) {
    const parameters& param = getParameters();

    if (param.linearSearch) {
      return apply(img,param.colors,chnl);
    } else {
      if (!treeBuilded_) {
        buildKdTree(param.colors);
      }

      return apply(img,tree_,chnl);
    }

    return false;
  }

  bool usePalette::apply(const image& img,
			 const palette& thePalette,
			 matrix<ubyte>& chnl) {
    if (thePalette.size() > 256) {
      setStatusString("Palette has more than 256 entries.");
      return false;
    }

    if (thePalette.empty()) {
      setStatusString("Palette empty.");
      return false;
    }

    int y;
    int tDist;
    vector<image::value_type>::const_iterator it,eit;
    matrix<ubyte>::iterator iit;

    const parameters& par = getParameters();

    if (par.linearSearch) {
      // this ensure a connected image
      chnl.resize(img.size(),ubyte(0),AllocateOnly);

      for (y=0,iit=chnl.begin();y<img.rows();++y) {
        const vector<image::value_type>& vct = img.getRow(y);
        for (it=vct.begin(),eit=vct.end();it!=eit;++it,++iit) {
          int minDist = 195075; //255*255*3
          for(int palIdx=0;palIdx<thePalette.size();++palIdx) {
            if((tDist=thePalette.at(palIdx).distanceSqr(*it))<minDist) {
              minDist = tDist;
              (*iit)=palIdx;
            }
          }
        }
      }
    } else if (buildKdTree(par.colors)) {
      return apply(img,tree_,chnl);
    }
  
    return true;
  }

  bool usePalette::apply(const image& img,
			 const palette& thePalette,
			 matrix<int>& chnl) {
    int y;
    int tDist;
    vector<image::value_type>::const_iterator it,eit;
    matrix<int>::iterator iit;

    if (thePalette.empty()) {
      setStatusString("Palette empty.");
      return false;
    }

    const parameters& par = getParameters();

    if (par.linearSearch) {
      // this ensures a connected image
      chnl.resize(img.size(),int(0),AllocateOnly);

      for (y=0,iit=chnl.begin();y<img.rows();++y) {
        const vector<image::value_type>& vct = img.getRow(y);
        for (it=vct.begin(),eit=vct.end();it!=eit;++it,++iit) {
          int minDist = 195076; //255*255*3+1
          for(int palIdx=0;palIdx<thePalette.size();++palIdx) {
            if((tDist=thePalette.at(palIdx).distanceSqr(*it))<minDist) {
              minDist = tDist;
              (*iit)=palIdx;
            }
          }
        }
      }
    } else if (buildKdTree(par.colors)) {
      return apply(img,tree_,chnl);
    }

    return true;
  }


  bool usePalette::apply(const image& img,
			 const kdTree<rgbaPixel,int>& tree,
			 matrix<ubyte>& chnl) const {
    int y;
    int tDist(0);
    vector<image::value_type>::const_iterator it,eit;
    vector<ubyte>::iterator iit;

    if (tree.empty()) {
      setStatusString("k-d tree empty");
      return false;
    }

    // this ensure a connected image
    chnl.resize(img.size(),ubyte(),AllocateOnly);

    for (y=0;y<img.rows();++y) {
      const vector<image::value_type>& vct = img.getRow(y);
      iit=chnl.getRow(y).begin();
      for (it=vct.begin(),eit=vct.end();it!=eit;++it,++iit) {
        tree.searchNearest((*it),tDist);
        (*iit)=static_cast<ubyte>(tDist);
      }
    }

    return true;
  }

  bool usePalette::apply(const image& img,
			 const kdTree<rgbaPixel,int>& tree,
			 matrix<int>& chnl) const {
    int y;
    vector<image::value_type>::const_iterator it,eit;
    vector<int>::iterator iit;

    // this ensure a connected image
    chnl.resize(img.size(),int(),AllocateOnly);

    for (y=0;y<img.rows();++y) {
      const vector<image::value_type>& vct = img.getRow(y);
      iit=chnl.getRow(y).begin();
      for (it=vct.begin(),eit=vct.end();it!=eit;++it,++iit) {
        tree.searchNearest((*it),(*iit));
      }
    }

    return true;
  }

  bool usePalette::buildKdTree(const palette& pal) {
    const parameters& par = getParameters();
    tree_.clear();
    palette::const_iterator it,eit;
    int i=0;
    for (it=pal.begin(),eit=pal.end();it!=eit;++it,++i) {
      tree_.add((*it),i);
    }

    return (treeBuilded_ = tree_.build(par.bucketSize));
  }

  /*
   * Get a constant reference to the internal k-d tree.
   */
  const kdTree<rgbaPixel,int>& usePalette::getKdTree() const {
    return tree_;
  }


}





