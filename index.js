const addon = require('bindings')('addon');
const js = require('./js');

const fs = require('fs');
const writeStream = fs.createWriteStream('/dev/null');

function run(fun, iterations) {
    const start = Date.now();
    for (let i = 0; i < iterations; i++) {
        fun();
    }
    return Date.now() - start;
}

function benchmark(name, jsFun, addonFun, iterations) {
    console.log(`TEST: ${name} (${iterations} iterations)`);

    console.debug('Run javascript');
    const jsTime = run(jsFun, iterations);
    console.debug(`Time: ${jsTime}ms`);

    console.log('Run addon');
    const addonTime = run(addonFun, iterations);
    console.log(`Time: ${addonTime}ms`);

    const won = jsTime >= addonTime ? 'javascript' : 'addon';
    const fraction = jsTime >= addonTime ? jsTime / addonTime : addonTime / jsTime;
    console.log(`${won} is ${fraction} times slower`);

    console.log('\n\n');
}

benchmark('HELLO', js.hello, addon.hello, 1e6);

benchmark('HELLO', js.hello, addon.hello, 1e6);
benchmark('HELLO_2', () => writeStream.write(js.hello()), () => writeStream.write(addon.hello()), 1e6);

benchmark('FIBONACCI_35', () => js.fibonacci(35), () => addon.fibonacci35(), 5);
benchmark('FIBONACCI_40', () => js.fibonacci(40), () => addon.fibonacci40(), 5);