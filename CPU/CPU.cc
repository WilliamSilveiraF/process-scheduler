#include "CPU.h"

/**
 * CPU
 * Inicializo todos os registradores, stack pointer, program counter e status
*/
CPU::CPU() {
    STACK_POINTER = 0;
    PROGRAM_COUNTER = 0;
    STATUS = 0;

    for (int i = 0; i < 6; ++i) registrars[i] = 0;
}




/**
 * Contexto da CPU
 * Inicializo todos os registradores, stack pointer, program counter e status
*/
CPU::Context::Context() {
    STACK_POINTER = 0;
    PROGRAM_COUNTER = 0;
    STATUS = 0;
    
    for (int i = 0; i < 6; ++i) registrars[i] = 0;
}

/**
 * Construtor do contexto da CPU criado a partir de uma cópia
 * Instancio os valores de todos os registradores, stack pointer, program counter e status
*/
CPU::Context::Context(const Context& context_instance) {
    STACK_POINTER = context_instance.STACK_POINTER;
    PROGRAM_COUNTER = context_instance.PROGRAM_COUNTER;
    STATUS = context_instance.STATUS;

    for (int i = 0; i < 6; +i) registrars[i] = context_instance.registrars[i];
}