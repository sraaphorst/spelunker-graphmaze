/**
 * AldousBroderMazeGenerator.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <tuple>

#include <math/RNG.h>

#include "GraphUtils.h"
#include "MazeGraph.h"
#include "MazeGenerator.h"
#include "AldousBroderMazeGenerator.h"

namespace spelunker::graphmaze {

    std::pair<const MazeGraph, const vertex> AldousBroderMazeGenerator::generate(const MazeGraph &tmplt) const {
        auto seed = GraphUtils::makeSeed(tmplt);

        // Pick a random vertex to start.
        const auto start = GraphUtils::randomStartVertex(seed.maze);
        auto v = start;
        auto visitedCells = 1;
        seed.unvisited[v] = false;

        // Continue until we have visited all the cells.
        while (visitedCells < seed.numVertices) {
            // Get all the neighbours of the current cell and move to one at random.
            const auto nbrs = GraphUtils::neighbours(seed, v);

            // Select an unvisited neighbour at random.
            const auto nxt = math::RNG::randomElement(nbrs);

            if (seed.unvisited[nxt]) {
                seed.unvisited[nxt] = false;
                ++visitedCells;
                GraphUtils::addEdge(v, nxt, seed);
            }
            v = nxt;
        }

        return {seed.maze, start};
    }
}
