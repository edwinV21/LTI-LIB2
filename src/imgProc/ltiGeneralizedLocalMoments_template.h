/*
 * Copyright (C) 1998 - 2005
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
 * \file   ltiGeneralizedLocalMoments_template.h
 *         This file contains the declaration of the accumulator used
 *         for generalizedLocalMoments.
 * \author Arnd Hannemann
 * \date   27.05.2005
 *
 * $Id: ltiGeneralizedLocalMoments_template.h,v 1.5 2007-11-16 02:59:21 alvarado Exp $
 */



namespace lti {

  // --------------------------------------------------
  // generalizedLocalMoments::noGPaccumulator<T>
  // --------------------------------------------------


  // default constructor
  template <typename T>
  generalizedLocalMoments::noGPaccumulator<T>::noGPaccumulator() {
    m_ = new double[getArraySize()];
    reset();
  }

  // Default Destructor
  template <typename T>
  generalizedLocalMoments::noGPaccumulator<T>::~noGPaccumulator() {
    delete m_;
    m_ = 0;
  }

  // copy constructor
  template <typename T>
  generalizedLocalMoments::noGPaccumulator<T>::noGPaccumulator(
    const noGPaccumulator<T>& other) {
    m_ = new double[getArraySize()];
    copy(other);
  }

  // copy()
  template <typename T>
  generalizedLocalMoments::noGPaccumulator<T>&
  generalizedLocalMoments::noGPaccumulator<T>::copy(
    const noGPaccumulator<T>& other) {
    memcpy(m_,other.m_,getArraySize());
    return *this;
  }

  
  // reset
  template <typename T>
  bool generalizedLocalMoments::noGPaccumulator<T>::reset() {
    int i;
    // setting all moments to null
    for (i=0;i<getArraySize();i++) {
      m_[i] = static_cast<double>(0);
    }
    return true;
  }

  // getResult
  template <typename T>
  bool generalizedLocalMoments::noGPaccumulator<T>::
    getResult(double result[]) {
   
    // copy elements
    memcpy(result,m_,getArraySize()*sizeof(double));
    return true;
  }

  // accumulate
  template <typename T>
  bool generalizedLocalMoments::noGPaccumulator<T>::
    accumulate(const T& value,
               const float posx,
               const float posy) {
    // not yet implemented for general types only for specialization
    return false;
  }

  // accumulate
  template <typename T>
  bool generalizedLocalMoments::noGPaccumulator<T>::
    accumulate(const T& value,
               const float weight,
               const float posx,
               const float posy) {
    // not yet implemented for general types only for specialization
    return false;
  }
  

  // getArraySize
  template <typename T>
  inline int generalizedLocalMoments::noGPaccumulator<T>::getArraySize() {
    return 1;
  }
  

  // --------------------------------------------------
  // generalizedLocalMoments::accumulator<T>
  // --------------------------------------------------


  // default constructor
  template <typename T>
  generalizedLocalMoments::accumulator<T>::accumulator() {
    m_ = new double[getArraySize()];
    reset();
  }

  // Default Destructor
  template <typename T>
  generalizedLocalMoments::accumulator<T>::~accumulator() {
    delete m_;
    m_ = 0;
  }

  // copy constructor
  template <typename T>
  generalizedLocalMoments::accumulator<T>::accumulator(
    const accumulator<T>& other) {
    m_ = new double[getArraySize()];
    copy(other);
  }

  // copy()
  template <typename T>
  inline generalizedLocalMoments::accumulator<T>&
  generalizedLocalMoments::accumulator<T>::copy(
    const accumulator<T>& other) {
    memcpy(m_,other.m_,getArraySize());
    return *this;
  }


  // reset()
  template <typename T>
  inline bool generalizedLocalMoments::accumulator<T>::reset() {
    int i;
    // setting all moments to null
    for (i=0;i<getArraySize();i++) {
      m_[i] = static_cast<double>(0);
    }
    return true;
  }

  // getResult()
  template <typename T>
  inline bool generalizedLocalMoments::accumulator<T>::
    getResult(double result[]) {
   
    // copy elements
    memcpy(result,m_,getArraySize()*sizeof(double));
    return true;
  }


  // accumulate
  template <typename T>
  inline bool generalizedLocalMoments::accumulator<T>::
    accumulate(const T& value,
               const float posx,
               const float posy) {
    // not yet implemented for general types only for specialization
    return false;
  }

  // accumulate
  template <typename T>
  inline bool generalizedLocalMoments::accumulator<T>::
    accumulate(const T& value,
               const float weight,
               const float posx,
               const float posy) {
    // not yet implemented for general types only for specialization
    return false;
  }
  
  // getArraySize
  template <typename T>
  inline int generalizedLocalMoments::accumulator<T>::getArraySize() {
    return 1;
  }


  //-------------------
  // generalizedLocalMoments::noGPaccumulator<rgbaPixel>
  //-------------------

  // default constructor
  generalizedLocalMoments::noGPaccumulator<rgbaPixel>::noGPaccumulator() {
    m_ = new double[getArraySize()];
    reset();
  }

  // Default Destructor
  generalizedLocalMoments::noGPaccumulator<rgbaPixel>::~noGPaccumulator() {
    delete m_;
    m_ = 0;
  }

  // copy constructor
  generalizedLocalMoments::noGPaccumulator<rgbaPixel>::noGPaccumulator(
    const noGPaccumulator<rgbaPixel>& other) {
    m_ = new double[getArraySize()];
    copy(other);
  }

  // copy()
  generalizedLocalMoments::noGPaccumulator<rgbaPixel>&
  generalizedLocalMoments::noGPaccumulator<rgbaPixel>::copy(
    const noGPaccumulator<rgbaPixel>& other) {
    memcpy(m_,other.m_,getArraySize());
    return *this;
  }

  // reset()
  bool generalizedLocalMoments::noGPaccumulator<rgbaPixel>::reset() {
    int i;
    // setting all moments to null
    for (i=0;i<getArraySize();i++) {
      m_[i] = static_cast<double>(0);
    }
    return true;
  }

  // getResult()
  bool generalizedLocalMoments::noGPaccumulator<rgbaPixel>::
    getResult(double result[]) {
   
    // copy elements
    memcpy(result,m_,getArraySize()*sizeof(double));
    return true;
  }

  inline int 
  generalizedLocalMoments::noGPaccumulator<rgbaPixel>::getArraySize() {
     return 22;
  }

  bool generalizedLocalMoments::
  noGPaccumulator<rgbaPixel>::accumulate(const rgbaPixel& value,
                                         const float weight,
                                         const float posx,
                                         const float posy) {
    // TODO: just a dummy implementation!  return false to get attention
    // DOES IT MAKE ANY SENSE WHAT IS DONE HERE?

    // splitting into colors and calculate common values
    const float r = weight*value.getRed()/255.f;   // /255
    const float g = weight*value.getGreen()/255.f; // /255
    const float b = weight*value.getBlue()/255.f;  // /255
    const float x = posx;
    const float y = posy;
    const float rg = r*g;
    const float rb = r*b;
    const float gb = g*b;
    const float xy = x * y;
    const float x2 = x * x;
    const float y2 = y * y;


    // one band
    m_[R_M] += r;   m_[G_M] += g;   m_[B_M] += b;
    m_[R_X] += r*x; m_[G_X] += g*x; m_[B_X] += b*x;
    m_[R_Y] += r*y; m_[G_Y] += g*y; m_[B_Y] += b*y;
    
    // one band with squared coordinates
    m_[R_XY] += r*xy; m_[G_XY] += g*xy; m_[B_XY] += b*xy; 
    m_[R_X2] += r*x2; m_[G_X2] += g*x2; m_[B_X2] += b*x2;
    m_[R_Y2] += r*y2; m_[G_Y2] += g*y2; m_[B_Y2] += b*y2;
    
    // two band combinations
    m_[RG_M] += rg;   m_[RB_M] += rb;   m_[GB_M] += gb;

    
    m_[M_M]++;

    return false; // when someone take a look if this makes sense, then return 
                  // true

  }

  bool generalizedLocalMoments::
  noGPaccumulator<rgbaPixel>::accumulate(const rgbaPixel& value,
                                         const float posx,
                                         const float posy) {
    
    // splitting into colors and calculate common values
    const float r = value.getRed()/255.f;   // /255
    const float g = value.getGreen()/255.f; // /255
    const float b = value.getBlue()/255.f;  // /255
    const float x = posx;
    const float y = posy;
    const float rg = r*g;
    const float rb = r*b;
    const float gb = g*b;
    const float xy = x * y;
    const float x2 = x * x;
    const float y2 = y * y;


    // one band
    m_[R_M] += r;   m_[G_M] += g;   m_[B_M] += b;
    m_[R_X] += r*x; m_[G_X] += g*x; m_[B_X] += b*x;
    m_[R_Y] += r*y; m_[G_Y] += g*y; m_[B_Y] += b*y;
    
    // one band with squared coordinates
    m_[R_XY] += r*xy; m_[G_XY] += g*xy; m_[B_XY] += b*xy; 
    m_[R_X2] += r*x2; m_[G_X2] += g*x2; m_[B_X2] += b*x2;
    m_[R_Y2] += r*y2; m_[G_Y2] += g*y2; m_[B_Y2] += b*y2;
    
    // two band combinations
    m_[RG_M] += rg;   m_[RB_M] += rb;   m_[GB_M] += gb;

    
    m_[M_M]++;

    return true;
  }


  //-------------------
  // generalizedLocalMoments::accumulator<rgbaPixel>
  //-------------------
  generalizedLocalMoments::accumulator<rgbaPixel>::accumulator() {
    m_ = new double[getArraySize()];
    reset();
  }

  // Default Destructor
  generalizedLocalMoments::accumulator<rgbaPixel>::~accumulator() {
    delete m_;
    m_ = 0;
  }

  // copy constructor
  generalizedLocalMoments::accumulator<rgbaPixel>::accumulator(
    const accumulator<rgbaPixel>& other) {
    m_ = new double[getArraySize()];
    copy(other);
  }

  // copy()
  generalizedLocalMoments::accumulator<rgbaPixel>&
  generalizedLocalMoments::accumulator<rgbaPixel>::copy(
    const accumulator<rgbaPixel>& other) {
    memcpy(m_,other.m_,getArraySize());
    return *this;
  }

  // reset()
  bool generalizedLocalMoments::accumulator<rgbaPixel>::reset() {
    int i;
    // setting all moments to null
    for (i=0;i<getArraySize();i++) {
      m_[i] = static_cast<double>(0);
    }
    return true;
  }

  // getResult()
  bool generalizedLocalMoments::accumulator<rgbaPixel>::
    getResult(double result[]) {
   
    // copy elements
    memcpy(result,m_,getArraySize()*sizeof(double));
    return true;
  }


  inline int 
  generalizedLocalMoments::accumulator<rgbaPixel>::getArraySize() {
    return 28;
  }

  bool generalizedLocalMoments::
  accumulator<rgbaPixel>::accumulate(const rgbaPixel& value,
                                     const float weight,
                                     const float posx,
                                     const float posy) {

    // TODO: FIXME: This is just a dummy implementation which might not make
    // any sense.  Just weighted each RGB value, and this may be nonsense.

    // splitting into colors and calculate common values
    const float r = weight*value.getRed()/255.f; // /255
    const float r2 = r*r;
    const float g = weight*value.getGreen()/255.f; // /255
    const float g2 = g*g;
    const float b = weight*value.getBlue()/255.f; // /255
    const float b2 = b*b;
    const float x = posx;
    const float y = posy;
    const float rg = r*g;
    const float rb = r*b;
    const float gb = g*b;


    // one band
    m_[R_M] += r;   m_[G_M] += g;   m_[B_M] += b;
    m_[R_X] += r*x; m_[G_X] += g*x; m_[B_X] += b*x;
    m_[R_Y] += r*y; m_[G_Y] += g*y; m_[B_Y] += b*y;
    
    // one band with squared color
    m_[R2_M] += r2;   m_[G2_M] += g2;   m_[B2_M] += b2; 
    m_[R2_X] += r2*x; m_[G2_X] += g2*x; m_[B2_X] += b2*x;
    m_[R2_Y] += r2*y; m_[G2_Y] += g2*y; m_[B2_Y] += b2*y;
    
    // two band combinations
    m_[RG_M] += rg;   m_[RB_M] += rb;   m_[GB_M] += gb;
    m_[RG_X] += rg*x; m_[RB_X] += rb*x; m_[GB_X] += gb*x;
    m_[RG_Y] += rg*y; m_[RB_Y] += rb*y; m_[GB_Y] += gb*y;

    // some more
//     m[M_X] += x;
//     m[M_Y] += y;
    m_[M_M]++;

    // not yet checked.  After reviewing this implementation, set to true:
    return false;
  }

  bool generalizedLocalMoments::
  accumulator<rgbaPixel>::accumulate(const rgbaPixel& value,
                                     const float posx,
                                     const float posy) {
    
    // splitting into colors and calculate common values
    const float r = value.getRed()/255.f; // /255
    const float r2 = r*r;
    const float g = value.getGreen()/255.f; // /255
    const float g2 = g*g;
    const float b = value.getBlue()/255.f; // /255
    const float b2 = b*b;
    const float x = posx;
    const float y = posy;
    const float rg = r*g;
    const float rb = r*b;
    const float gb = g*b;


    // one band
    m_[R_M] += r;   m_[G_M] += g;   m_[B_M] += b;
    m_[R_X] += r*x; m_[G_X] += g*x; m_[B_X] += b*x;
    m_[R_Y] += r*y; m_[G_Y] += g*y; m_[B_Y] += b*y;
    
    // one band with squared color
    m_[R2_M] += r2;   m_[G2_M] += g2;   m_[B2_M] += b2; 
    m_[R2_X] += r2*x; m_[G2_X] += g2*x; m_[B2_X] += b2*x;
    m_[R2_Y] += r2*y; m_[G2_Y] += g2*y; m_[B2_Y] += b2*y;
    
    // two band combinations
    m_[RG_M] += rg;   m_[RB_M] += rb;   m_[GB_M] += gb;
    m_[RG_X] += rg*x; m_[RB_X] += rb*x; m_[GB_X] += gb*x;
    m_[RG_Y] += rg*y; m_[RB_Y] += rb*y; m_[GB_Y] += gb*y;

    // some more
//     m[M_X] += x;
//     m[M_Y] += y;
    m_[M_M]++;

    return true;
  }

}

