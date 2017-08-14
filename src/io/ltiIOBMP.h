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
 * \file   ltiIOBMP.h
 *         Contains the lti::ioBMP class to deal with .bmp files.
 * \author Pablo Alvarado
 * \date   19.04.1999 (LTI-Lib 1)
 * \date   14.01.2006 (LTI-Lib 2)
 *
 * revisions ..: $Id: ltiIOBMP.h,v 1.5 2008-05-29 02:52:11 alvarado Exp $
 */


#ifndef _LTI_IO_BMP_H_
#define _LTI_IO_BMP_H_

#include "ltiIOImageInterface.h"
#include "ltiImage.h"
#include <fstream>

namespace lti {
  /** 
   * Class to load and save images and channels in the BMP format.
   *
   * You can use it the same way as the other image io classes.
   *
   * See a detailed description of the BMP format at 
   * http://www.fileformat.info/format/bmp/egff.htm
   *
   * Some limitations need to be worked out:
   * - There is no support for "negative" values in the height and width
   *   which are in principle supported by the format.
   * - BMP Format 4.0 or NT are not supported yet.
   * - This functor cannot cope with 16 or 32 bits per pixel files yet.
   *
   * This class is NOT thread safe, in the sense that the same instance cannot
   * be used in different threads.  You can however load BMP files in paralell
   * if each thread uses its own instance of ioBMP (or if you protect your
   * instace with mutexes).
   *
   * @see ioPNG, ioJPEG
   *
   * @ingroup gIOImage
   */
  class ioBMP : public ioImageInterface {
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
       * Copy method.
       */
      parameters& copy(const parameters& other);

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
       * write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool write(ioHandler& handler,const bool complete=true) const;

      /**
       * write the parameters in the given ioHandler
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
       * Activate compression of the data.
       *
       * RLE4 or RLE8 will be used if the number of bits are 4 or 8.
       *
       * Default value: true.
       */
      bool compression;

      /**
       * Bits per pixel.
       *
       * Default value: 24
       */
      int bitsPerPixel;
    };

  protected:
    /**
     * Class header implements BITMAPFILEHEADER.
     *
     * This class is defined within the class lti::ioBMP.
     */
    class header {
    public:
      /**
       * file type. For Bitmaps this must be 'BM'
       */
      const uint16 type ;

      /**
       * file size in byte
       */
      uint32 size ;

      /**
       * must be 0
       */
      const uint16 reserved1 ;

      /**
       * must be 0
       */
      const uint16 reserved2 ;

      /**
       * offset (in bytes) from this header to the bitmap pixels
       */
      uint32 offsetPixels ;

      /**
       * default constructor
       */
      header();

      /**
       * reads header from the stream "in".
       * @return false if stream had an invalid header.
       */
      bool read(std::ifstream& in);

      /**
       * writes header to the stream "out".
       * @return false if an error occurs.
       */
      bool write(std::ofstream& out) const;

      /**
       * size of the header (in bytes)
       */
      inline int length() const {return 14;};
    };

    /**
     * Class infoHeader implements BITMAPINFOHEADER
     *
     * This class is defined within the class lti::ioBMP.
     */
    class infoHeader {
    public:
      /**
       * constructor
       */
      infoHeader();

      /**
       * size
       */
      mutable uint32 size;

      /**
       * width
       */
      uint32 width;

      /**
       * height
       */
      uint32 height;

      /**
       * planes
       */
      uint16 planes ;

      /**
       * bitcount
       */
      uint16 bitCount ;

      /**
       * compression
       */
      uint32 compression;

      /**
       * sizeImage
       */
      uint32 sizeImage;

      /**
       * xPixPerMeter
       */
      uint32 xPixPerMeter;

      /**
       * yPixPerMeter
       */
      uint32 yPixPerMeter;

      /**
       * colorsUsed
       */
      uint32 colorsUsed;

      /**
       * colorsImportant
       */
      uint32 colorsImportant;

      /**
       * read
       */
      bool read(std::ifstream& in);

      /**
       * write
       */
      bool write(std::ofstream& out) const;

      /**
       * size of the info header (in bytes)
       */
      inline int length() const {return 40;};
    };

    /** 
     * Color Palette found in a BMP file.
     *
     * This class is defined within the class lti::ioBMP.
     */
    class palette : public vector<rgbaPixel> {
    public:
      /**
       * default constructor
       * @param entries number of entries in the palette
       */
      palette(int entries = 0);

      /**
       * read the palette from the in stream
       * @param in input stream
       * @return true if everything is ok, false otherwise
       */
      bool read(std::ifstream& in);


      /**
       * write palette to the out stream
       * @param out output stream
       * @return true if everything is ok, false otherwise
       */
      bool write(std::ofstream& out) const;

      using vector<rgbaPixel>::read;
      using vector<rgbaPixel>::write;

    };

  public:
    /**
     * Default constructor
     */
    ioBMP();

    /**
     * Constructor with parameters
     */
    ioBMP(const parameters& par);

    /**
     * destructor
     */
    virtual ~ioBMP();

    /**
     * returns current parameters.
     */
    const parameters& getParameters() const;

    /**
     * Returns the name of this class
     */
    virtual const std::string& name() const;

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual ioBMP* clone() const;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual ioBMP* newInstance() const;

    /**
     * Load file with BMP image
     *
     * @param filename name of the file to be readed
     * @param theImage variable where the image will to be stored
     */
    virtual bool load(const std::string& filename,image& theImage);

    /**
     * Load BMP containing a labeled mask with its corresponding palette
     * entries.
     *
     * Use this method if you know that the file contains a gray valued image
     * or an indexed color image.  If you try to load a 24-bit image with this
     * method, then "false" will be returned.
     *
     * @param filename name of the file to be readed
     * @param theChannel variable where the image will be stored
     * @param colors the palette used will be stored here
     */
    virtual bool load(const std::string& filename,
                      matrix<ubyte>& theChannel,
                      lti::palette& colors);

    /**
     * Load gray valued channel.
     *
     * Use this method if you know that the file contains a gray valued image.
     *
     * @param filename name of the file to be readed
     * @param theChannel variable where the image will be stored
     */
    virtual bool load(const std::string& filename,
                      matrix<ubyte>& theChannel);

    /**
     * Load gray valued channel.
     *
     * Use this method if you know that the file contains a gray valued image.
     *
     * @param filename name of the file to be readed
     * @param theChannel variable where the image will be stored
     */
    virtual bool load(const std::string& filename,
                      matrix<float>& theChannel);

    /**
     * Load BMP containing a labeled mask with its corresponding palette
     * entries.
     *
     * Use this method if you know that the file contains a gray valued image
     * or an indexed color image.  If you try to load a 24-bit image with this
     * method, then "false" will be returned.
     *
     * @param filename name of the file to be readed
     * @param theChannel variable where the image will be stored
     * @param colors the palette used will be stored here
     */
    virtual bool load(const std::string& filename,
                      matrix<int32>& theChannel,
                      lti::palette& colors);

    /**
     * Load gray valued channel.
     *
     * Use this method if you know that the file contains a gray valued image.
     *
     * @param filename name of the file to be readed
     * @param theChannel variable where the image will be stored
     */
    virtual bool load(const std::string& filename,
                      matrix<int32>& theChannel);

    /**
     * Load a BMP from an opened stream.
     *
     * @param file an already opened stream for reading binary data
     * @param theImage variable where the image will to be stored
     */
    virtual bool load(std::ifstream& file, image& theImage);

    /**
     * This method loads an image or a matrix<ubyte>, depending on the contents
     * of the file header.
     *
     * If the number of bits per pixel is 24, the image will be loaded,
     * if the number of bits is less or equal 8, the channel and palette
     * will be initialized.
     *
     * The wrong type will be resized to (0,0)
     * @param file     and already opened stream for reading binary data
     * @param theImage if the file contains a 24 bit color image, this
     *                 parameter will be resized and initialized with
     *                 the contents of the file
     * @param theChannel if the file contains a 8 bit or less indexed image,
     *                 this parameter and the palette will be initialized
     *                 with the contents of the file.
     * @param colors   the color palette for the indexed image.
     * @return the number of bits per pixel of the loaded image or 0 if an
     *         error occured.  The valid values are 1, 4, 8, or 24
     */
    virtual int load(std::ifstream& file,
                     image& theImage,
                     matrix<ubyte>& theChannel,
                     lti::palette& colors);

    /**
     * This method loads an image or a matrix<ubyte>, depending on the contents
     * of the file header.
     *
     * If the number of bits per pixel is 24, the image will be loaded,
     * if the number of bits is less or equal 8, the channel and palette
     * will be initialized.
     *
     * The wrong type will be resized to (0,0)
     * @param filename name of the file to be readed
     * @param theImage if the file contains a 24 bit color image, this
     *                 parameter will be resized and initialized with
     *                 the contents of the file
     * @param theChannel if the file contains a 8 bit or less indexed image,
     *                 this parameter and the palette will be initialized
     *                 with the contents of the file.
     * @param colors   the color palette for the indexed image.
     * @return the number of bits per pixel of the loaded image or 0 if an
     *         error occured.  The valid values are 1, 4, 8, or 24
     */
    int load(const std::string& filename,
             image& theImage,
             matrix<ubyte>& theChannel,
             lti::palette& colors);


    /**
     * check the data of the bitmap header
     * @param filename name of the bitmap file to be tested
     * @param info contains the header data
     * @return true if file and header are ok
     */
    bool checkHeader(const std::string& filename,
                     headerInformation& info);

  private:
    /**
     * Load a 1-bit channel (2 colors).
     *
     * theChannel has to have the proper dimensions before calling this method.
     */
    bool load1bit(std::ifstream& in,matrix<ubyte>& theChannel);

    /**
     * Load a 4-bit channel (16 colors)
     *
     * theChannel has to have the proper dimensions before calling this method.
     */
    bool load4bit(std::ifstream& in,matrix<ubyte>& theChannel);

    /**
     * Load an 8-bit channel (256 colors)
     *
     * theChannel has to have the proper dimensions before calling this method.
     */
    bool load8bit(std::ifstream& in,matrix<ubyte>& theChannel);

    /**
     * Load a 24-bit image (2^24 colors)
     *
     * theChannel has to have the proper dimensions before calling this method.
     */
    bool load24bit(std::ifstream& in,image& theImage);

    /**
     * Last read header
     */
    header theHeader_;
    
    /**
     * Last read info header
     */
    infoHeader theInfoHeader_;

    /**
     * Last read palette
     */
    palette thePalette_;

  public:
    /**
     * Save an image as a 24 bit RGB bitmap image.
     */
    virtual bool save(const std::string& filename,
                      const image& theImage);

    /**
     * Save a channel8 as an 8 bit RGB bitmap image.
     *
     * @param filename name of the destination file
     * @param theChannel the channel to be save
     * @param colors the palette to be used.
     */
    virtual bool save(const std::string& filename,
                      const matrix<ubyte>& theChannel,
                      const lti::palette& colors);

    /**
     * Save a channel8 as an 8 bit RGB bitmap image.
     *
     * @param filename name of the destination file
     * @param theChannel the channel to be save
     */
    virtual bool save(const std::string& filename,
                      const matrix<ubyte>& theChannel);

    /**
     * Save a floating point channel as an 8 bit RGB bitmap image.
     *
     * The values of the channel must be between 0.0f and 1.0f!
     *
     * @param filename name of the destination file
     * @param theChannel the channel to be saved
     */
    virtual bool save(const std::string& filename,
                      const matrix<float>& theChannel);

    /**
     * Save the contents as a integer mask.
     *
     * Default implementation returns the casting of the channel8 related
     * method.
     */
    virtual bool save(const std::string& filename,
                      const matrix<int32>& chnl,
                      const lti::palette& pal);

    /**
     * Save the contents as a integer mask.
     *
     * Default implementation discards the palette of the other matrix<int32>
     * method.
     */
    virtual bool save(const std::string& filename,
                      const matrix<int32>& chnl);

  private:
    /**
     * Save 1 bit channel (2 colors)
     */
    bool save1bit(std::ofstream& out,const matrix<ubyte>& theChannel);
    /**
     * Save 4 bit channel (16 colors)
     */
    bool save4bit(std::ofstream& out,const matrix<ubyte>& theChannel);
    /**
     * Save 8 bit channel (256 colors)
     */
    bool save8bit(std::ofstream& out,const matrix<ubyte>& theChannel);
    /**
     * Save true color image
     */
    bool save24bit(std::ofstream& out,const image& theImage);
  };
}  //namespace lti

#endif

