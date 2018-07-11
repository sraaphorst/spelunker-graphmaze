/**
 * MazeGenerator.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <boost/graph/adjacency_list.hpp>

#include "MazeGraph.h"
#include "MazeGenerator.h"

namespace spelunker::graphmaze {
    MazeGenerator::MazeGenerator(const int w, const int h)
            : width{w}, height{h} {}

    MazeGraph MazeGenerator::createInitialMaze(const MazeGraph &tmplt) const noexcept {
        MazeGraph out;
        for (auto[vIter, vEnd] = boost::vertices(tmplt); vIter != vEnd; ++vIter)
            boost::add_vertex(out);
        return out;
    }

    MazeGenerator::UnvisitedVertices MazeGenerator::initializeUnvisitedVertices(const MazeGraph &tmplt) const noexcept {
        UnvisitedVertices unvisited;
        for (auto[vIter, vEnd] = boost::vertices(tmplt); vIter != vEnd; ++vIter)
            unvisited[*vIter] = true;
        return unvisited;
    }
}