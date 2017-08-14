/*
 * Copyright (C) 1998 - 2005
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
 * \file   ltiProgressReporter.h
 *         Contains the class lti::progressReporter. All classes that
 *         want to report progress via a progressInfo object should
 *         derive from this class.
 * \author Peter Doerfler
 * \date   30.03.2005
 *
 * $Id: ltiProgressReporter.h,v 1.5 2008-03-18 21:00:10 alvarado Exp $
 */

#ifndef _LTI_PROGRESS_REPORTER_H_
#define _LTI_PROGRESS_REPORTER_H_

#include "ltiProgressInfo.h"

namespace lti  {

  /**
   * Interface for all objects that provide progressInfo interface.
   *
   * \ingroup gInterfaces
   */
  class progressReporter {
  public:

    /**
     * Default destructor
     */
    progressReporter();

    /**
     * Copy destructor
     */
    progressReporter(const progressReporter& other);

    /**
     * Destructor
     */
    virtual ~progressReporter();

    /**
     * Copy the \a other progressReporter
     */
    progressReporter& copy(const progressReporter& other);

    /**
     * Copy the \a other progressReporter
     */
    progressReporter& operator=(const progressReporter& other);

    /**
     * Return a pointer to a clone of this progressReporter
     */
    progressReporter* clone() const;

    /**
     * Return a pointer to a new instance of this progressReporter
     */
    progressReporter* newInstance() const;

    /**
     * Set the progressInfo %object to be used.
     *
     * A clone of the given %object will be generated.
     */
    void setProgressObject(const progressInfo& progress);

    /**
     * Remove the active progressInfo %object.
     */
    void removeProgressObject();

    /**
     * Return true if a valid progressInfo %object has already been set.
     */
    bool haveValidProgressObject() const;

    /**
     * Return true if a valid progressInfo %object has already been set, and if
     * its detail level is greater or equal the given value.
     */
    bool haveValidProgressObject(const int detailLevel) const;

    /**
     * Get a read-only reference to the progressInfo %object
     */
    const progressInfo& getProgressObject() const;
   
    /**
     * Get a modifiable reference to the progressInfo %object
     */
    progressInfo& getProgressObject();

  protected:
    
    /**
     * The current progressInfo %object.
     */
    progressInfo* progressObject_;
  };


} //namespace lti

#endif
