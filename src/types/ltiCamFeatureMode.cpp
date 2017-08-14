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
 * \file   ltiCamFeatureMode.cpp
 *         Type used to indicate the operation mode of camera
 *         features, like manual, automatic, off, not-available, etc.
 * \author Pablo Alvarado
 * \date   16.04.2007
 * $Id: ltiCamFeatureMode.cpp,v 1.1 2007-04-18 21:16:38 alvarado Exp $
 */


#include "ltiCamFeatureMode.h"

namespace lti {
  /*
   * Read function for eCamFeatureMode type
   *
   * @ingroup gStorable
   */
  bool read(ioHandler& handler, eCamFeatureMode& data) {
    std::string str;
    if (handler.read(str)) {
      if (str.find("NA") != std::string::npos) {
        data=FeatureNA;
      } else if (str.find("ff") != std::string::npos) {
        data=FeatureOff;
      } else if (str.find("uto") != std::string::npos) {
        data=FeatureAuto;
      } else if (str.find("anual") != std::string::npos) {
        data=FeatureManual;
      }  else if (str.find("ush") != std::string::npos) {
        data=FeatureOnePush;
      } else if (str.find("bsolute") != std::string::npos) {
        data=FeatureAbsolute;
      } else {
        data=FeatureOff;
        str="Unknown symbol " + str;
        handler.setStatusString(str.c_str());
        return false; // unknown symbol
      }
      return true;
    } 
    return false;
  }

  /*
   * Write function for eCamFeatureMode type
   *
   * @ingroup gStorable
   */
  bool write(ioHandler& handler, const eCamFeatureMode& data) {
    switch(data) {
      case FeatureNA:
        return write(handler,"FeatureNA");
        break;
      case FeatureOff:
        return write(handler,"FeatureOff");
        break;
      case FeatureAuto:
        return write(handler,"FeatureAuto");
        break;
      case FeatureManual:
        return write(handler,"FeatureManual");
        break;
      case FeatureOnePush:
        return write(handler,"FeatureOnePush");
        break;
      case FeatureAbsolute:
        return write(handler,"FeatureAbsolute");
        break;
      default:
        return write(handler,"Unknown");
        break;
    }
    return false;
  }


} // namespace lti
