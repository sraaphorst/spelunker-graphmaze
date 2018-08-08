/**
 * UpsilonTemplates.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <types/AxialOrientation.h>

#include "MazeGraph.h"

namespace spelunker::graphmaze {
    class UpsilonTemplates {
    public:
        UpsilonTemplates() = delete;

        /**
         * Create a maze template representing an octagonal grid with diamonds at diagonals to the octagonal cells.
         * There are:
         * 1. width * height octagonal (type 0) cells; and
         * 2. (width - 1) * (height - 1) diamond (type 1) cells.
         * @param width octagonal width of the grid
         * @param height octagonal height of the grid
         * @return the graph representing this
         */
        static MazeGraph makeUpsilonGrid(int width, int height);

        /**
         * Create a maze template representing an octagonal cylinder with diamonds at diagonals to the octagonal cells.
         * There are:
         * 1. width * height octagonal (type 0) cells; and
         * 2. width * (height - 1) diamond (type 1) cells.
         * @param width octagonal circumference of the cylinder
         * @param height octagonal height of the cylinder
         * @return the graph representing this
         */
        static MazeGraph makeUpsilonCylinder(int width, int height);

        /**
         * Create a maze template representing an octagonal torus with diamonds at diagonals to the octagonal cells.
         * There are:
         * 1. width * height octagonal (type 0) cells; and
         * 2. width * height diamond (type 1) cells.
         * @param width octagonal width of the grid comprising the torus
         * @param height octagonal height of the grid comprising the torus
         * @return the graph representing this
         */
        static MazeGraph makeUpsilonTorus(int width, int height);

    protected:
        /**
         * Create a tiling of width by height octagonal tiles. Note that between each 2x2 configuration of octagonal
         * tiles is a diamond tile, so there are tiles connected to octagonal tiles
         * by NE, NW, SE, and SW. Thus, the final graph has:
         * 1. width * height octagonal tiles;
         * 2a. (width - 1) * (height - 1) diamond tiles if disconnected / disconnected;
         * 2b. width * (height - 1) diamond tiles if looped / disconnected;
         * 2c. width * height diamond tiles if looped / looped.
         *
         * Reverse-looping is not supported as it is not clear what the last diamond has as its SE link.
         * Omitting the diamond tiles would be equivalent to a regular grid.
         * @param width the octagonal tile width
         * @param height the octagonal tile height
         * @param xorientation the
         * @return the graph representing this
         */
        static MazeGraph makeUpsilonGrid(int width, int height,
                                         types::AxialOrientation xorientation, types::AxialOrientation yorientation);

    };
}


