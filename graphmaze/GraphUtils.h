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
        static MazeGraph makeTorus(const int width, const int height);

        static MazeSeed makeSeed(const MazeGraph &tmplt) noexcept;

        static void outputGraph(std::ostream &out, MazeGraph graph);

        static vertex randomStartVertex(const MazeGraph &maze) noexcept;

        static VertexCollection unvisitedNeighbours(const MazeSeed &seed, const vertex &v);

        static VertexCollection neighbours(const MazeSeed &seed, const vertex &v);

    private:
        static MazeGraph createInitialMaze(const MazeGraph &tmplt) noexcept;

        static UnvisitedVertices initializeUnvisitedVertices(const MazeGraph &tmplt) noexcept;

        static int numVertices(const MazeGraph &maze);
    };
}


