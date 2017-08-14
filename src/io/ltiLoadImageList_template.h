/*
 * Copyright (C) 2006
 * Peter Doerfler
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
 * \file   ltiLoadImageList_template.h
 *         Contains the class lti::loadImageList, which has still 
 *         to be documented by Peter Doerfler
 * \author Peter Doerfler
 * \date   10.2.2006
 *
 * revisions ..: $Id: ltiLoadImageList_template.h,v 1.2 2012-08-24 21:33:42 alvarado Exp $
 */


namespace lti {

  template <typename InputIterator>
  bool loadImageList::useRange(InputIterator first, InputIterator last) {

    filenames_.clear();

    if (first==last) {
      setStatusString("Empty range\n");
      return false;
    }

    while (first!=last) {
      if (hasValidFileExtension(*first)) {
        filenames_.push_back(*first);
      }
      ++first;
    }

    it_ = filenames_.begin();

    if (filenames_.empty()) {
      setStatusString("No valid filenames in range");
      return false;
    } else {
      return true;
    }

  }

  // -------------------------------------------------------------------
  // helper functions
  // -------------------------------------------------------------------

  template <class T>
  bool loadImageList::loadHelp(T& dest) {

    // first, check if it_ is a valid iterator
    if (filenames_.empty()) {
      setStatusString("Empty files list");
      return false;
    }

    if (it_==filenames_.end()) {
      setStatusString("Next image unexistent");
      return false;
    }

    bool b = loader_.load(*it_, dest);
    ++it_;

    if (!b) {
      if (getParameters().continueOnError) {
        if (it_!=filenames_.end()) {
          return loadHelp(dest);
        } else {
          setStatusString("No more filenames in list while trying to recover from IO problems (continueOnError is true)");
          return false;
        }
      } else {
        setStatusString("Error loading image file:\n");
        appendStatusString(loader_);
        return false;
      }
    }
    return true;
  }

  template <class T>
  bool loadImageList::loadHelp(T& dest, palette& pal) {

    // first, check if it_ is a valid iterator
    if (filenames_.empty()) {
      setStatusString("Empty files list");
      return false;
    }

    if (it_==filenames_.end()) {
      setStatusString("Next image unexistent");
      return false;
    }


    bool b = loader_.load(*it_, dest, pal);
    ++it_;

    if (!b) {
      if (getParameters().continueOnError) {
        if (it_!=filenames_.end()) {
          return loadHelp(dest);
        } else {
          setStatusString("No more filenames in list while trying to recover from IO problems (continueOnError is true)");
          return false;
        }
      } else {
        setStatusString("Error loading image file:\n");
        appendStatusString(loader_);
        return false;
      }
    }
    return true;
  }

  template <class T>
  bool loadImageList::loadHelp(std::vector<T*>& dest) {

    const int sz = static_cast<int>(filenames_.size());
    dest.reserve(sz);

    // save old position
    std::list<std::string>::const_iterator itSave(it_);

    it_ = filenames_.begin();
    while (it_ != filenames_.end()) {
      T* tmp = new T();
      if (!loadHelp(*tmp)) {
        dest.clear();
        it_=itSave;
        return false;
      }
      dest.push_back(tmp);
    }

    it_=itSave;
    return true;
  }

  template <class T>
  bool loadImageList::loadHelp(std::vector<T*>& dest,
                               std::vector<palette*>& pal) {

    const int sz = static_cast<int>(filenames_.size());
    dest.reserve(sz);
    pal.reserve(sz);

    // save old position
    std::list<std::string>::const_iterator itSave(it_);

    it_ = filenames_.begin();
    while (it_ != filenames_.end()) {
      T* tmp = new T();
      palette* p = new palette();
      if (!loadHelp(*tmp, *p)) {
        dest.clear();
        it_=itSave;
        return false;
      }
      dest.push_back(tmp);
      pal.push_back(p);
    }

    it_=itSave;
    return true;
  }
}
