#ifndef TRANSITION_H
#define TRANSITION_H

#include <QObject>
#include <QDebug>

typedef QString State;
typedef QString Input;
typedef QString Output;

class Transition
{
public:
    Transition(State initialState, State finalState, Input input, Output output);
    Transition(const Transition &t);
    int operator==(const Transition &other) const;
    State getInitialState();
    State getFinalState();
    Input getInput();
    Output getOutput();
    QString print() const;

private:
    State initialState;
    State finalState;
    Input input;
    Output output;
    
};

#endif // TRANSITION_H
