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
 *
 * It has been shown to fail on some mazes generated from masks.
 */

#pragma once

#include <tuple>

#include "MazeGenerator.h"
#include "MazeGraph.h"

namespace spelunker::graphmaze {

    class BinaryTreeMazeGenerator final : public MazeGenerator {
    public:
        BinaryTreeMazeGenerator() = default;
        virtual ~BinaryTreeMazeGenerator() final = default;

        virtual std::pair<const MazeGraph, const vertex> generate(const MazeGraph &tmplt) const final;
    };
}



