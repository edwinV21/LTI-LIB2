/*
 * Copyright (C) 1998-2005
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
 * \file   ltiBicubicInterpolationWorkaround_template.h
 *         Contains the template class bicubicInterpolationWorkaround, which is
 *         a workaround for lti::bicubicInterpolation. It uses 
 *         lti::genericInterpolation with a bicubic Kernel.
 *
 * \author Arnd Hannemann
 * \date   13.5.2005
 *
 * revisions ..: $Id: ltiBicubicInterpolationWorkaround_template.h,v 1.5 2006-05-16 11:59:36 doerfler Exp $
 */



namespace lti {
  // --------------------------------------------------
  // bicubicInterpolationWorkaround<T>::parameters
  // --------------------------------------------------

  // default constructor
  template<typename T>
  bicubicInterpolationWorkaround<T>::parameters::parameters()
    : genericInterpolation<T>::parameters() {

    // hardcode to use the bicubickernel
    this->kernelType = genericInterpolation<T>::BicubicKernel;
    
  }

  // copy constructor
  template<typename T>
  bicubicInterpolationWorkaround<T>::parameters::parameters(const parameters& other)
    : genericInterpolation<T>::parameters() {
    copy(other);
  }

  // destructor
  template<typename T>
  bicubicInterpolationWorkaround<T>::parameters::~parameters() {
  }

  // copy member

  template<typename T>
  typename bicubicInterpolationWorkaround<T>::parameters&
  bicubicInterpolationWorkaround<T>::parameters::copy(const parameters& other) {
    genericInterpolation<T>::parameters::copy(other);


    return *this;
  }

  // alias for copy method
  template<typename T>
  typename bicubicInterpolationWorkaround<T>::parameters&
  bicubicInterpolationWorkaround<T>::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  // class name
  template<typename T>
  const std::string&
  bicubicInterpolationWorkaround<T>::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone method
  template<typename T>
  typename bicubicInterpolationWorkaround<T>::parameters*
  bicubicInterpolationWorkaround<T>::parameters::clone() const {
    return new parameters(*this);
  }

  // new instance
  template<typename T>
  typename bicubicInterpolationWorkaround<T>::parameters* 
  bicubicInterpolationWorkaround<T>::parameters::newInstance() const {
    return new parameters();
  }

  /*
   * Write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  template<typename T>
  bool bicubicInterpolationWorkaround<T>::parameters::write(ioHandler& handler,
                                            const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {
     
    }

    b = b && genericInterpolation<T>::parameters::write(handler,false);

    if (complete) {
      b = b && handler.writeEnd();
    }

    return b;
  }

  /*
   * Read the parameters from the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also read, otherwise only the data block will be read.
   * @return true if read was successful
   */
  template<typename T>
  bool bicubicInterpolationWorkaround<T>::parameters::read(ioHandler& handler,
                                           const bool complete)  {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {
      
    }

    b = b && genericInterpolation<T>::parameters::read(handler,false);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

  // --------------------------------------------------
  // bicubicInterpolationWorkaround
  // --------------------------------------------------

  // default constructor
  template<typename T>
  bicubicInterpolationWorkaround<T>::bicubicInterpolationWorkaround()
    : genericInterpolation<T>() {

    //TODO: comment the attributes of your functor
    // If you add more attributes manually, do not forget to do following:
    // 1. indicate in the default constructor the default values
    // 2. make sure that the copy member also copy your new attributes, or
    //    to ensure there, that these attributes are properly initialized.

    // create an instance of the parameters with the default values
    parameters defaultParameters;
    // set the default parameters
    setParameters(defaultParameters);
  }

  // default constructor
  template<typename T>
  bicubicInterpolationWorkaround<T>::bicubicInterpolationWorkaround(const parameters& par)
    : genericInterpolation<T>() {

    //TODO: comment the attributes of your functor
    // If you add more attributes manually, do not forget to do following:
    // 1. indicate in the default constructor the default values
    // 2. make sure that the copy member also copy your new attributes, or
    //    to ensure there, that these attributes are properly initialized.

    // set the given parameters
    setParameters(par);
  }


  // copy constructor
  template<typename T>
  bicubicInterpolationWorkaround<T>::bicubicInterpolationWorkaround(const bicubicInterpolationWorkaround<T>& other)
    : genericInterpolation<T>() {
    copy(other);
  }

  // destructor
  template<typename T>
  bicubicInterpolationWorkaround<T>::~bicubicInterpolationWorkaround<T>() {
  }

  // copy member
  template<typename T>
  bicubicInterpolationWorkaround<T>& 
  bicubicInterpolationWorkaround<T>::copy(const bicubicInterpolationWorkaround<T>& other) {
    genericInterpolation<T>::copy(other);

    //TODO: comment the attributes of your functor
    // If you add more attributes manually, do not forget to do following:
    // 1. indicate in the default constructor the default values
    // 2. make sure that the copy member also copy your new attributes, or
    //    to ensure there, that these attributes are properly initialized.

    return (*this);
  }

  // alias for copy member
  template<typename T>
  bicubicInterpolationWorkaround<T>&
  bicubicInterpolationWorkaround<T>::operator=(const bicubicInterpolationWorkaround<T>& other) {
    return (copy(other));
  }

  // class name
  template<typename T>
  const std::string&
  bicubicInterpolationWorkaround<T>::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone member
  template<typename T>
  bicubicInterpolationWorkaround<T>* bicubicInterpolationWorkaround<T>::clone() const {
    return new bicubicInterpolationWorkaround<T>(*this);
  }

  // clone member
  template<typename T>
  bicubicInterpolationWorkaround<T>* bicubicInterpolationWorkaround<T>::newInstance() const {
    return new bicubicInterpolationWorkaround<T>();
  }

  // return parameters
  template<typename T>
  const typename bicubicInterpolationWorkaround<T>::parameters&
  bicubicInterpolationWorkaround<T>::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }

  // -------------------------------------------------------------------
  // The apply-methods!
  // -------------------------------------------------------------------

  //TODO: This has to be fixed to the template syntax:

  
}
