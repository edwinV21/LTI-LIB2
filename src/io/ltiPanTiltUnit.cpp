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
 * \file   ltiPanTiltUnit.cpp
 *         Contains an abstract parent class for all panTilt control classes.
 * \author Pablo Alvarado
 * \date   16.04.2007
 * $Id: ltiPanTiltUnit.cpp,v 1.1 2007-04-18 21:18:18 alvarado Exp $
 */

#include "ltiPanTiltUnit.h"

namespace lti {

  /**
   * Write the angular units format used
   */
  bool write(ioHandler& handler,const panTiltUnit::eAngularUnitsType val) {
    switch(val) {
    case panTiltUnit::DeviceDependent:
      handler.write("DeviceDependent");
      break;
    case panTiltUnit::Degrees:
      handler.write("Degrees");
      break;
    case panTiltUnit::Radians:
      handler.write("Radians");
      break;
    default:
      handler.write("DeviceDependent");
    }
    return false;
  }

  /**
   * Read the angular units format used
   */
  bool read(ioHandler& handler,panTiltUnit::eAngularUnitsType& val) {
    std::string str;
    if (handler.read(str)) {
      if (str.find("evice")!=std::string::npos) {
        val = panTiltUnit::DeviceDependent;
      } else if (str.find("Deg") != std::string::npos) {
        val = panTiltUnit::Degrees;
      } else if (str.find("Rad") != std::string::npos) {
        val = panTiltUnit::Radians;
      } else if (str.find("deg") != std::string::npos) {
        val = panTiltUnit::Degrees;
      } else if (str.find("rad") != std::string::npos) {
        val = panTiltUnit::Radians;
      } else {
        val = panTiltUnit::DeviceDependent;
      }
      return true;
    }
    return false;
  }


  // ------------------------------------------------------------------
  // panTiltUnit::parameters
  // ------------------------------------------------------------------

  /*
   * Default constructor
   */
  panTiltUnit::parameters::parameters() {
    angularUnits=DeviceDependent;
    panMode=FeatureAuto;
    pan=0;
    tiltMode=FeatureAuto;
    tilt=0;
  }

  /*
   * Copy constructor
   * @param other the parameters object to be copied
   */
  panTiltUnit::parameters::parameters(const parameters& other) {
    copy(other);
  }
  
  /*
   * Destructor
   */
  panTiltUnit::parameters::~parameters() {
  }

  /*
   * Copy the contents of a parameters object
   * @param other the parameters object to be copied
   * @return a reference to this parameters object
   */
  panTiltUnit::parameters& 
  panTiltUnit::parameters::copy(const parameters& other) {
    panMode=other.panMode;
    pan=other.pan;

    tiltMode=other.tiltMode;
    tilt=other.tilt;

    return *this;
  }

  /*
   * Copy the contents of a parameters object
   * @param other the parameters object to be copied
   * @return a reference to this parameters object
   */
  panTiltUnit::parameters& 
  panTiltUnit::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  /*
   * Write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool 
  panTiltUnit::parameters::write(ioHandler& handler,const bool complete) const{
    bool b=true;

    if (complete) {
      b=handler.writeBegin();
    }

    b = b && lti::write(handler,"angularUnits",angularUnits);
    b = b && lti::write(handler,"panMode",panMode);
    b = b && lti::write(handler,"pan",pan);
    b = b && lti::write(handler,"tiltMode",tiltMode);
    b = b && lti::write(handler,"tilt",tilt);
    
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
  bool panTiltUnit::parameters::read(ioHandler& handler,const bool complete) {
    bool b=true;

    if (complete) {
      b=handler.readBegin();
    }

    b = b && lti::read(handler,"angularUnits",angularUnits);
    b = b && lti::read(handler,"panMode",panMode);
    b = b && lti::read(handler,"pan",pan);
    b = b && lti::read(handler,"tiltMode",tiltMode);
    b = b && lti::read(handler,"tilt",tilt);
    
    if (complete) {
      b=b && handler.readEnd();
    }
    
    return b;
  }

  // ------------------------------------------------------------------
  // panTiltUnit
  // ------------------------------------------------------------------

  /*
   * Default constructor
   */
  panTiltUnit::panTiltUnit() {
  }

  panTiltUnit::~panTiltUnit() {
  }
}

