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
 * \file   ltiClassName.cpp
 *         Defines the class className to gain the fully qualified name of
 *         a class inherited from lti::object.
 * \author Jochen Wickel
 * \date   01.04.99
 *
 * $Id: ltiClassName.cpp,v 1.5 2008-08-06 03:40:56 alvarado Exp $
 */

#include "ltiConfig.h"
#include "ltiMacroSymbols.h"
#include "ltiClassName.h"

#include <typeinfo>
#include <cstdlib> // for free

#ifdef HAVE_GCC_ABI_DEMANGLE
#include <cxxabi.h>
#endif

namespace lti {

  className::className() {
  }

  className::~className() {
  }

#if defined(HAVE_GCC_ABI_DEMANGLE)

  // implementation for GCC 3.3.x or higher
  void className::decode(const std::string& mangled,
                         std::string& demangled) {

    int error;
    char* buffer = 0;
    __SIZE_TYPE__ size;
    buffer = ::abi::__cxa_demangle(mangled.c_str(),0,&size,&error);
    if (error == 0) {
      demangled = buffer;
    } else {
      demangled = "unknown";
    }

    free(buffer); // __cxa_demangle allocated memory with malloc, so we have
                  // to free it with free()

    strip(demangled); // remove all spaces
  }

#elif defined(_LTI_MSC_VER)

  // implementation for VisualC x.y the encoding is straightforward: each
  // class name is prepended with "class "

  void className::decode(const std::string& mangled,
                         std::string& demangled) {
    
    std::string::size_type pos = 0;
    std::string::size_type p = 0;

    p = mangled.find("class ",pos);
    while (p != std::string::npos) {
      // string "class " found
      if (p>pos) {
        demangled+=mangled.substr(pos,p-pos);
      }
      pos=p+6;
      p = mangled.find("class ",pos);
    }

    // remaining part
    if (pos != std::string::npos) {
      demangled+=mangled.substr(pos);
    }

    strip(demangled); // remove all spaces
  }

#else

  // this is the default implementation
  void className::decode(const std::string& mangled,
                         std::string& demangled) {

    // since we don't know what for a compiler this is, we can just hope
    // the typeid provides something usefull.
    demangled=mangled;

    strip(demangled); // remove all spaces
  }
  
#endif

  std::string className::get(const object* o) {
    std::string dest;
    const char* tmp=typeid(*o).name();
    decode(tmp,dest);
    return dest;
  }

  std::string className::get(const object& o) {
    std::string dest;
    const char* tmp=typeid(o).name();
    decode(tmp,dest);
    return dest;
  }

  void className::get(const object& o,std::string& result) {
    const char* tmp=typeid(o).name();
    decode(tmp,result);
  }

  void className::get(const object* o,std::string& result) {
    const char* tmp=typeid(*o).name();
    decode(tmp,result);
  }

  std::string className::demangle(const std::string& mangled) {
    std::string str;
    decode(mangled,str);
    return str;
  }
  
  void className::strip(std::string& complete) {
    // remove all spaces
    std::string tmp;
    
    std::string::const_iterator it;
    for (it=complete.begin();it!=complete.end();++it) {
      if ((*it) != ' ') {
        tmp.push_back(*it);
      }
    }
    
    tmp.swap(complete);
  }

}
