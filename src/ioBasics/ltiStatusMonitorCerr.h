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
 * $Id: ltiStatusMonitorCerr.h,v 1.1 2007-05-04 23:57:04 alvarado Exp $
 */

#include "ltiStatusMonitor.h"
#include <iostream>

namespace lti {
  /**
   * Class statusMonitorCerr
   *
   * Status monitors are objects that control what to do when an error
   * is reported.  In this particular case, the error is reported on
   * the std::cerr, or, if you want it so, on another std::stream you specify
   *
   * You can call the global function lti::setStatusMonitor() to set
   * the object for the whole library.
   *
   * This is an abstract class.
   */
  class statusMonitorCerr : public statusMonitor {
  public:
    /**
     * Default constructor
     */
    statusMonitorCerr(std::ostream& theStream = std::cerr);

    /**
     * Copy constructor
     */
    statusMonitorCerr(const statusMonitorCerr& stMonitor);

    /**
     * Destructor
     */
    virtual ~statusMonitorCerr();

    /**
     * Return the name of this class
     */
    virtual const std::string& name() const;

    /**
     * Return a static instance of the current object type, which is not
     * clonable since it has a singleton pattern.
     */
    virtual statusMonitorCerr* clone() const;

    /**
     * Return a static instance of the current object type
     */
    virtual statusMonitorCerr* newInstance() const;

    /**
     * Set a status string.
     *
     * @param id of the reporting class.  Usually you give here the output
     *        of name().
     * @param msg the const string to be reported.
     */
     virtual void set(const std::string& id,const char* msg);

    /**
     * Set a status string.
     *
     * @param id of the reporting class.  Usually you give here the output
     *        of name().
     * @param msg the const string to be reported.
     */
    virtual void set(const std::string& id,const std::string& msg) ;

    /**
     * Append a message to the current status string.
     *
     * @param id of the reporting class.  Usually you give here the output
     *        of name().
     * @param msg message to be appended to the current status string.
     */
    virtual void append(const std::string& id,const char* msg);

    /**
     * Append a message to the current status string.
     *
     * @param id of the reporting class.  Usually you give here the output
     *        of name().
     * @param msg message to be appended to the current status string.
     */
    virtual void append(const std::string& id,const std::string& msg);

  protected:
    /**
     * Output stream where the errors should be written as they come.
     */
    std::ostream* theStream_;

    /**
     * End of line delay
     */
  };

  
}
