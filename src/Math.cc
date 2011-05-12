#include <limits.h>

#include "Math.h"



bool between_equal(u_int32_t left, u_int32_t number, u_int32_t right) {
    return  (less_than(left, number) || (left == number)) &&
            (less_than(number, right) || (number == right));
}

bool between_equal_right(u_int32_t left, u_int32_t number, u_int32_t right) {
    return  less_than(left, number) &&
            (less_than(number, right) || (number == right));
}

bool between_equal_left(u_int32_t left, u_int32_t number, u_int32_t right) {
    return  (less_than(left, number) || (left == number)) &&
            less_than(number, right);
}

bool between(u_int32_t left, u_int32_t number, u_int32_t right) {
    return  less_than(left, number) && less_than(number, right);
}

bool less_than(u_int32_t lhs, u_int32_t rhs) {
    // INT_MAX is half of the unsigned space
    return (lhs < rhs) && ((rhs - lhs) <= INT_MAX) ||
           (lhs > rhs) && ((lhs - rhs) >  INT_MAX);
}
