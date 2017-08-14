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
 * \file   ltiMinimumFilter.h
 *         Contains the class lti::minimumFilter, 
 *         which computes the minimum value within a rectangular window.
 * 
 * \author Pablo Alvarado
 * \date   08.01.2002
 *
 * revisions ..: $Id: ltiMinimumFilter.h,v 1.1 2009-03-26 22:15:30 alvarado Exp $
 */

#ifndef _LTI_MINIMUM_FILTER_H_
#define _LTI_MINIMUM_FILTER_H_ 

#include "ltiFunctor.h"
#include "ltiMatrix.h"
#include "ltiBoundaryType.h"

#include <cstring>

namespace lti {
  /**
   * Minimum filter.
   *
   * This is an efficient implementation of the non-linear minimum filter.
   * It assigns to each pixel the minimal value within the region defined by
   * the parameter kernelSize.
   *
   * The template parameter T defines the type of the matrix or vector
   * to be filtered.
   *
   * An instance of this class cannot be used from different threads (not
   * thread-save).  If you have multiple threads, use simply one functor for
   * each thread.
   *
   * @see minimumFilter::parameters.
   *
   * @ingroup gNonLinearFilters
   */
  template<class T>
  class minimumFilter : public functor {
  public:

    /**
     * the parameters for the class minimumFilter
     */
    class parameters : public functor::parameters {
    public:
      /**
       * Constructor that initializes the kernel mask to a size of k x k,
       * centered at 0,0
       *
       * @param k size of the side of the mask (default value k=3)
       */
      parameters(const int k = 3);

      /**
       * Copy constructor
       *
       * @param other the parameters object to be copied
       */
      parameters(const parameters& other);

      /**
       * Destructor
       */
      ~parameters();

      /**
       * Name of this class
       */
      const std::string& name() const;

      /**
       * Copy the contents of a parameters object
       *
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& copy(const parameters& other);

      /**
       * Returns a pointer to a clone of the parameters
       */
      virtual parameters* clone() const;

      /**
       * Returns a pointer to a new instance of the parameters
       */
      virtual parameters* newInstance() const;

      /**
       * Write the parameters in the given ioHandler
       *
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be
       *        written.
       * @return true if write was successful
       */
      virtual bool write(ioHandler& handler,const bool complete=true) const;

      /**
       * Read the parameters from the given ioHandler
       *
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also read, otherwise only the data block will be read.
       * @return true if write was successful
       */
      virtual bool read(ioHandler& handler,const bool complete = true);

      /**
       * Boundary type
       *
       * Specifies how to consider the outside borders of the channels.
       *
       * @see eBoundaryType
       *
       * Default value: Mirror.
       */
      eBoundaryType boundaryType;
      
      /**
       * The rectangular kernel mask defines the region around the actual pixel
       * in which the minimum value will be determined.
       *
       * Default value: irectangle(-1,-1,1,1), i.e. a 3x3 kernel
       */
      irectangle maskWindow;

      /**
       * Initialize the parameters for a symmetric square kernel size x size.
       * If the size is even, it will be "fixed" to the next odd number
       */
      void initSquare(const int size);
    };

    /**
     * Default constructor
     */
    minimumFilter();

    /**
     * Constructor with a set of parameters
     */
    minimumFilter(const parameters& par);

    /**
     * Constructor to initialize the parameters with an square filter
     * of k x k size.
     */
    minimumFilter(const int k);

    /**
     * Copy constructor
     *
     * @param other the object to be copied
     */
    minimumFilter(const minimumFilter& other);

    /**
     * Destructor
     */
    virtual ~minimumFilter();

    /**
     * Returns the name of this type ("minimumFilter")
     */
    virtual const std::string& name() const;

    /**
     * Get the minimum-filtered channel on-place
     *
     * @param srcdest matrix<T> with the source data.  The result
     *                 will be left here too.
     * @return true if successful, false otherwise.
     */
    bool apply(matrix<T>& srcdest);

    /**
     * Get the minimum-filtered vector on-place
     *
     * @param srcdest vector<T> with the source data.  The result
     *                 will be left here too.
     * @return true if successful, false otherwise.
     */
    bool apply(vector<T>& srcdest);

    /**
     * Get the minimum-filtered channel on an additional channel.
     *
     * @param src matrix<T> with the source data.
     * @param dest matrix<T> where the result will be left.
     *
     * @return true if successful, false otherwise.
     */
    bool apply(const matrix<T>& src,matrix<T>& dest);

    /**
     * Get the minimum-filtered vector on an additional vector.
     *
     * @param src vector<T> with the source data.
     * @param dest vector<T> where the result will be left.
     * @return true if successful, false otherwise.
     */
    bool apply(const vector<T>& src,vector<T>& dest);

    /**
     * Copy data of "other" functor.
     *
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    minimumFilter& copy(const minimumFilter& other);

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual minimumFilter<T>* clone() const;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual minimumFilter<T>* newInstance() const;

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * Shortcut to set the parameters with the given kernel size
     */
    bool setMaskWindow(const irectangle& wsize);

    /**
     * Shortcut to set the parameters with a square kernel 
     *
     * @see parameters::initSquare()
     */
    bool setSquareMaskWindow(const int wsize);

    /**
     * Update parameters.
     *
     * @return true if successful, false otherwise
     */
    virtual bool updateParameters();


  protected:
    /**
     * Returns used parameters
     */
    parameters& getRWParameters();

    /**
     * Applies the vertical part of the kernel to the matrix
     *
     * @param src matrix<T> with the source data.
     * @param dest matrix<T> where the result will be left.
     * @result a reference to the <code>dest</code>.
     */
    bool applyCol(const matrix<T>& src,matrix<T>& dest);

    /**
     * Calculate the real index in an vector of size srcSize for a
     * mirrored boundary
     */
    inline int getMirrorIndex(const int x,
                              const int srcSize,
                              bool& mirror) const;

    /**
     * Sorted elements.
     *
     * The memory is administrated by setParameters and the destructor.
     */
    T* rawBuffer_;

    /**
     * Pointer to the buffer withing rawBuffer.
     *
     * There are elements after and before used to check boundary conditions
     */
    T* buffer_;


    /**
     * Insert the element "elem" in the buffer of size "size" pointed by
     * the attribute buffer_. The last element of the buffer_ is supposed
     * to be uninitialized.
     */
    inline void insert(const int size,const T elem);

    /**
     * Insert the element "insElem" in the given buffer of size "size", and
     * remove the element "delElem".
     */
    inline void insdel(const int size,const T insElem,const T delElem);
  };



}
#endif
