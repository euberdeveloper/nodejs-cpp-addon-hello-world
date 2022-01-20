#include "../headers/modules/fibonacci.h"
#include "../headers/utils/fibonacci.h"

void Fibonacci(const FunctionCallbackInfo<Value> &args)
{
    Isolate *isolate = args.GetIsolate();

    if (args.Length() < 1)
    {
        isolate->ThrowException(Exception::TypeError(
            String::NewFromUtf8(isolate, "Wrong number of arguments").ToLocalChecked()));
        return;
    }

    if (!args[0]->IsNumber())
    {
        isolate->ThrowException(Exception::TypeError(
            String::NewFromUtf8(isolate, "Wrong arguments").ToLocalChecked()));
        return;
    }

    int n = args[0]->NumberValue(isolate->GetCurrentContext()).ToChecked();
    int result = fibonacci(n);

    args.GetReturnValue().Set(Integer::New(isolate, result));
}