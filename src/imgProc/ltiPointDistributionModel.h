/*
 * Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006
 * Lehrstuhl fuer Technische Informatik, RWTH-Aachen, Germany
 * Copyright (C) 2009
 * Pedro Urenna / Escuela de Ing. Electronica
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
 * \file   ltiPointDistributionModel.h
 *         Contains the data structure used with the ASM.
 * \author Benjamin Winkler
 * \date   23.10.2001
 * \author Pedro Urenna
 * \date   01.07.2009
 * revisions ..: $Id: ltiPointDistributionModel.h,v 1.4 2009-09-29 00:32:16 alvarado Exp $
 */


#ifndef _LTI_POINT_DISTRIBUTION_MODEL_H_
#define _LTI_POINT_DISTRIBUTION_MODEL_H_


#include "ltiIoObject.h"
#include "ltiPoint.h"
#include "ltiVector.h"
#include "ltiMatrix.h"
#include "ltiVector.h"

namespace lti {
  /**
   * Point Distribution Model
   *
   * This class is the data structure for a Point Distribution Model (PDM).
   *
   * A PDM allows to represent a shape and its possible deformations, by a
   * mean shape and the eigenvectors and eigenvalues of the shape set. It is
   * estimated by the pdmGenerator and can be used to regularize the
   * deformations of an activeShapeModel (ASM).
   *
   * For a description of PDMs see: Sonka, "Image Processing, Analysis, and
   * Machine Vision", p.380ff.
   *
   * Additional Remark:
   *
   * In ASMs a shape \e x is described as the sum of the mean shape and the 
   * weighted eigenvectors.
   *
   * Note that usually a subset of all eigenvectors is chosen (i.e. the
   * eigenvectors with the first n largest eigenvalues) for data compression
   * reasons.
   *
   * <code>
   * x = xMean + eigenVectors*w
   * </code>
   *
   * where the eigenvectors are stored in the columns of the matrix.
   *
   * @ingroup gShape
   *
   * @see pdmGeneration
   * @see activeShapeModel
   * @see pointDistributionModel
   */
  class pointDistributionModel : public ioObject {
  public:

    /**
     * Default constructor
     */
    pointDistributionModel();

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    pointDistributionModel(const pointDistributionModel& other);

    /**
     * Destructor
     */
    virtual ~pointDistributionModel();

    /**
     * Copy data of "other" pointDistributionModel.
     */
    pointDistributionModel& copy(const pointDistributionModel& other);

    /**
     * Assigment operator (alias for copy(other)).
     *
     * @param other the pointDistributionModel to be copied
     * @return a reference to the actual pointDistributionModel
     */
    pointDistributionModel& operator=(const pointDistributionModel& other);

    /**
     * Returns the name of this class
     */
    const std::string& name() const;

    /**
     * Clone this class
     */
    pointDistributionModel* clone() const;

    /**
     * Create new instance of the same type as this class
     */
    pointDistributionModel* newInstance() const;

    /**
     * Write the pointDistributionModel in the given ioHandler
     * @param handler the ioHandler to be used
     * @param complete if true (the default) the enclosing begin/end will
     *        be also written, otherwise only the data block will be written.
     * @return true if write was successful
     */
    virtual bool write(ioHandler& handler, const bool complete=true) const;

    /**
     * Read the pointDistributionModel from the given ioHandler
     * @param handler the ioHandler to be used
     * @param complete if true (the default) the enclosing begin/end will
     *        be also written, otherwise only the data block will be written.
     * @return true if write was successful
     */
    virtual bool read(ioHandler& handler,const bool complete=true);

    //
    // Types and attributes
    //

    /**
     * Type for a single PDM shape.
     *
     * A shape is a vector of landmark points of a 2D object. Generally
     * this will be created by sampling a fixed number of N points along the
     * boundary.
     */
    typedef vector<fpoint> shape;

    /**
     * @name Attributes
     */
    //@{

    /**
     * The mean (average) shape of the model.
     */
    shape meanShape;

    /**
     * The matrix with all eigenvectors computed over a set of shapes.
     * Eigenvectors are sorted in descending order of magnitude of their
     * corresponding eigenvalues, in the \e columns of the matrix.  This allows
     * to find the projections just by multiplying by the weights vectors.
     */
    dmatrix eigenVectorMatrix;

    /**
     * The eigenvalues, that belong to the eigenvectors, stored in a vector.
     * The eigenvalue is the variance of the set of shapes along the direction
     * of the eigenvector, that it belongs to.
     */
    dvector varianceVector;

    //@}

  };

}

#endif
