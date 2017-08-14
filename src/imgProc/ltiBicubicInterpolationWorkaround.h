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
 * \file   ltiBicubicInterpolationWorkaround.h
 *         Contains the template class bicubicInterpolationWorkaround, which is
 *         a workaround for lti::bicubicInterpolation. It uses 
 *         lti::genericInterpolation with a bicubic Kernel.
 * \author Arnd Hannemann
 * \date   13.5.2005
 *
 * revisions ..: $Id: ltiBicubicInterpolationWorkaround.h,v 1.1 2005-05-24 11:33:52 arndh Exp $
 */

#ifndef _LTI_BICUBIC_INTERPOLATION_WORKAROUND_H_
#define _LTI_BICUBIC_INTERPOLATION_WORKAROUND_H_

//TODO: include _only_ those files which are needed in this header!!

// TODO: Check this include to parent class: 
#include "ltiGenericInterpolation.h"

namespace lti {

  /**
   * This functor use bicubic interpolation to approximate values
   * between the pixels or elements of vectors and matrices.
   *
   * It is a workaround for the incomplete implementation of
   * bicbuicInterpolation and uses the genericInterpolation
   * with a bicubic kernel lut.
   *
   * @see bicubicInterpolationWorkaround::parameters.
   *
   * @ingroup gInterpolator
   */
  template<typename T>
  class bicubicInterpolationWorkaround : public genericInterpolation<T> {
  public:
    /**
     * The parameters for the class bicubicInterpolationWorkaround
     */
    class parameters : public genericInterpolation<T>::parameters {
    public:
      /**
       * Default constructor
       */
      parameters();

      /**
       * Copy constructor
       * @param other the parameters object to be copied
       */
      parameters(const parameters& other);

      /**
       * Destructor
       */
      ~parameters();

      /**
       * Copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& copy(const parameters& other);

      /**
       * Copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& operator=(const parameters& other);

      /**
       * Returns a pointer to a clone of the parameters
       */
      virtual parameters* clone() const;

      /**
       * Returns a pointer to a new instance of the parameters
       */
      virtual parameters* newInstance() const;

      /**
       * Returns the name of this parameters class,
       * which depends on the template parameters.
       */
      virtual const std::string& name() const;

      /**
       * Write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool write(ioHandler& handler,const bool complete=true) const;

      /**
       * Read the parameters from the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool read(ioHandler& handler,const bool complete=true);

      // ------------------------------------------------
      // the parameters
      // ------------------------------------------------

    };

    /**
     * Default constructor
     */
    bicubicInterpolationWorkaround();

    /**
     * Construct a functor using the given parameters
     */
    bicubicInterpolationWorkaround(const parameters& par);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    bicubicInterpolationWorkaround(const bicubicInterpolationWorkaround<T>& other);

    /**
     * Destructor
     */
    virtual ~bicubicInterpolationWorkaround();

    //TODO: comment your apply methods!
    

    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    bicubicInterpolationWorkaround<T>& copy(const bicubicInterpolationWorkaround<T>& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    bicubicInterpolationWorkaround<T>& operator=(const bicubicInterpolationWorkaround<T>& other);

    /**
     * Returns the name of this type, which depends on the template parameters.
     */
    virtual const std::string& name() const;

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual bicubicInterpolationWorkaround<T>* clone() const;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual bicubicInterpolationWorkaround<T>* newInstance() const;

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;



  };
}
#include "ltiBicubicInterpolationWorkaround_template.h"
#endif

