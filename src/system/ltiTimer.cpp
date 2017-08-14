/*
 * Copyright (C) 1998, 1999, 2000, 2001
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
 * \file   ltiTimer.cpp
 *         Contains the class lti::timer which allows time measurements
 *         in CPU times and wall times
 * 
 * \author Pablo Alvarado
 * \date   07.04.1999
 *
 * revisions ..: $Id: ltiTimer.cpp,v 1.11 2012-09-16 04:58:34 alvarado Exp $
 */


#include "ltiTimer.h"
#include <sstream>  //for string operations
#include <iostream> //for error msg

#ifndef _LTI_MSC_VER
#  include <sys/time.h>
#  include <unistd.h>
#  include <cstdlib>  // for exit()
#else
#  include <windows.h> //for Wall time
#  include <time.h>    //for Cpu time
#endif

namespace lti {

#ifdef  _LTI_MSC_VER
  const double timer::max32bit_ = 4294967296.0; // 2^32;
#endif

  timer::timer(const eTimeType& tt) : object() {
    started_ = false;
    startTime_ = 0.0;
    endTime_ = 0.0;
    timeType_=tt;

#   ifdef _LTI_MSC_VER
    LARGE_INTEGER tmp;
    QueryPerformanceFrequency(&tmp);
    freq_ = double(tmp.LowPart) + max32bit_*double(tmp.HighPart);
#   endif
  }

  timer::timer(const timer& other) : object() {
    started_ = other.started_;
    startTime_ = other.startTime_;
    endTime_ = other.endTime_;
    timeType_=other.timeType_;

#   ifdef _LTI_MSC_VER
    LARGE_INTEGER tmp;
    QueryPerformanceFrequency(&tmp);
    freq_ = double(tmp.LowPart) + max32bit_*double(tmp.HighPart);
#   endif
  }

  timer::~timer() {
  }

  void timer::start() {
    started_ = true;
    startTime_ = getActualTime();
  }

  void timer::stop() {
    if (started_) {
      endTime_ = getActualTime();
      started_ = false;
    } else {
      endTime_ = startTime_;
    }
  }

  double timer::getTime() const {
    double t=0.0;
    if (!started_) {
      t = endTime_-startTime_;
    } else {
      t = getActualTime() - startTime_;
    }

    // started before midnight?
    if (t<=-1) {
      t += (60.0*60.0*24.0*1000000.0);
    }

    return (t);
  }
  
  double timer::getActualTime() const {
    double t=0.0;
    
    switch (timeType_) {
      
      case Wall: {
        
#ifndef _LTI_MSC_VER

        // Unix version
        
        timeval tv;
        
        if (gettimeofday(&tv,0) == 0) {
          t = double(tv.tv_sec)*1000000.0 + double(tv.tv_usec);
        }
        
#else
        
        // MS VC++ Implementation
        LARGE_INTEGER counter;
        if (QueryPerformanceCounter(&counter) != 0) {
          t = double(counter.LowPart) +
            max32bit_*double(counter.HighPart);
          t/=freq_;      // seconds
          t*=1000000.0;  // microseconds
        }
#endif
        break;
      }
        
      case Cpu: {
        
        //The value t is in usec
        clock_t clockval=clock();
        if (clockval != -1) {
          t = double(clockval)*1.E6/CLOCKS_PER_SEC;
        }
        break;
      }
        
      default:

        std::cerr << "Error: Unknown eTimeType in lti::timer\n";
        exit(1);
    }
        
    return t;
  }

  void timer::setTimeType(const eTimeType& timeType) {
    timeType_ = timeType;
  }

  const timer::eTimeType& timer::getTimeType() {
    return timeType_;
  }

  std::string timer::getDateAndTime() {
#ifndef _LTI_MSC_VER
    // Unix

    time_t t;
    time(&t);
    std::string str = ctime(&t);
    return str.substr(0,str.find('\n'));
#else
    // MS Visual

    const char* day;
    const char* month;

    SYSTEMTIME st;
    GetLocalTime(&st);
    
    switch(st.wDayOfWeek){
      case 0: day="Sun";break;
      case 1: day="Mon";break;
      case 2: day="Tue";break;
      case 3: day="Wed";break;
      case 4: day="Thu";break;
      case 5: day="Fri";break;
      case 6: day="Sat";break;
      case 7: day="Sun";break;
      default: day="error";
    }
    
    switch(st.wMonth){
      case  1: month="Jan";break;
      case  2: month="Feb";break;
      case  3: month="Mar";break;
      case  4: month="Apr";break;
      case  5: month="May";break;
      case  6: month="Jun";break;
      case  7: month="Jul";break;
      case  8: month="Aug";break;
      case  9: month="Sep";break;
      case 10: month="Oct";break;
      case 11: month="Nov";break;
      case 12: month="Dec";break;
      default: month="error";
    }

    std::ostringstream sstr;
    
    sstr << day << " " << month << " " << st.wDay << " " 
         << st.wHour << ":" << st.wMinute << ":" << st.wSecond << " "
         << st.wYear;

    return sstr.str();
    
#endif
  }

  double timer::getCpuTime() {
    return double(clock())*1.E6/CLOCKS_PER_SEC;
  }

  const std::string& timer::name() const {
    _LTI_RETURN_CLASS_NAME
  }


  timer* timer::clone() const {
    return new timer(*this);
  }

  timer* timer::newInstance() const {
    return new timer();
  }

}

