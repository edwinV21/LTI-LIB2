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
 * \file   ltiAreaDescriptor.h
 *         Contains the class lti::areaDescriptor, 
 *         which is a data structure to hold a simple descriptor of 
 *         a group of pixels, mostly centered on the area covered by those
 *         pixels.
 * \author Pablo Alvarado
 * \date   18.10.2009
 *
 * revisions ..: $Id: ltiAreaDescriptor.h,v 1.3 2009-11-11 03:34:17 alvarado Exp $
 */

#ifndef _LTI_AREA_DESCRIPTOR_H_
#define _LTI_AREA_DESCRIPTOR_H_

#include "ltiIoObject.h"
#include "ltiPoint.h"
#include "ltiRectangle.h"

namespace lti {

  /**
   * Class areaDescriptor
   *
   * Data structure to hold a simple descriptor of a group of pixels, mostly
   * centered on the area covered by those pixels.
   *
   * This data structure simplifies the computation of the bounding box,
   * the area and the center of gravity (cog) of a group of
   * pixels.
   *
   * It is used by several classes, most prominently by the
   * lti::fastAreaDescription class, which is related to the lti::fastRelabeling
   * class.
   *
   * @see lti::fastAreaDescription
   *
   * @ingroup gRegionAnalysis
   */
  class areaDescriptor : public ioObject {
  public:
    /**
     * Default constructor
     */
    areaDescriptor();

    /**
     * Copy constructor
     * @param other the areaDescriptor object to be copied
     */
    areaDescriptor(const areaDescriptor& other);
    
    /**
     * Destructor
     */
    ~areaDescriptor();
    
    /**
     * Copy the contents of a areaDescriptor object
     * @param other the areaDescriptor object to be copied
     * @return a reference to this areaDescriptor object
     */
    areaDescriptor& copy(const areaDescriptor& other);
    
    /**
     * Copy the contents of a areaDescriptor object
     * @param other the areaDescriptor object to be copied
     * @return a reference to this areaDescriptor object
     */
    areaDescriptor& operator=(const areaDescriptor& other);
    
    /**
     * Returns the complete name of the areaDescriptor class.
     */
    virtual const std::string& name() const;
    
    /**
     * Returns a pointer to a clone of the areaDescriptor.
     */
    virtual areaDescriptor* clone() const;
    
    /**
     * Returns a pointer to a new instance of the areaDescriptor.
     */
    virtual areaDescriptor* newInstance() const;
    
    /**
     * Write the areaDescriptor in the given ioHandler
     * @param handler the ioHandler to be used
     * @param complete if true (the default) the enclosing begin/end will
     *        be also written, otherwise only the data block will be written.
     * @return true if write was successful
     */
    virtual bool write(ioHandler& handler,const bool complete=true) const;
    
    /**
     * Read the areaDescriptor from the given ioHandler
     * @param handler the ioHandler to be used
     * @param complete if true (the default) the enclosing begin/end will
     *        be also written, otherwise only the data block will be written.
     * @return true if write was successful
     */
    virtual bool read(ioHandler& handler,const bool complete=true);
    

    /**
     * @name Methods to update the data
     */
    //@{

    /**
     * Set all values to zero, so that you can start considering new points
     */
    void reset();

    /**
     * Consider another point into the descriptor
     */
    void consider(const ipoint& p);

    /**
     * Consider a point into the descriptor
     */
    void consider(const int x,const int y);

    /**
     * Consider the other descriptor points, assuming this and the other
     * descriptor have both not been normalized yet.  The resulting COG will
     * not be normalized.
     */
    void considerUnnormalized(const areaDescriptor& other);

    /**
     * Consider the other descriptor points, assuming this and the other
     * descriptor have both already been normalized.  The resulting COG will be
     * normalized as well.
     */
    void considerNormalized(const areaDescriptor& other);

    /**
     * Alias for considerUnnormalized.
     *
     * Returns a reference to this
     */
    areaDescriptor& join(const areaDescriptor& other);

    /**
     * Compute the value of cog.
     *
     * The attribute cog accumulates all coordinates given by the consider() 
     * methods.  It is until you call this method that this attribute will
     * honor its name.
     *
     * If you desire to keep considering more points, then you have to
     * call denormalizeCOG() first
     */
    void normalizeCOG();

    /**
     * Denormalize cog attribute to continue with point considerations.
     */
    void denormalizeCOG();

    /**
     * Check for equality
     */
    bool operator==(const areaDescriptor& other) const;

    /**
     * Check for inequality
     */
    bool operator!=(const areaDescriptor& other) const;

    /**
     * Check for closeness
     *
     * Two area descriptors are said to be close to each other if the
     * rectangular bounding box of this descriptor overlaps with the
     * rectangular bounding box of the other descriptor expanded by the given
     * border (in x and in y directions by the respective coordinates).
     */
    bool isClose(const areaDescriptor& other,
                 const ipoint& border=ipoint(0,0)) const;

    /**
     * Bounding Box
     *
     * Compute the bounding box of the points described, optionally extended by
     * the given border.
     */
    irectangle computeBoundingBox(const ipoint& border = ipoint(0,0)) const;
    //@}

    // ------------------------------------------------
    // Attributes
    // ------------------------------------------------
    
    /**
     * Number of pixels considered for the descriptor
     */
    int area;
    
    /**
     * Coordinates of the point with the highest x coordinate so far.
     */
    ipoint maxX;
    
    /**
     * Coordinates of the point with the lowest x coordinate so far.
     */
    ipoint minX;
    
    /**
     * Coordinates of the point with the lowest y coordinate so far.
     */
    ipoint minY;

    /**
     * Coordinates of the point with the highest y coordinate so far.
     */
    ipoint maxY;

    /**
     * Center of Gravity
     *
     * For efficient computation, the cog attribute holds the sum of x and y
     * coordinates so far, until the normalizeCOG() method is called, which
     * normalizes by the area attribute to compute the real center of gravity.
     *
     * If you want to keep considering more points, you can always call
     * denormalizeCOG() to reset this attribute to the sum of x and y values.
     */
    fpoint cog;


  };
}

#endif

