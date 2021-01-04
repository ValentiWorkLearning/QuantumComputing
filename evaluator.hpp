#pragma once

#include "types.hpp"


#include <boost/variant/recursive_wrapper.hpp>
#include <boost/dynamic_bitset.hpp>

#include <functional>
#include <ostream>
#include <string>
#include <optional>


static const char indentstep[] = "    ";

using TBitsetType = boost::dynamic_bitset<>;

struct Evaluator : boost::static_visitor<TBitsetType>
{
    Evaluator(std::ostream& os) : outstream(os){}

    std::ostream& outstream;

    TBitsetType operator()(const var& v) const { return TBitsetType{}; }

    TBitsetType operator()(const binop<AndGate>& b) const
    {
        TBitsetType bitset(4);
        bitset[0] = 1;
        bitset[1] = 0;
        bitset[2] = 0;
        bitset[3] = 0;
        return Evaluate(bitset,b.eval, b.oper1, b.oper2);
    }
    TBitsetType operator()(const binop<OrGate>& b) const
    {
        TBitsetType bitset(4);
        bitset[0] = 1;
        bitset[1] = 1;
        bitset[2] = 1;
        bitset[3] = 0;
        return Evaluate(bitset,b.eval, b.oper1, b.oper2);
    }
    TBitsetType operator()(const binop<XorGate>& b) const
    {
        TBitsetType bitset(4);
        bitset[0] = 0;
        bitset[1] = 1;
        bitset[2] = 1;
        bitset[3] = 0;

        return Evaluate(bitset,b.eval, b.oper1, b.oper2);
    }

    //void print(const std::string& op, const Expression& l, const Expression& r) const
    //{
    //    boost::apply_visitor(Evaluator(outstream, m_intent + indentstep), l);
    //    outstream << m_intent << op << std::endl;
    //    boost::apply_visitor(Evaluator(outstream, m_intent + indentstep), r);
    //}

    TBitsetType Evaluate(const TBitsetType& _qCoverage,TEvaluator eval, const Expression& l, const Expression& r) const
    {
        TBitsetType lQCoverage = boost::apply_visitor(*this, l);
        TBitsetType rQCoverage = boost::apply_visitor(*this, r);

        if (!lQCoverage.empty() && !rQCoverage.empty())
        {
            TBitsetType resultValue;

            std::string lcoverage;
            std::string rcoverage;
            boost::to_string(lQCoverage, lcoverage);
            boost::to_string(rQCoverage, rcoverage);
            
            const std::size_t lSize = lQCoverage.size();
            const std::size_t rSize = rQCoverage.size();

            for (size_t i{}; i< lSize; ++i)
            {
                for (size_t j{}; j< rSize; ++j)
                {
                    bool result = eval(lQCoverage[i], rQCoverage[j]);
                    resultValue.push_back(result);
                }
            }

            std::string coverage;
            boost::to_string(resultValue, coverage);
            outstream << coverage;
            return resultValue;
        }
        return _qCoverage;
    }

    TBitsetType operator()(const unop<InvertorGate>& u) const
    {
        auto qCoverage = ~boost::apply_visitor(*this, u.oper1);
        std::string coverage;
        boost::to_string(qCoverage, coverage);
        outstream <<std::endl << coverage;
        return qCoverage;
    }
};

std::ostream& operator<<(std::ostream& os, const Expression& e)
{
    boost::apply_visitor(Evaluator(os), e); return os;
}