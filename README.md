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

GYP takes a **binding.gyp** file as configuration and generates a **.node** file, that can be **directly imported in js code**, as if it were a normal javascript module.

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


