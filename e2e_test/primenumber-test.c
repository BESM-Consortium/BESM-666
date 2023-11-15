#include "bootstrap.h"

rv64dw __umoddi3(rv64dw value, rv64dw div) {
    while(value >= div) {
        value -= div;
    }
    return value;
}

int is_prime(rv64dw value) {
    for(rv64dw i = 2; i < value / 2; ++i) {
        if(value % i == 0) {
            return 0;
        }
    }
    return 1;
}

rv64dw start() {
    return is_prime(1787); //155003 or 65003 for long count
}
