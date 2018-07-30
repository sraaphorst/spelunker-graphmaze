/**
 * HuntAndKillMazeGenerator.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include "MazeGenerator.h"
#include "MazeGraph.h"

namespace spelunker::graphmaze {

    class HuntAndKillMazeGenerator final : public MazeGenerator {
    public:
        HuntAndKillMazeGenerator() = default;
        ~HuntAndKillMazeGenerator() = default;

        const MazeGraph generate(const MazeGraph &tmplt) const final;
    };
}

