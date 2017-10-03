
/*--------------------------------------------------------------------
 * project ....: LTI-Lib: Image Processing and Computer Vision Library
 * file .......: ltiGeneticEngine.h
 * authors ....: Edwin Vasquez
 * organization: LTI, Tecnologico de Costa Rica
 * creation ...: 01.09.2017
 * revisions ..:
 */

#ifndef _LTI_GENETIC_ENGINE_H_
#define _LTI_GENETIC_ENGINE_H_
#include <list>
#include <vector>
#include "ltiVector.h"
#include "ltiFunctor.h"
#include "ltiProgressReporter.h"
#include "ltiGenetics.h"
#include "ltiRound.h"
#include "ltiThread.h"
#include <fstream>
#include <sstream>
#include "ltiMatrix.h"
#include "ltiLispStreamHandler.h"
#include "ltiSemaphore.h"



#ifdef _LTI_GNUC_2
namespace std {
  typedef ios ios_base;
}
#endif

#undef _LTI_DEBUG
//#define _LTI_DEBUG 2
#include "ltiDebug.h"



namespace lti{


  /**
   * ltiGeneticEngine
   *
   * Factory that creates the Genetic Algorithm specified,
   */

class geneticEngine: public functor, public progressReporter   {
  public :

  typedef genetics::chromosome chromosome;


  /**
   * The parameters for the class Genetic Engine.
   *
   * These are specific parameters for the computation of the NSGA2 or PESA algorithm.
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


    ~parameters();


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
     * Read the parameters from the given ioHandler
     * @param handler the ioHandler to be used
     * @param complete if true (the default) the enclosing begin/end will
     *        be also written, otherwise only the data block will be written.
     * @return true if write was successful
     */
    virtual bool read(ioHandler& handler,const bool complete=true);





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


    /**
     * Return a writable reference to the genetics object
     */
    genetics& getGeneticsObject();

    /**
     * Return a read only reference to the genetics object
     */
    const genetics& getGeneticsObject() const;


    /**
     * Set the genetics object to be used.
     *
     * A copy of the given object will be done.
     */
    bool setGeneticsObject( genetics* obj) ;


  protected:
    /**
     * Pointer to internal genetics object being used
     */
     genetics* geneticsObject_;




  };




    // virtual void setQueueProcessor(  );
    virtual const std::string& name() const;
    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual geneticEngine* clone() const;

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual geneticEngine* newInstance() const;

    /*
    * Default Constructor
    *
    */

    geneticEngine();

    /*
    * Default Destructor
    *
    */
    virtual ~geneticEngine();

    /**
     * Copy constructor
     * @param other the parameters object to be copied
     */

    geneticEngine(const geneticEngine& other);

    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */

    geneticEngine& copy(const geneticEngine& other);

    /**
     * Returns used parameters
     */

    const parameters& getParameters() const;

    /**
     * Returns used parameters
     */

    parameters& getRWParameters();



//  protected:

    /**
     * Structure characterizing an individual
     */
    class individual {
    public:
      /**
       * Constructor
       */
      individual() : fitness(),genotype(),squeezeFactor(0.0) {}

      /**
       * An individual is here "smaller" than another one if
       * its squeezeFactor is smaller.  This is required to sort
       * the individuals after their squeeze factors, which simplifies
       * getting the smallest or greatest elements.
       */
      inline bool operator<(const individual& other) const {
        return (squeezeFactor < other.squeezeFactor);
      }

      /**
       * Fitness of the individual
       */
      dvector fitness;

      int dominant_count;

      int rank;

      std::vector<individual> domination_set;

      int id;

      /**
       * Chromosome.
       *
       * You can get the phenotype anytime with cromosomeToPhenotype
       * methods in the corresponding lti::genetics class.
       */
      chromosome genotype;

      /**
       * Factor computed to determine which candidates should be taken
       * for mutation or crossover.  Dense locations in the fitness space
       * get a high squeeze factor.
       */
      double squeezeFactor;
    };




    std::list<individual> deadIndividuals_;


    /**
     * Initialize the internal population.
     *
     * You usually don't want to reimplement this method, but
     * randomIndividual().  However, you can reimplemented it if you
     * need more that the standard generation of
     * parameters::internalPopulationSize random individuals.
     *
     * @return true if successful, false otherwise.
     */


  //private:
    /**
     * @name PESA Algorithm
     */
    //@{
    /**
     * The PESA Algorithm
     *
     * Computes the Pareto front, which will be return as list of individuals
     * in PE.
     */
    //bool pesa(std::vector<individual>& PE,const bool initFromLog=false);

    /**
     * Insert non-dominated member from PI to PE
     *
     * Return the number of elements of PI that were inserted in PE.
     */
    int insert(std::vector<individual>& PI,
               std::vector<individual>& PE);

    /**
     * Insert non-dominated member into PE
     */
    bool insert(individual& genotype,
                std::vector<individual>& PE);

    /**
     * Returns a random individual in the given population, which has
     * been selected because it had a smaller squeeze factor in a binary
     * tournament.
     */
    int binaryTournament(const std::vector<individual>& PE) const;

    /**
     * Log all evaluation
     */
    bool logEvaluations_;





    //@}


    /**
     * Bounding box.
     *
     * The size of this matrix will be:
     * 2 x parameters::fitnessSpaceDimensionality
     */
    dmatrix bbox_;

    /**
     * Sigmas
     *
     * The fitness space grid size will be used to compute the std. deviation
     * per each axis.
     */
    dvector sigmas_;


    /**
     * Get data from log
     *
     * This method reads the log file and create the corresponding data.
     * Since usually the logs are broken (the user breaks the execution of
     * a long computing process), this method needs to cope with broken
     * files.
     *
     * @param logFile name of the file with the log
     * @param params parameters as written in the log file
     * @param data all data found in the log file.
     * @param boundingBox bounding box of the data
     * @return true if successful.
     */
    virtual bool getDataFromLog(const std::string& logFile,
                        parameters& params,
                        std::vector<individual>& data,
                        dmatrix& boundingBox,
                        int& lastIter) const;


    /**
     * Convert a chromosome into a string, to be saved in the log file
     */
    void chromosomeToString(const chromosome& genotype,std::string& str) const;


    /**
     * Convert a string into a chromosome into a string, to be loaded from
     * the log file
     */
    void stringToChromosome(const std::string& str,
                            chromosome& genotype) const;


    /**
     * Class used to compare individuals in "scanning order"
     */
    struct scanLess;


    /**
     * Output stream used to write the log
     */
    std::ofstream* logOut_;

    /**
     * Lisp-Stream-Handler used for log output
     */
    lispStreamHandler olsh_;

    /**
     * Copied from the parameters
     */
    bool logFront_;


    /*
    * Apply Method of the genetic Algorithm, this executes the NSGA-II or PESA Algorithm
    * with the specified parameters
    * @param PE resultant population
    * @param initFromLog flag that specifies if the execution should be resumed from a log file
    * @return return true if the algorithm was executed correctly
    *
    */

    virtual bool apply(std::vector<geneticEngine::individual>& PE,const bool initFromLog);


    /*
    * initialize the geneticEngine with the necessary variables from the paretoFront
    *
    */

    virtual void initAlg(dmatrix& bbox_,dvector& sigmas_ ,univariateContinuousDistribution& rnd_,
       bool& logEvaluations_, bool& logFront_ , lispStreamHandler& olsh_,std::ofstream* logOut_,
     std::list<individual>& deadIndividuals_ ,
     const double* expLUT_);


    mutable univariateContinuousDistribution rnd_;

    class queueProcessing;

    /**
     * Thread for execution of an evaluation task and then sleep until a new
     * computation is requested
     */
    class pfThread : public thread {
    public:
      /**
       * Constructor.
       *
       * Receives a pointer to the paretoFront class in order to report
       * the progress
       */
      pfThread(geneticEngine& info,
               queueProcessing& manager);

      /**
       * Destructor
       */
      ~pfThread();


      /**
       * Evalutate the given individual
       */
      void evaluate(individual& somebody,
                    ubyte& success,
                    const genetics& tool,
                    const int who);

    protected:
      /**
       * Execution method
       */
      void run();

      /**
       * Semaphore for inter-thread communication
       */
      lti::semaphore sem_;

      /**
       * Protect the attributes
       */
      mutex lock_;

      /**
       * Flag to stop the thread
       */
      bool stopRQ_;

      /**
       * Genetics tool to be used
       */
      const genetics* tool_;

      /**
       * Pointer to individual that has to be evaluated
       */
      individual* somebody_;

      /**
       * Id of the individual being under evaluation
       */
      int who_;

      /**
       * Success flag
       */
      ubyte* success_;

      /**
       * Manager
       */
      queueProcessing* manager_;

      /**
       * Pareto Front class with the progress info object
       */
      geneticEngine* info_;
    };



    /**
     * Class that takes the parallel process in execution
     *
     */
    class queueProcessing {

    public:
      /**
       * Constructor
       *
       * Needs the enclosing instance to make use of the progress info object
       */
      queueProcessing(geneticEngine& info);

      /**
       * Destructor
       *
       * Delete all threads
       */
      ~queueProcessing();

      /**
       * Set number of threads
       */
      void setNumberOfThreads(const int threads);

      /**
       * Initialize all internals to a state just for processing
       */
      void init();

      /**
       * Evaluate the whole internal population using the available threads
       */
      void evaluate(std::vector<individual>& PI,
                    vector<ubyte>& success,
                    const genetics& tool);

      /**
       * This method is used by the threads to report a job finished
       */
      void report(pfThread* theThread,const int who);

    protected:
      /**
       * Execution threads
       *
       * This vector grows in size, but never is reduced, to avoid the
       * unnecessary creation of many threads. In any case, all unused threads
       * are simply unactive
       */
      std::vector<pfThread*> threads_;

      /**
       * Protect the lists
       */
      mutex lock_;

      /**
       * Semaphore used to manage the threads list;
       */
      semaphore* sem_;

      /**
       * List of threads ready to process something
       */
      std::list<pfThread*> ready_;

      /**
       * Pareto front with the progress info objects
       */
      geneticEngine* info_;

    }; // end of queueProcessing class

    /**
     * The queueProcessor instance for this paretoFront instance
     */
    queueProcessing queueProcessor_;















};

}


#endif
