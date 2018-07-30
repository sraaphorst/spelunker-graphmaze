/**
 * SidewinderMazeGenerator.h
 *
 * By Sebastian Raaphorst, 2018.
 *
 * Starting at the first vertex, we form a run of cells over the first direction in the binary
 * tree function, continuing with a certain probability. When that probability is reached (or the
 * run of cells cannot be extended), we break out of the run by picking a cell at random and then
 * carve in the second direction.
 */

#pragma once

#include "MazeGenerator.h"
#include "MazeGraph.h"

namespace spelunker::graphmaze {

    class SidewinderMazeGenerator final : public MazeGenerator {
    public:
        SidewinderMazeGenerator(double probability = 0.5);
        ~SidewinderMazeGenerator() = default;

        const MazeGraph generate(const MazeGraph &tmplt) const;

    private:
        /**
         * The probability to carve east and extend the cell run.
         */
        double probability;
    };
}




