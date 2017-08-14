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
 * \file   ltiViewer2D.cpp
 *         Contains the viewer for 2D data
 * \author Pablo Alvarado
 * \date   05.01.2005
 *
 * revisions ..: $Id: ltiViewer2D.cpp,v 1.11 2013/05/19 02:22:08 alvarado Exp $
 */

#include <cstdio>

#include "ltiViewer2D.h"
#include "ltiViewer2DPainter.h"

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

  viewer2D::parameters::parameters() :
    parametersManager::parameters(), 
    viewerBase::parameters(),
    viewer2DPainter::parameters() {
    filename = "tux.png";
  }

  viewer2D::parameters::parameters(const parameters& other) :
    parametersManager::parameters(),
    viewerBase::parameters(),
    viewer2DPainter::parameters() {
    copy(other);
  }

  viewer2D::parameters::~parameters() {
  }
  
  viewer2D::parameters& viewer2D::parameters::copy(const parameters& other) {
    viewer2DPainter::parameters::copy(other);
    viewerBase::parameters::copy(other);

    filename = other.filename;
    return *this;
  }
  
  viewer2D::parameters& 
  viewer2D::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  const std::string& viewer2D::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }
  
  viewer2D::parameters* viewer2D::parameters::clone() const {
    return new viewer2D::parameters(*this);
  }
  
  viewer2D::parameters* viewer2D::parameters::newInstance() const {
    return new viewer2D::parameters();
  }
  
  bool viewer2D::parameters::write(ioHandler& handler,
                                   const bool complete) const {
    bool b = true;
    if (complete) {
      b=handler.writeBegin();
    }

    b = b && viewer2DPainter::parameters::write(handler,false);
    b = b && viewerBase::parameters::write(handler,false);
    b = b && lti::write(handler,"filename",filename);

    if (complete) {
      b=b && handler.writeEnd();
    }

    return b;
  }

  bool viewer2D::parameters::read(ioHandler& handler,
                                  const bool complete) {
    bool b = true;
    if (complete) {
      b=handler.readBegin();
    }

    b = b && viewer2DPainter::parameters::read(handler,false);
    b = b && viewerBase::parameters::read(handler,false);
    b = b && lti::read(handler,"filename",filename);

    if (complete) {
      b=b && handler.readEnd();
    }

    return b;
  }

  // -------------------------------------------------------------------------
  // viewer2D
  // -------------------------------------------------------------------------

  // some inline stuff

  inline const matrix<rgbaPixel>* viewer2D::getDataAsImage() const {
    return dynamic_cast<const matrix<rgbaPixel>*>(data_);
  }

  inline const matrix<float>* viewer2D::getDataAsChannel() const {
    return dynamic_cast<const matrix<float>*>(data_);
  }
  
  inline const matrix<ubyte>* viewer2D::getDataAsChannel8() const {
    return dynamic_cast<const matrix<ubyte>*>(data_);
  }
  
  inline const matrix<int32>* viewer2D::getDataAsChannel32() const {
    return dynamic_cast<const matrix<int32>*>(data_);
  }
  

  viewer2D::viewer2D(const std::string& title,
                     const viewer2DPainter::eMappingType mapping) 
    : viewerBase(),data_(0),painter_(0) {
    ensureMainWindow();
    attachConfig(new configDialog(*this)); // DEBUG
    parameters defPar;
    defPar.title=title;
    defPar.mappingType = mapping;
    setParameters(defPar);    
  }

  viewer2D::viewer2D(const viewer2D& other) 
    : viewerBase(),data_(0),painter_(0) {
    ensureMainWindow();
    attachConfig(new configDialog(*this)); // DEBUG
    setParameters(other.getParameters());    
  }

  viewer2D::viewer2D(const parameters& params) 
    : viewerBase(),data_(0),painter_(0) {
    ensureMainWindow();
    attachConfig(new configDialog(*this)); // DEBUG
    setParameters(params);    
  }

  viewer2D::~viewer2D() {
    lock_.lock();
    delete painter_;
    delete data_;
    data_=0;
    lock_.unlock();
  }

  viewer2D* viewer2D::clone() const {
    return new viewer2D(*this);
  }

  viewer2D* viewer2D::newInstance() const {
    return new viewer2D();
  }

  viewer2D& viewer2D::copy(const viewer2D& other) {
    viewerBase::copy(other);
    lock_.lock();
    if (notNull(other.data_)) {
      data_=other.data_->clone();
    }
    lock_.unlock();
    return *this;
  }

  const viewer2D::parameters& viewer2D::getParameters() const {
     const parameters* par =
      dynamic_cast<const parameters*>(&viewerBase::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }

  bool viewer2D::show(const matrix<rgbaPixel>& img) {
    lock_.lock();
    ensureInformationType(Image,img.size());
    delete data_;
    data_=img.clone();
    if (notNull(painter_)) {
      painter_->invalidateCache();
    }
    
    lock_.unlock();
    return getMainWindow().show();
  }

  bool viewer2D::show(const matrix<float>& chnl) {
    lock_.lock();
    ensureInformationType(Channel,chnl.size());
    delete data_;
    data_=chnl.clone();
    if (notNull(painter_)) {
      painter_->invalidateCache();
    }

    lock_.unlock();

    return getMainWindow().show();
  }

  bool viewer2D::show(const matrix<int32>& mask) {
    lock_.lock();
    ensureInformationType(Channel32,mask.size());
    delete data_;
    data_=mask.clone();
    if (notNull(painter_)) {
      painter_->invalidateCache();
    }

    lock_.unlock();

    return getMainWindow().show();
  }

  bool viewer2D::show(const matrix<ubyte>& chnl8) {

    lock_.lock();
    ensureInformationType(Channel8,chnl8.size());
    delete data_;
    data_=chnl8.clone();
    if (notNull(painter_)) {
      painter_->invalidateCache();
    }
    lock_.unlock();

    return getMainWindow().show();
  }

  bool viewer2D::ensureMainWindow() {
    if (!validMainWindow()) {
      attachMainWindow(new mainWindow(*this));
      return getMainWindow().build();
    }
    return false;
  }

  bool viewer2D::setPainter(const viewer2DPainter& painter) {
    lock_.lock();
    delete painter_;
    painter_ = painter.clone();
    lock_.unlock();
    return (notNull(painter_));
  }

  bool viewer2D::saveButtonPressed(const std::string& filename) {
    if (validMainWindow()) {
      return getMainWindow().saveButtonPressed(filename);
    }
    return false;
  }

  viewer2DPainter* viewer2D::newDefaultPainter() const {
    return new viewer2DPainter;
  }
  
  const viewer2D::information& viewer2D::getInformation() const {
    const information* info =
      dynamic_cast<const information*>(&viewerBase::getInformation());
    if (isNull(info)) {
      throw exception("Empty or wrong information block");
    }
    return *info;
  }

  viewer2D::information& viewer2D::getInformation() {
    information* info =
      dynamic_cast<information*>(&viewerBase::getInformation());
    if (isNull(info)) {
      throw exception("Empty or wrong information block");
    }
    return *info;
  }

  bool viewer2D::ensureInformationType(const eDataType& type,
                                       const ipoint& matrixSize) {
    if (!validInformation()) {
      information info;
      info.type = type;
      info.size = matrixSize;
      setInformation(info);
      return true; // indicate that something changed
    }
    
    information& info = getInformation();

    info.size = matrixSize;
    if (type == info.type) {
      return false; // nothing has changed
    }

    info.type = type;
    return true; // something changed.
  }

  bool viewer2D::updateInformation() {
    lock_.lock();
    if (viewerBase::updateInformation()) {
      information& i = getInformation();
      // since "ensureInformationType" is called in every show, we can
      // assume that the type of data_ is correctly known for the cast
      switch(i.type) {
        case Image: {
          const matrix<rgbaPixel>* img =getDataAsImage();
          if (notNull(img)) {
            i.compute(*img);
          }
        } break;
        case Channel: {
          const matrix<float>* chn = getDataAsChannel();
          if (notNull(chn)) {
            i.compute(*chn);
          }
        } break;
        case Channel8: {
          const matrix<ubyte>* chn = getDataAsChannel8();
          if (notNull(chn)) {
            i.compute(*chn);
          }
        } break;
        case Channel32: {
          const matrix<int32>* chn = getDataAsChannel32();
          if (notNull(chn)) {
            i.compute(*chn);
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

  const viewer2D::mainWindow& viewer2D::getMainWindow() const {
    const mainWindow* wnd =
      dynamic_cast<const mainWindow*>(&viewerBase::getMainWindow());
    if (isNull(wnd)) {
      throw exception("Empty or wrong main window");
    }
    return *wnd;
  }

  viewer2D::mainWindow& viewer2D::getMainWindow() {
    mainWindow* wnd = dynamic_cast<mainWindow*>(&viewerBase::getMainWindow());
    if (isNull(wnd)) {
      throw exception("Empty or wrong main window");
    }
    return *wnd;
  }

  const viewer2D::configDialog& viewer2D::getConfigDialog() const {
    const viewerBase::configDialog* ptr = &viewerBase::getConfigDialog();
    return *dynamic_cast<const viewer2D::configDialog*>(ptr);
  }

  viewer2D::configDialog& viewer2D::getConfigDialog() {
    viewerBase::configDialog* ptr = &viewerBase::getConfigDialog();
    return *dynamic_cast<viewer2D::configDialog*>(ptr);
  }

  bool viewer2D::validPosition(const ipoint& position) const {
    const information& info = getInformation();
    const parameters& par   = getParameters();
    
    const ipoint p(static_cast<int>(position.x/par.zoom),
                   static_cast<int>(position.y/par.zoom));

    return ((p.x<info.size.x) && (p.y<info.size.y) && (p.x>=0) && (p.y>=0));
  }

  // this overloaded version of onKMInteraction takes care of the display of
  // pixel coordinates and value
  bool viewer2D::onKMInteraction(const interaction& action,
                                 const ipoint& position) {
    lock_.lock(); // it can happen that while we are here, the data changes!
    if (validMainWindow() && notNull(data_)) {

      if ((action == MouseMoved)   ||
          (action == ButtonPressed)  ) {

        const information& info = getInformation();
        const parameters& par   = getParameters();
        // truncate the integer values
        const ipoint p(static_cast<int>(position.x/par.zoom),
                       static_cast<int>(position.y/par.zoom));

        char buffer[256];
        std::string str;
        sprintf(buffer,"at(%d,%d) = ",p.x,p.y);
        str = buffer;

        if ((p.x<0) || (p.x>=info.size.x) ||
            (p.y<0) || (p.y>=info.size.y)) {
          str += "undefined";
        } else {
          switch (info.type) {
            case Image: {
              const matrix<rgbaPixel>* img = getDataAsImage();
              const rgbaPixel& px = img->at(p);
              if (par.hexadecimal) {
                sprintf(buffer,"(%02X, %02X, %02X)",px.red,px.green,px.blue);
                str += buffer;
              } else {
                sprintf(buffer,"(%03d, %03d, %03d)",px.red,px.green,px.blue);
                str += buffer;
              }
            } break;
            
            case Channel32: {
              const matrix<int32>* chn = getDataAsChannel32();
              const int px = chn->at(p);
              if (par.hexadecimal) {
                sprintf(buffer,"%X",px);
                str += buffer;
              } else {
                sprintf(buffer,"%d",px);
                str += buffer;
              }
            } break;
            
            case Channel8: {
              const matrix<ubyte>* chn = getDataAsChannel8();
              const ubyte px = chn->at(p);
              if (par.paletteType == viewer2DPainter::HuePalette) {
                if (par.hexadecimal) {
                  sprintf(buffer,"%X (%f deg)",px,360.0f*px/255.0f);
                  str += buffer;
                } else {
                  sprintf(buffer,"%d (%f deg)",px,360.0f*px/255.0f);
                  str += buffer;
                }                
              } else {
                if (par.hexadecimal) {
                  sprintf(buffer,"%X",px);
                  str += buffer;
                } else {
                  sprintf(buffer,"%d",px);
                  str += buffer;
                }
              }
            } break;
            
            case Channel: {
              const matrix<float>* chn = getDataAsChannel();
              const float px = chn->at(p);
              if (par.paletteType == viewer2DPainter::HuePalette) {
                sprintf(buffer,"%f (%f deg)",px,180.0f*px/Pi);
                str += buffer;
              } else {
                sprintf(buffer,"%f",px);
                str += buffer;
              }
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

  bool viewer2D::getFormattedData(const irectangle& rect,
                                        image& dest) {
    lock_.lock();
    if (isNull(painter_)) {
      painter_=newDefaultPainter();
    }

    painter_->useParameters(getRWParameters());

    switch (getInformation().type) {
      case Image: {
        const matrix<rgbaPixel>* img = getDataAsImage();
        if (notNull(img)) {
          painter_->apply(*img,rect,dest);
          lock_.unlock();
          return true;
        }
      } break;
      case Channel: {
        const matrix<float>* chnl = getDataAsChannel();
        if (notNull(chnl)) {
          painter_->apply(*chnl,rect,dest);
          lock_.unlock();
          return true;
        }
      } break;
      case Channel8: {
        const matrix<ubyte>* chnl = getDataAsChannel8();
        if (notNull(chnl)) {
          painter_->apply(*chnl,rect,dest);
          lock_.unlock();
          return true;
        }
      } break;
      case Channel32: {
        const matrix<int32>* chnl = getDataAsChannel32();
        if (notNull(chnl)) {
          painter_->apply(*chnl,rect,dest);
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

  viewer2D::information::information() : viewerBase::information() {
    type = Unknown;
    size.set(0,0);
    minI = maxI = averageI = stdDeviation = 0;
    minRGB.set(0,0,0);
    maxRGB.set(0,0,0);
    averageRGB.set(0,0,0);
    covarianceRGB.resize(3,3,0.0f,Init);
  }

  viewer2D::information::information(const information& other) 
    : viewerBase::information() {
    copy(other);
  }

  viewer2D::information::~information() {
  }

  viewer2D::information& viewer2D::information::copy(const information& other){
    type=other.type;
    size.copy(other.size);
    minI = other.minI;
    maxI = other.maxI;
    averageI = other.averageI;
    stdDeviation = other.stdDeviation;
    minRGB.copy(minRGB);
    maxRGB.copy(maxRGB);
    averageRGB.copy(averageRGB);
    covarianceRGB.copy(covarianceRGB);
  
    return *this;
  }

  viewer2D::information* viewer2D::information::clone() const {
    return new information(*this);
  }

  template <typename T>
  void viewer2D::information::computeScalars(const matrix<T>& mat) {
    if (mat.empty()) {
      size.set(0,0);
      minI = maxI = averageI = stdDeviation = 0;
      minRGB.set(0,0,0);
      maxRGB.set(0,0,0);
      averageRGB.set(0,0,0);
      covarianceRGB.resize(3,3,0.0f,Init);
    } else {
      size=mat.size();
      typename matrix<T>::const_iterator it,eit;
      const int n = mat.columns()*mat.rows();
      double sum(0.0);
      double sum2(0.0);
      minI=maxI=static_cast<float>(mat.at(0,0));
      for (it=mat.begin(),eit=mat.end();it!=eit;++it) {
        if ((*it) < minI) {
          minI = static_cast<float>((*it));
        } else if ((*it) > maxI) {
          maxI = static_cast<float>((*it));
        }
        sum +=(*it);
        sum2+=(*it)*(*it);
      }
      averageI = static_cast<float>(sum/n);
      stdDeviation = static_cast<float>((sum2/n)-(averageI*averageI));

      minRGB.set(0,0,0);
      maxRGB.set(0,0,0);

      averageRGB.set(averageI,averageI,averageI);
      covarianceRGB.resize(3,3,stdDeviation,Init);
      stdDeviation=sqrt(stdDeviation);
    }
  }
  
  void viewer2D::information::compute(const matrix<rgbaPixel>& mat) {
    type=Image;
    if (mat.empty()) {
      size.set(0,0);
      minI = maxI = averageI = stdDeviation = 0;
      minRGB.set(0,0,0);
      maxRGB.set(0,0,0);
      averageRGB.set(0,0,0);
      covarianceRGB.resize(3,3,0.0f,Init);
    } else {
      size=mat.size();
      matrix<rgbaPixel>::const_iterator it,eit;
      const int n = mat.columns()*mat.rows();
      frgbPixel sum(0.f, 0.f, 0.f);
      matrix<double> tmp(3,3,0.0);
      matrix<double> sum2(3,3,0.0);

      float sumI(0.0f);
      double sumI2(0.0);
      float I(0.0f);
      I=(mat.at(0,0).red + mat.at(0,0).green + mat.at(0,0).blue)/3.0f;
      minI=maxI=I;

      minRGB=maxRGB=mat.at(0,0);

      for (it=mat.begin(),eit=mat.end();it!=eit;++it) {
        const rgbaPixel& p = (*it);
        I=(p.red+p.green+p.blue)/3.0f;
        if (I < minI) {
          minI = I;
        } else if (I > maxI) {
          maxI = I;
        }

        if (p.red<minRGB.red) {
          minRGB.red=p.red;
        } else if (p.red>maxRGB.red) {
          maxRGB.red=p.red;
        }

        if (p.green<minRGB.green) {
          minRGB.green=p.green;
        } else if (p.green>maxRGB.green) {
          maxRGB.green=p.green;
        }

        if (p.blue<minRGB.blue) {
          minRGB.blue=p.blue;
        } else if (p.blue>maxRGB.blue) {
          maxRGB.blue=p.blue;
        }

        sum.add(p);

        // compute outer product
        tmp.at(0,0)            =sqr(static_cast<double>(p.red));
        tmp.at(0,1)=tmp.at(1,0)=p.red*p.green;
        tmp.at(0,2)=tmp.at(2,0)=p.red*p.blue;
        tmp.at(1,1)            =sqr(static_cast<double>(p.green));
        tmp.at(1,2)=tmp.at(2,1)=p.green*p.blue;
        tmp.at(2,2)            =sqr(static_cast<double>(p.blue));

        sum2.add(tmp);

        sumI+=I;
        sumI2+=(I*I);

      }
      averageI = sumI/n;
      stdDeviation = sqrt(static_cast<float>((sumI2/n)-(averageI*averageI)));

      averageRGB.copy(sum);
      averageRGB.divide(n);

      tmp.at(0,0)            =sqr(averageRGB.red);
      tmp.at(0,1)=tmp.at(1,0)=averageRGB.red*averageRGB.green;
      tmp.at(0,2)=tmp.at(2,0)=averageRGB.red*averageRGB.blue;
      tmp.at(1,1)            =sqr(averageRGB.green);
      tmp.at(1,2)=tmp.at(2,1)=averageRGB.green*averageRGB.blue;
      tmp.at(2,2)            =sqr(averageRGB.blue);

      sum2.divide(n);
      sum2.subtract(tmp);
      covarianceRGB.castFrom(sum2);
    }

  }
  
  void viewer2D::information::compute(const matrix<ubyte>& mat) {
    computeScalars(mat);
    type=Channel8;
  }

  void viewer2D::information::compute(const matrix<float>& mat) {
    computeScalars(mat);
    type=Channel;
  }

  void viewer2D::information::compute(const matrix<int32>& mat) {
    computeScalars(mat);
    type=Channel32;
  }
  

  // -------------------------------------------------------------------------
  // Main window
  // -------------------------------------------------------------------------

  viewer2D::mainWindow::mainWindow(viewer2D& parent) :
    viewerBase::mainWindow(static_cast<viewerBase&>(parent)) {
  }

  const viewer2D::information& viewer2D::mainWindow::getInformation() const {
    const information* info =
      dynamic_cast<const information*>(&viewerBase::
                                       mainWindow::getInformation());
    assert(notNull(info));
    return *info;
  }

  bool viewer2D::mainWindow::getFormattedData(const irectangle& area,
                                              image& img) {
    viewer2D* vwr = dynamic_cast<viewer2D*>(parent_);

    return vwr->getFormattedData(area,img);
  }

  ipoint viewer2D::mainWindow::computeCanvasSize() const {
    const viewer2D::parameters* par = 
      dynamic_cast<const viewer2D::parameters*>(params_);
    assert(notNull(par));
    
    const float zoom = par->zoom;
    const ipoint imgSize = getInformation().size;
    
    return ipoint(static_cast<int>(ceilf(imgSize.x*zoom)),
                  static_cast<int>(ceilf(imgSize.y*zoom)));
  }

#ifndef HAVE_GTK
  bool viewer2D::mainWindow::saveButtonPressed(const std::string& /*filename*/) {
    return false;
  }

  void viewer2D::mainWindow::initToolkit() {
  }

  void viewer2D::mainWindow::destroyToolkit() {
  }
#endif

  // -------------------------------------------------------------------------
  // Config dialog
  // -------------------------------------------------------------------------

  viewer2D::configDialog::configDialog(viewer2D& parent) 
    : viewerBase::configDialog(parent) {
  }

  viewer2D::configDialog::configDialog(const configDialog& other,
                                       viewerBase& parent) 
    : viewerBase::configDialog(other,parent) {
    copy(other);
  }

  viewer2D::configDialog::~configDialog() {
  }

  viewer2D::configDialog* 
  viewer2D::configDialog::clone(viewerBase& parent) const {
    return (new configDialog(*this,parent));
  }

  viewer2D::configDialog& 
  viewer2D::configDialog::copy(const configDialog& other) {
    viewerBase::configDialog::copy(other);

    // TODO: Check this out:  does the toolkit need to be copied?
    return *this;
  }

  const viewer2D::information& viewer2D::configDialog::getInformation() const {
    const viewer2D::information* ptr = 
      dynamic_cast<const viewer2D::information*>
      (&viewerBase::configDialog::getInformation());
    assert(notNull(ptr));
    return *ptr;
  }

  bool viewer2D::configDialog::saveButtonPressed(const std::string& filename) {
    if (notNull(parent_)) {
      viewer2D* ptr = dynamic_cast<viewer2D*>(parent_);
      assert(notNull(ptr));
      return ptr->saveButtonPressed(filename);
    }
    return false;
  }

#ifndef HAVE_GTK  
  bool viewer2D::configDialog::useInformation(const viewerBase::information* /*dataInfo*/) {
    return false;
  }

  bool viewer2D::configDialog::build() {
    return false;
  }

  bool viewer2D::configDialog::show() {
    return false;
  }

  bool viewer2D::configDialog::hide() {
    return false;
  }

  bool viewer2D::configDialog::destroy() {
    return false;
  }

  void viewer2D::configDialog::initToolkit() {
  }

  void viewer2D::configDialog::destroyToolkit() {
  }

  bool viewer2D::configDialog::setParameters(const parameters& /*params*/) {
    return false;
  }
#endif
  
}
