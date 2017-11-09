/*
 * Copyright (C) 1998, 1999, 2000, 2001, 2002, 2003
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


/* -------------------------------------------------------------------
 * project ....: LTI-Lib: Image Processing and Computer Vision Library
 * file .......: ltiParetoFront.cpp
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 25.11.2003
 * revisions ..: $Id: ltiParetoFront.cpp,v 1.20 2010-04-07 01:33:53 alvarado Exp $
 */

#include "ltiParetoFront.h"
#include "ltiFactory.h"
#include "ltiProgressInfo.h"
#include "ltiTimer.h"
#include "ltiRound.h"
#include "ltiGeneticEngine.h"
#include "ltiPESA.h"
#include "ltiNSGA2.h"
#include <limits>
#include <string>
#include <sstream>
#include <algorithm>

#ifdef _LTI_GNUC_2
namespace std {
  typedef ios ios_base;
}
#endif

#undef _LTI_DEBUG
//#define _LTI_DEBUG 2
#include "ltiDebug.h"

namespace lti {
  // --------------------------------------------------
  // paretoFront::parameters
  // --------------------------------------------------

  // default constructor
  _LTI_REGISTER_IN_FACTORY(geneticEngine,PESA);
  _LTI_REGISTER_IN_FACTORY(geneticEngine,NSGA2);

  paretoFront::parameters::parameters()
    : functor::parameters() {

  //  internalPopulationSize = int(10);
    //fitnessSpaceDimensionality = int(2);

    geneticsObject_=0;

    engineName = "PESA";
    engineParamFile="pesa.cfg";

    logFilename = "pareto.log";

  }

  // copy constructor
  paretoFront::parameters::parameters(const parameters& other)
    : functor::parameters(),geneticsObject_(0) {

    copy(other);
  }

  // destructor
  paretoFront::parameters::~parameters() {
    delete geneticsObject_;
    geneticsObject_=0;
  }

  // get type name
  const std::string& paretoFront::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // copy member

  paretoFront::parameters&
  paretoFront::parameters::copy(const parameters& other) {
    functor::parameters::copy(other);

  //  internalPopulationSize     = other.internalPopulationSize;
    //fitnessSpaceDimensionality = other.fitnessSpaceDimensionality;
    engineName                 = other.engineName;
    engineParamFile            = other.engineParamFile;

    delete geneticsObject_;
    if (notNull(other.geneticsObject_)) {
      geneticsObject_=other.geneticsObject_->clone();
    } else {
      geneticsObject_=0;
    }

    logFilename      = other.logFilename;


    //randomParams     = other.randomParams;
    return *this;
  }

  // alias for copy member
  paretoFront::parameters&
  paretoFront::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  // clone member
  paretoFront::parameters* paretoFront::parameters::clone() const {
    return new parameters(*this);
  }

  // newInstance member
  paretoFront::parameters* paretoFront::parameters::newInstance() const {
    return new parameters();
  }

  /*
   * write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was succeful
   */
  bool paretoFront::parameters::write(ioHandler& handler,
                                      const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {

    //  lti::write(handler,"internalPopulationSize",internalPopulationSize);
    //  lti::write(handler,"fitnessSpaceDimensionality",
    //                     fitnessSpaceDimensionality);


      // serializing the genetics object is sort of difficult
      if (notNull(geneticsObject_)) {
        // if there is a valid genetics object just save the name of the class
        // and the state of that class
        lti::write(handler,"geneticsObject",geneticsObject_->name());
        lti::write(handler,"geneticsConfig",*geneticsObject_);
      } else {
        lti::write(handler,"geneticsObject","void");
      }

      lti::write(handler,"logFilename",logFilename);

    //  lti::write(handler,"randomParams",randomParams);

    }

    b = b && functor::parameters::write(handler,false);

    if (complete) {
      b = b && handler.writeEnd();
    }

    return b;
  }


  /*
   * read the parameters from the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also read, otherwise only the data block will be read.
   * @return true if read was succeful
   */
  bool paretoFront::parameters::read(ioHandler& handler,
                                     const bool complete) {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {

    //  lti::read(handler,"internalPopulationSize",internalPopulationSize);
    //  lti::read(handler,"fitnessSpaceDimensionality",
    //            fitnessSpaceDimensionality);

      lti::read(handler,"engineName",engineName);
      lti::read(handler,"engineParamFile",engineParamFile);




      std::string str;
      delete geneticsObject_;
      geneticsObject_=0;

      lti::read(handler,"geneticsObject",str);
      if (str!="void") {
        // only if a valid object was stored
        geneticsObject_=
          factory<genetics>::getFactory().newInstance(str);

        if (notNull(geneticsObject_)) {
          lti::read(handler,"geneticsConfig",*geneticsObject_);
        } else {
          str = std::string("factory<genetics> cannot create a ") +
            str;
          handler.setStatusString(str);
          b=false; // indicate an error
        }
      }



      lti::read(handler,"logFilename",logFilename);


    //  lti::read(handler,"randomParams",randomParams);
    }

    b = b && functor::parameters::read(handler,false);

    if (complete) {
      b = b && handler.readEnd();
    }

    //std::cout<< "geneticsObj " << getGeneticsObject().name() <<"\n";

    return b;
  }

  bool paretoFront::parameters::setGeneticsObject(const genetics& obj) {
    delete geneticsObject_;
    geneticsObject_=obj.clone();
    return (notNull(geneticsObject_));
  }

  genetics& paretoFront::parameters::getGeneticsObject() {
    assert(notNull(geneticsObject_));
    return *geneticsObject_;
  }

  genetics& paretoFront::parameters::getGenObject() {
    assert(notNull(geneticsObject_));
    return *geneticsObject_;
  }

  const genetics& paretoFront::parameters::getGeneticsObject() const {
    assert(notNull(geneticsObject_));
    return *geneticsObject_;
  }

  // --------------------------------------------------
  // paretoFront static members
  // --------------------------------------------------

  const double* paretoFront::expLUT_ = 0;

  bool paretoFront::initExpLUT() {
    if (isNull(expLUT_)) {
      // A static "singleton" LUT.  The compiler will remove it at the end.
      static const int lutSize = 3*1024;
      static double theLUT[lutSize];
      int i;
      for (i=0;i<lutSize;++i) {
        double fi = static_cast<double>(3.0*i)/lutSize;
        theLUT[i] = exp(-(fi*fi)/2.0);
      }
      expLUT_ = theLUT;
    }

    return true;
  }

  // --------------------------------------------------
  // paretoFront inline private members
  // --------------------------------------------------

  inline double paretoFront::random() const {
    return rnd_.rand();
  }

  /**
   * An efficient way to compute g(x)=exp(-x^2/2)
   */
  inline double paretoFront::exp2(const double& x) const {
    // 3 is the well known factor: above 3*sigma the Gaussian
    // is negligible.
    const double ax = abs(x);
    return (ax < 3.0) ? expLUT_[static_cast<int>(ax*1024)] : 0.0;
  }

  /**
   * Compute the fitness distance between the given two fitness points
   */
  inline double paretoFront::fitnessDistance(const lti::dvector& a,
                                             const lti::dvector& b) const {
    double res = 1.0;
    int i;
    const int size = min(a.size(),sigmas_.size());
    for (i=0;(res>0.0) && (i<size);++i) {
      res *= exp2((a[i]-b[i])/sigmas_[i]);
    }
    return res;
  }


  // --------------------------------------------------
  // paretoFront
  // --------------------------------------------------

  // default constructor
  paretoFront::paretoFront()
    : functor(),progressReporter(),logOut_(0),
      logFront_(false),queueProcessor_(*this) {

    initExpLUT();

    // create an instance of the parameters with the default values
    parameters defaultParameters;
    // set the default parameters
    setParameters(defaultParameters);


  }

  // default constructor
  paretoFront::paretoFront(const parameters& par)
    : functor(),progressReporter(),logOut_(0),
      logFront_(false),queueProcessor_(*this) {

    initExpLUT();

    // set the given parameters
    setParameters(par);
  }


  // copy constructor
  paretoFront::paretoFront(const paretoFront& other)
    : functor(),progressReporter(),logOut_(0),
      logFront_(false),queueProcessor_(*this) {
    copy(other);
  }

  // destructor
  paretoFront::~paretoFront() {
    if (notNull(logOut_)) {
      delete logOut_;
      logOut_ = 0;
    }
  }

  // returns the name of this type
  const std::string& paretoFront::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // copy member
  paretoFront& paretoFront::copy(const paretoFront& other) {
    functor::copy(other);
    progressReporter::copy(other);
    return (*this);
  }

  // alias for copy member
  paretoFront&
  paretoFront::operator=(const paretoFront& other) {
    return (copy(other));
  }

  paretoFront* paretoFront::clone() const {
    return new paretoFront(*this);
  }

  paretoFront* paretoFront::newInstance() const {
    return new paretoFront();
  }


  // return parameters
  const paretoFront::parameters& paretoFront::getParameters() const {
    const parameters* par =
    dynamic_cast<const parameters*>(&functor::getParameters()  );
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }

  bool paretoFront::updateParameters() {
    if (functor::updateParameters()) {
      const parameters& par = getParameters();

      engine_=
        factory<geneticEngine>::getFactory().newInstance(par.engineName);

      //  std::cout<<"pasa 1 \n" ;
        geneticEngine::parameters* pptr=engine_->getParameters().clone();

        std::ifstream in (par.engineParamFile.c_str() );

        lispStreamHandler shandler(in);

        lti::read(shandler,"parameters",*pptr);


        engine_->setParameters(*pptr);

        delete pptr;

        std::vector<individual> PE;
        parameters& parRW = getRWParameters();
        genetics* geneticTools = &parRW.getGeneticsObject();
        engine_->getRWParameters().setGeneticsObject(geneticTools);
        //engine_->setProgressObject(getProgressObject);
        //bool a=engine_->getParameters().setGeneticsObject( geneticTools  );
        std::cout<<"verifying LogFront parameters GE: " <<  engine_->getParameters().getGeneticsObject().name() <<"\n";
        std::cout<<"verifying LogFront parameters PF: " <<  par.getGeneticsObject().name() <<"\n";
        //engine_->setParetoFront(this);
        //engine_->apply(PE,true);


      rnd_.setParameters(engine_->getParameters().randomParams);
      if (engine_->getParameters().numberOfThreads>1) {
        queueProcessor_.setNumberOfThreads(engine_->getParameters().numberOfThreads);
        engine_->queueProcessor_.setNumberOfThreads(  engine_->getParameters().numberOfThreads );
      }

      return true;
    }
    return false;
  }

  // return parameters
  paretoFront::parameters& paretoFront::getRWParameters() {
    parameters* par =
      dynamic_cast<parameters*>(&functor::getRWParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }

  // -------------------------------------------------------------------
  // The apply-methods!
  // -------------------------------------------------------------------

  // privat class used to sort the vectors in scanning order
  struct  paretoFront::scanLess
    : public std::binary_function<dvector,dvector,bool> {
    bool operator()(const geneticEngine::individual& a,
                    const geneticEngine::individual& b) const {
      int i=a.fitness.lastIdx();
      for (;i>=0;--i) {
        if (a.fitness[i]<b.fitness[i]) {
          return true;
        } else if (b.fitness[i]<a.fitness[i]) {
          return false;
        }
      }
      // they are equal
      return false;
    }
  };


  // On place apply for type matrix!
  bool paretoFront::apply(matrix<double>& front) {
    const geneticEngine::parameters& par = engine_->getParameters();

    // some attributes need initialization before calling PESA
    logEvaluations_ = par.logAllEvaluations;
    deadIndividuals_.clear();
    initBoundingBox(bbox_);

    if (!initLog()) {
      return false;
    }

    std::vector<geneticEngine::individual> PE;  // The external population

    engine_->initAlg(bbox_,sigmas_ ,rnd_,logEvaluations_, logFront_ , olsh_, logOut_ ,deadIndividuals_,
     expLUT_);

    engine_->setProgressObject(static_cast<progressInfo&>(getProgressObject() ));


    if (engine_->apply(PE,0)) {

      // convert the PE vector inpadetto the standard output expected by the user
      front.resize(PE.size(),par.fitnessSpaceDimensionality);


      std::vector<functor::parameters*>
        phenotypes(PE.size(),
                   static_cast<functor::parameters*>(0));

      // sort the result if desired.

      if (par.sortResult) {
        std::sort(PE.begin(),PE.end(),scanLess());
      }


      _lti_debug("Pareto Front:\n");
      unsigned int i;
      std::cout<<"saving PE!" <<"\n";
      for (i=0;i<PE.size();++i) {
        front.getRow(i).copy(PE[i].fitness);
        if (par.createFrontFile) {
        //  std::cout<<"editing front file!" <<   <<"\n";
          phenotypes[i] =
            par.getGeneticsObject().chromosomeToPhenotype(PE[i].genotype);
        }
        _lti_debug(i << ": " << PE[i].fitness << " -> squeeze " <<
                   PE[i].squeezeFactor << "\n");
      }



      if (par.createFrontFile) {
        bool b = createFrontFile(par.frontFile,front,phenotypes);
        trash(phenotypes);

        return b;
      }

      _lti_debug("----------THE END-----------" << std::endl);
      return true;
    }
    return false;
  }

  // On copy apply for type matrix!
  bool paretoFront::apply(matrix<double>& front,
                          std::vector<functor::parameters*>& phenotypes) {
    const geneticEngine::parameters& par = engine_->getParameters();

    trash(phenotypes); // ensure that we removed all thing maybe allocated
                       // before

    // some attributes need initialization before calling PESA
    logEvaluations_ = par.logAllEvaluations;
    deadIndividuals_.clear();
    initBoundingBox(bbox_);

    if (!initLog()) {
      return false;
    }

    // An individual has three things:
    //  - fitness (dvector)
    //  - genotype (chromosome -> std::vector<bool>)
    //  - squeezeFactor (double)
    std::vector<geneticEngine::individual> PE;

    engine_->initAlg(bbox_,sigmas_ ,rnd_,logEvaluations_, logFront_ , olsh_, logOut_,deadIndividuals_,expLUT_   );

    engine_->setProgressObject(static_cast<progressInfo&>(getProgressObject()) );
    std::cout << "veriffff in apply 2 \n";

    if (engine_->apply(PE,0)) {

      // convert the PE vector into the standard output expected by the user
      front.resize(PE.size(),par.fitnessSpaceDimensionality);
      phenotypes.resize(PE.size(),static_cast<functor::parameters*>(0));

      // sort the result if desired.
      if (par.sortResult) {
        std::sort(PE.begin(),PE.end(),scanLess());
      }

      unsigned int i;
      for (i=0;i<PE.size();++i) {
        front.getRow(i).copy(PE[i].fitness);
        phenotypes[i] =
          par.getGeneticsObject().chromosomeToPhenotype(PE[i].genotype);
      }

      if (par.createFrontFile) {
        return createFrontFile(par.frontFile,front,phenotypes);
      }

      return true;
    }
    return false;
  }

  // On copy apply for type matrix!
  bool paretoFront::resume(matrix<double>& front,
                           std::vector<functor::parameters*>& phenotypes) {

    const geneticEngine::parameters& par = engine_->getParameters();
    std::cout << "resuming 1 \n";
    trash(phenotypes); // ensure that we removed all thing maybe allocated
                       // before

    // some attributes need initialization before calling PESA
    logEvaluations_ = par.logAllEvaluations;
    deadIndividuals_.clear();
    initBoundingBox(bbox_);

    std::vector<geneticEngine::individual> PE;
    engine_->initAlg(bbox_,sigmas_ ,rnd_,logEvaluations_, logFront_ , olsh_, logOut_,deadIndividuals_,expLUT_  );
    engine_->setProgressObject(static_cast<progressInfo&>(getProgressObject()) );


    if (engine_->apply(PE,true)) { // "true" means initFromLog

      // convert the PE vector into the standard output expected by the user
      front.resize(PE.size(),par.fitnessSpaceDimensionality);
      phenotypes.resize(PE.size(),static_cast<functor::parameters*>(0));

      // sort the result if desired.
      if (par.sortResult) {
        std::sort(PE.begin(),PE.end(),scanLess());
      }

      unsigned int i;
      for (i=0;i<PE.size();++i) {
        front.getRow(i).copy(PE[i].fitness);
        phenotypes[i] =
          par.getGeneticsObject().chromosomeToPhenotype(PE[i].genotype);
      }

      if (par.createFrontFile) {
        return createFrontFile(par.frontFile,front,phenotypes);
      }

      return true;
    }
    return false;
  }

  // On copy apply for type matrix!
  bool paretoFront::resume(matrix<double>& front) {
    std::cout << "resuming 2 \n";
    const geneticEngine::parameters& par = engine_->getParameters();

    // some attributes need initialization before calling PESA
    logEvaluations_ = par.logAllEvaluations;
    deadIndividuals_.clear();
    initBoundingBox(bbox_);

    std::vector<geneticEngine::individual> PE;

    engine_->initAlg(bbox_,sigmas_ ,rnd_,logEvaluations_, logFront_ , olsh_, logOut_,deadIndividuals_,expLUT_   );
    engine_->setProgressObject(static_cast<progressInfo&>(getProgressObject()) );
  //  engine_->set
    if (engine_->apply(PE,true)) { // "true" means initFromLog

      // convert the PE vector into the standard output expected by the user
      front.resize(PE.size(),par.fitnessSpaceDimensionality);

      std::vector<functor::parameters*>
        phenotypes(PE.size(),
                   static_cast<functor::parameters*>(0));

      // sort the result if desired.
      if (par.sortResult) {
        std::sort(PE.begin(),PE.end(),scanLess());
      }

      unsigned int i;
      for (i=0;i<PE.size();++i) {
        front.getRow(i).copy(PE[i].fitness);
        if (par.createFrontFile) {
          phenotypes[i] =
            par.getGeneticsObject().chromosomeToPhenotype(PE[i].genotype);
        }
      }

      if (par.createFrontFile) {
        bool b = createFrontFile(par.frontFile,front,phenotypes);
        trash(phenotypes);
        return b;
      }

      return true;
    }
    return false;
  }

  bool paretoFront::
  createFrontFile(const std::string& filename,
                  const matrix<double>& front,
                  const std::vector<functor::parameters*>& phenotypes) const {

    std::ofstream out(filename.c_str());

    if (out) {
      bool b = true;
      lispStreamHandler lsh(out);

      for (int j=0;j<front.rows();++j) {
        b = b && lsh.writeBegin();
        b = b && front.getRow(j).write(lsh);
        b = b && phenotypes[j]->write(lsh);
        b = b && lsh.writeEnd();
        b = b && lsh.writeEOL();
      }

      b = b && lti::write(lsh,"BoundingBox",bbox_);

      out << std::endl;
      out.close();

      return true;
    }

    return false;
  }

  // remove all elements in the given vector
  bool
  paretoFront::trash(std::vector<functor::parameters*>& phenotypes) const {
    std::vector<functor::parameters*>::iterator it;
    for (it=phenotypes.begin();it!=phenotypes.end();++it) {
      delete (*it);
      (*it) = 0;
    }
    phenotypes.clear();

    return true;
  }

  // random initialization
  bool paretoFront::initInternalPopulation(std::vector<individual>& data) {
    const geneticEngine::parameters& par = engine_->getParameters();
    data.resize(par.internalPopulationSize);

    unsigned int i,abort;
    // if no valid individual can be generated after abortThreshold tries,
    // there is something wrong
    static const unsigned int abortThreshold = 1000000;

    i=0;
    abort=0;
    while ((abort<abortThreshold) && (i<data.size())) {
      // for each individual

      // for each bit
      if (par.getGeneticsObject().initIndividual(i,data[i].genotype)) {
        // only accept valid chromosomes in the initial population
        i++;
        abort = 0;
      }
      else {
        abort++;
      }
    }

    if (abort >= abortThreshold) {
      setStatusString("Too many errors generating an individual.  Aborting.");
      return false;
    }

    return true;
  }

  bool paretoFront::dominate(const dvector& a,
                             const dvector& b) const {
    bool theOne = false;

    // a little pointer arithmetic to accelerate this frequently called
    // function
    dvector::const_iterator aPtr = a.begin();
    dvector::const_iterator bPtr = b.begin();
    const dvector::const_iterator ePtr = a.end();

    while (aPtr != ePtr) {
      if ((*aPtr) < (*bPtr)) {
        // if any elemenCt is smaller => definitively not greater!
        return false;
      } else if ((*aPtr) > (*bPtr)) {
        // only greater if at least one element has been strictly greater
        theOne = true;
      }

      ++aPtr;
      ++bPtr;
    }

    return theOne;
  }


/*  bool paretoFront::pesa(std::vector<individual>& PE,const bool initFromLog) {

  //  std::cout<<"applying pesa! \n";

    const geneticEngine::parameters& par = engine_->getParameters();
    const genetics* geneticTools = &par.getGeneticsObject();




    if (isNull(geneticTools)) {
      setStatusString("Not a valid genetics object set yet");
      return false;
    }
    // set the shadow for the mutation rate
    const double initialMutationRate = (par.initialMutationRate < 0.0) ?
      abs(par.initialMutationRate)/geneticTools->getChromosomeSize() :
      par.initialMutationRate;

    const double finalMutationRate = (par.finalMutationRate < 0.0) ?
      abs(par.finalMutationRate)/geneticTools->getChromosomeSize() :
      par.finalMutationRate;

    // initial value for mutation rate
    double mutationRate = initialMutationRate;

    std::vector<individual> PI; // internal population
    vector<ubyte> mtSuccess;    // success flags for multi-threading mode

    PE.clear();

    // if the user desires to watch the evolution progress
    if (haveValidProgressObject()) {
      getProgressObject().reset();
      std::string str("Pareto Front Evaluation Test.\n");
      str += "Evaluation class: ";
      str += par.getGeneticsObject().name();
      getProgressObject().setTitle(str);
      getProgressObject().setMaxSteps(par.numOfIterations+2);
    }

    // ensure that the PE and PI vectors will have all memory they need
    PE.reserve(par.internalPopulationSize+par.externalPopulationSize+1);


    if (par.numberOfThreads > 1) {
      queueProcessor_.init();
    }

    int lastIter=0;

    if (initFromLog) {
      // read the whole log and use it as initialization
      // Some output if desired
      if (haveValidProgressObject()) {
        getProgressObject().step("Initialization from log file.");
      }
      if (getDataFromLog(par.logFilename,getRWParameters(),PI,bbox_,lastIter)){
        // we need to re-adapt the parameters from the log file
        if (haveValidProgressObject()) {
          getProgressObject().setMaxSteps(par.numOfIterations+2);
          getProgressObject().setStep(lastIter);
        }

        // update the genetic tools used
        geneticTools = &par.getGeneticsObject();

        // well, we need to continue logging at the end of the file
        if (notNull(logOut_)) {
          logOut_->close();
          delete logOut_;
          logOut_=0;
        }
        if (par.logFront) {
          // append at the end of the file!
          logOut_ = new std::ofstream(par.logFilename.c_str(),
                                      std::ios_base::app);
          olsh_.use(*logOut_);
          logFront_ = false; // avoid rewriting the initialization
        }
      } else {
        if (haveValidProgressObject()) {
          std::string msg = "Problems reading log file (";
          msg += getStatusString() + "). Aborting";
          getProgressObject().step(msg);
        }
        return false;
      }

      // If there are not enough individuals in the internal population
      // create a few more.
      if (static_cast<int>(PI.size()) < par.internalPopulationSize) {
        std::vector<individual> tmpPI;
        tmpPI.reserve(par.internalPopulationSize);

        // Initialization of internal population: create random individuals
        if (!initInternalPopulation(tmpPI)) {

          // Some output if desired
          if (haveValidProgressObject()) {
            getProgressObject().step("Initialization failed.");
          }
          return false;
        }

        // copy all new generated elements
        int i;
        for (i=PI.size();i<par.internalPopulationSize;++i) {
          PI.push_back(tmpPI[i]);
        }

      }
    } // end of init from log
    else {
      // normal initialization

      PI.reserve(par.internalPopulationSize);

      // Some output if desired
      if (haveValidProgressObject()) {
        getProgressObject().step("Initialization.");
      }

      // Initialization of internal population.
      if (!initInternalPopulation(PI)) {

        // Some output if desired
        if (haveValidProgressObject()) {
          getProgressObject().step("Intialization failed.");
        }
        return false;
      }
    }

    // -----------------------------------------------------------------
    //                            LET'S EVOLVE!
    // -----------------------------------------------------------------

    int extPop = 0; // number of individuals in the external population
    int iter = lastIter;
    int inserted;
    bool updateSqueezeFactors;
    bool initFirstFromLog = initFromLog;
    unsigned int premortum;

    timer chrono(timer::Wall); // timer used to estimate remaining time
    double startTime(0.0);
    int startIteration(0);

    if (haveValidProgressObject()) {
      chrono.start();
      startTime=chrono.getTime();
      startIteration=getProgressObject().getStep();
    }

    do {

      // Evaluate Internal Population (PI)
      updateSqueezeFactors = false;
      premortum = 0;

      if (initFirstFromLog) {
        // initialization from the log file
        updateSqueezeFactors = true;
      }
      else {

        if (par.numberOfThreads<=1) {
          // ---------------------------------------------
          // Single thread processing is done sequentially
          // ---------------------------------------------
          for (unsigned int i=0;i<PI.size();++i) {
            // for each individual in the internal pop.

            if (haveValidProgressObject(1)) {
              std::ostringstream oss;
              oss << "Internal evaluation " << i+1 << "/" << PI.size();
              getProgressObject().substep(1,oss.str());
            }

            // normal algorithm
            if(geneticTools->evaluateChromosome(PI[i].genotype,
                                                PI[i].fitness,
						PI[i].genotype)) {
              updateSqueezeFactors = (updateBoundingBox(PI[i].fitness,bbox_) ||
                                      updateSqueezeFactors);
            } else {
              // evaluation failed, but we need some dummy fitness:
              // let's make the worst fitness for this one: zero everywhere
              PI[i].fitness.assign(par.fitnessSpaceDimensionality,0.0);
              premortum++;
            }

            if (haveValidProgressObject(2)) {
              // if the user wants, show the fitness vector
              std::ostringstream oss;
              oss << "Fitness: " << PI[i].fitness;
              getProgressObject().substep(2,oss.str());
            }
          }
        } else {
          // --------------------------------------------------
          // Multiple thread processing is done through a queue
          // --------------------------------------------------

          queueProcessor_.evaluate(PI,mtSuccess,*geneticTools);
          for (int i=0;i<mtSuccess.size();++i) {
            if (mtSuccess.at(i) != 0) {
              updateSqueezeFactors = (updateBoundingBox(PI[i].fitness,bbox_) ||
                                      updateSqueezeFactors);
            } else {
              premortum++;
            }
          }
        }
      }

      if (premortum >= PI.size()) {
        appendStatusString("\nAll evaluations in one iteration failed.");

        if (haveValidProgressObject()) {
          getProgressObject().step("Error: All evalutations in one iteration" \
                                   " failed. Aborting.");
          getProgressObject().step(geneticTools->getStatusString());
        }

        return false;
      }

      // If the bounding box changed, we need to recompute the density factors
      if (updateSqueezeFactors) {
        _lti_debug2("Updating Squeeze Factors"<<std::endl);

        updateFitnessSpaceSubdivision();
        updateDensityFactors(PE);

        _lti_debug3("Bounding box:\n" << bbox_ << std::endl);
        _lti_debug3("New sigmas:\n" << sigmas_ << std::endl);
      }

      // Add non-dominated members from PI to PE
      inserted = insert(PI,PE);
      extPop = PE.size();

      // Some output if desired
      if (haveValidProgressObject()) {
        std::ostringstream oss;

        oss << "Front size: " << extPop
            << " \tNew individuals: " << inserted;


        // first, compute the elapsed time since the first iteration in secs
        double t  = (chrono.getTime()-startTime)/1000000.0;

        const int currentStep = getProgressObject().getStep();

        if (currentStep > startIteration) {

          // estimated remaining time in seconds
          t *= (double(getProgressObject().getMaxSteps() - currentStep - 1)/
                double(currentStep - startIteration));

          const int days  = static_cast<int>(t/(60*60*24));
          t -= (days*(60*60*24));
          const int hours = static_cast<int>(t/(60*60));
          t -= (hours*60*60);
          const int mins  = static_cast<int>(t/60);
          t -= mins*60;
          const int secs  = iround(t);

          oss << " \tERT: ";

          if (days > 0) {
            oss << days << "d " << hours << "h";
          } else if (hours > 0) {
            oss << hours << "h " << mins << "m" ;
          } else if (mins > 0) {
            oss << mins << "m " << secs << "s";
          } else if (secs > 0) {
            oss << secs << "s";
          } else if (t>0.0) {
            oss << t << "s";
          }
        }

        getProgressObject().step(oss.str());
      }

      // Log which iteration has been currently logged
      if (par.logFront) {
        std::ostringstream oss;
        oss << ";; Iteration: " << iter << "  Front size: " << extPop
            << "  New individuals: " << inserted;
        oss << " (MR: " <<
          mutationRate*geneticTools->getChromosomeSize() << " bits)";
        (*logOut_) << oss.str() << std::endl;
      }

      // end of analysis?
      if ((++iter >= par.numOfIterations) ||
          (haveValidProgressObject() &&
           getProgressObject().breakRequested())) {

        // Some output if desired
        if (haveValidProgressObject()) {
          if (iter >= par.numOfIterations) {
            getProgressObject().step("Ready.");
          } else {
            getProgressObject().step("Stopped by the user.");
          }
        }
        break;
      }

      // the next iteration should happen normally
      if (initFirstFromLog) {
        // we can have the wrong size for PI here, so let's fix it if necessary
        if (static_cast<int>(PI.size()) != par.internalPopulationSize) {
          PI.resize(par.internalPopulationSize);
        }
        initFirstFromLog = false;
        logFront_ = par.logFront; // log if desired
      }

      // the evaluation of algorithm could set the random number generator in
      // a deterministic state.
      // we hope that reinitializing it with the system time is random enough!
      // crdist.init();

      // --------------------
      // Generate new PI set:
      // --------------------
      int j;
      j=0;
      while (j<par.internalPopulationSize) {
        const int a = binaryTournament(PE);
        if ((extPop >= 2) && (random() < par.crossoverProbability)) {
          // crossover
          int b = binaryTournament(PE);
          while (a == b) { // ups! two identical individuals, get another one.
            b = min(static_cast<int>(random()*extPop),extPop-1);
          }
          _lti_debug3("Crossover squeeze factors: " <<
                      PE[a].squeezeFactor << "," <<
                      PE[b].squeezeFactor << std::endl);
          geneticTools->crossover(PE[a].genotype,
                                  PE[b].genotype,
                                  PI[j].genotype,
                                  mutationRate);
        } else {
          _lti_debug3("Mutation squeeze factor: " <<
                      PE[a].squeezeFactor << std::endl);
          // mutation
          geneticTools->mutate(PE[a].genotype,PI[j].genotype,mutationRate);
        }
        ++j;
      }

      // update the mutation rate for the next time
      mutationRate = ((initialMutationRate-finalMutationRate)*
                      exp(-iter/par.mutationDecayRate)) + finalMutationRate;

    } while (true);

    if (logFront_) {
      if (notNull(logOut_)) {
        logOut_->close();
        delete logOut_;
        logOut_ = 0;
      }
    }

    return true;
  }*/

  // binary tournament
  int paretoFront::binaryTournament(const std::vector<individual>& PE) const {
    const int size = PE.size();

    if (size <= 1) {
      return 0;
    } else if (size <= 2) {
      if (PE[0].squeezeFactor < PE[1].squeezeFactor) {
        return 0;
      } else if (PE[0].squeezeFactor > PE[1].squeezeFactor) {
        return 1;
      } else {
        return (random() < 0.5) ? 0 : 1;
      }
    }

    // chose two random individuals
    int a = min(static_cast<int>(size*random()),size-1);
    int b = min(static_cast<int>(size*random()),size-1);
    while (b == a) {
      b = min(static_cast<int>(size*random()),size-1);
    }

    if (PE[a].squeezeFactor < PE[b].squeezeFactor) {
      return a;
    } else if (PE[a].squeezeFactor > PE[b].squeezeFactor) {
      return b;
    } else {
      return (random() < 0.5) ? a : b;
    }
  }

  // insert one individual into the external population
  bool paretoFront::insert(individual& genotype,
                           std::vector<individual>& PE) {

    std::list<int> removal;
    int freePlaces = 0;
    unsigned int j,i;
    double dist;
    genotype.squeezeFactor = 0.0;

    // Check which individuals in PE are dominated by the genotype,
    // and mark them for removal
    for (j=0;j<PE.size();++j) {
      if (dominate(genotype.fitness,PE[j].fitness)) {
        // the j element in PE needs to be removed.
        removal.push_back(j);
        if (logEvaluations_) {
          deadIndividuals_.push_back(PE[j]);
        }

        if (haveValidProgressObject(3)) {
          // if the user wants, show the recently dead individuals
          std::ostringstream oss;
          oss << "RDIn: " << PE[j].fitness;
          getProgressObject().substep(3,oss.str());
        }

        // for each removed item (j) we need to update the density
        // influences it caused on the other Pareto members:
        for (i=0;i<PE.size();++i) {
          PE[i].squeezeFactor -= fitnessDistance(PE[i].fitness,PE[j].fitness);
        }

        ++freePlaces;
      } else {
        // element j survives.  We can already compute its influence on the
        // new element
        dist = fitnessDistance(PE[j].fitness,genotype.fitness);
        genotype.squeezeFactor += dist;
        PE[j].squeezeFactor += dist;
      }
    }


    // Recycle as much removal places as possible, we can assume that
    // the elements in "removal" are sorted.
    if (freePlaces > 0) {
      std::list<int>::iterator it = removal.begin();
      PE[*it] = genotype;
      freePlaces--;
      ++it;

      // all incomers at place, now we need to really remove the rest
      // elements.
      int lastElem = PE.size()-1;
      std::list<int>::reverse_iterator rit;
      rit=removal.rbegin();

      int newVctSize = PE.size()-freePlaces;

      // shift all unused places to the end of the vector
      while (freePlaces > 0) {
        // get last used value in PE
        if ((*rit) < lastElem) {
          // swap the values (well, just half swap, since one element will be
          //                  removed anyway)
          PE[*it]=PE[lastElem];
          lastElem--;
          it++;
        } else {
          // element already at a position that will be removed
          lastElem--;
          rit++;
        }
        freePlaces--;
      }

      // resize the vector, removing the last elements (std::vector keeps the
      // rest).
      PE.resize(newVctSize);

    } else {
      // no place left on PE, we just append it
      PE.push_back(genotype);
    }

    return true;
  }

  bool paretoFront::logEntry(const individual& ind,const bool markDead) {
    if (logFront_) {
      std::string str;
      // save new incomer in the log if so desired
      olsh_.writeBegin();
      ind.fitness.write(olsh_);
      olsh_.writeDataSeparator();
      chromosomeToString(ind.genotype,str);
      olsh_.write(str);
      olsh_.writeEnd();
      if (markDead) {
        (*logOut_) << " ;; x" ;
      }
      (*logOut_) << std::endl; // force EOL

      logOut_->flush();                   // force to write the file
      return true;
    }
    return false;
  }

  int paretoFront::insert(std::vector<individual>& PI,
                          std::vector<individual>& PE) {

    const unsigned int extPopSize =
      static_cast<unsigned int>(engine_->getParameters().externalPopulationSize);

    // Check which elements of PI are, within PI, non-dominated.
    std::vector<bool> nonDominated(PI.size(),true);
    bool dominated;
    unsigned int i,j;
    int toInsert = PI.size();
    for (i=0;i<PI.size();++i) {
      dominated = false;
      for (j=0;!dominated && (j<PI.size());++j) {
        dominated = dominate(PI[j].fitness,PI[i].fitness);
      }
      nonDominated[i]=!dominated;
      if (dominated) {
        --toInsert;
        if (logEvaluations_) {
          logEntry(PI[i],true);
          deadIndividuals_.push_back(PI[i]);
        }
      }
    }

    // Only the non-Dominated points in PI need to be checked in PE
    // Check if any element of PI is non-dominated from the elements in PE
    for (i=0;i<PI.size();++i) {
      if (nonDominated[i]) {
        dominated = false;

        for (j=0;!dominated && (j<PE.size());++j) {
          dominated = (dominate(PE[j].fitness,PI[i].fitness));
        }
        nonDominated[i]=!dominated;
        if (dominated) {
          --toInsert;
          if (logEvaluations_) {
            logEntry(PI[i],true);
            deadIndividuals_.push_back(PI[i]);
          }
        }
      }
    }

    if (toInsert == 0) {
      // nothing to be done:
      // all new individuals were dominated by someone else
      return toInsert;
    }

    // the points at PI with (nonDominated == true) belong definitively in PE
    for (i=0;i<PI.size();++i) {
      if (nonDominated[i]) {
        // save new incomer in the log if so desired
        logEntry(PI[i]);
        insert(PI[i],PE);

        if (haveValidProgressObject(3)) {
          // if the user wants, show the new individuals
          std::ostringstream oss;
          oss << "NNDC: " << PI[i].fitness;
          getProgressObject().substep(3,oss.str());
        }
      }
    }

    // Now we need to check if the number of elements in the pareto front
    // is too high, and in that case we need to remove the elements with
    // the highest squeeze factors.
    if (PE.size() > extPopSize) {
      // it is indeed too big, check how many elements we need to remove

      // partially sort the elements
      std::nth_element(PE.begin(),PE.begin()+extPopSize,PE.end());

      // The elements to be removed are now at the end.
      // update the distances for the elements that are going to be removed.

      // for each removed item (j) we need to update the density
      // influences it caused on the other Pareto members:
      for (j=extPopSize;j<PE.size();++j) {
        _lti_debug3("Removing element with squeeze factor " <<
                    PE[j].squeezeFactor << std::endl);

        if (haveValidProgressObject(3)) {
          // if the user wants, show the recently dead individuals
          std::ostringstream oss;
          oss << "HDRI: " << PE[j].fitness;
          getProgressObject().substep(3,oss.str());
        }

        for (i=0;i<extPopSize;++i) {
          PE[i].squeezeFactor -= fitnessDistance(PE[i].fitness,PE[j].fitness);
        }
      }

      // remove the elements
      PE.resize(extPopSize);

    }

    return toInsert;
  }

  void paretoFront::initBoundingBox(dmatrix& boundingBox) const {
    const geneticEngine::parameters& par = engine_->getParameters();
    boundingBox.resize(2,par.fitnessSpaceDimensionality);
    // the min initialized with max
    boundingBox.getRow(0).fill(std::numeric_limits<double>::max());
    // the max initialized with min
    boundingBox.getRow(1).fill(-std::numeric_limits<double>::max());
  }

  /*
   * Update bounding box considering the given fitness space point
   */
  bool paretoFront::updateBoundingBox(const dvector& pnt,
                                            dmatrix& boundingBox) const {
    int i;
    bool changed = false;
    const int maxDim = min(boundingBox.columns(),pnt.size());
    for (i=0;i<maxDim;++i) {
      if (pnt.at(i) < boundingBox.at(0,i)) {
        boundingBox.at(0,i)=pnt.at(i);
        changed = true;
      }

      if (pnt.at(i) > boundingBox.at(1,i)) {
        boundingBox.at(1,i)=pnt.at(i);
        changed = true;
      }
    }

    return changed;
  }

  void paretoFront::updateFitnessSpaceSubdivision() {
    const geneticEngine::parameters& par = engine_->getParameters();
    // bbox_ is the bounding box with 2xfitSpcDim
    sigmas_.resize(bbox_.columns(),0.0,AllocateOnly); // sigmas with fitness
                                                    // space dimensionality
    int i;
    for (i=0;i<sigmas_.size();++i) {
      sigmas_.at(i)=(bbox_.at(1,i)-bbox_.at(0,i))/(par.fitnessSpacePartition*6.0);
    }
  }

  void paretoFront::updateDensityFactors(std::vector<individual>& PE) {
    std::vector<individual>::iterator it,jt;

    // clear the squeeze factors
    for (it=PE.begin();it!=PE.end();++it) {
      (*it).squeezeFactor = 0.0;
    }

    double d;

    for (it=PE.begin();it!=PE.end();++it) {
      for (jt=it;jt!=PE.end();++jt) {
        if (it != jt) {
          d = fitnessDistance((*it).fitness,(*jt).fitness);
          (*it).squeezeFactor += d;
          (*jt).squeezeFactor += d;
        }
      }
    }
  }

  // --------------------------------------------------------------------------

  void paretoFront::getAnalyzedBox(matrix<double>& bb) const {
    bb.copy(bbox_);
  }

  void paretoFront::getDominatedIndividuals(matrix<double>& dindiv) const {

    dindiv.resize(deadIndividuals_.size(),
                  engine_->getParameters().fitnessSpaceDimensionality);

    std::list<individual>::const_iterator it;
    int i;
    for (i=0,it=deadIndividuals_.begin();it!=deadIndividuals_.end();++it,++i) {
      dindiv.getRow(i).copy((*it).fitness);
    }
  }

  void paretoFront::chromosomeToString(const chromosome& genotype,
                                       std::string& str) const {
    str.resize(genotype.size(),'0');
    unsigned int i;
    for (i=0;i<genotype.size();++i) {
      if (genotype[i]) {
        str[i]='1';
      }
    }
  }

  void paretoFront::stringToChromosome(const std::string& str,
                                       chromosome& genotype) const {
    genotype.resize(str.length());
    unsigned int i;
    for (i=0;i<genotype.size();++i) {
      genotype[i] = (str[i]!='0');
    }
  }

  // ---------------------------------------------------------------------
  // Log related stuff
  // ---------------------------------------------------------------------

  bool paretoFront::initLog() {
    if (notNull(logOut_)) {
      delete logOut_;
      logOut_=0;
    }


    const geneticEngine::parameters& par = engine_->getParameters();
    logFront_ = par.logFront; // update shadow attribute of parameter

    if (logFront_) {

      logOut_ = new std::ofstream(par.logFilename.c_str());
      if (isNull(logOut_)) {
        setStatusString("Error opening log file.  Check your parameters.");
        return false;
      }

      olsh_.use(*logOut_);

      olsh_.writeComment(std::string("Protocol for ") +
                        par.getGeneticsObject().name());

      par.write(olsh_);
      olsh_.writeComment("Data");
    }

    return true;
  }

/*  bool paretoFront::
  getDataFromLog(const std::string& logFile,
                 matrix<double>& front,
                 std::vector<functor::parameters*>& phenotypes,
                 int& lastIter) {

    dmatrix dummyBbox;
    initBoundingBox(dummyBbox);
    trash(phenotypes);

    std::vector<geneticEngine::individual> PE;
    if (getDataFromLog(logFile,getRWParameters(),PE,dummyBbox,lastIter)) {
      const geneticEngine::parameters& par = engine_->getParameters();

      // convert the PE vector into the standard output expected by the user
      front.resize(PE.size(),par.fitnessSpaceDimensionality);
      phenotypes.resize(PE.size(),static_cast<functor::parameters*>(0));

      // sort the result if desired.
      if (par.sortResult) {
        std::sort(PE.begin(),PE.end(),scanLess());
      }

      unsigned int i;
      for (i=0;i<PE.size();++i) {
        front.getRow(i).copy(PE[i].fitness);
        phenotypes[i] =
          par.getGeneticsObject().chromosomeToPhenotype(PE[i].genotype);
      }

      return true;

    }

    return false;

  }*/

  int paretoFront::findLastIter(const std::string& logFile) const {
    static const std::string pattern(";; Iteration: ");

    std::ifstream in(logFile.c_str()); // open the log for reading
    std::string line,subline;
    std::string::size_type pos;
    int last=0;
    int count=0;
    if (in) { // if it exists
      while(std::getline(in,line)) { // while file ok
        if ((pos=line.find(pattern) != std::string::npos)) { // if log comment
          ++count; // one additional iteration found
          subline=line.substr(pos+pattern.length()-1);
          std::istringstream strstm(subline);
          int tmp;
          strstm >> tmp; // get the iteration from the log
          if (tmp > last) {
            last = tmp;
          }
        }
      }
    }
    return max(last-1,count-2); // last is 0-based, count isn't
                                // very last iteration incomplete, so -1
  }

/*  bool paretoFront::getDataFromLog(const std::string& logFile,
                                   parameters& params,
                                   std::vector<individual>& data,
                                   dmatrix& boundingBox,
                                   int& lastIter) const {
    std::ifstream in(logFile.c_str());
    lastIter=0;
    if (in) {
      lispStreamHandler lsh(in);
      if (params.read(lsh)) {
        data.clear();
        initBoundingBox(boundingBox);
        std::string str;
        bool ok;
        // read data one by one
        while (lsh.tryBegin()) {
          data.push_back(individual());
          individual& indiv = data[data.size()-1];
          ok = indiv.fitness.read(lsh);
          updateBoundingBox(indiv.fitness,boundingBox);
          ok = lsh.readDataSeparator() && ok;
          ok = lsh.read(str) && ok;
          stringToChromosome(str,indiv.genotype);
          ok = lsh.readEnd() && ok;
          if (!ok ||
              (static_cast<int>(indiv.fitness.size()) !=
               params.fitnessSpaceDimensionality) ||
              (static_cast<int>(indiv.genotype.size()) !=
               params.getGeneticsObject().getChromosomeSize())) {
            // wrong element.  Delete it
            data.pop_back();
          }
        }

        in.close(); // ensure the log is closed before the next step
        lastIter = findLastIter(logFile);

        return true;
      }
      setStatusString(lsh.getStatusString());
    }
    return false;
  }*/

  // -------------------------------------------------------------------------
  //                      Multiple threads
  // -------------------------------------------------------------------------

  paretoFront::pfThread::pfThread(paretoFront& info,
                                  queueProcessing& manager)
    : thread(),sem_(0),stopRQ_(false),tool_(0),somebody_(0),who_(0),
      success_(0),manager_(&manager),info_(&info) {

    _lti_debug2("paretoFront::pfThread constructor\n");
  }

  paretoFront::pfThread::~pfThread() {
    _lti_debug2("paretoFront::pfThread destructor\n");

    lock_.lock();
    stopRQ_ = true;
    tool_=0;
    somebody_=0;
    who_ = 0;
    success_=0;
    sem_.post();
    lock_.unlock();
  }

  void paretoFront::pfThread::evaluate(individual& somebody,
                                       ubyte& success,
                                       const genetics& tool,
                                       const int who ) {
    _lti_debug2("  paretoFront::pfThread::evaluate " << who << std::endl);

    lock_.lock();
    somebody_ = &somebody;
    who_      = who;
    tool_     = &tool;
    success_   = &success;
    sem_.post();
    lock_.unlock();
  }

  void paretoFront::pfThread::run() {
    // almost infinite loop
  /*  do {
      sem_.wait();
      _lti_debug2("    paretoFront::pfThread::run() just awaked "<<who_<< "\n");

      if (notNull(tool_)) {
        const parameters& par = info_->getParameters();
        if (tool_->evaluateChromosome(who_,
                                      somebody_->genotype,
                                      somebody_->fitness,
                                      somebody_->genotype)){
          *success_=1;
        } else {
          *success_=0;
          somebody_->fitness.assign(par.fitnessSpaceDimensionality,0.0);
        }

        if (info_->haveValidProgressObject(1)  ) {
          std::ostringstream oss;
          oss << "Internal evaluation " << who_+1 << "/"
              << par.internalPopulationSize;
          info_->getProgressObject().substep(1,oss.str());
        }
        if (info_->haveValidProgressObject(2)) {
          // if the user wants, show the fitness vector
          std::ostringstream oss;
          oss << "Fitness: " << somebody_->fitness;
          info_->getProgressObject().substep(2,oss.str());
        }

        // report to the queueProcessing class that we are ready to get
        // the next processing package
        manager_->report(this,who_);
      }
    } while(!stopRQ_);*/
  }

  // ---------------
  // queueProcessing
  // ---------------
  paretoFront::queueProcessing::queueProcessing(paretoFront& info)
    : sem_(0),info_(&info) {
  }

  paretoFront::queueProcessing::~queueProcessing() {
    lock_.lock();
    ready_.clear();
    lock_.unlock();

    delete sem_;

    for (std::vector<pfThread*>::size_type i=0;
         i<threads_.size();
         ++i) {
      delete threads_[i];
      threads_[i]=0;
    }
    threads_.clear();
  }

  void paretoFront::queueProcessing::setNumberOfThreads(const int threads) {

    _lti_debug2("paretoFront::queueProcessing::setNumberOfThreads to " <<
                threads);

    if (static_cast<std::vector<pfThread*>::size_type>(threads) >
        threads_.size()) {
      std::vector<pfThread*>::size_type oldSize = threads_.size();
      threads_.resize(threads,0);
      for (std::vector<pfThread*>::size_type i=oldSize;
           i<threads_.size();
           ++i) {
        threads_[i] = new pfThread(*info_,*this);
        threads_[i]->start();
      }
    }
  }

  void paretoFront::queueProcessing::init() {
    // tell all threads to report to me!
    lock_.lock();
    ready_.clear();
    for (std::vector<pfThread*>::size_type i = 0;
         i<threads_.size();
         ++i) {
      ready_.push_back(threads_[i]);
    }

    delete sem_;
    sem_ = new semaphore(threads_.size());
    lock_.unlock();

  }

  void paretoFront::queueProcessing::evaluate(std::vector<individual>& PI,
                                              vector<ubyte>& success,
                                              const genetics& tool) {
    success.assign(PI.size(),255); // 255 indicates nothing done yet!

    /**
     * List of elements still to be evaluated
     */
    std::list<int> toDo;
    for (int i=0;i<static_cast<int>(PI.size());++i) {
      toDo.push_back(i);
    }

    while(!toDo.empty()) {
      lock_.lock();
      int next = toDo.front();
      toDo.pop_front();
      lock_.unlock();

      _lti_debug2("queueProcessing::evaluate waiting...\n");

      sem_->wait(); // wait for the next processor to be ready

      _lti_debug2("queueProcessing::evaluate enough waiting...\n");

      lock_.lock();
      pfThread* cpu = ready_.front();
      ready_.pop_front();
      lock_.unlock();

      // this is done in a separate thread, which will insert itself into the
      // ready_ list when finished
      cpu->evaluate(PI[next],success[next],tool,next);
    }

    // wait until ALL busy threads are ready!
    lock_.lock();
    while (ready_.size() < threads_.size()) {
      lock_.unlock();
      sem_->wait();
      lock_.lock();
    }

    sem_->reset();
    lock_.unlock();
  }

  void paretoFront::queueProcessing::report(pfThread* theThread,const int) {
    lock_.lock();
    ready_.push_back(theThread);
    sem_->post();
    lock_.unlock();
  }

}
