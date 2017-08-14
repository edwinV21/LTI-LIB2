
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







 /*bool PESA::apply(std::vector<geneticEngine::individual>& PE,const bool initFromLog)  {

     const geneticEngine::parameters par=geneticEngine::getParameters() ;

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



       }














     return true;



 }*/



   // The PESA Algorithm
   bool PESA::apply(std::vector<paretoFront::individual>& PE,const bool initFromLog) {
     //paretoFront* pf=geneticEngine::pf_;
     std::cout<<"applying pesa! " <<pf_->getParameters().fitnessSpaceDimensionality <<"\n";

     const geneticEngine::parameters& par = geneticEngine::getParameters();
     const genetics* geneticTools = &par.getGeneticsObject();




     if (isNull(geneticTools)) {
       pf_->setStatusString("Not a valid genetics object set yet");
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

     std::vector<paretoFront::individual> PI; // internal population
     vector<ubyte> mtSuccess;    // success flags for multi-threading mode

     PE.clear();

     // if the user desires to watch the evolution progress
     if (pf_->haveValidProgressObject()) {
       pf_->getProgressObject().reset();
       std::string str("Pareto Front Evaluation Test.\n");
       str += "Evaluation class: ";
       str += par.getGeneticsObject().name();
       pf_->getProgressObject().setTitle(str);
       pf_->getProgressObject().setMaxSteps(par.numOfIterations+2);
     }

     // ensure that the PE and PI vectors will have all memory they need
     PE.reserve(par.internalPopulationSize+par.externalPopulationSize+1);


     if (par.numberOfThreads > 1) {
       pf_->queueProcessor_.init();
     }

     int lastIter=0;

     if (initFromLog) {
       // read the whole log and use it as initialization
       // Some output if desired
       if (pf_->haveValidProgressObject()) {
         pf_->getProgressObject().step("Initialization from log file.");
       }
       if (pf_->getDataFromLog(par.logFilename,pf_->getRWParameters(),PI,pf_->bbox_,lastIter)){
         // we need to re-adapt the parameters from the log file
         if (haveValidProgressObject()) {
           pf_->getProgressObject().setMaxSteps(par.numOfIterations+2);
           pf_->getProgressObject().setStep(lastIter);
         }

         // update the genetic tools used
         geneticTools = &par.getGeneticsObject();

         // well, we need to continue logging at the end of the file
         if (notNull(pf_->logOut_)) {
           pf_->logOut_->close();
           delete pf_->logOut_;
           pf_->logOut_=0;
         }
         if (par.logFront) {
           // append at the end of the file!
           pf_->logOut_ = new std::ofstream(par.logFilename.c_str(),
                                       std::ios_base::app);
           pf_->olsh_.use(*pf_->logOut_);
           pf_->logFront_ = false; // avoid rewriting the initialization
         }
       } else {
         if (pf_->haveValidProgressObject()) {
           std::string msg = "Problems reading log file (";
           msg += pf_->getStatusString() + "). Aborting";
           pf_->getProgressObject().step(msg);
         }
         return false;
       }

       // If there are not enough individuals in the internal population
       // create a few more.
       if (static_cast<int>(PI.size()) < par.internalPopulationSize) {
         std::vector<paretoFront::individual> tmpPI;
         tmpPI.reserve(par.internalPopulationSize);

         // Initialization of internal population: create random individuals
         if (!pf_->initInternalPopulation(tmpPI)) {

           // Some output if desired
           if (pf_->haveValidProgressObject()) {
             pf_->getProgressObject().step("Initialization failed.");
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
       if (pf_->haveValidProgressObject()) {
         pf_->getProgressObject().step("Initialization.");
       }

       // Initialization of internal population.
       if (!pf_->initInternalPopulation(PI)) {

         // Some output if desired
         if (pf_->haveValidProgressObject()) {
           pf_->getProgressObject().step("Intialization failed.");
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

     if (pf_->haveValidProgressObject()) {
       chrono.start();
       startTime=chrono.getTime();
       startIteration=pf_->getProgressObject().getStep();
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

             if (pf_->haveValidProgressObject(1)) {
               std::ostringstream oss;
               oss << "Internal evaluation " << i+1 << "/" << PI.size();
               pf_->getProgressObject().substep(1,oss.str());
             }

             // normal algorithm
             if(geneticTools->evaluateChromosome(PI[i].genotype,
                                                 PI[i].fitness,
 						PI[i].genotype)) {
               updateSqueezeFactors = (pf_->updateBoundingBox(PI[i].fitness,pf_->bbox_) ||
                                       updateSqueezeFactors);
             } else {
               // evaluation failed, but we need some dummy fitness:
               // let's make the worst fitness for this one: zero everywhere
               PI[i].fitness.assign(par.fitnessSpaceDimensionality,0.0);
               premortum++;
             }

             if (pf_->haveValidProgressObject(2)) {
               // if the user wants, show the fitness vector
               std::ostringstream oss;
               oss << "Fitness: " << PI[i].fitness;
               pf_->getProgressObject().substep(2,oss.str());
             }
           }
         } else {
           // --------------------------------------------------
           // Multiple thread processing is done through a queue
           // --------------------------------------------------

           pf_->queueProcessor_.evaluate(PI,mtSuccess,*geneticTools);
           for (int i=0;i<mtSuccess.size();++i) {
             if (mtSuccess.at(i) != 0) {
               updateSqueezeFactors = (pf_->updateBoundingBox(PI[i].fitness,pf_->bbox_) ||
                                       updateSqueezeFactors);
             } else {
               premortum++;
             }
           }
         }
       }

       if (premortum >= PI.size()) {
         pf_->appendStatusString("\nAll evaluations in one iteration failed.");

         if (pf_->haveValidProgressObject()) {
           pf_->getProgressObject().step("Error: All evalutations in one iteration" \
                                    " failed. Aborting.");
           pf_->getProgressObject().step(geneticTools->getStatusString());
         }

         return false;
       }

       // If the bounding box changed, we need to recompute the density factors
       if (updateSqueezeFactors) {
         _lti_debug2("Updating Squeeze Factors"<<std::endl);

         pf_->updateFitnessSpaceSubdivision();
         pf_->updateDensityFactors(PE);

         _lti_debug3("Bounding box:\n" << pf_->bbox_ << std::endl);
         _lti_debug3("New sigmas:\n" << pf_->sigmas_ << std::endl);
       }

       // Add non-dominated members from PI to PE
       inserted = pf_->insert(PI,PE);
       extPop = PE.size();

       // Some output if desired
       if (pf_->haveValidProgressObject()) {
         std::ostringstream oss;

         oss << "Front size: " << extPop
             << " \tNew individuals: " << inserted;


         // first, compute the elapsed time since the first iteration in secs
         double t  = (chrono.getTime()-startTime)/1000000.0;

         const int currentStep = pf_->getProgressObject().getStep();

         if (currentStep > startIteration) {

           // estimated remaining time in seconds
           t *= (double(pf_->getProgressObject().getMaxSteps() - currentStep - 1)/
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

         pf_->getProgressObject().step(oss.str());
       }

       // Log which iteration has been currently logged
       if (par.logFront) {
         std::ostringstream oss;
         oss << ";; Iteration: " << iter << "  Front size: " << extPop
             << "  New individuals: " << inserted;
         oss << " (MR: " <<
           mutationRate*geneticTools->getChromosomeSize() << " bits)";
         (*pf_->logOut_) << oss.str() << std::endl;
       }

       // end of analysis?
       if ((++iter >= par.numOfIterations) ||
           (pf_->haveValidProgressObject() &&
            pf_->getProgressObject().breakRequested())) {

         // Some output if desired
         if (pf_->haveValidProgressObject()) {
           if (iter >= par.numOfIterations) {
             pf_->getProgressObject().step("Ready.");
           } else {
             pf_->getProgressObject().step("Stopped by the user.");
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
         pf_->logFront_ = par.logFront; // log if desired
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
         const int a = pf_->binaryTournament(PE);
         if ((extPop >= 2) && ( pf_->rnd_.rand() < par.crossoverProbability)) {
           // crossover
           int b = pf_->binaryTournament(PE);
           while (a == b) { // ups! two identical individuals, get another one.
             b = min(static_cast<int>( pf_->rnd_.rand()*extPop),extPop-1);
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

     if (pf_->logFront_) {
       if (notNull(pf_->logOut_)) {
         pf_->logOut_->close();
         delete pf_->logOut_;
         pf_->logOut_ = 0;
       }
     }

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
