/**
 * AldousBroderMazeGenerator.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <tuple>

#include "MazeGenerator.h"
#include "MazeGraph.h"

namespace spelunker::graphmaze {

    class AldousBroderMazeGenerator final : public MazeGenerator {
    public:
        AldousBroderMazeGenerator() = default;
        virtual ~AldousBroderMazeGenerator() final = default;

        virtual std::pair<const MazeGraph, const vertex> generate(const MazeGraph &tmplt) const final;
    };
}


