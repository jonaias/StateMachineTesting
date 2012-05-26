#include "statemachine.h"
#include <iostream>

StateMachine::StateMachine(QObject *parent) :
    QObject(parent)
{
}

int StateMachine::loadFromFile(QString fileName)
{
    QRegExp rx("^\\s*(\\S+)\\s--\\s(\\S+)\\s/\\s(\\S+)\\s->\\s(\\S+)\\s*$");

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        std::cerr << "Error while opening file \"" << fileName.toStdString() << "\"." << std::endl;
        return 1;
    }

    QTextStream in(&file);
    int i = 1;
    while (!in.atEnd()) {
        QString currLine = in.readLine();

        int pos = rx.indexIn(currLine);
        if (pos == -1) {
            std::cerr << "Syntax error at input line "  << i << "." << std::endl;
            return 1;
        }
        QStringList matched = rx.capturedTexts();

        State initialState = matched[1],
                finalState = matched[4];
        Input input = matched[2];
        Output output = matched[3];
        Transition *t = new Transition(initialState, finalState, input, output);

        if (!stateList.contains(initialState)) {
            stateList.append(initialState);
        }
        if (!stateList.contains(finalState)) {
            stateList.append(finalState);
        }
        if (!inputList.contains(input)) {
            inputList.append(input);
        }
        if (!outputList.contains(output)) {
            outputList.append(output);
        }
        if (!transitionList.contains(t)) {
            transitionList.append(t);
        }

        i++;
    }

    file.close();
    return 0;
}

void StateMachine::addTransition(Transition *transition)
{
    transitionList.append(transition);
    if (!stateList.contains(transition->getInitialState())){
        stateList.append(transition->getInitialState());
    }
    if (!stateList.contains(transition->getFinalState())){
        stateList.append(transition->getFinalState());
    }
    if (!inputList.contains(transition->getInput())){
        inputList.append(transition->getInput());
    }
    if (!outputList.contains(transition->getOutput())){
        outputList.append(transition->getOutput());
    }

}

void StateMachine::printTransitions(){
    Transition *transition;
     foreach (transition, transitionList){
        qDebug() << transition->print();
     }
}

void StateMachine::printStates(){
    qDebug() << stateList;
}

void StateMachine::printInputs(){
    qDebug() << inputList;
}

void StateMachine::printOutput(){
    qDebug() << outputList;
}

QList<Transition*> StateMachine::getTransitionsExiting(State state){
    QList<Transition*> transitionsExiting;
    Transition *transition;
    foreach(transition,transitionList){
        if(transition->getInitialState() == state){
            transitionsExiting.append(transition);
        }
    }
    return transitionsExiting;
}

QList<State> StateMachine::getNextStates(State initialState){
    QList<State> nextStates;
    QList<Transition*> transitionsExiting = getTransitionsExiting(initialState);
    Transition *transition;
    foreach(transition,transitionsExiting){
        nextStates.append(transition->getFinalState());
    }
    return nextStates;
}

State StateMachine::getInitialState(){
    return stateList.first();
}

QList<State> StateMachine::getNextStatesOnInput(State state, Input input) {
    QList<State> ret;
    foreach(Transition *t, transitionList) {
        if (t->getInput() == input && t->getInitialState() == state) {
            if (!ret.contains(t->getFinalState()))
                ret.append(t->getFinalState());
        }
    }
    return ret;
}

QList<Input> StateMachine::getNextStatesOnInputL(QList<State> states, Input input) {
    QList<State> ret;
    foreach (State s, states) {
        QList<State> nextStates = getNextStatesOnInput(s, input);
        foreach(State nextState, nextStates) {
            if (!ret.contains(nextState)) {
                ret.append(nextState);
            }
        }
    }
    return ret;
}

QList<Output> StateMachine::getNextOutputOnInput(State state, Input input) {
    QList<Output> ret;
    foreach(Transition *t, transitionList) {
        if (t->getInput() == input && t->getInitialState() == state) {
            if (!ret.contains(t->getFinalState()))
                ret.append(t->getOutput());
        }
    }
    return ret;
}

QList<Input> StateMachine::getSeparatingSequence(State state1, State state2, QList<QPair<State, State> > *visited){
    QList<Input> result;

    visited->append(QPair<State,State>(state1,state2));
    visited->append(QPair<State,State>(state2,state1));

    Input input;
    foreach(input,inputList){
        State nextState1 = getNextStatesOnInput(state1,input).first();
        State nextState2 = getNextStatesOnInput(state2,input).first();

        if( getNextOutputOnInput(state1,input)!=getNextOutputOnInput(state2,input)){
            result.append(input);
            return result;
        }

        if(!visited->contains(QPair<State,State>(nextState1,nextState2))){
            QList<Input> tmp = getSeparatingSequence(nextState1,nextState2,visited);
            if (!tmp.empty()){
                result.append(input);
                result.append(tmp);
                return result;
            }
        }
    }

    return QList<State>();
}


QList<InputOutput> StateMachine::getReachingSequence( State startState, State endState, QList<State> *explored){
    explored->append(startState);

    QList<InputOutput> result;


    Transition *transition;
    foreach(transition,getTransitionsExiting(startState)){
        if (!explored->contains(transition->getFinalState())){

            if (transition->getFinalState() == endState){
                InputOutput io = {transition->getInput(),transition->getOutput()};
                result.append(io);
                return result;
            }

            QList<InputOutput> temp = getReachingSequence( transition->getFinalState(), endState,  explored);

            /* If getReachingSequence found the endState */
            if (!temp.empty()){
                InputOutput io = {transition->getInput(),transition->getOutput()};
                result.append(io);
                result.append(temp);
                return result;
            }
        }
    }
    return result;
}


QList<InputOutput> StateMachine::getSetSequence(State state){
   QList<InputOutput> result;
   result.append(getResetSequence());
   result.append(getReachingSequence(getInitialState(),state));
   return result;
}

QList<InputOutput> StateMachine::getStatusSequence(State state){

}

QList<InputOutput> StateMachine::getResetSequence(){

}

QList<InputOutput> StateMachine::getTestSequence(){

}
