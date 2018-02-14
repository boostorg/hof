#include <boost/hof/unpack.hpp>

struct foo
{};

namespace boost { namespace hof {

template<>
struct unpack_sequence<foo>
{
    template<class F, class S>
    constexpr static auto apply(F&&, S&& s) BOOST_HOF_RETURNS(s.bar);
};
}

int main() {
    boost::hof::unpack(boost::hof::always(1))(foo{});
}