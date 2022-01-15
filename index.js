const bindings = require('bindings');
const addon = bindings('addon');

function fibonacci(n) {
    return (n <= 1) ? n : fibonacci(n - 1) + fibonacci(n - 2);
}

class Test {
    #start = null;
    #end = null;

    start() {
        this.#end = null;
        this.#start = Date.now();
    }

    end() {
        if (this.#start) {
            this.#end = Date.now();
        }
        else {
            throw new Error('impossibile finire una cosa non iniziata');
        }
    }

    get elapsed() {
        return this.start && this.end ? this.#end - this.#start : null;
    }
}
const test = new Test();

console.log = () => { }

test.start();
for (let i = 0; i < 3; i++) {
    addon.fibonacci42();
}
test.end();
const elapsed1 = test.elapsed;
console.debug('addon', elapsed1);

test.start();
for (let i = 0; i < 3; i++) {
    fibonacci(42);
}
test.end();
const elapsed2 = test.elapsed;
console.debug('js', elapsed2);


console.debug('speedup', elapsed2 / elapsed1);