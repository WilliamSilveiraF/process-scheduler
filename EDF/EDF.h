#ifndef __EDF__
#define __EDF__

#include "Process.h"
#include "Algorithm.h"
#include <vector>

class EDF : public Algorithm {
public:
    EDF();
    virtual ~EDF();

    void yield() override;
    Process* orderReadyProcessInstancesByAlgorithmRules(std::vector<Process*> ready_process_instances) override;
    bool checkEDFPreemption(Process* executing_process);

private:
    const char *algorithmName = "Earliest Deadline First";
};

#endif // EDF