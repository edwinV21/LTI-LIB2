/*
 * Copyright (C) 2007
 * Pablo Alvarado, Instituto Tecnol�gico de Costa Rica
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
 * \file   ltiNonLocalMeansDenoising.h
 *         Contains a class for image denoising
 * 
 * \author Miguel Angel Aguilar Ulloa
 * \date   29/09/2007
 *
 * revisions ..: $Id: ltiNonLocalMeansDenoising.h,v 1.1 2008-02-18 00:17:23 alvarado Exp $
 */

#ifndef _LTI_NON_LOCAL_MEANS_H_
#define _LTI_NON_LOCAL_MEANS_H_


#include "ltiMatrix.h"
#include "ltiDenoising.h"
#include "ltiBoundaryType.h"

namespace lti {

  /**
   * Non-local means denoising algorithm.
   *
   * This class implements the algorithm described in the paper "A non-local
   * algorithm for image denoising" by Antoni Buades, Bartomeu Coll and 
   * Jean-Michel Morel.
   *
   * This is an algorihtm for noise reduction in images with no distorsion in
   * the result.
   *
   * Given a discrete noisy image \f$ v = {v(i)|i \in I}\f$, the estimated
   * value \f$NL[v](i)\f$, for a pixel is computed as a weighted average of all
   * pixels in the image,
   *
   * \f[NL[v](i)=\sum_{j \in I}w(i,j)v(j)\f]
   *
   * where the family of weights \f$ {w(i,j)}_{j}\f$ depend on the similarity
   * between the pixels i and j, and satisfy the usual conditions \f$ 0 \le
   * w(i,j) \le 1\f$ and \f$ \sum_{j}w(i,j) = 1\f$.
   *
   * The similarity between two pixels i and j depends on the similarity of the
   * intensity gray level vectors \f$ v(N_{i})\f$ and \f$ v(N_{j})\f$, where
   * \f$ N_{k}\f$ denotes a square neighborhood of fixed size and centered at a
   * pixel \f$k\f$. This similarity is measured as a decreasing function of the
   * weighted Euclidean distance.
   *
   * The pixels with a similar grey level neighborhood to \f$ v(N_{i})\f$, have
   * larger weights in the average. These weights are defined as,
   *
   * \f[w(i,j)=\frac{1}{Z(i)}e^{-\frac{||v(N_{i})-v(N_{j})||}{h^2}}\f]
   * 
   * where \f$ Z(i)\f$ is the normalizing constant,
   *
   * \f[Z(i)=\sum_{j}e^{-\frac{||v(N_{i})-v(N_{j})||}{h^2}}\f]
   *
   * and the parameter h acts as a degree of filtering. It controls the decay
   * of the exponential function and therefore the decay of the weights as a
   * function of the Euclidean distances.
   *
   * @see nonLocalMeansDenoising::parameters.
   *
   * @ingroup gDenoising
   */
  class nonLocalMeansDenoising : public denoising {
  public:
    /**
     * The parameters for the class nonLocalMeansDenoising
     */
    class parameters : public denoising::parameters {
    public:
      /**
       * Default constructor
       */
      parameters();

      /**
       * Copy constructor
       * @param other the parameters object to be copied
       */
      parameters(const parameters& other);

      /**
       * Destructor
       */
      ~parameters();

      /**
       * Copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& copy(const parameters& other);

      /**
       * Copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& operator=(const parameters& other);

      /**
       * Returns the complete name of the parameters class.
       */
      virtual const std::string& name() const;

      /**
       * Returns a pointer to a clone of the parameters.
       */
      virtual parameters* clone() const;

      /**
       * Returns a pointer to a new instance of the parameters.
       */
      virtual parameters* newInstance() const;

      /**
       * Write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool write(ioHandler& handler,const bool complete=true) const;

      /**
       * Read the parameters from the given ioHandler
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
       * Size of the search and similarity windows
       *
       * Default value: 5x5
       */
      int windowSize;

      /**
       * Size of the subregion of in the input image, where the NL-means
       * algorithm will be applied. A bigger region improves the denoising
       * result but it is slower.  The complete algorithm employes the whole
       * image, but the computational cost involved does not justify the
       * improvement.
       *
       * Default value: 11x11
       */
      int subregionSize;

      /**
       * Variance of the gaussian kernel.
       *
       * Default value: 5
       */
      double variance;

      /**
       * This parameter measures the degree of filtering of the obtained
       * image. When we know the standard desviation of the noise, sigma, the
       * value of filterDegree should depend on it. (a good value is between
       * \f$ 10*\sigma^2\f$ and \f$15*\sigma^2\f$). Too large values may cause 
       * blured images.
       * 
       * Default value: 0.006f
       */
      float filterDegree;
    };

    /**
     * Default constructor
     */
    nonLocalMeansDenoising();

    /**
     * Construct a functor using the given parameters
     */
    nonLocalMeansDenoising(const parameters& par);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    nonLocalMeansDenoising(const nonLocalMeansDenoising& other);

    /**
     * Destructor
     */
    virtual ~nonLocalMeansDenoising();

    /**
     * Operates on the given argument.
     * @param srcdest channel with the source data.  The result
     *                 will be left here too.
     * @return true if successful, false otherwise.
     */
    bool apply(channel& srcdest) const;

    /**
     * Operates on the given argument.
     * @param srcdest channel8 with the source data.  The result
     *                 will be left here too.
     * @return true if successful, false otherwise.
     */
    bool apply(channel8& srcdest) const;

    /**
     * Operates on a copy of the given arguments.
     * @param src channel with the source data.
     * @param dest channel where the result will be left.
     * @return true if successful, false otherwise.
     */
    bool apply(const channel& src, channel& dest) const;

    /**
     * Operates on a copy of the given arguments.
     * @param src channel8 with the source data.
     * @param dest channel8 where the result will be left.
     * @return true if successful, false otherwise.
     */
    bool apply(const channel8& src, channel8& dest) const;

    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    nonLocalMeansDenoising& copy(const nonLocalMeansDenoising& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    nonLocalMeansDenoising& operator=(const nonLocalMeansDenoising& other);

    /**
     * Returns the complete name of the functor class
     */
    virtual const std::string& name() const;

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual nonLocalMeansDenoising* clone() const;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual nonLocalMeansDenoising* newInstance() const;

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;

  private:
    /**
     * This method implements the non-local means to a noise image
     *
     * @param src the input image, to be denoise.
     * @param dest the output image, denoise.
     * @return true if successful, false otherwise.
     */
    bool denoise(const channel& src, channel& dest) const;

  };
}

#endif

