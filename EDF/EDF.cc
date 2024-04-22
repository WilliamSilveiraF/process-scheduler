#include "EDF.h"
#include <iostream>
#include <algorithm>
#include "Scheduler.h"

EDF::EDF() {}

EDF::~EDF() {}

void EDF::yield() {
    std::cout << "\nEarliest Deadline First\n\n";

    int currentTime = 0; 
    Process* prevProcess = nullptr;
    std::vector<Process*> allProcesses = process_instances;

    std::cout << "tempo ";
    for (size_t i = 1; i <= allProcesses.size(); i++) {
        std::cout << "P" << i << " ";
    }
    std::cout << "\n";

    auto printProcessesState = [&]() {
        std::cout << " " << currentTime << "-" << (currentTime + 1) << "  ";
        for (Process* p : allProcesses) {
            switch (p->getState()) {
                case Process::EXECUTING:
                    std::cout << "## ";
                    break;
                case Process::READY:
                    std::cout << "-- ";
                    p->setWaitingTime(p->getWaitingTime()+1);
                    break;
                case Process::SUSPENDED:
                    std::cout << "-- ";
                    break;
                default:
                    std::cout << "   ";
                    break;
            }
        }
        std::cout << "\n";
    };
    
    
    /**
     * Enquanto houver processos a
     * serem escalonados
     */    
    while (!process_instances.empty()) {
        syncReadyQueue(currentTime);
        Process* currentProcess = handleNextProcess();

        while (!currentProcess || currentProcess->getArrivalTime() > currentTime) {
            printProcessesState();
            currentTime++;
            syncReadyQueue(currentTime);
            if (!currentProcess) {
                currentProcess = handleNextProcess();
            }
        }

        
        /**
         * Realizo a troca de contexto se
         * o processo atual for diferente do anterior
        */
        if (prevProcess != currentProcess && holding_scheduler) {
            holding_scheduler->switchContext(prevProcess, currentProcess);
        }

        currentProcess->setStartTime(currentTime);
        currentProcess->setState(Process::EXECUTING);

        
        //std::cout << "P" << currentProcess->getID() << " execution time " << currentProcess->getExecutionTime() << "\n";
        //std::cout << "P" << currentProcess->getID() << " remaining time " << currentProcess->getRemainingTime() << "\n";
        /**
         * Executo o processo pelo
         * tempo de execução
        */
        bool preemptionExecuted = 0;
        while (currentProcess->getExecutionTime() > 0) {
            syncReadyQueue(currentTime);
            if (checkEDFPreemption(currentProcess)) {
                preemptionExecuted = 1;
                break;
            }
    
            if (currentProcess->getRemainingTime() > 0) {
                printProcessesState();
                currentTime++;
                syncReadyQueue(currentTime);
                currentProcess->setExecutionTime(currentProcess->getExecutionTime()-1);
                currentProcess->setRemainingTime(currentProcess->getRemainingTime()-1);
            } else {
                break;
            }
        }

        if (currentProcess->getRemainingTime() <= 0) {
            currentProcess->setEndTime(currentTime);
            if (currentProcess->getEndTime() > currentProcess->getDeadline()) {
                currentProcess->setMissedDeadlines(currentProcess->getMissedDeadlines() + 1);
            }
            currentProcess->setTurnaroundTime(currentTime - currentProcess->getCreationTime());
            currentProcess->setState(Process::FINISHED);
        } else if (preemptionExecuted) {
            currentProcess->setState(Process::READY);
            holding_scheduler->putProcess(currentProcess);
        } else {
            //std::cout << "P" << currentProcess->getID() << " Arrival time " << currentProcess->getArrivalTime() << "\n";
            //std::cout << "P" << currentProcess->getID() << " Period " << currentProcess->getPeriod() << "\n";
            //std::cout << "P" << currentProcess->getID() << " próxima execução em " << currentProcess->getArrivalTime() + currentProcess->getPeriod() << "\n";
            currentProcess->setArrivalTime(currentProcess->getArrivalTime() + currentProcess->getPeriod());
            currentProcess->setDeadline(currentProcess->getArrivalTime() + currentProcess->getPeriod());
            currentProcess->setState(Process::SUSPENDED);
            currentProcess->setExecutionTime(currentProcess->getExecutionTimeWorkload());
            holding_scheduler->putProcess(currentProcess);
        }
        
        prevProcess = currentProcess;
    }

    logStats(holding_scheduler->getSwitchContextAmount(), algorithmName);
}

Process* EDF::orderReadyProcessInstancesByAlgorithmRules(std::vector<Process*> ready_process_instances) {
    std::sort(ready_process_instances.begin(), ready_process_instances.end(), 
        [](Process* a, Process* b) {
            return a->getDeadline() < b->getDeadline();
        }
    );
    return !ready_process_instances.empty() ? ready_process_instances.front() : nullptr;
}

bool EDF::checkEDFPreemption(Process* executing_process) {
    for (auto &cursor : pidTable) {
        Process* process_cursor = cursor.second;
        if (process_cursor->getState() == Process::READY && process_cursor->getDeadline() < executing_process->getDeadline()) {
            return true;
        }
    }
    return false;
}