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
 * \file   ltiLocalSampling.h
 *         Contains the class lti::localSampling, is base class
 *         for all sampling functors.
 * \author Arnd Hannemann
 * \date   10.8.2004
 *
 * revisions ..: $Id: ltiLocalSampling.h,v 1.21 2009-08-30 04:30:29 alvarado Exp $
 */

#ifndef _LTI_LOCAL_SAMPLING_H_
#define _LTI_LOCAL_SAMPLING_H_

#include "ltiList.h"
#include "ltiFunctor.h"
#include "ltiImage.h"
#include "ltiRGBAPixel.h"
#include "ltiLocation.h"

namespace lti {

  /**
   * This class is parent class for all local sampling functors.
   * 
   * How to add a new sampling:
   *
   *  - should inherit from localSampling
   *  - in the header of the new sampling class only include ltiLocalSampling.h
   *    and NOT ltiLocalSampling_template.h
   *  - include the header of your new sampling class in
   *    ltiLocalSampling_template.h
   *  - for each apply in ltiLocalSampling_template.h add a new section that 
   *    calls the right apply of your new sampling
   *  - register your new sampling in localSampling factory
   *    (just add _LTI_REGISTER_IN_FACTORY(localSampling, yourNewSampling);
   *    to the .cpp of your sampling and make sure ltiFactory.h is included)
   *  - note that the header ltiLocalSampling.h does NOT include
   *    ltiLocalSampling_template.h to avoid forward class declarations, which
   *    gave errors with some compilers
   *  - for example see sliceSampling
   *    
   * How to add a new accumulator:
   * 
   *  - no parent class recommended (keep accumulator as small as possible) 
   *
   *  - make sure the following methods exists:
   *    bool getResult(double[] result), 
   *    bool reset(), 
   *    bool accumulate(T val, float posx, float posy),
   *    bool accumulate(T val, float weight, float posx, float posy);
   *    int  getArraySize();
   *    accu& copy(accu& other);
   *    copy constructor
   *    
   *  - make sure that the member value_type exists (typedef T value_type;)
   *  
   *  - for example see meanAccumulator
   *
   * Howto add a new descriptor extractor:
   * 
   *  - should inherit localDescriptorExtraction
   *
   *  - important: include "ltiLocalSampling_template.h" in the cpp of the
   *    new descriptor extraction
   *
   *  - get local sampling instance and invoke with an accumulator instance
   *    (localSampling* ls = getParameters().getSampling();)
   *
   *  - for example see localColorDescriptorExtraction
   *
   * \warning This tree of classes must be completely redesigned, since
   * the file interdependencies are too strong, and adding new classes 
   * is so complex and cumbersome that it would be even the same to have
   * the different samplings as methods of a single class.
   *
   * @see meanAccumulator
   * @see sliceSampling
   * @see localColorDescriptorExtraction
   *
   * @ingroup gLocalSampling
   */
  class localSampling : public functor {
  public:

    /**
     * Enumeration of result orders
     */
    enum eResultOrder {
      Interleaved, /**< the results of the accumulators in the result
                    *   vector are interleaved. (1. element of 1. accu,
                    *   1. element of 2. accu, ...)
                    */
      Sequential,  /**< the result of the accumulators in the result
                    *   vector are sequential. (1. element of 1. accu,
                    *   2. element of 1. accu, ...)
                    */
      DontCare     /**< the sampling chooses an order of its own */
    };

    /**
     * The parameters for the class localSampling
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
       * Determines if the key source consists of angles.
       * (Those angle should be in the range from [-Pi,Pi])
       * If this is true the key source is sampled with
       * nearestNeighborInterpolation instead of bilinearInterpolation.
       * Furthermore the values 
       * 
       *
       * Default: false
       */
      bool keyIsOrientation;

    };


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
               const eResultOrder& order = DontCare) const;

    /**
     * Operates on a copy of the given %parameters.
     * @param acc accumulator instance used for the sampling
     * @param src1 first matrix with source data.
     * @param src2 second matrix with weights data.
     * @param loc location where the sampling is done
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
               const eResultOrder& order = DontCare) const; 

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
               const eResultOrder& order = DontCare) const;

    /**
     * Operates on a copy of the given %parameters.
     * @param acc accumulator instance used for the sampling
     * @param src1 first matrix with source data.
     * @param src2 second matrix with weights data.
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
               const eResultOrder& order = DontCare) const;
    
    /**
     * This apply samples a region around the centers of the given
     * matrices, no interpolation is done.
     * @param acc accumulator instance used for the sampling
     * @param src1 first matrix with source data.
     * @param src2 second matrix with weights data.
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
     * Returns the number of accumulator results in the result
     * vector.
     *
     * The default implementation always returns 1.
     *
     * @return number of accumulator results in the output vector
     */
    virtual int getNumberOfRegions() const;

    /**
     * Returns the order of the accumulator results in the
     * result vector. (if there is more than one accumulator result)
     *
     * The default implementation always returns Sequential.
     *
     * @return oder of accumulator results
     */
    virtual eResultOrder getResultOrder() const;

    /**
     * Default constructor
     */
    localSampling();

    /**
     * Construct a functor using the given parameters
     */
    localSampling(const parameters& par);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    localSampling(const localSampling& other);

    /**
     * Destructor
     */
    virtual ~localSampling();

    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    localSampling& copy(const localSampling& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    localSampling& operator=(const localSampling& other);

    /**
     * Returns the name of this type.
     */
    virtual const std::string& name() const;

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual localSampling* clone() const;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual localSampling* newInstance() const;

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;


  protected:
    /**
     * Helper function which switches the order of a result vector
     */
    void switchOrder(dvector& srcdest) const;

    /**
     * Helper function which subtracts an angle of a value, and 
     * clips them to [0..2Pi]
     */
    template<typename T>
    inline void clipAngle(T& val, const float angle) const;
    
    /**
     * Last
     */
    mutable int last_;
  };
  
}
// DO NOT INCLUDE the template file at this place
// otherwise it won't compile with e.g. gcc 3.4.x
// instead include both ltiLocalSampling.h AND ltiLocalSampling_template.h
// where needed

// #include "ltiLocalSampling_template.h" 
#include "ltiLocalSampling_inline.h"

#endif

