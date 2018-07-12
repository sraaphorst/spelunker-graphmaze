/**
 * HuntAndKillMazeGenerator.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <iostream>
#include "GraphUtils.h"
#include "HuntAndKillMazeGenerator.h"
#include "MazeGraph.h"

namespace spelunker::graphmaze {

    const MazeGraph HuntAndKillMazeGenerator::generate(const MazeGraph &tmplt) const {
        MazeSeed seed = GraphUtils::makeSeed(tmplt);

        // Get a random starting cell.
        vertex v = GraphUtils::randomStartVertex(seed.maze);

        // Allow the first iteration to start without a visited neighbour.
        bool firstRun = true;

        while (v < seed.numVertices) {
            // If v is unvisited, add it to the maze through a visited neighbour unless we are on the first
            // iteration, in which case there will be no such neighbour.
            if (seed.unvisited[v] && !firstRun) {
                firstRun = false;

                const auto visitedNbrs = GraphUtils::visitedNeighbours(seed, v);
                std::cout << "1. v=" << v << ", nbrs=" << visitedNbrs.size() << std::endl;
                if (visitedNbrs.empty()) {
                    ++v;
                    continue;
                }

                // TODO: improve randomization.
                const auto visitedNbr = visitedNbrs[rand() % visitedNbrs.size()];
                boost::add_edge(v, visitedNbr, seed.maze);

            } else if (!firstRun) {
                // Otherwise we were at an already processed cell, so advance.
                ++v;
                continue;
            }
            firstRun = false;
            seed.unvisited[v] = false;

            // Continue to carve a random walk until we can no longer do so.
            while (true) {
                std::cout << "2. v=" << v << std::endl;
                // Get the unvisited neighbours of v and pick one.
                const auto unvisitedNbrs = GraphUtils::unvisitedNeighbours(seed, v);
                if (unvisitedNbrs.empty())
                    break;

                // TODO: improve randomization
                const auto unvisitedNbr = unvisitedNbrs[rand() % unvisitedNbrs.size()];
                boost::add_edge(v, unvisitedNbr, seed.maze);

                v = unvisitedNbr;
                seed.unvisited[v] = false;
            }

            v = 0;
        }

        // Now we have covered all vertices and added them to the maze.
        return seed.maze;
    }
}