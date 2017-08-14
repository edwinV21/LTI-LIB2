/*
 * Copyright (C) 2005, 2006
 * Peter Doerfler
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
 * \file   ltiMetaProgrammingTools.h
 *         Contains some useful meta programming tools such as if_t, switch_t
 * \author Peter Doerfler
 * \date   23.09.2005
 *
 * $Id: ltiMetaProgrammingTools.h,v 1.4 2007-09-15 18:29:28 alvarado Exp $
 */

#ifndef _LTI_META_PROGRAMMING_TOOLS_H_
#define _LTI_META_PROGRAMMING_TOOLS_H_

namespace lti {

  /**
   * Namespace containing material intended for the internal use within the
   * LTI-Lib, but is not supposed to be used outside the library.
   */
  namespace internal {

    /**
     * are_same<T, U>::value is \a true if \c T \c == \c U and \a false
     * otherwise.
     */
    template <typename T, typename U>
    struct are_same {
      enum {value = false};
    };

    template <typename T>
    struct are_same<T, T> {
      enum {value = true};
    };

    /**
     * enable_if<T, bool b>::type is T if b is true and empty otherwise
     */
    template <typename T, bool b>
    struct enable_if {
      typedef void type;
    };

    template <typename T>
    struct enable_if<T, true> {
      typedef T type;
    };
    

    /**
     * Defines the if_t<B,THEN,ELSE>::type as THEN if B==true and as
     * ELSE if B==false.
     *
     * The following example is not from the real world but shows the idea:
     * \code
     * struct container<bool readOnly> {
     *   typedef typename internal::if_t<readOnly, const_iterator, iterator> iterator_type;
     *   iterator_type begin();
     * \endcode
     *
     */
    template <bool B, class THEN, class ELSE = void> 
    struct if_t {
      typedef THEN type;
    };

    // specialization
    template <class THEN, class ELSE>
    struct if_t<false, THEN, ELSE> {
      typedef ELSE type;
    };

    // e.g if_t<false, T>::type no definitely creates a compilation error.
//     template <class THEN>
//     struct if_t<false, THEN, void> {
//     };
    
    /**
     * Helper type for the lti::internal::switch_t struct. Used as 'default'
     * when no default_t is given.
     */
    struct no_type {
      template <int>
      struct impl {
        typedef void type;
      };
    };

    /**
     * A "case" in a lti::internal::switch_t. If the value used for switching
     * is equal to \p Y the switch_t returns \p T, otherwise it goes on with
     * the next \p CASE. Default for case is \p lti::internal::no_type which
     * should be avoided.
     *
     * See lti::internal::switch_t for a more detailed documentation.
     */
    template <int Y, class T, class CASE=no_type>
    struct case_t {
      
      template <int X>
      struct impl {
        typedef typename if_t<
          X==Y,
          T, typename CASE::template impl<X>::type
        >::type type;
      };
    };

    /**
     * Specifies the default type \p T in an lti::internal::switch_t. See
     * there for further information.
     */
    template <class T>
    struct default_t {
      template <int>
      struct impl {
        typedef T type;
      };
    };
    
    /**
     * A compile time switch statement.
     *
     * It is recommended to use enum values or static const int
     * members for specifying \p X as well as the case parameters.
     *
     * The following examples clarify the syntax. They all use the
     * following setup, that defines a type foo<X>::type that is
     * dependent on the value of X:
     *
     * \code
     * enum VAL{One, Two, Three};
     * template <VAL X>
     * struct foo {
     *   typedef typename internal::switch_t<X, .....>::type type;
     * }
     * \endcode
     *
     * The most simple case is just a default type:
     * \code
     * typedef typename internal::switch_t<X, 
     *   internal::default_t<int> >::type type;
     * \endcode
     *
     * Just one case could have been done using if_t as well, but like
     * this it is more easily extended. Here, if \c X == \c One then
     * \c type is \c float otherwise \c type is \c int.
     *
     * \code
     * typedef typename internal::switch_t<X, 
     *   internal::case_t<One, float, 
     *   internal::default_t<int> > >::type type;
     * \endcode
     *
     * Finally, and recommended, include all possible values as cases
     * in the enum and supply a default that will lead to a
     * notification of some sort. \b Note that it is considered bad style
     * not to provide a default.
     *
     * \code
     * typedef typename internal::switch_t<X,
     *   internal::case_t<One, int,
     *   internal::case_t<Two, float,
     *   internal::case_t<Three, std::complex<float>,
     *   internal::default_t<void> > > > >::type type;
     * \endcode
     */
    template <int X, class CASE>
    struct switch_t {
      
      typedef typename CASE::template impl<X>::type type;
    };
    

  } // namespace internal
} // namespace lti

#endif
