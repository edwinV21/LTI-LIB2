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
 * \file   ltiModifier.h
 *         Contains the base class modifier.
 * \author Pablo Alvarado
 * \date   19.04.1999
 *
 * $Id: ltiModifier.h,v 1.11 2009-04-19 04:06:10 alvarado Exp $
 */

#ifndef _LTI_MODIFIER_H_
#define _LTI_MODIFIER_H_

#include "ltiImage.h"
#include "ltiFunctor.h"
#include "ltiBoundaryType.h"

namespace lti {
  /**
   * Abstract base class for all filters and other functors, which
   * transform an image or channel in another image of the same type.
   *
   * For those functors which generate more than one image or
   * transforms the image in another totally different space (like a
   * vector), are derived from lti::transform (see for example the FFT
   * transformation!)
   */
  class modifier : public functor {
  public:
    /**
     * parameter class for modifier
     */
    class parameters : public functor::parameters {
    public:

      /**
       * default constructor
       */
      parameters();

      /**
       * copy constructor
       */
      parameters(const parameters& other);

      /**
       * copy member
       */
      parameters& copy(const parameters& other);

      /**
       * returns the name of this type
       */
      virtual const std::string& name() const;

      /**
       * Returns a pointer to a clone of the parameters
       */
      virtual parameters* clone() const;

      /**
       * Returns a pointer to a new instance of the parameters
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

      /**
       * how the boundaries will be used
       *
       * Default: eBoundaryType::Zero
       */
      eBoundaryType boundaryType;


    };

    /**
     * default constructor
     */
    modifier() : functor() {};

    /**
     * destructor
     */
    virtual ~modifier();

    /**
     * returns current parameters.
     */
    const parameters& getParameters() const;

    /**
     * operates on the given parameter (vector of floats).
     *
     * If this method is not implemented, an exception of type
     * lti::functor::invalidMethodException will be thrown.
     *
     * @param srcdest vector of floats with the source data.  The result
     *                will be left here too.
     * @return true if successful, false otherwise.
     */
//     virtual bool apply(fvector& srcdest) const;

    /**
     * operates on the given parameter (vector of integers).
     * If this method is not implemented, an exception of type
     * lti::functor::invalidMethodException will be thrown.
     * @param srcdest vector of integers with the source data.  The result
     *                will be left here too.
     * @return true if successful, false otherwise.
     */
//     virtual bool apply(ivector& srcdest) const;

    /**
     * operates on the given parameter (vector of unsigned bytes).
     * If this method is not implemented, an exception of type
     * lti::functor::invalidMethodException will be thrown.
     * @param srcdest vector of unsigned bytes with the source data.  The
     *                result will be left here too.
     * @return true if successful, false otherwise.
     */
//     virtual bool apply(vector<ubyte>& srcdest) const;

    /**
     * operates on the given parameter (channel, i.e. matrix of floats).
     * If this method is not implemented, an exception of type
     * lti::functor::invalidMethodException will be thrown.
     * @param srcdest channel with the source data.  The result
     *                will be left here too.
     * @return true if successful, false otherwise.
     */
//     virtual bool apply(channel& srcdest) const;

    /**
     * operates on the given parameter (matrix of integers).
     * If this method is not implemented an exception of type
     * lti::functor::invalidMethodException will be thrown.
     * @param srcdest matrix of integers with the source data.  The result
     *                will be left here too.
     * @return true if successful, false otherwise.
     */
//     virtual bool apply(imatrix& srcdest) const;

    /**
     * operates on the given parameter (channel of unsigned bytes).
     * If this method is not implemented an exception of type
     * lti::functor::invalidMethodException will be thrown.
     * @param srcdest channel8  with the source data.
     *                The result will be left here too.
     * @return true if successful, false otherwise.
     */
//     virtual bool apply(channel8& srcdest) const;

    /**
     * operates on the given image
     * The image will be splitted in its RGB components, which will be
     * operated with the fastest method, and the results will be merged again.
     * @param srcdest lti::image with the source data.  The result will be left
     *                here.
     * @return true if successful, false otherwise.
     */
    virtual bool apply(image& srcdest) const;

    /**
     * operates on a copy of the given parameters (vector of floats)
     * @param src vector of floats with the source data.
     * @param dest vector of floats, where the result will be left.
     * @return true if successful, false otherwise.
     */
//     virtual bool apply(const fvector& src,fvector& dest) const;

    /**
     * operates on a copy of the given parameters (vector of integers)
     * @param src vector of integers with the source data.
     * @param dest vector of integers, where the result will be left.
     * @return true if successful, false otherwise.
     */
//     virtual bool apply(const ivector& src,ivector& dest) const;

    /**
     * operates on a copy of the given parameters (vector of unsigned bytes)
     * @param src vector of unsigned bytes with the source data.
     * @param dest vector of unsigned bytes, where the result will be left.
     * @return true if successful, false otherwise.
     */
//     virtual bool apply(const vector<ubyte>& src,
//                              vector<ubyte>& dest) const;

    /**
     * operates on a copy of the given parameters (channel)
     * @param src lti::channel with the source data.
     * @param dest channel, where the result will be left.
     * @return true if successful, false otherwise.
     */
//     virtual bool apply(const channel& src,channel& dest) const;

    /**
     * operates on a copy of the given parameters (matrix of integers)
     * @param src matrix of integers with the source data.
     * @param dest matrix of integers, where the result will be left.
     * @return true if successful, false otherwise.
     */
//     virtual bool apply(const imatrix& src,imatrix& dest) const;

    /**
     * operates on a copy of the given parameters (channel of unsigned bytes)
     * @param src lti::channel8 with the source data.
     * @param dest lti::channel8, where the result will be left.
     * @return true if successful, false otherwise.
     */
//     virtual bool apply(const channel8& src,channel8& dest) const;

    /**
     * operates on a copy of the given parameters (image)
     * The image will be splitted in its RGB components, which will be
     * operated with the fastest method, and the results will be merged.
     * @param src lti::image with the source data.
     * @param dest image, where the result will be left.
     * @return true if successful, false otherwise.
     */
    virtual bool apply(const image& src,image& dest) const;

    /**
     * returns the name of this type
     */
    virtual const std::string& name() const;


    /**
     * Shortcut for setting the boundary type of a functor derived from
     * modifier.
     *
     * \b Note: The boundaryType is modified via direct access to the
     * internal parameters object.  
     */
    bool setBoundaryType(const eBoundaryType boundaryType);
    

  protected:
    /**
     * non const getParameters()
     */
    parameters& getRWParameters();
  };
}

#endif

