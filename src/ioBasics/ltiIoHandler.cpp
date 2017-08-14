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

/* 
 * \file   ltiIoHandler.cpp
 *         Parent class for all ioHandlers.
 * \author Pablo Alvarado
 * \date   07.12.2000
 *
 * $Id: ltiIoHandler.cpp,v 1.13 2009-12-04 04:10:23 alvarado Exp $
 */

#include "ltiIoHandler.h"
#include <sstream> // stringstream required to append data in the status string

namespace lti {

  // --------------------------------------------------
  // ioHandler
  // --------------------------------------------------



  // default constructor
  ioHandler::ioHandler() : object(),level_(0) {
  }

  // copy constructor
  ioHandler::ioHandler(const ioHandler& other) : object(), status() {
    copy(other);
  }

  // destructor
  ioHandler::~ioHandler() {
  }

  // copy member
  ioHandler&
  ioHandler::copy(const ioHandler& other) {
    level_ = other.level_;
    return (*this);
  }

  const int& ioHandler::getLevel() const {
    return level_;
  }

  void ioHandler::resetLevel(const int theLevel) {
    level_ = theLevel;
  }

  bool ioHandler::writeBegin() {
    level_++;
    return true;
  }

  bool ioHandler::writeEnd() {
    level_--;
    return (level_>=0);
  }

  bool ioHandler::readBegin() {
    level_++;
    return true;
  }
  bool ioHandler::readEnd() {
    level_--;
    return (level_>=0);
  }

  /* 
   * write members for symbol data pairs
   */
  bool ioHandler::write(const std::string& fname,const std::string& data) {
    return lti::write(*this,fname,data);
  }
  bool ioHandler::write(const std::string& fname,const char* data) {
    return lti::write(*this,fname,data);
  }
  bool ioHandler::write(const std::string& fname,const double data) {
    return lti::write(*this,fname,data);
  }
  bool ioHandler::write(const std::string& fname,const float data) {
    return lti::write(*this,fname,data);
  }
  bool ioHandler::write(const std::string& fname, const int data) {
    return lti::write(*this,fname,data);
  }
  bool ioHandler::write(const std::string& fname,const unsigned int data) {
    return lti::write(*this,fname,data);
  }
  bool ioHandler::write(const std::string& fname,const char data) {
    return lti::write(*this,fname,data);
  }
  bool ioHandler::write(const std::string& fname,const byte data) {
    return lti::write(*this,fname,data);
  }
  bool ioHandler::write(const std::string& fname,const ubyte data) {
    return lti::write(*this,fname,data);
  }
  bool ioHandler::write(const std::string& fname,const bool data) {
    return lti::write(*this,fname,data);
  }
  bool ioHandler::write(const std::string& fname,const long data) {
    return lti::write(*this,fname,data);
  }
  bool ioHandler::write(const std::string& fname,const unsigned long data) {
    return lti::write(*this,fname,data);
  }
  bool ioHandler::write(const std::string& fname,const short data) {
    return lti::write(*this,fname,data);
  }
  bool ioHandler::write(const std::string& fname,const unsigned short data) {
    return lti::write(*this,fname,data);
  }

  /*
   * write member for standard types
   */
  bool write(ioHandler& handler,const std::string& data) {
    return handler.write(data);
  }
  bool write(ioHandler& handler,const char* data) {
    return handler.write(data);
  }
  bool write(ioHandler& handler,const double data) {
    return handler.write(data);
  }
  bool write(ioHandler& handler,const float data) {
    return handler.write(data);
  }
  bool write(ioHandler& handler,const int data) {
    return handler.write(data);
  }
  bool write(ioHandler& handler,const unsigned int data) {
    return handler.write(data);
  }
  bool write(ioHandler& handler,const char data) {
    return handler.write(data);
  }
  bool write(ioHandler& handler,const byte data) {
    return handler.write(data);
  }
  bool write(ioHandler& handler,const ubyte data) {
    return handler.write(data);
  }
  bool write(ioHandler& handler,const bool data) {
    return handler.write(data);
  }
  bool write(ioHandler& handler,const long data) {
    return handler.write(data);
  }
  bool write(ioHandler& handler,const unsigned long data) {
    return handler.write(data);
  }
  bool write(ioHandler& handler,const short data) {
    return handler.write(data);
  }
  bool write(ioHandler& handler,const unsigned short data) {
    return handler.write(data);
  }

  /*
   * read members for symbol data pairs
   */
  bool ioHandler::read(const std::string& fname,std::string& data) {
    return lti::read(*this,fname,data);
  }
  bool ioHandler::read(const std::string& fname,double& data) {
    return lti::read(*this,fname,data);
  }
  bool ioHandler::read(const std::string& fname,float& data) {
    return lti::read(*this,fname,data);
  }
  bool ioHandler::read(const std::string& fname,int& data) {
    return lti::read(*this,fname,data);
  }
  bool ioHandler::read(const std::string& fname,unsigned int& data) {
    return lti::read(*this,fname,data);
  }
  bool ioHandler::read(const std::string& fname,char& data) {
    return lti::read(*this,fname,data);
  }
  bool ioHandler::read(const std::string& fname,byte& data) {
    return lti::read(*this,fname,data);
  }
  bool ioHandler::read(const std::string& fname,ubyte& data) {
    return lti::read(*this,fname,data);
  }
  bool ioHandler::read(const std::string& fname,bool& data) {
    return lti::read(*this,fname,data);
  }
  bool ioHandler::read(const std::string& fname,long& data) {
    return lti::read(*this,fname,data);
  }
  bool ioHandler::read(const std::string& fname,unsigned long& data) {
    return lti::read(*this,fname,data);
  }
  bool ioHandler::read(const std::string& fname,short& data) {
    return lti::read(*this,fname,data);
  }
  bool ioHandler::read(const std::string& fname,unsigned short& data) {
    return lti::read(*this,fname,data);
  }

  /* 
   * read member for standard types
   */
  bool read(ioHandler& handler,std::string& data) {
    return handler.read(data);
  }
  bool read(ioHandler& handler,double& data) {
    return handler.read(data);
  }
  bool read(ioHandler& handler,float& data) {
    return handler.read(data);
  }
  bool read(ioHandler& handler,int& data) {
    return handler.read(data);
  }
  bool read(ioHandler& handler,unsigned int& data) {
    return handler.read(data);
  }
  bool read(ioHandler& handler,char& data) {
    return handler.read(data);
  }
  bool read(ioHandler& handler,byte& data) {
    return handler.read(data);
  }
  bool read(ioHandler& handler,ubyte& data) {
    return handler.read(data);
  }
  bool read(ioHandler& handler,bool& data) {
    return handler.read(data);
  }
  bool read(ioHandler& handler,long& data) {
    return handler.read(data);
  }
  bool read(ioHandler& handler,unsigned long& data) {
    return handler.read(data);
  }
  bool read(ioHandler& handler,short& data) {
    return handler.read(data);
  }
  bool read(ioHandler& handler,unsigned short& data) {
    return handler.read(data);
  }

  void ioHandler::appendContextStatus() const {
  }

//----------------------------------------------
// definitions of global read function overloads
// for often used types
// needed for library size reduction 
//----------------------------------------------

// use a macro for the identical function bodies.
#define _LTI_GLOBAL_READ_FUNCTION_IMPLEMENTATION \
    int level = handler.getLevel(); \
    bool result = true; \
    if (complete) { \
      result = handler.readBegin(); \
    } \
    if (result && handler.trySymbol(fname)) { \
      result = result && handler.readKeyValueSeparator(); \
      result = result && read(handler,data); \
      if (complete) { \
        result = result && handler.readEnd(); \
      } \
      while (result && (handler.getLevel() > level)) { \
        result = result && handler.readEnd(); \
      } \
    } else { \
      result=false; \
      handler.appendStatusString("\nSymbol " + fname + " not found: "); \
      handler.appendContextStatus(); \
      handler.restoreLevel(); \
    } \
    return result;

  bool read(ioHandler& handler, const std::string& fname,
            std::string& data, const bool complete) {
    _LTI_GLOBAL_READ_FUNCTION_IMPLEMENTATION
  }
  bool read(ioHandler& handler, const std::string& fname,
            double& data, const bool complete) {
    _LTI_GLOBAL_READ_FUNCTION_IMPLEMENTATION
  }
  bool read(ioHandler& handler, const std::string& fname,
            float& data, const bool complete) {
    _LTI_GLOBAL_READ_FUNCTION_IMPLEMENTATION
  }
  bool read(ioHandler& handler, const std::string& fname,
            int& data, const bool complete) {
    _LTI_GLOBAL_READ_FUNCTION_IMPLEMENTATION
  }
  bool read(ioHandler& handler, const std::string& fname,
            unsigned int& data, const bool complete) {
    _LTI_GLOBAL_READ_FUNCTION_IMPLEMENTATION
  }
  bool read(ioHandler& handler, const std::string& fname,
            char& data, const bool complete) {
    _LTI_GLOBAL_READ_FUNCTION_IMPLEMENTATION
  }
  bool read(ioHandler& handler, const std::string& fname,
            byte& data, const bool complete) {
    _LTI_GLOBAL_READ_FUNCTION_IMPLEMENTATION
  }
  bool read(ioHandler& handler, const std::string& fname,
            ubyte& data, const bool complete) {
    _LTI_GLOBAL_READ_FUNCTION_IMPLEMENTATION
  }
  bool read(ioHandler& handler, const std::string& fname,
            bool& data, const bool complete) {
    _LTI_GLOBAL_READ_FUNCTION_IMPLEMENTATION
  }
  bool read(ioHandler& handler, const std::string& fname,
            long& data, const bool complete) {
    _LTI_GLOBAL_READ_FUNCTION_IMPLEMENTATION
  }
  bool read(ioHandler& handler, const std::string& fname,
            unsigned long& data, const bool complete) {
    _LTI_GLOBAL_READ_FUNCTION_IMPLEMENTATION
  }
  bool read(ioHandler& handler, const std::string& fname,
            short& data, const bool complete) {
    _LTI_GLOBAL_READ_FUNCTION_IMPLEMENTATION
  }
  bool read(ioHandler& handler, const std::string& fname,
            unsigned short& data, const bool complete) {
    _LTI_GLOBAL_READ_FUNCTION_IMPLEMENTATION
  }
// away with it
#undef _LTI_GLOBAL_READ_FUNCTION_IMPLEMENTATION

//----------------------------------------------
// definitions of global write function overloads
// for often used types
// needed for library size reduction 
//----------------------------------------------

// use a macro for this identical function bodies.
#define _LTI_GLOBAL_WRITE_FUNCTION_IMPLEMENTATION \
    if (complete) { \
      handler.writeBegin(); \
    } \
    handler.writeSymbol(fname); \
    handler.writeKeyValueSeparator(); \
    write(handler,data); \
    if (complete) { \
      handler.writeEnd(); \
    } \
    return handler.writeEOL(); \

  bool write(ioHandler& handler, const std::string& fname,
             const std::string& data, const bool complete) {
    _LTI_GLOBAL_WRITE_FUNCTION_IMPLEMENTATION
  }
  bool write(ioHandler& handler, const std::string& fname,
             const char* data, const bool complete) {
    _LTI_GLOBAL_WRITE_FUNCTION_IMPLEMENTATION
  }
  bool write(ioHandler& handler, const std::string& fname,
             const double data, const bool complete) {
    _LTI_GLOBAL_WRITE_FUNCTION_IMPLEMENTATION
  }
  bool write(ioHandler& handler, const std::string& fname,
             const float data, const bool complete) {
    _LTI_GLOBAL_WRITE_FUNCTION_IMPLEMENTATION
  }
  bool write(ioHandler& handler, const std::string& fname,
             const int data, const bool complete) {
    _LTI_GLOBAL_WRITE_FUNCTION_IMPLEMENTATION
  }
  bool write(ioHandler& handler, const std::string& fname,
             const unsigned int data, const bool complete) {
    _LTI_GLOBAL_WRITE_FUNCTION_IMPLEMENTATION
  }
  bool write(ioHandler& handler, const std::string& fname,
             const char data, const bool complete) {
    _LTI_GLOBAL_WRITE_FUNCTION_IMPLEMENTATION
  }
  bool write(ioHandler& handler, const std::string& fname,
             const byte data, const bool complete) {
    _LTI_GLOBAL_WRITE_FUNCTION_IMPLEMENTATION
  }
  bool write(ioHandler& handler, const std::string& fname,
             const ubyte data, const bool complete) {
    _LTI_GLOBAL_WRITE_FUNCTION_IMPLEMENTATION
  }
  bool write(ioHandler& handler, const std::string& fname,
             const bool data, const bool complete) {
    _LTI_GLOBAL_WRITE_FUNCTION_IMPLEMENTATION
  }
  bool write(ioHandler& handler, const std::string& fname,
             const long data, const bool complete) {
    _LTI_GLOBAL_WRITE_FUNCTION_IMPLEMENTATION
  }
  bool write(ioHandler& handler, const std::string& fname,
             const unsigned long data, const bool complete) {
    _LTI_GLOBAL_WRITE_FUNCTION_IMPLEMENTATION
  }
  bool write(ioHandler& handler, const std::string& fname,
             const short data, const bool complete) {
    _LTI_GLOBAL_WRITE_FUNCTION_IMPLEMENTATION
  }
  bool write(ioHandler& handler, const std::string& fname,
             const unsigned short data, const bool complete) {
    _LTI_GLOBAL_WRITE_FUNCTION_IMPLEMENTATION
  }
// begone devil!
#undef _LTI_GLOBAL_WRITE_FUNCTION_IMPLEMENTATION

}


