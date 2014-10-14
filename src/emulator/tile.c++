/*
 * Copyright (C) 2014 Palmer Dabbelt
 *   <palmer.dabbelt@eecs.berkeley.edu>
 *
 * This file is part of hurricane-runtime.
 * 
 * hurricane-runtime is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * hurricane-runtime is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 * 
 * You should have received a copy of the GNU Affero General Public License
 * along with hurricane-runtime.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "tile.h++"
using namespace hurricane;

tile::tile(size_t x_posn, size_t y_posn)
    : _x_posn(x_posn),
      _y_posn(y_posn)
{
}

void tile::fork(void)
{
    _main = std::thread(main_wrapper, this);
}

int tile::join(void)
{
    _main.join();
    std::atomic_thread_fence(std::memory_order_seq_cst);
    return _main_ret;
}

void tile::main_wrapper(tile* t)
{
    auto ret = t->main();
    t->_main_ret = ret;
    std::atomic_thread_fence(std::memory_order_seq_cst);
}

void tile::send(enum direction dir, word dat)
{
    auto ql = _queues_out.find(dir);
    if (ql == _queues_out.end()) {
        fprintf(stderr, "Attempted to read non-existing queue\n");
        abort();
    }
    auto q = ql->second;

    q->enq(dat);
}

word tile::recv(enum direction dir)
{
    auto ql = _queues_in.find(dir);
    if (ql == _queues_in.end()) {
        fprintf(stderr, "Attempted to read non-existing queue\n");
        abort();
    }
    auto q = ql->second;

    return q->deq();
}
