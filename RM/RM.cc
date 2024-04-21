#include <iostream>
#include <queue>
#include <algorithm>
#include "Process.h"
#include "SchedulingAlgorithm.h"
#include "RM.h"

RM::RM() {}

Process* RM::orderReadyProcessInstancesByAlgorithmRules(std::vector<Process*> ready_process_instances) {
    std::sort(ready_process_instances.begin(), ready_process_instances.end(), 
        [](Process* a, Process* b) {
            return a->getPriority() > b->getPriority();  // Ordena pelo período em vez de prioridade
        }
    );
    return ready_process_instances.empty() ? nullptr : ready_process_instances.front();
}

void RM::yield() {
    std::cout << "-> Início algoritmo Rate Monotonic - RM...\n\n";

    int currentTime = 0; // Representa o tempo atual na simulação
    Process* prevProcess = nullptr; // Mantém o registro do processo anterior
    std::vector<Process*> allProcesses = process_instances;  // Todos os processos a serem escalonados

    // Imprime os cabeçalhos para o diagrama de tempo
    std::cout << "tempo ";
    for (size_t i = 1; i <= allProcesses.size(); i++) {
        std::cout << "P" << i << " ";
    }
    std::cout << "\n";

    // Função lambda para imprimir o estado atual dos processos
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
                    std::cout << "ss ";
                    p->setWaitingTime(p->getWaitingTime()+1);
                    break;
                default:
                    std::cout << "   ";
                    break;
            }
        }
        std::cout << "\n";
    };

    // Enquanto houver processos a serem escalonados
    while (!process_instances.empty()) {
        syncReadyQueue(currentTime); // Atualiza a lista de processos prontos
        Process* currentProcess = handleNextProcess(); // Pega o próximo processo a ser escalonado

        // Se não houver processo atual ou se o tempo de chegada do processo for maior que o tempo atual
        while (!currentProcess || currentProcess->getArrivalTime() > currentTime) {
            printProcessesState();
            currentTime++;
            syncReadyQueue(currentTime);
            if (!currentProcess) {
                currentProcess = handleNextProcess();
            }
        }

        // Realiza a troca de contexto se o processo atual for diferente do anterior
        if (prevProcess != currentProcess && holding_scheduler) {
            holding_scheduler->switchContext(prevProcess, currentProcess);
        }

        currentProcess->setStartTime(currentTime);
        currentProcess->setState(Process::EXECUTING);

        
        //std::cout << "P" << currentProcess->getID() << " execution time " << currentProcess->getExecutionTime() << "\n";
        //std::cout << "P" << currentProcess->getID() << " remaining time " << currentProcess->getRemainingTime() << "\n";
        // Executa o processo pelo tempo de execução
        bool preemptionExecuted = 0;
        for (int j = 0; j < currentProcess->getExecutionTime(); j++) {
            syncReadyQueue(currentTime);
            if (checkPreemptionAvaibility(currentProcess)) {
                preemptionExecuted = 1;
                break;
            }
    
            if (currentProcess->getRemainingTime() > 0) {
                printProcessesState();
                currentTime++;
                syncReadyQueue(currentTime);
                currentProcess->setRemainingTime(currentProcess->getRemainingTime()-1);
            } else {
                break;
            }
        }

        if (currentProcess->getRemainingTime() <= 0) {
            currentProcess->setEndTime(currentTime);
            //currentProcess->setTurnaroundTime(currentTime - currentProcess->getArrivalTime());
            currentProcess->setState(Process::FINISHED);
        } else if (preemptionExecuted) {
            currentProcess->setState(Process::READY);
            holding_scheduler->putProcess(currentProcess);
        } else {
            std::cout << "P" << currentProcess->getID() << " próxima execução em " << currentProcess->getArrivalTime() + currentProcess->getPeriod() << "\n";
            currentProcess->setArrivalTime(currentProcess->getArrivalTime() + currentProcess->getPeriod());
            currentProcess->setState(Process::SUSPENDED);
            holding_scheduler->putProcess(currentProcess);
        }
        //currentProcess->setTurnaroundTime(currentProcess->getEndTime() - currentProcess->getArrivalTime());
        
        prevProcess = currentProcess;
    }

    // Imprime os resultados da simulação
    logStats(holding_scheduler->getSwitchContextAmount(), algorithmName);
}
