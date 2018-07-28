/**
 * BinaryTreeMazeGenerator.h
 *
 * By Sebastian Raaphorst, 2018.
 *
 * This algorithm iterates from vertex 0, trying to carve one new edge per
 * vertex according to a list determined by the function that is part of the
 * graph properties.
 */

#pragma once

#include "MazeGenerator.h"
#include "MazeGraph.h"

namespace spelunker::graphmaze {
    class BinaryTreeMazeGenerator final : public MazeGenerator {
    public:
        BinaryTreeMazeGenerator() = default;
        ~BinaryTreeMazeGenerator() = default;

        const MazeGraph generate(const MazeGraph &tmplt) const;
    };
}



