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
 * \file   ltiValidator.h
 *         Contains a class that checks the validity of the elements of
 *         matrices and vectors of floating point types (e.g. float and double). * \author Jochen Wickel
 * \date   05.12.2001
 * 
 * $Id: ltiValidator.h,v 1.4 2005-01-03 16:17:52 alvarado Exp $
 */

#ifndef _LTI_VALIDATOR_H_
#define _LTI_VALIDATOR_H_


#include "ltiFunctor.h"
#include "ltiMatrix.h"

namespace lti {
  /**
   * Matrix and Vector validity check.
   *
   * Checks the validity of a matrix or vector with float or double
   * elements. A matrix is invalid, if it contains nan or inf elements.
   * Otherwise, it is assumed to be valid.
   *
   * @todo The pointer to functions are not a good idea, since it is
   *       not easily serializable.  We need a family of functors that can
   *       belong to a factory, which can then be easily serialized through
   *       their names.
   */
  class validator : public functor {
  public:

    /**
     * Type of a function that checks a single double value
     */
    typedef bool (doubleValidator)(double);

    /**
     * Type of a function that checks a single float value
     */
    typedef bool (floatValidator)(float);

    /**
     * The parameters for the class validator
     */
    class parameters : public functor::parameters {
    public:
      /**
       * Default constructor
       */
      parameters();

      /**
       * Copy constructor
       * @param other the parameters object to be copied
       */
      parameters(const parameters& other);

      /**
       * Destructor
       */
      ~parameters();

      /**
       * Copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& copy(const parameters& other);

      /**
       * Copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& operator=(const parameters& other);

      /**
       * Returns the name of this class.
       */
      virtual const std::string& name() const;

      /**
       * Returns a pointer to a clone of the parameters
       */
      virtual functor::parameters* clone() const;

      /**
       * Returns a pointer to a clone of the parameters
       */
      virtual functor::parameters* newInstance() const;
      
      /**
       * Write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool write(ioHandler& handler,const bool complete=true) const;

      /**
       * Read the parameters from the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool read(ioHandler& handler,const bool complete=true);

      // ------------------------------------------------
      // the parameters
      // ------------------------------------------------

      /**
       * Pointer to a C function or a static member function that
       * evaluates if a value is a valid floating point representation
       * for the double type.
       *
       * Default value: defaultValidateDouble()
       */
      doubleValidator* isDoubleValid;

      /**
       * Pointer to a C function or a static member function that
       * evaluates if a value is a valid floating point representation
       * for the float type.
       *
       * Default value: defaultValidateFloat()
       */
      floatValidator* isFloatValid;

      /**
       * Default function to check the validity of a double value
       */
      static bool defaultValidateDouble(double x);

      /**
       * Default function to check the validity of a float value
       */
      static bool defaultValidateFloat(float x);

    };

    /**
     * Default constructor
     */
    validator();

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    validator(const validator& other);

    /**
     * Destructor
     */
    virtual ~validator();

    /**
     * Operates on the given %parameter.
     * @param src vector<double> with the source data.
     * @return true if the vector is valid or false otherwise.
     */
    bool apply(const vector<double>& src) const;

    /**
     * Operates on the given %parameter.
     * @param src vector<float> with the source data.
     * @return true if the vector is valid or false otherwise.
     */
    bool apply(const vector<float>& src) const;

    /**
     * Operates on the given %parameter.
     * @param src matrix<double> with the source data.
     * @return true if the matrix is valid or false otherwise.
     */
    bool apply(const matrix<double>& src) const;

    /**
     * Operates on the given %parameter.
     * @param src matrix<float> with the source data.
     * @return true if the matrix is valid  or false otherwise.
     */
    bool apply(const matrix<float>& src) const;

    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    validator& copy(const validator& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    validator& operator=(const validator& other);

    /**
     * Returns the name of this class.
     */
    virtual const std::string& name() const;

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual functor* newInstance() const;

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;
  };
}

#endif

