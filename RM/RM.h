#ifndef __RM__
#define __RM__

// Inclui as bibliotecas necessárias
#include <vector>
#include <map>
#include <functional>
#include "Process.h"
#include "SchedulingAlgorithm.h"
#include "Scheduler.h"

// Define a classe RM, que herda de SchedulingAlgorithm
class RM : public SchedulingAlgorithm {
private:
    // Nome do algoritmo, usado para fins de impressão
    const char * algorithmName = "First Come First Served";

public:
    // Construtor padrão
    RM();

    // Destrutor virtual. É declarado como virtual para garantir que, se uma classe derivada de RM for destruída por um ponteiro para RM, o destrutor da classe derivada será chamado.
    virtual ~RM() {}

    // Função para simular o algoritmo RM. Ela é uma função virtual que sobrescreve a função da classe base.
    void yield() override;

    // Função para ordenar os processos prontos. No caso do RM, o processo que chegou primeiro é selecionado. Ela sobrescreve a função da classe base.
    Process* orderReadyProcessInstancesByAlgorithmRules(std::vector<Process*> ready_process_instances) override;
};

#endif // RM
