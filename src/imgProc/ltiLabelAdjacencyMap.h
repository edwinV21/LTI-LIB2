/*
 * Copyright (C) 1998, 1999, 2000, 2001
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
 * \file   ltiLabelAdjacencyMap.h
 * \author Pablo Alvarado
 * \date   18.11.2002
 * 
 * $Id: ltiLabelAdjacencyMap.h,v 1.8 2006-05-12 15:09:55 doerfler Exp $
 */

#ifndef _LTI_LABEL_ADJACENCY_MAP_H_
#define _LTI_LABEL_ADJACENCY_MAP_H_

#include "ltiFunctor.h"
#include "ltiImage.h"
#include <map>

namespace lti {
  
  /**
   * Visualize a label mask in a color image
   *
   * This class draws a color image using as input a labeled mask.  The
   * colors used for each label are chosen based on the adjacency, so that
   * two neighbor labels never get the same color.
   *
   * You can choose the kind of neighborhood used (4 or 8 neighborhood) and
   * the number of colors you want to use.
   *
   * Other classes, like the viewers, may require partial computations, like
   * the adjacency graph, which is a kind of 
   *
   * @ingroup gVisualization
   */
  class labelAdjacencyMap : public functor {
  
  public:

    /**
     * Default color palette
     *
     * {lti::Black,  lti::Red,  lti::Green,   lti::Blue,
     *  lti::Yellow, lti::Cyan, lti::Magenta, lti::DarkOrange,
     *  lti::Lemon,  lti::Violet}
     */
    static const palette defaultPalette;
    
    /**
     * The parameters for the class labelAdjacencyMap
     */
    class parameters : public functor::parameters {
  
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
       * returns name of this type
       */
      virtual const std::string& name() const;

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

      /**
       * returns a pointer to a clone of the parameters
       */
      virtual parameters* clone() const;

      /**
       * returns a pointer to a new instance of the parameters
       */
      virtual parameters* newInstance() const;

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

      // ------------------------------------------------
      // the parameters
      // ------------------------------------------------

      /**
       * If true, the mininum number of colors will be used, which will depend
       * on the neighborhood used.  (a max of 4 colors is required for a
       * 4 neighborhood, and a max of 8 color for a 8 neighborhood).
       *
       * If false, all colors in the palette might be used.
       *
       * Default: false
       */
      bool minColors;

      /**
       * The colors used to denote the labels.  Note that the assigment is not
       * 1 to 1, but will be done depending on the adjacency of the labels.
       *
       * Default: {lti::Black,lti::Red,lti::Green,lti::Blue,lti::Yellow,
       *           lti::Cyan,lti::Magenta,lti::DarkOrange,lti::Lemon,
       *           lti::Violet}
       *
       * This default palette can be accessed anytime as
       * lti::labelAdjacencyMap::defaultPalette
       */
      palette thePalette;

      /**
       * Neighborhood used.
       *
       * Valid values are 4 and 8.  Other values will be considered as
       * 8-neighborhood.
       *
       * Default value: 8
       */
      int neighborhood;

    };


    /**
     * This auxiliary class stores a simple adjacency graph between labels, in
     * which only the edges between the nodes contain data.
     *
     * It uses std::map to store in a memory friendly way a sparse
     * "adjacency" matrix.  
     *
     * In principle, only the "neighbors_" attribute of the class could also be
     * used as "graph" representation, but this class allows for some common
     * tasks with that container structure.
     */
    class graph {
      friend class labelAdjacencyMap;
    public:
      /**
       * Construct an empty graph
       */
      graph();

      /**
       * Increment the value stored in the edge between the nodes identified by
       * row and col.  If you visualize the graph as a matrix, this increments
       * the element at (row,col).
       */
      void acc(const int row,const int col);

      /**
       * Remove all data of graph
       */
      void clear();

      /**
       * Find the largest row id used
       */
      int findMaxId() const;

      /**
       * Find the largest row id used
       */
      int findMinId() const;

      /**
       * Find the largest row id used
       */
      void findMinMaxIds(int& minRow,int& maxRow) const;

      /**
       * Each node in nodes_ has a "list" of ids of the neighbors of that node.
       * There exist therefore an edge for each element in a list of this type.
       */
      typedef std::map<int,int> edges_type;

      /**
       * Double hash for the neighbors
       */
      typedef std::map<int, edges_type > nodes_type;

    private:
      /**
       * The neighbors is a sort of list of lists of neighbors for each 
       * label.
       *
       * The key of the hash is a label/region id, and the data is a 
       * hash of the neighbors, also keyed by the region/label id and
       * as data the graph usually will contain the number of elements
       * in common, i.e. the number of times acc() was called.
       */
      nodes_type nodes_;
    };


    /**
     * default constructor
     */
    labelAdjacencyMap();

    /**
     * Construct a functor using the given parameters
     */
    labelAdjacencyMap(const parameters& par);

    /**
     * copy constructor
     * @param other the object to be copied
     */
    labelAdjacencyMap(const labelAdjacencyMap& other);

    /**
     * destructor
     */
    virtual ~labelAdjacencyMap();

    /**
     * returns the name of this type ("labelAdjacencyMap")
     */
    virtual const std::string& name() const;

    /**
     * Compute the adjacency map of the given 8-bit labeled mask.
     *
     * @param src channel8 with the source data.
     * @param dest image where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const matrix<ubyte>& src,image& dest) const;

    /**
     * Compute the adjacency map of the given 32-bit labeled mask.
     *
     * The input data cannot have label ids less than zero and
     * internally a look-up table of the size equal to the maximum
     * value in the input data plus one will be created.  So, it will
     * be assumed that the input mask has all its labels with low positive ids.
     *
     * @param src matrix<int> with the source data.
     * @param dest image where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const matrix<int>& src,image& dest) const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    labelAdjacencyMap& copy(const labelAdjacencyMap& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    labelAdjacencyMap& operator=(const labelAdjacencyMap& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual labelAdjacencyMap* clone() const;
    
    /**
     * returns a pointer to a new instance of this functor.
     */
    virtual labelAdjacencyMap* newInstance() const;
    
    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * Compute the adjacency graph of labels for the given labeled mask.
     */
    bool adjacency(const matrix<ubyte>& mask,graph& dest) const;

    /**
     * Compute the adjacency graph of labels for the given labeled mask.
     */
    bool adjacency(const matrix<int>& mask,graph& dest) const;

    /**
     * Compute a transformation Look-up Table that maps the id of a label to
     * the corresponding entry in the parameters palette.
     */
    bool computePalette(const graph& adj,palette& pal) const;


    /**
     * Compute a transformation Look-up Table that maps the id of a label to
     * the corresponding entry in the parameters palette.
     */
    bool computePalette(const graph& adj,ivector& pal) const;

  protected:
    
    /**
     * Compute a "palette" from the graph, which can be accessed with the
     * id of a label to get the corresponding color in the map.
     *
     * The minimum number of colors will be used.
     */
    bool computeMinPalette(const graph& adj,palette& pal) const;


    /**
     * Compute a transformation Look-up Table that maps the id of a label to
     * the corresponding entry in the parameters palette.
     *
     * The minimum number of colors will be used.
     */
    bool computeMinPalette(const graph& adj,ivector& pal) const;

    /**
     * Compute a "palette" from the graph, which can be accessed with the
     * id of a label to get the corresponding color in the map.
     *
     * The maximum number of colors will be used.
     */
    bool computeMaxPalette(const graph& adj,palette& pal) const;

    /**
     * Compute a transformation Look-up Table that maps the id of a label to
     * the corresponding entry in the parameters palette.
     *
     * The maximum number of colors will be used.
     */
    bool computeMaxPalette(const graph& adj,ivector& pal) const;

  };
}

#endif

