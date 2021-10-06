#ifndef tipo_vm_h
#define tipo_vm_h

#include "chunk.h"

#define STACK_MAX 256

/**
 * 虚拟机结构体
 */
typedef struct {
    /**
     * 存储要执行代码的 Chunk
     */
    Chunk* chunk;
    /**
     * 指向下一行准备执行的代码
     */
    uint8_t* ip;
    /**
     * 存储调用栈的数组
     */
    Value stack[STACK_MAX];
    /**
     * 堆顶
     */
    Value* stackTop;
} VM;

typedef enum {
    INTERPRET_OK,
    INTERPRET_COMPILE_ERROR,
    INTERPRET_RUNTIME_ERROR
} InterpretResult;

void initVM();
void freeVM();
InterpretResult interpret(Chunk* chunk);
void push(Value value);
Value pop();

#endif tipo_vm_h
