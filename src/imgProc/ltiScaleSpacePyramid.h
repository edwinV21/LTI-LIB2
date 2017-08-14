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
 * \file   ltiScaleSpacePyramid.h
 *         This file contains an image pyramid to represent the scale space.
 * \author Pablo Alvarado
 * \author Frederik Lange
 * \date   17.03.2003
 *
 * $Id: ltiScaleSpacePyramid.h,v 1.6 2006-09-19 08:51:14 doerfler Exp $
 */

#ifndef _LTI_SCALESPACE_PYRAMID_H_
#define _LTI_SCALESPACE_PYRAMID_H_

#include "ltiObject.h"
#include "ltiFunctor.h"
#include "ltiPyramid.h"

#include "ltiGaussKernels.h"
#include "ltiConvolution.h"
#include "ltiBilinearInterpolation.h"
#include "ltiScaling.h"
#include "ltiParametersManager.h"
#include "ltiMath.h"

#include <string>

namespace lti {

  /**
   * Image pyramid to represent the scale space.
   *
   * This pyramid can be used for scale-dependent image access.  At
   * construction time the input channel is downsampled to several
   * scales as specified with the resize()-method.
   *
   * Each channel is downsampled by "factor"-parameter with Gaussian
   * convolution applied.  This factor is usually greater than 0.5 (the
   * usual one in image pyramids), but still must be less than 1.0.
   * This is done to archieve a much higher precision.
   *
   * As with other pyramids, the template type T represents the image type.
   * Usually you will want to use an scaleSpacePyramid<channel>.
   *
   * For this class the term "level" denotes one of the existent layers of
   * the pyramid.  The term "scale" denotes a real value.  The "levels"
   * have explicit scales, that can be obtaind with getLevelScale().
   *  
   */
  template<typename T, template<class> class Inter = bilinearInterpolation> 

  class scaleSpacePyramid : public pyramid<T>, 
                            public parametersManager {
  public:
    /**
     * Type of elements of the channels of type T
     */
    typedef typename T::value_type value_type;
      
    /**
     * Parameters for the generation of scaleSpace pyramids.
     *
     * Even if scaleSpacePyramid is by no means a functor, the
     * number of parameters required in its generation is large enough to 
     * justify the use of a parameters class.  It is just inherited from
     * functor::parameters to reuse the interface defined there.
     */
    class parameters : public parametersManager::parameters {
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
       * returns a new instance of the parameters
       */
      virtual parameters* newInstance() const;

      /**
       * Returns the name of this type.
       */
      virtual const std::string& name() const;
      
      /**
       * write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool write(ioHandler& handler,const bool complete=true) const;

      /**
       * read the parameters from the given ioHandler
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
       * Specify if each level in the pyramid should be smoothed with a
       * Gaussian kernel or not.
       *
       * Default: false
       */
      bool gaussian;
      
      /**
       * Kernel size used to smooth each pyramid level. 
       *
       * Used only if the automaticKernel mode is set to false and 
       * Gaussian smoothing is desired.
       *
       * Default is 5
       */
      int kernelSize;
      
      /**
       * Kernel variance
       *
       * Used only if the automaticKernel mode is set to false and  
       * Gaussian smoothing is desired.
       *
       * Default value: 1.6*1.6
       */
      double kernelVariance;

      /**
       * Automatic kernel computation.
       *
       * The size and the variance of the smoothing kernel are determined
       * depending on the factor for downsampling.
       *
       * Default value: true
       */
      bool automaticKernel;

      /**
       * Scale factor between adjacent pyramid layers. 
       *
       * This parameter determines the factor between the resolution
       * resolution(n) = resolution(0) * factor^n
       *
       * This value should be in the interval ]0.5,1.0[ in order for many
       * of the interpolation methods to work properly.
       *
       * Default: cubic root of 0.5 == 0.793700526
       */
      double factor;

      /**
       * Interpolation parameters.
       *
       * Default: default parameters of used Interpolation,
       * but with Constant boundaryType.
       */
      typename Inter<value_type>::parameters interpolationParameters;
    };

    /**
     * Default constructor
     */
    scaleSpacePyramid<T,Inter>();

    /**
     * Create a pyramid with the given number of levels and the given
     * parameters
     */
    scaleSpacePyramid<T,Inter>(const int levels,
                               const parameters& par = parameters());

    /**
     * Copy constructor
     *
     * @param other the object to be copied
     */
    scaleSpacePyramid<T,Inter>(const scaleSpacePyramid<T,Inter>& other);

    /**
     * Destructor
     */
    ~scaleSpacePyramid<T,Inter>();

    /**
     * Returns the name of this type.
     */
    virtual const std::string& name() const;


    /**
     * Returns a pointer to a clone of this pyramid.
     */
    virtual scaleSpacePyramid<T,Inter>* clone() const;

    /**
     * Returns a pointer to a new instance of this pyramid.
     */
    virtual scaleSpacePyramid<T,Inter>* newInstance() const;

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;
        
        
    /**
     * Copy data of "other" functor.
     *
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    scaleSpacePyramid<T,Inter>& copy(const scaleSpacePyramid<T,Inter>& other);

    /**
     * Alias for copy member
     *
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    scaleSpacePyramid<T,Inter>& 
    operator=(const scaleSpacePyramid<T,Inter>& other);

    /**
     * updateParameters
     */
    virtual bool updateParameters();

    /**
     * Change the number of resolutions of the pyramid
     *
     * @param levels the new number of levels or layers of the pyramid
     * @param copyData if true (default), the old data will be keeped.
     *                 If false, all data will be lost.
     */
    void resize(const int levels,
                const bool copyData=true);

    /**
     * Generate the pyramid of the given object.
     *
     * The pyramid will contain the number of levels specified in
     * the construction or in the resize() method. 
     *
     * The level "0" will correspond to the original channel or
     * image.  The level \e i+1 is always a parameters::factor
     * times smaller than the level \e i at each axis.
     */
    void generate(const T& src);
    
    /**
     * Generate the pyramid of the given object.
     *
     * The pyramid will contain the number of resolutions specified 
     * by \a theResolutions.
     *
     * The level "0" will correspond to the original channel or
     * image.  The resolution \e i+1 is always a parameters::factor
     * times smaller than the level \e i at each axis.
     */
    void generate(const T& src, const int numLevels);

    /**
     * @name Scale-space access operators
     *
     * The scale space can be accessed at any real x,y and scale values.
     * Interpolation is necessary to get a spatial subpixel or a value
     * between scales. 
     *
     * The spatial coordinates are always given with respect to the first
     * level (level 0), which mean they must be between (0,0) and 
     * (at(0).lastColumn(),at(0).lastRow()).
     */
    //@{
    
    /**
     * Get an "in level" interpolation. 
     *
     * The given level \a lev must be in the pyramid.
     */
    value_type at(const float y,const float x,const int lev) const;

    // make inherited at member functions available 
    using pyramid<T>::at;
     
    //@}

    /**
     * Get the scale corresponding for a circular area of the given
     * radius.  
     *
     * At the scale 0, the radius of a pixel is 0.5.
     */
    float getScaleForRadius(const float radius) const;

    /**
     * Get the radius of one "pixel" for the given level.
     *
     * At the level 0, the radius of a pixel is 0.5.
     */
    float getRadiusForScale(const float scale) const;

    /**
     * Return the scale for the levels of the pyramid
     */
    inline const double& getLevelScale(const int level) const;

    /**
     * Type of extrema detected at a given point
     */
    enum eExtremaType {
      NoExtremum=0, /**< No extremum point has been detected */
      Minimum,      /**< A (local) minimum has been detected */
      Maximum       /**< A (local) maximum has been detected */
    };

    /**
     * Searches for an extremum with sub-pixel accuracy in a 3x3 region
     * around the given access point.
     *
     * A 2D quadratic function will be used to compute the sub-pixel values.
     *
     * Note that it only makes sense to call this method if the
     * (col,row) point of the given level is already a extremum or a
     * minimum in the discrete coordinates system, i.e. if the point
     * is greater (smaller)  than its eight neighbors.  Otherwise this method
     * will just return "NoExtremum".
     *
     * @param row row in coordinates of the given level, i.e. this value must
     *            be between 0 and at(level).rows()
     * @param col column in coordinates of the given level, i.e. this value
     *            must be between 0 and at(level).columns()
     * @param level level at which the extremum must be looked for.
     * @param spRow in case a extremum is found in the 3x3 region, this is the
     *              row of the found extremum using the coordinates of the 
     *              corresponding level. 
     * @param spCol in case a extremum is found in the 3x3 region, this is the
     *              column of the found extremum using the coordinates of the 
     *              corresponding level.
     *
     * @return the type of extremum detected.  In case a
     *         maximum/minimum was found, the spRow and spCol will contain the
     *         position with sub-pixel precision of the found extremum.
     */
    eExtremaType interpolateExtremum(const int row, 
                                     const int col,
                                     const int level,
                                     float& spRow,
                                     float& spCol) const;


    /**
     * Seaches for an extremum with sub-pixel accuracy in a 3x3x3 region
     * around the given access point.
     *
     * A 3D quadratic function will be used to compute the sub-pixel values.
     *
     * This function will only work if the parameters::factor is in ]0.5,1.0[.
     *
     * Note that in makes only sense to call this method if the (col,row)
     * point of the given level is already a extremum in the discrete
     * coordinates system, i.e. if the point is greater than its 26
     * neighbors.  Otherwise this method will just return "NoExtremum".
     *
     * @param row row in coordinates of the given level, i.e. this value must
     *            be between 0 and at(level).rows()
     * @param col column in coordinates of the given level, i.e. this value
     *            must be between 0 and at(level).columns()
     * @param level level at which the extremum must be looked for.
     * @param spRow in case an extremum is found in the 3x3x3 region, this is 
     *              the row of the found extremum using the coordinates of 
     *              the corresponding level. 
     * @param spCol in case an extremum is found in the 3x3x3 region, this is
     *              the column of the found extremum using the coordinates of
     *              the corresponding level.
     * @param scale in case an extremum is found in the 3x3x3 region, this is
     *              the scale of the found extremum.
     *
     * @return the type of extremum detected.  In case a
     *         maximum/minimum was found, the spRow, spCol and scale  will 
     *         contain the position with sub-pixel precision of the found
     *         extremum.
     */
    eExtremaType interpolateExtremum(const int row, 
                                    const int col,
                                    const int level,
                                    float& spRow,
                                    float& spCol,
                                    float& scale) const;


    /**
     * Seaches the maximum with sub-pixel accuracy in a 3x3 region around the
     * access point.
     *
     * A 2D quadratic function will be used to compute the sub-pixel values.
     *
     * Note that in makes only sense to call this method if the (col,row)
     * point of the given level is already a maximum in the discrete
     * coordinates system, i.e. if the point is greater than its eight
     * neighbors.  Otherwise this method will just return false
     *
     * @param row row in coordinates of the given level, i.e. this value must
     *            be between 0 and at(level).rows()
     * @param col column in coordinates of the given level, i.e. this value
     *            must be between 0 and at(level).columns()
     * @param level level at which the maximum must be looked for.
     * @param spRow in case a maximum is found in the 3x3 region, this is the
     *              row of the found maximum using the coordinates of the 
     *              corresponding level. 
     * @param spCol in case a maximum is found in the 3x3 region, this is the
     *              column of the found maximum using the coordinates of the 
     *              corresponding level.
     *
     * @return true if a maximum was found within the 3x3 region.  In this case
     *         the spRow and spCol will contain the position with sub-pixel
     *         precision of the found maximum.  In case there is a maximum,
     *         but not in the 3x3 region, or only a minimum or saddle point
     *         is found, then false will be returned.
     */
    bool interpolateMaximum(const int row, 
                            const int col,
                            const int level,
                            float& spRow,
                            float& spCol) const;

    /**
     * Seaches the maximum with sub-pixel accuracy in a 3x3x3 region around the
     * access point.
     *
     * A 3D quadratic function will be used to compute the sub-pixel values.
     *
     * This function will only work if the parameters::factor is in ]0.5,1.0[.
     *
     * Note that in makes only sense to call this method if the (col,row)
     * point of the given level is already a maximum in the discrete
     * coordinates system, i.e. if the point is greater than its 26
     * neighbors.  Otherwise this method will just return false
     *
     * @param row row in coordinates of the given level, i.e. this value must
     *            be between 0 and at(level).rows()
     * @param col column in coordinates of the given level, i.e. this value
     *            must be between 0 and at(level).columns()
     * @param level level at which the maximum must be looked for.
     * @param spRow in case a maximum is found in the 3x3x3 region, this is 
     *              the row of the found extremum using the coordinates of 
     *              the corresponding level. 
     * @param spCol in case an maximum is found in the 3x3x3 region, this is
     *              the column of the found extremum using the coordinates of
     *              the corresponding level.
     * @param scale in case an maximum is found in the 3x3x3 region, this is
     *              the scale of the found extremum.
     *
     * @return true if a maximum was found within the 3x3 region.  In this case
     *         the spRow and spCol will contain the position with sub-pixel
     *         precision of the found maximum.  In case there is a maximum,
     *         but not in the 3x3 region, or only a minimum or saddle point
     *         is found, then false will be returned.
     */
    bool interpolateMaximum(const int row, 
                            const int col,
                            const int level,
                            float& spRow,
                            float& spCol,
                            float& scale) const;

    /**
     * Check if the given pixel at the given level (in the respective
     * coordinate system) is a maximum in the 3x3 neighborhood.
     *
     * @param row row of the point to be checked.  It must be between 0 and
     *            at(level).lastRow(), but for the borders false will be
     *            always returned, as a "constant boundary" will be always
     *            assumed.
     * @param col column of the point to be checked.  It must be between 0 and
     *            at(level).lastColumn(), but for the borders false will be
     *            always returned, as a "constant boundary" will be always
     *            assumed.
     * @param level the level where the maximum has to be checked.
     */ 
    bool checkMaximum(const int row,
                      const int col,
                      const int level) const;
    
    /**
     * Check if the given pixel at the given level (in the respective
     * coordinates) is a maximum in the 3x3x3 neighborhood
     *
     * @param row row of the point to be checked.  It must be between 0 and
     *            at(level).lastRow(), but for the borders false will be
     *            always returned, as a "constant boundary" will be always
     *            assumed.
     * @param col column of the point to be checked.  It must be between 0 and
     *            at(level).lastColumn(), but for the borders false will be
     *            always returned, as a "constant boundary" will be always
     *            assumed.
     * @param level the level where the maximum has to be checked.
     */ 
    bool checkInterlevelMaximum(const int row,
                                const int col,
                                const int level) const;

    /**
     * Map the coordinates of the given level into coordinates of level 0
     */
    inline void mapToLevel0(const int level,
                            const float row,
                            const float col,
                            float& row0,
                            float& col0) const;

    /**
     * Map the coordinates of level 0 into the coordinates of the given level
     */
    inline void mapToLevel(const int level,
                           const float& row0,
                           const float& col0,
                           float& row,
                           float& col) const;
    
    /**
     * Map the coordinates from level "from" to level "to"
     */
    inline void mapToLevel(const int fromLevel,
                           const int toLevel,
                           const float& rowFrom,
                           const float& colFrom,
                           float& rowTo,
                           float& colTo) const;

  protected:

    /**
     * Access with constant boundary
     */
    inline value_type cstAt(const T& img,const int y,const int x) const;

    /**
     * This "transparent" accumulator returns the first input value, while
     * it accumulates in the second variable
     */
    inline const value_type& tacc(const value_type& in,
                                  value_type& out) const;

  protected:
    /**
     * Vector containing the real scaling factors used for each level in
     * the pyramid.
     */
    dvector levelFactor_;

    /**
     * Interpolation functor.
     */
    Inter<value_type> interpol_;

    /**
     * Scaling functor.
     */
    scaling<value_type, Inter> scaler_;

        
    /**
     * Initialize the values for the levelFactor. 
     * It assumes that the vector has the proper size.
     */
    void initLevelFactor();

  };


}

#include "ltiScaleSpacePyramid_template.h"
#endif



