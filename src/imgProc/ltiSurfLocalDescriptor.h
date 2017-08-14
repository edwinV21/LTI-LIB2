/*
 * Copyright (C) 2007
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
 * \file   ltiSurfLocalDescriptor.h
 *         Contains the class lti::surfLocalDescriptor, 
 *         which computes local descriptors with the SURF approach.
 * \author Pablo Alvarado
 * \date   12.11.2007
 *
 * revisions ..: $Id: ltiSurfLocalDescriptor.h,v 1.1 2007-11-24 16:46:25 alvarado Exp $
 */

#ifndef _LTI_SURF_LOCAL_DESCRIPTOR_H_
#define _LTI_SURF_LOCAL_DESCRIPTOR_H_

#include "ltiChannel.h"
#include "ltiLocation.h"
#include "ltiChannel8.h"
#include "ltiFunctor.h"
#include "ltiList.h"
#include "ltiBoundaryType.h"

namespace lti {

  /**
   * Class surfLocalDescriptor
   *
   * This class computes the SURF descriptor for given image locations, as
   * explained in detail in the paper:
   *
   * Herbert Bay, Tinne Tuytelaars, and Luc Van Gool.  SURF: Speeded Up Robust
   * Features.  In Proceedings of the 9th European Conference on Computer
   * Vision, May 2006.  Available at
   * http://www.vision.ee.ethz.ch/~surf/papers.html
   *
   * The implementation is based on the interpretation of the paper, and
   * produce different results than the ones obtained from the closed-source
   * implementation of the authors.  Any suggestions are always welcome.
   *
   * @see surfLocalDescriptor::parameters.
   *
   * @ingroup gFeatureExtr
   */
  class surfLocalDescriptor : public functor {
  public:
    /**
     * Enumeration of the modes for the orientation considerations in the
     * descriptor computation.
     */
    enum eOrientationMode {
      Ignore,  /**< Ignore the orientation.  This method is fast, but
                *   the locations have to occur on the same direction.
                */
      Approximate, /**< The regions are never rotated, but the individual
                    *   responses of the Haar filters will be considered as
                    *   approximations of the oriented gaussian derivative
                    *   which is steerable.  The computation is fast but 
                    *   not as reliable.
                    */
      Cluster  /**< The location orientations will be discretized so that
                *   just a finite number of rotations need to be computed.
                */
    };

    /**
     * The parameters for the class surfLocalDescriptor
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
       * Mode for consideration of the orientation in the descriptor
       * computation.
       *
       * Depending on the speed requirements vs. precision, which at the same
       * time depend on the nature of the images, several modes are provided
       * for the descriptor computation, with respect to the consideration of
       * the orientation stored in the location.  
       *
       * @see surfLocationDescriptor::eOrientationMode
       *
       * Default value: Approximate
       */
      eOrientationMode orientationMode;

      /**
       * Number of clusters.
       *
       * If the orientation mode is set to Cluster, then this is the number
       * of orientations being used.
       *
       * Default value: 12
       */
      int orientationClusters;

      /**
       * Size of the wavelet employed to compute the descriptors.  This value
       * will be multiplied by location radius to produce de correct wavelet
       * size.
       *
       * Default value: 2
       */
      int waveletSize;

      /**
       * If true, the length of the descriptor will be multiplied by four,
       * since the statistics are split in terms of the signs of the wavelet
       * responses.  If you set to true with the other parameters untouched,
       * then the SURF-128 features described in the original paper will be
       * generated.
       *
       * Default value: false;
       */
      bool signSplit;

      /**
       * Std. Deviation of the gaussian function for weights.
       *
       * The weights of the wavelet responses are computed with a gaussian that
       * reduces the effects of elements far away from the location center.
       * This is the standard deviation used for the gaussian function.
       *
       * Default value: 3.5
       */
      float gaussianWeight;

      /**
       * Number of subregions per dimension to be considered. 
       *
       * For example, if you specify 4, then there will be 4x4=16 regions, each
       * one represented in the descriptor by several values.
       *
       * Default value: 4
       */
      int numberOfSubregions;

      /**
       * Number of samples per dimension considered in each subregion.  The
       * step between samples is always s, i.e. the radius of the location (or
       * the scale where the location has been detected).
       *
       * Note that the size of the complete described window is given by this
       * value multiplied by the numberOfSubregions.
       *
       * Default value: 5
       */
      int subregionSamples;

      /**
       * Normalize descriptor length.
       *
       * If set to \c true, the length of the vector will be normalized to 1.
       * If set to \c false, the descriptors will be intensity variant.
       *
       * Default value: true
       */
      bool normalize;

      /**
       * Boundary type
       *
       * If part of a location falls outside the image, how this part should be
       * interpreted.  (Note however, that the center of each location MUST lie
       * within the image boundaries.
       *
       * Default value: Constant
       */
      eBoundaryType boundaryType;
    };

    /**
     * Default constructor
     */
    surfLocalDescriptor();

    /**
     * Construct a functor using the given parameters
     */
    surfLocalDescriptor(const parameters& par);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    surfLocalDescriptor(const surfLocalDescriptor& other);

    /**
     * Destructor
     */
    virtual ~surfLocalDescriptor();

    /**
     * Operates on the given argument.
     *
     * @param src channel8 with the image to be described.
     * @param locs list of locations to be described
     * @param desc resulting descritors computed for each location.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel8& src,
               const list<location>& locs,
               std::list<dvector>& desc) const;

    /**
     * Operates on the given argument.
     *
     * @param src channel8 with the image to be described.
     * @param locs list of locations to be described
     * @param desc resulting descritors computed for each location.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel& src,
               const list<location>& locs,
               std::list<dvector>& desc) const;

    /**
     * Operates on the given argument.
     *
     * @param src channel8 with the image to be described.
     * @param loc the locations to be described
     * @param desc resulting descritor computed for the given location.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel& src,
               const location& loc,
               dvector& desc) const;

    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    surfLocalDescriptor& copy(const surfLocalDescriptor& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    surfLocalDescriptor& operator=(const surfLocalDescriptor& other);

    /**
     * Returns the complete name of the functor class
     */
    virtual const std::string& name() const;

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual surfLocalDescriptor* clone() const;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual surfLocalDescriptor* newInstance() const;

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * Update the parameters
     */
    bool updateParameters();

  private:
    /**
     * The dispatcherreal computation is done here
     */
    template<typename T>
    bool helper(const matrix<T>& src,
                const list<location>& locs,
                std::list<dvector>& desc) const;


    /**
     * The real computation is done here, with an approximation of the
     * rotation.
     */
    template<typename T>
    bool helperApprox(const matrix<T>& src,
                      const list<location>& locs,
                      std::list<dvector>& desc) const;


    /**
     * The real computation is done here, making a few rotations
     */
    template<typename T>
    bool helperCluster(const matrix<T>& src,
                       const list<location>& locs,
                       std::list<dvector>& desc) const;

    /**
     * The real computation is done here, ignoring the rotation
     */
    template<typename T>
    bool helperIgnore(const matrix<T>& src,
                      const list<location>& locs,
                      std::list<dvector>& desc) const;

    /**
     * Compute the shortest angular distance between to angles
     *
     * All angles have to be given in radians.
     */
    inline float adist(const float a,const float b) const;

    /**
     * Signed angular distance
     *
     * All angles have to be given in radians.
     *
     * If the angle a is after the angle b in the counter-clockwise direction 
     * then the result is positive, otherwise it is negative.  The absolute
     * value is always less than pi.
     */
    inline float sadist(const float a,const float b) const;


    /**
     * Compute mean and variance of cluster
     */
    void stats(const fvector& theSin,
               const fvector& theCos,
               const ivector& theLocs,
               const int numLocs,
               float& meanSin,
               float& meanCos,
               float& varSin,
               float& varCos) const;

    /**
     * K-Means clustering of orientations
     *
     * This method splits the location list in several lists that share
     * the same locations.  It uses the k-means algorithm to sort them out.
     *
     * @param locs The original list of locations
     * @param refs The locs list converted to a vector for random access.
     * @param clusters Each row of this matrix corresponds to an orientation
     *                 cluster, containing the index of the location in the
     *                 refs vector.
     * @param angles The centroid orientation of each cluster
     * @param sines The sine of the angle for each cluster
     * @param cosine The cosine of the angle for each cluster
     * @param numLocs The number of locations in each cluster (may be zero)
     */
    bool cluster(const list<location>& locs,
                 std::vector<location>& refs,
                 imatrix& clusters,
                 fvector& angles,
                 fvector& sines,
                 fvector& cosines,
                 ivector& numLocs) const;
                 

    /**
     * Gaussian weights
     */
    fmatrix gaussian_;

    /**
     * Some really private classes
     */
    class block;
    class block4;
    class block8;
  };

  /**
   * Read a surfLocalDescriptor::eOrientationMode value
   *
   * @ingroup gStorable
   */
  bool read(ioHandler& handler,
            surfLocalDescriptor::eOrientationMode& data);

  /**
   * Write a surfLocalDescriptor::eOrientationMode value
   *
   * @ingroup gStorable
   */
  bool write(ioHandler& handler,
             const surfLocalDescriptor::eOrientationMode& data);
  

}

#endif

