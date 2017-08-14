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
 * \file   ltiStdLayer.h
 * \author Peter Doerfler
 * \date   11.08.2000
 *
 * $Id: ltiStdLayer.h,v 1.3 2008-04-04 20:38:31 alvarado Exp $
 */

#ifndef _LTI_STD_LAYER_H_
#define _LTI_STD_LAYER_H_

#include <string>
#include <iostream>

#include "ltiMatrix.h"
#include "ltiVector.h"
#include "ltiObject.h"
#include "ltiStatus.h"

namespace lti {

  /**
   * Enumeration of flags that indicate usage of L1 or L2 distances
   * for propagation.
   */
  enum eDistanceType {
    L1Distance, /*!< L1-distance (Manhattan distance) */
    L2Distance  /*!< L2-distance (Euclidian distance) */
  };

  /**
   * This abstract class implements a standard neural network layer.
   *
   * This class contains the weights matrix, the vector with the
   * output values, the class id for each output element, the
   * dimensionality of the input vector and output layer (which must
   * be consistent with the dimensions of the weights matrix and the
   * output vector), the name of the layer (an ASCII string) and a learn
   * rate value.
   *
   * The enclosed classes are supponsed to modify this values.  There are
   * some initialization, activation, propagation and training functors.
   */
  class stdLayer : public ioObject,
                   public status {
  public:

    // ----------------------------------------------
    // initialization functor
    // ----------------------------------------------

    /**
     * This class and all it's derived classes should initialize
     * the network layer
     */
    class initFunctor : public object {
    public:
      /**
       * Default constructor
       */
      initFunctor();

      /**
       * Destructor
       */
      virtual ~initFunctor();

      /**
       * Initialization operator
       */
      virtual bool operator()(dmatrix& weights,ivector& outID) = 0;

      /**
       * Returns the name of this class
       */
      virtual const std::string& name() const = 0;

      /**
       * Clone function.
       */
      virtual initFunctor* clone() const = 0;

      /**
       * newInstance function.
       */
      virtual initFunctor* newInstance() const = 0;

    protected:

      /**
       * Returns the unique IDs in a vector.
       */
      ivector uniqueIDs(const ivector& allIDs) const;

      /**
       * Returns all train vectors of the given class.
       *
       * @param allVectors the training vectors are in the rows of this matrix
       * @param allIDs IDs for the training vectors
       * @param nbViews number of views per object
       * @param id the ID for which the features are returned
       */
      dmatrix classVectors(const dmatrix& allVectors,
                           const ivector& allIDs,
                           int nbViews,
                           int id) const;
    };

    // ----------------------------------------------
    // activation functor
    // ----------------------------------------------

    /**
     * Parent class for all activation function functors
     */
    class activationFunctor : public object {
    public:
      /**
       * The functor operator
       */
      virtual bool operator()(dvector& output) const = 0;

      /**
       * Returns the name of this class
       */
      virtual const std::string& name() const = 0;

      /**
       * Clone function.
       */
      virtual activationFunctor* clone() const = 0;

      /**
       * newInstance function.
       */
      virtual activationFunctor* newInstance() const = 0;


    };

    /**
     * A linear activation function
     */
    class linearActFunctor : public activationFunctor {
    public:
      /**
       * The functor operator
       */
      virtual bool operator()(dvector& output) const;

      /**
       * Returns the name of this class
       */
      virtual const std::string& name() const;

      /**
       * Clone function.
       */
      virtual activationFunctor* clone() const;

      /**
       * newInstance function.
       */
      virtual activationFunctor* newInstance() const;
    };

    /**
     * A gaussian activation function
     */
    class gaussActFunctor : public activationFunctor {
    protected:
      /**
       * Mean
       */
      double mu_;

      /**
       * Standard deviation
       */
      double sigma_;

      /**
       * Pointer to std deviation vector
       */
      const dvector* sigmaVec_;

    public:
      /**
       * Constructor for a gaussian activation function with
       * mean value theMu and std. deviation theSigma
       */
      gaussActFunctor(const double& theMu,
                      const double& theSigma);

      /**
       * Constructor for a gaussian activation function with
       * mean value theMu and std. deviation vector theSigma
       */
      gaussActFunctor(const double& theMu,
                      const dvector& theSigmaVec);


      /**
       * Functor operator
       */
      bool operator() (dvector& output) const;

      /**
       * Returns the name of this class
       */
      virtual const std::string& name() const;

      /**
       * Clone function.
       */
      virtual activationFunctor* clone() const;

      /**
       * newInstance function.
       */
      virtual activationFunctor* newInstance() const;

    };

    /**
     * A sigmoid activation function
     */
    class sigmoidActFunctor : public activationFunctor {
    protected:
      /**
       * Slope
       */
      double slope_;

    public:
      /**
       * Constructor with slope
       */
      sigmoidActFunctor(const double& theSlope);

      /**
       * Default constructor with a slope 1.
       */
      sigmoidActFunctor();

      /**
       * Function operator
       */
      bool operator () (dvector& output) const;

      /**
       * Returns the name of this class
       */
      virtual const std::string& name() const;

      /**
       * Clone function.
       */
      virtual activationFunctor* clone() const;

      /**
       * newInstance function.
       */
      virtual activationFunctor* newInstance() const;

    };

    // ----------------------------------------------
    // propagation functor
    // ----------------------------------------------
    /**
     * Parent class for all propagation functors
     *
     * All propagation functors use a given activation function
     * to calculate the output layer vector using a given input
     * vector and a given weights matrix
     */
    class propagationFunctor : public object {
    public:

      /**
       * Defines a linear activation function
       */
      propagationFunctor(const activationFunctor& theAct);

      /**
       * The functor operator
       * @param input the input vector
       * @param weights the weights matrix of the layer
       * @param outLayer the output vector
       */
      virtual bool operator() (const dvector& input,
                               const dmatrix& weights,
                               dvector& outLayer) const = 0;
      /**
       * Returns the name of this class
       */
      virtual const std::string& name() const = 0;

      /**
       * Clone function.
       */
      virtual propagationFunctor* clone() const = 0;

      /**
       * newInstance function.
       */
      virtual propagationFunctor* newInstance() const = 0;

    protected:
      /**
       * Reference to the activation function in use (given at construction
       * time)
       */
      const activationFunctor& act_;
    };

    /**
     * Distance propagation functor
     */
    class distancePropFunctor : public propagationFunctor {
    public:

      /**
       * Default constructor
       *
       * For Propagation L1 and L2 distance can be used
       */
      distancePropFunctor(const activationFunctor& theAct,
                          eDistanceType theDistance = L2Distance);

      /**
       * The functor operator
       * @param input the input vector
       * @param weights the weights matrix of the layer
       * @param outLayer the output vector
       */
      bool operator() (const dvector& input,
                       const dmatrix& weights,
                             dvector& outLayer) const;

      /**
       * Returns the name of this class
       */
      virtual const std::string& name() const;

      /**
       * Clone function.
       */
      virtual propagationFunctor* clone() const;

      /**
       * newInstance function.
       */
      virtual propagationFunctor* newInstance() const;


    private:
      /**
       * Distance
       */
      eDistanceType distance_;

      /**
       * l1 distance
       */
      bool l1(const dmatrix& a,
              const dvector& b,
              dvector& dest) const;

      /**
       * l2 distance
       */
       bool l2(const dmatrix& a,
               const dvector& b,
               dvector& dest) const;
    };

    /**
     * Dot-Product propagation functor
     */
    class dotPropFunctor : public propagationFunctor {
    public:
      /**
       * Default constructor
       */
      dotPropFunctor(const activationFunctor& theAct);

      /**
       * The functor operator
       * @param input the input vector
       * @param weights the weights matrix of the layer
       * @param outLayer the output vector
       */
      bool operator() (const dvector& input,
                       const dmatrix& weights,
                             dvector& outLayer) const;

      /**
       * Returns the name of this class
       */
      virtual const std::string& name() const;

      /**
       * Clone function.
       */
      virtual propagationFunctor* clone() const;

      /**
       * newInstance function.
       */
      virtual propagationFunctor* newInstance() const;
    };

    /**
     * training functor
     */
    class trainFunctor : public object {
    public:
      /**
       * Default constructor
       * Both neccessary since there are nets which are trained
       * with an overall net error, not a layer error
       */
      trainFunctor();

      /**
       * Destructor
       */
      virtual ~trainFunctor();

      /**
       * Clone
       */
      trainFunctor* clone() const = 0;

      /**
       * New instance
       */
      trainFunctor* newInstance() const = 0;

      /**
       * Returns the name of this class
       */
      virtual const std::string& name() const = 0;

      /**
       * Set the learn rate
       */
      virtual void setLearnRate(const double & lr);

      /**
       * The functor operator for supervised training requires
       * following parameters:
       * @param input the input vector
       * @param weights the weights matrix to be changed
       * @param outLayer the output layer vector
       * @param outID the class identification for each output neuron
       * @param trainID the class ID for the presented vector
       * @param netError the error of the layer
       * @param modified indicates whether the weights were modified
       */
      virtual bool operator()(const dvector& input,
                              dmatrix& weights,
                              dvector& outLayer,
                              const ivector& outID,
                              const int& trainID,
                              dvector& netError,
                              bool& modified);

      /**
       * The functor operator for unsupervised training requires
       * following parameters:
       * @param input the input vector
       * @param weights the weights matrix to be changed
       * @param outLayer the output layer vector
       * @param netError the error of the layer
       * @param modified indicates whether the weights were modified
       */
      virtual bool operator()(const dvector& input,
                              dmatrix& weights,
                              dvector& outLayer,
                              dvector& netError,
                              bool& modified);

      /**
       * The functor operator for supervised training requires
       * following parameters:
       * @param input the input vector
       * @param weights the weights matrix to be changed
       * @param outLayer the output layer vector
       * @param outID the class identification for each output neuron
       * @param trainID the class ID for the presented vector
       * @param modified indicates whether the weights were modified
       * @return true if successful, false otherwise
       */
      virtual bool operator()(const dvector& input,
                              dmatrix& weights,
                              dvector& outLayer,
                              const ivector& outID,
                              const int& trainID,
                              bool& modified);

      /**
       * The functor operator for unsupervised training requires
       * following parameters:
       * @param input the input vector
       * @param weights the weights matrix to be changed
       * @param outLayer the output layer vector
       * @param modified indicates whether the weights were modified
       * @return true if successful, false otherwise
       */
      virtual bool operator ()(const dvector& input,
                               dmatrix& weights,
                               dvector& outLayer,
                               bool& modified);

    protected:
      /**
       * Learn rate
       */
      double learnRate_;
    };

    // ----------------------------------------------
    // standard layer
    // ----------------------------------------------

    /**
     * Default constructor
     */
    stdLayer(const std::string& layerName);

    /**
     * Copy constructor
     */
    stdLayer(const stdLayer& other);

    /**
     * Destructor
     */
    virtual ~stdLayer();

    /**
     * Copy member
     */
    stdLayer& copy(const stdLayer& other);

    /**
     * Initialize the weights
     */
    virtual void initWeights(initFunctor& initFunc);

    /**
     * Train the network layer (supervised) with the given input vector
     *
     * Does one step of training, i.e. one feature vector 'input' of class
     * 'trainID' is learned by the layer as specified in the trainFunctor
     *
     * @return false if an error occurred, otherwise true
     */
    virtual bool train(const dvector& input,
                       const int& trainID,
                       trainFunctor& trainFunc);

    /**
     * Train the network layer (unsupervised) with the given input vector
     *
     * @return false if an error occurred, otherwise true
     */
    virtual bool train(const dvector& input,trainFunctor& trainFunc);

    /**
     * Propagate the vector through the network
     *
     * @return false if an error occurred, otherwise true
     */
    virtual bool propagate(const dvector& input,
                           propagationFunctor& prop,
                           dvector& output) const;

    /**
     * Get the class IDs for output vector elements
     */
    inline const ivector& getOutID() const;

    /**
     * Set the size of the input and output vectors
     * @param in size of the input vector
     * @param out size of the output vector
     */
    void setSize(const int& in, const int& out);

    /**
     * Write the standard layer in the given ioHandler
     * @param handler the ioHandler to be used
     * @param complete if true (the default) the enclosing begin/end will
     *        be also written, otherwise only the data block will be written.
     * @return true if write was successful
     */
    virtual bool write(ioHandler& handler,const bool complete=true) const;

    /**
     * Read the standard layer from the given ioHandler
     * @param handler the ioHandler to be used
     * @param complete if true (the default) the enclosing begin/end will
     *        be also written, otherwise only the data block will be written.
     * @return true if write was successful
     */
    virtual bool read(ioHandler& handler,const bool complete=true);

    /**
     * get the weights of the layer
     */
    inline const dmatrix& getWeights();

    /**
     * set the learn rate
     */
    inline void setLearnRate(const double& theLR);


    /**
     * These methods should only be used by the Axiom Java interface
     */
    //@{

    /**
     * Set the weights of the layer to the given matrix
     */
    void setWeights(const dmatrix& weights);

    /**
     * Set the IDs of the layer to the given vector
     */
    void setIDs(const ivector& ids);

    //@}

  protected:
    /**
     * Weights matrix
     */
    dmatrix weights_;

    /**
     * Class ID of output node
     */
    ivector outID_;

    /**
     * Dimensionality of the input vector
     */
    int sizeIn_;

    /**
     * Dimensionality of the output vector
     */
    int sizeOut_;

    /**
     * Name of a layer
     */
    std::string layerName_;

    /**
     * Learn rate
     */
    double learnRate_;
  };

  /**
   * @name Storable interface for classifiere classes
   *  Members for the storable interface
   */

  /**
   * Read a rbf::eLvqTrainType
   *
   * @ingroup gStorable
   */
  bool read(ioHandler& handler,eDistanceType& data);

  /**
   * Write a rbf::eLvqTrainType
   *
   * @ingroup gStorable
   */
  bool write(ioHandler& handler,const eDistanceType& data);


  // --------------------------------------
  // inline - methods implementation
  // --------------------------------------

  const ivector& stdLayer::getOutID() const {
    return outID_;
  }

  void stdLayer::setLearnRate(const double& theLR) {
    learnRate_=theLR;
  }

  const dmatrix& stdLayer::getWeights() {
    return weights_;
  }
}

#endif //_LTI_STD_LAYER_H_
