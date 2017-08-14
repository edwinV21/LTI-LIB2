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
 * \file   ltiSiftSampling.h
 *         Contains the class lti::siftSampling.
 * \author Arnd Hannemann
 * \date   5.1.2005
 *
 * revisions ..: $Id: ltiSiftSampling.h,v 1.10 2012-09-16 05:22:59 alvarado Exp $
 */

#ifndef _LTI_SIFT_SAMPLING_H_
#define _LTI_SIFT_SAMPLING_H_


#include "ltiImage.h"
#include "ltiLocalSampling.h"
#include "ltiKernel2D.h"
#include "ltiKernel1D.h"
#include "ltiBilinearInterpolation.h"
#include "ltiNearestNeighborInterpolation.h"
 
namespace lti {

  /**
   * This class samples square parts of a square region around a given
   * location. 
   *
   * @see siftSampling::parameters.
   *
   * A valid accumulator has to provide following members:
   *
   * - bool accumulate(const T& value, const float& posx, const float& posy)
   * - bool reset()
   * - bool getResult(double result[])
   * - static const int getArraySize()
   * - typedef T type_info;
   * - copy(other& acc)
   *
   * \code
   *   // example with meanAccumulator
   *
   *   // image to sample
   *   image img;
   *
   *   // parameters
   *   siftSampling::parameters sPar;
   *   // both values has to be even
   *   sPar.binsPerDimension = 3;
   *   sPar.binSize = 2;
   *
   *   // functor instance
   *   siftSampling siftS(sPar);
   *
   *   // location to sample (angle=0, radius=10)
   *   // for now radius is ignored
   *   location loc(ipoint(100,100),0,10);
   *
   *   // resulting descriptor
   *   dvector result;
   *
   *   // creating accumulator
   *   meanAccumulator<rgbaPixel> accu;
   *
   *   // sample with meanAccumulator
   *   siftS.apply(accu,img,loc,result);
   *
   * \endcode
   *
   * @ingroup gLocalSampling
   */
  class siftSampling : public localSampling {
  public:
    /**
     * The parameters for the class siftSampling
     */
    class parameters : public localSampling::parameters {
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
       * This value determines how many bins are used in each
       * direction.
       *
       * Default: 3
       */
      int binsPerDimension;

      /**
       * This value determines the edge length of each bin in pixel.
       * This value has to be even.
       *
       * Default: 2 
       */
      int binSize;

      /**
       * Whether the accumulated values are weighted with a gaussian 
       * filter.
       *
       * Default: true
       */
      bool filter;

      /**
       * Variance of the gaussian filter.
       * If negative, a default value will be used.
       * @see lti::gaussKernel1D<T>
       *
       * Default: -1
       */
      float variance;

    };

    /**
     * Default constructor
     */
    siftSampling();

    /**
     * Construct a functor using the given parameters
     */
    siftSampling(const parameters& par);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    siftSampling(const siftSampling& other);

    /**
     * Destructor
     */
    virtual ~siftSampling();

    /**
     * Operates on the given %parameters.
     * @param acc accumulator instance used for the sampling
     * @param src matrix with the source data.
     * @param loc location where the sampling is done
     * @param dest vector where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    template<class Acc>
    bool apply(Acc& acc,
               const matrix<typename Acc::value_type>& src,
               const location& loc,
               dvector& dest) const;

    /**
     * Operates on a copy of the given %parameters.
     * @param acc accumulator instance used for the sampling
     * @param src matrix with the source data.
     * @param loc location where the sampling is done
     * @param dest vector where the result will be left.
     * @param order order, in which the result should be
     * @return true if apply successful or false otherwise.
     */
    template<class Acc>
    bool apply(Acc& acc,
               const matrix<typename Acc::value_type>& src,
               const location& loc,
               dvector& dest, 
               const eResultOrder& order) const;

    /**
     * Operates on the given %parameters.
     * @param acc accumulator instance used for the sampling
     * @param src1 matrix with the source data.
     * @param src2 matrix with the source data.
     * @param loc location where the sampling is done
     * @param dest vector where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    template<class Acc>
    bool apply(Acc& acc,
               const matrix<typename Acc::value_type>& src1,
               const matrix<float>& src2,
               const location& loc,
               dvector& dest) const;

    /**
     * Operates on a copy of the given %parameters.
     * @param acc accumulator instance used for the sampling
     * @param src1 first matrix with source data.
     * @param src2 second matrix with source data.
     * @param loc loctaion where the sampling is done
     * @param dest vector where the result will be left.
     * @param order order, in which the result should be
     * @return true if apply successful or false otherwise.
     */
    template<class Acc>
    bool apply(Acc& acc,
               const matrix<typename Acc::value_type>& src1,
               const matrix<float>& src2,
               const location& loc,
               dvector& dest, 
               const eResultOrder& order) const;



    /**
     * This apply samples a region around the centers of the given
     * matrices, no interpolation is done.
     * @param acc accumulator instance used for the sampling
     * @param src1 first matrix with source data.
     * @param src2 second matrix with source data.
     * @param dest vector where the result will be left.
     * @param order order, in which the result should be
     * @return true if apply successful or false otherwise.
     */
    template<class Acc>
    bool apply(Acc& acc,
               const matrix<typename Acc::value_type>& src1,
               const matrix<float>& src2,
               dvector& dest,
               const eResultOrder& order = DontCare) const;

    /**
     * This apply samples a region around the center of the given
     * matrix, no interpolation is done.
     * @param acc accumulator instance used for the sampling
     * @param src matrix with source data.
     * @param dest vector where the result will be left.
     * @param order order, in which the result should be
     * @return true if apply successful or false otherwise.
     */
    template<class Acc>
    bool apply(Acc& acc,
               const matrix<typename Acc::value_type>& src,
               dvector& dest,
               const eResultOrder& order = DontCare) const;

    
    /**
     * Operates on a copy of the given %parameters.
     * @param acc accumulator instance used for the sampling
     * @param src matrix with the source data.
     * @param locs list of locations where the sampling is done
     * @param dest list of results.
     * @return true if apply successful or false otherwise.
     */
    template<class Acc>
    bool apply(Acc& acc,
               const matrix<typename Acc::value_type>& src,
               const list<location>& locs,
               std::list<dvector>& dest) const;


    /**
     * Operates on a copy of the given %parameters.
     * @param acc accumulator instance used for the sampling
     * @param src matrix with the source data.
     * @param locs list of locations where the sampling is done
     * @param dest list of results.
     * @param order order to be used in the result
     * @return true if apply successful or false otherwise.
     */
    template<class Acc>
    bool apply(Acc& acc,
               const matrix<typename Acc::value_type>& src,
               const list<location>& locs,
               std::list<dvector>& dest,
               const eResultOrder& order) const;

    /**
     * Operates on a copy of the given %parameters.
     * @param acc accumulator instance used for the sampling
     * @param src1 first matrix with source data.
     * @param src2 second matrix with source data.
     * @param locs list of locations where the sampling is done
     * @param dest list of results.
     * @param order order to be used in the result
     * @return true if apply successful or false otherwise.
     */
    template<class Acc>
    bool apply(Acc& acc,
               const matrix<typename Acc::value_type>& src1,
               const matrix<float>& src2,
               const list<location>& locs,
               std::list<dvector>& dest,
               const eResultOrder& order) const;

    /**
     * Operates on a copy of the given %parameters.
     * @param acc accumulator instance used for the sampling
     * @param src1 first matrix with source data.
     * @param src2 second matrix with source data.
     * @param locs list of locations where the sampling is done
     * @param dest list of results.
     * @return true if apply successful or false otherwise.
     */
    template<class Acc>
    bool apply(Acc& acc,
               const matrix<typename Acc::value_type>& src1,
               const matrix<float>& src2,
               const list<location>& locs,
               std::list<dvector>& dest) const;

    /**
     * Returns the number of accumulator results in the result
     * vector. This is equal to the square of parameters::binsPerDimension.
     *
     * @return number of accumulator results in the output vector
     */
    virtual int getNumberOfRegions() const;

    /**
     * Returns the order of the accumulator results in the
     * result vector. (if there is more than one accumulator result)
     * This sampling returns the result in sequential order.
     *
     * @return oder of accumulator results
     */
    virtual eResultOrder getResultOrder() const;

    /**
     * Update parameters this method is invoked by setParameters().
     */
    virtual bool updateParameters(); 
    

    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    siftSampling& copy(const siftSampling& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    siftSampling& operator=(const siftSampling& other);

    /**
     * Returns the complete name of the functor class
     */
    virtual const std::string& name() const;

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual siftSampling* clone() const;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual siftSampling* newInstance() const;

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;

  protected:
    /** constant used for accumulators */
    static const int NA_;

    /** helper function for two sources */
    template <class Acc>
    inline void accumulate(std::vector<Acc>& accuVec,
                           const typename Acc::value_type val,
                           const float val2,                  
                           const int x,                       
                           const int y,                      
                           const float ay) const;

    /** helper function for one source */
    template <class Acc>
    inline void accumulate(std::vector<Acc>& accuVec,
                           const typename Acc::value_type val,
                           const int x,                       
                           const int y,                      
                           const float ay) const;
    
    /** weight for upper left accumulator */
    kernel2D<float> mask1_;
    /** weight for upper right accumulator */
    kernel2D<float> mask2_;
    /** weight for lower left accumulator */
    kernel2D<float> mask3_;
    /** weight for lower right accumulator */
    kernel2D<float> mask4_;

    
    /** stores the number of the upper left accumulator for a 
     *  given pixel */
    kernel2D<int> accu1_;
    /** stores the number of the upper right accumulator for a 
     *  given pixel */
    kernel2D<int> accu2_;
    /** stores the number of the lower left accumulator for a 
     *  given pixel */
    kernel2D<int> accu3_;
    /** stores the number of the lower right accumulator for a 
     *  given pixel */
    kernel2D<int> accu4_;

    /** mask to do normalization of coordinates to (-1,1) */
    kernel1D<float> norm_;

    
    /** sampling will done within this maximum radius, 
     *  used for bounding checks */
    float rad_;

  };
}

#include "ltiSiftSampling_inline.h"
#include "ltiSiftSampling_template.h"

#endif

