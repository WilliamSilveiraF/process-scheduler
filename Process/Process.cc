#include "CPU.h"
#include "Process.h"

/**
 * Construtor do processo
 * -> Cada processo possui um contexto
 * -> O processo é criado no estado Novo e como id, tempo de chega, tempo de execução e prioridade parametrizável
*/

Process::Process(int id, int arrival_time, int execution_time, int period, int deadline, int priority)
: id(id), arrival_time(arrival_time), execution_time(execution_time), period(period),
  deadline(deadline), priority(priority), waiting_time(0), turnaround_time(0), missed_deadlines(0),
  start_time(0), end_time(0), remaining_time(period), execution_time_workload(execution_time), 
  creation_time(arrival_time), state(NEW), context() 
{};

/**
 * loadNewContext
 * - Carrego um novo contexto no processo
 * - Usado antes do chaveamento de contexto
 * para salvar o contexto do processo em execução 
 * na CPU antes dele ser chaveado
*/
void Process::loadNewContext(const CPU::Context& newContext) {
    context = newContext;
}

/**
 * fetchContext
 * Retorno o contexto do processo atual
*/
CPU::Context Process::fetchContext() const {
    return context;
}