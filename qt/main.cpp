#include <QtCore>
#include "statemachine.h"

/*
bool _isRedundant(QList<Input> input, QList<QList<Input> > inputsList)
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

void removeRedundants(QList<QList<Input> > *h)
{
    QList<QList<Input> >::iterator i = h->begin();
    while(i != h->end()) {
        if (_isRedundant(*i, *h))
            i = h->erase(i);
        else
            i++;
    }
}
*/

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

    QList<Input> i;
    i << "d" << "e";
    QList<QList<Input> > *h = new QList<QList<Input> >;
    QList<Input> i1;
    i1 << "a" << "b" << "c";
    QList<Input> i2;
    i2 << "d" << "e" << "f";
    h->append(i1);
    h->append(i2);
    h->append(i);

    removeRedundants(h);

    return 0;
}
