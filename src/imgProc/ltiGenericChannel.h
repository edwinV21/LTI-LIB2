/*
 * Copyright (C) 2007
 * ITCR, Pablo Alvarado
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
 * \file   ltiGenericChannel.h
 *         Definition of a meta-programming type which provides the channel
 *         corresponding to a specific type
 * 
 * \author Pablo Alvarado
 * \date   23.09.2007
 *
 * revisions ..: $Id: ltiGenericChannel.h,v 1.1 2007-10-14 02:45:37 alvarado Exp $
 */

#ifndef _LTI_GENERIC_CHANNEL_H_
#define _LTI_GENERIC_CHANNEL_H_

#include "ltiChannel.h"
#include "ltiChannel8.h"
#include "ltiChannel32.h"
#include "ltiChannelC.h"

namespace lti {

  /**
   * Class genericChannel
   *
   * This is a meta-programming class that allows to obtain the corresponding
   * channel from the type of the contained elements.  Thus:
   *
   * - lti::genericChannel<ubyte>::type equals lti::channel8
   * - lti::genericChannel<float>::type equals lti::channel
   * - lti::genericChannel<int32>::type equals lti::channel32
   * - lti::genericChannel<lti::fcomplex>::type equals lti::channelC
   *
   * Other types will produce a compiler error.
   *
   * \ingroup gInterfaces
   */
  template<typename T>
  class genericChannel {
    // the default implementation does not have an internal type, so that
    // the compiler will generate an error.
  };

  template<>
  class genericChannel<ubyte> {
  public:
    typedef channel8 type;
  };

  template<>
  class genericChannel<float> {
  public:
    typedef channel type;
  };

  template<>
  class genericChannel<int32> {
  public:
    typedef channel32 type;
  };

  template<>
  class genericChannel< complex<float> > {
  public:
    typedef channelC type;
  };

}

#endif

