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
 * \file   ltiComprehensiveColorNormalization.h
 *         Contains the functor comprehensiveColorNormalization.
 * \author Marius Wolf	
 * \date   19.03.2003
 *
 * $Id: ltiComprehensiveColorNormalization.h,v 1.3 2006-06-06 11:35:37 doerfler Exp $
 */

#ifndef _LTI_COMPREHENSIVE_COLOR_NORMALIZATION_H_
#define _LTI_COMPREHENSIVE_COLOR_NORMALIZATION_H_

#include "ltiColorNormalization.h"

namespace lti {

  /**
   * This class implements the algorithm described in the paper
   * "Comprehensive Color Image Normalization" by Finlayson, Schiele
   * and Crowley.
   *
   * The original paper can be found here:
   * http://web.media.mit.edu/~bernt/Pubs/eccv98.ps.gz
   * 
   * The algorithm can eliminate dependencies from lighting geometry and
   * illumination color, normalizing both chromaticities and intensities of
   * each channel.
   *
   * In brief the algorithm is an iterative process. It stopped when
   * either the parameters::maxIterations have been reached or the
   * maximum euclidian square distance between two pixel values of
   * consecutive iterations is lower than parameters::maxDistance.
   *
   * In each iteration first the pixel values are normalized to sum 1
   * and the average value of each channel is calculated. Then each
   * element of each pixel is again normalized with a third of its
   * respective channel average.
   *
   * @ingroup gColor
   */
  class comprehensiveColorNormalization : public colorNormalization {
  public:
    /**
     * the parameters for the class comprehensiveColorNormalization
     */
    class parameters : public colorNormalization::parameters {
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
       * copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& operator=(const parameters& other);

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
       * read the parameters from the given ioHandler
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
       * Maximum number of iterations done for an image.
       *
       * Default is 5.
       */
      int maxIterations;

      /**
       * Threshold for the maximum allowed square euclidian distance
       * between pixel values of consecutive iterations. The distance
       * is calculated in color space. The normalization stops if the
       * maximum distance found in any pixel to the last iteration is
       * below this value.
       *
       * Since we are dealing with three channels the distance can be
       * in the interval [0,3]. Choosing 0 makes no sense, unless you
       * want all iterations. Likewise a high value will always result
       * in just one iteration.
       *
       * Default: 1.f
       */
      float maxDistance;
    };

    /**
     * default constructor
     */
    comprehensiveColorNormalization();

    /**
     * Construct a functor using the given parameters
     */
    comprehensiveColorNormalization(const parameters& par);

    /**
     * copy constructor
     * @param other the object to be copied
     */
    comprehensiveColorNormalization(const comprehensiveColorNormalization& other);

    /**
     * destructor
     */
    virtual ~comprehensiveColorNormalization();

    /**
     * Normalize the color of \a srcdest.
     *
     * @param srcdest image with the source data.  The result
     *                 will be left here too.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(image& srcdest) const;

    /**
     * Normalize the color of \a src and leave the result in \a dest.
     *
     * @param src image with the source data.
     * @param dest image where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const image& src,image& dest) const;
      
    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    comprehensiveColorNormalization& 
    copy(const comprehensiveColorNormalization& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    comprehensiveColorNormalization& 
    operator=(const comprehensiveColorNormalization& other);

    /**
     * Returns the name of this functor.
     */
    virtual const std::string& name() const;

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual comprehensiveColorNormalization* clone() const;

    /**
     * returns a pointer to a new instance of this functor.
     */
    virtual comprehensiveColorNormalization* newInstance() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

  protected:
    /**
     * One iteration of the comprehensive normalization from src to dest
     *
     * This method assumes that all channels are Connected 
     * (see matrix::getMode())
     * 
     * The square of the maximum euclidian square distance between the
     * src and the dest channels will be left in \a dist
     */
    bool step(const channel& srcred,
              const channel& srcgreen,
              const channel& srcblue,
              channel& destred,
              channel& destgreen,
              channel& destblue,
              float& dist) const;

  };
}

#endif



