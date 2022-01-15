module.exports = {
    hello() {
        return 'world';
    },
    fibonacci(n) {
        return n <= 1 ? n : this.fibonacci(n - 1) + this.fibonacci(n - 2);
    }
}