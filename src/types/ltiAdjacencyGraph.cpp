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

/*
 * \file   ltiAdjacencyGraph.cpp
 *         Contains the template class lti::adjacencyGraph
 *         used to manage graph data structures that represent adjacency
 *         relationships.
 * 
 * \author Pablo Alvarado
 * \date   23.10.2007
 *
 * revisions ..: $Id: ltiAdjacencyGraph.cpp,v 1.1 2008-04-21 01:31:21 alvarado Exp $
 */

#include "ltiAdjacencyGraph.h"

#include <limits>

namespace lti {

  // ------------------------------------------------------------------------
  // --                        Edges
  // ------------------------------------------------------------------------

  template<class T>
  const T symmetricEdgeTraits<T>::Invalid = typeInfo<T>::min();

  template<class T>
  const bool symmetricEdgeTraits<T>::Symmetric = true;

  template<class T>
  const T asymmetricEdgeTraits<T>::Invalid = typeInfo<T>::min();

  template<class T>
  const bool asymmetricEdgeTraits<T>::Symmetric = false;

  // explicit instantiations
  template class symmetricEdgeTraits<char>;
  template class symmetricEdgeTraits<short>;
  template class symmetricEdgeTraits<int>;
  template class symmetricEdgeTraits<float>;
  template class symmetricEdgeTraits<double>;

  template class asymmetricEdgeTraits<char>;
  template class asymmetricEdgeTraits<short>;
  template class asymmetricEdgeTraits<int>;
  template class asymmetricEdgeTraits<float>;
  template class asymmetricEdgeTraits<double>;

} // end of namespace lti
