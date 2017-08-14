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

/*
 * \file   ltiFactory_template.h
 * \author Gustavo Quiros
 * \date   08.07.2004
 *
 * revisions ..: $Id:
 */

#include "ltiAssert.h"

namespace lti {
  // Dissabled methods:

  template <typename T>
  factory<T>::factory() {
    ensureReferences();
  }

  template <typename T>
  factory<T>::factory(const factory<T>& other) {
  }

  template <typename T>
  factory<T>& factory<T>::operator=(const factory<T>& other) {
    return *this;
  }

  // Implemented methods
  
  /*
   * Singleton instance accessor
   */
  template <typename T>
  factory<T>& factory<T>::getFactory() {
    static factory<T> f;
    return f;
  }
  
  /*
   * Registers an object under the given name
   */
  template <typename T>
  void factory<T>::registerObject(const T* obj, const char *name) {
    registerObject(obj,std::string(name));
  }

  /*
   * Registers an object under the given name
   */
  template <typename T>
  void factory<T>::registerObject(const T* obj, const std::string& name) {
    // No object should be already registered. Otherwise there would be a 
    // potential memory leak if we do not free the object, but then again,
    // we don't know if it can or should be freed.
    assert(isNull(objectMap_[name]));
    objectMap_[name] = obj;    
  }


  /*
   * Registers an object under its class name (dynamically obtained)
   */
  template <typename T>
  void factory<T>::registerObject(const T* obj) {
    //std::cerr << "Registering " << obj->name() << std::endl;
    registerObject(obj,className::demangle(typeid(*obj).name()).c_str());
  }

  /*
   * Constructs an object using the clone method of the object registered
   * under the given name. Returns a null pointer if no such object is
   * registered.
   */
  template <typename T>
  typename factory<T>::base_type* 
  factory<T>::newInstance(const std::string& name) const {
    typename object_map_type::const_iterator it;
    it = objectMap_.find(name);
    
    // object already registered?
    if (it == objectMap_.end()) {
      // seems not to...

      // maybe the user forgot the "lti::" namespace qualification?
      if (name.substr(0,5) != "lti::") {
        // well, he didn't indicate it, just try again with it
        std::string fullName = "lti::"+name;
        it = objectMap_.find(fullName);
        if (it == objectMap_.end()) {
          // definitively, not such class has been registered
          return static_cast<T*>(0);
        }
      } else {
        // no, it isn't, so return zero!
        return static_cast<T*>(0);
      }
    } 

    return ((*it).second)->clone();
  }

  /*
   * Constructs an object using the clone method of the object registered
   * under the given name. Returns a null pointer if no such object is
   * registered.
   */
  template <typename T>
  typename factory<T>::base_type* 
  factory<T>::newInstance(const char* name) const {
    return newInstance(std::string(name));
  }

  /*
   * Get a list with all registered names.  
   *
   * The list contains pairs of string, where the first string contains the
   * registered name and the second one the name of the class as reported by
   * lti::className.
   */
  template <typename T>
  void factory<T>::getRegisteredNames(std::map<std::string,
                                               std::string>& names) const {
    names.clear();

    typename object_map_type::const_iterator it;
    for (it=objectMap_.begin();it!=objectMap_.end();++it) {
      names[(*it).first] = className::demangle(typeid(*(*it).second).name());
    }
  }

  template <typename T>
  void factory<T>::ensureReferences() const {
    // call constructor which creates the references
    static factoryReferences refs;
  }

  template<typename T>
  factory<T>::~factory() {
    // this constructor is called only at the very end of a program, since the
    // only instance is private.  However, valgrind reports, with reason, that
    // the allocated memory is never released.  We have to take care of that
    // here.
    typename object_map_type::const_iterator it;
    for (it=objectMap_.begin();it!=objectMap_.end();++it) {
      // remove the instance
      delete (*it).second;
    }
    objectMap_.clear();
  }
}
