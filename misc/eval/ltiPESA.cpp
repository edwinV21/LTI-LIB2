
/* -------------------------------------------------------------------
 * project ....: LTI-Lib: Image Processing and Computer Vision Library
 * file .......: ltiPESA.cpp
 * authors ....: Edwin Vasquez
 * organization: Tecnologico de Costa Rica
 * creation ...: 01.09.2017
 * revisions ..:
 */

#include <iostream>
#include "ltiPESA.h"
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



/*
* Default Constructor
*
*/

PESA::PESA() {

}



/*
  * Destructor
  */
 PESA::~PESA() {
 }





 /**
  * Copy constructor
  * @param other the parameters object to be copied
  */
 PESA::PESA(const PESA& other) : geneticEngine()
    {
   copy(other);
 }
 /**
  * Copy data of "other" functor.
  * @param other the functor to be copied
  * @return a reference to this functor object
  */
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

 /**
  * Returns a pointer to a clone of this functor.
  */

 PESA* PESA::clone() const {
   return new PESA(*this);
 }

 /**
  * Returns a pointer to a clone of this functor.
  */

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
    std::cout<< "getting parameters PESA"   <<  "\n";

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


 const double* PESA::expLUT_ = 0;

 bool PESA::initExpLUT() {
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

 /**
  * An efficient way to compute g(x)=exp(-x^2/2)
  */
 inline double PESA::exp2(const double& x) const {
   // 3 is the well known factor: above 3*sigma the Gaussian
   // is negligible.
   const double ax = abs(x);
   return (ax < 3.0) ? expLUT_[static_cast<int>(ax*1024)] : 0.0;
 }

 /**
  * Compute the fitness distance between the given two fitness points
  */
 inline double PESA::fitnessDistance(const lti::dvector& a,
                                            const lti::dvector& b)  {
   double res = 1.0;
   int i;
   const int size = min(a.size(),sigmas_.size());
   for (i=0;(res>0.0) && (i<size);++i) {
     res *= PESA::exp2((a[i]-b[i])/sigmas_[i]);
   }
   return res;
 }


 /**
  * Class used to compare individuals in "scanning order"
  */

 struct  PESA::scanLess
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
 /**
  * Initialize the bounding box
  */
 void PESA::initBoundingBox(dmatrix& boundingBox) const {
   const geneticEngine::parameters& par =geneticEngine::getParameters();
   boundingBox.resize(2,par.fitnessSpaceDimensionality);
   // the min initialized with max
   boundingBox.getRow(0).fill(std::numeric_limits<double>::max());
   // the max initialized with min
   boundingBox.getRow(1).fill(-std::numeric_limits<double>::max());
 }


 /*
  * Update bounding box considering the given fitness space point
  */
 bool PESA::updateBoundingBox(const dvector& pnt,
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

 /**
  * Update fitness space subdivision.
  *
  * This initializes the sigmas based on the bounding box contents.
 */

 void PESA::updateFitnessSpaceSubdivision() {
   const geneticEngine::parameters& par = geneticEngine::getParameters();
   // bbox_ is the bounding box with 2xfitSpcDim
   sigmas_.resize(bbox_.columns(),0.0,AllocateOnly); // sigmas with fitness
                                                   // space dimensionality
   int i;
   for (i=0;i<sigmas_.size();++i) {
     sigmas_.at(i)=(bbox_.at(1,i)-bbox_.at(0,i))/(par.fitnessSpacePartition*6.0);
   }
 }

 /**
  * Get data from log
  *
  * If a log file is generated, usually you cannot read the used
  * parameterization.  With this method you will get from the log file the
  * list of parameters and their corresponding fitness values, as if you
  * had used the corresponding apply method.
  *
  * The parameters of the current functor will change without invalidating
  * the reference.  Therefore this method is not constant.
  */


 bool PESA::getDataFromLog(const std::string& logFile,
                                  geneticEngine::parameters& params,
                                  std::vector<geneticEngine::individual>& data,
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
         geneticEngine::stringToChromosome(str,indiv.genotype);
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
 }

 /**
 * The log-file has in the comments the iteration number.  We can
 * try to rescue that number from there.
 */

 int PESA::findLastIter(const std::string& logFile) const {
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


 /**
  * Update density factors.
  *
  * Recompute all squeeze factors for the individuals in the
  * external population
  */
 void PESA::updateDensityFactors(std::vector<geneticEngine::individual>& PE) {
   std::vector<geneticEngine::individual>::iterator it,jt;

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





   // The PESA Algorithm
   bool PESA::apply(std::vector<geneticEngine::individual>& PE,const bool initFromLog) {
    // std::cout<<"applying pesa 1! \n";

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
       //queueProcessor_ = geneticEngine::queueProcessor_;
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
     std::cout<<"applying pesa 4! \n";

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

             //std::cout<<"applying pesa 5! \n";

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
           std::cout<<"applying multi-threading" <<"\n";
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
         if ((extPop >= 2) && ( rnd_.rand() < par.crossoverProbability)) {
           // crossover
           int b = binaryTournament(PE);
           while (a == b) { // ups! two identical individuals, get another one.
             b = min(static_cast<int>( rnd_.rand()*extPop),extPop-1);
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
                    //   std::cout<<"mutationRate" <<mutationRate <<"\n";

     } while (true);


     if (logFront_) {
       if (notNull(logOut_)) {
         //std::cout <<"log out no es nulo \n";
        // logOut_->close();
        // delete logOut_;
        // logOut_ = 0;
       }
     }

     return true;
   }

   /*
   * initialize the geneticEngine with the necessary variables from the paretoFront
   *
   */
   void PESA::initAlg(dmatrix& pbbox_,dvector& psigmas_ ,univariateContinuousDistribution& prnd_,
      bool& plogEvaluations_, bool& plogFront_ , lispStreamHandler& polsh_,std::ofstream* plogOut_,
        std::list<geneticEngine::individual>& pdeadIndividuals_,
         const double* pexpLUT_ ){
        bbox_=pbbox_;
        sigmas_=psigmas_;
        rnd_=prnd_;
        logEvaluations_=plogEvaluations_;
        logFront_=plogFront_;
        olsh_=polsh_;
        logOut_=plogOut_;
        deadIndividuals_=pdeadIndividuals_;
        expLUT_=pexpLUT_;
      }

   // random initialization
   bool PESA::initInternalPopulation(std::vector<geneticEngine::individual>& data) {
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

   /**
    * Return true if a>b (a dominates b) after the definition used in the
    * Pareto literature:
    *
    * a>b <=> for all i a[i]>=b[i] and it exists one i such that a[i]>b[i]
    *
    * The arguments a and b represent here multidimensional fitness values.
    */
   bool PESA::dominate(const dvector& a,
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


   // binary tournament
   int PESA::binaryTournament(const std::vector<geneticEngine::individual>& PE) const {
     const int size = PE.size();

     if (size <= 1) {
       return 0;
     } else if (size <= 2) {
       if (PE[0].squeezeFactor < PE[1].squeezeFactor) {
         return 0;
       } else if (PE[0].squeezeFactor > PE[1].squeezeFactor) {
         return 1;
       } else {
         return (rnd_.rand() < 0.5) ? 0 : 1;
       }
     }

     // chose two random individuals
     int a = min(static_cast<int>(size*rnd_.rand() ),size-1);
     int b = min(static_cast<int>(size*rnd_.rand()  ),size-1);
     while (b == a) {
       b = min(static_cast<int>(size*rnd_.rand()   ),size-1);
     }

     if (PE[a].squeezeFactor < PE[b].squeezeFactor) {
       return a;
     } else if (PE[a].squeezeFactor > PE[b].squeezeFactor) {
       return b;
     } else {
       return (rnd_.rand() < 0.5) ? a : b;
     }
   }

   // insert one individual into the external population
   bool PESA::insert(geneticEngine::individual& genotype,
                            std::vector<geneticEngine::individual>& PE) {

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



     bool PESA::logEntry(const geneticEngine::individual& ind,const bool markDead) {
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

     int PESA::insert(std::vector<geneticEngine::individual>& PI,
                             std::vector<geneticEngine::individual>& PE) {

       const unsigned int extPopSize =
         static_cast<unsigned int>(geneticEngine::getParameters().externalPopulationSize);

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






}
