/*
 * Copyright (C) 2006
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
 * \file   ltiDebugIterator.h
 *         Contains a generic implementation of debug iterators (used in debug
 *         builds) for lti::containers
 * \author Peter Doerfler
 * \date   13.08.2006
 *
 * $Id: ltiDebugIterator.h,v 1.2 2007-12-26 04:49:23 alvarado Exp $
 */

#ifndef _LTI_DEBUG_ITERATOR
#define _LTI_DEBUG_ITERATOR

#include "ltiMetaProgrammingTools.h"
#include <iterator>

namespace lti {
  
  namespace internal {
    // the general class declaration which is not needed
    //
    // without this trick it is not possible that Container is a friend of
    // debugIterator, as it is not clear that Container is indeed a
    // class. However, in the specialization below ContainerBase<T> is sure to
    // be a class and is exactly the type the generalVector and generalMatrix
    // classes have.
    template <typename Container, bool constIterator>
    class debugIterator {};
    
    /**
     * A generic debugging iterator for classes genericVector and
     * genericMatrix. See those classes for usage examples.
     *
     * This iterator is compatible with those found in the STL and can thus be
     * used with all of its functions. Specifically, this iterator is also
     * compatible with a simple pointer which is used as a the iterator of the
     * above containers in release mode.
     *
     * Template parameters:
     * - \p Container that has the data, e.g. genericVector<T>. It must
     * provide \p value_type.
     * - \p constIterator: if true a const_iterator is generated, otherwise an
     * iterator
     */
    template <template <class> class ContainerBase, 
              typename T, bool constIterator>
    class debugIterator<ContainerBase<T>, constIterator> 
      : public std::iterator<
      std::random_access_iterator_tag, 
      typename if_t<constIterator,
                    const typename ContainerBase<T>::value_type, 
                    typename ContainerBase<T>::value_type>::type> {
    private:
      // the actual container type
      typedef ContainerBase<T> Container;

      // the std::iterator<> type
      // could have used T, but probably better style like this
      typedef std::iterator<
        std::random_access_iterator_tag, 
        typename if_t<constIterator,
                      const typename Container::value_type, 
                      typename Container::value_type>::type> base_type;
    public:
      typedef typename base_type::iterator_category iterator_category;
      typedef typename base_type::value_type        value_type;
      typedef typename base_type::difference_type   difference_type;
      typedef typename base_type::reference         reference;
      typedef typename base_type::pointer           pointer;

    private:
      // needed for special ctor that initializes the pointers
      friend class ContainerBase<T>;
      // needed for construction of a const_iterator from an iterator
      friend class debugIterator<Container, !constIterator>;

      /**
       * Pointer to the current element
       */
      pointer ptr_;

      /**
       * Pointer to the first element of the container
       */
      pointer begin_;

      /**
       * Pointer behind the last element of the container
       */
      pointer end_;
      
      /**
       * Protected constructor (for internal use only)
       * NEVER USE THIS CONSTRUCTOR EXPLICITLY, OR YOUR CODE WILL NOT
       * COMPILE IN THE RELEASE VERSION!
       *
       * This constructor is used in the Container to initialize the internal
       * pointers.
       *
       * @param ptr pointer to the position the iterator will point to
       * @param begin pointer to the first element of the Container
       * @param end pointer behind the last element of the Container
       */
      inline explicit debugIterator(pointer ptr, pointer begin, pointer end);

    public:

      /**
       * Default constructor
       */
      inline debugIterator();

      /**
       * Copy constructor
       */
      inline debugIterator(const debugIterator& other);

      /**
       * Copy constructor. Construct a debugIterator with \p constIterator ==
       * true from \p other with \p constIterator == false and with the same
       * Container.
       */
      template <bool b>
      inline debugIterator(const debugIterator<
                           typename enable_if<Container, 
                           constIterator&&!b>::type, b>& other);

      /**
       * Advance to next item
       */
      inline debugIterator& operator++();   // prefix

      /**
       * Advance to next item
       */
      inline debugIterator operator++(int); // postfix

      /**
       * Recede to previous item
       */
      inline debugIterator& operator--();   // prefix

      /**
       * Recede to previous item
       */
      inline debugIterator operator--(int); // postfix

      /**
       * Advance (skip) some elements.
       *
       * Use this operator with care! Note that you can skip the end of
       * the Container, and read (or even worse: write!) out of bounds!
       */
      inline debugIterator& operator+=(const difference_type& n);

      /**
       * Recede (skip) some elements.
       *
       * Use this operator with care! Note that you can skip the beginning of
       * the Container, and read (or even worse: write!) out of bounds!
       */
      inline debugIterator& operator-=(const difference_type& n);

      /**
       * Advance (skip) some elements.
       *
       * Use this operator with care! Note that you can skip the end of
       * the Container, and read (or even worse: write!) out of bounds!
       */
      inline debugIterator operator+(const difference_type& n);

      /**
       * Recede (skip) some elements.
       *
       * Use this operator with care! Note that you can skip the beginning of
       * the Container, and read (or even worse: write!) out of bounds!
       */
      inline debugIterator operator-(const difference_type& n);

      /**
       * Get pointed data
       */
      inline reference operator*() const;

      /**
       * Get pointer to data
       */
      inline pointer operator->() const;

      /**
       * Access the elements relative to the debugIterator position
       */
      inline reference operator[](const difference_type& n) const;

      /**
       * Compare if the pointed position of \p other is the same as that of
       * this.
       */
      template<bool constOther>
      inline bool
      operator==(const debugIterator<Container, constOther>& other) const;

      /**
       * Compare if the pointed position of \p other is different from that of
       * this.
       */
      template<bool constOther>
      inline bool
      operator!=(const debugIterator<Container, constOther>& other) const;

      /**
       * Compare if the position to by this is smaller than the position
       * pointed to by \p other.
       */
      template<bool constOther>
      inline bool
      operator<(const debugIterator<Container, constOther>& other) const;

      /**
       * Compare if the position to by this is greater than the position
       * pointed to by \p other.
       */
      template<bool constOther>
      inline bool
      operator>(const debugIterator<Container, constOther>& other) const;

      /**
       * Compare if the position to by this is smaller or equal than the
       * position pointed to by \p other.
       */
      template<bool constOther>
      inline bool
      operator<=(const debugIterator<Container, constOther>& other) const;

      /**
       * Compare if the position to by this is greater or equal than the
       * position pointed to by \p other.
       */
      template<bool constOther>
      inline bool
      operator>=(const debugIterator<Container, constOther>& other) const;

      /**
       * Difference between this and the \p other debugIterator.
       */
      template<bool constOther>
      inline difference_type
      operator-(const debugIterator<Container, constOther>& other) const;
    };

  } // namespace internal

} // namespace lti

#include "ltiDebugIterator_inline.h"

#endif
