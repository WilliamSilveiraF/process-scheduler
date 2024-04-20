#ifndef FCFS_H
#define FCFS_H

// Inclui as bibliotecas necessárias
#include <vector>
#include <map>
#include <functional>
#include "Process.h"
#include "SchedulingAlgorithm.h"
#include "Scheduler.h"

// Define a classe FCFS, que herda de SchedulingAlgorithm
class FCFS : public SchedulingAlgorithm {
private:
    // Nome do algoritmo, usado para fins de impressão
    const char * algorithmName = "First Come First Served";

public:
    // Construtor padrão
    FCFS();

    // Destrutor virtual. É declarado como virtual para garantir que, se uma classe derivada de FCFS for destruída por um ponteiro para FCFS, o destrutor da classe derivada será chamado.
    virtual ~FCFS() {}

    // Função para simular o algoritmo FCFS. Ela é uma função virtual que sobrescreve a função da classe base.
    void yield() override;

    // Função para ordenar os processos prontos. No caso do FCFS, o processo que chegou primeiro é selecionado. Ela sobrescreve a função da classe base.
    Process* orderReadyProcessInstancesByAlgorithmRules(std::vector<Process*> ready_process_instances) override;
};

#endif // FCFS_H
