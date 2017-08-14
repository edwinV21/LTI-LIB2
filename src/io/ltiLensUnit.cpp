/*
 * Copyright (C) 2007
 * Pablo Alvarado, ITCR
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


/*--------------------------------------------------------------------
 * \file   ltiLensUnit.cpp
 *         Contains an abstract parent class for all lens control classes.
 * \author Pablo Alvarado
 * \date   16.04.2007
 * $Id: ltiLensUnit.cpp,v 1.1 2007-04-18 21:18:18 alvarado Exp $
 */

#include "ltiLensUnit.h"

namespace lti {

  // ------------------------------------------------------------------
  // lensUnit::parameters
  // ------------------------------------------------------------------

  /*
   * Default constructor
   */
  lensUnit::parameters::parameters() {
    irisMode=FeatureAuto;
    iris=0;
    focusMode=FeatureAuto;
    focus=0;
    zoomMode=FeatureAuto;
    zoom=0;
    opticalFilterMode=FeatureAuto;
    opticalFilter=0;
  }

  /*
   * Copy constructor
   * @param other the parameters object to be copied
   */
  lensUnit::parameters::parameters(const parameters& other) {
    copy(other);
  }
  
  /*
   * Destructor
   */
  lensUnit::parameters::~parameters() {
  }

  /*
   * Copy the contents of a parameters object
   * @param other the parameters object to be copied
   * @return a reference to this parameters object
   */
  lensUnit::parameters& lensUnit::parameters::copy(const parameters& other) {
    irisMode=other.irisMode;
    iris=other.iris;

    focusMode=other.focusMode;
    focus=other.focus;

    zoomMode=other.zoomMode;
    zoom=other.zoom;

    opticalFilterMode=other.opticalFilterMode;
    opticalFilter=other.opticalFilter;

    return *this;
  }

  /*
   * Copy the contents of a parameters object
   * @param other the parameters object to be copied
   * @return a reference to this parameters object
   */
  lensUnit::parameters& lensUnit::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  /*
   * Write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool lensUnit::parameters::write(ioHandler& handler,const bool complete) const{
    bool b=true;

    if (complete) {
      b=handler.writeBegin();
    }

    b = b && lti::write(handler,"irisMode",irisMode);
    b = b && lti::write(handler,"iris",iris);
    b = b && lti::write(handler,"focusMode",focusMode);
    b = b && lti::write(handler,"focus",focus);
    b = b && lti::write(handler,"zoomMode",zoomMode);
    b = b && lti::write(handler,"zoom",zoom);
    b = b && lti::write(handler,"opticalFilterMode",opticalFilterMode);
    b = b && lti::write(handler,"opticalFilter",opticalFilter);
    
    if (complete) {
      b=b&&handler.writeEnd();
    }
    
    return b;
  }

  /*
   * Read the parameters from the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool lensUnit::parameters::read(ioHandler& handler,const bool complete) {
    bool b=true;

    if (complete) {
      b=handler.readBegin();
    }

    b = b && lti::read(handler,"irisMode",irisMode);
    b = b && lti::read(handler,"iris",iris);
    b = b && lti::read(handler,"focusMode",focusMode);
    b = b && lti::read(handler,"focus",focus);
    b = b && lti::read(handler,"zoomMode",zoomMode);
    b = b && lti::read(handler,"zoom",zoom);
    b = b && lti::read(handler,"opticalFilterMode",opticalFilterMode);
    b = b && lti::read(handler,"opticalFilter",opticalFilter);
    
    if (complete) {
      b=b && handler.readEnd();
    }
    
    return b;
  }

  // ------------------------------------------------------------------
  // lensUnit
  // ------------------------------------------------------------------

  /*
   * Default constructor
   */
  lensUnit::lensUnit() {
  }

  lensUnit::~lensUnit() {
  }
}

