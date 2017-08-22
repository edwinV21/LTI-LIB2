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
