/*
 * Copyright (C) 2010
 * Pablo Alvarado
 *
 * This file is part of the LTI-Computer Vision Library 2 (LTI-Lib-2)
 *
 * The LTI-Lib-2 is free software; you can redistribute it and/or
 * modify it under the terms of the BSD License.
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the authors nor the names of its contributors may be
 *    used to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * \file    ltiViewer1DGtk.h
 *          This is a "private" file.  It contains GTK specific stuff for
 *          implementing the viewer1D class.  Unless you are implementing
 *          GTK derived classes, you don't need to include this file.  See
 *          ltiViewer1D.h instead.
 * \author Pablo Alvarado
 * \date    05.07.2005
 * 
 * $Id: ltiViewer1DGtk.h,v 1.2 2012-09-16 05:05:59 alvarado Exp $
 */

#ifndef _LTI_VIEWER_1D_GTK_H_
#define _LTI_VIEWER_1D_GTK_H_

#include "ltiViewer1D.h"
#include "ltiViewerBaseGtk.h"
#include "ltiViewer1DPainter.h"
#include "ltiTimer.h"

#ifdef HAVE_GTK


#include <gtk/gtk.h>

namespace lti {

  /**
   * Private class of the configuration dialogs which deals with
   * all internal issues of the GTK toolkit.
   */
  class viewer1D::configDialog::tk : public viewerBase::configDialog::tk {
  public:
    /**
     * Constructor.
     *
     * This constructor does not call build(), since at creation time GTK
     * may not be initialized yet.
     */
    tk(viewer1D::configDialog& parent);

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
     * Update parameters
     */
    virtual bool updateParameters();

    /**
     * Update information
     */
    virtual bool updateInformation(const viewer1D::information& info);


    /**
     * Cast the internal parameters object to the proper type
     */
    parameters& getParameters();
    
  protected:
    /**
     * Timer used to avoid a too frequent update
     */
    timer updateTime;

    /**
     * @name All GTK widgets initialized in the build() method
     */
    //@{
    GtkWidget *displayCtrlBox_;
    GtkWidget *displayCtrlRBox_;
    GtkWidget *displayCtrlFrame;
    GtkWidget *displayCtrlAlignment_;
    GtkWidget *displayCtrlVBox_;
    GtkWidget *zoomXBox_;
    GtkWidget *zoomXLabel_;
    GtkWidget *zoomXEntry_;
    GtkWidget *zoomXScale_;
    GtkAdjustment *zoomXAdjustment_;
    GtkWidget *zoomXSeparator_;
    GtkWidget *zoomYSeparator_;
    GtkWidget *zoomYBox_;
    GtkWidget *zoomYLabel_;
    GtkWidget *zoomYEntry_;
    GtkWidget *zoomYScale_;
    GtkAdjustment *zoomYAdjustment_;
    GtkWidget *markerTable_;
    GtkWidget *styleComboBox_;
    GtkWidget *markerLabel_;
    GtkWidget *markerComboBox_;
    GtkWidget *heightLabel_;
    GtkObject *heightSpin__adj;
    GtkWidget *heightSpin_;
    GtkWidget *widthLabel_;
    GtkObject *widthSpin__adj;
    GtkWidget *widthSpin_;
    GtkWidget *styleLabel_;
    GtkWidget *ticksXLabel_;
    GtkObject *ticksXSpin__adj;
    GtkWidget *ticksXSpin_;
    GtkWidget *ticksYLabel_;
    GtkObject *ticksYSpin__adj;
    GtkWidget *ticksYSpin_;
    GtkWidget *showThingsTable_;
    GtkWidget *horizontalAxisButton_;
    GtkWidget *gridButton_;
    GtkWidget *meanValueButton_;
    GtkWidget *forceZeroButton_;
    GtkWidget *colorHBox_;
    GtkWidget *colorLabel_;
    GtkWidget *colorsComboBox_;
    GtkWidget *colorButton_;
    GtkWidget *displayCtrlFrameLabel_;
    GtkWidget *informationFrame_;
    GtkWidget *informationAlignment_;
    GtkWidget *infoTable_;
    GtkWidget *imageSizeLabel_;
    GtkWidget *sizeEntry_;
    GtkWidget *minLabel_;
    GtkWidget *maxLabel;
    GtkWidget *minEntry_;
    GtkWidget *maxEntry_;
    GtkWidget *averageLabel_;
    GtkWidget *stdDevLabel_;
    GtkWidget *averageEntry_;
    GtkWidget *stdDevEntry_;
    GtkWidget *informationFrameLabel_;
    GtkWidget *saveFrame_;
    GtkWidget *saveImageAlignment_;
    GtkWidget *saveImageHBox_;
    GtkWidget *saveImageEntry_;
    GtkWidget *openButton_;
    GtkWidget *saveButton_;
    GtkWidget *saveButtonAlignment_;
    GtkWidget *saveButtonHBox;
    GtkWidget *saveButtonImage_;
    GtkWidget *saveButtonLabel_;
    GtkWidget *saveAllButton_;
    GtkWidget *saveAllButtonAlignment_;
    GtkWidget *saveAllButtonHBox_;
    GtkWidget *saveAllButtonImage_;
    GtkWidget *saveAllButtonLabel_;
    GtkWidget *saveImageLabel_;
    GtkWidget *actionButtonBox_;
    GtkTooltips *tooltips_;
    //@}

    enum eSaveAction {
      SaveNone,
      SaveAll,
      SaveView
    };

    /**
     * @name Static functions used by GTK.
     *
     * Static functions used by GTK as callbacks and their corresponding local
     * versions.
     *
     * Even if the LTI-Lib forbids the use of prefixes, here an exception
     * has been done with "cb", meaning "callback", which is used to keep
     * a direct correspondence between the GTK required static function and
     * a local instance-bounded virtual method.
     */
    //@{
    /**
     * Call back if the browse button is clicked
     */
    static void cbOnBrowseButtonClicked(GtkButton *button,
                                        gpointer   userData);

    /**
     * Call back if the browse button is clicked
     */
    void onBrowseButtonClicked(GtkButton *button,
                               eSaveAction action);

    /**
     * Call back if the save button is clicked
     */
    static void cbOnSaveButtonClicked(GtkButton *button,
                                      gpointer   userData);

    /**
     * Call back if the save-all button is clicked
     */
    static void cbOnSaveAllButtonClicked(GtkButton *button,
                                         gpointer   userData);

    /**
     * Callback when the filename has been changed
     */
    static void cbOnFilenameEntryActivate(GtkCellEditable *celleditable,
                                          gpointer         userData);
    /**
     * Callback for zoom text entry
     */
    static void cbOnZoomXEntryActivate(GtkCellEditable *celleditable,
                                       gpointer         userData);
    
    /**
     * Callback for zoom horizontal scale bar
     */
    static void cbOnZoomXAdjustmentValueChanged(GtkAdjustment* scale,
                                                gpointer       userData);
    
    /**
     * Callback for zoom text entry
     */
    static void cbOnZoomYEntryActivate(GtkCellEditable *celleditable,
                                       gpointer         userData);
    
    /**
     * Callback for zoom horizontal scale bar
     */
    static void cbOnZoomYAdjustmentValueChanged(GtkAdjustment* scale,
                                                gpointer       userData);

    /**
     * Callback for zoom horizontal scale bar
     */
    static void cbOnStyleChanged(GtkComboBox* scale,
                                 gpointer     userData);

    /**
     * Callback for zoom horizontal scale bar
     */
    static void cbOnMarkerChanged(GtkComboBox* scale,
                                 gpointer     userData);

    /**
     * Call back if the spin height is changed
     */
    static void cbOnHeightSpinValueChanged(GtkSpinButton *button,
                                           gpointer   userData);

    /**
     * Call back if the spin width is changed
     */
    static void cbOnWidthSpinValueChanged(GtkSpinButton *button,
                                          gpointer       userData);

    /**
     * Call back if the spin ticks X is changed
     */
    static void cbOnTicksXSpinValueChanged(GtkSpinButton *button,
                                           gpointer   userData);

    /**
     * Call back if the spin ticks Y is changed
     */
    static void cbOnTicksYSpinValueChanged(GtkSpinButton *button,
                                           gpointer   userData);

    /**
     * Callback when horizontal axis button is clicked
     */
    static void cbOnHorizontalAxisToggled(GtkToggleButton* toggleButton,
                                          gpointer userData);

    /**
     * Callback when grid button is clicked
     */
    static void cbOnGridToggled(GtkToggleButton* toggleButton,
                                gpointer userData);

    /**
     * Callback when mean value button is clicked
     */
    static void cbOnMeanValueToggled(GtkToggleButton* toggleButton,
                                     gpointer userData);

    /**
     * Callback when force zero button is clicked
     */
    static void cbOnForceZeroToggled(GtkToggleButton* toggleButton,
                                     gpointer userData);

    /**
     * Callback when color selection button is clicked
     */
    static void cbOnColorButtonSet(GtkColorButton  *colorbutton,
                                   gpointer         userData);

    /**
     * Callback for marker changed
     */
    static void cbOnColorComboChanged(GtkComboBox* scale,
                                      gpointer     userData);

    //@}


    /**
     * @name Convert the given values to a string
     */
    //@{
    static std::string toString(const double val,const int decimals=5);
    static std::string toString(const int val);
    static double fromString(const char* val);
    static std::string toString(const ipoint& val);
    static std::string toString(const rgbaPixel& val);
    static std::string toString(const frgbPixel& val,const int decimals=2);
    
    //@}

    /**
     * Set the GUI elements to the proper zoom value
     */
    void setZoomX(const float& zoom);

    /**
     * Set the GUI elements to the proper zoom value
     */
    void setZoomY(const float& zoom);

    /**
     * Get the proper zoom value from the GUI elements
     */
    fpoint getZoom() const;

    /**
     * Get the proper zoom value from the GUI elements
     */
    float getZoomX() const;

    /**
     * Get the proper zoom value from the GUI elements
     */
    float getZoomY() const;


    /**
     * Set the style 
     */
    void setStyle(const viewer1DPainter::eStyle style);
    
    /**
     * Set the style 
     */
    viewer1DPainter::eStyle getStyle() const;

    /**
     * Set the marker
     */
    void setMarker(const std::string& str);
    
    /**
     * Set rgbaPixel in the parameters corresponding to the
     * selected color in the combo box
     */
    void setColor(const rgbaPixel& color);

    /**
     * Check which color has the parameter corresponding to the
     * selected color in the combo box.
     *
     * Return false if no color has been selected yet, or true otherwise
     */
    bool getColor(rgbaPixel& color);

    /**
     * Get the marker 
     */
    std::string getMarker() const;


    /**
     * Data type being displayed
     */
    eDataType dataType_;

    /**
     * Constant value used in several methods for the minimum posible zoom
     */
    static const float minZoom;
  };


  class viewer1D::mainWindow::tk : public viewerBase::mainWindow::tk {
  public:
    /**
     * Constructor.
     *
     * This constructor does not call build(), since at creation time GTK
     * may not be initialized yet.
     */
    tk(viewer1D::mainWindow& parent);

    /**
     * Destructor
     */
    virtual ~tk(); 

    /**
     * suggestWindowSize()
     */
    virtual ipoint suggestWindowSize() const;

    /**
     * Called when some part of the window has been exposed and need to be
     * redrawn.
     */
    virtual bool drawingAreaExposed(const irectangle& area);

    /**
     * Called when the user pressed the save button
     */
    virtual bool saveButtonPressed(const std::string& filename);

    /**
     * Called when the user pressed the save all button
     */
    virtual bool saveAllButtonPressed(const std::string& filename);

  private:
    /**
     * Working buffer
     *
     * This buffer grows continuously, as necessary
     */
    guchar* xbuffer_;

    /**
     * Size of the xbuffer
     */
    int xbufferSize_; 

  };


}

#endif
#endif
