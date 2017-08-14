/*
 * Copyright (C) 2007
 * Pablo Alvarado
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
 * \file   ltiDenoising.h
 *         Contains the class denoising,
 *         which is the parent class of all denoising functors like
 *         lti::median, lti::susanDenoising, etc.
 * \author Pablo Alvarado
 * \date   14.09.2007
 *
 * revisions ..: $Id: ltiDenoising.h,v 1.3 2007-10-14 02:51:00 alvarado Exp $
 */

#ifndef _LTI_DENOISING_H_
#define _LTI_DENOISING_H_

#include "ltiMatrix.h"
#include "ltiBoundaryType.h"
#include "ltiChannelProcessingInterface.h"
#include "ltiFunctor.h"

namespace lti {

  /**
   * Class denoising
   *
   * This is the parent abstract class of all denoising classes like 
   * lti::median or lti::susanDenoising.
   *
   * @see denoising::parameters.
   *
   * @ingroup gDenoising
   */
  class denoising : public functor,
                    public channelProcessingInterface<float>,
                    public channelProcessingInterface<ubyte> {
  public:
    /**
     * The parameters for the class denoising
     */
    class parameters : public functor::parameters {
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
       * Returns the complete name of the parameters class.
       */
      virtual const std::string& name() const;

      /**
       * Returns a pointer to a clone of the parameters.
       */
      virtual parameters* clone() const;

      /**
       * Returns a pointer to a new instance of the parameters.
       */
      virtual parameters* newInstance() const;

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

      /**
       * Boundary type of the image to be considered while filtered.
       *
       * Default value: Zero
       */
      eBoundaryType boundaryType;

    };

    /**
     * Default constructor
     */
    denoising();

    /**
     * Construct a functor using the given parameters
     */
    denoising(const parameters& par);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    denoising(const denoising& other);

    /**
     * Destructor
     */
    virtual ~denoising();

    /**
     * Denoise the given argument and leave the result there too.
     *
     * @param srcdest matrix<ubyte> with the source data. The result
     *                will be left here too.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(channel8& srcdest) const;

    /**
     * Operates on the given %parameter.
     *
     * @param srcdest channel with the source data. The result
     *                will be left here too.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(channel& srcdest) const;

    /**
     * Operates on a copy of the given arguments.
     *
     * @param src channel8 with the source data.
     * @param dest channel8 where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const channel8& src, channel8& dest) const=0;

    /**
     * Operates on a copy of the given arguments.
     *
     * @param src channel with the source data.
     * @param dest channel where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const channel& src, channel& dest) const=0;


    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    denoising& copy(const denoising& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    denoising& operator=(const denoising& other);

    /**
     * Returns the complete name of the functor class
     */
    virtual const std::string& name() const;

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual denoising* clone() const = 0;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual denoising* newInstance() const = 0;

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;
  };
}

#endif

