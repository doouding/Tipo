#include "common.h"
#include "debug.h"
#include "vm.h"

VM vm;

static InterpretResult run() {
#define READ_BYTE() (*vm.ip++)
#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])
// 注意 BINARY_OP 里 a 和 b 的顺序
// 当我们计算二目运算符的结果时，左侧先计算，然后再计算右侧
// 这意味着，左侧的结果先被推入栈中，然后才是右侧，因此 pop 的时候
// 第一个 pop 出来的是 b
#define BINARY_OP(op) \
    do {              \
        double b = pop(); \
        double a = pop(); \
        push(a op b); \
    } while (false)
#define READ_LONG_CONSTANT() (vm.chunk->constants.values[READ_BYTE() | (READ_BYTE() << 8) | (READ_BYTE() << 16)])
    for(;;) {
#ifdef DEBUG_TRACE_EXECUTION
        for (Value* slot = vm.stack; slot < vm.stackTop; slot++) {
            printf("[ ");
            printValue(*slot);
            printf(" ]");
        }
        printf("\n");
        disassembleInstruction(vm.chunk,(int)(vm.ip - vm.chunk->code));
#endif
        uint8_t instruction;
        switch (instruction = READ_BYTE()) {
            case OP_CONSTANT: {
                Value constant = READ_CONSTANT();
                push(constant);
                break;
            }
            case OP_CONSTANT_LONG: {
                Value constant = READ_LONG_CONSTANT();
                push(constant);
                break;
            }
            case OP_NEGATE:
                push(-pop()); break;
            case OP_ADD:
                BINARY_OP(+); break;
            case OP_SUBTRACT:
                BINARY_OP(-); break;
            case OP_MULTIPLY:
                BINARY_OP(*); break;
            case OP_DIVIDE:
                BINARY_OP(/); break;
            case OP_RETURN: {
                printValue(pop());
                printf("\n");
                return INTERPRET_OK;
            }
        }
    }
#undef READ_BYTE
#undef READ_CONSTANT
#undef BINARY_OP
}

static void resetStack() {
    vm.stackTop = vm.stack;
}

void initVM() {
    resetStack();
}

void freeVM() {}

void push(Value value) {
    *vm.stackTop = value;
    vm.stackTop++;
}

Value pop() {
    vm.stackTop--;
    return *vm.stackTop;
}

InterpretResult interpret(Chunk* chunk) {
    vm.chunk = chunk;
    vm.ip = vm.chunk->code;
    return run();
}