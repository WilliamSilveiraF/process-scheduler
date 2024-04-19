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
 * loadContextCursor
 * Carrego o contexto na CPU durante o chaveamento entre
 * o processo que está em execução e o próximo processo
 * a ser executado
*/
void CPU::loadContextCursor(const Context& context_instance) {
    STACK_POINTER = context_instance.STACK_POINTER;
    PROGRAM_COUNTER = context_instance.PROGRAM_COUNTER;
    STATUS = context_instance.STATUS;

    for (int i = 0; i < 6; ++i) registrars[i] = context_instance.registrars[i];
}

/**
 * fetchContextCursor
 * Busco o contexto atual na CPU 
 * Uso para salvar o estado atual da CPU antes do chaveamento do processo
*/
CPU::Context CPU::fetchContextCursor() const {
    Context cursor_context;
    cursor_context.STACK_POINTER = STACK_POINTER;
    cursor_context.PROGRAM_COUNTER = PROGRAM_COUNTER;
    cursor_context.STATUS = STATUS;

    for (int i = 0; i < 6; ++i) cursor_context.registrars[i] = registrars[i];
    
    return cursor_context;
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