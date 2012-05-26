#include <QtCore>
#include "statemachine.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    StateMachine stateMachine;
    stateMachine.addTransition(new Transition("1","2","b","0"));
    stateMachine.addTransition(new Transition("1","3","a","1"));
    stateMachine.addTransition(new Transition("2","4","b","0"));
    stateMachine.addTransition(new Transition("2","1","a","1"));
    stateMachine.addTransition(new Transition("3","4","b","1"));
    stateMachine.addTransition(new Transition("3","3","a","1"));
    stateMachine.addTransition(new Transition("4","4","b","0"));
    stateMachine.addTransition(new Transition("4","3","a","0"));
    stateMachine.printTransitions();
    stateMachine.getNextStates("2");
    stateMachine.getTransitionsExiting("2");
    stateMachine.printInputs();
    stateMachine.printOutput();
    stateMachine.printStates();
    QList<InputOutput> list = stateMachine.getReachingSequence("2","3");
    InputOutput iterator;
    foreach(iterator,list){
        qDebug() << iterator.input + "/" + iterator.output;
    }

    QList<State> list1 = stateMachine.getSeparatingSequence("1","3");
    State iterator1;
    qDebug() << "Sequencia de separacao:";
    foreach(iterator1,list1){
        qDebug() << iterator1 << " ";
    }

    return a.exec();
}
