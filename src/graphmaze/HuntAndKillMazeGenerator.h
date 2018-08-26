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
        virtual ~HuntAndKillMazeGenerator() final = default;

        virtual std::pair<const MazeGraph, const vertex> generate(const MazeGraph &tmplt) const final;
    };
}

