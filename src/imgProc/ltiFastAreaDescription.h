/*
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
 * \file   ltiFastAreaDescription.h
 *         Contains the class lti::fastAreaDescription, 
 *         which efficiently computes area descriptors of labeled masks.
 *
 * \author Dorothee Finck
 * \date   11.6.2003 
 * \author Pablo Alvarado
 * \date   18.10.2009
 *
 * revisions ..: $Id: ltiFastAreaDescription.h,v 1.2 2009-11-11 03:34:17 alvarado Exp $
 */

#ifndef _LTI_FAST_AREA_DESCRIPTION_H_
#define _LTI_FAST_AREA_DESCRIPTION_H_

#include "ltiFastRelabeling.h"
#include "ltiAreaDescriptor.h"
#include "ltiRectangle.h"
#include <vector>

namespace lti {

  /**
   * Class fastAreaDescription
   *
   * This class uses the speed of lti::fastRelabeling to compute some basic
   * area descriptors for the contiguous regions found in a labeled or
   * unlabeled mask.
   *
   * This class computes for each contiguous region the number of pixels of the
   * region, extracts the four pixels delimiting the regions's rectangular
   * bounding box and the center of gravity (cog) of each region.
   *
   * @see fastAreaDescription::parameters.
   *
   * @ingroup gRegionAnalysis
   */
  class fastAreaDescription : public fastRelabeling {
  public:
    /**
     * The parameters for the class fastAreaDescription
     */
    class parameters : public fastRelabeling::parameters {
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
       * The minimum distance in x and y direction between the rectangular
       * bounding boxes of regions that have to be considered as belonging to
       * the same object.
       *
       * Default: (0,0) (i.e. both bounding boxes must overlap)
       */
      ipoint minimumDistance;

      /**
       * If \c true regions are merged into the same object if the regions
       * are closed enough.  Otherwise, the regions are kept appart.
       *
       * Default: false
       */
      bool mergeClose;

      /**
       * If this value is greater than 0 and sortSize is true, only the n
       * largest objects (ignoring the background) will be processed and all
       * other objects are suppressed (i.e. assigned to the background).
       *
       * A value of zero or less means no regions should be ignored.
       *
       * Default: 0
       */
      int nLargest;

      /**
       * If the number of pixels of a region after merging is less than the
       * minimumMergedObjectSize, it will be assigned to the background label
       * (0).
       *
       * The inherited attribute minimumObjectSize applies to objects \b before
       * merging.
       *
       * Default value: 1 (i.e. consider all objects)
       */
      int minimumMergedObjectSize;

    };

    /**
     * Default constructor
     */
    fastAreaDescription();

    /**
     * Construct a functor using the given parameters
     */
    fastAreaDescription(const parameters& par);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    fastAreaDescription(const fastAreaDescription& other);

    /**
     * Destructor
     */
    virtual ~fastAreaDescription();

    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    fastAreaDescription& copy(const fastAreaDescription& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    fastAreaDescription& operator=(const fastAreaDescription& other);

    /**
     * Returns the complete name of the functor class
     */
    virtual const std::string& name() const;

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual fastAreaDescription* clone() const;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual fastAreaDescription* newInstance() const;

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * Compute the bounding boxes of the objects inside the image.
     *
     * The object with index zero always represents the background.
     *
     * @param src matrix<ubyte> with the source data.
     * @param objects a vector with the smallest rectangles containing
     *                each labeled object.  The first rectangle belongs
     *                always to the background.
     * @return true if apply successful or false otherwise.
     */    
    bool apply(const matrix<ubyte>& src,
                     std::vector<irectangle>& objects) const;
 

    /**
     * Compute the bounding boxes of the regions inside the mask.
     *
     * Additionally to the bounding boxes a mask with all the labels is
     * returned, where the label corresponds to the index into the vector with
     * the bounding boxes.
     *
     * The object with index zero always represents the background.
     *
     * @param src matrix<ubyte> with the source data.
     * @param dest imatrix with the labeled objects.
     * @param objects a vector with the smallest rectangles containing
     *                each labeled object.  The first rectangle belongs
     *                always to the background.
     * @return true if apply successful or false otherwise.
     */    
    bool apply(const matrix<ubyte>& src,
               imatrix& dest,
               std::vector<irectangle>& objects) const;
    
    /**
     * Compute area descriptors (bounding box, area, center of gravity) of the
     * regions inside the mask.
     *
     * The object with index zero always represents the background.
     *
     * @param src matrix<ubyte> with the source data.
     * @param objects a vector with the geometric features of each
     *                labeled object.  The first descriptor belongs
     *                always to the background.
     * @return true if apply successful or false otherwise.
     */  
    bool apply(const matrix<ubyte> &src,
               std::vector<areaDescriptor>& objects) const;
    
    /**
     * Compute area descriptors (bounding box, area, center of gravity) of the
     * regions inside the mask.
     *
     * Additionally to the bounding boxes, a mask with all the labels is
     * returned, where the label corresponds to the index of vector with the
     * area descriptor.
     *
     * The object with index zero always represents the background.
     *
     * @param src matrix<ubyte> with the source data.  
     * @param dest imatrix with the labeled objects.
     * @param objects a vector with the geometric features of each labeled
     *        object.  The first descriptor belongs always to the background.
     *
     * @return true if apply successful or false otherwise.
     */  
    bool apply(const matrix<ubyte> &src, imatrix& dest,
	       std::vector<areaDescriptor>& objects) const;

  private:
    /**
     * Computes the vector of equivalent labels, i.e. suppresses objects
     * smaller than the minimum size and those which are not among the
     * n-largest. Furthermore this suppression is more efficient than using the
     * parent class' methods because the labeled mask is not updated yet.
     *
     * @param src matrix<ubyte> with the source data
     * @param dest matrix with the labels
     * @param objSize vector with the object sizes
     * @param equivLabels vector with equivalent labels
     * @param numObjects contains the number of detected objects
     * @return true on success or false otherwise.
     */

    bool computeEquivLabels(const matrix<ubyte>& src, 
                            imatrix& dest,
			    ivector& objSize,
                            ivector& equivLabels,
			    int& numObjects) const;

    /**
     * Initializes the vector of bounding rectangles with the rectangle
     * containing the entire image.
     *
     * @param numLab the number of labels
     * @param size the size of the input channel
     * @param objects the vector to be initializes
     * @return true on success or false otherwise.
     */
    virtual bool initRectangleVector(const int numLab,
                                     const ipoint& size,
				     std::vector<irectangle>& objects) const;

    /**
     * Initializes the vector of area descriptors
     *
     * @param numLab the number of labels
     * @param objects the vector to be initializes
     *
     * @return true on success or false otherwise.
     */
    virtual bool initAreaDescriptors(const int numLab,
                                     std::vector<areaDescriptor>& objects)const;
    
    /**
     * Merge neighboring objects.
     *
     * The class T should be a rectangle or a areaDescription, or any class
     * having the method isClose(T,int) and join(T)
     *
     * @param src the vector with the rectangles to be merged
     * @param objSize vector with the number of pixels of each object in the
     *                src vector
     * @param dest    mask to be relabeled 
     * @return true on success or false otherwise.
     */
    template<class T>
    bool merge(std::vector<T>& src,
               ivector& objSize,
               imatrix& dest) const;

  };
}

#endif

