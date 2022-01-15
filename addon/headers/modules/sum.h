#ifndef __MODULES_SUM_H__
#define __MODULES_SUM_H__

#include <node.h>

using v8::FunctionCallbackInfo;
using v8::Isolate;
using v8::Integer;
using v8::Value;

/**
 * @brief The exported sum function, it will return the sum of 1 + 1
 * 
 * @param args The v8 args.
 */
void Sum(const FunctionCallbackInfo<Value> &args);

/**
 * @brief The exported sum1e7 function, it will calculate the sum of 1 + 1 for 10^7 times
 * 
 * @param args The v8 args.
 */
void Sum1e7(const FunctionCallbackInfo<Value> &args);

#endif