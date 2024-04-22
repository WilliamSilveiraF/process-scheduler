#ifndef __RM__
#define __RM__

#include <vector>
#include <map>
#include <functional>
#include "Process.h"
#include "Algorithm.h"
#include "Scheduler.h"

class RM : public Algorithm {
private:
    const char * algorithmName = "Rate Monotonic Served";

public:
    RM();

    virtual ~RM() {}

    void yield() override;

    Process* orderReadyProcessInstancesByAlgorithmRules(std::vector<Process*> ready_process_instances) override;
    /**
     * checkRMPreemptionAvaibility
     * - Verifica se um processo tem prioridade
     * maior do que o outro
    */
    bool checkRMPreemptionAvaibility(Process* executing_process);
};

#endif // RM
