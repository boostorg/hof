/*=============================================================================
    Copyright (c) 2014 Paul Fultz II
    builder.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_BUILDER_H
#define FIT_GUARD_BUILDER_H

#include <fit/conditional.h>
#include <fit/pipable.h>
#include <fit/fix.h>
#include <fit/detail/pp.h>
#include <boost/preprocessor.hpp>
#include <boost/preprocessor/facilities/is_empty.hpp>

#define FIT_PP_TRUE(...) 1
#define FIT_PP_FALSE(...) 0

//
// FIT_PP_IS_EMPTY will expands to 1 if the parameter is empty, otherwise
// it expands to 0. This will work even if the parameter given is a set
// of parenthesis.
//
#define FIT_PP_IS_EMPTY(x) BOOST_PP_CAT(FIT_PP_DETAIL_IS_EMPTY_, FIT_PP_IS_PAREN(x))(x)
#define FIT_PP_DETAIL_IS_EMPTY_0(x) BOOST_PP_IS_EMPTY(x)
#define FIT_PP_DETAIL_IS_EMPTY_1(x) 0 

//
// FIT_PP_HEAD retrieves the first element of a sequence.
// Example:
//
//     FIT_PP_HEAD((1)(2)(3)) // Expands to (1)
//
#define FIT_PP_HEAD(x) FIT_PP_PICK_HEAD(FIT_PP_MARK x)
#define FIT_PP_MARK(...) (__VA_ARGS__), 
#define FIT_PP_PICK_HEAD(...) FIT_PP_PICK_HEAD_I(__VA_ARGS__,)
#define FIT_PP_PICK_HEAD_I(x, ...) x


//
// FIT_PP_TAIL retrieves the tail of a sequence.
// Example:
//
//     FIT_PP_TAIL((1)(2)(3)) // Expands to (2)(3)
//
#define FIT_PP_TAIL(x) FIT_PP_EAT x

//
// FIT_PP_ARGS_TO_SEQ converts args to a seq
//
#define FIT_PP_ARGS_TO_SEQ(...) FIT_PP_DETAIL_ARGS_TO_SEQ(FIT_PP_NARGS(__VA_ARGS__), (__VA_ARGS__))
#define FIT_PP_DETAIL_ARGS_TO_SEQ(size, tuple) BOOST_PP_TUPLE_TO_SEQ(size, tuple)

#define FIT_PP_SEQ_IS_SINGLE(x) FIT_PP_IS_1(BOOST_PP_SEQ_SIZE(x))

#define FIT_PP_TYPEOF(x) FIT_PP_DETAIL_TYPEOF(FIT_PP_DETAIL_TYPEOF_PROBE x,)
#define FIT_PP_DETAIL_TYPEOF(...) FIT_PP_DETAIL_TYPEOF_HEAD(__VA_ARGS__)
#define FIT_PP_DETAIL_TYPEOF_HEAD(x, ...) FIT_PP_REM x
#define FIT_PP_DETAIL_TYPEOF_PROBE(...) (__VA_ARGS__),

#define FIT_PP_STRIP(x) FIT_PP_EAT x

#define FIT_PP_PAIR(x) FIT_PP_REM x

//
// FIT_PP_STRING_TOKEN
//
#define FIT_PP_STRING_TOKEN(x) BOOST_PP_IIF(FIT_PP_IS_PAREN(x), FIT_PP_STRING_TOKEN_PAREN, FIT_PP_STRING_TOKEN_KEYWORD)(x)
#define FIT_PP_STRING_TOKEN_KEYWORD(x) FIT_PP_STRING_TOKEN_KEYWORD_CHECK(BOOST_PP_CAT(FIT_PP_STRING_, x), x)
#define FIT_PP_STRING_TOKEN_KEYWORD_CHECK(tokened, raw) BOOST_PP_IIF(FIT_PP_IS_PAREN(tokened), tokened, (raw))
#define FIT_PP_STRING_TOKEN_PAREN(x) (FIT_PP_HEAD(x)) FIT_PP_TAIL(x) 

//
// FIT_PP_STRING_TO_SEQ converts a string into a preprocessor sequence
// 
#define FIT_PP_STRING_TO_SEQ(x) FIT_PP_STRING_TO_SEQ_WHILE_M \
( \
    BOOST_PP_WHILE(FIT_PP_STRING_TO_SEQ_WHILE_P, FIT_PP_STRING_TO_SEQ_WHILE_O, (,x)) \
)

#define FIT_PP_STRING_TO_SEQ_WHILE_P(r, state) FIT_PP_STRING_TO_SEQ_P state
#define FIT_PP_STRING_TO_SEQ_WHILE_O(r, state) FIT_PP_STRING_TO_SEQ_O state
#define FIT_PP_STRING_TO_SEQ_WHILE_M(state) FIT_PP_STRING_TO_SEQ_M state

#define FIT_PP_STRING_TO_SEQ_P(prev, tail) BOOST_PP_NOT(FIT_PP_IS_EMPTY(tail))
#define FIT_PP_STRING_TO_SEQ_O(prev, tail)  \
   FIT_PP_STRING_TO_SEQ_REPLACE(prev, FIT_PP_STRING_TOKEN(tail))

#define FIT_PP_STRING_TO_SEQ_REPLACE(prev, tail) \
    (prev FIT_PP_HEAD(tail), FIT_PP_TAIL(tail))

#define FIT_PP_STRING_TO_SEQ_M(prev, tail) prev

//
// FIT_PP_SEQ_TO_STRING outputs the sequence
//
// TODO: Use sequence iteration
#define FIT_PP_SEQ_TO_STRING(seq) BOOST_PP_SEQ_FOR_EACH(FIT_PP_SEQ_TO_STRING_EACH, ~, seq) 
#define FIT_PP_SEQ_TO_STRING_EACH(r, data, x) x 

//
// FIT_PP_SEQ_SPLIT
//
#define FIT_PP_SEQ_SPLIT(seq, pred, data) FIT_DETAIL_PP_SEQ_SPLIT_FOLD_LEFT_M(BOOST_PP_SEQ_FOLD_LEFT(FIT_DETAIL_PP_SEQ_SPLIT_FOLD_LEFT_O, (pred, data,,), seq))
#define FIT_DETAIL_PP_SEQ_SPLIT_FOLD_LEFT_O(s, state, x) FIT_DETAIL_PP_SEQ_SPLIT_FOLD_LEFT_INVOKE((s, x, FIT_PP_REM state))
#define FIT_DETAIL_PP_SEQ_SPLIT_FOLD_LEFT_INVOKE(x) FIT_DETAIL_PP_SEQ_SPLIT_OP x
#define FIT_DETAIL_PP_SEQ_SPLIT_OP(s, x, pred, data, seq, elem) \
    BOOST_PP_IF(pred(s, data, x), FIT_DETAIL_PP_SEQ_SPLIT_OP_TRUE, FIT_DETAIL_PP_SEQ_SPLIT_OP_FALSE)(x, pred, data, seq, elem)
#define FIT_DETAIL_PP_SEQ_SPLIT_OP_TRUE(x, pred, data, seq, elem) BOOST_PP_IIF(FIT_PP_IS_PAREN(elem), \
    (pred, data, seq(elem),),\
    (pred, data, seq,))
#define FIT_DETAIL_PP_SEQ_SPLIT_OP_FALSE(x, pred, data, seq, elem) (pred, data, seq, elem (x))
#define FIT_DETAIL_PP_SEQ_SPLIT_FOLD_LEFT_M(state) FIT_DETAIL_PP_SEQ_SPLIT_M state
#define FIT_DETAIL_PP_SEQ_SPLIT_M(pred, data, seq, elem) seq BOOST_PP_IIF(FIT_PP_IS_PAREN(elem), (elem),)


//
// FIT_PP_EQUAL
//
#define FIT_PP_IS_COMPARABLE(x) BOOST_PP_IIF(FIT_PP_IS_PAREN(x), FIT_PP_FALSE, FIT_PP_PRIMITIVE_IS_COMPARABLE)(x)
#define FIT_PP_PRIMITIVE_IS_COMPARABLE(x) FIT_PP_IS_PAREN( FIT_PP_COMPARE_ ## x (()) )
#define FIT_PP_PRIMITIVE_COMPARE(x, y) FIT_PP_IS_PAREN \
( \
FIT_PP_COMPARE_ ## x ( FIT_PP_COMPARE_ ## y) ((unused)) \
)
#define FIT_PP_NOT_EQUAL(x, y) \
BOOST_PP_IIF(BOOST_PP_BITAND( FIT_PP_IS_COMPARABLE(x), FIT_PP_IS_COMPARABLE(y) ), \
FIT_PP_PRIMITIVE_COMPARE, \
FIT_PP_TRUE \
)(x, y)
#define FIT_PP_EQUAL(x, y) BOOST_PP_COMPL(FIT_PP_NOT_EQUAL(x, y))



#ifndef FIT_PP_STRING_if
#define FIT_PP_STRING_if (if)
#endif
#ifndef FIT_PP_STRING_else
#define FIT_PP_STRING_else (else)
#endif
#ifndef FIT_PP_COMPARE_else
#define FIT_PP_COMPARE_else(x) x
#endif

#define FIT_FUNCTION_PARAM(x) BOOST_PP_CAT(FIT_FUNCTION_PARAM_, x)
#define FIT_FUNCTION_PARAM_const (const)
#define FIT_FUNCTION_PARAM_auto (())

#define FIT_FUNCTION_PARAM_IF_PARAM(x, t, f, data) FIT_DETAIL_FUNCTION_IF_PARAM(FIT_FUNCTION_PARAM(x), x, t, f, data)
#define FIT_DETAIL_FUNCTION_IF_PARAM(x, y, t, f, data) BOOST_PP_IIF(FIT_PP_IS_PAREN(x), t, f)(x, y, data)

#define FIT_FUNCTION_PARAM_REPLACE_ID() FIT_FUNCTION_PARAM_REPLACE

#define FIT_FUNCTION_PARAM_REPLACE(x, T) FIT_FUNCTION_PARAM_IF_PARAM(x, FIT_DETAIL_FUNCTION_PARAM_REPLACE_TRUE, FIT_DETAIL_FUNCTION_PARAM_REPLACE_FALSE, T)
#define FIT_DETAIL_FUNCTION_PARAM_REPLACE_TRUE(x, y, T) FIT_DETAIL_FUNCTION_PARAM_REPLACE_TRUE_I(FIT_PP_TYPEOF(x), FIT_PP_STRIP(x), T)
#define FIT_DETAIL_FUNCTION_PARAM_REPLACE_TRUE_I(head, tail, T) \
    BOOST_PP_IIF(FIT_PP_IS_PAREN(head), FIT_DETAIL_FUNCTION_PARAM_REPLACE_END, FIT_DETAIL_FUNCTION_PARAM_REPLACE_CONT)(head, tail, T)
#define FIT_DETAIL_FUNCTION_PARAM_REPLACE_END(head, tail, T) T tail
#define FIT_DETAIL_FUNCTION_PARAM_REPLACE_CONT(head, tail, T) head FIT_PP_DEFER(FIT_FUNCTION_PARAM_REPLACE_ID)()(tail, T)
#define FIT_DETAIL_FUNCTION_PARAM_REPLACE_FALSE(x, y, T) y

#define FIT_FUNCTION_PARAM_TEMPLATE_ID() FIT_FUNCTION_PARAM_TEMPLATE

#define FIT_FUNCTION_PARAM_TEMPLATE(x, T) FIT_FUNCTION_PARAM_IF_PARAM(x, FIT_DETAIL_FUNCTION_PARAM_TEMPLATE_TRUE, FIT_DETAIL_FUNCTION_PARAM_TEMPLATE_FALSE, T)
#define FIT_DETAIL_FUNCTION_PARAM_TEMPLATE_TRUE(x, y, T) FIT_DETAIL_FUNCTION_PARAM_TEMPLATE_TRUE_I(FIT_PP_TYPEOF(x), FIT_PP_STRIP(x), T)
#define FIT_DETAIL_FUNCTION_PARAM_TEMPLATE_TRUE_I(head, tail, T) \
    BOOST_PP_IIF(FIT_PP_IS_PAREN(head), FIT_DETAIL_FUNCTION_PARAM_TEMPLATE_END, FIT_DETAIL_FUNCTION_PARAM_TEMPLATE_CONT)(head, tail, T)
#define FIT_DETAIL_FUNCTION_PARAM_TEMPLATE_END(head, tail, T) (class T)
#define FIT_DETAIL_FUNCTION_PARAM_TEMPLATE_CONT(head, tail, T) FIT_PP_DEFER(FIT_FUNCTION_PARAM_TEMPLATE_ID)()(tail, T)
#define FIT_DETAIL_FUNCTION_PARAM_TEMPLATE_FALSE(x, y, T)

#define FIT_PP_SEQ_FOR_EACH_ID() BOOST_PP_SEQ_FOR_EACH_I
#define FIT_PP_SEQ_FOR_EACH_I_R_ID() BOOST_PP_SEQ_FOR_EACH_I_R

#define FIT_FUNCTION_PARAM_TEMPLATE_ARGS(r, seq, T) BOOST_PP_SEQ_FOR_EACH_I_R(r, FIT_FUNCTION_PARAM_TEMPLATE_ARGS_EACH, T, seq) 
#define FIT_FUNCTION_PARAM_TEMPLATE_ARGS_EACH(r, T, i, elem) FIT_FUNCTION_PARAM_TEMPLATE(elem, T ## i)

#define FIT_FUNCTION_PARAM_REPLACE_AUTO(r, seq, T) BOOST_PP_SEQ_FOR_EACH_I_R(r, FIT_FUNCTION_PARAM_REPLACE_AUTO_EACH, T, seq) 
#define FIT_FUNCTION_PARAM_REPLACE_AUTO_EACH(r, T, i, elem) (FIT_FUNCTION_PARAM_REPLACE(elem, T ## i))


#define FIT_PARAM_REQUIRES(reqs) typename std::enable_if<(decltype reqs ()), int>::type = 0
#define FIT_DETAIL_FUNCTION_CLASS_K_REQ(reqs) , FIT_PARAM_REQUIRES(reqs)
#define FIT_DETAIL_FUNCTION_CLASS_K(name, n, t_params, f_params, reqs, body) \
struct name \
{ \
    template<BOOST_PP_SEQ_ENUM(t_params)> \
    decltype(auto) \
    operator()(BOOST_PP_SEQ_ENUM(f_params) \
        BOOST_PP_IIF(FIT_PP_IS_PAREN(reqs), FIT_DETAIL_FUNCTION_CLASS_K_REQ, FIT_PP_EAT)(reqs) \
            ) const \
    { return body; } \
};

// TODO: Add support for bodies without double parenthesis, so you can write:
// FIT_FUNCTIONS_CLASS((foo)(x, y) x + y)
#define FIT_DETAIL_FUNCTION_CLASS_GEN(name, n, t_params, f_params, body) \
BOOST_PP_CAT(FIT_DETAIL_FUNCTION_CLASS_GEN_, BOOST_PP_SEQ_SIZE(body))\
(name, n, t_params, f_params, body)
#define FIT_DETAIL_FUNCTION_CLASS_GEN_1(name, n, t_params, f_params, body) \
FIT_DETAIL_FUNCTION_CLASS_K(name, n, t_params, f_params,, BOOST_PP_SEQ_HEAD(body))
#define FIT_DETAIL_FUNCTION_CLASS_GEN_3(name, n, t_params, f_params, body) \
FIT_DETAIL_FUNCTION_CLASS_K(name, n, t_params, f_params, BOOST_PP_SEQ_ELEM(1, body), BOOST_PP_SEQ_ELEM(2, body))

//
// Conditional adaptor
//
#define FIT_DETAIL_FUNCTION_CLASS_CONDITIONAL_EACH(r, data, i, x) FIT_DETAIL_FUNCTION_CLASS_INVOKE((i, FIT_PP_REM data, x))
#define FIT_DETAIL_FUNCTION_CLASS_INVOKE(x) FIT_DETAIL_FUNCTION_CLASS_OP x
#define FIT_DETAIL_FUNCTION_CLASS_OP(i, name, n, t_params, f_params, body) \
FIT_DETAIL_FUNCTION_CLASS_GEN(FIT_DETAIL_FUNCTION_CLASS_CONDITIONAL_NAME(i, name), n, t_params, f_params, body)

#define FIT_DETAIL_FUNCTION_CLASS_CONDITIONAL_NAME(i, name) BOOST_PP_CAT(fit_888_, BOOST_PP_CAT(i, name))
#define FIT_DETAIL_FUNCTION_CLASS_CONDITIONAL_NAMES(n, name) BOOST_PP_ENUM(n, FIT_DETAIL_FUNCTION_CLASS_CONDITIONAL_NAMES_EACH, name)
#define FIT_DETAIL_FUNCTION_CLASS_CONDITIONAL_NAMES_EACH(z, i, name) FIT_DETAIL_FUNCTION_CLASS_CONDITIONAL_NAME(i, name)

#define FIT_DETAIL_FUNCTION_CLASS_CONDITIONAL_PARSE_ARGS(r, self, params) \
    (class Fit_8Self)FIT_FUNCTION_PARAM_TEMPLATE_ARGS(r, params, Fit_8T), (Fit_8Self self)FIT_FUNCTION_PARAM_REPLACE_AUTO(r, params, Fit_8T)

#define FIT_DETAIL_FUNCTION_CLASS_CONDITIONAL_ALL(r, self,  name, n, params, bodies) \
FIT_PP_DEFER(FIT_PP_SEQ_FOR_EACH_I_R_ID)()(r, FIT_DETAIL_FUNCTION_CLASS_CONDITIONAL_EACH, \
    (name, n, FIT_DETAIL_FUNCTION_CLASS_CONDITIONAL_PARSE_ARGS(r, self, params)), \
    bodies) \
    typedef fit::fix_adaptor<fit::conditional_adaptor<FIT_DETAIL_FUNCTION_CLASS_CONDITIONAL_NAMES(BOOST_PP_SEQ_SIZE(bodies), name) > > name; 

#define FIT_DETAIL_FUNCTION_CLASS_CONDITIONAL_SINGLE_INVOKE(x) FIT_DETAIL_FUNCTION_CLASS_GEN x

#define FIT_DETAIL_FUNCTION_CLASS_CONDITIONAL_SINGLE(r, self, name, n, params, body) \
    FIT_DETAIL_FUNCTION_CLASS_CONDITIONAL_SINGLE_INVOKE((FIT_DETAIL_FUNCTION_CLASS_CONDITIONAL_NAME(0, name), n, FIT_DETAIL_FUNCTION_CLASS_CONDITIONAL_PARSE_ARGS(r, self, params), FIT_PP_REM body)) \
    typedef fit::fix_adaptor<FIT_DETAIL_FUNCTION_CLASS_CONDITIONAL_NAME(0, name) > name; 

#define FIT_DETAIL_FUNCTION_CLASS_CONDITIONAL(r, self, name, params, bodies) BOOST_PP_IIF(FIT_PP_SEQ_IS_SINGLE(bodies), \
                                                                    FIT_DETAIL_FUNCTION_CLASS_CONDITIONAL_SINGLE, \
                                                                    FIT_DETAIL_FUNCTION_CLASS_CONDITIONAL_ALL)\
                                                                    (r, self, name, FIT_PP_NARGS params, FIT_PP_ARGS_TO_SEQ params, bodies)

#define FIT_DETAIL_FUNCTION_CLASS_CONDITIONAL_P(s, data, x) FIT_PP_EQUAL(x, else)
#define FIT_DETAIL_FUNCTION_CLASS_CONDITIONAL_SPLIT(seq) FIT_PP_SEQ_SPLIT(seq, FIT_DETAIL_FUNCTION_CLASS_CONDITIONAL_P, data) 


#define FIT_DETAIL_FUNCTION_CLASS_NAME(seq) FIT_DETAIL_FUNCTION_CLASS_NAME_I(BOOST_PP_SEQ_ELEM(0, seq))
#define FIT_DETAIL_FUNCTION_CLASS_NAME_I(x) FIT_PP_REM x

// #define FIT_DETAIL_FUNCTION_CLASS_TRANSFORM_TAIL(self, name, seq) TRANSFORM_TAIL(self, name, seq)
#define FIT_DETAIL_FUNCTION_CLASS_TRANSFORM_TAIL(self, name, seq) \
FIT_DETAIL_FUNCTION_CLASS_CONDITIONAL(1, self, name, BOOST_PP_SEQ_HEAD(seq), FIT_DETAIL_FUNCTION_CLASS_CONDITIONAL_SPLIT(BOOST_PP_SEQ_TAIL(seq)))

#define FIT_DETAIL_FUNCTION_CLASS_TRANSFORM_WITH_NAME(name, seq) FIT_DETAIL_FUNCTION_CLASS_TRANSFORM_TAIL(FIT_DETAIL_FUNCTION_CLASS_NAME(seq), name, BOOST_PP_SEQ_TAIL(seq))
#define FIT_DETAIL_FUNCTION_CLASS_TRANSFORM_WITHOUT_NAME(seq) FIT_DETAIL_FUNCTION_CLASS_TRANSFORM_TAIL(FIT_DETAIL_FUNCTION_CLASS_NAME(seq), FIT_DETAIL_FUNCTION_CLASS_NAME(seq), BOOST_PP_SEQ_TAIL(seq))
// #define FIT_DETAIL_FUNCTION_CLASS_TRANSFORM_WITHOUT_NAME(seq) TRANSFORM_WITHOUT_NAME(seq)


// Apply additional scans
#define FIT_DETAIL_FUNCTION_CLASS_XX(...) __VA_ARGS__
#define FIT_DETAIL_FUNCTION_CLASS_X(...) FIT_DETAIL_FUNCTION_CLASS_XX(FIT_DETAIL_FUNCTION_CLASS_XX(__VA_ARGS__))

// class
#define FIT_FUNCTION_CLASS(x) FIT_DETAIL_FUNCTION_CLASS_X(FIT_DETAIL_FUNCTION_CLASS_TRANSFORM_WITHOUT_NAME(FIT_PP_STRING_TO_SEQ(x)))
#define FIT_FUNCTION_CLASS_NAME(name, x) FIT_DETAIL_FUNCTION_CLASS_X(FIT_DETAIL_FUNCTION_CLASS_TRANSFORM_WITH_NAME(name, FIT_PP_STRING_TO_SEQ(x)))

// function object
#define FIT_DETAIL_FUNCTION_OBJECT(name, x) FIT_FUNCTION_CLASS_NAME(BOOST_PP_CAT(fit_class_, name), x) const fit::static_<BOOST_PP_CAT(fit_class_, name)> name = {};
#define FIT_FUNCTION_OBJECT(x) FIT_DETAIL_FUNCTION_OBJECT(BOOST_PP_SEQ_HEAD(x), x)

// pipe
#define FIT_DETAIL_FUNCTION_PIPE_OBJECT(name, x) FIT_FUNCTION_CLASS_NAME(BOOST_PP_CAT(fit_class_, name), x) fit::static_<fit::pipable_adaptor<BOOST_PP_CAT(fit_class_, name) > > name = {};
#define FIT_FUNCTION_PIPE_OBJECT(x) FIT_DETAIL_FUNCTION_PIPE_OBJECT(BOOST_PP_SEQ_HEAD(x), x)

#define FIT_FUNCTION_KEYWORD_class (FIT_FUNCTION_CLASS)

#define FIT_FUNCTION_KEYWORD_pipe (FIT_FUNCTION_PIPE_OBJECT)

// Register with ZLANG
#define ZLANG_fit_function (FIT_FUNCTION)

#define FIT_FUNCTION_INVOKE(m, args) m args
#define FIT_FUNCTION_II(x) FIT_FUNCTION_INVOKE(BOOST_PP_SEQ_HEAD(x), (BOOST_PP_SEQ_TAIL(x)))
#define FIT_FUNCTION_I(x) FIT_FUNCTION_II(BOOST_PP_CAT(FIT_FUNCTION_KEYWORD_, x))
#define FIT_FUNCTION(x) BOOST_PP_IIF(FIT_PP_IS_PAREN(x), FIT_FUNCTION_OBJECT, FIT_FUNCTION_I)(x)

#endif