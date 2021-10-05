#ifndef tipo_value_h
#define tipo_value_h

#include "common.h"

typedef double Value;

/**
 * 常量池结构体，用来存储代码中所有声明的常量
 */
typedef struct {
    /**
     * 常量池容量
     */
    int capacity;
    /**
     * 常量池当前存储数量
     */
    int count;

    /**
     * 存储常量池所有的值，通过下标访问。
     * 声明常量的语句应该存储该下标。
     * 目前仅支持 double 类型
     */
    Value* values;
} ValueArray;

void initValueArray(ValueArray* array);
void writeValueArray(ValueArray* array, Value value);
void freeValueArray(ValueArray* array);
void printValue(Value value);

#endif