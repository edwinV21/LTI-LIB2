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
 * \file   ltiIOImage.h
 *         Parent class of all functors that write to or read from a file as
 *         its main task.
 * \author Pablo Alvarado
 * \date   11.01.2006
 *
 * $Id: ltiIOImage.h,v 1.3 2007-04-08 03:12:38 alvarado Exp $
 */

#ifndef _LTI_IO_IMAGE_H_
#define _LTI_IO_IMAGE_H_

#include "ltiIOImageInterface.h"
#include "ltiMatrix.h"
#include "ltiImage.h"
#include <map>
#include <string>

namespace lti {
  /** 
   * Class to read/write image files.
   *
   * This class makes use of all its sibling classes to save or load image
   * files in all supported formats (bmp, jpg, png).
   *
   * Example:
   * \code
   * lti::ioImage imgFiler; // create the object to save/load images
   * lti::image img(256,256,lti::White); // create a white image 256x256
   * imgFiler.save("myImage.png",img);   // save image as png file
   * 
   * lti::image other;                   // another variable
   * imgFiler.load("myImage.png",other); // load the recently created file.
   * \endcode
   */
  class ioImage : public ioImageInterface {
  public:
     
    /**
     * Default constructor
     */
    ioImage();

    /**
     * Copy constructor
     */
    ioImage(const ioImage& other);

    /**
     * destructor
     */
    virtual ~ioImage();

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
    virtual ioImage* clone() const;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual ioImage* newInstance() const;

    /**
     * Copy operator
     */
    ioImage& copy(const ioImage& other);
    
    /**
     * Copy operator
     */
    ioImage& operator=(const ioImage& other);

    /**
     * @name Loading images and channels
     */
    //@{
    /**
     * Load file as a color image.
     */
    virtual bool load(const std::string& filename,
                      image& img);

    /**
     * Load a channel8 as a label mask with its corresponding palette.
     *
     * If the file contained a color image, then the functor has to quantize
     * the colors and returned the quantized table.
     */
    virtual bool load(const std::string& filename,
                      matrix<ubyte>& chnl,
                      palette& pal);

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
                             headerInformation& info);
    //@}


    /**
     * @name Saving images and channels
     */
    //@{
    /**
     * Save file as a color image.
     */
    virtual bool save(const std::string& filename,
                      const image& img);

    /**
     * Save a channel8 as a label mask with its corresponding palette.
     *
     * If the file contained a color image, then the functor has to quantize
     * the colors and returned the quantized table.
     */
    virtual bool save(const std::string& filename,
                      const matrix<ubyte>& chnl,
                      const palette& pal);

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

    /**
     * Returns true if the given extension \p ext is supported (i.e. can be
     * loaded/saved). The argument is not case sensitive. 
     *
     * This function is dedicated for more sophisticated IO interfaces that
     * use ioImage to actually load different image types.
     */
    bool isSupportedExtension(const std::string& ext) const;
    

  protected:
    /**
     * Local object repository
     */
    std::map<std::string,ioImageInterface*> ioObjects_;

    /**
     * Build repository
     *
     * Use the factory to create the repository
     */
    bool buildRepository();

    /**
     * Clear repository
     *
     * Remove all objects in the repository
     */
    bool clearRepository();

    /**
     * Get file extension 
     */
    std::string getFileExtension(const std::string& filename) const;

    /**
     * Search an appropriate functor for the given file extension, or
     * null if there is nothing available.
     */
    ioImageInterface* get(const std::string& ext);

    
  };

}

#endif
