/**
 * BFSMazeGenerator.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <boost/graph/adjacency_list.hpp>

#include <stack>

#include "GraphUtils.h"
#include "MazeGraph.h"
#include "MazeGenerator.h"
#include "BFSMazeGenerator.h"

namespace spelunker::graphmaze {

    const MazeGraph BFSMazeGenerator::generate(const MazeGraph &tmplt) const {
        MazeGraph out = GraphUtils::createInitialMaze(tmplt);
        UnvisitedVertices unvisited = GraphUtils::initializeUnvisitedVertices(tmplt);

        std::stack<vertex> stack;
        stack.push(GraphUtils::randomStartVertex(out));
        while (!stack.empty()) {
            const auto start = stack.top();
            unvisited[start] = false;

            // Find the list of unvisited neighbours to start.
            const auto uNbrs = GraphUtils::unvisitedNeighbours(tmplt, unvisited, start);
            if (uNbrs.empty()) {
                stack.pop();
                continue;
            }

            // Select an unvisited neighbour at random.
            // TODO: Improve randomization.
            const int idx = rand() % uNbrs.size();
            const auto nxt = uNbrs[idx];

            // Add the wall from start to nxt to the output maze.
            const auto [e, success] = boost::add_edge(start, nxt, out);

            // Enqueue nxt and loop.
            stack.push(nxt);
        }

        return out;
    }
}