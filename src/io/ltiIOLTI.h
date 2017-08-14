/*
 * Copyright (C) 2008
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
 * \file   ltiIOLTI.h
 *         Contains the class lti::ioLTI,
 *         which provides IO access to LTI image files.
 * 
 * \author Pablo Alvarado
 * \author Jochen Wickel
 * \date   03.06.2003
 *
 * revisions ..: $Id: ltiIOLTI.h,v 1.2 2008-08-17 22:23:27 alvarado Exp $
 */


#ifndef _LTI_IO_LTI_H_
#define _LTI_IO_LTI_H_

#include "ltiIOImageInterface.h"
#include "ltiMatrix.h"
#include "ltiComplex.h"
#include "ltiDataCodec.h"
#include <fstream>

namespace lti {
  /** 
   * Class to load and save images and channels in the LTI format.
   *
   * The LTI format is a data file format used to store commonly used
   * matrices and images within the library, which are not supported
   * by other formats like PNG or BMP.  It is extensible in the sense
   * that the codecs used can be dynamically added (the codec name is
   * stored and restored by means of factories).
   *
   * You will usually employ this format when serializing floating
   * point channels or channel32 instances, as well as complex valued
   * matrices.
   *
   * @see lti::ioLTI::parameters
   * @see lti::ioLTI::header
   * 
   * This functor is at a very primitive development stage.  The
   * format will change in the future, mainly because at this time,
   * the plataform/compiler independency is rudimentary, i.e. maybe a
   * file stored on Linux won't be readable on Windows and vice-versa.
   * Floating point number format still need to be formally specified.
   *
   * It is although provided for those applications which need to store 
   * imatrices or channels, for which no other alternative is provided.
   *
   * You can use it the same way as the other image io classes.
   *
   * This class is NOT thread safe, in the sense that the same instance cannot
   * be used in different threads.  You can however load LTI files in paralell
   * if each thread uses its own instance of ioLTI (or if you protect your
   * instace with mutexes).
   *
   * @see ioPNG, ioJPEG, ioBMP
   *
   * @ingroup gIOImage
   */
  class ioLTI : public ioImageInterface {
  public:

    /**
     * The compression methods usable for compressing the data.
     * You should be aware that runlength compression makes only
     * sense with matrices of bytes. RunLength tries to find
     * runs of equal bytes, which basically never happens in 
     * matrices of elements that occupy more than one byte.
     */
    enum eCompressionType {
      None=0,              ///< use no compression
      RunLength,           ///< uses runlength compression
      Flate,               ///< uses zLib deflate compression
      Other                ///< other type of compression
    };

    /**
     * Parameter class of the ioLTI class
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
       * Codec to be used.
       *
       * You can provide any of the simple codecs (see the inherited
       * classes from lti::dataCodec).
       *
       * The most prominent used values here are:
       * - lti::identityCodec for no compression
       * - lti::runLengthCodec for RLC compression
       * - lti::flateCodec for ZIP compression (only available if the zlib was
       *                   found when configuring the LTI-Lib)
       *
       * Default value: "lti::identityCodec", i.e. do not compress.
       */
      std::string codec;
    };

    /**
     * LTI-Format file header
     *
     * This class is defined within the class lti::ioLTI. The header is
     * always stored uncompressed.
     */
    class header {
    public:
      /**
       * File type. For LTI-Types this must be 'LT'
       */
      const uint16 type;

      /**
       * Contents type:
       * -  0 : undefined
       * - 'b': ubyte channel
       * - 'i': int channel
       * - 'f': float channel
       * - 'd': double channel
       * - 'c': rgbPixel image
       * - 'w': channel of complex<float>
       * - 'z': channel of complex<double>
       */
      ubyte contents;

      /**
       * Compression type:
       * -  0 : no compression
       * -  1 : runlength compression 
       * -  2 : flate compression (only with zlib)
       * -  3 : other, specified through the string in codec
       */
      ubyte compression;

      /**
       * File size in byte (excluding the header, so only the data section
       * is counted).
       */
      uint32 size;

      /**
       * Must be 0
       * This can be used in the future.
       */
      const uint32 reserved1;

      /**
       * Must be 0
       * This can be used in the future.
       */
      const uint32 reserved2;

      /**
       * Number of rows
       */
      uint32 rows;

      /**
       * Number of columns
       */
      uint32 columns;

      /**
       * Name of codec being used for other compression.
       *
       * On the file this will be stored as a zero terminated string, if and
       * only if the \c compression attribute is greater or equal to 3.
       *
       * This is necessary to keep compatibility with the LTI-Lib-1 format, 
       * but allowing some extensability.
       */
      std::string codec;

      /**
       * Default constructor
       */
      header();

      /**
       * Reads header from the stream "in".
       * @return false if stream had an invalid header.
       */
      bool read(std::ifstream& in);

      /**
       * Writes header to the stream "out".
       * @return false if an error occurs.
       */
      bool write(std::ofstream& out) const;

      /**
       * Size of the header (in bytes)
       */
      int length() const;
    };

  public:
    /**
     * Default constructor
     */
    ioLTI();

    /**
     * Constructor with parameters
     */
    ioLTI(const parameters& par);

    /**
     * Destructor
     */
    virtual ~ioLTI();

    /**
     * Return current parameters.
     */
    const parameters& getParameters() const;

    /**
     * Returns the name of this class
     */
    virtual const std::string& name() const;

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual ioLTI* clone() const;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual ioLTI* newInstance() const;

    /**
     * Load file with LTI image
     *
     * @param filename name of the file to be readed
     * @param theImage variable where the image will to be stored
     */
    virtual bool load(const std::string& filename,image& theImage);

    /**
     * Load LTI containing a labeled mask with its corresponding palette
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
     * Load LTI containing a labeled mask with its corresponding palette
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
     * Load a LTI from an opened stream.
     *
     * @param file an already opened stream for reading binary data
     * @param theImage variable where the image will to be stored
     */
    virtual bool load(std::ifstream& file, image& theImage);

    /**
     * Load a LTI from an opened stream.
     *
     * @param file an already opened stream for reading binary data
     * @param the matrix variable where the image will to be stored
     */
    virtual bool load(std::ifstream& in, matrix<float>& theChannel);

    /**
     * Check the data of the bitmap header
     * @param filename name of the bitmap file to be tested
     * @param info contains the header data
     * @return true if file and header are ok
     */
    bool checkHeader(const std::string& filename,
                     headerInformation& info);

  private:
    /**
     * Loads a matrix from the given stream. 
     *
     * The data are written into theChannel, the meta-data are written
     * into theHeader.  These methods ignore all the parameters in this
     * objects.  The only reason they are not static is that they
     * modify the status string.
     */
    template<typename T>
    bool loadMatrix(const std::string& filename,
                    matrix<T>& theChannel,
                    header& theHeader) const;

    /**
     * Loads a matrix from the given stream. 
     *
     * The data are written into theChannel, the meta-data are written
     * into theHeader.  These methods ignore all the parameters in this
     * objects.  The only reason they are not static is that they
     * modify the status string.
     */
    template<typename T>
    bool loadMatrix(std::ifstream& in,
                    matrix<T>& theChannel,
                    header& theHeader) const;

    
    /**
     * Saves the given matrix to the given stream. The header is modified
     * with appropriate data derived from the matrix structure.
     */
    template<typename T>
    bool saveMatrix(const std::string& filename, 
                    const matrix<T>& theChannel,
                    header& theHeader, 
                    const std::string& codecName) const;


    /**
     * Saves the given matrix to the given stream. The header is modified
     * with appropriate data derived from the matrix structure.
     */
    template<typename T>
    bool saveMatrix(std::ofstream& out,
                    const matrix<T>& theChannel,
                    header& theHeader, 
                    const std::string& codecName) const;

    
    /**
     * Loads the matrix data from the given stream. The meta-data
     * must be passed in the given header.
     */
    template<typename T>
    bool loadBody(std::ifstream& in, matrix<T>& theChannel,
                  const header& theHeader,
                  dataCodec* codec) const;

    /**
     * Methods used to return the code type
     */
    //@{
    inline char getTypeCode(const ubyte&) const     {return 'b';};
    inline char getTypeCode(const rgbaPixel&) const {return 'c';};
    inline char getTypeCode(const int32&) const     {return 'i';};
    inline char getTypeCode(const float&) const     {return 'f';};
    inline char getTypeCode(const double&) const    {return 'd';};
    inline char getTypeCode(const fcomplex&) const  {return 'w';};
    inline char getTypeCode(const dcomplex&) const  {return 'z';};
    //@}

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
     * Last read header
     */
    header theHeader_;
  };

  /**
   * Read a eExtremaType
   *
   * @ingroup gStorable
   */
  bool read(ioHandler& handler,ioLTI::eCompressionType& data);

  /**
   * Write a eExtremaType
   *
   * @ingroup gStorable
   */
  bool write(ioHandler& handler,const ioLTI::eCompressionType& data);
 


}  //namespace lti

#endif

