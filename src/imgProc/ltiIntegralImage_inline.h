/*
 * Copyright (C) 2007 by Pablo Alvarado
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
 * \file   ltiIntegralImage_inline.h
 *         Contains the class lti::integralImage, 
 *         which is used as a preliminary stage of several other algorithms
 *         that need to convolve with boxes filled with a constant value.
 * \author Pablo Alvarado
 * \date   16.10.2007
 *
 * revisions ..: $Id: ltiIntegralImage_inline.h,v 1.11 2007-10-29 01:38:32 alvarado Exp $
 */

namespace lti {

  /*
   * Convenience method to use the computed integral image to produce the
   * sum of the results in the provided rectangle.
   *
   * @param intImage an integral image computed with apply()
   * @param from initial point of area to be internalSummed.
   * @param to final point of area to be internalSummed.
   */
  inline int32 integralImage::internalSum(const matrix<int32>& intImage,
                                          const int fromX, const int fromY,
                                          const int toX, const int toY) const {
    const int prevX = fromX-1;
    const int prevY = fromY-1;

    return (intImage.at(toY,toX) + intImage.at(prevY,prevX) -
            (intImage.at(prevY,toX) + intImage.at(toY,prevX)));
  }

  /*
   * Convenience method to use the computed integral image to produce the
   * sum of the results in the provided rectangle.
   *
   * @param intImage an integral image computed with apply()
   * @param from initial point of area to be internalSummed.
   * @param to final point of area to be internalSummed.
   */
  inline int32 integralImage::internalSum(const matrix<int32>& intImage,
                                          const ipoint& from,
                                          const ipoint& to) const {
    return internalSum(intImage,from.x,from.y,to.x,to.y);
  }


  /*
   * Convenience method to use the computed integral image to produce the
   * sum of the results in the provided rectangle.
   *
   * @param intImage an integral image computed with apply()
   * @param rect the delimiter of a region whose pixels have to be sumed.
   */
  inline int32 integralImage::internalSum(const matrix<int32>& intImage,
                                          const irectangle& rect) const {
    return internalSum(intImage,rect.ul,rect.br);
  }

  /*
   * Convenience method to use the computed integral image to produce the
   * sum of the results in the provided rectangle.
   *
   * @param intImage an integral image computed with apply()
   * @param from initial point of area to be internalSummed.
   * @param to final point of area to be internalSummed.
   */
  inline float integralImage::internalSum(const matrix<float>& intImage,
                                          const int fromX, const int fromY,
                                          const int toX, const int toY) const {
    const int prevX = fromX-1;
    const int prevY = fromY-1;

    return (intImage.at(toY,toX) + intImage.at(prevY,prevX) -
            (intImage.at(prevY,toX) + intImage.at(toY,prevX)));
  }


  /*
   * Convenience method to use the computed integral image to produce the
   * sum of the results in the provided rectangle.
   *
   * @param intImage an integral image computed with apply()
   * @param from initial point of area to be summed.
   * @param to final point of area to be summed.
   */
  inline float integralImage::internalSum(const fmatrix& intImage,
                                          const ipoint& from,
                                          const ipoint& to) const {
    return internalSum(intImage,from.x,from.y,to.x,to.y);
  }
  

  /*
   * Convenience method to use the computed integral image to produce the
   * sum of the results in the provided rectangle.
   *
   * @param intImage an integral image computed with apply()
   * @param rect the delimiter of a region whose pixels have to be sumed.
   */
  inline float integralImage::internalSum(const fmatrix& intImage,
                                          const irectangle& rect) const {
    return internalSum(intImage,rect.ul,rect.br);
  }

  // ------------------------------------------------------------------------
  //

#ifdef NDEBUG

  // implementation for the release mode:

#undef _LTI_INTEGRAL_IMAGE_INTERNAL_CONTEXT 
#define _LTI_INTEGRAL_IMAGE_INTERNAL_CONTEXT 

  template<typename T>
  inline T integralImage::sumDispatcher(const matrix<T>& intImage,
                                        const int fromX,const int fromY,
                                        const int toX,const int toY) const {
    switch(boundaryType_) {
      case NoBoundary:
        return sumNoBoundary(intImage,fromX,fromY,toX,toY);
        break;
      case Zero:
        return sumBox(intImage,fromX,fromY,toX,toY);
        break;
      case Constant:
        return sumConstant(intImage,fromX,fromY,toX,toY);
        break;
      case Periodic:
        return sumPeriodic(intImage,fromX,fromY,toX,toY);
        break;
      case Mirror:
        return sumMirror(intImage,fromX,fromY,toX,toY);
        break;
      default:
        return T();
    }
  }
  
#else

  // implementation for the debug mode:


#undef _LTI_INTEGRAL_IMAGE_INTERNAL_CONTEXT 
#define _LTI_INTEGRAL_IMAGE_INTERNAL_CONTEXT dispatcher<T>::


  template<typename T>
  integralImage::dispatcher<T>::dispatcher() {
  }


  template<typename T>
  integralImage::dispatcher<T>::~dispatcher() {
  }
  
  /*
   * Convenience method to use the computed integral image to produce the
   * sum of the results in the provided rectangle.
   *
   * @param intImage an integral image computed with apply()
   * @param from initial point of area to be internalSummed.
   * @param to final point of area to be internalSummed.
   */
  template<typename T>
  inline T 
  integralImage::dispatcher<T>::internalSum(const matrix<T>& intImage,
                                            const int fromX,const int fromY,
                                            const int toX,const int toY) const{
    const int prevX = fromX-1;
    const int prevY = fromY-1;
    return (intImage.at(toY,toX) + intImage.at(prevY,prevX) -
            (intImage.at(prevY,toX) + intImage.at(toY,prevX)));
  }

#endif // until here debug mode block


  /*
   * Generic implementation for the sumBox() methods for zero boundary
   */
  template<typename T>
  T integralImage:: _LTI_INTEGRAL_IMAGE_INTERNAL_CONTEXT
  sumBox(const matrix<T>& intImage,
         const int fromX,const int fromY,
         const int toX,const int toY) const {

    // force the window to be within the image, and leaving at least one
    // pixel at the top and left borders
    const ipoint nf(within(fromX,0,intImage.lastColumn()),
                    within(fromY,0,intImage.lastRow()));
    
    const ipoint nt(within(toX,0,intImage.lastColumn()),
                    within(toY,0,intImage.lastRow()));

    if ((nf.y>0) && (nf.x>0)) { 
      // if we have both borders, then we have to add two regions and subtract
      // two.
      const ipoint prev(nf.x-1,nf.y-1);
      return (intImage.at(nt) + intImage.at(prev) -
              (intImage.at(prev.y,nt.x) + intImage.at(nt.y,prev.x)));
    } else if (nf.y>0) {
      // if we have just a top border, but the region touches the left margin
      // then we need to subtract just one region from the main one.
      return (intImage.at(nt) - intImage.at(nf.y-1,nt.x));
    } else if (nf.x>0) {
      // if we have just a left border, but the region touches the top margin
      // then we need to subtract just one region from the main one.
      return (intImage.at(nt) - intImage.at(nt.y,nf.x-1));
    } else {
      // the windows touches both, left and top borders.
      return intImage.at(nt);
    }
  }

  // -----------------------------------------------------------
  // sumBox for NoBoudary
  // -----------------------------------------------------------
#ifndef NDEBUG

  template<typename T>
  integralImage::sumNoBoundary<T>::sumNoBoundary() {
  }

  template<typename T>
  T integralImage::sumNoBoundary<T>::sumBox(const matrix<T>& intImage,
                                            const int fromX,const int fromY,
                                            const int toX,const int toY) const
    
#else
    
  template<typename T>
  T integralImage::sumNoBoundary(const matrix<T>& intImage,
                                 const int fromX,const int fromY,
                                 const int toX,const int toY) const

#endif
  {
    // if any part of the box lies outside the image, then return nothing
    if ( (fromX < 0)                 || (fromY < 0) || 
         (toX >= intImage.columns()) || (toY >= intImage.rows()) ) {
      return T();
    } else {
      // otherwise this behaves as with a zero border
      return _LTI_INTEGRAL_IMAGE_INTERNAL_CONTEXT sumBox(intImage,
                                                         fromX,fromY,toX,toY);
    }

  } 

  // -----------------------------------------------------------
  // sumBox for Zero
  // -----------------------------------------------------------
#ifndef NDEBUG
  
  template<typename T>
  integralImage::sumZero<T>::sumZero() {
  }

  // -----------------------------------------------------------
  // sumBox for Constant
  // -----------------------------------------------------------

  template<typename T>
  integralImage::sumConstant<T>::sumConstant() {
  }

  /*
   * Generic implementation for the sumBox() methods for constant boundary
   */
  template<typename T>
  T integralImage::sumConstant<T>::sumBox(const matrix<T>& intImage,
                                       const int fromX,const int fromY,
                                       const int toX,const int toY) const

#else
  template<typename T>
  T integralImage::sumConstant(const matrix<T>& intImage,
                               const int fromX,const int fromY,
                               const int toX,const int toY) const

#endif
  {
    // ASSUMPTION: The window always overlaps the image

    // Accumulator for each block
    T acc(0);

    if ( (intImage.columns() > 1) && (intImage.rows() > 1) ) {

      // we can now confidently assume that the image is at least 2x2
      
      // Each side of the window can be split into one, two or three parts,
      // depending on how much the window covers the image.  The only possible
      // six configurations will have 1,2,3,4,6, or 9 different blocks

      // The three top blocks
      if (fromY < 0) {
        // potential blocks NW,N,NE

        if (fromX < 0) {
          // NW
          // just add the corner value times the number of pixels
          // note that we are multiplying two negative numbers
          acc += static_cast<T>(fromY*fromX)*intImage.at(0,0);
        }

        if (toX >= intImage.columns()) {
          // NE
          // we need to recover the image value at the NE corner
          const T val = (intImage.at(0,intImage.lastColumn()) -
                         intImage.at(0,intImage.lastColumn()-1));

          // note that we are multiplying two negative numbers
          acc += static_cast<T>(fromY*(intImage.lastColumn()-toX)) * val;
        }

        if ( (fromX < intImage.columns()) &&
             (toX >= 0) ) {
          // N
          // we need the sum of elements on the top border
          const T val = (intImage.at(0,within(toX,0,intImage.lastColumn())) -
                         ((fromX <= 0) ? T(0) : intImage.at(0,fromX-1)));

          acc += -fromY*val;
        }
      }

      // The three bottom blocks
      if (toY >= intImage.rows()) {
        const int side = toY - intImage.lastRow();
        // potential blocks SW,S,SE
        if (fromX < 0) {
          // SW
          // we need to recover the image value at the SW corner
          const T val = (intImage.at(intImage.lastRow(),0)-
                         intImage.at(intImage.lastRow()-1,0));
        
          acc += static_cast<T>(-side*fromX)*val;
        }

        if (toX >= intImage.columns()) {
          // SE
          // we need to recover the image value at the SE corner
          const ipoint last(intImage.lastColumn(),intImage.lastRow());
          const T val = this->internalSum(intImage,last.x,last.y,
                                          last.x,last.y);
        
          acc += static_cast<T>(side*(toX-intImage.lastColumn())) * val;
        }

        if ( (fromX < intImage.columns()) &&
             (toX >= 0) ) {
          // S
          // we need the sum of elements on the bottom border
          const T val = _LTI_INTEGRAL_IMAGE_INTERNAL_CONTEXT sumBox(intImage,
                                           fromX,intImage.lastRow(),
                                           toX,intImage.lastRow());

          acc += side*val;
        }
      }

      // The three middle blocks
      if ( (fromY < intImage.rows()) &&
           (toY >= 0) ) {
      
        const int fy     = max(0,fromY);
        const int ty     = min(intImage.lastRow(),toY);
        
        // potential blocks W,C,E
        if (fromX < 0) {
          // W
          const T val = intImage.at(ty,0)-((fy<=0)?T(0):intImage.at(fy-1,0));
          acc += static_cast<T>(-fromX)*val;
        }

        if (toX >= intImage.columns()) {
          // E
          const T val = _LTI_INTEGRAL_IMAGE_INTERNAL_CONTEXT sumBox(intImage,
                                           intImage.lastColumn(),fy,
                                           intImage.lastColumn(),ty);
          acc += static_cast<T>(toX-intImage.lastColumn())*val;
        }

        if ( (fromX < intImage.columns()) &&
             (toX >= 0) ) {
          // C;
          acc += _LTI_INTEGRAL_IMAGE_INTERNAL_CONTEXT sumBox(intImage,
                                                             fromX,fy,toX,ty);
        }
      }


      return acc;
    }

    // the rare cases now

    // empty image
    if (intImage.empty()) {
      return T();
    }


    if (intImage.columns() == 1) {

      if (intImage.rows() == 1) {
        // just a point
        return intImage.at(0,0)*((toY-fromY+1)*(toX-fromX+1));
      } else {

        // a thin vertical line
        const T thick = static_cast<T>(toX-fromX+1);
        if (fromY < 0) {
          // Top
          
          // just add the corner value times the number of pixels
          // note that we are multiplying two negative numbers
          acc += static_cast<T>(-thick*fromY)*intImage.at(0,0);
        }
        
        if (toY >= intImage.rows()) {
          // Bottom
          
          // we need to recover the image value at the bottom corner
          const T val = (intImage.at(intImage.lastRow(),0) -
                         intImage.at(intImage.lastRow()-1,0));
          
          // note that we are multiplying two negative numbers
          acc += static_cast<T>(thick*(toY-intImage.lastRow())) * val;
        }
        
        if ( (fromY < intImage.rows()) &&
             (toY >= 0) ) {
          // Center
          // we need the sum of elements on the line
          const T val = (intImage.at(min(intImage.lastRow(),toY),0) -
                         ((fromY <= 0) ? T(0) : intImage.at(fromY-1,0)));
          
          acc += thick*val;
        }
      }

      return acc;
      
    } else {
      // a thin horizontal line
      const int thick = toY-fromY+1;
      if (fromX < 0) {
        // Left
        // just add the corner value times the number of pixels
        acc += static_cast<T>(-thick*fromX)*intImage.at(0,0);
      }

      if (toX >= intImage.columns()) {
        // Right
        // we need to recover the image value at the NE corner
        const T val = (intImage.at(0,intImage.lastColumn()) -
                       intImage.at(0,intImage.lastColumn()-1));

        acc += static_cast<T>(thick*(toX-intImage.lastColumn())) * val;
      }

      if ( (fromX < intImage.columns()) &&
           (toX >= 0) ) {
        // we need the sum of elements in the line
        const T val = (intImage.at(0,min(intImage.lastColumn(),toX)) -
                       ((fromX <= 0) ? T(0) : intImage.at(0,fromX-1)));

        acc += thick*val;
      }

    }
    return acc;
  }

  // -----------------------------------------------------------
  // sumBox for Periodic
  // -----------------------------------------------------------

#ifndef NDEBUG

  template<typename T>
  integralImage::sumPeriodic<T>::sumPeriodic() {
  }

  template<typename T>
  T integralImage::sumPeriodic<T>::sumBox(const matrix<T>& intImage,
                                       const int fromX,const int fromY,
                                       const int toX,const int toY) const
#else
  template<typename T>
  T integralImage::sumPeriodic(const matrix<T>& intImage,
                               const int fromX,const int fromY,
                               const int toX,const int toY) const

#endif
  {
    
    // Accumulator for each block
    T acc(0);

    const int cols = intImage.columns();
    const int rows = intImage.rows();
    
    // the new from
    const ipoint nf((fromX < 0) ? (cols + (fromX % cols)) % cols 
                                : (fromX % cols),
                    (fromY < 0) ? (rows + (fromY % rows)) % rows 
                                : (fromY % rows));
    
    // the new to
    const ipoint nt(toX + (nf.x-fromX),toY + (nf.y-fromY));

    // we can now confidently assume that the image is at least 2x2

    // the advantage of the periodic pattern is that we can map shift the
    // window to always start within the image, so that we only need to
    // additionally check the E, SE, and S border blocks
    
    // Each side of the window can be split into one, two or three parts,
    // depending on how much the window covers the image.  The only possible
    // six configurations will have 1,2,3,4,6, or 9 different blocks, but
    // with the shift, we have limited the possibilities to 1, 2 or 3 blocks.
    
    // right border size
    const int sizeX = nt.x-intImage.lastColumn();
    
    // how many times does the image fit into the right block
    const int fullXE = sizeX/cols;
    // the remaining part to complete the right block
    const int partXE = sizeX%cols;
    
    const T all = intImage.at(intImage.lastRow(),intImage.lastColumn());
    
    // The two possible bottom blocks
    if (nt.y >= rows) {
      const int sizeY = nt.y - intImage.lastRow();
      
      // how many times does the image fit into the bottom block
      const int fullY = sizeY/rows;
      // the remaining part to complete the bottom block
      const int partY = sizeY%rows;
      
      // potential blocks SW,S,SE
      
      if (nt.x >= cols) {
        // SE
        
        // the SE block is partitioned in four blocks:
        //
        // --------------------
        // | FULL   | PART X  |
        // --------------------
        // | PART Y | PART XY |
        // --------------------
        
        // we need to recover the image value at the SE corner
        if (fullY > 0) {
          // we maybe have full and part y
          if (fullXE > 0) {
            // we have full
              acc += all*static_cast<T>(fullY*fullXE);
          } 
          if (partXE > 0) {
            // we have part x
            acc += intImage.at(intImage.lastRow(),partXE-1)*fullY;
          }
        }
        if (partY > 0) {
          // we maybe have part y and part xy
          if (fullXE > 0) {
            // we have part y
            acc += intImage.at(partY-1,intImage.lastColumn())*fullXE;
          } 
          if (partXE > 0) {
            // we have part xy
            acc += intImage.at(partY-1,partXE-1);
          }
        }
      }

      if ( (nf.x < cols) &&
           (nt.x >= 0) ) {
        // S

        // the S block is partitioned in two blocks:
        //
        // -----------
        // | PART X  |
        // -----------
        // | PART XY |
        // -----------

        if (fullY > 0) {
          acc += (intImage.at(intImage.lastRow(),
                              min(nt.x,intImage.lastColumn())) -
                  ( (nf.x <= 0) ? T(0) : intImage.at(intImage.lastRow(),
                                                     nf.x-1)))*fullY;
        }

        if (partY > 0) {
          acc += (intImage.at(partY-1,
                              min(nt.x,intImage.lastColumn())) -
                  ( (nf.x <= 0) ? T(0) : intImage.at(partY-1,
                                                     nf.x-1)));
            
        }
      }
    }

    // Just E remaining
           
    if (nt.x >= cols) {
      // E
        
      // the E block is partitioned in two blocks:
      //
      // --------------------
      // | PART Y | PART XY |
      // --------------------
      
      if (fullXE > 0) {
        // we have part y
        acc += (intImage.at(min(nt.y,intImage.lastRow()),
                            intImage.lastColumn()) -
                ( (nf.y <= 0) ?
                  T(0) :
                  intImage.at(nf.y-1,
                              intImage.lastColumn())))*fullXE;
      }

      if (partXE > 0) {
        // we have part xy
          
        acc += (intImage.at(min(nt.y,intImage.lastRow()),partXE-1) -
                ( (nf.y <= 0) ? T(0) : intImage.at(nf.y-1,partXE-1) ));
      }
    }
        
    // Central block always there ;
    acc += _LTI_INTEGRAL_IMAGE_INTERNAL_CONTEXT sumBox(intImage,nf.x,nf.y,
                              min(intImage.lastColumn(),nt.x),
                              min(intImage.lastRow(),nt.y));
      
    return acc;
  }
    

  // -----------------------------------------------------------
  // sumBox for Mirror
  // -----------------------------------------------------------

#ifndef NDEBUG

  template<typename T>
  integralImage::sumMirror<T>::sumMirror() {
  }

  template<typename T>
  T integralImage::sumMirror<T>::sumBox(const matrix<T>& intImage,
                                     const int fromX,const int fromY,
                                     const int toX,const int toY) const {

#else

  template<typename T>
  T integralImage::sumMirror(const matrix<T>& intImage,
                             const int fromX,const int fromY,
                             const int toX,const int toY) const {
#endif

    // ASSUMPTION: The window always overlaps the image

    // Accumulator for each block
    T acc(0);

    const int cols = intImage.columns();
    const int rows = intImage.rows();

    // empty image
    if (intImage.empty()) {
      return T();
    }

    // we can now confidently assume that the image is at least 1x1
      
    // Each side of the window can be split into one, two or three parts,
    // depending on how much the window covers the image.  The only possible
    // six configurations will have 1,2,3,4,6, or 9 different blocks

    // right border size
    const int sizeXE = toX-intImage.lastColumn();

    // how many times does the image fit into the right block
    const int fullXE = sizeXE/cols;
    // the remaining part to complete the right block
    const int partXE = sizeXE%cols;


    // left border size
    const int sizeXW = -fromX;

    // how many times does the image fit into the right block
    const int fullXW = sizeXW/cols;
    // the remaining part to complete the right block
    const int partXW = sizeXW%cols;

    const T all = intImage.at(intImage.lastRow(),intImage.lastColumn());


    // The three top blocks
    if (fromY < 0) {
      const int sizeY = -fromY;

      // how many times does the image fit into the upper block
      const int fullY = sizeY/rows;
      // the remaining part to complete the upper block
      const int partY = sizeY%rows;
        
      // potential blocks NW,N,NE

      if (fromX < 0) {
        // NW
          
        // the NW block is partitioned in four blocks:
        //
        // ---------------------
        // | PART XY | PART Y  |
        // ---------------------
        // | PART X  | FULL    |
        // ---------------------
          
        if (fullY > 0) {
          if (fullXW > 0) {
            // we have full
            acc += all*static_cast<T>(fullY*fullXW);              
          }

          if (partXW > 0) {
            // we have part x
            if (even(fullXW)) {
              // if even full screens then the part is on the left side
              acc += intImage.at(intImage.lastRow(),partXW-1)*fullY;
            } else {
              // if odd full screens then the part is on the right side
              acc += (all-intImage.at(intImage.lastRow(),
                                      intImage.lastColumn()-partXW))*fullY;
            }
          }
        }

        if (partY > 0) {
          if (fullXW > 0) {
            // we have part y
            if (even(fullY)) {
              // for even we need to get the upper part
              acc += intImage.at(partY-1,intImage.lastColumn())*fullXW;
            } else {
              // for odd we need to get the bottom image part
              acc += (all - intImage.at(intImage.lastRow()-partY,
                                        intImage.lastColumn()))*fullXW;
            }
          }
          if (partXW > 0) {
            // we have part xy
              
            // four possible cases
            if (even(fullY)) {
              // we need upper parts
              if (even(fullXW)) {
                // we need left side
                acc += intImage.at(partY-1,partXW-1);
              } else {
                // we need right side
                acc += (intImage.at(partY-1,intImage.lastColumn()) -
                        intImage.at(partY-1,intImage.lastColumn()-partXW));
              }
            } else {
              // we need bottom parts
              if (even(fullXW)) {
                // we need left side 
                acc += intImage.at(intImage.lastRow(),partXW-1) -
                  intImage.at(intImage.lastRow()-partY,partXW-1);
              } else {
                // we need right side
                acc += _LTI_INTEGRAL_IMAGE_INTERNAL_CONTEXT sumBox(intImage,
                                          cols-partXW,rows-partY,
                                          intImage.lastColumn(),
                                          intImage.lastRow());
              }
            }
                  
          }
        }
      }

      if (toX >= intImage.columns()) {
        // NE
        // the NE block is partitioned in four blocks:
        //
        // ---------------------
        // | PART  Y | PART XY |
        // ---------------------
        // | FULL    | PART X  |
        // ---------------------
          
        if (fullY > 0) {
          if (fullXE > 0) {
            // we have full
            acc += all*static_cast<T>(fullY*fullXE);
          }
          if (partXE > 0) {
            // we have part x
            if (odd(fullXE)) {
              // if odd full screens then the part is on the left side
              acc += intImage.at(intImage.lastRow(),partXE-1) *  fullY;
            } else {
              // if even full screens then the part is on the right side
              acc += (all-intImage.at(intImage.lastRow(),
                                      intImage.lastColumn()-partXE))* fullY;
            }
          }
        }
        if (partY > 0) {
          if (fullXE > 0) {
            // we have part y
            if (even(fullY)) {
              // for even we need to get the upper part
              acc += intImage.at(partY-1,intImage.lastColumn()) * fullXE;
            } else {
              // for odd we need to get the bottom image part
              acc += (all - intImage.at(intImage.lastRow()-partY,
                                        intImage.lastColumn())) * fullXE;
            }
          }

          if (partXE > 0) {
            // we have part xy
              
            // four possible cases
            if (even(fullY)) {
              // we need upper parts
              if (odd(fullXE)) {
                // we need left side
                acc += intImage.at(partY-1,partXE-1);
              } else {
                // we need right side
                acc += (intImage.at(partY-1,intImage.lastColumn()) -
                        intImage.at(partY-1,intImage.lastColumn()-partXE));
              }
            } else {
              // we need bottom parts
              if (odd(fullXE)) {
                // we need left side 
                acc += intImage.at(intImage.lastRow(),partXE-1) -
                  intImage.at(intImage.lastRow()-partY,partXE-1);
              } else {
                // we need right side
                acc += _LTI_INTEGRAL_IMAGE_INTERNAL_CONTEXT sumBox(intImage,
                                          cols-partXE,rows-partY,
                                          intImage.lastColumn(),
                                          intImage.lastRow());
              }
            }
                  
          }
        }
      }

      if ( (fromX < intImage.columns()) &&
           (toX >= 0) ) {
        // N

        // the N block is partitioned in two blocks:
        //
        // -----------
        // | PART XY |
        // -----------
        // | PART X  |
        // -----------
          
        if (fullY > 0) {
          // we have part x
          acc += (intImage.at(intImage.lastRow(),
                              min(toX,intImage.lastColumn())) -
                  ( (fromX <= 0) ? T(0) : intImage.at(intImage.lastRow(),
                                                      fromX-1))) * fullY;
        }
        if (partY > 0) {
          /// we have part xy
          if (even(fullY)) {
            // for even full blocks in y we need the upper part 
            acc += (intImage.at(partY-1,min(toX,intImage.lastColumn())) -
                    ( (fromX <= 0) ? T(0) : intImage.at(partY-1,
                                                        fromX-1)));
          } else {
            // for even full blocks in y we need the bottom part
            acc += _LTI_INTEGRAL_IMAGE_INTERNAL_CONTEXT sumBox(intImage,
                                      fromX,rows-partY,
                                      toX,intImage.lastRow());
          }
        }
      }
    }

    // The three bottom blocks
    if (toY >= intImage.rows()) {
      const int sizeY = toY - intImage.lastRow();

      // how many times does the image fit into the bottom block
      const int fullY = sizeY/rows;
      // the remaining part to complete the bottom block
      const int partY = sizeY%rows;

      // potential blocks SW,S,SE
      if (fromX < 0) {
        // SW
          
        // the SW block is partitioned in four blocks:
        //
        // ---------------------
        // | PART X  | FULL    |
        // ---------------------
        // | PART XY | PART Y  |
        // ---------------------
          
        if (fullY > 0) {
          if (fullXW > 0) {
            // we have full
            acc += all*static_cast<T>(fullY*fullXW);              
          }

          if (partXW > 0) {
            // we have part x
            if (even(fullXW)) {
              // if even full screens then the part is on the left side
              acc += intImage.at(intImage.lastRow(),partXW-1)*fullY;
            } else {
              // if odd full screens then the part is on the right side
              acc += (all-intImage.at(intImage.lastRow(),
                                      intImage.lastColumn()-partXW))*fullY;
            }
          }
        }

        if (partY > 0) {
          if (fullXW > 0) {
            // we have part y
            if (odd(fullY)) {
              // for even we need to get the upper part
              acc += intImage.at(partY-1,intImage.lastColumn())*fullXW;
            } else {
              // for odd we need to get the bottom image part
              acc += (all - intImage.at(intImage.lastRow()-partY,
                                        intImage.lastColumn()))*fullXW;
            }
          }
          if (partXW > 0) {
            // we have part xy
              
            // four possible cases
            if (odd(fullY)) {
              // we need upper parts
              if (even(fullXW)) {
                // we need left side
                acc += intImage.at(partY-1,partXW-1);
              } else {
                // we need right side
                acc += (intImage.at(partY-1,intImage.lastColumn()) -
                        intImage.at(partY-1,intImage.lastColumn()-partXW));
              }
            } else {
              // we need bottom parts
              if (even(fullXW)) {
                // we need left side 
                acc += intImage.at(intImage.lastRow(),partXW-1) -
                  intImage.at(intImage.lastRow()-partY,partXW-1);
              } else {
                // we need right side
                acc += _LTI_INTEGRAL_IMAGE_INTERNAL_CONTEXT sumBox(intImage,
                                          cols-partXW,rows-partY,
                                          intImage.lastColumn(),
                                          intImage.lastRow());
              }
            }
                  
          }
        }



      }

      if (toX >= intImage.columns()) {
        // SE
        // the SE block is partitioned in four blocks:
        //
        // ---------------------
        // | FULL    | PART X  |
        // ---------------------
        // | PART  Y | PART XY |
        // ---------------------
          
        if (fullY > 0) {
          if (fullXE > 0) {
            // we have full
            acc += all*static_cast<T>(fullY*fullXE);
          }
          if (partXE > 0) {
            // we have part x
            if (odd(fullXE)) {
              // if odd full screens then the part is on the left side
              acc += intImage.at(intImage.lastRow(),partXE-1) *  fullY;
            } else {
              // if even full screens then the part is on the right side
              acc += (all-intImage.at(intImage.lastRow(),
                                      intImage.lastColumn()-partXE))* fullY;
            }
          }
        }

        if (partY > 0) {
          if (fullXE > 0) {
            // we have part y
            if (odd(fullY)) {
              // for even we need to get the upper part
              acc += intImage.at(partY-1,intImage.lastColumn()) * fullXE;
            } else {
              // for odd we need to get the bottom image part
              acc += (all - intImage.at(intImage.lastRow()-partY,
                                        intImage.lastColumn())) * fullXE;
            }
          }

          if (partXE > 0) {
            // we have part xy
              
            // four possible cases
            if (odd(fullY)) {
              // we need upper parts
              if (odd(fullXE)) {
                // we need left side
                acc += intImage.at(partY-1,partXE-1);
              } else {
                // we need right side
                acc += (intImage.at(partY-1,intImage.lastColumn()) -
                        intImage.at(partY-1,intImage.lastColumn()-partXE));
              }
            } else {
              // we need bottom parts
              if (odd(fullXE)) {
                // we need left side 
                acc += intImage.at(intImage.lastRow(),partXE-1) -
                  intImage.at(intImage.lastRow()-partY,partXE-1);
              } else {
                // we need right side
                acc += _LTI_INTEGRAL_IMAGE_INTERNAL_CONTEXT sumBox(intImage,
                                          cols-partXE,rows-partY,
                                          intImage.lastColumn(),
                                          intImage.lastRow());
              }
            }
                  
          }
        }
      }

      if ( (fromX < intImage.columns()) &&
           (toX >= 0) ) {
        // S

        // the S block is partitioned in two blocks:
        //
        // -----------
        // | PART X  |
        // -----------
        // | PART XY |
        // -----------
          
        if (fullY > 0) {
          // we have part x
          acc += (intImage.at(intImage.lastRow(),
                              min(toX,intImage.lastColumn())) -
                  ( (fromX <= 0) ? T(0) : intImage.at(intImage.lastRow(),
                                                      fromX-1))) * fullY;
        }
        if (partY > 0) {
          /// we have part xy
          if (odd(fullY)) {
            // for even full blocks in y we need the upper part 
            acc += (intImage.at(partY-1,
                                min(toX,intImage.lastColumn())) -
                    ( (fromX <= 0) ? T(0) : intImage.at(partY-1,
                                                        fromX-1)));
          } else {
            // for even full blocks in y we need the bottom part
            acc += _LTI_INTEGRAL_IMAGE_INTERNAL_CONTEXT sumBox(intImage,
                                      fromX,rows-partY,
                                      toX,intImage.lastRow());
          }
        }


      }
    }

    // The three middle blocks
    if ( (fromY < intImage.rows()) &&
         (toY >= 0) ) {

      // potential blocks W,C,E
      if (fromX < 0) {
        // W

        // the W block is partitioned in two blocks:
        //
        // --------------------
        // | PART XY | PART Y |
        // --------------------

        if (fullXW > 0) {
          // we have part y
          acc += (intImage.at(min(toY,intImage.lastRow()),
                              intImage.lastColumn())-
                  ( (fromY <= 0) ? T(0) :
                    intImage.at(fromY-1,intImage.lastColumn())))*fullXW;
        }
        if (partXW > 0) {
          // we have part xy
          if (even(fullXW)) {
            // for even full blocks in x we need the left part 
            acc += (intImage.at(min(toY,intImage.lastRow()),partXW-1) -
                    ( (fromY <= 0) ? T(0) : intImage.at(fromY-1,
                                                        partXW-1)));
          } else {
            // for odd full blocks in x we need the right part 
            acc += _LTI_INTEGRAL_IMAGE_INTERNAL_CONTEXT sumBox(intImage,
                                      cols-partXW,fromY,
                                      intImage.lastColumn(),toY);
          }
        }
      }

      if (toX >= intImage.columns()) {
        // E

        // the E block is partitioned in two blocks:
        //
        // --------------------
        // | PART Y | PART XY |
        // --------------------
          
        if (fullXE > 0) {
          // we have part y
          acc += (intImage.at(min(toY,intImage.lastRow()),
                              intImage.lastColumn())-
                  ( (fromY <= 0) ? T(0) :
                    intImage.at(fromY-1,intImage.lastColumn())))*fullXE;
        }
        if (partXE > 0) {
          // we have part xy
          if (odd(fullXE)) {
            // for odd full blocks in x we need the left part 
            acc += (intImage.at(min(toY,intImage.lastRow()),partXE-1) -
                    ( (fromY <= 0) ? T(0) : intImage.at(fromY-1,
                                                        partXE-1)));
          } else {
            // for even full blocks in x we need the right part 
            acc += _LTI_INTEGRAL_IMAGE_INTERNAL_CONTEXT sumBox(intImage,
                                      cols-partXE,fromY,
                                      intImage.lastColumn(),toY);
          }
        }
      }

      if ( (fromX < intImage.columns()) &&
           (toX >= 0) ) {
        // C
        acc += _LTI_INTEGRAL_IMAGE_INTERNAL_CONTEXT sumBox(intImage,
                                                           fromX,fromY,
                                                           toX,toY);
      }
    }

    return acc;
  }

  /*
   * Convenience method to use the computed integral image to produce the
   * sum of the results in the provided rectangle.
   *
   * @param intImage an integral image computed with apply()
   * @param from initial point of area to be summed.
   * @param to final point of area to be summed.
   */
  inline int32 integralImage::sum(const matrix<int32>& intImage,
                                  const int fromX, const int fromY,
                                  const int toX, const int toY) const {
#ifndef NDEBUG
    return sumInt_->sumBox(intImage,fromX,fromY,toX,toY);
#else
    return sumDispatcher(intImage,fromX,fromY,toX,toY);
#endif
  }


  /*
   * Convenience method to use the computed integral image to produce the
   * sum of the results in the provided rectangle.
   *
   * @param intImage an integral image computed with apply()
   * @param from initial point of area to be summed.
   * @param to final point of area to be summed.
   */
  inline int32 integralImage::sum(const matrix<int32>& intImage,
                                  const ipoint& from,
                                  const ipoint& to) const {
#ifndef NDEBUG
    return sumInt_->sumBox(intImage,from.x,from.y,to.x,to.y);
#else
    return sumDispatcher(intImage,from.x,from.y,to.x,to.y);
#endif
  }


  /*
   * Convenience method to use the computed integral image to produce the
   * sum of the results in the provided rectangle.
   *
   * @param intImage an integral image computed with apply()
   * @param rect the delimiter of a region whose pixels have to be sumed.
   */
  inline int32 integralImage::sum(const matrix<int32>& intImage,
                                  const irectangle& rect) const {
#ifndef NDEBUG
    return sumInt_->sumBox(intImage,rect.ul.x,rect.ul.y,rect.br.x,rect.br.y);
#else
    return sumDispatcher(intImage,rect.ul.x,rect.ul.y,rect.br.x,rect.br.y);
#endif
  }

  /*
   * Convenience method to use the computed integral image to produce the
   * sum of the results in the provided rectangle.
   *
   * @param intImage an integral image computed with apply()
   * @param from initial point of area to be summed.
   * @param to final point of area to be summed.
   */
  inline float integralImage::sum(const matrix<float>& intImage,
                                  const int fromX, const int fromY,
                                  const int toX, const int toY) const {
#ifndef NDEBUG
    return sumFloat_->sumBox(intImage,fromX,fromY,toX,toY);
#else
    return sumDispatcher(intImage,fromX,fromY,toX,toY);
#endif
  }

  /*
   * Convenience method to use the computed integral image to produce the
   * sum of the results in the provided rectangle.
   *
   * @param intImage an integral image computed with apply()
   * @param from initial point of area to be summed.
   * @param to final point of area to be summed.
   */
  inline float integralImage::sum(const fmatrix& intImage,
                                  const ipoint& from,
                                  const ipoint& to) const {
#ifndef NDEBUG
    return sumFloat_->sumBox(intImage,from.x,from.y,to.x,to.y);
#else
    return sumDispatcher(intImage,from.x,from.y,to.x,to.y);
#endif
  }
  

  /*
   * Convenience method to use the computed integral image to produce the
   * sum of the results in the provided rectangle.
   *
   * @param intImage an integral image computed with apply()
   * @param rect the delimiter of a region whose pixels have to be sumed.
   */
  inline float integralImage::sum(const fmatrix& intImage,
                                  const irectangle& rect) const {
#ifndef NDEBUG
    return sumFloat_->sumBox(intImage,rect.ul.x,rect.ul.y,rect.br.x,rect.br.y);
#else
    return sumDispatcher(intImage,rect.ul.x,rect.ul.y,rect.br.x,rect.br.y);
#endif
  }
}


