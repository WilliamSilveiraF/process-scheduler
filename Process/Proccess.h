#ifndef __PROCCESS__
#define __PROCCESS__

#include "CPU.h"
#include <iostream>

class Process {
private:
    int id;
    int priority;

    int arrival_time;
    int execution_time;
    int waiting_time; 
    int turnaround_time;
    int start_time;
    int end_time;
    int remaining_time;
    int state;
    CPU::Context context;

public:
    Process(int id, int arrival_time, int execution_time, int priority);

    enum State {
        NEW,
        READY,
        EXECUTING,
        FINISHED
    };

    /**
     * loadNewContext
     * Carrego o novo contexto da CPU
     * no processo antes dele sair de execução da CPU
    */
    void loadNewContext(const CPU::Context& newContext); 
    /**
     * fetchContext
     * Busco o contexto atual do processo
    */
    CPU::Context fetchContext() const;

    int getID() const { return id; }
    void setID(int new_pid) { id = new_pid; }

    int getPriority() const { return priority; }
    void setPriority(int new_priority) { priority = new_priority; }

    int getArrivalTime() const { return arrival_time; }
    void setArrivalTime(int new_arrival_time) { arrival_time = new_arrival_time; } 
    
    int getExecutionTime() const { return execution_time; }
    void setExecutionTime(int new_execution_time) { execution_time = new_execution_time; }

    int getWaitingTime() const { return waiting_time; }
    void setWaitingTime(int new_waiting_time) { waiting_time = new_waiting_time; }
    
    int getTurnaroundTime() const { return turnaround_time; }
    void setTurnaroundTime(int new_turnaorund_time) { turnaround_time = new_turnaorund_time; }

    int getStartTime() const { return start_time; }
    void setStartTime(int new_start_time) { start_time = new_start_time; }

    int getEndTime() const { return end_time; }
    void setEndTime(int new_end_time) { end_time = new_end_time; }
    
    int getRemainingTime() const { return remaining_time; }
    void setRemainingTime(int new_remaining_time) { remaining_time = new_remaining_time; }

    int getState() const { return state; }
    void setState(int new_state) { state = new_state; }
};

#endif