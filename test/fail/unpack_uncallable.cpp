#include <fit/unpack.hpp>

struct foo
{};

namespace fit {

template<>
struct unpack_sequence<foo>
{
    template<class F, class S>
    constexpr static auto apply(F&&, S&& s) FIT_RETURNS(s.bar);
};
}

int main() {
    fit::unpack(fit::always(1))(foo{});
}