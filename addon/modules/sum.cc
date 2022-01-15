#include "../headers/modules/sum.h"
#include "../headers/utils/sum.h"

/* HELPERS DECLARATION */

static void ReturnResult(int result, const FunctionCallbackInfo<Value> &args);

/* EXPORTED IMPLEMENTATION */

void Sum(const FunctionCallbackInfo<Value> &args)
{
    int result = sum();
    ReturnResult(result, args);
}
void Sum1e7(const FunctionCallbackInfo<Value> &args)
{
    int result = multisum(1e7);
    ReturnResult(result, args);
}

/* HELPERS IMPLEMENTATION */

static void ReturnResult(int result, const FunctionCallbackInfo<Value> &args)
{
    Isolate *isolate = args.GetIsolate();
    args.GetReturnValue().Set(Integer::New(isolate, result));
}