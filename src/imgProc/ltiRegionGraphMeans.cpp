/*
 * Copyright (C) 2003, 2004, 2005, 2006
 * Lehrstuhl für Technische Informatik
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
 * \file   ltiRegionGraphMeans.cpp
 *         Graph merging class according to the mean value of image regions.
 * \author Pablo Alvarado
 * \date   25.10.2003
 *
 * revisions ..: $Id: ltiRegionGraphMeans.cpp,v 1.1 2008-04-22 02:26:28 alvarado Exp $
 */


#include "ltiMacroSymbols.h"

#include "ltiMath.h"
#include "ltiRegionGraphMeans.h"
#include "ltiRegionGraphMeans_template.h"
#include "ltiEuclidianDistance.h"
#include <limits>

namespace lti {
  // --------------------------------------------------
  // regionGraphEdge
  // --------------------------------------------------
  float regionGraphColorMeanDistance::operator()(const node_type& a,
                                                 const node_type& b,
                                                 const int& ) const {
    return static_cast<float>(sqrt(euclidianDistance(a.computeMean(),
                                                     b.computeMean())));
  }

  float regionGraphScalarMeanDistance::operator()(const node_type& a,
                                                  const node_type& b,
                                                  const int& ) const {
    return abs(a.computeMean()-b.computeMean());
  }

  float regionGraphColorHarisDistance::operator()(const node_type& a,
                                                  const node_type& b,
                                                  const int& ) const {
    const float l1 = static_cast<float>(a.size());
    const float l2 = static_cast<float>(b.size());
    // fixed:
    return static_cast<float>(l1*l2*(euclidianDistance(a.computeMean(),
                                                       b.computeMean()))/
                              (l1+l2));

    // this is buggy:
    //return l1*l2*sqrt(distanceSqr(a.computeMean(),b.computeMean()))/(l1+l2);
  }

  float regionGraphScalarHarisDistance::operator()(const node_type& a,
                                                   const node_type& b,
                                                   const int& ) const {
    const float l1 = static_cast<float>(a.size());
    const float l2 = static_cast<float>(b.size());
    return l1*l2*sqr(a.computeMean()-b.computeMean())/(l1+l2);
  }

  // explicit instantiation
  template class regionGraphMeansNode<float>;
  template class regionGraphMeansNode< rgbPixel<float> >;
  
  template class regionGraphColor<regionGraphColorMeanDistance>;
  template class regionGraphColor<regionGraphColorHarisDistance>;
  template class regionGraphGray<regionGraphScalarMeanDistance>;
  template class regionGraphGray<regionGraphScalarHarisDistance>;

}

