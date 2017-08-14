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
 * \file   ltiStatusMonitor.h
 *         Contains the parent class for all status monitors, which are
 *         objects that control what to do with error reports in functors,
 *         classifiers, viewers and ioHandlers.
 * \author Pablo Alvarado
 * \date   30.07.2004
 *
 * $Id: ltiStatusMonitor.h,v 1.4 2007-05-04 23:57:04 alvarado Exp $
 */

#ifndef _LTI_STATUS_MONITOR
#define _LTI_STATUS_MONITOR

#include "ltiObject.h"

namespace lti {
  /**
   * Class statusMonitor
   *
   * Status monitors are objects that control what to do when an error is
   * reported:  should it just be ignored?  should it throw and exception?
   * should it be streamed out to std::cerr?
   *
   * You can call the global function lti::setStatusMonitor() to set
   * the object for the whole library.
   *
   * This is an abstract class.
   */
  class statusMonitor : public object {
  public:
    /**
     * Default constructor
     */
    statusMonitor();

    /**
     * Destructor
     */
    virtual ~statusMonitor();

    /**
     * Return the name of this class
     */
    virtual const std::string& name() const = 0;

    /**
     * Return a static instance of the current object type, which is not
     * clonable since it has a singleton pattern.
     */
    virtual statusMonitor* clone() const = 0;

    /**
     * Return a static instance of the current object type
     */
    virtual statusMonitor* newInstance() const = 0;

    /**
     * Set a status string.
     *
     * @param id of the reporting class.  Usually you give here the output
     *        of name().
     * @param msg the const string to be reported.
     */
     virtual void set(const std::string& id,const char* msg) = 0;

    /**
     * Set a status string.
     *
     * @param id of the reporting class.  Usually you give here the output
     *        of name().
     * @param msg the const string to be reported.
     */
    virtual void set(const std::string& id,const std::string& msg) = 0;

    /**
     * Append a message to the current status string.
     *
     * @param id of the reporting class.  Usually you give here the output
     *        of name().
     * @param msg message to be appended to the current status string.
     */
    virtual void append(const std::string& id,const char* msg) = 0;

    /**
     * Append a message to the current status string.
     *
     * @param id of the reporting class.  Usually you give here the output
     *        of name().
     * @param msg message to be appended to the current status string.
     */
    virtual void append(const std::string& id,const std::string& msg) = 0;
  };
  
}

#endif
