#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include <QObject>
#include <QHash>
#include "transition.h"
#include <QDebug>

struct InputOutput{
    Input input;
    Output output;
};


class StateMachine : public QObject
{
    Q_OBJECT
public:
    explicit StateMachine(QObject *parent = 0);
    void addTransition(Transition *transition);
    
    void printTransitions();
    QList<Transition *> getTransitionsExiting(State state);
    QList<State> getNextStates(State initialState);
    State getInitialState();
    QList<State> getSynchronizationSequence(State state);
    QList<Input> getSeparatingSequence(State state1, State state2,
                                       QList< QPair<State, State> > *visited = new QList< QPair<State,State> >());
    QList<InputOutput> getReachingSequence(State startState, State endState,
                                       QList<State> *explored = new QList<State>());
    QList<InputOutput> getSetSequence(State state);
    QList<InputOutput> getStatusSequence(State state);
    QList<InputOutput> getResetSequence();
    QList<InputOutput> getTestSequence();
    void printStates();
    void printInputs();
    void printOutput();
    QList<Output> getNextOutputOnInput(State state, Input input);
signals:
    
public slots:
    
private:
    QList<Transition*> transitionList;
    QList<State> stateList;
    QList<State> inputList;
    QList<State> outputList;

    QList<State> getNextStatesOnInputL(QList<State> states, Input input);
    QList<State> getNextStatesOnInput(State State, Input input);
};

#endif // STATEMACHINE_H
