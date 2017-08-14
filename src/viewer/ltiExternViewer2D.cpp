/*
 * Copyright (C) 2007 by Pablo Alvarado
 * 
 * This file is part of the LTI-Computer Vision Library 2 (LTI-Lib-2)
 *
 * The LTI-Lib-2 is free software; you can redistribute it and/or
 * modify it under the terms of the BSD License.
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the authors nor the names of its contributors may be
 *    used to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/** 
 * \file   ltiExternViewer2D.cpp
 *         Contains the class lti::externViewer used when there is no 
 *         possibility to use GTK or any other widget tool for which we have a
 *         viewer
 * \author Pablo Alvarado
 * \date   21.07.99
 *
 * revisions ..: $Id: ltiExternViewer2D.cpp,v 1.3 2008-11-03 17:23:49 alvarado Exp $
 */

#include <string>
#include <cstdio>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>

#include "ltiObject.h"
#include "ltiRGBAPixel.h"

#ifndef _LTI_WIN32

// This is for unixes and macs
# include <dirent.h>
# include <unistd.h>
# include <cstdlib> // for system()

#else // if _LTI_WIN32

// This is for windows only
# include <windows.h>
# include <process.h>
# include "ltiThread.h"

#endif //

#include "ltiException.h"
#include "ltiExternViewer2D.h"
#include "ltiIOImage.h"

namespace lti {

  // default constructor for the externViewer2D parameters
  externViewer2D::parameters::parameters()  : parametersManager::parameters() {

# if defined(_LTI_WIN32)
    tmpDirectory = "C:\\TEMP";
    externViewerApp    = "mspaint";
# else
    tmpDirectory = "/tmp";
#   if defined(_LTI_MACOSX) 
    externViewerApp    = "open -a Preview";
#   else
    externViewerApp    = "kview"; // Let's assume KDE is everywhere
#   endif
# endif

  }

  // copy constructor
  externViewer2D::parameters::parameters(const parameters& other)
  : parametersManager::parameters() {

    copy(other);
  }

  // destructor
  externViewer2D::parameters::~parameters() {
  }
  
  /*
   * Returns the name of this class
   */
  const std::string& externViewer2D::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // copy member
  externViewer2D::parameters&
  externViewer2D::parameters::copy(const parameters& other) {

    tmpDirectory = other.tmpDirectory;
    externViewerApp  = other.externViewerApp;

    return *this;
  }

  externViewer2D::parameters&
  externViewer2D::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  // clone member
  externViewer2D::parameters* externViewer2D::parameters::clone() const {
    return new parameters(*this);
  }

  // new instance  member
  externViewer2D::parameters* externViewer2D::parameters::newInstance() const {
    return new parameters;
  }

  /*
   * Write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool externViewer2D::parameters::write(ioHandler& handler,
                                         const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {
      lti::write(handler,"tmpDirectory",tmpDirectory);
      lti::write(handler,"externViewerApp",externViewerApp);
    }

    b = b && parametersManager::parameters::write(handler,false);

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
  bool externViewer2D::parameters::read(ioHandler& handler,
                                        const bool complete) {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {
      lti::read(handler,"tmpDirectory",tmpDirectory);
      lti::read(handler,"externViewerApp",externViewerApp);
    }

    b = b && parametersManager::parameters::read(handler,false);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

  // --------------------------------------------------
  // extern viewer
  // --------------------------------------------------

  // static variables initilialization
  int externViewer2D::numberShows = 0;

  externViewer2D::externViewer2D()
  : ioObject(),status(),parametersManager() {
    
    parameters defParam;
    setParameters(defParam);
  }


  externViewer2D::externViewer2D(const std::string&)
  : ioObject(),status(),parametersManager() {
    
    parameters defParam;
    setParameters(defParam);
  }


  externViewer2D::externViewer2D(const parameters& param)
  : ioObject(),status(),parametersManager() {
    
    setParameters(param);
  }

  externViewer2D::externViewer2D(const externViewer2D& other)
    :  ioObject(),status(),parametersManager() {
    copy(other);
  }

  externViewer2D::~externViewer2D() {
  }

  /*
   * returns the name of this type ("externViewer2D")
   */
  const std::string& externViewer2D::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  const externViewer2D::parameters& externViewer2D::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&parametersManager::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }

  bool externViewer2D::show(const matrix<rgbaPixel>& mimg) {
    const parameters& param = getParameters();
    image img;
    img.copy(mimg);

#  ifndef _LTI_WIN32
    // LINUX/UNIX:

    // this implementation still ignores 'externViewer2DWnd'!
    numberShows++; // one more...

    // 1. Creates file in the temporary directory

    // 1.1. Check if the temporary directory exists
    int theDir;

    struct stat dirStat;

    theDir = stat(param.tmpDirectory.c_str(),&dirStat);

    if ((theDir < 0) || (!S_ISDIR(dirStat.st_mode))) {
      // an error has occured
      setStatusString("Directory for temporary files not found.");
      return false;
    }

    // 1.2. Generate a name for the file
    pid_t pid;
    pid = getpid(); // get process id

    std::string filename;

    filename = param.tmpDirectory;

    if (param.tmpDirectory.rfind('/') < param.tmpDirectory.size()-1)
      filename += "/";

    char buffer[1024];
    sprintf(buffer,"lti_%X_%X.bmp",pid,numberShows);
    filename += buffer;

    // 1.3. Save image

    ioImage saveImg;
    saveImg.save(filename,img);

    // 2. Starts new process

    if ( (pid = fork()) < 0) {
      // an error has occured
      setStatusString("Problem with fork");
      return false;
    }
    else {
      if (pid == 0) {
        // child process displays image!
        std::string commandLine;

        commandLine = param.externViewerApp + " " + filename;

        if (system(commandLine.c_str()) < 0) {
          // an error has occured
          setStatusString("Problem calling external application");
          return false;
        }

        // delete file!
        commandLine = "rm " + filename;
        int retVal = system(commandLine.c_str());
        _exit( (retVal < 0) ? EXIT_FAILURE : retVal);
      }
    }

# else // WINDOWS IMPLEMENTATION

    // this implementation still ignores 'externViewer2DWnd'!
    numberShows++; // one more...

    // 1. Creates file in the temporary directory

    // 1.1. Check if the temporary directory exists
    int theDir;

    // the same thing for WIN32

    struct _stat dirStat;

    theDir = _stat(param.tmpDirectory.c_str(),&dirStat);
    if ((theDir < 0) || ((_S_IFDIR & dirStat.st_mode) == 0)) {
      // an error has occured
      setStatusString("Directory for temporal files not found");
      return false;
    }

    // 1.2. Generate a name for the file
    int pid;
    pid = _getpid(); // get process id

    std::string filename,batchFile;

    filename = param.tmpDirectory;

    if (param.tmpDirectory.rfind('\\') < param.tmpDirectory.size()-1)
      filename += "\\";

    batchFile = filename;

    char buffer[1024];
    sprintf(buffer,"lti_%X_%X.bmp",pid,numberShows);
    filename += buffer;
    sprintf(buffer,"lti_%X_%X.bat",pid,numberShows);
    batchFile += buffer;

    // 1.3. Save image

    ioImage saveImg;
    saveImg.save(filename,img);

    // 1.4 Save a batch file
    std::ofstream batch(batchFile.c_str());
    batch << "@echo off" << std::endl;
    batch << "start /w " << param.externViewerApp
          << " " << filename << std::endl;
    batch << "del " << filename << std::endl;
    batch << "del " << batchFile;
    batch.close();

    // 2. Start new process

    _spawnlp(_P_NOWAIT,batchFile.c_str(),
                       batchFile.c_str(),
                       NULL);

# endif
    return true;
  }

  /*
   * shows a channel or matrix of float
   * @param data the object to be shown.
   * @return true if successful, false otherwise.
   */
  bool externViewer2D::show(const matrix<float>& data) {
    image img;
    img.castFrom(data,true,true);
    return show(img);
  }

  /*
   * shows a matrix of doubles as a channel
   * @param data the object to be shown.
   * @return true if successful, false otherwise.
   */
  bool externViewer2D::show(const matrix<ubyte>& data) {
    image img;
    img.castFrom(data);
    return show(img);
  }

  /*
   * shows a matrix of integers as a channel
   * @param data the object to be shown.
   * @return true if successful, false otherwise.
   */
  bool externViewer2D::show(const matrix<int32>& data) {
    image img;
    img.castFrom(data,true,true);
    return show(img);
  }

  bool externViewer2D::hide() {
    setStatusString("hide not implemented for externViewer2D");
    return false;
  }

  externViewer2D* externViewer2D::clone() const {
    return new externViewer2D(*this);
  }

  externViewer2D* externViewer2D::newInstance() const {
    return new externViewer2D;
  }

  /**
   * copy data of "other" functor.
   */
  externViewer2D& externViewer2D::copy(const externViewer2D& other) {
    parametersManager::copy(other);
    return (*this);
  }

  /**
   * copy data of "other" functor.
   */
  externViewer2D& externViewer2D::operator=(const externViewer2D& other) {
    return copy(other);
  }



} // namespace lti
