#include <boost/fit/apply_eval.hpp>

int main() {
    (void)boost::fit::apply_eval(boost::fit::always(), 1, 2);
}