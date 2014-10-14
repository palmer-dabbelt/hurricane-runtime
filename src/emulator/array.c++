/*
 * Copyright (C) 2014 Palmer Dabbelt
 *   <palmer.dabbelt@eecs.berkeley.edu>
 *
 * This file is part of dreamer-sim.
 * 
 * dreamer-sim is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * dreamer-sim is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 * 
 * You should have received a copy of the GNU Affero General Public License
 * along with dreamer-sim.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "array.h++"
#include <thread>
using namespace hurricane;

static std::map<std::pair<size_t, size_t>, tile_ptr>
build_tile_map(size_t width, size_t height,
               std::function<tile_ptr(size_t width,
                                      size_t height)> func
    );


array::array(size_t width, size_t height,
             std::function<tile_ptr(size_t width,
                                    size_t height)> func
            )
    : _width(width),
      _height(height),
      _tiles(build_tile_map(width, height, func))
{
}

std::map<std::pair<size_t, size_t>, tile_ptr>
build_tile_map(size_t width, size_t height,
               std::function<tile_ptr(size_t width,
                                      size_t height)> func
    )
{
    std::map<std::pair<size_t, size_t>, tile_ptr> out;

    for (size_t x = 0; x < width; ++x)
        for (size_t y = 0; y < height; ++y)
            out[std::make_pair(x, y)] = func(x, y);

    return out;
}

void array::fork(void)
{
    for (const auto& pair: _tiles) {
        auto tile = pair.second;
        tile->fork();
    }
}

std::map<std::pair<size_t, size_t>, int> array::join(void)
{
    std::map<std::pair<size_t, size_t>, int> out;
    for (const auto& pair: _tiles) {
        auto posn = pair.first;
        auto tile = pair.second;

        out[posn] = tile->join();
    }

    return out;
}
