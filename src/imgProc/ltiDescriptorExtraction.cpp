/*
 * Copyright (C) 1998 - 2004
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


/*
 * \file   ltiDescriptorExtraction.cpp
 *         Implementation of lti::descriptorExtraction, the base class of
 *         all functors that extract descriptors from images
 * \author Pablo Alvarado
 * \date   09.09.2000
 *
 * $Id: ltiDescriptorExtraction.cpp,v 1.1 2009-08-30 04:30:05 alvarado Exp $
 */

#include "ltiDescriptorExtraction.h"

namespace lti {

  // --------------------------------------------------
  // descriptorExtraction::parameters
  // --------------------------------------------------

  // default constructor
  descriptorExtraction::parameters::parameters()
    : functor::parameters() {

  }

  // copy constructor
  descriptorExtraction::parameters::parameters(const parameters& other)
    : functor::parameters()  {
    copy(other);
  }

  // destructor
  descriptorExtraction::parameters::~parameters() {
  }

  // copy member
  descriptorExtraction::parameters&
    descriptorExtraction::parameters::copy(const parameters& other) {
    functor::parameters::copy(other);
    return *this;
  }
  
  // Write the parameters in the given ioHandler
  bool descriptorExtraction::parameters::write(ioHandler& handler,
                                              const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }
    
    b = b && functor::parameters::write(handler,false);
    
    if (complete) {
      b = b && handler.writeEnd();
    }
    
    return b;
  }
  
  // Read the parameters from the given ioHandler
  bool descriptorExtraction::parameters::read(ioHandler& handler,
                                             const bool complete)  {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }
    
    b = b && functor::parameters::read(handler,false);
    
    if (complete) {
      b = b && handler.readEnd();
    }
    
    return b;
  }
  
  // --------------------------------------------------
  // descriptorExtraction
  // --------------------------------------------------

  // default constructor
  descriptorExtraction::descriptorExtraction()
    : functor() {
  }

  // copy constructor
  descriptorExtraction::descriptorExtraction(const descriptorExtraction& other)
    : functor()  {
    copy(other);
  }

  // destructor
  descriptorExtraction::~descriptorExtraction() {
  }

  // copy member
  descriptorExtraction&
  descriptorExtraction::copy(const descriptorExtraction& other) {
    functor::copy(other);
    return (*this);
  }

  // return parameters
  const descriptorExtraction::parameters&
    descriptorExtraction::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }

  // -------------------------------------------------------------------
  // The apply-methods!
  // -------------------------------------------------------------------

}




