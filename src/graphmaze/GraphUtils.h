/**
 * GraphUtils.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <iostream>

#include "MazeGraph.h"

namespace spelunker::graphmaze {

    class GraphUtils {
    private:
        GraphUtils() = delete;

    public:
        static MazeGraph makeGrid(int width, int height);

        static MazeGraph makeCylinder(int width, int height);

        static MazeGraph makeTorus(int width, int height);

        /**
         * Create a circular maze of radius rings, and uses adaptive subdivision to divide the rings
         * into cells, with the goal of keeping the cell width approximately the same as the cell height.
         * @param radius indicates the number of rings in the circle
         * @return the graph representing this
         */
        static MazeGraph makeCircular(int radius);

        static MazeSeed makeSeed(const MazeGraph &tmplt) noexcept;

        static void outputGraph(std::ostream &out, const MazeGraph &graph);

        static vertex randomStartVertex(const MazeGraph &maze) noexcept;

        static VertexCollection unvisitedNeighbours(const MazeSeed &seed, const vertex &v);

        static VertexCollection visitedNeighbours(const MazeSeed &seed, const vertex &v);

        static VertexCollection neighbours(const MazeSeed &seed, const vertex &v);

    private:
        static MazeGraph createInitialMaze(const MazeGraph &tmplt) noexcept;

        static UnvisitedVertices initializeUnvisitedVertices(const MazeGraph &tmplt) noexcept;

        static int numVertices(const MazeGraph &maze);
    };
}


