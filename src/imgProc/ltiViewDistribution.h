/*
 * Copyright (C) 1998-2004
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
 * \file   ltiViewDistribution.h
 *         Contains the class lti::viewDistribution, which uniformly 
 *         distributes viewing angles in the upper hemisphere of a view space.
 * \author Peter Doerfler
 * \author Peter Koken
 * \date   1.12.2004
 *
 * revisions ..: $Id: ltiViewDistribution.h,v 1.5 2012-09-16 05:22:59 alvarado Exp $
 */

#ifndef _LTI_VIEW_DISTRIBUTION_H_
#define _LTI_VIEW_DISTRIBUTION_H_

#include "ltiPoint.h"
#include "ltiPoint3D.h"
#include "ltiFunctor.h"

#include <vector>

namespace lti {

  /**
   * Class viewDistribution
   *
   * This class generates approximately uniformly distributed viewing
   * angles in the upper hemisphere of a view space. It can be used to
   * systematically scan a 3D object with the elevation and rotation
   * as degrees of freedom.
   *
   * To achieve uniform distribution of the views a Platonic
   * polyhedron is used to approximate the unit sphere. It is aligned
   * such that one vertice is located at the North pole of the
   * sphere. The center of each triangle serves as a viewing angle. To
   * increase the number of views each triangle can be split into 4
   * smaller triangle, repeatedly. With each split the accuracy of the
   * uniformity of the distribution decreases.
   *
   * The following initial polyhedrons are available (see ePolyhedronType):
   * - Icosahedron: 20 triangular faces. Usually the best approximation 
   *                results.
   * - Octahedron: 8 triangular faces.
   * - Dodecahedron: 12 pentagonal faces, resulting in 60 triangular
   * faces which are already non equilateral, and thus result in
   * non-uniform distribution.
   *
   * Since only the upper hemisphere of an object is usually visible
   * only those viewing angles are returned. Let \a s be the number of
   * splits and \a t the initial number of triangles. Then the number
   * of views \a n can be calculated as:
   *
   * \a n = 0.5 * t * 4^s
   *
   * @ingroup gAcquisition
   */
  class viewDistribution : public functor {
  public:
    /**
     * This type defines which Platonic solid is used as a first
     * uniform distribution of points on the viewing sphere. The
     * parameter numberOfSplits defines how often the initial
     * triangles are split to receive the final configuration.  Note
     * that only half the number of faces is used for the upper
     * hemisphere.
     */
    enum ePolyhedronType {
      Icosahedron, /**< An icosahedron has 20 faces and usually
		      results in the best approximations of a uniform
		      distribution of views */
      Octahedron,  /**< An octahedron has 8 faces */
      Dodecahedron /**< A dodecahedron has 12 pentagonal faces which
		      results in 60 triangles. These are already not
		      equilateral.*/
    };
    
    
    /**
     * The angles can either be returned in radians or
     * degrees. Ranges are:
     * - elevation: 0-90 degrees or 0-Pi/2 radians
     * - rotation: 0-360 degrees or 0-2*Pi radians
     */
    enum eAngleType {
      Radians, /**< Return the angles in radians */
      Degrees /**< Return the angles in degrees */
    };
    
    /**
     * The parameters for the class viewDistribution
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
       * Returns the name of this class
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
       * Use the directions of the triangle centers.
       *
       * Default: true
       */
      bool useTriangleCenters;

      /**
       * Use the directions of the corners of the triangles.
       *
       * Default: false
       */
      bool useTriangleCorners;

      /**
       * Sets the ePolyhedronType which is used as a starting point
       * for the generation of viewing angles.
       *
       * Default: Icosahedron (20 faces => 10 views)
       */
      ePolyhedronType polyhedronType;

      /**
       * The number of times each triangle in the original
       * configuration is split. Each split multiplies the number of
       * triangles by 4. Thus, the number of views is
       *
       * nbViews = 0.5 * nbTrianglesInPolyhedron * 4^numberOfSplits
       *
       * Default: 1 results in 40 views in the upper hemisphere 
       */
      int numberOfSplits;

      /**
       * The angle type defines how the viewing angles are returned:
       * in Degrees or Radians.
       *
       * Default: Degrees
       */
      eAngleType angleType;

      /**
       * The elevation angles are rounded to
       * 1/elevationRound. E.g. elevationRound = 10. lets the the
       * angles be exact to a tenth of a degree. This is only used
       * when angleType==Degrees.
       *
       * Default: 2.0 => round to half degrees.
       */
      float elevationRound;

      /**
       * The rotation angles are rounded to
       * 1/rotationRound. E.g. rotationRound = 10. lets the the
       * angles be exact to a tenth of a degree. This is only used
       * when angleType==Degrees.
       *
       * Default: 10.0 => round to tenth of a degree.
       */
      float rotationRound;
    };

    /**
     * Default constructor
     */
    viewDistribution();

    /**
     * Construct a functor using the given parameters
     */
    viewDistribution(const parameters& par);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    viewDistribution(const viewDistribution& other);

    /**
     * Destructor
     */
    virtual ~viewDistribution();
    
    /**
     * Generates viewing angles as specified by the parameters.
     *
     * The first element of each point is the elevation theta (0-90
     * degrees or 0-Pi/2), the second element is the rotation phi
     * (0-360 degrees or 0-2*Pi). Angles are given in radians or
     * degrees depending on parameters::angleType.
     *
     * @param angles the viewing angles generated. 
     * @return true if apply successful or false otherwise.
     */
    bool apply(std::vector<fpoint>& angles) const;

    /**
     * Returns the triangles used for finding optimal viewing
     * angles. Three consecutive points in the \a triangles make up
     * one triangle. The triangles are unsorted. This function is
     * mostly for visualization.
     */
    bool apply(std::vector<fpoint3D>& triangles) const;

    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    viewDistribution& copy(const viewDistribution& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    viewDistribution& operator=(const viewDistribution& other);

    /**
     * Returns the name of this type ("lti::viewDistribution")
     */
    virtual const std::string& name() const;

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual viewDistribution* clone() const;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual viewDistribution* newInstance() const;

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * Update parameters.
     *
     * Some functors make some pre-computations when the parameters are set.
     * For example, they initialize look-up tables, build some filter kernels,
     * etc.  This job has to be done by update parameters.
     *
     * In this functor the basic triangles are initialized and split
     * according to the parameters. The actual extraction of triangle
     * points or directions is done in the apply member functions.
     *
     * For disobedient people, who ignore the recomendation of using
     * setParameters() over useParameters(), this method should be called 
     * if attributes of the parameters instance are changed, before calling
     * the apply() method.
     *
     * \warning If you call an apply() method of a functor in which you
     * changed the attributes of the parameters instance without updating
     * them, you have to expect an unpredictible behaviour: segmentation
     * fault, system crash, blue screens, etc. can occur as the sizes of
     * internal structures may not be consistent with the given parameters.
     * This problem does not exist if you indicate the parameters to be used
     * with setParameters(), as the functor will have an instance of its own.
     */
    virtual bool updateParameters();

  protected:

    /**
     * A spherePoint is a 3D point that is locatet on the hull of the
     * unit sphere. Other 3D points used to initialize or set a sphere
     * point are automatically projected to a point on the hull in the
     * same direction as the original point.
     */
    class spherePoint : public fpoint3D {
      
    public:

      /** 
       * default constructor. The default point is (0,0,1), the North
       * pole
       */
      spherePoint();

      /** copy constructor */
      spherePoint(const spherePoint& other);
      
      /**
       * Constructor that initializes the coordinates to the given
       * values. Note that this point is instantly projected onto the
       * hull of the unit sphere.
       */
      spherePoint(const float& x, const float& y, const float& z);

      /**
       * returns the direction of the point looking from the
       * origin. The \a x value of the returned point contains the
       * rotation \a phi and the \a y value contains the elevation \a
       * theta.
       */
      fpoint direction(const eAngleType angleType) const;

      /**
       * Sets \a dir to the direction of the point looking from the
       * origin. The \a x value of the returned point contains the
       * rotation \a phi and the \a y value contains the elevation \a
       * theta.
       */
      void direction(fpoint& dir,const eAngleType angleType) const;

      /**
       * Calculate the angle between this spherePoint and the \a other
       * one. The result is in Radians.
       */
      float calculateAngle(const spherePoint& other) const;

    protected:

      /**
       * projects any fpoint3D onto the hull of the unit sphere by
       * keeping the direction. 
       */
      void project();
    };

    /**
     * A sphereTriangle is a triangle whose corner points are
     * spherePoints and thus lie on the hull of the unit sphere.
     *
     * A sphereTriangle can be split into four new sphereTriangles by
     * splitting each edge in half, projecting the resulting vertices
     * onto the hull and connecting these points.
     */
    class sphereTriangle {
    public:
      
      /**
       * Default constructor. Sets all three corners to (0,0,1).
       */
      sphereTriangle();

      /**
       * Constructor sets the three corners.
       */
      sphereTriangle(const spherePoint& c1,
                     const spherePoint& c2,
                     const spherePoint& c3);

      /**
       * Destructor.
       */
      ~sphereTriangle();

      /**
       * sets the three corners of the sphere triangle
       */
      void set(const spherePoint& c1,
               const spherePoint& c2,
               const spherePoint& c3);

      /**
       * splits the triangle into four child triangles. In this
       * process the original triangle (this) is replaced with the
       * middle child.
       */
      void split(sphereTriangle& child1,
                 sphereTriangle& child2,
                 sphereTriangle& child3);

      /**
       * Returns the direction to the center of the sphereTriangle.
       * The angles are in radians or degrees depending on \a
       * angleType. The \a x attribute of the fpoint is the rotation
       * phi and the \a y element is the elevation theta.
       */
      fpoint centerDirection(const eAngleType angleType) const;

      /**
       * Returns the direction to the center of the sphereTriangle in
       * \a dir. The \a x attribute is the rotation phi and the \a y
       * element is the elevation theta. The angles are in radians or
       * degrees depending on \a angleType.
       */
      void centerDirection(fpoint& dir,const eAngleType angleType) const;

      
      /**
       * Returns the three corners as fpoint3D
       */
      void getCorners(fpoint3D& c1, fpoint3D& c2, fpoint3D& c3) const;

      /**
       * Returns the three corners
       */
      void getCorners(spherePoint& c1, spherePoint& c2, spherePoint& c3) const;

    protected:
      
      /**
       * the three corners
       */
      spherePoint c1_, c2_, c3_;
    };
    
    /**
     * initialize upper hemisphere icosahedron
     */
    void initIcosahedron(std::vector<sphereTriangle>& triangles) const;

    /**
     * initialize upper hemisphere dodecahedron triangles
     */
    void initDodecahedron(std::vector<sphereTriangle>& triangles) const;

    /**
     * initialize upper hemisphere octahedron
     */
    void initOctahedron(std::vector<sphereTriangle>& triangles) const;

    /**
     * split all triangles found in \a triangles
     */
    void split(std::vector<sphereTriangle>& triangles) const;

    /**
     * Round \a value to the nearest 1/fac.
     */
    static inline void roundDecimal(float& value, const float& fac);

    /**
     * contains all triangles as specified by the current parameters.
     */
    std::vector<sphereTriangle> triangles_;

  };
}

#endif

