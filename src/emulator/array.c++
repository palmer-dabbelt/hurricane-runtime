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
    /* Build the list of tiles. */
    std::map<std::pair<size_t, size_t>, tile_ptr> out;
    for (size_t x = 0; x < width; ++x)
        for (size_t y = 0; y < height; ++y)
            out[std::make_pair(x, y)] = func(x, y);

    /* Build the queue array, which stores all the queues for now. */
    std::map<std::pair<size_t, size_t>,
             std::map<enum direction, queue_ptr>> in_queues;
    for (size_t x = 0; x < width; ++x) {
        for (size_t y = 0; y < height; ++y) {
            auto queues = std::map<enum direction, queue_ptr>();
            for (const auto direction: DIRECTIONS)
                queues[direction] = std::make_shared<queue>();
            in_queues[std::make_pair(x, y)] = queues;
        }
    }

    /* Go ahead and wire together the different tiles by connecting
     * their queues. */
    for (size_t x = 0; x < width; ++x) {
        for (size_t y = 0; y < height; ++y) {
            auto tlookup = [&](size_t x, size_t y) -> tile_ptr
                {
                    auto l = out.find(std::make_pair(x, y));
                    if (l == out.end())
                        return nullptr;
                    return l->second;
                };

            auto tile = tlookup(x, y);
            if (tile == NULL) {
                fprintf(stderr, "Unable to find tile at (%d, %d)\n",
                        (int)x, (int)y);
                abort();
            }

            auto qlookup = [&](size_t x, size_t y, enum direction dir) -> queue_ptr
                {
                    auto l = in_queues.find(std::make_pair(x, y));
                    if (l == in_queues.end())
                        return nullptr;

                    auto ll = l->second.find(dir);
                    if (ll == l->second.end())
                        return nullptr;

                    return ll->second;
                };

            auto qout = [&](enum direction dir)
                {
                    size_t dx = 0, dy = 0;
                    enum direction reverse;
                    switch (dir) {
                    case direction::NORTH:
                        dx = 0;
                        dy = -1;
                        reverse = direction::SOUTH;
                        break;
                    case direction::EAST:
                        dx = 1;
                        dy = 0;
                        reverse = direction::WEST;
                        break;
                    case direction::SOUTH:
                        dx = 0;
                        dy = 1;
                        reverse = direction::NORTH;
                        break;
                    case direction::WEST:
                        dx = -1;
                        dy = 0;
                        reverse = direction::EAST;
                        break;
                    }

                    return qlookup(x + dx, y + dy, reverse);
                };

            auto qin = [&](enum direction dir)
                {
                    return qlookup(x, y, dir);
                };

            for (const auto direction: DIRECTIONS) {
                fprintf(stderr, "direction: %d\n", (int)direction);

                if (qin(direction) != NULL)
                    tile->_queues_in[direction] = qin(direction);
                if (qout(direction) != NULL)
                    tile->_queues_out[direction] = qout(direction);
            }
        }
    }
    

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
