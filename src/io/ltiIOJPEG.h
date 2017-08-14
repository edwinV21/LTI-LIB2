/*
 * Copyright (C) 1998-2006
 * Lehrstuhl fuer Technische Informatik, RWTH-Aachen, Germany
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
 * \file   ltiIOJPEG.h
 *         Contains the lti::ioJPEG class to deal with .jpeg files.
 * \author Benjamin Winkler
 * \author Pablo Alvarado
 * \date   27.10.1999 (LTI-Lib 1)
 * \date   16.01.2006 (LTI-Lib 2)
 *
 * revisions ..: $Id: ltiIOJPEG.h,v 1.2 2006-08-28 08:08:12 doerfler Exp $
 */


#ifndef _LTI_IO_JPEG_H_
#define _LTI_IO_JPEG_H_

#include "ltiConfig.h"

#if defined HAVE_LIBJPEG || defined HAVE_LOCALJPEG

#include "ltiIOImageInterface.h"
#include <fstream>

#ifdef HAVE_LIBJPEG
#include <cstdio>
#endif

namespace lti {

  /**
   * Class to read and write images store in the Joint Picture Expert Group 
   * file format (JPEG).
   *
   * It is NOT thread save, this means, the SAME instance can not be used
   * from different threads or processes at the same time.  If this occurs
   * an unpredictible behaviour must be expected!.
   * If this is required, just use a different instance of the functor
   * for each thread, or use mutexes to avoid parallel accesses.
   *
   * Example:
   *
   * \code
   *
   * lti::image anImg;                      // an image
   * ...
   * lti::ioJPEG loader;                     // the load/save class
   * loader.load("~/tmp/theFile.jpeg",img); // load the image.
   * \endcode
   *
   * @exception BadJpegStream is thrown when the Stream is corrupted.
   *
   * @see ioImage
   *
   *
   * There are two implementations of this functor.
   *
   * - The first one uses the C++ JPEG interface from Miano (at
   *   Colosseum Builders C++ Image Library), for which several files
   *   are required.  These are published under another licence and
   *   therefore are not included in the default LTI-Lib.  You can use
   *   them in non-commercial applications and get them from
   *   ltilib.sourceforge.net.  See file ltilib/src/io/jpeg/00readme.txt
   *   for more information.
   *   If the file ltiJPEGFunctor.cpp and the directory jpeg/ are found
   *   in ltilib/src/io, and HAVE_LIBJPEG is not defined in basis/ltiConfig.h,
   *   this will be the implementation used.
   *
   * - The second implementation uses the standard JPEG Library, which is
   *   usually installed in all Linux distributions, but not for Windows.
   *   The implementation is included in the standard LTI-Lib, (file
   *   is called ltiJPEGLibFunctor.cpp).
   *   The autoconf script (./configure) will automatically detect if you
   *   have the libjpeg installed, and activate this implementation if
   *   possible. (see http://www.ijg.org/files/ and
   *   http://freealter.org/doc_distrib/libgr-2.0.13/jpeg/Libjpeg.html)
   *
   * We recomend to use the libjpeg, it is more robust against not 100%
   * correct JPEG-files, and it is faster.  Mianos' implementation is mainly
   * for windows users who do not want to install the jpeglib.
   *
   */
  class ioJPEG : public ioImageInterface {
  public:
    /**
     * Parameter class of ioJPEG
     */
    class parameters : public ioImageInterface::parameters {
      public:
      /**
       * Default constructor
       */
      parameters();

      /**
       * copy constructor
       */
      parameters(const parameters& other);

      /**
       * Copy member
       */
      parameters& copy(const parameters& other);

      /**
       * Copy operator
       */
      parameters& operator=(const parameters& other);

      /**
       * Returns the name of this class.
       */
      virtual const std::string& name() const;
      
      /**
       * Returns a pointer to a clone of the parameters.
       */
      virtual parameters* clone() const;

      /**
       * Returns a pointer to a new instance of the parameters
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
       * Write the parameters in the given ioHandler
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
       * Determines the compression of the image when saved.
       * Valid values range from 1 to 100. Default value is 75.
       */
      int quality;

      /**
       * Save in progressive or sequential mode.
       * Default is sequential mode (false).
       */
      bool progressive;

      /**
       * Save a comment chunk in the file. Default is an empty string.
       */
      std::string comment;

      /**
       * Number of rows between restart markers (0 = no restart markers)
       * when saving.
       * Default is 0.
       */
      int rowsPerRestart;
    };

    /**
     * Default constructor
     */
    ioJPEG();

    /**
     * Constructor with parameters
     */
    ioJPEG(const parameters& other);

    /**
     * Copy constructor
     */
    ioJPEG(const ioJPEG& other);

    /**
     * Destructor
     */
    ~ioJPEG();

    /**
     * Returns current parameters
     */
    const parameters& getParameters() const;

    /**
     * Returns the name of this class
     */
    virtual const std::string& name() const;

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual ioJPEG* clone() const;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual ioJPEG* newInstance() const;

    /**
     * Load true-color image.
     *
     * @param filename name of the file to read
     * @param theImage variable where the image is stored
     */
    virtual bool load(const std::string& filename, image& theImage);

    /**
     * Load indexed image 
     */
    virtual bool load(const std::string& filename,
                            matrix<ubyte>& theImage,
                            palette& colors);

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
     * @name Additional methods not in ioImageInterface
     */
    //@{
    /**
     * Load image with image type
     *
     * @param filename name of the file to read
     * @param theImage variable where the image is stored
     * @param isGray will be set with true if the image contains only gray
     *               values.
     */
    bool load(const std::string& filename, image& theImage, bool& isGray);
    //@}

    /**
     * Returns the essential image information.
     * This function will search the file for all chunks with
     * essential picture information.  Scanning stops before the first
     * StartOfScan-Block. That is, all comments that follow the image
     * data are not read!
     *
     * @param filename name of the file to check
     * @param info contains the header data
     * @return true if file and header are ok
     */
    bool checkHeader(const std::string& filename, 
                     headerInformation& info);

  private:
#ifdef HAVE_LIBJPEG
    /**
     * method for loading a JPEG image from an already opened
     * file.
     */
    bool load(FILE* file,image& theImage,bool& isGray);
#endif
  public:

    /**
     * Save an image in JPEG format.
     *
     * @param filename name of the file to save
     * @param theImage the image to be saved
     */
    virtual bool save (const std::string& filename,
                       const image &theImage);

    /**
     * Save an "indexed" image contained in the given channel8 (indices) and
     * the given color palette
     */
    virtual bool save(const std::string& filename,
                      const matrix<ubyte>& theChannel,
                      const lti::palette& colors);


    /**
     * shortcut for load JPEG
     *
     * @param filename name of the file to save
     * @param theChannel the channel to be saved
     */
    virtual bool save (const std::string& filename,
                       const matrix<ubyte>& theChannel);

    /**
     * Save a channel as a gray valued JPEG.
     *
     * @param filename name of the file to save
     * @param theChannel the channel to be saved
     */
    bool save (const std::string& filename,const matrix<float>& theChannel);

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
    

  private:
#ifdef HAVE_LIBJPEG

    /**
     * method for saving a JPEG image to an already opened
     * file.
     */
    bool save(FILE* file,const image& theImage);

#endif

  };
}

#endif
#endif

