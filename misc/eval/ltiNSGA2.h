
/*--------------------------------------------------------------------
 * project ....: LTI-Lib: Image Processing and Computer Vision Library
 * file .......: ltiNSGA2.h
 * authors ....: Edwin Vasquez
 * organization: LTI, Tecnologico de Costa Rica
 * creation ...: 01.09.2017
 * revisions ..:
 */


#include "ltiGeneticEngine.h"
#include "ltiProgressReporter.h"


#ifndef _LTI_NSGA2_H_
#define _LTI_NSGA2_H_


namespace lti {


    /**
     * Pareto Front computation with NSGA2
     *
     * The algorithm used here to generate the front is called NSGA2 (
     * Non Dominated Sorting Genetic Algorithm 2), and it is described in:
     *
     * Kalyanmoy Deb, Associate Member, IEEE, Amrit Pratap, Sameer Agarwal, and T. Meyarivan
     *
     * This algorithm uses two main procedures call the Fast Non Dominated Sort,
     * and the Crowding-Distance-Assignment.
     *
     * The Fast Non Dominated Sort is based in the calculation of a series
     * of fronts that selects the most non dominant individuals of the population
     *
     * The crowding distance assignment is basically a density estimation
     * of each individual, which calculates the distance between the members
     * of the population and determines which individuals are in a more dense
     * position than others
     *
     */


class NSGA2 : public geneticEngine {
  public :
    /*
    * Apply Method of the genetic Algorithm, this executes the NSGA-II Algorithm
    * with the specified parameters
    * @param PE resultant population
    * @param initFromLog flag that specifies if the execution should be resumed from a log file
    * @return return true if the algorithm was executed correctly
    *
    */
    virtual bool apply(std::vector<geneticEngine::individual>& PE,const bool initFromLog);

    /*
    * Returns a child population by making a crossover or mutation
    *of a given population
    * @param parentPop parent Population
    * @param mutationRate rate that specifies how much the new population should be mutated
    * @param childPop resultant population
    *
    */
    void selection(std::vector<geneticEngine::individual>& parentPop,double mutationRate,
      std::vector<geneticEngine::individual>& childPop);

    /*
    * Merges the child Population with the parent population
    * and verifies if there are duplicated members
    *
    * @param parentPop parent Population and the resultant population of the merge
    * @param childPop child population that will be merged
    *
    */

    void mergePop(std::vector<geneticEngine::individual>& parentPop,
    std::vector<geneticEngine::individual>& childPop);

    /*
    * Default Constructor
    *
    */

    NSGA2();
    /*
    * Default Destructor
    *
    */
    virtual ~NSGA2();

    /**
     * Copy constructor
     * @param other the parameters object to be copied
     */
    NSGA2(const NSGA2& other);
    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    NSGA2& copy(const NSGA2& other);

    /**
     * Returns the name of this type ("NSGA2")
     */
    virtual const std::string& name() const;
    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual NSGA2* clone() const;

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual NSGA2* newInstance() const;


    /**
     * Returns a random individual in the given population, which has
     * been selected because it had a smaller squeeze factor in a binary
     * tournament.
     */
    int binaryTournament(const std::vector<individual>& PE) const;



    /**
     * Return true if a>b (a dominates b) after the definition used in the
     * Pareto literature:
     *
     * a>b <=> for all i a[i]>=b[i] and it exists one i such that a[i]>b[i]
     *
     * The arguments a and b represent here multidimensional fitness values.
     */
    bool dominate(const dvector& a,
                  const dvector& b) const;

    /*
    *Class that sort two individuals based on the fitness, in ascending order
    */
    class sorter {
      int type_;
      public:
        sorter(int type) : type_(type) {}
        bool operator()(geneticEngine::individual& a, geneticEngine::individual& b) const {
        //  std::cout<<"sorting with type_" << type_ <<"\n";
        if (a.fitness[type_]>b.fitness[type_]) {
          return false;
        } else  {
            return true;
        }
        }
    };


    /**
     * The parameters for the class NSGA2.
     *
     * These are specific parameters for the computation of the NSGA2 algorithm.
     */
    class parameters : public functor::parameters {
    public:
      /**
       * Default constructor
       */
      parameters();


      /**
       * Copy constructor
       * @param other the parameters object to be copied
       */
      parameters(const parameters& other);



      /**
       * Returns name of this type
       */
      virtual const std::string& name() const;

      /**
       * Returns a pointer to a clone of the parameters
       */
      virtual parameters* clone() const;

      /**
       * Returns a pointer to a clone of the parameters
       */
      virtual parameters* newInstance() const;

      /**
       * Copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& copy(const parameters& other);




      /**
       * Crossover probability.
       *
       * The PESA algorithms does a uniform crossover with this
       * probability Pc.  This means, with probability Pc a crossover
       * between two parents will be done, otherwise only mutation
       * will be done.
       *
       * Default value: 0.7
       */
      double crossoverProbability;

      /**
       * Initial Bit-flip mutation probability.
       *
       * This value is usually set to 1/L, where L is the size of a
       * chromosome, i.e. the bit-length size given to the binary parameter
       * representation. (see paretoFront::phenotypeToChromosome).
       *
       * If negative, the value used will be |mutationRate|/L.
       * If positive, the value will be used "as is".
       *
       * Note that if positive, it only makes sense to have values between 0
       * and 1.
       *
       * This value has to be greater or equal finalMutationRate.
       *
       * Default value: -1 (i.e. 1/L will be used)
       */
      double initialMutationRate;

      /**
       * Final Bit-flip mutation probability.
       *
       * This value is usually set to 1/L, where L is the size of a
       * chromosome, i.e. the bit-length size given to the binary parameter
       * representation. (see paretoFront::phenotypeToChromosome).
       *
       * If negative, the value used will be |mutationRate|/L.
       * If positive, the value will be used "as is".
       *
       * Note that if positive, it only makes sense to have values between 0
       * and 1.
       *
       * This value has to be smaller or equal initialMutationRate.
       *
       * Default value: -1 (i.e. 1/L will be used)
       */
      double finalMutationRate;

      /**
       * Mutation Rate Decay Value
       *
       * It is possible to begin the evolution with a higher mutation rate
       * than in a "stable" evolution.  This supports a more random search a
       * the beginning, where nothing really good has been found.  After a few
       * steps can be however desirable to slowly reduce the mutation rate
       * into a more normal value.
       *
       * The ecuation used for the real mutation rate is:
       * (initialMutationRate-finalMutationRate)*exp(-i/d) with "i" the
       * iteration number and "d" this decay rate value.
       *
       * The smaller this value, the faster the mutation rate converges to
       * its final value.
       *
       * This value must be strictly positive (never zero).  If you want a
       * "traditional" PESA, just set the initial and final mutation rates
       * with the same value.
       *
       * Default value: 33.38 (i.e. after 100 steps only 5\% of the
       *                       (final-initial) interval remains)
       */
      double mutationDecayRate;

      /**
       * Size of elements that constitute the Pareto Front.
       *
       * Default value: 100
       */
      int externalPopulationSize;

      /**
       * Internal population size
       *
       * Size of elements produced by each iteration through cross
       * over or mutation as candidates for the front.
       *
       * Default value: 10
       */
      int internalPopulationSize;

      /**
       * Dimensionality of the space analyzed by the Pareto Front.
       *
       * Note that this is unrelated with the parameter-space, which usually
       * has many more dimensions that the fitness space.
       *
       * Usual values are 2 or 3, since more dimensions are very difficult to
       * visualize.
       *
       * Default value: 2
       */
      int fitnessSpaceDimensionality;

      /**
       * Number of iterations.
       *
       * The process of generating an internal population and then assign the
       * best candidates to the Pareto Front is repeated a number of times
       * specified by this parameter.
       *
       * Note that the total number of evaluations for the algorithms will
       * be approximatelly this factor times internalPopulationSize.  If
       * you really want at least externalPopulationSize elements in the
       * Pareto front, you need to provide enough iterations to allow that,
       * which should be considerably greater than
       * externalPopulationSize/internalPopulationSize, since not all
       * generated members are added to the pareto front.
       *
       * Default value: 1000
       */
      int numOfIterations;

      /**
       * Log all evaluated individuals.
       *
       * Sometimes, for documentation or debug purposes, you will want to know
       * all created individuals, even the ones not belonging to the pareto
       * front.  Since they are usually not required, and they demand some
       * resources, it is left to you if you want to keep track of them or not.
       *
       * Set this parameter to true, if you want to store all generated and
       * evaluated individuals, of false, if you want to save the space and
       * time required to remember them.
       *
       * Default value: false
       */
      bool logAllEvaluations;

      /**
       * Fitness space partitioning.
       *
       * The choice which individual(s) in the Pareto front should be chosen
       * for crossover or mutation is taken on a fitness-space density measure.
       * Elements will be removed from the dense locations, since there are
       * enough prototypes for those places, and for the generation of new
       * ones candidates are taken from the low-density regions.
       *
       * The bounding box for the fitness space will be computed
       * automatically as new elements are generated.
       * To determine the density at each location, a Gaussian
       * kernel will be used.  Its covariance matrix is assumed diagonal, where
       * each dimension will have as std. deviation a sixth of the length
       * obtaind dividing the interval with the given factor.
       *
       * In the original PESA paper a fixed grid was used, but this has
       * limitations in the reachable precision.
       *
       * Default value: 32
       */
      int fitnessSpacePartition;

      /**
       * Sort result in scanning order.
       *
       * If true, the individuals of the front will be sorted in ascending
       * order of their multidimensional fitness.
       *
       * In principle, this sorting has no semantical effects about the
       * overall fitness of an individual, i.e. an individual later in the
       * list is not necessarily better than another one with a smaller index.
       * This sorting is more oriented towards drawing tasks for the Pareto
       * front.
       *
       * According to the fitness scanning ordering, and individual A greater
       * than an individual B if
       *
       * \code
       *  (A[n-1] > B[n-1]) or
       * ((A[n-1] == A[n-1]) and ( (A[n-2] > A[n-2]) or
       *                          ((A[n-2] == A[n-2]) and (A[n-3] > A[n-3]))
       *                           ... ))
       * \endcode
       *
       * This is the same ordering employed for lti::tpoint<T>
       *
       * Sorting will use the STL methods to sort efficiently the result.
       * However, it is optional in case you don't care how the individuals
       * are sorted.
       *
       * Default value: false
       */
      bool sortResult;

      /**
       * Number of threads in the thread pool.
       *
       * The evaluation of each chromosome can be executed in parallel: each
       * in one thread.  With the number of threads used, you can control the
       * maximal number of parallel executions.  Note that the maximum number
       * of threads that can be executed is given by the size of the internal
       * population, so that it does not make sense to provide numbers larger
       * than that.  Additionally, since the evaluations are usually
       * computationally very expensive, you should provide only as many
       * threads as you have processors.
       *
       * This parameter has to be greater or equal 1.
       *
       * Default value: 1 (No parallel processing).
       */
      int numberOfThreads;

      /**
       * @name Log options
       */
      //@{
      /**
       * Activate log.
       *
       * If true, every new individual that is inserted to the front will be
       * logged in the given file.  Later on, you can use a special apply to
       * continue the analysis of a broken progress.
       *
       * Default value: false
       */
      bool logFront;

      /**
       * Log Filename.
       *
       * Filename used for the log of patterns.  The data will be written in
       * an ASCII format using a lti::lispStreamHandler.  It will contain for
       * each individual the chromosome binary representation and the computed
       * fitness.  At the beginning it will save all parameters necessary to
       * bring this functor to a compatible state.
       *
       * Default value: "pareto.log"
       */
      std::string logFilename;
      //@}

      /**
       * @name Pareto Front File
       *
       * The front file stores the final result of an evolutionary process,
       * including not only the achieved fitness values, but also the
       * parameterization that produced such fitnesses.
       *
       * The points in the front are stored in sequence of lisp pairs:
       *
       * ( (vector_1) (parameters_1) ) ( (vector_2) (parameters_2) ) ...
       *
       * and the file ends with the bounding box in the fitness space.
       */
      //@{
      /**
       * Create Pareto Front File
       *
       * If \c true then a pareto front file will be created at the end of the
       * computation, which will include the fitness and the corresponding
       * parameters.  If \c false, no file will be created.
       *
       * Default value: false
       */
      bool createFrontFile;

      /**
       * Name of Pareto Front File
       *
       * If \a parameters::createFrontFile is \c true then this attribute let
       * you specify the name of the file.
       *
       * Default value: "pareto.pf"
       */
      std::string frontFile;
      //@}

      /**
       * Configuration of the random number generator
       */
      univariateContinuousDistribution::parameters randomParams;


    protected:
      /**
       * Pointer to internal genetics object being used
       */
      genetics* geneticsObject_;


    };

    /**
     * Returns used parameters
     */

    const parameters& getParameters() const;


    virtual bool initInternalPopulation(std::vector<geneticEngine::individual>& data);




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


    virtual bool getDataFromLog(const std::string& logFile,
                        geneticEngine::parameters& params,
                        std::vector<individual>& data,
                        dmatrix& boundingBox,
                        int& lastIter) const;

  /**
  * The log-file has in the comments the iteration number.  We can
  * try to rescue that number from there.
  */

  int findLastIter(const std::string& logFile) const;


  bool logEntry(const geneticEngine::individual& ind,const bool markDead=false);






    /**
     * Makes the computation of the Fast Non Dominant Sort Algorithm
     * @param pop population to be applied this algorithm
     * @param frontResultant resultant fronts that contains the best individuals
     * of the population
     */
    void  fastNonDominatedSort(std::vector<geneticEngine::individual>& pop,
     std::vector<std::vector<geneticEngine::individual> >& frontResultant);
     /**
      * Makes the computation of the crowding distance given a population
      * @param nonDominated population that will be calculated his density
      */
    void calculateCrowdingDistance(std::vector<geneticEngine::individual>& nonDominated);

    /**
     * Structure that sorts a given population by his crowding distance
     */
    struct sortByCrowdingDistance;



  };


  }

  #endif
