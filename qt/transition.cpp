#include "transition.h"

Transition::Transition(State initialState, State finalState, Input input, Output output) :
    initialState(initialState), finalState(finalState), input(input), output(output)
{

}

/* Copy contructor */
Transition::Transition(const Transition& t) {
    Transition(t.initialState, t.finalState, t.input, t.output);
}

int Transition::operator ==(const Transition &other) const
{
    return (initialState==other.initialState) &&
           (finalState==other.finalState) &&
           (input==other.input) &&
            (output==other.output);
}

State Transition::getInitialState()
{
    return initialState;
}

State Transition::getFinalState()
{
    return finalState;
}

Input Transition::getInput()
{
    return input;
}

Output Transition::getOutput()
{
    return output;
}

QString Transition::print() const
{
    return "Initial state: " + initialState + ", Final state:" + finalState +
            ", Input:" + input + ", Output:" + output;
}


