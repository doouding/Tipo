#include "common.h"
#include "chunk.h"
#include "debug.h"
#include "vm.h"

int main(int argc, const char* argv[]) {
    initVM();

    Chunk chunk;
    initChunk(&chunk);

    writeChunk(&chunk, OP_CONSTANT_LONG, 1);
    writeConstant(&chunk, 1.2, 1);
    writeChunk(&chunk, OP_CONSTANT_LONG, 1);
    writeConstant(&chunk, 2.4, 1);
    writeChunk(&chunk, OP_NEGATE, 1);
    writeChunk(&chunk, OP_ADD, 1);
    writeChunk(&chunk, OP_RETURN, 2);

    interpret(&chunk);
    freeVM();
    freeChunk(&chunk);
    return 0;
}
