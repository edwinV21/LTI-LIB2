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

/**
 * \file   ltiPassiveWait.h
 *         Contains the function lti::passiveWait as wrapper for the different
 *         OS dependent passive wait formats with microsecond precision.
 * \author Pablo Alvarado
 * \date   12.06.2000
 *
 * $Id: ltiPassiveWait.h,v 1.2 2005-07-22 15:59:25 doerfler Exp $
 */

#ifndef _LTI_PASSIVE_WAIT_H
#define _LTI_PASSIVE_WAIT_H

namespace lti {

  /**
   * This global function is a replacement in the %lti namespace for
   * the sleep and Sleep functions of Windows and Unix.  To avoid
   * misunderstandings the name has been chosen to be passiveWait instead of
   * "sleep", which has different meanings on the different systems.
   *
   * The time must always be given in microseconds, although the real
   * precision will be determined by the operating system.  (Windows uses
   * milliseconds and Unixes a few microseconds, depending on the
   * implementation of usleep.)
   *
   * @param usTime number of microseconds to wait passively
   */
  void passiveWait(const int usTime);

}

#endif

