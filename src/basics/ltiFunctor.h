/*
 * Copyright (C) 1998-2005
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
 * \file   ltiFunctor.h
 *         Contains the class lti::functor, which is the parent class for all
 *         functionality classes that work on images, matrices, etc.
 * \author Thomas Rusert
 * \date   14.04.1999
 *
 * $Id: ltiFunctor.h,v 1.14 2006-08-29 12:27:42 doerfler Exp $
 */

#ifndef _LTI_FUNCTOR_H_
#define _LTI_FUNCTOR_H_

#include "ltiParametersManager.h"
#include "ltiInvalidMethodException.h"
#include "ltiIoHandler.h"

namespace lti  {

  /**
   * Base class for all lti functors.
   *
   * Every lti fuctor must have at least the member <em>apply()</em>,
   * which "applies" the functor's functionality on the data given through the
   * arguments of the apply method.
   *
   * There are two kinds of apply methods:
   *  - the "on-copy"-apply returns the result in a new
   *    object, and the original data will not be modified.
   *  - the "on-place"-apply returns the result on the same input object and
   *    therefore the original data will be destroyed.
   *
   * The operation of the functor can be controled with some parameters,
   * which will can be set with the "setParameters" member-function.
   *
   * Each functor may have also other setABC() members, to allow the
   * change of just one parameter-item at the time.
   */
  class functor : public ioObject,
                  public status, 
                  public parametersManager {

  public:
    /**
     * Parameters class of functor is empty and abstract
     */
    class parameters : public parametersManager::parameters {
    public:
      /**
       * Default constructor
       */
      parameters();
      
      /**
       * Copy constructor
       */
      parameters(const parameters& other);
      
      /**
       * Destructor
       */
      virtual ~parameters();
      
      /**
       * Returns the name of this class
       */
      virtual const std::string& name() const = 0;

      /**
       * Returns a pointer to a clone of the parameters.
       */
      virtual parameters* clone() const = 0;

      /**
       * Returns a pointer to a clone of the parameters.
       */
      virtual parameters* newInstance() const = 0;
    };

    /**
     * Default constructor
     */
    functor();

    /**
     * Copy constructor
     */
    functor(const functor& other);

    /**
     * Destructor
     */
    virtual ~functor();

    /**
     * Copy data of "other" functor.
     * Please note that the status string will _NOT_ be copied!
     */
    functor& copy(const functor& other);

    /**
     * Returns the name of this class.
     */
    virtual const std::string& name() const = 0;

    /**
     * Clone member
     */
    virtual functor* clone() const = 0;

    /**
     * Create new instance
     */
    virtual functor* newInstance() const = 0;
    
    /**
     * Write the parametersManager in the given ioHandler. The default
     * implementation is to write just the parameters object.
     *
     * @param handler the ioHandler to be used
     * @param complete if true (the default) the enclosing begin/end will
     *        be also written, otherwise only the data block will be written.
     * @return true if write was successful
     */
    virtual bool write(ioHandler& handler,
                       const bool complete=true) const;

    /**
     * Read the parametersManager from the given ioHandler. 
     *
     * The default implementation is to read just the parameters object.
     *
     * Since this virtual method needs to know the exact type of the
     * parameters to call the proper read method, it will just assume
     * that the current parametersManager instance has a valid, consistent
     * parameter set.  If this is not the case, you need to
     * reimplement the read method to set first a dummy parameter
     * object or the correct type.
     *
     * @param handler the ioHandler to be used
     * @param complete if true (the default) the enclosing begin/end will
     *        be also written, otherwise only the data block will be written.
     * @return true if write was successful
     */
    virtual bool read(ioHandler& handler,const bool complete=true);

  };
} // namespace lti

#endif

