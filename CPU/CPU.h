#ifndef CPU_HEADER
#define CPU_HEADER

#include <cstdint>

/**
 * CPU
 * Abstração de CPU do processador INE5412
 * - Defino stack pointer, program counter e status conforme enunciado
 *   do trabalho para a CPU (64 bits)
 * - Defino os 6 registradores conforme enuciado do trabalho (64 bits)
 * - Defino o método loadContextCursor para carregar um contexto de
 *   um novo processo selecionado na CPU
 * - Defino o método fetchContextCursor para buscar o contexto atual 
 *   da CPU antes de um chaveamento de contexto
*/
class CPU {
public:
    CPU(); /* Inicializo os 6 registradores, stack_pointer, program_counter e status da CPU */


    /**
     * Implemento o contexto a fim de permitir a mudança de 
     * processador para a abstração de um processo, uma vez que
     * cada abastração do processo tem um contexto
    */
    class Context {
        public:
            uint64_t STACK_POINTER;
            uint64_t PROGRAM_COUNTER;
            uint64_t STATUS;
            uint64_t registrars[6];
        
        Context(); /* Inicializo os 6 registradores, stack_pointer, program_counter e status do contexto */
        Context(const Context& context_instance); /* Construo uma contexto a partir de uma cópia */
    };


    /**
     * Crio os métodos loadContextCursor e fetchContextCursor para 
     * permitir o chaveamento de contexto na CPU entre o processo
     * que estava em execução e o próximo processo a ser executado
    */
    void loadContextCursor(const Context& context_instance); 
    /* Carrego o contexto da CPU com os valores fornecidos */
    Context fetchContextCursor() const; 
    /**
     * Busco o contexto atual da CPU -- Usado para salvar o estado 
     * atual da CPU antes do chaveamento do processo 
    */

    /* Inicializo os 6 registradores, stack_pointer, program_counter e status da CPU */
    private:
        uint64_t STACK_POINTER; 
        uint64_t PROGRAM_COUNTER;
        uint64_t STATUS;
        uint64_t registrars[6];
};

#endif
