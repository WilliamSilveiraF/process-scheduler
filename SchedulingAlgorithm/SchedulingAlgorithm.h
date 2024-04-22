#ifndef __SCHEDULING_ALGORITHM__
#define __SCHEDULING_ALGORITHM__

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <functional>
#include <algorithm>
#include "Process.h"


class Scheduler; // Declaração antecipada da classe Scheduler

// SchedulingAlgorithm Header File
class SchedulingAlgorithm 
{
protected:
    /**
     * holding_scheduler
     * - Referência para o escalonador 
     **/
    Scheduler* holding_scheduler = nullptr;
    /**
     * pidTable
     * - Mapa de processos
    */
    std::map<int, Process*> pidTable;
    /**
     * process_instances
     * - Lista de processos a serem escalonados
    */
    std::vector<Process*> process_instances;
    /**
     * orderReadyProcessInstancesByAlgorithmRules(std::vector<Process*> ready_process_instances)
     * - Ordeno os processos a serem escalonados
     * de acordo com o algoritmo de escalonamento
     * selecionado
    */
    virtual Process* orderReadyProcessInstancesByAlgorithmRules(std::vector<Process*> ready_process_instances) = 0; // Função virtual para ordenar os processos prontos de acordo com o algoritmo de escalonamento

public:
    /**
     * yield
     * Executa o algoritmo de escalonamento
    */  
    virtual void yield() = 0;

    virtual ~SchedulingAlgorithm() {}

    /**
     * handleNextProcess
     * Busco o próximo processo a ser escalonado
     * pelo algoritmo em execução
    */
    virtual Process* handleNextProcess() {
        if (process_instances.empty()) { // Se não há nenhum processo a ser escalonado, retorno nullptr
            //std::cout << "SchedulingAlgorithm::handleNextProcess ~ No process instances are currently available for scheduling" << std::endl;
            return nullptr;
        }

        std::vector<Process*> ready_process_instances;
        for (Process* process_cursor : process_instances) { // Filtro os processos no estado de Pronto
            if (process_cursor->getState() == Process::READY) {
                ready_process_instances.push_back(process_cursor);
            }
        }

        if (ready_process_instances.empty()) { // Se não há processos processos prontos, retorno nullptr
            //std::cout << "SchedulingAlgorithm::handleNextProcess ~ No ready process instances at this time." << std::endl;
            return nullptr;
        }

        /**
         * Ordeno os processos no estado pronto conforme
         * o algorimo de escalonamento em execução
        */
        Process* selected_process = orderReadyProcessInstancesByAlgorithmRules(ready_process_instances);

        /**
         * Removo o processo selecionado para execução
         * da minha lista de processos
        */
        auto cursor = std::find(process_instances.begin(), process_instances.end(), selected_process);
        if (cursor != process_instances.end()) {
            process_instances.erase(cursor);
        }

        return selected_process; // Retorno o processo selecionado para execução
    }

    /**
     * syncReadyQueue
     * - sincronizo processos de acordo com tempo
     * de chegada para o estado de pronto
    */
    virtual void syncReadyQueue(int current_time) {
        for (auto &cursor : pidTable) {
            Process* process_cursor = cursor.second; // puxo o "Process" da tabela
            if (process_cursor->getArrivalTime() <= current_time && process_cursor->getState() == Process::NEW) {
                process_cursor->setState(Process::READY); // Se o tempo de chegada é menor ou igual do que o tempo atual, seto o estado pronto
            }
            if (process_cursor->getArrivalTime() <= current_time && process_cursor->getState() == Process::SUSPENDED) {
                process_cursor->setState(Process::READY);
            }
        }
    }

    /**
     * loadScheduler
     * - Para cada algoritmo de escalonamento 
     * existe um escalonador
    */
    virtual void loadScheduler(Scheduler* new_scheduler) {
        holding_scheduler = new_scheduler;
    }

    /**
     * putProcess
     * - Insiro o processo na lista de processos
     * a serem escalonados
     * Nota: isso não significa que ele está no estado de pronto
    */
    virtual void putProcess(Process* new_process_instance) {
        process_instances.push_back(new_process_instance);
        pidTable[new_process_instance->getID()] = new_process_instance;
    }

    /**
     * getTurnaroundTimeAvg
     * - Retorno o Turnaround time (tempo transcorrido 
     * desde o momento em que o processo é criado 
     * até o instante em que termina sua execução
    */
    virtual float getTurnaroundTimeAvg() {
        float avg = 0;
        for (auto &cursor : pidTable) { // Varro toda minha tabela de processos pegando o turnaround calculado após o fim da execução
            int turnaround_time = cursor.second->getTurnaroundTime();
            avg += turnaround_time;
        };

        avg /= pidTable.size();
        return avg;
    }

    /**
     * getWaitingTimeAvg
     * - Retorno o Tempo médio de espera (soma dos 
     * períodos em que o processo estava no seu estado
     * pronto)
    */
    virtual float getWaitingTimeAvg() {
        float avg = 0;
        for (auto &cursor : pidTable) {
            int wt_time = cursor.second->getWaitingTime();
            avg += wt_time; 
        };

        avg /= pidTable.size();
        return avg;
    }

    /**
     * logStats
     * - Logo todas as estatísticas para o algoritmo de escalonamento em execução
    */
    virtual void logStats(int switched_context_amount, std::string algorithm_name) {
        std::cout << "\n\n========== [ Stats " << algorithm_name << " ] ==========\n";

        /**
         * Print das stats de tempo de espera
        */
        std::cout << "[Waiting time for each process] \n";
        for (const auto& cursor : pidTable) {
            Process* process_cursor = cursor.second;
            std::cout << "P" << cursor.first << "  " << process_cursor->getWaitingTime() << "\n";
        }

        std::cout << "Total Waiting Time Avg:  " << getWaitingTimeAvg() << "\n\n";

        /**
         * Print das stats de turnaround time
        */
        std::cout << "[Turnaround time for each process] \n";

        for (const auto& cursor : pidTable) {
            Process* process_cursor = cursor.second;
            std::cout << "P" << cursor.first << "  " << process_cursor->getTurnaroundTime() << "\n";
        };
        
        std::cout << "Total Turnaround Time Avg:  " << getTurnaroundTimeAvg() << "\n\n";

        /* PRINT DE CONTADOR DE TROCAS DE CONTEXTO */
        std::cout << "Switched context amount: " << switched_context_amount << "\n";

        std::cout << "\n========== [ END ] ==========\n\n";
    }
};

#endif // Algorithm