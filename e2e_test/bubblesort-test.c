#include "bootstrap.h"

rv64dw array[] = {
    5, 4, 6, 8, 3, 6, 2, 7, 9, 4, 2, 5, 7, 8, 9, 4, 2, 2, 9,
    5, 4, 6, 8, 3, 6, 2, 7, 9, 4, 2, 5, 7, 8, 9, 4, 2, 2, 9,
    5, 4, 6, 8, 3, 6, 2, 7, 9, 4, 2, 5, 7, 8, 9, 4, 2, 2, 9,
    5, 4, 6, 8, 3, 6, 2, 7, 9, 4, 2, 5, 7, 8, 9, 4, 2, 2, 9,
    5, 4, 6, 8, 3, 6, 2, 7, 9, 4, 2, 5, 7, 8, 9, 4, 2, 2, 9,
    5, 4, 6, 8, 3, 6, 2, 7, 9, 4, 2, 5, 7, 8, 9, 4, 2, 2, 9,
    5, 4, 6, 8, 3, 6, 2, 7, 9, 4, 2, 5, 7, 8, 9, 4, 2, 2, 9,
    5, 4, 6, 8, 3, 6, 2, 7, 9, 4, 2, 5, 7, 8, 9, 4, 2, 2, 9,
    5, 4, 6, 8, 3, 6, 2, 7, 9, 4, 2, 5, 7, 8, 9, 4, 2, 2, 9,
    5, 4, 6, 8, 3, 6, 2, 7, 9, 4, 2, 5, 7, 8, 9, 4, 2, 2, 9,
};

void bubble_sort(rv64dw* array, rv64dw size) {
    rv64dw* last = array + size - 1;
    for(rv64dw* i = array; i < last; ++i) {
        for(rv64dw* j = array; j < last; ++j) {
            rv64dw* cur = j;
            rv64dw* next = j + 1;

            if(*cur < *next) {
                rv64dw tmp = *cur;
                *cur = *next;
                *next = tmp;
            }
        }
    }
}

int validate_sort(rv64dw const* array, rv64dw size) {
    for(rv64dw const* i = array; i < array + size - 1; ++i) {
        if(*i < *(i + 1)) {
            return 0;
        }
    }
    return 1;
}

rv64dw start() {
    bubble_sort(array, sizeof(array) / sizeof(array[0]));
    return validate_sort(array, sizeof(array) / sizeof(array[0]));
}
