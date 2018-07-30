/**
 * BinaryTreeMazeGenerator.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <deque>
#include <vector>

#include <math/RNG.h>
#include <types/Exceptions.h>

#include "GraphUtils.h"
#include "MazeGraph.h"
#include "MazeGenerator.h"
#include "BinaryTreeMazeGenerator.h"

namespace spelunker::graphmaze {

    const MazeGraph BinaryTreeMazeGenerator::generate(const MazeGraph &tmplt) const {
        MazeSeed seed = GraphUtils::makeSeed(tmplt);

        // Make sure that we have a binary tree function, which is needed to pick carving directions.
        auto directionFnOpt = GraphUtils::getCandidateFunction(seed);
        if (!directionFnOpt.has_value())
            throw types::UnsupportedMazeGeneration();
        auto directionFn = directionFnOpt.value();

        // Start at vertex 0 and just keep carving, forcing carving into new
        // cells.
        for (vertex v = 0; v < seed.numVertices; ++v) {
            // Get the vertex type.
            const auto vi = boost::get(VertexInfoPropertyTag(), tmplt, v);

            // Get a list of directions in which we can carve and filter that to get a list of unvisited vertices
            // that are candidates.
            auto directions = directionFn(vi.type);
            std::vector<vertex> candidates;

            for (auto [eIter, eEnd] = boost::out_edges(v, tmplt); eIter != eEnd; ++eIter) {
                // If we have already visited the target vertex, ignore.
                const auto target = boost::target(*eIter, tmplt);
                if (!seed.unvisited[target])
                    continue;

                // Check if this edge contains a direction allowed by our binary tree function for this
                // vertex type.
                const auto ei  = boost::get(EdgeInfoPropertyTag(), tmplt, *eIter);
                const auto dir = v == ei.v1 ? ei.d1 : ei.d2;
                if (std::find(directions.cbegin(), directions.cend(), dir) != directions.cend())
                    candidates.emplace_back(target);
            }

            // If there are candidates, pick one and carve.
            if (!candidates.empty()) {
                const auto target = math::RNG::randomElement(candidates);
                GraphUtils::addEdge(v, target, seed);
                seed.unvisited[v] = false;
            }
        }

        return seed.maze;
    }
}