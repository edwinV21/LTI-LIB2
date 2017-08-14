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
 * \file   ltiColorProbabilityMapBase.h
 *         Contains the class lti::colorProbabilityMapBase, an abstract class
 *         of all functors computing color probability maps.
 * \author Benjamin Winkler
 * \author Florian Bley
 * \author Pablo Alvarado
 *
 * \date   30.01.2001
 *
 * revisions ..: $Id: ltiColorProbabilityMapBase.h,v 1.2 2008-12-28 04:18:34 alvarado Exp $
 */


#ifndef _LTI_COLOR_PROBABILITY_MAP_BASE_H_
#define _LTI_COLOR_PROBABILITY_MAP_BASE_H_


#include "ltiFunctor.h"
#include "ltiImage.h"
#include "ltiHistogram.h"

namespace lti {
  /**
   * Base class of color probability maps.
   *
   * This is the base abstract class for all functors that compute probability
   * maps based on color information.
   *
   * It defines and controls the internal state of the functors containing
   * the color models used for the object and the non-objects, which can
   * be created with the class lti::colorModelEstimation.
   *
   * \warning: Note that this class is different than its equivalent in the
   * LTI-Lib-1, specially in the fact that the histograms representing the
   * color models are not assigned to the parameters any more, but are part of
   * the functor's state, and have to be provided separately before the first
   * apply is employed.
   *
   * @see parameters
   *
   * @ingroup gColor
   */
  class colorProbabilityMapBase : public functor {
  public:
    /**
     * The parameters for the class colorProbabilityMapBase
     */
    class parameters : public functor::parameters {
    public:
      /**
       * Default constructor
       */
      parameters();

      /**
       * Copy constructor
       *
       * @param other the parameters object to be copied
       */
      parameters(const parameters& other);

      /**
       * Destructor
       */
      ~parameters();

      /**
       * Returns name of this type
       */
      const std::string& name() const;

      /**
       * Copy the contents of a parameters object.
       *
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& copy(const parameters& other);

      /**
       * Clone
       */
      parameters* clone() const;

      /**
       * New instance
       */
      parameters* newInstance() const;

      /**
       * Copy the contents of a parameters object.
       *
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& operator=(const parameters& other);

      /**
       * Write the parameters in the given ioHandler
       *
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
       * Overall probability for the object (a-priori object probability)
       *
       * Default value: 0.5
       */
      float objectProbability;


      /**
       * Number of Iterations.
       *
       * The first iteration always computes the probability map assuming that
       * all pixels in the color image share the same <em>a-priori</em>
       * probability given by parameters::objectProbability.
       *
       * The first iteration always takes the parameter 
       * \c parameters::objectProbability as a-priori value.
       *
       * Each subsequent iteration computes the <em>a-priori</em> value for
       * each pixel as some averaging of the probabilities computed in the
       * previous iteration in the neighbourhood of pixel computed.  The
       * available averaging operators are gaussian filters or square filters.
       * The size of the neighbourhood will be specified by the
       * parameters::windowSize parameter.

       * This member should be always greater than 1. If zero or negative
       * values are given, then 1 will be assumed.  At this time, the functor
       * requires BOTH models (object AND non-object) to compute more than
       * one iteration.
       *
       *
       * Default value: 1
       */
      int iterations;

      /**
       * Use Gaussian Filter
       *
       * This specifies the kind of filter used to average the probabilities in
       * a previous iteration in order to compute the current iteration's
       * a-priori probability value.
       *
       * If \c true, a Gaussian averaging filter will be used. 
       *
       * If \c false, a square filter will be used.
       *
       * Default value: \c false (use a square kernel)
       */
      bool gaussian;

      /**
       * Averaging window size.
       *
       * Size for the gaussian kernel or the square kernel used to compute
       * the average probability to be used as <em>a priori</em> probability in
       * the next iteration.
       *
       * Default value: 5 (use a 5x5 kernel)
       */
      int windowSize;

      /**
       * Variance of the Gaussian kernel used.
       *
       * In case the averaging operator is a gaussian kernel, this is the
       * variance to be used.
       *
       * Default value: -1 (meaning the lti::gaussKernel1D should compute a
       *                    default variance)
       */
      double variance;
    };

    /**
     * Default constructor
     */
    colorProbabilityMapBase();

    /**
     * Copy constructor.
     *
     * @param other the object to be copied
     */
    colorProbabilityMapBase(const colorProbabilityMapBase& other);

    /**
     * Destructor
     */
    virtual ~colorProbabilityMapBase();

    /**
     * Returns the name of this class
     */
    virtual const std::string& name() const;

    /**
     * Update parameters.
     */
    virtual bool updateParameters();

    /**
     * Copy data of "other" functor.
     *
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    colorProbabilityMapBase& copy(const colorProbabilityMapBase& other);

    /**
     * Copy data of "other" functor.
     *
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    colorProbabilityMapBase& operator=(const colorProbabilityMapBase& other);

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual colorProbabilityMapBase* clone() const = 0;

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual colorProbabilityMapBase* newInstance() const = 0;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * Read the functor from the given ioHandler. 
     *
     * \warning It cannot guessed here which kind of histograms are being used
     * in the derived class, therefore the derived class must initialize the
     * internal histograms with valid instances before calling this method.
     *
     *
     * @param handler the ioHandler to be used
     * @param complete if true (the default) the enclosing begin/end will
     *        be also written, otherwise only the data block will be written.
     * @return true if write was successful
     */
    virtual bool read(ioHandler& handler,const bool complete=true);

    /**
     * Write the functor into the given ioHandler. 
     *
     * @param handler the ioHandler to be used
     * @param complete if true (the default) the enclosing begin/end will
     *        be also written, otherwise only the data block will be written.
     * @return true if write was successful
     */
    virtual bool write(ioHandler& handler,const bool complete=true) const;

    /**
     * @name Color Model Management
     */
    //@{

    /**
     * Set the \a object color model.
     *
     * A copy of the object will be done.
     *
     * \note Setting the color model will automatically compute an internal
     * LUT, so this method will take its time.  Therefore, is preferable to use
     * the setColorModels() method, to compute this table just once.
     */
    virtual bool setObjectColorModel(const dhistogram &objModel);

    /**
     * Use the given \a object color model.
     *
     * No copy of the object will be done, so you have to ensure that the
     * object remains valid as long as the apply methods are used.  Note that
     * in multiple thread applications must also ensure that the models do not
     * change while using the apply methods.
     *
     * \note Setting the color model will automatically compute an internal
     * LUT, so this method will take its time.  Therefore, is preferable to use
     * the setColorModels() method, to compute this table just once.
     */
    virtual bool useObjectColorModel(const dhistogram &objModel);

    /**
     * Returns a reference to the object color model
     */
    const dhistogram &getObjectColorModel() const;
    
    /**
     * Check if the object color model is valid.
     *
     * Valid means that the model has already been set
     * (with setObjectColorModel) and that the dimensionality of the model
     * fits with the dimension required by the derived class requires.
     *
     * @return true if valid, false otherwise.
     */
    virtual bool isObjectColorModelValid() const = 0;
    
    /**
     * Set the \a non-object color model
     *
     * A copy of the object will be done.
     *
     * \note Setting the color model will automatically compute an internal
     * LUT, so this method will take its time.  Therefore, is preferable to use
     * the setColorModels() method, to compute this table just once.
     */
    virtual bool setNonObjectColorModel(const dhistogram &nonObjModel);
    
    /**
     * Use the given \a non-object color model
     *
     * No copy of the object will be done, so you have to ensure that the
     * object remains valid as long as the apply methods are used.  Note that
     * in multiple thread applications must also ensure that the models do not
     * change while using the apply methods.
     *
     * \note Setting the color model will automatically compute an internal
     * LUT, so this method will take its time.  Therefore, is preferable to use
     * the setColorModels() method, to compute this table just once.
     */
    virtual bool useNonObjectColorModel(const dhistogram &nonObjModel);

    /**
     * Returns a reference to the non-object color model
     */
    const dhistogram &getNonObjectColorModel() const;
    
    /**
     * Check if the non-object color model is valid
     *
     * Valid means that the model has already been set
     * (with setObjectColorModel) and that the dimensionality of the model
     * fits with the dimension which the derived class requires.
     *
     * @return true if valid, false otherwise.
     */
    virtual bool isNonObjectColorModelValid() const = 0;

    /**
     * Set the \a object and \a non-object color models.
     *
     * \note Setting the color model will automatically compute an internal
     * LUT, so this method will take its time.  Therefore, is preferable to use
     * the setColorModels() method, to compute this table just once.
     */
    virtual bool setColorModels(const dhistogram& objModel,
                                const dhistogram& nonObjModel);

    /**
     * Set the \a object and \a non-object color models.
     *
     * \note Setting the color model will automatically compute an internal
     * LUT, so this method will take its time.  Therefore, is preferable to use
     * this setColorModels() method, to compute this table just once.
     *
     * If both models are the same, then no regeneration will be done, unless
     * the flag forceRegeneration is set to true
     */
    virtual bool useColorModels(const dhistogram& objModel,
                                const dhistogram& nonObjModel,
                                const bool forceRegeneration=false);
    //@}
    

  protected:
    
    /**
     * Specify if the object histogram is administrated by this class or not.
     */
    bool ownObjModel_;

    /**
     * Specify if the non-object histogram is administrated by this class or
     * not.
     */
    bool ownNonObjModel_;

    /**
     * Internal pointer to non-object color model
     */
    const dhistogram* nonObjectColorModel_;

    /**
     * Internal pointer to object color model
     */
    const dhistogram* objectColorModel_;


    /**
     * Generate probability histogram.
     *
     * This is automatically called in the setParameters method, and the
     * set/use models methods too.
     *
     * The generate method initialize the probabilityHistogram method which
     * contain the pre-computations based on the bayes-theorem to compute
     * a probability of a pixel to be an object or not.
     */
    virtual bool generate();

    /**
     * Generate probability histogram from one histogram.
     */
    virtual bool generate(const dhistogram &objectModel);

    /**
     * Generate probability histogram from two histograms.
     */
    virtual bool generate(const dhistogram &objectModel,
                          const dhistogram &nonObjectModel);

    /**
     * Probability histogram
     *
     * This histogram contains already the computations for the first iteration
     * of the color map.
     */
    dhistogram probabilityHistogram_;

    /**
     * Generate lookup table for faster histogram element access
     */
    void generateLookupTable(const ivector &dimensions);

    /**
     * Element lookup table
     */
    matrix<ubyte> lookupTable_;
  };
}

#endif
