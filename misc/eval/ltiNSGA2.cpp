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

          struct  NSGA2::sortByCrowdingDistance
            : public std::binary_function<dvector,dvector,bool> {
            bool operator()(const geneticEngine::individual& a,
                            const geneticEngine::individual& b) const {
              int i=a.fitness.lastIdx();
              for (;i>=0;--i) {
                if (a.squeezeFactor<b.squeezeFactor) {
                  return true;
                } else if (b.squeezeFactor<a.squeezeFactor) {
                  return false;
                }
              }
              // they are equal
              return false;
            }
          };


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

   //std::cout <<"fitness zero individual" << PI[0].fitness.at(0) <<"\n";


  //std::vector<std::vector<geneticEngine::individual> > frontP= fastNonDominatedSort(PI);

/*   for (unsigned int i=0;i<frontP.size();++i) {
     std::cout<<"Frontier NUMBER "<<i <<"\n";
     for (unsigned int j=0;j<frontP.at(i).size();++j) {
       std::cout<<"individual with fitness "<<frontP.at(i).at(j).fitness <<"\n";

     }

   }

   for (unsigned int i=0;i<frontP.size();++i) {
     calculateCrowdingDistance(frontP.at(i) );
   }*/

  /* for (unsigned int i=0;i<frontP.size();++i) {
     std::cout<<"Frontier NUMBER "<<i <<"\n";
     for (unsigned int j=0;j<frontP.at(i).size();++j) {
       std::cout<<"individual with crowding distance "<<frontP.at(i).at(j).squeezeFactor <<"\n";

     }
   }*/

   std::vector<geneticEngine::individual> childPop;

   int numGenerations =0;
   std::cout <<"Num iterations" <<par.numOfIterations <<"\n";




   while(true){
     std::cout<<  "POPULATION IN ITERATION " <<numGenerations << "\n" ;

     for (unsigned i = 0; i < PI.size(); i++) {
        std::cout<<  PI.at(i).fitness  <<"\n";
     }
     selection(PI,mutationRate,childPop);

     for (unsigned int i=0;i<PI.size();++i) {
       // for each individual in the internal pop.

       if (haveValidProgressObject(1)) {
         std::ostringstream oss;
    //     oss << "Internal evaluation " << i+1 << "/" << childPop.size();
      //   getProgressObject().substep(1,oss.str());
       }
       // normal algorithm
       if(geneticTools->evaluateChromosome(childPop[i].genotype,
                                           childPop[i].fitness,
      childPop[i].genotype)) {
        // updateSqueezeFactors = (updateBoundingBox(PI[i].fitness,bbox_) ||
        //                         updateSqueezeFactors);
       } else {
         // evaluation failed, but we need some dummy fitness:
         // let's make the worst fitness for this one: zero everywhere
         childPop[i].fitness.assign(par.fitnessSpaceDimensionality,0.0);
        // premortum++;
       }
       if (haveValidProgressObject(2)) {
         // if the user wants, show the fitness vector
         std::ostringstream oss;
      //   oss << "Fitness: " << childPop[i].fitness;
        // getProgressObject().substep(2,oss.str());
       }
     }

     mergePop(PI,childPop);

     /*for (unsigned int i=0;i<PI.size();++i) {
       std::cout<<"fitness merged "<<PI.at(i).fitness <<"\n";
     }*/

     std::vector<std::vector<geneticEngine::individual> > frontResultant= fastNonDominatedSort(PI);

     std::vector <geneticEngine::individual> nextPop;

     std::cout<<"frontResultant size "<< frontResultant.size() <<"\n";
     for (unsigned int i=0;i<frontResultant.size();++i) {
         std::cout<<"Size in Frontier "<<i << "   "<< frontResultant.at(i).size()   <<"\n";


     }

     for (unsigned int i=0;i<frontResultant.size();++i) {
    //   std::cout<<"Frontier NUMBER "<<i <<"\n";

       if( nextPop.size()+ frontResultant.at(i).size() <=par.internalPopulationSize  ){
        // std::cout<<"isLESS!" <<nextPop.size()+ frontResultant.at(i).size() <<"\n";
         calculateCrowdingDistance(frontResultant.at(i) );
          for (unsigned int j=0;j<frontResultant.at(i).size();++j) {
            nextPop.push_back(frontResultant.at(i).at(j) );
          }
       }
       else{
         calculateCrowdingDistance(frontResultant.at(i));

         //for (unsigned int s=0;s<frontResultant.at(i).size();++s) {
        //   std::cout<<"Before sorting crowding distance "<<frontResultant.at(i).at(s).squeezeFactor <<"\n";
         //}
         std::sort(frontResultant.at(i).begin(),frontResultant.at(i).end(),sortByCrowdingDistance() ) ;

      //   for (unsigned int s=0;s<frontResultant.at(i).size();++s) {
    //       std::cout<<"Sorted crowding distance "<<frontResultant.at(i).at(s).squeezeFactor <<"\n";
    //     }

         int neededIndividuals= par.internalPopulationSize- nextPop.size();

         for (int r=0; r< neededIndividuals; r++   ){
           nextPop.push_back(frontResultant.at(i).at(r) )  ;
         }

         break;
       }

     }
     numGenerations++;

     if(numGenerations>=par.numOfIterations){
       break;
     }
     else{
       PI=nextPop;
     }

   }


   return true;

   }


   void NSGA2::mergePop(std::vector<geneticEngine::individual>& parentPop,
   std::vector<geneticEngine::individual>& childPop){
     for(unsigned int i=0; i<childPop.size() ;i++ ){
       parentPop.push_back(childPop.at(i));
     }

   }


   int NSGA2::binaryTournament(const std::vector<individual>& PE) const{
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

   void NSGA2::selection(std::vector<geneticEngine::individual>& parentPop, int mutationRate,
    std::vector<geneticEngine::individual>& childPop) {
     const geneticEngine::parameters& par = geneticEngine::getParameters();
     const genetics* geneticTools = &par.getGeneticsObject();

     initInternalPopulation(childPop);


     unsigned int j;
     j=0;
     while(j<parentPop.size() ){
       const int a = binaryTournament(parentPop);

       const int b = binaryTournament(parentPop);

       if ( rnd_.rand() < par.crossoverProbability){
        // std::cout<< "applying crossOver \n";
         geneticTools->crossover(parentPop[a].genotype,
                                 parentPop[b].genotype,
                                 childPop[j].genotype,
                                 mutationRate);
          //std::cout<< " crossOverApplied! \n";

       }
       else {
         geneticTools->mutate(parentPop[a].genotype,childPop[j].genotype,mutationRate);
       }
       ++j;
     }





   }





   void NSGA2::calculateCrowdingDistance(std::vector<geneticEngine::individual>& nonDominated){

     if(nonDominated.empty()){
       std::cout<<"error calculating crowding distance \n";
     }

     //for (unsigned int i=0;i<nonDominated.size();++i) {
          //std::cout<< "UNSORTED!"  <<nonDominated.at(i).fitness<<"\n";
    //      nonDominated.at(i).squeezeFactor=0;
    // }

     const geneticEngine::parameters& par = geneticEngine::getParameters();

     for (int i=0;i<par.fitnessSpaceDimensionality;++i) {

       std::sort(nonDominated.begin(),nonDominated.end(),sorter(i)) ;

       //for (unsigned int r=0;r<nonDominated.size();++r) {
            //std::cout<< "SORTED!"  <<nonDominated.at(r).fitness<<"\n";

       //}


       nonDominated.at(0).squeezeFactor = nonDominated.at( nonDominated.size()-1 ).squeezeFactor = std::numeric_limits<int>::max();
       double range = nonDominated.at(nonDominated.size()-1).fitness[i]- nonDominated.at(0).fitness[i];
       //std::cout<<"at zero" << nonDominated.at(0).squeezeFactor;
        for (unsigned int j=1;j<nonDominated.size()-1;++j) {
            double numerator= nonDominated.at(j+1).fitness[i]-  nonDominated.at(j-1).fitness[i];
            nonDominated.at(j).squeezeFactor= nonDominated.at(j).squeezeFactor+numerator/range;
          //  std::cout<< "result squeze! \n";

          //  std::cout<< nonDominated.at(j).squeezeFactor+numerator/range << "\n";
        }

      }


   }

  std::vector<std::vector<geneticEngine::individual> >
  NSGA2::fastNonDominatedSort(std::vector<geneticEngine::individual>& pop){
    std::vector<std::vector<geneticEngine::individual> > frontiers;
    //std::cout <<"applying rank and crowd distance! \n";


     std::vector<geneticEngine::individual> F1;

     std::vector<geneticEngine::individual > Sp;

     for (unsigned int i=0;i<pop.size();++i) {
       pop[i].dominant_count=0;
       pop[i].domination_set.clear();

       for (unsigned int j=0;j<pop.size();++j) {

         if (  dominate( pop[i].fitness,  pop[j].fitness )   ){
              pop[j].id=j;
              pop[i].domination_set.push_back(pop[j]);


         }
         else if (  dominate( pop[j].fitness,  pop[i].fitness )   ){
      //     std::cout<<"j dominates i \n";
           pop[i].dominant_count++;
         }
       }
       Sp.push_back(pop[i]);
    //   std::cout<<"dominant_count!" <<pop[i].dominant_count <<"\n";

    //  std::cout<< "domination Count!" << pop[i].dominant_count   <<"\n";
       if(pop[i].dominant_count==0){
          //for (unsigned int z = 0; z < pop[i].domination_set.size() ; i++) {
            //std::cout<< "dominationSet" << pop[i].domination_set.at(z).fitness   <<"\n";
        //  }
        //  std::cout<< "dominationSet Size" << pop[i].domination_set.size()   <<"\n";

        //  std::cout<< "nonDominated" <<"\n";
        //  std::cout<< pop[i].fitness <<"\n";
          pop[i].rank=0;
          F1.push_back(pop[i]);
       }
     }

     frontiers.push_back(F1);

     int iFirstFront=0;

     while(  true ){
       //std::cout<<"value indexFront!" <<iFirstFront <<"\n";
       std::vector<geneticEngine::individual> newFront;
       //std::cout<< "size first Frontier" <<  frontiers.at(iFirstFront).size()  <<"\n";
       std::vector<geneticEngine::individual> frontTemp = frontiers.at(iFirstFront);

       for (unsigned int i=0;i<frontTemp.size() ;++i   ){
         for (unsigned int j=0;j< frontTemp.at(i).domination_set.size() ;++j   ){
        //   std::cout<<  "dominant_ID!" << frontTemp.at(i).domination_set.at(j).id<< "\n";

          frontTemp.at(i).domination_set.at(j).dominant_count  =Sp.at(frontTemp.at(i).domination_set.at(j).id ).dominant_count;
      //    std::cout<<  "dominant_COUNT!" <<  frontTemp.at(i).domination_set.at(j).dominant_count  << "\n";

           frontTemp.at(i).domination_set.at(j).dominant_count--;
           if(frontTemp.at(i).domination_set.at(j).dominant_count==0){
            // std::cout<<  "Adding to Front! \n";

             frontTemp.at(i).domination_set.at(j).rank=iFirstFront+1;
             newFront.push_back(frontTemp.at(i).domination_set.at(j) );
      //       std::cout<<"PUSHING IN NEW FRONT \n";

           }
           else if (frontTemp.at(i).domination_set.at(j).dominant_count<0) {
             //std::cout<<"ERROR \n";
           }
       }
     }
     if (newFront.empty()){
       break;
     }
     else{
       iFirstFront++;
       frontiers.push_back(newFront);
       for (unsigned int z=0; z<Sp.size() ; z++){
         Sp.at(z).dominant_count--;
       }

     }

   }

   return frontiers;







 }


   bool NSGA2::dominate(const dvector& a,
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





















}
