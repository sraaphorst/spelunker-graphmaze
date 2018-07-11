/**
 * AldousBroderMazeGenerator.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <boost/graph/adjacency_list.hpp>

#include <map>
#include <stack>
#include <vector>

#include "MazeGraph.h"
#include "MazeGenerator.h"
#include "AldousBroderMazeGenerator.h"

namespace spelunker::graphmaze {
    AldousBroderMazeGenerator::AldousBroderMazeGenerator(int width, int height)
        : MazeGenerator{width, height} {}

    const MazeGraph AldousBroderMazeGenerator::generate(const MazeGraph &tmplt) const {
        MazeGraph out = createInitialMaze(tmplt);
        UnvisitedVertices unvisited = initializeUnvisitedVertices(tmplt);

        return out;
    }
}
