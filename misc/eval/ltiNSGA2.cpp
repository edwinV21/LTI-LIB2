
/*--------------------------------------------------------------------
 * project ....: LTI-Lib: Image Processing and Computer Vision Library
 * file .......: ltiNSGA2.cpp
 * authors ....: Edwin Vasquez
 * organization: LTI, Tecnologico de Costa Rica
 * creation ...: 01.09.2017
 * revisions ..:
 */
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
//calculateCrowdingDistance(PI);
#include <unistd.h>



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
 */
NSGA2::NSGA2() {
}
/*
 * Destructor
 */
NSGA2::~NSGA2() {
}

/**
 * Copy constructor
 * @param other the parameters object to be copied
 */
NSGA2::NSGA2(const NSGA2& other) : geneticEngine()
{
        copy(other);
}

/**
 * Copy data of "other" functor.
 * @param other the functor to be copied
 * @return a reference to this functor object
 */
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

/**
 * Returns a pointer to a clone of this functor.
 */

NSGA2* NSGA2::clone() const {
        return new NSGA2(*this);
}
/**
 * Returns a pointer to a clone of the parameters
 */
NSGA2* NSGA2::newInstance() const {
        return new NSGA2();
}


/*
 *
 * Initialize the NSGA2 parameters
 *
 */
NSGA2::parameters::parameters()
        : functor::parameters() {

        crossoverProbability = double(0.7);  //PESA Y NSGA
        initialMutationRate = double(-1);  //
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

/**
 * Structure that sorts a given population by his crowding distance
 */
struct  NSGA2::sortByCrowdingDistance
        : public std::binary_function<dvector,dvector,bool> {
        bool operator()(const geneticEngine::individual& a,
                        const geneticEngine::individual& b) const {
                int i=a.fitness.lastIdx();
                for (; i>=0; --i) {
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


bool NSGA2::getDataFromLog(const std::string& logFile,
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
    //  initBoundingBox(boundingBox);
      std::string str;
      bool ok;
      // read data one by one
      while (lsh.tryBegin()) {
        data.push_back(individual());
        geneticEngine::individual& indiv = data[data.size()-1];
        ok = indiv.fitness.read(lsh);
        //std::cout <<"fitness in file" << indiv.fitness <<"\n";
        //updateBoundingBox(indiv.fitness,boundingBox);
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

int NSGA2::findLastIter(const std::string& logFile) const {
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


bool NSGA2::logEntry(const geneticEngine::individual& ind,const bool markDead) {
  if (logFront_) {
    std::string str;
    // save new incomer in the log if so desired
    olsh_.writeBegin();
    ind.fitness.write(olsh_);
    olsh_.writeDataSeparator();
    geneticEngine::chromosomeToString(ind.genotype,str);
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




/*
 * Apply Method of the genetic Algorithm, this executes the NSGA-II Algorithm
 * with the specified parameters
 * @param PE resultant population
 * @param initFromLog flag that specifies if the execution should be resumed from a log file
 * @return return true if the algorithm was executed correctly
 *
 */
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

        //const double finalMutationRate = (par.finalMutationRate < 0.0) ?
        //                                 abs(par.finalMutationRate)/geneticTools->getChromosomeSize() :
        //                                 par.finalMutationRate;

        // initial value for mutation rate
        double mutationRate = initialMutationRate;

        std::vector<geneticEngine::individual> PI; // internal population
        vector<ubyte> mtSuccess; // success flags for multi-threading mode

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
      //  PE.reserve(par.internalPopulationSize+par.externalPopulationSize+1);


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
                if (getDataFromLog(par.logFilename,getRWParameters(),PI,bbox_,lastIter)) {
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
                //std::cout <<"PI SIZEEE!" <<PI.size();
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
                        for (i=PI.size(); i<par.internalPopulationSize; ++i) {
                                PI.push_back(tmpPI[i]);
                        }

                }
                else if (static_cast<int>(PI.size()) > par.internalPopulationSize){
                  std::cout <<"is greater! \n";
                  std::vector<geneticEngine::individual> tmpVect;
                  unsigned int sizePI=PI.size()-1;
                  std::cout <<"size PI!"<< sizePI <<"\n";
                  for(int j=0;j<par.internalPopulationSize;j++){
                    std::cout <<"fitness in sizePi!"<< PI[sizePI].fitness <<"\n";
                    tmpVect.push_back(PI[sizePI]);
                    sizePI--;
                  }
                  PI=tmpVect;


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
                        for (unsigned int i=0; i<PI.size(); ++i) {
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
                else{
                        std::cout<<"applying multi-threading" <<"\n";
                        queueProcessor_.evaluate(PI,mtSuccess,*geneticTools);
                }
        }


        calculateCrowdingDistance(PI);

        std::vector<geneticEngine::individual> childPop;

        int iter = 0;
        std::cout<<"iter" <<iter <<"\n";
        std::cout <<"Num iterations" <<par.numOfIterations <<"\n";

        // for (unsigned i = 0; i < PI.size(); i++) {
        //        std::cout<<  PI[i].fitness  <<"\n";
        //}

        timer chrono(timer::Wall); // timer used to estimate remaining time
        double startTime(0.0);
        int startIteration(0);
        std::cout<<"applying NSGA 2 start iteration"<<  startIteration <<"\n";

        if (haveValidProgressObject()) {
                chrono.start();
                startTime=chrono.getTime();
                startIteration=getProgressObject().getStep();
        }
        std::vector<std::vector<geneticEngine::individual> > frontResultant;

        while(true) {
                std::cout<<  "POPULATION IN ITERATION " <<iter << "\n";
                std::cout<<  "SIZE " <<PI.size() << "\n";

                //for (unsigned i = 0; i < PI.size(); i++) {
                      //std::cout<<  PI[i].squeezeFactor  <<"\n";

                //        std::cout<<  PI[i].fitness  <<"\n";

                //}

                // mutationRate = ((initialMutationRate-finalMutationRate)) + finalMutationRate;

                selection(PI,mutationRate,childPop,par.internalPopulationSize);
		
 		   
		

                for (unsigned int i=0; i<childPop.size(); ++i) {
                        // for each individual in the internal pop.
                        if (par.numberOfThreads<=1) {

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
                        else{
                                queueProcessor_.evaluate(childPop,mtSuccess,*geneticTools);

                        }
                }

		//for (unsigned int i=0;i<childPop.size();++i) {
                 //    std::cout<<"child "<<childPop[i].fitness <<"\n";

//                   }
                //PI.insert( PI.begin(),childPop.begin(),childPop.end()   );

                mergePop(PI,childPop);
                std::cout <<"after merged!" <<  PI.size()<<"\n";

                   //for (unsigned int i=0;i<PI.size();++i) {
                    // std::cout<<"fitness "<<PI[i].fitness <<"\n";
                     //std::cout<<"squeezeFactor "<<PI[i].squeezeFactor <<"\n";

                   //}

                frontResultant.clear();
                fastNonDominatedSort(PI,frontResultant);

                //std::cout <<"after fas non-dominant!" <<"\n";



                std::vector <geneticEngine::individual> nextPop;

                /*  std::cout<<"frontResultant size "<< frontResultant.size() <<"\n";
                   for (unsigned int i=0; i<frontResultant.size(); ++i) {
                          std::cout<<"Size in Frontier "<<i << "   "<< frontResultant.at(i).size()   <<"\n";


                   }*/
                //std::cout<<  "after fastNonDominatedSort "  << "\n";

                for (unsigned int i=0; i<frontResultant.size(); ++i) {
                        //   std::cout<<"Frontier NUMBER "<<i <<"\n";
			//std::cout<<"nextPop Size!" <<nextPop.size();
			
                        if( nextPop.size()+ frontResultant.at(i).size() <=(unsigned int)par.internalPopulationSize  ) {

                                // std::cout<<"isLESS!" <<nextPop.size()+ frontResultant.at(i).size() <<"\n";
                                //std::cout<<"isLESS!"<<"\n";

                                calculateCrowdingDistance(frontResultant.at(i) );
                                for (unsigned int j=0; j<frontResultant.at(i).size(); ++j) {
                                        //std::cout<<"frontResultant fitness" <<frontResultant[i][j].fitness   <<"\n";

                                        nextPop.push_back(frontResultant.at(i).at(j) );
                                                std::cout<<  frontResultant.at(i).at(j).fitness  <<"\n";

                                        logEntry(frontResultant.at(i).at(j) );
                                }
                                /*  for (unsigned int r=0;r<nextPop.size();++r) {
                                      std::cout<<"nextPop fitness" << nextPop[r].fitness   <<"\n";
                                   }*/

                        }
                        else{
                                //    std::cout<<"is More!"<<"\n";
                                calculateCrowdingDistance(frontResultant.at(i));

                                //for (unsigned int s=0;s<frontResultant.at(i).size();++s) {
                                //   std::cout<<"Before sorting crowding distance "<<frontResultant.at(i).at(s).squeezeFactor <<"\n";
                                //}
                                std::sort(frontResultant.at(i).begin(),frontResultant.at(i).end(),sortByCrowdingDistance() );

                                //   for (unsigned int s=0;s<frontResultant.at(i).size();++s) {
                                //       std::cout<<"Sorted crowding distance "<<frontResultant.at(i).at(s).squeezeFactor <<"\n";
                                //     }

                                int neededIndividuals= par.internalPopulationSize- nextPop.size();

                                for (int r=0; r< neededIndividuals; r++   ) {
                                        nextPop.push_back(frontResultant.at(i).at(r) );
                                        std::cout<<  frontResultant.at(i).at(r).fitness  <<"\n";
                                        logEntry(frontResultant.at(i).at(r) );
                                }

                                break;
                        }

                }
                // Some output if desired
		            //std::cout<<"Before ERT!"<< "\n";
                if (haveValidProgressObject()) {
                        std::ostringstream oss;

                        oss << "Front size: " << nextPop.size()  ;
                        //    << " \tNew individuals: " << inserted;


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
                                        oss << hours << "h " << mins << "m";
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
                  oss << ";; Iteration: " << iter << "  Front size: " << nextPop.size()
                      << "  New individuals: " << nextPop.size();
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
                        PE.clear();
                        PE=nextPop ;
                        for (unsigned int i = 0; i < PE.size(); i++) {
                          std::cout<< "Final Pop: " <<PE[i].fitness <<"\n";
			 // PE.push_back( nextPop[i] );
                        }
                        break;
                }
                else{
		       //PE.push_back( nextPop[0] );
		       //PE.push_back( nextPop[1] );
                        //mutationRate = ((initialMutationRate-finalMutationRate)*
                        //exp(-iter/par.mutationDecayRate)) + finalMutationRate;
			                    std::cout<<"mutation rate"<<mutationRate <<"\n";
			childPop.clear();
                        PI=nextPop;
                }

        }


        return true;

}
/*
 * Merges the child Population with the parent population
 * and verifies if there are duplicated members
 *
 * @param parentPop parent Population and the resultant population of the merge
 * @param childPop child population that will be merged
 *
 */

void NSGA2::mergePop(std::vector<geneticEngine::individual>& parentPop,
                     std::vector<geneticEngine::individual>& childPop){
        int isInparent=0;
        for(unsigned int i=0; i<childPop.size(); i++ ) {

                for(unsigned int j=0; j<parentPop.size(); j++ ) {
                        if(childPop[i].fitness==parentPop[j].fitness) {
                                isInparent=1;

                        }
                }
                if (isInparent==0) {
                        parentPop.push_back( childPop[i] );
			isInparent=0;

                }
                else{
			//std::cout<<"is In parent!" <<"\n";
                        isInparent=0;
                }

        }







}

/**
 * Returns a random individual in the given population, which has
 * been selected because it had a smaller squeeze factor in a binary
 * tournament.
 */

int NSGA2::binaryTournament(const std::vector<individual>& PE) const {
        const int size = PE.size();

        if (size <= 1) {
                return 0;
        } else if (size <= 2) {
                if (PE[0].squeezeFactor > PE[1].squeezeFactor) {
                        return 0;
                } else if (PE[0].squeezeFactor < PE[1].squeezeFactor) {
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

        //std::cout <<"A"<< PE[a].squeezeFactor << "\n";
        //std::cout <<"B"<< PE[b].squeezeFactor << "\n";;

        if (PE[a].squeezeFactor > PE[b].squeezeFactor) {
                return a;
        } else if (PE[a].squeezeFactor < PE[b].squeezeFactor) {
                return b;
        } else {
                return (rnd_.rand() < 0.5) ? a : b;
        }

}

/*
 * Returns a child population by making a crossover or mutation
 **of a given population
 * @param parentPop parent Population
 * @param mutationRate rate that specifies how much the new population should be mutated
 * @param childPop resultant population
 *
 */

void NSGA2::selection(std::vector<geneticEngine::individual>& parentPop, double mutationRate,
                      std::vector<geneticEngine::individual>& childPop, int pSize) {
        const geneticEngine::parameters& par = geneticEngine::getParameters();
        const genetics* geneticTools = &par.getGeneticsObject();

        initInternalPopulation(childPop);


        int j;
        const int extPopSize =pSize;


        j=0;
        while(j<pSize ) {
                const int a = binaryTournament(parentPop);

                int b = binaryTournament(parentPop);
		//std::cout<<"making bin tournament" <<"\n";

                //   std::cout<<"makin childs!" <<   a   <<"\n";
                //   std::cout<<"makin childs!" <<   b   <<"\n";


                while (a == b) { // ups! two identical individuals, get another one.
                        //     std::cout<<"identical! \n";
                        b = min(static_cast<int>( rnd_.rand()*extPopSize),extPopSize-1);
                }

                if ( rnd_.rand() < par.crossoverProbability) {
                        // std::cout<< "applying crossOver \n";
                        geneticTools->crossover(parentPop[a].genotype,
                                                parentPop[b].genotype,
                                                childPop[j].genotype,
                                                mutationRate);

                        //  std::cout<< " crossOverApplied! \n";
                        //  std::cout <<  "child fitness"   << childPop[j].fitness << "\n";

                }
                else {
                        //   std::cout<< " mutation! \n";
                        //   std::cout <<  "child fitness"   << childPop[j].fitness << "\n";
                        geneticTools->mutate(parentPop[a].genotype,childPop[j].genotype,mutationRate);
                }
                ++j;
        }





}

/**
 * Makes the computation of the crowding distance given a population
 * @param nonDominated population that will be calculated his density
 */


void NSGA2::calculateCrowdingDistance(std::vector<geneticEngine::individual>& nonDominated){

        if(nonDominated.empty()) {
                std::cout<<"error calculating crowding distance \n";
        }

        /*   for (unsigned int i=0;i<nonDominated.size();++i) {
                std::cout<< "UNSORTED!"  <<nonDominated.at(i).fitness<<"\n";
           //      nonDominated.at(i).squeezeFactor=0;
           }*/

        const geneticEngine::parameters& par = geneticEngine::getParameters();

        for (int i=0; i<par.fitnessSpaceDimensionality; ++i) {

                std::sort(nonDominated.begin(),nonDominated.end(),sorter(i));

                //for (unsigned int r=0;r<nonDominated.size();++r) {
                //  std::cout<< "SORTED!"  <<nonDominated.at(r).fitness<<"\n";

//                }


                nonDominated.at(0).squeezeFactor = nonDominated.at( nonDominated.size()-1 ).squeezeFactor = std::numeric_limits<int>::max();
                double range = nonDominated.at(nonDominated.size()-1).fitness[i]- nonDominated.at(0).fitness[i];
                //std::cout<<"at zero" << nonDominated.at(0).squeezeFactor;
                for (unsigned int j=1; j<nonDominated.size()-1; ++j) {
                        double numerator= nonDominated.at(j+1).fitness[i]-  nonDominated.at(j-1).fitness[i];
                        //std::cout<<"initial squeezeFactor \n" <<nonDominated.at(j).squeezeFactor<<"\n";

                        nonDominated.at(j).squeezeFactor= nonDominated.at(j).squeezeFactor+numerator/range;
                      
                }

        }


}

/**
 * Makes the computation of the Fast Non Dominant Sort Algorithm
 * @param pop population to be applied this algorithm
 * @param frontResultant resultant fronts that contains the best individuals
 * of the population
 */

void
NSGA2::fastNonDominatedSort(std::vector<geneticEngine::individual>& pop,
                            std::vector<std::vector<geneticEngine::individual> >& frontiers){
        //std::cout <<"size!"<<  pop.size()   <<"\n";


        std::vector<geneticEngine::individual> F1;

        F1.reserve(pop.size());
        // std::vector<geneticEngine::individual > Sp;


        for (unsigned int i=0; i<pop.size(); ++i) {
                pop[i].dominant_count=0;
                pop[i].domination_set.clear();
                //std::cout<<"i" << i <<"\n";
		
                for (unsigned int j=0; j<pop.size(); ++j) {
                      //  std::cout<<"j" << j<<"\n";
                        if (  dominate( pop[i].fitness,  pop[j].fitness )   ) {
                                pop[j].id=j;
                                pop[i].domination_set.push_back(pop[j]);


                        }
                        else if (  dominate( pop[j].fitness,  pop[i].fitness )   ) {
                                //     std::cout<<"j dominates i \n";
                                pop[i].dominant_count++;
                        }
                }
                // Sp.push_back(pop[i]);
                //   std::cout<<"dominant_count!" <<pop[i].dominant_count <<"\n";

                //std::cout<< "domination Count!" << pop[i].dominant_count   <<"\n";
                if(pop[i].dominant_count==0) {
                        //for (unsigned int z = 0; z < pop[i].domination_set.size() ; i++) {
                        //std::cout<< "dominationSet" << pop[i].domination_set.at(z).fitness   <<"\n";
                        //  }
                        //  std::cout<< "dominationSet Size" << pop[i].domination_set.size()   <<"\n";

                        //std::cout<< "nonDominated" <<"\n";
                        //std::cout<< pop[i].fitness <<"\n";
                      //  std::cout<< "before pushing" <<"\n";

                        //pop[i].rank=0;
                        F1.push_back(pop[i]);
                      //  std::cout<<"later of push \n";
                }
                //std::cout<<"fitness" << pop[i].fitness <<"\n";
                //std::cout<<"domination count" << pop[i].dominant_count <<"\n";
                //std::cout<<"domination set size()" << pop[i].domination_set.size() <<"\n";



        }

        //std::cout<< "before pushing in frontier" <<"\n";

        frontiers.push_back(F1);


        //std::cout<< "pushed in frontier" <<"\n";

        int iFirstFront;

        iFirstFront=0;
        std::vector<geneticEngine::individual> frontTemp;
        std::vector<geneticEngine::individual> newFront;
        newFront.reserve(pop.size());
        //std::cout<<"pushing!" <<"\n";

        //newFront.push_back(pop[0]);



        while(  true ) {
             
                 if(frontiers.size()==(unsigned int)iFirstFront){
                   break;
                 }
                 frontTemp= frontiers[iFirstFront];
                 //std::cout<< "size: "  <<frontTemp.size() <<"\n";
                 newFront.clear();
                 unsigned int i;
                 unsigned int j;
                for ( i=0; i<frontTemp.size(); ++i   ) {
                  //std::cout<< "in for: "  <<i <<"\n";

                        for (j=0; j< frontTemp[i].domination_set.size(); ++j   ) {
                           	
				//std::cout<<"Fitness "<<   frontTemp[i].domination_set[j].fitness  <<"\n";
				//std::cout<<"D Count1 "<<   pop[ frontTemp[i].domination_set[j].id].dominant_count  <<"\n";
                                //std::cout<<"D Count2 "<<   frontTemp[i].domination_set[j].dominant_count  <<"\n";
				pop[ frontTemp[i].domination_set[j].id].dominant_count--;
                              frontTemp[i].domination_set[j].dominant_count=pop[ frontTemp[i].domination_set[j].id].dominant_count  ;
				
                                

                                //if(frontTemp[i].domination_set[j].dominant_count<0) {
                                   

                                //}
                                if(frontTemp[i].domination_set[j].dominant_count==0) {

					//std::cout<<"Inserting: "<<frontTemp[i].domination_set[j].fitness<<"\n";
                                        newFront.push_back(frontTemp[i].domination_set[j]) ;

                                        //usleep(1000);
                                        //       std::cout<<"PUSHING IN NEW FRONT \n";

                                }
                           
                        }
                }
                //std::cout<<"later of for"   <<newFront.empty() <<"\n";
                if (newFront.empty()) {
                      //std::cout<<"is empty!";

                        break;
                }
                else{
                        //std::cout<<"is not empty!";
                        iFirstFront++;
                        frontiers.push_back(newFront);
                        //for (unsigned int z=0; z<pop.size(); z++) {
                        //        pop[z].dominant_count--;
                        //}

                }

        }


}

/**
 * Return true if a>b (a dominates b) after the definition used in the
 * Pareto literature:
 *
 * a>b <=> for all i a[i]>=b[i] and it exists one i such that a[i]>b[i]
 *
 * The arguments a and b represent here multidimensional fitness values.
 */

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
