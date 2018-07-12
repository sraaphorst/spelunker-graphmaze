/**
 * HuntAndKillMazeGenerator.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include "GraphUtils.h"
#include "HuntAndKillMazeGenerator.h"
#include "MazeGraph.h"

namespace spelunker::graphmaze {

    const MazeGraph HuntAndKillMazeGenerator::generate(const MazeGraph &tmplt) const {
        MazeSeed seed = GraphUtils::makeSeed(tmplt);

        // Get a random starting cell.
        const auto start = GraphUtils::randomStartVertex(seed.maze);

        // Keep track of where to search when we start fresh as we begin iterating at the top left, looking for an
        // unvisited cell.
        auto nextX = 0;
        auto nextY = 0;

        // Continuously carve a random path until
        return seed.maze;
    }
}