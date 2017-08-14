/*
 * Copyright (C) 2002-2008
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
 * \file   ltiMinimumFilter_template.h
 *         Contains the class lti::minimumFilter, 
 *         which computes the minimum value within a rectangular window.
 * 
 * \author Pablo Alvarado
 * \date   08.01.2002
 *
 * revisions ..: $Id: ltiMinimumFilter_template.h,v 1.4 2010-04-05 03:39:46 alvarado Exp $
 */

namespace lti {

  // --------------------------------------------------
  // minimumFilter<T>::parameters
  // --------------------------------------------------

  /*
   * Constructor that initializes the kernel mask to a size of k x k,
   * centered at 0,0
   */
  template<typename T>
  minimumFilter<T>::parameters::parameters(const int k) :
    functor::parameters() {
    boundaryType = Mirror;
    initSquare(k);
  }

  /*
   * Copy constructor
   */
  template<typename T>
  minimumFilter<T>::parameters::parameters(const parameters& other) 
    : functor::parameters() {
    copy(other);
  }

  /*
   * Destructor
   */
  template<typename T>
  minimumFilter<T>::parameters::~parameters() {
  }

  /*
   * returns name of this type
   */
  template<typename T>
  const std::string& minimumFilter<T>::parameters::name() const {
    _LTI_RETURN_CLASS_NAME;
  }
  
  /*
   * copy the contents of a parameters object
   * @param other the parameters object to be copied
   * @return a reference to this parameters object
   */
  template<typename T>
  typename minimumFilter<T>::parameters& 
  minimumFilter<T>::parameters::copy(const parameters& other)  {
    functor::parameters::copy(other);

    boundaryType = other.boundaryType;
    maskWindow = other.maskWindow;

    return *this;
  }

  /*
   * returns a pointer to a clone of the parameters
   */
  template<typename T>
  typename minimumFilter<T>::parameters*
  minimumFilter<T>::parameters::clone() const  {
    return new parameters(*this);
  }

  template<typename T>
  typename minimumFilter<T>::parameters* 
  minimumFilter<T>::parameters::newInstance() const {
    return new parameters;
  }

  /*
   * Write the parameters in the given ioHandler
   */
  template<typename T>
  bool minimumFilter<T>::parameters::write(ioHandler& handler,
                                           const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }
    
    if (b) {
      lti::write(handler,"boundaryType",boundaryType);
      lti::write(handler,"maskWindow",maskWindow);
    }

    b = b && functor::parameters::write(handler,false);
    
    if (complete) {
      b = b && handler.writeEnd();
    }
    
    return b;
  }
  

  /*
   * read the parameters from the given ioHandler
   */
  template<typename T>
  bool minimumFilter<T>::parameters::read(ioHandler& handler,
                                          const bool complete) {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }
    
    if (b) {
      lti::read(handler,"boundaryType",boundaryType);
      lti::read(handler,"maskWindow",maskWindow);
    }
    
    b = b && functor::parameters::read(handler,false);

    if (complete) {
      b = b && handler.readEnd();
    }
    
    return b;
  }
  


  /*
   * Initialize the parameters for a symmetric square kernel sizeXsize.
   * If the size is even, it will be "fixed" to the next odd number
   */
  template<typename T>
  void minimumFilter<T>::parameters::initSquare(const int size) {
    int realSize(size);
    if ((size % 2) == 0) {
      realSize++;
    }

    realSize/=2;
    maskWindow.ul.x = -realSize;
    maskWindow.ul.y = -realSize;
    maskWindow.br.x = +realSize;
    maskWindow.br.y = +realSize;
  }



  // --------------------------------------------------
  // minimumFilter<T>
  // --------------------------------------------------

  // default constructor
  template<class T>
  minimumFilter<T>::minimumFilter()
    : functor(),rawBuffer_(0),buffer_(0) {

    parameters tmp;
    setParameters(tmp); // default parameters

  }

  // default constructor
  template<class T>
  minimumFilter<T>::minimumFilter(const parameters& par)
    : functor(),rawBuffer_(0),buffer_(0) {

    setParameters(par); // default parameters
  }

  // default constructor
  template<class T>
  minimumFilter<T>::minimumFilter(const int k)
    : functor(),rawBuffer_(0),buffer_(0) {

    parameters tmp(k);
    setParameters(tmp); // default parameters
  }

  // copy constructor
  template<class T>
  minimumFilter<T>::minimumFilter(const minimumFilter<T>& o)
    : functor(),rawBuffer_(0),buffer_(0) {

    copy(o);
  }

  // destructor
  template<class T>
  minimumFilter<T>::~minimumFilter() {
    delete[] rawBuffer_;
    buffer_ = rawBuffer_ = 0;
  }

  // returns the name of this type
  template<class T>
  const std::string& minimumFilter<T>::name() const {
    _LTI_RETURN_CLASS_NAME;
  }

  // copy member
  template<class T>
  minimumFilter<T>&
  minimumFilter<T>::copy(const minimumFilter<T>& other) {
    functor::copy(other);
    return (*this);
  }

  // clone member
  template<class T>
  minimumFilter<T>* minimumFilter<T>::clone() const {
    return new minimumFilter<T>(*this);
  }

  template<class T>
  minimumFilter<T>* minimumFilter<T>::newInstance() const {
    return new minimumFilter<T>();
  }

  // return parameters
  template<class T>
  const typename minimumFilter<T>::parameters&
  minimumFilter<T>::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }

  template<class T>
  typename minimumFilter<T>::parameters&
  minimumFilter<T>::getRWParameters() {
    parameters* par = dynamic_cast<parameters*>(&functor::getRWParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }

  // set parameters
  template<class T>
  bool minimumFilter<T>::updateParameters() {
    const parameters& mfpar = getParameters();

    // two extra elements added: one at the beginning of buffer and one
    // at the end...
    delete[] rawBuffer_;
    rawBuffer_ = new T[max(mfpar.maskWindow.getDimensions().y,
                           mfpar.maskWindow.getDimensions().x)+2];
    
    buffer_ = &rawBuffer_[1];

    return true;
  }

  // set new parameters with the given kernel size
  template<class T>
  bool minimumFilter<T>::setMaskWindow(const irectangle& wsize) {
    // we need to use clone here, because this method can be called
    // from an inherited class
    
    parameters* par = 
      dynamic_cast<typename minimumFilter<T>::parameters*>
      (getParameters().clone());
    
    if (isNull(par)) {
      return false;
    }
    par->maskWindow = wsize;                    // change just the size
    setParameters(*par);
    delete par;                                // delete the temp. object
    return true;
  }

  // set new parameters with the given kernel size
  template<class T>
  bool minimumFilter<T>::setSquareMaskWindow(const int wsize) {
    getRWParameters().initSquare(wsize);
    return updateParameters();
  }

  template<class T>
  inline void minimumFilter<T>::insert(const int size,const T elem) {
    // assume the buffer is already sorted, so we need just to insert
    // the element

    // small trick to avoid compairing the index too...
    // it assumes that rawBuffer is 1 element before buffer
    rawBuffer_[0] = elem;

    int i=size-2;

    while (elem<buffer_[i]) {
      buffer_[i+1] = buffer_[i];
      --i;
    }
    buffer_[i+1] = elem;

  }

  template<class T>
  inline void minimumFilter<T>::insdel(const int size,
                                       const T insElem,
                                       const T delElem) {
    int i;

    if (insElem == delElem) {
      return;
    } else if (insElem < delElem) {
      rawBuffer_[0] = insElem;

      // find the position of the delElem
      i=size-1;
      while (delElem <= buffer_[i]) {
        --i;
      }
      // now i points to one element before the element to be deleted

      // shift the elements until there is place for the new insElem
      while (insElem < buffer_[i]) {
        buffer_[i+1] = buffer_[i];
        --i;
      }
      buffer_[i+1] = insElem;
    } else {
      // flag to ensure end of following loop (memory already reserved)
      buffer_[size] = insElem;

      // find the position of the delElem
      i=0;
      while (delElem >= buffer_[i]) {
        ++i;
      }
      // now i points to one element after the element to be deleted


      // shift the elements until there is place for the new insElem
      while (insElem > buffer_[i]) {
        buffer_[i-1] = buffer_[i];
        ++i;
      }
      buffer_[i-1] = insElem;
    }


  }


  // -------------------------------------------------------------------
  // The apply-methods!
  // -------------------------------------------------------------------

  // On place apply for type matrix<T>!
  template<class T>
  bool minimumFilter<T>::apply(matrix<T>& srcdest) {

    matrix<T> tmp;
    tmp.allocate(srcdest.size());

    int y;
    for (y=0;y<srcdest.rows();y++) {
      apply(srcdest.getRow(y),tmp.getRow(y));
    }

    return applyCol(tmp,srcdest);
  }

  // On place apply for type vector<T>!
  template<class T>
  bool minimumFilter<T>::apply(vector<T>& srcdest) {

    vector<T> tmp;

    apply(srcdest,tmp);
    tmp.detach(srcdest);

    return true;
  }

  // On copy apply for type matrix<T>!
  template<class T>
  bool minimumFilter<T>::apply(const matrix<T>& src,
                                     matrix<T>& dest) {
    matrix<T> tmp;
    tmp.allocate(src.size());

    int y;
    for (y=0;y<src.rows();y++) {
      apply(src.getRow(y),tmp.getRow(y));
    }

    dest.allocate(src.size());
    return applyCol(tmp,dest);
  }

  // On copy apply for type vector<T>!
  template<class T>
  bool minimumFilter<T>::apply(const vector<T>& src,
                                     vector<T>& dest) {
    const parameters& param = getParameters();

    // last valid index of the filter vector
    const int filterSize  = param.maskWindow.getDimensions().x;
    const int kernLastIdx = param.maskWindow.br.x;

    // if filter-kernel has dimension 1: just normalize
    if ((filterSize == 1) && (kernLastIdx == 0)) {
      dest.copy(src);
      return true;
    }

    int i;
    int begin,end;
    vector<T> firstResult;
    firstResult.allocate(filterSize);

    // initialize size of dest (if needed)
    if (dest.size() != src.size()) {
      dest.allocate(src.size());
    }

    // ----------------------------------------
    // if the kernel is smaller than the vector
    // ----------------------------------------
    if (src.size() >= filterSize) {

      // middle block filter (the whole kernel can be applied here!)
      begin = (kernLastIdx<0) ? -kernLastIdx : 0;
      end   = (kernLastIdx >= filterSize) ?
        src.size()-kernLastIdx-1 :
        src.size()-filterSize;

      // the used pointers
      const T *xp,*fp,*tp,*xxp,*endxxp,*endp,*endfp;
      T *ip,*endip;

      // This is the standard way to implement the convolution:
      //   for (x=begin,i=kern.lastIdx()+x;x<end;x++,i++) {
      //     // x: begin in src vector
      //     // i: place to write local dilation
      //     // lresult : local result
      //     // xx: sample on image to be checked
      //     for (xx=x,lresult.reset(),f=filterLast;f>=0;f--,xx++ ) {
      //       lresult.accumulate(filter.at(f),src.at(xx));
      //     }
      //     dest.at(i)=lresult.getResult();
      //   }

      endp = src.data();
      endp+=end;
      endip = &dest.at(dest.lastIdx());
      endip++;

      // first result at buffer
      xp=&src.at(begin);              // position at source
      ip=&dest.at(kernLastIdx+begin); // position at destination

      buffer_[0]=*xp;
      for (xxp=xp+1,endxxp=xp+filterSize,i=2;
           xxp!=endxxp;
           ++xxp,++i) {
        insert(i,*xxp);
      }

      // store result for first pixel
      firstResult.fill(buffer_);

      (*ip)=buffer_[0];

      // remember beginning!
      fp = xp;

      // next elements
      ip++;

      // the rest pixels can be calculated using only one new element!

      // The loop with pointer arithmetic (much faster this way!)
      for (;fp<endp;fp++,ip++,xxp++) {
        // x: begin in src vector
        // i: place to write local dilation
        // lresult : local result
        // xx: sample on image to be checked
        insdel(filterSize,*xxp,*fp);
        (*ip)=buffer_[0];
      }

      // -----------------------
      //    border conditions
      // -----------------------

      switch (param.boundaryType) {
          case lti::NoBoundary:
            break;
          case lti::Zero:

            // right border!
            if (kernLastIdx>=-1) {
              for (;ip<endip;fp++,ip++) {
                insdel(filterSize,T(0),*fp);
                (*ip)=buffer_[0];
              }
            } else {
              endfp = src.data();
              endfp+=src.size();
              for (;fp<endfp;fp++,ip++) {
                insdel(filterSize,T(0),*fp);
                (*ip)=buffer_[0];
              }
              for (;ip<endip;ip++) {
                (*ip)=buffer_[0];
              }
            }

            // left border!
            xp=&src.at(begin+filterSize-1); // position at source to the last
                                            // element of the kernel region

            memcpy(buffer_,firstResult.data(),filterSize*sizeof(T));

            endip = dest.data();
            ip=endip+(kernLastIdx+begin-1); // position at destination

            endip--;

            if (filterSize>=kernLastIdx) {
              for (;ip>endip;ip--,xp--) {
                insdel(filterSize,0,*xp);
                (*ip)=buffer_[0];
              }
            } else {
              endxxp = src.data();
              endxxp--;
              for (;xp>endxxp;ip--,xp--) {
                insdel(filterSize,0,*xp);
                (*ip)=buffer_[0];
              }
              for (;ip>endip;ip--) {
                (*ip)=buffer_[0];
              }

            }
            break;

          case lti::Mirror:

            // right border!

            tp = &(src.at(src.lastIdx())); // pointer to the mirrored data

            if (kernLastIdx>=-1) {
              for (;ip<endip;fp++,ip++,tp--) {
                insdel(filterSize,*tp,*fp);
                (*ip)=buffer_[0];
              }
            } else {
              endfp = src.data();
              endfp+=src.size();
              for (;fp<endfp;fp++,ip++,tp--) {
                insdel(filterSize,*tp,*fp);
                (*ip)=buffer_[0];
              }
              fp--;
              for (;ip<endip;fp--,ip++,tp--) {
                insdel(filterSize,*tp,*fp);
                (*ip)=buffer_[0];
              }
            }

            // left border!
            xp=&src.at(begin+filterSize-1); // position at source to the last
            // element of the kernel region

            memcpy(buffer_,firstResult.data(),filterSize*sizeof(T));

            endip = dest.data();
            ip=endip+(kernLastIdx+begin-1);  // position at destination
            endip--;

            tp = src.data();

            if (filterSize >= kernLastIdx) {
              for (;ip>endip;ip--,xp--,tp++) {
                insdel(filterSize,*tp,*xp);
                (*ip)=buffer_[0];
              }
            } else {
              endxxp = src.data();
              endxxp--;
              for (;xp>endxxp;ip--,xp--,tp++) {
                insdel(filterSize,*tp,*xp);
                (*ip)=buffer_[0];
              }
              xp++;
              for (;ip>endip;ip--,xp++,tp++) {
                insdel(filterSize,*tp,*xp);
                (*ip)=buffer_[0];
              }
            }

            break;

          case lti::Periodic:
            // right border!

            tp = src.data(); // pointer to the periodic data

            if (kernLastIdx>=-1) {
              for (;ip<endip;fp++,ip++,tp++) {
                insdel(filterSize,*tp,*fp);
                (*ip)=buffer_[0];
              }
            } else {
              endfp = src.data();
              endfp+=src.size();
              for (;fp<endfp;fp++,ip++,tp++) {
                insdel(filterSize,*tp,*fp);
                (*ip)=buffer_[0];
              }
              fp = src.data();
              for (;ip<endip;fp++,ip++,tp++) {
                insdel(filterSize,*tp,*fp);
                (*ip)=buffer_[0];
              }
            }

            // left border!
            xp=&src.at(begin+filterSize-1); // position at source to the last
            // element of the kernel region

            memcpy(buffer_,firstResult.data(),filterSize*sizeof(T));
            endip = dest.data();
            ip=endip+(kernLastIdx+begin-1); // position at destination
            endip--;

            tp = &(src.at(src.lastIdx()));

            if (filterSize >= kernLastIdx) {
              for (;ip>endip;ip--,xp--,tp--) {
                insdel(filterSize,*tp,*xp);
                (*ip)=buffer_[0];
              }
            } else {
              endxxp = src.data();
              endxxp--;
              for (;xp>endxxp;ip--,xp--,tp--) {
                insdel(filterSize,*tp,*xp);
                (*ip)=buffer_[0];
              }
              xp = &src.at(src.lastIdx());
              for (;ip>endip;ip--,xp--,tp--) {
                insdel(filterSize,*tp,*xp);
                (*ip)=buffer_[0];
              }
            }

            break;
          case lti::Constant:
            T y;

            // right border!
            y = src.at(src.lastIdx());

            if (kernLastIdx>=-1) {
              for (;ip<endip;fp++,ip++) {
                insdel(filterSize,y,*fp);
                (*ip)=buffer_[0];
              }
            } else {
              endfp = src.data();
              endfp+=src.size();
              for (;fp<endfp;fp++,ip++) {
                insdel(filterSize,y,*fp);
                (*ip)=buffer_[0];
              }
              for (;ip<endip;ip++) {
                (*ip)=buffer_[0];
              }
            }

            // left border!
            y = src.at(0);

            xp=&src.at(begin+filterSize-1); // position at source to the last
                                            // element of the kernel region

            memcpy(buffer_,firstResult.data(),filterSize*sizeof(T));
            endip = dest.data();
            ip=endip+(kernLastIdx+begin-1); // position at destination
            endip--;

            if (filterSize >= kernLastIdx) {
              for (;ip>endip;ip--,xp--) {
                insdel(filterSize,y,*xp);
                (*ip)=buffer_[0];
              }
            } else {
              endxxp = src.data();
              endxxp--;
              for (;xp>endxxp;ip--,xp--) {
                insdel(filterSize,y,*xp);
                (*ip)=buffer_[0];
              }
              for (;ip>endip;ip--,xp--) {
                (*ip)=buffer_[0];
              }
            }

            break;
          default:
            throw exception("unknown border type!");
      }
    } else {
      // ---------------------------------------
      // if the kernel is bigger than the vector
      // ---------------------------------------

      const int& kernFirstIdx = param.maskWindow.ul.x;
      int f,x;

      if ((param.boundaryType == lti::Mirror) ||
          (param.boundaryType == lti::Periodic)) {
        // all elements are equal the minimum!
        dest.fill(src.findMinimum());

      } else if ((param.boundaryType == lti::Zero) ||
                 (param.boundaryType == lti::Constant)) {

        // initialize lresult

        T right = src.at(src.lastIdx());
        T left =  src.at(0);

        if (param.boundaryType == lti::Zero) {
          right = T(0);
          left =  T(0);
        } else {
          right = src.at(src.lastIdx());
          left =  src.at(0);
        }

        // initialize the lresult with the kernel at(-1)
        const int lastInit = -kernFirstIdx;
        const int lastInit2 = min(lastInit,src.size());
        const int lastInit0 = min(0,-kernFirstIdx);

        i=1;

        for (f=-kernLastIdx-1;f<lastInit0;++f) {
          insert(i,left);
          ++i;
        }

        for (;f<lastInit2;++f) {
          insert(i,src.at(f));
          ++i;
        }

        for (;f<lastInit;++f) {
          insert(i,right);
          ++i;
        }

        // the last kernel element
        int l;
        l = lastInit;

        // the first kernel element
        f = (-kernLastIdx-1);

        T ins,del;

        // apply kernel
        for (x=0;x<src.size();++x,++f,++l) {

          if (f<0) {
            del=left;
          } else if (f>=src.size()) {
            del=right;
          } else {
            del=src.at(f);
          }

          if (l<0) {
            ins=left;
          } else if (l>=src.size()) {
            ins=right;
          } else {
            ins=src.at(l);
          }

          insdel(filterSize,ins,del);

          dest.at(x) = buffer_[0];
        }
      }
    }

    return true;
  }

  // On copy apply for type matrix<T>!
  template<class T>
  bool minimumFilter<T>::applyCol(const matrix<T>& src,
                                        matrix<T>& dest) {
    const parameters& param = getParameters();

    // last valid index of the filter vector
    const int filterSize = param.maskWindow.getDimensions().y;
    const int kernLastIdx = param.maskWindow.br.y;

    // if filter-kernel has dimension 1: just normalize
    if ((filterSize == 1) && (kernLastIdx == 0)) {
      dest.copy(src);
      return true;
    }

    int x,i,f,xx,endxx,endi,col,t,s;
    int begin,end;

    vector<T> firstResult;
    firstResult.allocate(filterSize);

    // initialize size of dest (if needed)
    if (dest.size() != src.size()) {
      dest.allocate(src.size());
    }

    // This is the standard way to implement the convolution
    //   for (x=begin,i=kern.lastIdx()+x;x<end;x++,i++) {
    //     // x: begin in src vector
    //     // i: place to write local dilation
    //     // lresult : local result
    //     // xx: sample on image to be checked
    //     for (xx=x,lresult.reset(),f=filterLast;f>=0;f--,xx++ ) {
    //       lresult.accumulate(filter.at(f),src.at(xx));
    //     }
    //     dest.at(i)=lresult.getResult();
    //   }

    if (src.rows() > filterSize) {
      // ----------------------------------------
      // if the kernel is smaller than the vector
      // ----------------------------------------

      // middle block filter (the whole kernel can be applied here!)
      begin = (kernLastIdx<0) ? -kernLastIdx : 0;
      end   = (kernLastIdx >= filterSize) ?
        src.rows()-kernLastIdx-1 :
        src.rows()-filterSize;

      // some weird kernels...
      // TODO: BUGGY this must still be fixed!
      // the best way to fix this is creating a minimumFilterHelper,
      // which assumes a normal kernel, and an extra function which
      // converts the "weird" kernel into an equivalent normal one!!!!

      if ((begin>=src.rows()) || (end<begin)) {
        switch (param.boundaryType) {
          case lti::Zero: {
            dest.fill(T(0));
            return true;
          }
            break;
          case lti::Mirror: {
            //TODO: redefine begin and end
          }
            break;
          case lti::Periodic: {
            begin = (begin % src.rows());
            // TODO: check end
          }
            break;
          case lti::Constant: {
            if (end>begin) {
              int idx = src.rows()-1;
              for (i=0;i<dest.rows();i++) {
                dest.getRow(i).copy(src.getRow(idx));
              }
            } else {
              for (i=0;i<dest.rows();i++) {
                dest.getRow(i).copy(src.getRow(0));
              }
            }
            return true;
          }
            break;
          case lti::NoBoundary: {
          }
            break;
          default: {
            setStatusString("unknown border type!");
            return false;
          }
        }
      }
      
      // first lresult value
      for (col=0;col<src.columns();col++) {
        
        x=begin;             // position at source
        i=kernLastIdx+begin; // position at destination

        buffer_[0]=src.at(x,col);
        for (xx=x+1,endxx=x+filterSize,s=2;
             xx!=endxx;
             ++xx,++s) {
          insert(s,src.at(xx,col));   // accumulate
        }
        
        // store result for first pixel
        firstResult.fill(buffer_);
        
        // store result for first pixel
        dest.at(i,col)=buffer_[0];

        // remember beginning!
        f = x;

        // next elements
        i++;
        
        // the rest pixels can be calculated using only one new element!

        // The loop
        for (;f<end;f++,i++) {
          // x: begin in src vector
          // i: place to write local dilation
          // lresult : local result
          // xx: sample on image to be checked
          insdel(filterSize,src.at(xx,col),src.at(f,col));
          dest.at(i,col)=buffer_[0];
          xx++;
        }

        // -----------------------
        //    border conditions
        // -----------------------
        endi = dest.rows();

        switch (param.boundaryType) {
            case lti::Zero:
              // right border!
              if (i>=f) {
                for (;i<endi;f++,i++) {
                  insdel(filterSize,0,src.at(f,col));
                  dest.at(i,col)=buffer_[0];
                }
              } else {
                for (;f<endi;f++,i++) {
                  insdel(filterSize,0,src.at(f,col));
                  dest.at(i,col)=buffer_[0];
                }
                for (;i<src.rows();i++) {
                  dest.at(i,col)=buffer_[0];
                }
              }
              // left border!

              x=begin+filterSize-1; // position at source to the last
              // element of the kernel region

              i=kernLastIdx+begin-1; // position at destination
              memcpy(buffer_,firstResult.data(),filterSize*sizeof(T));

              endi = -1;

              if (x>=i) {
                for (;i>=0;i--,x--) {
                  insdel(filterSize,0,src.at(x,col));
                  dest.at(i,col) = buffer_[0];
                }
              } else {
                for (;x>=0;i--,x--) {
                  insdel(filterSize,0,src.at(x,col));
                  dest.at(i,col) = buffer_[0];
                }
                for (;i>=0;i--) {
                  dest.at(i,col)=buffer_[0];
                }
              }

              break;

            case lti::Mirror:

              // right border!

              t = src.rows()-1; // pointer to the mirrored data

              if (i>=f) {
                for (;i<endi;f++,i++,t--) {
                  insdel(filterSize,src.at(t,col),src.at(f,col));
                  dest.at(i,col)=buffer_[0];
                }
              } else {
                for (;f<endi;f++,i++,t--) {
                  insdel(filterSize,src.at(t,col),src.at(f,col));
                  dest.at(i,col)=buffer_[0];
                }
                f--;
                for (;i<endi;f--,i++,t--) {
                  insdel(filterSize,src.at(t,col),src.at(f,col));
                  dest.at(i,col)=buffer_[0];
                }
              }

              // left border!

              x=begin+filterSize-1; // position at source to the last
              // element of the kernel region

              i=kernLastIdx+begin-1;  // position at destination
              memcpy(buffer_,firstResult.data(),filterSize*sizeof(T));

              endi = -1;

              t = 0;

              if (x>=i) {
                for (;i>=0;i--,x--,t++) {
                  insdel(filterSize,src.at(t,col),src.at(x,col));
                  dest.at(i,col)=buffer_[0];
                }
              } else {
                for (;x>=0;i--,x--,t++) {
                  insdel(filterSize,src.at(t,col),src.at(x,col));
                  dest.at(i,col)=buffer_[0];
                }
                x++;
                for (;i>=0;i--,x++,t++) {
                  insdel(filterSize,src.at(t,col),src.at(x,col));
                  dest.at(i,col)=buffer_[0];
                }
              }

              break;

            case lti::Periodic:
              // right border!

              t = 0; // pointer to the periodic data

              if (i>=f) {
                for (;i<endi;f++,i++,t++) {
                  insdel(filterSize,src.at(t,col),src.at(f,col));
                  dest.at(i,col)=buffer_[0];
                }
              } else {
                for (;f<endi;f++,i++,t++) {
                  insdel(filterSize,src.at(t,col),src.at(f,col));
                  dest.at(i,col)=buffer_[0];
                }
                f=0;
                for (;i<endi;f++,i++,t++) {
                  insdel(filterSize,src.at(t,col),src.at(f,col));
                  dest.at(i,col)=buffer_[0];
                }
              }

              // left border!

              x=begin+filterSize-1; // position at source to the last
              // element of the kernel region

              i=kernLastIdx+begin-1; // position at destination
              memcpy(buffer_,firstResult.data(),filterSize*sizeof(T));
              endi = -1;

              t = src.rows()-1;

              if (x>=i) {
                for (;i>=0;i--,x--,t--) {
                  insdel(filterSize,src.at(t,col),src.at(x,col));
                  dest.at(i,col)=buffer_[0];
                }
              } else {
                for (;x>=0;i--,x--,t--) {
                  insdel(filterSize,src.at(t,col),src.at(x,col));
                  dest.at(i,col)=buffer_[0];
                }
                x=src.rows()-1;
                for (;i>=0;i--,x--,t--) {
                  insdel(filterSize,src.at(t,col),src.at(x,col));
                  dest.at(i,col)=buffer_[0];
                }
              }

              break;
            case lti::Constant:
              T y;

              // right border!
              y = src.at(src.rows()-1,col);

              if (i>=f) {
                for (;i<endi;f++,i++) {
                  insdel(filterSize,y,src.at(f,col));
                  dest.at(i,col)=buffer_[0];
                }
              } else {
                for (;f<endi;f++,i++) {
                  insdel(filterSize,y,src.at(f,col));
                  dest.at(i,col)=buffer_[0];
                }
                for (;i<dest.rows();i++) {
                  dest.at(i,col)=buffer_[0];
                }
              }

              // left border!
              y = src.at(0,col);

              x=begin+filterSize-1; // position at source to the last
              // element of the kernel region

              i=kernLastIdx+begin-1; // position at destination
              memcpy(buffer_,firstResult.data(),filterSize*sizeof(T));
              endi = -1;

              if (x>=i) {
                for (;i>=0;i--,x--) {
                  insdel(filterSize,y,src.at(x,col));
                  dest.at(i,col)=buffer_[0];
                }
              } else {
                for (;x>=0;i--,x--) {
                  insdel(filterSize,y,src.at(x,col));
                  dest.at(i,col)=buffer_[0];
                }
                for (;i>=0;i--) {
                  dest.at(i,col)=buffer_[0];
                }
              }
              break;

            case lti::NoBoundary:
              break;
            default:
              throw exception("unknown border type!");
        }
      }
    } else {
      // ---------------------------------------
      // if the kernel is bigger than the vector
      // ----------------------------------------

      // compute the minimum of each column

      const int& kernFirstIdx = param.maskWindow.ul.x;

      if ((param.boundaryType == lti::Mirror) ||
          (param.boundaryType == lti::Periodic)) {

        // all elements are equal the minimum!
        for (col=0;col<src.columns();++col) {
          T tmp = src.at(0,col);
          for (f=1;f<src.rows();++f) {
            tmp = min(tmp,src.at(f,col));
          }

          for (f=0;f<dest.rows();++f) {
            dest.at(f,col) = tmp;
          }
        }

      } else if ((param.boundaryType == lti::Zero) ||
                 (param.boundaryType == lti::Constant)) {

        // initialize lresult
        for (col=0;col<src.columns();++col) {

          T right = src.at(src.rows()-1,col);
          T left =  src.at(0,col);

          if (param.boundaryType == lti::Zero) {
            right = T(0);
            left =  T(0);
          } else {
            right = src.at(src.rows()-1,col);
            left =  src.at(0,col);
          }

          // initialize the lresult with the kernel at(-1)
          const int lastInit = -kernFirstIdx;
          const int lastInit2 = min(lastInit,src.rows());
          const int lastInit0 = min(0,-kernFirstIdx);

          i=1;

          for (f=-kernLastIdx-1;f<lastInit0;++f) {
            insert(i,left);
            ++i;
          }

          for (;f<lastInit2;++f) {
            insert(i,src.at(f,col));
            ++i;
          }

          for (;f<lastInit;++f) {
            insert(i,right);
            ++i;
          }

          // the last kernel element
          int l;
          l = lastInit;

          // the first kernel element
          f = (-kernLastIdx-1);

          T ins,del;

          // apply kernel
          for (x=0;x<src.rows();++x,++f,++l) {

            if (f<0) {
              del=left;
            } else if (f>=src.rows()) {
              del=right;
            } else {
              del=src.at(f,col);
            }

            if (l<0) {
              ins=left;
            } else if (l>=src.rows()) {
              ins=right;
            } else {
              ins=src.at(l,col);
            }

            insdel(filterSize,ins,del);

            dest.at(x,col) = buffer_[0];
          }
        }
      }
    }

    return true;
  }

  template<class T>
  inline int minimumFilter<T>::getMirrorIndex(const int x,
                                              const int srcSize,
                                              bool& mirror) const {
    int tmp;
    mirror = (((x/srcSize)%2) != 0);

    tmp = x % srcSize;
    if (tmp<0) {
      tmp+=srcSize;
      mirror = !mirror;
    }

    if (mirror) {
      tmp = srcSize-1-tmp;
    }

    return tmp;
  }


}
