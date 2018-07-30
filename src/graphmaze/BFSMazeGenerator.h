/**
 * BFSMazeGenerator.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include "MazeGenerator.h"
#include "MazeGraph.h"

namespace spelunker::graphmaze {

    class BFSMazeGenerator final : public MazeGenerator{
    public:
        BFSMazeGenerator() = default;
        ~BFSMazeGenerator() = default;

        const MazeGraph generate(const MazeGraph &tmplt) const final;
    };
}


