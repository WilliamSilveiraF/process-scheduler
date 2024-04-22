#include "Scheduler.h"

/**
 * Scheduler (construtor)
 * - Carrego uma abstração de algoritmo
 * de escalonamento
 * - Deixo zerado o contador de chaveamento
 * de contexto
*/
Scheduler::Scheduler(Algorithm* new_algorithm) : holding_algorithm(new_algorithm) {
    holding_algorithm->loadScheduler(this);
    switch_context_amount = 0;
}

/**
 * yield
 * - Executo o algoritmo de escalonamento
 * selecionado no momento
*/
void Scheduler::yield() {
    holding_algorithm->yield();
}

/**
 * handleNextProcess
 * - Busco o próximo processo que vai ser escalonado
 * de acordo com algoritmo de escalonamento em execução
*/
Process* Scheduler::handleNextProcess() {
    return holding_algorithm->handleNextProcess();
}

/**
 * putProcess
 * - Insiro o processo na lista de processos a serem
 * escalonados pelo meu algoritmo selecionado
*/
void Scheduler::putProcess(Process* new_process_instance) {
    holding_algorithm->putProcess(new_process_instance);
}


/**
 * selectAlgorithm
 * Seleciono algoritmo que quero executar
*/
void Scheduler::selectAlgorithm(Algorithm* new_algorithm) {
    switch_context_amount = 0;
    holding_algorithm = new_algorithm;
    holding_algorithm->loadScheduler(this);
}

/**
 * switchContext
 * - Faço o chaveamento de contexto entre o processo atual
 * em execução e o novo processo 
 * - Salvo e restauro o contexto do processo que estava 
 * em execução e do próximo processo a ser executado
*/
void Scheduler::switchContext(Process* current_process, Process* new_process) {
    if (current_process) {
        current_process->loadNewContext(ine5412_core.fetchContextCursor());
    }

    if (new_process) {
        ine5412_core.loadContextCursor(new_process->fetchContext());
    }

    switch_context_amount++;
}