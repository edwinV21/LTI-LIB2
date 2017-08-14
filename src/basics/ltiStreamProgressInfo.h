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
 * \file   ltiStreamProgressInfo.h
 *         Contains the class streamProgressInfo, which prints
 *         progress information to a string
 * \author Pablo Alvarado
 * \author Peter Doerfler
 * \date   10.08.2000
 *
 * $Id: ltiStreamProgressInfo.h,v 1.3 2006-09-14 15:14:02 doerfler Exp $
 */

#ifndef _LTI_STREAM_PROGRESS_INFO_H_
#define _LTI_STREAM_PROGRESS_INFO_H_

#include <string>
#include <iostream>

#include "ltiProgressInfo.h"

namespace lti {

  /**
   * This class is a default implementation for the progress info
   * interface. It dumps the progress information to a std::ostream,
   * or std::cout if you do not give one.
   */
  class streamProgressInfo : public progressInfo {
  public:
    /**
     * Default constructor
     * @param title the name of the progressInfo object
     * @param maximumSteps the maximum number of steps of the process
     */
    streamProgressInfo(const std::string& title = "",
                       const int maximumSteps = 100);

    /**
     * Default constructor
     * @param outStream output stream, where the progress information will
     *                  be written.
     * @param title the name of the progressInfo object
     * @param maximumSteps the maximum number of steps of the process
     */
    streamProgressInfo(std::ostream& outStream,
                       const std::string& title = "",
                       const int maximumSteps = 100);


    /**
     * Copy constructor
     */
    streamProgressInfo(const streamProgressInfo& other);

    /**
     * Destructor
     */
    virtual ~streamProgressInfo();

    /**
     * Set a new stream. The streamProgressInfo keeps a pointer to the
     * stream.
     */
    virtual void useStream(std::ostream& stream);

    /**
     * Set the endline string used. The default is "\n".
     */
    void setEndline(const std::string& endline);

    /**
     * Report one step done
     * @param progressInfo string with some text information for the step
     */
    virtual void step(const std::string& progressInfo);

    /**
     * Report additional information for a step, with the given detail
     * level.
     *
     * The given information will be displayed only if the current detail level
     * is higher or equal than the detail specified in this method.
     */
    virtual void substep(const int detail,
                         const std::string& info);

    /**
     * The copy member
     */
    streamProgressInfo& copy(const streamProgressInfo& other);

    /**
     * alias for copy
     */
    streamProgressInfo& operator=(const streamProgressInfo& other);

    /**
     * Returns the name of the class
     */
    const std::string& name() const;

    /**
     * The clone member
     */
    virtual streamProgressInfo* clone() const;

    /**
     * The newInstance member
     */
    virtual streamProgressInfo* newInstance() const;

  protected:
    /**
     * Stream being used
     */
    std::ostream *out_;

    /**
     * The sequence that is used for end-of-line when emitting
     * steps.
     */
    std::string endline_;
  };


}

#endif
