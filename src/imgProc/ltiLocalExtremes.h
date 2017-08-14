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
 * \file   ltiLocalExtremes.h
 *         Contains the functor localExtremes.
 * \author Pablo Alvarado
 * \author Peter Doerfler
 * \date   01.11.2002
 *
 * $Id: ltiLocalExtremes.h,v 1.8 2010-04-18 04:44:56 alvarado Exp $
 */

#ifndef _LTI_LOCAL_EXTREMES_H_
#define _LTI_LOCAL_EXTREMES_H_

#include "ltiModifier.h"
#include "ltiMatrix.h"
#include "ltiPointList.h"
#include "ltiList.h"

namespace lti {
  /**
   * This functor finds local extremes in a matrix of type \a T. The
   * type of extremes can be specified via the parameters (see
   * eExtremesType). Three steps in total can be performed. 
   *
   * The first step is mandatory and detects whether an element is a
   * local extremum within a search window. It can be influenced by
   * the shape of the kernel window (Square or Circle) and by the
   * kernelSize.
   *
   * The next step is a simple relative thresholding of the
   * extremes. To be independet of value ranges the min and max values
   * of the given matrix are determined. The parameter
   * parameters::relativeThreshold determines which percentage of the
   * range is allowed to be an extremum. E.g. if the range of the
   * matrix is -10 to 10 and relativeThreshold is 0.8 values above -6
   * can be local maxima and values below 6 can be local minima.
   *
   * The third step, the hystheresis, does not depend on the relative
   * thresholding. Here, each extemum is the starting point of a local
   * search for a 'better' extremum. The search is continued along
   * pixels that obey a certain threshold. This threshold
   * (parameters::hystheresisThreshold) is determined depending on the
   * range of the matrix and the value of the current extremum. Pixels
   * that are by threshold times matrix range worse than the current
   * extremum are included in the search. E.g. for a local maximum of
   * 8, and a threshold of 0.1 in the previous example the search is
   * continued while pixels are greater than 8-0.1*20=6.
   *
   * The hystheresis is quite powerful in purging extremes that are
   * very shallow with respect to the threshold. However, it is quite
   * time consuming.
   *
   * The functor returns one or two pointLists of extremes. These are
   * sorted in descending order with respect to the 'extremness' of
   * the values. Convenience applies exist that return matrices
   * containing the values of the extremes and
   * parameters::nonExtremesValue for all other elements.
   *
   * The apply() template member functions are explicitly instantiated for the
   * types: \c double, \c float, \c int, \c ubyte. If you need to apply the
   * functor to any other type you'll have to include \c
   * ltiLocalExtremes_template.h as well. Make sure to do so only in a .cpp
   * file!
   *
   * A similar functor with more use in edge-detection applications is
   * the lti::nonMaximaSuppression, which is used for example in the Canny
   * edge detection.  
   *
   * @see parameters
   * @see lti::nonMaximaSuppression
   */
  class localExtremes : public modifier {
  public:

    /**
     * Defines the type of extremes sought. Choices are maximum
     * values, minimum values, or both.
     */
    enum eExtremesType {
      Min,    /**< find minimum values */
      Max,    /**< find maximum values */
      MinMax  /**< find minium and maximum values */
    };

    /**
     * Defines the choices for the kernel's shape that is used as a
     * search window. It can be set as parameters::kernelShape.
     */
    enum eKernelShape {
      Square,  /**< use a square as search window */
      Circle   /**< use a circle as search window */
    };

    /**
     * the parameters for the class localExtremes
     */
    class parameters : public modifier::parameters {
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
      ~parameters();

      /**
       * copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& copy(const parameters& other);

      /**
       * copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& operator=(const parameters& other);

      /*
       * class name
       */
      const std::string& name() const;

      /**
       * returns a pointer to a clone of the parameters
       */
      virtual parameters* clone() const;

      /**
       * returns a pointer to a newInstance of the parameters
       */
      virtual parameters* newInstance() const;

      /**
       * write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be
       *        written.
       * @return true if write was successful
       */
      virtual bool write(ioHandler& handler,const bool complete=true) const;

      /**
       * read the parameters from the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also read, otherwise only the data block will be read.
       * @return true if write was successful
       */
      virtual bool read(ioHandler& handler,const bool complete = true);

      // ------------------------------------------------
      // the parameters
      // ------------------------------------------------

      /**
       * This flag indicates which type of extremes is sought. Choose
       * between \a Max, \a Min, and \a MinMax.
       *
       * Default: Max
       */
      eExtremesType extremesType;

      /**
       * The kernelShape defines the search window used to find local
       * extremes. While Square is faster in most applications it is
       * not rotationally invariant. This can be achieved with Circle.
       *
       * Default: Square
       */
      eKernelShape kernelShape;

      /**
       * The size of the kernel used. Note that this value has to be
       * odd. For circular areas the \a kernelSize is the diameter of
       * the circle.
       *
       * Default: 3
       */
      int kernelSize;

      /**
       * Determines which percentage of the range of the matrix is
       * allowed to be an extremum. E.g. if the range of the matrix is
       * -10 to 10 and relativeThreshold is 0.8 values above -6 can be
       * local maxima and values below 6 can be local minima. 
       *
       * Values in (0., 1.) are allowed. For other values the relative
       * thresholding is not performed.
       *
       * Default value: 0.0f  => no relative thresholding
       */
      float relativeThreshold;

      /**
       * The hystheresis eliminates 'shallow' extremes. Each extemum
       * is the starting point of a local search for a 'better'
       * extremum. The search is continued along pixels that obey a
       * certain threshold. This threshold is determined depending on
       * the range of the matrix and the value of the current
       * extremum. Pixels that are by hystheresisThreshold times
       * matrix range worse than the current extremum are included in
       * the search. E.g. for a local maximum of 8 in a matrix with
       * range [-10,10], and a threshold of 0.1 the search is
       * continued while values are greater than 8-0.1*20=6.
       *
       * Values in [0, 1) are accepted while 0 means that the global
       * extreme value is sought. Values outside this range deactivate
       * the hysteresis.
       *
       * See also: strictHystheresis
       *
       * Default value: -1 (search deactivated)
       */
      float hystheresisThreshold;

      /**
       * The hystheresis can be configured to discard points with
       * equal values or to discard them. While discarding them is the
       * correct behaviour it is sometimes desireable to keep equal
       * valued points at least half the kernelSize apart.
       *
       * When true, equal values are discarded, else they are kept
       *
       * Default: true
       */
      bool strictHystheresis;

      /**
       * Maximum number of local extremes to be kept.
       *
       * If you are only interested in a fixed number of extremes, set
       * this attribute to a value larger or equal 1. If you want to
       * always obtain the maximum number of local extremes, set this value
       * to something below 1.
       * 
       * Default value: -1 (not used)
       */
      int maxNumber;
    };

    /**
     * default constructor
     */
    localExtremes();

    /**
     * Construct a functor using the given parameters
     */
    localExtremes(const parameters& par);

    /**
     * copy constructor
     * @param other the object to be copied
     */
    localExtremes(const localExtremes& other);

    /**
     * destructor
     */
    virtual ~localExtremes();

    /**
     * Extract the local extremes from src and leave their position in the
     * given \a extremes point-list.
     *
     * @param src matrix<T> with the source data.
     * @param extremes list of points where the local extremes are found.
     *                It is sorted in decreasing order of extremeness
     * @return true if apply successful or false otherwise.
     */
    template<typename T>
    bool apply(const matrix<T>& src,ipointList& extremes) const;

    /**
     * Extract the local extremes from src and leave their position in
     * the given \a extremes point-list. This method ignores the value
     * of parameters::extremesType and pretends it is MinMax.
     *
     * @param src matrix<T> with the source data.
     * @param mins local minima, sorted with ascending values
     * @param maxs local maxima, sorted with descending values
     * @return true if apply successful or false otherwise.
     */
    template<typename T>
    bool apply(const matrix<T>& src, ipointList& mins, ipointList& maxs) const;

    /**
     * Extract the local extremes from src and leave their position in the
     * given \a extremes index-list.
     *
     * @param src vector<T> with the source data.
     * @param extremes list of points where the local extremes are found.
     *                 It is sorted in decreasing order of extremeness
     * @return true if apply successful or false otherwise.
     */
    template<typename T>
    bool apply(const vector<T>& src,lti::list<int>& extremes) const;

    /**
     * Extract the local extremes from src and leave their position in
     * the given \a extremes index-list. This method ignores the value
     * of parameters::extremesType and pretends it is MinMax.
     *
     * @param src vector<T> with the source data.
     * @param mins local minima, sorted with ascending values
     * @param maxs local maxima, sorted with descending values
     * @return true if apply successful or false otherwise.
     */
    template<typename T>
    bool apply(const vector<T>& src,
               lti::list<int>& mins, 
               lti::list<int>& maxs) const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    localExtremes& copy(const localExtremes& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    localExtremes& operator=(const localExtremes& other);

    /*
     * returns the class name
     */
    const std::string& name() const;

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual localExtremes* clone() const;

    /**
     * returns a pointer to a newInstance of this functor.
     */
    virtual localExtremes* newInstance() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

  protected:
    /**
     * @name Functions for the matrix functionality
     */
    //@{

    /**
     * Coordinates the calling of filter(), threshold(), and
     * hystheresis(), depending on \a extremesType. For Min and Max \a
     * pts holds the result. For MinMax \a pts holds the mins and \a
     * pts2 the maxes.
     */
    template<typename T>
    bool worker(const eExtremesType extremesType,
                const matrix<T>& src,
                ipointList& pts,
                ipointList& pts2) const;

    /**
     * This function performs the actual search for local extremes in
     * the matrix \a src. Depending on the \a extremesType Min, Max,
     * or MinMax values are extracted. For the first two cases the
     * points are left in \a pts and \a pts2 is empty. In the last
     * case mins are in \a pts and maxes are in \a pts2.
     *
     * The \a Function determines if the current point is an
     * extremum. For Min and Max these are less and greater, while it
     * is something more difficult for MinMax.
     *
     * The Collector is used for collecting the absolute minimum and
     * maximum of the matrix. If these are not needed (no thresholding
     * or hystheresis) it is empty. Otherwise it stores the values.
     */
    template <typename T, class Function, class Collector, class Shape>
    bool filter(const matrix<T>& src, ipointList& pts,
                Function func, Collector& coll, Shape shape) const;

    /**
     * Performs the thresholding on extremal values \a pts. For MinMax
     * this is done separately on the two sets. The Comparator depends
     * on the type of extremes: less or greater.
     *
     * The values \a minn and \a maxx must be determined during filter().
     *
     * The type Data is whether a matrix<T> or a vector<T>
     * The type Idxs is a lti::list of the index type of Data (int or ipoint)
     */
    template <typename T, class Comparator,class Data,class Idxs>
    bool threshold(const Data& src,
                   Idxs& pts,
                   T& minn, T& maxx,
                   Comparator comp) const;

    /**
     * Suppression with hystheresis. Separately for mins and maxes.
     * Keeps points with equal values.
     *
     * The hystheresis needs a sorted list of points, the most
     * extremal points first. This is achieved with
     * CompareSort. Compare is used to compare two values directly
     * (less or greater). The \a minn and \a maxx values stem from
     * filter(). They are used to find the threshold for the
     * hystheresis search.
     */
    template <typename T, class Compare, class CompareSort>
    bool hystheresis(const matrix<T>& src,
                     ipointList& maxList,
                     const T minn, const T maxx,
                     Compare comp,
                     CompareSort compS) const;

    /**
     * Suppression with hystheresis. Separately for mins and maxes.
     * Discards points with equal values.
     *
     * The hystheresis needs a sorted list of points, the most
     * extremal points first. This is achieved with
     * CompareSort. Compare is used to compare two values directly
     * (less or greater). The \a minn and \a maxx values stem from
     * filter(). They are used to find the threshold for the
     * hystheresis search.
     */
    template <typename T, class Compare, class CompareSort>
    bool hystheresisStrict(const matrix<T>& src,
                           ipointList& maxList,
                           const T minn, const T maxx,
                           Compare comp,
                           CompareSort compS) const;

    /** 
     * Discards any points exceeding the \a maxNum from the sorted \a
     * pts.
     */
    void clipPoints(ipointList& pts, const int maxNum) const;

    //@}

    /**
     * @name Functions for the vector functionality
     */
    //@{

    /**
     * Coordinates the calling of filter(), threshold(), and
     * hystheresis(), depending on \a extremesType. For Min and Max \a
     * pts holds the result. For MinMax \a pts holds the mins and \a
     * pts2 the maxes.
     */
    template<typename T>
    bool worker(const eExtremesType extremesType,
                const vector<T>& src,
                lti::list<int>& pts,
                lti::list<int>& pts2) const;

    /**
     * This function performs the actual search for local extremes in
     * the matrix \a src. Depending on the \a extremesType Min, Max,
     * or MinMax values are extracted. For the first two cases the
     * points are left in \a pts and \a pts2 is empty. In the last
     * case mins are in \a pts and maxes are in \a pts2.
     *
     * The \a Function determines if the current point is an
     * extremum. For Min and Max these are less and greater, while it
     * is something more difficult for MinMax.
     *
     * The Collector is used for collecting the absolute minimum and
     * maximum of the matrix. If these are not needed (no thresholding
     * or hystheresis) it is empty. Otherwise it stores the values.
     */
    template <typename T, class Function, class Collector>
    bool filter(const vector<T>& src,
                lti::list<int>& pts,
                Function func,
                Collector& coll) const;

    /**
     * Suppression with hystheresis. Separately for mins and maxes.
     * Keeps points with equal values.
     *
     * The hystheresis needs a sorted list of points, the most
     * extremal points first. This is achieved with
     * CompareSort. Compare is used to compare two values directly
     * (less or greater). The \a minn and \a maxx values stem from
     * filter(). They are used to find the threshold for the
     * hystheresis search.
     */
    template <typename T, class Compare, class CompareSort>
    bool hystheresis(const vector<T>& src,
                     lti::list<int>& maxList,
                     const T minn, const T maxx,
                     Compare comp,
                     CompareSort compS) const;

    /**
     * Suppression with hystheresis. Separately for mins and maxes.
     * Discards points with equal values.
     *
     * The hystheresis needs a sorted list of points, the most
     * extremal points first. This is achieved with
     * CompareSort. Compare is used to compare two values directly
     * (less or greater). The \a minn and \a maxx values stem from
     * filter(). They are used to find the threshold for the
     * hystheresis search.
     */
    template <typename T, class Compare, class CompareSort>
    bool hystheresisStrict(const vector<T>& src,
                           lti::list<int>& maxList,
                           const T minn, const T maxx,
                           Compare comp,
                           CompareSort compS) const;

    /** 
     * Discards any points exceeding the \a maxNum from the sorted \a
     * pts.
     */
    void clipPoints(lti::list<int>& pts, const int maxNum) const;

    //@}

  };
}


#endif


