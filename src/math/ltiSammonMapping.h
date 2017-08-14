/*
 * Copyright (C) 2002, 2003, 2004, 2005, 2006
 * Lehrstuhl fuer Technische Informatik, RWTH-Aachen, Germany
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
 * \file   ltiSammonMapping.h
 *         Contains the class lti::sammonMapping,
 *         which maps points in a high dimensional space into a lower space.
 * 
 * \author Peter Doerfler
 * \date   9.10.2002
 *
 * revisions ..: $Id: ltiSammonMapping.h,v 1.2 2008-06-03 17:35:15 alvarado Exp $
 */


#ifndef _LTI_SAMMON_MAPPING_H_
#define _LTI_SAMMON_MAPPING_H_

#include "ltiFunctor.h"
#include "ltiVector.h"
#include "ltiMatrix.h"
#include "ltiProgressReporter.h"

namespace lti {
  /**
   * Sammon Mapping.
   *
   * Performs Sammon's Mapping. The data from some high dimensional
   * source space is mapped to a destination space of lower
   * dimensionality. In the process Sammon's Mapping tries to keep
   * distances between samples in the destination space as close to
   * those in the source space. Typical uses are data analysis or
   * visualization of SOFM networks. For details see:
   *
   * John W. Sammon, Jr. <i>A nonlinear mapping for data structure
   * analysis.</i> IEEE Transactions on Computers, C-18(5):401-409, May
   * 1969.
   *
   * There are several parameters the algorithm can be tuned
   * with. Initialization is usually performed using principal
   * component analysis. It projects the data from the original space
   * into the destination space using the eigenvectors with largest
   * eingenvalues. The other option --- random initialization ---
   * seems to be useful for demonstration purposes, only.
   *
   * Three methods are available for minimizing the error or stress
   * of the mapping: gradient descent, gradient descent with momentum
   * and steepest descent. Of these, steepest descent usually
   * requires the smallest number of iterations, but each iteration
   * is more costly. Further, with many planes in the error surface,
   * the computation of the second derivative might not be
   * possible. In case of 0 this value is set to 1.E-4 as a
   * work-around. The gradient descent methods are much more
   * stable. Their convergence, however, is much slower. Using
   * momentum can lead to faster convergence than regular gradient
   * descent but also needs more computation per iteration. According
   * to Sammon, errors should be below 1E-2 to be considered
   * good. Usually, much smaller errors are achieved.
   */
  class sammonMapping : public functor, public progressReporter {
  public:
    /**
     * Initialization for the lower dimensional image space. The
     * Random version uses initBox for the size of the hyper cube
     * the values are drawn from. The PCA version was suggested by
     * Sammon for real-world problems. It uses the projection of the
     * data points into the image space as initialization.
     */
    enum eInit {
      Random, /*!< choose initial values randomly from hyper cube initBox */
      PCA     /*!< initial values are projection of the data by PCA */
    };
    
    /**
     * Different methods for seeking the minimum of the error:
     * <p><ul>
     * <li><b>Steepest</b> uses steepest descent, i.e. the gradient
     *     is divided by the norm of the second derivative.</li>
     * <li><b>Gradient</b> uses regular gradient descent</li>
     * <li><b>Momentum</b> uses gradient descent with momentum. The momentum
     *      is set by the parameter mu.</li>
     * </ul>
     */
    enum eSearch {
      Steepest, /*!< steepest descent */
      Gradient, /*!< gradient descent */
      Momentum  /*!< gradient descent with momentum */
    };

    /**
     * The parameters for the class sammonMapping
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
      // The parameters
      // ------------------------------------------------

      /**
       * The number of dimensions of the output space. Since Sammon's
       * Mapping is mostly used for visualization the dafault is 2.
       *
       * Default value: 2
       */
      int dimensions;

      /**
       * Number of steps.
       *
       * Default is 200.
       */
      int steps;

      /**
       * Distance threshold
       *
       * All patterns which lie with this distance or less with another pattern
       * will be ignored.
       *
       * Default value: 0.0
       */
      double distanceThresh;

      /**
       * Threshold for the error of the mapping. By default not used, thus 0.
       *
       * Default value 0.0
       */
      double errorThresh;

      /**
       * A 'learn' rate or step size. According to Kohonen it should
       * be somewhere between 0.3 and 0.4. 
       *
       * The default is 0.35
       */
      double alpha;

      /**
       * Sets the initType. 
       *
       * Default is PCA.
       */
      eInit initType;

      /**
       * Hyper cube random init values are chosen from in case
       * initType is Random. Each dimension ranges from 0 to the given
       * value. Take care to choose the same dimesionality for initBox
       * as the value of dimensions. 
       *
       * Default is a unit cube.
       */
      dvector initBox;

      /**
       * Sets the type of search method used to minimize the
       * error. 
       *
       * Default is Steepest.
       */
      eSearch searchType;

      /**
       * The momentum. Used if searchType is set to Momentum. 
       *
       * Default is 0.1
       */
      double mu;

    };

    /**
     * Default constructor.
     */
    sammonMapping();


    /**
     * Default constructor with parameters.
     */
    sammonMapping(const parameters& par);

    /**
     * Copy constructor.
     *
     * @param other the object to be copied
     */
    sammonMapping(const sammonMapping& other);

    /**
     * Destructor
     */
    virtual ~sammonMapping();

    /**
     * Returns the name of this class
     */
    virtual const std::string& name() const;

    /**
     * Map the vectors in the rows of the \c src matrix into a lower 
     * dimensional space.
     *
     * @param src dmatrix with the source data.
     * @param dest dmatrix where the result will be left.
     * @param error the error of the mapping
     * @return true if apply successful or false otherwise.
     */
    bool apply(const dmatrix& src,dmatrix& dest,double& error) const;

    /**
     * Operates on a copy of the given %parameters.
     *
     * @param src dmatrix with the source data.
     * @param dest dmatrix where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const dmatrix& src,dmatrix& dest) const;

    /**
     * Copy data of "other" functor.
     *
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    sammonMapping& copy(const sammonMapping& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    sammonMapping& operator=(const sammonMapping& other);

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual sammonMapping* clone() const;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual sammonMapping* newInstance() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;


  protected:
    /** 
     * Random initialization 
     */
    void initRandom(dmatrix& dest) const ;

    /**
     * Initialization using PCA 
     */
    void initPca(const dmatrix& data, dmatrix& dest) const ;

  };
}

#endif
