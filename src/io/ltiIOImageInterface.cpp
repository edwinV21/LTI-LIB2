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
 * \file   ltiIOImageInterface.cpp
 *         Parent class of all functors that write to or read from a file as
 *         its main task.
 * \author Pablo Alvarado
 * \date   11.01.2006
 *
 * revisions ..: $Id: ltiIOImageInterface.cpp,v 1.5 2009-11-13 12:15:08 alvarado Exp $
 */


#include "ltiIOImageInterface.h"
#include "ltiSplitImageTorgI.h"

namespace lti {
  // -----------------------------------------------------------------------
  // ioImageInterface::headerInformation
  // -----------------------------------------------------------------------

  ioImageInterface::headerInformation::headerInformation() : ioObject() {    
  }

  ioImageInterface::headerInformation::
  headerInformation(const headerInformation& other) : ioObject() {
    copy(other);
  }

  const std::string& ioImageInterface::headerInformation::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  ioImageInterface::headerInformation* 
  ioImageInterface::headerInformation::clone() const {
    return new headerInformation(*this);
  }

  ioImageInterface::headerInformation* 
  ioImageInterface::headerInformation::newInstance() const {
    return new headerInformation();
  }

  ioImageInterface::headerInformation& 
  ioImageInterface::headerInformation::copy(const headerInformation& other) {
    type = other.type;
    size = other.size;
    bpp  = other.bpp;
    colorEntries = other.colorEntries;

    return (*this);
  }

  ioImageInterface::headerInformation& 
  ioImageInterface::headerInformation::
  operator=(const headerInformation& other) {
    return copy(other);
  }

  bool ioImageInterface::headerInformation::
  write(ioHandler& handler, const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {
      lti::write(handler,"type",type);
      lti::write(handler,"size",size);
      lti::write(handler,"bpp",bpp);
      lti::write(handler,"colorEntries",colorEntries);
    }

    if (complete) {
      b = b && handler.writeEnd();
    }

    return b;    
  }

  bool ioImageInterface::headerInformation::
  read(ioHandler& handler, const bool complete) {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {
      lti::read(handler,"type",type);
      lti::read(handler,"size",size);
      lti::read(handler,"bpp",bpp);
      lti::read(handler,"colorEntries",colorEntries);
    }

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;    
  }

  // ------------------------------------------------------------------------
  // ioImage parameters
  // ------------------------------------------------------------------------

  ioImageInterface::parameters::parameters() 
    : parametersManager::parameters() {
  }
  
  ioImageInterface::parameters::parameters(const parameters& other)
    : parametersManager::parameters() {
    copy(other);
  }

  ioImageInterface::parameters::~parameters() {
  }

  // class name
  const std::string& ioImageInterface::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }


  ioImageInterface::parameters& 
  ioImageInterface::parameters::copy(const parameters& other) {
    parametersManager::parameters::copy(other);
    return (*this);
  }

  ioImageInterface::parameters& 
  ioImageInterface::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  bool ioImageInterface::parameters::
  write(ioHandler& handler,const bool complete) const {
    return parametersManager::parameters::write(handler,complete);
  }

  bool ioImageInterface::parameters::
  read(ioHandler& handler,const bool complete) {
    return parametersManager::parameters::read(handler,complete);    
  }

  // ------------------------------------------------------------------------
  // ioImageInterface 
  // ------------------------------------------------------------------------

  // construction
  ioImageInterface::ioImageInterface() 
    : object(),status(),parametersManager() {
  }

  ioImageInterface::ioImageInterface(const ioImageInterface& other) 
    : object(),status(),parametersManager() {
    copy(other);
  }

  // destructor
  ioImageInterface::~ioImageInterface() {
  }

  // returns the current parameters
  const ioImageInterface::parameters& ioImageInterface::getParameters() const {
    const parameters* params =
      dynamic_cast<const parameters*>(&parametersManager::getParameters());
    if(isNull(params))
      throw invalidParametersException(name());
    return *params;
  }

  const std::string& ioImageInterface::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  ioImageInterface& ioImageInterface::copy(const ioImageInterface& other) {
    parametersManager::copy(other);
    return *this;
  }
  
  ioImageInterface& 
  ioImageInterface::operator=(const ioImageInterface& other) {
    parametersManager::copy(other);
    return *this;
  }

  bool ioImageInterface::load(const std::string& filename,
                              matrix<ubyte>& chnl) {
    palette pal;
    return load(filename,chnl,pal);
  }
  
  bool ioImageInterface::load(const std::string& filename,
                              matrix<float>& chnl) {
    image img;
    if (load(filename,img)) {
      splitImageTorgI splitter;
      splitter.getIntensity(img,chnl);
      return true;
    }
    
    return false;
  }
  
  bool ioImageInterface::load(const std::string& filename,
                              matrix<int32>& chnl,
                              palette& pal) {
    matrix<ubyte> chnl8;
    if (load(filename,chnl8,pal)) {
      chnl.castFrom(chnl8);
      return true;
    }
    
    return false;
  }
  
  bool ioImageInterface::load(const std::string& filename,
                              matrix<int32>& chnl) {
    palette pal;
    return load(filename,chnl,pal);
  }
  

  const palette& ioImageInterface::getGrayPalette() const {
    static palette grayScale; // singleton
    if (grayScale.empty()) {  // initialization
      grayScale.allocate(256);
      for (int i=0;i<grayScale.size();++i) {
        grayScale.at(i).set(i,i,i,0);
      }
    }
    return grayScale;
  }

  bool ioImageInterface::save(const std::string& filename,
                                const matrix<ubyte>& chnl) {
    const palette& pal = getGrayPalette();
    return save(filename,chnl,pal);
  }

  bool ioImageInterface::save(const std::string& filename,
                              const matrix<float>& chnl) {
    
    channel tmpChnl;
    if (chnl.getMode() == fmatrix::Connected) {
      // const cast is allowed, since tmpChnl won't change anything
      tmpChnl.useExternData(chnl.rows(),chnl.columns(),
                            const_cast<float*>(chnl.data()));
    } else {
      tmpChnl.castFrom(chnl);
    }
    
    channel8 chnl8;
    chnl8.castFrom(tmpChnl); // assume values from 0 to 1
    
    return save(filename,chnl8);
  }

  bool ioImageInterface::save(const std::string& filename,
                                const matrix<int32>& chnl,
                                const palette& pal) {
    matrix<ubyte> chnl8;
    chnl8.castFrom(chnl);
    return save(filename,chnl8,pal);
  }

  bool ioImageInterface::save(const std::string& filename,
                                const matrix<int32>& chnl) {
    const palette& pal = getGrayPalette();
    return save(filename,chnl,pal);
  }


  
} // namespace lti

