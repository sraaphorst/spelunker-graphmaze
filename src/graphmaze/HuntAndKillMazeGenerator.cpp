/**
 * HuntAndKillMazeGenerator.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <tuple>

#include <math/RNG.h>

#include "GraphUtils.h"
#include "HuntAndKillMazeGenerator.h"
#include "MazeGraph.h"

namespace spelunker::graphmaze {

    std::pair<const MazeGraph, const vertex> HuntAndKillMazeGenerator::generate(const MazeGraph &tmplt) const {
        auto seed = GraphUtils::makeSeed(tmplt);

        // Get a random starting cell.
        const auto start = GraphUtils::randomStartVertex(seed.maze);
        auto v = start;

        // Allow the first iteration to start without a visited neighbour.
        bool firstRun = true;

        while (v < seed.numVertices) {
            // If v is unvisited, add it to the maze through a visited neighbour unless we are on the first
            // iteration, in which case there will be no such neighbour.
            if (seed.unvisited[v] && !firstRun) {
                firstRun = false;

                const auto visitedNbrs = GraphUtils::visitedNeighbours(seed, v);
                if (visitedNbrs.empty()) {
                    ++v;
                    continue;
                }

                const auto visitedNbr = math::RNG::randomElement(visitedNbrs);
                GraphUtils::addEdge(v, visitedNbr, seed);

            } else if (!firstRun) {
                // Otherwise we were at an already processed cell, so advance.
                ++v;
                continue;
            }
            firstRun = false;
            seed.unvisited[v] = false;

            // Continue to carve a random walk until we can no longer do so.
            while (true) {
                // Get the unvisited neighbours of v and pick one.
                const auto unvisitedNbrs = GraphUtils::unvisitedNeighbours(seed, v);
                if (unvisitedNbrs.empty())
                    break;

                const auto unvisitedNbr = math::RNG::randomElement(unvisitedNbrs);
                GraphUtils::addEdge(v, unvisitedNbr, seed);

                v = unvisitedNbr;
                seed.unvisited[v] = false;
            }

            v = 0;
        }

        // Now we have covered all vertices and added them to the maze.
        return {seed.maze, start};
    }
}