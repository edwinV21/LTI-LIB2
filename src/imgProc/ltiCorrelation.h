/*
 * Copyright (C) 1999, 2000, 2001, 2002, 2003, 2004, 2005, 2006, 2007
 * Pablo Alvarado, ITCR
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
 * \file   ltiCorrelation.h
 *         Provides implementation of a correlation functor.
 * \author Pablo Alvarado
 * \date   19.04.99
 *
 * $Id: ltiCorrelation.h,v 1.3 2010-08-02 17:37:46 alvarado Exp $
 */

#ifndef _LTI_CORRELATION_H_
#define _LTI_CORRELATION_H_

#include <vector>
#include "ltiArray.h"
#include "ltiMatrix.h"
#include "ltiTypes.h"
#include "ltiKernel1D.h"
#include "ltiKernel2D.h"
#include "ltiSeparableKernel.h"
#include "ltiFunctor.h"
#include "ltiMatrixProcessingInterface.h"
#include "ltiBoundaryType.h"

namespace lti {
  /**
   * Correlation %functor.
   *
   * This %functor correlates a given vector or matrix (given in the
   * correlation::parameters) with another vector or matrix.
   *
   * Due to the very nature of the correlation computation, the best
   * results are obtained using containers of floating point values,
   * as the results obtained with containers of ubyte values lack the
   * necessary dynamic resolution to represent the results.
   *
   * You can choose to correlate a rectangular kernel, or a selected region
   * within, specifying a corresponding mask in the parameters.  However, this
   * option is relatively slow.
   *
   * @see lti::gaussKernel, lti::gaborKernel
   *
   * Example using a gaussian kernel
   *
   * \code
   * // the channel to be filtered:
   * lti::channel data,result;
   *
   * // ... initialize channel here ...
   *
   * // gauss filter kernel with dimensions 5x5, and a variance of 1.3
   * lti::gaussKernel2D<lti::float> kernel(5,1.3);
   *
   * lti::correlation filter;                        // correlation operator
   * lti::correlation::parameters param;             // parameters
   * param.setKernel(kernel);                        // use the gauss kernel
   * filter.setParameters(param);                    // use given parameters
   *
   * // filter the channel and leave the result there too
   * filter.apply(data);
   * \endcode
   *
   * You can also create the functor with a given filter kernel:
   *
   * \code
   * lti::correlation filter(lti::gaussKernel2D<float>(5,1.3);
   * filter.apply(data); // parameters already set in the constructor!
   * \endcode
   *
   * The filter kernel can also be set through the parameters instance or with
   * the shortcut setKernel():
   *
   * \code
   * param.setKernel(anotherKernel);
   * filter.setParamters(param);
   *
   * // this is equivalent to:
   *
   * filter.setKernel(anotherKernel);
   *
   * \endcode
   */
  class correlation : public functor,
                      public matrixProcessingInterface<float>,
                      public matrixProcessingInterface<ubyte>,
                      public matrixProcessingInterface<double> {
  public:
    
    /**
     * Available correlation modes 
     */
    enum eMode {
      Classic, /*!< The correlation is defined as f(x)*k(-x), where "*"
                 specifies convolution, f(x) is the image or data where
                 the kernel k(x) must be searched
               */
      Coefficient, /*!< Calculates the correlation coefficient defined in
                     Gonzales&Woods, 1993, "Digital Image Processing",
                     pp. 584.
                     
                     \f$\gamma(s,t) =
                     \frac{\displaystyle\sum_{x,y}\left[f(x,y)-
                     \overline{f}(x,y)\right]
                     \left[w(x-s,y-t)-\overline{w}\right]}
                     {\left\{\displaystyle\sum_{x,y}\left[f(x,y)-
                     \overline{f}(x,y)\right]^2
                     \sum_{x,y}\left[w(x-s,y-t)-
                     \overline{w}\right]^2\right\}^{1/2}}\f$
                   */
      C1,  /*!< Calculates matching criteria C1 defined in Sonka et.al 2nd
             Edition, pp. 191:
             
             \f$C_1(u,v)=
             \displaystyle\frac{1}
             {\displaystyle\max_{(i,j)\in V}
             \left|{f(i+u,j+v)-h(i,j)}\right|+1}\f$
           */
      C2,  /*!< Calculates matching criteria C2 defined in Sonka et.al 2nd
             Edition, pp. 191:
             \f$C_2(u,v)=
             \displaystyle\frac{1}
             {\left(\displaystyle\sum_{(i,j)\in V}\left|
             f(i+u,j+v)-h(i,j)\right|+1\right)}\f$
             
           */
      C3  /*!< Calculates matching criteria C3 defined in Sonka et.al 2nd
            Edition, pp. 191:
            
            \f$C_3(u,v)=
            \displaystyle\frac{1}
            {\left(\displaystyle\sum_{(i,j)\in V}
            \left[f(i+u,j+v)-h(i,j)\right]^2+1\right)}\f$
          */
    };
    

    /**
     * parameters of the lti::correlation functor
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
      virtual ~parameters();

      /**
       * Returns the name of this type.
       */
      virtual const std::string& name() const;

      /**
       * copy the contents of other parameter object
       * @param other the parameters object to be copied
       * @return a reference to this object
       */
      parameters& copy(const parameters& other);

      /**
       * returns a pointer to a clone of this parameters
       */
      virtual parameters* clone() const;

      /**
       * Returns a pointer to a new instance of this parameters
       */
      virtual parameters* newInstance() const;

      /**
       * returns the kernel in use.  If it is not set yet, an
       * lti::invalidParameters exception will be thrown
       * @return a const reference to the filter kernel.
       */
      const container& getKernel() const;

      /**
       * sets the filter kernel to be used.
       * A copy of the given %parameter will be made!
       * @param aKernel the filter kernel to be used
       */
      void setKernel(const container& aKernel);

      /**
       * Returns the mask in use. If none is set yet, an
       * lti::invalidParametersException will be thrown.
       */
      const matrix<ubyte>& getMask() const;

      /**
       * Sets the mask to be used to select a region within the kernel.
       */
      void setMask(const matrix<ubyte>& aMask);

      /**
       * Determine whether the whole (rectangular) kernel should be convolved,
       * or only an arbitrarily shaped object within it, described by the mask.
       * The default value for this option is false, meaning the whole kernel
       * is convolved (just as one would expect).
       */
      void setUseMask(const bool b);

      /**
       * Returns the value set by setUseMask.
       */
      const bool& getUseMask() const;

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

      /**
       * Boundary Type
       *
       * Specify how to treat the boundary.
       */
      eBoundaryType boundaryType;

      /**
       * Type of correlation (see parameters::eMode)
       */
      eMode mode;

      /**
       * Convolve the whole kernel, or just the object defined by the mask.
       */
      bool useMask;

      /**
       * get kernel average
       */
      const double& getKernelAverage() const;

      /**
       * get kernel size
       */
      const int& getKernelSize() const;

    protected:
      /**
       * pointer to the filter kernel copy
       */
      container* kernel_;

      /**
       * The mask that defines the object depicted in the kernel
       */
      const matrix<ubyte>* mask_;

      /**
       * average of the kernel elements.
       * This pseudo-parameters is calculated by set kernel
       */
      double kernelAverage_;

      /**
       * the number of elements of the kernel is also needed
       */
      int kernelSize_;

    };

    /**
     * default constructor
     */
    correlation();

    /**
     * construct a correlation functor with a parameters set
     * which includes the given filter kernel.
     */
    correlation(const container& aKernel);

    /**
     * Construct with the given parameters
     */
    correlation(const parameters& param);

    /**
     * copy constructor
     * @param other the other functor to be copied
     */
    correlation(const correlation& other);

    /**
     * destructor
     */
    virtual ~correlation();

    /**
     * Returns the name of this type.
     */
    virtual const std::string& name() const;

    /**
     * operates on the given %parameter.
     * @param srcdest matrix<ubyte> with the source data.  The result
     *                will be left here too.
     * @return true if successful, false otherwise.
     */
    bool apply(matrix<ubyte>& srcdest) const;

    /**
     * operates on the given %parameter.
     * @param srcdest fmatrix with the source data.  The result
     *                will be left here too.
     * @return true if successful, false otherwise.
     */
    bool apply(fmatrix& srcdest) const;

    /**
     * operates on the given %parameter.
     * @param srcdest dmatrix with the source data.  The result
     *                will be left here too.
     * @return true if successful, false otherwise.
     */
    bool apply(dmatrix& srcdest) const;


    /**
     * operates on the given %parameter.
     * @param srcdest vector<ubyte> with the source data.
     *                The result will be left here too.
     * @return true if successful, false otherwise.
     */
    bool apply(vector<ubyte>& srcdest) const;

    /**
     * operates on the given %parameter.
     * @param srcdest fvector with the source data.
     *                The result will be left here too.
     * @return true if successful, false otherwise.
     */
    bool apply(fvector& srcdest) const;

    /**
     * operates on the given %parameter.
     * @param srcdest dvector with the source data.
     *                The result will be left here too.
     * @return true if successful, false otherwise.
     */
    bool apply(dvector& srcdest) const;


    /**
     * operates on a copy of the given parameters.
     * @param src matrix<ubyte> with the source data.
     * @param dest matrix<ubyte> where the result will be left.
     * @return true if successful, false otherwise.
     */
    bool apply(const matrix<ubyte>& src,matrix<ubyte>& dest) const;

    /**
     * operates on a copy of the given parameters.
     * @param src channel with the source data.
     * @param dest channel where the result will be left.
     * @return true if successful, false otherwise.
     */
    bool apply(const fmatrix& src,fmatrix& dest) const;

    /**
     * operates on a copy of the given parameters.
     * @param src dmatrix with the source data.
     * @param dest dmatrix where the result will be left.
     * @return true if successful, false otherwise.
     */
    bool apply(const dmatrix& src,dmatrix& dest) const;

    /**
     * operates on a copy of the given parameters.
     * @param src vector<ubyte> with the source data.
     * @param dest vector<ubyte> where the result will be left.
     * @return true if successful, false otherwise.
     */
    bool apply(const vector<ubyte>& src,
               vector<ubyte>& dest) const;

    /**
     * operates on a copy of the given parameters.
     * @param src fvector with the source data.
     * @param dest fvector where the result will be left.
     * @return true if successful, false otherwise.
     */
    bool apply(const fvector& src,
               fvector& dest) const;

    /**
     * operates on a copy of the given parameters.
     * @param src dvector with the source data.
     * @param dest dvector where the result will be left.
     * @return true if successful, false otherwise.
     */
    bool apply(const dvector& src,
               dvector& dest) const;

    /**
     * copy data of "other" functor.
     */
    correlation& copy(const correlation& other);

    /**
     * returns a pointer to a clone of the functor.
     */
    virtual correlation* clone() const;

    /**
     * returns a pointer to a clone of the functor.
     */
    virtual correlation* newInstance() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * shortcut to set the filter kernel in the functor parameters.
     * The other parameters remain unchanged.
     */
    void setKernel(const container& aKernel);

  private:
    /**
     * This is the accumulator class needed by the convolution helper to
     * act as a linear convolution operator for gray valued images.
     *
     * The type T is the type of the elements of the object to be filtered
     * The (optional) type U is the type of the accumulator variable for
     * the filter.
     */
    template<class T,class U=T>
    class classic {
    public:
      /**
       * Default constructor
       */
      classic();

      /**
       * Accumulate the values of filter and src
       * (srcL and srcR for symetric or asymetric kernel)
       */
      inline void accumulate(const T& filter,const T& src);
      inline void accumulateASym(const T& filter,const T& srcL,const T& srcR);
      inline void accumulateSym(const T& filter,const T& srcL,const T& srcR);

      /**
       * Accumulate the values of T(0) and src
       */
      inline void accumulateZero(const T& src);

      /**
       * Get the state of the accumulator
       */
      inline T getResult() const;

      /**
       * Reset the state of the accumulator
       */
      inline void reset();

      /**
       * set norm
       */
      inline void setNorm(const T& norm);

    protected:
      /**
       * the accumulated value
       */
      U state_;

      /**
       * norm
       */
      T norm_;
    };


    /**
     * This is the accumulator class needed by the correlation helper to
     * act as a correlation coefficient operator for gray valued images.
     *
     * This implements the Eq. (9.3-8) on page 584 on Gonzalez & Woods,
     * "Digital Image Processing"
     *
     * The type T is the type of the elements of the object to be filtered
     * The (optional) type U is the type of the accumulator variable for
     * the filter.
     */
    template<class T,class U=T>
    class coefficient {
    public:
      /**
       * Default constructor
       */
      coefficient();
      
      /**
       * Accumulate the values of filter and src
       * (srcL and srcR for symetric or asymetric kernel)
       */
      inline void accumulate(const T& filter,const T& src);
      inline void accumulateASym(const T& filter,const T& srcL,const T& srcR);
      inline void accumulateSym(const T& filter,const T& srcL,const T& srcR);
      
      /**
       * Accumulate the values of T(0) and src
       */
      inline void accumulateZero(const T& src);
      
      /**
       * Get the state of the accumulator
       */
      inline T getResult() const;
      
      /**
       * Reset the state of the accumulator
       */
      inline void reset();
      
      /**
       * set norm
       */
      inline void setNorm(const T& norm);
      
      /**
       * set averages of data and kernel
       */
      void setData(const T& kernelAverage,const T& kernelSize);
      
    protected:
      
      /**
       * the accumulated w-E(w)
       */
      U sumW_;
      
      /**
       * the accumulated (w-E(w))(w-E(w))
       */
      U sumWW_;
      
      /**
       * the accumulated f*(w-E(w))
       */
      U sumFW_;
      
      /**
       * the accumulated f*f
       */
      U sumFF_;

      /**
       * the accumulated f
       */
      U sumF_;

      /**
       * norm
       */
      T norm_;

      /**
       * Average of the data
       */
      T kernSize_;

      /**
       * Average of the kernel
       */
      T kernAvg_;
    };

    /**
     * This is the accumulator class needed by the convolution helper to
     * calculate the matching criterium C1 described in Sonka et.al.,pp.191
     *
     * The type T is the type of the elements of the object to be filtered
     * The (optional) type U is the type of the accumulator variable for
     * the filter.
     */
    template<class T,class U=T>
    class criterium1 {
    public:
      /**
       * Default constructor
       */
      criterium1();

      /**
       * Accumulate the values of filter and src
       * (srcL and srcR for symetric or asymetric kernel)
       */
      inline void accumulate(const T& filter,const T& src);
      inline void accumulateASym(const T& filter,const T& srcL,const T& srcR);
      inline void accumulateSym(const T& filter,const T& srcL,const T& srcR);

      /**
       * Accumulate the values of T(0) and src
       */
      inline void accumulateZero(const T& src);

      /**
       * Get the state of the accumulator
       */
      inline T getResult() const;

      /**
       * Reset the state of the accumulator
       */
      inline void reset();

      /**
       * set norm
       */
      inline void setNorm(const T& norm);

    protected:
      /**
       * the accumulated value
       */
      U state_;

      /**
       * norm
       */
      T norm_;
    };

    /**
     * This is the accumulator class needed by the convolution helper to
     * calculate the matching criterium C2 described in Sonka et.al.,pp.191
     *
     * The type T is the type of the elements of the object to be filtered
     * The (optional) type U is the type of the accumulator variable for
     * the filter.
     */
    template<class T,class U=T>
    class criterium2 : public criterium1<T,U> {
    public:
      /**
       * Accumulate the values of filter and src
       * (srcL and srcR for symetric or asymetric kernel)
       */
      inline void accumulate(const T& filter,const T& src);
      inline void accumulateASym(const T& filter,const T& srcL,const T& srcR);
      inline void accumulateSym(const T& filter,const T& srcL,const T& srcR);

      /**
       * Accumulate the values of T(0) and src
       */
      inline void accumulateZero(const T& src);
    };

    /**
     * This is the accumulator class needed by the convolution helper to
     * calculate the matching criterium C3 described in Sonka et.al.,pp.191
     *
     * The type T is the type of the elements of the object to be filtered
     * The (optional) type U is the type of the accumulator variable for
     * the filter.
     */
    template<class T,class U=T>
    class criterium3 : public criterium1<T,U> {
    public:
      /**
       * Accumulate the values of filter and src
       * (srcL and srcR for symetric or asymetric kernel)
       */
      inline void accumulate(const T& filter,const T& src);
      inline void accumulateASym(const T& filter,const T& srcL,const T& srcR);
      inline void accumulateSym(const T& filter,const T& srcL,const T& srcR);

      /**
       * Accumulate the values of T(0) and src
       */
      inline void accumulateZero(const T& src);
    };

  };


  /**
   * Read the correlation mode
   *
   * @ingroup gStorable
   */
  bool read(ioHandler& handler,correlation::eMode& data);

  /**
   * Write the correlation mode
   *
   * @ingroup gStorable
   */
  bool write(ioHandler& handler,
             const correlation::eMode& data);


}
#include "ltiCorrelation_template.h"

#endif
