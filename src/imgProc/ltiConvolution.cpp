/*
 * Copyright (C) 1998-2004
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
 * \file   ltiConvolution.cpp
 *         Contains the class convolution.
 * \author Pablo Alvarado
 * \date   19.04.99
 *
 * revisions ..: $Id: ltiConvolution.cpp,v 1.10 2010-04-24 23:54:28 alvarado Exp $
 */
#include "ltiConvolution.h"
#include "ltiConvolution_template.h"

#include "ltiMinimizeBasis.h"
#include "ltiConvolutionHelper.h"
#include "ltiFactory.h"

#include <list>

namespace lti {


  // --------------------------------------------------
  // convolution::parameters
  // --------------------------------------------------

  // default constructor

  convolution::parameters::parameters()
    : functor::parameters(), kernel_(0) {
    boundaryType = Zero;
  }

  // copy constructor

  convolution::parameters::parameters(const parameters& other)
    : functor::parameters()  {
    kernel_ = 0;
    copy(other);
  }

  // destructor

  convolution::parameters::~parameters() {
    delete kernel_;
  }

  // copy member
  convolution::parameters&
    convolution::parameters::copy(const parameters& other) {
    functor::parameters::copy(other);

    boundaryType = other.boundaryType;

    delete kernel_;
    kernel_ = 0;

    if (notNull(other.kernel_)) {
      kernel_ = other.kernel_->clone();
    }

    return *this;
  }

  const std::string& convolution::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone member

  convolution::parameters* convolution::parameters::clone() const {
    return new parameters(*this);
  }

  convolution::parameters* convolution::parameters::newInstance() const {
    return new parameters();
  }

  const container& convolution::parameters::getKernel() const {
    if (isNull(kernel_)) {
      throw invalidParametersException(name());
    }
    return *kernel_;
  }

  void convolution::parameters::setKernel(const container& aKernel) {
    delete kernel_;
    kernel_ = 0;
    kernel_ = aKernel.clone();
  }

  /*
   * write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */

  bool convolution::parameters::write(ioHandler& handler,
                                         const bool complete) const
  {
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
    b = b && lti::write(handler,"boundaryType",boundaryType);
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
  bool convolution::parameters::read(ioHandler& handler,
                                     const bool complete)
  {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {
      std::string str;

      b = b && lti::read(handler,"kernelType",str);

      delete kernel_;
      kernel_ = factory<container>::getFactory().newInstance(str);
      
      if (notNull(kernel_)) {
        if ((b = b && handler.readBegin())) {
          if ((b = b && handler.trySymbol("kernel"))) {
            b = b && handler.readKeyValueSeparator();
            b = b && kernel_->read(handler);
          }
          b = b && handler.readEnd();
        }
      }

      b = b && lti::read(handler,"boundaryType",boundaryType);

    }

    b = b && functor::parameters::read(handler,false);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

  // --------------------------------------------------
  // convolution
  // --------------------------------------------------

  // default constructor
  convolution::convolution()
    : functor() {
    parameters tmpParam;
    setParameters(tmpParam);
  }

  convolution::convolution(const parameters& par)
    : functor() {
    setParameters(par);
  }

  // constructor to set functor kernel directly
  convolution::convolution(const container& aKernel,
                           const eBoundaryType& boundary)
    : functor() {
    parameters tmpParam;
    tmpParam.setKernel(aKernel);
    tmpParam.boundaryType = boundary;
    setParameters(tmpParam);
  }

  // copy constructor
  convolution::convolution(const convolution& other)
    : functor(),
      matrixProcessingInterface<float>(),
      matrixProcessingInterface<ubyte>(),
      matrixProcessingInterface<double>() {

    copy(other);
  }

  // destructor
  convolution::~convolution() {
  }

  // copy member
  convolution& convolution::copy(const convolution& other) {
    functor::copy(other);
    return (*this);
  }

  const std::string& convolution::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone member
  convolution* convolution::clone() const {
    return new convolution(*this);
  }

  // new instance
  convolution* convolution::newInstance() const {
    return new convolution();
  }

  // return parameters
  const convolution::parameters& convolution::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }

  void convolution::setKernel(const container& aKernel) {
    parameters tmpParam(getParameters());
    tmpParam.setKernel(aKernel);
    setParameters(tmpParam);
  }

  // -------------------------------------------------------------------
  // The apply-methods!
  // -------------------------------------------------------------------

  // On place apply for type matrix<ubyte>!
  bool convolution::apply(matrix<ubyte>& srcdest) const {
    matrix<ubyte> tmp;
    if (apply(srcdest,tmp)) {
      tmp.detach(srcdest);
      return true;
    }
    return false;
  }

  // On place apply for type fmatrix!
  bool convolution::apply(fmatrix& srcdest) const {
    fmatrix tmp;
    if (apply(srcdest,tmp)) {
      tmp.detach(srcdest);
      return true;
    }

    return false;
  }

  // On place apply for type fmatrix!
  bool convolution::apply(dmatrix& srcdest) const {
    dmatrix tmp;
    if (apply(srcdest,tmp)) {
      tmp.detach(srcdest);
      return true;
    }

    return false;
  }


  // On place apply for type vector<matrix<ubyte>::value_type>!
  bool convolution::apply(vector<ubyte>& srcdest) const {
    vector<ubyte> tmp;
    if (apply(srcdest,tmp)) {
      tmp.detach(srcdest);
      return true;
    }

    return false;
  }

  // On place apply for type fvector!
  bool convolution::apply(fvector& srcdest) const {

    fvector tmp;
    if (apply(srcdest,tmp)) {
      tmp.detach(srcdest);
      return true;
    }

    return false;
  }

  // On place apply for type dvector!
  bool convolution::apply(dvector& srcdest) const {

    dvector tmp;
    if (apply(srcdest,tmp)) {
      tmp.detach(srcdest);
      return true;
    }

    return false;
  }

  // On copy apply for type matrix<ubyte>!
  bool convolution::apply(const matrix<ubyte>& src,matrix<ubyte>& dest) const {

    // there are two possibilities: kernel2D or separableKernel
    const parameters& param = getParameters();
    convHelper2D<ubyte,
                 accumulator<ubyte,int> > conv;

    if (src.empty()) {
      dest.clear();
      return true;
    }

	if (!conv.setKernel(&param.getKernel())) {
      throw invalidParametersException(name());
    }

    if (conv.isKernelSeparable()) {
      conv.applySep(src,dest,param.boundaryType);
    } else {
      conv.apply(src,dest,param.boundaryType);
    }

    return true;
  }

  // On copy apply for type fmatrix!
  bool convolution::apply(const fmatrix& src,fmatrix& dest) const {

    const parameters& param = getParameters();
    convHelper2D<float,accumulator<float> > conv;

    if (src.empty()) {
      dest.clear();
      return true;
    }

    if (!conv.setKernel(&param.getKernel())) {
      throw invalidParametersException(name());
    }

    if (conv.isKernelSeparable()) {
      conv.applySep(src,dest,param.boundaryType);
    } else {
      conv.apply(src,dest,param.boundaryType);
    }

    return true;
  }

  // On copy apply for type dmatrix!
  bool convolution::apply(const dmatrix& src,dmatrix& dest) const {

    const parameters& param = getParameters();
    convHelper2D<dmatrix::value_type,accumulator<dmatrix::value_type> > conv;

    if (src.empty()) {
      dest.clear();
      return true;
    }

	if (!conv.setKernel(&param.getKernel())) {
      throw invalidParametersException(name());
    }

    if (conv.isKernelSeparable()) {
      conv.applySep(src,dest,param.boundaryType);
    } else {
      conv.apply(src,dest,param.boundaryType);
    }

    return true;
  }


  // On copy apply for type vector<ubyte>!
  bool convolution::apply(const vector<ubyte>& src,
                                vector<ubyte>& dest) const {

    const parameters& param = getParameters();
    convHelper1D<ubyte,accumulator<int> > conv;

    if (src.empty()) {
      dest.clear();
      return true;
    }

    if (!conv.setKernel(&param.getKernel())) {
      throw invalidParametersException(name());
    }

    conv.apply(src,dest,param.boundaryType);

    return true;
  }

  // On copy apply for type fvector!
  bool convolution::apply(const fvector& src,
                                fvector& dest) const {

    const parameters& param = getParameters();
    convHelper1D<float,accumulator<float> > conv;

    if (src.empty()) {
      dest.clear();
      return true;
    }

    if (!conv.setKernel(&param.getKernel())) {
      throw invalidParametersException(name());
    }

    conv.apply(src,dest,param.boundaryType);

    return true;
  }

  // On copy apply for type dvector!
  bool convolution::apply(const dvector& src,
                                dvector& dest) const {

    const parameters& param = getParameters();
    convHelper1D<double,accumulator<double> > conv;

    if (src.empty()) {
      dest.clear();
      return true;
    }

    if (!conv.setKernel(&param.getKernel())) {
      throw invalidParametersException(name());
    }

    conv.apply(src,dest,param.boundaryType);

    return true;
  }

}




