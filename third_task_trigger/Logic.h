#include <cassert>
#include <vector>
#include <map>
#include <string>
#include <functional>


struct SignalState
{ 
    enum State
    {
        False,
        True,
        Undefined
    };

    SignalState(bool value)
        : m_state(value ? SignalState::State::True : SignalState::State::False)
    {
    }

    SignalState(State value = State::Undefined)
        : m_state(value)
    {
    }

    bool isUndefined() const
    {
        return m_state == State::Undefined;
    }

    operator bool()
    {
        if (m_state == State::True)
        {
            return true;
        }
        else
        if (m_state == State::False)
        {
            return false;
        }
        else
        {
            assert(false);
            return false;
        }
    }

private:
    State m_state;
};

struct SignalLine;

class Obj
{
public:
    virtual void evaluate() = 0;
};


struct SignalLine
{
    SignalLine()
        : m_Value{ SignalState::State::Undefined }
        , m_LinkedObjects{}
    {
    }
    void notifyAboutChange()
    {
        for (auto obj : m_LinkedObjects)
        {
            obj->evaluate();
        }
    }

    template <typename T>
    void setValue(T value, bool notify = true)
    {
        if (!m_Value.isUndefined() && m_Value == value)
        {
            return;
        }

        m_Value = SignalState(value);
        if(notify)
            notifyAboutChange();
    }

    SignalState getValue() const { return m_Value; }

    SignalState m_Value;
    std::vector<Obj*> m_LinkedObjects;
};


struct TTrigger : public Obj
{
    TTrigger(SignalLine& sig, SignalLine& clk, SignalLine& rst, SignalLine& out, SignalLine& invout)
        : m_Signal(sig)
        , m_Clock(clk)
        , m_Reset(rst)
        , m_Out(out)
        , m_InvOut(invout)
    {
    }

    void evaluate() override
    {
        if (m_Reset.getValue()) 
        {
            m_Out.setValue(false); 
            m_InvOut.setValue(true);
        }
        if (m_Clock.getValue()) 
        {
            if (m_Out.getValue() != m_Signal.getValue())
            {
                m_Out.setValue(m_Signal.getValue());
                m_InvOut.setValue(!m_Signal.getValue());
            }
        }
    };

    SignalLine& m_Signal;
    SignalLine& m_Clock;
    SignalLine& m_Reset;

    SignalLine& m_Out;
    SignalLine& m_InvOut;
};

class LogicElement : public Obj
{
public:
    using TLogicGateInput = std::vector<SignalLine*>;
    using TGateFunction = std::function<SignalState(TLogicGateInput&)>;
    LogicElement(TLogicGateInput inputGateLines, SignalLine* output, const TGateFunction&)
    {
    }

    void evaluate() override
    {
        m_output->setValue(m_function(m_input));
    }

private:
    SignalLine* m_output;
    TLogicGateInput m_input;
    TGateFunction m_function;
};

class AndElement : public LogicElement
{
public:

    AndElement(TLogicGateInput inputLines, SignalLine* output)
        : LogicElement(
            inputLines,
            output, 
            [](TLogicGateInput& inputs) -> SignalState
            {
                for (auto* signal : inputs)
                {
                    if (!signal->getValue())
                    {
                        return false;
                    }

                    return true;
                }
                return SignalState::Undefined;
             }
        )
    {}
};

struct RTLScheme
{
    void addObject(Obj* _obj)
    {
        m_objectsPull.push_back(_obj);
    }

    SignalLine* signal(std::string name)
    {
        if( auto It = m_SignalsPull.find(name); It == m_SignalsPull.end())
        {
            auto it = m_SignalsPull.insert({name,new SignalLine()});
            static_assert(std::is_same_v<decltype(it.first->second),SignalLine*>);
            return it.first->second;
        }
        else 
            return It->second;
        assert(false);
        return nullptr;
    }

    std::vector<Obj*> m_objectsPull;
    std::map<std::string, SignalLine*> m_SignalsPull;
};