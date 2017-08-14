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
 * \file   ltiChannelProcessingInterface.h
 *         Simple interface from which all functors that process a matrix<T>
 *         to produce another matrix can be inherited.
 * 
 * \author Pablo Alvarado
 * \date   23.09.2007
 *
 * revisions ..: $Id: ltiChannelProcessingInterface.h,v 1.1 2007-10-14 02:45:37 alvarado Exp $
 */

#ifndef _LTI_CHANNEL_PROCESSING_INTERFACE_H_
#define _LTI_CHANNEL_PROCESSING_INTERFACE_H_

#include "ltiGenericChannel.h"

namespace lti {

  /**
   * Class channelProcessingInterface
   *
   * Very simple interface to allow virtualization of classes that transform a
   * channel into another one of the same type.
   *
   * Note that it is required that the apply methods do not alter the internal
   * state of the class.
   *
   * \ingroup gInterfaces
   */
  template<typename T>
  class channelProcessingInterface {
  public:

    typedef typename genericChannel<T>::type channel_type;
    
    /**
     * Virtual destructor
     */
    virtual ~channelProcessingInterface() {};

    /**
     * On-place processing apply.
     * 
     * The inherited methods should take the \a srcdest channel, process it in
     * some way, and on the same channel leave the result.  No restrictions are
     * imposed on whether the memory block of the resulting channel will be the
     * same that the one in the original channel.  As a matter of fact, it
     * usually won't be.
     *
     * If you need to ensure the memory constancy, and assuming the resulting
     * channel will always have the same size than the original one, then you
     * can use the following code: 
     * \code
     * channelProcessingInterfaceInheritedClass<T> theFunctor; 
     * channel<T> tmp;
     * theFunctor.apply(srcdest,tmp); 
     * srcdest.fill(tmp)
     * \endcode
     *
     * which of course will be slower as it requires to copy all the data of
     * the result in the original channel.
     *
     * @param srcdest channel with the source data. The result
     *                will be left here too.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(channel_type& srcdest) const = 0;

    /**
     * On-copy processing apply.
     * 
     * The inherited methods take the \a src channel and process it leaving the
     * result in the \a dest channel.
     *
     * @param src channel with the source data.
     * @param dest channel where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const channel_type& src, channel_type& dest) const = 0;
  };
}

#endif

