/**
 * RoundTemplates.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <vector>

namespace spelunker::graphmaze {
    class RoundTemplates {
    public:
        RoundTemplates() = delete;
    protected:
        static std::vector<int> calculateRingSizes(int radius);
    };
}

