/*
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
 * \file   ltiDistanceTransform.h
 *         Contains the class lti::distanceTransform, 
 *         which has still to be documented by Pablo Alvarado
 * 
 * \author Pablo Alvarado
 * \date   09.03.2008
 *
 * revisions ..: $Id: ltiDistanceTransform.h,v 1.1 2008-03-09 15:23:53 alvarado Exp $
 */

#ifndef _LTI_DISTANCE_TRANSFORM_H_
#define _LTI_DISTANCE_TRANSFORM_H_

#include "ltiMatrix.h"
#include "ltiVector.h"

#include "ltiFunctor.h"

namespace lti {

  /**
   * Class distanceTransform
   *
   * This simple morphological operator assumes that the input data is
   * a binary image, i.e. there are values 0 and not 0.  It computes the
   * minimal distance of a pixel with not-zero value to a zero pixel.
   *
   * You can choose in the parameters between a 4-Neighborhood or an
   * 8-Neighborhood for the computations.  It is also possible to
   * compute the euclidean distance transform.
   *
   * For two pixels p and q with position p(p.x,p.y) and
   * q(q.x,q.y) following is valid:
   * - the distance in a 4-neighborhood is |p.x-q.x| + |p.y-q.y|
   * - the distance in an 8-neighborhood is max(|p.x-q.x|,|p.y-q.y|)
   *
   * The computation for the 4- and 8-neighborhood based distance
   * transform is very efficient and traverses the input channel just
   * twice: once from top to bottom and once on the opposite
   * direction.  To compute the euclidean distance transform the
   * algorithm described in
   *
   * Calvin R. Maurer Jr., Rensheng Qi, Vijay V. Raghavan: 
   * "A Linear Time Algorithm for Computing Exact Euclidean Distance Transforms
   *  of Binary Images in Arbitrary Dimensions". 
   * IEEE Transactions on Pattern Analysis and Machine Intelligence,
   * Vol.25, No. 2, 2003, pp. 265-270
   *
   * is used.
   *
   * @see distanceTransform::parameters.
   *
   * @ingroup gMorphology
   */
  class distanceTransform : public functor {
  public:

    /**
     * Four distancetransformation computation types can be choosen:  
     * EightNeighbor, FourNeighbor, EuclideanSqr and Euclidean.
     *
     * For two pixels p and q with position p(p.x,p.y) and
     * q(q.x,q.y) following is valid:
     * - the distance in a 4-neighborhood is |p.x-q.x| + |p.y-q.y|
     * - the distance in an 8-neighborhood is max(|p.x-q.x|,|p.y-q.y|)
     * - the distance is euclidean square for (p.x - q.x)^2 + (p.y - q.y)^2 
     * - the distance is euclidean for ( (p.x - q.x)^2 + (p.y - q.y)^2 )^1/2
     */
    enum eDistanceType {
      EightNeighborhood, /**< eight neighborhood distance */
      FourNeighborhood,  /**< four neighborhood (city block) distance */
      EuclideanSqr,      /**< square of euclidean distance */
      Euclidean,         /**< euclidean distance */
      EightSED,          /**< eight point sequential euclidian
                          *   distance mapping 
                          */
      EightSEDSqr,       /**< square of the eight point sequential
                          *   euclidian distance mapping 
                          */
      FourSED,           /**< four point sequential euclidian
                          *   distance mapping 
                          */
      FourSEDSqr         /**< square of the four point sequential
                          *   euclidian distance mapping 
                          */
    };


    /**
     * The parameters for the class distanceTransform
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
       * Returns the complete name of the parameters class.
       */
      virtual const std::string& name() const;

      /**
       * Returns a pointer to a clone of the parameters.
       */
      virtual parameters* clone() const;

      /**
       * Returns a pointer to a new instance of the parameters.
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
       * Kind of distance transform to be computed.  See eDistanceType
       * for more information.
       *
       * Default value: Euclidean
       */
      eDistanceType distance;

    };

    /**
     * Default constructor
     */
    distanceTransform();

    /**
     * Construct a functor using the given parameters
     */
    distanceTransform(const parameters& par);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    distanceTransform(const distanceTransform& other);

    /**
     * Destructor
     */
    virtual ~distanceTransform();

    /**
     * Compute the distance transform of the srcdest channel, i.e. the
     * minimal distance from a non-background pixel to a background pixel.
     * It will be assumed, that all background pixels have the value zero.
     * (any non zero value will be taken as non-background pixel).
     *
     * If the size of \a srcdest and form of its non-background
     * regions allow distances greater than 255 (the maximal possible
     * representable value with a ubyte), the content of the distance
     * transform will be invalid.
     * 
     * @param srcdest matrix with the source data.  The result
     *                will be left here too.
     * @return true if successful, false otherwise.
     */
    bool apply(matrix<ubyte>& srcdest) const;

    /**
     * Compute the distance transform of the srcdest channel, i.e. the
     * minimal distance from a non-background pixel to a background pixel.
     * It will be assumed, that all background pixels have the value 0.0f.
     * (any non zero value will be taken as non-background pixel).
     *
     * The result in the channel will contain for each pixel the
     * distance value.  This means, the usual value range from 0.0f to 1.0f 
     * is ignored in this functor, providing the distances in "pixel" units.
     * 
     * @param srcdest channel with the source data.  The result
     *                 will be left here too.
     * @return true if successful, false otherwise.
     */
    bool apply(fmatrix& srcdest) const;

    /**
     * Compute the distance transform of the \a src channel and leave
     * the result in \a dest, i.e. calculate the minimal distance from
     * a non-background pixel to a background pixel.  It will be
     * assumed, that all background pixels have the value zero.  (any
     * non zero value will be taken as non-background pixel).
     *
     * If the size of \a srcdest and form of its non-background
     * regions allow distances greater than 255 (the maximal possible
     * representable value with a ubyte), the content of the distance
     * transform will be invalid.
     * 
     * @param src matrix<ubyte> with the source data.
     * @param dest matrix<ubyte> where the result will be left.
     * @return true if successful, false otherwise.
     */
    bool apply(const matrix<ubyte>& src, matrix<ubyte>& dest) const;

    /**
     * Compute the distance transform of the \a src fmatrix and leave
     * the result in \a dest, i.e. calculate the minimal distance from
     * a non-background pixel to a background pixel.  It will be
     * assumed, that all background pixels have the value zero.  (any
     * non zero value will be taken as non-background pixel).
     *
     * The result in the fmatrix will contain for each pixel the
     * distance value.  This means, the usual value range from 0.0f to 1.0f 
     * is ignored in this functor, providing the distances in "pixel" units.
     * 
     * @param src fmatrix with the source data.
     * @param dest fmatrix where the result will be left.
     * @return true if successful, false otherwise.
     */
    bool apply(const fmatrix& src, fmatrix& dest) const;


    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    distanceTransform& copy(const distanceTransform& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    distanceTransform& operator=(const distanceTransform& other);

    /**
     * Returns the complete name of the functor class
     */
    virtual const std::string& name() const;

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual distanceTransform* clone() const;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual distanceTransform* newInstance() const;

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * Iteration for distance computation using an 8-Neighborhood
     */
    void iteration8(fmatrix& chnl) const;

    /**
     * Iteration for distance computation using an 4-Neighborhood
     */
    void iteration4(fmatrix& chnl) const;

    /**
     * Backwards iteration for distance computation using an 8-Neighborhood
     */
    void iteration8back(fmatrix& chnl) const;

    /**
     * Backwards iteration for distance computation using an 4-Neighborhood
     */
    void iteration4back(fmatrix& chnl) const;
    
    /**
     * Method to compute ED for each column of the precomputed
     * distance image chnl. The chnl must be the output of a previos
     * call to EDT_1D.  Method uses the distance propagation of the
     * function voronoiEDT_2D.
     */
    inline void EDT_2D(fmatrix& chnl) const;
        
    /**
     * Queries some distance information for the EDT
     */
    inline bool removeEDT(const int du,
                          const int dv, 
                          const int dw,
                          const int u,
                          const int v,
                          const int w) const;
        
    /**
     * Fast linear order computation of the accurade euclidean
     * distance with voronoi diagrams
     */
    void voronoiEDT_2D(fmatrix& chnl, const int dim) const;
 
    /**
     * Method computes ED (euclidean distance) for each row pixel of
     * the given fmatrix to the closest background pixel. It assumes
     * that a background pixel is set to 0.0f and a foreground pixel is
     * > 0.0f. All rows without any background pixel will be set to an
     * undefined distance (i.e. < 0.0f). The ED is computed with a fast
     * and easy forward-backward distance propagation.
     */
    void EDT_1D(fmatrix& chnl) const;

    /**
     * Method computes ED (euclidean distance) for the given fmatrix
     * with the 8SED or 4SED (8 or 4 point sequential euclidian
     * distance mapping) method.
     */
    void sedFiltering(fmatrix &chnl, bool useEightSED) const;

    /**
     * Calculates the 4SED distance transform.
     */
    void fourSEDFiltering(fmatrix &chnl, matrix<ipoint> &dist) const;

    /**
     * Calculates the 8SED distance transform.
     */
    void eightSEDFiltering(fmatrix &chnl, matrix<ipoint> &dist) const;

  private:

    /**
     * Nested class for the SED_filtereing method.
     */
    class sedMask {
    public:
    
      /**
       * Inline Constructor
       *
       * @param mask a list of points of the mask positions
       * @param size the number of points mask contains
       */
      sedMask(const ipoint mask[], int size);
      
      /**
       * Inline Destructor
       */
      ~sedMask();
      
      /**
       * Filters the positoins in dist with the internal mask.
       *
       * @param pos the position in dist of the filter process
       */
      void filter(matrix<ipoint> &dist, const ipoint &pos) const;
      
    private:
      
      /**
       * After called shortest contains the value of the shorter
       * (euclidean) of the two given points. If any point contain
       * negative values its lenght is not calculated.
       */
      inline void queryDistance(ipoint &shortest, ipoint &other) const;
      
      /**
       * List to points of the filtermask.
       *
       * Just a reference to extern data
       */
      const ipoint *const mask_;
      
      /**
       * Number of point stored in mask
       */
      int size_;
    };
  };

  /**
   * Read a distanceTransform::eDistanceType
   *
   * @ingroup gStorable
   */
  bool read(ioHandler& handler,
            distanceTransform::eDistanceType& data);

  /**
   * Write a distanceTransform::eDistanceType
   *
   * @ingroup gStorable
   */
  bool write(ioHandler& handler,
             const distanceTransform::eDistanceType& data);
  

  
}

#endif

