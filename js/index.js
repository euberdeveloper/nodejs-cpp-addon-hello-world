module.exports = {
    hello() {
        return 'world';
    },
    fibonacci(n) {
        function fibonacci(n) {
            return n <= 1 ? n : fibonacci(n - 1) + fibonacci(n - 2);
        }
        return () => fibonacci(n);
    },
    sum() {
        const result = 1 + 1;
        return result;
    },
    multisum(n) {
        return () => {
            let result = 0;
            for (let i = 0; i < n; i++) {
                result += i;
            }
            return result;
        }
    }
}