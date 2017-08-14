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
 * \file   ltiSliceSampling.h
 *         Contains the template class lti::sliceSampling<T,Acc> and
 *         the protected class sliceMask.
 * \author Arnd Hannemann
 * \date   27.08.2004
 *
 * revisions ..: $Id: ltiSliceSampling.h,v 1.16 2012-07-01 00:36:14 alvarado Exp $
 */

#ifndef _LTI_SLICE_SAMPLING_H_
#define _LTI_SLICE_SAMPLING_H_

#include "ltiLocation.h"
#include "ltiKernel2D.h"
#include "ltiArctanLUT.h"
#include "ltiLocalSampling.h"
#include "ltiConvolution.h"
#include "ltiFactory.h"
#include "ltiBresenhamCircle.h"

#include <vector>
#include <list>


namespace lti {


  /**
   * Slice sampling.
   *
   * This class samples slices of a circular region around a given
   * location. 
   *
   * A valid accumulator has to provide following members:
   *   - bool accumulate(const T& value, const float& posx, const float& posy)
   *   - bool reset()
   *   - bool getResult(double result[])
   *   - static const int getArraySize()
   *   - typedef T type_info;
   *   - copy(other& acc)
   *
   * \code
   *   // example with meanAccumulator
   *
   *   // image to sample
   *   image img;
   *
   *   // parameters
   *   sliceSampling::parameters sPar;
   *   sPar.overlap = true;
   *   sPar.slices = 5;
   *
   *   // functor instance
   *   sliceSampling sliceS(sPar);
   *
   *   // location to sample (angle=0, radius=10)
   *   location loc(ipoint(100,100),0,10);
   *
   *   // resulting descriptor
   *   dvector result;
   *
   *   // creating accumulator
   *   meanAccumulator<rgbaPixel> accu;
   *
   *   // sample with meanAccumulator
   *   sliceS.apply(accu,img,loc,result);
   *
   * \endcode
   *
   * @ingroup gLocalSampling
   */
  class sliceSampling : public localSampling {
  public:
    /**
     * The parameters for the class sliceSampling
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
       * Returns the name of this class
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
       * Mask radius.
       *
       * The sampling will be made using a template mask of a fixed
       * size that is computed only once, when you set the parameters.  The
       * values in this fixed mask will be taken from the image using bilinear
       * interpolation.
       *
       * The greater the radius, the more pixels will be considered of 
       * computation in each slice, but the more the time it will take
       * to compute each location.
       *
       * This value should be greater than 2 and smaller than 255
       *
       * Default value: 7
       */
      int maskRadius;

      /**
       * Number of slices.
       *
       * A given location will be interpreted as a circle, that will
       * be divided in this number of slices.  The minimal allowed
       * value of slices is one, and the maximal allowed slices value
       * is 120.
       *
       * The number of dimensions of the output array will be
       * "slices" or "2*slices" depending on the "overlap" parameter.
       *
       * Default value: 6
       */
      int slices;

      /**
       * Whether overlapping slices should be used.
       *
       * If true, twice the number of slices will be computed.
       *
       * Default: true
       */
      bool overlap;

      /**
       * Whether the results should be filtered.
       *
       * If true, each "row" of results will be convolved 
       * with the filter kernel
       * 
       * Default: false
       */
      bool filter;

      /**
       * Filterkernel.
       * 
       * If filter is true, then each "row" of results will
       * be convolved with this kernel.
       */
      kernel1D<double> filterKernel;

    };

    /**
     * Operates on the given %parameters.
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
     * @param src matrix with source data.
     * @param src2 matrix with source data.
     * @param loc location where the sampling is done
     * @param dest vector where the result will be left.
     * @param order order, in which the result should be
     * @return true if apply successful or false otherwise.
     */
    template<class Acc>
    bool apply(Acc& acc,
               const matrix<typename Acc::value_type>& src,
               const matrix<float>& src2,
               const location& loc,
               dvector& dest, 
               const eResultOrder& order) const;

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
     * Operates on the given %parameters.
     * @param acc accumulator instance used for the sampling
     * @param src matrix with the source data.
     * @param src2 second matrix with source data.
     * @param loc location where the sampling is done
     * @param dest vector where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    template<class Acc>
    bool apply(Acc& acc,
               const matrix<typename Acc::value_type>& src,
               const matrix<float>& src2,
               const location& loc,
               dvector& dest) const;

    /**
     * Operates on the given %parameters.
     * @param acc accumulator instance used for the sampling
     * @param src matrix with the source data.
     * @param locs locations where the sampling is done
     * @param dest list of vectors  where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    template<class Acc>
    bool apply(Acc& acc,
               const matrix<typename Acc::value_type>& src,
               const list<location>& locs,
               std::list<dvector>& dest) const;

    /**
     * Operates on the given %parameters.
     * @param acc accumulator instance used for the sampling
     * @param src matrix with source data.
     * @param src2 matrix with source data.
     * @param locs locations where the sampling is done
     * @param dest list of vectors  where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    template<class Acc>
    bool apply(Acc& acc,
               const matrix<typename Acc::value_type>& src,
               const matrix<float>& src2,
               const list<location>& locs,
               std::list<dvector>& dest) const;


    /**
     * Operates on the given %parameters.
     * @param acc accumulator instance used for the sampling
     * @param src matrix with the source data.
     * @param locs locations where the sampling is done
     * @param dest list of vectors  where the result will be left.
     * @param order order to be used
     * @return true if apply successful or false otherwise.
     */
    template<class Acc>
    bool apply(Acc& acc,
               const matrix<typename Acc::value_type>& src,
               const list<location>& locs,
               std::list<dvector>& dest,
               const eResultOrder& order) const;

    /**
     * Returns the number of accumulator results in the result
     * vector.
     *
     * If parameters::overlap is false this corresponds to parameters::slices,
     * otherwise this corresponds to 2* parameters::slices.
     *
     * @return number of accumulator results in the output vector
     */
    virtual int getNumberOfRegions() const;

    /**
     * Returns the order of the accumulator results in the
     * result vector. (if there is more than one accumulator result)
     * This sampling returns the results in interleaved order.
     *
     * @return oder of accumulator results
     */
    virtual eResultOrder getResultOrder() const;

    /**
     * Update parameters this method is invoked by setParameters().
     */
    virtual bool updateParameters(); 


    /**
     * Default constructor
     */
    sliceSampling();

    /**
     * Construct a functor using the given parameters
     */
    sliceSampling(const parameters& par);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    sliceSampling(const sliceSampling& other);

    /**
     * Destructor
     */
    virtual ~sliceSampling();

    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    sliceSampling& copy(const sliceSampling& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    sliceSampling& operator=(const sliceSampling& other);

    /**
     * Returns the name of this class
     */
    virtual const std::string& name() const;

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual sliceSampling* clone() const;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual sliceSampling* newInstance() const;

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;


  protected:
    /** 
     * helper function to generate masks
     * usually invoked by updateParameters()
     */
    bool generateMasks();

    /**
     * Slice mask used to compute normal slices
     */
    kernel2D<ubyte> normalMask_;

    /**
     * Slice mask used to compute overlaping slices
     */
    kernel2D<ubyte> overlapMask_;

    /**
     * Input/output points.
     *
     * The size of this "matrix" will always be the 2 x number of rows of the
     * mask, but indexed also with negative values
     */
    array<int> ioPts_;

    /*
     * vector of accumulators used for computation of result
     */
    //    std::vector<Acc> accuVec_;

    /**
     * arctan lookup table
     */
    arctanLUT atan2_;

    /**
     * convolution used for filtering results
     */
    convolution filter_;

    /**
     * Functor which generates the circular mask.
     */
    bresenhamCircle circle_;
    
  };

}

#include "ltiSliceSampling_template.h"


#endif

