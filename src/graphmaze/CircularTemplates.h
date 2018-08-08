/**
 * CircularTemplates.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include "MazeGraph.h"
#include "RoundTemplates.h"

namespace spelunker::graphmaze {

    class CircularTemplates : RoundTemplates {
    private:
        CircularTemplates() = delete;

    public:
        /**
         * Create a circular maze of radius rings, and uses adaptive subdivision to divide the rings
         * into cells, with the goal of keeping the cell width approximately the same as the cell height.
         * @param radius indicates the number of rings in the circle
         * @return the graph representing this
         */
        static MazeGraph makeCircular(int radius);
    };
}

