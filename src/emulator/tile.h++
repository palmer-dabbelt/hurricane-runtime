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

#ifndef LIBHURRICANE_EMULATOR__TILE_HXX
#define LIBHURRICANE_EMULATOR__TILE_HXX

#include <memory>
#include <thread>
#include <future>

namespace hurricane {
    class tile;
    typedef std::shared_ptr<tile> tile_ptr;
}

#include "array.h++"
#include "direction.h++"
#include "queue.h++"
#include "word.h++"
#include <atomic>
#include <map>
#include <vector>

namespace hurricane {
    /* Stores a single tile, along with the code that needs to execute
     * on that tile. */
    class tile {
    private:
        /* The position this tile lives at. */
        const size_t _x_posn;
        const size_t _y_posn;

        /* The thread that shows this code is running -- note that
         * here I'm using a "future" because I can't seem to otherwise
         * get the return value from a std::thread, which is a bit
         * odd... */
        std::thread _main;
        int _main_ret;

        /* FIXME: These shouldn't be public, but the array constructor
         * wants access to them via a special static function... */
    public:
        /* This is essentially the UDN, there's a port that connects
         * this tile to many other tiles. */
        std::map<enum direction, queue_ptr> _queues_out;
        std::map<enum direction, queue_ptr> _queues_in;

    public:
        tile(size_t x_posn, size_t y_posn);

    public:
        size_t x_posn(void) const { return _x_posn; }
        size_t y_posn(void) const { return _y_posn; }

    public:
        /* Starts (or blocks until return from) the main function on
         * this tile. */
        void fork(void);
        int join(void);

    protected:
        /* The main function for this tile, which begins running with
         * its position stored in registers. */
        virtual int main(void) = 0;
        static void main_wrapper(tile* t);

        /* These functions allow one to enqueue or dequeue a word from
         * one of the network ports.  These will block until data is
         * availiable. */
        void send(enum direction dir, word dat);
        word recv(enum direction dir);
    };
}

#endif
