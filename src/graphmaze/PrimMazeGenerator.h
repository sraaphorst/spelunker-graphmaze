/**
 * PrimMazeGenerator.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include "MazeGenerator.h"
#include "MazeGraph.h"

namespace spelunker::graphmaze {
    class PrimMazeGenerator final : public MazeGenerator {
    public:
        PrimMazeGenerator() = default;
        virtual ~PrimMazeGenerator() final = default;

        const MazeGraph generate(const MazeGraph &tmplt) const final;
    };
}



