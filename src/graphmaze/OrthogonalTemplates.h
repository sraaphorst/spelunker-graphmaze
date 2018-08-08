/**
 * OrthogonalTemplates.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <istream>
#include <tuple>

#include <types/AxialOrientation.h>

#include "MazeGraph.h"

namespace spelunker::graphmaze {

    /**
     * This class is non-instantiable: it is simply the superclass for all graphs
     * that represent orthogonal grids.
     */
    class OrthogonalTemplates {
    protected:
        OrthogonalTemplates() = delete;

        /**
         * This function creates the vertices and edges of a grid graph from a mask, but omits
         * the GraphInfo.
         * This is so it can be used by other templates that have an underlying masked grid structure,
         * but distinct GraphInfo.
         *
         * @param width the width of the grid
         * @param height the height of the grid
         * @param xorientation the behaviour of rows
         * @param yorientation the behaviour of columns
         * @return a MazeGraph initialized with the appropriate vertices and edges, and the ranker
         */
        static std::tuple<MazeGraph, int, int, GridRankerMap> makeOrthogonalFromMaskGraph(std::istream &str);

        /**
         * This function creates the vertices and edges of a grid graph, but omits the GraphInfo.
         * This is so it can be used by other templates that have an underlying grid structure, but
         * distinct GraphInfo.
         *
         * @param width the width of the grid
         * @param height the height of the grid
         * @param xorientation the behaviour of rows
         * @param yorientation the behaviour of columns
         * @return a MazeGraph initialized with the appropriate vertices and edges, and the ranker
         */
        static std::pair<MazeGraph, GridRankerMap> makeOrthogonalGraph(int width, int height,
                                                                       types::AxialOrientation xorientation,
                                                                       types::AxialOrientation yorientation);

    };
}


