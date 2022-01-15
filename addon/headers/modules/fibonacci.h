#ifndef __MODULES_FIBONACCI_H__
#define __MODULES_FIBONACCI_H__

#include <node.h>

using v8::FunctionCallbackInfo;
using v8::Isolate;
using v8::Integer;
using v8::Value;

/**
 * @brief The exported fibonacci35 function, it will calculate recursevely the 35th fibonacci number and return it.
 * 
 * @param args The v8 args.
 */
void Fibonacci35(const FunctionCallbackInfo<Value> &args);

/**
 * @brief The exported fibonacci40 function, it will calculate recursevely the 40th fibonacci number and return it.
 * 
 * @param args The v8 args.
 */
void Fibonacci40(const FunctionCallbackInfo<Value> &args);

#endif