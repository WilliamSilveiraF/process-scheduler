#ifndef __SCHEDULING_ALGORITHM__
#define __SCHEDULING_ALGORITHM__

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <functional>
#include <algorithm>
#include "Proccess.h"

class Scheduler;

class SchedulingAlgorithm {
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
    virtual Process* orderReadyProcessInstancesByAlgorithmRules(std::vector<Process*> ready_process_instances) = 0;

public:
   /**
    * handleNextProcess
    * - Busco o próximo processo que vai ser escalonado
    * de acordo com algoritmo de escalonamento em execução
   */
    virtual Process* handleNextProcess();
    
    /**
     * syncReadyQueue
     * - Mantenho a lista de processos a serem escalonados 
     * atualizada funcionando como um "trigger" ouvinte do current_time 
     * Nota: aqui apenas "escuto" o tempo de chegada do processo e insiro
     * ele caso o tempo atual seja igual ou maior ao tempo de chegada
    */
    virtual void syncReadyQueue(int current_time);

    /**
     * putProcess
     * - Insiro o processo na lista de processos a serem
     * escalonados
    */
    virtual void putProcess(Process* new_process_instance);

    /**
     * putManyToManyProcess
     * - Insiro muitos processos na lista de processos a
     * serem escalonados
    */
    virtual void putManyToManyProcess(const std::vector<Process*>& new_process_instances);

    /**
     * getTurnaroundTimeAvg
     * - Retorno o Turnaround time (tempo transcorrido 
     * desde o momento em que o processo é criado 
     * até o instante em que termina sua execução
    */
    virtual float getTurnaroundTimeAvg();

    /**
     * getWaitingTimeAvg
     * - Retorno o Tempo médio de espera (soma dos 
     * períodos em que o processo estava no seu estado
     * pronto)
    */
    virtual float getWaitingTimeAvg();

    /**
     * loadScheduler
     * - Para cada algoritmo de escalonamento 
     * existe um escalonador
    */
    virtual void loadScheduler(Scheduler* new_scheduler);

    /**
     * logStats
     * - Logo todas as estatísticas para o algoritmo de escalonamento em execução
    */
    virtual void logStats(int switched_context_amount, std:: string algorithm_name);

    /**
     * yield
     * Executa o algoritmo de escalonamento
    */
    virtual void yield() = 0;

};

#endif