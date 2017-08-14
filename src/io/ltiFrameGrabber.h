/*
 * Copyright (C) 1998 - 2004
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
 * \file   ltiFrameGrabber.h
 *         Base class for all framegrabbers and similar like USB and 
 *         FireWire buses
 * \author Peter Mathes
 * \date   13.08.1999
 *
 * $Id: ltiFrameGrabber.h,v 1.9 2008-10-12 00:09:27 alvarado Exp $
 */

#ifndef _LTI_FRAMEGRABBER_H_
#define _LTI_FRAMEGRABBER_H_

#include "ltiFunctor.h"
#include "ltiImage.h"
#include "ltiChannel.h"
#include "ltiChannel8.h"
#include "ltiChannel32.h"
#include "ltiHardwareConfig.h"

namespace lti {
  /**
   * Frame grabber class.
   *
   * Class for grabbing frames from cameras and converting them to
   * ltiImage format.  The derived classes provides the necessary
   * interfaces to deal with real hardware interfaces, which can be
   * frame grabbers, USB, FireWire, CameraLink, etc.
   *
   * This is an abstract class.
   *
   * @ingroup gAcquisition
   */
  class frameGrabber : public functor {
  public:
    /**
     * Feature IDs.
     *
     * These IDs have been selected based on the IIDC standard
     */
    enum {
      FeatureTrigger=428,          /**< Trigger       */
      FeatureTriggerDelay=429      /**< Trigger delay */
    };


    /**
     * frameGrabber parameters class
     */
    class parameters : public functor::parameters {
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
       * Destructor
       */
      ~parameters();

      /**
       * copy member
       */
      parameters& copy( const parameters& other );

      /**
       * Copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& operator=(const parameters& other);

      /**
       * Returns the name of this class.
       */
      virtual const std::string& name() const;

      /**
       * returns a pointer to a clone of the parameters.
       */
      virtual parameters* clone() const;

      /**
       * returns a pointer to a new instance of the parameters.
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

      // -------------------------------------------------------------------
      //  Attributes of the parameters
      // -------------------------------------------------------------------

      /**
       * Turn around aquired image.
       *
       * If \c true, the grabber will flip the image before it is
       * returned to the caller. This is useful for upside-down mounted
       * cameras.
       *
       * Default value: false
       */
      bool turnAround;

      /**
       * Snapshot mode.
       *
       * Most frame-grabbers, or video interfaces, provide ways to
       * send just one image or an image stream.  The difference
       * relies in the bandwidth used while capturing images.  If you
       * require the stream, this value has to be set to \c false, as
       * it is usually more efficient, but if you need an image just
       * once in a while, then you should save bandwith setting this
       * value to \c true.
       *
       * Default value: true
       */
      bool snapShotMode;
      
    };

    /**
     * Default constructor
     */
    frameGrabber();

    /**
     * Copy constructor
     */
    frameGrabber(const frameGrabber& other);

    /**
     * Constructor, sets the parameters
     */
    frameGrabber(const parameters& theParam);

    /**
     * Destructor
     */
    virtual ~frameGrabber( void );

    /**
     * Returns the name of this class.
     */
    virtual const std::string& name() const;

    /**
     * Returns current parameters.
     */
    const parameters& getParameters() const;

    /**
     * Load an (color) image from the grabber
     */
    virtual bool apply(image& theImage)=0;

    /**
     * Load a grey value channel from the grabber
     */
    virtual bool apply(channel8& theChannel)=0;

    /**
     * Load a grey value channel from the grabber
     *
     * The default implementation grabs a channel8 and casts it to the 
     * channel.
     */
    virtual bool apply(channel32& theChannel);

    /**
     * Load a grey value channel from the grabber.
     *
     * The default implementation grabs a channel8 and casts it to the 
     * channel.
     */
    virtual bool apply(channel& theChannel);

    /**
     * Check if the frame grabber / camera system is active
     */
    virtual bool isActive() const=0;

    /**
     * Initialize frame grabber.
     *
     * @return true if successful, false otherwise
     */
    virtual bool init() = 0;

    /**
     * Returns true if the frame-grabber hardware has been initialized.
     */
    virtual bool isInitialized() const=0;

    /**
     * copy member
     */
    frameGrabber& copy(const frameGrabber& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    frameGrabber& operator=(const frameGrabber& other);
  };
}

#endif


