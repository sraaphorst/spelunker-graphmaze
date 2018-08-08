/**
 * ZetaTemplates.h
 *
 * By Sebastian Raaphorst, 2018.
 *
 * This is code created for zeta mazes, which are grid mazes but with diagonal moves allowed.
 */

#pragma once

namespace spelunker::graphmaze {
    class MazeGraph;

    /**
     * This class is meant to be used as factory methods, and thus instances of this class cannot be created.
     */
    class ZetaTemplates final {
    private:
        ZetaTemplates() = delete;

    public:
        /**
         * Create a zeta maze template representing a grid.
         * @param width width of the grid
         * @param height height of the grid
         * @return the template representing the grid.
         */
        static MazeGraph makeZetaGrid(int width, int height);

        /**
         * Create a zeta maze template representing a torus.
         * This links one of the axes (x) regularly, i.e. there are edges between:
         *   (0, y) and (width - 1, y).
         * @param width the circumference of the cylinder
         * @param height the height of the cylinder
         * @return the template representing the cylinder
         */
        static MazeGraph makeZetaCylinder(int width, int height);

        /**
         * Create a zeta maze template representing a torus.
         * This links both axes regularly, i.e. there are edges between:
         * 1. (0, y) and (width - 1, y).
         * 2. (x, 0) and (x, height - 1).
         * @param width the width of the grid comprising the torus
         * @param height the height of the grid comprising the torus
         * @return the template representing the torus
         */
        static MazeGraph mazeZetaTorus(int width, int height);

        /**
         * Creata a zeta maze template representing a mobius strip.
         * This links one of the axes (x) in reverse, i.e. there are edges between:
         *   (0, y) and (width - 1, height - y - 1).
         * @param width the width of the grid comprising the mobius strip
         * @param height the height of the grid comprising the mobius strip
         * @return the template representing the mobius strip
         */
        static MazeGraph makeZetaMobiusStrip(int width, int height);

        /**
         * Create a zeta maze template representing a Klein bottle.
         * This links one of the axes (x) in reverse, and one (y) regularly,
         * i.e. there are edges between:
         * 1. (0, y) and (width - 1, height - y - 1).
         * 2. (x, 0) and (x, height - 1).
         * @param width the width of the grid comprising the Klein bottle
         * @param height the height of the grid comprising the Klein bottle
         * @return the template representing the Klein bottle
         */
        static MazeGraph makeZetaKleinBottle(int width, int height);

        /**
         * Create a zeta maze template representing a projective plane.
         * This links the axes in reverse, i.e. there are edges between:
         * 1. (0, y) and (width - 1, height - y - 1).
         * 2. (x, 0) and (width - x - 1, height - 1).
         * @param width width of the plane
         * @param height height of the plane
         * @return the template representing the plane
         */
        static MazeGraph makeZetaProjectivePlane(int width, int height);

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
        static MazeGraph makeZetaGridFromMask(int width, int height);
    };
}


