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
 * \file   ltiSiftSampling_inline.h
 *         Contains inlined function definitions for the 
 *         class lti::siftSampling
 * \author Arnd Hannemann
 * \date   18.04.2005
 *
 * revisions ..: $Id: ltiSiftSampling_inline.h,v 1.2 2009-08-30 04:30:29 alvarado Exp $
 */

#ifndef _LTI_SIFT_SAMPLING_INLINE_H_
#define _LTI_SIFT_SAMPLING_INLINE_H_

namespace lti {
  // accumulate for two inputs
  template <class Acc>
  inline void siftSampling
  ::accumulate(std::vector<Acc>& accuVec,
               const typename Acc::value_type val, // interpolated value
               const float val2,                   // 2nd interpolated value
               const int x,                        // x-coordinate of masks
               const int y,                        // y-coordinate of masks
               const float ay) const {             // position to accumulate
      
    // get x-coordinate of position to accumulate
    const float ax = norm_.at(x);    

    // upper left accu
    const int ac1 = accu1_.at(y,x);
    if (ac1 != NA_) {
      const float weight = mask1_.at(y,x)*val2;
      accuVec[ac1].accumulate(val,weight,ax,ay);
    }
    
    // upper right accu
    const int ac2 = accu2_.at(y,x);
    if (ac2 != NA_) {
      const float weight = mask2_.at(y,x)*val2;
      accuVec[ac2].accumulate(val,weight,ax,ay);
    }
    
    // lower left accu
    const int ac3 = accu3_.at(y,x);
    if (ac3 != NA_) {
      const float weight = mask3_.at(y,x)*val2;
      accuVec[ac3].accumulate(val,weight,ax,ay);
    }
    
    // lower right accu
    const int ac4 = accu4_.at(y,x);
    if (ac4 != NA_) {
      const float weight = mask4_.at(y,x)*val2;
      accuVec[ac4].accumulate(val,weight,ax,ay);
    }
  }
  
  // accumulate for one source
  template <class Acc>
  inline void siftSampling
  ::accumulate(std::vector<Acc>& accuVec,
               const typename Acc::value_type val, // interpolated value
               const int x,                        // x-coordinate of masks
               const int y,                        // y-coordinate of masks
               const float ay) const {             // position to accumulate

    // get x-coordinate of position to accumulate
    const float ax = norm_.at(x);    
    
    // upper left accu
    const int ac1 = accu1_.at(y,x);
    if (ac1 != NA_) {
      const float weight = mask1_.at(y,x);
      accuVec[ac1].accumulate(val,weight,ax,ay);
    }
    
    // upper right accu
    const int ac2 = accu2_.at(y,x);
    if (ac2 != NA_) {
      const float weight = mask2_.at(y,x);
      accuVec[ac2].accumulate(val,weight,ax,ay);
    }
    
    // lower left accu
    const int ac3 = accu3_.at(y,x);
    if (ac3 != NA_) {
      const float weight = mask3_.at(y,x);
      accuVec[ac3].accumulate(val,weight,ax,ay);
    }
    
    // lower right accu
    const int ac4 = accu4_.at(y,x);
    if (ac4 != NA_) {
      const float weight = mask4_.at(y,x);
      accuVec[ac4].accumulate(val,weight,ax,ay);
    }    
    
  }

} // namespace lti

#endif
