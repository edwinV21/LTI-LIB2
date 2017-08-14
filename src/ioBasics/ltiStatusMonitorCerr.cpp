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
 * \file   ltiStatusMonitorCerr.h
 *         Contains a status monitor that displays the information as soon as
 *         a setStatusString is called.
 * \author Pablo Alvarado
 * \date   04.05.2007
 *
 * $Id: ltiStatusMonitorCerr.cpp,v 1.1 2007/05/04 23:57:04 alvarado Exp $
 */

#include "ltiStatusMonitorCerr.h"
#include <iostream>

namespace lti {
  /*
   * Default constructor
   */
  statusMonitorCerr::statusMonitorCerr(std::ostream& theStream)
    : statusMonitor(),theStream_(&theStream) { 
  }

  statusMonitorCerr::statusMonitorCerr(const statusMonitorCerr& stMonitor)
    : statusMonitor(),theStream_(stMonitor.theStream_) { 
  }
  
  /*
   * Destructor
   */
  statusMonitorCerr::~statusMonitorCerr() {
    theStream_ = 0;
  }

  /*
   * Return the name of this class
   */
  const std::string& statusMonitorCerr::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  /*
   * Return a static instance of the current object type, which is not
   * clonable since it has a singleton pattern.
   */
  statusMonitorCerr* statusMonitorCerr::clone() const {
    return new statusMonitorCerr(*this);
  }

  /*
   * Return a static instance of the current object type
   */
  statusMonitorCerr* statusMonitorCerr::newInstance() const {
    return new statusMonitorCerr(std::cerr);
  }

  /*
   * Set a status string.
   *
   * @param id of the reporting class.  Usually you give here the output
   *        of name().
   * @param msg the const string to be reported.
   */
  void statusMonitorCerr::set(const std::string& id,const char* msg) {
    *theStream_ << id << ": " << msg << std::endl;
  }

  /*
   * Set a status string.
   *
   * @param id of the reporting class.  Usually you give here the output
   *        of name().
   * @param msg the const string to be reported.
   */
  void statusMonitorCerr::set(const std::string& id,const std::string& msg) {
    *theStream_ << id << ": " << msg << std::endl;
  }

  /*
   * Append a message to the current status string.
   *
   * @param id of the reporting class.  Usually you give here the output
   *        of name().
   * @param msg message to be appended to the current status string.
   */
  void statusMonitorCerr::append(const std::string& id,const char* msg) {
    *theStream_ << id << ": " << msg << std::endl;
  }

  /*
   * Append a message to the current status string.
   *
   * @param id of the reporting class.  Usually you give here the output
   *        of name().
   * @param msg message to be appended to the current status string.
   */
  void statusMonitorCerr::append(const std::string& id,
                                 const std::string& msg) {
    *theStream_ << id << ": " << msg << std::endl;
  }
  
}
