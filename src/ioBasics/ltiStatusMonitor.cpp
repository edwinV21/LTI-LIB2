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


/*
 * \file   ltiStatusMonitor.cpp
 *         Contains the parent class for all status monitors, which are
 *         objects that control what to do with error reports in functors,
 *         classifiers, viewers and ioHandlers.
 * \author Pablo Alvarado
 * \date   30.07.2004
 *
 * $Id: ltiStatusMonitor.cpp,v 1.3 2007-05-04 23:57:04 alvarado Exp $
 */

#include "ltiStatusMonitor.h"

namespace lti {
  /*
   * Default constructor
   */
  statusMonitor::statusMonitor() {
  }
  
  /*
   * Destructor
   */
  statusMonitor::~statusMonitor() {
  }

  /*
   * Return the name of this class
   */
  const std::string& statusMonitor::name() const {
    static const std::string theName = ("lti::statusMonitor");
    return theName;
  }

}
