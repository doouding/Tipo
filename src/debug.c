#include <stdio.h>
#include <string.h>

#include "debug.h"
#include "value.h"

void disassembleChunk(Chunk* chunk, const char* name) {
    printf("== %s ==\n", name);

    for (int offset = 0; offset < chunk->count;) {
        offset = disassembleInstruction(chunk, offset);
    }
}

static int simpleInstruction(const char* name, int offset) {
    printf("%s\n", name);

    return offset + 1;
}

static int constantInstruction(const char* name, Chunk* chunk, int offset) {
    // 获取常量所存储的索引
    int constant_idx;
    int is_long_constant = strcmp(name, "OP_CONSTANT_LONG") == 0 ? 1 : 0;

    if(is_long_constant) {
        constant_idx = (int)chunk->code[offset + 1]
                     | ((int)chunk->code[offset + 2] << 8)
                     | ((int)chunk->code[offset + 3] << 16);
    } else {
        constant_idx = chunk->code[offset + 1];
    }

    printf("%-16s %4d ", name, constant_idx);
    printValue(chunk->constants.values[constant_idx]);
    printf("\n");

    return is_long_constant ? offset + 4 : offset + 2;
}

int disassembleInstruction(Chunk* chunk, int offset) {
    printf("%04d ", offset);
    int lineno = getLine(chunk, offset);

    if(offset > 0 && getLine(chunk, offset - 1) == lineno) {
        printf("   | ");
    } else {
        printf("%4d ", lineno);
    }

    uint8_t instruction = chunk->code[offset];

    switch (instruction) {
        case OP_NEGATE:
            return simpleInstruction("OP_NEGATE", offset);
        case OP_ADD:
            return simpleInstruction("OP_ADD", offset);
        case OP_SUBTRACT:
            return simpleInstruction("OP_SUBTRACT", offset);
        case OP_MULTIPLY:
            return simpleInstruction("OP_MULTIPLY", offset);
        case OP_DIVIDE:
            return simpleInstruction("OP_DIVIDE", offset);
        case OP_RETURN:
            return simpleInstruction("OP_RETURN", offset);
        case OP_CONSTANT:
            return constantInstruction("OP_CONSTANT", chunk, offset);
        case OP_CONSTANT_LONG:
            return constantInstruction("OP_CONSTANT_LONG", chunk, offset);
        default:
            printf("Unknown opcode %d\n", instruction);
            return offset + 1;
    }
}
