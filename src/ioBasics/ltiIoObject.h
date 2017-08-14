/*
 * Copyright (C) 1998, 1999, 2000, 2001
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
 * \file   ltiIoObject.h
 *         Containes base class for all serializable classes
 * \author Benjamin Winkler
 * \date   14.12.2001
 *
 * $Id: ltiIoObject.h,v 1.5 2006-01-07 22:19:09 alvarado Exp $
 */

#ifndef _LTI_IO_OBJECT_H_
#define _LTI_IO_OBJECT_H_

#include "ltiObject.h"
#include "ltiIoHandler.h"

namespace lti {

  /**
   * Basic input/output class for data objects.
   *
   * This is an abstract class.
   *
   * @ingroup gStorable
   */
  class ioObject : public object {
  public:

    /**
     * Default constructor
     */
    ioObject();

    /**
     * Destructor
     */
    virtual ~ioObject();

    /**
     * Returns the name of this class
     */
    virtual const std::string& name() const = 0;

    /**
     * Returns a copy of this object
     */
    virtual ioObject* clone() const = 0;

    /**
     * Returns a copy of this object
     */
    virtual ioObject* newInstance() const = 0;

    /**
     * Write the parameters in the given ioHandler
     *
     * @param handler the ioHandler to be used
     * @param complete if true (the default) the enclosing begin/end will
     *        be also written, otherwise only the data block will be written.
     * @return true if write was successful
     *
     * This method has to be overloaded.  If not it always returns false.
     */
    virtual bool write(ioHandler& handler, const bool complete=true) const;

    /**
     * Read the parameters from the given ioHandler
     *
     * @param handler the ioHandler to be used
     * @param complete if true (the default) the enclosing begin/end will
     *        be also written, otherwise only the data block will be written.
     * @return true if write was successful
     *
     * This method has to be overloaded.  If not it always returns false.
     */
    virtual bool read(ioHandler& handler,const bool complete=true);
  };

  /**
   * write
   *
   * @ingroup gStorable
   */
  bool write(ioHandler& handler, const ioObject& p, const bool complete=true);

  /**
   * read
   *
   * @ingroup gStorable
   */
  bool read(ioHandler& handler, ioObject& p, const bool complete=true);
}


#endif

