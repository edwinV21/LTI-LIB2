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
 * \file   ltiSemaphore.cpp
 *         Contains the lti::semaphore wrapper class to encapulate the
 *         differences between the POSIX and Win32 concepts.
 * \author Thomas Rusert
 * \date   04.11.1999
 *
 * $Id: ltiSemaphore.cpp,v 1.5 2009-08-29 04:20:53 alvarado Exp $
 */

#include "ltiSemaphore.h"
#include "ltiAssert.h"
#include "ltiException.h"
#include <errno.h>

namespace lti {

  // --------------------------------------------------------
  //                        UNIX/LINUX
  // --------------------------------------------------------

# ifndef _LTI_WIN32

# ifdef _LTI_GNUC_4_3
  // in gcc 4.3.1 there is no SEM_VALUE_MAX defined, so we will import
  // the place where is now defined
#   include <climits>
# endif

  // default constructor
  semaphore::semaphore(const int initialValue) :
    initValue(initialValue) {
    assert( initValue<SEM_VALUE_MAX );
    sem_init(&theSemaphore,0,initValue);
  }

  // destructor
  semaphore::~semaphore() {
    destroy();
  }

  // wait on semaphore, i.e. decrease the value or wait if counter <= 0
  bool semaphore::wait() {
    return (sem_wait(&theSemaphore) == 0);
  }

  // try to wait on semaphore, but do not block
  bool semaphore::tryWait() {
    return (sem_trywait(&theSemaphore)==0);
  }

  // post semaphore, i.e. increase the value
  bool semaphore::post() {
    return (sem_post(&theSemaphore) == 0);
  }

  // get current value
  int semaphore::getValue() {
    int value;
    sem_getvalue(&theSemaphore,&value);
    return value;
  }

  // reset value to initialValue
  void semaphore::reset() {
    destroy();
    sem_init(&theSemaphore,0,initValue);
  }

  // destroy semaphore
  void semaphore::destroy() {
    while(sem_destroy(&theSemaphore)!=0) {
      assert( errno==EBUSY );
      while(getValue()<=0) {
        post();
      }
    }
    sem_destroy(&theSemaphore);
  }

# else
  // --------------------------------------------------------
  //                        WINDOWS
  // --------------------------------------------------------

  // default constructor
  semaphore::semaphore(const int initialValue)
    : initValue(initialValue),counter(initialValue) {
    theSemaphore =
       CreateSemaphore(NULL,initialValue,SEM_VALUE_MAX,NULL);
    if (theSemaphore == NULL) {
      //unsigned int error;
      //error = GetLastError();
      throw exception("Semaphore cannot be created!");
    }
  }

  // destructor
  semaphore::~semaphore() {
    destroy();
  }

  // wait until lock for semaphore becomes available
  bool semaphore::wait() {
    counter--;
    return (WaitForSingleObject(theSemaphore,INFINITE) != WAIT_FAILED);
  }

  // try to lock semaphore, but do not block
  bool semaphore::tryWait() {
    if (WaitForSingleObject(theSemaphore,0) == WAIT_OBJECT_0) {
      counter--;
      return true;
    } else {
      return false;
    };
  }

  // unlock semaphore
  bool semaphore::post() {
    // TODO: This value might be invalid:
    counter++;
    return (ReleaseSemaphore(theSemaphore,1,NULL) != 0);
  }

  // destroy semaphore
  void semaphore::destroy() {

    while(!tryWait() && (counter < initValue) ) {
      post();
    }

    post();

    CloseHandle(theSemaphore);
  }
  // get current value
  int semaphore::getValue() {
    return counter;
  }

  // reset value to initialValue
  void semaphore::reset() {
    destroy();
    theSemaphore = CreateSemaphore(NULL,initValue,SEM_VALUE_MAX,NULL);
  }

# endif
}

