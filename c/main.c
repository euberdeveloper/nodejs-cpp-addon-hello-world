#include <stdio.h>
#include <time.h>
#include <sys/time.h>

long long int get_current_timestamp() {
    struct timeval tv;
    if (!gettimeofday(&tv, NULL)) {
        return (long long int)(tv.tv_sec) * 1000ll + (long long int)(tv.tv_usec) / 1000ll;
    } else {
        return -1;
    }
}

int multisum(int n) {
    int result = 0;
    for (int i = 0; i < n; i++) {
        result += i;
    }
    return result;
}

int main() {
    long long int start = get_current_timestamp();

    int result = 0;
    for (int i = 0; i < 1e3; i++) {
        result += multisum(1e7);
    }
    printf("result: %d\n", result);

    long long int end = get_current_timestamp();
    printf("Multisum 1e7 for 1e3 iterations took %lld millis\n", end - start);

    return 0;
}