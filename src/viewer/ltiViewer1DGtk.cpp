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
 * \file   ltiViewer1DGtk.cpp
 * \author Pablo Alvarado
 * \date   05.07.2005
 * 
 * $Id: ltiViewer1DGtk.cpp,v 1.4 2012-09-16 05:05:59 alvarado Exp $
 */

#include "ltiConfig.h"

#ifdef HAVE_GTK

#include "ltiViewer1DGtk.h"
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
  
  const float viewer1D::configDialog::tk::minZoom = 1.0f/128.0f;

  viewer1D::configDialog::tk::tk(viewer1D::configDialog& parent) :
    viewerBase::configDialog::tk::tk(parent) {
    _lti_enterCTOR();
    _lti_leaveCTOR();
  }

  viewer1D::configDialog::tk::~tk() {
    _lti_enterDTOR();
    // done by the "parent" class
    // hide();
    // destroy();
    _lti_leaveDTOR();
  }

  bool viewer1D::configDialog::tk::build() {
    _lti_debug2("entering viewer1D::configDialog::tk::build()\n");

    if (!viewerBase::configDialog::tk::build()) {
      // window already build!  This shouldn't happen, but it has!
      return false;
    }

    lock_.lock();
    // gdk_threads_enter();

    // the following code has been imported and modified from glade-2
    tooltips_ = gtk_tooltips_new ();

    // vbox_ is inherited from viewerBaseGtk
    // the vboxTop_ contains everything

    displayCtrlBox_ = gtk_hbox_new (FALSE, 0);
    gtk_widget_show (displayCtrlBox_);
    gtk_box_pack_start (GTK_BOX (vbox_), displayCtrlBox_, TRUE, TRUE, 0);

    displayCtrlRBox_ = gtk_vbox_new (FALSE, 0);
    gtk_widget_show (displayCtrlRBox_);
    gtk_box_pack_start (GTK_BOX (displayCtrlBox_), displayCtrlRBox_,
                        TRUE, TRUE, 0);

    displayCtrlFrame = gtk_frame_new (NULL);
    gtk_widget_show (displayCtrlFrame);
    gtk_box_pack_start (GTK_BOX (displayCtrlRBox_), displayCtrlFrame,
                        TRUE, TRUE, 0);
    gtk_frame_set_shadow_type (GTK_FRAME (displayCtrlFrame), GTK_SHADOW_NONE);

    displayCtrlAlignment_ = gtk_alignment_new (0.5, 0.5, 1, 1);
    gtk_widget_show (displayCtrlAlignment_);
    gtk_container_add (GTK_CONTAINER (displayCtrlFrame), displayCtrlAlignment_);
    gtk_alignment_set_padding (GTK_ALIGNMENT (displayCtrlAlignment_),
                               0, 0, 12, 0);

    displayCtrlVBox_ = gtk_vbox_new (FALSE, 0);
    gtk_widget_show (displayCtrlVBox_);
    gtk_container_add (GTK_CONTAINER (displayCtrlAlignment_), displayCtrlVBox_);

    zoomXBox_ = gtk_hbox_new (FALSE, 0);
    gtk_widget_set_name (zoomXBox_, "zoomXBox_");
    gtk_widget_show (zoomXBox_);
    gtk_box_pack_start (GTK_BOX (displayCtrlVBox_), zoomXBox_, FALSE, TRUE, 0);

    zoomXLabel_ = gtk_label_new ("Zoom X");
    gtk_widget_show (zoomXLabel_);
    gtk_box_pack_start (GTK_BOX (zoomXBox_), zoomXLabel_, FALSE, TRUE, 0);
    gtk_misc_set_alignment (GTK_MISC (zoomXLabel_), 0, 0.5);
    gtk_misc_set_padding (GTK_MISC (zoomXLabel_), 2, 0);
    gtk_label_set_width_chars (GTK_LABEL (zoomXLabel_), 7);

    zoomXEntry_ = gtk_entry_new ();
    gtk_widget_show (zoomXEntry_);
    gtk_box_pack_start (GTK_BOX (zoomXBox_), zoomXEntry_, FALSE, TRUE, 0);
    gtk_tooltips_set_tip (tooltips_, zoomXEntry_,
                          "n for zoom-in or 1/n for "                   \
                          "zoom-out, with n a positive integer less or equal " \
                          "32.", NULL);
    gtk_entry_set_max_length (GTK_ENTRY (zoomXEntry_), 5);
    gtk_entry_set_text (GTK_ENTRY (zoomXEntry_), "1");
    gtk_entry_set_width_chars (GTK_ENTRY (zoomXEntry_), 7);

    zoomXAdjustment_ = GTK_ADJUSTMENT(gtk_adjustment_new(0,-126,128,1,5,0));
    zoomXScale_ = gtk_hscale_new (zoomXAdjustment_);
    gtk_widget_show (zoomXScale_);
    gtk_box_pack_start (GTK_BOX (zoomXBox_), zoomXScale_, TRUE, TRUE, 0);
    gtk_scale_set_draw_value (GTK_SCALE (zoomXScale_), FALSE);
    gtk_scale_set_digits (GTK_SCALE (zoomXScale_), 3);

    zoomXSeparator_ = gtk_hseparator_new ();
    gtk_widget_show (zoomXSeparator_);
    gtk_box_pack_start (GTK_BOX (displayCtrlVBox_), zoomXSeparator_, 
                        TRUE, TRUE, 0);

    markerTable_ = gtk_table_new (3, 4, FALSE);
    gtk_widget_show (markerTable_);
    gtk_box_pack_start (GTK_BOX (displayCtrlVBox_), markerTable_,
                        FALSE, TRUE, 0);

    gtk_table_set_col_spacings (GTK_TABLE (markerTable_), 4);

    styleComboBox_ = gtk_combo_box_new_text ();
    gtk_widget_show (styleComboBox_);
    gtk_table_attach (GTK_TABLE (markerTable_), styleComboBox_, 1, 2, 0, 1,
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (GTK_FILL), 0, 0);
    gtk_combo_box_append_text (GTK_COMBO_BOX (styleComboBox_), "dots");
    gtk_combo_box_append_text (GTK_COMBO_BOX (styleComboBox_), "boxes");
    gtk_combo_box_append_text (GTK_COMBO_BOX (styleComboBox_), "steps");
    gtk_combo_box_append_text (GTK_COMBO_BOX (styleComboBox_), "impulses");
    gtk_combo_box_append_text (GTK_COMBO_BOX (styleComboBox_), "lines");

    gtk_combo_box_set_active  (GTK_COMBO_BOX (styleComboBox_), 0);

    markerLabel_ = gtk_label_new ("Marker ");
    gtk_widget_show (markerLabel_);
    gtk_table_attach (GTK_TABLE (markerTable_), markerLabel_, 2, 3, 0, 1,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (markerLabel_), 0, 0.5);

    markerComboBox_ = gtk_combo_box_new_text ();
    gtk_widget_show (markerComboBox_);
    gtk_table_attach (GTK_TABLE (markerTable_), markerComboBox_, 3, 4, 0, 1,
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (GTK_FILL), 0, 0);
    gtk_combo_box_append_text (GTK_COMBO_BOX (markerComboBox_), ". pixel");
    gtk_combo_box_append_text (GTK_COMBO_BOX (markerComboBox_), "o circle");
    gtk_combo_box_append_text (GTK_COMBO_BOX (markerComboBox_), "x cross");
    gtk_combo_box_append_text (GTK_COMBO_BOX (markerComboBox_), "+ plus");
    gtk_combo_box_append_text (GTK_COMBO_BOX (markerComboBox_), "* star");
    gtk_combo_box_append_text (GTK_COMBO_BOX (markerComboBox_), "square");
    gtk_combo_box_append_text (GTK_COMBO_BOX (markerComboBox_), "diamond");
    gtk_combo_box_append_text (GTK_COMBO_BOX (markerComboBox_),
                               "triangle up");
    gtk_combo_box_append_text (GTK_COMBO_BOX (markerComboBox_),
                               "triangle down");
    gtk_combo_box_append_text (GTK_COMBO_BOX (markerComboBox_),
                               "triangle left");
    gtk_combo_box_append_text (GTK_COMBO_BOX (markerComboBox_),
                               "triangle right");
    gtk_combo_box_append_text (GTK_COMBO_BOX (markerComboBox_), "dot");

    gtk_combo_box_set_active (GTK_COMBO_BOX (markerComboBox_), 0);

    heightLabel_ = gtk_label_new ("Height ");
    gtk_widget_show (heightLabel_);
    gtk_table_attach (GTK_TABLE (markerTable_), heightLabel_, 0, 1, 1, 2,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_label_set_justify (GTK_LABEL (heightLabel_), GTK_JUSTIFY_RIGHT);
    gtk_misc_set_alignment (GTK_MISC (heightLabel_), 0, 0.5);

    heightSpin__adj = gtk_adjustment_new (16, 16, 2048, 1, 10, 0);
    heightSpin_ = 
      gtk_spin_button_new (GTK_ADJUSTMENT (heightSpin__adj), 1, 0);
    gtk_widget_show (heightSpin_);
    gtk_table_attach (GTK_TABLE (markerTable_), heightSpin_, 1, 2, 1, 2,
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_spin_button_set_numeric (GTK_SPIN_BUTTON (heightSpin_), TRUE);

    widthLabel_ = gtk_label_new ("Width ");
    gtk_widget_show (widthLabel_);
    gtk_table_attach (GTK_TABLE (markerTable_), widthLabel_, 2, 3, 1, 2,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (widthLabel_), 0, 0.5);

    widthSpin__adj = gtk_adjustment_new (1, 1, 256, 1, 10, 0);
    widthSpin_ = gtk_spin_button_new (GTK_ADJUSTMENT (widthSpin__adj), 1, 0);
    gtk_widget_show (widthSpin_);
    gtk_table_attach (GTK_TABLE (markerTable_), widthSpin_, 3, 4, 1, 2,
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_spin_button_set_numeric (GTK_SPIN_BUTTON (widthSpin_), TRUE);

    styleLabel_ = gtk_label_new ("Style ");
    gtk_widget_show (styleLabel_);
    gtk_table_attach (GTK_TABLE (markerTable_), styleLabel_, 0, 1, 0, 1,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (styleLabel_), 0, 0.5);

    ticksXLabel_ = gtk_label_new ("Ticks X");
    gtk_widget_show (ticksXLabel_);
    gtk_table_attach (GTK_TABLE (markerTable_), ticksXLabel_, 0, 1, 2, 3,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_label_set_justify (GTK_LABEL (ticksXLabel_), GTK_JUSTIFY_RIGHT);
    gtk_misc_set_alignment (GTK_MISC (ticksXLabel_), 0, 0.5);

    ticksXSpin__adj = gtk_adjustment_new (0, 0, 2048, 1, 10, 0);
    ticksXSpin_ =
      gtk_spin_button_new (GTK_ADJUSTMENT (ticksXSpin__adj), 1, 0);
    gtk_widget_show (ticksXSpin_);
    gtk_table_attach (GTK_TABLE (markerTable_), ticksXSpin_, 1, 2, 2, 3,
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_spin_button_set_numeric (GTK_SPIN_BUTTON (ticksXSpin_), TRUE);

    ticksYLabel_ = gtk_label_new ("Ticks Y");
    gtk_widget_show (ticksYLabel_);
    gtk_table_attach (GTK_TABLE (markerTable_), ticksYLabel_, 2, 3, 2, 3,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_label_set_justify (GTK_LABEL (ticksYLabel_), GTK_JUSTIFY_RIGHT);
    gtk_misc_set_alignment (GTK_MISC (ticksYLabel_), 0, 0.5);

    ticksYSpin__adj = gtk_adjustment_new (0, 0, 2048, 1, 10, 0);
    ticksYSpin_ =
      gtk_spin_button_new (GTK_ADJUSTMENT (ticksYSpin__adj), 1, 0);
    gtk_widget_show (ticksYSpin_);
    gtk_table_attach (GTK_TABLE (markerTable_), ticksYSpin_, 3, 4, 2, 3,
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_spin_button_set_numeric (GTK_SPIN_BUTTON (ticksYSpin_), TRUE);

    showThingsTable_ = gtk_table_new (2, 2, TRUE);
    gtk_widget_show (showThingsTable_);
    gtk_box_pack_start (GTK_BOX (displayCtrlVBox_), showThingsTable_, 
                        TRUE, TRUE, 0);
    gtk_table_set_col_spacings (GTK_TABLE (showThingsTable_), 2);

    horizontalAxisButton_ =
      gtk_check_button_new_with_mnemonic ("Show horizontal axis");
    gtk_widget_show (horizontalAxisButton_);
    gtk_table_attach (GTK_TABLE (showThingsTable_),
                      horizontalAxisButton_, 0, 1, 0, 1,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);

    gridButton_ = gtk_check_button_new_with_mnemonic ("Show grid");
    gtk_widget_show (gridButton_);
    gtk_table_attach (GTK_TABLE (showThingsTable_), gridButton_, 1, 2, 0, 1,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);

    meanValueButton_ =
      gtk_check_button_new_with_mnemonic ("Show mean value");
    gtk_widget_show (meanValueButton_);
    gtk_table_attach (GTK_TABLE (showThingsTable_), 
                      meanValueButton_, 0, 1, 1, 2,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);

    forceZeroButton_ = gtk_check_button_new_with_mnemonic ("Force zero");
    gtk_widget_show (forceZeroButton_);
    gtk_table_attach (GTK_TABLE (showThingsTable_),
                      forceZeroButton_, 1, 2, 1, 2,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);

    colorHBox_ = gtk_hbox_new (FALSE, 0);
    gtk_widget_show (colorHBox_);
    gtk_box_pack_start (GTK_BOX (displayCtrlVBox_), colorHBox_, FALSE, TRUE, 0);

    colorLabel_ = gtk_label_new ("Color of ");
    gtk_widget_show (colorLabel_);
    gtk_box_pack_start (GTK_BOX (colorHBox_), colorLabel_, FALSE, FALSE, 0);

    colorsComboBox_ = gtk_combo_box_new_text ();
    gtk_widget_show (colorsComboBox_);
    gtk_box_pack_start (GTK_BOX (colorHBox_), colorsComboBox_, FALSE, TRUE, 0);
    gtk_combo_box_append_text (GTK_COMBO_BOX (colorsComboBox_), "background");
    gtk_combo_box_append_text (GTK_COMBO_BOX (colorsComboBox_), "axes");
    gtk_combo_box_append_text (GTK_COMBO_BOX (colorsComboBox_), "grid");
    gtk_combo_box_append_text (GTK_COMBO_BOX (colorsComboBox_), "lines");
    gtk_combo_box_append_text (GTK_COMBO_BOX (colorsComboBox_), "mean line");
    gtk_combo_box_append_text (GTK_COMBO_BOX (colorsComboBox_), "marker line");

    gtk_combo_box_set_active  (GTK_COMBO_BOX (colorsComboBox_), 0);

    colorButton_ = gtk_color_button_new ();
    gtk_widget_show (colorButton_);
    gtk_box_pack_start (GTK_BOX (colorHBox_), colorButton_, TRUE, TRUE, 0);

    displayCtrlFrameLabel_ = gtk_label_new ("<b>Display Control</b>");
    gtk_widget_show (displayCtrlFrameLabel_);
    gtk_frame_set_label_widget (GTK_FRAME (displayCtrlFrame),
                                displayCtrlFrameLabel_);
    gtk_label_set_use_markup (GTK_LABEL (displayCtrlFrameLabel_), TRUE);

    informationFrame_ = gtk_frame_new (NULL);
    gtk_widget_show (informationFrame_);
    gtk_box_pack_start (GTK_BOX (displayCtrlRBox_), informationFrame_,
                        FALSE, TRUE, 0);
    gtk_frame_set_shadow_type (GTK_FRAME (informationFrame_), GTK_SHADOW_NONE);

    informationAlignment_ = gtk_alignment_new (0.5, 0.5, 1, 1);
    gtk_widget_show (informationAlignment_);
    gtk_container_add (GTK_CONTAINER (informationFrame_),
                       informationAlignment_);
    gtk_alignment_set_padding (GTK_ALIGNMENT (informationAlignment_),
                               0, 0, 12, 0);

    infoTable_ = gtk_table_new (3, 4, FALSE);
    gtk_widget_show (infoTable_);
    gtk_container_add (GTK_CONTAINER (informationAlignment_), infoTable_);
    gtk_table_set_row_spacings (GTK_TABLE (infoTable_), 2);
    gtk_table_set_col_spacings (GTK_TABLE (infoTable_), 2);

    imageSizeLabel_ = gtk_label_new ("Image size");
    gtk_widget_show (imageSizeLabel_);
    gtk_table_attach (GTK_TABLE (infoTable_), imageSizeLabel_, 0, 1, 0, 1,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (imageSizeLabel_), 0, 0.5);

    sizeEntry_ = gtk_entry_new ();
    gtk_widget_show (sizeEntry_);
    gtk_table_attach (GTK_TABLE (infoTable_), sizeEntry_, 1, 2, 0, 1,
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_editable_set_editable (GTK_EDITABLE (sizeEntry_), FALSE);
    gtk_entry_set_text (GTK_ENTRY (sizeEntry_), "1024");
    gtk_entry_set_has_frame (GTK_ENTRY (sizeEntry_), FALSE);
    gtk_entry_set_width_chars (GTK_ENTRY (sizeEntry_), 8);

    minLabel_ = gtk_label_new ("Minimum");
    gtk_widget_show (minLabel_);
    gtk_table_attach (GTK_TABLE (infoTable_), minLabel_, 0, 1, 1, 2,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (minLabel_), 0, 0.5);

    maxLabel = gtk_label_new ("Maximum");
    gtk_widget_show (maxLabel);
    gtk_table_attach (GTK_TABLE (infoTable_), maxLabel, 0, 1, 2, 3,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (maxLabel), 0, 0.5);

    minEntry_ = gtk_entry_new ();
    gtk_widget_show (minEntry_);
    gtk_table_attach (GTK_TABLE (infoTable_), minEntry_, 1, 2, 1, 2,
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_editable_set_editable (GTK_EDITABLE (minEntry_), FALSE);
    gtk_entry_set_text (GTK_ENTRY (minEntry_), "1024");
    gtk_entry_set_has_frame (GTK_ENTRY (minEntry_), FALSE);
    gtk_entry_set_width_chars (GTK_ENTRY (minEntry_), 8);

    maxEntry_ = gtk_entry_new ();
    gtk_widget_show (maxEntry_);
    gtk_table_attach (GTK_TABLE (infoTable_), maxEntry_, 1, 2, 2, 3,
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_editable_set_editable (GTK_EDITABLE (maxEntry_), FALSE);
    gtk_entry_set_text (GTK_ENTRY (maxEntry_), "1024");
    gtk_entry_set_has_frame (GTK_ENTRY (maxEntry_), FALSE);
    gtk_entry_set_width_chars (GTK_ENTRY (maxEntry_), 8);

    averageLabel_ = gtk_label_new ("Average");
    gtk_widget_show (averageLabel_);
    gtk_table_attach (GTK_TABLE (infoTable_), averageLabel_, 2, 3, 1, 2,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (averageLabel_), 0, 0.5);

    stdDevLabel_ = gtk_label_new ("Std.Deviation");
    gtk_widget_show (stdDevLabel_);
    gtk_table_attach (GTK_TABLE (infoTable_), stdDevLabel_, 2, 3, 2, 3,
                      (GtkAttachOptions) (GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_misc_set_alignment (GTK_MISC (stdDevLabel_), 0, 0.5);

    averageEntry_ = gtk_entry_new ();
    gtk_widget_show (averageEntry_);
    gtk_table_attach (GTK_TABLE (infoTable_), averageEntry_, 3, 4, 1, 2,
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_editable_set_editable (GTK_EDITABLE (averageEntry_), FALSE);
    gtk_entry_set_text (GTK_ENTRY (averageEntry_), "1024");
    gtk_entry_set_has_frame (GTK_ENTRY (averageEntry_), FALSE);
    gtk_entry_set_width_chars (GTK_ENTRY (averageEntry_), 8);

    stdDevEntry_ = gtk_entry_new ();
    gtk_widget_show (stdDevEntry_);
    gtk_table_attach (GTK_TABLE (infoTable_), stdDevEntry_, 3, 4, 2, 3,
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (0), 0, 0);
    gtk_editable_set_editable (GTK_EDITABLE (stdDevEntry_), FALSE);
    gtk_entry_set_text (GTK_ENTRY (stdDevEntry_), "1024");
    gtk_entry_set_has_frame (GTK_ENTRY (stdDevEntry_), FALSE);
    gtk_entry_set_width_chars (GTK_ENTRY (stdDevEntry_), 8);

    informationFrameLabel_ = gtk_label_new ("<b>Information</b>");
    gtk_widget_show (informationFrameLabel_);
    gtk_frame_set_label_widget (GTK_FRAME (informationFrame_),
                                informationFrameLabel_);
    gtk_label_set_use_markup (GTK_LABEL (informationFrameLabel_), TRUE);

    saveFrame_ = gtk_frame_new (NULL);
    gtk_widget_show (saveFrame_);
    gtk_box_pack_start (GTK_BOX (displayCtrlRBox_), saveFrame_, FALSE, TRUE, 0);

    saveImageAlignment_ = gtk_alignment_new (0.5, 0, 1, 1);
    gtk_widget_show (saveImageAlignment_);
    gtk_container_add (GTK_CONTAINER (saveFrame_), saveImageAlignment_);
    gtk_alignment_set_padding (GTK_ALIGNMENT (saveImageAlignment_), 0, 0, 4, 0);

    saveImageHBox_ = gtk_hbox_new (FALSE, 0);
    gtk_widget_show (saveImageHBox_);
    gtk_container_add (GTK_CONTAINER (saveImageAlignment_), saveImageHBox_);

    saveImageEntry_ = gtk_entry_new ();
    gtk_widget_show (saveImageEntry_);
    gtk_box_pack_start (GTK_BOX (saveImageHBox_), saveImageEntry_,
                        TRUE, TRUE, 0);
    gtk_tooltips_set_tip (tooltips_, saveImageEntry_,
                          "Name for the image file", NULL);
    gtk_entry_set_text (GTK_ENTRY (saveImageEntry_), "tux.png");

    openButton_ = gtk_button_new_from_stock ("gtk-open");
    gtk_widget_show (openButton_);
    gtk_box_pack_start (GTK_BOX (saveImageHBox_), openButton_, FALSE, FALSE, 0);
    GTK_WIDGET_SET_FLAGS (openButton_, GTK_CAN_DEFAULT);
    gtk_tooltips_set_tip (tooltips_, openButton_,
                          "Browse your filesystem for a destination", NULL);
    gtk_button_set_focus_on_click (GTK_BUTTON (openButton_), FALSE);

    saveButton_ = gtk_button_new ();
    gtk_widget_show (saveButton_);
    gtk_box_pack_start (GTK_BOX (saveImageHBox_), saveButton_, FALSE, FALSE, 0);
    gtk_tooltips_set_tip (tooltips_, saveButton_, "Save the currently " \
                       "displayed image with the filename on the left", NULL);

    saveButtonAlignment_ = gtk_alignment_new (0.5, 0.5, 0, 0);
    gtk_widget_show (saveButtonAlignment_);
    gtk_container_add (GTK_CONTAINER (saveButton_), saveButtonAlignment_);
    
    saveButtonHBox = gtk_hbox_new (FALSE, 2);
    gtk_widget_show (saveButtonHBox);
    gtk_container_add (GTK_CONTAINER (saveButtonAlignment_), saveButtonHBox);
    
    saveButtonImage_ = gtk_image_new_from_stock ("gtk-save", 
                                                 GTK_ICON_SIZE_BUTTON);
    gtk_widget_show (saveButtonImage_);
    gtk_box_pack_start (GTK_BOX (saveButtonHBox), saveButtonImage_,
                        FALSE, FALSE, 0);
    
    saveButtonLabel_ = gtk_label_new_with_mnemonic ("View");
    gtk_widget_show (saveButtonLabel_);
    gtk_box_pack_start (GTK_BOX (saveButtonHBox), saveButtonLabel_,
                        FALSE, FALSE, 0);

    saveAllButton_ = gtk_button_new ();
    gtk_widget_show (saveAllButton_);
    gtk_box_pack_start (GTK_BOX (saveImageHBox_), saveAllButton_, 
                        FALSE, FALSE, 0);
    gtk_tooltips_set_tip (tooltips_, saveAllButton_, "Save complete vector " \
                           "in an image with the filename on the left", NULL);

    saveAllButtonAlignment_ = gtk_alignment_new (0.5, 0.5, 0, 0);
    gtk_widget_show (saveAllButtonAlignment_);
    gtk_container_add (GTK_CONTAINER (saveAllButton_), saveAllButtonAlignment_);
    
    saveAllButtonHBox_ = gtk_hbox_new (FALSE, 2);
    gtk_widget_show (saveAllButtonHBox_);
    gtk_container_add (GTK_CONTAINER (saveAllButtonAlignment_),
                       saveAllButtonHBox_);
    
    saveAllButtonImage_ = gtk_image_new_from_stock ("gtk-save",
                                                    GTK_ICON_SIZE_BUTTON);
    gtk_widget_show (saveAllButtonImage_);
    gtk_box_pack_start (GTK_BOX (saveAllButtonHBox_), saveAllButtonImage_,
                        FALSE, FALSE, 0);
    
    saveAllButtonLabel_ = gtk_label_new_with_mnemonic ("All");
    gtk_widget_show (saveAllButtonLabel_);
    gtk_box_pack_start (GTK_BOX (saveAllButtonHBox_),
                        saveAllButtonLabel_, FALSE, FALSE, 0);

    saveImageLabel_ = gtk_label_new ("Save image");
    gtk_widget_show (saveImageLabel_);
    gtk_frame_set_label_widget (GTK_FRAME (saveFrame_), saveImageLabel_);
    gtk_label_set_use_markup (GTK_LABEL (saveImageLabel_), TRUE);

    zoomYSeparator_ = gtk_vseparator_new ();
    gtk_widget_show (zoomYSeparator_);
    gtk_box_pack_start (GTK_BOX (displayCtrlBox_), zoomYSeparator_,
                        TRUE, TRUE, 0);

    zoomYBox_ = gtk_vbox_new (FALSE, 0);
    gtk_widget_show (zoomYBox_);
    gtk_box_pack_start (GTK_BOX (displayCtrlBox_), zoomYBox_, TRUE, TRUE, 0);

    zoomYLabel_ = gtk_label_new ("Zoom Y");
    gtk_widget_show (zoomYLabel_);
    gtk_box_pack_start (GTK_BOX (zoomYBox_), zoomYLabel_, FALSE, FALSE, 0);
    gtk_label_set_use_markup (GTK_LABEL (zoomYLabel_), TRUE);
    gtk_label_set_width_chars (GTK_LABEL (zoomYLabel_), 7);
    gtk_label_set_angle (GTK_LABEL (zoomYLabel_), 270);

    zoomYEntry_ = gtk_entry_new ();
    gtk_widget_show (zoomYEntry_);
    gtk_box_pack_start (GTK_BOX (zoomYBox_), zoomYEntry_, FALSE, FALSE, 0);
    gtk_tooltips_set_tip (tooltips_, zoomYEntry_,
                          "n for zoom-in or 1/n for "                   \
                          "zoom-out, with n a positive integer less or equal " \
                          "32.", NULL);
    gtk_entry_set_max_length (GTK_ENTRY (zoomYEntry_), 5);
    gtk_entry_set_text (GTK_ENTRY (zoomYEntry_), "1");
    gtk_entry_set_width_chars (GTK_ENTRY (zoomYEntry_), 0);

    zoomYAdjustment_ = GTK_ADJUSTMENT (gtk_adjustment_new (0, -126,128,
                                                           1, 5, 0));
    zoomYScale_ = gtk_vscale_new (zoomYAdjustment_);
    gtk_widget_show (zoomYScale_);
    gtk_box_pack_start (GTK_BOX (zoomYBox_), zoomYScale_, TRUE, TRUE, 0);
    gtk_scale_set_draw_value (GTK_SCALE (zoomYScale_), FALSE);
    gtk_scale_set_digits (GTK_SCALE (zoomYScale_), 3);

    // Signals and their callbacks

    // Zoom
    g_signal_connect ((gpointer) zoomXEntry_, "activate",
                      G_CALLBACK (cbOnZoomXEntryActivate),this);
    g_signal_connect ((gpointer) zoomXAdjustment_, "value_changed",
                      G_CALLBACK (cbOnZoomXAdjustmentValueChanged),this);

    g_signal_connect ((gpointer) zoomYEntry_, "activate",
                      G_CALLBACK (cbOnZoomYEntryActivate),this);
    g_signal_connect ((gpointer) zoomYAdjustment_, "value_changed",
                      G_CALLBACK (cbOnZoomYAdjustmentValueChanged),this);
    
    // Style
    g_signal_connect ((gpointer) styleComboBox_, "changed",
                      G_CALLBACK (cbOnStyleChanged),this);

    // Marker
    g_signal_connect ((gpointer) markerComboBox_, "changed",
                      G_CALLBACK (cbOnMarkerChanged),this);

    // Spin buttons
    g_signal_connect ((gpointer) heightSpin_, "value_changed",
                      G_CALLBACK (cbOnHeightSpinValueChanged),this);
    
    g_signal_connect ((gpointer) widthSpin_, "value_changed",
                      G_CALLBACK (cbOnWidthSpinValueChanged),this);

    g_signal_connect ((gpointer) ticksXSpin_, "value_changed",
                      G_CALLBACK (cbOnTicksXSpinValueChanged),this);

    g_signal_connect ((gpointer) ticksYSpin_, "value_changed",
                      G_CALLBACK (cbOnTicksYSpinValueChanged),this);

    // toggle buttons
    g_signal_connect ((gpointer) horizontalAxisButton_,"toggled",
                      G_CALLBACK (cbOnHorizontalAxisToggled),this);
    
    g_signal_connect ((gpointer) gridButton_,"toggled",
                      G_CALLBACK (cbOnGridToggled),this);
    
    g_signal_connect ((gpointer) meanValueButton_,"toggled",
                      G_CALLBACK (cbOnMeanValueToggled),this);
    
    g_signal_connect ((gpointer) forceZeroButton_,"toggled",
                      G_CALLBACK (cbOnForceZeroToggled),this);
    

    // Color combo box
    g_signal_connect ((gpointer) colorsComboBox_, "changed",
                      G_CALLBACK (cbOnColorComboChanged),
                      this);
    
    // Color selector
    g_signal_connect ((gpointer) colorButton_, "color_set",
                      G_CALLBACK (cbOnColorButtonSet),
                      this);

    // Image saving buttons
    g_signal_connect ((gpointer) openButton_, "clicked",
                      G_CALLBACK (cbOnBrowseButtonClicked),this);
    g_signal_connect ((gpointer) saveButton_, "clicked",
                      G_CALLBACK (cbOnSaveButtonClicked),this);
    g_signal_connect ((gpointer) saveAllButton_, "clicked",
                      G_CALLBACK (cbOnSaveAllButtonClicked),this);
    g_signal_connect ((gpointer) saveImageEntry_, "activate",
                      G_CALLBACK (cbOnFilenameEntryActivate),this);

    //gdk_flush();
    //gdk_threads_leave();
    lock_.unlock();
    _lti_debug2("leaving viewer1D::configDialog::tk::build()\n");

    return true;
  }

  bool viewer1D::configDialog::tk::show() {
    return viewerBase::configDialog::tk::show();
  }

  bool viewer1D::configDialog::tk::hide() {
    return viewerBase::configDialog::tk::hide();
  }

  bool viewer1D::configDialog::tk::destroy() {
    if (viewerBase::configDialog::tk::destroy()) {
      // indicate all destroyed widgets
      lock_.lock();

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
  viewer1D::configDialog::tk::updateParameters() {
    if (viewerBase::configDialog::tk::updateParameters() &&
        dlgBuild_) {
      // since in the if-condition an update was done, we can get the
      // parameters in the arguments of the method safetly casted
      parameters& par = getParameters();

      // zoom
      setZoomX(par.zoom.x);
      setZoomY(par.zoom.y);

      // style
      setStyle(par.style);
      
      // marker
      setMarker(par.marker);
      
      // spin buttons
      gtk_spin_button_set_value(GTK_SPIN_BUTTON(heightSpin_),
                                double(par.vectorHeight));
      gtk_spin_button_set_value(GTK_SPIN_BUTTON(widthSpin_),
                                double(par.pixelsPerElement));
      gtk_spin_button_set_value(GTK_SPIN_BUTTON(ticksXSpin_),
                                double(par.ticks.x));
      gtk_spin_button_set_value(GTK_SPIN_BUTTON(ticksYSpin_),
                                double(par.ticks.y));

      // toggle buttons
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(horizontalAxisButton_),
                                   par.drawHorizontalAxis ? TRUE : FALSE);
      
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gridButton_),
                                   par.drawGrid ? TRUE : FALSE);
      
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(meanValueButton_),
                                   par.drawMeanValue ? TRUE : FALSE);

      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(forceZeroButton_),
                                   par.forceZero ? TRUE : FALSE);

      return true;
    }
    return false;
  }

  bool viewer1D::configDialog::tk::
  updateInformation(const viewer1D::information& info) {
    _lti_debug4("viewer1D::configDialog::tk::updateInformation()\n");

    if (dlgBuild_) {
      dataType_=info.type;

      gtk_entry_set_text(GTK_ENTRY(sizeEntry_),
                         toString(info.size).c_str());

      gtk_entry_set_text(GTK_ENTRY(minEntry_),
                         toString(info.min).c_str());

      gtk_entry_set_text(GTK_ENTRY(maxEntry_),
                         toString(info.max).c_str());

      gtk_entry_set_text(GTK_ENTRY(averageEntry_),
                         toString(info.average).c_str());

      gtk_entry_set_text(GTK_ENTRY(stdDevEntry_),
                         toString(info.stdDeviation).c_str());
    }
    return false;
  }


  viewer1D::parameters& viewer1D::configDialog::tk::getParameters() {
    viewer1D::parameters* ptr =
      dynamic_cast<viewer1D::parameters*>(params_);
    assert(notNull(ptr));
    return *ptr;
  }

  viewer1D::configDialog::tk& 
  viewer1D::configDialog::tk::copy(const tk& other) {

    viewerBase::configDialog::tk::copy(other);
    // TODO: what to do?  deep copy of parameters?  maybe the tk should
    // never be copied... still to be decided...
    
    return *this;
  }

  
  std::string viewer1D::configDialog::tk::toString(const double val,
                                                   const int decimals) {
    char phrase[8];
    sprintf(phrase,"%s%i%s","%.",decimals,"f");
    char tmpBuf[64];
    sprintf(tmpBuf,phrase,val); //"%.4f"
    return std::string(tmpBuf);  
  }

  std::string viewer1D::configDialog::tk::toString(const int val) {
    char tmpBuf[64];
    sprintf(tmpBuf,"%i",val); //"%.4f"
    return std::string(tmpBuf);  
  }

  std::string viewer1D::configDialog::tk::toString(const ipoint& val) {
    return toString(val.x) + " x " + toString(val.y);
  }
  
  std::string viewer1D::configDialog::tk::toString(const rgbaPixel& val) {
    return
      "(" +
      toString(val.red) + ", " + 
      toString(val.green) + ", " +
      toString(val.blue) + ")";
  }

  std::string viewer1D::configDialog::tk::toString(const frgbPixel& val,
                                                   const int decimals) {
    return
      "(" +
      toString(val.red,  decimals) + ", " + 
      toString(val.green,decimals) + ", " +
      toString(val.blue, decimals) +
      ")";
  }

  double viewer1D::configDialog::tk::fromString(const char* val) {
    return atof(val);
  }

  /*
   * Set the GUI elements to the proper zoom value
   */
  void viewer1D::configDialog::tk::setZoomX(const float& zoom) {
    int izoom;
    std::string str;
    // ensure apropriate zoom values, no less than 1/32th and only n or 1/n
    // with n integer.

    if (zoom < minZoom) {
      izoom = 1;
    } else if (zoom<1.0f) {
      izoom = iround(1.0f/zoom);
      str = "1/" + toString(izoom);
      izoom=2-izoom;
    } else {
      izoom = iround(zoom);
      str = toString(izoom);
    }

    gtk_adjustment_set_value(GTK_ADJUSTMENT(zoomXAdjustment_),izoom);    
    gtk_entry_set_text(GTK_ENTRY(zoomXEntry_),str.c_str());
  }
  
  /*
   * Set the GUI elements to the proper zoom value
   */
  void viewer1D::configDialog::tk::setZoomY(const float& zoom) {
    int izoom;
    std::string str;
    // ensure apropriate zoom values, no less than 1/32th and only n or 1/n
    // with n integer.

    if (zoom < minZoom) {
      izoom = 1;
    } else if (zoom<1.0f) {
      izoom = iround(1.0f/zoom);
      str = "1/" + toString(izoom);
      izoom=2-izoom;
    } else {
      izoom = iround(zoom);
      str = toString(izoom);
    }

    gtk_adjustment_set_value(GTK_ADJUSTMENT(zoomYAdjustment_),float(izoom));    
    gtk_entry_set_text(GTK_ENTRY(zoomYEntry_),str.c_str());    
  }
  

  fpoint viewer1D::configDialog::tk::getZoom() const {
    return fpoint(getZoomX(),getZoomY());
  }

  /*
   * Get the proper zoom value from the GUI elements
   */
  float viewer1D::configDialog::tk::getZoomX() const {
    std::string str(gtk_entry_get_text(GTK_ENTRY(zoomXEntry_)));
    float fzoom;
    
    // try first if the user entered in the format "1/xx"
    if (str.find("1/") == 0) {
      str.erase(0,2); // erase the prefix
      fzoom = 1.0f/static_cast<float>(fromString(str.c_str()));
    } else {
      fzoom = static_cast<float>(fromString(str.c_str()));
    }
    
    if (fzoom < minZoom) {
      fzoom = 1.0f;
    } else if (fzoom<1.0f) {
      fzoom = 1.0f/(iround(1.0f/fzoom));
    } else {
      fzoom = iround(fzoom);
    }

    return fzoom;
  }

  float viewer1D::configDialog::tk::getZoomY() const {
    std::string str(gtk_entry_get_text(GTK_ENTRY(zoomYEntry_)));
    float fzoom;

    // try first if the user entered in the format "1/xx"
    if (str.find("1/") == 0) {
      str.erase(0,2); // erase the prefix
      fzoom = 1.0f/static_cast<float>(fromString(str.c_str()));
    } else {
      fzoom = static_cast<float>(fromString(str.c_str()));
    }
    
    if (fzoom < minZoom) {
      fzoom = 1.0f;
    } else if (fzoom<1.0f) {
      fzoom = 1.0f/(iround(1.0f/fzoom));
    } else {
      fzoom = iround(fzoom);
    }

    return fzoom;
  }

  void viewer1D::configDialog::tk::
  cbOnZoomXEntryActivate(GtkCellEditable *,
                         gpointer userData) {
    viewer1D::configDialog::tk* me = (viewer1D::configDialog::tk*)userData;
    float zoom = me->getZoomX();
    me->setZoomX(zoom); // only GUI
    me->getParameters().zoom.x = zoom;
  }
  
  void viewer1D::configDialog::tk::
  cbOnZoomXAdjustmentValueChanged(GtkAdjustment *scale,
                                  gpointer  userData) {
    viewer1D::configDialog::tk* me = (viewer1D::configDialog::tk*)userData;
    float zoom = static_cast<float>(scale->value);
    _lti_debug4("zoom x value: " << zoom << std::endl);

    if (zoom <= 0.5f) {
      zoom = 1.0f/round(2.0f-zoom);
    } else {
      zoom = round(zoom);
    }
    me->setZoomX(zoom); // only GUI
    me->getParameters().zoom.x = zoom;
  }

  void viewer1D::configDialog::tk::
  cbOnZoomYEntryActivate(GtkCellEditable *,
                         gpointer userData) {

    viewer1D::configDialog::tk* me = (viewer1D::configDialog::tk*)userData;
    float zoom = me->getZoomY();
    me->setZoomY(zoom); // only GUI
    me->getParameters().zoom.y = zoom;
  }
  
  void viewer1D::configDialog::tk::
  cbOnZoomYAdjustmentValueChanged(GtkAdjustment *scale,
                                  gpointer  userData) {
    viewer1D::configDialog::tk* me = (viewer1D::configDialog::tk*)userData;
    float zoom = static_cast<float>(scale->value);
    if (zoom <= 0.5f) {
      zoom = 1.0f/round(2.0f-zoom);
    } else {
      zoom = round(zoom);
    }
    me->setZoomY(zoom); // only GUI
    me->getParameters().zoom.y = zoom;
  }

  void viewer1D::configDialog::tk::cbOnStyleChanged(GtkComboBox*,
                                                    gpointer     userData) {

    viewer1D::configDialog::tk* me = (viewer1D::configDialog::tk*)userData;
    viewer1DPainter::eStyle style = me->getStyle();
    me->getParameters().style = style;
  }

  void viewer1D::configDialog::tk::
  cbOnHeightSpinValueChanged(GtkSpinButton* spin,gpointer userData) {

    viewer1D::configDialog::tk* me = (viewer1D::configDialog::tk*)userData;
    me->getParameters().vectorHeight = gtk_spin_button_get_value_as_int(spin);
  }

  void viewer1D::configDialog::tk::
  cbOnWidthSpinValueChanged(GtkSpinButton* spin,gpointer userData) {
    
    viewer1D::configDialog::tk* me = (viewer1D::configDialog::tk*)userData;
    me->getParameters().pixelsPerElement = 
      gtk_spin_button_get_value_as_int(spin);
  }

  void viewer1D::configDialog::tk::
  cbOnTicksXSpinValueChanged(GtkSpinButton* spin,gpointer userData) {

    viewer1D::configDialog::tk* me = (viewer1D::configDialog::tk*)userData;
    me->getParameters().ticks.x = gtk_spin_button_get_value_as_int(spin);
  }

  void viewer1D::configDialog::tk::
  cbOnTicksYSpinValueChanged(GtkSpinButton* spin,gpointer userData) {

    viewer1D::configDialog::tk* me = (viewer1D::configDialog::tk*)userData;
    me->getParameters().ticks.y = gtk_spin_button_get_value_as_int(spin);
  }

  /**
   * Set the style 
   */
  void viewer1D::configDialog::tk::
  setStyle(const viewer1DPainter::eStyle style) {
    int idx=-1;
    switch(style) {
    case viewer1DPainter::Dots:
      idx=0;
      break;
    case viewer1DPainter::Boxes:
      idx=1;
      break;
    case viewer1DPainter::Steps:
      idx=2;
      break;
    case viewer1DPainter::Impulses:
      idx=3;
      break;
    case viewer1DPainter::Lines:
      idx=4;
      break;
    default:
      idx=-1;
    }
    gtk_combo_box_set_active  (GTK_COMBO_BOX (styleComboBox_), idx);
  }
  
  /**
   * Get the style 
   */
  viewer1DPainter::eStyle viewer1D::configDialog::tk::getStyle() const {
    switch(gtk_combo_box_get_active(GTK_COMBO_BOX (styleComboBox_))) {
    case 0:
      return viewer1DPainter::Dots;
      break;
    case 1:
      return viewer1DPainter::Boxes;
      break;
    case 2:
      return viewer1DPainter::Steps;
      break;
    case 3:
      return viewer1DPainter::Impulses;
      break;
    case 4:
      return viewer1DPainter::Lines;
      break;
    default:
      return viewer1DPainter::Dots;
    }
    return viewer1DPainter::Dots; 
  }
  
  void viewer1D::configDialog::tk::cbOnMarkerChanged(GtkComboBox*,
                                                    gpointer     userData) {

    viewer1D::configDialog::tk* me = (viewer1D::configDialog::tk*)userData;
    me->getParameters().marker = me->getMarker();
    _lti_debug4("cb: marker=" << me->getParameters().marker << std::endl);
  }

  /**
   * Set the style 
   */
  void viewer1D::configDialog::tk::setMarker(const std::string & str) {
    int idx=-1;
    if (str.empty()) {
      return;
    }

    char c = str[str.length()-1];
    switch(c) {
    case '.':
      idx=0;
      break;
    case 'o':
      idx=1;
      break;
    case 'x':
      idx=2;
      break;
    case '+':
      idx=3;
      break;
    case '*':
      idx=4;
      break;
    case 's':
      idx=5;
      break;
    case 'd':
      idx=6;
      break;
    case '^':
      idx=7;
      break;
    case 'v':
      idx=8;
      break;
    case '<':
      idx=9;
      break;
    case '>':
      idx=10;
      break;
    case '#':
      idx=11;
      break;
    default:
      idx=-1;
    }
    gtk_combo_box_set_active  (GTK_COMBO_BOX (markerComboBox_), idx);
  }
  
  /**
   * Get the style 
   */
  std::string viewer1D::configDialog::tk::getMarker() const {
    const int idx = gtk_combo_box_get_active(GTK_COMBO_BOX (markerComboBox_));

    _lti_debug4("getMarker() got the index " << idx << std::endl);

    switch(idx) {
    case 0:
      return ".";
      break;
    case 1:
      return "o";
      break;
    case 2:
      return "x";
      break;
    case 3:
      return "+";
      break;
    case 4:
      return "*";
      break;
    case 5:
      return "s";
      break;
    case 6:
      return "d";
      break;
    case 7:
      return "^";
      break;
    case 8:
      return "v";
      break;
    case 9:
      return "<";
      break;
    case 10:
      return ">";
      break;
    case 11:
      return "#";
      break;
    default:
      return ".";
    }
    return ".";
  }
  
  void viewer1D::configDialog::tk::
  cbOnFilenameEntryActivate(GtkCellEditable *,
                            gpointer userData) {
    viewer1D::configDialog::tk* me = (viewer1D::configDialog::tk*)userData;

    std::string str;
    str = gtk_entry_get_text(GTK_ENTRY(me->saveImageEntry_));
    if (!str.empty()) {
      me->getParameters().filename = str;
    }
  }
  
  /*
   * Call back if the browse button is clicked
   */
  void viewer1D::configDialog::tk::onBrowseButtonClicked(GtkButton *,
                                                         eSaveAction action) {
    
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

    std::string filename = gtk_entry_get_text(GTK_ENTRY(saveImageEntry_));
    getParameters().filename = filename;

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
      getParameters().filename = f;

      // set the name in the entry
      gtk_entry_set_text(GTK_ENTRY(saveImageEntry_),f);

      if (!(getParameters().filename.empty())) {
        if (action == SaveAll) {
          // and call the corresponding save method
          cbOnSaveAllButtonClicked(GTK_BUTTON(saveAllButton_),this);
        } else if (action == SaveView) {
          // and call the corresponding save method
          cbOnSaveButtonClicked(GTK_BUTTON(saveButton_),this);
        }
      }

      g_free (f);
    }

    gtk_widget_destroy(fileChooser);
    
  }


  void viewer1D::configDialog::tk::cbOnBrowseButtonClicked(GtkButton *button,
                                                           gpointer userData) {
    viewer1D::configDialog::tk* me = (viewer1D::configDialog::tk*) userData;
    me->onBrowseButtonClicked(button,SaveNone);
  }
  
  void viewer1D::configDialog::tk::cbOnSaveButtonClicked(GtkButton *,
                                                         gpointer   userData){

    viewer1D::configDialog::tk* me = (viewer1D::configDialog::tk*) userData;
    
    viewer1D::configDialog* ptr =
      dynamic_cast<viewer1D::configDialog*>(&me->parent_);

    if (notNull(ptr)) {
      // get the filename in the entry
      std::string str;
      str = gtk_entry_get_text(GTK_ENTRY(me->saveImageEntry_));
      me->getParameters().filename = str;

      if (str.empty()) {
        me->onBrowseButtonClicked(GTK_BUTTON(me->openButton_),SaveView);
      } else {
        ptr->saveButtonPressed(str);
      }
    }
  }

  void viewer1D::configDialog::tk::cbOnSaveAllButtonClicked(GtkButton *,
                                                            gpointer userData){

    viewer1D::configDialog::tk* me = (viewer1D::configDialog::tk*) userData;
    
    viewer1D::configDialog* ptr =
      dynamic_cast<viewer1D::configDialog*>(&me->parent_);

    if (notNull(ptr)) {
      // get the filename in the entry
      std::string str;
      str = gtk_entry_get_text(GTK_ENTRY(me->saveImageEntry_));
      me->getParameters().filename = str;

      if (str.empty()) {
        me->onBrowseButtonClicked(GTK_BUTTON(me->openButton_),SaveAll);
      } else {
        ptr->saveAllButtonPressed(str);
      }
    }
  }


  /*
   * Callback when horizontal axis button is clicked
   */
  void viewer1D::configDialog::tk::
  cbOnHorizontalAxisToggled(GtkToggleButton* toggleButton,
                            gpointer userData) {

    viewer1D::configDialog::tk* me = (viewer1D::configDialog::tk*) userData;

    me->getParameters().drawHorizontalAxis = toggleButton->active;

  }

  /*
   * Callback when grid button is clicked
   */
  void viewer1D::configDialog::tk::
  cbOnGridToggled(GtkToggleButton* toggleButton,
                  gpointer userData) {

    viewer1D::configDialog::tk* me = (viewer1D::configDialog::tk*) userData;

    me->getParameters().drawGrid = toggleButton->active;
  }


  /*
   * Callback when mean value button is clicked
   */
  void viewer1D::configDialog::tk::
  cbOnMeanValueToggled(GtkToggleButton* toggleButton,
                       gpointer userData) {

    viewer1D::configDialog::tk* me = (viewer1D::configDialog::tk*) userData;

    me->getParameters().drawMeanValue = toggleButton->active;
  }

  
  /*
   * Callback when force zero button is clicked
   */
  void viewer1D::configDialog::tk::
  cbOnForceZeroToggled(GtkToggleButton* toggleButton,
                       gpointer userData) {

    viewer1D::configDialog::tk* me = (viewer1D::configDialog::tk*) userData;

    me->getParameters().forceZero = toggleButton->active;
  }

  /**
   * Callback when color selection button is clicked
   */
  void viewer1D::configDialog::tk::
  cbOnColorButtonSet(GtkColorButton  *colorbutton,
                     gpointer         userData) {

    viewer1D::configDialog::tk* me = (viewer1D::configDialog::tk*) userData;

    rgbaPixel rgb;
    if (me->getColor(rgb)) {
      //
      GdkColor color;
      gtk_color_button_get_color(colorbutton,&color);
      me->setColor(rgbaPixel(static_cast<ubyte>(color.red/256),
                             static_cast<ubyte>(color.green/256),
                             static_cast<ubyte>(color.blue/256),
                             0));
    }
  }
  
  /**
   * Callback for marker changed
   */
  void viewer1D::configDialog::tk::
  cbOnColorComboChanged(GtkComboBox*,
                        gpointer     userData) {

    viewer1D::configDialog::tk* me = (viewer1D::configDialog::tk*) userData;
    
    rgbaPixel rgb;
    me->getColor(rgb);

    GdkColor color;

    color.red   = rgb.red*257;
    color.green = rgb.green*257;
    color.blue  = rgb.blue*257;

    gtk_color_button_set_color(GTK_COLOR_BUTTON(me->colorButton_),
                               &color);
  }
    
  void viewer1D::configDialog::tk::setColor(const rgbaPixel& color) {
    
    parameters& par = getParameters();

    switch(gtk_combo_box_get_active(GTK_COMBO_BOX (colorsComboBox_))) {
    case 0:
      par.backgroundColor.copy(color);
      break;
    case 1:
      par.axisColor.copy(color);
      break;
    case 2:
      par.gridColor.copy(color);
      break;
    case 3:
      par.lineColor.copy(color);
      break;
    case 4:
      par.meanColor.copy(color);
      break;
    case 5:
      par.markerColor.copy(color);
      break;
    default:
      break;
    }
  }

  bool viewer1D::configDialog::tk::getColor(rgbaPixel& color) {
  
    const parameters& par = getParameters();

    switch(gtk_combo_box_get_active(GTK_COMBO_BOX (colorsComboBox_))) {
    case 0:
      color.copy(par.backgroundColor);
      break;
    case 1:
      color.copy(par.axisColor);
      break;
    case 2:
      color.copy(par.gridColor);
      break;
    case 3:
      color.copy(par.lineColor);
      break;
    case 4:
      color.copy(par.meanColor);
      break;
    case 5:
      color.copy(par.markerColor);
      break;
    default:
      return false;
    }
    return true;
  }

  // ------------------------------------------------------------------------
  //   Config Dialog
  // ------------------------------------------------------------------------


  bool viewer1D::configDialog::build() {
    return viewerBase::configDialog::build();
  }

  bool viewer1D::configDialog::show() {
    return viewerBase::configDialog::show();
  }

  bool viewer1D::configDialog::hide() {
    return viewerBase::configDialog::hide();
  }

  bool viewer1D::configDialog::destroy() {
    return viewerBase::configDialog::destroy();
  }

  void viewer1D::configDialog::initToolkit() {
    _lti_debug4("viewer1D::configDialog::initToolkit()\n");
    if (isNull(tk_)) {
      tk_=new tk(*this);
    }
  }

  void viewer1D::configDialog::destroyToolkit() {
    _lti_debug4("viewer1D::configDialog::destroyToolkit()\n");
    if (notNull(tk_)) {
      delete tk_;
      tk_=0;
    }
  }
  

  viewer1D::configDialog::tk* viewer1D::configDialog::getToolkit() {
    return dynamic_cast<tk*>(tk_);
  }

  bool 
  viewer1D::configDialog::useInformation(const viewerBase::information* data) {
   if (viewerBase::configDialog::useInformation(data)) {
      return getToolkit()->updateInformation(getInformation());
    }
    return false;
  } 
  
  bool viewer1D::configDialog::setParameters(const parameters& par) {
    if (viewerBase::configDialog::setParameters(par)) {
      return getToolkit()->updateParameters();
    }
    return false;
  }


  // -------------------------------------------------------------------------
  // viewer1D::mainWindow::tk
  // -------------------------------------------------------------------------

  viewer1D::mainWindow::tk::tk(viewer1D::mainWindow& parent)
    : viewerBase::mainWindow::tk::tk(parent),
      xbuffer_(0),xbufferSize_(0) {
  }

  viewer1D::mainWindow::tk::~tk() {
    delete[] xbuffer_;
    xbuffer_=0;
    xbufferSize_=0;
  }


  ipoint viewer1D::mainWindow::tk::suggestWindowSize() const {
    const viewer1D::mainWindow* ptr =
      dynamic_cast<const viewer1D::mainWindow*>(parent_);
    assert(notNull(ptr));
    return ptr->computeCanvasSize();
  }

  bool viewer1D::mainWindow::tk::drawingAreaExposed(const irectangle& area) {

    _lti_debug4("viewer1D::mainWindow::tk::drawingAreaExposed(" <<
                area << ")" << std::endl);

    viewer1D::mainWindow* par =
      dynamic_cast<viewer1D::mainWindow*>(parent_);

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
  viewer1D::mainWindow::tk::saveButtonPressed(const std::string& filename) {

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

    viewer1D::mainWindow* parent =
      dynamic_cast<viewer1D::mainWindow*>(parent_);
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
  
  bool 
  viewer1D::mainWindow::tk::saveAllButtonPressed(const std::string& filename) {

    viewer1D::mainWindow* parent =
      dynamic_cast<viewer1D::mainWindow*>(parent_);
    assert(notNull(parent));

    image img;
    parent->getFormattedData(img);
    
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
  // viewer1D::mainWindow
  // -------------------------------------------------------------------------

  void viewer1D::mainWindow::initToolkit() {
    _lti_debug4("viewer1D::configDialog::initToolkit()\n");
    if (isNull(tk_)) {
      tk_=new tk(*this);
    }
  }

  void viewer1D::mainWindow::destroyToolkit() {
    _lti_debug4("viewer1D::configDialog::destroyToolkit()\n");
    if (notNull(tk_)) {
      delete tk_;
      tk_=0;
    }
  }

  bool viewer1D::mainWindow::saveButtonPressed(const std::string& filename) {
    if (notNull(tk_)) {
      viewer1D::mainWindow::tk* ptr =
        dynamic_cast<viewer1D::mainWindow::tk*>(tk_);
      assert(notNull(ptr));
      return ptr->saveButtonPressed(filename);
    }
    return false;
  }

  bool viewer1D::mainWindow::saveAllButtonPressed(const std::string& filename) {
    if (notNull(tk_)) {
      viewer1D::mainWindow::tk* ptr =
        dynamic_cast<viewer1D::mainWindow::tk*>(tk_);
      assert(notNull(ptr));
      return ptr->saveAllButtonPressed(filename);
    }
    return false;
  }

}


#endif
