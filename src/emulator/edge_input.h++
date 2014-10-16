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

#ifndef LIBHURRICANE_EMULATOR__EDGE_INPUT_HXX
#define LIBHURRICANE_EMULATOR__EDGE_INPUT_HXX

#include <memory>

namespace hurricane {
    class edge_input;
    typedef std::shared_ptr<edge_input> edge_input_ptr;
}

#include "direction.h++"
#include "queue.h++"
#include "word.h++"
#include <functional>
#include <map>
#include <memory>
#include <thread>

namespace hurricane {
    /* Stores a special program that can produce an input stream. */
    class edge_input {
    private:
        /* The position this tile lives at. */
        const size_t _x_posn;
        const size_t _y_posn;

        /* The thread that shows this code is running. */
        std::thread _main;
        int _main_ret;

        /* FIXME: These shouldn't be public, but the array constructor
         * wants access to them via a special static function... */
    public:
        /* This is essentially the UDN, there's a port that connects
         * this tile to many other tiles. */
        std::map<enum direction, queue_ptr> _queues_out;

    public:
        edge_input(size_t x_posn, size_t y_posn);

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
        static void main_wrapper(edge_input* t);

        /* These functions allow one to enqueue or dequeue a word from
         * one of the network ports.  These will block until data is
         * availiable. */
        void send(enum direction dir, word dat);
    };
}

#endif
