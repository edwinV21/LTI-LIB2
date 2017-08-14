/*
 * Copyright (C) 2008
 * Ronald Eduardo Garcia Fernandez
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
 * \file   ltiZhangSuenThinning.h
 *         Contains the class lti::zhangSuenThinning for computing the  
 *         Zhang Suen Thinning Algorithm
 * 
 * \author Ronald Eduardo Garcia Fernandez
 * \date   05/14/2008
 *
 * revisions ..: $Id: None
 */

#ifndef _LTI_ZHANG_SUEN_TRANSFORM_
#define _LTI_ZHANG_SUEN_TRANSFORM_

#include "ltiFunctor.h"
#include "ltiMatrix.h"

namespace lti {

  /**
   * Class lti::zhangSuenThinning
   *
   * This class implements the Zhang Suen thinning algorithm for image
   * skeletonization, as described in:
   *
   * G. X. Ritter and Joseph N. Wilson.  Handbook of computer vision algorithms
   * in image algebra. 2nd Edition. CRC Press. 2001
   *
   * This class operatates on binary channels only, where the two classes
   * are defined as equal or not-equal to zero.
   *
   * For floating point matrices, the end values given in the parameters
   * are normalized by the respective constants.
   *
   * The algorithm implemented is not homotopy preserving, i.e., the original
   * image cannot be reconstructed from the obtained skeleton.
   *
   * @see lti::zhangSuenThinning::parameters
   *
   * @ingroup gMorphology
   */
  class zhangSuenThinning : public functor {
  public:
    /**
     * The parameters for the class zhangSuenThinning
     */
    class parameters : public functor::parameters {
    public:
      /**
       * Default constructor
       */
      parameters();

      /**
       * Copy constructor
       * @param other the parameters object to be copied
       */
      parameters(const parameters& other);

      /**
       * Destructor
       */
      ~parameters();

      /**
       * Copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& copy(const parameters& other);

      /**
       * Copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& operator=(const parameters& other);

      /**
       * Returns the complete name of the parameters class.
       */
      virtual const std::string& name() const;

      /**
       * Returns a pointer to a clone of the parameters.
       */
      virtual parameters* clone() const;

      /**
       * Returns a pointer to a new instance of the parameters.
       */
      virtual parameters* newInstance() const;

      /**
       * Write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool write(ioHandler& handler,const bool complete=true) const;

      /**
       * Read the parameters from the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool read(ioHandler& handler,const bool complete=true);

      // ------------------------------------------------
      // the parameters
      // ------------------------------------------------

      /**
       * Maximal number of iterations for the algorithm
       *
       * Default value: -1 (i.e., no limit).
       */
      int maxIterations;

      /**
       * Background value
       *
       * The value used for represent background in the resulting image.
       *
       * This value will be divided by 255 for the floating point methods.
       *
       * Default value: 0
       */
      ubyte backgroundValue;

      /**
       * Line value
       *
       * The value used for representing the thinned lines in the resulting
       * image.
       *
       * This value will be divided by 255 for the floating point methods.
       * 
       * Default value: 255 (i.e. 1 for the floating point channels)
       */
      ubyte lineValue;
    };

    /**
     * Default constructor
     */
    zhangSuenThinning();

    /**
     * Construct a Zhang Suen Thinning functor using the given parameters
     */
    zhangSuenThinning(const parameters& par);

    /**
     * Construct a Zhang Suen Thinning functor using the given limit of
     * iterations maxIterations.
     *
     * @param maxIterations limit of iterations maxIterations.
     */
    zhangSuenThinning(const int maxIterations);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    zhangSuenThinning(const zhangSuenThinning& other);

    /**
     * Destructor
     */
    virtual ~zhangSuenThinning();

    /**
     * Operates on the given argument.
     *
     * @param srcDest fmatrix with the source data. The result
     *                will be left here too.
     * @return true if apply successful or false otherwise.
     */
    bool apply(fmatrix& srcDest) const;

    /**
     * Operates on the given argument.
     *
     * @param srcDest matrix<ubyte> with the source data. The result
     *                will be left here too.
     * @return true if apply successful or false otherwise.
     */
    bool apply(matrix<ubyte>& srcDest) const;

    /**
     * Operates on a copy of the given arguments.
     *
     * @param src fmatrix with the source data.
     * @param dest fmatrix where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const fmatrix& src, fmatrix& dest) const;

    /**
     * Operates on a copy of the given arguments.
     *
     * @param src matrix<ubyte> with the source data.
     * @param dest matrix<ubyte> where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const matrix<ubyte>& src, matrix<ubyte>& dest) const;

    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    zhangSuenThinning& copy(const zhangSuenThinning& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    zhangSuenThinning& operator=(const zhangSuenThinning& other);

    /**
     * Returns the complete name of the functor class
     */
    virtual const std::string& name() const;

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual zhangSuenThinning* clone() const;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual zhangSuenThinning* newInstance() const;

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;

  private:

    /**
     * Obtain the skeleton of the given matrix<ubyte> 
     */
    template<typename T>
    bool skeletonImage(const matrix<T>& src,
                       matrix<T>& dest) const;
    
  };
}

#endif

