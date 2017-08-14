/*
 * Copyright (C) 1998-2006
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
 * \file   ltiKMColorQuantization.h
 *         Contains a class for color quantization using the k-Means algorithm.
 * \author Pablo Alvarado
 * \date   30.04.1999 (LTI-Lib 1)
 * \date   12.01.2006 (LTI-Lib 2)
 *
 * $Id: ltiKMColorQuantization.h,v 1.2 2006-01-15 22:15:27 alvarado Exp $
 */


#ifndef _LTI_KM_COLOR_QUANTIZATION_H_
#define _LTI_KM_COLOR_QUANTIZATION_H_

#include "ltiFunctor.h"
#include "ltiImage.h"
#include "ltiTypes.h"
#include "ltiVector.h"
#include "ltiColorQuantization.h"
#include "ltiRGBPixel.h"
#include <map>

namespace lti {
  /**
   * k-Means based color quantization.
   *
   * This functor calculates (using k-Means) an optimal color subpalette for
   * the input image.  The maximal number of colors to be used is given
   * through the parameters (inherited from colorQuantization::parameters).
   *
   * If the real number of colors used in the image is less than the desired
   * number of quantized colors, no quantization is done and the output palette
   * will contain all image colors, i.e. it will be smaller that the expected
   * size of parameters::numberOfColors.
   *
   * @ingroup gColorQuantization
   */
  class kMColorQuantization : public colorQuantization {
  public:

    /**
     * the parameters for the class kMColorQuantization
     */
    class parameters : public colorQuantization::parameters {
    public:
      /**
       * default constructor
       */
      parameters();

      /**
       * copy constructor
       * @param other the parameters object to be copied
       */
      parameters(const parameters& other);

      /**
       * destructor
       */
      ~parameters();


      /**
       * Returns the name of this type
       */
      virtual const std::string& name() const;

      /**
       * Returns a pointer to a clone of the parameters
       */
      virtual parameters* clone() const;

      /**
       * Returns a pointer to a new instance of the parameters
       */
      virtual parameters* newInstance() const;

      /**
       * Copy the other instance.here.
       */
      parameters& copy(const parameters& other);

      /**
       * Copy the contents of a parameters object.
       *
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& operator=(const parameters& other);

      /**
       * write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool write(ioHandler& handler,const bool complete=true) const;

      /**
       * write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool read(ioHandler& handler,const bool complete=true);

      // ------------------------------------------------
      // the parameters
      // ------------------------------------------------

      /**
       * Maximal number of iterations taken for the k-Means algorithm to
       * converge.  
       *
       * Default value: 50
       */
      int maximalNumberOfIterations;

      /**
       * Smallest change of palette.
       *
       * If the difference between the palette in the previous iteration
       * and the palette in the current iteration is smaller than this value
       * then it will be assumed that the algorithm has converged.
       *
       * The difference is computed as the sum of the distances between
       * corresponding entries of the palette, taken in the RGB color space,
       * with an L2 distance, i.e. sum(distanceSqr(palNew-palOld)).
       *
       * Note that the axes of the RGB space are dimensioned from 0 to 255.
       *
       * Default value: 0.2
       */
      float thresholdDeltaPalette;

    };

    /**
     * Default constructor
     */
    kMColorQuantization();

    /**
     * Constructor with parameters
     */
    kMColorQuantization(const parameters& par);

    /**
     * Copy constructor
     */
    kMColorQuantization(const kMColorQuantization& other);

    /**
     * Destructor
     */
    virtual ~kMColorQuantization();

    /**
     * Returns current parameters.
     */
    const parameters& getParameters() const;

    /**
     * Returns the name of this type.
     */
    virtual const std::string& name() const;

    /**
     * Copy data of "other" functor.
     *
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    kMColorQuantization& copy(const kMColorQuantization& other);

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual kMColorQuantization* clone() const;

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual kMColorQuantization* newInstance() const;


    /**
     * Quantize the colors of src and leave the labels for the quantized colors
     * in dest, and in the palette entries corresponding to the labels the
     * mean colors for each label.
     *
     * @param src Original image with the true-color data
     * @param dest channel8 where the indexes of the also calculated palette
     *             will be.
     * @param thePalette the color palette used by the channel. If it is not
     *             empty, it will be taken as a InitPalette for
     *             the computation of the new one.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const image& src,
                             matrix<ubyte>& dest,
                             palette& thePalette) const;


    /**
     * Quantize the colors of src and leave the labels for the quantized colors
     * in dest, and in the palette entries corresponding to the labels the
     * mean colors for each label.
     *
     * @param src Original image with the true-color data
     * @param dest matrix<int> where the indexes of the calculated palette
     *             will be.
     * @param thePalette The color palette used by the matrix. If it is not
     *             empty, it will be taken as a InitPalette for
     *             the computation of the new one.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const image& src,
                             matrix<int>& dest,
                             palette& thePalette) const;


    /**
     * Quantize the colors of the given image.
     *
     * @param srcdest image with the source data.  The result
     *                 will be left here too.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(image& srcdest) const;

    /**
     * Quantize the colors of the given src image and leave the result in dest.
     *
     * @param src image with the source data.
     * @param dest image with only the number of colors specified in
     *             the parameters
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const image& src,image& dest) const;

  private:
    /**
     * functor to calculate the kMeans
     */
    class kMeanColor {
    public:
      /**
       * constructor
       */
      kMeanColor(const int& maxNumOfClasses,
		 const int& maxIterations,
		 const float& thresdDeltaPal);

      /**
       * destructor
       */
      virtual ~kMeanColor();

      /**
       * calculate palette and colorMap using k-Means.
       * img is the input image, and it will be modified to use
       * the quantized colors!
       */
      bool operator()(const image& src,matrix<int>& dest,palette& thePalette);

    protected:
      /**
       * An entry of the hash table contains the number of pixels in
       * the image that have the color corresponding to this entry (counter)
       * and the index of this color in the cetroid-vector;
       */
      struct hashEntry {
        hashEntry(const int& idx=0,const int& cnt=0)
          : index(idx),counter(cnt) {};
        int index;
        int counter;
      };

      /**
       * Each entry of the hash has this type
       */
      typedef std::map<int,hashEntry> hashMapType;

      /**
       * Hash table type (see theHash for more details).
       */
      typedef hashMapType* hashType;

      /**
       * the centroids
       */
      vector< frgbPixel > centroids_;

      /**
       * centroid elements
       */
      vector<int> centerElems_;

      /**
       * The hash table.
       *
       * The hash table contains all relevant information of the image to be
       * quantized, necessary for the k-Mean algorithms.
       *
       * It is organized as an array of 4096 (12 bits) hashMapType elements.
       *
       * This array is then accessed with the lower 12 bits of the color
       * value.  The upper 12 bits of the color are used to index the
       * proper map, that returns a hashEntry structure, with the correponding
       * centroid index for this color and the number of pixels assigned
       * to the centroid until now.
       *
       * In real world images, there will be an average between 5 and 10
       * elements per map.
       *
       * To access theHash directly, you can use the method at(), or put()
       */
      hashType theHash_;

      /**
       * the maximal number of classes
       */
      const int maxNumberOfClasses_;

      /**
       * the number of classes really used in the image
       */
      int realNumberOfClasses_;

      /**
       * maximum number of iterations for the k-Means
       */
      const int maxNumberOfIterations_;

      /**
       * if "Palette-Changing" by iteration < thresholdDeltaPalette
       * than stop iterating
       * sum(distanceSqr(palNew-palOld))
       */
      const float thresholdDeltaPalette_;

      /**
       * returns a reference to the hash entry for the given pixel
       */
      inline hashEntry& at(const rgbaPixel& px);

      /**
       * put the given pixel in the hash table (increments the counter if
       * already exists).
       * @return true if the pixel is new added, false otherwise.
       */
      inline bool put(const rgbaPixel& px);

      /**
       * create and initialize the hash table with the image values
       */
      void initialize(const image& src);

      /**
       * get initial palette from hash
       */
      void getInitialPalette(const lti::palette& thePalette);

      /**
       * iterate to find the clusters
       */
      void iterate();

      /**
       * get a random color from the color hash
       */
      rgbaPixel getAnImageColor();

      /**
       * size of the maps array
       */
      static const int firstKeySize_;

      /**
       * a counter to generate the random colors
       */
      int lastHashPosition_;

      /**
       * Random number generator from 0.0 to 1.0
       */
      double random() const;
    };

  };

}

#endif
