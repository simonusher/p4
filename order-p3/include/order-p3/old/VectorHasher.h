//
// Created by Szymon on 24.11.2018.
//

#pragma once
#include <vector>
#include <cstddef>

class VectorHasher {
public:
    std::size_t operator()(std::vector<int> const& vec) const;
};