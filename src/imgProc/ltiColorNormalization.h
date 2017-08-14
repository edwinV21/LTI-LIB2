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
 * \file   ltiColorNormalization.h
 *         Contains the abstract base class colorNormalization.
 * \author Pablo Alvarado
 * \date   30.03.2003
 *
 * $Id: ltiColorNormalization.h,v 1.1 2005-06-23 12:39:14 doerfler Exp $
 */

#ifndef _LTI_COLOR_NORMALIZATION_H_
#define _LTI_COLOR_NORMALIZATION_H_

#include "ltiImage.h"
#include "ltiFunctor.h"

namespace lti {
  /**
   * Base class for color normalization methods usually used to eliminate
   * the effects of illumination changes.
   *
   * \ingroup gColor
   */
  class colorNormalization : public functor {
  public:
    /**
     * the parameters for the class colorNormalization
     */
    class parameters : public functor::parameters {
    public:
      /**
       * default constructor
       */
      parameters();

      /**
       * copy constructor
       * @param other the parameters object to be copied
       */
      parameters(const parameters& other);

      /**
       * destructor
       */
      ~parameters();

      /**
       * copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& copy(const parameters& other);

      /**
       * Returns the name of this functor.
       */
      virtual const std::string& name() const;

      /**
       * returns a pointer to a clone of the parameters
       */
      virtual parameters* clone() const;

      /**
       * returns a pointer to a new instance of the parameters
       */
      virtual parameters* newInstance() const;

      /**
       * write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool write(ioHandler& handler,const bool complete=true) const;

      /**
       * write the parameters in the given ioHandler
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
     * default constructor
     */
    colorNormalization();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    colorNormalization(const colorNormalization& other);

    /**
     * destructor
     */
    virtual ~colorNormalization();

    /**
     * Normalize the colors on the given image and write the result there too.
     *
     * @param srcdest image with the source data. The result will be left 
     *                 here too.
     * @return true if successful, false otherwise.
     */
    virtual bool apply(image& srcdest) const=0;

    /**
     * Normalize the colors on the image \a src and leave the result
     * in \a dest.
     *
     * @param src image with the source data.
     * @param dest image where the result will be left.
     * @return true if successful, false otherwise.
     */
    virtual bool apply(const image& src,image& dest) const=0;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    colorNormalization& copy(const colorNormalization& other);

    /**
     * Returns the name of this functor.
     */
    virtual const std::string& name() const;

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual colorNormalization* clone() const = 0;

    /**
     * returns a pointer to a new instance of this functor.
     */
    virtual colorNormalization* newInstance() const = 0;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

  };
}

#endif

