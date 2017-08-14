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

/* 
 * \file   ltiGuiServer.cpp
 *         Contains the class lti::guiServer, which contains a singleton that
 *         manages the thread running the main loop of the used GUI Toolkit 
 *         (like QT, GTK+, GTKMM, WxWidgets, etc.)
 * \author Pablo Alvarado
 * \date   3.1.2005
 *
 * revisions ..: $Id: ltiGuiServer.cpp,v 1.5 2012-06-10 02:02:20 alvarado Exp $
 */

#include "ltiGuiServer.h"
#include <cstdlib>

// ---------------------------------------------------------------------------
// Some Debuggin Macros

#undef _LTI_DEBUG
// #define _LTI_DEBUG 4
#include "ltiDebug.h"

// end of debugging macro definitions
// ---------------------------------------------------------------------------

#ifdef _LTI_HAVE_GUI

#ifdef _LTI_DEBUG
#include "ltiPassiveWait.h"

namespace std {
  // private method to print the eServerState type as string
  std::ostream& operator<<(std::ostream& s,
                           const lti::guiServer::eServerState& state) {
    switch (state) {
    case lti::guiServer::NotStarted:
      s << "NotStarted";
      break;
    case lti::guiServer::Running:
      s << "Running";
      break;
    case lti::guiServer::Stopped:
      s << "Stopped";
      break;
    }
    return s;
  }
  
}
#endif

namespace lti {
  // --------------------------------------------------
  // guiServer static attributes
  // --------------------------------------------------
  
  guiServer::guiThread* guiServer::thread_ = 0;
  mutex guiServer::lock_;
  guiServer::eServerState guiServer::serverState_ = guiServer::NotStarted;

  // --------------------------------------------------
  // guiServer
  // --------------------------------------------------

  // default constructor
  guiServer::guiServer() {
  }

  // copy constructor
  guiServer::guiServer(const guiServer&) {
  }

  // destructor
  guiServer::~guiServer() {
  }

  // alias for copy member
  guiServer& guiServer::operator=(const guiServer&) {
    return *this;
  }

  bool guiServer::start() {
    bool returnValue = false;
    if (serverState_ == NotStarted) { // preliminary check
      lock_.lock();
      if (serverState_ == NotStarted) { // now ensure that this really is not
                                        // started (the double-check technique)
        // the one and only thread object as a Meyers singleton
        static guiThread theThread;
        thread_ = &theThread;

        // ensure that at the end the gui thread is stopped
        static bool atexitFlag=false;
        if (!atexitFlag) {
          // this has to be done only once for the whole program
          atexit(stopForAtExit);
          atexitFlag=true;
        }

        // start the thread
        thread_->start(); // when the thread starts, the run method changes
                          // the state of the server.  This ensures it
                          // is changed only if everything is fine.
                          // This function blocks until the thread is
                          // really started.
        returnValue = (serverState_==Running);
      }
      lock_.unlock();
    }
    return returnValue;
  }

  guiServer::eServerState guiServer::state() {
    return serverState_;
  }

  bool guiServer::stop() {
    bool returnValue = false;
    if (serverState_ == Running) {
      lock_.lock();
      if (serverState_ == Running) {
        thread_->stop();
        thread_=0; // remove the reference to the thread
        returnValue=(serverState_==Stopped);
      }
      lock_.unlock();
    }
    return returnValue;
  }

  bool guiServer::representsGuiThread() {
    bool guiThread=false;
    if (serverState_ == Running) {
      lock_.lock();
      if (serverState_ == Running) {
        guiThread=thread_->representsCalledThread();
      }
      lock_.unlock();
    }
    return guiThread;
  }


  void guiServer::markStarted() {
    serverState_=Running;
  }

  void guiServer::markStopped() {
    serverState_=Stopped;
  }

  void guiServer::stopForAtExit() {
    _lti_debug4("guiServer::stopForAtExit in (state " << serverState_ << 
                ")\n");
    stop();
    _lti_debug4("guiServer::stopForAtExit out (state " << serverState_ <<
                ")\n");
  }

  // ------------------------------------------------------------------------
  // The GUI-Thread
  // ------------------------------------------------------------------------

  guiServer::guiThread::guiThread()
    : thread(),initReady_(0),finishReady_(0) {
  }

  guiServer::guiThread::guiThread(const guiThread&)
    : thread(),initReady_(0),finishReady_(0) {
  }

  guiServer::guiThread& 
  guiServer::guiThread::operator=(const guiThread&) {
    return *this;
  }
  
  guiServer::guiThread::~guiThread() {
  }

  void guiServer::guiThread::start() {
    if (!isAlive()) {
      _lti_debug4("GUI Thread start() Request {\n");
      thread::start();
      // wait until someone posts the semaphore within the main-loop
      initReady_.wait();
      _lti_debug4("} // GUI Thread start() Confirm\n");
    }
  }

  void guiServer::guiThread::stop() {
    if (isAlive()) {
      toolkitStop();
      finishReady_.wait(); // wait until the toolkit finishes its work
      // thread::stop(); // if we reach this point, is because the thread
                         // already ended, and therefore we don't really need
                         // to call the parent class method. 
    }
  }  

  void guiServer::guiThread::run() {
    _lti_debug4(" guiServer::guiThread::run() {\n");
    guiServer::markStarted();
    toolkitInit();
    toolkitMainLoop();
    guiServer::markStopped();
    _lti_debug4(" } // guiServer::guiThread::run()\n");
  }

  void guiServer::guiThread::cleanUp() {
    finishReady_.post();
  }

  void guiServer::guiThread::signalizeInitializationReady() {
    _lti_debug4("   Wait 3 sec for confirmation... ");
    _lti_if_debug4(passiveWait(3000000));
    _lti_debug4("done.\n");
    initReady_.post();
  }

}

#endif

