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
}
