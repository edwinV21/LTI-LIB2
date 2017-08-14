/*
 * Copyright (C) 2005-2007
 * Peter Doerfler
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
 * \file   ltiSinCos.h
 *         Contains platform and/or compiler specific implementations
 *         of sincos, sincosf, and sincosl - always appending _impl to
 *         the name. Don't use this file directly but include
 *         ltiMath.h instead which provides a template function
 *         sincos()
 *
 * \author Peter Doerfler
 * \date   27.10.2005
 *
 * $Id: ltiSinCos.h,v 1.1 2007-09-13 23:29:55 alvarado Exp $
 */


#ifndef _LTI_SIN_COS_H_
#define _LTI_SIN_COS_H_

#include "ltiTypes.h"
#include "ltiMacroSymbols.h"
#include <cmath>


namespace lti {

  namespace internal {
    // structure: first level: compiler, possibly second level with
    // machine type needed.
    
    // MacOSX somehow misses sincos() and sincosf()
#if defined(_LTI_GNUC_3) && !(defined(_LTI_MACOSX))
    
    // just wrap for constistency
    
    /**
     * Calculate the sine and cosine values of \p angle in one step if
     * the setup allows it.
     */
    void inline sincos_impl(double angle, double& sval, double& cval) {
      ::sincos(angle, &sval, &cval);
    }
    
    /**
     * Calculate the sine and cosine values of \p angle in one step if
     * the setup allows it.
     */
    void inline sincosf_impl(float angle, float& sval, float& cval) {
      ::sincosf(angle, &sval, &cval);
    }
    
#elif defined(_LTI_MSC_VER)
    
    // inline asm implementations
    
    /**
     * Calculate the sine and cosine values of \p angle in one step if
     * the setup allows it.
     */
    void inline sincos_impl(double angle, double& sval, double& cval) {
      /*
      __asm {
        
        fld QWORD PTR [angle]
          fsincos
          mov ebx, [cval]
          fstp QWORD PTR [ebx]
          mov ebx, [sval]
          fstp QWORD PTR [ebx]
          }  
      */
      sval = sin(angle);
      cval = cos(angle);
    }
    
    /**
     * Calculate the sine and cosine values of \p angle in one step if
     * the setup allows it.
     */
    void inline sincosf_impl(float angle, float& sval, float& cval) {
      /*
      __asm {
        
        fld DWORD PTR [angle]
          fsincos
          mov ebx, [cval]
          fstp DWORD PTR [ebx]
          mov ebx, [sval]
          fstp DWORD PTR [ebx]
          }  
      */
      sval = sin(angle);
      cval = cos(angle);
    }
    
#else
    
    // don't know how to do this right so just do the simple thing
    void inline sincos_impl(double angle, double& sval, double& cval) {
      sval = sin(angle);
      cval = cos(angle);
    }
    void inline sincosf_impl(float angle, float& sval, float& cval) {
      sval = sin(angle);
      cval = cos(angle);
    }
    
#endif // compilers
    
  }
}

#endif // _LTI_SIN_COS_H
