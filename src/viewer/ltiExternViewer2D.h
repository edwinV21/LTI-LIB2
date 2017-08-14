/*
 * Copyright (C) 2007 by Pablo Alvarado
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
 * \file   ltiExternViewer2D.h
 *         Contains the class lti::externViewer used when there is no 
 *         possibility to use GTK or any other widget tool for which we have a
 *         viewer
 * \author Pablo Alvarado
 * \date   21.07.99
 *
 * revisions ..: $Id: ltiExternViewer2D.h,v 1.1 2007-10-22 00:48:00 alvarado Exp $
 */

#ifndef _LTI_EXTERNVIEWER_2D_H_
#define _LTI_EXTERNVIEWER_2D_H_

#include "ltiMatrix.h"
#include "ltiRGBAPixel.h"
#include "ltiParametersManager.h"
#include "ltiStatus.h"
#include "ltiIoObject.h"

#include <string>
#include <list>

namespace lti {
  /**
   * External viewer for 2D images.
   *
   * This object shows an image using an external application like
   * kview, xv, mspaint, etc.
   *
   * Use the externViewer::parameters to specify which application
   * should be used.
   *
   * A temporal image file will be created to transfer the image data
   * to the external application.  If you need a faster viewer, use
   * the lti::viewer2D instead!
   *
   * The default viewer used in Linux is the kview and in Windows the
   * mspaint.  Make sure that these programs are included in one of
   * the directories specified by the system variable PATH.
   */
  class externViewer2D : public ioObject,
                         public status,
                         public parametersManager {

  public:

    /**
     * Parameters of the lti::externViewer2D class
     */
    class parameters : public parametersManager::parameters {
    public:
      /**
       * default constructor
       */
      parameters();

      /**
       * copy constructor
       */
      parameters(const parameters& other);

      /**
       * destructor
       */
      virtual ~parameters();

      /**
       * copy member
       */
      parameters& copy(const parameters& other);

      /**
       * copy data of "other" parameters
       */
      parameters& operator=(const parameters& other);

      /**
       * Returns the name of this class
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
      virtual bool write(ioHandler& handler,
                         const bool complete=true) const;

      /**
       * Read the parameters from the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool read(ioHandler& handler,const bool complete=true);

      // ------------------------
      // the parameters
      // ------------------------

      /**
       * Name of the directory that will hold the temporary image files.
       *
       * Default directory: /tmp in Linux and C:\\TEMP in Windows
       */
      std::string tmpDirectory;

      /**
       * Name of the external application.
       *
       * Default application: kview in Linux and MSPaint in Windows
       */
      std::string externViewerApp;
    };

    // --------------------------------------------------
    // externViewer
    // --------------------------------------------------

    /**
     * Default constructor
     */
    externViewer2D();

    /**
     * Constructor with a window name 
     *
     * (some implementations ignore the given name)
     */
    externViewer2D(const std::string& wndName);

    /**
     * Constructor with parameters
     */
    externViewer2D(const parameters& par);

    /**
     * Copy constructor
     */
    externViewer2D(const externViewer2D& other);

    /**
     * Default destructor
     */
    ~externViewer2D();

    /**
     * Returns the name of this class
     */
    virtual const std::string& name() const;
    
    /**
     * Returns a pointer to a clone of this instance.
     */
    virtual externViewer2D* clone() const;
    
    /**
     * Returns a pointer to a new instance of this class.
     */
    virtual externViewer2D* newInstance() const;

    /**
     * copy data of "other" functor.
     */
    externViewer2D& copy(const externViewer2D& other);

    /**
     * copy data of "other" functor.
     */
    externViewer2D& operator=(const externViewer2D& other);

    /**
     * returns the give parameters
     */
    const parameters& getParameters() const;

    /**
     * Shows an lti::image
     * @param data the object to be shown.
     * @return true if successful, false otherwise.
     */
    bool show(const matrix<rgbaPixel>& data);

    /**
     * Shows a 8-bit channel
     * @param data the object to be shown.
     * @return true if successful, false otherwise.
     */
    virtual bool show(const matrix<ubyte>& data);

    /**
     * Shows a floating point channel
     * @param data the object to be shown.
     * @return true if successful, false otherwise.
     */
    virtual bool show(const matrix<float>& data);

    /**
     * Shows an integer matrix.
     * @param data the object to be shown.
     * @return true if successful, false otherwise.
     */
    virtual bool show(const matrix<int32>& data);

    /**
     * hides the display window
     * @return true if successful, false otherwise.
     */
    virtual bool hide();


  protected:
    /**
     * How many times has 'show' been used?
     */
    static int numberShows;

  }; // class externViewer2D


} // namespace lti

#endif // _LTI_EXTERNVIEWER_2D_H_
