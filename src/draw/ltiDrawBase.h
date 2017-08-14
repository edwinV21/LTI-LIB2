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
 * \file   ltiDrawBase.h
 * \author Jens Paustenbach
 * \date   19.11.2002
 * 
 * $Id: ltiDrawBase.h,v 1.13 2009-06-08 01:02:48 alvarado Exp $
 */

#ifndef _LTI_DRAW_BASE_H_
#define _LTI_DRAW_BASE_H_

#include "ltiObject.h"
#include "ltiStatus.h"
#include "ltiTypes.h"
#include "ltiImage.h"
#include "ltiIoPoints.h"
#include "ltiBorderPoints.h"
#include "ltiAreaPoints.h"
#include "ltiIoHandler.h"
#include "ltiLocation.h"
#include "ltiAffineLocation.h"
#include "ltiRectLocation.h"
#include "ltiKernel1D.h"
#include "ltiKernel2D.h"
#include "ltiSeparableKernel.h"
#include "ltiPoint.h"
#include "ltiPointList.h"
#include "ltiRectangle.h"
#include "ltiList.h"

namespace lti {

  /**
   *  Abstract parent class for draw and epsDraw.
   *  This class provides all methods that are implemented in ltiDraw and
   *  ltiEpsDraw. It makes a base class availabel with all generall drawing
   *  methods. Some special methods, are not implemented in this base class.
   *  The class is aiming at providing a base class, so that, according to
   *  the given parameter, a picture is drawn or an eps file written. <br>
   */
  template<class T>
  class drawBase : public object, public status {
  public:

    /**
     * Specifies different types of marker which can be defined for
     * the marker-method.
     *
     * @see setMarkerType, setStyle
     */
    enum eMarkerType {
      Pixel = 0,     /**< Pixel          ('.') */
      Circle,        /**< Circle         ('o') */
      Xmark,         /**< Xmark          ('x') */
      Plus,          /**< Plus           ('+') */
      Star,          /**< Star           ('*') */
      Square,        /**< Square         ('s') */
      Diamond,       /**< Diamond        ('d') */
      TriangleUp,    /**< Triangle up    ('^') */
      TriangleDown,  /**< Triangle down  ('v') */
      TriangleLeft,  /**< Triangle left  ('<') */
      TriangleRight, /**< Triangle right ('>') */
      Dot,           /**< Dot            ('#') */
      LtiLogo        /**< LTI-Logo*/
    };

    /**
     * default constructor
     */
    drawBase();


    /**
     * copy constructor
     * @param other the object to be copied
     */
    drawBase(const drawBase& other);

    /**
     * destructor
     */
    virtual ~drawBase();

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    drawBase& copy(const drawBase& other);

    /**
     * returns the name of this type
     */
    virtual const std::string& name() const;

    /**
     * Specifies grayscale level to be used (range from 0 to 1) for
     * the next graphics objects.
     */
    virtual void setGray(const float k) = 0;

    /**
     * Specifies the size for the next markers.
     * Default is 5 in draw and 2 in epsDraw
     */
    virtual void setMarkerSize(int size);

    /**
     * Specifies color to be used.
     */
    virtual void setColor(const char* color) = 0;

    /**
     * Specifies color to be used.
     */
    virtual void setColor(const T& color) = 0;

    /**
     * Sets the style used for drawing points. This includes the color and
     * the symbols for the %point
     * @see point(int, int, const char*)
     *
     * The string characterizing consists of two parts: the first
     * definining color and the second defining the symbol. These are
     * as close to the MATLAB(TM) attributes as possible. If one of the
     * parts is missing, the corresponding value remains unchanged.
     *
     * <table>
     * <tr>
     * <th>Colors</th> <th width="10%" > <br></th> <th>Symbols</th>
     * </tr>
     * <tr> <td>
     * <table>
     * <tr><td>y</td><td>yellow</td></tr>
     * <tr><td>m</td><td>magenta</td></tr>
     * <tr><td>c</td><td>cyan</td></tr>
     * <tr><td>r</td><td>red</td></tr>
     * <tr><td>g</td><td>green</td></tr>
     * <tr><td>b</td><td>blue</td></tr>
     * <tr><td>w</td><td>white</td></tr>
     * <tr><td>k</td><td>black</td></tr>
     * </table>
     * </td> <td> <br> </td>
     * <td>
     * <table>
     * <tr><td>.</td><td>point/pixel</td></tr>
     * <tr><td>o</td><td>circle</td></tr>
     * <tr><td>x</td><td>x-mark</td></tr>
     * <tr><td>+</td><td>plus</td></tr>
     * <tr><td>*</td><td>star</td></tr>
     * <tr><td>s</td><td>square</td></tr>
     * <tr><td>d</td><td>diamond</td></tr>
     * <tr><td>^</td><td>triangle (up)</td></tr>
     * <tr><td>v</td><td>triangle (down)</td></tr>
     * <tr><td><</td><td>triangle (left)</td></tr>
     * <tr><td>></td><td>triangle (right)</td></tr>
     * <tr><td>#</td><td>dot</td></tr>
     * </table>
     * </td></tr>
     * </table>
     *
     * In addition all colors except for black can be darkened by a
     * number between 0 and 9 following the color letter. The default
     * is 0. The higher the number, the darker the color. Also the
     * closed symbols are available in a filled version. This is
     * activated by an 'f' following the symbol specifier.
     *
     * Colors only work properly if the canvas used is of type image,
     * ie using rgbaPixel as template type. Otherwise, gray-levels are
     * used instead of the colors. Here, 'c', 'm', and 'y' result in
     * the same gray-level as do 'r', 'g' and 'b'. For better
     * predictability use white with different factors for getting
     * gray-levels.
     *
     * A change of the color using setColor(const char*) or
     * setColor(const T&) does not affect the type of symbol being
     * used.
     */
    virtual void setStyle(const char* style);

    /**
     * Specifies the marker type to be used for points
     */
    virtual void setMarkerType(eMarkerType m);

    /*
     * Sets the pattern for drawing lines. All subsequent lines
     * will be drawn in the given pattern. The default is the
     * continuous line.
     * @param pat pattern for drawing lines.
     */
    // virtual void setLinePattern(const linePattern& pat);

    /*
     * Sets the pattern for filling areas. All subsequenly filled
     * areas will be filled with the given pattern. The default
     * pattern is a homogenouos fill.
     * @param pat pattern for filling areas.
     */
    // virtual void setFillPattern(const fillPattern& pat);

    /**
     * returns the size of the drawing area.
     */
    virtual ipoint getCanvasSize() = 0;

    /**
     * sets a single pixel at point (x, y)
     * In epsDraw this pixel is a small square.
     */
    virtual void set(int x, int y) = 0;

    /**
     * sets a single pixel at p
     */
    virtual inline void set(const ipoint& p);
    
    /**
     * sets a single pixel at p
     */
    /*
    template<class U>
    inline void set(const hPoint2D<U>& p);
    */

    /**
     * sets pixels at all points in c, moved by the given offset
     */
    virtual void set(const ipointList& c, 
                     const ipoint& offset = ipoint(0,0),
                     const bool lines = false);

    /**
     * draw a location.
     *
     * @param loc location with position, angle and radius
     * @param showAngleLine if true, a line from the middle point of the
     *        location (given by loc.position) with the angle given by
     *        loc.angle will be drawn.
     */
    virtual void set(const location& loc, bool showAngleLine = false);

    /**
     * draw a rectLocation
     *
     * @param loc location with position, angle and radius
     * @param showAngleLine if true, a line from the middle point of the
     *        location (given by loc.position) with the angle given by
     *        loc.angle will be drawn.
     */
    virtual void set(const rectLocation& loc, bool showAngleLine = false);

    /**
     * draw an affineLocation. Depending on \p approximateEllipse an ellipse
     * is approximated from the transMatrix of \p loc or the transformed
     * square is drawn.
     *
     * @param loc affineLocation
     * @param approximateEllipse if true, an ellipse is drawn, otherwise the
     *        transformed square
     * @param showAngleLine if true, a line from the middle point of the
     *        location (given by loc.position) with the angle given by
     *        loc.angle will be drawn.
     */
    virtual void set(const affineLocation& loc,
                     bool approximateEllipse = false,
                     bool showAngleLine = false);

    /**
     * draws a list of locations
     *
     * @see set(const location&, bool);
     */
    virtual void set(const list<location>& locs,
                     bool showAngleLine = false);

    /**
     * draws a list of affineLocation
     *
     * @see set(const affineLocation&, bool, bool);
     */
    virtual void set(const list<affineLocation>& locs,
                     bool approximateEllipse = false,
                     bool showAngleLine = false);

    /**
     * Set marker at point (x, y).
     * If there is no style, markerType or width is given, the value
     * set by the setStyle or setWidth is used.
     * @param x the x coordinate
     * @param y the y coordinate
     */
    virtual inline void marker(int x, int y);

    /**
     * Set marker at point (x, y).
     * If there is no style, markerType or width is given, the value
     * set by the setStyle or setWidth is used.
     * @param x the x coordinate
     * @param y the y coordinate
     * @param width the size of the marker symbol
     */
    virtual inline void marker(int x, int y, int width);

    /**
     * Set marker at point (x, y).
     * If there is no style, markerType or width is given, the value
     * set by the setStyle or setWidth is used.
     * @param x the x coordinate
     * @param y the y coordinate
     * @param style the style of the marker symbol.
     */
    virtual inline void marker(int x, int y, const char* style);

    /**
     * Set marker at point (x, y).
     * If there is no style, markerType or width is given, the value
     * set by the setStyle or setWidth is used.
     * @param x the x coordinate
     * @param y the y coordinate
     * @param t the type of the marker
     */
    virtual inline void marker(int x, int y, eMarkerType t);

    /**
     * Set marker at point (x, y).
     * If there is no style, markerType or width is given, the value
     * set by the setStyle or setWidth is used.
     * @param x the x coordinate
     * @param y the y coordinate
     * @param w the size of the marker symbol
     * @param style the style of the marker symbol.
     */
    virtual inline void marker(int x, int y, int w, const char* style);

    /**
     * Set marker at point (x, y).
     * If there is no style, markerType or width is given, the value
     * set by the setStyle or setWidth is used.
     * @param x the x coordinate
     * @param y the y coordinate
     * @param w the size of the marker symbol
     * @param t the type of the marker
     */
    virtual inline void marker(int x, int y, int w, eMarkerType t);

    /**
     * Set marker at p.
     * @see marker(int, int)
     */
    virtual inline void marker(const ipoint& p);

    /**
     * Set marker at p with given width.
     * @see marker(int, int)
     */
    virtual inline void marker(const ipoint& p, int width);

    /**
     * Set marker at p with given style
     * @see marker(int, int)
     */
    virtual inline void marker(const ipoint& p, const char* style);

    /**
     * Set given marker type at p.
     * @see marker(int, int)
     */
    virtual inline void marker(const ipoint& p, eMarkerType t);

    /**
     * Set given marker type at p with given width.
     * @see marker(int, int)
     */
    virtual inline void marker(const ipoint& p, int width, eMarkerType t);

    /**
     * Set marker at p with given width and style.
     * @see marker(int, int)
     */
    virtual inline void marker(const ipoint& p, int width, const char* style);

    /*
     * Set marker at p.
     * @param p coordinates of the pixel to be set
     * @see marker(int, int)
     */
    /* TODO
    template <class U>
    inline void marker(const hPoint2D<U>& p);
    */

    /*
     * Set marker at p width given width.
     * @see marker(int, int)
     */
    /* TODO
    template <class U>
    inline void marker(const hPoint2D<U>& p, int width);
    */

    /*
     * Set given marker type at p.
     * @see marker(int, int)
     */
    /* TODO
    template <class U>
    inline void marker(const hPoint2D<U>& p, eMarkerType t);
    */

    /*
     * Set marker at p width given style.
     * @see marker(int, int, const char*)
     */
    /* TODO
    template <class U>
    inline void marker(const hPoint2D<U>& p, const char* style);
    */

    /*
     * Set given marker type at p with given width.
     * @see marker(int, int)
     */
    /* TODO
    template <class U>
    inline void marker(const hPoint2D<U>& p, int width, eMarkerType t);
    */

    /*
     * Set marker at p with given width and style.
     * @see marker(int, int)
     */
    /* TODO
    template <class U>
    inline void marker(const hPoint2D<U>& p, int width, const char* style);
    */

    /**
     * Sets markers at all positions in c, moved by offset.
     * @see marker(int, int)
     *
     * @param c list of points to put markers on.
     * @param offset offset to be added to all points in the previous list.
     */
    virtual inline void marker(const ipointList& c, 
                               const ipoint& offset=ipoint(0,0));

    /**
     * Sets markers at all positions in c, moved by offset, with given width.
     * @see marker(int, int)
     *
     * @param c list of points to put markers on.
     * @param width width for all markers.
     * @param offset offset to be added to all points in the previous list.
     */
    virtual inline void marker(const ipointList& c, int width,
                               const ipoint& offset=ipoint(0,0));

    /**
     * Sets markers at all positions in c, moved by offset, with given style.
     *
     * @param c list of points to put markers on.
     * @param style string with format for the markers.
     * @param offset offset to be added to all points in the previous list.
     *
     * @see marker(int, int)
     */
    virtual inline void marker(const ipointList& c, const char* style,
                               const ipoint& offset=ipoint(0,0));
    
    /**
     * Sets given marker types at all positions in c, moved by offset.
     *
     * @param c list of points to put markers on.
     * @param t type of the markers.
     * @param offset offset to be added to all points in the previous list.
     *
     * @see marker(int, int)
     */
    virtual inline void marker(const ipointList& c, eMarkerType t,
                               const ipoint& offset=ipoint(0,0));

    /**
     * Sets markers at all positions in the c, moved by offset, with given
     * width and style.
     *
     * @param c list of points to put markers on.
     * @param width width of the marker
     * @param style string denoting the marker type.
     * @param offset offset to be added to all points in the previous list.
     *
     * @see marker(int, int)
     */
    virtual void marker(const ipointList& c, int width, const char* style,
                        const ipoint& offset=ipoint(0,0));

    /**
     * Sets given marker types at all positions in the c, moved by offset,
     * with given width.
     *
     * @param c list of points to put markers on.
     * @param width width of the marker
     * @param t marker type
     * @param offset offset to be added to all points in the previous list.
     *
     * @see marker(int, int)
     */
    virtual void marker(const ipointList& c, int width, eMarkerType t,
                        const ipoint& offset=ipoint(0,0));

    /**
     * Draws a line from the point (fx, fy) to point (tx, ty).
     * The "last point" will be defined with the last "set", "line" or
     * "lineTo" method.
     *
     * @param fx x component of the initial point.
     * @param fy y component of the initial point.
     * @param tx x component of the initial point.
     * @param ty y component of the initial point.
     */
    virtual void line(int fx, int fy,
                      int tx, int ty) = 0;


    /**
     * Draws a line from the point p to point p2.
     *
     * @see line(int, int, int, int)
     *
     * @param p1 the initial point.
     * @param p2 the initial point.
     */
    virtual inline void line(const ipoint& p1, const ipoint& p2);

    /**
     * Draws a line from the last point to point (x, y).
     * The "last point" will be defined with the last "set", "point", "line" or
     * "lineTo" method.
     *
     * @param x x component of the initial point.
     * @param y y component of the initial point.
     */
    virtual void lineTo(int x, int y) = 0;

    /**
     * Draw a line from the last point to point p.
     * @see lineTo(int, int)
     *
     * @param p initial point.
     *
     */
    virtual inline void lineTo(const ipoint& p);

    /**
     * Doesn't draw anything, only sets the last position to the given
     * coordinates. This can be useful before using lineTo()
     *
     * @param x x component of the destination point.
     * @param y y component of the destination point.
     */
    virtual void moveTo(int x, int y) = 0;
    
    /**
     * Doesn't draw anything, only sets the last position to the given
     * coordinates. This can be useful before using lineTo()
     *
     * @param p destination point.
     */
    virtual inline void moveTo(const ipoint& p);
    
    /**
     * Draws a vertical line from (x, y1) to (x, y2).
     *
     * @param x x component of the line
     * @param y1 y component of the initial point.
     * @param y2 y component of the final point.
     */
    virtual void verticalLine(int x, int y1, int y2) = 0;

    /**
     * Draws a horizontal line from (x1, y) to (x2, y).
     *
     * @param x1 x component of the initial point.
     * @param x2 x component of the final point.
     * @param y y component of the line.
     */
    virtual void horizontalLine(int x1, int x2, int y) = 0;

    /**
     * Draws a vertical line from (p1.x, p1.y) to (p1.x, p2.y).
     *
     * @param p1 position of the initial point
     * @param p2 provides only the end y position of the line.
     */
    virtual inline void verticalLine(const ipoint& p1, const ipoint& p2);

    /**
     * Draws a horizontal line from (p1.x, p1.y) to (p2.x, p1.y).
     *
     * @param p1 position of the initial point
     * @param p2 provides only the end x position of the line.
     */
    virtual inline void horizontalLine(const ipoint& p1, const ipoint& p2);

    /**
     * Draw a line from the point p with the length "length" and the
     * angle "angle".  The angles should be a value between -2Pi and
     * 2Pi in radian.
     *
     * @param p initial position of the line.
     * @param length length of the line.
     * @param angle angle of the line.
     */
    virtual void polarLine(const ipoint& p,
                           const float& length,
                           const float& angle);

    /**
     * Draws a grid in the image.
     *
     * The interpretation of \a delta depends on the value of \a interval.
     *
     * If \a interval is \a true, the values are taken as number of
     * pixels between two grid lines in x and y direction.
     * Otherwise, it is used as number of grid lines.  
     *
     * \warning This method is not yet implemented in epsDraw!
     *
     * @param delta Number of pixels between lines or number of lines in 
     *              the image.
     * @param interval if \a true then \a delta means distance between lines,
     *                 otherwise it means number of lines.
     */
    virtual void grid(const ipoint& delta, bool interval) = 0;

    /**
     * Draw a rectangle.
     *
     * @param x1 left x-coordinate.
     * @param y1 upper y-coordinate.
     * @param x2 right x-coordinate.
     * @param y2 bottom y-coordinate.
     * @param filled if true rectangle is filled
     */
    virtual void rectangle(int x1, int y1, 
                           int x2, int y2,
                           bool filled = false) = 0;

    /**
     * Draws a rectangle.
     *
     * @param upperLeft upper left corner of the rectangle
     * @param bottomRight bottom right corner of the rectangle
     * @param filled if true rectangle is filled
     */
    virtual inline void rectangle(const ipoint& upperLeft, 
                                  const ipoint& bottomRight,
                                  bool filled = false);

    /**
     * Draw a rectangle.
     *
     * The rectangle must contain the upper left and the bottom right point.
     *
     * @param r rectangle to be drawn
     * @param filled if true rectangle is filled
     */
    virtual inline void rectangle(const lti::rectangle<int>& r, 
                                  bool filled = false);

    /**
     * Draw a polygon represented by the given polygonPoints list
     * optionally, it can be rotated by the given angle and shifted.
     *
     * @param poly the polygon vertices
     * @param filled true if the polygon must be filled, false otherwise
     * @param angle rotation angle on the mean point of the vertices
     * @param shift shift amount to the polygon points.
     */
    virtual void polygon(const ipolygonPoints& poly,
                         bool filled = false,
                         const float angle=0.0f,
                         const ipoint& shift=ipoint(0,0)) = 0;

    /**
     * Draw a circle with circle center 'p1' and radius 'r'.
     *
     * @param p1 center of the circle
     * @param r radius of the circle
     * @param filled if true circle is filled
     */
    virtual void circle(const ipoint& p1, int r, 
                        bool filled = false) = 0;


    /**
     * Draw an ellipse with center 'p1' and main axes 'aX' and 'aY'.
     */
    virtual void ellipse(const ipoint& p1, int aX, int aY,
                         bool filled = false) = 0;

    /**
     * Draw an ellipse with center 'p1' and main axes 'aX' and 'aY'.
     *
     * The ellipse is turned by the given angle.
     *
     * @param p1 center of ellipse
     * @param aX radius of ellipse in the first main direction
     * @param aY radius of ellipse in the second main direction
     * @param angle angle between first main direction and x axis. Must be
     *              given in radians (rad=deg/180*Pi).
     * @param filled if true, the ellipse is filled
     */
    virtual void ellipse(const ipoint& p1, int aX, int aY,
                         const float& angle, bool filled = false) = 0;

    /**
     * Draw an arc from 'pA' to 'pB', clockwise around center 'p1'.
     *
     * @param p1 center of the arc.
     * @param pA initial point
     * @param pB final point
     */
    virtual void arc(const ipoint& p1, const ipoint& pA, const ipoint& pB) = 0;


    /**
     * Draw an arrow. arrow tip will be at (tx, ty).
     * If size<1.0 then tipsize will be the relative portion of arrow length.
     * If size>1.0 then tipsize will be (int)size, independent of arrow length.
     *
     * @param fx x coordinate of the initial point.
     * @param fy y coordinate of the initial point.
     * @param tx x coordinate of the final point.
     * @param ty y coordinate of the final point.
     * @param size size of the arrow
     */
    virtual void arrow(int fx, int fy,
                       int tx, int ty,
                       const float& size=0.2f) = 0;
    
    /**
     * Draw an arrow. arrow tip will be at p2.
     * If size<1.0 then tipsize will be the relative portion of arrow length.
     * If size>1.0 then tipsize will be (int)size, independent of arrow length.
     *
     * @param p1 initial point.
     * @param p2 coordinate of the initial point.
     * @param size size of the arrow
     */
    virtual inline void arrow(const ipoint& p1, 
                              const ipoint& p2, const float& size=0.2f);

    /**
     * Draw the given text at the position x, y.
     * According to the typ of the subclass the position and the lenght of
     * the drawn text could differ
     * @param txt the output text
     * @param x the x coordinate in the image where the text will be
     *                written
     * @param y the y coordinate in the image where the text will be
     *                written
     */
    virtual void text(const std::string txt, int x, int y) = 0;

    /**
     * Draw the given text at the position p=(x, y).
     * @see test(std::string, int, int)
     *
     * @param txt the output text
     * @param p coordinate in the image where the text will be written.
     */
    virtual inline void text(const std::string txt, const ipoint& p);

    /**
     * Draw an integer <code>num</code> at position 'x'/'y'.
     * According to the typ of the subclass the position and the lenght of
     * the drawn text could differ
     * @param num the number to be written
     * @param x the column where the number will be drawn
     * @param y the row where the number will be drawn
     */
    virtual void number(int num, int x, int y) = 0;

    /**
     * Draw an integer <code>num</code> at position p='x'/'y'.
     *
     * @see number(int, int, int)
     *
     * @param num the number to be written
     * @param p the coordinates where the number will be drawn
     */
    virtual inline void number(int num, const ipoint& p);


  protected:

    /**
     * actual (last used) position X
     */
    int actX_;

    /**
     * actual (last used) position Y
     */
    int actY_;

    /**
     * the actual symbol that is used for markers
     */
    eMarkerType actSymbol_;

    /**
     * the actual size that is used for markers
     * for epsDraw the default is 1 and for draw the default is 5
     * because the elements are much smaller and with a size smaller than 5
     * they are indistinguishable.
     */
    int actSize_;

    /**
     * If true symbols are filled where possible.
     */
    bool filled_;

    /**
     * The pattern for drawing lines.
     */
    // TODO 
    // linePattern linepat_;
    
    /**
     * The pattern for filling areas.
     */
    // TODO 
    // fillPattern fillpat_;

    /**
     * Flag telling the drawing methods to use the pattern.
     */
    bool useLinePattern_;

    /**
     * Flag telling the drawing methods to use the pattern.
     */
    bool useFillPattern_;

  private:
    /**
     * Abstract drawing methods for markers. This methods are needed by the
     * marker methods
     */
    virtual void drawSymbol(int x, int y, int w, const char* style) = 0;
    /**
     * Abstract drawing methods for markers. This methods are needed by the
     * marker methods
     */
    virtual void drawSymbol(int x, int y, int w, eMarkerType t) = 0;


  };

}

#include "ltiDrawBase_inline.h"

#endif

