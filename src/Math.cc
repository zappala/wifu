#include "Math.h"

bool between_equal(u_int32_t left, u_int32_t number, u_int32_t right) {
    if (right >= left) {
        return number >= left && number <= right;
    }

    return number <= right || number >= left;
}

bool between_equal_right(u_int32_t left, u_int32_t number, u_int32_t right) {
    if (right >= left) {
        return number > left && number <= right;
    }

    return number <= right || number > left;
}

bool between_equal_left(u_int32_t left, u_int32_t number, u_int32_t right) {
    if (right >= left) {
        return number >= left && number < right;
    }

    return number < right || number >= left;
}

bool between(u_int32_t left, u_int32_t number, u_int32_t right) {
    if (right >= left) {
        return number > left && number < right;
    }

    return number < right || number > left;
}