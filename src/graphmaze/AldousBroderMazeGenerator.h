/**
 * AldousBroderMazeGenerator.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include "MazeGenerator.h"
#include "MazeGraph.h"

namespace spelunker::graphmaze {

    class AldousBroderMazeGenerator final : public MazeGenerator {
    public:
        AldousBroderMazeGenerator() = default;
        virtual ~AldousBroderMazeGenerator() final = default;

        const MazeGraph generate(const MazeGraph &tmplt) const;
    };
}


