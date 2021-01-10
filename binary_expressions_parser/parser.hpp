#pragma once

// https://stackoverflow.com/questions/8706356/boolean-expression-grammar-parser-in-c/8707598#8707598
// http://coliru.stacked-crooked.com/a/72a05622bd700de3

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/variant/recursive_wrapper.hpp>

#include "types.hpp"

namespace qi = boost::spirit::qi;
namespace phx = boost::phoenix;


template <typename It, typename Skipper = qi::space_type>
struct parser : qi::grammar<It, Expression(), Skipper>
{
    parser() : parser::base_type(expr_)
    {
        using namespace qi;

        expr_ = or_.alias();

        not_ = ("not" > simple)[_val = phx::construct<unop <InvertorGate>>(_1)] | simple[_val = _1];
        or_ = xor_[_val = _1] >> *("or" >> xor_[_val = phx::construct<binop<OrGate>>(_val, _1)]);
        xor_ = and_[_val = _1] >> *("xor" >> and_[_val = phx::construct<binop<XorGate>>(_val, _1)]);
        and_ = not_[_val = _1] >> *("and" >> not_[_val = phx::construct<binop<AndGate>>(_val, _1)]);

        simple = (('(' > expr_ > ')') | var_);
        var_ = qi::lexeme[+alpha];

        BOOST_SPIRIT_DEBUG_NODE(expr_);
        BOOST_SPIRIT_DEBUG_NODE(or_);
        BOOST_SPIRIT_DEBUG_NODE(xor_);
        BOOST_SPIRIT_DEBUG_NODE(and_);
        BOOST_SPIRIT_DEBUG_NODE(not_);
        BOOST_SPIRIT_DEBUG_NODE(simple);
        BOOST_SPIRIT_DEBUG_NODE(var_);
    }

private:
    qi::rule<It, var(), Skipper> var_;
    qi::rule<It, Expression(), Skipper> not_, and_, xor_, or_, simple, expr_;
};