/*
 * Copyright (C) 1998-2006
 * Electronics Engineering School, ITCR, Costa Rica
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
 * \file   ltiIOImageInterface.h
 *         Parent class of all functors that write to or read from a file as
 *         its main task.
 * \author Pablo Alvarado
 * \date   11.01.2006
 *
 * $Id: ltiIOImageInterface.h,v 1.7 2014-08-21 09:03:49 alvarado Exp $
 */

#ifndef _LTI_IO_IMAGE_INTERFACE_H_
#define _LTI_IO_IMAGE_INTERFACE_H_

#include "ltiParametersManager.h"
#include "ltiMatrix.h"
#include "ltiImage.h"

namespace lti {
  /** 
   * Base class to all classes that read from or write to a image file.
   *
   * It is a very simple class with nothing but an empty definition of a
   * parameters class with the attribute "filename".
   *
   * Note that these classes cannot be considered LTI-Lib functors, as they
   * lack the for the later mandatory apply() methods.
   *
   * \ingroup gInterfaces
   */
  class ioImageInterface : public object,
                           public status, 
                           public parametersManager {
  public:
    /**
     * Parameters of the ioImageInterface class
     */
    class parameters : public parametersManager::parameters {
    public:
      /**
       * Default constructor
       */
      parameters();

      /**
       * Copy constructor
       */
      parameters(const parameters& other);

      /**
       * Destructor
       */
      ~parameters();
      
      /**
       * Copy member
       */
      parameters& copy(const parameters& other);

      /**
       * Copy member
       */
      parameters& operator=(const parameters& other);

      /**
       * Returns the name of this class.
       */
      const std::string& name() const;

      /**
       * Returns a pointer to a clone of the parameters
       */
      virtual parameters* clone() const = 0;

      /**
       * Returns a pointer to a new instance of the parameters
       */
      virtual parameters* newInstance() const = 0;

      /**
       * Write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool write(ioHandler& handler,const bool complete=true) const;

      /**
       * Write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool read(ioHandler& handler,const bool complete=true);

    };

    /**
     * Common header information block.
     *
     * All inherited classes have to provide a way to get information from the
     * file without reading it completely.  The information can vary among the
     * file formats, but usually this common data set can be obtained.
     */
    class headerInformation : public ioObject {
    public:
      /**
       * Default constructor
       */
      headerInformation();

      /**
       * Default constructor
       */
      headerInformation(const headerInformation& other);

      /**
       * Returns the name of this class
       */
      virtual const std::string& name() const;
      
      /**
       * Returns a copy of this object
       */
      virtual headerInformation* clone() const;
      
      /**
       * Returns a copy of this object
       */
      virtual headerInformation* newInstance() const;
      
      /**
       * Copy 
       */
      headerInformation& copy(const headerInformation& other);
      
      /**
       * Copy 
       */
      headerInformation& operator=(const headerInformation& other);

      /**
       * Write the parameters in the given ioHandler
       *
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       *
       * This method has to be overloaded.  If not it always returns false.
       */
      virtual bool write(ioHandler& handler, const bool complete=true) const;
      
      /**
       * Read the parameters from the given ioHandler
       *
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       *
       * This method has to be overloaded.  If not it always returns false.
       */
      virtual bool read(ioHandler& handler,const bool complete=true);
      
      /**
       * A std::string coding the data type.  It is usually a 3-characters long
       * string.  Usual values are:
       * - "LTI" for LTI format
       * - "PNG" for PNG format
       * - "JPG" for JPG format
       * - "BMP" for BMP format
       */
      std::string type;

      /**
       * Size of the image in the file
       */
      ipoint size;

      /**
       * Number of bits per pixel in the file
       */
      int bpp;

      /**
       * Number of color entries in an internal palette, which usually has to
       * be less than 2^bpp, and zero if there is no palette. 
       */
      int colorEntries;

      /**
       * Extra information depending on the format
       */
      std::string info;
    };
     
    /**
     * Default constructor
     */
    ioImageInterface();

    /**
     * Copy constructor
     */
    ioImageInterface(const ioImageInterface& other);

    /**
     * destructor
     */
    virtual ~ioImageInterface();

    /**
     * returns current parameters.
     */
    const parameters& getParameters() const;

    /**
     * Returns the name of this class
     */
    virtual const std::string& name() const;

    /**
     * Returns a pointer to a clone of the functor.
     */
    virtual ioImageInterface* clone() const = 0;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual ioImageInterface* newInstance() const = 0;

    /**
     * Copy operator
     */
    ioImageInterface& copy(const ioImageInterface& other);
    
    /**
     * Copy operator
     */
    ioImageInterface& operator=(const ioImageInterface& other);

    /**
     * @name Loading images and channels
     */
    //@{
    /**
     * Load file as a color image.
     */
    virtual bool load(const std::string& filename,
                      image& img) = 0;

    /**
     * Load a channel8 as a label mask with its corresponding palette.
     *
     * If the file contained a color image, then the functor has to quantize
     * the colors and returned the quantized table.
     */
    virtual bool load(const std::string& filename,
                      matrix<ubyte>& chnl,
                      palette& pal) = 0;

    /**
     * Load the contents as a gray channel.
     *
     * Default implementation just discards the palette of 
     * load(const std::string&,matrix<ubyte>&,palette&)
     */
    virtual bool load(const std::string& filename,
                      matrix<ubyte>& chnl);
                 

    /**
     * Load a floating point channel.  Not many file formats support floating
     * point channels, so the default implementation just casts a channel8 into
     * the float.
     */
    virtual bool load(const std::string& filename,
                      matrix<float>& chnl);

    /**
     * Load the contents as a integer mask
     *
     * Default implementation returns the casting of the channel8 related
     * method.
     */
    virtual bool load(const std::string& filename,
                      matrix<int32>& chnl,
                      palette& pal);

    /**
     * Load the contents as a integer mask
     *
     * Default implementation discards the palette of the other matrix<int32>
     * method.
     */
    virtual bool load(const std::string& filename,
                      matrix<int32>& chnl);
                 
    /**
     * Check the file header for common information.
     *
     * All classes implementing this interface usually provide additional 
     * methods to check other options of the specific file formats, or overload
     * the headerInformation to include additional items.
     */
    virtual bool checkHeader(const std::string& filename,
                             headerInformation& info) = 0;
    //@}


    /**
     * @name Saving images and channels
     */
    //@{
    /**
     * Save file as a color image.
     */
    virtual bool save(const std::string& filename,
                      const image& img) = 0;

    /**
     * Save a channel8 as a label mask with its corresponding palette.
     *
     * If the file contained a color image, then the functor has to quantize
     * the colors and returned the quantized table.
     */
    virtual bool save(const std::string& filename,
                      const matrix<ubyte>& chnl,
                      const palette& pal) = 0;

    /**
     * Save the contents as a gray channel.
     *
     * Default implementation just discards the palette of 
     * save(const std::string&,matrix<ubyte>&,palette&)
     */
    virtual bool save(const std::string& filename,
                      const matrix<ubyte>& chnl);
                 

    /**
     * Save a floating point channel.
     *
     * Not many file formats support floating point channels, so the default
     * implementation just casts the channel to a channel8, which is saved.
     * You normally lose precision.
     */
    virtual bool save(const std::string& filename,
                      const matrix<float>& chnl);

    /**
     * Save the contents as a integer mask.
     *
     * Default implementation returns the casting of the channel8 related
     * method.
     */
    virtual bool save(const std::string& filename,
                      const matrix<int32>& chnl,
                      const palette& pal);

    /**
     * Save the contents as a integer mask.
     *
     * Default implementation discards the palette of the other matrix<int32>
     * method.
     */
    virtual bool save(const std::string& filename,
                      const matrix<int32>& chnl);
    
    //@}
  protected:
    /**
     * Method to get a read-only reference to a 256 long gray-palette.
     */
    const palette& getGrayPalette() const;
  };

}

#endif
