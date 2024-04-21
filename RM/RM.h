#ifndef __RM__
#define __RM__

#include "SchedulingAlgorithm.h"

class RM: public SchedulingAlgorithm {
private:
    const char * algortihm_name = "Rate Monolitic";
public:
    RM();
    virtual ~ RM() {};
    
    Process* orderReadyProcessInstancesByAlgorithmRules(std::vector<Process*> ready_process_instances) override;
    
    void yield() override;
     
};

#endif