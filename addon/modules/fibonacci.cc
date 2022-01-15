#include "../headers/modules/fibonacci.h"
#include "../headers/utils/fibonacci.h"

/* HELPERS DECLARATION */

static void Fibonacci(int result, const FunctionCallbackInfo<Value> &args);

/* EXPORTED IMPLEMENTATION */

void Fibonacci35(const FunctionCallbackInfo<Value> &args)
{
    int result = fibonacci(35);
    Fibonacci(result, args);
}
void Fibonacci40(const FunctionCallbackInfo<Value> &args)
{
    int result = fibonacci(40);
    Fibonacci(result, args);
}

/* HELPERS IMPLEMENTATION */

static void Fibonacci(int result, const FunctionCallbackInfo<Value> &args)
{
    Isolate *isolate = args.GetIsolate();
    args.GetReturnValue().Set(Integer::New(isolate, result));
}