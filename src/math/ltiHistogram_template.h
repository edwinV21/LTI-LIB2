/*
 * Copyright (C) 1998, 1999, 2000, 2001
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
 * \file   ltiHistogram_template.h
 * \author Pablo Alvarado
 * \date   29.06.00
 * 
 * $Id: ltiHistogram_template.h,v 1.7 2008-12-27 05:49:35 alvarado Exp $
 */

namespace lti {

  /*
   * default constructor creates an empty histogram;
   */
  template<class T>
  histogram<T>::histogram()
    : totalNumberOfCells_(0),numberOfEntries_(0),dimensionality_(0) {
  }

  /*
   * create a histogram of the given dimensionality.
   *
   * Each dimension will have the given number of cells, i.e. the histogram
   * will have \f$cells^dimensions\f$ cells.
   *
   * @param dimensions the dimensionality of the histogram.
   * @param cells the number of cells per dimension.
   */
  template<class T>
  histogram<T>::histogram(const int dimensions, const int cells)
    : totalNumberOfCells_(0),numberOfEntries_(0),dimensionality_(0) {
    resize(dimensions,cells);
  }

  /*
   * create a histogram with the given dimensionality.
   *
   * Each dimension \e i will have the number of cells indicated in
   * the \e i-th element of the vector <code>cells</code>.
   *
   * If the dimensionaly differs from the size of the given vector, the
   * number of cells of the dimension \e i will be given by
   * \f$dim_i = cells[i \mod cells.size()].
   *
   * This means, if you want a 6-dimensional histogram, and your cells-vector
   * has only three elements [10,15,5], the number of cells per dimension
   * will be [10,15,5,10,15,5]
   *
   * @param dimensions the dimensionality of the histogram
   * @param cells a vector with the number of cell per dimension
   */
  template<class T>
  histogram<T>::histogram(const int dimensions,const ivector& cells)
    : totalNumberOfCells_(0),numberOfEntries_(0),dimensionality_(0) {
    resize(dimensions,cells);
  }

  /*
   * create this histogram as a copy of another histogram
   * @param other the histogram to be copied.
   */
  template<class T>
  histogram<T>::histogram(const histogram<T>& other)
    : container() {
    copy(other);
  }

  /*
   * destructor
   */
  template<class T>
  histogram<T>::~histogram() {
  }

  template<class T>
  template<class U>
  histogram<T>& histogram<T>::castFrom(const histogram<U>& other) {
    resize(other.dimensions(), other.cellsPerDimension());
    
    typename histogram<U>::const_iterator otherIt = other.begin();
    iterator thisIt = begin();
    
    while (! (otherIt == other.end())) {
      *thisIt = static_cast<T>(*otherIt);
      otherIt++;
      thisIt++;
    }

    updateNumberOfEntries();
    
    return (*this);
  }

  /*
   * change dimensionality and cell number of the histogram.  All data will
   * be lost!
   *
   * @param dimensions the new dimensionality of the histogram
   * @param cells      the number of cells per dimension
   *
   */
  template<class T>
  void histogram<T>::resize(const int dimensions,
                            const int cells) {
    // a temporary dimensionality_ vector
    ivector tmp(dimensions,cells);
    // resize
    resize(dimensions,tmp);
  }

  /*
   * change dimensionality and cell number of the histogram.  All data will
   * be lost!
   *
   * @param dimensions the new dimensionality of the histogram
   * @param cells      the number of cells per dimension
   *
   */
  template<class T>
  void histogram<T>::resize(const int dim,
                         const ivector& cells) {
    // should this histogram be cleaned up?
    if ((dim < 1) || (cells.size() < 1)) {
      totalNumberOfCells_ = 0;
      numberOfEntries_ = 0;
      dimensionality_ = 0;
      theHistogram_.clear();
      theCellsPerDimension_.clear();
      firstCell_.clear();
      lastCell_.clear();
      return;
    }

    // determine how many cells the new histogram should have
    int i,totalCells;

    theCellsPerDimension_.resize(dim);
    theCellsPerDimension_.at(0)=cells.at(0);

    for (totalCells=cells.at(0),i=1;
         i<dim;
         i++) {
      totalCells *= cells.at(i % dim);
      theCellsPerDimension_.at(i)=cells.at(i % dim);
    }

    // resize and initialize the vector
    theHistogram_.resize(totalCells,0,Init);
    totalNumberOfCells_ = totalCells;

    numberOfEntries_ = 0;
    dimensionality_ = dim;
    firstCell_.resize(dim,0);
    lastCell_.copy(theCellsPerDimension_);
    lastCell_.add(-1); // subtract 1 from all elements
  }

  template<class T>
  void histogram<T>::clear() {
    resize(0,0);
  }

  /*
   * initialize all cells of the histogram with 0 (or another specified
   * number).
   */
  template<class T>
  void histogram<T>::initialize(const T& value) {
    fill(value);
  }


  /*
   * Normalize the histogram and then denormalize it with the given number
   * of entries
   */
  template<class T>
  void histogram<T>::setNumberOfEntries(const T& newNumberOfEntries) {

    T tmp = static_cast<T>(0);
    if (numberOfEntries_ != 0) {
      tmp = newNumberOfEntries/numberOfEntries_;
    }

    theHistogram_.multiply(tmp);

//      normalize();
//      theHistogram_.multiply(newNumberOfEntries);
    numberOfEntries_ = newNumberOfEntries;
  }

  /*
   * counts the number of entries in the whole histogram and sets
   * the internal counter for the total number of entries.
   * if some direct access to the cell contents
   * have been done, you should update the number of entries with this
   * function
   */
  template<class T>
  void histogram<T>::updateNumberOfEntries() {
    const_iterator it,e;
    T entries = 0;

    for (it = begin(),e=end();
         it!=e;
         it++) {
      entries += (*it);
    }

    numberOfEntries_ = entries;
  }

  /*
   * fills the histogram elements with <code>iniValue</code> between
   * the n-dimensional points <code>from</code> and <code>to</code>.
   * @param iniValue the elements will be initialized with this
   *                 value.
   * @param from     first element index
   * @param to       last element index
   *
   * If <code>from</code> or <code>to</code> are out of bounds,
   * they will be (internaly) adjusted to a correct value.
   *
   * Example:
   * \code
   *   lti::histogram hist(1,10); // 1D-histogram with 10 elements
   *   hist.clear;
   *   hist.fill(9,ivector(1,1),ivector(1,3)); // hist=[0,9,9,9,0,0,0,0,0,0]
   * \endcode
   */
  template<class T>
  void histogram<T>::fill(const T& iniValue,
                           const ivector& from,
                           const ivector& to) {

    ivector first;
    ivector last;

    if (from.size() == 0) {
      first.copy(firstCell_);
    } else {
      first.copy(from);
    }

    if (to.size() == 0) {
      last.copy(lastCell_);
    } else {
      last.copy(to);
    }

    if ((first.size() != last.size()) || (first.size() != dimensionality_)) {
      // invalid dimensionality
      return;
    }

    // can I fill everything?
    if (first.equals(firstCell_) && last.equals(lastCell_)) {
      theHistogram_.fill(iniValue);
      numberOfEntries_ = iniValue*totalNumberOfCells_;
      return;
    }

    int i,mi,ma;
    T tmp;

    // check and correct the boundaries
    for (i=0;i<dimensionality_;i++) {
      mi = min(first.at(i),last.at(i));
      ma = max(first.at(i),last.at(i));

      first.at(i)= max( mi, firstCell_.at(i) );
      last.at(i) = min( ma, lastCell_.at(i)  );
    }

    ivector idx(first); // the index! this will iterate on the histogram
                        // hyperbox

    bool ready = false;

    // to check if index at the end, we just need to check the last
    // dimension of the index
    while ( !ready ) {

      // update of numberOfEntries_
      tmp = iniValue - at(idx); // update of entry number due to this cell
      numberOfEntries_ += tmp;

      // set the cell value
      at(idx) = iniValue;

      // idx update
      i = 0;
      while (i<dimensionality_) {
        idx.at(i)++;
        if (idx.at(i)>last.at(i)) {
          idx.at(i)=first.at(i);
          i++;
          if (i==dimensionality_) {
            ready = true;
          }
        } else {
          i=dimensionality_; // ready flag!
        }
      }
    }
  }

  /*
   * read-only access to the element x of the histogram
   * @param x index of the histogram element to be accessed.  It should
   *          be between getFirstCell() and getLastCell()
   * @return the number of entries in the given cell
   */
  template<class T>
  const T& histogram<T>::at(const ivector& x) const {
    return theHistogram_.at(vectorToIndex(x));
  }

  /*
   * access element x of the histogram
   * @param x index of the histogram element to be accessed.  It should
   *          be between getFirstCell() and getLastCell()
   * @return the number of entries in the given cell
   */
  template<class T>
  T& histogram<T>::at(const ivector& x) {
    return theHistogram_.at(vectorToIndex(x));
  }

  /*
   * increment the cell at \e x by the given number of elements (or 1.0 if
   * nothing is explicitly indicated!) and update the number of entries in
   * the histogram.
   * @param x         index of the histogram element to be incremented
   * @param increment amount of the incrementation (default: 1)
   * @return the new number of entries of the incremented cell.
   */
  template<class T>
  const T& histogram<T>::put(const ivector& x,const T& increment) {
    int idx(vectorToIndex(x));
    theHistogram_.at(idx)+=increment;
    numberOfEntries_ += increment;
    return theHistogram_.at(idx);
  }

  /*
   * read-only access to the element x of the histogram as a discrete
   * probability distribution term.  This is equivalent to \f$at(x)/n\f$,
   * where \e n is the number of entries in the histogram (see
   * getNumberOfEntries()).
   *
   * @param x index of the histogram element to be accessed.  It should
   *          be between getFirstCell() and getLastCell()
   * @return the probabilty of the element x, respect to the histogram
   *         data.
   */
  template<class T>
  double histogram<T>::getProbability(const ivector& x) const {
    int idx(vectorToIndex(x));
    return static_cast<double>(theHistogram_.at(idx))/numberOfEntries_;
  }

  /*
   * assigment operator.
   * copy the contents of <code>other</code> in this %object.
   * @param other the source histogram to be copied.
   * @return a reference to this object
   */
  template<class T>
  histogram<T>& histogram<T>::copy(const histogram<T>& other) {

    totalNumberOfCells_ = other.totalNumberOfCells_;
    numberOfEntries_ = other.numberOfEntries_;
    dimensionality_ = other.dimensionality_;
    theHistogram_.copy(other.theHistogram_);
    theCellsPerDimension_.copy(other.theCellsPerDimension_);
    firstCell_.copy(other.firstCell_);
    lastCell_.copy(other.lastCell_);

    return *this;
  }

  /*
   * create a clone of this histogram
   * @return a pointer to a copy of this histogram
   */
  template<class T>
  histogram<T>* histogram<T>::clone() const {
    return new histogram<T>(*this);
  }

  template<class T>
  histogram<T>* histogram<T>::newInstance() const {
    return new histogram<T>;
  }
  
  template<class T>
  const std::string& histogram<T>::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  /*
   * compare this histogram with another one.
   *
   * @param other the other histogram to be compared with
   * @return true if both histograms have the same elements and same size
   */
  template<class T>
  bool histogram<T>::equals(const histogram<T>& other) const {
    return (( totalNumberOfCells_ == other.totalNumberOfCells_) &&
            ( dimensionality_ == other.dimensionality_) &&
            ( theHistogram_.equals(other.theHistogram_) ) &&
            ( theCellsPerDimension_.equals(other.theCellsPerDimension_)));
  }

  /*
   * compare this histogram with another one, and use the given tolerance to
   * determine if the value of each element of the other histogram
   * approximately equals the values of the actual histogram elements.
   *
   * An element \e x is approximately equal to another element \e y
   * with a tolerance \e t, if following equation holds:
   * <i>x</i>-t < <i>y</i> < <i>x</i>+t
   *
   * @param other the other histogram to be compared with
   * @param tolerance the tolerance to be used
   *
   * @return true if both histograms are approximatly equal
   */
  template<class T>
  bool histogram<T>::prettyCloseTo(const histogram<T>& other,
                                const T& tolerance) const {
    return (( totalNumberOfCells_ == other.totalNumberOfCells_) &&
            ( dimensionality_ == other.dimensionality_) &&
            ( theHistogram_.prettyCloseTo(other.theHistogram_,tolerance )) &&
            ( theCellsPerDimension_.equals(other.theCellsPerDimension_)));
  }

  /*
   * applies a C-function to each element of the histogram.
   * @param function a pointer to a C-function
   * @return a reference to the actual histogram
   */
  template<class T>
  histogram<T>& histogram<T>::apply(T (*function)(T)) {
    theHistogram_.apply(function);
    return (*this);
  }

  /*
   * applies a C-function to each element of the histogram.
   * @param function a pointer to a C-function
   * @return a reference to the actual histogram
   */
  template<class T>
  histogram<T>& histogram<T>::apply(T (*function)(const T&)) {
    theHistogram_.apply(function);
    return (*this);
  }

  /*
   * Elementwise multiplication.
   *
   * Each element of this histogram will be multiplied with the elements
   * of the other histogram and the result will be left in this %object!
   *
   * Both histograms are first normalized, then multiplied. After the
   * multiplication, this histogram cannot be interpreted anymore as a
   * histogram, but as a combined probability distribution.  You can
   * use setNumberOfEntries() to change this fact under your own risk
   * (the semathical meaning of that is left to you!).
   *
   * The returned histogram is this %object!
   *
   * @param other the other histogram to be multiplied with
   * @return a reference to the actual histogram
   */
  template<class T>
  histogram<T>& histogram<T>::emultiply(const histogram<T>& other) {
    assert ( totalNumberOfCells_ == other.totalNumberOfCells_ );
    assert ( dimensionality_ == other.dimensionality_);
    int i;

    for (i=0;i<totalNumberOfCells_;i++) {
      theHistogram_.at(i)/=numberOfEntries_;
      theHistogram_.at(i)*=(other.theHistogram_.at(i)/other.numberOfEntries_);
    }

    numberOfEntries_=theHistogram_.computeSumOfElements();

    return *this;
  }

  /*
   * Elementwise multiplication.
   * This histogram will contain the elementwise multiplication of the
   * elements in <code>first</code> and <code>second</code>.
   *
   * Both histograms are first normalized, then multiplied, and the number of
   * entries is after the multiplication 1!  After this multiplication, this
   * histogram cannot be anymore interpreted as a histogram, but as a
   * combined probabilty distribution.  You can use setNumberOfEntries() to
   * change this fact under your own risk (the semathical meaning of that is
   * left to you!).
   *
   * @param first the first histogram
   * @param second the second histogram will be multiplied with the
   *               first histogram
   * @return a reference to the actual histogram
   */
  template<class T>
  histogram<T>& histogram<T>::emultiply(const histogram<T>& first,
                                          const histogram<T>& second) {
    assert ( first.totalNumberOfCells_ == second.totalNumberOfCells_);
    assert ( first.dimensionality_     == second.dimensionality_);

    int i;
    theHistogram_.copy(first.theHistogram_);

    for (i=0;i<totalNumberOfCells_;i++) {
      theHistogram_.at(i)/=numberOfEntries_;
      theHistogram_.at(i)*=(second.theHistogram_.at(i)/second.numberOfEntries_);
    }


    totalNumberOfCells_ = first.totalNumberOfCells_;
    numberOfEntries_=theHistogram_.computeSumOfElements();
    dimensionality_ = first.dimensionality_;
    theCellsPerDimension_.copy(first.theCellsPerDimension_);
    firstCell_.copy(first.firstCell_);
    lastCell_.copy(first.lastCell_);

    return *this;
  }

  /*
   * Add another histogram of the same type and same dimension and
   * leave the result in this %object.  The number of entries of both
   * histograms are also added.
   *
   * @param other the other histogram to be added with
   * @return a reference to the actual histogram
   */
  template<class T>
  histogram<T>& histogram<T>::add(const histogram<T>& other) {
    assert ( totalNumberOfCells_ == other.totalNumberOfCells_ );
    assert ( dimensionality_     == other.dimensionality_);

    theHistogram_.add(other.theHistogram_);
    numberOfEntries_+=other.numberOfEntries_;

    return *this;
  }

  /*
   * Add two histogram and leave the result in this %object.
   * @param first the first histogram.  The number of entries of both
   * histograms are also added.
   * @param second the second histogram will be added with the first
   *               histogram
   * @return a reference to the actual histogram
   */
  template<class T>
  histogram<T>& histogram<T>::add(const histogram<T>& first,
                                    const histogram<T>& second) {
    assert ( first.totalNumberOfCells_ == second.totalNumberOfCells_);
    assert ( first.dimensionality_     == second.dimensionality_);
   
    theHistogram_.add(first.theHistogram_,second.theHistogram_);
    totalNumberOfCells_ = first.totalNumberOfCells_;
    numberOfEntries_ = first.numberOfEntries_+second.numberOfEntries_;
    dimensionality_ = first.dimensionality_;
    theCellsPerDimension_.copy(first.theCellsPerDimension_);
    firstCell_.copy(first.firstCell_);
    lastCell_.copy(first.lastCell_);
    
    return *this;
  }

  /*
   * Subtracts another histogram of the same type and same dimension
   * and leaves the result in this %object
   *
   * @param other will be substracted from this histogram
   * @return a reference to the actual histogram
   */
  template<class T>
  histogram<T>& histogram<T>::subtract(const histogram<T>& other) {
    assert ( totalNumberOfCells_ == other.totalNumberOfCells_ );
    assert ( dimensionality_     == other.dimensionality_);

    theHistogram_.subtract(other.theHistogram_);
    numberOfEntries_-=other.numberOfEntries_;

    return *this;
  }

  /*
   * Subtracts two histograms and leaves the result in this %object.
   * @param first the first histogram
   * @param second the second histogram will be substracted from the
   *                   first histogram
   * @return a reference to the actual histogram
   */
  template<class T>
  histogram<T>& histogram<T>::subtract(const histogram<T>& first,
                                         const histogram<T>& second) {
    assert ( first.totalNumberOfCells_ == second.totalNumberOfCells_ );
    assert ( first.dimensionality_     == second.dimensionality_ );
    
    theHistogram_.subtract(first.theHistogram_,second.theHistogram_);

    totalNumberOfCells_ = first.totalNumberOfCells_;
    numberOfEntries_ = first.numberOfEntries_-second.numberOfEntries_;
    dimensionality_ = first.dimensionality_;
    theCellsPerDimension_.copy(first.theCellsPerDimension_);
    firstCell_.copy(first.firstCell_);
    lastCell_.copy(first.lastCell_);

    return *this;
  }

  /*
   * Multiply this histogram with a constant.
   * Returns this histogram.  The total number of entries will also be
   * updated. Note that if you use this operation, the number of entries
   * could be false at the end due to numerical instabilities.
   *
   * @see setNumberOfEntries()
   *
   * @param cst constant scalar to be multiplied with
   * @return a reference to the actual histogram
   */
  template<class T>
  histogram<T>& histogram<T>::multiply(const T& cst) {
    const T cstm1 = cst - static_cast<T>(1);

    for (int i=0;i<totalNumberOfCells_;i++) {
      numberOfEntries_+=(theHistogram_.at(i)*cstm1);
      theHistogram_.at(i)*=cst;
    }

    return *this;
  }

  /*
   * Multiply the other %histogram with a constant and leave the result here.
   * Returns a reference to this histogram.
   * Note that if you use this operation, the number of entries
   * could be false at the end due to numerical instabilities.
   * @see setNumberOfEntries()
   *
   * @param other the other histogram to be multiplied with the
   *              constant value
   * @param cst constant scalar to be multiplied with the other histogram.
   * @return a reference to the actual histogram
   */
  template<class T>
  histogram<T>& histogram<T>::multiply(const histogram<T>& other,
                                         const T& cst) {
    copy(other);
    return multiply(cst);
  }

  /* free the data of this object and attach it to the "receiver".
   *
   * It is a very efficient way to make a copy of this histogram, if you
   * don't need the source data anymore!
   *
   * At the end of the detachment, this histogram will be empty.
   * @param receiver the histogram which will receive the memory.
   *                 this histogram!
   */
  template<class T>
  void histogram<T>::detach(histogram<T>& receiver) {
    receiver.totalNumberOfCells_ = totalNumberOfCells_;
    receiver.numberOfEntries_ = numberOfEntries_;
    receiver.dimensionality_ = dimensionality_;

    totalNumberOfCells_ = 0;
    numberOfEntries_ = 0;
    dimensionality_ = 0;

    theHistogram_.detach(receiver.theHistogram_);
    theCellsPerDimension_.detach(receiver.theCellsPerDimension_);
    firstCell_.detach(receiver.firstCell_);
    lastCell_.detach(receiver.lastCell_);
  }

  /*
   * Divide this histogram with a constant.
   * Returns this histogram.  @see setNumberOfEntries()
   *
   * @param cst the elements of the histogram will be divided with this
   *            constant
   * @return a reference to the actual histogram
   */
  template<class T>
  histogram<T>& histogram<T>::divide(const T& cst) {
    const double cstm1 = (1.0/double(cst)) - 1.0;
    double delta = 0;

    for (int i=0;i<totalNumberOfCells_;i++) {
      delta+=cstm1*theHistogram_.at(i);
      theHistogram_.at(i)/=cst;
    }
    numberOfEntries_+=static_cast<T>(delta);
    return *this;
  }

  /*
   * Divide the other histogram with a constant and leave the result here.
   * Returns a reference to this histogram.
   * @param other the histogram to be divide by the constant value
   * @param cst the elements of the histogram will be divided with this
   *            constant
   * @return a reference to the actual histogram
   */
  template<class T>
  histogram<T>& histogram<T>::divide(const histogram<T>& other,
                                       const T& cst) {
    copy(other);
    return divide(cst);
  }

  /*
   * Add constant to this histogram.  This histogram is changed.
   * Returns this histogram.
   * @param cst constant scala to be added with each element
   * @return a reference to the actual histogram
   */
  template<class T>
  histogram<T>& histogram<T>::add(const T& cst) {
    theHistogram_.add(cst);
    numberOfEntries_ += (cst*totalNumberOfCells_);
    return *this;
  }

  /*
   * Add constant to the other histogram and leave the result here.
   * Returns a reference to this histogram.
   * @param other the oder histogram
   * @param cst constant scala to be added with each element of the other
   *            histogram
   * @return a reference to the actual histogram
   */
  template<class T>
  histogram<T>& histogram<T>::add(const histogram<T>& other,const T& cst) {
    copy(other);
    return add(cst);
  }

  /*
   * normalize the histogram
   * the total number of entries will be set to 1.0
   * @return a reference to the actual histogram
   */
  template<class T>
  histogram<T>& histogram<T>::normalize(const bool forceUpdateOfNumEntries) {
    
    if (forceUpdateOfNumEntries) {
      updateNumberOfEntries();
    }

    theHistogram_.divide(numberOfEntries_);
    numberOfEntries_ = static_cast<T>(1.0);

    return *this;
  }


  /**
   * write the object in the given ioHandler
   */
  template<class T>
  bool histogram<T>::write(ioHandler& handler,const bool complete) const {
    bool b = true;
    if (complete) b = handler.writeBegin();

    if (b) {
      lti::write(handler,"totalNumberOfCells",totalNumberOfCells_);
      lti::write(handler,"numberOfEntries",numberOfEntries_);
      lti::write(handler,"dimensionality",dimensionality_);
      lti::write(handler,"data",theHistogram_);
      lti::write(handler,"cellsPerDimension",theCellsPerDimension_);
      lti::write(handler,"firstCell",firstCell_);
      lti::write(handler,"lastCell",lastCell_);
    }

    if (complete) b = handler.writeEnd();
    return b;
  }

  /**
   * read the object from the given ioHandler
   */
  template<class T>
  bool histogram<T>::read(ioHandler& handler,const bool complete) {
    bool b = true;
    if (complete) b = handler.readBegin();

    if (b) {
      lti::read(handler,"totalNumberOfCells",totalNumberOfCells_);
      lti::read(handler,"numberOfEntries",numberOfEntries_);
      lti::read(handler,"dimensionality",dimensionality_);
      lti::read(handler,"data",theHistogram_);
      lti::read(handler,"cellsPerDimension",theCellsPerDimension_);
      lti::read(handler,"firstCell",firstCell_);
      lti::read(handler,"lastCell",lastCell_);
    }

    if (complete) b = handler.readEnd();
    return b;
  }
  
} // namespace lti

/// outputs the elements of the histogram on a stream
template <class T>
std::ostream& operator<<(std::ostream& s,const lti::histogram<T>& v) {

  lti::ivector idx(v.getFirstCell()); // the index! this will iterate on
                                      // the histogram hyperbox

  const lti::ivector& first = v.getFirstCell();
  const lti::ivector& last = v.getLastCell();

  // index of the last dimension
  const int dims = v.dimensions();
  int i,j;
  bool ready = false;

  // string with spaces!
  char* spc = new char[dims+1];
  for (i=0;i<dims;i++) {
    spc[i]=' ';
  }
  spc[dims]=0;

  j = dims;

  // to check if index at the end, we just need to check the last
  // dimension of the index
  while ( !ready ) {

    // j indicates how many "(" must be written

    if (j>0) {
      s << std::endl;

      for (i=dims;i>0;i--) {
        if (i>j)
          s << ' ';
        else
          s << '(';
      }
    }

    s << v.at(idx) << "\t ";

    // idx update
    i = j = 0;

    while (i<dims) {
      idx.at(i)++;
      if (idx.at(i)>last.at(i)) {
        s << ")";
        j++;
        idx.at(i)=first.at(i);
        i++;
        if (i==dims) {
          ready = true;
        }
      } else {
        i=dims; // ready flag!
      }
    }

    // j indicates how many ")" were written.
  }

  return s;
}

