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
 * \file   ltiProgressInfo.h
 *         Contains the class progressInfo, base class for progress information
 * \author Pablo Alvarado
 * \author Peter Doerfler
 * \date   10.08.2000
 *
 * $Id: ltiProgressInfo.h,v 1.4 2008-10-15 17:16:39 alvarado Exp $
 */

#ifndef _LTI_PROGRESS_INFO_H_
#define _LTI_PROGRESS_INFO_H_

#include <string>

#include "ltiObject.h"

namespace lti {

  /**
   * This class can be overloaded to display the training progress of
   * a process. It is mostly used for classifiers, where training can
   * take a long time.
   */
  class progressInfo : public object {
  public:
    /**
     * Default constructor.
     * 
     * The default are empty title and 100 maximum steps. These are
     * rarely the correct settings. For informative progress
     * information set these values either with the constructor or the
     * setMaximumSteps() and setTitle() functions.
     *
     * @param title the name of the progressInfo object
     * @param maximumSteps the maximum number of steps of the process
     */
    progressInfo(const std::string& title = "",
                 const int maximumSteps = 100);

    /**
     * Copy constructor
     */
    progressInfo(const progressInfo& other);

    /**
     * Destructor
     */
    virtual ~progressInfo();

    /**
     * Set the title of the progress info block
     */
    virtual void setTitle(const std::string& theTitle);

    /**
     * Maximal number of steps
     */
    virtual void setMaxSteps(const int maximalSteps);

    /**
     * Get maximal number of steps
     */
    int getMaxSteps() const;

    /**
     * Get current step
     */
    int getStep() const;

    /**
     * Set current step.  If the given value is greater than the
     * currently maximal step, it will be 'cropped' to that value.
     */
    void setStep(const int theStep);

    /**
     * Get progress
     * 
     * The returned value is equivalent to dividing the current
     * step by the maximal number of steps.
     */
    double calculateProgress() const;

    /**
     * Detail level of substep information. Level 1 is equal to that
     * of normal steps.
     */
    virtual void setDetailLevel(const int level);

    /**
     * Return the used detail level of substep information
     */
    virtual int getDetailLevel() const;

    /**
     * Report one step done
     * @param progressInfo string with some text information for the step
     */
    virtual void step(const std::string& progressInfo)=0;

    /**
     * Report additional information for a step, with the given detail
     * level.
     *
     * The given information will be displayed only if the current detail level
     * is higher or equal than the detail specified in this method.
     *
     * @param detail is the detail level. The lowest (as in step()) is 1.
     * @param info the string displayed for the substep
     */
    virtual void substep(const int detail,
                         const std::string& info)=0;

    /**
     * Reset progress information
     */
    virtual void reset();

    /**
     * Returns true if someone wants the caller of this progress info
     * object to terminate. This function is mostly useful for GUI
     * progressInfo implementations.
     *
     * This default implementation always returns false.
     */
    virtual bool breakRequested() const;

    /**
     * The copy member
     */
    progressInfo& copy(const progressInfo& other);

    /**
     * Returns the name of the class.
     */
    const std::string& name() const;

    /**
     * The clone member
     */
    virtual progressInfo* clone() const=0;

    /**
     * The new instance member
     */
    virtual progressInfo* newInstance() const=0;

  protected:
    /**
     * Title for this progress info
     */
    std::string title_;

    /**
     * Maximum number of steps expected
     */
    int maxSteps_;

    /**
     * Detail level used for the substeps
     */
    int detailLevel_;

    /**
     * Last processed step
     */
    int lastStep_;

  };


}

#endif

