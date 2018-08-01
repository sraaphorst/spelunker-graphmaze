/**
 * MazeGraph.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/properties.hpp>

#include <deque>
#include <functional>
#include <map>
#include <optional>
#include <set>
#include <stdexcept>
#include <tuple>
#include <vector>

#include <typeclasses/Show.h>
#include <types/Direction.h>

namespace spelunker::graphmaze {
    /**
     * Forward declarations of MazeGraph to get vertex_descriptor.
     */
    struct VertexInfo;
    struct EdgeInfo;
    struct GraphInfo;

    struct VertexInfoPropertyTag {
        using kind = boost::vertex_property_tag;
        static const std::size_t num;
    };

    struct EdgeInfoPropertyTag {
        using kind = boost::edge_property_tag;
        static const std::size_t num;
    };

    struct GraphInfoPropertyTag {
        using kind = boost::graph_property_tag;
        static const std::size_t num;
    };

    using vertex_info_prop_type = boost::property<VertexInfoPropertyTag, VertexInfo>;
    using edge_info_prop_type   = boost::property<EdgeInfoPropertyTag, EdgeInfo>;
    using graph_info_prop_type  = boost::property<GraphInfoPropertyTag, GraphInfo>;

    /**
     * To represent a potential maze for building, we have vertices
     * and we have edges. We want to derive, for a perfect maze, a
     * spanning tree of this graph. Afterward, we can add features
     * like braiding to increase the complexity.
     */
    using MazeGraph = boost::adjacency_list<
            boost::setS, boost::vecS, boost::undirectedS,
            vertex_info_prop_type, edge_info_prop_type, graph_info_prop_type
            >;

    /// Alias for vertices of MazeGraph.
    using vertex = MazeGraph::vertex_descriptor;

    /// Alias for edges of MazeGraph.
    using edge = MazeGraph::edge_descriptor;

    /// A grid ranker map used to map (x,y) coordinates into vertices.
    using GridRankerMap = std::map<std::pair<int, int>, vertex>;

    /**
     * Vertex properties. A vertex can have a type to indicate its shape or orientation, which is graph-dependent.
     * Examples include some types of omega (graph with non-orthogonal tessellation) graphs, such as:
     * 1. Delta (triangular) graphs, where each triangle has an orientation;
     * 2. Upsilon graphs, where the cells comprise octagons and squares.
     * The type should be uniquely interpreted by the graph.
     */
    struct VertexInfo {
        int type;
    };

    /**
     * Edge properties. We want to have some notion of "direction" associated with edges.
     * We maintain two pairs, each consisting of a vertex of the edge and the direction the edge represents to reach the
     * other endpoint. This thus contains some redundant information, but we include it for completeness. If may be
     * possible to exploit it in some way to produce interesting effects.
     */
     struct EdgeInfo {
        vertex v1;
        types::Direction d1;
        vertex v2;
        types::Direction d2;
     };

     /**
      * Graph properties. These consist of several fundamental properties of the graph that allow us to use or not
      * use certain algorithms. For example, to use the binary tree algorithm, we need to specify directions based
      * on the vertex type for the algorithm to choose from. For the sidewinder algorithm, we need these same
      * directions, but with a specific order imposed on them: hence, the use of a deque.
      */
     using BTCandidateFunction = std::function<std::deque<types::Direction>(int)>;
     struct GraphInfo {
         GraphInfo() {}
         GraphInfo(const BTCandidateFunction &binaryTreeCandidates,
                   const GridRankerMap &gridRankerMap)
            : binaryTreeCandidates{binaryTreeCandidates},
              gridRankerMap{gridRankerMap} {};

         std::optional<GridRankerMap> gridRankerMap;
         std::optional<BTCandidateFunction> binaryTreeCandidates;
     };

    /// A collection of vertices.
    using VertexCollection = std::vector<vertex>;

    /// A collection of unvisited vertices.
    using UnvisitedVertices = std::map<vertex, bool>;

    /// Starting point for a maze.
    /**
     * These are the starting parameters for maze generation from a graph.
     * They are as follows:
     * 1. tmplt: The template from which this maze will be based: represents the cells and their possible connections.
     * 1. maze: A graph created from the template graph: it is the same as the template graph but with no edges.
     *    The edges added during maze generation will represent the carved passages and should be a subset
     *    of edges that appear in the template.
     * 2. numVertices: The number of vertices (aka cells) in the maze.
     * 3. unvisited: A vector of length numVertices representing if a vertex has yet to be visited or not.
     */
     struct MazeSeed {
         const MazeGraph &tmplt;
         MazeGraph maze;
         const int numVertices;
         UnvisitedVertices unvisited;
     };

     /// A map of (row, col) to vertex.
     using VertexRank = std::map<std::pair<int, int>, vertex>;
}
