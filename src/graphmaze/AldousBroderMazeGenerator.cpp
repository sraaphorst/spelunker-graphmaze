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

#include <math/RNG.h>

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
            std::cout << "Visited=" << visitedCells << " of " << seed.numVertices << std::endl;
            // Get all the neighbours of the current cell and move to one at random.
            const auto nbrs = GraphUtils::neighbours(seed, v);

            // Select an unvisited neighbour at random.
            const auto nxt = math::RNG::randomElement(nbrs);

            if (seed.unvisited[nxt]) {
                seed.unvisited[nxt] = false;
                ++visitedCells;
                std::cout << "Making edge: " << v << " -> " << nxt << std::endl;
                GraphUtils::addEdge(v, nxt, seed);
                auto [ei, ee] = boost::edges(seed.maze);
                std::cout << "Now: " << std::distance(ei, ee) << " edges." << std::endl;
            }
            v = nxt;
        }
        return seed.maze;
    }
}
