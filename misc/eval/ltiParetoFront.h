/*
 * Copyright (C) 1998, 1999, 2000, 2001, 2002, 2003,
 * Lehrstuhl fuer Technische Informatik, RWTH-Aachen, Germany
 *
 * Copyright (C) 2006
 * Pablo Alvarado, ITCR
 *
 * This file is part of the LTI-Computer Vision Library (LTI-Lib)
 *
 * The LTI-Lib is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License (LGPL)
 * as published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * The LTI-Lib is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with the LTI-Lib; see the file LICENSE.  If
 * not, write to the Free Software Foundation, Inc., 59 Temple Place -
 * Suite 330, Boston, MA 02111-1307, USA.
 */


/*--------------------------------------------------------------------
 * project ....: LTI-Lib: Image Processing and Computer Vision Library
 * file .......: ltiParetoFront.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 25.11.2003
 * revisions ..: $Id: ltiParetoFront.h,v 1.10 2008-10-14 22:47:52 alvarado Exp $
 */

#ifndef _LTI_PARETO_FRONT_H_
#define _LTI_PARETO_FRONT_H_

#include <list>
#include <vector>
#include <fstream>
#include "ltiVector.h"
#include "ltiMatrix.h"
#include "ltiFunctor.h"
#include "ltiProgressReporter.h"
#include "ltiLispStreamHandler.h"
#include "ltiGenetics.h"

#include "ltiThread.h"
#include "ltiMutex.h"
#include "ltiSemaphore.h"
//#include "ltiGeneticEngine.h"

namespace lti {
  class geneticEngine;


  /**
   * Pareto Front computation with PESA
   *
   * The Pareto Front functor provides an evaluation mechanism for algorithms,
   * in which the qualitiy of the algorithm cannot be restricted to one single
   * scalar measure.  It is adecuate to compare different parameterizations for
   * one or several algorithms, which are supposed to solve the same task.
   *
   * This class generates a "front" in a multidimensional fitness
   * space, which represents a trade-off between several fitness
   * values, that each derived class explicitelly implements.
   *
   * The algorithm used here to generate the front is called PESA (Pareto
   * Envelope-based Selection Algorithm), and it is described in:
   *
   * David. W. Corne, Joshua D. Knowles and Martin J. Oates
   * The Pareto Envelope-based Selection Algorithm for Multiobjective
   * Optimization. In: Proceedings of the International Conference on Parallel
   * Problem Solving from Nature (PPSN VI). (2000) 839-848
   *
   * A good introduction to the application of this problem for the
   * evaluation of image processing algorithms, specifically to the evaluation
   * of segmentation is given in:
   *
   * Mark Everingham, Henk Muller and Barry Thomas, Evaluating Image
   * Segmentation Algorithms using the Pareto Front. In Proceedings of
   * the 7th European Conference on Computer Vision (ECCV2002), Part
   * IV (LNCS 2353), pages 34-48. Springer, June 2002.
   *
   * http://www.cs.bris.ac.uk/Tools/Reports/Abstracts/2002-everingham-1.html
   *
   * and it was also used in
   *
   * Pablo Alvarado, "Segmentation of color images for interactive 3D object
   * retrieval".  Ph.D. Thesis.  RWTH-Aachen, July 2004 that can be downloaded
   * at
   *
   * http://sylvester.bth.rwth-aachen.de/dissertationen/2004/121/04_121.pdf
   *
   *
   * This is the parent class for all evaluation algorithms using PESA, which
   * usually do not need any modification.  The genetic part of the algorithm
   * is mostly implemented in the application dependent classes inherited from
   * lti::genetics.
   *
   * There is a deviation from the original paper which is more suitable for
   * the evaluation of algorithms used here.  The original algorithms
   * separates the fitness space into regular hyperboxes and computes the
   * density of individuals in each hyperbox to decide which elements will be
   * used for crossover or mutation (those boxes with the smallest density)
   * and which elements need to be removed (those with the highest density).
   * The computation of the density is in the present functor much more
   * computationally expensive, but its computation is nevertheless negligible
   * in comparison to the computation of the fitness measures for an
   * algorithm:
   *
   * The current algorithm keeps track of the bounding box of the fitness
   * space for which individuals have been created.  This bounding box is used
   * to determine the size of a Gaussian kernel, which will be used on each
   * individual to compute the influence of all other individuals in the
   * front.  This way, the selection is not strongly bounded to the selection
   * of a partitioning of the fitness space.
   *
   * For an example of an evaluation class see lti::locationSearchEvaluation.
   *
   * \section Progress information
   *
   * To monitor the progress of the evolutionary process this functor makes use
   * of classes derived from lti::progressInfo, where the detail level of
   * the substeps can be used to show the information required.
   * - Detail Level 0: Only the steps are shown, with the front size and
   *                   the numbers of new individuals inserted.
   * - Detail Level 1: A line for each individual in the internal
   *                   population indicating the start of the evaluation is
   *                   shown
   * - Detail Level 2: For each individual, the fitness vector computed
   *                   is also displayed.
   * - Detail Level 3: At the end of each evaluation cicle (all internal
   *                   population already evaluated) show
   *                   - New non-dominated child (NNDC)
   *                   - Recently dead individual (RDIn)
   *                   - Individual removed by overpopulation (HDRI)
   *
   * Each step reports the number of generation, the percentage of
   * the evolutionary process completed, the number of individuals in
   * the Pareto front, the number of individuals recently added to the
   * front, and the Estimated Remaining Time (ERT) of the process,
   * which should become more reliable the longer the process has been running.
   */
  class paretoFront : public functor,
                      public progressReporter {
  public:
      geneticEngine* engine_;

    /**
     * The parameters for the class paretoFront.
     *
     * These are the general parameters for the Pareto Front computation.
     * For each special evaluation class, more attributes are usually added.
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
       * Destructor
       */
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
       * Copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& operator=(const parameters& other);

      /**
       * Write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool write(ioHandler& handler,const bool complete=true) const;

      /**
       * Read the parameters from the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool read(ioHandler& handler,const bool complete=true);

      // ------------------------------------------------
      // the parameters
      // ------------------------------------------------

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

      /**
       * Size of elements that constitute the Pareto Front.
       *
       * Default value: 100
       */

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
      //@}

      std::string logFilename;



      std::string engineName;


      std::string engineParamFile;

      geneticEngine* engine_;


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

      /**
       * Name of Pareto Front File
       *
       * If \a parameters::createFrontFile is \c true then this attribute let
       * you specify the name of the file.
       *
       * Default value: "pareto.pf"
       */
      //@}

      /**
       * Configuration of the random number generator
       */
      univariateContinuousDistribution::parameters randomParams;

      /**
       * Set the genetics object to be used.
       *
       * A copy of the given object will be done.
       */
      bool setGeneticsObject(const genetics& obj);

      /**
       * Return a writable reference to the genetics object
       */
      genetics& getGeneticsObject();

      genetics& getGenObject();

      /**
       * Return a read only reference to the genetics object
       */
      const genetics& getGeneticsObject() const;

    protected:
      /**
       * Pointer to internal genetics object being used
       */
      genetics* geneticsObject_;
    };
    /**
     * Default constructor
     */
    paretoFront();

    /**
     * Construct a functor using the given parameters
     */
    paretoFront(const parameters& par);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    paretoFront(const paretoFront& other);

    /**
     * Destructor
     */
    virtual ~paretoFront();

    /**
     * Returns the name of this type ("paretoFront")
     */
    virtual const std::string& name() const;

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual paretoFront* clone() const;

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual paretoFront* newInstance() const;

    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    paretoFront& copy(const paretoFront& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    paretoFront& operator=(const paretoFront& other);

    /**
     * Compute the Pareto Front
     *
     * The Pareto Front will be represented by a matrix of size \e m x \e n
     * with \e m parameters::externalPopulationSize and \e n
     * fitnessSpaceDimensionality.
     *
     * @param front matrix containing for each row the multidimensional
     *             fitness value.
     *
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(matrix<double>& front);

    /**
     * Compute the Pareto Front and the corresponding parameter objects
     * for each non-dominated point.
     *
     * The Pareto Front will be represented by a matrix of size \e m x \e n
     * with \e m parameters::externalPopulationSize and \e n
     * fitnessSpaceDimensionality.
     *
     * For each row in this matrix (an individual), the corresponding
     * parameter setting (a phenotype) is returned.  Since different
     * algorithms usually use different parameters, this can only be
     * done with dynamically allocated objects.  You need to take care of
     * them, but this class provides an utility trash() method, which deletes
     * them for you if you want to.
     *
     * @param front matrix containing for each row the multidimensional
     *             fitness value.
     * @param phenotypes the parameters for each fitness value.
     *
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(matrix<double>& front,
                       std::vector<functor::parameters*>& phenotypes);

    /**
     * Compute the Pareto Front and the corresponding parameter objects
     * for each non-dominated point.
     *
     * This is a special method which reassumes a broken analysis, gaining
     * the lost information from a previously generated file.  That file
     * can be generated activating in the parameters the option logFront and
     * indicating there the name for the file.
     *
     * This method will work only if the parameters logFront and logFilename
     * are properly set (logFront in true and logFilename with the proper
     * filename).  It is not named "apply" for the simple reason that the
     * parameters will be changed to those stored in the log-file itself.
     *
     * The Pareto Front will be represented by a matrix of size \e m x \e n
     * with \e m parameters::externalPopulationSize and \e n
     * fitnessSpaceDimensionality.
     *
     * For each row in this matrix (an individual), the corresponding
     * parameter setting (a phenotype) is returned.  Since different
     * algorithms usually use different parameters, this can only be done with
     * dynamically allocated objects.  You need to take care of them, but this
     * class provides an handy trash() method, which deletes them for you if
     * you want to.  If you don't remove them properly you will have a huge
     * memory-leak!  Therefore, we recomend the use of trash().
     *
     * @param front matrix containing for each row the multidimensional
     *             fitness value.
     * @param phenotypes the parameters for each fitness value.
     *
     * @return true if apply successful or false otherwise.
     */
    virtual bool resume(matrix<double>& front,
                        std::vector<functor::parameters*>& phenotypes);

    /**
     * Compute the Pareto Front and the corresponding parameter objects
     * for each non-dominated point.
     *
     * This is a special method which reassumes a broken analysis, gaining
     * the lost information from a previously generated file.  That file
     * can be generated activating in the parameters the option logFront and
     * indicating there the name for the file.
     *
     * This method will work only if the parameters logFront and logFilename
     * are properly set (logFront in true and logFilename with the proper
     * filename).  It is not named "apply" for the simple reason that the
     * parameters will be changed to those stored in the log-file itself.
     *
     * The Pareto Front will be represented by a matrix of size \e m x \e n
     * with \e m parameters::externalPopulationSize and \e n
     * fitnessSpaceDimensionality.
     *
     * @param front matrix containing for each row the multidimensional
     *             fitness value.
     *
     * @return true if apply successful or false otherwise.
     */
    virtual bool resume(matrix<double>& front);


    /**
     * @name Additional Information
     */
    //@{
    /**
     * Fitness space bounding box.
     *
     * After determining the Pareto Front, you usually want to know the
     * bounding box of the fitness space that was analyzed.  This for
     * visualization purposes among others.
     *
     * After the apply() methods you can get the bounding box calling this
     * one.
     *
     * This will always be a matrix 2 x parameters::fitnessSpaceDimensionality,
     * where the first row contains the minima for each dimension and the
     * second row the maxima.
     */
    void getAnalyzedBox(matrix<double>& bbox) const;

    /**
     * Dominated individuals.
     *
     * This method returns all points in the fitness space that were
     * analyzed while constructing the feature space.  For this method
     * to return something meaningful, you need to set in the
     * parameters the attribute logAllEvaluations to \c true.
     *
     * The resulting matrix will have the
     * size \e n x parameters::fitnessSpaceDimensionality, where \e n is
     * the number of dominated individuals analyzed in the process.
     */
    void getDominatedIndividuals(matrix<double>& dindiv) const;

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
    bool getDataFromLog(const std::string& logFile,
                        matrix<double>& front,
                        std::vector<functor::parameters*>& phenotypes,
                        int& lastIter);
    //@}


    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * Update parameters
     */
    bool updateParameters();

    /**
     * Type used to represent chromosomes
     */
    typedef genetics::chromosome chromosome;

    /**
     * Delete all parameter objects in the given vector.
     *
     * The vector size is left unchanged, but each pointed object is removed
     * and the pointers set to null.
     */
    bool trash(std::vector<functor::parameters*>& phenotypes) const;

  //protected:
    /**
     * Returns used parameters
     */
    parameters& getRWParameters();

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
    virtual bool initInternalPopulation(std::vector<individual>& data);


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
    bool pesa(std::vector<individual>& PE,const bool initFromLog=false);

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
     * Return true if a>b (a dominates b) after the definition used in the
     * Pareto literature:
     *
     * a>b <=> for all i a[i]>=b[i] and it exists one i such that a[i]>b[i]
     *
     * The arguments a and b represent here multidimensional fitness values.
     */
    bool dominate(const dvector& a,
                  const dvector& b) const;

    //@}

    /**
     * LUT-based method for computing g(x) = exp(-x^2).
     * The value given is x and not x^2
     *
     * Only values between 0 and 3 will produce something, all the rest
     * produce zero as output
     */
    static const double* expLUT_;

    /**
     * Initialize expLUT
     */
    bool initExpLUT();

    /**
     * An efficient way to compute g(x)=exp(-x^2)
     */
    inline double exp2(const double& x) const;

    /**
     * Compute the fitness distance between the given two fitness points
     */
    inline double fitnessDistance(const dvector& a,
                                  const dvector& b) const;

    /**
     * Log all evaluation
     */
    bool logEvaluations_;

    /**
     * All individuals not belonging to the pareto front are somehow dead!
     *
     * This will be used only if logEvaluations is true
     */
    std::list<individual> deadIndividuals_;

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
     * Initialize log.
     *
     * This method reinitializes the log.  It writes the functor parameters
     * and internal configuration.
     */
    bool initLog();

    /**
     * Make an entry for the individual in the log file (if desired).
     *
     * @return True if the logging is activated, or false if no log was
     *         desired.
     */
    bool logEntry(const individual& ind,const bool markDead=false);

    /**
     * Initialize the bounding box
     */
    void initBoundingBox(dmatrix& boundingBox) const;

    /**
     * Update bounding box considering the given fitness space point.
     *
     * @return True if there was a change in the bounding box, false if
     * given point was already within the bounding box.
     */
    bool updateBoundingBox(const dvector& pnt,
                                 dmatrix& boundingBox) const;

    /**
     * Update fitness space subdivision.
     *
     * This initializes the sigmas based on the bounding box contents.
     */
    void updateFitnessSpaceSubdivision();

    /**
     * Update density factors.
     *
     * Recompute all squeeze factors for the individuals in the
     * external population
     */
    void updateDensityFactors(std::vector<individual>& PE);

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
    bool getDataFromLog(const std::string& logFile,
                        parameters& params,
                        std::vector<individual>& data,
                        dmatrix& boundingBox,
                        int& lastIter) const;

    /**
     * The log-file has in the comments the iteration number.  We can
     * try to rescue that number from there.
     */
    int findLastIter(const std::string& logFile) const;

    /**
     * Create the final pareto front file
     */
    bool
    createFrontFile(const std::string& filename,
                    const matrix<double>& front,
                    const std::vector<functor::parameters*>& phenotypes) const;


    /**
     * Get a random value between 0 and 1
     */
    inline double random() const;

    /**
     * Random number generator
     */
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
      pfThread(paretoFront& info,
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
      semaphore sem_;

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
      paretoFront* info_;
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
      queueProcessing(paretoFront& info);

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
      paretoFront* info_;

    }; // end of queueProcessing class

    /**
     * The queueProcessor instance for this paretoFront instance
     */
    queueProcessing queueProcessor_;

  };
}

#endif
