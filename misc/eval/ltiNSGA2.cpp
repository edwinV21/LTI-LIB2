#include <iostream>
#include "ltiNSGA2.h"
#include "ltiFunctor.h"
#include <vector>
#include "ltiProgressInfo.h"
#include "ltiProgressReporter.h"
#include "ltiTimer.h"
#include <limits>
#include <string>
#include <sstream>
#include <algorithm>

#include "ltiRound.h"





#ifdef _LTI_GNUC_2
namespace std {
  typedef ios ios_base;
}
#endif

#undef _LTI_DEBUG
//#define _LTI_DEBUG 2
#include "ltiDebug.h"

namespace lti {


  NSGA2::NSGA2() {
    std::cout <<"printing NSGA2";
  }


  /*
    * Destructor
    */
   NSGA2::~NSGA2() {
   }


   bool NSGA2::apply(std::vector<geneticEngine::individual>& PE,const bool initFromLog) {

     const geneticEngine::parameters& par = geneticEngine::getParameters();
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

     std::vector<geneticEngine::individual> PI; // internal population
     vector<ubyte> mtSuccess;    // success flags for multi-threading mode

     PE.clear();

     //std::cout<<"applying pesa 2! \n";

     // if the user desires to watch the evolution progress
     if (haveValidProgressObject()) {
       std::cout <<"valid progress Object \n";
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
         std::vector<geneticEngine::individual> tmpPI;
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

       std::cout<<"applying pesa 3! \n";

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


     bool initFirstFromLog = initFromLog;

       // First Internal Population Evaluation (PI)

      // updateSqueezeFactors = false;
       //premortum = 0;

       if (initFirstFromLog) {
         // initialization from the log file
        // updateSqueezeFactors = true;
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
              // updateSqueezeFactors = (updateBoundingBox(PI[i].fitness,bbox_) ||
              //                         updateSqueezeFactors);
             } else {
               // evaluation failed, but we need some dummy fitness:
               // let's make the worst fitness for this one: zero everywhere
               PI[i].fitness.assign(par.fitnessSpaceDimensionality,0.0);
              // premortum++;
             }

             if (haveValidProgressObject(2)) {
               // if the user wants, show the fitness vector
               std::ostringstream oss;
               oss << "Fitness: " << PI[i].fitness;
               getProgressObject().substep(2,oss.str());
             }
           }
         }
       }

   std::cout <<"fitness zero individual" << PI[0].fitness.at(0) <<"\n";

   std::vector<std::vector<geneticEngine::individual> > frontiers;

   assign_rank_and_crowding_distance(PI);



   return true;

   }

  void NSGA2::assign_rank_and_crowding_distance(std::vector<geneticEngine::individual>& pop){
    std::cout <<"applying rank and crowd distance! \n";
     int rank=1;

     for (unsigned int i=0;i<PI.size();++i) {

        /*

        We now create each front

        */

         for (unsigned int i=0;i<PI.size();++i) {
           std::vector<geneticEngine::individual> front; 


         }




     }



   }



   // random initialization
   bool NSGA2::initInternalPopulation(std::vector<geneticEngine::individual>& data) {
     const geneticEngine::parameters& par = geneticEngine::getParameters();
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



    NSGA2::NSGA2(const NSGA2& other) : geneticEngine()
       {
      copy(other);
    }

    NSGA2& NSGA2::copy(const NSGA2& other)  {
       //geneticEngine::copy(other);
       // all other attributes are initialized by updateParameters, called when
       // the copy of the parent class sets the parameters.
       return *this;
     }




   /*
     * Return the fully qualified name of this class.
     */
    const std::string& NSGA2::name() const {
      _LTI_RETURN_CLASS_NAME
    }



    NSGA2* NSGA2::clone() const {
      return new NSGA2(*this);
    }



    NSGA2* NSGA2::newInstance() const {
      return new NSGA2();
    }





    NSGA2::parameters::parameters()
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





    NSGA2::parameters::parameters(const parameters& other)
    : functor::parameters(),geneticsObject_(0)
      {

      copy(other);
    }


    NSGA2::parameters&
    NSGA2::parameters::copy(const parameters& other) {
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
    NSGA2::parameters* NSGA2::parameters::newInstance() const {
      return new parameters();
    }


    // clone member
    NSGA2::parameters* NSGA2::parameters::clone() const {
      return new parameters(*this);
    }


    // get type name
    const std::string& NSGA2::parameters::name() const {
      _LTI_RETURN_CLASS_NAME
    }



    // return parameters
    const NSGA2::parameters& NSGA2::getParameters() const {
      const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters()  );
      if(isNull(par)) {
        std::cout<< "is Null IN NSGA2 "   <<  "\n";
        throw invalidParametersException(name());
      }
      return *par;
    }
















}
