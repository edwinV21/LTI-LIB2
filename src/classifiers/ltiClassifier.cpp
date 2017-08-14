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
 * \file   ltiClassifier.cpp
 * \author Peter Doerfler
 * \author Pablo Alvarado
 * \date   10.08.2000
 *
 * $Id: ltiClassifier.cpp,v 1.8 2008-08-06 03:40:57 alvarado Exp $
 */

#include "ltiClassifier.h"
#include "ltiIoHandler.h"
#include "ltiMath.h"
#include "ltiSort.h"
#include "ltiSort2.h"
#include "ltiSTLIoInterface.h"

namespace lti {

  // --------------------------------------------------
  // static member initialization
  // --------------------------------------------------
  const double classifier::NullProbability(0.0000001);


  // --------------------------------------------------
  // classifier::result
  // --------------------------------------------------

  classifier::result::result()
    : ioObject(), ids_(0), values_(0) {

    winner_=-1;
    confidence_=1.;
    reject_=false;
    valid_=true;
    sortedAsc_=false;
    sortedDesc_=false;
  }

  classifier::result::result(const result& other)
    : ioObject() {

    copy(other);
  }

  classifier::result::result(const int sz) {

    values_=dvector(sz);
    ids_=ivector(sz, UnknownObject);

    winner_=-1;
    confidence_=1.;
    reject_=false;
    valid_=true;
    sortedAsc_=false;
    sortedDesc_=false;
  }

  classifier::result::result(const ivector& theIds,
                             const dvector& theValues) {

    setIdsAndValues(theIds,theValues);
  }

  classifier::result& classifier::result::copy(const result& other) {

    values_.copy(other.values_);
    ids_.copy(other.ids_);
    winner_=other.winner_;
    reject_=other.reject_;
    valid_=other.valid_;
    sortedAsc_=other.sortedAsc_;
    sortedDesc_=other.sortedDesc_;

    return *this;
  }

  classifier::result& classifier::result::operator= (const result& other) {

    return copy(other);
  }

  // returns the name of this type
  const std::string& classifier::result::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  classifier::result* classifier::result::clone() const {

    return new result(*this);
  }

  classifier::result* classifier::result::newInstance() const {

    return new result;
  }

  int classifier::result::size() const {
    return values_.size();
  }

  void classifier::result::setValues(const dvector& theValues) {
    values_.copy(theValues);
    sortedAsc_=false;
    sortedDesc_=false;
  }

  bool classifier::result::setValueByPosition(const int pos,
                                              const double& value) {

    if (pos<0 || pos>=values_.size()) {
      return false;
    }

    values_.at(pos)=value;
    return true;
  }

  bool classifier::result::setValueById(const int id, const double& value) {

    int pos=0;
    ivector::const_iterator it=ids_.begin();
    for (;*it!=id; pos++, ++it) {
    }

    if (it==ids_.end()) {
      return false;
    }

    values_.at(pos)=value;
    return true;
  }

  const dvector& classifier::result::getValues() const {
    return values_;
  }

  bool classifier::result::find(const int id, int& pos) {

    pos=0;
    ivector::const_iterator it=ids_.begin();
    ivector::const_iterator ite=ids_.end();

    for (;it!=ite; pos++, ++it) {
      if (*it == id) {
        break;
      }
    }

    if (it==ite) {
      pos=-1;
      return false;
    }

    return true;
  }

  void classifier::result::setIds(const ivector& theIds) {
    ids_.copy(theIds);
    sortedAsc_=false;
    sortedDesc_=false;
  }

  void classifier::result::setIdsAndValues(const ivector& theIds,
                                           const dvector& theValues) {

    if (theIds.size() == theValues.size()) {
      ids_.copy(theIds);
      values_.copy(theValues);
    } else {
      // get the minimum common data block
      const int sz = lti::min(theIds.size(),theValues.size());
      ids_.allocate(sz);
      ids_.fill(theIds);
      values_.allocate(sz);
      values_.fill(theValues);
    }

    winner_=-1;
    confidence_=1.;
    reject_=false;
    valid_=true;
    sortedAsc_=false;
    sortedDesc_=false;
  }

  bool classifier::result::setPair(const int pos, const int id,
                                     const double& value) {

    if (pos<0 || pos>=values_.size()) {
      return false;
    }

    values_.at(pos)=value;
    ids_.at(pos)=id;

    sortedAsc_=false;
    sortedDesc_=false;
    return true;
  }

  bool classifier::result::getId(const int pos, int& id) const {

    if (pos<0 || pos>=values_.size()) {
      id=NoObject;
      return false;
    }

    id=ids_.at(pos);
    return true;
  }

  const ivector& classifier::result::getIds() const {

    return ids_;
  }

  bool classifier::result::getValueByPosition(const int pos,
                                                double& value) const {

    if (pos<0 || pos>=values_.size()) {
      value=0.;
      return false;
    }

    value=values_.at(pos);
    return true;
  }

  bool classifier::result::getValueById(const int id,
                                          double& value) const {

    int i=0;
    while (i<ids_.size()) {
      if (ids_.at(i)==id) {
        value=values_.at(i);
        return true;
      }
      i++;
    }
    value=0.;
    return false;
  }

  bool classifier::result::getPair(const int pos,
                                     int& id, double& value) const {

    if (pos<0 || pos>=values_.size()) {
      id=NoObject;
      value=0.;
      return false;
    }

    id=ids_.at(pos);
    value=values_.at(pos);
    return true;
  }

  bool classifier::result::setWinner(const int pos) {
    if (pos>=0 && pos<ids_.size()) {
      winner_=pos;
      return true;
    }
    return false;
  }

  int classifier::result::setWinnerAtMax() {

    if (!values_.empty()) {
      winner_=values_.findIndexOfMaximum();
    } else {
      winner_ = -1;
    }
    return winner_;
  }

  int classifier::result::getWinner() const {
    return winner_;
  }

  void classifier::result::setReject(const bool rej) {
    reject_=rej;
  }

  bool classifier::result::isRejected() const {
    return reject_;
  }

  void classifier::result::setConfidenceValue(const double& conf) {
    confidence_=conf;
  }

  double classifier::result::getConfidenceValue() const {
    return confidence_;
  }

  void classifier::result::sortAscending() {

    //To ensure that the winner member still contains the right value
    //after sorting a nifty little trick suggested by wickel is used here:
    //We set the id of the winner to the negative id. After sorting
    //only find the negative id, set winner and revert negation. Great.

    sort2 sorter;

    dvector dtemp(values_);
    ivector itemp(ids_);
    if (winner_!=-1) {
      itemp.at(winner_)*=-1;
    }

    sorter.apply(dtemp, itemp, values_, ids_);

    for (int i=0; i<ids_.size(); i++) {
      if (ids_.at(i)<0) {
        winner_=i;
        ids_.at(i)*=-1;
        break;
      }
    }
    sortedAsc_=true;
    sortedDesc_=false;
  }

  void classifier::result::sortDescending() {

    //To ensure that the winner member still contains the right value
    //after sorting a nifty little trick suggested by wickel is used here:
    //We set the id of the winner to the negative id. After sorting
    //only find the negative id, set winner and revert negation. Great.

    //argument makes the functor sort in descending order
    sort2 sorter(Descending);

    dvector dtemp(values_);
    ivector itemp(ids_);
    if (winner_!=-1) {
      itemp.at(winner_)*=-1;
    }

    sorter.apply(dtemp, itemp, values_, ids_);

    for (int i=0; i<ids_.size(); i++) {
      if (ids_.at(i)<0) {
        winner_=i;
        ids_.at(i)*=-1;
        break;
      }
    }
    sortedAsc_=false;
    sortedDesc_=true;
  }

  void classifier::result::makeProbDistribution() {

    double sum(0.0);
    double val;
    for (int i=0; i<values_.size(); i++) {
      val = values_.at(i);
      if (val < 0) {
        values_.at(i)=0;
      } else {
        sum+=val;
      }
    }

    values_.divide(sum);
  }

  bool classifier::result::isCompatible(const result& other) const {

    if (ids_.size()!=other.ids_.size()) {
      return false;
    }

    sort sorter;

    ivector sThis, sOther;
    sorter.apply(ids_, sThis);
    sorter.apply(other.ids_, sOther);

    for (int i=0; i<sThis.size(); i++) {
      if (sThis.at(i)!=sOther.at(i)) {
        return false;
      }
    }
    return true;
  }

  void classifier::result::uniqueIdsMaximumValue() {

    int i,j;
    sort2 sorter;
    int sz, newSize;
    sz=newSize=ids_.size();

    dvector tValues;
    ivector tIds;

    sorter.apply(ids_, values_, tIds, tValues);

    int currId=-1;
    double maxVal=-1;
    int maxPos=0;
    for (i=0; i<sz; i++) {
      if (tIds.at(i)==currId) {
        if (tValues.at(i)>maxVal) {
          tIds.at(maxPos)=-1;
          maxPos=i;
          maxVal=tValues.at(i);
        } else {
          tIds.at(i)=-1;
        }
        newSize--;
      } else {
        maxPos=i;
        currId=tIds.at(i);
        maxVal=tValues.at(i);
      }
    }

    ids_.allocate(newSize);
    values_.allocate(newSize);

    j=0;
    for (i=0; i<sz; i++) {
      if (tIds.at(i)>0) {
        ids_.at(j)=tIds.at(i);
        values_.at(j)=tValues.at(i);
        j++;
      }
    }
    sortedAsc_=false;
    sortedDesc_=false;
  }

  void classifier::result::uniqueIdsSumValues() {

    int i,j;
    sort2 sorter;
    int sz, newSize;
    sz=newSize=ids_.size();

    dvector tValues;
    ivector tIds;

    sorter.apply(ids_, values_, tIds, tValues);

    int currId=tIds.at(0);
    double sum=tValues.at(0);
    int sumPos=0;
    for (i=1; i<sz; i++) {
      if (tIds.at(i)==currId) {
        sum+=tValues.at(i);
        tIds.at(i)=-1;
        newSize--;
      } else {
        //write the old sum
        tValues.at(sumPos)=sum;
        sumPos=i;
        currId=tIds.at(i);
        sum=tValues.at(i);
      }
    }

    ids_.allocate(newSize);
    values_.allocate(newSize);

    j=0;
    for (i=0; i<sz; i++) {
      if (tIds.at(i)>0) {
        ids_.at(j)=tIds.at(i);
        values_.at(j)=tValues.at(i);
        j++;
      }
    }
    sortedAsc_=false;
    sortedDesc_=false;
  }

  double classifier::result::findMaximumValue() const {

    return values_.findMaximum();
  }

  int classifier::result::findMaximumPosition() const {

    return values_.findIndexOfMaximum();
  }

  int classifier::result::findMaximumId() const {

    return ids_.at(values_.findIndexOfMaximum());
  }

  void classifier::result::findMaximumPair(int& id, double& value) const {

    int i=values_.findIndexOfMaximum();
    id=ids_.at(i);
    value=values_.at(i);
  }

  double classifier::result::findMinimumValue() const {

    return values_.findMinimum();
  }

  int classifier::result::findMinimumPosition() const {

    return values_.findIndexOfMinimum();
  }

  int classifier::result::findMinimumId() const {

    return ids_.at(values_.findIndexOfMinimum());
  }

  void classifier::result::findMinimumPair(int& id, double& value) const {

    int i=values_.findIndexOfMinimum();
    id=ids_.at(i);
    value=values_.at(i);
  }

  classifier::result& classifier::result::add(const result& other) {

    // Check whether ids are used more than once in the oV -> not allowed
    assert(noMultipleIds());
    assert(other.noMultipleIds());

    sort2 sorter;

    ivector tIds, oIds;
    dvector tValues, oValues;

    sorter.apply(ids_, values_, tIds, tValues);
    sorter.apply(other.ids_, other.values_, oIds, oValues);

    int s=ids_.size()+other.ids_.size();
    ids_.allocate(s);
    values_.allocate(s);

    int i=0;
    int j=0;
    int k=0;
    //First merge/add vectors when there are values left in both vectors
    //Take smaller ids first. Because of sorting there is self-syncing, ie
    //equal ids will be found again.
    while (i<tIds.size() && j<oIds.size()) {
      if (tIds.at(i)==oIds.at(j)) {
        ids_.at(k)=tIds.at(i);
        values_.at(k)=tValues.at(i)+oValues.at(j);
        k++;
        i++;
        j++;
      } else if (tIds.at(i)<oIds.at(j)) {
        ids_.at(k)=tIds.at(i);
        values_.at(k)=tValues.at(i);
        k++;
        i++;
      } else {
        ids_.at(k)=oIds.at(j);
        values_.at(k)=oValues.at(j);
        k++;
        j++;
      }
    }
    //Check whether there are any values left at the end of one of the
    //vectors. If yes copy to the target.
    while (i<tIds.size()) {
      ids_.at(k)=tIds.at(i);
      values_.at(k)=tValues.at(i);
      k++;
      i++;
    }
    while (j<oIds.size()) {
      ids_.at(k)=oIds.at(j);
      values_.at(k)=oValues.at(j);
      k++;
      j++;
    }

    ids_.resize(k,Copy);
    values_.resize(k,Copy);

    sortedAsc_=false;
    sortedDesc_=false;
    return *this;
  }

  classifier::result& classifier::result::add(const result& a,
                                              const result& b) {

    // Check whether ids are used more than once in the oV -> not allowed
    assert(a.noMultipleIds());
    assert(b.noMultipleIds());

    sort2 sorter;

    ivector aIds, bIds;
    dvector aValues, bValues;

    sorter.apply(a.ids_, a.values_, aIds, aValues);
    sorter.apply(b.ids_, b.values_, bIds, bValues);

    int s=a.ids_.size()+b.ids_.size();
    ids_.allocate(s);
    values_.allocate(s);

    int i=0;
    int j=0;
    int k=0;
    //First merge/add vectors when there are values left in both vectors
    //Take smaller ids first. Because of sorting there is self-syncing, ie
    //equal ids will be found again.
    while (i<aIds.size() && j<bIds.size()) {
      if (aIds.at(i)==bIds.at(j)) {
        ids_.at(k)=aIds.at(i);
        values_.at(k)=aValues.at(i)+bValues.at(j);
        k++;
        i++;
        j++;
      } else if (aIds.at(i)<bIds.at(j)) {
        ids_.at(k)=aIds.at(i);
        values_.at(k)=aValues.at(i);
        k++;
        i++;
      } else {
        ids_.at(k)=bIds.at(j);
        values_.at(k)=bValues.at(j);
        k++;
        j++;
      }
    }
    //Check whether there are any values left at the end of one of the
    //vectors. If yes copy to the target.
    while (i<aIds.size()) {
      ids_.at(k)=aIds.at(i);
      values_.at(k)=aValues.at(i);
      k++;
      i++;
    }
    while (j<bIds.size()) {
      ids_.at(k)=bIds.at(j);
      values_.at(k)=bValues.at(j);
      k++;
      j++;
    }

    ids_.resize(k);
    values_.resize(k);

    sortedAsc_=false;
    sortedDesc_=false;
    return *this;
  }

  classifier::result& classifier::result::add(const double& s) {

    values_.add(s);
    return *this;
  }

  classifier::result& classifier::result::addScaled(const result& other,
                                                    const double& scale) {

    // Check whether ids are used more than once in the oV -> not allowed
    assert(noMultipleIds());
    assert(other.noMultipleIds());

    sort2 sorter;

    ivector tIds, oIds;
    dvector tValues, oValues;

    sorter.apply(ids_, values_, tIds, tValues);
    sorter.apply(other.ids_, other.values_, oIds, oValues);

    int s=ids_.size()+other.ids_.size();
    ids_.allocate(s);
    values_.allocate(s);

    int i=0;
    int j=0;
    int k=0;
    //First merge/add vectors when there are values left in both vectors
    //Take smaller ids first. Because of sorting there is self-syncing, ie
    //equal ids will be found again.
    while (i<tIds.size() && j<oIds.size()) {
      if (tIds.at(i)==oIds.at(j)) {
        ids_.at(k)=tIds.at(i);
        values_.at(k)=tValues.at(i)+scale*oValues.at(j);
        k++;
        i++;
        j++;
      } else if (tIds.at(i)<oIds.at(j)) {
        ids_.at(k)=tIds.at(i);
        values_.at(k)=tValues.at(i);
        k++;
        i++;
      } else {
        ids_.at(k)=oIds.at(j);
        values_.at(k)=scale*oValues.at(j);
        k++;
        j++;
      }
    }
    //Check whether there are any values left at the end of one of the
    //vectors. If yes copy to the target.
    while (i<tIds.size()) {
      ids_.at(k)=tIds.at(i);
      values_.at(k)=tValues.at(i);
      k++;
      i++;
    }
    while (j<oIds.size()) {
      ids_.at(k)=oIds.at(j);
      values_.at(k)=scale*oValues.at(j);
      k++;
      j++;
    }

    ids_.resize(k);
    values_.resize(k);

    sortedAsc_=false;
    sortedDesc_=false;
    return *this;
  }

  classifier::result& classifier::result::mul(const result& other) {

    // Check whether ids are used more than once in the oV -> not allowed
    assert(noMultipleIds());
    assert(other.noMultipleIds());

    sort2 sorter;

    ivector tIds, oIds;
    dvector tValues, oValues;

    sorter.apply(ids_, values_, tIds, tValues);
    sorter.apply(other.ids_, other.values_, oIds, oValues);

    int s=ids_.size()+other.ids_.size();
    ids_.allocate(s);
    values_.allocate(s);

    int i=0;
    int j=0;
    int k=0;
    //First merge/add vectors when there are values left in both vectors
    //Take smaller ids first. Because of sorting there is self-syncing, ie
    //equal ids will be found again.
    while (i<tIds.size() && j<oIds.size()) {
      if (tIds.at(i)==oIds.at(j)) {
        ids_.at(k)=tIds.at(i);
        values_.at(k)=tValues.at(i)*oValues.at(j);
        k++;
        i++;
        j++;
      } else if (tIds.at(i)<oIds.at(j)) {
        ids_.at(k)=tIds.at(i);
        values_.at(k)=0.;
        k++;
        i++;
      } else {
        ids_.at(k)=oIds.at(j);
        values_.at(k)=0.;
        k++;
        j++;
      }
    }
    //Check whether there are any values left at the end of one of the
    //vectors. If yes copy to the target.
    while (i<tIds.size()) {
      ids_.at(k)=tIds.at(i);
      values_.at(k)=0.;
      k++;
      i++;
    }
    while (j<oIds.size()) {
      ids_.at(k)=oIds.at(j);
      values_.at(k)=0.;
      k++;
      j++;
    }

    ids_.resize(k);
    values_.resize(k);

    sortedAsc_=false;
    sortedDesc_=false;
    return *this;
  }

  classifier::result& classifier::result::mul(const result& a,
                                              const result& b) {

    // Check whether ids are used more than once in the oV -> not allowed
    assert(a.noMultipleIds());
    assert(b.noMultipleIds());

    sort2 sorter;

    ivector aIds, bIds;
    dvector aValues, bValues;

    sorter.apply(a.ids_, a.values_, aIds, aValues);
    sorter.apply(b.ids_, b.values_, bIds, bValues);

    int s=a.ids_.size()+b.ids_.size();
    ids_.allocate(s);
    values_.allocate(s);

    int i=0;
    int j=0;
    int k=0;
    //First merge/add vectors when there are values left in both vectors
    //Take smaller ids first. Because of sorting there is self-syncing, ie
    //equal ids will be found again.
    while (i<aIds.size() && j<bIds.size()) {
      if (aIds.at(i)==bIds.at(j)) {
        ids_.at(k)=aIds.at(i);
        values_.at(k)=aValues.at(i)*bValues.at(j);
        k++;
        i++;
        j++;
      } else if (aIds.at(i)<bIds.at(j)) {
        ids_.at(k)=aIds.at(i);
        values_.at(k)=0.;
        k++;
        i++;
      } else {
        ids_.at(k)=bIds.at(j);
        values_.at(k)=0.;
        k++;
        j++;
      }
    }
    //Check whether there are any values left at the end of one of the
    //vectors. If yes copy to the target.
    while (i<aIds.size()) {
      ids_.at(k)=aIds.at(i);
      values_.at(k)=0.;
      k++;
      i++;
    }
    while (j<bIds.size()) {
      ids_.at(k)=bIds.at(j);
      values_.at(k)=0.;
      k++;
      j++;
    }

    ids_.resize(k);
    values_.resize(k);

    sortedAsc_=false;
    sortedDesc_=false;
    return *this;
  }

  classifier::result& classifier::result::mul(const double& s) {

    values_.multiply(s);
    return *this;
  }

  classifier::result& classifier::result::divide(const double& s) {

    values_.divide(s);
    return *this;
  }

  classifier::result& classifier::result::max(const result& other) {

    // Check whether ids are used more than once in the oV -> not allowed
    assert(noMultipleIds());
    assert(other.noMultipleIds());

    sort2 sorter;

    ivector tIds, oIds;
    dvector tValues, oValues;

    sorter.apply(ids_, values_, tIds, tValues);
    sorter.apply(other.ids_, other.values_, oIds, oValues);

    int s=ids_.size()+other.ids_.size();
    ids_.allocate(s);
    values_.allocate(s);

    int i=0;
    int j=0;
    int k=0;
    //First merge/add vectors when there are values left in both vectors
    //Take smaller ids first. Because of sorting there is self-syncing, ie
    //equal ids will be found again.
    while (i<tIds.size() && j<oIds.size()) {
      if (tIds.at(i)==oIds.at(j)) {
        ids_.at(k)=tIds.at(i);
        values_.at(k)=lti::max(tValues.at(i),oValues.at(j));
        k++;
        i++;
        j++;
      } else if (tIds.at(i)<oIds.at(j)) {
        ids_.at(k)=tIds.at(i);
        values_.at(k)=tValues.at(i);
        k++;
        i++;
      } else {
        ids_.at(k)=oIds.at(j);
        values_.at(k)=oValues.at(j);
        k++;
        j++;
      }
    }
    //Check whether there are any values left at the end of one of the
    //vectors. If yes copy to the target.
    while (i<tIds.size()) {
      ids_.at(k)=tIds.at(i);
      values_.at(k)=tValues.at(i);
      k++;
      i++;
    }
    while (j<oIds.size()) {
      ids_.at(k)=oIds.at(j);
      values_.at(k)=oValues.at(j);
      k++;
      j++;
    }

    ids_.resize(k);
    values_.resize(k);

    sortedAsc_=false;
    sortedDesc_=false;
    return *this;
  }

  classifier::result& classifier::result::max(const result& a, 
                                              const result& b) {

    // Check whether ids are used more than once in the oV -> not allowed
    assert(a.noMultipleIds());
    assert(b.noMultipleIds());

    sort2 sorter;

    ivector aIds, bIds;
    dvector aValues, bValues;

    sorter.apply(a.ids_, a.values_, aIds, aValues);
    sorter.apply(b.ids_, b.values_, bIds, bValues);

    int s=a.ids_.size()+b.ids_.size();
    ids_.allocate(s);
    values_.allocate(s);

    int i=0;
    int j=0;
    int k=0;
    //First merge/add vectors when there are values left in both vectors
    //Take smaller ids first. Because of sorting there is self-syncing, ie
    //equal ids will be found again.
    while (i<aIds.size() && j<bIds.size()) {
      if (aIds.at(i)==bIds.at(j)) {
        ids_.at(k)=aIds.at(i);
        values_.at(k)=lti::max(aValues.at(i),bValues.at(j));
        k++;
        i++;
        j++;
      } else if (aIds.at(i)<bIds.at(j)) {
        ids_.at(k)=aIds.at(i);
        values_.at(k)=aValues.at(i);
        k++;
        i++;
      } else {
        ids_.at(k)=bIds.at(j);
        values_.at(k)=bValues.at(j);
        k++;
        j++;
      }
    }
    //Check whether there are any values left at the end of one of the
    //vectors. If yes copy to the target.
    while (i<aIds.size()) {
      ids_.at(k)=aIds.at(i);
      values_.at(k)=aValues.at(i);
      k++;
      i++;
    }
    while (j<bIds.size()) {
      ids_.at(k)=bIds.at(j);
      values_.at(k)=bValues.at(j);
      k++;
      j++;
    }

    ids_.resize(k);
    values_.resize(k);

    sortedAsc_=false;
    sortedDesc_=false;
    return *this;
  }

  classifier::result& classifier::result::min(const result& other) {

    // Check whether ids are used more than once in the oV -> not allowed
    assert(noMultipleIds());
    assert(other.noMultipleIds());

    sort2 sorter;

    ivector tIds, oIds;
    dvector tValues, oValues;

    sorter.apply(ids_, values_, tIds, tValues);
    sorter.apply(other.ids_, other.values_, oIds, oValues);

    int s=ids_.size()+other.ids_.size();
    ids_.allocate(s);
    values_.allocate(s);

    int i=0;
    int j=0;
    int k=0;
    //First merge/add vectors when there are values left in both vectors
    //Take smaller ids first. Because of sorting there is self-syncing, ie
    //equal ids will be found again.
    while (i<tIds.size() && j<oIds.size()) {
      if (tIds.at(i)==oIds.at(j)) {
        ids_.at(k)=tIds.at(i);
        values_.at(k)=lti::min(tValues.at(i), oValues.at(j));
        k++;
        i++;
        j++;
      } else if (tIds.at(i)<oIds.at(j)) {
        ids_.at(k)=tIds.at(i);
        values_.at(k)=0.;
        k++;
        i++;
      } else {
        ids_.at(k)=oIds.at(j);
        values_.at(k)=0.;
        k++;
        j++;
      }
    }
    //Check whether there are any values left at the end of one of the
    //vectors. If yes copy to the target.
    while (i<tIds.size()) {
      ids_.at(k)=tIds.at(i);
      values_.at(k)=0.;
      k++;
      i++;
    }
    while (j<oIds.size()) {
      ids_.at(k)=oIds.at(j);
      values_.at(k)=0.;
      k++;
      j++;
    }

    ids_.resize(k);
    values_.resize(k);

    sortedAsc_=false;
    sortedDesc_=false;
    return *this;
  }

  classifier::result& classifier::result::min(const result& a,
                                              const result& b) {

    // Check whether ids are used more than once in the oV -> not allowed
    assert(a.noMultipleIds());
    assert(b.noMultipleIds());

    sort2 sorter;

    ivector aIds, bIds;
    dvector aValues, bValues;

    sorter.apply(a.ids_, a.values_, aIds, aValues);
    sorter.apply(b.ids_, b.values_, bIds, bValues);

    int s=a.ids_.size()+b.ids_.size();
    ids_.allocate(s);
    values_.allocate(s);

    int i=0;
    int j=0;
    int k=0;
    //First merge/add vectors when there are values left in both vectors
    //Take smaller ids first. Because of sorting there is self-syncing, ie
    //equal ids will be found again.
    while (i<aIds.size() && j<bIds.size()) {
      if (aIds.at(i)==bIds.at(j)) {
        ids_.at(k)=aIds.at(i);
        values_.at(k)=lti::min(aValues.at(i), bValues.at(j));
        k++;
        i++;
        j++;
      } else if (aIds.at(i)<bIds.at(j)) {
        ids_.at(k)=aIds.at(i);
        values_.at(k)=0.;
        k++;
        i++;
      } else {
        ids_.at(k)=bIds.at(j);
        values_.at(k)=0.;
        k++;
        j++;
      }
    }
    //Check whether there are any values left at the end of one of the
    //vectors. If yes copy to the target.
    while (i<aIds.size()) {
      ids_.at(k)=aIds.at(i);
      values_.at(k)=0.;
      k++;
      i++;
    }
    while (j<bIds.size()) {
      ids_.at(k)=bIds.at(j);
      values_.at(k)=0.;
      k++;
      j++;
    }

    ids_.resize(k);
    values_.resize(k);

    sortedAsc_ =false;
    sortedDesc_=false;
    return *this;
  }

  bool classifier::result::noMultipleIds() const {

    sort sorter;
    ivector tids;
    sorter.apply(ids_, tids);

    int i;
    for (i=0; i<tids.size()-1; i++) {
      if (ids_.at(i)==ids_.at(i+1)) {
        return false;
      }
    }
    return true;
  }

  bool classifier::result::write(ioHandler& handler,
                                 const bool complete) const {
    bool b(true);

    if (complete) {
      b = b && handler.writeBegin();
    }

    b = b && lti::write(handler, "winner", winner_);
    b = b && lti::write(handler, "confidence", confidence_);
    b = b && lti::write(handler, "reject", reject_);
    b = b && lti::write(handler, "valid", valid_);
    b = b && lti::write(handler, "sortedAsc", sortedAsc_);
    b = b && lti::write(handler, "sortedDesc", sortedDesc_);
    b = b && lti::write(handler, "ids", ids_);
    b = b && lti::write(handler, "values", values_);


    if (complete) {
      b = b && handler.writeEnd();
    }

    return b;
  }

  bool classifier::result::read(ioHandler& handler,const bool complete) {
    bool b(true);

    if (complete) {
      b = b && handler.readBegin();
    }

    b = b && lti::read(handler, "winner", winner_);
    b = b && lti::read(handler, "confidence", confidence_);
    b = b && lti::read(handler, "reject", reject_);
    b = b && lti::read(handler, "valid", valid_);
    b = b && lti::read(handler, "sortedAsc", sortedAsc_);
    b = b && lti::read(handler, "sortedDesc", sortedDesc_);
    b = b && lti::read(handler, "ids", ids_);
    b = b && lti::read(handler, "values", values_);


    if (complete) {
      b = b && handler.readEnd();
    } 

    return b;
  }

  // --------------------------------------------------
  // classifier::outputTemplate
  // --------------------------------------------------

  classifier::outputTemplate::outputTemplate()
    : ioObject(), multipleMode_(ObjProb), probList_(0), defaultIds_(0) {
  }

  classifier::outputTemplate::outputTemplate(const outputTemplate& other) 
    : ioObject() {

    copy(other);
  }

  classifier::outputTemplate::outputTemplate(const ivector& theIds)
    : ioObject(),multipleMode_(Ignore),probList_(0),defaultIds_(theIds) {
  }

  classifier::outputTemplate::outputTemplate(const int& theSize)
    : ioObject(),multipleMode_(ObjProb),probList_(theSize),
      defaultIds_(theSize,0) {
  }

  classifier::outputTemplate&
  classifier::outputTemplate::copy(const outputTemplate& other) {

    multipleMode_=other.multipleMode_;
    probList_=other.probList_;
    defaultIds_.copy(other.defaultIds_);
    return *this;
  }

  classifier::outputTemplate* classifier::outputTemplate::clone() const {

    return new outputTemplate(*this);
  }

  classifier::outputTemplate* classifier::outputTemplate::newInstance() const {

    return new outputTemplate;
  }

  // returns the name of this type
  const std::string& classifier::outputTemplate::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  void classifier::outputTemplate::setMultipleMode(const eMultipleMode& mode) {

    multipleMode_=mode;
  }

  classifier::eMultipleMode
  classifier::outputTemplate::getMultipleMode() const {

    return multipleMode_;
  }

  bool classifier::outputTemplate::setIds(const ivector& theIds) {

    defaultIds_.copy(theIds);
    return true;
  }

  const ivector& classifier::outputTemplate::getIds() const {

    return defaultIds_;
  }

  bool classifier::outputTemplate::setProbs(const int& pos,
                                            const ivector& theIds,
                                            const dvector& theValues) {

    if (pos<0 || pos>=static_cast<int>(probList_.size())) {
      return false;
    }

    probList_[pos].setIdsAndValues(theIds, theValues);

    return true;
  }

  bool classifier::outputTemplate::setProbs(const int& pos,
                                            const result& outV) {

    if (pos<0 || pos>=static_cast<int>(probList_.size())) {
      return false;
    }

    probList_[pos]=outV;

    return true;
  }

  bool classifier::outputTemplate::setData(const int& pos,
                                           const int& id,
                                           const result& outV) {

    bool b=true;

    if (pos<0) {
      return false;
    }

    if (pos<static_cast<int>(probList_.size())) {
      probList_[pos]=outV;
    } else {
      b=false;
    }

    if (pos<defaultIds_.size()) {
      defaultIds_.at(pos)=id;
    } else {
      b=false;
    }

    return b;
  }

  const classifier::result&
  classifier::outputTemplate::getProbs(const int& pos) const {

    if (pos<0 || pos>(signed int)probList_.size()) {
      return probList_[0];
    } else {
      return probList_[pos];
    }
  }

  int classifier::outputTemplate::size() const {

    int pSize=static_cast<int>(probList_.size());
    int iSize=static_cast<int>(defaultIds_.size());
    return (pSize>iSize ? pSize : iSize);
  }

  bool classifier::outputTemplate::apply(const dvector& data,
                                         result& res) const {

    bool b=true;
    eMultipleMode localMode=multipleMode_;

    // check if defaultIds is valid when mode is Ignore
    if (localMode==Ignore && defaultIds_.size()==0) {
      // if not ok set mode to Max if probList is there else bad error
      if (probList_.size()==0) {
        res=result(0);
        return false;
      } else {
        localMode = Max;
        b=false;
      }
    }

    // check if probList is valid when mode is other than Ignore
    if (localMode!=Ignore && probList_.size()==0) {
      // if not ok, set mode to Ignore if defaultIds !=0 else bad error
      if (defaultIds_.size()==0) {
        res=result(0);
        return false;
      } else {
        localMode = Ignore;
        b = false;
      }
    }

    switch (localMode) {
      
      case Ignore: {
        res.setIdsAndValues(defaultIds_, data);
        break;
      }
        
      case Max: {
        ivector ids(data.size());
        std::vector<result>::const_iterator it=probList_.begin();
        std::vector<result>::const_iterator ite=probList_.end();
        int i=0;
        while (it!=ite) {
          ids.at(i++)=it->findMaximumId();
          it++;
        }
        res.setIdsAndValues(ids, data);
        break;
      }
        
      case Uniform: {
        result tmpVec;
        dvector uniProbs;
        int uniSize;
        tmpVec=probList_[0];
        uniSize=tmpVec.size();
        uniProbs=dvector(uniSize, 1./uniSize);
        tmpVec.setValues(uniProbs);
        res=tmpVec.mul(data.at(0));
        std::vector<result>::const_iterator it=probList_.begin();
        std::vector<result>::const_iterator ite=probList_.end();
        int i=1;
        it++;
        while (it!=ite) {
          tmpVec=*it++;
          uniSize=tmpVec.size();
          uniProbs=dvector(uniSize, 1./uniSize);
          tmpVec.setValues(uniProbs);
          res.add(tmpVec.mul(data.at(i++)));
        }
        break;
      }
        
      case ObjProb:
      default: {
        res=probList_[0];
        res.mul(data.at(0));
        std::vector<result>::const_iterator it=probList_.begin();
        std::vector<result>::const_iterator ite=probList_.end();
        int i=1;
        result tmpVec;
        it++;
        while (it!=ite) {
          tmpVec=*it++;
          res.add(tmpVec.mul(data.at(i++)));
        }
      }
    }
    return b;
  }

  bool classifier::outputTemplate::write(ioHandler& handler,
                                         const bool complete) const {
    bool b(true);

    if (complete) {
      b = b && handler.writeBegin();
    }

    b = b && lti::write(handler, "multipleMode", multipleMode_);
    b = b && lti::write(handler, "defaultIds", defaultIds_);
    b = b && lti::write(handler, "probList", probList_);

    if (complete) {
      b = b && handler.writeEnd();
    }

    return b;
  }

  bool classifier::outputTemplate::read(ioHandler& handler,
                                        const bool complete) {
    bool b(true);

    if (complete) {
      b = b && handler.readBegin();
    }

    b = b && lti::read(handler,"multipleMode",multipleMode_);
    b = b && lti::read(handler, "defaultIds",defaultIds_);
    b = b && lti::read(handler, "probList", probList_);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

  // ------------------------------------------------------------------
  // classifier
  // ------------------------------------------------------------------

  // default constructor
  classifier::classifier()
    : parametersManager(), progressReporter(), outTemplate_(0) {
  }

  // copy constructor
  classifier::classifier(const classifier& other)
    : ioObject(), status(), parametersManager(), progressReporter() {
    copy(other);
  }

  // destructor
  classifier::~classifier() {
  }

  // copy member
  classifier& classifier::copy(const classifier& other) {
    parametersManager::copy(other);
    progressReporter::copy(other);

    outTemplate_.copy(other.outTemplate_);

    return (*this);
  }

  /*
   * write the classifier in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool classifier::write(ioHandler& handler,const bool complete) const {
    bool b;
    b=this->parametersManager::write(handler,complete);
    b=b && lti::write(handler,"outputTemplate",outTemplate_);
    return b;
  }

  /*
   * read the classifier from the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool classifier::read(ioHandler& handler,const bool complete) {
    bool b;
    b=this->parametersManager::read(handler,complete);
    b=b && lti::read(handler,"outputTemplate",outTemplate_);
    return b;
  }

  /*
   * Sets the classifier's outputTemplate
   */
  void 
  classifier::setOutputTemplate(const classifier::outputTemplate& outTempl) {
    outTemplate_.copy(outTempl);
  }

  /*
   * Returns the outputTemplate
   */
  const classifier::outputTemplate& classifier::getOutputTemplate() const {
    return outTemplate_;
  }

  /**
   * Read a classifier::eMultipleMode
   *
   * @ingroup gStorable
   */
  bool read(ioHandler& handler,classifier::eMultipleMode& data) {
    std::string str;
    
    if (handler.read(str)) {
      if (str == "Ignore") {
        data = classifier::Ignore;
      } else if (str == "Max") {
        data = classifier::Max;
      } else if (str == "Uniform") {
        data = classifier::Uniform;
      } else if (str == "ObjProb") {
        data = classifier::ObjProb;
      } else {
        data = classifier::ObjProb;
      }
      return true;
    }
    return false;
  }

  /**
   * Write a classifier::eMultipleMode
   *
   * @ingroup gStorable
   */
  bool write(ioHandler& handler,const classifier::eMultipleMode& data) {
    bool b = false;
    switch(data) {
    case classifier::Ignore:
      b = handler.write("Ignore");
      break;
    case classifier::Max:
      b = handler.write("Max");
      break;
    case classifier::Uniform:
      b = handler.write("Uniform");
      break;
    case classifier::ObjProb:
      b = handler.write("ObjProb");
      break;
    default:
      handler.write("Unknown");
      b = false;
      handler.setStatusString("undefined classifier::eMultipleMode");
      break;
    }

    return b;
  }


}
