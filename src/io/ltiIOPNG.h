/*
 * Copyright (C) 1998-2006
 * Lehrstuhl fuer Technische Informatik, RWTH-Aachen, Germany
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
 * \file   ltiIOPNG.h
 *         Contains the lti::ioPNG class to deal with .png files.
 * \author Stefan Syberichs
 * \author Pablo Alvarado
 * \date   19.04.1999
 *
 * revisions ..: $Id: ltiIOPNG.h,v 1.7 2007-10-07 04:21:25 alvarado Exp $
 */

#ifndef _LTI_IO_PNG_H_
#define _LTI_IO_PNG_H_

#include "ltiConfig.h"

#if defined HAVE_LIBPNG || defined HAVE_LOCALPNG

#include "ltiIOImageInterface.h"
#include <fstream>

#ifdef HAVE_LIBPNG
#include <cstdio>
#else
#include "ltipngdecoder.h"
#include "ltipngencoder.h"
#endif

namespace lti {
  /**
   * Class to read and write files with images and channels in PNG format.
   *
   * It is NOT thread save, this means, the SAME instance can not be used
   * from different threads or processes at the same time.  If this occurs
   * an unpredictible behaviour must be expected!.
   * If this is required, just use a different instance of the functor
   * for each thread, or use mutexes to avoid parallel accesses.
   *
   * There are two implementations of this functor.
   * - The first one uses the C++ PNG interface from Miano (at
   *   Colosseum Builders C++ Image Library), for which several files
   *   are required.  These are published under another licence and
   *   therefore are not included in the default LTI-Lib.  You can use
   *   them in non-commercial applications and get them from
   *   ltilib.sourceforge.net.  See file ltilib/src/io/png/00readme.txt
   *   for more information.
   *   If the file ltiPNGFunctor.cpp and the directory png/ are found
   *   in ltilib/src/io, and HAVE_LIBPNG is not defined in basis/ltiConfig.h
   *   this will be the implementation used.
   *
   * - The second implementation uses the standard PNG Library, which is
   *   usually installed in all Linux distributions, but not for Windows.
   *   The implementation is included in the standard LTI-Lib, (file
   *   is called ltiPNGLibFunctor.cpp).
   *   The autoconf script (./configure) will automatically detect if you
   *   have the libpng installed, and activate this implementation if
   *   possible.
   *
   * We recomend to use the libpng, it is more robust against not 100%
   * correct PNG-files, and it is faster.  Mianos' implementation is mainly
   * for windows users that do not want to install the pnglib.
   *
   * \section inpng Reading PNG files
   * 
   * To read PNG files, you can use following code
   *
   * Example:
   *
   * \code
   *
   * lti::image anImg;                      // an image
   * lti::ioPNG loader;                     // the load functor
   * loader.load("~/tmp/theFile.png",img); // load the image.
   * \endcode
   *
   * \section outpng Saving PNG files
   *
   * To save a Protable Network graphic (PNG) file just initialize the
   * parameters and call the save member.
   *
   * Example:
   *
   * \code
   *
   * lti::image anImg;                      // an image
   * ... [ initialize you image data here ]
   * lti::ioPNG saveImg;                    // the save functor
   * saveImg.save("~/tmp/theFile.png",img); // save the image.
   * \endcode
   *
   * @exception BadPngStream is thrown when the Stream is corrupted.
   *
   * @ingroup gIOImage
   */
  class ioPNG : public ioImageInterface {
  public:
    /**
     * Parameter class of the ioBMP class
     */
    class parameters : public ioImageInterface::parameters {
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
       * @param handler The ioHandler to be used
       * @param complete If true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool write(ioHandler& handler,const bool complete=true) const;

      /**
       * Write the parameters in the given ioHandler
       * @param handler The ioHandler to be used
       * @param complete If true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool read(ioHandler& handler,const bool complete=true);


      // ------------------------------------------------
      // the parameters
      // ------------------------------------------------

      /**
       * Bits per pixel
       *
       * Default value: 24
       */
      int bitsPerPixel;

      /**
       * True if alpha channel is to be considered. If this is true,
       * bitsPerPixel \b must be 32!
       *
       * Default value: false
       */
      bool useAlphaChannel;
    };

    /**
     * Default constructor
     */
    ioPNG();

    /**
     * Constructor with parameters
     */
    ioPNG(const parameters& par);

    /**
     * Copy constructor
     */
    ioPNG(const ioPNG& other);

    /**
     * Destructor
     */
    virtual ~ioPNG();

    /**
     * Returns current parameters.
     */
    const parameters& getParameters() const;

    /**
     * Returns the name of this class
     */
    virtual const std::string& name() const;

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual ioPNG* clone() const;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual ioPNG* newInstance() const;

    /**
     * Load true-color image
     */
    virtual bool load(const std::string& filename,image& theImage);

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
     * Check the data of the PNG header
     * @param filename name of the PNG file to be tested
     * @param info header information.
     * @return true if file is ok
     */
    virtual bool checkHeader(const std::string& filename,
                             headerInformation& info);



    /**
     * This will save an image as a 24 bit RGB bitmap image
     */
    bool save(const std::string& filename,
              const image& theImage);

    /**
     * Save an "indexed" image contained in the given channel8 (indices) and
     * the given color palette
     */
    bool save(const std::string& filename,
              const matrix<ubyte>& theChannel,
              const lti::palette& colors);

    /**
     * Save an gray scale channel
     */
    bool save(const std::string& filename,
              const matrix<ubyte>& theChannel);


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
    



  private:
#ifdef HAVE_LIBPNG
    /**
     * method for loading a PNG image from an already opened
     * file.
     */
    bool load(FILE* file,image& theImage);

#endif

  };

}  //namespace lti

#endif
#endif

