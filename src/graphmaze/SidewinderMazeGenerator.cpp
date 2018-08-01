/**
 * SidewinderMazeGenerator.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <iostream>
#include <deque>
#include <vector>

#include <math/RNG.h>
#include <types/Exceptions.h>

#include "GraphUtils.h"
#include "MazeGraph.h"
#include "MazeGenerator.h"
#include "SidewinderMazeGenerator.h"

namespace spelunker::graphmaze {

    SidewinderMazeGenerator::SidewinderMazeGenerator(double probability)
        : probability{probability} {}

    const MazeGraph SidewinderMazeGenerator::generate(const MazeGraph &tmplt) const {
        MazeSeed seed = GraphUtils::makeSeed(tmplt);

        // Make sure that we have a binary tree function, which is needed to pick carving directions.
        auto directionFnOpt = GraphUtils::getCandidateFunction(tmplt);
        if (!directionFnOpt.has_value())
            throw types::UnsupportedMazeGeneration();
        auto directionFn = directionFnOpt.value();

        // Keep track of the current run of cells.
        std::vector<vertex> run;

        // Start at the first vertex and continue until we reach the end.
        // We proceed as follows: continue to extend the run with the first direction in the
        // directionFn for the previous cell if possible (i.e. the next cell exists) and the
        // probability is true.
        const auto numVertices = boost::num_vertices(seed.maze);
        auto numVerticesCovered = 0;
        for (vertex v = 0; numVerticesCovered < numVertices && v < numVertices;) {
            // Add this vertex to the run.
            ++numVerticesCovered;
            seed.unvisited[v] = false;
            run.emplace_back(v);

            // Now check if we can and want to extend this run, or alternatively,
            // if we MUST extend this run (i.e. none of the other directions are valid).
            const auto vi = boost::get(VertexInfoPropertyTag(), tmplt, v);
            auto dirs = directionFn(vi.type);
            const auto dir = dirs.front();
            dirs.pop_front();

            // Check if any of the other directions are valid.
            // This is a bit complicated. This idea is:
            // Find if there is a direction (other than the first) in the direction list that
            // leads to a valid, unvisited cell. (If not, then the first direction is the only valid
            // direction, and we must follow it: this happens, for example, in the last row of
            // a grid, where we must only carve east since there are no southern cells).
            auto validDirs = std::find_if(dirs.cbegin(), dirs.cend(), [v, &seed](const auto d) {
                auto [eIter, eEnd] = boost::out_edges(v, seed.tmplt);
                return std::find_if(eIter, eEnd, [d, v, &seed](const auto e) {
                   const auto ei = boost::get(EdgeInfoPropertyTag(), seed.tmplt, e);
                   return (ei.v1 == v && ei.d1 == d && seed.unvisited[ei.v2]) ||
                          (ei.v2 == v && ei.d2 == d && seed.unvisited[ei.v1]);
                }) != eEnd;
            }) != dirs.cend();

            if (math::RNG::randomProbability() < probability || !validDirs) {
                // Find the edge - if any - corresponding to this direction.
                auto [eIter, eEnd] = boost::out_edges(v, tmplt);
                auto iter = std::find_if(eIter, eEnd, [v, dir, &seed](auto e) {
                   const auto ei = boost::get(EdgeInfoPropertyTag(), seed.tmplt, e);
                   return (v == ei.v1 && dir == ei.d1 && seed.unvisited[ei.v2]) ||
                          (v == ei.v2 && dir == ei.d2 && seed.unvisited[ei.v1]);
                });
                if (iter != eEnd) {
                    // Carve the edge, and extend.
                    const auto vt = boost::target(*iter, tmplt);
                    GraphUtils::addEdge(v, vt, seed);
                    v = vt;
                    continue;
                }
            }

            // If we reach this point, it's because we either couldn't or didn't want to extend.
            // We thus has to carve another direction to an unvisited vertex.
            // Collect up the unvisited neighbours to which we can carve via the directions
            // available for each vertex in the run.
            std::vector< edge > candidates;
            for (const auto vs: run) {
                // Get the directions for this vertex, omitting the first, and if cells exist
                // in these directions, add the edges to candidates.
                auto vs_dirs = directionFn(vs);
                vs_dirs.pop_front();
                for (auto [eIter, eEnd] = boost::out_edges(vs, tmplt); eIter != eEnd; ++eIter) {
                    if (!seed.unvisited[boost::target(*eIter, tmplt)]) continue;
                    const auto ei = boost::get(EdgeInfoPropertyTag(), tmplt, *eIter);
                    const auto dir = ei.v1 == vs ? ei.d1 : ei.d2;
                    if (std::find(vs_dirs.cbegin(), vs_dirs.cend(), dir) != vs_dirs.cend())
                        candidates.emplace_back(*eIter);
                }
            }

            // If there are no candidates, then we cannot generate any outward vertices here.
            // This will almost certainly result in a graph that is not perfect.
            if (!candidates.empty()) {
                // Now pick a random candidate.
                auto candidate = math::RNG::randomElement(candidates);

                // And carve! We do not, however, mark this vertex as covered.
                auto vs = boost::source(candidate, tmplt);
                auto vt = boost::target(candidate, tmplt);
                GraphUtils::addEdge(vs, vt, seed);
            }

            // Clear the run, advance v, and loop.
            run.clear();
            ++v;
        }
        return seed.maze;
    }
}