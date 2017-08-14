/*
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

/**
 * \file   ltiGenetics.h
 *         Defines the abstract parent class of the application dependent
 *         genetic algorithm interface
 * \author Pablo Alvarado
 * \date   19.08.06
 *
 * $Id: ltiGenetics.h,v 1.12 2010-03-26 20:56:20 alvarado Exp $
 */

#ifndef _LTI_GENETICS_H_
#define _LTI_GENETICS_H_

#include "ltiIoObject.h"
#include "ltiFunctor.h"
#include "ltiStatus.h"
#include "ltiParametersManager.h"
#include "ltiUnivariateContinuousDistribution.h"
#include "ltiVector.h"
#include <vector>

namespace lti {
  /**
   * Base abstract class for genetic data structures.
   *
   * Different kinds of genetic algorithms have to represent "phenotypes" of
   * parameterizations in an generic form that allows an easy implementation of
   * the basic mutation and crossover operations.  
   *
   * This abstract class provides a standard interface to convert "phenotypes"
   * (i.e. the parameters instance) into a chromosome string (a chain of zeros
   * and ones) and viceversa.  It also allows the generation of random
   * individuals.
   *
   * You can check the lti::pareto class for an example of use of this data
   * structure.
   *
   * The genetic algorithms will make use of a factory to obtain the proper
   * instances of the lti::genetics inherited classes.
   */
  class genetics 
    : public ioObject,
      public parametersManager,
      public status {
  public:

    /**
     * The parameters for the class genetics.
     *
     * These are the specific parameters of the evaluation concepts,
     * where filenames, which fitness scalars compose the aggregate
     * fitness, and so on.
     */
    class parameters : public parametersManager::parameters {
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
       * Configuration of the random number generator used.
       *
       * This parameters are used in the generation of new genotypes, and in
       * the mutation and crossover of the genotypes of other individuals.
       */
      univariateContinuousDistribution::parameters randomParams;
    };

    /**
     * Type used to represent chromosomes
     */
    typedef std::vector<bool> chromosome;

    /**
     * Default constructor.
     *
     * You can optionally specify with a boolean if you want to initialize
     * the parameters (true) or not (false).  The latter is usually used by
     * inherited classes to avoid a redundant setting of the parameters.
     */
    genetics(const bool init = true);

    /**
     * Copy constructor
     */
    genetics(const genetics& other);

    /**
     * Destructor
     */
    virtual ~genetics();

    /**
     * Return the fully qualified name of this class.
     */
    virtual const std::string& name() const;

    /**
     * Clone method.
     */
    virtual genetics* clone() const = 0;

    /**
     * New instance method.
     */
    virtual genetics* newInstance() const = 0;

    /**
     * Copy method
     */
    genetics& copy(const genetics& other);

    /**
     * Alias for Copy method
     */
    genetics& operator=(const genetics& other);

    /**
     * Write the parameters in the given ioHandler
     *
     * @param handler the ioHandler to be used
     * @param complete if true (the default) the enclosing begin/end will
     *        be also written, otherwise only the data block will be written.
     * @return true if write was successful
     *
     * This method has to be overloaded.  If not it always returns false.
     */
    virtual bool write(ioHandler& handler, const bool complete=true) const;

    /**
     * Read the parameters from the given ioHandler
     *
     * @param handler the ioHandler to be used
     * @param complete if true (the default) the enclosing begin/end will
     *        be also written, otherwise only the data block will be written.
     * @return true if write was successful
     *
     * This method has to be overloaded.  If not it always returns false.
     */
    virtual bool read(ioHandler& handler,const bool complete=true);

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * Update parameters
     */
    bool updateParameters();


    /**
     * @name Public methods to be reimplemented
     *
     * Following methods need to be reimplemented to evaluate specific
     * algorithms.
     */
    //@{

    /**
     * Convert a binary-chain representation of a chromosome to a valid
     * parameter object.
     *
     * There are some tools to convert standard types into some binary chains,
     * which can be used by all derived classes:
     * - binToInt()
     * - binToUInt()
     * - binToDouble()
     */
    virtual bool 
    chromosomeToPhenotype(const chromosome& genotype,
                                functor::parameters& phenotype) const=0;

    /**
     * Return a fresh allocated parameters for the evaluated functor, which is
     * equivalent to the given genotype.
     *
     * There are some tools to convert binary chains into standard types:
     * - intToBin()
     * - uintToBin()
     * - doubleToBin()
     */
    virtual functor::parameters* 
    chromosomeToPhenotype(const chromosome& genotype) const = 0;

    /**
     * Convert a valid parameters object (phenotype) into binary-chain
     * representation of a chromosome.
     */
    virtual bool phenotypeToChromosome(const functor::parameters& phenotype,
                                             chromosome& genotype) const=0;

    /**
     * Return the length in bits for a chromosome.
     *
     * This method needs to be reimplemented, in order to get some 
     * default implementations to work.
     */
    virtual int getChromosomeSize() const = 0;

    /**
     * Evaluate Chromosome
     *
     * This method is one of the most important ones for any genetic algorithm.
     * Its task is to produce a multidimensional fitness measure for a given
     * chromosome.
     *
     * It returns true if the evaluation was successful, of false if the
     * phenotype represents some invalid parameterization.  It is highly 
     * recomended that the mutation and crossover methods are reimplemented to 
     * avoid invalid parameterizations.
     *
     * There are mainly two types of fitness measures that can be analyzed:
     * empirical goodness and empirical discrepancy (Zhang).  The empirical
     * goodness computes some measure using exclusively the test data, without
     * requiring any ground truth.  The empirical discrepancy assumes the
     * existency of ground truth and provides as measure some distance between
     * the result of an algorithm and the ground truth.  Each class derived
     * from genetics should specify clearly which kind of fitness measures it
     * provides.
     *
     * Since the implementations of genetic algorithms are highly parallel,
     * ensure that this method can be called by several threads without causing
     * any conflicts (avoid the use of static methods and protect the internal
     * state of your inherited classes).
     *
     * This method is responsible for initializing the correct size of
     * the \a fitness vector.
     *
     * @param individual is the encoded parameter object to be evaluated.
     * @param fitness is the result of the evaluation.
     * @return true if the evaluation was successful, or false if it failed.
     */
    virtual bool evaluateChromosome(const chromosome& individual,
                                          dvector& fitness) const = 0; 


    /**
     *
     * This method is one of the most important ones for any genetic algorithm.
     * Its task is to produce a multidimensional fitness measure for a given
     * chromosome.
     *
     * It returns true if the evaluation was successful, of false if the
     * phenotype represents some invalid parameterization.  It is highly 
     * recomended that the mutation and crossover methods are reimplemented to 
     * avoid invalid parameterizations.
     *
     * There are mainly two types of fitness measures that can be analyzed:
     * empirical goodness and empirical discrepancy (Zhang).  The empirical
     * goodness computes some measure using exclusively the test data, without
     * requiring any ground truth.  The empirical discrepancy assumes the
     * existency of ground truth and provides as measure some distance between
     * the result of an algorithm and the ground truth.  Each class derived
     * from genetics should specify clearly which kind of fitness measures it
     * provides.
     *
     * Since the implementations of genetic algorithms are highly parallel,
     * ensure that this method can be called by several threads without causing
     * any conflicts (avoid the use of static methods and protect the internal
     * state of your inherited classes).
     *
     * The default implementation just ignores the suboptimized chromosome,
     * and calls the evaluateChromosome(individual,fitness).
     *
     * This method is responsible for initializing the correct size of
     * the \a fitness vector.
     *
     * @param individual is the encoded parameter object to be evaluated.
     * @param fitness is the result of the evaluation.
     * @param suboptimized is the reencoded parameter object
     * @return true if the evaluation was successful, or false if it failed.
     */
    virtual bool evaluateChromosome(const chromosome& individual,
				          dvector& fitness,
				          chromosome& suboptimized) const; 

    /**
     * Evaluate Chromosome
     *
     * This method is one of the most important ones for any genetic algorithm.
     * Its task is to produce a multidimensional fitness measure for a given
     * chromosome.
     *
     * It returns true if the evaluation was successful, of false if the
     * phenotype represents some invalid parameterization.  It is highly
     * recomended that the mutation and crossover methods are reimplemented to
     * avoid invalid parameterizations.
     *
     * There are mainly two types of fitness measures that can be analyzed:
     * empirical goodness and empirical discrepancy (Zhang).  The empirical
     * goodness computes some measure using exclusively the test data, without
     * requiring any ground truth.  The empirical discrepancy assumes the
     * existency of ground truth and provides as measure some distance between
     * the result of an algorithm and the ground truth.  Each class derived
     * from genetics should specify clearly which kind of fitness measures it
     * provides.
     *
     * Since the implementations of genetic algorithms are highly parallel,
     * ensure that this method can be called by several threads without causing
     * any conflicts (avoid the use of static methods and protect the internal
     * state of your inherited classes).
     *
     * This method is responsible for initializing the correct size of
     * the \a fitness vector.
     *
     * @param id identification of the iteration number.  This parameter is
     *           used in parallel environments to identify the number of the 
     *           "thread" or "process" being run.
     *           The id will be always between 0 and the size of the internal 
     *           population-1.
     * @param individual is the encoded parameter object to be evaluated.
     * @param fitness is the result of the evaluation.
     * @return true if the evaluation was successful, or false if it failed.
     */
    virtual bool evaluateChromosome(const int id,
                                    const chromosome& individual,
                                    dvector& fitness) const;

    /**
     * Evaluate Chromosome
     *
     * This method is one of the most important ones for any genetic algorithm.
     * Its task is to produce a multidimensional fitness measure for a given
     * chromosome.
     *
     * It returns true if the evaluation was successful, of false if the
     * phenotype represents some invalid parameterization.  It is highly
     * recomended that the mutation and crossover methods are reimplemented to
     * avoid invalid parameterizations.
     *
     * There are mainly two types of fitness measures that can be analyzed:
     * empirical goodness and empirical discrepancy (Zhang).  The empirical
     * goodness computes some measure using exclusively the test data, without
     * requiring any ground truth.  The empirical discrepancy assumes the
     * existency of ground truth and provides as measure some distance between
     * the result of an algorithm and the ground truth.  Each class derived
     * from genetics should specify clearly which kind of fitness measures it
     * provides.
     *
     * Since the implementations of genetic algorithms are highly parallel,
     * ensure that this method can be called by several threads without causing
     * any conflicts (avoid the use of static methods and protect the internal
     * state of your inherited classes).
     *
     * The default implementation just ignores the suboptimized chromosome,
     * and calls the evaluateChromosome(individual,fitness).
     *
     * This method is responsible for initializing the correct size of
     * the \a fitness vector.
     *
     * @param id identification of the iteration number.  This parameter is
     *           used in parallel environments to identify the number of the 
     *           "thread" or "process" being run.
     *           The id will be always between 0 and the size of the internal 
     *           population-1.
     * @param individual is the encoded parameter object to be evaluated.
     * @param fitness is the result of the evaluation.
     * @param suboptimized is the reencoded parameter object
     * @return true if the evaluation was successful, or false if it failed.
     */
    virtual bool evaluateChromosome(const int id,
                                    const chromosome& individual,
                                          dvector& fitness,
				          chromosome& suboptimized) const;
    //@}

    /**
     * @name Genetic operations that may be reimplemented
     */
    //@{

    /**
     * Generate an initial individual.
     *
     * You need to reimplement this method only if you have to ensure that the
     * generated individuals have a valid phenotype, i.e. that the chromosome
     * binary representation in "genotype" has an equivalent parameter object
     * for the class you are using.
     *
     * The default implementation just fills the genotype with random bits.
     *
     * @param index index given to facilitate the creation of deterministic
     * individuals.  
     * @param genotype chain of bits.
     *
     * Several genetic algorithms require initial individuals which could be
     * random (like the default implementation of the method), or that may be
     * some special individuals.  Overloading this method you can create
     * deterministic individuals depending on the given index, or just ensure
     * with the index that some parameters settings are initially considered.
     *
     * Return true if successful, false otherwise.
     */
    virtual bool initIndividual(const int index,chromosome& genotype) const;

    /**
     * Generate a random individual.
     *
     * You need to reimplement this method only if you have to ensure that
     * the generated random individuals have a valid phenotype, i.e. that
     * the chromosome binary representation in "genotype" has an equivalent
     * parameter object for the class you are using.
     *
     * The default implementation just fills the genotype with random bits.
     *
     * @param genotype chain of bits.
     *
     * Return true if successful, false otherwise.
     */
    virtual bool randomIndividual(chromosome& genotype) const;

    /**
     * Mutate the given chromosome.
     *
     * This should be reimplemented only if you have to ensure that the
     * mutation is a valid phenotype.
     *
     * The default implementation flips the bits with the probability
     * given in the arguments.
     */
    virtual bool mutate(const chromosome& parent,
                              chromosome& mutant,
                        const double& mutationRate) const;

    /**
     * Crossover between two chromosomes.
     * 
     * This should be reimplemented only if you have to ensure that the
     * crossover produces a valid phenotype.
     *
     * The default implementation does a so called uniform crossover, in which
     * each pair of corresponding bits are exchanged with a probability of
     * 0.5, followed by the mutation indicated in the original PESA paper.
     */
    virtual bool crossover(const chromosome& parent1,
                           const chromosome& parent2,
                                 chromosome& child,
                           const double& mutationRate) const;

    //@}

    /**
     * @name Conversion Tools
     *
     * Converting between phenotypes and binary chains representing chromosomes
     * can become a tedious task.  These static tools should facilitate the
     * process.
     */
    //@{
    /**
     * Convert binary chain into an integer representation.
     *
     * The lower indices in the chain represent the MSB of the number.
     * 
     * @param chain the binary chain representing a chromosome
     * @param startBit index of the bit, at which the integer begins
     * @param bitLength number of bits used for this integer (never greater
     *                  than 32).
     * @param result integer value read from the chain
     * @return the next valid index in the chain, chain.size() if it was
     *         completely read, or -1 if the request goes beyond the 
     *         size of the chain.
     */
    static int binToInt(const chromosome& chain,
                        const int startBit,
                        const int bitLength,
                        int32& result);

    /**
     * Convert binary chain into an integer representation.
     *
     * The lower indices in the chain represent the MSB of the number.
     * 
     * If the given value range is higher than the range representable with
     * the given number of bits, only the lowLimit will be considered.  If
     * the read number has a higher range than the high-low one, then the 
     * modulo operation will be applied to keep the values in range.
     * 
     * @param chain the binary chain representing a chromosome
     * @param startBit index of the bit, at which the integer begins
     * @param bitLength number of bits used for this integer (never greater
     *                  than 32).
     * @param lowLimit lowest value (inclusive) that the stored variable
     *                 can take.
     * @param highLimit highest value (inclusive) that the stored variable
     *                  can take.
     * @param result integer value read from the chain
     * @return the next valid index in the chain, chain.size() if it was
     *         completelly read, or -1 if the request goes beyond the 
     *         size of the chain.
     */
    static int binToInt(const chromosome& chain,
                        const int startBit,
                        const int bitLength,
                        const int lowLimit,
                        const int highLimit,
                        int32& result);


    /**
     * Convert binary chain into an unsigned integer representation.
     *
     * The lower indices in the chain represent the MSB of the number.
     * 
     * @param chain the binary chain representing a chromosome
     * @param startBit index of the bit, at which the integer begins
     * @param bitLength number of bits used for this integer (never greater
     *                  than 32).
     * @param result integer value read from the chain
     * @return the next valid index in the chain, chain.size() if it was
     *         completelly read, or -1 if the request goes beyond the 
     *         size of the chain.
     */
    static int binToUInt(const chromosome& chain,
                         const int startBit,
                         const int bitLength,                  
                         uint32& result);

    /**
     * Convert binary chain into an unsigned integer representation.
     *
     * The lower indices in the chain represent the MSB of the number.
     * 
     * @param chain the binary chain representing a chromosome
     * @param startBit index of the bit, at which the integer begins
     * @param bitLength number of bits used for this integer (never greater
     *                  than 32).
     * @param lowLimit lowest value (inclusive) that the stored variable
     *                 can take.
     * @param highLimit highest value (inclusive) that the stored variable
     *                  can take.
     * @param result integer value read from the chain
     * @return the next valid index in the chain, chain.size() if it was
     *         completelly read, or -1 if the request goes beyond the 
     *         size of the chain.
     */
    static int binToUInt(const chromosome& chain,
                         const int startBit,
                         const int bitLength,                  
                         const uint32 lowLimit,
                         const uint32 highLimit,
                         uint32& result);

    /**
     * Convert binary chain into an double floating point representation.
     *
     * The binarization of floating point values for "genetic" manipulation
     * can easily result in non-sense values.  Therefore here a specified
     * value range is quantized in the desired number of bits between 0 and
     * 31.
     *     
     * @param chain the binary chain representing a chromosome
     * @param startBit index of the bit, at which the integer begins.
     * @param bitLength number of bits used for this double number
     *                  (never greater or equal than 32).
     * @param lowLimit lowest value (inclusive) that the stored variable
     *                 can take.
     * @param highLimit highest value (inclusive) that the stored variable
     *                  can take.
     * @param result double value read from the chain
     * @return the next valid index in the chain, chain.size() if it was
     *         completelly read, or -1 if the request goes beyond the 
     *         size of the chain.
     */
    static int binToDouble(const chromosome& chain,
                           const int startBit,
                           const int bitLength,
                           const double& lowLimit,
                           const double& highLimit,
                           double& result);

    /**
     * Convert integer value in a binary change.
     *
     * @param value the value to be stored.
     * @param startBit starting index in the chain, where the representation
     *                 has to be stored.
     * @param bitLength number of bits used to represent the number.  If it
     *                  is not possible, then -1 will be returned.
     * @param chain binary change with the resulting representation.
     * @return the next valid index in the chain, where further data can
     *         be inserted.  If there is not enough space, -2 will be returned
     */
    static int intToBin(const int value,
                        const int startBit,
                        const int bitLength,
                              chromosome& chain);

    /**
     * Convert integer value in a binary change.
     *
     * @param value the value to be stored.
     * @param startBit starting index in the chain, where the representation
     *                 has to be stored.
     * @param bitLength number of bits used to represent the number.  If it
     *                  is not possible, then -1 will be returned.
     * @param chain binary change with the resulting representation.
     * @param lowLimit lowest value (inclusive) that the stored variable
     *                 can take.
     * @param highLimit highest value (inclusive) that the stored variable
     *                  can take.
     * @return the next valid index in the chain, where further data can
     *         be inserted.  If there is not enough space, -2 will be returned
     */
    static int intToBin(const int value,
                        const int startBit,
                        const int bitLength,
                        const int lowLimit,
                        const int highLimit,
                              chromosome& chain);

    /**
     * Convert integer value in a binary change.
     *
     * @param value the value to be stored.
     * @param startBit starting index in the chain, where the representation
     *                 has to be stored.
     * @param bitLength number of bits used to represent the number.  If it
     *                  is not possible, then -1 will be returned.
     * @param chain binary change with the resulting representation.
     * @return the next valid index in the chain, where further data can
     *         be inserted.  If there is not enough space, -2 will be returned
     */
    static int uintToBin(const uint32 value,
                         const int startBit,
                         const int bitLength,
                               chromosome& chain);

    /**
     * Convert unsigned integer value in a binary change.
     *
     * @param value the value to be stored.
     * @param startBit starting index in the chain, where the representation
     *                 has to be stored.
     * @param bitLength number of bits used to represent the number.  If it
     *                  is not possible, then -1 will be returned.
     * @param chain binary change with the resulting representation.
     * @param lowLimit lowest value (inclusive) that the stored variable
     *                 can take.
     * @param highLimit highest value (inclusive) that the stored variable
     *                  can take.
     * @return the next valid index in the chain, where further data can
     *         be inserted.  If there is not enough space, -2 will be returned
     */
    static int uintToBin(const uint32 value,
                         const int startBit,
                         const int bitLength,
                         const uint32 lowLimit,
                         const uint32 highLimit,
                               chromosome& chain);

    /**
     * Convert double value in a binary change.
     * 
     * @param value value to be quantized.
     * @param startBit starting index in the chain, where the representation 
     *                 has to be stored.
     * @param bitLength number of bits used to represent the number.  If it
     *                  is not possible, then -1 will be returned.
     * @param lowLimit lowest value (inclusive) that the stored variable
     *                 can take.
     * @param highLimit highest value (inclusive) that the stored variable
     *                  can take.
     * @param chain binary change with the resulting representation.
     */
    static int doubleToBin(const double& value,
                           const int startBit,
                           const int bitLength,
                           const double& lowLimit,
                           const double& highLimit,
                                 chromosome& chain);
    
    //@}

  protected:
    /**
     * Return the gray code of the given number
     */
    static inline uint32 grayCode(const uint32 i);

    /**
     * Return the integer value corresponding to the given gray code
     */
    static inline uint32 iGrayCode(const uint32 g);

    /**
     * Random number generator
     */
    mutable univariateContinuousDistribution rnd_;

  };

}

#endif
