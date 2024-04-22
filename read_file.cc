#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Process.h"
#include "Scheduler.h"
#include "RM.h"
#include "EDF.h"

using namespace std;

class File
{

public:

    File() {
        myfile.open("entrada.txt");
        if (!myfile.is_open()) {
            cout << "Erro ao abrir o arquivo!\n";
        }
    }
    
    void read_file() {
    
        int arrival_time, execution_time, period, deadline, priority;
        
        if (!myfile.is_open()) {
            cout << "Arquivo não está aberto!" << endl;
        }
        
        int processID = 1;
        while (myfile >> arrival_time >> execution_time >> period >> deadline >> priority) {
            Process *p = new Process(processID, arrival_time, execution_time, period, deadline, priority);
            processes.push_back(p);
            processID++;
        }

        cout << "Quantidade de processos lidos do arquivo: " << processes.size() << endl;
    }

    void print_processes_params() {
        for (auto p : processes) {
            cout << "\nProcess ID = " << p->getID() 
                 << " Arrival Time = " << p->getArrivalTime()
                 << " Execution Time = " << p->getExecutionTime()
                 << " Execution Time Workload = " << p->getExecutionTimeWorkload()
                 << " Period = " << p->getPeriod()
                 << " Deadline = " << p->getDeadline()
                 << " Priority = " << p->getPriority() << endl;
        }
    }

    vector<Process *> get_processes() {
        return processes;
    }

    ~File() {
        for(auto p : processes) {
            delete p;
        }
    }

private:
    ifstream myfile; 
    vector<Process *> processes;
};


int main(int argc, char *argv[]) {
    if (argc < 2) {
        cout << "Usage: " << argv[0] << " [EDF | RM]" << endl;
        return 1;
    }

    string algorithm(argv[1]);

    File fileReader;
    fileReader.read_file();
    fileReader.print_processes_params();

    vector<Process *> processes = fileReader.get_processes();

    Algorithm* schedAlg = nullptr;
    if (algorithm == "EDF") {
        schedAlg = new EDF();
    } else if (algorithm == "RM") {
        schedAlg = new RM();
    } else {
        cout << "Invalid scheduling algorithm specified. Use 'EDF' or 'RM'." << endl;
        return 1;
    }

    Scheduler scheduler(schedAlg);

    for(auto process : processes) {
        scheduler.putProcess(process);
    }

    scheduler.yield();

    delete schedAlg;

    return 0;
}