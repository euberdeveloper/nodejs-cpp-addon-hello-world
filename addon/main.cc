#include <node.h>
#include "./headers/modules/hello.h"
#include "./headers/modules/fibonacci.h"
#include "./headers/modules/sum.h"

using v8::Local;
using v8::Object;

void Initialize(Local<Object> exports)
{
    NODE_SET_METHOD(exports, "hello", Hello);
    NODE_SET_METHOD(exports, "fibonacci", Fibonacci);
    NODE_SET_METHOD(exports, "sum", Sum);
    NODE_SET_METHOD(exports, "sum1e7", Sum1e7);
}

NODE_MODULE(NODE_GYP_MODULE_NAME, Initialize)