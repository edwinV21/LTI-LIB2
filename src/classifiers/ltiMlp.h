/*
 * Copyright (C) 2002, 2003, 2004, 2005, 2006
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
 * \file   ltiMlp.h
 * \author Bastian Ibach
 * \author Pablo Alvarado
 * \date   13.8.2002
 *
 * $Id: ltiMlp.h,v 1.5 2008-06-03 17:34:46 alvarado Exp $
 */


#ifndef _LTI_M_L_P_H_
#define _LTI_M_L_P_H_

#include "ltiSupervisedInstanceClassifier.h"
#include "ltiRandomDistribution.h"
#include "ltiUniformContinuousDistribution.h"
#include "ltiVector.h"
#include <vector>

namespace lti {

  /**
   * Multi-layer perceptrons
   *
   * This class implements multi-layer neural networks using different
   * training methods.
   *
   * A number of layers between 1 and 3 is allowed.
   *
   * Training methods implemented at this time are:
   * - generalized delta-rule (steepest descent) with and without momentum,
   * - conjugate gradients.
   *
   * Following example shows how to use this sort of classifier:
   *
   * \code
   *    double inData[] = {-1,-1,
   *                       -1, 0,
   *                       -1,+1,
   *                       +0,+1,
   *                       +1,+1,
   *                       +1,+0,
   *                       +1,-1,
   *                       +0,-1,
   *                       +0,+0};
   *
   *    lti::dmatrix inputs(9,2,inData);          // training vectors
   *
   *    int idsData[] = {1,0,1,0,1,0,1,0,1}; // and the respective ids
   *    lti::ivector ids(9,idsData);
   *
   *    lti::mlp ann;  // our artificial neural network
   *
   *    lti::mlp::parameters param;
   *
   *    // the problem above cannot be solved with 1 hidden layer, try 2:
   *    param.hiddenUnits.assign(2,6); // 2 hidden layers with 6 units each
   *    param.activationFunctions.assign(param.hiddenUnits.size()+1,
   *                                     "sigmoidFunctor");
   *
   *    param.trainingMode = lti::mlp::SteepestDescent;
   *    param.learnrate = 0.2;
   *    param.maxNumberOfEpochs=5000;
   *
   *    ann.setParameters(param);
   *
   *    // we want to see some info while training
   *    streamProgressInfo prog(std::cout);
   *    ann.setProgressObject(prog);
   *
   *    // train the network
   *    ann.train(inputs,ids);
   *
   *    // let us save our network for future use
   *    // in the file called mlp.dat
   *    std::ofstream out("mlp.dat");
   *    lti::lispStreamHandler lsh(out);
   *
   *    // save the network
   *    ann.write(lsh);
   *    // close the file
   *    out.close();
   *
   *    // show some results with the same training set:
   *
   *    lti::mlp::result outv; // here we will get some
   *                                 // classification results
   *    std::cout << std::endl << "Results: " << std::endl;
   *
   *    int i,id;
   *    for (i=0;i<inputs.rows();++i) {
   *      ann.classify(inputs.getRow(i),outv);
   *      std::cout << "Input " << inputs.getRow(i) << " \tOutput: ";
   *      outv.getId(outv.getWinner(),id);
   *      std::cout << id;
   *      if (id != ids.at(i)) {
   *        std::cout << " <- should be " << ids.at(i);
   *      }
   *      std::cout << std::endl;
   *    }
   *  }
   * \endcode
   *
   * @see mlp::parameters
   */
  class mlp : public supervisedInstanceClassifier {
  public:

    /**
     * Training type
     */
    enum eTrainingType {
      SteepestDescent,   /**< Generalized Delta-Rule.  Just use the gradient
                          *   and the learnrate in a simple gradient descent
                          *   approach.
                          */
      ConjugateGradients /**< Conjugate gradients algorithm */
    };
    

    // ----------------------------------------------
    // activation functor
    // ----------------------------------------------

    /**
     * Parent class for all activation function functors.
     *
     * This is a pure abstract class
     */
    class activationFunctor : public ioObject {
    public:
      /**
       * The functor operator.  Operates on place, applying the function
       * to each element of the vector
       */
      virtual bool apply(dvector& output) const = 0;

      /**
       * The functor operator.  Operates on copy
       */
      virtual bool apply(const dvector& src, dvector& output) const =  0;

      /**
       * The derivative of the functor
       */
      virtual bool deriv(dvector& output) const = 0;

      /**
       * Return a copy of this functor
       */
      virtual activationFunctor* clone() const = 0;

      /**
       * Return a copy of this functor
       */
      virtual activationFunctor* newInstance() const = 0;

      /**
       * Returns the name of this class
       */
      virtual const std::string& name() const = 0;

      /**
       * Return value used to represent "true" or "on"
       */
      virtual const double& onValue() const = 0;

      /**
       * Return value used to represent "false" or "off"
       */
      virtual const double& offValue() const = 0;

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
     * A linear activation function
     */
    class linearActFunctor : public activationFunctor {
    public:
      /**
       * The functor operator
       */
      virtual bool apply(dvector& output) const;

      /**
       * The functor operator.  Operates on copy
       */
      virtual bool apply(const dvector& src, dvector& output) const;

      /**
       * The derivative of the functor
       */
      virtual bool deriv(dvector& output) const;

      /**
       * Return value used to represent "true" or "on"
       */
      virtual const double& onValue() const;

      /**
       * Return value used to represent "false" or "off"
       */
      virtual const double& offValue() const;

      /**
       * Return a copy of this functor
       */
      virtual activationFunctor* clone() const;

      /**
       * Return a copy of this functor
       */
      virtual activationFunctor* newInstance() const;

      /**
       * Returns the name of this class
       */
      virtual const std::string& name() const;
    };

    /**
     * A sign activation function (1.0 if input 0 or positive, -1.0 otherwise)
     */
    class signFunctor : public activationFunctor {
    public:
      /**
       * The functor operator
       */
      virtual bool apply(dvector& output) const;

      /**
       * The functor operator.  Operates on copy
       */
      virtual bool apply(const dvector& src, dvector& output) const;

      /**
       * The derivative of the functor (will return 1.0 to allow learning)
       */
      virtual bool deriv(dvector& output) const;

      /**
       * Return value used to represent "true" or "on"
       */
      virtual const double& onValue() const;

      /**
       * Return value used to represent "false" or "off"
       */
      virtual const double& offValue() const;

      /**
       * Return a copy of this functor
       */
      virtual activationFunctor* clone() const;

      /**
       * Return a copy of this functor
       */
      virtual activationFunctor* newInstance() const;

      /**
       * Returns the name of this class
       */
      virtual const std::string& name() const;
    };

    /**
     * A sigmoid activation function
     */
    class sigmoidFunctor : public activationFunctor {

    protected:
      /**
       * Slope used in the sigmoid function
       */
      double slope_;

    public:
      /**
       * Constructor using explicit sigmoid slope
       */
      sigmoidFunctor(const double& theSlope);

      /**
       * Constructor of a sigmoid with slope 1.0
       */
      sigmoidFunctor();

      /**
       * The functor operator
       */
      virtual bool apply(dvector& output) const;

      /**
       * The functor operator.  Operates on copy
       */
      virtual bool apply(const dvector& src, dvector& output) const;

      /**
       * The derivative of the functor
       */
      virtual bool deriv(dvector& output) const;

      /**
       * Return a copy of this functor
       */
      virtual activationFunctor* clone() const;

      /**
       * Return a copy of this functor
       */
      virtual activationFunctor* newInstance() const;

      /**
       * Returns the name of this class
       */
      virtual const std::string& name() const;

      /**
       * Return value used to represent "true" or "on"
       */
      virtual const double& onValue() const;

      /**
       * Return value used to represent "false" or "off"
       */
      virtual const double& offValue() const;

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
     * The parameters for the class mlp
     */
    class parameters : public supervisedInstanceClassifier::parameters {
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
      virtual ~parameters();

      /**
       * Returns the name of this class
       */
      virtual const std::string& name() const;

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
       * Returns a pointer to a clone of the parameters
       */
      virtual parameters* clone() const;

      /**
       * Returns a pointer to a clone of the parameters
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

      /**
       * Initialize the parameters to create a mlp with two layers with the
       * given number of hidden units (the number of input and output units is
       * determined in the training stage).
       *
       * @param hidden number of hidden units
       * @param activ activation function to be used in all units
       *
       */
      bool setLayers(const int hidden,
                     const activationFunctor& activ);

      /**
       * Initialize the parameters to create a mlp with two layers with the
       * given number of hidden units (the number of input and output units is
       * determined in the training stage).
       *
       * @param hidden number of hidden units
       * @param activ activation function to be used in all units
       *
       */
      bool setLayers(const int hidden,
                     const std::string& activ);


      /**
       * Initialize the parameters to create a mlp with one single layer
       * (the number of input and output units is determined in the training 
       * stage).
       *
       * @param activ activation function to be used in all units
       */
      bool setLayers(const activationFunctor& activ);

      /**
       * Initialize the parameters to create a mlp with one single layer
       * (the number of input and output units is determined in the training 
       * stage).
       *
       * @param activ activation function to be used in all units
       */
      bool setLayers(const std::string& activ);


      /**
       * Set the activation functor for a given layer.  The current number of
       * layers is determined by the size of the \a hiddenUnits attribute.
       *
       * @param layer number of layer
       * @param aFct activationFunctor
       * @return true if successful, or false otherwise (usually, the
       * layer number is invalid
       */
      bool setLayerActivation(const int layer, const activationFunctor& aFct);


      // ------------------------------------------------
      // The parameters
      // ------------------------------------------------

      /**
       * Traning mode to be used.
       *
       * Default value: ConjugateGradients
       */
      eTrainingType trainingMode;

      /**
       * If true, an epoch (all the training data) will be presented before a
       * weigh adaption is taken.  Otherwise just training point is considered
       * to adapt the weights.  For the conjugate gradient method this mode
       * is ignored (assumed true).
       *
       * Default value: true
       */
      bool batchMode;

      /**
       * Value for the momentum used in the steepest descent methods.
       * Should be between 0.0 and 1.0.
       *
       * Default value: 0.0 (no momentum)
       */
      double momentum;

      /**
       * Number of units in the hidden layers.
       *
       * The size of this vector determines indirectly the number of
       * layers for the network.  It corresponds to is the number of
       * layers minus one, which means that the total number of layers
       * for the network is equal to the size of this vector plus one.
       *
       * Default value: [4] (i.e. a 2 layer network with 4 units in the hidden
       *                     layer)
       */
      ivector hiddenUnits;

      /**
       * Learning rate for the steepest descent method.
       *
       * Default value: 0.1
       */
      float learnrate;

      /**
       * Maximal number of epochs (number of presentations of the entire
       * training set)
       *
       * Default: 500
       */
      int maxNumberOfEpochs;

      /**
       * If this error value (or lower) is reached, the training is stopped.
       * For the conjugate gradients method, the algorithm is stopped if
       * the magnitude of the gradient is smaller than this value multiplied
       * by the magnitude of the initial gradient.
       *
       * Default value: 0.005
       */
      double stopError;

      /**
       * Activation functors (per layer).  The strings given correspond to the
       * class name of the activation function used, which must be one of:
       * - "linearActFunctor"
       * - "signFunctor"
       * - "sigmoidFunctor"
       *
       * You can create your own activation functions and inscribe them in the
       * lti::factory<mlp::activationFunctor> for their use here.  The name
       * given here is the name used as key in the factory.
       *
       * The size of this vector should be exaclty equal to
       * hiddenUnits.size()+1
       *
       * Default value: ["sigmoidFunctor","sigmoidFunctor"]
       */
      std::vector<std::string> activationFunctions;

      /**
       * General configuration for random number generators.
       *
       * Default value: default configuration
       */
      randomDistribution::parameters rndConfig;
    };

    /**
     * Default constructor
     */
    mlp();

    /**
     * Default constructor with parameters
     */
    mlp(const parameters& par);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    mlp(const mlp& other);

    /**
     * Destructor
     */
    virtual ~mlp();

    /**
     * Copy data of "other" classifier.
     * @param other the classifier to be copied
     * @return a reference to this classifier object
     */
    mlp& copy(const mlp& other);

    /**
     * Alias for copy member
     * @param other the classifier to be copied
     * @return a reference to this classifier object
     */
    mlp& operator=(const mlp& other);

    /**
     * Returns a pointer to a clone of this classifier.
     */
    virtual classifier* clone() const;

    /**
     * Returns a pointer to a clone of this classifier.
     */
    virtual classifier* newInstance() const;

    /**
     * Returns the name of this class
     */
    virtual const std::string& name() const;

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * Update the parameters
     */
    bool updateParameters();
    
    /**
     * Supervised training.
     *
     * The vectors in the <code>input</code> matrix
     * must be trained using as "known" classes the values given in
     * <code>ids</code>.
     *
     * @param input the matrix with input vectors (each row is a training
     *              vector)
     * @param ids the output classes ids for the input vectors.
     * @return true if successful, false otherwise.  (if false you can check
     *              the error message with getStatusString())
     */
    virtual bool train(const dmatrix& input,
                       const ivector& ids);

    /**
     * Supervised training.
     *
     * This method, used for debugging purposes mainly, initialize the
     * weights with the values given.
     *
     * The vectors in the \a input matrix must be trained using as "known"
     * classes the values given in \a ids.
     *
     * @param weights this vector is used to initialize the weights.  Must
     *                be consistent with the parameters.
     * @param input the matrix with input vectors (each row is a training
     *              vector)
     * @param ids the output classes ids for the input vectors.
     * @return true if successful, false otherwise.  (if false you can check
     *              the error message with getStatusString())
     */
    virtual bool train(const dvector& weights,
                       const dmatrix& input,
                       const ivector& ids);
    /**
     * Classification.
     *
     * Classifies the feature and returns the outputVector with
     * the classification result.
     * @param feature the %vector to be classified
     * @param res the result of the classification
     * @return false if an error occurred during classification else true
     */
    virtual bool classify(const dvector& feature, result& res) const;

    /**
     * Write the rbf classifier in the given ioHandler
     * @param handler the ioHandler to be used
     * @param complete if true (the default) the enclosing begin/end will
     *        be also written, otherwise only the data block will be written.
     * @return true if write was successful
     */
    virtual bool write(ioHandler& handler,const bool complete=true) const;

    /**
     * Read the rbf classifier from the given ioHandler
     * @param handler the ioHandler to be used
     * @param complete if true (the default) the enclosing begin/end will
     *        be also written, otherwise only the data block will be written.
     * @return true if write was successful
     */
    virtual bool read(ioHandler& handler,const bool complete=true);

    /*
     * Undocumented function for debug purposes only.  It displays the
     * internal weight matrices
     */
    bool prettyPrint();

    /**
     * Return a reference to the internal weights vector.
     * Used mainly for debugging purposes.
     */
    const dvector& getWeights() const;

  private:

    // used variables

    /**
     * vector for saving the weights of the network
     */
    dvector weights_;

    /**
     * indices for the begining of each layer in the weights vector
     */
    ivector layerIndex_;

    /**
     * weights as matrix, to simplify propagation.
     * The matrices share the memory with weights.
     *
     * All matrices are initialized in initWeights()
     */
    std::vector< dmatrix > matWeights_;

    /**
     * number of input units (determined by the size of the training data)
     */
    int inputs_;

    /**
     * number of outputs (determined by training)
     */
    int outputs_;

    /**
     * check how many outputs are required for the given
     * (external) ids, and set the outputs attribute.
     */
    void checkHowManyOutputs(const ivector& ids);


    /**
     * UnitsOut, save output of each unit
     * each layer ist one element of the vector
     * first layer has index 0
     */
    mutable std::vector< dvector > unitsOut_;

    /**
     * Net value at each layer
     */
    mutable std::vector < dvector > unitsNet_;

    /**
     * Activation functors (per layer).  The objects pointed by these
     * elements will be deleted when the class is deleted.
     */
    std::vector<activationFunctor*> activationFunctions_;

    /**
     * Clean the activationFunctions_ array
     */
    void cleanActivationFunctions();

    /**
     * Value for "off" at the output layer (it depends on the activation
     * functor of the output layer.
     */
    double off_;

    /**
     * Value for "on" at the output layer (it depends on the activation
     * functor of the output layer.
     */
    double on_;

    /**
     * Initialize weights with random values.
     * This method assumes that the attributes inputs and outputs are correct.
     *
     * @param keepWeightVals if true, the contents of weights won't be
     *                       changed (if they are incompatible with the
     *                       actual parameters, false will be returned).
     *                       if false, the values of the weights will be
     *                       initialized with random values between
     *                       "from" and "to".
     * @return true if successful, false otherwise.
     */
    bool initWeights(const bool keepWeightVals = false);


    /**
     * Given the weights vector, update the vector of matrices so that
     * each matrix uses its respective memory block in the vector.
     */
    void updateWeightIndices(dvector& theWeights,
                             std::vector<dmatrix>& theWMats) const;


    /**
     * Accumulated error for one epoch
     */
    double totalError_;

    /**
     * Error norm
     */
    double errorNorm_;

    /**
     * Calculate all outputs for all network units.  The result will
     * be left in the unitsOut attribute.
     *
     * @param input vector
     */
    bool propagate(const dvector& input) const;

    /**
     * Calculate all outputs for all network units.
     *
     * @param input input vector
     * @param mWeights weights in matrix form
     * @param uNet preliminary results for each unit with the "net" values
     * @param uOut output for all units (f(net)).
     * @return true if successful, false otherwise.
     */
    bool propagate(const dvector& input,
                   const std::vector<dmatrix>& mWeights,
                   std::vector<dvector>& uNet,
                   std::vector<dvector>& uOut) const;


    /**
     * Compute the error using the last propagated input and the given
     * pattern
     */
    bool computeActualError(const int id,double& error) const;

    /**
     * Compute the error using the given output units vector
     */
    bool computePatternError(const int id,
                             const dvector& outUnits,
                             double& error) const;

    /**
     * Compute the error of the given weights for the whole training set.
     */
    bool computeTotalError(const std::vector<dmatrix>& mWeights,
                           const dmatrix& inputs,
                           const ivector& ids,
                           double& totalError) const;

    /**
     * Compute mat*vct' where vct' is a vector with one additional element
     * (1.0) at the beginning of vct.
     */
    bool biasMultiply(const dmatrix& mat,
                      const dvector& vct,
                      dvector& res) const;

    /**
     * calculate the negative gradient of error surface using
     * back-propagation algorithm
     *
     * @param input input vector
     * @param outputId desired output.  This value must be between 0 and
     *                 the number of output elements-1.
     * @param grad computed gradient of the error surface
     * @return true if successful, or false otherwise.
     */
    bool calcGradient(const dvector& input,
                      const int outputId,
                      dvector& grad);

    /**
     * calculate negative gradient of error surface using
     * back-propagation algorithm for all patterns in an epoch.
     *
     * @param input input vector
     * @param outputId desired output.  This value must be between 0 and
     *                 the number of output elements-1.
     * @param grad computed gradient of the error surface
     * @return true if successful, or false otherwise.
     */
    bool calcGradient(const dmatrix& inputs,
                      const ivector& ids,
                      dvector& grad);


    /**
     * train the network with steepest descent method (batch mode)
     * Weights must be initialized previously
     */
    bool trainSteepestBatch(const dmatrix& inputs,
                            const ivector& internalIds);


    /**
     * train the network with steepest descent method (sequential mode)
     * Weights must be initialized previously
     */
    bool trainSteepestSequential(const dmatrix& inputs,
                                 const ivector& internalIds);


    /**
     * train the network with steepest descent method (batch mode)
     * Weights must be initialized previously
     */
    bool trainConjugateGradients(const dmatrix& inputs,
                                 const ivector& internalIds);

    /**
     * compute the error norm, with which all displayed error values are
     * normalized.  This will allow to compare easily different classifiers.
     */
    bool computeErrorNorm(const ivector& internIds);

    /**
     * line search computes the eta scalar factor at which the error
     * is minimized.  It begins at the actual weight and follows the given
     * direction.
     */
    bool lineSearch(const dmatrix& inputs,
                    const ivector& ids,
                    const dvector& direction,
                    double& eta,
                    dvector& newWeights) const;

    /**
     * Random number generator
     */
    uniformContinuousDistribution unifRnd_;
  };


  /**
   * Read a mlp::eTrainingType
   *
   * @ingroup gStorable
   */
  bool read(ioHandler& handler,mlp::eTrainingType& data);

  /**
   * Write a mlp::eTrainingType
   *
   * @ingroup gStorable
   */
  bool write(ioHandler& handler,const mlp::eTrainingType& data);

}

#endif
