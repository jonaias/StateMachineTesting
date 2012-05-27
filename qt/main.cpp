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
    stateMachine.loadFromFile("../examples/machine0.txt");
    stateMachine.printTransitions();
    stateMachine.printInputs();
    stateMachine.printOutput();
    stateMachine.printStates();
    QList<InputOutput> list = stateMachine.getReachingSequence("S2","S3");
    InputOutput iterator;
    foreach(iterator,list){
        qDebug() << iterator.input + "/" + iterator.output;
    }

    QList<State> list1 = stateMachine.getSeparatingSequence("S1","S3");
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

    //removeRedundants(h);
    QList<Input> inputs;
    inputs << "b" << "b" << "a" << "b";
    QList<InputOutput> io = stateMachine.getInputOutputSequenceFromInput("1", inputs);

    return 0;
}
