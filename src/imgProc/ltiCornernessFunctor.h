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
 * \file   ltiCornernessFunctor.h
 *         Contains the class lti::cornernessFunctor, which is the
 *         base for functors that compute any cornerness
 * \author Peter Doerfler
 * \date   13.1.2005
 *
 * revisions ..: $Id: ltiCornernessFunctor.h,v 1.4 2012-07-29 03:42:38 alvarado Exp $
 */

#ifndef _LTI_CORNERNESS_FUNCTOR_H_
#define _LTI_CORNERNESS_FUNCTOR_H_

#include "ltiImage.h"
#include "ltiChannel.h"
#include "ltiChannel8.h"
#include "ltiFunctor.h"

namespace lti {

  /**
   * Abstract class for computation of cornerness.
   *
   * Please use an inherited class from this one.
   *
   * The cornenerness value at a given pixel expresses how likely it
   * is that a corner is found at that position.
   *
   * For most cornerness functions found in the literature the
   * likelihood of a corner is high for high values of the cornerness,
   * for some it is vice versa and still others suggest corners for
   * minima and maxima of the cornerness.
   * This information is available from each cornernessFunctor via
   * getType() which returns:
   * - \a cornernessFunctor::Max for corners found at maxima
   * - \a cornernessFunctor::Min for corners found at minima
   * - \a cornernessFunctor::MinMax for corners found at minima and maxima
   *
   * CornernessFunctors are used in some cornerDetector classes such
   * as harrisCorners or in location search methods like
   * gradientLocationSearch.
   *
   * @ingroup gEdgeCorner
   */
  class cornernessFunctor : public functor {
  public:

    /**
     * The eCornernessType states at which values of the cornerness
     * the corners can be found: maxima, minima, or both.
     */
    enum eCornernessType {
      Max,   /**< Corners found at maxima of the cornerness*/
      Min,   /**< Corners found at minima of the cornerness*/
      MinMax /**< Corners found at minima and maxima of the cornerness*/
    };

    /**
     * The parameters for the class cornernessFunctor
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
    };

    /**
     * Default constructor
     */
    cornernessFunctor();

    /**
     * Construct a functor using the given parameters
     */
    cornernessFunctor(const parameters& par);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    cornernessFunctor(const cornernessFunctor& other);

    /**
     * Destructor
     */
    virtual ~cornernessFunctor();

    /**
     * Calculates the cornerness of srcdest and leaves it there.
     *
     * @param srcdest channel with the source data.  The result
     *                 will be left here too.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(channel& srcdest) const;

    /**
     * Calculates the cornerness of srcdest and leaves it there. The
     * minimal and maximal cornerness values are returned as well.
     *
     * @param srcdest channel with the source data.  The result
     *                will be left here too.
     * @param minCornerness minimum cornerness value
     * @param maxCornerness maximum cornerness value
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(channel& srcdest,
                       float& minCornerness, float& maxCornerness) const;
    
    /**
     * Calculates the cornerness of \a src and leaves it in \a dest.
     *
     * @param src channel with the source data.
     * @param dest channel where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const channel& src, channel& dest) const;

    /**
     * Calculates the cornerness of \a src and leaves it in \a
     * dest. The minimal and maximal cornerness values are returned as
     * well.
     *
     * @param src channel with the source data.
     * @param dest channel where the result will be left.
     * @param minCornerness minimum cornerness value
     * @param maxCornerness maximum cornerness value
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const channel& src, channel& dest,
                       float& minCornerness, float& maxCornerness) const;

    /**
     * Calculates the cornerness of \a src and leaves it in \a dest. By
     * default this is merely a shortcut for conversion of the
     * channel8 to a channel and back after cornerness calculation
     *
     * @param src channel8 with the source data.
     * @param dest channel where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const channel8& src, channel& dest) const;

    /**
     * Calculates the cornerness of \a src and leaves it in \a dest.
     * The minimal and maximal cornerness values are returned as
     * well. By default this is merely a shortcut for conversion of
     * the channel8 to a channel and back after cornerness calculation
     *
     * @param src channel8 with the source data.
     * @param dest channel where the result will be left.
     * @param minCornerness minimum cornerness value
     * @param maxCornerness maximum cornerness value
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const channel8& src, channel& dest,
                       float& minCornerness, float& maxCornerness) const;

    /**
     * Calculates the cornerness of \a src and leaves it in \a dest.
     *
     * Due to the different nature of grey and color images a
     * different but similar method will be used here. This can be
     * done by e.g. using a colorGradient instead of a normal
     * gradient.
     *
     * @param src channel with the source data.
     * @param dest channel where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const image& src, channel& dest) const;

    /**
     * Calculates the cornerness of \a src and leaves it in \a dest.
     *
     * Due to the different nature of grey and color images a
     * different but similar method will be used here. This can be
     * done by e.g. using a colorGradient instead of a normal
     * gradient.
     *
     * @param src channel with the source data.
     * @param dest channel where the result will be left.
     * @param minCornerness minimum cornerness value
     * @param maxCornerness maximum cornerness value
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const image& src, channel& dest,
                       float& minCornerness, float& maxCornerness) const;

    /**
     * Returns which values of the cornerness indicate a corners:
     * maxima, minima or both. See eCornernessType and class docu for
     * more explanation.
     * @return eCornernessType of this cornernessFunctor
     */
    virtual const eCornernessType& getType() const;

    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    cornernessFunctor& copy(const cornernessFunctor& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    cornernessFunctor& operator=(const cornernessFunctor& other);

    /**
     * Returns the complete name of the functor class
     */
    virtual const std::string& name() const;

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual cornernessFunctor* clone() const=0;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual cornernessFunctor* newInstance() const=0;

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;

  protected:
    /**
     * Stores the eCornernessType of the specific implementation. Every
     * derived class _must_ set this!
     */
    eCornernessType cornernessType_;


    /**
     * Does the actual calculation of the cornerness of \a src and leaves it
     * in \a dest.
     *
     * @param src channel with the source data.
     * @param dest channel where the result will be left.
     * @param minCornerness minimum cornerness value
     * @param maxCornerness maximum cornerness value
     * @return true if apply successful or false otherwise.
     */
    virtual bool worker(const channel& src, channel& dest,
                        float& minCornerness, float& maxCornerness) const=0;

    /**
     * Does the actual calculation of the cornerness of \a src and leaves it
     * in \a dest.
     *
     * Due to the different nature of grey and color images a
     * different but similar method will be used here. This can be
     * done by e.g. using a colorGradient instead of a normal
     * gradient.
     *
     * @param src channel with the source data.
     * @param dest channel where the result will be left.
     * @param minCornerness minimum cornerness value
     * @param maxCornerness maximum cornerness value
     * @return true if apply successful or false otherwise.
     */
    virtual bool worker(const image& src, channel& dest,
                        float& minCornerness, float& maxCornerness) const=0;
  };
}

#endif

