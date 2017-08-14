/*
 * Copyright (C) 1999, 2000, 2001, 2002, 2003, 2004, 2005, 2006
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
 * \file   ltiCorrelation.cpp
 *         Contains the class convolution.
 * \author Pablo Alvarado
 * \date   17.11.99
 *
 * revisions ..: $Id: ltiCorrelation.cpp,v 1.3 2010-08-02 17:37:46 alvarado Exp $
 */

#include "ltiCorrelation.h"
#include "ltiMinimizeBasis.h"
#include "ltiConvolutionHelper.h"

#include <list>

namespace lti {

  // --------------------------------------------------
  // correlation::parameters
  // --------------------------------------------------

  // default constructor

  correlation::parameters::parameters()
    : functor::parameters(), 
      useMask(false), kernel_(0), mask_(0), 
      kernelAverage_(0.0),kernelSize_(0) {
    boundaryType = Zero;
    mode = C3;
  }

  // copy constructor

  correlation::parameters::parameters(const parameters& other)
    : functor::parameters(), kernel_(0), mask_(0), 
      kernelAverage_(0.0),kernelSize_(0)  {
    copy(other);
  }

  // destructor

  correlation::parameters::~parameters() {
    delete kernel_;
    delete mask_;
  }

  // get type name
  const std::string& correlation::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // copy member

  correlation::parameters&
    correlation::parameters::copy(const parameters& other) {
    functor::parameters::copy(other);

    delete kernel_;
    kernel_ = 0;

    kernelAverage_ = 0.0;
    kernelSize_    = 0;

    if (notNull(other.kernel_)) {
      kernel_ = other.kernel_->clone();

      kernelAverage_ = other.kernelAverage_;
      kernelSize_    = other.kernelSize_;
    }

    delete mask_;
    mask_ = 0;

    if (notNull(other.mask_)) {
      mask_ = dynamic_cast<matrix<ubyte>*>(other.mask_->clone());
    }

    mode = other.mode;
    useMask = other.useMask;
    boundaryType = other.boundaryType;

    return *this;
  }

  // clone member

  correlation::parameters* correlation::parameters::clone() const {
    return new parameters(*this);
  }

  correlation::parameters* correlation::parameters::newInstance() const {
    return new parameters;
  }

  const container& correlation::parameters::getKernel() const {
    if (isNull(kernel_)) {
      throw invalidParametersException(name());
    }
    return *kernel_;
  }

  const double& correlation::parameters::getKernelAverage() const {
    return kernelAverage_;
  }

  const int& correlation::parameters::getKernelSize() const {
    return kernelSize_;
  }

  void correlation::parameters::setKernel(const container& aKernel) {
    // remove old kernel
    delete kernel_;
    kernel_ = 0;

    kernelAverage_ = 0.0;
    kernelSize_    = 0;


    // 2D Kernels and Matrices of float or ubyte!
    //
    if (notNull(dynamic_cast<const fmatrix*>(&aKernel))) {
      // there are now two possibilities: its a fmatrix (or channel)
      // or its a kernel2D<float>
      if (notNull(dynamic_cast<const kernel2D<float>*>(&aKernel))) {
        kernel_ = aKernel.clone();
        kernel2D<float>& tmp = *dynamic_cast<kernel2D<float>*>(kernel_);
        tmp.mirror();
        kernelSize_ = (tmp.lastRow()-tmp.firstRow()+1)*
                     (tmp.lastColumn()-tmp.firstColumn()+1);
        kernelAverage_ = tmp.computeSumOfElements()/double(kernelSize_);
      } else {
        // its a channel or fmatrix
        kernel2D<float>* tmp = new kernel2D<float>();
        const fmatrix& mat =
          *dynamic_cast<const fmatrix*>(&aKernel);
        tmp->castFrom(mat,1-mat.rows(),1-mat.columns());
        tmp->mirror();
        kernel_ = tmp;
        kernelSize_ = mat.rows()*mat.columns();
        kernelAverage_ = mat.computeSumOfElements()/double(kernelSize_);
      }
    } else if (notNull(dynamic_cast<const matrix<ubyte>*>(&aKernel))) {
      // there are now two possibilities: 
      // - its a matrix<ubyte> (or matrix<ubyte>), or
      // - its a kernel2D<ubyte>
      if (notNull(dynamic_cast<const kernel2D<ubyte>*>(&aKernel))) {
        kernel_ = aKernel.clone();
        kernel2D<ubyte>& tmp = *dynamic_cast<kernel2D<ubyte>*>(kernel_);
        tmp.mirror();
        matrix<ubyte>::const_iterator it,eit;
        for (it=tmp.begin(),eit=tmp.end(),kernelAverage_=0;
             it!=eit;
             ++it) {
          kernelAverage_+=(*it);
        }
        kernelSize_ = (tmp.lastRow()-tmp.firstRow()+1)*
                     (tmp.lastColumn()-tmp.firstColumn()+1);
        kernelAverage_/=double(kernelSize_);
      } else {
        // its a matrix<ubyte> or matrix<ubyte>
        kernel2D<ubyte>* tmp = new kernel2D<ubyte>();
        const matrix<ubyte>& mat =
          *dynamic_cast<const matrix<ubyte>*>(&aKernel);
        tmp->castFrom(mat,1-mat.rows(),1-mat.columns());
        tmp->mirror();
        kernel_ = tmp;
        matrix<ubyte>::const_iterator it,eit;
        for (it=tmp->begin(),eit=tmp->end(),kernelAverage_=0;
             it!=eit;
             ++it) {
          kernelAverage_+=(*it);
        }
        kernelSize_ = mat.rows()*mat.columns();
        kernelAverage_/=double(kernelSize_);
      }
    } else if (notNull(dynamic_cast<const matrix<double>*>(&aKernel))) {
      // there are now two possibilities: its a matrix<double>
      // or its a kernel2D<double>
      if (notNull(dynamic_cast<const kernel2D<double>*>(&aKernel))) {
        kernel_ = aKernel.clone();
        kernel2D<double>& tmp = *dynamic_cast<kernel2D<double>*>(kernel_);
        tmp.mirror();
        kernelSize_ = (tmp.lastRow()-tmp.firstRow()+1)*
                     (tmp.lastColumn()-tmp.firstColumn()+1);
        kernelAverage_ = tmp.computeSumOfElements()/double(kernelSize_);
      } else {
        // its a matrix<double>
        kernel2D<double>* tmp = new kernel2D<double>();
        const matrix<double>& mat =
          *dynamic_cast<const matrix<double>*>(&aKernel);
        tmp->castFrom(mat,1-mat.rows(),1-mat.columns());
        tmp->mirror();
        kernel_ = tmp;
        kernelSize_ = mat.rows()*mat.columns();
        kernelAverage_ = mat.computeSumOfElements()/double(kernelSize_);
      }

      // continue with 1D kernels!
    } else if (notNull(dynamic_cast<const vector<double>*>(&aKernel))) {
      // there are now two possibilities: its a vector<double>
      // or its a kernel1D<double>
      if (notNull(dynamic_cast<const kernel1D<double>*>(&aKernel))) {
        kernel_ = aKernel.clone();
        kernel1D<double>& tmp = *dynamic_cast<kernel1D<double>*>(kernel_);
        tmp.mirror();
        kernelSize_ = tmp.lastIdx()-tmp.firstIdx()+1;
        kernelAverage_ = tmp.computeSumOfElements()/double(kernelSize_);
      } else {
        // its a vector<double>
        kernel1D<double>* tmp = new kernel1D<double>();
        const vector<double>& vct =
          *dynamic_cast<const vector<double>*>(&aKernel);
        tmp->castFrom(vct,1-vct.size());
        tmp->mirror();
        kernel_ = tmp;
        kernelSize_ = vct.size();
        kernelAverage_ = vct.computeSumOfElements()/double(kernelSize_);
      }
    } else if (notNull(dynamic_cast<const fvector*>(&aKernel))) {
      // there are now two possibilities: its a fvector
      // or its a kernel1D<float>
      if (notNull(dynamic_cast<const kernel1D<float>*>(&aKernel))) {
        kernel_ = aKernel.clone();
        kernel1D<float>& tmp = *dynamic_cast<kernel1D<float>*>(kernel_);
        tmp.mirror();
        kernelSize_ = tmp.lastIdx()-tmp.firstIdx()+1;
        kernelAverage_ = tmp.computeSumOfElements()/double(kernelSize_);
      } else {
        // its a fvector
        kernel1D<float>* tmp = new kernel1D<float>();
        const fvector& vct =
          *dynamic_cast<const fvector*>(&aKernel);
        tmp->castFrom(vct,1-vct.size());
        tmp->mirror();
        kernel_ = tmp;
        kernelSize_ = vct.size();
        kernelAverage_ = vct.computeSumOfElements()/double(kernelSize_);
      }

      // continue with separable kernels
    } else if (notNull(dynamic_cast<const separableKernel<float>*>(&aKernel))){
      kernel_ = aKernel.clone();
      separableKernel<float>& tmp =
        *dynamic_cast<separableKernel<float>*>(kernel_);
      tmp.mirror();
      kernelSize_ = tmp.getRowFilter(0).size()*
                   tmp.getColFilter(0).size();

      kernelAverage_ = double(tmp.computeSumOfElements())/double(kernelSize_);
    } else if (notNull(dynamic_cast<const separableKernel<ubyte>*>(&aKernel))){
      kernel_ = aKernel.clone();
      separableKernel<ubyte>& tmp =
        *dynamic_cast<separableKernel<ubyte>*>(kernel_);
      tmp.mirror();
      kernelSize_ = tmp.getRowFilter(0).size()*
                   tmp.getColFilter(0).size();
      kernelAverage_ = double(tmp.computeSumOfElements())/double(kernelSize_);
    } else if (notNull(dynamic_cast<const separableKernel<double>*>(&aKernel))){
      kernel_ = aKernel.clone();
      separableKernel<double>& tmp = 
        *dynamic_cast<separableKernel<double>*>(kernel_);
      tmp.mirror();
      kernelSize_ = tmp.getRowFilter(0).size()*
                    tmp.getColFilter(0).size();

      kernelAverage_ = double(tmp.computeSumOfElements())/double(kernelSize_);
    } else {
      throw exception("Correlation: Kernel type not supported");
    }

  }

  // set the mask to use
  void correlation::parameters::setMask(const matrix<ubyte>& aMask) {
    delete mask_;
    mask_ = aMask.clone();
  }

  // return the mask in use, or throw exception if none present
  const matrix<ubyte>& correlation::parameters::getMask() const {
    if (notNull(mask_)) {
      return *mask_;
    } else {
      throw invalidParametersException(name());
    }
  }

  // decide whether to use the mask
  void correlation::parameters::setUseMask(const bool b) {
    useMask = b;
  }

  // tell whether the mask is used
  const bool& correlation::parameters::getUseMask() const {
    return useMask;
  }

  /*
   * write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool correlation::parameters::write(ioHandler& handler,
                                      const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    if (isNull(kernel_)) {
      b = b && lti::write(handler,"kernelType","none");
    } else {
      b = b && lti::write(handler,"kernelType",kernel_->name());
      b = b && lti::write(handler,"kernel",getKernel());
    }
    b = b && lti::write(handler,"maskPresent",notNull(mask_));

    if (notNull(mask_)) {
      b = b && mask_->write(handler);
    }

    b = b && lti::write(handler,"boundaryType",boundaryType);
    b = b && lti::write(handler,"mode",mode);

    b = b && functor::parameters::write(handler,false);

    if (complete) {
      b = b && handler.writeEnd();
    }

    return b;
  }

  /*
   * read the parameters from the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also read, otherwise only the data block will be read.
   * @return true if write was successful
   */
  bool correlation::parameters::read(ioHandler& handler,
                                        const bool complete) {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {
      delete kernel_;
      kernel_ = 0;

      std::string str;
      lti::read(handler,"kernelType",str);

      // TODO: replace this with a container factory

      if (str.find("kernel1D") != std::string::npos) {
        // assume float (maybe to be changed in the near future ;-)
        kernel1D<float> kern;
        lti::read(handler,"kernel",kern);
        kernel_ = kern.clone();
        kernelSize_ = kern.lastIdx()-kern.firstIdx()+1;
        kernelAverage_ = kern.computeSumOfElements()/double(kernelSize_);
      } else if (str.find("kernel2D") != std::string::npos) {
        // assume float (maybe to be changed in the near future ;-)
        kernel2D<float> kern;
        lti::read(handler,"kernel",kern);
        kernel_ = kern.clone();
        kernelSize_ = (kern.lastRow()-kern.firstRow()+1)*
                     (kern.lastColumn()-kern.firstColumn()+1);
        kernelAverage_ = kern.computeSumOfElements()/double(kernelSize_);
      } else if (str.find("separableKernel") != std::string::npos) {
        // assume float (maybe to be changed in the near future ;-)
        separableKernel<float> kern;
        lti::read(handler,"kernel",kern);
        kernel_ = kern.clone();
        kernelSize_ = kern.getRowFilter(0).size()*
                      kern.getColFilter(0).size();
        kernelAverage_ = double(kern.computeSumOfElements())/
                         double(kernelSize_);
      }

      delete mask_;
      mask_ = 0;

      bool maskFlag;
      lti::read(handler,"maskPresent",maskFlag);
      if (maskFlag) {
        matrix<ubyte>* newMask = new matrix<ubyte>;
        newMask->read(handler);
        mask_ = newMask;
      }
    }

    b = b && lti::read(handler,"boundaryType",boundaryType);
    b = b && lti::read(handler,"mode",mode);
    
    b = b && functor::parameters::read(handler,false);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

  // --------------------------------------------------
  // correlation
  // --------------------------------------------------

  // default constructor
  correlation::correlation()
    : functor() {
    parameters tmpParam;
    setParameters(tmpParam);
  }

  // default constructor
  correlation::correlation(const parameters& param)
    : functor() {
    setParameters(param);
  }

  // constructor to set filter kernel directly
  correlation::correlation(const container& aKernel)
    : functor() {
    parameters tmpParam;
    tmpParam.setKernel(aKernel);
    setParameters(tmpParam);
  }

  // copy constructor
  correlation::correlation(const correlation& other)
    : functor(),
      matrixProcessingInterface<float>(),
      matrixProcessingInterface<ubyte>(),
      matrixProcessingInterface<double>() {
    copy(other);
  }

  // destructor
  correlation::~correlation() {
  }

  // returns the name of this type
  const std::string& correlation::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // copy member
  correlation& correlation::copy(const correlation& other) {
    functor::copy(other);
    return (*this);
  }

  // clone member
  correlation* correlation::clone() const {
    return new correlation(*this);
  }

  // newInstance member
  correlation* correlation::newInstance() const {
    return new correlation;
  }

  // return parameters
  const correlation::parameters& correlation::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }

  void correlation::setKernel(const container& aKernel) {
    parameters tmpParam(getParameters());
    tmpParam.setKernel(aKernel);
    setParameters(tmpParam);
  }

  // -------------------------------------------------------------------
  // The apply-methods!
  // -------------------------------------------------------------------

  // On place apply for type matrix<ubyte>!
  bool correlation::apply(matrix<ubyte>& srcdest) const {
    matrix<ubyte> tmp;
    if (apply(srcdest,tmp)) {
      tmp.detach(srcdest);
      return true;
    }

    return false;
  }

  // On place apply for type fmatrix!
  bool correlation::apply(fmatrix& srcdest) const {
    fmatrix tmp;
    if (apply(srcdest,tmp)) {
      tmp.detach(srcdest);
      return true;
    }

    return false;
  }

  // On place apply for type fmatrix!
  bool correlation::apply(dmatrix& srcdest) const {
    dmatrix tmp;
    if (apply(srcdest,tmp)) {
      tmp.detach(srcdest);
      return true;
    }
    return false;
  }


  // On place apply for type vector<ubyte>!
  bool correlation::apply(vector<ubyte>& srcdest) const {
    vector<ubyte> tmp;
    if (apply(srcdest,tmp)) {
      tmp.detach(srcdest);
      return true;
    }
    return false;
  }

  // On place apply for type fvector!
  bool correlation::apply(fvector& srcdest) const {

    fvector tmp;
    if (apply(srcdest,tmp)) {
      tmp.detach(srcdest);

      return true;
    }

    return false;
  }

  // On place apply for type dvector!
  bool correlation::apply(dvector& srcdest) const {

    dvector tmp;
    if (apply(srcdest,tmp)) {
      tmp.detach(srcdest);

      return true;
    }

    return false;
  }

  // On copy apply for type matrix<ubyte>!
  bool correlation::apply(const matrix<ubyte>& src,matrix<ubyte>& dest) const {

    // there are two possibilities: kernel2D or separableKernel
    const parameters& param = getParameters();

    // this typedefs are required as a workaround for another VC++ bug!
    typedef convHelper2D<ubyte,classic<ubyte,int> >    convClas;
    typedef convHelper2D<ubyte,coefficient<ubyte,int> > convCoef;
    typedef convHelper2D<ubyte,criterium1<ubyte,int> >  convC1;
    typedef convHelper2D<ubyte,criterium2<ubyte,int> >  convC2;
    typedef convHelper2D<ubyte,criterium3<ubyte,int> >  convC3;

    switch(param.mode) {
      case Classic: {
        convClas conv;

        if (!conv.setKernel(&param.getKernel())) {
          throw invalidParametersException(name());
        }

        if (!param.getUseMask()) {
          if (conv.isKernelSeparable()) {
            conv.applySep(src,dest,param.boundaryType);
          } else {
            conv.apply(src,dest,param.boundaryType);
          }
        } else {
          conv.setMask(param.getMask());
          conv.applyMask(src,dest,param.boundaryType);
        }
      }
      break;
      case Coefficient: {
        convCoef conv;
        conv.getAccumulator().setData(int(param.getKernelAverage()),
                                      param.getKernelSize());

        if (!conv.setKernel(&param.getKernel())) {
          throw invalidParametersException(name());
        }

        if (!param.getUseMask()) {
          if (conv.isKernelSeparable()) {
            conv.applySep(src,dest,param.boundaryType);
          } else {
            conv.apply(src,dest,param.boundaryType);
          }
        } else {
          conv.setMask(param.getMask());
          conv.applyMask(src,dest,param.boundaryType);
        }
      }
      break;
      case C1: {
        convC1 conv;

        if (!conv.setKernel(&param.getKernel())) {
          throw invalidParametersException(name());
        }

        if (!param.getUseMask()) {
          if (conv.isKernelSeparable()) {
            conv.applySep(src,dest,param.boundaryType);
          } else {
            conv.apply(src,dest,param.boundaryType);
          }
        } else {
          conv.setMask(param.getMask());
          conv.applyMask(src,dest,param.boundaryType);
        }
      }
      break;
      case C2: {
        convC2 conv;

        if (!conv.setKernel(&param.getKernel())) {
          throw invalidParametersException(name());
        }

        if (!param.getUseMask()) {
          if (conv.isKernelSeparable()) {
            conv.applySep(src,dest,param.boundaryType);
          } else {
            conv.apply(src,dest,param.boundaryType);
          }
        } else {
          conv.setMask(param.getMask());
          conv.applyMask(src,dest,param.boundaryType);
        }
      }
      break;
      case C3: {
        convC3 conv;

        if (!conv.setKernel(&param.getKernel())) {
          throw invalidParametersException(name());
        }

        if (!param.getUseMask()) {
          if (conv.isKernelSeparable()) {
            conv.applySep(src,dest,param.boundaryType);
          } else {
            conv.apply(src,dest,param.boundaryType);
          }
        } else {
          conv.setMask(param.getMask());
          conv.applyMask(src,dest,param.boundaryType);
        }
      }
      break;
      default:
        throw exception("Invalid correlation mode");
    }

    return true;
  }

  // On copy apply for type channel!
  bool correlation::apply(const fmatrix& src,
                                fmatrix& dest) const {

    const parameters& param = getParameters();

    // this typedefs are required as a workaround for another VC++ bug!
    typedef convHelper2D<float,classic<float> >     convClas;
    typedef convHelper2D<float,coefficient<float> > convCoef;
    typedef convHelper2D<float,criterium1<float> >  convC1;
    typedef convHelper2D<float,criterium2<float> >  convC2;
    typedef convHelper2D<float,criterium3<float> >  convC3;

    switch(param.mode) {
      case Classic: {
        convClas conv;

        if (!conv.setKernel(&param.getKernel())) {
          throw invalidParametersException(name());
        }

        if (!param.getUseMask()) {
          if (conv.isKernelSeparable()) {
            conv.applySep(src,dest,param.boundaryType);
          } else {
            conv.apply(src,dest,param.boundaryType);
          }
        } else {
          conv.setMask(param.getMask());
          conv.applyMask(src,dest,param.boundaryType);
        }
      }
      break;
      case Coefficient: {
        convCoef conv;
        conv.getAccumulator().
          setData(static_cast<float>(param.getKernelAverage()),
                  static_cast<float>(param.getKernelSize()));

        if (!conv.setKernel(&param.getKernel())) {
          throw invalidParametersException(name());
        }

        if (!param.getUseMask()) {
          if (conv.isKernelSeparable()) {
            conv.applySep(src,dest,param.boundaryType);
          } else {
            conv.apply(src,dest,param.boundaryType);
          }
        } else {
          conv.setMask(param.getMask());
          conv.applyMask(src,dest,param.boundaryType);
        }
      }
      break;
      case C1: {
        convC1 conv;

        if (!conv.setKernel(&param.getKernel())) {
          throw invalidParametersException(name());
        }

        if (!param.getUseMask()) {
          if (conv.isKernelSeparable()) {
            conv.applySep(src,dest,param.boundaryType);
          } else {
            conv.apply(src,dest,param.boundaryType);
          }
        } else {
          conv.setMask(param.getMask());
          conv.applyMask(src,dest,param.boundaryType);
        }
      }
      break;
      case C2: {
        convC2 conv;

        if (!conv.setKernel(&param.getKernel())) {
          throw invalidParametersException(name());
        }

        if (!param.getUseMask()) {
          if (conv.isKernelSeparable()) {
            conv.applySep(src,dest,param.boundaryType);
          } else {
            conv.apply(src,dest,param.boundaryType);
          }
        } else {
          conv.setMask(param.getMask());
          conv.applyMask(src,dest,param.boundaryType);
        }
      }
      break;
      case C3: {
        convC3 conv;

        if (!conv.setKernel(&param.getKernel())) {
          throw invalidParametersException(name());
        }

        if (!param.getUseMask()) {
          if (conv.isKernelSeparable()) {
            conv.applySep(src,dest,param.boundaryType);
          } else {
            conv.apply(src,dest,param.boundaryType);
          }
        } else {
          conv.setMask(param.getMask());
          conv.applyMask(src,dest,param.boundaryType);
        }
      }
      break;
      default:
        throw exception("Invalid correlation mode");
    }

    return true;
  }

  // On copy apply for type dmatrix!
  bool correlation::apply(const dmatrix& src,dmatrix& dest) const {

    const parameters& param = getParameters();

    // this typedefs are required as a workaround for another VC++ bug!
    typedef convHelper2D<double,classic<double> >     convClas;
    typedef convHelper2D<double,coefficient<double> > convCoef;
    typedef convHelper2D<double,criterium1<double> >  convC1;
    typedef convHelper2D<double,criterium2<double> >  convC2;
    typedef convHelper2D<double,criterium3<double> >  convC3;

    switch(param.mode) {
      case Classic: {
        convClas conv;

        if (!conv.setKernel(&param.getKernel())) {
          throw invalidParametersException(name());
        }

        if (!param.getUseMask()) {
          if (conv.isKernelSeparable()) {
            conv.applySep(src,dest,param.boundaryType);
          } else {
            conv.apply(src,dest,param.boundaryType);
          }
        } else {
          conv.setMask(param.getMask());
          conv.applyMask(src,dest,param.boundaryType);
        }
      }
      break;
      case Coefficient: {
        convCoef conv;
        conv.getAccumulator().setData(param.getKernelAverage(),
                                      param.getKernelSize());

        if (!conv.setKernel(&param.getKernel())) {
          throw invalidParametersException(name());
        }

        if (!param.getUseMask()) {
          if (conv.isKernelSeparable()) {
            conv.applySep(src,dest,param.boundaryType);
          } else {
            conv.apply(src,dest,param.boundaryType);
          }
        } else {
          conv.setMask(param.getMask());
          conv.applyMask(src,dest,param.boundaryType);
        }
      }
      break;
      case C1: {
        convC1 conv;

        if (!conv.setKernel(&param.getKernel())) {
          throw invalidParametersException(name());
        }

        if (!param.getUseMask()) {
          if (conv.isKernelSeparable()) {
            conv.applySep(src,dest,param.boundaryType);
          } else {
            conv.apply(src,dest,param.boundaryType);
          }
        } else {
          conv.setMask(param.getMask());
          conv.applyMask(src,dest,param.boundaryType);
        }
      }
      break;
      case C2: {
        convC2 conv;

        if (!conv.setKernel(&param.getKernel())) {
          throw invalidParametersException(name());
        }

        if (!param.getUseMask()) {
          if (conv.isKernelSeparable()) {
            conv.applySep(src,dest,param.boundaryType);
          } else {
            conv.apply(src,dest,param.boundaryType);
          }
        } else {
          conv.setMask(param.getMask());
          conv.applyMask(src,dest,param.boundaryType);
        }
      }
      break;
      case C3: {
        convC3 conv;

        if (!conv.setKernel(&param.getKernel())) {
          throw invalidParametersException(name());
        }

        if (!param.getUseMask()) {
          if (conv.isKernelSeparable()) {
            conv.applySep(src,dest,param.boundaryType);
          } else {
            conv.apply(src,dest,param.boundaryType);
          }
        } else {
          conv.setMask(param.getMask());
          conv.applyMask(src,dest,param.boundaryType);
        }
      }
      break;
      default:
        throw exception("Invalid correlation mode");
    }

    return true;
  }


  // On copy apply for type vector<ubyte>!
  bool correlation::apply(const vector<ubyte>& src,
                                vector<ubyte>& dest) const {

    const parameters& param = getParameters();

    // this typedefs are required as a workaround for another VC++ bug!
    typedef convHelper1D<ubyte,classic<ubyte,int> >     convClas;
    typedef convHelper1D<ubyte,coefficient<ubyte,int> > convCoef;
    typedef convHelper1D<ubyte,criterium1<ubyte,int> >  convC1;
    typedef convHelper1D<ubyte,criterium2<ubyte,int> >  convC2;
    typedef convHelper1D<ubyte,criterium3<ubyte,int> >  convC3;

    switch(param.mode) {
      case Classic: {
        convClas conv;

        if (!conv.setKernel(&param.getKernel())) {
          throw invalidParametersException(name());
        }

        conv.apply(src,dest,param.boundaryType);
      }
      break;
      case Coefficient: {
        convCoef conv;

        const ubyte kernelAverage =
          static_cast<ubyte>(param.getKernelAverage());
        const ubyte kernelSize =
          static_cast<ubyte>(param.getKernelSize());

        conv.getAccumulator().setData(kernelAverage,
                                      kernelSize);

        if (!conv.setKernel(&param.getKernel())) {
          throw invalidParametersException(name());
        }

        conv.apply(src,dest,param.boundaryType);
      }
      break;
      case C1: {
        convC1 conv;

        if (!conv.setKernel(&param.getKernel())) {
          throw invalidParametersException(name());
        }

        conv.apply(src,dest,param.boundaryType);
      }
      break;
      case C2: {
        convC2 conv;

        if (!conv.setKernel(&param.getKernel())) {
          throw invalidParametersException(name());
        }

        conv.apply(src,dest,param.boundaryType);
      }
      break;
      case C3: {
        convC3 conv;

        if (!conv.setKernel(&param.getKernel())) {
          throw invalidParametersException(name());
        }

        conv.apply(src,dest,param.boundaryType);
      }
      break;
      default:
        throw exception("Invalid correlation mode");
    }

    return true;
  }

  // On copy apply for type fvector!
  bool correlation::apply(const fvector& src,
                                fvector& dest) const {

    const parameters& param = getParameters();

    // this typedefs are required as a workaround for another VC++ bug!
    typedef convHelper1D<float,classic<float> >     convClas;
    typedef convHelper1D<float,coefficient<float> > convCoef;
    typedef convHelper1D<float,criterium1<float> >  convC1;
    typedef convHelper1D<float,criterium2<float> >  convC2;
    typedef convHelper1D<float,criterium3<float> >  convC3;

    switch(param.mode) {
      case Classic: {
        convClas conv;

        if (!conv.setKernel(&param.getKernel())) {
          throw invalidParametersException(name());
        }

        conv.apply(src,dest,param.boundaryType);
      }
      break;
      case Coefficient: {
        convCoef conv;

        conv.getAccumulator().
          setData(static_cast<float>(param.getKernelAverage()),
                  static_cast<float>(param.getKernelSize()));

        if (!conv.setKernel(&param.getKernel())) {
          throw invalidParametersException(name());
        }

        conv.apply(src,dest,param.boundaryType);
      }
      break;
      case C1: {
        convC1 conv;

        if (!conv.setKernel(&param.getKernel())) {
          throw invalidParametersException(name());
        }

        conv.apply(src,dest,param.boundaryType);
      }
      break;
      case C2: {
        convC2 conv;

        if (!conv.setKernel(&param.getKernel())) {
          throw invalidParametersException(name());
        }

        conv.apply(src,dest,param.boundaryType);
      }
      break;
      case C3: {
        convC3 conv;

        if (!conv.setKernel(&param.getKernel())) {
          throw invalidParametersException(name());
        }

        conv.apply(src,dest,param.boundaryType);
      }
      break;
      default:
        throw exception("Invalid correlation mode");
    }

    return true;
  }

  // On copy apply for type dvector!
  bool correlation::apply(const dvector& src,
                                dvector& dest) const {

    const parameters& param = getParameters();

    // this typedefs are required as a workaround for another VC++ bug!
    typedef convHelper1D<double,classic<double> >     convClas;
    typedef convHelper1D<double,coefficient<double> > convCoef;
    typedef convHelper1D<double,criterium1<double> >  convC1;
    typedef convHelper1D<double,criterium2<double> >  convC2;
    typedef convHelper1D<double,criterium3<double> >  convC3;

    switch(param.mode) {
      case Classic: {
        convClas conv;

        if (!conv.setKernel(&param.getKernel())) {
          throw invalidParametersException(name());
        }

        conv.apply(src,dest,param.boundaryType);
      }
      break;
      case Coefficient: {
        convCoef conv;
        conv.getAccumulator().setData(param.getKernelAverage(),
                                      param.getKernelSize());

        if (!conv.setKernel(&param.getKernel())) {
          throw invalidParametersException(name());
        }

        conv.apply(src,dest,param.boundaryType);
      }
      break;
      case C1: {
        convC1 conv;

        if (!conv.setKernel(&param.getKernel())) {
          throw invalidParametersException(name());
        }

        conv.apply(src,dest,param.boundaryType);
      }
      break;
      case C2: {
        convC2 conv;

        if (!conv.setKernel(&param.getKernel())) {
          throw invalidParametersException(name());
        }

        conv.apply(src,dest,param.boundaryType);
      }
      break;
      case C3: {
        convC3 conv;

        if (!conv.setKernel(&param.getKernel())) {
          throw invalidParametersException(name());
        }

        conv.apply(src,dest,param.boundaryType);
      }
      break;
      default:
        throw exception("Invalid correlation mode");
    }

    return true;
  }

  // ------------------------------------------
  //  template specializations of accumulators
  // ------------------------------------------
  /*
   * classic specialization for ubyte elements
   */
  template<>
  void correlation::classic<ubyte,int>::accumulate(const ubyte& filter,
                                                   const ubyte& src) {
    state_ += static_cast<int>(src)*static_cast<int>(filter);
  }

  /*
   * specialization for integers
   */
  template<>
  int correlation::classic<int,int>::getResult() const {
    return state_/norm_;
  }
  
  template<>
  ubyte correlation::classic<ubyte,int>::getResult() const {
    return static_cast<ubyte>(state_/norm_);
  }

  template<> void 
  lti::correlation::coefficient<ubyte,int>::accumulate(const ubyte& bfilter,
                                                       const ubyte& bsrc) {
    int src = static_cast<int>(bsrc);
    int filter = static_cast<int>(bfilter);

    int tmp = filter-static_cast<int>(kernAvg_);
    sumW_+=tmp;
    sumFW_+=(src*tmp);
    sumFF_+=(src*src);
    sumF_+=src;
  }

  template<>
  int correlation::coefficient<int,int>::getResult() const {
    int tmp = (sumFF_-(sumF_*sumF_/kernSize_))*sumWW_;
    if (tmp>0)
      return static_cast<int>(norm_*(sumFW_-(sumF_*sumW_/kernSize_))/
                              sqrt(tmp));
    else
      return 0;
  }

  template<>
  ubyte correlation::coefficient<ubyte,int>::getResult() const {
    int tmp = (sumFF_-(sumF_*sumF_/kernSize_))*sumWW_;
    if (tmp>0)
      return static_cast<ubyte>(norm_*(sumFW_-(sumF_*sumW_/kernSize_))/
                                (2*sqrt(tmp)));
    else
      return 0;
  }

  /*
   * criterium1 (general implementation)
   * this should work for all floating point formats
   */
  template<>
  void correlation::criterium1<ubyte,int>::accumulate(const ubyte& filter,
                                                      const ubyte& src) {
    if (src>filter) {
      state_ = max(state_,static_cast<int>(src)-static_cast<int>(filter));
    } else {
      state_ = max(state_,static_cast<int>(filter)-static_cast<int>(src));
    }
  }

  template<>
  int correlation::criterium1<int,int>::getResult() const {
    return (static_cast<int>(norm_/(state_+1)));
  }

  template<>
  ubyte correlation::criterium1<ubyte,int>::getResult() const {
    return (static_cast<ubyte>(norm_/(state_+1)));
  }

  template<>
  void correlation::criterium2<ubyte,int>::accumulate(const ubyte& filter,
                                                      const ubyte& src) {
    if (filter>src) {
      this->state_ += (static_cast<int>(filter)-static_cast<int>(src));
    } else {
      this->state_ += (static_cast<int>(src)-static_cast<int>(filter));
    }
  }

  template<>
  void correlation::criterium3<ubyte,int>::accumulate(const ubyte& filter,
                                                      const ubyte& src) {
    int tmp = static_cast<int>(filter)-static_cast<int>(src);
    this->state_ += tmp*tmp;
  }


  /*
   * Read the correlation mode
   *
   * @ingroup gStorable
   */
  bool read(ioHandler& handler,correlation::eMode& data) {
    std::string str;
    if (handler.read(str)) {
      if (str.find("lassic") != std::string::npos) {
        data = correlation::Classic;
      } else if (str.find("oefficient") != std::string::npos) { 
        data = correlation::Coefficient;
      } else if (str == "C1") { 
        data = correlation::C1;
      } else if (str == "C2") { 
        data = correlation::C2;
      } else if (str == "C3") { 
        data = correlation::C3;
      } else {
        data = correlation::Classic;
        str = "Invalid correlation::eMode value " + str;
        handler.setStatusString(str);
        return false;
      }
    } else {
      handler.setStatusString("Could not read correlation::eMode.");
      return false;
    }
    return true;
  }

  /*
   * Write the correlation mode
   *
   * @ingroup gStorable
   */
  bool 
  write(ioHandler& handler,const correlation::eMode& data) {
    bool b=false;
    switch(data) {
      case correlation::Classic:
        b=handler.write("Classic");
        break;
      case correlation::Coefficient:
        b=handler.write("Coefficient");
        break;
      case correlation::C1:
        b=handler.write("C1");
        break;
      case correlation::C2:
        b=handler.write("C2");
        break;
      case correlation::C3:
        b=handler.write("C3");
        break;
      default:
        handler.setStatusString("Unknown correlation::eMode");
        handler.write("Unknown");        
    }
    return b;
  }


}

