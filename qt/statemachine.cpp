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

        addTransition(t);

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
            if (!ret.contains(t->getFinalState())) // XXX: ESQUISITO ISSO!!!!
                ret.append(t->getOutput());
        }
    }
    return ret;
}

QList<InputOutput> StateMachine::getInputOutputSequenceFromInput(State state, QList<Input> inputList)
{
    QList<InputOutput> result;
    for(int i=0; i<inputList.size(); i++) {
        foreach(Transition *t, transitionList) {
            if(t->getInput() == inputList[i] && t->getInitialState() == state) {
                InputOutput io = {t->getInput(), t->getOutput()};
                result.append(io);
                state = t->getFinalState();
                break;
            }
        }
    }
    return result;
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
   result.append(getStatusSequence(state));
   return result;
}

bool isRedundant(QList<Input> input, QList<QList<Input> > inputsList)
{
    foreach(QList<Input> otherInput, inputsList) {
        if(input.size() < otherInput.size()) {
            bool isPrefix = true;
            for(int i=0; i<input.size() && isPrefix; i++) {
                if(input[i] != otherInput[i])
                    isPrefix = false;
            }
            if (isPrefix) {
                return true;
            }
        }
    }
    return false;
}

QList<QList<Input> > StateMachine::generateHSequence(State state)
{
    /* First of all, we need the list of the states other than "state". */
    QList<State> otherStates = stateList;
    otherStates.removeOne(state);

    /* Then, we find the separation sequence between "state" and all other states. */
    QList<QList<Input> > h;
    foreach(State s, otherStates) {
        QList<Input> currrentSeparationSequence = getSeparatingSequence(state, s);
        if(!h.contains(currrentSeparationSequence))
            h.append(currrentSeparationSequence);
    }

    /* Finally, we remove redundant sequences. */
    QList<QList<Input> >::iterator i = h.begin();
    while(i != h.end()) {
        if (isRedundant(*i, h))
            i = h.erase(i);
        else
            i++;
    }
}

QList<InputOutput> StateMachine::getStatusSequence(State state)
{
    QList<InputOutput> result;
    QList<QList<Input> > hSequence = generateHSequence(state);
    foreach(QList<Input> inputSequence, hSequence) {
        QList<InputOutput> ioSequence = getInputOutputSequenceFromInput(state, inputSequence);
        result.append(getResetSequence() + getReachingSequence(getInitialState(), state) + ioSequence);
    }
    return result;
}

QList<InputOutput> StateMachine::getResetSequence(){
    QList<InputOutput> ret;
    InputOutput io = {"\n", ""};
    ret.append(io);
    return ret;
}

QList<InputOutput> StateMachine::getTestSequence(){

}
