#include <boost/hof/lazy.hpp>
#include <boost/hof/placeholders.hpp>
#include <boost/hof/rotate.hpp>

int main() {
    auto i = (boost::hof::rotate(boost::hof::_1 - boost::hof::_2) * boost::hof::_1)(3, 6);
    (void)i;
}
