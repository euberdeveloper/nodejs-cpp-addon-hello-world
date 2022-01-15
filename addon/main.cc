#include <node.h>
#include "./headers/modules/hello.h"
#include "./headers/modules/fibonacci.h"

using v8::Local;
using v8::Object;

void Initialize(Local<Object> exports)
{
    NODE_SET_METHOD(exports, "hello", Hello);
    NODE_SET_METHOD(exports, "fibonacci35", Fibonacci35);
    NODE_SET_METHOD(exports, "fibonacci40", Fibonacci40);
}

NODE_MODULE(NODE_GYP_MODULE_NAME, Initialize)