#include "types.hpp"
#include "parser.hpp"

//#include "debug_tree_visualize.hpp"
#include "evaluator.hpp"

int main(void)
{

    for (auto& input : std::list<std::string>{
        // From the OP:
        "not((a and b) or (c or d));"
       /* "(a and b) xor ((c and d) or (a and b));",
        "a and b xor (c and d or a and b);",

        /// Simpler tests:
        "a and b;",
        "a or b;",
        "a xor b;",
        "not a;",
        "not a and b;",
        "not (a and b);",
        "a or b or c;",*/
        })
    {
        auto f(std::begin(input)), l(std::end(input));
        parser<decltype(f)> p;

        try
        {
            Expression result;
            bool ok = qi::phrase_parse(f, l, p > ';', qi::space, result);

            if (!ok)
                std::cerr << "invalid input\n";
            else
                std::cout << "result: " << std::endl << result << "\n";

        }
        catch (const qi::expectation_failure<decltype(f)>& e)
        {
            std::cerr << "expectation_failure at '" << std::string(e.first, e.last) << "'\n";
        }

        if (f != l) std::cerr << "unparsed: '" << std::string(f, l) << "'\n";
    }

    return 0;
}
