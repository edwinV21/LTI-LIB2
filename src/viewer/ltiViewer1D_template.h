/*
 * Copyright (C) 2010
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
 * \file   ltiViewer1D_template.h
 *         Contains the viewer for 1D data
 * \author Pablo Alvarado
 * \date   05.01.2005
 *
 * revisions ..: $Id: ltiViewer1D_template.h,v 1.1 2010-01-19 03:34:40 alvarado Exp $
 */

namespace lti {

  template<class V>
  bool viewer1D::show(const V& vct) {
    lock_.lock();

    delete data_;
    data_=0;
    if (std::numeric_limits<typename V::value_type>::is_integer) {
      ensureInformationType(Fixed,vct.size());
      ivector* tmp = new ivector();
      tmp->castFrom(vct);
      data_ = tmp;
    } else {
      ensureInformationType(Float,vct.size());
      dvector* tmp = new dvector();
      tmp->castFrom(vct);
      data_ = tmp;
    }

    if (notNull(painter_)) {
      painter_->invalidateCache();
    }
    
    lock_.unlock();
    return getMainWindow().show();
  }

  template<class V>
  void viewer1D::information::compute(const V& vct) {
    if (std::numeric_limits<typename V::value_type>::is_integer) {
      type=Fixed;
    } else {
      type=Float;
    }
    
    if (vct.empty()) {
      size = 0;
      min = max = 0.0;
      average = stdDeviation = 0.0;
    } else {
      size = vct.size();
      double acc(0.0),acc2(0.0);
      typename V::const_iterator it = vct.begin();
      const typename V::const_iterator eit = vct.end();
      min=max=(*it);
      while (it!=eit) {
        const double d(*it);
        acc+=d;
        acc2+=(d*d);
        minmax(d,min,max);
        ++it;
      }
      
      average=acc/size;
      stdDeviation = sqrt(acc2/size - average*average);
      
    }

  }



}
