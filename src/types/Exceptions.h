/**
 * Exceptions.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <stdexcept>
#include <string>

#include <typeclasses/Show.h>

namespace spelunker::types {
    /// The root of all non-STL exceptions throwable by this library.
    class Exception : public std::logic_error {
    protected:
        explicit Exception(const std::string &what) : std::logic_error(what) {}
    };

    /// Thrown if coordinates out of bounds of the maze are attempted to be accessed.
    class OutOfBoundsCoordinates : public Exception {
    public:
        OutOfBoundsCoordinates(const int x, const int y) : Exception{msg({x, y})} {}

    private:
        static std::string msg(const std::pair<int, int> &c) {
            return "Cell "
                   + typeclasses::Show<std::pair<int, int>>::show(c)
                   + " is out of bounds.";
        }
    };

    /// Thrown if the user tries to create a maze with illegal dimensions.
    class IllegalDimensions : public Exception {
    public:
        IllegalDimensions(const int width, const int height) : Exception{msg({width, height})} {}

    private:
        static std::string msg(const std::pair<int, int> &d) {
            return "Dimensions "
                   + typeclasses::Show<std::pair<int, int>>::show(d)
                   + " are not legal.";
        }
    };

    /// Thrown if the user tries to access a vertex that does not exist.
    class VertexDoesNotExist : public Exception {
    public:
        VertexDoesNotExist(unsigned long v) : Exception{msg(v)} {}

    private:
        static std::string msg(const unsigned long v) {
            return "Vertex " + typeclasses::Show<unsigned long>::show(v)
                + " does not exist.";
        }
    };

    /// Thrown if the user tries to apply a maze generation technique on a graph that does not support it.
    class UnsupportedMazeGeneration : public Exception {
    public:
        UnsupportedMazeGeneration() : Exception("Illegal maze generation operation attempted.") {}
    };

    /// Thrown if the user tries to generate a template that is not supported.
    class UnsupportedTemplateGeneration : public Exception {
    public:
        UnsupportedTemplateGeneration() : Exception("Illegal template generation operation attempted.") {}
    };

    /// Thrown if the user tries to invoke a rendering with the wrong type of graph.
    class UnsupportedRendering : public Exception {
    public:
        UnsupportedRendering() : Exception("Illegal rendering operation attempted.") {}
    };
}
