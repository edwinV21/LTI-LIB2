/*
 * Copyright (C) 1998 - 2005
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
 * \file   ltiKdTree_template.h
 *         Contains the implementation for class lti::kdTree
 *         which implements the k dimensional binary tree for fast searching
 * 
 * \author Pablo Alvarado
 * \author Frederik Lange
 * \author Jens Rietzschel
 * \date   09.01.2003
 *
 * revisions ..: $Id: ltiKdTree_template.h,v 1.10 2012-09-16 04:58:33 alvarado Exp $
 */

// MS VC++ defines min and max somewhere.  This conflicts with the LTI-Lib
// definition of min and max as functions, and with the std::numeric_limits,
// which have methods min and max.  So let's get rid of this awful old C
// stuff:
#undef min
#undef max

#include <limits>


#ifdef _LTI_DEBUG
#include "ltiLispStreamHandler.h"
// a lisp stream handler used to output some elements
static lti::lispStreamHandler _lti_debug_lsh(std::cerr);
#endif

namespace lti {
  // --------------------------------------------------
  // kdTree::element
  // --------------------------------------------------
  
  // default constructor
  template<typename T,typename D,class U>
  kdTree<T,D,U>::element::element() : ioObject(),point(T()),data(D()) {
  }

  // constructor
  template<typename T,typename D,class U>
  kdTree<T,D,U>::element::element(const T& pos, const D& dat)
    : ioObject(),point(pos),data(dat) {
  }

  // copy constructor
  template<typename T,typename D,class U>
  kdTree<T,D,U>::element::element(const element& other) 
    : ioObject(),point(other.point),data(other.data) {
  }

  // destructor
  template<typename T,typename D,class U>
  kdTree<T,D,U>::element::~element() {
  }

  // name
  template <typename T,typename D,class U>
  const std::string& kdTree<T,D,U>::element::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone function
  template<typename T,typename D,class U>
  typename kdTree<T,D,U>::element* kdTree<T,D,U>::element::clone() const {
    return new element(*this);
  }

  // newInstance function
  template<typename T,typename D,class U>
  typename kdTree<T,D,U>::element* 
  kdTree<T,D,U>::element::newInstance() const {
    return new element();
  }

  // read
  template<typename T,typename D,class U>
  bool kdTree<T,D,U>::element::read(ioHandler &handler,
                                    const bool complete) {
    bool b=true;

    if (complete) {
      b = handler.readBegin();
    }

    b = b && lti::read(handler,point);
    b = b && handler.readDataSeparator();
    b = b && lti::read(handler,data);

    if (complete) {
      b= b && handler.readEnd();
    }

    return b;
  }

  // write
  template<typename T,typename D,class U>
  bool kdTree<T,D,U>::element::write(ioHandler& handler,
                                     const bool complete) const {
    bool b=true;

    if (complete) {
      b = handler.writeBegin();
    }

    b = b && lti::write(handler,point);
    b = b && handler.writeDataSeparator();
    b = b && lti::write(handler,data);

    if (complete) {
      b= b && handler.writeEnd();
    }

    return b;
  }


  // --------------------------------------------------
  // kdTree::node
  // --------------------------------------------------

  // default constructor
  template<typename T,typename D,class U>
  kdTree<T,D,U>::node::node() 
    : ioObject(),points(),left(0),right(0),splitDim(0),
      partition(value_type()) {
  }

  // copy constructor
  template<typename T,typename D,class U>
  kdTree<T,D,U>::node::node(const node& other) 
    : ioObject(),points(),left(0),right(0) {
    copy(other);
  }

  // default constructor
  template<typename T,typename D,class U>
  kdTree<T,D,U>::node::~node() {
    clearPoints();
    
    if(left!=0) {
      delete left;
      left=0;
    }
    if(right!=0) {
      delete right;
      right=0;
    }
  }
  
  // copy
  template<typename T,typename D,class U>
  typename kdTree<T,D,U>::node& kdTree<T,D,U>::node::copy(const node& other) {

    // copy the data
    splitDim = other.splitDim;
    partition = other.partition;

    // remove the current elements
    clearPoints();
    
    // deep copy the other elements
    typename points_type::const_iterator cit,ceit;
    for (cit=other.points.begin(),ceit=other.points.end();
         cit!=ceit;
         ++cit) {
      points.push_back((*cit)->clone());
    }
    
    // remove old data
    if (notNull(left)) {
      delete left;
      left = 0;
    }
    
    if (notNull(right)) {
      delete right;
      right = 0;
    }
    
    // copy children if necessary.
    if (notNull(other.left)) {
      left = other.left->clone();
    }
    
    if (notNull(other.right)) {
      right = other.right->clone();
    }
    
    return *this;
  }
  
  // name
  template<typename T,typename D,class U>
  const std::string& kdTree<T,D,U>::node::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone
  template<typename T,typename D,class U>
  typename kdTree<T,D,U>::node* kdTree<T,D,U>::node::clone() const {
    return new node(*this);
  }

  // newInstance
  template<typename T,typename D,class U>
  typename kdTree<T,D,U>::node* kdTree<T,D,U>::node::newInstance() const {
    return new node();
  }

  // read
  template<typename T,typename D,class U>
  bool kdTree<T,D,U>::node::read(ioHandler &handler,
                                 const bool complete) {
    bool b=true;
    
    delete left;
    left = 0;
    delete right;
    right = 0;
    
    if (complete) {
      b = handler.readBegin();
    }
    
    b = b && lti::read(handler,"splitDim",splitDim);
    b = b && lti::read(handler,"partition",partition);
    
    // read the points with their corresponding data
    clearPoints();
    
    // -----------------------------------------------------
    
    b = b && handler.readBegin();              // points field (1)
    b = b && handler.trySymbol("points");      //
    b = b && handler.readKeyValueSeparator();  //
    b = b && handler.readBegin();              // vector data  (2)
    
    // -----------------------------
    int i;
    element* tmpElem;
    int pointsSize;
    b = b && handler.read("size",pointsSize);
    
    points.resize(pointsSize);
    
    b = b && handler.readBegin(); // data block of vector      (3)
    if (pointsSize > 0) {
      for (i=0;i<pointsSize-1;++i) {
        tmpElem = new element;
        b = b && tmpElem->read(handler);
        points[i] = tmpElem;
        tmpElem=0;
        b = b && handler.readDataSeparator();
      }
      tmpElem = new element;
          b = b && tmpElem->read(handler);
          points[i] = tmpElem;
          tmpElem=0;
    }
    b = b && handler.readEnd();   // data block of vector      (2)
        
    // -----------------------------
    
    b = b && handler.readEnd(); // vector data                 (1)
    b = b && handler.readEnd(); // points field                (0)
    
    // -----------------------------------------------------
    
    
    // load the children
    
    
    // left node first
    b = b && handler.readBegin();              // left scope  (1)
    b = b && handler.trySymbol("left");        //
    b = b && handler.readKeyValueSeparator();  //
    b = b && handler.readBegin();              // left data   (2)
    //
    if (!handler.tryEnd()) {                   // 
      left = new node;                         //
      // recursively read the children         //
      b = b && left->read(handler,false);      //
      b = b && handler.readEnd();              // left data   (1)
    }                                          //
    //
    b = b && handler.readEnd();                // left scope  (0)
    
    // right node next
    b = b && handler.readBegin();              // right scope (1)
    b = b && handler.trySymbol("right");       //
    b = b && handler.readKeyValueSeparator();  //
    b = b && handler.readBegin();              // right data  (2)
    //
    if (!handler.tryEnd()) {                   // 
      right = new node();                      //
      // recursively read the children         //
      b = b && right->read(handler,false);     //
      b = b && handler.readEnd();              // right data  (1)
    }                                          //
    //
    b = b && handler.readEnd();                // right scope (0)
    
    if (complete) {
      b = b && handler.readEnd();
    }
    
    return b;
  }

  template<typename T,typename D,class U>
  bool kdTree<T,D,U>::node::write(ioHandler& handler,
                                  const bool complete) const {
    bool b=true;
    
    if (complete) {
      b = handler.writeBegin();
    }
    
    b = b && lti::write(handler,"splitDim",splitDim);
    b = b && lti::write(handler,"partition",partition);
    
    // write from the points only the index
    // -------------------------------------------------------
    
    b = b && handler.writeBegin();             // points field
    b = b && handler.writeSymbol("points");    //
    b = b && handler.writeKeyValueSeparator(); //
    //
    b = b && handler.writeBegin();             // vector data

    // ------------------------------------------
    b = b && handler.write("size",static_cast<int>(points.size()));
    b = b && handler.writeBegin(); // data block of vector
    if (!points.empty()) {                     //      |
      typename points_type::const_iterator it; //      |
      it=points.begin();                       //      |
      b = b && (*it)->write(handler);          //      |
      for (++it;it!=points.end();++it) {       //      |
        b = b && handler.writeDataSeparator(); //      |
        b = b && (*it)->write(handler);        //      |
      }                                        //      |
    }                                          //      |
    b = b && handler.writeEnd();   // data block of vector
        
    // -------------------------------------------

    b = b && handler.writeEnd(); // vector data
    b = b && handler.writeEnd(); // points field

    // -------------------------------------------------------

    b = b && handler.writeEOL();  
        
    // Save the children

    // left node first
    b = b && handler.writeBegin();             // left child   (1)
    b = b && handler.writeSymbol("left");      //
    b = b && handler.writeKeyValueSeparator(); //
    b = b && handler.writeBegin();             // node block   (2)
    if (notNull(left)) {                       //
      // recursivelly save the children        // 
      b = b && left->write(handler,false);     //
    }                                          //
    b = b && handler.writeEnd();               // node block   (1)
    b = b && handler.writeEnd();               // left child   (0)
    b = b && handler.writeEOL();  
        
    // right node next
    b = b && handler.writeBegin();             // right child  (1)
    b = b && handler.writeSymbol("right");     //
    b = b && handler.writeKeyValueSeparator(); //
    b = b && handler.writeBegin();             // node block   (2)
    if (notNull(right)) {                      //
      // recursivelly save the children        //
      b = b && right->write(handler,false);    //
    }                                          //
    b = b && handler.writeEnd();               // node block   (1)
    b = b && handler.writeEnd();               // right child  (0)

    b = b && handler.writeEOL();

    if (complete) {
      b=b && handler.writeEnd();
    }

    return b;
  }

  template<typename T,typename D,class U>
  void kdTree<T,D,U>::node::subdivide(const int maxCount,
                                      int& levels, int& leaves) {
    const int n = static_cast<const int>(points.size());
    if (n <= maxCount) {
      // we don't need to split.  This now is small enough
      leaves++;
      levels=1;
      return;
    }

    // remove the old children and create empty ones.
    if (notNull(left)) {
      delete left;
      left = 0;
    }
    left = new node();

    if (notNull(right)) {
      delete right;
      right = 0;
    }
    right = new node();      

    // split dimension at this node
    // remember split dimension (splitDim is a class attribute)
    const int dim = splitDim = getDimWithHighestVariance();

    // get the median of the splitDim as the value to split the space
    // remember split position
    const value_type medVal = partition = getMedianVal(dim);

    const int lc = n/2;
    int npleft(0),npeq(0); // number of points in the left side
    typename points_type::iterator it=points.begin();
    std::list<element*> stack;
    // split the points into the two children. 
    while (it!=points.end()) {
      if (((*it)->point[dim] < medVal)) {
        left->add(*it);
        npleft++;
      } else if (((*it)->point[dim] > medVal)) {
        right->add(*it);
      } else {
        npeq++;
        stack.push_back(*it);
      }
      ++it;
    }

    // the remaining median-equal values need to be split so
    // that both children have the same number of points
    while (npleft<lc) {
      left->add(stack.front());
      stack.pop_front();
      npleft++;
    }
    if (!stack.empty()) {
      right->add(stack);
    }
        
    points.clear();

    // recursively split the data into both child nodes
    int llev,rlev;
    left->subdivide(maxCount,llev,leaves);
    right->subdivide(maxCount,rlev,leaves);
    levels = 1+max(llev,rlev);
  }

  template<typename T,typename D,class U>
  int kdTree<T,D,U>::node::getDimWithHighestVariance() const {
    assert(!points.empty() && notNull(points.front()));

    // get the used dimension
    const int dim = points.front()->size();

    assert(dim>0);
        
    // total number of points
    const int n = static_cast<int>(points.size());

    // type for squares
    typedef typename typeInfo<value_type>::square_accumulation_type
      sqrType;

    // sum of all elements
    vector<sqrType> sum(dim,sqrType(0));
    // variance
    vector<sqrType> var(dim,sqrType(0));

    typename points_type::const_iterator it;
    int mxi,j;
    sqrType tmp,mx;

    // compute some temporary variables with the sum of elements and
    // the sum of square of elements
    for (it=points.begin();it!=points.end();++it) {
      // we cannot just add the points, because we don't really know
      // the type.  So let's add the elements separately
      for (j=0;j<dim;++j) {
        tmp=(*(*it))[j];
        sum[j]+=tmp;
        var[j]+=(tmp*tmp);
      }
    }

    // compute not the variance, but m x variance, to save some time,
    // and get the dimension where the maximal one is found
    mxi=0;
    tmp=sum[mxi];
    mx = var[mxi]-(tmp*tmp/n); // the first "variance" for dimension 0
    for (j=1;j<dim;++j) {      // the rest of "variances"
      tmp = sum[j];
      tmp = var[j]-(tmp*tmp/n);
      if (tmp>mx) {            // if greater
        mxi=j;                 // remember it.
        mx=tmp;
      }
    }

    return mxi;
  }

  template<typename T,typename D,class U>
  inline typename kdTree<T,D,U>::value_type 
  kdTree<T,D,U>::node::getMedianVal(const int searchDim) const {
    int i;
    const int n = static_cast<int>(points.size());
    
    vector<typename kdTree<T,D,U>::value_type> val(n); // don't initialize
    
    // get elements in the split dimension
    typename points_type::const_iterator it;
    for (i=0,it=points.begin(); it!=points.end();++it,++i) {
      val[i] = (*(*it))[searchDim];
    }
    
    // the quickMedian functor can be created just once and const,
    // because we don't need to change the parameters and the required
    // apply is const.  Even multi-thread applications would use different
    // stack-frames, and should work. (This should be tested!!!)
    static const quickMedian qmed;
    
    // get the median of desired dimension
    return qmed.median(val);
  }
  
  // --------------------------------------------------
  // kdTree
  // --------------------------------------------------

  // default constructor
  template<typename T,typename D,class U>
  kdTree<T,D,U>::kdTree()
    : container(),status(),root_(0),levels_(0),
      numElements_(0),numAddedElements_(0),numLeaves_(0) {
  }

  // copy constructor
  template<typename T,typename D,class U>
  kdTree<T,D,U>::kdTree(const kdTree<T,D,U>& other) 
    : container(),status(),root_(0),levels_(0),
      numElements_(0),numAddedElements_(0),numLeaves_(0) {
    copy(other);
  }

  // destructor
  template<typename T,typename D,class U>
  kdTree<T,D,U>::~kdTree() {
    clear();
  }

  // copy member
  template<typename T,typename D,class U>
  bool kdTree<T,D,U>::empty() const {
    return (isNull(root_));
  }

  /*
   * get the number of elements added to the tree
   */
  template<typename T,typename D,class U>
  int kdTree<T,D,U>::getNumberOfAddedElements() const {
    return numAddedElements_;
  }

  template<typename T,typename D,class U>
  int kdTree<T,D,U>::size() const {
    return numElements_;    
  }
  
  /*
   * get the number of leaves in the tree
   */
  template<typename T,typename D,class U>
  int kdTree<T,D,U>::getNumberOfLeaves() const {
    if (isNull(root_)) {
      return 0;
    } else {
      return numLeaves_;
    }
  }

  /*
   * get the number of levels of the tree
   */
  template<typename T,typename D,class U>
  int kdTree<T,D,U>::getNumberOfLevels() const {
    if (isNull(root_)) {
      return 0;
    } else {
      return levels_;
    }
  }

  /*
   *  get pointer to root node
   */
  template<typename T, typename D, class U>
  typename kdTree<T,D,U>::node* kdTree<T,D,U>::getRoot(){
      return root_;
  }


  // copy member
  template<typename T,typename D,class U>
  kdTree<T,D,U>&
  kdTree<T,D,U>::copy(const kdTree<T,D,U>& other) {
//    container::copy(other);

    clear();

    if (notNull(root_)) {

      if (!other.empty()) {
        root_->copy(*other.root_);
      }

    } else {

      if (!other.empty()) {
        root_ = other.root_->clone();
      }

    }

    // we also have to copy the elements added since the last call to
    // build() or rebuild()
    if (!other.treePoints_.empty()) {
      typename std::list<element*>::const_iterator it 
        = other.treePoints_.begin();
      const typename std::list<element*>::const_iterator ite 
        = other.treePoints_.end();
      while (it!=ite) {
        treePoints_.push_back((*it)->clone());
        ++it;
      }
    }

    totalBounds_.copy(other.totalBounds_);
    bounds_.copy(totalBounds_);
    minBounds_.useExternData(bounds_.columns(),bounds_.data());
    maxBounds_.useExternData(bounds_.columns(),&bounds_.at(1,0));


    levels_ = other.levels_;
    numElements_ = other.numElements_;
    numAddedElements_ = other.numAddedElements_;
    numLeaves_ = other.numLeaves_;

    return (*this);
  }

  // name
  template<typename T,typename D,class U>
  const std::string& kdTree<T,D,U>::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // alias for copy member
  template<typename T,typename D,class U>
  kdTree<T,D,U>& kdTree<T,D,U>::operator=(const kdTree<T,D,U>& other) {
    return (copy(other));
  }

  // clone member
  template<typename T,typename D,class U>
  kdTree<T,D,U>* kdTree<T,D,U>::clone() const {
    return new kdTree(*this);
  }

  // new instance member
  template<typename T,typename D,class U>
  kdTree<T,D,U>* kdTree<T,D,U>::newInstance() const {
    return new kdTree();
  }

  // clear member
  template<typename T,typename D,class U>
  void kdTree<T,D,U>::clear() {
    // delete all nodes
    delete root_;
    root_=0;

    // delete unassigned data nodes
    typename std::list<element*>::iterator it,eit;
    for (it=treePoints_.begin(),eit=treePoints_.end();it!=eit;++it) {
      delete (*it);
      (*it)=0;
    }
    treePoints_.clear();

    levels_=0;
    numElements_=0;
    numAddedElements_=0;
    numLeaves_=0;
  }

  template<typename T,typename D,class U>
  bool kdTree<T,D,U>::write(ioHandler &handler, 
                          const bool complete) const {
    bool b=true;

    if (complete) {
      b = handler.writeBegin();
    }

    // 
    b = b && lti::write(handler,"numElements",numElements_);
    b = b && lti::write(handler,"numLeaves",numLeaves_);
    b = b && lti::write(handler,"levels",levels_);
    b = b && lti::write(handler,"pointDim",totalBounds_.columns());

    // write the nodes
    b = b && handler.writeBegin(); // nodes           ----|
    b = b && handler.writeSymbol("nodes");       //       |
    b = b && handler.writeKeyValueSeparator();   //       |
                                   //                     |
    b = b && handler.writeBegin(); // data in the nodes   |
    if (notNull(root_)) {                         //  |    |
      b = b && root_->write(handler,false);       //  |    |
    }                                            //  |    |
                                                 //  |    |
    b = b && handler.writeEnd();   // data in the nodes   |
    b = b && handler.writeEnd();   // nodes           ____|

    if (complete) {
      b = handler.writeEnd();
    }
    
    return b;
  }

  template<typename T,typename D,class U>
  bool kdTree<T,D,U>::read(ioHandler &handler, const bool complete) {
    bool b = true;
    int dim;

    clear(); // remove root and all related stuff

    if (complete) {
      b = handler.readBegin();
    }

    // 
    b = b && lti::read(handler,"numElements",numElements_);
    b = b && lti::read(handler,"numLeaves",numLeaves_);
    b = b && lti::read(handler,"levels",levels_);
    b = b && lti::read(handler,"pointDim",dim);

    // read the nodes
    b = b && handler.readBegin();             // nodes         -----|   
    b = b && handler.trySymbol("nodes");      //                    |
    b = b && handler.readKeyValueSeparator(); //                    |
                                              //                    |
    b = b && handler.readBegin();             // data in the nodes  |
    if (!handler.tryEnd()) {                  //                 |  |
      root_ = new node();                      //                 |  |
      b = b && root_->read(handler,false);     //                 |  |
      b = b && handler.readEnd();             // data in the nodes  |
    }                                         //                    |
                                              //                    |
    b = b && handler.readEnd();               // nodes         _____|

    if (complete) {
      b = handler.readEnd() && b;
    }

    // restore the sizes for the rest of the internal data

    if (numElements_ > 0) {
      totalBounds_.allocate(2,dim);

      if (std::numeric_limits<value_type>::is_integer) {
        // initialize the mins with max
        totalBounds_.getRow(1).fill(std::numeric_limits<value_type>::max());
        
        value_type tmp = std::numeric_limits<value_type>::min();
        
        if (tmp == 0) {
          // initialize the maxs with min --> unsigned types 
          totalBounds_.getRow(0).fill(tmp);
        } else {
          // initialize the maxs with min
          // don't use the absolute minimum:  it cannot be negated and can
          // produce arithmetic errors
          totalBounds_.getRow(0).fill(-std::numeric_limits<value_type>::max());
        }
      

      } else {
        // floating point types
        value_type tmp = sqrt(std::numeric_limits<value_type>::max());
        // initialize the mins with max
        totalBounds_.getRow(1).fill(tmp);
        // initialize the maxs with min
        totalBounds_.getRow(0).fill(-tmp);
      }

      // resize the temporal bounds and indicate memory sharing...
      bounds_.copy(totalBounds_);
      minBounds_.useExternData(bounds_.columns(),bounds_.data());
      maxBounds_.useExternData(bounds_.columns(),&bounds_.at(1,0));
    }

    return b;
  }
  
  template<typename T,typename D,class U>
  void kdTree<T,D,U>::add(const T& point,const D& data) {
    element* e = new element(point,data);
    treePoints_.push_back(e);
    ++numAddedElements_;
  }

  template<typename T,typename D,class U>
  bool kdTree<T,D,U>::rebuild(const int bucketSize) {
    if (bucketSize<1) {
      return false;
    }

    if (treePoints_.empty()) {
      if (isNull(root_)) {
        setStatusString("no data added to kdTree when trying to rebuild\n");
        return false;
      } else {
        //all we could do is build the tree again, which we wont
        return true;
      }
    }

    // check whether there are already elements in the tree.
    // if yes get them out and include in the rebuild of the tree.
    if (!isNull(root_)) {
      std::list<element*> tmp;
      getDataInSubtree(root_, tmp);
      typename std::list<element*>::const_iterator it=tmp.begin();
      typename std::list<element*>::const_iterator ite=tmp.end();
      for (; it!=ite; ++it) {
        treePoints_.push_back((*it)->clone());
      }
      delete root_;
      root_=0;
      numAddedElements_+=numElements_;
    }

    return build(bucketSize);
  }

  template<typename T,typename D,class U>
  bool kdTree<T,D,U>::build(const int bucketSize) {
    if (bucketSize<1) {
      return false;
    }


    if (treePoints_.empty()) {
      setStatusString("no data added to kdTree when trying to build\n");
      return false;
    }

    //reset
    delete root_;
    root_=0;
    numLeaves_=0;
    levels_=0;
    numElements_=numAddedElements_;
    numAddedElements_=0;

    root_=new node;
    const int dim = treePoints_.front()->size();
    root_->add(treePoints_);
    root_->subdivide(bucketSize,levels_,numLeaves_);
    // we must delete the treePoints_ vector, otherwise its elements will
    // be deleted twice
    treePoints_.clear();

    // initialize the bounds
    // this is somehow tricky.  The distances will be manipulated as square
    // values of euclidean distances.  For integer values, this can easily
    // cause overflows in the variable contents when building the square if
    // the absolute max and min values for the types are used.  So, the
    // square roots of those values should be used instead! (they are still
    // big enough)
    totalBounds_.allocate(2,dim);
    // initialize the maxs with min
    if (std::numeric_limits<value_type>::is_integer) {
      // initialize the mins with max
      totalBounds_.getRow(1).fill(std::numeric_limits<value_type>::max());

      value_type tmp = std::numeric_limits<value_type>::min();
      
      if (tmp == 0) {
        // initialize the maxs with min --> unsigned types 
        totalBounds_.getRow(0).fill(tmp);
      } else {
        // initialize the maxs with min
        // don't use the absolute minimum:  it cannot be negated and can
        // produce arithmetic errors
        totalBounds_.getRow(0).fill(-std::numeric_limits<value_type>::max());
      }
      

    } else {
      // floating point types
      value_type tmp = sqrt(std::numeric_limits<value_type>::max());
      // initialize the mins with max
      totalBounds_.getRow(1).fill(tmp);
      // initialize the maxs with min
      totalBounds_.getRow(0).fill(-tmp);
    }

    // resize the temporal bounds and indicate memory sharing...
    bounds_.copy(totalBounds_);
    minBounds_.useExternData(bounds_.columns(),bounds_.data());
    maxBounds_.useExternData(bounds_.columns(),&bounds_.at(1,0));

    return true;
  }


  template<typename T,typename D,class U>
  void kdTree<T,D,U>::getDataInSubtree(node* nodePtr,
                                     std::list<element*>& data) const {
    // if node contains points, append them at the end of this->treePoints_
    if (!(nodePtr->points.empty())) {
      data.insert(data.end(),nodePtr->points.begin(),nodePtr->points.end());
    }
    
    // get data from the left child
    if (notNull(nodePtr->left)) {
      getDataInSubtree(nodePtr->left,data);
    }

    // get data from the right child
    if (notNull(nodePtr->right)) {
      getDataInSubtree(nodePtr->right,data);
    }
  }


  /*
   * search for an element with exactly the given position in the
   * hyperspace.
   *
   * This is the classical search in a kd-tree, that assumes that 
   * a leaf node contains exactly the given point.
   * If found, the contents of the element will by copied in the 
   * elem parameters and true will be returned.   Otherwise false will
   * be returned.
   * @param key point in the n-dimensional space you are looking for.
   * @param elem the contents of the found point will be left here.
   * @return true if key was found, otherwise false.
   */
  template<typename T,typename D,class U>
  bool kdTree<T,D,U>::searchExactly(const T& key,element& elem) const {
    if (notNull(root_)) {
      return searchExactly(root_,key,elem);
    }
    return false;
  }

  template<typename T,typename D,class U>
  bool kdTree<T,D,U>::searchExactly(const node* nptr,
                                  const T& key,element& elem) const {
    if (nptr->isLeaf()) {
      // linearly search for the key in the contained points.
      return nptr->getPoint(key,elem);
    } else {
      if ((key[nptr->splitDim]<nptr->partition) && (notNull(nptr->left))) {
        // if at all there, the key must be on the left subtree
        return searchExactly(nptr->left,key,elem);
      } 
      else if ((key[nptr->splitDim]>nptr->partition) && 
               (notNull(nptr->right))) {
        // if at all there, the key must be on the right subtree
        return searchExactly(nptr->right,key,elem);
      } 
      else if (key[nptr->splitDim]==nptr->partition) {
        // the key can be in any subtree, so we need to look for it everywhere
        if (notNull(nptr->left) && (searchExactly(nptr->left,key,elem))) {
          return true;
        }
        
        if (notNull(nptr->right) && (searchExactly(nptr->right,key,elem))) {
          return true;
        }
      }
   
    }
    return false;
  }

  template<typename T,typename D,class U>
  bool kdTree<T,D,U>::searchExactly(const T& key,
                                  std::list<element>& elem) const {
    elem.clear();
    if (notNull(root_)) {
      return searchExactly(root_,key,elem);
    }
    return false;
  }

  template<typename T,typename D,class U>
  bool kdTree<T,D,U>::searchExactly(const node* nptr,
                                    const T& key,
                                    std::list<element>& elem) const {
    if (nptr->isLeaf()) {
      // linearly search for the key in the contained points.
      return nptr->getPoint(key,elem);
    } else {
      if ((key[nptr->splitDim]<nptr->partition) && (notNull(nptr->left))) {
        // if at all there, the key must be on the left subtree
        return searchExactly(nptr->left,key,elem);
      } 
      else if ((key[nptr->splitDim]>nptr->partition) && 
               (notNull(nptr->right))) {
        // if at all there, the key must be on the right subtree
        return searchExactly(nptr->right,key,elem);
      } 
      else if (key[nptr->splitDim]==nptr->partition) {
        // the key can be in any subtree, so we need to look for it everywhere
        bool res(false);
        if (notNull(nptr->left)) {
          res = searchExactly(nptr->left,key,elem);
        }
        
        if (notNull(nptr->right)) {
          // first search the other node, and after that compute res...
          res = searchExactly(nptr->right,key,elem) || res;
        }
        return res;
      }   
    }
    return false;
  }

  /*
   * search for the nearest element in the tree to the given key point.
   *
   * If found, the contents of the element will by copied in the 
   * elem parameters and true will be returned.   Otherwise false will
   * be returned.
   * @param key point in the n-dimensional space you are looking for.
   * @param elem the contents of the found point will be left here.
   * @return true if key was found, otherwise false (i.e. tree empty).
   */
  template<typename T,typename D,class U>
  bool kdTree<T,D,U>::searchNearest(const T& key,element*& elem) const {
    elem = 0;
    if (notNull(root_)) {
      // the fastest contents copy between the two matrices is required here!
      // without resize (to avoid new memory allocation).
      // use pointer arithmetic.
      // is is equivalent to bounds_.fill(totalBounds_), but avoiding
      // many (here) unnecessary boundary checks.
      const value_type *pt,*pe;
      value_type *p;
      for (pt=totalBounds_.data(),
             pe=&totalBounds_.at(1,maxBounds_.lastIdx()),
             p=minBounds_.data()
             ;pt<=pe;++pt,++p) {
        (*p)=(*pt);
      }

      // clear old data
      std::pair<distance_type,element*> 
        neigh(std::numeric_limits<distance_type>::max(),
              static_cast<element*>(0));
      searchNearest(root_,key,neigh);
      if (notNull(neigh.second)) {
        elem=neigh.second;
        return true; // one neighbor is always found
      }
    }

    return false;
  }

  /*
   * search for the nearest element in the tree to the given key point.
   *
   * If found, the contents of the element will by copied in the 
   * elem parameters and true will be returned.   Otherwise false will
   * be returned.
   * @param key point in the n-dimensional space you are looking for.
   * @param elem the contents of the found point will be left here.
   * @return true if key was found, otherwise false (i.e. tree empty).
   */
  template<typename T,typename D,class U>
  bool kdTree<T,D,U>::searchNearest(const T& key,
                                    element*& elem,
                                    distance_type& dist) const {
    elem = 0;
    if (notNull(root_)) {
      // the fastest contents copy between the two matrices is required here!
      // without resize (to avoid new memory allocation).
      // use pointer arithmetic.
      // is is equivalent to bounds_.fill(totalBounds_), but avoiding
      // many (here) unnecessary boundary checks.
      const value_type *pt,*pe;
      value_type *p;
      for (pt=totalBounds_.data(),
             pe=&totalBounds_.at(1,maxBounds_.lastIdx()),
             p=minBounds_.data();
           pt<=pe;++pt,++p) {
        (*p)=(*pt);
      }

      // clear old data
      std::pair<distance_type,element*> 
        neigh(std::numeric_limits<distance_type>::max(),
              static_cast<element*>(0));
      searchNearest(root_,key,neigh);
      if (notNull(neigh.second)) {
        elem=neigh.second;
        dist=neigh.first;
        return true; // one neighbor is always found
      }
    }

    return false;
  }

  /*
   * search for the nearest element in the tree to the given key point.
   *
   * If found, the contents of the element will by copied in the 
   * elem parameters and true will be returned.   Otherwise false will
   * be returned.
   * @param key point in the n-dimensional space you are looking for.
   * @param elem the contents of the found point will be left here.
   * @return true if key was found, otherwise false (i.e. tree empty).
   */
  template<typename T,typename D,class U>
  bool kdTree<T,D,U>::searchNearest(const T& key,element& elem) const {

    if (notNull(root_)) {
      // the fastest contents copy between the two matrices is required here!
      // without resize (to avoid new memory allocation).
      // use pointer arithmetic.
      // is is equivalent to bounds_.fill(totalBounds_), but avoiding
      // many (here) unnecessary boundary checks.
      const value_type *pt,*pe;
      value_type *p;
      for (pt=totalBounds_.data(),
             pe=&totalBounds_.at(1,maxBounds_.lastIdx()),
             p=minBounds_.data();
           pt<=pe;++pt,++p) {
        (*p)=(*pt);
      }

      // clear old data
      std::pair<distance_type,element*> 
        neigh(std::numeric_limits<distance_type>::max(),
              static_cast<element*>(0));
      searchNearest(root_,key,neigh);
      if (notNull(neigh.second)) {
        elem=*(neigh.second);
        return true; // one neighbor is always found
      }
    }

    return false;
  }

  /*
   * search for the nearest element in the tree to the given key point.
   *
   * If found, the contents of the element will by copied in the 
   * elem parameters and true will be returned.   Otherwise false will
   * be returned.
   * @param key point in the n-dimensional space you are looking for.
   * @param elem the contents of the found point will be left here.
   * @return true if key was found, otherwise false (i.e. tree empty).
   */
  template<typename T,typename D,class U>
  bool kdTree<T,D,U>::searchNearest(const T& key,
                                    element& elem,
                                    distance_type& dist) const {

    if (notNull(root_)) {
      // the fastest contents copy between the two matrices is required here!
      // without resize (to avoid new memory allocation).
      // use pointer arithmetic.
      // is is equivalent to bounds_.fill(totalBounds_), but avoiding
      // many (here) unnecessary boundary checks.
      const value_type *pt,*pe;
      value_type *p;
      for (pt=totalBounds_.data(),
             pe=&totalBounds_.at(1,maxBounds_.lastIdx()),
             p=minBounds_.data();
           pt<=pe;++pt,++p) {
        (*p)=(*pt);
      }

      // clear old data
      std::pair<distance_type,element*> 
        neigh(std::numeric_limits<distance_type>::max(),
              static_cast<element*>(0));
      searchNearest(root_,key,neigh);
      if (notNull(neigh.second)) {
        elem=*(neigh.second);
        dist=  neigh.first;
        return true; // one neighbor is always found
      }
    }

    return false;
  }

  /*
   * search for the nearest element in the tree to the given key point.
   *
   * If found, the contents of the element will by copied in the 
   * elem parameters and true will be returned.   Otherwise false will
   * be returned.
   * @param key point in the n-dimensional space you are looking for.
   * @param elem the contents of the found point will be left here.
   * @return true if key was found, otherwise false (i.e. tree empty).
   */
  template<typename T,typename D,class U>
  bool kdTree<T,D,U>::searchNearest(const T& key,D& data) const {

    if (notNull(root_)) {
      // the fastest contents copy between the two matrices is required here!
      // without resize (to avoid new memory allocation).
      // use pointer arithmetic.
      // is is equivalent to bounds_.fill(totalBounds_), but avoiding
      // many (here) unnecessary boundary checks.
      const value_type *pt,*pe;
      value_type *p;
      for (pt=totalBounds_.data(),
             pe=&totalBounds_.at(1,maxBounds_.lastIdx()),
             p=minBounds_.data();
           pt<=pe;++pt,++p) {
        (*p)=(*pt);
      }

      // clear old data
      std::pair<distance_type,element*> 
        neigh(std::numeric_limits<distance_type>::max(),
              static_cast<element*>(0));
      searchNearest(root_,key,neigh);
      if (notNull(neigh.second)) {
        data=(neigh.second)->data;
        return true; // one neighbor is always found
      }
    }

    return false;
  }

  /*
   * search for the nearest k elements in the tree to the given key point.
   * @param k number of elements you are looking for
   * @param key the point in the n-dimensional space you are looking for
   * @param neighbors contains the pointers to the found elements.  You
   *                  should NEVER delete these elements.
   * @return true if k elements were found, or false, if the tree contains
   *             less than k elements
   */
  template<typename T,typename D,class U>
  bool kdTree<T,D,U>::searchNearest(const int k,
                                  const T& key,
                                  std::list<element*>& neighbors) const {

    // clear all data
    neighbors.clear();

    if (notNull(root_) && (k<=numElements_)) {
      matrix<value_type> bounds(totalBounds_);

      // clear old data
      mmap_type neigh;
      int nsize(0);

      while (nsize<k) {
        neigh.insert(std::make_pair(std::numeric_limits<distance_type>::max(),
                                    static_cast<element*>(0)));
        ++nsize;
      }

      searchNearest(root_,k,key,bounds,neigh);
      typename mmap_type::iterator it;
      for (it=neigh.begin();it!=neigh.end();++it) {
        if (notNull((*it).second)) {
          neighbors.push_back((*it).second);
        } else {
          return false;
        }
      }
      return true; // if k neighbors found, then everything is ok!
  
    }

    return false;
  }

  /*
   * search for the nearest k elements in the tree to the given key point.
   * @param k number of elements you are looking for
   * @param key the point in the n-dimensional space you are looking for
   * @param neighbors contains the pointers to the found elements.  You
   *                  should NEVER delete these elements.
   * @return true if k elements were found, or false, if the tree contains
   *             less than k elements
   */
  template<typename T,typename D,class U>
  bool 
  kdTree<T,D,U>::searchNearest(const int k,
                          const T& key,
                          mmap_type& neigh) const {

    // clear old data
    neigh.clear();

    if (notNull(root_) && (k<=numElements_)) {
      matrix<value_type> bounds(totalBounds_);

      int nsize(0);
      while (nsize<k) {
        neigh.insert(std::make_pair(std::numeric_limits<distance_type>::max(),
                                    static_cast<element*>(0)));
        ++nsize;
      }

      return searchNearest(root_,k,key,bounds,neigh);
    }

    return false;
  }

 
  /*
   * Search for elements in dist neighbourhood of the given key poin
   */
  template <typename T, typename D, class U>
  bool kdTree<T,D,U>::searchWithin(const T& key,
                                   const distance_type& dist,
                                   std::list<element*>& elems) const {
    elems.clear();
    if (notNull(root_)){
      matrix<value_type> bounds(totalBounds_);
      return searchWithin(root_, key, dist, bounds, elems);
    }
    return false;

  }
 
  /*
   * Search for elements in dist neighbourhood of the given key poin
   */
  template <typename T, typename D, class U>
  bool 
  kdTree<T,D,U>::searchWithin(const T& key,
                      const distance_type& dist,
                      mmap_type& neighbors) const {
    neighbors.clear();
    if (notNull(root_)){
      matrix<value_type> bounds(totalBounds_);
      return searchWithin(root_, key, dist, bounds, neighbors);
    }
    return false;
  }
  
  /*
   * search for the nearest k elements in the tree to the given key point.
   * @param k number of elements you are looking for
   * @param key the point in the n-dimensional space you are looking for
   * @param neighbors contains the pointers to the found elements.  You
   *                  should NEVER delete these elements.
   * @return true if k elements were found, or false, if the tree contains
   *             less than k elements
   */
  template<typename T,typename D,class U>
  bool 
  kdTree<T,D,U>::searchNearest(const node* nptr,
                      const int k,
                      const T& key,
                      matrix<value_type>& bounds,
                      mmap_type& neighbors) const {
    
    // if this is a leaf, elements should be inserted in neighbors...
    if (nptr->isLeaf()) {
      // examine records in bucket, and update the neighbors map if necessary
      typename node::points_type::const_iterator it,eit;
      distance_type d;

      _lti_debug3("In leave with size: " << nptr->points.size() << "\n");
      _lti_debug4("highest distance in neighbors: " << (neighbors.rbegin())->first << "\n");
      _lti_debug4("key: " << key << "\n");

      for (it=nptr->points.begin(),eit=nptr->points.end();it!=eit;++it) {
        // if neighbors have too few elements or if the farest
        // nearest element have a greater distance than the actual
        // element then insert the elements in the map
        _lti_debug4("current point: " << (*it)->point << "\n");
        _lti_debug4("distance: " << distantor_(key,(*it)->point) << "\n");
        if ((neighbors.rbegin())->first > 
            (d=distantor_(key,(*it)->point))) {
          
          // delete the last one
          typename mmap_type::iterator mit;
          mit = neighbors.end();
          --mit;
#         if defined(_LTI_DEBUG) && (_LTI_DEBUG > 3)
          if (notNull((*mit).second)) {
            _lti_debug3("Deleting " << ((*mit).second)->point << std::endl);
          } else {
            _lti_debug3("Deleting dummy entry" << std::endl);
          }
#         endif
          neighbors.erase(mit);

          _lti_debug3("Inserting " << (*it)->point << std::endl);
          // insert one element
          neighbors.insert(std::make_pair(d,(*it)));
        }
      }

      // check "hypersphere within bounds"
      return (checkHypersphereWithinBounds(key,
                                           bounds,
                                           (neighbors.rbegin())->first));
    } // end if leaf

    value_type tmp;
    const int& d=nptr->splitDim;
    const value_type& p = (nptr->partition);
    value_type& boxMin = bounds.at(0,d);
    value_type& boxMax = bounds.at(1,d);

    // ------------
    //  left child
    // ------------

    if (key[d] <= p) {
      // recursive call on closer son
      tmp=boxMax;
      boxMax=p;
      _lti_debug3("Searching closer son left" << std::endl);
      if (searchNearest(nptr->left,k,key,bounds,neighbors)) {
        _lti_debug3("done." << std::endl);
        boxMax=tmp;
        return true;
      } 
      boxMax=tmp;

      // recursive call on farther son, if necessary
      tmp=boxMin;
      boxMin=p;
      if (checkBoundsOverlapHypersphere(key,
                                        bounds,
                                        (neighbors.rbegin())->first)) {
        _lti_debug3("Searching farther son right" << std::endl);
        if (searchNearest(nptr->right,k,key,bounds,neighbors)) {
          boxMin=tmp;
          _lti_debug3("done." << std::endl);
          return true;
        }
      }
      boxMin=tmp;

    } else {

      // ------------
      //  right child
      // ------------
      
      // recursive call on closer son
      tmp=boxMin;
      boxMin=p;
      _lti_debug3("Searching closer son right" << std::endl);
      if (searchNearest(nptr->right,k,key,bounds,neighbors)) {
        boxMin=tmp;
        _lti_debug3("done." << std::endl);
        return true;
      } 

      boxMin=tmp;

      // recursive call on farther son, if necessary
      tmp=boxMax;
      boxMax=p;
      if (checkBoundsOverlapHypersphere(key,
                                        bounds,
                                        (neighbors.rbegin())->first)) {

        _lti_debug3("Searching farther son left" << std::endl);
        if (searchNearest(nptr->left,k,key,bounds,neighbors)) {
          boxMax=tmp;
          _lti_debug3("done." << std::endl);
          return true;
        }
      }
      boxMax=tmp;       
    }
    
    _lti_debug3("Return or terminate" << std::endl);
    // see if we should return or terminate
    return (checkHypersphereWithinBounds(key,
                                         bounds,
                                         (neighbors.rbegin())->first));
  }
  

  /*
   * search for the nearest element in the tree to the given key point.
   * @param key the point in the n-dimensional space you are looking for
   * @param neighbors contains the pointers to the found elements.  You
   *                  should NEVER delete these elements.
   * @return true if k elements were found, or false, if the tree contains
   *             less than k elements
   */
  template<typename T,typename D,class U>
  bool 
  kdTree<T,D,U>::searchNearest(const node* nptr,
                             const T& key,
                             std::pair<distance_type,element*>& neighbors) const {

    // if this is a leaf, elements should be inserted in neighbors...
    if (nptr->isLeaf()) {
      // examine records in bucket, and update the neighbors map if necessary
      typename node::points_type::const_iterator it,eit;
      distance_type d;

      for (it=nptr->points.begin(),eit=nptr->points.end();it!=eit;++it) {
        // if neighbors have too few elements or if the farest
        // nearest element have a greater distance than the actual
        // element then insert the elements in the map
        if (neighbors.first > (d=distantor_(key,(*it)->point))) {
          
          _lti_debug3("Updating " << (*it)->point << std::endl);
          // insert one element
          neighbors.first=d;
          neighbors.second=(*it);
        }
      }

      // check "hypersphere within bounds"
      return (checkHypersphereWithinBounds(key,
                                           bounds_,
                                           neighbors.first));
    } // end if leaf

    // bounds_ is a matrix containing both vectors maxBounds_ and minBounds_
    value_type tmp;
    const int& d = nptr->splitDim;
    const value_type& p = nptr->partition;

    // save some time avoiding too many accesses to maxBounds_/minBounds_
    value_type& boxMax = maxBounds_.at(d);
    value_type& boxMin = minBounds_.at(d);

    // ------------
    //  left child
    // ------------

    if (key[d] <= p) {
      // recursive call on closer son
      tmp=boxMax;
      boxMax=p;
      _lti_debug3("Searching closer son left" << std::endl);
      if (searchNearest(nptr->left,key,neighbors)) {
        _lti_debug3("done." << std::endl);
        boxMax=tmp;
        return true;
      } 
      boxMax=tmp;

      // recursive call on farther son, if necessary
      tmp=boxMin;
      boxMin=p;
      if (checkBoundsOverlapHypersphere(key,
                                        bounds_,
                                        neighbors.first)) {
        _lti_debug3("Searching farther son right" << std::endl);
        if (searchNearest(nptr->right,key,neighbors)) {
          boxMin=tmp;
          _lti_debug3("done." << std::endl);
          return true;
        }
      }
      boxMin=tmp;

    } 

    // ------------
    //  right child
    // ------------

    if (key[d] >= p) {

      // recursive call on closer son
      tmp=boxMin;
      boxMin=p;
      _lti_debug3("Searching closer son right" << std::endl);
      if (searchNearest(nptr->right,key,neighbors)) {
        boxMin=tmp;
        _lti_debug3("done." << std::endl);
        return true;
      } 
      boxMin=tmp;

      // recursive call on farther son, if necessary
      tmp=boxMax;
      boxMax=p;
      if (checkBoundsOverlapHypersphere(key,
                                        bounds_,
                                        neighbors.first)) {

        _lti_debug3("Searching farther son left" << std::endl);
        if (searchNearest(nptr->left,key,neighbors)) {
          boxMax=tmp;
          _lti_debug3("done." << std::endl);
          return true;
        }
      }
      boxMax=tmp;
    }
    
    _lti_debug3("Return or terminate" << std::endl);
    // see if we should return or terminate
    return (checkHypersphereWithinBounds(key,
                                         bounds_,
                                         neighbors.first));
  }


  /*
   * search for the nearest k elements in the tree to the given key point.
   * @param k number of elements you are looking for
   * @param key the point in the n-dimensional space you are looking for
   * @param neighbors contains the pointers to the found elements.  You
   *                  should NEVER delete these elements.
   * @return true if k elements were found, or false, if the tree contains
   *             less than k elements
   */
  template<typename T,typename D, class U>
  bool kdTree<T,D,U>::searchWithin(const node* nptr,
                                   const T& key,
                                   const distance_type& dist,
                                   matrix<value_type>& bounds,
                                   std::list<element*>& elems) const {
    
    // if this is a leaf, elements should be inserted in neighbors...
    if (nptr->isLeaf()) {
     
      // examine records in bucket, and update the elements list if necessary
      typename node::points_type::const_iterator it,eit;

      for (it=nptr->points.begin(),eit=nptr->points.end();it!=eit;++it) {
        // if point is within dist add to list of elements
        if (dist >= distantor_(key,(*it)->point)) {
          elems.push_back(*it);
        }
      }
      // check "hypersphere within bounds"
      return (checkHypersphereWithinBounds(key, bounds,dist)); 
    } // end if leaf


    value_type tmp;
    const int& d = nptr->splitDim;
    const value_type& p = nptr->partition;
    value_type& boxMin = bounds.at(0,d);
    value_type& boxMax = bounds.at(1,d);

    // ------------
    //  left child
    // ------------

    if (key[d] <= p) {
      // recursive call on closer son
      tmp=boxMax;
      boxMax=p;
      _lti_debug("Searching closer son left" << std::endl);
      if (searchWithin(nptr->left,key,dist,bounds,elems)) {
        _lti_debug("done." << std::endl);
        boxMax=tmp;
        return true;
      } 
      boxMax=tmp;

      // recursive call on farther son, if necessary
      tmp=boxMin;
      boxMin=p;
      if (checkBoundsOverlapHypersphere(key,bounds,dist)) {
        _lti_debug("Searching farther son right" << std::endl);
        if (searchWithin(nptr->right,key,dist,bounds,elems)) {
          boxMin=tmp;
          _lti_debug("done." << std::endl);
          return true;
        }
      }
      boxMin=tmp;

    } else {

      // ------------
      //  right child
      // ------------
      
      // recursive call on closer son
      tmp=boxMin;
      boxMin=p;
      _lti_debug("Searching closer son right" << std::endl);
      if (searchWithin(nptr->right,key,dist,bounds,elems)) {
        boxMin=tmp;
        _lti_debug("done." << std::endl);
        return true;
      } 

      boxMin=tmp;

      // recursive call on farther son, if necessary
      tmp=boxMax;
      boxMax=p;
      if (checkBoundsOverlapHypersphere(key, bounds,dist)) {

        _lti_debug("Searching farther son left" << std::endl);
        if (searchWithin(nptr->left,key,dist,bounds,elems)) {
          boxMax=tmp;
          _lti_debug("done." << std::endl);
          return true;
        }
      }
      boxMax=tmp;       
    }
    
    _lti_debug("Return or terminate" << std::endl);
    // see if we should return or terminate
    return (checkHypersphereWithinBounds(key,bounds,dist));
  }

  /*
   * search for the nearest k elements in the tree to the given key point.
   * @param k number of elements you are looking for
   * @param key the point in the n-dimensional space you are looking for
   * @param neighbors contains the pointers to the found elements.  You
   *                  should NEVER delete these elements.
   * @return true if k elements were found, or false, if the tree contains
   *             less than k elements
   */
  template<typename T,typename D, class U>
  bool kdTree<T,D,U>::searchWithin(const node* nptr,
                                   const T& key,
                                   const distance_type& dist,
                                   matrix<value_type>& bounds,
                               mmap_type& neigh) const {
    
    // if this is a leaf, elements should be inserted in neighbors...
    if (nptr->isLeaf()) {
     
      // examine records in bucket, and update the elements list if necessary
      typename node::points_type::const_iterator it,eit;
      distance_type d;

      for (it=nptr->points.begin(),eit=nptr->points.end();it!=eit;++it) {
        // if point is within dist add to list of elements
        if (dist >= (d=distantor_(key,(*it)->point))) {
          neigh.insert(std::make_pair(d,(*it)));
        }
      }
      // check "hypersphere within bounds"
      return (checkHypersphereWithinBounds(key, bounds,dist)); 
    } // end if leaf


    value_type tmp;
    const int& d = nptr->splitDim;
    const value_type& p = nptr->partition;
    value_type& boxMin = bounds.at(0,d);
    value_type& boxMax = bounds.at(1,d);

    // ------------
    //  left child
    // ------------

    if (key[d] <= p) {
      // recursive call on closer son
      tmp=boxMax;
      boxMax=p;
      _lti_debug("Searching closer son left" << std::endl);
      if (searchWithin(nptr->left,key,dist,bounds,neigh)) {
        _lti_debug("done." << std::endl);
        boxMax=tmp;
        return true;
      } 
      boxMax=tmp;

      // recursive call on farther son, if necessary
      tmp=boxMin;
      boxMin=p;
      if (checkBoundsOverlapHypersphere(key,bounds,dist)) {
        _lti_debug("Searching farther son right" << std::endl);
        if (searchWithin(nptr->right,key,dist,bounds,neigh)) {
          boxMin=tmp;
          _lti_debug("done." << std::endl);
          return true;
        }
      }
      boxMin=tmp;

    } else {

      // ------------
      //  right child
      // ------------
      
      // recursive call on closer son
      tmp=boxMin;
      boxMin=p;
      _lti_debug("Searching closer son right" << std::endl);
      if (searchWithin(nptr->right,key,dist,bounds,neigh)) {
        boxMin=tmp;
        _lti_debug("done." << std::endl);
        return true;
      } 

      boxMin=tmp;

      // recursive call on farther son, if necessary
      tmp=boxMax;
      boxMax=p;
      if (checkBoundsOverlapHypersphere(key, bounds,dist)) {

        _lti_debug("Searching farther son left" << std::endl);
        if (searchWithin(nptr->left,key,dist,bounds,neigh)) {
          boxMax=tmp;
          _lti_debug("done." << std::endl);
          return true;
        }
      }
      boxMax=tmp;       
    }
    
    _lti_debug("Return or terminate" << std::endl);
    // see if we should return or terminate
    return (checkHypersphereWithinBounds(key,bounds,dist));
  }



  /*
   * search the best-bin-first algorithm of Beis and Lowe.  It search
   * for the approximate k nearest neighbors.
   * @param k number of elements you are looking for.
   * @param key the point in the n-dimensional space you are looking for.
   * @param neighbors contains the pointers to the found elements.  You
   *                  should NEVER delete these elements.
   * @param emax is the maximal number of visits allowed for leaf nodes.
   *             (in the original paper is called \f$E_{max}\f$).
   * @return true if k elements were found, or false, if the tree contains
   *             less than k elements
   */
  template<typename T,typename D,class U>
  bool kdTree<T,D,U>::searchBestBinFirst(const int k,
                                       const T& key,
                                       const int emax,
                                       std::list<element*>& neighbors) const {


    // clear all data
    neighbors.clear();

    if (notNull(root_) && (numElements_>=k)) {

      // clear old data
      mmap_type neigh;
      int nsize(0);

      while (nsize<k) {
        neigh.insert(std::make_pair(std::numeric_limits<distance_type>::max(),
                                    static_cast<element*>(0)));
        ++nsize;
      }

      if (searchBestBinFirst(root_,k,key,neigh,emax)) {
        typename mmap_type::iterator it;
        for (it=neigh.begin();it!=neigh.end();++it) {
          neighbors.push_back((*it).second);
        }
        return true; // if k neighbors found, then everything is ok!
      }
    }

    return false;
  }

  /*
   * search the best-bin-first algorithm of Beis and Lowe.  It search
   * for the approximate k nearest neighbors.
   * @param k number of elements you are looking for.
   * @param key the point in the n-dimensional space you are looking for.
   * @param neighbors contains the pointers to the found elements.  You
   *                  should NEVER delete these elements.
   * @param emax is the maximal number of visits allowed for leaf nodes.
   *             (in the original paper is called \f$E_{max}\f$).
   * @return true if k elements were found, or false, if the tree contains
   *             less than k elements
   */
  template<typename T,typename D,class U>
  bool
  kdTree<T,D,U>::searchBestBinFirst(const int k,
                                  const T& key,
                                  const int emax,
                                  mmap_type& neighbors
                                  ) const {
    
    // clear all data
    neighbors.clear();

    if (notNull(root_) && (numElements_>=k)) {
      // clear old data
      for (int i=0;i<k;++i) {
        neighbors.insert(std::make_pair(std::numeric_limits<distance_type>::max(),
                                        static_cast<element*>(0)));
      }

      return searchBestBinFirst(root_,k,key,neighbors,emax);
    }

    return false;
  }

  /*
   * search the best-bin-first algorithm of Beis and Lowe.  It search
   * for the approximate k nearest neighbors.
   * @param k number of elements you are looking for.
   * @param key the point in the n-dimensional space you are looking for.
   * @param neighbors contains the pointers to the found elements.  You
   *                  should NEVER delete these elements.
   * @return true if k elements were found, or false, if the tree contains
   *         less than k elements
   */
  template<typename T,typename D,class U>
  bool 
  kdTree<T,D,U>::searchBestBinFirst(const node* root,
                                  const int k,
                                  const T& key,
                                  mmap_type& neighbors,
                                  const int emax
                                  ) const {

    // priority queue
    typedef
      std::multimap<distance_type,std::pair< node*,matrix<value_type> > > qtype;
    qtype pqueue;
    typename qtype::iterator qit;

    matrix<value_type> bounds(totalBounds_);

    const int maxVisits = min(emax,numLeaves_);// ensure termination
    int nodeVisits(0);                        // number of leaf nodes visited
    int elems(0);                             // number of elements inserted in
                                              // neighbors
    const node* nptr = root;

    while ( notNull(nptr) && ((elems<k) || (nodeVisits<maxVisits)) ) {
      // if this is a leaf, elements should be inserted in neighbors...
      if (nptr->isLeaf()) {
        // examine records in bucket, and update the neighbors map if necessary
        typename node::points_type::const_iterator it,eit;
        distance_type d;
        
        for (it=nptr->points.begin(),eit=nptr->points.end();it!=eit;++it) {
          // if neighbors have too few elements or if the farest
          // nearest element have a greater distance than the actual
          // element then insert the elements in the map
          if ( (neighbors.rbegin())->first > 
               (d=distantor_(key,(*it)->point)) ) {
            
            // delete the last one
            typename mmap_type::iterator mit;
            mit = neighbors.end();
            --mit;
#           if defined(_LTI_DEBUG) && (_LTI_DEBUG > 3)
            if (notNull((*mit).second)) {
              _lti_debug3("Deleting " << ((*mit).second)->point << std::endl);
            } else {
              _lti_debug3("Deleting dummy entry" << std::endl);
            }
#           endif
            neighbors.erase(mit);
            
            _lti_debug3("Inserting " << (*it)->point << std::endl);
            // insert one element
            neighbors.insert(std::make_pair(d,(*it)));
            ++elems;
          }
        }
        
        nodeVisits++;

        // check "hypersphere within bounds"
        if (checkHypersphereWithinBounds(key,
                                         bounds,
                                         (neighbors.rbegin())->first)) {
          return true;
        }

        // get the next node to be analyzed from the priority queue
        if (pqueue.empty()) {
          nptr=0;
        } else {
          qit = pqueue.begin();
          (*qit).second.second.detach(bounds);
          nptr = (*qit).second.first;
          pqueue.erase(qit);
        }

      } // end if leaf
      else {
        value_type tmp;
        const int& d = (nptr->splitDim);
        const value_type& p = (nptr->partition);
        value_type& boxMin = bounds.at(0,d);
        value_type& boxMax = bounds.at(1,d);
        
        // ------------
        //  left child
        // ------------
        
        if (key[d] <= p) {
          // insert in the priority queue the distance to the
          // farest node
          tmp=boxMin;
          boxMin=p;
          pqueue.insert(std::make_pair(minDistancePointToBox(key,bounds),
                                       std::make_pair(nptr->right,bounds)));
          
          boxMin=tmp;
          
          // call on closer son
          boxMax=p;
          nptr = nptr->left;
        } else {
          
          // ------------
          //  right child
          // ------------
          
          // insert in the priority queue the distance to the
          // farest node
          tmp=boxMax;
          boxMax=p;
          pqueue.insert(std::make_pair(minDistancePointToBox(key,bounds),
                                       std::make_pair(nptr->left,bounds)));
          
          boxMax=tmp;
          
          // call on closer son
          boxMin=p;
          nptr = nptr->right;
        }
      }
        
    } // end of while
    
    return true;
  }

  // ----------------------------------------
  // Search range
  // ----------------------------------------
  


  /*
   * Search for all points lying within the given hyperbox.
   * @param nptr pointer to the node root of the subtree to be evaluated
   * @param bbox matrix specifying the hyperbox.  Its first row specify the
   *             minimum values at each dimension of the box, and the second
   *             row the maximum values.  The number of columns of the matrix
   *             must be equal as the number of dimensions of the points 
   *             stored in the kd-Tree.
   * @param neighbors list of pointer to the elements found until now.
   * @return true if any element was found.
   */
  template<typename T,typename D,class U>
  bool kdTree<T,D,U>::searchRange(const T& boxMin,
                                  const T& boxMax,
                                  std::list<element*>& neighbors) const {

    _lti_debug3("Search In Range: from " << boxMin << 
                " to " << boxMax << std::endl);

    neighbors.clear();

    // if this is a leaf, elements should be inserted in neighbors...
    if (notNull(root_)) {
      // the fastest contents copy between the two matrices is required here!
      // without resize (to avoid new memory allocation).
      // use pointer arithmetic.
      // is is equivalent to bounds_.fill(totalBounds_), but avoiding
      // many (here) unnecessary boundary checks.
      const value_type *pt,*pe;
      value_type *p;
      for (pt=totalBounds_.data(),pe=&totalBounds_.at(1,maxBounds_.lastIdx()),
             p=minBounds_.data();pt<=pe;++pt,++p) {
        (*p)=(*pt);
      }

      searchRange(root_,boxMin,boxMax,neighbors);
    }

    return !neighbors.empty();
  }

  template<typename T,typename D,class U>
  bool kdTree<T,D,U>::searchRange(const node* nptr,
                                  const T& boxMin,
                                  const T& boxMax,
                                  std::list<element*>& neighbors) const {
    
    if (nptr->isLeaf()) {
      typename node::points_type::const_iterator it,eit;

      _lti_debug3("Leaf node bounds:\n" << bounds_ << endl);

      // examine records in bucket, and update the neighbors map if necessary
      for (it=nptr->points.begin(),eit=nptr->points.end();it!=eit;++it) {
        if (withinBox(boxMin,boxMax,(*it)->point)) {
          _lti_debug3("Inserting (once) " << (*it)->point << endl);
          neighbors.push_back(*it);
        }
      }
      return withinBox(bounds_,boxMin,boxMax);  // false means, still need to
                                                  // search somewhere else
      
    } // end is leaf.
    
    // bounds_ is a matrix containing both vectors maxBounds_ and minBounds_
    value_type tmp;
    const int& d = nptr->splitDim;
    const value_type& p = nptr->partition;

    // save some time avoiding too many accesses to maxBounds_/minBounds_
    value_type& nodeBoxMax = maxBounds_.at(d);
    value_type& nodeBoxMin = minBounds_.at(d);

    // ------------
    //  left child
    // ------------
  
    if (boxMin[d] <= p) {
      tmp=nodeBoxMax;
      nodeBoxMax=p;
      _lti_debug3("Searching left child" << std::endl);
      if (searchRange(nptr->left,boxMin,boxMax,neighbors)) {
        _lti_debug3("done." << std::endl);
        nodeBoxMax=tmp;
        return true;
      }
      nodeBoxMax=tmp;
    }

    // ------------
    //  right child
    // ------------

    if (boxMax[d] >= p) {
      tmp=nodeBoxMin;
      nodeBoxMin=p;
      _lti_debug3("Searching right child" << std::endl);
      if (searchRange(nptr->right,boxMin,boxMax,neighbors)) {
        _lti_debug3("done." << std::endl);
        nodeBoxMin=tmp;
        return true;        
      }
      nodeBoxMin=tmp;
    }

    return withinBox(bounds_,boxMin,boxMax);
  }

}
