#include <node.h>
#include "./headers/fibonacci.h"

using v8::FunctionCallbackInfo;
using v8::Integer;
using v8::Isolate;
using v8::Local;
using v8::Object;
using v8::String;
using v8::Value;

void Fibonacci(int result, const FunctionCallbackInfo<Value> &args)
{
    Isolate *isolate = args.GetIsolate();
    args.GetReturnValue().Set(Integer::New(isolate, result));
}

void Fibonacci30(const FunctionCallbackInfo<Value> &args)
{
    int result = fibonacci(30);
    Fibonacci(result, args);
}
void Fibonacci35(const FunctionCallbackInfo<Value> &args)
{
    int result = fibonacci(35);
    Fibonacci(result, args);
}
void Fibonacci37(const FunctionCallbackInfo<Value> &args)
{
    int result = fibonacci(37);
    Fibonacci(result, args);
}
void Fibonacci40(const FunctionCallbackInfo<Value> &args)
{
    int result = fibonacci(40);
    Fibonacci(result, args);
}
void Fibonacci42(const FunctionCallbackInfo<Value> &args)
{
    int result = fibonacci(42);
    Fibonacci(result, args);
}


void Initialize(Local<Object> exports)
{
    NODE_SET_METHOD(exports, "fibonacci30", Fibonacci30);
    NODE_SET_METHOD(exports, "fibonacci35", Fibonacci35);
    NODE_SET_METHOD(exports, "fibonacci37", Fibonacci37);
    NODE_SET_METHOD(exports, "fibonacci40", Fibonacci40);
    NODE_SET_METHOD(exports, "fibonacci42", Fibonacci42);
}

NODE_MODULE(NODE_GYP_MODULE_NAME, Initialize)