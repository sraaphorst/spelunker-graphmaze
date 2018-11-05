# spelunker_graphmaze

**Status:** In progress, temporarily on hold.

An independent refactor of the https://github.com/sraaphorst/spelunker GraphMaze to make GraphMaze the fundamental maze specification unit, which will allow a much more diverse number of mazes to be built instead of having mazes constrained to grids.

The maze specification is provided by a graph (using the Boost Graph Library), with vertices representing the cells, and edges representing the possible passages. This allows:

1. Mazes over cylinders, cubes, toruses, spheres, and other geometrical shapes;

2. Mazes with polar coordinates;

3. Mazes with non-square cells (e.g. hexagonal cells, triangular cells, or a mix of cells, such as on the surface of a soccer ball);

4. Mazes with masks (e.g. mazes with holes or intricate shapes);

and any other type of maze that can be represented by a graph.

I am keeping this project separate due to the scope of the change. When I am done implementing the basic maze generation algorithms and have the artchitecture developed, I will begin merging it with https://github.com/sraaphorst/spelunker.
