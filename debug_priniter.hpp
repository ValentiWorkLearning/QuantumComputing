#pragma once

#include "types.hpp"

#include <boost/variant/recursive_wrapper.hpp>
#include <ostream>

struct DebugPrinter : boost::static_visitor<void>
{
    DebugPrinter(std::ostream& os) : m_os(os) {}
    std::ostream& m_os;

    //
    void operator()(const var& v) const { m_os << v; }

    void operator()(const binop<AndGate>& b) const { print(" & ", b.oper1, b.oper2); }
    void operator()(const binop<OrGate>& b) const { print(" | ", b.oper1, b.oper2); }
    void operator()(const binop<XorGate>& b) const { print(" ^ ", b.oper1, b.oper2); }

    void print(const std::string& op, const Expression& l, const Expression& r) const
    {
        m_os << "(";
        boost::apply_visitor(*this, l);
        m_os << op;
        boost::apply_visitor(*this, r);
        m_os << ")";
    }

    void operator()(const unop<InvertorGate>& u) const
    {
        m_os << "(";
        m_os << "!";
        boost::apply_visitor(*this, u.oper1);
        m_os << ")";
    }
};

std::ostream& operator<<(std::ostream& os, const Expression& e)
{
    boost::apply_visitor(DebugPrinter(os), e); return os;
}