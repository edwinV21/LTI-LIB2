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
 * \file   ltiTypeInfo.h
 *         Definition of some template functions that give information
 *         about its template type.
 * \author Pablo Alvarado
 * \date   16.05.2001
 *
 * $Id: ltiTypeInfo.h,v 1.6 2007-10-14 20:25:38 alvarado Exp $
 */

#ifndef _LTI_TYPE_INFO_H_
#define _LTI_TYPE_INFO_H_

#include "ltiTypes.h"
#include "ltiClassName.h"
#include <limits>
#include <typeinfo>

// Visual define these as macros
#undef min
#undef max

namespace lti {

  /**
   * This class allows to ask some information required in the
   * LTI-Lib for some types.
   * Note that the members are static, and you do not need any
   * instance of the class to get the type information.
   *
   * For more information about a type you can also use the 
   * std::numeric_limits type of the Standard Template Library.
   *
   * Example:
   * \code
   * if (typeInfo<dreal>::isFloatingPointType()) {
   *   cout << "dreal is a floating point type" << endl;
   * } else {
   *   cout << "this is a really weird case!" << endl;
   * }
   * \endcode
   */
  template<class T>
  class typeInfo {
  public:
    /**
     * Type suggested for accumulation of current type elements
     * (for example int for ubyte).
     */
    typedef T accumulation_type;

    /**
     * Type suggested to accumulate the square of values of the current
     * type
     */
    typedef T square_accumulation_type;

    /**
     * Suggest a norm for the given type.
     * Usually 255 is used for ubyte, 127 for byte, 65535 for all other
     * integer types and 1.0 for the floating point types.
     */
    static T suggestedNorm() throw() {return 65535;}

    /**
     * Return true if the type T is a floating point type.
     */
    static bool isFloatingPointType() throw() {return false;}

    /**
     * Return a const char* with the name of the type
     */
    static const char* name() throw() {return "";}

    /**
     * The difference of this minimum with the std::numeric_limits<T>::min() is
     * that here the minimum value is returned for floating point types and
     * fixed point types.  The STL method returns for floating point values
     * the minimal representable value above zero.  For max() you can use
     * the standard version
     */
    static T min() throw() {return std::numeric_limits<T>::min();}

  private:
    /**
     * Disable creation of instances of this class.
     */
    typeInfo() {};
  };

  // ------------------------
  // template specializations
  // ------------------------

  template<>
  class typeInfo<ubyte> {
  public:
    typedef int32 accumulation_type;
    typedef int32 square_accumulation_type;
    static ubyte suggestedNorm() throw() {return 255;}
    static bool isFloatingPointType() throw() {return false;}
    static const char* name() throw() {return "lti::ubyte";};
    static ubyte min() throw() {return std::numeric_limits<ubyte>::min();}
  private:
    typeInfo() {};
  };

  template<>
  class typeInfo<byte> {
  public:
    typedef int32 accumulation_type;
    typedef int32 square_accumulation_type;
    static byte suggestedNorm() throw() {return 127;}
    static bool isFloatingPointType() throw() {return false;}
    static const char* name() throw() {return "lti::byte";}
    static byte min() throw() {return std::numeric_limits<byte>::min();}
  private:
    typeInfo() {};
  };

  template<>
  class typeInfo<int16> {
  public:
    typedef int32 accumulation_type;
    typedef dreal square_accumulation_type;
    static int16 suggestedNorm() throw() {return 255;}
    static bool isFloatingPointType() throw() {return false;}
    static const char* name() throw() {return "lti::int16";}
    static short int min() throw() {
      return std::numeric_limits<int16>::min();
    }
  private:
    typeInfo() {};
  };

  template<>
  class typeInfo<uint16> {
  public:
    typedef int32 accumulation_type;
    typedef dreal square_accumulation_type;
    static uint16 suggestedNorm() throw() {return 255;}
    static bool isFloatingPointType() throw() {return false;}
    static const char* name() throw() {return "lti::uint16";}
    static uint16 min() throw() {
      return std::numeric_limits<uint16>::min();
    }
  private:
    typeInfo() {};
  };

  template<>
  class typeInfo<int32> {
  public:
    typedef int32 accumulation_type;
    typedef dreal square_accumulation_type;
    static int32 suggestedNorm() throw() {return 65535;}
    static bool isFloatingPointType() throw() {return false;}
    static const char* name() throw() {return "lti::int32";}
    static int32 min() throw() {return std::numeric_limits<int32>::min();}
  private:
    typeInfo() {};
  };

  template<>
  class typeInfo<uint32> {
  public:
    typedef int32 accumulation_type;
    typedef dreal square_accumulation_type;
    static uint32 suggestedNorm() throw() {return 65535;}
    static bool isFloatingPointType() throw() {return false;}
    static const char* name() throw() {return "lti::uint32";}
    static uint32 min() throw() {
      return std::numeric_limits<uint32>::min();
    }
  private:
    typeInfo() {};
  };

  template<>
  class typeInfo<sreal> {
  public:
    typedef sreal accumulation_type;
    typedef dreal square_accumulation_type;
    static sreal suggestedNorm() throw() {return static_cast<sreal>(1);}
    static bool isFloatingPointType() throw() {return true;}
    static const char* name() throw() {return "lti::sreal";};
    static float min() throw() {return -std::numeric_limits<sreal>::max();}
  private:
    typeInfo() {};
  };

  template<>
  class typeInfo<dreal> {
  public:
    typedef dreal accumulation_type;
    typedef dreal square_accumulation_type;
    static dreal suggestedNorm() throw() {return static_cast<dreal>(1);}
    static bool isFloatingPointType() throw() {return true;}
    static const char* name() throw() {return "lti::dreal";};
    static dreal min() throw() {return -std::numeric_limits<dreal>::max();}
  private:
    typeInfo() {};
  };

  /**
   * This tricky specialization lets the typeInfo of any template class with a
   * single template parameter make use of the typeInfo of its template
   * parameter.
   */
  template<typename T, template <class> class U>
  class typeInfo< U<T> > {
  public:
    typedef U<typename typeInfo<T>::accumulation_type> 
    accumulation_type;

    typedef U<typename typeInfo<T>::square_accumulation_type> 
    square_accumulation_type;

    // Let us hope that this trick way works in all cases:
    static U<T> suggestedNorm() throw() {
      return U<T>(typeInfo<T>::suggestedNorm());
    }

    // no container is a pure floating point type!
    static bool isFloatingPointType() throw() {return false;}

    //
    static const char* name() throw() {
      static const std::string theName = 
        className::demangle(typeid(U<T>()).name());
      return theName.c_str();
    }

    // this class has no minimum, as it does not make much sense here.
  private:
    typeInfo() {};
  };
}

#endif

