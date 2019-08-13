#include <iostream>
#include <fstream>
#include <vector>
#include "Process.h"
#include <sstream>
#include <queue>


using namespace std;


int main () {
    string line;
    ifstream myfile ("definition.txt");//file path
    string name;//process name
    string insName;//instruction name
    int exeT;//exe time
    int priority;
    string code; //code name
    int arrivalT; //arrival time
    int currTime=0;//current time

    struct CustomCompare//priority comparator
    {
        bool operator()(const Process& lhs, const Process& rhs)
        {
            if(lhs.priority==rhs.priority){
                return lhs.arrivalT > rhs.arrivalT;
            }else{
                return lhs.priority > rhs.priority;
            }

        }
    };
    struct CustomCompare1//arrival time comparator
    {
        bool operator()(const Process& lhs, const Process& rhs)
        {
            return lhs.arrivalT > rhs.arrivalT;
        }
    };



    priority_queue<Process,vector<Process>, CustomCompare > readyQueue ;//ready queue
    priority_queue<Process,vector<Process>, CustomCompare1 > processQ;//process queue

    if (myfile.is_open())
    {
        while ( getline (myfile,line) )
        {
            string lineP;

            int countW=0;
            //cout << line << '\n';
            istringstream buf(line);
            for(string word; buf >> word; ) {//reading input line by line
                if(countW==0) {
                    name = word;
                }else if(countW==1) {
                    priority = stoi( word);
                }else if(countW==2) {
                    code = word;
                }else {
                    arrivalT = stoi( word);
                }
                countW++;

            }
            Process x=Process(name,priority,code,arrivalT);//creating process instance
            string y=x.code+".txt";//code file path
            ifstream myfileP (y);
            if(myfileP.is_open()){
                while(getline(myfileP,lineP)){ //reads code file and creates instruction vector
                    int countW2=0;

                    istringstream buf(lineP);
                    for(string word; buf >> word; ) {
                        if(countW2==0) {
                            insName = word;
                        }else {
                            exeT = stoi( word);
                        }
                        countW2++;

                    }
                    Instruction xy=Instruction(insName,exeT);//instruction instance
                    x.insts.push_back(xy);//push it to instruction vector
                }
            }

            processQ.push(x);

        }

        myfile.close();
    }
    ofstream out ("output.txt");
    if (out.is_open())
    {
        if(processQ.top().arrivalT!=0){//print
            out << currTime<<":HEAD-";
            out << "-TAIL"<<"\n";
        }


        if(!processQ.empty()){//checks any process exists
            priority_queue<Process,vector<Process>, CustomCompare1 > processQtoFinalPrint;//copy processQ for final print
            priority_queue<Process,vector<Process>, CustomCompare > readyQueueCopy ;//copy for print ready queue
            readyQueue.push(processQ.top()); //put first process to ready queue
            readyQueueCopy=readyQueue;//copy
            currTime=processQ.top().arrivalT;//time update
            out << currTime<<":HEAD-";//print
            while(!readyQueueCopy.empty()) {
                out <<readyQueueCopy.top().name<<"["<<(readyQueueCopy.top().leftinst+1)<<"]"<<"-";

                readyQueueCopy.pop();
            }
            out << "TAIL"<<"\n";

            processQ.pop();

            while(!readyQueue.empty()){

                bool added=false;// any process added or not to ready queue
                Process p=readyQueue.top();
                readyQueue.pop();
                if(!readyQueue.empty()){//check for next ready process exists
                    if(p.priority==readyQueue.top().priority){//equal priority case
                        priority_queue<Process,vector<Process>, CustomCompare1 > temp ;//equal priority queue
                        temp.push(p);
                        while(p.priority==readyQueue.top().priority){
                            temp.push(readyQueue.top());
                            readyQueue.pop();
                            if(readyQueue.empty()){
                                break;
                            }
                        }
                        p=temp.top();//first arrived max priority

                        temp.pop();
                        while(!temp.empty()){//put other processes back to ready queue
                            readyQueue.push(temp.top());
                            temp.pop();
                        }

                    }
                }


                currTime=currTime+p.insts.at(p.leftinst).exeT;//time update
                p.totalInsExe=p.totalInsExe+p.insts.at(p.leftinst).exeT;//total exetime update
                if((p.insts.at(p.leftinst).name.compare("exit"))!=0){//checks exit instruction
                    p.leftinst++; //if not then increment left instruction
                    readyQueue.push(p);
                }else{
                    p.finishT=currTime;//if exit then finishtime update
                    processQtoFinalPrint.push(p);
                    added=true;
                }
                if(!processQ.empty()){
                    while(processQ.top().arrivalT<currTime||processQ.top().arrivalT==currTime){
                        added=true;//new process to ready queue
                        readyQueue.push(processQ.top());
                        processQ.pop();
                        if(processQ.empty()){
                            break;
                        }
                    }
                }
                if(readyQueue.empty()){//check readyqueue
                    if(!processQ.empty()){//check process queue
                        out << currTime<<":HEAD--TAIL"<<"\n";;//print
                        currTime=processQ.top().arrivalT;//if processes not finished then time update
                        added=true;
                        readyQueue.push(processQ.top());//push new process
                        processQ.pop();
                    }
                }
                if(added){
                    readyQueueCopy=readyQueue;//copy
                    out << currTime<<":HEAD-";//print
                    while(!readyQueueCopy.empty()) {
                        out <<readyQueueCopy.top().name<<"["<<(readyQueueCopy.top().leftinst+1)<<"]"<<"-";

                        readyQueueCopy.pop();
                    }
                    if(readyQueue.empty()){
                        out << "-TAIL"<<"\n";;//print
                    }else{
                        out << "TAIL"<<"\n";;//print
                    }

                }
                if(readyQueue.empty()&&processQ.empty()){//if process queue empty
                    out<<"\n";
                    while(!processQtoFinalPrint.empty()) {//print
                        int turnA=processQtoFinalPrint.top().finishT-processQtoFinalPrint.top().arrivalT;
                        out <<"Turnaround time for "<<processQtoFinalPrint.top().name<<" = "<<turnA<<" ms\n";
                        out <<"Waiting time for "<<processQtoFinalPrint.top().name<<" = "<<turnA-processQtoFinalPrint.top().totalInsExe<<"\n";
                        processQtoFinalPrint.pop();
                    }

                }
            }
        }else{
            out<<"Process Queue is empty";
        }
    }
    out.close();


    return 0;
}