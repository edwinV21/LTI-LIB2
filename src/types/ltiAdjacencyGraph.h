/*
 * Copyright (C) 2003, 2004, 2005, 2006
 * Lehrstuhl für Technische Informatik
 * Copyright (C) 2008
 * Pablo Alvarado
 *
 * This file is part of the LTI-Computer Vision Library 2 (LTI-Lib-2)
 *
 * The LTI-Lib-2 is free software; you can redistribute it and/or
 * modify it under the terms of the BSD License.
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the authors nor the names of its contributors may be
 *    used to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/** 
 * \file   ltiAdjacencyGraph.h
 *         Contains the template class lti::adjacencyGraph
 *         used to manage graph data structures that represent adjacency
 *         relationships.
 * 
 * \author Pablo Alvarado
 * \date   23.10.2007
 *
 * revisions ..: $Id: ltiAdjacencyGraph.h,v 1.1 2008-04-21 01:31:21 alvarado Exp $
 */

#ifndef _LTI_ADJACENCY_GRAPH_H_
#define _LTI_ADJACENCY_GRAPH_H_

#include "ltiMacroSymbols.h"

#include "ltiContainer.h"
#include "ltiPriorityQueue.h"
#include "ltiIoHandler.h"
#include "ltiTypeInfo.h"
#include <utility> // for std::pair

namespace lti {
  // ------------------------------------------------------------------------
  // --                        Edge basic classes                          --
  // ------------------------------------------------------------------------
  /**
   * Basic class for symmetric edges.  You can specialize it for other types of
   * W than the standard ones (float,double,int,etc.)
   *
   * The template type W is the weight type, which is usually float or double,
   * but it can be a \e signed integer type too.
   */
  template <class W>
  class symmetricEdgeTraits {
  public:
    /**
     * Constant used to mark a weight as invalid, unexistent, etc.
     */
    static const W Invalid;

    /**
     * Constant used to indicate if the adjacency edges are symmetric,
     * i.e. if weight(A,B) == weight(B,A).
     */
    static const bool Symmetric;
  };

  /**
   * Basic class for asymmetric edges.  You can specialize it for other types
   * of W than the standard ones (float,double,int,etc.)
   *
   * The template type W is the weight type, which is usually float or double,
   * but it can be a \e signed integer type too.
   */
  template <class W>
  class asymmetricEdgeTraits {
  public:
    /**
     * Constant used to mark a weight as invalid, unexistent, etc.
     */
    static const W Invalid;

    /**
     * Constant used to indicate if the adjacency edges are symmetric,
     * i.e. if weight(A,B) == weight(B,A).
     */
    static const bool Symmetric;
  };
  
  /**
   * Dummy weight functor that returns the default weight type.
   *
   * The template parameters are:
   * - N node type
   * - W weight type (usually float or double)
   * - D edge data type
   */
  template<class N,class W,class D>
  class adjacencyGraphVoidWeightFunction {
  public:
    /**
     * Method to compute the edge weight from the nodes and the data of
     * the edge
     */
    inline W operator()(const N& first,const N& second,const D& data) const {
      return W();
    }
  };

  // ------------------------------------------------------------------------
  // --                        adjacencyGraph                              --
  // ------------------------------------------------------------------------  

  /**
   * Adjacency Graph.
   *
   * An adjacency graph is a set of nodes and edges linking nodes.
   * Each node can contain some data (of type N).  Each edge contains
   * also some data (of type D) and a weight (of type W).
   *
   * The adjacency property between two nodes is symmetrical, i.e. if
   * A is adjacent to B, then B is adjacent to A.  However, the
   * weights and data of the edges from A-to-B or B-to-A not
   * necessarily have to be the same.  This means that an adjacency
   * graph ensures always two edges between two nodes.  Furthermore, a
   * node can never be adjacent to itself.
   *
   * Several methods are provided to merge nodes, insert other ones, insert
   * edges to connect nodes, and update the data of existing nodes or edges.
   *
   * This class can be used for example to represent an adjacency
   * graph of all regions in an image partition.  The main data
   * structure is an affinity matrix: an usually sparse matrix
   * encoding the weight of the edges between the nodes of a graph.
   *
   * A second property of this class is that it is possible to access in
   * O(1) the edge of the graph with the smallest weight, even if you change
   * the weights of the edges during the use of the graph.
   *
   * Merging two nodes implies updating the weights of the edges in
   * the neighborhood of the new merged node, and the data object of
   * the node.  For this reason this class needs a function object to compute
   * weight_type weight_function(const N&,const N&,const D&) const
   * which computes the weight of an edge as a function of the two linked
   * nodes and the information chunk stored in the edge itself.  Of course, if
   * you want, you can avoid the mergeNodes() method and set the weight of
   * your graph by yourself.  For this last case there is also a
   * topologicalMerge() method, that won't recompute anything, not even the
   * new node's data.
   *
   * <b>Template parameters</b>
   *
   * The required template types are the following:
   * - Type \b N specifies the node information type.  Each node gets an own 
   *   identification id which is always an integer greater or equal zero.
   *   With it, you can get or set the node's data (of type N).
   *   The type name of N is node_type.
   *   Following are the requirements for this type N:
   *   - The operator=(const N&) to copy another node's data.
   *   - The operator+= to merge the contents of two nodes.
   *   - Overload of the global function lti::read(ioHandler&,N)
   *   - Overload of the global function lti::write(ioHandler&,const N&)
   * - The weight type \b W with following interface
   *   - operator= to copy other weights
   *   - operator< that allows to define an order of weight_type elements
   *   - operator== to compare equality
   *   - operator!= to compare inequality
   *   - Overload of the global function lti::read(ioHandler&,W&)
   *   - Overload of the global function lti::write(ioHandler&,const W&)
   *   This type is usually just \c float or \c double.  Its name is
   *   weight_type
   * - The edge data type \b D, which allows to describe additional 
   *   information about the edge.  This information is used together with
   *   the nodes information in the computation of the edge's weight.  It
   *   can describe for example the number of pixels in the border between
   *   two regions.
   *   This type (named edge_data_type) must support 
   *   - operator+=(const D& other) to combine the information of another edge.
   *   - Overload of the global function lti::read(ioHandler&,D&)
   *   - Overload of the global function lti::write(ioHandler&,const D&)
   * - The weight computation functor \b F is a small class with the
   *   implementation of the operator() with following interface:
   *   <code>W operator()(const N& first,const N& second,const D& data) const
   *   </code>
   *   The graph provides methods to get a const reference to the internal
   *   instance of this functor or to set the internal instance.  This allow
   *   the computation of the weights to use some additional data.
   *   If this functors must have a state, you should implement the assignment
   *   operator=(), which will be used by the method setWeightFunctor()
   *   This type has the name weight_functor
   * - Type E specifies the edge traits.  It must provide following
   *   interface:
   *   - A static constant attribute E::Invalid of type W
   *     which will represent an invalid edge.  Since the weights
   *     usually represent similarities or distances between nodes, they
   *     are usually zero or possitive.  Therefore, usual invalid flags
   *     are negative constants.
   *   - A static constant boolean attribute E::Symmetric, which is \c true
   *     if the weights are symmetric, i.e. if weight(A,B) == weight(B,A), or
   *     false otherwise.
   *   The name of this type is edge_traits
   *
   * Please note that for all standard types (int, float, double, etc.) and 
   * most all types in the \c lti namespace, the lti::read and lti::write
   * functions are already implemented.
   *
   * Since each node is identified by an unique id of type \c id_type,
   * the edges can be uniquely specified by a pair of id_type elements.
   *
   * <b>Symmetric edges</b>
   *
   * The use of a symmetric edge type (edge_traits::Symmetric == true) have
   * several implications:
   * - The computation of the weight occurs only once for the given
   *   edge (a,b) and (b,a) will get the same value.
   * - Only the lower diagonal affinity matrix will be stored, to save some
   *   space.
   *
   * <b>Iterators</b>
   *
   * You can use the iterators and edge_iterators to iterate on the
   * graph nodes or the node's outgoing edges respectively.
   * 
   * The iterators visit only valid nodes, and allow you to access 
   * the data of the node in the usual way (operator*), but you can also
   * get the id of the visited node with the method id().
   *
   * The edge_iterators allow to access the data of an edge.  Using
   * several methods of the graph class, you can also efficiently
   * access other information like the nodes linked by the pointed
   * edge, or the weight assigned to the edge.
   *
   * <b>IO</b>
   *
   * The adjacency graphs are containers, and therefore are
   * serializable.  If you want to edit the written files, you need to
   * know the syntax therein.
   * A file contains four data blocks:
   * -# The size of the data structure, which is always greater or equal 
   *    the number of nodes.
   * -# The nodes list containing for each node two elements: the
   *    node's id and the data of the node.
   * -# A boolean indicating if the saved data belongs to a symmetric graph
   *    or not.
   * -# The edge list containing for each edge three elements: the edge as
   *    pair of node ids, the edge's weight, the edge's data
   *
   * You can find another more general weighted graph representation
   * in the class lti::weightedGraph.
   *
   * @see lti::weightedGragh
   * @see lti::adjacencyGraphEdge,lti::adjacencyGraphNode
   */
  template< class N,       // the node type
            class W = float, // the weight type
            class D = int,   // the edge data type
            class F = adjacencyGraphVoidWeightFunction<N,W,D>, // functor
            class E = symmetricEdgeTraits<W> >
  class adjacencyGraph : public container {
  public:
    /**
     * @name Type definitions
     */
    //@{
    /**
     * Type of the weight of the edges
     *
     * It provides with following interface
     * - operator= to copy other weights
     * - operator<
     * - operator== to compare equality
     * - operator!= to compare inequality
     */
    typedef W weight_type;

    /**
     * Type for the identification key of the nodes.
     *
     * All nodes in a graph have consecutive indices.  When a node is removed,
     * its index can be (and will be) reused, so that you should take care not
     * to use "obsoleted" indices, because they could bring you to a new 
     * node you didn't want.
     */
    typedef int id_type;

    /**
     * The edge type contains two adjacent nodes "first" and "second".  The
     * edge direction is always from first to second.
     */
    typedef std::pair<id_type,id_type> node_pair;

    /**
     * Type of the nodes
     */
    typedef N node_type;

    /**
     * Type of the nodes
     */
    typedef N value_type;

    /**
     * Type for additional edge information (besides the weight).
     */
    typedef D edge_data_type;

    /**
     * Edge traits type containing the Symmetric and Invalid constants.
     */
    typedef E edge_traits;

    /**
     * Type of the functor implementing the weight computation functor
     */
    typedef F weight_functor;

    //@}

  protected:

    /**
     * @name Protected type definitions
     */
    //@{

    /**
     * Priority queue type used. 
     *
     * The priority queue keeps the edges sorted in increasing order by their
     * weights.
     *
     * The weight_type is used as key to sort the elements,
     * the point type is used to indicate the source and destination nodes
     * (y source, x destination),
     */
    typedef priorityQueue<weight_type, node_pair> queue_type;

    /**
     * Entry type used in the sparse matrix representing the affinity matrix
     *
     * The entry contains an attribute index to access the weight of the
     * corresponding edge in the priority queue and additional information of
     * type data_type.
     */
    class entry_type {
    public:
      // ---------------------------------------------------------------------
      // The attributes
      // ---------------------------------------------------------------------

      /**
       * Index of this entry in the priority queue.
       */
      typename queue_type::index_type index;

      /**
       * The data contains information relevant for the two regions to which
       * this entry belongs.  It can be for example the length of the common
       * border, or the distance between the colors of the boundaries.
       */
      edge_data_type data;

      /**
       * Type to point to another entry
       */
      typedef typename std::map<id_type,
                                entry_type>::iterator entry_iterator;
    
      /**
       * Iterator to the complement of this entry.
       *
       * If this entry belongs to the ids (i,j), then the complement has
       * the ids (j,i).
       * 
       * The use of this iterator saves time computing the position of the
       * complement, which is in manipulation operations required (like
       * merge or remove).
       *
       * This is possible considering that map has the important
       * property that inserting a new element into a map does not
       * invalidate iterators that point to existing elements. Erasing
       * an element from a map also does not invalidate any iterators,
       * except, of course, for iterators that actually point to the
       * element that is being erased.
       */
      entry_iterator complement;

      // ---------------------------------------------------------------------
      // The methods
      // ---------------------------------------------------------------------

      /**
       * Default constructor
       */
      entry_type() : index(),data(),complement() {}

      /**
       * Construct with given values
       */
      entry_type(const typename queue_type::index_type& idx,
                 const edge_data_type& dta = edge_data_type(),
                 const entry_iterator& it = entry_iterator()) 
        : index(idx),data(dta),complement(it) {
      }

      /**
       * Copy constructor
       */
      entry_type(const entry_type& other) 
        : index(other.index),data(other.data),complement(other.complement) {
      }

      /**
       * Change the index and return a reference to this entry
       */
      entry_type& setIndex(const typename queue_type::index_type& idx) {
        index=idx;
        return *this;
      }

      /**
       * Copy operator
       */
      entry_type& operator=(const entry_type& other) {
        index = other.index;
        data = other.data;
        complement = other.complement;

        return *this;
      }
    };

    /**
     * The row of the affinity matrix contains for one specific node,
     * all nodes to which an edge exists.
     */
    typedef std::map<id_type,entry_type> row_type;

    /**
     * Type for (sparse) adjacency matrix of complex elements
     */
    typedef std::vector< row_type > adjacency_type;

    /**
     * Type used to contain the nodes' data and a flag indicating how many
     * outgoing edges can be found in the node.  This number will be 
     * negative if the node has been deleted.
     */
    typedef std::vector< std::pair<int,node_type> > nodes_type;

    //@}

  public:

    // -----------------------------------------------------------------------
    // iterators
    // -----------------------------------------------------------------------
    class const_iterator;

    /**
     * The graph iterator iterates on all nodes.
     *
     * This class is similar to the usual iterators of the STL.
     * The iterator allows you to access the data the nodes, but
     * you can also get the node's id for other kind of processing.
     *
     * For example:
     * \code
     * adjacencyGraph::iterator it;
     * // ... do something with a graph an let the iterator point somewhere 
     * //     therein.
     * adjacencyGraphNode node = (*it); // get the pointed data.
     * adjacencyGraph::id_type = it.id(); // get the id of the pointed node.
     * \endcode
     */
    class iterator {
      friend class adjacencyGraph<N,W,D,F,E>;
      friend class adjacencyGraph<N,W,D,F,E>::const_iterator;

    public:
      /**
       * Default constructor
       */
      iterator()
        : pos(0), theVector(0) {};

      /**
       * Copy constructor
       */
      iterator(const iterator& other)
        : pos(other.pos),theVector(other.theVector) {};

      /**
       * Advance to next item
       */
      inline iterator& operator++() { // prefix
        do {
          ++pos;
        } while ((pos < static_cast<int>(theVector->size())) &&
                 ((*theVector)[pos].first < 0)) ;
        return *this;
      };  

      /**
       * Advance to next item
       */
      inline iterator operator++(int) { // postfix
        iterator tmp(*this);
        do {
          ++pos;
        } while ((pos < static_cast<int>(theVector->size())) &&
                 ((*theVector)[pos].first < 0));
        return tmp;
      }; 

      /**
       * Recede to previous item    // prefix
       */
      inline iterator& operator--() {
        do {
          --pos;
        } while ((pos >= 0) && 
                 ((*theVector)[pos].first < 0));
        return *this;
      };

      /**
       * Recede to previous item
       */
      inline iterator operator--(int) {
        iterator tmp(*this);
        do {
          --pos;
        } while ((pos >= 0) && ((*theVector)[pos].first < 0));
        return tmp;
      }; // postfix

      /**
       * Compare if both pointed positions are the same
       */
      inline bool operator==(const iterator& other) const {
        return (pos == other.pos);
      };

      /**
       * Compare if both pointed positions are different
       */
      inline bool operator!=(const iterator& other) const {
        return (pos != other.pos);
      };

      /**
       * Get pointed data
       */
      inline node_type& operator*() {return (*theVector)[pos].second;};
      
      /**
       * Get pointed node's id.
       *
       * Please note that the id can become invalid if the pointed node
       * is removed.
       */
      inline id_type id() const {
        return pos;
      }

      /**
       * Copy member
       */
      inline iterator& operator=(const iterator& other) {
        pos = other.pos;
        theVector = other.theVector;
        return *this;
      };

    protected:
      /**
       * Protected constructor (for internal use only)
       */
      explicit iterator(const int startPos,nodes_type* vct)
        : pos(startPos), theVector(vct) {};

      /**
       * For internal use only!!!
       */
      const int& getPos() const {return pos;};

      /**
       * For internal use only!!!
       */
      const nodes_type* getVector() const {return theVector;};

    private:
      /**
       * Actual vector index
       */
      int pos;

      /**
       * Pointer to the actual vector
       */
      nodes_type* theVector;
    };

    /**
     * The graph const_iterator iterates on all nodes in a read-only fashion.
     *
     * This class is similar to the usual iterators of the STL.
     * The const_iterator also allows you to access the id of the nodes. 
     * See the class iterator for more information.
     * 
     */
    class const_iterator {
      friend class adjacencyGraph<N,W,D,F,E>;
    public:
      /**
       * Default constructor
       */
      const_iterator()
        : pos(0), theVector(0) {};

      /**
       * Copy constructor
       */
      const_iterator(const const_iterator& other)
        : pos(other.pos),theVector(other.theVector) {};

      /**
       * Copy constructor
       */
      const_iterator(const iterator& other) {(*this)=other;};

      /**
       * Advance to next item
       */
      inline const_iterator& operator++() { // prefix
        do {
          ++pos;
        } while ((pos < static_cast<int>(theVector->size())) &&
                 ((*theVector)[pos].first < 0)) ;
        return *this;
      };  

      /**
       * Advance to next item
       */
      inline const_iterator operator++(int) { // postfix
        const_iterator tmp(*this);
        do {
          ++pos;
        } while ((pos < static_cast<int>(theVector->size())) &&
                 ((*theVector)[pos].first < 0));
        return tmp;
      }; 

      /**
       * Recede to previous item    // prefix
       */
      inline const_iterator& operator--() {
        do {
          --pos;
        } while ((pos >= 0) && ((*theVector)[pos].first < 0));
        return *this;
      };

      /**
       * Recede to previous item
       */
      inline const_iterator operator--(int) {
        const_iterator tmp(*this);
        do {
          --pos;
        } while ((pos >= 0) && ((*theVector)[pos].first < 0));
        return tmp;
      }; // postfix

      /**
       * Compare if both pointed positions are the same
       */
      inline bool operator==(const const_iterator& other) const {
        return (pos == other.pos);
      };

      /**
       * Compare if both pointed positions are the same
       */
      inline bool operator==(const iterator& other) const {
        return (pos == other.pos);
      };

      /**
       * Compare if both pointed positions are different
       */
      inline bool operator!=(const const_iterator& other) const {
        return (pos != other.pos);
      };

      /**
       * Compare if both pointed positions are different
       */
      inline bool operator!=(const iterator& other) const {
        return (pos != other.pos);
      };


      /**
       * Get pointed data
       */
      inline const node_type& operator*() const {
        return (*theVector)[pos].second;
      };

      /**
       * Get pointed node's id.
       *
       * Please note that the id can become invalid if the pointed node
       * is removed.
       */
      inline id_type id() const {
        return pos;
      }

      /**
       * Copy member
       */
      inline const_iterator& operator=(const iterator& other) {
        pos = other.getPos();
        theVector = other.getVector();
        return *this;
      };

      /**
       * Copy member
       */
      inline const_iterator& operator=(const const_iterator& other) {
        pos = other.pos;
        theVector = other.theVector;
        return *this;
      };

    protected:
      /**
       * Protected constructor (for internal use only)
       */
      explicit const_iterator(const int startPos,const nodes_type* vct)
        : pos(startPos), theVector(vct) {};

    private:
      /**
       * Actual vector index
       */
      int pos;

      /**
       * Pointer to the actual vector
       */
      const nodes_type* theVector;
    };

    class const_edge_iterator;

    /**
     * The edge iterator iterates on all outgoing edges of a node.
     *
     * This class is similar to the usual iterators of the STL.
     * The iterator allows you to access the data the edge, but
     * you can also get the next node's id for other kind of processing.
     * 
     * Since you got this iterator, you should know somehow which is the
     * starting edge node.
     *
     * To access the edge's data, use the operator* (for example, if
     * \c it is an edge_iterator, get the data with (*it) ).  To get the
     * ids of the nodes or the weight of the edge, use the corresponding
     * methods in the adjacencyGraph class.
     * 
     */
    class edge_iterator {
      friend class adjacencyGraph<N,W,D,F,E>;
      friend class adjacencyGraph<N,W,D,F,E>::const_edge_iterator;

    public:
      /**
       * Default constructor
       */
      edge_iterator() : pos(),currentNode() {};

      /**
       * Copy constructor
       */
      edge_iterator(const edge_iterator& other) 
        : pos(other.pos),currentNode(other.currentNode) {};

      /**
       * Advance to next item
       */
      inline edge_iterator& operator++() { // prefix
        ++pos;
        return *this;
      };  

      /**
       * Advance to next item
       */
      inline edge_iterator operator++(int) { // postfix
        edge_iterator tmp(*this);
        ++pos;
        return tmp;
      }; 

      /**
       * Recede to previous item    // prefix
       */
      inline edge_iterator& operator--() {
        --pos;
        return *this;
      };

      /**
       * Recede to previous item
       */
      inline edge_iterator operator--(int) {
        edge_iterator tmp(*this);
        --pos;
        return tmp;
      }; // postfix

      /**
       * Compare if both pointed positions are the same
       */
      inline bool operator==(const edge_iterator& other) const {
        return (pos == other.pos);
      };

      /**
       * Compare if both pointed positions are different
       */
      inline bool operator!=(const edge_iterator& other) const {
        return (pos != other.pos);
      };

      /**
       * Get pointed data
       */
      inline edge_data_type& operator*() {return (*pos).second.data;};
      
      /**
       * Get pointed data
       */
      inline const edge_data_type& operator*() const {
        return (*pos).second.data;
      };

      /**
       * Copy member
       */
      inline edge_iterator& operator=(const edge_iterator& other) {
        pos = other.pos;
        currentNode = other.currentNode;
        return *this;
      };

    protected:
      /**
       * Protected constructor (for internal use only)
       *
       * @param startPos map iterator
       * @param node id of the current node
       */
      explicit edge_iterator(const typename row_type::iterator& startPos,
                             const id_type node)
        : pos(startPos),currentNode(node) {};

      /**
       * For internal use only!!!
       */
      const typename row_type::iterator& getPos() const {return pos;};

      /**
       * Get complete matrix element entry.
       *
       * For internal use only!!!
       */
      entry_type& getRWEntry() const {
        return (*pos).second;
      };
      
      /**
       * Get complete matrix element entry.
       *
       * For internal use only!!!
       */
      const entry_type& getEntry() const {
        return (*pos).second;
      };

      /**
       * For internal use only!!!
       */
      id_type getNode() const {return currentNode;};

    private:
      /**
       * Actual vector index
       */
      typename row_type::iterator pos;

      /**
       * Current node, from which all edges come out
       */
      id_type currentNode;
    };

    /**
     * The graph const_iterator iterates on all nodes in a read-only fashion.
     *
     * This class is similar to the usual iterators of the STL.
     * The iterator allows you to access the data the edge, but
     * you can also get the next node's id for other kind of processing.
     * 
     * Since you got this iterator, you should know somehow which is the
     * starting edge node.
     *
     * To access the edge's data, use the operator* (for example, if
     * \c it is an edge_iterator, get the data with (*it) ).  To get the
     * ids of the nodes or the weight of the edge, use the corresponding
     * methods in the adjacencyGraph class.
     */
    class const_edge_iterator {
      friend class adjacencyGraph<N,W,D,F,E>;
    public:
      /**
       * Default constructor
       */
      const_edge_iterator() : pos(),currentNode() {};

      /**
       * Copy constructor
       */
      const_edge_iterator(const const_edge_iterator& other) 
        : pos(other.pos),currentNode(other.currentNode) {};

      /**
       * Copy constructor
       */
      const_edge_iterator(const edge_iterator& other) {(*this)=other;};

      /**
       * Advance to next item
       */
      inline const_edge_iterator& operator++() { // prefix
        ++pos;
        return *this;
      };  

      /**
       * Advance to next item
       */
      inline const_edge_iterator operator++(int) { // postfix
        const_edge_iterator tmp(*this);
        ++pos;        
        return tmp;
      }; 

      /**
       * Recede to previous item    // prefix
       */
      inline const_edge_iterator& operator--() {
        --pos;
        return *this;
      };

      /**
       * Recede to previous item
       */
      inline const_edge_iterator operator--(int) {
        const_edge_iterator tmp(*this);
        --pos;
        return tmp;
      }; // postfix

      /**
       * Compare if both pointed positions are the same
       */
      inline bool operator==(const const_edge_iterator& other) const {
        return (pos == other.pos);
      };

      /**
       * Compare if both pointed positions are the same
       */
      inline bool operator==(const edge_iterator& other) const {
        return (pos == other.getPos());
      };

      /**
       * Compare if both pointed positions are different
       */
      inline bool operator!=(const const_edge_iterator& other) const {
        return (pos != other.pos);
      };

      /**
       * Compare if both pointed positions are different
       */
      inline bool operator!=(const edge_iterator& other) const {
        return (pos != other.getPos());
      };

      /**
       * Get pointed data
       */
      inline const edge_data_type& operator*() const {
        return (*pos).second.data;
      };

      /**
       * Copy member
       */
      inline const_edge_iterator& operator=(const edge_iterator& other) {
        pos = other.getPos();
        currentNode = other.getNode();
        return *this;
      };

      /**
       * Copy member
       */
      inline const_edge_iterator& operator=(const const_edge_iterator& other) {
        pos = other.pos;
        currentNode = other.currentNode;
        return *this;
      };

    protected:
      /**
       * Protected constructor (for internal use only)
       */
      explicit const_edge_iterator(const typename row_type::const_iterator& sp,
                                   const id_type node)
        : pos(sp),currentNode(node) {};

      /**
       * Get complete matrix element entry.
       *
       * For internal use only!!!
       */
      const entry_type& getEntry() const {
        return (*pos).second;
      };

      /**
       * For internal use only!!!
       */
      id_type getNode() const {return currentNode;};

    private:
      /**
       * Actual vector index
       */
      typename row_type::const_iterator pos;

      /**
       * Current node, from which all edges come out
       */
      id_type currentNode;
    };
    
    // -----------------------------------------------------------------------
    // adjacency graph methods
    // -----------------------------------------------------------------------

    /**
     * Default constructor
     */
    adjacencyGraph();

    /**
     * Construct a graph with the given number of nodes, each
     * one initialized with the given data.
     */
    adjacencyGraph(const int number,const node_type& nodeData = node_type());

    /**
     * Copy constructor
     */
    adjacencyGraph(const adjacencyGraph<N,W,D,F,E>& other);

    /**
     * @name Iterators
     */
    //@{
    /**
     * Return a const_iterator to the first valid node of the graph.
     *
     * Note that you can not change the values of the graph's nodes
     * when you use a const_iterator. See also begin()
     */
    const_iterator begin() const;

    /**
     * Return an iterator to the first valid node of the graph.
     *
     * The use of the iterators is similar to the iterators of the
     * Standard Template Library (STL).
     * If you need to iterate on all nodes of a graph, you can
     * use following code:
     *
     * \code
     *   int tmp,accu;                        // a temporal variable
     *   lti::adjacencyGraph myGraph(10);     // a graph with 10 elements
     *   lti::adjacencyGraph::iterator it,eit;
     *
     *   for (it=myGraph.begin(),eit=myGraph.end(); it!=eit ; ++it) {
     *     // get the id of the nodes
     *     lti::adjacencyGraph::id_type theId = it.id();
     *     // do something with the graph id
     *   }
     * \endcode
     */
    iterator begin();
    
    /**
     * Returns last index as a const iterator.
     * For an example see begin()
     */
    const_iterator end() const;

    /**
     * Returns last index as an iterator.
     * For an example see begin()
     */
    iterator end();

    /**
     * Get edge iterator to the first outgoing edge of the given start node
     */
    edge_iterator begin(const id_type startNode);

    /**
     * Get edge iterator to the end of the outgoing edge list for the
     * given start node.
     */
    edge_iterator end(const id_type startNode);

    /**
     * Get edge iterator to the first outgoing edge of the given start node
     */
    const_edge_iterator begin(const id_type startNode) const;

    /**
     * Get edge iterator to the end of the outgoing edge list for the
     * given start node.
     */
    const_edge_iterator end(const id_type startNode) const;

    //@}

    /**
     * @name Node operations
     */
    //@{

    /**
     * Check if the given id is a valid one
     */
    bool isNodeIdValid(const id_type id) const;

    /**
     * Return the data contained in the node with the given id. 
     *
     * You must ensure that the given id is valid.  If it is not, 
     * an assertion will be thrown in debug modus, or some problems
     * will occur in release modus (segmentation fault or similar).
     *
     */
    const node_type& getNodeData(const id_type id) const;

    /**
     * Return the data contained in the node with the given id. 
     *
     * You must ensure that the given id is valid.  If it is not, 
     * an assertion will be thrown in debug modus, or some problems
     * will occur in release modus (segmentation fault or similar).
     *
     */
    node_type& getNodeData(const id_type id);

    /**
     * Change the data in a given node. 
     *
     * You must ensure that the given id is valid.  If it is not, 
     * an assertion will be thrown in debug modus, or some problems
     * will occur in release modus (segmentation fault or similar).
     *
     */
    node_type& setNodeData(const id_type id,const node_type& data);

    /**
     * Insert a node in the graph with the given data.
     * @param nodeData data to be included in the new node. If not given, a
     *                 default object will be created.
     * @return an identification key for the new inserted node.
     */
    id_type insertNode(const node_type& nodeData = node_type());

    /**
     * Insert the given number of nodes in the graphs, all having copies of
     * the same data object.
     * @param number number of nodes to be inserted.
     * @param nodeData data to be included in the new nodes. If omitted, a
     *                 default object will be created.
     * @return true if successful, false otherwise.
     */
    bool insertNodes(const int number,
                     const node_type& nodeData = node_type());

    /**
     * Remove the node and all edges from/to it.
     *
     * @param id identification key for the node to be removed.
     * @return true if node could be deleted.  false if some problem occurred,
     *         for example if the id is not valid.
     */
    bool removeNode(const id_type id);
    
    /**
     * Merge the two given nodes.
     * Topological merge of the two given nodes.
     *
     * The difference of this method with topologicalMerge is that
     * besides ensuring a topological merge, the data of the merged
     * edges and nodes is updated.  Consider for instance the
     * following graph with nodes A,B,w,x,y,z and edges Ax,Az,Aw,AB,Bw,By:
     *     
     * \code
     * x---A---B---y
     *     |\  |
     *     | \ |
     *     z   w
     * \endcode
     *
     * The merge of A and B results in
     *
     * \code
     * x---A---y
     *     |\  
     *     | \ 
     *     z   w
     * \endcode
     *
     * Here, the new weight of Aw is computed using the operator+= of the
     * weight_type type, which in this case does an update of:
     * - the node information of A (which is the combination of the previous
     *   node data A and B).  For this task, the N type must provide
     *   the operator+=.
     * - the edge weights between A and its old 
     *   neighbors x, z and w, the additional information update of
     *   the edge Aw, which now encompasses the old Aw and Bw, 
     *   and the weights to the new neighbor y.  The update of edge information
     *   assumes that the data_type provides the operator+=.
     *
     * @param first one of the two nodes to be merged.  
     * @param second the node to be merged with the first one.
     * @return the id of the new merged node.  This will usually be the
     *         id of the first node.
     *         If one of the ids does not exist, a negative id will be
     *         returned.
     */
    id_type mergeNodes(const id_type first,const id_type second);

    
    /**
     * Merge two nodes.
     *
     * @see mergeNodes(const id_type,const id_type)
     */
    id_type mergeNodes(const node_pair& edge);


    /**
     * Topological merge of the two given nodes.
     *
     * The difference of this method with mergeNodes is that it only
     * ensures that the topology of the merge node is kept.  Consider
     * the following graph with nodes A,B,w,x,y,z and edges
     * Ax,Az,Aw,AB,Bw and By:
     *     
     * \code
     * x---A---B---y
     *     |\  |
     *     | \ |
     *     z   w
     * \endcode
     *
     * The merge of A and B results in
     *
     * \code
     * x---A---y
     *     |\  
     *     | \ 
     *     z   w
     * \endcode
     *
     * Here, the weight of Aw is kept and the weight of A remain unchanged. 
     *
     * @param first one of the two nodes to be merged.  
     * @param second the node to be merged with the first one.
     * @return the id of the new merged node.  This will usually be the
     *         id of the first node.
     *         If one of the ids does not exist, a negative id will be
     *         returned.
     */
    id_type topologicalMerge(const id_type first,const id_type second);

    /**
     * Merge two nodes
     *
     * @see topologicalMerge(const id_type,const id_type)
     */
    id_type topologicalMerge(const node_pair& edge);


    /**
     * Return the number of outgoing edges for the given node
     */
    int numberEdges(const id_type node) const;


    /**
     * Remove all nodes and edges from the graph and insert the given
     * number of nodes, without any edges.
     *
     * This method ensures that the ids for the nodes lie between zero
     * and \a number-1.
     *
     * @param number number of nodes the graph must have.
     * @param nodeData data to be included in all nodes. If omitted, a
     *                 default data object will be created.
     * @return true if successful, false otherwise.
     */
    bool resize(const int number,
                const node_type& nodeData = node_type());


    /**
     * Return the number of nodes of this graph.  This value needs to be
     * computed, and therefore takes a little bit time.
     *
     * This value counts all nodes, independently if they are connected to
     * other nodes or not.  In an adjacency graph, isolated nodes can exist,
     * but they do not make much sence, since they are not adjacent to
     * anything.  Hence, Many algorithms require instead the number returned by
     * totalAdjacentNodes().
     *
     * @see totalAdjacentNodes();
     *
     */
    int size() const;

    /**
     * Return the number of nodes of this graph, that are connected to at
     * least another node.  Note that this value is always less or equal
     * the size().
     *
     * This value needs to be computed, and therefore, takes a little
     * bit time.
     *
     * In an adjacency graph, isolated nodes can exist, but they do not make
     * much sence, since they are not adjacent to anyone.  Many algorithm
     * require instead the number of nodes that are adjacent to something,
     * which is exactly what this method computes.
     *
     * @see size();
     */
    int totalAdjacentNodes() const;

    /**
     * Return the total number of edges of this graph.  This value needs to be
     * computed, and therefore takes a little bit time.
     */
    int totalEdges() const;

    /**
     * Return the largest valid node id (or negative if the graph is empty)
     */
    id_type lastValidId() const;

    //@}


    /**
     * @name Edge related methods
     */
    //@{

    /**
     * Get edge weight.
     *
     * Please remember that getEdgeWeight(a,b) is not necessarily the same 
     * than getEdgeWeight(b,a).  This depends on the definition of 
     * the function object of type weight_functor
     * 
     * @param a first node
     * @param b second node
     * @return the weight of the edge or edge_traits::Invalid if edge does
     * not exist
     */
    const weight_type& getEdgeWeight(const id_type a,
                                     const id_type b) const;


    /**
     * Get edge weight.
     *
     * Please remember that getEdgeWeight(a,b) is not necessarily the same 
     * than getEdgeWeight(b,a).  This depends on the definition of 
     * the function E::weight()
     * 
     * @param edge the pair of nodes describing an edge.
     * @return the weight of the edge or edge_traits::Invalid if edge does 
     *         not exist or is a topological edge, without associated weight.
     */
    const weight_type& getEdgeWeight(const node_pair& edge) const;

    /**
     * Get edge weight.
     *
     * Please remember that getEdgeWeight(a,b) is not necessarily the same 
     * than getEdgeWeight(b,a).  This depends on the definition of 
     * the function E::weight()
     * 
     * @param it iterator to the edge
     * @return the weight of the edge or edge_traits::Invalid if it
     *         is a topological edge, without associated weight
     */
    const weight_type& getEdgeWeight(const edge_iterator& it) const;

    /**
     * Get edge weight.
     *
     * Please remember that getEdgeWeight(a,b) is not necessarily the same 
     * than getEdgeWeight(b,a).  This depends on the definition of 
     * the function E::weight()
     * 
     * @param it iterator to the edge
     * @return the weith of the edge or edge_traits::Invalid if edge does not
     *         exist
     */
    const weight_type& getEdgeWeight(const const_edge_iterator& it) const;

    /**
     * Change the weight of the given edge.
     *
     * This method updates the weigth of the edge (a,b).
     *
     * If the constant E::Symmetric is true, then also the weight of
     * the edge (b,a) is updated.
     *
     * @param a first node
     * @param b second node
     * @param weight new weight for the edge.
     * @return true if update successful, or false if the edge didn't exist.
     */
    bool updateEdgeWeight(const id_type a,
                          const id_type b,
                          const weight_type& weight);

    /**
     * Change the weight of the given edge.
     *
     * This method updates the weigth of the edge (a,b).
     *
     * If the constant E::Symmetric is true, then also the weight of
     * the edge (b,a) is updated.
     *
     * @param edges pair of nodes (a=edges.first,b=edges.second)
     * @param weight new weight for the edge.
     * @return true if update successful, or false if the edge didn't exist.
     */
    bool updateEdgeWeight(const node_pair& edges,
                          const weight_type& weight);

    /**
     * Change the weight of the given edge
     *
     * This method updates the weigth of the edge (a,b).
     *
     * If the constant E::Symmetric is true, then also the weight of
     * the edge (b,a) is updated.
     *
     * @param it iterator to the edge
     * @param weight new weight for the edge.
     * @return true if update successful, or false if the edge didn't exist.
     */
    bool updateEdgeWeight(const edge_iterator& it,
                          const weight_type& weight);

    /**
     * Recompute the weight of the given edge.
     *
     * This method updates the weigth of the edge (a,b) to the one computed
     * by the E::weight() method.
     *
     * If the constant E::Symmetric is true, then also the weight of
     * the edge (b,a) is updated.
     *
     * @param a first node
     * @param b second node
     * @return true if update successful, or false if the edge didn't exist.
     */
    bool updateEdgeWeight(const id_type a,
                          const id_type b);

    /**
     * Change the weight of the given edge.
     *
     * This method updates the weigth of the edge (a,b) to the one computed
     * by the E::weight() method.
     *
     * If the constant E::Symmetric is true, then also the weight of
     * the edge (b,a) is updated.
     *
     * @param edges pair of nodes (a=edges.first,b=edges.second)
     * @return true if update successful, or false if the edge didn't exist.
     */
    bool updateEdgeWeight(const node_pair& edges);

    /**
     * Change the weight of the given edge
     *
     * This method updates the weigth of the edge (a,b) to the one computed
     * by the E::weight() method.
     *
     * If the constant E::Symmetric is true, then also the weight of
     * the edge (b,a) is updated.
     *
     * @param it iterator to the edge
     * @return true if update successful, or false if the edge didn't exist.
     */
    bool updateEdgeWeight(const edge_iterator& it);

    /**
     * Change the data of the given edge.
     *
     * This method updates the data of the edge (a,b).
     *
     * If the constant E::Symmetric is true, then also the data of
     * the edge (b,a) is updated.
     *
     * @param a first node
     * @param b second node
     * @param data new data for the edge.
     * @return true if update successful, or false if the edge didn't exist.
     */
    bool setEdgeData(const id_type a,
                     const id_type b,
                     const edge_data_type& data);

    /**
     * Change the data of the given edge.
     *
     * This method updates the data of the edge (a,b).
     *
     * If the constant E::Symmetric is true, then also the data of
     * the edge (b,a) is updated.
     *
     * @param edges pair of nodes (a=edges.first,b=edges.second)
     * @param data new data for the edge.
     * @return true if update successful, or false if the edge didn't exist.
     */
    bool setEdgeData(const node_pair& edges,
                     const edge_data_type& data);

    /**
     * Change the data of the given edge
     *
     * This method updates the data of the edge (a,b).
     *
     * If the constant E::Symmetric is true, then also the data of
     * the edge (b,a) is updated.
     *
     * @param it iterator to the edge
     * @param data new data for the edge.
     * @return true if update successful, or false if the edge didn't exist.
     */
    bool setEdgeData(const edge_iterator& it,
                     const edge_data_type& data);
    

    /**
     * Get a read-only reference to the data object of the edge.
     *
     * If you have a const_edge_iterator, remember that you can get the edge
     * data dereferencing it, i.e. if \c iter is your iterator, then 
     * <code>(*iter)</code> is the edge's data.
     *
     * @param a first node
     * @param b second node
     * @return read-only reference to the data.  If the edge didn't exist
     *         expect unpredictable behaviour!
     *
     */
    const edge_data_type& getEdgeData(const id_type a,
                                      const id_type b) const;

    /**
     * Get a read-only reference to the data object of the edge.
     *
     * If you have a const_edge_iterator, remember that you can get the edge
     * data dereferencing it, i.e. if \c iter is your iterator, then 
     * <code>(*iter)</code> is the edge's data.
     *
     * @param edges pair of nodes (a=edges.first,b=edges.second)
     * @return read-only reference to the data.  If the edge didn't exist
     *         expect unpredictable behaviour!
     */
    const edge_data_type& getEdgeData(const node_pair& edges) const;

    /**
     * Get a writable reference to the data object of the edge.
     *
     * If you have an edge_iterator, remember that you can get the edge
     * data dereferencing it, i.e. if \c iter is your iterator, then 
     * <code>(*iter)</code> is the edge's data.
     *
     * @param a first node
     * @param b second node
     * @return read-only reference to the data.  If the edge didn't exist
     *         expect unpredictable behaviour!
     */
    edge_data_type& getEdgeData(const id_type a,
                                const id_type b);

    /**
     * Get a writable reference to the data object of the edge.
     *
     * If you have an edge_iterator, remember that you can get the edge
     * data dereferencing it, i.e. if \c iter is your iterator, then 
     * <code>(*iter)</code> is the edge's data.
     *
     * @param edges pair of nodes (a=edges.first,b=edges.second)
     * @return read-only reference to the data.  If the edge didn't exist
     *         expect unpredictable behaviour!
     */
    edge_data_type& getEdgeData(const node_pair& edges);

    /**
     * Get edge with the lowest weight in the graph.
     * @param a first node
     * @param b second node
     * @param weight weight of the edge
     * @return true if graph not empty, false otherwise (in which case the
     *              three parameters will be left untouched.
     */
    bool getLowestWeightEdge(id_type& a,
                             id_type& b,
                             weight_type& weight) const;


    /**
     * Get pair of nodes with the lowest valid edge weight.
     * @param edge the edge contains both nodes
     * @param weight weight of the edge
     * @return true if graph not empty, false otherwise (in which case the
     *              three parameters will be left untouched).
     */
    bool getLowestWeightEdge(node_pair& edge,
                             weight_type& weight) const;


    /**
     * Get the id of the node at the other side of the edge pointed by the
     * given iterator
     */
    id_type getOtherNode(const edge_iterator& it) const;
    
    /**
     * Get the id of the node at the other side of the edge pointed by the
     * given iterator
     */
    id_type getOtherNode(const const_edge_iterator& it) const;

    /**
     * Get edge as node_pair pointed by the given iterator
     */
    node_pair getEdge(const edge_iterator& it) const;

    /**
     * Get edge as node_pair pointed by the given iterator
     */
    node_pair getEdge(const const_edge_iterator& it) const;

    /**
     * Insert an edge between the first and second nodes, identified by their
     * ids.  The edge is assumed symmetrical, so that an edge with the same
     * data type will be inserted between the second and the first nodes.
     *
     * The weight of both edges (first->second, second->first) will be
     * computed from the data of both nodes and the given edge data,
     * using the function E::weight().
     *
     * @param first id of the first node (source one).
     * @param second id of the second node (destination one).
     * @param init initial data set for the edge.
     * @return true if edge could be set successfully inserted, false
     *         otherwise (for example, if one or both ids are invalid, or if
     *         the edge already existed).
     */
    bool insertEdge(const id_type first,
                    const id_type second,
                    const edge_data_type& init = edge_data_type());

    /**
     * Insert an edge between the first and second nodes, identified by their
     * ids.  The edge is assumed symmetrical, so that an edge with the same
     * data type will be inserted between the second and the first nodes.
     *
     * The weight of both edges (first->second, second->first) will be
     * computed from the data of both nodes and the given edge data,
     * using the function E::weight().
     *
     * @param nodes the first and second nodes, which identify the edge
     * @param init initial data set for the edge.
     * @return true if edge could be set successfully inserted, false
     *         otherwise (for example, if one or both ids are invalid, or if
     *         the edge already existed).
     */
    bool insertEdge(const node_pair& nodes,
                    const edge_data_type& init = edge_data_type());


    /**
     * Insert an edge between the first and second nodes, identified by their
     * ids.
     *
     * The weight of both edges (first->second, second->first) will be
     * computed from the data of both nodes and the given edge data,
     * using the function E::weight().
     *
     * @param first id of the first node (source one).
     * @param second id of the second node (destination one).
     * @param init12 initial data for the first to second edge.
     * @param init21 initial data for the second to first edge.
     * @return true if edge could be set successfully inserted, false
     *         otherwise (for example, if one or both ids are invalid, or if
     *         the edge already existed).
     */
    bool insertEdge(const id_type first,
                    const id_type second,
                    const edge_data_type& init12,
                    const edge_data_type& init21);

    /**
     * Insert an edge between the first and second nodes, identified by their
     * ids.  The data of each
     *
     * The weight of both edges (first->second, second->first) will be
     * computed from the data of both nodes and the given edge data,
     * using the function E::weight().
     *
     * @param nodes the first and second nodes, which identify the edge
     * @param init12 initial data for the first to second edge.
     * @param init21 initial data for the second to first edge.
     * @return true if edge could be set successfully inserted, false
     *         otherwise (for example, if one or both ids are invalid, or if
     *         the edge already existed).
     */
    bool insertEdge(const node_pair& nodes,
                    const edge_data_type& init12,
                    const edge_data_type& init21);

    /**
     * Insert an edge between the first and second nodes, identified by their
     * ids.
     *
     * @param first id of the first node (source one).
     * @param second id of the second node (destination one).
     * @param init12 initial data for the first to second edge.
     * @param weight12 weight of the edge between first and second
     * @param init21 initial data for the second to first edge.
     * @param weight21 weight of the edge between second and first
     * @return true if edge could be set successfully inserted, false
     *         otherwise (for example, if one or both ids are invalid, or if
     *         the edge already existed).
     */
    bool insertWeightedEdge(const id_type first,
                            const id_type second,
                            const edge_data_type& init12,
                            const weight_type& weight12,
                            const edge_data_type& init21,
                            const weight_type& weight21);

    /**
     * Insert an edge between the first and second nodes, identified by their
     * ids.  The data of each
     *
     * @param nodes the first and second nodes, which identify the edge
     * @param init12 initial data for the first to second edge.
     * @param weight12 weight of the edge between nodes.first and nodes.second
     * @param init21 initial data for the second to first edge.
     * @param weight21 weight of the edge between nodes.second and nodes.first
     * @return true if edge could be set successfully inserted, false
     *         otherwise (for example, if one or both ids are invalid, or if
     *         the edge already existed).
     */
    bool insertWeightedEdge(const node_pair& nodes,
                            const edge_data_type& init12,
                            const weight_type& weight12,
                            const edge_data_type& init21,
                            const weight_type& weight21);
    

    /**
     * Return a reference to the given edge's data, or create the edge
     * pair if it didn't exist and return the reference to the created
     * data.
     * 
     * The created edge will not have an associated weight, to save the time
     * of computing the weight and administrating it in the corresponding
     * sorted data structure.
     *
     * This method is useful for complex graph operations, which need to build
     * first the graph structure and compute in a second stage all weights and
     * set them at once with the method TODO.  Avoiding an unnecessary
     * preliminar weight computation and the unnecessary sorting of such
     * weight permits to save lots of time.
     *
     * Of course, this method can also be (ab)used to create simple
     * non-weighted bidirectional adjacency graphs if you need to analyze only
     * topological features of an adjacency problem.
     * 
     * You can later assign a weight to the edges with the updateEdgeWeight
     * methods.
     * 
     */
    edge_data_type& forceTopologicalEdge(const node_pair& nodes);
                                         
    /**
     * Return a reference to the given edge's data, or create the edge
     * pair if it didn't exist and return the reference to the created
     * data.
     * 
     * The created edge will not have an associated weight, to save the time
     * of computing the weight and administrating it in the corresponding
     * sorted data structure.
     *
     * This method is useful for complex graph operations, which need to build
     * first the graph structure and compute in a second stage all weights and
     * set them at once with the method TODO.  Avoiding an unnecessary
     * preliminar weight computation and the unnecessary sorting of such
     * weight permits to save lots of time.
     *
     * Of course, this method can also be (ab)used to create simple
     * non-weighted bidirectional adjacency graphs if you need to analyze only
     * topological features of an adjacency problem.
     * 
     * You can later assign a weight to the edges with the updateEdgeWeight
     * methods.
     * 
     */
    edge_data_type& forceTopologicalEdge(const id_type first,
                                         const id_type second);

    /**
     * Remove the edge between the given two nodes.
     *
     * In order to ensure the adjacency graph consistency not only the
     * first-to-second edge will be removed, but also the
     * second-to-first one.
     *
     * @param first id of the first node (source one).
     * @param second id of the second node (destination one).
     * @return true if edge could be deleted.  false if some problem occurred,
     *         for example if the ids are not valid, or the edge didn't exist.
     */
    bool removeEdge(const id_type first,const id_type second);

    /**
     * Remove the edge between the given two nodes.
     *
     * In order to ensure the adjacency graph consistency not only the
     * first-to-second edge will be removed, but also the
     * second-to-first one.
     *
     * @param nodes first and second node ids, which identify the edge
     * @return true if edge could be deleted.  false if some problem occurred,
     *         for example if the ids are not valid, or the edge didn't exist.
     */
    bool removeEdge(const node_pair& nodes);

    /**
     * Remove the edge between the given two nodes.
     *
     * In order to ensure the adjacency graph consistency not only the
     * first-to-second edge will be removed, but also the
     * second-to-first one.
     *
     * @param edge edge_iterator pointing to the edge to be removed.
     */
    bool removeEdge(const edge_iterator& edge);

    /**
     * Call the E::weight() method to compute the weight an edge
     * from a to b should have, considering the node and edges current data.  
     *
     * Please note that if you just want to get the current weight of an edge,
     * the getEdgeWeight methods are much much faster (they don't compute
     * anything, but just return a previously set or computed  weight value).
     *
     * For this method, you should check if a and b are valid ids or your
     * program may crash!
     *
     * Note that after topological node merges or topogical edge insertions
     * the weight values of edges can be edge_traits::Invalid.  Therefore,
     * this method is provided to compute them, without changing anything.
     */
    weight_type computeEdgeWeight(const id_type a,
                                  const id_type b) const;


    /**
     * Call the E::weight() method to compute the weight an edge
     * from a to b should have, considering the node and edges current data.  
     *
     * Please note that if you just want to get the current weight of an edge,
     * the getEdgeWeight methods are much much faster (they don't compute
     * anything, but just return a previously set or computed  weight value).
     *
     * For this method, you should check if a and b are valid ids or your
     * program may crash!
     *
     * Note that after topological node merges or topogical edge insertions
     * the weight values of edges can be edge_traits::Invalid.  Therefore,
     * this method is provided to compute them, without changing anything.
     */
    weight_type computeEdgeWeight(const node_pair& edge) const;

    /**
     * Call the E::weight() method to compute the weight an edge
     * from a to b should have, considering the node and edges current data.  
     *
     * Please note that if you just want to get the current weight of an edge,
     * the getEdgeWeight methods are much much faster (they don't compute
     * anything, but just return a previously set or computed  weight value).
     *
     * For this method, you should check if a and b are valid ids or your
     * program may crash!
     *
     * Note that after topological node merges or topogical edge insertions
     * the weight values of edges can be edge_traits::Invalid.  Therefore,
     * this method is provided to compute them, without changing anything.
     */
    weight_type computeEdgeWeight(const edge_iterator& it) const;

    /**
     * Call the E::weight() method to compute the weight an edge
     * from a to b should have, considering the node and edges current data.  
     *
     * Please note that if you just want to get the current weight of an edge,
     * the getEdgeWeight methods are much much faster (they don't compute
     * anything, but just return a previously set or computed  weight value).
     *
     * For this method, you should check if a and b are valid ids or your
     * program may crash!
     *
     * Note that after topological node merges or topogical edge insertions
     * the weight values of edges can be edge_traits::Invalid.  Therefore,
     * this method is provided to compute them, without changing anything.
     */
    weight_type computeEdgeWeight(const const_edge_iterator& it) const;

    /**
     * This method uses the data in the nodes and the edges to recompute
     * all graph weights.  It is more efficient than recomputing the
     * weights one by one, since the sorting of the weights can occur 
     * at once, and not iteratively.
     *
     * If the graph is symmetric, only the bottom diagonal affinity matrix
     * will be used, i.e. only the edges (a,b) with a\>b will be considered
     * to the computations and the results will also be assigned to (b,a).
     */
    bool recomputeAllWeights();

    //@}

    /**
     * Remove all nodes and edges from the graph
     */
    bool clear();

    /**
     * Check if the graph is empty, i.e. if it has no nodes.
     */
    inline bool empty() const;

    /**
     * @name Weight computation functor
     */
    //@{
    /**
     * Get a read-only reference to the internal weight computation functor 
     * of type weight_functor (template parameter F)
     */
    const weight_functor& getWeightFunctor() const;

    /**
     * Set a the functor to be used to compute the weights.
     *
     * This method assumes that the weight_functor implements the operator=(),
     * even the default implementation or a specialized one.
     */
    void setWeightFunctor(const weight_functor& ftor);
    //@}
    
    /**
     * @name Copy methods
     */
    //@{
    /**
     * Copy the other adjacency matrix
     */
    adjacencyGraph<N,W,D,F,E>& copy(const adjacencyGraph<N,W,D,F,E>& other);

    /**
     * Copy the other adjacency matrix
     */
    adjacencyGraph<N,W,D,F,E>& 
    operator=(const adjacencyGraph<N,W,D,F,E>& other);
    
    /**
     * Returns a copy of this object
     */
    virtual container* clone() const;

    /**
     * Returns a copy of this object
     */
    virtual container* newInstance() const;
    //@}

    /**
     * Returns the name of this class.
     */
    virtual const std::string& name() const;

    /**
     * Write the parameters in the given ioHandler.
     *
     * The adjacency graphs are containers, and therefore are
     * serializable.  If you want to edit the written files, you need to
     * know the syntax therein.
     * A file contains four data blocks:
     * -# The size of the data structure, which is always greater or equal 
     *    to the number of nodes.
     * -# The nodes list containing for each node two elements: the
     *    node's id and the data of the node.
     * -# A boolean indicating if the saved data belongs to a symmetric graph
     *    or not.
     * -# The edge list containing for each one three elements: the edge as
     *    pair of node ids, the edge's weight, the edge's data
     *
     * @param handler the ioHandler to be used
     * @param complete if true (the default) the enclosing begin/end will
     *        be also written, otherwise only the data block will be written.
     * @return true if write was successful
     */
    virtual bool write(ioHandler& handler, const bool complete=true) const;

    /**
     * Read the parameters from the given ioHandler.
     * @param handler the ioHandler to be used
     * @param complete if true (the default) the enclosing begin/end will
     *        be also written, otherwise only the data block will be written.
     * @return true if write was successful
     */
    virtual bool read(ioHandler& handler,const bool complete=true);    

    /**
     * Check Graph Consistency
     *
     * This method is for debug purposes only.
     *
     * If everything is ok, this should by equivalent to \c true.  But
     * if some bug decided to live here, it can produce a \c false,
     * which shouldn't ever happen!
     *
     * Don't ever rely on this method to do anything, since it will be removed
     * as soon as we are sure, everything works.  In other words, this method
     * is already deprecated!
     */
    bool checkConsistency() const;

  protected:
    /**
     * Adjacency matrix
     */
    adjacency_type adjacency_;

    /**
     * The nodes
     */
    nodes_type nodes_;

    /**
     * Priority queue ordered by the distances.
     */
    queue_type distances_;

    /**
     * First valid vector index
     */
    id_type firstValidNodeIndex_;

    /**
     * First valid vector index
     */
    id_type lastValidNodeIndex_;

    /**
     * Number of elements in the nodes vector that has been freed
     */
    int freeElements_;

    /**
     * The one and only instance of E, used to compute the weights
     * between two nodes
     */
    weight_functor theWeightFunctor_;

    /**
     * After copying or loading a graph, the "complement" interators in the
     * entries of the matrix are invalid.  This method ensures consistency
     */
    bool fixEntryIterators();

    /**
     * Insert an edge between the first and second nodes, identified by their
     * ids.
     *
     * This method assumes the validity of the first and second ids, and 
     * therefore is protected!
     * 
     * @param first id of the first node (source one).
     * @param second id of the second node (destination one).
     * @param init12 initial data for the first to second edge.
     * @param weight12 weight of the edge between first and second
     * @param init21 initial data for the second to first edge.
     * @param weight21 weight of the edge between second and first
     * @return true if edge could be set successfully inserted, false
     *         otherwise (for example, if one or both ids are invalid, or if
     *         the edge already existed).
     */
    bool insertWeightedEdgeProt(const id_type first,
                                const id_type second,
                                const edge_data_type& init12,
                                const weight_type& weight12,
                                const edge_data_type& init21,
                                const weight_type& weight21);


    /**
     * Get the id of the node corresponding to the given 
     * iterator
     */
    id_type getNodeId(const typename entry_type::entry_iterator& it) const;

  };
}

#include "ltiAdjacencyGraph_template.h"
#endif

