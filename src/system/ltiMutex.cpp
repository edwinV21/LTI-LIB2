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
 * \file   ltiMutex.cpp
 * \author Thomas Rusert
 * \date   04.11.1999
 *
 * $Id: ltiMutex.cpp,v 1.3 2005-01-03 16:16:22 alvarado Exp $
 */

#include "ltiMutex.h"
#include "ltiAssert.h"
#include <errno.h>

namespace lti {

  // --------------------------------------------------------
  //                        UNIX/LINUX
  // --------------------------------------------------------

# ifndef _LTI_WIN32

  // default constructor
  mutex::mutex() {
    pthread_mutex_init(&theMutex_,0);
  }

  // destructor
  mutex::~mutex() {
    destroy();
  }

  // wait until lock for mutex becomes available
  void mutex::lock() {
    pthread_mutex_lock(&theMutex_);
  }

  // try to lock mutex, but do not block
  bool mutex::tryLock() {
    return (pthread_mutex_trylock(&theMutex_)==0);
  }

  // unlock mutex
  void mutex::unlock() {
    pthread_mutex_unlock(&theMutex_);
  }

  // destroy mutex
  void mutex::destroy() {
    int status;
    while((status=pthread_mutex_destroy(&theMutex_))!=0) {
      assert( status==EBUSY );
      unlock();
    }
    pthread_mutex_destroy(&theMutex_);
  }

# else

  // --------------------------------------------------------
  //                        WINDOWS
  // --------------------------------------------------------

  // default constructor
  mutex::mutex() {
    theMutex_ = CreateMutex(NULL,FALSE,NULL);
  }

  // destructor
  mutex::~mutex() {
    destroy();
    CloseHandle(theMutex_);
  }

  // wait until lock for mutex becomes available
  void mutex::lock() {
    WaitForSingleObject(theMutex_,INFINITE);
  }

  // try to lock mutex, but do not block
  bool mutex::tryLock() {
    return (WaitForSingleObject(theMutex_,0) == WAIT_OBJECT_0);
  }

  // unlock mutex
  void mutex::unlock() {
    ReleaseMutex(theMutex_);
  }

  // destroy mutex
  void mutex::destroy() {
    unlock();
  }

# endif
}

