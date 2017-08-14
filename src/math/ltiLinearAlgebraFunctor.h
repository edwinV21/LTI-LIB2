/*
 * Copyright (C) 1998 - 2004
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
 * \file   ltiLinearAlgebraFunctor.h
 *         Contains the class lti::linearAlgebraFunctor which is the base
 *         class of all linear algebra functors and defines the global type
 *         lti::integer
 * \author Thomas Rusert
 * \date   01.06.1999
 * 
 * $Id: ltiLinearAlgebraFunctor.h,v 1.7 2006-09-07 11:43:53 doerfler Exp $
 */

#ifndef _LTI_LINEAR_ALGEBRA_FUNCTOR_H_
#define _LTI_LINEAR_ALGEBRA_FUNCTOR_H_

#include "ltiFunctor.h"

namespace lti {

  /**
   * type of integer as defined in f2c.h
   */
  typedef _LTI_F2C_INTEGER integer;

  /**
   * Base class for all lti linear algebra functors.
   *
   * This class is abstract.
   *
   * @ingroup gLinearAlgebra
   */
  class linearAlgebraFunctor : public functor {
  public:

    /**
     * The parameters for the linear algebra functors
     */
    class parameters : public functor::parameters {
    public:
      /**
       * default constructor
       */
      parameters();

      /**
       * copy constructor
       * @param other the parameters object to be copied
       */
      parameters(const parameters& other);

      /**
       * destructor
       */
      virtual ~parameters();

      /**
       * copy the contents of a parameters object
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
       * Returns the name of this type 
       */
      virtual const std::string& name() const;

      /**
       * Returns a pointer to a clone of the parameters
       */
      virtual parameters* clone() const;

      /**
       * Returns a pointer to a new instance of the parameters
       */
      virtual parameters* newInstance() const;

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

      //--------------------------------------------------
      // the parameters
      //--------------------------------------------------

      /**
       * Many linear algebra functors offer (besides the LTI-Lib
       * implementation) to use LAPACK functions for solving a certain
       * problem. If you have LAPACK installed and configured the
       * LTI-Lib to make use of it (e.g. through changing a config
       * file or by just using autoconf) useLapack==true carries out
       * that implementation.
       *
       * If a linear algebra functor does not provide a LAPACK version
       * the setting of this parameter has no effect. Such
       * linearAlgebraFunctors should state that fact in their class
       * description.
       *
       * Since mostly LAPACK methods are faster that those provided in
       * the LTI-Lib the default is true whenever LAPACK was detected
       * and false otherwise. The latter really doesn't have any
       * effect but looks more informative.
       *
       * Sometimes it can be slower to use LAPACK because it uses
       * column-wise scanning of matrices while the LTI-Lib uses
       * rows. This comes from the predominant orientation in the two
       * programming languages (FORTRAN and C++). Thus, often it is
       * necessary to transpose data before and after using
       * LAPACK. For small problems this might be significant.
       *
       * Default: true if LAPACK is available, false otherwise
       */
      bool useLapack;

    };

    
    /**
     * Default constructor
     */
    linearAlgebraFunctor();

    /**
     * Construct a functor using the given parameters
     */
    linearAlgebraFunctor(const parameters& par);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    linearAlgebraFunctor(const linearAlgebraFunctor& other);

    /**
     * Destructor
     */
    virtual ~linearAlgebraFunctor();

    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    linearAlgebraFunctor& copy(const linearAlgebraFunctor& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    linearAlgebraFunctor& operator=(const linearAlgebraFunctor& other);

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;
  };
}


#endif

