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
 * \file   ltiProgressInfo.cpp
 *         Contains the class progressInfo, base class for progress information
 * \author Pablo Alvarado
 * \author Peter Doerfler
 * \date   10.08.2000
 *
 * $Id: ltiProgressInfo.cpp,v 1.5 2008-10-15 17:16:38 alvarado Exp $
 */

#include "ltiProgressInfo.h"

namespace lti {

  progressInfo::progressInfo(const std::string& theTitle,
                             const int maximumSteps) 
    : object(), title_(theTitle), maxSteps_(maximumSteps), detailLevel_(1),
      lastStep_(0) {
  }

  progressInfo::progressInfo(const progressInfo& other) 
    : object(), title_(), maxSteps_(0), detailLevel_(1), lastStep_(0) {
    copy(other);
  }

  progressInfo::~progressInfo() {
  }

  progressInfo& progressInfo::copy(const progressInfo& other) {
    detailLevel_  = other.detailLevel_;
    maxSteps_     = other.maxSteps_;
    lastStep_     = other.lastStep_;
    title_        = other.title_;
    return *this;
  }

  const std::string& progressInfo::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  void progressInfo::setDetailLevel(const int level) {
    detailLevel_ = level;
  }

  int progressInfo::getDetailLevel() const {
    return detailLevel_;
  }

  void progressInfo::setTitle(const std::string& theTitle) {
    title_ = theTitle;
  }

  void progressInfo::setMaxSteps(const int maxSteps) {
    maxSteps_ = maxSteps;
  }

  int progressInfo::getMaxSteps() const {
    return maxSteps_;
  }

  int progressInfo::getStep() const {
    return lastStep_;
  }

  void progressInfo::setStep(const int theStep) {
    lastStep_ = (maxSteps_ > theStep) ? theStep : maxSteps_;
  }

  double progressInfo::calculateProgress() const {
    if (maxSteps_ > 0) {
      return double(lastStep_)/double(maxSteps_);
    }
    return 0.0;
  }

  void progressInfo::reset() {
    lastStep_ = 0;
  }

  bool progressInfo::breakRequested() const {
    return false;
  }


}

