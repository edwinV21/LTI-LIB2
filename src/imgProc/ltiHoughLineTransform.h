/*
 * Copyright (C) 2008
 * Pablo Alvarado
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

/** 
 * \file   ltiHoughLineTransform.h
 *         Contains the class lti::houghLineTransform, that computes the
 *         line detecting Hough transformation in an efficient way
 * 
 * \author Henning Luepschen
 * \author Pablo Alvarado
 * \date   18.12.2008
 *
 * revisions ..: $Id: ltiHoughLineTransform.h,v 1.5 2012-07-25 23:33:30 alvarado Exp $
 */

#ifndef _LTI_HOUGH_LINE_TRANSFORM_H_
#define _LTI_HOUGH_LINE_TRANSFORM_H_

#include "ltiLine.h"
#include "ltiGenericVector.h"

#include "ltiChannel8.h"
#include "ltiChannel.h"
#include "ltiImage.h"
#include "ltiChannel32.h"
#include "ltiFunctor.h"

namespace lti {

  /**
   * Class houghLineTransform
   *
   * This functor is used to make a fast Hough Line Transform
   * and creates a channel32 transformation accumulator.
   *
   * The houghLineTransform::parameters specify what part of
   * a given image/channel is transformed and which point values
   * are considered.
   *
   * To accelerate the computations, an orientation map containing the angles
   * perpendicular to the edges is used.  This can be generated using
   * for example lti::gradientFunctor or lti::cannyEdges.
   *
   * Two additional methods help in the use of the Hough space.  The first
   * one detects the local maxima ( getHoughSpacePoints() ), and the second
   * one finds two points that define the line segment found in the image
   * ( getSegment() ).
   *
   * The Hough space contains as expected two parameters: radius and angle.
   * They are measured from the middle point of the analysed region.  The angle
   * is always between 0 and 180 degrees.  The radius can be negative and its
   * absolute value will be between 0 and the image diagonal divided by two.
   *
   * Example:
   * \code
   *
   * #include "ltiHoughLineTransform.h"  // the Hough transform
   * #include "ltiCannyEdges.h"          // the edge detector
   * #include "ltiIOImage.h"             // to read png, jpg and bmp images
   * #include "ltiLine.h"
   * #include "ltiDraw.h"                // drawing tool
   * #include "ltiViewer2D.h"            // visualization tool
   *
   * // ...
   *
   * lti::image inputImg;
   * lti::channel8 edges;
   * lti::channel angles; // orientation map
   * lti::channel32 transformAccu;
   *  
   * // load an image and extract the intensity channel from it
   * lti::ioImage imgLoader;
   * imgLoader.load("../img/hough1.png",inputImg); // load an image
   *
   * // we need the edges
   * cannyEdges canny;
   * canny.apply(inputImg,edges,angles);
   *
   * // create default HLT (Hough Line Transform) using default parameters
   * lti::houghLineTransform HLT;
   * 
   * // The Hough Transform
   * 
   * HLT.apply(edges,angles,transformAccu);
   *
   * // Display the Hough space
   * viewer2D viewHS("Hough Space");
   * viewHS.show(transformAccu);
   *  
   * // Now get the local maxima of the transformAccu
   * lti::vector< fpoint > maxpos;
   * HLT.getHoughSpacePoints(transformAccu,maxpos);
   * 
   * // And draw the found lines in the original image
   * lti::draw<rgbaPixel> drawer;
   * drawer.use(inputImg);
   * drawer.setColor(lti::White);
   * iline seg;
   * for (int k=0;k<maxpos.size();++k) {
   *   // the x and y coordinates represent radius and angle, respectively
   *   HLT.getSegment(inputImg.size(),maxpos.at(k).x,maxpos.at(k).y,seg);
   *   drawer.line(seg);
   * }
   * 
   * // at last, show the resulting lines
   * viewer2D view("Detected lines");
   * view.show(inputImg);
   *
   * viewer2D::interaction action;
   * view.waitKeyPressed(action); // wait for a key being pressed
   * \endcode
   *
   * The previous example shows how to use the accumulator space (or Hough
   * space) directly, so that you can manipulate it and each detected line
   * segment as you wish, however, the usual use just requires the list of 
   * line segments, as the following example shows:
   * 
   * \code
   * #include "ltiLine.h"
   * #include "ltiHoughLineTransform.h"  // the Hough transform
   * #include "ltiCannyEdges.h"          // the edge detector
   * #include "ltiIOImage.h"             // to read png, jpg and bmp images
   * #include "ltiDraw.h"                // drawing tool
   * #include "ltiViewer2D.h"            // visualization tool
   *
   * // ...
   *
   * lti::image inputImg;
   * lti::channel8 edges;
   * lti::channel angles; // orientation map
   * lti::genericVector<iline> segments; // line segments
   *  
   * // load an image and extract the intensity channel from it
   * lti::ioImage imgLoader;
   * imgLoader.load("../img/hough1.png",inputImg); // load an image
   *
   * // we need the edges
   * cannyEdges canny;
   * canny.apply(inputImg,edges,angles);
   *
   * // create default HLT (Hough Line Transform) using default parameters
   * lti::houghLineTransform HLT;
   * 
   * // The Hough Transform
   * HLT.apply(edges,angles,segments);
   *
   * // Draw the found lines in the original image
   * lti::draw<rgbaPixel> drawer;
   * drawer.use(inputImg);
   * drawer.setColor(lti::Green);
   *
   * for (int k=0;k<segments.size();++k) {
   *   drawer.line(segments.at(k));
   * }
   * 
   * // at last, show the resulting lines
   * viewer2D view("Detected lines");
   * view.show(inputImg);
   *
   * viewer2D::interaction action;
   * view.waitKeyPressed(action); // wait for a key being pressed
   * \endcode
   *
   * @ingroup gShape
   */
  class houghLineTransform : public functor {
  public:
    /**
     * Accumulation modes
     */
    enum eAccumulationMode {
      Classic,  /**< A value of 1 will be accumulated in the Hough space
                 *   each time an edge is found with a value greater than
                 *   parameters::baseValue
                 */
      Gradient  /**< The value in the given edges image will be accumulated
                 *   allowing to weight the strength of an edge.  If the
                 *   edges image is a channel, their values will be
                 *   multiplied by 255 before accumulating.
                 */
    };
    

    /**
     * The parameters for the class houghLineTransform
     */
    class parameters : public functor::parameters {
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
       * Part of the input image/channel that will be considered for
       * transformation.
       *
       * This parameter will be overriden by the argument given in the
       * the apply methods that allow to provide the area directly.
       *
       * If this area is bigger than the image/channel size given in the
       * apply() method, the size of the channel will be used.
       *
       * The default values are (0,0,
       *                         std::numeric_limits<int>::max(),
       *                         std::numeric_limits<int>::max())
       * (i.e. use the whole image)
       */
      irectangle transformationArea;

      /**
       * All points of the input image/channel whose value is greater
       * than the base value are used for transformation.
       * (usually the base value will be 0 (or a little higher) which
       * results in all coloured lines being considered on a black background)
       *
       * For channels (with float values), the comparison value will be
       * baseValue/255.0.
       *
       * Default Value: 0
       */
      int baseValue;

      /**
       * Accuracy of Hough Line Transformation. Determines what kind of
       * line angles can be detected. The following values are
       * allowed:
       * - 45  (4 degrees steps)  :   very inaccurate, but very fast
       * - 90  (2 degrees steps)  :   inaccurate, but fast
       * - 180 (1 degree steps)   :   quite accurate
       * - 360 (1/2 degree steps) :   accurate, but slower
       * - 540 (1/3 degree steps) :   very accurate, but very slow
       * - 720 (1/4 degree steps) :   even more accurate, but even slower...
       * - ...
       *
       * Higher values that are divisible by 180 are also allowed, but
       * do make the transformation _really_ slow.
       *
       * Default value: 180
       */
      int accuracy;

      /**
       * Angle range. 
       * 
       * The sinusoids in the Hough space are only drawn from
       * (gradientAngle-range) to (gradientAngle+range). Values lower
       * than 5 are not recommended (units in degrees).
       *
       * Default value is 10.
       */
      int range;

      /**
       * Mode used for the accumulation
       *
       * Default: Classic
       */
      eAccumulationMode accumulationMode;

      /**
       * @name Parameters for apply methods that provide the line segments
       *
       * For those apply methods that directly provide the line segments some
       * additional parameters are required to configure the search of the
       * local maxima.
       */
      //@{
      /**
       * Factor to multiply the standard deviation of local maxima values.
       *
       * The average and std. deviation values between all detected local
       * maxima will be computed.  Only those values greater than the average
       * plus this parameter multiplied by the std. deviation will be
       * considered.  The default value 1.0 should be ok.
       *
       * Default value: 1.0f
       */
      float stdDevFactor;

      /**
       * Hystheresis threshold value.
       *
       * To detect the local maxima a region search starting from preliminary
       * maxima is done. 
       *
       * This value multiplied by the preliminary maxima determine the
       * threshold value that has to be reached in order to continue the
       * search for 'better' maxima.
       *
       * This value must be between 0 and 1.
       *
       * If it is chosen too low, only the biggest maxima will be found, as the
       * region search will easily grow.
       *
       * If it is too high, then too many maxima will be found.
       *
       * The default value of 0.5 should work well for most of cases.
       *
       * Default value: 0.5
       */
      float hystheresis;

      /**
       * Window to search for local maxima
       * 
       * This is the radius of a window used to detect the local maxima,
       * i.e. only values that are maxima on a window of the given radius will
       * be considered as candidates of points of interest in the Hough
       * accumulation space.
       *
       * This value must be greater than zero.
       *
       * Default value: 1
       */
      int localMaximaRadius;

      /**
       * Search radius
       *
       * In the search of interesting local maxima the region growing can go
       * out of control.  This value allows you to limit how far the region
       * growing algorithms is allowed to go from the initial local maximum
       * used as seed in the Hough space.
       *
       * If you give a negative value, no search limit will be applied.
       *
       * A value between 0 and 1 will be considered as a fraction of the
       * maximal radius found in the accumulation Hough space.
       *
       * A value greater than one will be interpreted as absolute distance in
       * pixels.
       *
       * Default value: 0.25
       */
      float searchRadius;
      //@}
    };

    /**
     * Default constructor
     */
    houghLineTransform();

    /**
     * Construct a functor using the given parameters
     */
    houghLineTransform(const parameters& par);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    houghLineTransform(const houghLineTransform& other);

    /**
     * Destructor
     */
    virtual ~houghLineTransform();

    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    houghLineTransform& copy(const houghLineTransform& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    houghLineTransform& operator=(const houghLineTransform& other);

    /**
     * Returns the complete name of the functor class
     */
    virtual const std::string& name() const;

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual houghLineTransform* clone() const;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual houghLineTransform* newInstance() const;

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * Update the parameters
     */
    virtual bool updateParameters();

    /**
     * @name Apply methods to compute the Hough accumulation space
     */
    //@{

    /**
     * Compute the Hough accumulation space of radia and angles
     *
     * @param src channel with the source data (gradient).
     * @param angles channel with gradient angles.
     * @param dest channel32 where the result will be left.
     *
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel& src,
               const channel& angles,
               channel32& dest) const;

    /**
     * Compute the Hough accumulation space of radia and angles
     *
     * @param src channel8 with the source data.
     * @param angles channel with gradient angles.
     * @param dest channel32 where the result will be left.
     *
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel8& src,
               const channel& angles,
               channel32& dest) const;

    /**
     * Compute the Hough accumulation space of radia and angles
     *
     * @param src image with the source data.
     * @param angle_src channel with gradient angles.
     * @param dest channel32 where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const image& src,
               const channel& angle_src,
               channel32& dest) const;

    /**
     * Compute the Hough accumulation space of radia and angles
     *
     * @param src channel32 with the source data.
     * @param angles channel with gradient angles.
     * @param dest channel32 where the result will be left.
     *
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel32& src,
               const channel& angles,
               channel32& dest) const;
    //@}

    /**
     * @name Apply methods to compute the line segments present in channels.
     *
     * Note that these methods compute the line segments for the whole
     * channels, and do not seach if the is just a small segment within the
     * detected line.
     */
    //@{

    /**
     * Compute the line segments present in the given channel.
     *
     * @param src channel with the source data (gradient).
     * @param angles channel with gradient angles.
     * @param segments line segments detected in the \a src channel.
     *
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel& src,
               const channel& angles,
               genericVector<iline>& segments) const;

    /**
     * Compute the line segments present in the given channel.
     *
     * @param src channel8 with the source data.
     * @param angles channel with gradient angles.
     * @param segments line segments detected in the \a src channel.
     *
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel8& src,
               const channel& angles,
               genericVector<iline>& segments) const;

    /**
     * Compute the line segments present in the given channel.
     *
     * @param src image with the source data.
     * @param angle_src channel with gradient angles.
     * @param segments line segments detected in the \a src channel.
     *
     * @return true if apply successful or false otherwise.
     */
    bool apply(const image& src,
               const channel& angle_src,
               genericVector<iline>& segments) const;

    /**
     * Compute the line segments present in the given channel.
     *
     * @param src channel32 with the source data.
     * @param angles channel with gradient angles.
     * @param segments line segments detected in the \a src channel.
     *
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel32& src,
               const channel& angles,
               genericVector<iline>& segments) const;
    //@}

    /**
     * @name Apply methods to compute the accumultation space of an image
     *       section.
     */
    //@{

    /**
     * Compute the Hough accumulation space of radia and angles
     *
     * @param area area of interest in the channels to be analyzed.  This
     *        overrides the parameters::transformationArea.
     * @param src channel with the source data (gradient).
     * @param angles channel with gradient angles.
     * @param dest channel32 where the result will be left.
     *
     * @return true if apply successful or false otherwise.
     */
    bool apply(const irectangle& area,
               const channel& src,
               const channel& angles,
               channel32& dest) const;

    /**
     * Compute the Hough accumulation space of radia and angles
     *
     * @param area area of interest in the channels to be analyzed.  This
     *        overrides the parameters::transformationArea.
     * @param src channel8 with the source data.
     * @param angles channel with gradient angles.
     * @param dest channel32 where the result will be left.
     *
     * @return true if apply successful or false otherwise.
     */
    bool apply(const irectangle& area,
               const channel8& src,
               const channel& angles,
               channel32& dest) const;

    /**
     * Compute the Hough accumulation space of radia and angles
     *
     * @param area area of interest in the channels to be analyzed.  This
     *        overrides the parameters::transformationArea.
     * @param src image with the source data.
     * @param angle_src channel with gradient angles.
     * @param dest channel32 where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const irectangle& area,
               const image& src,
               const channel& angle_src,
               channel32& dest) const;

    /**
     * Compute the Hough accumulation space of radia and angles
     *
     * @param area area of interest in the channels to be analyzed.  This
     *        overrides the parameters::transformationArea.
     * @param src channel32 with the source data.
     * @param angles channel with gradient angles.
     * @param dest channel32 where the result will be left.
     *
     * @return true if apply successful or false otherwise.
     */
    bool apply(const irectangle& area,
               const channel32& src,
               const channel& angles,
               channel32& dest) const;
    //@}


    /**
     * @name Help methods
     */
    //@{

    /**
     * This method is used to get the list of points in the parameter
     * space, sorted by their value in the Hough accumulation space , with the
     * maxima at the beginning.
     *
     * If you have computed the accumulation space with a wide angular range,
     * then the noisy data obtained can cause that the search for maxima in
     * that space get lost and you obtain nonsense data.  Try reducing
     * the range parameters or restricting the search ratio.
     *
     * @param hough a Hough transformation image, as produced by the apply
     *              methods
     * @param positions the positions of the points found in the Hough image,
     *                  sorted by the Hough value.  The x value will be the
     *                  radius (in pixels) and the y value the angle
     *                  (in radians).
     * @param stdDevFactor the average and std. deviation values
     *                     between all detected maxima will be computed.
     *                     Only those values greater than the average plus this
     *                     parameter multiplied by the std. deviation will be
     *                     considered.  The default value 1.0 should be ok.
     * @param hystheresis  to detect the local maxima a region search starting
     *                     from preliminary maxima will be done.  This value
     *                     multiplied by the preliminary maxima determine the
     *                     threshold that must be reached in order to continue
     *                     the search.  The value must be between 0 and 1.  If
     *                     it is chosen too low, only the biggest maxima will
     *                     be found.  If it is too high, then too many maxima
     *                     will be found. 
     *                     The default value of 0.5 should work well for most
     *                     of cases.
     * @param wndRadius    radius of a window used to detect the local maxima,
     *                     i.e. only values that are maxima on a window of the
     *                     given radius will be considered as candidates.
     *                     This value must be greater than zero and less than
     *                     the smallest dimension of the \a hough channel.
     *                     Default value is 1
     * @param searchRadius from a local maximum used as seed, reach as far as
     *                     this value for better maxima, but no more.
     *                     If you give a negative value, no search limit will
     *                     be applied.
     *                     A value between 0 and 1 will be considered as a
     *                     fraction of the maximal ratio found in the acc space
     *                     A value greater than one will be interpreted as
     *                     absolute distance in pixels.
     *
     * @return \c true if successful, \c false otherwise (like argument values
     *         out of range.)
     */
    bool getHoughSpacePoints(const channel32& hough,
                             vector<fpoint>& positions,
                             const float stdDevFactor,
                             const float hystheresis,
                             const int wndRadius,
                             const float searchRadius) const;


    /**
     * This method is used to get the list of points in the parameter
     * space, sorted by their value in the Hough accumulation space, with the
     * maxima at the beginning.
     *
     * If you have computed the accumulation space with a wide angular range,
     * then the noisy data obtained can cause that the search for maxima in
     * that space get lost and you obtain nonsense data.  Try reducing
     * the range parameters or restricting the search ratio.
     *
     * This method calls getHoughSpacePoints with the configuration given
     * in the parameters
     *
     * @param hough a Hough transformation image, as produced by the apply
     *              methods
     * @param positions the positions of the points found in the Hough image,
     *                  sorted by the Hough value.  The x value will be the
     *                  radius (in pixels) and the y value the angle
     *                  (in radians).
     *
     * @return \c true if successful, \c false otherwise (like argument values
     *         out of range.)
     */
    bool getHoughSpacePoints(const channel32& hough,
                             vector<fpoint>& positions) const;


    /**
     * Given the coordinates in the Hough accumulation space (provided by some
     * of the apply methods) compute the corresponding segment
     *
     * @param accSize size of the accumulation space
     * @param pos   position of the point in the accumulation space
     * @param imageSize size of the image for which the points a and b
     *                  of the segment need to be computed
     * @param segment line segment corresponding to the radius and angle
     * @param origin optional parameter that specifies the origin of
     *               the coordinates system in the image.  If not given
     *               or if it contains negative values, the middle of the
     *               image will be used as origin.
     * @return \c true if successful, \c false otherwise (like argument values
     *         out of range.)
     */
    bool accToSegment(const ipoint& accSize,
                      const ipoint& pos,
                      const ipoint& imageSize,
                      iline& segment,
                      const ipoint& origin = ipoint(-1,-1)) const;

    /**
     * Given a radius and an angle, compute the corresponding two points at the
     * border of the image.
     *
     * @param imageSize size of the image for which the points a and b
     *                  need to be computed
     * @param radius radius parameter of the Hough space
     * @param angle angle parameter of the Hough space
     * @param segment line segment corresponding to the radius and angle
     * @param origin optional parameter that specifies the origin of
     *               the coordinates system in the image.  If not given
     *               or if it contains negative values, the middle of the
     *               image will be used as origin.
     * @return \c true if successful, \c false otherwise (like argument values
     *         out of range.)
     */
    bool getSegment(const ipoint& imageSize,
                    const float radius,
                    const float angle,
                    iline& segment,
                    const ipoint& origin = ipoint(-1,-1)) const;

    /**
     * This help method transforms the first point which is given as radius and
     * angle (as provided in the output vector of the method
     * getHoughSpacePoints() ) to a (x,y) point in the accumulator channel
     *
     * @param accSize size of the accumulation image
     * @param houghSpace encoded point with radius (x) and angle (y)
     * @param accImage equivalent point with x,y in the accumulator image.
     *
     * @return \c true if successful, \c false otherwise.
     */
    void houghSpaceToAccImage(const ipoint& accSize,
                              const ipoint& houghSpace,
                              ipoint& accImage) const;

    //@}

  protected:
    /**
     * Returns used parameters
     */
    parameters& getRWParameters();

    /**
     * Returns accuracy of Hough Line Transform
     * @see accuracy_
     *
     * @return value of attribute accuracy_
     */
    int getAccuracy() const;

    /**
     * Sets accuracy of Hough Line Transform and creates the
     * appropriate sinus and cosinus integer tables
     *
     * @see accuracy_
     * @return true, if successful
     */
    bool setAccuracy( int accuracy );

  private:

    /**
     * Create sinus/cosinus integer tables of given size.
     */
    void createSinCosTable(const int size);

    /**
     * Cosinus integer table. Used to speed up the transformation.
     */
    ivector cosinus_;

    /**
     * Sinus integer table. Used to speed up the transformation.
     */
    ivector sinus_;

    /**
     * Accuracy of Hough Line Transform. Determines what kind of
     * line angles can be detected. The following values are
     * allowed:
     *
     * 45  (4 degrees steps)  :   very inaccurate, but very fast
     * 90  (2 degrees steps)  :   inaccurate, but fast
     * 180 (1 degree steps)   :   quite accurate
     * 360 (1/2 degree steps) :   accurate, but slower
     * 540 (1/3 degree steps) :   very accurate, but very slow
     * 720 (1/4 degree steps) :   even more accurate, but even slower...
     * ...
     *
     * Higher values that are divisible by 180 are also allowed, but
     * do make the transformation _really_ slow. 180 is the DEFAULT value.
     *
     * -------------------------------------------------------------------
     *  The private copy of the parameters.accuracy is only kept in order
     *  to check if it is necessary to create _new_ sinus/cosinus tables
     * -------------------------------------------------------------------
     */
    int accuracy_;

    /**
     * Helper method used to perform the real Hough transform
     */
    template<typename T>
    bool hough(const matrix<T>& src,
               const channel& angleSrc,
               channel32& dest) const;

    /**
     * Helper method used to perform the real Hough transform
     */
    template<typename T>
    bool hough(const irectangle& transformationArea,
               const matrix<T>& src,
               const channel& angleSrc,
               channel32& dest) const;


    /**
     * Classic: Accumulate 1 if src.at(y,x) > baseValue
     */
    template<typename T>
    bool houghClassic(const irectangle& transformationArea,
                      const matrix<T>& src,
                      const channel& angleSrc,
                      channel32& dest) const;

    /**
     * Gradient: Accumulate src.at(y,x) if it is greater than baseValue
     */
    template<typename T>
    bool houghGradient(const irectangle& transformationArea,
                       const matrix<T>& src,
                       const channel& angleSrc,
                       channel32& dest) const;

    /**
     * Get the segments in an accumulation channel
     */
    bool getSegments(const channel32& accSpace,
                     const ipoint& chnlSize,
                     genericVector<iline>& segments) const;

    /**
     * Simple method that returns 1 for all types except for floating point
     * ones.
     */
    template<typename T>
    inline T norm(const int val) const;

    /**
     * Cast method between given value in src and the value_type of the
     * accumulator.
     */
    template<typename T>
    inline channel32::value_type getAccVal(const T val) const;
  };

  /**
   * Read a houghLineTransform::eAccumulationMode
   *
   * @ingroup gStorable
   */
  bool read(ioHandler& handler,
            houghLineTransform::eAccumulationMode& data);

  /**
   * Write a houghLineTransform::eAccumulationMode
   *
   * @ingroup gStorable
   */
  bool write(ioHandler& handler,
             const houghLineTransform::eAccumulationMode& data);

}

#endif

