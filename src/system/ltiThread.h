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
 * \file   ltiThread.h
 *         Contains the lti::thread wrapper class to encapulate the differences
 *         between the POSIX threads in *ix systems and the Win32 concepts.
 * \author Thomas Rusert
 * \author Pablo Alvarado
 * \date   04.11.1999
 *
 * $Id: ltiThread.h,v 1.5 2005-04-20 16:02:17 doerfler Exp $
 */

#ifndef _LTI_THREAD_H_
#define _LTI_THREAD_H_

#include "ltiMutex.h"
#include "ltiSemaphore.h"
#include "ltiMacroSymbols.h"

#ifndef _LTI_WIN32
#  include <pthread.h>
#else
#  include <winbase.h>
#endif

namespace lti {

  /**
   * LTI-Lib thread class.
   *
   * Representiation of a system thread (currently a posix thread for Unix
   * systems, and WIN32 thread for windows systems)
   *
   * If you need a thread, just inherit from this class and reimplement the
   * method run():
   * \code
   * class myThread : public thread {
   * protected:
   *   void run() {
   *     // your thread's job here!
   *   }
   * };
   *
   * ...
   *
   * // somewhere else in your code:
   *
   * myThread aThread;
   * aThread.start(); // do your threads job...
   * // continue with other things (your thread runs in parallel...)
   * \endcode
   */
  class thread {
  public:
    /**
     * Default constructor
     */
    thread();

    /**
     * Destructor.
     *
     * If the thread is still executing while destroying the thread object,
     * the thread will be forced to stop. Use join() to wait for the thread
     * to terminate itself.
     */
    virtual ~thread();

    /**
     * Start thread.
     *
     * This method is called by the parent thread, to start in a
     * second thread the execution of the run() method, which is overloaded
     * in the derived classes.
     */
    virtual void start();

    /**
     * Stop thread execution.
     * 
     * Forces the thread to stop executing.
     *
     * Overload this function to cleanup things that the run() method
     * could leave unfinished after thread cancellation.
     */
    virtual void stop();

    /**
     * Wait for thread termination.
     *
     * This has to be called by a thread different than the one
     * executing the run() method, in order to wait that the run()
     * method normally finishes.
     */
    virtual void join();

    /**
     * Check if thread is alive.
     *
     * Test if the thread that should execute the run() method is still alive.
     *
     * @return true if the thread is alive, or false if it was already
     * stopped or finished.
     */
    bool isAlive() const;

    /**
     * Check which thread is calling.
     *
     * Returns whether the thread that calls this method represents the
     * thread executing the run() method (i.e. if the method
     * is directly or indirectly called within the run() method).
     *
     * @return true if the thread executing the run() method is the one that
     *              called the current method, or false otherwise.
     */
    bool representsCalledThread() const;

  protected:
    /**
     * Thread execution method.
     *
     * The whole thread object is somehow just a representation for
     * this method, since the created thread will just call it, (which
     * means will execute the overloaded run() method of the derived
     * class).  The thread will also persist as long as this method
     * does, i.e. the thread will stop when the method finishes.
     */
    virtual void run() = 0;

    /**
     * Clean up the thread.
     *
     * Method to be called when finishing regulary or cancelling the thread;
     * something like a destructor for the run() method.
     *
     * The default behaviour, if not overloaded, is do nothing.
     */
    virtual void cleanUp() {};

  private:
    /**
     * The function to be called by the OS to clean up the thread. 
     *
     * It will be called with the instance of the object as a parameter.
     * The static interface allows to use it as a C function.
     */
    static void clean(void* threadObject);

    /**
     * Flag to indicate if the thread is alive or not.
     */
    bool alive;

    /**
     * Mutex to protect the access to the multithreading functions.
     */
    static mutex startMutex;

    /**
     * Semaphore used to join called thread with the calling one
     */
    semaphore suspendSem;

# ifndef _LTI_WIN32 // UNIX/LINUX
    /**
     * The real C function called as the POSIX thread standard
     * requires.  It just wrapps calling the method run() of the given
     * instance.
     */
    static void* execute(void* threadObject);

    /**
     * Handler of the created POSIX thread.
     */
    pthread_t theThread;
# else          // WINDOWS
    /**
     * The real C function called as the Win32 thread interface
     * requires.  It just wrapps calling the method run() of the given
     * instance.
     */    
    static void execute(void* threadObject);

    /**
     * Windows handle of the thread.
     */
    HANDLE theThread;

    /**
     * ID of the called thread
     */
    unsigned int calledThreadId;
# endif
  };

}

#endif

