/**
 * SphericalTemplates.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include "MazeGraph.h"
#include "RoundTemplates.h"

namespace spelunker::graphmaze {

    class SphericalTemplates : public RoundTemplates {
    public:
        SphericalTemplates() = delete;

        static MazeGraph makeSpherical(int diameter);
    };
}


