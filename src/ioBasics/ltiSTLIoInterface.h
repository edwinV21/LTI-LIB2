/*
 * Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006
 * Lehrstuhl fuer Technische Informatik, RWTH-Aachen, Germany
 *
 * This file is part of the LTI-Computer Vision Library 2 (LTI-Lib-2)
 *
 * The LTI-Lib-2 is free software; you can redistribute it and/or
 * modify it under the terms of the BSD License.
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the authors nor the names of its contributors may be
 *    used to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/** 
 * \file   ltiSTLIoInterface.h
 * This file contains read and write methods for some often used STL
 * containers. 
 *
 * Note that for the read() functions to work the type T must
 * implement the the operator=(). Further, for all functions the type T must
 * have a corresponding read/write function. The following functions are
 * implemented:
 * - bool read(ioHandler& handler, std::list<T>& data, const bool complete)
 * - bool write(ioHandler& handler, const std::list<T>& data, const bool comp)
 * - bool read(ioHandler& handler, std::vector<T>& data, const bool comp)
 * - bool write(ioHandler& handler, const std::vector<T>& data, const bool comp)
 * - bool read(ioHandler& handler, std::pair<T,U>& data, const bool comp)
 * - bool write(ioHandler& handler, const std::pair<T,U>& data,const bool comp)
 * - bool read(ioHandler& handler, std::map<T,U>& data, const bool comp)
 * - bool write(ioHandler& handler, const std::map<T,U>& data, const bool comp)
 * - bool read(ioHandler& handler, std::multimap<T,U>& data, const bool comp)
 * - bool write(ioHandler& handler, const std::multimap<T,U>& data, const bool)
 * The parameters are:
 * - \a handler: ioHandler to which the data is to be written or from which it
 *               is read.
 * - \a data: the data to be written.
 * - \a complete: if true the data is enclosed by the begin and end markers of
 *                the given ioHandler
 * 
 * \author Pablo Alvarado
 * \date   11.05.2001
 *
 * revisions ..: $Id: ltiSTLIoInterface.h,v 1.4 2013-09-15 19:20:18 alvarado Exp $
 */

#ifndef _LTI_STL_IO_INTERFACE_H_
#define _LTI_STL_IO_INTERFACE_H_

#include "ltiIoHandler.h"
#include <list>
#include <vector>
#include <map>
#include <set>

/** @file ltiSTLIoInterface.h 
 */

namespace lti {

  // -------------------------------------------------------------------------
  // std::pair
  // -------------------------------------------------------------------------

  /**
   * Read function for STL std::pair.
   * Works only if the types in the std::pair can be read/written with
   * the lti::read/lti::write functions.
   *
   * @ingroup gStorable
   */
  template<class T,class U>
  bool read(ioHandler& handler,std::pair<T,U>& data,const bool complete=true) {
    bool b = true;

    if (complete) {
      b = handler.readBegin();
    }

    b = b && lti::read(handler,data.first);
    b = b && handler.readDataSeparator();
    b = b && lti::read(handler,data.second);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

  /**
   * Write function for STL std::pair.
   * Works only if the types in the std::pair can be read/written with
   * the lti::read/lti::write functions.
   *
   * @ingroup gStorable
   */
  template<class T,class U>
  bool write(ioHandler& handler,const std::pair<T,U>& data,
             const bool complete=true){
    bool b = true;

    if (complete) {
      b = handler.writeBegin();
    }

    b = b && lti::write(handler,data.first);
    b = b && handler.writeDataSeparator();
    b = b && lti::write(handler,data.second);

    if (complete) {
      b = b && handler.writeEnd();
    }

    return b;
  }


  // -------------------------------------------------------------------------
  // std::list
  // -------------------------------------------------------------------------

  /**
   * Read function for STL list containers
   * Works only if the type T implements the operator=()
   *
   * @ingroup gStorable
   */
  template <class T>
  bool read(ioHandler& handler,std::list<T>& data,const bool complete=true) {
    int i,size;
    bool b = true;

    if (complete) {
      b = handler.readBegin();
    }

    b = b && handler.read("size",size);
    std::string str;

    int level = handler.getLevel();

    b = b && handler.readBegin();
    b = b && handler.readSymbol(str);

    if (str == "data") {
      b = b && handler.readKeyValueSeparator();
      b = b && handler.readBegin();
      data.clear();
      if (size > 0) {
        size--;
        T tmp;
        for (i=0;i<size;++i) {
          b = b && read(handler,tmp);
          data.push_back(tmp);
          b = b && handler.readDataSeparator();
        }
        b = b && read(handler,tmp);
        data.push_back(tmp);
      }
    }

    // read all the next end tokens... and ensure consistency
    while (handler.readEnd() && (handler.getLevel() > level)) {
    };

    if (complete) {
      b = b && handler.readEnd();
    }
    return b;
  }

  /**
   * Write function for STL list containers
   *
   * @ingroup gStorable
   */
  template <class T>
  bool write(ioHandler& handler,
             const std::list<T>& data,
             const bool complete=true) {
    int i;
    const int theSize = static_cast<int>(data.size());
    bool b = true;

    if (complete) {
      b = handler.writeBegin();
    }

    b = b && handler.write("size",theSize);

    b = b && handler.writeBegin();
    b = b && handler.writeSymbol(std::string("data"));
    b = b && handler.writeKeyValueSeparator();
    b = b && handler.writeBegin();
    if (theSize > 0) {
      typename std::list<T>::const_iterator it;
      // begin from one to ensure that the last element of the container
      // will NOT be written yet!
      for (i=1,it=data.begin();i<theSize;++it,++i) {
        b = b && write(handler,(*it));
        b = b && handler.writeDataSeparator();
      }
      b = b && write(handler,(*it));
    }
    b = b && handler.writeEnd();
    b = b && handler.writeEnd();

    if (complete) {
      b = b && handler.writeEnd();
    }

    return b;
  }

  // -------------------------------------------------------------------------
  // std::set
  // -------------------------------------------------------------------------

  /**
   * Read function for STL set containers
   * Works only if the type T implements the operator=()
   *
   * @ingroup gStorable
   */
  template <class T>
  bool read(ioHandler& handler,std::set<T>& data,const bool complete=true) {
    int i,size;
    bool b = true;

    if (complete) {
      b = handler.readBegin();
    }

    b = b && handler.read("size",size);
    std::string str;

    int level = handler.getLevel();

    b = b && handler.readBegin();
    b = b && handler.readSymbol(str);

    if (str == "data") {
      b = b && handler.readKeyValueSeparator();
      b = b && handler.readBegin();
      data.clear();
      if (size > 0) {
        size--;
        T tmp;
        for (i=0;i<size;++i) {
          b = b && read(handler,tmp);
          data.insert(tmp);
          b = b && handler.readDataSeparator();
        }
        b = b && read(handler,tmp);
        data.insert(tmp);
      }
    }

    // read all the next end tokens... and ensure consistency
    while (handler.readEnd() && (handler.getLevel() > level)) {
    };

    if (complete) {
      b = b && handler.readEnd();
    }
    return b;
  }

  /**
   * Write function for STL set containers
   *
   * @ingroup gStorable
   */
  template <class T>
  bool write(ioHandler& handler,
             const std::set<T>& data,
             const bool complete=true) {
    int i;
    const int theSize = static_cast<int>(data.size());
    bool b = true;

    if (complete) {
      b = handler.writeBegin();
    }

    b = b && handler.write("size",theSize);

    b = b && handler.writeBegin();
    b = b && handler.writeSymbol(std::string("data"));
    b = b && handler.writeKeyValueSeparator();
    b = b && handler.writeBegin();
    if (theSize > 0) {
      typename std::set<T>::const_iterator it;
      // begin from one to ensure that the last element of the container
      // will NOT be written yet!
      for (i=1,it=data.begin();i<theSize;++it,++i) {
        b = b && write(handler,(*it));
        b = b && handler.writeDataSeparator();
      }
      b = b && write(handler,(*it));
    }
    b = b && handler.writeEnd();
    b = b && handler.writeEnd();

    if (complete) {
      b = b && handler.writeEnd();
    }

    return b;
  }

  // -------------------------------------------------------------------------
  // std::vector
  // -------------------------------------------------------------------------

  /**
   * Read function for STL vector containers.
   * Works only if the type T implements the operator=()
   *
   * @ingroup gStorable
   */
  template <class T>
  bool read(ioHandler& handler,
            std::vector<T>& data,
            const bool complete=true) {

    int i,size;
    bool b = true;

    if (complete) {
      b = handler.readBegin();
    }

    b = b && handler.read("size",size);
    std::string str;

    int level = handler.getLevel();

    b = b && handler.readBegin();
    b = b && handler.readSymbol(str);

    if (str == "data") {
      b = b && handler.readKeyValueSeparator();
      b = b && handler.readBegin();
      data.resize(size);
      if (size > 0) {
        size--;
        for (i=0;i<size;++i) {
          b = b && read(handler,data[i]);
          b = b && handler.readDataSeparator();
        }
        b = b && read(handler,data[i]);
      }
    }

    // read all the next end tokens... and ensure consistency
    while (handler.readEnd() && (handler.getLevel() > level)) {
    }

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

  /**
   * Write function for STL vector containers
   *
   * @ingroup gStorable
   */
  template <class T>
  bool write(ioHandler& handler,
             const std::vector<T>& data,
             const bool complete=true) {
    int i;
    const int theSize = static_cast<int>(data.size());
    bool b = true;

    if (complete) {
      b = handler.writeBegin();
    }

    b = b && handler.write("size",theSize);

    b = b && handler.writeBegin();
    b = b && handler.writeSymbol(std::string("data"));
    b = b && handler.writeKeyValueSeparator();
    b = b && handler.writeBegin();
    if (theSize > 0) {
      typename std::vector<T>::const_iterator it;
      // begin from one to ensure that the last element of the container
      // will NOT be written yet!
      for (i=1,it=data.begin();i<theSize;++it,++i) {
        b = b && write(handler,(*it));
        b = b && handler.writeDataSeparator();
      }
      b = b && write(handler,(*it));
    }
    b = b && handler.writeEnd();
    b = b && handler.writeEnd();

    if (complete) {
      b = b && handler.writeEnd();
    }

    return b;
  }


  // -------------------------------------------------------------------------
  // std::map
  // -------------------------------------------------------------------------

  /**
   * read function for STL map containers
   * Works only if the type U implements the operator=()
   *
   * @ingroup gStorable
   */
  template <class T,class U>
  bool read(ioHandler& handler,std::map<T,U>& data,const bool complete=true) {
    int i,size;
    bool b = true;

    if (complete) {
      b = handler.readBegin();
    }

    b = b && handler.read("size",size);
    std::string str;

    int level = handler.getLevel();

    b = b && handler.readBegin();
    b = b && handler.readSymbol(str);

    if (str == "data") {
      b = b && handler.readKeyValueSeparator();
      b = b && handler.readBegin();
      data.clear();
      if (size > 0) {
        size--;
        std::pair<T,U> tmp;
        for (i=0;i<size;++i) {
          b = b && read(handler,tmp);         
          data.insert(tmp);
          b = b && handler.readDataSeparator();
        }
        b = b && read(handler,tmp);
        data.insert(tmp);
      }
    }

    // read all the next end tokens... and ensure consistency
    while (handler.readEnd() && (handler.getLevel() > level)) {
    }

    if (complete) {
      b = b && handler.readEnd();
    }
    return b;
  }

  /**
   * write function for STL map containers
   *
   * @ingroup gStorable
   */
  template <class T,class U>
  bool write(ioHandler& handler,
             const std::map<T,U>& data,
             const bool complete=true) {
    int i;
    const int theSize = static_cast<int>(data.size());
    bool b = true;

    if (complete) {
      b = handler.writeBegin();
    }

    b = b && handler.write("size",theSize);

    b = b && handler.writeBegin();
    b = b && handler.writeSymbol(std::string("data"));
    b = b && handler.writeKeyValueSeparator();
    b = b && handler.writeBegin();
    if (theSize > 0) {
      typename std::map<T,U>::const_iterator it;
      // begin from one to ensure that the last element of the container
      // will NOT be written yet!
      for (i=1,it=data.begin();i<theSize;++it,++i) {
        b = b && write(handler,(*it));
        b = b && handler.writeDataSeparator();
      }
      b = b && write(handler,(*it));
    }
    b = b && handler.writeEnd();
    b = b && handler.writeEnd();

    if (complete) {
      b = b && handler.writeEnd();
    }

    return b;
  }

  // -------------------------------------------------------------------------
  // std::multimap
  // -------------------------------------------------------------------------


  /**
   * read function for STL multimap containers
   * Works only if the type U implements the operator=()
   *
   * @ingroup gStorable
   */
  template <class T,class U>
  bool read(ioHandler& handler,std::multimap<T,U>& data,
            const bool complete=true) {
    int i,size;
    bool b = true;
    
    if (complete) {
      b = handler.readBegin();
    }

    b = b && handler.read("size",size);
    std::string str;

    int level = handler.getLevel();

    b = b && handler.readBegin();
    b = b && handler.readSymbol(str);

    if (str == "data") {
      b = b && handler.readKeyValueSeparator();
      b = b && handler.readBegin();
      data.clear();
      if (size > 0) {
        size--;
        std::pair<T,U> tmp;
        for (i=0;i<size;++i) {
          b = b && read(handler,tmp);
          data.insert(tmp);
          b = b && handler.readDataSeparator();
        }
        b = b && read(handler,tmp);
        data.insert(tmp);
      }
    }

    // read all the next end tokens... and ensure consistency
    while (handler.readEnd() && (handler.getLevel() > level)) {
    }

    if (complete) {
      b = b && handler.readEnd();
    }
    return b;
  }

  /**
   * write function for STL multimap containers
   *
   * @ingroup gStorable
   */
  template <class T,class U>
  bool write(ioHandler& handler,const std::multimap<T,U>& data,
             const bool complete=true) {
    int i;
    const int theSize = data.size();
    bool b = true;

    if (complete) {
      b = handler.writeBegin();
    }

    b = b && handler.write("size",theSize);

    b = b && handler.writeBegin();
    b = b && handler.writeSymbol(std::string("data"));
    b = b && handler.writeKeyValueSeparator();
    b = b && handler.writeBegin();
    if (theSize > 0) {
      typename std::map<T,U>::const_iterator it;
      // begin from one to ensure that the last element of the container
      // will NOT be written yet!
      for (i=1,it=data.begin();i<theSize;++it,++i) {
        b = b && write(handler,(*it));
        b = b && handler.writeDataSeparator();
      }
      b = b && write(handler,(*it));
    }
    b = b && handler.writeEnd();
    b = b && handler.writeEnd();

    if (complete) {
      b = b && handler.writeEnd();
    }

    return b;
  }


}

#endif
