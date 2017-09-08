#include <boost/fit/unpack.hpp>

struct foo
{};

namespace boost { namespace fit {

template<>
struct unpack_sequence<foo>
{
    template<class F, class S>
    constexpr static auto apply(F&&, S&& s) BOOST_FIT_RETURNS(s.bar);
};
}

int main() {
    boost::fit::unpack(boost::fit::always(1))(foo{});
}