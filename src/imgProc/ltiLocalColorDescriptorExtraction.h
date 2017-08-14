/*
 * Copyright (C) 1998-2005
 * Lehrstuhl fuer Technische Informatik, RWTH-Aachen, Germany
 *
 * This file is part of the LTI-Computer Vision Library (LTI-Lib)
 *
 * The LTI-Lib is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License (LGPL)
 * as published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * The LTI-Lib is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with the LTI-Lib; see the file LICENSE.  If
 * not, write to the Free Software Foundation, Inc., 59 Temple Place -
 * Suite 330, Boston, MA 02111-1307, USA.
 */

/** 
 * \file   ltiLocalColorDescriptorExtraction.h
 *         Contains the class lti::localColorDescriptorExtraction.
 * \author Arnd Hannemann
 * \date   10.12.2004
 *
 * revisions ..: $Id: ltiLocalColorDescriptorExtraction.h,v 1.1 2009-08-30 04:30:05 alvarado Exp $
 */

#ifndef _LTI_LOCAL_COLOR_DESCRIPTOR_EXTRACTION_H_
#define _LTI_LOCAL_COLOR_DESCRIPTOR_EXTRACTION_H_


#include "ltiLocalSampling.h"
#include "ltiLocalDescriptorExtraction.h"

namespace lti {

  /**
   * This functor calculates color-descriptors for each location in a
   * given list.
   * 
   * In default behavior lti::sliceSampling is used to sample the regions
   * around a location.
   * Each location will be considered as a circular region, that will
   * be divided in a specified number of slices.  The color feature will
   * compute for each average color. For each slice the mean will be computed.
   *
   * It is also possible to compute overlapping slices, to make the feature
   * more robust against noise.
   *
   * You can compute the locations with the functor lti::axLocationSearch or
   * with lti::pyramidLocationSearch.
   *
   * Examplecode:
   * \code
   *  // load an image
   *  loadPNG loader;
   *  image img;
   *  loader.load("hough1.png",img);
   * 
   *  // acquire locations
   *  location loc(ipoint(114,114),0,10);
   *
   *  // create sampling parameters instance
   *  sliceSampling::parameters slicePar;
   *  
   *  // change sampling parameters
   *  slicePar.slices = 2;
   *
   *  // create descriptor extraction parameters
   *  localColorDescriptorExtraction::parameters lcdePar;
   *
   *  // set sampling and sampling parameters
   *  lcdePar.setSampling("lti::sliceSampling",slicePar);
   *
   *  // create descriptor extraction
   *  localColorDescriptorExtraction lcde(lcdePar);
   *
   *  // apply it 
   *  dvector dest;
   *  lcde.apply(img,loc,dest);
   *
   * \endcode
   * 
   * @see localColorDescriptorExtraction::parameters.
   * @see sliceSampling
   * @see meanAccumulator
   * @see varianceAccumulator
   *
   * @ingroup gLocalDescriptorExtraction
   */
  class localColorDescriptorExtraction : public localDescriptorExtraction {
  public:
    /**
     * Enumeration of accumulator types
     */
    enum eAccuType {
      Mean,           /**< Mean only         */
      MeanAndVariance /**< Mean and Variance */
    };

    /**
     * The parameters for the class localColorDescriptorExtraction
     */
    class parameters : public localDescriptorExtraction::parameters {
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
       * Returns the name of this type.
       */
      virtual const std::string& name() const;

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
      // the parameters
      // ------------------------------------------------
      /**
       * which accumulator is used to compute descriptors
       * 
       * Default: Mean
       */
      eAccuType accuType;

    };

    /**
     * Default constructor
     */
    localColorDescriptorExtraction();

    /**
     * Construct a functor using the given parameters
     */
    localColorDescriptorExtraction(const parameters& par);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    localColorDescriptorExtraction(const localColorDescriptorExtraction& other);

    /**
     * Destructor
     */
    virtual ~localColorDescriptorExtraction();

    /**
     * Returns the name of this type.
     */
    virtual const std::string& name() const;
    
    /**
     * Operates on a copy of the given arguments.
     *
     * @param src image with source data.
     * @param loc location that need to be analyzed.
     * @param dest descriptor vector.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const matrix<rgbaPixel>& src,
               const location& loc,
               dvector& dest) const;

    /**
     * Operates on a copy of the given arguments.
     *
     * @param src image with source data.
     * @param dest descriptor vector.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const matrix<rgbaPixel>& src,
               dvector& dest) const;

    /**
     * Compute the descriptor for each location.
     *
     * @param src original image to be analyzed.
     * @param locs locations that need to be analyzed.
     * @param dest list of descriptor vectors.  This list will have the
     *             same size as locs.
     * @return true if successful, or false otherwise.  */
    bool apply(const matrix<rgbaPixel>& src,
               const list<location>& locs,
               std::list<dvector>& dest) const;

    /**
     * Operates on a copy of the given arguments.
     *
     * @param src channel with source data.
     * @param loc location that need to be analyzed.
     * @param dest descriptor vector.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const matrix<float>& src,
               const location& loc,
               dvector& dest) const;

    /**
     * Operates on a copy of the given arguments.
     *
     * @param src channel with source data.
     * @param dest descriptor vector.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const matrix<float>& src,
               dvector& dest) const;

    /**
     * Compute the descriptor for each location.
     *
     * @param src original channel to be analyzed.
     * @param locs locations that need to be analyzed.
     * @param dest list of descriptor vectors.  This list will have the
     *             same size as locs.
     * @return true if successful, or false otherwise.  */
    bool apply(const matrix<float>& src,
               const list<location>& locs,
               std::list<dvector>& dest) const;
    
    /**
     * Compute the local descriptor for each location.
     * The standard implementation casts the channel8 to a channel
     * and invokes the apply for channel.
     *
     * @param src original channel8 to be analyzed.
     * @param locs locations that need to be analyzed.
     * @param dest list of descriptor vectors.  
     * @return true if successful, or false otherwise.  
     */
    virtual bool apply(const channel8& src,
                       const list<location>& locs,
                       std::list<dvector>& dest) const;

    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    localColorDescriptorExtraction&
    copy(const localColorDescriptorExtraction& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    localColorDescriptorExtraction&
    operator=(const localColorDescriptorExtraction& other);

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual localColorDescriptorExtraction* clone() const;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual localColorDescriptorExtraction* newInstance() const;

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;

  protected:
    /** mean accumulator for float */
    //meanAccumulator<float> fMeanAcc_;
    
    /** mean accumulaotr for rgbaPixel */
    //    meanAccumulator<rgbaPixel> rMeanAcc_;


  };
}

#endif

