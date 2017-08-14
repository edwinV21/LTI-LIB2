/*
 * Copyright (C) 1998-2004
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
 * \file   ltiNearestNeighborDemosaicing.h
 *         Contains the class lti::nearestNeighborDemosaicing which is
 *         the most simple bayerDemosaicing.
 * \author Arnd Hannemann
 * \author Peter Doerfler
 * \date   11.12.2004
 *
 * revisions ..: $Id: ltiNearestNeighborDemosaicing.h,v 1.3 2012-09-16 04:58:33 alvarado Exp $
 */

#ifndef _LTI_NEAREST_NEIGHBOR_DEMOSAICING_H_
#define _LTI_NEAREST_NEIGHBOR_DEMOSAICING_H_

#include "ltiMatrix.h"
#include "ltiImage.h"
#include "ltiChannel.h"
#include "ltiChannel8.h"
#include "ltiBayerDemosaicing.h"

namespace lti {

  /**
   * This is the most simple kind of bayerDemosaicing. 
   *
   * @ingroup gAcquisition
   */
  class nearestNeighborDemosaicing : public bayerDemosaicing {
  public:
    /**
     * The parameters for the class nearestNeighborDemosaicing
     */
    class parameters : public bayerDemosaicing::parameters {
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
       * Returns the name of the parameter class
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

    };

    /**
     * Default constructor
     */
    nearestNeighborDemosaicing();

    /**
     * Construct a functor using the given parameters
     */
    nearestNeighborDemosaicing(const parameters& par);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    nearestNeighborDemosaicing(const nearestNeighborDemosaicing& other);

    /**
     * Destructor
     */
    virtual ~nearestNeighborDemosaicing();

    /**
     * Transforms a raw Bayer pattern image into an lti::image using
     * the nearest neighbor method as described in the class
     * documentation.
     *
     * @param raw contains a raw image of one of the parameters::eBayerPattern
     * @param reconst the demosaiced image
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const matrix<ubyte>& raw, image& reconst) const;

    // make other apply member functions available
    using bayerDemosaicing::apply;

    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    nearestNeighborDemosaicing& copy(const nearestNeighborDemosaicing& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    nearestNeighborDemosaicing& 
    operator=(const nearestNeighborDemosaicing& other);

    /**
     * Returns the name of the parameter class
     */
    virtual const std::string& name() const;

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual nearestNeighborDemosaicing* clone() const;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual nearestNeighborDemosaicing* newInstance() const;

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;
  };
}

#endif

