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
 * \file   ltiClassifier.h
 * \author Peter Doerfler
 * \author Pablo Alvarado
 * \date   10.08.2000
 *
 * $Id: ltiClassifier.h,v 1.7 2008-10-15 17:18:55 alvarado Exp $
 */

#ifndef _LTI_CLASSIFIER_H_
#define _LTI_CLASSIFIER_H_

#include "ltiParametersManager.h"
#include "ltiProgressReporter.h"
#include "ltiException.h"
#include "ltiVector.h"
#include "ltiIoHandler.h"

# ifdef min
#   undef min
# endif

# ifdef max
#   undef max
# endif

namespace lti {

  /**
   * Abstract parent class for all classifiers.
   *
   * Classifiers are parametersManager and as such also inherit
   * status. They are also progressReporter, since mostly the training
   * of a classifier takes more than a few seconds. Make sure to
   * implement progress reporting in training functions of derived
   * classes.
   *
   * Classifiers can be divided into two large groups: supervised and
   * unsupervised. These two groups have the base classes
   * supervisedClassifier and unsupervisedClassifier which are
   * subclasses of this class. Examples for the first type are RBF
   * networks, multi layer perceptron, maximum likelihood classifiers
   * etc. The second type comprises statistic and neural clustering
   * methods.
   *
   * All classifiers return a result when classify() is
   * called.
   *
   */
  class classifier : public ioObject,
                     public status,
                     public parametersManager, 
                     public progressReporter {
  public:
    /**
     * Anonymous enum with constants
     */
    enum {
      UnknownObject=-1, /*< UnknownObject constant id */
      NoObject=-2       /*< NoObject constant id */
    };

    /**
     * This type specifies how the output element probability and
     * the probabilities in the list should be combined. See description of
     * outputTemplate.
     */
    enum eMultipleMode {
      Ignore = 0, /*!< Ignore the %object probability */
      Max,        /*!< Set the prob of the id with max prob to 1, others
                       to zero. */
      Uniform,    /*!< Assume that all objects in the list of one output
                       element have the same probability 
                       (1/number of elements).  */
      ObjProb     /*!< Consider the given %object probabilities */
    };
    
    /**
     * All probabilities under this value will be considered as 0
     */
    static const double NullProbability;

    /**
     * Data structure containing classification results.
     *
     * An instance of classifier::result is produce when classifying data with
     * any classifier.
     *
     * The classifier::result contains labels (also called ids) and values
     * (usually probabilities). Typically, each label occurs only once
     * in the classifier::result and all values are between 0 and 1 and form
     * a probability distribution. However, this is not required by
     * definition. Each classifier should document the nature of its
     * classifier::result.
     *
     * In general, an ouputVector looks like this:
     * \image html classifierResult0.png
     *
     * \image latex classifierResult0.eps
     *
     * Some classifiers have multiple outputs for each class or
     * unnormalized values. In case this is not wanted in further
     * processing classifier::result supplies several methods for
     * manipulation of labels and values.
     *
     * The method idMaximize() searches for all multiple ids and
     * erases all but the entry with the highest value. For the above
     * example this leads to the following classifier::result:
     *
     * \image html classifierResultLabelMax.png
     *
     * \image latex classifierResultLabelMax.eps
     *
     * The values for multiple ids are summed using the method
     * idSum(). After execution there is only one entry for each label
     * with the sum of values as new value. For the example the result
     * is:
     * \image html classifierResultLabelSum.png
     *
     * \image latex classifierResultLabelSum.eps
     *
     * Due to algorithmic reasons both methods sort the labels in
     * ascending order. You can check wether multiple labels exist with the
     * method noMultipleIds().
     *
     * For many application it is useful if the values of the
     * ouputVector form a probability distribution over the labels,
     * i.e. all values are greater equal zero and the sum over all
     * values is one. This is accomplished by the method
     * makeProbDistribution(). It sets values below zero to zero and
     * then normalizes the values to sum to one. When applied to the
     * two classifier::results above the method gives the following results:
     *
     * \image html classifierResultLabelMaxProb.png
     *
     * \image html classifierResultLabelSumProb.png
     *
     * \image latex classifierResultLabelMaxProb.eps
     *
     * \image latex classifierResultLabelSumProb.eps
     *
     * A classifier or an independent module can decide that a
     * classification result does not contain any useful
     * informations. Such an classifier::result is marked as rejected by
     * calling setReject() with argument either true or false. The
     * status is queried with isRejected().
     *
     * Some classifiers, instead of rejecting a result or not, give
     * confidence values. These lie between 0 for lowest and 1 for
     * highest confidence. The confidence is set with
     * setConfidenceValue() and queried with getConfidenceValue().
     *
     * \b NOTE: Confidence values are not taken into account when
     * algebraic methods are called.
     *
     * If possible all classifiers should define a winner unit. This
     * is the index (which usually corresponds to the same posisition
     * in an internal result) in the ouputVector which achieved the
     * best result in the classification. The winner unit is specified
     * by calling setWinnerUnit(). If the winner unit is the one with
     * the maximum value in the ouputVector, the method
     * setWinnerAtMax() can be used. The information is retrieved with
     * getWinner().
     *
     * If two or more classifier::results are to be merged or compared it is
     * useful to know whether they describe the same set of
     * labels. The method isCompatible() checks whether another
     * classifier::result contains exactly the same labels as this
     * classifier::result.
     *
     * Other functional groups of methods include:
     * - read-access methods: getId(), getValues(), getValueByPosition(),
     *   getValueById(), getPair(), getValues(), getIds()
     * - write-access methods: setId(), setValueByPosition(),
     *   setValueById(), setPair(), setValues(), setIds(),setIdsAndValues()
     * - searchMethods: find(), findMaximumValue(), findMaximumPosition(),
     *   findMaximumId(),findMaximumPair(), findMinimumValue(),
     *   findMinimumPosition(), findMinimumId(),findMinimumPair()
     * - sorting methods: sortAscending(), sortDescending()
     * - algebraic methods: various methods that add or multiply
     *   classifier::result instances or apply min or max operators.
     */
    class result : public ioObject {
    public:
      
      /**
       * Default Constructor
       */
      result();

      /**
       * Copy Constructor
       */
      result(const result& other);

      /**
       * Constructor. Creates an classifier::result of the given size.
       * @param size size of the classifier::result vector
       */
      result(const int size);

      /**
       * Constructor. Sets the values and the ids.
       */
      result(const ivector& theIds, const dvector& theValues);

      /**
       * copy method. Copy other into this and return this.
       */
      result& copy(const result& other);

      /**
       * copy operator. Copy other into this and return this.
       */
      result& operator=(const result& other);

      /**
       * Returns the name of this class
       */
      virtual const std::string& name() const;

      /**
       * clone function.
       */
      result* clone() const;

      /**
       * newInstance function.
       */
      result* newInstance() const;

      /**
       * Returns the number of elements in the classifier::result
       */
      int size() const;

      /**
       * Finds the given id in the classifier::result and returns its position
       * if contained in the %classifier::result. If the id is not found false
       * is returned and the position set to -1.
       *
       * @param id find this id
       * @param pos the position of the id.
       *
       * @return true if id was found, false otherwise
       */
      bool find(const int id, int& pos);

      /**
       * Sets the values of the classifier::result.
       * The internal vector will be resized to the size of theValues.
       *
       * @param theValues the new values
       */
      void setValues(const dvector& theValues);

      /**
       * Sets the value at the given position of the classifier::result.
       * @param pos vector entry, where the value should be written
       * @param value the new value
       * @return true if successful, false otherwise.
       */
      bool setValueByPosition(const int pos, const double& value);

      /**
       * Sets the value for the given id in the classifier::result.
       * @param id id whose value is to be changed
       * @param value the new value
       * @return true if successful, false otherwise.
       */
      bool setValueById(const int id, const double& value);

      /**
       * Returns a const reference to the values of the %classifier::result.
       */
      const dvector& getValues() const;

      /**
       * Sets the ids of the classifier::result.
       * @param theIds the new ids
       */
      void setIds(const ivector& theIds);

      /**
       * Sets the ids of the classifier::result and the values.
       *
       * The state of the instance will be exactly as after constructing an
       * classifier::result with ids and values.
       *
       * If the sizes of both vectors are not equal, the smallest size will be
       * used and the largest vector will be cut.
       *
       * @param theIds the new ids
       * @param theValues the new values
       */
      void setIdsAndValues(const ivector& theIds,
                           const dvector& theValues);

      /**
       * Returns a const reference to the ids of the %classifier::result
       */
      const ivector& getIds() const;

      /**
       * Sets the id and the value at the given position. If the position
       * does not exist, false is returned.
       * @param pos position to be changed
       * @param id id at that position
       * @param value value at that position
       * @return false on error
       */
      bool setPair(const int pos, const int id, const double& value);

      /**
       * Returns the id at the given position.
       * @param pos position in the vector
       * @param id the id at that position
       * @return true if id returned, false e.g. for invalid pos
       */
      bool getId(const int pos, int& id) const;

      /**
       * Returns the value at the given position
       * @param pos position in the vector
       * @param value the value at that position
       * @return true if id returned, false e.g. for invalid pos
       */
      bool getValueByPosition(const int pos, double& value) const;

      /**
       * Returns the value for the given id. If an id is used more than
       * once the value with the lowest position is returned.
       * @param id id value
       * @param value the value for the id
       * @return true if id returned, false e.g. for invalid pos
       */
      bool getValueById(const int id, double& value) const;

      /**
       * Returns the id and the value at the given position.
       * @param pos position in the vector
       * @param id the id at that position
       * @param value the value at that position
       * @return true if id returned, false e.g. for invalid pos
       */
      bool getPair(const int pos, int& id, double& value) const;

      /**
       * Set the winner unit
       * @param pos position of the winner unit in the vector
       * @return false if pos not between 0..n-1
       */
      bool setWinner(const int pos);

      /**
       * Set the winner unit as the position of the maximum value of the
       * classifier::result.
       * @return position of the winner unit in the vector
       */
      int setWinnerAtMax();

      /**
       * Get position of the winner
       */
      int getWinner() const;

      /**
       * Sets whether result is rejected. Default for reject is false.
       * @param rej true if rejected
       */
      void setReject(const bool rej);

      /**
       * Returns whether the ouputVector was marked as rejected or not.
       */
      bool isRejected() const;

      /**
       * Sets the confidence value for this classifier::result.
       * The value must be between 0 and 1 for lowest and highest confidence.
       * Default is 1.
       * Other values are cropped.
       * @param conf new confidence value betwenn 0 and 1
       */
      void setConfidenceValue(const double& conf);

      /**
       * Returns the confidence value for this classifier::result. Values are
       * between zero (no confidence) and one (total confidence).
       */
      double getConfidenceValue() const;

      /**
       * Sorts the classifier::result so that values are in ascending order.
       * The sort is only executed if the vector is not already sorted.
       * Ids and winner position are adjusted accordingly.
       */
      void sortAscending();

      /**
       * Sorts the classifier::result so that values are in descending order.
       * The sort is only executed if the vector is not already sorted.
       * Ids and winner position are adjusted accordingly.
       */
      void sortDescending();

      /**
       * Lets the classifier::result comply to rules for probability
       * distributions: Values between 0 and 1, sum of values equals
       * 1. This is done by setting values that are lower than zero to
       * zero and afterwards dividing each value by the sum of values.
       */
      void makeProbDistribution();

      /**
       * Checks whether the other classifier::result contains exactly the same
       * ids as this classifier::result.
       * @param other classifier::result to be compared to this
       * @return true if ids are exactly the same.
       */
      bool isCompatible(const result& other) const;

      /**
       * Returns true if there are never two elements of the classifier::result
       * with the same id.
       */
      bool noMultipleIds() const;

      /**
       * If an id is given more than once, the position with the maximum
       * value is keps and all others are erased. The resulting
       * classifier::result is sorted by ids.
       */
      void uniqueIdsMaximumValue();

      /**
       * If an id is given more than once, the sum of the values of that
       * id is computed and only one unit of that id remains. The
       * resulting classifier::result is sorted by ids.
       */
      void uniqueIdsSumValues();

      /**
       * Find the maximum value in the classifier::result
       * @return the maximum value
       */
      double findMaximumValue() const;

      /**
       * Find the position of the maximum value in the classifier::result
       * @return the position of the maximum value
       */
      int findMaximumPosition() const;

      /**
       * Find the id of the maximum value in the classifier::result
       * @return the id of the maximum value
       */
      int findMaximumId() const;

      /**
       * Find the id-value pair with the maximum value in the
       * classifier::result.  This is faster than finding them separately!
       *
       * @param id id of the maximum value
       * @param value the maximum value
       */
      void findMaximumPair(int& id, double& value) const;

      /**
       * Find the minimum value in the classifier::result
       * @return the minimum value
       */
      double findMinimumValue() const;

      /**
       * Find the position of the minimum value in the classifier::result
       * @return the position of the minimum value
       */
      int findMinimumPosition() const;

      /**
       * Find the id of the minimum value in the classifier::result
       * @return the id of the minimum value
       */
      int findMinimumId() const;

      /**
       * Find the id-value pair with the minimum value in the
       * classifier::result.
       *
       * This is faster than finding them separately!
       *
       * @param id id of the minimum value
       * @param value the minimum value
       */
      void findMinimumPair(int& id, double& value) const;

      /**
       * Adds the other classifier::result to this classifier::result.
       * I.e. for each id that exists in both classifier::results the values
       * are added, else the value remains unchanged and the new ids
       * are appended to this classifier::result.
       * @param other the classifier::result to be added to this one
       * @return this classifier::result
       */
      result& add(const result& other);

      /**
       * Adds the two classifier::results and writes the result into this
       * classifier::result.
       * @param a the first summand
       * @param b the second summand
       * @return this classifier::result
       * @see add(const classifier::result& other)
       */
      result& add(const result& a, 
                  const result& b);

      /**
       * Adds the given scalar to each value of the classifier::result
       * @param s the scalar to be added
       * @return this classifier::result
       */
      result& add(const double& s);

      /**
       * Adds the other classifier::result scaled by s to this
       * classifier::result.
       *
       * I.e. for each id that exists in both classifier::results the values
       * are added, else the value remains unchanged and the new ids
       * are appended to this classifier::result.
       * @param other the classifier::result to be added to this one
       * @param s scaling factor for the other
       * @return this classifier::result
       */
      result& addScaled(const result& other, 
                        const double& s);

      /**
       * Multiplies the other classifier::result with this
       * classifier::result.
       *
       * I.e. for each id that exists in both classifier::results the values
       * are multiplied, else the value is zero and the new ids
       * are appended to this classifier::result.
       * @param other the classifier::result to be multiplied to this one
       * @return this classifier::result
       */
      result& mul(const result& other);

      /**
       * Multiplies the two classifier::results and writes the result into this
       * classifier::result.
       * @param a the first factor
       * @param b the second factor
       * @return this classifier::result
       * @see mul(const classifier::result& other)
       */
      result& mul(const result& a, 
                  const result& b);

      /**
       * Multiplies each value of the classifier::result with the given scalar.
       * @param s the scalar
       * @return this classifier::result
       */
      result& mul(const double& s);

      /**
       * Divides each value of the classifier::result by the given scalar.
       * @param s the scalar
       * @return this classifier::result
       */
      result& divide(const double& s);

      /**
       * Calculates the maximum between the other and this classifier::result.
       * I.e. for each id that exists in both classifier::results the
       * maximum is taken, else the value stays unchanged and the new ids
       * are appended to this classifier::result.
       * @param other the classifier::result to be compared to this
       * @return this classifier::result
       */
      result& max(const result& other);

      /**
       * Calculates the maximum between the two classifier::results and writes
       * it into this classifier::result. Analog to inplace version.
       * @param a the first classifier::result
       * @param b the second classifier::result
       * @return this classifier::result
       * @see max(const classifier::result& other)
       */
      result& max(const result& a, 
                  const result& b);

      /**
       * Calculates the minimum between the other and this classifier::result.
       * I.e. for each id that exists in both classifier::results the
       * minimum is taken, else the value is zero and the new ids
       * are appended to this classifier::result.
       * @param other the classifier::result to be compared to this
       * @return this classifier::result
       */
      result& min(const result& other);

      /**
       * Calculates the minimum between the two classifier::results and writes
       * it into this classifier::result. Analog to inplace version.
       *
       * @param a the first classifier::result
       * @param b the second classifier::result
       * @return this classifier::result
       * @see min(const classifier::result& other)
       */
      result& min(const result& a,
                  const result& b);

      /**
       * Write the objectProb in the given ioHandler.
       *
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      bool write(ioHandler& handler,const bool complete=true) const;

      /**
       * Read the objectProb from the given ioHandler.
       *
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      bool read(ioHandler& handler,const bool complete=true);

    protected:

      /**
       * The ids of the the objects for which the corresponding values
       * (usually a probability) stand
       */
      ivector ids_;

      /**
       * The actual results of each output unit. Usually, the values are
       * probabilities but other double values are legitimate as well.
       */
      dvector values_;

      /**
       * The position in the classifier::result that contains the winner
       * element, ie internal id.
       */
      int winner_;

      /**
       * Expresses the confidence in this classifier::result. A value of 0
       * means no confidence, a value of 1 stands for 100% confidence.
       *
       * Default is 1.
       */
      double confidence_;

      /**
       * The classifier::result is rejected when this flag is true
       */
      bool reject_;

      /**
       * The classifier::result is valid only when this flag is true (default).
       * Invalid results should not be used for further processing.
       */
      bool valid_;

      /**
       * Indicates if vector is sorted in ascending order
       */
      bool sortedAsc_;

      /**
       * Indicates if vector is sorted in descending order
       */
      bool sortedDesc_;
    };


    /**
     * The %outputTemplate stores the relation between the different positions
     * (sometimes called internal ids) of a classification result and the ids.
     * Applying the %outputTemplate to such a %vector results in an
     * classifier::result which is not to be confused with the classification
     * result.
     *
     * There are two data structures within the outputTemplate storing the
     * relevant data:
     * - A simple list of ids, one for each element of the classification
     *   result. These are used, when the parameter multipleMode is set to
     *   Ignore. If %Ignore is set, but the data is not available, the
     *   %multipleMode is set to Max temporarily.
     * - For each element of the classification result exists a list of
     *   ids and respective probabilities. They state that, when that
     *   element is activated there is a certain probability that an
     *   input belonging to the class of the id was presented. These
     *   probabilities are usually generated by classifying a data-set and
     *   generating a probability distribution of the ids for the element
     *   of the classification result with the highest value. This data is
     *   used for all values of multipleMode but Ignore. If the data is not
     *   available, %multipleMode is set to %Ignore temprarily.
     *
     * The calculation of the classifier::result using the apply method
     * depends on the value of the parameter multipleType, which is of
     * type eMultipleType. The following settings are available:
     *
     * <dl>
     * <dt>Ignore</dt>
     * <dd>
     *  If default ids have been stored in the %outputTemplate via the
     *  constructor that receives an %ivector, the method setIds or setData
     *  these ids are simply copied to the %classifier::result. I.e. no
     *  statistics about the actual classification performance of the
     *  %classifier are used. If the data is not set, the option Max is used
     *  and false is returned by the apply method.
     * </dd>
     * <dt>Max</dt>
     * <dd>
     *  The probability lists are used. For each element of the classification
     *  result, the id with the highest probability is found and set to one
     *  while all other probabilities for that element are set to zero. This
     *  leads to an %classifier::result which is equal or similar to the one
     *  generated by Ignore. There will be differences, however, if a certain
     *  element of the classification result was trained for one class, but
     *  when building the probability distributions another class caused this
     *  element to have the highest value more frequently. This case can be
     *  seen for the second element in the example below.
     * </dd>
     * <dt>Uniform</dt>
     * <dd>
     *  The probability lists are used. For each of the classification result
     *  the number of ids in the list is found and their probabilities set
     *  to be uniformly distributed. This method puts very little trust in
     *  the data used for generating the probabilities, i.e. that it
     *  represents the true distribution of the data. On the other hand,
     *  it is very susceptible to noise in the data: One misclassified
     *  example can completely change the outcome of future classifications.
     * </dd>
     * <dt>ObjProb</dt>
     * <dd>
     *  The probablity lists are used. The complete information is used.
     *  This has a functionality similar to a rule set: If element A is
     *  activated, then there is a probability of 0.3 for class 1 and 0.7
     *  for class 5. This method works quite well, when the training data
     *  represents the actual distributions quite well, but the classifier
     *  is not able to build the correct models. A typical effect of using
     *  this approach rather than Ignore is that misclassified unknown data
     *  will have greater probability and thus a higher ranking. On the
     *  downside, sometimes data correctly classified by Ignore can be
     *  just misclassified.
     * </dd>
     * </dl>
     *
     * As mentioned above for all cases but Ignore, the
     * %outputTemplate contains a list of class probabilities for each
     * element of the classification result. These are interpreted as
     * dependent probablities: P(o|x) where o stands for
     * the id and x for the position in the classification
     * result. Each element of the classification results is also
     * taken as a probability p(x). Thus the values for each id
     * are calculated as \f$P(o)=\sum_x p(x)\cdot P(o|x)\f$. <br>
     *
     * Here is a short example for the behavior of an outputTemplate
     * when applied to a classification result. The figure shows the
     * classification result on the lefthand side, the default ids
     * which are used with the option Ignore in the middle and the
     * probabiltity lists which are used for Max, Uniform and ObjProb
     * on the righthand side.
     *
     * \image html outputTemplate.png
     *
     * \image latex outputTemplate.eps
     *
     * Depending on the value of multipleMode the following classifier::result
     * is generated by calling apply:
     *
     * <table border=1>
     * <tr>
     *  <td> </td><td>1</td><td>3</td><td>5</td>
     *  <td>6</td><td>17</td><td>22</td><td>41</td>
     * </tr><tr>
     *  <td>Ignore</td><td>0.15</td><td>0.50</td><td>---</td>
     *  <td>---</td><td>0.03</td><td>0.30</td><td>0.02</td>
     * </tr><tr>
     *  <td>Max</td><td>0.15</td><td>---</td><td>---</td>
     *  <td>0.50</td><td>0.03</td><td>0.30</td><td>0.02</td>
     * </tr><tr>
     *  <td>Uniform</td><td>0.15</td><td>0.35</td><td>0.10</td>
     *  <td>0.25</td><td>0.04</td><td>0.10</td><td>0.01</td>
     * </tr><tr>
     *  <td>ObjProb</td><td>0.15</td><td>0.33</td><td>0.05</td>
     *  <td>0.27</td><td>0.04</td><td>0.15</td><td>0.01</td>
     * </tr>
     * </table>
     *
     * If the use of all four options is desired, the constructor
     * outputTemplate(int) receiving an int value must be used. All
     * data can be set using methods setIds, setProbs and/or
     * setData. If the other constructors are used, no space is
     * reserved for the lists of probabilities, since these take much
     * space and some, especially unsupervised, %classifiers do not
     * need or have no means to gather this information.
     */
    class outputTemplate : public ioObject {

    public:

      /**
       * Default constructor. <p>
       * multipleMode is ObjProb.
       */
      outputTemplate();

      /**
       * Copy constructor.
       */
      outputTemplate(const outputTemplate& other);

      /**
       * Constructor. Since a vector of ids is given multipleMode is Ignore and
       * the probability lists are not initialized and thus cannot be set
       * later.
       */
      outputTemplate(const ivector& theIds);

      /**
       * Constructor. The number of output units is
       * given. multipleMode is ObjeProb. Default ids as well as lists
       * of probabilities can be set.
       */
      outputTemplate(const int& size);

      /**
       * Copy member
       */
      outputTemplate& copy(const outputTemplate& other);

      /**
       * Assigment operator (alias for copy(other)).
       * @param other the outputTemplate to be copied
       * @return a reference to the actual outputTemplate
       */
      inline outputTemplate& operator=(const outputTemplate& other) {
        return copy(other);
      }

      /**
       * Clone member
       */
      virtual outputTemplate* clone() const;

      /**
       * New instance method
       */
      virtual outputTemplate* newInstance() const;

      /**
       * Returns the name of this class
       */
      virtual const std::string& name() const;


      /**
       * Change the setting of how the object probabilities of each unit
       * are taken into account when calculating the result. See
       * description of outputTemplate.
       */
      void setMultipleMode(const eMultipleMode& mode);

      /**
       * Get the setting of multipleMode
       */
      eMultipleMode getMultipleMode() const;

      /**
       * Set the default id vector. These are used when multipleMode is set
       * to Ignore.
       */
      bool setIds(const ivector& theIds);

      /**
       * Returns a const reference to the id vector.
       */
      const ivector& getIds() const;

      /**
       * Set the probabilities of one unit. This information must be set for
       * all elements of the classification result. Then is can be used
       * by the apply method when multipleMode is set to one of Max, Uniform
       * or ObjProb.
       * @param pos the posision in the classification result this distribution
       *            is for-
       * @param theIds list of ids of classes possibly correct, when this
       *               position has high probability.
       * @param theValues probabilities of each of these ids.
       * @return false on errer, e.g. illegal pos
       */
      bool setProbs(const int& pos,
                    const ivector& theIds,
                    const dvector& theValues);

      /**
       * Set the probabilities of one unit. This information must be set for
       * all elements of the classification result. Then is can be used
       * by the apply method when multipleMode is set to one of Max, Uniform
       * or ObjProb.
       * @param pos the posision in the classification result this distribution
       *            is for-
       * @param outV list of ids and corresponding probabilities of classes
       *             possibly correct, when this position has high
       *             probability.
       * @return false on error, e.g. illegal pos
       */
      bool setProbs(const int& pos, const result& outV);

      /**
       * Returns a const reference to the probability distribution at
       * the given position of the template.
       */
      const result& getProbs(const int& pos) const;

      /**
       * Set the probabilities and the default id of one unit. This
       * information must be set for all elements of the
       * classification result. Then is can be used by the apply
       * method for any value of multipleMode.
       * @param pos the posision in the classification result this distribution
       *            is for.
       * @param realId the expected or desired id of this posision of the
       *               classification result.
       * @param outV list of ids and corresponding probabilities of classes
       *              possibly correct, when this position has high
       *              probability.
       * @return false on error, e.g. illegal pos
       */
      bool setData(const int& pos, const int& realId,
                   const result& outV);

      /**
       * Returns the number of output units handled by this outputTemplate
       */
      int size() const;

      /**
       * Uses the information stored in the outputTemplate to generate
       * an result from a dvector. See description of
       * outputTemplate for details. The classification result should
       * contain only positive values which are greater for better
       * fit. The best interpretability is obtained if data is a
       * probability distribution.
       * @param data the classification result
       * @param res %result calculted using the outputTemplate.
       * @return false on error (check getStatusString())
       */
      bool apply(const dvector& data, result& res) const;

      /**
       * write the outputTemplate in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      bool write(ioHandler& handler,const bool complete=true) const;

      /**
       * read the outputTemplate from the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      bool read(ioHandler& handler,const bool complete=true);

    protected:

      /**
       * Determines what data is used for calculation of an
       * result from the classification result and the
       * outputTemplate.  See eUseObjectProb and outputTemplate for
       * detailed description.
       */
      eMultipleMode multipleMode_;

      /**
       * Contains one result for each output unit. These
       * hold the probabilities for the ids being correct when this
       * unit is activated.
       */
      std::vector<result> probList_;

      /**
       * List of ids for each output unit.
       */
      ivector defaultIds_;
    };


    // --------------------------------------------------
    // classifier
    // --------------------------------------------------

    /**
     * default constructor
     */
    classifier();

    /**
     * copy constructor
     * @param other the %object to be copied
     */
    classifier(const classifier& other);

    /**
     * destructor
     */
    virtual ~classifier();

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor %object
     */
    classifier& copy(const classifier& other);

    /**
     *
     * Alias for "copy".
     */
    inline classifier& operator=(const classifier& other) {
      return copy(other);
    }

    /**
     * Returns the name of this class
     */
    virtual const std::string& name() const=0;

    /**
     * returns a pointer to a clone of this classifier.
     */
    virtual classifier* clone() const = 0;

    /**
     * returns a pointer to a new instance of this classifier.
     */
    virtual classifier* newInstance() const = 0;

    /**
     * Write the classifier in the given ioHandler.
     *
     * Shortcut to parametersManager::write().
     *
     * @param handler the ioHandler to be used
     * @param complete if true (the default) the enclosing begin/end will
     *        be also written, otherwise only the data block will be written.
     * @return true if write was successful
     */
    virtual bool write(ioHandler& handler,const bool complete=true) const;

    /**
     * Read the classifier from the given ioHandler.
     *
     * Shortcut to parametersManager::read().
     *
     * @param handler the ioHandler to be used
     * @param complete if true (the default) the enclosing begin/end will
     *        be also written, otherwise only the data block will be written.
     * @return true if write was successful
     */
    virtual bool read(ioHandler& handler,const bool complete=true);

    /**
     * Set the classifier's outputTemplate.
     *
     * This member makes a copy of <em>theOutputTemplate</em>: the classifier
     * will keep its own copy of the outputTemplate
     */
    virtual void setOutputTemplate(const outputTemplate& theOutputTemplate);

    /**
     * Get a const reference to the outputTemplate
     */
    const outputTemplate& getOutputTemplate() const;

  protected:
    /**
     * The outputTemplate for this classifier
     */
    outputTemplate outTemplate_;
  };

  /**
   * Read a classifier::eMultipleMode
   *
   * @ingroup gStorable
   */
  bool read(ioHandler& handler,classifier::eMultipleMode& data);

  /**
   * Write a classifier::eMultipleMode
   *
   * @ingroup gStorable
   */
  bool write(ioHandler& handler,const classifier::eMultipleMode& data);
    

}


#endif

