/*
 * Copyright (C) 1998-2004
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
 * \file   ltiBilinearDemosaicing.h
 *         Contains the class lti::bilinearDemosaicing, which does
 *         simple bilinear interpolation of real colors for a Bayer
 *         pattern.
 * \author Peter Doerfler
 * \date   12.12.2004
 *
 * revisions ..: $Id: ltiBilinearDemosaicing.h,v 1.6 2008-10-20 02:05:01 alvarado Exp $
 */

#ifndef _LTI_BILINEAR_DEMOSAICING_H_
#define _LTI_BILINEAR_DEMOSAICING_H_

#include "ltiMatrix.h"
#include "ltiImage.h"
#include "ltiChannel.h"
#include "ltiChannel8.h"
#include "ltiBayerDemosaicing.h"

namespace lti {

  /**
   * This class performs simple bilinear interpolation to reconstruct
   * the original colors of each pixel of a Bayer pattern. Regard the
   * following Bayer pattern, with green(G) and A and C being red and
   * blue or vice versa:
   *
   * \code
   * G01 C02 G03 C04 G05
   * A06 G07 A08 G09 A10
   * G11 C12 G13 C14 G15 
   * \endcode
   *
   * Then:
   * - G at A or C: bilinear interpolation of 4 neighbors, e.g. 
   *   - G08 = 0.25(G03+G07+G09+G13)
   * - A or C at G: interpolation in horizontal OR vertical direction, e.g.
   *   - A07 = 0.5(A06+A08)
   *   - C07 = 0.5(C02+C12)
   * - A at C and vice versa: interpolat 4 pixels diagonally, e.g. 
   *   - C08 = 0.25(C02+C04+C12+C14)
   *
   * Interpolation at the borders proceeds similarily.
   *
   * This Bayer demosaicing is quite fast, on the average 2
   * multiplications and 4 additions are needed per pixel. However, it
   * shows quite a few artifacts such a zipper on straight edges,
   * moiré, etc.
   *
   * If you need quality instead of speed, then use
   * lti::optimalGradientDemosaicing.  If you need even more speed, take a look
   * at lti::nearestNeighborDemosaicing.
   *
   * @ingroup gAcquisition
   */
  class bilinearDemosaicing : public bayerDemosaicing {
  public:
    /**
     * The parameters for the class bilinearDemosaicing
     */
    class parameters : public bayerDemosaicing::parameters {
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
       * Returns the name of this type.
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
    bilinearDemosaicing();

    /**
     * Construct a functor using the given parameters
     */
    bilinearDemosaicing(const parameters& par);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    bilinearDemosaicing(const bilinearDemosaicing& other);

    /**
     * Destructor
     */
    virtual ~bilinearDemosaicing();

    /**
     * Transforms a raw Bayer pattern image into an lti::image using
     * bilinear interpolation as described in the class documentation.
     *
     * @param src contains a raw image of one of the parameters::eBayerPattern
     * @param dest the demosaiced image
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const matrix<ubyte>& src, image& dest) const;

    // make other apply member functions available
    using bayerDemosaicing::apply;

    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    bilinearDemosaicing& copy(const bilinearDemosaicing& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    bilinearDemosaicing& operator=(const bilinearDemosaicing& other);

    /**
     * Returns the name of this type.
     */
    virtual const std::string& name() const;

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual bilinearDemosaicing* clone() const;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual bilinearDemosaicing* newInstance() const;

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;

  protected:
    /**
     * Transform the pattern
     */
    bool pattern(const matrix<ubyte>& src,image& dest) const;
  };
}

#endif

