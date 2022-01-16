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

### hello test

The hello test is the same of the official documentation: **it is just a function that returns the string `world`**.

```js
 hello() {
    return 'world';
}
```

In the **benchmarks** I compared both the addon and the js implementations first **by just calling the function 10^6 times** and then by **writing the result in a file (/dev/null) 10^6 times**. 

| **TEST**        | **ITERATIONS** | **JS TIME** | **ADDON TIME** | **WINNER**                          |
|-----------------|----------------|-------------|----------------|-------------------------------------|
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

**But why in writing in the file the gap was smaller?**. Here the reason is obvious. The **most computational expensive part** in the second test **is the file writing**, not the string creation. If you look at the **times**, for the same number of iterations the **second test** was definitely slower. It is as if we took the first test times and added 500 millis. For numbers bigger than 1, `x > y; x / y` is bigger than `(x + 500) / (y + 500)`. This can be interpreted as: **the addon is slower to the same extent of the first test**, but this overhead is **almost negligible**, because **the bottleneck here is the file writing**.

### fibonacci test



