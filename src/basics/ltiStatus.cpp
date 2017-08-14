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
 * \file   ltiStatus.cpp
 * \author Jochen Wickel
 * \author Thomas Rusert
 * \date   14.04.1999
 *
 * $Id: ltiStatus.cpp,v 1.6 2008-08-06 03:40:56 alvarado Exp $
 */


#include "ltiStatus.h"
#include "ltiClassName.h"
#include "ltiStatusMonitor.h"
#include "ltiObject.h"

#include <sstream>
#include <typeinfo>

namespace lti {

  // static attribute
  statusMonitor* status::statusMonitor_ = 0;

  // ----------------------------
  // status
  // ----------------------------

  // constructor
  status::status()
    : statusString_("") {
  }


  // destructor
  status::~status() {
    statusString_.clear();
  }

  // Generate class name
  const std::string status::getClassName() const {
    return className::demangle(typeid(*this).name());
  }
  
  /*
   * return the last message set with setStatusString().  This will
   * never return 0.  If no status-string has been set yet an empty string
   * (pointer to a string with only the char(0)) will be returned.
   */
  const std::string& status::getStatusString() const {
    return statusString_;
  }

  /*
   * set a status string.
   *
   * @param msg the const string to be reported next time by
   * getStatusString()
   * This message will be usually set within the apply methods to indicate
   * an error cause.
   */
  void status::setStatusString(const char* msg) const {
    statusString_ = msg;
    if (notNull(statusMonitor_)) {
      statusMonitor_->set(getClassName(),msg);
    }
  }

  void status::setStatusString(const std::string& msg) const {
    statusString_ = msg;
    if (notNull(statusMonitor_)) {
      statusMonitor_->set(getClassName(),msg);
    }
  }

  void status::appendStatusString(const char* msg) const {
    statusString_+=msg;
    if (notNull(statusMonitor_)) {
      statusMonitor_->append(getClassName(),msg);
    }
  }

  void status::appendStatusString(const std::string& msg) const {
    statusString_+=msg;
    if (notNull(statusMonitor_)) {
      statusMonitor_->append(getClassName(),msg);
    }
  }

  void status::appendStatusString(const int msg) const {
    std::ostringstream ostr;
    ostr << msg;
    std::string tmp=ostr.str();
    statusString_+=tmp;
    if (notNull(statusMonitor_)) {
      statusMonitor_->append(getClassName(),tmp);
    }
  }

  void status::appendStatusString(const double& msg) const {
    std::ostringstream ostr;
    ostr << msg;
    std::string tmp=ostr.str();
    statusString_+=tmp;
    if (notNull(statusMonitor_)) {
      statusMonitor_->append(getClassName(),tmp);
    }
  }

  void status::appendStatusString(const status& other) const {
		std::string tmp=other.getStatusString();
    statusString_+=other.getClassName();
		statusString_+=": ";
		statusString_+=tmp;
    if (notNull(statusMonitor_)) {
      statusMonitor_->append(getClassName(),statusString_);
    }
  }

  void status::setStatusMonitor(statusMonitor& monitor) {
    statusMonitor_ = monitor.clone();
  }

  /*
   * Global function
   */
  void setStatusMonitor(statusMonitor& monitor) {
    status::setStatusMonitor(monitor);
  }


} // namespace lti

