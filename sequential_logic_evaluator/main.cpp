#include <iostream>
#include <vector>
#include <bitset>
#include <map>
#include <fstream>
#include <iterator>
#include <sstream>
#include <filesystem>

using GateInputLines = std::vector<int>;
using TruthTable = std::vector<bool>;
using TReservedTruthTables = std::map<std::size_t, TruthTable>;

struct Parser
{
    void addInput(const bool value)
    {
        m_inputs.push_back(value);
    }

    void addGate(std::string& inputs, std::string& outputs)
    {
        GateInputLines inputsValue;
        for (auto input : inputs)
        {
            inputsValue.push_back(std::atoi(&input));
        }
        m_gatesInputs.push_back(inputsValue);

        TruthTable truthTable;
        for (auto output : outputs)
        {
            truthTable.push_back(std::atoi(&output));
        }

        m_gatesTruthTables.push_back({ truthTable,std::hash<std::string>{}(outputs)});
        m_gatesTruthTablesPool.insert({std::hash<std::string>{}(outputs),truthTable});
    }

    bool getGateValue(const int gateIndex)
    {
        int currentBitValue {1};
        int resultInput {0};

        for (auto input : m_gatesInputs[gateIndex])
        {
            const size_t inputsCount = m_inputs.size();
            if (input > inputsCount)
            {
                resultInput += getGateValue(input - (inputsCount + 1)) * currentBitValue;
            }
            else
            {
                resultInput += m_inputs[input - 1] * currentBitValue;
            }
            currentBitValue *= 2;
        }
        const auto& [thruthTable, gateKind] = m_gatesTruthTables[gateIndex];

        if(thruthTable.empty())
        {
            return m_gatesTruthTablesPool[gateKind][resultInput];
        }
        return m_gatesTruthTables[gateIndex].first[resultInput];
    }

    bool getInput(const int index)
    {
        return m_inputs[index];
    }

    size_t getGatesCount() const
    {
        return m_gatesInputs.size();
    }
    
    std::vector<bool>                               m_inputs;
    std::vector<GateInputLines>                     m_gatesInputs;
    std::vector<std::pair<TruthTable,std::size_t>>  m_gatesTruthTables;
    TReservedTruthTables                            m_gatesTruthTablesPool;
};

int main(int argc, char** argv)
{

    Parser parser;
    std::string inputValue;
    std::string gateInputs;
    std::string gateTruthTable;
    std::string brokeGate;
    std::filesystem::path inputfilePath;

#ifdef USE_ARGV_FILE_PATH
    if(argc != 2 )
    {
        std::cout << " Wrong launch arguments count. Second argument must be file for evalute." << std::endl;
        return - 1;
    }
    inputfilePath = argv[1];
    std::cout << "Completed file path reading. Evaluating ..." << std::endl;
#else
    //std::cin >> inputfilePath;
#endif
    std::fstream infile{inputfilePath};
    std::stringstream sstream;

    if (infile)
    {
        sstream << infile.rdbuf();
        infile.close();
    }

    while (true)
    {
        sstream>> inputValue;
        if (inputValue == "I")
        {
            do
            {
                sstream >> inputValue;
                if (inputValue.size() != 1)
                {
                    std::cout<<"Invalid input for gate"<<std::endl;
                }
            } while (inputValue.size() != 1);

            parser.addInput(inputValue[0] == '1' ? true : false);
        }
        else if (inputValue == "E")
        {
            sstream >> gateInputs;
            sstream >> gateTruthTable;

            parser.addGate(gateInputs, gateTruthTable);
        }
        else if( inputValue == "BrokeGate")
        {
            sstream >> brokeGate;
        }
        else if (inputValue == "End")
        {
            break;
        }
    }

    if( !brokeGate.empty() )
    {
        parser.m_gatesTruthTables[std::atoi(brokeGate.c_str()) - 1].first.clear();
        std::cout<< "Broke gate:" << brokeGate << std::endl;
    }

    for (int i =0; i < parser.getGatesCount(); ++i)
    {
        std::cout<<parser.getGateValue(i);
    }
}