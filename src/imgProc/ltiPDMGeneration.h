/*
 * Copyright (C) 2002, 2003, 2004, 2005, 2006
 * Lehrstuhl fuer Technische Informatik, RWTH-Aachen, Germany
 * Copyright (C) 2009
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
 * \file   ltiPDMGeneration.h
 *         Class to compute a shape model from a set of sample shapes.
 * \author Benjamin Winkler
 * \date   26.04.2001
 * \author Pablo Alvarado
 * \date   01.07.2009
 * revisions ..: $Id: ltiPDMGeneration.h,v 1.4 2010-09-10 00:15:51 alvarado Exp $
 */

#ifndef _LTI_PDM_GENERATOR_H_
#define _LTI_PDM_GENERATOR_H_


#include "ltiFunctor.h"
#include "ltiActiveShapeModel.h"
#include "ltiSTLIoInterface.h"


namespace lti {
  /**
   * Generation for Point Distribution Models (PDM)
   *
   * This class creates a pointDistributionModel (PDM) from a given a set of
   * shapes of type pointDistributionModel::shape.
   *
   * All shapes must have an equal number of points. These are ordered and
   * should ideally have a fixed correspondence across different shapes.
   *
   * @ingroup gShape
   *
   * @see pdmGeneration::parameters
   *
   * @see pdmGeneration
   * @see activeShapeModel
   * @see pointDistributionModel
   */
  class pdmGeneration : public functor {
  public:
    /**
     * Parameters for the class pdmGeneration
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
       * Returns name of this type
       */
      virtual const std::string& name() const;

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
       * Returns a pointer to a clone of the parameters
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
       * Read the parameters in the given ioHandler
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
       * This %parameter is an upper limit to the number of eigenvectors to
       * consider.
       *
       * This is the highest number of weights used, no matter which value
       * you set for maxRelativeWeights.
       *
       * Eigenvectors are ordered by magnitude of eigenvalue, so that the ones
       * with high eigenvalues are chosen first. All others are discarded.
       *
       * Only the first maxNumberOfWeights eigenVectors are taken into account
       * from a total of 2N, where N is the number of points in one shape.
       * Reasonable values lie between 0 and 2N.
       *
       * If set to 0 (default), all eigenVectors are allowed.
       *
       * Default value: 0
       */
      int maxNumberOfWeights;

      /**
       * This %parameter specifies a relative way to set the number of
       * eigenvectors used in the transformed space, and therefore, the
       * dimensionality of the subspace used.
       *
       * At least the first K eigenVectors are taken into account, for which
       * it holds:
       *
       * (eigenValue[0] + ... + eigenValue[K-1]) / 
       * sum(eigenValue) >= maxRelativeWeights
       *
       * If the value K obtained is larger than maxNumberOfWeights, then
       * maxNumberOfWeights is used.  If it is lower than such attribute, then
       * the lower value is used.
       *
       * Reasonable values range from 0.0 (none) to 1.0 (all).
       *
       * Note that a value of 1.0f indicates to use exactly the given
       * maxNumberOfWeights.
       *
       * Default value: 1.0f
       */
      float maxRelativeWeights;

      /**
       * Alignment interations
       *
       * Maximum number of iterations allowed for the shape alignment process
       *
       * Default: 100
       */
      int maxAlignmentIterations;
    };


    /**
     * Default constructor
     */
    pdmGeneration();

    /**
     * Constructor with parameters
     */
    pdmGeneration(const parameters& par);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    pdmGeneration(const pdmGeneration& other);

    /**
     * Destructor
     */
    virtual ~pdmGeneration();

    /**
     * Returns the name of this type ("pdmGeneration")
     */
    virtual const std::string& name() const;

    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    pdmGeneration& copy(const pdmGeneration& other);

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual pdmGeneration* clone() const;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual pdmGeneration* newInstance() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

    /** @name Training shape set
     *
     *  These members are for managing the training shape set.
     */
    //@{

    /**
     * Clear all considered shapes
     */
    void clear();

    /**
     * Take shape into consideration
     */
    bool consider(const pointDistributionModel::shape &theShape);
    
    /**
     * Take list of shapes into consideration
     */
    bool consider(const std::list<pointDistributionModel::shape> &theShapes);
    
    /**
     * Return number of shapes considered so far (not the number of points!).
     */
    int getN() const;
    
    //@}

    /**
     * Generate pointDistributionModel for the given dimension
     *
     * @param model a pointDistributionModel
     *
     * @return true if successful, false otherwise
     */
     bool apply(pointDistributionModel& model);

  private:
    
    /**
     * Generate model.
     *
     * Generate a point distribution model using all shapes considered so far
     */
    void generate(pointDistributionModel &theModel);
    
    /**
     * Normalize shape (i.e. move to (0,0), scale to 1.0)
     */
    void normalizeShape(pointDistributionModel::shape &theShape) const;

    /**
     * This method does:
     * - align all shapes to cog=(0,0), meanDistance=1.0
     * - create mean shape
     * - align all shapes iteratively to the mean shape
     * @return the mean shape
     */
    void alignShapes();

    /**
     * Create mean shape
     */
    void createMeanShape(pointDistributionModel::shape& meanShape) const;

    /**
     * Convert the list of shapes to a matrix 
     */
    void calculateShapeMatrix(dmatrix& data) const;

    /**
     * Create shape vector (x1, y1, x2, y2, ...)
     */
    void buildVector(const pointDistributionModel::shape &src,
                     dvector& vct) const;
    
    /**
     * Create shape vector (x1, y1, x2, y2, ...)
     */
    void buildShape(const dvector& vct,
                          pointDistributionModel::shape &src) const;
    
    /**
     * List of considered shapes
     */
    std::list<pointDistributionModel::shape> shapes_;

    /**
     * Needed to align the given shapes
     */
    activeShapeModel theASM_;

    /**
     * Shape size
     */
    int shapeSize_;
  };
}

#endif
