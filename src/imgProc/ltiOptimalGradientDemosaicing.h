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
 * \file   ltiOptimalGradientDemosaicing.h
 *         Contains the class lti::optimalGradientDemosaicing, a Bayer
 *         demosaicing that uses linear interpolation and gradients
 *         with optimal weights.
 * \author Peter Doerfler
 * \date   14.12.2004
 *
 * $Id: ltiOptimalGradientDemosaicing.h,v 1.4 2009-11-12 03:53:47 alvarado Exp $
 */

#ifndef _LTI_OPTIMAL_GRADIENT_DEMOSAICING_H_
#define _LTI_OPTIMAL_GRADIENT_DEMOSAICING_H_


#include "ltiMatrix.h"
#include "ltiImage.h"
#include "ltiChannel.h"
#include "ltiChannel8.h"
#include "ltiBayerDemosaicing.h"

namespace lti {

  /**
   * The class optimalGradientDemosaicing performs demosaicing of a
   * Bayer pattern filtered image using linear interpolation of and
   * second order gradients with optimal coefficients.
   *
   * For each reconstructed color in a pixel position the mean value
   * of the nearest neighbors of that color and a second order
   * gradient or Laplacian of the known color of that position in a
   * 5x5 neighborhood are optimally combined. The coefficients are
   * optimal in a Wiener sense.
   *
   * Consider the following Bayer pattern , with green(G) and A and C
   * being red and blue or vice versa:
   *
   * \code
   * G01 C02 G03 C04 G05 C06 G07
   * A08 G09 A10 G11 A12 G13 A14
   * G15 C16 G17 C18 G19 C20 G21
   * A22 G23 A24 G25 A26 G27 A28
   * G29 C30 G31 C32 G33 C34 G35
   * A36 G37 A38 G39 A40 G41 A42
   * G43 C44 G45 C46 G47 C48 G49
   * \endcode
   *
   * The missing colors are interpolated as follows:
   * - Green channel: interpolate 4 neighbors and add Laplacian of A/C, e.g.:
   *   - G24=0.25(G17+G23+G25+G31)+0.125(4*A24-(A10+A22+A26+A38))
   * - A at C and vice versa: similar to Green but different weights, e.g.:
   *   - C24=0.25(C16+C18+C30+C32)+1/12*(4*A24-(A10+A22+A26+A38))
   * - A at Green depends whether in an A row or a C row. Since only
   *   two neighbors of A color are either in the same column or row
   *   the filter is rotated. It uses a second and a first order
   *   gradient part (combined for implementation):
   *   - for rows: \f$A25=0.5(A24+A26)+0.125(4*G25-(G17+G19+G31+G33))
   *                   +0.125(G25-G23-G27+0.5(G11+G39))\f$
   *   - for cols: \f$C25=0.5(C18+C32)+0.125(4*G25-(G17+G19+G31+G33))
   *                   +0.125(G25-G11-G39+0.5(G23+G27))\f$
   *
   * This class is implemented after:
   * Henrique S. Malvar, Li-wei He, and Ross Cutler, "High-Quality
   * Linera Interpolation for Demosaicing of Bayer-Patterned Color
   * Images", Proc. of International Conference on Acoustics, Speech,
   * and Signal Processing (ICASSP 2004), 2004
   *
   * @ingroup gAcquisition
   */
  class optimalGradientDemosaicing : public bayerDemosaicing {
  public:
    /**
     * The parameters for the class optimalGradientDemosaicing
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
       * Returns name of this class
       */
      const std::string& name() const;

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
    optimalGradientDemosaicing();

    /**
     * Construct a functor using the given parameters
     */
    optimalGradientDemosaicing(const parameters& par);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    optimalGradientDemosaicing(const optimalGradientDemosaicing& other);

    /**
     * Destructor
     */
    virtual ~optimalGradientDemosaicing();

    /**
     * Returns the name of this type ("lti::optimalGradientDemosaicing")
     */
    virtual const std::string& name() const;

    /**
     * Transforms a raw Bayer pattern image into an lti::image using
     * an optimized interpolation scheme as described in the class
     * documentation.
     *
     * @param src contains a raw image of one of the parameters::eBayerPattern
     * @param dest the demosaiced image
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const matrix<ubyte>& src, image& dest) const;

    /** make inherited apply member functions available*/
    using bayerDemosaicing::apply;

    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    optimalGradientDemosaicing& copy(const optimalGradientDemosaicing& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    optimalGradientDemosaicing& operator=(const optimalGradientDemosaicing& other);

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual optimalGradientDemosaicing* clone() const;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual optimalGradientDemosaicing* newInstance() const;

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;


  };
}

#endif

