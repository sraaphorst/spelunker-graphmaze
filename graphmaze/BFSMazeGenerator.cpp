/**
 * BFSMazeGenerator.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <boost/graph/adjacency_list.hpp>

#include <map>
#include <stack>
#include <vector>

#include "MazeGraph.h"
#include "MazeGenerator.h"
#include "BFSMazeGenerator.h"

namespace spelunker::graphmaze {
    BFSMazeGenerator::BFSMazeGenerator(const int width, const int height)
            : MazeGenerator{width, height} {}

    const MazeGraph BFSMazeGenerator::generate(const MazeGraph &tmplt) const {
        MazeGraph out = createInitialMaze(tmplt);
        UnvisitedVertices unvisited = initializeUnvisitedVertices(tmplt);

        std::stack<vertex> stack;
        stack.push(randomStartVertex(out));
        while (!stack.empty()) {
            const auto start = stack.top();
            unvisited[start] = false;

            // Find the list of unvisited neighbours to start.
            std::vector<vertex> unvisitedNeighbours;
            for (auto [eiter, eEnd] = boost::out_edges(start, tmplt); eiter != eEnd; ++eiter) {
                const auto t = boost::target(*eiter, tmplt);
                if (unvisited[t])
                    unvisitedNeighbours.emplace_back(t);
            }
            if (unvisitedNeighbours.empty()) {
                stack.pop();
                continue;
            }

            // Select an unvisited neighbour at random.
            // TODO: Improve randomization.
            const int idx = rand() % unvisitedNeighbours.size();
            const auto nxt = unvisitedNeighbours[idx];

            // Add the wall from start to nxt to the output maze.
            const auto [e, success] = boost::add_edge(start, nxt, out);

            // Enqueue nxt and loop.
            stack.push(nxt);
        }

        return out;
    }
}