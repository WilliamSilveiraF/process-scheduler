#ifndef __SCHEDULER__
#define __SCHEDULER__

#include "CPU.h"
#include "Algorithm.h"

class Scheduler {
private:
    /* CPU */
    CPU ine5412_core;
    /* Abstração do algoritmo de escalonamento */
    Algorithm* holding_algorithm;
    /* indexador de trocas de contexto */
    int switch_context_amount; 

public:
    Scheduler(Algorithm* new_algorithm);
   
   /**
    * handleNextProcess
    * - Busco o próximo processo que vai ser escalonado
    * de acordo com algoritmo de escalonamento em execução
   */
    Process* handleNextProcess();
    
    /**
     * putProcess
     * - Insiro o processo na lista de processos a serem
     * escalonados pelo meu algoritmo selecionado
    */
    void putProcess(Process* new_process_instance);
    
    /**
     * restartProcessInstances
     * - Restarto todos os processos da minha lista de processos
     * para a execução do próximo algoritmo de escalonamento
    */
    void restartProcessInstances(const std::vector<Process*>& process_instances);
    
    /**
     * switchContext
     * - Faço o chaveamento de contexto entre o processo atual
     * em execução e o novo processo 
    */
    void switchContext(Process* current_process, Process* new_process);

    /**
     * yield
     * - Executo o algoritmo de escalonamento
     * selecionado no momento
    */
    void yield();

    /**
     * selectAlgorithm
     * Seleciono algoritmo que quero executar
    */
    void selectAlgorithm(Algorithm* new_algorithm);

    /**
     * getSwitchContextAmount
     * Retorno a quantidade de trocas de contexto
    */
    int getSwitchContextAmount() { return switch_context_amount; }
};



#endif // Scheduler