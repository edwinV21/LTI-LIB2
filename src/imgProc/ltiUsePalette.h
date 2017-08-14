/*
 * Copyright (C) 1998, 1999, 2000, 2001
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
 * \file   ltiUsePalette.h
 *         Contains class to create a color image from a labeled mask and
 *         a palette of colors (one for each label).
 * \author Pablo Alvarado
 * \date   8.5.2001
 *
 * $Id: ltiUsePalette.h,v 1.4 2006-01-13 04:59:04 alvarado Exp $
 */

#ifndef _LTI_USE_PALETTE_H_
#define _LTI_USE_PALETTE_H_

#include "ltiImage.h"
#include "ltiVector.h"
#include "ltiFunctor.h"
#include "ltiKdTree.h"

namespace lti {
  /**
   *
   * Use color or gray-valued palette.
   *
   * This class has two uses:

   * - For a given labeled mask (which is usually an lti::imatrix or a
   *   lti::channel8) and a palette, replace in a destination image each region
   *   label by the corresponding palette entry.  This is a simple but frequent
   *   used operation when dealing with segmentation results, color
   *   quantization results, and other image partitioning operators.
   *

   * - For a given image and a palette, find which palette entry in the image
   *   is the most similar to each corresponding pixel, and generate an index
   *   image (lti::imatrix or lti::channel8) containing those indices.
   *
   * For the second operation mode you can choose in the parameters to use
   * a lti::kdTree in order to avoid a "brute-force" search.
   *
   * @see lti::computePalette
   *
   * @ingroup gRegionAnalysis
   */
  class usePalette : public functor {
  public:
    /**
     * the parameters for the class usePalette
     */
    class parameters : public functor::parameters {
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
       * returns name of this type
       */
      const std::string& name() const;

      /**
       * copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& copy(const parameters& other);

      /**
       * copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& operator=(const parameters& other);

      /**
       * returns a pointer to a clone of the parameters
       */
      virtual parameters* clone() const;

      /**
       * returns a pointer to a new instance of the parameters
       */
      virtual parameters* newInstance() const;

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
       * The palette to be used.  This is the vector of rgbaPixel
       * values to be used.
       *
       * Default Palete: gray-value palette (256 entries, where
       *                 ( colors.at(i) == rgbPixel(i,i,i) )
       */
      palette colors;

      /**
       * To efficiently get the best entry in a palette, other techniques
       * besides the "brute force" linear search exist.  This functor can
       * also use k-d trees.  
       *
       * This makes only sense if the same palette is going to be used
       * with different images, or if the images are big enough to compensate
       * the overhead of generating the k-d tree, task which only depends on
       * the size of the color palette.
       *
       * If this attribute is set to true, the linear search will be used,
       * avoiding the computation of the k-d Tree.
       *
       * If set to false, a k-d Tree will be computed from the \c colors
       * attribute at a time determined by the attribute \c kdTreeOnDemand.
       *
       * Default value: false (i.e. use k-d trees)
       */
      bool linearSearch;

      /**
       * For those apply methods that get directly a palette or k-d Tree,
       * this parameter will be ignored.
       *
       * For all other methods that use the color entries in the 
       * \c colors attribute this parameter determines when to compute
       * the k-d Tree (only if linearSearch is set to false):
       * - If true, the k-d Tree is computed the first time an apply() method
       *   is called.  This can take little bit time, and the total time
       *   execution for this first apply can be longer than a simple
       *   linear search (when the images are too small, for example).
       * - If false, the k-d Tree is computed when the parameters are set.
       * 
       * If you want to apply the same palette to many different images, set
       * this parameter to false in order to force the computation of the
       * k-d Tree off-line.
       *
       * Default value: false
       */
      bool kdTreeOnDemand;

      /**
       * Each leaf node of the k-d Tree can contain a number of
       * palette entries greater than one.  A linear search method
       * takes place on sets of maximal this size.
       *
       * Default value: 16
       */
      int bucketSize;
    };

    /**
     * default constructor
     */
    usePalette();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    usePalette(const usePalette& other);

    /**
     * destructor
     */
    virtual ~usePalette();

    /**
     * returns the name of this type
     */
    virtual const std::string& name() const;

    /**
     * Takes the matrix<ubyte> and uses its elements as index for the palette
     * in the parameters.  The result will be left in a new image.
     *
     * @param chnl the matrix<ubyte> with the indices for the palette
     * @param other the resulting image
     * @return true if successful, false otherwise
     */
    bool apply(const matrix<ubyte>& chnl,image& other) const;

    /**
     * Takes the matrix<int> and uses its elements as index for the palette in
     * the parameters.  The result will be left in a new image.
     *
     * @param chnl the matrix<int> with the indices for the palette
     * @param other the resulting image
     * @return true if successful, false otherwise
     */
    bool apply(const matrix<int>& chnl,image& other) const;

    /**
     * Takes the matrix<ubyte> and uses its elements as index for the given
     * palette.  The result will be left in a new image.
     *
     * @param chnl the matrix<ubyte> with the indices for the palette
     * @param thePalette palette to be used
     * @param other the resulting image
     * @return true if successful, false otherwise
     */
    bool apply(const matrix<ubyte>& chnl,
               const palette& thePalette,
               image& other) const;
    
    /**
     * Takes the matrix<int> and uses its elements as index for the given
     * palette.  The result will be left in a new image.
     *
     * @param chnl the matrix<int> with the indices for the palette
     * @param thePalette palette to be used
     * @param other the resulting image
     * @return true if successful, false otherwise
     */
    bool apply(const matrix<int>& chnl,
               const palette& thePalette,
               image& other) const;

    /**
     * Use gray valued palette.
     *
     * This method is provided for completeness.  It is not for color images,
     * but for gray valued ones, but since the principle used is the same than
     * with colored palettes, the method seems to be in a proper place here.
     * It complements the corresponding apply method of lti::computePalette to
     * compute gray-valued palettes from channels.
     *
     * @param regions regions map, containing for each region a palette index
     *        entry.
     * @param values gray-valued palette entries.  For each region with index
     *        \e i, the output channel will contain values.at(i).
     * @param dest output channel, the result of using the gray-valued palette
     *        for the given regions map will be left here.
     * @return true if successful, false otherwise.
     */
    bool apply(const matrix<ubyte>& regions,
               const vector<float>& values,
               fmatrix& dest) const;

    /**
     * Use gray valued palette.
     *
     * This method is provided for completeness.  It is not for color images,
     * but for gray valued ones, but since the principle used is the same than
     * with colored palettes, the method seems to be in a proper place here.
     * It complements the corresponding apply method of lti::computePalette to
     * compute gray-valued palettes from channels.
     *
     * @param regions regions map, containing for each region a palette index
     *        entry.
     * @param values gray-valued palette entries.  For each region with index
     *        \e i, the output channel will contain values.at(i).
     * @param dest output channel, the result of using the gray-valued palette
     *        for the given regions map will be left here.
     * @return true if successful, false otherwise.
     */
    bool apply(const matrix<int>& regions,
               const vector<float>& values,
               fmatrix& dest) const;

    /**
     * For each pixel in the given image, find the "best" entry in the palette
     * found in the parameters and leave the index of the entry in the
     * correspondig pixel of the matrix<ubyte>.
     *
     * @param img the color image (true color)
     * @param chnl the indices for each pixel in img of the correponding
     *             palette entry in the parameters object
     */
    bool apply(const image& img, matrix<ubyte>& chnl);

    /**
     * Find for each pixel in the given image the best entry in the
     * palette found in the parameters and leave the index of the
     * entry in the correspondig pixel of the matrix<int>.
     *
     * @param img the color image (true color)
     * @param chnl the indices for each pixel in img of the correponding
     *             palette entry in the parameters object
     */
    bool apply(const image& img, matrix<int>& chnl);

    /**
     * Find for each pixel in the given image the best entry of the
     * given palette and leave the index of that entry in the
     * correspondig pixel of the matrix<ubyte>.
     *
     * Note that with this method the parameters::colors attribute will be
     * ignored.  The internal k-d Tree will be computed using thePalette 
     * given here.
     *
     * @param img the color image (true color)
     * @param thePalette a color palette.  The index of the most similar color
     *                   entry within this palette to each pixel of the
     *                   <code>img</code> will be left in the <code>chnl</code>
     * @param chnl the indices for each pixel in img of the correponding
     *             palette entry in <code>thePalette</code>
     */
    bool apply(const image& img,const palette& thePalette,matrix<ubyte>& chnl);

    /**
     * Find for each pixel in the given image the best entry of the
     * given palette and leave the index of that entry in the
     * correspondig pixel of the matrix<int>.
     *
     * Note that with this method the parameters::colors attribute will be
     * ignored.
     *
     * @param img the color image (true color)
     * @param thePalette a color palette.  The index of the most similar color
     *                   entry within this palette to each pixel of the
     *                   <code>img</code> will be left in the <code>chnl</code>
     * @param chnl the indices for each pixel in img of the correponding
     *             palette entry in <code>thePalette</code>
     */
    bool apply(const image& img, const palette& thePalette, matrix<int>& chnl);

    /**
     * Find for each pixel in the given image the nearest entry in the
     * given k-d Tree and leave the data component of that entry in the
     * correspondig pixel of the matrix<ubyte>.
     *
     * Note that with this method the parameters::colors attribute or
     * previously computed k-d trees will be ignored.
     *
     * @param img the color image (true color)
     * @param tree k-d Tree coding a color palette.  The index in
     *             the integer data component of the tree elements will
     *             be assumed to be the index of a color palette.
     * @param chnl the data of the nearest component for each pixel 
     *             in img will be stored in the correponding pixel of this
     *             channel.
     */
    bool apply(const image& img,
               const kdTree<rgbaPixel,int>& tree,
               matrix<ubyte>& chnl) const;

    /**
     * Find for each pixel in the given image the nearest entry in the
     * given k-d Tree and leave the data component of that entry in the
     * correspondig pixel of the matrix<ubyte>.
     *
     * Note that with this method the parameters::colors attribute or
     * previously computed k-d trees will be ignored.
     *
     * @param img the color image (true color)
     * @param tree k-d Tree coding a color palette.  The index in
     *             the integer data component of the tree elements will
     *             be assumed to be the index of a color palette.
     * @param chnl the data of the nearest component for each pixel 
     *             in img will be stored in the correponding pixel of this
     *             channel.
     */
    bool apply(const image& img,
               const kdTree<rgbaPixel,int>& tree,
               matrix<int>& chnl) const;

    /**
     * Get a constant reference to the internal k-d tree.
     */
    const kdTree<rgbaPixel,int>& getKdTree() const;

    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    usePalette& copy(const usePalette& other);

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual usePalette* clone() const;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual usePalette* newInstance() const;

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * Set functor's parameters.
     * This member makes a copy of <em>theParam</em>: the functor
     * will keep its own copy of the parameters!
     * @return true if successful, false otherwise
     */
    virtual bool updateParameters();

  protected:
    /**
     * The parameters palette is converted (if desired) into this
     * k-d Tree.
     *
     * The "data" type int states for the entry index.
     */ 
    kdTree<rgbaPixel,int> tree_;

    /**
     * Build the k-d Tree from the given color palette
     */
    bool buildKdTree(const palette& pal);

    /**
     * Flag to indicate if the k-d Tree has already been build or not.
     */
    bool treeBuilded_;
  };
}

#endif

