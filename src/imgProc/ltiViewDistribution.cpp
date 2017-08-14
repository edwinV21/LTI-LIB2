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


/* 
 * \file   ltiViewDistribution.cpp
 *         Contains the class viewDistribution, which generates
 *         viewing directions that are equally distributed over the
 *         upper hemisphere.
 * \author Peter Doerfler
 * \author Peter Koken
 * \date   01.12.2004
 *
 * revisions ..: $Id: ltiViewDistribution.cpp,v 1.8 2012-09-16 05:22:59 alvarado Exp $
 */

#include "ltiMath.h" //for trig functions and sqrt
#undef swap
#include <algorithm> //for sort
#include <set>
//TODO: include files
#include "ltiViewDistribution.h"
#include "ltiConstants.h" // for halfPi()
#include "ltiRound.h"

using std::swap;

namespace lti {
  
  // --------------------------------------------------
  // viewDistribution::spherePoint
  // --------------------------------------------------
  
  //default constructor
  viewDistribution::spherePoint::spherePoint()
    : fpoint3D(0,0,1) {
  }
  
  //copy constructor
  viewDistribution::spherePoint::spherePoint(const spherePoint& other)
    : fpoint3D(other) {
    project();
  }
  
  //value setting constructor
  viewDistribution::spherePoint::spherePoint(const float& xx, 
                                             const float& yy, 
                                             const float& zz)
    : fpoint3D(xx,yy,zz) {
    project();
  }
  
  //direction of the point from the origin
  fpoint 
  viewDistribution::spherePoint::direction(const eAngleType angleType) const {

    fpoint dummy;
    direction(dummy, angleType);
    return dummy;
  }

  //direction of the point from the origin
  void 
  viewDistribution::spherePoint::direction(fpoint& dir,
					   const eAngleType angleType) const {
    
    dir.x=atan2(y,x);
    if (dir.x<0) {
      dir.x+=constants<float>::twoPi();
    }

    // this assumes that the point is on the hull
    // otherwise acos(z/sqrt(x²+y²+z²)) is needed
    dir.y=acos(z);

    //do conversion if needed
    if (angleType==Degrees) {
      dir.x=radToDeg(dir.x);
      dir.y=radToDeg(dir.y);
    }
  }
  
  //project point onto hull
  void viewDistribution::spherePoint::project() {
    const float len=sqrt(x*x+y*y+z*z);
    x/=len;
    y/=len;
    z/=len;
  }

  // angle between two spherePoints
  float viewDistribution::spherePoint::calculateAngle
  (const spherePoint& other) const {
    
    const float dist=this->distanceSqr(other);
    return acos(1.f-0.5f*dist);

  }

  // --------------------------------------------------
  // viewDistribution::sphereTriangle
  // --------------------------------------------------

  //default constructor
  viewDistribution::sphereTriangle::sphereTriangle() {

    c1_=c2_=c3_=spherePoint();

  }

  // constructor
  viewDistribution::sphereTriangle::sphereTriangle(const spherePoint& c1,
                                                   const spherePoint& c2,
                                                   const spherePoint& c3) {
    c1_=c1;
    c2_=c2;
    c3_=c3;
  }

  // destructor
  viewDistribution::sphereTriangle::~sphereTriangle() {}

  //set corners
  void viewDistribution::sphereTriangle::set(const spherePoint& c1,
                                             const spherePoint& c2,
                                             const spherePoint& c3) {
    c1_=c1;
    c2_=c2;
    c3_=c3;
  }    

  //split triangle
  void viewDistribution::sphereTriangle::split(sphereTriangle& child1,
                                               sphereTriangle& child2,
                                               sphereTriangle& child3) {

//     std::cout << "Splitting " << c1_ << std::endl << c2_ << std::endl << c3_ << std::endl;

    const spherePoint a((c1_.x+c3_.x)/2.f,
                        (c1_.y+c3_.y)/2.f,
                        (c1_.z+c3_.z)/2.f);
    const spherePoint b((c1_.x+c2_.x)/2.f,
                        (c1_.y+c2_.y)/2.f,
                        (c1_.z+c2_.z)/2.f);
    const spherePoint c((c2_.x+c3_.x)/2.f,
                        (c2_.y+c3_.y)/2.f,
                        (c2_.z+c3_.z)/2.f);

//     std::cout << "a: " << a << std::endl;
//     std::cout << "b: " << b << std::endl;
//     std::cout << "c: " << c << std::endl;

    child1.set(b,c2_,c);
    child2.set(a,c,c3_);
    child3.set(c1_,b,a);
    set(a,b,c);

//     spherePoint c1,c2,c3;
//     fpoint dir;
//     child1.getCorners(c1,c2,c3);
//     child1.centerDirection(dir, Degrees);
//     std::cout << "child1 " << c1 << std::endl << c2 << std::endl << c3 << std::endl << dir << std::endl;
//     child2.getCorners(c1,c2,c3);
//     child2.centerDirection(dir, Degrees);
//     std::cout << "child2 " << c1 << std::endl << c2 << std::endl << c3 << std::endl<< dir << std::endl;
//     child3.getCorners(c1,c2,c3);
//     child3.centerDirection(dir, Degrees);
//     std::cout << "child3 " << c1 << std::endl << c2 << std::endl << c3 << std::endl<< dir << std::endl;
//     centerDirection(dir, Degrees);
//     std::cout << "child4 " << c1_ << std::endl << c2_ << std::endl << c3_ << std::endl<< dir << std::endl;

  }

  //direction of center
  fpoint viewDistribution::sphereTriangle::
  centerDirection(const eAngleType angleType)
    const {

    fpoint dummy;
    centerDirection(dummy, angleType);
    return dummy;
  }

  //direction of center
  void viewDistribution::sphereTriangle::
  centerDirection(fpoint& dir, const eAngleType angleType) const {

    const spherePoint mid((c1_.x+c2_.x+c3_.x)/3.f,
                          (c1_.y+c2_.y+c3_.y)/3.f,
                          (c1_.z+c2_.z+c3_.z)/3.f);
    mid.direction(dir, angleType);
  }

  void viewDistribution::sphereTriangle::getCorners(fpoint3D& c1,
                                                    fpoint3D& c2,
                                                    fpoint3D& c3) const {
    c1=c1_;
    c2=c2_;
    c3=c3_;
  }

  void viewDistribution::sphereTriangle::getCorners(spherePoint& c1,
                                                    spherePoint& c2,
                                                    spherePoint& c3) const {
    c1=c1_;
    c2=c2_;
    c3=c3_;
  }


  // --------------------------------------------------
  // viewDistribution::parameters
  // --------------------------------------------------

  // default constructor
  viewDistribution::parameters::parameters()
    : functor::parameters() {
    
    useTriangleCenters = true;
    useTriangleCorners = false;
    polyhedronType     = Icosahedron;
    numberOfSplits     = 1;
    angleType          = Degrees;
    elevationRound     = 2.f;
    rotationRound      = 10.f;
  }

  // copy constructor
  viewDistribution::parameters::parameters(const parameters& other)
    : functor::parameters() {
    copy(other);
  }

  // destructor
  viewDistribution::parameters::~parameters() {
  }

  // copy member
  viewDistribution::parameters&
  viewDistribution::parameters::copy(const parameters& other) {
    functor::parameters::copy(other);
    
    useTriangleCenters = other.useTriangleCenters;
    useTriangleCorners = other.useTriangleCorners;
    polyhedronType = other.polyhedronType;
    numberOfSplits = other.numberOfSplits;
    angleType      = other.angleType;
    elevationRound = other.elevationRound;
    rotationRound = other.rotationRound;
    
    return *this;
  }
  
  // alias for copy method
  viewDistribution::parameters&
  viewDistribution::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  // name of the class
  const std::string& viewDistribution::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone method
  viewDistribution::parameters* viewDistribution::parameters::clone() const {
    return new parameters(*this);
  }

  // new instance
  viewDistribution::parameters* viewDistribution::parameters::newInstance() const {
    return new parameters();
  }

  /*
   * Write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool viewDistribution::parameters::write(ioHandler& handler,
                                         const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {
      
      b = b && lti::write(handler,"useTriangleCenters",useTriangleCenters);
      b = b && lti::write(handler,"useTriangleCorners",useTriangleCorners);

      switch (polyhedronType) {
        case Icosahedron:
          lti::write(handler,"polyhedronType","Icosahedron");
          break;
        case Octahedron:
          lti::write(handler,"polyhedronType","Octahedron");
          break;
        case Dodecahedron:
          lti::write(handler,"polyhedronType","Dodecahedron");
          break;
        default:
          lti::write(handler,"polyhedronType","Icosahedron");
          handler.setStatusString("Unknown ePolyhedronType in viewDistribution::parameters::write()");
          b=false;
          break;
      }

      lti::write(handler,"numberOfSplits",numberOfSplits);

      switch (angleType) {
        case Radians:
          lti::write(handler,"angleType","Radians");
          break;
        case Degrees:
          lti::write(handler,"angleType","Degrees");
          break;
        default:
          lti::write(handler,"angleType","Degrees");
          handler.setStatusString("Unknown eAngleType in viewDistribution::parameters::write()");
          b=false;
          break;
      }

      b=b && lti::write(handler,"elevationRound",elevationRound);
      b=b && lti::write(handler,"rotationRound",rotationRound);

    }

    b = b && functor::parameters::write(handler,false);

    if (complete) {
      b = b && handler.writeEnd();
    }

    return b;
  }

  /*
   * Read the parameters from the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also read, otherwise only the data block will be read.
   * @return true if read was successful
   */
  bool viewDistribution::parameters::read(ioHandler& handler,
                                        const bool complete)  {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {

      b = b && lti::read(handler,"useTriangleCenters",useTriangleCenters);
      b = b && lti::read(handler,"useTriangleCorners",useTriangleCorners);

      std::string tmp;
      
      lti::read(handler,"polyhedronType",tmp);
      if ((tmp=="Icosahedron") || (tmp=="isocahedron")) {
        polyhedronType=Icosahedron;
      } else if ((tmp=="Octahedron") || (tmp=="octahedron")) {
        polyhedronType=Octahedron;
      } else if ((tmp=="Dodecahedron") || (tmp=="dodecahedron")) {
        polyhedronType=Dodecahedron;
      } else {
        polyhedronType=Icosahedron;
        std::string msg="Unknown ePolyhedronType " + tmp + " in viewDistribution::parameters::read()";
        handler.setStatusString(msg);
        b=false;
      }
      
      lti::read(handler,"numberOfSplits",numberOfSplits);

      lti::read(handler,"angleType",tmp);
      if ((tmp=="Radians") || (tmp=="radians")) {
        angleType=Radians;
      } else if ((tmp=="Degrees") || (tmp=="degrees")) {
        angleType=Degrees;
      } else {
        angleType=Degrees;
        std::string msg="Unknown eAngleType " + tmp + " in viewDistribution::parameters::read()";
        handler.setStatusString(msg);
        b=false;
      }

      b=b && lti::read(handler,"elevationRound",elevationRound);
      b=b && lti::read(handler,"rotationRound",rotationRound);

    }

    b = b && functor::parameters::read(handler,false);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

  // --------------------------------------------------
  // viewDistribution
  // --------------------------------------------------

  // default constructor
  viewDistribution::viewDistribution()
    : functor() {

    //TODO: comment the attributes of your functor
    // If you add more attributes manually, do not forget to do following:
    // 1. indicate in the default constructor the default values
    // 2. make sure that the copy member also copy your new attributes, or
    //    to ensure there, that these attributes are properly initialized.

    // create an instance of the parameters with the default values
    parameters defaultParameters;
    // set the default parameters
    setParameters(defaultParameters);
  }

  // default constructor
  viewDistribution::viewDistribution(const parameters& par)
    : functor() {

    //TODO: comment the attributes of your functor
    // If you add more attributes manually, do not forget to do following:
    // 1. indicate in the default constructor the default values
    // 2. make sure that the copy member also copy your new attributes, or
    //    to ensure there, that these attributes are properly initialized.

    // set the given parameters
    setParameters(par);
  }


  // copy constructor
  viewDistribution::viewDistribution(const viewDistribution& other)
    : functor() {
    copy(other);
  }

  // destructor
  viewDistribution::~viewDistribution() {
  }

  // copy member
  viewDistribution& 
  viewDistribution::copy(const viewDistribution& other) {
    functor::copy(other);
    return (*this);
  }

  // alias for copy member
  viewDistribution&
  viewDistribution::operator=(const viewDistribution& other) {
    return (copy(other));
  }

  // name of the class
  const std::string& viewDistribution::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone member
  viewDistribution* viewDistribution::clone() const {
    return new viewDistribution(*this);
  }

  // clone member
  viewDistribution* viewDistribution::newInstance() const {
    return new viewDistribution();
  }

  // return parameters
  const viewDistribution::parameters&
  viewDistribution::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }

  // update parameters
  bool viewDistribution::updateParameters() {

    triangles_.clear();

    const parameters& param=getParameters();

    switch (param.polyhedronType) {
      case Icosahedron:
        initIcosahedron(triangles_);
        break;
      case Octahedron:
        initOctahedron(triangles_);
        break;
      case Dodecahedron:
        initDodecahedron(triangles_);
        break;
      default:
        throw invalidParametersException
          ("unknown polyhedronType in viewDistribution::updateParameters()");
    }

    for (int i=1; i<param.numberOfSplits; i++) {
      split(triangles_);
    }

    return true;

  }


  // -------------------------------------------------------------------
  // initialization functions
  // -------------------------------------------------------------------

  void viewDistribution
  ::initOctahedron(std::vector<sphereTriangle>& triang) const {

    const spherePoint p0(1,0,0);
    const spherePoint p1(0,1,0);
    const spherePoint p2(-1,0,0);
    const spherePoint p3(0,-1,0);
    const spherePoint p4(0,0,1);
    
    sphereTriangle t(p0,p1,p4);
    triang.push_back(t);
    t.set(p1,p2,p4);
    triang.push_back(t);
    t.set(p2,p3,p4);
    triang.push_back(t);
    t.set(p3,p0,p4);
    triang.push_back(t);
  }
  
  void viewDistribution::initDodecahedron
  (std::vector<sphereTriangle>& triang) const {

    const float t = (1+sqrt(5.f))/2;
    const float l = 1/(sqrt(3.f));
    const float a = sqrt(t)/sqrt(sqrt(5.f));
    const float b = sqrt(1-a*a);

    const float lTbDt=l*b/t; //l Times b Divided t
    const float lTaTt=l*a*t;
    const float twolTb=2*l*b;
    const float twolTa=2*l*a;
    const float lTt=l*t;
    const float lDt=l/t;
    const float lTa=l*a;
    const float lTb=l*b;
  
    const spherePoint p0(-lTbDt, l, lTaTt); 
    const spherePoint p1(-lTbDt,-l, lTaTt);
    const spherePoint p2(-lTaTt, l,-lTbDt);
    const spherePoint p3(-lTaTt,-l,-lTbDt);
    const spherePoint p4( lTaTt, l, lTbDt);
    const spherePoint p5( lTaTt,-l, lTbDt);
    const spherePoint p6( lTbDt, l,-lTaTt);
    const spherePoint p7( lTbDt,-l,-lTaTt);
 
    const spherePoint q0(-twolTb,0, lTaTt); 
    const spherePoint q1(-twolTa,0, lTbDt); 
    const spherePoint q2( twolTa,0,-lTbDt);
    const spherePoint q3( twolTb,0,-lTaTt); 
    const spherePoint q4( lTa, lDt, lTaTt);
    const spherePoint q5( lTa,-lDt, lTaTt); 
    const spherePoint q6(-lTa, lDt,-lTaTt);
    const spherePoint q7(-lTa,-lDt,-lTaTt);
    const spherePoint q8(-lTb, lTt, lTbDt);
    const spherePoint q9( lTb, lTt,-lTbDt);
    const spherePoint q10(-lTb,-lTt, lTbDt);
    const spherePoint q11( lTb,-lTt,-lTbDt);
  

    // midpoints of the pentagons
    const spherePoint m0(0.2f*(q0.x+p0.x+q4.x+q5.x+p1.x),
                            0.2f*(q0.y+p0.y+q4.y+q5.y+p1.y),
                            0.2f*(q0.z+p0.z+q4.z+q5.z+p1.z));

    const spherePoint m1(0.2f*(q0.x+p1.x+q10.x+p3.x+q1.x),
                            0.2f*(q0.y+p1.y+q10.y+p3.y+q1.y),
                            0.2f*(q0.z+p1.z+q10.z+p3.z+q1.z));


    const spherePoint m2(0.2f*(p1.x+q5.x+p5.x+q11.x+q10.x),
                            0.2f*(p1.y+q5.y+p5.y+q11.y+q10.y),
                            0.2f*(p1.z+q5.z+p5.z+q11.z+q10.z));
  
    const spherePoint m3(0.2f*(q5.x+q4.x+p4.x+q2.x+p5.x),
                            0.2f*(q5.y+q4.y+p4.y+q2.y+p5.y),
                            0.2f*(q5.z+q4.z+p4.z+q2.z+p5.z));
  
    const spherePoint m4(0.2f*(p0.x+q4.x+p4.x+q9.x+q8.x),
                            0.2f*(p0.y+q4.y+p4.y+q9.y+q8.y),
                            0.2f*(p0.z+q4.z+p4.z+q9.z+q8.z));

    const spherePoint m5(0.2f*(q0.x+p0.x+q8.x+p2.x+q1.x),
                            0.2f*(q0.y+p0.y+q8.y+p2.y+q1.y),
                            0.2f*(q0.z+p0.z+q8.z+p2.z+q1.z));

    const spherePoint m6(0.2f*(q1.x+p3.x+q7.x+q6.x+p2.x),
                            0.2f*(q1.y+p3.y+q7.y+q6.y+p2.y),
                            0.2f*(q1.z+p3.z+q7.z+q6.z+p2.z));

    const spherePoint m7(0.2f*(p3.x+q10.x+q11.x+p7.x+q7.x),
                            0.2f*(p3.y+q10.y+q11.y+p7.y+q7.y),
                            0.2f*(p3.z+q10.z+q11.z+p7.z+q7.z));

    const spherePoint m8(0.2f*(q11.x+p5.x+q2.x+q3.x+p7.x),
                            0.2f*(q11.y+p5.y+q2.y+q3.y+p7.y),
                            0.2f*(q11.z+p5.z+q2.z+q3.z+p7.z));

    const spherePoint m9(0.2f*(q9.x+p4.x+q2.x+q3.x+p6.x),
                            0.2f*(q9.y+p4.y+q2.y+q3.y+p6.y),
                            0.2f*(q9.z+p4.z+q2.z+q3.z+p6.z));

    const spherePoint m10(0.2f*(p2.x+q6.x+p6.x+q9.x+q8.x),
                             0.2f*(p2.y+q6.y+p6.y+q9.y+q8.y),
                             0.2f*(p2.z+q6.z+p6.z+q9.z+q8.z));


    sphereTriangle tri(q0,p0,m0);
    triang.push_back(tri);
    tri.set(p0,q4,m0);
    triang.push_back(tri);
    tri.set(q4,q5,m0);
    triang.push_back(tri);
    tri.set(q5,p1,m0);
    triang.push_back(tri);
    tri.set(p1,q0,m0);
    triang.push_back(tri);
  
    tri.set(q0,p1,m1);
    triang.push_back(tri);
    tri.set(p1,q10,m1);
    triang.push_back(tri);
    tri.set(q10,p3,m1);
    triang.push_back(tri);
    tri.set(p3,q1,m1);
    triang.push_back(tri);
    tri.set(q1,q0,m1);
    triang.push_back(tri);

    tri.set(p1,q5,m2);
    triang.push_back(tri);
    tri.set(q5,p5,m2);
    triang.push_back(tri);
    tri.set(p5,q11,m2);
    triang.push_back(tri);
    tri.set(q11,q10,m2);
    triang.push_back(tri);
    tri.set(q10,p1,m2);
    triang.push_back(tri);

    tri.set(q5,q4,m3);
    triang.push_back(tri);
    tri.set(q4,p4,m3);
    triang.push_back(tri);
    tri.set(p4,q2,m3);
    triang.push_back(tri);
    tri.set(q2,p5,m3);
    triang.push_back(tri);
    tri.set(p5,q5,m3);
    triang.push_back(tri);

    tri.set(p0,q4,m4);
    triang.push_back(tri);
    tri.set(q4,p4,m4);
    triang.push_back(tri);
    tri.set(p4,q9,m4);
    triang.push_back(tri);
    tri.set(q9,q8,m4);
    triang.push_back(tri);
    tri.set(q8,p0,m4);
    triang.push_back(tri);
  
    tri.set(q0,p0,m5);
    triang.push_back(tri);
    tri.set(p0,q8,m5);
    triang.push_back(tri);
    tri.set(q8,p2,m5);
    triang.push_back(tri);
    tri.set(p2,q1,m5);
    triang.push_back(tri);
    tri.set(q1,q0,m5);
    triang.push_back(tri);


    tri.set(p2,q1,m6);
    triang.push_back(tri);
    tri.set(q1,p3,m6);
    triang.push_back(tri);
  
    tri.set(p3,q10,m7);
    triang.push_back(tri);
    tri.set(q10,q11,m7);
    triang.push_back(tri);
  
    tri.set(q11,p5,m8);
    triang.push_back(tri);
    tri.set(p5,q2,m8);
    triang.push_back(tri);
 
    tri.set(q9,p4,m9);
    triang.push_back(tri);
    tri.set(p4,q2,m9);
    triang.push_back(tri);

    tri.set(q9,q8,m10);
    triang.push_back(tri);
    tri.set(q8,p2,m10);
    triang.push_back(tri);

  }


  void viewDistribution::initIcosahedron
  (std::vector<sphereTriangle>& triang) const {
    
    const float t  = (1+sqrt(5.f))/2;
    const float a  = sqrt(t)/sqrt(sqrt(5.f));
    const float aa = a*a;
    const float bb = 1-aa;
    const float b  = sqrt(bb);
    const float ab = a*b;

    const spherePoint p0(-bb,a,ab);
    const spherePoint p1(-bb,-a,ab);

    const spherePoint p2(bb,a,-ab);
    const spherePoint p3(bb,-a,-ab);

    const spherePoint p4(0,0,1);
    const spherePoint p5(-2*ab,0,-bb+aa);

    const spherePoint p6(2*ab,0,bb-aa);
    //const spherePoint p7(0,0,-1);       // NOT USED

    const spherePoint p8(aa,b,ab);
    const spherePoint p9(-aa,b,-ab);

    const spherePoint p10(aa,-b,ab);
    const spherePoint p11(-aa,-b,-ab);
  
    sphereTriangle tri(p5,p4,p0);
    triang.push_back(tri);
    tri.set(p0,p8,p4);
    triang.push_back(tri);
    tri.set(p4,p10,p8);
    triang.push_back(tri);
    tri.set(p8,p6,p10);
    triang.push_back(tri);
    tri.set(p10,p3,p6);
    triang.push_back(tri);
    tri.set(p11,p5,p9);
    triang.push_back(tri);
    tri.set(p9,p0,p5);
    triang.push_back(tri);
    tri.set(p5,p4,p1);
    triang.push_back(tri);
    tri.set(p0,p8,p2);
    triang.push_back(tri);
    tri.set(p4,p10,p1);
    triang.push_back(tri);
    tri.set(p8,p6,p2);
    triang.push_back(tri);
    tri.set(p10,p3,p1);
    triang.push_back(tri);
    tri.set(p3,p11,p1);
    triang.push_back(tri);
    tri.set(p11,p5,p1);
    triang.push_back(tri);
    tri.set(p9,p0,p2);
    triang.push_back(tri);

  }

  
  void viewDistribution::split(std::vector<sphereTriangle>& triang) const {

    const int sz=static_cast<int>(triang.size());
    int i;
    sphereTriangle c1,c2,c3;

    //debugging
//     fpoint3D aa,bb,cc;

    for (i=0; i<sz; i++) {
//       triang[i].getCorners(aa,bb,cc);
//       std::cout << "splitting [" << aa << bb << cc << ":\n";
      triang[i].split(c1,c2,c3);
//       triang[i].getCorners(aa,bb,cc);
//       std::cout << "[" << aa << bb << cc << "\n";
//       c1.getCorners(aa,bb,cc);
//       std::cout << "[" << aa << bb << cc << "\n";
//       c2.getCorners(aa,bb,cc);
//       std::cout << "[" << aa << bb << cc << "\n";
//       c3.getCorners(aa,bb,cc);
//       std::cout << "[" << aa << bb << cc << "\n\n";
      triang.push_back(c1);
      triang.push_back(c2);
      triang.push_back(c3);
    }

  }

  inline void 
  viewDistribution::roundDecimal(float& value, const float& fac) {
    
    value=round(value*fac)/fac;
  }

  // -------------------------------------------------------------------
  // The apply-methods!
  // -------------------------------------------------------------------

  
  // On place apply for type std::vector<fpoint>!
  bool viewDistribution::apply(std::vector<fpoint>& directions) const {

    const parameters& param=getParameters();

    directions.clear();
    fpoint dir;
    std::vector<sphereTriangle>::const_iterator it=triangles_.begin();
    const std::vector<sphereTriangle>::const_iterator ite=triangles_.end();
    
    //for debuggin only
    spherePoint c1,c2,c3;

    eAngleType angleType=param.angleType;
   
    if (param.useTriangleCenters) {
      
      for (; it!=ite; it++) {
        (*it).getCorners(c1,c2,c3);
//         std::cout << "[" << c1 << c2 << c3 << "]  ";
        (*it).centerDirection(dir, angleType);
        if (angleType==Degrees) {
          roundDecimal(dir.x, param.rotationRound);
          roundDecimal(dir.y, param.elevationRound);
        }
        if (((angleType==Radians) &&
             (dir.y<constants<float>::halfPi())) ||
            ((angleType==Degrees) &&
             (dir.y<90.f))) {
          directions.push_back(dir);
//           std::cout << dir;
        }
//         std::cout << std::endl;
      }
      
    }

    
    if (param.useTriangleCorners) {
      std::set<spherePoint> corners;
      for (it=triangles_.begin(); it!=ite; it++) {
        (*it).getCorners(c1,c2,c3);
        corners.insert(c1);
        corners.insert(c2);
        corners.insert(c3);
      }
      std::set<spherePoint>::const_iterator cit=corners.begin();
      const std::set<spherePoint>::const_iterator cite=corners.end();
      std::set<fpoint> tmpDirs;
      for (; cit!=cite; cit++) {
//         std::cout << (*cit) << std::endl;
        (*cit).direction(dir, angleType);
        if (angleType==Degrees) {
          roundDecimal(dir.x, param.rotationRound);
          roundDecimal(dir.y, param.elevationRound);
        }
        if (((angleType==Radians) &&
             (dir.y<constants<float>::halfPi())) ||
            ((angleType==Degrees) &&
             (dir.y<90.f))) {
          tmpDirs.insert(dir);
//         std::cout << dir;
        }
//       std::cout << std::endl;
      }
      std::set<fpoint>::const_iterator dit=tmpDirs.begin();
      const std::set<fpoint>::const_iterator dite=tmpDirs.end();
      directions.insert(directions.end(),dit,dite);
    }      
    
    using std::swap;
    std::sort(directions.begin(),directions.end());
      
    return true;

  }

  bool viewDistribution::apply(std::vector<fpoint3D>& tris) const {
    
    std::vector<sphereTriangle>::const_iterator it=triangles_.begin();

    const int sz=static_cast<int>(triangles_.size())*3;
    tris.resize(sz);
    int i;
    for (i=0; i<sz; i++, it++) {
      (*it).getCorners(tris[i],tris[i+1],tris[i+2]);
    }
   
    return true;
  }

}

