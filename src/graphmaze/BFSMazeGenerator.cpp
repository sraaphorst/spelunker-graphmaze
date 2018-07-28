/**
 * BFSMazeGenerator.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <boost/graph/adjacency_list.hpp>

#include <queue>

#include <math/RNG.h>

#include "GraphUtils.h"
#include "MazeGraph.h"
#include "MazeGenerator.h"
#include "BFSMazeGenerator.h"

namespace spelunker::graphmaze {

    const MazeGraph BFSMazeGenerator::generate(const MazeGraph &tmplt) const {
        MazeSeed seed = GraphUtils::makeSeed(tmplt);

        std::queue<vertex> queue;

        // We begin by picking a random vertex, and then adding all of its neighbours to the queue.
        // Each queue iteration creates a new wall connecting an unvisited vertex to tha maze.
        // Thus, we don't want BFS to make any walls for the first vertex picked, and we don't
        // enqueue it.
        const auto start = GraphUtils::randomStartVertex(seed.maze);
        seed.unvisited[start] = false;

        const auto startNbrs = GraphUtils::neighbours(seed, start);
        for (const auto nbr: startNbrs)
            queue.emplace(nbr);

        while (!queue.empty()) {
            const auto v = queue.front();
            queue.pop();

            if (!seed.unvisited[v])
                continue;
            seed.unvisited[v] = false;

            // Find all its visited neighbours and carve a passage to one of them.
            const auto visitedNbrs = GraphUtils::visitedNeighbours(seed, v);
            const auto visitedNbr = math::RNG::randomElement(visitedNbrs);
            GraphUtils::addEdge(v, visitedNbr, seed);

            // Enqueue all the unvisited neighbours.
            auto unvisitedNeighbours = GraphUtils::unvisitedNeighbours(seed, v);
            math::RNG::shuffle(unvisitedNeighbours);
            for (const auto nbr: unvisitedNeighbours)
                queue.emplace(nbr);
        }

        return seed.maze;
    }
}