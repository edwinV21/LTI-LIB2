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
 * \file   ltiClassName.h
 *         Defines the class className to gain the fully qualified name of
 *         a class inherited from lti::object.
 * \author Jochen Wickel
 * \date   01.04.99
 *
 * $Id: ltiClassName.h,v 1.5 2005-01-17 13:57:18 alvarado Exp $
 */

#ifndef _LTI_CLASSNAME_H_
#define _LTI_CLASSNAME_H_

#include "ltiObject.h"
#include <string>

namespace lti {
  /**
   * Provides methods for getting the class names of lti::objects.
   *
   * This class uses the std::string container to avoid possible
   * memory leaks.
   */
  class className {
  public:
    /**
     * Default constructor
     */
    className();

    /**
     * Destructor
     */
    ~className();

    /**
     * Return the name of the given type.
     *
     * This class uses the std::typeinfo functions, but it "demangles" the
     * output to produce a valid C++ type name, i.e. the returned string
     * considers the template arguments and is usually fully qualified.
     *
     * The implementations should ensure that the returned name has the
     * following format: 
     *
     * \<namespace\>::\<classname\>::\<innerclassname\>::...
     *
     * Example: 
     * \code
     * lti::boundingBox<double>::parameters
     * \endcode
     *
     * \warning The function returns a pointer to an internal class
     * member. So, the pointer is only valid for the lifetime of this
     * object or until the next time \c get is called, whatever occurs
     * first.
     */
    static std::string get(const object* o);

    /**
     * Return the name of the given object.
     *
     * @see get(const object*)
     */
    static std::string get(const object& o);

    /**
     * Return the name of the given type.
     *
     * This class uses the std::typeinfo functions, but it "demangles" the
     * output to produce a valid C++ type name, i.e. the returned string
     * considers the template arguments and is usually fully qualified.
     *
     * The implementations should ensure that the returned name has the
     * following format: 
     *
     * \<namespace\>::\<classname\>::\<innerclassname\>::...
     *
     * Example: 
     * \code
     * lti::boundingBox<double>::parameters
     * \endcode
     *
     *
     * The name is returned in the result parameter. This is a much safer
     * method than the method returning a pointer.
     */
    static void get(const object& o, std::string& result);

    /**
     * Return the name of the given type.
     *
     * This class uses the std::typeinfo functions, but it "demangles" the
     * output to produce a valid C++ type name, i.e. the returned string
     * considers the template arguments and is usually fully qualified.
     *
     * The implementations should ensure that the returned name has the
     * following format: 
     *
     * \<namespace\>::\<classname\>::\<innerclassname\>::...
     *
     * Example: 
     * \code
     * lti::boundingBox<double>::parameters
     * \endcode
     *
     * The name is returned in the result parameter. This is a much safer
     * method than the method returning a pointer.
     */
    static void get(const object* o, std::string& result);

    /**
     * Demangle 
     *
     * You can use this methods to gain the name of a class that is not
     * inherited from lti::object.
     *
     * The input is the one given by typeid().name()
     *
     * Example:
     * \code
     * lti::point<float> p;
     * std::string typeOfP = className::demangle(typeid(p).name());
     * \endcode
     *
     * \warning Remember that the pointer returned by typeid belongs to the
     * system.  You should never delete that pointer.
     */
    static std::string demangle(const std::string& mangled);

  private:
    /**
     * Demangle the given string using OS/Compiler dependent code.
     *
     * This method is highly recursive, so that the current positions in the
     * source and destination strings have to be maintained.
     *
     * @param mangled mangled string
     * @param pos start position in \a mangled to be analyzed
     * @param demangled demangled string
     */
    static void decode(const std::string& mangled,
                       std::string& demangled);

    /**
     * Strip name.
     *
     * All spaces are eliminated
     */
    static void strip(std::string& complete);


  };
}

#endif
