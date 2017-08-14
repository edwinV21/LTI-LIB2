/*
 * Copyright (C) 2009
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
 * \file   ltiMorphology.h
 *         Contains the class lti::morphology, which implements basic general
 *         constructs for morphological operators.
 * \author Pablo Alvarado
 * \date   10.10.2009
 *
 * revisions ..: $Id: ltiMorphology.h,v 1.2 2010-04-25 05:28:18 alvarado Exp $
 */


#ifndef _LTI_MORPHOLOGY_H_
#define _LTI_MORPHOLOGY_H_

#include "ltiIoObject.h"
#include "ltiImage.h"
#include "ltiContainer.h"
#include "ltiMatrixProcessingInterface.h"
#include "ltiBoundaryType.h"
#include "ltiTypes.h"
#include "ltiMatrix.h"
#include "ltiFunctor.h"

namespace lti {
  /**
   * Morphology abstract class.
   *
   * This abstract class defines the basic interface for all basic morphology
   * classes and defines the parameters such that the user can set the
   * structuring element.
   *
   * @see lti::morphology::parameters
   *
   * @ingroup gMorphology
   */
  class morphology : public functor,
                     public matrixProcessingInterface<float>,
                     public matrixProcessingInterface<ubyte> {

  public:

    /**
     * eMorphologyMode
     *
     *
     * The morphology type allows to select between morphology for gray valued 
     * channels or for binary channels.
     *
     * For this type the methods read(ioHandler&, eMorphologyMode&) and
     * write(ioHandler&, const eMorphologyMode&) are implemented. Note
     * that template functions read(ioHandler&, const std::string&, T&)
     * and write(ioHandler&, const std::string&, const T&) exist. (see
     * Developer's Guide for further information).
     *
     * @ingroup gTypes
     */
    enum eMorphologyMode {
      Binary,      /**< Morphology for binary images. */
      GrayFlat,    /**< Morphology for gray valued images with flat structuring
                    *   elements. 
                    */
      GrayNonFlat  /**< Morphology for gray valued images with non-flat
                    *   (i.e. gray valued) structuring elements. 
                    */
    };
    

    /**
     * Parameters for the class lti::morphology
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
       * Returns name of this type
       */
      const std::string& name() const;

      /**
       * Copy the contents of a parameters object
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
       * Sets the structuring element to be used.
       * A copy of the given parameter will be made!
       * @param se the structuring element to be used
       */
      void setStructuringElement(const container& se);

      /**
       * Returns the structuring element in use.  If it is not set yet, an
       * lti::invalidParameters exception will be thrown
       * @return a const reference to the filter kernel.
       */
      const container& getStructuringElement() const;

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

      // ---------------------------------------------------
      // the parameters
      // ---------------------------------------------------

      /**
       * Morphology mode.
       *
       * The morphology is defined for "Binary" images (only two values per
       * pixel), for gray scaled images (channels or channel8) with a flat 
       * structuring element (GrayFlat), and for gray scaled images with a 
       * gray valued structuring element (GrayNonFlat).
       *
       * In "Binary" mode, the channels and channel8s will be interpreted as
       * not belonging to the set of elements with "0" and belonging to the
       * set with any other value.
       *
       * Default value: "Binary"
       */
      eMorphologyMode mode;

      /**
       * Boundary Type
       *
       * How to treat the input channel or image outside the boundaries.
       *
       * Default value: eBoundaryType::Zero
       */
      eBoundaryType boundaryType;

    protected:
      /**
       * The structuring element to be used.
       * This parameter can only be set through the setStructuringElement
       * member.
       */
      container* structuringElement_;
    };

    /**
     * Default constructor
     */
    morphology();

    /**
     * Construct  with the given structuring element
     */
    morphology(const container& aKernel);

    /**
     * Construct with the given parameters
     */
    morphology(const parameters& par);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    morphology(const morphology& other);

    /**
     * Destructor
     */
    virtual ~morphology();

    /**
     * Return the name of this class
     */
    virtual const std::string& name() const = 0;

    /**
     * Operates on the given parameter.
     * @param srcdest fmatrix with the source data.  The result
     *                 will be left here too.
     * @result a reference to the <code>srcdest</code>.
     */
    virtual bool apply(fmatrix& srcdest) const = 0;

    /**
     * Operates on the given parameter.
     * @param srcdest matrix<ubyte> with the source data.  The result
     *                 will be left here too.
     * @return true if successful, false otherwise.
     */
    virtual bool apply(matrix<ubyte>& srcdest) const = 0;

    /**
     * Operates on the given parameter.
     * @param srcdest fvector with the source data.  The result
     *                 will be left here too.
     * @return true if successful, false otherwise.
     */
    virtual bool apply(fvector& srcdest) const = 0;

    /**
     * Operates on the given parameter.
     * @param srcdest vector<ubyte> with the source data.
     *                The result will be left here too.
     * @return true if successful, false otherwise.
     */
    virtual bool apply(vector<ubyte>& srcdest) const = 0;

    /**
     * Operates on a copy of the given parameters.
     * @param src fmatrix with the source data.
     * @param dest fmatrix where the result will be left.
     * @return true if successful, false otherwise.
     */
    virtual bool apply(const fmatrix& src,fmatrix& dest) const = 0;

    /**
     * Operates on a copy of the given parameters.
     * @param src matrix<ubyte> with the source data.
     * @param dest matrix<ubyte> where the result will be left.
     * @return true if successful, false otherwise.
     */
    virtual bool apply(const matrix<ubyte>& src,matrix<ubyte>& dest) const = 0;

    /**
     * Operates on a copy of the given parameters.
     * @param src fvector with the source data.
     * @param dest fvector where the result will be left.
     * @return true if successful, false otherwise.
     */
    virtual bool apply(const fvector& src,fvector& dest) const = 0;

    /**
     * Operates on a copy of the given parameters.
     * @param src vector<ubyte> with the source data.
     * @param dest vector<ubyte> where the result will be left.
     * @return true if successful, false otherwise.
     */
    virtual bool apply(const vector<ubyte>& src,
                       vector<ubyte>& dest) const = 0;

    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    morphology& copy(const morphology& other);

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual morphology* clone() const = 0;

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual morphology* newInstance() const = 0;

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * Shortcut to set the structuring element in the functor's parameters.
     *
     * The other parameters remain unchanged.
     */
    void setStructuringElement(const container& se);
  };

  /**
   * Read function for eMorphologyMode
   *
   * @ingroup gStorable
   */
  bool read(ioHandler& handler,morphology::eMorphologyMode& data);

  /**
   * Write function for eMorphologyMode
   *
   * @ingroup gStorable
   */
  bool write(ioHandler& handler,const morphology::eMorphologyMode& data);

}

#endif
