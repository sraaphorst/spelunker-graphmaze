/**
 * DFSMazeGenerator.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <boost/graph/adjacency_list.hpp>

#include <stack>

#include <math/RNG.h>

#include "GraphUtils.h"
#include "MazeGraph.h"
#include "MazeGenerator.h"
#include "DFSMazeGenerator.h"

namespace spelunker::graphmaze {

    const MazeGraph DFSMazeGenerator::generate(const MazeGraph &tmplt) const {
        MazeSeed seed = GraphUtils::makeSeed(tmplt);

        std::stack<vertex> stack;
        stack.push(GraphUtils::randomStartVertex(seed.maze));
        while (!stack.empty()) {
            const auto start = stack.top();
            seed.unvisited[start] = false;

            // Find the list of unvisited neighbours to start.
            const auto uNbrs = GraphUtils::unvisitedNeighbours(seed, start);
            if (uNbrs.empty()) {
                stack.pop();
                continue;
            }

            // Select an unvisited neighbour at random.
            const auto nxt = math::RNG::randomElement(uNbrs);

            // Add the wall from start to nxt to the output maze.
            GraphUtils::addEdge(start, nxt, seed);

            // Enqueue nxt and loop.
            stack.push(nxt);
        }

        return seed.maze;
    }
}