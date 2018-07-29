/**
 * GraphUtils.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <iostream>
#include <map>
#include <tuple>

#include <types/AxialOrientation.h>
#include "MazeGraph.h"

namespace spelunker::graphmaze {

    class GraphUtils final {
    private:
        GraphUtils() = delete;

    public:
        /**
         * Create a maze template representing a grid.
         * @param width the width of the grid
         * @param height the height of the grid
         * @return the template representing the grid
         */
        static MazeGraph makeGrid(int width, int height);

        /**
         * Create a maze template representing a cylinder.
         * This links one of the axes (x) regularly, i.e. there are edges between:
         *   (0, y) and (width - 1, y).
         * @param width the circumference of the cylinder
         * @param height the height of the cylinder
         * @return the template representing the cylinder
         */
        static MazeGraph makeCylinder(int width, int height);

        /**
         * Create a maze template representing a torus.
         * This links both axes regularly, i.e. there are edges between:
         * 1. (0, y) and (width - 1, y).
         * 2. (x, 0) and (x, height - 1).
         * @param width the width of the grid comprising the torus
         * @param height the height of the grid comprising the torus
         * @return the template representing the torus
         */
        static MazeGraph makeTorus(int width, int height);

        /**
         * Creata a maze template representing a mobius strip.
         * This links one of the axes (x) in reverse, i.e. there are edges between:
         *   (0, y) and (width - 1, height - y - 1).
         * @param width the width of the grid comprising the mobius strip
         * @param height the height of the grid comprising the mobius strip
         * @return the template representing the mobius strip
         */
        static MazeGraph makeMobiusStrip(int width, int height);

        /**
         * Create a maze template representing a Klein bottle.
         * This links one of the axes (x) in reverse, and one (y) regularly,
         * i.e. there are edges between:
         * 1. (0, y) and (width - 1, height - y - 1).
         * 2. (x, 0) and (x, height - 1).
         * @param width the width of the grid comprising the Klein bottle
         * @param height the height of the grid comprising the Klein bottle
         * @return the template representing the Klein bottle
         */
        static MazeGraph makeKleinBottle(int width, int height);

        /**
         * Create a maze template representing a projective plane.
         * This links the axes in reverse, i.e. there are edges between:
         * 1. (0, y) and (width - 1, height - y - 1).
         * 2. (x, 0) and (width - x - 1, height - 1).
         * @param width width of the plane
         * @param height height of the plane
         * @return the template representing the plane
         */
        static MazeGraph makeProjectivePlane(int width, int height);

        /**
         * Create a grid from a mask as read by a stream.
         * Cells should be indicated with anything other than a space,
         * so for example:
         *
         * ##  ##
         *  ####
         * ##  ##
         *
         * would give us a 6x3 maze with 6 holes and 12 cells.
         * @param str the stream containing the mask
         * @return the graph representing this
         */
        static MazeGraph makeGridFromMask(std::istream &str);

        /**
         * Create a circular maze of radius rings, and uses adaptive subdivision to divide the rings
         * into cells, with the goal of keeping the cell width approximately the same as the cell height.
         * @param radius indicates the number of rings in the circle
         * @return the graph representing this
         */
        static MazeGraph makeCircular(int radius);

        /**
         * Create a spherical maze of diameter rings. Note that we use diameter as opposed to radius to allow for
         * an odd number of rings.
         * @param diameter the diameter of the sphere in rows
         * @return the graph representing this
         */
        static MazeGraph makeSpherical(int diameter);

        /**
         * Given a template for a maze, generate the seed for maze generation, which
         * contains all the information required by maze generators to generate complete meazes.
         * @param tmplt the maze template
         * @return the populated seed structure
         */
        static MazeSeed makeSeed(const MazeGraph &tmplt) noexcept;

        /**
         * Select a random starting vertex in a graph.
         * @param maze the graph
         * @return a random vertex in the graph
         */
        static vertex randomStartVertex(const MazeGraph &maze) noexcept;

        /**
         * Get the unvisited neighbours of a vertex v in a maze seed.
         * @param seed the maze seed
         * @param v the vertex
         * @return a collection of the unvisited neighbours of v
         */
        static VertexCollection unvisitedNeighbours(const MazeSeed &seed, const vertex &v);

        /**
         * Get the visited neighbours of a vertex v in a maze seed.
         * @param seed the maze seed
         * @param v the vertex
         * @return a collection of the visited neighbours of v
         */
        static VertexCollection visitedNeighbours(const MazeSeed &seed, const vertex &v);

        /**
         * Get the neighbours of a vertex v in a maze seed.
         * @param seed the maze seed
         * @param v the vertex
         * @return a collection of all neighbours of v
         */
        static VertexCollection neighbours(const MazeSeed &seed, const vertex &v);

        /**
         * Given two vertices, add an edge between them in the maze.
         * We do this through a method here because we want to copy the properties as well.
         * @param v1 first vertex in edge
         * @param v2 second vertex in edge
         * @param seed MazeSeed that contains template and final maze
         */
        static void addEdge(vertex v1, vertex v2, MazeSeed &seed);

        /**
         * A temporary way to output graphs for analysis, simply by listing their edges.
         * @param out the output stream
         * @param graph the graph to output
         */
        static void outputGraph(std::ostream &out, const MazeGraph &graph);

    private:
        /**
         * An auxiliary function used by other neighbours functions.
         * @param seed the maze seed
         * @param v the vertex to examine
         * @param visited true if we want the visited neighbours of v, and false otherwise
         * @return the corresponding neighbours of v
         */
        static VertexCollection nbrs(const MazeSeed &seed, const vertex &v, const bool visited);

        /**
         * For a circular graph, calculate the number of cells in each ring using an adaptive
         * technique that attempts to maintain the cell width and height approximately the same.
         *
         * Note: if we have rings i and i+1 with r_{i} and r_{i+1} cells in each, then
         * each cell in ring i has r_{i+1} / r_{i} children in ring i+1.
         *
         * @param radius the radius of the circle
         * @return vector of number of cells per ring
         */
        static std::vector<int> calculateRingSizes(int radius);

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

        /**
         * Given a template maze, create the initial maze to begin maze generation.
         * This comprises a maze simply consisting of the vertices in the template maze,
         * and no edges, which will be added by the maze generation technique.
         * @param tmplt the template maze
         * @return the initialized maze graph of vertices
         */
        static MazeGraph createInitialMaze(const MazeGraph &tmplt) noexcept;

        /**
         * Given a maze template, create a structure representing whether or not a vertex has been
         * visited. All vertices are initialized as unvisited.
         * @param tmplt the template maze
         * @return an indexable structure over all vertices indicating whether or not they have been visited
         */
        static UnvisitedVertices initializeUnvisitedVertices(const MazeGraph &tmplt) noexcept;

        /**
         * Calculate and return the number of vertices for a MazeGraph.
         * Note that this is linear in the number of vertices: thus, avoid repeated calls.
         * @param maze the maze of interest
         * @return the number of vertices in the maze
         */
        static int numVertices(const MazeGraph &maze);
    };
}


