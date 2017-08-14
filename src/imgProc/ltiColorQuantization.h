/*
 * Copyright (C) 1998-2006
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
 * \file   ltiColorQuantization.h
 *         Contains parent class for all color quantization algorithms.
 * \author Pablo Alvarado
 * \date   08.05.2001 (LTI-Lib 1)
 * \date   12.01.2006 (LTI-Lib 2)
 *
 * $Id: ltiColorQuantization.h,v 1.3 2009-10-19 00:26:07 alvarado Exp $
 */

#ifndef _LTI_COLOR_QUANTIZATION_H_
#define _LTI_COLOR_QUANTIZATION_H_

#include "ltiFunctor.h"
#include "ltiMatrix.h"
#include "ltiImage.h"

namespace lti {

  /**
   * Class colorQuantization
   *
   * Abstract parent class for all color quantization algorithms.
   *
   * All color quantization functors must overload the apply-methods defined
   * here (see colorQuantization::apply())
   *
   * The final quantized color image can be generated using the lti::usePalette
   * functor.
   *
   * @see colorQuantization::parameters
   *
   * @ingroup gColorQuantization
   */
  class colorQuantization : public functor {
  public:
    /**
     * The parameters for the class colorQuantization
     */
    class parameters : public functor::parameters {
    public:
      /**
       * Default constructor.
       */
      parameters();

      /**
       * Copy constructor.
       *
       * @param other the parameters object to be copied
       */
      parameters(const parameters& other);

      /**
       * Destructor
       */
      virtual ~parameters();

      /**
       * Returns the name of this type
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
       * Copy the other instance.here.
       */
      parameters& copy(const parameters& other);

      /**
       * Copy the contents of a parameters object.
       *
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& operator=(const parameters& other);

      /**
       * Write the parameters in the given ioHandler.
       *
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool write(ioHandler& handler,const bool complete=true) const;

      /**
       * Read the parameters from the given ioHandler.
       *
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
       * Number of colors that the resulting quantization must have.
       *
       * Usually this value must be between 2 and 256, but each functor
       * can modify this requirement.
       *
       * Default value: 256
       */
      int numberOfColors;

    };

    /**
     * Default constructor.
     */
    colorQuantization();

    /**
     * Copy constructor.
     *
     * @param other the object to be copied
     */
    colorQuantization(const colorQuantization& other);

    /**
     * Destructor.
     */
    virtual ~colorQuantization();

    /**
     * Returns the name of this type.
     */
    virtual const std::string& name() const;

    /**
     * Copy data of "other" functor.
     *
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    colorQuantization& copy(const colorQuantization& other);

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual colorQuantization* clone() const = 0;

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual colorQuantization* newInstance() const = 0;

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * Quantize the colors of src and leave the labels for the quantized colors
     * in dest, and in the palette entries corresponding to the labels the
     * mean colors for each label.
     *
     * @param src original image with the true-color data
     * @param dest channel8 where the indexes of the also calculated palette
     *             will be.
     * @param thePalette the color palette used by the channel.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const image& src,
                       matrix<ubyte>& dest,
                       palette& thePalette) const = 0;

    /**
     * Quantize the colors of src and leave the labels for the quantized colors
     * in dest, and in the palette entries corresponding to the labels the
     * mean colors for each label.
     *
     * @param src original image with the true-color data
     * @param dest channel8 where the indexes of the also calculated palette
     *             will be.
     * @param thePalette the color palette used by the channel.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const image& src,
                       matrix<int32>& dest,
                       palette& thePalette) const = 0;

    /**
     * Quantize the colors of the given image.
     *
     * @param srcdest image with the source data.  The result
     *                 will be left here too.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(image& srcdest) const;

    /**
     * Quantize the colors of the given src image and leave the result in dest.
     *
     * @param src image with the source data.
     * @param dest image with only the number of colors specified in
     *             the parameters
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const image& src,image& dest) const;
  };
}

#endif

