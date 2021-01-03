#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/variant/recursive_wrapper.hpp>

#include "types.hpp"

namespace qi = boost::spirit::qi;
namespace phx = boost::phoenix;

#include <boost/variant/recursive_wrapper.hpp>
#include <ostream>
#include <string>

static const char indentstep[] = "    ";

struct TreeViewDebugger : boost::static_visitor<void>
{
    TreeViewDebugger(std::ostream& os, const std::string& indent = indentstep) : outstream(os), m_intent(indent) {}
    std::ostream& outstream;
    std::string m_intent;

    void operator()(const var& v) const { outstream << m_intent << v << std::endl; }

    void operator()(const binop<AndGate>& b) const { print("0001  ", b.oper1, b.oper2); }
    void operator()(const binop<OrGate>& b) const { print("0111   ", b.oper2, b.oper1); }
    void operator()(const binop<XorGate>& b) const { print("0110  ", b.oper2, b.oper1); }

    void print(const std::string& op, const Expression& l, const Expression& r) const
    {
        boost::apply_visitor(TreeViewDebugger(outstream, m_intent + indentstep), l);
        outstream << m_intent << op << std::endl;
        boost::apply_visitor(TreeViewDebugger(outstream, m_intent + indentstep), r);
    }

    void operator()(const unop<InvertorGate>& u) const
    {
        outstream << m_intent << "!";
        boost::apply_visitor(TreeViewDebugger(outstream, m_intent + indentstep), u.oper1);
    }
};

std::ostream& operator<<(std::ostream& os, const Expression& e)
{
    boost::apply_visitor(TreeViewDebugger(os), e); return os;
}