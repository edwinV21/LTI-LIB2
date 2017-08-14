/*
 * Copyright (C) 2007
 * Pablo Alvarado, Instituto Tecnológico de Costa Rica
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
 * \file   ltiGALContrastEnhancement.h
 *         Contains a class for contrast enhancement
 * 
 * \author Miguel Angel Aguilar Ulloa
 * \date   12/10/2007
 *
 * revisions ..: $Id: ltiGALContrastEnhancement.h,v 1.2 2009-11-12 03:53:47 alvarado Exp $
 */

#ifndef _LTI_G_A_L_CONTRAST_ENHANCEMENT_
#define _LTI_G_A_L_CONTRAST_ENHANCEMENT_

#include "ltiMatrix.h"
#include "ltiFastRelabeling.h"
#include "ltiContrastEnhancement.h"

namespace lti {

  /**
   * Class lti::galContrastEnhancement.
   *
   * This class implements the algorithm describe in the papers "Greedy
   * Algorithm for Local Contrast Enhacement" and "Contrast Enhacement of
   * Images using Human contrast Sensivity" by Kartic Subr, Aditi Majumder and
   * Sandy Irani. The name of the class galContrastEnhancement is by "Greedy
   * Algorithm for Local Contrast Enhacement".
   *
   * This algorithm is a local contrast enhacement method, the goal is to
   * enhance the local gradients, which are directly related to the local
   * contrast of an image. Contrast enhancement is pose as an optimization
   * problem that maximizes the average local contrast of an image. The
   * objetive function to maximize is:
   *
   * \f[f(\Omega) = 
   *    \frac{1}{4|\Omega|}\sum_{p \in \Omega}\sum_{q \in N_4(p)}
   *    \frac{I'(p)-I'(q)}{I(p)-I(q)}
   * \f]
   *
   * subject to the constraints,
   *
   * \f[1\le\frac{I'(p)-I'(q)}{I(p)-I(q)}\le(1+\delta)\f]
   *
   * \f[L\le I'(p)\le U\f]
   *
   * where scalar functions \f$I(p)\f$ and \f$I'(p)\f$ represent the gray value
   * at pixel p of the input and output images respectively, \f$\Omega\f$
   * denotes set of pixels that makes up the image, \f$|\Omega|\f$ denotes the
   * cardinality of \f$\Omega\f$, \f$N_4(p)\f$ denotes the set of four
   * neighbors of \f$p\f$. \f$L\f$ and \f$U\f$ are the lower and upper bounds
   * on the gray values, and \f$\delta>0\f$ is the single parameter that
   * controls the amound of enhancement achieved.  The first contraint assures
   * a bounded enhancement of gradients. The second constraint ensures that the
   * output image does not have saturated intensity values.
   *
   * @ingroup gContrastEnhancement
   */
  class galContrastEnhancement : public contrastEnhancement {
  public:
    /**
     * The parameters for the class galContrastEnhancement
     */
    class parameters : public contrastEnhancement::parameters {
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
       * Delta \f$\delta\f$: amount of enhacement achieved.
       *
       * Default value: 0.5.
       */
      float delta;

    };

    /**
     * Default constructor
     */
    galContrastEnhancement();

    /**
     * Construct a contrast enhancement functor using the given parameters
     */
    galContrastEnhancement(const parameters& par);

    /**
     * Construct a constrast enhancement functor using the given delta \f$\delta\f$.
     *
     * @param delta amount of contrast enhancement achieved.
     */
    galContrastEnhancement(const float delta);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    galContrastEnhancement(const galContrastEnhancement& other);

    /**
     * Destructor
     */
    virtual ~galContrastEnhancement();

    /**
     * Operates on the given argument.
     *
     * @param srcdest channel with the source data. The result
     *                will be left here too.
     * @return true if apply successful or false otherwise.
     */
    bool apply(channel& srcdest) const;

    /**
     * Operates on the given argument.
     *
     * @param srcdest channel8 with the source data. The result
     *                will be left here too.
     * @return true if apply successful or false otherwise.
     */
    bool apply(channel8& srcdest) const;

    /**
     * Operates on a copy of the given arguments.
     *
     * @param src channel with the source data.
     * @param dest channel where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel& src, channel& dest) const;

    /**
     * Operates on a copy of the given arguments.
     *
     * @param src channel8 with the source data.
     * @param dest channel8 where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel8& src, channel8& dest) const;

    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    galContrastEnhancement& copy(const galContrastEnhancement& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    galContrastEnhancement& operator=(const galContrastEnhancement& other);

    /**
     * Returns the complete name of the functor class
     */
    virtual const std::string& name() const;

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual galContrastEnhancement* clone() const;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual galContrastEnhancement* newInstance() const;

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;

  private:
    /**
     * Complement the given image
     */
    inline  bool complementImage(channel8& srcdest) const;

    /**
     * Sweep and process hillocks
     */
    inline bool processHillocks(channel8& srcdest,const float delta) const;

     /**
     * Apply the process 
     */
    bool enhance (channel8& srcdest) const;
  };
}

#endif

