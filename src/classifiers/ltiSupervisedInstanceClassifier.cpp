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

/**
 * \file   ltiSupervisedInstanceClassifier.cpp
 * \author Peter Doerfler
 * \date   29.08.2001
 *
 * $Id: ltiSupervisedInstanceClassifier.cpp,v 1.1 2008-03-18 21:01:19 alvarado Exp $
 */

#include "ltiObject.h"
#include "ltiClassifier.h"
#include "ltiSupervisedInstanceClassifier.h"

#include <map>

namespace lti {

  supervisedInstanceClassifier::supervisedInstanceClassifier()
    : classifier() {}

  supervisedInstanceClassifier::
  supervisedInstanceClassifier(const supervisedInstanceClassifier& other)
    : classifier(other)  {
  }

  // copy member
  supervisedInstanceClassifier& supervisedInstanceClassifier::
  copy(const supervisedInstanceClassifier& other) {

    classifier::copy(other);
    return (*this);

  }

  bool supervisedInstanceClassifier::makeOutputTemplate(const int& outSize,
                                                        const dmatrix& data,
                                                        const ivector& ids) {

    bool allOk=true;
    int i, j, k;
    imatrix clCount(outSize,outSize,0);
    result outV;

    //map the actual ids to numbers between 0 and n-1
    std::map<int, int> realToIntern;
    ivector internToReal(outSize);
    j=0;
    for (i=0; i<ids.size(); i++) {
      if (realToIntern.find(ids.at(i))==realToIntern.end()) {
        realToIntern[ids.at(i)]=j;
        internToReal.at(j)=ids.at(i);
        j++;
      }
    }

    //initialize outTemplate with values from 0 to n-1 matching the
    //positions in the outTemplate
    ivector ideez(outSize);
    for (i=0; i<outSize; i++) {
      ideez.at(i)=i;
    }
    outTemplate_=outputTemplate(ideez);

    //classify all data and count 'hits' for each position
    for(i=0;i<data.rows();i++) {
      allOk = classify(data[i], outV) && allOk;
      clCount[outV.findMaximumPosition()][realToIntern[ids[i]]]++;
    }

    double rowsum;
    int rowsize;
    outTemplate_=outputTemplate(outSize);
    for(i=0;i<outSize;i++) {
      rowsum=clCount[i].computeSumOfElements();
      result rowV;
      if (rowsum!=0) {
        rowsize=0;
        for (j=0;j<outSize;j++) {
          if (clCount[i][j]!=0) {
            rowsize++;
          }
        }
        rowV=result(rowsize);
        for(j=0,k=0;j<outSize;j++) {
          if (clCount[i][j]!=0) {
            rowV.setPair(k++, internToReal[j], clCount[i][j]/rowsum);
          }
        }
      } else {
        allOk=false;
      }
      outTemplate_.setProbs(i, rowV);
    }
    return allOk;
  }

}
