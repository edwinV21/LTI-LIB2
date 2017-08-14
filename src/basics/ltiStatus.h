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
 * \file   ltiStatus.h
 *         Contains the class lti::status, which is the parent class for all
 *         classes that can provide status information.
 * \author Jochen Wickel
 * \author Thomas Rusert
 * \author Pablo Alvarado
 * \date   14.04.2004
 *
 * $Id: ltiStatus.h,v 1.7 2007-10-07 03:17:00 alvarado Exp $
 */

#ifndef _LTI_STATUS_H_
#define _LTI_STATUS_H_

#include <string>


namespace lti  {
  class statusMonitor;

  /**
   * Base class for all lti objects that have a status for error
   * handling.
   *
   * This is sort of an interface, which does not provide the
   * standard lti::object interface, since it is assumed that the
   * objects that inherit from this class will also directly or
   * indirectly inherit from lti::object.
   *
   * Note that you can set a class inherited from lti::statusMonitor
   * to select how error have to be reported.  See the method
   * setStatusMonitor for more information.
   *
   * \ingroup gInterfaces
   */
  class status {
  public:
    /**
     * Default constructor
     */
    status();

    /**
     * Destructor
     */
    virtual ~status();

    /**
     * @name Error handling and report
     */
    //@{
    /**
     * Return the last message set with setStatusString().  This will
     * never return 0.  If no status-string has been set yet an empty string
     * (pointer to a string with only the char(0)) will be returned.
     */
    virtual const std::string& getStatusString() const;

    /**
     * Set a status string.
     *
     * @param msg the const string to be reported next time by
     * getStatusString(). The given string will be copied.
     * This message will be usually set within the apply methods to indicate
     * an error cause.
     *
     * Note that the change of the status string is not considered as
     * a change in the functor status.
     */
    virtual void setStatusString(const char* msg) const;

    /**
     * Set a status string.
     *
     * @param msg the const string to be reported next time by
     * getStatusString(). The given string will be copied.
     * This message will be usually set within the apply methods to indicate
     * an error cause.
     *
     * Note that the change of the status string is not considered as
     * a change in the functor status.
     */
    virtual void setStatusString(const std::string& msg) const;

    /**
     * Append a message to the current status string. Take care to
     * reset the status string by calling setStatusString() for each
     * call of an apply() or similar method. appendStatusString()
     * should only be used after setStatusString() has been called.
     *
     * @param msg message to be appended to the current status string.
     *
     * Note that the change of the status string is not considered as
     * a change in the functor status.
     */
    virtual void appendStatusString(const char* msg) const;

    /**
     * Append a message to the current status string. Take care to
     * reset the status string by calling setStatusString() for each
     * call of an apply() or similar method. appendStatusString()
     * should only be used after setStatusString() has been called.
     *
     * @param msg message to be appended to the current status string.
     *
     * Note that the change of the status string is not considered as
     * a change in the functor status.
     */
    virtual void appendStatusString(const std::string& msg) const;

    /**
     * Append an integer value to the current status string. Take care
     * to reset the status string by calling setStatusString() for
     * each call of an apply() or similar method. appendStatusString()
     * should only be used after setStatusString() has been called.
     *
     * @param msg integer value to be appended to the current status
     * string.
     *
     * Note that the change of the status string is not considered as
     * a change in the functor status.
     */
    virtual void appendStatusString(const int msg) const;

    /**
     * Append a double value to the current status string. Take care
     * to reset the status string by calling setStatusString() for
     * each call of an apply() or similar method. appendStatusString()
     * should only be used after setStatusString() has been called.
     *
     * @param msg double value to be appended to the current status
     * string.
     *
     * Note that the change of the status string is not considered as
     * a change in the functor status.
     */
    virtual void appendStatusString(const double&  msg) const;

    /**
     * Append the status string of another %functor to this functors
     * status string. To achieve better readability of the resulting
     * message a new line is started with the other functor's name and
     * the message.
     *
     * @param other %functor whose status string is to be append to this
     *        status string.
     *
     * Note that the change of the status string is not considered as
     * a change in the functor status.
     */
    virtual void appendStatusString(const status& other) const;
    
    /**
     * Set Status Monitor.
     *
     * All functors in the LTI-Lib share this object, which controls what to
     * do when the status string is set or changed.  You can set it to throw
     * an exception after a few milliseconds (such that many
     * appendStatusString() calls can be done before the exception is thrown;
     * see lti::statusMonitorException), you can redirect the statusString
     * directly to the std::cerr; see lti::statusMonitorCerr), or you can do
     * nothing but store internally the statusString (see
     * lti::statusMonitorInactive), which is the default behaviour.
     *
     * If you implement an own class for this (like and error-log window)
     * ensure that you follow the interface explained in lti::statusMonitor
     * to avoid some memory leaks.
     *
     * You have to ensure that this method is called only once at a
     * time in your application, especially if you use several
     * threads.
     *
     */
    static void setStatusMonitor(statusMonitor& monitor);
    //@}

  protected:
    /**
     * Generate the class name.
     *
     * For complex status monitors, they expect to receive as parameter
     * the name of the caller class.  Since this class serves there only
     * as interface, here we don't have access to the name of the caller class
     * so that we have to recreate that name again using this wrapper class.
     */
    const std::string getClassName() const;

  private:
    /**
     * The status string written with setStatusString
     */
    mutable std::string statusString_;

    /**
     * Class that controls what to do with the status string.  You can set it
     * to throw an exception after a few milliseconds (such that many
     * appendStatusStrings can be called before the exception is thrown; see
     * lti::statusMonitorException), you can redirect the statusString
     * directly to the std::cerr; see lti::statusMonitorCerr), or you can do
     * nothing but store internally the statusString (see
     * lti::statusMonitorInactive), which is the default behaviour.
     */
    static statusMonitor* statusMonitor_;
  };

  /**
   * Global function to set the status monitor in all LTI-Lib objects that
   * manage a status string, like lti::functor, lti::ioHandler,
   * lti::classifier, etc.
   */
  void setStatusMonitor(statusMonitor& monitor);


} // namespace lti

#endif

