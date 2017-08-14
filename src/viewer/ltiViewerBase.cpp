/*
 * Copyright (C) 1998, 1999, 2000, 2001
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
 * \file   ltiViewerBase.cpp
 * \author Pablo Alvarado
 * \date   11.11.2001
 * 
 * $Id: ltiViewerBase.cpp,v 1.17 2012/09/16 05:05:59 alvarado Exp $
 */

#include "ltiViewerBase.h"
#include "ltiException.h"
#include "ltiMergeRGBToImage.h"
#include "ltiDraw.h"
#include "ltiRGBPixel.h"

#include <string>
#include <cstring>
#include <cstdio>

// ---------------------------------------------------------------------------
// Some Debuggin Macros

#undef _LTI_DEBUG
// #define _LTI_DEBUG 4
#include "ltiDebug.h"

// end of debugging macro definitions
// ---------------------------------------------------------------------------

namespace lti {

  // --------------------------------------------------
  // viewerBase::interaction
  // --------------------------------------------------

  viewerBase::interaction::interaction(const viewerBase::eInteractionType a,
                                       const int k,
                                       const int m) 
    : action(a),key(k),modifiers(m) {
  }

  viewerBase::interaction::interaction() 
    : action(viewerBase::ButtonPressed),
      key(viewerBase::RightButton),
      modifiers(0) {
  }

  // --------------------------------------------------
  // viewerBase::parameters
  // --------------------------------------------------

  // default constructor
  viewerBase::parameters::parameters() : parametersManager::parameters() {
    title = "Unknown";
    actionForConfig=interaction(ButtonPressed,
                                RightButton,
                                NoModifier);
    position = ipoint(0,0);
    size = ipoint(-1,-1);
  }

  // copy constructor
  viewerBase::parameters::parameters(const parameters& other)
    : parametersManager::parameters()  {
    copy(other);
  }

  // destructor
  viewerBase::parameters::~parameters() {
  }

  // copy method
  viewerBase::parameters&
  viewerBase::parameters::copy(const parameters& other) {
    title = other.title;
    actionForConfig=other.actionForConfig;
    position.copy(other.position);
    size.copy(other.size);
    return (*this);
  }

  /*
   * copy data of "other" parameters
   */
  viewerBase::parameters&
  viewerBase::parameters::operator=(const parameters&) {

    std::string errorMsg;

    errorMsg  = "operator= not implemented for these parameters,";
    errorMsg += "please use copy() member instead or implement the ";
    errorMsg += "operator= properly";

    throw exception(errorMsg.c_str());
    return *this;
  }

  bool viewerBase::parameters::write(ioHandler& handler,
                                     const bool complete) const {
    bool b = true;

    if (complete) {
      b = handler.writeBegin();
    }

    b = b && lti::write(handler,"title",title);
    b = b && lti::write(handler,"actionForConfig",title);
    b = b && lti::write(handler,"position",position);
    b = b && lti::write(handler,"size",size);

    if (complete) {
      b = b && handler.writeEnd();
    }

    return b;
  }

  bool viewerBase::parameters::read(ioHandler& handler,
                                    const bool complete) {
    bool b = true;

    if (complete) {
      b = handler.readBegin();
    }

    b = b && lti::read(handler,"title",title);
    b = b && lti::read(handler,"actionForConfig",title);
    b = b && lti::read(handler,"position",position);
    b = b && lti::read(handler,"size",size);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

  // --------------------------------------------------
  // viewerBase
  // --------------------------------------------------

  // constructor
  viewerBase::viewerBase() : ioObject(),
                             status(),
                             parametersManager(),
                             mainWnd_(0),
                             ownsMainWnd_(true),
                             configDlg_(0),
                             ownsConfigDlg_(true),
                             info_(0),
                             ownsInfo_(true),
                             waitingInteraction_(false),
                             kmSem_(0),
                             modifiers_(0),
                             key_(0),
                             button_(NoButton),
                             pos_(0,0),
                             action_(Idle) {
    
  }

  // copy constructor
  viewerBase::viewerBase(const viewerBase& other) : ioObject(),
                                                    status(),
                                                    parametersManager(),
                                                    mainWnd_(0),
                                                    ownsMainWnd_(true),
                                                    configDlg_(0),
                                                    ownsConfigDlg_(true),
                                                    info_(0),
                                                    ownsInfo_(true),
                                                    waitingInteraction_(false),
                                                    kmSem_(0),
                                                    modifiers_(0),
                                                    key_(0),
                                                    button_(NoButton),
                                                    pos_(0,0),
                                                    action_(Idle) {
    copy(other);
  }

  // destructor
  viewerBase::~viewerBase() {
    if (ownsConfigDlg_) {
      delete configDlg_;
    }
    configDlg_=0;

    if (ownsMainWnd_) {
      delete mainWnd_;
    }
    mainWnd_=0;

    if (ownsInfo_) {
      delete info_;
    }
    info_=0;
  }

  // returns the name of this type
  const std::string& viewerBase::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // copy member
  viewerBase& viewerBase::copy(const viewerBase& other) {
    parametersManager::copy(other);

    // copy mainWindow
    if (ownsMainWnd_) {
      delete mainWnd_; // remove only if owned
    }
    mainWnd_=0;
    if (notNull(other.mainWnd_)) {
      mainWnd_=other.mainWnd_->clone(*this);
    }
    ownsMainWnd_=true;

    // copy configuration dialog
    if (ownsConfigDlg_) {
      delete configDlg_;
    }
    configDlg_=0;
    if (notNull(other.configDlg_)) {
      configDlg_=other.configDlg_->clone(*this);
    }
    ownsConfigDlg_=true;

    // copy information block
    if (ownsInfo_) {
      delete info_;
    }
    info_=0;
    
    if (notNull(other.info_)) {
      info_=other.info_->clone();
    }
    ownsInfo_=true;

    return (*this);
  }

  // alias for copy
  viewerBase& viewerBase::operator=(const viewerBase&) {
    std::string str;
    str = "operator= not defined for class ";
    str += name();
    str += ". Please use copy member instead, or add the operator=().";
    throw exception(str.c_str());
    return *this;
  }

  bool viewerBase::hide() {
    if (notNull(configDlg_)) {
      configDlg_->hide();
    }
    if (notNull(mainWnd_)) {
      return mainWnd_->hide();
    }
    return false;
  }

  bool viewerBase::updateParameters() {
    if (notNull(mainWnd_)) {
      return mainWnd_->updateParameters(getRWParameters());
    }
    // if the mainWindow was still NULL, the updating the parameters is just
    // equivalent to setting them, so we are ready!
    return parametersManager::updateParameters();
  }

  //
  // Get parameters
  //
  const viewerBase::parameters& viewerBase::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&parametersManager::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }
  
  viewerBase::parameters& viewerBase::getRWParameters() {
    parameters* par =
      dynamic_cast<parameters*>(&parametersManager::getRWParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }

  // Change the window title
  bool viewerBase::setTitle(const std::string& title) {
    parameters* par =
      dynamic_cast<parameters*>(&parametersManager::getRWParameters());
    if (notNull(par)) {
      par->title = title;
      return updateParameters();
    }
    return false;
  }

  bool viewerBase::waitInteraction(interaction& action,
                                   ipoint& position,
                                   const bool onlyIfValid) const {
    bool valid=true;
    lock_.lock();
    waitingInteraction_=true;
    lock_.unlock();
    do {
      kmSem_.wait();
      
      _lti_debug4("wi::param.size=" << getParameters().size << 
                  "  wi::position=" << pos_ << std::endl);

      lock_.lock();
      action.action=action_;
      action.key=static_cast<int>((action.action<=KeyReleased) ?
        key_ : button_);
      action.modifiers =modifiers_;
      position=pos_;

      if (onlyIfValid && validParameters() && (action!=Closed)) {
        
        valid = validPosition(position);
        if (!valid) {
          // only true may be set, since false could make that some other
          // thread waits infinite time for an interaction that won't be
          // reported.
          waitingInteraction_=true;
        }
      } else {
        valid=true;
      }

      lock_.unlock();
    } while(!valid);
    
    return (action.action!=Closed);
  }


  bool viewerBase::waitButtonPressed(interaction& action,
                                     ipoint& position,
                                     const bool onlyIfValid) const {
    bool valid=true;
    lock_.lock();
    waitingInteraction_=true;
    lock_.unlock();
    do {
      kmSem_.wait();
      
      lock_.lock();
      action.action=action_;
      action.key=(static_cast<int>(action.action<=KeyReleased)) ?
        key_ : button_;
      action.modifiers =modifiers_;
      position=pos_;

      if (action.action == ButtonPressed) {
        if (onlyIfValid && validParameters()) {
          const parameters& par = getParameters();
          valid = ((static_cast<uint32>(position.x) <
                    static_cast<uint32>(par.size.x)) &&
                   (static_cast<uint32>(position.y) <
                    static_cast<uint32>(par.size.y)));
          if (!valid) {
            // only true may be set, since false could make that some other
            // thread waits infinite time for an interaction that won't be
            // reported.
            waitingInteraction_=true;
          }
        } else {
          valid=true;
        }
      } else if (action.action == Closed) {
        valid = true;
      } else {
        valid = false;
        waitingInteraction_=true;
      }
      lock_.unlock();
    } while(!valid);
    
    return (action.action!=Closed);
  }
  
  bool viewerBase::waitButtonReleased(interaction& action,
                                      ipoint& position,
                                      const bool onlyIfValid) const {
    bool valid=true;
    lock_.lock();
    waitingInteraction_=true;
    lock_.unlock();
    do {
      kmSem_.wait();
      
      lock_.lock();
      action.action=action_;
      action.key=(static_cast<int>(action.action<=KeyReleased)) ?
        key_ : button_;
      action.modifiers =modifiers_;
      position=pos_;

      if (action.action == ButtonReleased) {
        if (onlyIfValid && validParameters()) {
          const parameters& par = getParameters();          
          valid = ((static_cast<uint32>(position.x) <
                    static_cast<uint32>(par.size.x)) &&
                   (static_cast<uint32>(position.y) <
                    static_cast<uint32>(par.size.y)));
          if (!valid) {
            // only true may be set, since false could make that some other
            // thread waits infinite time for an interaction that won't be
            // reported.
            waitingInteraction_=true;
          }
        } else {
          valid=true;
        }
      } else if (action.action == Closed) {
        valid = true;
      } else {
        valid = false;
        waitingInteraction_=true;
      }
      lock_.unlock();
    } while(!valid);
    
    return (action.action!=Closed);
  }
  
  bool viewerBase::waitKey(interaction& action) {
    bool valid=true;
    lock_.lock();
    waitingInteraction_=true;
    lock_.unlock();
    do {
      kmSem_.wait();
      
      lock_.lock();
      action.action=action_;
      action.key=(static_cast<int>(action.action<=KeyReleased)) ?
        key_ : button_;
      action.modifiers =modifiers_;

      if ((action.action==KeyPressed) ||
          (action.action==KeyReleased)||
          (action.action==Closed)) {
        valid=true;
      } else {
        valid = false;
        waitingInteraction_=true;
      }
      lock_.unlock();
    } while(!valid);
    
    return (action.action!=Closed);
  }

  bool viewerBase::waitKeyPressed() {
    interaction action;
    return waitKeyPressed(action);
  }
  bool viewerBase::waitKeyPressed(interaction& action) {
    bool valid=true;
    lock_.lock();
    waitingInteraction_=true;
    lock_.unlock();
    do {
      kmSem_.wait();
      
      lock_.lock();
      action.action=action_;
      action.key=(static_cast<int>(action.action<=KeyReleased)) ?
        key_ : button_;
      action.modifiers =modifiers_;

      if ((action.action==KeyPressed) ||
          (action.action==Closed)) {
        valid=true;
      } else {
        valid = false;
        waitingInteraction_=true;
      }
      lock_.unlock();
    } while(!valid);
    
    return (action.action!=Closed);
  }

  bool viewerBase::waitClosed() {
    bool valid=true;
    lock_.lock();
    waitingInteraction_=true;
    lock_.unlock();

    do {
      kmSem_.wait();
      
      lock_.lock();
      if (action_==Closed) {
        valid=true;
      } else {
        valid = false;
        waitingInteraction_=true;
      }
      lock_.unlock();
    } while(!valid);
    
    return true;
  }

  
  void viewerBase::getLastAction(interaction& action,
                                 ipoint& position) const {
    lock_.lock();
    action.action=action_;
    action.key=(static_cast<int>(action.action<=KeyReleased)) ?
      key_ : button_;
    action.modifiers =modifiers_;
    position=pos_;
    lock_.unlock();
  }
    
  bool viewerBase::setKMInteraction(const interaction& action,
                                    const ipoint& position) {
    // no matter if the user waits for interaction, we have to take care that
    // the configuration dialog is opened if the user wants to
    if (validParameters() && action==getParameters().actionForConfig) {
      showConfig();
    }

    // call the virtual function so the user can override whatever he needs
    // to do, like displaying something in the status bar
    onKMInteraction(action,position);

    // update internal flags in case the user needs the non-blocking
    // "getLastAction()"
    
    if (action.action != MouseMoved) {
      lock_.lock();

      action_=action.action;
      switch (action_) {
        case KeyPressed:
        case KeyReleased:
          key_=action.key;
          modifiers_=action.modifiers;
          break;
        case ButtonPressed:
        case ButtonReleased:
        case MouseMoved:
          button_=static_cast<eMouseButton>(action.key);
          modifiers_=action.modifiers;
          pos_=position;
          break;
        default:
          break;
      }

      if (waitingInteraction_) {
        // post as many messages as threads are waiting for an event
        do {
          _lti_debug4("kmSem_=" << kmSem_.getValue()<<std::endl);
          kmSem_.post();
        } while (kmSem_.getValue()<0);
        waitingInteraction_=false;
      }
      lock_.unlock();

    } else { // in case MouseMoved:
      // mouse moved reported if and only if the user waits for it
      if (waitingInteraction_) {
        lock_.lock();
        action_=action.action;
        button_=static_cast<eMouseButton>(action.key);
        modifiers_=action.modifiers;
        pos_=position;        

        do {
          _lti_debug4("kmSem_=" << kmSem_.getValue()<<std::endl);
          kmSem_.post();
        } while (kmSem_.getValue()<0);
        waitingInteraction_=false;

        lock_.unlock();
      }
    }
    
    return true; 
  }

  bool viewerBase::validPosition(const ipoint& position) const {
    const parameters& par = getParameters();
    return ((static_cast<uint32>(position.x)<static_cast<uint32>(par.size.x))&&
            (static_cast<uint32>(position.y)<static_cast<uint32>(par.size.y)));
  }

  // this default version of onKMInteraction just displays in the statusBar
  // the position in which the mouse button has been pressed
  bool viewerBase::onKMInteraction(const interaction& action,
                                   const ipoint& position) {
    if (notNull(mainWnd_)) {
      if ((action == MouseMoved)   ||
          (action == ButtonPressed)  ) {
        char buffer[256];
        sprintf(buffer,"%4dx%4d",position.x,position.y);
        mainWnd_->statusBar(buffer);
      }
      return true;
    }
    return false;
  }

  const viewerBase::configDialog& viewerBase::getConfigDialog() const {
    assert(notNull(configDlg_));
    return *configDlg_;
  }

  viewerBase::configDialog& viewerBase::getConfigDialog() {
    assert(notNull(configDlg_));
    return *configDlg_;
  }

  bool viewerBase::useConfig(configDialog* dlg) {
    if (ownsConfigDlg_) {
      delete configDlg_;
      configDlg_=0;
    }
    if (notNull(dlg)) {
      configDlg_=dlg;
      ownsConfigDlg_=false;
      configDlg_->initToolkit();
      return true;
    }

    return false;
  }

  bool viewerBase::attachConfig(configDialog* dlg) {
    if (ownsConfigDlg_) {
      delete configDlg_;
      configDlg_=0;
    }

    if (notNull(dlg)) {
      configDlg_=dlg;
      ownsConfigDlg_=true;
      configDlg_->initToolkit();
      return true;
    }

    return false;
  }

  bool viewerBase::showConfig() {
    if (notNull(configDlg_)) {
      updateInformation();
      configDlg_->build();
      configDlg_->useInformation(info_);
      configDlg_->setParameters(getParameters());
      configDlg_->show();
      return true;
    }
    return false;
  }

  bool viewerBase::updateFromConfig(const bool keepConfigData) {
    lock_.lock();
    
    bool result = false;

    // very frequently, the user changes the position and size of the
    // main window while the configure is open, which do not nothing
    // about it!  So we just transfer the current data 
    ipoint position(getParameters().position),size(getParameters().size);
    
    
    if (keepConfigData) {
      parameters* par = &configDlg_->getParameters();
      if (notNull(par)) {
        par->size = size;
        par->position = position;
        result = setParameters(*par);
      }
    } else {
      parameters* ptr = configDlg_->detachParameters();
      if (notNull(ptr)) {
        // give memory control to the manager!
        ptr->size = size;
        ptr->position = position;
        result = attachParameters(*ptr); 
      }
    }

    lock_.unlock();
    return result;
  }

  bool viewerBase::hideConfig() {
    return false;
  }

  bool viewerBase::ensureMainWindow() {
    return true;
  }

  bool viewerBase::validMainWindow() const {
    return notNull(mainWnd_);
  }

  const viewerBase::mainWindow& viewerBase::getMainWindow() const {
    assert(notNull(mainWnd_));
    return *mainWnd_;
  }

  viewerBase::mainWindow& viewerBase::getMainWindow() {
    assert(notNull(mainWnd_));
    return *mainWnd_;    
  }

  bool viewerBase::useMainWindow(mainWindow* newWnd) {
    bool ret=true;
    if (validMainWindow() && ownsMainWnd_) {
      delete mainWnd_;
      mainWnd_=0;
      ret=false;
    }

    mainWnd_=newWnd;
    ownsMainWnd_=false;
    if (notNull(mainWnd_)) {
      mainWnd_->initToolkit();
    }

    return ret;
  }

  bool viewerBase::attachMainWindow(mainWindow* newWnd) {
    bool ret=true;
    if (validMainWindow() && ownsMainWnd_) {
      delete mainWnd_;
      mainWnd_=0;
      ret=false;
    }

    mainWnd_=newWnd;
    ownsMainWnd_=true;
    if (notNull(mainWnd_)) {
      mainWnd_->initToolkit();
    }

    return ret;
  }

  bool viewerBase::validInformation() const {
    return notNull(info_);
  }

  bool viewerBase::setInformation(const information& info) {
    if (ownsInfo_) {
      delete info_;
    }
    info_=info.clone();
    ownsInfo_=true;
    return notNull(info_);
  }
  
  
  const viewerBase::information& viewerBase::getInformation() const {
    return *info_;
  }
  
  viewerBase::information& viewerBase::getInformation() {
    return *info_;
  }

  bool viewerBase::updateInformation() {
    return validInformation();
  }

  //
  // Some global io stuff
  // 
  bool read(ioHandler& handler,viewerBase::interaction& obj, bool complete) {

    bool b = true;
    
    if (complete) {
      b = handler.readBegin();
    }
    
    std::string str;
    b = b && lti::read(handler,"action",str);
    
    if (str.find("dle") != std::string::npos) {
      obj.action=viewerBase::Idle;
    } else if (str.find("eyPr") != std::string::npos) {
      obj.action=viewerBase::KeyPressed;
    } else if (str.find("eyRe") != std::string::npos) {
      obj.action=viewerBase::KeyReleased;
    } else if (str.find("onPress") != std::string::npos) {
      obj.action=viewerBase::ButtonPressed;
    } else if (str.find("onReleas") != std::string::npos) {
      obj.action=viewerBase::ButtonReleased;
    } else if (str.find("ouse") != std::string::npos) {
      obj.action=viewerBase::MouseMoved;
    } else if (str.find("lose") != std::string::npos) {
      obj.action=viewerBase::Closed;
    } else {
      obj.action=viewerBase::Idle;
    }
               
    b = b && lti::read(handler,"key",obj.key);
    b = b && lti::read(handler,"modifiers",obj.modifiers);
    
    if (complete) {
      b = b && handler.readEnd();
    }
    return b;
  }

  bool write(ioHandler& handler,
             const viewerBase::interaction& obj, 
             bool complete) {
    bool b = true;

    if (complete) {
      b = handler.writeBegin();
    }

    switch(obj.action) {
      case viewerBase::Idle:
        lti::write(handler,"action","Idle");
        break;        
      case viewerBase::KeyPressed:
        lti::write(handler,"action","KeyPressed");
        break;
      case viewerBase::KeyReleased:
        lti::write(handler,"action","KeyReleased");
        break;
      case viewerBase::ButtonPressed:
        lti::write(handler,"action","ButtonPressed");
        break;
      case viewerBase::ButtonReleased:
        lti::write(handler,"action","ButtonReleased");
        break;
      case viewerBase::MouseMoved:
        lti::write(handler,"action","MouseMoved");
        break;
      case viewerBase::Closed:
        lti::write(handler,"action","Closed");
        break;
      default:
        lti::write(handler,"action","Unknown");
        break;
    }

    lti::write(handler,"key",obj.key);
    lti::write(handler,"modifiers",obj.modifiers);

    if (complete) {
      b = b && handler.writeEnd();
    }
    return b;
  }

  // ------------------------------------------------------------------------
  //   Information block
  // ------------------------------------------------------------------------

  viewerBase::information::information() {
  }

  viewerBase::information::information(const information& other) {
    copy(other);
  }

  viewerBase::information::~information() {
  }

  viewerBase::information& 
  viewerBase::information::copy(const information&) {
    return *this;
  }
  

  // ------------------------------------------------------------------------
  //   Configuration Dialog
  // ------------------------------------------------------------------------

  viewerBase::configDialog::configDialog(viewerBase& parent)
    : data_(0),ownsData_(true),parent_(&parent),params_(0),tk_(0) {
  }

  viewerBase::configDialog::configDialog(const configDialog& other,
                                               viewerBase& parent)
    : data_(0),ownsData_(true),parent_(&parent),params_(0),tk_(0) {
    copy(other);
  }

  viewerBase::configDialog::~configDialog() {
    destroyToolkit();
    if (ownsData_) {
      delete data_;
    }
    data_=0;
    delete params_;
    params_=0;
  }

  bool viewerBase::configDialog::useInformation(const information* dataInfo) {
    if (ownsData_) {
      delete data_;
    }
    data_=dataInfo;
    ownsData_=false;
    return (notNull(data_));
  }

  bool 
  viewerBase::configDialog::attachInformation(const information* dataInfo) {
    useInformation(dataInfo);
    ownsData_=true;
  
    return (notNull(data_));
  }

  const viewerBase::information& 
  viewerBase::configDialog::getInformation() const {
    assert(notNull(data_));
    return *data_;
  }

  bool viewerBase::configDialog::validInformation() const {
    return notNull(data_);
  }

  bool viewerBase::configDialog::setParameters(const parameters& par) {
    delete params_;
    params_=par.clone();
    return (notNull(params_));
  }

  const viewerBase::parameters& 
  viewerBase::configDialog::getParameters() const {
    assert(notNull(params_));
    return *params_;
  }

  viewerBase::parameters& viewerBase::configDialog::getParameters() {
    assert(notNull(params_));
    return *params_;
  }

  viewerBase::parameters* viewerBase::configDialog::detachParameters() {
    parameters* ptr = params_;
    params_=0;
    return ptr;
  }

  viewerBase::configDialog&
  viewerBase::configDialog::copy(const configDialog& other) {
    if (notNull(other.data_)) {
      attachInformation(other.data_->clone());
    } else {
      attachInformation(0);
    }

    if (notNull(other.params_)) {
      setParameters(*other.params_);
    } else {
      delete params_;
      params_=0;
    }

    return *this;
  }

  viewerBase::configDialog* 
  viewerBase::configDialog::clone(viewerBase& parent) const {
    return new configDialog(*this,parent);
  }

  bool viewerBase::configDialog::onCancelButtonClicked() {
    return hide();
  }
  
  bool viewerBase::configDialog::onApplyButtonClicked() {
    if (notNull(parent_)) {
      return parent_->updateFromConfig(true); // just apply = true
    }
    return false;
  }
  
  bool viewerBase::configDialog::onOkButtonClicked() {
    hide();
    if (notNull(parent_)) {
      // false => can detach parameters
      return parent_->updateFromConfig(false); 
    }
    return false;
  }

#ifndef HAVE_GTK  
  bool viewerBase::configDialog::build() {
    return false;
  }

  bool viewerBase::configDialog::show() {
    return false;
  }

  bool viewerBase::configDialog::hide() {
    return false;
  }

  bool viewerBase::configDialog::destroy() {
    return false;
  }

  void viewerBase::configDialog::initToolkit() {
  }

  void viewerBase::configDialog::destroyToolkit() {
  }
#endif

  // ------------------------------------------------------------------------
  //   Main Window
  // ------------------------------------------------------------------------

  viewerBase::mainWindow::mainWindow(viewerBase& parent) :
    tk_(0),parent_(&parent),params_(0),canvasSize_(256,256) {
  }

  viewerBase::mainWindow::mainWindow(const mainWindow&,
                                           viewerBase& parent) :
    tk_(0),parent_(&parent),params_(0) {
  }

  viewerBase::mainWindow::~mainWindow() {
    hide();
    destroy();
    parent_=0;
    destroyToolkit();
  }

  viewerBase::mainWindow* 
  viewerBase::mainWindow::clone(viewerBase& parent) const {
    return new mainWindow(*this,parent);
  }

  const viewerBase::information& 
  viewerBase::mainWindow::getInformation() const {
    const information* info = &parent_->getInformation();
    assert(notNull(info));
    return *info;
  }

  const ipoint& viewerBase::mainWindow::getCanvasSize() const {
    return canvasSize_;
  }

  ipoint viewerBase::mainWindow::computeCanvasSize() const {
    return canvasSize_; // nothing changed is the default behaviour
  }

  bool viewerBase::mainWindow::closeWindow() {
    // indicate an event in case someone is waiting for interaction    
    onKMInteraction(interaction(Closed,0,0),ipoint(0,0));

    if (notNull(parent_)) {
      return parent_->hide();
    }
    return false;
  }

#ifndef HAVE_GTK
  bool viewerBase::mainWindow::build() {
    return false;
  }

  bool viewerBase::mainWindow::show() {
    return false;
  }

  bool viewerBase::mainWindow::hide() {
    return false;
  }

  bool viewerBase::mainWindow::destroy() {
    return false;
  }

  bool viewerBase::mainWindow::statusBar(const std::string& /*txt*/) {
    return false;
  }

  bool viewerBase::mainWindow::setCanvasSize(const ipoint& /*newSize*/) {
    return false;
  }

  bool viewerBase::mainWindow::updateParameters(parameters& /*params*/) {
    return false;
  }

  void viewerBase::mainWindow::initToolkit() {
  }

  void viewerBase::mainWindow::destroyToolkit() {
  }
#endif

  const viewerBase::parameters& viewerBase::mainWindow::getParameters() const {
    assert(notNull(params_));
    return *params_;
  }

  bool viewerBase::mainWindow::onKMInteraction(const interaction& action,
                                               const ipoint& position) {
    if (notNull(parent_)) {
      return parent_->setKMInteraction(action,position);
    }

    return false;
  }


}
