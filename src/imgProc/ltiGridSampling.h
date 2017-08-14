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
 * \file   ltiGridSampling.h
 *         Contains the template class lti::gridSampling<T,Acc>, which is 
 *         a local sampling functor. (works on locations)
 * \author Arnd Hannemann
 * \date   12.8.2004
 *
 * revisions ..: $Id: ltiGridSampling.h,v 1.7 2009-08-30 04:30:29 alvarado Exp $
 */

#ifndef _LTI_GRID_SAMPLING_H_
#define _LTI_GRID_SAMPLING_H_


#include "ltiLocalSampling.h"
#include "ltiBilinearInterpolation.h"
#include "ltiBresenhamCircle.h"

#include "ltiArray.h"

// register in local sampling factory
//#include "ltiFactory.h"
//_LTI_REGISTER_IN_FACTORY("localSampling","gridSampling");

namespace lti {

  /**
   * This class samples a circular region around a given location.
   *
   * @see gridSampling::parameters.
   *
   * \b Note:
   * A mask for the sampling will be created when setParameters() 
   * is invoked. This speeds up the apply() methods but makes 
   * setParameters() a bit time consuming.   
   *
   * @ingroup gLocalSampling
   */
  class gridSampling : public localSampling {
  public:
    /**
     * The parameters for the class gridSampling
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
       * Returns name of this type
       */
      const std::string& name() const;

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
       * If the angle from the location should be used.
       *
       * Default: true
       */
      bool useOrientation;

      /**
       * Which radius is used for the grid mask
       *
       * Default: 3
       */
      int gridRadius;

    };

    /**
     * Default constructor
     */
    gridSampling();

    /**
     * Construct a functor using the given parameters
     */
    gridSampling(const parameters& par);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    gridSampling(const gridSampling& other);

    /**
     * Destructor
     */
    virtual ~gridSampling();

    /**
     * Returns the name of this type, which depends on the template parameters.
     */
    virtual const std::string& name() const;


    /**
     * Update parameters this method is invoked by setParameters().
     */
    virtual bool updateParameters(); 


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
     * matrix, no  interpolation is done.
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
     * Samples a region around a given location in a given image.
     * @param acc accumulator instance used for the sampling
     * @param src matrix with the source data.
     * @param loc location around the sampling will be done
     * @param dest array where the result will be left.
     * @param order order, in which the result should be
     * @return true if apply successful or false otherwise.
     */
    template<class Acc>
    bool apply(Acc& acc,
               const matrix<typename Acc::value_type>& src,
               const location& loc,
               dvector& dest,
               const eResultOrder& order=DontCare) const;

    /**
     * Samples regions around a list of given locations.
     * @param acc accumulator instance used for the sampling
     * @param src matrix with the source data.
     * @param loc location around the sampling will be done
     * @param dest array where the result will be left.
     * @param order desired order (default: DontCare).
     * @return true if apply successful or false otherwise.
     */
    template<class Acc>
    bool apply(Acc& acc,
               const matrix<typename Acc::value_type>& src,
               const list<location>& loc,
               std::list<dvector>& dest,
               const eResultOrder& order=DontCare) const;
    
    /**
     * Samples a region around a given location in a given image.
     * @param acc accumulator instance used for the sampling
     * @param src matrix with the source data.
     * @param src2 channel with source data
     * @param loc location around the sampling will be done
     * @param dest array where the result will be left.
     * @param order desired order (default: DontCare).
     * @return true if apply successful or false otherwise.
     */
    template<class Acc>
    bool apply(Acc& acc,
               const matrix<typename Acc::value_type>& src,
               const matrix<float>& src2,
               const location& loc,
               dvector& dest,
               const eResultOrder& order=DontCare) const;

    /**
     * Samples regions around a list of given locations.
     * @param acc accumulator instance used for the sampling
     * @param src matrix with the source data.
     * @param src2 channel with source data
     * @param loc location around the sampling will be done
     * @param dest array where the result will be left.
     * @param order desired order (default: DontCare).
     * @return true if apply successful or false otherwise.
     */
    template<class Acc>
    bool apply(Acc& acc,
               const matrix<typename Acc::value_type>& src,
               const matrix<float>& src2,
               const list<location>& loc,
               std::list<dvector>& dest,
               const eResultOrder& order=DontCare) const;


    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    gridSampling& copy(const gridSampling& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    gridSampling& operator=(const gridSampling& other);

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual gridSampling* clone() const;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual gridSampling* newInstance() const;

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;

  protected:

    /** 
     * internal helper functor to create circular mask 
     */
    bresenhamCircle initCircle_;

    /** circular mask */
    array<int> circPoints_;

  };
}

#include "ltiGridSampling_template.h"

#endif

