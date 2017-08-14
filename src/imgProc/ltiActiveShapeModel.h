/*
 * Copyright (C) 2002, 2003, 2004, 2005, 2006
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
 * \author Holger Fillbrandt
 * \date   01.03.2002
 * \author Pedro Urenna
 * \date   01.07.2009
 * revisions ..: $Id: ltiActiveShapeModel.h,v 1.11 2013-03-02 16:13:43 alvarado Exp $
 */

#ifndef _LTI_ACTIVE_SHAPE_MODEL_H_
#define _LTI_ACTIVE_SHAPE_MODEL_H_


#include "ltiPointDistributionModel.h"
#include "ltiMatrixTransform.h"
#include "ltiFunctor.h"
#include "ltiBilinearInterpolation.h"
#include "ltiLinearLeastSquares.h"

namespace lti {
  /**
   * Active Shape Models (ASM)
   *
   * This is the base class for active shape models (ASM).
   *
   * ASMs are deformable 2D-templates, that can align themselves to image
   * features according to defined search strategies (see derived classes). The
   * deformation is regularized (i.e. constrained) by an underlying PDM
   * (lti::pointDistributionModel).
   *
   * This class provides the methods for aligning PDM shapes
   * (lti::pointDistributionModel::shape) and to correct invalid shapes to
   * satisfy the restrictions of a given PDM.  Furthermore, methods to obtain
   * information about the position of a shape are provided.  Shapes with
   * weighted points (point reliabilities) are also supported.
   *
   * @ingroup gShape
   *
   * @see activeShapeModel::parameters
   *
   * @see pdmGeneration
   * @see activeShapeModel
   * @see pointDistributionModel
   */
  class activeShapeModel : public functor {
  public:
    /**
     * Parameters for the class lti::activeShapeModel
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
       * Copy the contents of a parameters object.
       *
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
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
       * Returns a pointer to a clone of the parameters
       */
      virtual parameters* clone() const;

      /**
       * Returns a pointer to a new instance of the parameters
       */
      virtual parameters* newInstance() const;

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
       * The PDM (pointDistributionModel), that regularizes the deformation of
       * an ASM.
       */
      pointDistributionModel model;

      /**
       * Defines the limit for valid shapes in eigenspace.
       *
       * The dimensions that exceede the limit given by
       * varianceCoefficient*eigenvalue, where eigenvalue is
       * the corresponding variance, are truncated.
       *
       * Default value is 3.0
       */
      double varianceCoefficient;

      /**
       * Weight points by point reliabilities before correcting the shape,
       * i.e. unreliable points are moved towards the mean shape. Thus if
       * true, then use
       *
       * <code>
       *   w = (eigenVectorMatrix)^T * ((shape - meanShape) * reliabilities)
       * </code>
       *
       * instead of
       *
       * <code>
       *   w = (eigenVectorMatrix)^T * (shape - meanShape)
       * </code>
       *
       * Reliability values are then only considered for the geometric
       * transformations (shift, rotate, scale).
       *
       * In other words, in the trimShape() method, where you give
       * a reliability vector, you can indicate through this value if
       * the given vector should be ignored or not.
       *
       * By default this is set to false.
       *
       */
      bool reliabilityAsWeightingFactor;

    };

    /**
     * Default constructor
     */
    activeShapeModel();

    /**
     * Construct with the given parameters
     */
    activeShapeModel(const parameters& par);


    /**
     * Copy constructor
     *
     * @param other the object to be copied
     */
    activeShapeModel(const activeShapeModel& other);

    /**
     * Destructor
     */
    virtual ~activeShapeModel();

    /**
     * Returns the name of this class
     */
    const std::string& name() const;

    /**
     * Copy data of "other" functor.
     *
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    activeShapeModel& copy(const activeShapeModel& other);

    /**
     * Alias for copy member
     *
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    activeShapeModel& operator=(const activeShapeModel& other);

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual activeShapeModel* clone() const;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual activeShapeModel* newInstance() const;

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * @name Transformation
     *
     * Methods for transforming a shape from shape space to eigenspace and
     * viceversa.
     *
     * Eigenspace (w) -> shape space (x):
     *               x = eigenVectorMatrix * weightsVector  +  meanShape 
     *
     * Shape space (x) -> eigenspace (w): 
     *               w = (eigenVectorMatrix)^T * (shape - meanShape)
     */
    //@{

    /**
     * Convert a shape to a weights vector, i.e. project a shape into the PDM
     * eigenspace
     *
     * Formula: w = (eigenVectorMatrix)^T * (shape - meanShape)
     *
     * \warning: the shape is assumed to be aligned to position, size and
     * rotation of meanShape!
     *
     * @param src shape with the source data.
     * @param dest vector where the result will be left.
     *
     * @return true if successful, false otherwise
     */
    bool convertShapeToWeightsVector(const pointDistributionModel::shape &src,
                                     dvector &dest) const;

    /**
     * Convert a reliability-weighted shape to a weights vector,
     * i.e. project a shape into the pdm eigenspace
     *
     * Formula:
     *
     * w = (eigenVectorMatrix)^T * ((shape - meanShape) * reliabilities)
     *
     * \warning the shape is assumed to be aligned to position, size and
     * rotation of meanShape!
     *
     * @param src shape with the source data.
     * @param dest vector where the result will be left.
     * @param pointReliability vector containing point reliabilities.
     *
     * @return true if successful, false otherwise
     */
    bool convertShapeToWeightsVector(const pointDistributionModel::shape &src,
                                     const fvector &pointReliability,
                                     dvector &dest) const;

    /**
     * Convert a weights vector into a shape (i.e. shape space)
     *
     * Formula: x = eigenVectorMatrix * weightsVector + meanShape
     *
     * @param src vector with the source data.
     * @param dest shape where the result will be left.
     */
    void 
    convertWeightsVectorToShape(const dvector &src, 
                                pointDistributionModel::shape &dest) const;
    //@}

    /** 
     * @name Comparison
     * Methods for comparing and aligning two shapes.
     */
    //@{
    
    /**
     * Align a given shape to another.
     *
     * Note that the return geometricTransform::parameters describe the
     * transformation from the referenceShape to the unaligned shape! This
     * is done to align a given shape to a normalized shape and be able to
     * easily project this normalized shape (e.g. meanShape) on the original
     * shape.
     *
     * @param refShape fixed shape to be aligned to
     * @param shape shape to be aligned. the transformed shape will also be
     *              saved here.
     * @param p geometric parameters that were used to align shape to refShape
     *
     * @return true if successful, or false otherwise
     */
    bool 
    alignShape(const pointDistributionModel::shape &refShape,
               pointDistributionModel::shape &shape,
               matrixTransform<float>::parameters& p) const;
    
   
    /**
     * Align a given weighted shape to another.
     *
     * @param referenceShape fixed shape to be aligned to
     * @param pointReliability vector containing reliability values between
     *                         0 (unreliable) and 1 (reliable) for all points
     * @param shape shape to be aligned. the transformed shape will also be
     *              saved here.
     * @param p geometric parameters that were used to align shape to refShape
     *
     * @return true if successful, or false otherwise
     */
    bool
    alignShape(const pointDistributionModel::shape &referenceShape,
               const fvector &pointReliability,
               pointDistributionModel::shape &shape,
               matrixTransform<float>::parameters& p) const;

    /**
     * Force a shape to a valid one.
     *
     * This method aligns the given shape to the mean-shape of the model in use, 
     * trims the weights to valid ones and aligns the resulting fixed shape back
     * to the original position.
     *
     * @param srcdest Shape with the source data. 
     *                The result will be left here too.
     *
     * @return true if successful, false otherwise.
     */
    bool trimShape(pointDistributionModel::shape& srcdest) const;

    /**
     * Force a shape to a valid one.
     *
     * This method aligns the given shape to the mean-shape of the model in use, 
     * trims the weights to valid ones and aligns the resulting fixed shape back
     * to the original position.
     *
     * The point reliability allows to give different importance to each point.
     *
     * @param srcdest shape with the source data.
     *                The result will be left here too.
     * @param pointReliability the reliabilities of each point of the shape as
     *                         value between 0.0 and 1.0
     *
     * @return true if successful, false otherwise.
     */
    bool trimShape(pointDistributionModel::shape& srcdest, 
                   const fvector &pointReliability) const;

    /**
     * Correct weights vector according to varianceCoefficient.
     *
     * All vector entries that trimmed to lie between -c*v and c*v, where
     * c is the given varianceCoefficient and v is the variance of the
     * corresponding dimension (eigenvector), i.e. if a value is lower than
     * -c*v, than is cut to be -c*v, and if a value is larger than c*v then
     * c*v is used instead.
     *
     * @param srcdest dvector with the source data.  The result
     *                will be left here too.
     */
    void trimWeightsVector(dvector& srcdest) const;
    
    /**
     * Calculate mean difference of two shapes, i.e. the sum of squared
     * distances of all corresponding points divided by the number of points
     */
    bool 
    calculateShapeDifference(const pointDistributionModel::shape &firstShape,
                             const pointDistributionModel::shape &secondShape,
                             float& diff) const;
    
    /**
     * Calculate difference of two weighted shapes.
     */
    bool
    calculateShapeDifference(const pointDistributionModel::shape &firstShape,
                             const pointDistributionModel::shape &secondShape,
                             const fvector &pointReliability,
                             float& diff) const;
    //@}
    
    
    /**
     * @name Properties
     *
     * Methods for computing properties of one shape.
     */
    //@{
    
    /**
     * Calculate the center of a given shape.
     *
     * All points of the shape are summed and the results divided by the
     * number of points.
     *
     * @param shape the shape to compute the center.
     * @param center computed center.
     *
     * @return true if successful, false otherwise
     */    
    bool
    calculateShapeCenter(const pointDistributionModel::shape &shape,
                         fpoint& center) const;

    /**
     * Calculate the center of a given shape.
     *
     * All points of the shape are multiplied with their reliability factor,
     * then summed, and the results divided by the sum of reliabilities.
     *
     * @param shape the shape to compute the center.
     * @param pointReliability vector containing the reliability of each
     *                         point in the computations.  All values set
     *                         to 1.0f weights all points equally.
     * @param center computed center.
     *
     * @return true if successful, false otherwise
     */
    bool
    calculateShapeCenter(const pointDistributionModel::shape &shape,
                         const fvector &pointReliability,
                         fpoint& center) const;

    /**
     * Calculate the mean distance of a given point to the points of a shape
     * @return average distance
     */
    bool 
    calculateShapeDistance(const pointDistributionModel::shape &shape,
                           float& dist,
                           const fpoint &thePoint =
                           fpoint(0.0f, 0.0f)) const;
    
    /**
     * Calculate the mean distance of a given point to the
     * reliability-weighted points of a shape
     */
    bool calculateShapeDistance(const pointDistributionModel::shape &shape,
                                const fvector &pointReliability,
                                float& dist,
                                const fpoint &thePoint =
                                fpoint(0.0f, 0.0f)) const;
    
    /**
     * Calculate the relative orientation difference of two shapes,
     * with regard to a given rotation center.
     *
     * @param shape Shape to be analyzed
     * @param referenceShape Reference shape for the orientation computation
     * @param angle between referenceShape and shape
     * @param theCenter Optional parameter to indicate the orientation center
     *
     * @return true if successful, false otherwise
     */
    bool
    calculateShapeOrientation(const pointDistributionModel::shape &shape,
                           const pointDistributionModel::shape &referenceShape,
                              double& angle,
                              const fpoint &theCenter = 
                              fpoint(0.0f, 0.0f)) const;
    
    /**
     * Calculate the relative orientation difference of two shapes
     * with regard to a given rotation center and given point reliabilities
     */
    bool
    calculateShapeOrientation(const pointDistributionModel::shape &shape,
                              const pointDistributionModel::shape &refShape,
                              const fvector &pointReliability,
                              double& angle,
                              const fpoint &theCenter = 
                              fpoint(0.0f, 0.0f)) const;
    //@}

  protected:

    /**
     * Calculate reliability-weighted scaling factor
     */
    bool
    calculateRelativeShapeScale(const pointDistributionModel::shape& shape,
                                const fpoint& shapeCenter,
                                const pointDistributionModel::shape& refShape,
                                const fpoint& referenceShapeCenter,
                                const fvector& pointReliability,
                                float& scale) const;

    /*
     * Calculate normal direction to the segment line between the two 
     * given points.  The result is normalized in length.
     *
     * Following the points in clockwise direction, the normal will point
     * inwards the shape
     */
    inline fpoint calculateNormal(const fpoint &a,const fpoint &b) const;

    /**
     * Calculate angle between a point and the x-axis
     */
    inline double calculateAngle(const fpoint &thePoint) const;

    /**
     * Convert the shape [(x1,y1),...] to a vector [x1, y1, x2, y2, ...]
     */
    void buildVector(const pointDistributionModel::shape &src,
                     dvector& destVector) const;

    /**
     * Convert a vector [x1, y1, x2, y2, ...] into a shape [(x1,y1),...]
     */
    void buildShape(const dvector &src,
                    pointDistributionModel::shape& destShape) const;

    /*
     * Return normalized vector
     */
    fpoint getNormalizedVector(const fpoint &vec) const;

    /**
     * Create normals for each point.
     *
     * The normal is computed perpendicular to the segment between the two
     * neighbour points, assuming that the shape is closed.
     */
    void createVectorOfNormals(const pointDistributionModel::shape &theShape,
                               vector<fpoint>& normals) const;

    /**
     * Linear least squares instance used for shape alignment
     */
    linearLeastSquares lls_;

  private:
    /**
     * This method is for debugging purposes of the class
     *
     * Usually does absolutelly nothing, but if debugging flags are
     * activated, it shows a window with the shaped painted on it.
     *
     * Each point is multiplied by the scale, and after that the offset is
     * added.
     */
    void show(const pointDistributionModel::shape &theShape,
              const float scale,
              const ipoint offset) const;

    /**
     * This method is for debugging purposes of the class
     *
     * Usually does absolutelly nothing, but if debugging flags are
     * activated, it shows a window with the shaped painted on it.
     *
     * Each point is multiplied by the scale, and after that the offset is
     * added.
     */
    void show(const pointDistributionModel::shape &refShape,
              const pointDistributionModel::shape &theShape,
              const float scale,
              const ipoint offset) const;

  };
}

#endif
