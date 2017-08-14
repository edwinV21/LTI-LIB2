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
 * \file   ltiSemaphore.h
 *         Contains the lti::semaphore wrapper class to encapulate the
 *         differences between the POSIX and Win32 concepts.
 * \author Thomas Rusert
 * \date   04.11.1999
 *
 * $Id: ltiSemaphore.h,v 1.6 2007-12-25 19:56:20 alvarado Exp $
 */


#ifndef _LTI_SEMAPHORE_H_
#define _LTI_SEMAPHORE_H_

#include "ltiObject.h"
#include "ltiMacroSymbols.h"

#ifndef _LTI_WIN32
#  include <semaphore.h>
#else
#  include <windows.h>
#  include <process.h>
#  define SEM_VALUE_MAX 16777215 // (2^24-1)
#endif

namespace lti {
  /**
   * Class for inter-thread synchronisation concept of semaphores.
   *
   * The difference between semaphores and mutexes is that the latter one only
   * tracks if the mutex has been locked or not, while the semaphore counts
   * the number of "locks" (called here "waits()") and requires the same
   * number of unlocks (here "posts()") to release the access.
   *
   * This class is in principle a wrapper for POSIX semaphores and behaves like
   * them (under Windows, the same behaviour is simulated): a semaphore holds
   * an integer whose value is never allowed to fall below zero.  Two
   * operations can be performed on semaphores: increment the semaphore value
   * by one (with post()) and decrement the semaphore value by one (with
   * wait()).  If the value of a semaphore is currently zero, then a
   * wait() operation will block until the value becomes greater than
   * zero (by enough post() calls).
   *
   * @see lti::mutex, lti::thread
   */
  class semaphore {
  public:
    /**
     * Default constructor.
     *
     * The semaphore is locked when the counter reaches the value zero, which
     * means that in construction time you decide how far from locking the
     * semaphore is (for values greater than zero) or if it is from the
     * beginning unlocked (for value zero).
     *
     * @param initialValue initial value for the semaphore counter.
     */
    semaphore(const int initialValue = 1);

    /**
     * Destructor
     */
    virtual ~semaphore();

    /**
     * Wait on semaphore.
     *
     * Wait on semaphore, i.e. decrease the value or wait if counter <= 0
     *
     * @return true if successful, false otherwise
     */
    bool wait();

    /**
     * Try to wait on semaphore, but do not block.
     *
     * If this function returns true, then you can get access to the resource
     * you are trying to protect, but if it returns false, then you may not
     * assume you have rights to do that.
     *
     * @return true if value was decreased.
     */
    bool tryWait();

    /**
     * Post semaphore.
     *
     * Post semaphore, i.e. increase its value, unlocking if it reaches zero.
     *
     * @return true if successful, false otherwise
     */
    bool post();

    /**
     * Get current value. 
     *
     * The returned value will be usually lower than zero or zero if
     * the semaphore has been locked.
     */
    int getValue();

    /**
     * Reset value to initialValue
     */
    void reset();

  protected:
    /**
     * Destroy the semaphore
     */
    void destroy();

    /**
     * Initial semaphore value.
     *
     * This value is required to allow resetting the semaphore
     */
    int initValue;

# ifndef _LTI_WIN32
    /**
     * The posix semaphore
     */
    sem_t theSemaphore;
# else
    /**
     * The WIN32 semaphore object
     */
    HANDLE theSemaphore;
    
    /**
     * Counter of the semaphore
     */
    int counter;
# endif
  };
}

#endif

