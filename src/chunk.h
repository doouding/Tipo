#ifndef tipo_chunk_h
#define tipo_chunk_h

#include "common.h"
#include "value.h"

typedef enum {
    OP_CONSTANT_LONG,
    OP_CONSTANT,
    OP_ADD,
    OP_SUBTRACT,
    OP_MULTIPLY,
    OP_DIVIDE,
    OP_NEGATE,
    OP_RETURN,
} OpCode;

/**
 * 行号信息
 */
typedef struct {
    /**
     * 起始 opcode 的 offset，
     * 第一个行号为该行的 opcode 的 offset
     */
    int start_offset;
    /**
     * 有多少个 opcode 是这个行号
     */
    int count;
    /**
     * 行号
     */
    int line;
} Line;

/**
 * 块结构体，用来存储所有编译后的运算码（opcode）
 */
typedef struct {
    /**
     * 块的容量
     */
    int capacity;
    /**
     * 当前存储的 opcode 的数量
     */
    int count;
    /**
     * 所有的 opcode 存储在一个线性内存里
     * 每个 opcode 都用一个 uint8 数字表示
     */
    uint8_t* code;

    /**
     * 下面三个成员用来存储行号信息
     */
    /**
     * 行存储容量
     */
    int line_capacity;
    /**
     * 已存储行的数量
     */
    int line_count;
    /**
     * 已经存储的行号信息
     */
    Line* lines;

    ValueArray constants;
} Chunk;

/**
 * 初始化 Chunk
 * @param chunk
 */
void initChunk(Chunk* chunk);
/**
 * 释放 Chunk 内存
 * @param chunk
 */
void freeChunk(Chunk* chunk);
/**
 * 向 Chunk 写入 opcode
 * @param chunk
 * @param byte opcode
 * @param line opcode 所属行号
 */
void writeChunk(Chunk* chunk, uint8_t byte, int line);

/**
 * 向常量池中写入一个常量值
 * @param chunk
 * @param value 要写入的值
 * @param line 所属行号
 */
void writeConstant(Chunk* chunk, Value value, int line);

/**
 * 查询 opcode 所属的行号
 * @param chunk 要查询的 Chunk
 * @param offset opcode 存储的 offset
 * @return
 */
int getLine(Chunk* chunk, int offset);

#endif
