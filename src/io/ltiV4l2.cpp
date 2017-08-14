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
 * \file   ltiV4l2.cpp
 *         Contains the class lti::v4l2, which provide access to webcams and
 *         other devices that support the Video4Linux2 API.
 * 
 * \author Pablo Alvarado
 * \date   05.10.2008
 *
 * revisions ..: $Id: ltiV4l2.cpp,v 1.10 2013-05-04 22:56:58 alvarado Exp $
 */

#include "ltiHardwareConfig.h"
#ifdef _USE_V4L2
#ifdef __linux__

#include "ltiV4l2.h"
#include "libv4l2.h"

#include "ltiTypes.h"
#include "ltiMath.h"
#include "ltiRound.h"
#include "ltiPassiveWait.h"

#include "ltiBayerDemosaicing.h"
#include "ltiFactory.h" // for bayerDemosaicing

#include <fcntl.h>
#include <sys/ioctl.h>

#include <sys/stat.h>
#include <sys/time.h>
#include <sys/mman.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <unistd.h>
#include <cerrno>

#undef _LTI_DEBUG
// #define _LTI_DEBUG 4
#include "ltiDebug.h"



namespace lti {

  // -----------------------------------------------------------------
  // v4l2 converters factory
  // -----------------------------------------------------------------

  // Private macro to inscribe all classes you want;
#define _LTI_V4L2_REGISTER_(NAME,CLASS) \
::lti::v4l2::factory::registration<CLASS> \
_LTI_V4L2_REGISTRATION_ ## NAME ## _(::lti::v4l2:: NAME)

  v4l2::factory::factory() {
  }

  v4l2::factory::factory(const factory&) {
  }
  
  v4l2::factory::~factory() {
  }

  v4l2::factory& v4l2::factory::getFactory() {
    static factory theInstance;
    return theInstance;
  }

  v4l2::convertBase* v4l2::factory::instance(const ePixelFormat pxFmt) {
    object_map_type::iterator it;
    it = objectMap_.find(pxFmt);
    if (it == objectMap_.end()) {
      return 0;
    }
    return (*it).second;
  }

  void v4l2::factory::registerObject(convertBase* obj,
                                     const ePixelFormat index) {
    // No object should be already registered. Otherwise there would be a 
    // potential memory leak if we do not free the object, but then again,
    // we don't know if it can or should be freed.
    assert(isNull(objectMap_[index]));
    objectMap_[index] = obj;    
  }

  v4l2::convertBase::~convertBase() {
  }

  /*
   * Default implementation of convert just casts the image to a
   * grey-valued image
   */
  bool v4l2::convertBase::convert(void* data,
                                  const unsigned int bsize,
                                  const bool turnAround,
                                  channel8& chnl) const {
    image img(chnl.size());
    if (convert(data,bsize,turnAround,img)) {
      chnl.castFrom(img);
      return true;
    }
    return false;
  }

  template<class I>
  bool v4l2::convertBase::turn(I& img) const {
    I tmp;
    tmp.allocate(img.size());
    typename I::iterator it = tmp.begin();
    const typename I::iterator eit = tmp.end();
    typename I::const_iterator sit = img.inverseBegin();
    while (it != eit) {
      (*it) = (*sit);
      --sit;
      ++it;
    }
    tmp.detach(img);
    return true;
  }

  namespace internals {

    // -----------------------------------------------------------------
    //                             YUV420
    // -----------------------------------------------------------------

    /*
     * All format converters will reside on this namespace
     */
    class convertYUV420 : public ::lti::v4l2::convertBase {
      /*
       * Called when setting the parameters
       */
      virtual bool init(const v4l2::parameters& par);

      /*
       * Convert data buffer to an image
       */
      virtual bool convert(void* data,
                           const unsigned int bsize,
                           const bool turnAround,
                           image& img) const;

      /*
       * Convert data buffer to an channel8 (grey valued channel)
       */
      virtual bool convert(void* data,
                           const unsigned int bsize,
                           const bool turnAround,
                           channel8& img) const;

    protected:
      /*
       * Look-up table to accelerate computation of saturation
       */
      static const ubyte* lutSaturation_;
      
      /*
       * Look up tables to accelerate conversion Luv -> RGB
       */
      //@{
      static const int* lutUg_;
      static const int* lutUb_;
      static const int* lutVg_;
      static const int* lutVr_;
      //@}
      
      /*
       * Initialize the Look-Up-Tables
       */
      void initializeLUTs();

    };

    _LTI_V4L2_REGISTER_(YUV420,convertYUV420);
      
    // -----------------------------------------------------------------
    // v4l2 static members
    // -----------------------------------------------------------------

    // Static attributes
    const ubyte* convertYUV420::lutSaturation_ = 0;
      
    /*
     * Look up tables to accelerate conversion Luv -> RGB
     */
    //@{
    const int* convertYUV420::lutUg_ = 0;
    const int* convertYUV420::lutUb_ = 0;
    const int* convertYUV420::lutVg_ = 0;
    const int* convertYUV420::lutVr_ = 0;

    void convertYUV420::initializeLUTs() {
      if (lutSaturation_ == 0) {
        int i;
        /*
         * look up table to accelerate computation of saturation
         */
        static ubyte tmpSat[1024];
        for (i=-256;i<512;++i) {
          tmpSat[i & 0x3FF] = lti::within(i,0,255);
        }
        lutSaturation_ = tmpSat;
        
        /*
         * look up tables to accelerate conversion Luv -> RGB
         */
        static int lutUgTmp[256];
        static int lutUbTmp[256];
        static int lutVgTmp[256];
        static int lutVrTmp[256];      
        
        for (i=0;i<256;++i) {
          lutUgTmp[i] = (i-128)*88;
          lutUbTmp[i] = (i-128)*454;
          lutVgTmp[i] = (i-128)*183;
          lutVrTmp[i] = (i-128)*359;
        }
        
        lutUg_ = lutUgTmp;
        lutUb_ = lutUbTmp;
        lutVg_ = lutVgTmp;
        lutVr_ = lutVrTmp;
      }
    }

    /*
     * Called when setting the parameters
     */
    bool convertYUV420::init(const v4l2::parameters&) {
      initializeLUTs();
      return notNull(lutVr_);
    }

    /*
     * Convert data buffer to an image
     */
    bool convertYUV420::convert(void* data,
                                const unsigned int bsize,
                                const bool turnAround,
                                image& theImage) const {

      const ipoint& resolution(theImage.size());
      
      if (bsize<=0) {
        return false;
      }
      
      const ubyte* buffer = reinterpret_cast<const ubyte*>(data);
            
      const ubyte *Y = buffer;
      const ubyte *u = &buffer[resolution.x*resolution.y];
      const ubyte *v = &buffer[5*resolution.x*resolution.y/4];
      const ubyte *Yend;
      
      if (turnAround) {
        ubyte* pxptr = reinterpret_cast<ubyte*>(theImage.data());
        pxptr += (resolution.x*resolution.y*4);
        
        int yy,sy;
        
        const int usize = resolution.x/2;
        
        for (sy=0;sy<resolution.y;++sy) {
          Yend = Y+resolution.x;
          while (Y != Yend) {
            yy = (*Y)<<8;
            
            // dummy
            --pxptr;
            // red
            --pxptr;
            (*pxptr) = lutSaturation_[((yy+lutVr_[*v]) >> 8)&0x3FF];
            // green
            --pxptr;
            (*pxptr) = lutSaturation_[((yy-lutUg_[*u]-lutVg_[*v]) >> 8)&0x3FF];
            // blue
            --pxptr;
            (*pxptr) = lutSaturation_[((yy+lutUb_[*u]) >> 8) & 0x3FF];
            
            
            // next pixel doesn't change uv values
            ++Y;
            yy = (*Y)<<8;
            
            // dummy
            --pxptr;
            // red
            --pxptr;
            (*pxptr) = lutSaturation_[((yy+lutVr_[*v]) >> 8)&0x3FF];
            // green
            --pxptr;
            (*pxptr) = lutSaturation_[((yy-lutUg_[*u]-lutVg_[*v]) >> 8)&0x3FF];
            // blue
            --pxptr;
            (*pxptr) = lutSaturation_[((yy+lutUb_[*u]) >> 8)&0x3FF];
            
            ++Y;
            ++u;
            ++v;
          }
          if ((sy & 1) == 0) {
            u-=usize;
            v-=usize;
          }
        }
      
      } else {
        int yy,sy;

        ubyte* pxptr = reinterpret_cast<ubyte*>(theImage.data());      
        const int usize = resolution.x/2;
        
        for (sy=0;sy<resolution.y;++sy) {
          Yend = Y+resolution.x;
          while (Y != Yend) {
            yy = (*Y)<<8;
            
            // blue
            (*pxptr) = lutSaturation_[((yy+lutUb_[*u])>>8)&0x3FF];
            pxptr++;
            // green
            (*pxptr) = lutSaturation_[((yy-lutUg_[*u]-lutVg_[*v])>>8)&0x3FF];
            pxptr++;
            // red
            (*pxptr) = lutSaturation_[((yy+lutVr_[*v])>>8)&0x3FF];
            pxptr++;
            // dummy
            pxptr++;
            
            // next pixel doesn't change uv values
            ++Y;
            yy = (*Y)<<8;
            
            // blue
            (*pxptr) = lutSaturation_[((yy+lutUb_[*u])>>8)&0x3FF];
            pxptr++;
            // green
            (*pxptr) = lutSaturation_[((yy-lutUg_[*u]-lutVg_[*v])>>8)&0x3FF];
            pxptr++;
            // red
            (*pxptr) = lutSaturation_[((yy+lutVr_[*v])>>8)&0x3FF];
            pxptr++;
            // dummy
            pxptr++;
            
            ++Y;
            ++u;
            ++v;
          }
          if ((sy & 1) == 0) {
            u-=usize;
            v-=usize;
          }
        }
      }
      
      return true;
    }

    /*
     * Convert data buffer to an channel8 (grey valued channel)
     */
    bool convertYUV420::convert(void* data,
                                const unsigned int bsize,
                                const bool turnAround,
                                channel8& img) const {
      return ::lti::v4l2::convertBase::convert(data,bsize,turnAround,img);
    }

    // -----------------------------------------------------------------
    //                             YUYV
    // -----------------------------------------------------------------

    /*
     * All format converters will reside on this namespace
     */
    class convertYUYV : public convertYUV420 {

      /*
       * Convert data buffer to an image
       */
      virtual bool convert(void* data,
                           const unsigned int bsize,
                           const bool turnAround,
                           image& img) const;

      /*
       * Convert data buffer to an channel8 (grey valued channel)
       */
      virtual bool convert(void* data,
                           const unsigned int bsize,
                           const bool turnAround,
                           channel8& img) const;

    };

    _LTI_V4L2_REGISTER_(YUYV,convertYUYV);
      
    // -----------------------------------------------------------------
    // v4l2 static members
    // -----------------------------------------------------------------


    /*
     * Convert data buffer to an image
     */
    bool convertYUYV::convert(void* data,
                              const unsigned int bsize,
                              const bool turnAround,
                              image& theImage) const {
      
      const ipoint& resolution(theImage.size());
      
      if (bsize<=0) {
        return false;
      }
      
      const ubyte* buffer = reinterpret_cast<const ubyte*>(data);
            
      const ubyte *Y = buffer;
      const ubyte *u = &buffer[1];
      const ubyte *v = &buffer[3];
      const ubyte *Yend;

      const int rowBytes = resolution.x*2;
      
      if (turnAround) {
        ubyte* pxptr = reinterpret_cast<ubyte*>(theImage.data());
        pxptr += (resolution.x*resolution.y*4);
        
        int yy,sy;
        
        for (sy=0;sy<resolution.y;++sy) {
          Yend = Y+rowBytes;
          while (Y != Yend) {
            yy = (*Y)<<8;
            
            // dummy
            --pxptr;
            // red
            --pxptr;
            (*pxptr) = lutSaturation_[((yy+lutVr_[*v]) >> 8)&0x3FF];
            // green
            --pxptr;
            (*pxptr) = lutSaturation_[((yy-lutUg_[*u]-lutVg_[*v]) >> 8)&0x3FF];
            // blue
            --pxptr;
            (*pxptr) = lutSaturation_[((yy+lutUb_[*u]) >> 8) & 0x3FF];
            
            
            // next pixel doesn't change uv values
            Y+=2;
            yy = (*Y)<<8;
            
            // dummy
            --pxptr;
            // red
            --pxptr;
            (*pxptr) = lutSaturation_[((yy+lutVr_[*v]) >> 8)&0x3FF];
            // green
            --pxptr;
            (*pxptr) = lutSaturation_[((yy-lutUg_[*u]-lutVg_[*v]) >> 8)&0x3FF];
            // blue
            --pxptr;
            (*pxptr) = lutSaturation_[((yy+lutUb_[*u]) >> 8)&0x3FF];
            
            Y+=2;
            u+=4;
            v+=4;
          }
        }
      
      } else {
        int yy,sy;

        ubyte* pxptr = reinterpret_cast<ubyte*>(theImage.data());      

        for (sy=0;sy<resolution.y;++sy) {
          Yend = Y+rowBytes;
          while (Y < Yend) {
            yy = (*Y)<<8;
            
            // blue
            (*pxptr) = lutSaturation_[((yy+lutUb_[*u])>>8)&0x3FF];
            pxptr++;
            // green
            (*pxptr) = lutSaturation_[((yy-lutUg_[*u]-lutVg_[*v])>>8)&0x3FF];
            pxptr++;
            // red
            (*pxptr) = lutSaturation_[((yy+lutVr_[*v])>>8)&0x3FF];
            pxptr++;
            // dummy
            pxptr++;
            
            // next pixel doesn't change uv values
            Y+=2;
            yy = (*Y)<<8;
            
            // blue
            (*pxptr) = lutSaturation_[((yy+lutUb_[*u])>>8)&0x3FF];
            pxptr++;
            // green
            (*pxptr) = lutSaturation_[((yy-lutUg_[*u]-lutVg_[*v])>>8)&0x3FF];
            pxptr++;
            // red
            (*pxptr) = lutSaturation_[((yy+lutVr_[*v])>>8)&0x3FF];
            pxptr++;
            // dummy
            pxptr++;
            
            Y+=2;
            u+=4;
            v+=4;
          }
        }
      }
      
      return true;
    }

    /*
     * Convert data buffer to an channel8 (grey valued channel)
     */
    bool convertYUYV::convert(void* data,
                                const unsigned int bsize,
                                const bool turnAround,
                                channel8& img) const {
      return ::lti::v4l2::convertBase::convert(data,bsize,turnAround,img);
    }



    // -----------------------------------------------------------------
    //                        Bayer SBGGR8
    // -----------------------------------------------------------------

    /*
     * All format converters will reside on this namespace
     */
    class convertSBGGR8 : public ::lti::v4l2::convertBase {
    public:
      /*
       * Constructor
       */
      convertSBGGR8();
      
      /*
       * Destructor
       */
      virtual ~convertSBGGR8();
      
      /*
       * Called when setting the parameters
       */
      virtual bool init(const v4l2::parameters& par);

      /*
       * Convert data buffer to an image
       */
      virtual bool convert(void* data,
                           const unsigned int bsize,
                           const bool turnAround,
                           image& img) const;

      /*
       * Convert data buffer to an channel8 (grey valued channel)
       */
      virtual bool convert(void* data,
                           const unsigned int bsize,
                           const bool turnAround,
                           channel8& img) const;
    protected:
      /*
       * Functor used to deinterpolate the bayer pattern
       */
      bayerDemosaicing* bayer_;
    };

    _LTI_V4L2_REGISTER_(SBGGR8,convertSBGGR8);
      

    convertSBGGR8::convertSBGGR8() : bayer_(0) {
    }

    convertSBGGR8::~convertSBGGR8() {
      delete bayer_;
    }
    

    /*
     * Called when setting the parameters
     */
    bool convertSBGGR8::init(const v4l2::parameters& par) {
      if (notNull(bayer_)) {
        // already set! 
        return true;
      }

      bayer_ = factory<bayerDemosaicing>::
        getFactory().newInstance(par.bayerDemosaicing);

      if (isNull(bayer_)) { // no bayer functor could be found
        return false;
      }

      // we clone the parameters, since we don't know exactly which one is
      // the write inherited class.
      bayerDemosaicing::parameters* bPar = bayer_->getParameters().clone();
      bPar->bayerPattern = bayerDemosaicing::BGGR;
      // bPar->bayerPattern = bayerDemosaicing::GRBG;
      bayer_->attachParameters(*bPar);

      return true; 
    }

    /*
     * Convert data buffer to an image
     */
    bool convertSBGGR8::convert(void* data,
                                const unsigned int bsize,
                                const bool turnAround,
                                image& theImage) const {

      if (isNull(bayer_)) {
        return false;
      }

      if (bsize<=0) {
        return false;
      }

      matrix<ubyte> src(theImage.rows(),theImage.columns(),
                        reinterpret_cast<const ubyte*>(data));

      bayer_->apply(src,theImage);

      if (turnAround) {
        return turn(theImage);
      }
      
      return true;
    }

    /*
     * Convert data buffer to an channel8 (grey valued channel)
     */
    bool convertSBGGR8::convert(void* data,
                                const unsigned int bsize,
                                const bool turnAround,
                                channel8& img) const {
      return ::lti::v4l2::convertBase::convert(data,bsize,turnAround,img);
    }

    // -----------------------------------------------------------------
    //                      SN9C10x compression
    // -----------------------------------------------------------------

    /*
     * Convert compressed SONIX SN9C10X format.
     * 
     * The following code is based on the sn9c102.txt file of the sn9c102
     * kernel module and on the sn9c10x.c file of the libv4lconvert library, by
     * Bertrik Sikken. 
     *
     * From the sn9c102.txt we read:
     *
     * \code 
     * The Huffman compressed video frame consists of a bitstream that
     * encodes for every R, G, or B pixel the difference between the value of
     * the pixel itself and some reference pixel value. Pixels are organised in
     * the Bayer pattern and the Bayer sub-pixels are tracked individually and
     * alternatingly. For example, in the first line values for the B and G1
     * pixels are alternatingly encoded, while in the second line values for
     * the G2 and R pixels are alternatingly encoded.
     *
     * The pixel reference value is calculated as follows:
     * - the 4 top left pixels are encoded in raw uncompressed 8-bit format;
     * - the reference value in the top two rows is the value of the pixel left
     *   of the current pixel;
     * - the reference value in the left column is the value of the pixel above
     *   the current pixel;
     * - for all other pixels, the reference value is the average of the value
     *   of the pixel on the left and the value of the pixel above the current
     *   pixel;
     * - there is one code in the bitstream that specifies the value of a pixel
     *   directly (in 4-bit resolution);
     * - pixel values need to be clamped inside the range [0..255] for proper
     *   decoding.
     *
     * The algorithm purely describes the conversion from compressed Bayer code
     * used in the SN9C101, SN9C102 and SN9C103 chips to uncompressed
     * Bayer. Additional steps are required to convert this to a color image
     * (i.e. a color interpolation algorithm).
     *
     * The following Huffman codes have been found:
     * - 0: +0 (relative to reference pixel value) 
     * - 100: +4
     * - 101: -4?
     * - 1110xxxx: set absolute value to xxxx.0000 
     * - 1101: +11
     * - 1111: -11
     * - 11001: +20
     * - 110000: -20
     * - 110001: ??? - these codes are apparently not used
     *
     * [1] The Huffman compression algorithm has been reverse-engineered and
     *     documented by Bertrik Sikken.
     * /endcode
     */
    class convertSN9C10X : public convertSBGGR8 {
    public:
      /*
       * Constructor
       */
      convertSN9C10X();
      
      /*
       * Destructor
       */
      virtual ~convertSN9C10X();
      
      /*
       * Called when setting the parameters
       */
      virtual bool init(const v4l2::parameters& par);

      /*
       * Convert data buffer to an image
       */
      virtual bool convert(void* data,
                           const unsigned int bsize,
                           const bool turnAround,
                           image& img) const;

      /*
       * Convert data buffer to an channel8 (grey valued channel)
       */
      virtual bool convert(void* data,
                           const unsigned int bsize,
                           const bool turnAround,
                           channel8& img) const;
    protected:
      /**
       * Type for the elements of the coding LUT.
       *
       * The bit stream is aligned with the MSB of a byte, and that byte is
       * used as index to get the code length, the corresponding diference
       * value with respect to the reference.
       */
      struct element {
        /**
         * Length in bits of the current code
         */
        ubyte length;

        /**
         * Difference to be added to the reference pixel
         */ 
        int difference;

        /**
         * If true, the value of the current pixel is composed of the lower
         * nibble as the higher nibble, i.e., if the index is 0xEX, then the
         * real value is 0xX0.
         */
        bool nibble;
      };

      static const element* lut_;
    };

    _LTI_V4L2_REGISTER_(SN9C10X,convertSN9C10X);
      
    // initialization of static attributes
    const convertSN9C10X::element* convertSN9C10X::lut_ = 0;

    // constructor
    convertSN9C10X::convertSN9C10X() : convertSBGGR8() {
    }

    // destructor
    convertSN9C10X::~convertSN9C10X() {
    }
    

    /*
     * Called when setting the parameters
     */
    bool convertSN9C10X::init(const v4l2::parameters& par) {
      if (notNull(lut_)) {
        // already set! 
        return true;
      }

      static element lut[256]; // the one and only LUT

      for (int i=0;i<256;++i) {
        element& e = lut[i];
        e.nibble=false;    // almost all codes do not keep the low nibble...
        if ( (i & 0x80) == 0) { // Code 0: keep reference value
          e.difference = 0;
          e.length = 1;
        } else if ((i & 0xE0) == 0x80 ) { // Code 100
          e.difference = +4;
          e.length = 3;
        } else if ((i & 0xE0) == 0xA0) { // Code 101 
          e.difference = -4;
          e.length = 3;
        } else if ((i & 0xF0) == 0xD0) { // Code 1101
          e.difference = +11;
          e.length = 4;
        } else if ((i & 0xF0) == 0xE0) { // Code 1110xxxx
          e.nibble = true;
          e.difference = (i & 0x0F) << 4;
          e.length = 8;
        } else if ((i & 0xF0) == 0xF0) { // Code 1111
          e.difference = -11;
          e.length = 4;
        } else if ((i & 0xF8) == 0xC8) { // Code 11001
          e.difference = +20;
          e.length = 5;
        } else if ((i & 0xFC) == 0xC0) { // Code 110000
          e.difference = -20;
          e.length = 6;
        } else if ((i & 0xFC) == 0xC4) { // Unknown Code 110001xx:
          e.difference = 0;
          e.length = 8;
        } else { // Are there any codes left?
          e.length = 0;
          e.difference = 0;
	}        
      }
      
      lut_ = lut; // transfer the data

      return convertSBGGR8::init(par); 
    }

    /*
     * Convert data buffer to an image
     */
    bool convertSN9C10X::convert(void* data,
                                 const unsigned int bsize,
                                 const bool turnAround,
                                 image& theImage) const {

      if (isNull(lut_)) {
        return false;
      }

      if (bsize<=0) {
        return false;
      }

      // we don't exactly know the length of the input buffer, but we need
      // it in bytes
      const ubyte *const src = reinterpret_cast<const ubyte*>(data);
      const ubyte* ptr = 0;

      // temporal destination is a bayer channel
      channel8 theChannel(theImage.size());
      ubyte* dest = theChannel.data();

      int bitPos = 0;
      int ref; // reference value

      const int rows = theImage.rows();
      const int cols = theImage.columns();
      const int mcols2 = -2*cols;
      ubyte code;

      const int firstRows = min(2,rows);
      const int firstCols = min(2,cols);
      int y;

      // the first two rows are somehow special
      for (y = 0; y < firstRows; ++y) {

        int x = 0;
        
        // first two pixels in first two rows are stored as raw 8-bit
        ptr = src + (bitPos >> 3);
        int fbitPos7 = (bitPos & 7);
        
        // align the bits with the MSB of code
        code = (ptr[0] << fbitPos7) | (ptr[1] >> (8 - fbitPos7));
        bitPos += 8;
        *dest++ = code;
        
        fbitPos7 = (bitPos & 7);
        
        ptr = src + (bitPos >> 3);
        code = (ptr[0] << fbitPos7) | (ptr[1] >> (8 - fbitPos7));
        bitPos += 8;
        *dest++ = code;
        
        x += 2;

        // the rest of the line (cols-2 pixels)
        while (x < cols) {
          // get bitcode from bitstream
          ptr = src + (bitPos >> 3);
          
          const int bitPos7 = (bitPos & 7);

          // align the bits with the MSB of code
          code = (ptr[0] << bitPos7) | (ptr[1] >> (8 - bitPos7));
          
          // update bit position 
          bitPos += lut_[code].length;
          
          // calculate pixel value
          ref = lut_[code].difference;
          if (!lut_[code].nibble) {
            ref += dest[-2];  // 2 because we have to keep the color
          }
          
          *dest++ = static_cast<ubyte>(within(ref,0,255));
          x++;
        }
      }

      // from the third row on
      for (; y < rows; ++y) {

        int x = 0;

        // the first two pixels
        while (x < firstCols) {
          // get bitcode from bitstream
          ptr = src + (bitPos >> 3);

          const int bitPos7 = (bitPos & 7);

          // align the bits with the MSB of code
          code = (ptr[0] << bitPos7) | (ptr[1] >> (8 - bitPos7));
          
          // update bit position 
          bitPos += lut_[code].length;
          
          // calculate pixel value
          ref = lut_[code].difference;
          if (!lut_[code].nibble) {
            ref += dest[mcols2]; // 2 because we have to keep the color line
                                 // (i.e. BG with BG and GR with GR)
          }
          
          *dest++ = static_cast<ubyte>(within(ref,0,255));
          x++;
        } // while columns
        
        // the rest of the line (cols-2 pixels)
        while (x < cols) {
          // get bitcode from bitstream
          ptr = src + (bitPos >> 3);

          const int bitPos7 = (bitPos & 7);

          // align the bits with the MSB of code
          code = (ptr[0] << bitPos7) | (ptr[1] >> (8 - bitPos7));
          
          // update bit position 
          bitPos += lut_[code].length;
          
          // calculate pixel value
          ref = lut_[code].difference;

          if (!lut_[code].nibble) {
            // main area: average of left pixel and top pixel
            ref += (dest[-2] + dest[mcols2]) / 2;
          }
          
          *dest++ = static_cast<ubyte>(within(ref,0,255));
          x++;
        } // while columns
      } // for all rows
    
      // now we can call the parent class, which does the
      // bayer pattern decoding
      return convertSBGGR8::convert(theChannel.data(),
                                    bsize,
                                    turnAround,
                                    theImage);
    }

    /*
     * Convert data buffer to an channel8 (grey valued channel)
     */
    bool convertSN9C10X::convert(void* data,
                                 const unsigned int bsize,
                                 const bool turnAround,
                                 channel8& img) const {
      return ::lti::v4l2::convertBase::convert(data,bsize,turnAround,img);
    }


    // -----------------------------------------------------------------
    //                        Raw Data Pseudo-Format
    // -----------------------------------------------------------------

    /*
     * All format converters will reside on this namespace
     */
    class convertRaw : public ::lti::v4l2::convertBase {
    public:
      /*
       * Constructor
       */
      convertRaw();
      
      /*
       * Destructor
       */
      virtual ~convertRaw();
      
      /*
       * Called when setting the parameters
       */
      virtual bool init(const v4l2::parameters& par);

      /*
       * Convert data buffer to an image
       */
      virtual bool convert(void* data,
                           const unsigned int bsize,
                           const bool turnAround,
                           image& img) const;

      /*
       * Convert data buffer to an channel8 (grey valued channel)
       */
      virtual bool convert(void* data,
                           const unsigned int bsize,
                           const bool turnAround,
                           channel8& img) const;
    };

    _LTI_V4L2_REGISTER_(Raw,convertRaw);

    convertRaw::convertRaw() {
    }

    convertRaw::~convertRaw() {
    }
    

    /*
     * Called when setting the parameters
     */
    bool convertRaw::init(const v4l2::parameters& ) {
      return true; 
    }

    /*
     * Convert data buffer to an image
     */
    bool convertRaw::convert(void* data,
                             const unsigned int bsize,
                             const bool turnAround,
                             image& theImage) const {
      
      // just fill as long as it gets
      ubyte* ptr = reinterpret_cast<ubyte*>(data);

      image::iterator it=theImage.begin();
      unsigned int i=0;
      const unsigned int top = min(bsize,
                                   uint32(theImage.rows()*theImage.columns()));
      while (i<top) {
        (*it).set(*ptr,*ptr,*ptr);
        ++ptr;
        ++i;
        ++it;
      }

      if (turnAround) {
        return turn(theImage);
      }
      
      return true;
    }

    /*
     * Convert data buffer to an channel8 (grey valued channel)
     */
    bool convertRaw::convert(void* data,
                             const unsigned int bsize,
                             const bool turnAround,
                             channel8& img) const {
      
      // just fill as long as it gets
      ubyte* ptr = reinterpret_cast<ubyte*>(data);

      channel8::iterator it=img.begin();
      const channel8::iterator eit = img.end();
      unsigned int i=0;
      
      while ( (i<bsize) && (it!=eit) ) {
        (*it) = *ptr;
        ++ptr;
        ++i;
        ++it;
      }

      if (turnAround) {
        return turn(img);
      }
      
      return true;
    }

  } // end of namespace internals
  
  // -----------------------------------------------------------------
  //  END OF CONVERT CLASSES
  // -----------------------------------------------------------------


  // -----------------------------------------------------------------
  //  v4l2::bufferType
  // -----------------------------------------------------------------
  v4l2::bufferType::bufferType() : ptr_(0),size_(0),method_(Read) {
  }

  v4l2::bufferType::~bufferType() {
    release();
  }

  inline void* v4l2::bufferType::ptr() const {
    return ptr_;
  }
  
  inline v4l2::eIOMethod v4l2::bufferType::getMethod() const {
    return method_;
  }

  inline unsigned int v4l2::bufferType::size() const {
    return size_;
  }
  
  void* v4l2::bufferType::reserve(const unsigned int bufferSize,
                                  const eIOMethod method,
                                  const int handleOrPageSize,
                                  const int offset) {

    if (notNull(ptr_)) {
      release();
    }
    
    void* p = 0;
    switch (method) {
      case Read:
        p = malloc(bufferSize);
        break;
        
      case UserSpace:
        if (handleOrPageSize < 0) {
          return 0;
        }
        
        if (posix_memalign(&p,handleOrPageSize,bufferSize) != 0) {
          // error occured: whether wrong alignment or insufficient memory
          return 0;
        }
        break;
      case MemoryMap:
        // check first if the handle is ok
        if (handleOrPageSize == -1) {
          return 0;
        }

        // get the mapped memory
        p = v4l2_mmap(NULL, /* start anywhere */
                      bufferSize,
                      PROT_READ | PROT_WRITE, /* required */
                      MAP_SHARED, /* recommended */
                      handleOrPageSize,
                      offset);

        if (p == MAP_FAILED) {
          return 0;
        }
        break;
    default:
        return 0;
    }
    
    size_ = bufferSize;
    method_ = method;
    ptr_ = p;

    return ptr_;
  }

  bool v4l2::bufferType::release() {
    if (isNull(ptr_)) {
      return true;
    }
    
    switch(method_) {
      case Read:
      case UserSpace:
        free(ptr_);
        ptr_=0;
        break;
      case MemoryMap:
        if (v4l2_munmap(ptr_,size_) == -1) {
          // something went really wrong
          return false;
        }
        ptr_=0;
        break;
      default:
        return false;
    }

    size_=0;
    method_=Read;
    return (isNull(ptr_));
  }
  
  // -----------------------------------------------------------------
  //  v4l2::parameters
  // -----------------------------------------------------------------

  v4l2::parameters::parameters(const bool valid)
    : frameGrabber::parameters(), 
      camera::parameters(), 
      panTiltUnit::parameters(),
      lensUnit::parameters() {
    

    if (valid) {
      brightnessMode = FeatureManual;
      contrastMode =  FeatureManual;
      sharpnessMode = FeatureManual;
      hueMode = FeatureManual;
      saturationMode = FeatureManual;
      gammaMode = FeatureManual;
      shutterMode = FeatureAuto;
      gainMode = FeatureAuto;
      whiteBalanceMode = FeatureAuto;

      resolution = ipoint(320,240);
      framerate = 30;

      brightness = 0.5;
      contrast = 0.5;
      sharpness = 0.0;

      redGain = 0.5;
      blueGain= 0.5;

      hue = 0.5;
      saturation = 0.5;
      gamma = 0.5;
      shutter = 0.75;
      gain = 0.75;
      
      ioMethod = MemoryMap;
      pixelFormat = Auto;
      bayerDemosaicing = "lti::bilinearDemosaicing";

      verticalFlipMode = FeatureManual;
      verticalFlip = false;
      horizontalFlipMode = FeatureManual;
      horizontalFlip = false;

      powerLineFilterMode = FeatureManual;
      powerLineFilter = Disabled;
      whiteBalanceTemperatureMode = FeatureManual;
      whiteBalanceTemperature = 6500;
      backlightCompensationMode = FeatureManual;
      backlightCompensation = 0;

      deviceFile = "/dev/video0";
      selectTimeout = 2.0f;
      selectRetries = 5;

    } else {
      // initialize all last used parameters with invalid values to force
      // setting the correct parameters the first time setParameters is called

      brightnessMode = FeatureNA;
      contrastMode =  FeatureNA;
      sharpnessMode = FeatureNA;
      hueMode = FeatureNA;
      saturationMode = FeatureNA;
      gammaMode = FeatureNA;
      shutterMode = FeatureNA;
      gainMode = FeatureNA;
      whiteBalanceMode = FeatureNA;

      resolution = ipoint(-1,-1);
      framerate = -1;

      brightness = -1;
      contrast = -1;
      sharpness = -1;

      redGain = -1;
      blueGain = -1;

      hue = -1;
      saturation = -1;
      gamma = -1;
      shutter = -1;
      gain = -1;

      ioMethod = MemoryMap;
      pixelFormat = Invalid;
      bayerDemosaicing = "bilinearDemosaicing";

      verticalFlipMode = FeatureNA;
      verticalFlip = false;

      horizontalFlipMode = FeatureNA;
      horizontalFlip = false;

      powerLineFilterMode = FeatureNA;
      powerLineFilter = Disabled;

      whiteBalanceTemperatureMode = FeatureNA;
      whiteBalanceTemperature = -1;

      backlightCompensationMode = FeatureNA;
      backlightCompensation = -1;
      
      deviceFile = "/dev/video0";
      selectTimeout = 2.0f;
      selectRetries = 5;
    }
  }

  v4l2::parameters::parameters(const parameters& other) 
    : frameGrabber::parameters(), 
      camera::parameters(), 
      panTiltUnit::parameters(),
      lensUnit::parameters() {
    copy(other);
  }

  v4l2::parameters* v4l2::parameters::clone() const {
    return ( new parameters(*this) );
  }

  v4l2::parameters* v4l2::parameters::newInstance() const {
    return ( new parameters );
  }

  // returns the name of this class
  const std::string& v4l2::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  v4l2::parameters&
  v4l2::parameters::copy(const parameters& other ) {

    frameGrabber::parameters::copy(other);
    camera::parameters::copy(other);
    panTiltUnit::parameters::copy(other);
    lensUnit::parameters::copy(other);

    ioMethod = other.ioMethod;
    pixelFormat = other.pixelFormat;
    bayerDemosaicing = other.bayerDemosaicing;
    verticalFlipMode = other.verticalFlipMode;
    verticalFlip     = other.verticalFlip;
    horizontalFlip   = other.horizontalFlip;
    horizontalFlip   = other.horizontalFlip;
    
    powerLineFilterMode = other.powerLineFilterMode;
    powerLineFilter     = other.powerLineFilter;
    whiteBalanceTemperatureMode = other.whiteBalanceTemperatureMode;
    whiteBalanceTemperature     = other.whiteBalanceTemperature;
    backlightCompensationMode   = other.backlightCompensationMode;
    backlightCompensation       = other.backlightCompensation;
    
    deviceFile = other.deviceFile;
    selectTimeout = other.selectTimeout;
    selectRetries = other.selectRetries;

    return ( *this );
  }

  /*
   * write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool v4l2::parameters::write(ioHandler& handler,
                               const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    b = b && lti::write(handler,"ioMethod",ioMethod);
    b = b && lti::write(handler,"pixelFormat",pixelFormat);
    b = b && lti::write(handler,"bayerDemosaicing",bayerDemosaicing);

    b = b && lti::write(handler,"verticalFlipMode",verticalFlipMode);
    b = b && lti::write(handler,"verticalFlip",verticalFlip);
    b = b && lti::write(handler,"horizontalFlipMode",horizontalFlipMode);
    b = b && lti::write(handler,"horizontalFlip",horizontalFlip);
    
    b = b && lti::write(handler,"powerLineFilterMode",powerLineFilterMode);
    b = b && lti::write(handler,"powerLineFilter",powerLineFilter);
    b = b && lti::write(handler,"whiteBalanceTemperatureMode",
                        whiteBalanceTemperatureMode);
    b = b && lti::write(handler,"whiteBalanceTemperature",
                        whiteBalanceTemperature);

    b = b && lti::write(handler,"backlightCompensationMode",
                        backlightCompensationMode);
    b = b && lti::write(handler,"backlightCompensation",backlightCompensation);

    b = b && lti::write(handler,"deviceFile",deviceFile);
    b = b && lti::write(handler,"selectTimeout",selectTimeout);
    b = b && lti::write(handler,"selectRetries",selectTimeout);

    b = b && frameGrabber::parameters::write(handler,false);
    b = b && camera::parameters::write(handler,false);
    b = b && panTiltUnit::parameters::write(handler,false);
    
    if (complete) {
      b = b && handler.writeEnd();
    }
    
    return b;
  }

  /*
   * read the parameters from the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also read, otherwise only the data block will be read.
   * @return true if write was successful
   */
  bool v4l2::parameters::read(ioHandler& handler,
                              const bool complete) {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    b = b && lti::read(handler,"ioMethod",ioMethod);
    b = b && lti::read(handler,"pixelFormat",pixelFormat);
    b = b && lti::read(handler,"bayerDemosaicing",bayerDemosaicing);

    b = b && lti::read(handler,"verticalFlipMode",verticalFlipMode);
    b = b && lti::read(handler,"verticalFlip",verticalFlip);
    b = b && lti::read(handler,"horizontalFlipMode",horizontalFlipMode);
    b = b && lti::read(handler,"horizontalFlip",horizontalFlip);
    
    b = b && lti::read(handler,"powerLineFilterMode",powerLineFilterMode);
    b = b && lti::read(handler,"powerLineFilter",powerLineFilter);
    b = b && lti::read(handler,"whiteBalanceTemperatureMode",
                       whiteBalanceTemperatureMode);
    b = b && lti::read(handler,"whiteBalanceTemperature",
                       whiteBalanceTemperature);

    b = b && lti::read(handler,"backlightCompensationMode",
                       backlightCompensationMode);
    b = b && lti::read(handler,"backlightCompensation",backlightCompensation);

    b = b && lti::read(handler,"deviceFile",deviceFile);
    b = b && lti::read(handler,"selectTimeout",selectTimeout);
    b = b && lti::read(handler,"selectRetries",selectRetries);

    b = b && frameGrabber::parameters::read(handler,false);
    b = b && camera::parameters::read(handler,false);
    b = b && panTiltUnit::parameters::read(handler,false);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }


  // --------------------------------
  // The main V4L2 class
  // --------------------------------

  /*
   * Convert a value using the given format, to degrees
   */
  inline float v4l2::toDegrees(const float val,
                               const eAngularUnitsType atype) const {
    switch(atype) {
    case DeviceDependent:
      return val/100.0f;
      break;
    case Degrees:
      return val;
      break;
    case Radians:
      return radToDeg(val);
    }
    return 0;
  }

  /*
   * Convert a value using the given format, to degrees
   */
  inline float v4l2::toAngleVal(const float deg,
                                const eAngularUnitsType atype) const {
    switch(atype) {
    case DeviceDependent:
      return deg*100.0f;
      break;
    case Degrees:
      return deg;
      break;
    case Radians:
      return degToRad(deg);
    }
    return 0;
  }



  v4l2::v4l2()
    : frameGrabber(),camera(),panTiltUnit(),lensUnit(),
      initialized_(false),capturing_(false),cameraHndl_(-1),recursions_(0),
      lastUsedParams_(false) {

    // default parameters
    parameters param;
    setParameters(param);
  }

  v4l2::v4l2(const std::string& device)
    : frameGrabber(),camera(),panTiltUnit(),lensUnit(),
      initialized_(false),capturing_(false),cameraHndl_(-1),recursions_(0),
      lastUsedParams_(false) {

    // default parameters
    parameters param;
    param.deviceFile = device;
    frameGrabber::setParameters(param);

 }

  v4l2::v4l2( const parameters& theParam )
    : frameGrabber(),camera(),panTiltUnit(),lensUnit(),
      initialized_(false),capturing_(false),cameraHndl_(-1),recursions_(0),
      lastUsedParams_(false) {

    setParameters(theParam);
  }

  v4l2::v4l2(const v4l2& other)
    : frameGrabber(),camera(),panTiltUnit(),lensUnit(),
      initialized_(false),capturing_(false),cameraHndl_(-1),recursions_(0),
      lastUsedParams_(false) {
    copy(other);
  }

  v4l2::~v4l2() {
    uninitDevice();
    _lti_debug4("v4l2::destructor called and camera freed" << std::endl);
  }

  // returns the name of this class
  const std::string& v4l2::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  v4l2& v4l2::copy(const v4l2& other) {
    frameGrabber::copy(other);

    return (*this);
  }

  v4l2* v4l2::clone() const {
    return new v4l2(*this);
  }
  
  // newInstance member function
  v4l2* v4l2::newInstance() const {
    return new v4l2;
  }

  const v4l2::parameters& v4l2::getParameters() const {
    const parameters* params =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if( isNull(params) ) {
      throw invalidParametersException(name());
    }
    return *params;
  }

  v4l2::parameters& v4l2::getRWParameters() {
    parameters* params = dynamic_cast<parameters*>(&functor::getRWParameters());
    if( isNull(params) ) {
      throw invalidParametersException(name());
    }
    return *params;
  }

  /*
   * setParameters overload to allow changing the camera attributes
   */
  bool v4l2::updateParameters() {
    if (isInitialized() && (resolution_ != getParameters().resolution)) {
      uninitDevice();
    }
    
    return setCameraParameters();
  }

  float v4l2::normalize(const int val,const v4l2_queryctrl& ctrl) const {
    const float mi = static_cast<float>(ctrl.minimum);
    const float ma = static_cast<float>(ctrl.maximum);

    return (float(val)-mi)/(ma-mi);
  }

  int32 v4l2::denormalize(const float par,const v4l2_queryctrl& ctrl) const {
    const float mi = static_cast<float>(ctrl.minimum);
    const float ma = static_cast<float>(ctrl.maximum);

    return iround(mi + (ma-mi)*par);
  }

  bool v4l2::dumpParameters(parameters& par) {
    // get all parameters from the camera

    // we use the VIDIOC_G_FMT to get information about settings in use
    struct v4l2_format format;
    format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (xioctl(cameraHndl_,VIDIOC_G_FMT,&format) < 0) {
      report("VIDIOC_G_FMT");
      return false;
    }
    
    // format.pix contains some valuable information
    par.resolution.set(format.fmt.pix.width,format.fmt.pix.height);
    par.pixelFormat = static_cast<ePixelFormat>(format.fmt.pix.pixelformat);

    if (par.bayerDemosaicing.empty()) {
      par.bayerDemosaicing = "lti::bilinearDemosaicing";
    }

    // also there, but not yet used: 
    //   format.pix.field
    //   format.pix.bytesperline
    //   format.pix.sizeimage
    //   format.pix.colorspace
    
    struct v4l2_streamparm stmPar;
    clear(stmPar);
    bool frameRateSupport = false;
    stmPar.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    int ret; // holder of the ioctl return values
    if ((ret=xioctl(cameraHndl_,VIDIOC_G_PARM,&stmPar)) == 0) {
      frameRateSupport = 
        (stmPar.parm.capture.capability & V4L2_CAP_TIMEPERFRAME);
    }
    
    if (frameRateSupport) {
      par.framerate = (float(stmPar.parm.capture.timeperframe.denominator)/
                       float(stmPar.parm.capture.timeperframe.numerator));
    }

    controlsType::iterator it;
    struct v4l2_control ctrl;

    // --------------------------------------------------------------------
    // Brightness

    ctrl.id = V4L2_CID_BRIGHTNESS;
    if ((it=controls_.find(ctrl.id)) == controls_.end()) {
      par.brightnessMode = FeatureNA;
    } else {
      if (xioctl(cameraHndl_,VIDIOC_G_CTRL,&ctrl) < 0) {
        // some error getting camera id
        report("VIDIOC_G_CTRL:V4L2_CID_BRIGHTNESS");
        return false;
      }
      par.brightnessMode = FeatureManual; // the only available mode
      par.brightness = normalize(ctrl.value,it->second);
    }

    // --------------------------------------------------------------------
    // Contrast

    ctrl.id = V4L2_CID_CONTRAST;
    if ((it=controls_.find(ctrl.id)) == controls_.end()) {
      par.contrastMode = FeatureNA;
    } else {
      if (xioctl(cameraHndl_,VIDIOC_G_CTRL,&ctrl) < 0) {
        // some error getting camera id
        report("VIDIOC_G_CTRL:V4L2_CID_CONTRAST");
        return false;
      }
      par.contrastMode = FeatureManual; // the only available mode
      par.contrast = normalize(ctrl.value,it->second);
    }
    
    // --------------------------------------------------------------------
    // Saturation

    ctrl.id = V4L2_CID_SATURATION;
    if ((it=controls_.find(ctrl.id)) == controls_.end()) {
      par.saturationMode = FeatureNA;
    } else {
      if (xioctl(cameraHndl_,VIDIOC_G_CTRL,&ctrl) < 0) {
        // some error getting camera id
        report("VIDIOC_G_CTRL:V4L2_CID_SATURATION");
        return false;
      }
      par.saturationMode = FeatureManual; // the only available mode
      par.saturation = normalize(ctrl.value,it->second);
    }

    // --------------------------------------------------------------------
    // Hue

    // first try if we have auto hue
    ctrl.id = V4L2_CID_HUE_AUTO;
    if ((it=controls_.find(ctrl.id)) == controls_.end()) {
      par.hueMode = FeatureNA; // not so, guess NA
    } else {
      if (xioctl(cameraHndl_,VIDIOC_G_CTRL,&ctrl) < 0) {
        // some error getting camera id
        report("VIDIOC_G_CTRL:V4L2_CID_HUE_AUTO");
        return false;
      }
      par.hueMode = (ctrl.value != 0) ? FeatureAuto : FeatureManual;
    }

    if (par.hueMode != FeatureAuto) { // check NA guess or get manual value
      ctrl.id = V4L2_CID_HUE;
      if ((it=controls_.find(ctrl.id)) == controls_.end()) {
        par.hueMode = FeatureNA; // indeed not available!
      } else {
        if (xioctl(cameraHndl_,VIDIOC_G_CTRL,&ctrl) < 0) {
          // some error getting camera id
          report("VIDIOC_G_CTRL:V4L2_CID_HUE");
          return false;
        }
      
        par.hueMode = FeatureManual; 
        par.hue = normalize(ctrl.value,it->second);
      }
    }

    // --------------------------------------------------------------------
    // White balance

    // first try if we have auto white balance
    ctrl.id = V4L2_CID_AUTO_WHITE_BALANCE;
    if ((it=controls_.find(ctrl.id)) == controls_.end()) {
      par.whiteBalanceMode = FeatureNA; // not so, guess NA
    } else {
      if (xioctl(cameraHndl_,VIDIOC_G_CTRL,&ctrl) < 0) {
        // some error getting camera id
        report("VIDIOC_G_CTRL:V4L2_CID_AUTO_WHITE_BALANCE");
        return false;
      }
      par.whiteBalanceMode = (ctrl.value != 0) ? FeatureAuto : FeatureManual;
    }

    if (par.whiteBalanceMode != FeatureAuto) { // check NA guess or get manual
      // white balance is manual, so we need to get the red and blue gains
      
      ctrl.id = V4L2_CID_RED_BALANCE;
      if ((it=controls_.find(ctrl.id)) != controls_.end()) {
        if (xioctl(cameraHndl_,VIDIOC_G_CTRL,&ctrl) < 0) {
          // some error getting camera id
          report("VIDIOC_G_CTRL:V4L2_CID_RED_BALANCE");
          return false;
        }
        par.redGain = normalize(ctrl.value,it->second);
      }

      ctrl.id = V4L2_CID_BLUE_BALANCE;
      if ((it=controls_.find(ctrl.id)) != controls_.end()) {
        if (xioctl(cameraHndl_,VIDIOC_G_CTRL,&ctrl) < 0) {
          // some error getting camera id
          report("VIDIOC_G_CTRL:V4L2_CID_BLUE_BALANCE");
          return false;
        }
        par.blueGain = normalize(ctrl.value,it->second);
      }
      
      par.whiteBalanceMode = FeatureManual; 
    }

    // --------------------------------------------------------------------
    // Gamma

    ctrl.id = V4L2_CID_GAMMA;
    if ((it=controls_.find(ctrl.id)) == controls_.end()) {
      par.gammaMode = FeatureNA;
    } else {
      if (xioctl(cameraHndl_,VIDIOC_G_CTRL,&ctrl) < 0) {
        // some error getting camera id
        report("VIDIOC_G_CTRL:V4L2_CID_GAMMA");
        return false;
      }
      par.gammaMode = FeatureManual; // the only available mode
      par.gamma = normalize(ctrl.value,it->second);
    }

    // --------------------------------------------------------------------
    // Shutter

    struct v4l2_ext_control ectrl;
    clear(ectrl);
    ectrl.id = V4L2_CID_EXPOSURE_AUTO;
    if ((it=controls_.find(ectrl.id)) == controls_.end()) {
      par.shutterMode = FeatureNA; // not so, guess NA
    } else {
      // it seems we have auto exposure (an extended camera class)

      struct v4l2_ext_controls extCtrls;
      clear(extCtrls);
      extCtrls.ctrl_class = V4L2_CTRL_CLASS_CAMERA;
      extCtrls.count = 1;
      extCtrls.controls = &ectrl;

      if (xioctl(cameraHndl_,VIDIOC_G_EXT_CTRLS,&extCtrls) < 0) {
        par.shutterMode = FeatureNA;
        
        // some error getting camera id
        report("VIDIOC_G_CTRL:V4L2_CID_EXPOSURE_AUTO");
        // return false; // Better not abort with extended controls
      } 
      par.shutterMode =
        (ectrl.value == V4L2_EXPOSURE_AUTO) ? FeatureAuto : FeatureManual;
    }
    

    if (par.shutterMode != FeatureAuto) { // seems we have manual shutter
      ctrl.id = V4L2_CID_EXPOSURE;
      if ((it=controls_.find(ctrl.id)) == controls_.end()) {
        par.shutterMode = FeatureNA;
      } else {
        if (xioctl(cameraHndl_,VIDIOC_G_CTRL,&ctrl) < 0) {
          // some error getting camera id
          report("VIDIOC_G_CTRL:V4L2_CID_EXPOSURE");
          return false;
        }
        par.shutterMode = FeatureManual; // the only available mode
        par.shutter = normalize(ctrl.value,it->second);
      }
    }
      
    // --------------------------------------------------------------------
    // Gain

    // first try if we have auto gain
    ctrl.id = V4L2_CID_AUTOGAIN;
    if ((it=controls_.find(ctrl.id)) == controls_.end()) {
      par.gainMode = FeatureNA; // not so, guess NA
    } else {
      if (xioctl(cameraHndl_,VIDIOC_G_CTRL,&ctrl) < 0) {
        // some error getting camera id
        report("VIDIOC_G_CTRL:V4L2_CID_GAIN_AUTO");
        return false;
      }
      par.gainMode = (ctrl.value != 0) ? FeatureAuto : FeatureManual;
    }

    if (par.gainMode != FeatureAuto) { // check NA guess or get manual value
      ctrl.id = V4L2_CID_GAIN;
      if ((it=controls_.find(ctrl.id)) == controls_.end()) {
        par.gainMode = FeatureNA; // indeed not available!
      } else {
        if (xioctl(cameraHndl_,VIDIOC_G_CTRL,&ctrl) < 0) {
          // some error getting camera id
          report("VIDIOC_G_CTRL:V4L2_CID_GAIN");
          return false;
        }
      
        par.gainMode = FeatureManual; 
        par.gain = normalize(ctrl.value,it->second);
      }
    }

    // --------------------------------------------------------------------
    // Horizontal Flip

    ctrl.id = V4L2_CID_HFLIP;
    if ((it=controls_.find(ctrl.id)) == controls_.end()) {
      par.horizontalFlipMode = FeatureNA;
    } else {
      if (xioctl(cameraHndl_,VIDIOC_G_CTRL,&ctrl) < 0) {
        // some error getting camera id
        report("VIDIOC_G_CTRL:V4L2_CID_HFLIP");
        return false;
      }
      par.horizontalFlipMode = FeatureManual; // the only available mode
      par.horizontalFlip = (ctrl.value != 0);
    }
    
    // --------------------------------------------------------------------
    // Vertical Flip

    ctrl.id = V4L2_CID_VFLIP;
    if ((it=controls_.find(ctrl.id)) == controls_.end()) {
      par.verticalFlipMode = FeatureNA;
    } else {
      if (xioctl(cameraHndl_,VIDIOC_G_CTRL,&ctrl) < 0) {
        // some error getting camera id
        report("VIDIOC_G_CTRL:V4L2_CID_VFLIP");
        return false;
      }
      par.verticalFlipMode = FeatureManual; // the only available mode
      par.verticalFlip = (ctrl.value != 0);
    }
   
    // --------------------------------------------------------------------
    // Power line frequency filter

    ctrl.id = V4L2_CID_POWER_LINE_FREQUENCY;
    if ((it=controls_.find(ctrl.id)) == controls_.end()) {
      par.powerLineFilterMode = FeatureNA;
    } else {
      if (xioctl(cameraHndl_,VIDIOC_G_CTRL,&ctrl) < 0) {
        // some error getting camera id
        report("VIDIOC_G_CTRL:V4L2_CID_POWER_LINE_FREQUENCY");
        return false;
      }
      par.powerLineFilterMode = FeatureManual; // the only available mode
      par.powerLineFilter = static_cast<ePowerLineFilter>(ctrl.value);
    }
    
    // --------------------------------------------------------------------
    // WhiteBalanceTemperature

    ctrl.id = V4L2_CID_WHITE_BALANCE_TEMPERATURE;
    if ((it=controls_.find(ctrl.id)) == controls_.end()) {
      par.whiteBalanceTemperatureMode = FeatureNA;
    } else {
      if (xioctl(cameraHndl_,VIDIOC_G_CTRL,&ctrl) < 0) {
        // some error getting camera id
        report("VIDIOC_G_CTRL:V4L2_CID_WHITE_BALANCE_TEMPERATURE");
        return false;
      }
      par.whiteBalanceTemperatureMode = FeatureManual; 
      par.whiteBalanceTemperature = within(ctrl.value,
                                           it->second.minimum,
                                           it->second.maximum);
    }

    // --------------------------------------------------------------------
    // Sharpness

    ctrl.id = V4L2_CID_SHARPNESS;
    if ((it=controls_.find(ctrl.id)) == controls_.end()) {
      par.sharpnessMode = FeatureNA;
    } else {
      if (xioctl(cameraHndl_,VIDIOC_G_CTRL,&ctrl) < 0) {
        // some error getting camera id
        report("VIDIOC_G_CTRL:V4L2_CID_SHARPNESS");
        return false;
      }
      par.sharpnessMode = FeatureManual; // the only available mode
      par.sharpness = normalize(ctrl.value,it->second);
    }

    // --------------------------------------------------------------------
    // BacklightCompensation

    ctrl.id = V4L2_CID_BACKLIGHT_COMPENSATION;
    if ((it=controls_.find(ctrl.id)) == controls_.end()) {
      par.backlightCompensationMode = FeatureNA;
    } else {
      if (xioctl(cameraHndl_,VIDIOC_G_CTRL,&ctrl) < 0) {
        // some error getting camera id
        report("VIDIOC_G_CTRL:V4L2_CID_BACKLIGHT_COMPENSATION");
        return false;
      }
      par.backlightCompensationMode = FeatureManual; // the only available mode
      par.whiteBalanceTemperature = within(ctrl.value,
                                           it->second.minimum,
                                           it->second.maximum);
    }

    // TODO: pan/tilt/focus

    return true;
  }

  template<class T>
  inline void v4l2::clear(T& data) const {
    bzero(&data,sizeof(T));
  }

  void v4l2::report(const char *const what) const {
    std::string err = what;
    err += std::string(": ") + std::string(strerror(errno));
    setStatusString(err);
    _lti_debug(err);
  }

  int v4l2::xioctl(int cameraHndl,
                   uint32 request,
                   void *arg) const {

    static const int maxTries = 4;
    int tries = maxTries;
    int res;

    do {
      // ioctl returns -1 when an error occured
      res = ioctl (cameraHndl, static_cast<int>(request), arg);
    } while ( (res == -1) && 
              (tries-- > 0) && 
              ((errno == EINTR) || (errno == EAGAIN) || (errno == ETIMEDOUT)));
    
    return res;
  }

  bool v4l2::openDevice(const parameters& par) {
    struct stat st; 

    if (stat(par.deviceFile.c_str(), &st) == -1) {
      std::string err = "Cannot identify '" + par.deviceFile + "': " +
        std::string(strerror (errno));
      setStatusString(err);
      return false;
    }
      
    if (!S_ISCHR(st.st_mode)) {
      std::string err = par.deviceFile + " is no device";
      setStatusString(err);
      return false;
    }

    cameraHndl_ = v4l2_open(par.deviceFile.c_str(), 
                            O_RDWR /* required */ | O_NONBLOCK,
                            0);

    if (cameraHndl_ == -1) {
      std::string err = "Cannot open '" + par.deviceFile + "': " +
        std::string(strerror(errno));
      setStatusString(err);
      return false;
    }

    return true;
  }

  bool v4l2::closeDevice() {
    if ((cameraHndl_!=-1) && (v4l2_close(cameraHndl_) == -1)) {
      cameraHndl_=-1;
      report("Error closing device");
      return false;
    }
    cameraHndl_ = -1;
    return true;
  }

  /*
   * Init read() based I/O method
   */
  bool v4l2::initRead(const int imageSize) {
    buffers_.resize(1);
    if (isNull(buffers_[0].reserve(imageSize,Read))) {
      setStatusString("Out of memory");
      return false;
    }
    return true;
  }
  
  /*
   * Init memory map based I/O method
   */
  bool v4l2::initMemoryMap() {
    struct v4l2_requestbuffers req;
    clear(req);
    req.count  = 4;
    req.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req.memory = V4L2_MEMORY_MMAP;

    if (buffers_.empty()) {
      // Buffers not requested yet... 
      if (xioctl (cameraHndl_, VIDIOC_REQBUFS, &req) == -1) {
        if (EINVAL == errno) {
          const parameters& par = getParameters();
          std::string err = par.deviceFile + 
            " does not support memory mapping";
          setStatusString(err);
          return false;
        } else {
          report("VIDIOC_REQBUFS");
          return false;
        }
      }

      if (req.count < 2) {
        const parameters& par = getParameters();
        std::string err = "Insufficient buffer memory on " +
          par.deviceFile;
        return false;
      }

      buffers_.resize(req.count);
      std::vector<int> sizes(buffers_.size());
      std::vector<int> offsets(buffers_.size());

      for (unsigned int i=0; i < buffers_.size(); ++i) {
        struct v4l2_buffer buf;
        clear(buf);
        
        buf.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index  = i;
        
        // first, query the buffer
        if (xioctl (cameraHndl_, VIDIOC_QUERYBUF, &buf) == -1) {
          report("VIDIOC_QUERYBUF");
          return false;
        }
        sizes.at(i)  =buf.length;
        offsets.at(i)=buf.m.offset;
      }

      // reserve the freshly installed buffers
      for (unsigned int i=0; i < buffers_.size(); ++i) {
        if (isNull(buffers_[i].reserve(sizes.at(i),MemoryMap,
                                       cameraHndl_,offsets.at(i)))) {
          report("mmap");
          return false;
        }
      }

    }

    return true;
  }

  /*
   * Init user space based I/O method
   */
  bool v4l2::initUserSpace(const int imageSize) {
    struct v4l2_requestbuffers req;
    const int pageSize = getpagesize(); // from <unistd.h>
    const int bufferSize = (imageSize + pageSize - 1) & ~(pageSize - 1);

    clear(req);

    req.count  = 4;
    req.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req.memory = V4L2_MEMORY_USERPTR;

    if (xioctl (cameraHndl_, VIDIOC_REQBUFS, &req) == -1) {
      if (errno == EINVAL) {
        std::string err = getParameters().deviceFile +
          "does not support user pointer I/O";
        return false;
      } else {
        report("VIDIOC_REQBUFS");
        return false;
      }
    }

    buffers_.resize(4);
    
    for (int i = 0; i < 4; ++i) {
      if (isNull(buffers_[i].reserve(bufferSize,MemoryMap,pageSize))) {
        setStatusString("Out of memory");
        return false;
      }
    }
   
    return true;
  }

  bool v4l2::initDevice(const parameters& par) {
    // system structures for Video4Linux2
    struct v4l2_capability cap;

    if (xioctl (cameraHndl_, VIDIOC_QUERYCAP, &cap) == -1) {
      if (errno == EINVAL) {
        std::string err = par.deviceFile + " is no V4L2 device";
        return false;
      } else {
        report("VIDIOC_QUERYCAP");
      }
    }
    
    if (!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)) {
      std::string err = par.deviceFile + " is no video capture device";
      setStatusString(err);
      return false;
    }
    
    switch (par.ioMethod) {
      case Read:
        if (!(cap.capabilities & V4L2_CAP_READWRITE)) {
          std::string err = par.deviceFile + 
            " does not support read I/O";
          setStatusString(err);
          return false;
        }
        break;
      case MemoryMap:
      case UserSpace:
        if (!(cap.capabilities & V4L2_CAP_STREAMING)) {
          std::string err = par.deviceFile + 
            " does not support streaming I/O";
          setStatusString(err);
          return false;
        }
        break;
      default:
        return false;
    }

    cap.card[31]='\0'; // ensure that the next assigment will end!
    camName_.clear();
    transfer(camName_,cap.card);
    _lti_debug4("Camera " << camName_ << " detected." << std::endl);

    cap.driver[15]='\0';
    driverName_.clear();
    transfer(driverName_,cap.driver);
    _lti_debug4("  using driver " << driverName_ << std::endl);

    cap.bus_info[31]='\0';
    busInfo_.clear();
    transfer(busInfo_,cap.bus_info);
    _lti_debug4("  with bus info " << busInfo_ << std::endl);

    _lti_debug4("  version " << ((cap.version >> 16) & 0xFF) << "." << 
                                ((cap.version >>  8) & 0xFF) << "." << 
                                ((cap.version      ) & 0xFF) << std::endl);

    /* Select video input, video standard and tune here. */
    
    struct v4l2_cropcap cropcap;
    clear(cropcap);
    cropcap.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    cropSupported_ = false;

    if (xioctl(cameraHndl_, VIDIOC_CROPCAP, &cropcap) == 0) {
      struct v4l2_crop crop;

      crop.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
      crop.c = cropcap.defrect; /* reset to default */

      if (xioctl (cameraHndl_, VIDIOC_S_CROP, &crop) == -1) {
        switch (errno) {
          case EINVAL:
            /* Cropping not supported. */
            break;
          default:
            /* Errors ignored. */
            break;
        }
      } else {
        cropSupported_ = true;
      }
    } 
    
    return true;
  }

  bool v4l2::initFormat(const eIOMethod ioMethod,
                        const ePixelFormat pxFmt,
                        const v4l2_field fieldOrder,
                        ipoint& res) {

    struct v4l2_format fmt;
    clear(fmt);
    
    fmt.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmt.fmt.pix.width       = res.x;
    fmt.fmt.pix.height      = res.y;
    fmt.fmt.pix.pixelformat = pxFmt;
    fmt.fmt.pix.field       = fieldOrder;

    if (xioctl(cameraHndl_, VIDIOC_S_FMT, &fmt) == -1) {
      report("VIDIOC_S_FMT");
      return false;
    }
    
    pixelFormat_ = pxFmt;
    
    /* Note VIDIOC_S_FMT may change width and height. */
    res.x = fmt.fmt.pix.width;
    res.y = fmt.fmt.pix.height;

    /* Buggy driver paranoia. */
    unsigned int lmin = fmt.fmt.pix.width * 2;
    if (fmt.fmt.pix.bytesperline < lmin) {
      fmt.fmt.pix.bytesperline = lmin;
    }
    
    lmin = fmt.fmt.pix.bytesperline * fmt.fmt.pix.height;
    if (fmt.fmt.pix.sizeimage < lmin) {
      fmt.fmt.pix.sizeimage = lmin;
    }

    switch (ioMethod) {
      case Read:
        return initRead(fmt.fmt.pix.sizeimage);
        break;
        
      case MemoryMap:
        return initMemoryMap();
        break;
        
      case UserSpace:
        return initUserSpace(fmt.fmt.pix.sizeimage);
        break;
    }

    return false;
  }


  bool v4l2::uninitDevice() {
    if (capturing_) {
      stopCapturing();
    }
    closeDevice();

    buffers_.clear();
    initialized_ = false;

    return true;
  }

  bool v4l2::initControls() {
    // Set-up controls_ database
    controls_.clear();
    struct v4l2_queryctrl queryCtrl;
    clear(queryCtrl);

    for (queryCtrl.id = V4L2_CID_BASE;
         queryCtrl.id < V4L2_CID_LASTP1;
         ++queryCtrl.id) {
      if (xioctl(cameraHndl_,VIDIOC_QUERYCTRL,&queryCtrl) == 0) {
        // control id seems to be ok 
        if (queryCtrl.flags & V4L2_CTRL_FLAG_DISABLED) {
          // feature not supported
          continue;
        } else {
          controls_[queryCtrl.id]=queryCtrl;
          
          _lti_debug4("Control: " << (queryCtrl.id & 0x0000FF) << ": " << 
                      queryCtrl.name << "\n");

        }
      } else {
        // hmm, problems querying the control capabilities
        if (errno == EINVAL) {
          continue; // simply the control is not there, get on with it...
        }
        
        // huge problem: something wrong with the camera or device
        report("VIDIOC_QUERYCTRL");
        return false;
      }
    }

    // now to the extended controls

    // CHECKME

#ifdef V4L2_CTRL_CLASS_CAMERA  
    queryCtrl.id = V4L2_CTRL_CLASS_CAMERA | V4L2_CTRL_FLAG_NEXT_CTRL;
    while (xioctl(cameraHndl_, VIDIOC_QUERYCTRL, &queryCtrl) == 0) {
      if (V4L2_CTRL_ID2CLASS(queryCtrl.id) != V4L2_CTRL_CLASS_CAMERA) {
        // finished, we got into another class
        break;
      }

      // control id seems to be ok 
      if ( (queryCtrl.flags & V4L2_CTRL_FLAG_DISABLED) == 0) {
        controls_[queryCtrl.id]=queryCtrl;
        
        _lti_debug4("Class Camera: " << (queryCtrl.id & 0x0000FF) << ": " << 
                    queryCtrl.name << "\n");
        
      }
      
      /* Ensure to get on with the controls ... */
      queryCtrl.id |= V4L2_CTRL_FLAG_NEXT_CTRL;
    }    
#endif
    
    return true;
  }

  bool v4l2::initPixelFormats() {
    pixelFormats_.clear();
    
    struct v4l2_fmtdesc format;
    format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    format.index = 0;

    int ret;
    while ((ret=xioctl(cameraHndl_,VIDIOC_ENUM_FMT,&format)) == 0) {
      pixelFormats_.push_back(format);
      _lti_debug4("Pixel Format: " << pixelFormats_.back().description << 
                  "\n");
      format.index++;
    }

    if (ret == -1) {
      if (errno != EINVAL) {
        report("VIDIOC_ENUM_FMT");
        return false;
      }
    }

    return !pixelFormats_.empty();
  }

  // three private global operators
  //@{

  uint32 gcd(const v4l2_fract& x) {
    uint32 p, q, r;

    p=x.numerator;
    q=x.denominator;
        
    if( q == 0 ) { /* swap p and q if q is zero */
      swap(p,q);
    }

    do { /* do Euclid's algorithm */
      r = p % q;
      p = q;
      q = r;
    }
    while ( r != 0);

    return p;
  }

  inline v4l2_fract operator+(const v4l2_fract& a,
                              const v4l2_fract& b) {
    v4l2_fract tmp;
    tmp.numerator = a.numerator*b.denominator + a.denominator*b.numerator;
    tmp.denominator = a.denominator*b.denominator;
    uint32 tgcd = gcd(tmp);
    tmp.numerator/=tgcd;
    tmp.denominator/=tgcd;

    return tmp;
  }

  inline v4l2_fract operator/(const v4l2_fract& a,
                              const int b) {
    v4l2_fract tmp(a);
    tmp.denominator*=b;
    uint32 tgcd = gcd(tmp);
    tmp.numerator/=tgcd;
    tmp.denominator/=tgcd;

    return tmp;
  }

  inline v4l2_fract operator*(const v4l2_fract& a,
                              const int b) {
    v4l2_fract tmp(a);
    tmp.numerator*=b;
    uint32 tgcd = gcd(tmp);
    tmp.numerator/=tgcd;
    tmp.denominator/=tgcd;

    return tmp;
  }

  inline bool operator<=(const v4l2_fract& a,
                         const v4l2_fract& b) {
    return ((a.numerator*b.denominator) <= (a.denominator*b.numerator)); 
  }
  //@}

  bool v4l2::initSizeFR() {

    // Get some information about features supported
    struct v4l2_streamparm stmPar;
    clear(stmPar);
    stmPar.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    int ret; // holder of the ioctl return values
    if ((ret=xioctl(cameraHndl_,VIDIOC_G_PARM,&stmPar)) == -1) {
      if (errno != EINVAL) {
        report("VIDIOC_G_PARM");
        return false;
      } else {
        frameRateChangeSupported_ = false;
      }
    } else {
      if (stmPar.parm.capture.capability & V4L2_CAP_TIMEPERFRAME) {
        // it seems the frameRateChange is supported, but let us verify if it
        // is really so...

        stmPar.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        stmPar.parm.capture.capability |= V4L2_CAP_TIMEPERFRAME;

        frameRateChangeSupported_ = 
          (xioctl(cameraHndl_,VIDIOC_S_PARM,&stmPar) == 0);
      }
    }

    // assuming that formats is already initialized
    if (pixelFormats_.empty()) {
      return false;
    }

    sizesFR_.clear();
    sizesFR_.resize(pixelFormats_.size()); // ensure right size

    for (unsigned int f=0;f<pixelFormats_.size();++f) {
      _lti_debug4("Pixel Format '" << pixelFormats_[f].description << "'\n");


      // an alias for fast reference (and less confusion!)
      std::vector<resolutionData>& rvct = sizesFR_[f];

      // for each available pixel format
      struct v4l2_frmsizeenum frmSize;
      frmSize.index = 0;
      frmSize.pixel_format = pixelFormats_[f].pixelformat;

      // the first item is special
      if ((ret=xioctl(cameraHndl_,VIDIOC_ENUM_FRAMESIZES,&frmSize)) == 0) {
        switch (frmSize.type) {
          case V4L2_FRMSIZE_TYPE_DISCRETE:
            // we are lucky, that is what we hoped for
            rvct.push_back(resolutionData());
            rvct.back().resolution.set(frmSize.discrete.width,
                                       frmSize.discrete.height);
            minSize_ = maxSize_ = rvct.back().resolution;
            _lti_debug4("  Resolution " << frmSize.discrete.width << "x" <<
                        frmSize.discrete.height << "\n");
            break;
          case V4L2_FRMSIZE_TYPE_CONTINUOUS: 
          case V4L2_FRMSIZE_TYPE_STEPWISE: {
            // some heuristics here: try to keep aspect ratio
            // (for all other sizes cropping is better suited!)
            const struct v4l2_frmsize_stepwise& fsw = frmSize.stepwise;
            ipoint res(fsw.max_width,fsw.max_height);

            // we already have the limits:
            minSize_.set(fsw.min_width,fsw.min_height);
            maxSize_.copy(res);

            ipoint nrw(res),nrh(res);

            // we recover all possible resolutions that have the same 
            // aspect ratio and that fit in the set 
            //(max_width/n,max_height/n) with n integer
            
            // the following is a kind of merge-sort algorithm
            while((res.x>=int(fsw.min_width))&&(res.y>=int(fsw.min_height))) {

              // check if the resolution fulfills the mentioned criterion
              if (fsw.max_width*res.y == fsw.max_height*res.x) {

                rvct.push_back(resolutionData());
                rvct.back().resolution.copy(res);
                _lti_debug4("[" << rvct.size() << "] " <<
                            "Try "<< res.x << "x" << res.y << "\n");
              }
 
              while(nrw.x >= res.x) {
                nrw.x -= fsw.step_width;
                nrw.y = nrw.x*fsw.max_height/fsw.max_width;
              }

              while(nrh.x >= res.x) {
                nrh.y -= fsw.step_height;
                nrh.x = nrh.y*fsw.max_width/fsw.max_height;
              }

              if (nrw.x > nrh.x) {
                res = nrw;
              } else {
                res = nrh;
              }
            }

          } break;
          default:
            // this should never happen
            setStatusString("Unknown v4l2_frmsizeenum value for type");
            return false;
        }
      } else {
        if (ret == -1) {
          if (errno != EINVAL) {
            report("VIDIOC_ENUM_FRAMESIZES");
            return false;
          }
        }
      }
      
      // continue getting the discrete resolutions if necessary
      if (frmSize.type == V4L2_FRMSIZE_TYPE_DISCRETE) {
        frmSize.index++;
        while ((ret=xioctl(cameraHndl_,VIDIOC_ENUM_FRAMESIZES,&frmSize)) == 0) {
          rvct.push_back(resolutionData());
          rvct.back().resolution.set(frmSize.discrete.width,
                                     frmSize.discrete.height);

          // Update the available resolutions range
          minmax(int(frmSize.discrete.width),minSize_.x,maxSize_.x);
          minmax(int(frmSize.discrete.height),minSize_.y,maxSize_.y);

          _lti_debug4("  Resolution " << frmSize.discrete.width << "x" <<
                      frmSize.discrete.height << "\n");
          
          frmSize.index++;
        }
        if (ret == -1) {
          if (errno != EINVAL) {
            report("VIDIOC_ENUM_FRAMESIZES");
            return false;
          }
        }
      } 

      // ----------------------------------------------------------------------
      // now, for each resolution detected in this pixel format we have to find
      // the available framerates.
      std::vector<resolutionData>::iterator it=rvct.begin();
      while (it!=rvct.end()) {
        // for each available resolution
        _lti_debug4("  Studying resolution " << (*it).resolution << "\n");
        struct v4l2_frmivalenum frmInt;        
        clear(frmInt);

        frmInt.index = 0;
        frmInt.pixel_format = pixelFormats_[f].pixelformat;
        frmInt.width = (*it).resolution.x;
        frmInt.height= (*it).resolution.y;
        
        // the first item is special
        if ((ret=xioctl(cameraHndl_,VIDIOC_ENUM_FRAMEINTERVALS,&frmInt)) == 0) {
          switch (frmInt.type) {
            case V4L2_FRMIVAL_TYPE_DISCRETE:
              // we are lucky, that is what we hoped for
              (*it).frameIntervals.push_back(frmInt.discrete);
              
              _lti_debug4("    Frame intervals " << 
                          frmInt.discrete.numerator << "/" << 
                          frmInt.discrete.denominator << " ");
              break;
            case V4L2_FRMIVAL_TYPE_CONTINUOUS:
            case V4L2_FRMIVAL_TYPE_STEPWISE: {
              _lti_debug4("    Frame intervals ");
              const struct v4l2_frmival_stepwise& fsw = frmInt.stepwise;
              // if I got the documentation right, we can do the following:
              // it would be too much to have step(1) for all frame rates
              // but we can restrict the valid ones to a finite set of
              // related by factors of the powers of 2.
              //
              // Warning: This might change in the future
              for (struct v4l2_fract ivl=fsw.min;
                   ivl <= fsw.max;
                   ivl = ivl+fsw.step) {
                (*it).frameIntervals.push_back(ivl);
                _lti_debug4(ivl.numerator << "/" << ivl.denominator << " ");
              }
            } break;
            default:
              // this should never happen
              setStatusString("Unknown v4l2_frmivalenum value for type");
              return false;
          } // switch
        } else {// if ioctl
          if (ret == -1) {
            if (errno != EINVAL) {
              report("VIDIOC_ENUM_FRAMEINTERVALS");
              return false;
            }
          }
        }

        // continue getting the discrete frame intervals if necessary
        if (frmInt.type == V4L2_FRMIVAL_TYPE_DISCRETE) {
          frmInt.index++;
          while ((ret=xioctl(cameraHndl_,
                            VIDIOC_ENUM_FRAMEINTERVALS,&frmInt)) == 0) {
            (*it).frameIntervals.push_back(frmInt.discrete);
            
            _lti_debug4(frmInt.discrete.numerator << "/" << 
                        frmInt.discrete.denominator << " ");
            frmInt.index++;
          }
          _lti_debug4("\n");
          if (ret == -1) {
            if (errno != EINVAL) {
              report("VIDIOC_ENUM_FRAMEINTERVALS");
              return false;
            }
          }
        } 
        
        if ((*it).frameIntervals.empty()) {
          // resolution has no frame intervals associated or the
          // kernel module still does not support it!
          //
          // we have to guess:  Assume 60 Hz and per frame dropping all
          // typical ones below.
          // WARNING: this guess can be really wrong with european standards!
          //          this can then be changed to ask which standard is being
          //          used...
          _lti_debug4("    Guessing as driver doesn't support frame " \
                      "interval checking...\n      Frame intervals: ");
          
          struct v4l2_fract fi;
          fi.numerator = 1;
          fi.denominator = 60;
          
          for (int i=0;i<5;++i) {
            (*it).frameIntervals.push_back(fi);
            _lti_debug4(fi.numerator << "/" << fi.denominator << " ");
            fi = fi*2;
          }

          _lti_debug4("\n");
        }
        ++it;
      } // for each resolution
    } // for each pixel format

    return true;
  }

  // Initialize the camera with the functor parameters
  bool v4l2::init() {
    if (initialized_) {
      // already initialized
      return true;
    }

    const parameters& par = getParameters();
    if (openDevice(par) && initDevice(par)) {
      initialized_ = true;
    } else {
      return false; // could not open device or preliminary initializations
    }

    // query for supported controls, formats, sizes and frame intervals
    if (!initControls() || !initPixelFormats() || !initSizeFR()) {
      return false;
    }

    // last step

    if (setCameraParameters(recursions_)) {
      return true;
    } else {
      uninitDevice();
      return false;
    }

    return false;
  }

  /*
   * Set brightness control mode and register in camera directly.
   *
   * @param mode Camera desired mode (@see eCamFeatureMode)
   * @param brightness If mode is manual, the value to be used.
   *
   * @return true if operation was successfull, false otherwise
   */
  bool v4l2::setBrightness(const eCamFeatureMode mode,
                           const float brightness) {


    if (!initialized_ ) {
      if (!init()) {
        return false;
      }
    }

    controlsType::iterator it;
    struct v4l2_control ctrl;
    ctrl.id = V4L2_CID_BRIGHTNESS;

    if ((it=controls_.find(ctrl.id)) == controls_.end()) {
      lastUsedParams_.brightnessMode = FeatureNA;
      getRWParameters().brightnessMode = FeatureNA; 

      setStatusString("Brightness control not supported by your camera.");
      return false;
    }

    if (mode != FeatureManual) {
      lastUsedParams_.brightnessMode = FeatureNA;
      getRWParameters().brightnessMode = FeatureNA; 

      setStatusString("Brightness only available in manual mode.");
      return false;
    }

    if (lastUsedParams_.brightness != brightness) {

      ctrl.value = denormalize(brightness,(*it).second);

      if (xioctl(cameraHndl_,VIDIOC_S_CTRL,&ctrl) < 0) {
        lastUsedParams_.brightnessMode = FeatureNA;
        getRWParameters().brightnessMode = FeatureNA; 

        report("VIDIOC_S_CTRL:V4L2_CID_BRIGHTNESS");
        return false;
      } else {
        lastUsedParams_.brightness = brightness;
        lastUsedParams_.brightnessMode = mode;
        getRWParameters().brightness = brightness;
        getRWParameters().brightnessMode = mode; 
      }

    }

    return true;
  }
  
  /*
   * Get brightness mode and value directly out of camera.
   *
   * @param mode Variable where the current brightness mode is to be 
   *              written.
   * @param brightness Variable where the brightness value is to be written.
   *
   * @return true if operation was successfull, false otherwise
   */ 
  bool v4l2::getBrightness(eCamFeatureMode& mode,
                          float& brightness) const {
    if (initialized_) {
      struct v4l2_control ctrl;
      mode = FeatureNA; // first guess: not supported!
      
      ctrl.id = V4L2_CID_BRIGHTNESS;
      controlsType::const_iterator it = controls_.find(ctrl.id);

      if (it == controls_.end()) {
        brightness = 0.0f;
        mode = FeatureNA; 
        return true;
      }

      if (xioctl(cameraHndl_,VIDIOC_G_CTRL,&ctrl) < 0) {
        // some error getting camera id
        report("VIDIOC_G_CTRL:V4L2_CID_BRIGHTNESS");
        return false;
      }
      
      mode = FeatureManual; // the only available mode
      brightness = normalize(ctrl.value,(*it).second);

      return true;
    }
    return false;
  }
  
  /*
   * Set contrast through the exposure method control mode and
   * register in camera directly.
   *
   * @param mode Camera desired mode (@see eCamFeatureMode)
   * @param exposure If mode is manual, the value to be used.
   *
   * @return true if operation was successfull, false otherwise
   */
  bool v4l2::setExposure(const eCamFeatureMode mode,
                         const float contrast) {

    if (!initialized_ ) {
      if (!init()) {
        return false;
      }
    }

    controlsType::iterator it;
    struct v4l2_control ctrl;
    ctrl.id = V4L2_CID_CONTRAST;

    if ((it=controls_.find(ctrl.id)) == controls_.end()) {
      lastUsedParams_.contrastMode = FeatureNA;
      getRWParameters().contrastMode = FeatureNA; 

      setStatusString("Contrast control not supported by your camera.");
      return false;
    }

    if (mode != FeatureManual) {
      lastUsedParams_.contrastMode = FeatureNA;
      getRWParameters().contrastMode = FeatureNA; 

      setStatusString("Contrast only available in manual mode.");
      return false;
    }

    if (lastUsedParams_.contrast != contrast) {

      ctrl.value = denormalize(contrast,(*it).second);

      if (xioctl(cameraHndl_,VIDIOC_S_CTRL,&ctrl) < 0) {
        lastUsedParams_.contrastMode = FeatureNA;
        getRWParameters().contrastMode = FeatureNA; 

        report("VIDIOC_S_CTRL:V4L2_CID_CONTRAST");
        return false;
      } else {
        lastUsedParams_.contrast = contrast;
        lastUsedParams_.contrastMode = mode;
        getRWParameters().contrast = contrast;
        getRWParameters().contrastMode = mode; 
      }

    }

    return true;
  }

  /*
   * Get contrast through the exposure method.
   *
   * @param mode Variable where the current exposure mode is to be 
   *              written.
   * @param exposure Variable where the exposure value is to be written.
   *
   * @return true if operation was successfull, false otherwise
   */ 
  bool v4l2::getExposure(eCamFeatureMode& mode,
                         float& contrast) const {

    if (initialized_) {
      struct v4l2_control ctrl;
      mode = FeatureNA; // first guess: not supported!
      
      ctrl.id = V4L2_CID_CONTRAST;
      controlsType::const_iterator it = controls_.find(ctrl.id);

      if (it == controls_.end()) {
        contrast = 0.0f;
        mode = FeatureNA; 
        return true;
      }

      if (xioctl(cameraHndl_,VIDIOC_G_CTRL,&ctrl) < 0) {
        // some error getting camera id
        report("VIDIOC_G_CTRL:V4L2_CID_CONTRAST");
        mode = FeatureNA; 
        return false;
      }
      
      mode = FeatureManual; // the only available mode
      contrast = normalize(ctrl.value,(*it).second);

      return true;
    }
    return false;
  }

  /*
   * Set sharpness control mode and register in camera directly.
   *
   * @param mode Camera desired mode (@see eCamFeatureMode)
   * @param sharpness If mode is manual, the value to be used.
   *
   * @return true if operation was successfull, false otherwise
   */
  bool v4l2::setSharpness(const eCamFeatureMode mode,
                          const float sharpness) {
    

    if (!initialized_ ) {
      if (!init()) {
        return false;
      }
    }

    controlsType::iterator it;
    struct v4l2_control ctrl;
    ctrl.id = V4L2_CID_SHARPNESS;

    if ((it=controls_.find(ctrl.id)) == controls_.end()) {
      lastUsedParams_.sharpnessMode = FeatureNA;
      getRWParameters().sharpnessMode = FeatureNA; 

      setStatusString("Sharpness control not supported by your camera.");
      return false;
    }

    if (mode != FeatureManual) {
      lastUsedParams_.sharpnessMode = FeatureNA;
      getRWParameters().sharpnessMode = FeatureNA; 

      setStatusString("Sharpness only available in manual mode.");
      return false;
    }

    if (lastUsedParams_.sharpness != sharpness) {

      ctrl.value = denormalize(sharpness,(*it).second);

      if (xioctl(cameraHndl_,VIDIOC_S_CTRL,&ctrl) < 0) {
        lastUsedParams_.sharpnessMode = FeatureNA;
        getRWParameters().sharpnessMode = FeatureNA; 

        report("VIDIOC_S_CTRL:V4L2_CID_SHARPNESS");
        return false;
      } else {
        lastUsedParams_.sharpness = sharpness;
        lastUsedParams_.sharpnessMode = mode;
        getRWParameters().sharpness = sharpness;
        getRWParameters().sharpnessMode = mode; 
      }

    }

    return true;
  }

    /*
     * Get sharpness mode and value directly out of camera.
     *
     * @param mode Variable where the current sharpness mode is to be 
     *              written.
     * @param sharpness Variable where the sharpness value is to be written.
     *
     * @return true if operation was successfull, false otherwise
     */ 
  bool v4l2::getSharpness(eCamFeatureMode& mode,
                         float& sharpness) const {
    if (initialized_) {
      struct v4l2_control ctrl;
      mode = FeatureNA; // first guess: not supported!
      
      ctrl.id = V4L2_CID_SHARPNESS;
      controlsType::const_iterator it = controls_.find(ctrl.id);

      if (it == controls_.end()) {
        sharpness = 0.0f;
        mode = FeatureNA; 
        return true;
      }

      if (xioctl(cameraHndl_,VIDIOC_G_CTRL,&ctrl) < 0) {
        // some error getting camera id
        report("VIDIOC_G_CTRL:V4L2_CID_SHARPNESS");
        return false;
      }
      
      mode = FeatureManual; // the only available mode
      sharpness = normalize(ctrl.value,(*it).second);

      return true;
    }
    return false;
  }


  bool v4l2::setWhiteBalance(const eCamFeatureMode mode,
                             const float ubGain,
                             const float vrGain) {

    if (!initialized_ ) {
      if (!init()) {
        return false;
      }
    }
    
    controlsType::iterator it;
    struct v4l2_control ctrl;

    // first try if we have auto white balance
    ctrl.id = V4L2_CID_AUTO_WHITE_BALANCE;
    ctrl.value = (mode == FeatureAuto) ? 1 : 0;
    if ((it=controls_.find(ctrl.id)) != controls_.end()) {
      if (xioctl(cameraHndl_,VIDIOC_S_CTRL,&ctrl) < 0) {
        lastUsedParams_.whiteBalanceMode = FeatureNA;
        getRWParameters().whiteBalanceMode = FeatureNA; 

        // some error getting camera id
        report("VIDIOC_G_CTRL:V4L2_CID_AUTO_WHITE_BALANCE");
        return false;
      }
    } else {
      if (mode == FeatureAuto) {
        lastUsedParams_.whiteBalanceMode = FeatureManual;
        getRWParameters().whiteBalanceMode = FeatureManual;
        setStatusString("No automatic white balance supported.");
        return false;
      }
    }

    lastUsedParams_.whiteBalanceMode = mode;
    getRWParameters().whiteBalanceMode = mode;
    
    if (mode == FeatureManual) {

      if (lastUsedParams_.redGain != vrGain) {

        ctrl.id = V4L2_CID_RED_BALANCE;
        if ((it=controls_.find(ctrl.id)) != controls_.end()) {
          ctrl.value = denormalize(vrGain,(*it).second);
          if (xioctl(cameraHndl_,VIDIOC_S_CTRL,&ctrl) < 0) {
            report("VIDIOC_S_CTRL:V4L2_CID_RED_BALANCE");
            return false;
          } else {
            lastUsedParams_.redGain = vrGain;
            getRWParameters().redGain = vrGain;
          }
        }
      }

      if (lastUsedParams_.blueGain != ubGain) {

        ctrl.id = V4L2_CID_BLUE_BALANCE;
        if ((it=controls_.find(ctrl.id)) != controls_.end()) {
          ctrl.value = denormalize(ubGain,(*it).second);
          if (xioctl(cameraHndl_,VIDIOC_S_CTRL,&ctrl) < 0) {
            report("VIDIOC_S_CTRL:V4L2_CID_BLUE_BALANCE");
            return false;
          } else {
            lastUsedParams_.blueGain = ubGain;
            getRWParameters().blueGain = ubGain;
          }
        }
      }
    }

    return true;
  }
    

  bool v4l2::getWhiteBalance(eCamFeatureMode& mode,
                            float& ubGain,
                            float& vrGain) const {

    if (initialized_) {
      struct v4l2_control ctrl;
      mode = FeatureNA; // first guess: not supported!

      ctrl.id = V4L2_CID_AUTO_WHITE_BALANCE;
      controlsType::const_iterator it = controls_.find(ctrl.id);

      if ((it=controls_.find(ctrl.id)) != controls_.end()) {
        if (xioctl(cameraHndl_,VIDIOC_G_CTRL,&ctrl) < 0) {
          // some error getting camera id
          mode = FeatureNA; 
          ubGain = vrGain = 0.0f;
          report("VIDIOC_G_CTRL:V4L2_CID_AUTO_WHITE_BALANCE");
          return false;
        }
        mode = (ctrl.value != 0) ? FeatureAuto : FeatureManual;
      }
      
      if (mode != FeatureAuto) { 
        // white balance is manual, so we need to get the red and blue gains
        
        ctrl.id = V4L2_CID_RED_BALANCE;
        if ((it=controls_.find(ctrl.id)) != controls_.end()) {
          if (xioctl(cameraHndl_,VIDIOC_G_CTRL,&ctrl) < 0) {
            // some error getting camera id
            report("VIDIOC_G_CTRL:V4L2_CID_RED_BALANCE");
            mode = FeatureNA; 
            return false;
          }
          vrGain = normalize(ctrl.value,it->second);
        }
        
        ctrl.id = V4L2_CID_BLUE_BALANCE;
        if ((it=controls_.find(ctrl.id)) != controls_.end()) {
          if (xioctl(cameraHndl_,VIDIOC_G_CTRL,&ctrl) < 0) {
            // some error getting camera id
            report("VIDIOC_G_CTRL:V4L2_CID_BLUE_BALANCE");
            mode = FeatureNA; 
            return false;
          }
          ubGain = normalize(ctrl.value,it->second);
        }
        
        mode = FeatureManual; 
      }
      return true;
    }
    
    mode = FeatureNA; 
    return false;
  }

  /*
   * Set hue control mode and register in camera directly.
   *
   * @param mode Camera desired mode (@see eCamFeatureMode)
   * @param hue If mode is manual, the value to be used.
   *
   * @return true if operation was successfull, false otherwise
   */
  bool v4l2::setHue(const eCamFeatureMode mode,
                    const float hue) {

    if (!initialized_ ) {
      if (!init()) {
        return false;
      }
    }

    controlsType::iterator it;
    struct v4l2_control ctrl;

    // first try if we have auto white balance
    ctrl.id = V4L2_CID_HUE_AUTO;
    ctrl.value = (mode == FeatureAuto) ? 1 : 0;

    if ((it=controls_.find(ctrl.id)) != controls_.end()) {
      if (xioctl(cameraHndl_,VIDIOC_S_CTRL,&ctrl) < 0) {
        lastUsedParams_.hueMode = FeatureNA;
        getRWParameters().hueMode = FeatureNA;

        // some error getting camera id
        report("VIDIOC_G_CTRL:V4L2_CID_HUE_AUTO");
        return false;
      }
    }

    lastUsedParams_.hueMode = mode;
    getRWParameters().hueMode = mode;

    if (mode == FeatureManual) {
      if (lastUsedParams_.hue != hue) {

        ctrl.id = V4L2_CID_HUE;
        if ((it=controls_.find(ctrl.id)) != controls_.end()) {
          ctrl.value = denormalize(hue,(*it).second);

          if (xioctl(cameraHndl_,VIDIOC_S_CTRL,&ctrl) < 0) {
            lastUsedParams_.hueMode = FeatureNA;
            getRWParameters().hueMode = FeatureNA;
            
            report("VIDIOC_S_CTRL:V4L2_CID_HUE");
            return false;
          } else {
            lastUsedParams_.hue = hue;
            getRWParameters().hue = hue;
          }
        }
      }
    }

    return true;
  }

  /*
   * Get hue mode and value directly out of camera.
   *
   * @param mode Variable where the current hue mode is to be 
   *              written.
   * @param hue Variable where the hue value is to be written.
   *
   * @return true if operation was successfull, false otherwise
   */ 
  bool v4l2::getHue(eCamFeatureMode& mode,
                    float& hue) const {
    if (initialized_) {
      struct v4l2_control ctrl;
      mode = FeatureNA; // first guess: not supported!
      
      ctrl.id = V4L2_CID_HUE_AUTO;
      controlsType::const_iterator it = controls_.find(ctrl.id);

      if ((it=controls_.find(ctrl.id)) != controls_.end()) {
        // auto-mode supported
        if (xioctl(cameraHndl_,VIDIOC_G_CTRL,&ctrl) < 0) {
          mode = FeatureNA; 
          hue = 0.0f;
          // some error getting camera id
          report("VIDIOC_G_CTRL:V4L2_CID_HUE_AUTO");
          return false;
        }
        mode = (ctrl.value != 0) ? FeatureAuto : FeatureManual;
      }
      
      if (mode != FeatureAuto) { 
        ctrl.id = V4L2_CID_HUE;
        if ((it=controls_.find(ctrl.id)) == controls_.end()) {
          mode = FeatureNA; // indeed not available!
        } else {
          if (xioctl(cameraHndl_,VIDIOC_G_CTRL,&ctrl) < 0) {
            // some error getting camera id
            report("VIDIOC_G_CTRL:V4L2_CID_HUE");
            mode = FeatureNA;
            hue = 0.0f;
            return false;
          }
          hue = normalize(ctrl.value,it->second);
          mode = FeatureManual; 
        }
      }
      return true;
    }

    mode = FeatureNA; 
    return false;
  }

  /*
   * Set saturation control mode and register in camera directly.
   *
   * @param mode Camera desired mode (@see eCamFeatureMode)
   * @param saturation If mode is manual, the value to be used.
   *
   * @return true if operation was successfull, false otherwise
   */
  bool v4l2::setSaturation(const eCamFeatureMode mode,
                           const float saturation) {


    if (!initialized_ ) {
      if (!init()) {
        return false;
      }
    }

    controlsType::iterator it;
    struct v4l2_control ctrl;
    ctrl.id = V4L2_CID_SATURATION;

    if ((it=controls_.find(ctrl.id)) == controls_.end()) {
      lastUsedParams_.saturationMode = FeatureNA;
      getRWParameters().saturationMode = FeatureNA; 

      setStatusString("Saturation control not supported by your camera.");
      return false;
    }

    if (mode != FeatureManual) {
      lastUsedParams_.saturationMode = FeatureNA;
      getRWParameters().saturationMode = FeatureNA; 

      setStatusString("Saturation only available in manual mode.");
      return false;
    }

    if (lastUsedParams_.saturation != saturation) {

      ctrl.value = denormalize(saturation,(*it).second);

      if (xioctl(cameraHndl_,VIDIOC_S_CTRL,&ctrl) < 0) {
        lastUsedParams_.saturationMode = FeatureNA;
        getRWParameters().saturationMode = FeatureNA; 

        report("VIDIOC_S_CTRL:V4L2_CID_SATURATION");
        return false;
      } else {
        lastUsedParams_.saturation = saturation;
        lastUsedParams_.saturationMode = mode;
        getRWParameters().saturation = saturation;
        getRWParameters().saturationMode = mode; 
      }

    }

    return true;
  }

  /*
   * Get saturation mode and value directly out of camera.
   *
   * @param mode Variable where the current saturation mode is to be 
   *              written.
   * @param saturation Variable where the saturation value is to be written.
   *
   * @return true if operation was successfull, false otherwise
   */ 
  bool v4l2::getSaturation(eCamFeatureMode& mode,
                           float& saturation) const {
    if (initialized_) {
      struct v4l2_control ctrl;
      mode = FeatureNA; // first guess: not supported!
      
      ctrl.id = V4L2_CID_SATURATION;
      controlsType::const_iterator it = controls_.find(ctrl.id);

      if (it == controls_.end()) {
        saturation = 0.0f;
        return true;
      }

      if (xioctl(cameraHndl_,VIDIOC_G_CTRL,&ctrl) < 0) {
        // some error getting camera id
        report("VIDIOC_G_CTRL:V4L2_CID_SATURATION");
        return false;
      }
      
      mode = FeatureManual; // the only available mode
      saturation = normalize(ctrl.value,(*it).second);

      return true;
    }
    return false;
  }
    
  /*
   * Set gamma control mode and register in camera directly.
   *
   * @param mode Camera desired mode (@see eCamFeatureMode)
   * @param gamma If mode is manual, the value to be used.
   *
   * @return true if operation was successfull, false otherwise
   */
  bool v4l2::setGamma(const eCamFeatureMode mode,
                      const float gamma) {
    if (!initialized_ ) {
      if (!init()) {
        return false;
      }
    }

    controlsType::iterator it;
    struct v4l2_control ctrl;
    ctrl.id = V4L2_CID_GAMMA;

    if ((it=controls_.find(ctrl.id)) == controls_.end()) {
      lastUsedParams_.gammaMode = FeatureNA;
      getRWParameters().gammaMode = FeatureNA; 

      setStatusString("Gamma control not supported by your camera.");
      return false;
    }

    if (mode != FeatureManual) {
      lastUsedParams_.gammaMode = FeatureNA;
      getRWParameters().gammaMode = FeatureNA; 

      setStatusString("Gamma only available in manual mode.");
      return false;
    }

    if (lastUsedParams_.gamma != gamma) {

      ctrl.value = denormalize(gamma,(*it).second);

      if (xioctl(cameraHndl_,VIDIOC_S_CTRL,&ctrl) < 0) {
        lastUsedParams_.gammaMode = FeatureNA;
        getRWParameters().gammaMode = FeatureNA;

        report("VIDIOC_S_CTRL:V4L2_CID_GAMMA");
        return false;
      } else {
        lastUsedParams_.gamma = gamma;
        lastUsedParams_.gammaMode = mode;
        getRWParameters().gamma = gamma;
        getRWParameters().gammaMode = mode; 
      }

    }

    return true;
  }

  /*
   * Get gamma mode and value directly out of camera.
   *
   * @param mode Variable where the current gamma mode is to be 
   *              written.
   * @param gamma Variable where the gamma value is to be written.
   *
   * @return true if operation was successfull, false otherwise
   */ 
  bool v4l2::getGamma(eCamFeatureMode& mode,
                      float& gamma) const {
    if (initialized_) {
      struct v4l2_control ctrl;
      mode = FeatureNA; // first guess: not supported!
      
      ctrl.id = V4L2_CID_GAMMA;
      controlsType::const_iterator it = controls_.find(ctrl.id);

      if (it == controls_.end()) {
        gamma = 0.0f;
        return true;
      }

      if (xioctl(cameraHndl_,VIDIOC_G_CTRL,&ctrl) < 0) {
        // some error getting camera id
        report("VIDIOC_G_CTRL:V4L2_CID_GAMMA");
        return false;
      }
      
      mode = FeatureManual; // the only available mode
      gamma = normalize(ctrl.value,(*it).second);

      return true;
    }
    return false;
  }

  /*
   * Set shutter control mode and register in camera directly.
   *
   * @param mode Camera desired mode (@see eCamFeatureMode)
   * @param shutter If mode is manual, the value to be used.
   *
   * @return true if operation was successfull, false otherwise
   */
  bool v4l2::setShutter(const eCamFeatureMode mode,
                        const float shutter) {
    
    if (!initialized_ ) {
      if (!init()) {
        return false;
      }
    }

    if (mode == FeatureNA) {
      lastUsedParams_.shutterMode = FeatureNA;
      getRWParameters().shutterMode = FeatureNA;
      return true;
    }

    controlsType::iterator it;
    struct v4l2_control ctrl;
    clear(ctrl);
    ctrl.id = V4L2_CID_EXPOSURE_AUTO;
    
    if ((it=controls_.find(ctrl.id)) != controls_.end()) {
      
      // first try if we have auto white balance
      ctrl.value = (mode == FeatureAuto) ? V4L2_EXPOSURE_AUTO :
        V4L2_EXPOSURE_MANUAL;

      // for the uvcvideo driver, it seems that the value is otherwise
      // conceived as it should!
      if (driverName_ == "uvcvideo") {
        ctrl.value = 1 << ctrl.value;
      }
     
      if (xioctl(cameraHndl_,VIDIOC_S_CTRL,&ctrl) < 0) {
        lastUsedParams_.shutterMode = FeatureNA;
        getRWParameters().shutterMode = FeatureNA; 

        // some error getting camera id
        report("VIDIOC_S_CTRL:V4L2_CID_EXPOSURE_AUTO");
        // return false; // better do not return error for extended cam class
      }
    }

    lastUsedParams_.shutterMode = mode;
    getRWParameters().shutterMode = mode;

    if (mode == FeatureManual) {
      if (lastUsedParams_.shutter != shutter) {
        ctrl.id = V4L2_CID_EXPOSURE;
        if ((it=controls_.find(ctrl.id)) != controls_.end()) {
        
          ctrl.value = denormalize(shutter,(*it).second);
          
          if (xioctl(cameraHndl_,VIDIOC_S_CTRL,&ctrl) < 0) {
            lastUsedParams_.gainMode = FeatureNA;
            getRWParameters().gainMode = FeatureNA; 
            
            report("VIDIOC_S_CTRL:V4L2_CID_EXPOSURE");
            return false;
          } else {
            lastUsedParams_.shutter = shutter;
            getRWParameters().shutter = shutter;
          }
        }
      }
    }

    return true;

  }

  /*
   * Get shutter mode and value directly out of camera.
   *
   * @param mode Variable where the current shutter mode is to be 
   *              written.
   * @param shutter Variable where the shutter value is to be written.
   *
   * @return true if operation was successfull, false otherwise
   */ 
  bool v4l2::getShutter(eCamFeatureMode& mode,
                        float& shutter) const {


    if (initialized_) {

      controlsType::const_iterator it;
      struct v4l2_control ctrl;
      ctrl.id = V4L2_CID_EXPOSURE_AUTO;
      mode = FeatureNA; // first guess: not supported!
      
      if ((it=controls_.find(ctrl.id)) != controls_.end()) {
      
        if (xioctl(cameraHndl_,VIDIOC_G_CTRL,&ctrl) < 0) {
          mode = FeatureNA;
          
          // some error getting camera id
          report("VIDIOC_G_CTRL:V4L2_CID_EXPOSURE_AUTO");
          return false;
        } 

        if (driverName_ == "uvcvideo") { // special case for this driver
          // 8 -> 3
          // 4 -> 2
          // 2 -> 1
          // 1 -> 0
          switch(ctrl.value) {
            case 8:
              ctrl.value = 3;
              break;
            case 4:
              ctrl.value = 2;
              break;
            case 2:
              ctrl.value = 1;
              break;
            case 1:
              ctrl.value = 0;
              break;
            default:
              ctrl.value =  V4L2_EXPOSURE_AUTO;
          }
        }

        mode = (ctrl.value == V4L2_EXPOSURE_AUTO) ? FeatureAuto :
          FeatureManual;
      }
    

      ctrl.id = V4L2_CID_EXPOSURE;

      if (mode != FeatureAuto) { // seems we have manual shutter
        if ((it=controls_.find(ctrl.id)) != controls_.end()) {
          if (xioctl(cameraHndl_,VIDIOC_G_CTRL,&ctrl) < 0) {
            // some error getting camera id
            report("VIDIOC_G_CTRL:V4L2_CID_EXPOSURE");
            return false;
          }
          mode = FeatureManual; // the only available mode
          shutter = normalize(ctrl.value,it->second);
        }
      }
    }

    return true;
  }
  
  /*
   * Set gain control mode and register in camera directly.
   *
   * @param mode Camera desired mode (@see eCamFeatureMode)
   * @param gain If mode is manual, the value to be used.
   *
   * @return true if operation was successfull, false otherwise
   */
  bool v4l2::setGain(const eCamFeatureMode mode,
                     const float gain) {
    
    if (!initialized_ ) {
      if (!init()) {
        return false;
      }
    }

    controlsType::iterator it;
    struct v4l2_control ctrl;

    // first try if we have auto white balance
    ctrl.id = V4L2_CID_AUTOGAIN;
    ctrl.value = (mode == FeatureAuto) ? 1 : 0;

    if ((it=controls_.find(ctrl.id)) != controls_.end()) {
      if (xioctl(cameraHndl_,VIDIOC_S_CTRL,&ctrl) < 0) {
        lastUsedParams_.gainMode = FeatureNA;
        getRWParameters().gainMode = FeatureNA; 

        // some error getting camera id
        report("VIDIOC_G_CTRL:V4L2_CID_AUTO_WHITE_BALANCE");
        return false;
      }
    }

    lastUsedParams_.gainMode = mode;
    getRWParameters().gainMode = mode;

    if (mode == FeatureManual) {
      if (lastUsedParams_.gain != gain) {

        ctrl.id = V4L2_CID_GAIN;
        if ((it=controls_.find(ctrl.id)) != controls_.end()) {
        
          ctrl.value = denormalize(gain,(*it).second);
          
          if (xioctl(cameraHndl_,VIDIOC_S_CTRL,&ctrl) < 0) {
            lastUsedParams_.gainMode = FeatureNA;
            getRWParameters().gainMode = FeatureNA; 
            
            report("VIDIOC_S_CTRL:V4L2_CID_GAIN");
            return false;
          } else {
            lastUsedParams_.gain = gain;
            getRWParameters().gain = gain;
          }
        }
      }
    }

    return true;
  }

  /*
   * Get gain mode and value directly out of camera.
   *
   * @param mode Variable where the current gain mode is to be 
   *              written.
   * @param gain Variable where the gain value is to be written.
   *
   * @return true if operation was successfull, false otherwise
   */ 
  bool v4l2::getGain(eCamFeatureMode& mode,
                     float& gain) const {
    if (initialized_) {
      struct v4l2_control ctrl;
      mode = FeatureNA; // first guess: not supported!
      
      ctrl.id = V4L2_CID_AUTOGAIN;
      controlsType::const_iterator it = controls_.find(ctrl.id);

      if ((it=controls_.find(ctrl.id)) != controls_.end()) {
        // auto-mode supported
        if (xioctl(cameraHndl_,VIDIOC_G_CTRL,&ctrl) < 0) {
          // some error getting camera id
          report("VIDIOC_G_CTRL:V4L2_CID_GAIN_AUTO");
          return false;
        }
        mode = (ctrl.value != 0) ? FeatureAuto : FeatureManual;
      }
      
      if (mode != FeatureAuto) { 
        ctrl.id = V4L2_CID_GAIN;
        if ((it=controls_.find(ctrl.id)) == controls_.end()) {
          mode = FeatureNA; // indeed not available!
        } else {
          if (xioctl(cameraHndl_,VIDIOC_G_CTRL,&ctrl) < 0) {
            // some error getting camera id
            report("VIDIOC_G_CTRL:V4L2_CID_GAIN");
            return false;
          }
          gain = normalize(ctrl.value,it->second);
          mode = FeatureManual; 
        }
      }
      return true;
    }

    return false;
  }

  /*
   * Get current frame rate directly out of camera.
   * @return true if operation was successfull, false otherwise
   */
  bool v4l2::getFramerate(float& framerate) const {
    // FIXME!
    framerate=lastUsedParams_.framerate;
    return true;
  }

  /*
   * Set frame rate directly to of camera.
   * @return true if operation was successfull, false otherwise
   */
  // bool v4l2::setFramerateAndResolution(const float framerate,
  //                                      const ipoint& resolution) {
  //   return false;
  // }
  bool v4l2::setFramerateAndResolution(const float,
                                       const ipoint&) {
    setStatusString("Not implemented yet");
    return false;
  }

  /*
   * Get current resolution, posibly directly out of the camera
   */
  bool v4l2::getResolution(ipoint& resolution) const {
    // we use the VIDIOC_G_FMT to get information about settings in use
    struct v4l2_format format;
    format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (xioctl(cameraHndl_,VIDIOC_G_FMT,&format) < 0) {
      report("VIDIOC_G_FMT");
      return false;
    }
    
    // format.pix contains some valuable information
    resolution.set(format.fmt.pix.width,format.fmt.pix.height);

    return true;
  }


  /*
   * Set pan control mode and register in device directly.
   *
   * @param mode Device desired mode (@see eCamFeatureMode)
   * @param pan If mode is manual, the value to be used.
   *
   * @return true if operation was successfull, false otherwise
   */
  // bool v4l2::setPan(const eCamFeatureMode mode,
  //                   const float pan) {
  //   if (!panTilt_) {
  //     setStatusString("No pan-tilt unit available");
  //     return false;
  //   }

  //   return true;
  // }

  bool v4l2::setPan(const eCamFeatureMode,
                    const float) {
    if (!panTilt_) {
      setStatusString("No pan-tilt unit available");
      return false;
    }

    setStatusString("Not implemented yet");
    return false;
  }

  /*
   * Get pan mode and value directly out of device.
   *
   * @param mode Variable where the current pan mode is to be 
   *              written.
   * @param pan Variable where the pan value is to be written.
   *
   * @return true if operation was successfull, false otherwise
   */ 
  bool v4l2::getPan(eCamFeatureMode& mode,
                   float& pan) const {
    float tmp;
    eCamFeatureMode m;
    return getPanTilt(mode,pan,m,tmp);
  }

  /*
   * Set tilt control mode and register in device directly.
   *
   * @param mode Device desired mode (@see eCamFeatureMode)
   * @param tilt If mode is manual, the value to be used.
   *
   * @return true if operation was successfull, false otherwise
   */
  // bool v4l2::setTilt(const eCamFeatureMode mode,
  //                   const float tilt) {
  //   if (!panTilt_) {
  //     setStatusString("No pan-tilt unit available");
  //     return false;
  //   }

  //   return false;
  // }
  bool v4l2::setTilt(const eCamFeatureMode,
                    const float) {

    if (!panTilt_) {
      setStatusString("No pan-tilt unit available");
      return false;
    }

    setStatusString("Not implemented yet");
    return false;
  }

  /*
   * Get tilt mode and value directly out of device.
   *
   * @param mode Variable where the current tilt mode is to be 
   *              written.
   * @param tilt Variable where the tilt value is to be written.
   *
   * @return true if operation was successfull, false otherwise
   */ 
  bool v4l2::getTilt(eCamFeatureMode& mode,
                     float& tilt) const {
    float tmp;
    eCamFeatureMode m;
    return getPanTilt(mode,tmp,m,tilt);
  }

  
  /*
   * Set pan control mode and register in device directly.
   *
   * @param mode Device desired mode (@see eCamFeatureMode)
   * @param pan If mode is manual, the value to be used.
   *
   * @return true if operation was successfull, false otherwise
   */
  // bool v4l2::setPanTilt(const eCamFeatureMode mode,
  //                       const float pan,
  //                       const float tilt) {
  //   if (!panTilt_) {
  //     setStatusString("No pan-tilt unit available");
  //     return false;
  //   }

  //   return false;
  // }
  bool v4l2::setPanTilt(const eCamFeatureMode,
                        const float,
                        const float) {
    if (!panTilt_) {
      setStatusString("No pan-tilt unit available");
      return false;
    }

    setStatusString("Not implemented yet");
    return false;
  }

  /*
   * Get pan mode and value directly out of device.
   *
   * @param mode Variable where the current pan mode is to be 
   *              written.
   * @param pan Variable where the pan value is to be written.
   *
   * @return true if operation was successfull, false otherwise
   */ 
  bool v4l2::getPanTilt(eCamFeatureMode& panMode,
                       float& pan,
                       eCamFeatureMode& tiltMode,
                       float& tilt) const {
    if (!panTilt_) {
      panMode=tiltMode=FeatureNA;
      pan=tilt=0.0f;
      return true;
    }

    return true;
  }

  bool v4l2::panTiltReset() {
    if (!panTilt_) {
      return true; // everything is ok, we don't even have a unit!
    }

    return true;
  }

  /*
   * Set iris control mode and register in device directly.
   *
   * @param mode Device desired mode (@see eCamFeatureMode)
   * @param iris If mode is manual, the value to be used.
   *
   * @return true if operation was successfull, false otherwise
   */
  // bool v4l2::setIris(const eCamFeatureMode mode,
  //                    const float iris) {
  //   return false;
  // }

  bool v4l2::setIris(const eCamFeatureMode,
                     const float) {
    setStatusString("Not implemented yet");
    return false;
  }
  
  /*
   * Get iris mode and value directly out of device.
   *
   * @param mode Variable where the current iris mode is to be 
   *              written.
   * @param iris Variable where the iris value is to be written.
   *
   * @return true if operation was successfull, false otherwise
   */ 
  // bool v4l2::getIris(eCamFeatureMode& mode,
  //                    float& iris) const {
  //   return false;
  // }
  bool v4l2::getIris(eCamFeatureMode&,
                     float&) const {
    setStatusString("Not implemented yet");
    return false;
  }
  
  /*
   * Set focus control mode and register in device directly.
   *
   * @param mode Device desired mode (@see eCamFeatureMode)
   * @param focus If mode is manual, the value to be used.
   *
   * @return true if operation was successfull, false otherwise
   */
  // bool v4l2::setFocus(const eCamFeatureMode mode,
  //                     const float focus) {
  //   return false;
  // }
  bool v4l2::setFocus(const eCamFeatureMode,
                      const float) {
    setStatusString("Not implemented yet");
    return false;
  }
  
  /*
   * Get focus mode and value directly out of device.
   *
   * @param mode Variable where the current focus mode is to be 
   *              written.
   * @param focus Variable where the focus value is to be written.
   *
   * @return true if operation was successfull, false otherwise
   */ 
  // bool v4l2::getFocus(eCamFeatureMode& mode,
  //                     float& focus) const {
  //   return false;
  // }
  bool v4l2::getFocus(eCamFeatureMode&,
                      float&) const {
    setStatusString("Not implemented yet");
    return false;
  }
  
  /*
   * Set zoom control mode and register in device directly.
   *
   * @param mode Device desired mode (@see eCamFeatureMode)
   * @param zoom If mode is manual, the value to be used.
   *
   * @return true if operation was successfull, false otherwise
   */
  // bool v4l2::setZoom(const eCamFeatureMode mode,
  //                    const float zoom) {
  //   return false;
  // }
  bool v4l2::setZoom(const eCamFeatureMode,
                     const float) {
    setStatusString("Not implemented yet");
    return false;
  }
  
  /*
   * Get zoom mode and value directly out of device.
   *
   * @param mode Variable where the current zoom mode is to be 
   *              written.
   * @param zoom Variable where the zoom value is to be written.
   *
   * @return true if operation was successfull, false otherwise
   */ 
  // bool v4l2::getZoom(eCamFeatureMode& mode,
  //                    float& zoom) const {
  //   return false;
  // }
  bool v4l2::getZoom(eCamFeatureMode&,
                     float&) const {
    setStatusString("Not implemented yet");
    return false;
  }
  
  /*
   * Set opticalFilter control mode and register in device directly.
   *
   * Depending on the color space used by the device (RGB or YUV), the
   * color gains are applied to UV or to BR.
   *
   * @param mode Device desired mode (@see eCamFeatureMode)
   * @param opticalFilter value to set
   *
   * @return true if operation was successfull, false otherwise
   */
  // bool v4l2::setOpticalFilter(const eCamFeatureMode mode,
  //                             const float opticalFilter) {
  //   return false;
  // }
  bool v4l2::setOpticalFilter(const eCamFeatureMode,
                              const float) {
    setStatusString("Not implemented yet");
    return false;
  }
  
  /*
   * Get opticalFilter mode and value directly out of device.
   *
   * @param mode Variable where the current opticalFilter mode is to be 
   *              written.
   * @param opticalFilter variable to store the value obtained from the
   *                      device
   *
   * @return true if operation was successfull, false otherwise
   */ 
  // bool v4l2::getOpticalFilter(eCamFeatureMode& mode,
  //                             float& opticalFilter) const {
  //   return false;
  // }
  bool v4l2::getOpticalFilter(eCamFeatureMode&,
                              float&) const {
    setStatusString("Not implemented yet");
    return false;
  }
  
  /*
   * Set vertical flip.
   *
   * @param flip If set to false, the flip-feature will be deactivated.
   */
  bool v4l2::setVerticalFlip(const bool verticalFlip) {
    if (!initialized_ ) {
      if (!init()) {
        return false;
      }
    }

    controlsType::iterator it;
    struct v4l2_control ctrl;
    ctrl.id = V4L2_CID_VFLIP;

    if ((it=controls_.find(ctrl.id)) == controls_.end()) {
      setStatusString("VerticalFlip control not supported by your camera.");
      lastUsedParams_.verticalFlipMode = FeatureNA;
      getRWParameters().verticalFlipMode = FeatureNA;
      return false;
    }

    ctrl.value = verticalFlip ? 1 : 0;
    
    if (xioctl(cameraHndl_,VIDIOC_S_CTRL,&ctrl) < 0) {
      lastUsedParams_.verticalFlipMode = FeatureNA;
      getRWParameters().verticalFlipMode = FeatureNA;
      report("VIDIOC_S_CTRL:V4L2_CID_VFLIP");
      return false;
    } else {
      lastUsedParams_.verticalFlip = verticalFlip;
      lastUsedParams_.verticalFlipMode = FeatureManual;
      getRWParameters().verticalFlip = verticalFlip;
      getRWParameters().verticalFlipMode = FeatureManual; 
    }
    
    return true;
  }

  bool v4l2::getVerticalFlip(bool& flip) const {
    if (initialized_) {
      struct v4l2_control ctrl;
      
      ctrl.id = V4L2_CID_VFLIP;
      controlsType::const_iterator it = controls_.find(ctrl.id);

      if (it == controls_.end()) {
        flip = false;
        return true;
      }

      if (xioctl(cameraHndl_,VIDIOC_G_CTRL,&ctrl) < 0) {
        // some error getting camera id
        report("VIDIOC_G_CTRL:V4L2_CID_VFLIP");
        return false;
      }
      
      flip = (ctrl.value != 0);

      return true;
    }
    return false;
  }

  /*
   * Set horizontal flip.
   *
   * @param flip If set to false, the flip-feature will be deactivated.
   */
  bool v4l2::setHorizontalFlip(const bool horizontalFlip) {
    if (!initialized_ ) {
      if (!init()) {
        return false;
      }
    }

    controlsType::iterator it;
    struct v4l2_control ctrl;
    ctrl.id = V4L2_CID_HFLIP;

    if ((it=controls_.find(ctrl.id)) == controls_.end()) {
      setStatusString("HorizontalFlip control not supported by your camera.");
      lastUsedParams_.horizontalFlipMode = FeatureNA;
      getRWParameters().horizontalFlipMode = FeatureNA;
      return false;
    }

    ctrl.value = horizontalFlip ? 1 : 0;
    
    if (xioctl(cameraHndl_,VIDIOC_S_CTRL,&ctrl) < 0) {
      lastUsedParams_.horizontalFlipMode = FeatureNA;
      getRWParameters().horizontalFlipMode = FeatureNA;
      
      report("VIDIOC_S_CTRL:V4L2_CID_HFLIP");
      return false;
    } else {
      lastUsedParams_.horizontalFlip = horizontalFlip;
      lastUsedParams_.horizontalFlipMode = FeatureManual;
      getRWParameters().horizontalFlip = horizontalFlip;
      getRWParameters().horizontalFlipMode = FeatureManual; 
    }

    return true;
  }

  bool v4l2::getHorizontalFlip(bool& flip) const {
    if (initialized_) {
      struct v4l2_control ctrl;

      ctrl.id = V4L2_CID_HFLIP;
      controlsType::const_iterator it = controls_.find(ctrl.id);

      if (it == controls_.end()) {
        flip = false;
        return true;
      }

      if (xioctl(cameraHndl_,VIDIOC_G_CTRL,&ctrl) < 0) {
        // some error getting camera id
        report("VIDIOC_G_CTRL:V4L2_CID_HFLIP");
        return false;
      }
      
      flip = (ctrl.value != 0);

      return true;
    }
    return false;
  }

  /*
   * Set power line filter frequency.
   */
  bool v4l2::setPowerLineFilter(const ePowerLineFilter powerLineFilter) {
    if (!initialized_ ) {
      if (!init()) {
        return false;
      }
    }

    controlsType::iterator it;
    struct v4l2_control ctrl;
    ctrl.id = V4L2_CID_POWER_LINE_FREQUENCY;

    if ((it=controls_.find(ctrl.id))==controls_.end()) {
      lastUsedParams_.powerLineFilterMode = FeatureNA;
      getRWParameters().powerLineFilterMode = FeatureNA; 
      setStatusString("PowerLineFilter control not supported by your camera.");
      return false;
    }

    if (lastUsedParams_.powerLineFilter != powerLineFilter) {

      ctrl.value = powerLineFilter;

      if (xioctl(cameraHndl_,VIDIOC_S_CTRL,&ctrl) < 0) {
        lastUsedParams_.powerLineFilterMode = FeatureNA;
        getRWParameters().powerLineFilterMode = FeatureNA; 

        report("VIDIOC_S_CTRL:V4L2_CID_POWER_LINE_FREQUENCY");
        return false;
      } else {
        lastUsedParams_.powerLineFilter = powerLineFilter;
        lastUsedParams_.powerLineFilterMode = FeatureManual;
        getRWParameters().powerLineFilter = powerLineFilter;
        getRWParameters().powerLineFilterMode = FeatureManual; 
      }

    }

    return true;
  }
  
  /*
   * Get power line filter frequency.
   */
  bool v4l2::getPowerLineFilter(ePowerLineFilter& powerLineFilter) const {
    if (initialized_) {
      struct v4l2_control ctrl;
      
      ctrl.id = V4L2_CID_POWER_LINE_FREQUENCY;
      controlsType::const_iterator it = controls_.find(ctrl.id);

      if (it == controls_.end()) {
        powerLineFilter = Disabled;
        return true;
      }

      if (xioctl(cameraHndl_,VIDIOC_G_CTRL,&ctrl) < 0) {
        // some error getting camera id
        report("VIDIOC_G_CTRL:V4L2_CID_POWER_LINE_FREQUENCY");
        return false;
      }
      
      powerLineFilter = static_cast<ePowerLineFilter>(ctrl.value);

      return true;
    }
    return false;
  }
  
  /*
   * Set white balance temperature
   */
  bool v4l2::setWhiteBalanceTemperature(const int whiteBalanceTemperature) {
    if (!initialized_ ) {
      if (!init()) {
        return false;
      }
    }

    controlsType::iterator it;
    struct v4l2_control ctrl;
    ctrl.id = V4L2_CID_WHITE_BALANCE_TEMPERATURE;

    if ((it=controls_.find(ctrl.id)) == controls_.end()) {
      setStatusString("WhiteBalanceTemperature control not supported.");

      lastUsedParams_.whiteBalanceTemperatureMode = FeatureNA;
      getRWParameters().whiteBalanceTemperatureMode = FeatureNA; 

      return false;
    }

    if (lastUsedParams_.whiteBalanceTemperature != whiteBalanceTemperature) {

      ctrl.value = within(whiteBalanceTemperature,
                          it->second.minimum,
                          it->second.maximum);

      if (xioctl(cameraHndl_,VIDIOC_S_CTRL,&ctrl) < 0) {
        lastUsedParams_.whiteBalanceTemperatureMode = FeatureNA;
        getRWParameters().whiteBalanceTemperatureMode = FeatureNA; 

        report("VIDIOC_S_CTRL:V4L2_CID_WHITE_BALANCE_TEMPERATURE");
        return false;
      } else {
        lastUsedParams_.whiteBalanceTemperature = whiteBalanceTemperature;
        lastUsedParams_.whiteBalanceTemperatureMode = FeatureManual;
        getRWParameters().whiteBalanceTemperature = whiteBalanceTemperature;
        getRWParameters().whiteBalanceTemperatureMode = FeatureManual; 
      }

    }

    return true;
  }
  
  /*
   * Get white balance temperature
   */
  bool v4l2::getWhiteBalanceTemperature(int& whiteBalanceTemperature) const {
    if (initialized_) {
      struct v4l2_control ctrl;
      
      ctrl.id = V4L2_CID_WHITE_BALANCE_TEMPERATURE;
      controlsType::const_iterator it = controls_.find(ctrl.id);

      if (it == controls_.end()) {
        whiteBalanceTemperature = 0;
        return true;
      }

      if (xioctl(cameraHndl_,VIDIOC_G_CTRL,&ctrl) < 0) {
        // some error getting camera id
        report("VIDIOC_G_CTRL:V4L2_CID_WHITE_BALANCE_TEMPERATURE");
        return false;
      }
      
      whiteBalanceTemperature = ctrl.value;

      return true;
    }
    return false;
  }
  
  /*
   * Set backlight compensation.
   */
  bool v4l2::setBacklightCompensation(const int backlightCompensation) {
    if (!initialized_ ) {
      if (!init()) {
        return false;
      }
    }

    controlsType::iterator it;
    struct v4l2_control ctrl;
    ctrl.id = V4L2_CID_BACKLIGHT_COMPENSATION;

    if ((it=controls_.find(ctrl.id)) == controls_.end()) {
      setStatusString("BacklightCompensation control not supported.");

      lastUsedParams_.backlightCompensationMode = FeatureNA;
      getRWParameters().backlightCompensationMode = FeatureNA; 

      return false;
    }
    
    if (lastUsedParams_.backlightCompensation != backlightCompensation) {

      ctrl.value = within(backlightCompensation,
                          it->second.minimum,
                          it->second.maximum);

      if (xioctl(cameraHndl_,VIDIOC_S_CTRL,&ctrl) < 0) {
        lastUsedParams_.backlightCompensationMode = FeatureNA;
        getRWParameters().backlightCompensationMode = FeatureNA;

        report("VIDIOC_S_CTRL:V4L2_CID_BACKLIGHT_COMPENSATION");
        return false;
      } else {
        lastUsedParams_.backlightCompensation = backlightCompensation;
        lastUsedParams_.backlightCompensationMode = FeatureManual;
        getRWParameters().backlightCompensation = backlightCompensation;
        getRWParameters().backlightCompensationMode = FeatureManual; 
      }

    }

    return true;
  }
  
  /*
   * Get backlight compensation.
   */
  bool v4l2::getBacklightCompensation(int& backlightCompensation) const {
    if (initialized_) {
      struct v4l2_control ctrl;
      
      ctrl.id = V4L2_CID_BACKLIGHT_COMPENSATION;
      controlsType::const_iterator it = controls_.find(ctrl.id);

      if (it == controls_.end()) {
        backlightCompensation = 0;
        return true;
      }

      if (xioctl(cameraHndl_,VIDIOC_G_CTRL,&ctrl) < 0) {
        // some error getting camera id
        report("VIDIOC_G_CTRL:V4L2_CID_BACKLIGHT_COMPENSATION");
        return false;
      }
      
      backlightCompensation = ctrl.value;

      return true;
    }
    return false;
  }
  
  
  /*
   * Check if a particular feature is available in the current active
   * device.
   *
   * You have to ensure that a device is already active (see
   * isActive()).
   *
   * \param featureId The identification of the feature expects one of the
   *                  - FeaturePan
   *                  - FeatureTilt
   *                  - FeatureZoom
   *                  - FeatureOpticalFilter
   *
   * This pure virtual method may have other possible values as
   */
  bool v4l2::isFeatureAvailable(const unsigned int featureId) const {
    switch(featureId) {
    case FeatureBrightness:
      return (controls_.find(V4L2_CID_BRIGHTNESS) != controls_.end());
      break;
    case FeatureExposure:
      return (controls_.find(V4L2_CID_CONTRAST) != controls_.end());
      break;
    case FeatureSharpness:
      return (controls_.find(V4L2_CID_SHARPNESS) != controls_.end());
      break;
    case FeatureWhiteBalance:
      return ((controls_.find(V4L2_CID_AUTO_WHITE_BALANCE)!=controls_.end()) ||
              (controls_.find(V4L2_CID_RED_BALANCE)!=controls_.end()) ||
              (controls_.find(V4L2_CID_BLUE_BALANCE)!=controls_.end()));
      break;
    case FeatureHue:
      return (controls_.find(V4L2_CID_HUE) != controls_.end());
      break;
    case FeatureSaturation:
      return (controls_.find(V4L2_CID_SATURATION) != controls_.end());
      break;
    case FeatureGamma:
      return (controls_.find(V4L2_CID_GAMMA) != controls_.end());
      break;
    case FeatureShutter:
      return (controls_.find(V4L2_CID_EXPOSURE) != controls_.end());
      break;
    case FeatureGain:
      return ((controls_.find(V4L2_CID_GAIN) != controls_.end()) ||
              (controls_.find(V4L2_CID_AUTOGAIN) != controls_.end()));
      break;
    case FeatureVerticalFlip:
      return (controls_.find(V4L2_CID_VFLIP) != controls_.end());
      break;
    case FeatureHorizontalFlip:
      return (controls_.find(V4L2_CID_HFLIP) != controls_.end());
      break;
    case FeaturePowerLineFilter:
      return (controls_.find(V4L2_CID_POWER_LINE_FREQUENCY)!=controls_.end());
      break;
    case FeatureWhiteBalanceTemperature:
      return (controls_.find(V4L2_CID_WHITE_BALANCE_TEMPERATURE) !=
              controls_.end());
      break;
    case FeatureBacklightCompensation:
      return (controls_.find(V4L2_CID_BACKLIGHT_COMPENSATION) != 
              controls_.end());
      break;
    case FeaturePan:
    case FeatureTilt:
      return false; // TODO
      break;
    default:
      return false;
    }
    return false;
  }

  /*
   * Check if a feature is available and is read-out capable.
   *
   * Some devices have a feature, but you cannot read the state in
   * which that feature is set.  This method returns true if the
   * device can provide information on the feature state/mode.
   *
   * \param featureId identification for the feature to be queried.
   *                  (see device::isFeatureAvailable() for a list of
   *                   valid values)
   * \return \c true if feature is read-out capable or \c false if it is not
   *         available at all, or if it is not readable.
   */
  bool v4l2::isFeatureReadable(const unsigned int featureId) const {
    // in V4L2 if the feature is availabe, it is readable.
    return isFeatureAvailable(featureId);
  }

  /*
   * Check for availability of feature modes
   *
   * A given feature (see the list of valid identifiers in
   * lti::device::isFeatureAvailable() ) can have one of five modes:
   * - Not available: the feature does not exist in the current device
   * - Off mode: the feature is deactivated (some features cannot be
   *             deactivated).
   * - Auto mode: the feature is automatically adjusted 
   * - Manual mode: the user controls the values a feature can get 
   * - One-Push mode: the feature is automatically adjusted but only once.
   *
   * This method asks the active device which modes are supported and
   * returns a bitwise combination of lti::eCamFeatureMode constants.
   *
   * You have to ensure that a device is active (see device::isActive()).
   *
   * \param featId identification for the feature to be modified
   *                  (see device::isFeatureAvailable() for a list of
   *                   valid values)
   *
   */
  unsigned int v4l2::checkFeatureModes(const unsigned int featureId) const {
    if (isFeatureAvailable(featureId)) {
      switch(featureId) {
      case FeatureBrightness:
      case FeatureExposure:
      case FeatureSaturation:
      case FeatureGamma:
      case FeatureSharpness:
      case FeatureVerticalFlip:
      case FeatureHorizontalFlip:
      case FeaturePowerLineFilter:
      case FeatureWhiteBalanceTemperature:
      case FeatureBacklightCompensation:
        return FeatureManual;
        break;
      case FeaturePan:
      case FeatureTilt:
        return (panTilt_)?FeatureManual:FeatureNA;
        break;
      case FeatureHue:
        if (controls_.find(V4L2_CID_HUE_AUTO) != controls_.end()) {
          return static_cast<unsigned int>(FeatureManual | FeatureAuto);
        } else {
          return FeatureManual;
        }
        break;
      case FeatureWhiteBalance:
        if (controls_.find(V4L2_CID_AUTO_WHITE_BALANCE) != controls_.end()) {
          return static_cast<unsigned int>(FeatureManual | FeatureAuto);
        } else {
          return FeatureManual;
        }
      case FeatureShutter: {
        unsigned int tmp = 0;
        if (controls_.find(V4L2_CID_EXPOSURE_AUTO) != controls_.end()) {
          tmp |= FeatureAuto;
        } 
        if (controls_.find(V4L2_CID_EXPOSURE_ABSOLUTE) != controls_.end()) {
          tmp |= FeatureAbsolute;
        } 
        tmp |= FeatureManual;
        return tmp;
      } break;
      case FeatureGain:
        if (controls_.find(V4L2_CID_AUTOGAIN) != controls_.end()) {
          return static_cast<unsigned int>(FeatureManual | FeatureAuto);
        } else {
          return FeatureManual;
        }
        break;
      default:
        return FeatureNA;
      }
    }
    return FeatureNA;
  }  
    
  /*
   * Get feature mode of the device for one specific feature.
   *
   * @return \c true if feature mode could be successfully read, or \c false
   *         if this was not possible (maybe the device does not support
   *         the given feature at all).
   */
  bool v4l2::getFeatureMode(const unsigned int featureId,
                            eCamFeatureMode& mode) const {
    float u,v; // temp variables;

    if (isFeatureAvailable(featureId)) {
      switch(featureId) {
      case FeatureBrightness:
      case FeatureExposure:
      case FeatureSaturation:
      case FeatureGamma:
      case FeatureSharpness:
      case FeatureVerticalFlip:
      case FeatureHorizontalFlip:
      case FeaturePowerLineFilter:
      case FeatureWhiteBalanceTemperature:
      case FeatureBacklightCompensation:
        mode = FeatureManual;
        return true;
        break;
      case FeaturePan:
      case FeatureTilt:
        mode = (panTilt_)?FeatureManual:FeatureNA;
        return true;
        break;
      case FeatureWhiteBalance:
        return getWhiteBalance(mode,u,v);
        break;
      case FeatureHue:
        return getHue(mode,u);
        break;
      case FeatureShutter: 
        return getShutter(mode,u);
        break;
      case FeatureGain:
        return getGain(mode,u);
        break;
      default:
        return false;
      }
    }

    mode = FeatureNA;
    return false;
  }

  /*
   * Set operation mode of a particular feature of the device.
   *
   * @return \c true if feature mode could be successfully set, or \c false
   *         if this was not possible (maybe the device does not support
   *         the given mode for the feature.
   */
  bool v4l2::setFeatureMode(const unsigned int featureId,
                            const eCamFeatureMode mode) {
    parameters& par = getRWParameters();
    eCamFeatureMode m;
    float u,v;

    if (!isFeatureAvailable(featureId)) {
      return false;
    }

    switch(featureId) {
      case FeatureBrightness:
        if (mode == FeatureManual) {
          par.brightnessMode = FeatureManual;
          return true;
        }
        break;
      case FeatureExposure:
        if (mode == FeatureManual) {
          par.exposureMode = FeatureManual;
          return true;
        }
        break;
      case FeatureSaturation:
        if (mode == FeatureManual) {
          par.saturationMode = FeatureManual;
          return true;
        }
        break;
      case FeatureGamma:
        if (mode == FeatureManual) {
          par.gammaMode = FeatureManual;
          return true;
        }
        break;
      case FeatureSharpness: 
        if (mode == FeatureManual) {
          par.sharpnessMode = FeatureManual;
          return true;
        }
        break;      
      case FeatureVerticalFlip: 
        if (mode == FeatureManual) {
          par.verticalFlipMode = FeatureManual;
          return true;
        }
        break;      
      case FeatureHorizontalFlip: 
        if (mode == FeatureManual) {
          par.horizontalFlipMode = FeatureManual;
          return true;
        }
        break;      
      case FeaturePowerLineFilter: 
        if (mode == FeatureManual) {
          par.powerLineFilterMode = FeatureManual;
          return true;
        }
        break;      
      case FeatureWhiteBalanceTemperature: 
        if (mode == FeatureManual) {
          par.whiteBalanceTemperatureMode = FeatureManual;
          return true;
        }
        break;      
      case FeatureBacklightCompensation: 
        if (mode == FeatureManual) {
          par.backlightCompensationMode = FeatureManual;
          return true;
        }
        break;      


      case FeaturePan:
        if (panTilt_ && (mode == FeatureManual)) {
          par.panMode = FeatureManual;
          return true;
        }
        break;
      case FeatureTilt:
        if (panTilt_ && (mode == FeatureManual)) {
          par.tiltMode = FeatureManual;
          return true;
        }
        break;

      case FeatureWhiteBalance:
        if (getWhiteBalance(m,u,v)) {
          return setWhiteBalance(mode,u,v);
        }
        break;
      case FeatureHue:
        if (getHue(m,u)) {
          return setHue(mode,u);
        }
        break;
      case FeatureShutter:
        if (getShutter(m,u)) {
          return setShutter(mode,u);
        }
        break;
      case FeatureGain:
        if (getGain(m,v)) {
          return setGain(mode,v);
        }
        break;
        
    default:
      return false;
    }

    return false;
  }

  /*
   * Set control mode and value of a particular feature.
   *
   * This method does not support those features that require two values,
   * like white balance.
   *
   * @param featureId identification for the feature to be modified
   *                  (see device::isFeatureAvailable() for a list of
   *                   valid values)
   * @param mode Device desired mode (@see eCamFeatureMode)
   * @param value If mode is manual, the value to be used.
   *
   * @return true if operation was successfull, false otherwise
   */
  bool v4l2::setFeatureValue(const unsigned int featureId,
                             const eCamFeatureMode mode,
                             const float value) {

    switch(featureId) {
      case FeatureBrightness:
        return setBrightness(mode,value);
        break;
      case FeatureExposure:
        return setContrast(mode,value);
        break;
      case FeatureHue:
        return setHue(mode,value);
        break;
      case FeatureSaturation:
        return setSaturation(mode,value);
        break;
      case FeatureGamma:
        return setGamma(mode,value);
        break;
      case FeatureVerticalFlip:
        return setVerticalFlip(value!=0.0f);
        break;
      case FeatureHorizontalFlip:
        return setHorizontalFlip(value!=0.0f);
        break;
      case FeaturePowerLineFilter:
        return setPowerLineFilter(static_cast<ePowerLineFilter>(value));
        break;
      case FeatureWhiteBalanceTemperature:
        return setWhiteBalanceTemperature(static_cast<int>(value));
        break;
      case FeatureBacklightCompensation:
        return setBacklightCompensation(static_cast<int>(value));
        break;
        
      case FeaturePan:
        return setPan(mode,value);
        break;
      case FeatureTilt:
        return setTilt(mode,value);
        break;

      case FeatureSharpness:
        return setSharpness(mode,value);
        break;      
      case FeatureWhiteBalance:
        // not set-able with this method!
        return false;
        break;

      case FeatureShutter:
        return setShutter(mode,value);
        break;      
      case FeatureGain:
        return setGain(mode,value);
        break;
      default:
        return false;
    }
    return false;
  }
    
    /*
     * Get control mode and value of a particular feature.
     *
     * This method does not support those features that require two values,
     * like white balance.
     *
     * @param featureId identification for the feature to be queried.
     * @param mode Mode in which the feature is being used
     * @param value if mode is manual, the value to be used.
     *
     * @return \c true if operation was successfull, \c false otherwise (for
     *         example, if the feature is not supported by the device).
     */
  bool v4l2::getFeatureValue(const unsigned int featureId,
                            eCamFeatureMode& mode,
                            float& value) const {
    switch(featureId) {
    case FeatureBrightness:
      return getBrightness(mode,value);
      break;
    case FeatureExposure:
      return getContrast(mode,value);
      break;
    case FeatureHue:
      return getHue(mode,value);
      break;
    case FeatureSaturation:
      return getSaturation(mode,value);
      break;
    case FeatureGamma:
      return getGamma(mode,value);
      break;
    case FeaturePan:
      return getPan(mode,value);
      break;
    case FeatureTilt:
      return getTilt(mode,value);
      break;
    case FeatureSharpness:
      return getSharpness(mode,value);
      break;      
    case FeatureWhiteBalance:
      // not getable with this method!
      return false;
      break;
    case FeatureShutter:
      return getShutter(mode,value);
      break;      
    case FeatureGain:
      return getGain(mode,value);
      break;
      case FeatureVerticalFlip: {
        bool b;
        if (getVerticalFlip(b)) {
          value = b ? 1.0f : 0.0f;
          return true;
        } else {
          return false;
        }
      } break;
      case FeatureHorizontalFlip: {
        bool b;
        if (getHorizontalFlip(b)) {
          value = b ? 1.0f : 0.0f;
          return true;
        } else {
          return false;
        }
      } break;
      case FeaturePowerLineFilter: {
        ePowerLineFilter p;
        if (getPowerLineFilter(p)) {
          value = static_cast<float>(p);
          return true;
        } else {
          return false;
        }
      } break;
      case FeatureWhiteBalanceTemperature: {
        int i;
        if (getWhiteBalanceTemperature(i)) {
          value = static_cast<float>(i);
          return true;
        } else {
          return false;
        }
      } break;
      case FeatureBacklightCompensation: {
        int i;
        if (getBacklightCompensation(i)) {
          value = static_cast<float>(i);
          return true;
        } else {
          return false;
        }
      } break;
      default:
      return false;
    }
    return false;
  }

    /*
     * Check the valid value range for a specified feature.
     *
     * You have to ensure that there is an active device before calling this
     * method. (see device::isActive() )
     *
     * @param featureId identification for the feature to be modified
     *                  (see device::isFeatureAvailable() for a list of
     *                   valid values)
     * @param minValue minimum value accepted for the specified feature.
     * @param maxValue maximum value accepted for the specified feature.
     *
     * @return \c true if successful or \c false otherwise.
     */
  bool v4l2::checkFeatureValueRange(const unsigned int featureId,
                                    float& minValue,
                                    float& maxValue) const {
    controlsType::const_iterator it = controls_.find(featureId);

    if (it==controls_.end()) {
      return false;
    }
    
    minValue = static_cast<float>(it->second.minimum);
    maxValue = static_cast<float>(it->second.maximum);
    
    return true;
  }

  v4l2::ePixelFormat v4l2::getBestFormat(const ePixelFormat pxFrm) const {
    // check that there are available formats
    if (pixelFormats_.empty()) {
      return Invalid;
    } 

    // if Auto, then choose the first available one (this may change in the
    // future to take the best one for the LTI-Lib
    if ( (pxFrm != Auto) && (pxFrm != Raw) ) {
      // first, try to find if the format is directly supported
      unsigned int i;
      for (i=0;i<pixelFormats_.size();++i) {
        if (ePixelFormat(pixelFormats_[i].pixelformat) == pxFrm) {
          return pxFrm;
        }
      }
    }

    // let's initialize a map with unique weights (just once for the whole lib)
    static std::map<ePixelFormat,int> weights;
    if (weights.empty()) {
      // this ordering is completely arbitrary, and may be changed
      // as long as we measure the time it takes to convert it to
      // a lti::image
      int counter(0);

      // The order is given by the position on the following declarations
      // the lower the counter value, the better the format.  There are
      // so many formats that we will have to implement them slowly in time

      /*
       * RGB-8-8-8-8    (32 bits per pixel)
       */
      weights[RGB32] = counter++;

      /*
       * BGR-8-8-8-8    (32 bits per pixel)
       */
      weights[BGR32] = counter++;

      /*
       * RGB-8-8-8      (24 bits per pixel)
       */
      weights[RGB24] = counter++;

      /*
       * BGR-8-8-8      (24 bits per pixel)
       */
      weights[BGR24] = counter++;
     
      /*
       * RGB-3-3-2 (8 bits per pixel)
       */
      weights[RGB332] = counter++;

      /*
       * RGB-5-6-5      (16 bits per pixel)
       */
      weights[RGB565] = counter++;

      /*
       * RGB-5-5-5      (16 bits per pixel)
       */
      weights[RGB555] = counter++;

      /*
       * RGB-5-5-5 BE   (16 bits per pixel)
       */
      weights[RGB555X] = counter++;

      /*
       * RGB-5-6-5 BE   (16 bits per pixel)
       */
      weights[RGB565X] = counter++;

      /*
       * xxxxrrrr ggggbbbb  (16 bits per pixel)
       */
      weights[RGB444] = counter++;

      /*
       * BGBG.. GRGR..  (8 bits per pixel)
       * see http://www.siliconimaging.com/RGB%20Bayer.htm
       */
      weights[SBGGR8] = counter++;

      /*
       * BGBG.. GRGR..  (16 bits per pixel)
       * see http://www.siliconimaging.com/RGB%20Bayer.htm
       */
      weights[SBGGR16] = counter++;

      /*
       * Greyscale      (8 bits per pixel)
       */
      weights[GREY] = counter++;

      /*
       * Greyscale      (16 bits per pixel)
       */
      weights[Y16] = counter++;

      /*
       * YVU 4:1:0      (9 bits per pixel)
       */
      weights[YVU410] = counter++;

      /*
       * YVU 4:2:0      (12 bits per pixel)
       */
      weights[YVU420] = counter++;

      /*
       * YUV 4:2:2      (16 bits per pixel)
       */
      weights[YUYV] = counter++;

      /*
       * YUV 4:2:2      (16 bits per pixel)
       */
      weights[UYVY] = counter++;

      /*
       * YVU422 planar  (16 bits per pixel)
       */
      weights[YUV422P] = counter++;

      /*
       * YVU411 planar  (16 bits per pixel)
       */
      weights[YUV411P] = counter++;

      /*
       * YUV 4:1:1      (12 bits per pixel)
       */
      weights[Y41P] = counter++;

      /*
       * xxxxyyyy uuuuvvvv  (16 bits per pixel)
       */
      weights[YUV444] = counter++;

      /*
       * YUV-5-5-5      (16 bits per pixel)
       */
      weights[YUV555] = counter++;

      /*
       * YUV-5-6-5      (16 bits per pixel)
       */
      weights[YUV565] = counter++;

      /*
       * YUV-8-8-8-8    (32 bits per pixel)
       */
      weights[YUV32] = counter++;

      /*
       * Y/CbCr 4:2:0   (12 bits per pixel)
       *
       * two planes -- one Y, one Cr + Cb interleaved
       */
      weights[NV12] = counter++;
      /*
       * Y/CrCb 4:2:0   (12 bits per pixel)
       */
      weights[NV21] = counter++;

      /*
       * YUV 4:1:0      (9 bits per pixel)
       *
       * (not defined in the V4L2 specification)
       */
      weights[YUV410] = counter++;

      /*
       * YUV 4:2:0      (12 bits per pixel)
       *
       * (not defined in the V4L2 specification)
       */
      weights[YUV420] = counter++;

      /*
       * YUV 4:2:2      (16 bits per pixel)
       *
       * (not defined in the V4L2 specification)
       */
      weights[YYUV] = counter++;

      /*
       * 8-bit palette  (8 bits per pixel)
       */
      weights[PAL8] = counter++;

      /*
       * 8-bit color    (8 bits per pixel)
       *
       * (not defined in the V4L2 specification)
       */
      weights[HI240] = counter++;

      /*
       * YUV 4:2:0 16x16 macroblocks  (8 bits per pixel)
       *
       * (not defined in the V4L2 specification)
       */
      weights[HM12] = counter++;

      /*
       * Motion-JPEG    (compressed format)
       */
      weights[MJPEG] = counter++;
      /*
       * JFIF JPEG      (compressed format)
       */
      weights[JPEG] = counter++;
      /*
       * Unknown (compressed format)
       */
      weights[DV] = counter++;
      /*
       * MPEG-1/2/4     (compressed format)
       */
      weights[MPEG] = counter++;

      /*
       * Winnov hw compress  (Vendor-specific formatl)
       */
      weights[WNVA] = counter++;

      /*
       * SN9C10x compression  (Vendor-specific format)
       */
      weights[SN9C10X] = counter++;

      /*
       * pwc older webcam  (Vendor-specific format)
       */
      weights[PWC1] = counter++;

      /*
       * pwc newer webcam  (Vendor-specific format)
       */
      weights[PWC2] = counter++;

      /*
       * ET61X251 compression  (Vendor-specific format)
       */
      weights[ET61X251] = counter++;
    }

    std::map<ePixelFormat,int>::const_iterator it,best;
    best = weights.find(ePixelFormat(pixelFormats_[0].pixelformat));
    
    for (unsigned int i=1;i<pixelFormats_.size();++i) {
      it = weights.find(ePixelFormat(pixelFormats_[i].pixelformat));
      if ( (it != weights.end()) &&
           (it->second < best->second) ) { // the lower, the better
        best = it;
      }
    }

    // return the best match
    return best->first;
  }

  v4l2_fract v4l2::getBestFramePeriod(const float frameRate,
                                      const ePixelFormat pxFrm,
                                      const ipoint& resolution) const {
    struct v4l2_fract fp;
    fp.numerator = fp.denominator = 0;

    // search for the pixel format
    for (unsigned int pfIdx=0;pfIdx<pixelFormats_.size();++pfIdx) {
      if (ePixelFormat(pixelFormats_[pfIdx].pixelformat) == pxFrm) {
        // Ok, we got it!
        // now search for each resolution available for that format
        const std::vector<resolutionData>& rv = sizesFR_[pfIdx];
        for (unsigned int r=0;r<rv.size();++r) {
          if (rv[r].resolution == resolution) {
            // we got the resolution, now search the nearest frame rate
            const std::vector<v4l2_fract>& fi = rv[r].frameIntervals;
            fp = fi[0];
            float dist =
              abs((float(fp.denominator)/float(fp.numerator))-frameRate);
            for (unsigned int i=1;i<fi.size();++i) {
              float tmp =
                abs((float(fi[i].denominator)/float(fi[i].numerator)) -
                    frameRate);
              if (tmp < dist) {
                fp = fi[i];
                dist = tmp;
              }
            }
            break; // we already found it!
          }
        }
        break; // we already have it!
      }
    }

    return fp;
  }


  // Initialize the camera with the functor parameters
  bool v4l2::setCameraParameters(const int recursion) {
    recursions_=recursion; // only where the counter is set

    const parameters& param = getParameters();
    
    if (capturing_) {
      stopCapturing();
    }

    if (!isInitialized()) {
      // not initialized yet
      return init();
    } 

    timeout_ = param.selectTimeout;
    retries_ = param.selectRetries;

    if ( buffers_.empty() ||
         (buffers_[0].getMethod() != param.ioMethod) ||
         (pixelFormat_ != param.pixelFormat) ||
         (resolution_  != param.resolution) ) {
     
      resolution_.copy(param.resolution);
      lastUsedParams_.resolution.copy(param.resolution);

      // set the camera format
      if (!initFormat(param.ioMethod,
                      getBestFormat(param.pixelFormat),
                      V4L2_FIELD_ANY,
                      resolution_)) {
        // error setting format.  Maybe it was already set?  
	if (recursion<2) {
	  // try again
	  uninitDevice();
	  if (setCameraParameters(recursion+1)) {
	    return true;
	  }
	}
	converter_ = 0;
	return false;
      }

      // get the right pixel format convertion plug-in
      if (param.pixelFormat == Raw) {
        converter_ = factory::getFactory().instance(Raw);
      } else {
        converter_ = factory::getFactory().instance(pixelFormat_);
      }

      if (isNull(converter_)) {
        std::string str = "Pixel format '";
        std::vector<v4l2_fmtdesc>::const_iterator it;
        for (it=pixelFormats_.begin();
             it!=pixelFormats_.end();
             ++it) {
          if (ePixelFormat(it->pixelformat) == pixelFormat_) {
            transfer(str,it->description);
            break;
          }
        }
        str += "' cannot be converted: plug-in not found.";
        setStatusString(str);
        return false;
      }

      // initialize LUTs and other stuff just now, that we are not grabbing
      converter_->init(param);
    }
    
    lastUsedParams_.resolution.copy(resolution_);

    // update framerate
    if (frameRateChangeSupported_) {
      struct v4l2_streamparm stmPar;
      clear(stmPar);
      stmPar.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
      stmPar.parm.capture.capability = V4L2_CAP_TIMEPERFRAME;
      stmPar.parm.capture.capturemode = 0; // normal quality (no interpolation)
      stmPar.parm.capture.timeperframe=getBestFramePeriod(param.framerate,
                                                          pixelFormat_,
                                                          resolution_);
      
      int ret; // holder of the ioctl return values
      if ((ret=xioctl(cameraHndl_,VIDIOC_S_PARM,&stmPar)) == -1) {
        if (ret != EINVAL) {
          report("VIDIOC_S_PARM");
          return false;
        }
      }
    }

    // --------------------------------------------------
    // set the available control values
    // --------------------------------------------------

    // flag used to force some cam initializations of boolean vars, which 
    // cannot be set to an invalid initial value
    const bool firstTime = (lastUsedParams_.brightnessMode == FeatureNA);
    bool result = true;
    if (firstTime) {
      if (lastUsedParams_.brightness != param.brightness) {
        if (controls_.find(V4L2_CID_BRIGHTNESS) != controls_.end()) {
          result = setBrightness(FeatureManual,param.brightness);
        }
      }

      if (lastUsedParams_.contrast != param.contrast) {
        if (controls_.find(V4L2_CID_CONTRAST) != controls_.end()) {
          result = setContrast(FeatureManual,param.contrast) && result;
        }
      }

      if (lastUsedParams_.saturation != param.saturation) {
        if (controls_.find(V4L2_CID_SATURATION) != controls_.end()) {
          result = setSaturation(FeatureManual,param.saturation) && result;
        }
      }

      if ( (lastUsedParams_.hue != param.hue) ||
           (lastUsedParams_.hueMode != param.hueMode ) ) {
        if ( (controls_.find(V4L2_CID_HUE) != controls_.end()) ||
             (controls_.find(V4L2_CID_HUE_AUTO) != controls_.end()) ) {
          result = setHue(param.hueMode,param.hue) && result;
        }
      }

      if ( (lastUsedParams_.whiteBalanceMode != param.whiteBalanceMode) ||
           (lastUsedParams_.redGain != param.redGain) ||
           (lastUsedParams_.blueGain != param.blueGain) ) {
        if ( (controls_.find(V4L2_CID_AUTO_WHITE_BALANCE) != controls_.end())||
             (controls_.find(V4L2_CID_RED_BALANCE) != controls_.end()) ||
             (controls_.find(V4L2_CID_BLUE_BALANCE) != controls_.end()) ) {
          result = setWhiteBalance(param.whiteBalanceMode,
                                   param.blueGain,
                                   param.redGain) && result;
        }
      }

      if (lastUsedParams_.gamma != param.gamma) {
        if (controls_.find(V4L2_CID_GAMMA) != controls_.end()) {
          result = setGamma(FeatureManual,param.gamma) && result;
        }
      }

      if ( (lastUsedParams_.shutter != param.shutter) ||
           (lastUsedParams_.shutterMode != param.shutterMode ) ){
        if ( (controls_.find(V4L2_CID_EXPOSURE) != controls_.end()) ||
             (controls_.find(V4L2_CID_EXPOSURE_AUTO) != controls_.end()) ) {
          result = setShutter(param.shutterMode,param.shutter) && result;
        }
      }

      if ( (lastUsedParams_.gain != param.gain) ||
           (lastUsedParams_.gainMode != param.gainMode ) ){
        if ( (controls_.find(V4L2_CID_GAIN) != controls_.end()) ||
             (controls_.find(V4L2_CID_AUTOGAIN) != controls_.end()) ) {
          result = setGain(param.gainMode,param.gain) && result;
        }
      }

      if (controls_.find(V4L2_CID_HFLIP) != controls_.end()) {
        result = setHorizontalFlip(param.horizontalFlip) && result;
      }

      if (controls_.find(V4L2_CID_VFLIP) != controls_.end()) {
        result = setVerticalFlip(param.verticalFlip) && result;
      }

      if (lastUsedParams_.powerLineFilter != param.powerLineFilter) {
        if (controls_.find(V4L2_CID_POWER_LINE_FREQUENCY) != controls_.end()) {
          result = setPowerLineFilter(param.powerLineFilter) && result;
        }
      }

      if (lastUsedParams_.whiteBalanceTemperature != 
          param.whiteBalanceTemperature) {
        if (controls_.find(V4L2_CID_WHITE_BALANCE_TEMPERATURE) !=
            controls_.end()) {
          result = setWhiteBalanceTemperature(param.whiteBalanceTemperature) &&
            result;
        }
      }

      if ( (lastUsedParams_.sharpness != param.sharpness) ||
           (lastUsedParams_.sharpnessMode != param.sharpnessMode ) ){
        if (controls_.find(V4L2_CID_SHARPNESS) != controls_.end()) {
          result = setSharpness(param.sharpnessMode,param.sharpness) && result;
        }
      }

      if (lastUsedParams_.backlightCompensation!=param.backlightCompensation) {
        if (controls_.find(V4L2_CID_BACKLIGHT_COMPENSATION)!=controls_.end()) {
          result = setBacklightCompensation(param.backlightCompensation) &&
            result;
        }
      }

    }

#if 0
    if (panTilt_) {
      if (param.panMode == param.tiltMode) {
        if (!setPanTilt(param.panMode,
                        param.pan,
                        param.tilt)) {
          return false;
        }
      } else {
        if (!setPan(param.panMode,param.pan) ||
            !setTilt(param.tiltMode,param.tilt)) {
          return false;
        }
      }
    }
#endif

    return startCapturing();
  }

  // ----------------------------------------------------------------
  bool v4l2::isInitialized() const {
    return initialized_;
  }

  bool v4l2::isActive() const {
    return isInitialized() && (cameraHndl_>=0);
  }

  const std::string v4l2::cameraName() const {
    return camName_;
  }
 
  bool v4l2::supportedPixelFormats(std::vector<ePixelFormat>& formats) const {
    formats.resize(pixelFormats_.size());
    for (unsigned int i=0;i<formats.size();++i) {
      formats[i] = static_cast<ePixelFormat>(pixelFormats_[i].pixelformat);
    }
    return !formats.empty();
  }

  bool v4l2::supportedFrameratesAndResolutions(fvector& framerates,
                                               vector<ipoint>& resols,
                                               matrix<ubyte>& val) const {

    framerates.clear();
    resols.clear();
    val.clear();

    const ePixelFormat pf = getBestFormat(getParameters().pixelFormat);
    
    // find the index for the given format
    unsigned int idx;
    for (idx=0;idx<pixelFormats_.size();++idx) {
      if (ePixelFormat(pixelFormats_[idx].pixelformat) == pf) {
        break;
      }
    }
    
    if (idx>=pixelFormats_.size()) {
      setStatusString("Could not get supported framerates and resolutions.");
      return false; 
    }

    const std::vector<resolutionData>& res = sizesFR_[idx];

    std::map<ipoint,int> allRes;  // map to contain all resolutions
    std::map<float,int> allFRs;  // map to contain all framerates
    
    // first order in the maps all the required information
    for (unsigned int r=0;r<res.size();++r) {
      allRes[res[r].resolution] = r;
      const std::vector<v4l2_fract>& fi = res[r].frameIntervals;
      for (unsigned int i=0;i<fi.size();++i) {
        allFRs[float(fi[i].denominator)/float(fi[i].numerator)] = i;
      }
    }

    // now with the maps create the output arguments and prepare to create the
    // matrix
    std::map<ipoint,int>::iterator rit;
    resols.allocate(allRes.size());
    int i;
    for (rit=allRes.begin(),i=0;rit!=allRes.end();++rit,++i) {
      resols.at(i).copy((*rit).first);
      (*rit).second = i;
    }
    
    std::map<float,int>::iterator fit;
    framerates.allocate(allFRs.size());
    for (fit=allFRs.begin(),i=0;fit!=allFRs.end();++fit,++i) {
      framerates.at(i) = (*fit).first;
      (*fit).second = i;
    }

    // again, go thru all resolutions and framerates to initialize the matrix
    // rows: framerate
    // cols: resolutions
    val.assign(framerates.size(),resols.size(),0);

    // first order in the maps all the required information
    for (unsigned int r=0;r<res.size();++r) {
      int resIdx = allRes[res[r].resolution];
      const std::vector<v4l2_fract>& fi = res[r].frameIntervals;
      for (unsigned int id=0;id<fi.size();++id) {
        int frIdx = allFRs[float(fi[id].denominator)/float(fi[id].numerator)];
        val.at(frIdx,resIdx)=1;
      }
    }

    return true;
  }

  template<class I>
  bool v4l2::processImage(void* data,
                          const unsigned int bsize,
                          const bool turnAround,
                          I& img) {

    img.allocate(resolution_); // allocate the right size before
                               // calling convert
    if (isNull(converter_) ||
        !converter_->convert(data,bsize,turnAround,img)) {
      return false;
    }
    
    return true;
  }



  template<class I>
  bool v4l2::readFrame(I& img) {
    
    const parameters& par = getParameters();
    struct v4l2_buffer buf;
    unsigned int i;
  
    switch (par.ioMethod) {
      case Read:
        if (::v4l2_read(cameraHndl_,
                        buffers_[0].ptr(), 
                        buffers_[0].size()) == -1) {
          switch (errno) {
            case EAGAIN:
              // Non-blocking I/O has been selected using O_NONBLOCK and no
              // data was immediately available for reading.
              img.clear();
              return false;
              
            case EIO:
              /* Could ignore EIO, see spec. */
              
              /* fall through */

              // I/O error. This indicates some hardware problem or a failure
              // to communicate with a remote device (USB camera etc.).
            default:
              report("read");
              return false;
          }
        }
        
        return processImage(buffers_[0].ptr(),
                            buffers_[0].size(),
                            par.turnAround,
                            img); 
        break;
        
      case MemoryMap:
        clear(buf);
      
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
      
        if (xioctl (cameraHndl_, VIDIOC_DQBUF, &buf) < 0) {
          switch (errno) {
          case EAGAIN:
            img.clear();
            return false;
            break;
          case EINVAL:
            report("VDIOC_DQBUF: EINVAL buffer type not supported or index" \
                   "out of bounds or no buffers allocated or userptr or " \
                   "length invalid");
            return false;
            break;
          case EIO:
            /* Could ignore EIO, see spec. */
            report("VDIOC_DQBUF: EIO internal error (signal loss?)");
            return false;
            break;
          case ENOMEM:
            report("VDIOC_DQBUF: ENOMEM not enough memory to enqueue buffer");
            return false;
            break;
            
          default:
            report("VIDIOC_DQBUF");
            return false;
          }
        }
        
        assert(buf.index < buffers_.size());
      
        processImage(buffers_[buf.index].ptr(),
                     buffers_[buf.index].size(),
                     par.turnAround,
                     img);
      
        if (xioctl(cameraHndl_, VIDIOC_QBUF, &buf) == -1) {
          report("VIDIOC_QBUF");
          return false;
        }
        
        break;
      
      case UserSpace:
        
        clear(buf);
        
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_USERPTR;
        
        if (xioctl (cameraHndl_, VIDIOC_DQBUF, &buf) == -1) {
          switch (errno) {
            case EAGAIN:
              img.clear();
              return true;
              
            case EIO:
              /* Could ignore EIO, see spec. */
              
              /* fall through */
              
            default:
              report("VIDIOC_DQBUF");
              return false;
          }
        }
        
        for (i = 0; i < buffers_.size(); ++i) {
          if (buf.m.userptr==reinterpret_cast<unsigned long>(buffers_[i].ptr())
              && buf.length==buffers_[i].size()) {
            break;
          }
        }
        
        assert (i < buffers_.size());
        
        processImage((void*) buf.m.userptr,
                     buf.length,
                     par.turnAround,
                     img);
        
        if (xioctl (cameraHndl_, VIDIOC_QBUF, &buf) == -1) {
          report("VIDIOC_QBUF");
          return false;
        }
        
        break;
    }
    
    return true;
  }



  // load a color image from the usb port
  bool v4l2::apply(image& theImage) {

    if (!isInitialized()) {
      theImage.clear();
      // the error message should still be there...
      return false;      
    }

    // file descriptor set
    fd_set fdSet;
    timeval tv;
    int retval;

    bool timedout=false;
    int tries=0;
    {
      // Watch camera to see when it has input.
      FD_ZERO(&fdSet);
      FD_SET(cameraHndl_, &fdSet);
      
      /* Wait up to two seconds. */
      tv.tv_sec = static_cast<int>(timeout_);
      tv.tv_usec = static_cast<int>(1000000*(timeout_-tv.tv_sec));
      
      // flag indicating everything potentially ok
      timedout=false; 

      // With the select() we suspend execution until the driver has
      // captured data
      retval = select(cameraHndl_+1, &fdSet, NULL, NULL, &tv);
      
      // On success select() returns the total number of bits set in the
      // fdSet. When the function time's out it returns a value of
      // zero. On failure it returns -1 and the errno variable is set
      // appropriately.
      
      // When the application did not call VIDIOC_QBUF or
      // VIDIOC_STREAMON yet the select() function succeeds, setting the
      // bit of the file descriptor in readfds or writefds, but
      // subsequent VIDIOC_DQBUF calls will fail.
      
      if (retval == -1) {
	if (errno == EINTR) {
	  setStatusString("Not ready yet!");
	  return false;
	}
	report("select()");
	return false;
      }
      
      if (retval == 0) {
	timedout=true;
	tries++;
      }
    } while(timedout && (tries<retries_));

    if (timedout) {
      setStatusString("select() timed out");
      return false;
    }
    
    return readFrame(theImage);
  }

  // load a grey value channel from the
  bool v4l2::apply(channel8& theChannel) {

    if (!isInitialized()) {
      theChannel.clear();
      // the error message should still be there...
      return false;      
    }

    // file descriptor set
    fd_set fdSet;
    timeval tv;
    int retval;

    bool timedout=false;
    int tries=0;
    {

      // Watch camera to see when it has input.
      FD_ZERO(&fdSet);
      FD_SET(cameraHndl_, &fdSet);
      
      /* Wait up to two seconds. */
      tv.tv_sec = static_cast<int>(timeout_);
      tv.tv_usec = static_cast<int>(1000000*(timeout_-tv.tv_sec));

      // flag indicating everything potentially ok
      timedout=false; 
     
      // With the select() we suspend execution until the driver has
      // captured data
      retval = select(cameraHndl_+1, &fdSet, NULL, NULL, &tv);

      // On success select() returns the total number of bits set in the
      // fdSet. When the function time's out it returns a value of
      // zero. On failure it returns -1 and the errno variable is set
      // appropriately.
      
      // When the application did not call VIDIOC_QBUF or
      // VIDIOC_STREAMON yet the select() function succeeds, setting the
      // bit of the file descriptor in readfds or writefds, but
      // subsequent VIDIOC_DQBUF calls will fail.
      
      if (retval == -1) {
	if (errno == EINTR) {
	  setStatusString("Not ready yet!");
	  return false;
	}
	report("select()");
	return false;
      }
    
      if (retval == 0) {
	timedout=true;
	tries++;
      }
    } while(timedout && (tries<retries_));
    
    if (timedout) {
      setStatusString("select() timed out");
      return false;
    }

    return readFrame(theChannel);
  }

  // copy a constant unsigned char chain into a string!
  void v4l2::transfer(std::string& dest,const __u8* src) const {
    const __u8* ptr=src;
    while ( (dest.length()<32) && ((*ptr) != 0) ) {
      dest+=static_cast<std::string::value_type>(*ptr);
      ++ptr;
    }
  }

  bool v4l2::stopCapturing() {
    const parameters& par = getParameters();
    enum v4l2_buf_type type;

    switch (par.ioMethod) {
      case Read:
        /* Nothing to do. */
        break;
        
      case MemoryMap:
      case UserSpace:
        type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

        if (xioctl (cameraHndl_, VIDIOC_STREAMOFF, &type) == -1) {
          report("VIDIOC_STREAMOFF");
          return false;
        }
        
        break;
    }

    capturing_ = false;
    return !capturing_;
  }

  bool v4l2::startCapturing() {
    unsigned int i;
    const parameters& par = getParameters();
    enum v4l2_buf_type type;
    
    switch (par.ioMethod) {
      case Read:
        /* Nothing to do. */
        break;
        
      case MemoryMap:
        for (i = 0; i < buffers_.size(); ++i) {
          struct v4l2_buffer buf;

          clear(buf);

          buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
          buf.memory      = V4L2_MEMORY_MMAP;
          buf.index       = i;

          if (xioctl (cameraHndl_, VIDIOC_QBUF, &buf) == -1) {
            report("VIDIOC_QBUF");
            return false;
          }
        }
		
        type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

        if (xioctl (cameraHndl_, VIDIOC_STREAMON, &type) == -1) {
          report("VIDIOC_STREAMON");
          return false;
        }

        break;

      case UserSpace:
        for (i = 0; i < buffers_.size(); ++i) {
          struct v4l2_buffer buf;

          clear(buf);

          buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
          buf.memory      = V4L2_MEMORY_USERPTR;
          buf.index       = i;
          buf.m.userptr	= (unsigned long) buffers_[i].ptr();
          buf.length      = buffers_[i].size();
          
          if (xioctl (cameraHndl_, VIDIOC_QBUF, &buf) == -1) {
            report("VIDIOC_QBUF");
            return false;
          }
        }

        type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        
        if (xioctl (cameraHndl_, VIDIOC_STREAMON, &type) == -1) {
          report("VIDIOC_STREAMON");
          return false;
        }

        break;
    }

    capturing_ = true;
    return capturing_;
  }

  /*
   * Write v4l2::ePixelFormat
   */
  bool write(ioHandler& handler,const v4l2::ePixelFormat val) {
    switch(val) {
      case v4l2::Raw:
        return handler.write("Raw");
        break;
      case v4l2::Invalid:
        return handler.write("Invalid");
        break;
      case v4l2::Auto:
        return handler.write("Auto");
        break;
      case v4l2::RGB332:
        return handler.write("RGB332");
        break;
      case v4l2::RGB444:
        return handler.write("RGB444");
        break;
      case v4l2::RGB555:
        return handler.write("RGB555");
        break;
      case v4l2::RGB565:
        return handler.write("RGB565");
        break;
      case v4l2::RGB555X:
        return handler.write("RGB555X");
        break;
      case v4l2::RGB565X:
        return handler.write("RGB565X");
        break;
      case v4l2::BGR24:
        return handler.write("BGR24");
        break;
      case v4l2::RGB24:
        return handler.write("RGB24");
        break;
      case v4l2::BGR32:
        return handler.write("BGR32");
        break;
      case v4l2::RGB32:
        return handler.write("RGB32");
        break;
      case v4l2::GREY:
        return handler.write("GREY");
        break;
      case v4l2::Y16:
        return handler.write("Y16");
        break;
      case v4l2::PAL8:
        return handler.write("PAL8");
        break;
      case v4l2::YVU410:
        return handler.write("YVU410");
        break;
      case v4l2::YVU420:
        return handler.write("YVU420");
        break;
      case v4l2::YUYV:
        return handler.write("YUYV");
        break;
      case v4l2::UYVY:
        return handler.write("UYVY");
        break;
      case v4l2::YUV422P:
        return handler.write("YUV422P");
        break;
      case v4l2::YUV411P:
        return handler.write("YUV411P");
        break;
      case v4l2::Y41P:
        return handler.write("Y41P");
        break;
      case v4l2::YUV444:
        return handler.write("YUV444");
        break;
      case v4l2::YUV555:
        return handler.write("YUV555");
        break;
      case v4l2::YUV565:
        return handler.write("YUV565");
        break;
      case v4l2::YUV32:
        return handler.write("YUV32");
        break;
      case v4l2::NV12:
        return handler.write("NV12");
        break;
      case v4l2::NV21:
        return handler.write("NV21");
        break;
      case v4l2::YUV410:
        return handler.write("YUV410");
        break;
      case v4l2::YUV420:
        return handler.write("YUV420");
        break;
      case v4l2::YYUV:
        return handler.write("YYUV");
        break;
      case v4l2::HI240:
        return handler.write("HI240");
        break;
      case v4l2::HM12:
        return handler.write("HM12");
        break;
      case v4l2::SBGGR8:
        return handler.write("SBGGR8");
        break;
      case v4l2::SBGGR16:
        return handler.write("SBGGR16");
        break;
      case v4l2::MJPEG:
        return handler.write("MJPEG");
        break;
      case v4l2::JPEG:
        return handler.write("JPEG");
        break;
      case v4l2::DV:
        return handler.write("DV");
        break;
      case v4l2::MPEG:
        return handler.write("MPEG");
        break;
      case v4l2::WNVA:
        return handler.write("WNVA");
        break;
      case v4l2::SN9C10X:
        return handler.write("SN9C10X");
        break;
      case v4l2::PWC1:
        return handler.write("PWC1");
        break;
      case v4l2::PWC2:
        return handler.write("PWC2");
        break;
      case v4l2::ET61X251:
        return handler.write("ET61X251");
        break;

      default:
        handler.write("Invalid");
        handler.setStatusString("Unknown v4l2::ePixelFormat value");
        return false;
    }    
  
    return false;
  }

  /*
   * Read v4l2::ePixelFormat
   */
  bool read(ioHandler& handler,v4l2::ePixelFormat& val) {
    std::string tmp;
    if (handler.read(tmp)) {
      if (tmp.find("Invalid") != std::string::npos) {
        val = v4l2::Invalid;
      } else if (tmp.find("Raw") != std::string::npos) {
        val = v4l2::Raw;
      } else if (tmp.find("Auto") != std::string::npos) {
        val = v4l2::Auto;
      } else if (tmp.find("RGB332") != std::string::npos) {
        val = v4l2::RGB332;
      } else if (tmp.find("RGB444") != std::string::npos) {
        val = v4l2::RGB444;
      } else if (tmp.find("RGB555") != std::string::npos) {
        val = v4l2::RGB555;
      } else if (tmp.find("RGB565") != std::string::npos) {
        val = v4l2::RGB565;
      } else if (tmp.find("RGB555X") != std::string::npos) {
        val = v4l2::RGB555X;
      } else if (tmp.find("RGB565X") != std::string::npos) {
        val = v4l2::RGB565X;
      } else if (tmp.find("BGR24") != std::string::npos) {
        val = v4l2::BGR24;
      } else if (tmp.find("RGB24") != std::string::npos) {
        val = v4l2::RGB24;
      } else if (tmp.find("BGR32") != std::string::npos) {
        val = v4l2::BGR32;
      } else if (tmp.find("RGB32") != std::string::npos) {
        val = v4l2::RGB32;
      } else if (tmp.find("GREY") != std::string::npos) {
        val = v4l2::GREY;
      } else if (tmp.find("Y16") != std::string::npos) {
        val = v4l2::Y16;
      } else if (tmp.find("PAL8") != std::string::npos) {
        val = v4l2::PAL8;
      } else if (tmp.find("YVU410") != std::string::npos) {
        val = v4l2::YVU410;
      } else if (tmp.find("YVU420") != std::string::npos) {
        val = v4l2::YVU420;
      } else if (tmp.find("YUYV") != std::string::npos) {
        val = v4l2::YUYV;
      } else if (tmp.find("UYVY") != std::string::npos) {
        val = v4l2::UYVY;
      } else if (tmp.find("YUV422P") != std::string::npos) {
        val = v4l2::YUV422P;
      } else if (tmp.find("YUV411P") != std::string::npos) {
        val = v4l2::YUV411P;
      } else if (tmp.find("Y41P") != std::string::npos) {
        val = v4l2::Y41P;
      } else if (tmp.find("YUV444") != std::string::npos) {
        val = v4l2::YUV444;
      } else if (tmp.find("YUV555") != std::string::npos) {
        val = v4l2::YUV555;
      } else if (tmp.find("YUV565") != std::string::npos) {
        val = v4l2::YUV565;
      } else if (tmp.find("YUV32") != std::string::npos) {
        val = v4l2::YUV32;
      } else if (tmp.find("NV12") != std::string::npos) {
        val = v4l2::NV12;
      } else if (tmp.find("NV21") != std::string::npos) {
        val = v4l2::NV21;
      } else if (tmp.find("YUV410") != std::string::npos) {
        val = v4l2::YUV410;
      } else if (tmp.find("YUV420") != std::string::npos) {
        val = v4l2::YUV420;
      } else if (tmp.find("YYUV") != std::string::npos) {
        val = v4l2::YYUV;
      } else if (tmp.find("HI240") != std::string::npos) {
        val = v4l2::HI240;
      } else if (tmp.find("HM12") != std::string::npos) {
        val = v4l2::HM12;
      } else if (tmp.find("SBGGR8") != std::string::npos) {
        val = v4l2::SBGGR8;
      } else if (tmp.find("SBGGR16") != std::string::npos) {
        val = v4l2::SBGGR16;
      } else if (tmp.find("MJPEG") != std::string::npos) {
        val = v4l2::MJPEG;
      } else if (tmp.find("JPEG") != std::string::npos) {
        val = v4l2::JPEG;
      } else if (tmp.find("DV") != std::string::npos) {
        val = v4l2::DV;
      } else if (tmp.find("MPEG") != std::string::npos) {
        val = v4l2::MPEG;
      } else if (tmp.find("WNVA") != std::string::npos) {
        val = v4l2::WNVA;
      } else if (tmp.find("SN9C10X") != std::string::npos) {
        val = v4l2::SN9C10X;
      } else if (tmp.find("PWC1") != std::string::npos) {
        val = v4l2::PWC1;
      } else if (tmp.find("PWC2") != std::string::npos) {
        val = v4l2::PWC2;
      } else if (tmp.find("ET61X251") != std::string::npos) {
        val = v4l2::ET61X251;
      } else {
        val = v4l2::Invalid; // default value
        
        std::string str("Unknown symbol ");
        str+=tmp;
        handler.setStatusString(str);
        
        return false;
      }
      return true;
    }
    return false;

  }



  /*
   * Write v4l2::ePowerLineFilter
   */
  bool write(ioHandler& handler,const v4l2::ePowerLineFilter val) {
    switch(val) {
      case v4l2::Disabled:
        return handler.write("Disabled");
        break;
      case v4l2::Filter50Hz:
        return handler.write("Filter50Hz");
        break;
      case v4l2::Filter60Hz:
        return handler.write("Filter60Hz");
        break;
      default:
        handler.write("Disabled");
        handler.setStatusString("Unknown v4l2::ePowerLineFilter value");
        return false;
    }
    return false;
  }

  /*
   * Read v4l2::ePowerLineFilter
   */
  bool read(ioHandler& handler,v4l2::ePowerLineFilter& val) {
    std::string tmp;
    if (handler.read(tmp)) {
      if (tmp == "Disabled") {
        val = v4l2::Disabled;
      } else if (tmp.find("50") != std::string::npos) {
        val = v4l2::Filter50Hz;
      } else if (tmp.find("60") != std::string::npos) {
        val = v4l2::Filter60Hz;
      } else {
        val = v4l2::Disabled; // default value
        
        std::string str("Unknown symbol ");
        str+=tmp;
        handler.setStatusString(str);
        
        return false;
      }
      return true;
    }
    return false;
  }

  /*
   * Write v4l2::eIOMethod
   */
  bool write(ioHandler& handler,const v4l2::eIOMethod val) {
    switch(val) {
      case v4l2::Read:
        return handler.write("Read");
        break;
      case v4l2::MemoryMap:
        return handler.write("MemoryMap");
        break;
      case v4l2::UserSpace:
        return handler.write("UserSpace");
        break;
      default:
        handler.write("Read");
        handler.setStatusString("Unknown v4l2::eIOMethod value");
        return false;
    }
    return false;
  }

  /*
   * Read v4l2::eIOMethod
   */
  bool read(ioHandler& handler,v4l2::eIOMethod& val) {
    std::string tmp;
    if (handler.read(tmp)) {
      if (tmp == "Read") {
        val = v4l2::Read;
      } else if (tmp.find("Map") != std::string::npos) {
        val = v4l2::MemoryMap;
      } else if (tmp.find("User") != std::string::npos) {
        val = v4l2::UserSpace;
      } else {
        val = v4l2::MemoryMap; // default value
        
        std::string str("Unknown symbol ");
        str+=tmp;
        handler.setStatusString(str);
        
        return false;
      }
      return true;
    }
    return false;
  }

} // namespace lti

#endif // __linux__
#endif // _USE_V4L2
