#include <stdlib.h>

#include "chunk.h"
#include "memory.h"

void initChunk(Chunk* chunk) {
    chunk->count = 0;
    chunk->capacity = 0;
    chunk->code = NULL;

    chunk->line_count = 0;
    chunk->line_capacity = 0;
    chunk->lines = NULL;

    initValueArray(&chunk->constants);
}

void writeChunk(Chunk* chunk, uint8_t byte, int line) {
    if (chunk->capacity < chunk->count + 1) {
        int oldCapacity = chunk->capacity;
        chunk->capacity = GROW_CAPACITY(oldCapacity);
        chunk->code = GROW_ARRAY(uint8_t, chunk->code, oldCapacity, chunk->capacity);
    }

    writeLineNo(chunk, chunk->count, line);
    chunk->code[chunk->count] = byte;
    chunk->count++;
}

void freeChunk(Chunk* chunk) {
    FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
    FREE_ARRAY(Line, chunk->lines, chunk->line_capacity);
    freeValueArray(&chunk->constants);
    initChunk(chunk);
}

int addConstant(Chunk* chunk, Value value) {
    writeValueArray(&chunk->constants, value);
    return chunk->constants.count - 1;
}

void writeLineNo(Chunk* chunk, int offset, int line) {
    // 如果最后一个行信息的 line 和准备写入的 line 的行数一样
    // 则直接 count + 1 即可
    if(chunk->line_capacity > 0) {
        Line line_info = chunk->lines[chunk->line_count - 1];

        if(line_info.line == line) {
            line_info.count++;
            return;
        }
    }

    if(chunk->line_capacity < chunk->line_count + 1) {
        int oldCapacity = chunk->line_capacity;
        chunk->line_capacity = GROW_CAPACITY(oldCapacity);
        chunk->lines = GROW_ARRAY(Line, chunk->lines, oldCapacity, chunk->line_capacity);
    }

    chunk->lines[chunk->line_count].line = line;
    chunk->lines[chunk->line_count].start_offset = offset;
    chunk->lines[chunk->line_count].count = 1;
    chunk->line_count++;
}

int getLine(Chunk* chunk, int offset) {
    for(int i = 0; i < chunk->line_count; i++) {
        Line line_info = chunk->lines[i];

        if(offset >= line_info.start_offset && offset < line_info.start_offset + line_info.count) {
            return line_info.line;
        }
    }

    return -1;
}

/**
 * 写入 OP_CONSTANT_LANG 的 operand，小端序
 * @param chunk
 * @param value
 * @param line
 */
void writeConstant(Chunk* chunk, Value value, int line) {
    int index = addConstant(chunk, value);

    writeChunk(chunk, index | 0, line);
    writeChunk(chunk, index >> 8 | 0, line);
    writeChunk(chunk, index >> 16 | 0, line);
}
