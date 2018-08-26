/**
 * DFSMazeGenerator.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <stack>

#include <math/RNG.h>

#include "GraphUtils.h"
#include "MazeGraph.h"
#include "MazeGenerator.h"
#include "DFSMazeGenerator.h"

namespace spelunker::graphmaze {

    std::pair<const MazeGraph, const vertex> DFSMazeGenerator::generate(const MazeGraph &tmplt) const {
        auto seed = GraphUtils::makeSeed(tmplt);

        std::stack<vertex> stack;
        const auto start = GraphUtils::randomStartVertex(seed.maze);
        stack.push(start);
        while (!stack.empty()) {
            const auto v = stack.top();
            seed.unvisited[v] = false;

            // Find the list of unvisited neighbours to start.
            const auto uNbrs = GraphUtils::unvisitedNeighbours(seed, v);
            if (uNbrs.empty()) {
                stack.pop();
                continue;
            }

            // Select an unvisited neighbour at random.
            const auto u = math::RNG::randomElement(uNbrs);

            // Add the wall from start to nxt to the output maze.
            GraphUtils::addEdge(v, u, seed);

            // Enqueue nxt and loop.
            stack.push(u);
        }

        return {seed.maze, start};
    }
}