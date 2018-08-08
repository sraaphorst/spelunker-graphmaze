/**
 * GraphUtils.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <iostream>
#include <map>
#include <optional>
#include <tuple>

#include <types/AxialOrientation.h>
#include "MazeGraph.h"

namespace spelunker::graphmaze {

    class GraphUtils final {
    public:
        GraphUtils() = delete;

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

        /**
         * Get the candidate function in the graph used by the binary tree and sidewinder
         * algorithms to dictate directions at cells. This is an optional parameter, so
         * none is returned if it does not exist.
         * @param graph the grapg
         * @return the function as an optional
         */
        static const std::optional<BTCandidateFunction> &getCandidateFunction(const MazeGraph &graph);

        /**
         * Get the ranker functions in the graph, used for rendering.
         * The ith ranker function relates to the ith vector type.
         * @param graph the graph
         * @return a collection of the ranker functions
         */
        static const std::vector<GridRankerMap> &getRankerFunctions(const MazeGraph &graph);

        /**
         * Get the entire graph info.
         * @param graph the graph
         * @return the graph info
         */
         static const GraphInfo &getGraphInfo(const MazeGraph &graph);

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


