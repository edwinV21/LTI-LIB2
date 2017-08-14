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
 * \file   ltiSerial.cpp
 *         Contains the class lti::serial to provide easy access to serial
 *         ports used in to control several types of devices, like pan/tilt
 *         units, robots, embedded systems, etc.
 * \author Guy Wafo Moudhe
 * \date   11.4.2001
 * \author Pablo Alvarado
 * \date   23.10.2007
 *
 * revisions ..: $Id: ltiSerial.cpp,v 1.4 2009/12/15 15:14:12 alvarado Exp $
 */

#include "ltiSerial.h"
#include "ltiMath.h"
#include "ltiRound.h"
#include <string>
#include <cstring>

#ifdef _LTI_WIN32
#include <winnt.h>
#endif

#undef _LTI_DEBUG
// #define _LTI_DEBUG 1
#include "ltiDebug.h"

#ifdef _LTI_DEBUG

#include <iostream>
#endif

namespace lti {

  // --------------------------------------------------
  // serial::parameters
  // --------------------------------------------------

  // default constructor
  serial::parameters::parameters() : parametersManager::parameters() {
    baudRate = Baud9600;
    port = Com1;
    characterSize = Cs8;
    parity = No;
    stopBits = One;
    receiveTimeout = 5000; // five seconds
    flowControl = NoFlowControl;
  }

  // copy constructor
  serial::parameters::parameters(const parameters& other) 
    : parametersManager::parameters() {
    copy(other);
  }

  // destructor
  serial::parameters::~parameters() {
  }

  // get type name
  const std::string& serial::parameters::name() const {
    _LTI_RETURN_CLASS_NAME;
  }

  // copy member

  serial::parameters& serial::parameters::copy(const parameters& other) {

    baudRate = other.baudRate;
    port = other.port;
    characterSize = other.characterSize;
    parity = other.parity;
    stopBits = other.stopBits;
    receiveTimeout=other.receiveTimeout;
    flowControl=other.flowControl;

    return *this;
  }

  // alias for copy member
  serial::parameters&
  serial::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  // clone member
  serial::parameters* serial::parameters::clone() const {
    return new parameters(*this);
  }

  // clone member
  serial::parameters* serial::parameters::newInstance() const {
    return new parameters;
  }

  /*
   * write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool serial::parameters::write(ioHandler& handler,
                                         const bool& complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {
      b=lti::write(handler,"baudRate"      ,baudRate)&&b;
      b=lti::write(handler,"port"          ,port)&&b;
      b=lti::write(handler,"characterSize" ,characterSize)&&b;
      b=lti::write(handler,"parity"        ,parity)&&b;
      b=lti::write(handler,"stopBits"      ,stopBits)&&b;
      b=lti::write(handler,"receiveTimeout",receiveTimeout)&&b;
      b=lti::write(handler,"flowControl"   ,flowControl)&&b;
    }

    if (complete) {
      b = b && handler.writeEnd();
    }

    return b;
  }

  /*
   * read the parameters from the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also read, otherwise only the data block will be read.
   * @return true if write was successful
   */
  bool serial::parameters::read(ioHandler& handler,
                                const bool& complete) {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {
      b=lti::read(handler,"baudRate"      ,baudRate)&&b;
      b=lti::read(handler,"port"          ,port)&&b;
      b=lti::read(handler,"characterSize" ,characterSize)&&b;
      b=lti::read(handler,"parity"        ,parity)&&b;
      b=lti::read(handler,"stopBits"      ,stopBits)&&b;
      b=lti::read(handler,"receiveTimeout",receiveTimeout)&&b;
      b=lti::read(handler,"flowControl"   ,flowControl)&&b;
    }

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

  // --------------------------------------------------
  // serial
  // --------------------------------------------------

  // default constructor
  serial::serial() : ioObject(),status(),parametersManager() {

    isPortOpened_=false;

    // create an instance of the parameters with the default values
    parameters defaultParameters;
    // set the default parameters
    setParameters(defaultParameters);
  }

  // default constructor
  serial::serial(const serial::parameters& serialPar)
    : ioObject(),status(),parametersManager() {

    isPortOpened_=false;

    // set the default parameters
    setParameters(serialPar);
  }

  // copy constructor
  serial::serial(const serial& other) 
    : ioObject(),status(),parametersManager() {
    copy(other);
  }

  // destructor
  serial::~serial() {
  }

  // returns the name of this type
  const std::string& serial::name() const {
    _LTI_RETURN_CLASS_NAME;
  }


  // copy member
  serial& serial::copy(const serial& other) {
    // first close the current port, before we acquire the next one
    closePort();
    parametersManager::copy(other);
    return *this;
  }

  // clone member
  serial* serial::clone() const {
    return new serial(*this);
  }  

  // new instance
  serial* serial::newInstance() const {
    return new serial;
  }

  // return parameters
  const serial::parameters& serial::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&parametersManager::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }

  bool serial::updateParameters() {
    // As soon as the user sets its parameters, try to configure the port.
    // If it does not work, return an error.
    return openPort();
  }

  bool serial::openPort() {
    const parameters& param = getParameters();

    if(isPortOpened_){
      closePort();
    }

#ifdef _LTI_WIN32 // Lars Libuda
    const char* portName;

    switch(param.port){
        case Com1 :  portName = "COM1"; break;
        case Com2 :  portName = "COM2"; break;
        case Com3 :  portName = "COM3"; break;
        case Com4 :  portName = "COM4"; break;
        case Com5 :  portName = "COM5"; break;
        case Com6 :  portName = "COM6"; break;
        case Com7 :  portName = "COM7"; break;
        case Com8 :  portName = "COM8"; break;
        case USB0 :  portName = "USB0"; break;
        case USB1 :  portName = "USB1"; break;
        default:                 portName = "COM1"; break;
    };

    DCB	dcb; // Device Control Block

    BOOL ioError = 0;

    // opening serial interface
    hHandle_=CreateFile((LPCWSTR)portName, // the port name  // Lars Libuda
                        GENERIC_READ | GENERIC_WRITE,
                        0,0,
                        OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0); // nonoverlaped

    if (hHandle_ != INVALID_HANDLE_VALUE) {
      // in case of success

      if(GetCommState(hHandle_,&dcb)) {  // fetch DeviceControllBlock

        switch(param.baudRate) {

            case Baud600    : dcb.BaudRate = CBR_600;    break;
            case Baud1200   : dcb.BaudRate = CBR_1200;   break;
            case Baud2400   : dcb.BaudRate = CBR_2400;   break;
            case Baud4800   : dcb.BaudRate = CBR_4800;   break;
            case Baud9600   : dcb.BaudRate = CBR_9600;   break;
            case Baud19200  : dcb.BaudRate = CBR_19200;  break;
            case Baud38400  : dcb.BaudRate = CBR_38400;  break;
            case Baud57600  : dcb.BaudRate = CBR_57600;  break;
            case Baud115200 : dcb.BaudRate = CBR_115200; break;
            default :                     dcb.BaudRate = CBR_9600;   break;
        }

        dcb.ByteSize = static_cast<unsigned char>(param.characterSize);

        switch(param.parity) {

            case Odd   : dcb.Parity=ODDPARITY;   break;
            case Even  : dcb.Parity=EVENPARITY;  break;
            case Mark  : dcb.Parity=MARKPARITY;  break;
            case Space : dcb.Parity=SPACEPARITY; break;
            case No    : dcb.Parity=NOPARITY;    break;
            default                : dcb.Parity=NOPARITY;    break;
        }

        switch(param.stopBits){

          case One    : dcb.StopBits = ONESTOPBIT;   break;
          case OneFive: dcb.StopBits = ONE5STOPBITS; break;
          case Two    : dcb.StopBits = TWOSTOPBITS;  break;
          default     : dcb.StopBits = ONESTOPBIT;   break;
        }

        switch(param.flowControl) {
        case NoFlowControl: 
          // WARNING: This code has not been tested yet! (091020)
          dcb.fOutX = FALSE; // Disable Xon/Xoff for transmission
          dcb.fInX  = FALSE; // Disable Xon/Xoff for reception
          
          dcb.fDtrControl  = DTR_CONTROL_DISABLE; // Disable Data Terminal Ready
          dcb.fOutxCtsFlow = FALSE;               // Disable Clear To Send
          dcb.fOutxDsrFlow = FALSE;               // Disable Data Set Ready
          dcb.fRtsControl  = RTS_CONTROL_DISABLE; // Disable Ready to Send

          break;

        case RtsCts:
          // WARNING: This code has not been tested yet! (091020)
          //          No windows devices available to control this out
          dcb.fOutX = FALSE; // Disable Xon/Xoff for transmission
          dcb.fInX  = FALSE; // Disable Xon/Xoff for reception

          dcb.fDtrControl  = DTR_CONTROL_HANDSHAKE;// Enable Data Terminal Ready
          dcb.fOutxCtsFlow = TRUE;                 // Enable Clear To Send
          dcb.fOutxDsrFlow = TRUE;                 // Enable Data Set Ready
          dcb.fRtsControl  = RTS_CONTROL_HANDSHAKE;// Enable Ready to Send
          break;

        case XonXoff:
          // WARNING: This code has not been tested yet! (091020)
          dcb.fOutX = TRUE; // Enable Xon/Xoff for transmission
          dcb.fInX  = TRUE; // Enable Xon/Xoff for reception
          
          dcb.fDtrControl  = DTR_CONTROL_DISABLE; // Disable Data Terminal Ready
          dcb.fOutxCtsFlow = FALSE;               // Disable Clear To Send
          dcb.fOutxDsrFlow = FALSE;               // Disable Data Set Ready
          dcb.fRtsControl  = RTS_CONTROL_DISABLE; // Disable Ready to Send

          break;
        }
        
        if(!SetCommState(hHandle_,&dcb)) {    // set device settimgs
          closePort(); // in case of invalid settings : close port
          return false;
        }
      } else {
        // some error occured

        setStatusString("Can not access the control setting parameters");
        closePort(); // DCB not accessible, close port
        return false;
      }

    } else {
      // handle invalid
      setStatusString("The port could not be opened");
      return false;	 // COMx could not be opened, terminate thread
    }

    timeOut_.ReadIntervalTimeout = MAXDWORD;
    timeOut_.ReadTotalTimeoutMultiplier = MAXDWORD;
    timeOut_.ReadTotalTimeoutConstant = param.receiveTimeout; // in ms
    timeOut_.WriteTotalTimeoutMultiplier = 0;
    timeOut_.WriteTotalTimeoutConstant = 0;

    ioError = SetCommTimeouts(hHandle_, &timeOut_);
    if (ioError == 0) {
      setStatusString("Time out could not be set");
      return false;
    }

    SetCommMask(hHandle_,EV_RXCHAR);

    _lti_debug(" The port: Port " << portName << 
               " was opened successfully!" << endl);

#else
    // POSIX Version:

    struct termios allParameters;

    // original flags
    int flags = O_RDWR | O_NOCTTY; // | O_NDELAY
    
    // flags used in serial of robotic group
    // int flags = O_CREAT|O_RDWR|O_NONBLOCK

    switch(param.port) {
        case Com1: hHandle_=open("/dev/ttyS0",flags); break;
        case Com2: hHandle_=open("/dev/ttyS1",flags); break;
        case Com3: hHandle_=open("/dev/ttyS2",flags); break;
        case Com4: hHandle_=open("/dev/ttyS3",flags); break;
        case Com5: hHandle_=open("/dev/ttyS4",flags); break;
        case Com6: hHandle_=open("/dev/ttyS5",flags); break;
        case Com7: hHandle_=open("/dev/ttyS6",flags); break;
        case Com8: hHandle_=open("/dev/ttyS7",flags); break;
        case USB0: hHandle_=open("/dev/ttyUSB0",flags); break;
        case USB1: hHandle_=open("/dev/ttyUSB1",flags); break;
        default:               hHandle_=open("/dev/ttyS0",flags); break;
    }

    if(hHandle_ != -1) {

      //get the current parameters for the port
      if (tcgetattr(hHandle_,&allParameters) != -1) {

        // first than all, clear everything!
        allParameters.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP
                                   |INLCR|IGNCR|ICRNL);
        // Raw output option,similary as the raw input.The characters
        // are transmitted as-is
        allParameters.c_oflag &= ~OPOST;
        allParameters.c_lflag &= ~(ECHO|ECHONL|ICANON|ISIG|IEXTEN);
        allParameters.c_cflag &= ~(CSIZE|PARENB);

        // now set the parameters chosen by the user:
        switch(param.baudRate) {

            case Baud0:
              cfsetispeed(&allParameters,B0);
              cfsetospeed(&allParameters,B0);
              break;
            case Baud300:
              cfsetispeed(&allParameters,B300);
              cfsetospeed(&allParameters,B300);
              break;
            case Baud600:
              cfsetispeed(&allParameters,B600);
              cfsetospeed(&allParameters,B600);
              break;
            case Baud1200:
              cfsetispeed(&allParameters,B1200);
              cfsetospeed(&allParameters,B1200);
              break;
            case Baud1800:
              cfsetispeed(&allParameters,B1800);
              cfsetospeed(&allParameters,B1800);
              break;
            case Baud2400:
              cfsetispeed(&allParameters,B2400);
              cfsetospeed(&allParameters,B2400);
              break;
            case Baud4800:
              cfsetispeed(&allParameters,B4800);
              cfsetospeed(&allParameters,B4800);
              break;
            case Baud9600:
              cfsetispeed(&allParameters,B9600);
              cfsetospeed(&allParameters,B9600);
              break;
            case Baud19200:
              cfsetispeed(&allParameters,B19200);
              cfsetospeed(&allParameters,B19200);
              break;
            case Baud38400:
              cfsetispeed(&allParameters,B38400);
              cfsetospeed(&allParameters,B38400);
              break;
            case Baud57600:
              cfsetispeed(&allParameters,B57600);
              cfsetospeed(&allParameters,B57600);
              break;
            case Baud76800:
              setStatusString("Baud76800 not supported");
              return false;
              break;
            case Baud115200:
              cfsetispeed(&allParameters,B115200);
              cfsetospeed(&allParameters,B115200);
              break;
            default:
              cfsetispeed(&allParameters,B9600);
              cfsetospeed(&allParameters,B9600);
        }

        switch(param.parity) {

            case Odd:
              allParameters.c_cflag |= PARENB; // enable parity
              allParameters.c_cflag |= PARODD; // odd parity
              allParameters.c_iflag |= (INPCK | ISTRIP);
              break;

            case Even:
              allParameters.c_cflag |= PARENB;  // enable parity
              allParameters.c_cflag &= ~PARODD; // even parity
              allParameters.c_iflag |= (INPCK | ISTRIP);
              break;

            case Space:
            case No:
              allParameters.c_cflag &= ~PARENB;
              break;
            default:
              allParameters.c_cflag &= ~PARENB;
        }

        allParameters.c_cflag &= ~CSIZE;    // mask the character size bits

        switch(param.characterSize){
            case Cs5: allParameters.c_cflag |=CS5; break;
            case Cs6: allParameters.c_cflag |=CS6; break;
            case Cs7: allParameters.c_cflag |=CS7; break;
            case Cs8: allParameters.c_cflag |=CS8; break;
            default:              allParameters.c_cflag |=CS8; break;
        }

        if (param.stopBits == Two) { //if the stop bit is
           //set,2 stop bits are used.Otherwise,only 1 stop bit is used.
          allParameters.c_cflag |= CSTOPB;   //set the stop bit.
        } else {
          allParameters.c_cflag &= ~CSTOPB;   //clear the 2 stop bit.
        }

        // These ones should always be set:
        allParameters.c_cflag |= CREAD;  // enable receiver
        allParameters.c_cflag |= CLOCAL; // Local line -
                                         // do not change "owner" of port

        switch(param.flowControl) {
        case NoFlowControl:
          // software flow control disabled
          allParameters.c_iflag &= ~(IXON | IXOFF | IXANY);

#ifdef CNEW_RTSCTS
          allParameters.c_cflag &= ~CNEW_RTSCTS;
#elifdef CRTSCTS
          allParameters.c_cflag &= ~CRTSCTS;
#endif

          break;
        case RtsCts:
          allParameters.c_iflag &= ~(IXON | IXOFF | IXANY);

#ifdef CNEW_RTSCTS
          allParameters.c_cflag |= CNEW_RTSCTS;
#elifdef CRTSCTS
          allParameters.c_cflag |= CRTSCTS;
#endif
          break;
        case XonXoff:
          allParameters.c_iflag |= (IXON | IXOFF);
          allParameters.c_iflag &= ~(IXANY);

#ifdef CNEW_RTSCTS
          allParameters.c_cflag &= ~CNEW_RTSCTS;
#elifdef CRTSCTS
          allParameters.c_cflag &= ~CRTSCTS;
#endif

          break;
        }

        // Raw input.
        // Input characters are passed through exactly as they are received
        allParameters.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG | IEXTEN);

        //
        // Taken from: http://www.easysw.com/~mike/serial/serial.html#2_5_2
        //
        // VMIN specifies the minimum number of characters to read. If
        // it is set to 0, then the VTIME value specifies the time to
        // wait for every character read. Note that this does not mean
        // that a read call for N bytes will wait for N characters to
        // come in. Rather, the timeout will apply to the first
        // character and the read call will return the number of
        // characters immediately available (up to the number you
        // request).
        //
        // If VMIN is non-zero, VTIME specifies the time to wait for
        // the first character read. If a character is read within the
        // time given, any read will block (wait) until all VMIN
        // characters are read. That is, once the first character is
        // read, the serial interface driver expects to receive an
        // entire packet of characters (VMIN bytes total). If no
        // character is read within the time allowed, then the call to
        // read returns 0. This method allows you to tell the serial
        // driver you need exactly N bytes and any read call will
        // return 0 or N bytes. However, the timeout only applies to
        // the first character read, so if for some reason the driver
        // misses one character inside the N byte packet then the read
        // call could block forever waiting for additional input
        // characters.
        //
        // VTIME specifies the amount of time to wait for incoming
        // characters in tenths of seconds. If VTIME is set to 0 (the
        // default), reads will block (wait) indefinitely unless the
        // NDELAY option is set on the port with open or fcntl.

        // number of characters to be received (zero means wait until anything
        // comes!
        allParameters.c_cc[VMIN]=0;
        //time to wait in deciseconds to receive every character,while VMIN=0
        allParameters.c_cc[VTIME]=iround(param.receiveTimeout/100.0);     

        tcflush(hHandle_, TCIFLUSH);

        if (tcsetattr(hHandle_,TCSANOW,&allParameters)==-1) {
          //set all the parameters without waiting for data to complete
          setStatusString("Could not set the parameters");
          closePort();
          return false;
        }
      } else {
        setStatusString("Unable to get the current parameters");
        closePort();
        return false;
      }
    } else {
      setStatusString(strerror(errno));
      closePort();
      return false;
    }

    fcntl(hHandle_,F_SETFL,0);

#endif

    isPortOpened_=true;
    return isPortOpened_;
  }

  //sends only one character
  bool serial::send(const unsigned char c) {
    return send(static_cast<char>(c));
  }

  bool serial::send(const char c) {

    if (!isPortOpened_){
      if (!openPort()) {
        // some error occured by opening port
        return false;
      }
    }

#ifdef _LTI_WIN32

    BOOL ioError = 0;
    DWORD numberOfBytes;

    ioError = WriteFile(hHandle_,&c,1,&numberOfBytes,0);
    if (ioError == 0 || numberOfBytes!=1) {
      setStatusString("Error while writing a character to the serial port");
      return false;
    }

    return true;

#else
    int numberOfBytes;
    numberOfBytes = ::write(hHandle_,&c,1);
    if(numberOfBytes==1) {
      return true;
    } else {
      setStatusString("Could not send a character");
      return false;
    }
#endif
  }

  //sends a string
  bool serial::send(const std::string& theString) {

    if(!isPortOpened_) {
      if (!openPort()) {
        // some error occured by opening port
        return false;
      }
    }

    int counter=0;

    while((counter < static_cast<int>(theString.size())) &&
          (send(theString[counter])==true)) {
      counter++;
    }

    return (counter == static_cast<int>(theString.size()));
  }

  //sends a string
  bool serial::send(const vector<ubyte>& theString) {

    if(!isPortOpened_) {
      if (!openPort()) {
        // some error occured by opening port
        return false;
      }
    }

    int counter=0;

    while((counter < theString.size()) &&
          (send(theString[counter])==true)) {
      counter++;
    }

    return (counter == static_cast<int>(theString.size()));
  }

  bool serial::receive(unsigned char& c) {
    char* tmp = reinterpret_cast<char*>(&c);
    return receive(*tmp);
  }

  //receives only one character
  bool serial::receive(char& c) {

    if (!isPortOpened_) {
      if (!openPort()) {
        // some error occured by opening port
        return false;
      }
    }

#ifdef _LTI_WIN32
    BOOL ioError;
    DWORD numberOfBytes;
    ioError=ReadFile(hHandle_,&c,1,&numberOfBytes,0);

    if (ioError==0 || numberOfBytes != 1) {
      c = 0;
      setStatusString("Could not receive the character");
      return false;
    }

    return true;
#else

    int numberOfBytes;
    numberOfBytes = ::read(hHandle_,&c,1);

    if (numberOfBytes==1) {
      _lti_debug("{"<<static_cast<int>(static_cast<unsigned char>(c))<<"}");
      return true;
    } else {
      setStatusString("Could not receive the character");
      return false;
    }
#endif
  }

  //receives a string
  bool serial::receive(std::string& theString) {

    bool control=false;
    char ch;

    theString = "";

    do {
      if ( (control = receive(ch)) ) {
        theString += ch;
      }
    } while ((ch!='\n') && (ch!=0) && control);

    return control;
  }

  // receives a string
  bool serial::receive(vector<ubyte>& theString) {

    bool control=true;
    char ch;

    int i=0;
    for (i=0;control && (i<theString.size());++i) {
      if ( (control = receive(ch)) ) {
        theString[i] = static_cast<ubyte>(ch);
      }
    }

    return control;
  }


  // close the port
  bool serial::closePort() {
    bool result=true;
#ifdef _LTI_WIN32
    if (CloseHandle(hHandle_)==0) {
      setStatusString("The port could not be closed");
      result=false;
    }
    hHandle_ = INVALID_HANDLE_VALUE; // an invalid handle
#else
    if(close(hHandle_)==-1) {
      setStatusString("The port could not be closed");
      result=false;
    }
    hHandle_=-1;
#endif
    isPortOpened_=false;
    return result;
  }

  /**
   * Write a serial::eBaudRateType
   *
   * @ingroup gStorable
   */
  bool write(ioHandler& handler,
             const serial::eBaudRateType& data) {

    switch (data) {
    case serial::Baud0:
      return handler.write("Baud0");
      break;
    case serial::Baud300:
      return handler.write("Baud300");
      break;
    case serial::Baud600:
      return handler.write("Baud600");
      break;
    case serial::Baud1200:
      return handler.write("Baud1200");
      break;
    case serial::Baud1800:
      return handler.write("Baud1800");
      break;
    case serial::Baud2400:
      return handler.write("Baud2400");
      break;
    case serial::Baud4800:
      return handler.write("Baud4800");
      break;
    case serial::Baud9600:
      return handler.write("Baud9600");
      break;
    case serial::Baud19200:
      return handler.write("Baud19200");
      break;
    case serial::Baud38400:
      return handler.write("Baud38400");
      break;
    case serial::Baud57600:
      return handler.write("Baud57600");
      break;
    case serial::Baud76800:
      return handler.write("Baud76800");
      break;
    case serial::Baud115200:
      return handler.write("Baud115200");
      break;
    default:
      handler.write("Baud9600");
      handler.setStatusString("Unknown eBaudRateType");
    }
    return false;
  }

    
  bool write(ioHandler& handler,
             const serial::ePortType& data) {
    switch(data) {
    case serial::Com1:
      return handler.write("Com1");
      break;
    case serial::Com2:
      return handler.write("Com2");
      break;
    case serial::Com3:
      return handler.write("Com3");
      break;
    case serial::Com4:
      return handler.write("Com4");
      break;
    case serial::Com5:
      return handler.write("Com5");
      break;
    case serial::Com6:
      return handler.write("Com6");
      break;
    case serial::Com7:
      return handler.write("Com7");
      break;
    case serial::Com8:
      return handler.write("Com8");
      break;
    case serial::USB0:
      return handler.write("USB0");
      break;
    case serial::USB1:
      return handler.write("USB1");
      break;
    default:
      handler.write("Com1");
      handler.setStatusString("Unknown ePortType");
    }
    return false;
  }


  /**
   * Write a serial::eCharBitSizeType
   *
   * @ingroup gStorable
   */
  bool write(ioHandler& handler,
             const serial::eCharBitSizeType& data) {
    
    switch (data) {
    case serial::Cs4:
      return handler.write("Cs4");
      break;
    case serial::Cs5:
      return handler.write("Cs5");
      break;
    case serial::Cs6:
      return handler.write("Cs6");
      break;
    case serial::Cs7:
      return handler.write("Cs7");
      break;
    default:
      handler.write("Cs8");
      handler.setStatusString("Unknown eCharBitSizeType");
      break;
    }

    return false;
  }


  /**
   * Write a serial::eParityType
   *
   * @ingroup gStorable
   */
  bool write(ioHandler& handler,
             const serial::eParityType& data) {

    switch (data) {
    case serial::No:
      return handler.write("No");
      break;
    case serial::Even:
      return handler.write("Even");
      break;
    case serial::Odd:
      return handler.write("Odd");
      break;
    case serial::Space:
      return handler.write("Space");
      break;
    case serial::Mark:
      return handler.write("Mark");
      break;
    default:
      handler.write("No");
      handler.setStatusString("Unknown eParityType");
      break;
    }
    return false;
  }

 bool write(ioHandler& handler,
            const serial::eStopBitType& data) {

   switch (data) {
   case serial::One:
     return handler.write("One");
     break;
   case serial::OneFive:
     return handler.write("OneFive");
     break;
   case serial::Two:
     return handler.write("Two");
     break;
   default:
     handler.write("One");
     handler.setStatusString("Unknown eStopBitType");
     break;
   }

   return false;
 }


 bool write(ioHandler& handler,
            const serial::eFlowControl& data) {

   switch (data) {
   case serial::NoFlowControl:
     return handler.write("NoFlowControl");
     break;
   case serial::RtsCts:
     return handler.write("RtsCts");
     break;
   case serial::XonXoff:
     return handler.write("XonXoff");
     break;
   default:
     handler.write("NoFlowControl");
     handler.setStatusString("Unknown eFlowControl");
     break;
   }

   return false;
 }


  bool read(ioHandler& handler,
            serial::eBaudRateType& baudRate) {

    std::string tmp;
    
    if (lti::read(handler,"baudRate",tmp)) {
      if (tmp.find("Baud0") != std::string::npos) {
        baudRate = serial::Baud0;
      } else if (tmp.find("300") != std::string::npos) {
        baudRate = serial::Baud300;
      } else if (tmp.find("600") != std::string::npos) {
        baudRate = serial::Baud600;
      } else if (tmp.find("1200") != std::string::npos) {
        baudRate = serial::Baud1200;
      } else if (tmp.find("1800") != std::string::npos) {
        baudRate = serial::Baud1800;
      } else if (tmp.find("2400") != std::string::npos) {
        baudRate = serial::Baud2400;
      } else if (tmp.find("4800") != std::string::npos) {
        baudRate = serial::Baud4800;
      } else if (tmp.find("9600") != std::string::npos) {
        baudRate = serial::Baud9600;
      } else if (tmp.find("19200") != std::string::npos) {
        baudRate = serial::Baud19200;
      } else if (tmp.find("38400") != std::string::npos) {
        baudRate = serial::Baud38400;
      } else if (tmp.find("57600") != std::string::npos) {
        baudRate = serial::Baud57600;
      } else if (tmp.find("76800") != std::string::npos) {
        baudRate = serial::Baud76800;
      } else if (tmp.find("115200") != std::string::npos) {
        baudRate = serial::Baud115200;
      } else {
        // default value if unknown tag
        baudRate = serial::Baud9600;
      }
    } else {
      return false;
    }

    return true;
  }

  /**
   * Read a serial::ePortType
   *
   * @ingroup gStorable
   */
  bool read(ioHandler& handler,
            serial::ePortType& port) {
    
    std::string tmp;
    if (lti::read(handler,"port",tmp)) {
      if (tmp == "Com1") {
        port = serial::Com1;
      } else if (tmp == "Com2") {
        port = serial::Com2;
      } else if (tmp == "Com3") {
        port = serial::Com3;
      } else if (tmp == "Com4") {
        port = serial::Com4;
      } else if (tmp == "Com5") {
        port = serial::Com5;
      } else if (tmp == "Com6") {
        port = serial::Com6;
      } else if (tmp == "Com7") {
        port = serial::Com7;
      } else if (tmp == "Com8") {
        port = serial::Com8;
      } else if (tmp == "USB0") {
        port = serial::USB0;
      } else if (tmp == "USB1") {
        port = serial::USB1;
      } else {
        // default value
        port = serial::Com1;
      }
    } else {
      return false;
    }
    return true;
  }

  /**
   * Read a serial::eCharBitSizeType
   *
   * @ingroup gStorable
   */
  bool read(ioHandler& handler,
            serial::eCharBitSizeType& characterSize) {

    std::string tmp;

    if (lti::read(handler,"characterSize",tmp)) {

      if (tmp == "Cs4") {
        characterSize = serial::Cs4;
      } else if (tmp == "Cs5") {
        characterSize = serial::Cs5;
      } else if (tmp == "Cs6") {
        characterSize = serial::Cs6;
      } else if (tmp == "Cs7") {
        characterSize = serial::Cs7;
      } else if (tmp == "Cs8") {
        characterSize = serial::Cs8;
      } else {
        // default value
        characterSize = serial::Cs8;
      }
    } else {
      return false;
    }

    return true;
  }

  /**
   * Read a serial::eParityType
   *
   * @ingroup gStorable
   */
  bool read(ioHandler& handler,
            serial::eParityType& parity) {
    
    std::string tmp;

    if (lti::read(handler,"parity",tmp)) {
      if (tmp == "No") {
        parity = serial::No;
      } else if (tmp == "Even") {
        parity = serial::Even;
      } else if (tmp == "Odd") {
        parity = serial::Odd;
      } else if (tmp == "Space") {
        parity = serial::Space;
      } else if (tmp == "Mark") {
        parity = serial::Mark;
      } else {
        // default value
        parity = serial::No;
      }
    } else {
      return false;
    }
    return true;
  }

  bool read(ioHandler& handler,
            serial::eStopBitType& stopBits) {

    std::string tmp;

    if (lti::read(handler,"stopBits",tmp)) {
      if (tmp == "One") {
        stopBits = serial::One;
      } else if (tmp == "OneFive") {
        stopBits = serial::OneFive;
      } else if (tmp == "Two") {
        stopBits = serial::Two;
      } else {
        // default value
        stopBits = serial::One;
      }
    } else {
      return false;
    }

    return true;
  }

  bool read(ioHandler& handler,
            serial::eFlowControl& flowControl) {

    std::string tmp;

    if (lti::read(handler,"flowControl",tmp)) {
      if (tmp.find("No") != std::string::npos) {
        flowControl = serial::NoFlowControl;
      } else if (tmp.find("Cts") != std::string::npos) {
        flowControl = serial::RtsCts;
      } else if (tmp.find("Xon") != std::string::npos) {
        flowControl = serial::XonXoff;
      } else {
        // default value
        flowControl = serial::NoFlowControl;
      }
    } else {
      return false;
    }

    return true;
  }

}


