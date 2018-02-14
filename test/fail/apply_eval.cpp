#include <boost/hof/apply_eval.hpp>

int main() {
    (void)boost::hof::apply_eval(boost::hof::always(), 1, 2);
}