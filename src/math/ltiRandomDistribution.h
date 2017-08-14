/*
 * Copyright (C) 2007
 * Pablo Alvarado
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
 * \file   ltiRandomDistribution.h
 *         Contains the class lti::randomDistribution, which is the parent
 *         class of all random distributions in the library.
 * 
 * \author Pablo Alvarado
 * \date   21.09.2007
 *
 * revisions ..: $Id: ltiRandomDistribution.h,v 1.4 2010-04-22 03:16:09 alvarado Exp $
 */

#ifndef _LTI_RANDOM_DISTRIBUTION_H_
#define _LTI_RANDOM_DISTRIBUTION_H_

#include "ltiFunctor.h"
#include <cstdio>

namespace lti {

  /**
   * Class randomDistribution
   *
   * The class lti::randomDistribution is the parent class of all random number
   * generators in the LTI-Lib.
   *
   * The concept followed in this library is complementary to the standard
   * concept found in the STL \<random\> classes, which are employed within the
   * classes of the present hierarchy.  Since the std::tr1 classes are not
   * implemented by all STL implementations, the LTI-Lib provides the required
   * partial implementations for internal use only.
   *
   * Note that the standard random number generators and distributions are
   * highly template based, what makes them very efficient for execution time,
   * but relatively hard to be dinamically configured.  The present hierarchy
   * is not intended as replacement of the standard concepts. It simply is a
   * wrapper concept that allows the number generators to be configured
   * dinamically, through the accustomed parameter objects, at the cost of
   * reduced speed.  In case you need the ultimative speed, then use the
   * standard concept instead, or the original Boost Random Number Library
   * (http://www.boost.org/libs/random/index.html), on which the standard
   * relies.  If you need runtime configuration capabilities, then the LTI-Lib
   * concept is for you.
   *
   * For image applications 32 bit precision is still sufficient.  Hence, that
   * is the precision you will get by the number generation of these
   * distributions.  This means that internally, the uniformly distributed
   * number generator will produce numbers of type lti::uint32.
   *
   * @see randomDistribution::parameters.
   *
   * @ingroup gRandom
   */
  class randomDistribution : public functor {
  public:
    /**
     * Enumerator with the possible underlying uniformly distributed number
     * generators.
     */
    enum eNumberGenerator {
      SystemWide, /**< The system-wide random generator makes use of the old
                   *   C-style rand() function, which is shared among all
                   *   instances of classes derived from randomDistribution.
                   */
      Stochastic, /**< The stochastic method makes use of the device random
                   *   generator, which is usually in "/dev/urandom".  It is a
                   *   non-deterministic random number generator, usually
                   *   implemented in the operating system, using highly 
                   *   entropic processes.  
                   *   Since it uses file I/O it is relatively slow.
                   */
      MinStd,     /**< Linear congruential generator which uses \f$x_{n+1} = (
                   *   a x_{n} ) \mod m \f$ con \f$ a=16807 \f$ y \f$ m =
                   *   2147483647 \f$.  Therefore, the generated numbers are
                   *   unsigned of 31 bit precision, and since \a m and \a c
                   *   are relative primes, the repetition period equals \a m.
                   */
      
      MinStd0,    /**< Linear congruential generator which uses \f$x_{n+1} = (
                   *   a x_{n} ) \mod m \f$ con \f$ a=48271 \f$ y \f$ m =
                   *   2147483647 \f$.  Therefore, the generated numbers are
                   *   unsigned of 31 bit precision, and since \a m and \a c
                   *   are relative primes, the repetition period equals \a m.
                   */
      MT          /**< Mersenne Twister random number generator.  The code for
                   *   this generator is based on
                   *   M. Matsumoto and T. Nishimura, "Mersenne Twister:
                   *   A 623-Dimensionally Equidistributed Uniform
                   *   Pseudo-Random Number Generator", ACM Transactions on 
                   *   Modeling and Computer Simulation, Vol. 8, No. 1, 
                   *   January 1998, pp 3-30.
                   *
                   *   Check also the page 
                   *   http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/emt.html
                   */
    };

    /**
     * The parameters for the class randomDistribution
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
       * Returns the complete name of the parameters class.
       */
      virtual const std::string& name() const;

      /**
       * Returns a pointer to a clone of the parameters.
       */
      virtual parameters* clone() const;

      /**
       * Returns a pointer to a new instance of the parameters.
       */
      virtual parameters* newInstance() const;

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
       * Type of number generator to be used.
       *
       * Default value: MT
       */
      eNumberGenerator generator;

      /**
       * Seed for the number generator.
       *
       * The seed is not allowed to be 0.  In that case, no seed setting
       * will be done.
       *
       * Default value: 15485863 (the 1 millionth prime number)
       */
      uint32 seed;

      /**
       * Name of the device for the stochastic number generator.
       *
       * This is used only if generator=Stochastic.  In case the
       * given device does not exist, setting the parameters will fail, and
       * the corresponding status string will be set accordingly.
       *
       * Default value: "/dev/urandom"
       */
      std::string device;

    };

    /**
     * Default constructor
     */
    randomDistribution();

    /**
     * Construct a functor using the given parameters
     */
    randomDistribution(const parameters& par);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    randomDistribution(const randomDistribution& other);

    /**
     * Destructor
     */
    virtual ~randomDistribution();

    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    randomDistribution& copy(const randomDistribution& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    randomDistribution& operator=(const randomDistribution& other);

    /**
     * Returns the complete name of the functor class
     */
    virtual const std::string& name() const;

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual randomDistribution* clone() const;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual randomDistribution* newInstance() const;

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;
    
    /**
     * Update the parameters
     */
    bool updateParameters();

    /**
     * Shortcut for changing the seed of the number generator
     *
     * @param newSeed new seed for the number generator.
     */
    void setSeed(const uint32 newSeed);

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

  protected:

    /**
     * Returns read-writable reference to the internal parameters
     */
    parameters& getRWParameters();


    /**
     * Pure virtual nested class base of all random number generators.
     *
     * It does not need copy constructors, since the updateParameters of each
     * new functor will create a new instance of the correct rndBase class.
     */
    class rndBase {
    public:
      /**
       * Destructor
       */
      virtual ~rndBase();

      /**
       * Get a single number
       */
      virtual uint32 draw() = 0;

      /**
       * Set the seed for the generator
       */
      virtual void setSeed(const uint32 seed) = 0;

      /**
       * Return the maximal obtainable number (inclusive)
       */
      virtual uint32 max() const = 0;

      /**
       * Write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool write(ioHandler& handler,const bool complete=true) const=0;

      /**
       * Read the parameters from the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool read(ioHandler& handler,const bool complete=true)=0;
    };

    /**
     * Wrapper for the \<cstdlib\> rand() function.  Note that this class will
     * interfere with other instances when the seed is set.
     */
    class rndSystem : public rndBase {
    public:
      /**
       * Constructor
       */
      rndSystem();

      /**
       * Destructor
       */
      virtual ~rndSystem();

      /**
       * Get a single number
       */
      virtual uint32 draw();

      /**
       * Set the seed for the generator
       */
      virtual void setSeed(const uint32 seed);

      /**
       * Return the maximal obtainable number (inclusive)
       */
      uint32 max() const;

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
    };
    
    /**
     * Wrapper for \<random\> std::tr1::random_device, which is a hardware based
     * true, unpredictable, random number generator.
     */
    class rndStochastic : public rndBase {
    public:
      /**
       * Constructor
       */
      rndStochastic();

      /**
       * Destructor
       */
      virtual ~rndStochastic();

      /**
       * Get a single number
       */
      virtual uint32 draw();

      /**
       * Set the seed for the generator
       */
      virtual void setSeed(const uint32 seed);

      /**
       * Return the maximal obtainable number (inclusive)
       */
      uint32 max() const;

      /**
       * Set the file device to be used
       */
      bool setDevice(const std::string& device);

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

    protected:
      /**
       * File handler to the device used to generate random numbers
       */
      FILE* handler_;
    };

    /**
     * Linear Congruency Random Number Generator
     */
    class rndMinStd : public rndBase {
    public:
      /**
       * Constructor
       */
      rndMinStd();

      /**
       * Destructor
       */
      virtual ~rndMinStd();

      /**
       * Get a single number
       */
      virtual uint32 draw();

      /**
       * Set the seed for the generator
       */
      virtual void setSeed(const uint32 seed);

      /**
       * Return the maximal obtainable number (inclusive)
       */
      uint32 max() const;

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

    protected:
      /**
       * Current state
       */
      uint32 state_;

      /**
       * Constant a
       */
      static const uint32 a_ = 16807;

      /**
       * Constant m
       */
      static const uint32 m_ = 2147483647;

    };

    /**
     * Linear Congruency Random Number Generator
     */
    class rndMinStd0 : public rndBase {
    public:
      /**
       * Constructor
       */
      rndMinStd0();

      /**
       * Destructor
       */
      virtual ~rndMinStd0();

      /**
       * Get a single number
       */
      virtual uint32 draw();

      /**
       * Set the seed for the generator
       */
      virtual void setSeed(const uint32 seed);

      /**
       * Return the maximal obtainable number (inclusive)
       */
      uint32 max() const;

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

    protected:
      /**
       * Current state
       */
      uint32 state_;

      /**
       * Constant a
       */
      static const uint32 a_ = 48271;

      /**
       * Constant m
       */
      static const uint32 m_ = 2147483647;
    };

    /**
     * Mersenne Twister Random Number Generator.
     *
     * Reference:
     * M. Matsumoto and T. Nishimura, "Mersenne Twister: A 623-Dimensionally
     * Equidistributed Uniform Pseudo-Random Number Generator", 
     * ACM Transactions on Modeling and Computer Simulation, Vol. 8, No. 1,
     * January 1998, pp 3-30.
     */
    class rndMersenneTwister : public rndBase {
    public:
      /**
       * Constructor
       */
      rndMersenneTwister();

      /**
       * Destructor
       */
      virtual ~rndMersenneTwister();

      /**
       * Get a single number
       */
      virtual uint32 draw();

      /**
       * Set the seed for the generator
       */
      virtual void setSeed(const uint32 seed);

      /**
       * Return the maximal obtainable number (inclusive)
       */
      uint32 max() const;

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
      
    private:

      // Internal private constants
      static const int    stateSize_ = 624; // __n
      static const int    shiftSize_ = 397; // __m
      static const int    maskBits_  = 31; // __r
      static const uint32 a_         = uint32(0x9908b0dfu);
      static const int    u_         = 11;
      static const int    s_         = 7;
      static const uint32 b_         = uint32(0x9d2c5680u);
      static const int    t_         = 15;
      static const uint32 c_         = uint32(0xefc60000u);
      static const int    l_         = 18;

      /**
       * State of the random generator
       */
      uint32 x_[stateSize_];
      
      /**
       * Pointer within state x_.
       */
      int p_;
    };

    /**
     * Pointer to the random number generator for float.
     */
    rndBase* generator_;

    /**
     * Maximum number obtainable in the generator
     */
    uint32 max_;

  };

  /**
   * Read the resize mode
   *
   * @ingroup gStorable
   */
  bool read(ioHandler& handler,randomDistribution::eNumberGenerator& data);

  /**
   * Write the resize mode
   *
   * @ingroup gStorable
   */
  bool write(ioHandler& handler,
             const randomDistribution::eNumberGenerator& data);
  
}

#endif

