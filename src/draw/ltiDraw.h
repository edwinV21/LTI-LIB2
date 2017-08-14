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
 * \file   ltiDraw.h
 * \author Pablo Alvarado
 * \date   25.5.1998
 * 
 * $Id: ltiDraw.h,v 1.15 2009-06-08 01:02:48 alvarado Exp $
 */

#ifndef _LTI_DRAW_H_
#define _LTI_DRAW_H_


#include <vector>

#include "ltiTypes.h"
// #include "ltiHTypes.h"
#include "ltiGenericMatrix.h"
#include "ltiImage.h"
#include "ltiIoPoints.h"
#include "ltiBorderPoints.h"
#include "ltiAreaPoints.h"
#include "ltiPolygonPoints.h"
#include "ltiIoHandler.h"
#include "ltiPoint.h"
#include "ltiLine.h"
#include "ltiRectangle.h"
#include "ltiLocation.h"
#include "ltiAffineLocation.h"
#include "ltiKernel1D.h"
#include "ltiKernel2D.h"
#include "ltiSeparableKernel.h"
#include "ltiRound.h"

#include "ltiDrawBase.h"

namespace lti {

  /**
   * Object for drawing a number of geometric figures and lists
   * thereof, simple texts and a few other types with graphic
   * representation in a lti::matrix. There are also methods to draw
   * Matlab-style markers instead of single pixels for points (see
   * marker()).
   *
   * The type T of this template class corresponds to the type T of the matrix.
   *
   * Example:
   *
   * if you want to draw on a channel, you can create an instance of
   * the draw-object this way:
   *
   * \code
   * lti::draw<lti::channel::value_type> drawer;  // drawer for channels
   * lti::channel canvas(256,256,0.0f);   // a channel to draw on
   *
   * drawer.use(canvas);                  // draw on canvas
   * drawer.setColor(0.5f);               // use gray
   * drawer.line(0,0,255,255);            // draw a line from to upper-left
   *                                      // to the bottom-right corner.
   * \endcode
   *
   */
  template<class T>
  class draw : public drawBase<T> {
  public:

    /**
     * default constructor
     */
    draw();

    /**
     * copy constructor
     */
    draw(const draw<T>& other);

    /**
     * destructor
     */
    ~draw();

    /**
     * Gets the name of this object's class.
     */ 
    const std::string& name() const;

    /**
     * Creates a copy of this object
     */
    virtual draw<T>* clone() const;

    /**
     * Creates a new instance of the object's class
     */
    virtual draw<T>* newInstance() const;

    /**
     * Indicates in which image will be drawn
     */
    virtual void use(matrix<T>& img);

    /**
     * get a reference to the image currently being used.
     * \warning if you haven't set any image yet (see use()), an lti::exception
     *          will be thrown
     */
    virtual matrix<T>& getCanvas();

    /**
     * get a read-only reference to the image currently being used.
     * \warning if you haven't set any image yet (see use()), an lti::exception
     *          will be thrown
     */
    virtual const matrix<T>& getCanvas() const;

    /**
     * returns the size of the canvas
     */
    ipoint getCanvasSize();

    /**
     * Specifies color to be used. If the template type is rgbPixel
     * this means an actual color. For other template types, this sets
     * a gray-level.
     */
    void setColor(const T& px);

    /**
     * Specifies color to be used. If the template type is rgbPixel
     * this means an actual color. For other template types, this sets
     * a gray-level. Here, MATLAB style specifiers are used. See
     * setStyle(const char*) for details.
     */
    void setColor(const char* color);

    /**
     * Specifies gray-level to be used.
     */
    void setGray(const float k);

    /**
     * Get color being used
     */
    const T& getColor() const;

    /**
     * scale each pixel value with the given constant
     */
    void scale(const double factor);
    
    /**
     * fills the canvas with the actual color
     */
    void clear();

    /**
     * fill the canvas with the current color, starting at the given
     * seed until a color different to the original one at the seed is
     * found.  This is similar to the fill tools usually found in
     * paint programs.
     */
    void fill(const ipoint& seed);

    /**
     * fill the canvas with the current color, starting at the given
     * seed until a color different to the original one at the seed is
     * found.  This is similar to the fill tools usually found in
     * paint programs.
     */
    inline void fill(int x, int y);

    /**
     * fillUntil fills the canvas with the current color, 
     * starting at the given seed until the stop color is reached.
     */
    void fillUntil(const ipoint& seed, const T& stopColor);

    /**
     * fillUntil fill the canvas with the current color, 
     * starting at the given seed until the stop color is reached.
     */
    inline void fillUntil(int x, int y, const T& stopColor);

    /**
     * Set pixel at x, y in the color set by setColor().
     * @param x x-coordinate of the pixel to be set.
     * @param y y-coordinate of the pixel to be set.
     */
    void set(int x, int y);

    /**
     * Set pixel at p in the color set by setColor().
     * @param p coordinates of the pixel to be set
     */
    inline void set(const ipoint& p);

    /*
     * Set pixel at p in the color set by setColor().
     * @param p coordinates of the pixel to be set
     */
    /* TODO
    template<class U>
    inline void set(const hPoint2D<U>& p);
    */

    /**
     * Draw a line from the point (fx, fy) to point (tx, ty).
     * The "last point" will be defined with the last "set", "line" or
     * "lineTo" method.
     *
     * @param fx x coordinate of initial point.
     * @param fy y coordinate of initial point.
     * @param tx x coordinate of final point.
     * @param ty y coordinate of final point.
     */
    void line(int fx, int fy,
              int tx, int ty);

    /**
     * Draw a line from the point p to point p2.
     *
     * @param p1 initial point
     * @param p2 final point
     *
     * @see line(int, int, int, int)
     */
    inline void line(const ipoint& p1, const ipoint& p2);

    /**
     * Draw a line 
     * @see line(int, int, int, int)
     */
    inline void line(const lti::line<int>& l);

    /**
     * the homogeneus point p represents also a line, which equation is
     * given by p.x*x + p.y*y + p.h*1 = 0.  This line is drawn with this
     * member
     */
    /* TODO
    template <class U>
    inline void line(const hPoint2D<U>& p);
    */
    
    /**
     * Draw a line from the last point to (x, y).
     * The "last point" will be defined with the last "set", "point", "line" or
     * "lineTo" method.
     */
    void lineTo(int x, int y);

    /**
     * Draw a line from the last point to the point p.
     * The "last point" will be defined with the last "set", "point", "line" or
     * "lineTo" method.
     */
    inline void lineTo(const ipoint& p);

    /**
     * Doesn't draw anything, only sets the last position to the given
     * coordinates. This can be useful before using lineTo()
     */
    virtual void moveTo(int x, int y);

    /// make other base member functions lineTo available
    using drawBase<T>::lineTo;

    /**
     * Draws a vertical line from (x, y1) to (x, y2).
     */
    void verticalLine(int x, int y1, int y2);

    /**
     * Draws a vertical line from (p1.x, p1.y) to (p1.x, p2.y).
     */
    void verticalLine(const ipoint& p1, const ipoint& p2);

    /**
     * Draws a horizontal line from (x1, y) to (x2, y).
     */
    void horizontalLine(int x1, int x2, int y);

    /**
     * Draws a horizontal line from (p1.x, p1.y) to (p2.x, p1.y).
     */
    void horizontalLine(const ipoint& p1, const ipoint& p2);

    /**
     * draw an arrow. arrow tip will be at (tx, ty).
     * If size<1.0 then tipsize will be the relative portion of arrow length.
     * If size>1.0 then tipsize will be (int)size, independent of arrow length.
     */
    void arrow(int fx, int fy,
               int tx, int ty,
               const float& size=0.2f);


    /**
     * draw an arrow from point p1 to point p2
     */
    inline void arrow(const ipoint& p1, const ipoint& p2,
                      const float& size=0.2f);

    /**
     * draw a rectangle
     * @param r rectangle (with orientation 0)
     * @param filled true if the rectangle must be filled, false if only the
     *               border needs to be drawn.
     */
    inline void rectangle(const lti::rectangle<int>& r, bool filled = false);
    
    /**
     * draw a rotated rectangle
     * @param r rectangle (with orientation 0)
     * @param angle rotation angle at the middle point of the rectangle
     * @param filled true if the rectangle must be filled, false if only the
     *               border needs to be drawn.
     */
    inline void rectangle(const lti::rectangle<int>& r, const float angle,
                          bool filled);

    /**
     * draw a rectangle
     * @param upperLeft upper left corner of the rectangle with orientation 0
     * @param bottomRight bottom right corner of the rectangle with orientation
     *                    0
     * @param filled true if the rectangle must be filled, false if only the
     *               border needs to be drawn.
     * @see rectangle(int, int, int, int, bool)
     */
    inline void rectangle(const ipoint& upperLeft, const ipoint& bottomRight,
                          bool filled = false);

    /**
     * draw a rectangle
     * @param upperLeft upper left corner of the rectangle with orientation 0
     * @param bottomRight bottom right corner of the rectangle with orientation
     *                    0
     * @param angle rotation angle at the middle point of the rectangle
     * @param filled true if the rectangle must be filled, false if only the
     *               border needs to be drawn.
     * @see rectangle(int, int, int, int, bool)
     */
    inline void rectangle(const ipoint& upperLeft, const ipoint& bottomRight,
                          const float angle,
                          bool filled = false);

    /**
     * draw a rectangle.
     * @param xl left x-coordinate.
     * @param yu upper y-coordinate.
     * @param xr right x-coordinate.
     * @param yb bottom y-coordinate.
     * @param filled if true rectangle is filled
     */
    void rectangle(int xl, int yu,
                   int xr, int yb, bool filled = false);
    
    /**
     * draw a rectangle.
     * @param xl left x-coordinate.
     * @param yu upper y-coordinate.
     * @param xr right x-coordinate.
     * @param yb bottom y-coordinate.
     * @param angle rotation angle at the middle point of the rectangle
     * @param filled if true rectangle is filled
     */
    void rectangle(int xl, int yu,
                   int xr, int yb,
                   const float angle,
                   bool filled = false);
    
    /**
     * draw a polygon represented by the given polygonPoints list
     * optionally, it can be rotated by the given angle and shifted
     * @param poly the polygon vertices
     * @param filled true if the polygon must be filled, false otherwise
     * @param angle rotation angle on the mean point of the vertices
     * @param shift shift amount to the polygon points.
     */
    void polygon(const ipolygonPoints& poly,
                 bool filled = false,
                 const float angle=0.0f,
                 const ipoint& shift=ipoint(0,0));
    
    /**
     * draw a circle with circle center 'p1' and radius 'r'
     */
    void circle(const ipoint& p1, int r, bool filled = false);
    
    /**
     * draw a circle with circle center (x, y) and radius 'r'
     */
    inline void circle(int x, int y,
                       int r, 
                       bool filled = false);

    /**
     * draw an ellipse with center 'p1' and main axes 'aX' and 'aY'
     */
    void ellipse(const ipoint& p1, int aX, int aY,
                 bool filled = false);
    
    /**
     * draw an ellipse with center 'p1' and main axes 'aX' and 'aY'
     */
    void ellipse(const ipoint& p1, int aX, int aY,
                 const float& angle, bool filled = false);

    /**
     * draw an arc from 'pA' to 'pB', clockwise around center 'p1'.
     */
    void arc(const ipoint& p1, const ipoint& pA, const ipoint& pB);

    /**
     * Sets pixels at all points in c, moved by he given offset.
     * @param c the list of points
     * @param offset an optional displacement
     * @param lines an optional boolean indicating if between the
     *        points lines should be drawn
     */
    inline void set(const ipointList& c,
                    const ipoint& offset=ipoint(0,0),
                    const bool lines = false);
    
    /**
     * draw a location.
     *
     * @param loc location with position, angle and radius
     * @param showAngleLine if true, a line from the middle point of the
     *        location (given by loc.position) with the angle given by
     *        loc.angle will be drawn.
     */
    inline void set(const location& loc, bool showAngleLine = false);

    /**
     * draw a rectLocation
     *
     * @param loc location with position, angle and radius
     * @param showAngleLine if true, a line from the middle point of the
     *        location (given by loc.position) with the angle given by
     *        loc.angle will be drawn.
     */
    inline void set(const rectLocation& loc, bool showAngleLine = false);

    /**
     * draw an affineLocation.
     *
     * @param loc affineLocation
     * @param approximateEllipse if true, an ellipse is drawn, otherwise the
     *        transformed square
     * @param showAngleLine if true, a line from the middle point of the
     *        location (given by loc.position) with the angle given by
     *        loc.angle will be drawn.
     */
    inline void set(const affineLocation& loc,
                    bool approximateEllipse = false,
                    bool showAngleLine = false);

    /**
     * draw a list of locations
     */
    inline void set(const list<location>& locs,
                    bool showAngleLine = false);

    /**
     * draw a list of locations
     */
    inline void set(const list<affineLocation>& locs,
                    bool approximateEllipse = false,
                    bool showAngleLine = false);

    /**
     * draw the contents of the vector using the whole image
     * @param vct the vector to be shown
     * @param axisColor color for the axis.  The vector will be drawn in the
     *                  default color (see setColor())
     * @param forceAxis0 if true, both axis will be shown, i.e. the value 0 for
     *                  the vector will be always shown.  If false, only the
     *                  required value-range will be shown
     */
    void set(const dvector& vct,
             const T& axisColor,
             bool forceAxis0 = true);

    /**
     * draw the contents of the vector using the whole image
     * @param vct the vector to be shown
     * @param axisColor color for the axis.  The vector will be drawn in the
     *                  default color (see setColor())
     * @param forceAxis0 if true, both axis will be shown, i.e. the value 0 for
     *                  the vector will be always shown.  If false, only the
     *                  required value-range will be shown
     */
    void set(const fvector& vct,
             const T& axisColor,
             bool forceAxis0 = true);

    /**
     * draw the contents of the vector using the whole image
     * @param vct the vector to be shown
     * @param axisColor color for the axis.  The vector will be drawn in the
     *                  default color (see setColor())
     * @param forceAxis0 if true, both axis will be shown, i.e. the value 0 for
     *                  the vector will be always shown.  If false, only the
     *                  required value-range will be shown     
     */
    void set(const ivector& vct,
             const T& axisColor,
             bool forceAxis0 = true);

    /**
     * Draws a grid in the image.
     * The interpretation of delta depends on the value of interval.
     * if interval is true, the values are taken as number of pixels
     * between two grid lines in x and y direction. Otherwise, it
     * is used as number of grid lines.
     */
    void grid(const ipoint& delta, bool interval=true);

    /**
     * overlay the given channel using the actual color
     * The new pixel color is calculated as (1-c)*old_pixel_color+c*used_color,
     * where c is the channel color and used_color is the color set with
     * setColor().
     * The given channel will be overlayed at the given position
     * (default 0,0)
     * @param overlay channel to be overlayed
     * @param x position at x (the column)
     * @param y position at y (the row)
     */
    void overlay(const channel& overlay, int x = 0, int y = 0);

    /**
     * overlay the given channel using the actual color
     * The new pixel color is calculated as (1-c)*old_pixel_color+c*used_color,
     * where c is the channel color and used_color is the color set with
     * setColor().
     * The given channel will be overlayed at the given position
     * (default 0,0)
     * @param overlay channel to be overlayed
     * @param p position where <code>overlay</code> will be placed
     */
    void overlay(const channel& overlay, const ipoint& p);

    /**
     * overlay the given channel8 using the actual color
     * The new pixel color is calculated as
     * ((255-c)*old_pixel_color+c*used_color)/255,
     * where c is the channel color and used_color is the color set with
     * setColor()
     * @param overlay channel8 to be overlayed
     * @param x position at x (the column)
     * @param y position at y (the row)
     */
    void overlay(const channel8& overlay, int x = 0, int y = 0);

    /**
     * overlay the given channel8 using the actual color
     * The new pixel color is calculated as
     * ((255-c)*old_pixel_color+c*used_color)/255,
     * where c is the channel color and used_color is the color set with
     * setColor()
     * @param overlay channel to be overlayed
     * @param p position where <code>overlay</code> will be placed
     */
    void overlay(const channel8& overlay, const ipoint& p);

    /**
     * draw an integer <code>num</code> at position 'p1' with size 's'.
     * 's' must be given as quota of image height (0.0 to 1.0).
     *
     * @see text()
     *
     * @param num the number to be written
     * @param p1 the position where the number will be drawn
     * @param s scale factor as quota of image height
     */
    void number(int num,
                const ipoint& p1 = ipoint(0,0),
                const float& s = 0.03125f);

    /**
     * draw an integer <code>num</code> at position 'x'/'y' with size 's'.
     * 's' must be given as quota of image height (0.0 to 1.0).
     * @see text()
     * @param num the number to be written
     * @param x the column where the number will be drawn
     * @param y the row where the number will be drawn
     * @param s scale factor as quota of image height
     */
    void number(int num,
                int x,
                int y,
                const float& s);

    /**
     * draw an integer <code>num</code> at position 'x'/'y' with size 's'.
     * 's' must be given as quota of image height (0.0 to 1.0).
     * @see text()
     * @param num the number to be written
     * @param x the column where the number will be drawn
     * @param y the row where the number will be drawn
     */
    inline void number(int num,
                       int x,
                       int y);

    /**
     * Draw the given text at the position <code>upperleft</code> with
     * a specified scale.
     * scale.x and scale.y must have values greater than zero.
     * The original size of one character is 8x8.
     * With the parameter opaqueness you can make transparent text.
     * Opaqueness = 1.0f means no transparency.
     * If <code>horizontal</code> is true the text is writen from left
     * to right.
     * @param txt the output text
     * @param upperleft the coordinates in the image where the text will be
     *                  written
     * @param scale a point with the (integer) scaling factor greater or equal
     *              one for each coordinate.  Note that you can use different
     *              scales for the width and height of the caracters
     * @param opaqueness value in percent (between 0.0f and 1.0f)
     * @param horizontal true left to rigth, false top to bottom
     */
    void text(const std::string txt,
              const ipoint& upperleft = ipoint(0,0),
              const ipoint& scale = ipoint(1,1),
              const float opaqueness = 1.0f,
              bool horizontal = true);


    /**
     * Draw the given text at the position <code>upperleft</code> with
     * a specified scale.
     * scale.x and scale.y must have values greater than zero.
     * The original size of one character is 8x8.
     * With the parameter opaqueness you can make transparent text.
     * Opaqueness = 1.0f means no transparency.
     * If <code>horizontal</code> is true the text is writen from left
     * to right.
     * @param txt the output text
     * @param x the x coordinate in the image where the text will be
     *                written
     * @param y the y coordinate in the image where the text will be
     *                written
     * @param scale a point with the (integer) scaling factor greater or equal
     *              one for each coordinate.  Note that you can use different
     *              scales for the width and height of the caracters
     * @param opaqueness value in percent (between 0.0f and 1.0f)
     * @param horizontal true left to rigth, false top to bottom
     */
    void text(const std::string txt,
              int x,
              int y,
              const ipoint& scale,
              const float opaqueness,
              bool horizontal);

    /**
     * Draw the text string txt at position x, y.
     * @see text(const std::string, int, int, const point&,
     *           const float, bool)
     */
    inline void text(const std::string txt, int x, int y);
    
  protected:
    /**
     * the "actual" image
     */
    matrix<T>* img_;

    /**
     * Color in use
     */
    T actColor_;
    
    /**
     * Returns "true" if the point is within the image
     */
    inline bool inCanvas(const ipoint& p) const;

    /**
     * Returns "true" if the point is within the image
     */
    inline bool inCanvas(int x, int y) const;

    /**
     * Checks if the line-segment between (x, y) and (x2, y2) lies
     * within the image.  If (x, y) lays out of the image boudaries
     * it will be "transformed" to a point in the image
     */
    bool correctLine (int &x, int &y, int x2, int y2);

    /**
     * This is need for drawing filled rotated ellipses
     */
    inline static ipoint turnPoint(const float x, const float y, float angle);

    /**
     * This is need for drawing filled rotated ellipses
     * this enters a point into the point list, the first one into x1
     * the second one into x2
     * if any more come, they are merged into the closest one.
     *
     * The vectors x1 and x2 can be seen as input- or output point lists, while
     * miny and maxy contain the range of y values used until now.
     *
     * You must ensure that the x1 and x2 arrays are bigger than the
     * value contained in p.y.
     */
    inline void enterPX(const ipoint& p,
                        int* x1, int* x2, int& miny, int& maxy);
    
    /**
     * Draws a symbol set with the method setStyle() at the given
     * position. Used by the marker() methods.
     */
    void drawSymbol(int x, int y);

    /**
     * Draws a symbol at the given location.  Used by the marker()
     * methods.
     */
    void drawSymbol(int x, int y, int w,
                    typename drawBase<T>::eMarkerType t);
    
    /**
     * Draws a symbol at the given location.  Used by the marker() methods.
     */
    void drawSymbol(int x, int y, int w, const char* style);
    
  };

}

#include "ltiDraw_inline.h"

#endif

