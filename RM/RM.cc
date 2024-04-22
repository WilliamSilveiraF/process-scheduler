#include <iostream>
#include <queue>
#include <algorithm>
#include "Process.h"
#include "Algorithm.h"
#include "RM.h"

RM::RM() {}

Process* RM::orderReadyProcessInstancesByAlgorithmRules(std::vector<Process*> ready_process_instances) {
    std::sort(ready_process_instances.begin(), ready_process_instances.end(), 
        [](Process* a, Process* b) {
            return a->getPriority() > b->getPriority(); 
        }
    );
    return ready_process_instances.empty() ? nullptr : ready_process_instances.front();
}

void RM::yield() {
    std::cout << "\nRate Monotonic\n\n";

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

        if (prevProcess != currentProcess && holding_scheduler) {
            holding_scheduler->switchContext(prevProcess, currentProcess);
        }

        currentProcess->setStartTime(currentTime);
        currentProcess->setState(Process::EXECUTING);

        
        //std::cout << "P" << currentProcess->getID() << " execution time " << currentProcess->getExecutionTime() << "\n";
        //std::cout << "P" << currentProcess->getID() << " remaining time " << currentProcess->getRemainingTime() << "\n";
        bool preemptionExecuted = 0;
        while (currentProcess->getExecutionTime() > 0) {
            syncReadyQueue(currentTime);
            if (checkRMPreemptionAvaibility(currentProcess)) {
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
            //std::cout << "P" << currentProcess->getID() << " próxima execução em " << currentProcess->getArrivalTime() + currentProcess->getPeriod() << "\n";
            currentProcess->setArrivalTime(currentProcess->getArrivalTime() + currentProcess->getPeriod());
            currentProcess->setState(Process::SUSPENDED);
            currentProcess->setExecutionTime(currentProcess->getExecutionTimeWorkload());
            holding_scheduler->putProcess(currentProcess);
        }
        //currentProcess->setTurnaroundTime(currentProcess->getEndTime() - currentProcess->getArrivalTime());
        
        prevProcess = currentProcess;
    }

    logStats(holding_scheduler->getSwitchContextAmount(), algorithmName);
}

/**
 * checkRMPreemptionAvaibility
 * - Verifica se um processo tem prioridade
 * maior do que o outro
*/
bool RM::checkRMPreemptionAvaibility(Process* executing_process) {
    /**
     * Posso premptar se acho algum processo em execução
     * com prioridade maior
    */
    
    for (auto &cursor : pidTable) {
        Process* process_cursor = cursor.second;
        if (process_cursor->getState() == Process::READY && executing_process->getPriority() < process_cursor->getPriority()) {
            return 1;
        };
    }
    return 0;
}