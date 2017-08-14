/*
 * Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006
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

/*
 * \file   ltiKnnClassifier.cpp
 * \author Frederik Lange
 * \author Pablo Alvarado
 * \date   15.06.2003
 *
 * $Id: ltiKnnClassifier.cpp,v 1.1 2008-05-26 02:44:53 alvarado Exp $
 */


#include "ltiObject.h"
#include "ltiSTLIoInterface.h"
#include "ltiKnnClassifier.h"
#include "ltiMath.h"
#include "ltiRound.h"

#include <limits>
#include <cstdio>

#undef _LTI_DEBUG
// #define _LTI_DEBUG 1
#include "ltiDebug.h"

namespace lti {

  // --------------------------------------------------
  // knnClassifier static functions
  // --------------------------------------------------
  double knnClassifier::linear(const double& r,const double& t) {
    return min(1.0,(r-1)/(t-1));
  }

  double knnClassifier::exponential(const double& r,const double& t) {
    return (1.0 - exp(-(r-1.0)/t));
  }

  // --------------------------------------------------
  // knnClassifier::parameters
  // --------------------------------------------------

  // default constructor
  knnClassifier::parameters::parameters()
    : supervisedInstanceClassifier::parameters() {

    kNN = 1;
    normalizeData = true;
    normalizeOutput = true;
    useReliabilityMeasure = false;
    reliabilityMode = Linear;
    reliabilityThreshold = 10.0;
    maxUnreliableNeighborhood = 20;

    bestBinFirst = false;
    eMax = 100;
    bucketSize = 5;

  }

  // copy constructor
  knnClassifier::parameters::parameters(const parameters& other)
    : supervisedInstanceClassifier::parameters() {
    copy(other);
  }

  // destructor
  knnClassifier::parameters::~parameters() {
  }


  // get type name
  const std::string& knnClassifier::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // copy member
  knnClassifier::parameters&
  knnClassifier::parameters::copy(const parameters& other) {
    supervisedInstanceClassifier::parameters::copy(other);

    kNN                   = other.kNN;
    normalizeData         = other.normalizeData;
    normalizeOutput       = other.normalizeOutput;
    useReliabilityMeasure = other.useReliabilityMeasure;
    reliabilityMode       = other.reliabilityMode;
    reliabilityThreshold  = other.reliabilityThreshold;
    maxUnreliableNeighborhood = other.maxUnreliableNeighborhood;
    bestBinFirst          = other.bestBinFirst;
    eMax                  = other.eMax;
    bucketSize            = other.bucketSize;

    return *this;
  }

  // alias for copy member
  knnClassifier::parameters&
  knnClassifier::parameters::operator=(const parameters& other) {
    copy(other);

    return *this;
  }

  // clone member
  knnClassifier::parameters* knnClassifier::parameters::clone() const {
    return new parameters(*this);
  }

  knnClassifier::parameters* knnClassifier::parameters::newInstance() const {
    return new parameters();
  }

  /*
   * write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  bool knnClassifier::parameters::write(ioHandler& handler,
                                         const bool complete) const {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {
      b=b && lti::write(handler, "kNN",kNN);
      b=b && lti::write(handler, "normalizeData",normalizeData);
      b=b && lti::write(handler, "normalizeOutput",normalizeOutput);
      b=b && lti::write(handler, "useReliabilityMeasure",
                        useReliabilityMeasure);      

      switch(reliabilityMode) {
        case Linear:
          b=b && lti::write(handler, "reliabilityMode","Linear");
          break;
        case Exponential:
          b=b && lti::write(handler, "reliabilityMode","Exponential");
          break;
        default:
          b=b && lti::write(handler, "reliabilityMode","Linear");
          handler.setStatusString("Reliability mode unknown!");
          b=false;
      }

      b=b && lti::write(handler, "reliabilityThreshold",reliabilityThreshold);
      b=b && lti::write(handler, "maxUnreliableNeighborhood",
                        maxUnreliableNeighborhood);

      b=b && lti::write(handler, "bestBinFirst",bestBinFirst);
      b=b && lti::write(handler, "eMax",eMax);
      b=b && lti::write(handler, "bucketSize",bucketSize);

    }

    b = b && supervisedInstanceClassifier::parameters::write(handler,false);

    if (complete) {
      b = b && handler.writeEnd();
    }

    return b;
  }

  /*
   * read the parameters from the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also read, otherwise only the data block will be read.
   * @return true if write was successful
   */
  bool knnClassifier::parameters::read(ioHandler& handler,
                                        const bool complete) {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {
      b=lti::read(handler, "kNN",kNN) && b;

      b=lti::read(handler, "normalizeData",normalizeData) && b;
      b=lti::read(handler, "normalizeOutput",normalizeOutput) && b;

      b=lti::read(handler, "useReliabilityMeasure",useReliabilityMeasure) && b;

      std::string str;
      b=lti::read(handler,"reliabilityMode",str) && b;
      if (str == "Exponential") {
        reliabilityMode = Exponential;
      } else if (str == "Linear") {
        reliabilityMode = Linear;
      } else {
        reliabilityMode = Linear;
        handler.setStatusString("Reliability mode symbol not recognized:");
        handler.appendStatusString(str.c_str());
        b=false;
      }
      
      b=lti::read(handler, "reliabilityThreshold",reliabilityThreshold) && b;
      b=lti::read(handler, "maxUnreliableNeighborhood",
                  maxUnreliableNeighborhood) && b;

      b=lti::read(handler, "bestBinFirst",bestBinFirst) && b;
      b=lti::read(handler, "eMax",eMax) && b;
      b=lti::read(handler, "bucketSize",bucketSize) && b;

    }

    b = b && supervisedInstanceClassifier::parameters::read(handler,false);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

  // --------------------------------------------------
  // knnClassifier
  // --------------------------------------------------

  // default constructor
  knnClassifier::knnClassifier() : supervisedInstanceClassifier() {

    clear();

    // create an instance of the parameters with the default values
    parameters defaultParameters;
    // set the default parameters
    setParameters(defaultParameters);
  }

  // default constructor
  knnClassifier::knnClassifier(const parameters& params) 
    : supervisedInstanceClassifier() {

    clear();

    // set the default parameters
    setParameters(params);
  }

  // copy constructor
  knnClassifier::knnClassifier(const knnClassifier& other)
    : supervisedInstanceClassifier()  {
    copy(other);
  }

  // destructor
  knnClassifier::~knnClassifier() {
    databaseTree_.clear();
  }

  // returns the name of this type
  const std::string& knnClassifier::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // copy member
  knnClassifier& knnClassifier::copy(const knnClassifier& other) {
    supervisedInstanceClassifier::copy(other);

    databaseTree_.copy(other.databaseTree_);
    nClasses_=other.nClasses_;
    idMap_=other.idMap_;
    rIdMap_=other.rIdMap_;
    classWeight_=other.classWeight_;

    return (*this);
  }

  // alias for copy member
  knnClassifier& knnClassifier::operator=(const knnClassifier& other) {
    return copy(other);
  }


  // clone member
  knnClassifier* knnClassifier::clone() const {
    return new knnClassifier(*this);
  }

  knnClassifier* knnClassifier::newInstance() const {
    return new knnClassifier();
  }

  // return parameters
  const knnClassifier::parameters&
  knnClassifier::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&classifier::getParameters());
    if(isNull(par)) {
      throw invalidParametersException();
    }
    return *par;
  }

  // -------------------------------------------------------------------
  // The train-methods!
  // -------------------------------------------------------------------

  bool knnClassifier::train(const dmatrix& input, 
                            const ivector& ids) {
    ivector pointIds(ids.size());
    for (int i=0;i<pointIds.size();++i) {
      pointIds.at(i)=i;
    }
    return train(input,ids,pointIds);
  }

  bool knnClassifier::train(const dmatrix& input, 
                            const ivector& ids,
                            const ivector& pointIds) {
    int i,j,intId;

    if (input.rows() != ids.size()) {
      setStatusString("Incompatible data matrix size and ids vector size");
      return false;
    }

    if (input.rows() != pointIds.size()) {
      setStatusString("Incompatible data matrix size and number of ids for" \
                      " points");
      return false;
    }

    clear();

    // initialize the map attributes of the class
    buildIdMaps(ids);

    if (haveValidProgressObject()) {
      getProgressObject().reset();
      getProgressObject().setTitle("k nearest neighbor training");
      getProgressObject().setMaxSteps(4);
    }

    if (haveValidProgressObject()) {
      getProgressObject().step("Initialize data structure.");
    }

    // compute as weights the number of elements per class
    classWeight_.clear();
    classWeight_.resize(nClasses_,0.0);

    if (haveValidProgressObject()) {
      getProgressObject().step("Add samples to the database.");
    }

    // put each "point" (row of the matrix) as element in the kd-Tree
    for (i=0; i<input.rows(); ++i) {
      intId = idMap_[ids[i]];
      // since no id for each point is specified, use just the row
      // index at the input matrix
      databaseTree_.add(input[i], std::make_pair(intId,pointIds[i]) );
      classWeight_[intId] += 1.0;
    }

    // and adjust the weights
    for (i = 0; i < nClasses_; ++i) {
      j = iround(classWeight_[i]);
      minPointsPerClass_ = min(minPointsPerClass_,j);
      maxPointsPerClass_ = max(maxPointsPerClass_,j);
      classWeight_[i] = 1.0 / classWeight_[i];
    }

    if (haveValidProgressObject()) {
      getProgressObject().step("Create database index.");
    }

    build();

    if (haveValidProgressObject()) {
      getProgressObject().step("Training ready.");
    }

    return true;
  }


  /*
   * Adds an object to this classifier. The id is determined automatically
   * and returned in the parameter.
   */
  bool knnClassifier::trainObject(const dmatrix& input, int& id) {
    ivector pointIds(input.rows());
    for (int i=0;i<pointIds.size();++i) {
      pointIds.at(i)=i;
    }
    return trainObject(input,id,pointIds);
  }

  /*
   * Adds an object to this classifier. The id is determined automatically
   * and returned in the argument \a id.
   */
  bool knnClassifier::trainObject(const dmatrix& input, 
                                  int& id,
                                  const ivector& pointIds) {

    if (pointIds.size() != input.rows()) {
      setStatusString("Incorrect number of point ids for the given input");
      return false;
    }

    // use as external id the maximal id used until now + 1
    // since the std::map is stored as sorted tree, its last element has
    // the biggest key.
    id = (*(idMap_.rbegin())).first + 1;
    idMap_[id]=nClasses_;
    rIdMap_[nClasses_]=id;

    // insert the "points" (rows of the matrix) as points in the kd-Tree
    int j;
    for (j=0; j<input.rows(); ++j) {
      databaseTree_.add(input[j], std::make_pair(nClasses_,pointIds.at(j)));
    }
    classWeight_.push_back(1.0/double(input.rows()));

    j = input.rows();
    minPointsPerClass_ = min(minPointsPerClass_,j);
    maxPointsPerClass_ = max(maxPointsPerClass_,j);

    nClasses_++;

    return true;
  }

  /*
   * Adds an object to this classifier. The id is determined automatically
   * and returned in the parameter.
   */
  bool knnClassifier::trainObjectId(const dmatrix& input,const int id) {
    ivector pointIds(input.rows());
    for (int i=0;i<pointIds.size();++i) {
      pointIds.at(i)=i;
    }
    return trainObjectId(input,id,pointIds);
  }

  /*
   * Adds an object to this classifier. The id is determined automatically
   * and returned in the parameter.
   */
  bool knnClassifier::trainObjectId(const dmatrix& input,
                                    const int id,
                                    const ivector& pointIds) {

    // internal id in use
    unsigned int intId = nClasses_;

    idMap_type::iterator it = idMap_.find(id);
    // check if the class id has already been used
    if (it == idMap_.end()) {
      // nope!  no such id used previously, this is a new class
      // use as external id the one given
      idMap_[id]=intId; // from external to internal
      rIdMap_[intId]=id; // from internal to external
      nClasses_++;
    } else {
      // recover internal id
      intId = (*it).second;
    }

    // insert the "points" (rows of the matrix) as points in the kd-Tree
    int j;
    for (j=0; j<input.rows(); ++j) {
      databaseTree_.add(input[j], std::make_pair(intId,pointIds.at(j)));
    }

    j = input.rows();
    double w = (j!=0) ? 1.0/double(j) : 0.0;

    if (classWeight_.size() == intId) {
      classWeight_.push_back(w);
    } else if (intId > classWeight_.size()) {
      classWeight_.resize(intId+1,0.0);
      classWeight_[intId]=w;
    } else {
      // the internal id was already in use
      if (classWeight_[intId] == 0.0) {
        // not really in use, so just update
        classWeight_[intId]=w;
      } else {
        // how many elements belong to the class?
        int n = iround(1.0/classWeight_[intId]);
        j+=n;
        classWeight_[intId]= (j!=0) ? 1.0/double(j) : 0.0;        
      }
    }

    // the {min,max}PointsPerClass will be updated in the build() method

    return true;
  }

  void knnClassifier::defineOutputTemplate() {

    ivector tids(static_cast<const int>(rIdMap_.size()));
    unsigned int i=0;
    idMap_type::const_iterator it;
    for (i=0,it=rIdMap_.begin(); i<rIdMap_.size(); ++i,++it) {
      tids.at(i)=(*it).second;
    }

    outputTemplate tmpOutTemp(tids);
    setOutputTemplate(tmpOutTemp);
  }

  void knnClassifier::buildIdMaps(const ivector& ids) {
    // remove old maps
    idMap_.clear();
    rIdMap_.clear();

    // create reverse id map
    int j=0;
    int i=0;
    for (i=0; i<ids.size(); ++i) {
      // id used by elements i already used?
      if (idMap_.find(ids.at(i)) == idMap_.end()) {
        // no => insert it in both maps
        _lti_debug("Mapping external id " << ids.at(i) << " to " << j 
                   << std::endl);
        rIdMap_[j]=ids.at(i);
        idMap_[ids.at(i)]=j;
        j++;
      }
    }

    // we know by now the real number of classes being used:
    nClasses_=j;
  }



  // -------------------------------------------------------------------
  // The classify-method!
  // -------------------------------------------------------------------

  bool knnClassifier::classify(const dvector& feature, 
                               result& output) const {
    std::multimap<double,tree_type::element*> resList;
    return classify(feature,output,resList);
  }

  bool knnClassifier::classify(const dmatrix& features, 
                                     result& output) const {

    dvector res(nClasses_,0.0);

    const parameters& par = getParameters();

    const int n = features.rows();
    const int newKnn = min(par.kNN,databaseTree_.size());

    int i;

    if (!par.useReliabilityMeasure) {  
      // don't use the reliability measure: simple kNN classifier
      std::list<tree_type::element*> resList;
      std::list<tree_type::element*>::iterator itr;

      for (i = 0; i < n; ++i) {
        // get the k nearest neighbors
        if (par.bestBinFirst) {
          databaseTree_.searchBestBinFirst(newKnn,
                                           features[i],
                                           par.eMax,
                                           resList);
        } else {
          databaseTree_.searchNearest(newKnn,features[i], resList);          
        }

        if (par.normalizeData) {
          for (itr = resList.begin(); itr != resList.end(); ++itr) {
            // each neighbor votes for its corresponding class (internal id)
            // in the weight obtained as the inverse of the number of samples
            // for that class.
            res[(*itr)->data.first] += classWeight_[(*itr)->data.first];
          }
        } else {
          for (itr = resList.begin(); itr != resList.end(); ++itr) {
            // each neighbor votes for its corresponding class.
            res[(*itr)->data.first] += 1.0;
          }
        }
      }
    } else {
      // reliability consideration desired!

      // get the proper reliability function
      double (*reliability)(const double&, const double&) = 0;
      if (par.reliabilityMode == Exponential) {
        reliability = &exponential;
      } else {
        reliability = &linear;
      }

      std::multimap<double,tree_type::element*> resList;
      std::multimap<double,tree_type::element*>::iterator itr;

      // number of samples to be retrieved to ensure k elements and that
      // at least 1 element belongs to another class.
      const int newK = min(max(min(maxPointsPerClass_+1,
                                   par.maxUnreliableNeighborhood),par.kNN),
                           databaseTree_.size());

      double winner;    // distance to the winner sample.
      double nextOther; // distance to the first sample of a different class
                        // than the winner sample.
      bool foundOther;
      int j;

      // use the reliability measure, so kNN is one but not for access!!
      for (i = 0; i < n; ++i) {
        resList.clear(); 
        
        // ensure to get at least 1 element of another class
        if (par.bestBinFirst) {
          databaseTree_.searchBestBinFirst(newK,features[i],par.eMax,resList);
        } else {
          databaseTree_.searchNearest(newK,features[i], resList);          
        }

        itr = resList.begin();
        const int resultID = (*itr).second->data.first;    // the best hit

        // distance between winner and access point
        winner = ((*itr).first);
        
        // get the next point not belonging to the winner class
        itr++;
        while ((itr!=resList.end()) && 
               ((*itr).second->data.first == resultID)) {
          ++itr;
        }
        
        // this should always be true! except if the classifier has only one
        // single class (which doesn't really make sense):
        foundOther = (itr!=resList.end());
        if (foundOther) {
          nextOther = ((*itr).first);
        }


        if (foundOther) {    

          if (par.normalizeData) {
            for (j=0,itr = resList.begin(); j<newKnn; ++itr,++j) {
              // each neighbor votes for its corresponding class (internal id)
              // in the weight obtained as the inverse of the number of samples
              // for that class.
              if (winner>0.0) {
                res[(*itr).second->data.first] += 
                  classWeight_[(*itr).second->data.first] *
                  (*reliability)(nextOther/winner,par.reliabilityThreshold);
              } else {
                res[(*itr).second->data.first] += 
                  classWeight_[(*itr).second->data.first];
              }
            }
          } else {
            for (j=0,itr = resList.begin(); j<newKnn; ++itr,++j) {
              // each neighbor votes for its corresponding class.
              if (winner>0.0) {
                res[(*itr).second->data.first] += 
                  (*reliability)(nextOther/winner,par.reliabilityThreshold);
              } else {
                res[(*itr).second->data.first] += 1.0;
              }
            }
          }

        } else {
          // since no other class was found, we can assume an infinite 
          // difference between the classes.
          if (par.normalizeData) {
            for (j=0,itr = resList.begin(); j<newKnn; ++itr,++j) {
              // each neighbor votes for its corresponding class (internal id)
              // in the weight obtained as the inverse of the number of samples
              // for that class.
              res[(*itr).second->data.first] +=
                classWeight_[(*itr).second->data.first];
            }
          } else {
            for (j=0,itr = resList.begin(); j<newKnn; ++itr,++j) {
              // each neighbor votes for its corresponding class.
              res[(*itr).second->data.first] += 1.0;
            }
          }
        }
      }
    }

    if (outTemplate_.apply(res,output)) {
      output.setWinnerAtMax();
      if (par.normalizeOutput) {
        output.makeProbDistribution();
      } 
      return true;
    }
    return false;
  }

  //
  // Classify many vector separatelly
  //
  bool knnClassifier::classify(const dmatrix& features, 
                                     dmatrix& res) const {

    const parameters& par = getParameters();

    const int n = features.rows();
    const int newKnn = min(par.kNN,databaseTree_.size());

    res.assign(n,nClasses_,0.0);

    int i;

    if (!par.useReliabilityMeasure) {
      // don't use the reliability measure: simple kNN classifier
      std::multimap<tree_type::distance_type,tree_type::element*> resList;
      std::multimap<tree_type::distance_type,
                    tree_type::element*>::iterator itr;

      for (i = 0; i < n; ++i) {
        // get the k nearest neighbors
        if (par.bestBinFirst) {
          databaseTree_.searchBestBinFirst(newKnn,features[i],
                                           par.eMax,resList);
        } else {
          databaseTree_.searchNearest(newKnn,features[i], resList);          
        }

        if (par.normalizeData) {
          for (itr = resList.begin(); itr != resList.end(); ++itr) {
            // each neighbor votes for its corresponding class (internal id)
            // in the weight obtained as the inverse of the number of samples
            // for that class.
            res.at(i,(*itr).second->data.first) =
              classWeight_[(*itr).second->data.first];
          }
        } else {
          for (itr = resList.begin(); itr != resList.end(); ++itr) {
            // each neighbor votes for its corresponding class.
            res.at(i,(*itr).second->data.first) = 1.0;
          }
        }
      }
    } else {
      // reliability consideration desired!

      // get the proper reliability function
      double (*reliability)(const double&, const double&) = 0;
      if (par.reliabilityMode == Exponential) {
        reliability = &exponential;
      } else {
        reliability = &linear;
      }

      std::multimap<double,tree_type::element*> resList;
      std::multimap<double,tree_type::element*>::iterator itr;

      // number of samples to be retrieved to ensure k elements and that
      // at least 1 element belongs to another class.
      const int newK = min(max(min(maxPointsPerClass_+1,
                                   par.maxUnreliableNeighborhood),par.kNN),
                           databaseTree_.size());

      double winner;    // distance to the winner sample.
      double nextOther; // distance to the first sample of a different class
                        // than the winner sample.
      bool foundOther;
      int j;

      // use the reliability measure, so kNN is one but not for access!!
      for (i = 0; i < n; ++i) {
        resList.clear(); 
        
        // ensure to get at least 1 element of another class
        if (par.bestBinFirst) {
          databaseTree_.searchBestBinFirst(newK,features[i],par.eMax,resList);
        } else {
          databaseTree_.searchNearest(newK,features[i], resList);          
        }

        itr = resList.begin();
        const int resultID = (*itr).second->data.first;    // the best hit

        // distance between winner and access point
        winner = ((*itr).first);
        
        // get the next point not belonging to the winner class
        itr++;
        while ((itr!=resList.end()) && 
               ((*itr).second->data.first == resultID)) {
          ++itr;
        }
        
        // this should always be true! except if the classifier has only one
        // single class (which doesn't really make sense):
        foundOther = (itr!=resList.end());
        if (foundOther) {
          nextOther = ((*itr).first);
        }


        if (foundOther) {    

          if (par.normalizeData) {
            for (j=0,itr = resList.begin(); j<newKnn; ++itr,++j) {
              // each neighbor votes for its corresponding class (internal id)
              // in the weight obtained as the inverse of the number of samples
              // for that class.
              if (winner>0.0) {
                res.at(i,(*itr).second->data.first) = 
                  classWeight_[(*itr).second->data.first] *
                  (*reliability)(nextOther/winner,par.reliabilityThreshold);
              } else {
                res.at(i,(*itr).second->data.first) = 
                  classWeight_[(*itr).second->data.first];
              }
            }
          } else {
            for (j=0,itr = resList.begin(); j<newKnn; ++itr,++j) {
              // each neighbor votes for its corresponding class.
              if (winner>0.0) {
                res.at(i,(*itr).second->data.first) = 
                  (*reliability)(nextOther/winner,par.reliabilityThreshold);
              } else {
                res.at(i,(*itr).second->data.first) = 1.0;
              }
            }
          }

        } else {
          // since no other class was found, we can assume an infinite 
          // difference between the classes.
          if (par.normalizeData) {
            for (j=0,itr = resList.begin(); j<newKnn; ++itr,++j) {
              // each neighbor votes for its corresponding class (internal id)
              // in the weight obtained as the inverse of the number of samples
              // for that class.
              res.at(i,(*itr).second->data.first) =
                classWeight_[(*itr).second->data.first];
            }
          } else {
            for (j=0,itr = resList.begin(); j<newKnn; ++itr,++j) {
              // each neighbor votes for its corresponding class.
              res.at(i,(*itr).second->data.first) = 1.0;
            }
          }
        }
      }
    }

    return true;
  }


  // -------------------------------------------------------------------------

  knnClassifier::pointInfo::pointInfo() 
    : point(0),classId(-1),pointId(-1),distance(0.0) {}

  bool knnClassifier::nearest(const dvector& feature, 
                              pointInfo& nearestPoint) const {
    tree_type::element* elem;
    if (databaseTree_.searchNearest(feature,elem,nearestPoint.distance)) {

      nearestPoint.point = &(elem->point);
      nearestPoint.classId = elem->data.first;
      nearestPoint.pointId = elem->data.second;
      // fix the square of the distance to be the distance itself
      nearestPoint.distance = sqrt(nearestPoint.distance);

      return true;
    } 

    nearestPoint.point = 0;
    nearestPoint.classId = -1;
    nearestPoint.pointId = -1;
    nearestPoint.distance = std::numeric_limits<double>::max();

    setStatusString("No point found");    
    return false;
  }


  bool knnClassifier::classify(const dvector& feature, 
                               result& output,
                               std::vector<pointInfo>& points) const {

    std::multimap<double,tree_type::element*> resList;
    if (classify(feature,output,resList)) {
      points.clear();
      points.reserve(getParameters().kNN);
      std::multimap<double,tree_type::element*>::iterator it;
      pointInfo pinf;
      for (it=resList.begin();it!=resList.end();++it) {
        pinf.point    = &((*it).second->point);
        pinf.classId  = (*it).second->data.first;
        pinf.pointId  = (*it).second->data.second;
        pinf.distance = (*it).first;
        points.push_back(pinf);
      }

      return true;
    }
    return false;
  }

  bool knnClassifier::classify(const dvector& feature, 
                               result& output,
                    std::multimap<double,tree_type::element*>& resList) const {

    _lti_debug("Classifying: " << feature << "\n");
    const parameters& par = getParameters();
    dvector res(nClasses_,0.0);
    const int newKnn = min(par.kNN,databaseTree_.size());

    resList.clear();
    std::multimap<double,tree_type::element*>::iterator itr;
    int classId;

    if (!par.useReliabilityMeasure) {  
      // don't use the reliability measure: simple kNN classifier

      // get the k nearest neighbors
      if (par.bestBinFirst) {
        databaseTree_.searchBestBinFirst(newKnn,feature,par.eMax,resList);
      } else {
        databaseTree_.searchNearest(newKnn,feature, resList);          
      }
      
      if (par.normalizeData) {
        for (itr = resList.begin(); itr != resList.end(); ++itr) {
          // each neighbor votes for its corresponding class (internal id)
          // in the weight obtained as the inverse of the number of samples
          // for that class.
          classId = (*itr).second->data.first;
          res[classId] += classWeight_[classId];
        }
      } else {
        for (itr = resList.begin(); itr != resList.end(); ++itr) {
          // each neighbor votes for its corresponding class.
          res[(*itr).second->data.first] += 1.0;
        }
      }
    } else {
      // reliability consideration desired!

      // get the proper reliability function
      double (*reliability)(const double&, const double&) = 0;
      int i;
      if (par.reliabilityMode == Exponential) {
        reliability = &exponential;
      } else {
        reliability = &linear;
      }

      // number of samples to be retrieved to ensure k elements and that
      // at least 1 element belongs to another class.
      const int newK = min(max(min(maxPointsPerClass_+1,
                                   par.maxUnreliableNeighborhood),par.kNN),
                           databaseTree_.size());

      double winner;    // distance to the winner sample.
      double nextOther; // distance to the first sample of a different class
                        // than the winner sample.
      bool foundOther;
      
      // ensure to get at least 1 element of another class
      if (par.bestBinFirst) {
        databaseTree_.searchBestBinFirst(newK,feature,par.eMax,resList);
      } else {
        databaseTree_.searchNearest(newK,feature, resList);          
      }

      itr = resList.begin();
      const int resultID = (*itr).second->data.first;    // the best hit
      
      // distance between winner and access point
      winner = (*itr).first;
        
      // get the next point not belonging to the winner class
      itr++;
      while ((itr!=resList.end()) && ((*itr).second->data.first == resultID)) {
        ++itr;
      }
      
      // this should always be true! except if the classifier has only one
      // single class (which doesn't really make sense):
      foundOther = (itr!=resList.end());
      if (foundOther) {
        nextOther = (*itr).first;
      }

      if (foundOther) {    
        
        if (par.normalizeData) {
          for (i=0,itr = resList.begin(); i<newKnn; ++itr,++i) {
            // each neighbor votes for its corresponding class (internal id)
            // in the weight obtained as the inverse of the number of samples
            // for that class.
            classId = (*itr).second->data.first;
            if (winner>0.0) {
              res[classId] += classWeight_[classId] *
                (*reliability)(nextOther/winner,par.reliabilityThreshold);
            } else {
              res[classId] += classWeight_[classId];
            }
          }
        } else {
          for (i=0,itr = resList.begin(); i<newKnn; ++itr,++i) {
            classId = (*itr).second->data.first;
            // each neighbor votes for its corresponding class.
            if (winner>0.0) {
              res[classId] += 
                (*reliability)(nextOther/winner,par.reliabilityThreshold);
            } else {
              res[classId] += 1.0;
            }
          }
        }

      } else {
        // since no other class was found, we can assume an infinite 
        // difference between the classes.
        if (par.normalizeData) {
          for (i=0,itr = resList.begin(); i<newKnn; ++itr,++i) {
            // each neighbor votes for its corresponding class (internal id)
            // in the weight obtained as the inverse of the number of samples
            // for that class.
            classId = (*itr).second->data.first;
            res[classId] += classWeight_[classId];
          }
        } else {
          for (i=0,itr = resList.begin(); i<newKnn; ++itr,++i) {
            // each neighbor votes for its corresponding class.
            res[(*itr).second->data.first] += 1.0;
          }
        }
      }      
    }

    if (outTemplate_.apply(res,output)) {
      output.setWinnerAtMax();
      if (par.normalizeOutput) {
        output.makeProbDistribution();
      }
      return true;
    }
    return false;

  }

  // -------------------------------------------------------------------------


  void knnClassifier::clear() {
    databaseTree_.clear();
    classWeight_.clear();
    nClasses_=0;
    idMap_.clear();
    rIdMap_.clear();

    minPointsPerClass_ = std::numeric_limits<int>::max();
    maxPointsPerClass_ = 0;
  }

  // parameter shortcuts

  //
  // IO methods
  //

  /*
   * write the classifier in the given ioHandler
   */
  bool knnClassifier::write(ioHandler& handler,const bool complete) const {
    bool b=true;
    if (complete) {
      b=handler.writeBegin();
    }
    
    // save the parameters
    b = b && supervisedInstanceClassifier::write(handler,false);
    if (b) {
      // Write data
      b=b && lti::write(handler, "nClasses",nClasses_);
      b=b && lti::write(handler, "idMap",idMap_);
      b=b && lti::write(handler, "rIdMap",rIdMap_);
      b=b && lti::write(handler, "classWeight",classWeight_);
      b=b && lti::write(handler, "minPointsPerClass",minPointsPerClass_);
      b=b && lti::write(handler, "maxPointsPerClass",maxPointsPerClass_);
      b=b && databaseTree_.write(handler);
    }

    if (complete) {
      b=handler.writeEnd();
    }
    return b;
  }

  /*
   * read the classifier from the given ioHandler
   */
  bool knnClassifier::read(ioHandler& handler,const bool complete) {
    bool b=true;
    if (complete) {
      b=handler.readBegin();
    }
    b = b && supervisedInstanceClassifier::read(handler,false);    
    if (b) {     
      b=b && lti::read(handler, "nClasses",nClasses_); 
      b=b && lti::read(handler, "idMap",idMap_);
      b=b && lti::read(handler, "rIdMap",rIdMap_);
      b=b && lti::read(handler, "classWeight",classWeight_);      
      b=b && lti::read(handler, "minPointsPerClass",minPointsPerClass_);
      b=b && lti::read(handler, "maxPointsPerClass",maxPointsPerClass_);

      b=b && databaseTree_.read(handler);

      defineOutputTemplate();
    } 
    if (complete) {
      b=b && handler.readEnd();
    }
    
    return b;
  }

  // do this if you add single data sets
  void knnClassifier::build() {
    // the min number of elements per class can be wrong, so update it
    if (classWeight_.size() > 0) {
      minPointsPerClass_ = maxPointsPerClass_ = iround(1.0/classWeight_[0]);
    }
    for (unsigned int i = 1; i < classWeight_.size(); ++i) {
      int j = iround(1.0/classWeight_[i]);
      minPointsPerClass_ = min(minPointsPerClass_,j);
      maxPointsPerClass_ = max(maxPointsPerClass_,j);
    }
    
    defineOutputTemplate();
    databaseTree_.build(getParameters().bucketSize);
  }

}
