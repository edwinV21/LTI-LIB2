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
 * \file   ltiFactory.h
 *         Contains the template type for LTI-Lib factories.
 * \author Gustavo Quiros
 * \date   08.07.2004
 *
 * $Id:
 */

#ifndef _LTI_FACTORY_H_
#define _LTI_FACTORY_H_

#include "ltiObject.h"
#include "ltiClassName.h"
#include "ltiMacroSymbols.h"
#include "ltiFactoryReferences.h"

#include <map>
#include <string>

#include <typeinfo>

/**
 * Helper macro to register a class in a factory at compile time. It must be
 * used in a global context (outside of any function or class), and should be
 * compiled only once, which means this must be in your .cpp file and never in
 * a header!
 *
 * Some scripts parse these entries in the cpp files of the library sources, so
 * please ensure that your registration occupies only one line.  You can use
 * this macro in your own code with no restrictions.
 * 
 * Usage: _LTI_REGISTER_IN_FACTORY("base class name","class name");
 * Example: _LTI_REGISTER_IN_FACTORY(lti::object,lti::derived);
 */
#define _LTI_REGISTER_IN_FACTORY(BASE,CLASS) \
factory<BASE>::registration<CLASS> \
_LTI_FACTORY_REGISTRATION_ ## BASE ## _ ## CLASS ## _;

/**
 * Helper macro to register a class in a factory at compile time, where the
 * base class and the inherited one are both templates of a given type. It must
 * be used in a global context (outside of any function or class), and should
 * be compiled only once, which means this must be in your .cpp file and never
 * in a header!
 *
 * Some scripts parse these entries in the cpp files of the library sources, so
 * please ensure that your registration occupies only one line.  You can use
 * this macro in your own code with no restrictions.
 * 
 * Usage: _LTI_REGISTER_IN_TEMPLATE_FACTORY("template type",
 *                                          "base class name","class name");
 * Example: _LTI_REGISTER_IN_FACTORY(float,lti::parent,lti::derived);
 */
#define _LTI_REGISTER_IN_TEMPLATE_FACTORY(TYPE,BASE,CLASS) \
factory< BASE<TYPE> >::registration< CLASS<TYPE> > \
_LTI_FACTORY_REGISTRATION_ ## BASE ## _ ## CLASS ## _ ## TYPE ## _;

/**
 * Helper macro to register a class in a factory at compile time, where only
 * the inherited class is a template of the given type. It must
 * be used in a global context (outside of any function or class), and should
 * be compiled only once, which means this must be in your .cpp file and never
 * in a header!
 *
 * Some scripts parse these entries in the cpp files of the library sources, so
 * please ensure that your registration occupies only one line.  You can use
 * this macro in your own code with no restrictions.
 * 
 * Usage: _LTI_REGISTER_IN_TEMPLATE_FACTORY("template type",
 *                                          "base class name","class name");
 * Example: _LTI_REGISTER_IN_FACTORY(float,lti::parent,lti::derived);
 */
#define _LTI_REGISTER_TEMPLATE_IN_FACTORY(TYPE,BASE,CLASS) \
factory< BASE >::registration< CLASS<TYPE> > \
_LTI_FACTORY_REGISTRATION_ ## BASE ## _ ## CLASS ## _ ## TYPE ## _;

/**
 * Helper macro to register a class in a factory at compile time, under a given
 * name. It must be used in a global context (outside of any function or
 * class), and should be compiled only once, which means this must be in your
 * .cpp file and never in a header!
 *
 * Some scripts parse these entries in the cpp files of the library sources, so
 * please ensure that your registration occupies only one line.  You can use
 * this macro in your own code with no restrictions.
 *
 * The given name must not contain spaces, or other signs (like :<> etc.) and
 * you give it without any quotes.
 * 
 * Usage: _LTI_REGISTER_IN_FACTORY_AS("query name","base class name",
 *                                    "class name");
 * Example: _LTI_REGISTER_IN_FACTORY_AS(object,lti::object,lti::derived);
 */
#define _LTI_REGISTER_IN_FACTORY_AS(NAME,BASE,CLASS) \
factory<BASE>::registration<CLASS> \
_LTI_FACTORY_REGISTRATION_ ## NAME ## _ ## BASE ## _ ## CLASS ## _(# NAME);

/**
 * Helper macro to register a class in a factory at compile time, under a given
 * name. It must be used in a global context (outside of any function or
 * class), and should be compiled only once, which means this must be in your
 * .cpp file and never in a header!
 *
 * This macro allows to provide a name for the static variable, in those cases
 * where nested classes, template classes or other type of classes do not allow
 * an automatic generation of the name
 *
 * Some scripts parse these entries in the cpp files of the library sources, so
 * please ensure that your registration occupies only one line.  You can use
 * this macro in your own code with no restrictions.
 *
 * The given name must not contain spaces, and you give it without any quotes.
 * 
 * Usage: _LTI_REGISTER_IN_FACTORY_AS_VAR("query name","base class",
 *                                         "class name","variable name");
 * Example: _LTI_REGISTER_IN_FACTORY_AS_VAR(derived<double>,lti::object,
 *                                          lti::derived<double>,
 *                                          DerivedDouble);
 */
#define _LTI_REGISTER_IN_FACTORY_AS_VAR(NAME,BASE,CLASS,VAR) \
factory<BASE>::registration<CLASS> \
_LTI_FACTORY_REGISTRATION_ ## VAR ## _(# NAME);

namespace lti {

  /**
   * Factory of objects of base type T.
   *
   * There is a singleton instance of the factory for each base type T used,
   * accessible through the getFactory() static method. Objects can be
   * registered at compile time by the use of the _LTI_REGISTER_IN_FACTORY
   * macro, or dynamically at runtime with the registerObject() method. An
   * object is registered under a string name, usually the fully qualified
   * name of the class (obtained dynamically from RTTI using the lti::object
   * method name(), or the lti::className). However, registerObject() also
   * allows the registration of an object under an arbitrary name.
   *
   * If you use the macros _LTI_REGISTER_IN_FACTORY*, please be sure
   * that you write the registration in one single line, as those lines
   * are parsed automatically to force the necessary static references.
   *
   * New objects are created using the newInstance() methods. It takes a name,
   * and uses the object registered under this name as a template, calling its
   * clone method (which must be defined).
   *
   * Currently it is not allowed to register an object under an already
   * registered name (the names have to be unique), nor to de-register an
   * object.
   *
   * \section linkproblems Linkers and factory problems.
   *
   * The chosen factory architecture in the LTI-Lib allows to register objects
   * in the factory in a very flexible way.  However, most linkers eliminate
   * all references to classes that are not explicitly used, which affects
   * directly the factories, as the code generated by the compiler, which
   * should make the class registration in the factory is also removed.  To
   * solve this problem in a relatively standard form, there are two files
   * ltiFactoryReferences.h and ltiFactoryReferences.cpp which ensure that
   * whenever a factory is used, all registered objects in the factory are also
   * referenced and therefore not eliminated.
   *
   * Please note that you can register your own classes in the factories, but,
   * for them to work, you have to ensure that the linker will not eliminate
   * your classes.  This can be easily accomplished creating a dummy function
   * or class with references to your classes, which is exactly the task of 
   * ltiFactoryReferences.*.
   *
   * The references in ltiFactoryReferences.cpp are created automatically by
   * the Makefile that compiles the library.
   */
  template <typename T>
  class factory {   
    friend class registration;    
  public:
    /**
     * Base type of the instances in this factory
     */
    typedef T base_type;

    /**
     * Registers an object under the given name.
     *
     * Note that exactly the given instance will be kept.  This means you
     * should never ever delete the given object.  This factory will take care
     * of that at the proper time.
     */
    void registerObject(const base_type* obj, const char *name);

    /**
     * Registers an object under the given name.
     *
     * Note that exactly the given instance will be kept.  This means you
     * should never ever delete the given object.  This factory will take care
     * of that at the proper time.
     */
    void registerObject(const base_type* obj, const std::string& name);

    /**
     * Registers an object under its class name (dynamically obtained)
     */
    void registerObject(const base_type* obj);
    
    /**
     * Creates a new instance, which is a "clone" the object registered
     * under the given name.
     *
     * Returns a null pointer if no such object is registered.
     *
     * \warning You have to take care of the memory management of the returned
     * instance, i.e. you have the remove the object with delete.
     */
    base_type* newInstance(const char *name) const;
    
    /**
     * Creates a new instance, which is a "clone" the object registered
     * under the given name.
     *
     * Returns a null pointer if no such object is registered.
     *
     * \warning You have to take care of the memory management of the returned
     * instance, i.e. you have the remove the object with delete.
     */
    base_type* newInstance(const std::string& name) const;

    /**
     * Singleton factory access
     */
    static factory& getFactory();

    /**
     * Get a list with all registered names.  
     *
     * The map contains pairs of strings, where the first string contains the
     * registered name and the second one the name of the class as reported by
     * lti::className.
     */
    void getRegisteredNames(std::map<std::string,std::string> & names) const;

    /**
     * Represents the registration of an object of type U, constructed with the
     * default parameterless constructor, and under its class name.
     */
    template <typename U>
    class registration {      
    public:      
      /**
       * Creates a new registration, thus registering an object of class U.
       */
      registration() {
        U* u=new U;
        base_type* tmp=getFactory().newInstance(u->name());
        if (isNull(tmp)) {
          getFactory().registerObject(u);
        } else {
          delete u;
          delete tmp;
        }
      }

      /**
       * Creates a new registration, thus registering an object of class U,
       * but under a given name, which is not necessarily the class name.
       */
      registration(const char* name) {
        base_type* tmp=getFactory().newInstance(name);
        if (isNull(tmp)) {
          U* u=new U;
          getFactory().registerObject(u,name);
        } else {
          delete tmp;
        }
      }
    };

  private:
    /**
     * The type of the object map
     */
    typedef std::map<std::string,const base_type*> object_map_type;
    
    /**
     * The object map. Maps names to instances.
     */
    object_map_type objectMap_;
    
    /**
     * Disable the default constructor.
     */
    factory();
    
    /**
     * Disable the copy constructor.
     */
    factory(const factory& f);

    /**
     * The compiler has to be able to destroy the factory
     */
    ~factory();

    /**
     * Disable the assignment operator.
     */
    factory& operator=(const factory& f);

    /**
     * Force static references to allow all library classes to be considered
     * even if the user of the library never makes an explicit reference to one
     * of them.
     */
    void ensureReferences() const;
  };

}  

#include "ltiFactory_template.h"

#endif
