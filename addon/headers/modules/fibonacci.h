#ifndef __MODULES_FIBONACCI_H__
#define __MODULES_FIBONACCI_H__

#include <node.h>

using v8::FunctionCallbackInfo;
using v8::Isolate;
using v8::String;
using v8::Integer;
using v8::Value;
using v8::Exception;

/**
 * @brief The exported fibonacci function, it will calculate recursevely the nth fibonacci number and return it.
 * 
 * @param args The v8 args.
 */
void Fibonacci(const FunctionCallbackInfo<Value> &args);

#endif