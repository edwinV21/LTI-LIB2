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
 * \file    ltiViewer2DGtk.cpp
 * \author Pablo Alvarado
 * \date    05.07.2005
 * 
 * $Id: ltiViewer2DGtk.cpp,v 1.8 2012-09-16 05:05:59 alvarado Exp $
 */

#include "ltiConfig.h"

#ifdef HAVE_GTK

#include "ltiViewer2DGtk.h"
#include "ltiGuiServer.h"
#include "ltiRound.h"
#include "ltiIOImage.h"

#include <cstdlib> // for atof

// --------------------------------------------------------------------------
// Some Debuggin Macros

#undef _LTI_DEBUG
// #define _LTI_DEBUG 4
#include "ltiDebug.h"

// end of debugging macro definitions
// --------------------------------------------------------------------------


namespace lti {

  // ------------------------------------------------------------------------
  //   Configuration Dialog's toolkit
  // ------------------------------------------------------------------------

  gtkCurveSelection::gtkCurveSelection() {
    curveSelector_=0;
    dialogVBox_=0;
    gammaCurve_=0;
    dialogActionArea_=0;
    cancelButton_=0;
    okButton_=0;
    holder_=0;
  }

  gtkCurveSelection::~gtkCurveSelection() {
    hide();
    destroy();
  }

  void gtkCurveSelection::show() {
    if (isNull(curveSelector_)) {
      build();
    }
    gtk_widget_show(curveSelector_);
  }

  void gtkCurveSelection::hide() {
    gtk_widget_hide(curveSelector_);
  }

  void gtkCurveSelection::destroy() {
    gtk_widget_destroy(curveSelector_);
    curveSelector_=0;
    dialogVBox_=0;
    gammaCurve_=0;
    dialogActionArea_=0;
    cancelButton_=0;
    okButton_=0;
    holder_=0;
  }

  void gtkCurveSelection::use(vector<ubyte>& data) {
    holder_=&data;
    
    if (notNull(gammaCurve_)) {
      transferFromVector(GTK_CURVE(GTK_GAMMA_CURVE(gammaCurve_)->curve));
    }
  }

  void gtkCurveSelection::build() {
    if (notNull(curveSelector_)) {
      // already build
      return;
    }

    curveSelector_ = gtk_dialog_new ();
    gtk_window_set_title (GTK_WINDOW (curveSelector_), "User defined mapping");
    gtk_window_set_type_hint (GTK_WINDOW (curveSelector_),
                              GDK_WINDOW_TYPE_HINT_DIALOG);

    dialogVBox_ = GTK_DIALOG (curveSelector_)->vbox;
    gtk_widget_show (dialogVBox_);

    gammaCurve_ = gtk_gamma_curve_new ();
    gtk_widget_show (gammaCurve_);
    gtk_box_pack_start (GTK_BOX (dialogVBox_), gammaCurve_, TRUE, TRUE, 0);
    gtk_curve_set_range (GTK_CURVE (GTK_GAMMA_CURVE (gammaCurve_)->curve),
                         0, 1, 0, 1);
    gtk_widget_set_size_request(gammaCurve_,291,256);

    dialogActionArea_ = GTK_DIALOG (curveSelector_)->action_area;
    gtk_widget_show (dialogActionArea_);

    cancelButton_ = gtk_button_new_from_stock ("gtk-cancel");
    gtk_widget_show (cancelButton_);
    gtk_dialog_add_action_widget (GTK_DIALOG (curveSelector_),
                                  cancelButton_, GTK_RESPONSE_CANCEL);
    GTK_WIDGET_SET_FLAGS (cancelButton_, GTK_CAN_DEFAULT);

    okButton_ = gtk_button_new_from_stock ("gtk-ok");
    gtk_widget_show (okButton_);
    gtk_dialog_add_action_widget (GTK_DIALOG (curveSelector_), okButton_,
                                  GTK_RESPONSE_OK);
    GTK_WIDGET_SET_FLAGS (okButton_, GTK_CAN_DEFAULT);

    g_signal_connect (G_OBJECT (curveSelector_), "delete_event",
                      G_CALLBACK (cbCloseWindow), this);

    g_signal_connect (G_OBJECT(cancelButton_), "clicked",
                      G_CALLBACK (cbOnCancelButtonClicked),
                      this);

    g_signal_connect (G_OBJECT(okButton_), "clicked",
                      G_CALLBACK (cbOnOkButtonClicked),
                      this);

  }

  gint gtkCurveSelection::cbCloseWindow(GtkWidget *,
                                        GdkEvent  *,
                                        gpointer   data) {
    gtkCurveSelection* me = reinterpret_cast<gtkCurveSelection*>(data);

    cbOnCancelButtonClicked(GTK_BUTTON(me->cancelButton_),data);

    /*
     * Change TRUE to FALSE and the main window will be destroyed with
     * a "delete_event".
     */
    return(TRUE); // TRUE indicates that the event has been handled
  }


  void gtkCurveSelection::cbOnCancelButtonClicked(GtkButton *,
                                                  gpointer   data) {
    gtkCurveSelection* me = reinterpret_cast<gtkCurveSelection*>(data);

    me->hide();
  }

  void gtkCurveSelection::cbOnOkButtonClicked(GtkButton *,
                                                         gpointer   data) {
    gtkCurveSelection* me = reinterpret_cast<gtkCurveSelection*>(data);
    me->transferFromCurve(GTK_CURVE(GTK_GAMMA_CURVE(me->gammaCurve_)->curve));

    me->hide();
  }

  void gtkCurveSelection::transferFromCurve(GtkCurve* curve) {
    if (isNull(holder_) || holder_->empty()) {
      return;
    }

    const int size=holder_->size();
    fvector tvct(size);
    gtk_curve_get_vector(curve,size,tvct.data());

    int i;
    for (i=0;i<size;++i) {
      // GTK has a small bug that the given interval is not strictly 
      // considered, so that values equivalent to 256 can be obtained.
      // we make a clipping here:
      const float val = tvct.at(i)*255;
      if (val>=255.5f) {
        holder_->at(i)=255;
      } else if (val<0.0f) {
        holder_->at(i)=0;
      } else {
        holder_->at(i)=static_cast<ubyte>(val+0.5f);
      }
    }
    
  }

  void gtkCurveSelection::transferFromVector(GtkCurve* curve) {
    if (isNull(holder_)) {
      return;
    }
    
    const int size = holder_->size();
    fvector tvct(size);
    int i;
    for (i=0;i<size;++i) {
      tvct.at(i)=static_cast<float>(holder_->at(i))/255.0f;
    }
    gtk_curve_set_vector(curve,size,tvct.data());
  }

  // ------------------------------------------------------------------------
  //   Configuration Dialog's toolkit
  // ------------------------------------------------------------------------
  
  viewer2D::configDialog::tk::tk(viewer2D::configDialog& parent) :
    viewerBase::configDialog::tk::tk(parent) {
    _lti_enterCTOR();
    curveSelector_=0;
    _lti_leaveCTOR();
  }

  viewer2D::configDialog::tk::~tk() {
    _lti_enterDTOR();
    // done by the "parent" class
    // hide();
    // destroy();
    _lti_leaveDTOR();
  }

  bool viewer2D::configDialog::tk::build() {
    _lti_debug2("entering viewer2D::configDialog::tk::build()\n");

    if (!viewerBase::configDialog::tk::build()) {
      // window already build!  This shouldn't happen, but it has!
      return false;
    }

    lock_.lock();
    // gdk_threads_enter();

    // the additional controls in the dialog window
    linearButton__group = 0;    
    grayPaletteButton__group = 0;
    fourNeighborhoodButton__group = 0;

    // the following code has been imported and modified from glade-2
    tooltips_ = gtk_tooltips_new ();

    // vbox_ is inherited from viewerBaseGtk
    // the vboxTop_ contains everything
    vboxTop_ = gtk_vbox_new (FALSE, 0);
    gtk_widget_set_name (vboxTop_, "vboxTop_");
    gtk_widget_show (vboxTop_);
    gtk_box_pack_start (GTK_BOX (vbox_), vboxTop_, FALSE, TRUE, 0);
    
    displayCtrlFrame_ = gtk_frame_new (NULL);
    gtk_widget_set_name (displayCtrlFrame_, "displayCtrlFrame_");
    gtk_widget_show (displayCtrlFrame_);
    gtk_box_pack_start (GTK_BOX (vboxTop_), displayCtrlFrame_, FALSE, TRUE, 0);

    displayCtrlAlignment_ = gtk_alignment_new (0.5, 0.5, 1, 1);
    gtk_widget_set_name (displayCtrlAlignment_, "displayCtrlAlignment_");
    gtk_widget_show (displayCtrlAlignment_);
    gtk_container_add (GTK_CONTAINER (displayCtrlFrame_),
                       displayCtrlAlignment_);
    gtk_alignment_set_padding (GTK_ALIGNMENT (displayCtrlAlignment_), 
                               0, 0, 4, 0);

    disCtrlVBox_ = gtk_vbox_new (FALSE, 0);
    gtk_widget_set_name (disCtrlVBox_, "disCtrlVBox_");
    gtk_widget_show (disCtrlVBox_);
    gtk_container_add (GTK_CONTAINER (displayCtrlAlignment_), disCtrlVBox_);
    
    zoomHBox_ = gtk_hbox_new (FALSE, 0);
    gtk_widget_set_name (zoomHBox_, "zoomHBox_");
    gtk_widget_show (zoomHBox_);
    gtk_box_pack_start (GTK_BOX (disCtrlVBox_), zoomHBox_, FALSE, TRUE, 0);
    
    zoomLabel_ = gtk_label_new ("Zoom");
    gtk_widget_set_name (zoomLabel_, "zoomLabel_");
    gtk_widget_show (zoomLabel_);
    gtk_box_pack_start (GTK_BOX (zoomHBox_), zoomLabel_, FALSE, FALSE, 0);
    gtk_misc_set_padding (GTK_MISC (zoomLabel_), 2, 0);
    
    zoomEntry_ = gtk_entry_new ();
    gtk_widget_set_name (zoomEntry_, "zoomEntry_");
    gtk_widget_show (zoomEntry_);
    gtk_box_pack_start (GTK_BOX (zoomHBox_), zoomEntry_, FALSE, TRUE, 0);
    gtk_tooltips_set_tip (tooltips_, zoomEntry_, "n for zoom-in or 1/n for " \
                          "zoom-out, with n a positive integer less or equal "\
                          "32.", NULL);
    gtk_entry_set_max_length (GTK_ENTRY (zoomEntry_), 5);
    gtk_entry_set_text (GTK_ENTRY (zoomEntry_), "1");
    gtk_entry_set_width_chars (GTK_ENTRY (zoomEntry_), 7);
    
    zoomHAdjustment_ = GTK_ADJUSTMENT(gtk_adjustment_new (0,-30,32,1,5,0));
    zoomHScale_ = gtk_hscale_new (GTK_ADJUSTMENT ( zoomHAdjustment_ ));

    gtk_widget_set_name (zoomHScale_, "zoomHScale_");
    gtk_widget_show (zoomHScale_);
    gtk_box_pack_start (GTK_BOX (zoomHBox_), zoomHScale_, TRUE, TRUE, 0);
    gtk_scale_set_draw_value (GTK_SCALE (zoomHScale_), FALSE);
    gtk_scale_set_digits (GTK_SCALE (zoomHScale_), 3);
    
    linearButton_ =
      gtk_radio_button_new_with_mnemonic (NULL, "Original linear mapping");
    gtk_widget_set_name (linearButton_, "linearButton_");
    gtk_widget_show (linearButton_);
    gtk_box_pack_start (GTK_BOX (disCtrlVBox_),linearButton_, FALSE, FALSE, 0);
    gtk_radio_button_set_group (GTK_RADIO_BUTTON (linearButton_), 
                                linearButton__group);
    linearButton__group =
      gtk_radio_button_get_group (GTK_RADIO_BUTTON (linearButton_));
    
    offsetSlopeHBox_ = gtk_hbox_new (FALSE, 0);
    gtk_widget_set_name (offsetSlopeHBox_, "offsetSlopeHBox_");
    gtk_widget_show (offsetSlopeHBox_);
    gtk_box_pack_start (GTK_BOX (disCtrlVBox_), 
                        offsetSlopeHBox_, FALSE, TRUE, 0);
    
    offsetButton_ = gtk_radio_button_new_with_mnemonic (NULL, "Offset");
    gtk_widget_set_name (offsetButton_, "offsetButton_");
    gtk_widget_show (offsetButton_);
    gtk_box_pack_start (GTK_BOX (offsetSlopeHBox_),
                        offsetButton_, FALSE, FALSE, 0);
    gtk_radio_button_set_group (GTK_RADIO_BUTTON (offsetButton_),
                                linearButton__group);
    gtk_tooltips_set_tip (tooltips_, offsetButton_, 
                          "Always between -1 and 1", NULL);

    linearButton__group =
      gtk_radio_button_get_group (GTK_RADIO_BUTTON (offsetButton_));

    offsetEntry_ = gtk_entry_new ();
    gtk_widget_set_name (offsetEntry_, "offsetEntry_");
    gtk_widget_show (offsetEntry_);
    gtk_box_pack_start (GTK_BOX (offsetSlopeHBox_),
                        offsetEntry_, FALSE, TRUE, 0);
    gtk_tooltips_set_tip (tooltips_, offsetEntry_, 
                 "Set to zero for default", NULL);
    gtk_entry_set_max_length (GTK_ENTRY (offsetEntry_), 16);
    gtk_entry_set_text (GTK_ENTRY (offsetEntry_), "0");
    gtk_entry_set_width_chars (GTK_ENTRY (offsetEntry_), 7);
    
    offsetHAdjustment_ = GTK_ADJUSTMENT(gtk_adjustment_new(0,-1.0,1.0,
                                                           1.0/512.0,
                                                           1.0/16.0,0));

    offsetHScale_ = gtk_hscale_new (GTK_ADJUSTMENT (offsetHAdjustment_));
    gtk_widget_set_name (offsetHScale_, "offsetHScale_");
    gtk_widget_show (offsetHScale_);
    gtk_box_pack_start (GTK_BOX (offsetSlopeHBox_), offsetHScale_,
                        TRUE, TRUE, 0);
    gtk_scale_set_draw_value (GTK_SCALE (offsetHScale_), FALSE);
    gtk_scale_set_digits (GTK_SCALE (offsetHScale_), 3);
    
    slopeLabel_ = gtk_label_new_with_mnemonic ("Slope");
    gtk_widget_set_name (slopeLabel_, "slopeLabel_");
    gtk_widget_show (slopeLabel_);
    gtk_box_pack_start (GTK_BOX (offsetSlopeHBox_), slopeLabel_, 
                        FALSE, FALSE, 0);
    gtk_misc_set_padding (GTK_MISC (slopeLabel_), 4, 0);
    
    slopeEntry_ = gtk_entry_new ();
    gtk_widget_set_name (slopeEntry_, "slopeEntry_");
    gtk_widget_show (slopeEntry_);
    gtk_box_pack_start (GTK_BOX (offsetSlopeHBox_), slopeEntry_,
                        FALSE, TRUE, 0);
    gtk_tooltips_set_tip (tooltips_, slopeEntry_,
                     "Set to 1 for default or -1 if you want to invert", NULL);
    gtk_entry_set_max_length (GTK_ENTRY (slopeEntry_), 16);
    gtk_entry_set_text (GTK_ENTRY (slopeEntry_), "1");
    gtk_entry_set_width_chars (GTK_ENTRY (slopeEntry_), 7);
    
    slopeHAdjustment_ = GTK_ADJUSTMENT(gtk_adjustment_new (1,-16,16,
                                                           0.05,0.5,0));
    slopeHScale_ 
      = gtk_hscale_new (GTK_ADJUSTMENT (slopeHAdjustment_));
    gtk_widget_set_name (slopeHScale_, "slopeHScale_");
    gtk_widget_show (slopeHScale_);
    gtk_box_pack_start (GTK_BOX (offsetSlopeHBox_), slopeHScale_, 
                        TRUE, TRUE, 0);
    gtk_scale_set_draw_value (GTK_SCALE (slopeHScale_), FALSE);
    gtk_scale_set_digits (GTK_SCALE (slopeHScale_), 3);
    
    mappingsTable_ = gtk_table_new (3, 2, FALSE);
    gtk_widget_set_name (mappingsTable_, "mappingsTable_");
    gtk_widget_show (mappingsTable_);
    gtk_box_pack_start (GTK_BOX (disCtrlVBox_), mappingsTable_,
                        FALSE, FALSE, 0);
    
    minButton_ = gtk_radio_button_new_with_mnemonic(NULL, "Minimum to black");
    gtk_widget_set_name (minButton_, "minButton_");
    gtk_widget_show (minButton_);
    gtk_table_attach (GTK_TABLE (mappingsTable_), minButton_, 0, 1, 0, 1,
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_tooltips_set_tip (tooltips_, minButton_, "Linear function maps " \
                          "minimum value to black and 255 to white", NULL);
    gtk_radio_button_set_group (GTK_RADIO_BUTTON (minButton_), 
                                linearButton__group);
    linearButton__group =
      gtk_radio_button_get_group (GTK_RADIO_BUTTON (minButton_));
    
    maxButton_ = gtk_radio_button_new_with_mnemonic(NULL, "Maximum to white");
    gtk_widget_set_name (maxButton_, "maxButton_");
    gtk_widget_show (maxButton_);
    gtk_table_attach (GTK_TABLE (mappingsTable_), maxButton_, 1, 2, 0, 1,
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_tooltips_set_tip (tooltips_, maxButton_,"Linear function maps 0 to " \
                          "black and maximum value to 255", NULL);
    gtk_radio_button_set_group (GTK_RADIO_BUTTON (maxButton_), 
                                linearButton__group);
    linearButton__group =
      gtk_radio_button_get_group (GTK_RADIO_BUTTON (maxButton_));

    optLinButton_ 
      = gtk_radio_button_new_with_mnemonic (NULL, "Optimal linear mapping");
    gtk_widget_set_name (optLinButton_, "optLinButton_");
    gtk_widget_show (optLinButton_);
    gtk_table_attach (GTK_TABLE (mappingsTable_), optLinButton_, 0, 1, 1, 2,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_tooltips_set_tip (tooltips_, optLinButton_,"Linear function maps " \
                          "minimum value to black and maximum value to 255",
                          NULL);
    gtk_radio_button_set_group (GTK_RADIO_BUTTON (optLinButton_),
                                linearButton__group);
    linearButton__group = 
      gtk_radio_button_get_group (GTK_RADIO_BUTTON (optLinButton_));
    
    histEqButton_ = 
      gtk_radio_button_new_with_mnemonic (NULL, "Histogram equalization");
    gtk_widget_set_name (histEqButton_, "histEqButton_");
    gtk_widget_show (histEqButton_);
    gtk_table_attach (GTK_TABLE (mappingsTable_), histEqButton_, 1, 2, 1, 2,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_tooltips_set_tip (tooltips_, histEqButton_,
                          "Non-linear transformation that tries to equalize " \
                          "the histogram", NULL);
    gtk_radio_button_set_group (GTK_RADIO_BUTTON (histEqButton_), 
                                linearButton__group);
    linearButton__group =
      gtk_radio_button_get_group (GTK_RADIO_BUTTON (histEqButton_));
    
    logButton_ = gtk_radio_button_new_with_mnemonic (NULL,
                                                     "Logarithmic curve");
    gtk_widget_set_name (logButton_, "logButton_");
    gtk_widget_show (logButton_);
    gtk_table_attach (GTK_TABLE (mappingsTable_), logButton_, 0, 1, 2, 3,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_tooltips_set_tip (tooltips_, logButton_,
                          "Applies the curve log(x+1)/log(256)", NULL);
    gtk_radio_button_set_group (GTK_RADIO_BUTTON (logButton_), 
                                linearButton__group);
    linearButton__group = 
      gtk_radio_button_get_group (GTK_RADIO_BUTTON (logButton_));
    
    optLogButton_ = 
      gtk_radio_button_new_with_mnemonic (NULL, "Optimal logarithmic curve");
    gtk_widget_set_name (optLogButton_, "optLogButton_");
    gtk_widget_show (optLogButton_);
    gtk_table_attach (GTK_TABLE (mappingsTable_), optLogButton_, 1, 2, 2, 3,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_tooltips_set_tip (tooltips_, optLogButton_, 
                          "Linearly maps the input values to the interval " \
                          "0..255 and then applies log(1+x)/log(256)", NULL);
    gtk_radio_button_set_group (GTK_RADIO_BUTTON (optLogButton_),
                                linearButton__group);
    linearButton__group =
      gtk_radio_button_get_group (GTK_RADIO_BUTTON (optLogButton_));
    
    userCurveHBox_ = gtk_hbox_new (FALSE, 0);
    gtk_widget_set_name (userCurveHBox_, "userCurveHBox_");
    gtk_widget_show (userCurveHBox_);
    gtk_box_pack_start (GTK_BOX (disCtrlVBox_), userCurveHBox_,
                        FALSE, TRUE, 0);
    
    userDefButton_ = 
      gtk_radio_button_new_with_mnemonic (NULL, "User defined mapping   ");
    gtk_widget_set_name (userDefButton_, "userDefButton_");
    gtk_widget_show (userDefButton_);
    gtk_box_pack_start (GTK_BOX (userCurveHBox_), userDefButton_, 
                        FALSE, FALSE, 0);
    gtk_tooltips_set_tip(tooltips_, userDefButton_,
                         "Arbitrary LUT with 256 entries can be painted",NULL);
    gtk_radio_button_set_group (GTK_RADIO_BUTTON (userDefButton_),
                                linearButton__group);
    linearButton__group = 
      gtk_radio_button_get_group (GTK_RADIO_BUTTON (userDefButton_));
    
    editCurveButton_ = gtk_button_new_with_mnemonic ("Edit Curve");
    gtk_widget_set_name (editCurveButton_, "editCurveButton_");
    gtk_widget_show (editCurveButton_);
    gtk_box_pack_start (GTK_BOX (userCurveHBox_), editCurveButton_,
                        FALSE, FALSE, 0);
    
    hexColorVBox_ = gtk_hbox_new (FALSE, 4);
    gtk_widget_set_name (hexColorVBox_, "hexColorVBox_");
    gtk_widget_show (hexColorVBox_);
    gtk_box_pack_start (GTK_BOX (disCtrlVBox_), hexColorVBox_, FALSE, TRUE, 0);
    
    hexButton_ = 
      gtk_check_button_new_with_mnemonic ("Display pixel information " \
                                          "in hexadecimal");
    gtk_widget_set_name (hexButton_, "hexButton_");
    gtk_widget_show (hexButton_);
    gtk_box_pack_start (GTK_BOX (hexColorVBox_), hexButton_, TRUE, TRUE, 0);
    gtk_tooltips_set_tip(tooltips_, hexButton_,
                         "Should the pixel information at the bottom of the " \
                         "viewer window be displayed in hexadecimal?",NULL);
    
    underflowColorButton_ = gtk_color_button_new ();
    gtk_widget_set_name (underflowColorButton_, "underflowColorButton_");
    gtk_widget_show (underflowColorButton_);
    gtk_box_pack_start (GTK_BOX (hexColorVBox_), underflowColorButton_,
                        TRUE, TRUE, 0);
    gtk_tooltips_set_tip (tooltips_, underflowColorButton_,
                          "Select underflow color", NULL);
    gtk_color_button_set_title (GTK_COLOR_BUTTON (underflowColorButton_),
                                "Underflow color");
    
    overflowColorButton_ = gtk_color_button_new ();
    gtk_widget_set_name (overflowColorButton_, "overflowColorButton_");
    gtk_widget_show (overflowColorButton_);
    gtk_box_pack_start (GTK_BOX (hexColorVBox_), overflowColorButton_,
                        TRUE, TRUE, 0);
    gtk_tooltips_set_tip (tooltips_, overflowColorButton_,
                          "Select overflow color", NULL);
    gtk_color_button_set_title (GTK_COLOR_BUTTON (overflowColorButton_),
                                "Overflow color");
    
    displayLabel_ = gtk_label_new ("Display Control");
    gtk_widget_set_name (displayLabel_, "displayLabel_");
    gtk_widget_show (displayLabel_);
    gtk_frame_set_label_widget (GTK_FRAME (displayCtrlFrame_), displayLabel_);
    gtk_label_set_use_markup (GTK_LABEL (displayLabel_), TRUE);
    
    paletteCtrlFrame_ = gtk_frame_new (NULL);
    gtk_widget_set_name (paletteCtrlFrame_, "paletteCtrlFrame_");
    gtk_widget_show (paletteCtrlFrame_);
    gtk_box_pack_start (GTK_BOX (vboxTop_), paletteCtrlFrame_, TRUE, TRUE, 0);
    
    paletteCtrlAlignment_ = gtk_alignment_new (0.5, 0, 1, 1);
    gtk_widget_set_name (paletteCtrlAlignment_, "paletteCtrlAlignment_");
    gtk_widget_show (paletteCtrlAlignment_);
    gtk_container_add (GTK_CONTAINER (paletteCtrlFrame_),
                       paletteCtrlAlignment_);
    gtk_alignment_set_padding (GTK_ALIGNMENT (paletteCtrlAlignment_),
                               0, 0, 4, 0);
    
    paletteCtrlVBox_ = gtk_vbox_new (FALSE, 0);
    gtk_widget_set_name (paletteCtrlVBox_, "paletteCtrlVBox_");
    gtk_widget_show (paletteCtrlVBox_);
    gtk_container_add(GTK_CONTAINER (paletteCtrlAlignment_), paletteCtrlVBox_);
    
    paletteColorHBox_ = gtk_hbox_new (TRUE, 0);
    gtk_widget_set_name (paletteColorHBox_, "paletteColorHBox_");
    gtk_widget_show (paletteColorHBox_);
    gtk_box_pack_start (GTK_BOX (paletteCtrlVBox_), paletteColorHBox_,
                        TRUE, TRUE, 0);
    
    grayPaletteButton_ = gtk_radio_button_new_with_mnemonic (NULL, "Gray");
    gtk_widget_set_name (grayPaletteButton_, "grayPaletteButton_");
    gtk_widget_show (grayPaletteButton_);
    gtk_box_pack_start (GTK_BOX (paletteColorHBox_), grayPaletteButton_,
                        FALSE, FALSE, 0);
    gtk_radio_button_set_group (GTK_RADIO_BUTTON (grayPaletteButton_),
                                grayPaletteButton__group);
    grayPaletteButton__group =
      gtk_radio_button_get_group(GTK_RADIO_BUTTON (grayPaletteButton_));
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON (grayPaletteButton_), TRUE);
    
    redPaletteButton_ = gtk_radio_button_new_with_mnemonic (NULL, "Red");
    gtk_widget_set_name (redPaletteButton_, "redPaletteButton_");
    gtk_widget_show (redPaletteButton_);
    gtk_box_pack_start (GTK_BOX (paletteColorHBox_), redPaletteButton_,
                        FALSE, FALSE, 0);
    gtk_radio_button_set_group (GTK_RADIO_BUTTON (redPaletteButton_), 
                                grayPaletteButton__group);
    grayPaletteButton__group = 
      gtk_radio_button_get_group (GTK_RADIO_BUTTON (redPaletteButton_));
    
    greenPaletteButton_ = gtk_radio_button_new_with_mnemonic (NULL, "Green");
    gtk_widget_set_name (greenPaletteButton_, "greenPaletteButton_");
    gtk_widget_show (greenPaletteButton_);
    gtk_box_pack_start (GTK_BOX (paletteColorHBox_), greenPaletteButton_,
                        FALSE, FALSE, 0);
    gtk_radio_button_set_group (GTK_RADIO_BUTTON (greenPaletteButton_), 
                                grayPaletteButton__group);
    grayPaletteButton__group = 
      gtk_radio_button_get_group (GTK_RADIO_BUTTON (greenPaletteButton_));
    
    bluePaletteButton_ = gtk_radio_button_new_with_mnemonic (NULL, "Blue");
    gtk_widget_set_name (bluePaletteButton_, "bluePaletteButton_");
    gtk_widget_show (bluePaletteButton_);
    gtk_box_pack_start (GTK_BOX (paletteColorHBox_), bluePaletteButton_, 
                        FALSE, FALSE, 0);
    gtk_radio_button_set_group (GTK_RADIO_BUTTON (bluePaletteButton_), 
                                grayPaletteButton__group);
    grayPaletteButton__group = 
      gtk_radio_button_get_group (GTK_RADIO_BUTTON (bluePaletteButton_));
    
    huePaletteButton_ = gtk_radio_button_new_with_mnemonic (NULL, "Hue");
    gtk_widget_set_name (huePaletteButton_, "huePaletteButton_");
    gtk_widget_show (huePaletteButton_);
    gtk_box_pack_start (GTK_BOX (paletteColorHBox_), huePaletteButton_, 
                        FALSE, FALSE, 0);
    gtk_radio_button_set_group (GTK_RADIO_BUTTON (huePaletteButton_), 
                                grayPaletteButton__group);
    grayPaletteButton__group = 
      gtk_radio_button_get_group (GTK_RADIO_BUTTON (huePaletteButton_));
    
    userPaletteButton_ = gtk_radio_button_new_with_mnemonic(NULL,
                                                            "User defined");
    gtk_widget_set_name (userPaletteButton_, "userPaletteButton_");
    gtk_widget_show (userPaletteButton_);
    gtk_box_pack_start (GTK_BOX (paletteColorHBox_), userPaletteButton_,
                        FALSE, FALSE, 0);
    gtk_radio_button_set_group (GTK_RADIO_BUTTON (userPaletteButton_),
                                grayPaletteButton__group);
    grayPaletteButton__group = 
      gtk_radio_button_get_group (GTK_RADIO_BUTTON (userPaletteButton_));
    
    labelAdjacencyAnalysisButton_ = 
      gtk_check_button_new_with_mnemonic ("Label adjacency analysis");
    gtk_widget_set_name (labelAdjacencyAnalysisButton_, 
                         "labelAdjacencyAnalysisButton_");
    gtk_widget_show (labelAdjacencyAnalysisButton_);
    gtk_box_pack_start (GTK_BOX (paletteCtrlVBox_),
                        labelAdjacencyAnalysisButton_, FALSE, FALSE, 0);
    gtk_tooltips_set_tip (tooltips_, labelAdjacencyAnalysisButton_,
                          "Assume the displayed data is a labeled mask and " \
                          "assign a color to each label such that adjacent " \
                          "regions can have very different colors despite of "\
                          "similar labels.", NULL);
    
    labelAdjacencyRadioBox_ = gtk_hbox_new (TRUE, 0);
    gtk_widget_set_name (labelAdjacencyRadioBox_, "labelAdjacencyRadioBox_");
    gtk_widget_show (labelAdjacencyRadioBox_);
    gtk_box_pack_start (GTK_BOX (paletteCtrlVBox_), labelAdjacencyRadioBox_,
                        TRUE, TRUE, 0);
    
    fourNeighborhoodButton_ = 
      gtk_radio_button_new_with_mnemonic (NULL, "4-Neighborhood");
    gtk_widget_set_name (fourNeighborhoodButton_, "fourNeighborhoodButton_");
    gtk_widget_show (fourNeighborhoodButton_);
    gtk_box_pack_start (GTK_BOX (labelAdjacencyRadioBox_), 
                        fourNeighborhoodButton_, FALSE, FALSE, 0);
    gtk_radio_button_set_group (GTK_RADIO_BUTTON (fourNeighborhoodButton_),
                                fourNeighborhoodButton__group);
    fourNeighborhoodButton__group =
      gtk_radio_button_get_group (GTK_RADIO_BUTTON (fourNeighborhoodButton_));
    
    eightNeighborhoodButton_ =
      gtk_radio_button_new_with_mnemonic (NULL, "8-Neighborhood");
    gtk_widget_set_name (eightNeighborhoodButton_, "eightNeighborhoodButton_");
    gtk_widget_show (eightNeighborhoodButton_);
    gtk_box_pack_start (GTK_BOX (labelAdjacencyRadioBox_), 
                        eightNeighborhoodButton_, FALSE, FALSE, 0);
    gtk_radio_button_set_group (GTK_RADIO_BUTTON (eightNeighborhoodButton_),
                                fourNeighborhoodButton__group);
    fourNeighborhoodButton__group = 
      gtk_radio_button_get_group (GTK_RADIO_BUTTON (eightNeighborhoodButton_));
    
    minColorsButton_ =
      gtk_check_button_new_with_mnemonic ("Use minimum number of colors");
    gtk_widget_set_name (minColorsButton_, "minColorsButton_");
    gtk_widget_show (minColorsButton_);
    gtk_box_pack_start (GTK_BOX (labelAdjacencyRadioBox_), minColorsButton_,
                        FALSE, FALSE, 0);
    
    paletteCtrlLabel_ = gtk_label_new ("Palette Control");
    gtk_widget_set_name (paletteCtrlLabel_, "paletteCtrlLabel_");
    gtk_widget_show (paletteCtrlLabel_);
    gtk_frame_set_label_widget (GTK_FRAME (paletteCtrlFrame_), 
                                paletteCtrlLabel_);
    gtk_label_set_use_markup (GTK_LABEL (paletteCtrlLabel_), TRUE);
    
    informationFrame_ = gtk_frame_new (NULL);
    gtk_widget_set_name (informationFrame_, "informationFrame_");
    gtk_widget_show (informationFrame_);
    gtk_box_pack_start (GTK_BOX (vboxTop_), informationFrame_, 
                        FALSE, FALSE, 0);
    
    infoHBox_ = gtk_hbox_new (FALSE, 0);
    gtk_widget_set_name (infoHBox_, "infoHBox_");
    gtk_widget_show (infoHBox_);
    gtk_container_add (GTK_CONTAINER (informationFrame_), infoHBox_);
    
    generalStatsTable_ = gtk_table_new (4, 2, FALSE);
    gtk_widget_set_name (generalStatsTable_, "generalStatsTable_");
    gtk_widget_show (generalStatsTable_);
    gtk_box_pack_start (GTK_BOX (infoHBox_),generalStatsTable_, TRUE, TRUE, 0);
    gtk_table_set_row_spacings (GTK_TABLE (generalStatsTable_), 2);
    gtk_table_set_col_spacings (GTK_TABLE (generalStatsTable_), 4);
    
    imgSizeLabel_ = gtk_label_new ("Image size");
    gtk_widget_set_name (imgSizeLabel_, "imgSizeLabel_");
    gtk_widget_show (imgSizeLabel_);
    gtk_table_attach (GTK_TABLE (generalStatsTable_),imgSizeLabel_, 0, 1, 0, 1,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (imgSizeLabel_), 0, 0.5);
    
    minValLabel_ = gtk_label_new ("Minimum value");
    gtk_widget_set_name (minValLabel_, "minValLabel_");
    gtk_widget_show (minValLabel_);
    gtk_table_attach (GTK_TABLE (generalStatsTable_), minValLabel_, 0, 1, 1, 2,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_label_set_use_markup (GTK_LABEL (minValLabel_), TRUE);
    gtk_misc_set_alignment (GTK_MISC (minValLabel_), 0, 0.5);
    
    maxValLabel_ = gtk_label_new ("Maximum value");
    gtk_widget_set_name (maxValLabel_, "maxValLabel_");
    gtk_widget_show (maxValLabel_);
    gtk_table_attach (GTK_TABLE (generalStatsTable_), maxValLabel_, 0, 1, 2, 3,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (maxValLabel_), 0, 0.5);
    
    avrgValLabel_ = gtk_label_new ("Average value");
    gtk_widget_set_name (avrgValLabel_, "avrgValLabel_");
    gtk_widget_show (avrgValLabel_);
    gtk_table_attach (GTK_TABLE (generalStatsTable_), avrgValLabel_, 
                      0, 1, 3, 4,(GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (avrgValLabel_), 0, 0.5);
    
    imgSizeEntry_ = gtk_entry_new ();
    gtk_widget_set_name (imgSizeEntry_, "imgSizeEntry_");
    gtk_widget_show (imgSizeEntry_);
    gtk_table_attach (GTK_TABLE (generalStatsTable_),imgSizeEntry_, 1, 2, 0, 1,
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_editable_set_editable (GTK_EDITABLE (imgSizeEntry_), FALSE);
    gtk_entry_set_text (GTK_ENTRY (imgSizeEntry_), "1024x1024");
    gtk_entry_set_has_frame (GTK_ENTRY (imgSizeEntry_), FALSE);
    gtk_entry_set_width_chars (GTK_ENTRY (imgSizeEntry_), 14);
    
    minValEntry_ = gtk_entry_new ();
    gtk_widget_set_name (minValEntry_, "minValEntry_");
    gtk_widget_show (minValEntry_);
    gtk_table_attach (GTK_TABLE (generalStatsTable_), minValEntry_, 1, 2, 1, 2,
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_editable_set_editable (GTK_EDITABLE (minValEntry_), FALSE);
    gtk_entry_set_text (GTK_ENTRY (minValEntry_), "(0,0,0)");
    gtk_entry_set_has_frame (GTK_ENTRY (minValEntry_), FALSE);
    gtk_entry_set_width_chars (GTK_ENTRY (minValEntry_), 14);
    
    maxValEntry_ = gtk_entry_new ();
    gtk_widget_set_name (maxValEntry_, "maxValEntry_");
    gtk_widget_show (maxValEntry_);
    gtk_table_attach (GTK_TABLE (generalStatsTable_), maxValEntry_, 1, 2, 2, 3,
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_editable_set_editable (GTK_EDITABLE (maxValEntry_), FALSE);
    gtk_entry_set_text (GTK_ENTRY (maxValEntry_), "(255,255,255)");
    gtk_entry_set_has_frame (GTK_ENTRY (maxValEntry_), FALSE);
    gtk_entry_set_width_chars (GTK_ENTRY (maxValEntry_), 14);
    
    avrgValEntry_ = gtk_entry_new ();
    gtk_widget_set_name (avrgValEntry_, "avrgValEntry_");
    gtk_widget_show (avrgValEntry_);
    gtk_table_attach (GTK_TABLE (generalStatsTable_), avrgValEntry_,
                      1, 2, 3, 4,(GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_editable_set_editable (GTK_EDITABLE (avrgValEntry_), FALSE);
    gtk_entry_set_text (GTK_ENTRY (avrgValEntry_), "(255,255,255)");
    gtk_entry_set_has_frame (GTK_ENTRY (avrgValEntry_), FALSE);
    gtk_entry_set_width_chars (GTK_ENTRY (avrgValEntry_), 14);

    generalStatsRightTable_ = gtk_table_new (2, 2, FALSE);
    gtk_widget_set_name (generalStatsRightTable_, "generalStatsRightTable_");
    gtk_widget_show (generalStatsRightTable_);
    gtk_box_pack_start (GTK_BOX (infoHBox_), generalStatsRightTable_,
                        TRUE, TRUE, 1);

    stdDevLabel_ = gtk_label_new ("Std.Dev.");
    gtk_widget_set_name (stdDevLabel_, "stdDevLabel_");
    gtk_widget_show (stdDevLabel_);
    gtk_table_attach (GTK_TABLE (generalStatsRightTable_), stdDevLabel_,
                      0, 1, 0, 1,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
    gtk_label_set_justify (GTK_LABEL (stdDevLabel_), GTK_JUSTIFY_RIGHT);
    gtk_misc_set_alignment (GTK_MISC (stdDevLabel_), 0, 0.5);
    gtk_label_set_width_chars (GTK_LABEL (stdDevLabel_), 10);

    
    stdDevEntry_ = gtk_entry_new ();
    gtk_widget_set_name (stdDevEntry_, "stdDevEntry_");
    gtk_widget_show (stdDevEntry_);
    gtk_table_attach (GTK_TABLE (generalStatsRightTable_), stdDevEntry_,
                      1, 2, 0, 1, (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_editable_set_editable (GTK_EDITABLE (stdDevEntry_), FALSE);
    gtk_entry_set_text (GTK_ENTRY (stdDevEntry_), "(255.5,255.5,255.5)");
    gtk_entry_set_has_frame (GTK_ENTRY (stdDevEntry_), FALSE);
    gtk_entry_set_width_chars (GTK_ENTRY (stdDevEntry_), 20);

    covLabel_ = gtk_label_new ("Covariance");
    gtk_widget_set_name (covLabel_, "covLabel_");
    gtk_widget_show (covLabel_);
    gtk_table_attach (GTK_TABLE (generalStatsRightTable_), covLabel_,
                      0, 1, 1, 2, (GtkAttachOptions) (0),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_label_set_justify (GTK_LABEL (covLabel_), GTK_JUSTIFY_RIGHT);
    gtk_misc_set_padding (GTK_MISC (covLabel_), 4, 0);
    gtk_label_set_width_chars (GTK_LABEL (covLabel_), 10);
    
    covarTable_ = gtk_table_new (3, 3, TRUE);
    gtk_widget_set_name (covarTable_, "covarTable_");
    gtk_widget_show (covarTable_);
    gtk_table_attach (GTK_TABLE (generalStatsRightTable_), covarTable_,
                      1, 2, 1, 2,(GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (GTK_FILL), 0, 0);
    gtk_container_set_border_width (GTK_CONTAINER (covarTable_), 1);
    gtk_table_set_row_spacings (GTK_TABLE (covarTable_), 1);
    gtk_table_set_col_spacings (GTK_TABLE (covarTable_), 1);
    
    rgEntry_ = gtk_entry_new ();
    gtk_widget_set_name (rgEntry_, "rgEntry_");
    gtk_widget_show (rgEntry_);
    gtk_table_attach (GTK_TABLE (covarTable_), rgEntry_, 1, 2, 0, 1,
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_editable_set_editable (GTK_EDITABLE (rgEntry_), FALSE);
    gtk_entry_set_has_frame (GTK_ENTRY (rgEntry_), FALSE);
    gtk_entry_set_width_chars (GTK_ENTRY (rgEntry_), 10);
    
    rbEntry_ = gtk_entry_new ();
    gtk_widget_set_name (rbEntry_, "rbEntry_");
    gtk_widget_show (rbEntry_);
    gtk_table_attach (GTK_TABLE (covarTable_), rbEntry_, 2, 3, 0, 1,
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_editable_set_editable (GTK_EDITABLE (rbEntry_), FALSE);
    gtk_entry_set_has_frame (GTK_ENTRY (rbEntry_), FALSE);
    gtk_entry_set_width_chars (GTK_ENTRY (rbEntry_), 10);
    
    ggEntry_ = gtk_entry_new ();
    gtk_widget_set_name (ggEntry_, "ggEntry_");
    gtk_widget_show (ggEntry_);
    gtk_table_attach (GTK_TABLE (covarTable_), ggEntry_, 1, 2, 1, 2,
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_editable_set_editable (GTK_EDITABLE (ggEntry_), FALSE);
    gtk_entry_set_has_frame (GTK_ENTRY (ggEntry_), FALSE);
    gtk_entry_set_activates_default (GTK_ENTRY (ggEntry_), TRUE);
    gtk_entry_set_width_chars (GTK_ENTRY (ggEntry_), 10);
    
    bgEntry_ = gtk_entry_new ();
    gtk_widget_set_name (bgEntry_, "bgEntry_");
    gtk_widget_show (bgEntry_);
    gtk_table_attach (GTK_TABLE (covarTable_), bgEntry_, 1, 2, 2, 3,
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_editable_set_editable (GTK_EDITABLE (bgEntry_), FALSE);
    gtk_entry_set_has_frame (GTK_ENTRY (bgEntry_), FALSE);
    gtk_entry_set_width_chars (GTK_ENTRY (bgEntry_), 10);
    
    grEntry_ = gtk_entry_new ();
    gtk_widget_set_name (grEntry_, "grEntry_");
    gtk_widget_show (grEntry_);
    gtk_table_attach (GTK_TABLE (covarTable_), grEntry_, 0, 1, 1, 2,
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_editable_set_editable (GTK_EDITABLE (grEntry_), FALSE);
    gtk_entry_set_has_frame (GTK_ENTRY (grEntry_), FALSE);
    gtk_entry_set_width_chars (GTK_ENTRY (grEntry_), 10);
    
    brEntry_ = gtk_entry_new ();
    gtk_widget_set_name (brEntry_, "brEntry_");
    gtk_widget_show (brEntry_);
    gtk_table_attach (GTK_TABLE (covarTable_), brEntry_, 0, 1, 2, 3,
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_editable_set_editable (GTK_EDITABLE (brEntry_), FALSE);
    gtk_entry_set_has_frame (GTK_ENTRY (brEntry_), FALSE);
    gtk_entry_set_width_chars (GTK_ENTRY (brEntry_), 10);
    
    gbEntry_ = gtk_entry_new ();
    gtk_widget_set_name (gbEntry_, "gbEntry_");
    gtk_widget_show (gbEntry_);
    gtk_table_attach (GTK_TABLE (covarTable_), gbEntry_, 2, 3, 1, 2,
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_editable_set_editable (GTK_EDITABLE (gbEntry_), FALSE);
    gtk_entry_set_has_frame (GTK_ENTRY (gbEntry_), FALSE);
    gtk_entry_set_width_chars (GTK_ENTRY (gbEntry_), 10);
    
    bbEntry_ = gtk_entry_new ();
    gtk_widget_set_name (bbEntry_, "bbEntry_");
    gtk_widget_show (bbEntry_);
    gtk_table_attach (GTK_TABLE (covarTable_), bbEntry_, 2, 3, 2, 3,
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_editable_set_editable (GTK_EDITABLE (bbEntry_), FALSE);
    gtk_entry_set_has_frame (GTK_ENTRY (bbEntry_), FALSE);
    gtk_entry_set_width_chars (GTK_ENTRY (bbEntry_), 10);
    
    rrEntry_ = gtk_entry_new ();
    gtk_widget_set_name (rrEntry_, "rrEntry_");
    gtk_widget_show (rrEntry_);
    gtk_table_attach (GTK_TABLE (covarTable_), rrEntry_, 0, 1, 0, 1,
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (GTK_EXPAND), 0, 0);
    gtk_entry_set_max_length (GTK_ENTRY (rrEntry_), 10);
    gtk_editable_set_editable (GTK_EDITABLE (rrEntry_), FALSE);
    gtk_entry_set_has_frame (GTK_ENTRY (rrEntry_), FALSE);
    gtk_entry_set_width_chars (GTK_ENTRY (rrEntry_), 10);
    
    infoLabel_ = gtk_label_new ("Information");
    gtk_widget_set_name (infoLabel_, "infoLabel_");
    gtk_widget_show (infoLabel_);
    gtk_frame_set_label_widget (GTK_FRAME (informationFrame_), infoLabel_);
    gtk_label_set_use_markup (GTK_LABEL (infoLabel_), TRUE);
    
    ioFrame_ = gtk_frame_new (NULL);
    gtk_widget_set_name (ioFrame_, "ioFrame_");
    gtk_widget_show (ioFrame_);
    gtk_box_pack_start (GTK_BOX (vboxTop_), ioFrame_, FALSE, TRUE, 0);
    
    saveImgAlignment_ = gtk_alignment_new (0.5, 0, 1, 1);
    gtk_widget_set_name (saveImgAlignment_, "saveImgAlignment_");
    gtk_widget_show (saveImgAlignment_);
    gtk_container_add (GTK_CONTAINER (ioFrame_), saveImgAlignment_);
    gtk_alignment_set_padding (GTK_ALIGNMENT (saveImgAlignment_), 0, 0, 4, 0);
    
    saveHBox_ = gtk_hbox_new (FALSE, 0);
    gtk_widget_set_name (saveHBox_, "saveHBox_");
    gtk_widget_show (saveHBox_);
    gtk_container_add (GTK_CONTAINER (saveImgAlignment_), saveHBox_);
    
    filenameEntry_ = gtk_entry_new ();
    gtk_widget_set_name (filenameEntry_, "filenameEntry_");
    gtk_widget_show (filenameEntry_);
    gtk_box_pack_start (GTK_BOX (saveHBox_), filenameEntry_, TRUE, TRUE, 0);
    gtk_tooltips_set_tip (tooltips_, filenameEntry_, 
                          "Name for the image file", NULL);
    gtk_entry_set_text (GTK_ENTRY (filenameEntry_), "tux.png");
    
    browseButton_ = gtk_button_new_from_stock ("gtk-open");
    gtk_widget_set_name (browseButton_, "browseButton_");
    gtk_widget_show (browseButton_);
    gtk_box_pack_start (GTK_BOX (saveHBox_), browseButton_, FALSE, FALSE, 0);
    GTK_WIDGET_SET_FLAGS (browseButton_, GTK_CAN_DEFAULT);
    gtk_tooltips_set_tip (tooltips_, browseButton_,
                          "Browse your filesystem for a destination", NULL);
    gtk_button_set_focus_on_click (GTK_BUTTON (browseButton_), FALSE);
    
    saveButton_ = gtk_button_new_from_stock ("gtk-save");
    gtk_widget_set_name (saveButton_, "saveButton_");
    gtk_widget_show (saveButton_);
    gtk_box_pack_start (GTK_BOX (saveHBox_), saveButton_, FALSE, FALSE, 0);
    gtk_tooltips_set_tip (tooltips_, saveButton_,
                          "Save the currently displayed image with the " \
                          "filename on the left", NULL);
    
    saveImageLabel_ = gtk_label_new ("Save image");
    gtk_widget_set_name (saveImageLabel_, "saveImageLabel_");
    gtk_widget_show (saveImageLabel_);
    gtk_frame_set_label_widget (GTK_FRAME (ioFrame_), saveImageLabel_);
    gtk_label_set_use_markup (GTK_LABEL (saveImageLabel_), TRUE);
    
    if (notNull(curveSelector_)) {
      delete curveSelector_;
    }

    curveSelector_ = new gtkCurveSelection;

    //
    // The signals and their call-backs
    //
    
    // Zoom
    g_signal_connect ((gpointer) zoomEntry_, "activate",
                      G_CALLBACK (cbOnZoomEntryActivate),
                      this);
    g_signal_connect ((gpointer) zoomHAdjustment_, "value_changed",
                      G_CALLBACK (cbOnZoomHAdjustmentValueChanged),
                      this);

    // Offset
    g_signal_connect ((gpointer) offsetEntry_, "activate",
                      G_CALLBACK (cbOnOffsetEntryActivate),
                      this);
    g_signal_connect ((gpointer) offsetHAdjustment_, "value_changed",
                      G_CALLBACK (cbOnOffsetHAdjustmentValueChanged),
                      this);

    // Slope
    g_signal_connect ((gpointer) slopeEntry_, "activate",
                      G_CALLBACK (cbOnSlopeEntryActivate),
                      this);
    g_signal_connect ((gpointer) slopeHAdjustment_, "value_changed",
                      G_CALLBACK (cbOnSlopeHAdjustmentValueChanged),
                      this);

    // Radio buttons for mapping
    g_signal_connect ((gpointer) linearButton_,"toggled",
                      G_CALLBACK (cbOnMappingRadioToggled),this);
    g_signal_connect ((gpointer) offsetButton_,"toggled",
                      G_CALLBACK (cbOnMappingRadioToggled),this);
    g_signal_connect ((gpointer) minButton_,"toggled",
                      G_CALLBACK (cbOnMappingRadioToggled),this);
    g_signal_connect ((gpointer) maxButton_,"toggled",
                      G_CALLBACK (cbOnMappingRadioToggled),this);
    g_signal_connect ((gpointer) optLinButton_,"toggled",
                      G_CALLBACK (cbOnMappingRadioToggled),this);
    g_signal_connect ((gpointer) histEqButton_,"toggled",
                      G_CALLBACK (cbOnMappingRadioToggled),this);
    g_signal_connect ((gpointer) logButton_,"toggled",
                      G_CALLBACK (cbOnMappingRadioToggled),this);
    g_signal_connect ((gpointer) optLogButton_,"toggled",
                      G_CALLBACK (cbOnMappingRadioToggled),this);
    g_signal_connect ((gpointer) userDefButton_,"toggled",
                      G_CALLBACK (cbOnMappingRadioToggled),this);


    // Curve button
    g_signal_connect ((gpointer) editCurveButton_, "clicked",
                      G_CALLBACK (cbOnCurveButtonClicked),
                      this);

    g_signal_connect ((gpointer) hexButton_, "toggled",
                      G_CALLBACK (cbOnHexButtonToggled),this);

    // Color selectors
    g_signal_connect ((gpointer) underflowColorButton_, "color_set",
                      G_CALLBACK (cbOnUnderflowColorButtonSet),
                      this);

    g_signal_connect ((gpointer) overflowColorButton_, "color_set",
                      G_CALLBACK (cbOnOverflowColorButtonSet),
                      this);

    // File browser
    g_signal_connect ((gpointer) browseButton_, "clicked",
                      G_CALLBACK (cbOnBrowseButtonClicked),
                      this);

    // Save button
    g_signal_connect ((gpointer) saveButton_, "clicked",
                      G_CALLBACK (cbOnSaveButtonClicked),
                      this);
    
    // Radio buttons for palette
    g_signal_connect ((gpointer) grayPaletteButton_,"toggled",
                      G_CALLBACK (cbOnPaletteRadioToggled),this);
    g_signal_connect ((gpointer) redPaletteButton_,"toggled",
                      G_CALLBACK (cbOnPaletteRadioToggled),this);
    g_signal_connect ((gpointer) greenPaletteButton_,"toggled",
                      G_CALLBACK (cbOnPaletteRadioToggled),this);
    g_signal_connect ((gpointer) bluePaletteButton_,"toggled",
                      G_CALLBACK (cbOnPaletteRadioToggled),this);
    g_signal_connect ((gpointer) huePaletteButton_,"toggled",
                      G_CALLBACK (cbOnPaletteRadioToggled),this);
    g_signal_connect ((gpointer) userPaletteButton_,"toggled",
                      G_CALLBACK (cbOnPaletteRadioToggled),this);
    
    // Label adjacency analysis
    g_signal_connect ((gpointer) labelAdjacencyAnalysisButton_, "toggled",
                      G_CALLBACK (cbOnLabelAdjacencyButtonToggled),this);

    // Neighborhood
    g_signal_connect ((gpointer) fourNeighborhoodButton_, "toggled",
                      G_CALLBACK(cbOnNeighborhoodRadioToggled),this);
    g_signal_connect ((gpointer) eightNeighborhoodButton_, "toggled",
                      G_CALLBACK(cbOnNeighborhoodRadioToggled),this);


    // Label adjacency analysis
    g_signal_connect ((gpointer) minColorsButton_, "toggled",
                      G_CALLBACK (cbOnMinColorsButtonToggled),this);

    // 
    g_signal_connect ((gpointer) filenameEntry_, "activate",
                      G_CALLBACK (cbOnFilenameEntryActivate),
                      this);

    //gdk_flush();
    //gdk_threads_leave();
    lock_.unlock();
    _lti_debug2("leaving viewer2D::configDialog::tk::build()\n");

    return true;
  }

  bool viewer2D::configDialog::tk::show() {
    return viewerBase::configDialog::tk::show();
  }

  bool viewer2D::configDialog::tk::hide() {
    return viewerBase::configDialog::tk::hide();
  }

  bool viewer2D::configDialog::tk::destroy() {
    if (viewerBase::configDialog::tk::destroy()) {
      // indicate all destroyed widgets
      lock_.lock();

      delete curveSelector_;
      curveSelector_=0;
      
      if (guiServer::representsGuiThread()) {
        gtk_widget_destroy(GTK_WIDGET(tooltips_));
      } else {
        gdk_threads_enter();
        gtk_widget_destroy(GTK_WIDGET(tooltips_));
        gdk_flush();
        gdk_threads_leave();
      }

      tooltips_=0;

      lock_.unlock();

      return true;
    }

    return false;
  }

  bool 
  viewer2D::configDialog::tk::updateParameters() {
    if (viewerBase::configDialog::tk::updateParameters() &&
        dlgBuild_) {
      // since in the if-condition an update was done, we can get the
      // parameters in the arguments of the method safetly casted
      parameters& par = getParameters();

      // zoom
      setZoom(par.zoom);

      // radio buttons for mapping
      setMapping(par.mappingType);
      
      // slope and offset
      gtk_adjustment_set_value(GTK_ADJUSTMENT(offsetHAdjustment_),
                               par.offset);
      gtk_entry_set_text(GTK_ENTRY(offsetEntry_),
                         toString(par.offset).c_str());
        
      gtk_adjustment_set_value(GTK_ADJUSTMENT(slopeHAdjustment_),
                               par.slope);
      gtk_entry_set_text(GTK_ENTRY(slopeEntry_),
                         toString(par.slope).c_str());

      // user defined curve
      curveSelector_->use(par.userMapping);
      
      // hexadecimal
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(hexButton_),
                                   par.hexadecimal ? TRUE : FALSE);
      
      // set the under/overflow button colors
      GdkColor color;

      color.red   = par.underflowColor.red*257;
      color.green = par.underflowColor.green*257;
      color.blue  = par.underflowColor.blue*257;

      gtk_color_button_set_color(GTK_COLOR_BUTTON(underflowColorButton_),
                                 &color);
      
      color.red   = par.overflowColor.red*257;
      color.green = par.overflowColor.green*257;
      color.blue  = par.overflowColor.blue*257;

      gtk_color_button_set_color(GTK_COLOR_BUTTON(overflowColorButton_),
                                 &color);

      // set the palette type
      setPaletteType(par.paletteType);

      // Label adjacency analysis
      gtk_toggle_button_set_active(
                             GTK_TOGGLE_BUTTON(labelAdjacencyAnalysisButton_),
                             par.labelAdjacency ? TRUE : FALSE);

      // enable/disable parts of the dialog
      setAdjacency(par.labelAdjacency,dataType_);

      // neighborhood for adjacency
      setNeighborhood(par.adjacencyNeighborhood);

      // min adjacent colors
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(minColorsButton_),
                                   par.minAdjacencyColors ? TRUE : FALSE);

      // filename
      gtk_entry_set_text(GTK_ENTRY(filenameEntry_),par.filename.c_str());


      return true;
    }
    return false;
  }

  bool viewer2D::configDialog::tk::
  updateInformation(const viewer2D::information& info) {
    _lti_debug4("viewer2D::configDialog::tk::updateInformation()\n");

    if (dlgBuild_) {
      dataType_=info.type;

      gtk_entry_set_text(GTK_ENTRY(imgSizeEntry_),
                         toString(info.size).c_str());

      if (info.type == Image) {
        gtk_entry_set_text(GTK_ENTRY(minValEntry_),
                           toString(info.minRGB).c_str());
        gtk_entry_set_text(GTK_ENTRY(maxValEntry_),
                           toString(info.maxRGB).c_str());
        gtk_entry_set_text(GTK_ENTRY(avrgValEntry_),
                           toString(info.averageRGB).c_str());

        frgbPixel stdDevRGB(sqrt(info.covarianceRGB.at(0,0)),
                            sqrt(info.covarianceRGB.at(1,1)),
                            sqrt(info.covarianceRGB.at(2,2)));

        gtk_entry_set_text(GTK_ENTRY(stdDevEntry_),
                           toString(stdDevRGB).c_str());

      } else {
        gtk_entry_set_text(GTK_ENTRY(minValEntry_),
                           toString(info.minI).c_str());
        gtk_entry_set_text(GTK_ENTRY(maxValEntry_),
                           toString(info.maxI).c_str());
        gtk_entry_set_text(GTK_ENTRY(avrgValEntry_),
                           toString(info.averageI).c_str());
        gtk_entry_set_text(GTK_ENTRY(stdDevEntry_),
                           toString(info.stdDeviation).c_str());
      }

      static const int nd = 4;
      
      gtk_entry_set_text(GTK_ENTRY(rrEntry_),
                         toString(info.covarianceRGB.at(0,0),nd).c_str());
      gtk_entry_set_text(GTK_ENTRY(rgEntry_),
                         toString(info.covarianceRGB.at(0,1),nd).c_str());
      gtk_entry_set_text(GTK_ENTRY(rbEntry_),
                         toString(info.covarianceRGB.at(0,2),nd).c_str());
      
      gtk_entry_set_text(GTK_ENTRY(grEntry_),
                         toString(info.covarianceRGB.at(1,0),nd).c_str());
      gtk_entry_set_text(GTK_ENTRY(ggEntry_),
                         toString(info.covarianceRGB.at(1,1),nd).c_str());
      gtk_entry_set_text(GTK_ENTRY(gbEntry_),
                         toString(info.covarianceRGB.at(1,2),nd).c_str());
      
      gtk_entry_set_text(GTK_ENTRY(brEntry_),
                         toString(info.covarianceRGB.at(2,0),nd).c_str());
      gtk_entry_set_text(GTK_ENTRY(bgEntry_),
                         toString(info.covarianceRGB.at(2,1),nd).c_str());
      gtk_entry_set_text(GTK_ENTRY(bbEntry_),
                         toString(info.covarianceRGB.at(2,2),nd).c_str());
      
      return true;
    }
    return false;
  }


  viewer2D::parameters& viewer2D::configDialog::tk::getParameters() {
    viewer2D::parameters* ptr =
      dynamic_cast<viewer2D::parameters*>(params_);
    assert(notNull(ptr));
    return *ptr;
  }

  viewer2D::configDialog::tk& 
  viewer2D::configDialog::tk::copy(const tk& other) {

    viewerBase::configDialog::tk::copy(other);
    // TODO: what to do?  deep copy of parameters?  maybe the tk should
    // never be copied... still to be decided...
    
    return *this;
  }

  
  std::string viewer2D::configDialog::tk::toString(const double val,
                                                   const int decimals) {
    char phrase[8];
    sprintf(phrase,"%s%i%s","%.",decimals,"f");
    char tmpBuf[64];
    sprintf(tmpBuf,phrase,val); //"%.4f"
    return std::string(tmpBuf);  
  }

  std::string viewer2D::configDialog::tk::toString(const int val) {
    char tmpBuf[64];
    sprintf(tmpBuf,"%i",val); //"%.4f"
    return std::string(tmpBuf);  
  }

  std::string viewer2D::configDialog::tk::toString(const ipoint& val) {
    return toString(val.x) + " x " + toString(val.y);
  }
  
  std::string viewer2D::configDialog::tk::toString(const rgbaPixel& val) {
    return
      "(" +
      toString(val.red) + ", " + 
      toString(val.green) + ", " +
      toString(val.blue) + ")";
  }

  std::string viewer2D::configDialog::tk::toString(const frgbPixel& val,
                                                   const int decimals) {
    return
      "(" +
      toString(val.red,  decimals) + ", " + 
      toString(val.green,decimals) + ", " +
      toString(val.blue, decimals) +
      ")";
  }

  double viewer2D::configDialog::tk::fromString(const char* val) {
    return atof(val);
  }

  /*
   * Set the GUI elements to the proper zoom value
   */
  void viewer2D::configDialog::tk::setZoom(const float zoom) {
    int izoom;
    std::string str;
    // ensure apropriate zoom values, no less than 1/32th and only n or 1/n
    // with n integer.

    if (zoom < 1.0f/32.0f) {
      izoom = 1;
    } else if (zoom<1.0f) {
      izoom = iround(1.0f/zoom);
      str = "1/" + toString(izoom);
      izoom=2-izoom;
    } else {
      izoom = iround(zoom);
      str = toString(izoom);
    }

    gtk_adjustment_set_value(GTK_ADJUSTMENT(zoomHAdjustment_),izoom);    
    gtk_entry_set_text(GTK_ENTRY(zoomEntry_),str.c_str());    
  }
  
  /*
   * Get the proper zoom value from the GUI elements
   */
  float viewer2D::configDialog::tk::getZoom() const {
    std::string str(gtk_entry_get_text(GTK_ENTRY(zoomEntry_)));
    float fzoom;

    // try first if the user entered in the format "1/xx"
    if (str.find("1/") == 0) {
      str.erase(0,2); // erase the prefix
      fzoom = 1.0f/static_cast<float>(fromString(str.c_str()));
    } else {
      fzoom = static_cast<float>(fromString(str.c_str()));
    }
    
    if (fzoom < 1.0f/32.0f) {
      fzoom = 1.0f;
    } else if (fzoom<1.0f) {
      fzoom = 1.0f/(iround(1.0f/fzoom));  
    } else {
      fzoom = iround(fzoom);
    }

    return fzoom;
  }

  void viewer2D::configDialog::tk::
  cbOnZoomEntryActivate(GtkCellEditable *,
                        gpointer userData) {
    viewer2D::configDialog::tk* me = (viewer2D::configDialog::tk*)userData;
    float zoom = me->getZoom();
    me->setZoom(zoom); // only GUI
    me->getParameters().zoom = zoom;
  }
  
  void viewer2D::configDialog::tk::
  cbOnZoomHAdjustmentValueChanged(GtkAdjustment *scale,
                                  gpointer  userData) {
    viewer2D::configDialog::tk* me = (viewer2D::configDialog::tk*)userData;
    float zoom = static_cast<float>(scale->value);
    if (zoom <= 0.5f) {
      zoom = 1.0f/round(2.0f-zoom);
    } else {
      zoom = round(zoom);
    }
    me->setZoom(zoom); // only GUI
    me->getParameters().zoom = zoom;
  }
  

  void viewer2D::configDialog::tk::
  cbOnOffsetEntryActivate(GtkCellEditable *,
                          gpointer         userData) {
    viewer2D::configDialog::tk* me = (viewer2D::configDialog::tk*)userData;

    std::string str(gtk_entry_get_text(GTK_ENTRY(me->offsetEntry_)));
    float offset = static_cast<float>(fromString(str.c_str()));

    gtk_adjustment_set_value(GTK_ADJUSTMENT(me->offsetHAdjustment_),offset);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(me->offsetButton_),TRUE);

    me->getParameters().offset = offset;
    me->getParameters().mappingType = viewer2DPainter::OffsetSlope;
  }
  
  void viewer2D::configDialog::tk::
  cbOnOffsetHAdjustmentValueChanged(GtkAdjustment *scale,
                                    gpointer  userData) {
    viewer2D::configDialog::tk* me = (viewer2D::configDialog::tk*)userData;

    float offset = static_cast<float>(scale->value);

    gtk_entry_set_text(GTK_ENTRY(me->offsetEntry_),toString(offset).c_str());
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(me->offsetButton_),TRUE);

    me->getParameters().offset = offset;
    me->getParameters().mappingType = viewer2DPainter::OffsetSlope;

  }

  void viewer2D::configDialog::tk::
  cbOnSlopeEntryActivate(GtkCellEditable *,
                          gpointer         userData) {
    viewer2D::configDialog::tk* me = (viewer2D::configDialog::tk*)userData;

    std::string str(gtk_entry_get_text(GTK_ENTRY(me->slopeEntry_)));
    float slope = static_cast<float>(fromString(str.c_str()));

    gtk_adjustment_set_value(GTK_ADJUSTMENT(me->slopeHAdjustment_),slope);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(me->offsetButton_),TRUE);

    me->getParameters().slope = slope;
    me->getParameters().mappingType = viewer2DPainter::OffsetSlope;
  }
  
  void viewer2D::configDialog::tk::
  cbOnSlopeHAdjustmentValueChanged(GtkAdjustment *scale,
                                    gpointer  userData) {
    viewer2D::configDialog::tk* me = (viewer2D::configDialog::tk*)userData;

    float slope = static_cast<float>(scale->value);

    gtk_entry_set_text(GTK_ENTRY(me->slopeEntry_),
                       toString(slope).c_str());
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(me->offsetButton_),TRUE);

    me->getParameters().slope = slope;
    me->getParameters().mappingType = viewer2DPainter::OffsetSlope;
  }

  void viewer2D::configDialog::tk::
  setMapping(const viewer2DPainter::eMappingType mapping) {
    switch(mapping) {
      case viewer2DPainter::Original:
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(linearButton_),TRUE);
        break;    
      case viewer2DPainter::OffsetSlope:
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(offsetButton_),TRUE);
        break;
      case viewer2DPainter::MinToBlack:
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(minButton_),TRUE);
        break;
      case viewer2DPainter::MaxToWhite:
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(maxButton_),TRUE);
        break;
      case viewer2DPainter::Optimal:
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(optLinButton_),TRUE);
        break;
      case viewer2DPainter::HistogramEq:
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(histEqButton_),TRUE);
        break;
      case viewer2DPainter::Log:
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(logButton_),TRUE);
        break;
      case viewer2DPainter::OptimalLog:
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(optLogButton_),TRUE);
        break;
      case viewer2DPainter::UserDefMap:
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(userDefButton_),TRUE);
        break;
      default:
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(linearButton_),TRUE);
        break;    
    }
  }

  viewer2DPainter::eMappingType viewer2D::configDialog::tk::getMapping() {

    if (GTK_TOGGLE_BUTTON(linearButton_)->active) {
      return viewer2DPainter::Original;
    } else if (GTK_TOGGLE_BUTTON(offsetButton_)->active) {
      return viewer2DPainter::OffsetSlope; 
    } else if (GTK_TOGGLE_BUTTON(minButton_)->active) {
      return viewer2DPainter::MinToBlack;
    } else if (GTK_TOGGLE_BUTTON(maxButton_)->active) {
      return viewer2DPainter::MaxToWhite;
    } else if (GTK_TOGGLE_BUTTON(optLinButton_)->active) {
      return viewer2DPainter::Optimal;
    } else if (GTK_TOGGLE_BUTTON(histEqButton_)->active) {
      return viewer2DPainter::HistogramEq;
    } else if (GTK_TOGGLE_BUTTON(logButton_)->active) {
      return viewer2DPainter::Log; 
    } else if (GTK_TOGGLE_BUTTON(optLogButton_)->active) {
      return viewer2DPainter::OptimalLog;
    } else if(GTK_TOGGLE_BUTTON(userDefButton_)->active) {
      return viewer2DPainter::UserDefMap;
    } 

    return viewer2DPainter::UserDefMap;
  }

  void viewer2D::configDialog::tk::
  cbOnMappingRadioToggled(GtkToggleButton*,
                          gpointer userData) {
    viewer2D::configDialog::tk* me = (viewer2D::configDialog::tk*) userData;

    viewer2DPainter::eMappingType m = me->getMapping();
    me->getParameters().mappingType = m;
  }


  void viewer2D::configDialog::tk::cbOnCurveButtonClicked(GtkButton *,
                                                          gpointer   userData){
    viewer2D::configDialog::tk* me = (viewer2D::configDialog::tk*) userData;

    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(me->userDefButton_),TRUE);

    me->getParameters().mappingType = viewer2DPainter::UserDefMap;
    me->curveSelector_->use(me->getParameters().userMapping);
    me->curveSelector_->show();
  }


  void viewer2D::configDialog::tk::
  cbOnHexButtonToggled(GtkToggleButton* toggleButton,
                       gpointer userData) {
    
    viewer2D::configDialog::tk* me = (viewer2D::configDialog::tk*) userData;

    me->getParameters().hexadecimal = toggleButton->active;
  }
  
  void viewer2D::configDialog::tk::
  cbOnUnderflowColorButtonSet(GtkColorButton  *colorbutton,
                              gpointer         userData) {
    viewer2D::configDialog::tk* me = (viewer2D::configDialog::tk*) userData;
    GdkColor color;
    gtk_color_button_get_color(colorbutton,&color);
    me->getParameters().underflowColor.set(static_cast<ubyte>(color.red/256),
                                           static_cast<ubyte>(color.green/256),
                                           static_cast<ubyte>(color.blue/256),
                                           0);
  }
  
  void viewer2D::configDialog::tk::
  cbOnOverflowColorButtonSet(GtkColorButton  *colorbutton,
                             gpointer         userData) {
    viewer2D::configDialog::tk* me = (viewer2D::configDialog::tk*) userData;
    
    GdkColor color;
    gtk_color_button_get_color(colorbutton,&color);
    me->getParameters().overflowColor.set(static_cast<ubyte>(color.red/256),
                                          static_cast<ubyte>(color.green/256),
                                          static_cast<ubyte>(color.blue/256),
                                           0);
  }
  
  void viewer2D::configDialog::tk::
  cbOnPaletteRadioToggled(GtkToggleButton*,
                          gpointer userData) {
    viewer2D::configDialog::tk* me = (viewer2D::configDialog::tk*) userData;

    viewer2DPainter::ePaletteType m = me->getPaletteType();
    me->getParameters().paletteType = m;
  }

  void viewer2D::configDialog::tk::
  setPaletteType(const viewer2DPainter::ePaletteType pal) {
    switch(pal){
      case viewer2DPainter::GrayPalette:
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(grayPaletteButton_),
                                     TRUE);
        break;
      case viewer2DPainter::RedPalette:
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(redPaletteButton_),
                                     TRUE);
        break;        
      case viewer2DPainter::GreenPalette:
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(greenPaletteButton_),
                                     TRUE);
        break;      
      case viewer2DPainter::BluePalette:
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(bluePaletteButton_),
                                     TRUE);
        break;
      case viewer2DPainter::HuePalette:
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(huePaletteButton_),
                                     TRUE);
        break;
      case viewer2DPainter::UserDefPalette:
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(userPaletteButton_),
                                     TRUE);
        break; 
      default:
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(grayPaletteButton_),
                                     TRUE);
    }
  }

  viewer2DPainter::ePaletteType viewer2D::configDialog::tk::getPaletteType() {
    if (GTK_TOGGLE_BUTTON(grayPaletteButton_)->active) {
      return viewer2DPainter::GrayPalette;
    } else if (GTK_TOGGLE_BUTTON(redPaletteButton_)->active) {
      return viewer2DPainter::RedPalette;
    } else if (GTK_TOGGLE_BUTTON(greenPaletteButton_)->active) {
      return viewer2DPainter::GreenPalette;
    } else if (GTK_TOGGLE_BUTTON(bluePaletteButton_)->active) {
      return viewer2DPainter::BluePalette;
    } else if (GTK_TOGGLE_BUTTON(huePaletteButton_)->active) {
      return viewer2DPainter::HuePalette;
    } else if (GTK_TOGGLE_BUTTON(userPaletteButton_)->active) {
      return viewer2DPainter::UserDefPalette;
    } 
    return viewer2DPainter::GrayPalette;
  }

  void viewer2D::configDialog::tk::
  cbOnLabelAdjacencyButtonToggled(GtkToggleButton* toggleButton,
                                  gpointer userData) {
    
    viewer2D::configDialog::tk* me = (viewer2D::configDialog::tk*) userData;

    bool flag = toggleButton->active;
    me->getParameters().labelAdjacency = flag;
    me->setAdjacency(flag,me->dataType_);
  }

  void viewer2D::configDialog::tk::
  cbOnNeighborhoodRadioToggled(GtkToggleButton*,
                               gpointer userData) {
    viewer2D::configDialog::tk* me = (viewer2D::configDialog::tk*) userData;

    viewer2DPainter::eNeighborhoodType m = me->getNeighborhood();
    me->getParameters().adjacencyNeighborhood = m;
  }

  void viewer2D::configDialog::tk::
  setNeighborhood(const viewer2DPainter::eNeighborhoodType n) {
    switch(n) {
      case viewer2DPainter::FourNeighborhood:
        gtk_toggle_button_set_active(
                                  GTK_TOGGLE_BUTTON(fourNeighborhoodButton_),
                                  TRUE);
        break;
      case viewer2DPainter::EightNeighborhood:
        gtk_toggle_button_set_active(
                                  GTK_TOGGLE_BUTTON(eightNeighborhoodButton_),
                                  TRUE);
        break;
      default:
        gtk_toggle_button_set_active(
                                  GTK_TOGGLE_BUTTON(fourNeighborhoodButton_),
                                  TRUE);        
    }    
  }

  viewer2DPainter::eNeighborhoodType 
  viewer2D::configDialog::tk::getNeighborhood() {
    if (GTK_TOGGLE_BUTTON(fourNeighborhoodButton_)->active) {
      return viewer2DPainter::FourNeighborhood;
    } else if (GTK_TOGGLE_BUTTON(eightNeighborhoodButton_)->active) {
      return viewer2DPainter::EightNeighborhood;
    } 
    return viewer2DPainter::FourNeighborhood;
  }

  void viewer2D::configDialog::tk::
  cbOnMinColorsButtonToggled(GtkToggleButton* toggleButton,
                             gpointer userData) {
    
    viewer2D::configDialog::tk* me = (viewer2D::configDialog::tk*) userData;
    me->getParameters().minAdjacencyColors = toggleButton->active;
  }

  void viewer2D::configDialog::tk::
  cbOnFilenameEntryActivate(GtkCellEditable *,
                            gpointer         userData) {
    viewer2D::configDialog::tk* me = (viewer2D::configDialog::tk*)userData;

    std::string str;
    str = gtk_entry_get_text(GTK_ENTRY(me->filenameEntry_));
    if (!str.empty()) {
      me->getParameters().filename = str;
    }
  }



  void viewer2D::configDialog::tk::cbOnBrowseButtonClicked(GtkButton *,
                                                           gpointer userData) {
    viewer2D::configDialog::tk* me = (viewer2D::configDialog::tk*) userData;

    GtkWidget* fileChooser =
      gtk_file_chooser_dialog_new ("Save displayed image",
                                   NULL,
                                   GTK_FILE_CHOOSER_ACTION_SAVE,
                                   GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                   GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT,
                                   NULL);
    gtk_file_chooser_set_do_overwrite_confirmation(
                                                GTK_FILE_CHOOSER(fileChooser),
                                                TRUE);

    std::string filename = gtk_entry_get_text(GTK_ENTRY(me->filenameEntry_));
    me->getParameters().filename = filename;

    std::string folder,name;
    std::string::size_type pos; 
    if ((pos = filename.rfind("/")) != std::string::npos) {
      name = filename.substr(pos+1);
      folder = filename.substr(0,pos+1);
      gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER (fileChooser),
                                          folder.c_str());
      gtk_file_chooser_set_current_name (GTK_FILE_CHOOSER (fileChooser),
                                         name.c_str());
    } else {
      name = filename;
      folder = "";
      gtk_file_chooser_set_current_name (GTK_FILE_CHOOSER (fileChooser),
                                         name.c_str());
    }
    
    if (gtk_dialog_run(GTK_DIALOG(fileChooser)) == GTK_RESPONSE_ACCEPT) {
      // ok, the user said...
      char *f;
      // get the filename (gtk reserves some memory
      f = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(fileChooser));
      me->getParameters().filename = f;

      // set the name in the entry
      gtk_entry_set_text(GTK_ENTRY(me->filenameEntry_),f);

      if (!(me->getParameters().filename.empty())) {
        // and call the corresponding save method
        cbOnSaveButtonClicked(GTK_BUTTON(me->saveButton_),userData);
      }

      g_free (f);
    }

    gtk_widget_destroy(fileChooser);
  }
  
  void viewer2D::configDialog::tk::cbOnSaveButtonClicked(GtkButton *,
                                                         gpointer   userData){

    viewer2D::configDialog::tk* me = (viewer2D::configDialog::tk*) userData;
    
    viewer2D::configDialog* ptr =
      dynamic_cast<viewer2D::configDialog*>(&me->parent_);

    if (notNull(ptr)) {
      // get the filename in the entry
      std::string str;
      str = gtk_entry_get_text(GTK_ENTRY(me->filenameEntry_));
      me->getParameters().filename = str;

      if (str.empty()) {
        cbOnBrowseButtonClicked(GTK_BUTTON(me->browseButton_),userData);
      } else {
        ptr->saveButtonPressed(str);
      }
    }
  }


  void viewer2D::configDialog::tk::setAdjacency(const bool adj,
                                                const eDataType type) {

    bool nadj = not adj;
    bool ad = adj;
    gtk_widget_set_sensitive(linearButton_,nadj);
    gtk_widget_set_sensitive(offsetButton_,nadj);
    gtk_widget_set_sensitive(offsetEntry_,nadj);
    gtk_widget_set_sensitive(offsetHScale_,nadj);
    gtk_widget_set_sensitive(slopeLabel_,nadj);
    gtk_widget_set_sensitive(slopeEntry_,nadj);
    gtk_widget_set_sensitive(slopeHScale_,nadj);
    gtk_widget_set_sensitive(minButton_,nadj);
    gtk_widget_set_sensitive(maxButton_,nadj);
    gtk_widget_set_sensitive(optLinButton_,nadj);
    gtk_widget_set_sensitive(histEqButton_,nadj);
    gtk_widget_set_sensitive(logButton_,nadj);
    gtk_widget_set_sensitive(optLogButton_,nadj);
    gtk_widget_set_sensitive(userDefButton_,nadj);
    gtk_widget_set_sensitive(editCurveButton_,nadj);
    gtk_widget_set_sensitive(underflowColorButton_,nadj);
    gtk_widget_set_sensitive(overflowColorButton_,nadj);

    if (type == Image) {
      gtk_label_set_text(GTK_LABEL(paletteCtrlLabel_),
                         "Mapping applied to");
      gtk_button_set_label(GTK_BUTTON(grayPaletteButton_),
                           "Intensity");
      gtk_button_set_label(GTK_BUTTON(userPaletteButton_),
                           "RGB");
    } else {
      gtk_label_set_text(GTK_LABEL(paletteCtrlLabel_),
                         "Palette Control");
      gtk_button_set_label(GTK_BUTTON(grayPaletteButton_),
                           "Gra_y");
      gtk_button_set_label(GTK_BUTTON(userPaletteButton_),
                           "User _defined");
    }

    if (type == Channel) {
      gtk_widget_set_sensitive(hexButton_,false);
    } else {
      gtk_widget_set_sensitive(hexButton_,true);
    }

    if ((type == Image) || (type == Channel)) {
      // nadj = (type != Image);
      ad   = false;
      gtk_widget_set_sensitive(labelAdjacencyAnalysisButton_,false);
    } else {
      gtk_widget_set_sensitive(labelAdjacencyAnalysisButton_,true);
    }

    gtk_widget_set_sensitive(grayPaletteButton_,nadj);
    gtk_widget_set_sensitive(redPaletteButton_,nadj);
    gtk_widget_set_sensitive(greenPaletteButton_,nadj);
    gtk_widget_set_sensitive(bluePaletteButton_,nadj);
    gtk_widget_set_sensitive(huePaletteButton_,nadj);
    gtk_widget_set_sensitive(userPaletteButton_,nadj);

    gtk_widget_set_sensitive(fourNeighborhoodButton_,ad);
    gtk_widget_set_sensitive(eightNeighborhoodButton_,ad);
    gtk_widget_set_sensitive(minColorsButton_,ad);
  }

  // ------------------------------------------------------------------------
  //   Config Dialog
  // ------------------------------------------------------------------------


  bool viewer2D::configDialog::build() {
    return viewerBase::configDialog::build();
  }

  bool viewer2D::configDialog::show() {
    return viewerBase::configDialog::show();
  }

  bool viewer2D::configDialog::hide() {
    return viewerBase::configDialog::hide();
  }

  bool viewer2D::configDialog::destroy() {
    return viewerBase::configDialog::destroy();
  }

  void viewer2D::configDialog::initToolkit() {
    _lti_debug4("viewer2D::configDialog::initToolkit()\n");
    if (isNull(tk_)) {
      tk_=new tk(*this);
    }
  }

  void viewer2D::configDialog::destroyToolkit() {
    _lti_debug4("viewer2D::configDialog::destroyToolkit()\n");
    if (notNull(tk_)) {
      delete tk_;
      tk_=0;
    }
  }
  

  viewer2D::configDialog::tk* viewer2D::configDialog::getToolkit() {
    return dynamic_cast<tk*>(tk_);
  }

  bool 
  viewer2D::configDialog::useInformation(const viewerBase::information* data) {
   if (viewerBase::configDialog::useInformation(data)) {
      return getToolkit()->updateInformation(getInformation());
    }
    return false;
  } 
  
  bool viewer2D::configDialog::setParameters(const parameters& par) {
    if (viewerBase::configDialog::setParameters(par)) {
      return getToolkit()->updateParameters();
    }
    return false;
  }


  // -------------------------------------------------------------------------
  // viewer2D::mainWindow::tk
  // -------------------------------------------------------------------------

  viewer2D::mainWindow::tk::tk(viewer2D::mainWindow& parent)
    : viewerBase::mainWindow::tk::tk(parent),
      xbuffer_(0),xbufferSize_(0) {
  }

  viewer2D::mainWindow::tk::~tk() {
    delete[] xbuffer_;
    xbuffer_=0;
    xbufferSize_=0;
  }


  ipoint viewer2D::mainWindow::tk::suggestWindowSize() const {
    const viewer2D::mainWindow* ptr =
      dynamic_cast<const viewer2D::mainWindow*>(parent_);
    assert(notNull(ptr));
    const viewer2D::information& info = ptr->getInformation();
    return info.size;
  }

  bool viewer2D::mainWindow::tk::drawingAreaExposed(const irectangle& area) {

    _lti_debug4("viewer2D::mainWindow::tk::drawingAreaExposed(" <<
                area << ")" << std::endl);

    viewer2D::mainWindow* par =
      dynamic_cast<viewer2D::mainWindow*>(parent_);

    // this can be 0 at the very end of the program, when everything was
    // deleted and we got here in the last microseconds left to the process.
    if (isNull(par)) {
      return false;
    }

    image img;
    par->getFormattedData(area,img);

    _lti_debug4("  drawn image: " << img.size() << std::endl);

    // Convert image to xbuffer
    int currentSize = img.rows()*img.columns()*3;

    if (currentSize > xbufferSize_) {
      delete[] xbuffer_;
      xbuffer_ = new guchar[currentSize];
      xbufferSize_ = currentSize;
    }
    
    guchar* ptr = xbuffer_;

    vector<rgbaPixel>::const_iterator it,e;
    int py;
    for (py=0;py<img.rows();++py) {
      const vector<rgbaPixel>& vct = img.getRow(py);
      for (it = vct.begin(),e=vct.end();it!=e;++it) {
        *ptr++ = static_cast<guchar>((*it).red);
        *ptr++ = static_cast<guchar>((*it).green);
        *ptr++ = static_cast<guchar>((*it).blue);
      }
    }

    // now, on the drawable
    const int xbufCols = area.br.x-area.ul.x+1;

    _lti_debug4("  calling gdk_draw_rgb_image... ");

    // Draw the xbuffer on the drawable.
    gdk_draw_rgb_image(drawingArea_->window,
                       drawingArea_->style->fg_gc[GTK_STATE_NORMAL],
                       area.ul.x,area.ul.y,
                       img.columns(),img.rows(),
                       GDK_RGB_DITHER_NORMAL,
                       xbuffer_,min(xbufCols,img.columns())*3);

    _lti_debug4("done.\n");

    return true;
  }

  bool 
  viewer2D::mainWindow::tk::saveButtonPressed(const std::string& filename) {

    // get info from adjustments  
    GtkAdjustment* hadj =
     gtk_scrolled_window_get_hadjustment(GTK_SCROLLED_WINDOW(scrolledWindow_));
    GtkAdjustment* vadj =
     gtk_scrolled_window_get_vadjustment(GTK_SCROLLED_WINDOW(scrolledWindow_));

    _lti_debug3("  HAdjustment:\n" <<
                "    lower    : " << hadj->lower << std::endl <<
                "    upper    : " << hadj->upper << std::endl <<
                "    value    : " << hadj->value << std::endl <<
                "    page_size: " << hadj->page_size << std::endl);
    _lti_debug3("  VAdjustment:\n" <<
                "    lower    : " << vadj->lower << std::endl <<
                "    upper    : " << vadj->upper << std::endl <<
                "    value    : " << vadj->value << std::endl <<
                "    page_size: " << vadj->page_size << std::endl);

    viewer2D::mainWindow* parent =
      dynamic_cast<viewer2D::mainWindow*>(parent_);
    assert(notNull(parent));

    image img;
    irectangle area(iround(hadj->value),
                    iround(vadj->value),
                    iround(hadj->value+hadj->page_size-1),
                    iround(vadj->value+vadj->page_size-1));
    parent->getFormattedData(area,img);
    
    ioImage saver;
    if (!saver.save(filename,img)) {
      parent->statusBar("ERROR: " + saver.getStatusString());
    } else {
      parent->statusBar("Image " + filename + " successfully saved.");
      return true;
    }

    return false;
  }
  

  // -------------------------------------------------------------------------
  // viewer2D::mainWindow
  // -------------------------------------------------------------------------

  void viewer2D::mainWindow::initToolkit() {
    _lti_debug4("viewer2D::configDialog::initToolkit()\n");
    if (isNull(tk_)) {
      tk_=new tk(*this);
    }
  }

  void viewer2D::mainWindow::destroyToolkit() {
    _lti_debug4("viewer2D::configDialog::destroyToolkit()\n");
    if (notNull(tk_)) {
      delete tk_;
      tk_=0;
    }
  }

  bool viewer2D::mainWindow::saveButtonPressed(const std::string& filename) {
    if (notNull(tk_)) {
      viewer2D::mainWindow::tk* ptr =
        dynamic_cast<viewer2D::mainWindow::tk*>(tk_);
      assert(notNull(ptr));
      return ptr->saveButtonPressed(filename);
    }
    return false;
  }
}

#endif
