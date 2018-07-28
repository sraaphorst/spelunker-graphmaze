/**
 * GraphUtils.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <iostream>

#include <types/AxialOrientation.h>
#include "MazeGraph.h"

namespace spelunker::graphmaze {

    class GraphUtils final {
    private:
        GraphUtils() = delete;

    public:
        static MazeGraph makeGrid(int width, int height);

        static MazeGraph makeCylinder(int width, int height);

        static MazeGraph makeTorus(int width, int height);

        static MazeGraph makeMobiusStrip(int width, int height);

        static MazeGraph makeKleinBottle(int width, int height);

        static MazeGraph makeProjectivePlane(int width, int height);

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

        /**
         * Given two vertices, add an edge between them in the maze.
         * We do this through a method here because we want to copy the properties as well.
         * @param v1 first vertex in edge
         * @param v2 second vertex in edge
         * @param seed MazeSeed that contains template and final maze
         */
        static void addEdge(vertex v1, vertex v2, MazeSeed &seed);

    private:
        /**
         * This is the function to which we delegate construction of all grid-based objects, namely:
         * 1. A grid (xorientation = DISCONNECTED, yorientation = DISCONNECTED);
         * 2. A cylinder (xorientation = LOOPED, yorientation = DISCONNECTED);
         * 3. A torus (xorientation = LOOPED, yorientation = LOOPED);
         * 4. A mobius strip (xorientation = REVERSE_LOOPED, yorientation = DISCONNECTED);
         * 5. A Klein bottle (xorientation = REVERSE_LOOPED, yorientation = LOOPED); and
         * 6. A projective plane (xorientation = REVERSE_LOOPED, yorientation = REVERSE_LOOPED).
         *
         * See http://math.uchicago.edu/~may/REU2016/REUPapers/Zhang,Y.pdf for more details.
         *
         * @param width the width of the grid
         * @param height the height of the grid
         * @param xorientation the behaviour of rows
         * @param yorientation the behaviour of columns
         * @return a MazeGraph initialized with the appropriate edges and properties
         */
        static MazeGraph makeGrid(int width, int height,
                types::AxialOrientation xorientation, types::AxialOrientation yorientation);

        static MazeGraph createInitialMaze(const MazeGraph &tmplt) noexcept;

        static UnvisitedVertices initializeUnvisitedVertices(const MazeGraph &tmplt) noexcept;

        static int numVertices(const MazeGraph &maze);
    };
}


