#include "CPU.h"
#include "Proccess.h"

/**
 * Construtor do processo
 * -> Cada processo possuim um contexto
 * -> O processo é criado no estado Novo e como id, tempo de chega, tempo de execução e prioridade parametrizável
*/
Process::Process(int id, int arrival_time, int execution_time, int priority)
: id(id),
  arrival_time(arrival_time),
  execution_time(execution_time),
  priority(priority),
  waiting_time(0),
  turnaround_time(0),
  start_time(0),
  end_time(0),
  remaining_time(execution_time),
  state(NEW),
  context() {};

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
 * Retorna o contexto do processo atual
*/
CPU::Context Process::fetchContext() const {
    return context;
}