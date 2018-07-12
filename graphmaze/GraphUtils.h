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

        static void outputGraph(std::ostream &out, MazeGraph graph);

        static MazeGraph createInitialMaze(const MazeGraph &tmplt) noexcept;

        static UnvisitedVertices initializeUnvisitedVertices(const MazeGraph &tmplt) noexcept;

        static vertex randomStartVertex(const MazeGraph &maze) noexcept;

        static VertexCollection unvisitedNeighbours(const MazeGraph &tmplt, UnvisitedVertices &unvisited, const vertex &v);

        static VertexCollection neighbours(const MazeGraph &tmplt, const vertex &v);

        static int numCells(const MazeGraph &maze);
    };
}


