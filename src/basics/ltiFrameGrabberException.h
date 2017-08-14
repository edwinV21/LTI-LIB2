/*
 * Copyright (C) 1998 - 2004
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
 * \file   ltiFrameGrabberException.h
 *         Contains the class lti::frameGrabberException which is thrown by
 *         classes derived from lti::frameGrabber
 * \author Peter Mathes
 * \date   13.08.1999
 *
 * $Id: ltiFrameGrabberException.h,v 1.1 2006-08-29 12:27:42 doerfler Exp $
 */

#ifndef _LTI_FRAMEGRABBER_H_
#define _LTI_FRAMEGRABBER_H_

#include "ltiException.h"

namespace lti {

  /**
   * class for frame grabber exceptions
   */
  class frameGrabberException : public lti::exception {
  public:
    /**
     * default constructor
     */
    frameGrabberException(const char* msg = "frame grabber exception");

    /**
     * Returns the name of this type 
     */
    virtual const std::string& name() const;

    /**
     * Returns a pointer to a clone of the exception
     */
    virtual frameGrabberException* clone() const;

    /**
     * Returns a pointer to a new instance of the exception
     */
    virtual frameGrabberException* newInstance() const;

  };

}

#endif
