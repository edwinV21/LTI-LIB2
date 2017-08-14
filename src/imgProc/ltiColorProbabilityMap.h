/*
 * Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006
 * Lehrstuhl fuer Technische Informatik, RWTH-Aachen, Germany
 *
 * Copyright (C) 2008
 * Pablo Alvarado
 *
 * This file is part of the LTI-Computer Vision Library 2 (LTI-Lib-2)
 *
 * The LTI-Lib-2 is free software; you can redistribute it and/or
 * modify it under the terms of the BSD License.
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the authors nor the names of its contributors may be
 *    used to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/** 
 * \file   ltiColorProbabilityMap.h
 *         Contains the class lti::colorProbabilityMap, the main derived class
 *         from colorProbabilityMapBase, that uses 3D color histograms.
 * \author Benjamin Winkler
 * \author Florian Bley
 * \author Pablo Alvarado
 *
 * \date   30.01.2001
 *
 * revisions ..: $Id: ltiColorProbabilityMap.h,v 1.1 2008-12-27 20:14:56 alvarado Exp $
 */


#ifndef _LTI_COLOR_PROBABILITY_MAP_H_
#define _LTI_COLOR_PROBABILITY_MAP_H_


#include "ltiColorProbabilityMapBase.h"

namespace lti {
  /**
   * Probability Map based on 3D non-parametric (color) models.
   *
   * Creates a probability map given two color histogram, one
   * modelling the %object color and the other modeling the
   * non-object colors.
   *
   * The probability of a specified color \e c is calculated according to
   * the Bayes formula:
   *
   * \f[
   * p(obj|rgb) = \frac{p(c|obj) \cdot p(obj)}
   *             {(p(c|obj) \cdot p(obj) + p(c|nonobj) \cdot p(nonobj))}
   * \f]
   *
   * where p(obj) is the overall objectProbability (or a-priori probability),
   * p(nonobj) := 1 - p(obj).
   *
   * \f$p(c|obj)\f$ and \f$p(c|nonobj)\f$ are read from the given object
   * and non-object models.
   *
   * In case you provide only the object probability histogram, the non-object
   * histogram will be assumed to be uniformly distributed, i.e. all colors can
   * be non-object with the same probability.
   *
   * @see parameters
   *
   * @ingroup gColor
   */
  class colorProbabilityMap : public colorProbabilityMapBase {
  public:

    /**
     * The parameters for the class colorProbabilityMap are the same of its
     * base class.
     */
    typedef colorProbabilityMapBase::parameters parameters;

    // ---------------------------------------------------------------------
    
    /**
     * Default constructor
     */
    colorProbabilityMap();

    /**
     * Constructor
     *
     * @param theParams the parameters to be used
     */
    colorProbabilityMap(const parameters& theParams);

    /**
     * Copy constructor
     *
     * @param other the object to be copied
     */
    colorProbabilityMap(const colorProbabilityMap& other);

    /**
     * Destructor
     */
    virtual ~colorProbabilityMap();

    /**
     * Returns the name of this type ("colorProbabilityMap")
     */
    virtual const std::string& name() const;

    /**
     * Creates an object probability channel of an image (values range from 0.0
     * to 1.0).
     *
     * @param src image with the source data.
     * @param dest channel where the result will be left.
     * @return true is successful, false otherwise
     */
    virtual bool apply(const image& src,channel& dest) const;

    /**
     * Creates an object probability channel of an image (values range from 0.0
     * to 1.0).
     *
     * @param src image with the source data.
     * @param dest channel where the result will be left.
     * @param apriori By using this argument it is possible to include a given
     *                position-dependent apriori object probability channel in
     *                the computation of the object probability channel. 
     *                A value of 0.5 at a point indicates equal probabilities
     *                for object and non-object values. 
     *                Any bigger value (< 1.0) indicates a higher probability
     *                for  object values, any smaller value (> 0) indicates a
     *                lower probability.
     *                The a-priori channel must have the same size as the 
     *                input image.
     * @return true is successful, false otherwise
     */
    virtual bool apply(const image& src,
                             channel& dest,
                       const channel &apriori) const;

    /**
     * Returns the object probability for an rgb color value (values range
     * from 0.0 to 1.0).
     *
     * This method uses the given vector to compute the index of the histogram.
     * It \b MUST be preinitialized with a size of 3, or the method will crash.
     * 
     * The idea is to externally provide an ivector in order to spare some
     * creation time.
     *
     * @param src rgbaPixel with the source color.
     * @param theBin index vector computed by the method to the histogram 
     *               entry.
     *     
     * @return the object probability
     */
    virtual float apply(const rgbaPixel &src,ivector& theBin) const;

    /**
     * Returns the object probability for an rgb color value (values range
     * from 0.0 to 1.0).
     *
     * This method is thread safe, but much slower than the other one.
     * 
     * @param src rgbaPixel with the source color.
     * @return the object probability
     */
    virtual float apply(const rgbaPixel &src) const;

    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    colorProbabilityMap& copy(const colorProbabilityMap& other);

    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    colorProbabilityMap& operator=(const colorProbabilityMap& other);

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual colorProbabilityMap* clone() const;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual colorProbabilityMap* newInstance() const;

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * Read the functor from the given ioHandler. 
     *
     * The default implementation is to read just the parameters object.
     *
     * Since this virtual method needs to know the exact type of the
     * parameters to call the proper read method, it will just assume that the
     * current functor instance has a valid parameter set.  If this is not
     * the case, you need to reimplement the read method to set first a dummy
     * parameter object.
     *
     * @param handler the ioHandler to be used
     * @param complete if true (the default) the enclosing begin/end will
     *        be also written, otherwise only the data block will be written.
     * @return true if write was successful
     */
    virtual bool read(ioHandler& handler,const bool complete=true);

    /**
     * Check if the object color model is valid.
     *
     * Valid means that the model has already been set
     * (with setObjectColorModel) and that the dimensionality of the model
     * is 3.
     *
     * @return true if valid, false otherwise.
     */
    virtual bool isObjectColorModelValid() const;
    
    /**
     * Check if the non-object color model is valid
     *
     * Valid means that the model has already been set
     * (with setObjectColorModel) and that the dimensionality of the model
     * is 3.
     *
     * @return true if valid, false otherwise.
     */
    virtual bool isNonObjectColorModelValid() const;

  protected:

    /**
     * Compute the second and up iterations of a probability map
     * using the given a-priori probabilites per pixel.
     */
    void computeMap(const image& img,
                    channel& aPrioriDest) const;

  };
}

#endif
