/*
 * Copyright (C) 1998, 2005, 2006, 2007
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
 * \file   ltiBayerDemosaicing.h
 *         Contains the class lti::bayerDemosaicing which is the base
 *         class for all types of Bayer pattern demosaicing
 * \author Arnd Hannemann
 * \author Peter Doerfler
 * \date   10.12.2004
 *
 * revisions ..: $Id: ltiBayerDemosaicing.h,v 1.5 2012-09-16 04:58:33 alvarado Exp $
 */

#ifndef _LTI_BAYER_DEMOSAICING_H_
#define _LTI_BAYER_DEMOSAICING_H_


#include "ltiRGBAPixel.h"
#include "ltiMatrix.h"
#include "ltiImage.h"
#include "ltiChannel.h"
#include "ltiChannel8.h"
#include "ltiFunctor.h"

namespace lti {

  /**
   * Bayer Filter Demosaicing
   *
   * The class bayerDemosaicing is the base abstract class for all functors
   * that implement demosaicing of raw (Mono8) data taken with a
   * camera that uses a Bayer pattern to simulate 3 CCD chips.
   *
   * There are four different configurations of the red(R), green(G),
   * and blue(B) filters (see eBayerPattern). It is
   * important to set the correct one for satisfactory results. A
   * wrong bayerPattern usually results in false colors in the
   * reconstructed image.
   *
   * The Bayer image format uses 1 byte per pixel, where the value of one color
   * component R, G or B is always given, while the other two have to be
   * interpolated from the neighbors.
   *
   * Many digital cameras use highly sophisticated demosaicing
   * methods. Unfortunately, many of these are patented and cannot be
   * used in the LTI-Lib. Algorithms differ widely in image quality
   * and processing speed. It is desirable to find some hints about
   * these issues for each algorithm in the documentation. For speed
   * the number of multiplication and addition per pixel is a good
   * measure.
   *
   * @ingroup gAcquisition
   */
  class bayerDemosaicing : public functor {
  public:
    
    /**
     * In a Bayer pattern there are three different color filters
     * used in front of the actual pixels of the CCD chip. They have
     * the colors red(R), green(G), and blue(B). On a 2x2 grid of
     * pixels two are always green and these always lie on a
     * diagonal. This results in four possible constellations for
     * the three colors.
     *
     * If NoBayer is chosen then no demosaicing takes place. This
     * makes sense for mono cameras or to avoid demosaicing.
     */
    enum eBayerPattern {
      NoBayer, /**< no Bayer pattern used, no demosaicing, mono camera */
      RGGB,    /**< Red, Green in first row, Green, Blue in second */
      BGGR,    /**< Blue, Green in first row, Green, Red in second */
      GBRG,    /**< Green, Blue in first row, Red, Green in second */
      GRBG     /**< Green, Red in first row, Blue, Green in second */
    };
    
    /**
     * The parameters for the class bayerDemosaicing
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

      /**
       * Specifies which Bayer pattern the raw image was taken with.
       *
       * Default: RGGB
       */
      bayerDemosaicing::eBayerPattern bayerPattern;

    };

    /**
     * Default constructor
     */
    bayerDemosaicing();

    /**
     * Construct a functor using the given parameters
     */
    bayerDemosaicing(const parameters& par);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    bayerDemosaicing(const bayerDemosaicing& other);

    /**
     * Destructor
     */
    virtual ~bayerDemosaicing();

    /**
     * Transforms a raw Bayer pattern image into an lti::image. The
     * method used for this process is determined by the subclass
     * used.
     *
     * @param src contains a raw image of one of the parameters::eBayerPattern
     * @param dest the demosaiced image
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const matrix<ubyte>& src, image& dest) const=0;

    /**
     * Transforms a raw Bayer pattern image into a channel. This is a
     * shortcut. It uses apply(const matrix<ubyte>&,
     * image&) and then the castFrom member function.
     *
     * @param src contains a raw image of one of the parameters::eBayerPattern
     * @param dest the demosaiced image
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const matrix<ubyte>& src, channel& dest) const;

    /**
     * Transforms a raw Bayer pattern image into a channel8. This is a
     * shortcut. It uses apply(const matrix<ubyte>&,
     * image&) and then the castFrom member function.
     *
     * @param src contains a raw image of one of the parameters::eBayerPattern
     * @param dest the demosaiced image
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const matrix<ubyte>& src, channel8& dest) const;

    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    bayerDemosaicing& copy(const bayerDemosaicing& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    bayerDemosaicing& operator=(const bayerDemosaicing& other);

    /**
     * Returns the name of this class.
     */
    virtual const std::string& name() const=0;    

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual bayerDemosaicing* clone() const=0;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual bayerDemosaicing* newInstance() const=0;

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * This function is mainly useful for debugging and visualizing
     * Bayer filtered images. Each value in \a src is only displayed
     * in the color that it was taken with according to the \a
     * bayerPattern.
     *
     * @param src raw data to be shown in color as Bayer filtered
     * @param bayerPattern type of Bayer filter
     * @param dest each pixel has one color only
     */
    static void generateBayerImage(const matrix<ubyte>& src,
                                   const eBayerPattern& bayerPattern,
                                   image& dest);

    /**
     * This function is mainly useful for debugging and visualizing
     * Bayer filtered images. Only the appropriate color of each pixel
     * is copied from \a src to \a dest. This is done assuming the
     * given \a bayerPattern.
     *
     * @param src image to be shown as Bayer filtered
     * @param bayerPattern type of Bayer filter
     * @param dest each pixel has one color only
     */
    static void generateBayerImage(const image& src,
                                   const eBayerPattern& bayerPattern,
                                   image& dest);

    /**
     * Creates a Bayer mosaiced channel8, e.g. depending on the
     * setting of \a bayerPattern only the red, green, or blue value
     * of a pixel are copied into the \a dest matrix.
     *
     * @param src image containing full color information
     * @param bayerPattern eBayerPattern to use for mosaicing
     * @param dest Bayer filtered image
     */
    static void filter(const image& src, const eBayerPattern& bayerPattern,
                       channel8& dest);

  protected:

    /**
     * Clip the double \a val to a ubyte by first clipping to a value
     * between 0. and 255. and then static_cast.
     */
    inline ubyte clip(const double& value) const;


    /**
     * If one of the applies returning an image is called and
     * patternType is NoBayer then assume the camera to return grey
     * values and return a grey image
     */
    bool generateGreyImage(const matrix<ubyte>& src, image& dest) const;
    
  };


  /**
   * Read function for bayerDemosaicing::eBayerPattern
   *
   * @ingroup gStorable
   */
  bool read(ioHandler& handler, bayerDemosaicing::eBayerPattern& data);

  /**
   * Write function for bayerDemosaicing::eBayerPattern
   *
   * @ingroup gStorable
   */
  bool write(ioHandler& handler, const bayerDemosaicing::eBayerPattern& data);

}

#include "ltiBayerDemosaicing_inline.h"

#endif

