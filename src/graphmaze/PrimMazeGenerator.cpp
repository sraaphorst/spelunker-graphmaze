/**
 * PrimMazeGenerator.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <algorithm>
#include <tuple>
#include <vector>

#include <math/RNG.h>

#include "GraphUtils.h"
#include "MazeGenerator.h"
#include "MazeGraph.h"
#include "PrimMazeGenerator.h"

namespace spelunker::graphmaze {
    std::pair<const MazeGraph, const vertex> PrimMazeGenerator::generate(const MazeGraph &tmplt) const {
        auto seed = GraphUtils::makeSeed(tmplt);

        std::vector<vertex> processing;

        // Begin by selecting a random vertex and mark it for processing.
        // Continue randomly selecting vertices from processing and, if applicable,
        // add one of their unvisited neighbours to the graph by carving a wall.
        // Then add that neighbour to processing.
        const auto start = GraphUtils::randomStartVertex(seed.maze);
        processing.emplace_back(start);
        seed.unvisited[start] = false;

        while (!processing.empty()) {
            // Select a random vertex and check if it has unvisited neighbours.
            const auto elemIdx = math::RNG::randomRange(processing.size());
            const auto v = processing.at(elemIdx);
            const auto unvisitedNeighbours = GraphUtils::unvisitedNeighbours(seed, v);

            // If there are no unvisited neighbours, pop it and continue.
            if (unvisitedNeighbours.empty()) {
                std::swap(processing.at(elemIdx), processing.back());
                processing.pop_back();
                continue;
            }

            const auto u = math::RNG::randomElement(unvisitedNeighbours);
            GraphUtils::addEdge(v, u, seed);
            seed.unvisited[u] = false;
            processing.emplace_back(u);
        }

        return {seed.maze, start};
    }
}