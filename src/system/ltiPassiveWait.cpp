/*
 * Copyright (C) 1998, 1999, 2000, 2001
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
 * \file   ltiPassiveWait.cpp
 * \author Pablo Alvarado
 * \date   07.04.1999
 *
 * $Id: ltiPassiveWait.cpp,v 1.4 2005-07-22 15:59:25 doerfler Exp $
 */

#include "ltiMacroSymbols.h"
#include "ltiPassiveWait.h"

#ifndef _LTI_MSC_VER
#  include <sys/time.h>
#  include <unistd.h>
#else
#  include <windows.h>
#endif

namespace lti {

  // replacement for Sleep/usleep
  void passiveWait(const int usTime) {
#   ifdef _LTI_MSC_VER
    Sleep(usTime/1000);
#   else
    usleep(long(usTime));
#   endif
  }

}

