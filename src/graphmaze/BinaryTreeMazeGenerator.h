/**
 * BinaryTreeMazeGenerator.h
 *
 * By Sebastian Raaphorst, 2018.
 *
 * This algorithm iterates from vertex 0, trying to carve one new edge per
 * vertex according to a list determined by the function that is part of the
 * graph properties.
 *
 * It has been tested on the following and found to generate mazes:
 * 1. Grids, cylinders, toruses, mobius strips, Klein bottles, and projective planes; and
 * 2. Circles and spheres.
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



