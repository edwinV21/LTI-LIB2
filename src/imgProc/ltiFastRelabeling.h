/*
 * Copyright (C) 2003, 2004, 2005, 2006
 * Lehrstuhl fuer Technische Informatik, RWTH-Aachen, Germany
 * Copyright (C) 2007
 * Pablo Alvarado, ITCR
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
 * \file   ltiFastRelabeling.h
 *         Contains the class lti::fastRelabeling used to detect connected
 *         regions in a labeled mask.
 * \author Pablo Alvarado
 * \date   16.5.2003
 *
 * revisions ..: $Id: ltiFastRelabeling.h,v 1.5 2013-08-29 08:33:47 alvarado Exp $
 */

#ifndef _LTI_FAST_RELABELING_H_
#define _LTI_FAST_RELABELING_H_

#include "ltiAreaPoints.h"
#include "ltiMatrix.h"
#include "ltiFunctor.h"
#include <vector>

namespace lti {
  /**
   * Fast Relabeling.
   *
   * This corresponds to a connected-component labeling or region
   * labeling functor.
   *
   * This functor takes a mask (labeled or not), where it is assumed
   * that a region must be connected.  Then the mask is evaluated to
   * reassign labels to the regions in a way that each connected
   * region gets its own label.
   *
   * Derived classes can compute for each found object additional
   * information like geometric features, bounding boxes, weighted
   * probabilities, etc.
   *
   * For the simplest relabeling, the algorithms just needs to
   * traverse the input matrix twice, which makes it fast compared
   * with other more sophisticated algorithms.
   *
   * If you select to sort the labels according to their sizes or to suppress
   * small objects, then the algorithm used will traverse the input matrix
   * three times, which is still fast, but in any case slower than just
   * relabeling.
   *
   * @see lti::fastRelabeling::parameters
   *
   * @ingroup gRegionAnalysis
   */
  class fastRelabeling : public functor {
  public:
    /**
     * The parameters for the class fastRelabeling
     */
    class parameters : public functor::parameters {
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
      ~parameters();

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
       * Returns the complete name of the parameters class.
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
       * @name General Options
       */
      //@{

      /**
       * All values in the original mask above or equal this minimum threshold
       * will be considered as candidates to relabeling.
       *
       * Default value: 1
       */
      int minThreshold;

      /**
       * All values in the original mask below or equal this maximum threshold
       * will be considered as candidates to relabeling.
       *
       * Default value: std::numeric_limits<int>::max()
       */
      int maxThreshold;

      /**
       * If true, each integer value in the original image will be considered
       * as a new region label.  If false, only two "values" will be taken:
       * within the threshold interval or outside it.
       *
       * Default value: true
       */
      bool assumeLabeledMask;

      /**
       * If true, a 4-neighborhood will be used to detect the adjacent pixels,
       * otherwise an 8-neighborhood will be used.
       *
       * Due to the lesser number of required comparisons, the 4-neighborhood
       * allows a faster relabeling.  Sometimes however the 8-neighborhood
       * is required.
       *
       * Default value: true (i.e. 4-neighborhood)
       */
      bool fourNeighborhood;
      //@}

      /**
       * @name Filter objects
       */
      //@{
      /**
       * If sortSize is true, the object labels will be assigned according to
       * the number of pixels of each object.
       *
       * \warning There is an exception.  The label 0 has a special meaning
       * and it will always be assigned to the background (everything outside
       * the given interval between minThreshold and maxThreshold)
       * independently of its size.  This means the regions will be sorted
       * decreasingly but begining with the second element.  The first one is
       * always the background.  If you set minThreshold to zero, the first
       * element is always empty.
       *
       * Defalut value: false
       */
      bool sortSize;

      /**
       * If the number of pixels of an object is less than the
       * minimumObjectSize, it will be assigned to the background label (0).
       *
       * If this value is 1 or less, then no size filtering will be performed.
       *
       * Note that objects of exactly minimumObjectSize are still valid and 
       * therefore are not assigned to the background.
       *
       * Default value: 1 (i.e. consider all objects)
       */
      int minimumObjectSize;
      //@}
    };

    /**
     * Default constructor
     */
    fastRelabeling();

    /**
     * Construct a functor using the given parameters
     */
    fastRelabeling(const parameters& par);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    fastRelabeling(const fastRelabeling& other);

    /**
     * destructor
     */
    virtual ~fastRelabeling();

    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    fastRelabeling& copy(const fastRelabeling& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    fastRelabeling& operator=(const fastRelabeling& other);

    /**
     * Returns the complete name of the functor class
     */
    virtual const std::string& name() const;

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual fastRelabeling* clone() const;

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual fastRelabeling* newInstance() const;

    /**
     * @name Only relabel
     */
    //@{
    /**
     * Relabel the given mask and leave the new labeled mask on the same
     * object.
     *
     * @param srcdest matrix<ubyte> with the source data.  The result
     *                 will be left here too.
     * @return true if apply successful or false otherwise.
     */
    bool apply(matrix<ubyte>& srcdest) const;

    /**
     * Relabel the given mask and leave the new labeled mask on the same
     * object.
     *
     * @param srcdest imatrix with the source data.  The result
     *                 will be left here too.
     * @return true if apply successful or false otherwise.
     */
    bool apply(imatrix& srcdest) const;

    /**
     * Relabel the mask in \a src and write the result in \a dest.
     * @param src matrix<ubyte> with the source data.
     * @param dest matrix<ubyte> where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const matrix<ubyte>& src,matrix<ubyte>& dest) const;

    /**
     * Relabel the mask in \a src and write the result in \a dest.
     *
     * @param src matrix<ubyte> with the source data.
     * @param dest matrix<ubyte> where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const matrix<ubyte>& src,imatrix& dest) const;

    /**
     * Relabel the mask in \a src and write the result in \a dest.
     *
     * @param src imatrix with the source data.
     * @param dest imatrix where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const imatrix& src,imatrix& dest) const;
    //@}

    /**
     * @name Only relabel and count how many labels have been assigned
     */
    //@{
    /**
     * Relabel the given mask and leave the new labeled mask on the same
     * object.
     *
     * @param srcdest matrix<ubyte> with the source data.  The result
     *                 will be left here too.
     * @param numObjects how many labels have been found
     * @return true if apply successful or false otherwise.
     */
    bool apply(matrix<ubyte>& srcdest,int& numObjects) const;

    /**
     * Relabel the given mask and leave the new labeled mask on the same
     * object.
     *
     * @param srcdest imatrix with the source data.  The result
     *                 will be left here too.
     * @param numObjects how many labels have been found
     * @return true if apply successful or false otherwise.
     */
    bool apply(imatrix& srcdest,int& numObjects) const;

    /**
     * Relabel the mask in \a src and write the result in \a dest.
     * @param src matrix<ubyte> with the source data.
     * @param dest matrix<ubyte> where the result will be left.
     * @param numObjects how many labels have been found
     * @return true if apply successful or false otherwise.
     */
    bool apply(const matrix<ubyte>& src,
               matrix<ubyte>& dest,
               int& numObjects) const;

    /**
     * Relabel the mask in \a src and write the result in \a dest.
     *
     * @param src matrix<ubyte> with the source data.
     * @param dest matrix<ubyte> where the result will be left.
     * @param numObjects how many labels have been found
     * @return true if apply successful or false otherwise.
     */
    bool apply(const matrix<ubyte>& src,imatrix& dest,int& numObjects) const;

    /**
     * Relabel the mask in \a src and write the result in \a dest.
     *
     * @param src imatrix with the source data.
     * @param dest imatrix where the result will be left.
     * @param numObjects how many labels have been found
     * @return true if apply successful or false otherwise.
     */
    bool apply(const imatrix& src,imatrix& dest,int& numObjects) const;
    //@}

    /**
     * @name Relabel and count pixels on each new region
     */
    //@{
    /**
     * Relabel the mask in \a src and write the result in \a dest.
     * Store the number of pixels of each label in \a numPixels.
     *
     * @param src matrix<ubyte> with the source data.
     * @param dest matrix<ubyte> where the result will be left.
     * @param numPixels number of pixel per new object label
     * @return true if apply successful or false otherwise.
     */
    bool apply(const matrix<ubyte>& src,
                     matrix<ubyte>& dest,
                     ivector& numPixels) const;

    /**
     * Relabel the mask in \a src and write the result in \a dest.
     * Store the number of pixels of each label in \a numPixels.
     *
     * @param src matrix<ubyte> with the source data.
     * @param dest matrix<ubyte> where the result will be left.
     * @param numPixels number of pixel per new object label
     * @return true if apply successful or false otherwise.
     */
    bool apply(const matrix<ubyte>& src,
                     imatrix& dest,
                     ivector& numPixels) const;

    /**
     * Relabel the mask in \a src and write the result in \a dest.
     * Store the number of pixels of each label in \a numPixels.
     *
     * @param src imatrix with the source data.
     * @param dest imatrix where the result will be left.
     * @param numPixels number of pixel per new object label
     * @return true if apply successful or false otherwise.
     */
    bool apply(const imatrix& src,imatrix& dest,ivector& numPixels) const;
    //@}

    /**
     * @name Relabel, count and extract regions
     */
    //@{

    /**
     * Relabel the mask in \a src and write the result in \a dest.
     * Store the number of pixels of each label in \a numPixels.
     *
     * @param src matrix<ubyte> with the source data.
     * @param dest matrix<ubyte> where the result will be left.
     * @param numPixels number of pixel per new object label
     * @param objects for each label o, objects[o] contains all points 
     *                belonging to that object.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const matrix<ubyte>& src,
                     matrix<ubyte>& dest,
                     ivector& numPixels,
                     std::vector<areaPoints>& objects) const;

    /**
     * Relabel the mask in \a src and write the result in \a dest.
     * Store the number of pixels of each label in \a numPixels.
     *
     * @param src matrix<ubyte> with the source data.
     * @param dest matrix<ubyte> where the result will be left.
     * @param numPixels number of pixel per new object label
     * @param objects for each label o, objects[o] contains all points 
     *                belonging to that object.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const matrix<ubyte>& src,
                     imatrix& dest,
                     ivector& numPixels,
                     std::vector<areaPoints>& objects) const;

    /**
     * Relabel the mask in \a src and write the result in \a dest.
     * Store the number of pixels of each label in \a numPixels.
     *
     * @param src imatrix with the source data.
     * @param dest imatrix where the result will be left.
     * @param numPixels number of pixel per new object label
     * @param objects for each label o, objects[o] contains all points 
     *                belonging to that object.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const imatrix& src,
                     imatrix& dest,
                     ivector& numPixels,
                     std::vector<areaPoints>& objects) const;
    //@}


    /**
     * @name Partial relabeling methods.
     */
    //@{
    /**
     * This method does not complete the relabeling process.  Instead of
     * that it returns an incomplete labeled mask and the label equivalences
     * vector, with which other functors can efficiently compute other
     * information about the found objects.
     *
     * @param src original labeled or unlabeled mask.
     * @param partMask partial labeled mask.
     * @param equivLabels vector with the equivalence labels
     * @param numObjects number of objects found in the src mask.
     * @return true if successful, false otherwise.
     *
     * The simplest way to get a consistend finished mask (which is directly
     * provided in other apply methods) is to assign to each pixel of partMask
     * the corresponding equivalent label, i.e. for each x and y, 
     * partMask.at(y,x) = equivLabels(partMask.at(y,x)).
     */
    bool partial(const matrix<ubyte>& src,
                       matrix<ubyte>& partMask,
                       ivector& equivLabels,
                       int& numObjects) const;

    /**
     * This method does not complete the relabeling process.  Instead of
     * that it returns an incomplete labeled mask and the label equivalences
     * vector, with which other functors can efficiently compute other
     * information about the found objects.
     *
     * @param src original labeled or unlabeled mask.
     * @param partMask partial labeled mask.
     * @param equivLabels vector with the equivalence labels
     * @param numObjects number of objects found in the src mask.
     * @return true if successful, false otherwise.
     *
     * The simplest way to get a consistend finished mask (which is directly
     * provided in other apply methods) is to assign to each pixel of partMask
     * the corresponding equivalent label, i.e. for each x and y, 
     * partMask.at(y,x) = equivLabels(partMask.at(y,x)).
     */
    bool partial(const matrix<ubyte>& src,
                       imatrix& partMask,
                       ivector& equivLabels,
                       int& numObjects) const;

    /**
     * This method does not complete the relabeling process.  Instead of
     * that it returns an incomplete labeled mask and the label equivalences
     * vector, with which other functors can efficiently compute other
     * information about the found objects.
     *
     * @param src original labeled or unlabeled mask.
     * @param partMask partial labeled mask.
     * @param equivLabels vector with the equivalence labels
     * @param numObjects number of objects found in the src mask.
     * @return true if successful, false otherwise.
     *
     * The simplest way to get a consistend finished mask (which is directly
     * provided in other apply methods) is to assign to each pixel of partMask
     * the corresponding equivalent label, i.e. for each x and y, 
     * partMask.at(y,x) = equivLabels(partMask.at(y,x)).
     */
    bool partial(const imatrix& src,
                       imatrix& partMask,
                       ivector& equivLabels,
                       int& numObjects) const;
    //@}

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

  protected:

    /**
     * Sort labels and eliminate those objects with sizes smaller than the
     * given threshold.
     *
     * @param minSize minimum number of pixels an object must have.
     * @param nLargest if greater than zero, only the given number of objects
     *                 will be returned (besides the background).
     * @param numPixels number of pixels per object.  The index of the vector
     *                  corresponds to the label in the given mask.
     *                  After calling this vector is also sorted.
     * @param newNumPixelsSize new valid number of entries in numPixels
     * @param reindex new indices
     */
    bool sortLabels(const int minSize, 
                    const int nLargest,
                          ivector& numPixels, 
                          int& newNumPixelsSize,
                          ivector& reindex) const;

    /**
     * Compute new label equivalences considering the suppression of
     * small regions.
     *
     * @return New number of labels used.
     */
    bool suppress(const int minSize,
                        ivector& numPixels,
                        int& newNumPixelsSize,
                        ivector& reindex) const;
   

    /**
     * Just relabel the destination, but compute also the size of each object.
     */
    bool relabelWithArea(const ivector& equivLabels,
			 const int numObjects,
			 imatrix& dest,
			 ivector& numPixels) const;

    /**
     * Just relabel the destination, but compute also the size of each object.
     */
    bool relabelWithArea(const vector<ubyte>& equivLabels,
			 const int numObjects,
			 matrix<ubyte>& dest,
			 ivector& numPixels) const;
  };
}

#endif
