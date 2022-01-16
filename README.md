# nodejs-cpp-addons-first-approach
This is my first experience with a **C++ addon** for **Node.js**.

## Introduction

Node.js is known to be effective for tasks like **handling several HTTP requests**, thanks to its events loop. It is also known to be (relatively) **fast**, but it remains an **interpreted language** and **can not compete** with compiled languages such as **C**, **Rust** or **Go**. The performance problems of Node.js are expecially noticeable when dealing with **heavy computations**.

## C++ addons

Node.js is written in **C++** (and Javascript), based on the **V8 engine**, the same engine that powers **Chrome** for executing Javascript. Node.js supports an interoperability beween **C** and **Javascript**, so that parts of the code can be written in a compiled language in order to be faster.

There are four ways to develop a C++ addon for Node.js:
* By using directly the **internal V8, libuv and Node.js libraries**
* By using **nan** (Native Abstractions for Node.js)
* By using **Node-API**
* By using **node-addon-api**

### Using the internal V8, libuv and Node.js libraries

Using the internal V8, libuv and Node.js libraries is totally discouraged. First of all, because it is the **most low-level** way, so the code would be very **verbose** and **difficult to handle**. But the **biggest reason** not to use it is that there can be **huge differences** between **different Node.js versions**, expecially due to changes in the V8 engine. This means that the code that you will write, if it is not using just the most basic functionalities, **will probably work just on your current Node.js version**. This should be used only if you need to use **functionalities not available in the other methods**.

### Using nan (Native Absractions for Node.js)

**nan** (which in this case, is not not-a-number), is an **abstraction layer** between your code and the previous citated libraries. The aim of nan is providing a **stable ABI**, so that the code that you write will be at least **forward compatible** with future versions of Node.js. Being an abstraction layer, the code will be also **quite simpler** than the one of the previous method. The problem with this method is that, even if the **code is forward compatible** with future versions, the **compiled binaries are not**, so they need a **recompilation**. The newer 
**Node-API is a better alternative**, so the biggest reason to use **nan** is if you need functionalities that **are not available on the Node-API** or if you need your code to work with **very old versions of Node.js**, that don't support Node-API.

### Using Node-API

**Node-API** is **not a layer** between your code and the internal libraries used by Node.js itself. It is an **ad-hoc API** made to make writing native **addons way easier and more convenient**. Being designed exactly for this purpose, it is the **nicest way** to write addons. The most important thing is that **not only the code, but also the binaries are forward compatible** with future versions of Node.js.

### Using node-addon-api

**node-addon-api** is exactly **the same of Node-API**, with the difference that the **code becomes easier**. It is just a **header wrapper**, that has consequently **exactly the same functionalities**, that **furtherly simplifies the code**. This should be **the preferred way** to write addons for Node.js.


## How are addons built?

Node.js addons are build with **node-gyp**, a node-js flavour of Google's **GYP** building system. **GYP** is an **obsolete build automation** tool that was used, among the others, to build Chromium, V8 and Dart. In 2016 these projects migrated to **GN**, a far quicker tool that obtains the same result. While **GYP** generates **Make** files, **GN** generates **Ninja** files. **Node.js** still relies on this obsolete tool (of course through the still-maintained **node-gyp**) to build native modules. As of today (I dare you to look through the commits' history to understand when is today), also **Telegram** is using **GYP** to build its native modules.

GYP takes a **binding.gyp** file as configuration and generates a **.node** file, that can be **directly imported in js code**, as if it were a normal javascript module. It will generate with the **configuration command** a **Makefile** and other building files and, with the **build** command, it will generated the **compiled module**.

The **binding.gyp**, written in **json**, looks like this:

```json
{
  "targets": [
    {
      "target_name": "addon",
      "sources": [
        "addon/main.cc",
        "addon/utils/fibonacci.cc",
        "addon/utils/sum.cc",
        "addon/modules/hello.cc",
        "addon/modules/fibonacci.cc",
        "addon/modules/sum.cc"
      ]
    }
  ]
}
```

Where "sources" indicates the source c++ files and target_name will generate **addon.node**. This output file is usually placed on **build/Release** or **build/Debug**.

It will be **during the installation of a native package** that the native code will be **compiled**. The destination of the compiled addon is not always the same, so the **safest way** to **import it** is by using the **[bindings](https://github.com/TooTallNate/node-bindings)** module.

## My first approach

In my first approach, I followed the **[first documentation on the official Node.js website](https://nodejs.org/api/addons.html)**, by choosing the **hardest way**, so directly the interlal node.js and V8 libraries. I did this because it would have been probably the **first and last** times that I would have done it (and I was right).

What I wanted to know was: **does it really make sense** to loose time writing C++ code? Are the **performance benefits worth it**? 

So I **developed some C++ addons** and **javascript analogues** and **compared their performance**.

The **main.cc** file looks like this:

```cpp
#include <node.h>
#include "./headers/modules/hello.h"
#include "./headers/modules/fibonacci.h"
#include "./headers/modules/sum.h"

using v8::Local;
using v8::Object;

void Initialize(Local<Object> exports)
{
    NODE_SET_METHOD(exports, "hello", Hello);
    NODE_SET_METHOD(exports, "fibonacci35", Fibonacci35);
    NODE_SET_METHOD(exports, "fibonacci40", Fibonacci40);
    NODE_SET_METHOD(exports, "sum", Sum);
    NODE_SET_METHOD(exports, "sum1e7", Sum1e7);
}

NODE_MODULE(NODE_GYP_MODULE_NAME, Initialize)
```

while the **modules c++** files look like this:

```cpp
#include "../headers/modules/hello.h"

void Hello(const FunctionCallbackInfo<Value> &args)
{
    Isolate *isolate = args.GetIsolate();
    args.GetReturnValue().Set(String::NewFromUtf8(isolate, "world").ToLocalChecked());
}
```
This is the ugly code that you will write if you choose the bad way.

### hello bechmark

The hello bechmark is the same of the official documentation: **it is just a function that returns the string `world`**.

```js
 hello() {
    return 'world';
}
```

In the **benchmarks** I compared both the addon and the js implementations first **by just calling the function 10^6 times** and then by **writing the result in a file (/dev/null) 10^6 times**. 

| **BENCHMARK**   | **ITERATIONS** | **JS TIME** | **ADDON TIME** | **WINNER**                          |
| --------------- | -------------- | ----------- | -------------- | ----------------------------------- |
| HELLO           | 10^6           | 7ms         | 67ms           | *Javascript* is *9.57* times faster |
| HELLO OVER FILE | 10^6           | 509ms       | 428ms          | *Javascript* is *1.18* times faster |

**What happened?**. After all the fatigue made to create the addon, **Javascript** is faster. The reason is simple: **loading the addon introduces a small overhead**. This overhead does not make sense in cases like this, where the **function computation cost is irrelevant**. The reason why Javascript is almost **tenfold faster** is because **the overhead for loading tha addon is about tenfold the function's execution time itself**.

If we run the same javascript function agains this **other javascript** function:

```js
function hello() {
    function hi() {
        function servus() {
            function ciao() {
                return 'world';
            }
            return ciao();
        }
        return servus();
    }
    return hi();
}
```

We can see that the **second version** is **1.33 times slower** and this is because of the **function calls overhead**, a similar readon of why, in this case, the addon is slower.

**But why in writing in the file the gap was smaller?**. Here the reason is obvious. The **most computational expensive part** in the second benchmark **is the file writing**, not the string creation. If you look at the **times**, for the same number of iterations the **second benchmark** was definitely slower. It is as if we took the first benchmark times and added 500 millis. For numbers bigger than 1, `x > y; x / y` is bigger than `(x + 500) / (y + 500)`. This can be interpreted as: **the addon is slower to the same extent of the first benchmark**, but this overhead is **almost negligible**, because **the bottleneck here is the file writing**.

### fibonacci benchmark

This benchmark covers a **very stupid implementation** of the **fibonacci** algorithm. 

```cpp
int fibonacci(int n)
{
    return n <= 1 ? n : fibonacci(n - 1) + fibonacci(n - 2);
}
```

This recursive function is **exponential**, while it could be just linear by using **Dynamic programming** and **Memoization**. With even quite small arguments, it involves **heavy computations**.

I tested the computation of `fibonacci(35)` and `fibonacci(40)` with `5` iterations:

| **BENCHMARK** | **ITERATIONS** | **JS TIME** | **ADDON TIME** | **WINNER**                     |
| ------------- | -------------- | ----------- | -------------- | ------------------------------ |
| fibonacci(35) | 5              | 500ms       | 116ms          | *Addon* is *4.31* times faster |
| fibonacci(40) | 5              | 12279ms     | 1284ms         | *Addon* is *9.56* times faster |

This time we can see, how it was expected, that **the addon is way faster than javascript**. Here we can see the power of compiled (and optimized) languages such as C++. The **addon** is **4 times faster for fibonacci(35)** and **almost 10 times faster for fibonacci(40)**. Again, this gab between the two tests is just **an illusion**: being the algorithm **exponential**, **by increasing** even slightly **the input**, the **number of computations increase in an exponential way**. Being C++ faster, the gap increases exponentially by increasing the input.

Here we can see how with **heavy computation functions**, **native addons make sense**.

### sum benchmark

In this benchmark I made something that **consolidates** what learnt from the two other tests. I wrote two functions.

The first function is just **the sum of 1 + 1**:

```cpp
int sum()
{
    int result =  1 + 1;
    return result;
}
```

While in the other there is **the sum of he first n natural numbers**.

```cpp
int multisum(int n) {
    int result = 0;
    for (int i = 0; i < n; i++) {
        result += i;
    }
    return result;
}
```

While the first one executes **a single sum for 10^7 times**, while the second one executes **10^7 sums for one time**.

The results are:

| **BENCHMARK** | **ITERATIONS** | **JS TIME** | **ADDON TIME** | **WINNER**                          |
| ------------- | -------------- | ----------- | -------------- | ----------------------------------- |
| sum           | 10^7           | 63ms        | 293ms          | *Javascript* is *4.65* times faster |
| multisum(1e7) | 1              | 12ms        | 1ms            | *Addon* is *12* times faster        |

As we can see, **the function behind is the same**, it is the sum, but the fact that **all the addon sums are executed together**, in just one call, or if **they are executed one by call** makes the difference. **If they are executed in one call, the addon is 12 times faster than javascript**, **otherwise, it is 4 times slower**.

### when to use the addon

This explains well that **C++ addons should be used just for computational heavy tasks**, while they are **even slower if the task is simple**, due to a **small overhead, which is negligible if the algorithm is complex**. This means that **one can not just think of writing simple code in C++ and calling it by complex javascript code to make programs faster**, but **the C++ code has to execute a quite complex task in itself in order to have a benefit**. 

One of these cases is **cryptography**, something that **requires big computations** but that is much used, for instance, in **Rest APIs**, to **check hashed passwords or json web tokens**. One of the **few npm packages written in C++** is indeed **[bcrypt](https://github.com/kelektiv/node.bcrypt.js#readme)**.

### But is the C++ addon's code as faster as normal C code?

Excluding the small **loading overhead** and the **Node.js C++ functions wrapping overhead**, the **C++ addon code is just normal C++ code**, so it has **the same speed as normal C code**.

To check this, I wrote the **multisum** function in a normal **C** file and compiled it with **gcc**. 

Note that the `get_current_timestamp` implementation is omitted.

```c
#include <stdio.h>
#include <time.h>
#include <sys/time.h>

long long int get_current_timestamp();

int multisum(int n)
{
    int result = 0;
    for (int i = 0; i < n; i++)
    {
        result += i;
    }
    return result;
}

int main()
{
    long long int start = get_current_timestamp();

    int result = 0;
    result += multisum(1e7);
    printf("result: %d\n", result);

    long long int end = get_current_timestamp();
    printf("Multisum 1e7 took %lld millis\n", end - start);

    return 0;
}
```

By running it we can obtain `Multisum 1e7 took 23 millis`.

If we look at the **benchmark**, we can see that the **addon code took just 1ms**, while the **javascript code took 12ms**. **How is this possible that javascript is 2 times faster than normal C?**. It is because this **C file was not compiled with the optimization flag**.

By running it with a `gcc -O3` flag, we can obatin `Multisum 1e7 took 1 millis`, which is **exactly the same performance of the addon**.

The fact that **javascript was even faster than the not-optimized C code** should in any case make you think that **javascript is not so slow** and that the **V8 engine is highly optimized**, making Javascript, **of course not always**, quite **faster than other interpreted languages** such as python, PHP, Ruby and Perl.