#ifndef tipo_chunk_h
#define tipo_chunk_h

#include "common.h"
#include "value.h"

typedef enum {
    OP_CONSTANT,
    OP_RETURN,
} OpCode;

typedef struct {
    int start_offset;
    int line;
    int count;
} Line;

typedef struct {
    int count;
    int capacity;
    uint8_t* code;

    int line_capacity;
    int line_count;
    Line* lines;
    ValueArray constants;
} Chunk;

void initChunk(Chunk* chunk);
void freeChunk(Chunk* chunk);
void writeChunk(Chunk* chunk, uint8_t byte, int line);

int addConstant(Chunk* chunk, Value value);

void writeLine(Chunk* chunk, int offset, int line);
int getLine(Chunk* chunk, int offset);

#endif
