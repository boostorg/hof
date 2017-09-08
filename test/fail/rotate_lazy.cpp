#include <boost/fit/lazy.hpp>
#include <boost/fit/placeholders.hpp>
#include <boost/fit/rotate.hpp>

int main() {
    auto i = (boost::fit::rotate(boost::fit::_1 - boost::fit::_2) * boost::fit::_1)(3, 6);
    (void)i;
}
