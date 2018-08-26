/**
 * BinaryTreeMazeGenerator.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <tuple>
#include <vector>

#include <boost/graph/adjacency_list.hpp>

#include <math/RNG.h>
#include <types/Exceptions.h>

#include "GraphUtils.h"
#include "MazeGraph.h"
#include "MazeGenerator.h"
#include "BinaryTreeMazeGenerator.h"

namespace spelunker::graphmaze {

    std::pair<const MazeGraph, const vertex> BinaryTreeMazeGenerator::generate(const MazeGraph &tmplt) const {
        auto seed = GraphUtils::makeSeed(tmplt);

        // Make sure that we have a binary tree function, which is needed to pick carving directions.
        auto directionFnOpt = GraphUtils::getCandidateFunction(seed.tmplt);
        if (!directionFnOpt.has_value())
            throw types::UnsupportedMazeGeneration();
        auto directionFn = directionFnOpt.value();

        // Start at vertex 0 and just keep carving, forcing carving into new
        // cells.
        for (auto [vPtr, vEnd] = boost::vertices(tmplt); vPtr != vEnd; ++vPtr) {
            // Get the vertex type.
            const auto v = *vPtr;
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

        return {seed.maze, *boost::vertices(seed.maze).first};
    }
}