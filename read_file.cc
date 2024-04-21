#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Process.h"
#include "Scheduler.h"
#include "RM.h"

using namespace std;

class ProcessParams
{
public:
	ProcessParams(int c, int d, int p) { 
		creation_time = c;
		duration = d;
		priority = p;
	}

	friend ostream &operator<<(ostream& os, const ProcessParams& p) {
		os << "Creation time = " << p.creation_time << " duration = " << p.duration << " priority = " << p.priority << endl;
		return os;
	}

	//getters
	int getCreationTime() { return creation_time; }
	int getDuration() { return duration; }
	int getPriority() { return priority; }

private:	
	int creation_time;
	int duration; //seconds
	int priority;
};

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
    
        int a, b, c, d, e;
        
        if (!myfile.is_open()) {
            cout << "Arquivo não está aberto!" << endl;
        }
        
        int processID = 1;  // Assuming IDs start at 1 and increment by 1
        while (myfile >> a >> b >> c >> d >> e) {
            Process *p = new Process(processID, a, b, c, d, e);
            processes.push_back(p);
            processID++;  // Increment the process ID for the next process
        }

        cout << "Quantidade de processos lidos do arquivo: " << processes.size() << endl;
    }

    void print_processes_params() {
        for (auto p : processes) {
            cout << "\nProcess ID = " << p->getID() 
                 << " Arrival Time = " << p->getArrivalTime()
                 << " Execution Time = " << p->getExecutionTime()
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


int main() {
    // Read processes from the file
    File fileReader;
    fileReader.read_file();
    fileReader.print_processes_params();

    // Get processes
    vector<Process *> processes = fileReader.get_processes();

    // Start the simulations
    std::cout<< " \n\n  ======================================\n"
             << "||                                      ||\n"
             << "||  Simulação de escalonamento de CPU   ||\n"
             << "||                                      ||\n"
            << "  ======================================\n" << std::endl;
   
    //======================================
    // RM Scheduling Algorithm
    //======================================

    // Setup scheduler with RM algorithm
    RM rmAlgorithm;
    Scheduler scheduler(&rmAlgorithm);

    // Add processes to scheduler
    for(auto process : processes) {
        scheduler.putProcess(process);
    }

    // Run the scheduling simulation for RM
    scheduler.yield();
    scheduler.restartProcessInstances(processes);
}