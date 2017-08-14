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
 * \file    ltiViewerBaseGtk.h
 *          This is a "private" file.  It contains GTK specific stuff for
 *          implementing the viewerBase class.  Unless you are implementing
 *          GTK derived classes, you don't need to include this file.  See
 *          ltiViewerBase.h instead.
 * \author Pablo Alvarado
 * \date    05.07.2005
 * 
 * $Id: ltiViewerBaseGtk.h,v 1.3 2012-09-16 05:05:59 alvarado Exp $
 */

#ifndef _LTI_VIEWER_BASE_GTK_H_
#define _LTI_VIEWER_BASE_GTK_H_

#include "ltiViewerBase.h"

#ifdef HAVE_GTK

#include "ltiRectangle.h"

// GTK uses variadic macros, which exist since C++11
// Disable the warnings
#include "ltiMacroSymbols.h"

#include <gtk/gtk.h>

namespace lti {

  // ------------------------------------------------------------------------
  //   GTK Toolkit class
  // ------------------------------------------------------------------------
  // For GTK, which is C, this class is simply a container of static functions
  // which are used as slots for all activity

  /**
   * GTK toolkit class
   *
   * This is the parent class for the viewer windows implemented with
   * GTK.
   * The class initializes the drawing widget, status bar, and scroll bars.
   */
  class viewerBase::mainWindow::tk {
  public:
    /**
     * Constructor.
     *
     * This constructor does not call build(), so the main window still has to
     * be created.
     */
    tk(viewerBase::mainWindow& parent);

    /**
     * Destructor
     */
    virtual ~tk();

    /**
     * Create window
     */
    virtual bool build();

    /**
     * Show the window
     */
    virtual bool show();

    /**
     * Hide this window
     */
    virtual bool hide();

    /**
     * Destroy window
     */
    virtual bool destroy();

    /**
     * Display in the status bar the given text
     */
    virtual bool statusBar(const std::string& txt);

    /**
     * Copy another mainWindow toolkit configuration
     */
    tk& copy(const tk& other);

    /**
     * Change position of the window
     */
    virtual bool setPosition(const ipoint& p);

    /**
     * Get position of the window
     */
    ipoint getPosition() const;

    /**
     * Set size of the window
     */
    virtual bool setSize(const ipoint& p);

    /**
     * Get size of the window
     */
    ipoint getSize() const;

    /**
     * Get initial suggestion for window size
     */
    virtual ipoint suggestWindowSize() const;

    /**
     * Set window title
     */
    virtual bool setTitle(const std::string& title);

    /**
     * Get a string with the title of this window
     */
    std::string getTitle() const;

    /**
     * Update parameters.
     */
    virtual bool updateParameters(viewerBase::parameters& param);

    /**
     * Resize canvas
     */
    virtual bool setCanvasSize(const ipoint& newSize);

  protected:
    /**
     * Pointer to a read-only version of the parameters
     */
    viewerBase::parameters* params_;

    /**
     * Pointer to the class that owns this instance.
     */
    viewerBase::mainWindow* parent_;

    /**
     * Flag that indicates if the window is currently being shown
     */
    bool winShown_;

    /**
     * Flag that indicates if the window is currently being shown
     */
    bool winBuild_;

    /*
     * Border offset
     *
     * The user always indicates the size of the drawing area, without
     * considering the borders which may be style/theme dependent.
     *
     * This value is set automatically when the user interacts with the
     * window, which is the first time.
     *
     * It is initialized with negative values to indicate that the value has
     * to be updated as soon as reliable information is available (ie. the
     * viewer has been displayed)
     */
    ipoint borderOffset_;

    /**
     * The main window. 
     *
     * In GTK a window widget should be a pop up menu or the top window
     * application.  It is therefore more appropriate to use the gtk_dialog
     * to hold the images, as they have less restrictions and are easier to
     * manipulate.
     */
    GtkWidget* window_;

    /**
     * Vertical box for packing two components: the drawing area and the status
     * bar
     */
    GtkWidget* vbox_;
    
    /**
     * The scrolled window.
     */
    GtkWidget *scrolledWindow_;

    /**
     * The status bar.
     */
    GtkWidget *statusBar_;

    /**
     * Context id of the status bar.
     */
    guint statusBarContextId_;

    /**
     * Drawing area that holds the image.
     */
    GtkWidget *drawingArea_;

    /**
     * Event box to capture mouse and button events.
     */
    GtkWidget *eventBox_;

    GdkCursor* cursor_;

    /**
     * Protect all members from multiple access
     */
    mutex lock_;

    /**
     * Static functions used by GTK as callbacks and their corresponding 
     * local versions.
     *
     * Even if the LTI-Lib forbids the use of prefixes, here an exception
     * has been done with "cb", meaning "callback", which is used to keep
     * a direct correspondence between the GTK required static function and
     * a local instance-bounded virtual method.
     */
    //@{

    /**
     * Callback function called when the user has explicitelly indicated
     * a show()
     */
    static gboolean cbShow(gpointer data);

    /**
     * Callback function called when the user selects the "close window"
     * button of the window.  This method overrides the delete structure with
     * a hide().
     */
    static gint cbCloseWindow(GtkWidget *widget,
                              GdkEvent  *event,
                              gpointer   data);

    /**
     * Close the window and all related elements
     */
    virtual bool closeWindow();

    /**
     * Callback fuction called when the window size or position have been
     * changed.
     */
    static gint cbGeometryChanged(GtkWidget*        widget,
                                 GdkEventConfigure* event,
                                 gpointer           data);

    /**
     * Called when the geometry of the window is changed.
     *
     * Note that this won't change the geometry, since it is called by
     * someone who changed it already.
     */
    virtual bool geometryChanged(const ipoint pos,
                                 const ipoint size);

    /**
     * Callback funtion to redraw the image been displayed.
     */
    static gint cbDrawingAreaExposed(GtkWidget*      widget,
                                     GdkEventExpose* event,
                                     gpointer        data);

    /**
     * Called when some part of the window has been exposed and need to be
     * redrawn.
     */
    virtual bool drawingAreaExposed(const irectangle& area);

    /**
     * Callback function called when the mouse is moved while a mouse
     * button is been pressed.
     */
    static gint cbMouseMoved(GtkWidget*      widget,
                             GdkEventMotion* event,
                             gpointer        data);

    /**
     * Function called when the mouse has been moved and some button is pressed
     */
    bool mouseMoved(const viewerBase::interaction action,
                    const ipoint pos);

    /**
     * Callback function called when any mouse button has been pressed.
     */
    static gint cbButtonPressed(GtkWidget*      widget,
                                GdkEventButton* event,
                                gpointer        data);

    /**
     * Function called when a mouse button has been pressed
     */
    bool buttonPressed(const viewerBase::interaction action,
                       const ipoint pos);

    /**
     * Callback function called when any mouse button has been pressed.
     */
    static gint cbScrolled(GtkWidget*      widget,
                           GdkEventScroll* event,
                           gpointer        data);

    /**
     * Function called when a mouse button has been pressed
     */
    bool scrolled(const viewerBase::interaction action,
                  const ipoint pos);

    /**
     * Callback function called when any mouse button has been pressed.
     */
    static gint cbButtonReleased(GtkWidget*      widget,
                                 GdkEventButton* event,
                                 gpointer        data);

    /**
     * Function called when a mouse button has been released
     */
    bool buttonReleased(const viewerBase::interaction action,
                        const ipoint pos);

    /**
     * Callback function called when any key has been pressed.
     */
    static gint cbKeyPressed(GtkWidget*   widget,
                             GdkEventKey* event,
                             gpointer     data);
    /**
     * Function called when a key has been pressed
     */
    bool keyPressed(const viewerBase::interaction action);

    /**
     * Callback function called when any key has been released.
     */
    static gint cbKeyReleased(GtkWidget*   widget,
                              GdkEventKey* event,
                              gpointer     data);
    
    /**
     * Function called when a key has been released
     */
    bool keyReleased(const viewerBase::interaction action);

    /**
     * Callback function used to force a expose event of the whole window.
     */
    static gboolean cbForceRedraw(gpointer data);

    /**
     * This function can be called by either thread, and it will ensure that
     * an expose event of the whole window is generated.
     */
    bool forceRedraw();
    //@}
  };

  /**
   * Private class of the configuration dialogs which deals with
   * all internal issues of the GTK toolkit.
   */
  class viewerBase::configDialog::tk {
  public:
    /**
     * Constructor.
     *
     * This constructor does not call build(), since at creation time GTK
     * may not be initialized yet.
     */
    tk(viewerBase::configDialog& parent);

    /**
     * Destructor
     */
    virtual ~tk();

    /**
     * Create window
     */
    virtual bool build();

    /**
     * Show the window
     */
    virtual bool show();

    /**
     * Hide this window
     */
    virtual bool hide();

    /**
     * Destroy window
     */
    virtual bool destroy();

    /**
     * Copy another mainWindow toolkit configuration
     */
    tk& copy(const tk& other);

    /**
     * Update parameters.
     *
     * Since the base class has no attributes, this method does nothing but
     * to establish the virtual interface.
     *
     * Since the params_ attribute of the class is just an alias for the ones
     * in the enclosing configDialog class, no pointer needs to be
     * adjusted.
     */
    virtual bool updateParameters();


  protected:
    /**
     * Reference to the class that owns this instance.
     */
    viewerBase::configDialog& parent_;

    /**
     * Pointer to a read-writable version of the parameters.
     *
     * It is a reference to the parameters pointer in the configDialog
     * enclosing class, initialized at construction time.  This avoids
     * inconsistencies between the objects being used.
     */
    viewerBase::parameters*& params_;

    /**
     * Flag that indicates if the window is currently being shown
     */
    bool dlgShown_;

    /**
     * Flag that indicates if the window is currently being shown
     */
    bool dlgBuild_;

    /**
     * The main window. 
     *
     * In GTK a window widget should be a pop up menu or the top window
     * application.  It is therefore more appropriate to use the gtk_dialog
     * to hold the images, as they have less restrictions and are easier to
     * manipulate.
     */
    GtkWidget* dialog_;

    /**
     * Vertical box for packing all components in inherited classes
     */
    GtkWidget* vbox_;
    
    /**
     * Action area
     */
    GtkWidget *actionArea_;

    /**
     * Cancel button
     */
    GtkWidget *cancelButton_;

    /**
     * Apply button
     */
    GtkWidget *applyButton_;
    
    /**
     * Ok button
     */
    GtkWidget *okButton_;

    /**
     * Protect all members from multiple access
     */
    mutex lock_;

    /**
     * Static functions used by GTK as callbacks and their corresponding 
     * local versions.
     *
     * Even if the LTI-Lib forbids the use of prefixes, here an exception
     * has been done with "cb", meaning "callback", which is used to keep
     * a direct correspondence between the GTK required static function and
     * a local instance-bounded virtual method.
     */
    //@{
    /**
     * Callback function called when the user has explicitelly indicated
     * a show()
     */
    static gboolean cbShow(gpointer data);

    /**
     * Callback function called when the user selects the "close window"
     * button of the window.  This method overrides the delete structure with
     * a hide().
     */
    static gint cbCloseWindow(GtkWidget *widget,
                              GdkEvent  *event,
                              gpointer   data);  

    /**
     * Callback function called when the user has pressed the cancel button
     */
    static void cbOnCancelButtonClicked(GtkButton* button,
                                        gpointer data);

    /**
     * On apply button
     */
    virtual bool onCancelButtonClicked();

    /**
     * Callback function called when the user has pressed the apply button
     */
    static void cbOnApplyButtonClicked(GtkButton* button,
                                       gpointer data);
    
    /**
     * On apply button
     */
    virtual bool onApplyButtonClicked();

    /**
     * Callback function called when the user has pressed the ok button
     */
    static void cbOnOkButtonClicked(GtkButton* button,
                                    gpointer data);

    /**
     * On ok button
     */
    virtual bool onOkButtonClicked();

    //@}
  };


}


#endif
#endif
