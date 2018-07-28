/**
 * Exceptions.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <stdexcept>
#include <string>

namespace spelunker::types {
    /// The root of all non-STL exceptions throwable by this library.
    class Exception : public std::logic_error {
    protected:
        explicit Exception(const std::string &what) : std::logic_error(what) {}
    };

     /// Thrown if the user tries to apply a maze generation technique on a graph that does not support it.
      class UnsupportedMazeGeneration : public Exception {
      public:
          UnsupportedMazeGeneration() : Exception("Illegal maze generation operation attempted") {}
      };
}
