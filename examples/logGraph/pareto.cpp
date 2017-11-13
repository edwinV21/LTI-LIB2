/*
 * Copyright (C) 2003, 2004, 2005, 2006
 * Pablo Alvarado
 * Lehrstuhl fuer Technische Informatik, RWTH-Aachen, Germany
 * Copyright (C) 2007
 * Pablo Alvarado
 * Instituto Tecnol�gico de Costa Rica
 */

/**
 * \file   pareto.cpp
 *         Utility to create with gnuplot representations of pareto fronts.
 * \author Pablo Alvarado
 * \date   17.07.07
 *
 * $Id: pareto.cpp,v 1.9 2014-03-14 19:52:43 alvarado Exp $
 */


/*
 * Pareto Analysis Tool
 */

#include <ltiRGBAPixel.h>
#include <ltiMatrix.h>
#include <ltiLispStreamHandler.h>
#include <ltiTimer.h>
#include <ltiPoint.h>
#include <ltiSort2.h>
#include <ltiPoint3D.h>

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <fstream>

#include <algorithm>
#include <cstdlib>
#include <cstdio>
#include <getopt.h>

/**
 * Types of files
 */
enum eTypeOfFile {
  ParetoFront, /*< Pareto front only                   */
  Log,         /*< Log with several points             */
  Raw,         /*< Raw data written in a previous step */
  Unknown      /*< Unknown type                        */
};

/**
 * Get type of file
 */
eTypeOfFile getTypeOfFile(const std::string& filename) {
  if (filename.rfind(".data") != std::string::npos) {
    return Raw;
  }

  if (filename.rfind(".pf") != std::string::npos) {
    return ParetoFront;
  }

  if (filename.rfind(".log") != std::string::npos) {
    return Log;
  }

  return Unknown;
}

/**
 * Get data in a pareto file
 */
bool getRawData(const std::string& filename,
                lti::dmatrix& data) {
  std::ifstream in(filename.c_str());
  if (in) {
    float v1,v2,v3,v4;
    std::vector<lti::dvector> vcts;
    vcts.reserve(200); // just a preliminary estimation 200
    lti::dvector aVector;
    std::string str;
    int i;

    std::getline(in,str);

    // try 4D
    if (sscanf(str.c_str(),"%f %f %f %f",&v1,&v2,&v3,&v4) == 4) {
      aVector.resize(4);
      vcts.push_back(aVector);
      vcts[0].at(0)=v1;
      vcts[0].at(1)=v2;
      vcts[0].at(2)=v3;
      vcts[0].at(3)=v4;
    } else if (sscanf(str.c_str(),"%f %f %f",&v1,&v2,&v3) == 3) {
    // try 3D
      aVector.resize(3);
      vcts.push_back(aVector);
      vcts[0].at(0)=v1;
      vcts[0].at(1)=v2;
      vcts[0].at(2)=v3;
    } else if (sscanf(str.c_str(),"%f %f",&v1,&v2) == 2) {
      // try 2D
      aVector.resize(2);
      vcts.push_back(aVector);
      vcts[0].allocate(2);
      vcts[0].at(0)=v1;
      vcts[0].at(1)=v2;
    } else {
      // could not read 2D or 3D
      return false;
    }

    // read the rest of the data
    while (in.good() && !in.eof()) {
      for (i=0;i<aVector.size();++i) {
        in >> aVector[i];
      }
      vcts.push_back(aVector);
    }

    // convert the data into a matrix
    if (!vcts.empty()) {
      data.allocate(vcts.size(),vcts[0].size());
      int i;
      for (i=0;i<data.rows();++i) {
        data.getRow(i).fill(vcts[i]);
      }
      return true;
    } else {
      std::cerr << "File " << filename << " empty." << std::endl;
    }

  } else {
    std::cerr << "File " << filename << " could not be opened." << std::endl;
  }
  return false;

}

/**
 * Get data in a pareto file
 */
bool getParetoData(const std::string& filename,
                  std::vector<lti::dvector>& vcts) {
  std::cout<< "filename" <<filename.c_str() <<"\n";
  std::ifstream in(filename.c_str());
  if (in) {
    lti::lispStreamHandler lsh(in);
    //std::cout<<"reading lispStreamHandler!" <<"\n";
    vcts.reserve(200); // just a preliminary estimation 200
    lti::dvector emptyVector;
    bool ok=false;

    // read first "("
    while (lsh.readBegin()) {
      // two possibilities:
      // 1. BoundingBox symbol (the last one), or
      // 2. A list with the vector of interest and the parameterization
      if (lsh.tryBegin()) {
        // the vector
        vcts.push_back(emptyVector);   // push an empty vector
        ok = vcts[vcts.size()-1].read(lsh,false); // read the vector

        lsh.readEnd();
        //std::cout<<"lsh begin!";
        // the phenotype, that we are not interested in
        if (lsh.tryBegin()) { // only if it is there!
          ok = lsh.readEnd(); // of the phenotype
        }
        ok = lsh.readEnd(); // of the pair vector-phenotype
      } else {
        in.close();
        break;
      }
    }

    if (!ok) {
      std::cerr << "Warning: something went wrong reading file: "
                << filename << std::endl;
    }


    // now convert the vector of vectors in a matrix
    if (!vcts.empty()) {
    //  data.allocate(vcts.size(),vcts[0].size());
    //  int i;
    //  for (i=0;i<data.rows();++i) {
    //    data.getRow(i).fill(vcts[i]);
    //  }
      return true;
    } else {
      std::cout << "File " << filename << " empty." << std::endl;
    }
  } else {
    std::cout << "File " << filename << " could not be opened." << std::endl;
  }
  return false;
}



bool getLogData(const std::string& filename,
                  std::vector<lti::dvector>& vcts) {
  std::cout<< "filename: " << filename.c_str() <<"\n";
  std::ifstream in(filename.c_str());
  if (in) {
    lti::lispStreamHandler lsh(in);

    vcts.reserve(1000); // just a preliminary estimation 1000
    lti::dmatrix bbox;
    lti::dvector emptyVector;
    std::string chromosome;

    // The log is headed by the parameterization used.  We don't need it
    lsh.readBegin();
    lsh.readEnd();


    // read first "("
    while (lsh.readBegin()) {
      // the vector
      vcts.push_back(emptyVector);   // push an empty vector
      vcts[vcts.size()-1].read(lsh); // read the vector
      //std::cout<<"read chromosome" <<  lsh.read(chromosome) <<"\n";
      // the chromosome
      lsh.read(chromosome);
      lsh.readEnd(); // of the pair vector-chromosome
    }

    // now convert the vector of vectors in a matrix
    if (!vcts.empty()) {
    //  data.allocate(vcts.size(),vcts[0].size());
    //  int i;
    //  for (i=0;i<data.rows();++i) {
    //    data.getRow(i).fill(vcts[i]);
    //  }
      return true;
    } else {
      std::cerr << "File " << filename << " empty." << std::endl;
    }
  } else {
    std::cerr << "File " << filename << " could not be opened." << std::endl;
  }
  return false;
}

/**
 * Get data in a log file
 */
/*bool getLogData(const std::string& filename,
                lti::dmatrix& data) {
  std::ifstream in(filename.c_str());
  if (in) {
    lti::lispStreamHandler lsh(in);
    std::vector<lti::dvector> vcts;
    vcts.reserve(1000); // just a preliminary estimation 1000
    lti::dmatrix bbox;
    lti::dvector emptyVector;
    std::string chromosome;

    // The log is headed by the parameterization used.  We don't need it
    lsh.readBegin();
    lsh.readEnd();


    // read first "("
    while (lsh.readBegin()) {
      // the vector
      vcts.push_back(emptyVector);   // push an empty vector
      vcts[vcts.size()-1].read(lsh); // read the vector

      // the chromosome
      lsh.read(chromosome);
      lsh.readEnd(); // of the pair vector-chromosome
    }

    // now convert the vector of vectors in a matrix
    if (!vcts.empty()) {
      data.allocate(vcts.size(),vcts[0].size());
      int i;
      for (i=0;i<data.rows();++i) {
        data.getRow(i).fill(vcts[i]);
      }
      return true;
    } else {
      std::cerr << "File " << filename << " empty." << std::endl;
    }
  } else {
    std::cerr << "File " << filename << " could not be opened." << std::endl;
  }
  return false;
} */

/**
 * Get data from file
 */
eTypeOfFile getData(const std::string& filename,
		    lti::dmatrix& data) {
  switch (getTypeOfFile(filename)) {
    case ParetoFront:

    //  if (getParetoData(filename,data)) {
    //    return ParetoFront;
    //  }
      break;
    case Log:
    //  if (getLogData(filename,data)) {
    //    return Log;
    //  }
      break;
    case Raw:
      if (getRawData(filename,data)) {
        return Raw;
      }
      break;
    default:
      return Unknown;
  }
  return Unknown;
}

bool writeDataFile(std::ostream& out,
                   const lti::dmatrix& data) {
  int y,x;
  for (y=0;y<data.rows();++y) {
    x=0;
    out << data.at(y,x);
    for (x=1;x<data.columns();++x) {
      out << " " << data.at(y,x);
    }
    out << std::endl;
  }
  return true;
}

bool writeDataFile(const std::string& filename,
                   const lti::dmatrix& data) {
  if (data.empty()) {
    std::cerr << "Nothing to write in " << filename << std::endl;

    return false;
  }

  std::ofstream out(filename.c_str());
  if (out) {
    bool res = writeDataFile(out,data);
    return res;
  }

  std::cerr << "File " << filename << " could not be written" << std::endl;
  return false;
}

bool writeDataSlices(std::ostream& out,
                     const std::list<lti::dmatrix>& data) {

  if (data.empty()) {
    return false;
  }

  std::list<lti::dmatrix>::const_iterator it;

  it = data.begin();
  writeDataFile(out,*it);
  for (it++;it!=data.end();++it) {
    out << std::endl << std::endl; // index separation in gnuplot
    writeDataFile(out,*it);
  }
  return true;
}

bool writeDataSlices(const std::string& filename,
                     const std::list<lti::dmatrix>& data) {
  if (data.empty()) {
    std::cerr << "Nothing to write in " << filename << std::endl;

    return false;
  }

  std::ofstream out(filename.c_str());
  if (out) {
    bool res = writeDataSlices(out,data);
    return res;
  }

  std::cerr << "File " << filename << " could not be written" << std::endl;
  return false;
}

/**
 * Pareto Front domination
 */
bool dominate(const lti::dvector& a,
              const lti::dvector& b) {
  bool theOne = false;

  // a little pointer arithmetic to accelerate this frequently called
  // function
  lti::dvector::const_iterator aPtr = a.begin();
  lti::dvector::const_iterator bPtr = b.begin();
  const lti::dvector::const_iterator ePtr = a.end();

  while (aPtr != ePtr) {
    if ((*aPtr) < (*bPtr)) {
      // if any element is smaller => definitively not greater!
      return false;
    } else if ((*aPtr) > (*bPtr)) {
      // only greater if at least one element has been strictly greater
      theOne = true;
    }

    ++aPtr;
    ++bPtr;
  }

  return theOne;
}

/**
 * Get non-dominated elements in the matrix
 */
void getNonDominated(const lti::dmatrix& mat,
                     std::vector<lti::dvector>& nonDom) {
  int i,j;
  bool dominated;
  nonDom.clear();
  for (i=0;i<mat.rows();++i) {
    dominated = false;
    for (j=0;!dominated && (j<mat.rows());++j) {
      dominated = dominate(mat.getRow(j),mat.getRow(i));
    }
    if (!dominated) {
      nonDom.push_back(mat.getRow(i));
    }
  }
}

void getNonDominatedVector(std::vector<lti::dvector>& individuals,
                     std::vector<lti::dvector>& nonDom) {
  unsigned int i,j;
  bool dominated;
  nonDom.clear();
  for (i=0;i<individuals.size();++i) {
    dominated = false;
    for (j=0;!dominated && (j<individuals.size());++j) {
      if(i!=j){
        dominated = dominate(individuals[j],individuals[i]);
      }
    }
    if (!dominated) {
      nonDom.push_back(individuals[i]);
    }
  }

  /*for (i=0;i<static_cast<int>(nonDom.size());++i) {
    if (nonDominated[i]) {
      // remove first all dominated elements in the list
      vit = front.begin();
      while(vit != front.end()) {
        if (dominate(tmp[i],*vit)) {
          vit = front.erase(vit);
        } else {
          ++vit;
        }
      }
      front.push_back(tmp[i]);
    }
  }*/


}

/**
 * Returns true if a < b in a scanning sense
 */
bool compareVectors(const lti::dvector& a,const lti::dvector b) {
  for (int i=a.lastIdx();i>=0;i--) {
    if (a.at(i) < b.at(i)) {
      return false;
    } else if (a.at(i) > b.at(i)) {
      return true;
    }
  }
  return false;
}

bool sortPareto(lti::dmatrix& data) {
  if (data.columns() == 2) {
    std::vector<lti::dpoint> vct(data.rows());
    int i;
    for (i=0;i<data.rows();++i) {
      vct[i].set(data.at(i,0),data.at(i,1));
    }
    std::sort(vct.begin(),vct.end());

    for (i=0;i<data.rows();++i) {
      data.at(i,0) = vct[i].x;
      data.at(i,1) = vct[i].y;
    }
    return true;

  } else if (data.columns() == 3) {
    std::vector<lti::dpoint3D> vct(data.rows());
    int i;
    for (i=0;i<data.rows();++i) {
      vct[i].set(data.at(i,0),data.at(i,1),data.at(i,2));
    }
    std::sort(vct.begin(),vct.end());

    for (i=0;i<data.rows();++i) {
      data.at(i,0) = vct[i].x;
      data.at(i,1) = vct[i].y;
      data.at(i,2) = vct[i].z;
    }

    return true;
  } else {
    std::vector<lti::dvector> vct(data.rows());
    int i;
    for (i=0;i<data.rows();++i) {
      vct[i].copy(data.getRow(i));
    }
    std::sort(vct.begin(),vct.end(),compareVectors);

    for (i=0;i<data.rows();++i) {
      data.getRow(i).copy(vct.at(i));
    }

    return true;
  }

  return false;
}

void removeIndividuals(std::vector<lti::dvector>& front, std::vector<lti::dvector>& nonDominated   ){
  int i;
  //std::vector<lti::dvector>& front;
  for (i=0;i<static_cast<int>(nonDominated.size());++i) {

    for (unsigned j = 0; j < front.size(); j++) {
      if (dominate(nonDominated[i],front[j])) {
        front.erase(  front.begin()+  j  );
        std::cout<< "removing individuals!" <<"\n";
      }
    }
    //front.push_back(nonDominated[i]);

    /*if (nonDominated[i]) {
      // remove first all dominated elements in the list
      vit = front.begin();
      while(vit != front.end()) {
        if (dominate(tmp[i],*vit)) {
          vit = front.erase(vit);
        } else {
          ++vit;
        }
      }
      front.push_back(tmp[i]);
    }*/
  }


}





int computeFront (const std::list<lti::dmatrix> allData,
                   lti::dmatrix& data) {

  if (allData.empty()) {
    return false;
  }

  std::list<lti::dvector> front;
  std::vector<lti::dvector> tmp;

  std::list<lti::dmatrix>::const_iterator it;
  std::list<lti::dvector>::iterator vit;
  //bool dominated;
  int i;

  // for each matrix
  std::vector<bool> nonDominated;



  /*for (it=allData.begin();it!=allData.end();++it) {
    getNonDominated((*it),tmp);
    nonDominated.resize(tmp.size());

    // insert elements in tmp only if they are not dominated by the elements
    // already in the front
    for (i=0;i<static_cast<int>(tmp.size());++i) {
      vit = front.begin();
      dominated = false;
      while(!dominated && (vit != front.end())) {
        dominated = dominate(*vit,tmp[i]);
        ++vit;
      }
      nonDominated[i] = !dominated;
    }

    // insert non-dominated elements in the front, removing the points
    // they dominate
    for (i=0;i<static_cast<int>(tmp.size());++i) {
      if (nonDominated[i]) {
        // remove first all dominated elements in the list
        vit = front.begin();
        while(vit != front.end()) {
          if (dominate(tmp[i],*vit)) {
            vit = front.erase(vit);
          } else {
            ++vit;
          }
        }
        front.push_back(tmp[i]);
      }
    }
  } */

  if (!front.empty()) {
    data.assign(front.size(),front.front().size(),0.0);
    for (i=0,vit=front.begin();vit!=front.end();++i,++vit) {
      data.getRow(i).fill(*vit);
    }
    return true;
  }

  return false;
}


/**
 * Compute the pareto front
 */
bool computePareto(const std::list<lti::dmatrix> allData,
                   lti::dmatrix& data) {

  if (allData.empty()) {
    return false;
  }

  std::list<lti::dvector> front;
  std::vector<lti::dvector> tmp;

  std::list<lti::dmatrix>::const_iterator it;
  std::list<lti::dvector>::iterator vit;
  bool dominated;
  int i;

  // check first if it is possible
  it=allData.begin();
  i = (*it).columns();
  for (++it;it!=allData.end();++it) {
    if (i != (*it).columns()) {
      std::cerr << "Pareto front cannot be computed since dimensionality of \n"
                << "the data files is not compatible\n" << std::endl;
      return false;
    }
  }

  // for each matrix
  std::vector<bool> nonDominated;
  for (it=allData.begin();it!=allData.end();++it) {
    getNonDominated((*it),tmp);
    nonDominated.resize(tmp.size());

    // insert elements in tmp only if they are not dominated by the elements
    // already in the front
    for (i=0;i<static_cast<int>(tmp.size());++i) {
      vit = front.begin();
      dominated = false;
      while(!dominated && (vit != front.end())) {
        dominated = dominate(*vit,tmp[i]);
        ++vit;
      }
      nonDominated[i] = !dominated;
    }

    // insert non-dominated elements in the front, removing the points
    // they dominate
    for (i=0;i<static_cast<int>(tmp.size());++i) {
      if (nonDominated[i]) {
        // remove first all dominated elements in the list
        vit = front.begin();
        while(vit != front.end()) {
          if (dominate(tmp[i],*vit)) {
            vit = front.erase(vit);
          } else {
            ++vit;
          }
        }
        front.push_back(tmp[i]);
      }
    }
  }

  if (!front.empty()) {
    data.assign(front.size(),front.front().size(),0.0);
    for (i=0,vit=front.begin();vit!=front.end();++i,++vit) {
      data.getRow(i).fill(*vit);
    }
    return true;
  }

  return false;
}

// otherCoords contains the 2D data and the resulting 3D will be left there
// too.
void rebuildMatrix(const int sliceDim,
                   const double sliceCoord,
                   lti::dmatrix& otherCoords) {
  lti::dmatrix newMat;
  newMat.allocate(otherCoords.rows(),1+otherCoords.columns());
  int y,x,i;
  for (y=0;y<otherCoords.rows();++y) {
    for (x=0,i=0;x<newMat.columns();++x) {
      if (x!=sliceDim) {
        newMat.at(y,x)=otherCoords.at(y,i);
        ++i;
      } else {
        newMat.at(y,x)  = sliceCoord;
      }
    }
  }

  newMat.detach(otherCoords);
}

bool computeParetoLine3D(const lti::dmatrix& data,
                               lti::dmatrix& result,
                         const lti::dvector& mins) {

  std::list<lti::dmatrix> tmpMat;
  tmpMat.resize(1);
  tmpMat.back().useExternData(data.rows(),
                              data.columns(),
                              const_cast<double*>(&data.at(0,0)));
  computePareto(tmpMat,result);
  sortPareto(result);

  lti::dmatrix dest(result.rows()*2 + 1, result.columns(),0.0);
  int i=0;
  int j=0;
  double lasty = mins.at(1);

  for (;j<result.rows();++j) {
    dest.at(i,0)=result.at(j,0);
    dest.at(i,1)=lasty;
    ++i;
    dest.at(i,0)=result.at(j,0);
    dest.at(i,1)=lasty=result.at(j,1);
    ++i;
  }

  // the last point
  dest.at(i,0)=mins.at(0);
  dest.at(i,1)=lasty;

  dest.swap(result);

  return true;
}


/**
 * compute Pareto 3D
 *
 * This method is an extension of compute pareto, to split a 3D pareto
 * front into several slices
 */
bool computePareto3D(const lti::dmatrix& allData,
                     std::list<lti::dmatrix>& slices,
                     const int sliceDim = 2,
                     const int numSlices = 25) {

  if (allData.columns() < 3) {
    // no 3D pareto
    return false;
  }

  if (sliceDim<0 || sliceDim>allData.lastColumn()) {
    // wrong slice dim
    return false;
  }

  // split the data into the slice coordinate and the rest
  lti::dmatrix data2D(allData.rows(),allData.columns()-1);
  lti::dvector coord;
  allData.getColumnCopy(sliceDim,coord);
  int i,j;
  for (i=0,j=0;i<allData.columns();++i) {
    if (i!=sliceDim) {
      data2D.fill(allData,0,j,data2D.lastRow(),j,0,i);
      ++j;
    }
  }

  // get bounding box
  lti::dvector mins(data2D.columns(),std::numeric_limits<double>::max());
  for (i=0;i<data2D.rows();++i) {
    for (j=0;j<data2D.columns();++j) {
      mins.at(j)=lti::min(data2D.at(i,j),mins.at(j));
    }
  }

  slices.clear();
  slices.resize(numSlices);
  // sort the rows of the matrix according to the slice coordinate
  lti::sort2 sorter;
  sorter.apply(coord,data2D);

  //
  const double step =
    (coord.at(coord.lastIdx()) - coord.at(0))/lti::max(1,numSlices-1);

  // the first and the last slices should be exactly placed at their original
  // coordinates
  std::list<lti::dmatrix>::iterator it = slices.begin();

  lti::dmatrix subdata;
  computeParetoLine3D(data2D,*it,mins);
  rebuildMatrix(sliceDim,coord.at(0),*it);

  // middle slices and last slice
  double c;
  for (j=0,i=1,++it,c=coord.at(0)+step;i<numSlices;++i,c+=step,++it) {
    // compute how many elements belong in the tmpMat
    while((j<coord.size()) && (coord.at(j) <= c)) {
      ++j;
    }
    j--;
    subdata.useExternData(data2D.rows()-j,
                          data2D.columns(),
                          &data2D.at(j,0));
    computeParetoLine3D(subdata,*it,mins);
    rebuildMatrix(sliceDim,coord.at(j),*it);
  }

  return true;
}

/**
 * compute Pareto 3D
 *
 * This method is an extension of compute pareto, to split a 3D pareto
 * front into several slices
 */
bool computePareto3D(const std::list<lti::dmatrix>& allData,
                     std::list<lti::dmatrix>& slices,
                     const int numSlices,
                     const bool complete=false) {
  lti::dmatrix allPareto;
  if (computePareto(allData,allPareto)) {
    if (complete) {
      slices.clear();
      std::list<lti::dmatrix> sl;
      computePareto3D(allPareto,slices,0,numSlices);
      computePareto3D(allPareto,sl,1,numSlices);
      slices.splice(slices.end(),sl);
      computePareto3D(allPareto,sl,2,numSlices);
      slices.splice(slices.end(),sl);
    } else {
      slices.clear();
      std::list<lti::dmatrix> sl;
      computePareto3D(allPareto,slices,2,numSlices);

      computePareto3D(allPareto,sl,0,2);
      slices.splice(slices.end(),sl);
      computePareto3D(allPareto,sl,1,2);
      slices.splice(slices.end(),sl);
    }
    return true;
  }

  return false;
}

/**
 * Usage
 */
void usage(const std::string& str) {
  std::cout << "\nUsage: \n"
            << "  " << str
            << " [-p gnuplot.gp [-f -e -x -n]] [-r] [-3 [-# nn]] file.log[.pf] "
            << "[file.log[.pf]]*\n";
  std::cout << std::endl;
  std::cout << "  -p gnuplot.gp\tOutput file for GnuPlot\n";
  std::cout << "  -f \t\tGnuPlot output .fig file\n";
  std::cout << "  -e \t\tGnuPlot output .eps file (default)\n";
  std::cout << "  -x \t\tGnuPlot output to X11 window\n";
  std::cout << "  -n \t\tSuppress labels in GnuPlot output.\n";
  std::cout << "  -a \t\tCompute the Pareto Front for all data.\n";
  std::cout << "  -r data-file \tConstruct the pareto front for the data.\n";
  std::cout << "  -3 \t\tBuild a 3D pareto front. (only with -p)\n";
  std::cout << "  -#n\t\tNumber of slices in 3D Fronts.\n";
  std::cout << "  -2w\t\tFrom an n dimensional front, extract 2D.\n";
  std::cout << "     \t\tw is a bit-wise flag indicating which dimensions\n";
  std::cout << "     \t\tto take (e.g. w=3 1st&2nd, w=5 2nd&3rd, etc.)\n";
  std::cout << "  -s \t\tForce steps\n";
  std::cout << "For every input file, an file ending with .data is created.";
  std::cout << std::endl;
}

/**
 * Fatal error
 */
void error(const char *name, const char *msg){
  std::cerr << name << ": error: " << msg << std::endl;
  usage(name);
  exit(EXIT_FAILURE);
}

int findNewIndividualsList(const std::string& logFile, std::vector<int>&  newIndList ) {
   static const std::string pattern(";; Iteration: ");
   static const std::string pattern2("New individuals: ");

   std::ifstream in(logFile.c_str()); // open the log for reading
   std::string line,subline,sublineNEWIndv;
   std::string::size_type pos;
   int last=0;
   int count=0;
   if (in) { // if it exists
     while(std::getline(in,line)) { // while file ok
       if ((pos=line.find(pattern) != std::string::npos)) { // if log comment

         ++count; // one additional iteration found
         subline=line.substr(pos+pattern.length()-1);
        // sublineNEWIndv=line.substr(pos+patternNewInd.length()-1);
         //std::cout<<"line!" << line<< "\n";
         //std::cout <<"subline1!" <<subline <<"\n";

         std::istringstream strstm(subline);
         int tmp;

         strstm >> tmp; // get the iteration from the log
         //std::cout<<"inLine iteration!"<<  tmp <<"\n";
         if (tmp > last) {
           last = tmp;
         }
         pos=line.find(pattern2);
         subline=line.substr(pos+pattern2.length()-1);
         //std::cout <<"subline2!" <<subline <<"\n";
         std::istringstream strstm2(subline);

         strstm2 >> tmp; // get the iteration from the log
         std::cout<<"New individuals: "<<  tmp <<"\n";
         newIndList.push_back(tmp);







       }
     }
   }
   //return max(last-1,count-2); // last is 0-based, count isn't
   return 0;
                               // very last iteration incomplete, so -1
 }


float calculateAreaVector(std::vector<lti::dvector>& pVector){
  float allArea=0.0;
  float pastX=0.0;
  float x;
  float y;
  unsigned int i;
  for (i=0; i<pVector.size();i++){
    x= pVector[i].at(0)-pastX;
    y= pVector[i].at(1);
    //std::cout << "X"<<x <<"\n";
    //std::cout << "Y"<<x <<"\n";
    allArea+=(float)x*y;
    //std::cout << "allArea"<<allArea <<"\n";
    pastX=pVector[i].at(0);

  }
  return allArea;
}


void calculateAreaLogPESA(  std::vector<lti::dvector>& individualsPESALog ,
  std::vector<int>& newIndividualsList, int LenPareto , std::vector<lti::dvector>& front,
  std::vector<float>& areaResult,float areaIdeal  ){
    std::cout <<"calculating Area Log" << "\n";
    std::cout <<"Individuals PESA Size" <<individualsPESALog.size() <<"\n";
    std::cout <<"New Individuals PESA Size" <<newIndividualsList.size() <<"\n";
    //std::cout <<"Front PESA Size" <<front.size() <<"\n";
    //std::cout <<"First elements log" <<individualsPESALog[0] <<"\n";

    std::vector < lti::dvector  > nonDominatedVec;
    int actualIndex=0;
    int iteration=0;
    unsigned int k;
    unsigned int i;
    int j;
    for ( i = 0; i < newIndividualsList.size(); i++) {
      std::cout<< "new Individuals List[i]: " <<  newIndividualsList[i] <<"\n";

      for (j = 0; j < newIndividualsList[i]; j++) {
        //std::cout << "adding elements!" << "\n";
        getNonDominatedVector(front,nonDominatedVec);
        removeIndividuals(front,nonDominatedVec);

        front.push_back (  individualsPESALog[actualIndex]   );
        actualIndex++;
      }
      if(iteration==0){
        std::cout<< "after iteration: " <<  iteration <<"\n";
        std::cout<< "size Front" <<front.size()<<"\n";
        for ( k = 0; k < front.size(); k++) {
          std::cout <<"front element" << front[k] << "\n";
        }
      }
      else  {
      //  for ( k = 0; k < front.size(); k++) {
      //    std::cout <<"front element" << front[k] << "\n";
      //  }
        //front.insert(front.end(), nonDominatedVec.begin(),  nonDominatedVec.end()    );

        if(front.size()>LenPareto){
          unsigned k;
          std::cout<<"excess size!";
          //std::cout<< "size Front Before" <<front.size()<<"\n";
          unsigned tmpSize=front.size();
          std::nth_element(front.begin(),front.begin()+LenPareto,front.end());
          for (k=LenPareto;k<tmpSize;++k) {
            front.erase( front.begin()+  k  );
          }
        }

        std::cout<< "after iteration: " <<   iteration <<"\n";

        std::cout<< "size Front" <<front.size()<<"\n";
        std::cout<< "size nonDominated" <<nonDominatedVec.size()<<"\n";
      //  for ( k = 0; k < front.size(); k++) {
      //    std::cout <<"front element" << front[k] << "\n";
      //  }
        std::sort(front.begin(),front.end(),compareVectors);
        std::cout<<"after sort!" <<"\n";
        for ( k = 0; k < front.size(); k++) {
          std::cout <<"front element" << front[k] << "\n";
        }
        float result= std::abs(areaIdeal-calculateAreaVector(front));
        areaResult.push_back(result  );

        //for ( k = 0; k < front.size(); k++) {
        //  std::cout <<"front element" << front[k] << "\n";
        //}
      }





      //front.clear();
      iteration++;

    }

  }

  void calculateAreaLogNSGA(  std::vector<lti::dvector>& individualsPESALog ,
    std::vector<int>& newIndividualsList, int LenPareto , std::vector<lti::dvector>& front,
      std::vector<float>& areaResult,float areaIdeal  ){
      std::cout <<"calculating Area Log" << "\n";
      std::cout <<"Individuals NSGA Size" <<individualsPESALog.size() <<"\n";
      std::cout <<"New Individuals PESA Size" <<newIndividualsList.size() <<"\n";
      //std::cout <<"Front PESA Size" <<front.size() <<"\n";
      //std::cout <<"First elements log" <<individualsPESALog[0] <<"\n";

      int actualIndex=0;
      int iteration=0;
      unsigned int k;
      unsigned int i;
      int j;
      for ( i = 0; i < newIndividualsList.size(); i++) {
        std::cout<< "new Individuals List[i]: " <<  newIndividualsList[i] <<"\n";

        for (j = 0; j < newIndividualsList[i]; j++) {
          //std::cout << "adding elements!" << "\n";
          front.push_back (  individualsPESALog[actualIndex]   );
          actualIndex++;
        }

        //  for ( k = 0; k < front.size(); k++) {
        //    std::cout <<"front element" << front[k] << "\n";
        //  }
          //front.insert(front.end(), nonDominatedVec.begin(),  nonDominatedVec.end()    );


          std::cout<< "after iteration: " <<   iteration <<"\n";

          std::cout<< "size Front" <<front.size()<<"\n";
          for ( k = 0; k < front.size(); k++) {
            std::cout <<"front element" << front[k] << "\n";
          }
          std::sort(front.begin(),front.end(),compareVectors);
          std::cout<<"after sort!" <<"\n";
          for ( k = 0; k < front.size(); k++) {
            std::cout <<"front element" << front[k] << "\n";
          }
          float result= std::abs(areaIdeal-calculateAreaVector(front));
          areaResult.push_back(result  );
          front.clear();

          //for ( k = 0; k < front.size(); k++) {
          //  std::cout <<"front element" << front[k] << "\n";
          //}








        //front.clear();
        iteration++;

      }

    }





/**
 * the main function
 */
int main(int argc, char *argv[]) {
  std::ofstream pesafile;
  pesafile.open ("pesagraph.txt");

  std::ofstream nsgafile;
  nsgafile.open ("nsgagraph.txt");

  std::vector<lti::dvector> frontPESA;
  std::vector<lti::dvector> frontNSGA;
  std::vector<float> areaResultPESA;
  std::vector<float> areaResultNSGA;

  std::vector<lti::dvector> individualsPESABest;
  std::vector<lti::dvector> individualsPESALog;
  std::vector<int> newIndividualsListPESA;


  std::vector<lti::dvector> individualsNSGABest;
  std::vector<lti::dvector> individualsNSGALog;
  std::vector<int> newIndividualsListNSGA;



  std::string fileLogPESABest="paretoPESABest.pf";
  std::string fileLogNSGABest="paretoNSGABest.pf";

  std::string fileLogPESA="paretoPESA.log";
  std::string fileLogNSGA="paretoNSGA2.log";


  getParetoData(fileLogPESABest,individualsPESABest);

  getParetoData(fileLogNSGABest,individualsNSGABest);


  std::sort(individualsNSGABest.begin(),individualsNSGABest.end(),compareVectors);

  std::sort(individualsPESABest.begin(),individualsPESABest.end(),compareVectors);

  float areaNSGA=calculateAreaVector(individualsNSGABest);

  float areaPESA=calculateAreaVector(individualsPESABest);

  std::cout <<"Area NSGA" << areaNSGA<<"\n";

  std::cout <<"Area PESA" << areaPESA<<"\n";

  std::cout <<"Len PESA BEST!" <<individualsPESABest.size() <<"\n";

  getLogData(fileLogPESA,individualsPESALog);

  getLogData(fileLogNSGA,individualsNSGALog);





  findNewIndividualsList( fileLogPESA,newIndividualsListPESA );

  findNewIndividualsList( fileLogNSGA,newIndividualsListNSGA );


  calculateAreaLogPESA(individualsPESALog,newIndividualsListPESA,
    individualsPESABest.size(),frontPESA,areaResultPESA,areaPESA
  );

  calculateAreaLogNSGA(individualsNSGALog,newIndividualsListNSGA,
    individualsNSGABest.size(),frontNSGA,areaResultNSGA,areaNSGA
  );




  unsigned int i;
  for (i = 0; i < areaResultPESA.size(); i++) {
    pesafile <<i+1 << " " << areaResultPESA[i] << "\n";
    std::cout<<"printing element![" <<i<<"] "<< areaResultPESA[i] <<"\n";

  }
  pesafile.close();

  for (i = 0; i < areaResultNSGA.size(); i++) {
    nsgafile <<i+1 << " " << areaResultNSGA[i] << "\n";
    std::cout<<"printing element![" <<i<<"] "<< areaResultNSGA[i] <<"\n";

  }


  //unsigned int i;

















  /*// Options
  int opt,first;
  std::string plot = "";
  bool gnuPlotOutput = false;
  bool labels = true;
  bool all = false;
  bool threeD = false;
  bool paretoOfRaw=false;
  int numSlices = 25;
  bool complete = false;
  bool proj2 = false; // 2D projection
  int dims2D = 3; // which dimensions to take
  bool forceSteps = false;

  enum eOutputType {
    Xfig,
    Eps,
    XWin
  };

  eOutputType outType = Eps;

  while((opt = getopt(argc,argv,"p:r:#:2:3sfanexh?")) != -1){
    switch(opt){
      case 'a':
        all = true;
        break;
      case 'p':
        plot = optarg;
        gnuPlotOutput = true;
        break;
      case 'r':
        plot = optarg;
        paretoOfRaw=true;
        break;
      case 'f':
        outType = Xfig;
        break;
      case '#':
        numSlices = lti::max(2,atoi(optarg));
        break;
      case '2':
	proj2=true;
	dims2D = atoi(optarg);
	break;
      case '3':
        threeD = true;
        break;
      case 's':
	forceSteps = true;
	break;
      case 'e':
        outType = Eps;
        break;
      case 'x':
        outType = XWin;
        break;
      case 'n':
        labels = false;
        break;
      case 'h':
      case '?':
        usage(argv[0]);
        exit(EXIT_SUCCESS);
      default:
        usage(argv[0]);
        exit(EXIT_FAILURE);
    }
  }

  // for each given file
  lti::dmatrix data;
  std::list<lti::dmatrix> allData,slices;
  std::string str;
  eTypeOfFile type = Raw;

  // get the pareto front for a raw data set
  if (paretoOfRaw) {
    allData.resize(1); // only one matrix required
    if (getData(plot,allData.front()) &&
        computePareto(allData,data) &&
        sortPareto(data) &&
        writeDataFile(std::cout,data)) {
      exit(EXIT_SUCCESS);
    }
    exit(EXIT_FAILURE);
  }

  // at least one file is required
  if (optind == argc){
    error(argv[0],"No inputs specified");
  }

  // GnuPlot output desired?
  std::ofstream out;
  if (gnuPlotOutput) {
    // header of the GnuPlot file
    out.open(plot.c_str());
    if (out) {
      out << "# Automatically created by pareto\n\n";
      out << "set size ratio 1\n";
      if (outType == Xfig) {
        out << "set terminal fig color metric fontsize 12 size 9 9 depth 50\n"
            << "set output \"" << plot + ".fig\"\n";
      } else if (outType == XWin) {
        out << "set terminal x11 reset\n";
      } else {
        out << "set terminal postscript eps color \"Helvetica\" 18\n"
            << "set output \"" << plot + ".eps\"\n";
      }
      out << "set xlabel \"Fitness 1\"\n"
          << "set ylabel \"Fitness 2\"\n";

      if (threeD) {
        out << "set zlabel \"Fitness 3\"\n\n";
        out << "set border 895 lw 0\n";
        out << "set xtics nomirror\n";
        out << "set ytics nomirror\n";
        out << "set view 60,120\n";
        out << "set ticslevel 0\n\n";
        out << "splot ";
      } else {
        out << "\nplot";
      }

    } else {
      std::cerr << "Error: GnuPlot file " << plot << " could not be created."
                << std::endl;
      gnuPlotOutput = false;
    }
  }

  // Normal operation
  // Get the data of all given files and generate the pareto gnuplot stuff

  first = 1;

  while(optind < argc){
    str = argv[optind];
    if ((type = getData(str,data)) != Unknown) {
      std::cout << data.rows() << " "
                << data.columns() << "D points read from "
                << argv[optind] << std::endl;

      if (proj2 && (data.columns()>2)) { // the user desires a 2D projection
	lti::ivector cols(2,0);
	int bits = dims2D;
	int t(0),i(0);
	while((bits != 0) && (i<2)) { // just get 2 columns
	  if ((bits % 2) == 1) { // is the current bit 1?
	    cols.at(i)=lti::min(t,data.lastColumn()); // yes: mark the
                                                   // corresponding column
	    ++i; // go to next column
	  }
	  t++; // column counter
	  bits/=2; // get next bit
	}

	std::cout << "Only dimensions " << cols.at(0) << " and "
		  << cols.at(1) << " considered." << std::endl;

	std::list<lti::dmatrix> tmpData;
	tmpData.resize(1);
	tmpData.front().copy(data,cols,false); // get the desired columns
	computePareto(tmpData,data);
      }

      if (type != Raw) {
        if (data.columns() >= 3) {
          threeD=true;
          if (!all) {
            std::list<lti::dmatrix> tmp;
            tmp.resize(1);
            tmp.back().useExternData(data.rows(),data.columns(),&data.at(0,0));
            computePareto3D(tmp,slices,numSlices,complete);
            writeDataSlices(str+".data3D",slices);
            writeDataFile(str+".data",data);
          }
        } else {
          sortPareto(data);
          writeDataFile(str+".data",data);
        }
      }

      // GnuPlot
      if (gnuPlotOutput) {

        if (data.columns() >= 3) {
          if (!all) {
            if (first>1) {
              out << ",";
            }
            // filename
            out << " \\\n  \"" << str;
            if (type != Raw) {
              out << ".data3D\" ";
            } else {
              out << "\" ";
            }

            if (labels) {
              out << " title \"" << str << "\" ";
            } else {
              out << " notitle ";
            }

            // with
            out << " with steps";
          }
        } else {
          if (first > 1) {
            out << ",";
          }
          out << " \\\n  \"" << str;
          if (type != Raw) {
            out << ".data\" ";
          } else {
            out << "\" ";
          }

          if (labels) {
            out << "title \"" << str << "\" ";
          } else {
            out << "notitle ";
          }
          out << "with ";
          if ((type == Log) && (!forceSteps)) {
            out << "points";
          } else {
            out << "steps";
          }
        }

        // if a whole pareto is desired:
        if (all) {
          allData.push_back(lti::dmatrix()); // push empty matrix
          data.detach(allData.back());
        }
      }

    }
    ++first;
    ++optind;
  }

  // the final complete pareto
  if (gnuPlotOutput && all) {
    if (threeD) {
      if (computePareto3D(allData,slices,numSlices,complete)) {
        writeDataSlices(str+".data3D",slices);
        // filename
        out << " \\\n  \"" << str;
        if (type != Raw) {
          out << ".data3D\" ";
        } else {
          out << "\" ";
        }

        if (labels) {
          out << " title \"all\" ";
        } else {
          out << " notitle ";
        }

        // with
        out << "with lines";
      }
    } else {
      if (computePareto(allData,data)) {
        sortPareto(data);
        writeDataFile(plot+".data",data);
        // and the last line in the gnu plot file:
        out << ", \\\n  \"" << plot;
        if (type != Raw) {
          out << ".data\" ";
        } else {
          out << "\" ";
        }
        if (labels) {
          out << "title \"all\" ";
        } else {
          out << "notitle ";
        }
        out << "with steps";
      }
    }
  }

  if (gnuPlotOutput) {
    out << std::endl;
    out.close();
    std::cout << "GnuPlot file written in " << plot << std::endl;
  }*/


}
