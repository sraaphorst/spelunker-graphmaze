/**
 * RandomizedBFSMazeGenerator.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <boost/graph/adjacency_list.hpp>

#include "MazeGenerator.h"
#include "MazeGraph.h"

namespace spelunker::graphmaze {
    /**
     * This is a MazeGenerator that works based on a randomized breadth first search.
     */
    class BFSMazeGenerator : public MazeGenerator {
    public:
        BFSMazeGenerator(int width, int height);

        const MazeGraph generate(const MazeGraph &tmplt) const final;
    };
}
