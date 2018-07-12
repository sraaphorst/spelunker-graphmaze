/**
 * MazeGraph.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <boost/graph/adjacency_list.hpp>

#include <map>
#include <vector>

namespace spelunker::graphmaze {
    /**
     * To represent a potential maze for building, we have vertices
     * and we have edges. We want to derive, for a perfect maze, a
     * spanning tree of this graph. Afterward, we can add features
     * like braiding to increase the complexity.
     */
    using MazeGraph = boost::adjacency_list<
            boost::setS, boost::vecS, boost::undirectedS>;

    /// Alias for vertices of MazeGraph.
    using vertex = MazeGraph::vertex_descriptor;

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
}
