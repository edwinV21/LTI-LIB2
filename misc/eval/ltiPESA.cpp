
#include <iostream>
#include "ltiPESA.h"
#include "ltiFunctor.h"
#include <vector>
#include "ltiProgressInfo.h"
#include "ltiProgressReporter.h"
#include "ltiTimer.h"




#ifdef _LTI_GNUC_2
namespace std {
  typedef ios ios_base;
}
#endif

#undef _LTI_DEBUG
//#define _LTI_DEBUG 2
#include "ltiDebug.h"

namespace lti {



PESA::PESA() {
  std::cout <<"printing PESA";
}



/*
  * Destructor
  */
 PESA::~PESA() {
 }

 PESA::PESA(const PESA& other) : geneticEngine()
    {
   copy(other);
 }

 PESA& PESA::copy(const PESA& other)  {
    //geneticEngine::copy(other);
    // all other attributes are initialized by updateParameters, called when
    // the copy of the parent class sets the parameters.
    return *this;
  }




/*
  * Return the fully qualified name of this class.
  */
 const std::string& PESA::name() const {
   _LTI_RETURN_CLASS_NAME
 }



 PESA* PESA::clone() const {
   return new PESA(*this);
 }



 PESA* PESA::newInstance() const {
   return new PESA();
 }



 PESA::parameters::parameters()
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








 PESA::parameters::parameters(const parameters& other)
 : functor::parameters(),geneticsObject_(0)
   {

   copy(other);
 }


 PESA::parameters&
 PESA::parameters::copy(const parameters& other) {
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
 PESA::parameters* PESA::parameters::newInstance() const {
   return new parameters();
 }


 // clone member
 PESA::parameters* PESA::parameters::clone() const {
   return new parameters(*this);
 }

 // return parameters
 const PESA::parameters& PESA::getParameters() const {
   const parameters* par =
   dynamic_cast<const parameters*>(&functor::getParameters()  );
   if(isNull(par)) {
     std::cout<< "is Null IN PESA "   <<  "\n";
     throw invalidParametersException(name());
   }
   return *par;
 }

 // get type name
 const std::string& PESA::parameters::name() const {
   _LTI_RETURN_CLASS_NAME
 }



 bool PESA::apply(std::vector<geneticEngine::individual>& PE,const bool initFromLog)  {
     std::cout <<"in apply  PESA\n";

     /*const geneticEngine::parameters& par = geneticEngine::getParameters();
     std::cout<<"verifying Parameters in PESA: " <<  par.numOfIterations <<"\n";

     const genetics* geneticTools = &par.getGeneticsObject();

     if (isNull(geneticTools)) {
       setStatusString("Not a valid genetics object set yet");
       return false;
     }


     const double initialMutationRate = (par.initialMutationRate < 0.0) ?
       abs(par.initialMutationRate)/geneticTools->getChromosomeSize() :
       par.initialMutationRate;

     const double finalMutationRate = (par.finalMutationRate < 0.0) ?
       abs(par.finalMutationRate)/geneticTools->getChromosomeSize() :
       par.finalMutationRate;


       // initial value for mutation rate
       double mutationRate = initialMutationRate;

       std::vector<paretoFront::individual> PI; // internal population
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


       PE.reserve(par.internalPopulationSize+par.externalPopulationSize+1);

       if (par.numberOfThreads > 1) {
         //queueProcessor_.init();
       }

       int lastIter=0;

       if (initFromLog) {
         // read the whole log and use it as initialization
         // Some output if desired

         if (haveValidProgressObject()) {
           getProgressObject().step("Initialization from log file.");
         }



       }*/














     return true;



 }



/*
bool PESA::apply(std::vector<lti::paretoFront::individual>& PE,const bool initFromLog,
  lti::paretoFront& pParetoF ){

  const lti::paretoFront::parameters& par = pParetoF.getParameters();


  const lti::genetics* geneticTools = &par.getGeneticsObject();


  if (isNull(geneticTools)) {
    pParetoF.setStatusString("Not a valid genetics object set yet");
    std::cout << "Genetic Tools null  \n" ;

    return false;
  }

  std::cout << "Applying PESA!  \n" ;

  const double initialMutationRate = (par.initialMutationRate < 0.0) ?
    lti::abs(par.initialMutationRate)/geneticTools->getChromosomeSize() :
    par.initialMutationRate;

  const double finalMutationRate = (par.finalMutationRate < 0.0) ?
    lti::abs(par.finalMutationRate)/geneticTools->getChromosomeSize() :
    par.finalMutationRate;

    double mutationRate = initialMutationRate;

    std::vector<lti::paretoFront::individual> PI; // internal population
    std::vector<lti::ubyte> mtSuccess;    // success flags for multi-threading mode
    PE.clear();

    // if the user desires to watch the evolution progress
    if ( pParetoF.haveValidProgressObject()   ) {   //Check herency
       pParetoF.getProgressObject().reset();
      std::string str("Pareto Front Evaluation Test.\n");
      str += "Evaluation class: ";
      str += par.getGeneticsObject().name();
       pParetoF.getProgressObject().setTitle(str);
       pParetoF.getProgressObject().setMaxSteps(par.numOfIterations+2);
    }

    PE.reserve(par.internalPopulationSize+par.externalPopulationSize+1);

    lti::paretoFront::queueProcessing queueProcessor_(pParetoF);

    if (par.numberOfThreads > 1) {
      queueProcessor_.init();
    }

    int lastIter=0;

    if (initFromLog) {
      // read the whole log and use it as initialization
      // Some output if desired
      if ( pParetoF.haveValidProgressObject()  ) {
         pParetoF.getProgressObject().step("Initialization from log file.");
      }

      if (pParetoF.getDataFromLog(par.logFilename,pParetoF.getRWParameters(),PI,pParetoF.bbox_,lastIter)){
        // we need to re-adapt the parameters from the log file
        if (pParetoF.haveValidProgressObject()) {
          pParetoF.getProgressObject().setMaxSteps(par.numOfIterations+2);
          pParetoF.getProgressObject().setStep(lastIter);
        }

        // update the genetic tools used
        geneticTools = &par.getGeneticsObject();

        // well, we need to continue logging at the end of the file
        if (lti::notNull(pParetoF.logOut_)) {
          pParetoF.logOut_->close();
          delete pParetoF.logOut_;
          pParetoF.logOut_=0;
        }
        if (par.logFront) {
          // append at the end of the file!
          pParetoF.logOut_ = new std::ofstream(par.logFilename.c_str(),
                                      std::ios_base::app);
          pParetoF.olsh_.use(*pParetoF.logOut_);
          pParetoF.logFront_ = false; // avoid rewriting the initialization
        }


      }else {
        if (pParetoF.haveValidProgressObject()) {
          std::string msg = "Problems reading log file (";
          msg += pParetoF.getStatusString() + "). Aborting";
          pParetoF.getProgressObject().step(msg);
        }
        return false;
      }
      // If there are not enough individuals in the internal population
      // create a few more.
      if (static_cast<int>(PI.size()) < par.internalPopulationSize) {
        std::vector<lti::paretoFront::individual> tmpPI;
        tmpPI.reserve(par.internalPopulationSize);

        // Initialization of internal population: create random individuals
        if (!pParetoF.initInternalPopulation(tmpPI)) {

          // Some output if desired
          if (pParetoF.haveValidProgressObject()) {
            pParetoF.getProgressObject().step("Initialization failed.");
          }
          return false;
        }

        // copy all new generated elements
        int i;
        for (i=PI.size();i<par.internalPopulationSize;++i) {
          PI.push_back(tmpPI[i]);
        }

      }
    }  // end of init from log

    else {
      // normal initialization

      PI.reserve(par.internalPopulationSize);

      // Some output if desired
      if (pParetoF.haveValidProgressObject()) {
        pParetoF.getProgressObject().step("Initialization.");
      }

      // Initialization of internal population.
      if (!pParetoF.initInternalPopulation(PI)) {

        // Some output if desired
        if (pParetoF.haveValidProgressObject()) {
          pParetoF.getProgressObject().step("Intialization failed.");
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

    lti::timer chrono(lti::timer::Wall); // timer used to estimate remaining time
    double startTime(0.0);
    int startIteration(0);

    std::cout<< "returning true";





  return true;
}*/

}
