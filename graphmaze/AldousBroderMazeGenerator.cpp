/**
 * AldousBroderMazeGenerator.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <boost/graph/adjacency_list.hpp>

#include <algorithm>
#include <map>
#include <stack>
#include <vector>

#include "GraphUtils.h"
#include "MazeGraph.h"
#include "MazeGenerator.h"
#include "AldousBroderMazeGenerator.h"

#include <iostream>

namespace spelunker::graphmaze {

    const MazeGraph AldousBroderMazeGenerator::generate(const MazeGraph &tmplt) const {
        MazeSeed seed = GraphUtils::makeSeed(tmplt);

        // Pick a random vertex to start.
        auto v = GraphUtils::randomStartVertex(seed.maze);
        auto visitedCells = 1;
        seed.unvisited[v] = false;

        // Continue until we have visited all the cells.
        while (visitedCells < seed.numVertices) {
            // Get all the neighbours of the current cell and move to one at random.
            const auto nbrs = GraphUtils::neighbours(seed, v);

            // Select an unvisited neighbour at random.
            // TODO: Improve randomization.
            const int idx = rand() % nbrs.size();
            const auto nxt = nbrs[idx];

            if (seed.unvisited[nxt]) {
                seed.unvisited[nxt] = false;
                ++visitedCells;
                boost::add_edge(v, nxt, seed.maze);
            }
            v = nxt;
        }
        return seed.maze;
    }
}
