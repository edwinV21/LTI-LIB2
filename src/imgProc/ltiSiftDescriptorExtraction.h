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
 * \file   ltiSiftDescriptorExtraction.h
 *         Contains the class lti::siftDescriptorExtraction.
 *
 * \author Arnd Hannemann
 * \date   2.3.2005
 *
 * revisions ..: $Id: ltiSiftDescriptorExtraction.h,v 1.1 2009-08-30 04:30:05 alvarado Exp $
 */

#ifndef _LTI_SIFT_DESCRIPTOR_EXTRACTION_H_
#define _LTI_SIFT_DESCRIPTOR_EXTRACTION_H_

#include "ltiLocalDescriptorExtraction.h"

namespace lti {

  /**
   * This functor implements the SIFT-Features developed by Lowe.
   * 
   * By default it uses the lti::siftSampling to sample the image data, but
   * the sampling my be changed.
   * (see lti::localDescriptorExtraction::parameters).
   *
   * \section Considerations
   *
   * To use the parent class method apply(fmatrix,dvector) you have to ensure
   * that your image region * has the size of binsPerDimension multiplied by
   * binSize.
   *
   * Example code:
   * \code
   * // load image
   * loadPNG loader;
   * image img;
   * loader.load("hough1.png",img);
   *
   * // compute gradients
   * channel mag,arg;
   * channel src;
   * src.castFrom(img);
   * gradientFunctor grad;
   * grad.apply(src,mag,arg);
   *          
   *
   * // acquire locations
   * location loc(ipoint(114,114),0,10);
   *
   * // create sampling parameters instance
   * siftSampling::parameters siftPar;
   * 
   * // change sampling parameters
   * siftPar.filter = false;
   * siftPar.binSize = 4;
   * siftPar.binsPerDimension = 2;
   *
   * // create descriptor extraction parameters
   * siftDescriptorExtraction::parameters sdePar;
   *
   * // set sampling and sampling parameters
   * sdePar.setSampling("lti::siftSampling",siftPar);
   * sdePar.histogramBins = 36;
   *
   * // create descriptor extraction
   * siftDescriptorExtraction sift(sdePar);
   *
   * // apply it 
   * dvector dest;
   * sift.apply(arg,mag,loc,dest);
   *
   * // display result
   * std::cout << dest <<"\n";
   *
   * \endcode
   *
   * @see siftDescriptorExtraction::parameters.
   *
   *
   * @ingroup gLocalDescriptorExtraction
   */
  class siftDescriptorExtraction : public localDescriptorExtraction {
  public:

    /**
     * Enumeration of accumulator types
     */
    enum eAccuType {
      Histogram,            /**< Histogram only.   */
      InterpolatedHistogram, /**< Histogram with bilinear interpolation.*/
      InterpolatedCyclicHistogram /**< Histogram for a cyclic value range
                                   *   with bilinear interpolation. */
    };
    

    /**
     * The parameters for the class siftDescriptorExtraction
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
       * How many bins are used for each histogram 
       */
      int histogramBins;

      /**
       * which accumulator is used to compute descriptors
       * 
       * Default: InterpolatedCyclicHistogram
       */
      eAccuType accuType;

      /**
       * low bound of histograms.
       *
       * Default: 0
       */
      float histLow;
      
      /**
       * high bound of histograms.
       *
       * Default: 2*Pi()
       */
      float histHigh;

      /**
       * If true the resulting descriptor is normalized to sum of 1.
       *
       * Default: true
       */
      bool normalize;

      /**
       * If normalize is true and useClipping is true values greater
       * than clippingValue are clipped to clippingValue and the
       * descriptor is normalized again.
       *
       * Default: true
       */
      bool useClipping;

      /**
       * Values greater clippingValue are clipped if normalize and
       * useClipping are true. See also useClipping.
       *
       * Default: 0.2 (as suggested by Lowe)
       */
      double clippingValue;
    };

    /**
     * Default constructor
     */
    siftDescriptorExtraction();

    /**
     * Construct a functor using the given parameters
     */
    siftDescriptorExtraction(const parameters& par);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    siftDescriptorExtraction(const siftDescriptorExtraction& other);

    /**
     * Destructor
     */
    virtual ~siftDescriptorExtraction();
    
    // uses apply of parent class
    using localDescriptorExtraction::apply;

    /**
     * Operates on a copy of the given %parameters.
     *
     * @param keys this channel determines to which bin a particular value
     * is added. (e.g. gradient orientation).
     * @param values channel with the values added to a particular bin. 
     * (e.g. gradient magnitude)
     * @param loc location that need to be analyzed.
     * @param dest descriptor vector.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const matrix<float>& keys,
               const matrix<float>& values, 
               const location& loc,
               dvector& dest);

    /**
     * Operates on a copy of the given %parameters.
     *
     * @param keys this channel determines to which bin a particular value
     * is added. (e.g. gradient orientation).
     * @param values channel with the values added to a particular bin. 
     * (e.g. gradient magnitude)
     * @param dest descriptor vector.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const matrix<float>& keys,
               const matrix<float>& values, 
               dvector& dest);


    /**
     * Operates on a copy of the given %parameters.
     *
     * @param src channel with source data.
     * @param loc location that need to be analyzed.
     * @param dest descriptor vector.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const matrix<float>& src, const location& loc,
               dvector& dest);

    /**
     * Operates on a copy of the given %parameters.
     *
     * @param src channel with source data.
     * @param dest descriptor vector.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const matrix<float>& src,
               dvector& dest);
    

    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    siftDescriptorExtraction& copy(const siftDescriptorExtraction& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    siftDescriptorExtraction& operator=(const siftDescriptorExtraction& other);

    /**
     * Returns the complete name of the functor class
     */
    virtual const std::string& name() const;

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual siftDescriptorExtraction* clone() const;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual siftDescriptorExtraction* newInstance() const;

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;

  protected:
    
    /**
     * Normalize and clip the \a descriptor depending on the parameter
     * settings.
     */
    void normalizeAndClip(dvector& descriptor) const;


  };
} // namespace

#endif

