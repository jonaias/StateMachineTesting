#include <QtCore>
#include "statemachine.h"
#include <iostream>


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QString inputMachineFile;
    QString outputSequenceFile;


    if (argc < 2){
        inputMachineFile = "machine.txt";
    }
    else{
        inputMachineFile = a.arguments().at(1);
    }
    if (argc < 3){
        outputSequenceFile = "sequence.txt";
    }
    else{
        outputSequenceFile = a.arguments().at(2);
    }


    StateMachine stateMachine;
    stateMachine.loadFromFile(inputMachineFile);

    qDebug() << "Printing all transitions";
    stateMachine.printTransitions();

    qDebug() << "Printing all inputs";
    stateMachine.printInputs();

    qDebug() << "Printing all outputs";
    stateMachine.printOutput();

    qDebug() << "Printing all states";
    stateMachine.printStates();

    qDebug() << "Generating test sequence";
    QList<InputOutput> testSequence = stateMachine.getTestSequence();

    qDebug() << "Writing to file";
    stateMachine.writeInputSequenceToFile(testSequence,outputSequenceFile);

    return 0;
}
