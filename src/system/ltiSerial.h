/*
 * Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006
 * Lehrstuhl fuer Technische Informatik, RWTH-Aachen, Germany
 *
 * Copyright (C) 2009
 * Pablo Alvarado
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
 * \file   ltiSerial.h
 *         Contains the class lti::serial to provide easy access to serial
 *         ports used in to control several types of devices, like pan/tilt
 *         units, robots, embedded systems, etc.
 * \author Guy Wafo Moudhe
 * \date   11.4.2001
 * \author Pablo Alvarado
 * \date   23.10.2007
 *
 * revisions ..: $Id: ltiSerial.h,v 1.3 2009/10/20 19:09:00 alvarado Exp $
 */


#ifndef _LTI_SERIAL_H_
#define _LTI_SERIAL_H_

#include "ltiIoObject.h"
#include "ltiParametersManager.h"
#include "ltiStatus.h"
#include "ltiVector.h"

#ifdef _LTI_WIN32 // Lars Libuda
// headers required in visual (or perhaps windows?)
#include <windows.h>
#include <winbase.h>

#else

// for posix systems
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <cstdio>

#endif

#include <cstdlib>
#include <string>

namespace lti {
  /**
   * Class to control serial ports.
   *
   * This serial class provides an unified interface to access serial
   * ports in posix and windows operating systems.  At this time only
   * Linux and Windows are supported.
   *
   * You just need to set the parameters if you do not want the default ones,
   * and send or receive data with the send() and receive() methods.
   *
   * If you use a Linux system, do not forget to make the users who
   * needs access to the serial ports to be members of the group
   * <code>uucp</code> or the corresponding group of the
   * <code>/dev/ttyS*</code> devices. 
   *
   * Detailed information of the Posix version can be found at
   * http://www.easysw.com/~mike/serial/serial.html
   *
   * @see serial::parameters
   *
   * @ingroup gSystem
   */
  class serial : public ioObject,
                 public status, 
                 public parametersManager {
  public:

    /**
     * Type to specificy the port to be used.
     */
    enum ePortType {
      Com1=0, /**< First port usually called COM1 in Windows or
                 /dev/ttyS0 in Linux */
      Com2,   /**< Second port usually called COM2 in Windows or
                 /dev/ttyS1 in Linux */
      Com3,   /**< Third port usually called COM3 in Windows or
                 /dev/ttyS2 in Linux */
      Com4,   /**< Fourth port usually called COM4 in Windows or
                 /dev/ttyS3 in Linux */
      Com5,   /**< Fifth port usually called COM5 in Windows or
                 /dev/ttyS4 in Linux */
      Com6,   /**< Sixth port usually called COM6 in Windows or
                 /dev/ttyS5 in Linux */
      Com7,   /**< Seventh port usually called COM7 in Windows or
                 /dev/ttyS6 in Linux */
      Com8,   /**< Eighth port usually called COM8 in Windows or
                 /dev/ttyS7 in Linux */
      USB0,   /**< First serial USB port  */
      USB1    /**< Second serial USB port */
    };
    
    /**
     * Type to specify the baud rate.
     *
     * Baud rate is a measure of the number of times per second a
     * signal in a communications channel varies, or makes a
     * transition between states (states being frequencies, voltage
     * levels, or phase angles). One baud is one such change. Thus,
     * a 1200-baud modem's signal changes state 1200 times each
     * second.  The number of bits transmitted in a second (baudrate
     * in bits-per-second (bps) ) depends on the modulation
     * technique being used.  A 2400 baud modem can transmit 9400
     * bps if there are 4 bits/baud.
     */
    enum eBaudRateType {
      Baud0,      /**<     0 baud */
      Baud300,    /**<   300 baud */
      Baud600,    /**<   600 baud */
      Baud1200,   /**<  1200 baud */
      Baud1800,   /**<  1800 baud */
      Baud2400,   /**<  2400 baud */
      Baud4800,   /**<  4800 baud */
      Baud9600,   /**<  9600 baud */
      Baud19200,  /**< 19200 baud */
      Baud38400,  /**< 38400 baud */
      Baud57600,  /**< 57600 baud */
      Baud76800,  /**< 76800 baud */
      Baud115200  /**<115200 baud */
    };
    
    /**
     * Type to specify the number of bits of a character.
     *
     * The first element Cs4 means for example 4 bits/character.
     */
    enum eCharBitSizeType {
      Cs4=4, /**< 4 bits per character */
      Cs5,   /**< 5 bits per character */
      Cs6,   /**< 6 bits per character */
      Cs7,   /**< 7 bits per character */
      Cs8    /**< 8 bits per character */
    };
    
    /**
     * Definiton of the stop bit.
     *
     * This is also called the remaining bits.  Stop bits serve to
     * synchronise the receiving computer to the incoming characters.
     */
    enum eStopBitType {
      One,     /**< One stop bit             */
      OneFive, /**< One and a half stop bits */
      Two      /**< Two stop bits            */
    };
    
    /**
     * Definition of the parity bit.
     *
     * The optional parity bit is a simple checksum of the data bits
     * indicating whether or not the data contains an even or odd
     * number of "1" bits.
     */
    enum eParityType {
      No,    /**< No parity   */
      Even,  /**< Even parity */
      Odd,   /**< Odd parity  */
      Space, /**< Space       */
      Mark   /**< Mark        */
    };
    
    /**
     * Flow control.
     * 
     * The flow control regulates the data transfer between two serial
     * interfaces.  
     *
     * The flow control establishes if the DTR, CTS and RTS signals should
     * control the information data (so called "hardware" flow control, here
     * RtsCts), or if the Xon/Xoff characters are send (so called "software"
     * flow control, or here XonXoff) to indicate that the computer or the
     * device are ready or not to receive data.
     */
    enum eFlowControl {
      NoFlowControl, /**< Ignore flow control */
      RtsCts,        /**< Use "Hardware" flow control    */
      XonXoff        /**< Use "Software" flow control    */
    };

    /**
     * The parameters for the class serial
     */
    class parameters : public parametersManager::parameters {
    public:
      /**
       * Default constructor
       */
      parameters();

      /**
       * Copy constructor
       * @param other the parameters object to be copied
       */
      parameters(const parameters& other);

      /**
       * Destructor
       */
      virtual ~parameters();

      /**
       * Returns name of this type
       */
      virtual const std::string& name() const;

      /**
       * Copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& copy(const parameters& other);

      /**
       * Copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& operator=(const parameters& other);

      /**
       * Returns a pointer to a clone of the parameters
       */
      virtual parameters* clone() const;

      /**
       * Returns a pointer to a new instance of the parameters
       */
      virtual parameters* newInstance() const;

      /**
       * Write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool write(ioHandler& handler,const bool& complete=true) const;

      /**
       * Read the parameters from the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool read(ioHandler& handler,const bool& complete=true);

      // ------------------------------------------------
      // The attributes of the parameters
      // ------------------------------------------------

      /**
       * Baud rate.
       * Default value: Baud9600
       */
      eBaudRateType baudRate;

      /**
       * Which serial port.
       * Default value: Com1
       */
      ePortType port;

      /**
       * Character size.
       * Default value: Cs8
       */
      eCharBitSizeType characterSize;

      /**
       * Parity.
       * Default value: No
       */
      eParityType parity;

      /**
       * Stop bits
       * Default: One
       */
      eStopBitType stopBits;

      /**
       * Receive timeout in milliseconds.
       *
       * If you specify zero, then no timeout will exist.  The receive method
       * will check if there is something in the buffer.  If so, then it
       * will normally return (reading the first character in the serial FIFO).
       * If there is nothing in the buffer, the receive() method will return
       * inmediatly a "false" value.
       *
       * You should consider that for Linux systems the expected timeout
       * precision is in deciseconds.  The given value will be rounded
       * to the best decisecond precision.  For example 1 ms will be rounded
       * to 0 ms, 80ms will be rounded to 0.1s.
       *
       * Windows OS accepts the ms value.
       *
       * Default value: 5000 (i.e. 5 seconds)
       */
      int receiveTimeout;
      
      /**
       * Flow control
       *
       * The flow control regulates the data transfer between two serial
       * interfaces.
       *
       * The flow control establishes if the DTR, CTS and RTS signals should
       * control the information data (so called "hardware" flow control, here
       * RtsCts), or if the Xon/Xoff characters are send (so called "software"
       * flow control, or here XonXoff) to indicate that the computer or the
       * device are ready or not to receive data.
       *
       * Default value: NoFlowControl
       */
      eFlowControl flowControl;
    };


    /**
     * Default constructor
     */
    serial();

    /**
     * Construct a serial instance with the given parameters.
     */
    serial(const parameters& serialPar);

    /**
     * Copy constructor.
     *
     * @param other the object to be copied
     */
    serial(const serial& other);

    /**
     * Destructor.
     */
    virtual ~serial();

    /**
     * Returns the name of this class
     */
    virtual const std::string& name() const;

    /**
     * Copy data of "other" functor.
     *
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    serial& copy(const serial& other);

    /**
     * Produce a copy of this functor
     */
    serial* clone() const;

    /**
     * Produce a new instance of serial
     */
    serial* newInstance() const;

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * Update parameters
     */
    bool updateParameters();

    /**
     * Sends a character c through the port specified in the
     * parameters.
     *
     * @return true if the character could be send successfully, or
     *              false otherwise.
     */
    bool send(const char c);

    /**
     * Sends an unsigned character c through the port specified in the
     * parameters.
     *
     * @return true if the character could be send successfully, or
     *              false otherwise.
     */
    bool send(const ubyte c);

    /**
     * Sends a string through the port specified in the parameters.
     *
     * @return true if all the string characters are send
     * successfully, or false otherwise.
     */
    bool send(const std::string& theString);

    /**
     * Sends a vector of ubytes
     *
     * Sends all ubytes contained in the vector through the port
     * specified in the parameters.  The vector is more appropriate if
     * the data to be send has a more "binary" nature than an
     * ASCII string.
     *
     * @return true if all the string characters are send
     * successfully, or false otherwise.  
     */
    bool send(const vector<ubyte>& theVector);

    /**
     * Wait for a character to be received in the port specified in
     * the parameters.
     *
     * @return true if the character has been received, or false if a
     * problem occured.  In this case you can read the status string
     * to check the source of the problem.  Note that false will also
     * be returned if a timeout occurs.
     */
    bool receive(char& c);

    /**
     * Wait for an unsigned character to be received in the port specified in
     * the parameters.
     *
     * @return true if the character has been received, or false if a
     * problem occured.  In this case you can read the status string
     * to check the source of the problem.  Note that false will also
     * be returned if a timeout occurs.
     */
    bool receive(ubyte& c);

    /**
     * Read all characters until the next new line or zero character, and
     * store them in the given string.
     */
    bool receive(std::string& theString);

    /**
     * Read exactly theVector.size() characters from the port specified in
     * the parameters.
     *
     * @return true if the character has been received, or false if a
     * problem occured.  In this case you can read the status string
     * to check the source of the problem.  Note that false will also
     * be returned if a timeout occurs.
     */
    bool receive(vector<ubyte>& theVector);

  protected:

    /**
     * Open the port using the parameters
     */
    bool openPort();

  private:

#ifdef _LTI_WIN32
    /**
     * handler used in windows
     */
    HANDLE hHandle_;

    /**
     * Sets and query the time-out parameters for a communications device
     */
    COMMTIMEOUTS timeOut_;
#else

    /**
     * File descriptor for the port in POSIX.
     * get the value -1 in case of error,while opening the port
     */
    int hHandle_;

#endif

    /**
     * Contains true if the port is opened, else false
     */
    bool isPortOpened_;

    /**
     * Close the port
     */
    bool closePort();
  };

  /**
   * Read a serial::ePortType
   *
   * @ingroup gStorable
   */
  bool read(ioHandler& handler,
            serial::ePortType& data);

  /**
   * Write a serial::ePortType
   *
   * @ingroup gStorable
   */
  bool write(ioHandler& handler,
             const serial::ePortType& data);

  /**
   * Read a serial::eBaudRateType
   *
   * @ingroup gStorable
   */
  bool read(ioHandler& handler,
            serial::eBaudRateType& data);

  /**
   * Write a serial::eBaudRateType
   *
   * @ingroup gStorable
   */
  bool write(ioHandler& handler,
             const serial::eBaudRateType& data);

  /**
   * Read a serial::eCharBitSizeType
   *
   * @ingroup gStorable
   */
  bool read(ioHandler& handler,
            serial::eCharBitSizeType& data);

  /**
   * Write a serial::eCharBitSizeType
   *
   * @ingroup gStorable
   */
  bool write(ioHandler& handler,
             const serial::eCharBitSizeType& data);

  /**
   * Read a serial::eStopBitType
   *
   * @ingroup gStorable
   */
  bool read(ioHandler& handler,
            serial::eStopBitType& data);

  /**
   * Write a serial::eStopBitType
   *
   * @ingroup gStorable
   */
  bool write(ioHandler& handler,
             const serial::eStopBitType& data);

  /**
   * Read a serial::eParityType
   *
   * @ingroup gStorable
   */
  bool read(ioHandler& handler,
            serial::eParityType& data);

  /**
   * Write a serial::eParityType
   *
   * @ingroup gStorable
   */
  bool write(ioHandler& handler,
             const serial::eParityType& data);


}

#endif
