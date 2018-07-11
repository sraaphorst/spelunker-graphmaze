/**
 * MazeGenerator.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#include "MazeGraph.h"

 namespace spelunker::mazegraph {
/**
 * Abstract case for a graph generator.
 * Given a MazeGraph and a subclass of this class, which embodies an
 * essential algorithm, derive a Maze.
 */
  class MazeGenerator {
  public:
      MazeGenerator(int w, int h) : width{w}, height{h} {}

      const MazeGraph generate() const = 0;

  private:
      int width;
      int height;
  }
}
