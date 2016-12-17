#include <fit/decorate.hpp>
#include "test.hpp"

FIT_TEST_CASE()
{
    FIT_TEST_CHECK(fit::decorate(fit::always(1))(fit::always(1))(fit::always(1))(5) == 1);
}

#if FIT_HAS_NOEXCEPT_DEDUCTION
struct copy_throws 
{
    copy_throws() {}
    copy_throws(copy_throws const&) {}
    copy_throws(copy_throws&&) noexcept {}
};

struct no_throw_fo 
{
    void operator()() const noexcept {}
    void operator()(copy_throws) const noexcept {}
};

FIT_TEST_CASE()
{
    static_assert(noexcept(fit::decorate(fit::always(1))(fit::always(1))(fit::always(1))(5)), "noexcept decorator");
    static_assert(!noexcept(fit::decorate(fit::always(1))(fit::always(1))(fit::always(1))(copy_throws{})), "noexcept decorator");
}

#endif