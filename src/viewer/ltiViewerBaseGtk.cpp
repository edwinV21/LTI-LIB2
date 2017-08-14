/*
 * Copyright (C) 1998-2006
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
 * \file    ltiViewerBaseGtk.cpp
 * \author Pablo Alvarado
 * \date    05.07.2005
 * 
 * $Id: ltiViewerBaseGtk.cpp,v 1.6 2012-09-16 05:05:59 alvarado Exp $
 */

#include "ltiConfig.h"

#ifdef HAVE_GTK

#include "ltiViewerBaseGtk.h"
#include "ltiGuiServer.h"
#include "ltiRound.h"

// ---------------------------------------------------------------------------
// Some Debuggin Macros

#undef _LTI_DEBUG
// #define _LTI_DEBUG 4
#include "ltiDebug.h"

// end of debugging macro definitions
// ---------------------------------------------------------------------------


namespace lti {

  // ------------------------------------------------------------------------
  //   Main Window's toolkit
  // ------------------------------------------------------------------------
  
  viewerBase::mainWindow::tk::tk(viewerBase::mainWindow& parent) :
    params_(0),
    parent_(&parent),
    winShown_(false),
    winBuild_(false),
    borderOffset_(-4,-29), // magic numbers empirically determined
    window_(0),
    scrolledWindow_(0),
    statusBarContextId_(0),
    drawingArea_(0),
    eventBox_(0) {
    _lti_enterCTOR();
    _lti_leaveCTOR();
  }

  viewerBase::mainWindow::tk::~tk() {
    _lti_enterDTOR();
    hide();
    destroy();
    _lti_leaveDTOR();
  }

  bool viewerBase::mainWindow::tk::build() {
    _lti_debug2("entering viewerBase::mainWindow::tk::build()\n");

    if (winBuild_) {
      // window already build!  This should happend, but it has!
      return false;
    }

    if (guiServer::state() != guiServer::Running) {
      guiServer::start();
    }


    lock_.lock();
    gdk_threads_enter();

    // The main viewer window is a dialog one 
    window_ = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    
    // The main container in the window
    vbox_ = gtk_vbox_new(false,0); // homogeneous=false, spacing=0

    // include the vbox_ in the widget
    gtk_container_add(GTK_CONTAINER(window_),vbox_);

    // create a new scrolled window. 
    scrolledWindow_ = gtk_scrolled_window_new (NULL, NULL);

    // add the scrolled window to the main window
    // TRUE,TRUE,0 means Expand=true,Fill=true,Padding=0
    gtk_box_pack_start (GTK_BOX (vbox_), scrolledWindow_, TRUE, TRUE, 0);

    /* the policy is one of GTK_POLICY AUTOMATIC, or GTK_POLICY_ALWAYS.
     * GTK_POLICY_AUTOMATIC will automatically decide whether you need
     * scrollbars, whereas GTK_POLICY_ALWAYS will always leave the scrollbars
     * there.  The first one is the horizontal scrollbar, the second,
     * the vertical. */
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolledWindow_),
                                    GTK_POLICY_AUTOMATIC,
                                    GTK_POLICY_AUTOMATIC);

    // Status bar 
    statusBar_ = gtk_statusbar_new();

    // Add the status bar at the bottom of the window
    gtk_box_pack_start(GTK_BOX(vbox_),statusBar_,FALSE,FALSE,0);

    // Eliminate resize grip, because it is not necessary
    gtk_statusbar_set_has_resize_grip(GTK_STATUSBAR(statusBar_),false);

    statusBarContextId_ =
      gtk_statusbar_get_context_id(GTK_STATUSBAR(statusBar_),
                                   "viewerBaseStatusBar");

    // drawing area 
    drawingArea_ = gtk_drawing_area_new();

    // set the minimum allowable size for the 64,64 
    const ipoint pt = parent_->getCanvasSize();
    gtk_widget_set_size_request(drawingArea_, pt.x, pt.y);
    
    // set the minimum allowable size for the window 
    gtk_widget_set_size_request(window_, 64, 64);

    // event box 
    eventBox_ = gtk_event_box_new();

    // TODO: with_viewport?
    gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(scrolledWindow_),
                                          eventBox_);

    gtk_container_add(GTK_CONTAINER (eventBox_),drawingArea_);
    gtk_widget_set_events(eventBox_, ( GDK_EXPOSURE_MASK        |
                                    // GDK_POINTER_MOTION_MASK  |
                                       GDK_BUTTON_MOTION_MASK	|
                                       GDK_BUTTON1_MOTION_MASK	|
                                       GDK_BUTTON2_MOTION_MASK	|
                                       GDK_BUTTON3_MOTION_MASK	|
                                       GDK_SCROLL_MASK          |
                                       GDK_BUTTON_PRESS_MASK	|
                                       GDK_BUTTON_RELEASE_MASK	|
                                       GDK_FOCUS_CHANGE_MASK    |
                                       GDK_VISIBILITY_NOTIFY_MASK ) );


    // the signals used here
    g_signal_connect (G_OBJECT (window_), "delete_event",
                      G_CALLBACK (cbCloseWindow), this);
    g_signal_connect (G_OBJECT (window_), "configure-event",
                      G_CALLBACK (cbGeometryChanged), this);
    g_signal_connect (G_OBJECT (drawingArea_), "expose-event",
                      G_CALLBACK (cbDrawingAreaExposed), this);
    g_signal_connect (G_OBJECT (eventBox_), "motion_notify_event",
                      G_CALLBACK (cbMouseMoved), this);
    g_signal_connect (G_OBJECT (eventBox_), "button_press_event",
                      G_CALLBACK (cbButtonPressed), this);
    g_signal_connect (G_OBJECT (eventBox_), "button_release_event",
                      G_CALLBACK (cbButtonReleased), this);
    g_signal_connect (G_OBJECT (eventBox_), "scroll_event",
                      G_CALLBACK (cbScrolled), this);
    // end of signals

    gtk_widget_realize (eventBox_);

    // possible cursors
    // GDK_X_CURSOR   (X Cross, but relativ thick)
    // GDK_ARROW      (Arrow pointing up + right)
    // GDK_CENTER_PTR (Arrow pointing up!)
    // GDK_CIRCLE     (black circle, the pointer is in its middle)
    // GDK_CROSS      (A + similar cross, the traditional one)
    // GDK_HAND1      (Black right hand pointing up + right)
    // GDK_HAND2      (White right hand pointing up + left)
    // GDK_DOTBOX
    cursor_ = gdk_cursor_new (GDK_CROSS);
    gdk_window_set_cursor(eventBox_->window, cursor_);

    // activate key detection
    gtk_widget_add_events(window_, ( GDK_KEY_PRESS_MASK |
                                     GDK_KEY_RELEASE_MASK ) );

    g_signal_connect (G_OBJECT (window_), "key_press_event",
                      G_CALLBACK (cbKeyPressed), this);

    g_signal_connect (G_OBJECT (window_), "key_release_event",
                      G_CALLBACK (cbKeyReleased), this);

    winBuild_=true;

    gdk_flush();
    gdk_threads_leave();
    lock_.unlock();
    _lti_debug2("leaving viewerBase::mainWindow::tk::build()\n");

    return true;
  }

  gboolean viewerBase::mainWindow::tk::cbShow(gpointer data) {
    viewerBase::mainWindow::tk* me = 
      reinterpret_cast<viewerBase::mainWindow::tk*>(data);
    me->show();
    return false; // false => remove from the idle functions when ready
  }

  bool viewerBase::mainWindow::tk::show() {
    if (notNull(window_)) {
      // if we get here then we can assume that the window is already build
      if (guiServer::representsGuiThread()) {
        _lti_debug2("enter viewerBase::mainWindow::tk::show() (gui th.)\n");
        if (!winShown_) {
          _lti_debug4("  showing window\n");
          gdk_threads_enter();

          ipoint csize = parent_->computeCanvasSize();
          
          if (csize != parent_->getCanvasSize()) {
            parent_->setCanvasSize(csize);
          }

          // this is the task if called within the gui thread
          gtk_widget_show_all(window_);

          // use info in the parameters if they exist
          if (notNull(params_)) {
            if ((params_->size.x<0) || (params_->size.y<0)) {
              // lower classes have to take care of the automatic resizing.
              // here we can only move the window

              ipoint sizeSuggestion = suggestWindowSize();
              if ((sizeSuggestion.x > 0) && (sizeSuggestion.y > 0)) {
                gdk_window_move_resize(window_->window,
                                       params_->position.x,
                                       params_->position.y,
                                       sizeSuggestion.x+abs(borderOffset_.x),
                                       sizeSuggestion.y+abs(borderOffset_.y));
              } else {
                gdk_window_move(window_->window,
                                params_->position.x,
                                params_->position.y);
              }
            } else {
              gdk_window_move_resize(window_->window,
                                     params_->position.x,
                                     params_->position.y,
                                     params_->size.x+abs(borderOffset_.x),
                                     params_->size.y+abs(borderOffset_.y));
            }
            gtk_window_set_title(GTK_WINDOW(window_),params_->title.c_str());
          }

          gdk_flush();
          gdk_threads_leave();
          winShown_=true;
          _lti_debug4("  finish showing window\n");
        } else {
          _lti_debug4("  window already shown, recomputing size\n");
          ipoint csize = parent_->computeCanvasSize();
          
          if (csize != parent_->getCanvasSize()) {
            parent_->setCanvasSize(csize);
          }

          forceRedraw();
          _lti_debug4("  window already shown, forceRedraw done.\n");
        }
      } else {
        _lti_debug2("enter viewerBase::mainWindow::tk::show()(not gui th.)\n");
        // called from another thread, so we have to activate a callback
        g_idle_add(cbShow,this);
      }
      return true;
    }
    return false;
  }

  bool viewerBase::mainWindow::tk::hide() {
    lock_.lock();
    if (notNull(window_)) {
      // This method can be explicitely (indirectly) be called by a
      // users who wants to hide the window from his C++ code, but also
      // from the GUI if he selects the "close" icon (usually an X).
      // In the first case we have to protect the GTK, in the second everything
      // occurs within the proper thread.
      _lti_debug2("enter viewerBase::mainWindow::tk::hide() ");
      if (guiServer::representsGuiThread()) {
        _lti_debug2("(in gui)\n");
        
        gtk_widget_hide_all(window_);
      } else {
        _lti_debug2("(not gui)\n");
        gdk_threads_enter();
        gtk_widget_hide_all(window_);
        gdk_flush();
        gdk_threads_leave();
      }
      winShown_=false;
      lock_.unlock();
      return true;
    }
    lock_.unlock();
    return false;
  }

  bool viewerBase::mainWindow::tk::destroy() {
    if (hide()) {
      lock_.lock();
      _lti_debug2("enter viewerBase::mainWindow::tk::destroy()\n");
      if (guiServer::representsGuiThread()) {
        gtk_widget_destroy(GTK_WIDGET(window_));
        gdk_cursor_unref(cursor_);
      } else {
        gdk_threads_enter();
        gtk_widget_destroy(GTK_WIDGET(window_));
        gdk_cursor_unref(cursor_);
        gdk_flush();
        gdk_threads_leave();
      }
      winBuild_=false;
      params_=0;
      // set pointers to zero
      window_             = 0;
      scrolledWindow_     = 0;
      statusBar_          = 0;
      statusBarContextId_ = 0;
      drawingArea_        = 0;
      eventBox_           = 0;
      cursor_             = 0;
      lock_.unlock();
      return true;
    }

    return false;
  }

  bool viewerBase::mainWindow::tk::statusBar(const std::string& txt) {
    lock_.lock();
    if (notNull(window_)) {
      // This method can be explicitely (indirectly) be called by a
      // users who wants to display text from his C++ code, but also
      // from the GUI if it just has some info to be shown.
      if (guiServer::representsGuiThread()) {
        gtk_statusbar_pop(GTK_STATUSBAR(statusBar_),statusBarContextId_);
        gtk_statusbar_push(GTK_STATUSBAR(statusBar_),
                           statusBarContextId_,txt.c_str());
      } else {
        gdk_threads_enter();
        gtk_statusbar_pop(GTK_STATUSBAR(statusBar_),statusBarContextId_);
        gtk_statusbar_push(GTK_STATUSBAR(statusBar_),
                           statusBarContextId_,txt.c_str());
        gdk_flush();
        gdk_threads_leave();
      }
      lock_.unlock();
      return true;
    }
    lock_.unlock();
    return false;    
  }

  viewerBase::mainWindow::tk& 
  viewerBase::mainWindow::tk::copy(const tk& ) {
    // TODO: what to do?  deep copy of parameters?  maybe the tk should
    // never be copied... yet to be decided...
    
    return *this;
  }

  bool viewerBase::mainWindow::tk::setPosition(const ipoint& p) {

    if (winShown_) {
      gtk_window_move(GTK_WINDOW(window_),p.x,p.y);
      // the parameters are updated when attending the configure-event in
      // the method cbGeometryChanged()
      return true;
    }
    return false;
  }

  ipoint viewerBase::mainWindow::tk::getPosition() const {
    if (notNull(params_)) {
      return params_->position;
    }
    return ipoint(0,0); // TODO: define an error position
  }

  bool viewerBase::mainWindow::tk::setSize(const ipoint& p) {

    if (winShown_) {
      // the size given means the drawing area, so we have to add 
      // the offsets
      gtk_window_resize(GTK_WINDOW(window_),
                        p.x+abs(borderOffset_.x),
                        p.y+abs(borderOffset_.y));
      // the parameters are updated when attending the configure-event in
      // the method cbGeometryChanged()
      return true;
    }

    return false;
  }

  ipoint viewerBase::mainWindow::tk::getSize() const {
    if (notNull(params_)) {
      return params_->size;
    }
    return ipoint(-1,-1); // error size is negative
  }

  ipoint viewerBase::mainWindow::tk::suggestWindowSize() const {
    return ipoint(-1,-1);
  }

  bool viewerBase::mainWindow::tk::setTitle(const std::string& title) {
    if (notNull(window_)) {
      gtk_window_set_title (GTK_WINDOW (window_), title.c_str());
      params_->title = title;
      return true;
    }
    return false;
  }

  std::string viewerBase::mainWindow::tk::getTitle() const {
    if (notNull(params_)) {
      return params_->title;
    }
    return "Unknown"; 
  }

  

  bool 
  viewerBase::mainWindow::tk::updateParameters(viewerBase::parameters& param) {
    _lti_debug2("Enter viewerBase::mainWindow::tk::updateParameters\n");
    params_=&param;
    if (winBuild_ && winShown_) {
      lock_.lock();
      const bool guiThread = guiServer::representsGuiThread();

      if (!guiThread) {
        gdk_threads_enter();
      }

      if ((param.size.x > 0) && (param.size.y > 0)) {
        // we avoid calling the methods
        //   setPosition(param.position);
        //   setSize(param.size);
        // to avoid 2 events.  Instead we generate one single event with:
        gdk_window_move_resize(window_->window,
                               param.position.x,
                               param.position.y,
                               param.size.x+abs(borderOffset_.x),
                               param.size.y+abs(borderOffset_.y));        
      } else {
        setPosition(param.position);
      }

      gtk_window_set_title(GTK_WINDOW(window_),param.title.c_str());

      // maybe we have a new canvas size
      ipoint csize = parent_->computeCanvasSize();
      
      if (csize != parent_->getCanvasSize()) {
        parent_->setCanvasSize(csize);
      }

      forceRedraw();

      if (!guiThread) {
        gdk_flush();
        gdk_threads_leave();
      }

      lock_.unlock();

    }
    return true;
  }
  
  gint viewerBase::mainWindow::tk::cbCloseWindow(GtkWidget *,
                                                 GdkEvent  *,
                                                 gpointer   data) {
    viewerBase::mainWindow::tk* me = 
      reinterpret_cast<viewerBase::mainWindow::tk*>(data);

    me->closeWindow();


    /*
     * Change TRUE to FALSE and the main window will be destroyed with
     * a "delete_event".
     */
    return(TRUE); // TRUE indicates that the event has been handled
  }

  bool viewerBase::mainWindow::tk::closeWindow() {
    // hide the window
    return parent_->closeWindow();
  }

  gint viewerBase::mainWindow::tk::cbGeometryChanged(GtkWidget*,
                                                     GdkEventConfigure* event,
                                                     gpointer           data) {
    viewerBase::mainWindow::tk* me = 
      reinterpret_cast<viewerBase::mainWindow::tk*>(data);
    
    me->geometryChanged(ipoint(event->x,event->y),
                        ipoint(event->width,event->height));
    
    return (FALSE); // continue normal event handling
                    // TRUE to indicate that the event handling is complete

  }
 
  bool viewerBase::mainWindow::tk::geometryChanged(const ipoint pos,
                                                   const ipoint size) {


    _lti_debug4("viewerBase::mainWindow::tk::geometryChanged(" <<
                pos << " , " << size << ")\n");

    if (notNull(params_)) {
      
      // transfer information to the parameters
      params_->position=pos;
      params_->size.x=size.x-abs(borderOffset_.x);
      params_->size.y=size.y-abs(borderOffset_.y);

      return true;
    }

    return false;
  }

  gint 
  viewerBase::mainWindow::tk::cbDrawingAreaExposed(GtkWidget*,
                                                   GdkEventExpose* event,
                                                   gpointer        data) {
    viewerBase::mainWindow::tk* me = 
      reinterpret_cast<viewerBase::mainWindow::tk*>(data);
    
    // GTK2 provides also a "region" attribute which is sort of a list of fine
    // grained rectangles.  It is still to be decided if the list of
    // rectangles are necessary or just "area", which is their bounding box,
    // is sufficient.
    irectangle rect(event->area.x,
                    event->area.y,
                    event->area.x+event->area.width-1,
                    event->area.y+event->area.height-1);

    me->drawingAreaExposed(rect);
        
    return (FALSE); // continue normal event handling
                    // TRUE to stop normal event handling

  }

  bool viewerBase::mainWindow::tk::drawingAreaExposed(const irectangle&) {
    // This should be overloaded.
        
    return true;
  }

  gint viewerBase::mainWindow::tk::cbMouseMoved(GtkWidget*,
                                                GdkEventMotion* event,
                                                gpointer        data) {
    
    viewerBase::mainWindow::tk* me = 
      reinterpret_cast<viewerBase::mainWindow::tk*>(data);
    
    const int mod = ((((event->state & GDK_SHIFT_MASK) != 0) ?
                      viewerBase::ShiftKey : 0) +
                     (((event->state & GDK_CONTROL_MASK) != 0) ?
                      viewerBase::CtrlKey : 0) + 
                     (((event->state & GDK_MOD1_MASK) != 0) ?
                      viewerBase::AltKey : 0));
    
    const int button = ((((event->state & GDK_BUTTON1_MASK) != 0) ?
                         viewerBase::LeftButton : 0) +
                        (((event->state & GDK_BUTTON2_MASK) != 0) ?
                         viewerBase::MiddleButton : 0) + 
                        (((event->state & GDK_BUTTON3_MASK) != 0) ?
                         viewerBase::RightButton : 0) +
                        (((event->state & GDK_BUTTON4_MASK) != 0) ?
                         viewerBase::WheelUp : 0) +
                        (((event->state & GDK_BUTTON5_MASK) != 0) ?
                         viewerBase::WheelDown : 0));

    const ipoint pos(iround(event->x),iround(event->y));

    const viewerBase::interaction action(viewerBase::MouseMoved,
                                         button,
                                         mod);
    
    me->mouseMoved(action,pos);

    return FALSE; // let GTK do whatever it has to do processing the motion
  }
  
  bool 
  viewerBase::mainWindow::tk::mouseMoved(const viewerBase::interaction action,
                                         const ipoint pos) {
    _lti_debug4(".");
    return parent_->onKMInteraction(action,pos);
  }

  gint viewerBase::mainWindow::tk::cbButtonPressed(GtkWidget*,
                                                   GdkEventButton* event,
                                                   gpointer        data) {
    
    viewerBase::mainWindow::tk* me = 
      reinterpret_cast<viewerBase::mainWindow::tk*>(data);
    
    const int mod = ((((event->state & GDK_SHIFT_MASK) != 0) ?
                      viewerBase::ShiftKey : 0) +
                     (((event->state & GDK_CONTROL_MASK) != 0) ?
                      viewerBase::CtrlKey : 0) + 
                     (((event->state & GDK_MOD1_MASK) != 0) ?
                      viewerBase::AltKey : 0));
    
    int button = 0;
    switch(event->button) {
      case 1:
        button=viewerBase::LeftButton;
        break;
      case 2:
        button=viewerBase::MiddleButton;
        break;
      case 3:
        button=viewerBase::RightButton;
        break;
      case 4:
        button=viewerBase::WheelUp;
        break;
      case 5:
        button=viewerBase::WheelDown;
        break;
      default:
        button=viewerBase::OtherButton;
    }

    const ipoint pos(iround(event->x),iround(event->y));

    const viewerBase::interaction action(viewerBase::ButtonPressed,
                                         button,
                                         mod);
    
    me->buttonPressed(action,pos);

    return FALSE; // let GTK do whatever it has to do processing the motion
  }

  bool viewerBase::mainWindow::tk::
  buttonPressed(const viewerBase::interaction action,
                const ipoint pos) {
    _lti_debug4(".");
    return parent_->onKMInteraction(action,pos);
  }

  gint viewerBase::mainWindow::tk::cbScrolled(GtkWidget*,
                                              GdkEventScroll* event,
                                              gpointer        data) {
    
    viewerBase::mainWindow::tk* me = 
      reinterpret_cast<viewerBase::mainWindow::tk*>(data);
    
    const int mod = ((((event->state & GDK_SHIFT_MASK) != 0) ?
                      viewerBase::ShiftKey : 0) +
                     (((event->state & GDK_CONTROL_MASK) != 0) ?
                      viewerBase::CtrlKey : 0) + 
                     (((event->state & GDK_MOD1_MASK) != 0) ?
                      viewerBase::AltKey : 0));
    
    int button = 0;
    switch(event->direction) {
      case GDK_SCROLL_UP:
      case GDK_SCROLL_RIGHT:
        button=viewerBase::WheelUp;
        break;
      case GDK_SCROLL_DOWN:
      case GDK_SCROLL_LEFT:
        button=viewerBase::WheelDown;
        break;
      default:
        button=viewerBase::OtherButton;
    }

    const ipoint pos(iround(event->x),iround(event->y));

    const viewerBase::interaction action(viewerBase::ButtonPressed,
                                         button,
                                         mod);
    
    me->scrolled(action,pos);

    return FALSE; // let GTK do whatever it has to do processing the motion
  }

  bool viewerBase::mainWindow::tk::
  scrolled(const viewerBase::interaction action,
           const ipoint pos) {
    _lti_debug4(".");
    return parent_->onKMInteraction(action,pos);
  }

  gint viewerBase::mainWindow::tk::cbButtonReleased(GtkWidget*,
                                                    GdkEventButton* event,
                                                    gpointer        data) {
    viewerBase::mainWindow::tk* me = 
      reinterpret_cast<viewerBase::mainWindow::tk*>(data);
    
    const int mod = ((((event->state & GDK_SHIFT_MASK) != 0) ?
                      viewerBase::ShiftKey : 0) +
                     (((event->state & GDK_CONTROL_MASK) != 0) ?
                      viewerBase::CtrlKey : 0) + 
                     (((event->state & GDK_MOD1_MASK) != 0) ?
                      viewerBase::AltKey : 0));
    
    int button = 0;
    switch(event->button) {
      case 1:
        button=viewerBase::LeftButton;
        break;
      case 2:
        button=viewerBase::MiddleButton;
        break;
      case 3:
        button=viewerBase::RightButton;
        break;
      case 4:
        button=viewerBase::WheelUp;
        break;
      case 5:
        button=viewerBase::WheelDown;
        break;
      default:
        button=viewerBase::OtherButton;
        break;
    }

    const ipoint pos(iround(event->x),iround(event->y));

    const viewerBase::interaction action(viewerBase::ButtonReleased,
                                         button,
                                         mod);
    
    me->buttonReleased(action,pos);

    return FALSE; // let GTK do whatever it has to do processing the motion
  }

  bool viewerBase::mainWindow::tk::
  buttonReleased(const viewerBase::interaction action,
                 const ipoint pos) {
    _lti_debug4(".");    
    return parent_->onKMInteraction(action,pos);
  }

  gint viewerBase::mainWindow::tk::cbKeyPressed(GtkWidget*,
                                                GdkEventKey* event,
                                                gpointer     data) {
    viewerBase::mainWindow::tk* me = 
      reinterpret_cast<viewerBase::mainWindow::tk*>(data);
    
    const int mod = ((((event->state & GDK_SHIFT_MASK) != 0) ?
                      viewerBase::ShiftKey : 0) +
                     (((event->state & GDK_CONTROL_MASK) != 0) ?
                      viewerBase::CtrlKey : 0) + 
                     (((event->state & GDK_MOD1_MASK) != 0) ?
                      viewerBase::AltKey : 0));
    
    const viewerBase::interaction action(viewerBase::KeyPressed,
                                         event->keyval,
                                         mod);
    
    me->keyPressed(action);

    return FALSE; // let GTK do whatever it has to do processing the motion
  }

  bool viewerBase::mainWindow::tk::
  keyPressed(const viewerBase::interaction action) {
    _lti_debug4(".");
    return parent_->onKMInteraction(action,ipoint(0,0));
  }

  gint viewerBase::mainWindow::tk::cbKeyReleased(GtkWidget*,
                                                 GdkEventKey* event,
                                                 gpointer     data) {
    viewerBase::mainWindow::tk* me = 
      reinterpret_cast<viewerBase::mainWindow::tk*>(data);
    
    const int mod = ((((event->state & GDK_SHIFT_MASK) != 0) ?
                      viewerBase::ShiftKey : 0) +
                     (((event->state & GDK_CONTROL_MASK) != 0) ?
                      viewerBase::CtrlKey : 0) + 
                     (((event->state & GDK_MOD1_MASK) != 0) ?
                      viewerBase::AltKey : 0));
    
    const viewerBase::interaction action(viewerBase::KeyReleased,
                                         event->keyval,
                                         mod);
    
    me->keyReleased(action);

    return FALSE; // let GTK do whatever it has to do processing the motion
  }

  bool viewerBase::mainWindow::tk::
  keyReleased(const viewerBase::interaction action) {
    _lti_debug4(".");
    return parent_->onKMInteraction(action,ipoint(0,0));
  }

  gboolean viewerBase::mainWindow::tk::cbForceRedraw(gpointer data) {
    viewerBase::mainWindow::tk* me = 
      reinterpret_cast<viewerBase::mainWindow::tk*>(data);
    gdk_threads_enter();
    me->forceRedraw();
    gdk_flush();    
    gdk_threads_leave();
    return false; // false => remove from the idle functions when ready
  }

  bool viewerBase::mainWindow::tk::forceRedraw() {
    if (guiServer::representsGuiThread()) {
      // Within the GUI thread just "post" the redraw
      if (notNull(drawingArea_)) {
        gtk_widget_queue_draw(drawingArea_);
      }
    } else {
      // Outside the GUI thread generate the proper event on idle, which will
      // call this function again, but within the GUI thread.
      g_idle_add(cbForceRedraw,this);
    }

    return true;
  }


  bool viewerBase::mainWindow::tk::setCanvasSize(const ipoint& newSize) {
    gtk_widget_set_size_request(drawingArea_, newSize.x, newSize.y);
    return true;
  }

  // ------------------------------------------------------------------------
  //   Configuration Dialog's toolkit
  // ------------------------------------------------------------------------
  
  viewerBase::configDialog::tk::tk(viewerBase::configDialog& parent) :
    parent_(parent),
    params_(parent.params_),
    dlgShown_(false),
    dlgBuild_(false),
    dialog_(0),
    vbox_(0),
    actionArea_(0),
    cancelButton_(0),
    applyButton_(0),
    okButton_(0) {
    _lti_enterCTOR();
    _lti_leaveCTOR();
  }

  viewerBase::configDialog::tk::~tk() {
    _lti_enterDTOR();
    hide();
    destroy();
    _lti_leaveDTOR();
  }

  bool viewerBase::configDialog::tk::build() {
    _lti_debug2("entering viewerBase::configDialog::tk::build()\n");

    if (dlgBuild_) {
      // window already build!  This shouldn't happen, but it has!
      return false;
    }

    lock_.lock();
    // gdk_threads_enter();

    // the dialog window
    dialog_ = gtk_dialog_new ();
    gtk_window_set_title (GTK_WINDOW (dialog_), "Configuration");
    gtk_window_set_type_hint (GTK_WINDOW (dialog_), 
                              GDK_WINDOW_TYPE_HINT_DIALOG);

    vbox_ = GTK_DIALOG (dialog_)->vbox;
    gtk_widget_set_name (vbox_, "vbox_");

    actionArea_ = GTK_DIALOG (dialog_)->action_area;
    
    gtk_button_box_set_layout (GTK_BUTTON_BOX (actionArea_),
                               GTK_BUTTONBOX_SPREAD);

    cancelButton_ = gtk_button_new_from_stock ("gtk-cancel");
    gtk_dialog_add_action_widget (GTK_DIALOG (dialog_), 
                                  cancelButton_,
                                  GTK_RESPONSE_CANCEL);
    GTK_WIDGET_SET_FLAGS (cancelButton_, GTK_CAN_DEFAULT);

    applyButton_ = gtk_button_new_from_stock ("gtk-apply");
    gtk_widget_show (applyButton_);
    gtk_dialog_add_action_widget (GTK_DIALOG (dialog_),
                                  applyButton_,
                                  GTK_RESPONSE_APPLY);
    GTK_WIDGET_SET_FLAGS (applyButton_, GTK_CAN_DEFAULT);

    okButton_ = gtk_button_new_from_stock ("gtk-ok");
    gtk_widget_show (okButton_);
    gtk_dialog_add_action_widget (GTK_DIALOG (dialog_),
                                  okButton_,
                                  GTK_RESPONSE_OK);
    GTK_WIDGET_SET_FLAGS (okButton_, GTK_CAN_DEFAULT);

    g_signal_connect (G_OBJECT (dialog_), "delete_event",
                      G_CALLBACK (cbCloseWindow), this);

    g_signal_connect (G_OBJECT(cancelButton_), "clicked",
                      G_CALLBACK (cbOnCancelButtonClicked),
                      this);
    g_signal_connect (G_OBJECT(applyButton_), "clicked",
                      G_CALLBACK (cbOnApplyButtonClicked),
                      this);
    g_signal_connect (G_OBJECT(okButton_), "clicked",
                      G_CALLBACK (cbOnOkButtonClicked),
                      this);
    dlgBuild_=true;

    //gdk_flush();
    //gdk_threads_leave();
    lock_.unlock();
    _lti_debug2("leaving viewerBase::configDialog::tk::build()\n");

    return true;
  }

  gboolean viewerBase::configDialog::tk::cbShow(gpointer data) {
    viewerBase::configDialog::tk* me = 
      reinterpret_cast<viewerBase::configDialog::tk*>(data);
    me->show();
    return false; // false => remove from the idle functions when ready
  }

  bool viewerBase::configDialog::tk::show() {
    lock_.lock();
    if ((isNull(dialog_) || !dlgBuild_)) {
      lock_.unlock();
      build();
      lock_.lock();
    } 
    
    if (notNull(dialog_)) {
      _lti_debug2("enter viewerBase::configDialog::tk::show() (gui th.)\n");
      if (!dlgShown_) {
        // this is the task if called within the gui thread
        gtk_widget_show_all(dialog_);
        dlgShown_=true;
      } else {
        // dialog shown, but maybe we need to raise it
        gdk_window_raise(dialog_->window);
        gdk_window_deiconify(dialog_->window);
      }
      lock_.unlock();
      return true;
    }
    
    lock_.unlock();
    return false;
  }

  bool viewerBase::configDialog::tk::hide() {
    lock_.lock();
    if (notNull(dialog_)) {
      // This method can be explicitely (indirectly) be called by a
      // users who wants to hide the window from his C++ code, but also
      // from the GUI if he selects the "close" icon (usually an X).
      // In the first case we have to protect the GTK, in the second everything
      // occurs within the proper thread.
      _lti_debug2("enter viewerBase::configDialog::tk::hide() ");
      if (guiServer::representsGuiThread()) {
        _lti_debug2("(in gui)\n");        
        gtk_widget_hide_all(dialog_);
      } else {
        _lti_debug2("(not gui)\n");
        gdk_threads_enter();
        gtk_widget_hide_all(dialog_);
        gdk_flush();
        gdk_threads_leave();
      }
      dlgShown_=false;
      lock_.unlock();
      return true;
    }
    lock_.unlock();
    return false;
  }

  bool viewerBase::configDialog::tk::destroy() {
    if (hide()) {
      lock_.lock();
      _lti_debug2("enter viewerBase::configDialog::tk::destroy()\n");
      if (guiServer::representsGuiThread()) {
        gtk_widget_destroy(GTK_WIDGET(dialog_));
      } else {
        gdk_threads_enter();
        gtk_widget_destroy(GTK_WIDGET(dialog_));
        gdk_flush();
        gdk_threads_leave();
      }
      dlgBuild_=false;
      // params_=0;
      // parent_=0;
      // set pointers to zero
      dialog_       = 0;
      vbox_         = 0;
      actionArea_   = 0;
      cancelButton_ = 0;
      applyButton_  = 0;
      okButton_     = 0;
      lock_.unlock();
      return true;
    }

    return false;
  }

  viewerBase::configDialog::tk& 
  viewerBase::configDialog::tk::copy(const tk& ) {
    // TODO: what to do?  deep copy of parameters?  maybe the tk should
    // never be copied... still to be decided...
    
    return *this;
  }

  bool 
  viewerBase::configDialog::tk::updateParameters() {
    _lti_debug2("Enter viewerBase::configDialog::tk::updateParameters\n");
    
    // if params_ are NULL then nothing can be updated!
    return notNull(params_); 
  }

  gint viewerBase::configDialog::tk::cbCloseWindow(GtkWidget *,
                                                   GdkEvent  *,
                                                   gpointer   data) {
    viewerBase::configDialog::tk* me = 
      reinterpret_cast<viewerBase::configDialog::tk*>(data);

    me->onCancelButtonClicked();

    /*
     * Change TRUE to FALSE and the main window will be destroyed with
     * a "delete_event".
     */
    return(TRUE); // TRUE indicates that the event has been handled
  }


  void viewerBase::configDialog::tk::cbOnCancelButtonClicked(GtkButton *,
                                                             gpointer   data) {
    viewerBase::configDialog::tk* me = 
      reinterpret_cast<viewerBase::configDialog::tk*>(data);

    me->onCancelButtonClicked();
  }

  bool viewerBase::configDialog::tk::onCancelButtonClicked() {
    // indicate an event in case someone is waiting for interaction    
    return parent_.onCancelButtonClicked();
  }


  void viewerBase::configDialog::tk::cbOnApplyButtonClicked(GtkButton *,
                                                            gpointer   data) {
    viewerBase::configDialog::tk* me = 
      reinterpret_cast<viewerBase::configDialog::tk*>(data);

    me->onApplyButtonClicked();
  }

  bool viewerBase::configDialog::tk::onApplyButtonClicked() {
    // indicate an event in case someone is waiting for interaction    
    return parent_.onApplyButtonClicked();
  }

  void viewerBase::configDialog::tk::cbOnOkButtonClicked(GtkButton *,
                                                         gpointer   data) {
    viewerBase::configDialog::tk* me = 
      reinterpret_cast<viewerBase::configDialog::tk*>(data);

    me->onOkButtonClicked();
  }

  bool viewerBase::configDialog::tk::onOkButtonClicked() {
    // indicate an event in case someone is waiting for interaction    
    return parent_.onOkButtonClicked();
  }

  // ------------------------------------------------------------------------
  //   Main Window
  // ------------------------------------------------------------------------


  bool viewerBase::mainWindow::build() {
    // since tk_ is initialied at construction time, this should always exist.
    assert(notNull(tk_));
    return tk_->build();
  }

  bool viewerBase::mainWindow::show() {
    assert(notNull(tk_));
    return tk_->show();
  }

  bool viewerBase::mainWindow::hide() {
    return tk_->hide(); // is this right? TODO!!!
  }

  bool viewerBase::mainWindow::destroy() {
    return tk_->destroy();
  }

  bool viewerBase::mainWindow::statusBar(const std::string& txt) {
    return tk_->statusBar(txt);
  }

  void viewerBase::mainWindow::initToolkit() {
    if (isNull(tk_)) {
      tk_=new tk(*this);
    }
  }

  void viewerBase::mainWindow::destroyToolkit() {
    if (notNull(tk_)) {
      delete tk_;
      tk_=0;
    }
  }

  bool 
  viewerBase::mainWindow::updateParameters(viewerBase::parameters& params) {
    assert(notNull(tk_)); // toolkit should exist since construction time
    params_ = &params;
    // update the toolkit
    return tk_->updateParameters(params);
  }

  bool viewerBase::mainWindow::setCanvasSize(const ipoint& newSize) {
    _lti_debug4("viewerBase::mainWindow::setCanvasSize(" << newSize <<
                ")" << std::endl);
    
    assert(notNull(tk_));
    if (tk_->setCanvasSize(newSize)) {
      canvasSize_.copy(newSize);
      return true;
    }
    return false;
  }

  // ------------------------------------------------------------------------
  //   Config Dialog
  // ------------------------------------------------------------------------


  bool viewerBase::configDialog::build() {
    // since tk_ is initialied at construction time, this should always exist.
    assert(notNull(tk_));
    return tk_->build();
  }

  bool viewerBase::configDialog::show() {
    assert(notNull(tk_));

    return tk_->show() && tk_->updateParameters();
  }

  bool viewerBase::configDialog::hide() {
    return tk_->hide(); // is this right? TODO!!!
  }

  bool viewerBase::configDialog::destroy() {
    return tk_->destroy();
  }

  void viewerBase::configDialog::initToolkit() {
    if (isNull(tk_)) {
      tk_=new tk(*this);
    }
  }

  void viewerBase::configDialog::destroyToolkit() {
    if (notNull(tk_)) {
      delete tk_;
      tk_=0;
    }
  }
}

#endif
