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
 * \file   ltiViewer1D.cpp
 *         Contains the viewer for 1D data
 * \author Pablo Alvarado
 * \date   05.01.2005
 *
 * revisions ..: $Id: ltiViewer1D.cpp,v 1.1 2010/01/19 03:34:39 alvarado Exp $
 */

#include <cstdio>

#include "ltiViewer1D.h"
#include "ltiViewer1DPainter.h"

// ---------------------------------------------------------------------------
// Some Debuggin Macros

#undef _LTI_DEBUG
// #define _LTI_DEBUG 4
#include "ltiDebug.h"

// end of debugging macro definitions
// ---------------------------------------------------------------------------

namespace lti {
  // --------------------------------------------------------------
  // The parameters
  // --------------------------------------------------------------

  viewer1D::parameters::parameters() :
    parametersManager::parameters(), 
    viewerBase::parameters(),
    viewer1DPainter::parameters() {
    filename = "tux.png";
  }

  viewer1D::parameters::parameters(const parameters& other) :
    parametersManager::parameters(),
    viewerBase::parameters(),
    viewer1DPainter::parameters() {
    copy(other);
  }

  viewer1D::parameters::~parameters() {
  }
  
  viewer1D::parameters& viewer1D::parameters::copy(const parameters& other) {
    viewer1DPainter::parameters::copy(other);
    viewerBase::parameters::copy(other);

    filename = other.filename;
    return *this;
  }
  
  viewer1D::parameters& 
  viewer1D::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  const std::string& viewer1D::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }
  
  viewer1D::parameters* viewer1D::parameters::clone() const {
    return new viewer1D::parameters(*this);
  }
  
  viewer1D::parameters* viewer1D::parameters::newInstance() const {
    return new viewer1D::parameters();
  }
  
  bool viewer1D::parameters::write(ioHandler& handler,
                                   const bool complete) const {
    bool b = true;
    if (complete) {
      b=handler.writeBegin();
    }

    b = b && viewer1DPainter::parameters::write(handler,false);
    b = b && viewerBase::parameters::write(handler,false);
    b = b && lti::write(handler,"filename",filename);

    if (complete) {
      b=b && handler.writeEnd();
    }

    return b;
  }

  bool viewer1D::parameters::read(ioHandler& handler,
                                  const bool complete) {
    bool b = true;
    if (complete) {
      b=handler.readBegin();
    }

    b = b && viewer1DPainter::parameters::read(handler,false);
    b = b && viewerBase::parameters::read(handler,false);
    b = b && lti::read(handler,"filename",filename);

    if (complete) {
      b=b && handler.readEnd();
    }

    return b;
  }

  // -------------------------------------------------------------------------
  // viewer1D
  // -------------------------------------------------------------------------

  // some inline stuff

  inline const ivector* viewer1D::getDataAsIntegers() const {
    return dynamic_cast<const ivector*>(data_);
  }

  inline const dvector* viewer1D::getDataAsDoubles() const {
    return dynamic_cast<const dvector*>(data_);
  }
  
  viewer1D::viewer1D(const std::string& title) 
    : viewerBase(),data_(0),painter_(0) {
    ensureMainWindow();
    attachConfig(new configDialog(*this)); // DEBUG
    parameters defPar;
    defPar.title=title;
    setParameters(defPar);    
  }

  viewer1D::viewer1D(const viewer1D& other) 
    : viewerBase(),data_(0),painter_(0) {
    ensureMainWindow();
    attachConfig(new configDialog(*this)); // DEBUG
    copy(other);
  }

  viewer1D::viewer1D(const parameters& params) 
    : viewerBase(),data_(0),painter_(0) {
    ensureMainWindow();
    attachConfig(new configDialog(*this)); // DEBUG
    setParameters(params);    
  }

  viewer1D::~viewer1D() {
    lock_.lock();
    delete painter_;
    delete data_;
    data_=0;
    lock_.unlock();
  }

  viewer1D* viewer1D::clone() const {
    return new viewer1D(*this);
  }

  viewer1D* viewer1D::newInstance() const {
    return new viewer1D();
  }

  viewer1D& viewer1D::copy(const viewer1D& other) {
    viewerBase::copy(other);
    lock_.lock();
    if (notNull(other.data_)) {
      data_=other.data_->clone();
    }
    lock_.unlock();
    return *this;
  }

  const viewer1D::parameters& viewer1D::getParameters() const {
     const parameters* par =
      dynamic_cast<const parameters*>(&viewerBase::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }

  bool viewer1D::ensureMainWindow() {
    if (!validMainWindow()) {
      attachMainWindow(new mainWindow(*this));
      return getMainWindow().build();
    }
    return false;
  }

  bool viewer1D::setPainter(const viewer1DPainter& painter) {
    lock_.lock();
    delete painter_;
    painter_ = painter.clone();
    lock_.unlock();
    return (notNull(painter_));
  }

  bool viewer1D::saveButtonPressed(const std::string& filename) {
    if (validMainWindow()) {
      return getMainWindow().saveButtonPressed(filename);
    }
    return false;
  }

  bool viewer1D::saveAllButtonPressed(const std::string& filename) {
    if (validMainWindow()) {
      return getMainWindow().saveAllButtonPressed(filename);
    }
    return false;
  }

  viewer1DPainter* viewer1D::newDefaultPainter() const {
    return new viewer1DPainter;
  }
  
  const viewer1D::information& viewer1D::getInformation() const {
    const information* info =
      dynamic_cast<const information*>(&viewerBase::getInformation());
    if (isNull(info)) {
      throw exception("Empty or wrong information block");
    }
    return *info;
  }

  viewer1D::information& viewer1D::getInformation() {
    information* info =
      dynamic_cast<information*>(&viewerBase::getInformation());
    if (isNull(info)) {
      throw exception("Empty or wrong information block");
    }
    return *info;
  }

  bool viewer1D::ensureInformationType(const eDataType& type,
                                       const int& vectorSize) {
    if (!validInformation()) {
      information info;
      info.type = type;
      info.size = vectorSize;
      setInformation(info);
      return true; // indicate that something changed
    }
    
    information& info = getInformation();

    info.size = vectorSize;
    if (type == info.type) {
      return false; // nothing has changed
    }

    info.type = type;
    return true; // something changed.
  }

  bool viewer1D::updateInformation() {
    lock_.lock();
    if (viewerBase::updateInformation()) {
      information& i = getInformation();
      // since "ensureInformationType" is called in every show, we can
      // assume that the type of data_ is correctly known for the cast
      switch(i.type) {
        case Fixed: {
          const ivector* vct =getDataAsIntegers();
          if (notNull(vct)) {
            i.compute(*vct);
          }
        } break;
        case Float: {
          const dvector* vct =getDataAsDoubles();
          if (notNull(vct)) {
            i.compute(*vct);
          }
        } break;
        default:
          break;
      }
      lock_.unlock();
      return true;
    }
    lock_.unlock();
    return false;
  }

  const viewer1D::mainWindow& viewer1D::getMainWindow() const {
    const mainWindow* wnd =
      dynamic_cast<const mainWindow*>(&viewerBase::getMainWindow());
    if (isNull(wnd)) {
      throw exception("Empty or wrong main window");
    }
    return *wnd;
  }

  viewer1D::mainWindow& viewer1D::getMainWindow() {
    mainWindow* wnd = dynamic_cast<mainWindow*>(&viewerBase::getMainWindow());
    if (isNull(wnd)) {
      throw exception("Empty or wrong main window");
    }
    return *wnd;
  }

  const viewer1D::configDialog& viewer1D::getConfigDialog() const {
    const viewerBase::configDialog* ptr = &viewerBase::getConfigDialog();
    return *dynamic_cast<const viewer1D::configDialog*>(ptr);
  }

  viewer1D::configDialog& viewer1D::getConfigDialog() {
    viewerBase::configDialog* ptr = &viewerBase::getConfigDialog();
    return *dynamic_cast<viewer1D::configDialog*>(ptr);
  }

  bool viewer1D::validPosition(const ipoint& position) const {
    const information& info = getInformation();
    const parameters& par   = getParameters();
    const ipoint p(static_cast<int>(position.x/
                                    (par.zoom.x*par.pixelsPerElement)),
                   static_cast<int>(position.y/par.zoom.y));

    return ((p.x<info.size) && (p.y<par.vectorHeight) && (p.x>=0) && (p.y>=0));
  }

  // this overloaded version of onKMInteraction takes care of the display of
  // pixel coordinates and value
  bool viewer1D::onKMInteraction(const interaction& action,
                                 const ipoint& position) {
    lock_.lock(); // it can happen that while we are here, the data changes!
    if (validMainWindow() && notNull(data_)) {

      if ((action == MouseMoved)   ||
          (action == ButtonPressed)  ) {

        const information& info = getInformation();
        const parameters& par   = getParameters();
        // truncate the integer values
        const ipoint p(static_cast<int>(position.x/
                                        (par.zoom.x*par.pixelsPerElement)),
                       static_cast<int>(position.y/par.zoom.y));

        char buffer[256];
        std::string str;
        sprintf(buffer,"at(%d) = ",p.x);
        str = buffer;

        if ((p.x<0) || (p.x>=info.size)) {
          str += "undefined";
        } else {
          switch (info.type) {
          case Fixed: {
            const ivector* vct = getDataAsIntegers();
            const int val = vct->at(p.x);
            sprintf(buffer,"%d",val);
            str += buffer;
          } break;
          case Float: {
            const dvector* vct = getDataAsDoubles();
            const double val = vct->at(p.x);
            sprintf(buffer,"%f",val);
            str += buffer;
          } break;
          default:
            str += "unknown";
          }
        }
        getMainWindow().statusBar(str);
      }
      lock_.unlock();
      return true;
    }
    
    // if not valid info
    lock_.unlock();
    return false;
  }

  bool viewer1D::getFormattedData(const irectangle& rect,
                                        image& dest) {
    lock_.lock();
    if (isNull(painter_)) {
      painter_=newDefaultPainter();
    }
    
    painter_->useParameters(getRWParameters());

    switch (getInformation().type) {
    case Fixed: {
      const ivector* vct = getDataAsIntegers();
      if (notNull(vct)) {
        painter_->apply(*vct,rect,dest);
        lock_.unlock();
        return true;
      }
    } break;
    case Float: {
      const dvector* vct = getDataAsDoubles();
      if (notNull(vct)) {
        painter_->apply(*vct,rect,dest);
        lock_.unlock();
        return true;
      }
    } break;
    default:
      lock_.unlock();
      return false;
    }
    lock_.unlock();          
    return false;
  }

  bool viewer1D::getFormattedData(image& dest) {
    lock_.lock();
    if (isNull(painter_)) {
      painter_=newDefaultPainter();
    }
    
    painter_->useParameters(getRWParameters());

    switch (getInformation().type) {
    case Fixed: {
      const ivector* vct = getDataAsIntegers();
      if (notNull(vct)) {
        painter_->apply(*vct,dest);
        lock_.unlock();
        return true;
      }
    } break;
    case Float: {
      const dvector* vct = getDataAsDoubles();
      if (notNull(vct)) {
        painter_->apply(*vct,dest);
        lock_.unlock();
        return true;
      }
    } break;
    default:
      lock_.unlock();
      return false;
    }
    lock_.unlock();          
    return false;
  }

  // -------------------------------------------------------------------------
  // Information block
  // -------------------------------------------------------------------------

  viewer1D::information::information() : viewerBase::information() {
    type = Unknown;
    size = 0;
    min  = max = average = stdDeviation = 0.0;
  }

  viewer1D::information::information(const information& other) 
    : viewerBase::information() {
    copy(other);
  }

  viewer1D::information::~information() {
  }

  viewer1D::information& viewer1D::information::copy(const information& other){
    type = other.type;
    size = other.size;
    min  = other.min;
    max  = other.max;
    average = other.average ;
    stdDeviation = other.stdDeviation;
  
    return *this;
  }

  viewer1D::information* viewer1D::information::clone() const {
    return new information(*this);
  }

  // -------------------------------------------------------------------------
  // Main window
  // -------------------------------------------------------------------------

  viewer1D::mainWindow::mainWindow(viewer1D& parent) : 
    viewerBase::mainWindow(static_cast<viewerBase&>(parent)) {
  }

  const viewer1D::information& viewer1D::mainWindow::getInformation() const {
    const information* info =
      dynamic_cast<const information*>(&viewerBase::
                                       mainWindow::getInformation());
    assert(notNull(info));
    return *info;
  }

  bool viewer1D::mainWindow::getFormattedData(const irectangle& area,
                                              image& img) {
    viewer1D* vwr = dynamic_cast<viewer1D*>(parent_);

    return vwr->getFormattedData(area,img);
  }

  bool viewer1D::mainWindow::getFormattedData(image& img) {
    viewer1D* vwr = dynamic_cast<viewer1D*>(parent_);

    return vwr->getFormattedData(img);
  }

  ipoint viewer1D::mainWindow::computeCanvasSize() const {
    const viewer1D::parameters* par = 
      dynamic_cast<const viewer1D::parameters*>(params_);
    assert(notNull(par));
    
    const fpoint& zoom = par->zoom;
    const int vctSize = getInformation().size;
    
    return ipoint(static_cast<int>(ceilf(zoom.x*par->pixelsPerElement*vctSize)),
                  static_cast<int>(ceilf(zoom.y*par->vectorHeight)));
  }
#ifndef HAVE_GTK
  bool viewer1D::mainWindow::saveButtonPressed(const std::string& /*filename*/) {
    return false;
  }

  bool viewer1D::mainWindow::saveAllButtonPressed(const std::string& /*filename*/) {
    return false;
  }

  void viewer1D::mainWindow::initToolkit() {
  }

  void viewer1D::mainWindow::destroyToolkit() {
  }

  #endif
  // -------------------------------------------------------------------------
  // Config dialog
  // -------------------------------------------------------------------------

  viewer1D::configDialog::configDialog(viewer1D& parent) 
    : viewerBase::configDialog(parent) {
  }

  viewer1D::configDialog::configDialog(const configDialog& other,
                                       viewerBase& parent) 
    : viewerBase::configDialog(other,parent) {
    copy(other);
  }

  viewer1D::configDialog::~configDialog() {
  }

  viewer1D::configDialog* 
  viewer1D::configDialog::clone(viewerBase& parent) const {
    return (new configDialog(*this,parent));
  }

  viewer1D::configDialog& 
  viewer1D::configDialog::copy(const configDialog& other) {
    viewerBase::configDialog::copy(other);

    // TODO: Check this out:  does the toolkit need to be copied?
    return *this;
  }

  const viewer1D::information& viewer1D::configDialog::getInformation() const {
    const viewer1D::information* ptr = 
      dynamic_cast<const viewer1D::information*>
      (&viewerBase::configDialog::getInformation());
    assert(notNull(ptr));
    return *ptr;
  }

  bool viewer1D::configDialog::saveButtonPressed(const std::string& filename) {
    if (notNull(parent_)) {
      viewer1D* ptr = dynamic_cast<viewer1D*>(parent_);
      assert(notNull(ptr));
      return ptr->saveButtonPressed(filename);
    }
    return false;
  }

  bool 
  viewer1D::configDialog::saveAllButtonPressed(const std::string& filename) {
    if (notNull(parent_)) {
      viewer1D* ptr = dynamic_cast<viewer1D*>(parent_);
      assert(notNull(ptr));
      return ptr->saveAllButtonPressed(filename);
    }
    return false;
  }

#ifndef HAVE_GTK
  bool viewer1D::configDialog::useInformation(const viewerBase::information* /*dataInfo*/) {
    return false;
  }

  bool viewer1D::configDialog::build() {
    return false;
  }

  bool viewer1D::configDialog::show() {
    return false;
  }

  bool viewer1D::configDialog::hide() {
    return false;
  }

  bool viewer1D::configDialog::destroy() {
    return false;
  }

  void viewer1D::configDialog::initToolkit() {
  }

  void viewer1D::configDialog::destroyToolkit() {
  }

  bool viewer1D::configDialog::setParameters(const parameters& /*params*/) {
    return false;
  }
#endif

}
