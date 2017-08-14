/*
 * Copyright (C) 2007
 * Pablo Alvarado
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
 * \file   ltiGeometricTransform.cpp
 *         Contains the class geometricTransform,
 *         which has still to be documented by Pablo Alvarado.
 * 
 * \author Pablo Alvarado
 * \date   08.09.2007
 *
 * revisions ..: $Id: ltiGeometricTransform_template.h,v 1.5 2010-02-09 15:55:32 alvarado Exp $
 */

namespace lti {

  // --------------------------------------------------
  // geometricTransform<I>::parameters
  // --------------------------------------------------

  // default constructor
  template <typename T, template<class> class I>
  geometricTransform<T,I>::parameters::parameters()
    : geometricTransformBase::parameters() {
  }

  // copy constructor
  template <typename T, template<class> class I>
  geometricTransform<T,I>::parameters::parameters(const parameters& other)
    : geometricTransformBase::parameters() {
    copy(other);
  }

  // destructor
  template <typename T, template<class> class I>
  geometricTransform<T,I>::parameters::~parameters() {
  }

  // copy member

  template <typename T, template<class> class I>
  typename geometricTransform<T,I>::parameters&
  geometricTransform<T,I>::parameters::copy(const parameters& other) {
    geometricTransformBase::parameters::copy(other);

    
    interpolatorParams = other.interpolatorParams;

    return *this;
  }

  // alias for copy method
  template <typename T, template<class> class I>
  typename geometricTransform<T,I>::parameters&
  geometricTransform<T,I>::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  // class name
  template <typename T, template<class> class I>
  const std::string& geometricTransform<T,I>::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone method
  template <typename T, template<class> class I>
  typename geometricTransform<T,I>::parameters*
  geometricTransform<T,I>::parameters::clone() const {
    return new parameters(*this);
  }

  // new instance
  template <typename T, template<class> class I>
  typename geometricTransform<T,I>::parameters*
  geometricTransform<T,I>::parameters::newInstance() const {
    return new parameters();
  }

  /*
   * Write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  template <typename T, template<class> class I>
  bool geometricTransform<T,I>::parameters::write(ioHandler& handler,
                                         const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {
      lti::write(handler,"interpolatorParams",interpolatorParams);
    }

    b = b && geometricTransformBase::parameters::write(handler,false);

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
  template <typename T, template<class> class I>
  bool geometricTransform<T,I>::parameters::read(ioHandler& handler,
                                        const bool complete)  {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {
      lti::read(handler,"interpolatorParams",interpolatorParams);
    }

    b = b && geometricTransformBase::parameters::read(handler,false);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

  // --------------------------------------------------
  // geometricTransform<T,I>
  // --------------------------------------------------

  // default constructor
  template <typename T, template<class> class I>
  geometricTransform<T,I>::geometricTransform()
    : geometricTransformBase() {
  }

  // default constructor
  template <typename T, template<class> class I>
  geometricTransform<T,I>::geometricTransform(const parameters& par)
    : geometricTransformBase() {


    // set the given parameters
    setParameters(par);
  }


  // copy constructor
  template <typename T, template<class> class I>
  geometricTransform<T,I>::
  geometricTransform(const geometricTransform<T,I>& other)
    : geometricTransformBase(),
      matrixProcessingInterface<T>() {
    copy(other);
  }

  // destructor
  template <typename T, template<class> class I>
  geometricTransform<T,I>::~geometricTransform() {
  }

  // copy member
  template <typename T, template<class> class I>
  geometricTransform<T,I>& 
  geometricTransform<T,I>::copy(const geometricTransform<T,I>& other) {
    geometricTransformBase::copy(other);

    return (*this);
  }

  // alias for copy member
  template <typename T, template<class> class I>
  geometricTransform<T,I>&
  geometricTransform<T,I>::operator=(const geometricTransform<T,I>& other) {
    return (copy(other));
  }

  // class name
  template <typename T, template<class> class I>
  const std::string& geometricTransform<T,I>::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // return parameters
  template <typename T, template<class> class I>
  const typename geometricTransform<T,I>::parameters&
  geometricTransform<T,I>::getParameters() const {
    const parameters* par =
     dynamic_cast<const parameters*>(&geometricTransformBase::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }

  // return parameters
  template <typename T, template<class> class I>
  typename geometricTransform<T,I>::parameters&
  geometricTransform<T,I>::getRWParameters()  {
    parameters* par =
     dynamic_cast<parameters*>(&geometricTransformBase::getRWParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }


  template <typename T, template<class> class I>
  bool geometricTransform<T,I>::updateParameters() {
    const parameters& par = getParameters();

    typename interpolator_type::parameters p = par.interpolatorParams;

    return interpolator_.setParameters(p);
  }

  template <typename T, template<class> class I>
  I<T>& geometricTransform<T,I>::getInterpolator() {
    return interpolator_;
  }

  template <typename T, template<class> class I>
  const I<T>& geometricTransform<T,I>::getInterpolator() const {
    return interpolator_;
  }

  // -------------------------------------------------------------------
  // The apply() member functions
  // -------------------------------------------------------------------
  
  // On place apply for type matrix<T>!
  template <typename T, template<class> class I>
  bool geometricTransform<T,I>::
  apply(matrix<T>& srcdest) const {
    matrix<T> tmp;
    if (apply(srcdest,tmp)) {
      srcdest.detach(tmp);
      return true;
    }
    return false;
  }

  // On place apply for type matrix<T>!
  template <typename T, template<class> class I>
  bool geometricTransform<T,I>::
  apply(matrix<T>& srcdest,fpoint& offset) const {
    matrix<T> tmp;
    if (apply(srcdest,tmp,offset)) {
      srcdest.detach(tmp);
      return true;
    }
    return false;
  }

}

