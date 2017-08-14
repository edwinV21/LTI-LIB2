/*
 * Copyright (C) 1998-2006
 * Lehrstuhl fuer Technische Informatik, RWTH-Aachen, Germany
 * Instituto Tecnológico de Costa Rica, Costa Rica
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
 * \file   ltiGuiServer.h
 *         Contains the class lti::guiServer, which manages a singleton that
 *         controls the thread running the main loop of the used GUI Toolkit
 *         (like QT, GTK+, GTKMM, WxWidgets, etc.)
 * \author Pablo Alvarado
 * \date   3.1.2005
 *
 * revisions ..: $Id: ltiGuiServer.h,v 1.3 2008-05-26 23:45:45 alvarado Exp $
 */

#ifndef _LTI_GUI_SERVER_H_
#define _LTI_GUI_SERVER_H_

#include "ltiConfig.h"

#undef _LTI_HAVE_GUI

// the following code is only interesting if there is a GUI Toolkit available
// At this time only GTK+-2.0 is supported, but this may change in the
// future.
#if defined(HAVE_GTK) || defined(HAVE_QT) || defined(HAVE_WXWIDGETS)
#define _LTI_HAVE_GUI 1
#endif

#ifdef _LTI_HAVE_GUI

#include "ltiMutex.h"
#include "ltiThread.h"
#include "ltiSemaphore.h"
#include "ltiObject.h"
#include "ltiException.h"

namespace lti {

  /**
   * Class guiServer.
   *
   * The guiServer holds the one an only thread which executes the
   * main loop of the used GUI Toolkit, like GTK+, GTKMM, WxWidgets,
   * QT or MFC.  If you are just using the viewers and other GUI
   * related classes, this server should never come in your way.
   *
   * No one can create an instance of the guiServer, which is a
   * monostate object.  Some public static functions are provided to
   * check and alter the state of the main loop: 
   * - NotStarted 
   * - Running
   * - Stopped
   *
   * The server can be started only once in the whole program, and
   * this will occur automatically when you use any viewer or GUI
   * element in the library.  If you are writting a new GUI element,
   * then you have to ensure that the server is running just before
   * you have to interact with any interface related event. It is a
   * bad idea to start the server too early, since it will waste some
   * time.
   *
   * The stop() method is provided for completeness and debugging
   * purposes only.  This method should never ever be called, as the
   * main loop cannot be started again, due to the limitations of some
   * toolkits.
   *
   * The general implementation of the current classes is divided in
   * the files ltiGuiServer.cpp and lti???Server.cpp, where the latter
   * provides the toolkit specific issues (the methods toolkitInit(),
   * toolkitMainLoop() and toolkitStop()).
   * 
   * @ingroup gVisualization
   */
  class guiServer {
  public:
    /**
     * GUI Server State
     */
    enum eServerState {
      NotStarted, /**< The server has not been started yet.        */
      Running,    /**< The server is already running and attending 
                   *   GUI requests
                   */
      Stopped     /**< The server has been stopped.                */
    };

    /**
     * Start the server.
     *
     * This method returns until the server is running and ready to attend
     * requests.
     * 
     * @return true if successful, false otherwise (e.g. the server
     *         was manually stopped or is already running.)
     */
    static bool start();

    /**
     * Check for the server state.
     */
    static eServerState state();

    /**
     * Stop the server.
     *
     * If the server has not been started at all, this method does nothing,
     * but to return false, as it cannot be stopped!
     *
     * \warning If you stop the server, you cannot re-start it within your
     *          application.
     *          This method is public for debuggin purposes and for 
     *          completenes, but you should never call it, unless you really
     *          know what you are doing.
     *
     * @return true if the thread could be stopped or false if it was already
     *         stopped or if the server wasn't started at all.
     */
    static bool stop();

    /**
     * Returns true if called within the thread attending the main loop
     */
    static bool representsGuiThread();

  private:
    /**
     * Default constructor disabled
     */
    guiServer();

    /**
     * Disable copy constructor
     */
    guiServer(const guiServer& other);

    /**
     * Disable assignment operator
     */
    guiServer& operator=(const guiServer& other);

    /**
     * Disable destructor
     */
    virtual ~guiServer();

    /**
     * A unique instance of this class will run the event main loop of
     * the GUI toolkit.
     * 
     * @see guiServer
     */
    class guiThread : public thread {
    public:
      /**
       * Default constructor
       */
      guiThread();

      /**
       * Destructor
       */
      virtual ~guiThread();

      /**
       * Wait until the initialization of the GUI toolkit is ready
       */
      void start();

      /**
       * Stop the gui-Thread.
       */
      virtual void stop();

      /**
       * This method is the "slot" to be called when the GUI toolkit reaches
       * the normal main-loop, in order to signalize the current thread that
       * everything is ok.
       */
      void signalizeInitializationReady();

    protected:
      /**
       * The job to be independently executed in a new thread.  This
       * function has as task to call the toolkit initialization
       * routines (toolkitInit()) and the toolkit main-loop (toolkitMainLoop())
       */
      virtual void run();

      /**
       * Clean up the run() method
       */
      virtual void cleanUp();

      /**
       * Initializes GUI toolkit.
       * 
       * This function is called within the server-thread.
       */
      void toolkitInit();

      /**
       * Enters event handling routine of the toolkit
       * 
       * This function is called within the server-thread.
       */
      void toolkitMainLoop();

      /**
       * Tell the main-loop to stop.
       *
       * This function is called outside the server-thread and blocks
       * until the toolkit is really stopped.
       */
      void toolkitStop();

      /**
       * Semaphore used to indicate if initialization is ready.
       *
       * The counter of the semaphore is initialized with zero, which
       * means that any call to initReady_.wait() will block.  As soon as
       * the toolkit has been initialized, by some means this semaphore
       * has to be unlocked (initReady_.post()).
       */
      semaphore initReady_;

      /**
       * Semaphore used to indicate if finalization is ready.
       */
      semaphore finishReady_;

    private:
      /**
       * Disable copy constructor
       */
      guiThread(const guiThread& other);

      /**
       * Disable copy operator
       */
      guiThread& operator=(const guiThread& other);
    };

    /**
     * Pointer to the one and only thread object
     */
    static guiThread* thread_;

    /**
     * Protect access to member from public methods.
     */
    static mutex lock_;

    /**
     * State of the server
     */
    static eServerState serverState_;

    // the guiThread will want to change the state of this holder.
    friend class guiThread;

    /**
     * Each specific toolkit will need some private access.
     *
     * This enclosed class is private and knows about its sister class
     * guiThread, to which it may require some access.
     * 
     * Since it is still undefined, it is quite flexible to define its
     * interface depending on each toolkit.
     */
    class server;

    /**
     * Mark the thread as started 
     */
    static void markStarted();

    /**
     * Mark the thread as stopped in case of normal thread termination
     */
    static void markStopped();

    /**
     * atexit ensures that this function is called as soon as the main
     * program leaves its execution
     */
    static void stopForAtExit();
  };
}

#endif
#endif
