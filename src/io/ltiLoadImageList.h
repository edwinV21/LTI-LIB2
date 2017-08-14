/*
 * Copyright (C) 2006
 * Peter Doerfler
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
 * \file   ltiLoadImageList.h
 *         Contains the class lti::loadImageList, which has still 
 *         to be documented by Peter Doerfler
 * \author Peter Doerfler
 * \date   10.2.2006
 *
 * revisions ..: $Id: ltiLoadImageList.h,v 1.7 2009-09-18 00:55:33 alvarado Exp $
 */

#ifndef _LTI_LOAD_IMAGE_LIST_H_
#define _LTI_LOAD_IMAGE_LIST_H_

#include "ltiObject.h"
#include "ltiParametersManager.h"
#include "ltiStatus.h"

#include "ltiMatrix.h"
#include "ltiImage.h"

#include "ltiIOImage.h"
#include <list>
#include <vector>
#include <string>

namespace lti {

  /**
   * Load lists of images
   *
   * This class allows convenient loading of a set of images. It can be
   * supplied with a directory, a text file containing image filenames, or
   * forward iterators to strings. To be consistent over different operating
   * systems the filenames are sorted alphabetically.
   *
   * Using this class is somewhat similar to using an iterator. However, to
   * make sure no copying is involved in loading the images the destination
   * objects are arguments to the load() member functions instead of
   * dereferencing an iterator. The following example shows how to use the
   * class:
   *
   * \code
   * loadImageList loader;
   * loader.useDirectory("/home/foo/bar");
   * viewer2D viewer;
   * viewer2D::interaction action;
   * image img;
   * while (loader.hasNext()) {
   *   loader.load(img);
   *   viewer.show(img);
   *   viewer.waitKey(action);
   * }
   * \endcode
   *
   * The example first finds all image files in the directory
   * /home/foo/bar. Then it shows them consecutively, each image until a key
   * is pressed.
   *
   * There are also \c load() member functions that load pointers to all
   * images at once into a std::vector. This can be useful e.g. if the lag of
   * loading the images can't be tolerated in latter processing. \b NOTE that
   * the user is responsible for deleting the allocated memory. Not using this
   * way of memory managment can result in excessive copying of the images.
   *
   * There are also member functions to retrieve the name of the next image
   * before actually loading it and to retrieve all filenames.
   *
   * You can confine the image filetype you want to load by setting
   * parameters::fileType to something other than "ALL". Also you can
   * configure via continueOnError that errors while loading a file should be
   * ignored and the next valid image should be returned instead.
   *
   * @see loadImageList::parameters.
   *
   * @ingroup IOImage
   */
  class loadImageList 
      : public object, public parametersManager, public status {
  public:
    /**
     * The parameters for the class loadImageList
     */
    class parameters : public parametersManager::parameters {
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
       * If set to true then errors on reading an image file are ignored and
       * the next image is tried. This can be useful for batch processing.
       *
       * Default: false
       */
      bool continueOnError;

      /**
       * Determines the filetypes that should be loaded. Available filetypes
       * can be found by looking at the subclasses of
       * lti::IOImageInterface. Just use the typical three letter
       * abbreviations, e.g. PNG for portable network graphics. You can also
       * use ALL to load all available file types.
       *
       * Default: "ALL"
       */
      std::string fileType;

    };

    /**
     * Default constructor
     */
    loadImageList();

    /**
     * Construct using the given parameters
     */
    loadImageList(const parameters& par);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    loadImageList(const loadImageList& other);

    /**
     * Destructor
     */
    virtual ~loadImageList();

    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    loadImageList& copy(const loadImageList& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    loadImageList& operator=(const loadImageList& other);

    /**
     * Returns the complete name of the functor class
     */
    virtual const std::string& name() const;

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual loadImageList* clone() const;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual loadImageList* newInstance() const;

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * Makes sure the given parameters esp. fileType are valid.
     */
    virtual bool updateParameters();

    /**
     * @name Flow control.
     */
    //@{

    /**
     * Compute the total size of images available since the last use().
     *
     * Please note that the size is computed, and therefore it is
     * somehow slow.  Never use this method to check if all images
     * have been loaded; use hasNext() instead.
     */
    int computeSize() const;

    /**
     * After calling rewind the next image loaded by a single image load()
     * function will be the first one in the list.
     */
    void rewind();

    /**
     * Returns true if there are more images to be loaded on the list.
     */
    bool hasNext() const;

    /**
     * Skips the current image filename on the list and without actually
     * loading it. For example if you call rewind() and then skip() then the
     * next load() will load the second image file on the list.
     *
     * \warning Note that analogous to load it is your responsibility to check
     * whether the list hasNext().
     */
    void skip();

    //@}

    /**
     * @name Set the source for the filenames
     */
    //@{
    
    enum {
      Recurse = true /**< Symbol to facilitate code reading */
    };
    

    /**
     * Use all image filenames as specified in the parameters that can be found
     * in \p dirname.  Optionally, you can specify if a recursive search for
     * files have to be done in all subdirectories within the given directory.
     *
     * \code
     * loadImageList loader;
     * // this will force descending recursively in all subdirectories 
     * loader.useDirectory("/home/jsmith",lti::loadImageList::Recurse);
     * \endcode
     *
     * @param dirname name of directory where the images are found
     * @param recurse optional agument to indicate if searching for images
     *                in subdirectories have to be done recursively.  You
     *                can use the constant lti::loadImageList::Recurse to
     *                make the code more readable.
     */
    bool useDirectory(const std::string& dirname, const bool recurse = false);

    /**
     * Use all image filenames as specified in the parameters that are present
     * in \p filename. 
     *
     * \b Note that lines starting with a semi-colon are regarded as comment
     * lines and that the image filenames contained in \p filename are \b NOT
     * sorted alphabetically.
     */
    bool useFileList(const std::string& filename);

    /**
     * Use the valid image filenames contained in the range \p first to \p
     * last: [first; last). The iterators must be valid input iterators and
     * dereference to std::string.
     * 
     * \b Note that the image filenames contained in the range are \b NOT
     * sorted alphabetically.
     */
    template <typename InputIterator>
    bool useRange(InputIterator first, InputIterator last); 

    //@}

    /**
     * @name Load single image
     *
     * Get the next image/channel etc. from the given list of filenames.
     */
    //@{

    /**
     * Returns the filename for the image that will be loaded by the \b next
     * call to load().
     *
     * \b Note that if parameters::continueOnError is true the filename
     * returned by this member function might actually not be loaded by the
     * next load() call in case an error occurs. Consider using
     * getLastLoadedFilename() instead.
     */
    const std::string& getNextFilename() const;

    /**
     * Returns the name of the image file that was loaded by the last call to
     * one of the single image load() member functions. This member function
     * is a bit slower than getNextFilename() but is recommended of
     * parameters::continueOnError is true.
     */
    const std::string& getLastLoadedFilename() const;
    
    /**
     * Load file as a color image.
     *
     * @param img the RGB image
     */
    bool load(image& img);

    /**
     * Load a channel8 as a label mask with its corresponding palette.
     *
     * If the file containes a color image, then the functor has to quantize
     * the colors and returnes the quantized table.
     *
     * @param chnl index channel
     * @param pal corresponding palette
     */
    bool load(matrix<ubyte>& chnl,
              palette& pal);

    /**
     * Load the contents as a gray channel.
     *
     * Default implementation just discards the palette of 
     * load(matrix<ubyte>&, palette&)
     *
     * @param chnl 8-bit gray channel8
     */
    bool load(matrix<ubyte>& chnl);

    /**
     * Load a floating point channel.  Not many file formats support floating
     * point channels, so the default implementation just casts a channel8 into
     * the float.
     *
     * @param chnl floating point gray channel
     */
    bool load(matrix<float>& chnl);

    /**
     * Load the contents as a integer mask
     *
     * Default implementation returns the casting of the channel8 related
     * method.
     *
     * @param chnl integer index channel
     * @param pal corresponding palette
     */
    bool load(matrix<int32>& chnl,
              palette& pal);

    /**
     * Load the contents as a integer mask
     *
     * Default implementation discards the palette of the other matrix<int32>
     * method.
     *
     * @param chnl integer mask
     */
    bool load(matrix<int32>& chnl);
    //@}

    /**
     * @name Load all images/channels etc.
     *
     * Load all images at once from the given list of filenames.
     */
    //@{

    /**
     * Puts the filenames of all files into \p filenames in the same order
     * that they are in the load() member functions for the complete set.
     */
    void getFilenames(std::vector<std::string>& filenames) const;
    
    /**
     * Load all files as a color image.
     *
     * @param img the RGB image
     */
    bool load(std::vector<image*>& img);

    /**
     * Load a channel8 as a label mask with its corresponding palette.
     *
     * If the file containes a color image, then the functor has to quantize
     * the colors and returnes the quantized table.
     *
     * @param chnl index channel
     * @param pal corresponding palette
     */
    bool load(std::vector<matrix<ubyte>*>& chnl,
	      std::vector<palette*>& pal);

    /**
     * Load the contents as a gray channel.
     *
     * Default implementation just discards the palette of 
     * load(matrix<ubyte>&, palette&)
     *
     * @param chnl 8-bit gray channel8
     */
    bool load(std::vector<matrix<ubyte>*>& chnl);

    /**
     * Load a floating point channel.  Not many file formats support floating
     * point channels, so the default implementation just casts a channel8 into
     * the float.
     *
     * @param chnl floating point gray channel
     */
    bool load(std::vector<matrix<float>*>& chnl);

    /**
     * Load the contents as a integer mask
     *
     * Default implementation returns the casting of the channel8 related
     * method.
     *
     * @param chnl integer index channel
     * @param pal corresponding palette
     */
    bool load(std::vector<matrix<int32>*>& chnl,
	      std::vector<palette*>& pal);

    /**
     * Load the contents as a integer mask
     *
     * Default implementation discards the palette of the other matrix<int32>
     * method.
     *
     * @param chnl integer mask
     */
    bool load(std::vector<matrix<int32>*>& chnl);
    //@}

  protected:
    /**
     * IOImage object used for actually loading the images and for checking
     * valid filetypes when "ALL" is selected
     */
    ioImage loader_;

    /**
     * This list stores all valid image filenames to be loaded.
     */
    std::list<std::string> filenames_;

    /**
     * Iterator pointing to the next filename to load
     */
    std::list<std::string>::const_iterator it_;

    /**
     * If true all filetypes supported by lti::IOImage should be considered.
     */
    bool loadAllImageTypes_;

    /**
     * Correct file extension if only one type is to be considered.
     */
    std::string fileExt_;

    /**
     * Checks whether a filename has the extension specified in the parameters
     */
    bool hasValidFileExtension(const std::string& filename) const;

    /**
     * @name Helper functions for loading
     */
    //@{

    /**
     * Load an image of type T
     */
    template <class T>
    bool loadHelp(T& dest);

    /**
     * Load a channel of type T and a corresponding palette
     */
    template <class T>
    bool loadHelp(T& dest, palette& pal);

    /**
     * Load all images of type T
     */
    template <class T>
    bool loadHelp(std::vector<T*>& dest);

    /**
     * Load all channels of type T and corresponding palettes
     */
    template <class T>
    bool loadHelp(std::vector<T*>& dest, std::vector<palette*>& pal);

    /**
     * Read all image filenames as specified in the parameters that can be
     * found in \p dirname.
     *
     * This method is called recursively by itself and is used by
     * useDirectory
     *
     * The main difference is that useDirectory clear the filename_ list, but
     * this method does not.
     *
     * @param dirname name of directory where the images are found
     * @param recurse optional agument to indicate if searching for images
     *                 in subdirectories have to be done recursively.
     * @param err String reporting the error occurred.
     */
    bool readDirectory(const std::string& dirname, 
                       const bool recurse,
                       std::string& err);

    //@}

  private:

    /**
     * Dummy string needed to return empty string in getLastLoadedFilename()
     */
    static const std::string emptyString_;
  };
}

#include "ltiLoadImageList_template.h"

#endif

