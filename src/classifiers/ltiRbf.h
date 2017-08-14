/*
 * Copyright (C) 2000, 2001, 2002, 2003, 2004, 2005, 2006
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
 * \file   ltiRbf.h
 * \author Peter Doerfler
 * \author Pablo Alvarado
 * \date   11.08.2000
 *
 * $Id: ltiRbf.h,v 1.5 2013-02-17 02:22:12 alvarado Exp $
 */


#ifndef _LTI_RBF_H_
#define _LTI_RBF_H_

#include "ltiObject.h"
#include "ltiVector.h"
#include "ltiMatrix.h"
#include "ltiStdLayer.h"
#include "ltiClassifier.h"
#include "ltiSupervisedInstanceClassifier.h"
#include "ltiLinearLeastSquares.h"

#include <map>

namespace lti {

  /**
   * Radial Basis Function networks.
   *
   * This class implements the RBF networks within the LTI-Lib classification
   * framework.
   *
   * The RBF networks are useful when the feature space forms clusters clearly
   * associated with the classes, and can be covered with hyperspheres.
   *
   * This class uses two phases to train the first layer, which in
   * principle finds the positions for the chosen prototypes.  It may
   * suffice to use just one algorithm.
   *
   * Example:
   *
   * \code
   * rbf::parameters rbfPar;
   * rbfPar.nbHiddenNeurons = 5;
   *
   * double inData[] = {    -1,-1,
   *                        -1, 0,
   *                        -1,+1,
   *                        +0,+1,
   *                        +1,+1,
   *                        +1,+0,
   *                        +1,-1,
   *                        +0,-1,
   *                        +0,+0};
   * 
   * lti::dmatrix inputs(9,2,inData);          // training vectors
   * 
   * int idsData[] = {1,0,1,0,1,0,1,0,1}; // and the respective ids
   * lti::ivector ids(9,idsData);
   * 
   * lti::rbf ann(rbfPar);  // our artificial neural network
   *     
   * // we want to see some info while training
   * streamProgressInfo prog(std::cout);
   * ann.setProgressObject(prog);
   * 
   * // train the network
   * ann.train(inputs,ids);
   * 
   * // let us save our network for future use
   * // in the file called rbf.dat
   * std::ofstream out("rbf.dat");
   * lsh.use(out);
   * 
   * // save the network
   * ann.write(lsh);
   * // close the file
   * out.close();
   * 
   * // show some results with the same training set:
   * 
   * lti::rbf::result outv; // here we will get some
   * // classification results
   * std::cout << std::endl << "Results: " << std::endl;
   * 
   * int i,id;
   * for (i=0;i<inputs.rows();++i) {
   *   ann.classify(inputs.getRow(i),outv);
   *   std::cout << "Input " << inputs.getRow(i) << " \tOutput: ";
   *   outv.getId(outv.getWinner(),id);
   *   std::cout << id;
   *   if (id != ids.at(i)) {
   *     std::cout << " <- should be " << ids.at(i);
   *   }
   *   std::cout << std::endl;
   * }
   * \endcode
   *
   * @see rbf::parameters
   */
  class rbf : public supervisedInstanceClassifier {
  public:
    /**
     * Type to specify the kind of initialization for the networks
     */
    enum eLvqInit {
      LvqRand,   /*!< Random initialization of the vector code */
      LvqMaxDist /*!< Initialization with the maximum distance */
    };

    /**
     * The LVQ training algorithm
     */
    enum eLvqTrainType {
      NO_LVQ=-1, /*!< Do not use LVQ */
      LVQ1,      /*!< Use LVQ1       */
      OLVQ1,     /*!< Use OLVQ1      */
      LVQ3,      /*!< Use LVQ3       */
      OLVQ3,     /*!< Use OLVQ3      */
      LVQ4       /*!< Use LVQ4       */
    };

    // ----------------------------------------------------
    // rbf::parameters
    // ----------------------------------------------------

    /**
     * Parameters class for the RBF-Networks
     *
     * @see lti::rbf
     */
    class parameters: public supervisedInstanceClassifier::parameters {

    public:

      /**
       * default constructor
       */
      parameters();

      /**
       * copy constructor
       * @param other the parameters object to be copied
       */
      parameters(const parameters& other);

      /**
       * destructor
       */
      virtual ~parameters();

      /**
       * Returns the name of this class
       */
      virtual const std::string& name() const;

      /**
       * clone function.
       */
      parameters* clone() const;

      /**
       * newInstance function.
       */
      parameters* newInstance() const;

      /**
       * copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& copy(const parameters& other);

      /**
       * Alias for "copy".
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      inline parameters& operator=(const parameters& other) {
        return copy(other);
      }

      /**
       * write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool write(ioHandler& handler,const bool complete=true) const;

      /**
       * read the parameters from the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool read(ioHandler& handler,const bool complete=true);


      // --------------
      // the parameters
      // --------------

      /**
       * @name Classifier parameters
       */
      //@{

      /**
       * Number of presentations of the training patterns to the training
       * method specified with lvqType1 (for the first training phase).
       *
       * nbPresentations1
       *
       * Default value: 0
       */
      int nbPresentations1;

      /**
       * Number of presentations of the training patterns to the training
       * method specified with lvqType2 (for the second training phase).
       *
       * nbPresentations2
       *
       * Default value: 20
       */
      int nbPresentations2;

      /**
       * Number of hidden neurons used for each output class.
       *
       * The output neuron will fire if any ocf its associated hidden
       * neurons fires.
       *
       * Default value: 1
       */
      int nbHiddenNeurons;

      /**
       * Learning rate used with the first LVQ training phase.
       *
       * The learning rate is the factor used to compute the displacement
       * of the prototypes with respect to the distance between the
       * winner prototype and a presented prototype.
       * 
       * Default value: 0.3
       */
      double learnRate1;

      /**
       * Learning rate used with the second LVQ training phase.
       *
       * The learning rate is the factor used to compute the displacement
       * of the prototypes with respect to the distance between the
       * winner prototype and a presented prototype.
       * 
       * Default value: 0.1
       */
      double learnRate2;

      /**
       * LVQ learn factor
       *
       * The learnFactor is multiplied with the result of multiplying
       * the learnRate1 (or 2) with the distance between prototypes
       * and training patterns.
       *
       * Default value: 0.3
       */
      double learnFactor;

      /**
       * LVQ window size
       *
       * Only the prototypes within a window of the given radius will
       * be affected in the training.
       *
       * Default value: 0.2
       */
      double windowSize;

      /**
       * Sigma Factor
       *
       * Each (gaussian) radial basis function associated with a
       * prototype of the first layer manages its own standard deviation
       * (sigma).  The sigma for a prototype is computed as the
       * smallest distance of a prototype to its neighbors multiplied
       * by this factor.
       *
       * Default value: 0.7 (such that the probability at the nearest neighbor
       *                     of a prototype decays to ~0.25)
       */
      double sigmaFactor;

      /**
       * lambda
       *
       * Factor used in the second layer to control the smoothness of the
       * interpolated function.
       *
       * Default value: 0.0
       */
      double lambda;

      /**
       * This factor determines the value of the sigmoid function that
       * will be used as threshold for a correct classification.
       *
       * Default value: 0.99
       */
      double high;

      /**
       * distance
       *
       * Type of distance used to compute the distances.  L1Distance and
       * L2Distance are the possible values.
       *
       * Default value: L2Distance
       */
      eDistanceType distance;

      /**
       * Initialization type used for the first layer.
       *
       * You can choose between rbf::LvqMaxDist and rbf::LvqRand
       *
       * Default value: rbf::LvqMaxDist
       */
      eLvqInit lvqInitType;

      /**
       * Specify the type for the first LVQ training of the first layer, 
       * which tries a first positioning of the radial basis functions.
       * (usually LVQ1 or OLVQ1)
       *
       * Default value: OLVQ1
       */
      eLvqTrainType lvqType1;

      /**
       * Specify the type for the second LVQ training phase, which takes
       * the result of the first training and tries to improve them.
       * (usually LVQ3 or OLVQ3)
       *
       * Default value: OLVQ3
       */
      eLvqTrainType lvqType2;

      /**
       * Parameters for the linear least squares computation in the
       * second layer.
       *
       * The computation of the weights in the second layer can be
       * formulated as a linear least squares problem, that is solved
       * with the corresponding functor lti::linearLeastSquares, which
       * is parameterized with these parameters.  The default values are
       * set to SVD solution for robustness.
       */
      linearLeastSquares::parameters llsParameters;

      //@}
    };

    // ----------------------------------------------------
    // rbf::initFunctors
    // ----------------------------------------------------
    class lvqTrainFunctor;

    /**
     * Initialization functor for the first layer of an RBF network.
     *
     * The weights of the layer 1 represent the centers of the radial
     * basis functions and are computed in this case with an LVQ
     * related algorithm.
     *
     */
    class initFunctor1 : public stdLayer::initFunctor {
    public:
      /**
       * Default constructor
       *
       * @param theFeatures the matrix with the training patterns (each row
       *                    a pattern)
       * @param theTrainIDs the numerical ids for each pattern in theFeatures
       * @param theNbViewsObj the number of patterns per class
       * @param theNbObj      the number of classes (or objects)
       * @param theNbNeurObj  the number of neurons per class
       * @param theLvqInit initialization method for the LVQ training
       * @param distance the used distance (L1Distance or L2Distance)
       */
      initFunctor1(const dmatrix& theFeatures,
                   const ivector& theTrainIDs,
                   const ivector& theNbViewsObj,
                   const int& theNbObj,
                   const int& theNbNeurObj,
                   const eLvqInit& theLvqInit,
                   const eDistanceType& distance);

      /**
       * Destructor
       */
      ~initFunctor1();

      /**
       * Apply operator
       */
      bool operator() (dmatrix& weights, ivector& outID);

      /**
       * Set the train parameters
       *
       * @param theTrainType specify training for the LVQ initialization
       * @param theLearnRate
       * @param theLrFac the learn factor
       * @param theWinSize the window size
       */
      void setTrainParams(const eLvqTrainType& theTrainType,
                          const double& theLearnRate,
                          const double& theLrFac=0.,
                          const double& theWinSize=0.);

      /**
       * Returns the name of this class
       */
      virtual const std::string& name() const;

      /**
       * clone function.
       */
      stdLayer::initFunctor* clone() const;

      /**
       * newInstance function.
       */
      stdLayer::initFunctor* newInstance() const;

    protected:
      /**
       * A reference to the training patterns
       */
      const dmatrix& features_;

      /**
       * A reference to the numerical ids of the training patterns
       */
      const ivector& trainIDs_;

      /**
       * A reference to the number of views per object
       */
      const ivector& nbViewsObj_;

      /**
       * Number of objects (or clases)
       */
      int nbObj_;

      /**
       * Number of neurons per object
       */
      int nbNeurObj_;

      /**
       * Learning rate
       */
      double learnRate_;

      /**
       * Learn factor
       */
      double lrFac_;

      /**
       * Window size
       */
      double winSize_;

      /**
       * Which lvq initialization type should be used
       */
      eLvqInit lvqInit_;

      /**
       * Which LVQ training type should be used
       */
      eLvqTrainType lvqType_;

      /**
       * Which distance is used to compute distances L1 or L2
       */
      eDistanceType distance_;

      /**
       * Linear activation functor
       */
      stdLayer::linearActFunctor linear_;

      /**
       * Distance propagation functor
       */
      stdLayer::distancePropFunctor distProp_;

      /**
       * LVQ training functor
       */
      lvqTrainFunctor* trainer_;

      /**
       * Modification flag
       */
      bool changed_;

      /**
       * Indices holding the (random) presentation sequences
       */
      ivector index_;

      /**
       * Storage holder (attribute to avoid construction times)
       */
      dvector dummy_;

      /**
       * Number of training step in course.
       */
      int step_;

      /**
       * Flag that indicates whether the weights were changed at all during
       * the last epoche. If not, no further 'training' is performed.
       */
      bool modified_;
    };

    /**
     * Initialization functor for the layer 2 of an RBF network
     *
     * The weights are initialized (trained) at once by solving a
     * linear least squares problem.
     *
     * The coefficients C for a given neuron are computed by solving
     *
     * \f[ C = (H^T H + \lambda \tilde{H})^{-1}H^T Y \f]
     */
    class initFunctor2 : public stdLayer::initFunctor {
    public:
      /**
       * Constructor
       *
       * \param theH The matrix H is generated by the rbf::layer1 class and
       *        holds some relations between the input patterns and the centers
       *        of the layer 1
       * \param theHtilde The matrix Htilde is also generated by the 
       *        rbf::layer1 class and holds some relations among all centers of
       *        the layer 1.
       * \param theLambda Smooth factor (should be very small)
       * \param theTrainIDs contains the internal class ID for each training
       *        pattern.
       * \param lls Parameters for the linear least squares
       */
      initFunctor2(dmatrix& theH,
                   dmatrix& theHtilde,
                   const double& theLambda,
                   const ivector& theTrainIDs,
                   const linearLeastSquares::parameters& lls);

      /**
       * Apply operator
       */
      bool operator()(dmatrix& weights, ivector& outID);

      /**
       * Returns the name of this class
       */
      virtual const std::string& name() const;

      /**
       * clone function.
       */
      stdLayer::initFunctor* clone() const;

      /**
       * newInstance function.
       */
      stdLayer::initFunctor* newInstance() const;

    protected:

      /**
       * Matrix holding the relationships among centers and training patterns.
       */
      dmatrix& H_;

      /**
       * Matrix holding the relationships among the centers in layer 1
       */
      dmatrix& Htilde_;

      /**
       * Internal IDs
       */
      const ivector& trainIDs_;

      /**
       * Smoothing factor
       */
      double lambda_;

      /**
       * Parameters for the linear least squares solution
       */ 
      const linearLeastSquares::parameters& lls_;
    };

    // ----------------------------------------------------
    // rbf::layer1
    // ----------------------------------------------------

    /**
     * Layer 1 for rbf networks
     */
    class layer1 : public stdLayer {
    public:
      /**
       * constructor for the first layer
       */
      layer1(std::string theName);

      /**
       * destructor
       */
      virtual ~layer1();

      /**
       * train layer 1
       */
      bool train(const dvector& input,
                 const int& trainID,
                 trainFunctor& trainFunc);

      /**
       * get matrix H
       */
      void getH(const dmatrix& features,
                stdLayer::propagationFunctor& prop,
                dmatrix& H);

      /**
       * get matrix H~
       */
      void getHtilde(stdLayer::propagationFunctor& prop,dmatrix& Htilde);

      /**
       * calculate sigma-factors
       */
      void findSigmas(const double& sigmaFac, dvector& sigmas);

      /**
       * set training parameters
       */
      void setTrainParams(initFunctor1& initFunctor,
                          const eLvqTrainType& theTrainType,
                          const double& theLearnRate,
                          const double& theLrFac=0.,
                          const double& theWinSize=0.);

      /**
       * Returns the name of this class
       */
      virtual const std::string& name() const;

      /**
       * clone function.
       */
      stdLayer* clone() const;

      /**
       * newInstance function.
       */
      stdLayer* newInstance() const;
    };

    // ----------------------------------------------------
    // rbf::layer2
    // ----------------------------------------------------

    /**
     * layer 2 for rbf networks
     */
    class layer2 : public stdLayer {
    public:
      /**
       * constructor
       */
      layer2(std::string theName);

      /**
       * destructor
       */
      virtual ~layer2();

      /**
       * train layer 2
       */
      bool train(const dvector& input,
                 const int& trainID,
                 trainFunctor& trainFunc);

      /**
       * Returns the name of this class
       */
      virtual const std::string& name() const;

      /**
       * clone function.
       */
      stdLayer* clone() const;

      /**
       * newInstance function.
       */
      stdLayer* newInstance() const;
    };


    // --------------------------------------------------
    // lvqTrainFunctor
    // --------------------------------------------------
    
    /**
     * Base class of training functors for the lvq networks
     */
    class lvqTrainFunctor : public stdLayer::trainFunctor {
    public:
      /**
       * Constructor
       *
       * @param theProp the distance propagation functor
       */
      lvqTrainFunctor(const stdLayer::distancePropFunctor& theProp);
      
      /**
       * Set the learning rate factor
       */
      virtual void setLearnFactor(const double& lrFac);
      
      /**
       * Set the window size
       */
      virtual void setWindowSize(const double& winSize);
      
    protected:
      /**
       * Distance propagation functor
       */
      stdLayer::distancePropFunctor prop_;
      
      /**
       * Delta vector
       */
      dvector delta_;
      
      /**
       * Delta2 vector
       */
      dvector delta2_;
      
      /**
       * Calls vector
       */
      ivector calls_;
      
      /**
       * Lerning rate
       */
      double learnFactor_;
      
      /**
       * Window size
       */
      double windowSize_;
      
      /**
       * First use of the functor
       */
      bool firstUse_;
      
      /**
       * Get the indices of the two minimum values of the vector
       * min1 <= min2
       */
      void twoMinIndex(const dvector& vct,int& min1,int& min2);
    };
    
    /**
     * LVQ1
     */
    class lvq1TrainFunctor : public lvqTrainFunctor {
    public:
      /**
       * Constructor
       *
       * @param theProp the distance propagation functor
       */
      lvq1TrainFunctor(stdLayer::distancePropFunctor& theProp);
      
      /**
       * Apply operator
       */
      virtual bool operator()(const dvector& input,
                              dmatrix& weights,
                              dvector& outLayer,
                              const ivector& outID,
                              const int& trainID,
                              bool& modified);

      /**
       * Clone
       */
      trainFunctor* clone() const;

      /**
       * New instance
       */
      trainFunctor* newInstance() const;

      /**
       * Returns the name of this class
       */
      virtual const std::string& name() const;
    };
    
    /**
     * OLVQ1
     */
    class olvq1TrainFunctor : public lvqTrainFunctor {
    public:
      /**
       * Constructor
       */
      olvq1TrainFunctor(stdLayer::distancePropFunctor& theProp);
      
      /**
       * Set the learning rate
       */
      void setLearnRate(const double &theLr);
      
      /**
       * Apply operator
       */
      bool operator()(const dvector& input,
                      dmatrix& weights,
                      dvector& outLayer,
                      const ivector& outID,
                      const int& trainID,
                      bool& modified);
      
      /**
       * Clone
       */
      trainFunctor* clone() const;

      /**
       * New instance
       */
      trainFunctor* newInstance() const;

      /**
       * Returns the name of this class
       */
      virtual const std::string& name() const;

    protected:
      /**
       * Learning rate vector
       */
      dvector lr_;
    };
    
    /**
     * LVQ3
     */
    class lvq3TrainFunctor : public lvqTrainFunctor {
    public:
      /**
       * Constructor
       */
      lvq3TrainFunctor(stdLayer::distancePropFunctor& theProp);
      
      /**
       * Apply operator
       */
      bool operator()(const dvector& input,
                      dmatrix& weights,
                      dvector& outLayer,
                      const ivector& outID,
                      const int& trainID,
                      bool& modified);
      
      /**
       * Clone
       */
      trainFunctor* clone() const;

      /**
       * New instance
       */
      trainFunctor* newInstance() const;

      /**
       * Returns the name of this class
       */
      virtual const std::string& name() const;
    };
    
    /**
     * OLVQ3
     */
    class olvq3TrainFunctor : public lvqTrainFunctor {
    public:
      /**
       * Constructor
       */
      olvq3TrainFunctor(stdLayer::distancePropFunctor& theProp);
      
      /**
       * Set the learning rate
       */
      void setLearnRate(const double &lRate);
      
      /**
       * Apply operator
       */
      bool operator()(const dvector& input,
                      dmatrix& weights,
                      dvector& outLayer,
                      const ivector& outID,
                      const int& trainID,
                      bool& modified);

      /**
       * Clone
       */
      trainFunctor* clone() const;

      /**
       * New instance
       */
      trainFunctor* newInstance() const;

      /**
       * Returns the name of this class
       */
      virtual const std::string& name() const;

    protected:
      /**
       * Learning rate vector
       */
      dvector lr_;
      
    };
    
    /**
     * LVQ4
     */
    class lvq4TrainFunctor : public lvqTrainFunctor {
    public:
      /**
       * Constructor
       */
      lvq4TrainFunctor(stdLayer::distancePropFunctor& theProp);
      
      /**
       * Apply operator
       */
      bool operator()(const dvector& input,
                      dmatrix& weights,
                      dvector& outLayer,
                      const ivector& outID,
                      const int& trainID,
                      bool& modified);

      /**
       * Clone
       */
      trainFunctor* clone() const;

      /**
       * New instance
       */
      trainFunctor* newInstance() const;

      /**
       * Returns the name of this class
       */
      virtual const std::string& name() const;
    };
      
    // --------------------------------------------------
    // lvq::initFunctor
    // --------------------------------------------------

    /**
     * Random initialization
     */
    class randInitFunctor : public stdLayer::initFunctor {
    public:
      /**
       * LVQ ANNs are best initialized with the training data
       * therefore the somewhat bulky constructor
       */
      randInitFunctor(const dmatrix& theFeatures,
                      const ivector& theTrainIDs,
                      const ivector& theNbViewsObj,
                      const int& theNbObj,
                      const int& theNbNeurObj);
      
      /**
       * Function operator
       */
      bool operator()(dmatrix& weights, ivector& outID);
      
      /**
       * Returns the name of this class
       */
      virtual const std::string& name() const;

      /**
       * Clone function.
       */
      virtual initFunctor* clone() const;

      /**
       * newInstance function.
       */
      virtual initFunctor* newInstance() const;

    protected:
      /**
       * Reference matrix to the features
       */
      const dmatrix& features_;
      
      /**
       * Reference vector to the train IDs
       */
      const ivector& trainIDs_;
      
      /**
       * Number of views per object
       */
      const ivector& nbViewsObj_;
      
      /**
       * Number of objects
       */
      const int& nbObj_;
      
      /**
       * Number of neuron per object
       */
      const int& nbNeurObj_;
    };
    
    /**
     * Maximum distance initialization
     */
    class maxDistInitFunctor : public stdLayer::initFunctor {
    public:
      /**
       * LVQ ANNs are best initialized with the training data
       * therefore the somewhat bulky constructor
       */
      maxDistInitFunctor(const dmatrix& theFeatures,
                         const ivector& theTrainIDs,
                         const ivector& theNbViewsObj,
                         const int& theNbObj,
                         const int& theNbNeurObj,
                         const eDistanceType& theDistance);
      
      bool operator()(dmatrix& weights, ivector& outID);
       
      /**
       * Returns the name of this class
       */
      virtual const std::string& name() const;

      /**
       * Clone function.
       */
      virtual initFunctor* clone() const;

      /**
       * newInstance function.
       */
      virtual initFunctor* newInstance() const;
     
    protected:
      /**
       * Reference to the matrix with the feature vectors
       */
      const dmatrix& features_;
      
      /**
       * Vector with the train IDs
       */
      const ivector& trainIDs_;
      
      /**
       * Vector with the number of views per objectd
       */
      const ivector& nbViewsObj_;
      
      /**
       * Number of objects
       */
      const int& nbObj_;
      
      /**
       * Number of neurons per object
       */
      const int& nbNeurObj_;
      
      /**
       * Distance
       */
      const eDistanceType& distance_;
    };

    // ----------------------------------------------------
    // rbf
    // ----------------------------------------------------

    /**
     * Constructor
     */
    rbf();

    /**
     * Copy constructor
     */
    rbf(const rbf& other);

    /**
     * Create with the given parameters
     */
    rbf(const parameters& param);

    /**
     * Destructor
     */
    ~rbf();

    /**
     * get type name
     */
    const std::string& name() const;

    /**
     * Training.
     *
     * The row i the input matrix belongs to the class identified by ids.at(i)
     */
    bool train(const dmatrix& input,
               const ivector& ids);

    /**
     * Propagates a feature vector through the network. The winner is
     * set to the unit with the maximum value. If an error occurs, the
     * %outputVectors recect flag is set to true and false is
     * returned.
     */
    bool classify(const lti::dvector& feature, result& res) const;

    /**
     * Get dimensions of the output layer
     */
    inline int getDimOutputLayer() const;

    /**
     * Get the test-data for training statistics
     */
    void setTestSet(const dmatrix& input,
                    const ivector& ids);

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    rbf& copy(const rbf& other);

    /**
     * Alias for copy.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    inline rbf& operator=(const rbf& other) {
      return copy(other);
    }

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual classifier* clone() const;

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual classifier* newInstance() const;

    /**
     * @name Methods to access inner data.
     *
     * Should not be used for anything but Axioms or others JNI interface
     * to use this class.
     */
    //@{

    /**
     * Returns the weights of layer1
     */
    const dmatrix& getWeights1();

    /**
     * Returns the sigmas of layer1
     */
    const dvector& getSigmas1();

    /**
     * Returns the weights of layer2
     */
    const dmatrix& getWeights2();

    /**
     * Returns the IDs of each neuron of the network
     */
    const ivector& getIds();

    /**
     * Set the sigmas for layer 1
     */
    void setSigmas(const dvector& s);

    /**
     * Set data of layer1
     */
    void setLayer1Data(const dmatrix& weights);

    /**
     * Set data of layer2
     */
    void setLayer2Data(const dmatrix& weights, const ivector& ids);

    /**
     * Set the size and init functors after data is transmitted to layers
     * and sigmas are set.
     */
    void initNet(const int& inSize, const int& outSize);


    /**
     * write the rbf classifier in the given ioHandler
     * @param handler the ioHandler to be used
     * @param complete if true (the default) the enclosing begin/end will
     *        be also written, otherwise only the data block will be written.
     * @return true if write was successful
     */
    virtual bool write(ioHandler& handler,const bool complete=true) const;

    /**
     * read the rbf classifier from the given ioHandler
     * @param handler the ioHandler to be used
     * @param complete if true (the default) the enclosing begin/end will
     *        be also written, otherwise only the data block will be written.
     * @return true if write was successful
     */
    virtual bool read(ioHandler& handler,const bool complete=true);

  protected:

    /**
     * RBF layer 1
     */
    layer1 rbf1_;

    /**
     * RBF layer 2
     */
    layer2 rbf2_;

    /**
     * Number of dimensions of the patterns.
     */
    int sizeIn_;

    /**
     * Number of neurons in the output layer, which is equivalent to
     * the number of classes in use (and hence, the size of the
     * internToReal_ vector).
     */
    int sizeOut_;

    /**
     * Hold the internal id for each training pattern in features_
     */
    ivector trainID_;

    /**
     * Look-up table to find the real class Id based on the internal
     * sequential Id number.
     */
    ivector internToReal_;

    /**
     * Map to convert the real class Id numbers to the internal sequential ones
     */
    std::map<int,int> realToIntern_;

    /**
     * Shadown of the current training patterns in use.  Row-wise are stored
     * the training vectors.
     */
    dmatrix features_;

    dvector sigmas_;

    dmatrix testFeatures_;
    ivector testIDs_;
    ivector trainStatIDs_;

    stdLayer::distancePropFunctor* distProp_;
    stdLayer::dotPropFunctor* dotProp_;
    stdLayer::gaussActFunctor* gaussAct_;
    stdLayer::sigmoidActFunctor* sigmoidAct_;

    std::map<int,dvector> targetVec_;

    /**
     * Calculate object probabilities with given features
     */
    bool calcObjProbs(ivector& ids,
                      dmatrix& feats);

  };


  int rbf::getDimOutputLayer() const {
    return sizeOut_;
  }

  /**
   * Read a rbf::eLvqInit
   *
   * @ingroup gStorable
   */
  bool read(ioHandler& handler,rbf::eLvqInit& data);

  /**
   * Write a rbf::eLvqInit
   *
   * @ingroup gStorable
   */
  bool write(ioHandler& handler,const rbf::eLvqInit& data);

  /**
   * Read a rbf::eLvqTrainType
   *
   * @ingroup gStorable
   */
  bool read(ioHandler& handler,rbf::eLvqTrainType& data);

  /**
   * Write a rbf::eLvqTrainType
   *
   * @ingroup gStorable
   */
  bool write(ioHandler& handler,const rbf::eLvqTrainType& data);



}

#endif //_LTI_RBF_H_
