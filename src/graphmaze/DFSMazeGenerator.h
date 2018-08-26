/**
 * DFSMazeGenerator.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <tuple>

#include "MazeGenerator.h"
#include "MazeGraph.h"

namespace spelunker::graphmaze {

    /**
     * This is a MazeGenerator that works based on a randomized breadth first search.
     */
    class DFSMazeGenerator final : public MazeGenerator {
    public:
        DFSMazeGenerator() = default;
        virtual ~DFSMazeGenerator() final = default;

        virtual std::pair<const MazeGraph, const vertex> generate(const MazeGraph &tmplt) const final;
    };
}
