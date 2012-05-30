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
        exit(1);
    }

    QTextStream in(&file);
    int i = 1;
    while (!in.atEnd()) {
        QString currLine = in.readLine();

        int pos = rx.indexIn(currLine);
        if (pos == -1) {
            std::cerr << "Syntax error at input line "  << i << "." << std::endl;
            exit(1);
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

void StateMachine::printTransitions()
{
    Transition *transition;
     foreach (transition, transitionList){
        qDebug() << transition->print();
     }
}

void StateMachine::printStates()
{
    qDebug() << stateList;
}

void StateMachine::printInputs()
{
    qDebug() << inputList;
}

void StateMachine::printOutput()
{
    qDebug() << outputList;
}

QList<Transition*> StateMachine::getTransitionsExiting(State state)
{
    QList<Transition*> transitionsExiting;
    Transition *transition;
    foreach(transition,transitionList){
        if(transition->getInitialState() == state){
            transitionsExiting.append(transition);
        }
    }
    return transitionsExiting;
}

QList<State> StateMachine::getNextStates(State initialState)
{
    QList<State> nextStates;
    QList<Transition*> transitionsExiting = getTransitionsExiting(initialState);
    Transition *transition;
    foreach(transition,transitionsExiting){
        nextStates.append(transition->getFinalState());
    }
    return nextStates;
}

State StateMachine::getInitialState()
{
    return stateList.first();
}

State StateMachine::getNextStateOnInput(State state, Input input)
{
    foreach(Transition *t, transitionList) {
        if (t->getInput() == input && t->getInitialState() == state) {
            return t->getFinalState();
        }
    }
    return NULL;
}

QList<Input> StateMachine::getNextStatesOnInput(QList<State> states, Input input)
{
    QList<State> ret;
    foreach (State s, states) {
        State nextState = getNextStateOnInput(s, input);
        ret << nextState;
    }
    return ret;
}

QList<Output> StateMachine::getNextOutputOnInput(State state, Input input)
{
    QList<Output> ret;
    foreach(Transition *t, transitionList) {
        if (t->getInput() == input && t->getInitialState() == state) {
            if (!ret.contains(t->getFinalState()))
                ret.append(t->getOutput());
        }
    }
    return ret;
}

QList<InputOutput> StateMachine::getInputOutputSequenceFromInput(State state, QList<Input> inputs)
{
    QList<InputOutput> result;
    for(int i=0; i<inputs.size(); i++) {
        foreach(Transition *t, transitionList) {
            if(t->getInput() == inputs[i] && t->getInitialState() == state) {
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
        State nextState1 = getNextStateOnInput(state1,input);
        State nextState2 = getNextStateOnInput(state2,input);

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
   return getReachingSequence(getInitialState(),state);
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

    return h;
}

QList<InputOutput> StateMachine::getStatusSequence(State state, QList<InputOutput> reachingSequence)
{
    QList<InputOutput> result;
    QList<QList<Input> > hSequence = generateHSequence(state);
    foreach(QList<Input> inputSequence, hSequence) {
        QList<InputOutput> ioSequence = getInputOutputSequenceFromInput(state, inputSequence);
        result.append(getResetSequence() + reachingSequence + ioSequence);
    }
    return result;
}

QList<InputOutput> StateMachine::getResetSequence()
{
    QList<InputOutput> result;
    InputOutput io = {"\n", ""};
    result.append(io);
    return result;
}

State StateMachine::getFinalStateOnInputs(State state, QList<Input> inputs)
{
    foreach(Input i, inputs) {
        state = getNextStateOnInput(state, i);
    }
    return state;
}

QList<State> StateMachine::getOptimizedEvaluationOrder()
{
    QList<State> result, unexplored;
    unexplored << getInitialState();

    while(!unexplored.isEmpty()) {
        result << unexplored.first();
        QList<Transition *> transitionsExiting = getTransitionsExiting(unexplored.first());
        unexplored.removeFirst();
        foreach(Transition *t, transitionsExiting) {
            if (!result.contains(t->getFinalState()))
                unexplored << t->getFinalState();
        }
    }

    return result;
}

QList<Input> getInputs(QList<InputOutput> io)
{
    QList<Input> result;
    foreach(InputOutput _io, io) {
        result << _io.input;
    }
    return result;
}

QList<InputOutput> StateMachine::getTestSequence()
{
    QList<InputOutput> result, empty; // "empty" is unnecessary; "result" could be used instead,
                                      //    but it makes code clearer.
    QList<State> evaluationOrder = getOptimizedEvaluationOrder();

    qDebug() <<  "Optimized evaluation order: ";
    qDebug() << evaluationOrder;

    qDebug() << "Testing RESET-STATUS";
    qDebug() << "RESET";
    qDebug() << "Status sequence for " << getInitialState() << ":";
    qDebug() << getInputs(getStatusSequence(getInitialState(), empty));
    // RESET will be the first symbol of the status sequence for the initial state.
    // No need to call getResetSequence() before getStatusSequence.
    result << (getStatusSequence(getInitialState(), empty));

    foreach (State s, evaluationOrder) {
        foreach(Transition *t, getTransitionsExiting(s)) {
            qDebug() << ("Testing transition: " + t->print());
            InputOutput currIo = {t->getInput(), t->getOutput()};
            QList<InputOutput> aux; aux << currIo;

            if(t->getInitialState() != this->getInitialState()) {
                QList<InputOutput> setSequence = getSetSequence(t->getInitialState());
                QList<InputOutput> reachingSequence = setSequence; reachingSequence << aux;

                qDebug() << "RESET";
                qDebug() << ("Set sequence for " + t->getInitialState() + ":");
                qDebug() << getInputs(setSequence);
                qDebug() << currIo.input;
                qDebug() << ("Status sequence for " + t->getFinalState() + " coming" + ":");
                qDebug() << getInputs(getStatusSequence(t->getFinalState(), reachingSequence));

                result << (getResetSequence() << reachingSequence << getStatusSequence(t->getFinalState(), reachingSequence));
            } else {
                qDebug() << "RESET";
                qDebug() << currIo.input;
                qDebug() << ("Status sequence for " + t->getFinalState() + ":");
                qDebug() << getInputs(getStatusSequence(t->getFinalState(), aux));

                result << (getResetSequence() << aux << getStatusSequence(t->getFinalState(), aux));
            }
        }
    }
    return result;
}

void StateMachine::writeInputSequenceToFile(QList<InputOutput> ioSequence, QString fileName){
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        std::cerr << "Error while opening file \"" << fileName.toStdString() << "\"." << std::endl;
        exit(1);
    }

    QTextStream out(&file);
    /* Get input list from an input output list */
    QList<Input> inputSequence = getInputs(ioSequence);

    /* i starts in 1 to skip the first reset (simply an ENTER), which, in this case, is unnecessary.*/
    for(int i=1;i<inputSequence.length()-1;i++){
        out << inputSequence[i];
        if (inputSequence[i] == "\n"){
            continue;
        }
        if (inputSequence[i+1] == "\n"){
            continue;
        }
        out << ",";

    }

    out << inputSequence.last();

    out.flush();

    file.close();

}
