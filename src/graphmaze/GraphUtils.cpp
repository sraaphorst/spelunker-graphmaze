/**
 * GraphUtils.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <algorithm>
#include <cmath>
#include <deque>
#include <iostream>
#include <tuple>
#include <vector>

#include <math/DefaultRNG.h>
#include <types/AxialOrientation.h>
#include <types/Exceptions.h>

#include "MazeGraph.h"
#include "GraphUtils.h"

namespace spelunker::graphmaze {

    MazeSeed GraphUtils::makeSeed(const MazeGraph &tmplt) noexcept {
        return MazeSeed {
            tmplt,
            createInitialMaze(tmplt),
            numVertices(tmplt),
            initializeUnvisitedVertices(tmplt)
        };
    }

    vertex GraphUtils::randomStartVertex(const MazeGraph &maze) noexcept {
        return math::DefaultRNG::randomRange(numVertices(maze));
    }

    VertexCollection GraphUtils::unvisitedNeighbours(const MazeSeed &seed, const vertex &v) {
        return nbrs(seed, v, false);
    }

    VertexCollection GraphUtils::visitedNeighbours(const MazeSeed &seed, const vertex &v) {
        return nbrs(seed, v, true);
    }

    VertexCollection GraphUtils::neighbours(const MazeSeed &seed, const vertex &v) {
        VertexCollection neighbours;
        for (auto [eIter, eEnd] = boost::out_edges(v, seed.tmplt); eIter != eEnd; ++eIter) {
            const auto t = boost::target(*eIter, seed.tmplt);
            neighbours.emplace_back(t);
        }
        return neighbours;
    }

    void GraphUtils::addEdge(vertex v1, vertex v2, MazeSeed &seed) {
        // Get the edge properties from the template.
        auto edge = boost::edge(v1, v2, seed.tmplt);
        auto edgeInfo = boost::get(EdgeInfoPropertyTag(), seed.tmplt, edge.first);
        std::cout << "Adding edge: " << edgeInfo.v1 << " " << types::directionShortName(edgeInfo.d1) << " " << edgeInfo.v2 << " " << types::directionShortName(edgeInfo.d2) << std::endl;
        boost::add_edge(v1, v2, edgeInfo, seed.maze);
    }

    void GraphUtils::outputGraph(std::ostream &out, const MazeGraph &graph) {
        for (auto [eIter, eEnd] = boost::edges(graph); eIter != eEnd; ++eIter)
            std::cout << "Edge " << *eIter << std::endl;

        // These are perfect mazes, so we should have |edges| = #vertices - 1.
        const auto v = numVertices(graph);

        auto [ceIter, ceEnd] = boost::edges(graph);
        const auto e = std::distance(ceIter, ceEnd);

        std::cout << v << " vertices, " << e << " edges" << std::endl;
        assert(e == v - 1);
    }

    const std::optional<BTCandidateFunction> &GraphUtils::getCandidateFunction(const MazeGraph &graph) {
        return boost::get_property(graph, GraphInfoPropertyTag()).binaryTreeCandidates;
    }

    const std::vector<GridRankerMap> &GraphUtils::getRankerFunctions(const MazeGraph &graph) {
        return boost::get_property(graph, GraphInfoPropertyTag()).gridRankerMaps;
    }

    const GraphInfo &GraphUtils::getGraphInfo(const MazeGraph &graph) {
        return boost::get_property(graph, GraphInfoPropertyTag());
    }

    VertexCollection GraphUtils::nbrs(const MazeSeed &seed, const vertex &v, const bool visited) {
        VertexCollection vc;
        for (auto [eIter, eEnd] = boost::out_edges(v, seed.tmplt); eIter != eEnd; ++eIter) {
            const vertex t = boost::target(*eIter, seed.tmplt);
            if (seed.unvisited.at(t) != visited)
                vc.emplace_back(t);
        }
        return vc;
    }

    MazeGraph GraphUtils::createInitialMaze(const MazeGraph &tmplt) noexcept {
        MazeGraph out;
        for (auto[vIter, vEnd] = boost::vertices(tmplt); vIter != vEnd; ++vIter)
            boost::add_vertex(out);
        boost::set_property(out, GraphInfoPropertyTag(), boost::get_property(tmplt, GraphInfoPropertyTag()));
        return out;
    }

    UnvisitedVertices GraphUtils::initializeUnvisitedVertices(const MazeGraph &tmplt) noexcept {
        UnvisitedVertices unvisited;
        for (auto[vIter, vEnd] = boost::vertices(tmplt); vIter != vEnd; ++vIter)
            unvisited[*vIter] = true;
        return unvisited;
    }

    int GraphUtils::numVertices(const MazeGraph &maze) {
        auto [vIter, vEnd] = boost::vertices(maze);
        return std::distance(vIter, vEnd);
    }
}