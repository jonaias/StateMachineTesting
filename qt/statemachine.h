#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include <QtCore>
#include "transition.h"

struct InputOutput{
    Input input;
    Output output;
};


class StateMachine : public QObject
{
    Q_OBJECT
public:
    /*! \brief Creates an empty FSM.
      *
      */
    explicit StateMachine(QObject *parent = 0);

    /*! \brief Loads an FSM from a file.
      * \param fileName A path to the file from which the machine is to be loaded.
      */
    int loadFromFile(QString fileName);

    /*!
      * \brief Adds a transition to the FSM.
      */
    void addTransition(Transition *transition);

    /*! \brief Returns the transitions present in the FSM that
      *     exit a given state.
      * \param state The origin state of the transitions.
      * \return A list of the transitions.
      */
    QList<Transition *> getTransitionsExiting(State state);

    /*! \brief Returns all states that can be reached from a
      *     given state.
      * \return A list of the next states.
      */
    QList<State> getNextStates(State initialState);

    /*! \brief Returns the initial state of this FSM.
      * \return The initial state.
      */
    State getInitialState();

    /*! \brief Calculates the separating sequence between two states.
      * \param state1 One state.
      * \param state2 Other state, different from state1.
      */
    QList<Input> getSeparatingSequence(State state1, State state2,
                                       QList< QPair<State, State> > *visited = new QList< QPair<State,State> >());

    /*! \brief Calculates a sequence of inputs that takes the FSM
      *     from a state to another one.
      * \param startState The initial state.
      * \param endState The final state.
      */
    QList<InputOutput> getReachingSequence(State startState, State endState,
                                       QList<State> *explored = new QList<State>());

    /*! \brief Returns a special reset symbol.
      * \return The reset symbol, given in a list for convenience.
      */
    QList<InputOutput> getResetSequence();

    /*! \brief Calculates a sequence that, no matter which state the implementation is in,
      *     take it to a given state.
      * \param state The final state.
      * \return A sequence of input symbols, followed by theirs respecting expected
      *     output symbol.
      */
    QList<InputOutput> getSetSequence(State state);

    /*! \brief Calculates a sequence that determines whether or not the
      *     implementation is at a given state.

      * \return A sequence of input symbols, followed by theirs respecting expected
      *     output symbol.
      */
    QList<InputOutput> getStatusSequence(State state);

    /*! \brief Returns a sequence that allows testing the correctness of
      *     an implementation of this FSM.
      * \return A sequence of input symbols, followed by theirs respecting expected
      *     output symbol.
      */
    QList<InputOutput> getTestSequence();

    QList<InputOutput> getInputOutputSequenceFromInput(State state, QList<Input> input);

    /*! \brief Returns the expected output of the implementation, assuming
      *     it is in a known state.
      * \param state The assumed current state of the implementation.
      * \param input The given input.
      * \return The expected output, given in a list for convenience.
      */
    QList<Output> getNextOutputOnInput(State state, Input input);

    State getNextStateOnInput(State state, Input input);

    State getFinalStateOnInputs(State state, QList<Input> inputs);

    QList<State> getNextStatesOnInput(QList<State> states, Input input);

    QList<QList<Input> > generateHSequence(State state);

    void printTransitions();
    void printStates();
    void printInputs();
    void printOutput();

    /*! \brief Receives InputOutput list and writes only inputs to file fileName
      * \param ioSequence List of InputOutput.
      * \param fileName File name to write inputs.
      * \return void.
      */
    void writeInputSequenceToFile(QList<InputOutput> ioSequence, QString fileName);
signals:
    
public slots:
    
private:
    QList<Transition*> transitionList;
    QList<State> stateList;
    QList<State> inputList;
    QList<State> outputList;
};

#endif // STATEMACHINE_H
