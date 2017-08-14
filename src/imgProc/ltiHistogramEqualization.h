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
 * \file   ltiHistogramEqualization.h
 *         Contains a class for histogram equalization of different types of
 *         matrices
 * \author Thomas Erger
 * \author Pablo Alvarado
 * \date   14.04.1999 (LTI-Lib 1)
 * \date   06.01.2006 (LTI-Lib 2)
 *
 * $Id: ltiHistogramEqualization.h,v 1.6 2007-10-18 18:57:26 alvarado Exp $
 */

#ifndef _LTI_HISTOGRAM_EQUALIZATION_H_
#define _LTI_HISTOGRAM_EQUALIZATION_H_

#include "ltiFunctor.h"
#include "ltiGenericMatrix.h"
#include "ltiMatrixProcessingInterface.h"
#include "ltiContrastEnhancement.h"

namespace lti {
  /**
   * Histogram equalization.
   *
   * lti::histogramEqualization equalizes the values of the given matrices or
   * vector within a specific input range, which at the same time is mapped to
   * a given output range.
   *
   * The number of "bins" or "cells" in the histogram can be defined by the
   * user.
   *
   * The algorithm used can be found in Sonka et. al "Image Processing,
   * Analysis and Machine Vision" 2nd Edition, PWS Publishing, pp. 60-61
   *
   * The parameters to indicate value ranges of input and output are always
   * expected in a normalized form, i.e. in an interval from 0.0 to 1.0, where
   * the normalization norm is obtained with typeInfo<T>::suggestedNorm().
   *
   * Example using lti::histogramEqualization
   * \code
   *  // The channel to be equalized and the result channel
   *  lti::channel src,dest;
   *
   *  // ... initialize channel src here ...
   *
   *  // set parameters and construct the functor
   *  lti::histogramEqualization::parameters param;  // parameters
   *  param.cells = 256;           // number of histogram bins
   *  param.lowerInputLimit=0.0;   // equalizes the dark values
   *  param.upperInputLimit=0.2;   //
   *  param.lowerOutputLimit=0.6;  // renders the equalized values lighter
   *  param.upperOutputLimit=1.0;  //
   *  lti::histogramEqualization equalizer(param);          // functor
   *
   *  // equalizes the values within input range, maps them to the output range
   *  // and returns the partly equalized channel in dest.
   *  equalizer.apply(src,dest);
   * \endcode
   *
   * @ingroup gContrastEnhancement
   */
  class histogramEqualization : public contrastEnhancement,
                                public matrixProcessingInterface<ubyte>,
                                public matrixProcessingInterface<float> {
  public:
    /**
     * The parameters for the class lti::histogramEqualization
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
       * Write the parameters in the given ioHandler.
       *
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool write(ioHandler& handler,const bool complete=true) const;

      /**
       * Write the parameters in the given ioHandler.
       *
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
       * The least normalized value equalized.
       *
       * Use always values in the range between 0.0 and 1.0.
       *
       * Depending on the matrix type used, the values will be de-normalized
       * using the typeInfo<T>::suggestedNorm().
       *
       * Default 0.0
       */
      float lowerInputLimit;

      /**
       * The least value the equalized values are mapped to.
       *
       * Use always values in the range between 0.0 and 1.0.
       *
       * Depending on the matrix type used, the values will be de-normalized
       * using the typeInfo<T>::suggestedNorm().
       *
       * Default 0.0
       */
      float lowerOutputLimit;

      /**
       * The number of cells, the histogram uses for equalization
       *
       * Default 256
       */
      int cells;

      /**
       * The highest value equalized
       *
       * Use always values in the range between 0.0 and 1.0.
       *
       * Depending on the matrix type used, the values will be de-normalized
       * using the typeInfo<T>::suggestedNorm().
       *
       * Default 1.0
       */
      float upperInputLimit;

      /**
       * The highest value the equalized values are mapped to.
       *
       * Use always values in the range between 0.0 and 1.0.
       *
       * Depending on the matrix type used, the values will be de-normalized
       * using the typeInfo<T>::suggestedNorm().
       *
       * Default 1.0
       */
      float upperOutputLimit;
    };

    /**
     * Default constructor.
     */
    histogramEqualization();

    /**
     * Default constructor with parameters.
     */
    histogramEqualization(const parameters& params);

    /**
     * Copy constructor.
     *
     * @param other the object to be copied
     */
    histogramEqualization(const histogramEqualization& other);

    /**
     * Destructor
     */
    virtual ~histogramEqualization();

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
    histogramEqualization& copy(const histogramEqualization& other);

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual histogramEqualization* clone() const;

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual histogramEqualization* newInstance() const;

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * Initialize some frequently used constants
     */
    virtual bool updateParameters();

    /**
     * @name Matrix processing interface
     */
    //@{
    /**
     * Equalizes the values of the given matrix of floats (channels).
     *
     * @param srcdest Source image and result holder
     * @return true if ok, false otherwise.

     */
    bool apply(matrix<float>& srcdest) const;

    /**
     * Equalizes the values of the given matrix of ubytes (channel8s).
     *
     * @param srcdest Source image and result holder
     * @return true if ok, false otherwise.
     */
    bool apply(matrix<ubyte>& srcdest) const;

    /**
     * Equalize src and leave the result in dest.
     * @param src the input data
     * @param dest the channel where the result will be left
     * @return true if ok, false otherwise.
     */
    bool apply(const matrix<float>& src,
                     matrix<float>& dest) const;

    /**
     * Equalize src and leave the result in dest.
     * @param src the input data
     * @param dest the channel8 where the result will be left
     * @return true if ok, false otherwise.
     */
    bool apply(const matrix<ubyte>& src,
                     matrix<ubyte>& dest) const;

    //@}

    /**
     * Equalizes the values of the given vector of floats
     * @return true if ok, false otherwise.
     */
    bool apply(vector<float>& srcdest) const;

    /**
     * Equalizes the values of the given vector of ubytes
     * @return true if ok, false otherwise.
     */
    bool apply(vector<ubyte>& srcdest) const;

    /**
     * Generates the vector dest as the histogram equalization of the values
     * in the vector src.
     *
     * @param src the input data
     * @param dest the fvector where the result will be left
     * @return true if ok, false otherwise.
     */
    bool apply(const vector<float>& src,
                     vector<float>& dest) const;

    /**
     * Generates the vector dest as the histogram equalization of the values
     * in the vector src.
     *
     * @param src the input data
     * @param dest the vector where the result will be left
     * @return true if ok, false otherwise.
     */
    bool apply(const vector<ubyte>& src,
                     vector<ubyte>& dest) const;
    
    /**
     * @name Contrast enhancement interface
     */
    //@{
    /**
     * Equalizes the values of the given matrix of floats (channels).
     * @return true if ok, false otherwise.
     */
    bool apply(channel& srcdest) const;

    /**
     * Equalizes the values of the given matrix of ubytes (channel8s).
     * @return true if ok, false otherwise.
     */
    bool apply(channel8& srcdest) const;

    /**
     * Equalize src and leave the result in dest.
     * @param src the input data
     * @param dest the channel where the result will be left
     * @return true if ok, false otherwise.
     */
    bool apply(const channel& src,
                     channel& dest) const;

    /**
     * Equalize src and leave the result in dest.
     * @param src the input data
     * @param dest the channel8 where the result will be left
     * @return true if ok, false otherwise.
     */
    bool apply(const channel8& src,
                     channel8& dest) const;

    //@}

    /**
     * @name Partial computations
     *
     * Following methods are public since there are applicactions that need to
     * compute the statistics once (which are relatively expensive), and apply
     * them at different times.  The apply() methods are implemented using
     * these ones in the way indicated.
     */
    //@{
    /**
     * Inline method to compute from a pixel value the index of
     * the LUT obtained with the method computeLUT().
     */
    inline float getEqualized(const float p,const vector<float>& lut) const;

    /**
     * Compute LUT
     *
     * This method computes the LUT used to equalize the histogram of
     * a given floating-point channel.
     * 
     * If you want to compute the final value q of a pixel p, then you can use
     * something like:
     * 
     * \code
     *   channel chnl; // your channel
     *   // initialize chnl here
     *   ...
     *   // compute a Look-Up Table (LUT)
     *   vector<float> lut;
     *   histogramEqualization histEq;
     *   histEq.computeLUT(chnl,lut);
     *
     *   q = histEq.getEqualized(p,lut);
     * \endcode
     */
    bool computeLUT(const matrix<float>& chnl,
                    vector<float>& lut) const;

    /**
     * Compute LUT
     *
     * This method computes the LUT used to equalize the histogram of
     * a given channel8.
     * 
     * If you want to compute the final value q of a pixel p, then you can use
     * something like:
     * 
     * \code
     *   channel8 chnl; // your channel
     *   // initialize chnl here
     *   ...
     *   // compute a Look-Up Table (LUT)
     *   vector<ubyte> lut;
     *   histogramEqualization histEq;
     *   histEq.computeLUT(chnl,lut);
     *
     *   q = lut.at(p);
     * \endcode
     */
    bool computeLUT(const matrix<ubyte>& chnl,
                    vector<ubyte>& lut) const;
    //@}


  protected:
    /**
     * Shadow of parameters lowerInputLimit
     */
    float lower_;

    /**
     * Shadow of parameters upperInputLimit
     */
    float upper_;

    /**
     * Precomputed constant cst = (par.cells-1)/(upper-lower);
     */
    float cst_;

    /**
     * Shadow of parameters lowerInputLimit scaled by a norm of 255
     */
    int loweru_;

    /**
     * Shadow of parameters upperInputLimit scaled by a norm of 255
     */
    int upperu_;

  };

  // some inline methods
  
  inline float 
  histogramEqualization::getEqualized(const float p,
                                      const vector<float>& lut) const {
    return 
      ((p>=lower_) && (p<=upper_)) ?
      lut.at(static_cast<int>( 0.5f+ (p-lower_)*cst_ )) : p;
  }

}

#endif

