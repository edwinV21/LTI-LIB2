/*
 * Copyright (C) 1998 - 2005
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
 * \file   ltiStreamProgressInfo.h
 *         Contains the class streamProgressInfo, which prints
 *         progress information to a string
 * \author Pablo Alvarado
 * \author Peter Doerfler
 * \date   10.08.2000
 *
 * $Id: ltiStreamProgressInfo.cpp,v 1.3 2006-09-14 15:14:02 doerfler Exp $
 */

#include "ltiStreamProgressInfo.h"
#include <cstdio>

namespace lti {

  // --------------------------------------------------
  // progressInfo
  // --------------------------------------------------

  streamProgressInfo::streamProgressInfo(std::ostream& outStream,
                                         const std::string& theTitle,
                                         const int maximumSteps)
    : progressInfo(theTitle, maximumSteps), out_(&outStream), endline_("\n") {
  }

  streamProgressInfo::streamProgressInfo(const std::string& theTitle,
                                         const int maximumSteps)
    : progressInfo(theTitle, maximumSteps), out_(&std::cout), endline_("\n") {
  }

  streamProgressInfo::streamProgressInfo(const streamProgressInfo& other)
    : progressInfo(), out_(), endline_() {
    copy(other);
  }

  streamProgressInfo::~streamProgressInfo() {
    // do not delete the out stream...
  }

  void streamProgressInfo::useStream(std::ostream& stream) {
    out_=&stream;
  }

  void streamProgressInfo::setEndline(const std::string& endline) {
    endline_=endline;
  }

  void streamProgressInfo::step(const std::string& progressTxt) {
    lastStep_++;

    const int tmp=static_cast<int>(static_cast<float>(1000*lastStep_)/
                                   static_cast<float>(maxSteps_));
    char buf[32];

    sprintf(buf," %4d (%5.1f%%) ", lastStep_, double(tmp)/10.0);

    *out_ << buf << progressTxt << endline_;
    out_->flush();
  }

  void streamProgressInfo::substep(const int detail,
                                   const std::string& progressTxt) {
    if (detail <= detailLevel_) {
      std::string str(15+((detail<2)?0:(detail-1)*2),' ');
      *out_ << str << progressTxt << endline_;
      out_->flush();
    }
  }

  streamProgressInfo&
  streamProgressInfo::copy(const streamProgressInfo& other) {
    progressInfo::copy(other);
    out_      = other.out_;
    endline_  = other.endline_;
    return *this;
  }

  streamProgressInfo&
  streamProgressInfo::operator=(const streamProgressInfo& other) {
    return copy(other);
  }

  const std::string& streamProgressInfo::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  streamProgressInfo* streamProgressInfo::clone() const {
    return new streamProgressInfo(*this);
  }

  streamProgressInfo* streamProgressInfo::newInstance() const {
    return new streamProgressInfo();
  }

}
