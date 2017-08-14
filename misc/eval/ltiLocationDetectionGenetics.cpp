/*
 * Copyright (C) 2006
 * Pablo Alvarado, ITCR
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
 * \file   ltiLocationDetectionGenetics.cpp
 *         Defines the abstract parent class of the genetics for denoising 
 *         algorithm.
 * \author Pablo Alvarado
 * \date   02.12.07
 *
 * $Id: ltiLocationDetectionGenetics.cpp,v 1.5 2008-09-23 19:51:34 alvarado Exp $
 */

#include "ltiLocationDetectionGenetics.h"
#include "ltiMath.h"
#include "ltiRound.h"
#include "ltiIOImage.h"
#include "ltiLoadImageList.h"
#include "ltiTimer.h"
#include "ltiConstants.h"
#include "ltiList.h"
 
#undef _LTI_DEBUG
//#define _LTI_DEBUG 4
#include "ltiDebug.h"

#ifdef _LTI_DEBUG
#define _LTI_SAVE_PARAM 1

#include "ltiViewer2D.h"
#include "ltiDraw.h"
#include "ltiLispStreamHandler.h"
#include <cstdio>
#include <sys/types.h>
#include <unistd.h>
#include <fstream>
#endif

namespace lti {

  // -------------------------------------------------------------------------
  //   lti::genetics::parameters
  // -------------------------------------------------------------------------

  // default constructor
  locationDetectionGenetics::parameters::parameters()
    : genetics::parameters() {

    numberOfLocations  = false;
    locationsReciprocal = false;
    occupiedOctaves    = false;
    time               = false;
    scaleRepeatability         = false;
    rotationRepeatability      = false;
    rotationScaleRepeatability = true;
    scaleStable         = false;
    rotationStable      = false;
    rotationScaleStable = true;

    images       = "images.txt";
    postfix      = "";
    stepAngle    = 10.0f;
    stepScaling  = sqrt(2.0f);
    firstScaling = 0.25f;
    lastScaling  = 2.0f;
    firstAngle   = 0.0f;
    lastAngle    = 360.0f;

    considerAngle     = true;
    scaleTolerance    = 1.0f/sqrt(2);
    positionTolerance = 1.5f;
    angleTolerance    = 10.0f;
  }

  // copy constructor
  locationDetectionGenetics::parameters::parameters(const parameters& other)
    : genetics::parameters() {
    copy(other);
  }

  // destructor
  locationDetectionGenetics::parameters::~parameters() {
  }

  // copy member

  locationDetectionGenetics::parameters&
  locationDetectionGenetics::parameters::copy(const parameters& other) {
    genetics::parameters::copy(other);

    numberOfLocations          = other.numberOfLocations; 
    locationsReciprocal        = other.locationsReciprocal; 
    occupiedOctaves            = other.occupiedOctaves; 
    time                       = other.time; 
    scaleRepeatability         = other.scaleRepeatability;
    rotationRepeatability      = other.rotationRepeatability;
    rotationScaleRepeatability = other.rotationScaleRepeatability;
    scaleStable                = other.scaleStable;
    rotationStable             = other.rotationStable;
    rotationScaleStable        = other.rotationScaleStable;

    images   = other.images;
    postfix  = other.postfix;

    stepAngle    = other.stepAngle;
    stepScaling  = other.stepScaling;
    firstScaling = other.firstScaling;
    lastScaling  = other.lastScaling;
    firstAngle   = other.firstAngle;
    lastAngle    = other.lastAngle;

    considerAngle     = other.considerAngle;
    scaleTolerance    = other.scaleTolerance;
    positionTolerance = other.positionTolerance;
    angleTolerance    = other.angleTolerance;

    return *this;
  }

  // alias for copy method
  locationDetectionGenetics::parameters&
  locationDetectionGenetics::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  // class name
  const std::string& locationDetectionGenetics::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone method
  locationDetectionGenetics::parameters*
  locationDetectionGenetics::parameters::clone() const {
    return new parameters(*this);
  }

  // new instance
  locationDetectionGenetics::parameters*
  locationDetectionGenetics::parameters::newInstance() const {
    return new parameters();
  }

  /*
   * Write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool 
  locationDetectionGenetics::parameters::write(ioHandler& handler,
                                               const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {

      lti::write(handler,"numberOfLocations",numberOfLocations);
      lti::write(handler,"locationsReciprocal",locationsReciprocal);
      lti::write(handler,"occupiedOctaves",occupiedOctaves);
      lti::write(handler,"time",time);
      lti::write(handler,"scaleRepeatability",scaleRepeatability);
      lti::write(handler,"rotationRepeatability",rotationRepeatability);
      lti::write(handler,"rotationScaleRepeatability",
                         rotationScaleRepeatability);
      lti::write(handler,"scaleStable",scaleStable);
      lti::write(handler,"rotationStable",rotationStable);
      lti::write(handler,"rotationScaleStable",rotationScaleStable);

      lti::write(handler,"images",images);
      lti::write(handler,"postfix",postfix);
      lti::write(handler,"stepAngle",stepAngle);
      lti::write(handler,"stepScaling",stepScaling);
      lti::write(handler,"firstScaling",firstScaling);
      lti::write(handler,"lastScaling",lastScaling);
      lti::write(handler,"firstAngle",firstAngle);
      lti::write(handler,"lastAngle",lastAngle);

      lti::write(handler,"considerAngle",considerAngle);
      lti::write(handler,"scaleTolerance",scaleTolerance);
      lti::write(handler,"positionTolerance",positionTolerance);
      lti::write(handler,"angleTolerance",angleTolerance);
    }

    b = b && genetics::parameters::write(handler,false);

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
  bool locationDetectionGenetics::parameters::read(ioHandler& handler,
                                                   const bool complete)  {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {
      lti::read(handler,"numberOfLocations",numberOfLocations);
      lti::read(handler,"locationsReciprocal",locationsReciprocal);
      lti::read(handler,"occupiedOctaves",occupiedOctaves);
      lti::read(handler,"time",time);
      lti::read(handler,"scaleRepeatability",scaleRepeatability);
      lti::read(handler,"rotationRepeatability",rotationRepeatability);
      lti::read(handler,"rotationScaleRepeatability",
                         rotationScaleRepeatability);
      lti::read(handler,"scaleStable",scaleStable);
      lti::read(handler,"rotationStable",rotationStable);
      lti::read(handler,"rotationScaleStable",rotationScaleStable);

      lti::read(handler,"images",images);
      lti::read(handler,"postfix",postfix);
      lti::read(handler,"stepAngle",stepAngle);
      lti::read(handler,"stepScaling",stepScaling);
      lti::read(handler,"firstScaling",firstScaling);
      lti::read(handler,"lastScaling",lastScaling);
      lti::read(handler,"firstAngle",firstAngle);
      lti::read(handler,"lastAngle",lastAngle);

      lti::read(handler,"considerAngle",considerAngle);
      lti::read(handler,"scaleTolerance",scaleTolerance);
      lti::read(handler,"positionTolerance",positionTolerance);
      lti::read(handler,"angleTolerance",angleTolerance);
    }

    b = b && genetics::parameters::read(handler,false);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }


  // -------------------------------------------------------------------------
  //   lti::locationDetectionGenetics
  // -------------------------------------------------------------------------

  /*
   * Default constructor.
   */
  locationDetectionGenetics::locationDetectionGenetics() 
    : genetics(false), detector_(0), dimFitness_(0) {
  }

  /*
   * Default constructor.
   */
  locationDetectionGenetics::
  locationDetectionGenetics(const locationDetectionGenetics& other) 
    : genetics(false), detector_(0), dimFitness_(0) {
    copy(other);
  }

  /*
   * Destructor
   */
  locationDetectionGenetics::~locationDetectionGenetics() {
    delete detector_;
    detector_ = 0;
  }

  /*
   * Return the fully qualified name of this class.
   */
  const std::string& locationDetectionGenetics::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  /*
   * Copy method
   */
  locationDetectionGenetics& 
  locationDetectionGenetics::copy(const locationDetectionGenetics& other) {
    genetics::copy(other);
    setDetector(other.getDetector());
    // all other attributes are initialized by updateParameters, called when
    // the copy of the parent class sets the parameters.
    return *this;
  }
  
  /*
   * Alias of copy
   */
  locationDetectionGenetics& 
  locationDetectionGenetics::operator=(const locationDetectionGenetics& other){
    return copy(other);
  }
  

  bool 
  locationDetectionGenetics::write(ioHandler& handler,const bool c) const {
    return genetics::write(handler,c);
  }

  bool locationDetectionGenetics::read(ioHandler& handler,const bool c) {
    return genetics::read(handler,c);
  }

    // return parameters
  const locationDetectionGenetics::parameters& 
  locationDetectionGenetics::getParameters() const {
    
    const parameters* par =
      dynamic_cast<const parameters*>(&parametersManager::getParameters());

    if(isNull(par)) {
      throw invalidParametersException(name());
    }

    return *par;
  }

  bool locationDetectionGenetics::updateParameters() {
    if (!genetics::updateParameters()) {
      return false;
    }

    const parameters& par = getParameters();

    ioImage iloader;
    loadImageList loader;
    loader.useFileList(par.images);
    const int numImgs = loader.computeSize();

    origs_.clear();
    origs_.reserve(numImgs);

    masks_.clear();
    masks_.reserve(numImgs);

    std::string file;
    image img,nimg;
    channel chnl,empty,diff;
    channel8 empty8,nchnl;

    while (loader.hasNext()) {
      file = loader.getNextFilename();
      if (loader.load(img)) {
        if (img.empty()) {
          continue;
        }
        chnl.castFrom(img);
        if (par.postfix.empty() || 
            !iloader.load(getMaskName(file,par.postfix),nimg)) {
          // use an empty channel to 
          nchnl.clear();
        } else {
          nchnl.castFrom(nimg);
        }

        // cache the images
        origs_.push_back(empty);
        chnl.detach(origs_.back());
        masks_.push_back(empty8);
        nchnl.detach(masks_.back());
      }
    }

    dimFitness_ = ((par.numberOfLocations ?1:0) +
                   (par.locationsReciprocal ?1:0) +
                   (par.occupiedOctaves ?1:0) +
                   (par.time ?1:0) +
                   (par.scaleRepeatability ?1:0) +
                   (par.rotationRepeatability ?1:0) +
                   (par.rotationScaleRepeatability ?1:0) +
                   (par.scaleStable ?1:0) +
                   (par.rotationStable ?1:0) +
                   (par.rotationScaleStable ?1:0));

    trans_type::parameters gtPar;
    gtPar.resizeMode = trans_type::AdjustDimensions;
    gtPar.interpolatorParams.boundaryType = Constant;

    trans_.setParameters(gtPar);

    if (origs_.empty()) {
      setStatusString("No images could be found.  Maybe wrong path?");
      return false;
    }

    return true;
  }

  /*
   * Attach the instance of the denoiser to be used.
   *
   * This class will take care of the memory management
   */
  bool 
  locationDetectionGenetics::setDetector(const locationDetection& instance) {
    delete detector_;
    if (isNull(&instance)) {
      detector_= 0;
    } else {
      detector_ = instance.clone();
    }
    return notNull(detector_);
  }

  /*
   * Return a read-only reference to the internal denoiser
   */
  const locationDetection& locationDetectionGenetics::getDetector() const {
    return *detector_;
  }
   
  /*
   * Check if a valid locationDetection instance has already been set.
   */
  bool locationDetectionGenetics::canDetect() const {
    return notNull(detector_);
  }

  bool 
  locationDetectionGenetics::evaluateChromosome(const int, // id, here unused!
                                                const chromosome& individual,
                                                dvector& fitness) const {
    return evaluateChromosome(individual,fitness);
  }

  bool 
  locationDetectionGenetics::evaluateChromosome(const chromosome& individual,
                                                dvector& fitness) const {

    const locationDetection::parameters* lsPar =
      chromosomeToPhenotype(individual);
    dmatrix mfitness;

#if defined(_LTI_SAVE_PARAM)
    // for debug purposes, if the system crash the given file will have
    // the parameters currently under evaluation, so that it can be possible
    // to isolate the case:
    char buffer[128];
    sprintf(buffer,"lti_ls_eval_dbg_%i.txt",getpid());
    std::ofstream _dbg_out(buffer);
    lispStreamHandler lsh(_dbg_out);
    lsPar->write(lsh);
    _dbg_out << std::endl;
    _dbg_out.close();
#endif


    if (evaluate(*lsPar,mfitness,true)) {

      const parameters& par = getParameters();
      fitness.allocate(dimFitness_);
      int j(0);

      if ((par.numberOfLocations) && (j<dimFitness_)) {
        fitness.at(j)=mfitness.at(0,IdxNumberOfLocations);
        j++;
      } 

      if ((par.locationsReciprocal) && (j<dimFitness_)) {
        fitness.at(j)=mfitness.at(0,IdxLocationsRecip);
        j++;
      } 

      if ((par.occupiedOctaves) && (j<dimFitness_)) {
        fitness.at(j)=mfitness.at(0,IdxOccupiedOctaves);
        j++;
      }

      if ((par.time) && (j<dimFitness_)) {
        fitness.at(j)=mfitness.at(0,IdxTime);
        j++;
      }

      if ((par.scaleRepeatability) && (j<dimFitness_)) {
        if (par.considerAngle) {
          fitness.at(j)=mfitness.at(0,IdxScalePosRep);
        } else {
          fitness.at(j)=mfitness.at(0,IdxScaleARep);
        }
        j++;
      }

      if ((par.rotationRepeatability) && (j<dimFitness_)) {
        if (par.considerAngle) {
          fitness.at(j)=mfitness.at(0,IdxRotationPosRep);
        } else {
          fitness.at(j)=mfitness.at(0,IdxRotationARep);
        }
        j++;
      }

      if ((par.rotationScaleRepeatability) && (j<dimFitness_)) {
        if (par.considerAngle) {
          fitness.at(j)=mfitness.at(0,IdxRSPosRep);
        } else {
          fitness.at(j)=mfitness.at(0,IdxRSARep);
        }
        j++;
      }

      // 
      if ((par.scaleStable) && (j<dimFitness_)) {
        if (par.considerAngle) {
          fitness.at(j)=mfitness.at(0,IdxScalePosStable);
        } else {
          fitness.at(j)=mfitness.at(0,IdxScaleAStable);
        }
        j++;
      }

      if ((par.rotationStable) && (j<dimFitness_)) {
        if (par.considerAngle) {
          fitness.at(j)=mfitness.at(0,IdxRotationPosStable);
        } else {
          fitness.at(j)=mfitness.at(0,IdxRotationAStable);
        }
        j++;
      }

      if ((par.rotationScaleStable) && (j<dimFitness_)) {
        if (par.considerAngle) {
          fitness.at(j)=mfitness.at(0,IdxRSPosStable);
        } else {
          fitness.at(j)=mfitness.at(0,IdxRSAStable);
        }
        j++;
      }

    } else {
      delete lsPar;
      return false;
    }

    delete lsPar;
    return true;
  }

  bool locationDetectionGenetics::
  evaluate(const locationDetection::parameters& param,
           dmatrix& fitness,
           const bool onlyMeanAndVariance) const {

    // This method will be called in parallel, so that it requires
    // its own instance of the detector_ with its own parameters. 

    image img;
    channel chnl;

    // set the location parameters only once
    if (isNull(detector_)) {
      setStatusString("No location detection instance set yet.");
      fitness.clear();
      return false;
    }

    const int size = static_cast<int>(origs_.size());
    if (size == 0) {
      // the statusString was initialized while setting the parameters!
      return false;
    }

    // we need an instance of the detector for this thread only!
    locationDetection* detector = detector_->clone();
    detector->setParameters(param);

    // assume all images are ok
    fitness.assign(size,totalFitnessDimensionality_,-1.0);

    int i=0; // i counts the number of successful read images
//    char buffer[255];

    // for all channels read in updateParameters()
    while (i<size) {
//       if (validProgressObject(2)) {
//         sprintf(buffer,"Image %i/%i",i+1,size);
//         getProgressObject().substep(2,buffer);        
//       }
      evaluate(origs_[i],masks_[i],fitness.getRow(i),*detector);
      ++i;
    }

    // compute the statistics if desired
    if (onlyMeanAndVariance) {
      dmatrix mstats(2,fitness.columns());
      stats_.apply(fitness,mstats.getRow(0),mstats.getRow(1));
      mstats.detach(fitness);
    }

    // remove the thread-scoped detector instance
    delete detector;

    return true;
  }

  // evaluate just one channel
  bool 
  locationDetectionGenetics::evaluate(const channel& chnl,
                                      const channel8& mask,
                                      dvector& fitness,
                                      locationDetection& detector) const {

    // first than all, we need to copy the trans_, since this is a constant
    // method and in multithreading approaches it is dangerous ignoring this
    // attribute
    trans_type trans(trans_);

    // original locations
    std::vector<location> olocs;

    // locations of the transformed channel
    list<location> tlocs;
    int tlocsSize;

    // back transformed locations
    std::vector<location> blocs;

    // transformed channel
    channel tchnl;

    // ensure correctness of parameters
    const parameters& par = getParameters();

    int i,j;
    ipoint pt;
    timer chronos;
    float angle,scale;
    float fa,ta,fs,ts,sa,ss;
    std::vector<location>::iterator vit;
//    char buffer[256];

    // Complete interval check
    const float firstAngle= (abs(par.firstAngle)> constants<float>::twoPi()) ?
      degToRad(par.firstAngle) : par.firstAngle;
    const float lastAngle = (abs(par.lastAngle) > constants<float>::twoPi()) ?
      degToRad(par.lastAngle) : par.lastAngle;
    const float stepAngle = (abs(par.stepAngle) > constants<float>::twoPi()) ?
      degToRad(par.stepAngle) : par.stepAngle;

    minmax(firstAngle,lastAngle,fa,ta);
    minmax(par.firstScaling,par.lastScaling,fs,ts);

    sa = (stepAngle > 0) ? stepAngle : -stepAngle;
    ss = (par.stepScaling > 1) ? par.stepScaling : 1.0f/par.stepScaling;

    // avoid infinite loops forcing sa to be different than zero
    if (sa == 0.0f) {
      sa = (fa == ta) ? 1.0 : (ta-fa);
    }

    // avoid infinite loops forcing ss to be different than one
    if (ss == 1.0f) {
      ss = (fs == ts) ? 2.0f : ts/fs;
    }
   
    // compute the locations for the original image
    // (the parameters were already set by the caller method)
    int totalLocs;
    list<location> olocsList;

    chronos.start();
    detector.apply(chnl,olocsList,totalLocs);
    chronos.stop();

    if (olocsList.empty()) {
      // no locations detected... this is a very bad parameterization
      fitness.fill(0.0);
      return false;
    }

    // convert the location list into a location vector.
    olocs.clear();
    olocs.reserve(totalLocs);

    float minScale = olocsList.front().radius;
    float maxScale = minScale;

    list<location>::const_iterator lit;
    const list<location>::const_iterator eit=olocsList.end();

    if (mask.empty()) {
      // mask empty => consider everything!
      
      // for each location
      for (lit = olocsList.begin();lit!=eit;++lit) {
        // insert all elements of that level in the "original locs" list
        olocs.push_back(*lit);

        minmax((*lit).radius,minScale,maxScale);
      }
    } else {
      // use only those locations within the masked subset!

      // for each location
      for (lit = olocsList.begin();lit!=eit;++lit) {
        pt.castFrom((*lit).position);
        if ((static_cast<uint32>(pt.x)<static_cast<uint32>(mask.columns())) &&
            (static_cast<uint32>(pt.y)<static_cast<uint32>(mask.rows())) &&
            (mask.at(pt) != 0)) {
          // insert location since mask is not zero at the position
          olocs.push_back(*lit);

          minmax((*lit).radius,minScale,maxScale);
        }
      }      
    }

    // To efficiently search for the nearest locations use a kd-tree,
    // where the locations are going to be searched by their position in
    // the coordinates of the original image.
    // the data corresponds to the index of the location in the vector of
    // locations
    kdTree<fpoint,int> otree;
    const int olocsSize = olocs.size();
    for (i=0;i<olocsSize;++i) {
      otree.add(olocs[i].position,i);
    }

    otree.build(32); // build tree using a bucket of 32 (more efficient)

    // insert the first statistics in the results vector
    
    // the time (or rather images per second) we add 1us to avoid division by 0
    fitness.at(IdxTime) = 1000000.0/(1.0+chronos.getTime());
    fitness.at(IdxNumberOfLocations) = static_cast<double>(olocsSize);
    fitness.at(IdxLocationsRecip) = 1.0/(1.0+olocsSize);
    fitness.at(IdxOccupiedOctaves) = double(log(maxScale/minScale))/log(2.0);

    // now to the expensive evaluation of rotation and scale invariance...
    location loc;

    int posStable,rotStable;
    int totalPosStable,totalRotStable,totalPosUnstable,totalRotUnstable;
    double total;
    int n;

#if defined _LTI_DEBUG && (_LTI_DEBUG > 2)
    static viewer2D viewo("Original");
    static viewer2D viewt("Transformed");
    static viewer2D viewb("Back-transformed");

    image canvas;
    canvas.castFrom(chnl);
    draw<rgbaPixel> painter;
    painter.use(canvas);
    painter.setColor(rgbaPixel(255,128,64));
    for (int ii=0;ii<olocsSize;++ii) {
      painter.set(olocs[ii],true);
    }

    viewo.show(canvas);
#endif

    // sort of an automata with 3 states:
    // State 0: Compute rotation and scale repeatability
    // State 1: Compute scale repeatability only
    // State 2: Compute rotation repeatability only
    for (j=0;j<3;++j) {
      if ( (j==0) &&
           !(par.rotationScaleRepeatability || par.rotationScaleStable)) {
        // not necessary to compute the first stuff
        
        // fix the intervals for the next "scale only" test
        fa=ta=0.0f;

        continue;
      }
      if ( (j==1) &&
           !(par.scaleRepeatability || par.scaleStable)) {
        // not necessary to compute the scaling stuff

        // fix the intervals for the next "rotation only" test
        minmax(firstAngle,lastAngle,fa,ta);
        fs=ts=1.0f;

        continue;
      }

      if ( (j==2) &&
           !(par.rotationRepeatability || par.rotationStable)) {
        // not necessary to compute the rotation stuff
        continue;
      }

      totalPosStable = totalPosUnstable = 0;
      totalRotStable = totalRotUnstable = 0;
      n = 0;

      // scale/rotate image
      for (scale=fs;scale<=ts;scale*=ss) {

        const fmatrix s = scalingMatrix(fpoint(scale,scale));

        for (angle=fa;angle<=ta;angle+=sa) {
          // debug info
          _lti_debug3("  scale=" << scale << 
                      " \tangle=" << radToDeg(angle) << std::endl);

          // set desired rotation and scale
          trans.setMatrix(s * rotationMatrix(fpoint(0,0),angle));
          
          // rotate the image itself
          trans.apply(chnl,tchnl);
          
          // create the locations for the transformed channel
          detector.apply(tchnl,tlocs,tlocsSize);
          
          // map the transformed locations back into the original coordinate
          // system, to simplify searching for an equivalent match.
          trans.use(chnl.size());
          
          // transform all new locations to the coordinate system of the
          // original image
          blocs.clear(); 
          blocs.reserve(tlocsSize); // keep capacity!

          for (lit=tlocs.begin();lit!=tlocs.end();++lit) {
            trans.backwards((*lit).position,loc.position);
            loc.angle = (*lit).angle - angle;
            loc.radius = (*lit).radius/scale;
            blocs.push_back(loc);
          }
          
          compare(otree,olocs,blocs,minScale,posStable,rotStable);

#if defined _LTI_DEBUG && (_LTI_DEBUG > 2)          
          std::cerr << "Position stable locs: " << posStable << std::endl;
          std::cerr << "Rotation stable locs: " << rotStable << std::endl;
#endif

          totalPosStable += posStable;
          totalRotStable += rotStable;
          totalPosUnstable += max(0,totalLocs - posStable) +
                              (tlocsSize - posStable);
          totalRotUnstable += max(0,totalLocs - rotStable) +
                              (tlocsSize - rotStable);
          n++; // number of test images until now
#if defined _LTI_DEBUG && (_LTI_DEBUG > 2)
          canvas.castFrom(tchnl);
          painter.set(tlocs,true);
          viewt.show(canvas);
          canvas.castFrom(chnl);
          list<location> listOfLocs;
          std::vector<location>::const_iterator vlit;
          for (vlit=blocs.begin();vlit!=blocs.end();++vlit) {
            listOfLocs.push_back(*vlit);
          }
          painter.set(listOfLocs,true);
          viewb.show(canvas);

          viewer2D::interaction ia;
          _lti_if_debug3(viewb.waitKeyPressed(ia));
#endif

        }
      }

      switch(j) {  // do different things depending on the state
        case 0: 
          // State: Rotation and Scale variations:
          
          total = double(totalPosStable+totalPosUnstable);
          if (total == 0.0) {
            total = 1.0; // avoid division by 0
          }
          fitness.at(IdxRSPosRep) = double(totalPosStable)/total;
          // mean over n test images:
          fitness.at(IdxRSPosStable) = double(totalPosStable)/n; 
          
          total = double(totalRotStable+totalRotUnstable);
          if (total == 0.0) {
            total = 1.0; // avoid division by 0
          }
          fitness.at(IdxRSARep) = double(totalRotStable)/total;
          fitness.at(IdxRSAStable) = double(totalRotStable)/n;

          // fix the intervals for the next "scale only" text
          fa=ta=0.0f;
          break;
        case 1:
          // State: Scale variations only

          total = double(totalPosStable+totalPosUnstable);
          if (total == 0.0) {
            total = 1.0; // avoid division by 0
          }
          fitness.at(IdxScalePosRep) = double(totalPosStable)/total;
          fitness.at(IdxScalePosStable) = double(totalPosStable)/n;

          total = double(totalRotStable+totalRotUnstable);
          if (total == 0.0) {
            total = 1.0; // avoid division by 0
          }
          fitness.at(IdxScaleARep) = double(totalRotStable)/total;
          fitness.at(IdxScaleAStable) = double(totalRotStable)/n;

          // fix the intervals for the next "rotation only" test
          minmax(firstAngle,lastAngle,fa,ta);
          fs=ts=1.0f;
          break;
        case 2:
          // State: Rotation variations only

          total = double(totalPosStable+totalPosUnstable);
          if (total == 0.0) {
            total = 1.0; // avoid division by 0
          }
          fitness.at(IdxRotationPosRep) = double(totalPosStable)/total;
          fitness.at(IdxRotationPosStable) = double(totalPosStable)/n;

          total = double(totalRotStable+totalRotUnstable);
          if (total == 0.0) {
            total = 1.0; // avoid division by 0
          }
          fitness.at(IdxRotationARep)    = double(totalRotStable)/total;
          fitness.at(IdxRotationAStable) = double(totalRotStable)/n;

          break;
        default:
          assert(false); // we shouldn't come here at all!
          break;
      }
    }
    
    return true;
  }

  bool locationDetectionGenetics::compare(const kdTree<fpoint,int>& otree,
                                          const std::vector<location>& olocs,
                                          const std::vector<location>& blocs,
                                          const float minScale,
                                          int& posStable,
                                          int& rotStable) const {
    
    const parameters& par = getParameters();

    // degrees or radians?
    const float angleTolerance =
      (par.angleTolerance > constants<float>::twoPi()) ?
      degToRad(par.angleTolerance) : par.angleTolerance;

    // scaleTolerance has to be less than 1, ensure it even if told, to avoid
    // hours of search for unexistent bugs
    const float scaleTolerance = 
      (par.scaleTolerance > 1.0) ? 
      1.0f/par.scaleTolerance : par.scaleTolerance;

    typedef kdTree<fpoint,int>::mmap_type mmap_type;

    mmap_type candidates;
    mmap_type::iterator mit;

    unsigned int ui;

    float minRad,maxRad;
    bool match;
    posStable = rotStable = 0;
    const unsigned int bsize = blocs.size();

    vector<ubyte> oflags(olocs.size(),ubyte(1));

    float smallestAngularError;
    int bestMatch;

    // for each element of the back-transformed locations
    for (ui=0;ui<bsize;++ui) {
      otree.searchWithin(blocs[ui].position,
                         sqr(par.positionTolerance*
                             (blocs[ui].radius/minScale)),
                         candidates);

      // from all candidates, we need only one!
      mit=candidates.begin();
      match=false;
      smallestAngularError = constants<float>::twoPi();
      bestMatch = -1;
      while (mit!=candidates.end()) {
        // the elements in candidates are all within the positional tolerance
        // we need only to check if the radius is good enough.
        if (oflags[(*mit).second->data] == 0) {
          // location already used
          ++mit;
          continue;
        }
        minmax(blocs[ui].radius,olocs[(*mit).second->data].radius,
               minRad,maxRad);

        if (minRad >= maxRad*scaleTolerance) {
          match=true;
          // check if also the angles are all right
          minRad = abs(blocs[ui].angle - olocs[(*mit).second->data].angle);
          if (minRad > static_cast<float>(Pi)) {
            minRad = constants<float>::twoPi() - minRad;
          }

          if (minRad < smallestAngularError) {
            smallestAngularError = minRad;
            bestMatch = (*mit).second->data;
          }
        }
        ++mit;
      }

      if (match) {
        posStable++;
        oflags[bestMatch] = 0; // mark best match as used        
      }

      if (smallestAngularError <= angleTolerance) {
        rotStable++;
      }
    }

    return true;
  }

  /*
   * Construct the noisy image name from the original image name
   */
  std::string 
  locationDetectionGenetics::getMaskName(const std::string& imgName,
                                         const std::string& postfix) const {
    
    std::string file(imgName);
    std::string::size_type pos = file.rfind('.');
    if (pos != std::string::npos) {
      file.insert(pos,postfix);
    } else {
      file+=postfix;
    }
    return file;
  }
}
