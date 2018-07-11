/**
 * AldousBroderMazeGenerator.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include "MazeGenerator.h"

namespace spelunker::graphmaze {
    class AldousBroderMazeGenerator : public MazeGenerator {
        AldousBroderMazeGenerator(int width, int height);

        const MazeGraph generate(const MazeGraph &tmplt) const final;
    };
}


