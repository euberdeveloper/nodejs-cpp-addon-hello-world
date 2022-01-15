#ifndef __MODULES_HELLO_H__
#define __MODULES_HELLO_H__

#include <node.h>

using v8::FunctionCallbackInfo;
using v8::Isolate;
using v8::String;
using v8::Value;

/**
 * @brief The exported hello function, it will return the string "world".
 * 
 * @param args The v8 args.
 */
void Hello(const FunctionCallbackInfo<Value> &args);

#endif