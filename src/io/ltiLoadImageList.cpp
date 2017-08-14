/*
 * Copyright (C) 2006
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
 * \file   ltiLoadImageList.cpp
 *         Contains the class loadImageList, which has still to be documented
 *         by Peter Doerfler.
 * \author Peter Doerfler
 * \date   10.2.2006
 *
 * revisions ..: $Id: ltiLoadImageList.cpp,v 1.6 2012-01-03 02:22:25 alvarado Exp $
 */

#include "ltiLoadImageList.h"

//includes for reading directory entries. Defaults to *NIX system
#ifdef _LTI_WIN32
#include <io.h>
#else
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <cstdlib>
#include <cstddef>
#endif

#include <fstream>

namespace lti {
  // --------------------------------------------------
  // loadImageList::parameters
  // --------------------------------------------------

  // default constructor
  loadImageList::parameters::parameters()
    : parametersManager::parameters() {
    
    continueOnError = false;
    fileType = "ALL";
  }

  // copy constructor
  loadImageList::parameters::parameters(const parameters& other)
    : parametersManager::parameters() {
    copy(other);
  }

  // destructor
  loadImageList::parameters::~parameters() {
  }

  // copy member

  loadImageList::parameters&
  loadImageList::parameters::copy(const parameters& other) {
    parametersManager::parameters::copy(other);

      continueOnError = other.continueOnError;
      fileType = other.fileType;

    return *this;
  }

  // alias for copy method
  loadImageList::parameters&
  loadImageList::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  // class name
  const std::string& loadImageList::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone method
  loadImageList::parameters* loadImageList::parameters::clone() const {
    return new parameters(*this);
  }

  // new instance
  loadImageList::parameters* loadImageList::parameters::newInstance() const {
    return new parameters();
  }

  /*
   * Write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool loadImageList::parameters::write(ioHandler& handler,
                                        const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {
      
      lti::write(handler,"continueOnError",continueOnError);
      lti::write(handler,"fileType",fileType);
    }

    b = b && parametersManager::parameters::write(handler,false);

    if (complete) {
      b = b && handler.writeEnd();
    }

    return b;
  }

  /*
   * Read the parameters from the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also read, otherwise only the data block will be read.
   * @return true if read was successful
   */
  bool loadImageList::parameters::read(ioHandler& handler,
                                        const bool complete)  {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {
      
      lti::read(handler,"continueOnError",continueOnError);
      lti::read(handler,"fileType",fileType);
    }

    b = b && parametersManager::parameters::read(handler,false);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

  // --------------------------------------------------
  // loadImageList
  // --------------------------------------------------

  const std::string loadImageList::emptyString_ = "";

  // default constructor
  loadImageList::loadImageList()
    : object(), parametersManager(), status(), loader_() {

    // create an instance of the parameters with the default values
    parameters defaultParameters;
    // set the default parameters
    setParameters(defaultParameters);
  }

  // default constructor
  loadImageList::loadImageList(const parameters& par)
    : object(), parametersManager(), status(), loader_() {

    // set the given parameters
    setParameters(par);
  }

  // copy constructor
  loadImageList::loadImageList(const loadImageList& other)
    : object(), parametersManager(), status(), loader_() {
    copy(other);
  }

  // destructor
  loadImageList::~loadImageList() {
  }

  // copy member
  loadImageList& 
  loadImageList::copy(const loadImageList& other) {
    parametersManager::copy(other);

    loader_.copy(other.loader_);
    filenames_.assign(other.filenames_.begin(), other.filenames_.end());
    loadAllImageTypes_ = other.loadAllImageTypes_;
    fileExt_ = other.fileExt_;

    return (*this);
  }

  // alias for copy member
  loadImageList&
  loadImageList::operator=(const loadImageList& other) {
    return (copy(other));
  }

  // class name
  const std::string& loadImageList::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone member
  loadImageList* loadImageList::clone() const {
    return new loadImageList(*this);
  }

  // create a new instance
  loadImageList* loadImageList::newInstance() const {
    return new loadImageList();
  }

  // return parameters
  const loadImageList::parameters&
  loadImageList::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&parametersManager::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }

  bool loadImageList::updateParameters() {

    std::string ext = getParameters().fileType;

    std::string::iterator it;
    for(it=ext.begin();it!=ext.end();++it) {
	*it = toupper(*it);
    }

    if (ext == "ALL") {
      loadAllImageTypes_ = true;
      fileExt_ = "";
    } else if (loader_.isSupportedExtension(ext)) {
      loadAllImageTypes_ = false;
      fileExt_ = ext;
    } else {
      std::string msg = "Unknown fileType " + getParameters().fileType + "\n";
      throw invalidParametersException(msg);
    }

    return true;
  }

  // -------------------------------------------------------------------
  // control functions
  // -------------------------------------------------------------------

  int loadImageList::computeSize() const {
    return static_cast<int>(filenames_.size());
  }

  void loadImageList::rewind() {
    it_ = filenames_.begin();
  }

  bool loadImageList::hasNext() const {
    return (!filenames_.empty() && (it_!=filenames_.end()));
  }

  void loadImageList::skip() {
    ++it_;
  }

  // -------------------------------------------------------------------
  // setting filenames_
  // -------------------------------------------------------------------

  bool loadImageList::useDirectory(const std::string& dirname,const bool rec) {

    filenames_.clear();

    std::string err;
    readDirectory(dirname,rec,err);
    if (!err.empty()) {
      setStatusString(err);
      return false;
    }
    return true;
  }


  bool loadImageList::readDirectory(const std::string& dirname,
                                    const bool rec,
                                    std::string& err) {

    std::string ndir,file,path;

    // fix dirname
    ndir = dirname;
    if (ndir[ndir.length()-1] != '/') {
      ndir += "/";
    }
    
#ifdef _LTI_WIN32
    // windows version
    struct _finddata_t entry;
    intptr_t handle;

    const std::string base = ndir+"*";
    handle=_findfirst(base.c_str(),&entry);
    if (handle != -1) {
      do {
        file = entry.name;
        if ((file == ".") || (file == "..")) {
          continue;
        }
        path = ndir + file;

        if (rec) {
          if ((entry.attrib & _A_SUBDIR)!=0) {
            if (!readDirectory(path,rec,err)) {
              return false;
            }
          }
        }

        if (!hasValidFileExtension(file)) {
          continue;
        }
        filenames_.push_back(path);

      } while (_findnext(handle,&entry) == 0);
      _findclose(handle);
    } else {
      err += std::string("Could not open directory ") + dirname + "\n";
      return false;
    }

#else
    // linux version
    DIR *dir=0;
   
    // check if the path directory exists
    dir = opendir(dirname.c_str());
    if (dir==NULL) {
      err += std::string("Could not open directory ") + dirname + "\n";
      return false;
    }

    const int len =
      offsetof(struct dirent, d_name) +
      pathconf(dirname.c_str(), _PC_NAME_MAX) + 1;

    dirent* entry = static_cast<dirent*>(malloc(len));
    dirent* result=0;

    // while still objects in directory
    int error;
    while (((error=readdir_r(dir,entry,&result)) == 0) && (notNull(result))) {
      file = entry->d_name;
      if ((file == ".") || (file == "..")) {
        continue;
      }
      path = ndir + file;
      if (rec) { // should we recurse?
        struct stat buf; // data structure where the file/dir info will be left
        if (stat(path.c_str(),&buf)==0) {
          // stat was successful.  If the file is a directory, recurse, or
          // otherwise just keep on
          if (S_ISDIR(buf.st_mode)) {
            if (!readDirectory(path,rec,err)) {
              free(entry);
              entry = 0;
              return false;
            }
          }
        }
      }

      if (!hasValidFileExtension(file)) {
        // not a known image file
        continue;
      }
      filenames_.push_back(path);
    }
    
    free(entry);
    entry = 0;

    closedir(dir);
#endif

    filenames_.sort(); // the documentation specifies this will be done!
    it_ = filenames_.begin();

    return true;
  }



  bool loadImageList::useFileList(const std::string& filename) {

    filenames_.clear();
    
    std::ifstream is;
    is.open(filename.c_str());
    if (!is.good() || is.eof()) {
      setStatusString("could not open file or empty file");
      return false;
    }
    std::string tmp;
    std::string::size_type pos;
    while (!is.eof()) {
      std::getline(is, tmp);

      // remove stuff after comment symbol
      pos = tmp.find(';');
      if (pos != std::string::npos) {
        tmp.erase(pos);
      }

      // just a comment or empty line
      if (tmp.empty()) {continue;}

      // trim whitespaces left and right
      pos = 0;
#ifdef _LTI_MSC_VER
      while (isspace(tmp[pos])) {++pos;};
#else
      while (std::isspace(tmp[pos])) {++pos;};
#endif
      if (pos > 0) {tmp.erase(0, pos-1);}
      const std::string::size_type len = pos = tmp.length()-1;
#ifdef _LTI_MSC_VER
      while (isspace(tmp[pos])) {--pos;};
#else
      while (std::isspace(tmp[pos])) {--pos;};
#endif
      if (pos < len) {tmp.erase(pos+1);}

      // could be an empty line now.
      if (!tmp.empty() || hasValidFileExtension(tmp)) {
        filenames_.push_back(tmp);
      }
    }

    if (filenames_.empty()) {
      setStatusString("No valid image filenames contained in ");
      appendStatusString(filename);
      return false;
    } else {
      it_ = filenames_.begin();
      return true;
    }
  }

  // -------------------------------------------------------------------
  // single load stuff
  // -------------------------------------------------------------------

  const std::string& loadImageList::getNextFilename() const {
    return *it_;
  }

  const std::string& loadImageList::getLastLoadedFilename() const {
    if (it_ == filenames_.begin()) {
      return emptyString_;
    } else {
      std::list<std::string>::const_iterator tit(it_);
      --tit;
      return *tit;
    }
  }

  bool loadImageList::load(image& img) {
    return loadHelp(img);
  }

  bool loadImageList::load(matrix<ubyte>& chnl) {
    return loadHelp(chnl);
  }

  bool loadImageList::load(matrix<ubyte>& chnl,
                           palette& pal) {
    return loadHelp(chnl, pal);
  }

  bool loadImageList::load(matrix<float>& chnl) {
    return loadHelp(chnl);
  }

  bool loadImageList::load(matrix<int32>& chnl) {
    return loadHelp(chnl);
  }

  bool loadImageList::load(matrix<int32>& chnl,
                           palette& pal) {
    return loadHelp(chnl, pal);
  }

  // -------------------------------------------------------------------
  // all images load stuff
  // -------------------------------------------------------------------

  void loadImageList::getFilenames(std::vector<std::string>& filenames) const {

    filenames.assign(filenames_.begin(), filenames_.end());
  }

  bool loadImageList::load(std::vector<image*>& img) {
    return loadHelp(img);
  }

  bool loadImageList::load(std::vector<matrix<ubyte>*>& chnl) {
    return loadHelp(chnl);
  }

  bool loadImageList::load(std::vector<matrix<ubyte>*>& chnl,
                           std::vector<palette*>& pal) {
    return loadHelp(chnl, pal);
  }

  bool loadImageList::load(std::vector<matrix<float>*>& chnl) {
    return loadHelp(chnl);
  }

  bool loadImageList::load(std::vector<matrix<int32>*>& chnl) {
    return loadHelp(chnl);
  }

  bool loadImageList::load(std::vector<matrix<int32>*>& chnl,
                           std::vector<palette*>& pal) {
    return loadHelp(chnl, pal);
  }
  

  // -------------------------------------------------------------------
  // helper functions
  // -------------------------------------------------------------------

  bool 
  loadImageList::hasValidFileExtension(const std::string& filename) const {

    //find extension in filename
    const std::string::size_type pointPos =
      filename.rfind('.',filename.size());
    std::string ext = filename.substr(pointPos+1,filename.size());

    //format extension name
    std::string::iterator it;
    for(it=ext.begin();it!=ext.end();++it) {
	*it = toupper(*it);
    }

    if (loadAllImageTypes_) {
      return loader_.isSupportedExtension(ext);
    } else {
      return (ext==fileExt_);
    }
  }

}

