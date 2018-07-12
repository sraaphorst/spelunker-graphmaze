/**
 * MazeGenerator.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <boost/graph/adjacency_list.hpp>

#include <algorithm>

#include "MazeGraph.h"
#include "MazeGenerator.h"

namespace spelunker::graphmaze {
    MazeGenerator::MazeGenerator(const int w, const int h)
            : width{w}, height{h} {}

    MazeGraph MazeGenerator::createInitialMaze(const MazeGraph &tmplt) noexcept {
        MazeGraph out;
        for (auto[vIter, vEnd] = boost::vertices(tmplt); vIter != vEnd; ++vIter)
            boost::add_vertex(out);
        return out;
    }

    MazeGenerator::UnvisitedVertices MazeGenerator::initializeUnvisitedVertices(const MazeGraph &tmplt) noexcept {
        UnvisitedVertices unvisited;
        for (auto[vIter, vEnd] = boost::vertices(tmplt); vIter != vEnd; ++vIter)
            unvisited[*vIter] = true;
        return unvisited;
    }

    // TODO: Fix this up.
    vertex MazeGenerator::randomStartVertex(const MazeGraph &maze) noexcept {
        return 0;
    }

    VertexCollection MazeGenerator::unvisitedNeighbours(const MazeGraph &tmplt,
            MazeGenerator::UnvisitedVertices &unvisited,
            const vertex &v) {
        VertexCollection unvisitedNeighbours;
        for (auto [eiter, eEnd] = boost::out_edges(v, tmplt); eiter != eEnd; ++eiter) {
            const vertex t = boost::target(*eiter, tmplt);
            if (unvisited[t])
                unvisitedNeighbours.emplace_back(t);
        }
        return unvisitedNeighbours;
    }

    VertexCollection MazeGenerator::neighbours(const MazeGraph &tmplt, const vertex &v) {
        VertexCollection neighbours;
        for (auto [eiter, eEnd] = boost::out_edges(v, tmplt); eiter != eEnd; ++eiter) {
            const auto t = boost::target(*eiter, tmplt);
            neighbours.emplace_back(t);
        }
        return neighbours;
    }

    int MazeGenerator::numCells(const MazeGraph &maze) {
        auto [vIter, vEnd] = boost::vertices(maze);
        return std::distance(vIter, vEnd);
    }
}