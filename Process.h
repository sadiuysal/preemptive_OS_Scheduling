//
// Created by sadiuysal on 22.11.2018.
//

#ifndef UNTITLED1_PROCESS_H
#define UNTITLED1_PROCESS_H
#include<string>
#include <vector>
#include "Instruction.h"

using namespace std;
class Process {
public:
    Process(const string &name, int priority, const string &code, int arrivalT);

public:
    string name;
    int priority;
    string code;
    int arrivalT;
    int leftinst=0;
    int finishT=0;
    int totalInsExe=0;
    vector<Instruction> insts;

};


#endif //UNTITLED1_PROCESS_H
