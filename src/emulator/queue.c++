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

#include "queue.h++"
using namespace hurricane;

queue::queue(size_t element_count)
    : _count(element_count),
      _start(0),
      _used(0),
      _data(element_count)
{
}

word queue::deq(void)
{
    std::unique_lock<std::mutex> lock(_lock);
    _signal.wait(lock, [&](){ return _used > 0; });
    auto out = _data[_start];
    _start = (_start + 1) % _count;
    _used--;
    return out;
}

void queue::enq(word d)
{
    std::unique_lock<std::mutex> lock(_lock);
    _signal.wait(lock, [&](){ return _used < _count; });
    auto tail = (_start + _used) % _count;
    _data[tail] = d;
    _used++;
}
