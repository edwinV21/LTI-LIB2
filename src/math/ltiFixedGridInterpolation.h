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

/** 
 * \file   ltiFixedGridInterpolation.h
 *         Contains the class lti::fixedGridInterpolation, which is
 *         parent class for all interpolation functors, whose samples have
 *         equal distances to each other (per dimension).
 * \author Peter Doerfler
 * \author Pablo Alvarado
 * \date   03.05.2003
 *
 * $Id: ltiFixedGridInterpolation.h,v 1.5 2012-09-16 05:22:59 alvarado Exp $
 */

#ifndef _LTI_FIXED_GRID_INTERPOLATION_H_
#define _LTI_FIXED_GRID_INTERPOLATION_H_


#include "ltiFunctor.h"
#include "ltiVector.h"
#include "ltiMatrix.h"
#include "ltiBoundaryType.h"
#include "ltiPoint.h"

namespace lti {

  /**
   * This abstract class parents all interpolation functors, whose
   * samples have equal distances to each other (per dimension),
   * i.e. data is in a vector or matrix which includes images.
   *
   *
   * @ingroup gInterpolator
   */
  template <typename T>
  class fixedGridInterpolation : public functor {
  public:

    /**
     * Definition of the type of the elements in the applys (given type T)
     */ 
    typedef T value_type;

    /**
     * The parameters for the class fixedGridInterpolation
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
       * Constructor parameters with given with boundary type
       */
      parameters(const eBoundaryType btype);

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
      const std::string& name() const;

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

      // ------------------------------------------------
      // the parameters
      // ------------------------------------------------

      /**
       * Specify how the boundaries will be used. 
       *
       * See the documentation for lti::eBoundaryType for more information
       *
       * Default: lti::Zero
       */
      eBoundaryType boundaryType;
    };

    /**
     * Default constructor
     */
    fixedGridInterpolation();

    /**
     * Construct a functor using the given parameters
     */
    fixedGridInterpolation(const parameters& par);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    fixedGridInterpolation(const fixedGridInterpolation& other);

    /**
     * Destructor
     */
    virtual ~fixedGridInterpolation();

    /**
     * Returns the name of this class.
     */
    const std::string& name() const;

    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    fixedGridInterpolation& copy(const fixedGridInterpolation& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    fixedGridInterpolation& operator=(const fixedGridInterpolation& other);

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * all the next apply methods will return the interpolated values of the
     * given vector.
     */
    virtual bool use(const vector<T>& vct);

    /**
     * all the next apply methods will return the interpolated values of the
     * given matrix.
     */
    virtual bool use(const matrix<T>& vct);

    /**
     * Returns the interpolated value of the vector at the real valued
     * position x.
     * @param src vector<T> with the source data.
     * @param x the real valued position to be interpolated.
     * @return the interpolated value of the vector.
     */
    virtual T apply(const vector<T>& src,const float& x) const = 0;

    /**
     * Returns the interpolated value of the vector specified with
     * use() at the real valued position x.
     * @param x the real valued position to be interpolated.
     * @return the interpolated value of the vector.  */
    virtual T apply(const float& x) const = 0;

    /**
     * Returns the interpolated value of the matrix at the real valued
     * position (row,col).
     *
     * @param src matrix<T> with the source data.
     * @param row which row
     * @param col which column
     * @return the interpolated value of the matrix.
     */
    virtual T apply(const matrix<T>& src,
                    const float& row,
                    const float& col) const = 0;

    /**
     * Returns the interpolated value of the matrix at the real valued
     * position p.
     *
     * @param src matrix<T> with the source data.
     * @param p the real valued position to be interpolated.
     * @return the interpolated value of the matrix.
     */
    virtual T apply(const matrix<T>& src,const fpoint& p) const = 0;

    /**
     * Returns the interpolated value of the matrix specified with
     * use() at the real valued position (row,col).
     *
     * @param row which row
     * @param col which column
     * @return the interpolated value of the matrix.  */
    virtual T apply(const float& row, const float& col) const = 0;

    /**
     * Returns the interpolated value of the matrix specified with
     * use() at the real valued position p.
     *
     * @param p the real valued position to be interpolated.
     * @return the interpolated value of the matrix.
     */
    virtual T apply(const fpoint& p) const = 0;


    /**
     * Returns the interpolated value of the matrix at the real valued
     * position (row,col).  This method is NOT virtual and can be used
     * if this interpolation type is used as template parameter in time
     * critical situations.
     *
     * If not reimplemented in the inherited class, the virtual apply method
     * with the same signature will be called.
     *
     * @param src matrix<T> with the source data.
     * @param row which row
     * @param col which column
     * @return the interpolated value of the matrix.
     */
    inline T interpolate(const matrix<T>& src,
                         const float& row,
                         const float& col) const;

    /**
     * This method returns which pixel range around the interpolated postition
     * is considered by the interpolation functor.
     *
     * This is very useful for other functors to decide whether they should
     * call the interpolate() methods or the faster interpolateUnchk() methods.
     *
     * If the returned value is a and the interpolated position is (x,y) all
     * pixels with coordinates in \f$ [x-a .. x+a] \times [y-a .. y+a] \f$ 
     * may be considered by the interpolation functor.
     */
    virtual int getRangeOfInfluence() const = 0;

    /**
     * Shortcut for setting the boundary type of a functor derived from
     * modifier.
     *
     * \b Note: The boundaryType is modified via direct access to the
     * internal parameters object.  
     */
    bool setBoundaryType(const eBoundaryType boundaryType);
    
  protected:
    /**
     * the vector in use
     */
    const vector<T>* theVector_;

    /**
     * the matrix in use
     */
    const matrix<T>* theMatrix_;

    /**
     * @name Boundary access operators
     *
     * These inline non-virtual methods are intended to be used by
     * inherited classes to access the extended versions of the at() matrix
     * access methods, for still integer indices.
     */
    //@{
    /**
     * Access with zero boundary
     */
    inline T zeroAt(const vector<T>& img,const int x) const;
                                  
    /**
     * Access with constant boundary
     */
    inline T cstAt(const vector<T>& img,const int x) const;

    /**
     * Access with mirrored boundary
     */
    inline T mirrAt(const vector<T>& img,const int x) const;

    /**
     * Access with periodic boundary
     */
    inline T periAt(const vector<T>& img,const int x) const;

    /**
     * Access with zero boundary
     */
    inline T zeroAt(const matrix<T>& img,const int y,const int x) const;
                                  
    /**
     * Access with constant boundary
     */
    inline T cstAt(const matrix<T>& img,const int y,const int x) const;

    /**
     * Access with mirrored boundary
     */
    inline T mirrAt(const matrix<T>& img,const int y,const int x) const;

    /**
     * Access with periodic boundary
     */
    inline T periAt(const matrix<T>& img,const int y,const int x) const;
    //@}

  };
}

#include "ltiFixedGridInterpolation_inline.h"
#include "ltiFixedGridInterpolation_template.h"

#endif

