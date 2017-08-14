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
 * \file   ltiMutex.h
 *         Contains lti::mutex for protecting critical sections for 
 *         multithreading.
 * \author Thomas Rusert
 * \date   04.11.1999
 *
 * $Id: ltiMutex.h,v 1.6 2005-01-03 16:16:22 alvarado Exp $
 */

#ifndef _LTI_MUTEX_H_
#define _LTI_MUTEX_H_

#include "ltiMacroSymbols.h"

#ifndef _LTI_WIN32
#  include <pthread.h> // for unix/linux systems (posix threads!)
#else
#  include <windows.h>
#  include <process.h>
#endif

namespace lti {
  /** 
   * Mutex class for the library
   *
   * This object can be used to protect critical sections on multithreaded
   * applications.  The same thread should NOT try to lock the mutex more than
   * once.  The behavior of this will depend on the operating system: on
   * linux/unix the thread will be locked forever (posix standard); on
   * windows, the thread will count how many lock have been done, but it will
   * not be blocked by the later locks!
   *
   * Example:
   *
   * \code
   * // A class with some code to be protected
   * class A {
   * private:
   *   // The mutex used to protect the some code blocks
   *   lti::mutex lock_;
   *
   *   // Data that requires exclusive access, e.g. a std::list
   *   std::list<int> data_;
   *  
   * public:
   *   // A method that requires some exclusive access
   *   void access() {
   *     lock_.lock();   // ensure exclusive access to the data
   *     static int numAccesses = 0;
   *     data_.push_back(numAccesses++);
   *     lock_.unlock(); // realease exclusive access
   *   }
   * };
   * \endcode
   *
   * In the previous example the "access()" method ensures that only one thread
   * at a time has access to the data_ attribute of the class.  Other methods
   * that also access the data_ attribute should also protect the access. 
   *
   * @see lti::semaphore, lti::thread
   */
  class mutex {
  public:
    /**
     * Default constructor with unlocked mutex.
     */
    mutex();

    /**
     * Destructor
     */
    virtual ~mutex();

    /**
     * Wait until lock for mutex becomes available and lock it
     */
    void lock();

    /**
     * Try to lock mutex, but do not block.
     *
     * @return true if locking was successful. 
     */
    bool tryLock();

    /**
     * Unlock mutex
     */
    void unlock();

  protected:
    /**
     * Destroy the mutex
     */
    void destroy();

# ifndef _LTI_WIN32
    /**
     * the posix mutex object
     */
    pthread_mutex_t theMutex_;
# else
    /**
     * the WIN32 mutex objects
     */
    HANDLE theMutex_;
# endif
  };
}

#endif

