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
 * \file   ltiLocationMatch.h
 *         Contains the class lti::locationMatch, 
 *         which has still to be documented by Pablo Alvarado
 * 
 * \author Pablo Alvarado
 * \date   08.01.2008
 *
 * revisions ..: $Id: ltiLocationMatch.h,v 1.2 2008-01-16 21:52:27 alvarado Exp $
 */

#ifndef _LTI_LOCATION_MATCH_H_
#define _LTI_LOCATION_MATCH_H_

#include "ltiLocation.h"
#include "ltiVector.h"
#include "ltiMatrix.h"
#include "ltiList.h"

#include "ltiFunctor.h"

namespace lti {

  /**
   * Class locationMatch
   *
   * This class is used to match the locations on two images, based on the
   * descriptors associated to each location.
   *
   * This class receives as input the output to any feature extraction functor
   * (e.g. lti::surfLocalDescriptor).
   *
   * @see locationMatch::parameters.
   *
   * @ingroup gLocalFeatures
   */
  class locationMatch : public functor {
  public:
    /**
     * Types of algorithms to perform the match.
     */
    enum eMatchType {
      KdTreeBased, /*< Use a kd-Tree data structures to accelerate the
                       search */
      LinearSearch /*< Perform a linear search */
    };

    /**
     * The parameters for the class locationMatch
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
       * Bidirectional match.
       *
       * The match process can be unidirectional, in which the closest
       * descriptor in the second image is searched for each descriptor in the
       * first image (set this attribute to \c false).  It can also be
       * bidirectional, which means that a match is found only if the
       * descriptor of the second image is the closest to the one of the first
       * image and the descriptor in the first image is the closest to the one
       * in the second image.
       *
       * Note that the bidirectional is slower, but more robust.
       *
       * Default value: true
       */
      bool bidirectional;

      /**
       * Threshold distance.
       *
       * The distance between two descriptors must be less than this threshold.
       * This means, the higher the given value, the greater the number of
       * matches detected.  For lower values, just the most similar locations
       * are detected.
       *
       * Note that the threshold is an absolute value, whose range will depend
       * on the distances between points found by the other classes.
       *
       * Default value: 1.0
       */
      double threshold;

      /**
       * Algorithm used for the match.
       *
       * You can choose between several algorithms to perform the match.  The
       * straightforward method is based on a linear search, while more
       * elaborate methods use a kd-tree.  Use the second if the set of
       * locations is too large size.
       *
       * Default value: LinearSearch
       */
      eMatchType matchType;

      /**
       * Bucket size
       *
       * If the match method is set to KdTreeBased then the trees build will
       * use "buckets" of the given size, which means that at each leaf, the
       * given number of elements are stored, implying linear search for that
       * size.  The experience tells that setting this value to 1 (a pure tree)
       * is not very efficient, since the time to create the tree will simply
       * be too high.  The maximum efficiency is achieved usually with buckets
       * of 16 to 32 elements.
       *
       * Default value: 32
       */
      int bucketSize;

    };

    /**
     * Default constructor
     */
    locationMatch();

    /**
     * Construct a functor using the given parameters
     */
    locationMatch(const parameters& par);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    locationMatch(const locationMatch& other);

    /**
     * Destructor
     */
    virtual ~locationMatch();

    /**
     * Find the matches between the locations and descriptors of two different
     * images.
     *
     * This method finds the matching locations in two images A and B.  
     *
     * @param locsA locations of the image A
     * @param vctsA descriptors for the locations of image A.
     * @param locsB locations of the image B
     * @param vctsB descriptors for the locations of image B.
     * @param matchedLocsA locations for which a match in image B has been 
     *                     found.
     * @param descA descriptors corresponding to the matchedLocsA
     * @param matchedLocsB locations for which a match in image A has been 
     *                     found.
     * @param descB descriptors corresponding to the matchedLocsB
     *
     * @return true if apply successful or false otherwise.
     */
    bool apply(const list<location>& locsA,
               const std::list<dvector>& vctsA,
               const list<location>& locsB,
               const std::list<dvector>& vctsB,
               std::vector<location>& matchedLocsA,
               std::vector<dvector>& descA,
               std::vector<location>& matchedLocsB,
               std::vector<dvector>& descB);


    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    locationMatch& copy(const locationMatch& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    locationMatch& operator=(const locationMatch& other);

    /**
     * Returns the complete name of the functor class
     */
    virtual const std::string& name() const;

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual locationMatch* clone() const;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual locationMatch* newInstance() const;

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * Linear search
     */
    bool linearSearch(const list<location>& locsA,
                      const std::list<dvector>& vctsA,
                      const list<location>& locsB,
                      const std::list<dvector>& vctsB,
                      std::vector<location>& matchedLocsA,
                      std::vector<dvector>& descA,
                      std::vector<location>& matchedLocsB,
                      std::vector<dvector>& descB);

    /**
     * Search with kd-trees
     */
    bool kdtree(const list<location>& locsA,
                const std::list<dvector>& vctsA,
                const list<location>& locsB,
                const std::list<dvector>& vctsB,
                std::vector<location>& matchedLocsA,
                std::vector<dvector>& descA,
                std::vector<location>& matchedLocsB,
                std::vector<dvector>& descB);

    /**
     * Convert the two locations/vectors 
     */
    template<class T,class L>
    void convert(const L& theList,
                 std::vector<T>& theVct) const;

  };

  
  /**
   * Read a surfLocalDescriptor::eOrientationMode value
   *
   * @ingroup gStorable
   */
  bool read(ioHandler& handler,
            locationMatch::eMatchType& data);

  /**
   * Write a surfLocalDescriptor::eOrientationMode value
   *
   * @ingroup gStorable
   */
  bool write(ioHandler& handler,
             const locationMatch::eMatchType& data);
  


}

#endif

