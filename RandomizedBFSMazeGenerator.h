/**
 * RandomizedBFSMazeGenerator.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <boost/adjacency_list.hpp>
#include <map>
#include <cstdlib>
#include <stack>
#include <vector>

#include "MazeGenerator.h"
#include "MazeGraph.h"

namespace spelunker::mazegraph {
    class RandomizedBFSMazeGenerator : public MazeGenerator {
    public:
        BinaryMazeGenerator(int width, int height)
            : MazeGenerator(width, height) {}

        const MazeGraph generate(const MazeGraph &template) const {
                // Create a visitor for each vertex to indicate if it's
                // been visited.
                using vertex = template::vertex_type_descriptor;
                const auto vertices = boost::vertices(template);

                MazeGraph out;
                std::map<vertex, bool> unvisited;
                for (const auto &v: vertices) {
                    unvisited[v] = true;
                    boost::add_vertex(out);
                }

                std::stack<vertex> stk;
                stk.push(boost::vertices(template).head());
                while (!stack.empty()) {
                        const start = stack.top();
                        stack.pop();
                        unvisited[start] = false;

                        // Find the list of unvisited neighbours.
                        std::vector<vertex> unvisitedNeighbours;
                        for (auto [eiter, eEnd] = boost::out_edges(t, template));
                                eiter != eEnd; ++eiter) {
                            const auto t = boost::target(*eiter, template);
                            if (unvisited[t])
                                unvisitedNeighbours.emplace_back(t);
                        }

                        // Select one at random.
                }
        }
    }
};
