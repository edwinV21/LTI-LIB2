/*
 * Copyright (C) 1998 - 2005
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
 * \file   ltiLocalDescriptorExtraction.h
 *         Defines lti::localDescriptorExtraction which is the base class
 *         of all functors that extract a local description (region or
 *         location) from an image.
 * \author Arnd Hannemann
 * \date   05.04.2004
 *
 * $Id: ltiLocalDescriptorExtraction.h,v 1.1 2009-08-30 04:30:05 alvarado Exp $
 */


#ifndef _LTI_LOCAL_DESCRIPTOR_EXTRACTION_H_
#define _LTI_LOCAL_DESCRIPTOR_EXTRACTION_H_


#include <list>

#include "ltiImage.h"
#include "ltiChannel.h"
#include "ltiChannel8.h"
#include "ltiLocation.h"
#include "ltiDescriptorExtraction.h"
#include "ltiLocalSampling.h"
#include "ltiList.h"

namespace lti {
  /**
   * Local Descriptor Extraction.
   *
   * The local descriptor extractions are functors which extract information
   * from the given images or channels at the given locations. This class is
   * parent class for all implemented local descriptor extractions.
   *
   * 
   */
  class localDescriptorExtraction : public descriptorExtraction {
  public:
    
    /**
     * The parameters for the class localDescriptorExtraction.
     */
    class parameters : public descriptorExtraction::parameters {
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
       * Returns a pointer to a new instance of this functor.
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
       * Set the sampling class used to extract region out of the input
       * data.  For the sampling instance, its default parameters will be used.
       *
       * @param samplingName name of the sampling to use
       * @return true if instantiation of the sampling functor was successful
       */
      virtual bool setSampling(const std::string& samplingName);
   
      /**
       * Set the sampling class used to extract region out of the input data,
       * and its parameters too.
       *
       * @param samplingName name of the sampling to use
       * @param par parameters of sampling
       * @return true if instantiation of the sampling functor was successful
       */
      virtual bool setSampling(const std::string& samplingName,
                               const localSampling::parameters& par);

      /**
       * Set the parameters of the sampling internally used.
       */
      virtual bool setSamplingParameters(const localSampling::parameters& par);

      /**
       * This returns a pointer to the sampling functor internally used.
       * Be \b careful with that!
       * @return pointer to internal sampling
       */
      virtual localSampling* getSampling() const; 

    public:
      /**
       * Specifies a possible border for the direct apply on a local
       * normalized image region. 
       *
       * This parameter \b only takes effect when apply(src,dest)
       * is used.
       *
       * Default: 0
       */
      int borderWidth;

    protected:
      /**
       * Pointer to the localSampling class used by the functor
       */
      localSampling* sampling_;

      /**
       * Name of the class used for the local sampling.
       */
      std::string samplingName_;
    };

    /**
     * Default constructor
     */
    localDescriptorExtraction();

    /**
     * Construct a functor using the given parameters
     */
    localDescriptorExtraction(const parameters& par);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    localDescriptorExtraction(const localDescriptorExtraction& other);

    /**
     * Destructor
     */
    virtual ~localDescriptorExtraction();

    /**
     * Returns the name of this type.
     */
    virtual const std::string& name() const;

    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    localDescriptorExtraction& copy(const localDescriptorExtraction& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    localDescriptorExtraction& operator=(const localDescriptorExtraction& other);

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual localDescriptorExtraction* clone() const;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual localDescriptorExtraction* newInstance() const;

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;


    // Applies :
    /**
     * Compute the local descriptor for each location.
     * The standard implementation iterates through the lists and
     * invokes apply(const channel&,const location&,dvector&)
     * for each location.
     *
     * @param src original channel to be analyzed.
     * @param locs locations that need to be analyzed.
     * @param dest list of descriptor vectors.  
     * @return true if successful, or false otherwise.  
     */
    virtual bool apply(const fmatrix& src,
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
     * Compute the local descriptor for each location.
     * The standard implementation iterates through the lists and
     * invokes apply(const image&,const location&,dvector&)
     * for each location.
     *
     * @param src original image to be analyzed.
     * @param locs locations that need to be analyzed.
     * @param dest list of descriptor vectors.  
     * @return true if successful, or false otherwise.  
     */
    virtual bool apply(const matrix<rgbaPixel>& src,
                       const list<location>& locs,
                       std::list<dvector>& dest) const;
  
     /**
     * Compute the local descriptor for the given location.
     * The standard implementation splits the image in three
     * channels red,green,blue and concatenates the resulting
     * vectors of each apply(const channel&,const location&, dvector&)
     * in this order.
     *
     * @param src original image to be analyzed.
     * @param loc location that need to be analyzed.
     * @param dest descriptor vector. 
     * @return true if successful, or false otherwise.  
     */
    virtual bool apply(const matrix<rgbaPixel>& src,
                       const location& loc,
                       dvector& dest) const;

    /**
     * Compute the local descriptor for the given location.
     * 
     * \b NOTE: This is not implemented but needs to be defined to the
     * convenience functions in this functor. Any subclass MUST
     * implement this function.
     *
     * @param src original channel to be analyzed.
     * @param locs location that need to be analyzed.
     * @param dest descriptor vector. 
     * @return true if successful, or false otherwise.  
     */
    virtual bool apply(const fmatrix& src,
                       const location& locs,
                       dvector& dest) const;

    /**
     * Compute the descriptor for a square image.
     *
     * This method creates a location of angle zero that fits on the square
     * image leaving out the border width indicated in the parameters, and
     * computes the corresponding descriptor for it.
     *
     * \b Note: you should modify the sampling parameters such that the number
     * of sampled pixels is equal to the number of pixels in the image region.
     *
     * @param src region to use for sampling, has to be square.
     * @param dest descriptor vector.
     */
    virtual bool apply(const fmatrix& src,
                       dvector& dest) const;
   
  };
}

#endif

