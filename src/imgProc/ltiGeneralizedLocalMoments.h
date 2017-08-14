/*
 * Copyright (C) 1998 - 2005
 * Lehrstuhl fuer Technische Informatik, RWTH-Aachen, Germany
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
 * \file   ltiGeneralizedLocalMoments.h
 *         This file contains the definition of the class
 *         generalizedLocalMoments.
 * \author Arnd Hannemann
 * \date   27.05.2005
 *
 * $Id: ltiGeneralizedLocalMoments.h,v 1.12 2009-08-31 13:53:37 alvarado Exp $
 */



#ifndef _LTI_GENERALIZED_LOCAL_MOMENTS_H_
#define _LTI_GENERALIZED_LOCAL_MOMENTS_H_

#include "ltiLocalDescriptorExtraction.h"

namespace lti {

  /**
   * Generalized local color moments.
   *
   * This class implements a local feature extractor which uses several
   * different invariants based on generalized color moments described in the
   * paper "Moment invariants for recognition under changing viewpoint and
   * illumination" by Mindru et al.
   *
   * The generalized color moment \f$ M_{pq}^{abc} \f$ is defined by 
   * \f[ 
   * M_{pq}^{abc} =\int\!\!\!\int_{\Omega} x^py^q[R(x,y)]^a[G(x,y)]^b[B(x,y)]^c
   * \f]
   *
   * \f$ M_{pq}^{abc} \f$ is said to be a (generalized color) moment of 
   * \e order  p + q and \e degree a + b + c.
   *
   * Furthermore \f$ S_{cd} \f$ 
   * stands for 1-band invariants and \f$ D_{cd} \f$ for 2-band 
   * invariants of order c, and degree d.
   * 
   * At the moment there are five different types of invariants this functor
   * can calculate:
   *
   * - \b GPD geometric photometric invariants (Type D: diagonal) 
   *      - Those are photometric invariant against independent 
   *         scaling of each color band.
   *      - There are 24 of them and they will be stored in the following 
   *        order:
   *        \f[
   *         \begin{aligned}
   *         & S_{02}^{(R)}, S_{12}^{(R)}, 
   *           S_{02}^{(G)}, S_{12}^{(G)}, 
   *           S_{02}^{(B)}, S_{12}^{(B)}, \\
   *         & D_{11}^{(RG)}, D_{12}^{1(RG)}, D_{12}^{2(RG)}, D_{12}^{3(RG)},
   *           D_{12}^{4(RG)}, \\
   *         & D_{11}^{(RB)}, D_{12}^{1(RB)}, D_{12}^{2(RB)}, D_{12}^{3(RB)},
   *           D_{12}^{4(RB)} \\
   *         & D_{11}^{(GB)}, D_{12}^{1(GB)}, D_{12}^{2(GB)}, D_{12}^{3(GB)},
   *           D_{12}^{4(GB)}
   *         \end{aligned}
   *        \f]
   *      - There are only 21 independent invariants in the basis, which
   *        can be optained by removing the following three invariants: 
   *        \f$ D_{12}^{3(RB)}, D_{12}^{4(RG)}, D_{12}^{4(GB)} \f$
   *        (setting parameters::independent = true does exactly this)
   *        
   * - \b GPSO geometric photometric invariants (Type SO: scaling and offset)
   *      - Those are photometric invariant against independent 
   *        scaling of each color channel and an offset
   *      - There are 21 of them and they will be stored in the following
   *        order:
   *        \f[
   *         \begin{aligned}
   *          &S_{12}^{(R)},   S_{12}^{(G)},   S_{12}^{(B)}, 
   *           S_{12}^{(R)},   S_{12}^{(G)},   S_{12}^{(B)},  \\
   *          &D_{11}^{(RG)},  D_{12}^{1(RG)}, D_{12}^{2(RG)},
   *           D_{12}^{3(RG)}, D_{12}^{4(RG)}, D_{12}^{5(RG)}, \\
   *          &D_{11}^{(RB)},  D_{12}^{1(RB)}, D_{12}^{2(RB)},
   *           D_{12}^{3(RB)}, D_{12}^{4(RB)}, D_{12}^{5(RB)}, \\
   *          &D_{11}^{(GB)},  D_{12}^{1(GB)}, D_{12}^{2(GB)},
   *           D_{12}^{3(GB)}, D_{12}^{4(GB)}, D_{12}^{5(GB)}
   *         \end{aligned}
   *        \f]
   *      - There are only 18 independent invariants in the basis, which
   *        can be optained by removing the following three invariants: 
   *        \f$ D_{12}^{2(RG)}, D_{12}^{2(GB)}, D_{12}^{3(RB)} \f$
   *        (setting parameters::independent = true does exactly this)
   *
   * - \b PSO photometric invariants (not yet implemented)
   *      Those are meant for cases when no geometric deformations are present
   *      and the photometric transformations are of Type SO
   * - \b PSOx or PSOS (not yet implemented)
   *      numerical stabilized version of PSO
   * - \b PAFF (Type AFF: affine) (not yet implemented)
   *      Those are meant for cases when no geometric deformations are present
   *      and the photometric transformations are of Type AFF
   * - \b noGP invariants
   *      - Those are moment invariants used for comparing patterns
   *        within regions after normalization agains geometric and
   *        photometric deformations (see "Matching Widely Separeted
   *        Views Based on Affine Invariant Regions" by Tytelaars and Gool)
   *      - There are 18 of them and they will be stored in the following
   *        order:
   *        \f[
   *        \begin{aligned}
   *          & D_{02}^{(RG)}, D_{02}^{(GB)}, D_{02}^{(RB)},
   *            S_{11}^{1(R)}, S_{11}^{(G)},  S_{11}^{(B)},  \\
   *          & S_{11}^{2(R)}, S_{11}^{(G)},  S_{11}^{(B)}, 
   *            S_{21}^{1(R)}, S_{21}^{1(G)}, S_{21}^{1(B)}, \\
   *          & S_{21}^{2(R)}, S_{21}^{2(G)}, S_{21}^{2(B)},
   *            S_{21}^{3(R)}, S_{21}^{3(G)}, S_{21}^{3(B)}
   *        \end{aligned}
   *        \f]
   *
   * - \b Rot invariants
   *      - Those are moment invariants used for comparing patterns
   *        within an elliptic invariant region after normalization
   *        to a reference circular region.
   *      - There are 9 of them and they will be stored in the following
   *        order:
   *        \f[
   *          D_{02}^{(RG)}, D_{02}^{(RB)}, D_{02}^{(GB)},
   *          S_{11}^{(R)}, S_{21}^{(R)},
   *          S_{11}^{(G)}, S_{21}^{(G)},
   *          S_{11}^{(B)}, S_{21}^{(B)}
   *        \f]
   *
   * \b Note: in fact there were some more moments used in the formulas
   * for above invariants. But for now this functor only works for symmetric
   * regions around given locations and \f$ M_{01}^{00} = M_{10}^{00} = 0 \f$
   * for those shapes.
   *
   * <H2>Formulas of GPD invariants for symmetric shapes</H2>
   * \f$ S_{02} = \dfrac{M_{00}^2*M_{00}^0}{(M_{00}^1)^2} \f$
   * \f$ S_{12} = \dfrac{M_{10}^1*M_{01}^2-M_{10}^2*M_{01}^1}
   * {M_{00}^2*M_{00}^1}  \f$
   *
   * \f$ D_{02} = \dfrac{M_{00}^{11} \cdot M_{00}^{00}}
   *  {M_{00}^{10} \cdot M_{00}^{01}} \f$
   * \f$ D_{11} = \dfrac{M_{10}^{10} \cdot M_{01}^{01}
   *  -M_{10}^{01} \cdot M_{01}^{10}}
   *  {M_{00}^{01} \cdot M_{00}^{10}} \f$
   *
   * \f$ D_{12}^1 = \dfrac{M_{10}^{10} \cdot M_{01}^{11}
   * -M_{01}^{10} \cdot M_{10}^{11}}
   * {M_{00}^{11} \cdot M_{00}^{10}} \f$
   * \f$ D_{12}^2 = \dfrac{M_{10}^{01} \cdot M_{01}^{11}
   * -M_{01}^{01} \cdot M_{10}^{11}}
   * {M_{00}^{01} \cdot M_{00}^{11}} \f$
   *
   * \f$ D_{12}^3 = \dfrac{M_{10}^{10} \cdot M_{01}^{02}
   * -M_{10}^{02} \cdot M_{01}^{10}}
   * {M_{00}^{02} \cdot M_{00}^{10}} \f$
   * \f$ D_{12}^4 = \dfrac{M_{10}^{20} \cdot M_{01}^{01}
   * -M_{10}^{01} \cdot M_{01}^{20}}
   * {M_{00}^{01} \cdot M_{00}^{20}} \f$
   *
   * \f$ M^i_{pq} \f$ stands for either \f$ M^{i00}_{pq} \f$, 
   * \f$ M^{0o0}_{pq} \f$ or \f$ M^{i00}_{pq} \f$ , depending
   * on which color band is used; 
   * \f$ M^{ij}_{pq} \f$ , stands for either \f$ M^{ij0}_{pq} \f$ ,
   * \f$ M^{i0j}_{pq} \f$ or \f$ M^{0ij}_{pq} \f$ , depending
   * on which 2 of the 3 color bands are used
   * 
   * <H2>Formulas of GPSO invariants for symmetric shapes</H2>
   * \f$ S_{12} = \dfrac{M_{01}^2 M_{10}^1 - M_{01}^2 M_{10}^1}
   * {M_{00}^0 [M_{00}^2 M_{00}^0 - (M_{00}^1)^2]^3}\f$
   *
   * \f$ D_{12}^1 = \dfrac {[ M_{10}^{10} M_{01}^{01} 
   * - M_{01}^{10} M_{10}^{01}]^2}
   * { (M_{00}^{00})^2 [M_{00}^{20} M_{00}^{00} -(M_{00}^{10})^2]
   *   [M_{00}^{02} M_{00}^{00} -(M_{00}^{01})^2] } \f$
   *
   * \f$ D_{12}^2 = \dfrac {[M_{10}^{02} M_{01}^{01} M_{00}^{00}
   * - M_{01}^{20} M_{10}^{01} M_{00}^{00}
   * + 2M_{01}^{10} M_{10}^{01} M_{00}^{10}
   * - 2M_{10}^{10} M_{01}^{01} M_{00}^{10}]^2}
   * { (M_{00}^{00})^2 [M_{00}^{20} M_{00}^{00} -(M_{00}^{10})^2]
   *   [M_{00}^{02} M_{00}^{00} -(M_{00}^{01})^2]^2 } \f$
   *
   * \f$ D_{12}^3 = \dfrac {[M_{10}^{02} M_{01}^{10} M_{00}^{00}
   * - M_{01}^{02} M_{10}^{10} M_{00}^{00}
   * + 2M_{10}^{10} M_{01}^{01} M_{00}^{01}
   * - 2M_{10}^{01} M_{01}^{10} M_{00}^{01}]^2}
   * { (M_{00}^{00})^2 [M_{00}^{20} M_{00}^{00} -(M_{00}^{10})^2]^2
   *   [M_{00}^{02} M_{00}^{00} -(M_{00}^{01})^2] } \f$
   *
   * \f$ D_{12}^4 = \dfrac {[M_{10}^{11} M_{01}^{10} M_{00}^{00}
   * - M_{01}^{11} M_{10}^{10} M_{00}^{00}
   * + M_{10}^{10} M_{01}^{01} M_{00}^{10}
   * - M_{10}^{01} M_{01}^{10} M_{00}^{10}]^2}
   * { (M_{00}^{00})^2 [M_{00}^{20} M_{00}^{00} -(M_{00}^{10})^2]
   *   [M_{00}^{02} M_{00}^{00} -(M_{00}^{01})^2]^2 } \f$
   *
   * \f$ D_{12}^5 = \dfrac {[M_{10}^{11} M_{01}^{01} M_{00}^{00}
   * - M_{01}^{11} M_{10}^{01} M_{00}^{00}
   * - M_{10}^{10} M_{01}^{01} M_{00}^{01}
   * + M_{10}^{01} M_{01}^{10} M_{00}^{01}]^2}
   * { (M_{00}^{00})^2 [M_{00}^{20} M_{00}^{00} -(M_{00}^{10})^2]^2
   *   [M_{00}^{02} M_{00}^{00} -(M_{00}^{01})^2] } \f$
   *
   * <H2> Formulas of noGP Invariants </H2>
   * 
   * \f$ D_{02} = \dfrac {M_{00}^{11}}{M_{00}^{00}} \f$
   * \f$ S_{11}^1 = \dfrac {M_{10}^1}{M_{00}^1} \f$
   * \f$ S_{11}^2 = \dfrac {M_{01}^1}{M_{00}^1} \f$
   *
   * \f$ S_{21}^1 = \dfrac {M_{11}^1}{M_{00}^1} \f$
   * \f$ S_{21}^2 = \dfrac {M_{20}^1}{M_{00}^1} \f$
   * \f$ S_{21}^3 = \dfrac {M_{02}^1}{M_{00}^1} \f$
   *
   * <H2> Formulas of Rot Invariants </H2>
   *
   * \f$ D_{02} = \dfrac {M_{00}^{11}}{M_{00}^{00}} \f$
   * \f$ S_{11} = \dfrac {\sqrt{(M_{10}^1)^2+(M_{01}^1)^2}}
   * {M_{00}^1} \f$
   * \f$ S_{21} = \dfrac {\sqrt{M_{20}^1+M_{02}^1}}
   * {M_{00}^0 M_{00}^1} \f$
   */
  class generalizedLocalMoments : public localDescriptorExtraction {
  public:


    /**
     * Accumulator which is used with LocalSampling to calculate several
     * common generalized color moments.x
     */
    template <typename T>
    class noGPaccumulator {
          public:
      
      /**
       * Default Constructor
       */
      noGPaccumulator();    
      
      /**
       * Default Destructor
       */
      ~noGPaccumulator();

      /**
       * Copy constructor.
       * @param other the accumulator object to be copied
       */
      noGPaccumulator(const noGPaccumulator<T>& other);


      /**
       * Copy data of "other" accumulator.
       *
       * @param other the accumulator to be copied
       * @return a reference to this accumulator object
       */
      inline noGPaccumulator<T>& 
      copy(const noGPaccumulator<T>& other);
      
      
      /**
       * Type of the accumulator
       */
      typedef T value_type;
      
      /**
       * Accumulate function.
       */
      inline bool accumulate(const T& value, 
                             const float posx,
                             const float posy); 

      /**
       * Accumulates a given value
       * @param value value to accumulate
       * @param weight the value will weighted with that
       * @param posx x-position of the value (not used here)
       * @param posy y-position of the value (not used here)
       */
      inline bool accumulate(const T& value, 
                             const float weight,
                             const float posx, const float posy); 
    
              
      /** 
       * Resets all stored information
       */
      inline bool reset();

      /**      
       * Returns the stored information.
       */
      inline bool getResult(double result[]);

      /**
       * Return size of the resulting array
       */
      inline static int getArraySize();
      
    private:
      /**
       * Array in which the results are stored 
       */
      double* m_;
    };

    /** 
     * Accumulator which is used with LocalSampling to calculate
     * several common generalized color moments.
     */ 
    template <typename T>
    class accumulator {
    public:
      
      /**
       * Default Constructor
       */
      accumulator();    
      
      /**
       * Default Destructor
       */
      ~accumulator();

      /**
       * Copy constructor.
       * @param other the accumulator object to be copied
       */
      accumulator(const accumulator<T>& other);

      /**
       * Copy data of "other" accumulator.
       *
       * @param other the accumulator to be copied
       * @return a reference to this accumulator object
       */
      inline accumulator<T>& 
      copy(const accumulator<T>& other);


      /**
       * Type of the accumulator
       */
      typedef T value_type;
      
      /**
       * Accumulate function.
       */
      inline bool accumulate(const T& value, 
                             const float posx, 
                             const float posy); 
      
      /**
       * Accumulates a given value
       * @param value value to accumulate
       * @param weight the value will weighted with that
       * @param posx x-position of the value (not used here)
       * @param posy y-position of the value (not used here)
       */
      inline bool accumulate(const T& value, 
                             const float weight,
                             const float posx, const float posy);
              
      /** 
       * Resets all stored information
       */
      inline bool reset();

      /**      
       * Returns the stored information.
       */
      inline bool getResult(double result[]);

      /**
       * Return size of the resulting array
       */
      inline static int getArraySize();

    private:  
      /** 
       * Array in which the results are stored 
       */
      double* m_;
    };
    
    /**
     * The parameters for the class generalizedLocalMoments
     */
    class parameters : public localDescriptorExtraction::parameters {
    public:

      /**
       * Enumeration of different types of invariants.
       */
      enum eInvariantType {
        NoGP,   /**< 18 simple invariants that can be used when 
                 * the image region is already normalized
                 * with respect to geometric and photometric 
                 * deformations.
                 */
        Rot,    /**< 9 moment invariants used for comparing patterns
                 * within an elliptic invariant region after normalization
                 * to a reference circular region.
                 */
        GPD,    /**< 24 (21 independent) 
                 * Geometric/Photometric invariants (GPD Type)
                 */
        GPSO    /**< 21 (18 independent)
                 *  Geometric/Photometric invariants (GPSO Type)
                 */      
       
      };


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
       * Returns the name of this type.
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
       * Which type of invariants should be calculated.
       *
       * Default: GPD
       */
      eInvariantType invariantType;


      /**
       * If only independent invariants should be returned.
       * (ignored for simple invariants)
       *
       * Default: true
       */
      bool independent;


    };

    /**
     * Default constructor
     */
    generalizedLocalMoments();

    /**
     * Construct a functor using the given parameters
     */
    generalizedLocalMoments(const parameters& par);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    generalizedLocalMoments(const generalizedLocalMoments& other);

    /**
     * Destructor
     */
    virtual ~generalizedLocalMoments();

    /**
     * Returns the name of this type.
     */
    virtual const std::string& name() const;

    /**
     * Returns a pointer to a clone of the functor.
     */
    virtual generalizedLocalMoments* clone() const;
    
    /**
     * Returns a pointer to a new instance of the functor.
     */
    virtual generalizedLocalMoments* newInstance() const;

  
    // uses apply of parent class
    using localDescriptorExtraction::apply;


    /**
     * Computes the feature vector at the given location in the image.
     *
     * @param img input image
     * @param loc location which should be examined
     * @param result here the resulting feature vector will be stored.
     * @return true upon success, false otherwise
     */
    bool apply(const matrix<rgbaPixel>& img, 
               const location& loc, dvector& result) const;

    /**
     * Apply for a normalized local region. The \a img is expected to
     * be small. A circular region in the middle of \a img is used for
     * the calculation.
     *
     * @param img local region extracted from an image
     * @param result descriptor vector
     * @return true upon success, false otherwise
     */
    bool apply(const matrix<rgbaPixel>& img, dvector& result) const;


    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    generalizedLocalMoments& copy(const generalizedLocalMoments& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    generalizedLocalMoments& operator=(const generalizedLocalMoments& other);


    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;


  protected:
    /**
     * Helper function to calculate GPD invariants
     * @param momentArray filled with calculated moments
     * @param par shortcut to parameters object
     * @param result there the resulting invariants will be stored
     */
    bool calculateGPDInvariants(const double momentArray[], 
                                const parameters& par,
                                dvector& result) const;

    /**
     * Helper function to calculate GPSO invariants
     * @param momentArray filled with calculated moments
     * @param par shortcut to parameters object
     * @param result there the resulting invariants will be stored
     */
    bool calculateGPSOInvariants(const double momentArray[],
                                 const parameters& par,
                                 dvector& result) const;

    /**
     * Helper function to calculate PSO invariants
     * @param momentArray filled with calculated moments
     * @param par shortcut to parameters object
     * @param result there the resulting invariants will be stored
     */
    bool calculatePSOInvariants(const double momentArray[], 
                                const parameters& par,
                                dvector& result) const;
    /**
     * Helper function to calculate Simple invariants
     * @param momentArray filled with calculated moments
     * @param par shortcut to parameters object
     * @param result there the resulting invariants will be stored
     */
    bool calculateNoGPInvariants(const double momentArray[], 
                                 const parameters& par,
                                 dvector& result) const;


    /**
     * Helper function to calculate Rot invariants
     * @param momentArray filled with calculated moments
     * @param par shortcut to parameters object
     * @param result there the resulting invariants will be stored
     */
    bool calculateRotInvariants(const double momentArray[], 
                                const parameters& par,
                                dvector& result) const;
  };


  /**
   * Accumulator which is used with LocalSampling to calculate several
   * common gernalized color moments.x
   */
  template <>
  class generalizedLocalMoments::noGPaccumulator<rgbaPixel> {
  public:
    
    /**
     * Default Constructor
     */
    noGPaccumulator();    
    
    /**
     * Default Destructor
     */
    ~noGPaccumulator();

    /**
     * Copy constructor.
     * @param other the accumulator object to be copied
     */
    noGPaccumulator(const noGPaccumulator<rgbaPixel>& other);

    /**
     * Copy data of "other" accumulator.
     *
     * @param other the accumulator to be copied
     * @return a reference to this accumulator object
     */
    inline noGPaccumulator<rgbaPixel>& 
    copy(const noGPaccumulator<rgbaPixel>& other);


    /**
     * Type of the accumulator
     */
    typedef rgbaPixel value_type;
    
    /**
     * Accumulates a given value
     * @param value value to accumulate
     * @param posx x-position of the value (not used here)
     * @param posy y-position of the value (not used here)
     */
    inline bool accumulate(const rgbaPixel& value, 
                           const float posx,
                           const float posy); 


    /**
     * Accumulates a given value
     * @param value value to accumulate
     * @param weight the value will weighted with that
     * @param posx x-position of the value (not used here)
     * @param posy y-position of the value (not used here)
     */
    inline bool accumulate(const rgbaPixel& value, 
                           const float weight,
                           const float posx, const float posy);
              
    /** 
     * Resets all stored information
     */
    inline bool reset();
    
    /**      
     * Returns the stored information.
     */
    inline bool getResult(double result[]);
    
    /**
     * Return size of the resulting array
     */
    inline static int getArraySize();
    
    
    // --------------------------------------
    //     DESCRIPTION OF ARRAY INDICES
    // --------------------------------------
    
    enum {
      M_M = 0,
    
      // ------------------- ONE CHANNEL ------------------------
      R_M = 1, // red without coordinates influence
      R_X = 4, // red with x coordinate
      R_Y = 7, // red with y coordinate
      
      G_M = 2, // same in green
      G_X = 5,
      G_Y = 8,
      
      B_M = 3, // same in blue
      B_X = 6,
      B_Y = 9,
      
      R_XY = 10, // red with product of x- and y-coordinate
      R_X2 = 13, // red with squared x-coordinate
      R_Y2 = 16, // red with squared y-coordinate
      
      G_XY = 11, // same in green
      G_X2 = 14,
      G_Y2 = 17,
      
      B_XY = 12, // same in blue
      B_X2 = 15,
      B_Y2 = 18,
      
      // ------------------- TWO CHANNEL -----------------------           
      RG_M = 19,  // red combined with green 
      RB_M = 20,  // same with red and blue
      GB_M = 21   // same with green and blue
    };
    
  private:
    
    /** 
     * Array in which the results are stored 
     */
    double* m_;
    
  };

  /** 
   * Accumulator which is used with LocalSampling to calculate
   * several common generalized color moments. 
   */ 
  template <>
  class generalizedLocalMoments::accumulator<rgbaPixel> {
  public:
    
    /**
     * Default Constructor
     */
    accumulator();    
    
    /**
     * Default Destructor
     */
    ~accumulator();

    /**
     * Copy constructor.
     * @param other the accumulator object to be copied
     */
    accumulator(const accumulator<rgbaPixel>& other);

    /**
     * Copy data of "other" accumulator.
     *
     * @param other the accumulator to be copied
     * @return a reference to this accumulator object
     */
    inline accumulator<rgbaPixel>& 
    copy(const accumulator<rgbaPixel>& other);


    /**
     * Type of the accumulator
     */
    typedef rgbaPixel value_type;
    
    /**
     * Accumulates a given value
     * @param value value to accumulate
     * @param posx x-position of the value (not used here)
     * @param posy y-position of the value (not used here)
     */
    inline bool accumulate(const rgbaPixel& value, 
                           const float posx, 
                           const float posy); 

    /**
     * Accumulates a given value
     * @param value value to accumulate
     * @param weight the value will weighted with that
     * @param posx x-position of the value (not used here)
     * @param posy y-position of the value (not used here)
     */
    inline bool accumulate(const rgbaPixel& value, 
                           const float weight,
                           const float posx, const float posy); 
    
    
    /** 
     * Resets all stored information
     */
    inline bool reset();
    
    /**      
     * Returns the stored information.
     */
    inline bool getResult(double result[]);
    
    /**
     * Return size of the resulting array
     */
    inline static int getArraySize();
    
    
    // --------------------------------------
    //     DESCRIPTION OF ARRAY INDICES
    // --------------------------------------
    enum {
      M_M = 0,
      
      // ------------------- ONE CHANNEL ------------------------
      R_M = 1, // red without coordinates influence
      R_X = 4, // red with x coordinate
      R_Y = 7, // red with y coordinate
      
      G_M = 2, // same in green
      G_X = 5,
      G_Y = 8,
      
      B_M = 3, // same in blue
      B_X = 6,
      B_Y = 9,
      
      R2_M = 10, // square of red without coordinates
      R2_X = 13, // square of red with x coordinate
      R2_Y = 16, // square of red with y coordinate
      
      G2_M = 11, // same in green
      G2_X = 14,
      G2_Y = 17,
      
      B2_M = 12, // same in blue
      B2_X = 15,
      B2_Y = 18,
      
      // ------------------- TWO CHANNEL -----------------------           
      RG_M = 19,  // red combined with green 
      RG_X = 22,
      RG_Y = 25,
      
      RB_M = 20,  // same with red and blue
      RB_X = 23,
      RB_Y = 26,
      
      GB_M = 21,  // same with green and blue
      GB_X = 24,
      GB_Y = 27,
      
      M_X = 28,
      M_Y = 29
    };  
    
  private:
    
    /**
     * Array in which the results are stored 
     */
    double* m_;
    
  };
  
  
} // namespace

// DO NOT include the template here, instead include it in .cpp
// #include "ltiGeneralizedLocalMoments_template.h"

#endif


