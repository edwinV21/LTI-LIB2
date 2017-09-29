#include <iostream>
#include "ltiGeneticEngine.h"
#include "ltiPESA.h"
#include <string>
#include "ltiFactory.h"

#ifdef _LTI_GNUC_2
namespace std {
  typedef ios ios_base;
}
#endif

#undef _LTI_DEBUG

namespace lti {


   geneticEngine::geneticEngine(): functor(),progressReporter(),queueProcessor_(*this) {

     // create an instance of the parameters with the default values
     parameters defaultParameters;
     // set the default parameters
     setParameters(defaultParameters);

   }

    geneticEngine::~geneticEngine() {
    }



/*
  * Return the fully qualified name of this class.
  */
 const std::string& geneticEngine::name() const {
   _LTI_RETURN_CLASS_NAME
 }


 void geneticEngine::chromosomeToString(const chromosome& genotype,
                                      std::string& str) const {
   str.resize(genotype.size(),'0');
   unsigned int i;
   for (i=0;i<genotype.size();++i) {
     if (genotype[i]) {
       str[i]='1';
     }
   }
 }

 void geneticEngine::stringToChromosome(const std::string& str,
                                      chromosome& genotype) const {
   genotype.resize(str.length());
   unsigned int i;
   for (i=0;i<genotype.size();++i) {
     genotype[i] = (str[i]!='0');
   }
 }



 void geneticEngine::initAlg(dmatrix& pbbox_,dvector& psigmas_ ,univariateContinuousDistribution& prnd_,
    bool& plogEvaluations_, bool& plogFront_ , lispStreamHandler& polsh_,std::ofstream* plogOut_
    , std::list<individual>& pdeadIndividuals_ ,
     const double* expLUT_){
      bbox_=pbbox_;
      sigmas_=psigmas_;
      rnd_=prnd_;
      logEvaluations_=plogEvaluations_;
      logFront_=plogFront_;
      olsh_=polsh_;
      logOut_=plogOut_;
      deadIndividuals_=pdeadIndividuals_;

    }

/*
geneticEngine* geneticEngine::clone() const {
   return new geneticEngine(*this);
 }
*/

/*void geneticEngine::setParetoFront(paretoFront* pPf){
  pf_=pPf;

}*/


 geneticEngine* geneticEngine::newInstance() const {
   return new geneticEngine();
 }

 geneticEngine* geneticEngine::clone() const {
   return new geneticEngine(*this);
 }

 geneticEngine::geneticEngine(const geneticEngine& other)
    :functor(),progressReporter(),queueProcessor_(*this){
   copy(other);
 }

 geneticEngine& geneticEngine::copy(const geneticEngine& other)  {
    //geneticEngine::copy(other);
    // all other attributes are initialized by updateParameters, called when
    // the copy of the parent class sets the parameters.
    return *this;
  }

  bool geneticEngine::getDataFromLog(const std::string& logFile,
                                   parameters& params,
                                   std::vector<individual>& data,
                                   dmatrix& boundingBox,
                                   int& lastIter) const {
    return true;
  }


  geneticEngine::parameters::parameters()
    : functor::parameters() {

    crossoverProbability = double(0.7); //PESA Y NSGA
    initialMutationRate = double(-1); //
    finalMutationRate = double(-1);
    mutationDecayRate = double(33.3808200696);
    externalPopulationSize = int(100);
    internalPopulationSize = int(10);
    fitnessSpaceDimensionality = int(2);
    numOfIterations = int(1000);
    logAllEvaluations = false;
    fitnessSpacePartition = 32;
    sortResult = true;

    numberOfThreads=1;
    geneticsObject_=0;

    logFront = true;
    //engineName = "PESA";

    logFilename = "pareto.log";
    createFrontFile = false;
    frontFile = "pareto.pf";
  }


  // destructor
  geneticEngine::parameters::~parameters() {
    delete geneticsObject_;
    geneticsObject_=0;
  }










  geneticEngine::parameters::parameters(const parameters& other)
  : functor::parameters(),geneticsObject_(0)
    {

    copy(other);
  }


  geneticEngine::parameters&
  geneticEngine::parameters::copy(const parameters& other) {
    functor::parameters::copy(other);

    crossoverProbability       = other.crossoverProbability;
    initialMutationRate        = other.initialMutationRate;
    finalMutationRate          = other.finalMutationRate;
    mutationDecayRate          = other.mutationDecayRate;
    externalPopulationSize     = other.externalPopulationSize;
    internalPopulationSize     = other.internalPopulationSize;
    fitnessSpaceDimensionality = other.fitnessSpaceDimensionality;
    numOfIterations            = other.numOfIterations;
    logAllEvaluations          = other.logAllEvaluations;
    fitnessSpacePartition      = other.fitnessSpacePartition;
    sortResult                 = other.sortResult;

    numberOfThreads = other.numberOfThreads;

    delete geneticsObject_;
    if (notNull(other.geneticsObject_)) {
      geneticsObject_=other.geneticsObject_->clone();
    } else {
      geneticsObject_=0;
    }

    logFront         = other.logFront;
    logFilename      = other.logFilename;

    createFrontFile  = other.createFrontFile;
    frontFile        = other.frontFile;

    randomParams     = other.randomParams;
    return *this;
  }

  // newInstance member
  geneticEngine::parameters* geneticEngine::parameters::newInstance() const {
    return new parameters();
  }


  // clone member
  geneticEngine::parameters* geneticEngine::parameters::clone() const {
    return new parameters(*this);
  }

  // return parameters
  const geneticEngine::parameters& geneticEngine::getParameters() const {
    const parameters* par =
    dynamic_cast<const parameters*>(&functor::getParameters()  );
    //std::cout<< "getting parameters GE"   <<  "\n";

    if(isNull(par)) {
      std::cout<< "is Null in GE"   <<  "\n";
      throw invalidParametersException(name());
    }
    //std::cout<< "before returns"   <<  "\n";

    return *par;
  }

  geneticEngine::parameters& geneticEngine::getRWParameters() {
    parameters* par =
      dynamic_cast<parameters*>(&functor::getRWParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }

  // get type name
  const std::string& geneticEngine::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  bool geneticEngine::apply(std::vector<geneticEngine::individual>& PE,const bool initFromLog){
      std::cout <<"in apply GE \n";
      return true;
  }

  struct  geneticEngine::scanLess
    : public std::binary_function<dvector,dvector,bool> {
    bool operator()(const individual& a,
                    const individual& b) const {
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



  /*
   * read the parameters from the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also read, otherwise only the data block will be read.
   * @return true if read was succeful
   */

  bool geneticEngine::parameters::read(ioHandler& handler,
                                     const bool complete) {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {

      lti::read(handler,"crossoverProbability",crossoverProbability);
      lti::read(handler,"initialMutationRate",initialMutationRate);
      lti::read(handler,"finalMutationRate",finalMutationRate);
      lti::read(handler,"mutationDecayRate",mutationDecayRate);
      lti::read(handler,"externalPopulationSize",externalPopulationSize);
      lti::read(handler,"internalPopulationSize",internalPopulationSize);
      lti::read(handler,"fitnessSpaceDimensionality",
                fitnessSpaceDimensionality);
      lti::read(handler,"numOfIterations",numOfIterations);
      lti::read(handler,"logAllEvaluations",logAllEvaluations);
      lti::read(handler,"fitnessSpacePartition",fitnessSpacePartition);
      lti::read(handler,"sortResult",sortResult);



      lti::read(handler,"numberOfThreads",numberOfThreads);


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

      lti::read(handler,"logFront",logFront);
      lti::read(handler,"logFilename",logFilename);

      lti::read(handler,"createFrontFile",createFrontFile);
      lti::read(handler,"frontFile",frontFile);
      lti::read(handler,"randomParams",randomParams);
    }

    b = b && functor::parameters::read(handler,false);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }


  genetics& geneticEngine::parameters::getGeneticsObject() {
    assert(notNull(geneticsObject_));
    return *geneticsObject_;
  }



  const genetics& geneticEngine::parameters::getGeneticsObject() const {
    assert(notNull(geneticsObject_));
    return *geneticsObject_;
  }




  bool geneticEngine::parameters::setGeneticsObject( genetics* obj) {
    delete geneticsObject_;
    geneticsObject_=obj;
    return (notNull(geneticsObject_));
  }













  // -------------------------------------------------------------------------
  //                      Multiple threads
  // -------------------------------------------------------------------------

  geneticEngine::pfThread::pfThread(geneticEngine& info,
                                  queueProcessing& manager)
    : thread(),sem_(0),stopRQ_(false),tool_(0),somebody_(0),who_(0),
      success_(0),manager_(&manager),info_(&info) {

    _lti_debug2("paretoFront::pfThread constructor\n");
  }

  geneticEngine::pfThread::~pfThread() {
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

  void geneticEngine::pfThread::evaluate(individual& somebody,
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

  void geneticEngine::pfThread::run() {
    // almost infinite loop
    do {
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
    } while(!stopRQ_);
  }







  /*
  queueProcessing functions
  */

  // ---------------
  // queueProcessing
  // ---------------
  geneticEngine::queueProcessing::queueProcessing(geneticEngine& info)
    : sem_(0),info_(&info) {
  }


  geneticEngine::queueProcessing::~queueProcessing() {
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



    void geneticEngine::queueProcessing::setNumberOfThreads(const int threads) {

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

    void geneticEngine::queueProcessing::init() {
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

    void geneticEngine::queueProcessing::evaluate(std::vector<individual>& PI,
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

    void geneticEngine::queueProcessing::report(pfThread* theThread,const int) {
      lock_.lock();
      ready_.push_back(theThread);
      sem_->post();
      lock_.unlock();
    }













}
