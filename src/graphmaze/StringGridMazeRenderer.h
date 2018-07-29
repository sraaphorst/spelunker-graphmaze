/**
 * StringGridMazeRenderer.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <ostream>
#include <string>
#include <vector>

#include <types/Direction.h>
#include "MazeGraph.h"

namespace spelunker::graphmaze {
    /// A simple maze renderer to an ostream.
    /**
     * This class takes a w by h Maze and converts it to a string that displays the maze using unicode box
     * characters. The final dimensions of the box output are 2w+1 by h+1.
     */
    class StringGridMazeRenderer final {
    public:
        explicit StringGridMazeRenderer(std::ostream &o);
        ~StringGridMazeRenderer() = default;

        void render(const MazeGraph &m);

    private:
        std::ostream &out;

        /// A quick and dirty extractor to get a wall status from a Maze, returning false for illegal coordinates.
        static bool wall(const MazeGraph &m, int x, int y, int w, int h, types::Direction d);

        /// The characters used in the box form of the maze representation. There has to be a better way to do this.
        static const std::vector<std::string> boxchars;
    };
}





