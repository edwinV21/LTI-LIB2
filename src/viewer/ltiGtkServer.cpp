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
 * \file   ltiGtkServer.cpp
 *         Contains the class lti::guiServer, but only the GTK+ specifics
 * \author Pablo Alvarado
 * \date   3.1.2005
 *
 * revisions ..: $Id: ltiGtkServer.cpp,v 1.10 2014-04-25 22:21:15 alvarado Exp $
 */

#include "ltiGuiServer.h"

#ifdef _LTI_HAVE_GUI
#ifdef HAVE_GTK

#include <gtk/gtk.h>
#include <iostream>
#include <cstring>


// ---------------------------------------------------------------------------
// Some Debuggin Macros

#undef _LTI_DEBUG
// #define _LTI_DEBUG 4
#include "ltiDebug.h"

// end of debugging macro definitions
// ---------------------------------------------------------------------------


// Since GTK+ is a C library, we have to ignore here several standards of
// the LTI-Lib: the most common is the use of reinterpret_cast<>, as we need
// to force the proper type of the pointers (gpointer=void*) into a current
// class.  Note that all of this is hidden from the outer world.

namespace lti {

  /*
   * Private class that controls the GTK-Main-Loop start and end routines.
   */
  class guiServer::server {
  public:
    /**
     * Function called within the main-loop, as soon as it starts
     */
    static gboolean mainLoopRunning(gpointer data);

    /**
     * Function called to stop the main-loop
     */
    static gboolean mainLoopStop(gpointer data);
  };

  // init confirm
  gboolean guiServer::server::mainLoopRunning(gpointer data) {
    _lti_debug4("   guiServer::server::mainLoopRunning called\n");

    if (data != 0) {
      guiServer::guiThread* theThread = 
        reinterpret_cast<guiServer::guiThread*>(data);
      
      _lti_debug4("   signalizing init ready\n");
      theThread->signalizeInitializationReady();
    }

    // 'false' means that this function has to be removed as soon as 
    // the main loop finishes its execution.
    // 'true' would mean this function should be called as soon as the
    // main loop is idle again!
    return false;
  }

  gboolean guiServer::server::mainLoopStop(gpointer) {
    // The warning of "data" unused cannot be solved as this is
    // the gtk defined interface.
    _lti_debug4("   guiServer::server::mainLoopStop called\n");
    gtk_main_quit();
    return false; // false => remove from the idle functions when ready
  }

  // ------------------------------------------------------------------------
  // The GUI-Thread
  // ------------------------------------------------------------------------

  void guiServer::guiThread::toolkitInit() {
    // this functions is called from within the run() method of the
    // gui thread.

#ifdef _DEBUG
    static int argc=6;
    static const char* cargv[] = {"LTI-Lib-2",
                                  "--gtk-debug","misc",
                                  "--gdk-debug","misc",
				  "--sync",0};
#else
    static int argc=1;
    static const char* cargv[] = {"LTI-Lib-2",0};
#endif

    _lti_debug4("  guiServer::guiThread::toolkitInit() {\n");

    // convert the const char** to char**
    char** argv = new char*[argc+1];
    int i;
    for (i=0;i<argc;++i) {
      argv[i] = new char[strlen(cargv[i])+1];
      strcpy(argv[i],cargv[i]);
    }
    argv[i]=0;

    // initialize thread support of GTK
#if (GTK_MAJOR_VERSION==2) && (GTK_MINOR_VERSION < 24)
    // Deprecated since GTK 2.32
    _lti_debug4("  g_thread_init()\n");
    g_thread_init (NULL);
    _lti_debug4("  gdk_threads_init()\n");
#endif

    gdk_threads_init();

    // protect all the next functions
    gdk_threads_enter();

    // avoid setting the locale, since it should be managed outside of the 
    // LTI-Lib
    gtk_disable_setlocale();
    
    /* 
     * This is called in all GTK applications. Arguments are parsed
     * from the command line and are returned to the application.
     */
    gtk_init(&argc,&argv);

    /*
     * We are going to use many RGB data!
     */
    gdk_rgb_init();

    /*
     * Inscribe a function to be called as soon as the main-loop is up and
     * running.
     */
    g_idle_add(guiServer::server::mainLoopRunning,this);

    // remove allocated memory
    for (i=0;i<argc;++i) {
      delete[] argv[i];
      argv[i]=0;
    }
    delete[] argv;
    argv=0;
    _lti_debug4("  } // guiServer::guiThread::toolkitInit()\n");
  }

  void guiServer::guiThread::toolkitMainLoop() {
    _lti_debug4("  guiServer::guiThread::toolkitMainLoop() {\n");
    //gdk_threads_enter(); // already done just after init of threads
    gtk_main();
    gdk_threads_leave();
    _lti_debug4("  } // guiServer::guiThread::toolkitMainLoop()\n");
  }

  void guiServer::guiThread::toolkitStop() {
    _lti_debug4("  guiServer::guiThread::toolkitStop()\n");
    g_idle_add(guiServer::server::mainLoopStop,this);
  }

}

#endif
#endif
