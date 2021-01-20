#include <iostream>
#include <vector>
#include <bitset>
#include <map>
#include <fstream>
#include <iterator>
#include <sstream>
#include <filesystem>
#include <variant>
#include <cassert>

#include "Logic.h"

int main(int argc, char** argv)
{
    std::string inputValue;
    std::string gateInputs;
    std::string gateTruthTable;
    std::string brokeGate;
    std::filesystem::path inputfilePath;

#ifdef USE_ARGV_FILE_PATH
    if (argc != 2)
    {
        std::cout << "ERROR: Wrong launch arguments count. Second argument must be file for evalute." << std::endl;
        return -1;
    }
    inputfilePath = argv[1];
    if (inputfilePath.empty())
    {
        std::cout << "ERROR: Empty file path." << std::endl;
        return -1;
    }
    std::cout << "Completed file path reading. Evaluating ..." << std::endl;
#else
    //std::cin >> inputfilePath;
#endif
    std::fstream infile{ inputfilePath };
    std::stringstream sstream;

    RTLScheme rtlScheme;

    if (infile)
    {
        sstream << infile.rdbuf();
        infile.close();
    }

    while (true)
    {
        sstream >> inputValue;
        std::string input;
        if (inputValue == "AndGate")
        {
            std::string firstInput;
            std::string secondInput;
            std::string output;
            // First Input
            sstream >> firstInput;
            auto firstInputLine = rtlScheme.signal(firstInput);
            // Second input
            sstream >> secondInput;
            auto secondInputLine = rtlScheme.signal(secondInput);
            // Output
            sstream >> output;
            auto outputLine = rtlScheme.signal(output);

            LogicElement::TLogicGateInput inputs 
            {
                firstInputLine,
                secondInputLine,
            };
            rtlScheme.addObject(new AndElement(inputs, outputLine));
        }
        else if (inputValue == "TTrigger")
        {
            std::string stateInput;
            std::string clockInput;
            std::string resetInput;

            std::string output;
            std::string invOut;

            // state Input
            sstream >> stateInput;
            auto stateInputLine = rtlScheme.signal(stateInput);
            // clock input
            sstream >> clockInput;
            auto clockInputLine = rtlScheme.signal(clockInput);
            // reset input
            sstream >> resetInput;
            auto resetInputLine = rtlScheme.signal(resetInput);

            // Output
            sstream >> output;
            auto outputLine = rtlScheme.signal(output);

            // InvutOutput
            sstream >> invOut;
            auto invOutLine = rtlScheme.signal(invOut);

            rtlScheme.addObject(
                new TTrigger(
                    *stateInputLine,
                    *clockInputLine,
                    *resetInputLine,
                    *outputLine,
                    *invOutLine
                )
            );
        }
        else if (inputValue == "SimulationInput")
        {
            std::cout<<"Enter simulation values for the scheme inputs!" << std::endl;
        }
        else if (inputValue == "LineValue")
        {
            std::string lineName;
            bool gateValue;
            sstream >> lineName;
            sstream >> gateValue;

            static constexpr bool NotifyChange = false;
            rtlScheme.signal(lineName)->setValue(gateValue, NotifyChange);
        }
        else if( inputValue == "SimulateFrom")
        {
            std::string startLine;
            sstream >> startLine;
            if (startLine.empty())
            {
                break;
            }
            rtlScheme.signal(startLine)->setValue(
                !rtlScheme.signal(startLine)->getValue()
            );
            rtlScheme.signal(startLine)->setValue(
                !rtlScheme.signal(startLine)->getValue()
            );
        }


    }

    for (auto It : rtlScheme.m_SignalsPull)
    {
        std::cout<<It.first<<' '<<static_cast<bool>(It.second->getValue())<<std::endl;
    }
}