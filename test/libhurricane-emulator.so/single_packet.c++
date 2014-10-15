#include "test_main.h++"
#include <emulator/array.h++>

class tile: public hurricane::tile {
private:

public:
    tile(size_t x_posn, size_t y_posn)
        : hurricane::tile(x_posn, y_posn)
        {
        }

public:
    virtual int main(void)
        {
            if ((x_posn() == 0) && (y_posn() == 0)) {
                send(hurricane::direction::SOUTH, 33);
                return 1;
            }

            if ((x_posn() == 0) && (y_posn() == 1)) {
                return recv(hurricane::direction::NORTH);
            }

            return 0;
        }
};

int test_main(void)
{
    auto array = std::make_shared<hurricane::array>(
        8, 8,
        [](size_t x_posn, size_t y_posn)
        {
            return std::make_shared<tile>(x_posn, y_posn);
        });

    for (const auto& pair: array->run()) {
        auto posn_x = pair.first.first;
        auto posn_y = pair.first.second;
        auto ret = pair.second;

        fprintf(stderr, "(%d, %d) -> %d\n",
                (int)posn_x,
                (int)posn_y,
                ret
            );

        if ((posn_x == 0) && (posn_y == 0) && (ret != 1))
            abort();
        if ((posn_x == 0) && (posn_y == 1) && (ret != 33))
            abort();
    }

    return 0;
}
