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
 * \file   ltiProgressReporter.h
 *         Contains the class lti::progressReporter. All classes that
 *         want to report progress via a progressInfo object should
 *         derive from this class.
 * \author Peter Doerfler
 * \date   30.03.2005
 *
 * $Id: ltiProgressReporter.cpp,v 1.3 2006-09-14 15:14:02 doerfler Exp $
 */

#include "ltiProgressReporter.h"
#include "ltiProgressInfo.h"

namespace lti  {
  
  // default constructor
  progressReporter::progressReporter() : progressObject_(0) {
  }

  // copy constructor
  progressReporter::progressReporter(const progressReporter& other)
    : progressObject_(0) {
    copy(other);
  }

  // destructor
  progressReporter::~progressReporter() {
    removeProgressObject();
  }

  // copy
  progressReporter& progressReporter::copy(const progressReporter& other) {
    if (other.haveValidProgressObject()) {
      setProgressObject(other.getProgressObject());
    } else {
      progressObject_ = 0;
    }
    return *this;
  }

  // operator=
  progressReporter& 
  progressReporter::operator=(const progressReporter& other) {
    return copy(other);
  }
  
  // clone
  progressReporter* progressReporter::clone() const {
    return new progressReporter(*this);
  }

  // newInstance
  progressReporter* progressReporter::newInstance() const {
    return new progressReporter();
  }

  // set the progress object
  void progressReporter::setProgressObject(const progressInfo& progBox) {
    removeProgressObject();
    progressObject_ = progBox.clone();
  }

  // remove the active progress object
  void progressReporter::removeProgressObject() {
    delete progressObject_;
    progressObject_ = 0;
  }

  // valid progress object
  bool progressReporter::haveValidProgressObject() const {
    return notNull(progressObject_);
  }

  // valid progress object
  bool progressReporter::haveValidProgressObject(const int detailLevel) const {
    return notNull(progressObject_) && 
          (progressObject_->getDetailLevel() >= detailLevel);
  }

  // get progress object
  const progressInfo& progressReporter::getProgressObject() const {
    return *progressObject_;
  }

  // get progress object
  progressInfo& progressReporter::getProgressObject() {
    return *progressObject_;
  }

} //namespace lti
