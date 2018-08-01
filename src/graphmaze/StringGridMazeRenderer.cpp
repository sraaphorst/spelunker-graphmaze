/**
 * StringGridMazeRenderer.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <boost/graph/adjacency_list.hpp>

#include <ostream>
#include <string>
#include <vector>

#include <types/Exceptions.h>
#include <types/Tessellations.h>

#include "GraphUtils.h"
#include "MazeGraph.h"
#include "StringGridMazeRenderer.h"

namespace spelunker::graphmaze {
    const std::vector<std::string> StringGridMazeRenderer::boxchars = { " ","╶","╷","┌","╴","─","┐","┬","╵","└","│","├","┘","┴","┤","┼" };

    StringGridMazeRenderer::StringGridMazeRenderer(std::ostream &o) : out(o) {}

    bool StringGridMazeRenderer::wall(const MazeGraph &m, int x, int y, int w, int h, types::Direction d) {
        const GraphInfo &info = GraphUtils::getGraphInfo(m);

        if (x < 0 || x >= w) return false;
        if (y < 0 || y >= h) return false;

        const auto &ranker  = info.gridRankerMaps.front();
        const auto iter = ranker.find({x,y});
        if (iter == ranker.end()) return false;

        // Check all the out edges to see which one matches d, if any.
        const auto v = iter->second;
        for (auto [eIter, eEnd] = boost::out_edges(v, m); eIter != eEnd; ++eIter) {
            const auto props = boost::get(EdgeInfoPropertyTag(), m, *eIter);
            if ((props.v1 == v && props.d1 == d) || (props.v2 == v && props.d2 == d))
                return false;
        }

        return true;
    }

    void StringGridMazeRenderer::render(const MazeGraph &m) {
        /**
         * First, we have to convert the Maze from an x by y grid into an (x+1) by (y+1) box drawing.
         * To determine cell (x,y) in the box drawing, we need the following cells from the Maze:
         * (x-1,y-1), (x-1,y), (x,y-1), (x,y)
         *
         * Define two functions:
         * B(x,y,d) = true iff direction d is part of the symbol for box coordinate (x,y).
         * M(x,y,d) = true iff (x,y) is in bounds and cell (x,y) has a wall at position d.
         *
         * Then we define B(x,y,d) in terms of M(x,y,d) as follows:
         * 1. B(x,y,N) = M(x-1, y-1, E) || M(  x, y-1, W)
         * 2. B(x,y,W) = M(x-1, y-1, S) || M(x-1,   y, N)
         * 3. B(x,y,S) = M(x-1, y,   E) || M(  x,   y, W)
         * 4. B(x,y,E) = M(  x, y-1, S) || M(  x,   y, N)
         */
//        const auto &rankers = GraphUtils::getRankerFunctions(m);
//        const auto &ranker  = rankers.front();
//
//        // Calculate the width and height.
//        // TODO: Change to the graph properties.
//        int w = 0;
//        int h = 0;
//        for (auto iter = ranker.begin(); iter != ranker.end(); ++iter) {
//            const auto [ix, iy] = iter->first;
//            if (ix > w) w = ix;
//            if (iy > h) h = iy;
//        }
//        ++w;
//        ++h;
        const GraphInfo &info = GraphUtils::getGraphInfo(m);
        if (info.type != types::TessellationType::GRID)
            throw types::UnsupportedRendering();
        const int w = info.width.value();
        const int h = info.height.value();

        // Create the box representation of the maze.
        using BoxEntry = std::vector<bool>;
        using BoxColumn = std::vector<BoxEntry>;
        using BoxRepresentation = std::vector<BoxColumn>;

        BoxEntry          bDir(4, false);
        BoxColumn         bCol(h+1, bDir);
        BoxRepresentation box(w+1, bCol);

        const auto N = 0;
        const auto E = 1;
        const auto S = 2;
        const auto W = 3;


        for (auto x=0; x <= w; ++x) {
            for (auto y=0; y <= h; ++y) {
                box[x][y][N] = wall(m, x-1, y-1, w, h, types::Direction::EAST)  || wall(m,   x, y-1, w, h, types::Direction::WEST);
                box[x][y][W] = wall(m, x-1, y-1, w, h, types::Direction::SOUTH) || wall(m, x-1,   y, w, h, types::Direction::NORTH);
                box[x][y][S] = wall(m, x-1,   y, w, h, types::Direction::EAST)  || wall(m,   x,   y, w, h, types::Direction::WEST);
                box[x][y][E] = wall(m,   x, y-1, w, h, types::Direction::SOUTH) || wall(m,   x,   y, w, h, types::Direction::NORTH);
            }
        }

        // Now we are ready to render.
        for (auto y=0; y <= h; ++y) {
            for (auto x=0; x <= w; ++x) {
                const auto n = box[x][y][N] ? 1 : 0;
                const auto w = box[x][y][W] ? 1 : 0;
                const auto s = box[x][y][S] ? 1 : 0;
                const auto e = box[x][y][E] ? 1 : 0;

                const auto idx = n * 8 + w * 4 + s * 2 + e;
                out << boxchars[idx] << boxchars[e == 0 ? 0 : 5];
            }
            out << std::endl;
        }
    }
}