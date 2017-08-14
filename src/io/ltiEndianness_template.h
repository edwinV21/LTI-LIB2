/*
 * Copyright (C) 1998-2006
 * Lehrstuhl fuer Technische Informatik, RWTH-Aachen, Germany
 * Instituto Tecnológico de Costa Rica, Costa Rica
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
 * \file   ltiEndianness_template.h
 *         Contains the class endianness, which allows to read/write data from
 *         a file written with a known endianness.
 * \author Pablo Alvarado
 * \date   11.01.2006
 *
 */


namespace lti {

  template <eEndiannessType E>
  endianness<E>::endianness() {
  }

  template <eEndiannessType E>
  endianness<E>::~endianness() {
  }
  
  template <eEndiannessType E>
  bool endianness<E>::read(std::istream& in,byte& data) {
    in.read(reinterpret_cast<char*>(&data),1);
    return in.good();
  }

  template <eEndiannessType E>
  bool endianness<E>::read(std::istream& in,ubyte& data) {
    in.read(reinterpret_cast<char*>(&data),1);
    return in.good(); 
  }
  
  template <eEndiannessType E>
  bool endianness<E>::read(std::istream& in,int16& data) {
    in.read(reinterpret_cast<char*>(&data),2);
    return in.good();    
  }
    
  template <eEndiannessType E>
  bool endianness<E>::read(std::istream& in,uint16& data) {
    in.read(reinterpret_cast<char*>(&data),2);
    return in.good();    
  }
  
  template <eEndiannessType E>
  bool endianness<E>::read(std::istream& in,int32& data) {
    in.read(reinterpret_cast<char*>(&data),4);
    return in.good();
  }
  
  template <eEndiannessType E>
  bool endianness<E>::read(std::istream& in,uint32& data) {
    in.read(reinterpret_cast<char*>(&data),4);
    return in.good();
  }
  
  template <eEndiannessType E>
  bool endianness<E>::write(std::ostream& out,const byte& data) {
    out.write(reinterpret_cast<const char*>(&data),1);
    return out.good();
  }
  
  template <eEndiannessType E>
  bool endianness<E>::write(std::ostream& out,const ubyte& data) {
    out.write(reinterpret_cast<const char*>(&data),1);
    return out.good();
  }
  
  template <eEndiannessType E>
  bool endianness<E>::write(std::ostream& out,const int16& data) {
    out.write(reinterpret_cast<const char*>(&data),2);
    return out.good();
  }
  
  template <eEndiannessType E>
  bool endianness<E>::write(std::ostream& out,const uint16& data) {
    out.write(reinterpret_cast<const char*>(&data),2);
    return out.good();
  }
  
  template <eEndiannessType E>
  bool endianness<E>::write(std::ostream& out,const int32& data) {
    out.write(reinterpret_cast<const char*>(&data),4);
    return out.good();
  }
  
  template <eEndiannessType E>
  bool endianness<E>::write(std::ostream& out,const uint32& data) {
    out.write(reinterpret_cast<const char*>(&data),4);
    return out.good();
  }

  // -------------------------------------------------------------------------
  //
  // -------------------------------------------------------------------------

#undef _LTI_OPENDIAN
#ifdef WORDS_BIGENDIAN
  #define _LTI_OPENDIAN LittleEndian 
#else
  #define _LTI_OPENDIAN BigEndian
#endif

  template <> bool endianness<_LTI_OPENDIAN>::
  read(std::istream& in,int16& data) {
    char buffer[2];
    char* alias = reinterpret_cast<char*>(&data);
    in.read(buffer,2);
    alias[0]=buffer[1];
    alias[1]=buffer[0];
    return in.good();    
  }

  template <> bool endianness<_LTI_OPENDIAN>::
  read(std::istream& in,uint16& data) {
    char buffer[2];
    char* alias = reinterpret_cast<char*>(&data);
    in.read(buffer,2);
    alias[0]=buffer[1];
    alias[1]=buffer[0];
    return in.good();    
  }

  template <> bool endianness<_LTI_OPENDIAN>::
  read(std::istream& in,int32& data) {
    char buffer[4];
    char* alias = reinterpret_cast<char*>(&data);
    in.read(buffer,4);
    alias[0]=buffer[3];
    alias[1]=buffer[2];
    alias[2]=buffer[1];
    alias[3]=buffer[0];
    return in.good();    
  }

  template <> bool endianness<_LTI_OPENDIAN>::
  read(std::istream& in,uint32& data) {
    char buffer[4];
    char* alias = reinterpret_cast<char*>(&data);
    in.read(buffer,4);
    alias[0]=buffer[3];
    alias[1]=buffer[2];
    alias[2]=buffer[1];
    alias[3]=buffer[0];
    return in.good();    
  }

  template <> bool endianness<_LTI_OPENDIAN>::
  write(std::ostream& out,const int16& data) {
    char buffer[2];
    const char* alias = reinterpret_cast<const char*>(&data);
    buffer[0]=alias[1];
    buffer[1]=alias[0];
    
    out.write(buffer,2);
    return out.good();    
  }

  template <> bool endianness<_LTI_OPENDIAN>::
  write(std::ostream& out,const uint16& data) {
    char buffer[2];
    const char* alias = reinterpret_cast<const char*>(&data);
    buffer[0]=alias[1];
    buffer[1]=alias[0];
    
    out.write(buffer,2);
    return out.good();    
  }

  template <> bool endianness<_LTI_OPENDIAN>::
  write(std::ostream& out,const int32& data) {
    char buffer[4];
    const char* alias = reinterpret_cast<const char*>(&data);
    buffer[0]=alias[3];
    buffer[1]=alias[2];
    buffer[2]=alias[1];
    buffer[3]=alias[0];

    out.write(buffer,4);
    return out.good();    
  }

  template <> bool endianness<_LTI_OPENDIAN>::
  write(std::ostream& out,const uint32& data) {
    char buffer[4];
    const char* alias = reinterpret_cast<const char*>(&data);
    buffer[0]=alias[3];
    buffer[1]=alias[2];
    buffer[2]=alias[1];
    buffer[3]=alias[0];
    
    out.write(buffer,4);
    return out.good();    
  }
  

}

