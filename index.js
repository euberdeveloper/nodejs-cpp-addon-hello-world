const bindings = require('bindings');
const addon = bindings('addon');

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

console.log = () => {}

test.start();
for (let i = 0; i < 1e8; i++) {
    console.log(addon.hello());
}
test.end();
const elapsed1 = test.elapsed;
console.debug('addon', elapsed1);

test.start();
for (let i = 0; i < 1e8; i++) {
    console.log('world');
}
test.end();
const elapsed2 = test.elapsed;
console.debug('js', elapsed2);


console.debug('speedup', elapsed1 / elapsed2);