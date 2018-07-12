/**
 * AldousBroderMazeGenerator.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include "MazeGenerator.h"

namespace spelunker::graphmaze {
    class AldousBroderMazeGenerator : public MazeGenerator {
    public:
        AldousBroderMazeGenerator() = default;

        const MazeGraph generate(const MazeGraph &tmplt) const final;
    };
}


