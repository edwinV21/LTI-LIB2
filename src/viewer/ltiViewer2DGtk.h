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
 * \file    ltiViewer2DGtk.h
 *          This is a "private" file.  It contains GTK specific stuff for
 *          implementing the viewer2D class.  Unless you are implementing
 *          GTK derived classes, you don't need to include this file.  See
 *          ltiViewer2D.h instead.
 * \author Pablo Alvarado
 * \date    05.07.2005
 * 
 * $Id: ltiViewer2DGtk.h,v 1.6 2012-09-16 05:05:59 alvarado Exp $
 */

#ifndef _LTI_VIEWER_2D_GTK_H_
#define _LTI_VIEWER_2D_GTK_H_

#include "ltiViewer2D.h"
#include "ltiViewerBaseGtk.h"
#include "ltiViewer2DPainter.h"
#include "ltiTimer.h"

#ifdef HAVE_GTK

#include <gtk/gtk.h>

namespace lti {

  /**
   * Dialog for curve selection
   */
  class gtkCurveSelection {
  public:
    /**
     * Create the dialog
     */
    gtkCurveSelection();

    /**
     * Destructor
     */
    virtual ~gtkCurveSelection();

    /**
     * Show the dialog
     */
    void show();

    /**
     * Hide the dialog
     */
    void hide();

    /**
     * Destroy the dialog
     */
    void destroy();

    /**
     * Specify the vector of the curve to be used.
     *
     * At the end, this vector will be unmodified if the selector is not used or
     * "cancel" is pressed or it will be filled with the corresponding data.
     *
     * The lenght is not modified.
     */
    void use(vector<ubyte>& data);

  protected:

    /**
     * Construct the GTK widgets
     */
    void build();

    GtkWidget *curveSelector_;
    GtkWidget *dialogVBox_;
    GtkWidget *gammaCurve_;
    GtkWidget *dialogActionArea_;
    GtkWidget *cancelButton_;
    GtkWidget *okButton_;

    vector<ubyte>* holder_;

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
     * Callback function called when the user has pressed the cancel button
     */
    static void cbOnOkButtonClicked(GtkButton* button,
                                    gpointer data);

    /**
     * Transfer information from the curve to the holder vector
     */
    void transferFromCurve(GtkCurve* curve);
                      
    /**
     * Transfer information from the holder vector to the curve 
     */
    void transferFromVector(GtkCurve* curve);
  };


  /**
   * Private class of the configuration dialogs which deals with
   * all internal issues of the GTK toolkit.
   */
  class viewer2D::configDialog::tk : public viewerBase::configDialog::tk {
  public:
    /**
     * Constructor.
     *
     * This constructor does not call build(), since at creation time GTK
     * may not be initialized yet.
     */
    tk(viewer2D::configDialog& parent);

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
    virtual bool updateInformation(const viewer2D::information& info);


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
    GtkWidget *vboxTop_;
    GtkWidget *displayCtrlFrame_;
    GtkWidget *displayCtrlAlignment_;
    GtkWidget *disCtrlVBox_;
    GtkWidget *zoomHBox_;
    GtkWidget *zoomLabel_;
    GtkWidget *zoomEntry_;
    GtkWidget *zoomHScale_;
    GtkAdjustment* zoomHAdjustment_;
    GtkWidget *linearButton_;
    GSList *linearButton__group;
    GtkWidget *offsetSlopeHBox_;
    GtkWidget *offsetButton_;
    GtkWidget *offsetEntry_;
    GtkWidget *offsetHScale_;
    GtkAdjustment* offsetHAdjustment_;
    GtkWidget *slopeLabel_;
    GtkWidget *slopeEntry_;
    GtkWidget *slopeHScale_;
    GtkAdjustment* slopeHAdjustment_;
    GtkWidget *mappingsTable_;
    GtkWidget *minButton_;
    GtkWidget *maxButton_;
    GtkWidget *optLinButton_;
    GtkWidget *histEqButton_;
    GtkWidget *logButton_;
    GtkWidget *optLogButton_;
    GtkWidget *userCurveHBox_;
    GtkWidget *userDefButton_;
    GtkWidget *editCurveButton_;
    GtkWidget *hexColorVBox_;
    GtkWidget *hexButton_;
    GtkWidget *underflowColorButton_;
    GtkWidget *overflowColorButton_;
    GtkWidget *displayLabel_;
    GtkWidget *paletteCtrlFrame_;
    GtkWidget *paletteCtrlAlignment_;
    GtkWidget *paletteCtrlVBox_;
    GtkWidget *paletteColorHBox_;
    GtkWidget *grayPaletteButton_;
    GSList *grayPaletteButton__group;
    GtkWidget *redPaletteButton_;
    GtkWidget *greenPaletteButton_;
    GtkWidget *bluePaletteButton_;
    GtkWidget *huePaletteButton_;
    GtkWidget *userPaletteButton_;
    GtkWidget *labelAdjacencyAnalysisButton_;
    GtkWidget *labelAdjacencyRadioBox_;
    GtkWidget *fourNeighborhoodButton_;
    GSList *fourNeighborhoodButton__group;
    GtkWidget *eightNeighborhoodButton_;
    GtkWidget *minColorsButton_;
    GtkWidget *paletteCtrlLabel_;
    GtkWidget *informationFrame_;
    GtkWidget *infoHBox_;
    GtkWidget *generalStatsTable_;
    GtkWidget *generalStatsRightTable_;
    GtkWidget *imgSizeLabel_;
    GtkWidget *minValLabel_;
    GtkWidget *maxValLabel_;
    GtkWidget *avrgValLabel_;
    GtkWidget *stdDevLabel_;
    GtkWidget *imgSizeEntry_;
    GtkWidget *minValEntry_;
    GtkWidget *maxValEntry_;
    GtkWidget *avrgValEntry_;
    GtkWidget *stdDevEntry_;
    GtkWidget *covLabel_;
    GtkWidget *covarTable_;
    GtkWidget *rgEntry_;
    GtkWidget *rbEntry_;
    GtkWidget *ggEntry_;
    GtkWidget *bgEntry_;
    GtkWidget *grEntry_;
    GtkWidget *brEntry_;
    GtkWidget *gbEntry_;
    GtkWidget *bbEntry_;
    GtkWidget *rrEntry_;
    GtkWidget *infoLabel_;
    GtkWidget *ioFrame_;
    GtkWidget *saveImgAlignment_;
    GtkWidget *saveHBox_;
    GtkWidget *filenameEntry_;
    GtkWidget *browseButton_;
    GtkWidget *saveButton_;
    GtkWidget *saveImageLabel_;
    GtkTooltips *tooltips_;

    gtkCurveSelection* curveSelector_;
    //@}

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
     * If the browse button is clicked
     */
    static void cbOnBrowseButtonClicked(GtkButton *button,
                                        gpointer   userData);

    static void cbOnSaveButtonClicked(GtkButton *button,
                                      gpointer   userData);

    /**
     * Callback for zoom text entry
     */
    static void cbOnZoomEntryActivate(GtkCellEditable *celleditable,
                                      gpointer         userData);
    
    /**
     * Callback for zoom horizontal scale bar
     */
    static void cbOnZoomHAdjustmentValueChanged(GtkAdjustment* scale,
                                                gpointer       userData);
    
    /**
     * Callback if the user typed something in the offset entry
     */
    static void cbOnOffsetEntryActivate(GtkCellEditable *celleditable,
                                        gpointer         userData);
    
    /**
     * Callback if the user moved the offset slider
     */
    static void cbOnOffsetHAdjustmentValueChanged(GtkAdjustment* scale,
                                                  gpointer       userData);

    /**
     * Callback if the user typed something in the slope entry
     */
    static void cbOnSlopeEntryActivate(GtkCellEditable *celleditable,
                                       gpointer         userData);

    /**
     * Callback if the user moved the slope slider
     */
    static void cbOnSlopeHAdjustmentValueChanged(GtkAdjustment* scale,
                                                 gpointer       userData);

    /**
     * Callback for radio buttons to select a mapping type
     */
    static void cbOnMappingRadioToggled(GtkToggleButton* toggleButton,
                                        gpointer userData);
    
    /**
     * Callback when the curve edit button is clicked
     */
    static void cbOnCurveButtonClicked(GtkButton *button,
                                       gpointer   userData);
    
    /**
     * Callback when hex display button is clicked
     */
    static void cbOnHexButtonToggled(GtkToggleButton* toggleButton,
                                     gpointer userData);

    /**
     * Callback when underflow color selection button is clicked
     */
    static void cbOnUnderflowColorButtonSet(GtkColorButton  *colorbutton,
                                            gpointer         userData);

    /**
     * Callback when overflow color selection button is clicked
     */
    static void cbOnOverflowColorButtonSet(GtkColorButton  *colorbutton,
                                           gpointer         userData);

    
    /**
     * Callback for radio buttons to select the palette type
     */
    static void cbOnPaletteRadioToggled(GtkToggleButton* toggleButton,
                                        gpointer userData);

    /**
     * Callback when hex display button is clicked
     */
    static void cbOnLabelAdjacencyButtonToggled(GtkToggleButton* toggleButton,
                                                gpointer userData);

    /**
     * Callback for radio buttons to select the palette type
     */
    static void cbOnNeighborhoodRadioToggled(GtkToggleButton* toggleButton,
                                             gpointer userData);

    /**
     * Callback for min number of colors
     */
    static void cbOnMinColorsButtonToggled(GtkToggleButton* toggleButton,
                                           gpointer userData);

    /**
     * Callback when the filename has been changed
     */
    static void cbOnFilenameEntryActivate(GtkCellEditable *celleditable,
                                          gpointer         userData);
    

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
    void setZoom(const float zoom);

    /**
     * Get the proper zoom value from the GUI elements
     */
    float getZoom() const;

    /**
     * Set the GUI elements to the proper mapping style
     */
    void setMapping(const viewer2DPainter::eMappingType mapping);

    /**
     * Get the proper mapping style from the GUI elements
     */
    viewer2DPainter::eMappingType getMapping();


    /**
     * Set the GUI elements to the proper palette type
     */
    void setPaletteType(const viewer2DPainter::ePaletteType pal);

    /**
     * Get the proper mapping style from the GUI elements
     */
    viewer2DPainter::ePaletteType getPaletteType();

    /**
     * Set the GUI elements to the proper neighborhood
     */
    void setNeighborhood(const viewer2DPainter::eNeighborhoodType n);

    /**
     * Get the proper neighborhood from the GUI elements
     */
    viewer2DPainter::eNeighborhoodType getNeighborhood();

    /**
     * Set the adjacency flag and activate/deactivate all necessary GUI
     * elements
     */
    void setAdjacency(const bool adjOn,
                      const eDataType type);

    /**
     * Data type being displayed
     */
    eDataType dataType_;
  };


  class viewer2D::mainWindow::tk : public viewerBase::mainWindow::tk {
  public:
    /**
     * Constructor.
     *
     * This constructor does not call build(), since at creation time GTK
     * may not be initialized yet.
     */
    tk(viewer2D::mainWindow& parent);

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
