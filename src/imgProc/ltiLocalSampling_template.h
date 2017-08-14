/*
 * Copyright (C) 1998-2005
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
 * \file   ltiLocalSampling_sampling.h
 *         Contains the template apply functions of lti::localSampling.
 * \author Arnd Hannemann
 * \date   03.12.2004
 *
 * revisions ..: $Id: ltiLocalSampling_template.h,v 1.13 2009-08-30 04:30:29 alvarado Exp $
 */


// header and template implementation are included separetely 
// in each sampling functor to avoid forward class declarations
// so this define becomes necessary
#ifndef _LTI_LOCAL_SAMPLING_TEMPLATE_H_
#define _LTI_LOCAL_SAMPLING_TEMPLATE_H_


// include sampling headers to avoid forward class declarations
#include "ltiSliceSampling.h"
#include "ltiSiftSampling.h"
#include "ltiGridSampling.h"

namespace lti {

  template<class Acc>
  bool localSampling::apply(Acc& acc,
                            const matrix<typename Acc::value_type>& src,
                            const location& loc,
                            dvector& dest, 
                            const eResultOrder& order ) const {
    // for each new sampling a section has to be inserted 

    // try to cast into previous type
    switch (last_) {
      case 1: {
        const sliceSampling *s = dynamic_cast<const sliceSampling*>(this);
        if (s!=0) return s->apply(acc,src,loc,dest,order);
        break;
      }
      case 2: {
        const siftSampling *s = dynamic_cast<const siftSampling*>(this);
        if (s!=0) return s->apply(acc,src,loc,dest,order);
        break;
      }
      case 3: {
        const gridSampling *s = dynamic_cast<const gridSampling*>(this);
        if (s!=0) return s->apply(acc,src,loc,dest,order);
        break;
      }
    default: {
      }
    }

    if (this->name()=="lti::sliceSampling") {
      // remember cast
      last_ = 1;
      const sliceSampling *s = dynamic_cast<const sliceSampling*>(this);
      return s->apply(acc,src,loc,dest,order);
    } else
    if (this->name()=="lti::siftSampling") {
      // remember cast
      last_ = 2;
      const siftSampling *s = dynamic_cast<const siftSampling*>(this);
      return s->apply(acc,src,loc,dest,order);
    } else
    if (this->name()=="lti::gridSampling") {
      // remember cast
      last_ = 3;
      const gridSampling *s = dynamic_cast<const gridSampling*>(this);
      return s->apply(acc,src,loc,dest,order);
    }

    setStatusString("could not invoke apply, sampling not known\n");
    return false;
  }

  template<class Acc>
  bool localSampling::apply(Acc& acc,
                            const matrix<typename Acc::value_type>& src,
                            dvector& dest, 
                            const eResultOrder& order )  const {
    // for each new sampling a section has to be inserted 

    // try to cast into previous type
    switch (last_) {
      case 1: {
        const sliceSampling *s = dynamic_cast<const sliceSampling*>(this);
        if (s!=0) return s->apply(acc,src,dest,order);
        break;
      }
      case 2: {
        const siftSampling *s = dynamic_cast<const siftSampling*>(this);
        if (s!=0) return s->apply(acc,src,dest,order);
        break;
      }
      case 3: {
        const gridSampling *s = dynamic_cast<const gridSampling*>(this);
        if (s!=0) return s->apply(acc,src,dest,order);
        break;
      }
    default: {
      }
    }

    if (this->name()=="lti::sliceSampling") {
      // remember cast
      last_ = 1;
      const sliceSampling *s = dynamic_cast<const sliceSampling*>(this);
      return s->apply(acc,src,dest,order);
    } else
    if (this->name()=="lti::siftSampling") {
      // remember cast
      last_ = 2;
      const siftSampling *s = dynamic_cast<const siftSampling*>(this);
      return s->apply(acc,src,dest,order);
    } else
    if (this->name()=="lti::gridSampling") {
      // remember cast
      last_ = 3;
      const gridSampling *s = dynamic_cast<const gridSampling*>(this);
      return s->apply(acc,src,dest,order);
    }

    setStatusString("could not invoke apply, sampling not known\n");
    return false;
  }

  template<class Acc>
  bool localSampling::apply(Acc& acc,
                            const matrix<typename Acc::value_type>& src,
                            const list<location>& locs,
                            std::list<dvector>& dest,
                            const eResultOrder& order) const {
    // for each new sampling a section has to be inserted 

    // try to cast into previous type
    switch (last_) {
      case 1: {
        const sliceSampling *s = dynamic_cast<const sliceSampling*>(this);
        if (s!=0) return s->apply(acc,src,locs,dest,order);
        break;
      }
      case 2: {
        const siftSampling *s = dynamic_cast<const siftSampling*>(this);
        if (s!=0) return s->apply(acc,src,locs,dest,order);
        break;
      }
      case 3: {
        const gridSampling *s = dynamic_cast<const gridSampling*>(this);
        if (s!=0) return s->apply(acc,src,locs,dest,order);
        break;
      }
    default: {
      }
    }

    if (this->name()=="lti::sliceSampling") {
      // remember cast
      last_ = 1;
      const sliceSampling *s = dynamic_cast<const sliceSampling*>(this);
      return s->apply(acc,src,locs,dest,order);
    } else
    if (this->name()=="lti::siftSampling") {
      // remember cast
      last_ = 2;
      const siftSampling *s = dynamic_cast<const siftSampling*>(this);
      return s->apply(acc,src,locs,dest,order);
    }
    if (this->name()=="lti::gridSampling") {
      // remember cast
      last_ = 3;
      const gridSampling *s = dynamic_cast<const gridSampling*>(this);
      return s->apply(acc,src,locs,dest,order);
    }
    setStatusString("could not invoke apply, sampling not known\n");
    return false;
  }


  /**********************************
   * applys for two matrices        *
   **********************************/
  template<class Acc>
  bool localSampling::apply(Acc& acc,
                            const matrix<typename Acc::value_type>& src1,
                            const matrix<float>& src2,
                            const location& loc,
                            dvector& dest, 
                            const eResultOrder& order ) const {
    // for each new sampling a section has to be inserted 

    // try to cast into previous type
    switch (last_) {
      case 1: {
        const sliceSampling *s = dynamic_cast<const sliceSampling*>(this);
        if (s!=0) return s->apply(acc,src1,src2,loc,dest,order);
        break;
      }
      case 2: {
        const siftSampling *s = dynamic_cast<const siftSampling*>(this);
        if (s!=0) return s->apply(acc,src1,src2,loc,dest,order);
        break;
      }
      case 3: {
        const gridSampling *s = dynamic_cast<const gridSampling*>(this);
        if (s!=0) return s->apply(acc,src1,src2,loc,dest,order);
        break;
      }
    default: {
      }
    }

    if (this->name()=="lti::sliceSampling") {
      // remember cast
      last_ = 1;
      const sliceSampling *s = dynamic_cast<const sliceSampling*>(this);
      return s->apply(acc,src1,src2,loc,dest,order);
    } else
    if (this->name()=="lti::siftSampling") {
      // remember cast
      last_ = 2;
      const siftSampling *s = dynamic_cast<const siftSampling*>(this);
      return s->apply(acc,src1,src2,loc,dest,order);
    }
    if (this->name()=="lti::gridSampling") {
      // remember cast
      last_ = 3;
      const gridSampling *s = dynamic_cast<const gridSampling*>(this);
      return s->apply(acc,src1,src2,loc,dest,order);
    }
    setStatusString("could not invoke apply, sampling not known\n");
    return false;
  }

  template<class Acc>
  bool localSampling::apply(Acc& acc,
                            const matrix<typename Acc::value_type>& src1,
                            const matrix<float>& src2,
                            dvector& dest, 
                            const eResultOrder& order ) const {
    // for each new sampling a section has to be inserted 

    // try to cast into previous type
    switch (last_) {
      case 1: {
        const sliceSampling *s = dynamic_cast<const sliceSampling*>(this);
        if (s!=0) return s->apply(acc,src1,src2,dest,order);
        break;
      }
      case 2: {
        const siftSampling *s = dynamic_cast<const siftSampling*>(this);
        if (s!=0) return s->apply(acc,src1,src2,dest,order);
        break;
      }
      case 3: {
        const gridSampling *s = dynamic_cast<const gridSampling*>(this);
        if (s!=0) return s->apply(acc,src1,src2,dest,order);
        break;
      }
    default: {
      }
    }

    if (this->name()=="lti::sliceSampling") {
      // remember cast
      last_ = 1;
      const sliceSampling *s = dynamic_cast<const sliceSampling*>(this);
      return s->apply(acc,src1,src2,dest,order);
    } else
    if (this->name()=="lti::siftSampling") {
      // remember cast
      last_ = 2;
      const siftSampling *s = dynamic_cast<const siftSampling*>(this);
      return s->apply(acc,src1,src2,dest,order);
    }
    if (this->name()=="lti::gridSampling") {
      // remember cast
      last_ = 3;
      const gridSampling *s = dynamic_cast<const gridSampling*>(this);
      return s->apply(acc,src1,src2,dest,order);
    }
    setStatusString("could not invoke apply, sampling not known\n");
    return false;
  }

  template<class Acc>
  bool localSampling::apply(Acc& acc,
                            const matrix<typename Acc::value_type>& src1,
                            const matrix<float>& src2,
                            const list<location>& locs,
                            std::list<dvector>& dest,
                            const eResultOrder& order) const {
    // for each new sampling a section has to be inserted 

    // try to cast into previous type
    switch (last_) {
      case 1: {
        const sliceSampling *s = dynamic_cast<const sliceSampling*>(this);
        if (s!=0) return s->apply(acc,src1,src2,locs,dest,order);
        break;
      }
      case 2: {
        const siftSampling *s = dynamic_cast<const siftSampling*>(this);
        if (s!=0) return s->apply(acc,src1,src2,locs,dest,order);
        break;
      }
      case 3: {
        const gridSampling *s = dynamic_cast<const gridSampling*>(this);
        if (s!=0) return s->apply(acc,src1,src2,locs,dest,order);
        break;
      }
    default: {
      }
    }

    if (this->name()=="lti::sliceSampling") {
      // remember cast
      last_ = 1;
      const sliceSampling *s = dynamic_cast<const sliceSampling*>(this);
      return s->apply(acc,src1,src2,locs,dest,order);
    } else
    if (this->name()=="lti::siftSampling") {
      // remember cast
      last_ = 2;
      const siftSampling *s = dynamic_cast<const siftSampling*>(this);
      return s->apply(acc,src1,src2,locs,dest,order);
    }
    if (this->name()=="lti::gridSampling") {
      // remember cast
      last_ = 3;
      const gridSampling *s = dynamic_cast<const gridSampling*>(this);
      return s->apply(acc,src1,src2,locs,dest,order);
    }
    setStatusString("could not invoke apply, sampling not known\n");
    return false;
  }

}



#endif
