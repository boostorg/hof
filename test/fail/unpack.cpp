#include <fit/unpack.hpp>

struct foo
{};

namespace fit {

template<>
struct unpack_sequence<foo>
{
    template<class F, class S>
    constexpr static int apply(F&&, S&&)
    {
        return 0;
    }
};
}

int main() {
    fit::unpack(fit::always(1))(foo{});
}