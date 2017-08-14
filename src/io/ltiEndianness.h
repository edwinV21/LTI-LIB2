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
 * \file   ltiEndianness.h
 *         Contains the class endianness, which allows to read/write data from
 *         a file written with a known endianness.
 * \author Pablo Alvarado
 * \date   11.01.2006
 *
 */

#ifndef _LTI_ENDIANNESS_H_
#define _LTI_ENDIANNESS_H_

#include <iostream>
#include "ltiTypes.h"
#include "ltiConfig.h"

namespace lti {

  /**
   * Endianness naming constants
   */
  enum eEndiannessType {    
    LittleEndian, /**< Little Endian (Like in Intel x86 Processors)*/
    BigEndian     /**< Big Endian (Like in IBM PowerPC Processors) */
  };

  /**
   * Endianness.
   *
   * This template class is used to read from or write data to a file that was
   * created with a known endianness.  For example, Windows BMP image files
   * have a header with some integers written with little endianness.
   *
   * To be more specific, the problem this class tries to solve is the
   * following:
   *
   * Let us assume we have a binary file (and not an ASCII file) with
   * following byte stream:
   *
   * 0x01 0x02 0x03 0x04 ...
   *
   * and we use following code to read an 32 bit integer (i.e. 4 bytes long):
   *
   * \code
   *   lti::uint32 integer;             // hold the values been readed
   *   std::istream in("theFile.dat");  // the input stream
   *
   *  ...
   *  in.read(&integer,4);             // reads 4 bytes on "integer"
   * \endcode
   *
   * With a little endian system (for example Intel Pentium), the
   * contents of \c integer will be 0x04030201.
   *
   * With a big endian system (for example Motorola/IBM PowerPC), the
   * contents of \c integer will be 0x01020304.
   *
   * We therefore need a class, which allow us to load data from a file with
   * the correct endiannes!
   *
   * The detection of the system endianness will be done at configuration time
   * of the library.:
   *
   * - For Windows systems there is no problem if they run on Intel Processors
   *   (always little-endian).  If you need to change this (for example you
   *   have an Alpha processor), change the endianness on the file 
   *   ltiWinConfig.h, ensuring that the symbol <code>WORDS_BIGENDIAN</code>
   *   is defined as 1.
   *
   * - For Unix systems the processor and endianness are determined
   *   automatically by the configure script.  If the file config.h exists,
   *   everything should be ok!!  In case you really need to change the
   *   detected configuration, in config.h the symbol
   *   <code>WORDS_BIGENDIAN</code> is defined only in big endian systems.
   *
   * Template behaviour.
   * 
   * The class lti::endianness is a template class of an enumerate that can be
   * LittleEndian or BigEndian, defined in the lti namespace.
   *
   * lti::endianness<lti::LittleEndian>
   * lti::endianness<lti::BigEndian>
   *
   * You can check for the endianness used as default by your system with
   * systemEndianness().
   *
   * Please note that all methods are static, so that you don't need an
   * instance of the class to use them.
   */
  template <eEndiannessType E>
  class endianness {
  public:    
    /** 
     * Default constructor.
     */
    endianness();

    /**
     * Destructor
     */
    ~endianness();

    /**
     * Read a byte from the stream
     * @param in input stream
     * @param data variable where the data should be stored
     * @return a reference to the variable with the readed data
     */
    static bool read(std::istream& in,byte& data);

    /**
     * Read an unsigned byte from the stream
     * @param in input stream
     * @param data variable where the data should be stored
     * @return a reference to the variable with the readed data
     */
    static bool read(std::istream& in,ubyte& data);
    
    /**
     * Read a int16 (2 bytes)
     * @param in input stream
     * @param data variable where the data should be stored
     * @return a reference to the variable with the readed data
     */
    static bool read(std::istream& in,int16& data);

    /**
     * Read a uint16 (2 bytes)
     * @param in input stream
     * @param data variable where the data should be stored
     * @return a reference to the variable with the readed data
     */
    static bool read(std::istream& in,uint16& data);
    
    /**
     * Read a int32
     * @param in input stream
     * @param data variable where the data should be stored
     * @return a reference to the variable with the readed data
     */
    static bool read(std::istream& in,int32& data);

    /**
     * Read a uint32
     * @param in input stream
     * @param data variable where the data should be stored
     * @return a reference to the variable with the readed data
     */
    static bool read(std::istream& in,uint32& data);

    /**
     * Write a byte
     * @param out output stream
     * @param data variable with the data to be stored on the file
     * @return a reference to the variable with the data
     */
    static bool write(std::ostream& out,const byte& data);
    
    /**
     * Write a ubyte
     * @param out output stream
     * @param data variable with the data to be stored on the file
     * @return a reference to the variable with the data
     */
    static bool write(std::ostream& out,const ubyte& data);
    
    /**
     * Write a int16 (2 bytes)
     * @param out output stream
     * @param data variable with the data to be stored on the file
     * @return a reference to the variable with the data
     */
    static bool write(std::ostream& out,const int16& data);

    /**
     * Write a uint16 (2 bytes)
     * @param out output stream
     * @param data variable with the data to be stored on the file
     * @return a reference to the variable with the data
     */
    static bool write(std::ostream& out,const uint16& data);
    
    /**
     * Write a int32
     * @param out output stream
     * @param data variable with the data to be stored on the file
     * @return a reference to the variable with the data
     */
    static bool write(std::ostream& out,const int32& data);

    /**
     * Write a uint32
     * @param out output stream
     * @param data variable with the data to be stored on the file
     * @return a reference to the variable with the data
     */
    static bool write(std::ostream& out,const uint32& data);
  };
}

#endif
