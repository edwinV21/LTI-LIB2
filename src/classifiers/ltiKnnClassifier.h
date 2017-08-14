/*
 * Copyright (C) 2003, 2004, 2005, 2006
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
 * \file   ltiKnnClassifier.h
 * \author Frederik Lange
 * \author Pablo Alvarado
 * \date   15.06.2003
 *
 * $Id: ltiKnnClassifier.h,v 1.2 2012-09-16 04:58:33 alvarado Exp $
 */

#ifndef _LTI_KNN_CLASSIFIER_H_
#define _LTI_KNN_CLASSIFIER_H_

#include "ltiSupervisedInstanceClassifier.h"
#include "ltiKdTree.h"

#include <map>
#include <vector>

namespace lti {
  /**
   * Implements a k nearest neighbors search based classifier.
   *
   * The simplest case of a k-nearest neighbor classifier is for k=1, also
   * known as a nearest neighbor classifier, which assings as winner class for
   * a test point \a x the one belonging to the nearest sample point.
   *
   * For k>1, a k nearest neighbor classifier assigns to a point \a x the class
   * most represented in the k nearest neighbors.  In the simplest case, each
   * of the k nearest sample points vote with the same weight for their
   * respective classes.  In more sophisticated cases, each point will vote
   * with a weight depending on the total number of sample points of its class
   * and/or the ratio between the distance of the test point to the winner
   * sample and the distance of the test point to the first sample point
   * belonging to another class.
   *
   * At this point, only the use of Euclidean distance is allowed.
   *
   * This classifier uses a kd-Tree to perform the search in an efficient
   * manner, but shows therefore also the drawbacks of a normal kd-Tree: it is
   * not suitable for higher dimensional spaces.  If you use high-dimensional
   * spaces, maybe you should try increasing the bucket size, or activating
   * the best-bin-first mode, which is a suggestion of David Lowe to get
   * an aproximative solution (accurate enough) in much less time.
   *
   * This classificator differs a little bit from the other LTI-Lib
   * classificators.  Since the whole training set is stored as sample points,
   * it is useful in many applications to obtain, besides the winner class,
   * the exact winner samples.  Therefore, this class administrates two sets
   * of ID numbers.  One set for the classe IDs, which is used the same way
   * than all other LTI-Lib classifiers, and a second set that administrates
   * IDs for each sample point.  This second set can be explicitelly given, or
   * generated automatically.  You can then for example use some tables
   * containing additional information for each winner point, that are accessed
   * using the point ID.
   * 
   * Example:
   *
   * \code
   *
   * #include <iostream>
   * #include <fstream>
   *
   * #include "ltiKNNClassifier.h"
   * #include "ltiLispStreamHandler.h"
   *
   * // ...
   *
   * double inData[] = {-1,-1,
   *                    -1, 0,
   *                    -1,+1,
   *                    +0,+1,
   *                    +1,+1,
   *                    +1,+0,
   *                    +1,-1,
   *                    +0,-1,
   *                    +0,+0};
   *
   * lti::dmatrix inputs(9,2,inData);     // training vectors
   *
   * int idsData[] = {1,0,2,1,0,2,1,0,1}; // and the respective ids
   * lti::ivector ids(9,idsData);
   *
   * lti::knnClassifier knn;  // our nearest neighbor classifier
   *
   * lti::knnClassifier::parameters param;
   * param.useReliabilityMeasure = false;
   * knn.setParameters(param);
   *
   * // we want to see some info while training
   * streamProgressInfo prog(std::cout);
   * knn.setProgressObject(prog);
   *
   * // train the network
   * knn.train(inputs,ids);
   *
   * // let us save our network for future use
   * // in the file called mlp.dat
   * std::ofstream out("knn.dat");
   * lti::lispStreamHandler lsh(out);
   *
   * // save the network
   * knn.write(lsh);
   * // close the file
   * out.close();
   *
   * // show some results with the same training set:
   *
   * lti::knnClassifier::result outv; // here we will get some
   *                                        // classification results
   * std::cout << std::endl << "Results: " << std::endl;
   *
   * int id;
   * dvector sample(2,0.0);
   * // generate some points and check which would be the winner class
   * for (sample.at(0) = -1; sample.at(0) <= 1; sample.at(0)+=0.25) {
   *   for (sample.at(1) = -1; sample.at(1) <= 1; sample.at(1)+=0.25) {
   * 
   *     knn.classify(sample,outv);
   *     std::cout << "Input " << sample << " \tOutput: ";
   *     outv.getId(outv.getWinnerUnit(),id);
   *     std::cout << id;
   *     std::cout << std::endl;
   *   }
   * }
   *
   * \endcode
   */
  class knnClassifier : public supervisedInstanceClassifier {
  public:
    
    /**
     * If useReliabilityMeasure is true, then the weight of a point
     * can be determined using the ratio of two distances, but there
     * are many possiblities to consider this ratio.  Several simple
     * modes are provided here.  Let \e d1 be the distance to the
     * winner sample point and \e d2 the distance to the closest point
     * belonging to a different class than the winner.
     */
    enum eReliabilityMode {
      Linear,      /**< The weight is determined as 
                      min(1.0,((d2/d1)-1)/(threshold-1))).  The threshold
                      must be strict greater than 1.0 */
      Exponential  /**< The weight is determined as
                      1.0 - exp(-(d2/d1 - 1)/threshold) */
    };


    /**
     * The parameters for the class knnClassifier
     */
    class parameters : public supervisedInstanceClassifier::parameters {
    public:

      /**
       * Default constructor.
       */
      parameters();

      /**
       * Copy constructor.
       *
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
       * Copy the contents of a parameters object.
       *
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& copy(const parameters& other);

      /**
       * Copy the contents of a parameters object.
       *
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& operator=(const parameters& other);

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
       * How many nearest neighbors should be determined per classification.
       *
       * Default value: 1 (i.e. nearest neighbor classifier)
       */
      int kNN;

      /**
       * Normalize data to equal number of data samples.
       * 
       * The traditional k-NN classificator assumes that the a-priori
       * probability of a class is given as the number of patterns belonging
       * to the class divided by the total number of patterns.  In many
       * recognition classifications, however, the classes are all
       * equiprobable.  If normalizeData is set to true this second
       * alternative will be chosen.  In other words, if set to true, the
       * samples will be weighted relative to the number of samples per class.
       * If set to false, each sample has the weight 1.
       *
       * Default value is true
       */
      bool normalizeData;

      /**
       * Normalize the output vector.
       *
       * The k-nearest neighbor algorithm counts how many elements per class
       * are present in the nearest k points to the test point.  This voting
       * can be altered by the \a normalizeData parameter to count not one
       * per class, but 1/nc, where nc is the number of elements of the
       * corresponding class in the training set.
       *
       * The output can be returned "as is" setting this parameter to false,
       * or can be normalized to become a probability value setting this
       * parameter to true.
       *
       * Default value: true
       */
      bool normalizeOutput;

      /**
       * @name Reliability
       */
      //@{
      /**
       * Use the reliability measure suggested by Lowe. 
       *
       * Lowe suggested in his paper: "Distinctive Image Features from
       * Scale Invariant Keypoints", June, 2003 the use of a
       * reliability measure for classification.  It is defined as
       * the ratio between the distance from the analyzed point \a p
       * to the closest sample point \a w, and the distance from the
       * same point \a p to the closest point that belongs to a class
       * different to the one of the point \a w.
       *
       * You usually use this mode with \c kNN=1.  The normalization of the
       * output should be deactivated.
       * 
       * Default value: false
       */
      bool useReliabilityMeasure;

      /**
       * Reliability mode used.
       *
       * For possible values see eReliabilityMode.
       *
       * Default value: Linear
       */
      eReliabilityMode reliabilityMode;

      /**
       * Threshold value used for the reliability function
       *
       * Default value: 10, i.e. distances ratios greater than d2/d1 should be
       *                    consider with the same weight.
       */
      double reliabilityThreshold;

      /**
       * Maximal number of neighbors considered while detecting the
       * second point belonging to another class than the winner.  If
       * no point was found within this number of points, a "perfectly"
       * reliable point will be assumed.
       *
       * Default value: 20
       */
      int maxUnreliableNeighborhood;

      //@}

      /**
       * @name Nearest neighbor search options
       *
       * The search is optimized using a kd-Tree.  The parameters in
       * this group control how the tree is organized or how to search
       * for the data therein.
       */
      //@{
      /**
       * Best Bin First.
       *
       * If this parameter is set to true, the Best Bin First (BBF) algorithm
       * of Lowe et. al will be applied.  It is an approximative algorithm
       * appropriate for spaces of relative high dimensionality (100 or so)
       * in which some improbable bins are discarded in the search.
       *
       * Note that if you activate this, the result is possible optimal, but
       * not optimal.
       *
       * Default value: false
       */
      bool bestBinFirst;

      /**
       * Maximum visit number per leaf node allowed.
       *
       * This is only required if you specify a best-bin-first (BBF)
       * search.  It is the maximal number of visits allowed for leaf
       * nodes. (in the original paper is called Emax).  
       *
       * Usually this value depends on many factors.  You can set it
       * as a percentage of the expected number of leaf nodes
       * (approximately number of points/bucketSize).
       *
       * Default value: 100, but you should set it in order for BBF to work
       *                     appropriatelly.
       */
      int eMax;

      /**
       * Bucket size.
       *
       * Each node of the kd-Tree can contain several points.  The search
       * within a node is made with linear search, i.e. the brute force method
       * computing the distances to each single point.  This parameter 
       * indicates the number of points that will be stored in the node.
       * The name "bucket" comes from the original kd-Tree paper.
       *
       * Default value: 5
       */
      int bucketSize;
      //@}
    };

    /**
     * Default constructor
     */
    knnClassifier();

    /**
     * Default constructor
     */
    knnClassifier(const parameters& params);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    knnClassifier(const knnClassifier& other);

    /**
     * Destructor
     */
    virtual ~knnClassifier();

    /**
     * Returns the name of this class.
     */
    virtual const std::string& name() const;

    /**
     * Copy data of "other" clustering.
     * @param other the clustering to be copied
     * @return a reference to this clustering object
     */
    knnClassifier& copy(const knnClassifier& other);

    /**
     * Alias for copy member.
     * @param other the clustering to be copied
     * @return a reference to this clustering object
     */
    knnClassifier& operator=(const knnClassifier& other);

    /**
     * Returns a pointer to a clone of this classifier.
     */
    virtual knnClassifier* clone() const;

    /**
     * Returns a pointer to a new instance of this classifier.
     */
    virtual knnClassifier* newInstance() const;

    /**
     * Return used parameters
     */
    const parameters& getParameters() const;

    /**
     * Supervised training.  The vectors in the <code>input</code> matrix
     * are arranged row-wise, i.e. each row contains one data vector.
     * The <code>ids</code> vector contains the class label for each row.
     *
     * This is an alternative method to trainObject().  You cannot add further 
     * objects after you have called train(), nor can you call train()
     * after calling trainObject(), since all data provided with trainObject()
     * would be removed.  In other words, you must decide if you want to
     * supply all objects separately or if you want to give them all
     * simultaneously, but you cannot combine both methods.
     *
     * As ids for each feature point the index of the corresponding matrix
     * row will be used.
     *
     * @param input the matrix with the input vectors (each row is a training
     *              vector)
     * @param ids vector of class ids for each input point
     *
     * @return true if successful, false otherwise. (if false you can check
     *              the error message with getStatusString())
     */
    virtual bool train(const dmatrix& input,
                       const ivector& ids);

    /**
     * Supervised training.  The vectors in the <code>input</code> matrix
     * are arranged row-wise, i.e. each row contains one data vector.
     * The <code>ids</code> vector contains the class label for each row.
     *
     * This is an alternative method to trainObject().  You cannot add further 
     * objects after you have called train(), nor can you call train()
     * after calling trainObject(), since all data provided with trainObject()
     * would be removed.  In other words, you must decide if you want to
     * supply all objects separately or if you want to give them all
     * simultaneously, but you cannot combine both methods.
     *
     * As ids for each feature point the index of the corresponding values
     * will be used.
     *
     * @param input the matrix with the input vectors (each row is a training
     *              vector)
     * @param ids vector of class ids for each input point
     * @param pointIds vector containing the ids for each single feature point
     *                 of the training set.
     *
     * @return true if successful, false otherwise. (if false you can check
     *              the error message with getStatusString())
     */
    virtual bool train(const dmatrix& input,
                       const ivector& ids,
                       const ivector& pointIds);

    /**
     * Adds an object to this classifier. The id is given automatically
     * and returned in the parameter.
     *
     * After you have trained several objects, you must call the build() 
     * method to finish the training process.  If you don't do it, the 
     * classifier will ignore everything you have provided.
     *
     * This is an alternative method to train().  You cannot add further 
     * objects after you have called train, nor can you call train()
     * after calling this method, since all data provided with trainObject
     * would be removed.  In other words, you must decide if you want to
     * supply all objects separately or if you want to give them all
     * simultaneously, but you cannot combine both methods.
     *
     * Note that the difference with the method trainObjectId() is that here
     * you receive as as returned argument the id assigned to the object,
     * while in the method trainObjectId() you decide which id should be
     * used for the given object.
     *
     * As id for each point in the given matrix will be used the row index 
     * plus the number of points trained until now, i.e. just the successive
     * numeration of each sample point will be continued.
     *
     * @param input each row of this matrix represents a point in the feature
     *              space belonging to one single class.
     * @param id    this id will be used for the class represented by the
     *              points in the \a input matrix.
     * @return true if successful, false otherwise.
     */
    virtual bool trainObject(const dmatrix& input, int& id);

    /**
     * Adds an object to this classifier. The id is given automatically
     * and returned in the parameter.
     *
     * After you have trained several objects, you must call the build() 
     * method to finish the training process.  If you don't do it, the 
     * classifier will ignore everything you have provided.
     *
     * This is an alternative method to train().  You cannot add further 
     * objects after you have called train, nor can you call train()
     * after calling this method, since all data provided with trainObject
     * would be removed.  In other words, you must decide if you want to
     * supply all objects separately or if you want to give them all
     * simultaneously, but you cannot combine both methods.
     *
     * Note that the difference with the method trainObjectId() is that here
     * you will receive as a returned argument the id assigned to the object,
     * while in the method trainObjectId() you decide which id should be
     * used for the given object.
     *
     * @param input each row of this matrix represents a point in the feature
     *              space belonging to one single class.
     * @param id    this id will be used for the class represented by the
     *              points in the \a input matrix.
     * @param pointIds each point in the \a input matrix will have its own
     *        id    given by the entries of this vector, which must have
     *              a size equal to the number of rows of \a input.
     *
     * @return true if successful, false otherwise.
     */
    virtual bool trainObject(const dmatrix& input, 
                             int& id,
                             const ivector& pointIds);

    /**
     * Adds an object to this classifier. The object ID is given by the user.
     *
     * After you have trained several objects, you must call the build() 
     * method to finish the training process.  If you don't do it, the 
     * classifier will ignore everything you have provided.
     *
     * This is an alternative method to train().  You cannot add further 
     * objects after you have called train, nor can you call train()
     * after calling this method, since all data provided with trainObject
     * would be removed.  In other words, you must decide if you want to
     * supply all objects separately or if you want to give them all
     * simultaneously, but you cannot combine both methods.
     *
     * Note that the difference with the method trainObject() is that here you
     * directly provide the id to be used for the object, while the
     * trainObject() method returns one id that is computed automatically.
     *
     * As id for each point in the given matrix will be used the row index 
     * plus the number of points trained until now, i.e. just the successive
     * numeration of each sample point will be continued.
     *
     * @param input each row of this matrix represents a point in the feature
     *              space belonging to one single class.
     * @param id    this id will be used for the class represented by the
     *              points in the \a input matrix.
     * @return true if successful, false otherwise.
     */
    virtual bool trainObjectId(const dmatrix& input,
                               const int id);

    /**
     * Adds an object to this classifier. The object ID is given by the user.
     *
     * After you have trained several objects, you must call the build() 
     * method to finish the training process.  If you don't do it, the 
     * classifier will ignore everything you have provided.
     *
     * This is an alternative method to train().  You cannot add further 
     * objects after you have called train, nor can you call train()
     * after calling this method, since all data provided with trainObject
     * would be removed.  In other words, you must decide if you want to
     * supply all objects separately or if you want to give them all
     * simultaneously, but you cannot combine both methods.
     *
     * Note that the difference with the method trainObject() is that here you
     * directly provide the id to be used for the object, while the
     * trainObject() method returns one id that is computed automatically.
     *
     * @param input each row of this matrix represents a point in the feature
     *              space belonging to one single class.
     * @param id    this id will be used for the class represented by the
     *              points in the \a input matrix.
     * @param pointIds each point in the \a input matrix will have its own
     *                 ID, given by the entries in this vector, which must have
     *                 the same size than the number of rows of \a input.
     * @return true if successful, false otherwise.
     */
    virtual bool trainObjectId(const dmatrix& input,
                               const int id,
                               const ivector& pointIds);

    /**
     * Classification.
     *
     * Classifies the feature and returns the %object with
     * the classification result.
     *
     * <b>NOTE:</b> This method is NOT really const. Although the main
     * members of the knnClassifier are not changed some state variables used
     * for efficiency are. Thus, it is not save to use the same instance of
     * the knnClassifier in two different threads.
     *
     * @param feature pattern to be classified
     * @param res of the classifications as a classifier::result
     * @return true if the classification has been successful
     */
    virtual bool classify(const dvector& feature, 
                                result& res) const;
    
    /**
     * Classification.
     *
     * Classifies all features (the rows of the matrix) and returns
     * the result %object with the classification result.
     *
     * The classification will be the accumulation of the voting for all
     * given points, assuming that they all belong to the same class.
     *
     * <b>NOTE:</b> This method is NOT really const. Although the main members
     * of the knnClassifier are not changed some state variables used for
     * efficiency are. Thus, it is not save to use the same instance of the
     * knnClassifier in two different threads.
     *
     * @param features patterns to be classified each row is one feature
     * @param res of the classifications as a classifier::result
     * @return true if the classification has been successful
     */
    virtual bool classify(const dmatrix& features, 
                                result& res) const;

    /**
     * Classification.
     *
     * Classifies all features (the rows of the matrix) and returns
     * for each of them a vector of unnormalized probabilities, coded in the
     * rows of the \a result matrix.
     *
     * Since no classifier::result is constructed, only the
     * classification "raw data" is produced.  
     *
     * This method is used in recognition tasks based on many local hints, for
     * which the individual classification of each feature vector would cost
     * too much time.
     *
     * Each column of the output matrix represent one object.  To get 
     * the id represented by a vector column you can use the outputTemplate
     * of the classifier:
     *
     * \code
     * knnClassifier knn;
     *
     * knn.train(data);
     *
     * int columnId = knn.getOutputTemplate().getIds().at(columnNumber);
     * \endcode
     *
     * or the shortcut method of this class getColumnId()
     *
     * <b>NOTE:</b> This method is NOT really const. Although the main members
     * of the knnClassifier are not changed some, state variables used for
     * efficiency are. Thus, it is not save to use the same instance of the
     * knnClassifier in two different threads.
     *
     * @param features patterns to be classified each row is one feature
     * @param res of the classifications as a classifier::result
     * @return true if the classification has been successful
     */
    virtual bool classify(const dmatrix& features, 
                                dmatrix& res) const;

    /**
     * Shortcut method to comfortably access the object id for the column
     * of the result matrix of the classify(const dmatrix&,dmatrix&) method.
     *
     * It returns a negative value if the input column index is invalid.
     */
    inline int getColumnId(const int columnId) const {
      if (static_cast<uint32>(columnId)<static_cast<uint32>(nClasses_)) {
        return getOutputTemplate().getIds().at(columnId);
      }
      return -1;
    }


    /**
     * Information about a feature point.
     *
     * If you classify a point in the feature space and you are interested
     * in all available information about this point, this structure type will
     * be used.  
     */
    struct pointInfo {
      /**
       * Default constructor.
       *
       * Initializes all members with invalid values (negative Ids and null
       * pointers)
       */
      pointInfo();

      /**
       * Constant reference to the feature point
       */
      const dvector* point;

      /**
       * Class ID for the point
       */
      int classId;

      /**
       * ID for the point itself
       */
      int pointId;

      /**
       * Distance to the test point
       */
      double distance;      
    };

    /**
     * Classification.
     *
     * Classifies the feature and returns the result %object with
     * the classification result.
     *
     * <b>NOTE:</b> This method is NOT really const. Although the main
     * members of the knnClassifier are not changed some state variables used
     * for efficiency are. Thus, it is not save to use the same instance of
     * the knnClassifier in two different threads.
     *
     * @param feature pattern to be classified
     * @param res of the classifications as a classifier::result
     * @param points vector sorted in increasing order of the distances to
     *               the feature point and containing two ID numbers.  The
     *               first one corresponds to the class id, the second one
     *               to the point id.  Also a const pointer to the feature
     *               point of the train set and the distance to that point
     *               are contained in the pointInfo structure
     * @return true if the classification has been successful
     */
    virtual bool classify(const dvector& feature, 
                          result& res,
                          std::vector<pointInfo>& points) const;
    
    /**
     * Get only the nearest point to the given vector.
     *
     * Sometimes it is not necessary to have the probability distribution for
     * the objects computed with the classify() methods.  Only the nearest
     * point can be of interest.  This method provides an efficient way to
     * just search for the nearest point and obtain its data.
     *
     * @param feature reference multidimensional point
     * @param nearestPoint nearest point in the training set to the presented
     *                     point.
     * @return true if search was successful, false otherwise.
     */
    virtual bool nearest(const dvector& feature,
                         pointInfo& nearestPoint) const;
    

    /**
     * write the classifier in the given ioHandler
     * @param handler the ioHandler to be used
     * @param complete if true (the default) the enclosing begin/end will
     *        be also written, otherwise only the data block will be written.
     * @return true if write was successful
     */
    virtual bool write(ioHandler& handler,const bool complete=true) const;
    
    /**
     * read the classifier from the given ioHandler
     * @param handler the ioHandler to be used
     * @param complete if true (the default) the enclosing begin/end will
     *        be also written, otherwise only the data block will be written.
     * @return true if write was successful
     */
    virtual bool read(ioHandler& handler,const bool complete=true);
    
    /**
     * Finish a training process.
     *
     * If you used the methods trainObject() or trainObjectId() you must
     * call this method in order to complete the training process.
     *
     * If you used one of the train() methods, you must avoid calling this
     * method.
     *
     * Remember that the "incremental" training mode with trainObject() or
     * trainObjectId() cannot be combined with a "at-once" training step
     * using the method train().
     *
     * @see trainObject(),trainObjectId,reset()
     */
    void build();
    
    /**
     * Resets all values and deletes the content.
     *
     * If you want to forget the sample points and start giving new points
     * with trainObject, you need to call this method first
     */
    void clear();
  
  protected:

    /**
     * @name Reliability weighting functions
     */
    //@{
    static double linear(const double& ratio,const double& threshold);
    static double exponential(const double& ratio,const double& threshold);
    //@}

    /**
     *  mapping of the ids
     */    
    void buildIdMaps(const ivector& ids);
    
    /**
     * Define the output template
     */
    void defineOutputTemplate();

    /**
     * Type for maps mapping ids from internal to external and viceversa
     */
    typedef std::map<int,int> idMap_type;

    /**
     * Map from external id to internal id, used for training
     */
    idMap_type idMap_;

    /**
     * Map from internal to external id, used for training
     */
    idMap_type rIdMap_;

    /**
     * Number of classes currently in the classifier
     */
    int nClasses_;

    /**
     * Exact kd-Tree type used for the database
     *
     * The data in the tree is composed by two id numbers:
     * - The first component contains the object or class id
     * - The second component contains the point id, since using kNN it
     *   can be necessary to know exactly which points in the training set
     *   were the winners.
     */
    typedef kdTree< dvector, std::pair<int,int> > tree_type;
    

    /**
     * The database with accelerated nearest neighbor search.
     *
     * The kdTree uses as n-dimensional points dvectors and as data requires
     * a std::pair containing the class id and the point id.
     */
    tree_type databaseTree_;

    /**
     * Optionally, a scalar weight for each can be applied, as a-priori value.
     *
     * The std::vector is used due to the push_back interface.
     *
     * It is accessed with the internal id.
     */
    std::vector<double> classWeight_;

    /**
     * Minimum number of points per class.
     *
     * This attribute is valid only after the complete training process
     */
    int minPointsPerClass_;

    /**
     * Maximum number of points per class
     *
     * This attribute is valid only after the complete training process
     */
    int maxPointsPerClass_;

    /**
     * Helper for classification
     */
    bool classify(const dvector& feature, 
                        result& output,
                     std::multimap<double,tree_type::element*>& resList) const;

  };

}

#endif
