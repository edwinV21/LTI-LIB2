/*
 * Copyright (C) 1998-2006
 * Electronics Engineering School, ITCR, Costa Rica
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
 * \file   ltiIOImage.cpp
 *         Parent class of all functors that write to or read from a file as
 *         its main task.
 * \author Pablo Alvarado
 * \date   11.01.2006
 *
 * revisions ..: $Id: ltiIOImage.cpp,v 1.5 2006-02-14 17:02:50 doerfler Exp $
 */


#include "ltiIOImage.h"
#include "ltiFactory.h"
#include <cctype>


// ---------------------------------------------------------------------------
// Some Debuggin Macros

#undef _LTI_DEBUG
// #define _LTI_DEBUG 4
#include "ltiDebug.h"

// end of debugging macro definitions
// ---------------------------------------------------------------------------

namespace lti {


  // ------------------------------------------------------------------------
  // ioImage 
  // ------------------------------------------------------------------------

  // construction
  ioImage::ioImage() 
    : ioImageInterface() {
    buildRepository();
  }

  ioImage::ioImage(const ioImage& other) 
    : ioImageInterface() {
    buildRepository();
    copy(other);
  }

  // destructor
  ioImage::~ioImage() {
    clearRepository();
  }

  // returns the current parameters
  const ioImage::parameters& ioImage::getParameters() const {
    const parameters* params =
      dynamic_cast<const parameters*>(&parametersManager::getParameters());
    if(isNull(params))
      throw invalidParametersException(name());
    return *params;
  }

  const std::string& ioImage::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  ioImage& ioImage::copy(const ioImage& other) {
    ioImageInterface::copy(other);
    return *this;
  }
  
  ioImage& ioImage::operator=(const ioImage& other) {
    ioImageInterface::copy(other);
    return *this;
  }

  ioImage* ioImage::clone() const {
    return new ioImage(*this);
  }

  ioImage* ioImage::newInstance() const {
    return new ioImage();
  }

  bool ioImage::load(const std::string& filename,image& chnl) {
    std::string ext = getFileExtension(filename);
    ioImageInterface* ptr = get(ext);
    if (isNull(ptr)) {
      setStatusString("Unknown image format " + ext);
      return false;
    }

    if (!ptr->load(filename,chnl)) {
      setStatusString(ptr->getStatusString());
      return false;
    }
    
    return true;
  }

  bool ioImage::load(const std::string& filename,
                     matrix<ubyte>& chnl,
                     palette& pal) {
    std::string ext = getFileExtension(filename);
    ioImageInterface* ptr = get(ext);
    if (isNull(ptr)) {
      setStatusString("Unknown image format " + ext);
      return false;
    }
    
    if (!ptr->load(filename,chnl,pal)) {
      setStatusString(ptr->getStatusString());
      return false;
    }

    return true;
  }

  bool ioImage::load(const std::string& filename,matrix<ubyte>& chnl) {
    std::string ext = getFileExtension(filename);
    ioImageInterface* ptr = get(ext);
    if (isNull(ptr)) {
      setStatusString("Unknown image format " + ext);
      return false;
    }

    if (!ptr->load(filename,chnl)) {
      setStatusString(ptr->getStatusString());
      return false;
    } 
    return true;
  }
  
  bool ioImage::load(const std::string& filename,
                              matrix<float>& chnl) {
    std::string ext = getFileExtension(filename);
    ioImageInterface* ptr = get(ext);
    if (isNull(ptr)) {
      setStatusString("Unknown image format " + ext);
      return false;
    }

    if (!ptr->load(filename,chnl)) {
      setStatusString(ptr->getStatusString());
      return false;
    }

    return true;
  }
  
  bool ioImage::load(const std::string& filename,
                     matrix<int32>& chnl,
                     palette& pal) {
    std::string ext = getFileExtension(filename);
    ioImageInterface* ptr = get(ext);
    if (isNull(ptr)) {
      setStatusString("Unknown image format " + ext);
      return false;
    }

    if (!ptr->load(filename,chnl,pal)) {
      setStatusString(ptr->getStatusString());
      return false;
    }

    return true;
  }
  
  bool ioImage::load(const std::string& filename,
                     matrix<int32>& chnl) {
    std::string ext = getFileExtension(filename);
    ioImageInterface* ptr = get(ext);
    if (isNull(ptr)) {
      setStatusString("Unknown image format " + ext);
      return false;
    }

    if (!ptr->load(filename,chnl)) {
      setStatusString(ptr->getStatusString());
      return false;
    }

    return true;
  }
  
  bool ioImage::checkHeader(const std::string& filename,
                            headerInformation& info) {
    std::string ext = getFileExtension(filename);
    ioImageInterface* ptr = get(ext);
    if (isNull(ptr)) {
      setStatusString("Unknown image format " + ext);
      return false;
    }

    if (!ptr->checkHeader(filename,info)) {
      setStatusString(ptr->getStatusString());
      return false;
    } 
      
    return true;
  }
  
  bool ioImage::save(const std::string& filename,
                     const image& img) {
    std::string ext = getFileExtension(filename);
    ioImageInterface* ptr = get(ext);
    if (isNull(ptr)) {
      setStatusString("Unknown image format " + ext);
      return false;
    }

    if (!ptr->save(filename,img)) {
      setStatusString(ptr->getStatusString());
      return false;
    }

    return true;
  }

  bool ioImage::save(const std::string& filename,
                     const matrix<ubyte>& chnl,
                     const palette& pal) {
    std::string ext = getFileExtension(filename);
    ioImageInterface* ptr = get(ext);
    if (isNull(ptr)) {
      setStatusString("Unknown image format " + ext);
      return false;
    }

    if (!ptr->save(filename,chnl,pal)) {
      setStatusString(ptr->getStatusString());
      return false;
    } 

    return true;
  }

  bool ioImage::save(const std::string& filename,
                     const matrix<ubyte>& chnl) {
    std::string ext = getFileExtension(filename);
    ioImageInterface* ptr = get(ext);
    if (isNull(ptr)) {
      setStatusString("Unknown image format " + ext);
      return false;
    }

    if (!ptr->save(filename,chnl)) {
      setStatusString(ptr->getStatusString());
      return false;
    }
    
    return true;
  }

  bool ioImage::save(const std::string& filename,
                     const matrix<float>& chnl) {
    std::string ext = getFileExtension(filename);
    ioImageInterface* ptr = get(ext);
    if (isNull(ptr)) {
      setStatusString("Unknown image format " + ext);
      return false;
    }

    if (!ptr->save(filename,chnl)) {
      setStatusString(ptr->getStatusString());
      return false;
    }

    return true;
  }
  
  bool ioImage::save(const std::string& filename,
                     const matrix<int32>& chnl,
                     const palette& pal) {
    std::string ext = getFileExtension(filename);
    ioImageInterface* ptr = get(ext);
    if (isNull(ptr)) {
      setStatusString("Unknown image format " + ext);
      return false;
    }

    if (!ptr->save(filename,chnl,pal)) {
      setStatusString(ptr->getStatusString());
      return false;
    }
    
    return true;
  }

  bool ioImage::save(const std::string& filename,
                     const matrix<int32>& chnl) {
    std::string ext = getFileExtension(filename);
    ioImageInterface* ptr = get(ext);
    if (isNull(ptr)) {
      setStatusString("Unknown image format " + ext);
      return false;
    }

    if (!ptr->save(filename,chnl)) {
      setStatusString(ptr->getStatusString());
      return false;
    }

    return true;
  }

  bool ioImage::buildRepository() {
    clearRepository();

    _lti_debug("ioImage::buildRepository()");

    const factory<ioImageInterface>& theFactoryInstance =
      factory<ioImageInterface>::getFactory();

    std::map<std::string,std::string> theList;
    theFactoryInstance.getRegisteredNames(theList);

    std::map<std::string,std::string>::const_iterator it;

    for (it=theList.begin();it!=theList.end();++it) {
      _lti_debug("  " << (*it).first << " -> " << (*it).second << std::endl);
      ioObjects_[(*it).first] = theFactoryInstance.newInstance((*it).first);
    }

    return !ioObjects_.empty();
  }
  
  bool ioImage::clearRepository() {
    std::map<std::string,ioImageInterface*>::iterator it;
    for (it = ioObjects_.begin();it!=ioObjects_.end();++it) {
      delete (*it).second;
      (*it).second = 0;
    }
    ioObjects_.clear();

    return ioObjects_.empty();
  }

  std::string ioImage::getFileExtension(const std::string& filename) const {

    //find extension in filename
    const std::string::size_type pointPos =
      filename.rfind('.',filename.size());
    std::string ext = filename.substr(pointPos+1,filename.size());

    //formate extension name
    std::string::iterator it;
    for(it=ext.begin();it!=ext.end();++it) {
	*it = toupper(*it);
    }

    return ext;
  }

  ioImageInterface* ioImage::get(const std::string& ext) {
    std::map<std::string,ioImageInterface*>::const_iterator it;
    it = ioObjects_.find(ext);
    if (it == ioObjects_.end()) {
      return static_cast<ioImageInterface*>(0);
    }
    return (*it).second;
  }

  bool ioImage::isSupportedExtension(const std::string& ext) const {
    std::string tmpext(ext);
    std::string::iterator extit;
    for(extit=tmpext.begin(); extit!=tmpext.end(); ++extit) {
	*extit = toupper(*extit);
    }

    std::map<std::string,ioImageInterface*>::const_iterator it;
    it = ioObjects_.find(tmpext);
    if (it == ioObjects_.end()) {
      return false;
    }
    return true;
  }
  
} // namespace lti

