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
 * \file   ltiDescriptorExtraction.h
 *         Definition of lti::descriptorExtraction, the base class of
 *         all functors that extract descriptors from images
 * \author Pablo Alvarado
 * \date   09.09.2000
 *
 * $Id: ltiDescriptorExtraction.h,v 1.1 2009-08-30 04:30:05 alvarado Exp $
 */


#ifndef _LTI_DESCRIPTOR_EXTRACTION_H_
#define _LTI_DESCRIPTOR_EXTRACTION_H_

#include "ltiObject.h"
#include "ltiImage.h"
#include "ltiFunctor.h"

namespace lti {
  /**
   * Abstract base class for all funtors that extract descriptors
   *
   * The descriptor extractions are functors which extract information
   * from the given images or channels or local regions thereof, and
   * leave this information in the form of vectors of doubles. This
   * class is parent class for all implemented descriptor extractions.
   */
  class descriptorExtraction : public functor {
  public:
    /**
     * The parameters for the class descriptorExtraction
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
    };
    
    /**
     * Default constructor
     */
    descriptorExtraction();

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    descriptorExtraction(const descriptorExtraction& other);

    /**
     * Destructor
     */
    virtual ~descriptorExtraction();

    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    descriptorExtraction& copy(const descriptorExtraction& other);

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;
  };
}

#endif

