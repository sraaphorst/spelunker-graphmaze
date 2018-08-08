/**
 * GridTemplates.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <istream>
#include <tuple>

#include <boost/graph/adjacency_list.hpp>

#include <types/AxialOrientation.h>
#include "MazeGraph.h"
#include "OrthogonalTemplates.h"

#pragma once

namespace spelunker::graphmaze {

    class GridTemplates : public OrthogonalTemplates{
    private:
        GridTemplates() = delete;

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
         * @return the template representing the grid
         */
        static MazeGraph makeGrid(int width, int height,
                                  types::AxialOrientation xorientation,
                                  types::AxialOrientation yorientation);

        /**
         * This is the function that makes the GraphInfo that is unique to grids.
         * @param width the width of the grid
         * @param height the height of the grid
         * @param grm the grid ranker map: for grids, we only have one since the tessellation is made
         *            of unique tiles
         */
        static GraphInfo makeGraphInfo(int width, int height, GridRankerMap &grm);
    };
}

