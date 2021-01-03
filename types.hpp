#pragma once

// Boost 1.71 https://github.com/boostorg/proto/pull/19/commits/f7f698089b428b0139601cb8ff52a764eca16c7a
// Fix for boost headers https://stackoverflow.com/questions/60850066/is-there-a-way-to-avoid-error-c2039-value-is-not-a-member-of-boostproto

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/variant/recursive_wrapper.hpp>

namespace qi = boost::spirit::qi;
namespace phx = boost::phoenix;

using var = std::string;
template <typename tag> struct binop;
template <typename tag> struct unop;

struct OrGate {};
struct AndGate {};
struct XorGate{};
struct InvertorGate {};

using Expression =  boost::variant<var,
    boost::recursive_wrapper<unop<InvertorGate>>,
    boost::recursive_wrapper<binop<AndGate> >,
    boost::recursive_wrapper<binop<XorGate> >,
    boost::recursive_wrapper<binop<OrGate> >
>;

template <typename tag> struct binop
{
    explicit binop(const Expression& l, const Expression& r) : oper1(l), oper2(r) { }
    Expression oper1, oper2;
};

template <typename tag> struct unop
{
    explicit unop(const Expression& o) : oper1(o) { }
    Expression oper1;
};