/*
 * Copyright (C) 1998 - 2005
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
 * \file   ltiHashMap.h
 * \author Gustavo Quiros
 * \date   15.7.2005
 *
 * $Id: ltiHashMap.h,v 1.5 2008-08-06 17:03:41 alvarado Exp $
 */

#ifndef _LTI_HASH_MAP_H_
#define _LTI_HASH_MAP_H_

// order is important since _LTI_GNUC is also defined when _LTI_ICC is defined.
#if defined(_LTI_ICC)
# include <ext/hash_map>
#elif defined(_LTI_GNUC_4_3)
# include <map>
#elif defined(_LTI_GNUC)
# include <ext/hash_map>
#else
# include <map>
#endif

namespace lti {

  /**
   * Hash function template structure.
   *
   * Used to compute the hash code for a given key by lti::hashMap.
   * Specializations must implement the following method for the computation of
   * the hash:
   *
   * inline std::size_t operator()(T& key) const;
   *
   * The default implementation of this function returns
   *
   * static_cast<std::size_t>(key)
   *
   * which may suffice for some types.
   */
  template <typename T>
  struct hash;

#if defined(_LTI_ICC)

  /**
   * Use the hash function from icc if available.
   */
  template <typename T>
  struct hash : public std::hash_compare<T, std::less<T> > {
    enum
      {	// parameters for hash table
        bucket_size = 4,	// 0 < bucket_size
        min_buckets = 8};	// min_buckets = 2 ^^ N, 0 < N

//     using std::hash_compare<T, std::less<T> >::operator();

    inline size_t operator()(const T& key) const {
      return std::hash_compare<T, std::less<T> >::operator()(key);
    }

    inline bool operator()(const T& key1, const T& key2) const {
      return std::hash_compare<T, std::less<T> >::operator()(key1, key2);
    }
  };

#elif defined(_LTI_GNUC_4_3)
  // In 4.3.x there is a little caos between TR1 and c++0x, so, since we cannot
  // be sure which flavour the user wants to have, we use the most simple
  // approach (we might use a complex macro decision here, but better the
  // failsafe way)

  /**
   * Default implementation of the hash function.
   * The key value must be convertible to std::size_t via
   * a static cast.
   */
  template <typename T>
  struct hash {
    inline std::size_t operator()(const T& key) const {
      return static_cast<std::size_t>(key);
    }
  };


#elif defined(_LTI_GNUC)

  /**
   * Use the hash function from GNU C if available.
   */
  template <typename T>
  struct hash : public __gnu_cxx::hash<T> {
    inline size_t operator()(const T& key) const {
      return __gnu_cxx::hash<T>::operator()(key);
    }
  };
  
#else

  /**
   * Default implementation of the hash function.
   * The key value must be convertible to std::size_t via
   * a static cast.
   */
  template <typename T>
  struct hash {
    inline std::size_t operator()(const T& key) const {
      return static_cast<std::size_t>(key);
    }
  };
  
#endif 
  
  /**
   * An unordered associative container.
   *
   * Its interface aims to be compatible with the TR1 hashtable class, and
   * should be replaced by it after standarization. It is implemented either as
   * a __gnu_cxx::hash_map if using GNU C, or as a std::map otherwise.
   *
   * The corresponding hash function lti::hash<K> should be defined, as well
   * as the < operator for K (temporarily, as a requirement of std::map).
   */
  template <typename K, typename V>
  class hashMap : public
#if defined(_LTI_ICC)
  std::hash_map<K,V,lti::hash<K> >
#elif defined(_LTI_GNUC_4_3)
  std::map<K,V>
#elif defined( _LTI_GNUC)
  __gnu_cxx::hash_map<K,V,lti::hash<K> >
#else
  std::map<K,V>
#endif
  {};
  
}

#endif
