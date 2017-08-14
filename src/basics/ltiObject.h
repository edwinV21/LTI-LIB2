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
 * \file   ltiObject.h
 *         Defines the parent class of most classes in the LTI-Lib
 * \author Pablo Alvarado
 * \date   01.04.99
 *
 * $Id: ltiObject.h,v 1.7 2005-03-29 13:09:52 doerfler Exp $
 */

#ifndef _LTI_OBJECT_H_
#define _LTI_OBJECT_H_

#include <string>

/**
 * Macro used to simplify the efficient "computation" of the class name
 */
#undef _LTI_RETURN_CLASS_NAME 
#define _LTI_RETURN_CLASS_NAME \
  static const std::string theName = this->buildName(); \
  return theName;

/**
 * Namespace in which all classes, functions and constants of the LTI-Lib
 * are defined.
 */
namespace lti {
  /**
   * Base class for most objects in the LTI-Lib.
   *
   * All classes inherited from lti::object have three important methods:
   * - name() returns a const reference to a std::string with the name
   *   of the class (see lti::className for more information).
   * - clone() a new instance of the object is created and a pointer to
   *   this instance is returned.  The internal state of the instance is
   *   a copy of the original.
   * - newInstance() a new instance of the same object class is created
   *   using the default constructor.
   *
   * Note that there are many classes in the library which are not inherited
   * from this class, but most complex objects are (functors, classifiers,
   * containers, etc.).  The simplest decision rule to decide if a class
   * should inherit from lti::object is the question, should the objects be
   * "clonable"?
   *
   * The LTI-Lib makes extensive use of the C++ feature which allows to change
   * the type of overloaded virtual methods.  This feature was not supported
   * in old compilers like Visual C++ 6.0 or early GCC compilers, and is a
   * reason why they are not supported.
   *
   * In all inherited classes the method name() has to be overloaded.
   * The content of those methods can be easily implemented with the
   * macro _LTI_RETURN_CLASS_NAME.  For example:
   * 
   * \code
   * const std::string& yourClass::name() {
   *   _LTI_RETURN_CLASS_NAME
   * }
   * \endcode
   */
  class object {
  public:
    /**
     * Default constructor.
     *
     * Initializes the complete library
     */
    object();

    /**
     * Destructor
     */
    virtual ~object();

    /**
     * Return the fully qualified name of this class.
     *
     * Each class has to overload this function and return its name. The
     * returned string is system-independent.  It uses lti::className to
     * generate the class name but uses an internal singleton to avoid
     * building the class name more than once.
     *
     * Note that the factories of the LTI-Lib can use the name given by 
     * name() to create instances of a class at runtime (see lti::factory).
     *
     * This method must be overloaded in all derived classes.  You can
     * use the macro _LTI_RETURN_CLASS_NAME to produce the desired
     * behaviour.  For example:
     * 
     * \code
     * const std::string& yourClass::name() {
     *   _LTI_RETURN_CLASS_NAME
     * }
     * \endcode
     *
     * The returned strings do not have any spaces, even if the
     * resulting name is not anymore C++ compliant.  This rule is
     * easier to remember as a rule to specify when the spaces are
     * required!  For example, a type "lti::list< lti::point<double>
     * >" will produce the name "lti::list<lti::point<double>>".
     * Remember that the space between the last two angle brackets "> >" is
     * in C++ necessary to avoid syntactic confusion with the
     * operator ">>".
     *
     * @return The fully qualified name of this class without any spaces.
     *
     * This method replaces the old method getTypeName() in older versions of
     * the LTI-Lib.  Since the results are totally different, a new name
     * has been chosen to make this difference clear!
     */
    virtual const std::string& name() const = 0;

    /**
     * Clone method.
     *
     * You can create an identical instance of the current lti::object using
     * this method.
     *
     * The difference with the method newInstance() is that here the internal
     * state is kept in the copies while newInstance() creates always an
     * object with the default parameters.
     *
     * Usually, the implementation of the overloaded method in an inherited
     * class will make use of the copy constructor:
     * \code
     *   yourClass* yourClass::clone() const {
     *     return new yourClass(*this);
     *   }
     * \endcode
     *
     * @return a pointer to the new instance.
     *
     * \warning You have to take care of the memory management of the returned
     * instance and delete it when you don't need it any more.  Otherwise you
     * will have some memory leaks.
     */
    virtual object* clone() const = 0;

    /**
     * New instance method.
     *
     * You can create a new instance of the current object, using the
     * default constructor.
     *
     * Note that the difference with the clone() method is that the latter 
     * also copies the internal state of the object, while this method just
     * uses the default constructor.
     *
     * Usually, the implementation of the overloaded method in an inherited
     * class will make use of the default constructor:
     * \code
     *   yourClass* yourClass::newInstance() const {
     *     return new yourClass();
     *   }
     * \endcode
     *
     * @return a pointer to the new instance.
     *
     * \warning You have to take care of the memory management of the returned
     * instance and delete it when you don't need it any more.  Otherwise you
     * will have some memory leaks.
     */
    virtual object* newInstance() const = 0;

  protected:

    /**
     * Build for the name of the current class.
     *
     * Note that the same string is returned by the method name(), but
     * in a MUCH more efficient way.  This method has to demangle the
     * output of typeid().name() which takes some time.  The method
     * lti::object::name() calls the buildName() only once and stores
     * the result in a static variable which is just returned in
     * successive calls, taking much less time.
     *
     * You should NEVER EVER use this method directly, use name() instead.
     */
    virtual std::string buildName() const;
  };

  /**
   * Check if the given pointer is NULL
   */
  inline bool isNull(const void* p) {return p == 0;}

  /**
   * Check if the given pointer is not NULL
   */
  inline bool notNull(const void* p) {return p != 0;}

}

#endif
